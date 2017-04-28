/*
 * arch/arm/mach-ambarella/init-elephant.c
 *
 * Author: Anthony Ginger <hfjiang@ambarella.com>
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
#include <linux/platform_device.h>
#include <linux/dma-mapping.h>

#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <asm/hardware/gic.h>
#include <asm/gpio.h>
#include <asm/system_info.h>

#include <mach/hardware.h>
#include <mach/init.h>
#include <mach/board.h>
#include <mach/common.h>

#include <linux/spi/spi.h>
#include <linux/spi/spidev.h>

#include <linux/i2c.h>
#include <linux/i2c/pcf857x.h>
#include <linux/i2c/pca954x.h>

#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/delay.h>

#include <linux/input.h>
#include <plat/ambinput.h>

#include <linux/mmc/host.h>
#include <plat/ambcache.h>

#include "board-device.h"

#include <plat/dma.h>
#include <linux/rfkill-gpio.h>

#include <linux/mfd/wm831x/pdata.h>
/* ==========================================================================*/
#include <linux/pda_power.h>

static struct platform_device *ambarella_devices[] __initdata = {
	&ambarella_adc0,
#ifdef CONFIG_PLAT_AMBARELLA_SUPPORT_HW_CRYPTO
	&ambarella_crypto,
#endif
	&ambarella_ehci0,
	&ambarella_eth0,
	&ambarella_fb0,
	&ambarella_fb1,
	&ambarella_i2s0,
	&ambarella_pcm0,
	&ambarella_dummy_codec0,
	&ambarella_ambevk_audio_device,
	&ambarella_idc0,
	&ambarella_idc1,
	&ambarella_idc0_mux,
	&ambarella_ir0,
#ifndef CONFIG_PLAT_AMBARELLA_I1_CORTEX
	&ambarella_ohci0,
#endif
	&ambarella_ahci0,
	&ambarella_sd0,
	&ambarella_sd1,
	&ambarella_spi0,
	&ambarella_spi1,
	&ambarella_spi2,
	&ambarella_spi3,
	&ambarella_spi4,
	&ambarella_uart,
	&ambarella_uart1,
	&ambarella_uart2,
	&ambarella_uart3,
	&ambarella_udc0,
	&ambarella_dma,
	&ambarella_nand,
#if defined(CONFIG_PLAT_AMBARELLA_CORTEX)
	&mpcore_wdt,
#endif
};

static struct platform_device *ambarella_pwm_devices[] __initdata = {
	&ambarella_pwm_platform_device0,
	&ambarella_pwm_platform_device1,
	&ambarella_pwm_platform_device2,
	&ambarella_pwm_platform_device3,
	&ambarella_pwm_platform_device4,
};

/* ==========================================================================*/
static struct spi_board_info ambarella_spi_devices[] = {
	[0] = {
		.modalias	= "spidev",
		.bus_num	= 0,
		.chip_select	= 0,
	},
	[1] = {
		.modalias	= "spidev",
		.bus_num	= 0,
		.chip_select	= 1,
	},
	[2] = {
		.modalias	= "spidev",
		.bus_num	= 0,
		.chip_select	= 2,
	},
	[3] = {
		.modalias	= "spidev",
		.bus_num	= 0,
		.chip_select	= 3,
	},
	[4] = {
		.modalias	= "spidev",
		.bus_num	= 0,
		.chip_select	= 4,
	},
	[5] = {
		.modalias	= "spidev",
		.bus_num	= 0,
		.chip_select	= 5,
	},
	[6] = {
		.modalias	= "spidev",
		.bus_num	= 0,
		.chip_select	= 6,
	},
	[7] = {
		.modalias	= "spidev",
		.bus_num	= 0,
		.chip_select	= 7,
	},
	[8] = {
		.modalias	= "spidev",
		.bus_num	= 1,
		.chip_select	= 0,
	},
	[9] = {
		.modalias	= "spidev",
		.bus_num	= 2,
		.chip_select	= 0,
	},
	[10] = {
		.modalias	= "spidev",
		.bus_num	= 2,
		.chip_select	= 1,
	},
	[11] = {
		.modalias	= "spidev",
		.bus_num	= 2,
		.chip_select	= 2,
	},
	[12] = {
		.modalias	= "spidev",
		.bus_num	= 2,
		.chip_select	= 3,
	},
	[13] = {
		.modalias	= "spidev",
		.bus_num	= 2,
		.chip_select	= 4,
	},
	[14] = {
		.modalias	= "spidev",
		.bus_num	= 2,
		.chip_select	= 5,
	},
	[15] = {
		.modalias	= "spidev",
		.bus_num	= 2,
		.chip_select	= 6,
	},
	[16] = {
		.modalias	= "spidev",
		.bus_num	= 2,
		.chip_select	= 7,
	},
	[17] = {
		.modalias	= "spidev",
		.bus_num	= 3,
		.chip_select	= 0,
	},
	[18] = {
		.modalias	= "spidev",
		.bus_num	= 4,
		.chip_select	= 0,
	}
};

