/*
 * Copyright (C) 2013, Broadcom Corporation. All Rights Reserved.
 * 
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */


#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/err.h>
#include <linux/clk.h>
#include <linux/io.h>
#include <linux/delay.h>

#include <linux/platform_device.h>
#include <linux/pwm.h>

#ifdef CONFIG_DEBUG_FS
#include <linux/debugfs.h>
#endif

#define IPROC_PWM_CHANNEL_CNT   4
#define PWM_PRESCALER_MAX    63 /* 6 bits field */


#define PWM_CTL_ENABLE_SHIFT        (0)
#define PWM_CTL_POLARITY_SHIFT      (8)

#define IPROC_PWM_CTRL_OFFSET			0x00
#define IPROC_PWM_PRESCALE_OFFSET		0x24
#define IPROC_PWM_PERIOD_OFFSET(ch)		(0x04 + ((ch) << 3))
#define IPROC_PWM_DUTY_CYCLE_OFFSET(ch)		(0x08 + ((ch) << 3))

#define IPROC_PWM_PRESCALE_BITS			0x06
#define IPROC_PWM_PRESCALE_SHIFT(ch)		((3 - (ch)) * \
						 IPROC_PWM_PRESCALE_BITS)
#define IPROC_PWM_PRESCALE_MASK(ch)		(IPROC_PWM_PRESCALE_MAX << \
						 IPROC_PWM_PRESCALE_SHIFT(ch))
#define IPROC_PWM_PRESCALE_MIN			0x00
#define IPROC_PWM_PRESCALE_MAX			0x3f

#define PWM_DUTY_HI_CNT0_SHIFT      (0)
#define PWM_DUTY_HI_CNT0_MASK       (0xFFFF)
#define PWM_DUTY_HI_CNT1_SHIFT      (0)
#define PWM_DUTY_HI_CNT1_MASK       (0xFFFF)
#define PWM_DUTY_HI_CNT2_SHIFT      (0)
#define PWM_DUTY_HI_CNT2_MASK       (0xFFFF)
#define PWM_DUTY_HI_CNT3_SHIFT      (0)
#define PWM_DUTY_HI_CNT3_MASK       (0xFFFF)

#define PWM_PERIOD_CNT0_SHIFT      (0)
#define PWM_PERIOD_CNT0_MASK       (0xFFFF)
#define PWM_PERIOD_CNT1_SHIFT      (0)
#define PWM_PERIOD_CNT1_MASK       (0xFFFF)
#define PWM_PERIOD_CNT2_SHIFT      (0)
#define PWM_PERIOD_CNT2_MASK       (0xFFFF)
#define PWM_PERIOD_CNT3_SHIFT      (0)
#define PWM_PERIOD_CNT3_MASK       (0xFFFF)


struct pwm_reg_def {
    u32 mask ;
    u32 shift ;
    u32 offset ;
} ;

#define PWM_REG_DEF(c, m, s, a) \
    [c] = { 	\
        .mask 		= 	m,  \
        .shift 		= 	s, \
        .offset 	= 	a \
    }

static const 
struct pwm_reg_def pwm_chan_pre_scaler_info[IPROC_PWM_CHANNEL_CNT] = { 
    PWM_REG_DEF(0, IPROC_PWM_PRESCALE_MASK(0), 
        IPROC_PWM_PRESCALE_SHIFT(0), IPROC_PWM_PRESCALE_OFFSET),
    PWM_REG_DEF(1, IPROC_PWM_PRESCALE_MASK(1), 
        IPROC_PWM_PRESCALE_SHIFT(1), IPROC_PWM_PRESCALE_OFFSET),
    PWM_REG_DEF(2, IPROC_PWM_PRESCALE_MASK(2), 
        IPROC_PWM_PRESCALE_SHIFT(2), IPROC_PWM_PRESCALE_OFFSET),
    PWM_REG_DEF(3, IPROC_PWM_PRESCALE_MASK(3), 
        IPROC_PWM_PRESCALE_SHIFT(3), IPROC_PWM_PRESCALE_OFFSET),
} ;

