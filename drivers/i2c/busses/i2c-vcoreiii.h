/*
 *
 * VCore-III I2C platform data structure
 *
 * Copyright (C) 2011 Vitesse Semiconductor Inc.
 * Author: Lars Povlsen (lpovlsen@vitesse.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
 * VITESSE SEMICONDUCTOR INC SHALL HAVE NO LIABILITY WHATSOEVER OF ANY
 * KIND ARISING OUT OF OR RELATED TO THE PROGRAM OR THE OPEN SOURCE
 * MATERIALS UNDER ANY THEORY OF LIABILITY.
 *
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/err.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/delay.h>
#include <linux/spinlock.h>

#include <asm/mach-vcoreiii/hardware.h>
#include <asm/mach-vcoreiii/i2c.h>
#include <asm/mach-vcoreiii/vcoreiii_gpio.h>

#define DEBUG_I2C(x...) do { if(debug) printk(KERN_DEBUG x); } while(0)
#define DEBUG_I2C_L(l, x...) do { if(debug >= l) printk(KERN_DEBUG x); } while(0)

#define MOD_NAME     "i2c_vcoreiii"

#define VCOREIII_XFER_TIMEOUT       (HZ/2) // 0.5sec
#define VCOREIII_TX_FIFO_FULL_LEVEL 8
#define VCOREIII_TX_FIFO_THRESHOLD  6

struct vcoreiii_twi_iface {
    struct device      *dev;
    int                irq;
    u8                 *buf;
    size_t             buf_len;
    struct completion  cmd_complete;
    int                cmd_err;
    spinlock_t         lock;
    struct i2c_adapter adap;
};

static int debug;

enum {
    I2C_CMPLT_ABORT = (1 << 0),
};

static void i2c_reset(struct i2c_adapter *adap)
{
    writel(0x0, VTSS_TWI_TWI_CTRL); /* Leave it disabled */
}

static void stuff_tx(struct vcoreiii_twi_iface *dev)
{
    unsigned long flags;
    int lev;
    spin_lock_irqsave(&dev->lock, flags);
    while(dev->buf_len && (lev = readl(VTSS_TWI_TWI_TXFLR)) < VCOREIII_TX_FIFO_FULL_LEVEL) {
        DEBUG_I2C_L(2,"PUT 0x%x, %d bytes left, level %d\n", *(dev->buf), dev->buf_len, lev);
        writel(*(dev->buf), VTSS_TWI_TWI_DATA_CMD);
        dev->buf++;
        dev->buf_len--;
    }
    if(dev->buf_len == 0)
        writel(0, VTSS_TWI_TWI_TX_TL); /* when empty, call me */
    spin_unlock_irqrestore(&dev->lock, flags);
}

static int wait_for_tx_buffer(int level, int timeout)
{
    unsigned long status;

    while((status = readl(VTSS_TWI_TWI_TXFLR) > level) && timeout > 0) {
        DEBUG_I2C("TX wait level %ld - want %d\n", status, level);
        msleep(1);
        timeout--;
    }

    return status <= level;
}

