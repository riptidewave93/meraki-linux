/*
 * arch/arm/mach-ixp4xx/mr58-setup.c
 *
 * Meraki MR58 board setup
 *
 * Copyright (C) 2012 Meraki, Inc
 *
 * based on ixdp425-setup.c:
 *	Copyright (C) 2003-2005 MontaVista Software, Inc.
 *
 * Author: Kevin Paul Herbert <kph@meraki.net>
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/serial.h>
#include <linux/tty.h>
#include <linux/serial_8250.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/i2c-gpio.h>
#include <linux/i2c/pca953x.h>
#include <linux/io.h>
#include <linux/gpio.h>
#include <linux/leds.h>
#include <linux/dma-mapping.h>

#include <asm/types.h>
#include <asm/setup.h>
#include <asm/memory.h>
#include <mach/hardware.h>
#include <asm/mach-types.h>
#include <asm/irq.h>
#include <asm/mach/arch.h>
#include <asm/mach/flash.h>

#define MR58_GPIO_ABNER 1
#define MR58_GPIO_EXP_BASE 32

static struct flash_platform_data mr58_flash_data = {
	.map_name	= "cfi_probe",
	.width		= 2,
};

static struct resource mr58_flash_resource = {
	.flags		= IORESOURCE_MEM,
	.start		= IXP4XX_EXP_BUS_BASE(0),
	.end		= IXP4XX_EXP_BUS_BASE(0) + SZ_32M - 1,
};

static struct platform_device mr58_flash = {
	.name		= "IXP4XX-Flash",
	.id		= 0,
	.dev		= {
		.platform_data = &mr58_flash_data,
	},
	.num_resources	= 1,
	.resource	= &mr58_flash_resource,
};

static struct pca953x_platform_data mr58_gpio_ext_pdata_0 = {
	.gpio_base	= MR58_GPIO_EXP_BASE,
	.irq_base	= -1,
};

static struct i2c_board_info __initdata mr58_i2c_devices[] = {
	{
		I2C_BOARD_INFO("pca9534", 0x38),
		.platform_data = &mr58_gpio_ext_pdata_0,
	},
	{
		I2C_BOARD_INFO("pca9534", 0x70),
		.platform_data = &mr58_gpio_ext_pdata_0,
	},
};

static struct i2c_gpio_platform_data meraki_sd_i2c_gpio_pins = {
	.sda_pin    = 6,
	.scl_pin    = 2,
	.udelay	= 10,
};

static struct platform_device mr58_i2c_controller = {
	.name       = "i2c-gpio",
	.id     = 0,
	.dev        = {
		.platform_data = &meraki_sd_i2c_gpio_pins,
	},
	.num_resources  = 0
};

static struct gpio_led mr58_leds_gpio[] = {
	{
		.name		= "power:orange",
		.gpio		= MR58_GPIO_EXP_BASE + 1,
		.active_low	= 1,
		.default_trigger = "default-on",
	},
	{
		.name		= "power:green",
		.gpio		= MR58_GPIO_EXP_BASE + 0,
		.active_low	= 1,
	},
	{
		.name		= "lan",
		.gpio		= MR58_GPIO_EXP_BASE + 2,
		.active_low	= 1,
	},
	{
		.name		= "ssi:0",
		.gpio		= MR58_GPIO_EXP_BASE + 3,
		.active_low	= 1,
	},
	{
		.name		= "ssi:1",
		.gpio		= MR58_GPIO_EXP_BASE + 4,
		.active_low	= 1,
	},
	{
		.name		= "ssi:2",
		.gpio		= MR58_GPIO_EXP_BASE + 5,
		.active_low	= 1,
	},
	{
		.name		= "ssi:3",
		.gpio		= MR58_GPIO_EXP_BASE + 6,
		.active_low	= 1,
	},
};

static struct gpio_led_platform_data mr58_led_data = {
	.num_leds		= ARRAY_SIZE(mr58_leds_gpio),
	.leds			= mr58_leds_gpio,
};

static struct platform_device mr58_led_device = {
	.name = "leds-gpio",
	.dev = {
		.platform_data = &mr58_led_data,
	},
};

static struct resource mr58_uart_resources[] = {
	{
		.start		= IXP4XX_UART1_BASE_PHYS,
		.end		= IXP4XX_UART1_BASE_PHYS + 0x0fff,
		.flags		= IORESOURCE_MEM
	},
	{
		.start		= IXP4XX_UART2_BASE_PHYS,
		.end		= IXP4XX_UART2_BASE_PHYS + 0x0fff,
		.flags		= IORESOURCE_MEM
	}
};

static struct plat_serial8250_port mr58_uart_data[] = {
	{
		.mapbase	= IXP4XX_UART1_BASE_PHYS,
		.membase	= (char *)IXP4XX_UART1_BASE_VIRT + REG_OFFSET,
		.irq		= IRQ_IXP4XX_UART1,
		.flags		= UPF_BOOT_AUTOCONF | UPF_SKIP_TEST,
		.iotype		= UPIO_MEM,
		.regshift	= 2,
		.uartclk	= IXP4XX_UART_XTAL,
	},
	{
		.mapbase	= IXP4XX_UART2_BASE_PHYS,
		.membase	= (char *)IXP4XX_UART2_BASE_VIRT + REG_OFFSET,
		.irq		= IRQ_IXP4XX_UART2,
		.flags		= UPF_BOOT_AUTOCONF | UPF_SKIP_TEST,
		.iotype		= UPIO_MEM,
		.regshift	= 2,
		.uartclk	= IXP4XX_UART_XTAL,
	},
	{ },
};

static struct platform_device mr58_uart = {
	.name			= "serial8250",
	.id			= PLAT8250_DEV_PLATFORM,
	.dev.platform_data	= mr58_uart_data,
	.num_resources		= 2,
	.resource		= mr58_uart_resources,
};

static struct eth_plat_info plat_mac0 = {
	.phy	      = 1,
	.rxq	      = 4,
	.txreadyq   = 20,
};

static struct eth_plat_info plat_mac1 = {
	.phy	      = 3,
	.rxq	      = 5,
	.txreadyq   = 21,
};

static struct platform_device mac0 = {
	.name           = "ixp4xx_eth",
	.id             = IXP4XX_ETH_NPEB,
	.dev.platform_data = &plat_mac0,
};

static struct platform_device mac1 = {
	.name           = "ixp4xx_eth",
	.id             = IXP4XX_ETH_NPEC,
	.dev.platform_data = &plat_mac1,
};


static struct platform_device *mr58_devices[] __initdata = {
	&mr58_flash,
	&mr58_uart,
	&mr58_i2c_controller,
	&mr58_led_device,
        &mac0,
        &mac1,
};

static void __init mr58_init(void)
{
	ixp4xx_sys_init();

	/* Enable the "abner" LED daughter card */
	gpio_request(MR58_GPIO_ABNER, "Abner LED board");
	gpio_direction_output(MR58_GPIO_ABNER, 1);

	/* Register the devices */

	platform_add_devices(mr58_devices, ARRAY_SIZE(mr58_devices));
	i2c_register_board_info(0, mr58_i2c_devices,
				ARRAY_SIZE(mr58_i2c_devices));

	/* Configure Timer select high on the daughter board */
	gpio_request(MR58_GPIO_EXP_BASE + 7, "Timer select");
	gpio_direction_output(MR58_GPIO_EXP_BASE + 7, 1);
}

