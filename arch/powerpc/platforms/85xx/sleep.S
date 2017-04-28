/*
 * Enter and leave deep sleep/sleep state on MPC85xx
 *
 * Author: Scott Wood <scottwood@freescale.com>
 *
 * Copyright (C) 2006-2012 Freescale Semiconductor, Inc. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 */

#include <asm/page.h>
#include <asm/ppc_asm.h>
#include <asm/reg.h>
#include <asm/asm-offsets.h>

#define SS_TB		0x00
#define SS_HID		0x08 /* 2 HIDs */
#define SS_IAC		0x10 /* 2 IACs */
#define SS_DAC		0x18 /* 2 DACs */
#define SS_DBCR		0x20 /* 3 DBCRs */
#define SS_PID		0x2c /* 3 PIDs */
#define SS_SPRG		0x38 /* 8 SPRGs */
#define SS_IVOR		0x58 /* 20 interrupt vectors */
#define SS_TCR		0xa8
#define SS_BUCSR	0xac
#define SS_L1CSR	0xb0 /* 2 L1CSRs */
#define SS_MSR		0xb8
#define SS_USPRG	0xbc
#define SS_GPREG	0xc0 /* r12-r31 */
#define SS_LR		0x110
#define SS_CR		0x114
#define SS_SP		0x118
#define SS_CURRENT	0x11c
#define SS_IVPR		0x120
#define SS_BPTR		0x124


#define STATE_SAVE_SIZE 0x128

	.section .data
	.align	5
mpc85xx_sleep_save_area:
	.space	STATE_SAVE_SIZE
ccsrbase_low:
	.long	0
ccsrbase_high:
	.long	0
powmgtreq:
	.long	0

	.section .text
	.align	12

	/*
	 * r3 = high word of physical address of CCSR
	 * r4 = low word of physical address of CCSR
	 * r5 = JOG or deep sleep request
	 *      JOG-0x00200000, deep sleep-0x00100000
	 */
