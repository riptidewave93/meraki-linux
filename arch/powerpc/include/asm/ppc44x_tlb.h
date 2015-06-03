/*
 * PPC44x TLB helper functions
 *
 * Copyright (c) 2010 Applied Micro Circuits Corporation.
 * All rights reserved. Victor Gallardo <vgallardo@apm.com>.
 *
 * Based on arch/powerpc/kvm/44x_tlb.c
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

#ifndef __ASM_POWERPC_PPC44X_TLB_H
#define __ASM_POWERPC_PPC44X_TLB_H

#define PPC44x_TLB_SIZE 64

#ifndef __ASSEMBLY__

#include <asm/reg_booke.h>

struct ppc44x_tlbentry {
	unsigned int tid;
	unsigned int word0;
	unsigned int word1;
	unsigned int word2;
};

static inline unsigned int ppc44x_get_tlbindex(void * vaddr)
{
	unsigned int addr = (unsigned int)vaddr;
	unsigned int index;

	asm volatile("tlbsx %[index], 0, %[addr]\n"
		: [index] "=r"(index)
		: [addr] "r"(addr)
		: "cc"
	);

	return index;
}

static inline void ppc44x_get_tlbentry(unsigned int index,
                                struct ppc44x_tlbentry *tlbentry)
{
	asm volatile(
		"tlbre %[word0], %[index], 0\n"
		"mfspr %[tid], %[sprn_mmucr]\n"
		"andi. %[tid], %[tid], 0xff\n"
		"tlbre %[word1], %[index], 1\n"
		"tlbre %[word2], %[index], 2\n"
		: [word0] "=r"(tlbentry->word0),
		[word1] "=r"(tlbentry->word1),
		[word2] "=r"(tlbentry->word2),
		[tid] "=r"(tlbentry->tid)
		: [index] "r"(index),
		[sprn_mmucr] "i"(SPRN_MMUCR)
		: "cc"
	);
}

static inline void ppc44x_set_tlbentry(unsigned int index,
                                struct ppc44x_tlbentry *tlbentry)
{
	unsigned long tmp;

	asm volatile(
		"sync\n"
		"mfspr %[tmp], %[sprn_mmucr]\n"
		"rlwimi %[tmp], %[tid], 0, 0xff\n"
		"mtspr %[sprn_mmucr], %[tmp]\n"
		"tlbwe %[word0], %[index], 0\n"
		"tlbwe %[word1], %[index], 1\n"
		"tlbwe %[word2], %[index], 2\n"
		"isync\n"
		: [tmp]   "=&r"(tmp)
		: [word0] "r"(tlbentry->word0),
		[word1] "r"(tlbentry->word1),
		[word2] "r"(tlbentry->word2),
		[tid]   "r"(tlbentry->tid),
		[index] "r"(index),
		[sprn_mmucr] "i"(SPRN_MMUCR)
	);
}

#endif /* __ASSEMBLY__ */

#endif /* __ASM_POWERPC_PPC44X_TLB_H */
