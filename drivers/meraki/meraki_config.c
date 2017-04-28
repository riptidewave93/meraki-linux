/*
 *  Meraki board data config driver
 *
 *  Copyright (C) 2013 Cisco Systems, Inc.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 2 of the License.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/i2c.h>
#include <linux/device.h>
#include <linux/i2c-gpio.h>
#include <linux/i2c/at24.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/meraki/board_data.h>

#include <linux/meraki/meraki_config.h>

struct meraki_config_data {
    struct i2c_client *client;
};

static struct memory_accessor *eeprom_accessor = NULL;

static void eeprom_setup(struct memory_accessor *ma, void *context)
{
    *((struct memory_accessor **)context) = ma;
}

#define MERAKI_CONFIG_EEPROM_SIZE  8192

static struct at24_platform_data eeprom_data = {
    .byte_len = MERAKI_CONFIG_EEPROM_SIZE,
    .page_size = 32,
    .flags = AT24_FLAG_ADDR16,
    .setup = eeprom_setup,
    .context = &eeprom_accessor,
};

static struct i2c_board_info eeprom_board_info = {
    I2C_BOARD_INFO("at24", 0x51),
    .platform_data = &eeprom_data,
};

struct board_table_element {
    const char *board_name;
    int board_type;
};

static const struct board_table_element board_table[] = {
    { "meraki_MS220-48 600-21030", MERAKI_BOARD_MS220_48 },
    { "meraki_MS220-48LP 600-21040", MERAKI_BOARD_MS220_48LP },
    { "meraki_MS220-48FP 600-21050", MERAKI_BOARD_MS220_48FP },

    { "meraki_MS220-24 600-20030", MERAKI_BOARD_MS220_24 },
    { "meraki_MS220-24P 600-20040", MERAKI_BOARD_MS220_24P },

    { "meraki_MS320-24 600-20050", MERAKI_BOARD_MS320_24 },
    { "meraki_MS320-24P 600-20060", MERAKI_BOARD_MS320_24P },

    { "meraki_MS320-48 600-21060", MERAKI_BOARD_MS320_48 },
    { "meraki_MS320-48LP 600-21070", MERAKI_BOARD_MS320_48LP },
    { "meraki_MS320-48FP 600-21080", MERAKI_BOARD_MS320_48FP },
};

static DEFINE_MUTEX(fops_lock);

// Too big for stack.  Since reads and writes are mutex-protected we
// can make this a static variable.
static uint8_t meraki_config_buf[MERAKI_CONFIG_EEPROM_SIZE];

static ssize_t meraki_config_board_data_read(struct file* f, char __user *buf, size_t count, loff_t* offset)
{
    ssize_t ret = 0;

    if (!eeprom_accessor)
        return -ENODEV;

    if (count > (MERAKI_CONFIG_EEPROM_SIZE - *offset))
        count = MERAKI_CONFIG_EEPROM_SIZE - *offset;

    mutex_lock(&fops_lock);
    ret = eeprom_accessor->read(eeprom_accessor, meraki_config_buf, *offset, count);
    if (ret > 0 && copy_to_user(buf, meraki_config_buf, ret))
        ret = -EFAULT;
    mutex_unlock(&fops_lock);

    if (ret > 0)
        *offset += ret;

    return ret;
}

static ssize_t meraki_config_board_data_write(struct file* f, const char __user *buf, size_t count, loff_t* offset)
{
    ssize_t ret = 0;

    if (!eeprom_accessor)
        return -ENODEV;

    if (count > (MERAKI_CONFIG_EEPROM_SIZE - *offset))
        count = MERAKI_CONFIG_EEPROM_SIZE - *offset;

    mutex_lock(&fops_lock);
    if (copy_from_user(meraki_config_buf, buf, count))
        ret = -EFAULT;
    else
        ret = eeprom_accessor->write(eeprom_accessor, meraki_config_buf, *offset, count);
    mutex_unlock(&fops_lock);

    if (ret > 0)
        *offset += ret;

    return ret;
}

static loff_t meraki_config_board_data_llseek(struct file* f, loff_t offset, int orig)
{
    loff_t new_offset;

    switch (orig) {
    case SEEK_SET:
        new_offset = offset;
        break;
    case SEEK_CUR:
        new_offset = f->f_pos + offset;
        break;
    case SEEK_END:
        new_offset = MERAKI_CONFIG_EEPROM_SIZE + offset;
        break;
    default:
        return -EINVAL;
    }

    if (new_offset >= MERAKI_CONFIG_EEPROM_SIZE || new_offset < 0)
        return -EINVAL;

    f->f_pos = new_offset;

    return new_offset;
}

int meraki_config_get_product()
{
    struct ar531x_boarddata board_data;
    int i, retries = 5, ret;
    ssize_t count = 0, board_name_len;
    unsigned short csum;

    if (!eeprom_accessor) {
        return MERAKI_BOARD_UNKNOWN;
    }

    do {
        ret = eeprom_accessor->read(eeprom_accessor, ((char *)&board_data) + count, count, sizeof(board_data) - count);
        if (ret < 0)
            continue;

        count += ret;
        if (count >= sizeof(board_data))
            break;
    } while (--retries);

    if (!retries) {
        printk(KERN_INFO "meraki-config: Failure to read from eeprom\n");
        return MERAKI_BOARD_UNKNOWN;
    }

    if (ntohl(board_data.magic) != AR531X_BD_MAGIC) {
        printk(KERN_INFO "meraki-config: Board data has bad magic field\n");
        return MERAKI_BOARD_UNKNOWN;
    }

    csum = calc_board_data_checksum(&board_data);
    if (csum != 0 && csum != ntohs(board_data.cksum)) {
        printk(KERN_INFO "meraki-config: Board data has bad checksum\n");
        return MERAKI_BOARD_UNKNOWN;
    }

    for (i = 0; i < ARRAY_SIZE(board_table); ++i) {
        board_name_len = strlen(board_table[i].board_name);
        if (strncmp(&board_data.boardName[0], board_table[i].board_name, board_name_len) == 0 &&
            (board_data.boardName[board_name_len] == '\0' || board_data.boardName[board_name_len] == '-')) {
            return board_table[i].board_type;
        }
    }

    return MERAKI_BOARD_UNKNOWN;
}

static struct file_operations meraki_config_fileops = {
    .owner = THIS_MODULE,
    .read = meraki_config_board_data_read,
    .write = meraki_config_board_data_write,
    .llseek = meraki_config_board_data_llseek
};

static struct miscdevice meraki_config_miscdevice = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "meraki-board-config",
    .fops = &meraki_config_fileops,
};

int __init meraki_config_probe(struct platform_device *pdev)
{
    struct i2c_adapter *adap;
    struct meraki_config_data *data;
    struct meraki_config_platform_data *pdata;
    int ret;

    if (eeprom_accessor) {
        dev_info(&pdev->dev, "Cannot have more than one meraki-config\n");
        return -ENODEV;
    }
    pdev->id = -1;

    pdata = pdev->dev.platform_data;
    if (!pdata)
        goto fail;

    data = kzalloc(sizeof(*data), GFP_KERNEL);
    if (!data)
        return -ENOMEM;

    adap = i2c_get_adapter(pdata->i2c_adapter_id);
    if (!adap)
        goto fail_adapter;

    data->client = i2c_new_device(adap, &eeprom_board_info);
    if (!data->client)
        goto fail_client;

    ret = misc_register(&meraki_config_miscdevice);
    if (ret) {
        dev_err(&pdev->dev, "Failed to register misc device\n");
        goto fail_misc;
    }
    platform_set_drvdata(pdev, data);
    return 0;

fail_misc:
    i2c_unregister_device(data->client);
fail_client:
    i2c_put_adapter(adap);
fail_adapter:
    kfree(data);
fail:
    printk(KERN_ERR "Failure to add meraki-config\n");
    return -ENODEV;
}

static int __devexit meraki_config_remove(struct platform_device *pdev)
{
    struct meraki_config_data *data;
    struct i2c_adapter *adap;

    data = platform_get_drvdata(pdev);

    misc_deregister(&meraki_config_miscdevice);
    adap = data->client->adapter;
    eeprom_accessor = NULL;

    i2c_unregister_device(data->client);
    i2c_put_adapter(adap);
    kfree(data);

    return 0;
}

static struct platform_driver meraki_config_driver = {
	.driver		= {
		.name	= "meraki-config",
		.owner	= THIS_MODULE,
	},
	.probe		= meraki_config_probe,
	.remove		= __devexit_p(meraki_config_remove),
};

static int __init meraki_config_init(void)
{
    int ret;
    ret = platform_driver_register(&meraki_config_driver);
    if (ret)
        printk(KERN_ERR "meraki-config: driver init failed: %d\n", ret);

    return ret;
}
module_init(meraki_config_init);

static void __exit meraki_config_exit(void)
{
    platform_driver_unregister(&meraki_config_driver);
}
module_exit(meraki_config_exit);


MODULE_AUTHOR("J. Chris Parks <jamparks@cisco.com>");
MODULE_DESCRIPTION("Meraki board data config driver");
MODULE_LICENSE("GPL");
