/*
 * arch/arm/mach-ambarella/Wm831x-regulator-config.c
 *
 * Author: Bingliang-Hu <blhu@ambarella.com>
 *
 * Copyright (C) 2004-2010, Ambarella, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 */
#include <linux/kernel.h>
#include <linux/init.h>

#include <asm/mach-types.h>
#include <asm/mach/arch.h>

#include <mach/hardware.h>
#include <mach/board.h>

#include <linux/regulator/machine.h>

#include <linux/mfd/wm831x/core.h>
#include <linux/mfd/wm831x/pdata.h>
#include <linux/mfd/wm831x/regulator.h>

/* ==========================================================================*/
/* DCDC1: iOne_VDDAX for Cortex and 3D */
static struct regulator_consumer_supply dcdc1_consumers[] __initdata = {
	{
		.supply = "cpu_vcc",
	},
};

static struct regulator_init_data wm8310_default_dcdc1_data __initdata = {
	.constraints = {
		.name = "VDD_AXI_1.0V_1.4V",
		.min_uV = 1000000,
		.max_uV = 1400000,
		.apply_uV = 1,
		.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE | REGULATOR_CHANGE_MODE,
		.valid_modes_mask = REGULATOR_MODE_STANDBY | REGULATOR_MODE_NORMAL,
		.state_mem = {
			.disabled = 1,
			.mode = REGULATOR_MODE_STANDBY,
		},
		.initial_state = PM_SUSPEND_MEM,
		.always_on = 1,
		.boot_on = 1,
	},
	 .num_consumer_supplies = ARRAY_SIZE(dcdc1_consumers),
	 .consumer_supplies = dcdc1_consumers,
};

/* ==========================================================================*/
/* DCDC2 iOne_D1P5 for DDR3 */
static struct regulator_consumer_supply dcdc2_consumers[] __initdata = {
	{
		.supply = "ddr3_vcc",
	},
};

static struct regulator_init_data wm8310_default_dcdc2_data __initdata = {
	.constraints = {
		.name = "VDDQ_i1_0.6V_1.8",
		.min_uV = 600000,
		.max_uV = 1800000,
		.apply_uV = 1,
		.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE | REGULATOR_CHANGE_MODE,
		.valid_modes_mask = REGULATOR_MODE_STANDBY | REGULATOR_MODE_NORMAL,
		.state_mem = {
			.disabled = 1,
			.mode = REGULATOR_MODE_STANDBY,
		},
		.initial_state = PM_SUSPEND_MEM,
		.always_on = 1,
		.boot_on = 1,
	},
	 .num_consumer_supplies = ARRAY_SIZE(dcdc2_consumers),
	 .consumer_supplies = dcdc2_consumers,
};

/* ==========================================================================*/
/* DCDC3 iOne_D3P15 for A3P15 or D3P15 */
static struct regulator_consumer_supply dcdc3_consumers[] __initdata = {
	{
		.supply = "gen_vcc",
	},
	{
		.supply = "lp_vcc",
	},
};
static struct regulator_init_data wm8310_default_dcdc3_data __initdata = {
	.constraints = {
		.name = "VDD33_LP_AND_GEN_3.15V",
		.min_uV = 2800000,
		.max_uV = 3400000,
		.apply_uV = 0,
		.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE | REGULATOR_CHANGE_MODE,
		.valid_modes_mask = REGULATOR_MODE_STANDBY | REGULATOR_MODE_NORMAL,
		.state_mem = {
			.disabled = 1,
			.mode = REGULATOR_MODE_STANDBY,
		},
		.initial_state = PM_SUSPEND_MEM,
		//.always_on = 1,
		.boot_on = 1,
	},
	.num_consumer_supplies = ARRAY_SIZE(dcdc3_consumers),
	.consumer_supplies = dcdc3_consumers,
};

/* ==========================================================================*/
static struct regulator_consumer_supply dcdc4_consumers[] __initdata = {
	{
		.supply = "lcd_vcc",
	},
};
static struct regulator_init_data wm8310_default_dcdc4_data __initdata = {
	.constraints = {
		.name = "VDD_LCD_6.5V_30V",
		.min_uV = 6500000,
		.max_uV = 30000000,
		.apply_uV = 0,
		.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE | REGULATOR_CHANGE_MODE,
		.valid_modes_mask = REGULATOR_MODE_STANDBY | REGULATOR_MODE_NORMAL,
		.state_mem = {
			.disabled = 1,
		},
		//.initial_state = PM_SUSPEND_MAX,
		//.always_on = 1,
		.boot_on = 1,
	},
	 .num_consumer_supplies = ARRAY_SIZE(dcdc4_consumers),
	 .consumer_supplies = dcdc4_consumers,
};

