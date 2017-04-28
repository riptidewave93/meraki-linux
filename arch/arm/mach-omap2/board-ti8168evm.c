/*
 * Code for TI8168 EVM.
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
#include <linux/platform_device.h>
#include <linux/i2c/at24.h>
#include <linux/device.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>
#include <linux/device.h>
#include <linux/mtd/nand.h>
#include <linux/spi/spi.h>
#include <linux/spi/flash.h>
#include <linux/mtd/physmap.h>
#include <linux/phy.h>
#include <linux/gpio.h>
#include <linux/regulator/machine.h>
#include <linux/regulator/gpio-regulator.h>
#include <linux/sii9022a.h>
#include <linux/clk.h>

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

#include "control.h"


#include "clock.h"
#include "mux.h"
#include "hsmmc.h"
#include "board-flash.h"
#include <mach/board-ti816x.h>

static struct omap2_hsmmc_info mmc[] = {
	{
		.mmc		= 1,
		.caps           = MMC_CAP_4_BIT_DATA,
		.gpio_cd	= -EINVAL,/* Dedicated pins for CD and WP */
		.gpio_wp	= -EINVAL,
		.ocr_mask	= MMC_VDD_33_34,
	},
	{}	/* Terminator */
};

#define VPS_VC_IO_EXP_RESET_DEV_MASK        (0x0Fu)
#define VPS_VC_IO_EXP_SEL_VIN0_S1_MASK      (0x04u)
#define VPS_VC_IO_EXP_THS7368_DISABLE_MASK  (0x10u)
#define VPS_VC_IO_EXP_THS7368_BYPASS_MASK   (0x20u)
#define VPS_VC_IO_EXP_THS7368_FILTER1_MASK  (0x40u)
#define VPS_VC_IO_EXP_THS7368_FILTER2_MASK  (0x80u)
#define VPS_VC_IO_EXP_THS7368_FILTER_SHIFT  (0x06u)

static struct mtd_partition ti816x_evm_norflash_partitions[] = {
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


/* Maximum ports supported by the PCF8575 */
#define VPS_PCF8575_MAX_PORTS           (2u)

/* Macros for accessing for ports */
#define VPS_PCF8575_PORT0               (0u)
#define VPS_PCF8575_PORT1               (1u)

/* Macros for PCF8575 Pins */
#define VPS_PCF8575_PIN0                (0x1)
#define VPS_PCF8575_PIN1                (0x2)
#define VPS_PCF8575_PIN2                (0x4)
#define VPS_PCF8575_PIN3                (0x8)
#define VPS_PCF8575_PIN4                (0x10)
#define VPS_PCF8575_PIN5                (0x20)
#define VPS_PCF8575_PIN6                (0x40)
#define VPS_PCF8575_PIN7                (0x80)

#define VPS_PCF8575_PIN10               (0x1)
#define VPS_PCF8575_PIN11               (0x2)

#define VPS_THS7375_MASK                (VPS_PCF8575_PIN10 | VPS_PCF8575_PIN11)

#define VPS_THS7360_SD_MASK             (VPS_PCF8575_PIN2 | VPS_PCF8575_PIN5)

#define VPS_THS7360_SF_MASK             (VPS_PCF8575_PIN0 |                    \
					 VPS_PCF8575_PIN1 |                    \
					 VPS_PCF8575_PIN3 |                    \
					 VPS_PCF8575_PIN4)

#define NAND_BLOCK_SIZE					SZ_128K

/* Macro for GPIO voltage regulator */
#define VR_GPIO_INSTANCE	0

static struct mtd_partition ti816x_nand_partitions[] = {
/* All the partition sizes are listed in terms of NAND block size */
	{
		.name           = "U-Boot",
		.offset         = 0,    /* Offset = 0x0 */
		.size           = 19 * NAND_BLOCK_SIZE,
	},
	{
		.name           = "U-Boot Env",
		.offset         = MTDPART_OFS_APPEND,   /* Offset = 0x260000 */
		.size           = 1 * NAND_BLOCK_SIZE,
	},
	{
		.name           = "Kernel",
		.offset         = MTDPART_OFS_APPEND,   /* Offset = 0x280000 */
		.size           = 34 * NAND_BLOCK_SIZE,
	},
	{
		.name           = "File System",
		.offset         = MTDPART_OFS_APPEND,   /* Offset = 0x6C0000 */
		.size           = 1601 * NAND_BLOCK_SIZE,
	},
	{
		.name           = "Reserved",
		.offset         = MTDPART_OFS_APPEND,   /* Offset = 0xCEE0000 */
		.size           = MTDPART_SIZ_FULL,
	},
};

