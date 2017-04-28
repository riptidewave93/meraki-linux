/*
 * APM86xxx suspend support
 *
 * Copyright (c) 2010 Applied Micro Circuits Corporation.
 * Victor Gallardo (vgallardo@apm.com)
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <asm/ppc_asm.h>
#include <asm/reg.h>
#include <asm/cache.h>
#include <asm/page.h>

#include "apm86xxx_suspend.h"
#include "apm86xxx_suspend_lite.h"
#include <asm/apm86xxx_suspend_def.h>
#include <asm/apm_ipp_interface.h>

/*
 *  NOTE: for the following macros
 *	r3 = data pointer
 *	r6 = overwritten
 */
#define SAVE_SPR(addr,reg)	\
	mfspr	r6, reg;	\
	stw	r6, (addr)(r3);

#define RESTORE_SPR(addr, reg)	\
	lwz	r6, (addr)(r3);	\
	mtspr	reg, r6;

#define SAVE_SPRG(addr, reg)	\
	mfsprg	r6, reg;	\
	stw	r6, (addr)(r3);

#define RESTORE_SPRG(addr, reg)	\
	lwz	r6, (addr)(r3);	\
	mtsprg	reg, r6;

#define SAVE_MEMC(lite_addr, memc_addr) \
	sync; \
	lwz	r6, memc_addr(r9); \
	twi	0, r6, 0; \
	isync; \
	stw	r6, lite_addr(r3);

#define RESTORE_MEMC(lite_addr, memc_addr) \
	lwz	r6, lite_addr(r3); \
	sync; \
	stw	r6, memc_addr(r9);

#define GET_MEMC(memc_addr) \
	sync; \
	lwz	r6, memc_addr(r9); \
	twi	0, r6, 0; \
	isync;

#define SET_MEMC(memc_addr) \
	sync; \
	stw	r6, memc_addr(r9);

#define SAVE_SCU(lite_addr, scu_addr) \
	sync; \
	lwz	r6, scu_addr(r8); \
	twi	0, r6, 0; \
	isync; \
	stw	r6, lite_addr(r3);

#define RESTORE_SCU(lite_addr, scu_addr) \
	lwz	r6, lite_addr(r3); \
	sync; \
	stw	r6, scu_addr(r8);

#define GET_SCU(scu_addr) \
	sync; \
	lwz	r6, scu_addr(r8); \
	twi	0, r6, 0; \
	isync;

#define SET_SCU(scu_addr) \
	sync; \
	stw	r6, scu_addr(r8);

.data
.align PAGE_SHIFT

/*
 * memory to save registers
 */
_GLOBAL(apm86xxx_suspend_data)
        .space	SAVE_DATA_SIZE

.text

/*
 * void apm86xxx_suspend_deepsleep(void);
 *	NOTE: r3,r6,r7,r8 are overwritten 
 */
