/*
 *
 * Framebuffer device registration for TI TI816x platforms
 *
 * Copyright (C) 2009 Texas Instruments Inc.
 * Author: Yihe Hu <yihehu@ti.com>
 *
 * Some code and ideas taken from TI OMAP2 Platforms
 * by Tomi Valkeinen.
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
 * 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/bootmem.h>
#include <linux/io.h>
#include <linux/vps_capture.h>
#include <linux/ti81xxfb.h>
#include <linux/ti81xx.h>
#include <mach/hardware.h>
#include <mach/board-ti814x.h>
#include <mach/board-ti816x.h>
#include <mach/board-dm813x.h>
#include <mach/board-ti811x.h>
#include <asm/mach/map.h>


#if defined(CONFIG_TI81XX_VPSS) || defined(CONFIG_TI81XX_VPSS_MODULE)

static u64 ti81xx_dma_mask = ~(u32)0;
static struct platform_device vpss_device = {
	.name = "vpss",
	.id = -1,
	.dev = {
		.platform_data = NULL,
	},
};
static struct vps_platform_data vps_pdata;


static int __init ti81xx_vpss_init(void)
{
	/*FIXME add platform data here*/
	int r;
	if (cpu_is_ti816x() || cpu_is_dm385()) {
		if (cpu_is_dm385()) {
			vps_pdata.cpu = CPU_DM813X;
#if !defined(CONFIG_MACH_DM385IPNC) && !defined(CONFIG_MACH_TI8148IPNC)
			/*setup the ths filter functioin*/
			vps_pdata.pcf_ths_init = dm813x_pcf8575_init;
			vps_pdata.pcf_ths_exit = dm813x_pcf8575_exit;
			vps_pdata.pcf_ths_hd_set =
					dm813x_pcf8575_ths7360_hd_enable;
			vps_pdata.pcf_ths_sd_set =
					dm813x_pcf8575_ths7360_sd_enable;
#endif
		} else {
			vps_pdata.cpu = CPU_DM816X;
#if !defined(CONFIG_MACH_DM385IPNC) && !defined(CONFIG_MACH_TI8148IPNC)
			/*setup the ths filter functioin*/
			vps_pdata.pcf_ths_init = ti816x_pcf8575_init;
			vps_pdata.pcf_ths_exit = ti816x_pcf8575_exit;
			vps_pdata.pcf_ths_hd_set = pcf8575_ths7360_hd_enable;
			vps_pdata.pcf_ths_sd_set = pcf8575_ths7360_sd_enable;
#endif
		}
		vps_pdata.numvencs = 4;
		vps_pdata.vencmask = (1 << VPS_DC_MAX_VENC) - 1;
	} else if (cpu_is_ti811x()) {
		vps_pdata.cpu = CPU_DM811X;
		vps_pdata.numvencs = 3;
		vps_pdata.vencmask = (1 << VPS_DC_MAX_VENC) - 1 \
					- VPS_DC_VENC_HDCOMP;
#if !defined(CONFIG_MACH_DM385IPNC) && !defined(CONFIG_MACH_TI8148IPNC)
		/*setup the ths filter functioin*/
		vps_pdata.pcf_ths_init = ti811x_pcf8575_init;
		vps_pdata.pcf_ths_exit = ti811x_pcf8575_exit;
		vps_pdata.pcf_ths_hd_set = NULL;
		vps_pdata.pcf_ths_sd_set = NULL;
#endif
	} else if (cpu_is_ti814x()) {
		vps_pdata.cpu = CPU_DM814X;
		vps_pdata.numvencs = 3;
		vps_pdata.vencmask = (1 << VPS_DC_MAX_VENC) - 1 \
					- VPS_DC_VENC_HDCOMP;
#if !defined(CONFIG_MACH_DM385IPNC) && !defined(CONFIG_MACH_TI8148IPNC)
		/*setup the ths filter functioin*/
		vps_pdata.pcf_ths_init = ti814x_pcf8575_init;
		vps_pdata.pcf_ths_exit = ti814x_pcf8575_exit;
		vps_pdata.pcf_ths_hd_set = NULL;
		vps_pdata.pcf_ths_sd_set = NULL;
#endif
	}

	/*set up the grpx connections*/
	vps_pdata.numgrpx = VPS_DISP_GRPX_MAX_INST;
	/*grpx0 out to hdmi*/
	vps_pdata.gdata[0].snode = VPS_DC_GRPX0_INPUT_PATH;
	vps_pdata.gdata[0].numends = 1;
	vps_pdata.gdata[0].enode[0] = VPS_DC_HDMI_BLEND;

	/*grpx1 out to HDCOMP(DM816X) or DVO2(other platform)*/
	vps_pdata.gdata[1].snode = VPS_DC_GRPX1_INPUT_PATH;
	vps_pdata.gdata[1].numends = 1;
	if (cpu_is_ti816x())
		vps_pdata.gdata[1].enode[0] = VPS_DC_HDCOMP_BLEND;
	else
		vps_pdata.gdata[1].enode[0] = VPS_DC_DVO2_BLEND;
	/*grpx2 out to SD*/
	vps_pdata.gdata[2].snode = VPS_DC_GRPX2_INPUT_PATH;
	vps_pdata.gdata[2].numends = 1;
	vps_pdata.gdata[2].enode[0] = VPS_DC_SDVENC_BLEND;


	/*set up the v4l2 video display connections*/
	vps_pdata.numvideo = 3;
	/*/dev/video1 -> HDMI*/
	vps_pdata.vdata[0].idx = 0;
	vps_pdata.vdata[0].numedges = 2;
	vps_pdata.vdata[0].snodes[0] = VPS_DC_VCOMP_MUX;
	vps_pdata.vdata[0].snodes_inputid[0] = VPS_DC_BP0_INPUT_PATH;
	vps_pdata.vdata[0].snodes[1] = VPS_DC_VCOMP;
	vps_pdata.vdata[0].snodes_inputid[1] = VPS_DC_VCOMP_MUX;
	vps_pdata.vdata[0].numoutput = 1;
	vps_pdata.vdata[0].enodes[0] = VPS_DC_HDMI_BLEND;
	vps_pdata.vdata[0].enodes_inputid[0] =
			VPS_DC_CIG_NON_CONSTRAINED_OUTPUT;

	/*/dev/video2 -> HDCOMP or DVO2*/
	vps_pdata.vdata[1].idx = 1;
	vps_pdata.vdata[1].numedges = 2;
	vps_pdata.vdata[1].snodes[0] = VPS_DC_HDCOMP_MUX;
	vps_pdata.vdata[1].snodes_inputid[0] = VPS_DC_BP1_INPUT_PATH;
	vps_pdata.vdata[1].snodes[1] = VPS_DC_CIG_PIP_INPUT;
	vps_pdata.vdata[1].snodes_inputid[1] = VPS_DC_HDCOMP_MUX;
	vps_pdata.vdata[1].numoutput = 1;
	if (cpu_is_ti816x())
		vps_pdata.vdata[1].enodes[0] = VPS_DC_HDCOMP_BLEND;
	else
		vps_pdata.vdata[1].enodes[0] = VPS_DC_DVO2_BLEND;

	vps_pdata.vdata[1].enodes_inputid[0] = VPS_DC_CIG_PIP_OUTPUT;

	/*/dev/video3 is SD only*/
	vps_pdata.vdata[2].idx = 2;
	vps_pdata.vdata[2].numedges = 1;
	vps_pdata.vdata[2].snodes_inputid[0] = VPS_DC_SEC1_INPUT_PATH;
	vps_pdata.vdata[2].snodes[0] = VPS_DC_SDVENC_MUX;
	vps_pdata.vdata[2].numoutput = 1;
	vps_pdata.vdata[2].enodes[0] = VPS_DC_SDVENC_BLEND;
	vps_pdata.vdata[2].enodes_inputid[0] = VPS_DC_SDVENC_MUX;

	vpss_device.dev.platform_data = &vps_pdata;
	r = platform_device_register(&vpss_device);
	if (r)
		printk(KERN_ERR "unable to register ti81xx_vpss device\n");
	else
		printk(KERN_INFO "registered ti81xx_vpss device\n");
	return r;
}

