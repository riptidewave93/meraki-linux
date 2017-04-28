/*
 * ti81xxhdmi_sii9022a.c
 *
 * Copyright (C) 2011 Texas Instruments
 * Author: Senthil Natarajan
 *
 * Sii9022A HDMI Driver
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
 * Senthil Natarajan <senthil.n@ti.com> May 2011 I2C driver for Sii9022A
 */

#include <linux/delay.h>
#include <linux/i2c.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sii9022a.h>
#include "../vpss/display_interface.h"
#include "sii9022a_drv.h"

#define SI9022A_TPI_INPUTBUS_PIXEL_REP_BIT4_MASK (0x10u)
#define SI9022A_I2C_RETRY_COUNT (5)
#define SII9022A_MODULE_NAME	"sii9022a"
#define SII9022A_CRC0 13
#define SII9022A_CRC1 0x82
#define SII9022A_CRC2 0x02
#define SII9022A_CRC3 13

static struct i2c_client *client;

/********************************  Global Variables *********************/

/* Global object storing all information related to all HDMI driver handles */
static struct hdmi_sii9022a_obj gsii9022a_obj;

struct hdmi_sii9022a_mode_info sii9022_mode_info[] = {
	{FVID2_STD_720P_60, 4, 7425, 60, 1650, 750,
	{110, 0, 40, 5, 5}, {260, 25, 1280, 720} } ,
	{FVID2_STD_720P_50, 19, 7425, 50, 1980, 750,
	{440, 0, 40, 5, 5}, {260, 25, 1280, 720} } ,
	{FVID2_STD_1080P_30, 34, 7425, 30, 2200, 1125,
	{88, 0, 44, 4, 5}, {192, 41, 1920, 1080} } ,
	{FVID2_STD_1080P_50, 16, 7425, 50, 2640, 1125,
	{528, 0, 44, 4, 5}, {192, 41, 1920, 1080} } ,
	{FVID2_STD_1080P_60, 16, 14850, 60, 2200, 1125,
	{88, 0, 44, 4, 5}, {192, 41, 1920, 1080} } ,
	{FVID2_STD_1080I_60, 5, 7425, 60, 2200, 1125,
	{88, 550, 44, 4, 5}, {192, 20, 1920, 540} } ,
	{FVID2_STD_1080I_50, 20, 7425, 50, 2640, 1125,
	{88, 550, 44, 4, 5}, {192, 20, 1920, 540} } ,
};

/********************************  Function Prototypes **********************/
static int sii9022a_panel_enable(void *data);
static int sii9022a_panel_disable(void *data);
static int sii9022a_panel_suspend(void *data);
static int sii9022a_panel_resume(void *data);
static int sii9022a_set_timings(struct TI81xx_video_timings *timings,
				void *data);

static struct TI81xx_display_driver hdmi_driver = {
	.display = TI81xx_DISPLAY_HDMI,
	.enable	= sii9022a_panel_enable,
	.disable = sii9022a_panel_disable,
	.suspend = sii9022a_panel_suspend,
	.resume	= sii9022a_panel_resume,
	.set_timing = sii9022a_set_timings,
};

/******************************** functions *********************/

static int sii9022a_set_timings(struct TI81xx_video_timings *timings,
				void *data)
{
	if (timings->standard == FVID2_STD_1080P_60)
		gsii9022a_obj.standard = FVID2_STD_1080P_60;
	else if (timings->standard == FVID2_STD_1080P_30)
		gsii9022a_obj.standard = FVID2_STD_1080P_30;
	else if (timings->standard == FVID2_STD_1080I_60)
		gsii9022a_obj.standard = FVID2_STD_1080I_60;
	else if (timings->standard == FVID2_STD_720P_60)
		gsii9022a_obj.standard = FVID2_STD_720P_60;
	else
		return -1;

	return 0;

}

