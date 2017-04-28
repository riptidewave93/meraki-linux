/*
 * Code for DM385 EVM.
 *
 * Copyright (C) 2010 Texas Instruments, Inc. - http://www.ti.com/
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
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/spi/spi.h>
#include <linux/spi/flash.h>
#include <linux/platform_device.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/partitions.h>
#include <linux/gpio.h>
#include <linux/i2c.h>
#include <linux/phy.h>
#include <linux/i2c/at24.h>
#include <linux/i2c-algo-bit.h>
#include <linux/i2c-gpio.h>
#include <linux/regulator/machine.h>
#include <linux/mfd/tps65910.h>
#include <linux/wl12xx.h>
#include <linux/clk.h>
#include <linux/err.h>
#include <linux/ramoops.h>
#include <linux/memblock.h>

#include <mach/hardware.h>
#include <mach/gpioc.h>
#include <mach/a3967.h>
#include <mach/md124.h>
#include <mach/drv8836.h>
#include <asm/gpio.h>
#include <linux/input.h>
#include <linux/gpio_keys.h>
#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <asm/mach/map.h>

#include <plat/mcspi.h>
#include <plat/irqs.h>
#include <plat/board.h>
#include <plat/common.h>
#include <plat/asp.h>
#include <plat/usb.h>
#include <plat/mmc.h>
#include <plat/gpmc.h>
#include <plat/nand.h>
#include <plat/hdmi_lib.h>
#include <mach/board-ti814x.h>
#ifdef CONFIG_HAVE_PWM
#include <plat/pwm.h>
#endif

#include "board-flash.h"
#include "clock.h"
#include "mux.h"
#include "hsmmc.h"
#include "control.h"

/* Convert GPIO signal to GPIO pin number */
#define GPIO_TO_PIN(bank,gpio) (32 * (bank) + (gpio))


#ifdef CONFIG_OMAP_MUX

#define OMAP_MUX_INPUT (OMAP_MUX_MODE7 | TI814X_PULL_DIS | TI814X_INPUT_EN)
static struct omap_board_mux board_mux[] __initdata = {
	TI814X_MUX(VIN0_D0,			OMAP_MUX_MODE7),					/* gp1[11]  DO_0 */
	TI814X_MUX(GPMC_A22,		OMAP_MUX_MODE6),					/* gp1[17]  PWM */
	TI814X_MUX(GPMC_A23,		OMAP_MUX_MODE6),					/* gp1[18]  IRIS_PWM */
	TI814X_MUX(MMC2_DAT7,		OMAP_MUX_MODE7),					/* gp1[19]  I2C1_SDA*/
	TI814X_MUX(MMC2_DAT6,		OMAP_MUX_MODE7),					/* gp1[20]  I2C1_SCL*/
	TI814X_MUX(GPMC_CLK,		OMAP_MUX_MODE7),					/* gp1[27]  IRLED*/ 
	// change IR_LED from PWM mode into GPIO mode because IR_LED for PWM use TIM4_IO
	// its complicated to modify

	TI814X_MUX(GPMC_A17,		OMAP_MUX_INPUT),					/* gp2[6]   Zoom_Out*/ 
	TI814X_MUX(VIN0_D2,			OMAP_MUX_INPUT),					/* gp2[7]   DI_0 */
	TI814X_MUX(VIN0_D3,			OMAP_MUX_INPUT),					/* gp2[8]   LDEC(Video-out detect)*/
	TI814X_MUX(VIN0_D6,			OMAP_MUX_MODE7),					/* gp2[18]  CMOS_RST*/
	TI814X_MUX(VOUT0_B_CB_C2,	OMAP_MUX_MODE7),					/* gp2[22]  I2C0_SCL*/
	TI814X_MUX(VOUT0_B_CB_C3,	OMAP_MUX_INPUT),					/* gp2[23]  MIC_SW */
	TI814X_MUX(VOUT0_G_Y_YC2,	OMAP_MUX_MODE7),					/* gp2[24]  I2C0_SDA*/
	TI814X_MUX(VOUT0_G_Y_YC3,	OMAP_MUX_INPUT),					/* gp2[25]  NTSC_PAL */
	TI814X_MUX(VOUT0_R_CR3,		OMAP_MUX_INPUT),					/* gp2[27]  HW_RST */
	TI814X_MUX(VOUT1_HSYNC,		OMAP_MUX_MODE7),					/* gp2[29]  SOFT_RESET*/

