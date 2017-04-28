/**
 * AppliedMicro APM862xx SoC Ethernet SlimPRO IP Forward Driver
 *
 * Copyright (c) 2011 Applied Micro Circuits Corporation.
 * All rights reserved. Loc Ho <lho@apm.com>
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
 * This module configures the SlimPRO to handle IPv4 forward offload with
 * packet from APM86xxx SoC Ethernet ports.
 */
#ifndef __APM_ENET_SLIMPRO_IPFW_H__
#define __APM_ENET_SLIMPRO_IPFW_H__

#ifdef CONFIG_APM_ENET_SLIMPRO_IPFW

struct slimpro_ipfw_ctx {
	struct notifier_block nb;
	u8 ipfw_available;		/* SlimPRO available in SoC */ 
	u8 enable;			/* Enabled this feature */
	u8 ready;			/* Ready for operation */
	u32 ipfw_ipaddr;		/* IP address to forward */
	u8 ipfw_mac[6];			/* MAC address to forward to */		
};

/* Forward declaration */
struct apm_enet_dev_base;

int apm_enet_slimpro_ipfw_init(struct apm_enet_dev_base *priv);
int apm_enet_slimpro_ipfw_enable(struct apm_enet_dev_base *priv, u8 enable);
int apm_enet_slimpro_ipfw_set_ipaddr(struct apm_enet_dev_base *priv, 
				u32 ipaddr);
int apm_enet_slimpro_ipfw_set_mac(struct apm_enet_dev_base *priv, u8 *mac);

#endif /* CONFIG_APM_ENET_LRO */

#endif /* __APM_ENET_LRO_H__ */
