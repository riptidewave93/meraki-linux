/*
 * arch/arm/plat-ambarella/cortex/cpufreq.c
 *
 * Author: Cao Rongrong <rrcao@ambarella.com>
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

#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/regulator/consumer.h>
#include <linux/cpufreq.h>
#include <mach/hardware.h>
#include <plat/cpufreq.h>

static struct ambarella_cpufreq_config *cpufreq_config = NULL;

static int ambarella_verify_speed(struct cpufreq_policy *policy)
{
	return cpufreq_frequency_table_verify(policy, cpufreq_config->freq_table);
}

static unsigned int ambarella_getspeed(unsigned int cpu)
{
	if (cpu >= NR_CPUS)
		return 0;

	return amb_get_cortex_clock_frequency(HAL_BASE_VP) / 1000;
}

static int ambarella_target(struct cpufreq_policy *policy,
		       unsigned int target_freq,
		       unsigned int relation)
{
	int idx;
	amb_hal_success_t ret = AMB_HAL_SUCCESS;
	struct cpufreq_freqs freqs;

	cpufreq_frequency_table_target(policy, cpufreq_config->freq_table,
			target_freq, relation, &idx);

	freqs.old = ambarella_getspeed(0);
	freqs.new = cpufreq_config->freq_table[idx].frequency;
	freqs.cpu = policy->cpu;

	if (freqs.old == freqs.new)
		return 0;

	cpufreq_notify_transition(&freqs, CPUFREQ_PRECHANGE);

#ifdef CONFIG_CPU_FREQ_DEBUG
	printk(KERN_DEBUG "cpufreq-ambarella: transition: %u --> %u\n",
	       freqs.old, freqs.new);
#endif

	/* if moving to higher frequency, up the voltage beforehand */
	if (cpufreq_config && cpufreq_config->set_voltage && freqs.new > freqs.old) {
		ret = cpufreq_config->set_voltage(idx);
		if (ret)
			goto out;
	}

	ret = amb_set_cortex_clock_frequency(HAL_BASE_VP, freqs.new * 1000);
	if (ret != AMB_HAL_SUCCESS) {
		pr_err("cpu-ambarella: Failed to set cpu frequency to %d kHz\n",
			freqs.new);
		return ret;
	}

out:
	cpufreq_notify_transition(&freqs, CPUFREQ_POSTCHANGE);

	return 0;
}

static int ambarella_cpu_init(struct cpufreq_policy *policy)
{
	if (policy->cpu >= NR_CPUS)
		return -EINVAL;

	cpufreq_frequency_table_cpuinfo(policy, cpufreq_config->freq_table);
	cpufreq_frequency_table_get_attr(cpufreq_config->freq_table, policy->cpu);

	policy->cur = ambarella_getspeed(policy->cpu);
	/* FIXME: what's the actual transition time? */
	policy->cpuinfo.transition_latency = 300 * 1000;
	policy->shared_type = CPUFREQ_SHARED_TYPE_ALL;

	/* the frequency cannot be set independently. Each cpu is bound to the
	 * same speed. So the affected cpu is all of the cpus. */
	cpumask_setall(policy->cpus);

	return 0;
}

static int ambarella_cpu_exit(struct cpufreq_policy *policy)
{
	cpufreq_frequency_table_put_attr(policy->cpu);
	return 0;
}

static struct freq_attr *ambarella_cpufreq_attr[] = {
	&cpufreq_freq_attr_scaling_available_freqs,
	NULL,
};

static struct cpufreq_driver ambarella_driver = {
	.verify		= ambarella_verify_speed,
	.target		= ambarella_target,
	.get		= ambarella_getspeed,
	.init		= ambarella_cpu_init,
	.exit		= ambarella_cpu_exit,
	.name		= "ambarella",
	.attr		= ambarella_cpufreq_attr,
};

static int __init ambarella_cpufreq_probe(struct platform_device *pdev)
{
	int rval;

	cpufreq_config = pdev->dev.platform_data;

	if (!cpufreq_config)
		return -EINVAL;
	if (!cpufreq_config->freq_table)
		return -EINVAL;

	if (cpufreq_config->init) {
		rval = cpufreq_config->init();
		if (rval)
			return rval;
	}

	return cpufreq_register_driver(&ambarella_driver);
}

static int __exit ambarella_cpufreq_remove(struct platform_device *pdev)
{
	if (cpufreq_config && cpufreq_config->exit)
		cpufreq_config->exit();

	return cpufreq_unregister_driver(&ambarella_driver);
}

static struct platform_driver ambarella_cpufreq_driver = {
	.driver = {
		.name	 = "cpufreq-ambarella",
		.owner	 = THIS_MODULE,
	},
	.remove = __exit_p(ambarella_cpufreq_remove),
};

static int __init ambarella_cpufreq_init(void)
{
	return platform_driver_probe(&ambarella_cpufreq_driver,
			ambarella_cpufreq_probe);
}
late_initcall(ambarella_cpufreq_init);

