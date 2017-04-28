/*
 * linux/drivers/video/ti81xx/vpss/system.c
 *
 * system PLL driver for TI81XX Platform
 *
 * Copyright (C) 2010 TI
 * Author: Yihe Hu <yihehu@ti.com>
 *
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place - Suite 330, Boston, MA	02111-1307, USA.
 */
#define VPSS_SUBMODULE_NAME   "SYSTEM"

#include <linux/module.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/err.h>
#include <linux/sysfs.h>
#include <linux/kobject.h>
#include <linux/dma-mapping.h>
#include <linux/slab.h>

#include "system.h"
#include "core.h"


struct vps_system_ctrl {
	u32       *handle;
	struct vps_systemvpllclk   *pllclk;
	u32                        pllclk_phy;
	u32                        *pid;
	u32                        pid_phy;
	void __iomem		  *pbase;
	struct vps_systemvpllclk   vpllcfg[3];
};

/*************************************************
*** PLL related structure and macro
*************************************************/
struct system_videopll {
	u32                  __n;
	/**< Divider N for the PLL.*/
	u32                  __m;
	/**< Multiplier M for the PLL.*/
	u32                  __m2;
	/**< Divider M2 for the PLL.*/
	u32                  clkcfg;
	/**< For comparison based on the clkOut used */
	u32                  __sd;
	/*sigma delta divider*/
};


/*VIDEO PLL Register offset*/
#define CLKCTRL                 0x4
#define TENABLE                 0x8
#define TENABLEDIV              0xC
#define M2NDIV                  0x10
#define MN2DIV                  0x14
#define FRACDIV			0x18
#define BWCTRL			0x1c
#define STATUS                  0x24
#define VIDPLL_SIZE		0x30

/*VIDEO PLL base offset*/
#define VIDPLL_OFFSET		0x1A0




/*DPLL Bandwidth control*/
#define	BWCTRL_DEFAULT     0
#define BWCTRL_1X          1
#define	BWCTRL_2X          3
#define	BWCTRL_HALF        2
#define	BWCTRL_QUARTER     4
#define	BWCTRL_EIGHTH      6

/* CLKCRTL_Register Value Configurations */
#define CLKCRTL_HS2              (0x00000801)
/*HS2 Mode,TINTZ =1   */
#define CLKCRTL_HS1              (0x00001001)
/* HS1 Mode,TINTZ =1  */
/*CLKCRTL_Register SPEC bug  bit 19,bit29 -- CLKLDOEN,CLKDCOEN */
#define CLKCRTL_CLKDCO           (0x20020000)
#define CLKCTRL_CLKLDO           (0x00080000)
/* Enable CLKDCOEN,CLKLDOEN,CLKDCOPWDNZ -- used for HDMI */
#define CLKCTRL_BYPASS		0x00800000
#define CLKCTRL_RESET		0x00000001


#define KHZ                  (1000)
#define MHZ                  (KHZ * KHZ)
#define TI814X_OSC_FREQ      (20 * MHZ)
#define VPS_PRCM_MAX_REP_CNT	100
#define HDMI_CLOCK		0x80000000
static struct vps_payload_info   *system_payload_info;
static struct vps_system_ctrl   *sys_ctrl;

static inline bool isvalidpllclk(struct vps_systemvpllclk *pllclk)
{
	if (pllclk->outputclk == 0)
		return false;

	switch (pllclk->outputvenc) {
	case VPS_SYSTEM_VPLL_OUTPUT_VENC_RF:
		if (v_pdata->cpu == CPU_DM816X) {
			if (pllclk->outputclk != 216000)
				return false;
		}
		break;
	case VPS_SYSTEM_VPLL_OUTPUT_VENC_A:
	case VPS_SYSTEM_VPLL_OUTPUT_VENC_D:
		break;
	case VPS_SYSTEM_VPLL_OUTPUT_VENC_HDMI:
		if (v_pdata->cpu == CPU_DM816X)
			return false;
		break;
	default:
		return false;
	}

	return true;
}

