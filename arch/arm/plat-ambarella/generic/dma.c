/*
 * arch/arm/plat-ambarella/generic/dma.c
 * The file replaces the former dma.c for adding DMA engine driver usage
 * History:
 *	2012/05/10 - Ken He <jianhe@ambarella.com> created file
 *
 * Coryright (c) 2008-2012, Ambarella, Inc.
 * http://www.ambarella.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/dma-mapping.h>
#include <mach/hardware.h>
#include <plat/dma.h>

struct resource ambarella_dma_resources[] = {
	[0] = {
		.start	= DMA_BASE,
		.end	= DMA_BASE + 0x0FFF,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= DMA_IRQ,
		.end	= DMA_IRQ,
		.flags	= IORESOURCE_IRQ,
	},
};

struct platform_device ambarella_dma = {
	.name		= "ambarella-dma",
	.id		= 0,
	.resource	= ambarella_dma_resources,
	.num_resources	= ARRAY_SIZE(ambarella_dma_resources),
	.dev		= {
		.dma_mask		= &ambarella_dmamask,
		.coherent_dma_mask	= DMA_BIT_MASK(32),
	}
};
