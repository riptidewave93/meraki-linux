/*
 * arch/arm/mach-ambarella/init-durian.c
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
#include <asm/system_info.h>

#include <mach/hardware.h>
#include <mach/init.h>
#include <mach/board.h>

#include <linux/spi/spi.h>
#include <linux/spi/spidev.h>

#include <linux/i2c.h>
#include <linux/i2c/ak4183.h>
#include <linux/i2c/cy8ctmg.h>
#include <linux/i2c/pca953x.h>

#include <sound/ak4642_amb.h>

#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/delay.h>

#include <linux/input.h>
#include <plat/ambinput.h>
#include <plat/dma.h>
#include "board-device.h"
#include <linux/mmc/host.h>

/* ==========================================================================*/
#include <linux/pda_power.h>
static int ambarella_power_is_ac_online(void)
{
	return 1;
}

static struct pda_power_pdata  ambarella_power_supply_info = {
	.is_ac_online    = ambarella_power_is_ac_online,
};

static struct platform_device ambarella_power_supply = {
	.name = "pda-power",
	.id   = -1,
	.dev  = {
		.platform_data = &ambarella_power_supply_info,
	},
};

/* ==========================================================================*/
static struct platform_device *ambarella_devices[] __initdata = {
	&ambarella_adc0,
	&ambarella_fb0,
	&ambarella_fb1,
	&ambarella_i2s0,
	&ambarella_pcm0,
	&ambarella_dummy_codec0,
	&ambarella_dummy_audio_device,
	&ambarella_idc0,
	&ambarella_idc1,
	&ambarella_idc0_mux,
	&ambarella_ir0,
	&ambarella_rtc0,
	&ambarella_sd0,
	&ambarella_sd1,
	&ambarella_spi0,
	&ambarella_uart,
	&ambarella_uart1,
	&ambarella_udc0,
	&ambarella_wdt0,
	&ambarella_pwm_platform_device0,
	&ambarella_pwm_platform_device1,
	&ambarella_pwm_platform_device2,
	&ambarella_pwm_platform_device3,
	&ambarella_pwm_platform_device4,
	&ambarella_power_supply,
	&ambarella_dma,
	&ambarella_nand,
};

/* ==========================================================================*/
static struct spi_board_info ambarella_spi_devices[] = {
	{
		.modalias	= "spidev",
		.bus_num	= 0,
		.chip_select	= 0,
	},
	{
		.modalias	= "spidev",
		.bus_num	= 0,
		.chip_select	= 1,
	},
	{
		.modalias	= "spidev",
		.bus_num	= 0,
		.chip_select	= 2,
	},
	{
		.modalias	= "spidev",
		.bus_num	= 0,
		.chip_select	= 3,
	},
	{
		.modalias	= "spidev",
		.bus_num	= 0,
		.chip_select	= 4,
	},
	{
		.modalias	= "spidev",
		.bus_num	= 0,
		.chip_select	= 5,
	},
	{
		.modalias	= "spidev",
		.bus_num	= 0,
		.chip_select	= 6,
	},
	{
		.modalias	= "spidev",
		.bus_num	= 0,
		.chip_select	= 7,
	}
};

/* ==========================================================================*/
static struct ambarella_key_table durian_keymap[AMBINPUT_TABLE_SIZE] = {
	{AMBINPUT_VI_KEY,	{.vi_key	= {0,	0,	0}}},
	{AMBINPUT_VI_REL,	{.vi_rel	= {0,	0,	0}}},
	{AMBINPUT_VI_ABS,	{.vi_abs	= {0,	0,	0}}},
	{AMBINPUT_VI_SW,	{.vi_sw		= {0,	0,	0}}},

	{AMBINPUT_GPIO_KEY,	{.gpio_key	= {KEY_POWER,	0,	1,	GPIO(13),	IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING}}},

	{AMBINPUT_END}
};

static struct ambarella_input_board_info durian_board_input_info = {
	.pkeymap		= durian_keymap,
	.pinput_dev		= NULL,
	.pdev			= NULL,

	.abx_max_x		= 4095,
	.abx_max_y		= 4095,
	.abx_max_pressure	= 4095,
	.abx_max_width		= 16,
};

static struct platform_device durian_board_input = {
	.name		= "ambarella-input",
	.id		= -1,
	.resource	= NULL,
	.num_resources	= 0,
	.dev		= {
		.platform_data		= &durian_board_input_info,
		.dma_mask		= &ambarella_dmamask,
		.coherent_dma_mask	= DMA_BIT_MASK(32),
	}
};

static void durian_bub_evk_sd_set_vdd(u32 vdd)
{
	pr_debug("%s = %dmV\n", __func__, vdd);
	ambarella_gpio_config(GPIO(22), GPIO_FUNC_SW_OUTPUT);
	if (vdd == 1800) {
		ambarella_gpio_set(GPIO(22), 1);
	} else {
		ambarella_gpio_set(GPIO(22), 0);
	}
	msleep(10);
}

