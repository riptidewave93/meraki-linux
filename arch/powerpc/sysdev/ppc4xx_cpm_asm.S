/*
 * arch/powerpc/sysdev/ppc4xx_cpm_asm.S
 *
 * PowerPC 4xx Clock and Power Management
 *
 * (C) Copyright 2009, Applied Micro Circuits Corporation
 * Victor Gallardo (vgallardo@amcc.com)
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

#include <asm/reg.h>
#include <asm/dcr-regs.h>
#include <asm/mmio-regs.h>
#include <asm/ppc_asm.h>
#include <asm/ppc4xx_cpm.h>

/*
 *  NOTE: for the following macros
 *        r6 overwritten, r4 is data pointer
 */
#define SAVE_SPR(addr,reg)		\
	mfspr	r6, reg;		\
	stw	r6, ((addr)*4)(r4);

#define RESTORE_SPR(addr, reg)		\
	lwz	r6, ((addr)*4)(r4);	\
	mtspr	reg, r6;

#define SAVE_DCR(addr, reg)		\
	mfdcr	r6, reg;		\
	stw	r6, ((addr)*4)(r4);

#define RESTORE_DCR(addr, reg)		\
	lwz	r6, ((addr)*4)(r4);	\
	mtdcr	reg, r6;

#define SAVE_IDCR(addr, base, reg)			\
        li	r6, reg;				\
        mtdcr	DCRN_ ## base ## _CONFIG_ADDR, r6;	\
        mfdcr	r6, DCRN_ ## base ## _CONFIG_DATA;	\
	stw     r6, ((addr)*4)(r4);

#define RESTORE_IDCR(addr, base, reg)			\
        li	r6, reg;				\
        mtdcr	DCRN_ ## base ## _CONFIG_ADDR, r6;	\
	lwz     r6, ((addr)*4)(r4);			\
        mtdcr	DCRN_ ## base ## _CONFIG_DATA, r6;

/*
 *  NOTE: for the following macros
 *        r6 overwritten, r7 has value
 */
#define GET_IDCR(base, reg)				\
        li	r6, reg;				\
        mtdcr	DCRN_ ## base ## _CONFIG_ADDR, r6;	\
        mfdcr	r7, DCRN_ ## base ## _CONFIG_DATA;

#define SET_IDCR(base, reg)				\
        li	r6, reg;				\
        mtdcr	DCRN_ ## base ## _CONFIG_ADDR, r6;	\
        mtdcr	DCRN_ ## base ## _CONFIG_DATA, r7;


/*
 *  NOTE: helper function to get saved data offset
 */
#define GET_DATA(addr)	((addr)*4)(r4);

.text

/*
 * cpm_suspend_mem(int pm_mode, unsigned long *data, void *resume)
 *
 * NOTE: r3 => pm_mode
 *       r4 => data pointer
 *       r5 => resume
 */
_GLOBAL(cpm_suspend_mem)

 	/* save off current interrupt vector base and extint offset */
 	SAVE_SPR(0,  SPRN_IVPR)
 	SAVE_SPR(1,  SPRN_IVOR4)

	/* establish new interrupt vector base and extint offset */
 	mtspr	SPRN_IVPR,r5
 	mtspr	SPRN_IVOR4,r5

#if defined(CONFIG_460EX) || defined(CONFIG_460GT) \
	|| defined(CONFIG_APM82181) || defined(CONFIG_APM82161)

