/*
 *
 * Meraki MS220 board definitions
 *
 * Copyright (c) 2011-13 Meraki, Inc.
 * Author: Stephen Segal (ssegal@meraki.com)
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
 * This is the board-specific code for the Meraki MS220/MS220FP switches.
 */

#include <linux/kernel.h>

#include <linux/i2c.h>
#include <linux/i2c-gpio.h>
#include <linux/platform_device.h>
#include <linux/leds.h>
#include <linux/pd690xx.h>
#include <linux/thermal_gpio.h>
#include <linux/delay.h>
#include <asm/reboot.h>

#define SUPPORT_VTSS_VCOREIII_LUTON26

#include <asm/mach-vcoreiii/hardware.h>
#include <asm/mach-vcoreiii/vcoreiii_gpio.h>
#include <asm/mach-vcoreiii/phy8512_gpio.h>
#include <asm/mach-vcoreiii/sgpio.h>

static struct i2c_gpio_platform_data ms220_8_i2c_gpio_pins = {
    .sda_pin    = VCOREIII_GPIO_BASE + 6,
    .scl_pin    = VCOREIII_GPIO_BASE + 5,
    .udelay     = 10,
};

static struct platform_device ms220_8_gpio_i2c_controller = {
    .name       = "i2c-gpio",
    .id = 1,
    .dev = {
        .platform_data = &ms220_8_i2c_gpio_pins,
    },
    .num_resources = 0,
};

static struct gpio_led ms220_8_led_pins[] = {
    {
        .name = "power:green",
        .gpio = VCOREIII_GPIO_SGPIO_BASE(0) + 21,
        .active_low = true,
    },
    {
        .name = "power:orange",
        .gpio = VCOREIII_GPIO_SGPIO_BASE(0) + 20,
        .active_low = true,
        .default_trigger = "default-on",
    },
    {
        .name = "port1:green",
        .gpio = VCOREIII_GPIO_SGPIO_BASE(0) + 1,
        .active_low = true,
    },
    {
        .name = "port1:orange",
        .gpio = VCOREIII_GPIO_SGPIO_BASE(0) + 0,
        .active_low = true,
    },
    {
        .name = "port2:green",
        .gpio = VCOREIII_GPIO_SGPIO_BASE(0) + 3,
        .active_low = true,
    },
    {
        .name = "port2:orange",
        .gpio = VCOREIII_GPIO_SGPIO_BASE(0) + 2,
        .active_low = true,
    },
    {
        .name = "port3:green",
        .gpio = VCOREIII_GPIO_SGPIO_BASE(0) + 5,
        .active_low = true,
    },
    {
        .name = "port3:orange",
        .gpio = VCOREIII_GPIO_SGPIO_BASE(0) + 4,
        .active_low = true,
    },
    {
        .name = "port4:green",
        .gpio = VCOREIII_GPIO_SGPIO_BASE(0) + 7,
        .active_low = true,
    },
    {
        .name = "port4:orange",
        .gpio = VCOREIII_GPIO_SGPIO_BASE(0) + 6,
        .active_low = true,
    },
    {
        .name = "port5:green",
        .gpio = VCOREIII_GPIO_SGPIO_BASE(0) + 9,
        .active_low = true,
    },
    {
        .name = "port5:orange",
        .gpio = VCOREIII_GPIO_SGPIO_BASE(0) + 8,
        .active_low = true,
    },
    {
        .name = "port6:green",
        .gpio = VCOREIII_GPIO_SGPIO_BASE(0) + 11,
        .active_low = true,
    },
    {
        .name = "port6:orange",
        .gpio = VCOREIII_GPIO_SGPIO_BASE(0) + 10,
        .active_low = true,
    },
    {
        .name = "port7:green",
        .gpio = VCOREIII_GPIO_SGPIO_BASE(0) + 13,
        .active_low = true,
    },
    {
        .name = "port7:orange",
        .gpio = VCOREIII_GPIO_SGPIO_BASE(0) + 12,
        .active_low = true,
    },
    {
        .name = "port8:green",
        .gpio = VCOREIII_GPIO_SGPIO_BASE(0) + 15,
        .active_low = true,
    },
    {
        .name = "port8:orange",
        .gpio = VCOREIII_GPIO_SGPIO_BASE(0) + 14,
        .active_low = true,
    },
    {
        .name = "port9:green",
        .gpio = VCOREIII_GPIO_SGPIO_BASE(0) + 17,
        .active_low = true,
    },
    {
        .name = "port9:orange",
        .gpio = VCOREIII_GPIO_SGPIO_BASE(0) + 16,
        .active_low = true,
    },
    {
        .name = "port10:green",
        .gpio = VCOREIII_GPIO_SGPIO_BASE(0) + 19,
        .active_low = true,
    },
    {
        .name = "port10:orange",
        .gpio = VCOREIII_GPIO_SGPIO_BASE(0) + 18,
        .active_low = true,
    },
};

