/*
 * TI81XX Power Management
 *
 * This module implements TI81XX specific Power management Routines
 *
 * Copyright (C) {2011} Texas Instruments Incorporated - http://www.ti.com/
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

#include <linux/pm.h>
#include <linux/suspend.h>
#include <linux/module.h>
#include <linux/list.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <linux/clk.h>
#include <asm/io.h>
#include <asm/sizes.h>
#include "pm.h"
#include "powerdomain.h"
#include "clockdomain.h"
#include "cm81xx.h"
#include "cm-regbits-81xx.h"
#include "prm-regbits-81xx.h"
#include "prm2xxx_3xxx.h"
#include <mach/omap4-common.h>
#include <mach/ti81xx-common.h>
#include <plat/serial.h>
#include <plat/sram.h>

/**
 * suspend_cfg_addr_list - Index for addresses of configuration registers
 *
 * This enum is used to index the array passed to suspend routine with various
 * configuration register addresses.
 *
 * Since these are used to load into registers by suspend code, care must be
 * taken not to add too many entries here and to always be in sync with the
 * suspend code in arm/mach-omap2/sleep814x.S for any change made here and vice
 * verse.
 *
 * XXX: Passing DEEPSLEEP_CTRL for now as we don't support DS by default and
 * check for non-zero for this entry enables the suspend code to go ahead and
 * enter DS. We set this address only if enter_deep_sleep = true, else pass 0.
 *	=> This will be removed in later revisions when we support DeepSleep
 *	=> Though we pass real addres shere, the assembler code in sleep81xx.S
 *	is only interested in non zero value.
 */
enum suspend_cfg_addrs {
	EMIF0_BASE = 0,
	EMIF1_BASE,
	DMM_BASE,
	PLL_BASE,
	CTRL_BASE,
	DEEPSLEEP_CTRL,
	SUSPEND_CFG_ADDRS_END /* Must be the last entry */
};

void __iomem *suspend_cfg_addr_list[SUSPEND_CFG_ADDRS_END];

struct power_state {
	struct powerdomain *pwrdm;
	u32 next_state;
#ifdef CONFIG_SUSPEND
	u32 saved_state;
#endif
	struct list_head node;
};

static bool enter_deep_sleep;
static bool turnoff_idle_pwrdms;
static LIST_HEAD(pwrst_list);

static void (*_ti814x_ddr_self_refresh)(void __iomem *);

#ifdef CONFIG_SUSPEND
static int ti81xx_pwrdms_set_suspend_state(struct powerdomain *pwrdm)
{
	/* alwon*_pwrdm fail this check and return */
	if (!pwrdm->pwrsts)
		return 0;

	pwrdm_set_next_pwrst(pwrdm, PWRDM_POWER_OFF);
	return pwrdm_wait_transition(pwrdm);
}

static int ti81xx_pwrdms_read_suspend_state(struct powerdomain *pwrdm)
{
	int state;
	if (!pwrdm->pwrsts)
		return 0;

	state = pwrdm_read_pwrst(pwrdm);
	if (state != PWRDM_POWER_OFF)
		pr_err("%s did not enter off mode, current state = %d\n",
							pwrdm->name, state);
	return 0;
}

static int ti81xx_pm_enter_ddr_self_refresh(void)
{
	_ti814x_ddr_self_refresh(&suspend_cfg_addr_list[0]);
	return 0;
}

static int ti81xx_pm_suspend(void)
{
	struct power_state *pwrst;
	int ret = 0;

	if ((wakeup_timer_seconds || wakeup_timer_milliseconds) &&
		!enter_deep_sleep)
		omap2_pm_wakeup_on_timer(wakeup_timer_seconds,
					wakeup_timer_milliseconds);

	if (turnoff_idle_pwrdms) {
		/* Read current next_pwrsts */
		list_for_each_entry(pwrst, &pwrst_list, node) {
			pwrst->saved_state =
					pwrdm_read_next_pwrst(pwrst->pwrdm);
		}

		/* update pwrst->next_state */
		list_for_each_entry(pwrst, &pwrst_list, node) {
			if (ti81xx_pwrdms_set_suspend_state(pwrst->pwrdm)) {
				pr_err("Failed to set pwrdm suspend states\n");
				return ret;
			}
		}
	}

	ti81xx_pm_enter_ddr_self_refresh();

	if (turnoff_idle_pwrdms) {
		/* Check if pwrdms successfully transitioned to off state */
		list_for_each_entry(pwrst, &pwrst_list, node)
			ti81xx_pwrdms_read_suspend_state(pwrst->pwrdm);
		/* Restore the saved state */
		list_for_each_entry(pwrst, &pwrst_list, node)
			pwrdm_set_next_pwrst(pwrst->pwrdm, pwrst->saved_state);
	}
	return ret;
}

static int ti81xx_pm_enter(suspend_state_t suspend_state)
{
	int ret = 0;

	switch (suspend_state) {
	case PM_SUSPEND_STANDBY:
	case PM_SUSPEND_MEM:
		ret = ti81xx_pm_suspend();
		break;
	default:
		ret = -EINVAL;
	}
	return ret;
}