/* ==========================================================================*/
static void __init ambarella_init_durian(void)
{
	int					i;

	ambarella_init_machine("Durian");

	/* Config Board */
	ambarella_board_generic.power_detect.irq_gpio = GPIO(13);
	ambarella_board_generic.power_detect.irq_line = gpio_to_irq(13);
	ambarella_board_generic.power_detect.irq_type = IRQF_TRIGGER_FALLING;
	ambarella_board_generic.power_detect.irq_gpio_val = GPIO_LOW;
	ambarella_board_generic.power_detect.irq_gpio_mode = GPIO_FUNC_SW_INPUT;

	ambarella_board_generic.lcd_reset.gpio_id = GPIO(113);
	ambarella_board_generic.lcd_reset.active_level = GPIO_LOW;
	ambarella_board_generic.lcd_reset.active_delay = 1;

	ambarella_board_generic.lcd_backlight.gpio_id = GPIO(47);
	ambarella_board_generic.lcd_backlight.active_level = GPIO_HIGH;
	ambarella_board_generic.lcd_backlight.active_delay = 1;

	ambarella_board_generic.lcd_spi_hw.bus_id = 0;
	ambarella_board_generic.lcd_spi_hw.cs_id = 5;

	ambarella_board_generic.lcd_spi_cfg.spi_mode = 0;
	ambarella_board_generic.lcd_spi_cfg.cfs_dfs = 16;
	ambarella_board_generic.lcd_spi_cfg.baud_rate = 5000000;
	ambarella_board_generic.lcd_spi_cfg.cs_change = 0;

	ambarella_board_generic.vin0_reset.gpio_id = GPIO(118);
	ambarella_board_generic.vin0_reset.active_level = GPIO_LOW;
	ambarella_board_generic.vin0_reset.active_delay = 100;

	ambarella_board_generic.flash_charge_ready.irq_gpio = GPIO(51);
	ambarella_board_generic.flash_charge_ready.irq_line = gpio_to_irq(51);
	ambarella_board_generic.flash_charge_ready.irq_type = IRQF_TRIGGER_RISING;
	ambarella_board_generic.flash_charge_ready.irq_gpio_val = GPIO_HIGH;
	ambarella_board_generic.flash_charge_ready.irq_gpio_mode = GPIO_FUNC_SW_INPUT;

	ambarella_board_generic.flash_trigger.gpio_id = GPIO(54);
	ambarella_board_generic.flash_trigger.active_level = GPIO_LOW;
	ambarella_board_generic.flash_trigger.active_delay = 1;

	ambarella_board_generic.flash_enable.gpio_id = GPIO(11);
	ambarella_board_generic.flash_enable.active_level = GPIO_LOW;
	ambarella_board_generic.flash_enable.active_delay = 1;

	ambarella_board_generic.gyro_irq.irq_gpio = GPIO(53);
	ambarella_board_generic.gyro_irq.irq_line = gpio_to_irq(53);
	ambarella_board_generic.gyro_irq.irq_type = IRQF_TRIGGER_RISING;
	ambarella_board_generic.gyro_irq.irq_gpio_val = GPIO_HIGH;
	ambarella_board_generic.gyro_irq.irq_gpio_mode = GPIO_FUNC_SW_INPUT;

	ambarella_board_generic.gyro_power.gpio_id = EXT_GPIO(46);
	ambarella_board_generic.gyro_power.active_level = GPIO_HIGH;
	ambarella_board_generic.gyro_power.active_delay = 1;

	/* Config SD */
	fio_default_owner = SELECT_FIO_SD;
	ambarella_platform_sd_controller0.max_blk_mask = SD_BLK_SZ_512KB;
	ambarella_platform_sd_controller1.max_blk_mask = SD_BLK_SZ_512KB;

	if (AMBARELLA_BOARD_TYPE(system_rev) == AMBARELLA_BOARD_TYPE_BUB) {
		ambarella_platform_sd_controller0.max_clock = 48000000;
		ambarella_platform_sd_controller0.slot[0].default_caps |=
			(MMC_CAP_8_BIT_DATA);
		ambarella_platform_sd_controller0.slot[0].set_vdd =
			durian_bub_evk_sd_set_vdd;
		ambarella_platform_sd_controller1.max_clock = 48000000;
	}
	if (AMBARELLA_BOARD_TYPE(system_rev) == AMBARELLA_BOARD_TYPE_EVK) {
		ambarella_platform_sd_controller0.max_clock = 48000000;
		ambarella_platform_sd_controller0.slot[0].default_caps |=
			(MMC_CAP_8_BIT_DATA);
		ambarella_platform_sd_controller0.slot[0].set_vdd =
			durian_bub_evk_sd_set_vdd;
		ambarella_platform_sd_controller0.slot[0].fixed_cd = 0;
		ambarella_platform_sd_controller0.slot[0].fixed_wp = 0;
		ambarella_platform_sd_controller1.max_clock = 48000000;
	}

	/* Register audio codec */
#if defined(CONFIG_CODEC_AMBARELLA_AK4642)
	ambarella_init_ak4642(0, 0x12, GPIO(102));
#endif

	/* Register devices */
	platform_add_devices(ambarella_devices, ARRAY_SIZE(ambarella_devices));
	for (i = 0; i < ARRAY_SIZE(ambarella_devices); i++) {
		device_set_wakeup_capable(&ambarella_devices[i]->dev, 1);
		device_set_wakeup_enable(&ambarella_devices[i]->dev, 0);
	}

	spi_register_board_info(ambarella_spi_devices,
		ARRAY_SIZE(ambarella_spi_devices));

	i2c_register_board_info(0, ambarella_board_vin_infos,
		ARRAY_SIZE(ambarella_board_vin_infos));
	i2c_register_board_info(1, &ambarella_board_hdmi_info, 1);

	platform_device_register(&durian_board_input);
}

/* ==========================================================================*/
MACHINE_START(DURIAN, "Durian")
	.atag_offset	= 0x100,
	.restart_mode	= 's',
	.reserve	= ambarella_memblock_reserve,
	.map_io		= ambarella_map_io,
	.init_irq	= ambarella_init_irq,
	.timer		= &ambarella_timer,
	.init_machine	= ambarella_init_durian,
	.restart	= ambarella_restart_machine,
MACHINE_END