_GLOBAL(apm86xxx_suspend_deepsleep)
apm86xxx_suspend_deepsleep:
	/*
	 * Get pointer to suspend_data
	 */
	lis	r3, apm86xxx_suspend_data@ha
	addi	r3, r3, apm86xxx_suspend_data@l

        /*
         * save GPR Registers 
         */
	stw	r0, SAVE_GPR0(r3)
	stw	r1, SAVE_GPR1(r3)
	stw	r2, SAVE_GPR2(r3)
	stw	r3, SAVE_GPR3(r3)
	stw	r4, SAVE_GPR4(r3)
	stw	r5, SAVE_GPR5(r3)
	stw	r6, SAVE_GPR6(r3)
	stw	r7, SAVE_GPR7(r3)
	stw	r8, SAVE_GPR8(r3)
	stw	r9, SAVE_GPR9(r3)
	stw	r10, SAVE_GPR10(r3)
	stw	r11, SAVE_GPR11(r3)
	stw	r12, SAVE_GPR12(r3)
	stw	r13, SAVE_GPR13(r3)
	stw	r14, SAVE_GPR14(r3)
	stw	r15, SAVE_GPR15(r3)
	stw	r16, SAVE_GPR16(r3)
	stw	r17, SAVE_GPR17(r3)
	stw	r18, SAVE_GPR18(r3)
	stw	r19, SAVE_GPR19(r3)
	stw	r20, SAVE_GPR20(r3)
	stw	r21, SAVE_GPR21(r3)
	stw	r22, SAVE_GPR22(r3)
	stw	r23, SAVE_GPR23(r3)
	stw	r24, SAVE_GPR24(r3)
	stw	r25, SAVE_GPR25(r3)
	stw	r26, SAVE_GPR26(r3)
	stw	r27, SAVE_GPR27(r3)
	stw	r28, SAVE_GPR28(r3)
	stw	r29, SAVE_GPR29(r3)
	stw	r30, SAVE_GPR30(r3)
	stw	r31, SAVE_GPR31(r3)

        /*
         * save CR, LR, MSR
         */
	mfcr	r6
	stw	r6, SAVE_CR(r3)
	mflr	r6
	stw	r6, SAVE_LR(r3)
	mfmsr	r6
	stw	r6, SAVE_MSR(r3)

        /*
         * Save SPRG Registers 
         */
	SAVE_SPRG(SAVE_SPRG0, 0)
	SAVE_SPRG(SAVE_SPRG1, 1)
	SAVE_SPRG(SAVE_SPRG2, 2)
	SAVE_SPRG(SAVE_SPRG3, 3)
	SAVE_SPRG(SAVE_SPRG4, 4)
	SAVE_SPRG(SAVE_SPRG5, 5)
	SAVE_SPRG(SAVE_SPRG6, 6)
	SAVE_SPRG(SAVE_SPRG7, 7)

        /*
         * Save some more registers
         */

	SAVE_SPR(SAVE_CCR0, SPRN_CCR0)
	SAVE_SPR(SAVE_CCR1, SPRN_CCR1)
	SAVE_SPR(SAVE_USPRG0, SPRN_USPRG0)
	SAVE_SPR(SAVE_MMUCR, SPRN_MMUCR)
	SAVE_SPR(SAVE_DEC, SPRN_DEC)
#if 0
	SAVE_SPR(SAVE_TBWL, SPRN_TBWL)
	SAVE_SPR(SAVE_TBWU, SPRN_TBWU)
#else
	mftbu	r6
	stw	r6, SAVE_TBWU(r3)
	mftbl	r6
	stw	r6, SAVE_TBWL(r3)
#endif
	SAVE_SPR(SAVE_TCR, SPRN_TCR)
	SAVE_SPR(SAVE_PID, SPRN_PID)

        /*
         * Save Interrupt Vector's
         */
	SAVE_SPR(SAVE_IVPR, SPRN_IVPR)
	SAVE_SPR(SAVE_IVOR0, SPRN_IVOR0)
	SAVE_SPR(SAVE_IVOR1, SPRN_IVOR1)
	SAVE_SPR(SAVE_IVOR2, SPRN_IVOR2)
	SAVE_SPR(SAVE_IVOR3, SPRN_IVOR3)
	SAVE_SPR(SAVE_IVOR4, SPRN_IVOR4)
	SAVE_SPR(SAVE_IVOR5, SPRN_IVOR5)
	SAVE_SPR(SAVE_IVOR6, SPRN_IVOR6)
	SAVE_SPR(SAVE_IVOR7, SPRN_IVOR7)
	SAVE_SPR(SAVE_IVOR8, SPRN_IVOR8)
	SAVE_SPR(SAVE_IVOR9, SPRN_IVOR9)
	SAVE_SPR(SAVE_IVOR10, SPRN_IVOR10)
	SAVE_SPR(SAVE_IVOR11, SPRN_IVOR11)
	SAVE_SPR(SAVE_IVOR12, SPRN_IVOR12)
	SAVE_SPR(SAVE_IVOR13, SPRN_IVOR13)
	SAVE_SPR(SAVE_IVOR14, SPRN_IVOR14)
	SAVE_SPR(SAVE_IVOR15, SPRN_IVOR15)

        /*
         * Save TLB table
         *   r6 = TLB index
         *   r7 = current address
	 *   r8 = tmp value
         */
        li      r6, PPC44x_TLB_SIZE     /* MAX TLB entries */
        mtctr   r6
        li      r6, 0                   /* index = 0 */
        li      r7, 0
        addi    r7, r3, SAVE_TLB0_TID  /* ptr to tlbentry */
