/*
 *    Copyright (c) 2010 Grant Erickson <marathon96@gmail.com>
 *
 *    This program is free software; you can redistribute it and/or
 *    modify it under the terms of the GNU General Public License
 *    version 2 as published by the Free Software Foundation.
 *
 *    Description:
 *      This file is the core OMAP2/3 support for the generic, Linux
 *      PWM driver / controller, using the OMAP's dual-mode timers.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/err.h>
#include <linux/clk.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/pwm.h>
#include <mach/hardware.h>
#include <plat/dmtimer.h>
#include <plat/pwm.h>
#include <linux/proc_fs.h>
#include <asm/mach-types.h>

// walter @ 20140711 add for ioctl
#include <linux/init.h>
#include <linux/types.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <asm/io.h>

/* Preprocessor Definitions */

#undef OMAP_PWM_DEBUG

#if defined(OMAP_PWM_DEBUG)
#define DBG(args...)			\
	do {						\
		pr_info(args);			\
	} while (0)
#define DEV_DBG(dev, args...)	\
	do {						\
		dev_info(dev, args);	\
	} while (0)
#else
#define DBG(args...)			\
	do { } while (0)
#define DEV_DBG(dev, args...)	\
	do { } while (0)
#endif /* defined(OMAP_PWM_DEBUG) */

#define DM_TIMER_LOAD_MIN		0xFFFFFFFE
#define PWM_DEVS				1

/* Type Definitions */

/**
 * struct pwm_device - opaque internal PWM device instance state
 * @head: list head for all PWMs managed by this driver.
 * @pdev: corresponding platform device associated with this device instance.
 * @dm_timer: corresponding dual-mode timer associated with this device
 *  instance.
 * @config: platform-specific configuration data.
 * @label: description label.
 * @use_count: use count.
 * @pwm_id: generic PWM ID requested for this device instance.
 *
 * As far as clients of the PWM driver are concerned, PWM devices are
 * opaque abstract objects. Consequently, this structure is used for
 * tracking internal device instance state but is otherwise just a
 * instance reference externally.
 */

struct pwm_device {
	struct list_head				   head;
	struct platform_device			  *pdev;
	struct omap_dm_timer			  *dm_timer;
	struct omap2_pwm_platform_config   config;
	const char						  *label;
	unsigned int					   use_count;
	unsigned int					   pwm_id;
	int								   duty_ns;
	int								   period_ns;
	struct proc_dir_entry			   *pwm_proc;
};

/* Function Prototypes */

static int __devinit omap_pwm_probe(struct platform_device *pdev);
static int __devexit omap_pwm_remove(struct platform_device *pdev);

static int pwm_open(struct inode *inode, struct file *file);
static long pwm_ioctl(struct file *file, unsigned int cmd, unsigned long data);
static int pwm_release(struct inode *inode, struct file *file);

/* Global Variables */
// walter @ 20140711 add for ioctl
static struct pwm_device *pwm_table[PWM_DEVS];
static struct file_operations pwm_fops = {
	.open		= pwm_open,
	.unlocked_ioctl	= pwm_ioctl,
	.release	= pwm_release,
};

static struct platform_driver omap_pwm_driver = {
	.driver		= {
		.name	= "omap-pwm",
		.owner	= THIS_MODULE,
	},
	.probe		= omap_pwm_probe,
	.remove		= __devexit_p(omap_pwm_remove)
};

static struct proc_dir_entry *pwm_procdir = NULL;
/* List and associated lock for managing generic PWM devices bound to
 * this driver.
 */

static DEFINE_MUTEX(pwm_lock);
static LIST_HEAD(pwm_list);

// walter @ 20140711 add for ioctl
MODULE_DESCRIPTION("pwm driver");

static int pwm_major = 0;
static struct cdev pwm_cdev;
static struct class *pwm_class = NULL;
static dev_t pwm_dev = NULL;

/**
 * pwm_request - request and allocate the specified generic PWM device.
 * @pwm_id: The identifier associated with the desired generic PWM device.
 * @label: An optional pointer to a C string describing the usage of the
 *         requested generic PWM device.
 *
 * Returns a pointer to the requested generic PWM device on success;
 * otherwise, NULL on error.
 */
