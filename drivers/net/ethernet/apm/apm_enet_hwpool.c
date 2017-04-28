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
 * This file implements driver for RGMII, MAC and statistics blocks of
 * APM862xx SoC Ethernet subsystem
 *
 */

#include <linux/netdevice.h>
#include <net/ip.h>
#include <net/tcp.h>
#include <linux/if_vlan.h>
#include <linux/inetdevice.h>
#include <linux/in6.h>
#include <net/addrconf.h>
#include <asm/ipp.h>
#include <asm/cacheflush.h>
#if defined(CONFIG_SMP)
#include <asm/smp.h>
#endif

#include "apm_enet_access.h"
#include "apm_enet_mac.h"
#include "apm_enet_csr.h"
#include "apm_enet_misc.h"
#include "apm_enet_eee.h"
#include "apm_cle_csr.h"

extern struct apm_enet_dev_base * qid2priv[MB_MAX_QUEUES];

#if defined(CONFIG_SLAB_HW)
int eth0_fp_id;
int eth1_fp_id;
int ppc_fp_id = 0;
int ppc_fp_mbid;
int ppc_hw_buff_pool_init = 0;
extern struct kmem_cache *virt_to_cache_ext(const void *obj);
#endif

#ifdef CONFIG_DRIVER_POOL
struct kmem_cache *fp_slab_cache; /* Slab cache related parameters */
extern inline void __cache_free(struct kmem_cache *cachep, void *objp);
extern struct kmem_cache *virt_to_cache_ext(const void *obj);
#endif

#if defined(CONFIG_SLAB_HW)
#if 0
static int apm_enet_init_ppc_fp(int queue_id, int no_of_buffs)
{
	int i;
	phys_addr_t phy_addr;
	struct mb_qm_msg_desc fp_msg_desc;
	struct mb_qm_msg16 msg;
	struct kmem_cache *fp_slab_cache; /* Slab cache related parameters */
	char *tmp_ptr;
	int rc = 0;

	if (ppc_hw_buff_pool_init)
		return rc;

	/* Create Free pool slab cache for PPC */
	fp_slab_cache = kmem_cache_create("FreePool-PPC", 2048, 400, 
				   	__GFP_FREEPOOL_PPC, NULL);

	if (fp_slab_cache == NULL) {
		printk(KERN_ERR "Failed to create slab cache for PPC\n");
		return -1;
	}

	memset(&msg, 0, sizeof(msg));

	/* Common fields  */ 
	msg.C = apm_enet_pkt_iscoherent();
	msg.BufDataLen = APM_PKT_POOL_SIZE;	/* Set buffer size */ 
	msg.FPQNum = ppc_fp_id;
	msg.RType = MB_QM_PPC_RTYPE;
	
	fp_msg_desc.qid = ppc_fp_id; 
	fp_msg_desc.mb_id = 5;
	fp_msg_desc.msg = &msg;

	/* Program all the free buffer pools */
	for (i = 0; i < no_of_buffs; i++) {
		tmp_ptr = kmem_cache_alloc(fp_slab_cache, 0);
		phy_addr = virt_to_phys(tmp_ptr);
		msg.DataAddrMSB = (u32) (phy_addr >> 32);
		msg.DataAddrLSB = (u32) phy_addr;

		/* Dealloc to free pool */
		if ((mb_qm_fp_dealloc_buf(&fp_msg_desc)) != 0) {
			printk(KERN_ERR "Can not de-allocate FP buffer\n");
			return -1;
		}
	}

        ppc_hw_buff_pool_init = 1;

	return rc;
}
#endif

