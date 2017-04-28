/*
 * linux/drivers/video/ti81xx/ti81xxhdmi/hdmi.c
 *
 * Copyright (C) 2009 Texas Instruments
 * Author: Yong Zhi
 *
 * HDMI settings from TI's DSS driver
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 * History:
 * Mythripk <mythripk@ti.com>	Apr 2010 Modified for EDID reading and adding
 *				OMAP related timing
 *				May 2010 Added support of Hot Plug Detect
 *				July 2010 Redesigned HDMI EDID for Auto-detect
 *				of timing
 *				August 2010 Char device user space control for
 *				HDMI
 * Varada <varadab@ti.com> Jan2011, Re-worked to build TI81xx on chip HDMI drive
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/clk.h>
#include <linux/err.h>
#include <linux/io.h>
#include <linux/interrupt.h>
#include <linux/mutex.h>
#include <linux/completion.h>
#include <linux/delay.h>
#include <linux/string.h>
#include <linux/platform_device.h>
#include <plat/display.h>
#include <plat/cpu.h>
#include <plat/hdmi_lib.h>
#include <plat/gpio.h>
#include <linux/slab.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/ioctl.h>
#include <linux/uaccess.h>
#include <linux/ti81xxhdmi.h>
#include "../vpss/display_interface.h"
#include "hdmi.h"
#include "hdcp.h"
#include <linux/edid.h>

#define DEBUG 1 /* FIXME : this should come from menuconfig */
#ifdef DEBUG
unsigned int ti81xxhdmi_debug;
module_param_named(debug, ti81xxhdmi_debug, bool, 0644);
MODULE_PARM_DESC(ti81xxhdmi_debug, "debug on/off");
#endif

