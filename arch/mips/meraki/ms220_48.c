/*
 *
 * Meraki MS220-48 board definitions
 *
 * Copyright (c) 2013 Meraki, Inc.
 * Author: James Chris Parks (james.parks@meraki.net)
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
 * This is the board-specific code for the Meraki MS220-48/MS220-48P
 * switches.
 */

#include <linux/kernel.h>

#include <linux/i2c.h>
#include <linux/i2c-gpio.h>
#include <linux/i2c-gpio-scl.h>
#include <linux/platform_device.h>
#include <linux/i2c/at24.h>
#include <linux/leds.h>
#include <linux/pd690xx.h>
#include <linux/thermal_gpio.h>
#include <linux/delay.h>
#include <asm/reboot.h>
#include <linux/meraki/cisco-rps.h>
#include <linux/meraki/meraki_config.h>

#define SUPPORT_VTSS_VCOREIII_JAGUAR

#include <asm/mach-vcoreiii/hardware.h>
#include <asm/mach-vcoreiii/vcoreiii_gpio.h>
#include <asm/mach-vcoreiii/phy8512_gpio.h>
#include <asm/mach-vcoreiii/sgpio.h>

static unsigned int ms220_48_i2c_sfp_scl_pins[] = {
        VCOREIII_GPIO_BASE_SLV + 20,
        VCOREIII_GPIO_BASE_SLV + 19,
        VCOREIII_GPIO_BASE_SLV + 18,
        VCOREIII_GPIO_BASE_SLV + 17,
};

static struct i2c_gpio_scl_platform_data ms220_48_i2c_sfp_gpio_pins[] = {
        {
                .sda_pin = VCOREIII_GPIO_BASE_SLV + 21,
                .scl_pins = ms220_48_i2c_sfp_scl_pins,
                .num_scl = 4,
                .parent_id = 2,
                .child_id_base = 3,
                .udelay = 10,
        },
};

struct platform_device ms220_48_sfp_gpio_i2c_controller[] = {
	{
		.name	= "i2c-gpio-scl",
		.id	= 2,
		.dev = {
			.platform_data = &ms220_48_i2c_sfp_gpio_pins[0],
		},
		.num_resources = 0,
	},
};

static struct i2c_gpio_platform_data ms220_48_i2c_power_gpio_pins = {
	.sda_pin = VCOREIII_GPIO_BASE_SLV + 15,
	.scl_pin = VCOREIII_GPIO_BASE_SLV + 14,
	.udelay = 10,
};

static struct platform_device ms220_48_power_i2c_controller = {
	.name = "i2c-gpio",
	.id = 8,
	.dev = {
		.platform_data = &ms220_48_i2c_power_gpio_pins,
	},
	.num_resources = 0,
};

struct memory_accessor *ms220_48_sfp_eeprom_mems[4];
EXPORT_SYMBOL(ms220_48_sfp_eeprom_mems);

static void ms220_48_sfp_eeprom_setup(struct memory_accessor *ma, void *context)
{
	*((struct memory_accessor **)context) = ma;
}

static struct at24_platform_data ms220_48_sfp_eeprom_plat_data[4] = {
	{
		.byte_len = 256,
		.page_size = 1,
		.flags = AT24_FLAG_READONLY,
		.setup = ms220_48_sfp_eeprom_setup,
		.context = &ms220_48_sfp_eeprom_mems[0],
	},
	{
		.byte_len = 256,
		.page_size = 1,
		.flags = AT24_FLAG_READONLY,
		.setup = ms220_48_sfp_eeprom_setup,
		.context = &ms220_48_sfp_eeprom_mems[1],
	},
	{
		.byte_len = 256,
		.page_size = 1,
		.flags = AT24_FLAG_READONLY,
		.setup = ms220_48_sfp_eeprom_setup,
		.context = &ms220_48_sfp_eeprom_mems[2],
	},
	{
		.byte_len = 256,
		.page_size = 1,
		.flags = AT24_FLAG_READONLY,
		.setup = ms220_48_sfp_eeprom_setup,
		.context = &ms220_48_sfp_eeprom_mems[3],
	},
};

