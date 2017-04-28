/*
 * arch/arm/mach-ambarella/touch-ak4183.c
 *
 * Author: Anthony Ginger <hfjiang@ambarella.com>
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

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>

#include <asm/mach-types.h>
#include <asm/mach/arch.h>

#include <mach/hardware.h>
#include <mach/init.h>
#include <mach/board.h>

#include <linux/i2c.h>
#include <linux/i2c/ak4183.h>

#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/delay.h>

#include "touch.h"

/* ==========================================================================*/
int ambarella_ak4183_get_pendown_state(void)
{
	if (ambarella_gpio_get(
		ambarella_board_generic.touch_panel_irq.irq_gpio))
		return 0;
	else
		return 1;
}

void ambarella_ak4183_clear_penirq(void)
{
	ambarella_touch_common_ack_irq();
}

int ambarella_ak4183_init_platform_hw(void)
{
	return ambarella_touch_common_init_hw();
}

void ambarella_ak4183_exit_platform_hw(void)
{
}

struct ak4183_platform_data ambarella_ak4183_pdata = {
	.model = 4183,
	.x_plate_ohms = 310,
	.fix = {
		.x_invert = 1,
		.y_invert = 0,
		.x_rescale = 1,
		.y_rescale = 1,
		.x_min = 374,
		.x_max = 3716,
		.y_min = 185,
		.y_max = 3748,
	},
	.get_pendown_state = ambarella_ak4183_get_pendown_state,
	.clear_penirq = ambarella_ak4183_clear_penirq,
	.init_platform_hw = ambarella_ak4183_init_platform_hw,
	.exit_platform_hw = ambarella_ak4183_exit_platform_hw
};

struct i2c_board_info ambarella_ak4183_board_info = {
	.type		= "ak4183",
	.addr		= 0x90 >> 1,
	.platform_data	= &ambarella_ak4183_pdata,
};