#if defined(CONFIG_TI81XX_HDMI_MODULE) || defined(CONFIG_TI81XX_HDMI)

static struct platform_device ti81xx_hdmi_plat_device = {
	.name = "TI81XX_HDMI",
	.id = -1,
	.num_resources = 0,
	.dev = {
		/*.release = ti81xx_hdmi_platform_release,*/
		.platform_data = NULL,
	}
};

static int __init ti81xx_hdmi_init(void)
{
	int r;
	/*FIXME add platform data here*/
	r = platform_device_register(&ti81xx_hdmi_plat_device);
	if (r)
		printk(KERN_ERR "Unable to register ti81xx onchip-HDMI device\n");
	else
		printk(KERN_INFO "registered ti81xx on-chip HDMI device\n");
	return r;
}
#else
static int __init ti81xx_hdmi_init(void)
{
	return 0;
}
#endif

#if defined(CONFIG_VIDEO_TI81XX_VIDIN_MODULE) || \
		defined(CONFIG_VIDEO_TI81XX_VIDIN)

#define HDVPSS_CAPTURE_INST0_BASE	0x48105500
#define HDVPSS_CAPTURE_INST0_SIZE	1024u

#define HDVPSS_CAPTURE_INST2_BASE	0x48105A00
#define HDVPSS_CAPTURE_INST2_SIZE	1024u
u8 ti81xx_card_name[] = "TI81xx_catalogue";
struct ti81xxvin_interface tvp7002_pdata = {
	.clk_polarity = 0,
	.hs_polarity = 0,
	.vs_polarity = 1,
	.fid_polarity = 0,
	.sog_polarity = 0,

};
static struct ti81xxvin_subdev_info hdvpss_capture_sdev_info[] = {
	{
		.name	= TVP7002_INST0,
		.board_info = {
			/* TODO Find the correct address
				of the TVP7002 connected */
			I2C_BOARD_INFO("tvp7002", 0x5d),
			.platform_data = &tvp7002_pdata,
		},
		.vip_port_cfg = {
			.ctrlChanSel = VPS_VIP_CTRL_CHAN_SEL_15_8,
			.ancChSel8b = VPS_VIP_ANC_CH_SEL_DONT_CARE,
			.pixClkEdgePol = VPS_VIP_PIX_CLK_EDGE_POL_RISING,
			.invertFidPol = 0,
			.embConfig = {
				.errCorrEnable = 1,
				.srcNumPos = VPS_VIP_SRC_NUM_POS_DONT_CARE,
				.isMaxChan3Bits = 0,
			},
			.disConfig = {
				.fidSkewPostCnt = 0,
				.fidSkewPreCnt = 0,
				.lineCaptureStyle =
					VPS_VIP_LINE_CAPTURE_STYLE_DONT_CARE,
				.fidDetectMode =
					VPS_VIP_FID_DETECT_MODE_DONT_CARE,
				.actvidPol = VPS_VIP_POLARITY_DONT_CARE,
				.vsyncPol =  VPS_VIP_POLARITY_DONT_CARE,
				.hsyncPol = VPS_VIP_POLARITY_DONT_CARE,
			}
		},
		.video_capture_mode =
		   VPS_CAPT_VIDEO_CAPTURE_MODE_SINGLE_CH_NON_MUX_EMBEDDED_SYNC,
		.video_if_mode = VPS_CAPT_VIDEO_IF_MODE_16BIT,
		.input_data_format = FVID2_DF_YUV422P,
	},
	{
		.name	= TVP7002_INST1,
		.board_info = {
			I2C_BOARD_INFO("tvp7002", 0x5c),
			.platform_data = &tvp7002_pdata,
		},
		.vip_port_cfg = {
			.ctrlChanSel = VPS_VIP_CTRL_CHAN_SEL_15_8,
			.ancChSel8b = VPS_VIP_ANC_CH_SEL_DONT_CARE,
			.pixClkEdgePol = VPS_VIP_PIX_CLK_EDGE_POL_RISING,
			.invertFidPol = 0,
			.embConfig = {
				.errCorrEnable = 1,
				.srcNumPos = VPS_VIP_SRC_NUM_POS_DONT_CARE,
				.isMaxChan3Bits = 0,
			},
			.disConfig = {
				.fidSkewPostCnt = 0,
				.fidSkewPreCnt = 0,
				.lineCaptureStyle =
					VPS_VIP_LINE_CAPTURE_STYLE_DONT_CARE,
				.fidDetectMode =
					VPS_VIP_FID_DETECT_MODE_DONT_CARE,
				.actvidPol = VPS_VIP_POLARITY_DONT_CARE,
				.vsyncPol =  VPS_VIP_POLARITY_DONT_CARE,
				.hsyncPol = VPS_VIP_POLARITY_DONT_CARE,
			}
		},
		.video_capture_mode =
		   VPS_CAPT_VIDEO_CAPTURE_MODE_SINGLE_CH_NON_MUX_EMBEDDED_SYNC,
		.video_if_mode = VPS_CAPT_VIDEO_IF_MODE_16BIT,
		.input_data_format = FVID2_DF_YUV422P,
	},
};