static const struct i2c_board_info sfp_i2c_board_info[4] = {
	{
		I2C_BOARD_INFO("24c02", 0x50),
		.platform_data = &ms220_48_sfp_eeprom_plat_data[0],
	},
	{
		I2C_BOARD_INFO("24c02", 0x50),
		.platform_data = &ms220_48_sfp_eeprom_plat_data[1],
	},
	{
		I2C_BOARD_INFO("24c02", 0x50),
		.platform_data = &ms220_48_sfp_eeprom_plat_data[2],
	},
	{
		I2C_BOARD_INFO("24c02", 0x50),
		.platform_data = &ms220_48_sfp_eeprom_plat_data[3],
	},
};

static struct gpio_led ms220_48_led_pins[] = {
	{
		.name = "power:green",
		.gpio = VCOREIII_GPIO_BASE + 22,
	},
	{
		.name = "power:orange",
		.gpio = VCOREIII_GPIO_BASE + 23,
		.default_trigger = "default-on",
	},
	{
		.name = "port1:green",
		.gpio = VCOREIII_GPIO_PHY_BASE(0) + 5,
		.active_low = true,
	},
	{
		.name = "port1:orange",
		.gpio = VCOREIII_GPIO_PHY_BASE(0) + 17,
		.active_low = true,
	},
	{
		.name = "port2:green",
		.gpio = VCOREIII_GPIO_PHY_BASE(0) + 6,
		.active_low = true,
	},
	{
		.name = "port2:orange",
		.gpio = VCOREIII_GPIO_PHY_BASE(0) + 18,
		.active_low = true,
	},
	{
		.name = "port3:green",
		.gpio = VCOREIII_GPIO_PHY_BASE(0) + 7,
		.active_low = true,
	},
	{
		.name = "port3:orange",
		.gpio = VCOREIII_GPIO_PHY_BASE(0) + 19,
		.active_low = true,
	},
	{
		.name = "port4:green",
		.gpio = VCOREIII_GPIO_PHY_BASE(0) + 8,
		.active_low = true,
	},
	{
		.name = "port4:orange",
		.gpio = VCOREIII_GPIO_PHY_BASE(0) + 20,
		.active_low = true,
	},
	{
		.name = "port5:green",
		.gpio = VCOREIII_GPIO_PHY_BASE(0) + 9,
		.active_low = true,
	},
	{
		.name = "port5:orange",
		.gpio = VCOREIII_GPIO_PHY_BASE(0) + 21,
		.active_low = true,
	},
	{
		.name = "port6:green",
		.gpio = VCOREIII_GPIO_PHY_BASE(0) + 10,
		.active_low = true,
	},
	{
		.name = "port6:orange",
		.gpio = VCOREIII_GPIO_PHY_BASE(0) + 22,
		.active_low = true,
	},
	{
		.name = "port7:green",
		.gpio = VCOREIII_GPIO_PHY_BASE(0) + 11,
		.active_low = true,
	},
	{
		.name = "port7:orange",
		.gpio = VCOREIII_GPIO_PHY_BASE(0) + 23,
		.active_low = true,
	},
	{
		.name = "port8:green",
		.gpio = VCOREIII_GPIO_PHY_BASE(0) + 12,
		.active_low = true,
	},
	{
		.name = "port8:orange",
		.gpio = VCOREIII_GPIO_PHY_BASE(0) + 24,
		.active_low = true,
	},
	{
		.name = "port9:green",
		.gpio = VCOREIII_GPIO_PHY_BASE(0) + 13,
		.active_low = true,
	},
	{
		.name = "port9:orange",
		.gpio = VCOREIII_GPIO_PHY_BASE(0) + 25,
		.active_low = true,
	},
	{
		.name = "port10:green",
		.gpio = VCOREIII_GPIO_PHY_BASE(0) + 14,
		.active_low = true,
	},
	{
		.name = "port10:orange",
		.gpio = VCOREIII_GPIO_PHY_BASE(0) + 26,
		.active_low = true,
	},
	{
		.name = "port11:green",
		.gpio = VCOREIII_GPIO_PHY_BASE(0) + 15,
		.active_low = true,
	},
	{
		.name = "port11:orange",
		.gpio = VCOREIII_GPIO_PHY_BASE(0) + 27,
		.active_low = true,
	},
	{
		.name = "port12:green",
		.gpio = VCOREIII_GPIO_PHY_BASE(0) + 16,
		.active_low = true,
	},
	{
		.name = "port12:orange",
		.gpio = VCOREIII_GPIO_PHY_BASE(0) + 28,
		.active_low = true,
	},
	{
		.name = "port13:green",
		.gpio = VCOREIII_GPIO_PHY_BASE(1) + 5,
		.active_low = true,
	},
	{
		.name = "port13:orange",
		.gpio = VCOREIII_GPIO_PHY_BASE(1) + 17,
		.active_low = true,
	},
	{
		.name = "port14:green",
		.gpio = VCOREIII_GPIO_PHY_BASE(1) + 6,
		.active_low = true,
	},
	{
		.name = "port14:orange",
		.gpio = VCOREIII_GPIO_PHY_BASE(1) + 18,
		.active_low = true,
	},
	{
		.name = "port15:green",
		.gpio = VCOREIII_GPIO_PHY_BASE(1) + 7,
		.active_low = true,
	},
	{
		.name = "port15:orange",
		.gpio = VCOREIII_GPIO_PHY_BASE(1) + 19,
		.active_low = true,
	},
	{
		.name = "port16:green",
		.gpio = VCOREIII_GPIO_PHY_BASE(1) + 8,
		.active_low = true,
	},
	{
		.name = "port16:orange",
		.gpio = VCOREIII_GPIO_PHY_BASE(1) + 20,
		.active_low = true,
	},
	{
		.name = "port17:green",
		.gpio = VCOREIII_GPIO_PHY_BASE(1) + 9,
		.active_low = true,
	},
	{
		.name = "port17:orange",
		.gpio = VCOREIII_GPIO_PHY_BASE(1) + 21,
		.active_low = true,
	},
	{
		.name = "port18:green",
		.gpio = VCOREIII_GPIO_PHY_BASE(1) + 10,
		.active_low = true,
	},
	{
		.name = "port18:orange",
		.gpio = VCOREIII_GPIO_PHY_BASE(1) + 22,
		.active_low = true,
	},
	{
		.name = "port19:green",
		.gpio = VCOREIII_GPIO_PHY_BASE(1) + 11,
		.active_low = true,
	},
	{
		.name = "port19:orange",
		.gpio = VCOREIII_GPIO_PHY_BASE(1) + 23,
		.active_low = true,
	},
	{
		.name = "port20:green",
		.gpio = VCOREIII_GPIO_PHY_BASE(1) + 12,
		.active_low = true,
	},
	{
		.name = "port20:orange",
		.gpio = VCOREIII_GPIO_PHY_BASE(1) + 24,
		.active_low = true,
	},
	{
		.name = "port21:green",
		.gpio = VCOREIII_GPIO_PHY_BASE(1) + 13,
		.active_low = true,
	},
	{
		.name = "port21:orange",
		.gpio = VCOREIII_GPIO_PHY_BASE(1) + 25,
		.active_low = true,
	},
	{
		.name = "port22:green",
		.gpio = VCOREIII_GPIO_PHY_BASE(1) + 14,
		.active_low = true,
	},
	{
		.name = "port22:orange",
		.gpio = VCOREIII_GPIO_PHY_BASE(1) + 26,
		.active_low = true,
	},
	{
		.name = "port23:green",
		.gpio = VCOREIII_GPIO_PHY_BASE(1) + 15,
		.active_low = true,
	},
	{
		.name = "port23:orange",
		.gpio = VCOREIII_GPIO_PHY_BASE(1) + 27,
		.active_low = true,
	},
	{
		.name = "port24:green",
		.gpio = VCOREIII_GPIO_PHY_BASE(1) + 16,
		.active_low = true,
	},
	{
		.name = "port24:orange",
		.gpio = VCOREIII_GPIO_PHY_BASE(1) + 28,
		.active_low = true,
	},
	{
		.name = "port25:green",
		.gpio = VCOREIII_GPIO_PHY_BASE(2) + 5,
		.active_low = true,
	},
	{
		.name = "port25:orange",
		.gpio = VCOREIII_GPIO_PHY_BASE(2) + 17,
		.active_low = true,
	},
	{
		.name = "port26:green",
		.gpio = VCOREIII_GPIO_PHY_BASE(2) + 6,
		.active_low = true,
	},
	{
		.name = "port26:orange",
		.gpio = VCOREIII_GPIO_PHY_BASE(2) + 18,
		.active_low = true,
	},
	{
		.name = "port27:green",
		.gpio = VCOREIII_GPIO_PHY_BASE(2) + 7,
		.active_low = true,
	},
	{
		.name = "port27:orange",
		.gpio = VCOREIII_GPIO_PHY_BASE(2) + 19,
		.active_low = true,
	},
	{
		.name = "port28:green",
		.gpio = VCOREIII_GPIO_PHY_BASE(2) + 8,
		.active_low = true,
	},
	{
		.name = "port28:orange",
		.gpio = VCOREIII_GPIO_PHY_BASE(2) + 20,
		.active_low = true,
	},
	{
		.name = "port29:green",
		.gpio = VCOREIII_GPIO_PHY_BASE(2) + 9,
		.active_low = true,
	},
	{
		.name = "port29:orange",
		.gpio = VCOREIII_GPIO_PHY_BASE(2) + 21,
		.active_low = true,
	},
	{
		.name = "port30:green",
		.gpio = VCOREIII_GPIO_PHY_BASE(2) + 10,
		.active_low = true,
	},
	{
		.name = "port30:orange",
		.gpio = VCOREIII_GPIO_PHY_BASE(2) + 22,
		.active_low = true,
	},
	{
		.name = "port31:green",
		.gpio = VCOREIII_GPIO_PHY_BASE(2) + 11,
		.active_low = true,
	},
	{
		.name = "port31:orange",
		.gpio = VCOREIII_GPIO_PHY_BASE(2) + 23,
		.active_low = true,
	},
	{
		.name = "port32:green",
		.gpio = VCOREIII_GPIO_PHY_BASE(2) + 12,
		.active_low = true,
	},
	{
		.name = "port32:orange",
		.gpio = VCOREIII_GPIO_PHY_BASE(2) + 24,
		.active_low = true,
	},
	{
		.name = "port33:green",
		.gpio = VCOREIII_GPIO_PHY_BASE(2) + 13,
		.active_low = true,
	},
	{
		.name = "port33:orange",
		.gpio = VCOREIII_GPIO_PHY_BASE(2) + 25,
		.active_low = true,
	},
	{
		.name = "port34:green",
		.gpio = VCOREIII_GPIO_PHY_BASE(2) + 14,
		.active_low = true,
	},
	{
		.name = "port34:orange",
		.gpio = VCOREIII_GPIO_PHY_BASE(2) + 26,
		.active_low = true,
	},
	{
		.name = "port35:green",
		.gpio = VCOREIII_GPIO_PHY_BASE(2) + 15,
		.active_low = true,
	},
	{
		.name = "port35:orange",
		.gpio = VCOREIII_GPIO_PHY_BASE(2) + 27,
		.active_low = true,
	},
	{
		.name = "port36:green",
		.gpio = VCOREIII_GPIO_PHY_BASE(2) + 16,
		.active_low = true,
	},
	{
		.name = "port36:orange",
		.gpio = VCOREIII_GPIO_PHY_BASE(2) + 28,
		.active_low = true,
	},
	{
		.name = "port37:green",
		.gpio = VCOREIII_GPIO_PHY_BASE(3) + 5,
		.active_low = true,
	},
	{
		.name = "port37:orange",
		.gpio = VCOREIII_GPIO_PHY_BASE(3) + 17,
		.active_low = true,
	},
	{
		.name = "port38:green",
		.gpio = VCOREIII_GPIO_PHY_BASE(3) + 6,
		.active_low = true,
	},
	{
		.name = "port38:orange",
		.gpio = VCOREIII_GPIO_PHY_BASE(3) + 18,
		.active_low = true,
	},
	{
		.name = "port39:green",
		.gpio = VCOREIII_GPIO_PHY_BASE(3) + 7,
		.active_low = true,
	},
	{
		.name = "port39:orange",
		.gpio = VCOREIII_GPIO_PHY_BASE(3) + 19,
		.active_low = true,
	},
	{
		.name = "port40:green",
		.gpio = VCOREIII_GPIO_PHY_BASE(3) + 8,
		.active_low = true,
	},
	{
		.name = "port40:orange",
		.gpio = VCOREIII_GPIO_PHY_BASE(3) + 20,
		.active_low = true,
	},
	{
		.name = "port41:green",
		.gpio = VCOREIII_GPIO_PHY_BASE(3) + 9,
		.active_low = true,
	},
	{
		.name = "port41:orange",
		.gpio = VCOREIII_GPIO_PHY_BASE(3) + 21,
		.active_low = true,
	},
	{
		.name = "port42:green",
		.gpio = VCOREIII_GPIO_PHY_BASE(3) + 10,
		.active_low = true,
	},
	{
		.name = "port42:orange",
		.gpio = VCOREIII_GPIO_PHY_BASE(3) + 22,
		.active_low = true,
	},
	{
		.name = "port43:green",
		.gpio = VCOREIII_GPIO_PHY_BASE(3) + 11,
		.active_low = true,
	},
	{
		.name = "port43:orange",
		.gpio = VCOREIII_GPIO_PHY_BASE(3) + 23,
		.active_low = true,
	},
	{
		.name = "port44:green",
		.gpio = VCOREIII_GPIO_PHY_BASE(3) + 12,
		.active_low = true,
	},
	{
		.name = "port44:orange",
		.gpio = VCOREIII_GPIO_PHY_BASE(3) + 24,
		.active_low = true,
	},
	{
		.name = "port45:green",
		.gpio = VCOREIII_GPIO_PHY_BASE(3) + 13,
		.active_low = true,
	},
	{
		.name = "port45:orange",
		.gpio = VCOREIII_GPIO_PHY_BASE(3) + 25,
		.active_low = true,
	},
	{
		.name = "port46:green",
		.gpio = VCOREIII_GPIO_PHY_BASE(3) + 14,
		.active_low = true,
	},
	{
		.name = "port46:orange",
		.gpio = VCOREIII_GPIO_PHY_BASE(3) + 26,
		.active_low = true,
	},
	{
		.name = "port47:green",
		.gpio = VCOREIII_GPIO_PHY_BASE(3) + 15,
		.active_low = true,
	},
	{
		.name = "port47:orange",
		.gpio = VCOREIII_GPIO_PHY_BASE(3) + 27,
		.active_low = true,
	},
	{
		.name = "port48:green",
		.gpio = VCOREIII_GPIO_PHY_BASE(3) + 16,
		.active_low = true,
	},
	{
		.name = "port48:orange",
		.gpio = VCOREIII_GPIO_PHY_BASE(3) + 28,
		.active_low = true,
	},
	{
		.name = "port49:green",
		.gpio = VCOREIII_GPIO_SGPIO_BASE(1) + 1,
		.active_low = true,
	},
	{
		.name = "port49:orange",
		.gpio = VCOREIII_GPIO_SGPIO_BASE(1) + 0,
		.active_low = true,
	},
	{
		.name = "port50:green",
		.gpio = VCOREIII_GPIO_SGPIO_BASE(1) + 3,
		.active_low = true,
	},
	{
		.name = "port50:orange",
		.gpio = VCOREIII_GPIO_SGPIO_BASE(1) + 2,
		.active_low = true,
	},
	{
		.name = "port51:green",
		.gpio = VCOREIII_GPIO_SGPIO_BASE(3) + 1,
		.active_low = true,
	},
	{
		.name = "port51:orange",
		.gpio = VCOREIII_GPIO_SGPIO_BASE(3) + 0,
		.active_low = true,
	},
	{
		.name = "port52:green",
		.gpio = VCOREIII_GPIO_SGPIO_BASE(3) + 3,
		.active_low = true,
	},
	{
		.name = "port52:orange",
		.gpio = VCOREIII_GPIO_SGPIO_BASE(3) + 2,
		.active_low = true,
	},
};