struct pwm_device *pwm_request(int pwm_id, const char *label)
{
	struct pwm_device *pwm = NULL;
	bool found = false;

	mutex_lock(&pwm_lock);

	/* Walk the list of available PWMs and attempt to find a matching
	 * ID, regardless of whether it is in use or not.
	 */

	list_for_each_entry(pwm, &pwm_list, head) {
		if (pwm->pwm_id == pwm_id) {
			found = true;
			break;
		}
	}

	if (found) {
		if (pwm->use_count == 0) {
			pwm->use_count++;
			pwm->label = label;
		} else {
			pwm = ERR_PTR(-EBUSY);
		}
	} else {
		pwm = ERR_PTR(-ENOENT);
	}

	mutex_unlock(&pwm_lock);

	return pwm;
}
EXPORT_SYMBOL(pwm_request);

/**
 * pwm_free - deallocate/release a previously-requested generic PWM device.
 * @pwm: A pointer to the generic PWM device to release.
 */
void pwm_free(struct pwm_device *pwm)
{
	mutex_lock(&pwm_lock);

	if (pwm->use_count) {
		pwm->use_count--;
		pwm->label = NULL;
	} else {
		pr_err("PWM%d has already been freed.\n", pwm->pwm_id);
	}

	mutex_unlock(&pwm_lock);
}
EXPORT_SYMBOL(pwm_free);

/**
 * pwm_calc_value - determines the counter value for a clock rate and period.
 * @clk_rate: The clock rate, in Hz, of the PWM's clock source to compute the
 *            counter value for.
 * @ns: The period, in nanoseconds, to computer the counter value for.
 *
 * Returns the PWM counter value for the specified clock rate and period.
 */
static inline int pwm_calc_value(unsigned long clk_rate, int ns)
{
	const unsigned long nanoseconds_per_second = 1000000000;
	int cycles;
	__u64 c;
	
	// walter @ 20140711 add for ioctl
	// clk_rate is 20MHz when use source OMAP_TIMER_SRC_SYS_CLK 
	c = (__u64)clk_rate * ns;
	do_div(c, nanoseconds_per_second);
	cycles = c;

	return DM_TIMER_LOAD_MIN - cycles;
}

/**
 * pwm_config - configures the generic PWM device to the specified parameters.
 * @pwm: A pointer to the PWM device to configure.
 * @duty_ns: The duty period of the PWM, in nanoseconds.
 * @period_ns: The overall period of the PWM, in nanoseconds.
 *
 * Returns 0 if the generic PWM device was successfully configured;
 * otherwise, < 0 on error.
 */
int pwm_config(struct pwm_device *pwm, int duty_ns, int period_ns)
{
	int status = 0;
	const bool enable = true;
	const bool autoreload = true;
	const bool toggle = true;
	const int trigger = OMAP_TIMER_TRIGGER_OVERFLOW_AND_COMPARE;
	int load_value, match_value;
	unsigned long clk_rate;

	DEV_DBG(&pwm->pdev->dev,
			"duty cycle: %d, period %d\n",
			duty_ns, period_ns);

	clk_rate = clk_get_rate(omap_dm_timer_get_fclk(pwm->dm_timer));

	/* Calculate the appropriate load and match values based on the
	 * specified period and duty cycle. The load value determines the
	 * cycle time and the match value determines the duty cycle.
	 */

	load_value = pwm_calc_value(clk_rate, period_ns);
	match_value = pwm_calc_value(clk_rate, period_ns - duty_ns);
	printk("load %X match %X duty %d period %d\n",load_value,match_value,duty_ns,period_ns);
	/* We MUST enable yet stop the associated dual-mode timer before
	 * attempting to write its registers.
	 */

	omap_dm_timer_enable(pwm->dm_timer);
	omap_dm_timer_stop(pwm->dm_timer);

	omap_dm_timer_set_load(pwm->dm_timer, autoreload, load_value);
	omap_dm_timer_set_match(pwm->dm_timer, enable, match_value);

	DEV_DBG(&pwm->pdev->dev,
			"load value: %#08x (%d), "
			"match value: %#08x (%d)\n",
			load_value, load_value,
			match_value, match_value);

	omap_dm_timer_set_pwm(pwm->dm_timer,
						  !pwm->config.polarity,
						  toggle,
						  trigger);

	/* Set the counter to generate an overflow event immediately. */

	omap_dm_timer_write_counter(pwm->dm_timer, DM_TIMER_LOAD_MIN);

	/* Now that we're done configuring the dual-mode timer, disable it
	 * again. We'll enable and start it later, when requested.
	 */

	omap_dm_timer_disable(pwm->dm_timer);

	/* Update status */
	mutex_lock(&pwm_lock);
	pwm->duty_ns = duty_ns;
	pwm->period_ns = period_ns;
	mutex_unlock(&pwm_lock);
	
	return status;
}
EXPORT_SYMBOL(pwm_config);

