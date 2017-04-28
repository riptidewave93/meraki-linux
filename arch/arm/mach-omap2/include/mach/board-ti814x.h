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

#ifndef _BOARD_TI814X_H
#define _BOARD_TI814X_H

#include <plat/ti81xx-vpss.h>

#define VPS_SEL_TVP7002_DECODER	0
#define VPS_SEL_SIL9135_DECODER	1

#if  defined(CONFIG_ARCH_TI814X)
int ti814x_pcf8575_init(void);
int ti814x_pcf8575_exit(void);
int vps_ti814x_select_video_decoder(int vid_decoder_id);
int vps_ti814x_set_tvp7002_filter(enum fvid2_standard standard);
#else
static inline int ti814x_pcf8575_init(void)
{
	return 0;
}
static inline int ti814x_pcf8575_exit(void)
{
	return 0;
}
int vps_ti814x_select_video_decoder(int vid_decoder_id)
{
	return 0;
}
int vps_ti814x_set_tvp7002_filter(enum fvid2_standard standard)
{
	return 0;
}
#endif
#endif /* _BOARD_TI814X_H */

