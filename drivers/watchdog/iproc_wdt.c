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
#include <linux/moduleparam.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/notifier.h>
#include <linux/reboot.h>
#include <linux/platform_device.h>
#include <linux/watchdog.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/spinlock.h>
#include <linux/slab.h>

#ifdef CONFIG_IPROC_SW_RESET_RECORD
#include <plat/swreset_rec.h>
#endif

/* Chip specific */
#define IPROC_WATCHDOG_CLK_MSEC                       125000UL
#define IPROC_WATCHDOG_COUNTER_BITS                 (28)

/* Default values */
#define TIMEOUT_HW_MAX_MSEC                                \
    ((int)((1 << IPROC_WATCHDOG_COUNTER_BITS) - 1) / IPROC_WATCHDOG_CLK_MSEC)
#define TIMEOUT_DEFAULT     60

/* module parameters */
static int timeout =  TIMEOUT_DEFAULT;
module_param(timeout, int, 0);
MODULE_PARM_DESC(timeout,
        "Timeout value. (default="
        __MODULE_STRING(TIMEOUT_DEFAULT) ")");

static int nowayout = WATCHDOG_NOWAYOUT;
module_param(nowayout, int, 0);
MODULE_PARM_DESC(nowayout, "Watchdog cannot be stopped once started (default="
        __MODULE_STRING(WATCHDOG_NOWAYOUT) ")");

/* Watchdog register offsets and field */
#define IPROC_REG_WDT_INTSTATUS                     0x20
#define IPROC_FLD_WDT_INTSTATUS_WDT_RESET_MASK      0x80000000
#define IPROC_REG_WDT_WATCHDOG_COUNTER              0x80

/* Register access macro */
#define wdt_readl(dev, reg)                         \
    __raw_readl((dev)->regs + (reg))
#define wdt_writel(dev, reg, value)                 \
    __raw_writel((value), (dev)->regs + (reg))

/* Driver specific data */
struct wdt_iproc {
    volatile void               *regs;
    spinlock_t                  io_lock;
    int                         timeout;
    int				hw_timeout;
    int                         boot_status;
    unsigned long               users;
    struct miscdevice           miscdev;
    struct timer_list		timer;
    int				credits;
    int				ticks;
#ifdef CONFIG_IPROC_SW_RESET_RECORD
    SWRR_HANDLE                 swrr;
#endif
};
static struct wdt_iproc *wdt;
static char expect_release;

/*
 * Disable the watchdog.
 */
static inline void 
iproc_wdt_stop(void)
{
    spin_lock_bh(&wdt->io_lock);
#ifdef CONFIG_IPROC_SW_RESET_RECORD
    if (wdt->swrr != NULL)
        swreset_record_set(wdt->swrr, 0);
#endif
    wdt_writel(wdt, 
        IPROC_REG_WDT_WATCHDOG_COUNTER,
        0
        );
    spin_unlock_bh(&wdt->io_lock);
}

/*
 * Enable and reset the watchdog.
 */
static inline void 
iproc_wdt_start(void)
{
    spin_lock_bh(&wdt->io_lock);
#ifdef CONFIG_IPROC_SW_RESET_RECORD
    if (wdt->swrr != NULL)
        swreset_record_set(wdt->swrr, 1);
#endif
    wdt_writel(wdt, IPROC_REG_WDT_WATCHDOG_COUNTER,
	       wdt->hw_timeout * IPROC_WATCHDOG_CLK_MSEC);

    spin_unlock_bh(&wdt->io_lock);
}

static void wdt_timer_callback(unsigned long data)
{
    int ret;

    if (wdt->credits == wdt->ticks / 2) {
	printk(KERN_WARNING "%s: watchdog is half way to firing\n", __func__);
    }

    if (wdt->credits-- <= 0) {
	printk(KERN_ERR "%s: out of credits, we will die soon\n", __func__);
	return;			/* We are going to die soon */
    }
    iproc_wdt_start();		/* Restart WDT */

    ret = mod_timer(&wdt->timer, jiffies +
		    msecs_to_jiffies(wdt->hw_timeout / 2));
    if (ret)
	printk(KERN_ERR "%s: unable to set timer, ret = %d\n", __func__, ret);
}


