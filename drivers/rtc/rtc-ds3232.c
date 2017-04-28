/*
 * RTC client/driver for the Maxim/Dallas DS3232 Real-Time Clock over I2C
 *
 * Based on DS1374 code by Scott Wood <scottwood@freescale.com> and
 * DS1742 code by Atsushi Nemoto <anemo@mba.ocn.ne.jp>
 *
 * Copyright (C) 2009 Freescale Semiconductor, Inc. All rights reserved.
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */
/*
 * It would be more efficient to use i2c msgs/i2c_transfer directly but, as
 * recommened in .../Documentation/i2c/writing-clients section
 * "Sending and receiving", using SMBus level communication is preferred.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/i2c.h>
#include <linux/rtc.h>
#include <linux/bcd.h>
#include <linux/workqueue.h>

#define DS3232_REG_SECONDS	0x00
#define DS3232_REG_MINUTES	0x01
#define DS3232_REG_HOURS	0x02
#define DS3232_REG_AMPM		0x02
#define DS3232_REG_DAY		0x03
#define DS3232_REG_DATE		0x04
#define DS3232_REG_MONTH	0x05
#define DS3232_REG_CENTURY	0x05
#define DS3232_REG_YEAR		0x06
#define DS3232_REG_CTRL1	0x0E
#define DS3232_REG_CTRL2_STAT	0x0F

struct ds3232 {
	struct i2c_client *client;
	struct rtc_device *rtc;
	struct work_struct work;

	/* The mutex protects alarm operations, and prevents a race
	 * between the enable_irq() in the workqueue and the free_irq()
	 * in the remove function.
	 */
	struct mutex mutex;
	int exiting;
};

static struct i2c_driver ds3232_driver;

static int ds3232_check_rtc_status(struct i2c_client *client)
{
	/* To be filled later */
	return 0;
}

static int ds3232_read_time(struct device *dev, struct rtc_time *time)
{
	struct i2c_client *client = to_i2c_client(dev);
	int ret;
	u8 buf[7];
	unsigned int year, month, day, hour, minute, second;
	unsigned int week, twelve_hr, am_pm;
	unsigned int century, add_century = 0;

	ret = i2c_smbus_read_i2c_block_data(client, DS3232_REG_SECONDS, 7, buf);

	if (ret < 0)
		return ret;
	if (ret < 7)
		return -EIO;

	second = buf[0];
	minute = buf[1];
	hour = buf[2];
	week = buf[3];
	day = buf[4];
	month = buf[5];
	year = buf[6];

	/* Extract additional information for AM/PM and century */

	twelve_hr = hour & 0x40;
	am_pm = hour & 0x20;
	century = month & 0x80;

	/* Write to rtc_time structure */

	time->tm_sec = bcd2bin(second);
	time->tm_min = bcd2bin(minute);
	if (twelve_hr) {
		/* Convert to 24 hr */
		if (am_pm)
			time->tm_hour = bcd2bin(hour & 0x10) + 12;
		else
			time->tm_hour = bcd2bin(hour & 0xBF);
	} else {
		time->tm_hour = bcd2bin(hour);
	}

	time->tm_wday = bcd2bin(week);
	time->tm_mday = bcd2bin(day);
	time->tm_mon = bcd2bin(month & 0x7F);
	if (century)
		add_century = 100;

	time->tm_year = bcd2bin(year) + add_century;

	return 0;
}

static int ds3232_set_time(struct device *dev, struct rtc_time *time)
{
	struct i2c_client *client = to_i2c_client(dev);
	u8 buf[7];

	/* Extract time from rtc_time and load into ds3232*/

	buf[0] = bin2bcd(time->tm_sec);
	buf[1] = bin2bcd(time->tm_min);
	buf[2] = bin2bcd(time->tm_hour);
	buf[3] = bin2bcd(time->tm_wday); /* Day of the week */
	buf[4] = bin2bcd(time->tm_mday); /* Date */
	buf[5] = bin2bcd(time->tm_mon);
	if (time->tm_year >= 100) {
		buf[5] |= 0x80;
		buf[6] = bin2bcd(time->tm_year - 100);
	} else {
		buf[6] = bin2bcd(time->tm_year);
	}

	return i2c_smbus_write_i2c_block_data(client,
					      DS3232_REG_SECONDS, 7, buf);
}