static int sii9022a_read(u8 addr, u8 *dst)
{
	int retry;
	int error;

	for (retry = 0; retry < SI9022A_I2C_RETRY_COUNT; retry++) {
		error = i2c_smbus_read_byte_data(client, addr);

		if (error >= 0) {
			*dst = (u8)error;
			return 0;
		}
	}

	return error;
}

static int sii9022a_device_read(const u8 *reg_addr, u8 *reg_value, u32 num_regs)
{
	int status;
	u32 regId;
	u8 read_val = 0;
	u8 *read_ptr;

	if (reg_addr == NULL || reg_value == NULL || num_regs == 0)
		return -1;

	read_ptr = &read_val;

	for (regId = 0; regId < num_regs; regId++) {
		status = sii9022a_read(reg_addr[regId], read_ptr);
		reg_value[regId] = read_val;

		if (status != 0)
			break;
	}

	return status;
}

static int sii9022a_write(u8 addr, u8 value)
{
	int retry;
	int error;

	for (retry = 0; retry < SI9022A_I2C_RETRY_COUNT; retry++) {
		error = i2c_smbus_write_byte_data(client, addr, value);

		if (error >= 0)
			return 0;
	}

	return error;
}

static int sii9022a_device_write(const u8 *reg_addr, const u8 *reg_value,
	u32 num_regs)
{
	int status;
	u32 regId;
	u8 buffer[2];

	if (reg_addr == NULL || reg_value == NULL || num_regs == 0)
		return -1;

	for (regId = 0; regId < num_regs; regId++) {
		buffer[0] = reg_addr[regId];
		buffer[1] = reg_value[regId];

		status = sii9022a_write(buffer[0], buffer[1]);

		if (status != 0)
			break;
	 }

	return status;
}
/*
  Device Res-set
*/
static int sii9022a_reset(struct hdmi_sii9022a_obj *sii9022a_obj)
{
	int status = 0;
	u8 reg_addr[4];
	u8 reg_value[4];
	u8 num_regs;
	u32 rpt_cnt = 0;

	num_regs = 0;
	reg_addr[num_regs] = 0xc7;
	reg_value[num_regs] = 0x00;
	num_regs++;

	for (rpt_cnt = 0; rpt_cnt < 5; rpt_cnt++) {
		status = sii9022a_device_write(reg_addr, reg_value, num_regs);
		if (0 == status)
			break;
	}
	if (0 != status)
		status = -1;

	return status;

}

/*
  Device Enable
*/
static int sii9022a_enable(struct hdmi_sii9022a_obj *sii9022a_obj)
{
	int status = 0;
	u8 reg_addr[4];
	u8 reg_value[4];
	u8 num_regs;

	num_regs = 0;
	reg_addr[num_regs] = 0xBC;
	reg_value[num_regs] = 0x01;
	num_regs++;

	reg_addr[num_regs] = 0xBD;
	reg_value[num_regs] = 0x82;
	num_regs++;

	status = sii9022a_device_write(reg_addr, reg_value, num_regs);

	if (0 == status) {
		num_regs = 0;
		reg_addr[num_regs] = 0xBE;
		reg_value[num_regs] = 0x01;
		num_regs++;

		status = sii9022a_device_read(reg_addr, reg_value, num_regs);
		if (0 == status) {
			reg_value[0u] |= 0x01;
			status = sii9022a_device_write(reg_addr,
					reg_value, num_regs);
		}
	}

	return status;
}

/*
  Device Power up Transmitter
*/
static int sii9022a_power_up_txm(struct hdmi_sii9022a_obj *sii9022a_obj)
{
	int status = 0;
	u8 reg_addr;
	u8 reg_value = 0u;

	reg_addr = 0x1E;
	status = sii9022a_device_read(&reg_addr, &reg_value, 1u);;

	if (0 == status) {
		reg_value &= 0xFC;
		status = sii9022a_device_write(&reg_addr, &reg_value, 1);
	}

	return status;
}