#ifdef DEBUG
#define THDMIDBG(format, ...) \
	do { \
		if (ti81xxhdmi_debug) \
			printk(KERN_DEBUG "TI81XXHDMI  : " format, \
				## __VA_ARGS__); \
	} while (0)
#else
#define THDMIDBG(format, ...)
#endif

#define TI81XX_HDMI_DRIVER_NAME	 "TI81XX_HDMI"
#define ENABLE_VENC_COLORBAR_TEST
#undef ENABLE_VENC_COLORBAR_TEST
/* Comment above line to see venc color bar pattern*/

/* This MACRO controls video / audio streaming when HDMI cable is disconnected.
 *	When defined, on HDMI cable un-plug, both video / audio streaming is
 *		stopped.
 *	When undefined, on HDMI cable un-plug, video / audio streaming is NOT
 *		stopped
 */
/* #define DISABLE_ENABLE_ON_HPD */

static int hdmi_open(struct inode *inode, struct file *filp);
static int hdmi_release(struct inode *inode, struct file *filp);
static long hdmi_ioctl(struct file *file, unsigned int cmd, unsigned long arg);
static int hdmi_enable_display(void);
static void hdmi_disable_display(void);
static int hdmi_get_edid(void);
static int hdmi_start_display(void);
static void hdmi_stop_display(void);
static int ti816x_configure_hdmi_phy(volatile u32 phy_base, int deep_color);
static int ti81xx_hdmi_probe(struct platform_device *device);
static int ti81xx_hdmi_remove(struct platform_device *device);
static int hdmi_display_resume(void);
static int hdmi_display_suspend(void);

static const
struct video_timings all_timings_direct[VIDEO_TIMINGS_NB] = {
	{640, 480, 25200, 96, 16, 48, 2, 10, 33},
	{1280, 720, 74250, 40, 440, 220, 5, 5, 20},
	{1280, 720, 74250, 40, 110, 220, 5, 5, 20},
	{720, 480, 27027, 62, 16, 60, 6, 9, 30},
	{2880, 576, 108000, 256, 48, 272, 5, 5, 39},
	{1440, 240, 27027, 124, 38, 114, 3, 4, 15},
	{1440, 288, 27000, 126, 24, 138, 3, 2, 19},
	{1920, 540, 74250, 44, 528, 148, 5, 2, 15},
	{1920, 540, 74250, 44, 88, 148, 5, 2, 15},
	{1920, 1080, 148500, 44, 88, 148, 5, 4, 36},
	{720, 576, 27000, 64, 12, 68, 5, 5, 39},
	{1440, 576, 54000, 128, 24, 136, 5, 5, 39},
	{1920, 1080, 148500, 44, 528, 148, 5, 4, 36},
	{2880, 480, 108108, 248, 64, 240, 6, 9, 30},
	{1920, 1080, 74250, 44, 638, 148, 5, 4, 36},
	/*Vesa frome here*/
	{640, 480, 25175, 96, 16, 48, 2 , 11, 31},
	{800, 600, 40000, 128, 40, 88, 4 , 1, 23},
	{848, 480, 33750, 112, 16, 112, 8 , 6, 23},
	{1280, 768, 79500, 128, 64, 192, 7 , 3, 20},
	{1280, 800, 83500, 128, 72, 200, 6 , 3, 22},
	{1360, 768, 85500, 112, 64, 256, 6 , 3, 18},
	{1280, 960, 108000, 112, 96, 312, 3 , 1, 36},
	{1280, 1024, 108000, 112, 48, 248, 3 , 1, 38},
	{1024, 768, 65000, 136, 24, 160, 6, 3, 29},
	{1400, 1050, 121750, 144, 88, 232, 4, 3, 32},
	{1440, 900, 106500, 152, 80, 232, 6, 3, 25},
	{1680, 1050, 146250, 176 , 104, 280, 6, 3, 30},
	{1366, 768, 85500, 143, 70, 213, 3, 3, 24},
	{1920, 1080, 148500, 44, 88, 80, 5, 4, 36},
	{1280, 768, 68250, 32, 48, 80, 7, 3, 12},
	{1400, 1050, 101000, 32, 48, 80, 4, 3, 23},
	{1680, 1050, 119000, 32, 48, 80, 6, 3, 21},
	{1280, 800, 79500, 32, 48, 80, 6, 3, 14},
	{1280, 720, 74250, 40, 110, 220, 5, 5, 20} };

/* Array which maps the timing values with corresponding CEA / VESA code */
static int code_index[VIDEO_TIMINGS_NB] = {
	1, 19, 4, 2, 37, 6, 21, 20, 5, 16, 17, 29, 31, 35, 32,
	/* <--15 CEA 17--> vesa*/
	4, 9, 0xE, 0x17, 0x1C, 0x27, 0x20, 0x23, 0x10, 0x2A,
	0X2F, 0x3A, 0X51, 0X52, 0x16, 0x29, 0x39, 0x1B
};

/* Power status of HDMI */
enum hdmi_power_state hdmi_power;

/* Hdmi instance config */
struct hdmi hdmi;

/* Structures for chardevice move this to panel */
static int hdmi_major;
static dev_t hdmi_dev_id;
static struct cdev hdmi_cdev;

/* Read and write ioctls will be added to configure parameters */
static const struct file_operations hdmi_fops = {
	.owner = THIS_MODULE,
	.open = hdmi_open,
	.release = hdmi_release,
	.unlocked_ioctl = hdmi_ioctl,
};

struct hdmi_cm {
	int code;
	int mode;
};

static struct platform_driver ti81xx_hdmi_driver = {
	.probe = ti81xx_hdmi_probe,
	.remove = ti81xx_hdmi_remove,
	.driver = {
		.name = TI81XX_HDMI_DRIVER_NAME,
		.owner = THIS_MODULE,
	},

};


struct hdmi_cm cm;
static bool is_hdmi_on;		/* whether full power is needed */
static u8 edid[HDMI_EDID_MAX_LENGTH] = {0};
static u8 edid_set;
static u8 header[8] = {0x0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0};
struct video_timings edid_timings;
static struct device *ti81xx_hdmi_device;
static struct class *ti81xx_hdmi_class;
static DEFINE_SPINLOCK(irqstatus_lock);
#if 1
struct workqueue_struct *irq_wq;
struct hdmi_work_struct {
	struct work_struct work;
	int r;
};
#endif
static __u8 cec_pa[2] = {0, 0};

struct ti81xxhdmi_sink_edid_parsed parsed_edid;

/*S*******************************  Driver structure *********************/

static int hdmi_panel_enable(void *data);
static int hdmi_panel_disable(void *);
static int hdmi_panel_suspend(void *);
static int hdmi_panel_resume(void *);
static int hdmi_get_panel_edid(char *inbuf, void *);
static int hdmi_set_timings(struct TI81xx_video_timings *timings, void *);
static int hdmi_set_output(enum TI81xx_outputs outputs,
		struct vps_dcoutputinfo *outinfo, void *data);


static struct TI81xx_display_driver hdmi_driver = {
	.display	= TI81xx_DISPLAY_HDMI,
	/*HDMI must be master device to config HDMI_PLL at 10x
	other external device should be slaver*/
	.type		= TI81xx_DEVICE_TYPE_MASTER,
	.enable		= hdmi_panel_enable,
	.disable	= hdmi_panel_disable,
	.suspend	= hdmi_panel_suspend,
	.resume		= hdmi_panel_resume,
	.set_timing	= hdmi_set_timings,
	.get_edid	= hdmi_get_panel_edid,
	.set_output	= hdmi_set_output,
};

/*E*******************************  Driver structure *********************/

/*
 *	   This function is expected to be called when initializing or
 *	   when re-configuring. After re-configuration its recomended to reset
 *	   the core and wrapper. To stabilize the clocks, it recomended to
 *	   wait for a period of time.
 */
/* ti816x_configure_hdmi_phy() funciton is borrowed from ti81xx HDMI librabry */
static int ti816x_configure_hdmi_phy(volatile u32 phy_base, int deep_color)
{
	int rtn_value = 0x0;
	volatile u32 temp;

	/* THDMIDBG(">>>>configure_phy\n"); */

	/* Steps
	 * 0. Power up if powered down
	 * 1. Determine the TCLK based in Deep color mode (Dither mode is used
	 *	to get depth of the color) and Pixel repeatation (depends on
	 *      deep color / resolution and audio). Turn OFF BIST pattern
	 *	generator
	 * 2. Turn OFF BIST and DVI Encoder
	 * 3. Configure the source termination determination - we would require
	 *	when the sink terminates the source - recomended by HDMI
	 *      Spec 1.3A when operating higer frequencies
	 * 4. Enable the PHY
	 */
	temp = __raw_readl((phy_base + TI816x_PHY_TMDS_CNTL3_OFFSET));
	if ((temp & TI816x_HDMI_PHY_TMDS_CNTL3_PDB_MASK) !=
			TI816x_HDMI_PHY_TMDS_CNTL3_PDB_MASK) {
		temp |= TI816x_HDMI_PHY_TMDS_CNTL3_PDB_MASK;
		__raw_writel(temp, (phy_base + TI816x_PHY_TMDS_CNTL3_OFFSET));
	}
	/* BIST Pattern generator is disabled - leave it at that */
	temp = __raw_readl((phy_base + TI816x_PHY_TMDS_CNTL3_OFFSET));
	temp &= (~((TI816x_HDMI_PHY_TMDS_CNTL3_DPCOLOR_CTL_MASK) |
				(TI816x_HDMI_PHY_TMDS_CNTL3_CLKMULT_CTL_MASK) |
				(TI816x_HDMI_PHY_TMDS_CNTL3_BIST_SEL_MASK)));

	/* Step 1.1 - Output width of the dither module in core, determines
	 *			  deep color or not
	 */
	if (deep_color == HDMI_DEEP_COLOR_30BIT) {
		temp |= (TI816x_HDMI_PHY_TMDS_CNTL3_DPCOLOR_CTL_10BITCHANNEL <<
				TI816x_HDMI_PHY_TMDS_CNTL3_DPCOLOR_CTL_SHIFT);

	} else if (deep_color == HDMI_DEEP_COLOR_24BIT) {
		temp |= (TI816x_HDMI_PHY_TMDS_CNTL3_DPCOLOR_CTL_NO <<
				TI816x_HDMI_PHY_TMDS_CNTL3_DPCOLOR_CTL_SHIFT);
	} else {
		temp |= (TI816x_HDMI_PHY_TMDS_CNTL3_DPCOLOR_CTL_12BITCHANNEL <<
				TI816x_HDMI_PHY_TMDS_CNTL3_DPCOLOR_CTL_SHIFT);
	}


	#if 0 /* ToDo : FIXME, for initial testing we commented it out */
	rtn_value = determine_pixel_repeatation(inst_context);
	if (rtn_value == HDMI_PIXEL_REPLECATED_ONCE) {
		temp |= (TI816x_HDMI_PHY_TMDS_CNTL3_CLKMULT_CTL_2_0X <<
				TI816x_HDMI_PHY_TMDS_CNTL3_CLKMULT_CTL_SHIFT);
	} else if (rtn_value == HDMI_PIXEL_REPLECATED_FOUR_TIMES) {
		temp |= (TI816x_HDMI_PHY_TMDS_CNTL3_CLKMULT_CTL_4_0X <<
				TI816x_HDMI_PHY_TMDS_CNTL3_CLKMULT_CTL_SHIFT);
	} else if (rtn_value == 0x0) {
		temp |= (TI816x_HDMI_PHY_TMDS_CNTL3_CLKMULT_CTL_1_0X <<
				TI816x_HDMI_PHY_TMDS_CNTL3_CLKMULT_CTL_SHIFT);
	} else {
		THDBG("Could not calc pixel repeatation\n");
		THDBG("that would be required.\n");
		goto exit_this_func;
	}
	#endif
	rtn_value = 0x0;
	temp |= (TI816x_HDMI_PHY_TMDS_CNTL3_CLKMULT_CTL_1_0X <<
				TI816x_HDMI_PHY_TMDS_CNTL3_CLKMULT_CTL_SHIFT);
	/* ToDo : Currently assuming no pixel repitition. */
	__raw_writel(temp, (phy_base + TI816x_PHY_TMDS_CNTL3_OFFSET));


	temp = __raw_readl((phy_base + TI816x_PHY_BIST_CNTL_OFFSET));
	temp &= ~TI816x_HDMI_PHY_BIST_CNTL_BIST_EN_MASK;
	temp |= TI816x_HDMI_PHY_BIST_CNTL_ENC_BYP_MASK;
	__raw_writel(temp, (phy_base + TI816x_PHY_BIST_CNTL_OFFSET));


	/* Since the 10bit encode is done by the core, we would require to
	   disable 10bit encode in the PHY. Do So */
	__raw_writel(0xE0, (phy_base + TI816x_PHY_TMDS_CNTL9_OFFSET));


	/**************** PHY BIST Test @ half clock rate *********************/
#ifdef TEST_PHY_SEND_OUT_0xAA_AT_HALF_CLOCK_RATE_ON_ALL_DATA_LINES
	__raw_writel(0x40, (phy_base + TI816x_PHY_BIST_CNTL_OFFSET));
	__raw_writel(0xE9, (phy_base + TI816x_PHY_TMDS_CNTL3_OFFSET));
	__raw_writel(0x00, (phy_base + TI816x_PHY_BIST_PATTERN_OFFSET));
	/* Program the instruction, pattern, configuration registers */
	__raw_writel(0x81, (phy_base + TI816x_PHY_BIST_INST0_OFFSET));
	__raw_writel(0x00, (phy_base + TI816x_PHY_BIST_CONF0_OFFSET));
	__raw_writel(0x20, (phy_base + TI816x_PHY_BIST_INST1_OFFSET));
	temp = 0xFF;
	/* Wait for few clocks (say 20 TMDS clocks) would require this. */
	while (temp)
		temp--;
	__raw_writel(0x41, (phy_base + TI816x_PHY_BIST_CNTL_OFFSET));
#endif	/* TEST_PHY_SEND_OUT_0xAA_AT_HALF_CLOCK_RATE_ON_ALL_DATA_LINES */
	/**********PHY BIST Test @ half clock rate***************/

	/* Step 3 and 4 */
	temp = __raw_readl((phy_base + TI816x_PHY_TMDS_CNTL2_OFFSET));
	temp |=
		(TI816x_HDMI_PHY_TMDS_CNTL2_TERM_EN_MASK |
		 TI816x_HDMI_PHY_TMDS_CNTL2_OE_MASK);
	__raw_writel(temp, (phy_base + TI816x_PHY_TMDS_CNTL2_OFFSET));


/* exit_this_func: */
	/* THDMIDBG("configure_phy<<<<"); */
	return rtn_value;
}



/* ti816x_enable_hdmi_clocks() funciton is borrowed from ti81xx HDMI librabry */
static void ti816x_enable_hdmi_clocks(u32 prcm_base)
{
	u32 temp;
	THDMIDBG(" HDMI Clk enable in progress\n");
	temp = 2;
	/*Enable Power Domain Transition for HDMI */
	__raw_writel(temp, (prcm_base + TI816x_CM_HDMI_CLKSTCTRL_OFF));
	/*Enable HDMI Clocks*/
	__raw_writel(temp, (prcm_base + TI816x_CM_ACTIVE_HDMI_CLKCTRL_OFF));

	/*Check clocks are active*/
	while (((__raw_readl(prcm_base + TI816x_CM_HDMI_CLKSTCTRL_OFF)) >> 8)
	      != 0x3)
		;

	THDMIDBG(" HDMI Clk enabled\n");

	/* Check to see module is functional */
	while (((__raw_readl(prcm_base + TI816x_CM_ACTIVE_HDMI_CLKCTRL_OFF)
	      & 0x70000) >> 16) != 0)
		;

	THDMIDBG(" HDMI Clocks enabled successfully\n");
}

static void ti816x_disable_hdmi_clocks(u32 prcm_base)
{
	/* ToDo : implement this function */
	return;
}

static int ti816x_hdmi_phy_off(u32 name)
{
	/* ToDo : Implement it */
	return 0;

}


/* hdmi_write_reg(), hdmi_read_reg(), macros
 *(REG_FLD_MOD, FLD_MOD,FLD_MASK,FLD_VAL, FLD_GET) ,
 *hdmi_phy_off(), hdmi_phy_init() borrowed from OMAP4 HDMI library
 * These functions are applicable to only TI814x SoC
 */

static inline void hdmi_write_reg(u32 base, u16 idx, u32 val)
{
	void __iomem *b;

	switch (base) {
	case TI814x_HDMI_PHY_0_REGS:
		b = hdmi.base_phy;
		break;
	case TI814x_HDMI_PLL_BASE_ADDR:
		b = hdmi.base_pll;
		break;
	default:
		BUG();
	}
	__raw_writel(val, b + idx);
	/* DBG("write = 0x%x idx =0x%x\r\n", val, idx); */
}

static inline u32 hdmi_read_reg(u32 base, u16 idx)
{
	void __iomem *b;
	u32 l;

	switch (base) {
	case TI814x_HDMI_PHY_0_REGS:
		b = hdmi.base_phy;
		break;
	case TI814x_HDMI_PLL_BASE_ADDR:
		b = hdmi.base_pll;
		break;
	default:
		BUG();
	}
	l = __raw_readl(b + idx);

	/* DBG("addr = 0x%p rd = 0x%x idx = 0x%x\r\n", (b+idx), l, idx); */
	return l;
}

#define FLD_MASK(start, end)	(((1 << ((start) - (end) + 1)) - 1) << (end))
#define FLD_VAL(val, start, end) (((val) << (end)) & FLD_MASK(start, end))
#define FLD_GET(val, start, end) (((val) & FLD_MASK(start, end)) >> (end))
#define FLD_MOD(orig, val, start, end) \
	(((orig) & ~FLD_MASK(start, end)) | FLD_VAL(val, start, end))

#define REG_FLD_MOD(b, i, v, s, e) \
	hdmi_write_reg(b, i, FLD_MOD(hdmi_read_reg(b, i), v, s, e))

void ti814x_enable_ddc_pinmux(void)
{
	u32 temp;
	u32 ti814x_ctrl_module_base = 0x48140000;
	u32 base = (u32)ioremap(ti814x_ctrl_module_base, 0x10000);

	temp = 0xe0002;
	__raw_writel(temp, (base+0x0934));
	temp = 0xe0002;
	__raw_writel(temp, (base+0x0938));
	temp = 0x60010;
	__raw_writel(temp, (base+0x09b8));
	temp = 0x40010;
	__raw_writel(temp, (base+0x09bc));

	/* read back and print, only for debug */ /* Varada delete it */
	THDMIDBG("\n0x48140934:%x,0x48140938:%x,0x481409b8:%x,0x481409bc:%x,\n"
		, __raw_readl(base+0x0934),
		 __raw_readl(base+0x0938),
		 __raw_readl(base+0x09b8),
		 __raw_readl(base+0x09bc));


	THDMIDBG("DM814x : DDC pinmux enabled successfully\n");
}
/* This mux is for configuring the pixel clock to Venc through HDMI or PLL*/
void ti814x_set_clksrc_mux(int enable)
{
	u32 reg_value;
	u32 reg_base;

	reg_base = (u32)ioremap(TI814x_HDMI_MUX_ADDR, 0x10);
	reg_value = __raw_readl(reg_base);
	if (enable)
		reg_value |= 0x1;
	else
		reg_value &= 0xFFFFFFFE;

	__raw_writel(reg_value, reg_base);
	iounmap((u32 *)TI814x_HDMI_MUX_ADDR);
}


void ti814x_enable_hdmi_clocks(u32 prcm_base)
{
	u32 temp;

	temp = 2;
	/*Enable Power Domain Transition for HDMI */
	__raw_writel(temp, (prcm_base + TI814x_CM_ALWON_SDIO_CLKCTRL));

	/*Check clocks are active*/
	while (((__raw_readl(prcm_base + TI814x_CM_ALWON_SDIO_CLKCTRL))
		>> 16) != 0x0)
		;

	temp = 2;
	/*Enable Power Domain Transition for HDMI */
	__raw_writel(temp, (prcm_base + TI814x_CM_HDMI_CLKCTRL_OFF));

	/*Check clocks are active*/
	while (((__raw_readl(prcm_base + TI814x_CM_HDMI_CLKCTRL_OFF)) >> 16)
	      != 0x0)
		;

	THDMIDBG("HDMI Clocks enabled successfully\n");
}

void ti814x_disable_hdmi_clocks(u32 prcm_base)
{
	/* ToDo : implement this function */
	return;
}

static int ti814x_hdmi_phy_off(u32 name)
{
	int r = 0;

	/* wait till PHY_PWR_STATUS=OFF */
	/* HDMI_PHYPWRCMD_OFF = 0 */
	r = HDMI_W1_SetWaitPhyPwrState(name, 0);
	if (r)
		return r;

	return 0;
}


/* double check the order */
static int ti814x_hdmi_phy_init(u32 w1, u32 phy, int tmds)
{
	int r;

	/* wait till PHY_PWR_STATUS=LDOON */
	/* HDMI_PHYPWRCMD_LDOON = 1 */
	r = HDMI_W1_SetWaitPhyPwrState(w1, 1);
	if (r)
		return r;

	/* wait till PHY_PWR_STATUS=TXON */
	r = HDMI_W1_SetWaitPhyPwrState(w1, 2);
	if (r)
		return r;

	/* read address 0 in order to get the SCPreset done completed */
	/* Dummy access performed to solve resetdone issue */
	hdmi_read_reg(phy, TI814x_HDMI_TXPHY_TX_CTRL);

	/* write to phy address 0 to configure the clock */
	/* use HFBITCLK write HDMI_TXPHY_TX_CONTROL__FREQOUT field */
	REG_FLD_MOD(phy, TI814x_HDMI_TXPHY_TX_CTRL, tmds, 31, 30);

	/* setup max LDO voltage */
	REG_FLD_MOD(phy, TI814x_HDMI_TXPHY_POWER_CTRL, 0xB, 3, 0);


	/* Tried to mimic DM814x driver */
	r = hdmi_read_reg(phy, TI814x_HDMI_TXPHY_PAD_CFG_CTRL);
	/* Normal polarity for all the links */
	r |= 0x1 << 31;
	r |= 0x0 << 30;
	r |= 0x0 << 29;
	r |= 0x0 << 28;
	r |= 0x0 << 27;
	/* Channel assignement is 10101 D2- D1 D0-CLK */
	r |=  0x21 << 22;
	hdmi_write_reg(phy, TI814x_HDMI_TXPHY_PAD_CFG_CTRL, r);

	/* write to phy address 1 to start HDMI line (TXVALID and TMDSCLKEN) */
	hdmi_write_reg(phy, TI814x_HDMI_TXPHY_DIGITAL_CTRL, 0xF0000000);

	/* write to phy address 1 to start HDMI line (TXVALID and TMDSCLKEN */
	/* FIXME : Just test, May need to be deleted */
	hdmi_write_reg(phy, TI814x_HDMI_PHY_ANG_INT_CTRL, 0x00005800);

	mdelay(100);
	return 0;
}


static int ti814x_hdmi_pll_power_on(void)
{
	u32 r;

	HDMI_PllPwr_t PllPwrWaitParam;

	/* wait for wrapper rest */
	HDMI_W1_SetWaitSoftReset();

	/* power off PLL */
	PllPwrWaitParam = HDMI_PLLPWRCMD_ALLOFF;
	r = HDMI_W1_SetWaitPllPwrState(TI81xx_HDMI_WP, PllPwrWaitParam);
	if (r)
		return r;

	/* power on PLL */
	PllPwrWaitParam = HDMI_PLLPWRCMD_BOTHON_ALLCLKS;
	r = HDMI_W1_SetWaitPllPwrState(TI81xx_HDMI_WP, PllPwrWaitParam);

	return r;
}

static inline void print_omap_video_timings(struct video_timings *timings)
{


		THDMIDBG("Timing Info:\n");
		THDMIDBG("  pixel_clk = %d\n", timings->pixel_clock);
		THDMIDBG("  x_res	 = %d\n", timings->x_res);
		THDMIDBG("  y_res	 = %d\n", timings->y_res);
		THDMIDBG("  hfp	   = %d\n", timings->hfp);
		THDMIDBG("  hsw	   = %d\n", timings->hsw);
		THDMIDBG("  hbp	   = %d\n", timings->hbp);
		THDMIDBG("  vfp	   = %d\n", timings->vfp);
		THDMIDBG("  vsw	   = %d\n", timings->vsw);
		THDMIDBG("  vbp	   = %d\n", timings->vbp);

}

/* Note : Taken from OMAP driver and modified some,
 * for initial support of 4 resolutions. */
static struct hdmi_cm hdmi_get_code(struct TI81xx_video_timings *timing)
{
	int i = 0, code = -1, temp_vsync = 0, temp_hsync = 0;
	int timing_vsync = 0, timing_hsync = 0;
	struct video_timings temp;
	struct hdmi_cm cm = {-1};
	int custom = 1;

	/* default values */
	cm.mode = 1;  /* HDMI mode */
	cm.code = 16; /* 1080P-60 */

	if (timing->standard == FVID2_STD_1080P_30)
		cm.code = 34;

	cm.mode = timing->dvi_hdmi;
	if (custom) {

		for (i = 0; i < 33; i++) {
			temp = all_timings_direct[i];
			if (!timing->scanformat)
				temp.y_res *= 2;

			if (temp.pixel_clock != timing->pixel_clock ||
				temp.x_res != timing->width ||
				temp.y_res != timing->height)
				continue;

			temp_hsync = temp.hfp + temp.hsw + temp.hbp;
			timing_hsync = timing->hfp + timing->hsw + timing->hbp;
			temp_vsync = temp.vfp + temp.vsw + temp.vbp;
			timing_vsync = timing->vfp + timing->vsw + timing->vbp;

			THDMIDBG("Temp_hsync = %d, temp_vsync = %d, "
				"timing_hsync = %d, timing_vsync = %d",
					temp_hsync, temp_vsync,
					timing_hsync, timing_vsync);

			if (temp_hsync == timing_hsync &&
				 temp_vsync == timing_vsync) {
				code = i;
				cm.code = code_index[i];
				cm.mode = code < 14;
				THDMIDBG("Hdmi_code = %d mode = %d\n",
					cm.code, cm.mode);
				print_omap_video_timings(&temp);
				break;
			}
		}
	}

	return cm;
}


static int hdmi_get_edid(void)
{
	u8 i = 0, *e;
	int offset, addr, r;
	struct HDMI_EDID *edid_st = (struct HDMI_EDID *)edid;
	struct deep_color *vsdb_format;
	struct latency *lat;
	struct ti81xxhdmi_sink_edid_parsed *edid_p = &parsed_edid;

	r = 0;
	vsdb_format = kzalloc(sizeof(*vsdb_format), GFP_KERNEL);
	lat = kzalloc(sizeof(*lat), GFP_KERNEL);

	memset(&parsed_edid, 0, sizeof(struct ti81xxhdmi_sink_edid_parsed));
	edid_p->num_dt = 0;

	if (HDMI_CORE_DDC_READEDID(HDMI_CORE_SYS, edid,	HDMI_EDID_MAX_LENGTH)
	    != 0) {
		printk(KERN_INFO "HDMI failed to read E-EDID\n");
		r = -1;
		goto exit;
	}
	if (0 != memcmp(edid, header, sizeof(header))) {
		printk(KERN_INFO "Header mismatch\n");
		r = -1;
		goto exit;
	}
	edid_set = true;
	e = edid;
	 THDMIDBG(KERN_INFO "\nHeader:\n%02x\t%02x\t%02x\t%02x\t%02x\t%02x\t"
		"%02x\t%02x\n", e[0], e[1], e[2], e[3], e[4], e[5], e[6], e[7]);
	e += 8;
	THDMIDBG(KERN_INFO "Vendor & Product:\n%02x\t%02x\t%02x\t%02x\t%02x\t"
		"%02x\t%02x\t%02x\t%02x\t%02x\n",
		e[0], e[1], e[2], e[3], e[4], e[5], e[6], e[7], e[8], e[9]);
	e += 10;
	 THDMIDBG(KERN_INFO "EDID Structure:\n%02x\t%02x\n",
		e[0], e[1]);
	edid_p->version = e[0];
	edid_p->revision = e[1];
	e += 2;
	THDMIDBG(KERN_INFO
		 "Basic Display Parameter:\n%02x\t%02x\t%02x\t%02x\t%02x\n",
		e[0], e[1], e[2], e[3], e[4]);
	e += 5;
	THDMIDBG(KERN_INFO "Color Characteristics:\n%02x\t%02x\t%02x\t%02x\t"
		"%02x\t%02x\t%02x\t%02x\t%02x\t%02x\n",
		e[0], e[1], e[2], e[3], e[4], e[5], e[6], e[7], e[8], e[9]);
	e += 10;
	THDMIDBG(KERN_INFO "Established timings - VESA:\n%02x\t%02x\t%02x\n",
		e[0], e[1], e[2]);
	edid_p->established_timings[0] = e[0];
	edid_p->established_timings[1] = e[1];
	edid_p->established_timings[2] = e[2];
	e += 3;

	THDMIDBG(KERN_INFO "Refer VESA E-EDID Standard to interpret above\n");
	THDMIDBG(KERN_INFO "Standard timings - VESA :\n%02x\t%02x\t%02x\t%02x\t"
			 "%02x\t%02x\t%02x\t%02x\n",
		e[0], e[1], e[2], e[3], e[4], e[5], e[6], e[7]);
	THDMIDBG(KERN_INFO "Refer VESA E-EDID Standard to interpret above\n");
	edid_p->standard_timings[0] = e[0];
	edid_p->standard_timings[1] = e[1];
	edid_p->standard_timings[2] = e[2];
	edid_p->standard_timings[3] = e[3];
	edid_p->standard_timings[4] = e[4];
	edid_p->standard_timings[5] = e[5];
	edid_p->standard_timings[6] = e[6];
	edid_p->standard_timings[7] = e[7];
	e += 8;
	 THDMIDBG(KERN_INFO "%02x\t%02x\t%02x\t%02x\t%02x\t%02x\t%02x\t%02x\n",
		e[0], e[1], e[2], e[3], e[4], e[5], e[6], e[7]);
	e += 8;

	for (i = 0; i < EDID_SIZE_BLOCK0_TIMING_DESCRIPTOR; i++) {
		THDMIDBG(KERN_INFO "Extension 0 Block %d\n", i);
		get_edid_timing_info(&edid_st->DTD[i],
			&(edid_p->detailed_timings[edid_p->num_dt]));

		/*	print_omap_video_timings(&timings); */
		if (edid_p->detailed_timings[edid_p->num_dt].pixel_clock != 0)
			edid_p->num_dt++;
	}

	if (edid[0x7e] != 0x00) {
		offset = edid[EDID_DESCRIPTOR_BLOCK1_ADDRESS + 2];
		/* THDMIDBG(KERN_INFO "offset %x\n", offset); */
		if (offset != 0) {
			addr = EDID_DESCRIPTOR_BLOCK1_ADDRESS + offset;
			/*to determine the number of descriptor blocks */
			for (i = 0; i < EDID_SIZE_BLOCK1_TIMING_DESCRIPTOR;
									i++) {
				THDMIDBG(KERN_INFO "Extension 1 Block %d", i);
				get_eedid_timing_info(addr, edid,
				   &(edid_p->detailed_timings[edid_p->num_dt]));
				addr += EDID_TIMING_DESCRIPTOR_SIZE;
				/* print_omap_video_timings(&timings); */
		if (edid_p->detailed_timings[edid_p->num_dt].pixel_clock != 0)
			edid_p->num_dt++;
			}
		}
	}
	edid_p->is_hdmi_supported = hdmi_tv_hdmi_supported(edid);

	if (0x0 != edid_p->is_hdmi_supported) {
		hdmi_get_image_format(edid, &edid_p->supported_cea_vic);

		hdmi_get_audio_format(edid, &edid_p->audio_support);

		hdmi_deep_color_support_info(edid, vsdb_format);
		/* THDMIDBG(KERN_INFO "%d deep color bit 30 %d "
			"deep color 36 bit %d max tmds freq",
			vsdb_format->bit_30, vsdb_format->bit_36,
			vsdb_format->max_tmds_freq); */

		hdmi_get_av_delay(edid, lat);
		/* THDMIDBG(KERN_INFO "%d vid_latency %d aud_latency "
			"%d interlaced vid latency %d interlaced aud latency",
			lat->vid_latency, lat->aud_latency,
			lat->int_vid_latency, lat->int_aud_latency); */

		edid_p->is_yuv_supported = hdmi_tv_yuv_supported(edid);
	}

exit:
	kfree(vsdb_format);
	kfree(lat);
	return r;
}

#ifdef ENABLE_VENC_COLORBAR_TEST
/* Only for initial test, this function is here, mainly for generating
   Colorbar test, the HDMI driver is not responsible for VENC config. */
static void enable_debug_colorbar(u32 *venc_base)
{
	unsigned int data = *venc_base;
	*venc_base = data | 0x00008000;
	THDMIDBG("Venc 'stest' bit ON, for the test color bars\n");
}
#endif
static int hdmi_display_suspend(void)
{
	THDMIDBG("hdmi_display_suspend\n");

	if (hdmi.status != TI81xx_EXT_ENCODER_ENABLED)
		return -EINVAL;

	mutex_lock(&hdmi.lock);

	hdmi_stop_display();

	mutex_unlock(&hdmi.lock);

	hdmi.status = TI81xx_EXT_ENCODER_SUSPENDED;

	return 0;
}

static int hdmi_display_resume(void)
{
	int r = 0;
	THDMIDBG("hdmi_display_resume\n");

	if (hdmi.status != TI81xx_EXT_ENCODER_SUSPENDED)
		return -EINVAL;

	mutex_lock(&hdmi.lock);

	r = hdmi_start_display();

	mutex_unlock(&hdmi.lock);

	hdmi.status = TI81xx_EXT_ENCODER_ENABLED;

	return r;
}


static void hdmi_power_off(void)
{
	/* FIXME : ToDo : Maybe also turn off in the core SRST register,
	   bit 0 (SWRST) */
	hdmi_core_software_reset();

	HDMI_W1_StopVideoFrame(TI81xx_HDMI_WP);

	hdmi.power_state = HDMI_POWER_OFF;

	if (cpu_is_ti814x()) {
		ti814x_set_clksrc_mux(0);
		ti814x_hdmi_phy_off(TI81xx_HDMI_WP);
		HDMI_W1_SetWaitPllPwrState(TI81xx_HDMI_WP,
					   HDMI_PLLPWRCMD_ALLOFF);
		ti814x_disable_hdmi_clocks((u32)hdmi.base_prcm);
		ti814x_hdmi_phy_off(TI81xx_HDMI_WP);
	} else if (cpu_is_ti816x()) {

		ti816x_disable_hdmi_clocks((u32)hdmi.base_prcm);
		ti816x_hdmi_phy_off(TI81xx_HDMI_WP);
	}
	if (hdmi.cec_state != HDMI_CEC_BYPASS)
		hdmi.cec_state = HDMI_CEC_BYPASS;
	edid_set = false;
}



static int hdmi_open(struct inode *inode, struct file *filp)
{
	return 0;
}

static int hdmi_release(struct inode *inode, struct file *filp)
{
	return 0;
}


static long hdmi_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	int r = 0;
	struct ti81xxhdmi_status  *status;
	unsigned long flags;

	switch (cmd) {
	case TI81XXHDMI_START:
		THDMIDBG("ioctl TI81XXHDMI_START\n");
		if (hdmi.power_state != HDMI_POWER_FULL) {
			r = hdmi_enable_display();
			is_hdmi_on = true;
		} else
			THDMIDBG("Display already enabled");
		break;
	case TI81XXHDMI_STOP:
		THDMIDBG("ioctl TI81XXHDMI_STOP\n");
		hdmi_disable_display();
		break;
	case TI81XXHDMI_GET_STATUS:
		THDMIDBG("ioctl TI81XXHDMI_GET_STATUS\n");
		/* Varada : make it TI81XXHDMI_GET_STATUS
		   Varada : Implement it and test it */
		r = -EFAULT ;
		if (arg) {
			status = (struct ti81xxhdmi_status *)arg;
			status->is_hdmi_streaming = hdmi_w1_video_status();
			status->is_hpd_detected = hdmi_get_hpd_pin_state();
			r = 0x0;
		}
		break;
	case TI81XXHDMI_READ_EDID:
		THDMIDBG("ioctl TI81XXHDMI_READ_EDID\n");
		if (false == edid_set) {
			mutex_lock(&hdmi.lock);

			r = hdmi_get_edid();

			mutex_unlock(&hdmi.lock);
		} else {
			THDMIDBG("Sinks EDID was already read-skip re-read\n");
			THDMIDBG("Copying previously read EDID\n");
			r = 0;
		}

		/*make sure arg is not NULL*/
		if ((r == 0) && (arg)) {
			r = copy_to_user((void __user *)arg, &edid,
					 sizeof(struct HDMI_EDID));
			if (r) {
				THDMIDBG("Error : copy_to_user, r = %d ", r);
				r = -EFAULT;
			}
		}
		break;
	case TI81XXHDMI_GET_PARSED_EDID_INFO:
		THDMIDBG("ioctl TI81XXHDMI_GET_PARSED_EDID_INFO\n");
		mutex_lock(&hdmi.lock);

		if (arg) {
			r = copy_to_user((void __user *)arg, &parsed_edid,
				 sizeof(struct ti81xxhdmi_sink_edid_parsed));
			if (r) {
				THDMIDBG("Error : copy_to_user, r = %d ", r);
				r = -EFAULT;
			}
		}

		mutex_unlock(&hdmi.lock);
		break;
	case TI81XXHDMI_CEC_ACTIVATE:
		mutex_lock(&hdmi.lock);
		if (hdmi.cec_state == HDMI_CEC_BYPASS) {
			if (hdmi_lib_cec_activate() != 0) {
				/* Cant power up CEC, but why? */
				r = -ENODEV;
				break;
			}
			spin_lock_irqsave(&irqstatus_lock, flags);
			hdmi.cec_state = HDMI_CEC_UN_REGISTERED;
			spin_unlock_irqrestore(&irqstatus_lock, flags);
		}
		mutex_unlock(&hdmi.lock);
		break;
	case TI81XXHDMI_CEC_REG_DEV:
		THDMIDBG("ioctl TI81XXHDMI_CEC_REG_DEV\n");
		/* Require to be in HDMI mode and sink detected, to be able to
		 * register, check that */
		r = -EINVAL;
		mutex_lock(&hdmi.lock);
		while (arg) {
			if (hdmi.cec_state != HDMI_CEC_UN_REGISTERED) {
				r = -EBUSY;
				break;
			}
			if (!(hdmi.cfg.hdmi_dvi)) {
				r = -EPERM;
				break;
			}
			if (!hdmi_get_hpd_pin_state()) {
				r = -EPERM;
				break;
			}
			if (hdmi_tv_cec_get_pa(edid, cec_pa) != true)
				break;
			/* Change the state before registration, in case sink
			was disconnected, after this, the state would be updated
			by isr */
			spin_lock_irqsave(&irqstatus_lock, flags);
			if (hdmi_get_hpd_pin_state())
				hdmi.cec_state = HDMI_CEC_REGISTERED;

			spin_unlock_irqrestore(&irqstatus_lock, flags);

			r = hdmi_lib_cec_reg_dev(
				(struct ti81xxhdmi_cec_register_dev *) arg,
				cec_pa);
			if (hdmi.cec_state != HDMI_CEC_REGISTERED) {
				r = -EIO;
				break;
			}
			if (r != 0) {
				spin_lock_irqsave(&irqstatus_lock, flags);
				hdmi.cec_state = HDMI_CEC_UN_REGISTERED;
				spin_unlock_irqrestore(&irqstatus_lock, flags);
				break;
			}
			((struct ti81xxhdmi_cec_register_dev *)
				arg)->physical_addr[0] = cec_pa[0];
			((struct ti81xxhdmi_cec_register_dev *)
				arg)->physical_addr[1] = cec_pa[1];
			break;
		}
		if (r) {
			THDMIDBG("Error : hdmi_lib_cec_reg_dev, r = %d ", r);
			r = -EFAULT;
		}
		mutex_unlock(&hdmi.lock);
		break;
	case TI81XXHDMI_CEC_TRANSMIT_MSG:
		r = -EINVAL;
		mutex_lock(&hdmi.lock);
		while (arg) {
			struct ti81xxhdmi_cec_transmit_msg *msg =
				(struct ti81xxhdmi_cec_transmit_msg *)arg;
			r = -EPERM;
			if (!(hdmi.cfg.hdmi_dvi)) {
				THDMIDBG("Write failed - DVI mode\n");
				break;
			}
			if (!hdmi_get_hpd_pin_state()) {
				THDMIDBG("Write failed - No Sink\n");
				break;
			}
			if (hdmi.cec_state != HDMI_CEC_REGISTERED) {
				THDMIDBG("Write failed - CEC not reg\n");
				r = -EPERM;
				break;
			}
			if (msg->no_retransmits > 0x5u) {
				THDMIDBG("Write failed - retrans high\n");
				break;
			}
			if (msg->no_args > 0xFu) {
				THDMIDBG("Write failed - High args\n");
				break;
			}
			r = hdmi_lib_cec_write_msg(msg);
			break;
		}
		mutex_unlock(&hdmi.lock);
		break;
	case TI81XXHDMI_CEC_RECEIVE_MSG:
		r = -EINVAL;
		mutex_lock(&hdmi.lock);
		while (arg) {
			r = -EPERM;
			if (!(hdmi.cfg.hdmi_dvi)) {
				THDMIDBG("Read failed - DVI mode\n");
				break;
			}

			if (!hdmi_get_hpd_pin_state()) {
				THDMIDBG("Read failed - No Sink\n");
				break;
			}
			if (hdmi.cec_state == HDMI_CEC_BYPASS) {
				THDMIDBG("Read failed - Not Activated\n");
				r = -EPERM;
				break;
			}
			r = hdmi_lib_cec_read_msg(
				(struct ti81xxhdmi_cec_received_msg *)arg);
			break;
		}
		mutex_unlock(&hdmi.lock);
		break;
	case TI81XXHDMI_HDCP_ENABLE:
		mutex_lock(&hdmi.lock);
		if (hdmi.enable_ctl != 0){
			r = hdmi.enable_ctl((void __user *)arg);
		}
		mutex_unlock(&hdmi.lock);
		break;
	case TI81XXHDMI_HDCP_DISABLE:
		mutex_lock(&hdmi.lock);
		if (hdmi.disable_ctl != 0){
			hdmi.disable_ctl();
		}
		mutex_unlock(&hdmi.lock);
		break;
	case TI81XXHDMI_HDCP_GET_STATUS:
		mutex_lock(&hdmi.lock);
		if (hdmi.query_status_ctl != 0){
			hdmi.query_status_ctl((void __user *)arg);
		}
		mutex_unlock(&hdmi.lock);
		break;
	case TI81XXHDMI_HDCP_WAIT_EVENT:
		if (hdmi.wait_event_ctl != 0){
			hdmi.wait_event_ctl((void __user *)arg);
		}
		break;
	case TI81XXHDMI_HDCP_EVENT_DONE:
		mutex_lock(&hdmi.lock);
		if (hdmi.done_ctl != 0){
			hdmi.done_ctl((void __user *)arg);
		}
		mutex_unlock(&hdmi.lock);
		break;
	default:
		r = -EINVAL;
		THDMIDBG("Un-recoganized command, cmd = %d", cmd);
		break;
	}
	return r;
}



