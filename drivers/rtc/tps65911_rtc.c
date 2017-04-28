
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/i2c.h>
#include <linux/rtc.h>
#include <linux/bcd.h>
#include <linux/workqueue.h>
#include <linux/slab.h>
#include <linux/gpio.h>
#include <linux/mfd/tps65910.h>

struct tps65910_rtc{
	struct tps65910 *mfd;
	struct rtc_device *rtc;
};

static struct platform_driver tps65911_driver;

static int tps65911_read_time(struct device *dev, struct rtc_time *time)
{
	struct tps65910_rtc *rtc = dev_get_drvdata(dev);
	int ret;
	u8 buf[7];
	unsigned int year, month, day, hour, minute, second;
	unsigned int week, twelve_hr, am_pm;
	unsigned int century, add_century = 0;

	ret = rtc->mfd->read(rtc->mfd, TPS65910_SECONDS, 7, buf);

	if (ret < 0)
		return ret;

	second = buf[0];
	minute = buf[1];
	hour = buf[2];
	day = buf[3];
	month = buf[4];
	year = buf[5];
	week = buf[6];
	
	/* Extract additional information for AM/PM and century */

	twelve_hr = hour & 0x40;
	am_pm = hour & 0x20;
	century = 1;

	/* Write to rtc_time structure */

	time->tm_sec = bcd2bin(second);
	time->tm_min = bcd2bin(minute);
	if (twelve_hr) {
		/* Convert to 24 hr */
		if (am_pm)
			time->tm_hour = bcd2bin(hour & 0x1F) + 12;
		else
			time->tm_hour = bcd2bin(hour & 0x1F);
	} else {
		time->tm_hour = bcd2bin(hour);
	}

	time->tm_wday = bcd2bin(week);
	time->tm_mday = bcd2bin(day);
	//see rtc_tm_to_time
	time->tm_mon = (bcd2bin(month & 0x7F) - 1);
	if (century)
		add_century = 100;

	time->tm_year = bcd2bin(year) + add_century;
	
	return 0;
}

static int tps65911_set_time(struct device *dev, struct rtc_time *time)
{
	struct tps65910_rtc *rtc = dev_get_drvdata(dev);
	u8 buf[7];
	
	buf[0] = bin2bcd(time->tm_sec);
	buf[1] = bin2bcd(time->tm_min);
	buf[2] = bin2bcd(time->tm_hour);
	buf[6] = bin2bcd(time->tm_wday); /* Day of the week */
	buf[3] = bin2bcd(time->tm_mday); /* Date */
	//see rtc_time_to_tm
	buf[4] = bin2bcd(time->tm_mon) + 1;
	if (time->tm_year >= 100) {
		buf[4] |= 0x80;
		buf[5] = bin2bcd(time->tm_year - 100);
	} else {
		buf[5] = bin2bcd(time->tm_year);
	}
	
	return rtc->mfd->write(rtc->mfd, TPS65910_SECONDS, 7, buf);
}

static const struct rtc_class_ops tps65911_rtc_ops = {
	.read_time = tps65911_read_time,
	.set_time = tps65911_set_time,
};

static __devinit int tps65911_probe(struct platform_device *pdev)
{
	struct tps65910 *tps65910 = dev_get_drvdata(pdev->dev.parent);
	struct tps65910_rtc *tps65911;
	int ret;

	tps65911 = kzalloc(sizeof(struct tps65910_rtc), GFP_KERNEL);
	if (!tps65911)
		return -ENOMEM;

	tps65911->mfd = tps65910;
	platform_set_drvdata(pdev, tps65911);


	tps65911->rtc = rtc_device_register(pdev->name, &pdev->dev,
					  &tps65911_rtc_ops, THIS_MODULE);
	if (IS_ERR(tps65911->rtc)) {
		ret = PTR_ERR(tps65911->rtc);
		dev_err(&pdev->dev, "unable to register the class device\n");
		goto out_free;
	}

	return 0;

out_free:
	kfree(tps65911);
	return ret;
}

static int __devexit tps65911_remove(struct platform_device *pdev)
{
	struct tps65910_rtc *tps65911 = platform_get_drvdata(pdev);
	struct rtc_device *rtc = NULL;

	if(tps65911 != NULL){
		rtc = tps65911->rtc;
		if (rtc)
			rtc_device_unregister(rtc);
		kfree(tps65911);
	}

	return 0;
}

static struct platform_driver tps65911_driver = {
	.driver = {
		.name = "tps65911-rtc",
		.owner = THIS_MODULE,
	},
	.probe = tps65911_probe,
	.remove = __devexit_p(tps65911_remove),
};

static int __init tps65911_init(void)
{
	return platform_driver_register(&tps65911_driver);
}

static void __exit tps65911_exit(void)
{
	platform_driver_unregister(&tps65911_driver);
}

subsys_initcall(tps65911_init);
module_exit(tps65911_exit);

MODULE_AUTHOR("Appropho");
MODULE_DESCRIPTION("TPS65911 RTC Driver");
MODULE_LICENSE("GPL");