/**
 * pwm_enable - enable the generic PWM device.
 * @pwm: A pointer to the generic PWM device to enable.
 *
 * Returns 0 if the generic PWM device was successfully enabled;
 * otherwise, < 0 on error.
 */
int pwm_enable(struct pwm_device *pwm)
{
	int status = 0;

	/* Enable the counter--always--before attempting to write its
	 * registers and then set the timer to its minimum load value to
	 * ensure we get an overflow event right away once we start it.
	 */

	omap_dm_timer_enable(pwm->dm_timer);
	omap_dm_timer_write_counter(pwm->dm_timer, DM_TIMER_LOAD_MIN);
	omap_dm_timer_start(pwm->dm_timer);

	return status;
}
EXPORT_SYMBOL(pwm_enable);

/**
 * pwm_disable - disable the generic PWM device.
 * @pwm: A pointer to the generic PWM device to disable.
 */
void pwm_disable(struct pwm_device *pwm)
{
	omap_dm_timer_enable(pwm->dm_timer);
	omap_dm_timer_stop(pwm->dm_timer);
	omap_dm_timer_disable(pwm->dm_timer);
}
EXPORT_SYMBOL(pwm_disable);

static int proc_pwm_read(char *page, char **start, off_t off,
			 int count, int *eof, void *data)
{
	struct pwm_device *pwm = (struct pwm_device*)data;
	int len;

	if (count == 0)
		return 0;

	len = sprintf(page, "%s:duty cycle: %d, period %d\n",
			dev_name(&pwm->pdev->dev),
			pwm->duty_ns, pwm->period_ns);

	*eof = 1;
	*start = 0;

	return len;
}

// walter @ 20140711 add for ioctl
int pwm_set_control(struct pwm_device *pwm, int polarity, const bool toggle, const int trigger)
{
	int status = 0;

	omap_dm_timer_enable(pwm->dm_timer);
	omap_dm_timer_stop(pwm->dm_timer);

	omap_dm_timer_set_pwm(pwm->dm_timer,
						  polarity,
						  toggle,
						  trigger);

	omap_dm_timer_write_counter(pwm->dm_timer, DM_TIMER_LOAD_MIN);
	omap_dm_timer_disable(pwm->dm_timer);

	mutex_lock(&pwm_lock);
	pwm->config.polarity = !polarity;
	mutex_unlock(&pwm_lock);
	
	return status;
}

// walter @ 20140711 add for ioctl
int pwm_set_load(struct pwm_device *pwm, const bool autoreload)
{
	int status = 0;
	unsigned long clk_rate;
	int load_value;

	clk_rate = clk_get_rate(omap_dm_timer_get_fclk(pwm->dm_timer));
	load_value = pwm_calc_value(clk_rate, pwm->period_ns);

	omap_dm_timer_enable(pwm->dm_timer);
	omap_dm_timer_stop(pwm->dm_timer);

	omap_dm_timer_set_load(pwm->dm_timer, autoreload, load_value);
	omap_dm_timer_write_counter(pwm->dm_timer, DM_TIMER_LOAD_MIN);
	omap_dm_timer_disable(pwm->dm_timer);
	
	return status;
}