apm86xxx_suspend_deepsleep_save_tlb:
        tlbre	r8, r6, 0
        stw     r8, 4(r7)             /* tlb.word0 */
        mfspr   r8, SPRN_MMUCR
        stw     r8, 0(r7)             /* tlb.tid */
        tlbre	r8, r6, 1
        stw     r8, 8(r7)             /* tlb.word1 */
        tlbre   r8, r6, 2
        stw     r8, 12(r7)            /* tlb.word 2 */
        addi    r7, r7, SAVE_TLB1_TID-SAVE_TLB0_TID
        addi    r6, r6, 1               /* index += 1 */
        bdnz    apm86xxx_suspend_deepsleep_save_tlb

	/* flush data cache and suspend L2 Cache */
#if 1
	mr      r6, r3
	bl	flush_dcache
	isync 
	sync
	bl	l2c_suspend
	mr      r3, r6
#endif
        /* Put processor into Wait State */
        mfmsr   r7
        oris    r6,r7,(MSR_WE|MSR_EE|MSR_CE|MSR_DE)@h
        ori     r6,r6,(MSR_WE|MSR_EE|MSR_CE|MSR_DE)@l
	sync
	isync
        mtmsr   r6
	isync
	sync
#if defined(ISS_MODEL_PWRMGMT_SUPPORT)
	/* 
	 * For ISS DEEPSLEEP, we start back at 0xfffffffc and then
	 * jump back into Linux at apm86xxx_resume_deepsleep
	 *
	 * else we restore and return
	 */
	isync
        lis     r6, apm86xxx_pm_mode@ha
	lwz	r6, apm86xxx_pm_mode@l(r6)
	cmpwi	r6, PWRMODE_DSLEEP
	bne	apm86xxx_suspend_continue
	/* Jump back manually to 0xfffffffc */
        lis     r6,0xffff
        ori     r6,r6,0xfffc
	li	r7,0
        mtspr   SPRN_SRR1,r7
        mtspr   SPRN_SRR0,r6
        rfi
apm86xxx_suspend_continue:
#endif
	/* resume L2 Cache */
	mr      r6, r3
	bl	l2c_resume
	mr      r3, r6
	isync 
        mtmsr   r7
	isync
	lwz	r6, SAVE_LR(r3)
	mtlr	r6
	blr

/*
 * void apm86xxx_resume_deepsleep(void)
 *	NOTE: r6,r7,r8, r9 are overwritten 
 */
_GLOBAL(apm86xxx_resume_deepsleep)
apm86xxx_resume_deepsleep:
	b	apm86xxx_resume_ds
_GLOBAL(ds_dram_magic0)
	.long	IPP_DDR_RESTORE_MAGIC0
_GLOBAL(ds_dram_magic1)
	.long	IPP_DDR_RESTORE_MAGIC1
_GLOBAL(ds_dram_magic2)
	.long	IPP_DDR_RESTORE_MAGIC2
_GLOBAL(ds_dram_magic3)
	.long	IPP_DDR_RESTORE_MAGIC3
apm86xxx_resume_ds:
	/*
	 * Get pointer to suspend_data
	 */
	lis	r3, apm86xxx_suspend_data@ha
	addi	r3, r3, apm86xxx_suspend_data@l

        /*
         * Restore TLB table
         *   r6  = TLB index
         *   r7 = current address
	 *   r8 = tmp1 value
	 *   r9  = tmp2 value
         */
        li      r6, PPC44x_TLB_SIZE     /* MAX TLB entries */
        mtctr   r6
        li      r6, 0                   /* index = 0 */
        li      r7, 0
        addi    r7, r3, SAVE_TLB0_TID  /* ptr to tlbentry */
