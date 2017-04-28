/*
 * TI816X FAPLL clock functions
 *
 * Copyright (C) 2011 Texas Instruments, Inc. - http://www.ti.com/
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation version 2.
 *
 * This program is distributed "as is" WITHOUT ANY WARRANTY of any
 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/clk.h>
#include <linux/io.h>
#include <linux/delay.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/list.h>

#include <asm/div64.h>
#include <plat/clock.h>

#include "clock.h"
#include "cm-regbits-81xx.h"

/* FAPLL rate rounding: minimum FAPLL multiplier, divider values */
#define FAPLL_MIN_MULTIPLIER		2
#define FAPLL_MIN_DIVIDER		1
#define FAPLL_MAX_DIVIDER		255

/* FAPLL fvco frequency band limits */
#define FAPLL_FVCO_BAND_MIN		800000000
#define FAPLL_FVCO_BAND_MAX		1850000000

#define MAX_FAPLL_WAIT_TRIES		1000
#define	ROUNDING_DIVIDER		1000

#define TI816X_FAPLL_FREQ_MIN_VALUE	8
#define TI816X_FAPLL_FREQ_MAX_VALUE	15
#define TI816X_FAPLL_K			8

/* Enable disable flags */
#define TI816X_FAPLL_DISABLE		0x0
#define TI816X_FAPLL_ENABLE		0x1

#define TI816X_FAPLL_SYN_POWER_DOWN	0x1
#define TI816X_FAPLL_SYN_POWER_UP	0x0

#define TI816X_FAPLL_LOCKED		0x1

#define TI816X_FAPLL_BYPASS_EN		0x1
#define TI816X_FAPLL_BYPASS_DIS		0x0

#define TI816X_LOAD_DIV1_VALUE		0x1
#define TI816X_LOAD_FREQ_VALUE		0x1
#define TI816X_TRUNC_FREQ_VALUE_EN	0x1

struct ti816x_fapll_def {
	char		*clk_name;
	unsigned long	freq;
	bool		default_available;
};

/*
 * Initialization wrapper used to define an FAPLL set rate inputs
 * for TI816X variants.
 */
#define FAPLL_INITIALIZER(_clk_name, _enabled, _freq)		\
{								\
	.clk_name		= _clk_name,			\
	.default_available	= _enabled,			\
	.freq			= _freq,			\
}

static struct ti816x_fapll_def __initdata ti816x_freq_def_list[] = {
	/* Main PLL configuration */
	/* FAPLL - main_pll_clk1_ck, dsp*/
	FAPLL_INITIALIZER("main_pll_clk1_ck", true, 800000000),
	/* FAPLL - main_pll_clk3_ck, IVA HDs*/
	FAPLL_INITIALIZER("main_pll_clk3_ck", true, 532000000),
	/* FAPLL - main_pll_clk6_ck, usb clock*/
	FAPLL_INITIALIZER("main_pll_clk6_ck", true, 24000000),
	/* FAPLL - main_pll_clk7_ck, audio pll ref clock*/
	FAPLL_INITIALIZER("main_pll_clk7_ck", true, 432000000),

	/* VIDEO PLL configuration */
	/* FAPLL - video_pll_clk1_ck*/
	FAPLL_INITIALIZER("video_pll_clk1_ck", true, 216000000),
	/* FAPLL - video_pll_clk2_ck*/
	FAPLL_INITIALIZER("video_pll_clk2_ck", true, 594000000),
	/* FAPLL - video_pll_clk3_ck*/
	FAPLL_INITIALIZER("video_pll_clk3_ck", true, 594000000),

	/* AUDIO PLL configuration */
	/* FAPLL - audio_pll_clk2_ck*/
	FAPLL_INITIALIZER("audio_pll_clk2_ck", true, 160000000),
	/* FAPLL - audio_pll_clk3_ck*/
	FAPLL_INITIALIZER("audio_pll_clk3_ck", true, 196608000),
	/* FAPLL - audio_pll_clk4_ck*/
	FAPLL_INITIALIZER("audio_pll_clk4_ck", true, 45158400),
	/* FAPLL - audio_pll_clk5_ck*/
	FAPLL_INITIALIZER("audio_pll_clk5_ck", true, 32768000),
};

/* Private functions */

