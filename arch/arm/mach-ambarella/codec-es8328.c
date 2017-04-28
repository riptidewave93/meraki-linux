/*
 * arch/arm/mach-ambarella/codec-es8328.c
 *
 * Author: Cao Rongrong <rrcao@ambarella.com>
 *
 * History:
 *	2011/10/24 - [Cao Rongrong] Created file
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
#include <sound/es8328.h>

#define ES8328_POWER_PIN	GPIO(155)
#define ES8328_POWER_DELAY	1

static struct es8328_platform_data vendor61_es8328_pdata = {
	.power_pin	= ES8328_POWER_PIN,
	.power_delay	= ES8328_POWER_DELAY,
};

static struct i2c_board_info ambarella_es8328_board_info = {
	I2C_BOARD_INFO("es8328", 0x10),
	.platform_data	= &vendor61_es8328_pdata,
};

int __init ambarella_init_es8328(u8 i2c_bus_num, u8 i2c_addr)
{
	if (i2c_addr)
		ambarella_es8328_board_info.addr = i2c_addr;

	i2c_register_board_info(i2c_bus_num, &ambarella_es8328_board_info, 1);
	return 0;
}