static inline bool ispllchanged(struct vps_systemvpllclk *pllclk)
{
	struct vps_systemvpllclk  *pllcfg;

	u32 idx, clk;

	idx = pllclk->outputvenc;
	clk = pllclk->outputclk;
	if (pllclk->outputvenc == VPS_SYSTEM_VPLL_OUTPUT_VENC_HDMI) {
		idx = VPS_SYSTEM_VPLL_OUTPUT_VENC_A;
		clk |= HDMI_CLOCK;
	}

	pllcfg = &sys_ctrl->vpllcfg[idx];
	if (pllcfg->outputclk != clk)
		return true;

	return false;
}
/******************************************************************************
****
****                                   ********* RANGE ************
****   REF_CLK       = (OSC_FREQ)/N+1  [  REF_CLK < 2.5MHz      ]
****   DCOCLK_HS2    = (REF_CLK)*M     [500  < DCOCLK < 1000MHz ]
****   DCOCLK_HS1    = (REF_CLK)*M     [1000 < DCOCLK < 2000MHz ]
****   CLKOUT        =  DCOCLK/M2      [10   < CLKOUT < 2000MHz ]
****   N+1                             [1..256]
****   M                               [2..4095]
****   M2                              [1..127]
****   SD            = ceiling(DCOCLK/250MHz)
****
*******************************************************************************/
/* Get the divider value for video PLL for the specified frequency.
   N requires to start with 19 may need change later */
static int system_getdividers(struct system_videopll *config,
			   u32 reqoutclk,
			   u32 venclk)
{
	int   r = -EINVAL, i;
	int   n, m, m2;
	u32   mod;
	u32   refclk, dcoclk, clkout;
	u8   hsMode = 1; /* 1: HS2, 2:HS1*/
	u8   dco = 0;
	u8   factor = 1;
	u32   padding;
	u8 nvalue[] = {7, 19};

	/* Work around for where it failed to set
	54k frequency with generated with the values generated by
	getdivider function - Essentially m = 540 and m2 == 10 will not
	work */
	if (venclk == VPS_SYSTEM_VPLL_OUTPUT_VENC_HDMI) {
		/*if Freq is less than 50MHz, post-M2 clock is used to drive
		HDMI, the other case pre-M2 clock is used,
		so we set factor to 10 if > 50MHz, since
		pre-M2 clock is to driver the HDMI
		other wise, we use factor to 2 to make PLL run at high speed
		to reduce jitter.
		check HDMI wrapper spec and TI814X Clock Arch document to
		further understand this*/
		if (reqoutclk == 54000) {
			factor = 20;
			reqoutclk *= 1000 * factor;
			dco = 1;
		} else {
			if (reqoutclk >= 50000) {
				factor = 10;
				/*CLK must be 10x if it is for HDMI*/
				reqoutclk *= 1000 * factor;
				dco = 1;
			} else {
				if (reqoutclk >= 30000)
					factor = 2;
				else
					factor = 6;
				/*since it is post-m2, so clk must be 10x first
				moreover, we want PLL run at high speed to
				reduce jitter so we set factor based on the
				clock*/
				reqoutclk *= 1000 * 10 * factor;
				dco = 0;
			}
		}
	} else {
		/* since PLL has less jitter if running at high speed,
		so we will manual speed up the PLL clock based on the
		input frequency*/
		if (reqoutclk == 54000)
			factor = 20;
		else if (reqoutclk > 250000)
			factor = 2;
		else if (reqoutclk > 180000)
			factor = 4;
		else if (reqoutclk > 80000)
			factor = 10;
		else if (reqoutclk > 50000)
			factor = 20;
		else if (reqoutclk > 40000)
			factor = 30;
		else if (reqoutclk > 30000)
			factor = 40;
		else if (reqoutclk > 20000)
			factor = 50;
		else
			factor = 80;

		reqoutclk *= 1000 * factor;
	}
	/*use n=7(2.5MHz), and 19(1MHz)*/
	for (i = 0; ((i < ARRAY_SIZE(nvalue)) && (r != 0)); i++) {
		n = nvalue[i];

		/*round up to 1M unit*/
		mod = (i == 0) ? MHZ >> 1 : MHZ;
		padding = reqoutclk % mod;
		if (padding) {
			if (padding >= (mod >> 1))
				reqoutclk += mod - padding;
			else
				reqoutclk -= padding;
		}
		VPSSDBG("request clk %dHz\n", reqoutclk);
		refclk = TI814X_OSC_FREQ / (n + 1);
		if (refclk > ((5 * MHZ) >> 1))
			return r;

		for (m = 2; ((m < 4095) && (r != 0)); m++) {
			dcoclk = (refclk) * m;
			if ((dcoclk > (500 * MHZ)) &&
			    (dcoclk < (2000 * MHZ)) &&
			    (dcoclk >= reqoutclk)) {
				/*set up the hsmode based on the freq*/
				hsMode = (dcoclk > 1000 * MHZ) ? 2 : 1;
				for (m2 = 1; m2 <= 127; m2++) {
					/*start over*/
					if ((m2 * factor) > 127)
						break;
					clkout = dcoclk / m2;
					if (clkout  == reqoutclk) {
						config->__n = n;
						config->__m = m;
						config->__m2 = m2 * factor;
						config->__sd =
							dcoclk / (250 * MHZ);
						if (dcoclk % (250 * MHZ))
							config->__sd += 1;
						if (config->__sd < 2)
							config->__sd = 2;
						else if (config->__sd > 255)
							config->__sd = 255;

						r = 0;
						break;
					}
				}
			}
		}

	}
	/*got the desired freq, set up everything*/
	if (!r) {
		if (hsMode == 1)
			config->clkcfg =
				CLKCRTL_HS2;
		else
			config->clkcfg =
				CLKCRTL_HS1;
		/*enable the CLKDCOLDO
		    and CLKOUTLDO for HDMI*/
		if (venclk ==
		    VPS_SYSTEM_VPLL_OUTPUT_VENC_HDMI) {
			if (dco)
				config->clkcfg |=
					CLKCRTL_CLKDCO;
			else
				config->clkcfg |=
					CLKCTRL_CLKLDO;
		}
	}
	return r;
}

