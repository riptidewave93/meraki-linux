/*
 * md124.c
 *
 * Copyright (c) 2011 Vivotek, Inc
 * Author: Ramax Lo <ramax.lo@vivotek.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

/*
 * The driver uses gpiolib as the backend to control the motor driver
 * chip. The driver registers itself as a platform driver to kernel,
 * and you should register a platform device to pair with it.
 *
 * The example code is listed below:
 *
 * struct a3967_platdata xxxx_motor_platdata = {
 *     .gpio_reset         = 13,
 *     .gpio_enable        = 14,
 *     .gpio_dir           = 15,
 *     .gpio_step          = 16,
 *     .gpio_ms1           = -1,
 *     .gpio_ms2           = -1,
 *     .gpio_sleep         = -1,
 *     .pulse_per_step     = 1,
 *     .clockwise_forward  = 0,
 * };
 *
 * struct platform_device xxxx_motor_device = {
 *     .name  = "a3967",
 *     .id    = 0,
 *     .dev   = {
 *         .platform_data = &xxxx_motor_platdata,
 *     },
 * };
 *
 * void __init xxxx_board_init(void)
 * {
 *     ......
 *
 *     platform_device_register(&xxxx_motor_device);
 * }
 *
 * Note the driver is designed for working with Allegro A3967 only,
 * and if you want to port it for chips from other vendors, you may
 * need to modify the platform data structure definitions and timing
 * to fit the chip specification.
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/gpio.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/cdev.h>
#include <linux/interrupt.h>
#include <linux/hrtimer.h>
#include <linux/completion.h>
#include <linux/uaccess.h>
#include <linux/semaphore.h>
#include <linux/jiffies.h>
#include <linux/time.h>
#include <mach/md124.h>
#include "motor_ioctl.h"

MODULE_DESCRIPTION("MD124 motor driver");
MODULE_AUTHOR("Evan Chen <evan.chen@vivotek.com>");
MODULE_VERSION("0.02");
MODULE_LICENSE("GPL");

struct md124_data {
	dev_t			dev;
	struct cdev		cdev;
	struct class		*cls;
	struct gpio		*gpios;
	int			gpio_enable;
	int			gpio_in1;
	int			gpio_in2;
	bool        forward;
	int			opened;
	int			delay;
	int         speed;
	int			total_pattern;
	struct hrtimer		timer;
	int			remaining_steps;
	struct semaphore sem;
	struct completion	completion;
};

//char g_acForwardCmd[4] = {0x02, 0x03, 0x01, 0x00};
//char g_acBackwardCmd[4] = {0x01, 0x03, 0x02, 0x00};
char g_acStepCmd[4] = {0x02, 0x03, 0x01, 0x00};
static int md124_speed_values[] = {5, 5, 10, 15};//{3, 3, 6, 9};
static int g_iPulseExtend = 10;    //10ms extension
static struct class *cls = NULL;
static int g_byDEBUG = 0; // debug message level;
static int g_iFirstStep = 0;

static int md124_enable_motor(struct md124_data *data)
{
	gpio_set_value(data->gpio_enable, 1);

	return 0;
}

static int md124_disable_motor(struct md124_data *data)
{
	gpio_set_value(data->gpio_enable, 0);

	return 0;
}

static int md124_hw_init(struct md124_data *data)
{
	gpio_set_value(data->gpio_enable, 1);

	return 0;
}

static int md124_open(struct inode *inode, struct file *file)
{
	struct md124_data *drvdata = container_of(inode->i_cdev, struct md124_data, cdev);

	if (drvdata->opened)
		return -EBUSY;

	//md124_enable_motor(drvdata);
	drvdata->opened = 1;
	file->private_data = drvdata;

	return 0;
}

static int md124_release(struct inode *inode, struct file *file)
{
	struct md124_data *drvdata = (struct md124_data *)file->private_data;

	md124_disable_motor(drvdata);
	drvdata->opened = 0;

	return 0;
}

/******************************************************************************
* motor_single_step
*****************************************************************************/
int motor_single_step(struct md124_data *drvdata)
{
	static int iQuantCount = -1;

	if (iQuantCount < 0)	//Initial step
	{
		iQuantCount = 0;
	}
	else
	{
		if (drvdata->forward) // Forward
		{
			iQuantCount++;
	        if (iQuantCount == drvdata->total_pattern)
	        {
	        iQuantCount = 0;
	        }
		}
		else	// Backward
		{
			if (iQuantCount == 0)
	        {
	        iQuantCount = drvdata->total_pattern - 1;
	        }
	        else
	        {
	        iQuantCount--;
	        }
		}
	}

	gpio_set_value(drvdata->gpio_in1, (g_acStepCmd[iQuantCount] >> 1));
    gpio_set_value(drvdata->gpio_in2, (g_acStepCmd[iQuantCount] & 0x01));

	return 0;
}