static int do_xfer(struct i2c_adapter *adap, struct i2c_msg *msg)
{
    struct vcoreiii_twi_iface *dev = i2c_get_adapdata(adap);
    int r;

    if (msg->len == 0)
        return -EINVAL;

    /* Safety - wait upto 5 msecs for TX buffer to drain */
    wait_for_tx_buffer(0, 5);

    // disable controller to write TAR
    writel(0x0, VTSS_TWI_TWI_CTRL);

    // set target address
    writel(msg->addr, VTSS_TWI_TWI_TAR);

    // enable controller
    writel(VTSS_F_TWI_TWI_CTRL_ENABLE, VTSS_TWI_TWI_CTRL);

    dev->buf = msg->buf;
    dev->buf_len = msg->len;
    dev->cmd_err = 0;
    init_completion(&dev->cmd_complete);

    if (msg->flags & I2C_M_RD) {
        // read command - data = 0x00 (don't care)
        writel(VTSS_F_TWI_TWI_DATA_CMD_CMD, VTSS_TWI_TWI_DATA_CMD);
        writel(VTSS_F_TWI_TWI_INTR_MASK_M_RX_FULL, VTSS_TWI_TWI_INTR_MASK); // enable rx fifo interrupt
    } else {
        // write command - stuff data into fifo
        writel(VCOREIII_TX_FIFO_THRESHOLD, VTSS_TWI_TWI_TX_TL); /* less than 3/4 full, call me */
        stuff_tx(dev);
        writel(VTSS_F_TWI_TWI_INTR_MASK_M_TX_ABRT|
               VTSS_F_TWI_TWI_INTR_STAT_TX_OVER|
               VTSS_F_TWI_TWI_INTR_MASK_M_TX_EMPTY,
	       VTSS_TWI_TWI_INTR_MASK); // enable tx fifo interrupt
    }

    r = wait_for_completion_interruptible_timeout(&dev->cmd_complete, VCOREIII_XFER_TIMEOUT);
    dev->buf_len = 0;
    if (r < 0)
        return r;

    /* no error */
    if (likely(r != 0 && !dev->cmd_err))
        return msg->len;

    /* Something bad happened */
    i2c_reset(adap);

    if (r == 0) {
        DEBUG_I2C("controller timed out\n");
        return -ETIMEDOUT;
    }

    /* Something else... */
    return -EIO;
}

static int vcoreiii_xfer(struct i2c_adapter *adap, struct i2c_msg *pmsg, int num)
{
    int i, rc = 0;

    DEBUG_I2C_L(2, "%s: vcoreiii_xfer - processing %d messages:\n", MOD_NAME, num);

    for(i = 0; i < num; i++, pmsg++) {
	DEBUG_I2C("MSG #%d: %sing %d byte%s %s 0x%02x flags %0x\n", i,
		  pmsg->flags & I2C_M_RD ? "read" : "writ",
		  pmsg->len, pmsg->len == 1 ? "" : "s",
		  pmsg->flags & I2C_M_RD ? "from" : "to", pmsg->addr, pmsg->flags);
        if((rc = do_xfer(adap, pmsg)) < 0)
            return rc;
    }
    return num;
}

static unsigned int vcoreiii_func(struct i2c_adapter *adapter)
{
    return I2C_FUNC_I2C | I2C_FUNC_SMBUS_EMUL;
}

static irqreturn_t vcoreiii_twi_interrupt_entry(int irq, void *dev_id)
{
    struct vcoreiii_twi_iface *dev = dev_id;
    unsigned long status;
    int count = 0;

    while((status = readl(VTSS_TWI_TWI_INTR_STAT))) { // figure out what interrupt we got
        DEBUG_I2C_L(2, "IRQ stat 0x%lx\n", status);
        if (count++ == 100) {
            dev_err(dev->dev, "Too much work in one IRQ - stat 0x%lx\n", status);
            break;
        }
	if(status & VTSS_F_TWI_TWI_INTR_STAT_TX_ABRT) {
            DEBUG_I2C(" TX abrt src 0x%x\n", readl(VTSS_TWI_TWI_TX_ABRT_SOURCE));
            writel(0, VTSS_TWI_TWI_INTR_MASK);
            dev->cmd_err |= I2C_CMPLT_ABORT;
            complete(&dev->cmd_complete);
        } else if(status & VTSS_F_TWI_TWI_INTR_STAT_RX_FULL) {
            if (dev->buf_len) {
                *(dev->buf) = readl(VTSS_TWI_TWI_DATA_CMD);
                DEBUG_I2C("READ %x, %d bytes left\n", *(dev->buf), dev->buf_len);
                dev->buf++;
                if (--dev->buf_len == 0) {
                    writel(0, VTSS_TWI_TWI_INTR_MASK);
                    DEBUG_I2C("Read complete\n");
                    complete(&dev->cmd_complete);
                }
            } else {
                dev_err(dev->dev, "Data while idle? - 0x%lx\n", status);
            }
        } else if(status & VTSS_F_TWI_TWI_INTR_STAT_TX_EMPTY) {
            if (dev->buf_len) {
                stuff_tx(dev);
            } else {
                u32 lev = readl(VTSS_TWI_TWI_TXFLR);
                if(lev == 0) {
                    writel(0, VTSS_TWI_TWI_INTR_MASK);
                    DEBUG_I2C("Empty and done - status 0x%lx\n", status);
                    complete(&dev->cmd_complete);
                } else {
                    DEBUG_I2C("Empty and not done - status 0x%lx, level %d\n", status, lev);
                }
            }
        } else {
            dev_err(dev->dev, "Unexpected status 0x%lx\n", status);
        }
        readl(VTSS_TWI_TWI_CLR_INTR); // clear the interrupt(s)
    }
    return count ? IRQ_HANDLED : IRQ_NONE;
}

