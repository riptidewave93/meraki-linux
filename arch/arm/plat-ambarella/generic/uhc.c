/*
 * arch/arm/plat-ambarella/generic/uhc.c
 *
 * Author: Cao Rongrong <rrcao@ambarella.com>
 *
 * Copyright (C) 2004-2010, Ambarella, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/dma-mapping.h>
#include <linux/io.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <mach/hardware.h>
#include <mach/board.h>
#include <plat/uport.h>
#include <plat/uhc.h>

static atomic_t usb_host_initialized = ATOMIC_INIT(0);

static void ambarella_enable_usb_host(struct ambarella_uhc_controller *pdata)
{
	unsigned long flags;
	u32 pin_set, pin_clr;
#if !defined(CONFIG_PLAT_AMBARELLA_SUPPORT_HAL)
	u32 val;
#endif

	if (atomic_inc_return(&usb_host_initialized) > 1)
		return;

#if (CHIP_REV == AMB_S2)
	/* Determine the polarity of over-current protect pin
	 * bit16 == 1: high is normal, low will trigger the ocp interrupt
	 * bit16 == 0: row is normal, high will trigger the ocp interrupt */
	if ((ambarella_board_generic.uhc_use_ocp & (0x1<<16)) == 0)
		amba_setbitsl(AHB_SCRATCHPAD_REG(0xc), 0x1<<13);
	else
		amba_clrbitsl(AHB_SCRATCHPAD_REG(0xc), 0x1<<13);
#endif

	pin_set = 0;
	pin_clr = 0;

#if (CHIP_REV == I1)
	if (ambarella_board_generic.board_poc & USB1_IS_HOST)
		pdata->usb1_is_host = 1;
	else
		pdata->usb1_is_host = 0;

	/* GPIO8 and GPIO10 are programmed as hardware mode */
	if (pdata->usb1_is_host) {
		pin_set |= 0x1 << 10;
		if ((ambarella_board_generic.uhc_use_ocp & 0x2) == 0x2)
			pin_set |= 0x1 << 8;
		else
			pin_clr |= 0x1 << 8;
	}
#endif

	/* GPIO7 and GPIO9 are programmed as hardware mode */
	pin_set |= 0x1 << 9;
	if ((ambarella_board_generic.uhc_use_ocp & 0x1) == 0x1)
		pin_set |= 0x1 << 7;
	else
		pin_clr |= 0x1 << 7;
	ambarella_gpio_raw_lock(0, &flags);
	amba_clrbitsl(GPIO3_AFSEL_REG, 0x3);
	amba_setbitsl(GPIO0_AFSEL_REG, pin_set);
	amba_clrbitsl(GPIO0_AFSEL_REG, pin_clr);
	ambarella_gpio_raw_unlock(0, &flags);

	ambarella_enable_usb_port(UHC_OWN_PORT);

#if defined(CONFIG_PLAT_AMBARELLA_SUPPORT_HAL)
	/* Reset usb host controller */
	if (amb_usb_host_soft_reset(HAL_BASE_VP) != AMB_HAL_SUCCESS)
		pr_info("%s: amb_usb_host_soft_reset fail!\n", __func__);
#else
	val = amba_rct_readl(ANA_PWR_REG);
	/* force usbphy on */
	amba_rct_writel(ANA_PWR_REG, val | 0x4);
	udelay(1);
	/* UHC soft reset */
	amba_rct_setbitsl(USB_REFCLK_REG, 0x80000000);
	udelay(1);
	amba_rct_clrbitsl(USB_REFCLK_REG, 0x80000000);
	udelay(1);
	/* restore ana_pwr_reg */
	amba_rct_writel(ANA_PWR_REG, val);
	udelay(1);
#endif
}

static void ambarella_disable_usb_host(struct ambarella_uhc_controller *pdata)
{
	if (atomic_dec_return(&usb_host_initialized) > 0)
		return;

	ambarella_disable_usb_port(UHC_OWN_PORT);
}



/* ==========================================================================*/
struct resource ambarella_ehci_resources[] = {
	[0] = {
		.start	= USB_HOST_CTRL_EHCI_BASE,
		.end	= USB_HOST_CTRL_EHCI_BASE + 0xFFF,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= USB_EHCI_IRQ,
		.end	= USB_EHCI_IRQ,
		.flags	= IORESOURCE_IRQ,
	},
};


static struct ambarella_uhc_controller ambarella_platform_ehci_data = {
	.enable_host	= ambarella_enable_usb_host,
	.disable_host	= ambarella_disable_usb_host,
	.usb1_is_host	= -1,
	.irqflags	= IRQF_TRIGGER_HIGH,
};

struct platform_device ambarella_ehci0 = {
	.name		= "ambarella-ehci",
	.id		= -1,
	.resource	= ambarella_ehci_resources,
	.num_resources	= ARRAY_SIZE(ambarella_ehci_resources),
	.dev		= {
		.platform_data		= &ambarella_platform_ehci_data,
		.dma_mask		= &ambarella_dmamask,
		.coherent_dma_mask	= DMA_BIT_MASK(32),
	}
};

struct resource ambarella_ohci_resources[] = {
	[0] = {
		.start	= USB_HOST_CTRL_OHCI_BASE,
		.end	= USB_HOST_CTRL_OHCI_BASE + 0xFFF,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= USB_OHCI_IRQ,
		.end	= USB_OHCI_IRQ,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct ambarella_uhc_controller ambarella_platform_ohci_data = {
	.enable_host	= ambarella_enable_usb_host,
	.disable_host	= ambarella_disable_usb_host,
#if (CHIP_REV == I1)
	.irqflags	= IRQF_TRIGGER_LOW,
#else
	.irqflags	= IRQF_TRIGGER_HIGH,
#endif
};

struct platform_device ambarella_ohci0 = {
	.name		= "ambarella-ohci",
	.id		= -1,
	.resource	= ambarella_ohci_resources,
	.num_resources	= ARRAY_SIZE(ambarella_ohci_resources),
	.dev		= {
		.platform_data		= &ambarella_platform_ohci_data,
		.dma_mask		= &ambarella_dmamask,
		.coherent_dma_mask	= DMA_BIT_MASK(32),
	}
};

