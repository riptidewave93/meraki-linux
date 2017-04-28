/*
 * APM86xxx suspend sysfs support
 *
 * Copyright (c) 2010 Applied Micro Circuits Corporation.
 * Victor Gallardo (vgallardo@apm.com)
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
#include <asm/dcr.h>
#include <asm/dcr-regs.h>
#include <asm/ipp.h>

#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/reboot.h>

#include <linux/interrupt.h>
#include <linux/irq.h>
#include <asm/irq.h>
#include <asm/machdep.h>
#include <asm/mpic.h>

#if defined(CONFIG_SMP)
#include <asm/smp.h>
#endif

#include <asm/apm86xxx_soc.h>
#include "apm86xxx_suspend.h"

static struct apm86xxx_pm_modes apm86xxx_pm_modes[PWRMODE_MAX] = {
#if defined(CONFIG_SMP)
        [PWRMODE_DOZE]  = {{PWRMODE_CLK_OFF, PWRMODE_CLK_OFF}, "doze" },
        [PWRMODE_NAP]   = {{PWRMODE_CLK_OFF, PWRMODE_PWR_OFF}, "nap" },
        [PWRMODE_SLEEP] = {{PWRMODE_CLK_OFF, PWRMODE_CLK_OFF}, "sleep" },
        [PWRMODE_DSLEEP]= {{PWRMODE_PWR_OFF, PWRMODE_PWR_OFF}, "deepsleep" },
#else
        [PWRMODE_DOZE]  = {{PWRMODE_CLK_OFF, PWRMODE_CLK_OFF}, "doze" },
        [PWRMODE_NAP]   = {{PWRMODE_CLK_OFF, PWRMODE_CLK_OFF}, "nap" },
        [PWRMODE_SLEEP] = {{PWRMODE_CLK_OFF, PWRMODE_CLK_OFF}, "sleep" },
        [PWRMODE_DSLEEP]= {{PWRMODE_PWR_OFF, PWRMODE_PWR_OFF}, "deepsleep" },
#endif
};

static struct apm86xxx_pm_wakeup apm86xxx_pm_wakeup[WAKEUP_MAX] = {
        [WAKEUP_LAN]	= { 1, IPP_WAKEUP_ETHERNET_EVENT, "lan" },
        [WAKEUP_GPIO]	= { 1, IPP_WAKEUP_GPIO_EVENT,     "gpio" },
        [WAKEUP_USB]	= { 0, IPP_WAKEUP_USB_EVENT,      "usb" },
        [WAKEUP_RTC]	= { 0, IPP_WAKEUP_RTC_EVENT,      "rtc" },
};

static struct apm86xxx_pm apm86xxx_pm = { 
        .mode            = PWRMODE_DSLEEP,
        .sleep_timer     = 0,
        .deepsleep_timer = 0,
        .modes           = apm86xxx_pm_modes,
        .wakeup          = apm86xxx_pm_wakeup,
};

static ssize_t apm86xxx_suspend_show_mode_options(struct kobject *kobj,
			struct kobj_attribute *attr, char *buf)
{
	char *s = buf;
	int i;
	int max = PWRMODE_MAX;

	if (is_apm86xxx_lite())
		max--;

	for (i = 0; i < max; i++) {
		s += sprintf(s, "%s ", apm86xxx_pm.modes[i].name);
	}

	*(s-1) = '\n'; /* convert the last space to a newline */

	return (s - buf);
}

static ssize_t apm86xxx_suspend_show_mode(struct kobject *kobj,
			struct kobj_attribute *attr, char *buf)
{
	char *s = buf;
	s += sprintf(s, "%s\n", apm86xxx_pm.modes[apm86xxx_pm.mode].name);
	return (s - buf);
}

static ssize_t apm86xxx_suspend_store_mode(struct kobject *kobj,
			struct kobj_attribute *attr, const char *buf, size_t n)
{
	int i;
	char *p;
	int len;
	int max = PWRMODE_MAX;

	if (is_apm86xxx_lite())
		max--;

	p = memchr(buf, '\n', n);
	len = p ? p - buf : n;

	for (i = 0; i < max; i++) {
		if (strncmp(buf, apm86xxx_pm.modes[i].name, len) == 0) {
			apm86xxx_pm.mode = i;
			return n;
		}
	}

	return -EINVAL;
}

static ssize_t apm86xxx_suspend_show_wakeup_options(struct kobject *kobj,
			struct kobj_attribute *attr, char *buf)
{
	char *s = buf;
	int i;
	int max = WAKEUP_MAX;

	if (is_apm86xxx_lite())
		max -= 2;