/* _ti816x_wait_fapll_status: wait for a FAPLL to enter a specific state
 *
 * @clk: pointer to a FAPLL struct clk
 * @state: locked state
 *
 * Checks the FAPLL status whether it is in locked state or not
 * If the FAPLL locked successfully, return 0; if the FAPLL
 * did not lock in the time allotted, or FAPLL was passed in,
 * return -EINVAL.
 */
static int _ti816x_wait_fapll_status(struct clk *clk, u8 state)
{
	const struct fapll_data *fd;
	int i = 0;
	int ret = -EINVAL;

	fd = clk->fapll_data;

	state <<= __ffs(fd->lock_sts_mask);

	while (((__raw_readl(fd->control_reg) & fd->lock_sts_mask) != state) &&
			i < MAX_FAPLL_WAIT_TRIES) {
		i++;
		udelay(1);
	}

	if (i == MAX_FAPLL_WAIT_TRIES) {
		pr_err("clock: %s failed transition to 'locked'\n",
			clk->name);
	} else {
		pr_debug("clock: %s transition to 'locked' in %d loops\n",
			clk->name, i);
		ret = 0;
	}

	return ret;
}

/*
 * _ti816x_rel_fapll_bypass - instruct a FAPLL to release from bypass
 *						and wait for readiness
 * @clk: pointer to a FAPLL struct clk
 *
 * Instructs a FAPLL to release from bypass mode. In bypass mode,
 * the FAPLL's rate is set equal to its parent clock's rate.
 * Waits for the FAPLL to report readiness before returning. If the
 * FAPLL entered bypass mode successfully, return 0; if the FAPLL
 * did not enter bypass in the time allotted, or FAPLL was passed in,
 * or the FAPLL does not support bypass, return -EINVAL.
 */
static int _ti816x_rel_fapll_bypass(struct clk *clk)
{
	u32 v;
	const struct fapll_data *fd;
	fd = clk->fapll_data;

	v = __raw_readl(fd->control_reg);
	v &= ~fd->bypass_mask;
	v |= TI816X_FAPLL_BYPASS_DIS << __ffs(fd->bypass_mask);
	__raw_writel(v, fd->control_reg);

	return 0;
}

/*
 * _ti816x_put_fapll_bypass - instruct a FAPLL to bypass and wait for
 *							readiness
 * @clk: pointer to a FAPLL struct clk
 *
 * Instructs a FAPLL to enter bypass mode. In bypass mode,
 * the FAPLL's rate is set equal to its parent clock's rate.
 * Waits for the FAPLL to report readiness before returning. If the
 * FAPLL entered bypass mode successfully, return 0; if the FAPLL
 * did not enter bypass in the time allotted, or FAPLL was passed in,
 * or the FAPLL does not support bypass, return -EINVAL.
 */
static int _ti816x_put_fapll_bypass(struct clk *clk)
{
	u32 v;
	const struct fapll_data *fd;
	fd = clk->fapll_data;

	if (!(clk->fapll_data->modes & FAPLL_LOW_POWER_BYPASS))
		return -EINVAL;

	pr_debug("clock: configuring FAPLL %s for bypass\n",
		clk->name);

	v = __raw_readl(fd->control_reg);
	v &= ~fd->bypass_mask;
	v |= TI816X_FAPLL_BYPASS_EN << __ffs(fd->bypass_mask);
	__raw_writel(v, fd->control_reg);

	return 0;
}

/*
 * _ti816x_fapll_lock - instruct a FAPLL to lock and wait for readiness
 * @clk: pointer to a FAPLL struct clk
 *
 * Instructs a FAPLL to lock. Waits for the FAPLL to report
 * readiness before returning. If the FAPLL locked successfully,
 * return 0; if the FAPLL did not lock in the time allotted, or
 * FAPLL was passed in, return -EINVAL.
 */
static int _ti816x_fapll_lock(struct clk *clk)
{
	int r;
	const struct fapll_data *fd;
	fd = clk->fapll_data;

	r = _ti816x_wait_fapll_status(clk, TI816X_FAPLL_LOCKED);

	return r;
}

/*
 * _ti816x_fapll_syn_pwr_up - instruct FAPLL synthesizer to power up
 * @clk: pointer to a FAPLL struct clk
 *
 * Instructs a FAPLL to trun on the coresponding synthesizer.
 * If FAPLL was passed in, or the FAPLL does not support
 * low-power stop, return -EINVAL; otherwise, return 0.
 */
