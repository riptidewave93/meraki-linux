/*
 *  linux/arch/arm/plat-omap/cpu-omap.c
 *
 *  CPU frequency scaling for OMAP
 *
 *  Copyright (C) 2005 Nokia Corporation
 *  Written by Tony Lindgren <tony@atomide.com>
 *
 *  Based on cpu-sa1110.c, Copyright (C) 2001 Russell King
 *
 * Copyright (C) 2007-2008 Texas Instruments, Inc.
 * Updated to support OMAP3
 * Rajendra Nayak <rnayak@ti.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/cpufreq.h>
#include <linux/delay.h>
#include <linux/init.h>
#include <linux/err.h>
#include <linux/clk.h>
#include <linux/io.h>
#include <linux/opp.h>

#include <mach/hardware.h>
#include <plat/clock.h>
#include <asm/system.h>

#if (defined(CONFIG_ARCH_OMAP3) || defined(CONFIG_ARCH_TI814X)) && \
	!defined(CONFIG_OMAP_PM_NONE)
#include <plat/omap-pm.h>
#include <plat/common.h>
#endif
#include <plat/omap_device.h>

#if defined(CONFIG_ARCH_TI814X)
#include <linux/regulator/consumer.h>
#define DEFAULT_RESOLUTION 12500
#endif

#define VERY_HI_RATE	900000000

static struct cpufreq_frequency_table *freq_table;

#ifdef CONFIG_ARCH_OMAP1
#define MPU_CLK		"mpu"
#elif defined(CONFIG_ARCH_OMAP3)
#define MPU_CLK		"arm_fck"
#elif defined(CONFIG_ARCH_TI814X)
#define MPU_CLK		"arm_dpll_ck"
#else
#define MPU_CLK		"virt_prcm_set"
#endif

static struct clk *mpu_clk;
#ifdef CONFIG_ARCH_TI814X
static struct regulator *mpu_reg;
#endif

/* TODO: Add support for SDRAM timing changes */

static int omap_verify_speed(struct cpufreq_policy *policy)
{
	if (freq_table)
		return cpufreq_frequency_table_verify(policy, freq_table);

	if (policy->cpu)
		return -EINVAL;

	cpufreq_verify_within_limits(policy, policy->cpuinfo.min_freq,
				     policy->cpuinfo.max_freq);

	policy->min = clk_round_rate(mpu_clk, policy->min * 1000) / 1000;
	policy->max = clk_round_rate(mpu_clk, policy->max * 1000) / 1000;
	cpufreq_verify_within_limits(policy, policy->cpuinfo.min_freq,
				     policy->cpuinfo.max_freq);
	return 0;
}

static unsigned int omap_getspeed(unsigned int cpu)
{
	unsigned long rate;

	if (cpu)
		return 0;

	rate = clk_get_rate(mpu_clk) / 1000;
	return rate;
}

