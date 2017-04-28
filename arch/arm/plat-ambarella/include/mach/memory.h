/*
 * arch/arm/plat-ambarella/include/mach/memory.h
 *
 * History:
 *	2006/12/27 - [Charles Chiou] created file
 *
 * Copyright (C) 2004-2009, Ambarella, Inc.
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
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 */

#ifndef __ASM_ARCH_MEMORY_H
#define __ASM_ARCH_MEMORY_H

/*
 * MAP Sample	Phisical		Virtual			Size
 * --------------------------------------------------------------------------
 * AHB(IO)	AHB_PHYS_BASE		AHB_BASE		AHB_SIZE
 * APB(IO)	APB_PHYS_BASE		APB_BASE		APB_BASE
 * MISC(IO)	MISC_PHYS_BASE		MISC_BASE		MISC_SIZE
 * PPM		DEFAULT_MEM_START	NOLINUX_MEM_V_START	CONFIG_AMBARELLA_PPM_SIZE
 * Linux MEM	PHYS_OFFSET		CONFIG_VMSPLIT_xG	Linux MEM Size
 * BSB		BSB_START		BSB_BASE		BSB_SIZE
 * DSP		DSP_START		DSP_BASE		DSP_SIZE
 */

/* ==========================================================================*/
#if defined(CONFIG_PLAT_AMBARELLA_MEM_START_LOW)
#define DEFAULT_MEM_START		(0x00000000)
#else
#define DEFAULT_MEM_START		(0xc0000000)
#endif
#ifndef CONFIG_ARM_PATCH_PHYS_VIRT
#define PHYS_OFFSET			(DEFAULT_MEM_START + CONFIG_AMBARELLA_PPM_SIZE)
#endif

/* ==========================================================================*/
#if defined(CONFIG_PLAT_AMBARELLA_AHB_APB_HIGH)
#define AHB_PHYS_BASE			(0xe0000000)
#define APB_PHYS_BASE			(0xe8000000)
#else
#define AHB_PHYS_BASE			(0x60000000)
#define APB_PHYS_BASE			(0x70000000)
#endif
#define AHB_SIZE			(0x01000000)
#define APB_SIZE			(0x01000000)
#if defined(CONFIG_PLAT_AMBARELLA_SUPPORT_MMAP_AXI)
#define AXI_PHYS_BASE			(0xf0000000)
#if defined(CONFIG_PLAT_AMBARELLA_S2_CORTEX)
#define AXI_SIZE			(0x00030000)
#else
#define AXI_SIZE                       (0x00003000)
#endif
#endif
#if defined(CONFIG_PLAT_AMBARELLA_SUPPORT_MMAP_DDD)
#define DDD_PHYS_BASE			(0xf0020000)
#define DDD_SIZE			(0x00000e00)
#endif
#if defined(CONFIG_PLAT_AMBARELLA_SUPPORT_MMAP_DRAMC)
#define DRAMC_PHYS_BASE			(0xdffe0000)
#define DRAMC_SIZE			(0x00020000)
#endif
#if defined(CONFIG_PLAT_AMBARELLA_SUPPORT_MMAP_CRYPT)
#define CRYPT_PHYS_BASE			(0xfffef000)
#define CRYPT_SIZE			(0x00001000)
#endif
#if defined(CONFIG_PLAT_AMBARELLA_SUPPORT_MMAP_AHB64)
#define AHB64_PHYS_BASE			(0x80000000)
#define AHB64_SIZE			(0x00020000)
#endif

#if defined(CONFIG_PLAT_AMBARELLA_SUPPORT_MMAP_DBGBUS)
#define DBGBUS_PHYS_BASE		(0xec000000)
#define DBGBUS_SIZE			(0x00200000)
#endif

