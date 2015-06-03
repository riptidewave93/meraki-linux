/*
 * Copyright (C) 2013, Broadcom Corporation. All Rights Reserved.
 * 
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
/*
 * Header for declaring shim layer exports.
 */

#ifndef __SHM_DOT_H_INCLUDED__
#define __SHM_DOT_H_INCLUDED__

#include <linux/module.h>
#include <linux/platform_device.h>

extern struct resource *
iproc_platform_get_resource(struct platform_device *dev, unsigned int type,
		      unsigned int num);
extern int iproc_platform_device_register(struct platform_device * pdev);
extern void iproc_platform_device_unregister(struct platform_device * pdev);
extern int iproc_platform_driver_register(struct platform_driver *drv);
extern void iproc_platform_driver_unregister(struct platform_driver *drv);

#endif /*#ifndef __SHM_DOT_H_INCLUDED__*/