/*
 * Pat the watchdog timer.
 *
 * Since we run the hardware watchdog at half the frequency of the software
 * timer, assign two credits per tick.
 */
static inline void 
iproc_wdt_pat(void)
{
    wdt->credits = wdt->ticks * 2;
}

/*
 * Watchdog device is opened, and watchdog starts running.
 */
static int 
iproc_wdt_open(struct inode *inode, struct file *file)
{
    if (test_and_set_bit(1, &wdt->users))
        return -EBUSY;

    iproc_wdt_pat();
    wdt_timer_callback(0);	/* Start the WDT and timer */
    return nonseekable_open(inode, file);
}

/*
 * Close the watchdog device.
 */
static int 
iproc_wdt_close(struct inode *inode, struct file *file)
{
    if (expect_release == 1) {
        iproc_wdt_stop();
    } else {
        dev_dbg(wdt->miscdev.parent,
            "unexpected close, not stopping watchdog!\n");
        iproc_wdt_pat();
    }
    clear_bit(1, &wdt->users);
    expect_release = 0;
    return 0;
}

/*
 * Change the watchdog time interval.
 */
static int 
iproc_wdt_settimeout(int time)
{
    uint32_t time_ms;

    if (time < 0 || time >= (UINT_MAX / 1000)) {
        return -EINVAL;
    }

    /*
     * Calculate the values of the hardware and software watchdog.
     * We pet the hardware watchdog from the software watchdog, and
     * run the software watchdog at twice the frequency of the
     * hardware watchdog. This means that we will be able to report
     * when we are half way through the timeout.
     */

    time_ms = time * 1000;
    if (time_ms < (TIMEOUT_HW_MAX_MSEC / 2)) {
	wdt->hw_timeout = time_ms * 2;
	wdt->ticks = 1;
    } else {
	wdt->hw_timeout = TIMEOUT_HW_MAX_MSEC;
	wdt->ticks = (time_ms / TIMEOUT_HW_MAX_MSEC) + 1;
    }
    wdt->timeout = time;	/* Remember the user specified value */

    return 0;
}

/*
 * Get the watchdog status.
 */
static int 
iproc_wdt_get_status(void)
{
    if (wdt_readl(wdt, IPROC_REG_WDT_INTSTATUS) & 
        IPROC_FLD_WDT_INTSTATUS_WDT_RESET_MASK) {
        return WDIOF_CARDRESET;
    }

    return 0;
}

static const struct watchdog_info iproc_wdt_info = {
    .identity    = "iproc watchdog",
    .options    = WDIOF_SETTIMEOUT | WDIOF_KEEPALIVEPING | WDIOF_MAGICCLOSE,
};

/*
 * Handle commands from user-space.
 */
static long 
iproc_wdt_ioctl(struct file *file,
                unsigned int cmd, unsigned long arg)
{
    int ret = -ENOTTY;
    int time;
    void __user *argp = (void __user *)arg;
    int __user *p = argp;

    switch (cmd) {
    case WDIOC_GETSUPPORT:
        ret = copy_to_user(argp, &iproc_wdt_info,
                sizeof(iproc_wdt_info)) ? -EFAULT : 0;
        break;
    case WDIOC_GETSTATUS:
        ret = put_user(0, p);
        break;
    case WDIOC_GETBOOTSTATUS:
        ret = put_user(wdt->boot_status, p);
        break;
    case WDIOC_SETOPTIONS:
        ret = get_user(time, p);
        if (ret)
            break;
        if (time & WDIOS_DISABLECARD)
            iproc_wdt_stop();
        if (time & WDIOS_ENABLECARD)
            iproc_wdt_pat();
        ret = 0;
        break;
    case WDIOC_KEEPALIVE:
        iproc_wdt_pat();
        ret = 0;
        break;
    case WDIOC_SETTIMEOUT:
        ret = get_user(time, p);
        if (ret)
            break;
        ret = iproc_wdt_settimeout(time);
        if (ret)
            break;
        /* Enable new time value */
        iproc_wdt_pat();
        /* fall through */
    case WDIOC_GETTIMEOUT:
        ret = put_user(wdt->timeout, p);
        break;
    }

    return ret;
}