static const 
struct pwm_reg_def pwm_chan_period_cnt_info[IPROC_PWM_CHANNEL_CNT] = { 
    PWM_REG_DEF(0, PWM_PERIOD_CNT0_MASK, 
        PWM_PERIOD_CNT0_SHIFT, IPROC_PWM_PERIOD_OFFSET(0)),
    PWM_REG_DEF(1, PWM_PERIOD_CNT1_MASK, 
        PWM_PERIOD_CNT1_SHIFT, IPROC_PWM_PERIOD_OFFSET(1)),
    PWM_REG_DEF(2, PWM_PERIOD_CNT2_MASK, 
        PWM_PERIOD_CNT2_SHIFT, IPROC_PWM_PERIOD_OFFSET(2)),
    PWM_REG_DEF(3, PWM_PERIOD_CNT3_MASK, 
        PWM_PERIOD_CNT3_SHIFT, IPROC_PWM_PERIOD_OFFSET(3)),
} ;

static const 
struct pwm_reg_def pwm_chan_duty_cycle_info[IPROC_PWM_CHANNEL_CNT] = { 
    PWM_REG_DEF(0, PWM_DUTY_HI_CNT0_MASK, 
        PWM_DUTY_HI_CNT0_SHIFT, IPROC_PWM_DUTY_CYCLE_OFFSET(0)),
    PWM_REG_DEF(1, PWM_DUTY_HI_CNT1_MASK, 
        PWM_DUTY_HI_CNT1_SHIFT, IPROC_PWM_DUTY_CYCLE_OFFSET(1)),
    PWM_REG_DEF(2, PWM_DUTY_HI_CNT2_MASK, 
        PWM_DUTY_HI_CNT2_SHIFT, IPROC_PWM_DUTY_CYCLE_OFFSET(2)),
    PWM_REG_DEF(3, PWM_DUTY_HI_CNT3_MASK, 
        PWM_DUTY_HI_CNT3_SHIFT, IPROC_PWM_DUTY_CYCLE_OFFSET(3)),
} ;

#ifdef CONFIG_DEBUG_FS
struct iproc_pwm_config_debug_fs{
    struct dentry *period;
    struct dentry *duty;
    struct dentry *polarity;
    struct dentry *run;
};
#endif

struct iproc_pwm_config {
	struct device		*dev;
    struct pwm_device *pwm;
	int		pwm_id;
    int duty_ns;
    int period_ns;
	u32 duty_ticks;
	u32 period_ticks;    
	u8  polarity;	
	int running;	
#ifdef CONFIG_DEBUG_FS
    struct dentry *debugfs_entry;
    struct iproc_pwm_config_debug_fs config_entry;
#endif

};


struct iproc_pwmc {
    void __iomem *iobase;
	unsigned long	tick_hz;
    struct pwm_chip chip;
    struct iproc_pwm_config *config[IPROC_PWM_CHANNEL_CNT];
};


#ifdef CONFIG_DEBUG_FS
static int __init iproc_pwmc_debugfs_init(void);
void iproc_pwmc_debugfs_add_chan(struct iproc_pwmc *ap, int chan);
int iproc_pwmc_config_polarity(struct pwm_chip *chip,
                               struct pwm_device *p, int polarity);

static struct dentry *debugfs_base;

static int _debug_pwm_config_set(void *data, u64 val)
{
    struct iproc_pwm_config *config = data;

    if (val) {
        iproc_pwmc_config_polarity(config->pwm->chip, config->pwm, config->polarity);
        pwm_config(config->pwm, config->duty_ns, config->period_ns);
        pwm_enable(config->pwm);
        config->running = 1;
    } else {
        pwm_disable(config->pwm);
        config->running = 0;
    }
    return 0;
}
static int _debug_pwm_config_get(void *data, u64 *val)
{
    struct iproc_pwm_config *config = data;

    *val = config->running;
    return 0;
}

DEFINE_SIMPLE_ATTRIBUTE(iproc_pwm_config_fop, _debug_pwm_config_get,
			_debug_pwm_config_set, "%llu\n");

static int _debug_pwm_export_set(void *data, u64 val)
{

    struct iproc_pwm_config *config = data;
    struct pwm_device *pwm;
	char pwm_id[16];

    sprintf(pwm_id, "pwm-%d", config->pwm_id);

    if (val) {
        pwm = pwm_get(config->dev,pwm_id);
        config->pwm = pwm;
        config->config_entry.polarity = debugfs_create_u8("polarity", S_IRUGO | S_IWUSR, config->debugfs_entry,
            &config->polarity);
        config->config_entry.period = debugfs_create_u32("period_ns", S_IRUGO | S_IWUSR, config->debugfs_entry,
            &config->period_ns);
        config->config_entry.duty = debugfs_create_u32("duty_ns", S_IRUGO | S_IWUSR, config->debugfs_entry,
            &config->duty_ns);
        config->config_entry.run = debugfs_create_file("run", S_IRUGO | S_IWUSR, config->debugfs_entry,
            data, &iproc_pwm_config_fop);

    } else {
        debugfs_remove(config->config_entry.polarity);
        debugfs_remove(config->config_entry.period);
        debugfs_remove(config->config_entry.duty);
        debugfs_remove(config->config_entry.run);
        pwm_disable(config->pwm);
        pwm_put(config->pwm);
        config->pwm = NULL;
    }

    return 0;
}
static int _debug_pwm_export_get(void *data, u64 *val)
{
    struct iproc_pwm_config *config = data;

    if(config->pwm){
        *val = 1;
    } else {
        *val = 0;
    }
    return 0;
}

