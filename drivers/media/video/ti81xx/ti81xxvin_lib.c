/*
 * linux/drivers/media/video/ti81xx/ti81xxvin_lib.c
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

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/videodev2.h>
#include <linux/wait.h>
#include <linux/version.h>
#include <media/v4l2-device.h>
#include <media/v4l2-ioctl.h>

int ti81xxvin_try_window(struct v4l2_rect *crop, struct v4l2_window *new_win)
{
	struct v4l2_rect try_win;

	/* make a working copy of the new_win rectangle */
	try_win = new_win->w;

	/* adjust the preview window so it fits on the display by clipping any
	 * offscreen areas
	 */
	if (try_win.left < 0) {
		try_win.width += try_win.left;
		try_win.left = 0;
	}
	if (try_win.top < 0) {
		try_win.height += try_win.top;
		try_win.top = 0;
	}
	/* We do not permit upscaling Only downscaling is permitted */
	try_win.width = (try_win.width < crop->width) ?
		try_win.width : crop->width;
	try_win.height = (try_win.height < crop->height) ?
		try_win.height : crop->height;
	/* We now have a valid preview window, so go with it */
	new_win->w = try_win;
	new_win->field = /*V4L2_FIELD_NONE*/V4L2_FIELD_ANY;
	return 0;
}
EXPORT_SYMBOL_GPL(ti81xxvin_try_window);

/* Given a new render window in new_win, adjust the window to the
 * nearest supported configuration.  The image cropping window in crop
 * will also be adjusted if necessary.  Preference is given to keeping the
 * the window as close to the requested configuration as possible.  If
 * successful, new_win, vout->win, and crop are updated.
 * Returns zero if succesful, or -EINVAL if the requested preview window is
 * impossible and cannot reasonably be adjusted.
 */
int ti81xxvin_new_window(struct v4l2_rect *crop,
		struct v4l2_window *win, struct v4l2_window *new_win)
{
	int err;

	err = ti81xxvin_try_window(crop, new_win);
	if (err)
		return err;

	/* update our preview window */
	win->w = new_win->w;
	win->field = new_win->field;

	/* Taking resizing into effect. We can adjust either the crop window
	 * or the output window. But since output window may be fixed based
	 * on display device/Encoder buffer. Adjusting the crop window.
	 */
	if ((crop->height/win->w.height) >= 8) {
		/* The maximum vertical downsizing ratio is 2:1 */
		crop->height = win->w.height * 8;
	}
	if ((crop->width/win->w.width) >= 8) {
		/* The maximum horizontal downsizing ratio is 2:1 */
		crop->width = win->w.width * 8;
	}
	return 0;
}
EXPORT_SYMBOL_GPL(ti81xxvin_new_window);

/* Given a new cropping rectangle in new_crop, adjust the cropping rectangle to
 * the nearest supported configuration.  Winow wont be adjusted. Only the
 * croping parameters will be adjusted.
 * Returns zero if succesful, or -EINVAL if the requested cropping rectangle is
 * impossible and cannot reasonably be adjusted.
 */
int ti81xxvin_new_crop(u32 width, u32 height,
		struct v4l2_rect *crop, const struct v4l2_rect *new_crop)
{
	struct v4l2_rect try_crop;

	/* make a working copy of the new_crop rectangle */
	try_crop = *new_crop;

	/* adjust the cropping rectangle so it fits in the image */
	if (try_crop.left < 0) {
		try_crop.width += try_crop.left;
		try_crop.left = 0;
	}
	if (try_crop.top < 0) {
		try_crop.height += try_crop.top;
		try_crop.top = 0;
	}
	try_crop.width = (try_crop.width < width) ?
		try_crop.width : width;
	try_crop.height = (try_crop.height < height) ?
		try_crop.height : height;
	if (try_crop.left + try_crop.width > width)
		try_crop.width = width - try_crop.left;
	if (try_crop.top + try_crop.height > height)
		try_crop.height = height - try_crop.top;
	try_crop.width &= ~1;
	try_crop.height &= ~1;
	if (try_crop.width <= 0 || try_crop.height <= 0)
		return -EINVAL;
	/* update our cropping rectangle and we're done */
	*crop = try_crop;
	return 0;
}
EXPORT_SYMBOL_GPL(ti81xxvin_new_crop);
MODULE_AUTHOR("Hardik Shah(hardik.shah@ti.com");
MODULE_DESCRIPTION("DM81xx Vin Capture driver library");
MODULE_LICENSE("GPL");