static int md124_set_forward(struct md124_data *drvdata)
{
    //down_interruptible(&drvdata->sem);
	drvdata->forward = true;
	//up(&drvdata->sem);

	return 0;
}

static int md124_set_backward(struct md124_data *drvdata)
{
	//down_interruptible(&drvdata->sem);
	drvdata->forward = false;
	//up(&drvdata->sem);

	return 0;
}

static int md124_set_speed(struct md124_data *drvdata, int speed)
{
    //down_interruptible(&drvdata->sem);
    drvdata->speed = speed;
	if (speed > ARRAY_SIZE(md124_speed_values))
		speed = ARRAY_SIZE(md124_speed_values) - 1;
	else if (speed < 0)
		speed = 0;

	drvdata->delay = md124_speed_values[speed];
	if (g_byDEBUG > 0)
		printk("Set motor_set_speed :%d \n", speed);

    //up(&drvdata->sem);
	return 0;
}

static enum hrtimer_restart md124_timer_notify(struct hrtimer *timer)
{
	struct md124_data *drvdata = container_of(timer, struct md124_data, timer);
    //static ktime_t now, stamp_1, stamp_2;
	struct timeval tv;

	//stamp_2 = hrtimer_cb_get_time(timer);
    //printk("remaining_steps: %d\n", drvdata->remaining_steps);
    //printk("%u us\n", (stamp_2.tv.nsec-stamp_1.tv.nsec)/1000);
    //stamp_1.tv.nsec = stamp_2.tv.nsec;
	if (drvdata->remaining_steps >= 0)
	{
	    if (drvdata->remaining_steps == 0)
        {
            md124_disable_motor(drvdata);
            complete(&drvdata->completion);
            return HRTIMER_NORESTART;
        }

		motor_single_step(drvdata);
		--drvdata->remaining_steps;
		/*
        if (drvdata->remaining_steps == 0)
        {
            md124_disable_motor(drvdata);
            complete(&drvdata->completion);
            return HRTIMER_NORESTART;
        }*/
	}

	if (1 == g_iFirstStep)
    {
        tv.tv_sec = 0;
        tv.tv_usec = g_iPulseExtend * 1000;//evan: Stepper motor needs about 10ms extention at the begin to speed up slowly.
        g_iFirstStep = 0;
    }
    else
    {
        tv.tv_sec = 0;
        tv.tv_usec = drvdata->delay * 1000;
    }

	if (0 == drvdata->remaining_steps)
	{
	    tv.tv_sec = 0;
        tv.tv_usec = g_iPulseExtend * 1000;//evan: Stepper motor needs about 10ms extention at the end to slow down slowly.
	}
	//now = hrtimer_cb_get_time(timer);
	//hrtimer_forward(timer, now, timeval_to_ktime(tv));
	hrtimer_forward_now(timer, timeval_to_ktime(tv));

	return HRTIMER_RESTART;
}

