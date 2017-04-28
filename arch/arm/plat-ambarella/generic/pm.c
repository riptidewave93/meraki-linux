/*
 * arch/arm/plat-ambarella/generic/pm.c
 * Power Management Routines
 * Author: Anthony Ginger <hfjiang@ambarella.com>
 *
 * Copyright (C) 2004-2009, Ambarella, Inc.
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
#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/suspend.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/cpu.h>
#include <linux/power_supply.h>

#include <asm/cacheflush.h>
#include <asm/io.h>
#include <asm/system.h>
#include <asm/suspend.h>

#include <mach/board.h>
#include <mach/hardware.h>
#include <mach/init.h>

#include <plat/bapi.h>

/* ==========================================================================*/
#ifdef MODULE_PARAM_PREFIX
#undef MODULE_PARAM_PREFIX
#endif
#define MODULE_PARAM_PREFIX	"ambarella_config."

/* ==========================================================================*/
static int pm_check_power_supply = 1;
static int pm_force_power_on = 0;
#if defined(CONFIG_AMBARELLA_SYS_PM_CALL)
module_param(pm_check_power_supply, int, 0644);
module_param(pm_force_power_on, int, 0644);
#endif

/* ==========================================================================*/
void ambarella_power_off(void)
{
	if (ambarella_board_generic.power_control.gpio_id >= 0) {
		ambarella_set_gpio_output(
			&ambarella_board_generic.power_control, 0);
		ambarella_set_gpio_output(
			&ambarella_board_generic.power_control, 1);
	} else {
		amba_rct_setbitsl(ANA_PWR_REG, ANA_PWR_POWER_DOWN);
	}
}

void ambarella_power_off_prepare(void)
{
}

/* ==========================================================================*/
static int ambarella_pm_notify(unsigned long val)
{
	int					retval = 0;

	retval = notifier_to_errno(ambarella_set_event(val, NULL));
	if (retval)
		pr_debug("%s: 0x%08lx fail(%d)\n",__func__, val, retval);

	return retval;
}

static int ambarella_pm_notify_raw(unsigned long val)
{
	int					retval = 0;

	retval = notifier_to_errno(ambarella_set_raw_event(val, NULL));
	if (retval)
		pr_debug("%s: 0x%08lx fail(%d)\n",__func__, val, retval);

	return retval;
}

static int ambarella_pm_pre(unsigned long *irqflag, u32 bsuspend, u32 tm_level)
{
	int					retval = 0;

	if (irqflag)
		local_irq_save(*irqflag);

	if (bsuspend) {
		ambarella_adc_suspend(0);
		ambarella_timer_suspend(tm_level);
		ambarella_irq_suspend(0);
		ambarella_gpio_suspend(0);
	}

	retval = ambarella_pm_notify_raw(AMBA_EVENT_PRE_PM);

	return retval;
}

static int ambarella_pm_post(unsigned long *irqflag, u32 bresume, u32 tm_level)
{
	int					retval = 0;

	retval = ambarella_pm_notify_raw(AMBA_EVENT_POST_PM);

	if (bresume) {
		ambarella_gpio_resume(0);
		ambarella_irq_resume(0);
		ambarella_timer_resume(tm_level);
		ambarella_adc_resume(0);
	}

	if (irqflag)
		local_irq_restore(*irqflag);

	return retval;
}

static int ambarella_pm_check(suspend_state_t state)
{
	int					retval = 0;

	retval = ambarella_pm_notify_raw(AMBA_EVENT_CHECK_PM);
	if (retval)
		goto ambarella_pm_check_exit;

	retval = ambarella_pm_notify(AMBA_EVENT_CHECK_PM);
	if (retval)
		goto ambarella_pm_check_exit;

ambarella_pm_check_exit:
	return retval;
}

/* ==========================================================================*/
static int ambarella_pm_cpu_do_idle(unsigned long unused)
{
	cpu_do_idle();

	return 0;
}

