/**
 * AppliedMicro APM86xxx SoC Look aside Clasifier Driver
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
 * @file   apm_lac_access.c
 *
 * This is the Linux layer of the APM86xxx SoC Look aside Clasifier Driver
 */
#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <linux/in.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/mod_devicetable.h>
#include <linux/interrupt.h>
#include <linux/spinlock_types.h>
#include <linux/highmem.h>
#include <linux/scatterlist.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <linux/of_platform.h>
#include <linux/spinlock_types.h>
#include <asm/io.h>
#include <asm/ipp.h>
#include <asm/apm_qm_core.h>
#include <asm/apm_qm_access.h>
#include <asm/apm86xxx_soc.h>
#include <asm/apm86xxx_pm.h>
#include "apm_lac_access.h"
#include "apm_preclass_data.h"

#define	APMLAC_VERSION_STR		"0.1"
#define RES_SIZE(r)			((r)->end - (r)->start + 1)

u64 apm_lac_base_addr_p;
u32 apm_lac_base_addr;
struct lac_queues lac_queues[MAX_CORES];
u32 shared_mode;
u32 lac_state;
struct lac_stats lac_st = {0};
/* FIXME */
struct apm_ptree_config lac_cfg = {0, 64, 12, 512, 1, 8};


int apm_lac_rd32(u32 reg_offset, u32  *value)
{
	u32 addr;
	int rc = 0;
	
#ifdef APM_LAC_RDDEBUG
	if (value == NULL) {
		APMLAC_RDLOG("Error: Null value pointer in calling %s \n",
	     			 __FUNCTION__);
		return -ENOMEM;
	}
#endif
	addr = apm_lac_base_addr + reg_offset;
	*value = in_be32((void __iomem *) addr);
	APMLAC_RDLOG("LAC CSR Read at addr 0x%08X value 0x%08X \n",
			addr, *value);
	return rc;
}

int apm_lac_wr32(u32 reg_offset, u32 value)
{
	u32 addr;
	int rc = 0;
    
    	addr = apm_lac_base_addr + reg_offset;
    
    	APMLAC_WRLOG("LAC CSR Write at addr 0x%08X value 0x%08X \n",
		 	addr, value);
	
	out_be32((void __iomem *)addr, value);
    	return rc;
}

static inline int apm_lac_pkt_iscoherent(void)
{
#if defined(CONFIG_APM86xxx_IOCOHERENT)
	return 1;
#elif !defined(CONFIG_NOT_COHERENT_CACHE) && !defined(FAM_BUF_POOL)
	return 1;
#else
	return 0;
#endif
}

static int apm_lac_init_fp(int queue_id, int buff_size, int no_of_buffs)
{
	int i;
	struct sk_buff *tmp_skb;
	phys_addr_t phy_addr;
	struct apm_qm_msg_desc fp_msg_desc;
	struct apm_qm_msg16 msg;
	int rc = 0;

	memset(&msg, 0, sizeof(msg));
	fp_msg_desc.msg = &msg;

	/* Common fields */ 
	msg.C = apm_lac_pkt_iscoherent();
	msg.BufDataLen = APM_LAC_POOL_SIZE;	/* Set buffer size */ 
	msg.FPQNum = queue_id;
	msg.RType = APM_QM_CLASS_RTYPE;
	
	fp_msg_desc.qid = queue_id; 
	fp_msg_desc.mb_id = queue_id;
	fp_msg_desc.msg = &msg;

	/* Program all the free buffer pools */
	for (i = 0; i < no_of_buffs; i++) {
		tmp_skb = dev_alloc_skb(buff_size + APM_LAC_SKB_RESERVE_SIZE);
		if (unlikely(!tmp_skb)) {
			APMLAC_ERR(KERN_ERR 
				"Failed to allocate new skb size %d",
			      	buff_size);
			return -ENOMEM;
		}
		skb_reserve(tmp_skb, APM_LAC_SKB_RESERVE_SIZE);

		msg.UserInfo = (u32) tmp_skb;
		
		/* Convert to physical address and program free buffer pool */
		phy_addr = virt_to_phys(tmp_skb->data);
		msg.DataAddrMSB = (u32) (phy_addr >> 32);
		msg.DataAddrLSB = (u32) phy_addr;

#if defined CONFIG_NOT_COHERENT_CACHE && !defined CONFIG_APM86xxx_IOCOHERENT
		invalidate_dcache_range((u32) tmp_skb->data,
			(u32) tmp_skb->data + buff_size);
#endif 
		/* Fill with the new buffer address */
		if (unlikely((apm_qm_fp_dealloc_buf(&fp_msg_desc)) != 0)) {
			APMLAC_ERR(KERN_ERR 
			       "Can not allocate QM FP buffer\n");
			return -1;
		}
	}
	return rc;
}