static int md124_walk_steps(struct md124_data *drvdata, int steps)
{

	struct timeval tv;

	tv.tv_sec = 0;//drvdata->delay;//0;
	tv.tv_usec = drvdata->delay * 1000;

	if (g_byDEBUG > 0)
		printk(">>>>>>> motor_walk_steps :%d \n", steps);

	//down_interruptible(&drvdata->sem);
	drvdata->remaining_steps = steps;
	if (1 == steps) //Only one step, set total pulse time=10ms
	{
	    g_iFirstStep = 0;
	}
	else    //> 1 step, add 10ms extension at the begin of pulse.
	{
	    g_iFirstStep = 1;
	}
    if(drvdata->remaining_steps > 0)
    {
        md124_enable_motor(drvdata);
        hrtimer_start(&drvdata->timer, timeval_to_ktime(tv), HRTIMER_MODE_REL);
        wait_for_completion_interruptible(&drvdata->completion);
    }

	return 0;
}

/******************************************************************************
* motor_system_log
*****************************************************************************/
int motor_system_log(struct md124_data *drvdata, int log_level)
{
	if (log_level > 0)
	{
		printk("Motor-Iris Info:\n \
			Current Direction: %s \n \
			Current Speed: %d \n \
			Current Device Mode: %d \n \
			Current I2C Address: None \n", (drvdata->forward) ? "Forward": "Backward"
										, drvdata->speed
										, 1);
	}

	return 0;
}

static long md124_ioctl(struct file *file, unsigned int cmd, unsigned long data)
{
	struct md124_data *drvdata = file->private_data;
	int rc = 0;

	if (g_byDEBUG > 0)
		printk("type: '%c' cmd: 0x%x data:%d \n", _IOC_TYPE( cmd ), _IOC_NR( cmd ), (int)data);

	if (_IOC_TYPE(cmd) != MOTOR_MAGIC) {
		pr_err("Invalid command\n");
		return -EINVAL;
	}

	if (_IOC_NR(cmd) > MOTOR_MAXNR) {
		pr_err("Invalid command number\n");
		return -EINVAL;
	}

	switch (cmd) {
		case MOTOR_SET_FORWARD:
			md124_set_forward(drvdata);
			break;
		case MOTOR_SET_BACKWARD:
			md124_set_backward(drvdata);
			break;
		case MOTOR_SET_SPEED:
			md124_set_speed(drvdata, (int)data);
			break;
		case MOTOR_WALK_STEPS:
			md124_walk_steps(drvdata, (int)data);
			break;
		case MOTOR_WALK_STEPS_CLR:
		    drvdata->remaining_steps = 1; // Clear the current walk steps, Forced to set the remaining 1 step
			break;
		case MOTOR_TURN_OFF:
			md124_disable_motor(drvdata);
			break;
		case MOTOR_SET_MSG_LEVEL:
		    g_byDEBUG = (int)data;
			break;
		case MOTOR_SYS_LOG_LIST:
		    motor_system_log(drvdata, (int)data);
			break;
		default:
			rc = -EINVAL;
			break;
	}

	return rc;
}

static struct file_operations md124_fops = {
	.open		= md124_open,
	.unlocked_ioctl	= md124_ioctl,
	.release	= md124_release,
};

