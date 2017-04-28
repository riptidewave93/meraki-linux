/*
 * APM86xxx suspend and standby support
 *
 * Copyright (c) 2010 Applied Micro Circuits Corporation.
 * Victor Gallardo (vgallardo@apm.com)
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/suspend.h>
#include <linux/console.h>
#include <asm/dcr.h>
#include <asm/dcr-regs.h>
#include <asm/ipp.h>

#include <linux/kobject.h>
#include <linux/string.h>

#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/cpumask.h>
#include <asm/irq.h>
#include <asm/machdep.h>
#include <asm/smp.h>

#include <asm/apm_ahbc_csr.h>
#include <asm/apm86xxx_ocm.h>
#include <asm/apm86xxx_soc.h>
#include <asm/apm86xxx_pm.h>
#include "apm86xxx_suspend.h"
#include "apm86xxx_suspend_lite.h"
#include <linux/of.h>

#undef CONFIG_IPP_PM_DBG
#if defined(CONFIG_IPP_PM_DBG)
#       define IPP_PM_DBG(fmt, arg...) printk(KERN_INFO "ipp_pm: " fmt, ## arg)
#else
#       define IPP_PM_DBG(fmt, arg...) ((void)0)
#endif

extern u32 * apm86xxx_get_scu_base(void);

extern void __enable_irq(struct irq_desc *desc, unsigned int irq, bool resume);

#ifdef ISS_MODEL_PWRMGMT_SUPPORT
int apm86xxx_pm_mode;
#endif

static u32 enable_intf;
int apm86xxx_pm_suspend_to_deepsleep;
int apm86xxx_pm_resumed_from_deepsleep;
volatile int apm86xxx_pm_inprogress = 0;

static struct apm86xxx_pm *apm86xxx_pm;
ipp_pwr_state_t __iomem *ipp_pwr_state;

static struct apm86xxx_lite lite;

void apm86xxx_standby(void)
{
	unsigned int cpu = MP_PROCESSOR_ID();
	unsigned long msr_save;

	/* Flush Cache and Disable L2 */
	if (cpu == 0) {
		flush_dcache();
		l2c_suspend();
	} else {
		cpu_l2c_disable();
	}
	
	/* set wait state MSR */
	msr_save = mfmsr();
	__asm__ __volatile__ ("sync" : : : "memory");
	isync();
	mtmsr(msr_save|MSR_WE|MSR_EE|MSR_CE|MSR_DE);
	isync();

	/* return to initial state */
	mtmsr(msr_save);
	isync();

	if (cpu == 0) {
		l2c_resume();
	}
}

static int apm86xxx_suspend_valid(suspend_state_t state)
{
	PM_TRACE("%s\n", __func__);

	switch (state) {
	case PM_SUSPEND_STANDBY:
	case PM_SUSPEND_MEM:
		return 1;
	default:
		return 0;
	}
}

static int apm86xxx_suspend_begin(suspend_state_t state)
{
	PM_TRACE("%s\n", __func__);

	switch (state) {
	case PM_SUSPEND_STANDBY:
	case PM_SUSPEND_MEM:
#ifdef ISS_MODEL_PWRMGMT_SUPPORT
		apm86xxx_pm_mode = apm86xxx_pm->mode;
#endif
		if (apm86xxx_pm->mode == PWRMODE_DSLEEP ||
		    apm86xxx_pm->deepsleep_timer) {
			apm86xxx_pm_suspend_to_deepsleep = 1;
			apm86xxx_pm_resumed_from_deepsleep = 1;
		} else {
			apm86xxx_pm_suspend_to_deepsleep = 0;
			apm86xxx_pm_resumed_from_deepsleep = 0;
		}

		apm86xxx_pm_inprogress = 1;
		return 0;
	default:
		return -EINVAL;
	}
}

static int apm86xxx_suspend_prepare(void)
{
	PM_TRACE("%s\n", __func__);
	/* TBD: might not be needed */
	return 0;
}

