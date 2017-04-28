/*
 * arch/arm/plat-ambarella/misc/uport_switcher.c
 *
 * Author: Cao Rongrong <rrcao@ambarella.com>
 *
 * Copyright (C) 2012-2016, Ambarella, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 */

/*
 * Note: only S2 chip can switch usb port between host and device.
 */

#include <asm/uaccess.h>
#include <linux/io.h>
#include <linux/interrupt.h>
#include <linux/proc_fs.h>
#include <mach/hardware.h>
#include <mach/board.h>

/* check RCT_REG(0x88), but not SYS_CONFIG_REG */
#define USB_PORT_IS_HOST	(0x1 << 5)

#define USB_DETECT_DEVICE_NORMAL	0
#define USB_DETECT_DEVICE_OTG		1

#define USB_PORT_NO_CONTROL		-1
#define USB_PORT_TO_DEVICE		0
#define USB_PORT_TO_HOST		1


/* usb_detect_otg_flag is used to specify the kind of
 * device plugged into the device port */
static int usb_detect_otg_flag = USB_DETECT_DEVICE_NORMAL;
/* usb_port_control is used to route D+/D- signal to
 * device port or host port */
static int usb_port_control = USB_PORT_NO_CONTROL;

static inline void ambarella_switch_to_host(void)
{
	amba_setbitsl(USB_REFCLK_REG, 0x1<<5);
}

static inline void ambarella_switch_to_device(void)
{
	amba_clrbitsl(USB_REFCLK_REG, 0x1<<5);
}

static irqreturn_t ambarella_otg_detect_irq(int irq, void *dev_id)
{
	int otg_gpio_val;

	otg_gpio_val = ambarella_gpio_get(ambarella_board_generic.uport_irq.irq_gpio);

	if (otg_gpio_val == ambarella_board_generic.uport_irq.irq_gpio_val)
		usb_detect_otg_flag = USB_DETECT_DEVICE_OTG;
	else
		usb_detect_otg_flag = USB_DETECT_DEVICE_NORMAL;

	if (usb_port_control == USB_PORT_TO_DEVICE) {
		if (usb_detect_otg_flag == USB_DETECT_DEVICE_OTG) {
			ambarella_switch_to_host();
		} else {
			ambarella_switch_to_device();
		}
	}

	return IRQ_HANDLED;
}

static int ambarella_uport_switcher_proc_read(char *page, char **start,
	off_t off, int count, int *eof, void *data)
{
	int len = 0;
	const char *port_status, *controller_status;

	if (off != 0)
		return 0;

	if (amba_rct_readl(USB_REFCLK_REG) & USB_PORT_IS_HOST)
		controller_status = "HOST";
	else
		controller_status = "DEVICE";

	if (usb_port_control == USB_PORT_NO_CONTROL)
		port_status = controller_status;
	else if (usb_port_control == USB_PORT_TO_HOST)
		port_status = "HOST";
	else
		port_status = "DEVICE";

	len = scnprintf(page, count, "Possible parameter: host device\n");
	len += scnprintf((page + len), (count - len), "Current status:\n");
	len += scnprintf((page + len), (count - len),
		"\tport is %s: controller is %s\n\n", port_status, controller_status);

	*eof = 1;

	return len;
}

static int ambarella_uport_switcher_proc_write(struct file *file,
	const char __user *buffer, unsigned long count, void *data)
{
	int n;
	char str[32];

	n = (count < 32) ? count : 32;

	if (copy_from_user(str, buffer, n)) {
		pr_err("%s: copy_from_user fail!\n", __func__);
		return -EFAULT;
	}

	str[n - 1] = 0;

	if (!strcmp(str, "host")) {
		if (ambarella_board_generic.uport_control.gpio_id >= 0) {
			usb_port_control = USB_PORT_TO_HOST;
			ambarella_set_gpio_output(&ambarella_board_generic.uport_control, 1);
		}
		ambarella_switch_to_host();
	} else if (!strcmp(str, "device")) {
		/* if otg device is detected, we still switch to host mode */
		if (usb_detect_otg_flag == USB_DETECT_DEVICE_OTG)
			ambarella_switch_to_host();
		else
			ambarella_switch_to_device();
		if (ambarella_board_generic.uport_control.gpio_id >= 0) {
			usb_port_control = USB_PORT_TO_DEVICE;
			ambarella_set_gpio_output(&ambarella_board_generic.uport_control, 0);
		}
	} else {
		pr_err("Invalid argument!\n");
	}

	return count;
}

