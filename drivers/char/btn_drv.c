/**
 * Lanner Reset Button Driver (for MX50/70)
 */

/* Standard in kernel modules */
#include <linux/fs.h>
#include <linux/ioctl.h>
#include <linux/kernel.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <asm/io.h>
#include <asm/uaccess.h>

/* prevent access to the device at the same time */
static atomic_t device_open = ATOMIC_INIT(1);

/* Platform specific start */
/* vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv */

static unsigned char driver_version[] = "0.0.1 2010-09-08";

/*
 * Platform Depend GPIOs for Reset Button
 */
/*
 *------------------------------------------------------------------------------
 * MB-7565 Version V01
 *
 * The IO interface for Reset Button is connected to to Intel ICH8.
 * Refer to Inter ICH8 datasheet for details.
 * The high/low definition is as below:
 *	GPIO_PORT+0x0E	Button Status
 *  --------------------------
 *  0x10	Button Released
 *  else	Button Pressed
 *------------------------------------------------------------------------------
 */

#define INDEX_PORT	0x2e
#define DATA_PORT	0x2f
#define GPIO_PORT	0x500

#define MAGIC_CHAR	'B'
#define IOCTL_BTN_GET_STATUS	_IOW(MAGIC_CHAR, 0, int)

#define BTN_STATUS_RELEASED	0x00
#define BTN_STATUS_PRESSED	0x01

static void platform_information(void)
{
	printk("Reset Button Driver code for MB-7565...\n");
}

static int read_sb_gpio(int reg)
{
	return inb_p(GPIO_PORT + reg);
}

static int write_sb_gpio(unsigned int reg, unsigned int value)
{
	outb_p(value, GPIO_PORT+reg);

	return 0;
}

/* Platform specific end */
/*********************************************/

static long btn_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	int value;

	switch (cmd) {
	case IOCTL_BTN_GET_STATUS:
		value = read_sb_gpio(0x0e) & 0x10;
		if (value == 0x10)
			value = BTN_STATUS_RELEASED;
		else
			value = BTN_STATUS_PRESSED;
		if (copy_to_user((int *)arg, &value, sizeof(value)))
			return -EFAULT;

		break;
	default:
		return -EOPNOTSUPP;
	}

	return 0;
}

/*
 * This function is called whenever a process attempts to
 * open the device file
 */
static int btn_open(struct inode * inode, struct file * file)
{
	/*
	 * Get the minor device number in case you have more than
	 * one physical device using the driver.
	 */
	//printk("Device: %d.%d\n", inode->i_rdev>>8, inode->i_rdev & 0xff);

	/* we don't want to talk to two processes at the same time */
	if(__sync_bool_compare_and_swap(&device_open.counter, 1, 0)) {
		return -EBUSY;
	}

	/*
	 * Make sure that the module isn't removed while the file
	 * is open by incrementing the usage count (the number of
	 * opened references to the module,if it's zero emmod will
	 * fail)
	 */
	//printk("Lanner Reset Button Driver Opened\n");
	return 0;
}

/*
 * This function is called when a process closes the device file.
 */
static int btn_release(struct inode * inode, struct file * file)
{
	/* ready for next caller */
	__sync_add_and_fetch(&device_open.counter, 1);

	/*
	 * Decrement the usage count, otherwise once you opened the file
	 * you'll never get rid of the module.
	 */
	//printk("Lanner Reset Button Driver Closed\n");
	return 0;
}

/*
 * This structure will hold the functions to be called
 * when a process does something to the device we created.
 * Since a pointer to this structure is kept in the
 * devices table, it can't be local to init_module.
 * NULL is for unimplemented functions.
 */
static const struct file_operations btn_fops = {
	.owner		= THIS_MODULE,
	.unlocked_ioctl	= btn_ioctl,
	.compat_ioctl	= btn_ioctl,
	.open		= btn_open,
	.release	= btn_release,
};

static struct miscdevice btn_miscdev = {
	.fops		= &btn_fops,
	.nodename	= "btn_drv",
	.name		= "btn_drv",
	.minor		= MISC_DYNAMIC_MINOR,
};

static int btn_init(void)
{
	int result;

	/*
	 * Register the character device
	 */
	result = misc_register(&btn_miscdev);
	if (result < 0)
	{
		printk("btn : unable to register, err %d\n", result);
		return -EIO;
	}
	platform_information();
	printk("Reset Button Driver Version %s is loaded\n", driver_version);
	return 0;
}

/*
 * Cleanup - unregister the appropriate file from /proc
 */
static void btn_exit(void)
{
	/* Unregister the device */
	misc_deregister(&btn_miscdev);
	/* If there's an error, report it */
	printk("Reset Button Driver Version %s is unloaded\n", driver_version);
}

module_init(btn_init);
module_exit(btn_exit);

MODULE_AUTHOR("Lanner SW");
MODULE_DESCRIPTION("Lanner Reset Button for MX50/70");
MODULE_LICENSE("Dual BSD/GPL");