/* ==========================================================================*/
/* LDO1 MIPI_PHY */
static struct regulator_consumer_supply ldo1_consumers[] __initdata = {
	{
		.supply = "mipi_vcc",
	},
};
static struct regulator_init_data wm8310_default_ldo1_data __initdata = {
	.constraints = {
		.name = "VDD_MIPI_PHY_0.9V_3.3V",
		.min_uV = 900000,
		.max_uV = 3300000,
		.apply_uV = 1,
		.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE | REGULATOR_CHANGE_MODE,
		.valid_modes_mask = REGULATOR_MODE_STANDBY | REGULATOR_MODE_NORMAL,
		.state_mem = {
			.disabled = 1,
			.mode = WM831X_LDO1_SLP_MODE,
		},
		.initial_state = PM_SUSPEND_MEM,
		//.always_on = 1,
		.boot_on = 1,
	},
	 .num_consumer_supplies = ARRAY_SIZE(ldo1_consumers),
	 .consumer_supplies = ldo1_consumers,
};

/* ==========================================================================*/
/* LDO2 SEN_VDD */
static struct regulator_consumer_supply ldo2_consumers[] __initdata = {
	{
		.supply = "sensor_vcc",
	},
};
static struct regulator_init_data wm8310_default_ldo2_data __initdata = {
	.constraints = {
		.name = "VDD_SEN_0.9V_3.3V",
		.min_uV = 900000,
		.max_uV = 3300000,
		.apply_uV = 1,
		.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE | REGULATOR_CHANGE_MODE,
		.valid_modes_mask = REGULATOR_MODE_STANDBY | REGULATOR_MODE_NORMAL,
		.state_mem = {
			.disabled = 1,
			.mode = WM831X_LDO2_SLP_MODE,
		},
		.initial_state = PM_SUSPEND_MEM,
		//.always_on = 1,
		.boot_on = 1,
	},
	 .num_consumer_supplies = ARRAY_SIZE(ldo2_consumers),
	 .consumer_supplies = ldo2_consumers,
};

/* ==========================================================================*/
/* LDO3 Audio codec power */
static struct regulator_consumer_supply ldo3_consumers[] __initdata = {
	{
		.supply = "audio_vcc",
	},
};
static struct regulator_init_data wm8310_default_ldo3_data __initdata = {
	.constraints = {
		.name = "VDD_AUD_0.9V_3.3V",
		.min_uV = 900000,
		.max_uV = 3300000,
		.apply_uV = 1,
		.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE | REGULATOR_CHANGE_MODE,
		.valid_modes_mask = REGULATOR_MODE_STANDBY | REGULATOR_MODE_NORMAL,
		.state_mem = {
			.disabled = 1,
			.mode = WM831X_LDO3_SLP_MODE,
		},
		.initial_state = PM_SUSPEND_MEM,
		//.always_on = 1,
		.boot_on = 1,
	},
	 .num_consumer_supplies = ARRAY_SIZE(ldo3_consumers),
	 .consumer_supplies = ldo3_consumers,
};

/* ==========================================================================*/
/* LDO4 gyro sensor */
static struct regulator_consumer_supply ldo4_consumers[] __initdata = {
	{
		.supply = "gyro_vcc",
	},
};
static struct regulator_init_data wm8310_default_ldo4_data __initdata = {
	.constraints = {
		.name = "VDD_GY_0.9V_3.3V",
		.min_uV = 900000,
		.max_uV = 3300000,
		.apply_uV = 1,
		.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE | REGULATOR_CHANGE_MODE,
		.valid_modes_mask = REGULATOR_MODE_STANDBY | REGULATOR_MODE_NORMAL,
		.state_mem = {
			.disabled = 1,
			.mode = WM831X_LDO4_SLP_MODE,
		},
		.initial_state = PM_SUSPEND_MEM,
		//.always_on = 1,
		.boot_on = 1,
	},
	 .num_consumer_supplies = ARRAY_SIZE(ldo4_consumers),
	 .consumer_supplies = ldo4_consumers,
};