static int _ti816x_fapll_syn_pwr_up(struct clk *clk)
{
	u32 v;
	const struct fapll_data *fd;
	fd = clk->fapll_data;

	pr_debug("clock: stopping FAPLL synthesizer %s\n", clk->name);

	v = __raw_readl(fd->pwd_reg);
	v &= ~clk->pwd_mask;
	v |= TI816X_FAPLL_SYN_POWER_UP << __ffs(clk->pwd_mask);
	__raw_writel(v, fd->pwd_reg);

	return 0;
}

/*
 * _ti816x_fapll_syn_pwr_down - instruct FAPLL synthesizer to power down
 * @clk: pointer to a FAPLL struct clk
 *
 * Instructs a FAPLL to enter low-power stop/disable synthesizer.
 * If FAPLL was passed in, or the FAPLL does not support
 * low-power stop, return -EINVAL; otherwise, return 0.
 */
static int _ti816x_fapll_syn_pwr_down(struct clk *clk)
{
	u32 v;
	const struct fapll_data *fd;
	fd = clk->fapll_data;

	pr_debug("clock: stopping FAPLL synthesizer %s\n", clk->name);

	v = __raw_readl(fd->pwd_reg);
	v &= ~clk->pwd_mask;
	v |= TI816X_FAPLL_SYN_POWER_DOWN << __ffs(clk->pwd_mask);
	__raw_writel(v, fd->pwd_reg);


	return 0;
}

/**
 * _ti816x_fapll_stop - instruct a FAPLL to enter low-power stop
 * @clk: pointer to a FAPLL struct clk
 *
 * Instructs a FAPLL synthesizer to enter into power down mode. Also
 * diable the fapll to go to low power state. This function is
 * intended for use in struct clkops. No return value.
 */
void _ti816x_fapll_stop(struct clk *clk)
{
	u32 v;
	struct fapll_data *fd = clk->fapll_data;

	_ti816x_fapll_syn_pwr_down(clk);

	v = __raw_readl(fd->pwd_reg);

	if (v == 0) {
		/* Power down FAPLL if all the synthesizers are
		 * in power down mode */
		v = __raw_readl(fd->control_reg);
		v &= ~fd->enable_mask;
		v |= TI816X_FAPLL_DISABLE << __ffs(fd->enable_mask);
		__raw_writel(v, fd->control_reg);
	}
}

/*
 * _fapll_test_fvco - test the valid fvco value for the FAPLL
 * @ref_rate: FAPLL struct reference clk rate to test
 * @n: multiplier value (N) to test
 * @p: divider value (P) to test
 *
 * Tests whether a particular divider @p, multiplier @n will result in a
 * valid FAPLL internal clock frequency fvco. Returns 0 if OK or -1 if
 * fvco frequency is not in the band of valid values
 */
static unsigned long _fapll_test_fvco(unsigned long ref_rate, u16 n, u8 p)
{
	unsigned long fvco;

	fvco = (ref_rate/p) * n;
	if ((fvco < FAPLL_FVCO_BAND_MIN) || (fvco > FAPLL_FVCO_BAND_MAX)) {
		pr_err("rejecting n=%u, p=%u due"
				" to fvco failure\n", n, p);
		return -EINVAL;
	} else
		return 0;
}

/*
 * _fapll_get_rounded_vals - get the rounded diviser and multiplier vals
 * @clk: FAPLL struct clk
 * @target_rate: desired FAPLL clock rate
 *
 * Gives the rounded post divider and FREQ values for computing the rate
 * Returns -EINVAL upon error, or 0 upon success.
 */
