/*
 * APM86xxx cpufreq driver
 *
 * Copyright (c) 2010 Applied Micro Circuits Corporation.
 * All rights reserved. Victor Gallardo <vgallardo@apm.com>.
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
#include <linux/module.h>
#include <linux/init.h>
#include <linux/err.h>
#include <linux/clk.h>
#include <linux/cpufreq.h>
#include <include/asm/time.h> /* to get ppc_proc_freq */
#include <asm/ipp.h>
#include <asm/apm86xxx_soc.h>
#include <linux/delay.h>

#define DECREMENTER_MAX 0x7fffffff
extern unsigned long ppc_tb_freq;
void tb_update_recal(void);

#define CONFIG_PM_DEBUG_TRACE 0
#if CONFIG_PM_DEBUG_TRACE
#       define PM_TRACE(fmt, arg...) printk(KERN_INFO fmt, ## arg)
#else
#       define PM_TRACE(fmt, arg...) ((void)0)
#endif

#if defined(CONFIG_PM_DEBUG)
#       define PM_DEBUG(fmt, arg...) printk(KERN_INFO fmt, ## arg)
#else
#       define PM_DEBUG(fmt, arg...) ((void)0)
#endif

#define MP_PROCESSOR_ID() (unsigned int)mfspr(SPRN_PIR)
#define MP_NR_CPUS 2

/*
 * valid CPU divisors
 */
static const unsigned int cdivisors[] = {
	2, 3, 4, 5, 6, 8, 10, 12, 16, 20, 24, 32
};
#define CPUFREQ_TABLE_SIZE    ARRAY_SIZE(cdivisors)

/* 
 * CPU frequency table to be populated once we know the PLL frequency.
 * Calulated by PLL_FREQ/CPU_DIVISOR. Frequency in kHz to be
 * consistent with Linux interface.
 */
static struct cpufreq_frequency_table cpufreq_table[CPUFREQ_TABLE_SIZE+1];

/*
 * valid PLB divisors (plb_div = 2 * plb2x_div)
 *
 * NOTE1: All calculations are based in plb_divisor not plb2x_divisor
 *        to reduce calculation.
 * NOTE2: It is always safe to return CPUs to a CPU dividor value
 *        of 2 (cpu_idx = 0) prior to changing the PLB frequency,
 *        as all PLB2X divisors values are allowed with that setting.
 */
static const unsigned int pdivisors[] = {
	4, 6, 8, 10, 12, 16, 20, 24, 32
};
#define PLBFREQ_TABLE_SIZE    ARRAY_SIZE(pdivisors)

/* 
 * PLB frequency table to be populated once we know the PLL frequency.
 * Calulated by PLL_FREQ/PLB_DIVISOR. Frequency in kHz to be
 * consistent with Linux interface.
 */
static unsigned int plbfreq_table[PLBFREQ_TABLE_SIZE];

static struct cpuinfo_dev {
        unsigned int init;		/* initialized or not */
        unsigned int hbf;		/* hbf frequency */
        unsigned int pll;		/* PLL frequency is shared */
        int plb_fixed;			/* PLB frequency is fixed  */
        unsigned int plb_idx;		/* PLB frequency is shared */
        unsigned int plb_min;		/* PLB min frequency */
        unsigned int plb_max;		/* PLB max frequency */
        unsigned int plb_set;		/* PLB set frequency (pending request) */
        unsigned int plb_lowfreq_mode;  /* PLB lowfreq support enabled */
        unsigned int cpu_idx[MP_NR_CPUS]; /* cpufreq table index per cpu */
        unsigned int scaling_delay;	/* delay used when between freq steps */
} cpuinfo;

static inline unsigned int cpufreq_calc(unsigned int cpu_div)
{
        return (cpuinfo.pll/cpu_div); /* Mhz */
}

static inline unsigned int plbfreq_calc(unsigned int plb_div)
{
	return (cpuinfo.pll/plb_div); /* Mhz */
}