static int ds3232_read_alarm(struct device *dev, struct rtc_wkalrm *alarm)
{
	/* To be filled later */
	return 0;
}

static int ds3232_set_alarm(struct device *dev, struct rtc_wkalrm *alarm)
{
	/* To be filled later */
	return 0;
}

static irqreturn_t ds3232_irq(int irq, void *dev_id)
{
	struct i2c_client *client = dev_id;
	struct ds3232 *ds3232 = i2c_get_clientdata(client);

	disable_irq_nosync(irq);
	schedule_work(&ds3232->work);
	return IRQ_HANDLED;
}

static void ds3232_work(struct work_struct *work)
{
	/* To be filled later */
}

static int ds3232_ioctl(struct device *dev, unsigned int cmd, unsigned long arg)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct ds3232 *ds3232 = i2c_get_clientdata(client);
	int ret = -ENOIOCTLCMD;

	mutex_lock(&ds3232->mutex);
	/* To be filled later */
	mutex_unlock(&ds3232->mutex);

	return ret;
}

static const struct rtc_class_ops ds3232_rtc_ops = {
	.read_time = ds3232_read_time,
	.set_time = ds3232_set_time,
	.read_alarm = ds3232_read_alarm,
	.set_alarm = ds3232_set_alarm,
	.ioctl = ds3232_ioctl,
};

static int ds3232_probe(struct i2c_client *client,
		const struct i2c_device_id *id)
{
	struct ds3232 *ds3232;
	int ret;

	ds3232 = kzalloc(sizeof(struct ds3232), GFP_KERNEL);
	if (!ds3232)
		return -ENOMEM;

	ds3232->client = client;
	i2c_set_clientdata(client, ds3232);

	INIT_WORK(&ds3232->work, ds3232_work);
	mutex_init(&ds3232->mutex);

	ret = ds3232_check_rtc_status(client);
	if (ret)
		goto out_free;

	if (client->irq >= 0) {
		ret = request_irq(client->irq, ds3232_irq, 0,
				 "ds3232", client);
		if (ret)
			goto out_free;
	}

	ds3232->rtc = rtc_device_register(client->name, &client->dev,
					  &ds3232_rtc_ops, THIS_MODULE);
	if (IS_ERR(ds3232->rtc)) {
		ret = PTR_ERR(ds3232->rtc);
		dev_err(&client->dev, "unable to register the class device\n");
		goto out_irq;
	}

	return 0;

out_irq:
	if (client->irq >= 0)
		free_irq(client->irq, client);

out_free:
	i2c_set_clientdata(client, NULL);
	kfree(ds3232);
	return ret;
}

static int __devexit ds3232_remove(struct i2c_client *client)
{
	struct ds3232 *ds3232 = i2c_get_clientdata(client);

	if (client->irq >= 0) {
		mutex_lock(&ds3232->mutex);
		ds3232->exiting = 1;
		mutex_unlock(&ds3232->mutex);

		free_irq(client->irq, client);
		flush_scheduled_work();
	}

	rtc_device_unregister(ds3232->rtc);
	i2c_set_clientdata(client, NULL);
	kfree(ds3232);
	return 0;
}

static const struct i2c_device_id ds3232_id[] = {
	{ "ds3232", 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, ds3232_id);

static struct i2c_driver ds3232_driver = {
	.driver = {
		.name = "rtc-ds3232",
		.owner = THIS_MODULE,
	},
	.probe = ds3232_probe,
	.remove = __devexit_p(ds3232_remove),
	.id_table = ds3232_id,
};

static int __init ds3232_init(void)
{
	return i2c_add_driver(&ds3232_driver);
}

static void __exit ds3232_exit(void)
{
	i2c_del_driver(&ds3232_driver);
}

module_init(ds3232_init);
module_exit(ds3232_exit);

MODULE_AUTHOR("Srikanth Srinivasan <srikanth.srinivasan@freescale.com>");
MODULE_DESCRIPTION("Maxim/Dallas DS3232 RTC Driver");
MODULE_LICENSE("GPL");
