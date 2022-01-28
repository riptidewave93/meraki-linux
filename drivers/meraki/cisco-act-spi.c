/* cisco-act-spi.c - a device driver for the Cisco ACT2 chip  */
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
#include <linux/spi/spi.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/of_gpio.h>
#include <linux/input.h>
#include <asm/uaccess.h>
#include <linux/meraki/cisco-act.h>


#define MSG_PREAMBLE_BYTES	4

/* Default SPI Frequency set to 10 MHz */
#define DEFAULT_MAX_SPEED_HZ 125000

static u8* spi_tx_buf;
static u8* spi_rx_buf;

static const struct spi_device_id cisco_act_spi_ids[] = {
	{ "act-spi", 0 },
	{ /* END OF LIST */ }
};

MODULE_DEVICE_TABLE(spi, cisco_act_spi_ids);

static const struct of_device_id __maybe_unused cisco_act_dt_ids[] = {
	{ .compatible = "cisco,act-spi", },
	{ /* END OF LIST */ }
};
MODULE_DEVICE_TABLE(of, cisco_act_dt_ids);

static ssize_t act_spi_transfer(struct act_data *act,
				const char *in_buf,
				char *out_buf,
				size_t count )
{
	int status;
	u32 mailbox_addr;
	u8 addr_low_nibble;
	struct spi_device *spi = act->bus_dev;
	struct spi_message msg;
	struct spi_transfer xfer;
	size_t count_with_alignment;

	if (!spi) {
		dev_err(act->dev, "cisco-act-spi write failed to get device\n");
		return -ENODEV;
	}

	count_with_alignment = count;
	memset(&xfer, 0, sizeof(struct spi_transfer));
	xfer.bits_per_word = spi->bits_per_word;
	xfer.speed_hz = spi->max_speed_hz;
	xfer.rx_buf = spi_rx_buf;
	xfer.tx_buf = spi_tx_buf;

	/* The FPGA can only handle 4-byte aligned transactions */
	mailbox_addr =  ((u32)(in_buf[1]) << 16) |
			((u32)(in_buf[2]) << 8 ) |
			((u32)(in_buf[3]));
	addr_low_nibble = mailbox_addr & 0x3;
	mailbox_addr = mailbox_addr & ~0x3;

	mutex_lock(&act->lock);

	/* If we are given a transaction that starts at an unaligned address,
	   we first need to read the values on the device at the lower 4-byte
	   boundary, then write those values back at our 4-byte aligned
	   transaction. This is to ensure we don't inadvertently erase
	   any data
	*/
	if (addr_low_nibble) {
		if (in_buf[0] == MAILBOX_DIRECTION_READ) {
			count_with_alignment += (4 - addr_low_nibble);
		}
		else if (in_buf[0] == MAILBOX_DIRECTION_WRITE) {
			spi_tx_buf[0] = MAILBOX_DIRECTION_READ;
			spi_tx_buf[1] = (mailbox_addr & 0xff0000) >> 16;
			spi_tx_buf[2] = (mailbox_addr & 0x00ff00) >> 8;
			spi_tx_buf[3] = (mailbox_addr & 0x0000ff);

			xfer.len = 0x4 + MSG_PREAMBLE_BYTES;
			spi_message_init(&msg);
			spi_message_add_tail(&xfer, &msg);
			status = spi_sync(spi, &msg);

			memcpy( &spi_tx_buf[MSG_PREAMBLE_BYTES],
				&spi_rx_buf[MSG_PREAMBLE_BYTES],
				0x4);

			count_with_alignment += (4 - addr_low_nibble);
		}
	}

	/* Continue with a 4-byte aligned transaction */
	spi_tx_buf[0] = in_buf[0];
	spi_tx_buf[1] = (mailbox_addr & 0xff0000) >> 16;
	spi_tx_buf[2] = (mailbox_addr & 0x00ff00) >> 8;
	spi_tx_buf[3] = (mailbox_addr & 0x0000ff);
	memcpy( &spi_tx_buf[MSG_PREAMBLE_BYTES + addr_low_nibble],
		&in_buf[MSG_PREAMBLE_BYTES],
		count);

	xfer.len = count_with_alignment;
	/* Make sure we read multiples of 4 bytes, round up to the nearest
	   4-bytes
	*/
	xfer.len = (xfer.len + 3) & ~0x03;
	spi_message_init(&msg);
	spi_message_add_tail(&xfer, &msg);

	status = spi_sync(spi, &msg);
	memcpy( &out_buf[0],
		&spi_rx_buf[addr_low_nibble],
		count);

	mutex_unlock(&act->lock);

	if (status >= 0)
		return count;

	dev_err(act->dev, "spi_sync failed: %d\n", status);
	return -EIO;
}

