/*
 * Meraki MS420 series CPLD status register driver
 *
 * Copyright (C) 2013 Cisco Systems, Inc.
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#include <linux/stddef.h>
#include <linux/kernel.h>
#include <linux/kdev_t.h>
#include <linux/seq_file.h>
#include <linux/of_platform.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/leds.h>

#include <asm/fatboy_cpld.h>
#include <asm/fsl_lbc.h>

enum fatboy_leds {
    FATBOY_LED_FAN_ORANGE,
    FATBOY_LED_FAN_GREEN,
    FATBOY_LED_PSU_ORANGE,
    FATBOY_LED_PSU_GREEN,

    FATBOY_LED_COUNT
};

struct fatboy_cpld_dev {
    struct device* dev;
    u8 __iomem* regs;
    rwlock_t lock;
    struct led_classdev leds[FATBOY_LED_COUNT];
};

static struct fatboy_cpld_dev* cpld_dev = NULL;

int fatboy_cpld_read(enum fatboy_cpld_reg reg, u8* data)
{
    unsigned long flags;

    if (data == NULL)
        return -EINVAL;

    if (cpld_dev == NULL)
        return -ENODEV;

    read_lock_irqsave(&cpld_dev->lock, flags);
    *data = ioread8(cpld_dev->regs + (u8)reg);
    read_unlock_irqrestore(&cpld_dev->lock, flags);

    return 0;
}
EXPORT_SYMBOL(fatboy_cpld_read);

int fatboy_cpld_write_mask(enum fatboy_cpld_reg reg, u8 data, u8 mask)
{
    u8 d;
    unsigned long flags;

    if (cpld_dev == NULL)
        return -ENODEV;

    write_lock_irqsave(&cpld_dev->lock, flags);
    if (mask == 0xFF)
        d = data;
    else {
        d = ioread8(cpld_dev->regs + (u8)reg);
        d &= ~mask;
        d |= (data & mask);
    }
    iowrite8(d, cpld_dev->regs + (u8)reg);
    write_unlock_irqrestore(&cpld_dev->lock, flags);

    return 0;
}
EXPORT_SYMBOL(fatboy_cpld_write_mask);

int fatboy_cpld_write(enum fatboy_cpld_reg reg, u8 data)
{
    return fatboy_cpld_write_mask(reg, data, 0xFF);
}
EXPORT_SYMBOL(fatboy_cpld_write);

void fatboy_cpld_led_brightness_set(struct led_classdev* led_cdev, enum led_brightness value)
{
    uint8_t out;
    uint8_t mask;

    if (led_cdev == &cpld_dev->leds[FATBOY_LED_FAN_ORANGE]) {
        mask = 0x20;
        out = (value == LED_OFF ? 0x20 : 0x00);
    } else if (led_cdev == &cpld_dev->leds[FATBOY_LED_FAN_GREEN]) {
        mask = 0x10;
        out = (value == LED_OFF ? 0x10 : 0x00);
    } else if (led_cdev == &cpld_dev->leds[FATBOY_LED_PSU_ORANGE]) {
        mask = 0x40;
        out = (value == LED_OFF ? 0x40 : 0x00);
    } else if (led_cdev == &cpld_dev->leds[FATBOY_LED_PSU_GREEN]) {
        mask = 0x80;
        out = (value == LED_OFF ? 0x80 : 0x00);
    } else {
        dev_err(cpld_dev->dev, "Unknown LED");
        return;
    }

    fatboy_cpld_write_mask(FATBOY_CPLD_SYS_LED_CTRL, out, mask);
}


enum led_brightness fatboy_cpld_led_brightness_get(struct led_classdev* led_cdev)
{
    uint8_t mask;
    uint8_t in;

    if (led_cdev == &cpld_dev->leds[FATBOY_LED_FAN_ORANGE]) {
        mask = 0x20;
    } else if (led_cdev == &cpld_dev->leds[FATBOY_LED_FAN_GREEN]) {
        mask = 0x10;
    } else if (led_cdev == &cpld_dev->leds[FATBOY_LED_PSU_ORANGE]) {
        mask = 0x40;
    } else if (led_cdev == &cpld_dev->leds[FATBOY_LED_PSU_GREEN]) {
        mask = 0x80;
    } else {
        dev_err(cpld_dev->dev, "Unknown LED");
        return LED_OFF;
    }

    fatboy_cpld_read(FATBOY_CPLD_SYS_LED_CTRL, &in);

    return ((in & mask) ? LED_OFF : LED_FULL);
}


void fatboy_cpld_psu_trigger_activate(struct led_classdev* led_cdev)
{
    if (led_cdev != &cpld_dev->leds[FATBOY_LED_PSU_ORANGE])
        return;

    fatboy_cpld_write_mask(FATBOY_CPLD_MISC_CTRL, 0x00, 0x02);
}

void fatboy_cpld_psu_trigger_deactivate(struct led_classdev* led_cdev)
{
    if (led_cdev != &cpld_dev->leds[FATBOY_LED_PSU_ORANGE])
        return;

    fatboy_cpld_write_mask(FATBOY_CPLD_MISC_CTRL, 0x02, 0x02);
}


static struct led_trigger psu_led_trigger = {
    .name = "fatboy-psu-default",
    .activate = fatboy_cpld_psu_trigger_activate,
    .deactivate = fatboy_cpld_psu_trigger_deactivate,
};


static int __devinit fatboy_cpld_probe(struct platform_device *dev)
{
    int ret = 0;
    int numleds;
    int i;

    if (cpld_dev)
        return -EEXIST;

    if (!fsl_lbc_ctrl_dev || !fsl_lbc_ctrl_dev->regs)
        return -ENODEV;

    cpld_dev = devm_kzalloc(&dev->dev, sizeof(*cpld_dev), GFP_KERNEL);
    if (!cpld_dev)
        return -ENOMEM;

    dev_set_drvdata(&dev->dev, cpld_dev);
    cpld_dev->dev = &dev->dev;

    cpld_dev->regs = of_iomap(dev->dev.of_node, 0);
    if (!cpld_dev->regs) {
        dev_err(cpld_dev->dev, "failed to get device resources\n");
        ret = -EFAULT;
        goto err;
    }

    numleds = of_property_count_strings(dev->dev.of_node, "led-names");

    if (numleds > 0) {
        if (numleds != FATBOY_LED_COUNT) {
            dev_err(cpld_dev->dev, "wrong number of LEDs\n");
            ret = -EFAULT;
            goto err;
        }

        ret = led_trigger_register(&psu_led_trigger);
        if (ret < 0) {
            dev_err(cpld_dev->dev, "failed to register PSU LED trigger");
            goto err;
        }

        for (i = 0; i < FATBOY_LED_COUNT; i++) {
            ret = of_property_read_string_index(dev->dev.of_node, "led-names", i, &cpld_dev->leds[i].name);
            if (ret < 0) {
                dev_err(cpld_dev->dev, "failed to read LED string %d\n", i);
                goto err_trig;
            }

            cpld_dev->leds[i].brightness_set = fatboy_cpld_led_brightness_set;
            cpld_dev->leds[i].brightness_get = fatboy_cpld_led_brightness_get;
            if (i == FATBOY_LED_PSU_ORANGE)
                cpld_dev->leds[i].default_trigger = psu_led_trigger.name;

            ret = led_classdev_register(cpld_dev->dev, &cpld_dev->leds[i]);
            if (ret < 0) {
                dev_err(cpld_dev->dev, "Failed to register LED \"%s\"\n", cpld_dev->leds[i].name);
                goto err_trig;
            }
        }
    }

    rwlock_init(&cpld_dev->lock);

    dev_info(&dev->dev, "Cisco Meraki MS420 CPLD");

    return 0;

err_trig:
    led_trigger_unregister(&psu_led_trigger);

err:
    for (i = 0; i < FATBOY_LED_COUNT; i++)
        if (cpld_dev->leds[i].dev)
            led_classdev_unregister(&cpld_dev->leds[i]);

    devm_kfree(cpld_dev->dev, cpld_dev);
    cpld_dev = NULL;

    return ret;
}

static const struct of_device_id fatboy_cpld_match[] = {
    { .compatible = "meraki,fatboy-cpld", },
    {},
};

static struct platform_driver fatboy_cpld_driver = {
    .driver = {
        .name = "meraki,fatboy-cpld",
        .owner = THIS_MODULE,
        .of_match_table = fatboy_cpld_match,
    },
    .probe = fatboy_cpld_probe,
};

static int __init fatboy_cpld_init(void)
{
    return platform_driver_register(&fatboy_cpld_driver);
}
module_init(fatboy_cpld_init);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Stephen Segal <ssegal@meraki.com>");
MODULE_DESCRIPTION("Meraki MS420-series CPLD driver");
