#ifndef __DEV_XLNA_H__
#define __DEV_XLNA_H__

/*
 * Definitions for XLNA enabling driver for Meraki Z1
 *
 * Justin Delegard, Dean Thomas
 * Copyright (c) 2012, 2013 Cisco-Meraki, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */
#include <asm/addrspace.h>

struct xlna_platform_data {
    unsigned gpio0;
    unsigned gpio1;
    int enabled;
};

#define GPIO0_XLNA_ENABLE               0x2e
#define GPIO1_XLNA_ENABLE               0x2f

#endif /* __DEV_XLNA_H__ */
