/*
 * TI81XX Suspend to Memory
 *
 * This module implements TI814X specific suspend to memory routines
 *
 * Copyright (C) {2011} Texas Instruments Incorporated - http://www.ti.com/
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

#include <linux/linkage.h>
#include <asm/assembler.h>
#include <plat/sram.h>
#include <plat/ti81xx.h>
#include "cm2xxx_3xxx.h"
#include "prm2xxx_3xxx.h"
#include "cm-regbits-81xx.h"
#include "cm81xx.h"

/*
 * DDR PMCR configuration for Self-refresh
 * LP_MOD - Self refresh
 * SR_TIM - 16 ddr cycles lsb = 0x10... for now, use 0
 */
#define ENTER_SELF_REFRESH			0x200

/* Wait time for cke low (~1ms) */
#define CKE_WAIT_LOOP_COUNT			1000

/* DLL wait count - 1s (should be >10ms) */
#define DLL_LOCK_WAIT_COUNT			0x300000

/* EMIF clk Gate reg val */
#define EMIF_DDR_PHY_CLK_GATE_VAL		0x3

/* OSC1_CTRL: disable OSC1 */
#define OSC1_DISABLE				(1 << 3)

/* Offsets from PLL base */
#define DDR_PLL_CONTROL_OFFSET			(DDR_PLL_BASE + ADPLLJ_CLKCTRL)
#define DDR_PLL_STATUS_OFFSET			(DDR_PLL_BASE + ADPLLJ_STATUS)
#define DEEPSLEEP_CONTROL			0x324
#define DEEPSLEEP_STATUS			0x328

/* Offsets from control module base */
#define EMIF_CLK_GATE_OFFSET			0x0694	/* PG 2.x only */
#define OSC1_CTRL				0x046C

/**
 * ti81xx_do_wfi_sync - sync write paths from MPU and enter WFI
 *
 * This routine performs non-posted memory writes in MPU -> DDR and MPU -> L3
 * paths to drain the async bridges in respective paths before entering WFI.
 * This is required to avoid the write pointers in the bridges getting corrupted
 * on master disconnect during WFI pending earlier posted transfers (e.g, DDR
 * writes, device writes, cache flush etc).
 *
 * r0 & r1 are passed word pointers for locations on DDR & L3 mapped as Strongly
 * Ordered (SO) to do non-posted writes on respective paths.
 *
 * Since it *must* be ensured that no posted writes happen on these paths
 * *after* SO writes are performed, following constraints should be considered:
 *	1) No D cache op should happen, it may lead to a L1/L2 line being evicted.
 *		- We do avoid any cache operation for data during the time of SO
 *		write and entering WFI.
 *	2) No I cache op should happen. This is so because the PoU for Cortex A8
 *	is on L2, thus, an instruction fetch can cause miss on L2 and may lead
 *	to a line being evicted causing memory write.
 *		- As seen below, the routine is aligned on 16 instruction (word)
 *		boundary to ensure the code including WFI resides in the same
 *		line and thus not causing cache fill.
 *	3) Page table walk should be prevented after SO writes. This is
 *	necessary since L2 plays role here too and can have miss leading to
 *	eviction.
 *		- The approach used for (2) above automatically ensures this.
 *
 * *** IMPORTANT ***
 * Thus, above points must be honored before changing anything in this routine.
 *
 * Some of the simpler (but costlier) approaches could include:
 *	a) Disable D cache
 *	b) Disable L2 cache
 *	c) Preload code in L2 cache and
 *	d) TLB lockdown
 */
ENTRY(ti81xx_do_wfi_sync)
	.align	6

	stmfd	sp!, {r0-r3, lr}		@ push on stack

	ldr	r2, [r0]
	ldr	r3, [r1]

	dsb					@ ensure sync of memory, cache,
	isb					@ instruction execution etc.

	str	r3, [r1]			@ SO write on L3 path
	str	r2, [r0]			@ SO write on DDR

	wfi

	nop					@ ensure pipeline fill post isb
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop

	ldmfd	sp!, {r0-r3, pc}		@ pop and return
ENDPROC(ti81xx_do_wfi_sync)

/**
 * ti814x_cpu_suspend - Put DDR in self refresh and enter processor
 * idle/DeepSleep.
 *
 * This routine is supposed to be executed from internal RAM and expects the
 * array of base addresses of various registers required to enter idle are
 * passed in r0 _strictly_ in following order:
 * 	1) EMIF0 base virtual address [r0]
 * 	2) EMIF1 base virtual address [r1]
 * 	3) DMM base virtual address [r2]
 * 	4) PLL base virtual address [r3]
 * 	5) Control module base virtual address [r4]
 * 	6) Flag indicating whether to proceed for DeepSleep (non-zero = yes)
 * 	[r5]
 *
 * The code loads these addresses taking r0 value as reference to the array in
 * registers starting from r0, that is, EMIF0 base virtual address goes into r0
 * and so on till r5. Note that we still keep a room till r6 register and use
 * rest in the code for other purpose (r7 to r12).
 *
 * Notes:
 * 	1) Support DeepSleep is untested so far
 *	2) It is assumed that no TLB management is required for this mode. E.g.,
 *	not doing any invalidate operation on TLBs.
 *	3) All targets for load/store in this file during the time DDR is in
 *	self refresh need to be ensured having TLB entries and not lead a TLB
 *	miss.
 *	4) It is assumed that the DeepSleep wakeup signal polatiry configuration
 *	is done outside and *before* this routine and thus, just setting
 *	DSENABLE bit should lead to entry in DeepSleep state.
 *
 *	***DeepSleep support is currently incomplete & untested***
 */

