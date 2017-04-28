/*
 * APM APM86xxx RTC 
 *
 * Copyright (c) 2010 Applied Micro Circuits Corporation.
 * All rights reserved. Inderpal Saluja <isaluja@apm.com>.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 *
 * This driver adds support for APM86xxx RTC.
 */
#include <linux/bcd.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/rtc.h>
#include <linux/of_platform.h>
#include <linux/io.h>
#ifdef CONFIG_APM86xxx
#include <asm/ipp.h>
#endif

#define DRV_NAME	"APM86xxx-RTC"
#define DRV_VERSION	"0.1"

#define RTC_CCVR	0x00
#define RTC_CMR		0x04
#define RTC_CLR		0x08
#define RTC_CCR		0x0C
#define RTC_STAT	0x10
#define RTC_RSTAT	0x14
#define RTC_EOI		0x18
#define RTC_VER		0x1C

#define RTC_CCR_WEN	0x08
#define RTC_CCR_EN	0x04
#define RTC_CCR_MASK	0x02
#define RTC_CCR_IE	0x01
#define RTC_CCR_DIS	0x00

#define RTC_STAT_BIT	0x01
#define RTC_RESET_MASK	0x10

struct apm_rtc_pdata {
	struct rtc_device *rtc;
	struct device	*dev;
	void __iomem 	*baseaddr;		
	int	irq;
	unsigned long	alarm_time;
	spinlock_t	lock;
};

static int apm_rtc_set_mmss(struct device *dev, unsigned long secs)
{
	struct apm_rtc_pdata *pdata = dev_get_drvdata(dev);
	u32 cnt = secs;
	
	out_le32(pdata->baseaddr + RTC_CLR, cnt);
	return 0;	
}

static int apm_rtc_read_time(struct device *dev, struct rtc_time *tm)
{
	unsigned long time;
	struct apm_rtc_pdata *pdata = dev_get_drvdata(dev);
	
	time = in_le32(pdata->baseaddr + RTC_CCVR);
	
	rtc_time_to_tm(time, tm);
	
	pr_debug(DRV_NAME "tm is secs=%d, mins=%d, hours=%d, "
		"mday=%d, mon=%d, year=%d, wday=%d\n",
		tm->tm_sec, tm->tm_min, tm->tm_hour,tm->tm_mday,
		tm->tm_mon, tm->tm_year, tm->tm_wday);

	return 0;
}

static int apm_rtc_set_alarm(struct device *dev, struct rtc_wkalrm *alrm)
{
	struct apm_rtc_pdata *pdata = dev_get_drvdata(dev);
	unsigned long rtc_time;
	unsigned long alarm_time;
	u32 ccr;
	int ret;

	if (pdata->irq <= 0)
		return -EINVAL;
	
	rtc_time = in_le32(pdata->baseaddr + RTC_CCVR);
		
	ret = rtc_tm_to_time(&alrm->time, &alarm_time);
	if (ret)
		return ret;

	if (alarm_time < rtc_time)
		return -EINVAL;
		
	spin_lock_irq(&pdata->lock);
	pdata->alarm_time = alarm_time;

	out_le32(pdata->baseaddr + RTC_CMR, pdata->alarm_time);			
	
	ccr = in_le32(pdata->baseaddr + RTC_CCR);
	
	if (alrm->enabled) {
		ccr &= ~RTC_CCR_MASK;
		ccr |= RTC_CCR_IE;
	} else {
		ccr &= ~RTC_CCR_IE;
		ccr |= RTC_CCR_MASK;
	}
	out_le32(pdata->baseaddr + RTC_CCR,ccr);
	spin_unlock_irq(&pdata->lock);

	return 0;
}

static int apm_rtc_read_alarm(struct device *dev, struct rtc_wkalrm *alrm)
{
	struct apm_rtc_pdata *pdata = dev_get_drvdata(dev);
	
	spin_lock_irq(&pdata->lock);
	rtc_time_to_tm(pdata->alarm_time, &alrm->time);
	alrm->enabled = in_le32(pdata->baseaddr + RTC_CCR) & RTC_CCR_IE;
	spin_unlock_irq(&pdata->lock);
	
	return 0;
}

static irqreturn_t apm_interrupt(int irq, void *dev_id)
{
	struct apm_rtc_pdata *pdata  = dev_id ;
	unsigned long events = RTC_IRQF;
	
	spin_lock(&pdata->lock);

	/* read interrupt*/
	if (!(in_le32(pdata->baseaddr + RTC_STAT) & RTC_STAT_BIT)) {
	 	spin_unlock(&pdata->lock);
		return IRQ_NONE;
	}
	
	/* clear interrupt  */
	in_le32(pdata->baseaddr + RTC_EOI);
	
	events |= RTC_AF;
	rtc_update_irq(pdata->rtc, 1, events);
	spin_unlock(&pdata->lock);
	 
	return IRQ_HANDLED;
}