int apm_enet_init_fp(int queue_id, int buff_size,
				int no_of_buffs, int qm_blk, int hdr_rsvd)
{
	int i;
	phys_addr_t phy_addr;
	struct apm_qm_msg_desc fp_msg_desc;
	struct apm_qm_msg16 msg;
	struct kmem_cache *fp_slab_cache; /* Slab cache related parameters */
	char *tmp_ptr;
	struct apm_enet_dev_base *priv_base;
	int rc = 0;

	priv_base = qid2priv[queue_id];

	/* Create Free pool slab cache for Ethernet port */
	if (priv_base->port_id == 0) {
		fp_slab_cache = kmem_cache_create("FreePool-Eth0", 2048, 200, 
				     	__GFP_FREEPOOL_ETH0, NULL);
	} else if (priv_base->port_id == 1) {
		fp_slab_cache = kmem_cache_create("FreePool-Eth1", 2048, 200, 
				     	__GFP_FREEPOOL_ETH1, NULL);
	} else {
		printk("Invalid port id: %d from queue id: %d\n", priv_base->port_id, queue_id);
		return -1;
	}

	if (fp_slab_cache == NULL) {
		printk(KERN_ERR "Failed to create slab cache for Eth%d port\n", priv_base->port_id);
		return -1;
	}

	printk("SLAB cache created for port %d, with queue id: %d\n",  priv_base->port_id, queue_id);

	memset(&msg, 0, sizeof(msg));

	/* Common fields  */ 
	msg.C = apm_enet_pkt_iscoherent();
	msg.BufDataLen = APM_PKT_POOL_SIZE;	/* Set buffer size */ 
	msg.FPQNum = queue_id;
	msg.RType = APM_QM_ETH_RTYPE;
	
	fp_msg_desc.qid = queue_id; 
	fp_msg_desc.mb_id = queue_id;
	fp_msg_desc.msg = &msg;

	/* Program all the free buffer pools */
	for (i = 0; i < no_of_buffs; i++) {
		tmp_ptr = kmem_cache_alloc(fp_slab_cache, 0);
		tmp_ptr+=256;
		/* convert to physical address and program buffer pool */
		phy_addr = virt_to_phys(tmp_ptr);
		msg.DataAddrMSB = (u32) (phy_addr >> 32);
		msg.DataAddrLSB = (u32) phy_addr;

		/* Dealloc to free pool */
		if ((apm_qm_fp_dealloc_buf(&fp_msg_desc)) != 0) {
			printk(KERN_ERR "Can not de-allocate FP buffer\n");
			return -1;
		}
	}

	return rc;
}

