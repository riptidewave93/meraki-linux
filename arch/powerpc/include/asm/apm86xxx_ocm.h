/*
 * APM86xxx OCM memory allocation support
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

#ifndef __ASM_POWERPC_APM86xxx_OCM_H__
#define __ASM_POWERPC_APM86xxx_OCM_H__

#include <linux/types.h>

#define OCM_NON_CACHED	0x100
#define OCM_CACHED	0x101
#define OCM_IPP		0x200
#define OCM_QM		0x201

#if defined(CONFIG_APM86xxx_OCM)

void * ocm_alloc(phys_addr_t *phys, int size, int align,
		  int flags, const char *owner);
void ocm_free(const void * virt);

void * ocm_get_buf(phys_addr_t *phys, int *size, int type, int *offset);

#else

static inline void * ocm_alloc(phys_addr_t *phys, int size, int align,
			       int flags, const char *owner)
{
	return NULL;
}

static inline void ocm_free(const void * virt) {}

static inline void * ocm_get_buf(phys_addr_t *phys, int *size,
				 int type, int *offset)
{
	return NULL;
}
#endif /* CONFIG_APM86xxx_OCM */

#endif  /* __ASM_POWERPC_APM86xxx_OCM_H__ */

