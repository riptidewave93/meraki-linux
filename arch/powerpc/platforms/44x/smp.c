/*
 * APM SoC APM86xxx SMP Specific Routine
 *
 * Copyright (c) 2010 Applied Micro Circuits Corporation.
 * All rights reserved. Loc Ho <lho@apm.com>.
 *
 * Based on arch/powerpc/platforms/86xx/mpc86xx.c
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

#include <linux/stddef.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/cpu.h>
#include <linux/interrupt.h>
#include <asm/machdep.h>
#include <asm/mpic.h>

#if defined(CONFIG_APM86xxx)
#include "apm86xxx_suspend.h"
#include <asm/ipp.h>
#endif

extern void __mp_core_start(void);
extern unsigned long __secondary_hold_acknowledge;
extern void mpic_send_trigger(unsigned int msg_idx, unsigned int core_id,
                        unsigned long core_entry);
extern void mpic_resetcore(unsigned int);

static void smp_kick_cpu(int nr)
{
	int n = 0;
	unsigned long secondary_kernel_entry;

	if (nr <= 0 || nr > NR_CPUS) {
		printk(KERN_INFO "%s: CPU #%d not kickable\n", __func__, nr);
		return; 
	}

#ifdef CONFIG_APM86xxx
	if (is_apm86xxx_lite())
		apm86xxx_powerup_core(nr);
	else
		ipp_online_cpu(nr);
#endif
	__secondary_hold_acknowledge = 0;

	mpic_resetcore(nr);
	printk(KERN_INFO "%s: kick CPU #%d\n", __func__, nr);

	secondary_kernel_entry = (unsigned long) 0x4;

	/* Kick that CPU */
	mpic_send_trigger(0, nr, secondary_kernel_entry);

	while ((__secondary_hold_acknowledge != nr) && (n++, n < 10000))
		udelay(100);

	printk(KERN_INFO "%s: waited %d msecs for CPU #%d\n",
		__func__, (n / 1000), nr);
}

static void smp_setup_cpu(int cpu_nr)
{
	if (cpu_nr != 0) {
		/* For secondary CPU, identify the cpu and call 
		   setup_cpu via SMP function pointer. This is required
		   to enable FPU, etc. But don't use the identify_cpu
		   as it will not be available on wakeup from suspend */

		if (cur_cpu_spec->cpu_setup)
			cur_cpu_spec->cpu_setup(0, cur_cpu_spec);
		else
			BUG();
	}
	
	mpic_setup_this_cpu();
}

int __attribute__ ((weak)) __cpu_disable_prepare(void)
{
        return 0;
}

#if defined(CONFIG_HOTPLUG_CPU) && defined(CONFIG_SMP)

int smp_ppc44x_disable(void)
{
	unsigned int cpu = smp_processor_id();
	int err;

	if (cpu == boot_cpuid) {
		/* We cannot disable boot CPU because Linux Power
		 * Management uses it to enter suspend code
		 */
		printk(KERN_ERR "CPU%u: cannot disable boot CPU!\n", cpu);
		return -EBUSY;
	}

	/* 
	 * Check if we can disable this cpu, if not
	 * then do not allow it to be disabled
	 */
	err = __cpu_disable_prepare();
	if (err)
		return err;
		
	/* 
	 * the next line causes the cpu_should_die()
	 * to be true and cause cpu_idle to call
	 * cpu_die instead of entering the idle
	 * wait state
	 */
        set_cpu_online(smp_processor_id(), false);

        /* XXX reset cpu affinity here */
        mpic_cpu_set_priority(0xf);
        asm volatile("mtdec %0" : : "r" (0x7fffffff));
        mb();
        udelay(20);
        asm volatile("mtdec %0" : : "r" (0x7fffffff));
        return 0;
}

static int cpu_dead[NR_CPUS];

void cpu_die(void)
{
        local_irq_disable();
	idle_task_exit();
        cpu_dead[smp_processor_id()] = 1;
        mb();
#if defined(CONFIG_APM86xxx_SUSPEND)
	apm86xxx_standby();
#endif
cpu_hang:
	goto cpu_hang;
}

void smp_ppc44x_die(unsigned int cpu)
{
        int timeout;

        timeout = 1000;
        while (!cpu_dead[cpu]) {
                if (--timeout == 0) {
                        printk("CPU %u refused to die!\n", cpu);
                        break;
                }
                msleep(1);
        }

#if defined(CONFIG_APM86xxx)
	if (is_apm86xxx_lite()) {
		switch(system_state) {
		case SYSTEM_HALT:
		case SYSTEM_POWER_OFF:
		case SYSTEM_RESTART:
			break;
		default:
			apm86xxx_powerdown_core(cpu);
			break;
		}
	}
#endif
        cpu_dead[cpu] = 0;
}
#endif

struct smp_ops_t smp_ppc_ops = {
	.message_pass	= smp_mpic_message_pass,
	.probe		= smp_mpic_probe,
	.kick_cpu	= smp_kick_cpu,
	.setup_cpu	= smp_setup_cpu,
	.take_timebase	= smp_generic_take_timebase,
	.give_timebase	= smp_generic_give_timebase,
#if defined(CONFIG_HOTPLUG_CPU) && defined(CONFIG_SMP)
        .cpu_disable    = smp_ppc44x_disable,
        .cpu_die        = smp_ppc44x_die,
#endif
};

void smp_ppc44x_init(void)
{
	smp_ops = &smp_ppc_ops;
#if defined(CONFIG_HOTPLUG_CPU) && defined(CONFIG_SMP)
	ppc_md.cpu_die = cpu_die;
#endif
}

int __init arch_early_irq_init(void)
{
	int i;
	/* For SMP, map all interrupts to first core by default */
	
	irq_default_affinity[0] = CPU_MASK_CPU0;
	
	for (i = 0; i < ARRAY_SIZE(irq_desc); i++)
		irq_desc[i].affinity[0] = CPU_MASK_CPU0;
		
        return 0;
}