int apm_enet_rx_frame(struct apm_qm_msg_desc *rx_msg_desc)
{
	struct apm_qm_msg32 *msg = (struct apm_qm_msg32 *) rx_msg_desc->msg;
	struct apm_qm_msg16 *msg16 = &msg->msg16;
	struct sk_buff *skb;
	struct apm_enet_dev_base *priv_dev;
	/* free pool related variables */
	char *buffer_addr;
	u32 len = 0;
	struct skb_shared_info *shinfo;
	int size;

#ifdef ENET_DBG
	int port_id;
#endif

	/* Strip off CRC, HW isn't doing that */ 
	u32 data_len = apm_enet_rx_decode_len(msg16->BufDataLen) - 4;
	
	DEBG_HW_POOL(KERN_INFO "RX msg queue: %d Rtype: %d\n",
		rx_msg_desc->qid, msg16->RType);
	
	priv_dev = qid2priv[rx_msg_desc->qid];
#ifdef ENET_CHK
	if (priv_dev == NULL) {
		printk(KERN_ERR "private ptr for QID %d is NULL\n", 
			rx_msg_desc->qid);
		return -1;
	}
#endif
	
	/* Check for error, if packet received with error */
	/* FIXME - Work around to handle IP fragmented packets.
	   Use CLE instead to bypass HW checksum.
	*/
	if (unlikely((msg16->LErr) && (msg16->LErr != 7))) {
		/* Parse error */
		apm_enet_parse_error(msg16->LErr);
		printk(KERN_ERR "ENET error %d QID %d\n",
			msg16->LErr, rx_msg_desc->qid);
		if (msg16->LErr == ENET_ING_CRC_ERR) {
			/* For CRC, need to re-fill the free pool */
			/* goto err_refill;			 */ /* FIXME */
		}
		return -1;
	}

	ENET_DEBUGRXTX("RX QID %d port %d\n", 
		rx_msg_desc->qid, priv_dev->port_id);
	
#ifdef ENET_DBG
	if (priv_dev->ndev == NULL) {
		printk(KERN_INFO "Couldn't find ndev for port %d\n",
			priv_dev->port_id);
		return -1;
	}
#endif

#if defined FAM_BUF_POOL
	buffer_addr = (char *) ((char *) msg16->DataAddrLSB + 0xf1220000);
#else
	buffer_addr = phys_to_virt((u64) ((u64) msg16->DataAddrMSB << 32) |
				msg16->DataAddrLSB);
#endif

	prefetch(buffer_addr - HW_SKB_HEADROOM - NET_IP_ALIGN);

	/* Retrieve skb address */
	skb = alloc_control_skb();
	DEBG_HW_POOL(KERN_INFO "Allocated control SKB: %p\n", skb);

	size = SKB_DATA_ALIGN(data_len);
	skb->head = buffer_addr - HW_SKB_HEADROOM - NET_IP_ALIGN;
	skb->data = skb->tail = buffer_addr;

	/* point end to after max packet size and align 32 */
	skb->end = skb->data + 1536; 
	skb_put(skb, data_len);

        /* make sure we initialize shinfo sequentially */
       	shinfo = skb_shinfo(skb);
        atomic_set(&shinfo->dataref, 1);
       	shinfo->nr_frags  = 0;
        shinfo->gso_size = 0;
       	shinfo->gso_segs = 0;
        shinfo->gso_type = 0;
       	shinfo->ip6_frag_id = 0;
        shinfo->tx_flags.flags = 0;
	skb_frag_list_init(skb);
        memset(&shinfo->hwtstamps, 0, sizeof(shinfo->hwtstamps));

        /* FIXME - need to handle jumbo packet */

	len = skb->len;
	skb->dev = priv_dev->ndev;

	ENET_DEBUGRXTX("Got SKB data: 0x%p physical: 0x%x len %d skb len %d\n", 
		skb->data, (u32) virt_to_phys(skb->data), data_len, skb->len);
	
	ENET_DUMP("RX: ", (u8 *)skb->data, len);
	
	skb->protocol = eth_type_trans(skb, priv_dev->ndev);

	if (in_irq()) {
		printk(":::Shouldn't have come here:::Giving"
		       "frame to stack in IRQ\n");
/* :CD: See if we can simplify this */
#ifdef DRIVER_LOOPBACK
		apm_enet_start_xmit(skb, priv_dev->ndev);
#if defined(NOTIFICATION_OFF)
		FREE_SKB(skb);
#endif
#elif defined DRIVER_DROP
		FREE_SKB(skb);
		++priv_dev->estats.rx_dropped_stack; 
#else
		ENET_DEBUG("Calling netif_rx\n");
		if (unlikely(netif_rx(skb) == NET_RX_DROP)) {
			++priv_dev->estats.rx_dropped_stack; 
		}
#endif
	} else {
		ENET_DEBUG("Giving frame to stack in polling \n");
#ifdef DRIVER_LOOPBACK
		apm_enet_start_xmit(skb, priv_dev->ndev);
#if defined(NOTIFICATION_OFF)
#if !defined(CONFIG_SLAB_HW)
		FREE_SKB(skb);
#endif
#endif
#elif defined DRIVER_DROP
		FREE_SKB(skb);
		++priv_dev->estats.rx_dropped_stack;
#else
		netif_receive_skb(skb);
#endif
	}

	++priv_dev->stats.rx_packets;
	priv_dev->stats.rx_bytes += len;
	return 0;
}
#elif defined(CONFIG_DRIVER_POOL)
static int apm_enet_init_fp(int queue_id, int buff_size,
				int no_of_buffs, int qm_blk)
{
	int i;
	phys_addr_t phy_addr;
	struct mb_qm_msg_desc fp_msg_desc;
	struct mb_qm_msg16 msg;
	char *tmp_ptr;
	int rc = 0;

	/* Create Free pool slab cache for port 0 */
	fp_slab_cache = kmem_cache_create("Driver-Pool", 2048, 200, 
				     	__GFP_DRIVER_POOL, NULL);

	if (fp_slab_cache == NULL) {
		printk(KERN_ERR "Failed to create slab cache for Eth0 port\n");
		return -1;
	}

	memset(&msg, 0, sizeof(msg));

	/* Common fields  */ 
	msg.C = apm_enet_pkt_iscoherent();
	msg.BufDataLen = APM_PKT_POOL_SIZE;	/* Set buffer size */ 
	msg.FPQNum = queue_id;
	msg.RType = MB_QM_ETH_RTYPE;
	
	fp_msg_desc.qid = queue_id; 
	fp_msg_desc.mb_id = queue_id;
	fp_msg_desc.msg = &msg;

	/* Program all the free buffer pools */
	for (i = 0; i < no_of_buffs; i++) {
		tmp_ptr = kmem_cache_alloc(fp_slab_cache, 0);
		tmp_ptr+=256; /* reserve some space */
		/* convert to physical address and program buffer pool */
		phy_addr = virt_to_phys(tmp_ptr);
		msg.DataAddrMSB = (u32) (phy_addr >> 32);
		msg.DataAddrLSB = (u32) phy_addr;

		/* Dealloc to free pool */
		if ((mb_qm_fp_dealloc_buf(&fp_msg_desc)) != 0) {
			printk(KERN_ERR "Can not de-allocate FP buffer\n");
			return -1;
		}
	}

	return rc;
}