/* ==========================================================================*/
static struct ambarella_key_table elephant_keymap[AMBINPUT_TABLE_SIZE] = {
	{AMBINPUT_VI_KEY,	{.vi_key	= {0,	0,	0}}},
	{AMBINPUT_VI_REL,	{.vi_rel	= {0,	0,	0}}},
	{AMBINPUT_VI_ABS,	{.vi_abs	= {0,	0,	0}}},
	{AMBINPUT_VI_SW,	{.vi_sw		= {0,	0,	0}}},

	{AMBINPUT_IR_KEY,	{.ir_key	= {KEY_POWER,	3,	0x001a0015}}},	//POWER
	{AMBINPUT_IR_KEY,	{.ir_key	= {KEY_HOME,	3,	0x001a0061}}},	//HOME
	{AMBINPUT_IR_KEY,	{.ir_key	= {KEY_MENU,	3,	0x001a004d}}},	//MENU
	{AMBINPUT_IR_KEY,	{.ir_key	= {KEY_ESC,	3,	0x001a0063}}},	//BACK
	{AMBINPUT_IR_KEY,	{.ir_key	= {KEY_SEND,	3,	0x001a0010}}},	//CALL
	{AMBINPUT_IR_KEY,	{.ir_key	= {KEY_END,	3,	0x001a0020}}},	//ENDCALL
	{AMBINPUT_IR_SW,	{.ir_key	= {0,		0,	0x001a003d}}},	//SW0 OFF
	{AMBINPUT_IR_SW,	{.ir_key	= {0,		1,	0x001a0064}}},	//SW0 ON
	{AMBINPUT_IR_KEY,	{.ir_key	= {KEY_VOLUMEUP,3,	0x001a0077}}},	//VOLUME_UP
	{AMBINPUT_IR_KEY,	{.ir_key	= {KEY_VOLUMEDOWN,3,	0x001a0078}}},	//VOLUME_DOWN
	{AMBINPUT_IR_KEY,	{.ir_key	= {KEY_OK,	3,	0x001a0076}}},	//DPAD_CENTER
	{AMBINPUT_IR_KEY,	{.ir_key	= {KEY_S,	3,	0x001a0073}}},	//S
	{AMBINPUT_IR_KEY,	{.ir_key	= {KEY_W,	3,	0x001a0072}}},	//W
	{AMBINPUT_IR_KEY,	{.ir_key	= {KEY_A,	3,	0x001a0075}}},	//A
	{AMBINPUT_IR_KEY,	{.ir_key	= {KEY_D,	3,	0x001a0074}}},	//D
	{AMBINPUT_IR_KEY,	{.ir_key	= {KEY_SEARCH,	3,	0x001a007e}}},	//SEARCH

	{AMBINPUT_GPIO_KEY,	{.gpio_key	= {KEY_HOME,	1,	1,	GPIO(120),	IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING}}},
	{AMBINPUT_GPIO_KEY,	{.gpio_key	= {KEY_MENU,	1,	1,	GPIO(121),	IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING}}},
	{AMBINPUT_GPIO_KEY,	{.gpio_key	= {KEY_ESC,	1,	1,	GPIO(122),	IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING}}},
	{AMBINPUT_GPIO_KEY,	{.gpio_key	= {KEY_POWER,	1,	1,	GPIO(123),	IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING}}},

	{AMBINPUT_END},
};

static struct ambarella_key_table elephant_keymap_evk[AMBINPUT_TABLE_SIZE] = {
	{AMBINPUT_VI_KEY,	{.vi_key	= {0,	0,	0}}},
	{AMBINPUT_VI_REL,	{.vi_rel	= {0,	0,	0}}},
	{AMBINPUT_VI_ABS,	{.vi_abs	= {0,	0,	0}}},
	{AMBINPUT_VI_SW,	{.vi_sw		= {0,	0,	0}}},