#if defined(CONFIG_VMSPLIT_3G)
#define NOLINUX_MEM_V_START		(0xe0000000)
#define AHB_BASE			(0xf0000000)
#define APB_BASE			(0xf1000000)
#if defined(CONFIG_PLAT_AMBARELLA_SUPPORT_MMAP_AXI)
#define AXI_BASE			(0xf2000000)
#endif
#if defined(CONFIG_PLAT_AMBARELLA_SUPPORT_MMAP_DDD)
#define DDD_BASE			(0xf2020000)
#endif
#if defined(CONFIG_PLAT_AMBARELLA_SUPPORT_MMAP_DRAMC)
#define DRAMC_BASE			(0xf2040000)
#endif
#if defined(CONFIG_PLAT_AMBARELLA_SUPPORT_MMAP_CRYPT)
#define CRYPT_BASE			(0xf2080000)
#endif
#if defined(CONFIG_PLAT_AMBARELLA_SUPPORT_MMAP_AHB64)
#define AHB64_BASE			(0xf20c0000)
#endif
#if defined(CONFIG_PLAT_AMBARELLA_SUPPORT_MMAP_DBGBUS)
#define DBGBUS_BASE			(0xf2200000)
#endif
#elif defined(CONFIG_VMSPLIT_2G)
#define NOLINUX_MEM_V_START		(0xc0000000)
#define AHB_BASE			(0xe0000000)
#define APB_BASE			(0xe8000000)
#if defined(CONFIG_PLAT_AMBARELLA_SUPPORT_MMAP_AXI)
#define AXI_BASE			(0xf0000000)
#endif
#if defined(CONFIG_PLAT_AMBARELLA_SUPPORT_MMAP_DDD)
#define DDD_BASE			(0xf0020000)
#endif
#if defined(CONFIG_PLAT_AMBARELLA_SUPPORT_MMAP_DRAMC)
#define DRAMC_BASE			(0xef000000)
#endif
#if defined(CONFIG_PLAT_AMBARELLA_SUPPORT_MMAP_CRYPT)
#define CRYPT_BASE			(0xee000000)
#endif
#if defined(CONFIG_PLAT_AMBARELLA_SUPPORT_MMAP_AHB64)
#define AHB64_BASE			(0xed000000)
#endif
#if defined(CONFIG_PLAT_AMBARELLA_SUPPORT_MMAP_DBGBUS)
#define DBGBUS_BASE			(0xec000000)
#endif
#else /* CONFIG_VMSPLIT_1G */
#define NOLINUX_MEM_V_START		(0x80000000)
#define AHB_BASE			(0xe0000000)
#define APB_BASE			(0xe8000000)
#if defined(CONFIG_PLAT_AMBARELLA_SUPPORT_MMAP_AXI)
#define AXI_BASE			(0xf0000000)
#endif
#if defined(CONFIG_PLAT_AMBARELLA_SUPPORT_MMAP_DDD)
#define DDD_BASE			(0xf0020000)
#endif
#if defined(CONFIG_PLAT_AMBARELLA_SUPPORT_MMAP_DRAMC)
#define DRAMC_BASE			(0xef000000)
#endif
#if defined(CONFIG_PLAT_AMBARELLA_SUPPORT_MMAP_CRYPT)
#define CRYPT_BASE			(0xee000000)
#endif
#if defined(CONFIG_PLAT_AMBARELLA_SUPPORT_MMAP_AHB64)
#define AHB64_BASE			(0xed000000)
#endif
#if defined(CONFIG_PLAT_AMBARELLA_SUPPORT_MMAP_DBGBUS)
#define DBGBUS_BASE			(0xec000000)
#endif
#endif	/* CONFIG_VMSPLIT_3G */
#define NOLINUX_MEM_V_SIZE		(AHB_BASE - NOLINUX_MEM_V_START)

/* ==========================================================================*/
#define DEFAULT_BSB_START		(0x00000000)
#define DEFAULT_BSB_BASE		(0x00000000)
#define DEFAULT_BSB_SIZE		(0x00000000)

#define DEFAULT_DSP_START		(0x00000000)
#define DEFAULT_DSP_BASE		(0x00000000)
#define DEFAULT_DSP_SIZE		(0x00000000)

#define DEFAULT_BST_START		(DEFAULT_MEM_START + 0x00000000)
#define DEFAULT_BST_SIZE		(0x00000000)
#define AMB_BST_MAGIC			(0xffaa5500)
#define AMB_BST_INVALID			(0xdeadbeaf)
#define AMB_BST_START_COUNTER		(0xffffffff)

#define DEFAULT_DEBUG_START		(DEFAULT_MEM_START + 0x000f8000)
#define DEFAULT_DEBUG_SIZE		(0x00008000)

/* ==========================================================================*/
#define __virt_to_bus(x)		__virt_to_phys(x)
#define __bus_to_virt(x)		__phys_to_virt(x)
#define __pfn_to_bus(x)			__pfn_to_phys(x)
#define __bus_to_pfn(x)			__phys_to_pfn(x)

/* ==========================================================================*/
#define CONSISTENT_DMA_SIZE		SZ_8M

/* ==========================================================================*/
#define MAX_PHYSMEM_BITS		32
#define SECTION_SIZE_BITS		24
/* ==========================================================================*/

#endif

