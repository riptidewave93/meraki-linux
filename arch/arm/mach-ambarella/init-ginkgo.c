/*
 * arch/arm/mach-ambarella/init-ginkgo.c
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
#include <linux/i2c/pca953x.h>

#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/delay.h>

#include <linux/mmc/host.h>

#include <plat/ambinput.h>
#include <plat/ambcache.h>

#include "board-device.h"

#include <plat/dma.h>
#include <plat/clk.h>

#include <linux/input.h>
#include <plat/memconfig.h>
#include <linux/of_platform.h>
/* ==========================================================================*/
static struct platform_device *ambarella_devices[] __initdata = {
	&sc_adc_dn,
#ifdef CONFIG_PLAT_AMBARELLA_SUPPORT_HW_CRYPTO
	&ambarella_crypto,
#endif
	&ambarella_ehci0,
	&ambarella_ohci0,
	&ambarella_eth0,
	&ambarella_fb0,
	&ambarella_fb1,
	&ambarella_i2s0,
	&ambarella_pcm0,
	&ambarella_dummy_codec0,
	&ambarella_dummy_audio_device,
	&ambarella_ambevk_audio_device,
	&ambarella_idc0,
	&ambarella_idc1,
	&ambarella_idc2,
#ifdef _SUPPORT_MMCSD_
	&ambarella_sd0,
#endif
	&ambarella_spi0,
#ifdef CONFIG_PLAT_AMBARELLA_SUPPORT_MMAP_AHB64
	&ambarella_spi1,
#endif
	&ambarella_spi_slave,
	&ambarella_uart,
	&ambarella_uart1,
	&ambarella_uart2,
	&ambarella_uart3,
	&ambarella_udc0,
	&ambarella_wdt0,
	&ambarella_dma,
	&ambarella_nand,
#if defined(CONFIG_PLAT_AMBARELLA_CORTEX)
	&mpcore_wdt,
#endif
};

static struct platform_device *ambarella_pwm_devices[] __initdata = {
#if defined(CONFIG_PWM)
	//&ambarella_pwm_platform_device0,
	//&ambarella_pwm_platform_device1,
	//&ambarella_pwm_platform_device2,
	&ambarella_pwm_platform_device3,
	//&ambarella_pwm_platform_device4,
#endif
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
	},
	{
		.modalias	= "spidev",
		.bus_num	= 1,
		.chip_select	= 0,
	},
};

/* ==========================================================================*/
static struct ambarella_key_table generic_keymap[AMBINPUT_TABLE_SIZE] = {
	{AMBINPUT_VI_KEY,	{.vi_key	= {0,	0,	0}}},
	{AMBINPUT_VI_REL,	{.vi_rel	= {0,	0,	0}}},
	{AMBINPUT_VI_ABS,	{.vi_abs	= {0,	0,	0}}},
	{AMBINPUT_VI_SW,	{.vi_sw		= {0,	0,	0}}},

	{AMBINPUT_IR_KEY,	{.ir_key	= {KEY_POWER,		1,		0x0100bcbd}}},	//POWER
	{AMBINPUT_IR_KEY,	{.ir_key	= {KEY_VOLUMEUP,	1,		0x01000405}}},	//VOLUME_UP
	{AMBINPUT_IR_KEY,	{.ir_key	= {KEY_VOLUMEDOWN,	1,		0x01008485}}},	//VOLUME_DOWN

	{AMBINPUT_ADC_KEY,	{.adc_key	= {KEY_RESERVED,	0,	1,	3100,	4095}}},
	{AMBINPUT_ADC_KEY,	{.adc_key	= {KEY_MODE,		0,	1,	2700,	3100}}},
	{AMBINPUT_ADC_KEY,	{.adc_key	= {KEY_DELETE,		0,	1,	1800,	2400}}},
	{AMBINPUT_ADC_KEY,	{.adc_key	= {KEY_RECORD,		0,	1,	1000,	1350}}},
//	{AMBINPUT_ADC_KEY,	{.adc_key	= {KEY_HP,			0,	1,	300,		800}}},
	{AMBINPUT_ADC_KEY,	{.adc_key	= {KEY_CAMERA,		0,	1,	0,		200}}},

