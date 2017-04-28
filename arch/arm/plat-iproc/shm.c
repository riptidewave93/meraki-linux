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

#include "include/plat/shm.h"


/**
 *	iproc_platform_get_resource -
 *           wrapper function for platform_get_resource
 *	@dev: platform device
 *	@type: resource type
 *	@num: resource index
 */
struct resource *
iproc_platform_get_resource(struct platform_device *dev, unsigned int type,
		      unsigned int num)
{
	return platform_get_resource(dev, type, num);
}
EXPORT_SYMBOL(iproc_platform_get_resource);


/**
 *	iproc_platform_driver_register -
 *           wrapper function for platform_driver_register
 *	@drv: platform driver structure
 */
int iproc_platform_driver_register(struct platform_driver *drv)
{
	return platform_driver_register(drv);
}
EXPORT_SYMBOL(iproc_platform_driver_register);


/**
 *	iproc_platform_driver_unregister
 *           wrapper function for platform_driver_unregister
 *	@drv: platform driver structure
 */
void iproc_platform_driver_unregister(struct platform_driver *drv)
{
	return platform_driver_unregister(drv);
}
EXPORT_SYMBOL(iproc_platform_driver_unregister);


/**
 *      iproc_platform_device_register - add a platform-level device
 *          wrapper function for platform_device_register
 *      @pdev:  platform device we're adding
 *
 */
int iproc_platform_device_register(struct platform_device * pdev)
{
        return platform_device_register(pdev);
}
EXPORT_SYMBOL(iproc_platform_device_register);


/**
 *	iproc_platform_device_unregister -
 *          wrapper function for platform_device_unregister
 *	@pdev:	platform device we're unregistering
 */
void iproc_platform_device_unregister(struct platform_device * pdev)
{
	return platform_device_unregister(pdev);
}
EXPORT_SYMBOL(iproc_platform_device_unregister);