static const struct v4l2_dv_preset hdvpss_inst0_inp0_presets[] = {
	{
		.preset = V4L2_DV_720P60,
	},
	{
		.preset = V4L2_DV_1080I60,
	},
	{
		.preset = V4L2_DV_1080P60,
	},
	{
		.preset = V4L2_DV_1080P30,
	},
};

static const struct v4l2_dv_preset hdvpss_inst2_inp0_presets[] = {
	{
		.preset = V4L2_DV_720P60,
	},
	{
		.preset = V4L2_DV_1080I60,
	},
	{
		.preset = V4L2_DV_1080P60,
	},
	{
		.preset = V4L2_DV_1080P30,
	},
};

static const struct ti81xxvin_input hdvpss_inst0_inputs[] = {
	{
		.input = {
			.index		= 0,
			.name		= "Component",
			.type		= V4L2_INPUT_TYPE_CAMERA,
			.std		= V4L2_STD_UNKNOWN,
			.capabilities	= V4L2_OUT_CAP_PRESETS,
		},
		.subdev_name	= TVP7002_INST0,
		.dv_presets	= hdvpss_inst0_inp0_presets,
		.num_dv_presets	= ARRAY_SIZE(hdvpss_inst0_inp0_presets),
	},
};

static const struct ti81xxvin_input hdvpss_inst1_inputs[] = {
	{
		.input = {
			.index		= 0,
			.name		= "Component",
			.type		= V4L2_INPUT_TYPE_CAMERA,
			.std		= V4L2_STD_UNKNOWN,
			.capabilities	= V4L2_OUT_CAP_PRESETS,
		},
		.subdev_name	= TVP7002_INST1,
		.dv_presets	= hdvpss_inst2_inp0_presets,
		.num_dv_presets	= ARRAY_SIZE(hdvpss_inst2_inp0_presets),
	},
};