apm86xxx_resume_deepsleep_restore_tlb:
        lwz     r8, 0(r7)             /* tlb.tid */
	mfspr   r9, SPRN_MMUCR
	rlwimi  r9, r8, 0, 24, 31
	mtspr   SPRN_MMUCR, r9
        lwz     r8, 4(r7)             /* tlb.word0 */
        tlbwe	r8, r6, 0
        lwz     r8, 8(r7)             /* tlb.word1 */
        tlbwe	r8, r6, 1
        lwz     r8, 12(r7)            /* tlb.word 2 */
        tlbwe   r8, r6, 2
        addi    r7, r7, SAVE_TLB1_TID-SAVE_TLB0_TID
        addi    r6, r6, 1             /* index += 1 */
        bdnz    apm86xxx_resume_deepsleep_restore_tlb

        /*
         * Restore Interrupt Vector's
         */
	RESTORE_SPR(SAVE_IVPR, SPRN_IVPR)
	RESTORE_SPR(SAVE_IVOR0, SPRN_IVOR0)
	RESTORE_SPR(SAVE_IVOR1, SPRN_IVOR1)
	RESTORE_SPR(SAVE_IVOR2, SPRN_IVOR2)
	RESTORE_SPR(SAVE_IVOR3, SPRN_IVOR3)
	RESTORE_SPR(SAVE_IVOR4, SPRN_IVOR4)
	RESTORE_SPR(SAVE_IVOR5, SPRN_IVOR5)
	RESTORE_SPR(SAVE_IVOR6, SPRN_IVOR6)
	RESTORE_SPR(SAVE_IVOR7, SPRN_IVOR7)
	RESTORE_SPR(SAVE_IVOR8, SPRN_IVOR8)
	RESTORE_SPR(SAVE_IVOR9, SPRN_IVOR9)
	RESTORE_SPR(SAVE_IVOR10, SPRN_IVOR10)
	RESTORE_SPR(SAVE_IVOR11, SPRN_IVOR11)
	RESTORE_SPR(SAVE_IVOR12, SPRN_IVOR12)
	RESTORE_SPR(SAVE_IVOR13, SPRN_IVOR13)
	RESTORE_SPR(SAVE_IVOR14, SPRN_IVOR14)
	RESTORE_SPR(SAVE_IVOR15, SPRN_IVOR15)

        /*
         * Restore Some more registers
         */
	RESTORE_SPR(SAVE_CCR0, SPRN_CCR0)
	RESTORE_SPR(SAVE_CCR1, SPRN_CCR1)
	RESTORE_SPR(SAVE_USPRG0, SPRN_USPRG0)
	RESTORE_SPR(SAVE_MMUCR, SPRN_MMUCR)
	RESTORE_SPR(SAVE_TBWU, SPRN_TBWU)
	RESTORE_SPR(SAVE_TBWL, SPRN_TBWL)
	RESTORE_SPR(SAVE_DEC, SPRN_DEC)
	RESTORE_SPR(SAVE_TCR, SPRN_TCR)
	RESTORE_SPR(SAVE_PID, SPRN_PID)

        /*
         * Restore SPRG Registers 
         */
	RESTORE_SPRG(SAVE_SPRG0, 0)
	RESTORE_SPRG(SAVE_SPRG1, 1)
	RESTORE_SPRG(SAVE_SPRG2, 2)
	RESTORE_SPRG(SAVE_SPRG3, 3)
	RESTORE_SPRG(SAVE_SPRG4, 4)
	RESTORE_SPRG(SAVE_SPRG5, 5)
	RESTORE_SPRG(SAVE_SPRG6, 6)
	RESTORE_SPRG(SAVE_SPRG7, 7)

        /*
         * Restore LR and MSR via SRR0 and SRR1
         */
	lwz	r6, SAVE_LR(r3)
	mtlr	r6
	mtspr	SPRN_SRR0, r6
	lwz	r6, SAVE_MSR(r3)
	mtspr	SPRN_SRR1, r6

	/*
	 * Remember that we resumed from deepsleep
	 */
	li	r4, 1
	lis	r5, apm86xxx_pm_resumed_from_deepsleep@ha
	stw	r4, apm86xxx_pm_resumed_from_deepsleep@l(r5)

        /*
         * Restore GPR Registers 
         */
	lwz	r0, SAVE_GPR0(r3)
	lwz	r1, SAVE_GPR1(r3)
	lwz	r2, SAVE_GPR2(r3)
	lwz	r3, SAVE_GPR3(r3) /* ALREAD SET */
	lwz	r4, SAVE_GPR4(r3)
	lwz	r5, SAVE_GPR5(r3)
	lwz	r6, SAVE_GPR6(r3)
	lwz	r7, SAVE_GPR7(r3)
	lwz	r8, SAVE_GPR8(r3)
	lwz	r9, SAVE_GPR9(r3)
	lwz	r10, SAVE_GPR10(r3)
	lwz	r11, SAVE_GPR11(r3)
	lwz	r12, SAVE_GPR12(r3)
	lwz	r13, SAVE_GPR13(r3)
	lwz	r14, SAVE_GPR14(r3)
	lwz	r15, SAVE_GPR15(r3)
	lwz	r16, SAVE_GPR16(r3)
	lwz	r17, SAVE_GPR17(r3)
	lwz	r18, SAVE_GPR18(r3)
	lwz	r19, SAVE_GPR19(r3)
	lwz	r20, SAVE_GPR20(r3)
	lwz	r21, SAVE_GPR21(r3)
	lwz	r22, SAVE_GPR22(r3)
	lwz	r23, SAVE_GPR23(r3)
	lwz	r24, SAVE_GPR24(r3)
	lwz	r25, SAVE_GPR25(r3)
	lwz	r26, SAVE_GPR26(r3)
	lwz	r27, SAVE_GPR27(r3)
	lwz	r28, SAVE_GPR28(r3)
	lwz	r29, SAVE_GPR29(r3)
	lwz	r30, SAVE_GPR30(r3)
	lwz	r31, SAVE_GPR31(r3)

	/* flush data cache and resume L2 Cache */