/*
  Configure Input Bus and pixel repetation
*/
static int sii9022a_cfg_in_bus(struct hdmi_sii9022a_obj *sii9022a_obj)
{
	int status = 0;
	u8 reg_addr;
	u8 reg_value;

	reg_addr = 0x08;
	reg_value = sii9022a_obj->in_bus_cfg;

	status = sii9022a_device_write(&reg_addr, &reg_value, 1);

	return status;
}

/*
  Configure YC Mode
*/
static int sii9022a_cfg_yc_mode(struct hdmi_sii9022a_obj *sii9022a_obj)
{
	int status = 0;
	u8 reg_addr;
	u8 reg_value;

	reg_addr = 0x0B;
	reg_value = 0x00;

	status = sii9022a_device_write(&reg_addr, &reg_value, 1);

	return status;
}

/*
  Configure Sync Mode
*/
static int sii9022a_cfg_sync_mode(struct hdmi_sii9022a_obj *sii9022a_obj)
{
	int status = 0;
	u8 reg_addr;
	u8 reg_value;

	reg_addr = 0x60;
	reg_value = sii9022a_obj->sync_cfg_reg;
	status = sii9022a_device_write(&reg_addr, &reg_value, 1);
	return status;
}

/*
  Get TP5158 chip ID, revision ID and firmware patch ID
*/
static int sii9022a_get_chip_id(struct hdmi_sii9022a_obj *sii9022a_obj)
{
	int status = 0;
	u8 reg_addr[8];
	u8 reg_value[8];
	u8 num_regs;

	num_regs = 0;
	reg_addr[num_regs] = 0x1b;
	reg_value[num_regs] = 0;
	num_regs++;

	reg_addr[num_regs] = 0x1c;
	reg_value[num_regs] = 0;
	num_regs++;

	reg_addr[num_regs] = 0x1d;
	reg_value[num_regs] = 0;
	num_regs++;

	reg_addr[num_regs] = 0x30;
	reg_value[num_regs] = 0;
	num_regs++;

	status = sii9022a_device_read(reg_addr, reg_value, num_regs);

	if (0 != status)
		status = -1;

	if (0 == status) {
		sii9022a_obj->sii9022a_chip_id.device_id =
			reg_value[0];
		sii9022a_obj->sii9022a_chip_id.device_prod_rev_id =
			reg_value[1];
		sii9022a_obj->sii9022a_chip_id.tpi_rev_id =
			reg_value[2];
		sii9022a_obj->sii9022a_chip_id.hdcp_rev_tpi =
			reg_value[3];
	}

	return status;
}

static int sii9022a_prgm_emb_sync_timing(
	struct hdmi_sii9022a_obj *sii9022a_obj,
	struct hdmi_sii9022a_mode_info *si_mode_info)
{
	int ret_val;
	u8 reg_value[20], reg_addr[20];
	u32 num_regs;

	num_regs = 0;
	reg_addr[num_regs] = 0x62;
	reg_value[num_regs] = si_mode_info->emb_sync_prms.hbit_to_hsync & 0xFF;
	num_regs++;

	reg_addr[num_regs] = 0x63;
	sii9022a_obj->sync_polarity_reg &= ~(0x03);
	sii9022a_obj->sync_polarity_reg |=
		((si_mode_info->emb_sync_prms.hbit_to_hsync & 0x300) >> 8);
	reg_value[num_regs] = sii9022a_obj->sync_polarity_reg & 0xFF;
	num_regs++;

	reg_addr[num_regs] = 0x64;
	reg_value[num_regs] = si_mode_info->emb_sync_prms.field2offset & 0xFF;
	num_regs++;

	reg_addr[num_regs] = 0x65;
	reg_value[num_regs] =
		(si_mode_info->emb_sync_prms.field2offset & 0xF00) >> 8;
	num_regs++;

	reg_addr[num_regs] = 0x66;
	reg_value[num_regs] = si_mode_info->emb_sync_prms.hwidth & 0xFF;
	num_regs++;

