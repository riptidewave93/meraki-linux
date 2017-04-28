/**
 * AppliedMicro APM862xx SoC Ethernet Driver
 *
 * Copyright (c) 2010 Applied Micro Circuits Corporation.
 * All rights reserved.
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
 * @file apm_enet_offload.h
 *
 * This file defines offload functions for APM862xxSoC Ethernet subsystem
 *
 **
 */

#ifndef __APM_ENET_OFFLOAD_H__
#define __APM_ENET_OFFLOAD_H__

#include "apm_enet_access.h"

#define INVALID_LIST_HEAD(h) (!(h->next && h->prev))
#define OFFLOAD_KEY_SIZE 3

#define APM_RC_MISS 1

enum apm_ethoffload {
	ETHOFFLOAD_DEFAULT = 0,
	ETHOFFLOAD_IPV4_FORWARD,
	ETHOFFLOAD_ADD,
	ETHOFFLOAD_DEL,
	ETHOFFLOAD_SHOW,
	ETHOFFLOAD_MAX
};

extern struct ethoffload_ops apm_ethoffload_ops;

#endif /* __APM_ENET_OFFLOAD_H__ */

