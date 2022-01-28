/* cisco-act-lpc.c - a device driver for the Cisco ACT2 chip  */
/* ------------------------------------------------------------------------- */
/* Copyright (C) 2016 Cisco Systems, Inc.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.                */
/* ------------------------------------------------------------------------- */

/* Initial driver commit by Hart Thomson <hthomson@meraki.com> */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/ioport.h>
#include <linux/io.h>
#include <linux/meraki/cisco-act.h>

/*
 *  Set the flag CISCO_ACT_LPC_DEBUG_MESSAGES to enable debug printk messages
 *
 */
//#define CISCO_ACT_LPC_DEBUG_MESSAGES	1

#define MSG_PREAMBLE_BYTES	4

/*
 * This is an LPC driver specific minimum write value implemented as a
 * workaround to some TAM library errors. This value was recommended by
 * Jimmy Chou (Cisco STO).
 */
#define LPC_MIN_WRITE_BUFFER 0x700

static void __iomem *mem_iobase;

/*
 * Translates an FPGA address to an LPC-bus address. Automatically
 * sets window registers if requesting an address from the TLA buffer
 * or the Mailbox DPRAM buffer.
 *
 */
/* Use get_drv_data to derive iobase, not a static var */
static u32 tla_window = 0xffff;
static u32 sram_window = 0xffff;
static int lpc_translate_addr(u16 fpga_addr, void** __iomem lpc_addr)
{
	int status = 0;
	void __iomem* addr;
	u32 lpc_offset = 0x0;
	u32 window;

	if (lpc_addr == NULL) {
		return -1;
	}


	/* Secure Boot Registers */
	if (fpga_addr >= 0x0000 && fpga_addr < 0x0040) {
		lpc_offset = fpga_addr + 0x820;
	}
	/* TLA Status */
	else if (fpga_addr >= 0x00f0 && fpga_addr < 0x0100) {
		lpc_offset = fpga_addr - 0x00f0 + 0x0860;
	}
	/* Host IRQ Controller */
	else if (fpga_addr >= 0x1000 && fpga_addr < 0x1010) {
		lpc_offset = fpga_addr - 0x1000 + 0x0870;
	}
	/* TAM Resource Controller */
	else if (fpga_addr >= 0x2000 && fpga_addr < 0x203c) {
		lpc_offset = fpga_addr - 0x2000 + 0x0880;
	}
	/* TAM Mailbox Registers */
	else if (fpga_addr >= 0xc000 && fpga_addr < 0xc030) {
		lpc_offset = fpga_addr - 0xc000 + 0x08c0;
	}
	/* TLA Trace Buffer */
	else if (fpga_addr >= 0x0100 && fpga_addr < 0x0300) {
		window = (fpga_addr >> 4) & 0x1f;
		if (window != tla_window) {
			addr = (void __iomem*) ((u64)(mem_iobase));
			tla_window = window;
			iowrite32(window, addr);
		}

		lpc_offset = (fpga_addr & 0x000f) + 0x09f0;
	}
	/* TAM Mailbox DPRAM */
	else if (fpga_addr >= 0xe000 && fpga_addr < 0xe800) {
		window = (fpga_addr >> 9) & 0x3;
		if (window != sram_window) {
			addr = (void __iomem*) ((u64)(mem_iobase + 0x4));
			sram_window = window;
			iowrite32(window, addr);
		}

		lpc_offset = (fpga_addr & 0x01ff) + 0x0a00;
	}
	else {
		status = -1;
	}

	*lpc_addr = (void __iomem*) ((u64)(mem_iobase + lpc_offset - 0x800));

	return status;
}

/*
 *   The intention here is that users will issue a write/read when reading
 *   data from the TAM since a data address is required to be supplied by
 *   the user.
 *   act->writebuf is set in act_data_write_lpc
 *
 */
static ssize_t act_data_read_lpc(struct act_data *act,
				 char *buf,
				 unsigned offset,
				 size_t count )
{
	mutex_lock(&act->lock);
	memcpy(buf, act->writebuf, count);
	mutex_unlock(&act->lock);

	return count;
}

/*
 *	Anatomy of a TAM Mailbox Write:
 *	0x02 0x00 0xc0 0x00 0xde 0xad 0xbe 0xef
 *
 *	0x02 = hardcoded value, 0x02 indicates a write, 0x3 indicates read
 *	0x00 0x0c 0x00 = 12-bits of address, needs to be translated to 
 *			 LPC address space
 *	0xde 0xad 0xbe 0xef = n bytes of data
 */
