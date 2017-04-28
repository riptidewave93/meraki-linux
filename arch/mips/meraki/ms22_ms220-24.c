/*
 *
 * Meraki MS22/MS220-24 board definitions
 *
 * Copyright (c) 2011, 2013 Meraki, Inc.
 * Author: Kevin Paul Herbert (kph@meraki.com)
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
 * This is the board-specific code for the Meraki MS22(P)/MS220-24(P) switches.
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
#include <linux/meraki/cisco-rps.h>
#include <linux/vcoreiii-pwm.h>

#define SUPPORT_VTSS_VCOREIII_LUTON26

#include <asm/mach-vcoreiii/hardware.h>
#include <asm/mach-vcoreiii/vcoreiii_gpio.h>
#include <asm/mach-vcoreiii/phy8512_gpio.h>
#include <asm/mach-vcoreiii/sgpio.h>

static struct i2c_gpio_platform_data ms22_i2c_gpio_pins = {
	.sda_pin	= VCOREIII_GPIO_PHY_BASE(0) + 15,
	.scl_pin	= VCOREIII_GPIO_PHY_BASE(0) + 16,
	.udelay		= 10,
};

static struct platform_device ms22_gpio_i2c_controller = {
	.name	= "i2c-gpio",
	.id	= 2,
	.dev = {
		.platform_data = &ms22_i2c_gpio_pins,
	},
	.num_resources = 0,
};

static struct gpio_led ms22_ms220_24_led_pins[] = {
	{
		.name = "power:green",
		.gpio = VCOREIII_GPIO_PHY_BASE(0) + 13,
	},
	{
		.name = "power:orange",
		.gpio = VCOREIII_GPIO_PHY_BASE(0) + 14,
		.default_trigger = "default-on",
	},
};

static struct gpio_led_platform_data ms22_ms220_24_led_data = {
	.leds = ms22_ms220_24_led_pins,
	.num_leds = ARRAY_SIZE(ms22_ms220_24_led_pins),
};

static struct platform_device ms22_ms220_24_leds = {
	.name = "leds-gpio",
	.id = -1,
	.dev = {
		.platform_data = &ms22_ms220_24_led_data,
	}
};

static const u8 ms22_ms220_24_pd690xx_slave_addr[] = { 0, 1 };

static struct pd690xx_platform_data ms22_ms220_24_pd690xx_data = {
	.reset			=	VCOREIII_GPIO_PHY_BASE(0) + 17,
	.disable_ports		=	VCOREIII_GPIO_PHY_BASE(0) + 18,
	.ports			=	24,
	.slave_addr		=	ms22_ms220_24_pd690xx_slave_addr,
	.port_base		=	1, /* Match numbers on faceplate */
	.therm_domain		=	"system",
	.temp_warm		=	80,
	.power_budget		=	0, /* This is filled in by the init() functions */
	.controllers		=	2,
	.fast_state_detect	=	false,
};

static struct i2c_board_info ms22_ms220_24_i2c_board_info[] = {
	{
		I2C_BOARD_INFO("pd690xx", 0x30),
		.platform_data = &ms22_ms220_24_pd690xx_data,
	},
};

#ifdef CONFIG_VTSS_VCOREIII_PHY8512_GPIO

static struct phy8512_gpio_chip ms22_ms220_24_phy8512_gpio = {
	.gpio_chip = {
		.label = "phy8512-gpio0",
		.get = luton26_phy8512_gpio_get_value,
		.set = luton26_phy8512_gpio_set_value,
		.direction_input = luton26_phy8512_gpio_direction_input,
		.direction_output = luton26_phy8512_gpio_direction_output,
		.to_irq = luton26_phy8512_gpio_to_irq,
		.base = VCOREIII_GPIO_PHY_BASE(0),
		.ngpio = 32,
		.can_sleep = 1,
	},
	.inst = 1,
	.port_base = 12,
};

#endif /* CONFIG_VTSS_VCOREIII_PHY8512_GPIO */


static struct thermal_gpio_platform_data ms22_ms220_24_fan_data = {
	.gpio = VCOREIII_GPIO_PHY_BASE(0) + 19,
	.fan_active = 1,
	.type = "system",
};

static struct platform_device ms22_ms220_24_fan = {
	.name = "thermal-gpio",
	.dev = {
		.platform_data = &ms22_ms220_24_fan_data,
	}
};

static struct pwm_device pwm_info = {
	.label = "vcoreiii_pwm",
	.freq = PWM_FREQUENCY_10Hz,
	.duty_cycle = 0xff,
	.polarity = PWM_POLARITY_NORMAL,
	.open_collector = PWM_COLLECTOR_NORMAL,
	.gating = false,
};

static struct pwm_fan_ctx ms22_ms220_24_pwm_data = {
	.pwm = &pwm_info,
};

