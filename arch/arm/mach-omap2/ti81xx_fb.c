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
#include <asm/mach/map.h>


#if (defined(CONFIG_FB_TI81XX) || defined(CONFIG_FB_TI81XX_MODULE) || \
	defined(CONFIG_ARCH_TI81XX))


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

static u64 ti81xx_fb_dma_mask = ~(u32)0;
static struct ti81xxfb_platform_data ti81xxfb_config;

static struct resource ti81xx_hdvpss_resource[] = {
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

static struct platform_device ti81xx_fb_device = {
	.name		= "ti81xxfb",
	.id		= -1,
	.dev = {
		.dma_mask		= &ti81xx_fb_dma_mask,
		.coherent_dma_mask	= ~(u32)0,
		.platform_data		= &ti81xxfb_config,
	},
	.num_resources = 0,
};
static struct platform_device hdvpss_capture_dev = {
	.name		= "ti81xxvin",
	.id		= -1,
	.dev		= {
			.dma_mask		= &ti81xx_fb_dma_mask,
			.coherent_dma_mask	= ~(u32)0,
	},
	.num_resources = 2,
	.resource = ti81xx_hdvpss_resource,
};

void ti81xxfb_set_platform_data(struct ti81xxfb_platform_data *data)
{
	ti81xxfb_config = *data;
}

static inline int ti81xx_init_fb(void)
{
	int retval = 0;
	retval =  platform_device_register(&ti81xx_fb_device);
	hdvpss_capture_dev.dev.platform_data = &ti81xx_hsvpss_capture_cfg;
	if (cpu_is_ti814x()) {
		hdvpss_capture_sdev_info[0].ti81xxvin_select_decoder =
			vps_ti814x_select_video_decoder;
		hdvpss_capture_sdev_info[0].ti81xxvin_set_mode =
			vps_ti814x_set_tvp7002_filter;
		hdvpss_capture_sdev_info[0].decoder_id = 0;
		hdvpss_capture_sdev_info[1].ti81xxvin_select_decoder =
			NULL;
		hdvpss_capture_sdev_info[1].ti81xxvin_set_mode =
			NULL;
		hdvpss_capture_sdev_info[1].decoder_id = 0;
	} else {
		hdvpss_capture_sdev_info[0].ti81xxvin_select_decoder =
			vps_ti816x_select_video_decoder;
		hdvpss_capture_sdev_info[0].ti81xxvin_set_mode =
			vps_ti816x_set_tvp7002_filter;
		hdvpss_capture_sdev_info[0].decoder_id = 0;
		hdvpss_capture_sdev_info[1].ti81xxvin_select_decoder =
			NULL;
		hdvpss_capture_sdev_info[1].ti81xxvin_set_mode =
			NULL;
		hdvpss_capture_sdev_info[1].decoder_id = 0;
	}
	retval += platform_device_register(&hdvpss_capture_dev);
	return retval;
}

arch_initcall(ti81xx_init_fb);

#endif