#ifdef CONFIG_SMP
	bl	flush_dcache
	bl	l2c_resume
#endif
        rfi	/* return */

/*
 * apm86xxx_lite_suspend(unsigned long *data);
 *
 * NOTE: r3 => OCM data pointer
 */
_GLOBAL(apm86xxx_lite_suspend)
	/* save LR */
	mflr	r6
	stw	r6, LITE_LR(r3)

	/* save off current interrupt vector base and extint offset */
	SAVE_SPR(LITE_SPRN_IVPR, SPRN_IVPR)
	SAVE_SPR(LITE_SPRN_IVOR4, SPRN_IVOR4)

	/* establish new interrupt vector base and extint offset */
	lwz	r6, LITE_RESUME(r3)
	mtspr	SPRN_IVPR,r6
	mtspr	SPRN_IVOR4,r6

	/* get pointer to MEMC registers */
	lwz	r9, LITE_MEMC(r3)

	/* get pointer to SCU registers */
	lwz	r8, LITE_SCU(r3)

	/* put DDR in self-refresh */
	GET_MEMC(MEMC_REG_DDRC_0)
	oris	r6, r6, REG_DDRC_0_SELFREFRESH@h
	SET_MEMC(MEMC_REG_DDRC_0)

	/* wait for DDR to enter self-refresh mode */
	lis	r5, DDRC_REG_DBG_MODE_SELFREFRESH@h
	ori	r5, r5, DDRC_REG_DBG_MODE_SELFREFRESH@l