static inline int cpu_plb_valid(unsigned int cpu_div, unsigned int plb_div)
{
        return !(plb_div%cpu_div);
}

static inline int cpu_plb_idx_valid(unsigned int cpu_idx, unsigned int plb_idx)
{
	return cpu_plb_valid(cdivisors[cpu_idx], pdivisors[plb_idx]);
}

static inline int get_plb2x(unsigned int plb_idx)
{
        return (pdivisors[plb_idx]/2);
}

static inline int plb_fixed(void)
{
        return cpuinfo.plb_fixed;
}

static int apm86xxx_cpufreq_init(struct cpufreq_policy *policy)
{
	unsigned int i;
	unsigned int cur_freq = (ppc_proc_freq / 1000); /* make it kHz */
	unsigned int plb_freq = (ppc_proc_plbfreq / 1000); /* make it kHz */
	(void)plb_freq;

	PM_TRACE("%s\n", __func__);

#if 0
	/* if always want CPU1 to be the same as CPU0, then */
        if (policy->cpu != 0)
                return -ENODEV;
#endif
#ifdef CONFIG_SMP
	PM_DEBUG("CPU%d frequency %d MHz (PLL=%d PLB=%d)\n", 
		 policy->cpu, (cur_freq/1000),
		 cpuinfo.pll, (plb_freq/1000));
#else
	PM_DEBUG("CPU%d frequency %d MHz (PLL=%d PLB=%d)\n", 
		 MP_PROCESSOR_ID(), (cur_freq/1000),
		 cpuinfo.pll, (plb_freq/1000));
#endif

        policy->cpuinfo.transition_latency = CPUFREQ_ETERNAL;
        policy->cur = cpufreq_table[0].frequency;

	for (i = 0; i < CPUFREQ_TABLE_SIZE; i++) {
		if  (cpufreq_table[i].frequency == cur_freq) {
        		policy->cur = cpufreq_table[i].frequency;
        		cpuinfo.cpu_idx[policy->cpu] = i;
			break;
		}
	}

#ifdef CONFIG_SMP
	cpumask_copy(policy->cpus, &per_cpu(cpu_sibling_map, policy->cpu));
#endif

	cpufreq_frequency_table_get_attr(cpufreq_table, policy->cpu);

	/*
	 * This ensures that policy->cpuinfo_min 
	 * and policy->cpuinfo_max are set correctly
	 */
	return cpufreq_frequency_table_cpuinfo(policy, cpufreq_table);
}

static int apm86xxx_cpufreq_exit(struct cpufreq_policy *policy)
{
	PM_TRACE("%s\n", __func__);
	cpufreq_frequency_table_put_attr(policy->cpu);
	return 0;
}

static int apm86xxx_cpufreq_suspend(struct cpufreq_policy *policy, pm_message_t pmsg)
{
	PM_TRACE("%s\n", __func__);
	/* TBD: Do we need to send IPP a message here */
	return 0;
}

static int apm86xxx_cpufreq_resume(struct cpufreq_policy *policy)
{
	PM_TRACE("%s\n", __func__);
	/* TBD: Do we need to send IPP a message here */
	return 0;
}

static int apm86xxx_cpufreq_verify(struct cpufreq_policy *policy)
{
	PM_TRACE("%s\n", __func__);
	return cpufreq_frequency_table_verify(policy, cpufreq_table);
}

static int mp_get_cpuinfo(void)
{
	PM_TRACE("%s\n", __func__);
#if 0
	int err;
	int i;
	for(i = 0; i < MP_NR_CPUS; i++) {
		/* TBD: This should be a get function to get the 
		 *      CPU divisor for each CPU
		 */
		err = ipp_send_user_msg(IPP_PWRMGMT_HDLR,
					IPP_PWRMGMT_CMD_CPU_FREQ_SCALE,
					i, IPP_MSG_CONTROL_URG_BIT,
					cdivisors[cpu_idx]);

		if (err < 0)
			return -EFAULT;
	}
#endif
	return 0;
}

