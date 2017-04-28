/*
 *  Meraki MR11/MR14 Access Point board support
 *
 *  Author: Kevin Paul Herbert <kph@meraki.net>
 *  Copyright (c) 2013 Meraki, Inc.
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 */

#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/mtd/partitions.h>
#include <linux/spi/spi.h>
#include <linux/spi/flash.h>
#include <linux/i2c.h>
#include <linux/i2c-gpio.h>
#include <linux/i2c/pca953x.h>
#include <linux/gpio.h>
#include <linux/pci.h>
#include <asm/mips_machine.h>

#include "ath79.h"
#include "machtypes.h"
#include "dev-gpio-buttons.h"
#include "dev-leds-gpio.h"
#include "dev-spi.h"
#include "dev-usb.h"
#include "mach-meraki.h"

#define MR11_MR14_GPIO_I2C_SCL	1
#define MR11_MR14_GPIO_I2C_SDA	2

#define MR11_MR14_GPIO_I2C_RESET 5

#define MR11_MR14_GPIO_WATCHDOG 11

#define MR11_MR14_GPIO_EXP_BASE	16

#define MR11_MR14_KEYS_POLL_INTERVAL	 20	/* msecs */
#define MR11_MR14_KEYS_DEBOUNCE_INTERVAL (3 * MR11_MR14_KEYS_POLL_INTERVAL)

static bool is_mr11_mr14;

static struct delayed_work mr11_mr14_watchdog_work;
bool watchdog_status;

static struct i2c_gpio_platform_data mr11_mr14_i2c_gpio_data = {
	.sda_pin        = MR11_MR14_GPIO_I2C_SDA,
	.scl_pin        = MR11_MR14_GPIO_I2C_SCL,
};

static struct platform_device mr11_mr14_i2c_gpio_device = {
	.name		= "i2c-gpio",
	.id		= 0,
	.dev = {
		.platform_data	= &mr11_mr14_i2c_gpio_data,
	}
};

static struct pca953x_platform_data mr11_mr14_i2c_pca953x_data = {
	.gpio_base	= MR11_MR14_GPIO_EXP_BASE,
	.irq_base	= -1,
};

static struct i2c_board_info mr11_mr14_i2c_board_info[] __initdata = {
	{
		I2C_BOARD_INFO("pca9534", 0x73),
		.platform_data = &mr11_mr14_i2c_pca953x_data,
	},
};

static struct gpio_led mr11_mr14_leds_gpio[] __initdata = {
	{
		.name		= "power:orange",
		.gpio		= MR11_MR14_GPIO_EXP_BASE + 1,
		.active_low	= 1,
		.default_trigger = "default-on",
	},
	{
		.name		= "power:green",
		.gpio		= MR11_MR14_GPIO_EXP_BASE + 0,
		.active_low	= 1,
	},
	{
		.name		= "lan",
		.gpio		= MR11_MR14_GPIO_EXP_BASE + 2,
		.active_low	= 1,
	},
	{
		.name		= "ssi:0",
		.gpio		= MR11_MR14_GPIO_EXP_BASE + 3,
		.active_low	= 1,
	},
	{
		.name		= "ssi:1",
		.gpio		= MR11_MR14_GPIO_EXP_BASE + 4,
		.active_low	= 1,
	},
	{
		.name		= "ssi:2",
		.gpio		= MR11_MR14_GPIO_EXP_BASE + 5,
		.active_low	= 1,
	},
	{
		.name		= "ssi:3",
		.gpio		= MR11_MR14_GPIO_EXP_BASE + 6,
		.active_low	= 1,
	},
};

static struct gpio_keys_button mr11_mr14_gpio_keys[] __initdata = {
	{
		.desc		= "soft_reset",
		.type		= EV_KEY,
		.code		= KEY_RESTART,
		.debounce_interval = MR11_MR14_KEYS_DEBOUNCE_INTERVAL,
		.gpio		= 8,
		.active_low	= 1,
	},
};

