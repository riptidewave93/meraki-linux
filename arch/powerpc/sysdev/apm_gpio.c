/*
 * APM SoC AM86xxx GPIO Driver
 *
 * Copyright (c) 2010 Applied Micro Circuits Corporation.
 * All rights reserved. Inderpal Saluja <isaluja@apm.com>.
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
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/spinlock.h>
#include <linux/io.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/gpio.h>
#include <linux/types.h>
#include <linux/slab.h>

#define PORT_WIDTH          	0x1F
#define GPIO_MASK(gpio)		(gpio & PORT_WIDTH)

/* Physical GPIO register layout */
struct apm_gpio {
	__be32 dr;	/* Data Reg */
	__be32 ddr;	/* Data direction  */
	__be32 ctl;	/* Data Source Control */
	__be32 pad0[9];
	__be32 inten;	/* Interrupt enable */
	__be32 mask;	/* Interrupt mask */	
	__be32 int_type;	/* Interrupt level */
	__be32 int_polarity;	/*Interrupt polarity */
	__be32 int_stat;	/* Interrupt status */
	__be32 raw_intstat;	/* Raw interrupt status */
	__be32 dbc;		/* gpio_debounce */
	__be32 eoi;	/* Clear interrupt */
	__be32 ext_port;	/* External Port A */
	__be32 pad1[3];
	__be32 ls_sync;	/* Synchronization level */	
	__be32 id_code;	/* GPIO ID code */
	__be32 pad2;
	__be32 comp_version; /* GPIO Component Version */
};

struct apm_gpio_chip {
	struct of_mm_gpio_chip mm_gc;
	spinlock_t lock;
};

static inline struct apm_gpio_chip *to_apm_gpiochip(
				struct of_mm_gpio_chip *mm_gc)
{
	return container_of(mm_gc, struct apm_gpio_chip, mm_gc);
}

static int apm_gpio_get(struct gpio_chip *gc, unsigned int gpio)
{
	struct of_mm_gpio_chip *mm_gc = to_of_mm_gpio_chip(gc);
	struct apm_gpio __iomem *regs = mm_gc->regs;

	return in_be32(&regs->ext_port) & GPIO_MASK(gpio);
}

static inline void __apm_gpio_set(struct gpio_chip *gc, unsigned int gpio, 
				int val)
{
	struct of_mm_gpio_chip *mm_gc = to_of_mm_gpio_chip(gc);
	struct apm_gpio __iomem *regs = mm_gc->regs;

	if (val)
		setbits32(&regs->dr, GPIO_MASK(gpio));
	else
		clrbits32(&regs->dr, GPIO_MASK(gpio));
}

static void apm_gpio_set(struct gpio_chip *gc, unsigned int gpio, int val)
{
	struct of_mm_gpio_chip *mm_gc = to_of_mm_gpio_chip(gc);
	struct apm_gpio_chip *chip = to_apm_gpiochip(mm_gc);
	unsigned long flags;

	spin_lock_irqsave(&chip->lock, flags);

	__apm_gpio_set(gc, gpio, val);

	spin_unlock_irqrestore(&chip->lock, flags);

	pr_debug("%s: gpio: %d val: %d\n", __func__, gpio, val);
}

static int apm_gpio_dir_in(struct gpio_chip *gc, unsigned int gpio)
{
	struct of_mm_gpio_chip *mm_gc = to_of_mm_gpio_chip(gc);
	struct apm_gpio_chip *chip = to_apm_gpiochip(mm_gc);
	struct apm_gpio __iomem *regs = mm_gc->regs;
	unsigned long flags;

	spin_lock_irqsave(&chip->lock, flags);

	clrbits32(&regs->ddr, GPIO_MASK(gpio));

	spin_unlock_irqrestore(&chip->lock, flags);

	return 0;
}

static int apm_gpio_dir_out(struct gpio_chip *gc, unsigned int gpio, int val)
{
	struct of_mm_gpio_chip *mm_gc = to_of_mm_gpio_chip(gc);
	struct apm_gpio_chip *chip = to_apm_gpiochip(mm_gc);
	struct apm_gpio __iomem *regs = mm_gc->regs;
	unsigned long flags;

	spin_lock_irqsave(&chip->lock, flags);

	/* First set initial value */
	__apm_gpio_set(gc, gpio, val);

	setbits32(&regs->ddr, GPIO_MASK(gpio));

	spin_unlock_irqrestore(&chip->lock, flags);

	pr_debug("%s: gpio: %d val: %d\n", __func__, gpio, val);

	return 0;
}

static int __init apm_add_gpiochips(void)
{
	struct device_node *np;

	for_each_compatible_node(np, NULL, "apm,apm86xxx-gpio") {
		int ret;
		struct apm_gpio_chip *apm_gc;
		struct of_mm_gpio_chip *mm_gc;
		struct gpio_chip *gc;
		const unsigned int *prop;
		unsigned int val = 0;

		apm_gc = kzalloc(sizeof(*apm_gc), GFP_KERNEL);
		if (!apm_gc) {
			ret = -ENOMEM;
			goto err;
		}

		prop = of_get_property(np, "gpio-count", NULL);
		if (prop)
			val = *prop;

		spin_lock_init(&apm_gc->lock);

		mm_gc = &apm_gc->mm_gc;
		gc = &mm_gc->gc;

		gc->ngpio = val;
		gc->direction_input = apm_gpio_dir_in;
		gc->direction_output = apm_gpio_dir_out;
		gc->get = apm_gpio_get;
		gc->set = apm_gpio_set;

		ret = of_mm_gpiochip_add(np, mm_gc);
		if (ret)
			goto err;
		continue;
err:
		pr_err("%s: registration failed with status %d\n",
				np->full_name, ret);
		kfree(apm_gc);
	}
	return 0;
}
arch_initcall(apm_add_gpiochips);