/* 16 bit decoders are present on the Port A of VIP0 and VIP1 instances. Which
represents the VIP0 and VIP2 instances in software. While Port B of VIP0 and
VIP1 are represented by VIP1 and VIP3 instances. On these two instances no
decoders are present.
*/
static struct ti81xxvin_config ti81xx_hsvpss_capture_cfg = {
	.subdev_info = hdvpss_capture_sdev_info,
	.subdev_count = ARRAY_SIZE(hdvpss_capture_sdev_info),
	.card_name = ti81xx_card_name,
	.inst_config[0] = {
		.inputs = hdvpss_inst0_inputs,
		.input_count = ARRAY_SIZE(hdvpss_inst0_inputs),
	},
	.inst_config[1] = {
		.inputs = hdvpss_inst0_inputs,
		.input_count = 0,
	},
	.inst_config[2] = {
		.inputs = hdvpss_inst1_inputs,
		.input_count = ARRAY_SIZE(hdvpss_inst1_inputs),
	},
	.inst_config[3] = {
		.inputs = hdvpss_inst1_inputs,
		.input_count = 0,
	},

};

static struct resource ti81xx_hdvpss_capture_resource[] = {
	[0] = {
		.start = HDVPSS_CAPTURE_INST0_BASE,
		.end   = (HDVPSS_CAPTURE_INST0_BASE +
				HDVPSS_CAPTURE_INST0_SIZE - 1),
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = HDVPSS_CAPTURE_INST2_BASE,
		.end   = (HDVPSS_CAPTURE_INST2_BASE +
				HDVPSS_CAPTURE_INST2_SIZE - 1),
		.flags = IORESOURCE_MEM,
	},
};

static struct platform_device hdvpss_capture_dev = {
	.name		= "ti81xxvin",
	.id		= -1,
	.dev		= {
			.dma_mask		= &ti81xx_dma_mask,
			.coherent_dma_mask	= ~(u32)0,
	},
	.num_resources = 2,
	.resource = ti81xx_hdvpss_capture_resource,
};