/* ==========================================================================*/
/* LDO5 VDD33_SDXC */
static struct regulator_consumer_supply ldo5_consumers[] __initdata = {
	{
		.supply = "sdxc_vcc",
	},
};
static struct regulator_init_data wm8310_default_ldo5_data __initdata = {
	.constraints = {
		.name = "VDD33_SDXC_0.9V_3.3V",
		/* can not get 3.15 but 3.1 or 3.2 */
		.min_uV = 900000,
		.max_uV = 3300000,
		.apply_uV = 1,
		.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE | REGULATOR_CHANGE_MODE,
		.valid_modes_mask = REGULATOR_MODE_STANDBY | REGULATOR_MODE_NORMAL,
		.state_mem = {
			.disabled = 1,
			.mode = WM831X_LDO5_SLP_MODE,
		},
		.initial_state = PM_SUSPEND_MEM,
		//.always_on = 1,
		.boot_on = 1,
	},
	 .num_consumer_supplies = ARRAY_SIZE(ldo5_consumers),
	 .consumer_supplies = ldo5_consumers,
};

/* ==========================================================================*/
/* LDO6 Gsensor */
static struct regulator_consumer_supply ldo6_consumers[] __initdata = {
	{
		.supply = "gsensor_vcc",
	},
};
static struct regulator_init_data wm8310_default_ldo6_data __initdata = {
	.constraints = {
		.name = "VDD_G_0.9V_3.3V",
		.min_uV = 900000,
		.max_uV = 3300000,
		.apply_uV = 1,
		.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE | REGULATOR_CHANGE_MODE,
		.valid_modes_mask = REGULATOR_MODE_STANDBY | REGULATOR_MODE_NORMAL,
		.state_mem = {
			.disabled = 1,
			.mode = WM831X_LDO6_SLP_MODE,
		},
		.initial_state = PM_SUSPEND_MEM,
		//.always_on = 1,
		.boot_on = 1,
	},
	 .num_consumer_supplies = ARRAY_SIZE(ldo6_consumers),
	 .consumer_supplies = ldo6_consumers,
};

/* ==========================================================================*/
/* LDO7 VDDA_25_XX */
static struct regulator_consumer_supply ldo7_consumers[] __initdata = {
	{
		.supply = "analog_vcc",
	},
};
static struct regulator_init_data wm8310_default_ldo7_data __initdata = {
	.constraints = {
		.name = "VDDA_25_XXX_1V_3.5V",
		.min_uV = 1000000,
		.max_uV = 3500000,
		.apply_uV = 1,
		.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE | REGULATOR_CHANGE_MODE,
		.valid_modes_mask = REGULATOR_MODE_STANDBY | REGULATOR_MODE_NORMAL,
		.state_mem = {
			.disabled = 1,
			.mode = WM831X_LDO7_SLP_MODE,
		},
		.initial_state = PM_SUSPEND_MEM,
		//.always_on = 1,
		.boot_on = 1,
	},
	 .num_consumer_supplies = ARRAY_SIZE(ldo7_consumers),
	 .consumer_supplies = ldo7_consumers,
};

/* ==========================================================================*/
/* LDO8 Image sensor */
static struct regulator_consumer_supply ldo8_consumers[] __initdata = {
	{
		.supply = "imagesen_vcc",
	},
};
static struct regulator_init_data wm8310_default_ldo8_data __initdata = {
	.constraints = {
		.name = "VDD_IMG_SEN_1V_3.5",
		.min_uV = 1000000,
		.max_uV = 3500000,
		.apply_uV = 1,
		.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE | REGULATOR_CHANGE_MODE,
		.valid_modes_mask = REGULATOR_MODE_STANDBY | REGULATOR_MODE_NORMAL,
		.state_mem = {
			.disabled = 1,
			.mode = WM831X_LDO8_SLP_MODE,
		},
		.initial_state = PM_SUSPEND_MEM,
		//.always_on = 1,
		.boot_on = 1,
	},
	 .num_consumer_supplies = ARRAY_SIZE(ldo8_consumers),
	 .consumer_supplies = ldo8_consumers,
};

/* ==========================================================================*/
/* LDO9 VDD_BT */
static struct regulator_consumer_supply ldo9_consumers[] __initdata = {
	{
		.supply = "bt_vcc",
	},
};
static struct regulator_init_data wm8310_default_ldo9_data __initdata = {
	.constraints = {
		.name = "VDD_BT_1V_3.5V",
		.min_uV = 1000000,
		.max_uV = 3500000,
		.apply_uV = 1,
		.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE | REGULATOR_CHANGE_MODE,
		.valid_modes_mask = REGULATOR_MODE_STANDBY | REGULATOR_MODE_NORMAL,
		.state_mem = {
			.disabled = 1,
			.mode = WM831X_LDO9_SLP_MODE,
		},
		.initial_state = PM_SUSPEND_MEM,
		//.always_on = 1,
		.boot_on = 1,
	},
	 .num_consumer_supplies = ARRAY_SIZE(ldo9_consumers),
	 .consumer_supplies = ldo9_consumers,
};

