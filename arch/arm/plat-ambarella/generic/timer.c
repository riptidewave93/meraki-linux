/*
 * arch/arm/plat-ambarella/generic/timer.c
 *
 * Author: Anthony Ginger <hfjiang@ambarella.com>
 *
 * Copyright (C) 2004-2012, Ambarella, Inc.
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
 * Default clock is from APB.
 * Timer 3 is used as clock_event_device.
 * Timer 2 is used as free-running clocksource
 *         if CONFIG_AMBARELLA_SUPPORT_CLOCKSOURCE is defined
 * Timer 1 is not used.
 */

#include <linux/interrupt.h>
#include <linux/clockchips.h>
#include <linux/delay.h>
#include <linux/clk.h>

#include <asm/mach/irq.h>
#include <asm/mach/time.h>
#include <asm/smp_twd.h>
#include <asm/sched_clock.h>
#include <asm/mach-types.h>

#include <mach/hardware.h>
#include <plat/timer.h>
#include <plat/clk.h>

/* ==========================================================================*/
#if defined(CONFIG_PLAT_AMBARELLA_SUPPORT_HAL)
#else
static struct clk pll_out_core = {
	.parent		= NULL,
	.name		= "pll_out_core",
	.rate		= 0,
	.frac_mode	= 0,
	.ctrl_reg	= PLL_CORE_CTRL_REG,
	.pres_reg	= PLL_REG_UNAVAILABLE,
	.post_reg	= PLL_REG_UNAVAILABLE,
	.frac_reg	= PLL_CORE_FRAC_REG,
	.ctrl2_reg	= PLL_CORE_CTRL2_REG,
	.ctrl3_reg	= PLL_CORE_CTRL3_REG,
	.lock_reg	= PLL_LOCK_REG,
	.lock_bit	= 6,
	.divider	= 0,
	.max_divider	= 0,
	.extra_scaler	= 0,
	.ops		= &ambarella_rct_pll_ops,
};

static struct clk pll_out_idsp = {
	.parent		= NULL,
	.name		= "pll_out_idsp",
	.rate		= 0,
	.frac_mode	= 0,
	.ctrl_reg	= PLL_IDSP_CTRL_REG,
	.pres_reg	= PLL_REG_UNAVAILABLE,
	.post_reg	= PLL_REG_UNAVAILABLE,
	.frac_reg	= PLL_IDSP_FRAC_REG,
	.ctrl2_reg	= PLL_IDSP_CTRL2_REG,
	.ctrl3_reg	= PLL_IDSP_CTRL3_REG,
	.lock_reg	= PLL_LOCK_REG,
	.lock_bit	= 4,
	.divider	= 0,
	.max_divider	= 0,
	.extra_scaler	= 0,
	.ops		= &ambarella_rct_pll_ops,
};

static unsigned long ambarella_rct_core_get_rate(struct clk *c)
{
	u32 rate;
	u32 divider;

	if (!c->parent || !c->parent->ops || !c->parent->ops->get_rate) {
		goto ambarella_rct_core_get_rate_exit;
	}
	rate = c->parent->ops->get_rate(c->parent);
	if (c->post_reg != PLL_REG_UNAVAILABLE) {
		divider = amba_rct_readl(c->post_reg);
		if (divider) {
			rate /= divider;
		}
	}
	if (c->divider) {
		rate /= c->divider;
	}
#if (CHIP_REV == I1)
	if (amba_rct_readl(RCT_REG(0x24C))) {
		rate <<= 1;
	}
#endif
	c->rate = rate;

ambarella_rct_core_get_rate_exit:
	return c->rate;
}

struct clk_ops ambarella_rct_core_ops = {
	.enable		= NULL,
	.disable	= NULL,
	.get_rate	= ambarella_rct_core_get_rate,
	.round_rate	= NULL,
	.set_rate	= NULL,
	.set_parent	= NULL,
};

static struct clk gclk_core = {
	.parent		= &pll_out_core,
	.name		= "gclk_core",
	.rate		= 0,
	.frac_mode	= 0,
	.ctrl_reg	= PLL_REG_UNAVAILABLE,
	.pres_reg	= PLL_REG_UNAVAILABLE,
	.post_reg	= SCALER_CORE_POST_REG,
	.frac_reg	= PLL_REG_UNAVAILABLE,
	.ctrl2_reg	= PLL_REG_UNAVAILABLE,
	.ctrl3_reg	= PLL_REG_UNAVAILABLE,
	.lock_reg	= PLL_REG_UNAVAILABLE,
	.lock_bit	= 0,
#if ((CHIP_REV == A5S) || (CHIP_REV == AMB_S2))
	.divider	= 1,
#else
	.divider	= 2,
#endif
	.max_divider	= (1 << 4) - 1,
	.extra_scaler	= 0,
	.ops		= &ambarella_rct_core_ops,
};

static unsigned long ambarella_rct_axb_get_rate(struct clk *c)
{
	u32 rate;
	u32 divider;

	if (!c->parent || !c->parent->ops || !c->parent->ops->get_rate) {
		goto ambarella_rct_axb_get_rate_exit;
	}
	rate = c->parent->ops->get_rate(c->parent);
	if (c->post_reg != PLL_REG_UNAVAILABLE) {
		divider = amba_rct_readl(c->post_reg);
		if (divider) {
			rate /= divider;
		}
	}
	if (c->divider) {
		rate /= c->divider;
	}
#if (CHIP_REV == AMB_S2)
	if (amba_rct_readl(RCT_REG(0x24C))) {
		rate <<= 1;
	}
#endif
	c->rate = rate;

ambarella_rct_axb_get_rate_exit:
	return c->rate;
}

struct clk_ops ambarella_rct_axb_ops = {
	.enable		= NULL,
	.disable	= NULL,
	.get_rate	= ambarella_rct_axb_get_rate,
	.round_rate	= NULL,
	.set_rate	= NULL,
	.set_parent	= NULL,
};

static struct clk gclk_ahb = {
	.parent		= &pll_out_core,
	.name		= "gclk_ahb",
	.rate		= 0,
	.frac_mode	= 0,
	.ctrl_reg	= PLL_REG_UNAVAILABLE,
	.pres_reg	= PLL_REG_UNAVAILABLE,
	.post_reg	= SCALER_CORE_POST_REG,
	.frac_reg	= PLL_REG_UNAVAILABLE,
	.ctrl2_reg	= PLL_REG_UNAVAILABLE,
	.ctrl3_reg	= PLL_REG_UNAVAILABLE,
	.lock_reg	= PLL_REG_UNAVAILABLE,
	.lock_bit	= 0,
#if (CHIP_REV == A5S)
	.divider	= 1,
#else
	.divider	= 2,
#endif
	.max_divider	= (1 << 4) - 1,
	.extra_scaler	= 0,
	.ops		= &ambarella_rct_core_ops,
};