#if defined(CONFIG_APM82181) || defined(CONFIG_APM82161)

	SAVE_IDCR(2, SDRAM0, SDRAM0_BESR)
	SAVE_IDCR(3, SDRAM0, SDRAM0_BEARL)
	SAVE_IDCR(4, SDRAM0, SDRAM0_BEARH)
	SAVE_IDCR(5, SDRAM0, SDRAM0_WMIRQ)
	SAVE_IDCR(6, SDRAM0, SDRAM0_PLBOPT)
	SAVE_IDCR(7, SDRAM0, SDRAM0_PUABA)
	
	
	SAVE_IDCR(11, SDRAM0, SDRAM0_MCOPT2)
	SAVE_IDCR(12, SDRAM0, SDRAM0_MCOPT1)
	SAVE_IDCR(13, SDRAM0, SDRAM0_MODT0)
	SAVE_IDCR(14, SDRAM0, SDRAM0_MODT1)
	SAVE_IDCR(17, SDRAM0, SDRAM0_CODT)
	SAVE_IDCR(18, SDRAM0, SDRAM0_RTR)
	SAVE_IDCR(19, SDRAM0, SDRAM0_MB0CF)
	SAVE_IDCR(20, SDRAM0, SDRAM0_MB1CF)

	SAVE_IDCR(23, SDRAM0, SDRAM0_INITPLR0)
	SAVE_IDCR(24, SDRAM0, SDRAM0_INITPLR1)
	SAVE_IDCR(25, SDRAM0, SDRAM0_INITPLR2)
	SAVE_IDCR(26, SDRAM0, SDRAM0_INITPLR3)
	SAVE_IDCR(27, SDRAM0, SDRAM0_INITPLR4)
	SAVE_IDCR(28, SDRAM0, SDRAM0_INITPLR5)
	SAVE_IDCR(29, SDRAM0, SDRAM0_INITPLR6)
	SAVE_IDCR(30, SDRAM0, SDRAM0_INITPLR7)
	SAVE_IDCR(31, SDRAM0, SDRAM0_INITPLR8)
	SAVE_IDCR(32, SDRAM0, SDRAM0_INITPLR9)
	SAVE_IDCR(33, SDRAM0, SDRAM0_INITPLR10)
	SAVE_IDCR(34, SDRAM0, SDRAM0_INITPLR11)
	SAVE_IDCR(35, SDRAM0, SDRAM0_INITPLR12)
	SAVE_IDCR(36, SDRAM0, SDRAM0_INITPLR13)
	SAVE_IDCR(37, SDRAM0, SDRAM0_INITPLR14)
	SAVE_IDCR(38, SDRAM0, SDRAM0_INITPLR15)

	SAVE_IDCR(39, SDRAM0, SDRAM0_RQDC)
	SAVE_IDCR(40, SDRAM0, SDRAM0_RFDC)
	SAVE_IDCR(41, SDRAM0, SDRAM0_RDCC)
	SAVE_IDCR(42, SDRAM0, SDRAM0_DLCR)
	SAVE_IDCR(43, SDRAM0, SDRAM0_CLKTR)
	SAVE_IDCR(44, SDRAM0, SDRAM0_WRDTR)
	SAVE_IDCR(45, SDRAM0, SDRAM0_SDTR1)
	SAVE_IDCR(46, SDRAM0, SDRAM0_SDTR2)
	SAVE_IDCR(47, SDRAM0, SDRAM0_SDTR3)
	SAVE_IDCR(48, SDRAM0, SDRAM0_MMODE)
	SAVE_IDCR(49, SDRAM0, SDRAM0_MEMODE)
	SAVE_IDCR(50, SDRAM0, SDRAM0_ECCES)

	/* save off some CPR registers */
	SAVE_IDCR(51, CPR0, CPR0_PLB2D)
	SAVE_IDCR(52, CPR0, CPR0_OPBD)
	SAVE_IDCR(53, CPR0, CPR0_DDR2D)

#else
	/* save off Memory Queue registers */
	SAVE_DCR(2,  MQ0_B0BASE)
	SAVE_DCR(3,  MQ0_B1BASE)
	SAVE_DCR(4,  MQ0_B2BASE)
	SAVE_DCR(5,  MQ0_B3BASE)
	SAVE_DCR(6,  MQ0_CF1H)
	SAVE_DCR(7,  MQ0_BAUL)
	SAVE_DCR(8,  MQ0_CF1L)
	SAVE_DCR(9,  MQ0_CFBHL)
	SAVE_DCR(10, MQ0_BAUH)

	/* save off DDR Controller registers */
	SAVE_IDCR(11, MCIF0, MCIF0_MCOPT2)
	SAVE_IDCR(12, MCIF0, MCIF0_MCOPT1)
	SAVE_IDCR(13, MCIF0, MCIF0_MODT0)
	SAVE_IDCR(14, MCIF0, MCIF0_MODT1)
	SAVE_IDCR(15, MCIF0, MCIF0_MODT2)
	SAVE_IDCR(16, MCIF0, MCIF0_MODT3)
	SAVE_IDCR(17, MCIF0, MCIF0_CODT)
	SAVE_IDCR(18, MCIF0, MCIF0_RTR)
	SAVE_IDCR(19, MCIF0, MCIF0_MB0CF)
	SAVE_IDCR(20, MCIF0, MCIF0_MB1CF)
	SAVE_IDCR(21, MCIF0, MCIF0_MB2CF)
	SAVE_IDCR(22, MCIF0, MCIF0_MB3CF)

	SAVE_IDCR(23, MCIF0, MCIF0_INITPLR0)
	SAVE_IDCR(24, MCIF0, MCIF0_INITPLR1)
	SAVE_IDCR(25, MCIF0, MCIF0_INITPLR2)
	SAVE_IDCR(26, MCIF0, MCIF0_INITPLR3)
	SAVE_IDCR(27, MCIF0, MCIF0_INITPLR4)
	SAVE_IDCR(28, MCIF0, MCIF0_INITPLR5)
	SAVE_IDCR(29, MCIF0, MCIF0_INITPLR6)
	SAVE_IDCR(30, MCIF0, MCIF0_INITPLR7)
	SAVE_IDCR(31, MCIF0, MCIF0_INITPLR8)
	SAVE_IDCR(32, MCIF0, MCIF0_INITPLR9)
	SAVE_IDCR(33, MCIF0, MCIF0_INITPLR10)
	SAVE_IDCR(34, MCIF0, MCIF0_INITPLR11)
	SAVE_IDCR(35, MCIF0, MCIF0_INITPLR12)
	SAVE_IDCR(36, MCIF0, MCIF0_INITPLR13)
	SAVE_IDCR(37, MCIF0, MCIF0_INITPLR14)
	SAVE_IDCR(38, MCIF0, MCIF0_INITPLR15)

	SAVE_IDCR(39, MCIF0, MCIF0_RQDC)
	SAVE_IDCR(40, MCIF0, MCIF0_RFDC)
	SAVE_IDCR(41, MCIF0, MCIF0_RCDC)
	SAVE_IDCR(42, MCIF0, MCIF0_DLCR)
	SAVE_IDCR(43, MCIF0, MCIF0_CLKTR)
	SAVE_IDCR(44, MCIF0, MCIF0_WRDTR)
	SAVE_IDCR(45, MCIF0, MCIF0_SDTR1)
	SAVE_IDCR(46, MCIF0, MCIF0_SDTR2)
	SAVE_IDCR(47, MCIF0, MCIF0_SDTR3)
	SAVE_IDCR(48, MCIF0, MCIF0_MMODE)
	SAVE_IDCR(49, MCIF0, MCIF0_MEMODE)
	SAVE_IDCR(50, MCIF0, MCIF0_ECCES)

	/* save off some CPR registers */
	SAVE_IDCR(51, CPR0, CPR0_PLBED)
	SAVE_IDCR(52, CPR0, CPR0_OPBD)
	SAVE_IDCR(53, CPR0, CPR0_AHBD)
