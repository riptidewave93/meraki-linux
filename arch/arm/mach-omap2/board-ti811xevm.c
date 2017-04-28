/*
 * Code for TI811X EVM.
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
#include <linux/i2c/at24.h>
#include <linux/i2c/qt602240_ts.h>
#include <linux/i2c/pcf857x.h>
#include <linux/regulator/machine.h>
#include <linux/mfd/tps65910.h>
#include <linux/sii9022a.h>

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

#include "board-flash.h"
#include "clock.h"
#include "mux.h"
#include "hsmmc.h"
#include "control.h"
#include "cm81xx.h"

#define GPIO_TSC               31
/* Convert GPIO signal to GPIO pin number */
#define GPIO_TO_PIN(bank, gpio) (32 * (bank) + (gpio))

#ifdef CONFIG_OMAP_MUX
static struct omap_board_mux board_mux[] __initdata = {
	{ .reg_offset = OMAP_MUX_TERMINATOR },
};
#else
#define board_mux     NULL
#endif

static struct omap2_hsmmc_info mmc[] = {
	{
		.mmc		= 1,
		.caps		= MMC_CAP_4_BIT_DATA,
		.gpio_cd	= GPIO_TO_PIN(1, 6), /* Dedicated pins for CD and WP */
		.gpio_wp	= GPIO_TO_PIN(1, 3),
		.ocr_mask	= MMC_VDD_33_34,
	},
	{}	/* Terminator */
};

/* Touchscreen platform data */
static struct qt602240_platform_data ts_platform_data = {
	.x_line		= 18,
	.y_line		= 12,
	.x_size		= 800,
	.y_size		= 480,
	.blen		= 0x01,
	.threshold	= 30,
	.voltage	= 2800000,
	.orient		= QT602240_HORIZONTAL_FLIP,
};

static struct at24_platform_data eeprom_info = {
	.byte_len       = (256*1024) / 8,
	.page_size      = 64,
	.flags          = AT24_FLAG_ADDR16,
};

static struct regulator_consumer_supply ti811xevm_mpu_supply =
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
		.consumer_supplies	= &ti811xevm_mpu_supply,
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




#define GPIO_LCD_PWR_DOWN       13
#define GPIO_LCD_SELECT         14

static int setup_gpio_ioexp(struct i2c_client *client, int gpio_base,
	unsigned ngpio, void *context) {
	int ret = 0;
	int gpio = gpio_base + GPIO_LCD_PWR_DOWN;
	ret = gpio_request(gpio, "lcd_power");

	if (ret) {
		printk(KERN_ERR "%s: failed to request GPIO for LCD Power"
			": %d\n", __func__, ret);
	return ret;
	}
	gpio_export(gpio, true);
	gpio_direction_output(gpio, 0);

	gpio = gpio_base + GPIO_LCD_SELECT;
	ret = gpio_request(gpio, "lcd_select");

	if (ret) {
		printk(KERN_ERR "%s: failed to request GPIO for LCD Select"
			": %d\n", __func__, ret);
	return ret;
	}
	gpio_export(gpio, true);
	gpio_direction_output(gpio, 1);
	return 0;
}

static struct pcf857x_platform_data io_expander_data = {
	.gpio_base      = 6 * 32,
	.setup          = setup_gpio_ioexp,
};


static struct sii9022a_platform_data sii9022a_pdata = {
	.hdmi_hot_plug_gpio_intr_line	= 0,
	.sync_mode			= 0,
	.clk_edge			= 1,
};


static struct i2c_board_info __initdata ti814x_i2c_boardinfo[] = {
	{
		I2C_BOARD_INFO("eeprom", 0x50),
		.platform_data	= &eeprom_info,
	},
	{
		I2C_BOARD_INFO("cpld", 0x23),
	},
	{
		I2C_BOARD_INFO("tlv320aic3x", 0x18),
	},
	{
		I2C_BOARD_INFO("pcf8575", 0x20),
		.platform_data = &io_expander_data,
	},
	{
		I2C_BOARD_INFO("tlc59108", 0x40),
	},
	{
		I2C_BOARD_INFO("qt602240_ts", 0x4A),
		.platform_data = &ts_platform_data,
	},
	{
		I2C_BOARD_INFO("tps65911", 0x2D),
		.platform_data = &tps65911_pdata,
	},
};