	for (i = 0; i < max; i++) {
		s += sprintf(s, "%s ", apm86xxx_pm.wakeup[i].name);
	}

	*(s-1) = '\n'; /* convert the last space to a newline */

	return (s - buf);
}

static ssize_t apm86xxx_suspend_show_wakeup(struct kobject *kobj,
			struct kobj_attribute *attr, char *buf)
{
	char *s = buf;
	int i;
	int max = WAKEUP_MAX;

	if (is_apm86xxx_lite())
		max -= 2;

	for (i = 0; i < max; i++) {
		if (apm86xxx_pm.wakeup[i].enabled)
			s += sprintf(s, "%s ", apm86xxx_pm.wakeup[i].name);
	}

	*(s-1) = '\n'; /* convert the last space to a newline */

	return (s - buf);
}

static ssize_t apm86xxx_suspend_store_wakeup(struct kobject *kobj,
			struct kobj_attribute *attr, const char *buf, size_t n)
{
	int wakeup_on[WAKEUP_MAX] = { 0, 0, 0, 0 };
	int found = 0;
	int i;

	size_t nn = 32;
	char strbuf[nn];
	char *strptr, *token;
	int max = WAKEUP_MAX;

	if (is_apm86xxx_lite())
		max -= 2;

	if ( n > nn)
		n = nn;

	memcpy(strbuf, buf, n);
	strbuf[n-1] = '\0';

	strptr = memchr(strbuf, '\n', n);
	if (strptr)
		*strptr = '\0';

	strptr = &strbuf[0];

	while ((token = strsep(&strptr, "\t ")) != NULL) {
		for (i = 0; i < max; i++) {
			if (strcmp(token, apm86xxx_pm.wakeup[i].name) == 0) {
				wakeup_on[i] = 1;
				found = 1;
				break;
			}
		}
	}

	if (found) {
		for (i = 0; i < max; i++)
			apm86xxx_pm.wakeup[i].enabled = wakeup_on[i];
	}

	return found ? n : -EINVAL;
}

static ssize_t apm86xxx_suspend_show_sleep_timer(struct kobject *kobj,
			struct kobj_attribute *attr, char *buf)
{
        char *s = buf;
        s += sprintf(s, "%u\n", apm86xxx_pm.sleep_timer);
        return (s - buf);
}

static ssize_t apm86xxx_suspend_store_sleep_timer(struct kobject *kobj,
			struct kobj_attribute *attr, const char *buf, size_t n)
{
        unsigned int data;
        int ret;

        ret = sscanf(buf, "%u", &data);

        if (ret == 1) {
		apm86xxx_pm.sleep_timer = data;
		apm86xxx_pm.sleep_timer_changed = 1;
		return n;
        }

	return -EINVAL;
}

static ssize_t apm86xxx_suspend_show_deepsleep_timer(struct kobject *kobj,
			struct kobj_attribute *attr, char *buf)
{
        char *s = buf;
        s += sprintf(s, "%u\n", apm86xxx_pm.deepsleep_timer);
        return (s - buf);
}

static ssize_t apm86xxx_suspend_store_deepsleep_timer(struct kobject *kobj,
			struct kobj_attribute *attr, const char *buf, size_t n)
{
        unsigned int data;
        int ret;

        ret = sscanf(buf, "%u", &data);

        if (ret == 1) {
		apm86xxx_pm.deepsleep_timer = data;
		apm86xxx_pm.ds_timer_changed = 1;
		return n;
        }

	return -EINVAL;
}

static ssize_t apm86xxx_suspend_show_gpio_wakeup(struct kobject *kobj,
			struct kobj_attribute *attr, char *buf)
{
        char *s = buf;
        s += sprintf(s, "%u\n", apm86xxx_pm.gpio_wakeup);
        return (s - buf);
}

static ssize_t apm86xxx_suspend_store_gpio_wakeup(struct kobject *kobj,
			struct kobj_attribute *attr, const char *buf, size_t n)
{
        unsigned int data;
        int ret;

        ret = sscanf(buf, "%u", &data);

        if (ret == 1 && data < IPP_MAX_GPI0) {
		apm86xxx_pm.gpio_wakeup = data;
		return n;
        }

	return -EINVAL;
}

static ssize_t apm86xxx_suspend_show_vscale(struct kobject *kobj,
			struct kobj_attribute *attr, char *buf)
{
        char *s = buf;
        s += sprintf(s, "%u\n", apm86xxx_pm.vscale);
        return (s - buf);
}

static ssize_t apm86xxx_suspend_store_vscale(struct kobject *kobj,
			struct kobj_attribute *attr, const char *buf, size_t n)
{
        unsigned int data;
        int ret;

