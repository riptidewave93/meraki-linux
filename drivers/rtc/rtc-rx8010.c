/*
 * An I2C driver for the Epson RX8010 RTC
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Based on: rtc-rx8581.c
 * Copyright 2008 GE Intelligent Platforms Embedded Systems, Inc.
 */

#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/bcd.h>
#include <linux/rtc.h>
#include <linux/log2.h>
#include <linux/delay.h>

#define DRV_VERSION "0.1"

#define RX8010_REG_BASE		0x10
#define RX8010_REG_SC		0x10 /* Second in BCD */
#define RX8010_REG_MN		0x11 /* Minute in BCD */
#define RX8010_REG_HR		0x12 /* Hour in BCD */
#define RX8010_REG_DW		0x13 /* Day of Week */
#define RX8010_REG_DM		0x14 /* Day of Month in BCD */
#define RX8010_REG_MO		0x15 /* Month in BCD */
#define RX8010_REG_YR		0x16 /* Year in BCD */
#define RX8010_REG_RSV17	0x17 /* Reserved */
#define RX8010_REG_AMN		0x18 /* Alarm Min in BCD*/
#define RX8010_REG_AHR		0x19 /* Alarm Hour in BCD */
#define RX8010_REG_ADM		0x1A
#define RX8010_REG_ADW		0x1A
#define RX8010_REG_TMR0		0x1B
#define RX8010_REG_TMR1		0x1C
#define RX8010_REG_EXT		0x1D /* Extension Register */
#define RX8010_REG_FLAG		0x1E /* Flag Register */
#define RX8010_REG_CTRL		0x1F /* Control Register */
#define RX8010_REG_RSV30	0x30 /* Reserved */
#define RX8010_REG_RSV31	0x31 /* Reserved */
#define RX8010_REG_RSV32	0x32 /* Reserved */
#define RX8010_REG_RAM		0x20 /* RAM */

/* Extension Register bit definitions */
#define RX8010_EXT_FSEL1	0x80 /* Frequency Select */
#define RX8010_EXT_FSEL0	0x40 /* Frequency Select */
#define RX8010_EXT_USEL		0x20 /* Update Interrupt Select */
#define RX8010_EXT_TE		0x10 /* Timer Enbale */
#define RX8010_EXT_WADA		0x08 /* Week Alarm/Day Alarm */
#define RX8010_EXT_TSEL2	0x04 /* Timer Select */
#define RX8010_EXT_TSEL1	0x02 /* Timer select */
#define RX8010_EXT_TSEL0	0x01 /* Timer select */

/* Flag Register bit definitions */
#define RX8010_FLAG_UF		0x20 /* Update */
#define RX8010_FLAG_TF		0x10 /* Timer */
#define RX8010_FLAG_AF		0x08 /* Alarm */
#define RX8010_FLAG_VLF		0x02 /* Voltage Low */

/* Control Register bit definitions */
#define RX8010_CTRL_TEST	0x80 /* TEST bit */
#define RX8010_CTRL_STOP	0x40 /* STOP bit */
#define RX8010_CTRL_UIE		0x20 /* Update Interrupt Enable */
#define RX8010_CTRL_TIE		0x10 /* Timer Interrupt Enable */
#define RX8010_CTRL_AIE		0x08 /* Alarm Interrupt Enable */
#define RX8010_CTRL_TSTP	0x04 /* Timer stop */

/* User Register */
#define RX8010_NVRAM		0x20 /* User Resigter start address */

#define NVRAM_SIZE			16

/* Oscillation start time */
#define OSC_START_TIME_IN_MS    1000

static struct i2c_driver rx8010_driver;