static int apm_lac_replenish_fp(int queue_id, int size)
{
	struct apm_qm_msg_desc fp_msg_desc;
	struct apm_qm_msg16 msg;
	struct sk_buff *tmp_skb;
	phys_addr_t phy_addr;

	fp_msg_desc.msg = &msg;
	fp_msg_desc.qid = queue_id; 
	fp_msg_desc.mb_id = queue_id;

	tmp_skb = dev_alloc_skb(size + APM_LAC_SKB_RESERVE_SIZE);
	if (unlikely(!tmp_skb)){
		APMLAC_ERR(KERN_ERR "Failed to allocate new skb size %d\n",
			size);
		return -ENOMEM;
	}
	skb_reserve(tmp_skb, APM_LAC_SKB_RESERVE_SIZE);

	phy_addr = virt_to_phys(tmp_skb->data);
	
	/* Program individual WORD to avoid use of memzero */
	((u32 *) &msg)[0] = apm_lac_pkt_iscoherent() << 23 | 
				(APM_LAC_POOL_SIZE << 8) | 
				(u32) (phy_addr >> 32);
	((u32 *) &msg)[1] = (u32) phy_addr;
	((u32 *) &msg)[2] = APM_QM_CLASS_RTYPE << 24 | queue_id;
	((u32 *) &msg)[3] = (u32) tmp_skb;

#if defined CONFIG_NOT_COHERENT_CACHE && !defined CONFIG_APM86xxx_IOCOHERENT
	invalidate_dcache_range((u32) tmp_skb->data,
		(u32) tmp_skb->data + size);
#endif
	/* Fill with the new buffer address */
	if (unlikely((apm_qm_fp_dealloc_buf(&fp_msg_desc)) != 0)) {
		kfree_skb(tmp_skb);
		APMLAC_ERR(KERN_ERR "Can not replenish FP buffer\n");
		return -1;
	}
	lac_st.fp_dealloc_cnt++;
	return 0;
}