_GLOBAL(mpc85xx_enter_deep_sleep)
	lis	r6, ccsrbase_low@ha
	stw	r4, ccsrbase_low@l(r6)
	lis	r6, ccsrbase_high@ha
	stw	r3, ccsrbase_high@l(r6)

	lis	r6, powmgtreq@ha
	stw	r5, powmgtreq@l(r6)

	lis	r10, mpc85xx_sleep_save_area@h
	ori	r10, r10, mpc85xx_sleep_save_area@l

	mfspr	r5, SPRN_HID0
	mfspr	r6, SPRN_HID1

	stw	r5, SS_HID+0(r10)
	stw	r6, SS_HID+4(r10)

	mfspr	r4, SPRN_IAC1
	mfspr	r5, SPRN_IAC2
	mfspr	r6, SPRN_DAC1
	mfspr	r7, SPRN_DAC2

	stw	r4, SS_IAC+0(r10)
	stw	r5, SS_IAC+4(r10)
	stw	r6, SS_DAC+0(r10)
	stw	r7, SS_DAC+4(r10)

	mfspr	r4, SPRN_DBCR0
	mfspr	r5, SPRN_DBCR1
	mfspr	r6, SPRN_DBCR2

	stw	r4, SS_DBCR+0(r10)
	stw	r5, SS_DBCR+4(r10)
	stw	r6, SS_DBCR+8(r10)

	mfspr	r4, SPRN_PID0
	mfspr	r5, SPRN_PID1
	mfspr	r6, SPRN_PID2

	stw	r4, SS_PID+0(r10)
	stw	r5, SS_PID+4(r10)
	stw	r6, SS_PID+8(r10)

	mfspr	r4, SPRN_SPRG0
	mfspr	r5, SPRN_SPRG1
	mfspr	r6, SPRN_SPRG2
	mfspr	r7, SPRN_SPRG3

	stw	r4, SS_SPRG+0x00(r10)
	stw	r5, SS_SPRG+0x04(r10)
	stw	r6, SS_SPRG+0x08(r10)
	stw	r7, SS_SPRG+0x0c(r10)

	mfspr	r4, SPRN_SPRG4
	mfspr	r5, SPRN_SPRG5
	mfspr	r6, SPRN_SPRG6
	mfspr	r7, SPRN_SPRG7

	stw	r4, SS_SPRG+0x10(r10)
	stw	r5, SS_SPRG+0x14(r10)
	stw	r6, SS_SPRG+0x18(r10)
	stw	r7, SS_SPRG+0x1c(r10)

	mfspr	r4, SPRN_IVPR
	stw	r4, SS_IVPR(r10)

	mfspr	r4, SPRN_IVOR0
	mfspr	r5, SPRN_IVOR1
	mfspr	r6, SPRN_IVOR2
	mfspr	r7, SPRN_IVOR3

	stw	r4, SS_IVOR+0x00(r10)
	stw	r5, SS_IVOR+0x04(r10)
	stw	r6, SS_IVOR+0x08(r10)
	stw	r7, SS_IVOR+0x0c(r10)

	mfspr	r4, SPRN_IVOR4
	mfspr	r5, SPRN_IVOR5
	mfspr	r6, SPRN_IVOR6
	mfspr	r7, SPRN_IVOR7

	stw	r4, SS_IVOR+0x10(r10)
	stw	r5, SS_IVOR+0x14(r10)
	stw	r6, SS_IVOR+0x18(r10)
	stw	r7, SS_IVOR+0x1c(r10)

	mfspr	r4, SPRN_IVOR8
	mfspr	r5, SPRN_IVOR9
	mfspr	r6, SPRN_IVOR10
	mfspr	r7, SPRN_IVOR11

	stw	r4, SS_IVOR+0x20(r10)
	stw	r5, SS_IVOR+0x24(r10)
	stw	r6, SS_IVOR+0x28(r10)
	stw	r7, SS_IVOR+0x2c(r10)

	mfspr	r4, SPRN_IVOR12
	mfspr	r5, SPRN_IVOR13
	mfspr	r6, SPRN_IVOR14
	mfspr	r7, SPRN_IVOR15

	stw	r4, SS_IVOR+0x30(r10)
	stw	r5, SS_IVOR+0x34(r10)
	stw	r6, SS_IVOR+0x38(r10)
	stw	r7, SS_IVOR+0x3c(r10)

	mfspr	r4, SPRN_IVOR32
	mfspr	r5, SPRN_IVOR33
	mfspr	r6, SPRN_IVOR34
	mfspr	r7, SPRN_IVOR35

	stw	r4, SS_IVOR+0x40(r10)
	stw	r5, SS_IVOR+0x44(r10)
	stw	r6, SS_IVOR+0x48(r10)
	stw	r7, SS_IVOR+0x4c(r10)

	mfspr	r4, SPRN_TCR
	mfspr	r5, SPRN_BUCSR
	mfspr	r6, SPRN_L1CSR0
	mfspr	r7, SPRN_L1CSR1
	mfspr	r8, SPRN_USPRG0

	stw	r4, SS_TCR(r10)
	stw	r5, SS_BUCSR(r10)
	stw	r6, SS_L1CSR+0(r10)
	stw	r7, SS_L1CSR+4(r10)
	stw	r8, SS_USPRG+0(r10)

	stmw	r12, SS_GPREG(r10)

	mfmsr	r4
	mflr	r5
	mfcr	r6

	stw	r4, SS_MSR(r10)
	stw	r5, SS_LR(r10)
	stw	r6, SS_CR(r10)
	stw	r1, SS_SP(r10)
	stw	r2, SS_CURRENT(r10)

