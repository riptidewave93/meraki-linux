/*
 * arch/arm/plat-ambarella/include/hal/hal.h
 *
 * Author: Anthony Ginger <hfjiang@ambarella.com>
 *
 * Copyright (C) 2004-2010, Ambarella, Inc.
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

#ifndef __PLAT_AMBARELLA_HAL_H__
#define __PLAT_AMBARELLA_HAL_H__

/* ==========================================================================*/
#ifndef __ASM_ARCH_HARDWARE_H
#error "include <mach/hardware.h> first"
#endif

#define DEFAULT_HAL_START		(DEFAULT_MEM_START + 0x000a0000)
#define DEFAULT_HAL_BASE		(0xfee00000)
#define DEFAULT_HAL_SIZE		(0x00030000)

#define AMBARELLA_HAL_OS_LOCK()		AMBARELLA_GLOBAL_HW_LOCK()
#define AMBARELLA_HAL_OS_UNLOCK()	AMBARELLA_GLOBAL_HW_UNLOCK()

/* ==========================================================================*/
#ifndef __ASSEMBLER__

#include <hal/header.h>
#if (CHIP_REV == A5S)
#include <hal/a5s/ambhal.h>
#elif (CHIP_REV == I1)
#include <hal/i1/ambhal.h>
#elif (CHIP_REV == A7L)
#include <hal/a7l/ambhal.h>
#elif (CHIP_REV == A8)
#include <hal/a8/ambhal.h>
#elif (CHIP_REV == AMB_S2)
#include <hal/s2/ambhal.h>
#else
#error "Undefined CHIP_REV, Can't support HAL!"
#endif

struct ambarella_mem_hal_desc {
	u32 physaddr;
	u32 size;
	u32 virtual;
	u32 remapped;
	u32 inited;
};

/* ==========================================================================*/
extern void ambarella_hal_set_invalid(void);
extern u32 ambarella_hal_get_size(void);
extern void *ambarella_hal_get_vp(void);

extern u32 get_apb_bus_freq_hz(void);
extern u32 get_ahb_bus_freq_hz(void);
extern u32 get_core_bus_freq_hz(void);

/* ==========================================================================*/
#define HAL_BASE_VP			(ambarella_hal_get_vp())

#endif /* __ASSEMBLER__ */
/* ==========================================================================*/

#endif /* __PLAT_AMBARELLA_HAL_H__ */

