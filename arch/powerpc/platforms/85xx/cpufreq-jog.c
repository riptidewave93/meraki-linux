/*
 * Copyright (C) 2008-2012 Freescale Semiconductor, Inc.
 * Author: Dave Liu <daveliu@freescale.com>
 * Modifier: Chenhui Zhao <chenhui.zhao@freescale.com>
 *
 * The cpufreq driver is for Freescale 85xx processor,
 * based on arch/powerpc/platforms/cell/cbe_cpufreq.c
 * (C) Copyright IBM Deutschland Entwicklung GmbH 2005-2007
 *	Christian Krafft <krafft@de.ibm.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <linux/module.h>
#include <linux/cpufreq.h>
#include <linux/of_platform.h>
#include <linux/suspend.h>
#include <linux/cpu.h>

#include <asm/prom.h>
#include <asm/time.h>
#include <asm/reg.h>
#include <asm/io.h>
#include <asm/machdep.h>
#include <asm/smp.h>

#include <sysdev/fsl_soc.h>

static DEFINE_MUTEX(mpc85xx_switch_mutex);
static void __iomem *guts;

static u32 sysfreq;
static unsigned int max_pll[2];
static atomic_t in_jog_process;
static struct cpufreq_frequency_table *mpc85xx_freqs;
static int (*set_pll)(unsigned int cpu, unsigned int pll);

static struct cpufreq_frequency_table mpc8536_freqs_table[] = {
	{3,	0},
	{4,	0},
	{5,	0},
	{6,	0},
	{7,	0},
	{8,	0},
	{0,	CPUFREQ_TABLE_END},
};

static struct cpufreq_frequency_table p1022_freqs_table[] = {
	{2,	0},
	{3,	0},
	{4,	0},
	{5,	0},
	{6,	0},
	{7,	0},
	{8,	0},
	{0,	CPUFREQ_TABLE_END},
};

#define FREQ_500MHz	500000000
#define FREQ_800MHz	800000000

#define CORE_RATIO_STRIDE	8
#define CORE_RATIO_MASK		0x3f
#define CORE_RATIO_SHIFT	16

#define PORPLLSR	0x0	/* Power-On Reset PLL ratio status register */

#define PMJCR		0x7c	/* Power Management Jog Control Register */
#define PMJCR_CORE0_SPD	0x00001000
#define PMJCR_CORE_SPD	0x00002000

#define POWMGTCSR	0x80 /* Power management control and status register */
#define POWMGTCSR_JOG		0x00200000
#define POWMGTCSR_INT_MASK	0x00000f00

static void spin_while_jogging(void *dummy)
{
	unsigned long flags;

	local_irq_save(flags);

	atomic_inc(&in_jog_process);

	while (atomic_read(&in_jog_process) != 0)
		barrier();

	local_irq_restore(flags);
}

static int get_pll(int hw_cpu)
{
	int shift;
	u32 val = in_be32(guts + PORPLLSR);

	shift = hw_cpu * CORE_RATIO_STRIDE + CORE_RATIO_SHIFT;

	return (val >> shift) & CORE_RATIO_MASK;
}

static int mpc8536_set_pll(unsigned int cpu, unsigned int pll)
{
	u32 corefreq, val, mask;
	unsigned int cur_pll = get_pll(0);
	unsigned long flags;

	if (pll == cur_pll)
		return 0;

	val = (pll & CORE_RATIO_MASK) << CORE_RATIO_SHIFT;

	corefreq = sysfreq * pll / 2;
	/*
	 * Set the COREx_SPD bit if the requested core frequency
	 * is larger than the threshold frequency.
	 */
	if (corefreq > FREQ_800MHz)
			val |= PMJCR_CORE_SPD;

	mask = (CORE_RATIO_MASK << CORE_RATIO_SHIFT) | PMJCR_CORE_SPD;
	clrsetbits_be32(guts + PMJCR, mask, val);

	/* readback to sync write */
	in_be32(guts + PMJCR);

	local_irq_save(flags);
	mpc85xx_enter_jog(get_immrbase(), POWMGTCSR_JOG);
	local_irq_restore(flags);

	/* verify */
	cur_pll =  get_pll(0);
	if (cur_pll != pll) {
		pr_err("%s: error. The current PLL of core 0 is %d instead of %d.\n",
				__func__, cur_pll, pll);
		return -1;
	}

	return 0;
}