static struct at24_platform_data eeprom_info = {
	.byte_len       = (256*1024) / 8,
	.page_size      = 64,
	.flags          = AT24_FLAG_ADDR16,
};

#ifdef CONFIG_REGULATOR_GPIO
static struct regulator_consumer_supply ti816x_gpio_dcdc_supply[] = {
	{
		.supply = "vdd_avs",
	},
};

static struct regulator_init_data gpio_pmic_init_data = {
	.constraints = {
		.min_uV		= 800000,
		.max_uV		= 1025000,
		.valid_ops_mask	= (REGULATOR_CHANGE_VOLTAGE |
			REGULATOR_CHANGE_STATUS),
	},
	.num_consumer_supplies	= ARRAY_SIZE(ti816x_gpio_dcdc_supply),
	.consumer_supplies	= ti816x_gpio_dcdc_supply,
};

/* Supported voltage values for regulators */
static struct gpio_vr_data ti816x_vsel_table[] = {
	{0x0, 800000}, {0x8, 815000}, {0x4, 830000}, {0xC, 845000},
	{0x2, 860000}, {0xA, 875000}, {0x6, 890000}, {0xE, 905000},
	{0x1, 920000}, {0x9, 935000}, {0x5, 950000}, {0xD, 965000},
	{0x3, 980000}, {0xB, 995000}, {0x7, 1010000}, {0xF, 1025000},
};

static struct gpio vcore_gpios[] = {
	{ (VR_GPIO_INSTANCE * 32) + 0, GPIOF_IN, "vgpio 0"},
	{ (VR_GPIO_INSTANCE * 32) + 1, GPIOF_IN, "vgpio 1"},
	{ (VR_GPIO_INSTANCE * 32) + 2, GPIOF_IN, "vgpio 2"},
	{ (VR_GPIO_INSTANCE * 32) + 3, GPIOF_IN, "vgpio 3"},
};

/* GPIO regulator platform data */
static struct gpio_reg_platform_data gpio_vr_init_data = {
	.name			= "VFB",
	.pmic_init_data		= &gpio_pmic_init_data,
	.gpio_vsel_table	= ti816x_vsel_table,
	.num_voltages		= ARRAY_SIZE(ti816x_vsel_table),
	.gpios			= vcore_gpios,
	.gpio_single_bank	= true,
	.gpio_arr_mask		= 0xF,
	.num_gpio_pins		= ARRAY_SIZE(vcore_gpios),
	.pmic_vout		= 600000,
};

/* VCORE for SR regulator init */
static struct platform_device ti816x_gpio_vr_device = {
	.name		= "gpio_vr",
	.id		= -1,
	.dev = {
		.platform_data = &gpio_vr_init_data,
	},
};

static void __init ti816x_gpio_vr_init(void)
{
	if (platform_device_register(&ti816x_gpio_vr_device))
		printk(KERN_ERR "failed to register ti816x_gpio_vr device\n");
	else
		printk(KERN_INFO "registered ti816x_gpio_vr device\n");
}
#else
static inline void ti816x_gpio_vr_init(void) {}
#endif

static struct sii9022a_platform_data sii9022a_pdata = {
	.hdmi_hot_plug_gpio_intr_line   = 0,
	.sync_mode                      = 0,
	.clk_edge                       = 0,
};

static struct i2c_board_info __initdata ti816x_i2c_boardinfo0[] = {
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

};

static struct i2c_board_info __initdata ti816x_i2c_boardinfo1[] = {
	{
		I2C_BOARD_INFO("pcf8575_1", 0x20),
	},
	{
		I2C_BOARD_INFO("sii9022a", 0x39),
		.platform_data	= &sii9022a_pdata,
	},
	{
		I2C_BOARD_INFO("pcf8575_1", 0x21),
	},
	{
		I2C_BOARD_INFO("pcf8575_1", 0x2e),
	},
};

static struct i2c_client *pcf8575_1_client;
static unsigned char pcf8575_1_port[2] = {0, 0};

static struct i2c_client *pcf8575_2_client;
static unsigned char pcf8575_2_port[2] = {0xFF, 0x2F};