wait_for_selfrefresh:
	GET_MEMC(MEMC_DDRC_REG_DBG)
	and	r6, r6, r5
	cmpwi	r6, DDRC_REG_DBG_MODE_SELFREFRESH
	bne	wait_for_selfrefresh

	/* Save DDRC IOs */
	SAVE_MEMC(LITE_MEMC_REG_DDRC_0, MEMC_REG_DDRC_0)
	SAVE_MEMC(LITE_MEMC_REG_PHY_1, MEMC_REG_PHY_1)
	SAVE_MEMC(LITE_MEMC_DDR_IO_CONTROL, MEMC_DDR_IO_CONTROL)
	SAVE_MEMC(LITE_MEMC_DDRC_MISC, MEMC_DDRC_MISC)

	/* Put DDRC IOs in LP mode */
	GET_MEMC(MEMC_REG_PHY_1)
	lis	r5, ~(DIF_ON1_MASK|DIF_OFF1_MASK)@h
	ori	r5, r5, ~(DIF_ON1_MASK|DIF_OFF1_MASK)@l
	and	r6, r6, r5
	SET_MEMC(MEMC_REG_PHY_1)

	GET_MEMC(MEMC_REG_DDRC_0)
	lis	r5, ~(RMODE0_MASK|PHY_SOFT_RSTB0_MASK|ACTIVE_RANKS0_MASK)@h
	ori	r5, r5, ~(RMODE0_MASK|PHY_SOFT_RSTB0_MASK|ACTIVE_RANKS0_MASK)@l
	and	r6, r6, r5
	SET_MEMC(MEMC_REG_DDRC_0)

	GET_MEMC(MEMC_DDR_IO_CONTROL)
	lis	r5, ~(RF_IO_DRVRCNTR_MASK|RF_IO_DRVRCNTR_ADD_MASK| \
			RF_IO_SR_MASK|RF_IO_SR_ADD_MASK|RF_IO_PECNTR_MASK| \
			RF_IO_CROWBAREN_MASK|RF_IO_RMODE_31_0_MASK| \
			RF_IO_RMODE_63_32_MASK|RF_IO_DDR2_MODE_MASK| \
			RF_IO_ODT_MODE_MASK)@h
	ori	r5, r5, ~(RF_IO_DRVRCNTR_MASK|RF_IO_DRVRCNTR_ADD_MASK| \
			RF_IO_SR_MASK|RF_IO_SR_ADD_MASK|RF_IO_PECNTR_MASK| \
			RF_IO_CROWBAREN_MASK|RF_IO_RMODE_31_0_MASK| \
			RF_IO_RMODE_63_32_MASK|RF_IO_DDR2_MODE_MASK| \
			RF_IO_ODT_MODE_MASK)@l
	and	r6, r6, r5
	oris	r6, r6, (RF_IO_POWER_DOWN_MODE_MASK|RF_IO_PWSAVE_MODE_MASK)@h
	ori	r6, r6, (RF_IO_POWER_DOWN_MODE_MASK|RF_IO_PWSAVE_MODE_MASK)@l
	oris	r6, r6, (0x2<<RF_IO_ODT_MODE_SHIFT)@h
	ori	r6, r6, (0x2<<RF_IO_ODT_MODE_SHIFT)@l
	SET_MEMC(MEMC_DDR_IO_CONTROL)

	GET_MEMC(MEMC_DDRC_MISC)
	lis	r5, ~(RF_IO_PEMPHEN_MASK|RF_IO_TEN_MASK)@h
	ori	r5, r5, ~(RF_IO_PEMPHEN_MASK|RF_IO_TEN_MASK)@l
	and	r6, r6, r5
	SET_MEMC(MEMC_DDRC_MISC)
#if 0
	/* Power down DDR PLL */
	GET_SCU(SCU_SOCPLL2)
	oris	r6, r6, (SOCPLL2_RESET)@h
	ori 	r6, r6, (SOCPLL2_RESET)@l
	SET_SCU(SCU_SOCPLL2)
	GET_SCU(SCU_SOCPLL2)
	oris	r6, r6, (SOCPLL2_PWRDN)@h
	ori 	r6, r6, (SOCPLL2_PWRDN)@l
	SET_SCU(SCU_SOCPLL2)