	{AMBINPUT_IR_KEY,	{.ir_key	= {KEY_POWER,	3,	0x001a0015}}},	//POWER
	{AMBINPUT_IR_KEY,	{.ir_key	= {KEY_HOME,	3,	0x001a0061}}},	//HOME
	{AMBINPUT_IR_KEY,	{.ir_key	= {KEY_MENU,	3,	0x001a004d}}},	//MENU
	{AMBINPUT_IR_KEY,	{.ir_key	= {KEY_ESC,	3,	0x001a0063}}},	//BACK
	{AMBINPUT_IR_KEY,	{.ir_key	= {KEY_SEND,	3,	0x001a0010}}},	//CALL
	{AMBINPUT_IR_KEY,	{.ir_key	= {KEY_END,	3,	0x001a0020}}},	//ENDCALL
	{AMBINPUT_IR_SW,	{.ir_key	= {0,		0,	0x001a003d}}},	//SW0 OFF
	{AMBINPUT_IR_SW,	{.ir_key	= {0,		1,	0x001a0064}}},	//SW0 ON
	{AMBINPUT_IR_KEY,	{.ir_key	= {KEY_VOLUMEUP,3,	0x001a0077}}},	//VOLUME_UP
	{AMBINPUT_IR_KEY,	{.ir_key	= {KEY_VOLUMEDOWN,3,	0x001a0078}}},	//VOLUME_DOWN
	{AMBINPUT_IR_KEY,	{.ir_key	= {KEY_OK,	3,	0x001a0076}}},	//DPAD_CENTER
	{AMBINPUT_IR_KEY,	{.ir_key	= {KEY_S,	3,	0x001a0073}}},	//S
	{AMBINPUT_IR_KEY,	{.ir_key	= {KEY_W,	3,	0x001a0072}}},	//W
	{AMBINPUT_IR_KEY,	{.ir_key	= {KEY_A,	3,	0x001a0075}}},	//A
	{AMBINPUT_IR_KEY,	{.ir_key	= {KEY_D,	3,	0x001a0074}}},	//D
	{AMBINPUT_IR_KEY,	{.ir_key	= {KEY_SEARCH,	3,	0x001a007e}}},	//SEARCH

	{AMBINPUT_ADC_KEY,	{.adc_key	= {KEY_RESERVED,	0,	2,	3000,	4095}}},
	{AMBINPUT_ADC_KEY,	{.adc_key	= {KEY_HOME,		0,	2,	1820,	2250}}},	//HOME
	{AMBINPUT_ADC_KEY,	{.adc_key	= {KEY_MENU,		0,	2,	1450,	1750}}},	//MENU
	{AMBINPUT_ADC_KEY,	{.adc_key	= {KEY_ESC,		0,	2,	1050,	1350}}},	//ESC
	{AMBINPUT_ADC_KEY,	{.adc_key	= {KEY_SEARCH,		0,	2,	700,	900}}},		//SEARCH
	//{AMBINPUT_ADC_KEY,	{.adc_key	= {KEY_HP,		0,	2,	300,	500}}},		//FOCUS, do not use
	{AMBINPUT_ADC_KEY,	{.adc_key	= {KEY_CAMERA,		0,	2,	0,	100}}},		//CAMERA

	{AMBINPUT_ADC_KEY,	{.adc_key	= {KEY_RESERVED,	0,	4,	3000,	4095}}},
	{AMBINPUT_ADC_KEY,	{.adc_key	= {KEY_VOLUMEDOWN,	0,	4,	1750,	2150}}},	//VOLUME DOWN
	{AMBINPUT_ADC_KEY,	{.adc_key	= {KEY_VOLUMEUP,	0,	4,	1300,	1650}}},	//VOLUME UP
	{AMBINPUT_ADC_KEY,	{.adc_key	= {KEY_STOP,		0,	4,	850,	1100}}},	//STOP
	{AMBINPUT_ADC_KEY,	{.adc_key	= {KEY_RECORD,		0,	4,	400,	600}}},		//RECORD

	{AMBINPUT_GPIO_KEY,	{.gpio_key	= {KEY_SEND,	0,	0,	GPIO(187),
							IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING}}},		//CALL (GPIO_K2)
	{AMBINPUT_GPIO_KEY,	{.gpio_key	= {KEY_END,	0,	0,	GPIO(191),
							IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING}}},		//ENDCALL (GPIO_K1)

	{AMBINPUT_END},
};

static struct ambarella_key_table elephant_keymap_ipcam_nvr[AMBINPUT_TABLE_SIZE] = {
        {AMBINPUT_VI_KEY,       {.vi_key        = {0,   0,      0}}},
        {AMBINPUT_VI_REL,       {.vi_rel        = {0,   0,      0}}},
        {AMBINPUT_VI_ABS,       {.vi_abs        = {0,   0,      0}}},
        {AMBINPUT_VI_SW,        {.vi_sw         = {0,   0,      0}}},

	{AMBINPUT_GPIO_KEY,     {.gpio_key      = {KEY_POWER,    1,      1,
		GPIO(40), (IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING)}}},
	{AMBINPUT_GPIO_KEY,     {.gpio_key      = {KEY_HOME,     1,      1,
		GPIO(41), (IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING)}}},
	{AMBINPUT_GPIO_KEY,     {.gpio_key      = {KEY_MENU,    1,      1,
		GPIO(42), (IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING)}}},
	{AMBINPUT_GPIO_KEY,     {.gpio_key      = {KEY_ESC,     1,      1,
		GPIO(43), (IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING)}}},
	{AMBINPUT_GPIO_KEY,     {.gpio_key      = {KEY_OK,    1,      1,
		GPIO(44), (IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING)}}},
	{AMBINPUT_GPIO_KEY,     {.gpio_key      = {KEY_END,     1,      1,
		GPIO(45), (IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING)}}},