static int omap_target(struct cpufreq_policy *policy,
		       unsigned int target_freq,
		       unsigned int relation)
{
#if defined(CONFIG_ARCH_OMAP1) || defined(CONFIG_ARCH_TI814X)
	struct cpufreq_freqs freqs;
#endif
#if defined(CONFIG_ARCH_OMAP3) || defined(CONFIG_ARCH_TI814X)
	unsigned long freq;
	struct device *mpu_dev = omap2_get_mpuss_device();
#endif
#if defined(CONFIG_ARCH_TI814X)
	struct opp *opp;
	int volt_old = 0, volt_new = 0;
#endif
	int ret = 0;

	/* Ensure desired rate is within allowed range.  Some govenors
	 * (ondemand) will just pass target_freq=0 to get the minimum. */
	if (target_freq < policy->min)
		target_freq = policy->min;
	if (target_freq > policy->max)
		target_freq = policy->max;

#if defined(CONFIG_ARCH_OMAP1) || defined(CONFIG_ARCH_TI814X)
	freqs.old = omap_getspeed(0);
	freqs.new = clk_round_rate(mpu_clk, target_freq * 1000) / 1000;
	freqs.cpu = 0;

	if (freqs.old == freqs.new)
		return ret;
#if defined(CONFIG_ARCH_TI814X)
	freq = target_freq * 1000; /* fix warning */
	opp = opp_find_freq_exact(mpu_dev, freqs.new * 1000, true);
	if (IS_ERR(opp)) {
		dev_err(mpu_dev, "%s: cpu%d: no opp match for freq %d\n",
			__func__, policy->cpu, target_freq);
		return -EINVAL;
	}

	volt_new = opp_get_voltage(opp);
	if (!volt_new) {
		dev_err(mpu_dev, "%s: cpu%d: no opp voltage for freq %d\n",
			__func__, policy->cpu, target_freq);
		return -EINVAL;
	}

	volt_old = regulator_get_voltage(mpu_reg);

#ifdef CONFIG_CPU_FREQ_DEBUG
	pr_info("cpufreq-omap: frequency transition: %u --> %u\n",
			freqs.old, freqs.new);
	pr_info("cpufreq-omap: voltage transition: %d --> %d\n",
			volt_old, volt_new);
#endif

	if (freqs.new > freqs.old) {
		ret = regulator_set_voltage(mpu_reg, volt_new,
			volt_new + DEFAULT_RESOLUTION - 1);
		if (ret) {
			dev_err(mpu_dev, "%s: unable to set voltage to %d uV (for %u MHz)\n",
				__func__, volt_new, freqs.new/1000);
			return ret;
		}
	}
	/* Notify the registered users that we gonna change frequency */
	cpufreq_notify_transition(&freqs, CPUFREQ_PRECHANGE);

	ret = clk_set_rate(mpu_clk, freqs.new * 1000);
	freqs.new = omap_getspeed(policy->cpu);

	/* Notify the registered users that frequency is changed */
	cpufreq_notify_transition(&freqs, CPUFREQ_POSTCHANGE);

	if (freqs.new < freqs.old) {
		ret = regulator_set_voltage(mpu_reg, volt_new,
			volt_new + DEFAULT_RESOLUTION - 1);
		if (ret) {
			dev_err(mpu_dev, "%s: unable to set voltage to %d uV (for %u MHz)\n",
				__func__, volt_new, freqs.new/1000);

			if (clk_set_rate(mpu_clk, freqs.old * 1000)) {
				dev_err(mpu_dev,
					"%s: failed restoring clock rate to %u MHz, clock rate is %u MHz",
					__func__,
					freqs.old/1000, freqs.new/1000);
				return ret;
			}

			return ret;
		}
	}
#else
	cpufreq_notify_transition(&freqs, CPUFREQ_PRECHANGE);
#ifdef CONFIG_CPU_FREQ_DEBUG
	printk(KERN_DEBUG "cpufreq-omap: transition: %u --> %u\n",
	       freqs.old, freqs.new);
#endif
	ret = clk_set_rate(mpu_clk, freqs.new * 1000);
	cpufreq_notify_transition(&freqs, CPUFREQ_POSTCHANGE);
#endif
#elif defined(CONFIG_ARCH_OMAP3)
	freq = target_freq * 1000;
	if (opp_find_freq_ceil(mpu_dev, &freq))
		omap_device_scale(mpu_dev, mpu_dev, freq);
#endif
	return ret;
}

static int omap_cpu_init(struct cpufreq_policy *policy)
{
	int result = 0;

	mpu_clk = clk_get(NULL, MPU_CLK);
	if (IS_ERR(mpu_clk))
		return PTR_ERR(mpu_clk);

#if defined(CONFIG_ARCH_TI814X)
	mpu_reg = regulator_get(NULL, "mpu");
	if (IS_ERR(mpu_reg)) {
		result = -EINVAL;
		pr_err("%s:Regulatro get failed\n", __func__);
		goto fail_reg;
	}
#endif
	if (policy->cpu != 0)
		return -EINVAL;

	policy->cur = policy->min = policy->max = omap_getspeed(0);

	if (!cpu_is_omap34xx() && !cpu_is_ti814x()) {
		clk_init_cpufreq_table(&freq_table);
	} else {
		struct device *mpu_dev = omap2_get_mpuss_device();

		opp_init_cpufreq_table(mpu_dev, &freq_table);
	}

	if (freq_table) {
		result = cpufreq_frequency_table_cpuinfo(policy, freq_table);
		if (!result)
			cpufreq_frequency_table_get_attr(freq_table,
							policy->cpu);
	} else {
		policy->cpuinfo.min_freq = clk_round_rate(mpu_clk, 0) / 1000;
		policy->cpuinfo.max_freq = clk_round_rate(mpu_clk,
							VERY_HI_RATE) / 1000;
	}

	policy->min = policy->cpuinfo.min_freq;
	policy->max = policy->cpuinfo.max_freq;
	policy->cur = omap_getspeed(0);

	/* FIXME: what's the actual transition time? */
	policy->cpuinfo.transition_latency = 300 * 1000;

	return 0;
fail_reg:
	clk_put(mpu_clk);
	return result;
}

static int omap_cpu_exit(struct cpufreq_policy *policy)
{
	clk_exit_cpufreq_table(&freq_table);
	clk_put(mpu_clk);
	return 0;
}

static struct freq_attr *omap_cpufreq_attr[] = {
	&cpufreq_freq_attr_scaling_available_freqs,
	NULL,
};

static struct cpufreq_driver omap_driver = {
	.flags		= CPUFREQ_STICKY,
	.verify		= omap_verify_speed,
	.target		= omap_target,
	.get		= omap_getspeed,
	.init		= omap_cpu_init,
	.exit		= omap_cpu_exit,
	.name		= "omap",
	.attr		= omap_cpufreq_attr,
};

static int __init omap_cpufreq_init(void)
{
	return cpufreq_register_driver(&omap_driver);
}

late_initcall(omap_cpufreq_init);

/*
 * if ever we want to remove this, upon cleanup call:
 *
 * cpufreq_unregister_driver()
 * cpufreq_frequency_table_put_attr()
 */