static int hdmi_power_on(void)
{
	int r = 0;

	hdmi.power_state = HDMI_POWER_FULL;

	hdmi.cfg.v_pol = 0;  /* TI81xx specific */
	hdmi.cfg.h_pol = 0;  /* TI81xx specific */

	THDMIDBG("\n	hdmi.code   =%x\n", hdmi.code);

	/* ti81xx_hdmi_set_mode(hdmi_mode, &hdmi_config); */

	HDMI_W1_StopVideoFrame(TI81xx_HDMI_WP);

	HDMI_W1_SetWaitSoftReset(); /* Adding here based on DM816x, maybe move
				       this to configure_phy function */


	/* Turn ON the PLL, control in the WP */
	if (cpu_is_ti814x()) {
		r = ti814x_hdmi_pll_power_on();
		if (r)
			goto err;
	}
	/* Configure PHY */
	if (cpu_is_ti814x()) {
		r = ti814x_hdmi_phy_init(TI81xx_HDMI_WP,
				TI814x_HDMI_PHY_0_REGS, hdmi.freq);
		if (r) {
			THDMIDBG("DM814x : Failed to start PHY\n");
			r = -EIO;
			goto err;
		}
	}
	if (cpu_is_ti816x()) {
		r = ti816x_configure_hdmi_phy((volatile u32)hdmi.base_phy,
				hdmi.cfg.deep_color);
		if (r != 0) {
			THDMIDBG("DM816x : Failed to start PHY\n");
			goto err;
		}
	}


	hdmi.cfg.hdmi_dvi	 = hdmi.mode;
	hdmi.cfg.video_format = hdmi.code;

	if ((hdmi.mode)) {
		switch (hdmi.code) {
		case 20:
		case 5:
		case 6:
		case 21:
			hdmi.cfg.interlace = 1;
			break;
		default:
			hdmi.cfg.interlace = 0;
			break;
		}
	}

	/* ToDo :  initial release , no deep color support */
	hdmi.cfg.deep_color = HDMI_DEEP_COLOR_24BIT;
	/*force HDMI output DVI if the TV does not support HDMI
	  or TV connected*/
	/* Note that, we need not wait for exclusive access, as the
	 * function caller has already ensured the same.
	 * i.e. hdmi.lock has been acquired 
	 */
	/* If the EDID read fails, we fall back to supplied HDMI mode
	 * HDMI / DVI 1/0 */
	if (0 != hdmi_get_edid())
		hdmi.cfg.hdmi_dvi   = hdmi.mode;
	else
		hdmi.cfg.hdmi_dvi = (u16)parsed_edid.is_hdmi_supported;
	THDMIDBG("Interface is %d [0:DVI, 1:HDMI]\n", hdmi.cfg.hdmi_dvi);
	/* Start HDMI library */
	hdmi_lib_enable(&hdmi.cfg);

	/* ToDo : Support next release */
	/* hdmi_configure_lr_fr(); */

	/* Turn ON VENC Color bar.*/
#if 0
	void __iomem *venc_v_addr =  ioremap(0x48106000, 0x80);
	if (venc_v_addr == 0x0)
		THDMIDBG("hdmi_power_on : Could not ioremap for Venc\n");
	enable_debug_colorbar(venc_v_addr);
#endif

	/* After all configuration, Turn ON in HDMI Wrapper  */
	HDMI_W1_StartVideoFrame(TI81xx_HDMI_WP);

	hdmi_set_irqs(0);

	if (cpu_is_ti814x())
		ti814x_set_clksrc_mux(1);

	return 0;
err:
	return r;
}

