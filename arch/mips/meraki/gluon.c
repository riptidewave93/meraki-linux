/*
 *
 * Meraki Gluon (MX-40) board definitions
 *
 * Copyright (c) 2011 Meraki, Inc.
 * Author: Justin Delegard (jdizzle@meraki.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
 * This is the board-specific code for the Meraki MX40 security appliance.
 */

#include <linux/kernel.h>

#include <linux/platform_device.h>
#include <linux/leds.h>
#include <linux/gpio.h>
#include <linux/gpio_keys.h>
#include <linux/input.h>
#include <linux/leds-nu801.h>

#include "xlna.h"

static struct gpio_led gluon_led_pins[] = {
    {
        .name = "power:orange",
        .gpio = ATHEROS_GPIO_BASE + 17,
        .default_state = LEDS_GPIO_DEFSTATE_OFF,
        .active_low = 1,
    },
};

static struct gpio_led_platform_data gluon_led_data = {
    .leds = gluon_led_pins,
    .num_leds = ARRAY_SIZE(gluon_led_pins),
};

static struct platform_device gluon_leds = {
    .name = "leds-gpio",
    .id = -1,
    .dev = {
        .platform_data = &gluon_led_data,
    }
};

static struct led_nu801_template tricolor_led_template = {
    .name = "tricolor",
    .num_leds = 1,
    .cki = ATHEROS_GPIO_BASE + 14,
    .sdi = ATHEROS_GPIO_BASE + 15,
    .lei = -1,
    .ndelay = 50,
    .init_brightness = {
        LED_OFF,
        LED_OFF,
        LED_OFF,
    },
#ifdef CONFIG_LEDS_TRIGGERS
    .default_trigger = "fade",
#endif
};

static struct led_nu801_platform_data tricolor_led_data = {
    .num_controllers = 1,
    .template = &tricolor_led_template,
};

static struct platform_device tricolor_leds = {
    .name = "leds-nu801",
    .id = -1,
    .dev.platform_data = &tricolor_led_data,
};

static struct gpio_keys_button sw_reset_button = {
    .code = KEY_RESTART,
    .gpio = ATHEROS_GPIO_BASE + 12,
    .active_low = 1,
    .desc = "Software Reset Switch",
    .type = EV_KEY,
    .wakeup = 0,
    .debounce_interval = 50,
};

static struct gpio_keys_platform_data sw_reset_data= {
    .buttons = &sw_reset_button,
    .nbuttons = 1,
    .rep = 0,
};

static struct platform_device sw_reset = {
    .name = "gpio-keys",
    .id = -1,
    .dev.platform_data = &sw_reset_data,
};

static struct xlna_platform_data xlna_data = {
    .gpio0 = ATHEROS_GPIO_BASE + 18,
    .gpio1 = ATHEROS_GPIO_BASE + 19,
};

static struct platform_device xlna = {
    .name = "ath-xlna",
    .dev.platform_data = &xlna_data,
};

static struct platform_device *gluon_platform_devices[] __initdata = {
    &tricolor_leds,
    &gluon_leds,
    &sw_reset,
    &xlna,
};

int __init gluon_init(void)
{
    printk("MERAKI GLUON INIT\n");
    return platform_add_devices(gluon_platform_devices, ARRAY_SIZE(gluon_platform_devices));
}

device_initcall(gluon_init);
