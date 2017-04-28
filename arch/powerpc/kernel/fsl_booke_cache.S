/*
 * Copyright 2009-2012 Freescale Semiconductor, Inc.
 *	Scott Wood <scottwood@freescale.com>
 *	Dave Liu <daveliu@freescale.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#include <asm/reg.h>
#include <asm/page.h>
#include <asm/ppc_asm.h>
#include <asm/asm-offsets.h>

	.section .text

/********    L1 Cache    ********/

/* flush L1 d-cache */
_GLOBAL(flush_dcache_L1)
	mfspr	r3,SPRN_L1CFG0

	rlwinm	r5,r3,9,3	/* Extract cache block size */
	twlgti	r5,1		/* Only 32 and 64 byte cache blocks
				 * are currently defined.
				 */
	li	r4,32
	subfic	r6,r5,2		/* r6 = log2(1KiB / cache block size) -
				 *      log2(number of ways)
				 */
	slw	r5,r4,r5	/* r5 = cache block size */

	rlwinm	r7,r3,0,0xff	/* Extract number of KiB in the cache */
	mulli	r7,r7,13	/* An 8-way cache will require 13
				 * loads per set.
				 */
	slw	r7,r7,r6

	/* save off HID0 and set DCFA */
	mfspr	r8,SPRN_HID0
	ori	r9,r8,HID0_DCFA@l
	mtspr	SPRN_HID0,r9
	isync

	LOAD_REG_IMMEDIATE(r4, KERNELBASE)
	mtctr	r7

1:	lwz	r3,0(r4)	/* Load... */
	add	r4,r4,r5
	bdnz	1b

	msync
	LOAD_REG_IMMEDIATE(r4, KERNELBASE)
	mtctr	r7

1:	dcbf	0,r4		/* ...and flush. */
	add	r4,r4,r5
	bdnz	1b

	/* restore HID0 */
	mtspr	SPRN_HID0,r8
	isync

	blr

#define PVR_E6500	0x8040

/* Flush L1 d-cache, invalidate and disable d-cache and i-cache */
_GLOBAL(__flush_disable_L1)
/* L1 Data Cache of e6500 contains no modified data, no flush is required */
	mfspr	r3, SPRN_PVR
	rlwinm	r4, r3, 16, 0xffff
	lis	r5, 0
	ori	r5, r5, PVR_E6500@l
	cmpw	r4, r5
	beq	2f
	mflr	r10
	bl	flush_dcache_L1	/* Flush L1 d-cache */
	mtlr	r10

2:	msync
	mfspr	r4, SPRN_L1CSR0	/* Invalidate and disable d-cache */
	li	r5, 2
	rlwimi	r4, r5, 0, 3

	msync
	isync
	mtspr	SPRN_L1CSR0, r4
	isync

	msync
1:	mfspr	r4, SPRN_L1CSR0	/* Wait for the invalidate to finish */
	andi.	r4, r4, 2
	bne	1b

	msync
	mfspr	r4, SPRN_L1CSR1	/* Invalidate and disable i-cache */
	li	r5, 2
	rlwimi	r4, r5, 0, 3

	msync
	isync
	mtspr	SPRN_L1CSR1, r4
	isync
	msync

	blr

/********    Backside L2 Cache    ********/

#define SVR_P2040	0x821000

need_L2_cache:
	/* skip L2 cache on P2040/P2040E as they have no L2 cache */
	mfspr	r3, SPRN_SVR
	/* shift right by 8 bits and clear E bit of SVR */
	rlwinm	r4, r3, 24, ~0x800

	lis	r3, SVR_P2040@h
	ori	r3, r3, SVR_P2040@l
	cmpw	r4, r3
	beq	1f

	/* If L2 cache is disabled, skip it */
	mfspr	r3, SPRN_L2CSR0
	andis.	r3, r3, L2CSR0_L2E@h
	beq	1f

	li	r3, 0
	blr
1:
	li	r3, 1
	blr

/* flush backside L2 cache */
_GLOBAL(flush_backside_L2_cache)
	mflr	r10
	bl	need_L2_cache
	mtlr	r10
	cmpwi	r3, 0
	bne	2f

__flush_backside_L2_cache:
	/* Flush the L2 cache */
	mfspr	r3, SPRN_L2CSR0
	ori	r3, r3, L2CSR0_L2FL@l
	msync
	isync
	mtspr	SPRN_L2CSR0,r3
	isync
1:
	mfspr	r3,SPRN_L2CSR0
	andi.	r3, r3, L2CSR0_L2FL@l
	bne	1b
2:
	blr

/* flush and disable backside L2 cache */
_GLOBAL(disable_backside_L2_cache)
	mflr	r10
	bl	need_L2_cache
	mtlr	r10
	cmpwi	r3, 0
	bne	1f

	mflr	r10
	bl	__flush_backside_L2_cache
	mtlr	r10

	/* disable L2 cache */
	li	r3, 0
	msync
	isync
	mtspr	SPRN_L2CSR0, r3
	isync
1:
	blr

/********    Platform Cache    ********/

#define L2CTL_L2E	0x80000000
#define L2CTL_L2I	0x40000000

/* r3 = virtual address of L2 controller, WIMG = 01xx */
_GLOBAL(flush_disable_L2)
	/* It's a write-through cache, so only invalidation is needed. */
	mbar
	isync
	lwz	r4, 0(r3)
	li	r5, 1
	rlwimi	r4, r5, 30, L2CTL_L2E | L2CTL_L2I
	stw	r4, 0(r3)

	/* Wait for the invalidate to finish */
1:	lwz	r4, 0(r3)
	andis.	r4, r4, L2CTL_L2I@h
	bne	1b
	mbar

	blr

/* r3 = virtual address of L2 controller, WIMG = 01xx */
_GLOBAL(invalidate_enable_L2)
	mbar
	isync
	lwz	r4, 0(r3)
	li	r5, 3
	rlwimi	r4, r5, 30, L2CTL_L2E | L2CTL_L2I
	stw	r4, 0(r3)

	/* Wait for the invalidate to finish */
1:	lwz	r4, 0(r3)
	andis.	r4, r4, L2CTL_L2I@h
	bne	1b
	mbar

	blr