/* set power state depending on device state and HDMI state */
static int hdmi_set_power(void)
{
	int r = 0;

	/* do not change power state if suspended */
	r = hdmi_power_on();  /* DM81xx: currently support only full power up*/

	return r;
}

static void hdmi_work_queue(struct work_struct *ws)
{
	struct hdmi_work_struct *work =
		container_of(ws, struct hdmi_work_struct, work);
	int r = work->r;


	if (r & HDMI_HPD_MODIFY) {
		/* FIX ME When connected sink is removed, (not a toggel on HPD)
			ensure to set hdmi.cec_state to HDMI_CEC_BYPASS
			if this device was registered in the CEC n/w */
		if (hdmi.cec_state != HDMI_CEC_BYPASS)
			hdmi.cec_state = HDMI_CEC_BYPASS;

		edid_set = false;

#ifdef DISABLE_ENABLE_ON_HPD
		hdmi_disable_display();
		hdmi_enable_display();
#endif

	}

	kfree(work);
}


static inline void hdmi_handle_irq_work(int r)
{
	struct hdmi_work_struct *work;
	if (r) {
		work = kmalloc(sizeof(*work), GFP_ATOMIC);

		if (work) {
			INIT_WORK(&work->work, hdmi_work_queue);
			work->r = r;
			queue_work(irq_wq, &work->work);
		} else {
			THDMIDBG(KERN_ERR "Cannot allo memory to create work");
		}
	}
}

