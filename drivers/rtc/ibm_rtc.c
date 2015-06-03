/*
 * IBM RTC driver
 * Copyright (c) 2010 Applied Micro
 *
 * Author: Duy Nguyen <dpnguyen@appliedmicro.com>
 *
 * Base on ST M48T86 / Dallas DS12887 RTC driver
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This drivers only supports the clock running in BCD and 24H mode.
 * If it will be ever adapted to binary and 12H mode, care must be taken
 * to not introduce bugs.
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/of_platform.h>
#include <linux/rtc.h>
#include <linux/bcd.h>

#include <asm/dcr.h>
#include <asm/dcr-regs.h>

#define RTC_REG_SEC		0x00
#define RTC_REG_SECALRM		0x01
#define RTC_REG_MIN		0x02
#define RTC_REG_MINALRM		0x03
#define RTC_REG_HOUR		0x04
#define RTC_REG_HOURALRM	0x05
#define RTC_REG_DOW		0x06 /* 1 = sunday */
#define RTC_REG_DOM		0x07
#define RTC_REG_MONTH		0x08 /* 1 - 12 */
#define RTC_REG_YEAR		0x09 /* 0 - 99 */
#define RTC_REG_A		0x0A
#define RTC_REG_B		0x0B
#define RTC_REG_C		0x0C
#define RTC_REG_D		0x0D

#define RTC_REG_B_H24	(1 << 1)
#define RTC_REG_B_DM	(1 << 2)
#define RTC_REG_B_SET	(1 << 7)
#define RTC_REG_D_VRT	(1 << 7)

#define RTC_REG_PF		0x80	/* Period */
#define RTC_REG_AF		0x40	/* Alarm */
#define RTC_REG_UF		0x10	/* Update */

#define RTC_DCR_BASE_ADDR	0x300
#define RTC_READ(reg)		mfdcr(RTC_DCR_BASE_ADDR + reg)
#define RTC_WRITE(val, reg)	mtdcr(RTC_DCR_BASE_ADDR + reg, val)

/* Debug support */
#define DBG_LEVEL		0