	reg_addr[num_regs] = 0x67;
	reg_value[num_regs] = (si_mode_info->emb_sync_prms.hwidth & 0x300) >> 8;
	num_regs++;

	reg_addr[num_regs] = 0x68;
	reg_value[num_regs] = si_mode_info->emb_sync_prms.vbit_to_vsync & 0x3F;
	num_regs++;

	reg_addr[num_regs] = 0x69;
	reg_value[num_regs] = si_mode_info->emb_sync_prms.vwidth & 0x3F;
	num_regs++;

	reg_addr[num_regs] = 0x00;
	reg_value[num_regs] = si_mode_info->pix_clk & 0xFF;
	num_regs++;

	reg_addr[num_regs] = 0x01;
	reg_value[num_regs] = (si_mode_info->pix_clk & 0xFF00) >> 8;
	num_regs++;

	reg_addr[num_regs] = 0x02;
	reg_value[num_regs] = si_mode_info->vfreq & 0xFF;
	num_regs++;

	reg_addr[num_regs] = 0x03;
	reg_value[num_regs] = (si_mode_info->vfreq & 0xFF00) >> 8;
	num_regs++;

	reg_addr[num_regs] = 0x04;
	reg_value[num_regs] = si_mode_info->pixels & 0xFF;
	num_regs++;

	reg_addr[num_regs] = 0x05;
	reg_value[num_regs] = (si_mode_info->pixels & 0xFF00) >> 8;
	num_regs++;

	reg_addr[num_regs] = 0x06;
	reg_value[num_regs] = si_mode_info->lines & 0xFF;
	num_regs++;

	reg_addr[num_regs] = 0x07;
	reg_value[num_regs] = (si_mode_info->lines & 0xFF00) >> 8;
	num_regs++;

	reg_addr[num_regs] = 0x08;
	reg_value[num_regs] = sii9022a_obj->in_bus_cfg;
	num_regs++;

	reg_addr[num_regs] = 0x09;
	reg_value[num_regs] = 0x02;
	num_regs++;

	reg_addr[num_regs] = 0x0A;
	reg_value[num_regs] = 0x12;
	num_regs++;
	sii9022a_obj->is_rgb_output = 0;

	ret_val = sii9022a_device_write(reg_addr, reg_value, num_regs);

	return ret_val;
}

/*
 * Program Timing information for the separate sync input
 */
static int sii9022a_prgm_ext_sync_timing(
	struct hdmi_sii9022a_obj *sii9022a_obj,
	struct hdmi_sii9022a_mode_info *si_mode_info)
{
	int ret_val;
	u8 reg_value[20], reg_addr[20];
	u32 num_regs;

	num_regs = 0;
	reg_addr[num_regs] = 0x62;
	reg_value[num_regs] = si_mode_info->ext_sync_prms.de_delay & 0xFF;
	num_regs++;

	reg_addr[num_regs] = 0x63;
	sii9022a_obj->sync_polarity_reg &= ~(0x03);
	sii9022a_obj->sync_polarity_reg |=
		((si_mode_info->ext_sync_prms.de_delay & 0x300) >> 8);
	reg_value[num_regs] = sii9022a_obj->sync_polarity_reg & 0xFF;
	num_regs++;

	reg_addr[num_regs] = 0x64;
	reg_value[num_regs] = si_mode_info->ext_sync_prms.de_top & 0xFF;
	num_regs++;

	reg_addr[num_regs] = 0x66;
	reg_value[num_regs] = si_mode_info->ext_sync_prms.de_cnt & 0xFF;
	num_regs++;

	reg_addr[num_regs] = 0x67;
	reg_value[num_regs] = (si_mode_info->ext_sync_prms.de_cnt & 0xF00) >> 8;
	num_regs++;

	reg_addr[num_regs] = 0x68;
	reg_value[num_regs] = si_mode_info->ext_sync_prms.de_line & 0xFF;
	num_regs++;