	TI814X_MUX(VOUT1_B_CB_C3,	OMAP_MUX_MODE7),					/* gp3[0]   ZOOM_B1*/
	TI814X_MUX(VOUT1_B_CB_C4,	OMAP_MUX_MODE7),					/* gp3[1]   ZOOM_A1*/
	TI814X_MUX(VOUT1_B_CB_C5,	OMAP_MUX_MODE7),					/* gp3[2]   ZOOM_A2*/
	TI814X_MUX(VOUT1_B_CB_C6,	OMAP_MUX_MODE7),					/* gp3[3]   FOCUS_B1*///  MODE
	TI814X_MUX(VOUT1_B_CB_C7,	OMAP_MUX_MODE7),					/* gp3[4]   FOCUS_A1*/
	TI814X_MUX(VOUT1_B_CB_C8,	OMAP_MUX_MODE7),					/* gp3[5]   FOCUS_A2*/
	TI814X_MUX(VOUT1_B_CB_C9,	OMAP_MUX_MODE7),					/* gp3[6]   FOCUS_B2*///  MODE
	TI814X_MUX(VOUT1_G_Y_YC3,	OMAP_MUX_MODE7),					/* gp3[7]   LED_G */
	TI814X_MUX(VOUT1_G_Y_YC4,	OMAP_MUX_MODE7),					/* gp3[8]   LED_R */
	TI814X_MUX(VOUT1_G_Y_YC5,	OMAP_MUX_INPUT),					/* gp3[9]   FOCUS_AST*/
	TI814X_MUX(VOUT1_G_Y_YC7,	OMAP_MUX_INPUT),					/* gp3[11]  AT_AF-Det*/// MODE
	TI814X_MUX(VOUT1_G_Y_YC8,	OMAP_MUX_MODE7),					/* gp3[12]  IRCUT ICR*/
	TI814X_MUX(VOUT1_G_Y_YC9,	OMAP_MUX_MODE7),					/* gp3[13]  IRCUT ICR_EN*/
	TI814X_MUX(VOUT1_R_CR5,		OMAP_MUX_MODE7),					/* gp3[15]  PIRIS_EN*/
	TI814X_MUX(VOUT1_R_CR6,		OMAP_MUX_MODE7),					/* gp3[16]  PIRIS_IN1*/
	TI814X_MUX(VOUT1_R_CR7,		OMAP_MUX_MODE7),					/* gp3[17]  PIRIS_IN2*/
	TI814X_MUX(VOUT1_R_CR8,		OMAP_MUX_INPUT),					/* gp3[18]  IRIS_DET*///  MODE
	TI814X_MUX(VOUT1_R_CR9,		OMAP_MUX_MODE7),					/* gp3[19]  IRIS_SEL*///  MODE
	TI814X_MUX(VOUT1_G_Y_YC2,	OMAP_MUX_INPUT),					/* gp3[20]  Zoom_In*/ //   MODEx
	TI814X_MUX(VOUT1_R_CR3,		OMAP_MUX_MODE7),					/* gp3[21]  Heater_HL_Soc*/// MODE
	TI814X_MUX(VOUT1_R_CR2,		OMAP_MUX_INPUT),					/* gp3[22]  AC-DC_Det*/// MODE
	TI814X_MUX(VOUT1_B_CB_C2,	OMAP_MUX_MODE7),					/* gp3[30]  ZOOM_B2*/

	TI814X_MUX(UART0_DSRN,		TI81XX_MUX_MODE6 | TI814X_PIN_INPUT_PULL_UP),	/* gp1[3]   MMC WP */
	TI814X_MUX(SPI0_CS1,		TI81XX_MUX_MODE1 | TI814X_PIN_INPUT_PULL_UP),	/* gp1[6]   MMC CD */

	TI814X_MUX(CLKIN32,		OMAP_MUX_MODE2),    /* unknown GPIO Pin */
	{ .reg_offset = OMAP_MUX_TERMINATOR },
};
#else
#define board_mux     NULL
#endif

static struct omap2_hsmmc_info mmc[] = {
       {
#ifdef CONFIG_WL12XX_PLATFORM_DATA
/* WLAN_EN is GP2[22] */
#define GPIO_WLAN_EN	((2 * 32) + 22)
/* WLAN_IRQ is GP2[24] */
#define GPIO_WLAN_IRQ	((2 * 32) + 24)
		.mmc		= 1,
		.caps		= MMC_CAP_4_BIT_DATA | MMC_CAP_POWER_OFF_CARD,
		.gpio_cd	= -EINVAL,
		.gpio_wp	= -EINVAL,
		.ocr_mask	= MMC_VDD_165_195,
		.nonremovable	= true,
	},
	{
		.mmc            = 2,
#else
        .mmc            = 2,
#endif
		.caps		= MMC_CAP_4_BIT_DATA | MMC_CAP_NEEDS_POLL,
		.gpio_cd	= GPIO_TO_PIN(1, 6), /* Dedicated pins for CD and WP */
		.gpio_wp	= GPIO_TO_PIN(1, 3), /* Dedicatedd pins for WP */
		.ocr_mask	= MMC_VDD_33_34,
	},
	{}	/* Terminator */
};