static struct clk gclk_apb = {
	.parent		= &pll_out_core,
	.name		= "gclk_apb",
	.rate		= 0,
	.frac_mode	= 0,
	.ctrl_reg	= PLL_REG_UNAVAILABLE,
	.pres_reg	= PLL_REG_UNAVAILABLE,
#if (CHIP_REV == A8)
	.post_reg	= PLL_REG_UNAVAILABLE,
#else
	.post_reg	= SCALER_CORE_POST_REG,
#endif
	.frac_reg	= PLL_REG_UNAVAILABLE,
	.ctrl2_reg	= PLL_REG_UNAVAILABLE,
	.ctrl3_reg	= PLL_REG_UNAVAILABLE,
	.lock_reg	= PLL_REG_UNAVAILABLE,
	.lock_bit	= 0,
#if (CHIP_REV == A5S)
	.divider	= 2,
#else
	.divider	= 4,
#endif
	.max_divider	= (1 << 4) - 1,
	.extra_scaler	= 0,
	.ops		= &ambarella_rct_core_ops,
};
#endif

/* ==========================================================================*/
static u32 ambarella_timer_get_pll(void)
{
#if defined(CONFIG_PLAT_AMBARELLA_SUPPORT_HAL)
	return get_apb_bus_freq_hz();
#else
	return clk_get_rate(clk_get(NULL, "gclk_apb"));
#endif
}

#define AMBARELLA_TIMER_FREQ			(ambarella_timer_get_pll())
#define AMBARELLA_TIMER_RATING			(300)

struct ambarella_timer_pm_info {
	u32 timer_clk;
	u32 timer_ctr_reg;
	u32 timer_ce_status_reg;
	u32 timer_ce_reload_reg;
	u32 timer_ce_match1_reg;
	u32 timer_ce_match2_reg;
#if defined(CONFIG_AMBARELLA_SUPPORT_CLOCKSOURCE)
	u32 timer_cs_status_reg;
	u32 timer_cs_reload_reg;
	u32 timer_cs_match1_reg;
	u32 timer_cs_match2_reg;
#endif
};
struct ambarella_timer_pm_info ambarella_timer_pm;
static struct clock_event_device ambarella_clkevt;
static struct irqaction ambarella_ce_timer_irq;

/* ==========================================================================*/
#if !defined(CONFIG_MACH_HYACINTH_0) && !defined(CONFIG_MACH_HYACINTH_1)
#if (INTERVAL_TIMER_INSTANCES == 8) && defined(CONFIG_PLAT_AMBARELLA_CORTEX)
#define AMBARELLA_CE_TIMER_STATUS_REG		TIMER8_STATUS_REG
#define AMBARELLA_CE_TIMER_RELOAD_REG		TIMER8_RELOAD_REG
#define AMBARELLA_CE_TIMER_MATCH1_REG		TIMER8_MATCH1_REG
#define AMBARELLA_CE_TIMER_MATCH2_REG		TIMER8_MATCH2_REG
#define AMBARELLA_CE_TIMER_IRQ			TIMER8_IRQ
#define AMBARELLA_CE_TIMER_CTR_EN		TIMER_CTR_EN8
#define AMBARELLA_CE_TIMER_CTR_OF		TIMER_CTR_OF8
#define AMBARELLA_CE_TIMER_CTR_CSL		TIMER_CTR_CSL8
#define AMBARELLA_CE_TIMER_CTR_MASK		0xF0000000
#else
#define AMBARELLA_CE_TIMER_STATUS_REG		TIMER3_STATUS_REG
#define AMBARELLA_CE_TIMER_RELOAD_REG		TIMER3_RELOAD_REG
#define AMBARELLA_CE_TIMER_MATCH1_REG		TIMER3_MATCH1_REG
#define AMBARELLA_CE_TIMER_MATCH2_REG		TIMER3_MATCH2_REG
#define AMBARELLA_CE_TIMER_IRQ			TIMER3_IRQ
#define AMBARELLA_CE_TIMER_CTR_EN		TIMER_CTR_EN3
#define AMBARELLA_CE_TIMER_CTR_OF		TIMER_CTR_OF3
#define AMBARELLA_CE_TIMER_CTR_CSL		TIMER_CTR_CSL3
#define AMBARELLA_CE_TIMER_CTR_MASK		0x00000F00
#endif //INTERVAL_TIMER_INSTANCES
#else /* defined(CONFIG_MACH_HYACINTH_0) || defined(CONFIG_MACH_HYACINTH_1) */
#define AMBARELLA_CE_TIMER_AXI0_STATUS_REG	TIMER6_STATUS_REG
#define AMBARELLA_CE_TIMER_AXI0_RELOAD_REG	TIMER6_RELOAD_REG
#define AMBARELLA_CE_TIMER_AXI0_MATCH1_REG	TIMER6_MATCH1_REG
#define AMBARELLA_CE_TIMER_AXI0_MATCH2_REG	TIMER6_MATCH2_REG
#define AMBARELLA_CE_TIMER_AXI0_IRQ		TIMER6_IRQ
#define AMBARELLA_CE_TIMER_AXI0_CTR_EN		TIMER_CTR_EN6
#define AMBARELLA_CE_TIMER_AXI0_CTR_OF		TIMER_CTR_OF6
#define AMBARELLA_CE_TIMER_AXI0_CTR_CSL		TIMER_CTR_CSL6
#define AMBARELLA_CE_TIMER_AXI0_CTR_MASK	0x00F00000

#define AMBARELLA_CE_TIMER_AXI1_STATUS_REG	TIMER8_STATUS_REG
#define AMBARELLA_CE_TIMER_AXI1_RELOAD_REG	TIMER8_RELOAD_REG
#define AMBARELLA_CE_TIMER_AXI1_MATCH1_REG	TIMER8_MATCH1_REG
#define AMBARELLA_CE_TIMER_AXI1_MATCH2_REG	TIMER8_MATCH2_REG
#define AMBARELLA_CE_TIMER_AXI1_IRQ		TIMER8_IRQ
#define AMBARELLA_CE_TIMER_AXI1_CTR_EN		TIMER_CTR_EN8
#define AMBARELLA_CE_TIMER_AXI1_CTR_OF		TIMER_CTR_OF8
#define AMBARELLA_CE_TIMER_AXI1_CTR_CSL		TIMER_CTR_CSL8
#define AMBARELLA_CE_TIMER_AXI1_CTR_MASK	0xF0000000
#endif /* defined(CONFIG_MACH_HYACINTH_0) || defined(CONFIG_MACH_HYACINTH_1) */