static int __init ambarella_init_uport_switcher(void)
{
	int rval = 0, otg_gpio_val = -1;
	struct proc_dir_entry *proc_file;

	if (ambarella_is_valid_gpio_irq(&ambarella_board_generic.uport_irq)) {
		ambarella_gpio_config(ambarella_board_generic.uport_irq.irq_gpio,
			GPIO_FUNC_SW_INPUT);
		otg_gpio_val = ambarella_gpio_get(
			ambarella_board_generic.uport_irq.irq_gpio);
		if (otg_gpio_val == ambarella_board_generic.uport_irq.irq_gpio_val)
			usb_detect_otg_flag = USB_DETECT_DEVICE_OTG;

		rval = request_irq(ambarella_board_generic.uport_irq.irq_line,
				ambarella_otg_detect_irq,
				ambarella_board_generic.uport_irq.irq_type,
				"usb_detect_otg", NULL);
		if (rval) {
			pr_err("%s: request usb_detec_org irq failed\n", __func__);
			goto uport_switcher_err1;
		}

		/* allow to wake up system when sleeping */
		rval = enable_irq_wake(ambarella_board_generic.uport_irq.irq_line);
		if (rval) {
			pr_err("%s : enable_irq_wake(%d) = %d\n", __func__,
				ambarella_board_generic.uport_irq.irq_line, rval);
			goto uport_switcher_err2;
		}
	}

	if (ambarella_board_generic.uport_control.gpio_id >= 0) {
		rval = gpio_request(ambarella_board_generic.uport_control.gpio_id,
				"uport_switcher");
		if (rval) {
			pr_err("%s: request uport_switcher gpio failed\n", __func__);
			goto uport_switcher_err2;
		}
		if (amba_readl(USB_REFCLK_REG) & USB_PORT_IS_HOST) {
			/* route D+/D- signal to host port */
			usb_port_control = USB_PORT_TO_HOST;
			ambarella_set_gpio_output(&ambarella_board_generic.uport_control, 1);
		} else {
			/* route D+/D- signal to device port */
			usb_port_control = USB_PORT_TO_DEVICE;
			ambarella_set_gpio_output(&ambarella_board_generic.uport_control, 0);
		}
	}

	if (usb_port_control == USB_PORT_TO_DEVICE) {
		/* if otg device is detected, we switch to host mode */
		if (usb_detect_otg_flag == USB_DETECT_DEVICE_OTG) {
			ambarella_switch_to_host();
		}
	}

	proc_file = create_proc_entry("uport", S_IRUGO|S_IWUSR, get_ambarella_proc_dir());
	if (proc_file == NULL) {
		rval = -ENOMEM;
		pr_err("%s: create uport proc file failed!\n", __func__);
		goto uport_switcher_err3;
	} else {
		proc_file->read_proc = ambarella_uport_switcher_proc_read;
		proc_file->write_proc = ambarella_uport_switcher_proc_write;
	}

	return 0;

uport_switcher_err3:
	gpio_free(ambarella_board_generic.uport_control.gpio_id);
uport_switcher_err2:
	free_irq(ambarella_board_generic.uport_irq.irq_line, NULL);
uport_switcher_err1:
	return rval;
}

late_initcall(ambarella_init_uport_switcher);