	reg_addr[num_regs] = 0x69;
	reg_value[num_regs] =
		(si_mode_info->ext_sync_prms.de_line & 0x700) >> 8;
	num_regs++;

	reg_addr[num_regs] = 0x00;
	reg_value[num_regs] = si_mode_info->pix_clk & 0xFF;
	num_regs++;

	reg_addr[num_regs] = 0x01;
	reg_value[num_regs] = (si_mode_info->pix_clk & 0xFF00) >> 8;
	num_regs++;

	reg_addr[num_regs] = 0x02;
	reg_value[num_regs] = si_mode_info->vfreq & 0xFF;
	num_regs++;

	reg_addr[num_regs] = 0x03;
	reg_value[num_regs] = (si_mode_info->vfreq & 0xFF00) >> 8;
	num_regs++;

	reg_addr[num_regs] = 0x04;
	reg_value[num_regs] = si_mode_info->pixels & 0xFF;
	num_regs++;

	reg_addr[num_regs] = 0x05;
	reg_value[num_regs] = (si_mode_info->pixels & 0xFF00) >> 8;
	num_regs++;

	reg_addr[num_regs] = 0x06;
	reg_value[num_regs] = si_mode_info->lines & 0xFF;
	num_regs++;

	reg_addr[num_regs] = 0x07;
	reg_value[num_regs] = (si_mode_info->lines & 0xFF00) >> 8;
	num_regs++;

	reg_addr[num_regs] = 0x08;
	reg_value[num_regs] = sii9022a_obj->in_bus_cfg;
	num_regs++;

	reg_addr[num_regs] = 0x09;
	reg_value[num_regs] = 0x00;
	num_regs++;

	reg_addr[num_regs] = 0x0A;
	reg_value[num_regs] = 0x10;
	num_regs++;
	sii9022a_obj->is_rgb_output = 1;

	ret_val = sii9022a_device_write(reg_addr, reg_value, num_regs);

	return ret_val;
}

static int sii9022a_calc_crc(struct hdmi_sii9022a_obj *sii9022a_obj, u32 *crc)
{
	int ret_val = 0;
	u32 cnt, sum = 0;
	u8 reg_addr, reg_value = 0u;

	for (cnt = 0u; cnt < SII9022A_CRC0; cnt++) {
		reg_addr = 0x0D + cnt;
		ret_val |= sii9022a_device_read(&reg_addr, &reg_value, 1u);

		if (0 == ret_val)
			sum += reg_value;
		else
			break;
	}

	if (0 == ret_val) {
		sum += SII9022A_CRC1 + SII9022A_CRC2 + SII9022A_CRC3;
		sum &= 0xFF;
		*crc = 0x100 - sum;
	}

	return ret_val;
}

/* Program AVInfo Frame */
static int sii9022a_prgm_av_info_frame(struct hdmi_sii9022a_obj *sii9022a_obj,
	struct hdmi_sii9022a_mode_info *mode_info)
{
	int ret_val;
	u8 reg_value, reg_addr;
	u32 crc;

	reg_addr = 0x0D;
	if (sii9022a_obj->is_rgb_output)
		reg_value = 0x01;
	else
		reg_value = 0x21;

	ret_val = sii9022a_device_write(&reg_addr, &reg_value, 1);

	reg_addr = 0x0E;
	reg_value = 0xA0;
	ret_val |= sii9022a_device_write(&reg_addr, &reg_value, 1);

	reg_addr = 0x0F;
	reg_value = 0x00;
	ret_val |= sii9022a_device_write(&reg_addr, &reg_value, 1);

	reg_addr = 0x10;
	reg_value = mode_info->modeCode & 0x7F;
	ret_val |= sii9022a_device_write(&reg_addr, &reg_value, 1);

	reg_addr = 0x11;
	reg_value = 0x00;
	ret_val |= sii9022a_device_write(&reg_addr, &reg_value, 1);

