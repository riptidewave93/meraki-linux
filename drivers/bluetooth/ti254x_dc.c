/*
 * drivers/bluetooth/ti254x_dc.c
 *
 * Debug Communications Support for TI CC2540 and CC2541 chips
 *
 * Copyright (C) 2014 Cisco, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/atomic.h>
#include <linux/firmware.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <net/bluetooth/bluetooth.h>
#include "ti254x_dc.h"

#define TI_BLOCK_SIZE     (32 * 1024)
#define TI_MAX_XBANK_ADDR 0xFFFF
#define TI_FLASH_BANK     0x8000
#define CLK_DELAY         1
#define CMD_UDELAY        16
#define ERASE_MDELAY      20
#define RETRY_COUNT       300

#define LOBYTE(w) ((u8)(w))
#define HIBYTE(w) ((u8)(((u16)(w) >> 8) & 0xFF))
#define XREG_TO_INT(a) (a)

/* 0x4000(16k) << CHIPINFO_FLASH_X */
#define CHIPINFO_FLASH_SIZE(x) (0x4000 << ((x & 0x70) >> 4))
#define CHIPINFO_FLASH_32       0x1
#define CHIPINFO_FLASH_64       0x2
#define CHIPINFO_FLASH_128      0x3
#define CHIPINFO_FLASH_256      0x4

#define TI_SFR       0x7080
#define TI_MEMCTL    0x70c7
#define TI_CHIPINFO0 0x6276
#define CHIPINFO0    0x6276
#define FADDRL       0x6271
#define FADDRH       0x6272
#define FWDATA       0x6273
#define DBGDATA      0x6260
#define FCTL         0x6270

#define FCTL_BUSY  0x80
#define FCTL_FULL  0x40
#define FCTL_ABRT  0x20
#define FCTL_CACHE 0x04
#define FCTL_WRITE 0x02

#define X_DMA1CFGL  0x70d2
#define X_DMA1CFGH  0x70d3
#define X_DMA0CFGL  0x70d4
#define X_DMA0CFGH  0x70d5
#define X_DMAARM    0x70d6

/* BUFFERS */
#define ADDR_BUF0         0x0000
#define ADDR_DMA_DESC0    0x0400

/* COMMANDS */
#define DC_CHIP_ERASE       0x10
#define DC_WR_CONFIG        0x18
#define DC_RD_CONFIG        0x20
#define DC_GET_PC           0x28
#define DC_READ_STATUS      0x30
#define DC_RESUME           0x48
#define DC_DEBUG_INSTR      0x50
#define DC_GET_CHIP_ID      0x68
#define DC_CMD_BURST_WRITE  0x80

const u8 dma_desc[32] = {
	/* channel 1 */
	HIBYTE(XREG_TO_INT(DBGDATA)),   /* src[15:8]  */
	LOBYTE(XREG_TO_INT(DBGDATA)),   /* src[7:0]   */
	HIBYTE(ADDR_BUF0),              /* dest[15:8] */
	LOBYTE(ADDR_BUF0),              /* dest[7:0]  */
	HIBYTE(TI254X_DMA_BLOCK_SIZE),
	LOBYTE(TI254X_DMA_BLOCK_SIZE),
	31,
	0x11,
	/* channel 2 */
	HIBYTE(XREG_TO_INT(ADDR_BUF0)),   /* src[15:8]  */
	LOBYTE(XREG_TO_INT(ADDR_BUF0)),   /* src[7:0]   */
	HIBYTE(FWDATA),                   /* dest[15:8] */
	LOBYTE(FWDATA),                   /* dest[7:0]  */
	HIBYTE(TI254X_DMA_BLOCK_SIZE),
	LOBYTE(TI254X_DMA_BLOCK_SIZE),
	18,
	0x42,
};

static void setval(unsigned int pin, unsigned int value)
{
	gpio_set_value(pin, value);
	udelay(CLK_DELAY);
}

static void sethi(unsigned int pin)
{
	setval(pin,1);
}

static void setlo(unsigned int pin)
{
	setval(pin,0);
}

