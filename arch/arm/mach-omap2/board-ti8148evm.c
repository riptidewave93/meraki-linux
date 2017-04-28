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
#include <linux/i2c/qt602240_ts.h>
#include <linux/i2c/pcf857x.h>
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
#ifdef CONFIG_TI8148EVM_WL12XX
/* WLAN_EN is GP1[15] */
#define GPIO_WLAN_EN	((1 * 32) + 15)
/* WLAN_IRQ is GP0[30] */
#define GPIO_WLAN_IRQ	((0 * 32) + 30)
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
		.gpio_cd        = -EINVAL, /* check setup_mmc2_pin_mux() */
		.gpio_wp	= -EINVAL,
		.ocr_mask	= MMC_VDD_33_34,
	},
	{}	/* Terminator */
};

#define GPIO_LCD_PWR_DOWN	0

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

	return 0;
}

/* IO expander data */
static struct pcf857x_platform_data io_expander_data = {
	.gpio_base	= 4 * 32,
	.setup		= setup_gpio_ioexp,
};
static struct i2c_board_info __initdata ti814x_i2c_boardinfo1[] = {
	{
		I2C_BOARD_INFO("pcf8575_1", 0x21),
	},

};

#define VPS_VC_IO_EXP_RESET_DEV_MASK        (0x0Fu)
#define VPS_VC_IO_EXP_SEL_VIN0_S1_MASK      (0x04u)
#define VPS_VC_IO_EXP_THS7368_DISABLE_MASK  (0x10u)
#define VPS_VC_IO_EXP_THS7368_BYPASS_MASK   (0x20u)
#define VPS_VC_IO_EXP_THS7368_FILTER1_MASK  (0x40u)
#define VPS_VC_IO_EXP_THS7368_FILTER2_MASK  (0x80u)
#define VPS_VC_IO_EXP_THS7368_FILTER_SHIFT  (0x06u)
#define pcf8575_IR_REMOTE_OFF (0x40)


static const struct i2c_device_id pcf8575_video_id[] = {
	{ "pcf8575_1", 0 },
	{ }
};
static struct i2c_client *pcf8575_client;
static unsigned char pcf8575_port[2] = {0x4F, 0x7F};
int vps_ti814x_select_video_decoder(int vid_decoder_id);

static int pcf8575_video_probe(struct i2c_client *client,
				const struct i2c_device_id *id)
{
	pcf8575_client = client;
	vps_ti814x_select_video_decoder(0);
	return 0;
}

static int __devexit pcf8575_video_remove(struct i2c_client *client)
{
	pcf8575_client = NULL;
	return 0;
}

static struct i2c_driver pcf8575_driver = {
	.driver = {
		.name   = "pcf8575_1",
	},
	.probe          = pcf8575_video_probe,
	.remove         = pcf8575_video_remove,
	.id_table       = pcf8575_video_id,
};
static const struct i2c_device_id pcf8575_cir_id[] = {
	{ "IO Expander", 0 },
	{ }
};
static struct i2c_client *pcf8575_cir_client;
static unsigned char pcf8575_cir_port[2] = {0, 0xbf};
static int pcf8575_cir_enable(void);
static int pcf8575_cir_probe(struct i2c_client *client,
				const struct i2c_device_id *id)
{
	pcf8575_cir_client = client;
	pcf8575_cir_enable();
	return 0;
}

static int __devexit pcf8575_cir_remove(struct i2c_client *client)
{
	pcf8575_cir_client = NULL;
	return 0;
}
static struct i2c_driver pcf8575_cir_driver = {
	.driver = {
		.name	= "IO Expander",
	},
	.probe		= pcf8575_cir_probe,
	.remove		= pcf8575_cir_remove,
	.id_table		= pcf8575_cir_id,
};
int ti814x_pcf8575_cir_init(void)
{
	i2c_add_driver(&pcf8575_cir_driver);
	return 0;
}