static int apm_lac_start_hw(struct apm_ptree_config* lac_cfg)
{
	int rc = 0;
	u32 data;
	int core_id = apm_processor_id();
	
	if ((shared_mode && core_id))
		return 0;

	APMLAC_DEBUG("Ptree Config snptr:%d, sptr:%d, default_result:%d,"
     		"SE:%d, nh:%d \n",
	       	lac_cfg->start_node_ptr, lac_cfg->start_pkt_ptr,
	      	lac_cfg->default_result, lac_cfg->start_engine,
	       	lac_cfg->max_hop);
	
	/* snptr */
	data = SNPTR2_WR(lac_cfg->start_node_ptr);
	rc = apm_lac_wr32(SNPTR2_ADDR, data);
	
	/* spptr */ 
	data = SPPTR0_WR(lac_cfg->start_pkt_ptr);
#ifdef CARBON_MODEL  /* FIXME */
	data = data + 4;
#endif		      
	rc |= apm_lac_wr32(SPPTR2_ADDR, data);
	
	/* Tree Max Hops */ 
	data = TMAXHOP0_WR(lac_cfg->max_hop);
	rc |= apm_lac_wr32(TMAXHOP2_ADDR, data);
	
	if (lac_cfg->start_engine == 0) {
		data = PARSER_HALT2_WR(1);
		rc |= apm_lac_wr32(PARSER_CTL2_ADDR, data);
		lac_state = 0;
	} else {
		lac_state = 1;
	}
	/* Assign Default Classification DB Ptr */
	if (lac_cfg->default_result) {
		APMLAC_DEBUG("Assign Default Classification DB Ptr \n");
		data = DFCLSRESDBPTR2_WR(lac_cfg->default_result);
		rc |= apm_lac_wr32(DFCLSRESDBPTR2_ADDR, data);
	}
	/* Enable drop if FP not available */
	rc |= apm_lac_rd32(RSIF_CFG_CTL_ADDR, &data);
	data = CFG_RSIF_RTYPE_SET(data, 0);
	data |= FPBUFF_DROP_EN_WR(1);
	rc |= apm_lac_wr32(RSIF_CFG_CTL_ADDR, data);
	return rc;
}
static int apm_lac_qconfig(void)
{
	struct apm_qm_qstate *lac_qstates, *tmp_qstate;
	struct apm_qm_qalloc lac_qalloc;
	int core = 0;
	int rc = 0;
	int core_id = apm_processor_id();
	
	/* Allocate queue state memory */
	lac_qstates = kmalloc(sizeof(struct apm_qm_qstate), GFP_KERNEL);
	if (lac_qstates == NULL) {
		APMLAC_ERR(KERN_ERR "Unable to allocate memory for"
				"queues state \n");
		return -ENOMEM;
	} 
	memset(&lac_qalloc, 0, sizeof(struct apm_qm_qalloc));
	
#ifndef CONFIG_SMP
	core = 1;
#endif 

	if (!(shared_mode && core_id)) {
		/* Allocate egress work queue from PPCx to LACx */
		memset(lac_qstates, 0, sizeof(struct apm_qm_qstate));
		lac_qalloc.qstates = lac_qstates;
		lac_qalloc.qm_ip_blk = IP_BLK_QM;
		lac_qalloc.ip_blk = IP_CLASS;
		lac_qalloc.ppc_id = 0;
		lac_qalloc.q_type = P_QUEUE;
		lac_qalloc.q_count = 1;
		lac_qalloc.direction = DIR_EGRESS;
		lac_qalloc.qsize = SIZE_16KB;
		lac_qalloc.thr_set = 1;
		
		if ((rc = apm_qm_alloc_q(&lac_qalloc)) != 0) {
			APMLAC_ERR(KERN_ERR "Error allocate egress work"
				   "queues for LAC \n");
			goto done;
		}
		lac_queues[0].tx_qid = lac_qstates->q_id;
		lac_queues[1].tx_qid = lac_qstates->q_id;
	}
	
	APMLAC_DEBUG("Got QID for ppc: %d, TX to Lac: %d \n",
       		core_id, lac_queues[core_id].tx_qid);
	
	for( ; core < MAX_CORES; core++) {
		
#ifdef CONFIG_SMP
	core_id = core;
#endif 				
		/* Allocate ingress queue from LAC to PPCx */
		memset(lac_qstates, 0, sizeof(struct apm_qm_qstate));
		lac_qalloc.qstates = lac_qstates;
		lac_qalloc.direction = DIR_INGRESS;
		lac_qalloc.ppc_id = core_id;

		if ((rc = apm_qm_alloc_q(&lac_qalloc)) != 0) {
			APMLAC_ERR(KERN_ERR 
				"Error allocate ingress queues for LAC "
				"to PPC%d\n", core_id);
			goto done;
		}
		lac_queues[core_id].rx_qid = lac_qstates->q_id;
		lac_queues[core_id].rx_mbid = lac_qstates->mb_id;
		APMLAC_DEBUG("Got RX QID for ppc%d from LAC: %d and MBID: %d\n", 
			   core_id, lac_queues[core_id].rx_qid,
			   lac_queues[core_id].rx_mbid);

		/* Allocate completion queue from LAC to PPCx */
		tmp_qstate = get_comp_queue(IP_CLASS, core_id);
		memcpy(lac_qstates, tmp_qstate, sizeof(struct apm_qm_qstate));
		
		lac_queues[core_id].comp_qid = lac_qstates->q_id;
		lac_queues[core_id].comp_mb = lac_qstates->mb_id;
		APMLAC_DEBUG("Got COMP QID for ppc%d from LAC: %d and"
		     	"MBID: %d\n", 
			core_id, lac_queues[core_id].comp_qid,
			lac_queues[core_id].comp_mb);
		/* FIXME - Disable interrupts for completion queue mailboxes */
		apm_qm_disable_mb_irq(lac_queues[core_id].comp_mb);

		/* Allocate free pool for LAC from PPCx */
		memset(lac_qstates, 0, sizeof(struct apm_qm_qstate));
		lac_qalloc.qstates = lac_qstates;
		lac_qalloc.ip_blk = IP_CLASS;
		lac_qalloc.q_type = FREE_POOL;
		lac_qalloc.direction = DIR_EGRESS;		
		if ((rc = apm_qm_alloc_q(&lac_qalloc)) != 0) {
			APMLAC_ERR(KERN_ERR 
				"Error allocate free pool for PPC%d\n",
				core_id);
			goto done;
		}
		lac_queues[core_id].fp_qid = lac_qstates->q_id;
		lac_queues[core_id].fp_pbn = lac_qstates->pbn;
		lac_queues[core_id].fp_mbid = lac_qstates->mb_id;
		
		APMLAC_DEBUG("Core:%d<=>LAC: RX QID %d RX MB %d TX QID %d "
			"CQID %d CMB %d FP %d FP PBN %d\n", core_id, 
			lac_queues[core_id].rx_qid, 
			lac_queues[core_id].rx_mbid,
			lac_queues[core_id].tx_qid, 
			lac_queues[core_id].comp_qid,
			lac_queues[core_id].comp_mb, 
			lac_queues[core_id].fp_qid,
			lac_queues[core_id].fp_pbn);
	}
done:
	kfree(lac_qstates);	
	return rc;	
}

