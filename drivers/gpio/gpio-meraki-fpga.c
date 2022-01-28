/*
 *  CISCO - Expose FPGA registers as GPIO pins
 *
 *  Copyright (C) 2017 Cisco Systems, Inc.
 *
 *  Derived from drivers/gpio/gpio-pca953x.c
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 2 of the License.
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <linux/spi/spi.h>
#ifdef CONFIG_OF_GPIO
#include <linux/of_platform.h>
#endif

extern ssize_t act_reg_read(struct device *dev,
			    uint32_t reg,
			    uint8_t* val,
			    size_t count);
extern ssize_t act_reg_write(struct device *dev,
			     uint32_t reg,
			     uint8_t* val,
			     size_t count);
extern struct device* of_find_act_device_by_node(struct device_node *node);

struct virtual_gpio_chip {
        u32    register_size;
	struct device_node* fpga_np;
	struct device* fpga;
	struct gpio_chip gc;
};

static inline struct virtual_gpio_chip *to_fpga(struct gpio_chip *gc_given)
{
	return container_of(gc_given, struct virtual_gpio_chip, gc);
}

static int fpga2gpio_direction_input(struct gpio_chip *gc, unsigned off)
{
	// Do Nothing - registers don't need any configuring
	return 0;
}

static int fpga2gpio_direction_output(struct gpio_chip *gc,
		unsigned off, int val)
{
	// Do Nothing - registers don't need any configuring
	return 0;
}

static inline int highest_bit_set(unsigned int x)
{
	unsigned int i;
	unsigned int y = x;
	for(i = 1; y > 1; i++) {
		x |= x >> i;
		y = y >> 1;
	}
	return x = x ^ (x >> 1);
}

static int fpga2gpio_get_value(struct gpio_chip *gc, unsigned off)
{
	uint32_t read_val = 0;
	ssize_t bytes = 0;
	struct virtual_gpio_chip* chip = to_fpga(gc);
	unsigned int reg_addr = 0;
	reg_addr = off/chip->register_size + chip->gc.base;
	bytes = act_reg_read(chip->fpga, reg_addr, (uint8_t *)&read_val, (chip->register_size/8));
	if(bytes <= 0) {
		pr_debug("\n%s: Error reading from FPGA pin %d  \n",__func__, off);
	}
	return (read_val & (1u << (off % chip->register_size)))?1:0;
}

static void fpga2gpio_set_value(struct gpio_chip *gc, unsigned off, int val)
{
	uint32_t read_val = 0, write_val = 0;
	ssize_t bytes = 0;
	struct virtual_gpio_chip* chip = to_fpga(gc);
	unsigned int reg_addr = 0;
	reg_addr = off/chip->register_size + chip->gc.base;
	bytes = act_reg_read(chip->fpga, reg_addr, (uint8_t *)&read_val, (chip->register_size/8));
	if(bytes <= 0) {
		pr_debug("\n%s: Error reading from FPGA pin %d  \n",__func__, off);
	}
	if(val) {
		write_val = read_val | (1u << (off % chip->register_size));
	} else {
		write_val = read_val & (~(1u << (off % chip->register_size)));
	}
	bytes = act_reg_write(chip->fpga, reg_addr, (uint8_t *)&write_val, (chip->register_size/8));
	if(bytes <= 0) {
		pr_debug("\n%s: Error writing to FPGA pin %d	\n",__func__, off);
	}
}

static int fpga2gpio_probe(struct platform_device *pdev)
{
	struct virtual_gpio_chip *chip;
	int ret, err;
	struct device_node* fpga_np;
	struct device* fpga;

	fpga_np = of_parse_phandle(pdev->dev.of_node, "fpga-parent", 0);
	if (!fpga_np) {
		dev_err(&pdev->dev, "Cannot parse fpga-parent\n");
		return -ENODEV;
	}

	fpga = of_find_act_device_by_node(fpga_np);
	of_node_put(fpga_np);
	if(!fpga)
		return -EPROBE_DEFER;

	chip = devm_kzalloc(&pdev->dev,
			sizeof(struct virtual_gpio_chip), GFP_KERNEL);
	if (!chip)
		return -ENOMEM;

	chip->fpga = fpga;

	if (of_property_read_u32(pdev->dev.of_node,
				 "ngpios",
				 &chip->gc.ngpio)) {
		dev_err(&pdev->dev, "Missing ngpios OF property\n");
		return -ENODEV;
	}
	if (of_property_read_u32(pdev->dev.of_node,
				 "reg",
				 &chip->gc.base)) {
		dev_err(&pdev->dev, "Missing reg OF property\n");
		return -ENODEV;
	}
	if (of_property_read_u32(pdev->dev.of_node,
				 "reg-size",
				 &chip->register_size)) {
		dev_err(&pdev->dev, "Missing reg-size OF property\n");
		return -ENODEV;
	}
	chip->register_size = highest_bit_set(chip->register_size);
	if (chip->register_size < 8) {
		dev_err(&pdev->dev, "Register size less than 8 bits. Needs to be atleast 8 bits wide\n");
		return -ENODEV;
	}

	chip->gc.direction_input  = fpga2gpio_direction_input;
	chip->gc.direction_output = fpga2gpio_direction_output;
	chip->gc.get = fpga2gpio_get_value;
	chip->gc.set = fpga2gpio_set_value;
	chip->gc.can_sleep = true;
	chip->gc.label = pdev->name;
	chip->gc.owner = THIS_MODULE;
	chip->gc.parent = &pdev->dev;
	ret = gpiochip_add(&chip->gc);
	if (ret)
		return ret;
	platform_set_drvdata(pdev, chip);
	return 0;
}

static int fpga2gpio_remove(struct platform_device *pdev)
{
	struct virtual_gpio_chip *chip = platform_get_drvdata(pdev);
	gpiochip_remove(&chip->gc);
	return 0;
}

static const struct of_device_id fpga_dt_ids[] = {
	{ .compatible = "meraki,fpga-to-gpio", },
	{ }
};

MODULE_DEVICE_TABLE(of, fpga_dt_ids);

static struct platform_driver fpga2gpio_driver = {
	.driver= {
		.name	= "meraki-fpga2gpio",
		.of_match_table = fpga_dt_ids,
	},
	.probe		= fpga2gpio_probe,
	.remove		= fpga2gpio_remove,
};

module_platform_driver(fpga2gpio_driver);

MODULE_AUTHOR("Karthik Ramanathan <kramana3@cisco.com>");
MODULE_DESCRIPTION("GPIO controller to expose registers in Meraki FPGA as GPIO pins");
MODULE_LICENSE("GPL");