static inline void ambarella_ce_timer_disable(void)
{
#if !defined(CONFIG_MACH_HYACINTH_0) && !defined(CONFIG_MACH_HYACINTH_1)
	amba_clrbitsl(TIMER_CTR_REG, AMBARELLA_CE_TIMER_CTR_EN);
#else /* defined(CONFIG_MACH_HYACINTH_0) || defined(CONFIG_MACH_HYACINTH_1) */
	if (machine_is_hyacinth_0())
		amba_clrbitsl(TIMER_CTR_REG, AMBARELLA_CE_TIMER_AXI0_CTR_EN);
	else if (machine_is_hyacinth_1())
		amba_clrbitsl(TIMER_CTR_REG, AMBARELLA_CE_TIMER_AXI1_CTR_EN);
#endif /* defined(CONFIG_MACH_HYACINTH_0) || defined(CONFIG_MACH_HYACINTH_1) */
}

static inline void ambarella_ce_timer_enable(void)
{
#if !defined(CONFIG_MACH_HYACINTH_0) && !defined(CONFIG_MACH_HYACINTH_1)
	amba_setbitsl(TIMER_CTR_REG, AMBARELLA_CE_TIMER_CTR_EN);
#else /* defined(CONFIG_MACH_HYACINTH_0) || defined(CONFIG_MACH_HYACINTH_1) */
	if (machine_is_hyacinth_0())
		amba_setbitsl(TIMER_CTR_REG, AMBARELLA_CE_TIMER_AXI0_CTR_EN);
	else if (machine_is_hyacinth_1())
		amba_setbitsl(TIMER_CTR_REG, AMBARELLA_CE_TIMER_AXI1_CTR_EN);
#endif /* defined(CONFIG_MACH_HYACINTH_0) || defined(CONFIG_MACH_HYACINTH_1) */
}

static inline void ambarella_ce_timer_misc(void)
{
#if !defined(CONFIG_MACH_HYACINTH_0) && !defined(CONFIG_MACH_HYACINTH_1)
	amba_setbitsl(TIMER_CTR_REG, AMBARELLA_CE_TIMER_CTR_OF);
	amba_clrbitsl(TIMER_CTR_REG, AMBARELLA_CE_TIMER_CTR_CSL);
#else /* defined(CONFIG_MACH_HYACINTH_0) || defined(CONFIG_MACH_HYACINTH_1) */
	if (machine_is_hyacinth_0()) {
		amba_setbitsl(TIMER_CTR_REG, AMBARELLA_CE_TIMER_AXI0_CTR_OF);
		amba_clrbitsl(TIMER_CTR_REG, AMBARELLA_CE_TIMER_AXI0_CTR_CSL);
	}
	else if (machine_is_hyacinth_1()) {
		amba_setbitsl(TIMER_CTR_REG, AMBARELLA_CE_TIMER_AXI1_CTR_OF);
		amba_clrbitsl(TIMER_CTR_REG, AMBARELLA_CE_TIMER_AXI1_CTR_CSL);
	}
#endif /* defined(CONFIG_MACH_HYACINTH_0) || defined(CONFIG_MACH_HYACINTH_1) */
}

static inline void ambarella_ce_timer_set_periodic(void)
{
	u32					cnt;

	cnt = AMBARELLA_TIMER_FREQ / HZ;
#if !defined(CONFIG_MACH_HYACINTH_0) && !defined(CONFIG_MACH_HYACINTH_1)
	amba_writel(AMBARELLA_CE_TIMER_STATUS_REG, cnt);
	amba_writel(AMBARELLA_CE_TIMER_RELOAD_REG, cnt);
	amba_writel(AMBARELLA_CE_TIMER_MATCH1_REG, 0x0);
	amba_writel(AMBARELLA_CE_TIMER_MATCH2_REG, 0x0);
#else /* defined(CONFIG_MACH_HYACINTH_0) || defined(CONFIG_MACH_HYACINTH_1) */
	if (machine_is_hyacinth_0()) {
		amba_writel(AMBARELLA_CE_TIMER_AXI0_STATUS_REG, cnt);
		amba_writel(AMBARELLA_CE_TIMER_AXI0_RELOAD_REG, cnt);
		amba_writel(AMBARELLA_CE_TIMER_AXI0_MATCH1_REG, 0x0);
		amba_writel(AMBARELLA_CE_TIMER_AXI0_MATCH2_REG, 0x0);
	}
	else if (machine_is_hyacinth_1()) {
		amba_writel(AMBARELLA_CE_TIMER_AXI1_STATUS_REG, cnt);
		amba_writel(AMBARELLA_CE_TIMER_AXI1_RELOAD_REG, cnt);
		amba_writel(AMBARELLA_CE_TIMER_AXI1_MATCH1_REG, 0x0);
		amba_writel(AMBARELLA_CE_TIMER_AXI1_MATCH2_REG, 0x0);
	}
#endif /* defined(CONFIG_MACH_HYACINTH_0) || defined(CONFIG_MACH_HYACINTH_1) */
	ambarella_ce_timer_misc();
}

static inline void ambarella_ce_timer_set_oneshot(void)
{
#if !defined(CONFIG_MACH_HYACINTH_0) && !defined(CONFIG_MACH_HYACINTH_1)
	amba_writel(AMBARELLA_CE_TIMER_STATUS_REG, 0x0);
	amba_writel(AMBARELLA_CE_TIMER_RELOAD_REG, 0xffffffff);
	amba_writel(AMBARELLA_CE_TIMER_MATCH1_REG, 0x0);
	amba_writel(AMBARELLA_CE_TIMER_MATCH2_REG, 0x0);
#else /* defined(CONFIG_MACH_HYACINTH_0) || defined(CONFIG_MACH_HYACINTH_1) */
	if (machine_is_hyacinth_0()) {
		amba_writel(AMBARELLA_CE_TIMER_AXI0_STATUS_REG, 0x0);
		amba_writel(AMBARELLA_CE_TIMER_AXI0_RELOAD_REG, 0xffffffff);
		amba_writel(AMBARELLA_CE_TIMER_AXI0_MATCH1_REG, 0x0);
		amba_writel(AMBARELLA_CE_TIMER_AXI0_MATCH2_REG, 0x0);
	}
	else if (machine_is_hyacinth_1()) {
		amba_writel(AMBARELLA_CE_TIMER_AXI1_STATUS_REG, 0x0);
		amba_writel(AMBARELLA_CE_TIMER_AXI1_RELOAD_REG, 0xffffffff);
		amba_writel(AMBARELLA_CE_TIMER_AXI1_MATCH1_REG, 0x0);
		amba_writel(AMBARELLA_CE_TIMER_AXI1_MATCH2_REG, 0x0);
	}
#endif /* defined(CONFIG_MACH_HYACINTH_0) || defined(CONFIG_MACH_HYACINTH_1) */
	ambarella_ce_timer_misc();
}

