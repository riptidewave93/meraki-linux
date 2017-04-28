/*
 * arch/arm/plat-ambarella/generic/ir.c
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
#include <linux/moduleparam.h>
#include <linux/clk.h>

#include <mach/hardware.h>
#include <plat/ir.h>
#include <plat/clk.h>

/* ==========================================================================*/
#ifdef MODULE_PARAM_PREFIX
#undef MODULE_PARAM_PREFIX
#endif
#define MODULE_PARAM_PREFIX	"ambarella_config."

/* ==========================================================================*/
#if defined(CONFIG_PLAT_AMBARELLA_SUPPORT_HAL)
#else
static struct clk gclk_ir = {
	.parent		= NULL,
	.name		= "gclk_ir",
	.rate		= 0,
	.frac_mode	= 0,
	.ctrl_reg	= PLL_REG_UNAVAILABLE,
	.pres_reg	= PLL_REG_UNAVAILABLE,
	.post_reg	= CG_IR_REG,
	.frac_reg	= PLL_REG_UNAVAILABLE,
	.ctrl2_reg	= PLL_REG_UNAVAILABLE,
	.ctrl3_reg	= PLL_REG_UNAVAILABLE,
	.lock_reg	= PLL_REG_UNAVAILABLE,
	.lock_bit	= 0,
	.divider	= 0,
	.max_divider	= (1 << 24) - 1,
	.extra_scaler	= 0,
	.ops		= &ambarella_rct_pll_ops,
};

static struct clk *ambarella_ir_register_clk(void)
{
	struct clk *pgclk_ir = NULL;

	pgclk_ir = clk_get(NULL, "gclk_ir");
	if (IS_ERR(pgclk_ir)) {
		ambarella_register_clk(&gclk_ir);
		pgclk_ir = &gclk_ir;
		pr_info("SYSCLK:IR[%lu]\n", clk_get_rate(pgclk_ir));
	}

	return pgclk_ir;
}
#endif

static void ambarella_ir_set_pll(void)
{
#if defined(CONFIG_PLAT_AMBARELLA_SUPPORT_HAL)
#else
	ambarella_ir_register_clk();
#endif
}

static u32 ambarella_ir_get_pll(void)
{
#if defined(CONFIG_PLAT_AMBARELLA_SUPPORT_HAL)
	return (u32)amb_get_ir_clock_frequency(HAL_BASE_VP);
#else
	return clk_get_rate(ambarella_ir_register_clk());
#endif
}

/* ==========================================================================*/
struct resource ambarella_ir_resources[] = {
	[0] = {
		.start	= IR_BASE,
		.end	= IR_BASE + 0x0FFF,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= IRIF_IRQ,
		.end	= IRIF_IRQ,
		.flags	= IORESOURCE_IRQ,
	},
	[2] = {
		.start	= IR_IN,
		.end	= IR_IN,
		.flags	= IORESOURCE_IO,
	},
};

struct ambarella_ir_controller ambarella_platform_ir_controller0 = {
	.set_pll		= ambarella_ir_set_pll,
	.get_pll		= ambarella_ir_get_pll,
	.protocol		= AMBA_IR_PROTOCOL_NEC,
	.debug			= 0,
};
#if defined(CONFIG_AMBARELLA_SYS_IR_CALL)
AMBA_IR_PARAM_CALL(ambarella_platform_ir_controller0, 0644);
#endif

struct platform_device ambarella_ir0 = {
	.name		= "ambarella-ir",
	.id		= -1,
	.resource	= ambarella_ir_resources,
	.num_resources	= ARRAY_SIZE(ambarella_ir_resources),
	.dev			= {
		.platform_data		= &ambarella_platform_ir_controller0,
		.dma_mask		= &ambarella_dmamask,
		.coherent_dma_mask	= DMA_BIT_MASK(32),
	}
};