        {AMBINPUT_END},
};

static struct ambarella_input_board_info elephant_board_input_info = {
	.pkeymap		= elephant_keymap,
	.pinput_dev		= NULL,
	.pdev			= NULL,

	.abx_max_x		= 4095,
	.abx_max_y		= 4095,
	.abx_max_pressure	= 4095,
	.abx_max_width		= 16,
};

struct platform_device elephant_board_input = {
	.name		= "ambarella-input",
	.id		= -1,
	.resource	= NULL,
	.num_resources	= 0,
	.dev		= {
		.platform_data		= &elephant_board_input_info,
		.dma_mask		= &ambarella_dmamask,
		.coherent_dma_mask	= DMA_BIT_MASK(32),
	}
};

static struct rfkill_gpio_platform_data elephant_board_bt_info = {
	.name		= "bt-gpio",
	.reset_gpio	= GPIO(190),
	.shutdown_gpio	= -1,
	.type		= RFKILL_TYPE_BLUETOOTH,
};

static struct platform_device elephant_bt_rfkill = {
	.name		= "rfkill_gpio",
	.id		= -1,
	.resource	= NULL,
	.num_resources	= 0,
	.dev		= {
		.platform_data		= &elephant_board_bt_info,
		.dma_mask		= &ambarella_dmamask,
		.coherent_dma_mask	= DMA_BIT_MASK(32),
	}
};

/* ==========================================================================*/
static struct pcf857x_platform_data elephant_board_ext_gpio0_pdata = {
	.gpio_base	= EXT_GPIO(0),
};

static struct i2c_board_info elephant_board_ext_gpio_info = {
	.type			= "pcf8574a",
	.addr			= 0x3e,
	.flags			= 0,
	.platform_data		= &elephant_board_ext_gpio0_pdata,
};

static struct pca954x_platform_mode elephant_board_ext_i2c_mode = {
	.adap_id		= 0,
	.deselect_on_exit	= 1,
};

static struct pca954x_platform_data elephant_board_ext_i2c_pdata = {
	.modes			= &elephant_board_ext_i2c_mode,
	.num_modes		= 0,
};

static struct i2c_board_info elephant_board_ext_i2c_info = {
	.type			= "pca9542",
	.addr			= 0x70,
	.flags			= 0,
	.platform_data		= &elephant_board_ext_i2c_pdata,
};

/*==========================================================================*/
static void elephant_ipcam_nvr_sdxc_set_vdd(u32 vdd)
{
	pr_debug("%s = %dmV\n", __func__, vdd);
	ambarella_gpio_config(GPIO(23), GPIO_FUNC_SW_OUTPUT);
	if (vdd == 1800) {
		ambarella_gpio_set(GPIO(23), 0);
	} else {
		ambarella_gpio_set(GPIO(23), 1);
	}
	msleep(10);
}