static ssize_t 
iproc_wdt_write(struct file *file, const char __user *data, 
                size_t len, loff_t *ppos)
{
    /* See if we got the magic character 'V' and reload the timer */
    if (len) {
        if (!nowayout) {
            size_t i;

            /*
             * note: just in case someone wrote the magic
             * character five months ago...
             */
            expect_release = 0;

            /*
             * scan to see whether or not we got the magic
             * character
             */
            for (i = 0; i != len; i++) {
                char c;
                if (get_user(c, data + i))
                    return -EFAULT;
                if (c == 'V')
                    expect_release = 1;
            }
        }
        /* someone wrote to us, we should pat the watchdog */
        iproc_wdt_pat();
    }
    return len;
}

static int 
iproc_wdt_notify_sys(struct notifier_block *this, unsigned long code, void *unused)
{
    if (code == SYS_DOWN || code == SYS_HALT) {
        /* Turn the watchdog off */
#ifdef CONFIG_IPROC_SW_RESET_RECORD
        if (wdt->swrr != NULL) {
            swreset_record_set(wdt->swrr, 0);
        }
#endif
        iproc_wdt_stop();
    }
    return NOTIFY_DONE;
}

static struct notifier_block iproc_wdt_notifier = {
    .notifier_call	= iproc_wdt_notify_sys,
};

static const struct file_operations iproc_wdt_fops = {
    .owner              = THIS_MODULE,
    .llseek             = no_llseek,
    .unlocked_ioctl     = iproc_wdt_ioctl,
    .open               = iproc_wdt_open,
    .release            = iproc_wdt_close,
    .write              = iproc_wdt_write,
};

static int __init 
iproc_wdt_probe(struct platform_device *pdev)
{
    struct resource    *regs;
    int ret;

    if (wdt) {
        dev_dbg(&pdev->dev, "only 1 wdt instance supported.\n");
        return -EBUSY;
    }

    regs = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    if (!regs) {
        dev_dbg(&pdev->dev, "missing resource 0\n");
        return -ENXIO;
    }

    wdt = kzalloc(sizeof(struct wdt_iproc), GFP_KERNEL);
    if (!wdt) {
        dev_dbg(&pdev->dev, "no memory for wdt structure\n");
        return -ENOMEM;
    }

    wdt->regs = ioremap(regs->start, resource_size(regs));
    if (!wdt->regs) {
        ret = -ENOMEM;
        dev_dbg(&pdev->dev, "could not map I/O memory\n");
        goto err_free;
    }
    
#ifdef CONFIG_IPROC_SW_RESET_RECORD
    if (swreset_record_get_record_count() == 0) {
        /* No software reset record support */
        wdt->swrr = NULL;
        wdt->boot_status = iproc_wdt_get_status();
    } else {
        wdt->swrr = swreset_record_register("iproc_cca_watchdog");
        if (wdt->swrr == NULL) {
            /* All software reset records are used (unlikely) */
            dev_warn(&pdev->dev, "Boot status is not available!\n");
            /* Still try to use original watchdog reset status */
            wdt->boot_status = iproc_wdt_get_status();
        } else {
            /* Get previous value */
            swreset_record_get(wdt->swrr, &wdt->boot_status);
            /* Clear it */
            swreset_record_set(wdt->swrr, 0);
        }
    }
#else
    wdt->boot_status = iproc_wdt_get_status();
#endif

    /* To avoid treating manual reboot as a watchdog reset */
    ret = register_reboot_notifier(&iproc_wdt_notifier);
    if (ret) {
        /* In this case, manual reboot could be mistaken as watchdog reset */
        dev_dbg(&pdev->dev, "could not register reboot notifier\n");
    }

    expect_release = 0;
    spin_lock_init(&wdt->io_lock);
    wdt->users = 0;

    wdt->miscdev.minor    = WATCHDOG_MINOR;
    wdt->miscdev.name    = "watchdog";
    wdt->miscdev.fops    = &iproc_wdt_fops;
    wdt->miscdev.parent    = &pdev->dev;

    platform_set_drvdata(pdev, wdt);

    setup_timer(&wdt->timer, wdt_timer_callback, (unsigned long)wdt);

    if (iproc_wdt_settimeout(timeout)) {
        iproc_wdt_settimeout(TIMEOUT_DEFAULT);
        dev_dbg(&pdev->dev,
            "default timeout invalid, set to %d sec.\n",
            TIMEOUT_DEFAULT);
    }

    ret = misc_register(&wdt->miscdev);
    if (ret) {
        dev_dbg(&pdev->dev, "failed to register wdt miscdev\n");
        goto err_register;
    }

    dev_info(&pdev->dev,
        "iProc Watchdog Timer - timeout=%d sec, nowayout=%d\n",
        wdt->timeout, nowayout);

    return 0;

err_register:
    platform_set_drvdata(pdev, NULL);
    iounmap(wdt->regs);
err_free:
    kfree(wdt);
    wdt = NULL;
    return ret;
}