static int apm86xxx_cpufreq_scale(unsigned int cpu, unsigned int new_cpu_idx)
{
	int err = 0;
	unsigned int cpu_mask = (1 << cpu);
 	int cpu_idx = (int)cpuinfo.cpu_idx[cpu];
 	int plb_idx = (int)cpuinfo.plb_idx;
 	int step = (cpu_idx < (int)new_cpu_idx)? 1 : -1;
 	int end = ((int)new_cpu_idx + step);

	PM_TRACE("%s\n", __func__);

#ifdef CONFIG_SMP
	for (cpu = 0; cpu < MP_NR_CPUS; cpu++)
		cpu_mask |= (1 << cpu);
#endif
	/*
 	 * NOTE: We need to do frequency stepping to avoid 
 	 *       potential voltage sag effects.
 	 */
	for (cpu_idx += step; cpu_idx != end; cpu_idx += step) {
		if (!cpu_plb_idx_valid(cpu_idx, plb_idx))
			continue;

		PM_DEBUG("%s: step CPU = %u, PLB = %u\n", __func__,
			cpufreq_table[cpu_idx].frequency,
			plbfreq_table[plb_idx]);

		if (is_apm86xxx_lite()) {
			err = cpm_set_cpu_div(cpu_mask, cdivisors[cpu_idx]);
		} else {
			err = ipp_send_user_msg(IPP_PWRMGMT_HDLR,
					IPP_PWRMGMT_CMD_CPU_FREQ_SCALE,
					cpu_mask, IPP_MSG_CONTROL_URG_BIT,
					cdivisors[cpu_idx]);
		}
		if (err < 0)
			goto cpufreq_scale_err;

		/* keep track of successful current step */
#ifdef CONFIG_SMP
		for (cpu = 0; cpu < MP_NR_CPUS; cpu++)
 			cpuinfo.cpu_idx[cpu] = (unsigned int)cpu_idx;
#else
 		cpuinfo.cpu_idx[0] = (unsigned int)cpu_idx;
#endif
		if (cpuinfo.scaling_delay)
			msleep(cpuinfo.scaling_delay);
	}

	return 0;

cpufreq_scale_err:
	printk(KERN_ERR "CPU%d: CPU_FREQ_SCALE failed\n", cpu);
	return err;
}

static int plbfreq_find_valid(unsigned int cpu, unsigned int new_cpu_idx,
			      unsigned int plb_idx)
{
#ifndef CONFIG_SMP
	unsigned int i;
#endif

	if (!cpu_plb_idx_valid(new_cpu_idx, plb_idx))
		return 0;

#ifndef CONFIG_SMP
	/* found a valid plb frequency, now check other CPU's */
	/* TBD for AMP need an ipp API to get other CPU frequency */

	for(i = 0; i < MP_NR_CPUS; i++) {
		if ( cpu != i) {
			unsigned int cpu_idx = cpuinfo.cpu_idx[i];
			if (!cpu_plb_idx_valid(cpu_idx, plb_idx)) {
				return 0;
			}
		}
	}
#endif

	return 1;
}

static int plbfreq_find_best(unsigned int cpu, unsigned int new_cpu_idx,
			     unsigned int *new_plb_idx)
{
	unsigned int plb_idx;

	PM_TRACE("%s\n", __func__);

	if (cpuinfo.plb_set != cpuinfo.plb_idx) {
		if (plbfreq_find_valid(cpu, new_cpu_idx, cpuinfo.plb_set)) {
			*new_plb_idx = cpuinfo.plb_set;
			return 0;
		} else {
			printk(KERN_ERR "error: PLB frequency not valid\n");
			return -EINVAL;
		}
	}

	for (plb_idx = 0; plb_idx < PLBFREQ_TABLE_SIZE; plb_idx++) {
		if (plb_idx < cpuinfo.plb_max || plb_idx > cpuinfo.plb_min)
			continue;

		if (plbfreq_find_valid(cpu, new_cpu_idx, plb_idx)) {
			*new_plb_idx = plb_idx;
			return 0;
		}
	}

	return -EINVAL;
}

