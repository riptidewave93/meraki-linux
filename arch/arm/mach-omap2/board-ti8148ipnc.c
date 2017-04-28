/*
 * Code for TI8148 EVM.
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
#include <linux/regulator/machine.h>
#include <linux/mfd/tps65910.h>
#include <linux/wl12xx.h>
#include <linux/clk.h>
#include <linux/err.h>

#include <mach/hardware.h>
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
#ifdef CONFIG_SND_SOC_TLV320AIC3X
#include <sound/tlv320aic3x.h>
#endif
#include "board-flash.h"
#include "clock.h"
#include "mux.h"
#include "hsmmc.h"
#include "control.h"
#include "cm81xx.h"

#ifdef CONFIG_OMAP_MUX
static struct omap_board_mux board_mux[] __initdata = {
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
		.mmc		= 1,
#endif
		.caps		= MMC_CAP_4_BIT_DATA,
               .gpio_cd        = 79,/* Dedicated pins for CD and WP */
               .gpio_wp        = 80,
               .ocr_mask       = MMC_VDD_33_34,
       },
       {}      /* Terminator */
};
#if 0
static struct at24_platform_data eeprom_info = {
	.byte_len       = (256*1024) / 8,
	.page_size      = 64,
	.flags          = AT24_FLAG_ADDR16,
};
#endif
#ifdef CONFIG_SND_SOC_TLV320AIC3X
static struct aic3x_pdata dm8148ipnc_aic3x_data = {
	.gpio_reset = 8,
};
#endif

static struct regulator_consumer_supply ti8148ipnc_mpu_supply =
	REGULATOR_SUPPLY("mpu", NULL);

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
		.consumer_supplies	= &ti8148ipnc_mpu_supply,
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

static struct i2c_board_info __initdata ti814x_i2c_boardinfo[] = {
#ifdef CONFIG_SND_SOC_TLV320AIC3X
	{
		I2C_BOARD_INFO("tlv320aic3104", 0x18),
		.platform_data = &dm8148ipnc_aic3x_data,
	},
#endif
	{
		I2C_BOARD_INFO("tps65911", 0x2D),
		.platform_data = &tps65911_pdata,
	},
};

static void __init ti814x_evm_i2c_init(void)
{
	/* There are 4 instances of I2C in TI814X but currently only one
	 * instance is being used on the TI8148 EVM
	 */
#ifndef CONFIG_TI8148_EVM_OPTIMIZED
	omap_register_i2c_bus(1, 100, ti814x_i2c_boardinfo,
				ARRAY_SIZE(ti814x_i2c_boardinfo));
#else
	omap_register_i2c_bus(1, 100, ti814x_i2c_boardinfo,
				ARRAY_SIZE(ti814x_i2c_boardinfo));
#endif
}

static u8 ti8148_iis_serializer_direction[] = {
	TX_MODE,	RX_MODE,	INACTIVE_MODE,	INACTIVE_MODE,
	INACTIVE_MODE,	INACTIVE_MODE,	INACTIVE_MODE,	INACTIVE_MODE,
	INACTIVE_MODE,	INACTIVE_MODE,	INACTIVE_MODE,	INACTIVE_MODE,
	INACTIVE_MODE,	INACTIVE_MODE,	INACTIVE_MODE,	INACTIVE_MODE,
};

static struct snd_platform_data ti8148_evm_snd_data = {
	.tx_dma_offset	= 0x46800000,
	.rx_dma_offset	= 0x46800000,
	.op_mode	= DAVINCI_MCASP_IIS_MODE,
	.num_serializer = ARRAY_SIZE(ti8148_iis_serializer_direction),
	.tdm_slots	= 2,
	.serial_dir	= ti8148_iis_serializer_direction,
	.asp_chan_q	= EVENTQ_2,
	.version	= MCASP_VERSION_2,
	.txnumevt	= 64,
	.rxnumevt	= 64,
};