static ssize_t act_data_write_lpc(struct act_data *act,
				  const char *buf,
				  unsigned offset,
				  size_t count )
{
#ifdef CISCO_ACT_LPC_DEBUG_MESSAGES
	int i;
#endif
	void __iomem* lpc_addr;
	u32* lpc_data;
	u8 direction;
	u32 mailbox_addr;
	int num_bytes;
	size_t bytes_transferred = 0;
	u8 addr_low_nibble;
	u32 word_temp;

	/* 4-byte header plus 1-byte of data minimum
	*/
	if (count < 5) {
		return -EBADMSG;
	}

	direction = *buf;
	mailbox_addr =  ( *(buf + 3) & 0xff ) |
			(( *(buf + 2) & 0xff ) << 8 |
			(( *(buf + 1) & 0xff ) << 16));

	/* subtract 4 from supplied number since we are stripping the 1 byte
	 * direction and 3 byte address
	 */
	num_bytes = (int)count - MSG_PREAMBLE_BYTES;

	/* Write */
	if (direction == MAILBOX_DIRECTION_WRITE) {
		lpc_data = (u32*)(buf + MSG_PREAMBLE_BYTES);
	}
	/* Read */
	else if (direction == MAILBOX_DIRECTION_READ) {
		lpc_data = (u32*)(act->writebuf + MSG_PREAMBLE_BYTES);
	}
	else {
		printk(KERN_ERR "Invalid Direction\n");
		return -EINVAL;
	}

#ifdef CISCO_ACT_LPC_DEBUG_MESSAGES
	printk("Package: ");
	for(i = 0; i < count; i++) {
		printk("0x%02x ", (u8)buf[i]);
	}
	printk("\n");
#endif

	mutex_lock(&act->lock);

	/* Support to read host registers. These are not stored on the device,
	   but simply in the driver's buffers. This is used to facility IAP
	   reset, but could be extended to support other features if required.
	*/
	if (mailbox_addr >= act->host_storage_addr_start &&
	    mailbox_addr + num_bytes <= act->host_storage_addr_end) {
		if (direction == MAILBOX_DIRECTION_READ) {
			memcpy(lpc_data, act->host_storage, num_bytes);
		}
		if (direction == MAILBOX_DIRECTION_WRITE) {
			memcpy(act->host_storage, lpc_data, num_bytes);
		}
	}
	else {
		/*
		 * Reads and Writes need to start on 4-byte boundaries.
		 *
		 */
		addr_low_nibble = mailbox_addr & 0x3;
		mailbox_addr = mailbox_addr & 0xfffc;
		if (addr_low_nibble)
		{
			/* Translate TAM address to LPC address and set window register
			 * if necessary
			*/
			if (lpc_translate_addr(mailbox_addr, &lpc_addr) < 0) {
				printk("Invalid Address special\n");
				return 0;
			}
			if (direction == MAILBOX_DIRECTION_WRITE) {
				word_temp = ioread32(lpc_addr) &
					( (1 << (8 * addr_low_nibble)) - 1);
				word_temp |= (*lpc_data) << (addr_low_nibble * 8);

	#ifdef CISCO_ACT_LPC_DEBUG_MESSAGES
				printk("lpc: send 0x%08x to 0x%08x\n",
						word_temp, lpc_addr);
	#endif
				iowrite32(word_temp, lpc_addr);
			}
			if (direction == MAILBOX_DIRECTION_READ) {
				word_temp = ioread32(lpc_addr);
				*lpc_data = word_temp >> (addr_low_nibble * 8);
	#ifdef CISCO_ACT_LPC_DEBUG_MESSAGES
				printk("lpc: recv 0x%08x from 0x%08x\n",
					word_temp, lpc_addr);
	#endif
			}

			mailbox_addr += 4;
			num_bytes -= (0x4 - addr_low_nibble);
			bytes_transferred = (0x4 - addr_low_nibble);
			lpc_data = (u32*)(( (u8*)lpc_data ) + bytes_transferred);
		}

		while (num_bytes > 0) {
			/* Translate TAM address to LPC address and set window register
			 * if necessary
			*/
			if (lpc_translate_addr(mailbox_addr, &lpc_addr) < 0) {
				printk("Invalid Address\n");
				break;
			}

			if (direction == MAILBOX_DIRECTION_WRITE) {
	#ifdef CISCO_ACT_LPC_DEBUG_MESSAGES
				printk("lpc: send 0x%08x to 0x%08x\n",
						*lpc_data, lpc_addr);
	#endif
				iowrite32(*lpc_data, lpc_addr);
			}
			if (direction == MAILBOX_DIRECTION_READ) {
				*lpc_data = ioread32(lpc_addr);
	#ifdef CISCO_ACT_LPC_DEBUG_MESSAGES
				printk("lpc: recv 0x%08x from 0x%08x\n",
						*lpc_data, lpc_addr);
	#endif
			}

			lpc_data++;
			num_bytes -= 4;
			mailbox_addr += 4;
			bytes_transferred += 4;
		}

	}



	mutex_unlock(&act->lock);
	return count;
}