#if 0
static struct regulator_consumer_supply dm385evm_mpu_supply =
	REGULATOR_SUPPLY("mpu", "mpu.0");
/*
 * DM814x/AM387x (TI814x) devices have restriction that none of the supply to
 * the device should be turned of.
 *
 * NOTE: To prevent turning off regulators not explicitly consumed by drivers
 * depending on it, ensure following:
 *	1) Set always_on = 1 for them OR
 *	2) Avoid calling regulator_has_full_constraints()
 *
 * With just (2), there will be a warning about incomplete constraints.
 * E.g., "regulator_init_complete: incomplete constraints, leaving LDO8 on"
 *
 * In either cases, the supply won't be disabled.
 *
 * We are taking approach (1).
 */
static struct regulator_init_data tps65911_reg_data[] = {
	/* VRTC */
	{
		.constraints = {
			.min_uV = 1800000,
			.max_uV = 1800000,
			.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE |
						REGULATOR_CHANGE_STATUS,
			.always_on = 1,
		},
	},

	/* VIO -VDDA 1.8V */
	{
		.constraints = {
			.min_uV = 1500000,
			.max_uV = 1500000,
			.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE |
						REGULATOR_CHANGE_STATUS,
			.always_on = 1,
		},
	},

	/* VDD1 - MPU */
	{
		.constraints = {
			.min_uV = 600000,
			.max_uV = 1500000,
			.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE,
			.always_on = 1,
		},
		.num_consumer_supplies	= 1,
		.consumer_supplies	= &dm385evm_mpu_supply,
	},

	/* VDD2 - DSP */
	{
		.constraints = {
			.min_uV = 600000,
			.max_uV = 1500000,
			.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE |
						REGULATOR_CHANGE_STATUS,
			.always_on = 1,
		},
	},

	/* VDDCtrl - CORE */
	{
		.constraints = {
			.min_uV = 600000,
			.max_uV = 1400000,
			.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE,
			.always_on = 1,
		},
	},

	/* LDO1 - VDAC */
	{
		.constraints = {
			.min_uV = 1100000,
			.max_uV = 3300000,
			.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE |
						REGULATOR_CHANGE_STATUS,
			.always_on = 1,
		},
	},

	/* LDO2 - HDMI */
	{
		.constraints = {
			.min_uV = 1100000,
			.max_uV = 3300000,
			.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE |
						REGULATOR_CHANGE_STATUS,
			.always_on = 1,
		},
	},

	/* LDO3 - GPIO 3.3V */
	{
		.constraints = {
			.min_uV = 1100000,
			.max_uV = 3300000,
			.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE |
						REGULATOR_CHANGE_STATUS,
			.always_on = 1,
		},
	},

	/* LDO4 - PLL 1.8V */
	{
		.constraints = {
			.min_uV = 1100000,
			.max_uV = 3300000,
			.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE,
			.always_on = 1,
		},
	},

	/* LDO5 - SPARE */
	{
		.constraints = {
			.min_uV = 1100000,
			.max_uV = 3300000,
			.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE |
						REGULATOR_CHANGE_STATUS,
			.always_on = 1,
		},
	},

	/* LDO6 - CDC */
	{
		.constraints = {
			.min_uV = 1100000,
			.max_uV = 3300000,
			.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE,
			.always_on = 1,
		},
	},

	/* LDO7 - SPARE */
	{
		.constraints = {
			.min_uV = 1100000,
			.max_uV = 3300000,
			.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE |
						REGULATOR_CHANGE_STATUS,
			.always_on = 1,
		},
	},

	/* LDO8 - USB 1.8V */
	{
		.constraints = {
			.min_uV = 1100000,
			.max_uV = 3300000,
			.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE |
						REGULATOR_CHANGE_STATUS,
			.always_on = 1,
		},
	},
};

static struct tps65910_board __refdata tps65911_pdata = {
	.irq				= 0,	/* No support currently */
	.gpio_base			= 0,	/* No support currently */
	.tps65910_pmic_init_data	= tps65911_reg_data,
};
#endif

static struct i2c_board_info __initdata ti814x_i2c_boardinfo[] = {
	{
		I2C_BOARD_INFO("tlv320aic3x", 0x18),
	},
	{
		I2C_BOARD_INFO("rx8010", 0x32),
	},
	{
		I2C_BOARD_INFO("isl29003", 0x44),
	},

};

static struct i2c_gpio_platform_data ti814x_ipnc_i2c_gpio_data0 = {
        /* keep defaults for timeouts; pins are push-pull bidirectional */
        .scl_pin = 86,
        .sda_pin = 88,
};

