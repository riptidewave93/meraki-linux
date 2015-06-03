/*
 * arch/arm/mach-ixp4xx/mr58-setup.c
 *
 * Meraki MR58 board setup
 *
 * Copyright (C) 2011 Meraki, Inc
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

#include <asm/types.h>
#include <asm/setup.h>
#include <asm/memory.h>
#include <mach/hardware.h>
#include <asm/mach-types.h>
#include <asm/irq.h>
#include <asm/mach/arch.h>
#include <asm/mach/flash.h>

static struct flash_platform_data mr58_flash_data = {
	.map_name	= "cfi_probe",
	.width		= 2,
};

static struct resource mr58_flash_resource = {
	.flags		= IORESOURCE_MEM,
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
	.gpio_base = 32,
};

static struct i2c_board_info __initdata mr58_i2c_devices[] = {
	{
		I2C_BOARD_INFO("pca9534", 0x38),
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
        &mac0,
        &mac1,
};

static void __init mr58_init(void)
{
	u32 data;
	int i;
	/* WP188 support, from Tian Yee
	 * WP188 has 2 RTL8201 (phy addr = 1) in place of the Marvell switch */
	data = (1<<31) | (1<<21) | (3<<16);
	// write mdio cmd reg C800908(i*4)
	for (i = 0; i < 4; i++) {
		*(u32 *) (IXP4XX_EthB_BASE_VIRT + 0x80 + i*4) = data & 0xff;
		data >>=8;
	}
	// poll mdio status regs at c800909(i*4) 1250 times
	for (i = 0; i < 5000; i++) {
		data = (*(u32 *) (IXP4XX_EthB_BASE_VIRT + 0x90 + i*4) & 0xff) << (i*8);
	}
	// finally read them again - kinda ghetto don't really need this...
	for (i = 0; i < 4; i++) {
		data |= (*(u32 *) (IXP4XX_EthB_BASE_VIRT + 0x90 + i*4) & 0xFF) << (i*8);
	}
	data &= 0xffff;
	if (data == 0x8201) {
		plat_mac0.phy = 1;
	}

	ixp4xx_sys_init();

	mr58_flash_resource.start = IXP4XX_EXP_BUS_BASE(0);
	mr58_flash_resource.end =
		IXP4XX_EXP_BUS_BASE(0) + SZ_32M - 1;

    //set gpios for 'abner' led board
#define ABNER_PIN 1
	gpio_line_config(ABNER_PIN, IXP4XX_GPIO_OUT);
	gpio_line_set(ABNER_PIN, IXP4XX_GPIO_HIGH);

    // Register the devices

	platform_add_devices(mr58_devices, ARRAY_SIZE(mr58_devices));
	i2c_register_board_info(0, mr58_i2c_devices,
				ARRAY_SIZE(mr58_i2c_devices));
}

#ifdef CONFIG_MACH_MERAKI_MR58
MACHINE_START(MERAKI_MR58, "Meraki MR58")
	/* Maintainer: Kevin Paul Herbert <kph@meraki.net> */
	.phys_io	= IXP4XX_PERIPHERAL_BASE_PHYS,
	.io_pg_offst	= ((IXP4XX_PERIPHERAL_BASE_VIRT) >> 18) & 0xfffc,
	.map_io		= ixp4xx_map_io,
	.init_irq	= ixp4xx_init_irq,
	.timer		= &ixp4xx_timer,
	.boot_params	= 0x0100,
	.init_machine	= mr58_init,
MACHINE_END

  /* The following hack is necessary for RedBoot compatibility. When
   * we update RedBoot to pass a proper machine type for the MR58
   * this should be removed.
   */

MACHINE_START(COMPEXWP18, "Meraki MR58")
	/* Maintainer: Kevin Paul Herbert <kph@meraki.net> */
	.phys_io	= IXP4XX_PERIPHERAL_BASE_PHYS,
	.io_pg_offst	= ((IXP4XX_PERIPHERAL_BASE_VIRT) >> 18) & 0xfffc,
	.map_io		= ixp4xx_map_io,
	.init_irq	= ixp4xx_init_irq,
	.timer		= &ixp4xx_timer,
	.boot_params	= 0x0100,
	.init_machine	= mr58_init,
MACHINE_END
#endif