DEFINE_SIMPLE_ATTRIBUTE(iproc_pwm_export_fop, _debug_pwm_export_get,
			_debug_pwm_export_set, "%llu\n");

void iproc_pwmc_debugfs_add_chan(struct iproc_pwmc *ap, int chan)
{
	char fname[16];

    if (!debugfs_base)
        return;


    sprintf(fname, "iproc-pwm%d", chan);
    ap->config[chan]->debugfs_entry = debugfs_create_dir(fname, debugfs_base);


    debugfs_create_file("export", S_IRUGO | S_IWUSR, ap->config[chan]->debugfs_entry,
        ap->config[chan], &iproc_pwm_export_fop);

}   
static int __init iproc_pwmc_debugfs_init(void)
{

    debugfs_base = debugfs_create_dir("iproc", NULL);
    if (!debugfs_base)
        return -ENOMEM;

    return 0;
}
#endif


static int iproc_get_chan(const struct iproc_pwmc *ap, const struct pwm_device *p)
{
    int chan;

    chan = p->hwpwm;
    return chan;
}

static void iproc_pwmc_clear_set_bit(const struct iproc_pwmc *ap, unsigned int offset, 
    unsigned int shift, unsigned char en_dis)
{
    unsigned long val = readl(ap->iobase + offset ) ;

    // Clear bit.
    clear_bit(shift,&val) ;
    if ( en_dis == 1 ) 
        set_bit(shift,&val);

    writel(val, (ap->iobase + offset ));
}


static void iproc_pwmc_set_field(const struct iproc_pwmc *ap, unsigned int offset, 
    unsigned int mask, unsigned int shift, unsigned int wval)
{
    unsigned int val = readl(ap->iobase + offset ) ;

    val = (val & ~mask) | ( wval << shift ) ;
    writel(val, (ap->iobase + offset ));
}

static void iproc_pwmc_get_field(const struct iproc_pwmc *ap, unsigned int offset, 
    unsigned int mask, unsigned int shift, unsigned int *val)
{
    *val = readl(ap->iobase + offset ) ;
    *val = ( *val & mask ) >> shift ; 
}


#define to_iproc_chip(chip)       container_of(chip, struct iproc_pwmc, chip)
static int iproc_pwmc_start(struct pwm_chip *chip, struct pwm_device *pwm)
{
    struct iproc_pwmc *ap = to_iproc_chip(chip);
    int chan = iproc_get_chan(ap, pwm);

    iproc_pwmc_clear_set_bit(ap, IPROC_PWM_CTRL_OFFSET, 
        (PWM_CTL_ENABLE_SHIFT + chan), 1) ;

    return 0;
}

static void iproc_pwmc_stop(struct pwm_chip *chip, struct pwm_device *pwm)
{
    struct iproc_pwmc *ap = to_iproc_chip(chip);
    int chan = iproc_get_chan(ap, pwm);

    iproc_pwmc_clear_set_bit(ap, IPROC_PWM_CTRL_OFFSET, 
        (PWM_CTL_ENABLE_SHIFT + chan), 0) ;

}
static int iproc_pwmc_request(struct pwm_chip *chip, struct pwm_device *pwm)
{
    iproc_pwmc_stop(chip, pwm);
    return 0;
}