static struct i2c_client *ths7353_client;

int vps_ti816x_select_video_decoder(int vid_decoder_id)
{
	int ret = 0;
	struct i2c_msg msg = {
			.addr = pcf8575_2_client->addr,
			.flags = 0,
			.len = 2,
		};
	msg.buf = pcf8575_2_port;
	if (VPS_SEL_TVP7002_DECODER == vid_decoder_id)
		pcf8575_2_port[1] &= ~VPS_VC_IO_EXP_SEL_VIN0_S1_MASK;
	else
		pcf8575_2_port[1] |= VPS_VC_IO_EXP_SEL_VIN0_S1_MASK;
	ret = (i2c_transfer(pcf8575_2_client->adapter, &msg, 1));

	if (ret < 0)
		printk(KERN_ERR "I2C: Transfer failed at %s %d with error code: %d\n",
			__func__, __LINE__, ret);
	return ret;
}
EXPORT_SYMBOL(vps_ti816x_select_video_decoder);
#define I2C_RETRY_COUNT 10u
int vps_ti816x_set_tvp7002_filter(enum fvid2_standard standard)
{
	int filter_sel;
	int ret;
	struct i2c_msg msg = {
			.addr = pcf8575_2_client->addr,
			.flags = 0,
			.len = 2,
		};

	pcf8575_2_port[0] &= ~(VPS_VC_IO_EXP_THS7368_DISABLE_MASK
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
	pcf8575_2_port[0] |=
		(filter_sel << VPS_VC_IO_EXP_THS7368_FILTER_SHIFT);
	msg.buf = pcf8575_2_port;
	ret =  (i2c_transfer(pcf8575_2_client->adapter, &msg, 1));
	if (ret < 0) {
		printk(KERN_ERR "I2C: Transfer failed at %s %d with error code: %d\n",
			__func__, __LINE__, ret);
		return ret;
	}
	return 0;
#if 0
	int	status = FVID2_SOK;
	u8	regAddr, regValue;
	int	retry, error;

	switch (standard) {
	case FVID2_STD_1080P_60:
	case FVID2_STD_1080P_50:
	case FVID2_STD_SXGA_60:
	case FVID2_STD_SXGA_75:
	case FVID2_STD_SXGAP_60:
	case FVID2_STD_SXGAP_75:
	case FVID2_STD_UXGA_60:
	    regValue = 0x9Cu;
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
	    regValue = 0x94u;
	    break;

	case FVID2_STD_480P:
	case FVID2_STD_576P:
	case FVID2_STD_VGA_60:
	case FVID2_STD_VGA_72:
	case FVID2_STD_VGA_75:
	case FVID2_STD_VGA_85:
	    regValue = 0x4Cu;
	    break;

	case FVID2_STD_NTSC:
	case FVID2_STD_PAL:
	case FVID2_STD_480I:
	case FVID2_STD_576I:
	case FVID2_STD_D1:
	    regValue = 0x04u;
	    break;

	default:
	    regValue = 0x94u;
	    break;
	}
	for (retry = 0; retry < I2C_RETRY_COUNT; retry++) {
		regAddr = 0x01u;
		error = i2c_smbus_write_byte_data(ths7353_client,
				regAddr, regValue);
		if (error < 0) {
			printk(KERN_ERR "I2C write failed at %d\n\n", __LINE__);
			status = -1;
		}
		regAddr = 0x02u;
		error = i2c_smbus_write_byte_data(ths7353_client,
				regAddr, regValue);
		if (error < 0) {
			printk(KERN_ERR "I2C write failed at %d\n\n", __LINE__);
			status = -1;
		}
		regAddr = 0x03u;
		error = i2c_smbus_write_byte_data(ths7353_client,
				regAddr, regValue);
		if (error < 0) {
			printk(KERN_ERR "I2C write failed at %d\n\n", __LINE__);
			status = -1;
		}
	}

	return status;
#endif
}
EXPORT_SYMBOL(vps_ti816x_set_tvp7002_filter);
int pcf8575_ths7375_enable(enum ti81xx_ths_filter_ctrl ctrl)
{
	struct i2c_msg msg = {
			.addr = pcf8575_1_client->addr,
			.flags = 0,
			.len = 2,
		};

	pcf8575_1_port[1] &= ~VPS_THS7375_MASK;
	pcf8575_1_port[1] |= (ctrl & VPS_THS7375_MASK);
	msg.buf = pcf8575_1_port;

	return i2c_transfer(pcf8575_1_client->adapter, &msg, 1);
}

int pcf8575_ths7360_sd_enable(enum ti81xx_ths_filter_ctrl ctrl)
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

int pcf8575_ths7360_hd_enable(enum ti81xx_ths7360_sf_ctrl ctrl)
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
	if (client->addr == 0x20)
		pcf8575_1_client = client;
	else if (client->addr == 0x21)
		pcf8575_2_client = client;
	else if (client->addr == 0x2e)
		ths7353_client = client;
	 else
		BUG();
	return 0;
}