static int apm86xxx_suspend_ipp_prepare(void)
{
	int i;
	int mode;
	unsigned int ipp_wakeup = 0;
	int err = 0;
	unsigned int cpu = MP_PROCESSOR_ID();
	unsigned int val1, val2;

	PM_TRACE("%s\n", __func__);

	if (!apm86xxx_pm_inprogress)
		goto cpu_offline;

#ifndef CONFIG_SMP
	if (!cpu_enabled(1)) {
		err = ipp_send_user_msg(IPP_PWRMGMT_HDLR,
				IPP_PWRMGMT_CMD_CPU_DBGPWROFF,
				IPP_MSG_PARAM_UNUSED,
				IPP_MSG_CONTROL_URG_BIT,
				IPP_MSG_PARAM_UNUSED);
		if (err < 0) {
			PM_DEBUG("CPU%d failed to send UP poweroff command\n", cpu);
			goto out;
		}
	}
#endif

	if (cpu == 0 && apm86xxx_pm->vscale) {
		PM_DEBUG("CPU%d enabling voltage scaling using PS_CTL[2]\n", cpu);
		err = ipp_send_user_msg(IPP_PWRMGMT_HDLR,
					IPP_PWRMGMT_CMD_VOLTAGE_SCALE,
					IPP_VOLTAGE_SCALE_DEFAULT_SUBCMD,
					IPP_MSG_CONTROL_URG_BIT,
					IPP_MSG_PARAM_UNUSED);
		if (err < 0) {
			PM_DEBUG("CPU%d failed to enable voltage scaling\n", cpu);
			/* will continue because it might not be supported */
		}
	}

	if (cpu == 0 && apm86xxx_pm->iscale) {
		PM_DEBUG("CPU%d enabling IPP scaling\n", cpu);
		err = ipp_send_user_msg(IPP_PWRMGMT_HDLR,
					IPP_PWRMGMT_CMD_ISCALE,
					apm86xxx_pm->iscale,
					IPP_MSG_CONTROL_URG_BIT,
					IPP_MSG_PARAM_UNUSED);
		if (err < 0) {
			PM_DEBUG("CPU%d failed to enable IPP scaling\n", cpu);
			/* will continue because it might not be supported */
		}
	}

	if (apm86xxx_pm->sleep_timer_changed) {
		IPP_PM_DBG("IPP_SLEEP_TIMER_VAR(cpu=%u, timer=%u\n)",
			   cpu, apm86xxx_pm->sleep_timer);
		err = ipp_send_user_msg(IPP_CONFIG_SET_HDLR,
					IPP_SLEEP_TIMER_VAR, cpu,
					IPP_MSG_CONTROL_URG_BIT,
					apm86xxx_pm->sleep_timer);
		if (err < 0) {
			PM_DEBUG("CPU%d failed to set sleep timer\n", cpu);
			goto out;
		}
		apm86xxx_pm->sleep_timer_changed = 0;
	}

	if (apm86xxx_pm->ds_timer_changed) {
		IPP_PM_DBG("IPP_DEEPSLEEP_TIMER_VAR(cpu=%u, timer=%u\n)",
			   cpu, apm86xxx_pm->deepsleep_timer);
		err = ipp_send_user_msg(IPP_CONFIG_SET_HDLR,
					IPP_DEEPSLEEP_TIMER_VAR, cpu,
					IPP_MSG_CONTROL_URG_BIT,
					apm86xxx_pm->deepsleep_timer);
		if (err < 0) {
			PM_DEBUG("CPU%d failed to set deep sleep timer\n", cpu);
			goto out;
		}
		apm86xxx_pm->ds_timer_changed = 0;
	}

	for (i = 0; i < WAKEUP_MAX; i++) {
		if (apm86xxx_pm->wakeup[i].enabled) {
			PM_DEBUG("CPU%d setting wakeup state (%s)\n", 
				cpu, apm86xxx_pm->wakeup[i].name);
			ipp_wakeup |= apm86xxx_pm->wakeup[i].ipp_mask;
		}
	}

	val1 = get_gpio_wakeup_level_mask();
	val2 = get_gpio_wakeup_mask();
	apm86xxx_pm->gpio_wakeup = ENCODE_GPIO_WAKE_LVL(val1, val2); 

	if (ipp_wakeup & IPP_WAKEUP_GPIO_EVENT) {
		IPP_PM_DBG("IPP_WAKEUP_GPIO(cpu=%u, gpio=%u)\n",
			   cpu, apm86xxx_pm->gpio_wakeup);

		err = ipp_send_user_msg(IPP_CONFIG_SET_HDLR,
					IPP_GPIO_CONFIG_VAR, GPIO_TYPE_WAKEUP_IPP_INPUT,
					IPP_MSG_CONTROL_URG_BIT,
					apm86xxx_pm->gpio_wakeup);
		if (err < 0) {
			PM_DEBUG("CPU%d failed to set GPIO wakeuup IRQ\n", cpu);
			goto out;
		}
	}

	if (apm86xxx_pm->wakeup_led_changed) {
		IPP_PM_DBG("IPP_WAKEUPTIME_LEDMASK(cpu=%u, ledmask=%u\n)",
			   cpu, apm86xxx_pm->wakeup_ledmsk);
		err = ipp_send_user_msg(IPP_CONFIG_SET_HDLR,
					IPP_LED_MEASURE_MASK,
					IPP_MSG_PARAM_UNUSED,
					IPP_MSG_CONTROL_URG_BIT,
					apm86xxx_pm->wakeup_ledmsk);
		if (err < 0) {
			PM_DEBUG("CPU%d failed to set wakeup time LED mask\n", cpu);
			goto out;
		}
		apm86xxx_pm->wakeup_led_changed = 0;
	}

	if (apm86xxx_pm->disable_gpio_sd_changed) {
		IPP_PM_DBG("IPP_DISABLE_GPIO_SHUTDOWN(cpu=%u, gpio_shutdown=%u\n)",
			   cpu, apm86xxx_pm->disable_gpio_sd);
		err = ipp_send_user_msg(IPP_CONFIG_SET_HDLR,
					IPP_DISABLE_GPIO_SHUTDOWN,
					IPP_MSG_PARAM_UNUSED,
					IPP_MSG_CONTROL_URG_BIT,
					apm86xxx_pm->disable_gpio_sd);
		if (err < 0) {
			PM_DEBUG("CPU%d failed to set disable GPIO shutdown\n", cpu);
			goto out;
		}
		apm86xxx_pm->disable_gpio_sd_changed = 0;
	}

cpu_offline:
	IPP_PM_DBG("IPP_WAKEUP_EVENTMAP_VAR(cpu=%u, wakeup=0x%x)\n",
		   cpu, ipp_wakeup);
	err = ipp_send_user_msg(IPP_CONFIG_SET_HDLR,
				IPP_WAKEUP_EVENTMAP_VAR, cpu,
				IPP_MSG_CONTROL_URG_BIT, ipp_wakeup);
	if (err < 0) {
		PM_DEBUG("CPU%d failed to set wakeup event\n", cpu);
		goto out;
	}

	if (apm86xxx_pm_inprogress)
		mode = apm86xxx_pm->modes[apm86xxx_pm->mode].cpu[cpu];
	else
		mode = PWRMODE_PWR_OFF;

	PM_DEBUG("CPU%d preparing for %s\n", cpu, apm86xxx_pm_mode_type(mode));

	IPP_PM_DBG("IPP_PWRMGMT_HDLR(cpu=%u, mode=0x%x)\n", cpu, mode);
	err = ipp_send_user_msg(IPP_PWRMGMT_HDLR,
				mode, cpu, IPP_MSG_CONTROL_URG_BIT,
				IPP_MSG_PARAM_UNUSED);
	if (err < 0) {
		PM_DEBUG("CPU%d failed to set sleep mode mode\n", cpu);
		goto out;
	}

out:
	return err;
}