/* ==========================================================================*/
static void __init ambarella_init_elephant(void)
{
	int					i, ret;
	int					use_bub_default = 1;
	int					use_ambarella_rtc0 = 1;
	int					use_ambarella_wdt0 = 1;

	ambarella_init_machine("Elephant");

	/* Config SD */
	fio_default_owner = SELECT_FIO_SDIO;
	ambarella_platform_sd_controller0.slot[0].gpio_cd.irq_gpio = SMIO_5;
	ambarella_platform_sd_controller0.slot[0].gpio_cd.irq_line = gpio_to_irq(SMIO_5);
	ambarella_platform_sd_controller0.slot[0].gpio_cd.irq_type = IRQ_TYPE_EDGE_BOTH;
	ambarella_platform_sd_controller0.slot[0].gpio_cd.irq_gpio_mode = GPIO_FUNC_SW_INPUT;
	ambarella_platform_sd_controller0.slot[1].gpio_cd.irq_gpio = SMIO_44;
	ambarella_platform_sd_controller0.slot[1].gpio_cd.irq_line = gpio_to_irq(SMIO_44);
	ambarella_platform_sd_controller0.slot[1].gpio_cd.irq_type = IRQ_TYPE_EDGE_BOTH;
	ambarella_platform_sd_controller0.slot[1].gpio_wp.gpio_id = SMIO_45;
	ambarella_platform_sd_controller1.slot[0].ext_power.gpio_id = GPIO(106);
	ambarella_platform_sd_controller1.slot[0].ext_power.active_level = GPIO_HIGH;
	ambarella_platform_sd_controller1.slot[0].ext_power.active_delay = 300;

	if (AMBARELLA_BOARD_TYPE(system_rev) == AMBARELLA_BOARD_TYPE_EVK) {
		switch (AMBARELLA_BOARD_REV(system_rev)) {
		case 'C':
		case 'B':
			ambarella_platform_sd_controller1.slot[0].ext_power.gpio_id = GPIO(111);
			ambarella_platform_sd_controller1.slot[0].ext_power.active_level = GPIO_HIGH;
			ambarella_platform_sd_controller1.slot[0].ext_power.active_delay = 300;
#if defined(CONFIG_CODEC_AMBARELLA_WM8994)
		 	/* the cs_pin of spi0.1 is used to determine wm8994's
		 	 * I2C address, and the cs_pin of spi0.4, spi0,5, spi0.6
		 	 * spi0.7 are used as I2S signals, so we need to prevent
		 	 * them from be modified by SPI driver */
			ambarella_spi0_cs_pins[1] = -1;
			ambarella_spi0_cs_pins[4] = -1;
			ambarella_spi0_cs_pins[5] = -1;
			ambarella_spi0_cs_pins[6] = -1;
			ambarella_spi0_cs_pins[7] = -1;
			ambarella_init_wm8994();
#endif
		case 'A':
			ambarella_board_generic.lcd_reset.gpio_id = GPIO(105);
			ambarella_board_generic.lcd_reset.active_level = GPIO_LOW;
			ambarella_board_generic.lcd_reset.active_delay = 10;

			ambarella_board_generic.lcd_backlight.gpio_id = GPIO(16);
			ambarella_board_generic.lcd_backlight.active_level = GPIO_HIGH;
			ambarella_board_generic.lcd_backlight.active_delay = 1;

			ambarella_board_generic.lcd_spi_hw.bus_id = 2;
			ambarella_board_generic.lcd_spi_hw.cs_id = 0;

			ambarella_board_generic.lcd_spi_cfg.spi_mode = 0;
			ambarella_board_generic.lcd_spi_cfg.cfs_dfs = 16;
			ambarella_board_generic.lcd_spi_cfg.baud_rate = 5000000;
			ambarella_board_generic.lcd_spi_cfg.cs_change = 0;

			ambarella_board_generic.touch_panel_irq.irq_gpio = GPIO(44);
			ambarella_board_generic.touch_panel_irq.irq_line = gpio_to_irq(44);
			ambarella_board_generic.touch_panel_irq.irq_type = IRQF_TRIGGER_FALLING;
			ambarella_board_generic.touch_panel_irq.irq_gpio_val = GPIO_LOW;
			ambarella_board_generic.touch_panel_irq.irq_gpio_mode = GPIO_FUNC_SW_INPUT;

			ambarella_board_generic.hdmi_extpower.gpio_id = GPIO(104);
			ambarella_board_generic.hdmi_extpower.active_level = GPIO_HIGH;
			ambarella_board_generic.hdmi_extpower.active_delay = 1;

			ambarella_board_generic.wifi_power.gpio_id = GPIO(109);
			ambarella_board_generic.wifi_power.active_level = GPIO_HIGH;
			ambarella_board_generic.wifi_power.active_delay = 300;
			ambarella_board_generic.wifi_sd_bus = 0;
			ambarella_board_generic.wifi_sd_slot = 1;

			ambarella_board_generic.pmic_irq.irq_gpio = GPIO(54);
			ambarella_board_generic.pmic_irq.irq_line = gpio_to_irq(54);
			ambarella_board_generic.pmic_irq.irq_type = IRQF_TRIGGER_FALLING;
			ambarella_board_generic.pmic_irq.irq_gpio_val = GPIO_LOW;
			ambarella_board_generic.pmic_irq.irq_gpio_mode = GPIO_FUNC_SW_INPUT;

			ambarella_board_generic.sata_power.gpio_id = GPIO(101);
			ambarella_board_generic.sata_power.active_level = GPIO_HIGH;
			ambarella_board_generic.sata_power.active_delay = 1;

			ambarella_board_generic.power_control.gpio_id = GPIO(120);
			ambarella_board_generic.power_control.active_level = GPIO_LOW;
#if defined(CONFIG_PMIC_AMBARELLA_WM831X)
			memcpy(ambarella_spi_devices[12].modalias, "wm8310", 6);
			ambarella_spi_devices[12].max_speed_hz = 500000;
			ambarella_spi_devices[12].platform_data = &wm8310_default_pdata;
			ambarella_spi_devices[12].irq = ambarella_board_generic.pmic_irq.irq_line;
			use_ambarella_rtc0 = 0;
			use_ambarella_wdt0= 0;
#endif
			ambarella_board_generic.gsensor_power.gpio_id = GPIO(151);
			ambarella_board_generic.gsensor_power.active_level = GPIO_HIGH;
			ambarella_board_generic.gsensor_power.active_delay = 10;

			ambarella_board_generic.gsensor_irq.irq_gpio = GPIO(49);
			ambarella_board_generic.gsensor_irq.irq_line = gpio_to_irq(GPIO(49));
			ambarella_board_generic.gsensor_irq.irq_type = IRQF_TRIGGER_RISING;
			ambarella_board_generic.gsensor_irq.irq_gpio_val = GPIO_LOW;
			ambarella_board_generic.gsensor_irq.irq_gpio_mode = GPIO_FUNC_SW_INPUT;

			ambarella_eth0_platform_info.mii_id = 0;
			ambarella_eth0_platform_info.phy_id = 0x001cc912;
			ambarella_eth0_platform_info.phy_irq.irq_gpio = GPIO(21);
			ambarella_eth0_platform_info.phy_irq.irq_line = gpio_to_irq(GPIO(21));
			ambarella_eth0_platform_info.phy_irq.irq_type = IRQF_TRIGGER_LOW;
			ambarella_eth0_platform_info.phy_irq.irq_gpio_val = GPIO_LOW;
			ambarella_eth0_platform_info.phy_irq.irq_gpio_mode = GPIO_FUNC_SW_INPUT;
			ambarella_eth0_platform_info.mii_power.gpio_id = GPIO(97);
			ambarella_eth0_platform_info.mii_reset.gpio_id = GPIO(98);

			ambarella_platform_sd_controller0.slot[0].ext_power.gpio_id = GPIO(157);
			ambarella_platform_sd_controller0.slot[0].ext_power.active_level = GPIO_HIGH;
			ambarella_platform_sd_controller0.slot[0].ext_power.active_delay = 300;
			ambarella_platform_sd_controller0.max_blk_mask = SD_BLK_SZ_512KB;
			ambarella_platform_sd_controller0.slot[0].fixed_cd = 1;
			ambarella_platform_sd_controller0.slot[0].gpio_cd.irq_gpio = -1;
			ambarella_platform_sd_controller0.slot[0].gpio_cd.irq_line = -1;
			ambarella_platform_sd_controller0.slot[0].fixed_wp = 0;
			ambarella_platform_sd_controller0.slot[0].gpio_wp.gpio_id = -1;
			ambarella_platform_sd_controller0.slot[0].default_caps |= (MMC_CAP_8_BIT_DATA | MMC_CAP_NONREMOVABLE);
			ambarella_platform_sd_controller0.slot[1].fixed_cd = 0;
			ambarella_platform_sd_controller0.slot[1].gpio_cd.irq_gpio = -1;
			ambarella_platform_sd_controller0.slot[1].gpio_cd.irq_line = -1;
			ambarella_platform_sd_controller0.slot[1].fixed_wp = 0;
			ambarella_platform_sd_controller0.slot[1].gpio_wp.gpio_id = -1;
			ambarella_platform_sd_controller1.max_clock = 12000000;
			ambarella_platform_sd_controller1.max_blk_mask = SD_BLK_SZ_512KB;
			ambarella_platform_sd_controller1.slot[0].default_caps |= MMC_CAP_8_BIT_DATA;
#if defined(CONFIG_TOUCH_AMBARELLA_TM1726)
			ambarella_tm1726_board_info.irq = ambarella_board_generic.touch_panel_irq.irq_line;
			i2c_register_board_info(2, &ambarella_tm1726_board_info, 1);
#endif
			//vin0: Rear Sensor vin1: Front Sensor
			ambarella_board_generic.vin0_power.gpio_id = GPIO(122);
			ambarella_board_generic.vin0_power.active_level = GPIO_HIGH;
			ambarella_board_generic.vin0_power.active_delay = 1;
			i2c_register_board_info(0, &ambarella_board_vin_infos[0], 1);

			ambarella_board_generic.vin1_power.gpio_id = GPIO(121);
			ambarella_board_generic.vin1_power.active_level = GPIO_HIGH;
			ambarella_board_generic.vin1_power.active_delay = 1;

			ambarella_board_generic.vin1_reset.gpio_id = GPIO(123);
			ambarella_board_generic.vin1_reset.active_level = GPIO_LOW;
			ambarella_board_generic.vin1_reset.active_delay = 1;

			ambarella_board_generic.vin1_af_power.gpio_id = GPIO(124);
			ambarella_board_generic.vin1_af_power.active_level = GPIO_HIGH;
			ambarella_board_generic.vin1_af_power.active_delay = 1;

			i2c_register_board_info(2, &ambarella_board_vin_infos[1], 1);

			platform_device_register(&elephant_bt_rfkill);
#if defined(CONFIG_LCD_AMBARELLA_1P3831)
			platform_device_register(&lcd_1p3831);
#endif
			platform_device_register(&i1evk_cpufreq_device);

			elephant_board_input_info.pkeymap = elephant_keymap_evk;

			ambarella_board_generic.pwm0_config.max_duty = 255;
			ambarella_board_generic.pwm0_config.default_duty = 255;

			ambarella_platform_adc_temper_controller0.adc_temper_channel = ADC_CH9;
			ambarella_platform_adc_temper_controller0.temper_curve = amb_temper_curve;
			platform_device_register(&ambarella_adc_temper);

			use_bub_default = 0;
			break;

		default:
			pr_warn("%s: Unknown EVK Rev[%d]\n", __func__, AMBARELLA_BOARD_REV(system_rev));
			break;
		}
	} else if (AMBARELLA_BOARD_TYPE(system_rev) == AMBARELLA_BOARD_TYPE_IPCAM) {
		ambarella_spi0_cs_pins[4] = -1;
		ambarella_spi0_cs_pins[5] = -1;
		ambarella_spi0_cs_pins[6] = -1;
		ambarella_spi0_cs_pins[7] = -1;

		/* both usb0 and usb1 don't use over-current protection */
		ambarella_board_generic.uhc_use_ocp = 0;

		ambarella_board_generic.sata_power.gpio_id = GPIO(122);
		ambarella_board_generic.sata_power.active_level = GPIO_HIGH;
		ambarella_board_generic.sata_power.active_delay = 1;

		ambarella_eth0_platform_info.mii_id = 1;
		ambarella_eth0_platform_info.phy_id = 0x001cc915;
		ambarella_eth0_platform_info.phy_irq.irq_gpio = GPIO(21);
		ambarella_eth0_platform_info.phy_irq.irq_line = gpio_to_irq(GPIO(21));
		ambarella_eth0_platform_info.phy_irq.irq_type = IRQF_TRIGGER_LOW;
		ambarella_eth0_platform_info.phy_irq.irq_gpio_val = GPIO_LOW;
		ambarella_eth0_platform_info.phy_irq.irq_gpio_mode = GPIO_FUNC_SW_INPUT;
		ambarella_eth0_platform_info.mii_power.gpio_id = GPIO(124);
		ambarella_eth0_platform_info.mii_power.active_level = GPIO_HIGH;
		ambarella_eth0_platform_info.mii_reset.gpio_id = GPIO(125);
		ambarella_eth0_platform_info.mii_reset.active_level = GPIO_LOW;

		ambarella_platform_sd_controller0.max_clock = 24000000;
		ambarella_platform_sd_controller0.max_blk_mask = SD_BLK_SZ_512KB;
		ambarella_platform_sd_controller0.slot[0].fixed_cd = 0;
		ambarella_platform_sd_controller0.slot[1].ext_power.gpio_id = GPIO(110);
		ambarella_platform_sd_controller0.slot[1].ext_power.active_level = GPIO_HIGH;
		ambarella_platform_sd_controller0.slot[1].ext_power.active_delay = 300;
		ambarella_platform_sd_controller0.slot[1].fixed_cd = -1;
		ambarella_platform_sd_controller0.slot[1].gpio_cd.irq_gpio = GPIO(76);
		ambarella_platform_sd_controller0.slot[1].gpio_cd.irq_line = gpio_to_irq(GPIO(76));
		ambarella_platform_sd_controller0.slot[1].gpio_cd.irq_type = IRQ_TYPE_EDGE_BOTH;
		ambarella_platform_sd_controller0.slot[1].gpio_wp.gpio_id = GPIO(75);

		ambarella_platform_sd_controller1.max_clock = 24000000;
		ambarella_platform_sd_controller1.max_blk_mask = SD_BLK_SZ_512KB;
		ambarella_platform_sd_controller1.slot[0].default_caps |=
			(MMC_CAP_8_BIT_DATA | MMC_CAP_BUS_WIDTH_TEST);
		ambarella_platform_sd_controller1.slot[0].ext_power.gpio_id = GPIO(111);
		ambarella_platform_sd_controller1.slot[0].ext_power.active_level = GPIO_HIGH;
		ambarella_platform_sd_controller1.slot[0].ext_power.active_delay = 300;
		ambarella_platform_sd_controller1.slot[0].set_vdd =
			elephant_ipcam_nvr_sdxc_set_vdd;
		ambarella_platform_sd_controller1.slot[0].fixed_cd = -1;
		ambarella_platform_sd_controller1.slot[0].gpio_cd.irq_gpio = GPIO(129);
		ambarella_platform_sd_controller1.slot[0].gpio_cd.irq_line = gpio_to_irq(GPIO(129));
		ambarella_platform_sd_controller1.slot[0].gpio_cd.irq_type = IRQ_TYPE_EDGE_BOTH;
		ambarella_platform_sd_controller1.slot[0].gpio_wp.gpio_id = GPIO(128);

		elephant_board_input_info.pkeymap = elephant_keymap_ipcam_nvr;

		ambarella_platform_adc_temper_controller0.adc_temper_channel = ADC_CH9;
		ambarella_platform_adc_temper_controller0.temper_curve = amb_temper_curve;
		platform_device_register(&ambarella_adc_temper);

		ambarella_board_generic.hdmi_extpower.gpio_id = GPIO(104);
		ambarella_board_generic.hdmi_extpower.active_level = GPIO_HIGH;
		ambarella_board_generic.hdmi_extpower.active_delay = 1;

#if defined(CONFIG_CODEC_AMBARELLA_AK4642)
			ambarella_init_ak4642(0, 0x12, GPIO(123));
#endif

		use_bub_default = 0;
	}

	if (use_bub_default) {
		pr_info("%s: Default BUB config:\n", __func__);
		ambarella_board_generic.lcd_reset.gpio_id = GPIO(46);
		ambarella_board_generic.lcd_reset.active_level = GPIO_LOW;
		ambarella_board_generic.lcd_reset.active_delay = 1;

		ambarella_board_generic.lcd_spi_hw.bus_id = 3;
		ambarella_board_generic.lcd_spi_hw.cs_id = 0;

		ambarella_board_generic.touch_panel_irq.irq_gpio = GPIO(45);
		ambarella_board_generic.touch_panel_irq.irq_line = gpio_to_irq(45);
		ambarella_board_generic.touch_panel_irq.irq_type = IRQF_TRIGGER_FALLING;
		ambarella_board_generic.touch_panel_irq.irq_gpio_val = GPIO_LOW;
		ambarella_board_generic.touch_panel_irq.irq_gpio_mode = GPIO_FUNC_SW_INPUT;

		ambarella_board_generic.vin0_reset.gpio_id = GPIO(12);
		ambarella_board_generic.vin0_reset.active_level = GPIO_LOW;
		ambarella_board_generic.vin0_reset.active_delay = 100;

		ambarella_eth0_platform_info.mii_id = 0;
		ambarella_eth0_platform_info.phy_id = 0x001cc912;

#if defined(CONFIG_TOUCH_AMBARELLA_TM1510)
		ambarella_tm1510_board_info.irq = ambarella_board_generic.touch_panel_irq.irq_line;
		i2c_register_board_info(2, &ambarella_tm1510_board_info, 1);
#endif

		i2c_register_board_info(0, &elephant_board_ext_gpio_info, 1);
		i2c_register_board_info(0, &elephant_board_ext_i2c_info, 1);

		i2c_register_board_info(2, ambarella_board_vin_infos, ARRAY_SIZE(ambarella_board_vin_infos));
	}

	ambarella_platform_ir_controller0.protocol = AMBA_IR_PROTOCOL_SONY;

	platform_add_devices(ambarella_devices, ARRAY_SIZE(ambarella_devices));
	for (i = 0; i < ARRAY_SIZE(ambarella_devices); i++) {
		device_set_wakeup_capable(&ambarella_devices[i]->dev, 1);
		device_set_wakeup_enable(&ambarella_devices[i]->dev, 0);
	}

	for (i = 0; i < ARRAY_SIZE(ambarella_pwm_devices); i++) {
		if (i == 1 && AMBARELLA_BOARD_TYPE(system_rev) ==
			AMBARELLA_BOARD_TYPE_BUB)
			continue;
		if (i == 2 && AMBARELLA_BOARD_TYPE(system_rev) ==
			AMBARELLA_BOARD_TYPE_BUB)
			continue;
		if (i == 4 && AMBARELLA_BOARD_TYPE(system_rev) ==
			AMBARELLA_BOARD_TYPE_EVK)
			continue;
		if (AMBARELLA_BOARD_TYPE(system_rev) ==
			AMBARELLA_BOARD_TYPE_IPCAM) {
			continue;
		}
		ret = platform_device_register(ambarella_pwm_devices[i]);
		if (ret)
			continue;
		device_set_wakeup_capable(&ambarella_pwm_devices[i]->dev, 1);
		device_set_wakeup_enable(&ambarella_pwm_devices[i]->dev, 0);
	}

	spi_register_board_info(ambarella_spi_devices,
		ARRAY_SIZE(ambarella_spi_devices));

	platform_device_register(&elephant_board_input);

	if (use_ambarella_rtc0){
		platform_device_register(&ambarella_rtc0);
		device_set_wakeup_capable(&ambarella_rtc0.dev, 1);
		device_set_wakeup_enable(&ambarella_rtc0.dev, 1);
	}

	if (use_ambarella_wdt0)
		platform_device_register(&ambarella_wdt0);

	i2c_register_board_info(1, &ambarella_board_hdmi_info, 1);
}

/* ==========================================================================*/
MACHINE_START(ELEPHANT, "Elephant")
	.atag_offset	= 0x100,
	.restart_mode	= 's',
	.smp		= smp_ops(ambarella_smp_ops),
	.reserve	= ambarella_memblock_reserve,
	.map_io		= ambarella_map_io,
	.init_irq	= ambarella_init_irq,
	.timer		= &ambarella_timer,
	.init_machine	= ambarella_init_elephant,
#if defined(CONFIG_ARM_GIC)
	.handle_irq	= gic_handle_irq,
#endif
	.restart	= ambarella_restart_machine,
MACHINE_END