1:	mftbu	r4
	mftb	r5
	mftbu	r6
	cmpw	r4, r6
	bne	1b

	stw	r4, SS_TB+0(r10)
	stw	r5, SS_TB+4(r10)

	lis	r5, ccsrbase_low@ha
	lwz	r4, ccsrbase_low@l(r5)
	lis	r5, ccsrbase_high@ha
	lwz	r3, ccsrbase_high@l(r5)

	/* Disable machine checks and critical exceptions */
	mfmsr	r5
	rlwinm	r5, r5, 0, ~MSR_CE
	rlwinm	r5, r5, 0, ~MSR_ME
	mtmsr	r5
	isync

	/* Use TLB1[15] to map the CCSR at 0xf0000000 */
	lis	r5, 0x100f
	mtspr	SPRN_MAS0, r5
	lis	r5, 0xc000
	ori	r5, r5, 0x0500
	mtspr	SPRN_MAS1, r5
	lis	r5, 0xf000
	ori	r5, r5, 0x000a
	mtspr	SPRN_MAS2, r5
	rlwinm	r5, r4, 0, 0xfffff000
	ori	r5, r5, 0x0005
	mtspr	SPRN_MAS3, r5
	mtspr	SPRN_MAS7, r3
	isync
	tlbwe
	isync

	lis	r3, 0xf000
	lwz	r4, 0x20(r3)
	stw	r4, SS_BPTR(r10)

	lis	r3, 0xf002	/* L2 cache controller at CCSR+0x20000 */
	bl	flush_disable_L2
	bl	__flush_disable_L1

	/* Enable I-cache, so as not to upset the bus
	 * with our loop.
	 */

	mfspr	r4, SPRN_L1CSR1
	ori	r4, r4, 1
	mtspr	SPRN_L1CSR1, r4
	isync

	/* Set boot page translation */
	lis	r3, 0xf000
	lis	r4, (mpc85xx_deep_resume - PAGE_OFFSET)@h
	ori	r4, r4, (mpc85xx_deep_resume - PAGE_OFFSET)@l
	rlwinm	r4, r4, 20, 0x000fffff
	oris	r4, r4, 0x8000
	stw	r4, 0x20(r3)
	lwz	r4, 0x20(r3)		/* read-back to flush write */
	twi	0, r4, 0
	isync

	/* Disable the decrementer */
	mfspr	r4, SPRN_TCR
	rlwinm	r4, r4, 0, ~TCR_DIE
	mtspr	SPRN_TCR, r4

	mfspr	r4, SPRN_TSR
	oris	r4, r4, TSR_DIS@h
	mtspr	SPRN_TSR, r4

	/* set PMRCCR[VRCNT] to wait power stable for 40ms */
	lis	r3, 0xf00e
	lwz	r4, 0x84(r3)
	clrlwi	r4, r4, 16
	oris	r4, r4, 0x12a3
	stw	r4, 0x84(r3)
	lwz	r4, 0x84(r3)

	/* set deep sleep bit in POWMGTSCR */
	lis	r3, powmgtreq@ha
	lwz	r8, powmgtreq@l(r3)

	lis	r3, 0xf00e
	lwz	r4, 0x80(r3)
	or	r4, r4, r8
	stw	r4, 0x80(r3)
	lwz	r4, 0x80(r3)		/* read-back to flush write */
	twi	0, r4, 0
	isync

	mftb	r5
1:	/* spin until either we enter deep sleep, or the sleep process is
	 * aborted due to a pending wakeup event.  Wait some time between
	 * accesses, so we don't flood the bus and prevent the pmc from
	 * detecting an idle system.
	 */

	mftb	r4
	subf	r7, r5, r4
	cmpwi	r7, 1000
	blt	1b
	mr	r5, r4

	lwz	r6, 0x80(r3)
	andis.	r6, r6, 0x0010
	bne	1b
	b	2f

2:	mfspr	r4, SPRN_PIR
	andi.	r4, r4, 1
99:	bne	99b

	/* Establish a temporary 64MB 0->0 mapping in TLB1[1]. */
	lis	r4, 0x1001
	mtspr	SPRN_MAS0, r4
	lis	r4, 0xc000
	ori	r4, r4, 0x0800
	mtspr	SPRN_MAS1, r4
	li	r4, 0
	mtspr	SPRN_MAS2, r4
	li	r4, 0x0015
	mtspr	SPRN_MAS3, r4
	li	r4, 0
	mtspr	SPRN_MAS7, r4
	isync
	tlbwe
	isync

	lis	r3, (3f - PAGE_OFFSET)@h
	ori	r3, r3, (3f - PAGE_OFFSET)@l
	mtctr	r3
	bctr

	/* Locate the resume vector in the last word of the current page. */
	. = mpc85xx_enter_deep_sleep + 0xffc
mpc85xx_deep_resume:
	b	2b