#endif /*defined(CONFIG_APM82181) || defined(CONFIG_APM82161)*/
	
	

#if defined(CONFIG_APM82181) || defined(CONFIG_APM82161)
	/* Put DDR in self refresh */
	GET_IDCR(SDRAM0, SDRAM0_MCOPT2)
	lis r6, SDRAM0_MCOPT2_SREN@h
	ori r6, r6, SDRAM0_MCOPT2_SREN@l
	or  r7, r7, r6
	SET_IDCR(SDRAM0, SDRAM0_MCOPT2)
chk_self_refresh:
	GET_IDCR(SDRAM0, SDRAM0_MCSTAT)
	lis r6, SDRAM0_MCSTAT_SRMS@h
	ori r6, r6, SDRAM0_MCSTAT_SRMS@l
	and r7, r7, r6
	cmpwi   r7, 0
	beq     chk_self_refresh
#else
        /* Put DDR in self refresh */
	GET_IDCR(MCIF0, MCIF0_MCOPT2)
	lis r6, MCIF0_MCOPT2_SREN@h
	ori r6, r6, MCIF0_MCOPT2_SREN@l
	or  r7, r7, r6
	SET_IDCR(MCIF0, MCIF0_MCOPT2)
chk_self_refresh:
	GET_IDCR(MCIF0, MCIF0_MCSTAT)
	lis r6, MCIF0_MCSTAT_SRMS@h
	ori r6, r6, MCIF0_MCSTAT_SRMS@l
	and r7, r7, r6
	cmpwi   r7, 0
	beq     chk_self_refresh
#endif /*defined(CONFIG_APM82181) || defined(CONFIG_APM82161)*/

	/* Put DDR and L2C in soft reset */
	GET_IDCR(SDR0, SDR0_SRST0)
	lis   r6, (SDR0_SRST0_DMC | SDR0_SRST0_L2C)@h
	ori   r6, r6, (SDR0_SRST0_DMC | SDR0_SRST0_L2C)@l
	or    r7, r7, r6
	SET_IDCR(SDR0, SDR0_SRST0)

	/* Put PLL in bypass mode */
	GET_IDCR(CPR0, CPR0_PLLC)
	lis r6, ~CPR0_PLLC_ENG@h
	ori r6, r6, ~CPR0_PLLC_ENG@l
	and r7, r7, r6
	SET_IDCR(CPR0, CPR0_PLLC)

	/* Configure OPB and EBC clock to same freq as PLB clock - 33MHz */