static struct i2c_board_info __initdata ti811x_i2c_boardinfo1[] = {
	{
		I2C_BOARD_INFO("sii9022a", 0x39),
		.platform_data = &sii9022a_pdata,
	},
};

static const struct i2c_device_id pcf8575_video_id[] = {
	{ "pcf8575_1_ti811x", 0 },
	{ }
};
static struct i2c_client *pcf8575_1_client;
static unsigned char pcf8575_1_port[2] = {0x4F, 0x7F};

#define VPS_PCF8575_PIN0                (0x10)
#define VPS_PCF8575_PIN1                (0x20)
#define VPS_PCF8575_PIN2                (0x4)
#define VPS_PCF8575_PIN3                (0x8)
#define VPS_PCF8575_PIN4                (0x2)
#define VPS_PCF8575_PIN5                (0x1)
#define VPS_PCF8575_PIN6                (0x40)
#define VPS_PCF8575_PIN7                (0x80)

#define VPS_PCF8575_PIN10               (0x1)
#define VPS_PCF8575_PIN11               (0x2)

#define VPS_PCF8575_LCD_PWR_DW          (0x20)
#define VPS_THS7375_MASK                (VPS_PCF8575_PIN10 | VPS_PCF8575_PIN11)

#define VPS_THS7360_SD_MASK             (VPS_PCF8575_PIN2 | VPS_PCF8575_PIN5)

#define VPS_THS7360_SF_MASK             (VPS_PCF8575_PIN0 |                    \
					VPS_PCF8575_PIN1 |                    \
					VPS_PCF8575_PIN3 |                    \
					VPS_PCF8575_PIN4)
int ti811x_pcf8575_ths7360_sd_enable(enum ti81xx_ths_filter_ctrl ctrl)
{
	struct i2c_msg msg = {
		.addr = pcf8575_1_client->addr,
		.flags = 0,
		.len = 2,
	};
	pcf8575_1_port[0] &= ~VPS_THS7360_SD_MASK;
	switch (ctrl) {
	case TI81XX_THSFILTER_ENABLE_MODULE:
		pcf8575_1_port[0] &= ~(VPS_THS7360_SD_MASK);
		break;
	case TI81XX_THSFILTER_BYPASS_MODULE:
		pcf8575_1_port[0] |= VPS_PCF8575_PIN2;
		break;
	case TI81XX_THSFILTER_DISABLE_MODULE:
		pcf8575_1_port[0] |= VPS_THS7360_SD_MASK;
		break;
	default:
		return -EINVAL;
	}

	msg.buf = pcf8575_1_port;
	return i2c_transfer(pcf8575_1_client->adapter, &msg, 1);
}

int ti811x_pcf8575_ths7360_hd_enable(enum ti81xx_ths7360_sf_ctrl ctrl)
{
	int ret_val;
	struct i2c_msg msg = {
		.addr = pcf8575_1_client->addr,
		.flags = 0,
		.len = 2,
	};

	pcf8575_1_port[0] &= ~VPS_THS7360_SF_MASK;
	switch (ctrl) {
	case TI81XX_THS7360_DISABLE_SF:
		pcf8575_1_port[0] |= VPS_PCF8575_PIN4;
		break;
	case TI81XX_THS7360_BYPASS_SF:
		pcf8575_1_port[0] |= VPS_PCF8575_PIN3;
		break;
	case TI81XX_THS7360_SF_SD_MODE:
		pcf8575_1_port[0] &= ~(VPS_THS7360_SF_MASK);
		break;
	case TI81XX_THS7360_SF_ED_MODE:
		pcf8575_1_port[0] |= VPS_PCF8575_PIN0;
		break;
	case TI81XX_THS7360_SF_HD_MODE:
		pcf8575_1_port[0] |= VPS_PCF8575_PIN1;
		break;
	case TI81XX_THS7360_SF_TRUE_HD_MODE:
		pcf8575_1_port[0] |= VPS_PCF8575_PIN0|VPS_PCF8575_PIN1;
		break;
	default:
		return -EINVAL;
	}
	msg.buf = pcf8575_1_port;

	ret_val = i2c_transfer(pcf8575_1_client->adapter, &msg, 1);
	return ret_val;

}
static int pcf8575_video_probe(struct i2c_client *client,
				const struct i2c_device_id *id)
{
	pcf8575_1_client = client;
	return 0;
}

