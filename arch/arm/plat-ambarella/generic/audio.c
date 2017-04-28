/*
 * arch/arm/plat-ambarella/generic/audio.c
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
#include <linux/export.h>
#include <linux/delay.h>
#include <linux/clk.h>

#include <mach/hardware.h>
#include <plat/audio.h>
#include <plat/clk.h>

/* ==========================================================================*/
#if defined(CONFIG_PLAT_AMBARELLA_SUPPORT_HAL)
void rct_set_audio_pll_hal(u32 clksrc, u32 mclk)
{
	int chk_count = 0;
	amb_hal_success_t success;
	amb_clock_source_t clk_src_sel;

	switch (clksrc) {
#if (CHIP_REV == A5S)
		case AMBARELLA_CLKSRC_SP_CLK:
			clk_src_sel = AMB_PLL_REFERENCE_CLOCK_SOURCE_SP_CLK;
			break;
#endif
		case AMBARELLA_CLKSRC_CLK_SI:
#if (CHIP_REV == AMB_S2)
			clk_src_sel = AMB_REFERENCE_CLOCK_SOURCE_CLK_SI;
#else
			clk_src_sel = AMB_PLL_REFERENCE_CLOCK_SOURCE_CLK_SI;
#endif
			break;
		case AMBARELLA_CLKSRC_LVDS_IDSP_SCLK:
#if (CHIP_REV == AMB_S2)
			clk_src_sel = AMB_REFERENCE_CLOCK_SOURCE_LVDS_IDSP_SCLK;
#else
			clk_src_sel = AMB_PLL_REFERENCE_CLOCK_SOURCE_LVDS_IDSP_SCLK;
#endif
			break;
		case AMBARELLA_CLKSRC_EXTERNAL:
			clk_src_sel = AMB_EXTERNAL_CLOCK_SOURCE;
			break;
		default:
		case AMBARELLA_CLKSRC_ONCHIP:
#if (CHIP_REV == AMB_S2)
			clk_src_sel = AMB_REFERENCE_CLOCK_SOURCE_CLK_REF;
#else
			clk_src_sel = AMB_PLL_REFERENCE_CLOCK_SOURCE_CLK_REF;
#endif
			break;
	}

#if (CHIP_REV == AMB_S2)
	if (clk_src_sel == AMB_REFERENCE_CLOCK_SOURCE_CLK_SI ||
		clk_src_sel == AMB_REFERENCE_CLOCK_SOURCE_LVDS_IDSP_SCLK)
#else
	if (clk_src_sel == AMB_PLL_REFERENCE_CLOCK_SOURCE_CLK_SI ||
		clk_src_sel == AMB_PLL_REFERENCE_CLOCK_SOURCE_LVDS_IDSP_SCLK)
#endif
	{
		success = amb_set_audio_clock_source(HAL_BASE_VP, clk_src_sel, mclk);
	} else {
		success = amb_set_audio_clock_source(HAL_BASE_VP, clk_src_sel, 0);
	}

	if (success == AMB_HAL_SUCCESS) {
		if(clk_src_sel != AMB_EXTERNAL_CLOCK_SOURCE) {
			success = amb_set_audio_clock_frequency(HAL_BASE_VP, mclk);
			while (success == AMB_HAL_RETRY) {
				// do something else or sleep
				mdelay(10);
				chk_count ++;
				success = amb_set_audio_clock_frequency(HAL_BASE_VP, mclk);
				if (chk_count == 100) {
					printk("a previous audio pll frequency change request is still outstanding");
					break;
				}
			};
			if (success == AMB_HAL_FAIL)
				printk("new audio pll frequency requested is not supported");
		}
	} else {
		printk("new audio pll reference clock source is not supported");
	}

}

#else

