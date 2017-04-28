/*
 * TI81XX common defs
 *
 * This module implements TI81XX specific Power management Routines
 *
 * Copyright (C) 2012 Texas Instruments Incorporated - http://www.ti.com/
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

#ifndef OMAP_MACH_TI81XX_COMMON_H
#define OMAP_MACH_TI81XX_COMMON_H

extern void __iomem *dram_sync;
extern void __iomem *sram_sync;

extern void ti81xx_do_wfi_sync(void __iomem *dram_sync,
		void __iomem *sram_sync);

#endif /* !OMAP_MACH_TI81XX_COMMON_H */
