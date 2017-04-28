/*
 * Texas Instruments Ethernet Switch Driver
 *
 * Copyright (C) 2010 Texas Instruments
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation version 2.
 *
 * This program is distributed "as is" WITHOUT ANY WARRANTY of any
 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
/*
 * PTP 1588 clock using the CPTS
 *
 */

#include <linux/device.h>
#include <linux/err.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/irq.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/clk.h>
#include <plat/clock.h>

#include <linux/ptp_clock_kernel.h>
#include <net/cpts.h>

#define DRIVER		"ptp_cpts"
#define N_EXT_TS	1

#define CPTS_REF_CLOCK_NAME		"cpts_rft_clk_ck"

struct cpts_clock {
	struct ptp_clock *ptp_clock;
	struct ptp_clock_info caps;
	struct clk *cpts_ref_clk;
	int exts0_enabled;
	int initial_freq;
};

DEFINE_SPINLOCK(register_lock);

/*
 * Register access functions
 */

/*
 * PTP clock operations
 */

static int ptp_cpts_adjfreq(struct ptp_clock_info *ptp, s32 ppb)
{
	int ret;
	unsigned long freq = 0, target_freq = 0, current_freq = 0;
	int diff;
	int neg_adj = 0;
	u64 adj;
	struct cpts_clock *cpts_clock = container_of(ptp, struct cpts_clock,
			caps);

	current_freq = cpts_clock->cpts_ref_clk->recalc(
			cpts_clock->cpts_ref_clk);
	freq = cpts_clock->initial_freq;

	if (ppb < 0) {
		neg_adj = 1;
		ppb = -ppb;
	}

	adj = freq;
	adj *= ppb;
	diff = div_u64(adj, 1000000000ULL);
	target_freq = (neg_adj ? freq - diff : freq + diff);

	ret = clk_set_rate(cpts_clock->cpts_ref_clk, target_freq);
	if (ret)
		printk(KERN_ERR "Failed to set the rate %d\n", ret);

	return 0;
}

static int ptp_cpts_adjtime(struct ptp_clock_info *ptp, s64 delta)
{
	s64 now;
	unsigned long flags;
	/* struct cpts_clock *cpts_clock = container_of(ptp,
			struct cpts_clock, caps); */

	spin_lock_irqsave(&register_lock, flags);

	cpts_systime_read(&now);
	now += delta;
	cpts_systime_write(now);

	spin_unlock_irqrestore(&register_lock, flags);

	return 0;
}

static int ptp_cpts_gettime(struct ptp_clock_info *ptp, struct timespec *ts)
{
	u64 ns;
	u32 remainder;
	unsigned long flags;
	/* struct cpts_clock *cpts_clock = container_of(ptp,
			struct cpts_clock, caps); */

	spin_lock_irqsave(&register_lock, flags);

	cpts_systime_read(&ns);

	spin_unlock_irqrestore(&register_lock, flags);

	ts->tv_sec = div_u64_rem(ns, 1000000000, &remainder);
	ts->tv_nsec = remainder;
	return 0;
}

static int ptp_cpts_settime(struct ptp_clock_info *ptp,
			   const struct timespec *ts)
{
	u64 ns;
	unsigned long flags;
	/* struct cpts_clock *cpts_clock = container_of(ptp,
			struct cpts_clock, caps); */

	ns = ts->tv_sec * 1000000000ULL;
	ns += ts->tv_nsec;

	spin_lock_irqsave(&register_lock, flags);

	cpts_systime_write(ns);

	spin_unlock_irqrestore(&register_lock, flags);

	return 0;
}

static int ptp_cpts_enable(struct ptp_clock_info *ptp,
			  struct ptp_clock_request *rq, int on)
{
	struct cpts_clock *cpts_clock = container_of(ptp,
			struct cpts_clock, caps);

	switch (rq->type) {
	case PTP_CLK_REQ_EXTTS:
		switch (rq->extts.index) {
		case 0:
			cpts_clock->exts0_enabled = on ? 1 : 0;
			return 0;
		default:
			return -EINVAL;
		}
	default:
		break;
	}

	return -EOPNOTSUPP;
}

static struct ptp_clock_info ptp_cpts_caps = {
	.owner		= THIS_MODULE,
	.name		= "CTPS timer",
	.max_adj	= 66666655,
	.n_ext_ts	= N_EXT_TS,
	.pps		= 0,
	.adjfreq	= ptp_cpts_adjfreq,
	.adjtime	= ptp_cpts_adjtime,
	.gettime	= ptp_cpts_gettime,
	.settime	= ptp_cpts_settime,
	.enable		= ptp_cpts_enable,
};

/* module operations */

static struct cpts_clock cpts_clock;

static void __exit ptp_cpts_exit(void)
{
	clk_disable(cpts_clock.cpts_ref_clk);
	clk_put(cpts_clock.cpts_ref_clk);
	ptp_clock_unregister(cpts_clock.ptp_clock);
}

static int __init ptp_cpts_init(void)
{
	cpts_clock.cpts_ref_clk = clk_get(NULL, CPTS_REF_CLOCK_NAME);
	if (IS_ERR(cpts_clock.cpts_ref_clk))
		printk(KERN_ERR "Could not get %s clk\n", CPTS_REF_CLOCK_NAME);
	clk_enable(cpts_clock.cpts_ref_clk);

	cpts_clock.initial_freq = cpts_clock.cpts_ref_clk->recalc(
			cpts_clock.cpts_ref_clk);

	cpts_clock.caps = ptp_cpts_caps;

	cpts_clock.ptp_clock = ptp_clock_register(&cpts_clock.caps);

	if (IS_ERR(cpts_clock.ptp_clock))
		return PTR_ERR(cpts_clock.ptp_clock);

	return 0;
}

module_init(ptp_cpts_init);
module_exit(ptp_cpts_exit);

MODULE_AUTHOR("Texas Instruments");
MODULE_DESCRIPTION("PTP clock using the TI CPTS timer");
MODULE_LICENSE("GPL");