static struct i2c_gpio_platform_data ti814x_ipnc_i2c_gpio_data1 = {
        /* keep defaults for timeouts; pins are push-pull bidirectional */
        .scl_pin = 52,
        .sda_pin = 51,
};

/* first bus: GPIO XX and YY */
static struct platform_device ti814x_ipnc_i2c_dev0 = {
        .name   = "i2c-gpio",
        .id     = 1,
        .dev    = {
                .platform_data = &ti814x_ipnc_i2c_gpio_data0,
        },
};
/* second bus: GPIO XX and YY */
static struct platform_device ti814x_ipnc_i2c_dev1 = {
        .name   = "i2c-gpio",
        .id     = 2,
        .dev    = {
                .platform_data = &ti814x_ipnc_i2c_gpio_data1,
        },
};


/*
static struct i2c_board_info __initdata ti814x_i2c_boardinfo[] = {
	{
		I2C_BOARD_INFO("tlv320aic3104", 0x18),
	},
	{
		I2C_BOARD_INFO("tps65911", 0x2D),
		.platform_data = &tps65911_pdata,
	},
};
*/

static int __init ti814x_ipnc_i2c_gpio_init(void)
{
	platform_device_register(&ti814x_ipnc_i2c_dev0);
	platform_device_register(&ti814x_ipnc_i2c_dev1);

	i2c_register_board_info(1, ti814x_i2c_boardinfo, ARRAY_SIZE(ti814x_i2c_boardinfo));

	return 0;
}

#if 0
static void __init ti814x_evm_i2c_init(void)
{
	/* There are 4 instances of I2C in TI814X but currently only one
	 * instance is being used on the TI8148 EVM
	 */
	omap_register_i2c_bus(1, 100, ti814x_i2c_boardinfo,
				ARRAY_SIZE(ti814x_i2c_boardinfo));
}
#endif

static u8 dm385_iis_serializer_direction[] = {
	TX_MODE,	RX_MODE,	INACTIVE_MODE,	INACTIVE_MODE,
	INACTIVE_MODE,	INACTIVE_MODE,	INACTIVE_MODE,	INACTIVE_MODE,
	INACTIVE_MODE,	INACTIVE_MODE,	INACTIVE_MODE,	INACTIVE_MODE,
	INACTIVE_MODE,	INACTIVE_MODE,	INACTIVE_MODE,	INACTIVE_MODE,
};

static struct snd_platform_data dm385_evm_snd_data = {
	.tx_dma_offset	= 0x46400000,
	.rx_dma_offset	= 0x46400000,
	.op_mode	= DAVINCI_MCASP_IIS_MODE,
	.num_serializer = ARRAY_SIZE(dm385_iis_serializer_direction),
	.tdm_slots	= 2,
	.serial_dir	= dm385_iis_serializer_direction,
	.asp_chan_q	= EVENTQ_2,
	.version	= MCASP_VERSION_2,
	.txnumevt	= 64,
	.rxnumevt	= 64,
};

static struct gpio_led cyclops_leds_gpio[] = {
   {
	  .name	= "power:orange",
	  .gpio	= 103,   // gpio3[7]
	  .active_low = 0,
   },
   {
	  .name	= "power:green",
	  .gpio	= 104,
	  .active_low = 0,
   },
};

static struct gpio_led_platform_data power_led_data = {
	.num_leds = ARRAY_SIZE(cyclops_leds_gpio),
	.leds = cyclops_leds_gpio,
};

static struct platform_device power_leds = {
	.name = "leds-gpio",
	.id = -1,
	.dev.platform_data = &power_led_data,
};

static struct gpio_keys_button cyclops_buttons[] = {
  {
	 .desc	= "soft_reset",
	 .type	= EV_KEY,
	 .code	= KEY_RESTART,
	 .debounce_interval = 60,
	 .gpio	= 91, // gpio2[29]
	 .active_low = 1,
  },
};

static struct gpio_keys_platform_data reset_btn_data = {
	.nbuttons = ARRAY_SIZE(cyclops_buttons),
	.buttons = cyclops_buttons,
	.poll_interval = 20,
};

static struct platform_device reset_btn = {
	.name = "gpio-keys-polled",
	.id = -1,
	.dev.platform_data = &reset_btn_data,
};

/* NAND flash information */
static struct mtd_partition ti814x_nand_partitions[] = {
/* All the partition sizes are listed in terms of NAND block size */
	{
		.name           = "U-Boot-min",
		.offset         = 0,
		.size           = 4 * SZ_128K,
	},
	{
		.name           = "U-Boot",
		.offset         = MTDPART_OFS_APPEND,
		.size           = 25 * SZ_128K,
	},
	{
		.name           = "bootkernel1",
		.offset         = MTDPART_OFS_APPEND,
		.size           = 48 * SZ_128K,
	},
	{
		.name           = "bootkernel2",
		.offset         = MTDPART_OFS_APPEND,
		.size           = 48 * SZ_128K,
	},
	{
		.name           = "ubi",
		.offset         = MTDPART_OFS_APPEND,
		.size           = 800 * SZ_128K,
	},
	{
		.name           = "all",
		.offset         = 0,
		.size           = MTDPART_SIZ_FULL,
	}
};