        ret = sscanf(buf, "%u", &data);

        if (ret == 1) {
		apm86xxx_pm.vscale = !!data;
		return n;
        }

	return -EINVAL;
}

static ssize_t apm86xxx_suspend_show_iscale(struct kobject *kobj,
			struct kobj_attribute *attr, char *buf)
{
        char *s = buf;
        s += sprintf(s, "%u\n", apm86xxx_pm.iscale);
        return (s - buf);
}

static ssize_t apm86xxx_suspend_store_iscale(struct kobject *kobj,
			struct kobj_attribute *attr, const char *buf, size_t n)
{
        unsigned int data;
        int ret;

        ret = sscanf(buf, "%u", &data);

        if (ret == 1) {
		apm86xxx_pm.iscale = data;
		return n;
        }

	return -EINVAL;
}

static ssize_t apm86xxx_suspend_show_wakeup_ledmsk(struct kobject *kobj,
			struct kobj_attribute *attr, char *buf)
{
        char *s = buf;
        s += sprintf(s, "0x%x\n", apm86xxx_pm.wakeup_ledmsk);
        return (s - buf);
}

static ssize_t apm86xxx_suspend_store_wakeup_ledmsk(struct kobject *kobj,
			struct kobj_attribute *attr, const char *buf, size_t n)
{
        unsigned int data;
        int ret;

        ret = sscanf(buf, "0x%x", &data);

        if (ret == 1) {
		apm86xxx_pm.wakeup_ledmsk = data;
		apm86xxx_pm.wakeup_led_changed = 1;
		return n;
        }

	return -EINVAL;
}

static ssize_t apm86xxx_suspend_show_disable_gpio_sd(struct kobject *kobj,
			struct kobj_attribute *attr, char *buf)
{
        char *s = buf;
        s += sprintf(s, "%u\n", apm86xxx_pm.disable_gpio_sd);
        return (s - buf);
}

static ssize_t apm86xxx_suspend_store_disable_gpio_sd(struct kobject *kobj,
			struct kobj_attribute *attr, const char *buf, size_t n)
{
        unsigned int data;
        int ret;

        ret = sscanf(buf, "%u", &data);

        if (ret == 1) {
		apm86xxx_pm.disable_gpio_sd = !!data;
		apm86xxx_pm.disable_gpio_sd_changed = 1;
		return n;
        }

	return -EINVAL;
}

#ifndef CONFIG_SMP
static ssize_t apm86xxx_suspend_show_online(struct kobject *kobj,
			struct kobj_attribute *attr, char *buf)
{
        char *s = buf;
        s += sprintf(s, "unknown\n");
        return (s - buf);
}

static ssize_t apm86xxx_suspend_store_online(struct kobject *kobj,
			struct kobj_attribute *attr, const char *buf, size_t n)
{
        unsigned int online;
        unsigned int cpu;
        int ret;

        ret = sscanf(buf, "%u %u", &cpu, &online);

        if (ret == 1) {
		/*
		 * if only 1 arg passed in then it must be the online value,
		 * fix the variables to handle it corretly below.
		 */
		online = !!cpu;
		cpu = !mfspr(SPRN_PIR);
		ret = 2;
	}

        if (ret == 2) {
		unsigned int cur_cpu = mfspr(SPRN_PIR);
		unsigned int num_cpus = mpic_num_cpus();

		if (cpu == cur_cpu || cpu >= num_cpus)
			return -EINVAL;

		online = !!online;

		switch(online) {
		case 0:
			return -EINVAL;
		case 1:
			if (is_apm86xxx_lite()) {
				return -EINVAL;
			} else {
				ipp_online_cpu(cpu);
				return n;
			}
		}
        }

	return -EINVAL;
}
#endif

static ssize_t apm86xxx_suspend_show_reset(struct kobject *kobj,
			struct kobj_attribute *attr, char *buf)
{
        char *s = buf;
        s += sprintf(s, "core chip system\n");
        return (s - buf);
}

static ssize_t apm86xxx_suspend_store_reset(struct kobject *kobj,
			struct kobj_attribute *attr, const char *buf, size_t n)
{
	char *p;
	int len;

	p = memchr(buf, '\n', n);
	len = p ? p - buf : n;

	if (strncmp(buf, "core", len) == 0) {
		kernel_restart("core");
		return n;
	} else if (strncmp(buf, "chip", len) == 0) {
		kernel_restart("chip");
		return n;
	} else if (strncmp(buf, "system", len) == 0) {
		kernel_restart("system");
		return n;
	}

	return -EINVAL;
}