#if defined(CONFIG_APM82181) || defined(CONFIG_APM82161)
	lis r7, CPR0_PLB2D_DIV1@h
	ori r7, r7, CPR0_PLB2D_DIV1@l
	SET_IDCR(CPR0, CPR0_PLB2D)
	lis r7, CPR0_DDR2D_DIV1@h
	ori r7, r7, CPR0_DDR2D_DIV1@l
	SET_IDCR(CPR0, CPR0_DDR2D)
	lis r7, CPR0_OPBD_DIV1@h
	ori r7, r7, CPR0_OPBD_DIV1@l
	SET_IDCR(CPR0, CPR0_OPBD)
#else
	lis r7, CPR0_PLBED_DIV2@h
	ori r7, r7, CPR0_PLBED_DIV2@l
	SET_IDCR(CPR0, CPR0_PLBED)
	lis r7, CPR0_OPBD_DIV1@h
	ori r7, r7, CPR0_OPBD_DIV1@l
	SET_IDCR(CPR0, CPR0_OPBD)
	lis r7, CPR0_AHBD_DIV1@h
	ori r7, r7, CPR0_AHBD_DIV1@l
	SET_IDCR(CPR0, CPR0_AHBD)
#endif
	/* Update PLL now */
	lis r7, CPR0_CLKUPD_CUD@h
	ori r7, r7, CPR0_CLKUPD_CUD@l
	SET_IDCR(CPR0, CPR0_CLKUPD)

#endif /* defined(CONFIG_460EX) || defined(CONFIG_460GT) || defined(CONFIG_APM82181)\
		defined(CONFIG_APM82161)*/

	/* Reduce voltage from 1.2v to 1.0v (in steps of 50mv) */
	cmpwi   r3, CPM_PM_DEEPSLEEP 
	bne     cpm_suspend_mem_wait
	
#if defined(CONFIG_CPM_PM_AD5243)
        /* IIC voltage adjust*/
	lwz	r7, GET_DATA(CPM_PM_DATA_IIC_PTR)
	li	r19,6
voltage_config:
        cmpwi   r19,6
        bne     voltage_config_1
        li      r18,CPM_PM_AD5243_1_250V
        b       voltage_init
voltage_config_1:
        cmpwi   r19,5
        bne     voltage_config_2
        li      r18,CPM_PM_AD5243_1_200V
        b       voltage_init
voltage_config_2:
        cmpwi   r19,4
        bne     voltage_config_3
        li      r18,CPM_PM_AD5243_1_150V
        b       voltage_init
voltage_config_3:
        cmpwi   r19,3
        bne     voltage_config_4
        li      r18,CPM_PM_AD5243_1_100V
        b       voltage_init
voltage_config_4:
        cmpwi   r19,2
        bne     voltage_config_5
        li      r18,CPM_PM_AD5243_1_050V
        b       voltage_init
voltage_config_5:
        cmpwi   r19,1
        bne     voltage_exit
        li      r18,CPM_PM_AD5243_1_000V

voltage_init:
        li      r14,IIC_STS_SCMP
        stb     r14,IIC_STS(r7)
        eieio

        li      r15,12
voltage_init_1:
        sync
        lbz     r14,IIC_STS(r7)
        isync
        andi.   r14,r14,IIC_STS_PT
        cmpwi   r14,IIC_STS_PT
        bne     voltage_init_2
        addi    r15,r15,-1
        cmpwi   r15,0
        beq     voltage_end
        b       voltage_init_1
voltage_init_2:
        sync
        lbz     r14,IIC_MDCNTL(r7)
        isync
        ori     r14,r14,(IIC_MDCNTL_FMDB|IIC_MDCNTL_FSDB)
        stb     r14,IIC_MDCNTL(r7)
        eieio

        li      r14,0
        stb     r14,IIC_HMADR(r7)
        eieio
        li      r14,CPM_PM_AD5243_ADDR
        stb     r14,IIC_LMADR(r7)
        eieio

        li      r14,1
        stb     r14,IIC_MDBUF(r7)
        eieio
        li      r14,(IIC_CNTL_PT|IIC_CNTL_CHT)
        stb     r14,IIC_CNTL(r7)
        eieio

        li      r15,40
voltage_init_3:
        li      r17,256
voltage_init_3_delay:
        addi    r17,r17,-1
        cmpwi   r17,0
        bne     voltage_init_3_delay
        sync
        lbz     r14,IIC_STS(r7)
        isync
        andi.   r16,r14,IIC_STS_PT
        cmpwi   r16,IIC_STS_PT
        bne     voltage_init_4
        andi.   r16,r14,IIC_STS_ERR
        cmpwi   r16,IIC_STS_ERR
        beq     voltage_end
        addi    r15,r15,-1
        cmpwi   r15,0
        bne     voltage_init_3
voltage_init_4:
        stb     r18,IIC_MDBUF(r7)
        eieio
        li      r14,IIC_CNTL_PT
        stb     r14,IIC_CNTL(r7)
        eieio

        li      r15,40
