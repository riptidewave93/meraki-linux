/*
 * arch/arm/mach-ambarella/codec-alc5633.c
 *
 * Author: Ken He <jianhe@ambarella.com>
 *
 * History:
 *	2012/11/27 - [Ken He] Created file
 *
 * Copyright (C) 2012-2016, Ambarella, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 */

#include <linux/platform_device.h>
#include <linux/i2c.h>
#include <linux/gpio.h>

static struct i2c_board_info ambarella_alc5633_board_info = {
	I2C_BOARD_INFO("alc5633", (0x38>>1)),   /* 0x38 is 7-bit I2C addr */
	.platform_data	= NULL,
};

int __init ambarella_init_alc5633(u8 i2c_bus_num, u8 i2c_addr)
{
	int ret = 0;

	if (i2c_addr)
		ambarella_alc5633_board_info.addr = i2c_addr;

	ret = i2c_register_board_info(i2c_bus_num, &ambarella_alc5633_board_info, 1);
	return ret;
}