#ifdef CONFIG_HAVE_PWM
struct omap2_pwm_platform_config dm385_ipnc_pwm_cfg[] ={
/*
	{
		.timer_id = 6,
		.polarity = 1,
	},
*/
// walter @@20140715 only use timer7 for pwm0
	{
		.timer_id = 7,
		.polarity = 1,
	}
};
#endif

static void __init dm385_evm_init_irq(void)
{
	omap2_init_common_infrastructure();
	omap2_init_common_devices(NULL, NULL);
	omap_init_irq();
	gpmc_init();
}

#ifdef CONFIG_SND_SOC_TI81XX_HDMI
static struct snd_hdmi_platform_data dm385_snd_hdmi_pdata = {
	.dma_addr = TI81xx_HDMI_WP + HDMI_WP_AUDIO_DATA,
	.channel = 53,
	.dma_chan_q = EVENTQ_0,
	.data_type = 4,
	.acnt = 4,
	.fifo_level = 0x20,
};

static struct platform_device dm385_hdmi_audio_device = {
	.name   = "hdmi-dai",
	.id     = -1,
	.dev = {
		.platform_data = &dm385_snd_hdmi_pdata,
	}
};

static struct platform_device dm385_hdmi_codec_device = {
	.name   = "hdmi-dummy-codec",
	.id     = -1,
};

static struct platform_device *dm385_devices[] __initdata = {
	&dm385_hdmi_audio_device,
	&dm385_hdmi_codec_device,
};
/*
 * HDMI Audio Auto CTS MCLK configuration.
 * sysclk20, sysclk21, sysclk21 and CLKS(external)
 * setting sysclk20 as the parent of hdmi_i2s_ck
 * ToDo:
 */
void __init ti813x_hdmi_clk_init(void)
{
	int ret = 0;
	struct clk *parent, *child;

	/* modify the clk name to choose diff clk*/
	parent = clk_get(NULL, "sysclk20_ck");
	if (IS_ERR(parent))
		pr_err("Unable to get [sysclk20_ck] clk\n");

	child = clk_get(NULL, "hdmi_i2s_ck");
	if (IS_ERR(child))
		pr_err("Unable to get [hdmi_i2s_ck] clk\n");

	ret = clk_set_parent(child, parent);
	if (ret < 0)
		pr_err("Unable to set parent clk [hdmi_i2s_ck]\n");

	clk_put(child);
	clk_put(parent);
	pr_debug("{{HDMI Audio MCLK setup completed}}\n");
}
#endif

#ifdef CONFIG_WL12XX_PLATFORM_DATA

static struct wl12xx_platform_data wlan_data __initdata = {
	.irq = OMAP_GPIO_IRQ(GPIO_WLAN_IRQ),
	/* COM6 (127x) uses FREF */
	.board_ref_clock = WL12XX_REFCLOCK_38_XTAL,
	/* COM7 (128x) uses TCXO */
	.board_tcxo_clock = WL12XX_TCXOCLOCK_26,
};

static int wl12xx_set_power(struct device *dev, int slot, int power_on,
			    int vdd)
{
	static bool power_state;

	pr_debug("Powering %s wl12xx", power_on ? "on" : "off");

	if (power_on == power_state)
		return 0;
	power_state = power_on;

	if (power_on) {
		/* Power up sequence required for wl127x devices */
		gpio_set_value(GPIO_WLAN_EN, 1);
		usleep_range(15000, 15000);
		gpio_set_value(GPIO_WLAN_EN, 0);
		usleep_range(1000, 1000);
		gpio_set_value(GPIO_WLAN_EN, 1);
		msleep(70);
	} else {
		gpio_set_value(GPIO_WLAN_EN, 0);
	}

	return 0;
}

