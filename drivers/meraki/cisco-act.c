/* cisco-act.c - a device driver for the Cisco ACT2 chip  */
/* ------------------------------------------------------------------------- */
/* Copyright (C) 2016 Cisco Systems, Inc.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.                */
/* ------------------------------------------------------------------------- */

/* Initial driver commit by William Hauser <whauser@meraki.com> */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/of_gpio.h>
#include <linux/delay.h>
#include <linux/meraki/cisco-act.h>
#include <linux/gpio/consumer.h>

static ssize_t cisco_act_show_buffer_size(struct device *dev,
					  struct device_attribute *attr,
					  char *buf)
{
	struct act_data *act = dev_get_drvdata(dev);
	memcpy(buf, &act->writebuf_size, sizeof(uint16_t));
	return sizeof(uint16_t);
}
static DEVICE_ATTR(buffer_size, 0444, cisco_act_show_buffer_size, NULL);

static ssize_t cisco_act_lock(struct device *dev,
			      struct device_attribute *attr, char *buf)
{
	return 0;
}
static DEVICE_ATTR(lock, 0444, cisco_act_lock, NULL);

static ssize_t cisco_act_show_chip_family(struct device *dev,
				   struct device_attribute *attr,
				   char *buf)
{
	struct act_data *act = dev_get_drvdata(dev);
	memcpy(buf, &act->chip_family, sizeof(u32));
	return sizeof(u32);
}
static DEVICE_ATTR(chip_family, 0444, cisco_act_show_chip_family, NULL);

static ssize_t act_software_reset(struct device *dev)
{
	ssize_t err;
	uint32_t reset_reg = 0x2000;
	char reset_code[] = {0x37, 0x00, 0x00, 0x00};
	err = act_reg_write(dev, reset_reg, reset_code, 4);
	if (err < 0) {
		dev_err(dev, "Can't soft-reset ACT2\n");
		return err;
	}

	return 0;
}

static ssize_t cisco_act_store_software_reset(struct device *dev,
				struct device_attribute *attr,
				const char *buf, size_t count)
{
	struct act_data *act = dev_get_drvdata(dev);

	if (act == NULL) {
		dev_err(dev, "No act driver data\n");
	} else if (act->chip_family != TAM_FAMILY_FPGA) {
		dev_err(dev, "Soft reset not available on this device\n");
	} else {
		act_software_reset(dev);
	}

	return count;
}
static DEVICE_ATTR(software_reset, 0200, NULL, cisco_act_store_software_reset);

static struct device_attribute *cisco_act_attrs[] = {
	&dev_attr_buffer_size,
	&dev_attr_lock,
	&dev_attr_chip_family,
	&dev_attr_software_reset,
};

static ssize_t act_bin_read(struct file *filp, struct kobject *kobj,
			    struct bin_attribute *attr,
			    char *buf, loff_t off, size_t count)
{
	struct act_data *act;

	act = dev_get_drvdata(container_of(kobj, struct device, kobj));
	return act->bus_ops->read(act, buf, off, count);
}

static ssize_t act_bin_write(struct file *filp, struct kobject *kobj,
			     struct bin_attribute *attr,
			     char *buf, loff_t off, size_t count)
{
	struct act_data *act;

	act = dev_get_drvdata(container_of(kobj, struct device, kobj));
	return act->bus_ops->write(act, buf, off, count);
}

ssize_t act_reg_read(struct device* dev,
		     uint32_t reg,
		     uint8_t* val,
		     size_t count)
{
	struct act_data *act;

	act = dev_get_drvdata(dev);
	if (act == NULL) {
		return -ENODEV;
	}
	return act->bus_ops->reg_read(act, reg, val, count);
}

ssize_t act_reg_write(struct device *dev,
		      uint32_t reg,
		      uint8_t* val,
		      size_t count)
{
	struct act_data *act;

	act = dev_get_drvdata(dev);
	if (act == NULL) {
		return -ENODEV;
	}

	return act->bus_ops->reg_write(act, reg, val, count);
}