static void iproc_pwmc_config_duty_ticks(struct iproc_pwmc *ap, int chan,
					   unsigned long duty_ticks)
{
    /*struct pwm_device *p = ap->p[chan];*/
    unsigned int pre_scaler = 0 ;
    unsigned int duty_cnt = 0 ;

    iproc_pwmc_get_field(ap, pwm_chan_pre_scaler_info[chan].offset, 
        pwm_chan_pre_scaler_info[chan].mask, pwm_chan_pre_scaler_info[chan].shift, 
        &pre_scaler) ;

    /* Read prescaler value from register. */
    duty_cnt = duty_ticks / (pre_scaler + 1) ; 

    /* program duty cycle. */
    iproc_pwmc_set_field(ap, pwm_chan_duty_cycle_info[chan].offset, 
                    pwm_chan_duty_cycle_info[chan].mask, 
                    pwm_chan_duty_cycle_info[chan].shift, duty_cnt) ;

    if (BIT(chan) & (readl(ap->iobase + IPROC_PWM_CTRL_OFFSET) & 0xf)) {
        /* disable channel */
        iproc_pwmc_stop(&ap->chip, ap->config[chan]->pwm) ;
        udelay(1);
        /* enable channel. */
        iproc_pwmc_start(&ap->chip, ap->config[chan]->pwm) ;
    }

}

static int iproc_pwmc_config_period_ticks(struct iproc_pwmc *ap, int chan,
					    unsigned long period_ticks)
{
    unsigned int pcnt ;
    unsigned char pre_scaler = 0 ;
    /*struct pwm_device *p = ap->p[chan]; */
    
    pre_scaler = period_ticks / 0xFFFF ;
	if ( pre_scaler > PWM_PRESCALER_MAX ) 
        pre_scaler = PWM_PRESCALER_MAX ;

    pcnt = period_ticks / (pre_scaler + 1) ;

    /* programe prescaler */
    iproc_pwmc_set_field(ap, pwm_chan_pre_scaler_info[chan].offset, 
                        pwm_chan_pre_scaler_info[chan].mask, 
                        pwm_chan_pre_scaler_info[chan].shift, pre_scaler) ;

    /* program period count. */
    iproc_pwmc_set_field(ap, pwm_chan_period_cnt_info[chan].offset, 
                       pwm_chan_period_cnt_info[chan].mask, 
                       pwm_chan_period_cnt_info[chan].shift, pcnt) ;

    if (BIT(chan) & (readl(ap->iobase + IPROC_PWM_CTRL_OFFSET) & 0xf)) {
        /* disable channel */
        iproc_pwmc_stop(&ap->chip, ap->config[chan]->pwm) ;
        udelay(1);
        /* enable channel. */
        iproc_pwmc_start(&ap->chip, ap->config[chan]->pwm) ;
    }

    return 0;
}
static unsigned long pwm_ns_to_ticks(struct iproc_pwmc *ap, unsigned long nsecs)
{
	unsigned long long ticks;

	ticks = nsecs;
	ticks *= ap->tick_hz;	
    do_div(ticks,1000000000UL);

	return ticks;
}

int iproc_pwmc_config_polarity(struct pwm_chip *chip, 
        struct pwm_device *p, int polarity)
{

    struct iproc_pwmc *ap = to_iproc_chip(chip);
    int chan = iproc_get_chan(ap, p);

    if (polarity) {
        iproc_pwmc_clear_set_bit(ap, IPROC_PWM_CTRL_OFFSET, 
            (PWM_CTL_POLARITY_SHIFT + chan), 1) ;
    } else { 
        iproc_pwmc_clear_set_bit(ap, IPROC_PWM_CTRL_OFFSET, 
            (PWM_CTL_POLARITY_SHIFT + chan), 0) ;
    }

    if (BIT(chan) & (readl(ap->iobase + IPROC_PWM_CTRL_OFFSET) & 0xf)) {
        /* disable channel */
        iproc_pwmc_stop(&ap->chip, ap->config[chan]->pwm) ;
        udelay(1);
        /* enable channel. */
        iproc_pwmc_start(&ap->chip, ap->config[chan]->pwm) ;
    }
    ap->config[chan]->polarity = polarity;

    return 0;
}
EXPORT_SYMBOL_GPL(iproc_pwmc_config_polarity);


static int iproc_pwmc_config(struct pwm_chip *chip,
                struct pwm_device *p, int duty_ns, int period_ns)
{
    struct iproc_pwmc *ap = to_iproc_chip(chip);
    int chan = iproc_get_chan(ap, p);
    int ret;
    unsigned long period_ticks, duty_ticks;

    period_ticks = pwm_ns_to_ticks(ap, period_ns);
    ap->config[chan]->pwm = p;
    ret = iproc_pwmc_config_period_ticks(ap, chan, period_ticks);
    if (ret)
        return ret;
    ap->config[chan]->period_ticks = period_ticks;
    ap->config[chan]->period_ns = period_ns;

    duty_ticks = pwm_ns_to_ticks(ap, duty_ns);
    iproc_pwmc_config_duty_ticks(ap, chan, duty_ticks);    
    ap->config[chan]->duty_ticks = duty_ticks;
    ap->config[chan]->duty_ns = duty_ns;