static void write_byte(struct ti254x_dc_dev *dc_dev, u8 byte)
{
	int i;
	int value;
	unsigned long flags;

	local_irq_save(flags);
	for (i = 7; i >= 0; i--) {
		value = ((byte >> i) & 0x1);
		gpio_set_value(dc_dev->scl_pin, 1);
		gpio_set_value(dc_dev->sda_pin, value);
		udelay(CLK_DELAY);
		gpio_set_value(dc_dev->scl_pin, 0);
		udelay(CLK_DELAY);
	}
	local_irq_restore(flags);
}

static u8 read_byte(struct ti254x_dc_dev *dc_dev)
{
	int i;
	u8 value = 0;
	unsigned long flags;

	local_irq_save(flags);
	for (i = 0; i < 8; i++) {
		sethi(dc_dev->scl_pin);
		udelay(1);
		value *= 2;
		if (gpio_get_value(dc_dev->sda_pin))
			value |= 0x1;

		setlo(dc_dev->scl_pin);
	}
	local_irq_restore(flags);
	return value;
}

static int wait_for_response(struct ti254x_dc_dev *dc_dev)
{
	int i;
	int trycount = 0;
	int retval = 0;
	unsigned long flags;


	while (gpio_get_value(dc_dev->sda_pin) && trycount < RETRY_COUNT) {
		local_irq_save(flags);
		/*  clock 8 cycles */
		for (i = 0; i < 8; i++) {
			sethi(dc_dev->scl_pin);
			setlo(dc_dev->scl_pin);
		}
		local_irq_restore(flags);

		if (trycount != 0) {
		       dc_dev->retries += 1;
		}
		trycount++;
		udelay(CMD_UDELAY);
	}

	if (trycount == RETRY_COUNT)
		retval = -ETIMEDOUT;

	return retval;
}

/**
 * @brief ti254x_dc_command
 *
 * @param dev device structure
 * @param cmd command value
 * @param p parameter array
 * @param num_p number of parameters
 * @param r result array
 * @param num_r number of results expected
 *
 * @return debug status
 */
static int ti254x_dc_command(struct ti254x_dc_dev *dc_dev,
		u8 cmd, u8 *p, int num_p,
		u8 *r, int num_r)
{
	int i;
	int status = 0;

	dc_dev->commands += 1;

	gpio_direction_output(dc_dev->sda_pin, 0);
	write_byte(dc_dev, cmd | num_p);
	for (i = 0; i < num_p; i++) {
		udelay(CMD_UDELAY);
		write_byte(dc_dev, p[i]);
	}

	if (num_r) {
		gpio_direction_input(dc_dev->sda_pin);
		udelay(CMD_UDELAY);
		status = wait_for_response(dc_dev);
		if (status != -ETIMEDOUT) {
			for (i = 0; i < num_r; i++) {
				r[i] = read_byte(dc_dev);
				udelay(CMD_UDELAY);
			}
		} else {
			BT_ERR("ti254x ti254x_dc_command: timeout\n");
		}
		gpio_direction_output(dc_dev->sda_pin, 1);
		setlo(dc_dev->sda_pin);
	}

	return status;
}

static void ti254x_dc_write_xdata_byte(struct ti254x_dc_dev *dc_dev,
		u16 address, u8 value)
{
	u8 instr[3];
	u8 accum;

	/* MOV DPTR, address */
	instr[0] = 0x90;
	instr[1] = HIBYTE(address);
	instr[2] = LOBYTE(address);
	ti254x_dc_command(dc_dev, DC_DEBUG_INSTR, instr, 3, &accum, 1);

	/* MOV A, values[i] */
	instr[0] = 0x74;
	instr[1] = value;
	ti254x_dc_command(dc_dev, DC_DEBUG_INSTR, instr, 2, &accum, 1);

	/* MOV @DPTR, A */
	instr[0] = 0xF0;
	ti254x_dc_command(dc_dev, DC_DEBUG_INSTR, instr, 1, &accum, 1);
}

static u8 ti254x_dc_read_xdata_byte(struct ti254x_dc_dev *dc_dev,
		u16 address)
{
	u8 instr[3];
	u8 accum;

	/* MOV DPTR, address */
	instr[0] = 0x90;
	instr[1] = HIBYTE(address);
	instr[2] = LOBYTE(address);
	ti254x_dc_command(dc_dev, DC_DEBUG_INSTR, instr, 3, &accum, 1);

	/* MOV A, @DPTR */
	instr[0] = 0xE0;
	ti254x_dc_command(dc_dev, DC_DEBUG_INSTR, instr, 1, &accum, 1);

	return accum;
}