static struct gpio_led_platform_data ms220_48_led_data = {
	.leds = ms220_48_led_pins,
	.num_leds = ARRAY_SIZE(ms220_48_led_pins),
};

static struct platform_device ms220_48_leds = {
	.name = "leds-gpio",
	.id = -1,
	.dev = {
		.platform_data = &ms220_48_led_data,
	}
};

static const u8 ms220_48_pd690xx_slave_addr[] = { 0, 1, 3, 5 };

static struct pd690xx_platform_data ms220_48_pd690xx_data = {
	.reset			=	VCOREIII_GPIO_BASE_SLV + 8,
	.disable_ports		=	VCOREIII_GPIO_BASE_SLV + 9,
	.ports			=	48,
	.slave_addr		=	ms220_48_pd690xx_slave_addr,
	.port_base		=	1, /* Match numbers on faceplate */
	.therm_domain		=	"system",
	.power_budget		=	0,
	.controllers		=	4,
	.fast_state_detect	=	true,
};

static const struct i2c_board_info phy_i2c_board_info[] = {
	{
		I2C_BOARD_INFO("pd690xx", 0x30),
		.platform_data = &ms220_48_pd690xx_data,
	},
};

#ifdef CONFIG_VTSS_VCOREIII_PHY8512_GPIO