/*program the PLL with desired divider*/
static int system_platformpllcfg(u32 base, u32 n, u32 m, u32 m2,
			u32 sd, u32 clkctrl)
{
	u32 m2nval, mn2val, read_clkctrl;
	u32 repeatcnt = VPS_PRCM_MAX_REP_CNT;
	u32 clkctrlval;

	clkctrlval = __raw_readl(base + CLKCTRL);
	/* Put the PLL in bypass mode */
	/* we would require a soft reset before we program the dividers */
	__raw_writel(((clkctrlval | CLKCTRL_BYPASS) & ~CLKCTRL_RESET),
		(base + CLKCTRL));
	udelay(3);
	while (((__raw_readl(base + STATUS)) & 0x101) != 0x101) {
		udelay(100);
		if (!--repeatcnt) {
			VPSSERR("Not able to put PLL in idle!!!\n");
			return -EINVAL;
		}

	}

	/* Program the PLL for required frequency */
	m2nval = (m2 << 16) | n;
	mn2val =  m;
	/*ref_clk     = OSC_FREQ/(n+1);
	clkout_dco  = ref_clk*m;
	clk_out     = clkout_dco/m2;
	*/
	__raw_writel(m2nval, (base + M2NDIV));
	__raw_writel(mn2val, (base + MN2DIV));
	__raw_writel(((__raw_readl(base + FRACDIV)) & 0xFFFFFF) | \
		((sd & 0xff) << 24), (base + FRACDIV));
	udelay(1);
	__raw_writel(0x1, (base + TENABLEDIV));
	udelay(1);
	__raw_writel(0x0, (base + TENABLEDIV));
	udelay(1);
	__raw_writel(0x1, (base + TENABLE));
	udelay(1);
	__raw_writel(0x0, (base + TENABLE));
	udelay(1);
	read_clkctrl = __raw_readl(base + CLKCTRL);
	/*configure the TINITZ(bit0) and CLKDCO bits
	bit29 and bit 19 and bit 17,
	HS1 and HS2 if required */
	__raw_writel((read_clkctrl & 0xdf75e3ff) | clkctrl, base + CLKCTRL);
	read_clkctrl = __raw_readl(base + CLKCTRL);

	/* poll for the freq,phase lock to occur */
	repeatcnt = VPS_PRCM_MAX_REP_CNT;
	while (((__raw_readl(base + STATUS)) & 0x600) != 0x600) {
		udelay(100);
		if (!--repeatcnt) {
			VPSSERR("PLL Not Getting Locked for"
				"n: %d, m: %d, m2: %d\n",
				n, m, m2);
			return -EINVAL;

		}
	}
	/*wait fot the clocks to get stabized */
	udelay(100);
	return 0;
}
/*config PLL bandwidth*/
static u32 system_pll_bwctrl(u32 base, u32  ctrl)
{
	u32 repeatcnt = VPS_PRCM_MAX_REP_CNT;
	u32 clkctrlval;

	/* Put the PLL in bypass mode and reset*/
	clkctrlval = __raw_readl(base + CLKCTRL);
	__raw_writel(((clkctrlval | CLKCTRL_BYPASS) & ~CLKCTRL_RESET),
			(base + CLKCTRL));
	udelay(3);
	while (((__raw_readl(base + STATUS)) & 0x101) != 0x101) {
		udelay(100);
		if (!--repeatcnt) {
			VPSSERR("Not able to put PLL in idle!!!\n");
			return -EINVAL;
		}

	}

	__raw_writel(ctrl, (base + BWCTRL));
	__raw_writel(0x1, (base + TENABLEDIV));
	udelay(1);
	__raw_writel(0x0, (base + TENABLEDIV));
	udelay(1);
	__raw_writel(0x1, (base + TENABLE));
	udelay(1);
	__raw_writel(0x0, (base + TENABLE));
	udelay(1);

	__raw_writel((clkctrlval | CLKCTRL_RESET) & ~CLKCTRL_BYPASS,
		base + CLKCTRL);
	udelay(10);
	return 0;

}
/*program the pll*/
static u32 system_program_pll(struct vps_systemvpllclk *pll)
{
	int r;
	u32 offset;
	struct system_videopll vpll = {0};
	u32 idx;


	/*if the requested pll is the same as previous, do thing*/
	if ((v_pdata->cpu != CPU_DM816X) && ((!ispllchanged(pll))))
		return 0;

	idx = pll->outputvenc;
	if (pll->outputvenc == VPS_SYSTEM_VPLL_OUTPUT_VENC_HDMI) {
		offset = VIDPLL_SIZE * VPS_SYSTEM_VPLL_OUTPUT_VENC_A;
		idx = VPS_SYSTEM_VPLL_OUTPUT_VENC_A;
	}
	else
		offset = VIDPLL_SIZE * pll->outputvenc;


	r = system_getdividers(&vpll, pll->outputclk, pll->outputvenc);

	if (!r)
		r = system_platformpllcfg((u32)sys_ctrl->pbase + offset,
					vpll.__n,
					vpll.__m,
					vpll.__m2,
					vpll.__sd,
					vpll.clkcfg);

	/*store back*/
	if (!r) {

		sys_ctrl->vpllcfg[idx].outputclk = pll->outputclk;
		sys_ctrl->vpllcfg[idx].outputvenc = pll->outputvenc;
		if (pll->outputvenc == VPS_SYSTEM_VPLL_OUTPUT_VENC_HDMI) {
			sys_ctrl->vpllcfg[idx].outputclk |= HDMI_CLOCK;
			sys_ctrl->vpllcfg[idx].outputvenc =
					VPS_SYSTEM_VPLL_OUTPUT_VENC_A;
		}
	}
	return r;
}