	reg_addr = 0x12;
	reg_value = 0x00;
	ret_val |= sii9022a_device_write(&reg_addr, &reg_value, 1);

	reg_addr = 0x13;
	reg_value = 0x00;
	ret_val |= sii9022a_device_write(&reg_addr, &reg_value, 1);

	reg_addr = 0x14;
	reg_value = 0x00;
	ret_val |= sii9022a_device_write(&reg_addr, &reg_value, 1);

	reg_addr = 0x15;
	reg_value = 0x00;
	ret_val |= sii9022a_device_write(&reg_addr, &reg_value, 1);

	reg_addr = 0x16;
	reg_value = 0x00;
	ret_val |= sii9022a_device_write(&reg_addr, &reg_value, 1);

	reg_addr = 0x17;
	reg_value = 0x00;
	ret_val |= sii9022a_device_write(&reg_addr, &reg_value, 1);

	reg_addr = 0x18;
	reg_value = 0x00;
	ret_val |= sii9022a_device_write(&reg_addr, &reg_value, 1);

	if (0 == ret_val) {
		ret_val = sii9022a_calc_crc(sii9022a_obj, &crc);

		if (0 == ret_val) {
			reg_addr = 0x0C;
			reg_value = crc & 0xFF;
			ret_val = sii9022a_device_write(&reg_addr,
							&reg_value, 1);
		}
	}

	reg_addr = 0x19;
	reg_value = 0x00;
	ret_val |= sii9022a_device_write(&reg_addr, &reg_value, 1);

	return ret_val;
}

static int sii9022a_prgm_md_reset_regs(struct hdmi_sii9022a_obj *sii9022a_obj,
		struct hdmi_sii9022a_mode_info *si_mode_info)
{
	int ret_val = 0;
	u8 reg_addr, reg_value;

	reg_addr = 0x63;
	reg_value = sii9022a_obj->sync_polarity_reg;
	ret_val = sii9022a_device_write(&reg_addr, &reg_value, 1);

	reg_addr = 0x60;
	reg_value = sii9022a_obj->sync_cfg_reg;
	ret_val |= sii9022a_device_write(&reg_addr, &reg_value, 1);

	if (0 == ret_val) {
		mdelay(5);
		reg_addr = 0x61;
		ret_val = sii9022a_device_read(&reg_addr, &reg_value, 1u);

		if (0 == ret_val) {
			/* Set the same sync polarity in 0x63 register */
			sii9022a_obj->sync_polarity_reg &= ~(0x30);
			sii9022a_obj->sync_polarity_reg |=
				((reg_value & 0x30) >> 4);

			reg_addr = 0x63;
			reg_value = sii9022a_obj->sync_polarity_reg;
			ret_val = sii9022a_device_write(&reg_addr,
							&reg_value, 1);
		}
	}

	return ret_val;
}

/*
  Device Init
*/
static int sii9022a_device_init(struct hdmi_sii9022a_obj *sii9022a_obj)
{
	int status = 0;

	status = sii9022a_reset(sii9022a_obj);

	if (0 == status)
		status = sii9022a_get_chip_id(sii9022a_obj);

	if (0 == status)
		status = sii9022a_power_up_txm(sii9022a_obj);

	if (0 == status)
		status = sii9022a_enable(sii9022a_obj);

	if (0 == status)
		status = sii9022a_cfg_in_bus(sii9022a_obj);

	if (0 == status)
		status = sii9022a_cfg_yc_mode(sii9022a_obj);

	if (0 == status)
		status = sii9022a_cfg_sync_mode(sii9022a_obj);

	return status;
}