ENTRY(ti814x_cpu_suspend)
	stmfd	sp!, {r0-r12, lr}	@ save registers on stack

	/*
	 * Flush cache - we rely on current implementation of
	 * v7_flush_dcache_all not to destroy r8 (which is the case as of
	 * 2.6.37, else, may need to maintain private stack to restore.
	 *
	 * This also ensures isb.
	 */
	mov	r8, r0			@ same arg list passed to us
	ldr	r0, dcache_flush
	blx	r0

	/* Same as v7_flush_icache_all - saving a branch */
	mov	r0, #0
	mcr	p15, 0, r0, c7, c5, 0		@ I+BTB cache invalidate

	ldm	r8, {r0-r5}		@ gather virtual addresses of cfg regs

	/*
	 * Accessing register space  may result in tlb miss and cause ARM doing
	 * page table walk in ddr. To avoid any such indirect access to DDR when
	 * it is in self refresh (which may result in data loss), we do dummy
	 * loads from, respective bases (assuming we only go within 4KB around
	 * them) to ensure the translations be in TLB.
	 *
	 * Note: Since the DeepSleep is register is from PLL base and we really
	 * treat r5 as value, no dummy load is made.
	 */
	ldr	r8, [r0]
	/* Check if emif1 exists, non zero means presence of emif1 */
	cmp	r1, #0
	ldrne	r8, [r1]
	ldr	r8, [r2]
	ldr	r8, [r3]
	ldr	r8, [r4]

	/* DDR self refresh - NO ACCESS TO DDR FROM THIS POINT */