static void ambarella_ce_timer_set_mode(enum clock_event_mode mode,
	struct clock_event_device *evt)
{
	switch (mode) {
	case CLOCK_EVT_MODE_PERIODIC:
		ambarella_ce_timer_disable();
		ambarella_ce_timer_set_periodic();
		ambarella_ce_timer_enable();
		break;
	case CLOCK_EVT_MODE_ONESHOT:
		ambarella_ce_timer_disable();
		ambarella_ce_timer_set_oneshot();
		ambarella_ce_timer_enable();
		break;
	case CLOCK_EVT_MODE_UNUSED:
		remove_irq(ambarella_clkevt.irq, &ambarella_ce_timer_irq);
	case CLOCK_EVT_MODE_SHUTDOWN:
		ambarella_ce_timer_disable();
		break;
	case CLOCK_EVT_MODE_RESUME:
		break;
	}
	pr_debug("%s:%d\n", __func__, mode);
}

static int ambarella_ce_timer_set_next_event(unsigned long delta,
	struct clock_event_device *dev)
{
#if !defined(CONFIG_MACH_HYACINTH_0) && !defined(CONFIG_MACH_HYACINTH_1)
	amba_writel(AMBARELLA_CE_TIMER_STATUS_REG, delta);
#else /* defined(CONFIG_MACH_HYACINTH_0) || defined(CONFIG_MACH_HYACINTH_1) */
	if (machine_is_hyacinth_0())
		amba_writel(AMBARELLA_CE_TIMER_AXI0_STATUS_REG, delta);
	else if (machine_is_hyacinth_1())
		amba_writel(AMBARELLA_CE_TIMER_AXI1_STATUS_REG, delta);
#endif /* defined(CONFIG_MACH_HYACINTH_0) || defined(CONFIG_MACH_HYACINTH_1) */

	return 0;
}

static struct clock_event_device ambarella_clkevt = {
	.name		= "ambarella-clkevt",
	.features	= CLOCK_EVT_FEAT_PERIODIC | CLOCK_EVT_FEAT_ONESHOT,
	.rating		= AMBARELLA_TIMER_RATING,
	.set_next_event	= ambarella_ce_timer_set_next_event,
	.set_mode	= ambarella_ce_timer_set_mode,
	.mode		= CLOCK_EVT_MODE_UNUSED,
#if !defined(CONFIG_MACH_HYACINTH_0) && !defined(CONFIG_MACH_HYACINTH_1)
	.irq		= AMBARELLA_CE_TIMER_IRQ,
#endif
};

static irqreturn_t ambarella_ce_timer_interrupt(int irq, void *dev_id)
{
	ambarella_clkevt.event_handler(&ambarella_clkevt);

	return IRQ_HANDLED;
}

static struct irqaction ambarella_ce_timer_irq = {
	.name		= "ambarella-ce-timer",
	.flags		= IRQF_DISABLED | IRQF_TIMER | IRQF_TRIGGER_RISING,
	.handler	= ambarella_ce_timer_interrupt,
};

/* ==========================================================================*/
#if defined(CONFIG_AMBARELLA_SUPPORT_CLOCKSOURCE)
#if !defined(CONFIG_MACH_HYACINTH_0) && !defined(CONFIG_MACH_HYACINTH_1)
#if (INTERVAL_TIMER_INSTANCES == 8) && defined(CONFIG_PLAT_AMBARELLA_CORTEX)
#define AMBARELLA_CS_TIMER_STATUS_REG		TIMER7_STATUS_REG
#define AMBARELLA_CS_TIMER_RELOAD_REG		TIMER7_RELOAD_REG
#define AMBARELLA_CS_TIMER_MATCH1_REG		TIMER7_MATCH1_REG
#define AMBARELLA_CS_TIMER_MATCH2_REG		TIMER7_MATCH2_REG
#define AMBARELLA_CS_TIMER_CTR_EN		TIMER_CTR_EN7
#define AMBARELLA_CS_TIMER_CTR_OF		TIMER_CTR_OF7
#define AMBARELLA_CS_TIMER_CTR_CSL		TIMER_CTR_CSL7
#define AMBARELLA_CS_TIMER_CTR_MASK		0x0F000000
#else
#define AMBARELLA_CS_TIMER_STATUS_REG		TIMER2_STATUS_REG
#define AMBARELLA_CS_TIMER_RELOAD_REG		TIMER2_RELOAD_REG
#define AMBARELLA_CS_TIMER_MATCH1_REG		TIMER2_MATCH1_REG
#define AMBARELLA_CS_TIMER_MATCH2_REG		TIMER2_MATCH2_REG
#define AMBARELLA_CS_TIMER_CTR_EN		TIMER_CTR_EN2
#define AMBARELLA_CS_TIMER_CTR_OF		TIMER_CTR_OF2
#define AMBARELLA_CS_TIMER_CTR_CSL		TIMER_CTR_CSL2
#define AMBARELLA_CS_TIMER_CTR_MASK		0x000000F0
#endif //INTERVAL_TIMER_INSTANCES
#else /* defined(CONFIG_MACH_HYACINTH_0) || defined(CONFIG_MACH_HYACINTH_1) */
#define AMBARELLA_CS_TIMER_AXI0_STATUS_REG	TIMER5_STATUS_REG
#define AMBARELLA_CS_TIMER_AXI0_RELOAD_REG	TIMER5_RELOAD_REG
#define AMBARELLA_CS_TIMER_AXI0_MATCH1_REG	TIMER5_MATCH1_REG
#define AMBARELLA_CS_TIMER_AXI0_MATCH2_REG	TIMER5_MATCH2_REG
#define AMBARELLA_CS_TIMER_AXI0_CTR_EN		TIMER_CTR_EN5
#define AMBARELLA_CS_TIMER_AXI0_CTR_OF		TIMER_CTR_OF5
#define AMBARELLA_CS_TIMER_AXI0_CTR_CSL		TIMER_CTR_CSL5
#define AMBARELLA_CS_TIMER_AXI0_CTR_MASK	0x000F0000

