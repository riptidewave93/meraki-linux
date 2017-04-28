/*
 * Modifications by Matt Porter (mporter@mvista.com) to support
 * PPC44x Book E processors.
 *
 * This file contains the routines for initializing the MMU
 * on the 4xx series of chips.
 *  -- paulus
 *
 *  Derived from arch/ppc/mm/init.c:
 *    Copyright (C) 1995-1996 Gary Thomas (gdt@linuxppc.org)
 *
 *  Modifications by Paul Mackerras (PowerMac) (paulus@cs.anu.edu.au)
 *  and Cort Dougan (PReP) (cort@cs.nmt.edu)
 *    Copyright (C) 1996 Paul Mackerras
 *
 *  Derived from "arch/i386/mm/init.c"
 *    Copyright (C) 1991, 1992, 1993, 1994  Linus Torvalds
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version
 *  2 of the License, or (at your option) any later version.
 *
 */

#include <linux/init.h>
#include <asm/system.h>
#include <asm/page.h>
#include <asm/cacheflush.h>

#include "mmu_decl.h"

/* Used by the 44x TLB replacement exception handler.
 * Just needed it declared someplace.
 */
unsigned int tlb_44x_index[NR_CPUS]; /* = 0 */
unsigned int tlb_44x_hwater[NR_CPUS];
					
int icache_44x_need_flush;

static void ppc44x_update_tlb_hwater(void)
{
	extern unsigned int tlb_44x_patch_hwater_D[];
	extern unsigned int tlb_44x_patch_hwater_I[];

	/* The TLB miss handlers hard codes the watermark in a cmpli
	 * instruction to improve performances rather than loading it
	 * from the global variable. Thus, we patch the instructions
	 * in the 2 TLB miss handlers when updating the value
	 */
	tlb_44x_patch_hwater_D[0] = (tlb_44x_patch_hwater_D[0] & 0xffff0000) |
		tlb_44x_hwater[0];
	flush_icache_range((unsigned long)&tlb_44x_patch_hwater_D[0],
			   (unsigned long)&tlb_44x_patch_hwater_D[1]);
	tlb_44x_patch_hwater_I[0] = (tlb_44x_patch_hwater_I[0] & 0xffff0000) |
		tlb_44x_hwater[0];
	flush_icache_range((unsigned long)&tlb_44x_patch_hwater_I[0],
			   (unsigned long)&tlb_44x_patch_hwater_I[1]);
}

/*
 * "Pins" a 256MB TLB entry in AS0 for kernel lowmem
 */
static void ppc44x_pin_tlb(unsigned int virt, unsigned int phys,
					int cpu_id)
{
	unsigned int entry = tlb_44x_hwater[cpu_id]--;

	ppc44x_update_tlb_hwater();

	__asm__ __volatile__(
		"tlbwe	%2,%3,%4\n"
		"tlbwe	%1,%3,%5\n"
		"tlbwe	%0,%3,%6\n"
	:
#if !defined(CONFIG_NOT_COHERENT_CACHE) && defined(CONFIG_SMP) && defined(CONFIG_APM86xxx)
	/* Coherent cache, SMP, and APM86xxx */
        : "r" (PPC44x_TLB_SW | PPC44x_TLB_SR | PPC44x_TLB_SX | PPC44x_TLB_G | PPC44x_TLB_WL1 | PPC44x_TLB_M),
#elif !defined(CONFIG_NOT_COHERENT_CACHE) && !defined(CONFIG_SMP) && defined(CONFIG_APM86xxx)
	/* Coherent cache, Non-SMP, and APM86xx */
        : "r" (PPC44x_TLB_SW | PPC44x_TLB_SR | PPC44x_TLB_SX | PPC44x_TLB_G | PPC44x_TLB_WL1 | PPC44x_TLB_U2),
#elif defined(CONFIG_NOT_COHERENT_CACHE) && defined(CONFIG_APM86xxx)
	/* Non-coherent cache and APM86xxx */
        : "r" (PPC44x_TLB_SW | PPC44x_TLB_SR | PPC44x_TLB_SX | PPC44x_TLB_G | PPC44x_TLB_U2),
#else
        : "r" (PPC44x_TLB_SW | PPC44x_TLB_SR | PPC44x_TLB_SX | PPC44x_TLB_G),
#endif
	  "r" (phys),
	  "r" (virt | PPC44x_TLB_VALID | PPC44x_TLB_256M),
	  "r" (entry),
	  "i" (PPC44x_TLB_PAGEID),
	  "i" (PPC44x_TLB_XLAT),
	  "i" (PPC44x_TLB_ATTRIB));
}

void MMU_init_hw(void)
{
	int cpu_id = 0;
#ifdef CONFIG_SMP
        cpu_id = mfspr(SPRN_PIR);
#endif
	tlb_44x_hwater[cpu_id] = PPC44x_TLB_SIZE - 1 - PPC44x_EARLY_TLBS;
	ppc44x_update_tlb_hwater();

	flush_instruction_cache();
}

unsigned long mmu_mapin_ram(void)
{
	unsigned long addr;
        int cpu_id = 0;
#ifdef CONFIG_SMP
        cpu_id = mfspr(SPRN_PIR);
#endif

	/* Pin in enough TLBs to cover any lowmem not covered by the
	 * initial 256M mapping established in head_44x.S */
#ifndef CONFIG_RELOCATABLE
	for (addr = PPC_PIN_SIZE; (addr + PHYSICAL_START) < lowmem_end_addr;
	     addr += PPC_PIN_SIZE)
		ppc44x_pin_tlb(addr + PAGE_OFFSET, addr + PHYSICAL_START, 
				cpu_id);
#else
	for (addr = PPC_PIN_SIZE; (addr + memstart_addr) < lowmem_end_addr;
	     addr += PPC_PIN_SIZE)
		ppc44x_pin_tlb(addr + PAGE_OFFSET, addr + memstart_addr, 
				cpu_id);
#endif

	return total_lowmem;
}
