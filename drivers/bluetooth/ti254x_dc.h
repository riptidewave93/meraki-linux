/*
 * drivers/bluetooth/ti254x_dc.h
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
#ifndef _TI254X_DC_H_
#define _TI254X_DC_H_
#include <linux/module.h>

#define TI254X_FLASH_PAGE_SIZE  2048

/* FLASH DMA WRITE BLOCK SIZE */
#define TI254X_DMA_BLOCK_SIZE  1024

/* FLASH LOCK BYTES */
#define TI254X_BIT_LOCK_SIZE 16

/* LOCATION TO STORE THE HASH */
#define TI254X_HASH_OFFSET(flash_size) \
	(flash_size - (4 * TI254X_DMA_BLOCK_SIZE))


/* DEBUG STATUS */
#define TI254X_DC_CHIP_ERASE_BUSY   0x80
#define TI254X_DC_PCON_IDLE         0x40
#define TI254X_DC_CPU_HALTED        0x20
#define TI254X_DC_PM_ACTIVE         0x10
#define TI254X_DC_HALT_STATUS       0x8
#define TI254X_DC_DEBUG_LOCKED      0x4
#define TI254X_DC_OSCILLATOR_STABLE 0x2
#define TI254X_DC_STACK_OVERFLOW    0x1

#define TI254X_DC_DEBUG_MODE (TI254X_DC_CPU_HALTED | \
	TI254X_DC_PM_ACTIVE | TI254X_DC_DEBUG_LOCKED)

struct ti254x_dc_dev {
	unsigned int sda_pin;
	unsigned int scl_pin;
	unsigned int reset_pin;
	u8 reset_active_level;
	u8 chipinfo;
	u32 flash_size;
	const struct firmware *firmware;
	int status;
	u32 retries;
	u32 commands;
	atomic_t lock;
};

int ti254x_dc_lock(struct ti254x_dc_dev *dev);

void ti254x_dc_unlock(struct ti254x_dc_dev *dev);

int ti254x_dc_init(struct ti254x_dc_dev *dev, u8 *status);

void ti254x_dc_reset(struct ti254x_dc_dev *dev);

u8 ti254x_dc_resume(struct ti254x_dc_dev *dev);

int ti254x_dc_chip_erase(struct ti254x_dc_dev *dev, u8 *status);

ssize_t ti254x_dc_flash_read(struct ti254x_dc_dev *dev,
			 char *buf, loff_t off, size_t size);

int ti254x_dc_flash_write(struct ti254x_dc_dev *dev,
		const u8 *data,
		size_t offset,
		size_t size);
#endif