static void __init ti814x_wl12xx_wlan_init(void)
{
	struct device *dev;
	struct omap_mmc_platform_data *pdata;
	int ret;

	/* Set up mmc0 muxes */
	omap_mux_init_signal("mmc0_clk", TI814X_INPUT_EN | TI814X_PULL_UP);
	omap_mux_init_signal("mmc0_cmd", TI814X_INPUT_EN | TI814X_PULL_UP);
	omap_mux_init_signal("mmc0_dat0", TI814X_INPUT_EN | TI814X_PULL_UP);
	omap_mux_init_signal("mmc0_dat1", TI814X_INPUT_EN | TI814X_PULL_UP);
	omap_mux_init_signal("mmc0_dat2", TI814X_INPUT_EN | TI814X_PULL_UP);
	omap_mux_init_signal("mmc0_dat3", TI814X_INPUT_EN | TI814X_PULL_UP);

        /* Set up the WLAN_EN and WLAN_IRQ muxes */
        //gpio1_15_mux1 is good for application daughter board
        omap_mux_init_signal("gpio2_22", TI814X_PULL_DIS);
        omap_mux_init_signal("gpio2_24", TI814X_INPUT_EN | TI814X_PULL_DIS);
	/* Pass the wl12xx platform data information to the wl12xx driver */
	if (wl12xx_set_platform_data(&wlan_data)) {
		pr_err("Error setting wl12xx data\n");
		return;
	}

	/*
	 * The WLAN_EN gpio has to be toggled without using a fixed regulator,
	 * as the omap_hsmmc does not enable/disable regulators on the TI814X.
	 */
	ret = gpio_request_one(GPIO_WLAN_EN, GPIOF_OUT_INIT_LOW, "wlan_en");
	if (ret) {
		pr_err("Error requesting wlan enable gpio: %d\n", ret);
		return;
	}

	/*
	 * Set our set_power callback function which will be called from
	 * set_ios. This is requireq since, unlike other omap2+ platforms, a
	 * no-op set_power function is registered. Thus, we cannot use a fixed
	 * regulator, as it will never be toggled.
	 * Moreover, even if this was not the case, we're on mmc0, for which
	 * omap_hsmmc' set_power functions do not toggle any regulators.
	 * TODO: Consider modifying omap_hsmmc so it would enable/disable a
	 * regulator for ti814x/mmc0.
	 */
	dev = mmc[0].dev;
	if (!dev) {
		pr_err("wl12xx mmc device initialization failed\n");
		return;
	}

	pdata = dev->platform_data;
	if (!pdata) {
		pr_err("Platform data of wl12xx device not set\n");
		return;
	}

	pdata->slots[0].set_power = wl12xx_set_power;
}

static void __init ti814x_wl12xx_init(void)
{
	ti814x_wl12xx_wlan_init();
}

#else /* CONFIG_WL12XX_PLATFORM_DATA */

static void __init ti814x_wl12xx_init(void) { }

#endif

static struct gpioc_gpio ti814x_gpios[] = {
    {//DO_0
        .gpio       = 43,
        .initflags  = GPIOF_OUT_INIT_LOW,
    },
/*
    {// PWM
		.gpio       = 49,
		.initflags  = GPIOF_OUT_INIT_LOW,
    },
*/
/*
    {//IRIS_PWM
		.gpio       = 50,
		.initflags  = GPIOF_OUT_INIT_LOW,
    },
*/
/*
    {//I2C1_SDA
        .gpio       = 51,
        .initflags  = GPIOF_OUT_INIT_LOW,
    },
    {//I2C1_SCL
        .gpio       = 52,
        .initflags  = GPIOF_OUT_INIT_LOW,
    },
*/

    {//IR_LED
        .gpio       = 59,
        .initflags  = GPIOF_OUT_INIT_LOW,
    },

