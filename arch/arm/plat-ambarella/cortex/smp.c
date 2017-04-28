/*
 * arch/arm/plat-ambarella/cortex/smp.c
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

#include <linux/init.h>
#include <linux/errno.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/jiffies.h>
#include <linux/smp.h>
#include <linux/io.h>
#include <linux/bootmem.h>

#include <asm/cacheflush.h>
#include <asm/mach-types.h>
#include <asm/localtimer.h>
#include <asm/unified.h>
#include <asm/smp_scu.h>
#include <asm/hardware/gic.h>

#include <mach/hardware.h>
#include <plat/ambcache.h>
#include <mach/common.h>
#include <mach/smp.h>

/* ==========================================================================*/
extern void ambarella_secondary_startup(void);

static void __iomem *scu_base = __io(AMBARELLA_VA_SCU_BASE);
static DEFINE_SPINLOCK(boot_lock);
#ifdef CONFIG_HOTPLUG_CPU
static DECLARE_COMPLETION(cpu_killed);
#endif
static unsigned int smp_max_cpus = 0;
#ifdef CONFIG_OUTER_CACHE
static unsigned int smp_l2_mode = 0;
#endif

/* ==========================================================================*/
static void __cpuinit ambarella_smp_secondary_init(unsigned int cpu)
{
	gic_secondary_init(0);

	spin_lock(&boot_lock);
#ifdef CONFIG_OUTER_CACHE
	if (smp_l2_mode)
		ambcache_l2_enable_raw();
#endif
	spin_unlock(&boot_lock);
}

static int __cpuinit ambarella_smp_boot_secondary(unsigned int cpu,
	struct task_struct *idle)
{
	u32					timeout = 100000;
	u32					*phead_address;
	int					retval = 0;

	spin_lock(&boot_lock);

	phead_address = get_ambarella_bstmem_head();
	if (phead_address == (u32 *)AMB_BST_INVALID) {
		pr_err("Can't find SMP BST Header!\n");
		retval = -EPERM;
		goto boot_secondary_exit;
	}

#ifdef CONFIG_OUTER_CACHE
	smp_l2_mode = outer_is_enabled();
	if (smp_l2_mode)
		ambcache_l2_disable_raw();
#endif

	phead_address[PROCESSOR_START_0 + cpu] =
		virt_to_phys(ambarella_secondary_startup);
	phead_address[PROCESSOR_STATUS_0 + cpu] = AMB_BST_START_COUNTER;
	ambcache_flush_range((void *)(phead_address),
		AMBARELLA_BST_HEAD_CACHE_SIZE);
	smp_cross_call(cpumask_of(cpu), 1);
	while (timeout) {
		ambcache_inv_range((void *)(phead_address),
			AMBARELLA_BST_HEAD_CACHE_SIZE);
		if (phead_address[PROCESSOR_START_0 + cpu] == AMB_BST_INVALID)
			break;
		udelay(10);
		timeout--;
	}
	if (phead_address[PROCESSOR_STATUS_0 + cpu] > 0) {
		pr_err("CPU%d: spurious wakeup %d times.\n", cpu,
			phead_address[PROCESSOR_STATUS_0 + cpu]);
	}
	if (phead_address[PROCESSOR_START_0 + cpu] != AMB_BST_INVALID) {
		pr_err("CPU%d: tmo[%d] [0x%08x].\n", cpu, timeout,
			phead_address[PROCESSOR_START_0 + cpu]);
		retval = -EPERM;
	}

boot_secondary_exit:
	spin_unlock(&boot_lock);

	return retval;
}

static void __init ambarella_smp_init_cpus(void)
{
	int					i;
	unsigned int				ncores;

	ncores = scu_get_core_count(scu_base);
	if (ncores > NR_CPUS) {
		pr_warning("%s: no. of cores (%d) greater than configured\n"
			"maximum of %d - clipping\n",
			__func__, ncores, NR_CPUS);
		ncores = NR_CPUS;
	}
	for (i = 0; i < ncores; i++)
		set_cpu_possible(i, true);

	set_smp_cross_call(gic_raise_softirq);
}