static int apm86xxx_plbfreq_scale(unsigned int cpu, unsigned int new_plb_idx)
{
	int err;
	int cpu_idx = (int)cpuinfo.cpu_idx[cpu];
	int plb_idx = (int)cpuinfo.plb_idx;
 	int step = (plb_idx < (int)new_plb_idx)? 1 : -1;
 	int end = ((int)new_plb_idx + step);

	PM_TRACE("%s\n", __func__);

	/*
 	 * NOTE: We need to do frequency stepping to avoid 
 	 *       potential voltage sag effects.
 	 */

	for (plb_idx += step; plb_idx != end; plb_idx += step) {
		if (!cpu_plb_idx_valid(cpu_idx, plb_idx))
			continue;
			
		PM_DEBUG("%s: step CPU = %u, PLB = %u\n", __func__,
			cpufreq_table[cpu_idx].frequency,
			plbfreq_table[plb_idx]);

		if (plbfreq_table[plb_idx] < cpuinfo.hbf) {
			if (cpuinfo.plb_lowfreq_mode == 0) {
				cpuinfo.plb_lowfreq_mode = 1;
				set_plb_lowfreq_mode(cpuinfo.plb_lowfreq_mode);
			}
		} else {
			if (cpuinfo.plb_lowfreq_mode == 1) {
				cpuinfo.plb_lowfreq_mode = 0;
				set_plb_lowfreq_mode(cpuinfo.plb_lowfreq_mode);
			}
		}

		if (is_apm86xxx_lite()) {
			err = cpm_set_plb2x_div(get_plb2x(plb_idx));
		} else {
			err = ipp_send_user_msg(IPP_PWRMGMT_HDLR,
					IPP_PWRMGMT_CMD_PLB_FREQ_SCALE,
					IPP_MSG_PARAM_UNUSED,
					IPP_MSG_CONTROL_URG_BIT,
					get_plb2x(plb_idx));
		}

		if (err < 0) {
			printk(KERN_ERR "Failed to do PLB_FREQ_SCALE\n");
			return err;
			
		}

		/* keep track of successful current step */
 		cpuinfo.plb_idx = cpuinfo.plb_set = (unsigned int)plb_idx;

		if (cpuinfo.scaling_delay)
			msleep(cpuinfo.scaling_delay);
	}

	return 0;
}

