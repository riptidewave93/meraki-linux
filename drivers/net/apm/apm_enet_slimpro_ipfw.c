/*
 * APM86xxx SlimPRO IPv4 Forward Offload cDriver
 * 
 * Copyright (c) 2010 Applied Micro Circuits Corporation.
 * All rights reserved. Loc Ho <lho@apm.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * This module configures the SlimPRO to handle IPv4 forward offload with
 * packet from APM86xxx SoC Ethernet ports.
 * 
 */
#include <asm/apm_slimpro_offload.h>
#include "apm_enet_access.h"

#ifdef CONFIG_APM_ENET_SLIMPRO_IPFW

#define SLIMPRO_IPFWID			"SlimPRO IPFW: "

static int apm_enet_slimpro_ipfw_cle_deinit(struct apm_enet_dev_base *priv);
static int apm_enet_slimpro_ipfw_cle_init(struct apm_enet_dev_base *priv);

static int apm_enet_slimpro_ipfw_ready_cb(struct apm_enet_dev_base *priv)
{
	priv->lro.ready = 1;
	if (priv->slimpro_ipfw.enable) {
		apm_enet_slimpro_ipfw_enable(priv, 1);	
	}
	return 0;
}

static int apm_enet_slimpro_ipfw_event(struct notifier_block *this,
                             	unsigned long event, void *ptr)
{
	struct slimpro_ipfw_ctx	*ctx;
	struct apm_enet_dev_base *priv;
	
	ENET_DEBUG("SlimPRO IP forward SlimPRO notify ready\n");
	ctx = container_of(this, struct slimpro_ipfw_ctx, nb);
	priv = container_of(ctx, struct apm_enet_dev_base, slimpro_ipfw);
	apm_enet_slimpro_ipfw_ready_cb(priv);
        return NOTIFY_DONE;
}

int apm_enet_slimpro_ipfw_init(struct apm_enet_dev_base *priv)
{
	struct slimpro_ipfw_ctx *ctx = &priv->slimpro_ipfw;
	
	/* Assume feature not available */
	ctx->ipfw_available = 0;
	
	/* Check if SlimPRO is available */
	if (is_apm86xxx_lite()) 
		return -ENODEV;
	
	/* Check if SlimPRO has IP forward offload feature */
	/* FIXME */
	
	ctx->ready = 0;		/* ready when SlimPRO firmware loaded */
	ctx->ipfw_available = 1;
	ctx->enable = 0;
	
	/* Register for SlimPRO readiness */
	ctx->nb.notifier_call = apm_enet_slimpro_ipfw_event;
	register_ipp_fwload_notifier(&ctx->nb);
	
	printk(KERN_INFO "APM86xxx Ethernet SlimPRO IP forward initialized\n");
	return 0;
}

static int apm_enet_slimpro_ipfw_send_ipaddr(struct apm_enet_dev_base *priv)
{
	struct slimpro_ipfw_ctx *ctx = &priv->slimpro_ipfw;
	u32 dat[2];
	int rc;
	
	dat[0] = IPP_ENCODE_NETDATA_CTRL_WORD(IPP_NETDATA_OFFLOAD_TYPE,
					IPP_NETOFFLOAD_IP4ADDR, 0, 0);
	dat[1] = ctx->ipfw_ipaddr;
	rc = ipp_send_data_msg(IPP_NETDATA_HDLR, dat, sizeof(dat));
	if (rc) {
		printk(KERN_ERR SLIMPRO_IPFWID "Fail to send IP Address\n");
		return rc;
	}
	return rc;
}

int apm_enet_slimpro_ipfw_set_ipaddr(struct apm_enet_dev_base *priv, 
				u32 ipaddr)
{
	struct slimpro_ipfw_ctx *ctx = &priv->slimpro_ipfw;
	
	apm_enet_slimpro_ipfw_cle_deinit(priv);
	ctx->ipfw_ipaddr = ipaddr;
	apm_enet_slimpro_ipfw_send_ipaddr(priv);
	apm_enet_slimpro_ipfw_cle_init(priv);
	return 0;
}

static int apm_enet_slimpro_ipfw_send_mac(struct apm_enet_dev_base *priv)
{
	struct slimpro_ipfw_ctx *ctx = &priv->slimpro_ipfw;
	u32 dat[3];
	int rc;
	
	dat[0] = IPP_ENCODE_NETDATA_CTRL_WORD(IPP_NETDATA_OFFLOAD_TYPE,
					IPP_NETOFFLOAD_IP4MAC, 0, 0);
	memcpy(&dat[1], ctx->ipfw_mac, 6);
	rc = ipp_send_data_msg(IPP_NETDATA_HDLR, dat, 4 + 6);
	if (rc) {
		printk(KERN_ERR SLIMPRO_IPFWID "Fail to send MAC Address\n");
		return rc;
	}
	return rc;
}

int apm_enet_slimpro_ipfw_set_mac(struct apm_enet_dev_base *priv, u8 *mac)
{
	struct slimpro_ipfw_ctx *ctx = &priv->slimpro_ipfw;
	
	apm_enet_slimpro_ipfw_cle_deinit(priv);
	memcpy(ctx->ipfw_mac, mac, 6);
	apm_enet_slimpro_ipfw_send_mac(priv);
	apm_enet_slimpro_ipfw_cle_init(priv);
	return 0;
}

static int apm_enet_slimpro_ipfw_cle_init(struct apm_enet_dev_base *priv)
{
	/* FIXME */
	return 0;
}
		
static int apm_enet_slimpro_ipfw_cle_deinit(struct apm_enet_dev_base *priv)
{
	/* FIXME */
	return 0;
}

int apm_enet_slimpro_ipfw_enable(struct apm_enet_dev_base *priv, u8 enable)
{
	priv->slimpro_ipfw.enable = enable;
	if (!priv->slimpro_ipfw.ready)
		return 0;
	if (enable) {
		/* Query for MAC address */
		/* FIXME - if mac available, then do this */
		
		/* Configure the classifier */
		apm_enet_slimpro_ipfw_send_ipaddr(priv);
		apm_enet_slimpro_ipfw_send_mac(priv);		
		apm_enet_slimpro_ipfw_cle_init(priv);
	} else {
		apm_enet_slimpro_ipfw_cle_deinit(priv);
	}			
	return 0;
}

#endif
