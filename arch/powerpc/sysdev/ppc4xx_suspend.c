/*
 * PowerPC 4xx suspend and standby support
 *
 * (C) Copyright 2009, Applied Micro Circuits Corporation
 * Victor Gallardo (vgallardo@amcc.com)
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
#include <linux/kobject.h>
#include <linux/string.h>
#include <asm/ppc4xx_cpm.h>

#if defined(CONFIG_PM_DEBUG)
#	define PM_DEBUG(fmt, arg...) printk(KERN_DEBUG fmt, ## arg)
#else
#	define PM_DEBUG(fmt, arg...) ((void)0)
#endif


static int suspend_mode = CPM_PM_DOZE;

static ssize_t ppc_4xx_suspend_available_modes_show(struct kobject *kobj,
			struct kobj_attribute *attr, char *buf)
{
	char *s = buf;
	int i;

	for (i = 0; i < CPM_PM_MODES_MAX; i++) {
		s += sprintf(s, "%s ", cpm_mode_name(i));
	}

	*(s-1) = '\n'; /* convert the last space to a newline */

	return (s - buf);
}

static ssize_t ppc_4xx_suspend_mode_show(struct kobject *kobj,
			struct kobj_attribute *attr, char *buf)
{
	char *s = buf;
	s += sprintf(s, "%s\n", cpm_mode_name(suspend_mode));
	return (s - buf);
}

static ssize_t ppc_4xx_suspend_mode_store(struct kobject *kobj,
			struct kobj_attribute *attr, const char *buf, size_t n)
{
	int i;
	char *p;
	int len;

	p = memchr(buf, '\n', n);
	len = p ? p - buf : n;

	for (i = 0; i < CPM_PM_MODES_MAX; i++) {
		if (strncmp(buf, cpm_mode_name(i), len) == 0) {
			suspend_mode = i;
			return n;
		}
	}

	return -EINVAL;
}

static struct kobj_attribute ppc_4xx_suspend_available_modes_attr =
	__ATTR(suspend_available_modes, 0444,
	       ppc_4xx_suspend_available_modes_show, NULL);

static struct kobj_attribute ppc_4xx_suspend_mode_attr =
	__ATTR(suspend_mode, 0644,
	       ppc_4xx_suspend_mode_show, ppc_4xx_suspend_mode_store);


static int ppc4xx_suspend_valid(suspend_state_t state)
{
	return ((state == PM_SUSPEND_STANDBY) || (state == PM_SUSPEND_MEM));
}

static int ppc4xx_suspend_enter(suspend_state_t state)
{
	PM_DEBUG("%s\n", __func__);
	switch (state) {
	case PM_SUSPEND_STANDBY:
	case PM_SUSPEND_MEM:
		cpm_pm_suspend(state, suspend_mode);
		return 0;
	default:
		return -EINVAL;
	}
}

static struct platform_suspend_ops ppc4xx_suspend_ops = {
	.valid = ppc4xx_suspend_valid,
	.enter = ppc4xx_suspend_enter,
};

int __init ppc4xx_suspend_init(void)
{
	int err;
	suspend_set_ops(&ppc4xx_suspend_ops);
	PM_DEBUG("Create sysfs power");
	err = sysfs_create_file(power_kobj, 
		&ppc_4xx_suspend_mode_attr.attr);
	
	err = sysfs_create_file(power_kobj, 
		&ppc_4xx_suspend_available_modes_attr.attr);
	return 0;
}

arch_initcall(ppc4xx_suspend_init);