static int __exit 
iproc_wdt_remove(struct platform_device *pdev)
{
    if (wdt && platform_get_drvdata(pdev) == wdt) {
        /* Stop the timer before we leave */
        if (!nowayout)
            iproc_wdt_stop();

#ifdef CONFIG_IPROC_SW_RESET_RECORD
        if (wdt->swrr != NULL) {
            swreset_record_set(wdt->swrr, 0);
            swreset_record_unregister(wdt->swrr);
            wdt->swrr = NULL;
        }
#endif
	del_timer_sync(&wdt->timer);
        unregister_reboot_notifier(&iproc_wdt_notifier);
        misc_deregister(&wdt->miscdev);
        iounmap(wdt->regs);
        kfree(wdt);
        wdt = NULL;
        platform_set_drvdata(pdev, NULL);
    }
    return 0;
}

static void 
iproc_wdt_shutdown(struct platform_device *pdev)
{
    iproc_wdt_stop();
}

#ifdef CONFIG_PM
static int 
iproc_wdt_suspend(struct platform_device *pdev, pm_message_t message)
{
    iproc_wdt_stop();
    return 0;
}

static int 
iproc_wdt_resume(struct platform_device *pdev)
{
    if (wdt->users)
        iproc_wdt_pat();
    return 0;
}
#else
#define iproc_wdt_suspend NULL
#define iproc_wdt_resume NULL
#endif

static const struct of_device_id of_iproc_wdt_match[] = {
   { .compatible = "iproc-wdt" },
   {},
};
MODULE_DEVICE_TABLE(of, of_iproc_wdt_match);

/* work with hotplug and coldplug */
MODULE_ALIAS("platform:iproc_wdt");

static struct platform_driver iproc_wdt_driver = {
    .remove             = __exit_p(iproc_wdt_remove),
    .suspend            = iproc_wdt_suspend,
    .resume             = iproc_wdt_resume,
    .driver             = {
        .name           = "iproc_wdt",
        .owner          = THIS_MODULE,
        .of_match_table = of_iproc_wdt_match,
    },
    .shutdown           = iproc_wdt_shutdown,
};

static int __init 
iproc_wdt_init(void)
{
    return platform_driver_probe(&iproc_wdt_driver, iproc_wdt_probe);
}
module_init(iproc_wdt_init);

static void __exit 
iproc_wdt_exit(void)
{
    platform_driver_unregister(&iproc_wdt_driver);
}
module_exit(iproc_wdt_exit);

MODULE_AUTHOR("Broadcom Corporation");
MODULE_DESCRIPTION("Watchdog driver for Broadcom iProc chips");
MODULE_LICENSE("GPL");
MODULE_ALIAS_MISCDEV(WATCHDOG_MINOR);
