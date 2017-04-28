/**
 * AppliedMicro APM862xx SoC Ethernet LRO Driver
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
 * APM86xxx Ethernet LRO implementation for APM86xxx SoC.
 */
#ifndef __APM_ENET_LRO_H__
#define __APM_ENET_LRO_H__

#include <asm/apm_qm_core.h>

#ifdef CONFIG_APM_ENET_LRO

#define LRO_MAX_PORT		16
#define LRO_MAX_PKT 		1536
#define LRO_BUF_SIZE 		(64*1024)
#define LRO_MAX_HDR 		128
#define LRO_MAX_BUFFS 		((APM_PKT_BUF_SIZE - (LRO_MAX_HDR + 8)) / 12 )
#define LRO_SLIMPRO_ALIGN 	2

/* Check for cap LRO max buffer and max packet size */
#if LRO_MAX_BUFFS > 150
#undef LRO_MAX_BUFFS
#define LRO_MAX_BUFFS 		150
#endif
#if LRO_MAX_PKT < (LRO_MAX_HDR + 8 + (12 * LRO_MAX_BUFFS))
#undef LRO_MAX_PKT
#define LRO_MAX_PKT 		(LRO_MAX_HDR + 8 + (12 * LRO_MAX_BUFFS))
#endif

/**
 * Structure of the encode LRO gathered packet 
 *
 *  SKB data pointer                            
 *  |-----------------------------------------------------------------------------------|
 *  | LRO header | TCP/IP header | LRO Data Link list | skb back ptr | Linux sharedinfo |
 *  |-----------------------------------------------------------------------------------|
 */
struct apm_lro_buff_info {
	u16 buff_rsv;
	u16 buff_fpq;
	u32 buff_len;
	u32 buff_addr_msb;
	u32 buff_addr_lsb;
};
 
struct apm_lro_chain {
	u32 tot_len;		/* total length */
	u16 tot_buffs;		/* Total buffer */
	u16 hdr_len;		/* TCP/IP header length */
	u8 hdr[LRO_MAX_HDR];	/* TCP/IP header data */
	struct apm_lro_buff_info buff[LRO_MAX_BUFFS]; 
				/* Chained link list of data ptr */
};

struct apm_enet_lro_ctx {
	struct notifier_block nb;
	u8 lro_available;		/* SlimPRO available in SoC */ 
	u8 enable;			/* Enabled this feature */
	u8 ready;			/* Ready for operation */
	u16 port[LRO_MAX_PORT];
	u32 timeout_ms;
	u32 max_byte_cnt;
	struct apm_qm_msg_desc poll_msg_desc;
	
	/* Queue info */
	int ppc_lro_wqid[MAX_CORES];
	int slimpro_dma_wqid;
	int slimpro_dma_cqid;
	int slimpro_wqid;
	int slimpro_hdr_fqid;
	int slimpro_data_fqid;
	
	/* Static */
	int max_lro_rx_buffs;
};

/* Forward declaration */
struct apm_enet_dev_base;

int apm_enet_lro_enable(struct apm_enet_dev_base *priv, u8 enable);
int apm_enet_lro_set_port(struct apm_enet_dev_base *priv, u16 *port_list);
int apm_enet_lro_set_timeout(struct apm_enet_dev_base *priv, u32 timeoutms);
int apm_enet_lro_set_maxbytecnt(struct apm_enet_dev_base *priv, u32 maxbytecnt);

int apm_enet_lro_init(struct apm_enet_dev_base *priv);
int apm_enet_lro_rx_frame(struct apm_qm_msg_desc *rx_msg_desc);
int apm_enet_lro_poll(struct napi_struct *napi, int budget);

#endif /* CONFIG_APM_ENET_LRO */

#endif /* __APM_ENET_LRO_H__ */