void sii9022a_config(struct hdmi_video_encoder_create_params
			*encoder_create_params)
{
	struct hdmi_sii9022a_obj *sii9022a_obj;
	sii9022a_obj = &gsii9022a_obj;
	sii9022a_obj->create_args = encoder_create_params;

	if (ENCODER_EMBEDDED_SYNC == sii9022a_obj->create_args->sync_mode) {
		sii9022a_obj->sync_cfg_reg = 0x84;
		sii9022a_obj->in_bus_cfg = 0x60;
	} else /* Separate Sync Mode */ {
		sii9022a_obj->sync_cfg_reg = 0x04;
		sii9022a_obj->in_bus_cfg = 0x70;
	}

	/* 0 to latch data on falling clock edge. Rising edge otherwise */
	/* Bit 4 of of TPI Input bus and pixel repetation */
	if (sii9022a_obj->create_args->clk_edge == 0)
		sii9022a_obj->in_bus_cfg &=
			(~(SI9022A_TPI_INPUTBUS_PIXEL_REP_BIT4_MASK));
	else
		sii9022a_obj->in_bus_cfg |=
			SI9022A_TPI_INPUTBUS_PIXEL_REP_BIT4_MASK;

	/* Enable DE in syncpolarity register at 0x63 */
	sii9022a_obj->sync_polarity_reg = 0x40;

	sii9022a_device_init(sii9022a_obj);

}

/*
  Set Mode
*/
static int sii9022a_set_mode(u32 standard)
{
	int ret_val = 0;
	struct hdmi_sii9022a_mode_info *si_mode_info = NULL;
	struct hdmi_sii9022a_obj *sii9022a_obj;
	int resolution = 0;

	sii9022a_obj = &gsii9022a_obj;
	gsii9022a_obj.standard = standard;

	if (standard == FVID2_STD_720P_60)
		resolution = 0;
	else if (standard == FVID2_STD_720P_50)
		resolution = 1;
	else if (standard == FVID2_STD_1080P_30)
		resolution = 2;
	else if (standard == FVID2_STD_1080P_50)
		resolution = 3;
	else if (standard == FVID2_STD_1080P_60)
		resolution = 4;
	else if (standard == FVID2_STD_1080I_60)
		resolution = 5;
	else if (standard == FVID2_STD_1080I_50)
		resolution = 6;

	si_mode_info = &sii9022_mode_info[resolution];

	if (ENCODER_EMBEDDED_SYNC == sii9022a_obj->create_args->sync_mode)
		ret_val |= sii9022a_prgm_emb_sync_timing(sii9022a_obj,
							 si_mode_info);
	else if (ENCODER_EXTERNAL_SYNC == sii9022a_obj->create_args->sync_mode)
		ret_val |= sii9022a_prgm_ext_sync_timing
				(sii9022a_obj, si_mode_info);
	else
		ret_val = -1;

	ret_val |= sii9022a_prgm_av_info_frame(sii9022a_obj, si_mode_info);
	ret_val |= sii9022a_prgm_md_reset_regs(sii9022a_obj, si_mode_info);

	return ret_val;
}

static int sii9022a_start(void)
{
	int ret_val = 0;
	u8 reg_value = 0u, reg_addr;

	reg_addr = 0x1A;
	ret_val = sii9022a_device_read(&reg_addr, &reg_value, 1u);
	if (0 == ret_val) {
		/* Enable HDMI output */
		reg_value |= 0x1;
		/* Enable Output TMDS */
		reg_value &= 0xEF;

		ret_val = sii9022a_device_write(&reg_addr, &reg_value, 1u);

		/* Configure Input Bus after starting */
		ret_val |= sii9022a_cfg_in_bus(&gsii9022a_obj);
	}

	return ret_val;
}

static int sii9022a_stop(void)
{
	int ret_val = 0;
	u8 reg_value = 0u, reg_addr;

	reg_addr = 0x1A;
	ret_val = sii9022a_device_read(&reg_addr, &reg_value, 1u);
	if (0 == ret_val) {
		/* Enable HDMI output */
		reg_value |= 0x1;
		/* Power Down Output TMDS Clock */
		reg_value |= 0x10;

		ret_val = sii9022a_device_write(&reg_addr, &reg_value, 1u);
	}

	return ret_val;
}

