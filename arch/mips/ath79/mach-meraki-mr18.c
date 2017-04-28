/*
 *  Meraki MR18 Access Point board support
 *
 *  Author: Abhinav Acharya <abhinav.acharya@meraki.net>
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
#include <linux/spi/flash.h>
#include <linux/pci.h>
#include <asm/mips_machine.h>
#include <linux/leds.h>
#include <linux/leds-nu801.h>
#include <linux/ath9k_platform.h>

#include "ath79.h"
#include "machtypes.h"
#include "dev-gpio-buttons.h"
#include "dev-leds-gpio.h"
#include "dev-spi.h"
#include "pci.h"
#include <linux/sysfs.h>
#include <asm/gpio.h>

#define CARNAGE_KEYS_POLL_INTERVAL		20	/* msecs */
#define CARNAGE_KEYS_DEBOUNCE_INTERVAL	(3 * CARNAGE_KEYS_POLL_INTERVAL)

static bool is_mr18;
static bool mr18_has_janky_leds = false;

static void	power_led_release(struct device *dev)
{
   /* empty */
}

static struct gpio_led mr18_leds_gpio[] = {
	{
		.name		= "power:orange",
		.gpio		= 21,
		.active_low	= 0,
		.default_trigger = "default-on",
	}, {
		.name		= "power:white",
		.gpio		= 18,
		.active_low	= 1,
	},
};

static struct gpio_led_platform_data power_led_data = {
    .num_leds = ARRAY_SIZE(mr18_leds_gpio),
    .leds = mr18_leds_gpio,
};

static struct platform_device power_leds = {
    .name = "leds-gpio",
    .id = -1,
    .dev.platform_data = &power_led_data,
    .dev.release = power_led_release,
};

static struct led_nu801_template tricolor_led_template = {
    .name = "tricolor",
    .num_leds = 1,
    .cki = 11,
    .sdi = 12,
    .lei = -1,
    .ndelay = 500,
    .init_brightness = {
        LED_OFF,
        LED_OFF,
        LED_OFF,
    },
#ifdef CONFIG_LEDS_TRIGGERS
    .default_trigger = "none",
#endif
    .led_colors = { "red", "green", "blue" },
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

static struct gpio_keys_button mr18_gpio_keys[] __initdata = {
	{
		.desc		= "soft_reset",
		.type		= EV_KEY,
		.code		= KEY_RESTART,
		.debounce_interval = CARNAGE_KEYS_DEBOUNCE_INTERVAL,
		.gpio		= 17,
		.active_low	= 1,
	}
};

static struct mtd_partition mr18_nand_flash_parts[] = {
	{
		.name	= "nandloader",
		.offset = 0x00000000,
		.size	= 0x00080000,   /* 512 KiB  */
	},
	{
		.name	= "part1",
		.offset	= 0x00080000,
		.size	= 0x00800000,   /* 8192 KiB */
	},
	{
		.name	= "part2",
		.offset	= 0x00880000,
		.size	= 0x00800000,   /* 8192 KiB */
	},
	{
		.name	= "ubi",
		.offset	= 0x01080000,
		.size	= 0x6F00000,  /* 113664 KiB */
	},
	{
		.name	= "odm-caldata",
		.offset	= 0x7FE0000,
		.size	= 0x20000,  /* 128 KiB */
	},
};

static struct flash_platform_data mr18_nand_flash_data = {
	.parts		= mr18_nand_flash_parts,
	.nr_parts	= ARRAY_SIZE(mr18_nand_flash_parts),
};

static struct platform_device mr18_nand = {
    .name = "ath79-nand",
    .id = -1,
    .dev.platform_data = &mr18_nand_flash_data,
};

/*
 * DVT and PVT1 units have LED GPIO pins set differently
 *
 * Create seperate platform data to hold their gpio
 * configuration
 */
static struct gpio_led mr18_janky_leds_gpio[] = {
	{
		.name		= "power:orange",
		.gpio		= 21,
		.active_low	= 0,
	}, {
		.name		= "power:white",
		.gpio		= 4,
		.active_low	= 0,
	},
};

static struct gpio_led_platform_data mr18_janky_power_led_data = {
    .num_leds = ARRAY_SIZE(mr18_janky_leds_gpio),
    .leds = mr18_janky_leds_gpio,
};

static struct platform_device janky_power_leds = {
    .name = "leds-gpio",
    .id = -1,
    .dev.platform_data = &mr18_janky_power_led_data,
};

/*
 * Creat a sysfs entry to enble the DVT/PVT1 GPIO Leds
 *
 * /sys/devices/platform/leds-nu801/janky_led
 *
 */
static void
mr18_janky_led_set (bool enabled)
{
   if (mr18_has_janky_leds == enabled)
      return;

   /* Only support enabling DVT/PVT1 LEDs. Do not
      support actively switching back and forth
      between the old and new GPIO LED configurations */
   if (!enabled)
      return;

   mr18_has_janky_leds = enabled;

   gpio_free(4);

   platform_device_unregister(&power_leds);

   if( gpio_request(18, "tricolor") == 0) {
      if( gpio_direction_output(18, 0) < 0) {
         pr_err("%s: Unable to pull GPIO18 low\n", __func__);
      }
   }
   platform_device_register(&janky_power_leds);
}

static ssize_t
mr18_leds_read(struct device *dev, struct device_attribute *attr, char *buf)
{
    return scnprintf(buf, PAGE_SIZE, "%d\n", mr18_has_janky_leds);
}

static ssize_t
mr18_led_write(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
    if ((count<1)||(count>2))
        return -EINVAL;

    if (buf[0] == '1')
        mr18_janky_led_set(true);
    else if (buf[0] == '0')
        mr18_janky_led_set(false);
    else
        return -EINVAL;

    return count;
}

static DEVICE_ATTR(janky_led, S_IWUSR | S_IRUGO, mr18_leds_read, mr18_led_write);

static int __init mr18_init(void)
{
	if ((mips_machtype != ATH79_MACH_MERAKI) || !soc_is_qca955x())
		return -ENODEV;

	printk("Detected Meraki MR18\n");
	mips_set_machine_name("Meraki MR18");
	ath79_register_gpio_keys_polled(-1, CARNAGE_KEYS_POLL_INTERVAL,
					ARRAY_SIZE(mr18_gpio_keys),
					mr18_gpio_keys);
	platform_device_register(&power_leds);
	platform_device_register(&tricolor_leds);
	platform_device_register(&mr18_nand);

   if (tricolor_led_template.lei < 0) {
	/* Pull GPIO4 (NU801 LEI) low (not used) */
	if( gpio_request(4, "tricolor") == 0) {
		if( gpio_direction_output(4, 0) < 0) {
			 pr_err("%s: Unable to pull GPIO4 low\n", __func__);
			}
	}
   }

	is_mr18 = true;

	if (sysfs_create_file(&tricolor_leds.dev.kobj, &dev_attr_janky_led.attr))
		return -ENODEV;

	return 0;
}

device_initcall(mr18_init);