static int __devinit md124_probe(struct platform_device *pdev)
{
	struct md124_platdata *pdata = pdev->dev.platform_data;
	struct gpio *gpios;
	int ngpios = 3;
	struct md124_data *drvdata;
	dev_t dev;
	struct device *device;
	int rc;

	/* Request kernel for exclusive privilege of GPIOs */
	gpios = kmalloc(sizeof(struct gpio) * ngpios, GFP_KERNEL);
	if (!gpios) {
		pr_err("Memory allocation failure\n");
		return -ENOMEM;
	}

	gpios[0].gpio = pdata->gpio_enable;
	gpios[0].flags = GPIOF_OUT_INIT_LOW;
	gpios[0].label = "md124";

	gpios[1].gpio = pdata->gpio_in1;
	gpios[1].flags = GPIOF_OUT_INIT_LOW;
	gpios[1].label = "md124";

	gpios[2].gpio = pdata->gpio_in2;
	gpios[2].flags = GPIOF_OUT_INIT_LOW;
	gpios[2].label = "md124";


	rc = gpio_request_array(gpios, ngpios);
	if (rc) {
		pr_err("GPIO request failure\n");
		goto err_request;
	}

	/* Allocate driver private data structure */
	drvdata = kmalloc(sizeof(struct md124_data), GFP_KERNEL);
	if (!drvdata) {
		pr_err("Driver data allocation failure\n");
		rc = -ENOMEM;
		goto err_drvdata;
	}
	memset(drvdata, 0, sizeof(struct md124_data));

	/* Register character device to kernel */
	rc = alloc_chrdev_region(&dev, 0, 1, "md124");
	if (rc) {
		pr_err("Character device allocation failure\n");
		goto err_chrdev;
	}

	cdev_init(&drvdata->cdev, &md124_fops);
	drvdata->cdev.owner = THIS_MODULE;

	rc = cdev_add(&drvdata->cdev, dev, 1);
	if (rc) {
		pr_err("cdev add failure\n");
		goto err_cdev_add;
	}

	/* Create device nodes in sysfs */
	if (!cls) {
		cls = class_create(THIS_MODULE, "md124");
		if (IS_ERR(cls)) {
			pr_err("Class creation failure\n");
			rc = PTR_ERR(cls);
			goto err_cls_create;
		}
	}

	device = device_create(cls, &pdev->dev, dev, NULL, "aperture");
	if (IS_ERR(device)) {
		pr_err("Device creation failure\n");
		rc = PTR_ERR(device);
		goto err_cls_create;
	}

	/* Fill the private data structure */
	drvdata->dev = dev;
	drvdata->cls = cls;
	drvdata->gpios = gpios;
	drvdata->gpio_enable = pdata->gpio_enable;
	drvdata->gpio_in1 = pdata->gpio_in1;
	drvdata->gpio_in2 = pdata->gpio_in2;
	drvdata->total_pattern = sizeof(g_acStepCmd);
	/* initial hrtimer */
	hrtimer_init(&drvdata->timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	drvdata->timer.function = md124_timer_notify;
	init_completion(&drvdata->completion);

	//sema_init (&drvdata->sem, 1);

	md124_set_speed(drvdata, 0);

	platform_set_drvdata(pdev, drvdata);

	pr_info("md124 register done\n");

	return 0;

err_cls_create:
	cdev_del(&drvdata->cdev);

err_cdev_add:
	unregister_chrdev_region(dev, 1);

err_chrdev:
	kfree(drvdata);

err_drvdata:
	gpio_free_array(gpios, ngpios);

err_request:
	kfree(gpios);

	return rc;
}

static int __devexit md124_remove(struct platform_device *pdev)
{
	struct md124_data *drvdata = platform_get_drvdata(pdev);
	int ngpios = 3;

	device_destroy(drvdata->cls, drvdata->dev);

	cdev_del(&drvdata->cdev);
	unregister_chrdev_region(drvdata->dev, 1);
	gpio_free_array(drvdata->gpios, ngpios);

	kfree(drvdata->gpios);
	kfree(drvdata);
	pr_info("md124 deregister done\n");

	return 0;
}

static struct platform_driver md124_driver = {
	.probe		= md124_probe,
	.remove		= __devexit_p(md124_remove),
	.driver		= {
		.name	= "piris-motor",
		.owner	= THIS_MODULE,
	},
};

static int __init md124_init(void)
{
	int rc;

	rc = platform_driver_register(&md124_driver);

	return rc;
}

static void __exit md124_exit(void)
{
	platform_driver_unregister(&md124_driver);

	if (cls)
		class_destroy(cls);
}

module_init(md124_init);
module_exit(md124_exit);
