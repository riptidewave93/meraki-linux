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
#include <linux/kernel.h>
#include <linux/cpumask.h>

#include <asm/io.h>
#include <asm/mach/map.h>
#include <asm/hardware/gic.h>
#include <asm/system.h>
#include <asm/cp15.h>
#include <asm/pgtable.h>

#include <mach/hardware.h>
#include <mach/irqs.h>
#include <mach/io_map.h>
#include <mach/iproc.h>


void __init iproc_init_irq(void)
{
	unsigned int cr;

	cr = get_cr();
	cr |= CR_VE;
	set_cr(cr);

    /* start with GLBTIMER */ 

	// rewrote based upon before/after comparision of implementation
	// of omap4-common.c:gic_init_irq().

#warning "DCP TODO: fix interrupt initialization"
	gic_init(0, BCM_INT_ID_PPI0, 
		 IOMEM(IPROC_PERIPH_INT_DISTR_REG_VA),
		 IOMEM(IPROC_GICCPU_VA));
}