static ssize_t act_reg_read_lpc(struct act_data *act,
				uint32_t reg,
				uint8_t* val,
				size_t count)
{
	ssize_t ret;
	uint8_t buf[8];

	if (count > 4)
		return 0;

	buf[0] = MAILBOX_DIRECTION_READ;
	buf[1] = (reg & 0xff0000) >> 16;
	buf[2] = (reg & 0x00ff00) >> 8;
	buf[3] = (reg & 0x0000ff);

	buf[4] = 0x0;
	buf[5] = 0x0;
	buf[6] = 0x0;
	buf[7] = 0x0;


	act_data_write_lpc(act, buf, 0, count + MSG_PREAMBLE_BYTES);
	ret = act_data_read_lpc(act, buf, 0, count + MSG_PREAMBLE_BYTES);

	/* Skip past 4 bytes
	*/
	memcpy(val, &buf[MSG_PREAMBLE_BYTES], count);

	return ret - MSG_PREAMBLE_BYTES;
}

static ssize_t act_reg_write_lpc(struct act_data *act,
				 uint32_t reg,
				 uint8_t* val,
				 size_t count)
{
	ssize_t ret;
	uint8_t buf[8];

	if (count > 4)
		return 0;

	buf[0] = MAILBOX_DIRECTION_WRITE;
	buf[1] = (reg & 0xff0000) >> 16;
	buf[2] = (reg & 0x00ff00) >> 8;
	buf[3] = (reg & 0x0000ff);

	memcpy(&buf[4], val, count);

	/* TODO: Write verify */

	ret = act_data_write_lpc(act, buf, 0, count + MSG_PREAMBLE_BYTES);
	return ret - MSG_PREAMBLE_BYTES;
}

static const struct cisco_act_bus_ops cisco_act_lpc_ops = {
	.read = act_data_read_lpc,
	.write = act_data_write_lpc,
	.reg_read = act_reg_read_lpc,
	.reg_write= act_reg_write_lpc,
};

static int cisco_act_platform_probe(struct platform_device *pdev)
{
	struct resource *mem_resource;
	struct act_data *act;
	struct cisco_act_platform_data* pdata;
	int err;

	pdata = (struct cisco_act_platform_data*)pdev->dev.platform_data;

	/* Initialize LPC functions here */
	mem_resource = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (mem_resource == NULL) {
		printk(KERN_ERR "cisco-act no memory resource defined\n");
		err = -ENOMEM;
		goto fail_init_lpc;
	}

	mem_iobase = devm_ioremap_resource(&pdev->dev, mem_resource);

	/* Common probe functions */
	act = cisco_act_common_probe(&pdev->dev, &cisco_act_lpc_ops);
	if (IS_ERR(act)) {
		err = PTR_ERR(act);
		goto fail_init_lpc;
	}

	act->bus_dev = pdev;

	return cisco_act_common_lateinit(&pdev->dev);

fail_init_lpc:
	return err;
}

struct device* find_act_device_by_name(const char* name)
{
	struct device *dev;

	dev = bus_find_device_by_name(&platform_bus_type,
				      NULL,
				      name);

	if (!dev)
	{
		return NULL;
	}

	return dev;
}
EXPORT_SYMBOL(find_act_device_by_name);

static struct platform_driver cisco_act_lpc_driver = {
	.driver = {
		.owner		= THIS_MODULE,
		.name		= "tam_ich",
	},
	.probe      = cisco_act_platform_probe,
};

module_platform_driver(cisco_act_lpc_driver);

MODULE_AUTHOR("Hart A. Thomson <hthomson@meraki.com>");
MODULE_DESCRIPTION("Device driver for ACT2");
MODULE_LICENSE("GPL");

