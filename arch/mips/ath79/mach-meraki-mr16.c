/*
 *  Meraki MR16 Access Point board support
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
#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>
#include <linux/spi/spi.h>
#include <linux/spi/flash.h>
#include <linux/pci.h>
#include <linux/ath9k_platform.h>

#include <asm/mips_machine.h>

#include "ath79.h"
#include "machtypes.h"
#include "dev-gpio-buttons.h"
#include "dev-leds-gpio.h"
#include "dev-spi.h"
#include "pci.h"
#include "mach-meraki.h"

#define MR16_KEYS_POLL_INTERVAL		20	/* msecs */
#define MR16_KEYS_DEBOUNCE_INTERVAL	(3 * MR16_KEYS_POLL_INTERVAL)

static bool is_mr16;

static struct gpio_led mr16_leds_gpio[] __initdata = {
	{
		.name		= "power:orange",
		.gpio		= 5,
		.active_low	= 1,
		.default_trigger = "default-on",
	}, {
		.name		= "power:green",
		.gpio		= 6,
		.active_low	= 1,
	}, {
		.name		= "lan",
		.gpio		= 4,
		.active_low	= 1,
	}, {
		.name		= "ssi:0",
		.gpio		= 0,
		.active_low	= 1,
	}, {
		.name		= "ssi:1",
		.gpio		= 1,
		.active_low	= 1,
	}, {
		.name		= "ssi:2",
		.gpio		= 2,
		.active_low	= 1,
	}, {
		.name		= "ssi:3",
		.gpio		= 3,
		.active_low	= 1,
	},
};

static struct gpio_keys_button mr16_gpio_keys[] __initdata = {
	{
		.desc		= "soft_reset",
		.type		= EV_KEY,
		.code		= KEY_RESTART,
		.debounce_interval = MR16_KEYS_DEBOUNCE_INTERVAL,
		.gpio		= 7,
		.active_low	= 1,
	}
};

static struct mtd_partition mr16_spiflash_parts[] = {
	{
		.name	= "Uboot & env",
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
		.name	= "caldata",
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

static struct flash_platform_data mr16_spiflash_data = {
	.name		= "mx25l12805d",
	.parts		= mr16_spiflash_parts,
	.nr_parts	= ARRAY_SIZE(mr16_spiflash_parts),
	.type		= "mx25l12805d",
};

static struct spi_board_info mr16_spi_info[] = {
	{
		.bus_num	= 0,
		.chip_select	= 0,
		.max_speed_hz	= 25000000,
		.modalias	= "m25p80",
		.platform_data	= &mr16_spiflash_data,
	},
};

static struct ath79_spi_platform_data mr16_spi_data = {
	.bus_num		= 0,
	.num_chipselect		= 1,
};

static struct ath9k_platform_data mr16_wlan0 = {
	.devid			= 0x0029,
};

static struct ath9k_platform_data mr16_wlan1 = {
	.devid			= 0x0029,
};

#define MR16_WLAN0_CALDATA_OFFSET (0x11000)
#define MR16_WLAN1_CALDATA_OFFSET (0x15000)

static int __init mr16_init(void)
{
	struct pci_dev *pdev;

	if ((mips_machtype != ATH79_MACH_MERAKI) || !soc_is_ar71xx())
		return -ENODEV;

	pdev = pci_get_device(0x168c, 0xff1d, NULL);
	if (!pdev) {
		pr_debug("Not a MR16 - didn't find 0168:ff1d device\n");
		return -ENODEV;
	}

	pdev = pci_get_device(0x168c, 0xff1d, pdev);
	if (!pdev) {
		pr_debug("Not a MR16 - only one 0168:ff1d device found\n");
		return -ENODEV;
	}

	pci_dev_put(pdev);

	mips_set_machine_name("Meraki MR16");
	ath79_register_leds_gpio(-1, ARRAY_SIZE(mr16_leds_gpio),
				 mr16_leds_gpio);
	ath79_register_gpio_keys_polled(-1, MR16_KEYS_POLL_INTERVAL,
					ARRAY_SIZE(mr16_gpio_keys),
					mr16_gpio_keys);
	ath79_register_spi(&mr16_spi_data, mr16_spi_info,
			   ARRAY_SIZE(mr16_spi_info));

	is_mr16 = true;

	return 0;
}

static int __init mr16_init_sync(void)
{
	struct mtd_info *mtd;
	int retlen, ret;
	struct pci_dev *wlan0, *wlan1;

	if (!is_mr16)
		return -ENODEV;

	mtd = get_mtd_device_nm("caldata");
	if (IS_ERR(mtd)) {
		pr_err("%s: Unable to find calibration data, err = %lx\n",
		       __func__, PTR_ERR(mtd));
		return -EIO;
	}

	wlan0 = pci_get_device(0x168c, 0xff1d, NULL);
	if (!wlan0) {
		pr_err("%s: %s PCI device disappeared\n", __func__, "WLAN0");
		ret = -ENODEV;
		goto err_nowan0;
	}

	pci_dev_get(wlan0);	/* Hold the reference */
	wlan1 = pci_get_device(0x168c, 0xff1d, wlan0);
	if (!wlan1) {
		pr_err("%s: %s PCI device disappeared\n", __func__, "WLAN1");
		ret = -ENODEV;
		goto err_nowan1;
	}

	ret = mtd_read(mtd, MR16_WLAN0_CALDATA_OFFSET,
		       sizeof(mr16_wlan0.eeprom_data),&retlen,
		       (unsigned char *)mr16_wlan0.eeprom_data);
	if ((ret < 0) || (retlen != sizeof(mr16_wlan0.eeprom_data))) {
		pr_err("%s: Unable to read %s caldata, err=%d len=%d\n",
		       __func__, "WLAN0", ret, retlen);
		ret = -EIO;
		goto err_calio;
	}

	ret = mtd_read(mtd, MR16_WLAN1_CALDATA_OFFSET,
		       sizeof(mr16_wlan1.eeprom_data), &retlen,
		       (unsigned char *)mr16_wlan1.eeprom_data);
	if ((ret < 0) || (retlen != sizeof(mr16_wlan1.eeprom_data))) {
		pr_err("%s: Unable to read %s caldata, err=%d len=%d\n",
		       __func__, "WLAN1", ret, retlen);
		ret = -EIO;
		goto err_calio;
	}
	put_mtd_device(mtd);

	wlan0->dev.platform_data = &mr16_wlan0; /* Associate platform data */
	wlan1->dev.platform_data = &mr16_wlan1; /* Associate platform data */

	pci_dev_put(wlan0);
	pci_dev_put(wlan1);

	meraki_late_init();

	return 0;

err_calio:
	pci_dev_put(wlan1);
err_nowan1:
	pci_dev_put(wlan0);
err_nowan0:
	put_mtd_device(mtd);
	return ret;
}
device_initcall(mr16_init);
device_initcall_sync(mr16_init_sync);