static struct i2c_algorithm i2c_vcoreiii_algo = {
    .master_xfer    = vcoreiii_xfer,
    .functionality  = vcoreiii_func,
};

static int i2c_vcoreiii_hwinit(const struct vcoreiii_i2c_platform_data *pdata)
{
    unsigned long clk_freq = VCOREIII_AHB_CLOCK, reg_val;

    reg_val = (5 * clk_freq / 1000000) - 8;  // datasheet 6.17.1.5
    writel(reg_val, VTSS_TWI_TWI_SS_SCL_HCNT);

    reg_val = (5 * clk_freq / 1000000) - 1;  // datasheet 6.17.1.6
    writel(reg_val, VTSS_TWI_TWI_SS_SCL_LCNT);

    reg_val = VTSS_F_ICPU_CFG_TWI_DELAY_TWI_CONFIG_TWI_CNT_RELOAD((unsigned int) (0.3 * clk_freq / 1000000) - 1) |
                                              VTSS_F_ICPU_CFG_TWI_DELAY_TWI_CONFIG_TWI_DELAY_ENABLE;  // datasheet 6.17
    writel(reg_val, VTSS_ICPU_CFG_TWI_DELAY_TWI_CONFIG);

    reg_val = (1.1 * clk_freq / 1000000) - 1;  // datasheet 6.17.1.7
    writel(reg_val, VTSS_TWI_TWI_FS_SCL_HCNT);

    reg_val = (1.4 * clk_freq / 1000000) - 1;  // datasheet 6.17.1.8
    writel(reg_val, VTSS_TWI_TWI_FS_SCL_LCNT);

    reg_val =
        VTSS_F_TWI_TWI_CFG_MASTER_ENA |
        VTSS_F_TWI_TWI_CFG_SPEED(pdata->fast_mode ? 2 : 1) | /* 400 or 100 kbit/s */
        VTSS_F_TWI_TWI_CFG_RESTART_ENA |
        VTSS_F_TWI_TWI_CFG_SLAVE_DIS;
    writel(reg_val, VTSS_TWI_TWI_CFG);

    reg_val = (0.25 * clk_freq / 1000000);  // datasheet 6.17.1.30
    writel(reg_val, VTSS_TWI_TWI_SDA_SETUP);

    writel(0, VTSS_TWI_TWI_RX_TL); /* (n+1) => one byte of data */
    writel(0x0, VTSS_TWI_TWI_INTR_MASK); // mask all until we're ready
    writel(VTSS_F_TWI_TWI_CTRL_ENABLE, VTSS_TWI_TWI_CTRL);

    return 0;
}

static struct vcoreiii_i2c_platform_data i2c_data_default = {
    .fast_mode = 0,             /* 100 kbit/s */
};