static struct kobj_attribute apm86xxx_suspend_mode_options =
	__ATTR(mode_options, 0444,
	       apm86xxx_suspend_show_mode_options, NULL);

static struct kobj_attribute apm86xxx_suspend_mode =
	__ATTR(mode, 0644,
	       apm86xxx_suspend_show_mode, apm86xxx_suspend_store_mode);

static struct kobj_attribute apm86xxx_suspend_wakeup_options =
	__ATTR(wakeup_options, 0444,
	       apm86xxx_suspend_show_wakeup_options, NULL);

static struct kobj_attribute apm86xxx_suspend_wakeup =
	__ATTR(wakeup, 0644,
	       apm86xxx_suspend_show_wakeup, apm86xxx_suspend_store_wakeup);

static struct kobj_attribute apm86xxx_suspend_sleep_timer =
	__ATTR(sleep_timer, 0644,
		apm86xxx_suspend_show_sleep_timer,
		apm86xxx_suspend_store_sleep_timer);

static struct kobj_attribute apm86xxx_suspend_deepsleep_timer =
	__ATTR(deepsleep_timer, 0644,
		apm86xxx_suspend_show_deepsleep_timer,
		apm86xxx_suspend_store_deepsleep_timer);

static struct kobj_attribute apm86xxx_suspend_gpio_wakeup =
	__ATTR(gpio_wakeup, 0644,
		apm86xxx_suspend_show_gpio_wakeup,
		apm86xxx_suspend_store_gpio_wakeup);

static struct kobj_attribute apm86xxx_suspend_vscale =
	__ATTR(vscale, 0644,
		apm86xxx_suspend_show_vscale,
		apm86xxx_suspend_store_vscale);

static struct kobj_attribute apm86xxx_suspend_iscale =
	__ATTR(iscale, 0644,
		apm86xxx_suspend_show_iscale,
		apm86xxx_suspend_store_iscale);

static struct kobj_attribute apm86xxx_wakeup_ledmsk =
	__ATTR(wakeup_ledmsk, 0644,
		apm86xxx_suspend_show_wakeup_ledmsk,
		apm86xxx_suspend_store_wakeup_ledmsk);

static struct kobj_attribute apm86xxx_disable_gpio_sd =
	__ATTR(disable_gpio_sd, 0644,
		apm86xxx_suspend_show_disable_gpio_sd,
		apm86xxx_suspend_store_disable_gpio_sd);

#ifndef CONFIG_SMP
static struct kobj_attribute apm86xxx_suspend_online =
	__ATTR(online, 0644,
		apm86xxx_suspend_show_online,
		apm86xxx_suspend_store_online);
#endif

static struct kobj_attribute apm86xxx_suspend_reset =
	__ATTR(reset, 0644,
		apm86xxx_suspend_show_reset,
		apm86xxx_suspend_store_reset);

struct apm86xxx_pm * apm86xxx_suspend_sysfs_init(void)
{
	int err;

	err = sysfs_create_file(power_kobj,
				&apm86xxx_suspend_mode.attr);
	err = sysfs_create_file(power_kobj,
				&apm86xxx_suspend_mode_options.attr);
	err = sysfs_create_file(power_kobj,
				&apm86xxx_suspend_wakeup.attr);
	err = sysfs_create_file(power_kobj,
				&apm86xxx_suspend_wakeup_options.attr);
	if (!is_apm86xxx_lite()) {
		err = sysfs_create_file(power_kobj,
				&apm86xxx_suspend_sleep_timer.attr);
		err = sysfs_create_file(power_kobj,
				&apm86xxx_suspend_deepsleep_timer.attr);
		err = sysfs_create_file(power_kobj,
				&apm86xxx_suspend_gpio_wakeup.attr);
		err = sysfs_create_file(power_kobj,
				&apm86xxx_suspend_vscale.attr);
		err = sysfs_create_file(power_kobj,
				&apm86xxx_suspend_iscale.attr);
		err = sysfs_create_file(power_kobj,
				&apm86xxx_wakeup_ledmsk.attr);
		err = sysfs_create_file(power_kobj,
				&apm86xxx_disable_gpio_sd.attr);
	}
#ifndef CONFIG_SMP
	if ((!is_apm86xxx_lite()) && cpu_enabled(1)) {
		err = sysfs_create_file(power_kobj,
				&apm86xxx_suspend_online.attr);
	}
#endif
	err = sysfs_create_file(power_kobj,
				&apm86xxx_suspend_reset.attr);

	if (is_apm86xxx_lite())
		apm86xxx_pm.mode = PWRMODE_SLEEP;

	return &apm86xxx_pm;
}

