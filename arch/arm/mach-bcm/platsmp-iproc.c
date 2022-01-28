/*
 * Copyright (C) 2013, Broadcom Corporation. All Rights Reserved.
 * 
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <linux/init.h>
#include <linux/cpumask.h>
#include <linux/smp.h>
#include <linux/spinlock.h>
#include <linux/jiffies.h>
#include <linux/delay.h>
#include <linux/of_platform.h>

#include <asm/cacheflush.h>
#include <asm/smp_scu.h>
#include <asm/system_misc.h>  /* soft_restart() */
#include <asm/io.h>

extern void iproc_secondary_startup(void);
extern int __init scu_a9_enable(void);


/* Lookup table phys addr and offset */
#define SOC_ROM_BASE_PA    0xFFFF0000
#ifdef CONFIG_ARCH_BCM_5301X
#define SOC_ROM_LUT_OFF    0x400
#elif defined(CONFIG_MACH_HX4)
#define SOC_ROM_LUT_OFF    0x42c
#elif defined(CONFIG_MACH_HR2)
#define SOC_ROM_LUT_OFF    0x400
#elif defined(CONFIG_MACH_NSP)
#define SOC_ROM_LUT_OFF    0x42c
#define SOC_ROM_LUT_OFF_B0 0xfec
#elif defined(CONFIG_MACH_KT2)
#define SOC_ROM_LUT_OFF    0x42c
#else
#error "SOC_ROM_LUT_OFF is not defined"
#endif

static DEFINE_SPINLOCK(boot_lock);


#ifdef CONFIG_BCM_IPROC_CA9_PREFETCH

static void enable_a9_features(void) {
    unsigned int a9_aux=0, a9_aux_m=0, a9_sys=0;

    // Read a9_aux Register
    asm volatile ("MRC p15, 0, %0, c1, c0, 1\t\n": "=r"(a9_aux));

    // Write a9_aux Register; L1/L2 prefetch enable & Fill line zero
    asm volatile ("mcr p15, 0, %0, c1, c0, 1\t\n" : : "r"(a9_aux | (0x7 << 1)));

    // Read a9_aux Register again
    asm volatile ("MRC p15, 0, %0, c1, c0, 1\t\n": "=r"(a9_aux_m));

    //System Control Register
    asm volatile ("MRC p15, 0, %0, c1, c0, 0\t\n": "=r"(a9_sys));

    printk(KERN_INFO "\n<%s> CUP_ID %d a9_aux = 0x%x; a9_aux_m = 0x%x;"
	   "a9_sys = 0x%x\n",
	   __FUNCTION__,  smp_processor_id(), a9_aux, a9_aux_m, a9_sys);
}

#endif


static void iproc_secondary_init(unsigned int cpu)
{
	printk(KERN_DEBUG "platform_secondary_init: Enter cpu %d\n", cpu);

	/*
	 * let the primary processor know we're out of the
	 * pen, then head off into the C entry point
	 */
	pen_release = -1;


#ifdef CONFIG_BCM_IPROC_CA9_PREFETCH
    enable_a9_features();
    smp_call_function((void (*)(void))enable_a9_features, NULL, 0);
#endif


	smp_wmb();

	/*
	 * Synchronise with the boot thread.
	 */
	spin_lock(&boot_lock);
	spin_unlock(&boot_lock);

	printk(KERN_DEBUG "platform_secondary_init: Leave pen_release %d\n", pen_release);
}

static int iproc_boot_secondary(unsigned int cpu, struct task_struct *idle)
{
	unsigned long timeout;
	void __iomem * rombase = NULL;
	u32 offset = SOC_ROM_LUT_OFF;
	u32 saved_pa;

	printk(KERN_DEBUG "boot_secondary: Enter CPU%d\n", cpu);

	rombase = ioremap(SOC_ROM_BASE_PA, PAGE_SIZE);
	if (rombase == NULL) {
		return -1;
	}

	saved_pa = readl(rombase + offset);
	writel(virt_to_phys(iproc_secondary_startup),
	       rombase + offset);

	dsb_sev();		/* Wake the secondary core */

	/*
	 * Set synchronisation state between this boot processor
	 * and the secondary one
	 */
	spin_lock(&boot_lock);

	/*
	 * The secondary processor is waiting to be released from
	 * the holding pen - release it, then wait for it to flag
	 * that it has been released by resetting pen_release.
	 *
	 * Note that "pen_release" is the hardware CPU ID, whereas
	 * "cpu" is Linux's internal ID.
	 */
	pen_release = cpu;
	clean_dcache_area( (void *) &pen_release, sizeof(pen_release));
	outer_clean_range(__pa(&pen_release),
			  __pa(&pen_release + sizeof(pen_release)));

	smp_wmb();

	/* wait at most 1 second for the secondary to wake up */

	timeout = jiffies + (1 * HZ);
	while (time_before(jiffies, timeout)) {
		smp_rmb();
		if (pen_release == -1)
			break;

		udelay(10);
		clean_dcache_area( (void *) &pen_release, sizeof(pen_release));
	}

	writel(saved_pa, rombase + offset); /* Restore SoC vector */

	wmb();			/* Just to be safe */

	iounmap(rombase);

	/*
	 * Now the secondary core is starting up let it run its
	 * calibrations, then wait for it to finish
	 */
	spin_unlock(&boot_lock);

	printk(KERN_DEBUG "boot_secondary: Leave pen-release %d\n", pen_release);

	return pen_release != -1 ? -ENOSYS : 0;
}



static void __init iproc_smp_prepare_cpus(unsigned int max_cpus)
{
	int i;

	/*
	 * Initialise the present map, which describes the set of CPUs
	 * actually populated at the present time.
	 */
	for (i = 0; i < max_cpus; i++)
		set_cpu_present(i, true);

	/*
	 * Initialise the SCU.
	 */
	scu_a9_enable();
}

#ifdef CONFIG_HOTPLUG_CPU

#define hard_smp_processor_id()				\
	({						\
		unsigned int cpunum;			\
		__asm__("mrc p15, 0, %0, c0, c0, 5"	\
			: "=r" (cpunum));		\
		cpunum &= 0x0F;				\
	})

/*
 * platform-specific code to shutdown a CPU
 *
 * Called with IRQs disabled
 */
static void iproc_cpu_die(unsigned int cpu)
{
    unsigned int this_cpu = hard_smp_processor_id();
    if (cpu != this_cpu) {
	    printk(KERN_CRIT "Eek! platform_cpu_die running on %u, should be %u\n",
		   this_cpu, cpu);
	    BUG();
    }

    printk(KERN_NOTICE "CPU%u: %s shutdown\n", cpu, __func__);

    /* Transfer control to the on-SoC holding pen */

    soft_restart(0xffff0000);
}
#endif

static const struct smp_operations iproc_smp_ops __initdata = {
	.smp_prepare_cpus	= iproc_smp_prepare_cpus,
	.smp_secondary_init	= iproc_secondary_init,
	.smp_boot_secondary	= iproc_boot_secondary,
#ifdef CONFIG_HOTPLUG_CPU
	.cpu_die		= iproc_cpu_die,
#endif
};
CPU_METHOD_OF_DECLARE(bcm_smp_iproc, "brcm,bcm-iproc-smp",  &iproc_smp_ops);