static int apm86xxx_cpufreq_target(struct cpufreq_policy *policy,
			   unsigned int target_freq,
			   unsigned int relation)
{
	int i;
	int err;
	struct cpufreq_freqs freqs;
 	unsigned int cpu_idx = cpuinfo.cpu_idx[policy->cpu];
 	unsigned int plb_idx = cpuinfo.plb_idx;
	unsigned int new_plb_idx = 0;
 	unsigned int new_cpu_idx = 0;
	unsigned int cpu;

	PM_TRACE("%s\n", __func__);

	if (cpufreq_frequency_table_target(policy, cpufreq_table,
		target_freq, relation, &new_cpu_idx)) {
		return -EINVAL;
	}

	if (plb_fixed()) {
		if (cpu_idx == new_cpu_idx)
			return 0;
	} else if (plb_idx == cpuinfo.plb_set) {
		if (plb_idx >= cpuinfo.plb_max && plb_idx <= cpuinfo.plb_min) {
			if (cpu_idx == new_cpu_idx)
				return 0;
		}
	}

       	freqs.old = cpufreq_table[cpu_idx].frequency;
       	freqs.new = cpufreq_table[new_cpu_idx].frequency;
       	freqs.cpu = policy->cpu;

	cpu = MP_PROCESSOR_ID();

	if (plb_fixed()) {
		if (cpu_plb_idx_valid(new_cpu_idx, plb_idx)) {
			cpufreq_notify_transition(&freqs, CPUFREQ_PRECHANGE);
			apm86xxx_cpufreq_scale(cpu, new_cpu_idx);
		} else {
			printk(KERN_ERR "error: PLB fixed, CPU frequency invalid\n");
			return -EINVAL;
		}
	} else {
        	unsigned int tmp_cpu_idx[MP_NR_CPUS];

#if 0
		if (!cpuinfo.plb_lowfreq_mode) {
			set_plb_lowfreq_mode(1);
			cpuinfo.plb_lowfreq_mode = 1;
		}
#endif

		/* If we get here, this  mean we need to change the PLB
		 * frequency. Let's see if we can find one that works for
		 * all CPUs
		 */
		err = mp_get_cpuinfo();
		if (err) {
			printk(KERN_ERR "error: could not get CPU_FREQ info\n");
			return -EFAULT;
		}

		err = plbfreq_find_best(cpu, new_cpu_idx, &new_plb_idx);
		if (err) {
			printk(KERN_ERR "error: CPU frequency not valid\n");
			return -EINVAL;
		}

		/* Let's get ready for the change */

		for(i = 0; i < MP_NR_CPUS; i++) {
			tmp_cpu_idx[i] = cpuinfo.cpu_idx[i];
		}

		tmp_cpu_idx[cpu] = new_cpu_idx;

		cpufreq_notify_transition(&freqs, CPUFREQ_PRECHANGE);

#ifdef CONFIG_SMP
		/* It is always safe to return CPUs to a CPU dividor value
		 * of 2 (cpu_idx = 0) prior to changing the PLB frequency,
		 * as all PLB2X divisors values are allowed with that setting.
 		 */

		err = apm86xxx_cpufreq_scale(cpu, 0);
		if (err) {
			printk(KERN_ERR "error: CPU frequency change failed\n");
			return -EINVAL;
		}
		
		/* Now lets change the PLB frequency */

		err = apm86xxx_plbfreq_scale(cpu, new_plb_idx);
		if (err) {
			printk(KERN_ERR "error: PLB frequency change failed\n");
			return -EINVAL;
		}

		/* Now set new CPU frequency */

		err = apm86xxx_cpufreq_scale(cpu, tmp_cpu_idx[cpu]);
		if (err) {
			printk(KERN_ERR "error: CPU frequency change failed\n");
			return -EINVAL;
		}
#else
		/* It is always safe to return CPUs to a CPU dividor value
		 * of 2 (cpu_idx = 0) prior to changing the PLB frequency,
		 * as all PLB2X divisors values are allowed with that setting.
 		 */

		for(i = 0; i < MP_NR_CPUS; i++) {
			err = apm86xxx_cpufreq_scale(i, 0);
			if (err) {
				printk(KERN_ERR
					"error: CPU frequency change failed\n");
				return -EINVAL;
			}
		}

		/* Now lets change the PLB frequency */

		err = apm86xxx_plbfreq_scale(cpu, new_plb_idx);
		if (err) {
			printk(KERN_ERR "error: PLB frequency change failed\n");
			return -EINVAL;
		}

		/* Now restore all CPU's frequencies */

		for(i = 0; i < MP_NR_CPUS; i++) {
			err = apm86xxx_cpufreq_scale(i, tmp_cpu_idx[i]);
			if (err) {
				printk(KERN_ERR
					"error: CPU frequency change failed\n");
				return -EINVAL;
			}
		}
#endif
	}

	cpufreq_notify_transition(&freqs, CPUFREQ_POSTCHANGE);

        ppc_proc_freq = cpufreq_table[cpuinfo.cpu_idx[policy->cpu]].frequency * 1000;
        ppc_proc_plbfreq = plbfreq_table[cpuinfo.plb_idx] * 1000;

	if (ppc_tb_freq != ppc_proc_freq) {
		set_dec(DECREMENTER_MAX);

                ppc_tb_freq = ppc_proc_freq;
                tb_update_recal();

		set_dec(1);
	}

	return 0;
}