/* NAND flash information */
static struct mtd_partition ti814x_nand_partitions[] = {
/* All the partition sizes are listed in terms of NAND block size */
	{
		.name           = "U-Boot-min",
		.offset         = 0,    /* Offset = 0x0 */
#ifdef CONFIG_MTD_NAND_OMAP_ECC_BCH16_CODE_HW
		.size           = 1 * SZ_256K,
#else
		.size           = SZ_128K,
#endif
	},
	{
		.name           = "U-Boot",
		.offset         = MTDPART_OFS_APPEND,/* Offset = 0x0 + 128K */
#ifdef CONFIG_MTD_NAND_OMAP_ECC_BCH16_CODE_HW
		.size           = 8 * SZ_256K,
#else
		.size           = 18 * SZ_128K,
#endif
	},
	{
		.name           = "U-Boot Env",
		.offset         = MTDPART_OFS_APPEND,   /* Offset = 0x260000 */
#ifdef CONFIG_MTD_NAND_OMAP_ECC_BCH16_CODE_HW
		.size           = 1 * SZ_256K,
#else
		.size           = 1 * SZ_128K,
#endif
	},
	{
		.name           = "Kernel",
		.offset         = MTDPART_OFS_APPEND,   /* Offset = 0x280000 */
#ifdef CONFIG_MTD_NAND_OMAP_ECC_BCH16_CODE_HW
		.size           = 17 * SZ_256K,
#else
		.size           = 34 * SZ_128K,
#endif
	},
	{
		.name           = "File System",
		.offset         = MTDPART_OFS_APPEND,   /* Offset = 0x6C0000 */
#ifdef CONFIG_MTD_NAND_OMAP_ECC_BCH16_CODE_HW
		.size           = 420 * SZ_256K,
#else
		.size           = 840 * SZ_128K,
#endif
	},
	{
		.name           = "Data",
		.offset         = MTDPART_OFS_APPEND,   /* Offset = 0x55C0000 */
#ifdef CONFIG_MTD_NAND_OMAP_ECC_BCH16_CODE_HW
		.size           = 48 * SZ_256K,
#else
		.size           = 96 * SZ_128K,
#endif
	},
	{
		.name           = "File System2",
		.offset         = MTDPART_OFS_APPEND,   /* Offset = 0x61C0000 */
#ifdef CONFIG_MTD_NAND_OMAP_ECC_BCH16_CODE_HW
		.size           = 212 * SZ_256K,
#else
		.size           = 424 * SZ_128K,
#endif
	},
	{
		.name           = "Reserved",
		.offset         = MTDPART_OFS_APPEND,   /* Offset = 0xB0C0000 */
		.size           = MTDPART_SIZ_FULL,
	},
};

struct spi_board_info __initdata ti8148_spi_slave_info[] = {
	{
		.modalias	= "tlv320aic26-codec",
		.irq		= -1,
		.max_speed_hz	= 100000,
		.bus_num	= 1,
		.chip_select	= 0,
		.mode = SPI_MODE_1,
	},
};

void __init ti8148_spi_init(void)
{
	spi_register_board_info(ti8148_spi_slave_info,
				ARRAY_SIZE(ti8148_spi_slave_info));
}

static struct omap_musb_board_data musb_board_data = {
	.interface_type		= MUSB_INTERFACE_ULPI,
#ifdef CONFIG_USB_MUSB_OTG
	.mode           = MUSB_OTG,
#elif defined(CONFIG_USB_MUSB_HDRC_HCD)
	.mode           = MUSB_HOST,
#elif defined(CONFIG_USB_GADGET_MUSB_HDRC)
	.mode           = MUSB_PERIPHERAL,
#endif
	.power		= 500,
	.instances	= 1,
};

#ifdef CONFIG_HAVE_PWM
struct omap2_pwm_platform_config ti8148_ipnc_pwm_cfg[] ={
	{
		.timer_id = 6,
		.polarity = 1,
	},
	{
		.timer_id = 7,
		.polarity = 1,
	}
};
#endif

static void __init ti8148_ipnc_init_irq(void)
{
	omap2_init_common_infrastructure();
	omap2_init_common_devices(NULL, NULL);
	omap_init_irq();
	gpmc_init();
}


