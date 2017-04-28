/*
 * omap_hwmod_ti81xx_data.c - hardware modules data for TI81XX chips
 *
 * Copyright (C) 2010 Texas Instruments, Inc. - http://www.ti.com/
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation version 2.
 *
 * This program is distributed "as is" WITHOUT ANY WARRANTY of any
 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/gpio.h>

#include <plat/omap_hwmod.h>
#include <mach/irqs.h>
#include <plat/cpu.h>
#include <plat/dma.h>
#include <plat/serial.h>
#include <plat/l4_3xxx.h>
#include <plat/ti81xx.h>

#include "omap_hwmod_common_data.h"

#include "control.h"
#include "cm81xx.h"
#include "cm-regbits-81xx.h"

/*
 * TI816X hardware modules integration data
 *
 * Note: This is incomplete and at present, not generated from h/w database.
 *
 * TODO: Add EDMA in the 'user' field wherever applicable.
 */

static struct omap_hwmod ti816x_mpu_hwmod;
static struct omap_hwmod ti816x_l3_slow_hwmod;
static struct omap_hwmod ti816x_l4_slow_hwmod;

/* L3 SLOW -> L4_SLOW Peripheral interface */
static struct omap_hwmod_ocp_if ti816x_l3_slow__l4_slow = {
	.master	= &ti816x_l3_slow_hwmod,
	.slave	= &ti816x_l4_slow_hwmod,
	.user	= OCP_USER_MPU,
};

/* MPU -> L3 SLOW interface */
static struct omap_hwmod_ocp_if ti816x_mpu__l3_slow = {
	.master = &ti816x_mpu_hwmod,
	.slave	= &ti816x_l3_slow_hwmod,
	.user	= OCP_USER_MPU,
};

/* Slave interfaces on the L3 SLOW interconnect */
static struct omap_hwmod_ocp_if *ti816x_l3_slow_slaves[] = {
	&ti816x_mpu__l3_slow,
};

/* Master interfaces on the L3 SLOW interconnect */
static struct omap_hwmod_ocp_if *ti816x_l3_slow_masters[] = {
	&ti816x_l3_slow__l4_slow,
};

/* L3 SLOW */
static struct omap_hwmod ti816x_l3_slow_hwmod = {
	.name		= "l3_slow",
	.class		= &l3_hwmod_class,
	.masters	= ti816x_l3_slow_masters,
	.masters_cnt	= ARRAY_SIZE(ti816x_l3_slow_masters),
	.slaves		= ti816x_l3_slow_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti816x_l3_slow_slaves),
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI81XX),
	.flags		= HWMOD_NO_IDLEST,
};

static struct omap_hwmod ti816x_uart1_hwmod;
static struct omap_hwmod ti816x_uart2_hwmod;
static struct omap_hwmod ti816x_uart3_hwmod;
static struct omap_hwmod ti814x_uart4_hwmod;
static struct omap_hwmod ti814x_uart5_hwmod;
static struct omap_hwmod ti814x_uart6_hwmod;
static struct omap_hwmod ti816x_wd_timer2_hwmod;
static struct omap_hwmod ti814x_wd_timer1_hwmod;
static struct omap_hwmod ti81xx_i2c1_hwmod;
static struct omap_hwmod ti816x_i2c2_hwmod;
static struct omap_hwmod ti814x_i2c3_hwmod;
static struct omap_hwmod ti814x_i2c4_hwmod;
static struct omap_hwmod ti81xx_gpio1_hwmod;
static struct omap_hwmod ti81xx_gpio2_hwmod;
static struct omap_hwmod ti814x_gpio3_hwmod;
static struct omap_hwmod ti814x_gpio4_hwmod;
static struct omap_hwmod ti814x_gpio5_hwmod;
static struct omap_hwmod ti814x_gpio6_hwmod;
static struct omap_hwmod ti81xx_usbss_hwmod;
static struct omap_hwmod ti81xx_elm_hwmod;
struct omap_hwmod_ocp_if ti81xx_l4_slow__elm;

/* L4 SLOW -> UART1 interface */
static struct omap_hwmod_addr_space ti816x_uart1_addr_space[] = {
	{
		.pa_start	= TI81XX_UART1_BASE,
		.pa_end		= TI81XX_UART1_BASE + SZ_8K - 1,
		.flags		= ADDR_MAP_ON_INIT | ADDR_TYPE_RT,
	},
};

static struct omap_hwmod_ocp_if ti816x_l4_slow__uart1 = {
	.master		= &ti816x_l4_slow_hwmod,
	.slave		= &ti816x_uart1_hwmod,
	.clk		= "uart1_ick",
	.addr		= ti816x_uart1_addr_space,
	.addr_cnt	= ARRAY_SIZE(ti816x_uart1_addr_space),
	.user		= OCP_USER_MPU,
};

/* L4 SLOW -> UART2 interface */
static struct omap_hwmod_addr_space ti816x_uart2_addr_space[] = {
	{
		.pa_start	= TI81XX_UART2_BASE,
		.pa_end		= TI81XX_UART2_BASE + SZ_8K - 1,
		.flags		= ADDR_MAP_ON_INIT | ADDR_TYPE_RT,
	},
};

static struct omap_hwmod_ocp_if ti816x_l4_slow__uart2 = {
	.master		= &ti816x_l4_slow_hwmod,
	.slave		= &ti816x_uart2_hwmod,
	.clk		= "uart2_ick",
	.addr		= ti816x_uart2_addr_space,
	.addr_cnt	= ARRAY_SIZE(ti816x_uart2_addr_space),
	.user		= OCP_USER_MPU,
};

/* L4 SLOW -> UART3 interface */
static struct omap_hwmod_addr_space ti816x_uart3_addr_space[] = {
	{
		.pa_start	= TI81XX_UART3_BASE,
		.pa_end		= TI81XX_UART3_BASE + SZ_8K - 1,
		.flags		= ADDR_MAP_ON_INIT | ADDR_TYPE_RT,
	},
};

static struct omap_hwmod_ocp_if ti816x_l4_slow__uart3 = {
	.master		= &ti816x_l4_slow_hwmod,
	.slave		= &ti816x_uart3_hwmod,
	.clk		= "uart3_ick",
	.addr		= ti816x_uart3_addr_space,
	.addr_cnt	= ARRAY_SIZE(ti816x_uart3_addr_space),
	.user		= OCP_USER_MPU,
};

/* L4 SLOW -> UART4 interface */
static struct omap_hwmod_addr_space ti814x_uart4_addr_space[] = {
	{
		.pa_start	= TI814X_UART4_BASE,
		.pa_end		= TI814X_UART4_BASE + SZ_8K - 1,
		.flags		= ADDR_MAP_ON_INIT | ADDR_TYPE_RT,
	},
};