static int ambarella_pm_enter_standby(void)
{
	int					retval = 0;
	unsigned long				flags;

	if (ambarella_pm_pre(&flags, 1, 1))
		BUG();

	cpu_suspend(0, ambarella_pm_cpu_do_idle);

	if (ambarella_pm_post(&flags, 1, 1))
		BUG();

	return retval;
}

static int ambarella_pm_enter_mem(void)
{
	int					retval = 0;
	unsigned long				flags;
#if defined(CONFIG_AMBARELLA_SUPPORT_BAPI)
	struct ambarella_bapi_reboot_info_s	reboot_info;
#endif

	if (ambarella_pm_pre(&flags, 1, 1))
		BUG();

#if defined(CONFIG_AMBARELLA_SUPPORT_BAPI)
	reboot_info.magic = DEFAULT_BAPI_REBOOT_MAGIC;
	reboot_info.mode = AMBARELLA_BAPI_CMD_REBOOT_SELFREFERESH;
	retval = ambarella_bapi_cmd(AMBARELLA_BAPI_CMD_SET_REBOOT_INFO,
		&reboot_info);
	if (retval)
		goto ambarella_pm_enter_mem_exit_bapi;
	retval = ambarella_bapi_cmd(AMBARELLA_BAPI_CMD_AOSS_SAVE, NULL);
ambarella_pm_enter_mem_exit_bapi:
#endif

	if (ambarella_pm_post(&flags, 1, 1))
		BUG();

	return retval;
}

static int ambarella_pm_suspend_valid(suspend_state_t state)
{
	int					retval = 0;
	int					valid = 0;
#if defined(CONFIG_AMBARELLA_SUPPORT_BAPI)
	int					mode;
#endif

	retval = ambarella_pm_check(state);
	if (retval)
		goto ambarella_pm_valid_exit;

	switch (state) {
	case PM_SUSPEND_ON:
		valid = 1;
		break;

	case PM_SUSPEND_STANDBY:
		valid = 1;
		break;

	case PM_SUSPEND_MEM:
#if defined(CONFIG_AMBARELLA_SUPPORT_BAPI)
		mode = AMBARELLA_BAPI_REBOOT_SELFREFERESH;
		if (ambarella_bapi_cmd(AMBARELLA_BAPI_CMD_CHECK_REBOOT,
			&mode) == 1) {
			valid = 1;
		}
#endif
		if (pm_check_power_supply &&
			(power_supply_is_system_supplied() > 0))
			valid = 0;
		if (pm_force_power_on)
			valid = 0;
		break;

	default:
		break;
	}

ambarella_pm_valid_exit:
	pr_debug("%s: state[%d]=%d\n", __func__, state, valid);

	return valid;
}

static int ambarella_pm_suspend_begin(suspend_state_t state)
{
	int					retval = 0;

	switch (state) {
	case PM_SUSPEND_STANDBY:
	case PM_SUSPEND_MEM:
		retval = ambarella_pm_notify(AMBA_EVENT_PRE_PM);
		break;

	default:
		break;
	}

	return retval;
}

static int ambarella_pm_suspend_enter(suspend_state_t state)
{
	int					retval = 0;

	switch (state) {
	case PM_SUSPEND_ON:
		break;

	case PM_SUSPEND_STANDBY:
		retval = ambarella_pm_enter_standby();
		break;

	case PM_SUSPEND_MEM:
		retval = ambarella_pm_enter_mem();
		break;

	default:
		break;
	}

	return retval;
}

static void ambarella_pm_suspend_end(void)
{
	ambarella_pm_notify(AMBA_EVENT_POST_PM);
}

static struct platform_suspend_ops ambarella_pm_suspend_ops = {
	.valid		= ambarella_pm_suspend_valid,
	.begin		= ambarella_pm_suspend_begin,
	.enter		= ambarella_pm_suspend_enter,
	.end		= ambarella_pm_suspend_end,
};

/* ==========================================================================*/
int __init ambarella_init_pm(void)
{
	pm_power_off = ambarella_power_off;
	pm_power_off_prepare = ambarella_power_off_prepare;

	suspend_set_ops(&ambarella_pm_suspend_ops);

	return 0;
}

