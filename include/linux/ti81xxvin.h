/*
 * File: include/linux/ti81xxcapt.c
 * TI81xx V4L2 capture driver header file
 *
 * Copyright (C) 2011 TI
 * Author: Hardik Shah <hardik.shah@ti.com>
 *
 * Some code and ideas was from TI OMAP2 Driver by Tomi Valkeinen.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place - Suite 330, Boston, MA	02111-1307, USA.
 */
#ifndef __LINUX_TI81XXCAPT_H__
#define __LINUX_TI81XXCAPT_H__

#include <linux/ioctl.h>
#include <linux/types.h>


/*customer ioctl definitions*/
#define TICAPT_IOW(num, dtype)  _IOW('Z', num, dtype)
#define TICAPT_IOR(num, dtype)  _IOR('Z', num, dtype)
#define TICAPT_IOWR(num, dtype) _IOWR('Z', num, dtype)
#define TICAPT_IO(num)          _IO('Z', num)

#define TICAPT_CHECK_OVERFLOW   TICAPT_IOR(0, struct ti81xxvin_overflow_status)
#define TICAPT_RESET		TICAPT_IO(1)

struct ti81xxvin_overflow_status {
	int porta_overflow;
	int portb_overflow;
};

#endif