int ti814x_pcf8575_cir_exit(void)
{
	i2c_del_driver(&pcf8575_cir_driver);
	return 0;
}
int ti814x_pcf8575_init(void)
{
	i2c_add_driver(&pcf8575_driver);
	return 0;
}
EXPORT_SYMBOL(ti814x_pcf8575_init);

int ti814x_pcf8575_exit(void)
{
	i2c_del_driver(&pcf8575_driver);
	return 0;
}
EXPORT_SYMBOL(ti814x_pcf8575_exit);

/* The following two functions are stubs - Added to avoid compilation
 error, refer CQ SDOCM00094243 for more details, Need to remove this code once the
 CQ is fixed */
int ti811x_pcf8575_init(void)
{
        return 0;
}
int ti811x_pcf8575_exit(void)
{
        return 0;
}

#define VPS_VC_IO_EXP_RESET_DEV_MASK        (0x0Fu)
#define VPS_VC_IO_EXP_SEL_VIN0_S1_MASK      (0x04u)
#define VPS_VC_IO_EXP_THS7368_DISABLE_MASK  (0x10u)
#define VPS_VC_IO_EXP_THS7368_BYPASS_MASK   (0x20u)
#define VPS_VC_IO_EXP_THS7368_FILTER1_MASK  (0x40u)
#define VPS_VC_IO_EXP_THS7368_FILTER2_MASK  (0x80u)
#define VPS_VC_IO_EXP_THS7368_FILTER_SHIFT  (0x06u)
static void cir_pin_mux(void)
{
	char mux_name[100];
	sprintf(mux_name, "uart0_rin.uart1_rxd_mux0");
	omap_mux_init_signal(mux_name, OMAP_MUX_MODE0 |
			TI814X_PULL_DIS | TI814X_INPUT_EN);
	return;
}

int pcf8575_cir_enable(void)
{
	int ret = 0;
	struct i2c_msg msg = {
		.addr = pcf8575_cir_client->addr,
		.flags = 1,
		.len = 2,
	};
	msg.buf = pcf8575_cir_port;
	ret = i2c_transfer(pcf8575_cir_client->adapter, &msg, 1);
	msg.flags = 0;
	if (ret < 0)
		printk(KERN_ERR "I2C: Read failed at %s %d with error code: %d\n",
			__func__, __LINE__, ret);
	pcf8575_cir_port[0] = msg.buf[0];
	pcf8575_cir_port[1] = (msg.buf[1] & ~(pcf8575_IR_REMOTE_OFF));
	ret = i2c_transfer(pcf8575_cir_client->adapter, &msg, 1);
	cir_pin_mux();
	if (ret < 0)
		printk(KERN_ERR "I2C: Transfer failed at %s %d with error code: %d\n",
			__func__, __LINE__, ret);
	return ret;

}
int vps_ti814x_select_video_decoder(int vid_decoder_id)
{
	int ret = 0;
	struct i2c_msg msg = {
			.addr = pcf8575_client->addr,
			.flags = 0,
			.len = 2,
		};
	msg.buf = pcf8575_port;
	if (VPS_SEL_TVP7002_DECODER == vid_decoder_id)
		pcf8575_port[1] &= ~VPS_VC_IO_EXP_SEL_VIN0_S1_MASK;
	else
		pcf8575_port[1] |= VPS_VC_IO_EXP_SEL_VIN0_S1_MASK;
	ret = (i2c_transfer(pcf8575_client->adapter, &msg, 1));
	if (ret < 0)
		printk(KERN_ERR "I2C: Transfer failed at %s %d with error code: %d\n",
			__func__, __LINE__, ret);
	return ret;
}
EXPORT_SYMBOL(vps_ti814x_select_video_decoder);

