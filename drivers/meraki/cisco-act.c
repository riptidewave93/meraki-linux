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
#include <linux/i2c.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/platform_device.h>

/* Min/max number of bytes supported by tam library platform buffer */
#define WRITE_MIN 48
#define WRITE_MAX 259

struct act_data {
	/*
	 * Lock protects against activities from other Linux tasks,
	 * but not from changes by other I2C masters.
	 */
	struct mutex lock;

	struct bin_attribute bin;
	struct device *dev;
	struct i2c_client *data_client;

	u8 *writebuf;
	uint16_t writebuf_size;
};

static const struct i2c_device_id cisco_act_ids[] = {
	{ "act2", 0 },
	{ /* END OF LIST */ }
};
MODULE_DEVICE_TABLE(i2c, cisco_act_ids);

static const struct of_device_id __maybe_unused cisco_act_dt_ids[] = {
	{ .compatible = "cisco,act2", },
	{ /* END OF LIST */ }
};
MODULE_DEVICE_TABLE(of, cisco_act_dt_ids);

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

static struct device_attribute *cisco_act_attrs[] = {
	&dev_attr_buffer_size,
	&dev_attr_lock,
};

/*
 * Specs often allow 5 msec for a page write, sometimes 20 msec;
 * it's important to recover from write timeouts.
 */
static unsigned write_timeout = 5;
module_param(write_timeout, uint, 0);
MODULE_PARM_DESC(write_timeout, "Time (in ms) to try writes (default 5)");

static ssize_t act_data_read(struct act_data *act, char *buf, unsigned offset,
			     size_t count)
{
	struct i2c_client *client = act->data_client;
	struct i2c_msg msg;
	int status;

	memset(&msg, 0, sizeof(struct i2c_msg));

	if(client->adapter->quirks &&
	   count > client->adapter->quirks->max_read_len)
	count = client->adapter->quirks->max_read_len;

	msg.addr = client->addr;
	msg.flags = I2C_M_RD;
	msg.buf = buf;
	msg.len = count;

	/*
	 * Read data from chip, protecting against concurrent updates
	 * from this host, but not from other I2C masters.
	 */
	mutex_lock(&act->lock);
	status = i2c_transfer(client->adapter, &msg, 1);
	mutex_unlock(&act->lock);
	if (status == 1)
		status = count;
	dev_dbg(&client->dev, "read %zu@%d --> %d\n",
		count, offset, status);

	if (status == count)
		return count;

	return -ETIMEDOUT;

}

static ssize_t act_bin_read(struct file *filp, struct kobject *kobj,
			    struct bin_attribute *attr,
			    char *buf, loff_t off, size_t count)
{
	struct act_data *act;

	act = dev_get_drvdata(container_of(kobj, struct device, kobj));
	return act_data_read(act, buf, off, count);
}

static ssize_t act_data_write(struct act_data *act, const char *buf,
			      unsigned offset, size_t count)
{
	struct i2c_client *client = act->data_client;
	struct i2c_msg msg;
	ssize_t status;

	msg.addr = client->addr;
	msg.flags = 0;

	/* msg.buf is u8 and casts will mask the values */
	msg.buf = act->writebuf;

	memcpy(msg.buf, buf, count);
	msg.len = count;

	/*
	 * Write data to chip, protecting against concurrent updates
	 * from this host, but not from other I2C masters.
	 */
	mutex_lock(&act->lock);
	status = i2c_transfer(client->adapter, &msg, 1);
	mutex_unlock(&act->lock);
	if (status == 1)
		status = count;
	dev_dbg(&client->dev, "write %zu@%d --> %zd\n",
		count, offset, status);

	if (status == count)
		return count;

	return -ETIMEDOUT;
}

static ssize_t act_bin_write(struct file *filp, struct kobject *kobj,
			     struct bin_attribute *attr,
			     char *buf, loff_t off, size_t count)
{
	struct act_data *act;

	act = dev_get_drvdata(container_of(kobj, struct device, kobj));
	return act_data_write(act, buf, off, count);
}

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

static int cisco_act_probe(struct i2c_client *client,
			   const struct i2c_device_id *id)
{
	struct act_data *act;
	int err;
	int i;

	act = devm_kzalloc(&client->dev, sizeof(struct act_data), GFP_KERNEL);
	if (!act) {
		printk(KERN_ERR "ACT2 alloc fail");
		return -ENOMEM;
	}

	mutex_init(&act->lock);

	i2c_set_clientdata(client, act);

	err = cisco_act_init_device(act);
	if (err)
		goto fail_init_device;

	act->data_client = client;
	sysfs_bin_attr_init(&act->bin);
	act->bin.attr.name = "data";
	act->bin.attr.mode = 0644;
	act->bin.read = act_bin_read;
	act->bin.write = act_bin_write;
	act->bin.size = WRITE_MIN;

	err = sysfs_create_bin_file(&act->dev->kobj, &act->bin);
	if (err)
		goto fail_create_bin_file;

	for (i = 0; i < ARRAY_SIZE(cisco_act_attrs); ++i) {
		err = device_create_file(act->dev, cisco_act_attrs[i]);
		if (err < 0)
			goto fail_create_device_file;
	}

	err = of_property_read_u16(client->dev.of_node, "write_buf_size", &act->writebuf_size);
	if (err < 0)
		act->writebuf_size = WRITE_MIN;
	else if (act->writebuf_size < WRITE_MIN || WRITE_MAX < act->writebuf_size) {
		err = -EINVAL;
		printk(KERN_ERR "invalid write_buf_size must be between %u and %u\n",
			WRITE_MIN, WRITE_MAX);
		goto fail_alloc_writebuf;
	}
	act->writebuf = devm_kzalloc(&client->dev, act->writebuf_size, GFP_KERNEL);
	if (!act->writebuf) {
		err = -ENOMEM;
		goto fail_alloc_writebuf;
	}

	return 0;

fail_alloc_writebuf:
	sysfs_remove_bin_file(&client->dev.kobj, &act->bin);
fail_create_device_file:
	while (i > 0)
		device_remove_file(&client->dev, cisco_act_attrs[--i]);
fail_create_bin_file:
	put_device(act->dev);
fail_init_device:
	return err;
}

static int cisco_act_remove(struct i2c_client *client)
{
	i2c_unregister_device(client);
	return 0;
}

static struct i2c_driver cisco_act_driver = {
	.driver = {
		.name		= "cisco-act",
		.owner		= THIS_MODULE,
	},
	.probe		= cisco_act_probe,
	.remove		= cisco_act_remove,
	.id_table	= cisco_act_ids,
};

static int __init cisco_act_init(void)
{
	return i2c_add_driver(&cisco_act_driver);
}
module_init(cisco_act_init);

static void __exit cisco_act_exit(void)
{
	i2c_del_driver(&cisco_act_driver);
}
module_exit(cisco_act_exit);

MODULE_AUTHOR("William H. Hauser <whauser@meraki.com>");
MODULE_DESCRIPTION("Device driver for ACT2");
MODULE_LICENSE("GPL");