static int ti81xx_pm_begin(suspend_state_t state)
{
	disable_hlt();
	omap_uart_enable_irqs(0);
	return 0;
}

static void ti81xx_pm_end(void)
{
	enable_hlt();
	omap_uart_enable_irqs(1);
	return;
}

static const struct platform_suspend_ops ti81xx_pm_ops[] = {
	{
		.begin		= ti81xx_pm_begin,
		.end		= ti81xx_pm_end,
		.enter		= ti81xx_pm_enter,
		.valid		= suspend_valid_only_mem,
	}
};
#endif /* CONFIG_SUSPEND */

static int __init pwrdms_setup(struct powerdomain *pwrdm, void *unused)
{
	struct power_state *pwrst;

	if (!pwrdm->pwrsts)
		return 0;

	pwrst = kmalloc(sizeof(struct power_state), GFP_ATOMIC);
	if (!pwrst)
		return -ENOMEM;
	pwrst->pwrdm = pwrdm;
	pwrst->next_state = PWRDM_POWER_OFF;
	list_add(&pwrst->node, &pwrst_list);

	return pwrdm_set_next_pwrst(pwrst->pwrdm, pwrst->next_state);
}

/*
 * Enable hw supervised mode for all clockdomains if it's
 * supported. Initiate sleep transition for other clockdomains, if
 * they are not used
 */
static int __init clkdms_setup(struct clockdomain *clkdm, void *unused)
{
	if (clkdm->flags & CLKDM_CAN_ENABLE_AUTO)
		omap2_clkdm_allow_idle(clkdm);
	else if (clkdm->flags & CLKDM_CAN_FORCE_SLEEP &&
		atomic_read(&clkdm->usecount) == 0)
		omap2_clkdm_sleep(clkdm);
	return 0;
}

static void __init prcm_setup_regs(void)
{
	/* Clear reset status register */
	omap2_prm_write_mod_reg(0xffffffff, TI81XX_PRM_DEVICE_MOD,
							TI81XX_RM_RSTST);
	omap2_prm_write_mod_reg(0xffffffff, TI81XX_PRM_ALWON_MOD,
							TI81XX_RM_RSTST);
	omap2_prm_write_mod_reg(0xffffffff, TI814X_PRM_DSP_MOD,
							TI81XX_RM_RSTST);
	omap2_prm_write_mod_reg(0xffffffff, TI814X_PRM_ALWON2_MOD,
							TI81XX_RM_RSTST);
	omap2_prm_write_mod_reg(0xffffffff, TI814X_PRM_HDVICP_MOD,
							TI81XX_RM_RSTST);
	omap2_prm_write_mod_reg(0xffffffff, TI814X_PRM_ISP_MOD,
							TI81XX_RM_RSTST);
	omap2_prm_write_mod_reg(0xffffffff, TI814X_PRM_HDVPSS_MOD,
							TI81XX_RM_RSTST);
	omap2_prm_write_mod_reg(0xffffffff, TI814X_PRM_GFX_MOD,
							TI81XX_RM_RSTST);
}

void ti81xx_enable_deep_sleep(u32 deep_sleep_enabled)
{
	if (deep_sleep_enabled) {
		enter_deep_sleep = true;
		suspend_cfg_addr_list[DEEPSLEEP_CTRL] =
			TI814X_PLL_CMGC_DEEPSLEEP_CTRL;
	} else {
		enter_deep_sleep = false;
		suspend_cfg_addr_list[DEEPSLEEP_CTRL] = 0;
	}
}

#if defined(CONFIG_ARCH_TI814X)
/* Set DEEPSLEEPZ pin polarity */
void ti81xx_config_deepsleep_wake_polarity(u32 osc_wake_pol)
{
	u32 v;

	v = __raw_readl(TI814X_PLL_CMGC_DEEPSLEEP_CTRL);
	if (!osc_wake_pol) {
		/* Set polarity to active low */
		v &= ~(TI814X_DEEPSLEEP_CTRL_DSPOLARITY_MASK);
		osc_wake_polarity = 0;
	} else {
		/* set polarity to active high */
		v |= (TI814X_DEEPSLEEP_CTRL_DSPOLARITY_MASK);
		osc_wake_polarity = 1;
	}
	__raw_writel(v, TI814X_PLL_CMGC_DEEPSLEEP_CTRL);
}
#else
void ti81xx_config_deepsleep_wake_polarity(u32 osc_wake_pol)
{
}
#endif

void ti81xx_powerdown_idle_pwrdms(u32 pwrdown_idle_pwrdms)
{
	if (pwrdown_idle_pwrdms)
		turnoff_idle_pwrdms = true;
	else
		turnoff_idle_pwrdms = false;
}

void omap_push_sram_idle(void)
{
	_ti814x_ddr_self_refresh = omap_sram_push(ti814x_cpu_suspend,
					ti814x_cpu_suspend_sz);
}

