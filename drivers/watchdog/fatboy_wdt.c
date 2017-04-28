/*
 * Cisco Meraki MS420-series watchdog driver
 *
 * Author: Stephen Segal <stsegal@cisco.com>
 *
 * Copyright (C) 2013 Cisco Systems, Inc.
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/watchdog.h>
#include <linux/of.h>
#include <linux/of_platform.h>
#include <linux/miscdevice.h>
#include <linux/workqueue.h>

#include <asm/fatboy_cpld.h>


static bool nowayout = WATCHDOG_NOWAYOUT;
module_param(nowayout, bool, 0);
MODULE_PARM_DESC(nowayout, "Watchdog cannot be stopped once started");

static unsigned int timeout = 30;
module_param(timeout, uint, 0);
MODULE_PARM_DESC(timeout, "Watchdog timeout in seconds (default=30)");

#define MIN_SW_TIMEOUT_BUFFER 5

#define NUM_TIMEOUT_VALUES 16
static uint16_t timeout_map[NUM_TIMEOUT_VALUES] = {
    16, 32, 64, 96, 128, 144, 176, 192, 256, 272, 298, 384, 512, 640, 896, 1024
};

static void sw_wd_expired(struct work_struct *work)
{
    struct sysinfo si;
    do_sysinfo(&si);
    printk(KERN_CRIT "fatboy watchdog: Haven't been pet for %u seconds!!!\n", timeout);
    printk(KERN_CRIT "fatboy watchdog: 1/5/15 min load (divide by 65536): %lu %lu %lu mem: %lu/%lu\n",
           si.loads[0], si.loads[1], si.loads[2], si.freeram, si.totalram);
    dump_stack();
    panic("fatboy watchdog expired. panicking");
}

static DECLARE_DELAYED_WORK(sw_wd_expired_work, sw_wd_expired);

static int fatboy_wdt_start(struct watchdog_device* dev)
{
    uint8_t data = 0;

    schedule_delayed_work(&sw_wd_expired_work, msecs_to_jiffies(timeout * 1000));

    FATBOY_CPLD_WDOG_CTRL_ENABLE_SET(data, 1);
    return fatboy_cpld_write_mask(FATBOY_CPLD_WDOG_CTRL, data, FATBOY_CPLD_WDOG_CTRL_ENABLE_MASK);
}

static int fatboy_wdt_stop(struct watchdog_device* dev)
{
    uint8_t data = 0;

    cancel_delayed_work(&sw_wd_expired_work);

    FATBOY_CPLD_WDOG_CTRL_ENABLE_SET(data, 0);
    return fatboy_cpld_write_mask(FATBOY_CPLD_WDOG_CTRL, data, FATBOY_CPLD_WDOG_CTRL_ENABLE_MASK);
}

static int fatboy_wdt_ping(struct watchdog_device* dev)
{
    uint8_t data = 0;

    cancel_delayed_work(&sw_wd_expired_work);
    schedule_delayed_work(&sw_wd_expired_work, msecs_to_jiffies(timeout * 1000));

    FATBOY_CPLD_WDOG_CTRL_KICK_SET(data, 1);
    return fatboy_cpld_write_mask(FATBOY_CPLD_WDOG_CTRL, data, FATBOY_CPLD_WDOG_CTRL_KICK_MASK);
}

static int fatboy_wdt_set_timeout(struct watchdog_device* dev, unsigned int new_timeout)
{
    uint8_t data = 0;
    int i;

    for (i = 0; i < NUM_TIMEOUT_VALUES; i++) {
        if ((new_timeout + MIN_SW_TIMEOUT_BUFFER) <= timeout_map[i]) {
            break;
        }
    }

    if (i == NUM_TIMEOUT_VALUES) {
        return -EINVAL;
    }

    dev->timeout = new_timeout;
    FATBOY_CPLD_WDOG_CTRL_TIMEOUT_SET(data, i);

    return ((fatboy_cpld_write_mask(FATBOY_CPLD_WDOG_CTRL, data, FATBOY_CPLD_WDOG_CTRL_TIMEOUT_MASK) < 0) ? -1 : dev->timeout);
}

static const struct watchdog_info fatboy_wdt_info = {
    .options = WDIOF_MAGICCLOSE | WDIOF_KEEPALIVEPING | WDIOF_SETTIMEOUT,
    .identity = "Cisco Meraki MS420 watchdog",
};

static const struct watchdog_ops fatboy_wdt_ops = {
    .owner = THIS_MODULE,
    .start = fatboy_wdt_start,
    .stop = fatboy_wdt_stop,
    .ping = fatboy_wdt_ping,
    .set_timeout = fatboy_wdt_set_timeout,
};

static struct watchdog_device fatboy_wdt_wdd = {
    .info = &fatboy_wdt_info,
    .ops = &fatboy_wdt_ops,
};

static int __devinit fatboy_wdt_probe(struct platform_device* dev)
{
    int err;
    u8 data;

    /* Before we do anything, we need to check if the CPLD watchdog
       fired since last boot. We only get one chance to read it before
       the bit clears. */
    err = fatboy_cpld_read(FATBOY_CPLD_WDOG_CTRL, &data);
    if (err < 0)
        return err;

    if (FATBOY_CPLD_WDOG_CTRL_FIRED(data))
        fatboy_wdt_wdd.bootstatus |= WDIOF_CARDRESET;

    watchdog_set_nowayout(&fatboy_wdt_wdd, nowayout);
    err = watchdog_register_device(&fatboy_wdt_wdd);
    if (err < 0)
        return err;

    err = fatboy_wdt_set_timeout(&fatboy_wdt_wdd, timeout);
    if (err < 0)
        return err;

    dev_info(&dev->dev, "Cisco Meraki MS420 watchdog");

    return 0;
}

static int __devexit fatboy_wdt_remove(struct platform_device* dev)
{
    cancel_delayed_work(&sw_wd_expired_work);

    watchdog_unregister_device(&fatboy_wdt_wdd);

    return 0;
}

static const struct of_device_id fatboy_wdt_ids[] = {
    {
        .compatible = "meraki,fatboy-wdt",
    },
    {},
};

static struct platform_driver fatboy_wdt_driver = {
    .driver = {
        .name = "fatboy-wdt",
        .owner = THIS_MODULE,
        .of_match_table = fatboy_wdt_ids,
    },
    .probe = fatboy_wdt_probe,
    .remove = __devexit_p(fatboy_wdt_remove),
};


module_platform_driver(fatboy_wdt_driver);

MODULE_AUTHOR("Stephen Segal <stsegal@cisco.com>");
MODULE_DESCRIPTION("Cisco Meraki MS420-series watchdog driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS_MISCDEV(WATCHDOG_MINOR);