// walter @ 20140711 add for ioctl
int pwm_set_duration(struct pwm_device *pwm, int duty_ns)
{
	int status = 0;
	int match_value;
	unsigned long clk_rate;
	const bool enable = true;

	clk_rate = clk_get_rate(omap_dm_timer_get_fclk(pwm->dm_timer));

	/* Calculate the appropriate load and match values based on the
	 * specified period and duty cycle. The load value determines the
	 * cycle time and the match value determines the duty cycle.
	 */

	match_value = pwm_calc_value(clk_rate, pwm->period_ns - duty_ns);
	printk("Set Dur %d match %X\n", duty_ns,match_value);
	omap_dm_timer_set_match(pwm->dm_timer, enable, match_value);
	
	/* Update status */
	mutex_lock(&pwm_lock);
	pwm->duty_ns = duty_ns;
	mutex_unlock(&pwm_lock);

	return status;
}

static int proc_pwm_write(struct file *file, const char __user * buffer,
			  unsigned long count, void *data)
{
	struct pwm_device *pwm = (struct pwm_device*)data;
	int duty_ns;

	if (!count)
		return 0;
	if(buffer[0] == '+'){
		sscanf(buffer+1, "%d", &duty_ns);
		duty_ns = pwm->duty_ns + duty_ns;
	}else if(buffer[0] == '-'){
		sscanf(buffer+1, "%d", &duty_ns);
		duty_ns = pwm->duty_ns - duty_ns;
	}else
		sscanf(buffer, "%d", &duty_ns);

	pwm_set_duration(pwm, duty_ns);

	return count;
}

// walter @ 20140711 add for ioctl
static int pwm_dev_client_create(void)
{
	dev_t dev = MKDEV(pwm_major, 0);
	int alloc_ret = 0;
	int major;
	int cdev_err = 0;
	struct class_device* class_dev = NULL;
	int i;
	
	/* Register character device to kernel */
	alloc_ret = alloc_chrdev_region(&dev, 0, PWM_DEVS, "pwm");
	if (alloc_ret) {
		pr_err("Character device allocation failure\n");
		goto error;
	}
	pwm_major = major = MAJOR(dev);

	cdev_init(&pwm_cdev, &pwm_fops);
	pwm_cdev.owner = THIS_MODULE;
	pwm_cdev.ops = &pwm_fops;

	cdev_err = cdev_add(&pwm_cdev, MKDEV(pwm_major, 0), PWM_DEVS);
	if (cdev_err) {
		pr_err("cdev add failure\n");
		goto error;
	}

	/* Create device nodes in sysfs */
	pwm_class = class_create(THIS_MODULE, "pwm");
	
	if(IS_ERR(pwm_class))
		goto error;
	
	for(i=0; i<PWM_DEVS; ++i)
	{
		pwm_dev = MKDEV(pwm_major, i);
		/* Create device nodes in dev */
		class_dev = device_create(pwm_class,NULL,pwm_dev,NULL,"pwm/%d",i);
	}
	printk(KERN_ALERT "pwm driver (major %d) installed.\n", major);

	return 0;
	
error:
	if(cdev_err == 0)
		cdev_del(&pwm_cdev);
		
	if(alloc_ret == 0)
		unregister_chrdev_region(dev,PWM_DEVS);

	return -1;
}

static int __init pwm_proc_client_create(struct pwm_device *pwm)
{
	char name[16];

	if(pwm_procdir == NULL)
		pwm_procdir = proc_mkdir("pwm", 0);

	if (pwm_procdir == NULL) {
		dev_err(&pwm->pdev->dev,
			"Failed to register proc directory pwm\n");
		return -ENOMEM;
	}
	
	sprintf(name, "pwm%d", pwm->pwm_id);
	mutex_lock(&pwm_lock);
	pwm->pwm_proc = create_proc_entry(name, 0, pwm_procdir);
	if (pwm->pwm_proc == NULL) {
		mutex_unlock(&pwm_lock);
		dev_err(&pwm->pdev->dev,
		       "Failed to register proc device: %s\n",
		       name);
		return -ENOMEM;
	}
	pwm->pwm_proc->data = (void*)pwm;
	pwm->pwm_proc->read_proc = proc_pwm_read;
	pwm->pwm_proc->write_proc = proc_pwm_write;
	mutex_unlock(&pwm_lock);

	return 0;
}

/**
 * omap_pwm_probe - check for the PWM and bind it to the driver.
 * @pdev: A pointer to the platform device node associated with the
 *        PWM instance to be probed for driver binding.
 *
 * Returns 0 if the PWM instance was successfully bound to the driver;
 * otherwise, < 0 on error.
 */
