/*
 * gpio-regulator.h  --  GPIO Voltage regulation
 *
 * Copyright (C) 2011 Texas Instruments, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */

#ifndef REGULATOR_GPIO
#define REGULATOR_GPIO

#include <linux/gpio.h>
#include <linux/regulator/machine.h>

/**
 * gpio_vr_data - gpio voltage regulator data
 * @gpio_value:	gpio value of the regulator
 * @uV:		Micro voltage corresponds to each gpio value
 */
struct gpio_vr_data {
	u32	gpio_value;
	int	uV;
};

/**
 * gpio_reg_platform_data - platform data for gpio regulator
 * @name:		name of the pmic output
 * @gpios:		array of GPIOs, needed for voltage control
 * @num_gpio_pins:	number of gpio pins for voltage control
 * @num_voltages:	number of voltage steps
 * @pmic_vout:		pmic output voltage in mV
 * @gpio_single_bank:	Flag to notify to voltage regulator that
 *			the GPIO lines for voltage control belonging
 *			to single bank or not
 * @gpio_arr_mask:	Mask for GPIO lines if the voltage control GPIO
 *			lines are belonging to single bank
 * @pmic_init_data:	regulator platform inititialization data
 * @gpio_vsel_table:	gpio voltage regulator, gpio value and its
 *			corresponding voltage
 */
struct gpio_reg_platform_data {
	const	char			*name;
	struct	gpio			*gpios;
	u32				num_gpio_pins;
	u32				num_voltages;
	u32				pmic_vout;
	bool				gpio_single_bank;
	u32				gpio_arr_mask;
	struct	regulator_init_data	*pmic_init_data;
	struct	gpio_vr_data		*gpio_vsel_table;
};

#endif
