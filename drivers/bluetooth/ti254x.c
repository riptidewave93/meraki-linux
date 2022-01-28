/*
 * drivers/bluetooth/ti254x.c
 *
 * Firmware upload support Texas Instruments CC2540 and CC2541
 *
 * Copyright (C) 2014 Cisco, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */
#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/module.h>
#include <linux/debugfs.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/gpio.h>
#include <linux/of_gpio.h>
#include <linux/uaccess.h>
#include <linux/firmware.h>
#include <linux/crypto.h>
#include <linux/scatterlist.h>
#include <linux/string.h>
#include <net/bluetooth/bluetooth.h>
#include <net/bluetooth/hci_core.h>
#include "ti254x_dc.h"

#define TI254X_UNKNOWN_IMAGE -1
#define TI254X_INVALID_IMAGE -2
#define TI254X_INVALID_FLASH -3

#define HASH_TYPE        "sha1"
#define HASH_SIZE        20
#define WRITE_RETRY_MAX  2
#define DEV_NAME         "ti254x"
#define FW_IMAGE_NAME    "ti254x.sbin"

static ssize_t flash_read(struct file *filp, struct kobject *kobj,
		struct bin_attribute *bin_attr,
		char *buf, loff_t off, size_t size);

static int ti254x_probe(struct platform_device *pdev);

static int ti254x_remove(struct platform_device *pdev);

static ssize_t ti254x_control_show(struct kobject *kobj,
		struct kobj_attribute *attr,
		char *buf);

static ssize_t ti254x_control_store(struct kobject *kobj,
		struct kobj_attribute *attr,
		const char *buf, size_t size);

static ssize_t ti254x_debug_show(struct kobject *kobj,
		struct kobj_attribute *attr,
		char *buf);

static ssize_t ti254x_debug_store(struct kobject *kobj,
		struct kobj_attribute *attr,
		const char *buf, size_t size);

struct ti254x_work_struct {
       struct work_struct work;
       void *arg;
};

struct ti254x_private_data {
	struct kobj_attribute control_attr;
	struct kobj_attribute debug_attr;
	struct ti254x_dc_dev dc_dev;
	struct device *dev;
	struct ti254x_work_struct work;
	struct workqueue_struct *wq;
};

struct ti254x_platform_data {
	unsigned int sda_pin;
	unsigned int scl_pin;
	unsigned int reset_pin;
	u8 reset_active_level;
};

#ifdef FORCE_TI254X_DEVICE
struct ti254x_platform_data ti254x_data = {
	.sda_pin = 7,
	.scl_pin = 8,
	.reset_pin = 25,
	.reset_active_level = 0,
};
#endif

struct ti254x_private_data ti254x_private = {
	.control_attr = __ATTR(control,
			0664,
			ti254x_control_show,
			ti254x_control_store),
	.debug_attr = __ATTR(debug,
			0664,
			ti254x_debug_show,
			ti254x_debug_store),
	.dc_dev = {
		.sda_pin = 7,
		.scl_pin = 8,
		.reset_pin = 25,
		.reset_active_level = 0,
		.firmware = NULL,
		.retries = 0,
		.commands = 0,
		.lock = ATOMIC_INIT(1),
	},
	.dev = NULL,
};

static struct bin_attribute flash_attr = {
	.attr = {
		.name = DEV_NAME,
		.mode = S_IRUGO,
	},
	.read = flash_read,

	.write = NULL,
};

#ifdef FORCE_TI254X_DEVICE
static struct platform_device ti254x_device = {
	.name = DEV_NAME,
	.id   = 0,
	.num_resources = 0,
	.dev = {
		.platform_data = &ti254x_data,
	},
	.resource = NULL,
};
#endif

static const struct of_device_id of_ti254x_tbl[] = {
	{ .compatible = "ti,cc2540",},
	{ .compatible = "ti,cc2541",},
	{ }
};

static struct platform_driver ti254x_driver = {
	.driver = {
		.name =  DEV_NAME,
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(of_ti254x_tbl),
	},
	.probe = ti254x_probe,
	.remove = ti254x_remove,
};

static ssize_t flash_read(struct file *filp, struct kobject *kobj,
		struct bin_attribute *bin_attr,
		char *buf, loff_t off, size_t size)
{
	u8 debug_status;
	ssize_t retval;
	struct ti254x_dc_dev *dc_dev = bin_attr->private;

	if (!ti254x_dc_lock(dc_dev)) {
		retval = -EBUSY;
		goto out;
	}
	if (off > dc_dev->flash_size) {
		retval = 0;
		goto out;
	}
	if (off + size > dc_dev->flash_size)
		size = dc_dev->flash_size - off;

	ti254x_dc_init(dc_dev, &debug_status);
	retval = ti254x_dc_flash_read(dc_dev, buf, off, size);
out:
	ti254x_dc_resume(dc_dev);
	ti254x_dc_unlock(dc_dev);

	return retval;
}