static void apm86xxx_lite_standby(void)
{
	u32 dbg;

	/* read MEMC register to make sure TLB is avaliable */
	dbg = in_be32((u32*)(lite.memc + MEMC_DDRC_REG_DBG));
	if ((dbg & DDRC_REG_DBG_MODE_NORMAL) != DDRC_REG_DBG_MODE_NORMAL)
		printk(KERN_WARNING "DDR mode not valid\n");

	/* read SCU register to make sure TLB is avaliable */
	dbg = in_be32((u32*)(lite.scu + SCU_SOCPLL2));
	if ((dbg & SOCPLL2_PWRDN) == SOCPLL2_PWRDN)
		printk(KERN_WARNING "SOCPLL2 is not valid\n");

	lite.suspend(lite.data);
}

static int apm86xxx_suspend_enter(suspend_state_t state)
{
	int err;

	PM_TRACE("%s\n", __func__);

	switch (state) {
	case PM_SUSPEND_STANDBY:
	case PM_SUSPEND_MEM:
		switch(apm86xxx_pm->mode) {
		case PWRMODE_DOZE:
		case PWRMODE_NAP:
		case PWRMODE_SLEEP:
		case PWRMODE_DSLEEP:
		default:
			{
			unsigned int itlb;
			struct ppc44x_tlbentry tlbentry;
			int ipp_irq;

			if (is_apm86xxx_lite()) {
				PM_DEBUG("CPU%d going to %s\n",
					MP_PROCESSOR_ID(),
					apm86xxx_pm->modes[apm86xxx_pm->mode].name);
				if (lite.suspend && lite.memc) {
					apm86xxx_lite_standby();
				} else {
					apm86xxx_standby();
				}
				return 0;
			}

			ipp_irq = get_ipp_irq();

			/* Temporary disable IPP IRQ, so that we
			 * don't get an interrupt as soon as we
			 * set the EE bit in MSR in deepsleep */
			disable_irq(ipp_irq);

			itlb = ppc44x_get_tlbindex(&apm86xxx_resume_deepsleep);
			ppc44x_get_tlbentry(itlb, &tlbentry);

			err = get_ipp_pwr_state_buf(&ipp_pwr_state);
			IPP_PM_DBG("get_ipp_pwr_state_buf(%p)\n", ipp_pwr_state);
			if (err < 0) {
				/* FIXME */
				PM_DEBUG("%s: Failed to get iPP Power state"
					 "...aborting Deepsleep\n", __func__);
				return err;
			}

			ipp_pwr_state->resume_pc = 
				(unsigned int)&apm86xxx_resume_deepsleep;

			ipp_pwr_state->tlb_word0 = tlbentry.word0;
			ipp_pwr_state->tlb_word1 = tlbentry.word1;
			ipp_pwr_state->tlb_word2 = tlbentry.word2;

			IPP_PM_DBG("save_ipp_pwr_state_buf(%p)\n", ipp_pwr_state);
			err = save_ipp_pwr_state_buf(&ipp_pwr_state);
			if (err < 0) {
				/* FIXME */
				PM_DEBUG("%s: Failed to save iPP Power state"
					 "...aborting Deepsleep\n", __func__);
				return err;
			}

#ifdef ISS_MODEL_PWRMGMT_SUPPORT
			/*
			 * Add TLB entry to jump back to 0xfffffffc
			 */
			tlbentry.tid   = 0;
			tlbentry.word0 = 0xFF000270;
			tlbentry.word1 = 0xFF00000F;
			tlbentry.word2 = 0x0000013F;
			ppc44x_set_tlbentry(0, &tlbentry);
#endif

			err = apm86xxx_suspend_ipp_prepare();
			if (err < 0)
				return err;

			enable_intf = apm86xxx_ahbc_get_reg(ENABLE_INTF_ADDR);

			PM_DEBUG("CPU%d going to %s\n",
				MP_PROCESSOR_ID(),
				apm86xxx_pm->modes[apm86xxx_pm->mode].name);

			/* Renable IPP IRQ  */
			enable_irq(ipp_irq);

			/* Now disable ppc44x_idle and if SMP, also
			 * Re-Enable Snoop Inihibit */
			ppc44x_idle_disable();

			suspend_console_late();

			apm86xxx_suspend_deepsleep();
			break;
			}
		}
	default:
		return -EINVAL;
	}

	return 0;
}