/*----------------------------------------------------------------------*/
static ssize_t rx8010_nvram_read(
		struct file *filp,
		struct kobject *kobj,
		struct bin_attribute *attr,
		char *buf,
		loff_t off,
		size_t count)
{
	struct i2c_client	*client;
	int					result;

	client = kobj_to_i2c_client(kobj);

	if (unlikely(off >= NVRAM_SIZE))
		return 0;
	if ((off + count) > NVRAM_SIZE)
		count = NVRAM_SIZE - off;
	if (unlikely(!count))
		return count;

	result = i2c_smbus_read_i2c_block_data(client, RX8010_NVRAM + off, count, buf);
	if (result < 0)
		dev_err(&client->dev, "%s error %d\n", "nvram read", result);
	return result;
}

static ssize_t rx8010_nvram_write(
		struct file *filp,
		struct kobject *kobj,
		struct bin_attribute *attr,
		char *buf,
		loff_t off,
		size_t count)
{
	struct i2c_client	*client;
	int					result;

	client = kobj_to_i2c_client(kobj);

	if (unlikely(off >= NVRAM_SIZE))
		return -EFBIG;
	if ((off + count) > NVRAM_SIZE)
		count = NVRAM_SIZE - off;
	if (unlikely(!count))
		return count;

	result = i2c_smbus_write_i2c_block_data(client, RX8010_NVRAM + off, count, buf);
	if (result < 0) {
		dev_err(&client->dev, "%s error %d\n", "nvram write", result);
		return result;
	}
	return count;
}

static struct bin_attribute nvram = {
	.attr = {
		.name	= "nvram",
		.mode	= S_IRUGO | S_IWUSR,
	},

	.read	= rx8010_nvram_read,
	.write	= rx8010_nvram_write,
	.size	= NVRAM_SIZE,
};

/*
 * In the routines that deal directly with the rx8010 hardware, we use
 * rtc_time -- month 0-11, hour 0-23, yr = calendar year-epoch.
 */
static int rx8010_get_datetime(struct i2c_client *client, struct rtc_time *tm)
{
	unsigned char date[7];
	int err;

	/* Now read time and date */
	err = i2c_smbus_read_i2c_block_data(client, RX8010_REG_SC,
		7, date);
	if (err < 0) {
		dev_err(&client->dev, "Unable to read date\n");
		return -EIO;
	}

	dev_dbg(&client->dev,
		"%s: raw data is sec=%02x, min=%02x, hr=%02x, "
		"wday=%02x, mday=%02x, mon=%02x, year=%02x\n",
		__func__,
		date[0], date[1], date[2], date[3], date[4], date[5], date[6]);

	tm->tm_sec  = bcd2bin(date[RX8010_REG_SC - RX8010_REG_BASE] & 0x7F);
	tm->tm_min  = bcd2bin(date[RX8010_REG_MN - RX8010_REG_BASE] & 0x7F);
	tm->tm_hour = bcd2bin(date[RX8010_REG_HR - RX8010_REG_BASE] & 0x3F); /* rtc hr 0-23 */
	tm->tm_wday = ilog2  (date[RX8010_REG_DW - RX8010_REG_BASE] & 0x7F);
	tm->tm_mday = bcd2bin(date[RX8010_REG_DM - RX8010_REG_BASE] & 0x3F);
	tm->tm_mon  = bcd2bin(date[RX8010_REG_MO - RX8010_REG_BASE] & 0x1F) - 1; /* rtc mn 1-12 */
	tm->tm_year = bcd2bin(date[RX8010_REG_YR - RX8010_REG_BASE]);
	if (tm->tm_year < 70)
		tm->tm_year += 100;	/* assume we are in 1970...2069 */

	dev_dbg(&client->dev, "%s: tm is secs=%d, mins=%d, hours=%d, "
		"mday=%d, mon=%d, year=%d, wday=%d\n",
		__func__,
		tm->tm_sec, tm->tm_min, tm->tm_hour,
		tm->tm_mday, tm->tm_mon, tm->tm_year, tm->tm_wday);

	err = rtc_valid_tm(tm);
	if (err < 0) {
		dev_err(&client->dev, "retrieved date/time is not valid.\n");
	}

	return err;
}

