/*
 * Meraki Z1
 * Based on Atheros DB120 reference board support
 *
 * Author: Dean Thomas (deathoma@cisco.com)
 * Copyright (c) 2013 Cisco-Meraki, Inc.
 *
 */

#include <linux/pci.h>
#include <linux/ath9k_platform.h>
#include <linux/platform_device.h>
#include <linux/leds-nu801.h>
#include <linux/mtd/mtd.h>

#include "machtypes.h"
#include "dev-gpio-buttons.h"
#include "dev-leds-gpio.h"
#include "dev-spi.h"
#include "dev-usb.h"
#include "dev-wmac.h"
#include "pci.h"
#include "mach_meraki_z1.h"


#include "dev-xlna.h"

static struct gpio_led meraki_z1_leds_gpio[] __initdata = {
    {
        .name = "power:orange",
        .gpio = MERAKI_Z1_GPIO_LED_POWER,
        .default_state = LEDS_GPIO_DEFSTATE_OFF,
        .active_low = 1,
    },
};

static struct gpio_keys_button meraki_z1_gpio_keys[] __initdata = {
    {
	.code = KEY_RESTART,
	.gpio = MERAKI_Z1_GPIO_BTN_RESET,
	.active_low = 1,
	.desc = "Software Reset Switch",
	.type = EV_KEY,
	.wakeup = 0,
	.debounce_interval = MERAKI_Z1_KEYS_DEBOUNCE_INTERVAL,
    },
};

// Configure NU801 tri-color LED support

static struct led_nu801_template tricolor_led_template = {
    .name = "tricolor",
    .num_leds = 1,
    .cki = MERAKI_Z1_GPIO_NU801_CKI,
    .sdi = MERAKI_Z1_GPIO_NU801_SDI,
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

// End configure of NU801 tri-color LED support

// Configure Meraki Z1 XLNA driver

static struct xlna_platform_data xlna_data = {
    .gpio0 = MERAKI_Z1_GPIO_XLNA1,
    .gpio1 = MERAKI_Z1_GPIO_XLNA2,
};

static struct platform_device xlna = {
    .name = "meraki-z1-xlna",
    .id = -1,
    .dev.platform_data = &xlna_data,
};

static struct ath9k_platform_data meraki_z1_pci_ath9k_data;

static struct platform_device *meraki_z1_platform_devices[] __initdata = {
    &tricolor_leds,
    &xlna,
};

static void __init meraki_z1_setup(void)
{
	struct platform_device *nanddev =
		platform_device_alloc("ath79-nand", 0);

	ath79_register_leds_gpio(-1, ARRAY_SIZE(meraki_z1_leds_gpio),
				 meraki_z1_leds_gpio);
	ath79_register_gpio_keys_polled(-1, MERAKI_Z1_KEYS_POLL_INTERVAL,
					ARRAY_SIZE(meraki_z1_gpio_keys),
					meraki_z1_gpio_keys);
	ath79_register_pci();
	ath79_register_usb();
	if (!nanddev ||
	    platform_device_add(nanddev))
		platform_device_put(nanddev);
	platform_add_devices(meraki_z1_platform_devices,
		ARRAY_SIZE(meraki_z1_platform_devices));

}

static int __init meraki_z1_init_sync(void)
{
	struct mtd_info *mtd;
	struct pci_dev *pdev;
	int retlen, ret;

	mtd = get_mtd_device_nm("caldata");
	if (IS_ERR(mtd)) {
		pr_err("%s: Unable to find calibration data, err = %lx\n",
		       __func__, PTR_ERR(mtd));
		return -EIO;
	}

	ret = mtd_read(mtd, MERAKI_Z1_WMAC_CALDATA_OFFSET,
		       sizeof(ath79_wmac_data.eeprom_data), &retlen,
		       (unsigned char *)ath79_wmac_data.eeprom_data);
	if ((ret < 0) || (retlen != sizeof(ath79_wmac_data.eeprom_data))) {
		pr_err("%s: Unable to read %s caldata, err=%d len=%d\n",
		       __func__, "AHB WLAN", ret, retlen);
		ret = -EIO;
		goto err_calio;
	}

	ath79_register_wmac(NULL);

	pdev = pci_get_device(0x168c, 0xff1c, NULL);
	if (!pdev) {
		pr_err("%s: %s PCI device disappeared\n", __func__, "WLAN0");
		ret = -ENODEV;
		goto err_calio;
	}

	ret = mtd_read(mtd, MERAKI_Z1_PCIE_CALDATA_OFFSET,
		       sizeof(meraki_z1_pci_ath9k_data.eeprom_data),&retlen,
		       (unsigned char *)meraki_z1_pci_ath9k_data.eeprom_data);
	if ((ret < 0) ||
	    (retlen != sizeof(meraki_z1_pci_ath9k_data.eeprom_data))) {
		pr_err("%s: Unable to read %s caldata, err=%d len=%d\n",
		       __func__, "PCI WLAN", ret, retlen);
		ret = -EIO;
	} else {
		pdev->dev.platform_data = &meraki_z1_pci_ath9k_data;
	}

	pci_dev_put(pdev);
err_calio:
	put_mtd_device(mtd);

	return ret;
}

device_initcall_sync(meraki_z1_init_sync);

MIPS_MACHINE(ATH79_MACH_MERAKI_Z1, "MERAKI_Z1", "Meraki Z1",
	     meraki_z1_setup);
