/*
 * TI81XX Power Management
 *
 * This module implements TI81XX specific Power management Routines
 *
 * Copyright (C) {2011} Texas Instruments Incorporated - http://www.ti.com/
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation version 2.
 *
 * This program is distributed "as is" WITHOUT ANY WARRANTY of any
 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/module.h>
#include <linux/opp.h>

#include <plat/cpu.h>
#include <plat/omap_device.h>

#include "omap_opp_data.h"

static struct omap_opp_def __initdata ti814x_opp_def_list[] = {
	/* Disabling OPP50/120 and keep system default/reset voltage as
	 * enough testing is not done
	 * and may cause system instability on some boards
	 */
	OPP_INITIALIZER("mpu", true, 600000000, 1100000),
	/* MPU OPP2 - OPP 100 */
	OPP_INITIALIZER("mpu", false, 720000000, 1200000),
	/* MPU OPP3 - OPP 120 */
	OPP_INITIALIZER("mpu", true, 1000000000, 1350000),

};

/**
 * ti814x_opp_init() - initialize ti814x opp table
 */
static int __init ti814x_opp_init(void)
{
	int r = -ENODEV;

	if (!cpu_is_ti814x() || !(omap_rev() > TI8148_REV_ES1_0))
		return r;

	r = omap_init_opp_table(ti814x_opp_def_list,
			ARRAY_SIZE(ti814x_opp_def_list));

	return r;
}
device_initcall(ti814x_opp_init);