static int _fapll_get_rounded_vals(struct clk *clk, unsigned long target_rate)
{
	int i, m;
	struct fapll_data *fd;
	unsigned long freq_val;
	unsigned int freq, reminder, quotient;

	fd = clk->fapll_data;
	if (!fd)
		return -EINVAL;

	freq_val = ((clk->parent->rate/ROUNDING_DIVIDER) * fd->mult_n *
				TI816X_FAPLL_K)/fd->pre_div_p;
	target_rate = target_rate/ROUNDING_DIVIDER;

	reminder = freq_val%target_rate;
	quotient = freq_val/target_rate;

	if (clk->freq_flag != 1) {
		fd->last_rounded_freq_int = 0;
		fd->last_rounded_freq_frac = 0;
		fd->last_rounded_m = freq_val/(target_rate * TI816X_FAPLL_K);
		if (fd->last_rounded_m > fd->max_divider)
			return -EINVAL;
		return 0;
	}

	if (reminder == 0) {
		for (i = TI816X_FAPLL_FREQ_MAX_VALUE;
			i >= TI816X_FAPLL_FREQ_MIN_VALUE; i--) {
			if (quotient%i == 0) {
				m = quotient/i;
				if (m > FAPLL_MAX_DIVIDER)
					return -EINVAL;
				fd->last_rounded_m = m;
				fd->last_rounded_freq_int = i;
				fd->last_rounded_freq_frac = 0;
				break;
			}
		}
		if (i < TI816X_FAPLL_FREQ_MIN_VALUE)
			return -EINVAL;
	} else {
		for (i = FAPLL_MIN_DIVIDER; i <= fd->max_divider; i++) {
			freq = quotient/i;
			if ((freq >= TI816X_FAPLL_FREQ_MIN_VALUE) &&
				(freq <= TI816X_FAPLL_FREQ_MAX_VALUE)) {
				fd->last_rounded_m = i;
				fd->last_rounded_freq_int = freq;
				fd->last_rounded_freq_frac =
					((((quotient%i) << 16)/i +
					((reminder/ROUNDING_DIVIDER) << 16) /
					(i*(target_rate/ROUNDING_DIVIDER)))<<8);
					break;
			}
		}
		if (i > fd->max_divider)
			return -EINVAL;
	}
	return 0;
}

/* Public functions */

void ti816x_init_fapll_parent(struct clk *clk)
{
	u32 v;
	struct fapll_data *fd;

	fd = clk->fapll_data;
	if (!fd)
		return;

	/* Return bypass rate if FAPLL is bypassed */
	v = __raw_readl(fd->control_reg);
	v &= fd->bypass_mask;
	v >>= __ffs(fd->bypass_mask);

	/* Reparent in case the fapll is in bypass */
	if (v == fd->bypass_en)
		clk_reparent(clk, fd->clk_bypass);

	return;
}

/**
 * ti816x_fapll_get_rate - returns the current FAPLL CLKOUT rate
 * @clk: struct clk * of a FAPLL
 *
 * FAPLLs can be locked or bypassed - basically, enabled or disabled.
 * When locked, the FAPLL output depends on the M, N and FREG values.
 * When bypassed, on TI816x, the output rate is sys_clk. Returns the
 * current FAPLL CLKOUT rate if the FAPLL is locked, or the appropriate
 * bypass rate if the FAPLL is bypassed, or 0 if the clock @clk is not a FAPLL.
 */
u32 ti816x_fapll_get_rate(struct clk *clk)
{
	long long fapll_clk;
	long long fapll_num, fapll_den;
	u32 fapll_n, fapll_p, fapll_m, v;
	u32 frac = 0;
	u32 integ = 0;
	struct fapll_data *fd;

	fd = clk->fapll_data;
	if (!fd)
		return 0;

	pr_debug("clock: %s, reference clock rate %lu\n",
				clk->name, fd->clk_ref->rate);

	/* Return bypass rate if FAPLL is bypassed */
	v = __raw_readl(fd->control_reg);
	v &= fd->bypass_mask;
	v >>= __ffs(fd->bypass_mask);

	if (v == fd->bypass_en)
		return fd->clk_bypass->rate;

	/* N and P values */
	v = __raw_readl(fd->control_reg);
	fapll_n = v & fd->mult_mask;
	fapll_n >>= __ffs(fd->mult_mask);
	fapll_p = v & fd->div_mask;
	fapll_p >>= __ffs(fd->div_mask);

	/* freq value from sythesizer reg, fractional and integer parts */
	if (clk->freq_flag == 1) {
		v = __raw_readl(clk->freq_reg);
		frac = v & fd->freq_frac_mask;
		frac >>= __ffs(fd->freq_frac_mask);
		integ = v & fd->freq_int_mask;
		integ >>= __ffs(fd->freq_int_mask);
	}

	/* post_div (M) value */
	v = __raw_readl(clk->post_div_reg);
	fapll_m = v & fd->post_div_mask;
	fapll_m >>= __ffs(fd->post_div_mask);

	if (clk->freq_flag == 1) {
		fapll_num = (TI816X_FAPLL_K * fapll_n);
		/* Truncating last 8 bits to reach all the freq's */
		fapll_num <<= 16;
		fapll_den = ((integ << 24) + frac) >> 8;
		fapll_den = fapll_den * fapll_m;
	} else {
		/* Out put is directly taking from VCO so no K */
		fapll_num = fapll_n;
		fapll_den = fapll_m;
	}

	fapll_clk = (long long)fd->clk_ref->rate * fapll_num;
	do_div(fapll_clk, fapll_den);
	do_div(fapll_clk, fapll_p);


	return fapll_clk;
}