3:
	/* Restore the contents of TLB1[0].  It is assumed that it covers
	 * the currently executing code and the sleep save area, and that
	 * it does not alias our temporary mapping (which is at virtual zero).
	 */
	lis	r3, (TLBCAM - PAGE_OFFSET)@h
	ori	r3, r3, (TLBCAM - PAGE_OFFSET)@l

	lwz	r4, 0(r3)
	lwz	r5, 4(r3)
	lwz	r6, 8(r3)
	lwz	r7, 12(r3)
	lwz	r8, 16(r3)

	mtspr	SPRN_MAS0, r4
	mtspr	SPRN_MAS1, r5
	mtspr	SPRN_MAS2, r6
	mtspr	SPRN_MAS3, r7
	mtspr	SPRN_MAS7, r8

	isync
	tlbwe
	isync

	/* Access the ccsrbase address with TLB1[0] */
	lis	r5, ccsrbase_low@ha
	lwz	r4, ccsrbase_low@l(r5)
	lis	r5, ccsrbase_high@ha
	lwz	r3, ccsrbase_high@l(r5)

	/* Use TLB1[15] to map the CCSR at 0xf0000000 */
	lis	r5, 0x100f
	mtspr	SPRN_MAS0, r5
	lis	r5, 0xc000
	ori	r5, r5, 0x0500
	mtspr	SPRN_MAS1, r5
	lis	r5, 0xf000
	ori	r5, r5, 0x000a
	mtspr	SPRN_MAS2, r5
	rlwinm	r5, r4, 0, 0xfffff000
	ori	r5, r5, 0x0005
	mtspr	SPRN_MAS3, r5
	mtspr	SPRN_MAS7, r3
	isync
	tlbwe
	isync

	lis	r3, 0xf002	/* L2 cache controller at CCSR+0x20000 */
	bl	invalidate_enable_L2

	/* Access the MEM(r10) with TLB1[0] */
	lis	r10, mpc85xx_sleep_save_area@h
	ori	r10, r10, mpc85xx_sleep_save_area@l

	lis	r3, 0xf000
	lwz	r4, SS_BPTR(r10)
	stw	r4, 0x20(r3)		/* restore BPTR */

	/* Program shift running space to PAGE_OFFSET */
	mfmsr	r3
	lis	r4, 1f@h
	ori	r4, r4, 1f@l

	mtsrr1	r3
	mtsrr0	r4
	rfi

1:	/* Restore the rest of TLB1, in ascending order so that
	 * the TLB1[1] gets invalidated first.
	 *
	 * XXX: It's better to invalidate the temporary mapping
	 * TLB1[15] for CCSR before restore any TLB1 entry include 0.
	 */
	lis	r4, 0x100f
	mtspr	SPRN_MAS0, r4
	lis	r4, 0
	mtspr	SPRN_MAS1, r4
	isync
	tlbwe
	isync

	lis	r3, (TLBCAM + 5*4 - 4)@h
	ori	r3, r3, (TLBCAM + 5*4 - 4)@l
	li	r4, 15
	mtctr	r4

