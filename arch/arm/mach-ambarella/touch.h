/*
 * arch/arm/mach-ambarella/touch.h
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

#ifndef __MACH_AMBARELLA_TOUCH_H__
#define __MACH_AMBARELLA_TOUCH_H__

/* ==========================================================================*/
static inline int ambarella_touch_common_ack_irq(void)
{
	int					rval = 0;
	struct irq_data				*touch_irq_data;

	touch_irq_data = irq_get_irq_data(
		ambarella_board_generic.touch_panel_irq.irq_line);
	if (touch_irq_data && touch_irq_data->chip &&
		touch_irq_data->chip_data && touch_irq_data->chip->irq_ack) {
		touch_irq_data->chip->irq_ack(touch_irq_data->chip_data);
	} else {
		rval = -ENODEV;
	}

	return rval;
}

static inline int ambarella_touch_common_init_hw(void)
{
	int					rval = 0;
	struct irq_data				*touch_irq_data;

	ambarella_set_gpio_output(
		&ambarella_board_generic.touch_panel_power, 1);
	ambarella_set_gpio_reset(&ambarella_board_generic.touch_panel_reset);
	ambarella_gpio_config(ambarella_board_generic.touch_panel_irq.irq_gpio,
		ambarella_board_generic.touch_panel_irq.irq_gpio_mode);

	touch_irq_data = irq_get_irq_data(
		ambarella_board_generic.touch_panel_irq.irq_line);
	if (touch_irq_data && touch_irq_data->chip &&
		touch_irq_data->chip_data &&
		touch_irq_data->chip->irq_set_type) {
		touch_irq_data->chip->irq_set_type(touch_irq_data->chip_data,
			ambarella_board_generic.touch_panel_irq.irq_type);
	} else {
		rval = -ENODEV;
	}

	if (!rval) {
		rval = ambarella_touch_common_ack_irq();
	}

	return rval;
}

#endif //__MACH_AMBARELLA_TOUCH_H__