static int p1022_set_pll(unsigned int cpu, unsigned int pll)
{
	int index, hw_cpu = get_hard_smp_processor_id(cpu);
	int shift;
	u32 corefreq, val, mask = 0;
	unsigned int cur_pll = get_pll(hw_cpu);
	unsigned long flags;
	int ret = 0;

	if (pll == cur_pll)
		return 0;

	shift = hw_cpu * CORE_RATIO_STRIDE + CORE_RATIO_SHIFT;
	val = (pll & CORE_RATIO_MASK) << shift;

	corefreq = sysfreq * pll / 2;
	/*
	 * Set the COREx_SPD bit if the requested core frequency
	 * is larger than the threshold frequency.
	 */
	if (corefreq > FREQ_500MHz)
		val |= PMJCR_CORE0_SPD << hw_cpu;

	mask = (CORE_RATIO_MASK << shift) | (PMJCR_CORE0_SPD << hw_cpu);
	clrsetbits_be32(guts + PMJCR, mask, val);

	/* readback to sync write */
	in_be32(guts + PMJCR);

	cpu_hotplug_disable();
	/*
	 * A Jog request can not be asserted when any core is in a low
	 * power state on P1022. Before executing a jog request, any
	 * core which is in a low power state must be waked by a
	 * interrupt, and keep waking up until the sequence is
	 * finished.
	 */
	for_each_present_cpu(index) {
		if (!cpu_online(index)) {
			cpu_hotplug_enable();
			pr_err("%s: error, core%d is down.\n", __func__, index);
			return -1;
		}
	}

	atomic_set(&in_jog_process, 0);
	smp_call_function(spin_while_jogging, NULL, 0);

	local_irq_save(flags);

	/* Wait for the other core to wake. */
	if (!spin_event_timeout(atomic_read(&in_jog_process) == 1, 1000, 100)) {
		pr_err("%s: timeout, the other core is not at running state.\n",
					__func__);
		ret = -1;
		goto err;
	}

	out_be32(guts + POWMGTCSR, POWMGTCSR_JOG | POWMGTCSR_INT_MASK);

	if (!spin_event_timeout(
		(in_be32(guts + POWMGTCSR) & POWMGTCSR_JOG) == 0, 1000, 100)) {
		pr_err("%s: timeout, fail to switch the core frequency.\n",
				__func__);
		ret = -1;
		goto err;
	}

	clrbits32(guts + POWMGTCSR, POWMGTCSR_INT_MASK);
	in_be32(guts + POWMGTCSR);

	atomic_set(&in_jog_process, 0);
err:
	local_irq_restore(flags);
	cpu_hotplug_enable();

	/* verify */
	cur_pll =  get_pll(hw_cpu);
	if (cur_pll != pll) {
		pr_err("%s: error, the current PLL of core %d is %d instead of %d.\n",
				__func__, hw_cpu, cur_pll, pll);
		return -1;
	}

	return ret;
}

/*
 * cpufreq functions
 */
static int mpc85xx_cpufreq_cpu_init(struct cpufreq_policy *policy)
{
	unsigned int i, cur_pll;
	int hw_cpu = get_hard_smp_processor_id(policy->cpu);

	if (!cpu_present(policy->cpu))
		return -ENODEV;

	/* the latency of a transition, the unit is ns */
	policy->cpuinfo.transition_latency = 2000;

	cur_pll = get_pll(hw_cpu);

	/* initialize frequency table */
	pr_debug("core%d frequency table:\n", hw_cpu);
	for (i = 0; mpc85xx_freqs[i].frequency != CPUFREQ_TABLE_END; i++) {
		if (mpc85xx_freqs[i].index <= max_pll[hw_cpu]) {
			/* The frequency unit is kHz. */
			mpc85xx_freqs[i].frequency =
				(sysfreq * mpc85xx_freqs[i].index / 2) / 1000;
		} else {
			mpc85xx_freqs[i].frequency = CPUFREQ_ENTRY_INVALID;
		}

		pr_debug("%d: %dkHz\n", i, mpc85xx_freqs[i].frequency);

		if (mpc85xx_freqs[i].index == cur_pll)
			policy->cur = mpc85xx_freqs[i].frequency;
	}
	pr_debug("current pll is at %d, and core freq is%d\n",
			cur_pll, policy->cur);

	cpufreq_frequency_table_get_attr(mpc85xx_freqs, policy->cpu);

	/*
	 * This ensures that policy->cpuinfo_min
	 * and policy->cpuinfo_max are set correctly.
	 */
	return cpufreq_frequency_table_cpuinfo(policy, mpc85xx_freqs);
}