#define AMBARELLA_CS_TIMER_AXI1_STATUS_REG	TIMER7_STATUS_REG
#define AMBARELLA_CS_TIMER_AXI1_RELOAD_REG	TIMER7_RELOAD_REG
#define AMBARELLA_CS_TIMER_AXI1_MATCH1_REG	TIMER7_MATCH1_REG
#define AMBARELLA_CS_TIMER_AXI1_MATCH2_REG	TIMER7_MATCH2_REG
#define AMBARELLA_CS_TIMER_AXI1_CTR_EN		TIMER_CTR_EN7
#define AMBARELLA_CS_TIMER_AXI1_CTR_OF		TIMER_CTR_OF7
#define AMBARELLA_CS_TIMER_AXI1_CTR_CSL		TIMER_CTR_CSL7
#define AMBARELLA_CS_TIMER_AXI1_CTR_MASK	0x0F000000
#endif /* defined(CONFIG_MACH_HYACINTH_0) || defined(CONFIG_MACH_HYACINTH_1) */

static inline void ambarella_cs_timer_init(void)
{
#if defined(CONFIG_PLAT_AMBARELLA_SUPPORT_HAL)
#else
	ambarella_register_clk(&pll_out_core);
	ambarella_register_clk(&pll_out_idsp);
	ambarella_register_clk(&gclk_core);
	ambarella_register_clk(&gclk_ahb);
	ambarella_register_clk(&gclk_apb);
#endif
#if !defined(CONFIG_MACH_HYACINTH_0) && !defined(CONFIG_MACH_HYACINTH_1)
	amba_clrbitsl(TIMER_CTR_REG, AMBARELLA_CS_TIMER_CTR_EN);
	amba_clrbitsl(TIMER_CTR_REG, AMBARELLA_CS_TIMER_CTR_OF);
	amba_clrbitsl(TIMER_CTR_REG, AMBARELLA_CS_TIMER_CTR_CSL);
	amba_writel(AMBARELLA_CS_TIMER_STATUS_REG, 0xffffffff);
	amba_writel(AMBARELLA_CS_TIMER_RELOAD_REG, 0xffffffff);
	amba_writel(AMBARELLA_CS_TIMER_MATCH1_REG, 0x0);
	amba_writel(AMBARELLA_CS_TIMER_MATCH2_REG, 0x0);
	amba_setbitsl(TIMER_CTR_REG, AMBARELLA_CS_TIMER_CTR_EN);
#else /* defined(CONFIG_MACH_HYACINTH_0) || defined(CONFIG_MACH_HYACINTH_1) */
	if (machine_is_hyacinth_0()) {
		amba_clrbitsl(TIMER_CTR_REG, AMBARELLA_CS_TIMER_AXI0_CTR_EN);
		amba_clrbitsl(TIMER_CTR_REG, AMBARELLA_CS_TIMER_AXI0_CTR_OF);
		amba_clrbitsl(TIMER_CTR_REG, AMBARELLA_CS_TIMER_AXI0_CTR_CSL);
		amba_writel(AMBARELLA_CS_TIMER_AXI0_STATUS_REG, 0xffffffff);
		amba_writel(AMBARELLA_CS_TIMER_AXI0_RELOAD_REG, 0xffffffff);
		amba_writel(AMBARELLA_CS_TIMER_AXI0_MATCH1_REG, 0x0);
		amba_writel(AMBARELLA_CS_TIMER_AXI0_MATCH2_REG, 0x0);
		amba_setbitsl(TIMER_CTR_REG, AMBARELLA_CS_TIMER_AXI0_CTR_EN);
	}
	else if (machine_is_hyacinth_1()) {
		amba_clrbitsl(TIMER_CTR_REG, AMBARELLA_CS_TIMER_AXI1_CTR_EN);
		amba_clrbitsl(TIMER_CTR_REG, AMBARELLA_CS_TIMER_AXI1_CTR_OF);
		amba_clrbitsl(TIMER_CTR_REG, AMBARELLA_CS_TIMER_AXI1_CTR_CSL);
		amba_writel(AMBARELLA_CS_TIMER_AXI1_STATUS_REG, 0xffffffff);
		amba_writel(AMBARELLA_CS_TIMER_AXI1_RELOAD_REG, 0xffffffff);
		amba_writel(AMBARELLA_CS_TIMER_AXI1_MATCH1_REG, 0x0);
		amba_writel(AMBARELLA_CS_TIMER_AXI1_MATCH2_REG, 0x0);
		amba_setbitsl(TIMER_CTR_REG, AMBARELLA_CS_TIMER_AXI1_CTR_EN);
	}
#endif /* defined(CONFIG_MACH_HYACINTH_0) || defined(CONFIG_MACH_HYACINTH_1) */
}

static cycle_t ambarella_cs_timer_read(struct clocksource *cs)
{
#if !defined(CONFIG_MACH_HYACINTH_0) && !defined(CONFIG_MACH_HYACINTH_1)
	return (-(u32)amba_readl(AMBARELLA_CS_TIMER_STATUS_REG));
#else /* defined(CONFIG_MACH_HYACINTH_0) || defined(CONFIG_MACH_HYACINTH_1) */
	if (machine_is_hyacinth_0())
		return (-(u32)amba_readl(AMBARELLA_CS_TIMER_AXI0_STATUS_REG));
	else
		return (-(u32)amba_readl(AMBARELLA_CS_TIMER_AXI1_STATUS_REG));
#endif /* defined(CONFIG_MACH_HYACINTH_0) || defined(CONFIG_MACH_HYACINTH_1) */
}

static struct clocksource ambarella_cs_timer_clksrc = {
	.name		= "ambarella-cs-timer",
	.rating		= AMBARELLA_TIMER_RATING,
	.read		= ambarella_cs_timer_read,
	.mask		= CLOCKSOURCE_MASK(32),
	.mult		= 2236962133u,
	.shift		= 27,
	.flags		= CLOCK_SOURCE_IS_CONTINUOUS,
};

static u32 notrace ambarella_read_sched_clock(void)
{
#if !defined(CONFIG_MACH_HYACINTH_0) && !defined(CONFIG_MACH_HYACINTH_1)
	return (-(u32)amba_readl(AMBARELLA_CS_TIMER_STATUS_REG));
#else /* defined(CONFIG_MACH_HYACINTH_0) || defined(CONFIG_MACH_HYACINTH_1) */
	if (machine_is_hyacinth_0())
		return (-(u32)amba_readl(AMBARELLA_CS_TIMER_AXI0_STATUS_REG));
	if (machine_is_hyacinth_1())
		return (-(u32)amba_readl(AMBARELLA_CS_TIMER_AXI1_STATUS_REG));
#endif /* defined(CONFIG_MACH_HYACINTH_0) || defined(CONFIG_MACH_HYACINTH_1) */
}
#endif /* defined(CONFIG_AMBARELLA_SUPPORT_CLOCKSOURCE) */

#ifdef CONFIG_HAVE_ARM_TWD
static DEFINE_TWD_LOCAL_TIMER(twd_local_timer,
                             AMBARELLA_PA_PT_WD_BASE,
                             IRQ_LOCALTIMER);