static bool hdmi_connected;
static irqreturn_t hdmi_irq_handler(int irq, void *arg)
{
	unsigned long flags;
	int r = 0;



	/* process interrupt in critical section to handle conflicts */
	spin_lock_irqsave(&irqstatus_lock, flags);

	HDMI_W1_HPD_handler(&r);

	if (hdmi.isr_cb != 0)
		hdmi.isr_cb(r);

	if (r & HDMI_CONNECT)
		hdmi_connected = true;
	if (r & HDMI_DISCONNECT)
		hdmi_connected = false;

	spin_unlock_irqrestore(&irqstatus_lock, flags);

	hdmi_handle_irq_work(r);

	return IRQ_HANDLED;
}

static int hdmi_enable_display(void)
{
	int r = 0;
	mutex_lock(&hdmi.lock);

	r = hdmi_start_display();

	r = request_irq(TI81XX_IRQ_HDMIINT, hdmi_irq_handler, 0,
			"HDMI", (void *)0);

	hdmi.status = TI81xx_EXT_ENCODER_ENABLED;
	if (hdmi.frame_start_event != 0){
		hdmi.frame_start_event();
	}
	mutex_unlock(&hdmi.lock);
	return r;
}

static void hdmi_disable_display(void)
{

	mutex_lock(&hdmi.lock);
	/* Free irq befor disabling the hardware else it will generate continous
	   interrupts.
	 */
	if (TI81xx_EXT_ENCODER_DISABLED != hdmi.status) {
		free_irq(TI81XX_IRQ_HDMIINT, NULL);

		hdmi_stop_display();
		/*setting to default only in case of disable and not suspend*/
		hdmi.mode = 1 ; /* HDMI */

		hdmi.status = TI81xx_EXT_ENCODER_DISABLED;
		edid_set = false;
	}

	mutex_unlock(&hdmi.lock);
}