static int rx8010_set_datetime(struct i2c_client *client, struct rtc_time *tm)
{
	int data, err;
	unsigned char buf[7];

	dev_dbg(&client->dev, "%s: secs=%d, mins=%d, hours=%d, "
		"mday=%d, mon=%d, year=%d, wday=%d\n",
		__func__,
		tm->tm_sec, tm->tm_min, tm->tm_hour,
		tm->tm_mday, tm->tm_mon+1, tm->tm_year+1900, tm->tm_wday);

	/* hours, minutes and seconds */
	buf[RX8010_REG_SC - RX8010_REG_BASE] = bin2bcd(tm->tm_sec);
	buf[RX8010_REG_MN - RX8010_REG_BASE] = bin2bcd(tm->tm_min);
	buf[RX8010_REG_HR - RX8010_REG_BASE] = bin2bcd(tm->tm_hour);

	buf[RX8010_REG_DM - RX8010_REG_BASE] = bin2bcd(tm->tm_mday);

	/* month, 1 - 12 */
	buf[RX8010_REG_MO - RX8010_REG_BASE] = bin2bcd(tm->tm_mon + 1);

	/* year and century */
	buf[RX8010_REG_YR - RX8010_REG_BASE] = bin2bcd(tm->tm_year % 100);
	buf[RX8010_REG_DW - RX8010_REG_BASE] = (0x1 << tm->tm_wday);

	/* Stop the clock */
	data = i2c_smbus_read_byte_data(client, RX8010_REG_CTRL);
	if (data < 0) {
		dev_err(&client->dev, "Unable to read control register\n");
		return -EIO;
	}

	err = i2c_smbus_write_byte_data(client, RX8010_REG_CTRL,
		(data | RX8010_CTRL_STOP));
	if (err < 0) {
		dev_err(&client->dev, "Unable to write control register\n");
		return -EIO;
	}

	/* write register's data */
	err = i2c_smbus_write_i2c_block_data(client, RX8010_REG_SC, 7, buf);
	if (err < 0) {
		dev_err(&client->dev, "Unable to write to date registers\n");
		return -EIO;
	}

	/* get VLF and clear it */
	data = i2c_smbus_read_byte_data(client, RX8010_REG_FLAG);
	if (data < 0) {
		dev_err(&client->dev, "Unable to read flag register\n");
		return -EIO;
	}

	err = i2c_smbus_write_byte_data(client, RX8010_REG_FLAG,
		(data & ~(RX8010_FLAG_VLF)));
	if (err != 0) {
		dev_err(&client->dev, "Unable to write flag register\n");
		return -EIO;
	}

	/* Restart the clock */
	data = i2c_smbus_read_byte_data(client, RX8010_REG_CTRL);
	if (data < 0) {
		dev_err(&client->dev, "Unable to read control register\n");
		return -EIO;
	}

	err = i2c_smbus_write_byte_data(client, RX8010_REG_CTRL,
		(data & ~(RX8010_CTRL_STOP)));
	if (err != 0) {
		dev_err(&client->dev, "Unable to write control register\n");
		return -EIO;
	}

	return 0;
}

static int rx8010_rtc_read_time(struct device *dev, struct rtc_time *tm)
{
	return rx8010_get_datetime(to_i2c_client(dev), tm);
}

static int rx8010_rtc_set_time(struct device *dev, struct rtc_time *tm)
{
	return rx8010_set_datetime(to_i2c_client(dev), tm);
}

static const struct rtc_class_ops rx8010_rtc_ops = {
	.read_time	= rx8010_rtc_read_time,
	.set_time	= rx8010_rtc_set_time,
};