voltage_init_5:
        li      r17,256
voltage_init_5_delay:
        addi    r17,r17,-1
        cmpwi   r17,0
        bne     voltage_init_5_delay
        sync
        lbz     r14,IIC_STS(r7)
        isync
        andi.   r16,r14,IIC_STS_PT
        cmpwi   r16,IIC_STS_PT
        bne     voltage_end
        andi.   r16,r14,IIC_STS_ERR
        cmpwi   r16,IIC_STS_ERR
        beq     voltage_end
        addi    r15,r15,-1
        cmpwi   r15,0
        bne     voltage_init_5
voltage_end:
        addi    r19,r19,-1
        b       voltage_config
voltage_exit:
        /* End of voltage adjust*/
#endif /* defined(CONFIG_CPM_PM_AD5243) */

cpm_suspend_mem_wait:
	

	 /* Put processor into Wait State */
         mfmsr	r7
         oris	r6,r7,(MSR_WE|MSR_EE|MSR_CE|MSR_DE)@h
         ori	r6,r6,(MSR_WE|MSR_EE|MSR_CE|MSR_DE)@l
         mtmsr	r6
 	sync
 	isync
         mtmsr	r7
 	sync
 	isync
	/* return from SRAM/OCM code */

	blr

_GLOBAL(cpm_suspend_mem_size)
	.long $-cpm_suspend_mem

/*
 * cpm_resume_mem(void)
 *
 * NOTE: r3 => pm_mode
 *       r4 => data pointer
 */
_GLOBAL(cpm_resume_mem)
	/* save off some registers */
	mtsprg	0,r5
	mtsprg	1,r6
	mtsprg	2,r7


	/* Restore voltage */
	cmpwi   r3, CPM_PM_DEEPSLEEP 
	bne     cpm_pm_mem_extint_restore

#if defined(CONFIG_CPM_PM_AD5243)
        /* IIC voltage adjust*/
	lwz	r7, GET_DATA(CPM_PM_DATA_IIC_PTR)
	li      r19,6
ervoltage_config:
        cmpwi   r19,6
        bne     ervoltage_config_1
        li      r18,CPM_PM_AD5243_1_050V
        b       ervoltage_init
ervoltage_config_1:
        cmpwi   r19,5
        bne     ervoltage_config_2
        li      r18,CPM_PM_AD5243_1_100V
        b       ervoltage_init
ervoltage_config_2:
        cmpwi   r19,4
        bne     ervoltage_config_3
        li      r18,CPM_PM_AD5243_1_150V
        b       ervoltage_init
ervoltage_config_3:
        cmpwi   r19,3
        bne     ervoltage_config_4
        li      r18,CPM_PM_AD5243_1_200V
        b       ervoltage_init
ervoltage_config_4:
        cmpwi   r19,2
        bne     ervoltage_config_5
        li      r18,CPM_PM_AD5243_1_250V
        b       ervoltage_init
ervoltage_config_5:
        cmpwi   r19,1
        bne     ervoltage_exit
        li      r18,CPM_PM_AD5243_1_290V

ervoltage_init:
        li      r14,IIC_STS_SCMP
        stb     r14,IIC_STS(r7)
        eieio

        li      r15,12
ervoltage_init_1:
        sync
        lbz     r14,IIC_STS(r7)
        isync
        andi.   r14,r14,IIC_STS_PT
        cmpwi   r14,IIC_STS_PT
        bne     ervoltage_init_2
        addi    r15,r15,-1
        cmpwi   r15,0
        beq     ervoltage_end
        b       ervoltage_init_1
ervoltage_init_2:
        sync
        lbz     r14,IIC_MDCNTL(r7)
        isync
        ori     r14,r14,(IIC_MDCNTL_FMDB|IIC_MDCNTL_FSDB)
        stb     r14,IIC_MDCNTL(r7)
        eieio

        li      r14,0
        stb     r14,IIC_HMADR(r7)
        eieio
        li      r14,CPM_PM_AD5243_ADDR
        stb     r14,IIC_LMADR(r7)
        eieio

        li      r14,1
        stb     r14,IIC_MDBUF(r7)
        eieio
        li      r14,(IIC_CNTL_PT|IIC_CNTL_CHT)
        stb     r14,IIC_CNTL(r7)
        eieio

        li      r15,40
ervoltage_init_3:
        li      r17,256
ervoltage_init_3_delay:
        addi    r17,r17,-1
        cmpwi   r17,0
        bne     ervoltage_init_3_delay
        sync
        lbz     r14,IIC_STS(r7)
        isync
        andi.   r16,r14,IIC_STS_PT
        cmpwi   r16,IIC_STS_PT
        bne     ervoltage_init_4
        andi.   r16,r14,IIC_STS_ERR
        cmpwi   r16,IIC_STS_ERR
        beq     ervoltage_end
        addi    r15,r15,-1
        cmpwi   r15,0
        bne     ervoltage_init_3
