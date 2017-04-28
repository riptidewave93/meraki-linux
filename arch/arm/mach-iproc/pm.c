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

#include <linux/pm.h>
#include <linux/sched.h>
#include <linux/proc_fs.h>
#include <linux/suspend.h>
#include <linux/sysfs.h>
#include <linux/module.h>

/*
 * Prepare suspend
 */
static int suspend_prepare(void)
{
	return 0;
}

/*
 * Enter the system sleep state
 */
static int suspend_enter(suspend_state_t state)
{
	int ret = 0;
	switch (state) {
		case PM_SUSPEND_STANDBY:
			printk("%s: Entering SUSPEND_STANDBY mode\n", __func__);
			/* Flow through to suspend to memory */
		case PM_SUSPEND_MEM:
			printk("%s: Entering SUSPEND_MEM mode\n", __func__);
			printk(KERN_INFO "%s: Entering run_from_ARAM()\n", __func__);

			printk("%s: Leaving SUSPEND_STANDBY mode\n", __func__);
			break;
	}
	return ret;
}

/*
 * Called when the system has just left a sleep state
 */
static void suspend_finish(void)
{
	return;
}

static struct platform_suspend_ops suspend_ops ={
	.valid = suspend_valid_only_mem,
	.prepare = suspend_prepare,
	.enter = suspend_enter,
	.finish = suspend_finish,
};

int __init pm_init(void)
{
	printk("%s: Initializing Power Management ....\n", __func__);
	suspend_set_ops(&suspend_ops);

	return 0;
}
__initcall(pm_init);