static int while_fctl_set(struct ti254x_dc_dev *dc_dev, u8 flag, u8 retry)
{
	u8 count;
	u8 fctl;
	fctl = ti254x_dc_read_xdata_byte(dc_dev, XREG_TO_INT(FCTL));
	for (count = retry; count > 0 && (fctl & flag); count--)
		fctl = ti254x_dc_read_xdata_byte(dc_dev, XREG_TO_INT(FCTL));

	if (!count)
		return -ETIMEDOUT;

	return 0;
}

static void ti254x_dc_set_dptr(struct ti254x_dc_dev *dc_dev, u16 address)
{
	u8 accum;
	u8 instr[3];

	/* MOV DPTR, address */
	instr[0] = 0x90;
	instr[1] = HIBYTE(address);
	instr[2] = LOBYTE(address);
	ti254x_dc_command(dc_dev, DC_DEBUG_INSTR, instr, 3, &accum, 1);
}

static u8 ti254x_dc_read_dptr(struct ti254x_dc_dev *dc_dev)
{
	u8 accum;
	u8 instr[3];

	/* MOV A, @DPTR */
	instr[0] = 0xE0;
	ti254x_dc_command(dc_dev, DC_DEBUG_INSTR, instr, 1, &accum, 1);
	return accum;
}

static void ti254x_dc_inc_dptr(struct ti254x_dc_dev *dc_dev)
{
	u8 accum;
	u8 instr[3];

	/*INC DPTR*/
	instr[0] = 0xA3;
	ti254x_dc_command(dc_dev, DC_DEBUG_INSTR, instr, 1, &accum, 1);
}

static size_t ti254x_dc_read_xdata_block(struct ti254x_dc_dev *dc_dev,
				u16 address, u8 *data, size_t size) {
	ssize_t i;
	size_t bytes_read = 0;

	ti254x_dc_set_dptr(dc_dev, address);
	for (i = 0; i < size; i++) {
		if ((u32)(address + i) > TI_MAX_XBANK_ADDR) {
			break;
		} else {
			data[i] = ti254x_dc_read_dptr(dc_dev);
			ti254x_dc_inc_dptr(dc_dev);
			++bytes_read;
		}
	}

	return bytes_read;
}

static void ti254x_dc_write_xdata_block(struct ti254x_dc_dev *dc_dev,
				u16 address, u8 const *data, u16 size)
{
	int i;
	u8 accum;
	u8 instr[3];

	ti254x_dc_set_dptr(dc_dev, address);
	for (i = 0; i < size; i++) {
		/*MOV A, values[i]*/
		instr[0] = 0x74;
		instr[1] = data[i];
		ti254x_dc_command(dc_dev, DC_DEBUG_INSTR, instr, 2, &accum, 1);

		/*MOV @DPTR, A*/
		instr[0] = 0xF0;
		ti254x_dc_command(dc_dev, DC_DEBUG_INSTR, instr, 1, &accum, 1);
		ti254x_dc_inc_dptr(dc_dev);
	}
}

static void ti254x_dc_addr_to_bank(loff_t address, u8 *block, u16 *offset)
{
	*block = address / TI_BLOCK_SIZE;
	*offset = address % TI_BLOCK_SIZE;
}

static u16 burst_write(struct ti254x_dc_dev *dc_dev, u8 *data,
		       ssize_t size, u8 *status)
{
	ssize_t i;

	gpio_direction_output(dc_dev->sda_pin, 1);
	write_byte(dc_dev, DC_CMD_BURST_WRITE | HIBYTE(TI254X_DMA_BLOCK_SIZE));
	write_byte(dc_dev, LOBYTE(TI254X_DMA_BLOCK_SIZE));
	for (i = 0; i < size; i++) {
		udelay(4*CLK_DELAY);
		write_byte(dc_dev, data[i]);
	}

	/*Flash writes occur in blocks of TI254X_DMA_BLOCK_SIZE bytes. To keep
	 * garbage values in sram from being written to flash fill sram with
	 * 1s.*/
	if (size < TI254X_DMA_BLOCK_SIZE) {
		for (i = 0; i < (TI254X_DMA_BLOCK_SIZE - size); i++)
			write_byte(dc_dev, 0xff);
	}


	sethi(dc_dev->sda_pin);
	gpio_direction_input(dc_dev->sda_pin);
	if ((wait_for_response(dc_dev)) != -ETIMEDOUT)
		*status = read_byte(dc_dev);

	gpio_direction_output(dc_dev->sda_pin, 1);
	setlo(dc_dev->sda_pin);

	return size;
}