ervoltage_init_4:
        stb     r18,IIC_MDBUF(r7)
        eieio
        li      r14,IIC_CNTL_PT
        stb     r14,IIC_CNTL(r7)
        eieio

        li      r15,40
ervoltage_init_5:
        li      r17,256
ervoltage_init_5_delay:
        addi    r17,r17,-1
        cmpwi   r17,0
        bne     ervoltage_init_5_delay
        sync
        lbz     r14,IIC_STS(r7)
        isync
        andi.   r16,r14,IIC_STS_PT
        cmpwi   r16,IIC_STS_PT
        bne     ervoltage_end
        andi.   r16,r14,IIC_STS_ERR
        cmpwi   r16,IIC_STS_ERR
        beq     ervoltage_end
        addi    r15,r15,-1
        cmpwi   r15,0
        bne     ervoltage_init_5
ervoltage_end:
        addi    r19,r19,-1
        b       ervoltage_config
ervoltage_exit:
        /* End of voltage adjust*/
#endif /* defined(CONFIG_CPM_PM_AD5243) */

cpm_pm_mem_extint_restore:

#if defined(CONFIG_460EX) || defined(CONFIG_460GT) || defined(CONFIG_APM82181)\
||	defined(CONFIG_APM82161)

#if defined(CONFIG_APM82181) || defined(CONFIG_APM82161)
	/* Configure OPB and EBC clock to same freq as PLB clock - normal */
	RESTORE_IDCR(51, CPR0, CPR0_PLB2D)
	RESTORE_IDCR(52, CPR0, CPR0_OPBD)
	RESTORE_IDCR(53, CPR0, CPR0_DDR2D)
#else
	/* Configure OPB and EBC clock to same freq as PLB clock - normal */
	RESTORE_IDCR(51, CPR0, CPR0_PLBED)
	RESTORE_IDCR(52, CPR0, CPR0_OPBD)
	RESTORE_IDCR(53, CPR0, CPR0_AHBD)
#endif /* defined(CONFIG_APM82181) || defined(CONFIG_APM82161) */
	/* Put PLL in normal mode */
	GET_IDCR(CPR0, CPR0_PLLC)
	lis r6, CPR0_PLLC_ENG@h
	ori r6, r6, CPR0_PLLC_ENG@l
	or  r7, r7, r6
	SET_IDCR(CPR0, CPR0_PLLC)

	/* Update PLL now */
	lis r7, CPR0_CLKUPD_CUD@h
	ori r7, r7, CPR0_CLKUPD_CUD@l
	SET_IDCR(CPR0, CPR0_CLKUPD)


        /* Take DDR and L2C out of soft reset */
	GET_IDCR(SDR0, SDR0_SRST0)
	lis   r6, ~(SDR0_SRST0_DMC | SDR0_SRST0_L2C)@h
	ori   r6, r6, ~(SDR0_SRST0_DMC | SDR0_SRST0_L2C)@l
	and   r7, r7, r6
	SET_IDCR(SDR0, SDR0_SRST0)