void apm_lac_dump_stats(void)
{
	printk("Look Aside Classifier (LAC) stats: \n"); 
	printk("req_sent_cnt: %d \n", lac_st.req_sent_cnt);
	printk("res_rcvd_cnt: %d \n", lac_st.res_rcvd_cnt);
	printk("comp_msg_cnt: %d \n", lac_st.comp_msg_cnt);
	printk("fp_dealloc_cnt: %d \n", lac_st.fp_dealloc_cnt);
	printk("lerr_cnt: %d \n", lac_st.lerr_cnt);
	printk("pkt_drop_cnt: %d \n",
	       (lac_st.req_sent_cnt - lac_st.res_rcvd_cnt));
}

/* Send lookup request to LAC  */
int apm_lac_send_req(struct sk_buff *skb)
{
	phys_addr_t paddr;
	struct apm_qm_msg_desc tx_msg_desc;
	struct apm_qm_msg64 msg;
	struct sk_buff *skb_to_free; 
	u32 skbval;

	struct apm_qm_msg16 *msg16;
	struct apm_qm_msg_up8 *msg8_1;
	int core_id = apm_processor_id();
	u32 nr_frags = skb_shinfo(skb)->nr_frags;

	paddr = virt_to_phys((volatile void *) skb->data);

	APMLAC_TXLOG("TX Frame PADDR: 0x%010llx VADDR: 0x%p len: %d frag %d core:%d \n",
		     paddr, skb->data, skb->len, nr_frags, core_id);

	/* Packet not fragmented */
	if (likely(nr_frags == 0)) {
		memset(&msg.msg32_1, 0, sizeof(msg.msg32_1));
		msg16 = &msg.msg32_1.msg16;
		msg8_1 = &msg.msg32_1.msgup8_1;
			
		/* Prepare QM message */
		msg16->BufDataLen = skb->len;

		msg16->DataAddrMSB = (u32) (paddr >> 32);
		msg16->DataAddrLSB = (u32) paddr;

		APMLAC_TXDUMP("TX ", skb->data, skb->len);
	} else if (nr_frags < 6) {
		struct apm_lac_msg_ext8 *ext_msg;
		int i;
		memset(&msg, 0, sizeof(msg));
		msg16 = &msg.msg32_1.msg16;
		msg8_1 = &msg.msg32_1.msgup8_1;
		ext_msg = (struct apm_lac_msg_ext8*) &msg.msg32_2;
		/* First Fragment */
		msg16->BufDataLen = skb_headlen(skb);
		msg16->DataAddrMSB = (u32) (paddr >> 32);
		msg16->DataAddrLSB = (u32) paddr;
		/* Its 64B msg */
		msg16->NV = 1;
		
		APMLAC_TXDUMP("TX ", skb->data, msg16->BufDataLen);

		for (i = 0; i < nr_frags; i++) {
			skb_frag_t *frag = &skb_shinfo(skb)->frags[i];
			void *addr;
			
			addr  = page_address(frag->page) + frag->page_offset;
			paddr = virt_to_phys(addr);
			ext_msg->NxtBufDataLength = frag->size;
			ext_msg->NxtDataAddrMSB = (u32) (paddr >> 32);
			ext_msg->NxtDataAddrLSB = (u32) paddr;
			APMLAC_TXLOG(
				"Frag[%d] len %d PADDR 0x%010llx VADDR 0x%p\n",
				i, frag->size, paddr, addr);
			APMLAC_TXDUMP("TX ", addr, frag->size);
#if defined(CONFIG_NOT_COHERENT_CACHE)
			flush_dcache_range((u32) addr,
					(u32) addr + frag->size);
#endif 
			ext_msg++;
		}
		if (nr_frags < 5)	/* Marking the end of buffers */
			ext_msg->NxtBufDataLength = 0x7800;
	} else {
		struct apm_lac_msg_ext8 *ext_msg;
		struct apm_lac_msg_ext8 *ext_msg_ll;
		int i, j;
		u32 byte_cnt = 0;
		int ll_cnt = nr_frags - 4;
		u32 *tmp;
		if (unlikely(nr_frags > APM_LAC_MAX_FRAG)) {
			APMLAC_TXLOG("No of fragments exceeding LL size\n");
			return -EINVAL;
		}
		memset(&msg, 0, sizeof(msg));
		msg16 = &msg.msg32_1.msg16;
		msg8_1 = &msg.msg32_1.msgup8_1;
		ext_msg = (struct apm_lac_msg_ext8*) &msg.msg32_2;
		/* First Fragment */
		msg16->BufDataLen = skb_headlen(skb);
		msg16->DataAddrMSB = (u32) (paddr >> 32);
		msg16->DataAddrLSB = (u32) paddr;
		msg16->UserInfo = (u32) skb;
		/* Its 64B msg */
		msg16->NV = 1;
		/* It has LL */
		msg16->LL = 1;
				
		for (i = 0; i < 4; i++) {
			skb_frag_t *frag = &skb_shinfo(skb)->frags[i];
			void *addr;
			ext_msg->NxtBufDataLength = frag->size;
			addr = page_address(frag->page) + frag->page_offset;
			paddr = virt_to_phys(addr);			
			ext_msg->NxtDataAddrMSB = (u32) (paddr >> 32);
			ext_msg->NxtDataAddrLSB = (u32) paddr;
			APMLAC_TXLOG(
				"NxtBufDataLength:%d, NxtDataAddrLSB:%x\n",
				ext_msg->NxtBufDataLength, 
				ext_msg->NxtDataAddrLSB);
#if defined(CONFIG_NOT_COHERENT_CACHE)
			flush_dcache_range((u32) addr,
					(u32) addr + frag->size);
#endif 
			ext_msg++;
		}
		ext_msg_ll = kmalloc(ll_cnt * sizeof(struct apm_lac_msg_ext8),
			GFP_KERNEL);
		if (ext_msg_ll == NULL) {
			APMLAC_TXLOG("Could not allocate memory for Lac"
				" fragments LL\n");
			return -ENOMEM;
		}
		/* Info for LL buffers */
		paddr = virt_to_phys(ext_msg_ll);
		ext_msg->NxtDataAddrMSB = (u32) (paddr >> 32);
		ext_msg->NxtDataAddrLSB = (u32) paddr;
		for (j = 0, i = 4; i < nr_frags; i++, j++) {
			skb_frag_t *frag = &skb_shinfo(skb)->frags[i];
			void *addr;
			byte_cnt += frag->size;
			ext_msg_ll->NxtBufDataLength = frag->size;
			addr = page_address(frag->page) + frag->page_offset;
			paddr = virt_to_phys(addr);
			ext_msg_ll->NxtDataAddrMSB = (u32) (paddr >> 32);
			ext_msg_ll->NxtDataAddrLSB = (u32) addr;
			APMLAC_TXLOG(
				"NxtBufDataLength:%d, NxtDataAddrLSB:%x\n",
				ext_msg_ll->NxtBufDataLength,
				ext_msg_ll->NxtDataAddrLSB);
#if defined(CONFIG_NOT_COHERENT_CACHE)
			flush_dcache_range((u32) addr,
					(u32) addr + frag->size);
#endif 
			ext_msg_ll++;
		}
		tmp = (u32 *) ext_msg;
		*tmp = (byte_cnt << 8) | (ll_cnt);
	}
	
	/* Common message Fields */
	msg16->C = apm_lac_pkt_iscoherent();
	msg16->UserInfo = (u32) skb;
	
	msg8_1->HE = 1;
	msg8_1->H0Enq_Num = lac_queues[core_id].comp_qid;
 	
	tx_msg_desc.qid = lac_queues[core_id].tx_qid;
	tx_msg_desc.msg = &msg;	

	/* Flush here as it is referenced multiple place */
#if defined(CONFIG_NOT_COHERENT_CACHE)
	if (likely(nr_frags == 0))
		flush_dcache_range((u32) skb->data, 
				(u32) skb->data + skb->len);
	else 
		flush_dcache_range((u32) skb->data, 
				(u32) skb->data + skb_headlen(skb)); 
#endif
	APMLAC_TXLOG("TX CQID %d QID %d len %d\n",
		msg8_1->H0Enq_Num, tx_msg_desc.qid, msg16->BufDataLen);
	APMLAC_TXDUMP("TX msg ", tx_msg_desc.msg, msg16->NV ? 64 : 32);

	lac_st.req_sent_cnt++;
	apm_qm_push_msg(&tx_msg_desc);
	
	APMLAC_TXLOG("Checking completion on queue: %d mailbox: %d\n",
			lac_queues[core_id].comp_qid, 
			lac_queues[core_id].comp_mb);
	while ((skbval =
		apm_qm_pull_comp_msg(lac_queues[core_id].comp_mb)) != -1) {

		lac_st.comp_msg_cnt++;
		/* Retrieve skb address */
		skb_to_free = (struct sk_buff *) skbval;
		APMLAC_TXLOG("Completion msg recieved, skb_to_free: 0x%p \n",
			     skb_to_free);
		if (likely(skb_to_free != NULL)) {
			APMLAC_TXLOG("Freeing SKB in TX module: 0x%p\n", 
				   	skb_to_free);
			dev_kfree_skb_any(skb_to_free);
		} else {
			APMLAC_ERR(KERN_ERR 
			     	"Error retriving msg from comp queue\n");
			break;
		}
	}
	return 0;
}