static struct omap_hwmod_ocp_if ti814x_l4_slow__uart4 = {
	.master		= &ti816x_l4_slow_hwmod,
	.slave		= &ti814x_uart4_hwmod,
	.clk		= "uart4_ick",
	.addr		= ti814x_uart4_addr_space,
	.addr_cnt	= ARRAY_SIZE(ti814x_uart4_addr_space),
	.user		= OCP_USER_MPU,
};

/* L4 SLOW -> UART5 interface */
static struct omap_hwmod_addr_space ti814x_uart5_addr_space[] = {
	{
		.pa_start	= TI814X_UART5_BASE,
		.pa_end		= TI814X_UART5_BASE + SZ_8K - 1,
		.flags		= ADDR_MAP_ON_INIT | ADDR_TYPE_RT,
	},
};

static struct omap_hwmod_ocp_if ti814x_l4_slow__uart5 = {
	.master		= &ti816x_l4_slow_hwmod,
	.slave		= &ti814x_uart5_hwmod,
	.clk		= "uart5_ick",
	.addr		= ti814x_uart5_addr_space,
	.addr_cnt	= ARRAY_SIZE(ti814x_uart5_addr_space),
	.user		= OCP_USER_MPU,
};

/* L4 SLOW -> UART6 interface */
static struct omap_hwmod_addr_space ti814x_uart6_addr_space[] = {
	{
		.pa_start	= TI814X_UART6_BASE,
		.pa_end		= TI814X_UART6_BASE + SZ_8K - 1,
		.flags		= ADDR_MAP_ON_INIT | ADDR_TYPE_RT,
	},
};

static struct omap_hwmod_ocp_if ti814x_l4_slow__uart6 = {
	.master		= &ti816x_l4_slow_hwmod,
	.slave		= &ti814x_uart6_hwmod,
	.clk		= "uart6_ick",
	.addr		= ti814x_uart6_addr_space,
	.addr_cnt	= ARRAY_SIZE(ti814x_uart6_addr_space),
	.user		= OCP_USER_MPU,
};

/* L4 SLOW -> Watchdog */
static struct omap_hwmod_addr_space ti816x_wd_timer2_addrs[] = {
	{
		.pa_start	= 0x480C2000,
		.pa_end		= 0x480C2FFF,
		.flags		= ADDR_TYPE_RT,
	},
};

static struct omap_hwmod_addr_space ti814x_wd_timer1_addrs[] = {
	{
		.pa_start	= 0x481C7000,
		.pa_end		= 0x481C7FFF,
		.flags		= ADDR_TYPE_RT,
	},
};

static struct omap_hwmod_ocp_if ti816x_l4_slow__wd_timer2 = {
	.master		= &ti816x_l4_slow_hwmod,
	.slave		= &ti816x_wd_timer2_hwmod,
	.clk		= "wdt2_ick",
	.addr		= ti816x_wd_timer2_addrs,
	.addr_cnt	= ARRAY_SIZE(ti816x_wd_timer2_addrs),
	.user		= OCP_USER_MPU,
};

static struct omap_hwmod_ocp_if ti814x_l4_slow__wd_timer1 = {
	.master		= &ti816x_l4_slow_hwmod,
	.slave		= &ti814x_wd_timer1_hwmod,
	.clk		= "wdt1_ick",
	.addr		= ti814x_wd_timer1_addrs,
	.addr_cnt	= ARRAY_SIZE(ti814x_wd_timer1_addrs),
	.user		= OCP_USER_MPU,
};

/* L4 SLOW -> I2C1 */
static struct omap_hwmod_addr_space ti816x_i2c1_addr_space[] = {
	{
		.pa_start	= 0x48028000,
		.pa_end		= 0x48028000 + SZ_4K - 1,
		.flags		= ADDR_MAP_ON_INIT | ADDR_TYPE_RT,
	},
};

static struct omap_hwmod_ocp_if ti816x_l4_slow__i2c1 = {
	.master		= &ti816x_l4_slow_hwmod,
	.slave		= &ti81xx_i2c1_hwmod,
	.clk		= "i2c1_ick",
	.addr		= ti816x_i2c1_addr_space,
	.addr_cnt	= ARRAY_SIZE(ti816x_i2c1_addr_space),
	.user		= OCP_USER_MPU,
};

/* L4 SLOW -> I2C2 */
static struct omap_hwmod_addr_space ti816x_i2c2_addr_space[] = {
	{
		.pa_start	= 0x4802A000,
		.pa_end		= 0x4802A000 + SZ_4K - 1,
		.flags		= ADDR_MAP_ON_INIT | ADDR_TYPE_RT,
	},
};

static struct omap_hwmod_ocp_if ti816x_l4_slow__i2c2 = {
	.master		= &ti816x_l4_slow_hwmod,
	.slave		= &ti816x_i2c2_hwmod,
	.clk		= "i2c2_ick",
	.addr		= ti816x_i2c2_addr_space,
	.addr_cnt	= ARRAY_SIZE(ti816x_i2c2_addr_space),
	.user		= OCP_USER_MPU,
};

static struct omap_hwmod_addr_space ti814x_i2c3_addr_space[] = {
	{
		.pa_start	= 0x4819C000,
		.pa_end		= 0x4819C000 + SZ_4K - 1,
		.flags		= ADDR_MAP_ON_INIT | ADDR_TYPE_RT,
	},
};

static struct omap_hwmod_ocp_if ti814x_l4_slow__i2c3 = {
	.master		= &ti816x_l4_slow_hwmod,
	.slave		= &ti814x_i2c3_hwmod,
	.clk		= "i2c3_ick",
	.addr		= ti814x_i2c3_addr_space,
	.addr_cnt	= ARRAY_SIZE(ti814x_i2c3_addr_space),
	.user		= OCP_USER_MPU,
};

static struct omap_hwmod_addr_space ti814x_i2c4_addr_space[] = {
	{
		.pa_start       = 0x4819E000,
		.pa_end         = 0x4819E000 + SZ_4K - 1,
		.flags          = ADDR_MAP_ON_INIT | ADDR_TYPE_RT,
	},
};

static struct omap_hwmod_ocp_if ti814x_l4_slow__i2c4 = {
	.master         = &ti816x_l4_slow_hwmod,
	.slave          = &ti814x_i2c4_hwmod,
	.clk            = "i2c4_ick",
	.addr           = ti814x_i2c4_addr_space,
	.addr_cnt       = ARRAY_SIZE(ti814x_i2c4_addr_space),
	.user           = OCP_USER_MPU,
};



/* L4 SLOW -> GPIO1 */
static struct omap_hwmod_addr_space ti81xx_gpio1_addrs[] = {
	{
		.pa_start	= TI81XX_GPIO0_BASE,
		.pa_end		= TI81XX_GPIO0_BASE + SZ_4K - 1,
		.flags		= ADDR_MAP_ON_INIT | ADDR_TYPE_RT,
	},
};