	{//Zoom_Out
		.gpio		= 70,
		.initflags	= GPIOF_IN,
	},
    {// DI_0
        .gpio       = 71,
        .initflags  = GPIOF_IN,
    },
    {//LDEC (video-out detect)
        .gpio       = 72,
        .initflags  = GPIOF_IN,
    },
	{// CMOS_RST
		.gpio		= 82,
		.initflags	= GPIOF_OUT_INIT_HIGH,
	},
/*
    {//I2C0_SCL
        .gpio       = 86,
        .initflags  = GPIOF_OUT_INIT_LOW,
    },
*/
    {//MIC_SW
        .gpio       = 87,
        .initflags  = GPIOF_IN,
    },
/*
    {//I2C0_SDA
        .gpio       = 88,
        .initflags  = GPIOF_OUT_INIT_LOW,
    },
*/
    {//NTSC_PAL
        .gpio       = 89,
        .initflags  = GPIOF_IN,
    },
    {//HW_RST
	    .gpio       = 91,
        .initflags  = GPIOF_IN,
    },
    {//SOFT_RESET ??? IR_LED1
        .gpio       = 93,
        .initflags  = GPIOF_OUT_INIT_HIGH,
    },
/*
    {// IR_LED2 ????
        .gpio       = 94,
        .initflags  = GPIOF_OUT_INIT_LOW,
    },
*/
    /* Back focus position boundary switch */
    //{
    //  .gpio       = 101,
    //  .initflags  = GPIOF_IN,
    //},
#if 1
    {// LED_G
        .gpio       = 103,
        .initflags  = GPIOF_OUT_INIT_LOW,
    },
    {// LED_R
        .gpio       = 104,
        .initflags  = GPIOF_OUT_INIT_LOW,
    },
    {// FOCUS_AST
        .gpio       = 105,
        .initflags  = GPIOF_IN,
    },
/*
    {//PRI_SENSOR
        .gpio       = 106,
        .initflags  = GPIOF_IN,
    },
*/
    {//AT_AF-Det
        .gpio       = 107,
        .initflags  = GPIOF_IN,
    },
    {//ICR (IRCUT)
        .gpio       = 108,
        .initflags  = GPIOF_OUT_INIT_LOW,
    },
    {//ICR_En (IRCUT_ENABLE)
        .gpio       = 109,
        .initflags  = GPIOF_OUT_INIT_LOW,
    },
    {// Zoom_In
        .gpio       = 116,
        .initflags  = GPIOF_IN,
    },
    {//Heater_HL_Soc
        .gpio       = 117,
        .initflags  = GPIOF_OUT_INIT_LOW,
    },
    {// AC-DC_Det
        .gpio       = 118,
        .initflags  = GPIOF_IN,
    }
#endif
#if 0	
	{
		.gpio       = 16,
		.initflags  = GPIOF_OUT_INIT_LOW,
	},
	{
		.gpio       = 17,
		.initflags  = GPIOF_OUT_INIT_LOW,
	},
	{
		.gpio       = 18,
		.initflags  = GPIOF_IN,
	},
	{
		.gpio       = 19,
		.initflags  = GPIOF_IN,
	},
	{
		.gpio       = 20,
		.initflags  = GPIOF_IN,
	},
	{
		.gpio       = 22,
		.initflags  = GPIOF_IN,
	},
	{
		.gpio       = 23,
		.initflags  = GPIOF_IN,
	},
	{
		.gpio       = 24,
		.initflags  = GPIOF_IN,
	},
	{
		.gpio       = 25,
		.initflags  = GPIOF_OUT_INIT_LOW,
	},
	{
		.gpio       = 26,
		.initflags  = GPIOF_OUT_INIT_LOW,
	},
	{
		.gpio       = 43,
		.initflags  = GPIOF_OUT_INIT_LOW,
	},
/*    {   
       .gpio       = 44,
       .initflags  = GPIOF_OUT_INIT_LOW,
	},
*/	{
		.gpio		= 46,
		.initflags	= GPIOF_OUT_INIT_HIGH,
	},
	{
		.gpio		= 65,
		.initflags	= GPIOF_OUT_INIT_HIGH,
	},
	{
		.gpio		= 75,
		.initflags	= GPIOF_OUT_INIT_HIGH,
	},
	/*{
		.gpio       = 85,
		.initflags  = GPIOF_OUT_INIT_LOW,
	},
	*/
	{
		.gpio       = 93,
		.initflags  = GPIOF_OUT_INIT_LOW,
	},
	{
		.gpio       = 94,
		.initflags  = GPIOF_OUT_INIT_LOW,
	},
	{
		.gpio		= 96,
		.initflags	= GPIOF_IN,
	},
	{
		.gpio		= 97,
		.initflags	= GPIOF_IN,
	},
	/* Back focus position boundary switch */
	{
		.gpio       = 101,
		.initflags  = GPIOF_IN,
	},
	{
		.gpio		= 112,
		.initflags	= GPIOF_OUT_INIT_LOW,
	},
	{
		.gpio		= 113,
		.initflags	= GPIOF_IN,
	},
#endif
};


static struct gpioc_export ti814x_gpioc_export = {
	.gpios  = ti814x_gpios,
	.ngpios = ARRAY_SIZE(ti814x_gpios),
	/* We assume all GPIOs use the same flags */
	.irqflags  = IRQF_TRIGGER_FALLING,
};

static struct platform_device ti814x_gpioc_device = {
	.name   = "gpioc",
	.dev    = {
		.platform_data = &ti814x_gpioc_export,
	},
};

static void __init ti814x_gpioc_init(void)
{
	if (platform_device_register(&ti814x_gpioc_device))
		printk(KERN_ERR "Could not register TI814x GPIOC device\n");
	else
		printk(KERN_INFO "registered TI814x GPIOC device\n");
}


static struct a3967_platdata ti814x_bf_motor_platdata = {
	.gpio_reset		= 73,
	.gpio_enable	= 74,
	.gpio_dir		= 76,
	.gpio_step		= 75,
	.gpio_ms1		= -1,
	.gpio_ms2		= -1,
	.gpio_sleep		= -1,
	.pulse_per_step		= 1,
	.clockwise_forward	= 0,
};


static struct platform_device ti814x_bf_motor_device = {
	.name   = "a3967",
	.id		= 0,
	.dev    = {
		.platform_data = &ti814x_bf_motor_platdata,
	},
};

