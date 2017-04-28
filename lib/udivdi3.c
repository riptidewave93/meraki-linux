/*
 * Generic C version of full 64 bit by 64 bit division
 * Extracted from version used by netfilter connection tracking
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * Code generated for this function might be very inefficient
 * for some CPUs, can be overridden by linking arch-specific
 * assembly versions such as arch/sparc/lib/udivdi.S
 */
#include <linux/types.h>
#include <linux/module.h>
#include <asm/div64.h>

uint64_t __udivdi3(uint64_t dividend, uint64_t divisor)
{
	uint32_t d = divisor;

	/* Scale divisor to 32 bits */
	if (divisor > 0xffffffffULL) {
	 unsigned int shift = fls(divisor >> 32);

	 d = divisor >> shift;
	 dividend >>= shift;
	}

	/* avoid 64 bit division if possible */
	if (dividend >> 32)
	 do_div(dividend, d);
	else
	 dividend = (uint32_t) dividend / d;

	return dividend;
}
EXPORT_SYMBOL(__udivdi3);