#if defined(CONFIG_APM82181) || defined(CONFIG_APM82161)
	/* restore DDR Controller registers */
	RESTORE_IDCR(12, SDRAM0, SDRAM0_MCOPT1)
	RESTORE_IDCR(17, SDRAM0, SDRAM0_CODT)
	RESTORE_IDCR(13, SDRAM0, SDRAM0_MODT0)
	RESTORE_IDCR(14, SDRAM0, SDRAM0_MODT1)
	RESTORE_IDCR(18, SDRAM0, SDRAM0_RTR)
	RESTORE_IDCR(48, SDRAM0, SDRAM0_MMODE)
	RESTORE_IDCR(44, SDRAM0, SDRAM0_WRDTR)
	RESTORE_IDCR(43, SDRAM0, SDRAM0_CLKTR)
	RESTORE_IDCR(19, SDRAM0, SDRAM0_MB0CF)
	RESTORE_IDCR(20, SDRAM0, SDRAM0_MB1CF)
	RESTORE_IDCR(45, SDRAM0, SDRAM0_SDTR1)
	RESTORE_IDCR(46, SDRAM0, SDRAM0_SDTR2)
	RESTORE_IDCR(47, SDRAM0, SDRAM0_SDTR3)
	RESTORE_IDCR(49, SDRAM0, SDRAM0_MEMODE)
	RESTORE_IDCR(50, SDRAM0, SDRAM0_ECCES)
	RESTORE_IDCR(23, SDRAM0, SDRAM0_INITPLR0)
	RESTORE_IDCR(24, SDRAM0, SDRAM0_INITPLR1)
	RESTORE_IDCR(25, SDRAM0, SDRAM0_INITPLR2)
	RESTORE_IDCR(26, SDRAM0, SDRAM0_INITPLR3)
	RESTORE_IDCR(27, SDRAM0, SDRAM0_INITPLR4)
	RESTORE_IDCR(28, SDRAM0, SDRAM0_INITPLR5)
	RESTORE_IDCR(29, SDRAM0, SDRAM0_INITPLR6)
	RESTORE_IDCR(30, SDRAM0, SDRAM0_INITPLR7)
	RESTORE_IDCR(31, SDRAM0, SDRAM0_INITPLR8)
	RESTORE_IDCR(32, SDRAM0, SDRAM0_INITPLR9)
	RESTORE_IDCR(33, SDRAM0, SDRAM0_INITPLR10)
	RESTORE_IDCR(34, SDRAM0, SDRAM0_INITPLR11)
	RESTORE_IDCR(35, SDRAM0, SDRAM0_INITPLR12)
	RESTORE_IDCR(36, SDRAM0, SDRAM0_INITPLR13)
	RESTORE_IDCR(37, SDRAM0, SDRAM0_INITPLR14)
	RESTORE_IDCR(38, SDRAM0, SDRAM0_INITPLR15)

	RESTORE_IDCR(2, SDRAM0, SDRAM0_BESR)
	RESTORE_IDCR(3, SDRAM0, SDRAM0_BEARL)
	RESTORE_IDCR(4, SDRAM0, SDRAM0_BEARH)
	RESTORE_IDCR(5, SDRAM0, SDRAM0_WMIRQ)
	RESTORE_IDCR(6, SDRAM0, SDRAM0_PLBOPT)
	RESTORE_IDCR(7, SDRAM0, SDRAM0_PUABA)


	/* take DDR out of self refresh */
	lis r7, SDRAM0_MCOPT2_IPTR@h
	ori r7, r7, SDRAM0_MCOPT2_IPTR@l
	SET_IDCR(SDRAM0, SDRAM0_MCOPT2)
ewait_proload_seq:
	GET_IDCR(SDRAM0, SDRAM0_MCSTAT)
	lis r6, SDRAM0_MCSTAT_MIC@h
	ori r6, r6, SDRAM0_MCSTAT_MIC@l
	and r7, r7, r6
	cmpwi   r7, 0
	beq     ewait_proload_seq
echk_self_refresh:
	GET_IDCR(SDRAM0, SDRAM0_MCSTAT)
	lis r6, SDRAM0_MCSTAT_SRMS@h
	ori r6, r6, SDRAM0_MCSTAT_SRMS@l
	and r7, r7, r6
	cmpwi   r7, 0
	bne     echk_self_refresh

	/* Enable DDR controller */
	GET_IDCR(SDRAM0, SDRAM0_MCOPT2)
	lis r6, SDRAM0_MCOPT2_DCEN@h
	ori r6, r6, SDRAM0_MCOPT2_DCEN@l
	or  r7, r7, r6
	SET_IDCR(SDRAM0, SDRAM0_MCOPT2)

	/* Restore FQDC, RFDC, and RCDC */
	RESTORE_IDCR(39, SDRAM0, SDRAM0_RQDC)
	RESTORE_IDCR(40, SDRAM0, SDRAM0_RFDC)
	RESTORE_IDCR(41, SDRAM0, SDRAM0_RDCC)
	RESTORE_IDCR(42, SDRAM0, SDRAM0_DLCR)	