static struct clk gclk_audio = {
	.parent		= NULL,
	.name		= "gclk_audio",
	.rate		= 0,
	.frac_mode	= 1,
	.ctrl_reg	= PLL_AUDIO_CTRL_REG,
	.pres_reg	= SCALER_AUDIO_PRE_REG,
	.post_reg	= SCALER_AUDIO_REG,
	.frac_reg	= PLL_AUDIO_FRAC_REG,
	.ctrl2_reg	= PLL_AUDIO_CTRL2_REG,
	.ctrl3_reg	= PLL_AUDIO_CTRL3_REG,
	.lock_reg	= PLL_LOCK_REG,
	.lock_bit	= 7,
	.divider	= 0,
	.max_divider	= 0,
	.extra_scaler	= 0,
	.ops		= &ambarella_rct_pll_ops,
};

static struct clk *ambarella_audio_register_clk(void)
{
	struct clk *pgclk_audio = NULL;

	pgclk_audio = clk_get(NULL, "gclk_audio");
	if (IS_ERR(pgclk_audio)) {
		ambarella_register_clk(&gclk_audio);
		pgclk_audio = &gclk_audio;
		pr_info("SYSCLK:AUDIO[%lu]\n", clk_get_rate(pgclk_audio));
	}

	return pgclk_audio;
}

#endif

static void set_audio_pll(u32 clksrc, u32 mclk)
{
#if defined(CONFIG_PLAT_AMBARELLA_SUPPORT_HAL)
	rct_set_audio_pll_hal(clksrc, mclk);
#else
	clk_set_rate(ambarella_audio_register_clk(), mclk);
#endif
}

static void aucodec_digitalio_on_0(void)
{
	/* aucodec_digitalio_on */
#if (CHIP_REV == A2S) || (CHIP_REV == A2M)
	unsigned long flags;

	ambarella_gpio_raw_lock(2, &flags);
	amba_setbitsl(GPIO2_AFSEL_REG, (0xf << 18) | (0xf << 13));
	ambarella_gpio_raw_unlock(2, &flags);

#elif (CHIP_REV == A2)
	unsigned long flags;

	ambarella_gpio_raw_lock(2, &flags);
	amba_setbitsl(GPIO2_AFSEL_REG, (0x3 << 15) | (0x3 << 20));
	ambarella_gpio_raw_unlock(2, &flags);

#elif (CHIP_REV == A3)||(CHIP_REV == A5)||(CHIP_REV == A6)|| \
	(CHIP_REV == A5S)||(CHIP_REV == I1)
	unsigned long flags;

	ambarella_gpio_raw_lock(1, &flags);
	amba_clrbitsl(GPIO1_AFSEL_REG, 0x80000000);
	ambarella_gpio_raw_unlock(1, &flags);

	/* GPIO77~GPIO81 program as hardware mode */
	ambarella_gpio_raw_lock(2, &flags);
	amba_setbitsl(GPIO2_AFSEL_REG, 0x0003e000);
	ambarella_gpio_raw_unlock(2, &flags);

#elif (CHIP_REV == AMB_S2)
	unsigned long flags;

	ambarella_gpio_raw_lock(3, &flags);
	amba_clrbitsl(GPIO3_AFSEL_REG, 0x00000030);
	ambarella_gpio_raw_unlock(3, &flags);

	/* GPIO77~GPIO81 program as hardware mode */
	ambarella_gpio_raw_lock(2, &flags);
	amba_setbitsl(GPIO2_AFSEL_REG, 0x0003e000);
	ambarella_gpio_raw_unlock(2, &flags);

#else
	pr_err("aucodec_digitalio_on: Unknown Chip Architecture\n");
#endif
}