/* Message from LAC */
int apm_lac_rx_msg(struct apm_qm_msg_desc *rx_msg_desc)
{
	struct apm_qm_msg32 *msg = (struct apm_qm_msg32 *) rx_msg_desc->msg;
	struct apm_qm_msg16 *msg16 = &msg->msg16;
	struct sk_buff *skb;
	int rc = 0;
	int core_id = apm_processor_id();
#ifdef 	APM_LAC_RXDEBUG
	struct apm_qm_msg_up8 *msg8_1 = &msg->msgup8_1;
	u32 data_len = (msg16->BufDataLen & 0xFFF);
#endif
	lac_st.res_rcvd_cnt++;
	if (unlikely(msg16->LErr)) {
		APMLAC_ERR(KERN_ERR "LAC error code %d QID %d\n", 
			msg16->LErr, rx_msg_desc->qid);
		lac_st.lerr_cnt++;
		return -1;
	}
	skb = (struct sk_buff *) msg16->UserInfo;
	if (skb == NULL) {
		APMLAC_ERR("SKB buffer address NULL\n");
		apm_lac_send_req(skb);
		return 0;
	}
	APMLAC_RXLOG("Rcvd pkt of len:%d and hoInfo_msb:0x%x "
	  	"and hoInfo_lsb:0x%x",
	  	data_len, msg8_1->H0Info_msb, msg8_1->H0Info_lsb);
	APMLAC_RXDUMP("Received MSG:", msg, msg16->NV ? 64 : 32);
	APMLAC_RXDUMP("Received DATA:", skb->data, data_len);

	apm_lac_replenish_fp(lac_queues[core_id].fp_qid, APM_LAC_BUF_SIZE);
	return rc;
}

