/*
 *
 * Meraki board definition proxy for Vitesse 742x-based products.
 *
 * Copyright (c) 2013 Meraki, Inc.
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
 */

#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/i2c-gpio.h>

#define SUPPORT_VTSS_VCOREIII_LUTON26

#include <asm/mach-vcoreiii/hardware.h>
#include <asm/mach-vcoreiii/vcoreiii_gpio.h>

#include "ms22_ms220-24.h"
#include "ms220-8.h"
#include <linux/meraki/meraki_config.h>

static struct i2c_gpio_platform_data luton26_config_i2c_controller_data = {
    .sda_pin = VCOREIII_GPIO_BASE + 10,
    .scl_pin = VCOREIII_GPIO_BASE + 11,
    .udelay = 10,
};

static struct meraki_config_platform_data luton26_config_platform_data = {
    .i2c_adapter_id = 1,
    .i2c_address = 0x51,
};

enum {
    PRODUCT_UNDETERMINED = -1,
    PRODUCT_UNKNOWN = 0,
    PRODUCT_MS22_MS220_24,
    PRODUCT_MS220_8,
};

static int __init determine_product(void)
{
    static int product = PRODUCT_UNDETERMINED;

    if (product == PRODUCT_UNDETERMINED) {
        switch (vcoreiii_get_chip_id()) {
        case 0x7427:
            product = PRODUCT_MS22_MS220_24;
            printk(KERN_INFO "Meraki MS22 or MS220-24 board detected\n");
            break;
        case 0x7425:
            product = PRODUCT_MS220_8;
            printk(KERN_INFO "Meraki MS220-8 board detected\n");
            break;
        default:
            product = PRODUCT_UNKNOWN;
            break;
        }
    }

    return product;
}

static int __init meraki_vtss_luton26_late_init(void)
{
    int product, ret;
    struct platform_device *adapter_dev, *eeprom_dev;

    if (!vcoreiii_check_chip_id())
        return -ENODEV;

    product = determine_product();
    if (product == PRODUCT_MS220_8)
        return meraki_ms220_8_late_init();

    /* Set up our I2C adapter */
    adapter_dev = platform_device_alloc("i2c-gpio", 1);
    if (!adapter_dev)
        return -ENOMEM;

    ret = platform_device_add_data(adapter_dev, &luton26_config_i2c_controller_data, sizeof(luton26_config_i2c_controller_data));
    if (ret < 0)
        goto fail_adapter;

    ret = platform_device_add(adapter_dev);
    if (ret < 0)
        goto fail_adapter;


    /* Set up the eeprom device */
    eeprom_dev = platform_device_alloc("meraki-config", -1);
    if (!eeprom_dev) {
        ret = -ENOMEM;
        goto fail_eeprom;
    }

    ret = platform_device_add_data(eeprom_dev, &luton26_config_platform_data, sizeof(luton26_config_platform_data));
    if (ret < 0) {
        platform_device_put(eeprom_dev);
        goto fail_eeprom;
    }

    ret = platform_device_add(eeprom_dev);
    if (!ret) {
        /* Try to use the EEPROM if we have one */
        switch (meraki_config_get_product()) {
        case MERAKI_BOARD_MS220_24P:
        case MERAKI_BOARD_MS220_24:
            return meraki_ms220_24_late_init();
        default:
            platform_device_del(eeprom_dev);
            break;
        }
    }

    platform_device_put(eeprom_dev);

    if (product == PRODUCT_MS22_MS220_24) {
        return meraki_ms22_late_init();
    }

    ret = -ENODEV;

fail_eeprom:
    platform_device_del(adapter_dev);
fail_adapter:
    platform_device_put(adapter_dev);
    return ret;
}

late_initcall(meraki_vtss_luton26_late_init);