static void apm86xxx_suspend_wake(void)
{
	PM_TRACE("%s\n", __func__);

	if (!is_apm86xxx_lite()) {
		if (resumed_from_deepsleep()) {
			apm86xxx_ahbc_set_reg(ENABLE_INTF_ADDR, enable_intf);
			PM_DEBUG("CPU%d resumed from deepsleep\n",
				 MP_PROCESSOR_ID());
		} else {
			resume_console();
		}
	}
}

static void apm86xxx_suspend_finish(void)
{
	PM_TRACE("%s\n", __func__);
}

static void apm86xxx_suspend_end(void)
{
	PM_TRACE("%s\n", __func__);

	apm86xxx_pm_inprogress = 0;

	if (!is_apm86xxx_lite()) {
		if (ipp_pwr_state)
		 	ipp_pwr_state->poweron_flag = 0;

		/* Now enable ppc44x_idle and if SMP, also
		 * Disable Snoop Inihibit */
		ppc44x_idle_enable();

		if (apm86xxx_pm->wakeup_ledmsk)
			apm86xxx_clear_led(apm86xxx_pm->wakeup_ledmsk);
	}
}

void apm86xxx_suspend_disable_irqs(void)
{
	unsigned int cpu = MP_PROCESSOR_ID();
	struct irq_desc *desc;
	int irq;
	(void)cpu;

	PM_TRACE("%s\n", __func__);

	/* disable decrement interrupt */
	apm86xxx_pm->tcr_save = mfspr(SPRN_TCR);
	mtspr(SPRN_TCR, apm86xxx_pm->tcr_save & ~TCR_DIE);
	
	/*
	 * Note, we need to enable IPP irq because it was disabled by 
	 *     suspend_device_irqs() : linux/kernel/irq/pm.c
	 * Which was called by 
	 *    dpm_suspend_noirq() : linux/drivers/base/power/main.c
	 * Which was called by 
	 *    suspend_enter() : linux/kernel/power/suspend.c	
	 *
	 * So let's enable IPP irq (for Mamba lite enable GPIO irq)
	 */

	if (!is_apm86xxx_lite())
		irq  = get_ipp_irq();
	else
		irq  = get_ipp_gpio_irq();
	
	desc = irq_to_desc(irq);

	if ((desc->status & IRQ_DISABLED) == IRQ_DISABLED &&
	    (desc->status & IRQ_SUSPENDED) == IRQ_SUSPENDED) {
		PM_DEBUG("CPU%d enable IPP irq\n", cpu);
		__enable_irq(desc, irq, true);
	}

#if 0
	/*
	 * Enable serial irq (use this to debug).
	 * With serial port enabled you can wakeup
	 * from standby mode by pressing any keyboard
	 * key. With it disabled you cannot.
	 */
	#define SERIAL_IRQ 17
	irq  = SERIAL_IRQ;
	desc = irq_to_desc(irq);
	if ((desc->status & IRQ_DISABLED) == IRQ_DISABLED &&
	    (desc->status & IRQ_SUSPENDED) == IRQ_SUSPENDED) {
		PM_DEBUG("CPU%d enable SERIAL irq (%d)\n", cpu, irq);
		__enable_irq(desc, irq, true);
	}
#endif
}