static void ti254x_dc_init_dma(struct ti254x_dc_dev *dc_dev)
{
	u8 instr[3];
	u8 debug_status;

	instr[0] = 0x22;
	ti254x_dc_command(dc_dev, DC_WR_CONFIG, instr, 1, &debug_status, 1);

	/* write the 4 DMA descriptors */
	ti254x_dc_write_xdata_block(dc_dev, ADDR_DMA_DESC0, dma_desc, 32);

	/* set the pointer to the DMA descriptors */
	ti254x_dc_write_xdata_byte(dc_dev, XREG_TO_INT(X_DMA1CFGH),
			HIBYTE(ADDR_DMA_DESC0));
	ti254x_dc_write_xdata_byte(dc_dev, XREG_TO_INT(X_DMA1CFGL),
			LOBYTE(ADDR_DMA_DESC0));
}





/*****************************************************************************
 * PUBLIC API
 *****************************************************************************/

int ti254x_dc_read_status(struct ti254x_dc_dev *dc_dev, u8 *status)
{
	int retval;
	retval = ti254x_dc_command(dc_dev, DC_READ_STATUS, NULL, 0, status, 1);
	return retval;
}

int ti254x_dc_read_chip_id(struct ti254x_dc_dev *dc_dev, u8 *id)
{
	int retval;
	retval = ti254x_dc_command(dc_dev, DC_GET_CHIP_ID, NULL, 0, id, 2);
	return retval;
}

void ti254x_dc_enter_debug_mode(struct ti254x_dc_dev *dc_dev)
{
	unsigned long flags;
	local_irq_save(flags);

	gpio_direction_output(dc_dev->sda_pin, 0);
	gpio_direction_output(dc_dev->scl_pin, 0);
	gpio_direction_output(dc_dev->reset_pin, !(dc_dev->reset_active_level));
	udelay(CMD_UDELAY);

	/* reset active */
	setval(dc_dev->reset_pin, dc_dev->reset_active_level);
	udelay(CMD_UDELAY);

	/* two clock flanks */
	sethi(dc_dev->scl_pin);
	setlo(dc_dev->scl_pin);

	sethi(dc_dev->scl_pin);
	setlo(dc_dev->scl_pin);

	/* reset clear */
	udelay(CMD_UDELAY);
	setval(dc_dev->reset_pin, !(dc_dev->reset_active_level));
	udelay(CMD_UDELAY);

	local_irq_restore(flags);
}

int ti254x_dc_init(struct ti254x_dc_dev *dc_dev, u8 *debug_status)
{
	int status = 0;

	ti254x_dc_enter_debug_mode(dc_dev);

	status = ti254x_dc_read_status(dc_dev, debug_status);

	if (status != -ETIMEDOUT) {
		dc_dev->chipinfo = ti254x_dc_read_xdata_byte(dc_dev, CHIPINFO0);
		dc_dev->flash_size = CHIPINFO_FLASH_SIZE(dc_dev->chipinfo);
	}

	return status;
}


int ti254x_dc_chip_erase(struct ti254x_dc_dev *dc_dev, u8 *status)
{
	int retval;

	ti254x_dc_enter_debug_mode(dc_dev);
	retval = ti254x_dc_command(dc_dev, DC_CHIP_ERASE, NULL, 0, status, 1);

	/* Wait 20 ms for mass erase to complete */
	msleep(ERASE_MDELAY);

	while (*status & TI254X_DC_CHIP_ERASE_BUSY && retval != -ETIMEDOUT) {
		retval |= ti254x_dc_command(dc_dev, DC_READ_STATUS,
					NULL, 0, status, 1);
	}
	return retval;
}