/*S===========================================================================
Public function
============================================================================*/
int vps_system_setpll(struct vps_systemvpllclk *pll)
{
	int r;
	struct vps_system_ctrl *sctrl = sys_ctrl;

	if ((sctrl == NULL) || (sctrl->handle == NULL))
		return 0;

	if (!isvalidpllclk(pll))
		return -EINVAL;

	VPSSDBG("enter set pll %dKHz for VENC %d\n",
		pll->outputclk, pll->outputvenc);


	*sctrl->pllclk = *pll;

	/*if it is pg1 and ti816x and digital clock source, double it*/
	if ((v_pdata->cpu == CPU_DM816X) &&
	    (TI8168_REV_ES1_0 == omap_rev())
	    && (VPS_SYSTEM_VPLL_OUTPUT_VENC_D == sctrl->pllclk->outputvenc))
		sctrl->pllclk->outputclk <<= 1;
	/*program ti814x vid pll from A8 if possible*/
	if ((v_pdata->cpu != CPU_DM816X) && ((u32)sctrl->pbase))
		r = system_program_pll(pll);

	else
		r = vps_fvid2_control(sctrl->handle,
				      IOCTL_VPS_VID_SYSTEM_SET_VIDEO_PLL,
				      (void *)sctrl->pllclk_phy,
				      NULL);

	if (r)
		VPSSERR("set pll failed\n");

	return r;

}