static ssize_t act_data_read_spi(struct act_data *act,
				 char *buf,
				 unsigned offset,
				 size_t count )
{
	memcpy(buf, act->writebuf, count);
	return count;
}
static ssize_t act_data_write_spi(struct act_data *act,
				  const char *buf,
				  unsigned offset,
				  size_t count )
{
	return act_spi_transfer(act, buf, act->writebuf, count);
}


static ssize_t act_reg_read_spi(struct act_data *act,
				uint32_t reg,
				uint8_t* val,
				size_t count)
{
	/* Only 1-byte register reads are supported */
	int ret;
	uint8_t in_buf[MSG_PREAMBLE_BYTES + 1];
	uint8_t out_buf[MSG_PREAMBLE_BYTES + 1];

	in_buf[0] = MAILBOX_DIRECTION_READ;
	in_buf[1] = (reg & 0xff0000) >> 16;
	in_buf[2] = (reg & 0x00ff00) >> 8;
	in_buf[3] = (reg & 0x0000ff);

	ret = act_spi_transfer(act, in_buf, out_buf, MSG_PREAMBLE_BYTES + 1);
	if (ret < 0)
		return ret;

	/* Skip past 4 bytes to return the 5th byte
	*/
	*val = out_buf[MSG_PREAMBLE_BYTES];

	return ret - MSG_PREAMBLE_BYTES;
}

static ssize_t act_reg_write_spi(struct act_data *act,
				 uint32_t reg,
				 uint8_t* val,
				 size_t count)
{
	/* Only 1-byte register writes are supported */
	uint8_t in_buf[MSG_PREAMBLE_BYTES + 1];
	uint8_t out_buf[MSG_PREAMBLE_BYTES + 1];
	in_buf[0] = MAILBOX_DIRECTION_WRITE;
	in_buf[1] = (reg & 0xff0000) >> 16;
	in_buf[2] = (reg & 0x00ff00) >> 8;
	in_buf[3] = (reg & 0x0000ff);
	in_buf[4] = *val;

	return act_spi_transfer(act, in_buf, out_buf, MSG_PREAMBLE_BYTES + 1);
}

static const struct cisco_act_bus_ops cisco_act_spi_ops = {
	.bustype = BUS_SPI,
	.read = act_data_read_spi,
	.write = act_data_write_spi,
	.reg_read = act_reg_read_spi,
	.reg_write = act_reg_write_spi,
};

static int cisco_act_spi_probe(struct spi_device *spi)
{
	struct act_data *act;
	int err;

	err = of_property_read_u32(spi->dev.of_node,
				   "spi-max-frequency",
				   &spi->max_speed_hz);
	if (err < 0)
	{
		spi->max_speed_hz = DEFAULT_MAX_SPEED_HZ;
	}


	/* Initialize spi_device before common_probe
	*/
	spi->mode = SPI_MODE_0;
	spi->bits_per_word = 8;
	spi->chip_select = 0;
	err = spi_setup(spi);
	if (err) {
		goto fail_init_spi;
	}

	/* Common probe functions */
	act = cisco_act_common_probe(&spi->dev, &cisco_act_spi_ops);
	if (IS_ERR(act)) {
		err = PTR_ERR(act);
		goto fail_init_spi;
	}

	/* This buffer must be DMA-friendly. We add 4 bytes to this
	   to accomodate padding for enforcing 4-byte aligned
	   memory access
	 */
	spi_tx_buf = devm_kzalloc(&spi->dev, act->writebuf_size + 4,
				  GFP_KERNEL);
	if (!spi_tx_buf) {
		err = -ENOMEM;
		goto fail_init_spi;
	}

	spi_rx_buf = devm_kzalloc(&spi->dev, act->writebuf_size + 4,
				  GFP_KERNEL);
	if (!spi_rx_buf) {
		err = -ENOMEM;
		goto fail_init_spi;
	}

	act->bus_dev = spi;

        return cisco_act_common_lateinit(&spi->dev);

fail_init_spi:
	return err;
}

static int of_dev_node_match(struct device *dev, void *data)
{
	return dev->of_node == data;
}

struct device* of_find_act_device_by_node(struct device_node *node)
{
	struct device *dev;

	dev = bus_find_device(&spi_bus_type,
			      NULL,
			      node,
			      of_dev_node_match);

	if (!dev)
	{
		return NULL;
	}

	return dev;
}
EXPORT_SYMBOL(of_find_act_device_by_node);

static struct spi_driver cisco_act_spi_driver = {
	.driver = {
		.name       = "cisco-act-spi",
		.owner      = THIS_MODULE,
	},
	.probe      = cisco_act_spi_probe,
	.id_table   = cisco_act_spi_ids,
};

module_spi_driver(cisco_act_spi_driver);

MODULE_AUTHOR("Hart A. Thomson <hthomson@meraki.com>");
MODULE_DESCRIPTION("Device driver for ACT2");
MODULE_LICENSE("GPL");