static int __devinit omap_pwm_probe(struct platform_device *pdev)
{
	struct pwm_device *pwm = NULL;
	struct omap2_pwm_platform_config *pdata = NULL;
	int status = 0;

	pdata = ((struct omap2_pwm_platform_config *)(pdev->dev.platform_data));

	BUG_ON(pdata == NULL);

	if (pdata == NULL) {
		dev_err(&pdev->dev, "Could not find required platform data.\n");
		status = -ENOENT;
		goto done;
	}

	/* Allocate memory for the driver-private PWM data and state */

	pwm = kzalloc(sizeof(struct pwm_device), GFP_KERNEL);

	if (pwm == NULL) {
		dev_err(&pdev->dev, "Could not allocate memory.\n");
		status = -ENOMEM;
		goto done;
	}

	/* Request the OMAP dual-mode timer that will be bound to and
	 * associated with this generic PWM.
	 */

	pwm->dm_timer = omap_dm_timer_request_specific(pdata->timer_id);

	if (pwm->dm_timer == NULL) {
		status = -ENOENT;
		goto err_free;
	}

	/* Configure the source for the dual-mode timer backing this
	 * generic PWM device. The clock source will ultimately determine
	 * how small or large the PWM frequency can be.
	 *
	 * At some point, it's probably worth revisiting moving this to
	 * the configure method and choosing either the slow- or
	 * system-clock source as appropriate for the desired PWM period.
	 */

	omap_dm_timer_set_source(pwm->dm_timer, OMAP_TIMER_SRC_SYS_CLK);

	/* Cache away other miscellaneous driver-private data and state
	 * information and add the driver-private data to the platform
	 * device.
	 */

	pwm->pdev = pdev;
	pwm->pwm_id = pdev->id;
	pwm->config = *pdata;

	platform_set_drvdata(pdev, pwm);

	/* Finally, push the added generic PWM device to the end of the
	 * list of available generic PWM devices.
	 */

	mutex_lock(&pwm_lock);
	list_add_tail(&pwm->head, &pwm_list);
	mutex_unlock(&pwm_lock);

	status = 0;
	goto done;

 err_free:
	kfree(pwm);

 done:

	if(machine_is_dm385ipnc() || machine_is_ti8148ipnc()){
		/* 
		   |-d-|
		    ___     ___     ___     ___
		___|   |___|   |___|   |___|   |___

		   |<--p-->|
		d : duty time
		p : period
		*/
		// walter @ 20140711 add for ioctl
		/* PWM IR LED */
		pwm_config(pwm, 0, 2000000);
		pwm_enable(pwm);
		if(pwm_proc_client_create(pwm) == 0)
			printk(KERN_INFO"PWM%d init success.\n", pwm->pwm_id);

		pwm_table[pwm->pwm_id] = pwm;
	}

	return status;
}

/**
 * omap_pwm_remove - unbind the specified PWM platform device from the driver.
 * @pdev: A pointer to the platform device node associated with the
 *        PWM instance to be unbound/removed.
 *
 * Returns 0 if the PWM was successfully removed as a platform device;
 * otherwise, < 0 on error.
 */
static int __devexit omap_pwm_remove(struct platform_device *pdev)
{
	struct pwm_device *pwm = NULL;
	int status = 0;

	/* Attempt to get the driver-private data from the platform device
	 * node.
	 */

	pwm = platform_get_drvdata(pdev);

	if (pwm == NULL) {
		status = -ENODEV;
		goto done;
	}

	/* Remove the generic PWM device from the list of available
	 * generic PWM devices.
	 */

	mutex_lock(&pwm_lock);
	list_del(&pwm->head);
	mutex_unlock(&pwm_lock);

	/* Unbind the OMAP dual-mode timer associated with the generic PWM
	 * device.
	 */

	omap_dm_timer_free(pwm->dm_timer);

	/* Finally, release the memory associated with the driver-private
	 * data and state.
	 */

	kfree(pwm);

 done:
	return status;
}

// walter @ 20140711 add for ioctl
static int pwm_open(struct inode *inode, struct file *file)
{
	printk(KERN_INFO"Open pwm%d\n",iminor(inode));
	file->private_data = (void*)pwm_table[iminor(inode)];

	return 0;
}