int vps_system_getpll(struct vps_systemvpllclk *pll)
{
	int r = 0;
	struct vps_system_ctrl *sctrl = sys_ctrl;

	if ((sctrl == NULL) || (sctrl->handle == NULL))
		return 0;
	VPSSDBG("getpll\n");

	sctrl->pllclk->outputvenc = pll->outputvenc;

	r = vps_fvid2_control(sctrl->handle,
			       IOCTL_VPS_VID_SYSTEM_GET_VIDEO_PLL,
			       (void *)sctrl->pllclk_phy,
			       NULL);


	if (r)
		VPSSERR("get pll failed\n");
	else {
		if (v_pdata->cpu == CPU_DM816X &&
		    (TI8168_REV_ES1_0 == omap_rev())
		    && (sctrl->pllclk->outputvenc ==
		    VPS_SYSTEM_VPLL_OUTPUT_VENC_D))
			sctrl->pllclk->outputclk >>= 1;

		*pll = *sctrl->pllclk;
	}
	return r;
}

int vps_system_getplatformid(u32 *pid)
{
	int r;
	struct vps_system_ctrl *sctrl = sys_ctrl;

	if ((sctrl == NULL) || (sctrl->handle == NULL))
		return 0;

	r = vps_fvid2_control(sctrl->handle,
			      IOCTL_VPS_VID_SYSTEM_GET_PLATFORM_ID,
			      (char *)sctrl->pid_phy,
			      NULL);
	if (r)
		VPSSERR("failed to get platform id\n");
	else
		*pid = *sctrl->pid;

	return r;

}
static u32 system_create(struct vps_system_ctrl *sctrl)
{
	if (sctrl == NULL)
		return -EINVAL;

	sctrl->handle = vps_fvid2_create(FVID2_VPS_VID_SYSTEM_DRV,
					 0,
					 NULL,
					 NULL,
					 NULL);

	if (sctrl->handle == NULL) {
		VPSSERR("failed to create handle\n");
		return -EINVAL;
	}

	return 0;
}

static u32 system_delete(struct vps_system_ctrl *sctrl)
{
	int r;
	if ((sctrl == NULL) || (sctrl->handle == NULL))
		return 0;

	r = vps_fvid2_delete(sctrl->handle, NULL);

	if (r)
		VPSSERR("failed to delete handle\n");

	return r;
}

static inline int get_payload_size(void)
{
	int size = 0;

	size += sizeof(struct vps_systemvpllclk);
	size += sizeof(enum vps_platformid);

	return size;
}

static inline void assign_payload_addr(struct vps_system_ctrl *sctrl,
				       struct vps_payload_info *pinfo,
				       u32 *buf_offset)
{

	sctrl->pllclk = (struct vps_systemvpllclk *)
			setaddr(pinfo,
				buf_offset,
				&sctrl->pllclk_phy,
				sizeof(struct vps_systemvpllclk));

	sctrl->pid = (u32 *)setaddr(pinfo,
				    buf_offset,
				    &sctrl->pid_phy,
				    sizeof(u32));

}