#define I2C_RETRY_COUNT 10u
int vps_ti814x_set_tvp7002_filter(enum fvid2_standard standard)
{
	int filter_sel;
	int ret;
	struct i2c_msg msg = {
			.addr = pcf8575_client->addr,
			.flags = 0,
			.len = 2,
		};

	pcf8575_port[0] &= ~(VPS_VC_IO_EXP_THS7368_DISABLE_MASK
		| VPS_VC_IO_EXP_THS7368_BYPASS_MASK
		| VPS_VC_IO_EXP_THS7368_FILTER1_MASK
		| VPS_VC_IO_EXP_THS7368_FILTER2_MASK);
	switch (standard) {
	case FVID2_STD_1080P_60:
	case FVID2_STD_1080P_50:
	case FVID2_STD_SXGA_60:
	case FVID2_STD_SXGA_75:
	case FVID2_STD_SXGAP_60:
	case FVID2_STD_SXGAP_75:
	case FVID2_STD_UXGA_60:
		filter_sel = 0x03u;  /* Filter2: 1, Filter1: 1 */
		break;
	case FVID2_STD_1080I_60:
	case FVID2_STD_1080I_50:
	case FVID2_STD_1080P_24:
	case FVID2_STD_1080P_30:
	case FVID2_STD_720P_60:
	case FVID2_STD_720P_50:
	case FVID2_STD_SVGA_60:
	case FVID2_STD_SVGA_72:
	case FVID2_STD_SVGA_75:
	case FVID2_STD_SVGA_85:
	case FVID2_STD_XGA_60:
	case FVID2_STD_XGA_70:
	case FVID2_STD_XGA_75:
	case FVID2_STD_XGA_85:
	case FVID2_STD_WXGA_60:
	case FVID2_STD_WXGA_75:
	case FVID2_STD_WXGA_85:
		filter_sel = 0x01u;  /* Filter2: 0, Filter1: 1 */
		break;
	case FVID2_STD_480P:
	case FVID2_STD_576P:
	case FVID2_STD_VGA_60:
	case FVID2_STD_VGA_72:
	case FVID2_STD_VGA_75:
	case FVID2_STD_VGA_85:
		filter_sel = 0x02u;  /* Filter2: 1, Filter1: 0 */
		break;
	case FVID2_STD_NTSC:
	case FVID2_STD_PAL:
	case FVID2_STD_480I:
	case FVID2_STD_576I:
	case FVID2_STD_D1:
		filter_sel = 0x00u;  /* Filter2: 0, Filter1: 0 */
		break;

	default:
		filter_sel = 0x01u;  /* Filter2: 0, Filter1: 1 */
		break;
	}
	pcf8575_port[0] |=
		(filter_sel << VPS_VC_IO_EXP_THS7368_FILTER_SHIFT);
	msg.buf = pcf8575_port;
	ret =  (i2c_transfer(pcf8575_client->adapter, &msg, 1));
	if (ret < 0) {
		printk(KERN_ERR "I2C: Transfer failed at %s %d with error code: %d\n",
			__func__, __LINE__, ret);
		return ret;
	}
	return 0;
}
EXPORT_SYMBOL(vps_ti814x_set_tvp7002_filter);
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

static struct regulator_consumer_supply ti8148evm_mpu_supply =
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
		.consumer_supplies	= &ti8148evm_mpu_supply,
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
		I2C_BOARD_INFO("IO Expander", 0x20),
	},
	{
		I2C_BOARD_INFO("tlc59108", 0x40),
	},
	{
		I2C_BOARD_INFO("pcf8575", 0x21),
		.platform_data = &io_expander_data,
	},
	{
		I2C_BOARD_INFO("qt602240_ts", 0x4A),
		.platform_data = &ts_platform_data,
	},
	{
		I2C_BOARD_INFO("tps65911", 0x2D),
		.platform_data = &tps65911_pdata,
	},
#ifndef CONFIG_TI8148_EVM_OPTIMIZED
	{
		I2C_BOARD_INFO("qt602240_ts", 0x4A),
		.platform_data = &ts_platform_data,
	},
	{
		I2C_BOARD_INFO("PCF_8575", 0x21),
	},
	{
		I2C_BOARD_INFO("tlc59108", 0x40),
	},