static unsigned int apm86xxx_cpufreq_get(unsigned int cpu)
{
	PM_TRACE("%s\n", __func__);

	/*
	 * TBD do we need to verify here with iPP
	 * if We don't then we don't really need this function
	 * cpufreq has it own one based on currently policy
	 * info. But for now let output something.
	 */

	return cpufreq_table[cpuinfo.cpu_idx[cpu]].frequency;
}

static ssize_t plbfreq_all_show(struct cpufreq_policy *policy, char *buf)
{
	char *s = buf;
	int i;

	for (i = 0; i < PLBFREQ_TABLE_SIZE; i++) {
		s += sprintf(s, "%u\n", plbfreq_table[i]);
	}

	return (s - buf);
}

static ssize_t plbfreq_fixed_show(struct cpufreq_policy *policy, char *buf)
{
	char *s = buf;

	s += sprintf(s, "%d\n", cpuinfo.plb_fixed);

	return (s - buf);
}

static ssize_t plbfreq_fixed_store(struct cpufreq_policy *policy,
				   const char *buf, size_t n)
{
	int ret;
	int data;

	ret = sscanf(buf, "%d", &data);

	if (ret == 1) {
		if ( data == 0 || data == 1) {
			cpuinfo.plb_fixed = data;
			return n;
		}
	}

	return -EINVAL;
}

static ssize_t plbfreq_cur_show(struct cpufreq_policy *policy, char *buf)
{
	char *s = buf;

	s += sprintf(s, "%u\n", plbfreq_table[cpuinfo.plb_idx]);

	return (s - buf);
}

static ssize_t plbfreq_set_show(struct cpufreq_policy *policy, char *buf)
{
	char *s = buf;

	s += sprintf(s, "%u\n", plbfreq_table[cpuinfo.plb_set]);

	return (s - buf);
}

static ssize_t plbfreq_set_store(struct cpufreq_policy *policy,
			     const char *buf, size_t n)
{
	int ret;
	unsigned int freq;
	unsigned int plb_idx;

	if (plb_fixed()) {
		printk(KERN_ERR "error: PLB fixed, cannot change at this time\n");
		return -EINVAL;
	}
		
	ret = sscanf(buf, "%u", &freq);

	if (ret == 1) {
		for (plb_idx = 0; plb_idx < PLBFREQ_TABLE_SIZE; plb_idx++) {
			if (plbfreq_table[plb_idx] == freq) {
				if (plb_idx >= cpuinfo.plb_max && 
				    plb_idx <= cpuinfo.plb_min) {
					cpuinfo.plb_set = plb_idx;
					return n;
				} 
				break;
			}
		}
	}

	return -EINVAL;
}

static ssize_t plbfreq_min_show(struct cpufreq_policy *policy, char *buf)
{
	char *s = buf;

	s += sprintf(s, "%u\n", plbfreq_table[cpuinfo.plb_min]);

	return (s - buf);
}

static ssize_t plbfreq_min_store(struct cpufreq_policy *policy,
			     const char *buf, size_t n)
{
	int ret;
	unsigned int freq;
	unsigned int plb_idx;

	if (plb_fixed()) {
		printk(KERN_ERR "error: PLB fixed, cannot change\n");
		return -EINVAL;
	}
		
	ret = sscanf(buf, "%u", &freq);

	if (ret == 1) {
		for (plb_idx = 0; plb_idx < PLBFREQ_TABLE_SIZE; plb_idx++) {
			if (plbfreq_table[plb_idx] == freq) {
				if (plb_idx >= cpuinfo.plb_max) {
					cpuinfo.plb_min = plb_idx;
					/* remove pending plb_set */
					cpuinfo.plb_set = cpuinfo.plb_idx;
					return n;
				} 
				break;
			}
		}
	}

	return -EINVAL;
}