static struct omap_hwmod_ocp_if ti81xx_l4_slow__gpio1 = {
	.master		= &ti816x_l4_slow_hwmod,
	.slave		= &ti81xx_gpio1_hwmod,
	.addr		= ti81xx_gpio1_addrs,
	.addr_cnt	= ARRAY_SIZE(ti81xx_gpio1_addrs),
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* L4 SLOW -> GPIO2 */
static struct omap_hwmod_addr_space ti81xx_gpio2_addrs[] = {
	{
		.pa_start	= TI81XX_GPIO1_BASE,
		.pa_end		= TI81XX_GPIO1_BASE + SZ_4K - 1,
		.flags		= ADDR_MAP_ON_INIT | ADDR_TYPE_RT,
	},
};

static struct omap_hwmod_ocp_if ti81xx_l4_slow__gpio2 = {
	.master		= &ti816x_l4_slow_hwmod,
	.slave		= &ti81xx_gpio2_hwmod,
	.addr		= ti81xx_gpio2_addrs,
	.addr_cnt	= ARRAY_SIZE(ti81xx_gpio2_addrs),
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* L4 SLOW -> GPIO3 */
static struct omap_hwmod_addr_space ti814x_gpio3_addrs[] = {
	{
		.pa_start	= TI814X_GPIO2_BASE,
		.pa_end		= TI814X_GPIO2_BASE + SZ_4K - 1,
		.flags		= ADDR_MAP_ON_INIT | ADDR_TYPE_RT,
	},
};

static struct omap_hwmod_ocp_if ti814x_l4_slow__gpio3 = {
	.master		= &ti816x_l4_slow_hwmod,
	.slave		= &ti814x_gpio3_hwmod,
	.addr		= ti814x_gpio3_addrs,
	.addr_cnt	= ARRAY_SIZE(ti814x_gpio3_addrs),
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* L4 SLOW -> GPIO4 */
static struct omap_hwmod_addr_space ti814x_gpio4_addrs[] = {
	{
		.pa_start	= TI814X_GPIO3_BASE,
		.pa_end		= TI814X_GPIO3_BASE + SZ_4K - 1,
		.flags		= ADDR_MAP_ON_INIT | ADDR_TYPE_RT,
	},
};

static struct omap_hwmod_ocp_if ti814x_l4_slow__gpio4 = {
	.master		= &ti816x_l4_slow_hwmod,
	.slave		= &ti814x_gpio4_hwmod,
	.addr		= ti814x_gpio4_addrs,
	.addr_cnt	= ARRAY_SIZE(ti814x_gpio4_addrs),
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* L4 SLOW -> GPIO5 */
static struct omap_hwmod_addr_space ti814x_gpio5_addrs[] = {
	{
		.pa_start	= TI811X_GPIO4_BASE,
		.pa_end		= TI811X_GPIO4_BASE + SZ_4K - 1,
		.flags		= ADDR_MAP_ON_INIT | ADDR_TYPE_RT,
	},
};

static struct omap_hwmod_ocp_if ti814x_l4_slow__gpio5 = {
	.master		= &ti816x_l4_slow_hwmod,
	.slave		= &ti814x_gpio5_hwmod,
	.addr		= ti814x_gpio5_addrs,
	.addr_cnt	= ARRAY_SIZE(ti814x_gpio5_addrs),
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* L4 SLOW -> GPIO6 */
static struct omap_hwmod_addr_space ti814x_gpio6_addrs[] = {
	{
		.pa_start	= TI811X_GPIO5_BASE,
		.pa_end		= TI811X_GPIO5_BASE + SZ_4K - 1,
		.flags		= ADDR_MAP_ON_INIT | ADDR_TYPE_RT,
	},
};

static struct omap_hwmod_ocp_if ti814x_l4_slow__gpio6 = {
	.master		= &ti816x_l4_slow_hwmod,
	.slave		= &ti814x_gpio6_hwmod,
	.addr		= ti814x_gpio6_addrs,
	.addr_cnt	= ARRAY_SIZE(ti814x_gpio6_addrs),
	.user		= OCP_USER_MPU | OCP_USER_SDMA,
};

/* Slave interfaces on the L4_SLOW interconnect */
static struct omap_hwmod_ocp_if *ti816x_l4_slow_slaves[] = {
	&ti816x_l3_slow__l4_slow,
};



/* Master interfaces on the L4_SLOW interconnect */
static struct omap_hwmod_ocp_if *ti816x_l4_slow_masters[] = {
	&ti816x_l4_slow__uart1,
	&ti816x_l4_slow__uart2,
	&ti816x_l4_slow__uart3,
	&ti814x_l4_slow__uart4,
	&ti814x_l4_slow__uart5,
	&ti814x_l4_slow__uart6,
	&ti816x_l4_slow__wd_timer2,
	&ti816x_l4_slow__i2c1,
	&ti816x_l4_slow__i2c2,
	&ti814x_l4_slow__i2c3,
	&ti814x_l4_slow__i2c4,
	&ti81xx_l4_slow__gpio1,
	&ti81xx_l4_slow__gpio2,
	&ti814x_l4_slow__gpio3,
	&ti814x_l4_slow__gpio4,
	&ti81xx_l4_slow__elm,
};

/* L4 SLOW */
static struct omap_hwmod ti816x_l4_slow_hwmod = {
	.name		= "l4_slow",
	.class		= &l4_hwmod_class,
	.masters	= ti816x_l4_slow_masters,
	.masters_cnt	= ARRAY_SIZE(ti816x_l4_slow_masters),
	.slaves		= ti816x_l4_slow_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti816x_l4_slow_slaves),
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI81XX),
	.flags		= HWMOD_NO_IDLEST,
};

/* Master interfaces on the MPU device */
static struct omap_hwmod_ocp_if *ti816x_mpu_masters[] = {
	&ti816x_mpu__l3_slow,
};

/* MPU */
static struct omap_hwmod ti816x_mpu_hwmod = {
	.name		= "mpu",
	.class		= &mpu_hwmod_class,
	.main_clk	= "mpu_ck",
	.vdd_name	= "mpu",
	.masters	= ti816x_mpu_masters,
	.masters_cnt	= ARRAY_SIZE(ti816x_mpu_masters),
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI81XX),
};

/* UART common */

static struct omap_hwmod_class_sysconfig uart_sysc = {
	.rev_offs	= 0x50,
	.sysc_offs	= 0x54,
	.syss_offs	= 0x58,
	.sysc_flags	= (SYSC_HAS_SIDLEMODE |
			   SYSC_HAS_ENAWAKEUP | SYSC_HAS_SOFTRESET |
			   SYSC_HAS_AUTOIDLE),
	.idlemodes	= (SIDLE_FORCE | SIDLE_NO | SIDLE_SMART),
	.sysc_fields    = &omap_hwmod_sysc_type1,
};

static struct omap_hwmod_class uart_class = {
	.name = "uart",
	.sysc = &uart_sysc,
};

/*
 * 'wd_timer' class
 * 32-bit watchdog upward counter that generates a pulse on the reset pin on
 * overflow condition
 */

static struct omap_hwmod_class_sysconfig wd_timer_sysc = {
	.rev_offs       = 0x0000,
	.sysc_offs      = 0x0010,
	.syss_offs      = 0x0014,
	.sysc_flags     = (SYSC_HAS_SIDLEMODE | SYSC_HAS_EMUFREE |
				SYSC_HAS_ENAWAKEUP | SYSC_HAS_SOFTRESET |
				SYSC_HAS_AUTOIDLE | SYSC_HAS_CLOCKACTIVITY),
	.idlemodes      = (SIDLE_FORCE | SIDLE_NO | SIDLE_SMART),
	.sysc_fields    = &omap_hwmod_sysc_type1,
};

static struct omap_hwmod_class wd_timer_class = {
	.name = "wd_timer",
	.sysc = &wd_timer_sysc,
};

/* I2C common */
static struct omap_hwmod_class_sysconfig i2c_sysc = {
	.rev_offs	= 0x0,
	.sysc_offs	= 0x10,
	.syss_offs	= 0x90,
	.sysc_flags	= (SYSC_HAS_SIDLEMODE |
			   SYSC_HAS_ENAWAKEUP | SYSC_HAS_SOFTRESET |
			   SYSC_HAS_AUTOIDLE),
	.idlemodes	= (SIDLE_FORCE | SIDLE_NO | SIDLE_SMART),
	.sysc_fields    = &omap_hwmod_sysc_type1,
};

static struct omap_hwmod_class i2c_class = {
	.name = "i2c",
	.sysc = &i2c_sysc,
};

/*
 * 'gpio' class
 * general purpose io module
 */
static struct omap_hwmod_class_sysconfig ti81xx_gpio_sysc = {
	.rev_offs	= 0x0000,
	.sysc_offs	= 0x0010,
	.syss_offs	= 0x0114,
	.sysc_flags	= (SYSC_HAS_AUTOIDLE | SYSC_HAS_ENAWAKEUP |
			   SYSC_HAS_SIDLEMODE | SYSC_HAS_SOFTRESET |
			   SYSS_HAS_RESET_STATUS),
	.idlemodes	= (SIDLE_FORCE | SIDLE_NO | SIDLE_SMART |
			   SIDLE_SMART_WKUP),
	.sysc_fields	= &omap_hwmod_sysc_type1,
};

static struct omap_hwmod_class ti81xx_gpio_hwmod_class = {
	.name	= "gpio",
	.sysc	= &ti81xx_gpio_sysc,
	.rev	= 2,
};

/* gpio dev_attr */
static struct omap_gpio_dev_attr gpio_dev_attr = {
	.bank_width	= 32,
	.dbck_flag	= true,
};

/* UART1 */

static struct omap_hwmod_irq_info uart1_mpu_irqs[] = {
	{ .irq = TI81XX_IRQ_UART0, },
};

/*
 * There is no SDMA on TI81XX, instead we have EDMA. Presently using dummy
 * channel numbers as the omap UART driver (drivers/serial/omap-serial.c)
 * requires these values to be filled in even if we don't have DMA enabled. Same
 * applies for other UARTs below.
 */
static struct omap_hwmod_dma_info uart1_edma_reqs[] = {
	{ .name = "tx",	.dma_req = 0, },
	{ .name = "rx",	.dma_req = 0, },
};

static struct omap_hwmod_ocp_if *ti816x_uart1_slaves[] = {
	&ti816x_l4_slow__uart1,
};

static struct omap_hwmod ti816x_uart1_hwmod = {
	.name		= "uart1",
	.mpu_irqs	= uart1_mpu_irqs,
	.mpu_irqs_cnt	= ARRAY_SIZE(uart1_mpu_irqs),
	.sdma_reqs	= uart1_edma_reqs,
	.sdma_reqs_cnt	= ARRAY_SIZE(uart1_edma_reqs),
	.main_clk	= "uart1_fck",
	.prcm		= {
		.omap4 = {
			.clkctrl_reg = TI81XX_CM_ALWON_UART_0_CLKCTRL,
		},
	},
	.slaves		= ti816x_uart1_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti816x_uart1_slaves),
	.class		= &uart_class,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI81XX),
};