static int __devexit pcf8575_video_remove(struct i2c_client *client)
{
	pcf8575_1_client = NULL;
	pcf8575_2_client = NULL;
	return 0;
}
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
int ti816x_pcf8575_cir_init(void)
{
	i2c_add_driver(&pcf8575_cir_driver);
	return 0;
}

int ti816x_pcf8575_cir_exit(void)
{
	i2c_del_driver(&pcf8575_cir_driver);
	return 0;
}
static const struct i2c_device_id pcf8575_video_id[] = {
	{ "pcf8575_1", 0 },
	{ }
};

static struct i2c_driver pcf8575_driver = {
	.driver = {
		.name   = "pcf8575_1",
	},
	.probe          = pcf8575_video_probe,
	.remove         = pcf8575_video_remove,
	.id_table       = pcf8575_video_id,
};

int ti816x_pcf8575_init(void)
{
	i2c_add_driver(&pcf8575_driver);
	return 0;
}

int ti816x_pcf8575_exit(void)
{
	i2c_del_driver(&pcf8575_driver);
	return 0;
}

/* FIX ME: Check on the Bit Value */

#define TI816X_EVM_CIR_UART BIT(5)
#define pcf8575_IR_REMOTE_OFF (0x40)

static struct i2c_client *cpld_reg0_client;

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
	if (ret < 0)
		printk(KERN_ERR "I2C: Transfer failed at %s %d with error code: %d\n",
			__func__, __LINE__, ret);
	return ret;

}
/* CPLD Register 0 Client: used for I/O Control */
static int cpld_reg0_probe(struct i2c_client *client,
			const struct i2c_device_id *id)
{
	u8 data;
	struct i2c_msg msg[2] = {
		{
			.addr = client->addr,
			.flags = I2C_M_RD,
			.len = 1,
			.buf = &data,
		},
		{
			.addr = client->addr,
			.flags = 0,
			.len = 1,
			.buf = &data,
		},
	};

	cpld_reg0_client = client;

	/* Clear UART CIR to enable cir operation. */
		i2c_transfer(client->adapter, msg, 1);
		data &= ~(TI816X_EVM_CIR_UART);
		i2c_transfer(client->adapter, msg + 1, 1);
	return 0;
}

static const struct i2c_device_id cpld_reg_ids[] = {
		{ "cpld_reg0", 0, },
		{ },
};

static struct i2c_driver ti816xevm_cpld_driver = {
	.driver.name    = "cpld_reg0",
	.id_table       = cpld_reg_ids,
	.probe          = cpld_reg0_probe,
};

static int __init ti816x_evm_i2c_init(void)
{
	omap_register_i2c_bus(1, 100, ti816x_i2c_boardinfo0,
		ARRAY_SIZE(ti816x_i2c_boardinfo0));
	omap_register_i2c_bus(2, 100, ti816x_i2c_boardinfo1,
		ARRAY_SIZE(ti816x_i2c_boardinfo1));
	return 0;
}

/* SPI fLash information */
struct mtd_partition ti816x_spi_partitions[] = {
	/* All the partition sizes are listed in terms of NAND block size */
	{
		.name		= "U-Boot",
		.offset		= 0,	/* Offset = 0x0 */
		.size		= 64 * SZ_4K,
		.mask_flags	= MTD_WRITEABLE,	/* force read-only */
	},
	{
		.name		= "U-Boot Env",
		.offset		= MTDPART_OFS_APPEND,	/* Offset = 0x40000 */
		.size		= 2 * SZ_4K,
	},
	{
		.name		= "Kernel",
		.offset		= MTDPART_OFS_APPEND,	/* Offset = 0x42000 */
		.size		= 640 * SZ_4K,
	},
	{
		.name		= "File System",
		.offset		= MTDPART_OFS_APPEND,	/* Offset = 0x2C2000 */
		.size		= MTDPART_SIZ_FULL,	/* size = 1.24 MiB */
	}
};