static ssize_t plbfreq_max_show(struct cpufreq_policy *policy, char *buf)
{
	char *s = buf;

	s += sprintf(s, "%u\n", plbfreq_table[cpuinfo.plb_max]);

	return (s - buf);
}

static ssize_t plbfreq_max_store(struct cpufreq_policy *policy,
			     const char *buf, size_t n)
{
	int ret;
	unsigned int freq;
	unsigned int plb_idx;

	if (plb_fixed()) {
		printk(KERN_ERR "error: PLB fixed, cannot change at this time\n");
		return -EINVAL;
	}
		
	ret = sscanf(buf, "%u", &freq);

	if (ret == 1) {
		for (plb_idx = 0; plb_idx < PLBFREQ_TABLE_SIZE; plb_idx++) {
			if (plbfreq_table[plb_idx] == freq) {
				if (plb_idx <= cpuinfo.plb_min) {
					cpuinfo.plb_max = plb_idx;
					/* remove pending plb_set */
					cpuinfo.plb_set = cpuinfo.plb_idx;
					return n;
				} 
				break;
			}
		}
	}

	return -EINVAL;
}

static ssize_t cpufreq_all_show(struct cpufreq_policy *policy, char *buf)
{
	char *s = buf;
	int i;

	for (i = 0; i < CPUFREQ_TABLE_SIZE; i++) {
		s += sprintf(s, "%u\n", cpufreq_table[i].frequency);
	}

	return (s - buf);
}

static ssize_t cpufreq_cur_show(struct cpufreq_policy *policy, char *buf)
{
	char *s = buf;

	s += sprintf(s, "%u\n",
		     cpufreq_table[cpuinfo.cpu_idx[policy->cpu]].frequency);

	return (s - buf);
}

static ssize_t scaling_delay_show(struct cpufreq_policy *policy, char *buf)
{
	char *s = buf;

	s += sprintf(s, "%u\n", cpuinfo.scaling_delay/1000);

	return (s - buf);
}

static ssize_t scaling_delay_store(struct cpufreq_policy *policy,
				   const char *buf, size_t n)
{
	int ret;
	unsigned int data;

	ret = sscanf(buf, "%u", &data);

	if (ret == 1) {
		cpuinfo.scaling_delay = data * 1000;
		return n;
	}

	return -EINVAL;
}

struct freq_attr plbfreq_all = {
        .attr = { .name = "plbfreq_all", .mode = 0444, },
        .show = plbfreq_all_show,
};

struct freq_attr plbfreq_cur = {
        .attr = { .name = "plbfreq_cur", .mode = 0444, },
        .show = plbfreq_cur_show,
};

struct freq_attr plbfreq_setspeed = {
        .attr = { .name = "plbfreq_set", .mode = 0644, },
        .show = plbfreq_set_show,
        .store = plbfreq_set_store,
};

struct freq_attr plbfreq_fixed = {
        .attr = { .name = "plbfreq_fixed", .mode = 0644, },
        .show = plbfreq_fixed_show,
        .store = plbfreq_fixed_store,
};

struct freq_attr plbfreq_min = {
        .attr = { .name = "plbfreq_min", .mode = 0644, },
        .show = plbfreq_min_show,
        .store = plbfreq_min_store,
};

struct freq_attr plbfreq_max = {
        .attr = { .name = "plbfreq_max", .mode = 0644, },
        .show = plbfreq_max_show,
        .store = plbfreq_max_store,
};

struct freq_attr cpufreq_all = {
        .attr = { .name = "cpufreq_all", .mode = 0444, },
        .show = cpufreq_all_show,
};

struct freq_attr cpufreq_cur = {
        .attr = { .name = "cpufreq_cur", .mode = 0444, },
        .show = cpufreq_cur_show,
};