static int __devexit pcf8575_video_remove(struct i2c_client *client)
{
	pcf8575_1_client = NULL;
	return 0;
}

static struct i2c_driver pcf8575_driver = {
	.driver = {
		.name   = "pcf8575_1_ti811x",
	},
	.probe          = pcf8575_video_probe,
	.remove         = pcf8575_video_remove,
	.id_table       = pcf8575_video_id,
};

int ti811x_pcf8575_init(void)
{
	i2c_add_driver(&pcf8575_driver);
	return 0;
}

int ti811x_pcf8575_exit(void)
{
	i2c_del_driver(&pcf8575_driver);
	return 0;
}

/* The following four functions are stubs - Added to avoid compilation
 error, refer CQ SDOCM00094243 for more details, Need to remove this code once the
 CQ is fixed */
int ti814x_pcf8575_init(void)
{
	return 0;
}
int ti814x_pcf8575_exit(void)
{
	return 0;
}
int vps_ti814x_select_video_decoder(int vid_decoder_id)
{
	return 0;
}
int vps_ti814x_set_tvp7002_filter(enum fvid2_standard standard)
{
	return 0;
}
/* Stubs Ends*/

static void __init ti814x_tsc_init(void)
{
	int error;

	omap_mux_init_signal("mlb_clk.gpio0_31", TI814X_PULL_DIS | (1 << 18));

	error = gpio_request(GPIO_TSC, "ts_irq");
	if (error < 0) {
		printk(KERN_ERR "%s: failed to request GPIO for TSC IRQ"
			": %d\n", __func__, error);
		return;
	}

	gpio_direction_input(GPIO_TSC);
	ti814x_i2c_boardinfo[5].irq = gpio_to_irq(GPIO_TSC);

	gpio_export(31, true);
}

static void __init ti814x_evm_i2c_init(void)
{
	/* There are 4 instances of I2C in TI814X but currently only one
	 * instance is being used on the TI8148 EVM
	 */
	omap_register_i2c_bus(1, 100, ti814x_i2c_boardinfo,
				ARRAY_SIZE(ti814x_i2c_boardinfo));
	omap_register_i2c_bus(3, 100, ti811x_i2c_boardinfo1,
				ARRAY_SIZE(ti811x_i2c_boardinfo1));

}

static u8 ti811x_iis_serializer_direction[] = {
	TX_MODE,	RX_MODE,	INACTIVE_MODE,	INACTIVE_MODE,
	INACTIVE_MODE,	INACTIVE_MODE,	INACTIVE_MODE,	INACTIVE_MODE,
	INACTIVE_MODE,	INACTIVE_MODE,	INACTIVE_MODE,	INACTIVE_MODE,
	INACTIVE_MODE,	INACTIVE_MODE,	INACTIVE_MODE,	INACTIVE_MODE,
};