/*
 * ti816x_fapll_program - set non-core FAPLL M,N values directly
 * @clk: struct clk * of FAPLL to set
 * @m: FAPLL multiplier to set
 * @n: FAPLL divider to set
 * @freq_int: integer Freq value to set
 * @freq_frac: fractional freq value to set
 *
 * Program the FAPLL with the supplied M, N values, and wait for the FAPLL to
 * lock.. Returns -EINVAL upon error, or 0 upon success.
 */
static int ti816x_fapll_program(struct clk *clk, u16 n, u8 p, u8 m,
						u8 freq_int, u32 freq_frac)
{
	struct fapll_data *fd = clk->fapll_data;
	u32 v;

	/* Put FAPLL in bypass mode */
	_ti816x_put_fapll_bypass(clk);

	/* Set FAPLL multiplier, divider */
	v = __raw_readl(fd->control_reg);
	v &= ~(fd->mult_mask | fd->div_mask);
	v |= n << __ffs(fd->mult_mask);
	v |= p << __ffs(fd->div_mask);
	__raw_writel(v, fd->control_reg);

	/* set FAPLL post divider */
	v = __raw_readl(clk->post_div_reg);
	v &= ~(fd->post_div_mask);
	v |= m << __ffs(fd->post_div_mask);
	v |= TI816X_LOAD_DIV1_VALUE << __ffs(fd->lddiv1_mask);
	__raw_writel(v, clk->post_div_reg);

	/* set FAPLL fractional and integer part of freq */
	if (clk->freq_flag == 1) {
		v = __raw_readl(clk->freq_reg);
		v &= ~(fd->freq_int_mask | fd->freq_frac_mask);
		v |= freq_int << __ffs(fd->freq_int_mask);
		v |= freq_frac << __ffs(fd->freq_frac_mask);
		v |= TI816X_TRUNC_FREQ_VALUE_EN << __ffs(fd->trunc_mask);
		v |= TI816X_LOAD_FREQ_VALUE << __ffs(fd->ldfreq_mask);
		__raw_writel(v, clk->freq_reg);
	}

	/* We let the clock framework set the other output dividers later */

	/* REVISIT: Set ramp-up delay? */

	/* Release FAPLL from bypass mode */
	_ti816x_rel_fapll_bypass(clk);

	_ti816x_fapll_lock(clk);

	return 0;
}

/**
 * ti816x_fapll_recalc - recalculate FAPLL rate
 * @clk: FAPLL struct clk
 *
 * Recalculate and propagate the FAPLL rate.
 */
unsigned long ti816x_fapll_recalc(struct clk *clk)
{
	return ti816x_fapll_get_rate(clk);
}

/* Non-CORE FAPLL (e.g., FAPLLs that do not control SDRC) clock functions */

/**
 * ti816x_fapll_enable - instruct a FAPLL to enter bypass or lock mode
 * @clk: pointer to a FAPLL struct clk
 *
 * Instructs a to enable, e.g., to enter bypass or lock. The choice of
 * modes depends on the FAPLL's programmed rate: if it is the same as
 * the FAPLL's parent clock, it will enter bypass; otherwise, it will
 * enter lock. This code will wait for the FAPLL to indicate readiness
 * before returning, unless the FAPLL takes too long to enter the target
 * state. Intended to be used as the struct clk's enable function.
 * If FAPLL was passed in, or if the FAPLL took too long to enter
 * bypass or lock, return -EINVAL; otherwise, return 0.
 */
int ti816x_fapll_enable(struct clk *clk)
{
	int r;
	u32 v;
	struct fapll_data *fd;

	fd = clk->fapll_data;
	if (!fd)
		return -EINVAL;

	/* Enable FAPLL */
	v = __raw_readl(fd->control_reg);
	v &= ~fd->enable_mask;
	v |= TI816X_FAPLL_ENABLE << __ffs(fd->enable_mask);
	__raw_writel(v, fd->control_reg);

	r = _ti816x_fapll_syn_pwr_up(clk);
	if (r) {
		pr_err("Failed to power up the synthesizer\n");
		return r;
	}

	if (clk->rate == fd->clk_bypass->rate) {
		WARN_ON(clk->parent != fd->clk_bypass);
		r = _ti816x_put_fapll_bypass(clk);
	}

	/*
	 *FIXME: this is dubious - if clk->rate has changed, what about
	 * propagating?
	 */
	if (!r)
		clk->rate = ti816x_fapll_get_rate(clk);

	return r;
}

