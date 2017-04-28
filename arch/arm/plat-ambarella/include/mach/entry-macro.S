/*
 * arch/arm/plat-ambarella/include/mach/entry-macro.S
 *
 * History:
 *	2006/12/27 - [Charles Chiou] created file
 *	2007/12/27 - [Grady Chen] Added VIC2, GPIO0, GPIO1, GPIO2
 *	2009/06/15 - [Anthony Ginger] Optimize with clz.
 *
 * Copyright (C) 2004-2009, Ambarella, Inc.
 *
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 */

#include <mach/hardware.h>
#include <asm/irq.h>

		.macro	stat2nr, stat, nr, tmp
		rsbs	\tmp, \stat, #0
		and	\nr, \tmp, \stat
		clzcc	\nr, \nr
		rsc	\nr, \nr, #32
		teq	\nr, #32
		.endm

		.macro	disable_fiq
		.endm

		.macro	get_irqnr_preamble, base, tmp
		.endm

		.macro	arch_ret_to_user, tmp1, tmp2
		.endm

#if !defined(CONFIG_PLAT_AMBARELLA_CORTEX)

		.macro	get_irqnr_and_base, irqnr, irqstat, base, tmp
		ldr	\base, =(VIC_BASE)
		ldr	\irqstat, [\base, #VIC_IRQ_STA_OFFSET]
		stat2nr	\irqstat, \irqnr, \tmp
#if (VIC_INSTANCES >= 2)
		bne	1000f
		ldr	\base, =(VIC2_BASE)
		ldr	\irqstat, [\base, #VIC_IRQ_STA_OFFSET]
		stat2nr	\irqstat, \irqnr, \tmp
		addne	\irqnr, \irqnr, #VIC2_INT_VEC(0)
#endif
#if (VIC_INSTANCES >= 3)
		bne	1000f
		ldr	\base, =(VIC3_BASE)
		ldr	\irqstat, [\base, #VIC_IRQ_STA_OFFSET]
		stat2nr	\irqstat, \irqnr, \tmp
		addne	\irqnr, \irqnr, #VIC3_INT_VEC(0)
#endif
1000:
		.endm

#else
#include <plat/cortex.h>
#include <asm/hardware/gic.h>

		.macro	get_irqnr_and_base, irqnr, irqstat, base, tmp
		ldr	\base, =AMBARELLA_VA_GIC_CPU_BASE
		ldr	\irqstat, [\base, #GIC_CPU_INTACK]
		ldr	\tmp, =1021
		bic	\irqnr, \irqstat, #0x1c00
		cmp	\irqnr, #29
		cmpcc	\irqnr, \irqnr
		cmpne	\irqnr, \tmp
		cmpcs	\irqnr, \irqnr
		.endm

		.macro	test_for_ipi, irqnr, irqstat, base, tmp
		bic	\irqnr, \irqstat, #0x1c00
		cmp	\irqnr, #16
		it	cc
		strcc	\irqstat, [\base, #GIC_CPU_EOI]
		it	cs
		cmpcs	\irqnr, \irqnr
		.endm

		.macro	test_for_ltirq, irqnr, irqstat, base, tmp
		bic	\irqnr, \irqstat, #0x1c00
		mov	\tmp, #0
		cmp	\irqnr, #29
		itt	eq
		moveq	\tmp, #1
		streq	\irqstat, [\base, #GIC_CPU_EOI]
		cmp	\tmp, #0
		.endm
#endif