static struct phy8512_gpio_chip ms220_48_phy8512_gpio[] = {
	{
		.gpio_chip = {
			.label = "phy8512-gpio0",
			.get = jaguar_phy8512_gpio_get_value,
			.set = jaguar_phy8512_gpio_set_value,
			.direction_input = jaguar_phy8512_gpio_direction_input,
			.direction_output = jaguar_phy8512_gpio_direction_output,
			.to_irq = jaguar_phy8512_gpio_to_irq,
			.base = VCOREIII_GPIO_PHY_BASE(0),
			.ngpio = 32,
			.can_sleep = 1,
		},
		.inst = 0,
		.port_base = 0,
	},
	{
		.gpio_chip = {
			.label = "phy8512-gpio1",
			.get = jaguar_phy8512_gpio_get_value,
			.set = jaguar_phy8512_gpio_set_value,
			.direction_input = jaguar_phy8512_gpio_direction_input,
			.direction_output = jaguar_phy8512_gpio_direction_output,
			.to_irq = jaguar_phy8512_gpio_to_irq,
			.base = VCOREIII_GPIO_PHY_BASE(1),
			.ngpio = 32,
			.can_sleep = 1,
		},
		.inst = 0,
		.port_base = 12,
	},
#ifdef CONFIG_VTSS_VCOREIII_JAGUAR_DUAL
	{
		.gpio_chip = {
			.label = "phy8512-gpio2",
			.get = jaguar_slv_phy8512_gpio_get_value,
			.set = jaguar_slv_phy8512_gpio_set_value,
			.direction_input = jaguar_slv_phy8512_gpio_direction_input,
			.direction_output = jaguar_slv_phy8512_gpio_direction_output,
			.to_irq = jaguar_slv_phy8512_gpio_to_irq,
			.base = VCOREIII_GPIO_PHY_BASE(2),
			.ngpio = 32,
			.can_sleep = 1,
		},
		.inst = 0,
		.port_base = 0,
	},
	{
		.gpio_chip = {
			.label = "phy8512-gpio3",
			.get = jaguar_slv_phy8512_gpio_get_value,
			.set = jaguar_slv_phy8512_gpio_set_value,
			.direction_input = jaguar_slv_phy8512_gpio_direction_input,
			.direction_output = jaguar_slv_phy8512_gpio_direction_output,
			.to_irq = jaguar_slv_phy8512_gpio_to_irq,
			.base = VCOREIII_GPIO_PHY_BASE(3),
			.ngpio = 32,
			.can_sleep = 1,
		},
		.inst = 0,
		.port_base = 12,
	},
#endif /* CONFIG_VTSS_VCOREIII_JAGUAR_DUAL */
};