static int __init ti81xx_vin_init(void)
{
	int r;
	hdvpss_capture_dev.dev.platform_data = &ti81xx_hsvpss_capture_cfg;
	if (cpu_is_ti814x()) {
		hdvpss_capture_sdev_info[0].ti81xxvin_select_decoder =
			vps_ti814x_select_video_decoder;
		hdvpss_capture_sdev_info[0].ti81xxvin_set_mode =
			vps_ti814x_set_tvp7002_filter;
		hdvpss_capture_sdev_info[0].decoder_id = 0;
		hdvpss_capture_sdev_info[0].i2c_adapter_id = 3;
		hdvpss_capture_sdev_info[1].ti81xxvin_select_decoder =
			NULL;
		hdvpss_capture_sdev_info[1].ti81xxvin_set_mode =
			NULL;
		hdvpss_capture_sdev_info[1].decoder_id = 0;
		hdvpss_capture_sdev_info[1].i2c_adapter_id = 3;
	} else {
		hdvpss_capture_sdev_info[0].ti81xxvin_select_decoder =
			vps_ti816x_select_video_decoder;
		hdvpss_capture_sdev_info[0].ti81xxvin_set_mode =
			vps_ti816x_set_tvp7002_filter;
		hdvpss_capture_sdev_info[0].decoder_id = 0;
		hdvpss_capture_sdev_info[0].i2c_adapter_id = 2;
		hdvpss_capture_sdev_info[1].ti81xxvin_select_decoder =
			NULL;
		hdvpss_capture_sdev_info[1].ti81xxvin_set_mode =
			NULL;
		hdvpss_capture_sdev_info[1].decoder_id = 0;
		hdvpss_capture_sdev_info[1].i2c_adapter_id = 2;
	}
	r = platform_device_register(&hdvpss_capture_dev);
	if (r)
		printk(KERN_ERR "unable to register ti81xx_vin device\n");
	else
		printk(KERN_INFO "registered ti81xx_vin device\n");
	return r;

}
#else
static int __init ti81xx_vin_init(void)
{
	return 0;
}

#endif

#if defined(CONFIG_FB_TI81XX_MODULE) || defined(CONFIG_FB_TI81XX)
static struct ti81xxfb_platform_data ti81xxfb_config;

static struct platform_device ti81xx_fb_device = {
	.name		= "ti81xxfb",
	.id		= -1,
	.dev = {
		.dma_mask		= &ti81xx_dma_mask,
		.coherent_dma_mask	= ~(u32)0,
		.platform_data		= &ti81xxfb_config,
	},
	.num_resources = 0,
};


void ti81xxfb_set_platform_data(struct ti81xxfb_platform_data *data)
{
	ti81xxfb_config = *data;
}

static int __init ti81xx_fb_init(void)
{
	int r;
	r = platform_device_register(&ti81xx_fb_device);
	if (r)
		printk(KERN_ERR "unable to register ti81xx_fb device\n");
	else
		printk(KERN_INFO "registered ti81xx_fb device\n");
	return r;

}
#else
static int __init ti81xx_fb_init(void)
{
	return 0;
}
void ti81xxfb_set_platform_data(struct ti81xxfb_platform_data *data)
{
}
#endif

#if defined(CONFIG_VIDEO_TI81XX_VIDOUT_MODULE) || \
		defined(CONFIG_VIDEO_TI81XX_VIDOUT)
static struct resource ti81xx_vidout_resource[VPS_DISPLAY_INST_MAX] = {
};

static struct platform_device ti81xx_vidout_device = {
	.name		= "t81xx_vidout",
	.num_resources  = ARRAY_SIZE(ti81xx_vidout_resource),
	.resource       = &ti81xx_vidout_resource[0],
	.id             = -1,
};

static int __init ti81xx_init_vout(void)
{
	int r;

	r = platform_device_register(&ti81xx_vidout_device);
	if (r)
		printk(KERN_ERR "Unable to register ti81xx_vidout device\n");
	else
		printk(KERN_INFO "registered ti81xx_vidout device\n");
	return r;
}
#else
static int __init ti81xx_init_vout(void)
{
	return 0;
}
#endif


static int __init ti81xx_init_vpss(void)
{
	int retval = 0;
	/*if vpss failed to register, none of the below could works*/
	if (ti81xx_vpss_init())
		return -1;
	retval = ti81xx_init_vout();
	retval += ti81xx_hdmi_init();
	retval += ti81xx_fb_init();
	retval += ti81xx_vin_init();
	return retval;
}

arch_initcall(ti81xx_init_vpss);

#endif