static void aucodec_digitalio_on_1(void)
{
	/* aucodec_digitalio_on */
#if (CHIP_REV == A3)||(CHIP_REV == A5)||(CHIP_REV == A6)|| \
	(CHIP_REV == A5S)||(CHIP_REV == I1)
	unsigned long flags;

	ambarella_gpio_raw_lock(1, &flags);
	amba_clrbitsl(GPIO1_AFSEL_REG, 0x80000000);
	ambarella_gpio_raw_unlock(1, &flags);

	/* GPIO77~GPIO78 and GPIO81~GPIO83 program as hardware mode */
	ambarella_gpio_raw_lock(2, &flags);
	amba_setbitsl(GPIO2_AFSEL_REG, 0x000e6000);
	ambarella_gpio_raw_unlock(2, &flags);

#elif (CHIP_REV == AMB_S2)
	unsigned long flags;

	ambarella_gpio_raw_lock(3, &flags);
	amba_clrbitsl(GPIO3_AFSEL_REG, 0x0000000c);
	ambarella_gpio_raw_unlock(3, &flags);

	/* GPIO82~GPIO83 program as hardware mode */
	ambarella_gpio_raw_lock(2, &flags);
	amba_setbitsl(GPIO2_AFSEL_REG, 0x000c0000);
	ambarella_gpio_raw_unlock(2, &flags);

#else
	pr_err("aucodec_digitalio_on: Unknown Chip Architecture\n");
#endif
}

static void aucodec_digitalio_on_2(void)
{
	/* aucodec_digitalio_on */
#if (CHIP_REV == A3)||(CHIP_REV == A5)||(CHIP_REV == A6)|| \
	(CHIP_REV == A5S)||(CHIP_REV == I1)
	unsigned long flags;

	ambarella_gpio_raw_lock(1, &flags);
	amba_clrbitsl(GPIO1_AFSEL_REG, 0x80000000);
	ambarella_gpio_raw_unlock(1, &flags);

	/* GPIO77~GPIO78, GPIO81 and GPIO84~GPIO85 program as hardware mode */
	ambarella_gpio_raw_lock(2, &flags);
	amba_setbitsl(GPIO2_AFSEL_REG, 0x00326000);
	ambarella_gpio_raw_unlock(2, &flags);
#else
	pr_err("aucodec_digitalio_on: Unknown Chip Architecture\n");
#endif
}

static void i2s_channel_select(u32 ch)
{
#if (CHIP_REV == A3)||(CHIP_REV == A5)||(CHIP_REV == A6)|| \
	(CHIP_REV == A5S)||(CHIP_REV == I1)||(CHIP_REV == AMB_S2)
	u32 ch_reg_num;

	ch_reg_num = amba_readl(I2S_CHANNEL_SELECT_REG);

	switch (ch) {
	case 2:
		if (ch_reg_num != 0)
			amba_writel(I2S_CHANNEL_SELECT_REG, I2S_2CHANNELS_ENB);
		break;
	case 4:
		if (ch_reg_num != 1)
			amba_writel(I2S_CHANNEL_SELECT_REG, I2S_4CHANNELS_ENB);
		break;
	case 6:
		if (ch_reg_num != 2)
			amba_writel(I2S_CHANNEL_SELECT_REG, I2S_6CHANNELS_ENB);
		break;
	default:
		printk("Don't support %d channels\n", ch);
		break;
	}
#endif
}

static struct ambarella_i2s_controller ambarella_platform_i2s_controller0 = {
	.set_audio_pll		= set_audio_pll,
	.aucodec_digitalio_0	= aucodec_digitalio_on_0,
	.aucodec_digitalio_1	= aucodec_digitalio_on_1,
	.aucodec_digitalio_2	= aucodec_digitalio_on_2,
	.channel_select		= i2s_channel_select,
};

static struct resource ambarella_i2s0_resources[] = {
	[0] = {
		.start	= I2S_BASE,
		.end	= I2S_BASE + 0x0FFF,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= I2STX_IRQ,
		.end	= I2SRX_IRQ,
		.flags	= IORESOURCE_IRQ,
	},
};

struct platform_device ambarella_i2s0 = {
	.name		= "ambarella-i2s",
	.id		= 0,
	.resource	= ambarella_i2s0_resources,
	.num_resources	= ARRAY_SIZE(ambarella_i2s0_resources),
	.dev		= {
		.platform_data		= &ambarella_platform_i2s_controller0,
		.dma_mask		= &ambarella_dmamask,
		.coherent_dma_mask	= DMA_BIT_MASK(32),
	}
};