/**
 * ti816x_fapll_disable - instruct a FAPLL to enter low-power stop
 * @clk: pointer to a FAPLL struct clk
 *
 * Instructs a FAPLL to enter into power down state. This function is
 * intended for use in struct clkops. No return value.
 */
void ti816x_fapll_disable(struct clk *clk)
{
	_ti816x_fapll_stop(clk);
}

/**
 * ti816x_fapll_set_rate - set non-core FAPLL rate
 * @clk: struct clk * of FAPLL to set
 * @rate: rounded target rate
 *
 * Set the FAPLL CLKOUT to the target rate. If the FAPLL can enter
 * low-power bypass, and the target rate is the bypass source clock
 * rate, then configure the FAPLL for bypass. Otherwise, round the
 * target rate if it hasn't been done already, then program and lock
 * the FAPLL. Returns -EINVAL upon error, or 0 upon success.
 */
int ti816x_fapll_set_rate(struct clk *clk, unsigned long rate)
{
	struct clk *new_parent = NULL;
	struct fapll_data *fd;
	int ret;

	if (!clk || !rate)
		return -EINVAL;

	fd = clk->fapll_data;
	if (!fd)
		return -EINVAL;

	if (rate == ti816x_fapll_get_rate(clk))
		return 0;

	ret = ti816x_fapll_enable(clk);
	if (ret)
		pr_err("failed to enable the fapll\n");

	/*
	 * Ensure both the bypass and ref clocks are enabled prior to
	 * doing anything; we need the bypass clock running to reprogram
	 * the FAPLL.
	 */
	omap2_clk_enable(fd->clk_bypass);
	omap2_clk_enable(fd->clk_ref);

	if (fd->clk_bypass->rate == rate &&
		(clk->fapll_data->modes & FAPLL_LOW_POWER_BYPASS)) {
		pr_debug("clock: %s: set rate: entering bypass.\n", clk->name);

		ret = _ti816x_put_fapll_bypass(clk);
		if (!ret)
			new_parent = fd->clk_bypass;
	} else {
		if (fd->last_rounded_rate != rate)
			ti816x_fapll_round_rate(clk, rate);

		if (fd->last_rounded_rate == 0)
			return -EINVAL;

		pr_debug("clock: %s: set rate: locking rate to %lu.\n",
				clk->name, fd->last_rounded_rate);

		ret = ti816x_fapll_program(clk, fd->mult_n,
						fd->pre_div_p,
						fd->last_rounded_m,
						fd->last_rounded_freq_int,
						fd->last_rounded_freq_frac);
		if (!ret)
			new_parent = fd->clk_ref;
	}
	if (!ret) {
		/*
		 * Switch the parent clock in the hierarchy, and make sure
		 * that the new parent's usecount is correct. Note: we
		 * enable the new parent before disabling the old to avoid
		 * any unnecessary hardware disable->enable transitions.
		 */
		if (clk->usecount) {
			omap2_clk_enable(new_parent);
			omap2_clk_disable(clk->parent);
		}
		clk_reparent(clk, new_parent);
		clk->rate = fd->last_rounded_rate;
	}
	omap2_clk_disable(fd->clk_ref);
	omap2_clk_disable(fd->clk_bypass);

	return 0;
}

/* FAPLL rate rounding code */

/**
 * ti816x_fapll_set_rate_tolerance: set the err tolerance during rate rounding
 * @clk: struct clk * of the FAPLL
 * @tolerance: maximum rate error tolerance
 *
 * Set the maximum FAPLL rate error tolerance for the rate rounding
 * algorithm. The rate tolerance is an attempt to balance FAPLL power
 * saving (the least divider value "n") vs. rate fidelity (the least
 * difference between the desired FAPLL target rate and the rounded
 * rate out of the algorithm). So, increasing the tolerance is likely
 * to decrease FAPLL power consumption and increase FAPLL rate error.
 * Returns -EINVAL if provided a null clock ptr or a clk that is not a
 * FAPLL; or 0 upon success.
 */