#endif /* CONFIG_VTSS_VCOREIII_PHY8512_GPIO */

static struct thermal_gpio_platform_data ms220_48_fan_data = {
	.gpio = VCOREIII_GPIO_BASE + 20,
	.fan_active = 1,
	.type = "system",
};

static struct platform_device ms220_48_fan = {
	.name = "thermal-gpio",
	.dev = {
		.platform_data = &ms220_48_fan_data,
	}
};

static struct sgpio_platform_data ms220_48_sgpio_data[] = {
	{
		.label = "master-serial-led",
		.gpio_di = -1,
		.gpio_do = VCOREIII_GPIO_BASE + 5,
		.gpio_clk = VCOREIII_GPIO_BASE + 4,
		.base = VCOREIII_GPIO_SGPIO_BASE(1),
		.num_ports = 2,
		.gpios_per_port = 2,
		.init_value = 1,
	},
	{
		.label = "slave-serial-led",
		.gpio_di = -1,
		.gpio_do = VCOREIII_GPIO_BASE_SLV + 5,
		.gpio_clk = VCOREIII_GPIO_BASE_SLV + 4,
		.base = VCOREIII_GPIO_SGPIO_BASE(3),
		.num_ports = 2,
		.gpios_per_port = 2,
		.init_value = 1,
	},
};