static int i2c_vcoreiii_probe(struct platform_device *pdev)
{
    struct vcoreiii_i2c_platform_data *pdata = pdev->dev.platform_data;
    struct vcoreiii_twi_iface *iface;
    struct i2c_adapter *adapter;
    int rc=1;

    DEBUG_I2C_L(2, "%s: i2c_vcoreiii_probe, pdata %p\n", MOD_NAME, pdata);
    if(!pdata)
        pdata = &i2c_data_default;

    if (gpio_request(VCOREIII_TWI_SCL, "TWI_SCL") < 0)
	goto failscl;
    if (gpio_request(VCOREIII_TWI_SDA, "TWI_SDA") < 0)
	goto failsda;

    vcoreiii_gpio_set_alternate(VCOREIII_TWI_SCL, 1); /* TWI_SCL */
    vcoreiii_gpio_set_alternate(VCOREIII_TWI_SDA, 1); /* TWI_SDA */

    i2c_vcoreiii_hwinit(pdata);

    iface = kzalloc(sizeof(struct vcoreiii_twi_iface), GFP_KERNEL);
    if(iface == NULL) {
        dev_err(&pdev->dev, "can't allocate interface\n");
        rc = -ENOMEM;
        goto failiface;
    }

    iface->dev = get_device(&pdev->dev);
    iface->irq = TWI_IRQ;
    spin_lock_init(&iface->lock);
    platform_set_drvdata(pdev, iface);

    adapter = &iface->adap;
    i2c_set_adapdata(adapter, iface);
    adapter->owner = THIS_MODULE;
    adapter->class = I2C_CLASS_HWMON;
    snprintf(adapter->name, sizeof(adapter->name), MOD_NAME);
    adapter->algo = &i2c_vcoreiii_algo;
    adapter->dev.parent = &pdev->dev;

    rc = request_irq(iface->irq, vcoreiii_twi_interrupt_entry, 0, pdev->name, iface);
    if (rc) {
	dev_err(&pdev->dev, "Can't get IRQ %d !\n", iface->irq);
	rc = -ENODEV;
	goto failirq;
    }

    rc = i2c_add_numbered_adapter(adapter);
    if(rc) {
        dev_err(&pdev->dev, "Adapter %s registration failed\n", adapter->name);
        goto failreg;
    }

    dev_info(&pdev->dev, "i2c bus driver on IRQ %d\n", iface->irq);
    return 0;

failreg:
    free_irq(iface->irq, iface);
failirq:
    platform_set_drvdata(pdev, NULL);
    kfree(adapter);
failiface:
    gpio_free(VCOREIII_TWI_SDA);
failsda:
    gpio_free(VCOREIII_TWI_SCL);
failscl:
    return 0;
}

static int i2c_vcoreiii_remove(struct platform_device *pdev)
{
    struct vcoreiii_twi_iface *iface = platform_get_drvdata(pdev);

    dev_info(&pdev->dev, "Driver remove (releasing IRQ %d)\n", iface->irq);

    platform_set_drvdata(pdev, NULL);

    i2c_del_adapter(&(iface->adap));
    free_irq(iface->irq, iface);
    kfree(iface);

    vcoreiii_gpio_set_alternate(VCOREIII_TWI_SCL, 0);
    vcoreiii_gpio_set_alternate(VCOREIII_TWI_SDA, 0);
    gpio_free(VCOREIII_TWI_SCL);
    gpio_free(VCOREIII_TWI_SDA);

    return 0;
}

static struct platform_driver i2c_vcoreiii_driver = {
	.probe		= i2c_vcoreiii_probe,
	.remove		= i2c_vcoreiii_remove,
	.driver		= {
		.name	= MOD_NAME,
		.owner	= THIS_MODULE,
	},
};

static int __init i2c_vcoreiii_init(void)
{
    if (vcoreiii_check_chip_id()) {
#ifdef DEBUG
	debug++;
#endif
	return platform_driver_register(&i2c_vcoreiii_driver);
    }
    return -ENODEV;
}
subsys_initcall(i2c_vcoreiii_init);

static void __exit i2c_vcoreiii_exit(void)
{
    platform_driver_unregister(&i2c_vcoreiii_driver);
}
module_exit(i2c_vcoreiii_exit);

module_param(debug, int, 0644);
MODULE_PARM_DESC(debug, "Enable debug");

/* work with hotplug and coldplug */
MODULE_AUTHOR("Lars Povlsen <lpovlsen at vitesse.com>");
MODULE_ALIAS("platform:i2c_vcoreiii");
MODULE_DESCRIPTION("Vitesse VCore-III I2C bus adapter");
MODULE_LICENSE("GPL");
