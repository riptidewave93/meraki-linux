/*
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


#include <linux/module.h>
#include <linux/kmod.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <asm/pmu.h>

/* Add for performance counter usage */
static void pmu_set_userspace_access(void *en)
{
	int val = 0;
	/* Read PMUSERENR Register */
	asm volatile("mrc p15, 0, %0, c9, c14, 0" : : "r" (val));
	val &= ~0x1;
	val |= (int)en;
	/* Write PMUSERENR Register */
	asm volatile("mcr p15, 0, %0, c9, c14, 0" : : "r" (val));
}

static int __init pmu_init(void)
{
	printk("Enable PMUSERENR on all cores ... ");

	on_each_cpu(pmu_set_userspace_access, (void*)1, 1);

	printk("done\n");
	return 0;
}

static void __exit pmu_exit(void)
{
	printk("Disable PMUSERENR on all cores ... ");

	on_each_cpu(pmu_set_userspace_access, (void*)0, 1);

	printk("done\n");
}

module_init(pmu_init);
module_exit(pmu_exit);

MODULE_AUTHOR("Tzu-Jung Lee <tjlee@ambarella.com>");
MODULE_LICENSE("GPL");