int apm_lac_hw_reset(void)
{
	struct clk *clk;
	int core_id = apm_processor_id();
	
	if ((shared_mode && core_id))
		return 0;
	
	/* Reset security hardware */
	clk = clk_get(NULL, "lac");
	if (IS_ERR(clk)) {
		APMLAC_ERR("ERROR in getting the Clock \n");
		return -ENODEV;
	}
	clk_enable(clk);
	return 0;
}

#ifdef CONFIG_PM
static int apm_lac_suspend(struct of_device * dev, pm_message_t state)
{
	lac_state = 0;
	APMLAC_LOG("Look aside Clasifier Driver suspended \n");
	return 0;
}

static int apm_lac_resume(struct of_device* dev)
{
	int rc = 0;

	if (!resumed_from_deepsleep())
		return rc;

	APMLAC_LOG("Look aside Clasifier Driver resumed \n");
	rc = apm_lac_hw_reset();
	rc |= apm_lac_start_hw(&lac_cfg);
	lac_state = 1;
	
	return rc;
}
#else
#define apm_lac_suspend NULL
#define apm_lac_resume	NULL
#endif /* CONFIG_PM */


static int __devinit apm_lac_probe(struct of_device *ofdev,
				const struct of_device_id *match)
{
	int rc = 0;
	struct device_node *np = ofdev->node;
	u32 *val;
	u32 val_len;
	struct resource res;
	u32 csr_addr_size;
	//int core_id = apm_processor_id();
	
	APMLAC_LOG("Loaded APM 862xx SoC Look aside Clasifier Driver \n");

	/* Retrieve Classifier CSR register address and size */
	if ((rc = of_address_to_resource(np, 0, &res)) != 0) {
		APMLAC_ERR(KERN_ERR
			"Unable to retrive Look aside Classifier csr"
			"addr from DTS\n");
		return rc;
	}
	apm_lac_base_addr_p = res.start;
	csr_addr_size = RES_SIZE(&res);
	apm_lac_base_addr = (u32) ioremap_nocache(apm_lac_base_addr_p,
						csr_addr_size);
	APMLAC_DEBUG("Look aside Classifier PADDR: 0x%010llx VADDR: 0x%08X\n",
		apm_lac_base_addr_p, apm_lac_base_addr);

	val = (u32 *) of_get_property(np, "sharemode", &val_len);
	if (val == NULL) 
		shared_mode = 0;
	else
		shared_mode = *val ? 1 : 0;

	/* Reset LAC */		
	apm_lac_hw_reset();
	
	/* Queue Allocation */
	apm_lac_qconfig();
	
	/* Configure free pool buffers */
	apm_lac_init_fp(lac_queues[0].fp_qid,
	       		APM_LAC_BUF_SIZE, APM_LAC_NO_PKT_BUF);

	/* Register QM callback function for Ethernet */
	apm_qm_msg_rx_register(APM_QM_CLASS_RTYPE, apm_lac_rx_msg);

	APMLAC_DEBUG("Start LAC HW \n");
	rc |= apm_lac_start_hw(&lac_cfg);
	
	return rc;
}

