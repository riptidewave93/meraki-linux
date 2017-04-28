/*
 * Copyright 2008 IBM Corp. 
 *
 * Based on arch/powerpc/platforms/pasemi/idle.c: 
 * Copyright (C) 2006-2007 PA Semi, Inc
 *
 * Added by: Jerone Young <jyoung5@us.ibm.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 */

#include <linux/of.h>
#include <linux/kernel.h>
#include <asm/machdep.h>
#if defined(CONFIG_APM86xxx)
#include <linux/sysfs.h>
#include <linux/cpu.h>
#include <asm/dcr-native.h>
#include <asm/apm86xxx_soc.h>
#endif

static int mode_spin = 0;

static int __init idle_param(char *p)
{ 

	if (!strcmp("spin", p)) {
		mode_spin = 1;
		ppc_md.power_save = NULL;
	}

	return 0;
}

early_param("idle", idle_param);

#if defined(CONFIG_APM86xxx)
#ifdef CONFIG_SMP
static unsigned long idle_count[NR_CPUS];
#else
static unsigned long idle_count;
#endif
static int idle_init = 0;

static void ppc44x_idle(void)
{
	unsigned long msr_save;

#ifdef CONFIG_SMP
	idle_count[mfspr(SPRN_PIR)]++;
#else
	idle_count++;
#endif

	msr_save = mfmsr();
	/* set wait state MSR */
	mtmsr(msr_save|MSR_WE|MSR_EE|MSR_CE|MSR_DE);
	isync();
	/* return to initial state */
	mtmsr(msr_save);
	isync();
}

static ssize_t ppc44x_idle_show(struct kobject *kobj,
				struct kobj_attribute *attr, char *buf)
{
	char *s = buf;
#ifdef CONFIG_SMP
	int i;
#endif

	s += sprintf(s, "status: %s\n", ppc_md.power_save ? "enabled" : "disabled");

	s += sprintf(s, "mode_spin = %d\n", mode_spin);
	s += sprintf(s, "STRK_CR4 = 0x%08x\n", mfdcr(DCRN_STRK_CR4));

#ifdef CONFIG_SMP
	for (i = 0; i < NR_CPUS; i++) {
		s += sprintf(s, "CPU%d:  %lu\n", i, idle_count[i]);
	}
#else
	s += sprintf(s, "CPU%lu:  %lu\n", mfspr(SPRN_PIR), idle_count);
#endif

	return (s - buf);
}

static struct kobj_attribute ppc44x_idle_attr =
		__ATTR(idle, 0444, ppc44x_idle_show, NULL);

void ppc44x_idle_enable(void)
{
	struct sys_device *sys_dev;
	unsigned long ret;

	if (mode_spin)
		return;

	/* Disable Snoop Inihibit */
	__asm__ __volatile__ ("sync" : : : "memory");
	isync();
	mtdcr(DCRN_STRK_CR4, mfdcr(DCRN_STRK_CR4) & ~STRK_CR4_SNPI);
	__asm__ __volatile__ ("sync" : : : "memory");
	isync();

        ppc_md.power_save = &ppc44x_idle;

	if (idle_init) return; 
		
        sys_dev = get_cpu_sysdev(0);
	if (!sys_dev) {
		printk(KERN_WARNING "failed to get cpu sysdev object\n");
		return;
	}
		
        ret = sysfs_create_file(&sys_dev->kobj, &ppc44x_idle_attr.attr);

	idle_init = 1;
}

void ppc44x_idle_disable(void)
{
	if (mode_spin)
		return;

        ppc_md.power_save = NULL;

        /* Re-Enable Snoop Inihibit */
	__asm__ __volatile__ ("sync" : : : "memory");
	isync();
        mtdcr(DCRN_STRK_CR4, mfdcr(DCRN_STRK_CR4) | STRK_CR4_SNPI);
	__asm__ __volatile__ ("sync" : : : "memory");
	isync();
}

EXPORT_SYMBOL(ppc44x_idle_enable);
EXPORT_SYMBOL(ppc44x_idle_disable);

int __init ppc44x_idle_init(void)
{
	ppc44x_idle_enable();
	return 0;
}
late_initcall(ppc44x_idle_init);

#else

static void ppc44x_idle(void)
{
	unsigned long msr_save;

	msr_save = mfmsr();
	/* set wait state MSR */
	mtmsr(msr_save|MSR_WE|MSR_EE|MSR_CE|MSR_DE);
	isync();
	/* return to initial state */
	mtmsr(msr_save);
	isync();
}

int __init ppc44x_idle_init(void)
{
	if (!mode_spin) {
		/* If we are not setting spin mode 
                   then we set to wait mode */
		ppc_md.power_save = &ppc44x_idle;
	}

	return 0;
}
arch_initcall(ppc44x_idle_init);
#endif