const struct flash_platform_data ti816x_spi_flash = {
	.name		= "spi_flash",
	.parts		= ti816x_spi_partitions,
	.nr_parts	= ARRAY_SIZE(ti816x_spi_partitions),
};

struct spi_board_info __initdata ti816x_spi_slave_info[] = {
	{
		.modalias	= "m25p80",
		.platform_data	= &ti816x_spi_flash,
		.irq		= -1,
		.max_speed_hz	= 75000000,
		.bus_num	= 1,
		.chip_select	= 0,
	},
};

static void __init ti816x_spi_init(void)
{
	spi_register_board_info(ti816x_spi_slave_info,
				ARRAY_SIZE(ti816x_spi_slave_info));
}


static void __init ti8168_evm_init_irq(void)
{
	omap2_init_common_infrastructure();
	omap2_init_common_devices(NULL, NULL);
	omap_init_irq();
	gpmc_init();
}

static u8 ti8168_iis_serializer_direction[] = {
	TX_MODE,	RX_MODE,	INACTIVE_MODE,	INACTIVE_MODE,
	INACTIVE_MODE,	INACTIVE_MODE,	INACTIVE_MODE,	INACTIVE_MODE,
	INACTIVE_MODE,	INACTIVE_MODE,	INACTIVE_MODE,	INACTIVE_MODE,
	INACTIVE_MODE,	INACTIVE_MODE,	INACTIVE_MODE,	INACTIVE_MODE,
};

static struct snd_platform_data ti8168_evm_snd_data = {
	.tx_dma_offset	= 0x46800000,
	.rx_dma_offset	= 0x46800000,
	.op_mode	= DAVINCI_MCASP_IIS_MODE,
	.num_serializer = ARRAY_SIZE(ti8168_iis_serializer_direction),
	.tdm_slots	= 2,
	.serial_dir	= ti8168_iis_serializer_direction,
	.asp_chan_q	= EVENTQ_2,
	.version	= MCASP_VERSION_2,
	.txnumevt	= 64,
	.rxnumevt	= 64,
};

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

#ifdef CONFIG_OMAP_MUX
static struct omap_board_mux board_mux[] __initdata = {

	/* PIN mux for non-muxed NOR */
	TI816X_MUX(TIM7_OUT, OMAP_MUX_MODE1),	/* gpmc_a12 */
	TI816X_MUX(UART1_CTSN, OMAP_MUX_MODE1),	/* gpmc_a13 */
	TI816X_MUX(UART1_RTSN, OMAP_MUX_MODE1),	/* gpmc_a14 */
	TI816X_MUX(UART2_RTSN, OMAP_MUX_MODE1),	/* gpmc_a15 */
	/* REVISIT: why 2 lines configured as gpmc_a15 */
	TI816X_MUX(SC1_RST, OMAP_MUX_MODE1),	/* gpmc_a15 */
	TI816X_MUX(UART2_CTSN, OMAP_MUX_MODE1),	/* gpmc_a16 */
	TI816X_MUX(UART0_RIN, OMAP_MUX_MODE1),	/* gpmc_a17 */
	TI816X_MUX(UART0_DCDN, OMAP_MUX_MODE1),	/* gpmc_a18 */
	TI816X_MUX(UART0_DSRN, OMAP_MUX_MODE1),	/* gpmc_a19 */
	TI816X_MUX(UART0_DTRN, OMAP_MUX_MODE1),	/* gpmc_a20 */
	TI816X_MUX(SPI_SCS3, OMAP_MUX_MODE1),	/* gpmc_a21 */
	TI816X_MUX(SPI_SCS2, OMAP_MUX_MODE1),	/* gpmc_a22 */
	TI816X_MUX(GP0_IO6, OMAP_MUX_MODE2),	/* gpmc_a23 */
	TI816X_MUX(TIM6_OUT, OMAP_MUX_MODE1),	/* gpmc-a24 */
	TI816X_MUX(SC0_DATA, OMAP_MUX_MODE1),	/* gpmc_a25 */
	/* for controlling high address */
	TI816X_MUX(GPMC_A27, OMAP_MUX_MODE1),	/* gpio-20 */
	{ .reg_offset = OMAP_MUX_TERMINATOR },
};
#else
#define board_mux	NULL
#endif

