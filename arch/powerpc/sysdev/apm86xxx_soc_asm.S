/*
 * AM86xxx SoC helper code
 *
 * Copyright (c) 2010 Applied Micro Circuits Corporation.
 * Victor Gallardo <vgallardo@apm.com>.
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
#include <asm/dcr-regs.h>
#include <asm/page.h>
#include <asm/apm86xxx_soc.h>

_GLOBAL(flush_dcache)
	/* Calculate the number data blocks required to fill cache
	 *    DATA_BLOCKS = SIZE_OF_CACHE / BYTES_PER_BLOCK
	 *    DATA_BLOCKS = (32*1024) / 32; if only L1
	 *    DATA_BLOCKS = (256*1024) / 32; if L1 and L2
	 * Lets take care of worse case
	 */
	lis	r3, ((256*1024))@h
	ori	r3, r3, ((256*1024))@l

	/* LOAD CACHE: Touch all 256K of cacheable memory using lwz
	 * instruction. This step is necessary to avoid having to
	 * calculate what the effective address is for each line.
	 * It also forces most of the dirty lines to be flushed from
	 * dcache when replaced.
	 */
	mtctr   r3			/* number of data blocks */
	lis     r4, KERNELBASE@h	/* start address */
1:	lwz	r5, 0(r4)
	addi	r4, r4, 4		/* move to next block */
	bdnz    1b
	sync

	li	r3, ((256*1024)/32)
	/* FLUSH CACHE: Flush all cache lines in dcache using the dcbf
	 * instruction with the effect address generated above. There
	 * is chance some of the addresses used above were already in
	 * dcache. This step ensures all dirty lines have been flushed.
	 */
	mtctr   r3			/* number of data blocks */
	lis     r4, KERNELBASE@h	/* start address */
2:	dcbf	0, r4
	addi	r4, r4, 32		/* move to next block */
	bdnz    2b
	sync

	/* return */
	blr

_GLOBAL(l2c_disable)
	/* Disable L2 cache if L2 cache enabled */
	li	r3, DCRN_L2CR0
	mtdcr	DCRN_L2C_ADDR, r3	/* set address offset */
	mfdcr	r4, DCRN_L2C_DATA
        lis	r3, L2CR0_AS_256K@h
        ori	r3, r3, L2CR0_AS_256K@l
	and	r4, r4, r3
	cmpw	r4, r3
	beq	1f
	blr		

	/* Invalidate L2 */
1:	li	r3, DCRN_L2CR0
	mtdcr	DCRN_L2C_ADDR, r3	/* set address offset */
	mfdcr	r4, DCRN_L2C_DATA
        lis	r3, L2CR0_TAI@h
        ori	r3, r3, L2CR0_TAI@l
	or	r4, r4, r3
	mtdcr	DCRN_L2C_DATA, r4
2:	mfdcr	r4, DCRN_L2C_DATA
	and	r4, r4, r3
	cmpwi	r4, 0
	bne	2b

#if defined(CONFIG_SMP)
	/* Take CPU out of coherency domain */
	li	r3, DCRN_L2CR2
	mtdcr	DCRN_L2C_ADDR, r3	/* set address offset */
	mfdcr	r4, DCRN_L2C_DATA
        lis	r3, L2CR2_SNPME@h
        ori	r3, r3, L2CR2_SNPME@l
	andc	r4, r4, r3
	mtdcr	DCRN_L2C_DATA, r4
#endif

	/* Disable L2 */
#ifdef DISABLE_L2C_ON_PWRMGMT
	li	r3, DCRN_L2CR0
	mtdcr	DCRN_L2C_ADDR, r3	/* set address offset */
	mfdcr	r4, DCRN_L2C_DATA
        lis	r3, L2CR0_AS_MASK@h
        ori	r3, r3, L2CR0_AS_MASK@l
	andc	r4, r4, r3
	mtdcr	DCRN_L2C_DATA, r4
#endif

	/* return */
	blr

_GLOBAL(l2c_suspend)
	/* Check if L2 cache enabled */
	li	r3, DCRN_L2CR0
	mtdcr	DCRN_L2C_ADDR, r3	/* set address offset */
	mfdcr	r4, DCRN_L2C_DATA
        lis	r3, L2CR0_AS_256K@h
        ori	r3, r3, L2CR0_AS_256K@l
	and	r4, r4, r3
	cmpw	r4, r3
	beq	1f
	blr		

	/* Invalidate L2 */
1:	li	r3, DCRN_L2CR0
	mtdcr	DCRN_L2C_ADDR, r3	/* set address offset */
	mfdcr	r4, DCRN_L2C_DATA
        lis	r3, L2CR0_TAI@h
        ori	r3, r3, L2CR0_TAI@l
	or	r4, r4, r3
	mtdcr	DCRN_L2C_DATA, r4
2:	mfdcr	r4, DCRN_L2C_DATA
	and	r4, r4, r3
	cmpwi	r4, 0
	bne	2b

#if 0 /*defined(CONFIG_SMP) */
	/* Take CPU out of coherency domain */
	li	r3, DCRN_L2CR2
	mtdcr	DCRN_L2C_ADDR, r3	/* set address offset */
	mfdcr	r4, DCRN_L2C_DATA
        lis	r3, L2CR2_SNPME@h
        ori	r3, r3, L2CR2_SNPME@l
	andc	r4, r4, r3
	mtdcr	DCRN_L2C_DATA, r4
#endif

	/* return */
	blr

_GLOBAL(l2c_resume)
	/* Check if L2 cache enabled */
	li	r3, DCRN_L2CR0
	mtdcr	DCRN_L2C_ADDR, r3	/* set address offset */
	mfdcr	r4, DCRN_L2C_DATA
        lis	r3, L2CR0_AS_256K@h
        ori	r3, r3, L2CR0_AS_256K@l
	and	r4, r4, r3
	cmpw	r4, r3
	beq	1f
	blr		

	/* Invalidate L2 */
1:	li	r3, DCRN_L2CR0
	mtdcr	DCRN_L2C_ADDR, r3	/* set address offset */
	mfdcr	r4, DCRN_L2C_DATA
        lis	r3, L2CR0_TAI@h
        ori	r3, r3, L2CR0_TAI@l
	or	r4, r4, r3
	mtdcr	DCRN_L2C_DATA, r4
2:	mfdcr	r4, DCRN_L2C_DATA
	and	r4, r4, r3
	cmpwi	r4, 0
	bne	2b

#if 0 /*defined(CONFIG_SMP) */
	/* Put CPU back into coherency domain */
	li	r3, DCRN_L2CR2
	mtdcr	DCRN_L2C_ADDR, r3	/* set address offset */
	mfdcr	r4, DCRN_L2C_DATA
        lis	r3, L2CR2_SNPME@h
        ori	r3, r3, L2CR2_SNPME@l
	or	r4, r4, r3
	mtdcr	DCRN_L2C_DATA, r4
#endif

#if !defined(CONFIG_SMP) && defined(CONFIG_APM86xxx_IOCOHERENT)	
	li	r3, 0			/* flush to L2 only */
	mtspr	SPRN_CCR1,r3		/* Mark CCR1[COBE]=0 */
#endif
	
	/* return */
	blr

