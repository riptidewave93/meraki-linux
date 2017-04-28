/*
 * linux/drivers/media/video/ti81xx/ti81xxvin_lib.h
 *
 * Capture driver library for caculating the Crop and Scaling for TI 816X
 *
 * Copyright (C) 2010 Texas Instruments
 * Author: Hardik Shah(hardik.shah@ti.com)
 *
 * OMAP VOUT driver is used as a reference for this library
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

#ifndef TI81XXVIN_LIB_H
#define TI81XXVIN_LIB_H

int ti81xxvin_try_window(struct v4l2_rect *crop, struct v4l2_window *new_win);
int ti81xxvin_new_window(struct v4l2_rect *crop, struct v4l2_window *win,
		struct v4l2_window *new_win);
int ti81xxvin_new_crop(u32 width, u32 height, struct v4l2_rect *crop,
		const struct v4l2_rect *new_crop);


#endif	/*TI81XXVIN_LIB_H*/