/* UART2 */

static struct omap_hwmod_irq_info uart2_mpu_irqs[] = {
	{ .irq = TI81XX_IRQ_UART1, },
};

static struct omap_hwmod_dma_info uart2_edma_reqs[] = {
	{ .name = "tx",	.dma_req = 0, },
	{ .name = "rx",	.dma_req = 0, },
};

static struct omap_hwmod_ocp_if *ti816x_uart2_slaves[] = {
	&ti816x_l4_slow__uart2,
};

static struct omap_hwmod ti816x_uart2_hwmod = {
	.name		= "uart2",
	.mpu_irqs	= uart2_mpu_irqs,
	.mpu_irqs_cnt	= ARRAY_SIZE(uart2_mpu_irqs),
	.sdma_reqs	= uart2_edma_reqs,
	.sdma_reqs_cnt	= ARRAY_SIZE(uart2_edma_reqs),
	.main_clk	= "uart2_fck",
	.prcm		= {
		.omap4 = {
			.clkctrl_reg = TI81XX_CM_ALWON_UART_1_CLKCTRL,
		},
	},
	.slaves		= ti816x_uart2_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti816x_uart2_slaves),
	.class		= &uart_class,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI81XX),
};

/* UART3 */

static struct omap_hwmod_irq_info uart3_mpu_irqs[] = {
	{ .irq = TI81XX_IRQ_UART2, },
};

static struct omap_hwmod_dma_info uart3_edma_reqs[] = {
	{ .name = "tx",	.dma_req = 0, },
	{ .name = "rx",	.dma_req = 0, },
};

static struct omap_hwmod_ocp_if *ti816x_uart3_slaves[] = {
	&ti816x_l4_slow__uart3,
};

static struct omap_hwmod ti816x_uart3_hwmod = {
	.name		= "uart3",
	.mpu_irqs	= uart3_mpu_irqs,
	.mpu_irqs_cnt	= ARRAY_SIZE(uart3_mpu_irqs),
	.sdma_reqs	= uart3_edma_reqs,
	.sdma_reqs_cnt	= ARRAY_SIZE(uart3_edma_reqs),
	.main_clk	= "uart3_fck",
	.prcm		= {
		.omap4 = {
			.clkctrl_reg = TI81XX_CM_ALWON_UART_2_CLKCTRL,
		},
	},
	.slaves		= ti816x_uart3_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti816x_uart3_slaves),
	.class		= &uart_class,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI81XX),
};