static inline int apm_enet_cfg_buf_address(int queue_id, int size)
{
	struct mb_qm_msg_desc fp_msg_desc;
	struct mb_qm_msg16 msg;
	char *tmp_ptr;
	phys_addr_t phy_addr;
	
	fp_msg_desc.msg = &msg;
	fp_msg_desc.qid = queue_id; 
	fp_msg_desc.mb_id = queue_id;

	tmp_ptr = kmem_cache_alloc(fp_slab_cache, 0);

	if (unlikely(!tmp_ptr)){
		printk(KERN_ERR "Failed to allocate memory from driver pool\n");
		return -ENOMEM;
	}

	tmp_ptr+=256; /* reserve some space */

	phy_addr = virt_to_phys(tmp_ptr);
	
	/* Program individual WORD to avoid use of memzero */
	((u32 *) &msg)[0] = apm_enet_pkt_iscoherent() << 23 | 
				(APM_PKT_POOL_SIZE << 8) | 
				(u32) (phy_addr >> 32);
	((u32 *) &msg)[1] = (u32) phy_addr;
	((u32 *) &msg)[2] = MB_QM_ETH_RTYPE << 24 | queue_id;

	/* Fill with the new buffer address */
	if (unlikely((mb_qm_fp_dealloc_buf(&fp_msg_desc)) != 0)) {
		__cache_free(fp_slab_cache, tmp_ptr);
		printk(KERN_ERR "Can not replenish FP buffer\n");
		return -1;
	}
	return 0;
}

int apm_enet_rx_frame(struct mb_qm_msg_desc *rx_msg_desc)
{
	struct mb_qm_msg32 *msg = (struct mb_qm_msg32 *) rx_msg_desc->msg;
	struct mb_qm_msg16 *msg16 = &msg->msg16;
	struct sk_buff *skb;
	struct apm_enet_dev_base *priv_dev;
	/* free pool related variables */
	char *buffer_addr;
	struct skb_shared_info *shinfo;
	int size;
	u32 data_len;  
#if !defined(CONFIG_SMP) || defined(SMP_LOAD_BALANCE)
	u32 core_id = apm_processor_id();
#else   
	u32 core_id = 0;		/* SMP - non-load balanced */
#endif 		

	/* Save variables locally from MailBox as we will lose the slot */
	u32 BufDataLen = msg16->BufDataLen;
	u8 LErr = msg16->LErr;
	u32 DataAddrMSB = msg16->DataAddrMSB;
	u32 DataAddrLSB = msg16->DataAddrLSB;

	/* Strip off CRC, HW isn't doing that */ 
	data_len = apm_enet_rx_decode_len(BufDataLen) - 4;
	
	priv_dev = qid2priv[rx_msg_desc->qid];
	
	/* Check for error, if packet received with error */
	/* FIXME - Work around to handle IP fragmented packets.
	   Use CLE instead to bypass HW checksum.
	*/
	if (unlikely((LErr) && (LErr != 7))) {
		/* Parse error */
		apm_enet_parse_error(LErr);
		printk(KERN_ERR "ENET error %d QID %d\n",
			LErr, rx_msg_desc->qid);
		if (LErr == ENET_ING_CRC_ERR) {
			/* For CRC, need to re-fill the free pool */
			/* goto err_refill;			 */ /* FIXME */
		}
		return -1;
	}

	ENET_DEBUGRX("RX QID %d port %d\n", 
		rx_msg_desc->qid, priv_dev->port_id);
	
	buffer_addr = phys_to_virt((u64) ((u64) DataAddrMSB << 32) |
				DataAddrLSB);

	/* Retrieve skb address */
	skb = alloc_control_skb();
	DEBG_HW_POOL(KERN_INFO "Allocated control SKB: %p\n", skb);

	size = SKB_DATA_ALIGN(data_len);
	skb->truesize += size;
	skb->head = buffer_addr - 256;
	/* FIXME, check if we want to add some head room */
	skb->data = skb->tail = buffer_addr;

	/* point end to after max packet size and align 32 */
	skb->end = skb->tail + 1536; 
	skb_put(skb, data_len);

        /* make sure we initialize shinfo sequentially */
       	shinfo = skb_shinfo(skb);
        atomic_set(&shinfo->dataref, 1);
       	shinfo->nr_frags  = 0;
        shinfo->gso_size = 0;
       	shinfo->gso_segs = 0;
        shinfo->gso_type = 0;
       	shinfo->ip6_frag_id = 0;
        shinfo->tx_flags.flags = 0;
       	shinfo->frag_list = NULL;
        memset(&shinfo->hwtstamps, 0, sizeof(shinfo->hwtstamps));

        /* FIXME - need to handle jumbo packet */

	skb->dev = priv_dev->ndev;

	ENET_DEBUGRX("Got SKB data: 0x%p physical: 0x%x len %d skb len %d\n", 
		skb->data, (u32) virt_to_phys(skb->data), data_len, skb->len);
	
	ENET_DUMP("RX: ", (u8 *)skb->data, skb->len);
	
	skb->protocol = eth_type_trans(skb, priv_dev->ndev);

	apm_enet_cfg_buf_address(priv_dev->qm_queues[core_id].rx_fp_qid, 
				APM_PKT_BUF_SIZE);
	netif_receive_skb(skb);
	++priv_dev->stats.rx_packets;
	priv_dev->stats.rx_bytes += skb->len;
	return 0;
}
#endif 
