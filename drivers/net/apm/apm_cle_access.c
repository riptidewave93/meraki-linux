/**
 * AppliedMicro APM862xx SoC Classifier Driver
 *
 * Copyright (c) 2010 Applied Micro Circuits Corporation.
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
 * @file apm_cle_access.c
 *
 * This file implements access layer for APM862xx SoC Classifier driver
 *
 */

#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <linux/in.h>
#include <linux/etherdevice.h>
#include <linux/crc32.h>
#include <linux/ethtool.h>
#include <linux/crc16.h>
#include <linux/of_platform.h>
#include <linux/kthread.h>

#if defined(CONFIG_SMP)
#include <asm/smp.h>
#endif

#include "apm_preclass_data.h"

u64 apm_class_base_addr_p;
u32 apm_class_base_addr;

int apm_gbl_cle_rd32(u32 reg_offset, u32  *value)
{
	u32 addr;
	
#ifdef CLE_DBG	   
	if (value == NULL) {
		PCLS_DBG("Error: Null value pointer in calling %s \n",
			 __FUNCTION__);
		return APM_RC_INVALID_PARM;
	}
#endif
	addr = apm_class_base_addr + reg_offset;
	*value = in_be32((void __iomem *) addr);
	PCLS_DBG("CLE CSR Read at addr 0x%08X value 0x%08X \n", addr, *value);
	return APM_RC_OK;
}

int apm_gbl_cle_wr32(u32 reg_offset, u32 value)
{
    u32	addr;
    
    addr = apm_class_base_addr + reg_offset;
    
    PCLS_DBG("CLE CSR Write at addr 0x%08X value 0x%08X \n", addr, value);
    out_be32((void __iomem *) addr, value);
    return APM_RC_OK;
}