/* UART4 */

static struct omap_hwmod_irq_info uart4_mpu_irqs[] = {
	{ .irq = TI814X_IRQ_UART3, },
};

static struct omap_hwmod_dma_info uart4_edma_reqs[] = {
	{ .name = "tx",	.dma_req = 0, },
	{ .name = "rx",	.dma_req = 0, },
};

static struct omap_hwmod_ocp_if *ti814x_uart4_slaves[] = {
	&ti814x_l4_slow__uart4,
};

static struct omap_hwmod ti814x_uart4_hwmod = {
	.name		= "uart4",
	.mpu_irqs	= uart4_mpu_irqs,
	.mpu_irqs_cnt	= ARRAY_SIZE(uart4_mpu_irqs),
	.sdma_reqs	= uart4_edma_reqs,
	.sdma_reqs_cnt	= ARRAY_SIZE(uart4_edma_reqs),
	.main_clk	= "uart4_fck",
	.prcm		= {
		.omap4 = {
			.clkctrl_reg = TI814X_CM_ALWON_UART_3_CLKCTRL,
		},
	},
	.slaves		= ti814x_uart4_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti814x_uart4_slaves),
	.class		= &uart_class,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI814X | CHIP_IS_TI811X),
};

/* UART5 */

static struct omap_hwmod_irq_info uart5_mpu_irqs[] = {
	{ .irq = TI814X_IRQ_UART4, },
};

static struct omap_hwmod_dma_info uart5_edma_reqs[] = {
	{ .name = "tx",	.dma_req = 0, },
	{ .name = "rx",	.dma_req = 0, },
};

static struct omap_hwmod_ocp_if *ti814x_uart5_slaves[] = {
	&ti814x_l4_slow__uart5,
};

static struct omap_hwmod ti814x_uart5_hwmod = {
	.name		= "uart5",
	.mpu_irqs	= uart5_mpu_irqs,
	.mpu_irqs_cnt	= ARRAY_SIZE(uart5_mpu_irqs),
	.sdma_reqs	= uart5_edma_reqs,
	.sdma_reqs_cnt	= ARRAY_SIZE(uart5_edma_reqs),
	.main_clk	= "uart5_fck",
	.prcm		= {
		.omap4 = {
			.clkctrl_reg = TI814X_CM_ALWON_UART_4_CLKCTRL,
		},
	},
	.slaves		= ti814x_uart5_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti814x_uart5_slaves),
	.class		= &uart_class,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI814X | CHIP_IS_TI811X),
};

/* UART6 */

static struct omap_hwmod_irq_info uart6_mpu_irqs[] = {
	{ .irq = TI814X_IRQ_UART5, },
};

static struct omap_hwmod_dma_info uart6_edma_reqs[] = {
	{ .name = "tx",	.dma_req = 0, },
	{ .name = "rx",	.dma_req = 0, },
};

static struct omap_hwmod_ocp_if *ti814x_uart6_slaves[] = {
	&ti814x_l4_slow__uart6,
};

static struct omap_hwmod ti814x_uart6_hwmod = {
	.name		= "uart6",
	.mpu_irqs	= uart6_mpu_irqs,
	.mpu_irqs_cnt	= ARRAY_SIZE(uart6_mpu_irqs),
	.sdma_reqs	= uart6_edma_reqs,
	.sdma_reqs_cnt	= ARRAY_SIZE(uart6_edma_reqs),
	.main_clk	= "uart6_fck",
	.prcm		= {
		.omap4 = {
			.clkctrl_reg = TI814X_CM_ALWON_UART_5_CLKCTRL,
		},
	},
	.slaves		= ti814x_uart6_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti814x_uart6_slaves),
	.class		= &uart_class,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI814X | CHIP_IS_TI811X),
};

/* Watchdog */

static struct omap_hwmod_ocp_if *ti816x_wd_timer2_slaves[] = {
	&ti816x_l4_slow__wd_timer2,
};

static struct omap_hwmod_ocp_if *ti814x_wd_timer1_slaves[] = {
	&ti814x_l4_slow__wd_timer1,
};

static struct omap_hwmod ti816x_wd_timer2_hwmod = {
	.name		= "wd_timer2",
	.main_clk	= "wdt2_fck",
	.prcm		= {
		.omap4 = {
			.clkctrl_reg = TI81XX_CM_ALWON_WDTIMER_CLKCTRL,
		},
	},
	.slaves		= ti816x_wd_timer2_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti816x_wd_timer2_slaves),
	.class		= &wd_timer_class,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI816X),
	.flags	  = HWMOD_INIT_NO_RESET,
};


static struct omap_hwmod ti814x_wd_timer1_hwmod = {
	.name		= "wd_timer1",
	.main_clk	= "wdt1_fck",
	.prcm		= {
		.omap4		= {
			.clkctrl_reg = TI81XX_CM_ALWON_WDTIMER_CLKCTRL,
		},
	},
	.slaves		= ti814x_wd_timer1_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti814x_wd_timer1_slaves),
	.class		= &wd_timer_class,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI814X | CHIP_IS_DM385 |
					CHIP_IS_TI811X),
	.flags	  = HWMOD_INIT_NO_RESET,
};

/* I2C1 */

static struct omap_hwmod_irq_info i2c1_mpu_irqs[] = {
	{ .irq = TI81XX_IRQ_I2C0, },
};

static struct omap_hwmod_dma_info i2c1_edma_reqs[] = {
	{ .name = "tx",	.dma_req = 0, },
	{ .name = "rx",	.dma_req = 0, },
};

static struct omap_hwmod_ocp_if *ti816x_i2c1_slaves[] = {
	&ti816x_l4_slow__i2c1,
};

static struct omap_hwmod ti81xx_i2c1_hwmod = {
	.name		= "i2c1",
	.mpu_irqs	= i2c1_mpu_irqs,
	.mpu_irqs_cnt	= ARRAY_SIZE(i2c1_mpu_irqs),
	.sdma_reqs	= i2c1_edma_reqs,
	.sdma_reqs_cnt	= ARRAY_SIZE(i2c1_edma_reqs),
	.main_clk	= "i2c1_fck",
	.prcm		= {
		.omap4 = {
			.clkctrl_reg = TI816X_CM_ALWON_I2C_0_CLKCTRL,
		},
	},
	.slaves		= ti816x_i2c1_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti816x_i2c1_slaves),
	.class		= &i2c_class,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI81XX),
};