    return 0;
}


static const struct pwm_ops iproc_pwm_ops = {
    .enable = iproc_pwmc_start,
    .disable = iproc_pwmc_stop,
    .request = iproc_pwmc_request,
    .free = iproc_pwmc_stop,
    .config = iproc_pwmc_config,
    .owner = THIS_MODULE,
};


static int iproc_pwmc_probe(struct platform_device *pdev)
{
    struct iproc_pwmc *ap;
    struct resource *r;
    int ret = 0;
    int chan;
    struct device_node *child;

    ap = devm_kzalloc(&pdev->dev, sizeof(*ap), GFP_KERNEL);
    if (!ap) {
        dev_err(&pdev->dev, "failed to allocate memory\n");
        return -ENOMEM;
    }
#ifdef CONFIG_MACH_NSP
    ap->tick_hz = 25000000UL;
#else
    ap->tick_hz = 1000000UL;
#endif
    ap->chip.ops = &iproc_pwm_ops;
    ap->chip.dev = &pdev->dev;
    ap->chip.base = -1;
    ap->chip.npwm = IPROC_PWM_CHANNEL_CNT;

    r = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    if (r == NULL) {
        dev_err(&pdev->dev, "no memory resource defined\n");
        return -ENODEV;
    }

    ap->iobase = devm_ioremap_resource(&pdev->dev, r);
    if (!ap->iobase) {
        return -EADDRNOTAVAIL;
    } 
    printk(KERN_INFO "iproc_pwmc_probe iobase %p phys:%x\n",
        ap->iobase,r->start);
    ret = pwmchip_add(&ap->chip);
    if (ret < 0)
            return ret;

    platform_set_drvdata(pdev, ap);


    for (chan = 0; chan < IPROC_PWM_CHANNEL_CNT; chan++) {
        ap->config[chan] = kzalloc(sizeof(struct iproc_pwm_config), GFP_KERNEL);
        ap->config[chan]->pwm_id = chan;
        ap->config[chan]->dev = &pdev->dev;
    }

    for_each_child_of_node(pdev->dev.of_node, child) {
        int polarity=0;
        of_property_read_u32(child, "channel", &chan);
        of_property_read_u32(child, "active_low", &polarity);
        ap->config[chan]->polarity = polarity;
        if (polarity) {
            iproc_pwmc_clear_set_bit(ap, IPROC_PWM_CTRL_OFFSET,
                    (PWM_CTL_POLARITY_SHIFT + chan), 1) ;
        }
    }


#ifdef CONFIG_DEBUG_FS
    iproc_pwmc_debugfs_init();
    for (chan = 0; chan < IPROC_PWM_CHANNEL_CNT; chan++) {
        iproc_pwmc_debugfs_add_chan(ap,chan);
    }
#endif

    return 0;

}

static int iproc_pwmc_remove(struct platform_device *pdev)
{
    struct iproc_pwmc *ap;
    int chan;

    ap = platform_get_drvdata(pdev);

#ifdef CONFIG_DEBUG_FS
    if (debugfs_base)
        debugfs_remove_recursive(debugfs_base);

#endif

    if (ap == NULL)
         return -ENODEV;

    for (chan = 0; chan < IPROC_PWM_CHANNEL_CNT; chan++){
        kfree(ap->config[chan]);
    }
    pwmchip_remove(&ap->chip);        

    devm_iounmap(&pdev->dev,ap->iobase);
    kfree(ap);

    return 0;
}

static const struct of_device_id of_iproc_pwm_match[] = {
	{ .compatible = "brcm,iproc-pwm" },
	{},
};

static struct platform_driver iproc_pwmc_driver = {
    .driver = {
        .name = "iproc_pwmc",
        .owner = THIS_MODULE,
        .of_match_table = of_iproc_pwm_match,
    },
    .probe = iproc_pwmc_probe,
    .remove = iproc_pwmc_remove,
};

static int __init iproc_pwmc_init(void)
{
        return platform_driver_register(&iproc_pwmc_driver);
}

static void __exit iproc_pwmc_exit(void)
{
    platform_driver_unregister(&iproc_pwmc_driver);
}
module_init(iproc_pwmc_init);
module_exit(iproc_pwmc_exit);

MODULE_AUTHOR("Broadcom Corporation");
MODULE_DESCRIPTION("Driver for iProc PWMC");
MODULE_LICENSE("GPL");