struct platform_device ambarella_pcm0 = {
	.name		= "ambarella-pcm-audio",
	.id		= -1,
};

struct platform_device ambarella_dummy_codec0 = {
	.name		= "ambdummy-codec",
	.id		= -1,
};

struct platform_device ambarella_ambevk_audio_device = {
	.name	= "snd_soc_card_ambevk",
	.id		= -1,
};

struct platform_device ambarella_ipcam_audio_device = {
	.name	= "snd_soc_card_ipcam",
	.id		= -1,
};

struct platform_device ambarella_i1evk_audio_device = {
	.name	= "snd_soc_card_i1evk",
	.id		= -1,
};

struct platform_device ambarella_dummy_audio_device = {
	.name	= "snd_soc_card_ambdummy",
	.id		= -1,
};

struct platform_device ambarella_a5spa2_audio_device = {
	.name	= "snd_soc_card_a5sevk",
	.id		= -1,
};

struct platform_device ambarella_ambxman_audio_device = {
	.name	= "snd_soc_card_ambxman",
	.id		= -1,
};

struct platform_device ambarella_ambcloudcam_audio_device = {
	.name	= "snd_soc_card_ambcloudcam",
	.id	= -1,
};

/* ==========================================================================*/
static struct srcu_notifier_head audio_notifier_list;
static struct notifier_block audio_notify;
static struct ambarella_i2s_interface audio_i2s_intf;

struct ambarella_i2s_interface get_audio_i2s_interface(void)
{
	return audio_i2s_intf;
}
EXPORT_SYMBOL(get_audio_i2s_interface);

static int audio_notify_transition(struct notifier_block *nb,
		unsigned long val, void *data)
{
	switch(val) {
	case AUDIO_NOTIFY_INIT:
		audio_i2s_intf.state = AUDIO_NOTIFY_INIT;
		memcpy(&audio_i2s_intf, data,
			sizeof(struct ambarella_i2s_interface));
		break;

	case AUDIO_NOTIFY_SETHWPARAMS:
		audio_i2s_intf.state = AUDIO_NOTIFY_SETHWPARAMS;
		memcpy(&audio_i2s_intf, data,
			sizeof(struct ambarella_i2s_interface));
		break;

	case AUDIO_NOTIFY_REMOVE:
		memset(&audio_i2s_intf, 0,
			sizeof(struct ambarella_i2s_interface));
		audio_i2s_intf.state = AUDIO_NOTIFY_REMOVE;
		break;
	default:
		audio_i2s_intf.state = AUDIO_NOTIFY_UNKNOWN;
		break;
	}

	return 0;
}

void ambarella_audio_notify_transition (
	struct ambarella_i2s_interface *data, unsigned int type)
{
	srcu_notifier_call_chain(&audio_notifier_list, type, data);
}
EXPORT_SYMBOL(ambarella_audio_notify_transition);

int ambarella_audio_register_notifier(struct notifier_block *nb)
{
	return srcu_notifier_chain_register( &audio_notifier_list, nb);
}
EXPORT_SYMBOL(ambarella_audio_register_notifier);


int ambarella_audio_unregister_notifier(struct notifier_block *nb)
{
	return srcu_notifier_chain_unregister(&audio_notifier_list, nb);
}
EXPORT_SYMBOL(ambarella_audio_unregister_notifier);


int __init ambarella_init_audio(void)
{
	int retval = 0;

	srcu_init_notifier_head(&audio_notifier_list);

	memset(&audio_i2s_intf, 0, sizeof(struct ambarella_i2s_interface));
	audio_i2s_intf.state = AUDIO_NOTIFY_UNKNOWN;

	audio_notify.notifier_call = audio_notify_transition;
	retval = ambarella_audio_register_notifier(&audio_notify);

	return retval;
}