struct platform_device ms220_48_sgpio_platform_data[] = {
	{
		.name = "vcoreiii-sgpio",
		.id = 0,
		.dev = {
			.platform_data = &ms220_48_sgpio_data[0]
		},
	},

	{
		.name = "vcoreiii-sgpio",
		.id = 1,
		.dev = {
			.platform_data = &ms220_48_sgpio_data[1]
		},
	},
};

static struct cisco_rps_platform_data ms220_48_rps_data = {
	.i2c_adapter = 8,
	.status_0 = VCOREIII_GPIO_BASE + 9,
	.status_1 = VCOREIII_GPIO_BASE + 8,
	.max_system_power = 250 * 1000,
};

static struct platform_device ms220_48_rps = {
	.name = "cisco-rps",
	.id = -1,
	.dev = {
		.platform_data = &ms220_48_rps_data,
	},
	.num_resources = 0,
};

static struct platform_device *ms220_48_devices[] __initdata = {
	&ms220_48_sgpio_platform_data[0],
	&ms220_48_leds,
	&ms220_48_sfp_gpio_i2c_controller[0],
	&ms220_48_fan,
	&ms220_48_power_i2c_controller,
	&ms220_48_rps,
};

static void ms220_48_machine_restart(char *command)
{
	int i;

	for (i = 0; i < 100; i++) {
		gpio_direction_output(VCOREIII_GPIO_BASE + 0, 0);
		udelay(100);
	}
        writel(VTSS_F_DEVCPU_GCB_DEVCPU_RST_REGS_SOFT_CHIP_RST_SOFT_CHIP_RST,
               VTSS_DEVCPU_GCB_DEVCPU_RST_REGS_SOFT_CHIP_RST);

	while (true)
		cpu_wait();
}