void apm86xxx_suspend_enable_irqs(void)
{
	PM_TRACE("%s\n", __func__);

	/* re-enable decrement interrupt */
	mtspr(SPRN_TCR, apm86xxx_pm->tcr_save);
}

static struct platform_suspend_ops apm86xxx_suspend_ops = {
	.valid		= apm86xxx_suspend_valid,
	.begin		= apm86xxx_suspend_begin,
	.prepare	= apm86xxx_suspend_prepare,
	.enter		= apm86xxx_suspend_enter,
	.wake		= apm86xxx_suspend_wake,
	.finish		= apm86xxx_suspend_finish,
	.end		= apm86xxx_suspend_end,
};

static int __init apm86xxx_suspend_lite_init(void)
{
	struct device_node *np;
	phys_addr_t p;
	int err = 0;

	lite.suspend = ocm_alloc(&p, apm86xxx_lite_suspend_size, LITE_ALIGN,
				OCM_NON_CACHED, "lite.suspend");
	lite.resume = ocm_alloc(&p, apm86xxx_lite_resume_size, LITE_ALIGN,
				OCM_NON_CACHED, "lite.resume");
	lite.data = ocm_alloc(&p, LITE_DATA_SIZE, LITE_ALIGN,
				OCM_NON_CACHED, "lite.data");
	lite.memc = 0;
	lite.scu = 0;

	printk(KERN_INFO "lite.suspend = 0x%p\n", lite.suspend);
	printk(KERN_INFO "lite.resume  = 0x%p\n", lite.resume);
	printk(KERN_INFO "lite.data    = 0x%p\n", lite.data);

        if ( !lite.suspend || !lite.resume ||!lite.data) {
                printk(KERN_ERR "lite: failed to allocate memory!\n");
		err = -ENOMEM;
		goto err_out;
        }

	memcpy(lite.suspend, apm86xxx_lite_suspend,
		apm86xxx_lite_suspend_size);
	memcpy(lite.resume, apm86xxx_lite_resume,
		apm86xxx_lite_resume_size);
	memset(lite.data, 0, LITE_DATA_SIZE);

	lite.data[LITE_RESUME] = (u32)(lite.resume);

	np = of_find_compatible_node(NULL, NULL, "apm,memc");
	if (np) {
		struct resource res;
		int ret = of_address_to_resource(np, 0, &res);
		if (!ret) {
			lite.memc = (u32)ioremap(res.start,
						 (res.end - res.start + 1));
			if (lite.memc)
				lite.data[LITE_MEMC/4] = lite.memc;
		}
	}

	lite.scu = (u32)apm86xxx_get_scu_base();
	lite.data[LITE_SCU/4] = lite.scu;

	return 0;
err_out:
	if (lite.suspend) {
		ocm_free(lite.suspend);
		lite.suspend = NULL;
	}

	if (lite.resume) {
		ocm_free(lite.resume);
		lite.resume = NULL;
	}

	if (lite.data) {
		ocm_free(lite.data);
		lite.data = NULL;
	}

	return err;
}

