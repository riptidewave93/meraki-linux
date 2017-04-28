/*
 * ti81xxhdmi_sii9022a.h
 *
 * Copyright (C) 2011 Texas Instruments
 * Author: Senthil Natarajan
 *
 * Sii9022A HDMI Driver Header
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
 *
 * Senthil <senthil.n@ti.com> May 2011 Header for I2C driver for Sii9022A
 */

#ifndef _TI81XX_HDMI_SII9022A_H_
#define _TI81XX_HDMI_SII9022A_H_

struct hdmi_video_encoder_create_params {
	u32 device_i2c_inst_id; /* I2C device instance ID to use 0 or 1 */
	u32 device_i2c_addr; /* I2C device address for each device */
	u32 inp_clk; /* input clock*/
	u32 hdmi_hot_plug_gpio_intr_line; /* HDMI hot plug GPIO int line */
	u32 sync_mode; /* Sync Mode */
	u32 clk_edge; /* clock edge to be used to latch data on.
				FALSE spacifies to latch on falling edge,
				raising edge otherwise */
};

struct hdmi_chipid {
	u32 device_id;		/* Device Id TPI */
	u32 device_prod_rev_id; /* Device Production Revision Id TPI  */
	u32 tpi_rev_id;		/* TPI Revision Id TPI */
	u32 hdcp_rev_tpi;		/* HDCP revision TPI */
};

struct hdmi_sii9022a_hpd_prms {
	u32 hpd_evt_pending; /* Hot Plug Connection Event Pending */
	u32 bus_error; /* Receiver Sense Event Pending or CTRL Bus Error */
	u32 hpd_status; /* Hot Plug Pin Current Status */
};

enum video_encoder_sync_mode {
	ENCODER_EXTERNAL_SYNC = 0, /* HDMI in external sync mode  */
	ENCODER_EMBEDDED_SYNC, /* Embedded sync mode */
	ENCODER_MAX_SYNC		 /* This should be the last mode id */
};

struct hdmi_sii9022a_mode_info {
	enum fvid2_standard standard;
	u32 modeCode;
	u32 pix_clk;
	u32 vfreq;
	u32 pixels;
	u32 lines;
	struct {
		u32 hbit_to_hsync;
		u32 field2offset;
		u32 hwidth;
		u32 vbit_to_vsync;
		u32 vwidth;
	} emb_sync_prms;
	struct {
		u32 de_delay;
		u32 de_top;
		u32 de_cnt;
		u32 de_line;
	} ext_sync_prms;
};

/* Device definition */
struct hdmi_sii9022a_obj {
	struct hdmi_video_encoder_create_params *create_args;
	struct hdmi_chipid sii9022a_chip_id; /* HDMI Chip id */
	struct hdmi_sii9022a_hpd_prms hpd;  /* HDMI Hot plug detect */
	u32 sync_cfg_reg; /* Sync Configuration Register */
	u32 sync_polarity_reg;
	u32 is_rgb_output;
	u32 in_bus_cfg;
	u32 standard;
	struct ti81xx_venc_info vencinfo;
	u32 venc_enabled;
};
#endif