static int hdmi_start_display(void)
{
	int r;

	r = hdmi_set_power();
	if (r) {
		ERR("failed to power on HDMI device\n");
		goto err;
	}

err:
	return r;
}


/**
 * hdmi_init() - Initialize TI81XX HDMI Driver
 */

int __init hdmi_init(void)
{
	int r = 0;

	THDMIDBG("Initializing HDMI\n");
	mutex_init(&hdmi.lock);

	/* Devices CEC default state */
	hdmi.cec_state = HDMI_CEC_BYPASS;

	/* Initialize globals */
	hdmi.base_pll = 0;
	hdmi.base_phy = 0;
	hdmi.base_prcm = 0;

	hdmi.isr_cb = 0;
	hdmi.frame_start_event = 0;
	hdmi.done_ctl = 0;
	hdmi.wait_event_ctl = 0;
	hdmi.query_status_ctl = 0;
	hdmi.disable_ctl = 0;
	hdmi.enable_ctl = 0;

	if (cpu_is_ti814x()) {
		hdmi.base_pll =  ioremap(TI814x_HDMI_PLL_BASE_ADDR, 0x80);
		if (!hdmi.base_pll) {
			ERR("DM814x can't ioremap pll\n");
			return -ENOMEM;
			}
		else{
			THDMIDBG("DM814x ioremap pll-DONE hdmi.base_pll = %x\n",
				 (int) hdmi.base_pll);
		}

		hdmi.base_phy = ioremap(TI814x_HDMI_PHY_0_REGS, 64);
		if (!hdmi.base_phy) {
			ERR("can't ioremap phy\n");
			r = -ENOMEM;
			goto err_unmap;
		}
	}

	if (cpu_is_ti816x()) {
		hdmi.base_phy = ioremap(TI816x_HDMI_PHY_0_REGS, 64);
		if (!hdmi.base_phy) {
			ERR("can't ioremap phy\n");
			r = -ENOMEM;
			goto err_unmap;
		}
	}

	hdmi.base_prcm =  ioremap(PRCM_0_REGS, 0x500);
	if (!hdmi.base_prcm) {
		ERR("can't ioremap PRCM 0\n");
			r = -ENOMEM;
			goto err_unmap;
	}


	if (cpu_is_ti816x())
		ti816x_enable_hdmi_clocks((u32)hdmi.base_prcm);
	if (cpu_is_ti814x())
		ti814x_enable_hdmi_clocks((u32)hdmi.base_prcm);
	if (cpu_is_ti814x())
		ti814x_enable_ddc_pinmux();

	hdmi_lib_init();

#ifdef CONFIG_TI81XX_HDMI_HDCP
	hdcp_init();
#endif

	hdmi_major = MAJOR(hdmi_dev_id);

	r = alloc_chrdev_region(&hdmi_dev_id, 0, 1, "onchip_hdmi_device");
	if (r) {
		THDMIDBG(KERN_WARNING "HDMI: Cound not register char device\n");
		r = -ENOMEM;
		goto err_uninit_lib;
	}
	/* initialize character device */
	cdev_init(&hdmi_cdev, &hdmi_fops);
	hdmi_cdev.owner = THIS_MODULE;
	hdmi_cdev.ops = &hdmi_fops;

	/* add char driver */
	r = cdev_add(&hdmi_cdev, hdmi_dev_id, 1);
	if (r) {
		THDMIDBG(KERN_WARNING "HDMI: Could not add hdmi char driver\n");
		r = -ENOMEM;
		goto err_unregister_chrdev;
	}
	irq_wq = create_singlethread_workqueue("HDMI WQ");

	/* register driver as a platform driver */
	r = platform_driver_register(&ti81xx_hdmi_driver);
	if (r) {
		THDMIDBG("TI81xx_hdmi: Could register driver\n");
		goto err_remove_cdev;
	}

	r = TI81xx_register_display_panel(&hdmi_driver, &hdmi.vencinfo);
	if (r) {
		THDMIDBG("TI81xx_hdmi: Registered to display controller\n");
		goto err_unregister_platform_driver;
	}
	r = hdmi_set_output(TI81xx_OUTPUT_HDMI, &hdmi.vencinfo.outinfo, NULL);
	if (r) {
		THDMIDBG("TI81xx_hdmi: Setting output failed\n");
		goto err_unregister_panel;
	}

	r = hdmi_set_timings(&hdmi.vencinfo.vtimings, NULL);
	if (r) {
		THDMIDBG("TI81xx_hdmi: Not able to set timings on chip HDMI\n");
		/*Do not exit since we need enable the HDMi even
		 timing is not right for some other VESA format.
		 with add proper code once we support all VESA mode*/
		/* goto err_unregister_panel;*/
	}
	if (hdmi.vencinfo.enabled) {
		r = hdmi_enable_display();
		if (r) {
			THDMIDBG("TI81xx_hdmi: Not able to enable HDMI\n");
			goto err_unregister_panel;
		}
	}
	THDMIDBG("%s %d\n", __func__, __LINE__);
	return r;
err_unregister_panel:
	TI81xx_un_register_display_panel(&hdmi_driver);
err_unregister_platform_driver:
	platform_driver_unregister(&ti81xx_hdmi_driver);
err_remove_cdev:
	cdev_del(&hdmi_cdev);
err_unregister_chrdev:
	unregister_chrdev_region(hdmi_dev_id, 1);
err_uninit_lib:
	hdmi_lib_exit();
	hdcp_exit();
err_unmap:
	if (hdmi.base_pll)
		iounmap(hdmi.base_pll);
	if (hdmi.base_phy)
		iounmap(hdmi.base_phy);
	if (hdmi.base_prcm)
		iounmap(hdmi.base_prcm);

	return r;
}


