/* cisco-act.h - definitions for cisco-act2 TAM support                     */ 
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

#ifndef _CISCO_ACT_H_
#define _CISCO_ACT_H_

#include <linux/types.h>
#include <linux/device.h>

/* Min/max number of bytes supported by tam library platform buffer */
#define WRITE_MIN 48
#define WRITE_MAX 0x1000

#define MAILBOX_DIRECTION_WRITE		0x2
#define MAILBOX_DIRECTION_READ		0x3

/* Host storage, does not exist on device
*/
#define FPGA_HOST_STORAGE_SIZE 0x10
#define FPGA_HOST_STORAGE_ADDR_START 0xff0000
#define FPGA_HOST_IAP_TRIGGER_OFFSET 0x00

struct cisco_act_multiboot_data {
	bool enabled;
	uint32_t reg;
	uint8_t mask;
	bool active_low;
};

struct act_data {
	/*
	* Lock protects against activities from other Linux tasks,
	* but not from changes by other I2C masters.
	*/
	struct mutex lock;

	struct bin_attribute bin;
	struct device *dev;
	void *bus_dev;

	const struct cisco_act_bus_ops* bus_ops;
	u8 *writebuf;
	uint16_t writebuf_size;
	u32 chip_family;

	u8 *host_storage;
	u8  host_storage_size;
	u8  software_reset;
	u32 host_storage_addr_start;
	u32 host_storage_addr_end;

	struct cisco_act_multiboot_data mboot;
};

#define TAM_FAMILY_ACT2		1
#define TAM_FAMILY_FPGA		2

struct cisco_act_platform_data {
	struct cisco_act_multiboot_data mboot;
	uint32_t chip_family;
	uint16_t write_buf_size;
	uint8_t software_reset;
};

struct cisco_act_bus_ops {
	u16 bustype;
	ssize_t (*read)(struct act_data *act,
			char* buf,
			unsigned offset,
			size_t count);
	ssize_t (*write)(struct act_data *act,
			 const char* buf,
			 unsigned offset,
			 size_t count);

	ssize_t (*reg_read)(struct act_data *act,
			    uint32_t reg,
			    uint8_t* val,
			    size_t count);
	ssize_t (*reg_write)(struct act_data *act,
			     uint32_t reg,
			     uint8_t* val,
			     size_t count);
};

extern struct act_data* cisco_act_common_probe(struct device* dev,
					  const struct cisco_act_bus_ops* ops);
extern int cisco_act_common_lateinit(struct device* dev);

/* Exported symbols */
extern ssize_t act_reg_read(struct device *dev,
			    uint32_t reg,
			    uint8_t* val,
			    size_t count);
extern ssize_t act_reg_write(struct device *dev,
			     uint32_t reg,
			     uint8_t* val,
			     size_t count);

#if IS_ENABLED(CONFIG_OF)
extern struct device* of_find_act_device_by_node(struct device_node *node);
#endif
extern struct device* find_act_device_by_name(const char* name);

#endif