int ti816x_fapll_set_rate_tolerance(struct clk *clk, unsigned int tolerance)
{
	if (!clk || !clk->fapll_data)
		return -EINVAL;

	clk->fapll_data->rate_tolerance = tolerance;

	return 0;
}

static unsigned long _fapll_syn_compute_new_rate(unsigned long ref_rate,
			unsigned int n, unsigned int p, unsigned int m,
			unsigned int f_int, unsigned int f_frac)
{
	unsigned long long num;
	unsigned long long den;

	if (f_int > 0) {
		num = (unsigned long long) ((TI816X_FAPLL_K * n) << 16);
		den = (unsigned long long) (((f_int << 24) + f_frac)>>8) * m;
	} else {
		num = (unsigned long long) n;
		den = (unsigned long long) m;
	}

	num = (long long)ref_rate * num;

	do_div(num, den);
	do_div(num, p);

	return num;
}

/**
 * ti816x_fapll_round_rate - round a target rate for an OMAP FAPLL
 * @clk: struct clk * for a FAPLL
 * @target_rate: desired FAPLL clock rate
 *
 * Given a FAPLL, a desired target rate, and a rate tolerance, round
 * the target rate to a possible, programmable rate for this FAPLL.
 * Rate tolerance is assumed to be set by the caller before this
 * function is called. Attempts to select the minimum possible n
 * within the tolerance to reduce power consumption. Stores the
 * computed (m, n) in the FAPLL's fapll_data structure so set_rate()
 * will not need to call this (expensive) function again. Returns ~0
 * if the target rate cannot be rounded, either because the rate is
 * too low or because the rate tolerance is set too tightly; or the
 * rounded rate upon success.
 */
long ti816x_fapll_round_rate(struct clk *clk, unsigned long target_rate)
{
	struct fapll_data *fd;
	int ret;

	if (!clk || !clk->fapll_data)
		return ~0;

	fd = clk->fapll_data;
	if (!fd)
		return -EINVAL;

	pr_debug("clock: starting FAPLL round_rate for clock %s, target rate "
					"%ld\n", clk->name, target_rate);
	fd->last_rounded_rate = 0;

	ret = _fapll_test_fvco(clk->parent->rate, fd->mult_n, fd->pre_div_p);
	if (ret)
		return ret;

	ret = _fapll_get_rounded_vals(clk, target_rate);
	if (ret) {
		pr_err("Failed to get the rounded values\n");
		return ret;
	}

	fd->last_rounded_rate = _fapll_syn_compute_new_rate(fd->clk_ref->rate,
				fd->mult_n, fd->pre_div_p, fd->last_rounded_m,
				fd->last_rounded_freq_int,
				fd->last_rounded_freq_frac);

	return fd->last_rounded_rate;
}

/**
 * ti816x_set_fapll_freq() - Set FAPLL frequencies
 * @freq_def:		freq default list for this silicon
 * @freq_def_size:	number of freq entries for this silicon
 *
 * Set the frequency defined in the table and reprogram the FAPLL
 * with new set of values. This is meant to be used only by clock
 * framework. This API should call after ti816x_clk_init() for
 * setting the new frequencies. User need to update the default
 * rate to the one new rate according to the requirement.
 */
int __init ti816x_set_fapll_freqs(struct ti816x_fapll_def *freq_def,
		u32 freq_def_size)
{
	int i;
	int ret = 0;
	struct clk *clk;

	if (!freq_def || !freq_def_size) {
		pr_err("%s: invalid params!\n", __func__);
		return -EINVAL;
	}

	/* Lets now set the rate with default values */
	for (i = 0; i < freq_def_size; i++) {
		if (freq_def[i].default_available == true) {
			clk = clk_get(NULL, freq_def[i].clk_name);
			if (WARN(IS_ERR(clk), "Failed to get %s.\n",
						freq_def[i].clk_name))
				ret = -1;

			if (!ret)
				if (clk_set_rate(clk, freq_def[i].freq))
					pr_err("Unable to set the frequency"
						" (%lu)\n", freq_def[i].freq);
		}

	}
	return 0;
}

/**
 * ti816x_fapll_init() - initialize rest of the fapll synthesizers
 */
int __init ti816x_fapll_init(void)
{
	int r = -ENODEV;

	r = ti816x_set_fapll_freqs(ti816x_freq_def_list,
			ARRAY_SIZE(ti816x_freq_def_list));

	return r;
}