void ti81xx_map_timings_to_code(struct TI81xx_video_timings *timings)
{
	/* populate hdmi.code, hdmi.mode etc */
	cm = hdmi_get_code(timings);
	hdmi.code = cm.code;
	hdmi.mode = cm.mode;
	hdmi.cfg.pixel_clock = timings->pixel_clock;
	hdmi.cfg.hbp = timings->hbp;
	hdmi.cfg.hfp = timings->hfp;
	hdmi.cfg.hsw = timings->hsw;
	hdmi.cfg.vfp = timings->vfp;
	hdmi.cfg.vbp = timings->vbp;
	hdmi.cfg.vsw = timings->vsw;
	hdmi.cfg.interlace = timings->scanformat ? 0 : 1;
	hdmi.cfg.ppl = timings->width;
	hdmi.cfg.lpp = timings->height >> hdmi.cfg.interlace;
	return;
}

void hdmi_exit(void)
{
	destroy_workqueue(irq_wq);
	hdmi_disable_display();
	if (cpu_is_ti814x())
		ti814x_set_clksrc_mux(0);
	hdmi_lib_exit();
	TI81xx_un_register_display_panel(&hdmi_driver);
	device_destroy(ti81xx_hdmi_class, hdmi_dev_id);
	class_destroy(ti81xx_hdmi_class);
	cdev_del(&hdmi_cdev);
	platform_driver_unregister(&ti81xx_hdmi_driver);
	unregister_chrdev_region(hdmi_dev_id, 1);


	iounmap(hdmi.base_pll);
	iounmap(hdmi.base_phy);
	iounmap(hdmi.base_prcm);
}

