#ifndef __LEDS_WS2801_H__
#define __LEDS_WS2801_H__

/*
 * Definitions for LED driver for WS2801
 *
 * Kevin Paul Herbert
 * Copyright (c) 2012, Meraki, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

struct led_ws2801_template {
	const char *name;	/* Name of this LED chain */
	int num_leds;		/* Number of LEDs in the chain */
	unsigned cki;		/* GPIO pin for CKI */
	unsigned sdi;		/* GPIO pin for SDI */
	u32 ndelay;		/* Delay in nanoseconds */
	enum led_brightness init_brightness[3]; /* Default rgb state */
};

struct led_ws2801_platform_data {
	int num_controllers;	/* Numnber of controllers */
	struct led_ws2801_template *template; /* Template per controller */
};

#endif /* __LEDS_WS2801_H__ */
