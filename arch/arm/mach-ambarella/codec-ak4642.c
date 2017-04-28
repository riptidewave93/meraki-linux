/*
 * arch/arm/mach-ambarella/codec-ak4642.c
 *
 * Author: Cao Rongrong <rrcao@ambarella.com>
 *
 * History:
 *	2012/06/27 - [Cao Rongrong] Created file
 *
 * Copyright (C) 2004-2010, Ambarella, Inc.
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
#include <sound/ak4642_amb.h>

static struct ak4642_platform_data ak4642_pdata;

static struct i2c_board_info ambarella_ak4642_board_info = {
	.type 		= "ak4642",
	.platform_data	= &ak4642_pdata,
};

int __init ambarella_init_ak4642(u8 i2c_bus_num, u8 i2c_addr, u8 rst_pin)
{
	ambarella_ak4642_board_info.addr = i2c_addr;

	ak4642_pdata.rst_pin = rst_pin;
	ak4642_pdata.rst_delay = 1;

	i2c_register_board_info(i2c_bus_num, &ambarella_ak4642_board_info, 1);
	return 0;
}

