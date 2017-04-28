/*
 * File: include/linux/ti81xxscalar.h
 *
 * Copyright (C) 2011 TI
 * Author: Bikash Kund <bkund@ti.com>
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
#ifndef __LINUX_TI81XXSCALE_H__
#define __LINUX_TI81XXSCALE_H__

#include <linux/ioctl.h>
#include <linux/types.h>


/*customer ioctl definitions*/
#define TISCALE_IOW(num, dtype)  _IOW('T', num, dtype)
#define TISCALE_IOR(num, dtype)  _IOR('T', num, dtype)

#define TISET_SCALAR_PARAMS   TISCALE_IOW(0, struct ti81xxvid_scalarparam)
#define TIGET_SCALAR_PARAMS   TISCALE_IOR(1, struct ti81xxvid_scalarparam)

struct ti81xxvid_scalarparam {
	int  scalar_enable;
	int  inframe_width;
	int  inframe_height;
	int  yframe_offset;
	int  yframe_size;
	int  yoffset_flag;
	int  chroma_offset;
	void   *scoutbuf;
};

#endif