static void __init ambarella_twd_init(void)
{
       int err = twd_local_timer_register(&twd_local_timer);
       if (err)
               pr_err("twd_local_timer_register failed %d\n", err);
}
#else
#define ambarella_twd_init()       do {} while(0)
#endif
/* ==========================================================================*/
static void __init ambarella_timer_init(void)
{
	int ret = 0;

#if defined(CONFIG_AMBARELLA_SUPPORT_CLOCKSOURCE)
	ambarella_cs_timer_init();
	ambarella_cs_timer_clksrc.mult = clocksource_hz2mult(
		AMBARELLA_TIMER_FREQ, ambarella_cs_timer_clksrc.shift);
	pr_debug("%s: mult = %u, shift = %u\n",
		ambarella_cs_timer_clksrc.name,
		ambarella_cs_timer_clksrc.mult,
		ambarella_cs_timer_clksrc.shift);
	clocksource_register(&ambarella_cs_timer_clksrc);
	setup_sched_clock(ambarella_read_sched_clock, 32, AMBARELLA_TIMER_FREQ);
#endif

	ambarella_clkevt.cpumask = cpumask_of(0);
#if !defined(CONFIG_MACH_HYACINTH_0) && !defined(CONFIG_MACH_HYACINTH_1)
	ambarella_clkevt.irq = AMBARELLA_CE_TIMER_IRQ;
#else /* defined(CONFIG_MACH_HYACINTH_0) || defined(CONFIG_MACH_HYACINTH_1) */
	if (machine_is_hyacinth_0())
		ambarella_clkevt.irq = AMBARELLA_CE_TIMER_AXI0_IRQ;
	else if (machine_is_hyacinth_1())
		ambarella_clkevt.irq = AMBARELLA_CE_TIMER_AXI1_IRQ;
#endif /* defined(CONFIG_MACH_HYACINTH_0) || defined(CONFIG_MACH_HYACINTH_1) */
	ret = setup_irq(ambarella_clkevt.irq, &ambarella_ce_timer_irq);
	if (ret) {
		printk(KERN_ERR "Failed to register timer IRQ: %d\n", ret);
		BUG();
	}
	clockevents_calc_mult_shift(&ambarella_clkevt, AMBARELLA_TIMER_FREQ, 5);
	ambarella_clkevt.max_delta_ns =
		clockevent_delta2ns(0xffffffff, &ambarella_clkevt);
	ambarella_clkevt.min_delta_ns =
		clockevent_delta2ns(1, &ambarella_clkevt);
	clockevents_register_device(&ambarella_clkevt);
	ambarella_twd_init();
}

struct sys_timer ambarella_timer = {
	.init		= ambarella_timer_init,
};

u32 ambarella_timer_suspend(u32 level)
{
	u32					timer_ctr_mask;

	ambarella_timer_pm.timer_ctr_reg = amba_readl(TIMER_CTR_REG);
	ambarella_timer_pm.timer_clk = AMBARELLA_TIMER_FREQ;

#if !defined(CONFIG_MACH_HYACINTH_0) && !defined(CONFIG_MACH_HYACINTH_1)
		ambarella_timer_pm.timer_ce_status_reg =
			amba_readl(AMBARELLA_CE_TIMER_STATUS_REG);
		ambarella_timer_pm.timer_ce_reload_reg =
			amba_readl(AMBARELLA_CE_TIMER_RELOAD_REG);
		ambarella_timer_pm.timer_ce_match1_reg =
			amba_readl(AMBARELLA_CE_TIMER_MATCH1_REG);
		ambarella_timer_pm.timer_ce_match2_reg =
			amba_readl(AMBARELLA_CE_TIMER_MATCH2_REG);
#if defined(CONFIG_AMBARELLA_SUPPORT_CLOCKSOURCE)
		ambarella_timer_pm.timer_cs_status_reg =
			amba_readl(AMBARELLA_CS_TIMER_STATUS_REG);
		ambarella_timer_pm.timer_cs_reload_reg =
			amba_readl(AMBARELLA_CS_TIMER_RELOAD_REG);
		ambarella_timer_pm.timer_cs_match1_reg =
			amba_readl(AMBARELLA_CS_TIMER_MATCH1_REG);
		ambarella_timer_pm.timer_cs_match2_reg =
			amba_readl(AMBARELLA_CS_TIMER_MATCH2_REG);
#endif

#else /* defined(CONFIG_MACH_HYACINTH_0) || defined(CONFIG_MACH_HYACINTH_1) */
	if (machine_is_hyacinth_0()) {
		ambarella_timer_pm.timer_ce_status_reg =
			amba_readl(AMBARELLA_CE_TIMER_AXI0_STATUS_REG);
		ambarella_timer_pm.timer_ce_reload_reg =
			amba_readl(AMBARELLA_CE_TIMER_AXI0_RELOAD_REG);
		ambarella_timer_pm.timer_ce_match1_reg =
			amba_readl(AMBARELLA_CE_TIMER_AXI0_MATCH1_REG);
		ambarella_timer_pm.timer_ce_match2_reg =
			amba_readl(AMBARELLA_CE_TIMER_AXI0_MATCH2_REG);
#if defined(CONFIG_AMBARELLA_SUPPORT_CLOCKSOURCE)
		ambarella_timer_pm.timer_cs_status_reg =
			amba_readl(AMBARELLA_CS_TIMER_AXI0_STATUS_REG);
		ambarella_timer_pm.timer_cs_reload_reg =
			amba_readl(AMBARELLA_CS_TIMER_AXI0_RELOAD_REG);
		ambarella_timer_pm.timer_cs_match1_reg =
			amba_readl(AMBARELLA_CS_TIMER_AXI0_MATCH1_REG);
		ambarella_timer_pm.timer_cs_match2_reg =
			amba_readl(AMBARELLA_CS_TIMER_AXI0_MATCH2_REG);
#endif
	}
	else if (machine_is_hyacinth_1()) {
		ambarella_timer_pm.timer_ce_status_reg =
			amba_readl(AMBARELLA_CE_TIMER_AXI1_STATUS_REG);
		ambarella_timer_pm.timer_ce_reload_reg =
			amba_readl(AMBARELLA_CE_TIMER_AXI1_RELOAD_REG);
		ambarella_timer_pm.timer_ce_match1_reg =
			amba_readl(AMBARELLA_CE_TIMER_AXI1_MATCH1_REG);
		ambarella_timer_pm.timer_ce_match2_reg =
			amba_readl(AMBARELLA_CE_TIMER_AXI1_MATCH2_REG);
#if defined(CONFIG_AMBARELLA_SUPPORT_CLOCKSOURCE)
		ambarella_timer_pm.timer_cs_status_reg =
			amba_readl(AMBARELLA_CS_TIMER_AXI1_STATUS_REG);
		ambarella_timer_pm.timer_cs_reload_reg =
			amba_readl(AMBARELLA_CS_TIMER_AXI1_RELOAD_REG);
		ambarella_timer_pm.timer_cs_match1_reg =
			amba_readl(AMBARELLA_CS_TIMER_AXI1_MATCH1_REG);
		ambarella_timer_pm.timer_cs_match2_reg =
			amba_readl(AMBARELLA_CS_TIMER_AXI1_MATCH2_REG);
#endif
	}
#endif /* defined(CONFIG_MACH_HYACINTH_0) || defined(CONFIG_MACH_HYACINTH_1) */
	if (level) {
#if !defined(CONFIG_MACH_HYACINTH_0) && !defined(CONFIG_MACH_HYACINTH_1)
			disable_irq(AMBARELLA_CE_TIMER_IRQ);
			timer_ctr_mask = AMBARELLA_CE_TIMER_CTR_MASK;
#if defined(CONFIG_AMBARELLA_SUPPORT_CLOCKSOURCE)
			timer_ctr_mask |= AMBARELLA_CS_TIMER_CTR_MASK;
#endif
#else /* defined(CONFIG_MACH_HYACINTH_0) || defined(CONFIG_MACH_HYACINTH_1) */
		if (machine_is_hyacinth_0()) {
			disable_irq(AMBARELLA_CE_TIMER_AXI0_IRQ);
			timer_ctr_mask = AMBARELLA_CE_TIMER_AXI0_CTR_MASK;
#if defined(CONFIG_AMBARELLA_SUPPORT_CLOCKSOURCE)
			timer_ctr_mask |= AMBARELLA_CS_TIMER_AXI0_CTR_MASK;
#endif
		}
		else if (machine_is_hyacinth_1()) {
			disable_irq(AMBARELLA_CE_TIMER_AXI1_IRQ);
			timer_ctr_mask = AMBARELLA_CE_TIMER_AXI1_CTR_MASK;
#if defined(CONFIG_AMBARELLA_SUPPORT_CLOCKSOURCE)
			timer_ctr_mask |= AMBARELLA_CS_TIMER_AXI1_CTR_MASK;
#endif
		}
#endif /* defined(CONFIG_MACH_HYACINTH_0) || defined(CONFIG_MACH_HYACINTH_1) */
		amba_clrbitsl(TIMER_CTR_REG, timer_ctr_mask);
	}

	return 0;
}

