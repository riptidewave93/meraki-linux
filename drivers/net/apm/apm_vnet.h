/**
 * APM APM86xxx virtual ethernet for inter-core communication
 *
 * Copyright (c) 2010, Applied Micro Circuits Corporation
 * Auther: Vivek Mahajan <vmahajan@apm.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 *
 */
#ifndef	__APM_VNET_H_
#define	__APM_VNET_H_

#define PFX			"APMVNET: "
#define APM86xxx_VNET		"AM86xxx Virtual Ethernet"
#define APM_VNET_NAME		"veth"
#define VNET_MAX_FRAME		(16*1024)
#define VNET_FRAME_PAD		18

#define APM_ENET_NAPI_WT	2
#define CONFIG_NAPI

struct apm_vnet_stats {
	u64 rx_packets;
	u64 rx_bytes;
	u64 tx_packets;
	u64 tx_bytes;
	u64 rx_packets_csum;
	u64 tx_packets_csum;
};

struct apm_vnet_error_stats {
	u64 rx_dropped;
	u64 rx_dropped_stack;
	u64 rx_dropped_error;
	u64 tx_dropped;
	u64 tx_dropped_error;
};

struct qmsg_address {
	u16 next_fpq_id	:8;
	u8 rsvd		:1;
	u16 next_data_len:15;
	u8  rsvd2	:4;	/* AB bit will be set here */
	u8  next_addr_hi:4;
	u32 next_addr_lo;
} __attribute__ ((packed));

struct xchng_qcfg {
	u8 src_msg_id   :4;	/* Inter-core message interrupt index */
	u8 src_core_id  :4;	/* Source of the core ID */
	u8 dying_core	:5;	/* This core ID - 1 wants shutdown */ 
	u8 ack		:1;	/* Require ACK response */
	u8 rsvd1	:2;	
	u16 port	:4;	/* Virtual Port number */ 
	u16 rsvd2	:12;	
} __attribute__ ((packed));

struct vnet_percpu_data {
	spinlock_t lock;
	struct tasklet_struct rx_tasklet;
	struct sk_buff *to_free_skb;
};

struct apm_vnet_priv_dev {
	struct net_device *ndev;
	unsigned int port_id;
	struct net_device_stats nstats;
	struct apm_vnet_stats stats;
	struct apm_vnet_error_stats estats;
	struct device dev;
	struct napi_struct napi;
#if defined(CONFIG_NAPI)
	struct apm_qm_msg_desc pending_fn_args;
	struct apm_qm_msg32 napi_qm_msg;
	struct qmsg_address napi_qm_msg_addr[4]; /* must be before napi_qm_msg */
#endif
	struct dma_chan_info dmach;	
	int irq;			/* IRQ number for interrupt */
	int local_msg_idx;		/* IRQ msg index to receive */
	int remote_core_id;		/* CPU id to send to */
	int remote_msg_idx;		/* Remote IRQ msg index to send to */
	int remote_qid;			/* Remote QID to enqueue to */
	int remote_qm_id;		/* QM IP ID to link queue to */
	int remote_link;		/* Status of link with remote port
	                                   -1 = not established, 0 = showdown,
	                                   1 = active */
	int remote_port;		/* Remote virtual port */	                                   
	atomic_t remote_inflight;	/* Operation in-flight */	                                   
	unsigned long remote_link_start_ts;

	struct vnet_percpu_data *percpu_ptr;
};

#endif
