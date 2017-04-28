/*
 * arch/arm/mach-ixp4xx/gpio.h
 *
 * GPIOLIB GPIO interface
 *
 * Written by Kevin Paul Herbert <kph@meraki.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 */

#include <linux/device.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/gpio.h>

static int ixp4xx_gpio_direction_input(struct gpio_chip *chip, unsigned gpio)
{
	gpio_line_config(gpio, IXP4XX_GPIO_IN);
	return 0;
}

static int ixp4xx_gpio_get_value(struct gpio_chip *chip, unsigned gpio)
{
	int value;

	gpio_line_get(gpio, &value);
	return value;
}

static int ixp4xx_gpio_direction_output(struct gpio_chip *chip, unsigned gpio,
					int level)
{
	gpio_line_set(gpio, level);
	gpio_line_config(gpio, IXP4XX_GPIO_OUT);
	return 0;
}

static void ixp4xx_gpio_set_value(struct gpio_chip *chip, unsigned gpio,
				  int value)
{
	gpio_line_set(gpio, value);
}

static struct gpio_chip ixp4xx_chip = {
	.label			= "ixp4xx",
	.direction_input	= ixp4xx_gpio_direction_input,
	.get			= ixp4xx_gpio_get_value,
	.direction_output	= ixp4xx_gpio_direction_output,
	.set			= ixp4xx_gpio_set_value,
	.base			= 0,
	.ngpio			= IXP4XX_N_GPIOS,
};

static int __init ixp4xx_gpio_setup(void)
{
	return gpiochip_add(&ixp4xx_chip);
}

arch_initcall(ixp4xx_gpio_setup);
