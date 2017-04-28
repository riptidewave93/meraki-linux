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
 * @file apm_enet_misc.c
 *
 * This file implements APM862xx SoC Ethernet subsystem driver
 *
 */

#ifndef __APM_ENET_MISC_H_
#define __APM_ENET_MISC_H_

#include "apm_enet_access.h"

/**
 * @brief   This function parse the LErr field from work message 
 * @param   LErr - Local Err as in work message 
 * @return  None
 **
 */
void apm_enet_parse_error(u8 LErr);

int apm_enet_fp_pb_flush(struct apm_data_priv *priv, int pbn);

void apm_enet_unmask_int(struct apm_data_priv *priv);

#ifdef INT_SUPPORT
irqreturn_t apm_enet_qmi_err_irq(int irq, void *dev_instance);
irqreturn_t apm_enet_mac_err_irq(int irq, void *dev_instance);
irqreturn_t apm_enet_err_irq(int irq, void *dev_instance);
#endif 	


#endif /* __APM_ENET_MISC_H_ */
