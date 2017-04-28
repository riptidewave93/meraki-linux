/*
 * arch/arm/plat-ambarella/generic/uart.c
 *
 * Author: Anthony Ginger <hfjiang@ambarella.com>
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
#include <linux/serial_core.h>
#include <linux/clk.h>

#include <mach/hardware.h>
#include <plat/uart.h>
#include <plat/clk.h>

/* ==========================================================================*/
#ifdef MODULE_PARAM_PREFIX
#undef MODULE_PARAM_PREFIX
#endif
#define MODULE_PARAM_PREFIX	"ambarella_config."

/* ==========================================================================*/
#if defined(CONFIG_PLAT_AMBARELLA_SUPPORT_HAL)
#else
static struct clk gclk_uart = {
	.parent		= NULL,
	.name		= "gclk_uart",
	.rate		= 0,
	.frac_mode	= 0,
	.ctrl_reg	= PLL_REG_UNAVAILABLE,
	.pres_reg	= PLL_REG_UNAVAILABLE,
	.post_reg	= CG_UART_REG,
	.frac_reg	= PLL_REG_UNAVAILABLE,
	.ctrl2_reg	= PLL_REG_UNAVAILABLE,
	.ctrl3_reg	= PLL_REG_UNAVAILABLE,
	.lock_reg	= PLL_REG_UNAVAILABLE,
	.lock_bit	= 0,
	.divider	= 0,
	.max_divider	= (1 << 24) - 1,
	.extra_scaler	= 0,
	.ops		= &ambarella_rct_pll_ops,
};

static struct clk *ambarella_uart_register_clk(void)
{
	struct clk *pgclk_uart = NULL;

	pgclk_uart = clk_get(NULL, "gclk_uart");
	if (IS_ERR(pgclk_uart)) {
		ambarella_register_clk(&gclk_uart);
		pgclk_uart = &gclk_uart;
		pr_info("SYSCLK:UART[%lu]\n", clk_get_rate(pgclk_uart));
	}

	return pgclk_uart;
}
#endif

static void ambarella_uart_set_pll(void)
{
/* Set prop UART clock in BSP, don't set clock by default in normal mode */
#if defined(CONFIG_AMBARELLA_RAW_BOOT)
	u32 uart_freq;
#if (CHIP_REV == A8)
	uart_freq = (REF_CLK_FREQ);
#else
	uart_freq = (115200 * 16);
#endif
#if defined(CONFIG_PLAT_AMBARELLA_SUPPORT_HAL)
	amb_set_uart_clock_frequency(HAL_BASE_VP, uart_freq);
#else
	clk_set_rate(ambarella_uart_register_clk(), uart_freq);
#endif
#endif
}

static u32 ambarella_uart_get_pll(void)
{
#if defined(CONFIG_PLAT_AMBARELLA_SUPPORT_HAL)
	return (u32)amb_get_uart_clock_frequency(HAL_BASE_VP);
#else
	return clk_get_rate(ambarella_uart_register_clk());
#endif
}

/* ==========================================================================*/
static struct uart_port	ambarella_uart_port_resource[] = {
	[0] = {
		.type		= PORT_UART00,
		.iotype		= UPIO_MEM,
		.membase	= (void *)UART0_BASE,
		.mapbase	= (unsigned long)(UART0_BASE -
				APB_BASE + APB_PHYS_BASE),
		.irq		= UART0_IRQ,
		.uartclk	= 27000000,
		.fifosize	= UART_FIFO_SIZE,
		.line		= 0,
	},
#if (UART_INSTANCES >= 2)
	[1] = {
		.type		= PORT_UART00,
		.iotype		= UPIO_MEM,
		.membase	= (void *)UART1_BASE,
		.mapbase	= (unsigned long)(UART1_BASE -
				APB_BASE + APB_PHYS_BASE),
		.irq		= UART1_IRQ,
		.uartclk	= 27000000,
		.fifosize	= UART_FIFO_SIZE,
		.line		= 0,
	},
#endif
#if (UART_INSTANCES >= 3)
	[2] = {
		.type		= PORT_UART00,
		.iotype		= UPIO_MEM,
		.membase	= (void *)UART2_BASE,
		.mapbase	= (unsigned long)(UART2_BASE -
				APB_BASE + APB_PHYS_BASE),
		.irq		= UART2_IRQ,
		.uartclk	= 27000000,
		.fifosize	= UART_FIFO_SIZE,
		.line		= 0,
	},
#endif
#if (UART_INSTANCES >= 4)
	[3] = {
		.type		= PORT_UART00,
		.iotype		= UPIO_MEM,
		.membase	= (void *)UART3_BASE,
		.mapbase	= (unsigned long)(UART3_BASE -
				APB_BASE + APB_PHYS_BASE),
		.irq		= UART3_IRQ,
		.uartclk	= 27000000,
		.fifosize	= UART_FIFO_SIZE,
		.line		= 0,
	},
#endif
};

#if (CHIP_REV == I1 || CHIP_REV == A8 || CHIP_REV == A7L)
static void ambarella_uart_stop_tx(unsigned char __iomem *membase)
{
	u32					ier;
	u32					iir;

	ier = amba_readl(membase + UART_IE_OFFSET);
	if ((ier & UART_IE_PTIME) != UART_IE_PTIME)
		amba_writel(membase + UART_IE_OFFSET,
			ier | (UART_IE_PTIME | UART_IE_ETBEI));
	iir = amba_readl(membase + UART_II_OFFSET);
	amba_writel(membase + UART_IE_OFFSET,
		ier & ~(UART_IE_PTIME | UART_IE_ETBEI));
	(void)(iir);
}