#define RTC_DBG(dev, name, fmt, arg...) \
	printk(KERN_DEBUG #name ": " fmt, ## arg)

#if DBG_LEVEL > 0
#  define DBG(d,f,x...)		RTC_DBG(d, ibm-rtc, f, ##x)
#  define NL                    "\n"
#else
#  define DBG(f,x...)		((void)0)
#endif

#if DBG_LEVEL > 1
#  define DBG2(d,f,x...) 	DBG(d,f, ##x)
#else
#  define DBG2(f,x...) 		((void)0)
#endif

/*
 * FIXME: Temporatorily comment out read/setalarm() 
 *	because it make /dev/rtc work wrong.
 *	Enabling it later (if needed)
 */
#undef CONFIG_IBM_RTC_ALARM

struct ibm_rtc_instance {
	void 			__iomem *ioaddr;
	int			irq;
	struct rtc_device	*rtc;
	struct of_device	*ofdev;
	spinlock_t lock; /* serialize the NVRAM and RTC access */
};

static int ibm_rtc_read_time(struct device *dev, struct rtc_time *tm)
{
	unsigned char reg;
	//struct platform_device *pdev = to_platform_device(dev);

	DBG2(dev, "ibm_rtc_read_time" NL);

	reg = RTC_READ(RTC_REG_B);

	if (reg & RTC_REG_B_DM) {
		/* data (binary) mode */
		tm->tm_sec	= RTC_READ(RTC_REG_SEC);
		tm->tm_min	= RTC_READ(RTC_REG_MIN);
		tm->tm_hour	= RTC_READ(RTC_REG_HOUR) & 0x3F;
		tm->tm_mday	= RTC_READ(RTC_REG_DOM);
		/* tm_mon is 0-11 */
		tm->tm_mon	= RTC_READ(RTC_REG_MONTH) - 1;
		tm->tm_year	= RTC_READ(RTC_REG_YEAR) + 100;
		tm->tm_wday	= RTC_READ(RTC_REG_DOW);
	} else {
		/* bcd mode */
		tm->tm_sec	= bcd2bin(RTC_READ(RTC_REG_SEC));
		tm->tm_min	= bcd2bin(RTC_READ(RTC_REG_MIN));
		tm->tm_hour	= bcd2bin(RTC_READ(RTC_REG_HOUR) & 0x3F);
		tm->tm_mday	= bcd2bin(RTC_READ(RTC_REG_DOM));
		/* tm_mon is 0-11 */
		tm->tm_mon	= bcd2bin(RTC_READ(RTC_REG_MONTH)) - 1;
		tm->tm_year	= bcd2bin(RTC_READ(RTC_REG_YEAR)) + 100;
		tm->tm_wday	= bcd2bin(RTC_READ(RTC_REG_DOW));
	}

	/* correct the hour if the clock is in 12h mode */
	if (!(reg & RTC_REG_B_H24))
		if (RTC_READ(RTC_REG_HOUR) & 0x80)
			tm->tm_hour += 12;

	return 0;
}

static int ibm_rtc_set_time(struct device *dev, struct rtc_time *tm)
{
	unsigned char reg;
	//struct platform_device *pdev = to_platform_device(dev);

	DBG2(dev, "ibm_rtc_set_time" NL);

	reg = RTC_READ(RTC_REG_B);

	/* update flag and 24h mode */
	reg |= RTC_REG_B_SET | RTC_REG_B_H24;
	RTC_WRITE(reg, RTC_REG_B);

	if (reg & RTC_REG_B_DM) {
		/* data (binary) mode */
		RTC_WRITE(tm->tm_sec, RTC_REG_SEC);
		RTC_WRITE(tm->tm_min, RTC_REG_MIN);
		RTC_WRITE(tm->tm_hour, RTC_REG_HOUR);
		RTC_WRITE(tm->tm_mday, RTC_REG_DOM);
		RTC_WRITE(tm->tm_mon + 1, RTC_REG_MONTH);
		RTC_WRITE(tm->tm_year % 100, RTC_REG_YEAR);
		RTC_WRITE(tm->tm_wday, RTC_REG_DOW);
	} else {
		/* bcd mode */
		RTC_WRITE(bin2bcd(tm->tm_sec), RTC_REG_SEC);
		RTC_WRITE(bin2bcd(tm->tm_min), RTC_REG_MIN);
		RTC_WRITE(bin2bcd(tm->tm_hour), RTC_REG_HOUR);
		RTC_WRITE(bin2bcd(tm->tm_mday), RTC_REG_DOM);
		RTC_WRITE(bin2bcd(tm->tm_mon + 1), RTC_REG_MONTH);
		RTC_WRITE(bin2bcd(tm->tm_year % 100), RTC_REG_YEAR);
		RTC_WRITE(bin2bcd(tm->tm_wday), RTC_REG_DOW);
	}

	/* update ended */
	reg &= ~RTC_REG_B_SET;
	RTC_WRITE(reg, RTC_REG_B);
	
	return 0;
}

#if defined(CONFIG_IBM_RTC_ALARM)
/*
 * Read alarm time and date in RTC
 */
static int ibm_rtc_read_alarm(struct device *dev, struct rtc_wkalrm *alrm)
{
	unsigned char reg;
	//struct platform_device *pdev = to_platform_device(dev);
	struct rtc_time *tm = &alrm->time;

	DBG2(dev, "ibm_rtc_read_time" NL);

	reg = RTC_READ(RTC_REG_B);

	if (reg & RTC_REG_B_DM) {
		/* data (binary) mode */
		tm->tm_sec	= RTC_READ(RTC_REG_SECALRM);
		tm->tm_min	= RTC_READ(RTC_REG_MINALRM);
		tm->tm_hour	= RTC_READ(RTC_REG_HOURALRM) & 0x3F;
		tm->tm_mday	= RTC_READ(RTC_REG_DOM);
		/* tm_mon is 0-11 */
		tm->tm_mon	= RTC_READ(RTC_REG_MONTH) - 1;
		tm->tm_year	= RTC_READ(RTC_REG_YEAR) + 100;
		tm->tm_wday	= RTC_READ(RTC_REG_DOW);
	} else {
		/* bcd mode */
		tm->tm_sec	= bcd2bin(RTC_READ(RTC_REG_SECALRM));
		tm->tm_min	= bcd2bin(RTC_READ(RTC_REG_MINALRM));
		tm->tm_hour	= bcd2bin(RTC_READ(RTC_REG_HOURALRM) & 0x3F);
		tm->tm_mday	= bcd2bin(RTC_READ(RTC_REG_DOM));
		/* tm_mon is 0-11 */
		tm->tm_mon	= bcd2bin(RTC_READ(RTC_REG_MONTH)) - 1;
		tm->tm_year	= bcd2bin(RTC_READ(RTC_REG_YEAR)) + 100;
		tm->tm_wday	= bcd2bin(RTC_READ(RTC_REG_DOW));
	}

	/* correct the hour if the clock is in 12h mode */
	if (!(reg & RTC_REG_B_H24))
		if (RTC_READ(RTC_REG_HOUR) & 0x80)
			tm->tm_hour += 12;

	return 0;
}

/*
 * Set alarm in RTC
 */
static int ibm_rtc_set_alarm(struct device *dev, struct rtc_wkalrm *alrm)
{
	unsigned char reg;
	struct platform_device *pdev = to_platform_device(dev);
	struct ibm_rtc_instance *rtc = platform_get_drvdata(pdev);
	struct rtc_time *tm = &alrm->time;

	/* If no irq, we don't support ALARM */
	if (rtc->irq == NO_IRQ)
		return -EIO;

	reg = RTC_READ(RTC_REG_B);

	/* update flag and 24h mode */
	reg |= RTC_REG_B_SET | RTC_REG_B_H24;
	RTC_WRITE(reg, RTC_REG_B);

	if (reg & RTC_REG_B_DM) {
		/* data (binary) mode */
		RTC_WRITE(tm->tm_sec, RTC_REG_SECALRM);
		RTC_WRITE(tm->tm_min, RTC_REG_MINALRM);
		RTC_WRITE(tm->tm_hour, RTC_REG_HOURALRM);
		RTC_WRITE(tm->tm_mday, RTC_REG_DOM);
		RTC_WRITE(tm->tm_mon + 1, RTC_REG_MONTH);
		RTC_WRITE(tm->tm_year % 100, RTC_REG_YEAR);
		RTC_WRITE(tm->tm_wday, RTC_REG_DOW);
	} else {
		/* bcd mode */
		RTC_WRITE(bin2bcd(tm->tm_sec), RTC_REG_SECALRM);
		RTC_WRITE(bin2bcd(tm->tm_min), RTC_REG_MINALRM);
		RTC_WRITE(bin2bcd(tm->tm_hour), RTC_REG_HOURALRM);
		RTC_WRITE(bin2bcd(tm->tm_mday), RTC_REG_DOM);
		RTC_WRITE(bin2bcd(tm->tm_mon + 1), RTC_REG_MONTH);
		RTC_WRITE(bin2bcd(tm->tm_year % 100), RTC_REG_YEAR);
		RTC_WRITE(bin2bcd(tm->tm_wday), RTC_REG_DOW);
	}

	/* update ended */
	reg &= ~RTC_REG_B_SET;
	RTC_WRITE(reg, RTC_REG_B);

	return 0;
}
#endif //CONFIG_IBM_RTC_ALARM

/*
 * Handle commands from user-space
 */
static int ibm_rtc_ioctl(struct device *dev, unsigned int cmd,
			unsigned long arg)
{
	unsigned char r;
	struct platform_device *pdev = to_platform_device(dev);
	struct ibm_rtc_instance *rtc = platform_get_drvdata(pdev);
	unsigned long flags;
	int ret = 0;

	spin_lock_irqsave(&rtc->lock, flags);
	r = RTC_READ(RTC_REG_B);

	switch (cmd) {
	case RTC_AIE_OFF:	/* alarm interrupt off */
		r &= ~RTC_REG_AF;
		break;
	case RTC_AIE_ON:	/* alarm interrupt on */
		r |= RTC_REG_AF;
		break;
	case RTC_UIE_OFF:	/* update interrupt off */
		r &= ~RTC_REG_UF;
		break;
	case RTC_UIE_ON:	/* update interrupt on */
		r |= RTC_REG_UF;
		break;
	case RTC_PIE_OFF:	/* periodic interrupt off */
		r &= ~RTC_REG_PF;
		break;
	case RTC_PIE_ON:	/* periodic interrupt on */
		r |= RTC_REG_PF;
		break;
	default:
		ret = -ENOIOCTLCMD;
		break;
	}

	RTC_WRITE(r, RTC_REG_B);
	spin_unlock_irqrestore(&rtc->lock, flags);

	return ret;
}			

static int ibm_rtc_proc(struct device *dev, struct seq_file *seq)
{
	unsigned char reg;
	//struct platform_device *pdev = to_platform_device(dev);

	DBG(dev, "ibm_rtc_proc" NL);

	reg = RTC_READ(RTC_REG_B);

	seq_printf(seq, "mode\t\t: %s\n",
		 (reg & RTC_REG_B_DM) ? "binary" : "bcd");

	reg = RTC_READ(RTC_REG_D);

	seq_printf(seq, "battery\t\t: %s\n",
		 (reg & RTC_REG_D_VRT) ? "ok" : "exhausted");
	
	return 0;
}

/*
 * IRQ handler for the RTC
 */
static irqreturn_t ibm_rtc_interrupt(int irq, void *dev_id)
{
	unsigned char reg;
	struct platform_device *pdev = to_platform_device((struct device *)dev_id);
	struct ibm_rtc_instance *dev = platform_get_drvdata(pdev);
	unsigned char events = RTC_IRQF;

	/*
	 * read and clear interrupt
	 */
	spin_lock(&dev->lock);
	reg = RTC_READ(RTC_REG_C);
	spin_unlock(&dev->lock);

	DBG(dev, "RTC_REG_C = %04x" NL, reg);

	if (reg & RTC_REG_PF) 
		events|= RTC_PF;	
	if (reg & RTC_REG_AF) 
		events|= RTC_AF;
	if (reg & RTC_REG_UF) 
		events|= RTC_UF;

	rtc_update_irq(dev->rtc, 1, events);

	return IRQ_HANDLED;
}

static const struct rtc_class_ops ibm_rtc_ops = {
	.ioctl		= ibm_rtc_ioctl,
	.read_time	= ibm_rtc_read_time,
	.set_time	= ibm_rtc_set_time,
#if defined(CONFIG_IBM_RTC_ALARM)
	.read_alarm	= ibm_rtc_read_alarm,
	.set_alarm	= ibm_rtc_set_alarm,
#endif
	.proc		= ibm_rtc_proc,
};

static int ibm_rtc_probe(struct of_device *ofdev,
			const struct of_device_id *match)
{
	unsigned char reg;
	struct platform_device *pdev = &ofdev->dev;
	struct ibm_rtc_instance *dev = platform_get_drvdata(pdev);
	struct device_node *np = ofdev->node;
	struct rtc_device *rtc;
	int err;

	DBG(dev, "ibm_rtc_probe" NL);

	/* 
	 * Skip unused/unwired RTC.
	 */
	if (of_get_property(np, "unused", NULL) || !of_device_is_available(np))
		return -ENODEV;

	/* Allocate RTC device */
	dev = kmalloc(sizeof(struct ibm_rtc_instance), GFP_KERNEL);
	if (dev == NULL) {
		dev_err(&ofdev->dev, "kmalloc failed for dev\n");
		err = -ENOMEM;
		goto out;
	}
	memset(dev, 0, sizeof(struct ibm_rtc_instance));

	rtc = rtc_device_register("ibm_rtc",
			&ofdev->dev, &ibm_rtc_ops, THIS_MODULE);
	
	if (IS_ERR(rtc))
		return PTR_ERR(rtc);

	dev->rtc = rtc;
	dev->ofdev = ofdev;

	/* Try to get irq number. We also can work in
	 * the mode without IRQ.
	 */
	dev->irq = irq_of_parse_and_map(np, 0);
	if (dev->irq == NO_IRQ) {
		printk(KERN_ERR "%s: Can't map main interrupt\n", np->full_name);
		err = -ENODEV;
		goto out;
	}

	if (dev->irq != NO_IRQ) {
		err = request_irq(dev->irq, ibm_rtc_interrupt,
			IRQF_SHARED, "IBM RTC", &ofdev->dev);
		if (err)
			goto out;
	}

	dev_set_drvdata(&ofdev->dev, dev);

	/* read battery status */
	reg = RTC_READ(RTC_REG_D);
	dev_info(&ofdev->dev, "battery %s\n",
		(reg & RTC_REG_D_VRT) ? "ok" : "exhausted");

	return 0;
out:
	return err;
}

static int ibm_rtc_remove(struct of_device *ofdev)
{
	struct platform_device *pdev = &ofdev->dev;
	struct ibm_rtc_instance *dev = platform_get_drvdata(pdev);
	struct rtc_device *rtc = dev->rtc;

	DBG(dev, "ibm_rtc_remove" NL);

	dev_set_drvdata(&ofdev->dev, NULL);

 	if (rtc)
		rtc_device_unregister(rtc);

	return 0;
}

static const struct of_device_id ibm_rtc_match[] = {
	{ .compatible = "ibm,rtc", },
	{}
};

static struct of_platform_driver ibm_rtc_platform_driver = {
	.name = "ibm-rtc",
	.match_table = ibm_rtc_match,

	.probe = ibm_rtc_probe,
	.remove = ibm_rtc_remove,
};

static int __init ibm_rtc_init(void)
{
	return of_register_platform_driver(&ibm_rtc_platform_driver);
}

static void __exit ibm_rtc_exit(void)
{
	of_unregister_platform_driver(&ibm_rtc_platform_driver);
}

MODULE_AUTHOR("Duy Nguyen <dpnguyen@appliedmicro.com>");
MODULE_DESCRIPTION("IBM RTC driver");
MODULE_LICENSE("GPL");
MODULE_VERSION(DRV_VERSION);
MODULE_ALIAS("platform:ibm-rtc");

module_init(ibm_rtc_init);
module_exit(ibm_rtc_exit);