	{AMBINPUT_ADC_KEY,	{.adc_key	= {KEY_RESERVED,	0,	3,	3100,	4095}}},
	{AMBINPUT_ADC_KEY,	{.adc_key	= {KEY_MENU,		0,	3,	2700,	3100}}},
	{AMBINPUT_ADC_KEY,	{.adc_key	= {KEY_RIGHT,		0,	3,	1800,	2400}}},
	{AMBINPUT_ADC_KEY,	{.adc_key	= {KEY_LEFT,			0,	3,	1000,	1350}}},
	{AMBINPUT_ADC_KEY,	{.adc_key	= {KEY_DOWN,		0,	3,	400,		600}}},
	{AMBINPUT_ADC_KEY,	{.adc_key	= {KEY_UP,			0,	3,	0,		200}}},

	{AMBINPUT_GPIO_KEY,	{.gpio_key	= {KEY_POWER,	1,	1,	GPIO(13),	IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING}}},

	{AMBINPUT_END},
};

static struct ambarella_input_board_info ginkgo_board_input_info = {
	.pkeymap		= generic_keymap,
	.pinput_dev		= NULL,
	.pdev			= NULL,

	.abx_max_x		= 4095,
	.abx_max_y		= 4095,
	.abx_max_pressure	= 4095,
	.abx_max_width		= 16,
};

struct platform_device ginkgo_board_input = {
	.name		= "ambarella-input",
	.id		= -1,
	.resource	= NULL,
	.num_resources	= 0,
	.dev		= {
		.platform_data		= &ginkgo_board_input_info,
		.dma_mask		= &ambarella_dmamask,
		.coherent_dma_mask	= DMA_BIT_MASK(32),
	}
};

static struct pca953x_platform_data ginkgo_ipcam_gpio_pca953x_platform_data = {
	.gpio_base		= EXT_GPIO(0),
	.irq_base		= EXT_IRQ(0),
	.invert			= 0,
};

static struct i2c_board_info ginkgo_ipcam_gpio_i2c_board_info = {
	.type			= "pca9539",
	.flags			= 0,
	.addr			= 0x74,
	.platform_data		= &ginkgo_ipcam_gpio_pca953x_platform_data,
	.irq			= GPIO_INT_VEC(11),
};

static void ginkgo_ipcam_sd_set_vdd(u32 vdd)
{
	pr_debug("%s = %dmV\n", __func__, vdd);
	ambarella_gpio_config(GPIO(22), GPIO_FUNC_SW_OUTPUT);
	if (vdd == 1800) {
		ambarella_gpio_set(GPIO(22), 0);
	} else {
		ambarella_gpio_set(GPIO(22), 1);
	}
	msleep(10);
}

static void ginkgo_ipcam_sd_set_bus_timing(u32 timing)
{
	u32 ms_delay_reg;

	pr_debug("%s = %d\n", __func__, timing);
	ms_delay_reg = amba_rct_readl(MS_DELAY_CTRL_REG);
	ms_delay_reg &= 0xE3E0E3E0;
	if (timing == MMC_TIMING_UHS_DDR50) {
		ms_delay_reg |= ((0x7 << 16) | (0x7 << 10));
	}
	amba_rct_writel(MS_DELAY_CTRL_REG, ms_delay_reg);
}

static void ginkgo_ipcam_sdio_set_vdd(u32 vdd)
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

static void ginkgo_ipcam_sdio_set_bus_timing(u32 timing)
{
	u32 ms_delay_reg;

	pr_debug("%s = %d\n", __func__, timing);
	ms_delay_reg = amba_rct_readl(MS_DELAY_CTRL_REG);
	ms_delay_reg &= 0x1C1F1C1F;
	if (timing == MMC_TIMING_UHS_DDR50) {
		ms_delay_reg |= ((0x7 << 21) | (0x7 << 13));
	}
	amba_rct_writel(MS_DELAY_CTRL_REG, ms_delay_reg);
}