#ifdef CONFIG_SND_SOC_TI81XX_HDMI
static struct snd_hdmi_platform_data ti8148_snd_hdmi_pdata = {
	.dma_addr = TI81xx_HDMI_WP + HDMI_WP_AUDIO_DATA,
	.channel = 53,
	.dma_chan_q = EVENTQ_0,
	.data_type = 4,
	.acnt = 4,
	.fifo_level = 0x20,
};

static struct platform_device ti8148_hdmi_audio_device = {
	.name   = "hdmi-dai",
	.id     = -1,
	.dev = {
		.platform_data = &ti8148_snd_hdmi_pdata,
	}
};

static struct platform_device ti8148_hdmi_codec_device = {
	.name   = "hdmi-dummy-codec",
	.id     = -1,
};

static struct platform_device *ti8148_devices[] __initdata = {
	&ti8148_hdmi_audio_device,
	&ti8148_hdmi_codec_device,
};

/*
 * HDMI Audio Auto CTS MCLK configuration.
 * sysclk20, sysclk21, sysclk21 and CLKS(external)
 * setting sysclk20 as the parent of hdmi_i2s_ck
 * ToDo:
 */
void __init ti8148_hdmi_clk_init(void)
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

#define LSI_PHY_ID		0x0282F014
#define LSI_PHY_MASK		0xffffffff
#define PHY_CONFIG_REG		22

static int ti8148_evm_lsi_phy_fixup(struct phy_device *phydev)
{
	unsigned int val;

	/* This enables TX_CLK-ing in case of 10/100MBps operation */
	val = phy_read(phydev, PHY_CONFIG_REG);
	phy_write(phydev, PHY_CONFIG_REG, (val | BIT(5)));

	return 0;
}

static int setup_mmc2_pin_mux(void)
{
	return omap_mux_init_signal("spi0_cs1.gpio1_6",
				TI814X_PIN_INPUT_PULL_UP);
}

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

static void __init ti8148_ipnc_init(void)
{

	ti814x_mux_init(board_mux);
	omap_serial_init();
	ti814x_evm_i2c_init();
	ti81xx_register_mcasp(0, &ti8148_evm_snd_data);

	setup_mmc2_pin_mux();
	omap2_hsmmc_init(mmc);

	/* nand initialisation */
	board_nand_init(ti814x_nand_partitions,
		ARRAY_SIZE(ti814x_nand_partitions), 0, NAND_OMAP_BUS_16);

	/* initialize usb */
	usb_musb_init(&musb_board_data);

	ti8148_spi_init();
#ifdef CONFIG_SND_SOC_TI81XX_HDMI
	/*setup the clokc for HDMI MCLK*/
	ti8148_hdmi_clk_init();
	__raw_writel(0x0, DSS_HDMI_RESET);
	platform_add_devices(ti8148_devices, ARRAY_SIZE(ti8148_devices));
#endif
	ti814x_wl12xx_init();
	regulator_use_dummy_regulator();
#ifdef CONFIG_HAVE_PWM
	omap_register_pwm_config(ti8148_ipnc_pwm_cfg, ARRAY_SIZE(ti8148_ipnc_pwm_cfg));
#endif

	/* LSI Gigabit Phy fixup */
	phy_register_fixup_for_uid(LSI_PHY_ID, LSI_PHY_MASK,
				   ti8148_evm_lsi_phy_fixup);
}

static void __init ti8148_ipnc_map_io(void)
{
	omap2_set_globals_ti814x();
	ti81xx_map_common_io();
}

MACHINE_START(TI8148IPNC, "ti8148ipnc")
	/* Maintainer: Texas Instruments */
	.boot_params	= 0x80000100,
	.map_io		= ti8148_ipnc_map_io,
	.reserve         = ti81xx_reserve,
	.init_irq	= ti8148_ipnc_init_irq,
	.init_machine	= ti8148_ipnc_init,
	.timer		= &omap_timer,
MACHINE_END
