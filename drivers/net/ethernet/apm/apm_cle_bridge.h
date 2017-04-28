/**
 * AppliedMicro APM862xx SoC Ethernet Driver
 *
 * Copyright (c) 2011 Applied Micro Circuits Corporation.
 * All rights reserved. Pranavkumar Sawargaonkar <psawargaonkar@apm.com>
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
 * @file apm_cle_bridge.h
 *
 *
 */

#ifndef __APM_CLE_BRIDGE_H_
#define __APM_CLE_BRIDGE_H_

#include "apm_enet_access.h"
#include "apm_cle_access.h"
#include "apm_preclass_data.h"
#include "apm_cle_cfg.h"
#include "apm_avl_api.h"
#include "apm_avl_base.h"

#undef DBG_CLE_BR_ERR
#undef DBG_CLE_BR_PRINTS
#undef DBG_CLE_BR_FDB_PRINTS
#undef APM_CLE_TEST_BR

#if defined(DBG_CLE_BR_ERR)
#define CLE_BR_ERR(x, ...)       printk(KERN_ERR x, ##__VA_ARGS__)
#else
#define CLE_BR_ERR(x, ...)
#endif

#if defined(DBG_CLE_BR_PRINTS)
#define CLE_BR_DBG(x, ...)       printk(KERN_INFO x, ##__VA_ARGS__)
#else
#define CLE_BR_DBG(x, ...)
#endif

#if defined(DBG_CLE_BR_FDB_PRINTS)
#define CLE_BR_FDB_DBG(x, ...)       printk(KERN_INFO x, ##__VA_ARGS__)
#else
#define CLE_BR_FDB_DBG(x, ...)
#endif

#define CLE_BR_CONFIG_PORT0	"bridge_port0"
#define CLE_BR_CONFIG_PORT1	"bridge_port1"
#define	APM_CLE_BR_DRIVER_NAME	"apm_cle_bridge"

#define MAX_AVL_ENTRIES AVL_SSIZE8B_DPTH
#define IFF_BRIDGE_PORT 0x8000
#define DEFAULT_BR_PRI  0

#define CLE_SET_DEF_RES_PRI(src)        (((u32) (src) << 10) & 0x00001c00)
#define CLE_DEF_RES_PRI         7

#define CLE_ETH0_RX_DB_IDX	0
#define CLE_ETH1_RX_DB_IDX	1
#define CLE_ETH0_FW_DB_IDX	2
#define CLE_ETH1_FW_DB_IDX	3

#define CMD_DISABLE_CLE_BR	'0'
#define CMD_ENABLE_CLE_BR	'1'

#undef CONFIG_CLE_BR_EXT_FWD
//#define CONFIG_CLE_BR_EXT_FWD

#if defined (CONFIG_CLE_BR_EXT_FWD)

#undef DBG_CLE_EXT_BR_FWD_PRINTS

#if defined(DBG_CLE_EXT_BR_FWD_PRINTS)
#define CLE_BR_DBG_EXT_FWD(x, ...)	printk(KERN_INFO x, ##__VA_ARGS__)
#else
#define CLE_BR_DBG_EXT_FWD(x, ...)
#endif


#define MAX_EXT_PORTS   4

/* These external ports does not represent apm eth ports
 *      For WAP -
 *         EXT_FWD_PORT1  corrosponds to ath0
 *         EXT_FWD_PORT2  corrosponds to ath1
 */
#define EXT_FWD_PORT1   4
#define EXT_FWD_PORT2   5
#define EXT_FWD_PORT3   6
#define EXT_FWD_PORT4   7

int apm_cle_br_register_ext_port(struct net_device *);
void apm_cle_br_unregister_ext_port(int port_id);
int apm_cle_br_fwd_directly(struct sk_buff *skb,
		struct apm_qm_msg_desc * rx_msg_desc);

/* Magic Keys */
#define CLE_BR_MAGIC1 0xdd
#define CLE_BR_MAGIC2 0xdd

#endif

int apm_cle_br_fdb_insert(struct net_device *, const unsigned char *);
int apm_cle_br_fdb_delete(struct net_device *, const unsigned char *);
int apm_cle_br_add_if(struct net_device *);
int apm_cle_br_del_if(struct net_device *);

#endif /* __APM_CLE_BRIDGE_H_ */