/* I2C2 */

static struct omap_hwmod_irq_info i2c2_mpu_irqs[] = {
	{ .irq = TI81XX_IRQ_I2C1, },
};

static struct omap_hwmod_dma_info i2c2_edma_reqs[] = {
	{ .name = "tx", .dma_req = 0, },
	{ .name = "rx", .dma_req = 0, },
};

static struct omap_hwmod_ocp_if *ti816x_i2c2_slaves[] = {
	&ti816x_l4_slow__i2c2,
};

static struct omap_hwmod ti816x_i2c2_hwmod = {
	.name           = "i2c2",
	.mpu_irqs       = i2c2_mpu_irqs,
	.mpu_irqs_cnt   = ARRAY_SIZE(i2c2_mpu_irqs),
	.sdma_reqs      = i2c2_edma_reqs,
	.sdma_reqs_cnt  = ARRAY_SIZE(i2c2_edma_reqs),
	.main_clk       = "i2c2_fck",
	.prcm           = {
		.omap4 = {
			.clkctrl_reg = TI816X_CM_ALWON_I2C_1_CLKCTRL,
		},
	},
	.slaves         = ti816x_i2c2_slaves,
	.slaves_cnt     = ARRAY_SIZE(ti816x_i2c2_slaves),
	.class          = &i2c_class,
	.omap_chip      = OMAP_CHIP_INIT(CHIP_IS_TI816X | CHIP_IS_TI814X |
					CHIP_IS_DM385),
};
/* I2C3 */
static struct omap_hwmod_irq_info i2c3_mpu_irqs[] = {
	{ .irq = TI814X_IRQ_I2C2, },
};

static struct omap_hwmod_dma_info i2c3_edma_reqs[] = {
	{ .name = "tx", .dma_req = 0, },
	{ .name = "rx", .dma_req = 0, },
};

static struct omap_hwmod_ocp_if *ti814x_i2c3_slaves[] = {
	&ti814x_l4_slow__i2c3,
};

static struct omap_hwmod ti814x_i2c3_hwmod = {
	.name           = "i2c3",
	.mpu_irqs       = i2c3_mpu_irqs,
	.mpu_irqs_cnt   = ARRAY_SIZE(i2c3_mpu_irqs),
	.sdma_reqs      = i2c3_edma_reqs,
	.sdma_reqs_cnt  = ARRAY_SIZE(i2c3_edma_reqs),
	.main_clk       = "i2c3_fck",
	.prcm           = {
		.omap4 = {
			.clkctrl_reg = TI816X_CM_ALWON_I2C_0_CLKCTRL,
		},
	},
	.slaves         = ti814x_i2c3_slaves,
	.slaves_cnt     = ARRAY_SIZE(ti814x_i2c3_slaves),
	.class          = &i2c_class,
	.omap_chip      = OMAP_CHIP_INIT(CHIP_IS_TI814X | CHIP_IS_DM385 |
					CHIP_IS_TI811X),
};

/* I2C4 */

static struct omap_hwmod_irq_info i2c4_mpu_irqs[] = {
	{ .irq = TI814X_IRQ_I2C3, },
};

static struct omap_hwmod_dma_info i2c4_edma_reqs[] = {
	{ .name = "tx",	.dma_req = 0, },
	{ .name = "rx",	.dma_req = 0, },
};

static struct omap_hwmod_ocp_if *ti814x_i2c4_slaves[] = {
	&ti814x_l4_slow__i2c4,
};

static struct omap_hwmod ti814x_i2c4_hwmod = {
	.name		= "i2c4",
	.mpu_irqs	= i2c4_mpu_irqs,
	.mpu_irqs_cnt	= ARRAY_SIZE(i2c4_mpu_irqs),
	.sdma_reqs	= i2c4_edma_reqs,
	.sdma_reqs_cnt	= ARRAY_SIZE(i2c4_edma_reqs),
	.main_clk	= "i2c4_fck",
	.prcm		= {
		.omap4 = {
			.clkctrl_reg = TI816X_CM_ALWON_I2C_1_CLKCTRL,
		},
	},
	.slaves		= ti814x_i2c4_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti814x_i2c4_slaves),
	.class		= &i2c_class,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI814X | CHIP_IS_DM385 |
					CHIP_IS_TI811X),
};

/* ELM */
static struct omap_hwmod_class_sysconfig ti81xx_elm_sysc = {
	.rev_offs	= 0x0000,
	.sysc_offs	= 0x0010,
	.syss_offs	= 0x0014,
	.sysc_flags	= (SYSC_HAS_CLOCKACTIVITY | SYSC_HAS_SIDLEMODE |
				SYSC_HAS_SOFTRESET |
				SYSS_HAS_RESET_STATUS),
	.idlemodes	= (SIDLE_FORCE | SIDLE_NO | SIDLE_SMART),
	.sysc_fields	= &omap_hwmod_sysc_type1,
};
/* 'elm' class */
static struct omap_hwmod_class ti81xx_elm_hwmod_class = {
	.name = "elm",
	.sysc = &ti81xx_elm_sysc,
};

static struct omap_hwmod_irq_info ti81xx_elm_irqs[] = {
	{ .irq = TI81XX_IRQ_ELM },
	{ .irq = -1 }
};

struct omap_hwmod_addr_space ti81xx_elm_addr_space[] = {
	{
		.pa_start	= TI81XX_ELM_BASE,
		.pa_end		= TI81XX_ELM_BASE + SZ_8K - 1,
		.flags		= ADDR_MAP_ON_INIT | ADDR_TYPE_RT,
	}
};

struct omap_hwmod_ocp_if ti81xx_l4_slow__elm = {
	.master		= &ti816x_l4_slow_hwmod,
	.slave		= &ti81xx_elm_hwmod,
	.addr		= ti81xx_elm_addr_space,
	.addr_cnt	= ARRAY_SIZE(ti81xx_elm_addr_space),
	.user		= OCP_USER_MPU,
};

static struct omap_hwmod_ocp_if *ti81xx_elm_slaves[] = {
	&ti81xx_l4_slow__elm,
};

/* elm */
static struct omap_hwmod ti81xx_elm_hwmod = {
	.name           = "elm",
	.class          = &ti81xx_elm_hwmod_class,
	.main_clk       = "elm_fck",
	.mpu_irqs		= ti81xx_elm_irqs,
	.mpu_irqs_cnt	= ARRAY_SIZE(ti81xx_elm_irqs),
	.slaves			= ti81xx_elm_slaves,
	.slaves_cnt		= ARRAY_SIZE(ti81xx_elm_slaves),
	.omap_chip		= OMAP_CHIP_INIT(CHIP_IS_TI81XX),
};

/* GPIO1 TI81XX */