void __init mr58_map_io(void)
{
	ixp4xx_map_io();
	init_consistent_dma_size(SZ_4M);
}

#ifdef CONFIG_MACH_MERAKI_MR58
MACHINE_START(MERAKI_MR58, "Meraki MR58")
	/* Maintainer: Kevin Paul Herbert <kph@meraki.net> */
	.atag_offset	= 0x100,
	.map_io		= mr58_map_io,
	.init_early	= ixp4xx_init_early,
	.init_irq	= ixp4xx_init_irq,
	.timer		= &ixp4xx_timer,
	.init_machine	= mr58_init,
#if defined(CONFIG_PCI)
	.dma_zone_size	= SZ_64M,
#endif
	.restart	= ixp4xx_restart,
MACHINE_END

  /* The following hack is necessary for RedBoot compatibility. When
   * we update RedBoot to pass a proper machine type for the MR58
   * this should be removed.
   */

MACHINE_START(COMPEXWP18, "Meraki MR58")
	/* Maintainer: Kevin Paul Herbert <kph@meraki.net> */
	.atag_offset	= 0x100,
	.map_io		= mr58_map_io,
	.init_early	= ixp4xx_init_early,
	.init_irq	= ixp4xx_init_irq,
	.timer		= &ixp4xx_timer,
	.init_machine	= mr58_init,
#if defined(CONFIG_PCI)
	.dma_zone_size	= SZ_64M,
#endif
	.restart	= ixp4xx_restart,
MACHINE_END
#endif
