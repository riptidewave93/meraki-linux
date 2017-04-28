/*
 *  Meraki MR12 Access Point board support
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
#include <linux/pci.h>
#include <linux/mtd/mtd.h>
#include <linux/ath9k_platform.h>

#include <asm/mips_machine.h>

#include "ath79.h"
#include "machtypes.h"
#include "dev-gpio-buttons.h"
#include "dev-leds-gpio.h"
#include "dev-spi.h"
#include "pci.h"
#include "mach-meraki.h"

#define MR12_KEYS_POLL_INTERVAL		20	/* msecs */
#define MR12_KEYS_DEBOUNCE_INTERVAL	(3 * MR12_KEYS_POLL_INTERVAL)

static bool is_mr12;

static struct gpio_led mr12_leds_gpio[] __initdata = {
	{
		.name		= "power:orange",
		.gpio		= 16,
		.active_low	= 1,
		.default_trigger = "default-on",
	}, {
		.name		= "power:green",
		.gpio		= 17,
		.active_low	= 1,
	}, {
		.name		= "lan",
		.gpio		= 15,
		.active_low	= 1,
	}, {
		.name		= "ssi:0",
		.gpio		= 11,
		.active_low	= 1,
	}, {
		.name		= "ssi:1",
		.gpio		= 12,
		.active_low	= 1,
	}, {
		.name		= "ssi:2",
		.gpio		= 13,
		.active_low	= 1,
	}, {
		.name		= "ssi:3",
		.gpio		= 14,
		.active_low	= 1,
	},
};

static struct gpio_keys_button mr12_gpio_keys[] __initdata = {
	{
		.desc		= "soft_reset",
		.type		= EV_KEY,
		.code		= KEY_RESTART,
		.debounce_interval = MR12_KEYS_DEBOUNCE_INTERVAL,
		.gpio		= 8,
		.active_low	= 1,
	}
};

static struct mtd_partition mr12_spiflash_parts[] = {
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

static struct flash_platform_data mr12_spiflash_data = {
	.name		= "mx25l12805d",
	.parts		= mr12_spiflash_parts,
	.nr_parts	= ARRAY_SIZE(mr12_spiflash_parts),
	.type		= "mx25l12805d",
};

static struct spi_board_info mr12_spi_info[] = {
	{
		.bus_num	= 0,
		.chip_select	= 0,
		.max_speed_hz	= 25000000,
		.modalias	= "m25p80",
		.platform_data	= &mr12_spiflash_data,
	},
};

static struct ath79_spi_platform_data mr12_spi_data = {
	.bus_num		= 0,
	.num_chipselect		= 1,
};

static struct ath9k_platform_data mr12_wlan0 = {
	.devid			= 0x002a,
};

#define MR12_WLAN0_CALDATA_OFFSET (0x11000)

static int __init mr12_init(void)
{
	struct pci_dev *pdev;

	if ((mips_machtype != ATH79_MACH_MERAKI) || !soc_is_ar724x())
		return -ENODEV;

	pdev = pci_get_device(0x168c, 0x002a, NULL);
	if (!pdev) {
		pr_debug("Not an MR12 - didn't find 0168:002a PCI device\n");
		return -ENODEV;
	}

	pci_dev_put(pdev);



	mips_set_machine_name("Meraki MR12");
	ath79_register_leds_gpio(-1, ARRAY_SIZE(mr12_leds_gpio),
				 mr12_leds_gpio);
	ath79_register_gpio_keys_polled(-1, MR12_KEYS_POLL_INTERVAL,
					ARRAY_SIZE(mr12_gpio_keys),
					mr12_gpio_keys);

	ath79_register_spi(&mr12_spi_data, mr12_spi_info,
			   ARRAY_SIZE(mr12_spi_info));

	is_mr12 = true;

	return 0;
}

static int __init mr12_init_sync(void)
{
	struct mtd_info *mtd;
	int retlen, ret;
	struct pci_dev *pdev;

	if (!is_mr12)
		return -ENODEV;

	mtd = get_mtd_device_nm("caldata");
	if (IS_ERR(mtd)) {
		pr_err("%s: Unable to find calibration data, err = %lx\n",
		       __func__, PTR_ERR(mtd));
		return -EIO;
	}

	pdev = pci_get_device(0x168c, 0x002a, NULL);
	if (!pdev) {
		pr_err("%s: WLAN PCI device disappeared\n", __func__);
		ret = -EIO;
		goto err_nowan;
	}

	ret = mtd_read(mtd, MR12_WLAN0_CALDATA_OFFSET,
		       sizeof(mr12_wlan0.eeprom_data), &retlen,
		       (unsigned char *)mr12_wlan0.eeprom_data);
	if ((ret < 0) || (retlen != sizeof(mr12_wlan0.eeprom_data))) {
		pr_err("%s: Unable to read caldata, err=%d len%d\n",
		       __func__, ret, retlen);
		ret = -EIO;
		goto err_calio;
	}
	put_mtd_device(mtd);

	pdev->dev.platform_data = &mr12_wlan0; /* Associate platform data */
	pci_dev_put(pdev);

	meraki_late_init();

	return 0;

err_calio:
	pci_dev_put(pdev);
err_nowan:
	put_mtd_device(mtd);
	return ret;
}

device_initcall(mr12_init);
device_initcall_sync(mr12_init_sync);