static void __init ti814x_bf_motor_init(void)
{
	if (platform_device_register(&ti814x_bf_motor_device))
		printk(KERN_ERR "Could not register A3967 device\n");
	else
		printk(KERN_INFO "registered A3967 device\n");
}

static struct md124_platdata ti814x_piris_motor_platdata = {
	.gpio_enable            = 111,
	.gpio_in1               = 112,
	.gpio_in2               = 113,
	.pulse_per_step         = 1,
};

static struct platform_device ti814x_piris_motor_device = {
	.name   = "piris-motor",
	.id     = 0,
	.dev    = {
		.platform_data = &ti814x_piris_motor_platdata,
	},
};

static void __init ti814x_piris_motor_init(void)
{
	if (platform_device_register(&ti814x_piris_motor_device))
		printk(KERN_ERR "Could not register MD124 device\n");
	else
		printk(KERN_INFO "registered MD124 device\n");
}

static struct drv8836_platdata motor_data[] = {
   {
        .label                  = "zoom",
        .gpio_Ain1              = 97,
        .gpio_Ain2              = 98,
        .gpio_Bin1              = 96,
        .gpio_Bin2              = 126,
        .pulse_per_step         = 1,
   },
   {
        .label                  = "focus",
        .gpio_Ain1              = 100,
        .gpio_Ain2              = 101,
        .gpio_Bin1              = 99,
        .gpio_Bin2              = 102,
        .pulse_per_step         = 1,
   }
};

static struct drv8836_platform_data ti814x_motor_platdata = {
	.num_motors   = ARRAY_SIZE(motor_data),
	.data = &motor_data,
};

static struct platform_device ti814x_motor_device = {
	.name   = "drv8836",
	.id     = 0,
	.dev    = {
		.platform_data = &ti814x_motor_platdata,
	},
};

static void __init ti814x_zoom_motor_init(void)
{
        if (platform_device_register(&ti814x_motor_device))
                printk(KERN_ERR "Could not register DRV8836 device\n");
        else
                printk(KERN_INFO "registered DRV8836 device\n");
}

static struct ramoops_platform_data ti814x_ramoops_pdata = {
	.mem_size = 0x00100000,
	.mem_address = 0x80000000 + 143*1024*1024,
};

static struct platform_device ramoops_device = {
	.name = "ramoops",
	.id = -1,
	.dev = {
		.platform_data = &ti814x_ramoops_pdata,
	},
};

static void __init dm385_evm_init(void)
{
	int bw; /* bus-width */

	ti814x_mux_init(board_mux);
	omap_serial_init();
	//penlin
	//ti814x_evm_i2c_init();
	ti814x_ipnc_i2c_gpio_init();
	ti81xx_register_mcasp(0, &dm385_evm_snd_data);
	platform_device_register(&power_leds);
	platform_device_register(&reset_btn);

	struct ramoops_platform_data *ramoops_pdata =
		(struct ramoops_platform_data*) ramoops_device.dev.platform_data;
	memblock_reserve((phys_addr_t)ramoops_pdata->mem_address,
					 (phys_addr_t)ramoops_pdata->mem_size);
	platform_device_register(&ramoops_device);

	omap2_hsmmc_init(mmc);

	/* nand initialisation */
		board_nand_init(ti814x_nand_partitions,
			ARRAY_SIZE(ti814x_nand_partitions), 0, NAND_OMAP_BUS_16);

#ifdef CONFIG_USB_SUPPORT
	/* initialize usb */
	//usb_musb_init(&musb_board_data);
#endif

#ifdef CONFIG_SND_SOC_TI81XX_HDMI
	/* hdmi mclk setup */
	ti813x_hdmi_clk_init();
	platform_add_devices(dm385_devices, ARRAY_SIZE(dm385_devices));
#endif
	//penlin
	//ti814x_wl12xx_init();
	regulator_use_dummy_regulator();
#ifdef CONFIG_HAVE_PWM
	omap_register_pwm_config(dm385_ipnc_pwm_cfg, ARRAY_SIZE(dm385_ipnc_pwm_cfg));
#endif

	ti814x_gpioc_init();
	ti814x_zoom_motor_init();
	ti814x_piris_motor_init();
}

static void __init dm385_evm_map_io(void)
{
	omap2_set_globals_ti816x();
	ti81xx_map_common_io();
}

MACHINE_START(DM385IPNC, "dm385ipnc")
	/* Maintainer: Texas Instruments */
	.boot_params	= 0x80000100,
	.map_io		= dm385_evm_map_io,
	.reserve         = ti81xx_reserve,
	.init_irq	= dm385_evm_init_irq,
	.init_machine	= dm385_evm_init,
	.timer		= &omap_timer,
MACHINE_END