static int __init apm86xxx_suspend_init(void)
{
	suspend_set_ops(&apm86xxx_suspend_ops);

	ppc_md.suspend_disable_irqs = apm86xxx_suspend_disable_irqs;
	ppc_md.suspend_enable_irqs = apm86xxx_suspend_enable_irqs;

	apm86xxx_pm = apm86xxx_suspend_sysfs_init();

	if (!apm86xxx_pm)
		return -EFAULT;

	if (is_apm86xxx_lite()) {
		apm86xxx_suspend_lite_init();
	}

	return 0;
}

/* override weak function */
int __cpu_disable_prepare(void)
{
	/* disable decrement interrupt */
	mtspr(SPRN_TCR, mfspr(SPRN_TCR) & ~TCR_DIE);

	switch(system_state) {
	case SYSTEM_HALT:
	case SYSTEM_POWER_OFF:
	case SYSTEM_RESTART:
		return 0;
	default:
		if (is_apm86xxx_lite())
			return 0;
		else
			return apm86xxx_suspend_ipp_prepare();
	};
}

int is_wake_on_usb_enabled(void)
{
	return apm86xxx_pm->wakeup[WAKEUP_USB].enabled;
}
EXPORT_SYMBOL(is_wake_on_usb_enabled);

int suspending_to_deepsleep(void)
{
	return apm86xxx_pm_suspend_to_deepsleep;
}
EXPORT_SYMBOL(suspending_to_deepsleep);

int resumed_from_deepsleep(void)
{
	return apm86xxx_pm_resumed_from_deepsleep;
}
EXPORT_SYMBOL(resumed_from_deepsleep);

__initcall(apm86xxx_suspend_init);