#endif

};

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
	ti814x_i2c_boardinfo[6].irq = gpio_to_irq(GPIO_TSC);

	gpio_export(31, true);
}

static void __init ti814x_evm_i2c_init(void)
{
	/* There are 4 instances of I2C in TI814X but currently only one
	 * instance is being used on the TI8148 EVM
	 */
#ifndef CONFIG_TI8148_EVM_OPTIMIZED
	omap_register_i2c_bus(1, 100, ti814x_i2c_boardinfo,
				ARRAY_SIZE(ti814x_i2c_boardinfo));
	omap_register_i2c_bus(3, 100, ti814x_i2c_boardinfo1,
				ARRAY_SIZE(ti814x_i2c_boardinfo1));
#else
	omap_register_i2c_bus(1, 400, ti814x_i2c_boardinfo,
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
		.size           = 800 * SZ_256K,
#else
		.size           = 1601 * SZ_128K,
#endif
	},
	{
		.name           = "Reserved",
		.offset         = MTDPART_OFS_APPEND,   /* Offset = 0xCEE0000 */
		.size           = MTDPART_SIZ_FULL,
	},
};

/* SPI fLash information */
struct mtd_partition ti8148_spi_partitions[] = {
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

const struct flash_platform_data ti8148_spi_flash = {
	.type		= "w25x32",
	.name		= "spi_flash",
	.parts		= ti8148_spi_partitions,
	.nr_parts	= ARRAY_SIZE(ti8148_spi_partitions),
};

struct spi_board_info __initdata ti8148_spi_slave_info[] = {
	{
		.modalias	= "m25p80",
		.platform_data	= &ti8148_spi_flash,
		.irq		= -1,
		.max_speed_hz	= 75000000,
		.bus_num	= 1,
		.chip_select	= 0,
	},
//to register SPI flash on SPI2.0
/*	{
                .modalias       = "m25p80",
                .platform_data  = &ti8148_spi_flash,
                .irq            = -1,
                .max_speed_hz   = 75000000,
                .bus_num        = 2,
                .chip_select    = 0,
        },
*/
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

static void __init ti8148_evm_init_irq(void)
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

#ifdef CONFIG_TI8148EVM_WL12XX

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
	omap_mux_init_signal("gpio1_15_mux1", TI814X_PULL_DIS);
	omap_mux_init_signal("gpio0_30", TI814X_INPUT_EN | TI814X_PULL_DIS);

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

#else /* CONFIG_TI8148EVM_WL12XX */

static void __init ti814x_wl12xx_init(void) { }

#endif

static void __init ti8148_evm_init(void)
{

	ti814x_mux_init(board_mux);
	omap_serial_init();
#ifndef CONFIG_TI8148_EVM_OPTIMIZED
	ti814x_tsc_init();
#endif
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
	board_nor_init(ti814x_evm_norflash_partitions,
		ARRAY_SIZE(ti814x_evm_norflash_partitions), 0);

	/* LSI Gigabit Phy fixup */
	phy_register_fixup_for_uid(LSI_PHY_ID, LSI_PHY_MASK,
				   ti8148_evm_lsi_phy_fixup);
	ti814x_pcf8575_cir_init();
}

static void __init ti8148_evm_map_io(void)
{
	omap2_set_globals_ti814x();
	ti81xx_map_common_io();
}

MACHINE_START(TI8148EVM, "ti8148evm")
	/* Maintainer: Texas Instruments */
	.boot_params	= 0x80000100,
	.map_io		= ti8148_evm_map_io,
	.reserve         = ti81xx_reserve,
	.init_irq	= ti8148_evm_init_irq,
	.init_machine	= ti8148_evm_init,
	.timer		= &omap_timer,
MACHINE_END