int __init ms220_48_late_init(uint8_t board_type)
{
        int i;
        int ret;
        struct i2c_adapter *adap;
        uint32_t poe_budget = 0;

        if (board_type == MERAKI_BOARD_MS220_48LP)
            poe_budget = 370 * 1000;
        else if (board_type == MERAKI_BOARD_MS220_48FP)
            poe_budget = 740 * 1000;

        ms220_48_pd690xx_data.power_budget = poe_budget;
        ms220_48_rps_data.max_poe_power = poe_budget;

        ms220_48_sgpio_data[0].ctrl =
                VTSS_DEVCPU_GCB_SIO_CTRL_SIO_INPUT_DATA(1, 0);
#ifdef CONFIG_VTSS_VCOREIII_PHY8512_GPIO
        gpiochip_add(&ms220_48_phy8512_gpio[0].gpio_chip);
        gpiochip_add(&ms220_48_phy8512_gpio[1].gpio_chip);
#endif
#ifdef CONFIG_VTSS_VCOREIII_JAGUAR_DUAL
        if (map_base_slv_switch) {
                ms220_48_sgpio_data[1].ctrl =
                        (struct vcoreiii_sgpio_ctrl *)
                        master2slave_addr(VTSS_DEVCPU_GCB_SIO_CTRL_SIO_INPUT_DATA(1, 0));
                platform_device_register(&ms220_48_sgpio_platform_data[1]);
#ifdef CONFIG_VTSS_VCOREIII_PHY8512_GPIO
                gpiochip_add(&ms220_48_phy8512_gpio[2].gpio_chip);
                gpiochip_add(&ms220_48_phy8512_gpio[3].gpio_chip);
#endif /* CONFIG_VTSS_VCOREIII_PHY8512_GPIO */
        }
#endif /* CONFIG_VTSS_VCOREIII_JAGUAR_DUAL */

        platform_add_devices(ms220_48_devices, ARRAY_SIZE(ms220_48_devices));

        adap = i2c_get_adapter(1);
        if (!adap)
                return -ENODEV;

        i2c_new_device(adap, &phy_i2c_board_info[0]);
        i2c_put_adapter(adap);

        for (i = 0; i < 4; ++i) {
                adap = i2c_get_adapter(3 + i);
                if (!adap)
                        return -ENODEV;
                i2c_new_device(adap, &sfp_i2c_board_info[i]);
                i2c_put_adapter(adap);
        }

        ret = gpio_request(VCOREIII_GPIO_BASE + 0, "reset");
        if (ret < 0) {
                printk(KERN_ERR "Unable to request reset pin, err=%d\n",
                       ret);
                return ret;
        } else {
                gpio_direction_output(VCOREIII_GPIO_BASE + 0, 1);
                _machine_restart = ms220_48_machine_restart;
        }
        return 0;
}