int __init vps_system_init(struct platform_device *pdev)
{
	int size;
	int r = 0;
	u32 offset = 0;
	struct vps_payload_info  *pinfo;
	enum vps_platformid   pid = VPS_PLATFORM_ID_MAX;

	VPSSDBG("enter system init\n");
	/*allocate payload info*/
	system_payload_info = kzalloc(sizeof(struct vps_payload_info),
				       GFP_KERNEL);

	if (!system_payload_info) {
		VPSSERR("failed to allocate payload structure\n");
		return -ENOMEM;
	}
	pinfo = system_payload_info;

	/*allocate system control*/
	sys_ctrl = kzalloc(sizeof(struct vps_system_ctrl), GFP_KERNEL);
	if (sys_ctrl == NULL) {
		VPSSERR("failed to allocate control\n");
		r = -ENOMEM;
		goto exit;

	}

	/*allocate shared payload buffer*/
	size = get_payload_size();
	pinfo->vaddr = vps_sbuf_alloc(size, &pinfo->paddr);
	if (pinfo->vaddr == NULL) {
		VPSSERR("failed to allocate payload\n");
		 r = -EINVAL;
		goto exit;
	}

	pinfo->size = PAGE_ALIGN(size);
	assign_payload_addr(sys_ctrl, pinfo, &offset);
	memset(pinfo->vaddr, 0, pinfo->size);

	r = system_create(sys_ctrl);
	if (r)
		goto exit;

	r = vps_system_getplatformid(&pid);
	if (r)
		goto exit;

	if (v_pdata->cpu == CPU_DM816X) {
		if (!((pid == VPS_PLATFORM_ID_EVM_TI816x) ||
			(pid == VPS_PLATFORM_ID_SIM_TI816x))) {
			VPSSERR("Wrong M3 firmware,"
				"please use TI816x M3 firmware\n");
			goto exit;
		}
	} else if (v_pdata->cpu == CPU_DM813X) {
		if (!(pid == VPS_PLATFORM_ID_EVM_TI8107)) {
			VPSSERR("Wrong M3 firmware,"
				" please use Dm385 M3 firmware\n");
			goto exit;
		}
	} else if (v_pdata->cpu == CPU_DM814X) {
		if (!((pid == VPS_PLATFORM_ID_EVM_TI814x) ||
			(pid == VPS_PLATFORM_ID_SIM_TI814x))) {
			VPSSERR("Wrong firmware,"
				" please use TI814x M3 firmware\n");
			goto exit;
		}
	} else if (v_pdata->cpu == CPU_DM811X) {
		if (!(pid == VPS_PLATFORM_ID_EVM_TI811x)) {
			
			VPSSERR("Wrong firmware,"
				" please use TI811x M3 firmware\n");
			goto exit;
		}
	}
	if (v_pdata->cpu != CPU_DM816X) {
		sys_ctrl->pbase = ioremap(TI814X_PLL_BASE + VIDPLL_OFFSET,
					VIDPLL_SIZE * 3);
		if (sys_ctrl->pbase) {
			/*set the PLL BW to 2x to reduce jitter*/
			system_pll_bwctrl((u32)sys_ctrl->pbase,
				BWCTRL_2X);
			system_pll_bwctrl((u32)sys_ctrl->pbase + VIDPLL_SIZE,
				BWCTRL_2X);
			system_pll_bwctrl((u32)sys_ctrl->pbase + \
				2 * VIDPLL_SIZE, BWCTRL_2X);

		}
	}
	return 0;
exit:
	vps_system_deinit(pdev);
	return r;

}

int __exit vps_system_deinit(struct platform_device *pdev)
{
	int r = 0;

	VPSSDBG("enter system deinit\n");
	if ((v_pdata->cpu != CPU_DM816X) && sys_ctrl->pbase)
		iounmap(sys_ctrl->pbase);

	if (sys_ctrl) {
		system_delete(sys_ctrl);
		kfree(sys_ctrl);
		sys_ctrl = NULL;
	}

	if (system_payload_info) {
		if (system_payload_info->vaddr) {
			vps_sbuf_free(system_payload_info->paddr,
				      system_payload_info->vaddr,
				      system_payload_info->size);
		}
		kfree(system_payload_info);
		system_payload_info = NULL;
	}
	return r;
}

