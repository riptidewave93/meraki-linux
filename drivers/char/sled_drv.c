/**
 * Lanner Status LED Driver
 */

/* Standard in kernel modules */
#include <linux/atomic.h>
#include <linux/fs.h>
#include <linux/ioctl.h>
#include <linux/kernel.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <asm/io.h>

/* prevent access to the device at the same time */
static atomic_t device_open = ATOMIC_INIT(1);

/* Platform specific start */
/* vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv */

static unsigned char driver_version[] = "0.0.1 2010-09-08";

#define INDEX_PORT	0x2e
#define DATA_PORT	0x2f

#define MAGIC_CHAR	'S'
#define IOCTL_LED_SET_STATUS	_IOW(MAGIC_CHAR, 0, int)

#define LED_SET_STATUS_OFF	0x00
#define LED_SET_STATUS_ORANGE	0x01
#define LED_SET_STATUS_YELLOW	0x02

static void platform_information(void)
{
	printk("Status LED Driver Code for MB-7565...\n");
}

static void enter_w83627_config(void)
{
	outb_p(0x87, INDEX_PORT); // must be called twice
	outb_p(0x87, INDEX_PORT);
	return;
}

static void exit_w83627_config(void)
{
	outb_p(0xAA, INDEX_PORT);
	return;
}

static unsigned char read_w83627_reg(int LDN, int reg)
{
	unsigned char tmp = 0;

	enter_w83627_config();
	outb_p(0x07, INDEX_PORT);
	outb_p(LDN, DATA_PORT);
	outb_p(reg, INDEX_PORT);
	tmp = inb_p(DATA_PORT);
	exit_w83627_config();

	return tmp;
}

static void write_w83627_reg(int LDN, int reg, int value)
{
	enter_w83627_config();
	outb_p(0x07, INDEX_PORT);
	outb_p(LDN, DATA_PORT);
	outb_p(reg, INDEX_PORT);
	outb_p(value, DATA_PORT);
	exit_w83627_config();
}

/* Platform specific end */
/*********************************************/

static void led_init(void)
{
	write_w83627_reg(0x7, 0x30, read_w83627_reg(0x7, 0x30) | 0x08);	//rx30[3]=1
	write_w83627_reg(0x7, 0xF3, read_w83627_reg(0x7, 0xF3) & 0xFC); //rxF3[1,0]=00
	write_w83627_reg(0x7, 0xF5, read_w83627_reg(0x7, 0xF5) & 0xFC); //rxF5[1,0]=00
	write_w83627_reg(0x7, 0xF4, read_w83627_reg(0x7, 0xF4) | 0x03); //rxF1[1,0]=00 default hi
}

/*
 * This function is called whenever a process attempts to
 * open the device file
 */
static int sled_open(struct inode * inode, struct file * file)
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
	//printk("Lanner Status LED Driver Opened\n");
	return 0;
}

/*
 * This function is called when a process closes the device file.
 */
static int sled_release(struct inode * inode, struct file * file)
{
	/* ready for next caller */
	__sync_add_and_fetch(&device_open.counter, 1);

	/*
	 * Decrement the usage count, otherwise once you opened the file
	 * you'll never get rid of the module.
	 */
	//printk("Lanner Status LED Driver Closed\n");
	return 0;
}

static void set_led_off(void)
{
	write_w83627_reg(0x07, 0xF4, read_w83627_reg(0x07, 0xF4) | 0x03); //rxF1[1,0]=11
}

static void set_led_orange(void)
{
	write_w83627_reg(0x07, 0xF4, (read_w83627_reg(0x07, 0xF4) & 0xFC) | 0x02); //rxF1[0]=0
}

static void set_led_yellow(void)
{
	write_w83627_reg(0x07, 0xF4, (read_w83627_reg(0x07, 0xF4) & 0xFC) | 0x01); //rxF1[1]=0
}

static long sled_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	switch (cmd) {
	case IOCTL_LED_SET_STATUS:
		switch (arg) {
		case LED_SET_STATUS_OFF:
			set_led_off();
			break;
		case LED_SET_STATUS_ORANGE:
			set_led_orange();
			break;
		case LED_SET_STATUS_YELLOW:
			set_led_yellow();
			break;
		default:
			return -EOPNOTSUPP;
		}
		break;
	default:
		return -EOPNOTSUPP;
	}

	return 0;
}

static const struct file_operations sled_fops = {
	.owner = THIS_MODULE,
	.unlocked_ioctl = sled_ioctl,
	.compat_ioctl = sled_ioctl,
	.open = sled_open,
	.release = sled_release,
};

static struct miscdevice sled_miscdev = {
	.fops = &sled_fops,
	.nodename = "sled_drv",
	.minor = MISC_DYNAMIC_MINOR,
};

static int sled_init(void)
{
	int result;

	/*
	 * Register the character device
	 */

	result = misc_register(&sled_miscdev);

	if (result < 0)
	{
		printk("sled : unable to register, err %d\n", result);
		return -EIO;
	}
	platform_information();
	led_init();
	printk("Status LED Driver Version %s is loaded\n", driver_version);
	return 0;
}

/*
 * Cleanup - unregister the appropriate file from /proc
 */
static void sled_exit(void)
{
	/* Unregister the device */
	misc_deregister(&sled_miscdev);
	/* If there's an error, report it */
	printk("Status LED Driver Version %s is unloaded\n", driver_version);
}

module_init(sled_init);
module_exit(sled_exit);

MODULE_AUTHOR("Lanner SW");
MODULE_DESCRIPTION("Lanner Status LEDs Linux Driver");
MODULE_LICENSE("Dual BSD/GPL");