static void __init ambarella_smp_prepare_cpus(unsigned int max_cpus)
{
	int					i;
	u32					bstadd;
	u32					bstsize;
	u32					*phead_address;

	if (get_ambarella_bstmem_info(&bstadd, &bstsize) != AMB_BST_MAGIC) {
		pr_err("Can't find SMP BST!\n");
		return;
	}
	phead_address = get_ambarella_bstmem_head();
	if (phead_address == (u32 *)AMB_BST_INVALID) {
		pr_err("Can't find SMP BST Head!\n");
		return;
	}

	for (i = 0; i < max_cpus; i++)
		set_cpu_present(i, true);
	scu_enable(scu_base);
	for (i = 1; i < max_cpus; i++) {
		phead_address[PROCESSOR_START_0 + i] =
			virt_to_phys(ambarella_secondary_startup);
		phead_address[PROCESSOR_STATUS_0 + i] = AMB_BST_START_COUNTER;
	}
	ambcache_flush_range((void *)(phead_address),
		AMBARELLA_BST_HEAD_CACHE_SIZE);
	smp_max_cpus = max_cpus;
}

u32 arch_smp_suspend(u32 level)
{
	scu_disable(scu_base);

	return 0;
}

u32 arch_smp_resume(u32 level)
{
	scu_enable(scu_base);

	return 0;
}

/* ==========================================================================*/
#ifdef CONFIG_HOTPLUG_CPU
static inline void cpu_enter_lowpower(void)
{
	unsigned int v;

	flush_cache_all();
	asm volatile(
	"	mrc	p15, 0, %0, c1, c0, 1\n"
	"	bic	%0, %0, #(1 << 6)\n"
	"	bic	%0, %0, #(1 << 0)\n"
	"	mcr	p15, 0, %0, c1, c0, 1\n"
	"	mrc	p15, 0, %0, c1, c0, 0\n"
	"	bic	%0, %0, #(1 << 2)\n"
	"	mcr	p15, 0, %0, c1, c0, 0\n"
		: "=&r" (v)
		: "r" (0)
		: "cc");
}

static inline void cpu_leave_lowpower(void)
{
	unsigned int v;

	asm volatile(
	"	mrc	p15, 0, %0, c1, c0, 0\n"
	"	orr	%0, %0, #(1 << 2)\n"
	"	mcr	p15, 0, %0, c1, c0, 0\n"
	"	mrc	p15, 0, %0, c1, c0, 1\n"
	"	orr	%0, %0, #(1 << 6)\n"
	"	orr	%0, %0, #(1 << 0)\n"
	"	mcr	p15, 0, %0, c1, c0, 1\n"
		: "=&r" (v)
		:
		: "cc");
}

static int ambarella_smp_cpu_kill(unsigned int cpu)
{
	return wait_for_completion_timeout(&cpu_killed, 5000);
}

static void ambarella_smp_cpu_die(unsigned int cpu)
{
	u32					*phead_address;

	phead_address = get_ambarella_bstmem_head();
	BUG_ON(phead_address == (u32 *)AMB_BST_INVALID);

	phead_address[PROCESSOR_STATUS_0 + cpu] = AMB_BST_START_COUNTER;
	phead_address[PROCESSOR_START_0 + cpu] = AMB_BST_INVALID;
	complete(&cpu_killed);
	cpu_enter_lowpower();
	for (;;) {
		asm volatile("wfi" : : : "memory", "cc");
		phead_address[PROCESSOR_STATUS_0 + cpu]++;
		if (phead_address[PROCESSOR_START_0 + cpu] != AMB_BST_INVALID) {
			phead_address[PROCESSOR_START_0 + cpu] =
				AMB_BST_INVALID;
			break;
		}
	}
	cpu_leave_lowpower();
}

static int ambarella_smp_cpu_disable(unsigned int cpu)
{
	u32 *phead_address = get_ambarella_bstmem_head();

	if (phead_address == (u32 *)AMB_BST_INVALID)
		return -EPERM;
	if (cpu > (PROCESSOR_STATUS_3 - PROCESSOR_STATUS_0))
		return -EPERM;

	return cpu == 0 ? -EPERM : 0;
}
#endif

struct smp_operations ambarella_smp_ops __initdata = {
	.smp_init_cpus		= ambarella_smp_init_cpus,
	.smp_prepare_cpus	= ambarella_smp_prepare_cpus,
	.smp_secondary_init	= ambarella_smp_secondary_init,
	.smp_boot_secondary	= ambarella_smp_boot_secondary,
#ifdef CONFIG_HOTPLUG_CPU
	.cpu_kill		= ambarella_smp_cpu_kill,
	.cpu_die		= ambarella_smp_cpu_die,
	.cpu_disable		= ambarella_smp_cpu_disable,
#endif
};