int ti254x_dc_flash_write(struct ti254x_dc_dev *dc_dev,
		const u8 *data,
		size_t offset,
		size_t size)
{
	u16 i;
	u8 *buf_ptr;
	ssize_t bytes_left;
	size_t bytes_written;
	ssize_t block_size;
	u16 num_buffers = size / TI254X_DMA_BLOCK_SIZE;
	u8 debug_status;
	const u16 faddr = ((offset >> 2) & 0xffff);

	ti254x_dc_init_dma(dc_dev);
	ti254x_dc_write_xdata_byte(dc_dev, XREG_TO_INT(FADDRH), HIBYTE(faddr));
	ti254x_dc_write_xdata_byte(dc_dev, XREG_TO_INT(FADDRL), LOBYTE(faddr));

	for (i = 0; i <= num_buffers; i++) {
		buf_ptr = (u8 *)data + (i * TI254X_DMA_BLOCK_SIZE);
		bytes_written = (buf_ptr - data);

		bytes_left = (size - bytes_written);
		if (bytes_left <= 0)
			break;

		block_size = min_t(ssize_t, TI254X_DMA_BLOCK_SIZE, bytes_left);


		/* arm DMA channel 1 */
		ti254x_dc_write_xdata_byte(dc_dev, XREG_TO_INT(X_DMAARM), 0x2);
		burst_write(dc_dev, buf_ptr, block_size, &debug_status);

		/* wait for write to finish */
		if (while_fctl_set(dc_dev, FCTL_BUSY, RETRY_COUNT))
			return -ETIMEDOUT;

		/* arm DMA channel 2 */
		ti254x_dc_write_xdata_byte(dc_dev, XREG_TO_INT(X_DMAARM), 0x4);

		/* trigger flash write */
		ti254x_dc_write_xdata_byte(dc_dev, XREG_TO_INT(FCTL), 0x06);

		if (while_fctl_set(dc_dev, FCTL_BUSY, RETRY_COUNT))
			return -ETIMEDOUT;
	}

	if (while_fctl_set(dc_dev, FCTL_BUSY, RETRY_COUNT))
		return -ETIMEDOUT;

	return 0;
}

u8 ti254x_dc_resume(struct ti254x_dc_dev *dc_dev)
{
	u8 status;
	ti254x_dc_command(dc_dev, DC_RESUME, NULL, 0, &status, 1);

	return status;
}

void ti254x_dc_reset(struct ti254x_dc_dev *dc_dev)
{
	unsigned long flags;

	local_irq_save(flags);
	setval(dc_dev->reset_pin, dc_dev->reset_active_level);
	udelay(CLK_DELAY);
	setval(dc_dev->reset_pin, !(dc_dev->reset_active_level));
	local_irq_restore(flags);
}

ssize_t ti254x_dc_flash_read(struct ti254x_dc_dev *dc_dev,
		char *buf, loff_t off, size_t size)
{
	u8 flash_block;
	u8 curr_flash_block;
	u16 flash_address;

	loff_t curr_flat_address = off;
	loff_t flat_address_end = off + size;
	ssize_t bytes_read = 0;
	ssize_t total_bytes_read = 0;

	ti254x_dc_addr_to_bank(curr_flat_address, &flash_block, &flash_address);
	ti254x_dc_write_xdata_byte(dc_dev, TI_MEMCTL, flash_block);
	curr_flash_block = flash_block;

	/* Flash is read through the XDATA region. This means flash has to be
	 * mapped into the XDATA region in 32k chunks */
	for (curr_flat_address = off; curr_flat_address < flat_address_end;) {
		ti254x_dc_addr_to_bank(curr_flat_address,
				&flash_block, &flash_address);

		if (flash_block != curr_flash_block) {
			ti254x_dc_write_xdata_byte(dc_dev,
					TI_MEMCTL, flash_block);
			curr_flash_block = flash_block;
		}

		bytes_read = ti254x_dc_read_xdata_block(dc_dev,
				flash_address + TI_FLASH_BANK,
				buf, size - total_bytes_read);
		buf += bytes_read;
		total_bytes_read += bytes_read;
		curr_flat_address += bytes_read;
		schedule();
	}

	return total_bytes_read;
}

int ti254x_dc_lock(struct ti254x_dc_dev *dc_dev)
{
	return atomic_dec_and_test(&dc_dev->lock);
}

void ti254x_dc_unlock(struct ti254x_dc_dev *dc_dev)
{
	atomic_inc(&dc_dev->lock);
}