static struct gpio_led_platform_data ms220_8_led_data = {
    .leds = ms220_8_led_pins,
    .num_leds = ARRAY_SIZE(ms220_8_led_pins),
};

static struct platform_device ms220_8_leds = {
    .name = "leds-gpio",
    .id = -1,
    .dev = {
        .platform_data = &ms220_8_led_data,
    }
};

static const u8 ms220_8_pd690xx_slave_addr[] = { 0 };

static struct pd690xx_platform_data ms220_8_pd690xx_data = {
    .reset              =       VCOREIII_GPIO_BASE + 12,
    .disable_ports      =       VCOREIII_GPIO_BASE + 7,
    .ports              =       8,
    .slave_addr         =       ms220_8_pd690xx_slave_addr,
    .port_base          =       1, /* Match numbers on faceplate */
    .therm_domain       =       "system",
    .temp_warm          =       80,
    .power_budget       =       124 * 1000,
    .controllers        =       1,
    .fast_state_detect  =       true,
};

static struct i2c_board_info phy_i2c_board_info[] = {
    {
        I2C_BOARD_INFO("pd690xx", 0x30),
        .platform_data = &ms220_8_pd690xx_data,
    },
};

static struct sgpio_platform_data ms220_8_sgpio_data = {
    .label = "serial-led",
    .gpio_di = -1,
    .gpio_do = VCOREIII_GPIO_BASE + 2,
    .gpio_clk = VCOREIII_GPIO_BASE + 0,
    .base = VCOREIII_GPIO_SGPIO_BASE(0),
    .num_ports = 30,
    .gpios_per_port = 2,
    .init_value = 1,
};

struct platform_device ms220_8_sgpio = {
    .name = "vcoreiii-sgpio",
    .id = 0,
    .dev = {
        .platform_data = &ms220_8_sgpio_data,
    },
};

static struct platform_device *ms220_8_devices[] __initdata = {
    &ms220_8_gpio_i2c_controller,
    &ms220_8_sgpio,
    &ms220_8_leds,
};

static void ms220_8_machine_restart(char *command)
{
    writel(VTSS_F_DEVCPU_GCB_DEVCPU_RST_REGS_SOFT_CHIP_RST_SOFT_CHIP_RST,
           VTSS_DEVCPU_GCB_DEVCPU_RST_REGS_SOFT_CHIP_RST);

    while (true)
        cpu_wait();
}

int __init meraki_ms220_8_late_init(void)
{
    struct i2c_adapter *adap;

    ms220_8_sgpio_data.ctrl =
        VTSS_DEVCPU_GCB_SIO_CTRL_SIO_INPUT_DATA(0);
    platform_add_devices(ms220_8_devices, ARRAY_SIZE(ms220_8_devices));

    adap = i2c_get_adapter(1);
    if (!adap)
        return -ENODEV;
    i2c_new_device(adap, &phy_i2c_board_info[0]);
    i2c_put_adapter(adap);

    _machine_restart = ms220_8_machine_restart;

    return 0;
}