static int ti254x_calc_hash(u8 const *const data, size_t size, u8 *hash_buf)
{
	struct scatterlist sg;
	struct hash_desc desc;
	int status = 0;

	sg_init_one(&sg, data, size);
	desc.tfm = crypto_alloc_hash(HASH_TYPE, 0, CRYPTO_ALG_ASYNC);
	desc.flags = 0;
	if (IS_ERR(desc.tfm)) {
		BT_ERR("Failed to alloc hash\n");
		status = -ENOMEM;
	} else {
		if ((status = crypto_hash_init(&desc))) {
			BT_ERR("ti254x: crypto_hash_init failed [%d]", status);
		}

		if ((status = crypto_hash_update(&desc, &sg, size))) {
			BT_ERR("ti254x: crypto_hash_init failed [%d]", status);
			goto hash_done;
		}

		if ((status = crypto_hash_final(&desc, hash_buf))) {
			BT_ERR("ti254x: crypto_hash_init failed [%d]", status);
		}

hash_done:
		crypto_free_hash(desc.tfm);
	}

	return status;
}

static u8 *ti254x_image_get_hash_ptr(const struct firmware *image)
{
	return ((u8 *)image->data) + (image->size - HASH_SIZE);

}

static u8 *ti254x_image_get_data_ptr(const struct firmware *image)
{
	return (u8 *)image->data;
}

static size_t ti254x_image_get_size(const struct firmware *image)
{
	return image->size;
}

static int ti254x_verify_image(const struct firmware *image)
{
	u8 *hash_buf;
	u8 *image_data;
	int status = 0;

	const u8 *image_hash_ptr = ti254x_image_get_hash_ptr(image);
	const u8 *image_data_ptr = ti254x_image_get_data_ptr(image);
	size_t image_size = ti254x_image_get_size(image);

	image_data = kmalloc(image_size - HASH_SIZE, GFP_KERNEL);
	hash_buf = kmalloc(HASH_SIZE, GFP_KERNEL);
	memcpy(image_data, image_data_ptr, image_size - HASH_SIZE);
	status  = ti254x_calc_hash(image_data,
				   image_size - HASH_SIZE,
				   hash_buf);

	status = memcmp(image_hash_ptr, hash_buf, HASH_SIZE);

	kfree(image_data);
	kfree(hash_buf);

	return status;
}

static int ti254x_verify_flash(struct ti254x_dc_dev *dev,
		u8 const *image_hash_ptr, size_t image_size)
{
	u8 hash_buf[HASH_SIZE];
	int status = -1;

	ti254x_dc_flash_read(dev, hash_buf,
			TI254X_HASH_OFFSET(dev->flash_size),
			HASH_SIZE);
	status = memcmp(image_hash_ptr, hash_buf, HASH_SIZE);
	return status;
}

int ti254x_flash_write(struct ti254x_private_data *pdata,
		size_t image_size,
		u8 const *image_hash_ptr)
{
	u8 debug_status;
	u8 *image_buf;
	size_t image_data_size;
	u8 hash_buf[HASH_SIZE];
	int status = 0;

	image_buf = kmalloc(image_size, GFP_KERNEL);
	if (!image_buf)
		return -ENOMEM;

	/* Erase the chip */
	if (!ti254x_dc_chip_erase(&pdata->dc_dev, &debug_status)) {
		BT_INFO("ti254x: loading firmware image");

		image_data_size = image_size - HASH_SIZE;

		/* Write the firmware image to flash (~2 seconds) */
		status = ti254x_dc_flash_write(&pdata->dc_dev,
				pdata->dc_dev.firmware->data,
				0,
				image_size);
		if (status) {
			BT_ERR("ti254x: firmware flash write failed [%d]", status);
			goto flash_write_cleanup;
		}

		BT_INFO("ti254x: Wrote firmware image. Verifying integrity...");

		/* reset DMA controller by forcing a chip reset */
		ti254x_dc_reset(&pdata->dc_dev);
		ti254x_dc_init(&pdata->dc_dev, &debug_status);

		/* Read the image back into a buffer (~6 seconds) */
		ti254x_dc_flash_read(&pdata->dc_dev,
				image_buf,
				0,
				image_data_size);

		/* Make sure what was written & read matches the firmware image */
		status = memcmp(image_buf, pdata->dc_dev.firmware->data, image_data_size);
		if (status) {
			BT_ERR("ti254x: written image didn't match upgrade image");
			goto flash_write_cleanup;
		}

		/* reset DMA controller by forcing a chip reset */
		ti254x_dc_reset(&pdata->dc_dev);
		ti254x_dc_init(&pdata->dc_dev, &debug_status);

		ti254x_dc_flash_read(&pdata->dc_dev,
				hash_buf,
				image_data_size,
				HASH_SIZE);

		/* Write the hash to flash at a special offset */
		status = ti254x_dc_flash_write(&pdata->dc_dev,
				hash_buf,
				TI254X_HASH_OFFSET(pdata->dc_dev.flash_size),
				HASH_SIZE);

		if (status != 0) {
			BT_ERR("ti254x: hash flash write failed [%d]", status);
		} else {
			BT_ERR("ti254x: firmware successfully upgraded");
		}
	} else {
		BT_ERR(" ti254x_flash_write: failed to erase flash");
	}

flash_write_cleanup:
	kfree(image_buf);
	return status;
}