struct freq_attr scaling_delay= {
        .attr = { .name = "scaling_setdelay", .mode = 0644, },
        .show = scaling_delay_show,
        .store = scaling_delay_store,
};

static struct freq_attr* apm86xxx_cpufreq_attr[] = {
	&cpufreq_freq_attr_scaling_available_freqs,
	&plbfreq_all,
	&plbfreq_cur,
	&plbfreq_setspeed,
	&plbfreq_fixed,
	&plbfreq_min,
	&plbfreq_max,
	&cpufreq_all,
	&cpufreq_cur,
	&scaling_delay,
	NULL,
};

static struct cpufreq_driver apm86xxx_driver = {
	.name		= "apm86xxx-cpufreq",
	.init		= apm86xxx_cpufreq_init,
	.exit		= apm86xxx_cpufreq_exit,
	.suspend	= apm86xxx_cpufreq_suspend,
	.resume		= apm86xxx_cpufreq_resume,
	.verify		= apm86xxx_cpufreq_verify,
	.target		= apm86xxx_cpufreq_target,
	.get		= apm86xxx_cpufreq_get,
	.attr		= apm86xxx_cpufreq_attr,
	.flags		= CPUFREQ_PM_NO_WARN,
	.owner		= THIS_MODULE,
};

static int __init apm86xxx_cpufreq_register(void)
{
	unsigned int i;
	struct clk *clk;

	PM_TRACE("%s\n", __func__);

	cpuinfo.init = 0;

	clk = clk_get(NULL, "cpu_pll");
	if(IS_ERR(clk)) {
		printk(KERN_ERR "cpufreq: Failed to get cpu_pll\n");
		return 0;
	} else {
		cpuinfo.pll = clk_get_rate(clk) / (1000*1000);
	}

	clk = clk_get(NULL, "hbf");
	if(IS_ERR(clk)) {
		printk(KERN_ERR "cpufreq: Failed to get hbf frequency\n");
		return 0;
	} else {
		cpuinfo.hbf = clk_get_rate(clk) / 1000; /* save it in Khz */
	}

	cpuinfo.scaling_delay = 0;
	cpuinfo.plb_idx = cpuinfo.plb_set = 0;
	cpuinfo.plb_max = 0;
	cpuinfo.plb_min = PLBFREQ_TABLE_SIZE-1;
        cpuinfo.plb_lowfreq_mode = 0;
	cpuinfo.plb_fixed = 0;
#ifndef CONFIG_SMP
        if (cpu_enabled(1))
		cpuinfo.plb_fixed = 1;
#endif
	for(i = 0; i < MP_NR_CPUS; i++) {
		cpuinfo.cpu_idx[i] = 0;
	}

	for (i = 0; i < CPUFREQ_TABLE_SIZE; i++) {
		/* calculate frequency and convert to Khz */
		cpufreq_table[i].index = i;
		cpufreq_table[i].frequency = cpufreq_calc(cdivisors[i]) * 1000;
	}

	cpufreq_table[CPUFREQ_TABLE_SIZE].index = 0;
	cpufreq_table[CPUFREQ_TABLE_SIZE].frequency = CPUFREQ_TABLE_END; 

	for (i = 0; i < PLBFREQ_TABLE_SIZE; i++) {
		/* calculate frequency and convert to Khz */
		plbfreq_table[i] = plbfreq_calc(pdivisors[i]) * 1000;
	}

	return cpufreq_register_driver(&apm86xxx_driver);
}

static void __exit apm86xxx_cpufreq_unregister(void)
{
	PM_TRACE("%s\n", __func__);
	cpufreq_unregister_driver(&apm86xxx_driver);
}

MODULE_DESCRIPTION ("APM86xxx Dynamic Frequency Scaling driver.");
MODULE_AUTHOR ("Victor Gallardo<vgallardo@amcc.com>");
MODULE_LICENSE ("GPL");

late_initcall(apm86xxx_cpufreq_register);
module_exit(apm86xxx_cpufreq_unregister);