#endif
#if 0
	/* Save DDR divisor */
	SAVE_SCU(LITE_SCU_SOCDIV1, SCU_SOCDIV1)

	/* Scale DDR divisor to lowest val */
	GET_SCU(SCU_SOCDIV1)
	oris	r6, r6, (SOCDIV1_DDR2X_CLK_FREQ_SEL1)@h
	ori 	r6, r6, (SOCDIV1_DDR2X_CLK_FREQ_SEL1)@l
	SET_SCU(SCU_SOCDIV1)
#endif
        /* Put processor into Wait State */
        mfmsr	r7
        oris	r6,r7,(MSR_WE|MSR_EE|MSR_CE|MSR_DE)@h
        ori	r6,r6,(MSR_WE|MSR_EE|MSR_CE|MSR_DE)@l
	sync
	isync
        mtmsr	r6
	isync

	/* restore MSR */
        mtmsr   r7
	isync

	/* restore LR */
	lwz	r6, LITE_LR(r3)
	mtlr	r6

	/* return */
	blr

_GLOBAL(apm86xxx_lite_suspend_size)
        .long $-apm86xxx_lite_suspend

/*
 * apm86xxx_lite_resume(unsigned long *data);
 *
 * NOTE: r3 => OCM data pointer
 */
_GLOBAL(apm86xxx_lite_resume)
#if 0
	/* Restore DDR divisor */
	RESTORE_SCU(LITE_SCU_SOCDIV1, SCU_SOCDIV1)
#endif
#if 0
	/* Power up DDR PLL */
	GET_SCU(SCU_SOCPLL2)
	lis	r5, ~SOCPLL2_PWRDN@h
	ori	r5, r5, ~SOCPLL2_PWRDN@l
	and	r6, r6, r5
	SET_SCU(SCU_SOCPLL2)

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

	GET_SCU(SCU_SOCPLL2)
	lis	r5, ~SOCPLL2_RESET@h
	ori	r5, r5, ~SOCPLL2_RESET@l
	and	r6, r6, r5
	SET_SCU(SCU_SOCPLL2)

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
#endif
	/* Restore DDRC IOs */
	RESTORE_MEMC(LITE_MEMC_REG_DDRC_0, MEMC_REG_DDRC_0)
	RESTORE_MEMC(LITE_MEMC_REG_PHY_1, MEMC_REG_PHY_1)
	RESTORE_MEMC(LITE_MEMC_DDR_IO_CONTROL, MEMC_DDR_IO_CONTROL)
	RESTORE_MEMC(LITE_MEMC_DDRC_MISC, MEMC_DDRC_MISC)

	/* take DDR out of self-refresh */
	GET_MEMC(MEMC_REG_DDRC_0)
	lis	r5, ~REG_DDRC_0_SELFREFRESH@h
	ori	r5, r5, ~REG_DDRC_0_SELFREFRESH@l
	and	r6, r6, r5
	SET_MEMC(MEMC_REG_DDRC_0)

	/* wait for DDR to enter normal mode */
	lis	r5, DDRC_REG_DBG_MODE_NORMAL@h
	ori	r5, r5, DDRC_REG_DBG_MODE_NORMAL@l
wait_for_normal:
	GET_MEMC(MEMC_DDRC_REG_DBG)
	and	r6, r6, r5
	cmpwi	r6, DDRC_REG_DBG_MODE_NORMAL
	bne	wait_for_normal

        /* restored saved off interrupt vector base and extint offset */
        RESTORE_SPR(LITE_SPRN_IVPR, SPRN_IVPR)
        RESTORE_SPR(LITE_SPRN_IVOR4, SPRN_IVOR4)

	/* return from interrupt */
	rfi

_GLOBAL(apm86xxx_lite_resume_size)
        .long $-apm86xxx_lite_resume