static struct snd_platform_data ti811x_evm_snd_data = {
	.tx_dma_offset	= 0x46800000,
	.rx_dma_offset	= 0x46800000,
	.op_mode	= DAVINCI_MCASP_IIS_MODE,
	.num_serializer = ARRAY_SIZE(ti811x_iis_serializer_direction),
	.tdm_slots	= 2,
	.serial_dir	= ti811x_iis_serializer_direction,
	.asp_chan_q	= EVENTQ_2,
	.version	= MCASP_VERSION_2,
	.txnumevt	= 1,
	.rxnumevt	= 1,
	/* McASP2_AHCLKX out to feed CODEC CLK*/
	.clk_input_pin	= MCASP_AHCLKX_OUT,
};

/* NOR Flash partitions */
static struct mtd_partition ti814x_evm_norflash_partitions[] = {
	/* bootloader (U-Boot, etc) in first 5 sectors */
	{
		.name		= "bootloader",
		.offset		= 0,
		.size		= 2 * SZ_128K,
		.mask_flags	= MTD_WRITEABLE, /* force read-only */
	},
	/* bootloader params in the next 1 sectors */
	{
		.name		= "env",
		.offset		= MTDPART_OFS_APPEND,
		.size		= SZ_128K,
		.mask_flags	= 0,
	},
	/* kernel */
	{
		.name		= "kernel",
		.offset		= MTDPART_OFS_APPEND,
		.size		= 2 * SZ_2M,
		.mask_flags	= 0
	},
	/* file system */
	{
		.name		= "filesystem",
		.offset		= MTDPART_OFS_APPEND,
		.size		= 25 * SZ_2M,
		.mask_flags	= 0
	},
	/* reserved */
	{
		.name		= "reserved",
		.offset		= MTDPART_OFS_APPEND,
		.size		= MTDPART_SIZ_FULL,
		.mask_flags	= 0
	}
};

/* NAND flash information */
static struct mtd_partition ti814x_nand_partitions[] = {
/* All the partition sizes are listed in terms of NAND block size */
	{
		.name           = "U-Boot-min",
		.offset         = 0,    /* Offset = 0x0 */
		.size           = SZ_128K,
	},
	{
		.name           = "U-Boot",
		.offset         = MTDPART_OFS_APPEND,/* Offset = 0x0 + 128K */
		.size           = 18 * SZ_128K,
	},
	{
		.name           = "U-Boot Env",
		.offset         = MTDPART_OFS_APPEND,   /* Offset = 0x260000 */
		.size           = 1 * SZ_128K,
	},
	{
		.name           = "Kernel",
		.offset         = MTDPART_OFS_APPEND,   /* Offset = 0x280000 */
		.size           = 34 * SZ_128K,
	},
	{
		.name           = "File System",
		.offset         = MTDPART_OFS_APPEND,   /* Offset = 0x6C0000 */
		.size           = 1601 * SZ_128K,
	},
	{
		.name           = "Reserved",
		.offset         = MTDPART_OFS_APPEND,   /* Offset = 0xCEE0000 */
		.size           = MTDPART_SIZ_FULL,
	},
};

/* SPI fLash information */
struct mtd_partition ti811x_spi_partitions[] = {
	/* All the partition sizes are listed in terms of erase size */
	{
		.name		= "U-Boot-min",
		.offset		= 0,	/* Offset = 0x0 */
		.size		= 32 * SZ_4K,
		.mask_flags	= MTD_WRITEABLE, /* force read-only */
	},
	{
		.name		= "U-Boot",
		.offset		= MTDPART_OFS_APPEND, /* 0x0 + (32*4K) */
		.size		= 64 * SZ_4K,
		.mask_flags	= MTD_WRITEABLE, /* force read-only */
	},
	{
		.name		= "U-Boot Env",
		.offset		= MTDPART_OFS_APPEND, /* 0x40000 + (32*4K) */
		.size		= 2 * SZ_4K,
	},
	{
		.name		= "Kernel",
		.offset		= MTDPART_OFS_APPEND, /* 0x42000 + (32*4K) */
		.size		= 640 * SZ_4K,
	},
	{
		.name		= "File System",
		.offset		= MTDPART_OFS_APPEND, /* 0x2C2000 + (32*4K) */
		.size		= MTDPART_SIZ_FULL, /* size ~= 1.1 MiB */
	}
};