static u32 ambarella_uart_read_ms(unsigned char __iomem *membase)
{
	u32					ier;
	u32					ms;

	ier = amba_readl(membase + UART_IE_OFFSET);
	if ((ier & UART_IE_EDSSI) != UART_IE_EDSSI)
		amba_writel(membase + UART_IE_OFFSET, ier | UART_IE_EDSSI);
	ms = amba_readl(membase + UART_MS_OFFSET);
	if ((ier & UART_IE_EDSSI) != UART_IE_EDSSI)
		amba_writel(membase + UART_IE_OFFSET, ier);

	return ms;
}
#else
static void ambarella_uart_stop_tx(unsigned char __iomem *membase)
{
	amba_clrbitsl(membase + UART_IE_OFFSET, UART_IE_ETBEI);
}

#if (UART_INSTANCES >= 2)
static u32 ambarella_uart_read_ms(unsigned char __iomem *membase)
{
	return amba_readl(membase + UART_MS_OFFSET);
}
#endif
#endif

struct ambarella_uart_platform_info ambarella_uart_ports = {
	.total_port_num		= ARRAY_SIZE(ambarella_uart_port_resource),
	.registed_port_num	= 0,
	.amba_port[0]		= {
		.port		= &ambarella_uart_port_resource[0],
		.mcr		= DEFAULT_AMBARELLA_UART_MCR,
		.fcr		= DEFAULT_AMBARELLA_UART_FCR,
		.ier		= DEFAULT_AMBARELLA_UART_IER,
		.stop_tx	= ambarella_uart_stop_tx,
		.set_pll	= ambarella_uart_set_pll,
		.get_pll	= ambarella_uart_get_pll,
		.get_ms		= NULL,
		.first_send_num	= DEFAULT_AMBARELLA_UART_FIRST_SEND_NUM,
	},
#if (UART_INSTANCES >= 2)
	.amba_port[1]		= {
		.port		= &ambarella_uart_port_resource[1],
		.mcr		= DEFAULT_AMBARELLA_UART_MCR,
		.fcr		= DEFAULT_AMBARELLA_UART_FCR,
		.ier		= DEFAULT_AMBARELLA_UART_IER,
		.stop_tx	= ambarella_uart_stop_tx,
		.set_pll	= ambarella_uart_set_pll,
		.get_pll	= ambarella_uart_get_pll,
		.get_ms		= ambarella_uart_read_ms,
		.first_send_num	= DEFAULT_AMBARELLA_UART_FIRST_SEND_NUM,
	},
#endif
#if (UART_INSTANCES >= 3)
	.amba_port[2]		= {
		.port		= &ambarella_uart_port_resource[2],
		.mcr		= DEFAULT_AMBARELLA_UART_MCR,
		.fcr		= DEFAULT_AMBARELLA_UART_FCR,
		.ier		= DEFAULT_AMBARELLA_UART_IER,
		.stop_tx	= ambarella_uart_stop_tx,
		.set_pll	= ambarella_uart_set_pll,
		.get_pll	= ambarella_uart_get_pll,
		.get_ms		= ambarella_uart_read_ms,
		.first_send_num	= DEFAULT_AMBARELLA_UART_FIRST_SEND_NUM,
	},
#endif
#if (UART_INSTANCES >= 4)
	.amba_port[3]		= {
		.port		= &ambarella_uart_port_resource[3],
		.mcr		= DEFAULT_AMBARELLA_UART_MCR,
		.fcr		= DEFAULT_AMBARELLA_UART_FCR,
		.ier		= DEFAULT_AMBARELLA_UART_IER,
		.stop_tx	= ambarella_uart_stop_tx,
		.set_pll	= ambarella_uart_set_pll,
		.get_pll	= ambarella_uart_get_pll,
		.get_ms		= ambarella_uart_read_ms,
		.first_send_num	= DEFAULT_AMBARELLA_UART_FIRST_SEND_NUM,
	},
#endif
};

static const char ambarella_uart_platform_device_name[] = "ambarella-uart";

struct platform_device ambarella_uart = {
	.name		= ambarella_uart_platform_device_name,
	.id		= 0,
	.resource	= NULL,
	.num_resources	= 0,
	.dev		= {
		.platform_data		= &ambarella_uart_ports,
		.dma_mask		= &ambarella_dmamask,
		.coherent_dma_mask	= DMA_BIT_MASK(32),
	}
};
#if (UART_INSTANCES >= 2)
struct platform_device ambarella_uart1 = {
	.name		= ambarella_uart_platform_device_name,
	.id		= 1,
	.resource	= NULL,
	.num_resources	= 0,
	.dev		= {
		.platform_data		= &ambarella_uart_ports,
		.dma_mask		= &ambarella_dmamask,
		.coherent_dma_mask	= DMA_BIT_MASK(32),
	}
};
#endif
#if (UART_INSTANCES >= 3)
struct platform_device ambarella_uart2 = {
	.name		= ambarella_uart_platform_device_name,
	.id		= 2,
	.resource	= NULL,
	.num_resources	= 0,
	.dev		= {
		.platform_data		= &ambarella_uart_ports,
		.dma_mask		= &ambarella_dmamask,
		.coherent_dma_mask	= DMA_BIT_MASK(32),
	}
};
#endif
#if (UART_INSTANCES >= 4)
struct platform_device ambarella_uart3 = {
	.name		= ambarella_uart_platform_device_name,
	.id		= 3,
	.resource	= NULL,
	.num_resources	= 0,
	.dev		= {
		.platform_data		= &ambarella_uart_ports,
		.dma_mask		= &ambarella_dmamask,
		.coherent_dma_mask	= DMA_BIT_MASK(32),
	}
};
#endif