static int ti254x_load_image(struct ti254x_private_data *pdata)
{
	int status;
	const u8 *image_hash_ptr;
	size_t image_size;
	size_t retry_count = 0;

	status = request_firmware(&pdata->dc_dev.firmware,
			FW_IMAGE_NAME, pdata->dev);
	if (status) {
		BT_ERR("ti254x: request_firmware failed");
		return TI254X_UNKNOWN_IMAGE;
	}

	if (!ti254x_verify_image(pdata->dc_dev.firmware)) {
		image_hash_ptr = ti254x_image_get_hash_ptr(pdata->dc_dev.firmware);
		image_size = ti254x_image_get_size(pdata->dc_dev.firmware);
		if (ti254x_verify_flash(&pdata->dc_dev,
					image_hash_ptr,
					image_size)) {
			do {
				BT_ERR("ti254x: write flash");
				status = ti254x_flash_write(pdata,
						image_size,
						image_hash_ptr);
				++retry_count;
			} while (status != 0 && retry_count < WRITE_RETRY_MAX);
			if (status)
				status = TI254X_INVALID_FLASH;
		} else {
			BT_INFO("ti254x: firmware is up to date.");
		}
	} else {
		BT_ERR("ti254x: invalid firmware image");
		status = TI254X_INVALID_IMAGE;
	}
	release_firmware(pdata->dc_dev.firmware);
	return status;
}

static void ti254x_work_load_image(struct work_struct *arg)
{
	struct ti254x_work_struct *ti254x_work = (struct ti254x_work_struct *)arg;
	struct ti254x_private_data *data = (struct ti254x_private_data *)ti254x_work->arg;
	struct ti254x_dc_dev *dc_dev = &data->dc_dev;
	u8 debug_status;

	ti254x_dc_init(dc_dev, &debug_status);
	dc_dev->status = ti254x_load_image(data);
	ti254x_dc_reset(dc_dev);
}

static int ti254x_probe(struct platform_device *pdev)
{
	u8 debug_status;
	struct ti254x_platform_data *pdata;
	struct ti254x_private_data *priv;
	struct device_node *np;
	enum of_gpio_flags reset_flags;
	int status = 0;

	np = pdev->dev.of_node;
	priv = &ti254x_private;
	platform_set_drvdata(pdev, priv);

	priv->wq = alloc_workqueue(DEV_NAME, WQ_CPU_INTENSIVE | WQ_SYSFS, 1);
	INIT_WORK((struct work_struct*)&priv->work, ti254x_work_load_image);

	pdata = (struct ti254x_platform_data *)(pdev->dev.platform_data);
	priv->dev = &pdev->dev;

	if (np) {
		BT_DBG("ti254x: of_node init");
		priv->dc_dev.reset_pin = of_get_gpio_flags(np, 0, &reset_flags);
		priv->dc_dev.scl_pin = of_get_gpio(np, 1);
		priv->dc_dev.sda_pin = of_get_gpio(np, 2);
		priv->dc_dev.reset_active_level = reset_flags & 0x1;
	} else if (pdata) {
		BT_DBG("ti254x: platform init");
		priv->dc_dev.sda_pin = pdata->sda_pin;
		priv->dc_dev.scl_pin = pdata->scl_pin;
		priv->dc_dev.reset_pin = pdata->reset_pin;
		priv->dc_dev.reset_active_level = pdata->reset_active_level;
	}

	status = gpio_request(priv->dc_dev.sda_pin, "sda");
	if (status) {
		BT_ERR("ti254x: gpio_request sda failed: %d",
				priv->dc_dev.sda_pin);
		return status;
	}
	status = gpio_request(priv->dc_dev.scl_pin, "scl");
	if (status) {
		BT_ERR("ti254x: gpio_request scl failed: %d",
				priv->dc_dev.scl_pin);

		gpio_free(priv->dc_dev.sda_pin);
		return status;
	}
	status = gpio_request(priv->dc_dev.reset_pin, "reset");
	if (status) {
		BT_ERR("ti254x: gpio_request reset failed: %d",
				priv->dc_dev.reset_pin);

		gpio_free(priv->dc_dev.sda_pin);
		gpio_free(priv->dc_dev.scl_pin);
		return status;
	}

	status = ti254x_dc_init(&priv->dc_dev, &debug_status);
	if (status || !(debug_status & TI254X_DC_DEBUG_MODE)) {
		BT_ERR("ti254x: ti254x_dc_init failed atempting to recover");
	}

	flash_attr.private = &priv->dc_dev;
	flash_attr.size = priv->dc_dev.flash_size;
	status = sysfs_create_bin_file(firmware_kobj, &flash_attr);
	if (sysfs_create_file(&pdev->dev.kobj, &priv->control_attr.attr))
		BT_ERR("ti254x: failed to create control attr");

	if (sysfs_create_file(&pdev->dev.kobj, &priv->debug_attr.attr))
		BT_ERR("ti254x: failed to create debug attr");

	ti254x_dc_reset(&priv->dc_dev);
	return 0;
}

