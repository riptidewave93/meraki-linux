/**
 * AppliedMicro APM862xx SoC Ethernet Driver
 *
 * Copyright (c) 2011 Applied Micro Circuits Corporation.
 * All rights reserved. Mahesh Pujara <mpujara@apm.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * @file apm_enet_eee.c
 *
 * This file implements sysfs interface for Energy Efficient Ethernet
 * functionality of APM862xx SoC Ethernet subsystem
 *
 */

#ifndef __APM_ENET_EEE_H_
#define __APM_ENET_EEE_H_

#include <asm/ipp.h>
#include "apm_enet_access.h"
#include "apm_enet_csr.h"


int apm_enet_add_sysfs(struct device_driver *driver);
void apm_enet_remove_sysfs(struct device_driver *driver);

#endif /* __APM_ENET_EEE_H_ */