static void ti814x_ddr_dynamic_pwr_down(void)
{
	u32 v;

	v = __raw_readl(suspend_cfg_addr_list[EMIF0_BASE] +
			TI814X_DDR_PHY_CTRL);
	v |= (TI814X_DDR_PHY_DYN_PWRDN_MASK);
	__raw_writel(v, (suspend_cfg_addr_list[EMIF0_BASE] +
			TI814X_DDR_PHY_CTRL));

	if (!cpu_is_dm385()) {
		v = __raw_readl(suspend_cfg_addr_list[EMIF1_BASE] +
				TI814X_DDR_PHY_CTRL);
		v |= (TI814X_DDR_PHY_DYN_PWRDN_MASK);
		__raw_writel(v, (suspend_cfg_addr_list[EMIF1_BASE] +
				TI814X_DDR_PHY_CTRL));
	}

}

/**
 * ti81xx_idle - idle routine for the CPU.'
 *
 * This function is used as replacement for default_idle()->arch_idle(), it does
 * following apart from just invoking WFI as arch_idle() would do otherwise:
 *	1) Skip wfi on ti81xx ES1.0
 *	2) Do DMM and L3 interconnect write buffer draining before entering WFI
 *	- this is required as workaround to avoid issue of aync brides in
 *	MPU->L3 and MPU->DMM path getting corrupted write pointers leading to
 *	incorrect writes resulting into undefined behavior (kernel crash or even
 *	lockup). This workaround involves writes to respective locations in the
 *	above paths (DDR for MPU->DMM), OCMC RAM for MPU->L3) mapped as strongly
 *	ordered which would go as non-posted writes.
 *	3) Ensure that no normal memory accesses (posted writes) or even cache
 *	maintainance kicks in after the above non-posted writes.
 *	4) If or during the time the DDR and OCMC regions are not mapped,
 *	proceed to use default cpu_do_idle().
 *
 *	Note: We don't disable and enable FIQs here. It is expected that we are
 *	called with IRQs disabled.
 */
static void ti81xx_idle(void)
{
	if (!cpu_is_ti814x() || cpu_is_dm385() ||
			(omap_rev() > TI8148_REV_ES1_0)) {

		if(dram_sync && sram_sync)
			ti81xx_do_wfi_sync(dram_sync, sram_sync);
		else
			cpu_do_idle();
	}

	local_irq_enable();
}

/**
 * ti81xx_pm_init - Init routine for TI81XX PM
 *
 * Initializes all powerdomain and clockdomain target states
 * and all PRCM settings.
 */
static int __init ti81xx_pm_init(void)
{
	struct power_state *pwrst, *tmp;
	int ret;

	pr_info("Power Management for TI81XX.\n");

	/*
	 * Override default_idle as we will be doing more than just wfi --> see
	 * ti81xx_idle()
	 */
	pm_idle = ti81xx_idle;

	/* Not all devices support PM beyond idle (WFI), return */
	if (!cpu_is_dm385() && !(cpu_is_ti814x() &&
				(omap_rev() > TI8148_REV_ES1_0)))
		return 0;

	prcm_setup_regs();

	suspend_cfg_addr_list[EMIF0_BASE] = ioremap(TI814X_EMIF0_BASE, SZ_64);
	WARN_ON(!suspend_cfg_addr_list[EMIF0_BASE]);

	if (!cpu_is_dm385()) {
		suspend_cfg_addr_list[EMIF1_BASE] =
					ioremap(TI814X_EMIF1_BASE, SZ_64);
		WARN_ON(!suspend_cfg_addr_list[EMIF1_BASE]);
	} else
		/* DM385 has only one instance of EMIF and DDR PHY */
		suspend_cfg_addr_list[EMIF1_BASE] = 0x0;

	suspend_cfg_addr_list[DMM_BASE] = ioremap(TI814X_DMM_BASE, SZ_64);
	WARN_ON(!suspend_cfg_addr_list[DMM_BASE]);

	suspend_cfg_addr_list[PLL_BASE] = TI814X_PLL_REGADDR(0);
	suspend_cfg_addr_list[CTRL_BASE] =
				TI81XX_L4_SLOW_IO_ADDRESS(TI81XX_CTRL_BASE);

	ti814x_ddr_dynamic_pwr_down();
	ti81xx_config_deepsleep_wake_polarity(1); /* 1- active high */

	ret = pwrdm_for_each(pwrdms_setup, NULL);
	if (ret) {
		pr_err("Failed to setup powerdomains\n");
		goto err1;
	}
	clkdm_for_each(clkdms_setup, NULL);
#ifdef CONFIG_SUSPEND
	suspend_set_ops(ti81xx_pm_ops);
#endif
	return ret;
err1:
	list_for_each_entry_safe(pwrst, tmp, &pwrst_list, node) {
		list_del(&pwrst->node);
		kfree(pwrst);
	}
	return ret;
}
late_initcall(ti81xx_pm_init);