u32 ambarella_timer_resume(u32 level)
{
	u32					timer_ctr_mask;

#if !defined(CONFIG_MACH_HYACINTH_0) && !defined(CONFIG_MACH_HYACINTH_1)
		timer_ctr_mask = AMBARELLA_CE_TIMER_CTR_MASK;
#if defined(CONFIG_AMBARELLA_SUPPORT_CLOCKSOURCE)
		timer_ctr_mask |= AMBARELLA_CS_TIMER_CTR_MASK;
#endif
		amba_clrbitsl(TIMER_CTR_REG, timer_ctr_mask);
#if defined(CONFIG_AMBARELLA_SUPPORT_CLOCKSOURCE)
		amba_writel(AMBARELLA_CS_TIMER_STATUS_REG,
				ambarella_timer_pm.timer_cs_status_reg);
		amba_writel(AMBARELLA_CS_TIMER_RELOAD_REG,
				ambarella_timer_pm.timer_cs_reload_reg);
		amba_writel(AMBARELLA_CS_TIMER_MATCH1_REG,
				ambarella_timer_pm.timer_cs_match1_reg);
		amba_writel(AMBARELLA_CS_TIMER_MATCH2_REG,
				ambarella_timer_pm.timer_cs_match2_reg);
#endif
		if ((ambarella_timer_pm.timer_ce_status_reg == 0) &&
			(ambarella_timer_pm.timer_ce_reload_reg == 0)){
			amba_writel(AMBARELLA_CE_TIMER_STATUS_REG,
					AMBARELLA_TIMER_FREQ / HZ);
		} else {
			amba_writel(AMBARELLA_CE_TIMER_STATUS_REG,
					ambarella_timer_pm.timer_ce_status_reg);
		}
		amba_writel(AMBARELLA_CE_TIMER_RELOAD_REG,
				ambarella_timer_pm.timer_ce_reload_reg);
		amba_writel(AMBARELLA_CE_TIMER_MATCH1_REG,
				ambarella_timer_pm.timer_ce_match1_reg);
		amba_writel(AMBARELLA_CE_TIMER_MATCH2_REG,
				ambarella_timer_pm.timer_ce_match2_reg);
#else /* defined(CONFIG_MACH_HYACINTH_0) || defined(CONFIG_MACH_HYACINTH_1) */
	if (machine_is_hyacinth_0()) {
		timer_ctr_mask = AMBARELLA_CE_TIMER_AXI0_CTR_MASK;
#if defined(CONFIG_AMBARELLA_SUPPORT_CLOCKSOURCE)
		timer_ctr_mask |= AMBARELLA_CS_TIMER_AXI0_CTR_MASK;
#endif
		amba_clrbitsl(TIMER_CTR_REG, timer_ctr_mask);
#if defined(CONFIG_AMBARELLA_SUPPORT_CLOCKSOURCE)
		amba_writel(AMBARELLA_CS_TIMER_AXI0_STATUS_REG,
				ambarella_timer_pm.timer_cs_status_reg);
		amba_writel(AMBARELLA_CS_TIMER_AXI0_RELOAD_REG,
				ambarella_timer_pm.timer_cs_reload_reg);
		amba_writel(AMBARELLA_CS_TIMER_AXI0_MATCH1_REG,
				ambarella_timer_pm.timer_cs_match1_reg);
		amba_writel(AMBARELLA_CS_TIMER_AXI0_MATCH2_REG,
				ambarella_timer_pm.timer_cs_match2_reg);
#endif
		if ((ambarella_timer_pm.timer_ce_status_reg == 0) &&
			(ambarella_timer_pm.timer_ce_reload_reg == 0)){
			amba_writel(AMBARELLA_CE_TIMER_AXI0_STATUS_REG,
					AMBARELLA_TIMER_FREQ / HZ);
		} else {
			amba_writel(AMBARELLA_CE_TIMER_AXI0_STATUS_REG,
					ambarella_timer_pm.timer_ce_status_reg);
		}
		amba_writel(AMBARELLA_CE_TIMER_AXI0_RELOAD_REG,
				ambarella_timer_pm.timer_ce_reload_reg);
		amba_writel(AMBARELLA_CE_TIMER_AXI0_MATCH1_REG,
				ambarella_timer_pm.timer_ce_match1_reg);
		amba_writel(AMBARELLA_CE_TIMER_AXI0_MATCH2_REG,
				ambarella_timer_pm.timer_ce_match2_reg);
	}
	else if (machine_is_hyacinth_1()) {
		timer_ctr_mask = AMBARELLA_CE_TIMER_AXI1_CTR_MASK;
#if defined(CONFIG_AMBARELLA_SUPPORT_CLOCKSOURCE)
		timer_ctr_mask |= AMBARELLA_CS_TIMER_AXI1_CTR_MASK;
#endif
		amba_clrbitsl(TIMER_CTR_REG, timer_ctr_mask);
#if defined(CONFIG_AMBARELLA_SUPPORT_CLOCKSOURCE)
		amba_writel(AMBARELLA_CS_TIMER_AXI1_STATUS_REG,
				ambarella_timer_pm.timer_cs_status_reg);
		amba_writel(AMBARELLA_CS_TIMER_AXI1_RELOAD_REG,
				ambarella_timer_pm.timer_cs_reload_reg);
		amba_writel(AMBARELLA_CS_TIMER_AXI1_MATCH1_REG,
				ambarella_timer_pm.timer_cs_match1_reg);
		amba_writel(AMBARELLA_CS_TIMER_AXI1_MATCH2_REG,
				ambarella_timer_pm.timer_cs_match2_reg);
#endif
		if ((ambarella_timer_pm.timer_ce_status_reg == 0) &&
			(ambarella_timer_pm.timer_ce_reload_reg == 0)){
			amba_writel(AMBARELLA_CE_TIMER_AXI1_STATUS_REG,
					AMBARELLA_TIMER_FREQ / HZ);
		} else {
			amba_writel(AMBARELLA_CE_TIMER_AXI1_STATUS_REG,
					ambarella_timer_pm.timer_ce_status_reg);
		}
		amba_writel(AMBARELLA_CE_TIMER_AXI1_RELOAD_REG,
				ambarella_timer_pm.timer_ce_reload_reg);
		amba_writel(AMBARELLA_CE_TIMER_AXI1_MATCH1_REG,
				ambarella_timer_pm.timer_ce_match1_reg);
		amba_writel(AMBARELLA_CE_TIMER_AXI1_MATCH2_REG,
				ambarella_timer_pm.timer_ce_match2_reg);
	}
#endif /* defined(CONFIG_MACH_HYACINTH_0) || defined(CONFIG_MACH_HYACINTH_1) */

	if (ambarella_timer_pm.timer_clk != AMBARELLA_TIMER_FREQ) {
		clockevents_calc_mult_shift(&ambarella_clkevt,
				AMBARELLA_TIMER_FREQ, 5);
		ambarella_clkevt.max_delta_ns =
			clockevent_delta2ns(0xffffffff, &ambarella_clkevt);
		ambarella_clkevt.min_delta_ns =
			clockevent_delta2ns(1, &ambarella_clkevt);
		switch (ambarella_clkevt.mode) {
		case CLOCK_EVT_MODE_PERIODIC:
			ambarella_ce_timer_set_periodic();
			break;
		case CLOCK_EVT_MODE_ONESHOT:
		case CLOCK_EVT_MODE_UNUSED:
		case CLOCK_EVT_MODE_SHUTDOWN:
		case CLOCK_EVT_MODE_RESUME:
			break;
		}

#if defined(CONFIG_AMBARELLA_SUPPORT_CLOCKSOURCE)
		clocksource_change_rating(&ambarella_cs_timer_clksrc, 0);
		ambarella_cs_timer_clksrc.mult = clocksource_hz2mult(
			AMBARELLA_TIMER_FREQ, ambarella_cs_timer_clksrc.shift);
		pr_debug("%s: mult = %u, shift = %u\n",
			ambarella_cs_timer_clksrc.name,
			ambarella_cs_timer_clksrc.mult,
			ambarella_cs_timer_clksrc.shift);
		clocksource_change_rating(&ambarella_cs_timer_clksrc,
			AMBARELLA_TIMER_RATING);
#if defined(CONFIG_HAVE_SCHED_CLOCK)
		setup_sched_clock(ambarella_read_sched_clock,
			32, AMBARELLA_TIMER_FREQ);
#endif
#endif
	}

#if !defined(CONFIG_MACH_HYACINTH_0) && !defined(CONFIG_MACH_HYACINTH_1)
		timer_ctr_mask = AMBARELLA_CE_TIMER_CTR_MASK;
#if defined(CONFIG_AMBARELLA_SUPPORT_CLOCKSOURCE)
		timer_ctr_mask |= AMBARELLA_CS_TIMER_CTR_MASK;
#endif
		amba_setbitsl(TIMER_CTR_REG,
				(ambarella_timer_pm.timer_ctr_reg & timer_ctr_mask));
		if (level)
			enable_irq(AMBARELLA_CE_TIMER_IRQ);
#else /* defined(CONFIG_MACH_HYACINTH_0) || defined(CONFIG_MACH_HYACINTH_1) */
	if (machine_is_hyacinth_0()) {
		timer_ctr_mask = AMBARELLA_CE_TIMER_AXI0_CTR_MASK;
#if defined(CONFIG_AMBARELLA_SUPPORT_CLOCKSOURCE)
		timer_ctr_mask |= AMBARELLA_CS_TIMER_AXI0_CTR_MASK;
#endif
		amba_setbitsl(TIMER_CTR_REG,
				(ambarella_timer_pm.timer_ctr_reg & timer_ctr_mask));
		if (level)
			enable_irq(AMBARELLA_CE_TIMER_AXI0_IRQ);
	}
	else if (machine_is_hyacinth_1()) {
		timer_ctr_mask = AMBARELLA_CE_TIMER_AXI1_CTR_MASK;
#if defined(CONFIG_AMBARELLA_SUPPORT_CLOCKSOURCE)
		timer_ctr_mask |= AMBARELLA_CS_TIMER_AXI1_CTR_MASK;
#endif
		amba_setbitsl(TIMER_CTR_REG,
				(ambarella_timer_pm.timer_ctr_reg & timer_ctr_mask));
		if (level)
			enable_irq(AMBARELLA_CE_TIMER_AXI1_IRQ);
	}
#endif /* defined(CONFIG_MACH_HYACINTH_0) || defined(CONFIG_MACH_HYACINTH_1) */

#if defined(CONFIG_SMP)
	//percpu_timer_update_rate(amb_get_axi_clock_frequency(HAL_BASE_VP));
#endif

	return 0;
}

int __init ambarella_init_tm(void)
{
	int					errCode = 0;

	return errCode;
}