static int mpc85xx_cpufreq_cpu_exit(struct cpufreq_policy *policy)
{
	cpufreq_frequency_table_put_attr(policy->cpu);

	return 0;
}

static int mpc85xx_cpufreq_verify(struct cpufreq_policy *policy)
{
	return cpufreq_frequency_table_verify(policy, mpc85xx_freqs);
}

static int mpc85xx_cpufreq_target(struct cpufreq_policy *policy,
			      unsigned int target_freq,
			      unsigned int relation)
{
	struct cpufreq_freqs freqs;
	unsigned int new;
	int ret = 0;

	if (!set_pll)
		return -ENODEV;

	cpufreq_frequency_table_target(policy,
				       mpc85xx_freqs,
				       target_freq,
				       relation,
				       &new);

	freqs.old = policy->cur;
	freqs.new = mpc85xx_freqs[new].frequency;
	freqs.cpu = policy->cpu;

	mutex_lock(&mpc85xx_switch_mutex);
	cpufreq_notify_transition(&freqs, CPUFREQ_PRECHANGE);

	ret = set_pll(policy->cpu, mpc85xx_freqs[new].index);
	if (!ret) {
		pr_info("cpufreq: Setting core%d frequency to %d kHz and PLL ratio to %d:2\n",
			 policy->cpu, mpc85xx_freqs[new].frequency,
			 mpc85xx_freqs[new].index);

		ppc_proc_freq = freqs.new * 1000ul;
	}
	cpufreq_notify_transition(&freqs, CPUFREQ_POSTCHANGE);
	mutex_unlock(&mpc85xx_switch_mutex);

	return ret;
}

static struct cpufreq_driver mpc85xx_cpufreq_driver = {
	.verify		= mpc85xx_cpufreq_verify,
	.target		= mpc85xx_cpufreq_target,
	.init		= mpc85xx_cpufreq_cpu_init,
	.exit		= mpc85xx_cpufreq_cpu_exit,
	.name		= "mpc85xx-JOG",
	.owner		= THIS_MODULE,
	.flags		= CPUFREQ_CONST_LOOPS,
};

static int mpc85xx_job_probe(struct platform_device *ofdev)
{
	struct device_node *np = ofdev->dev.of_node;
	unsigned int svr;

	if (of_device_is_compatible(np, "fsl,mpc8536-guts")) {
		svr = mfspr(SPRN_SVR);
		if ((svr & 0x7fff) == 0x10) {
			pr_err("MPC8536 Rev 1.0 do not support JOG.\n");
			return -ENODEV;
		}
		mpc85xx_freqs = mpc8536_freqs_table;
		set_pll = mpc8536_set_pll;
	} else if (of_device_is_compatible(np, "fsl,p1022-guts")) {
		mpc85xx_freqs = p1022_freqs_table;
		set_pll = p1022_set_pll;
	} else {
		return -ENODEV;
	}

	sysfreq = fsl_get_sys_freq();

	guts = of_iomap(np, 0);
	if (!guts)
		return -ENODEV;

	max_pll[0] = get_pll(0);
	if (mpc85xx_freqs == p1022_freqs_table)
		max_pll[1] = get_pll(1);

	pr_info("Freescale MPC85xx CPU frequency switching(JOG) driver\n");

	return cpufreq_register_driver(&mpc85xx_cpufreq_driver);
}

static int mpc85xx_jog_remove(struct platform_device *ofdev)
{
	iounmap(guts);
	cpufreq_unregister_driver(&mpc85xx_cpufreq_driver);

	return 0;
}

static struct of_device_id mpc85xx_jog_ids[] = {
	{ .compatible = "fsl,mpc8536-guts", },
	{ .compatible = "fsl,p1022-guts", },
	{}
};

static struct platform_driver mpc85xx_jog_driver = {
	.driver = {
		.name = "mpc85xx_cpufreq_jog",
		.owner = THIS_MODULE,
		.of_match_table = mpc85xx_jog_ids,
	},
	.probe = mpc85xx_job_probe,
	.remove = mpc85xx_jog_remove,
};

static int __init mpc85xx_jog_init(void)
{
	return platform_driver_register(&mpc85xx_jog_driver);
}

static void __exit mpc85xx_jog_exit(void)
{
	platform_driver_unregister(&mpc85xx_jog_driver);
}

module_init(mpc85xx_jog_init);
module_exit(mpc85xx_jog_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dave Liu <daveliu@freescale.com>");