EXPORT_SYMBOL(act_reg_read);
EXPORT_SYMBOL(act_reg_write);

static struct class *act_class;
static struct device_type act_dev_type;

static void act_dev_release(struct device *dev)
{
	pr_debug("device: '%s': %s\n", dev_name(dev), __func__);
	kfree(dev);
}

static int cisco_act_init_device(struct act_data *act)
{
	struct device *dev;
	int rc;

	dev = (struct device *)kzalloc(sizeof(*dev), GFP_KERNEL);
	if (!dev)
		return -ENOMEM;

	device_initialize(dev);

	dev->class = act_class;
	dev->type = &act_dev_type;
	dev->parent = NULL; // put device in /sys/devices/
	dev->release = act_dev_release;
	dev_set_drvdata(dev, act);
	act->dev = dev;

	rc = dev_set_name(dev, "act2");
	if (rc)
		goto dev_add_failed;

	rc = device_add(dev);
	if (rc)
		goto dev_set_name_failed;

	return 0;

dev_add_failed:
dev_set_name_failed:
	put_device(dev);
	return rc;
}

struct act_data* cisco_act_common_probe(struct device* dev,
					const struct cisco_act_bus_ops* bus_ops)
{
	struct act_data *act;
	struct cisco_act_platform_data* pdata;
	int err = 0;
	int i;
	uint16_t writebuf_size = WRITE_MIN;
	uint8_t* writebuf;
	struct gpio_desc* reset_gpio;
	struct gpio_desc* bus_select_gpio;

	if (!bus_ops) {
		dev_err(dev, "Invalid arguments: bus_ops\n");
		err = -EINVAL;
		goto fail_init_device;
	}

	act = devm_kzalloc(dev, sizeof(struct act_data), GFP_KERNEL);
	if (!act) {
		dev_err(dev, "ACT2 alloc fail\n");
		err = -ENOMEM;
		goto fail_init_device;
	}

	mutex_init(&act->lock);

	/* For platforms using Device Trees */
	if (IS_ENABLED(CONFIG_OF) && dev->of_node) {
		int num_elems;
		uint32_t mask;

		err = of_property_read_u16(dev->of_node,
					   "write-buf-size",
					   &writebuf_size);
		if (err < 0 && err != -EINVAL)
		    dev_err(dev, "Couldn't read write-buf-size\n");

		err = of_property_read_u32(dev->of_node,
					   "chip_family",
					   &act->chip_family);
		if (err < 0 && err != -EINVAL)
		    dev_err(dev, "Couldn't read chip_family\n");

		num_elems = of_property_count_elems_of_size(dev->of_node,
							    "multiboot",
							    sizeof(uint32_t));
		if (num_elems < 0) {
			act->mboot.enabled = false;
		} else {
			act->mboot.enabled = true;
			if (num_elems < 2) {
				dev_err(dev, "Too few multiboot params\n");
				err = -ENODEV;
				goto fail_init_device;
			}
			if (of_property_read_u32_index(dev->of_node,
						       "multiboot",
						       0, &act->mboot.reg) < 0) {
				dev_err(dev, "Can't read multiboot timer reg\n");
				err = -ENODEV;
				goto fail_init_device;
			}
			if (of_property_read_u32_index(dev->of_node,
						       "multiboot",
						       1, &mask) < 0) {
				dev_err(dev, "Can't read multiboot timer mask\n");
				err = -ENODEV;
				goto fail_init_device;
			}
			act->mboot.mask = (uint8_t)mask;
			if (num_elems == 3)
				act->mboot.active_low = false;
			else {
				uint32_t low;

				if (of_property_read_u32_index(dev->of_node,
							       "multiboot",
							       2, &low) < 0) {
					dev_err(dev, "Can't read multiboot timer active low flag\n");
					err = -ENODEV;
					goto fail_init_device;
				}
				act->mboot.active_low = (bool)low;
			}
		}
		// currently no device tree property for software_reset
		act->software_reset = false;
	}
	else if (dev->platform_data) {
		pdata = (struct cisco_act_platform_data*) dev->platform_data;
                if (pdata->write_buf_size)
			writebuf_size = pdata->write_buf_size;
		act->mboot = pdata->mboot;
		act->chip_family = pdata->chip_family;
		act->software_reset = pdata->software_reset;
	}

	if (writebuf_size < WRITE_MIN || WRITE_MAX < writebuf_size) {
		err = -EINVAL;
		dev_err(dev, "invalid write_buf_size (must be between %u and %u)\n",
			WRITE_MIN, WRITE_MAX);
		goto fail_init_device;
	}
	writebuf = devm_kzalloc(dev, writebuf_size, GFP_KERNEL);
	if (!writebuf) {
		err = -ENOMEM;
		goto fail_init_device;
	}

	act->writebuf_size = writebuf_size;
	act->writebuf = writebuf;

	act->host_storage = devm_kzalloc(dev, FPGA_HOST_STORAGE_SIZE, GFP_KERNEL);
	act->host_storage_size = 	(u8) FPGA_HOST_STORAGE_SIZE;
	act->host_storage_addr_start = (u32) FPGA_HOST_STORAGE_ADDR_START;
	act->host_storage_addr_end =   (u32) (act->host_storage_addr_start +
					       act->host_storage_size);

	err = cisco_act_init_device(act);
	if (err)
		goto fail_init_device;

	sysfs_bin_attr_init(&act->bin);
	act->bin.attr.name = "data";
	act->bin.attr.mode = 0644;
	act->bin.read = act_bin_read;
	act->bin.write = act_bin_write;
	act->bin.size = writebuf_size;

	err = sysfs_create_bin_file(&act->dev->kobj, &act->bin);
	if (err)
		goto fail_create_bin_file;

	for (i = 0; i < ARRAY_SIZE(cisco_act_attrs); ++i) {
		err = device_create_file(act->dev, cisco_act_attrs[i]);
		if (err < 0)
			goto fail_create_device_file;
	}

	act->bus_ops = bus_ops;
	dev_set_drvdata(dev, act);

	reset_gpio = devm_gpiod_get_optional(dev, "reset", GPIOD_OUT_LOW);

	/* bring into reset then back out to get to known state after kernel init */
	if (reset_gpio) {
		gpiod_set_value_cansleep(reset_gpio, 1);
		udelay(1000);
		gpiod_set_value_cansleep(reset_gpio, 0);
	}

	bus_select_gpio = devm_gpiod_get_optional(dev, "bus-select", GPIOD_OUT_LOW);
	if (bus_select_gpio)
		dev_info(dev, "FPGA bus set to secondary\n");

	return act;

fail_create_device_file:
	while (i > 0)
		device_remove_file(dev, cisco_act_attrs[--i]);
fail_create_bin_file:
	put_device(act->dev);
fail_init_device:
	return ERR_PTR(err);

}

int cisco_act_common_lateinit(struct device* dev)
{
	int err;
	uint8_t val;
	struct act_data* act = dev_get_drvdata(dev);

	if (act->mboot.enabled) {
		err = act_reg_read(dev, act->mboot.reg, &val, 1);
		if (err < 0) {
			dev_err(dev, "Can't read multiboot timer register\n");
			return err;
		}

		if (act->mboot.active_low)
			val &= ~act->mboot.mask;
		else
			val |= act->mboot.mask;

		err = act_reg_write(dev, act->mboot.reg, &val, 1);
		if (err < 0) {
			dev_err(dev, "Can't write multiboot timer register\n");
			return err;
		}
		dev_info(dev, "Multiboot timer disabled\n");
	}

	if (act->software_reset) {
		err = act_software_reset(dev);
		if (err < 0) {
			return err;
		}
	}

	return 0;
}

MODULE_AUTHOR("William H. Hauser <whauser@meraki.com>");
MODULE_DESCRIPTION("Device driver for ACT2");
MODULE_LICENSE("GPL");