/* set power state depending on device state and HDMI state */
static int sii9022a_set_power(void)
{
	int r = 0;

	struct hdmi_video_encoder_create_params  input_encoder_create_params;
	struct sii9022a_platform_data *pdata;
	u32 input_standard;

	pdata = dev_get_platdata(&client->dev);
	if (!pdata)
		printk(KERN_ERR "Platform data not found\n");

	input_encoder_create_params.hdmi_hot_plug_gpio_intr_line =
				pdata->hdmi_hot_plug_gpio_intr_line;
	input_encoder_create_params.sync_mode = pdata->sync_mode;
	input_encoder_create_params.clk_edge = pdata->clk_edge;

	input_standard = gsii9022a_obj.standard;

	sii9022a_config(&input_encoder_create_params);
	sii9022a_set_mode(input_standard);
	sii9022a_start();

	return r;
}

static int sii9022a_panel_enable(void *data)
{
	if (gsii9022a_obj.venc_enabled == 0) {
		sii9022a_set_power();
		gsii9022a_obj.venc_enabled = 1;
	}
	return 0;
}

static int sii9022a_panel_resume(void *data)
{
	if (gsii9022a_obj.venc_enabled == 0) {
		sii9022a_set_power();
		gsii9022a_obj.venc_enabled = 1;
	}
	return 0;
}

static int sii9022a_panel_disable(void *data)
{
	if (gsii9022a_obj.venc_enabled == 1) {
		sii9022a_stop();
		gsii9022a_obj.venc_enabled = 0;
	}
	return 0;
}

static int sii9022a_panel_suspend(void *data)
{
	if (gsii9022a_obj.venc_enabled == 1) {
		sii9022a_stop();
		gsii9022a_obj.venc_enabled = 0;
	}
	return 0;
}

static int sii9022a_probe(struct i2c_client *c, const struct i2c_device_id *id)
{
	client = c;

	return 0;
}

static int sii9022a_remove(struct i2c_client *c)
{
	client = NULL;
	return 0;
}

/* I2C Device ID table */
static const struct i2c_device_id sii9022a_id[] = {
	{ "sii9022a", 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, sii9022a_id);

/* I2C driver data */
static struct i2c_driver sii9022a_driver = {
	.driver = {
		.owner = THIS_MODULE,
		.name = SII9022A_MODULE_NAME,
	},
	.probe = sii9022a_probe,
	.remove = sii9022a_remove,
	.id_table = sii9022a_id,
};

/*
 * sii9022a_init - Initialize driver via I2C interface
 *
 * Register the SIi9022A driver.
 * Return 0 on success or error code on failure.
 */
static int __init sii9022a_init(void)
{
	int ret_val;

	gsii9022a_obj.venc_enabled = 0;

	ret_val = i2c_add_driver(&sii9022a_driver);

	TI81xx_register_display_panel(&hdmi_driver, &gsii9022a_obj.vencinfo);

	gsii9022a_obj.standard = gsii9022a_obj.vencinfo.vtimings.standard;
	if (gsii9022a_obj.vencinfo.enabled == 1) {
		sii9022a_set_power();
		gsii9022a_obj.venc_enabled = 1;
	}

	return ret_val;
}

/*
 * sii9022a_exit - Remove driver via I2C interface
 *
 * Unregister the sii9022a driver.
 * Returns nothing.
 */
static void __exit sii9022a_exit(void)
{
	if (gsii9022a_obj.venc_enabled == 1)
		sii9022a_stop();

	TI81xx_un_register_display_panel(&hdmi_driver);

	i2c_del_driver(&sii9022a_driver);
}

module_init(sii9022a_init);
module_exit(sii9022a_exit);

MODULE_DESCRIPTION("sii9022a HDMI driver");
MODULE_AUTHOR("Senthil Natarajan <senthil.n@ti.com>");
MODULE_LICENSE("GPL v2");
