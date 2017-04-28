/**
 * APM APM86xxx Shared Memory Manager header
 *
 * Copyright (c) 2010 Applied Micro Circuits Corporation.
 * All rights reserved. Vivek Mahajan <vmahajan@apm.com>
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
 * @file apm_shm.h
 **
 */

#ifndef __APM_SHM_H__
#define __APM_SHM_H__

#define APM86xxx_SHMEM_STRING	"APM86xxx shared memory"
#define SHM_SIGN		0xdeadbeef
#define NAME_SZ			16

/* Shared memory struct */
struct shmem_ctx {
	spinlock_t lock;
	unsigned int entries;
	unsigned int signature;
};

struct shmem_list {
	unsigned int phandle;
	unsigned int size;
	char name[NAME_SZ];
	void *paddr;
	int *pnext;
};

/* Local shared memory struct */
struct shmem_lo_ctx {
	unsigned int vbase;
	phys_addr_t pbase;
	phys_addr_t paddr;
	unsigned int size;
	struct shmem_ctx *shm;
};

enum mem_status {
	MEM_NOT_AVAIL = -1,
	MEM_JUST_ENOUGH,
	MEM_AVAIL,
};

void atomic_csr_setbits(u32 masks, void *csr);
void atomic_csr_clrbits(u32 masks, void *csr);
void atomic_csr_clrsetbits(u32 clrmasks, u32 setmasks, void *csr);
void *apm_shmem_alloc(char *name, unsigned int size);

#endif /* __APM_SHM_H__ */