static void hdmi_stop_display(void)
{
	hdmi_power_off();
}


static int hdmi_panel_enable(void *data)
{
	int r;
	THDMIDBG("\n Enter Panel function : hdmi_panel_enable()\n ");
	r = hdmi_enable_display();
	return r;
}


static int hdmi_panel_disable(void *data)
{
	THDMIDBG("\n Enter Panel function : hdmi_panel_disable()\n ");
	hdmi_disable_display();
	return 0;
}

static int hdmi_panel_suspend(void *data)
{
	THDMIDBG("\n Enter Panel function : hdmi_panel_suspend()\n ");
	hdmi_display_suspend();
	return 0;
}

static int hdmi_panel_resume(void *data)
{
	THDMIDBG("\n Enter Panel function : hdmi_panel_resume()\n ");
	hdmi_display_resume();
	return 0;
}
static int hdmi_set_output(enum TI81xx_outputs outputs,
		struct vps_dcoutputinfo *outinfo, void *data)
{
	memcpy(&hdmi.vencinfo.outinfo, outinfo,
			sizeof(struct vps_dcoutputinfo));
	if (outinfo->dataformat == FVID2_DF_RGB24_888) {
		hdmi.cfg.input_df = HDMI_DF_RGB;
		hdmi.cfg.output_df = HDMI_DF_RGB;
		if (outinfo->dvofmt  == (u32)VPS_DC_DVOFMT_TRIPLECHAN_EMBSYNC)
			hdmi.cfg.sync = HDMI_EMBEDDED_SYNC;
		else if (outinfo->dvofmt  ==
		    (u32)VPS_DC_DVOFMT_TRIPLECHAN_DISCSYNC)
			hdmi.cfg.sync = HDMI_DISCRETE_SYNC;
		else
			return -EINVAL;
	} else if (outinfo->dataformat == FVID2_DF_YUV422SP_UV &&
			outinfo->dvofmt == VPS_DC_DVOFMT_TRIPLECHAN_EMBSYNC) {
		hdmi.cfg.input_df = HDMI_DF_YUV444;
		hdmi.cfg.output_df = HDMI_DF_YUV422;
		hdmi.cfg.sync = HDMI_EMBEDDED_SYNC;
	} else if (outinfo->dataformat == FVID2_DF_YUV422SP_UV &&
			outinfo->dvofmt == VPS_DC_DVOFMT_TRIPLECHAN_DISCSYNC) {
		hdmi.cfg.input_df = HDMI_DF_YUV444;
		hdmi.cfg.output_df = HDMI_DF_YUV422;
		hdmi.cfg.sync = HDMI_DISCRETE_SYNC;
	} else if (outinfo->dataformat == FVID2_DF_YUV422SP_UV &&
			outinfo->dvofmt == VPS_DC_DVOFMT_DOUBLECHAN) {
		hdmi.cfg.input_df = HDMI_DF_YUV422;
		hdmi.cfg.output_df = HDMI_DF_YUV422;
		hdmi.cfg.sync = HDMI_EMBEDDED_SYNC;
	} else if (outinfo->dataformat == FVID2_DF_YUV422SP_UV &&
			outinfo->dvofmt == VPS_DC_DVOFMT_DOUBLECHAN_DISCSYNC) {
		hdmi.cfg.input_df = HDMI_DF_YUV422;
		hdmi.cfg.output_df = HDMI_DF_YUV422;
		hdmi.cfg.sync = HDMI_DISCRETE_SYNC;
	} else if (outinfo->dataformat == FVID2_DF_YUV444P) {
		hdmi.cfg.input_df = HDMI_DF_YUV444;
		hdmi.cfg.output_df = HDMI_DF_YUV444;
		if (outinfo->dvofmt == (u32)VPS_DC_DVOFMT_TRIPLECHAN_EMBSYNC)
			hdmi.cfg.sync = HDMI_EMBEDDED_SYNC;
		else if (outinfo->dvofmt ==
		    (u32)VPS_DC_DVOFMT_TRIPLECHAN_DISCSYNC)
			hdmi.cfg.sync = HDMI_DISCRETE_SYNC;
		else
			return -EINVAL;

	} else {
		hdmi.cfg.input_df = HDMI_DF_RGB;
		hdmi.cfg.output_df = HDMI_DF_RGB;
		return -EINVAL;
	}
	return 0;
}
static int hdmi_get_panel_edid(char *inbuf, void *data)
{
	int hpd_state;
	THDMIDBG("\n Enter Panel function : hdmi_get_panel_edid()\n");

	/* TODo : Fixme:: Maybe check HPD status and also whether HDCP is ON,
	   before getting edid */

	if (!inbuf)
		return -1;
	hpd_state = hdmi_get_hpd_pin_state();

	mutex_lock(&hdmi.lock);


	if (0 != hdmi_get_edid()) {
		mutex_unlock(&hdmi.lock);
		return -1;
	}

	mutex_unlock(&hdmi.lock);

	memcpy(inbuf, edid, HDMI_EDID_MAX_LENGTH);
	return HDMI_EDID_MAX_LENGTH;

}

static int hdmi_set_timings(struct TI81xx_video_timings *timings, void *data)
{
	THDMIDBG("\n Enter Panel function : hdmi_set_timings()\n");
	mutex_lock(&hdmi.lock);
	if (cpu_is_ti814x()) {
		if (timings->pixel_clock >= 100000)
			hdmi.freq = 0x2;
		else if (timings->pixel_clock < 50000)
			hdmi.freq = 0;
		else
			hdmi.freq = 0x1;
	}

	/* In OMAP, to change the resolutions, driver stopped and then restarted
	 * However in slave mode of 81xx, the VENC has to be stopped to change
	 * timings. The middle layer(dctrl.c) will disable the display, when
	 * VENC stopped.
	 */
	ti81xx_map_timings_to_code(timings);
	mutex_unlock(&hdmi.lock);

	return 0;
}

static int ti81xx_hdmi_probe(struct platform_device *device)
{
	int result = 0;
	THDMIDBG("TI81xx_hdmi: probe\n");

	/* This is needed, else the driver is not visible under /dev/... */
	ti81xx_hdmi_class = class_create(THIS_MODULE, TI81XX_HDMI_DRIVER_NAME);
	if (IS_ERR(ti81xx_hdmi_class)) {
		result = -EIO;
		THDMIDBG("TI81xx_hdmi: Could not create class\n");
		goto err_remove_class;
	}

	ti81xx_hdmi_device = device_create(ti81xx_hdmi_class, NULL,
			hdmi_dev_id, NULL,
			TI81XX_HDMI_DRIVER_NAME);
	if (IS_ERR(ti81xx_hdmi_device)) {
		result = -EIO;
		THDMIDBG("TI81xx_hdmi: Cound not create device file\n");
		goto err_remove_class;
	}
	return result;
err_remove_class:
	class_destroy(ti81xx_hdmi_class);

	return result;

}
static int ti81xx_hdmi_remove(struct platform_device *device)
{
	THDMIDBG("TI81xx_hdmi: remove\n");
	return 0;
}


module_init(hdmi_init);
module_exit(hdmi_exit);
MODULE_DESCRIPTION("TI TI81XX on-chip HDMI driver");
MODULE_AUTHOR("Varada Bellary<varadab@ti.com>");
MODULE_LICENSE("GPL v2");