/* ==========================================================================*/
/* LDO10 VDD_LCD */
static struct regulator_consumer_supply ldo10_consumers[] __initdata = {
	{
		.supply = "lcd_ctl_vcc",
	},
};
static struct regulator_init_data wm8310_default_ldo10_data __initdata = {
	.constraints = {
		.name = "VDD_LCD_1V_3.5V",
		.min_uV = 1000000,
		.max_uV = 3500000,
		.apply_uV = 1,
		.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE | REGULATOR_CHANGE_MODE,
		.valid_modes_mask = REGULATOR_MODE_STANDBY | REGULATOR_MODE_NORMAL,
		.state_mem = {
			.disabled = 1,
			.mode = WM831X_LDO10_SLP_MODE,
		},
		.initial_state = PM_SUSPEND_MEM,
		//.always_on = 1,
		.boot_on = 1,
	},
	 .num_consumer_supplies = ARRAY_SIZE(ldo10_consumers),
	 .consumer_supplies = ldo10_consumers,
};

/* ==========================================================================*/
/* ISINK1 backlight */
static struct regulator_consumer_supply lsink1_consumers[] __initdata = {
	{
		.supply = "lcd_bl_sink",
	},
};
static struct regulator_init_data wm8310_default_isink1_data __initdata = {
	.constraints = {
		.name = "VDD_LCD_BL_20mA_28V",
		.min_uA = 19484,
		.max_uA = 19484,
		//.apply_uV = 1,
		.valid_ops_mask = REGULATOR_CHANGE_STATUS,
		.state_mem = {
			.disabled = 1,
		},
		//.initial_state = PM_SUSPEND_MAX,
		//.always_on = 1,
		.boot_on = 1,
	},
	 .num_consumer_supplies = ARRAY_SIZE(lsink1_consumers),
	 .consumer_supplies = lsink1_consumers,
};

/* ==========================================================================*/
static struct wm831x_backlight_pdata wm8310_default_backlight_pdata __initdata = {
	.isink = 1,
	/* 20*2^(13.25) = 19484 */
	.max_uA = 19484,
};

static struct wm831x_backup_pdata wm8310_default_backup_pdata __initdata = {
	.charger_enable = 1,
	.vlim = 2500,  /* mV */
	.ilim = 200,   /* uA */
};

static struct wm831x_battery_pdata wm8310_default_battery_pdata __initdata = {
	.enable = 1,
	.fast_enable = 1,
	.off_mask = 0,
	.trickle_ilim = 50,
	.vsel = 4200,
	.eoc_iterm = 20,
	.fast_ilim = 1000,
	.timeout = 360,
};

static struct wm831x_watchdog_pdata wm8310_default_watchdog_pdata __initdata = {
	.primary = WM831X_WDOG_RESET,
	.secondary = WM831X_WDOG_INTERRUPT,
	.update_gpio = 112,
	.software = 1,
};

static struct wm831x_status_pdata wm8310_default_on_led __initdata = {
	.name = "wm8310:on:",
	.default_src = WM831X_STATUS_POWER,
};

static struct wm831x_status_pdata wm8310_default_chg_led __initdata = {
	.name = "wm8310:chg:",
	.default_src = WM831X_STATUS_CHARGER,
};

struct wm831x_pdata wm8310_default_pdata __initdata = {
	.wm831x_num = 0,
	.irq_cmos = true,
	.disable_touch = true,
	.irq_base = EXT_IRQ(0),
	.gpio_base = EXT_GPIO(0),
	.backlight = &wm8310_default_backlight_pdata,
	.backup = &wm8310_default_backup_pdata,
	.battery = &wm8310_default_battery_pdata,
	.watchdog = &wm8310_default_watchdog_pdata,

	.status = {
		&wm8310_default_on_led,
		&wm8310_default_chg_led,
	},
	.dcdc = {
		&wm8310_default_dcdc1_data, /* DCDC1 */
		&wm8310_default_dcdc2_data, /* DCDC2 */
		&wm8310_default_dcdc3_data, /* DCDC3 */
		&wm8310_default_dcdc4_data, /* DCDC4 */
	},
	.ldo = {
		&wm8310_default_ldo1_data,
		&wm8310_default_ldo2_data,
		&wm8310_default_ldo3_data,
		&wm8310_default_ldo4_data,
		&wm8310_default_ldo5_data,
		&wm8310_default_ldo6_data,
		&wm8310_default_ldo7_data,
		&wm8310_default_ldo8_data,
		&wm8310_default_ldo9_data,
		&wm8310_default_ldo10_data,
	},
	.isink = {
		&wm8310_default_isink1_data, /* ISINK1 */
	},
};