static struct omap_hwmod_irq_info ti81xx_gpio1_irqs[] = {
	{ .irq = TI81XX_IRQ_GPIO_0A },
	{ .irq = TI81XX_IRQ_GPIO_0B },
};

/* gpio1 slave ports */
static struct omap_hwmod_ocp_if *ti81xx_gpio1_slaves[] = {
	&ti81xx_l4_slow__gpio1,
};

static struct omap_hwmod_opt_clk gpio1_opt_clks[] = {
	{ .role = "dbclk", .clk = "gpio1_dbck" },
};

static struct omap_hwmod ti81xx_gpio1_hwmod = {
	.name		= "gpio1",
	.class		= &ti81xx_gpio_hwmod_class,
	.mpu_irqs	= ti81xx_gpio1_irqs,
	.mpu_irqs_cnt	= ARRAY_SIZE(ti81xx_gpio1_irqs),
	.main_clk	= "gpio1_ick",
	.prcm = {
		.omap4 = {
			.clkctrl_reg = TI81XX_CM_ALWON_GPIO_0_CLKCTRL,
		},
	},
	.opt_clks	= gpio1_opt_clks,
	.opt_clks_cnt	= ARRAY_SIZE(gpio1_opt_clks),
	.dev_attr	= &gpio_dev_attr,
	.slaves		= ti81xx_gpio1_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti81xx_gpio1_slaves),
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI81XX),
#if defined(CONFIG_MACH_DM385IPNC) || defined(CONFIG_MACH_TI8148IPNC)
	.flags	  = HWMOD_INIT_NO_RESET,
#endif
};

/* GPIO2 TI81XX*/

static struct omap_hwmod_irq_info ti81xx_gpio2_irqs[] = {
	{ .irq = TI81XX_IRQ_GPIO_1A },
	{ .irq = TI81XX_IRQ_GPIO_1B },
};

/* gpio2 slave ports */
static struct omap_hwmod_ocp_if *ti81xx_gpio2_slaves[] = {
	&ti81xx_l4_slow__gpio2,
};

static struct omap_hwmod_opt_clk gpio2_opt_clks[] = {
	{ .role = "dbclk", .clk = "gpio2_dbck" },
};

static struct omap_hwmod ti81xx_gpio2_hwmod = {
	.name		= "gpio2",
	.class		= &ti81xx_gpio_hwmod_class,
	.mpu_irqs	= ti81xx_gpio2_irqs,
	.mpu_irqs_cnt	= ARRAY_SIZE(ti81xx_gpio2_irqs),
	.main_clk	= "gpio2_ick",
	.prcm = {
		.omap4 = {
			.clkctrl_reg = TI81XX_CM_ALWON_GPIO_1_CLKCTRL,
		},
	},
	.opt_clks	= gpio2_opt_clks,
	.opt_clks_cnt	= ARRAY_SIZE(gpio2_opt_clks),
	.dev_attr	= &gpio_dev_attr,
	.slaves		= ti81xx_gpio2_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti81xx_gpio2_slaves),
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI81XX),
#if defined(CONFIG_MACH_DM385IPNC) || defined(CONFIG_MACH_TI8148IPNC)
	.flags	  = HWMOD_INIT_NO_RESET,
#endif
};

/* GPIO3 TI814X */

static struct omap_hwmod_irq_info ti814x_gpio3_irqs[] = {
	{ .irq = TI814X_IRQ_GPIO_2A },
	{ .irq = TI814X_IRQ_GPIO_2B },
};

/* gpio3 slave ports */
static struct omap_hwmod_ocp_if *ti814x_gpio3_slaves[] = {
	&ti814x_l4_slow__gpio3,
};

static struct omap_hwmod_opt_clk gpio3_opt_clks[] = {
	{ .role = "dbclk", .clk = "gpio3_dbck" },
};

static struct omap_hwmod ti814x_gpio3_hwmod = {
	.name		= "gpio3",
	.class		= &ti81xx_gpio_hwmod_class,
	.mpu_irqs	= ti814x_gpio3_irqs,
	.mpu_irqs_cnt	= ARRAY_SIZE(ti814x_gpio3_irqs),
	.main_clk	= "gpio3_ick",
	.prcm = {
		.omap4 = {
			.clkctrl_reg = TI81XX_CM_ALWON_GPIO_1_CLKCTRL,
		},
	},
	.opt_clks	= gpio3_opt_clks,
	.opt_clks_cnt	= ARRAY_SIZE(gpio3_opt_clks),
	.dev_attr	= &gpio_dev_attr,
	.slaves		= ti814x_gpio3_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti814x_gpio3_slaves),
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI814X | CHIP_IS_DM385 |
					CHIP_IS_TI811X),
#if defined(CONFIG_MACH_DM385IPNC) || defined(CONFIG_MACH_TI8148IPNC)
	.flags	  = HWMOD_INIT_NO_RESET,
#endif
};

/* GPIO4 TI814X*/

static struct omap_hwmod_irq_info ti814x_gpio4_irqs[] = {
	{ .irq = TI814X_IRQ_GPIO_3A },
	{ .irq = TI814X_IRQ_GPIO_3B },
};

/* gpio4 slave ports */
static struct omap_hwmod_ocp_if *ti814x_gpio4_slaves[] = {
	&ti814x_l4_slow__gpio4,
};

static struct omap_hwmod_opt_clk gpio4_opt_clks[] = {
	{ .role = "dbclk", .clk = "gpio4_dbck" },
};

static struct omap_hwmod ti814x_gpio4_hwmod = {
	.name		= "gpio4",
	.class		= &ti81xx_gpio_hwmod_class,
	.mpu_irqs	= ti814x_gpio4_irqs,
	.mpu_irqs_cnt	= ARRAY_SIZE(ti814x_gpio4_irqs),
	.main_clk	= "gpio4_ick",
	.prcm = {
		.omap4 = {
			.clkctrl_reg = TI81XX_CM_ALWON_GPIO_1_CLKCTRL,
		},
	},
	.opt_clks	= gpio4_opt_clks,
	.opt_clks_cnt	= ARRAY_SIZE(gpio4_opt_clks),
	.dev_attr	= &gpio_dev_attr,
	.slaves		= ti814x_gpio4_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti814x_gpio4_slaves),
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI814X | CHIP_IS_DM385 |
					CHIP_IS_TI811X),
#if defined(CONFIG_MACH_DM385IPNC) || defined(CONFIG_MACH_TI8148IPNC)
	.flags	  = HWMOD_INIT_NO_RESET,
#endif
};

/* GPIO5 TI811X*/

static struct omap_hwmod_irq_info ti814x_gpio5_irqs[] = {
	{ .irq = TI811X_IRQ_GPIO_4A },
	{ .irq = TI811X_IRQ_GPIO_4B },
};

