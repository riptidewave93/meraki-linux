/*
 * Copyright (C) 2013, Cisco Systems. All Rights Reserved
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

#include <linux/version.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <linux/dma-mapping.h>
#include <linux/clkdev.h>
#include <linux/gpio.h>
#include <linux/leds.h>
#include <linux/mtd/partitions.h>
#include <linux/input.h>
#include <linux/gpio_keys.h>
#include <asm/hardware/gic.h>

#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/irq.h>
#include <linux/input.h>
#include <linux/i2c/tsc2007.h>
#include <linux/spi/spi.h>
#include <mach/hardware.h>
#include <asm/mach/arch.h>
#include <asm/mach-types.h>
#include <mach/iproc.h>
#include <asm/io.h>
#include <mach/io_map.h>
#include <mach/reg_utils.h>
#include <mach/gpio_cfg.h>
#include <mach/nand_iproc.h>
#include <mach/common.h>
#include <mach/iproc.h>
#include <linux/of.h>
#include <linux/of_fdt.h>
#include <linux/of_platform.h>
#include <linux/of_irq.h>
#include <linux/gpio.h>


#include "northstar.h"
#include "common.h"
#include <linux/of_platform.h>
#include <asm/system_misc.h>

#define GPIO_EXPORT_FLAG_HIGH		(1 << 0) /* Initially set output high */
#define GPIO_EXPORT_DIRECTION_INPUT	(1 << 1) /* Initially set as input */
#define GPIO_EXPORT_DIRECTION_CHANGES	(1 << 2) /* Userland changes direction */

/* This is the main reference clock 25MHz from external crystal */
static struct clk clk_ref = {
	.name = "Refclk",
	.rate = 25 * 1000000,   /* run-time override */
	.fixed = 1,
	.type  = 0,
};


static struct clk_lookup board_clk_lookups[] = {
	{
	.con_id         = "refclk",
	.clk            = &clk_ref,
	}
};

extern void __init northstar_timer_init(struct clk *clk_ref);

struct brcmnand_platform_data board_nand_partitions = {
	.chip_select	= 0,
};

static struct platform_device board_nand_device = {
	.name		= "nand_iproc",
	.id		= 0,
	.dev		= {
		.platform_data		= &board_nand_partitions,
	},
};

static struct of_device_id irq_match[] __initdata = {
	{ .compatible = "arm,cortex-a9-gic", .data = gic_of_init, },
	{ }
};

void __init board_map_io(void)
{

	/*
	 * Install clock sources in the lookup table.
	 */
	clkdev_add_table(board_clk_lookups,
			ARRAY_SIZE(board_clk_lookups));

	/* Map machine specific iodesc here */
	northstar_map_io();
}

static struct platform_device *board_devices[] __initdata = {
    &board_nand_device,
};

static void __init board_init_irq(void)
{
	of_irq_init(irq_match);
}

static void __init board_read_device_tree()
{
	of_platform_populate(NULL, of_default_bus_match_table, NULL, NULL);
}

static void __init board_add_devices(void)
{
	u32 straps;

	/* Fill in NAND configuration */

	straps = __REG32(IPROC_DMU_BASE_VA + IPROC_DMU_STRAPS_OFFSET);
	board_nand_partitions.strap_boot =
		((straps >> IPROC_STRAP_BOOT_DEV_SHIFT) & 3) == 1;
	board_nand_partitions.strap_type =
		(straps >> IPROC_STRAP_NAND_TYPE_SHIFT) & 0xf;
	board_nand_partitions.strap_page_size =
		(straps >> IPROC_STRAP_NAND_PAGE_SHIFT) & 0x3;
	if (!board_nand_partitions.strap_boot) {
		board_nand_partitions.strap_type &= 0x7;
	}

	platform_add_devices(board_devices, ARRAY_SIZE(board_devices));
}

void __init board_timer_init(void)
{
	northstar_timer_init(&clk_ref);
}

struct sys_timer board_timer = {
	.init   = board_timer_init,
};


extern __init void arm_set_machine_name(const char *name);