static int ti254x_remove(struct platform_device *pdev)
{
	struct ti254x_private_data *priv;
	priv = platform_get_drvdata(pdev);
	flush_workqueue(priv->wq);
	destroy_workqueue(priv->wq);

	ti254x_dc_reset(&priv->dc_dev);
	gpio_free(priv->dc_dev.sda_pin);
	gpio_free(priv->dc_dev.scl_pin);
	gpio_free(priv->dc_dev.reset_pin);
	return 0;
}

static ssize_t ti254x_debug_show(struct kobject *kobj,
		struct kobj_attribute *attr,
		char *buf)
{
	struct ti254x_dc_dev *dc_dev;
	struct ti254x_private_data *data;

	data = container_of(attr, struct ti254x_private_data, debug_attr);
	dc_dev = &data->dc_dev;

	return sprintf(buf, "Commands: %u\nRetries: %u\n", dc_dev->commands, dc_dev->retries);
}

static ssize_t ti254x_debug_store(struct kobject *kobj,
		struct kobj_attribute *attr,
		const char *buf, size_t size)
{
	return size;
}


static ssize_t ti254x_control_show(struct kobject *kobj,
		struct kobj_attribute *attr,
		char *buf)
{
	struct ti254x_dc_dev *dc_dev;
	struct ti254x_private_data *data;

	data = container_of(attr, struct ti254x_private_data, control_attr);
	dc_dev = &data->dc_dev;

	return sprintf(buf, "%d\n", dc_dev->status);
}

static ssize_t ti254x_control_store(struct kobject *kobj,
		struct kobj_attribute *attr,
		const char *buf, size_t size)
{
	u8 debug_status;
	struct ti254x_dc_dev *dc_dev;
	struct ti254x_private_data *data;

	data = container_of(attr, struct ti254x_private_data, control_attr);
	dc_dev = &data->dc_dev;


	if(buf != NULL) {
		switch (buf[0]) {
			case '0':
				ti254x_dc_init(dc_dev, &debug_status);
				ti254x_dc_chip_erase(dc_dev, &debug_status);
				msleep(200);
				break;
			case '1':
				dc_dev->status = TI254X_UNKNOWN_IMAGE;
				flush_workqueue(data->wq);
				data->work.arg = data;
				queue_work(data->wq, (struct work_struct*)&data->work);
				break;
			default:
				break;
		}
	}

	return size;
}

#ifdef FORCE_TI254X_DEVICE
static void ti254x_register_device(void)
{
	platform_device_register(&ti254x_device);
}
#endif

static int __init ti254x_init(void)
{
	int retval = 0;

#ifdef FORCE_TI254X_DEVICE
	ti254x_register_device();
#endif
	retval = platform_driver_register(&ti254x_driver);

	return retval;
}

static void __exit ti254x_exit(void)
{
#ifdef FORCE_TI254X_DEVICE
	platform_device_del(&ti254x_device);
#endif
	platform_driver_unregister(&ti254x_driver);
	sysfs_remove_bin_file(firmware_kobj, &flash_attr);
}

module_init(ti254x_init);
module_exit(ti254x_exit);
MODULE_LICENSE("GPL");
