/*
 * arch/arm/mach-ambarella/codec-wm8940.c
 *
 * Author: Wu Dongge <dgwu@ambarella.com>
 *
 * History:
 *	2012/10/16 - [Wu Dongge] Created file
 *
 * Copyright (C) 2004-2011, Ambarella, Inc.
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

static struct i2c_board_info ambarella_wm8940_board_info = {
	I2C_BOARD_INFO("wm8940", 0x0D),   /* 0D is 7-bit I2C addr */
	.platform_data	= NULL,
};

int __init ambarella_init_wm8940(u8 i2c_bus_num, u8 i2c_addr)
{
	if (i2c_addr)
		ambarella_wm8940_board_info.addr = i2c_addr;

	i2c_register_board_info(i2c_bus_num, &ambarella_wm8940_board_info, 1);
	return 0;
}