/* gpio4 slave ports */
static struct omap_hwmod_ocp_if *ti814x_gpio5_slaves[] = {
	&ti814x_l4_slow__gpio5,
};

static struct omap_hwmod_opt_clk gpio5_opt_clks[] = {
	{ .role = "dbclk", .clk = "gpio5_dbck" },
};

static struct omap_hwmod ti814x_gpio5_hwmod = {
	.name		= "gpio5",
	.class		= &ti81xx_gpio_hwmod_class,
	.mpu_irqs	= ti814x_gpio5_irqs,
	.mpu_irqs_cnt	= ARRAY_SIZE(ti814x_gpio4_irqs),
	.main_clk	= "gpio5_ick",
	.prcm = {
		.omap4 = {
			.clkctrl_reg = TI81XX_CM_ALWON_GPIO_1_CLKCTRL,
		},
	},
	.opt_clks	= gpio5_opt_clks,
	.opt_clks_cnt	= ARRAY_SIZE(gpio4_opt_clks),
	.dev_attr	= &gpio_dev_attr,
	.slaves		= ti814x_gpio5_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti814x_gpio5_slaves),
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI811X),
};

/* GPIO6 TI811X*/

static struct omap_hwmod_irq_info ti814x_gpio6_irqs[] = {
	{ .irq = TI811X_IRQ_GPIO_5A },
	{ .irq = TI811X_IRQ_GPIO_5B },
};

/* gpio4 slave ports */
static struct omap_hwmod_ocp_if *ti814x_gpio6_slaves[] = {
	&ti814x_l4_slow__gpio6,
};

static struct omap_hwmod_opt_clk gpio6_opt_clks[] = {
	{ .role = "dbclk", .clk = "gpio6_dbck" },
};

static struct omap_hwmod ti814x_gpio6_hwmod = {
	.name		= "gpio6",
	.class		= &ti81xx_gpio_hwmod_class,
	.mpu_irqs	= ti814x_gpio6_irqs,
	.mpu_irqs_cnt	= ARRAY_SIZE(ti814x_gpio6_irqs),
	.main_clk	= "gpio6_ick",
	.prcm = {
		.omap4 = {
			.clkctrl_reg = TI81XX_CM_ALWON_GPIO_1_CLKCTRL,
		},
	},
	.opt_clks	= gpio6_opt_clks,
	.opt_clks_cnt	= ARRAY_SIZE(gpio6_opt_clks),
	.dev_attr	= &gpio_dev_attr,
	.slaves		= ti814x_gpio6_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti814x_gpio6_slaves),
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI811X),
};

/* L3 SLOW -> USBSS interface */
static struct omap_hwmod_addr_space ti81xx_usbss_addr_space[] = {
	{
		.name		= "usbss",
		.pa_start	= 0x47400000,
		.pa_end		= 0x47400000 + SZ_4K - 1,
		.flags		= ADDR_TYPE_RT
	},
	{
		.name		= "musb0",
		.pa_start	= 0x47401000,
		.pa_end		= 0x47401000 + SZ_2K - 1,
		.flags		= ADDR_TYPE_RT
	},
	{
		.name		= "musb1",
		.pa_start	= 0x47401800,
		.pa_end		= 0x47401800 + SZ_2K - 1,
		.flags		= ADDR_TYPE_RT
	},
};

static struct omap_hwmod_class_sysconfig ti81xx_usbhsotg_sysc = {
	.rev_offs	= 0x0,
	.sysc_offs	= 0x10,
};

static struct omap_hwmod_class ti81xx_usbotg_class = {
	.name = "usbotg",
	.sysc = &ti81xx_usbhsotg_sysc,
};

static struct omap_hwmod_irq_info ti81xx_usbss_mpu_irqs[] = {
	{ .name = "usbss-irq", .irq = 17, },
	{ .name = "musb0-irq", .irq = 18, },
	{ .name = "musb1-irq", .irq = 19, },
};

static struct omap_hwmod_ocp_if ti81xx_l3_slow__usbss = {
	.master		= &ti816x_l3_slow_hwmod,
	.slave		= &ti81xx_usbss_hwmod,
	.clk		= "usb_ick",
	.addr		= ti81xx_usbss_addr_space,
	.addr_cnt	= ARRAY_SIZE(ti81xx_usbss_addr_space),
	.user		= OCP_USER_MPU,
};

static struct omap_hwmod_ocp_if *ti81xx_usbss_slaves[] = {
	&ti81xx_l3_slow__usbss,
};

static struct omap_hwmod ti81xx_usbss_hwmod = {
	.name		= "usb_otg_hs",
	.mpu_irqs	= ti81xx_usbss_mpu_irqs,
	.mpu_irqs_cnt	= ARRAY_SIZE(ti81xx_usbss_mpu_irqs),
	.main_clk	= "usb_ick",
	.prcm		= {
		.omap4 = {
			.clkctrl_reg = TI816X_CM_DEFAULT_USB_CLKCTRL,
		},
	},
	.slaves		= ti81xx_usbss_slaves,
	.slaves_cnt	= ARRAY_SIZE(ti81xx_usbss_slaves),
	.class		= &ti81xx_usbotg_class,
	.omap_chip	= OMAP_CHIP_INIT(CHIP_IS_TI81XX)
};

static __initdata struct omap_hwmod *ti81xx_hwmods[] = {
	&ti816x_l3_slow_hwmod,
	&ti816x_l4_slow_hwmod,
	&ti816x_mpu_hwmod,
	&ti816x_uart1_hwmod,
#ifndef CONFIG_TI8148_EVM_OPTIMIZED
	&ti816x_uart2_hwmod,
	&ti816x_uart3_hwmod,
	&ti814x_uart4_hwmod,
	&ti814x_uart5_hwmod,
	&ti814x_uart6_hwmod,
#endif
	&ti816x_wd_timer2_hwmod,
	&ti814x_wd_timer1_hwmod,
	&ti81xx_i2c1_hwmod,	/* Note: In TI814X this enables I2C0/2 */
	&ti816x_i2c2_hwmod,
	&ti814x_i2c3_hwmod,	/* Note: In TI814X this enables I2C1/3 */
	&ti814x_i2c4_hwmod,	/* Note: In TI814X this enables I2C1/3 */
	&ti81xx_gpio1_hwmod,
	&ti81xx_gpio2_hwmod,
	&ti814x_gpio3_hwmod,
	&ti814x_gpio4_hwmod,
	&ti814x_gpio5_hwmod,
	&ti814x_gpio6_hwmod,
	&ti81xx_usbss_hwmod,
	&ti81xx_elm_hwmod,
	NULL,
};

int __init ti81xx_hwmod_init(void)
{
	return omap_hwmod_init(ti81xx_hwmods);
}