enter_ddr_self_refresh:
	mov	r8, #ENTER_SELF_REFRESH
	str	r8, [r0, #TI814X_PMCR]		@ EMIF0
	/* Check if emif1 exists, non zero means presence of emif1 */
	cmp	r1, #0
	strne	r8, [r1, #TI814X_PMCR]		@ EMIF1

wait_for_cke_low:
	ldr	r8, [r4, #EMIF_CLK_GATE_OFFSET]
	and	r8, r8, #0xc
	cmp	r8, #0
	bne	wait_for_cke_low

	/*
	 * Once in self refresh, emif should not be accessed so configure dmm
	 * for no emif, only mmr access is allowed
	 *
	 * We need to save DMM configuration context here - have option to use
	 * area in the OCMC RAM but since currently we don't destroy r9-r12,
	 * skipping that part to save time (see block inside #if 0).
	 */
configure_dmm_for_no_emif:
	add	r8, r2, #TI814X_DMM_LISA_MAP_0
	ldm	r8, {r9-r12}

#if 0
	adr	r8, dmm_context
	stm	r8, {r9-r12}
#endif

	mov	r8, #0x0
	str	r8, [r2, #TI814X_DMM_LISA_MAP_0]
	str	r8, [r2, #TI814X_DMM_LISA_MAP_1]
	str	r8, [r2, #TI814X_DMM_LISA_MAP_2]
	str	r8, [r2, #TI814X_DMM_LISA_MAP_3]

	/* Gate DDR PHY 0/1 clocks */
gate_dfi_clocks:
	mov	r8, #EMIF_DDR_PHY_CLK_GATE_VAL
	str	r8, [r4, #EMIF_CLK_GATE_OFFSET]

	/*
	 * Place DDR_PLL in Bypass, to save power
	 * Clearing tinitz drives pll to bypass
	 */
ddr_pll_enter_bypass:
	ldr	r8, [r3, #DDR_PLL_CONTROL_OFFSET]
	bic	r8, r8, #1				@ clear bit 0
	str	r8, [r3, #DDR_PLL_CONTROL_OFFSET]

	/* Wait for Frequency and Phase lock bits to clear */
unlock_loop:
	ldr	r8, [r3, #DDR_PLL_STATUS_OFFSET]
	and	r8, r8, #0x600
	cmp	r8, #0x0
	bne	unlock_loop

ti814x_do_ds_wfi:

	dsb

	/*
	 * XXX: Perhaps it is better to use a branch than all conditional
	 * instructions below. To be analyzed.
	 */

	cmp	r5, #0

	ldrne	r8, [r4, #OSC1_CTRL]
	orrne	r8, r8, #OSC1_DISABLE
	strne	r8, [r4, #OSC1_CTRL]		@ OSC1 disable in DS mode

	ldrne	r8, [r3, #DEEPSLEEP_CONTROL]
	orrne	r8, r8, #TI814X_DEEPSLEEP_CTRL_DSENABLE_MASK

	strne	r8, [r3, #DEEPSLEEP_CONTROL]	@ enter DeepSleep
	ldrne	r8, [r3, #DEEPSLEEP_CONTROL]	@ and readback

	beq	do_wfi

ds_completion_loop:
	ldr   r8, [r3, #DEEPSLEEP_STATUS]
	and   r8, r8, #0x1			@poll for DSCOMPLETE
	cmp   r8, #0x1				@ == 1 if DSCOUNT equals timer
	bne   ds_completion_loop

	cmp	r5, #0				@don't enter wfi if enterd ds
do_wfi:

	wfieq					@ or WFI

	/*
	 * XXX: Ensure sanity before resume path: specifically required for DS
	 * else the DS disable instruction gets executed (see below) and we
	 * never enter DS. None of the isb/dsb/dmb help here.
	 *
	 * To be analyzed the exact constraint. For now, use this as quick fix.
	 */
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop

	/*
	 * ===================================
	 * == Resume path ==
	 * ===================================
	 */

	dsb

	bicne	r8, r8, #TI814X_DEEPSLEEP_CTRL_DSENABLE_MASK
	strne   r8, [r3, #DEEPSLEEP_CONTROL]

	ldrne	r8, [r4, #OSC1_CTRL]
	bicne	r8, r8, #OSC1_DISABLE
	strne	r8, [r4, #OSC1_CTRL]

	/*
	 * Bring pll out of bypass - Setting tinitz starts locking sequence
	 *
	 * DDR_PLL will be locked to the same frequncy as before entering
	 * self-refresh. Clear idle bypass bit (automatically set as part of
	 * TINITZ clear) to ensure it does not go into idle Wait till Frequency
	 * is locked
	 */
ddr_pll_exit_bypass:
	ldr	r8, [r3, #DDR_PLL_CONTROL_OFFSET]
	orr	r8, r8, #1					@ set bit 0
	str	r8, [r3, #DDR_PLL_CONTROL_OFFSET]

clear_idle_bit:
	ldr	r8, [r3, #DDR_PLL_CONTROL_OFFSET]
	bic	r8, r8, #1 << 23				@ clear bit 23
	str	r8, [r3, #DDR_PLL_CONTROL_OFFSET]

	/* Wait till Frequency lock and Phase lock bits are asserted */
lock_loop:
	ldr	r8, [r3, #DDR_PLL_STATUS_OFFSET]
	and	r8, r8, #0x600
	cmp	r8, #0x600
	bne	lock_loop

	/*
	 * Frequency lock bit is asserted as soon as frequency is +/- 1% close
	 * to the target frequency, so when frequency is locked we might have
	 * not achieved final frequency.
	 *
	 * Wait for some time to allow frequency synthesis(this should take some
	 * ns)
	 */
wait_for_pll_synthesis:
	mov	r7, #CKE_WAIT_LOOP_COUNT
pll_loop:
	subs	r7, r7, #0x1
	bne	pll_loop

	/* Ungate DDR pHY 0/1 Clocks */
ungate_dfi_clocks:
	mov	r8, #0x0
	str	r8, [r4, #EMIF_CLK_GATE_OFFSET]

	/* Wait for DLL to lock before configuring dmm */
wait_for_dll_lock:
	mov	r7, #DLL_LOCK_WAIT_COUNT
dll_lock_loop:
	subs	r7, r7, #0x1
	bne	dll_lock_loop

	/*
	 * Restore DMM configuration back to what it was before self-refresh
	 *
	 * We could also save configuration to r9,r10,r11 and r12.
	 */
reconfigure_dmm:
#if 0
	adr	r8, dmm_context
	ldm	r8, {r9-r12}
#endif
	add	r8, r2, #TI814X_DMM_LISA_MAP_0
	stm	r8, {r9-r12}

	/* Prevent ddr going in to self refresh in idle time */
block_self_refresh:
	mov	r8, #0
	str	r8, [r0, #TI814X_PMCR]		@ EMIF0
	cmp	r1, #0
	strne	r8, [r1, #TI814X_PMCR]		@ EMIF1

wait_for_cke_high:
	ldr	r8, [r4, #EMIF_CLK_GATE_OFFSET]
	cmp	r1, #0
	beq	single_emif
	and	r8, r8, #0xc
	cmp	r8, #0xc
	bne	wait_for_cke_high
single_emif:
	and	r8, r8, #0x4
	cmp	r8, #0x4
	bne	wait_for_cke_high

	dsb

	/* DDR ACCESS OK FROM THIS POINT */

	/*
	 *=====================================
	 * Exit Point
	 *=====================================
	 */

restore_regs_pc:
	ldmfd	sp!, {r0-r12, pc}	@ restore regs and return

#if 0
dmm_context:
	.space  4 * 4			@ 4 registers
#endif

dcache_flush:
	.word	v7_flush_dcache_all

/* Suspend function size */
ENTRY(ti814x_cpu_suspend_sz)
	.word	. - ti814x_cpu_suspend
