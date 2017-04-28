/*
 * LED Kernel Network Trigger
 *
 * Copyright 2015 Cisco Inc.
 *
 * Author: Maxwell Dergosits <maxwell.dergosits@meraki.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/spinlock.h>
#include <linux/slab.h>
#include <linux/timer.h>
#include <linux/sched.h>
#include <linux/string.h>
#include <linux/leds.h>
#include <linux/ledtrig-network.h>
#include "leds.h"

#define _LED_HALF_PERIOD 125

static DEFINE_SPINLOCK(network_trigger_lock);

static const char * portnames[5] = {"port0", "port1", "port2", "port3", "port4"};
static struct led_classdev * leds[5];

char led_network_activity;
EXPORT_SYMBOL(led_network_activity);

static char led_toggle;
static struct timer_list timer;
static char led_off_cycle;

static void led_network_function(unsigned long data)
{
    unsigned long brightness;
    unsigned long delay = msecs_to_jiffies(_LED_HALF_PERIOD);
    int i;

    for (i = 0; i < 5; i++) {

        if (leds[i]) {
            if (led_toggle & (1 << i)) {  //LED is off, turn it back on
                brightness = leds[i]->max_brightness;
                led_toggle &= ~(1 << i);
                led_network_activity &= ~(1 << i);  //clear network activity when turning back on, so we don't count stuff during our off time.
            } else if (led_off_cycle && (led_network_activity & (1 << i))) {  //LED is on and we've detected network activity, need to blink off
                brightness = LED_OFF;
                led_toggle |= (1 << i);
            }
            led_set_brightness(leds[i], brightness);
        }
    }

    led_off_cycle = !led_off_cycle;
    mod_timer(&timer, jiffies + delay);
}


static void network_trig_activate(struct led_classdev *led_cdev)
{

    int i;

    spin_lock(&network_trigger_lock);

    for (i = 0; i < 5; i++) {
        if(!strncmp(led_cdev->name, portnames[i], strlen(portnames[i]))) {
            leds[i] = led_cdev;
            break;
        }
    }

    spin_unlock(&network_trigger_lock);

}

static void network_trig_deactivate(struct led_classdev *led_cdev)
{
    int i;

    led_set_brightness(led_cdev, LED_OFF);

    spin_lock(&network_trigger_lock);

    for (i = 0; i < 5; i++) {
        if (leds[i] == led_cdev) {
            leds[i] = NULL;
            break;
        }
    }

    spin_unlock(&network_trigger_lock);
}

static struct led_trigger network_led_trigger = {
    .name     = "network",
    .activate = network_trig_activate,
    .deactivate = network_trig_deactivate,
};

static int __init network_trig_init(void)
{
    setup_timer(&timer,
            led_network_function, 0);
    led_network_function(0);
    return led_trigger_register(&network_led_trigger);
}

static void __exit network_trig_exit(void)
{
    del_timer_sync(&timer);
    led_trigger_unregister(&network_led_trigger);
}

module_init(network_trig_init);
module_exit(network_trig_exit);

MODULE_AUTHOR("Maxwell Dergosits <maxwell.dergosits@meraki.net>");
MODULE_DESCRIPTION("Network LED trigger");
MODULE_LICENSE("GPL");