static struct mtd_partition mr11_mr14_spiflash_parts[] = {
	{
		.name	= "Redboot",
		.offset = 0x00000000,
		.size	= 0x00080000,
	},
	{
		.name	= "board-config",
		.offset	= 0x00080000,
		.size	= 0x00020000,
	},
	{
		.name	= "panic",
		.offset	= 0x000A0000,
		.size	= 0x00020000,
	},
	{
		.name	= "storage",
		.offset	= 0x000C0000,
		.size	= 0x00200000,
	},
	{
		.name	= "rb_fis_conf",
		.offset	= 0x00FE0000,
		.size	= 0x00020000,
	},
	{
		.name	= "part1",
		.offset	= 0x002C0000,
		.size	= 0x00680000,
	},
	{
		.name	= "part2",
		.offset	= 0x00940000,
		.size	= 0x00680000,
	},
	{
		.name	= "ALL",
		.offset	= 0x00000000,
		.size	= 0x01000000,
	},
};

static struct flash_platform_data mr11_mr14_spiflash_data = {
	.name		= "s25sl12801",
	.parts		= mr11_mr14_spiflash_parts,
	.nr_parts	= ARRAY_SIZE(mr11_mr14_spiflash_parts),
	.type		= "s25sl12801",
};

static struct spi_board_info mr11_mr14_spi_info[] = {
	{
		.bus_num	= 0,
		.chip_select	= 0,
		.max_speed_hz	= 25000000,
		.modalias	= "m25p80",
		.platform_data	= &mr11_mr14_spiflash_data,
	},
};

static struct ath79_spi_platform_data mr11_mr14_spi_data = {
	.bus_num		= 0,
	.num_chipselect		= 1,
};

static void mr11_mr14_ping_watchdog(struct work_struct *work)
{
	watchdog_status = !watchdog_status;
	gpio_set_value(MR11_MR14_GPIO_WATCHDOG, watchdog_status);
	schedule_delayed_work(&mr11_mr14_watchdog_work,
			      msecs_to_jiffies(5*1000));
}


static int __init mr11_mr14_init(void)
{
	struct pci_dev *pdev;

	if ((mips_machtype != ATH79_MACH_MERAKI) || !soc_is_ar71xx())
		return -ENODEV;

	pdev = pci_get_device(0x168c, 0x0029, NULL);
	if (!pdev) {
		pr_debug("Not an MR11 - didn't find 0168:0029 PCI device\n");
		return -ENODEV;
	}

	pdev = pci_get_device(0x168c, 0x0029, pdev);
	if (!pdev) {
		mips_set_machine_name("Meraki MR11");
		pr_debug("Found one 0168:0029 device - this is a MR11");
	} else {
		pci_dev_put(pdev); /* Free reference */
		mips_set_machine_name("Meraki MR14");
		pr_debug("Found two 0168:0029 device - this is a MR14");
	}

	gpio_request(MR11_MR14_GPIO_I2C_RESET, "I2C reset");
					/* Grab the I2C reset line */
	gpio_direction_output(MR11_MR14_GPIO_I2C_RESET, 1);
					/* Take the board out of reset */

	gpio_request(MR11_MR14_GPIO_WATCHDOG, "GPIO watchdog");
	gpio_direction_output(MR11_MR14_GPIO_WATCHDOG, 1);

	INIT_DELAYED_WORK(&mr11_mr14_watchdog_work, mr11_mr14_ping_watchdog);
	schedule_delayed_work(&mr11_mr14_watchdog_work,
			      msecs_to_jiffies(5*1000));

	i2c_register_board_info(0, mr11_mr14_i2c_board_info,
				ARRAY_SIZE(mr11_mr14_i2c_board_info));
	platform_device_register(&mr11_mr14_i2c_gpio_device);

	ath79_register_leds_gpio(-1, ARRAY_SIZE(mr11_mr14_leds_gpio),
				 mr11_mr14_leds_gpio);
	ath79_register_gpio_keys_polled(-1, MR11_MR14_KEYS_POLL_INTERVAL,
					ARRAY_SIZE(mr11_mr14_gpio_keys),
					mr11_mr14_gpio_keys);
	ath79_register_spi(&mr11_mr14_spi_data, mr11_mr14_spi_info,
			   ARRAY_SIZE(mr11_mr14_spi_info));

	is_mr11_mr14 = true;

	return 0;
}

static int __init mr11_mr14_init_sync(void)
{
	if (is_mr11_mr14)
		meraki_late_init();

	return 0;
}

device_initcall(mr11_mr14_init);
device_initcall_sync(mr11_mr14_init_sync);