int __init ti_ahci_register(u8 num_inst);

#ifdef CONFIG_SND_SOC_TI81XX_HDMI
static struct snd_hdmi_platform_data ti8168_snd_hdmi_pdata = {
	.dma_addr = TI81xx_HDMI_WP + HDMI_WP_AUDIO_DATA,
	.channel = 53,
	.dma_chan_q = EVENTQ_0,
	.data_type = 4,
	.acnt = 4,
	.fifo_level = 0x20,
};

static struct platform_device ti8168_hdmi_audio_device = {
	.name	= "hdmi-dai",
	.id	= -1,
        .dev = {
		.platform_data = &ti8168_snd_hdmi_pdata,
        }
};

static struct platform_device ti8168_hdmi_codec_device = {
	.name	= "hdmi-dummy-codec",
	.id	= -1,
};

static struct platform_device *ti8168_devices[] __initdata = {
	&ti8168_hdmi_audio_device,
	&ti8168_hdmi_codec_device,
};

/*
 * TI8168 PG2.0 support Auto CTS which needs MCLK .
 * McBSP clk is used as MCLK in PG2.0 which have 4 parent clks
 * sysclk20, sysclk21, sysclk21 and CLKS(external)
 * Currently we are using sysclk22 as the parent for McBSP clk
 * ToDo:
*/
void __init ti8168_hdmi_mclk_init(void)
{
	int ret = 0;
	struct clk *parent, *child;

	/* modify the clk name from list to use different clk source */
	parent = clk_get(NULL, "sysclk22_ck");
	if (IS_ERR(parent))
		pr_err("Unable to get [sysclk22_ck] clk\n");

	/* get HDMI dev clk*/
	child = clk_get(NULL, "hdmi_i2s_ck");
	if (IS_ERR(child))
		pr_err("Unable to get [hdmi_i2s_ck] clk\n");

	ret = clk_set_parent(child, parent);
	if (ret < 0)
		pr_err("Unable to set parent [hdmi_ck] clk\n");

	pr_debug("HDMI: Audio MCLK setup complete!\n");

}
#endif


static void __init ti8168_evm_init(void)
{
	int bw; /* bus-width */

	ti81xx_mux_init(board_mux);
	omap_serial_init();
	ti816x_evm_i2c_init();
	i2c_add_driver(&ti816xevm_cpld_driver);
	ti81xx_register_mcasp(0, &ti8168_evm_snd_data);
	ti816x_spi_init();
	/* initialize usb */
	usb_musb_init(&musb_board_data);

	/* nand initialisation */
	board_nand_init(ti816x_nand_partitions,
	ARRAY_SIZE(ti816x_nand_partitions), 0, NAND_OMAP_BUS_16);

	omap2_hsmmc_init(mmc);
	board_nor_init(ti816x_evm_norflash_partitions,
		ARRAY_SIZE(ti816x_evm_norflash_partitions), 0);
	ti816x_gpio_vr_init();
#ifdef CONFIG_SND_SOC_TI81XX_HDMI
	ti8168_hdmi_mclk_init();
	platform_add_devices(ti8168_devices, ARRAY_SIZE(ti8168_devices));
#endif
	regulator_has_full_constraints();
	regulator_use_dummy_regulator();
	ti816x_pcf8575_cir_init();
}

static int __init ti8168_evm_gpio_setup(void)
{
	/* GPIO-20 should be low for NOR access beyond 4KiB */
	gpio_request(20, "nor");
	gpio_direction_output(20, 0x0);
	return 0;
}
/* GPIO setup should be as subsys_initcall() as gpio driver
 * is registered in arch_initcall()
 */
subsys_initcall(ti8168_evm_gpio_setup);

static void __init ti8168_evm_map_io(void)
{
	omap2_set_globals_ti816x();
	ti81xx_map_common_io();
}

MACHINE_START(TI8168EVM, "ti8168evm")
	/* Maintainer: Texas Instruments */
	.boot_params	= 0x80000100,
	.map_io		= ti8168_evm_map_io,
	.reserve         = ti81xx_reserve,
	.init_irq	= ti8168_evm_init_irq,
	.init_machine	= ti8168_evm_init,
	.timer		= &omap_timer,
MACHINE_END