static struct platform_device ms22_ms220_24_pwm = {
	.name = "vcoreiii-pwm-fan",
	.dev = {
		.platform_data = &ms22_ms220_24_pwm_data,
	}
};

static struct sgpio_platform_data ms22_ms220_24_sgpio_data = {
	.label = "serial-led",
	.gpio_di = -1,
	.gpio_do = VCOREIII_GPIO_BASE + 2,
	.gpio_clk = VCOREIII_GPIO_BASE + 0,
	.base = VCOREIII_GPIO_SGPIO_BASE(0),
	.num_ports = 30,
	.gpios_per_port = 2,
	.init_value = 1,
};

struct platform_device ms22_ms220_24_sgpio = {
	.name = "vcoreiii-sgpio",
	.id = 0,
	.dev = {
		.platform_data = &ms22_ms220_24_sgpio_data,
	},
};

static struct cisco_rps_platform_data ms220_24_rps_data = {
	.i2c_adapter = 1,
	.status_0 = VCOREIII_GPIO_PHY_BASE(0) + 15,
	.status_1 = VCOREIII_GPIO_PHY_BASE(0) + 16,
	.max_system_power = 250 * 1000,
	.max_poe_power = 370 * 1000,
};

static struct platform_device ms220_24_rps = {
	.name = "cisco-rps",
	.id = -1,
	.dev = {
		.platform_data = &ms220_24_rps_data,
	},
};

static struct platform_device *ms22_ms220_24_devices[] __initdata = {
	&ms22_ms220_24_sgpio,
	&ms22_ms220_24_leds,
	&ms22_ms220_24_fan,
	&ms22_ms220_24_pwm,
};

static struct platform_device *ms22_devices[] __initdata = {
    &ms22_gpio_i2c_controller,
};

static struct platform_device *ms220_24_devices[] __initdata = {
    &ms220_24_rps,
};

static void ms22_ms220_24_machine_restart(char *command)
{
	int i;

	for (i = 0; i < 100; i++) {
		gpio_direction_output(VCOREIII_GPIO_BASE + 12, 0);
		udelay(100);
	}
        writel(VTSS_F_DEVCPU_GCB_DEVCPU_RST_REGS_SOFT_CHIP_RST_SOFT_CHIP_RST,
               VTSS_DEVCPU_GCB_DEVCPU_RST_REGS_SOFT_CHIP_RST);

	while (true)
		cpu_wait();
}

static void __init allocate_common_devices(void)
{
	int ret;

#ifdef CONFIG_VTSS_VCOREIII_PHY8512_GPIO
	gpiochip_add(&ms22_ms220_24_phy8512_gpio.gpio_chip);
#endif /* CONFIG_VTSS_VCOREIII_PHY8512_GPIO */
	ms22_ms220_24_sgpio_data.ctrl =
		VTSS_DEVCPU_GCB_SIO_CTRL_SIO_INPUT_DATA(0);

	platform_add_devices(ms22_ms220_24_devices, ARRAY_SIZE(ms22_ms220_24_devices));
	ret = gpio_request(VCOREIII_GPIO_BASE + 12, "reset");
	if (ret < 0) {
		printk(KERN_ERR "Unable to request reset pin, err=%d\n",
		       ret);
	} else {
		gpio_direction_output(VCOREIII_GPIO_BASE + 12, 1);
		_machine_restart = ms22_ms220_24_machine_restart;
	}
}

int __init meraki_ms22_late_init(void)
{
	struct i2c_adapter *adap;
	int i;

	allocate_common_devices();
	platform_add_devices(ms22_devices, ARRAY_SIZE(ms22_devices));

	adap = i2c_get_adapter(2);
	if (!adap)
		return -ENODEV;

	ms22_ms220_24_pd690xx_data.power_budget = 380 * 1000;
	for (i = 0; i < ARRAY_SIZE(ms22_ms220_24_i2c_board_info); ++i)
		i2c_new_device(adap, &ms22_ms220_24_i2c_board_info[i]);

	i2c_put_adapter(adap);

	return 0;
}

int __init meraki_ms220_24_late_init(void)
{
	struct i2c_adapter *adap;
	int i;

	allocate_common_devices();
	platform_add_devices(ms220_24_devices, ARRAY_SIZE(ms220_24_devices));

	adap = i2c_get_adapter(1);
	if (!adap)
		return -ENODEV;

	ms22_ms220_24_pd690xx_data.power_budget = 370 * 1000;
	for (i = 0; i < ARRAY_SIZE(ms22_ms220_24_i2c_board_info); ++i)
		i2c_new_device(adap, &ms22_ms220_24_i2c_board_info[i]);

	i2c_put_adapter(adap);

	return 0;
}