2:
	lwz	r5, 4(r3)
	lwz	r6, 8(r3)
	lwz	r7, 12(r3)
	lwz	r8, 16(r3)
	lwzu	r9, 20(r3)

	mtspr	SPRN_MAS0, r5
	mtspr	SPRN_MAS1, r6
	mtspr	SPRN_MAS2, r7
	mtspr	SPRN_MAS3, r8
	mtspr	SPRN_MAS7, r9

	isync
	tlbwe
	isync
	bdnz	2b

	lis	r10, mpc85xx_sleep_save_area@h
	ori	r10, r10, mpc85xx_sleep_save_area@l

	lwz	r5, SS_HID+0(r10)
	lwz	r6, SS_HID+4(r10)

	isync
	mtspr	SPRN_HID0, r5
	isync

	msync
	mtspr	SPRN_HID1, r6
	isync

	lwz	r4, SS_IAC+0(r10)
	lwz	r5, SS_IAC+4(r10)
	lwz	r6, SS_DAC+0(r10)
	lwz	r7, SS_DAC+4(r10)

	mtspr	SPRN_IAC1, r4
	mtspr	SPRN_IAC2, r5
	mtspr	SPRN_DAC1, r6
	mtspr	SPRN_DAC2, r7

	lwz	r4, SS_DBCR+0(r10)
	lwz	r5, SS_DBCR+4(r10)
	lwz	r6, SS_DBCR+8(r10)

	mtspr	SPRN_DBCR0, r4
	mtspr	SPRN_DBCR1, r5
	mtspr	SPRN_DBCR2, r6

	lwz	r4, SS_PID+0(r10)
	lwz	r5, SS_PID+4(r10)
	lwz	r6, SS_PID+8(r10)

	mtspr	SPRN_PID0, r4
	mtspr	SPRN_PID1, r5
	mtspr	SPRN_PID2, r6

	lwz	r4, SS_SPRG+0x00(r10)
	lwz	r5, SS_SPRG+0x04(r10)
	lwz	r6, SS_SPRG+0x08(r10)
	lwz	r7, SS_SPRG+0x0c(r10)

	mtspr	SPRN_SPRG0, r4
	mtspr	SPRN_SPRG1, r5
	mtspr	SPRN_SPRG2, r6
	mtspr	SPRN_SPRG3, r7

	lwz	r4, SS_SPRG+0x10(r10)
	lwz	r5, SS_SPRG+0x14(r10)
	lwz	r6, SS_SPRG+0x18(r10)
	lwz	r7, SS_SPRG+0x1c(r10)

	mtspr	SPRN_SPRG4, r4
	mtspr	SPRN_SPRG5, r5
	mtspr	SPRN_SPRG6, r6
	mtspr	SPRN_SPRG7, r7

	lwz	r4, SS_IVPR(r10)
	mtspr	SPRN_IVPR, r4

	lwz	r4, SS_IVOR+0x00(r10)
	lwz	r5, SS_IVOR+0x04(r10)
	lwz	r6, SS_IVOR+0x08(r10)
	lwz	r7, SS_IVOR+0x0c(r10)

	mtspr	SPRN_IVOR0, r4
	mtspr	SPRN_IVOR1, r5
	mtspr	SPRN_IVOR2, r6
	mtspr	SPRN_IVOR3, r7

	lwz	r4, SS_IVOR+0x10(r10)
	lwz	r5, SS_IVOR+0x14(r10)
	lwz	r6, SS_IVOR+0x18(r10)
	lwz	r7, SS_IVOR+0x1c(r10)

	mtspr	SPRN_IVOR4, r4
	mtspr	SPRN_IVOR5, r5
	mtspr	SPRN_IVOR6, r6
	mtspr	SPRN_IVOR7, r7

	lwz	r4, SS_IVOR+0x20(r10)
	lwz	r5, SS_IVOR+0x24(r10)
	lwz	r6, SS_IVOR+0x28(r10)
	lwz	r7, SS_IVOR+0x2c(r10)

	mtspr	SPRN_IVOR8, r4
	mtspr	SPRN_IVOR9, r5
	mtspr	SPRN_IVOR10, r6
	mtspr	SPRN_IVOR11, r7

	lwz	r4, SS_IVOR+0x30(r10)
	lwz	r5, SS_IVOR+0x34(r10)
	lwz	r6, SS_IVOR+0x38(r10)
	lwz	r7, SS_IVOR+0x3c(r10)

	mtspr	SPRN_IVOR12, r4
	mtspr	SPRN_IVOR13, r5
	mtspr	SPRN_IVOR14, r6
	mtspr	SPRN_IVOR15, r7

	lwz	r4, SS_IVOR+0x40(r10)
	lwz	r5, SS_IVOR+0x44(r10)
	lwz	r6, SS_IVOR+0x48(r10)
	lwz	r7, SS_IVOR+0x4c(r10)

	mtspr	SPRN_IVOR32, r4
	mtspr	SPRN_IVOR33, r5
	mtspr	SPRN_IVOR34, r6
	mtspr	SPRN_IVOR35, r7

	lwz	r4, SS_TCR(r10)
	lwz	r5, SS_BUCSR(r10)
	lwz	r6, SS_L1CSR+0(r10)
	lwz	r7, SS_L1CSR+4(r10)
	lwz	r8, SS_USPRG+0(r10)

	mtspr	SPRN_TCR, r4
	mtspr	SPRN_BUCSR, r5

	msync
	isync
	mtspr	SPRN_L1CSR0, r6
	isync

	mtspr	SPRN_L1CSR1, r7
	isync

	mtspr	SPRN_USPRG0, r8

	lmw	r12, SS_GPREG(r10)

	lwz	r1, SS_SP(r10)
	lwz	r2, SS_CURRENT(r10)
	lwz	r4, SS_MSR(r10)
	lwz	r5, SS_LR(r10)
	lwz	r6, SS_CR(r10)

	msync
	mtmsr	r4
	isync

	mtlr	r5
	mtcr	r6

	li	r4, 0
	mtspr	SPRN_TBWL, r4

	lwz	r4, SS_TB+0(r10)
	lwz	r5, SS_TB+4(r10)

	mtspr	SPRN_TBWU, r4
	mtspr	SPRN_TBWL, r5

	lis	r3, 1
	mtdec	r3

	blr