const struct flash_platform_data ti811x_spi_flash = {
	.type		= "w25x32",
	.name		= "spi_flash",
	.parts		= ti811x_spi_partitions,
	.nr_parts	= ARRAY_SIZE(ti811x_spi_partitions),
};

struct spi_board_info __initdata ti811x_spi_slave_info[] = {
	{
		.modalias	= "m25p80",
		.platform_data	= &ti811x_spi_flash,
		.irq		= -1,
		.max_speed_hz	= 75000000,
		.bus_num	= 1,
		.chip_select	= 0,
	},
};

void __init ti811x_spi_init(void)
{
	spi_register_board_info(ti811x_spi_slave_info,
				ARRAY_SIZE(ti811x_spi_slave_info));
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

static void __init ti811x_evm_init_irq(void)
{
	omap2_init_common_infrastructure();
	omap2_init_common_devices(NULL, NULL);
	omap_init_irq();
	gpmc_init();
}

#ifdef CONFIG_SND_SOC_TI81XX_HDMI
static struct snd_hdmi_platform_data ti811x_snd_hdmi_pdata = {
	.dma_addr = TI81xx_HDMI_WP + HDMI_WP_AUDIO_DATA,
	.channel = 53,
	.data_type = 4,
	.acnt = 4,
	.fifo_level = 0x20,
};

static struct platform_device ti811x_hdmi_audio_device = {
	.name   = "hdmi-dai",
	.id     = -1,
	.dev = {
		.platform_data = &ti811x_snd_hdmi_pdata,
	}
};

static struct platform_device ti811x_hdmi_codec_device = {
	.name   = "hdmi-dummy-codec",
	.id     = -1,
};

static struct platform_device *ti811x_devices[] __initdata = {
	&ti811x_hdmi_audio_device,
	&ti811x_hdmi_codec_device,
};
#endif

static void ti811x_interrupt_xbar_config(void)
{
	u32 tmp;

	/* Configure A8 interrupt mux 95_92 to receive GPIO4/5 interrupts */
	tmp = __raw_readl(TI811X_A8_INT_MUX_95_92);
	__raw_writel(0x1F1E1D1C, TI811X_A8_INT_MUX_95_92);
	tmp = __raw_readl(TI811X_A8_INT_MUX_95_92);

}

static void __init ti811x_evm_init(void)
{
	int bw; /* bus-width */

	ti814x_mux_init(board_mux);
	omap_serial_init();
	ti814x_tsc_init();
	ti814x_evm_i2c_init();
	ti81xx_register_mcasp(0, &ti811x_evm_snd_data);

	omap2_hsmmc_init(mmc);

	/* nand initialisation */
	board_nand_init(ti814x_nand_partitions,
	ARRAY_SIZE(ti814x_nand_partitions), 0, NAND_OMAP_BUS_16);

	/* initialize usb */
	usb_musb_init(&musb_board_data);

	ti811x_spi_init();
#ifdef CONFIG_SND_SOC_TI81XX_HDMI
	platform_add_devices(ti811x_devices, ARRAY_SIZE(ti811x_devices));
#endif
	regulator_use_dummy_regulator();
	board_nor_init(ti814x_evm_norflash_partitions,
		ARRAY_SIZE(ti814x_evm_norflash_partitions), 0);

	ti811x_interrupt_xbar_config();

}

static void __init ti811x_evm_map_io(void)
{
	omap2_set_globals_ti816x();
	ti81xx_map_common_io();
}

MACHINE_START(TI811XEVM, "ti811xevm")
	/* Maintainer: Texas Instruments */
	.boot_params	= 0x80000100,
	.map_io		= ti811x_evm_map_io,
	.reserve         = ti81xx_reserve,
	.init_irq	= ti811x_evm_init_irq,
	.init_machine	= ti811x_evm_init,
	.timer		= &omap_timer,
MACHINE_END