static int __devexit apm_lac_remove(struct of_device *dev)
{
	APMLAC_LOG("Unloaded APM 862xx SoC Look aside Clasifier Driver");
	return 0;
}

/* Called when system is shutwon */
static int apm_lac_shutdown(struct of_device *ofdev)
{
	int rc = 0;
	return rc;

}

static struct of_device_id apm_lac_match[] = {
	{ .compatible	   = "apm,apm86xxx-lac", },
	{ },
};

static struct of_platform_driver apm_lac_driver = {
	.name		= "apm-lac",
	.match_table	= apm_lac_match,
	.probe		= apm_lac_probe,
	.remove		= apm_lac_remove,
#if defined(CONFIG_PM)	
	.suspend	= apm_lac_suspend,
	.resume		= apm_lac_resume,
#endif
	.shutdown	= apm_lac_shutdown,
};

static int __init apm_lac_init(void)
{
	return of_register_platform_driver(&apm_lac_driver);
}

static void __exit apm_lac_exit(void)
{
	of_unregister_platform_driver(&apm_lac_driver);
}

module_init(apm_lac_init);
module_exit(apm_lac_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mahesh Pujara <mpujara@apm.com>");
MODULE_DESCRIPTION("Driver for APM 862xx SoC Lookaside Classifier engine");