static int rx8010_init_client(struct i2c_client *client)
{
	int err = 0;

	msleep(OSC_START_TIME_IN_MS);

	/* Write specific pattern in these reserved registers */
	err = i2c_smbus_write_byte_data(client, RX8010_REG_RSV17, 0x58);
	if (err < 0) {
		dev_err(&client->dev, "Unable to write RSV17 register\n");
		return -EIO;
	}
	err = i2c_smbus_write_byte_data(client, RX8010_REG_RSV30, 0);
	if (err < 0) {
		dev_err(&client->dev, "Unable to write RSV30 register\n");
		return -EIO;
	}
	err = i2c_smbus_write_byte_data(client, RX8010_REG_RSV31, 0x18);
	if (err < 0) {
		dev_err(&client->dev, "Unable to write RSV31 register\n");
		return -EIO;
	}
	err = i2c_smbus_write_byte_data(client, RX8010_REG_RSV32, 0);
	if (err < 0) {
		dev_err(&client->dev, "Unable to write RSV32 register\n");
		return -EIO;
	}

	/* Initialization steps for clock ONLY */
	err = i2c_smbus_write_byte_data(client, RX8010_REG_EXT, 0x04);
	if (err < 0) {
		dev_err(&client->dev, "Unable to write FLAG register\n");
		return -EIO;
	}

	err = i2c_smbus_write_byte_data(client, RX8010_REG_FLAG, 0);
	if (err < 0) {
		dev_err(&client->dev, "Unable to write FLAG register\n");
		return -EIO;
	}

	err = i2c_smbus_write_byte_data(client, RX8010_REG_CTRL, 0x2);
	if (err < 0) {
		dev_err(&client->dev, "Unable to write CTRL register\n");
		return -EIO;
	}

	err = i2c_smbus_write_byte_data(client, RX8010_REG_CTRL, 0);
	if (err != 0) {
		dev_err(&client->dev, "Unable to write CTRL register\n");
		return -EIO;
	}

	return err;
}

static int rx8010_probe(struct i2c_client *client,
			const struct i2c_device_id *id)
{
	int err;
	struct rtc_device *rtc = NULL;

	dev_dbg(&client->dev, "%s\n", __func__);

	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		err = -ENODEV;
		goto err_exit;
	}

	dev_info(&client->dev, "chip found, driver version " DRV_VERSION "\n");

	i2c_set_clientdata(client, rtc);

	err = rx8010_init_client(client);
	if (err) {
		goto err_unset;
	}

	rtc = rtc_device_register(rx8010_driver.driver.name, &client->dev, 
					&rx8010_rtc_ops, THIS_MODULE);

	if (IS_ERR(rtc)) {
		err = PTR_ERR(rtc);
		goto err_exit;
	}

	err = sysfs_create_bin_file(&client->dev.kobj, &nvram);
	if (err == 0) {
		dev_info(&client->dev, "16 bytes nvram\n");
	}

	return 0;

err_unset:
	i2c_set_clientdata(client, NULL);

err_exit:
	return err;
}

static int rx8010_remove(struct i2c_client *client)
{
	struct rtc_device *rtc = i2c_get_clientdata(client);

	sysfs_remove_bin_file(&client->dev.kobj, &nvram);
	rtc_device_unregister(rtc);
	i2c_set_clientdata(client, NULL);

	return 0;
}

static const struct i2c_device_id rx8010_id[] = {
	{ "rx8010", 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, rx8010_id);

static struct i2c_driver rx8010_driver = {
	.driver		= {
		.name	= "rtc-rx8010",
		.owner	= THIS_MODULE,
	},
	.probe		= rx8010_probe,
	.remove		= rx8010_remove,
	.id_table	= rx8010_id,
};

static int __init rx8010_init(void)
{
	return i2c_add_driver(&rx8010_driver);
}

static void __exit rx8010_exit(void)
{
	i2c_del_driver(&rx8010_driver);
}

MODULE_AUTHOR("Richard Pan <richard.pan@vivotek.com>");
MODULE_DESCRIPTION("Epson RX-8010 RTC driver");
MODULE_LICENSE("GPL");
MODULE_VERSION(DRV_VERSION);

module_init(rx8010_init);
module_exit(rx8010_exit);