void __init board_cfgpin(struct property *prop, int state)
{
	int sz;
	const __be32 *val;

	if (!prop)
		return;

	sz = prop->length / 4;
	val = prop->value;
	while (sz--) {
		pr_debug("%s: gpio %d is %s\n", __func__, be32_to_cpup(val), state==IPROC_GPIO_GENERAL?"General":"Aux");
		iproc_gpio_cfgpin(be32_to_cpup(val), state);
		val++;
	}
}

void __init board_exportpin(struct property *prop)
{
	int sz;
	const __be32 *val;

	if (!prop)
		return;

	sz = prop->length / 4;
	val = prop->value;

	do{
		int pin,flags;
		int ret;

		pin = be32_to_cpup(val);
		val++;
		flags = be32_to_cpup(val);
		val++;

		ret = gpio_request(pin, "sysfs");
		if (ret < 0) {
			printk(KERN_ERR
			       "%s: gpio_%d request failed, ret = %d\n",
				 __func__, pin, ret);
			continue;
		}

		ret = gpio_export(pin,
				  (flags & GPIO_EXPORT_DIRECTION_CHANGES) != 0);
		if (ret < 0) {
			printk(KERN_ERR
			       "%s: gpio_%d export failed, ret = %d\n",
				 __func__, pin, ret);
			continue;
		}
		iproc_gpio_cfgpin(pin, IPROC_GPIO_GENERAL);

		if (flags & GPIO_EXPORT_DIRECTION_INPUT)
			ret = gpio_direction_input(pin);
		else
			ret = gpio_direction_output(
				pin, (flags & GPIO_EXPORT_FLAG_HIGH) != 0);

		if (ret < 0) {
			printk(KERN_ERR
			       "%s: gpio_%d set direction failed, ret = %d\n",
				 __func__, pin, ret);
			continue;
		}
	}while(sz-=2);
}

static void venom_restart(char mode, const char *cmd)
{
        /* Make the watchdog reset us in one tick */
        __raw_writel(1, IPROC_CCA_CORE_REG_VA + 0x80);
}

void __init board_init(void)
{
	const char *model;
	unsigned long dt_root;
	struct device_node * np;
	struct property * prop;

	printk(KERN_DEBUG "board_init: Enter\n");

	// Change the restart code for Venom
	arm_pm_restart = venom_restart;

	/*
	 * Configure GPIOs
	 */
	np = of_find_compatible_node(NULL, NULL, "iproc-gpio-cfg");
	if (np) {
		prop = of_find_property(np, "gpios-general", NULL);
		board_cfgpin(prop, IPROC_GPIO_GENERAL);
		prop = of_find_property(np, "gpios-aux", NULL);
		board_cfgpin(prop, IPROC_GPIO_AUX_FUN);
		prop = of_find_property(np, "gpios-export", NULL);
		board_exportpin(prop);
	}

	/* Set machine name for /proc/cpuinfo */
	dt_root = of_get_flat_dt_root();
	model = of_get_flat_dt_prop(dt_root, "model", NULL);
	if (model) {
		arm_set_machine_name(model);
	}

	/*
	 * Add common platform devices that do not have board dependent HW
	 * configurations
	 */

	board_read_device_tree();
	board_add_common_devices(&clk_ref);
	board_add_devices();

	printk(KERN_DEBUG "board_init: Leave\n");
}

#ifdef CONFIG_OF
static const char *venom_boards_compat[] __initdata = {
	"meraki,venom",
	"meraki,mr26",
	"meraki,espresso",
	NULL,
};
#endif /* CONFIG_OF */

MACHINE_START(MERAKI_MR26, "Meraki Venom")
	.map_io = board_map_io,
	.init_irq = board_init_irq,
	.handle_irq     = gic_handle_irq,
	.timer  = &board_timer,
	.init_machine = board_init,
	.smp = smp_ops(iproc_smp_ops),
#ifdef CONFIG_OF
	.dt_compat = venom_boards_compat,
#endif
MACHINE_END
