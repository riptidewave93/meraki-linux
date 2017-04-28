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
#include <linux/smp.h>
#include <linux/clockchips.h>

#include <asm/irq.h>
#include <asm/smp_twd.h>
#include <asm/pgtable.h>
#include <asm/localtimer.h>

#include <mach/io_map.h>

#define IRQ_LOCALTIMER BCM_INT_ID_PPI13
/*
 * Setup the local clock events for a CPU.
 */

static DEFINE_TWD_LOCAL_TIMER(twd_local_timer,
			      HW_IO_VIRT_TO_PHYS(IPROC_PERIPH_PVT_TIM_REG_VA),
			      IRQ_LOCALTIMER);

void __init iproc_local_timer_init(void)
{
	int err;

	err = twd_local_timer_register(&twd_local_timer);
	if (err)
		printk(KERN_WARNING "iproc: twd_local_timer_register failed %d\n", err);
}