static struct i2c_board_info ambarella_s35390a_board_info = {
                I2C_BOARD_INFO("s35390a", 0x30),
};
/* ==========================================================================*/
static void __init ambarella_init_ginkgo(void)
{
	int					i, ret;
	int					use_bub_default = 1;
	u32 core_freq;

	ambarella_init_machine("ginkgo");

#ifdef CONFIG_OUTER_CACHE
	ambcache_l2_enable();
#endif

#ifdef _SUPPORT_MMCSD_
	/* Config SD */
	fio_default_owner = SELECT_FIO_SD;
	//ambarella_platform_sd_controller0.slot[0].fixed_cd = 1;
	ambarella_platform_sd_controller0.max_blk_mask = SD_BLK_SZ_512KB;
	ambarella_platform_sd_controller0.slot[0].gpio_cd.irq_gpio = SMIO_5;
	ambarella_platform_sd_controller0.slot[0].gpio_cd.irq_line = gpio_to_irq(SMIO_5);
	ambarella_platform_sd_controller0.slot[0].gpio_cd.irq_type = IRQ_TYPE_EDGE_BOTH;
	ambarella_platform_sd_controller0.slot[0].gpio_cd.irq_gpio_val = GPIO_HIGH;
	ambarella_platform_sd_controller0.slot[0].gpio_cd.irq_gpio_mode = GPIO_FUNC_SW_INPUT;
	ambarella_platform_sd_controller0.slot[0].cd_delay = 1000;
	//ambarella_platform_sd_controller0.slot[0].check_owner = ginkgo_fio_sd_sd2_check_owner;
	/*ambarella_platform_sd_controller1.max_blk_mask = SD_BLK_SZ_512KB;
	ambarella_platform_sd_controller1.slot[0].gpio_cd.irq_gpio = SMIO_44;
	ambarella_platform_sd_controller1.slot[0].gpio_cd.irq_line = gpio_to_irq(SMIO_44);
	ambarella_platform_sd_controller1.slot[0].gpio_cd.irq_type = IRQ_TYPE_EDGE_BOTH;
	ambarella_platform_sd_controller1.slot[0].cd_delay = 500;
	ambarella_platform_sd_controller1.slot[0].gpio_wp.gpio_id = SMIO_45;
	ambarella_platform_sd_controller1.slot[0].check_owner = ginkgo_fio_sd_sd2_check_owner;*/
#endif
	/* Config ETH */
	ambarella_eth0_platform_info.mii_id = 1;
	ambarella_eth0_platform_info.phy_id = 0x001cc915;
	ambarella_eth0_platform_info.default_tx_ring_size = 64;
	ambarella_eth0_platform_info.default_rx_ring_size = 64;
	ambarella_eth0_platform_info.default_dma_opmode |= ETH_DMA_OPMODE_TSF;

	/* Config Vin */
	ambarella_board_generic.vin1_reset.gpio_id = GPIO(49);
	ambarella_board_generic.vin1_reset.active_level = GPIO_LOW;
	ambarella_board_generic.vin1_reset.active_delay = 1;
#if defined(CONFIG_PLAT_AMBARELLA_SUPPORT_HAL)
	core_freq = get_core_bus_freq_hz();
#else
	core_freq = clk_get_rate(clk_get(NULL, "gclk_core"));
#endif

	/* Config Gyro */
	ambarella_board_generic.gyro_irq.irq_gpio = GPIO(20);
	ambarella_board_generic.gyro_irq.irq_line = gpio_to_irq(GPIO(20));
	ambarella_board_generic.gyro_irq.irq_type = IRQF_TRIGGER_FALLING;
	ambarella_board_generic.gyro_irq.irq_gpio_val = GPIO_LOW;
	ambarella_board_generic.gyro_irq.irq_gpio_mode = GPIO_FUNC_SW_INPUT;

	if (AMBARELLA_BOARD_TYPE(system_rev) == AMBARELLA_BOARD_TYPE_IPCAM) {
		switch (AMBARELLA_BOARD_REV(system_rev)) {
		case 'A':
			ambarella_board_generic.uport_irq.irq_gpio = GPIO(49);
			ambarella_board_generic.uport_irq.irq_line = gpio_to_irq(49);
			ambarella_board_generic.uport_irq.irq_type = IRQ_TYPE_EDGE_BOTH;
			ambarella_board_generic.uport_irq.irq_gpio_val = GPIO_LOW;
			ambarella_board_generic.uport_irq.irq_gpio_mode = GPIO_FUNC_SW_INPUT;

			/* EXT_GPIO(12) => the 4th gpio of second port */
			/*ambarella_board_generic.uport_control.gpio_id = EXT_GPIO(12);
			ambarella_board_generic.uport_control.active_level = GPIO_HIGH;
			ambarella_board_generic.uport_control.active_delay = 1;*/

			ambarella_platform_sd_controller0.max_clock = 128000000;
#if 0	// S2 using 4 bits SD card, so we remove the code as below to config to 8 bits for fix bug failed to detect some SD Card
			ambarella_platform_sd_controller0.slot[0].default_caps |=
				(MMC_CAP_8_BIT_DATA);
			ambarella_platform_sd_controller0.slot[0].private_caps |=
				(AMBA_SD_PRIVATE_CAPS_VDD_18 |
				AMBA_SD_PRIVATE_CAPS_DDR);
			/*ambarella_platform_sd_controller0.slot[0].ext_power.gpio_id = EXT_GPIO(0);
			ambarella_platform_sd_controller0.slot[0].ext_power.active_level = GPIO_HIGH;
			ambarella_platform_sd_controller0.slot[0].ext_power.active_delay = 300;
			ambarella_platform_sd_controller0.slot[0].set_vdd =
				ginkgo_ipcam_sd_set_vdd;*/
			ambarella_platform_sd_controller0.slot[0].set_bus_timing =
				ginkgo_ipcam_sd_set_bus_timing;
			ambarella_platform_sd_controller1.max_clock = 48000000;
			ambarella_platform_sd_controller1.slot[0].private_caps |=
				(AMBA_SD_PRIVATE_CAPS_VDD_18 |
				AMBA_SD_PRIVATE_CAPS_DDR);
			/*ambarella_platform_sd_controller1.slot[0].ext_power.gpio_id = EXT_GPIO(1);
			ambarella_platform_sd_controller1.slot[0].ext_power.active_level = GPIO_HIGH;
			ambarella_platform_sd_controller1.slot[0].ext_power.active_delay = 300;
			ambarella_platform_sd_controller1.slot[0].set_vdd =
				ginkgo_ipcam_sdio_set_vdd;*/
			ambarella_platform_sd_controller1.slot[0].set_bus_timing =
				ginkgo_ipcam_sdio_set_bus_timing;
#endif

			//ambarella_eth0_platform_info.mii_reset.gpio_id = EXT_GPIO(2);
			ambarella_eth0_platform_info.mii_reset.gpio_id = GPIO(54);

			/* Config USB over-curent protection */
			ambarella_board_generic.uhc_use_ocp = 0x1;

			/* Register audio codec
			 * the cs_pin of spi0.4, spi0,5, spi0.6 and spi0.7 are
			 * used as I2S signals, so we need to prevent
			 * them from be modified by SPI driver */
			ambarella_spi0_cs_pins[4] = -1;
			ambarella_spi0_cs_pins[5] = -1;
			ambarella_spi0_cs_pins[6] = -1;
			ambarella_spi0_cs_pins[7] = -1;
#if defined(CONFIG_CODEC_AMBARELLA_AK4642)
			ambarella_init_ak4642(0, 0x12, EXT_GPIO(3));
#endif
/*#if defined(CONFIG_RTC_AMBARELLA_IS112022M)
			i2c_register_board_info(2, &ambarella_isl12022m_board_info, 1);
#else
			platform_device_register(&ambarella_rtc0);
#endif*/
			//i2c_register_board_info(2, &ginkgo_ipcam_gpio_i2c_board_info, 1);
			use_bub_default = 0;
			break;

		default:
			pr_warn("%s: Unknown EVK Rev[%d]\n", __func__,
				AMBARELLA_BOARD_REV(system_rev));
			break;
		}
	}

	if (AMBARELLA_BOARD_TYPE(system_rev) == AMBARELLA_BOARD_TYPE_ATB) {
			ambarella_platform_sd_controller0.max_clock = 128000000;
			ambarella_platform_sd_controller0.slot[0].default_caps |=
				(MMC_CAP_8_BIT_DATA);
			ambarella_platform_sd_controller0.slot[0].private_caps |=
				(AMBA_SD_PRIVATE_CAPS_VDD_18 |
				AMBA_SD_PRIVATE_CAPS_DDR);
			ambarella_platform_sd_controller0.slot[0].set_bus_timing =
				ginkgo_ipcam_sd_set_bus_timing;
			ambarella_platform_sd_controller1.max_clock = 48000000;
			ambarella_platform_sd_controller1.slot[0].private_caps |=
				(AMBA_SD_PRIVATE_CAPS_VDD_18 |
				AMBA_SD_PRIVATE_CAPS_DDR);
			ambarella_platform_sd_controller1.slot[0].set_bus_timing =
				ginkgo_ipcam_sdio_set_bus_timing;

			/* Config USB over-curent protection */
			ambarella_board_generic.uhc_use_ocp = 0x0;

			/* Register audio codec
			 * the cs_pin of spi0.4, spi0,5, spi0.6 and spi0.7 are
			 * used as I2S signals, so we need to prevent
			 * them from be modified by SPI driver */
			ambarella_spi0_cs_pins[1] = -1;
			ambarella_spi0_cs_pins[2] = GPIO(91);
			ambarella_spi0_cs_pins[3] = -1;
			ambarella_spi0_cs_pins[4] = -1;
			ambarella_spi0_cs_pins[5] = -1;
			ambarella_spi0_cs_pins[6] = -1;
			ambarella_spi0_cs_pins[7] = -1;

			platform_device_register(&ambarella_rtc0);

			ambarella_board_generic.vin0_reset.gpio_id = GPIO(54);
			ambarella_board_generic.vin0_reset.active_level = GPIO_LOW;
			ambarella_board_generic.vin0_reset.active_delay = 500;
			//i2c_register_board_info(2, &ginkgo_ipcam_gpio_i2c_board_info, 1);
			ambarella_platform_adc_temper_controller0.adc_temper_channel = ADC_CH7;
			platform_device_register(&ambarella_adc_temper);

			use_bub_default = 0;
	}

	if (use_bub_default) {
		/* Config USB over-curent protection */
		ambarella_board_generic.uhc_use_ocp = (0x1 << 16) | 0x1;
		platform_device_register(&ambarella_rtc0);
	}

	ambarella_platform_ir_controller0.protocol = AMBA_IR_PROTOCOL_PANASONIC;

	platform_add_devices(ambarella_devices, ARRAY_SIZE(ambarella_devices));
	for (i = 0; i < ARRAY_SIZE(ambarella_devices); i++) {
		device_set_wakeup_capable(&ambarella_devices[i]->dev, 1);
		device_set_wakeup_enable(&ambarella_devices[i]->dev, 0);
	}

	for (i = 0; i < ARRAY_SIZE(ambarella_pwm_devices); i++) {
		ret = platform_device_register(ambarella_pwm_devices[i]);
		if (ret)
			continue;
		device_set_wakeup_capable(&ambarella_pwm_devices[i]->dev, 1);
		device_set_wakeup_enable(&ambarella_pwm_devices[i]->dev, 0);
	}

	spi_register_board_info(ambarella_spi_devices,
		ARRAY_SIZE(ambarella_spi_devices));

	i2c_register_board_info(0, ambarella_board_vin_infos,
		ARRAY_SIZE(ambarella_board_vin_infos));
	i2c_register_board_info(1, &ambarella_board_hdmi_info, 1);
	i2c_register_board_info(2, &ambarella_s35390a_board_info, 1);

#if defined(CONFIG_CODEC_AMBARELLA_WM8940)
	ambarella_init_wm8940(0, 0x1A);
#endif
	/* Register devices */
	platform_device_register(&ginkgo_board_input);
	of_platform_populate(NULL, of_default_bus_match_table, NULL, NULL);
}

#ifdef CONFIG_OF
static const char *cyclops_boards_compat[] __initdata = {
	"meraki,cyclops",
	NULL,
};
#endif /* CONFIG_OF */

/* ==========================================================================*/
MACHINE_START(GINKGO, "Ginkgo")
	.atag_offset	= 0x100,
	.restart_mode	= 's',
	.smp		= smp_ops(ambarella_smp_ops),
	.reserve	= ambarella_memblock_reserve,
	.map_io		= ambarella_map_io,
	.init_irq	= ambarella_init_irq,
	.timer		= &ambarella_timer,
	.init_machine	= ambarella_init_ginkgo,
#if defined(CONFIG_ARM_GIC)
	.handle_irq	= gic_handle_irq,
#endif
	.restart	= ambarella_restart_machine,
#ifdef CONFIG_OF
	.dt_compat = cyclops_boards_compat,
#endif
MACHINE_END