#else
	/* restore DDR Controller registers */
	RESTORE_IDCR(12, MCIF0, MCIF0_MCOPT1)
	RESTORE_IDCR(17, MCIF0, MCIF0_CODT)
	RESTORE_IDCR(13, MCIF0, MCIF0_MODT0)
	RESTORE_IDCR(14, MCIF0, MCIF0_MODT1)
	RESTORE_IDCR(15, MCIF0, MCIF0_MODT2)
	RESTORE_IDCR(16, MCIF0, MCIF0_MODT3)
	RESTORE_IDCR(18, MCIF0, MCIF0_RTR)
	RESTORE_IDCR(48, MCIF0, MCIF0_MMODE)
	RESTORE_IDCR(44, MCIF0, MCIF0_WRDTR)
	RESTORE_IDCR(43, MCIF0, MCIF0_CLKTR)
	RESTORE_IDCR(19, MCIF0, MCIF0_MB0CF)
	RESTORE_IDCR(20, MCIF0, MCIF0_MB1CF)
	RESTORE_IDCR(21, MCIF0, MCIF0_MB2CF)
	RESTORE_IDCR(22, MCIF0, MCIF0_MB3CF)
	RESTORE_IDCR(45, MCIF0, MCIF0_SDTR1)
	RESTORE_IDCR(46, MCIF0, MCIF0_SDTR2)
	RESTORE_IDCR(47, MCIF0, MCIF0_SDTR3)
	RESTORE_IDCR(49, MCIF0, MCIF0_MEMODE)
	RESTORE_IDCR(50, MCIF0, MCIF0_ECCES)
	RESTORE_IDCR(23, MCIF0, MCIF0_INITPLR0)
	RESTORE_IDCR(24, MCIF0, MCIF0_INITPLR1)
	RESTORE_IDCR(25, MCIF0, MCIF0_INITPLR2)
	RESTORE_IDCR(26, MCIF0, MCIF0_INITPLR3)
	RESTORE_IDCR(27, MCIF0, MCIF0_INITPLR4)
	RESTORE_IDCR(28, MCIF0, MCIF0_INITPLR5)
	RESTORE_IDCR(29, MCIF0, MCIF0_INITPLR6)
	RESTORE_IDCR(30, MCIF0, MCIF0_INITPLR7)
	RESTORE_IDCR(31, MCIF0, MCIF0_INITPLR8)
	RESTORE_IDCR(32, MCIF0, MCIF0_INITPLR9)
	RESTORE_IDCR(33, MCIF0, MCIF0_INITPLR10)
	RESTORE_IDCR(34, MCIF0, MCIF0_INITPLR11)
	RESTORE_IDCR(35, MCIF0, MCIF0_INITPLR12)
	RESTORE_IDCR(36, MCIF0, MCIF0_INITPLR13)
	RESTORE_IDCR(37, MCIF0, MCIF0_INITPLR14)
	RESTORE_IDCR(38, MCIF0, MCIF0_INITPLR15)

	/* restore Memory Queue registers */
    RESTORE_DCR(2,  MQ0_B0BASE)
    RESTORE_DCR(3,  MQ0_B1BASE)
    RESTORE_DCR(4,  MQ0_B2BASE)
    RESTORE_DCR(5,  MQ0_B3BASE)
    RESTORE_DCR(6,  MQ0_CF1H)
    RESTORE_DCR(7,  MQ0_BAUL)
    RESTORE_DCR(8,  MQ0_CF1L)
    RESTORE_DCR(9,  MQ0_CFBHL)
    RESTORE_DCR(10, MQ0_BAUH)

	/* take DDR out of self refresh */
	lis r7, MCIF0_MCOPT2_IPTR@h
	ori r7, r7, MCIF0_MCOPT2_IPTR@l
	SET_IDCR(MCIF0, MCIF0_MCOPT2)
ewait_proload_seq:
	GET_IDCR(MCIF0, MCIF0_MCSTAT)
	lis r6, MCIF0_MCSTAT_MIC@h
	ori r6, r6, MCIF0_MCSTAT_MIC@l
	and r7, r7, r6
	cmpwi   r7, 0
	beq     ewait_proload_seq
echk_self_refresh:
	GET_IDCR(MCIF0, MCIF0_MCSTAT)
	lis r6, MCIF0_MCSTAT_SRMS@h
	ori r6, r6, MCIF0_MCSTAT_SRMS@l
	and r7, r7, r6
	cmpwi   r7, 0
	bne     echk_self_refresh

	/* Enable DDR controller */
	GET_IDCR(MCIF0, MCIF0_MCOPT2)
	lis r6, MCIF0_MCOPT2_DCEN@h
	ori r6, r6, MCIF0_MCOPT2_DCEN@l
	or  r7, r7, r6
	SET_IDCR(MCIF0, MCIF0_MCOPT2)

	/* Restore FQDC, RFDC, and RCDC */
	RESTORE_IDCR(39, MCIF0, MCIF0_RQDC)
	RESTORE_IDCR(40, MCIF0, MCIF0_RFDC)
	RESTORE_IDCR(41, MCIF0, MCIF0_RCDC)
	RESTORE_IDCR(42, MCIF0, MCIF0_DLCR)
#endif /*defined(CONFIG_APM82181) || defined(CONFIG_APM82161)*/

#endif /* defined(CONFIG_460EX) || defined(CONFIG_460GT) || defined(CONFIG_APM82181) \
	defined(CONFIG_APM82161)*/

	/* restored saved off interrupt vector base and extint offset */
	RESTORE_SPR(0, SPRN_IVPR)
	RESTORE_SPR(1, SPRN_IVOR4)

	/* restore saved off registers */
	mfsprg	r5,0
	mfsprg	r6,1
	mfsprg	r7,2

	/* return from interrupt */
	rfi

_GLOBAL(cpm_resume_mem_size)
	.long $-cpm_resume_mem

