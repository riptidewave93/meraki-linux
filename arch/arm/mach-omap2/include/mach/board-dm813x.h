/*
 * arch/arm/mach-omap2/include/mach/board-ti814x.h
 *
 * Header file for TI814x evm.
 *
 * Copyright (C) 2010 Texas Instruments, Inc. - http://www.ti.com/
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

#ifndef _BOARD_DM813X_H
#define _BOARD_DM813X_H

#include <plat/ti81xx-vpss.h>


#ifdef CONFIG_MACH_DM385EVM
int dm813x_pcf8575_init(void);
int dm813x_pcf8575_exit(void);
int dm813x_pcf8575_ths7360_sd_enable(enum ti81xx_ths_filter_ctrl ctrl);
int dm813x_pcf8575_ths7360_hd_enable(enum ti81xx_ths7360_sf_ctrl ctrl);
#else
static inline int dm813x_pcf8575_init(void)
{
	return 0;
}
static inline int dm813x_pcf8575_exit(void)
{
	return 0;
}
static inline int dm813x_pcf8575_ths7360_sd_enable(
	enum ti81xx_ths_filter_ctrl ctrl)
{
	return 0;
}
static inline int dm813x_pcf8575_ths7360_hd_enable(
	enum ti81xx_ths7360_sf_ctrl ctrl)
{
	return 0;
}
#endif
#endif /* _BOARD_TI814X_H */