// walter @ 20140711 add for ioctl
static int pwm_release(struct inode *inode, struct file *file)
{
	// pwm will real release at omap_pwm_remove
	printk(KERN_INFO"Release pwm%d\n",iminor(inode));
	return 0;
}

// walter @ 20140711 add for ioctl
static long pwm_ioctl(struct file *file, unsigned int cmd, unsigned long data)
{
	struct pwm_device *pwm = (struct pwm_device *)file->private_data;
	int rc = 0;
	int duty_ns, period_ns;
	bool autoreload, polarity;

	if (_IOC_TYPE(cmd) != PWM_MAGIC) {
		pr_err("Invalid command\n");
		return -EINVAL;
	}

	if (_IOC_NR(cmd) > PWM_MAXNR) {
		pr_err("Invalid command number\n");
		return -EINVAL;
	}

	switch (cmd) {
		case PWMIOC_SET_DURATION:
			// data is 0~100
			// default period_ns = 10240
			// data change to 0~10240, one setp is 102.4
			duty_ns = (data * pwm->period_ns) / 100;
			printk("%s duty %d data %d\n",__func__,duty_ns,data);
			pwm_set_duration(pwm, duty_ns);
			break;
		case PWMIOC_SET_PERIOD:
			// keep duty:period same as old period
			// duty_ns = (pwm->duty_ns * data) / pwm->period_ns;
			duty_ns = data;
			period_ns = data;
			pwm_config(pwm, duty_ns, period_ns);
			pwm_enable(pwm);
			break;
		case PWMIOC_START:
			pwm_enable(pwm);
			break;
		case PWMIOC_STOP:
			pwm_disable(pwm);
			break;
		case PWMIOC_SET_MODE:
			// input data change to bool
			autoreload = !(!data);
			pwm_set_load(pwm, autoreload);
			pwm_enable(pwm);
			break;
		// Those two case is merge to one case, set SCPWM bit in TCLR
		// input data change to bool
		case PWMIOC_SET_FIRST_PHASE_STATE:
			// first phase state (only for DM368) 
		case PWMIOC_SET_INACT_OUT_STATE:
			// output phase state when timer stop (only for DM368) 
			polarity = !(!data);
			pwm_set_control(pwm, polarity, true, OMAP_TIMER_TRIGGER_OVERFLOW_AND_COMPARE);
			pwm_enable(pwm);
			break;
		case PWMIOC_SET_RPT_VAL:
			// repeat times of one shot mode (only for DM368)
			pr_err("Invalid command, not support PWMIOC_SET_RPT_VAL in IPNC\n");
			rc = -EINVAL;
			break;
		default:
			rc = -EINVAL;
			break;
	}
	
	return rc;
}

/**
 * omap_pwm_init - driver/module insertion entry point
 *
 * This routine is the driver/module insertion entry point. It
 * registers the driver as a platform driver.
 *
 * Returns 0 if the driver/module was successfully registered as a
 * platform driver driver; otherwise, < 0 on error.
 */
static int __init omap_pwm_init(void)
{
	if(pwm_dev_client_create() == 0)
	{
		printk(KERN_INFO"/dev/pwm create success.\n");
	}
	return platform_driver_register(&omap_pwm_driver);
}

/**
 * omap_pwm_exit - driver/module removal entry point
 *
 * This routine is the driver/module removal entry point. It
 * unregisters the driver as a platform driver.
 */
static void __exit omap_pwm_exit(void)
{
	int i;
	dev_t dev = MKDEV(pwm_major, 0);
	
	for(i=0; i<PWM_DEVS; ++i)
	{
		pwm_dev = MKDEV(pwm_major, i);
		device_destroy(pwm_class, pwm_dev);
	}
	class_destroy(pwm_class);
	
	cdev_del(&pwm_cdev);
	unregister_chrdev_region(dev, PWM_DEVS);
	
	printk(KERN_ALERT "pwm driver removed. \n");

	platform_driver_unregister(&omap_pwm_driver);
}

arch_initcall(omap_pwm_init);
module_exit(omap_pwm_exit);

MODULE_AUTHOR("Grant Erickson <marathon96@gmail.com>");
MODULE_LICENSE("GPLv2");
MODULE_VERSION("2010-11-09");