static int apm_rtc_proc(struct device *dev, struct seq_file *seq)
{
	struct apm_rtc_pdata *pdata = dev_get_drvdata(dev);

	seq_printf(seq, "ccr\t\t: %x\n",
			in_le32(pdata->baseaddr + RTC_CCR));
	seq_printf(seq, "cmr\t\t: %x\n",
			in_le32(pdata->baseaddr + RTC_CMR));
	seq_printf(seq, "clr\t\t: %x\n",
			in_le32(pdata->baseaddr + RTC_CLR));
	seq_printf(seq, "ccvr\t\t: %x\n",
			in_le32(pdata->baseaddr + RTC_CCVR));
	seq_printf(seq, "version\t\t: %x\n",
			in_le32(pdata->baseaddr + RTC_VER));

	return 0;
}

static const struct rtc_class_ops apm_rtc_ops = {
	.read_time	= apm_rtc_read_time,
	.set_mmss 	= apm_rtc_set_mmss,
	.read_alarm	= apm_rtc_read_alarm,
	.set_alarm	= apm_rtc_set_alarm,	
	.proc		= apm_rtc_proc,
};

static int __devinit apm_rtc_probe(struct of_device *op,
				   const struct of_device_id *match)
{
	struct rtc_device *rtc;
	struct apm_rtc_pdata *pdata;
	int ret;

	pdata = kzalloc(sizeof(*pdata), GFP_KERNEL);
	if (!pdata)
		return -ENOMEM;
	
	pdata->dev = &op->dev;
	pdata->baseaddr = of_iomap(op->node, 0);
	if (!pdata->baseaddr) {
		dev_err(pdata->dev, "failed to map controller\n");
		ret = -ENOMEM;
		goto fail_map;
	}
	pdata->irq = irq_of_parse_and_map(op->node, 0);
	if (pdata->irq > 0 ){ 
		ret = request_irq(pdata->irq, apm_interrupt,
				     0, DRV_NAME, pdata);
		if (ret < 0) {
			dev_err(pdata->dev, "failed to attach interrupt\n");
			goto fail_request;
		}
	}
	/*
	 * turn on the clock and the crystal, etc.
 	 */
	out_le32(pdata->baseaddr + RTC_CCR,RTC_CCR_DIS);
	out_le32(pdata->baseaddr + RTC_CCR,RTC_CCR_EN);
	
	device_init_wakeup(&op->dev, 1);
	
	rtc = rtc_device_register(DRV_NAME, &op->dev, &apm_rtc_ops,
				THIS_MODULE);
	if (IS_ERR(rtc)) {
		ret = PTR_ERR(rtc);
		goto out;
	}
	pdata->rtc = rtc;
	dev_set_drvdata(&op->dev, pdata);
	
	spin_lock_init(&pdata->lock);
	
	return 0;
	
out:	
	if (pdata->rtc)
		rtc_device_unregister(pdata->rtc);
	
	dev_set_drvdata(&op->dev, NULL);
	free_irq(pdata->irq, pdata);
	
fail_request:
	irq_dispose_mapping(pdata->irq);
	iounmap(pdata->baseaddr);
	
fail_map:
	kfree(pdata);
	
	return ret;
	
}

static int __devexit apm_rtc_remove(struct of_device *op)
{
	struct apm_rtc_pdata *pdata = dev_get_drvdata(&op->dev);

	rtc_device_unregister(pdata->rtc);
	pdata->rtc = NULL;
	dev_set_drvdata(&op->dev, NULL);
	
	if (pdata->irq > 0)
		free_irq(pdata->irq, pdata);
	
	irq_dispose_mapping(pdata->irq);
	iounmap(pdata->baseaddr);

	return 0;
}

static const struct of_device_id apm_i2c_of_match[] = {
	{.compatible = "apm-apm86xxx-rtc", } ,
	{}
};
/* Structure for a device driver */
static struct of_platform_driver apm_rtc_driver = {
	.match_table	= apm_i2c_of_match,
	.probe		= apm_rtc_probe,
	.remove		= __devexit_p(apm_rtc_remove),
	.driver		= {
		.owner	= THIS_MODULE,
		.name	= DRV_NAME,
	},
};

static int __init apm_rtc_init(void)
{
	return of_register_platform_driver(&apm_rtc_driver);
}

static void __exit apm_rtc_exit(void)
{
	of_unregister_platform_driver(&apm_rtc_driver);
}

module_init(apm_rtc_init);
module_exit(apm_rtc_exit);

MODULE_AUTHOR("Inderpal Saluja <isaluja@apm.com>");
MODULE_DESCRIPTION("APM RTC driver with Designware core");
MODULE_LICENSE("GPL");
MODULE_VERSION(DRV_VERSION);
