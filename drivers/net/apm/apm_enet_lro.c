/**
 * AppliedMicro APM86xxx SoC Ethernet LRO Driver
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
 * @file apm_enet_lro.c
 *
 * APM86xxx Ethernet LRO implementation for APM86xxx SoC.
 */
#include <asm/cacheflush.h>
#include <linux/etherdevice.h>
#include <net/ip.h>
#include <net/tcp.h>
#include <asm/ipp.h>
#include <asm/apm_slimpro_offload.h>
#include "apm_enet_access.h"
#include "apm_enet_lro.h"

#ifdef CONFIG_APM_ENET_LRO

#define LROID			"LRO: "

static int apm_enet_lro_hw_q_init(struct apm_enet_dev_base *priv);
static int apm_enet_lro_hw_q_deinit(struct apm_enet_dev_base *priv);

int apm_enet_lro_enable(struct apm_enet_dev_base *priv, u8 enable)
{
	/* Enable or disable LRO */
	if (priv->lro.enable == enable)
		return 0;
	priv->lro.enable = enable;
	if (enable) {
		apm_enet_lro_hw_q_init(priv);
	} else {
		apm_enet_lro_hw_q_deinit(priv);
	}			
	return 0;
}

int apm_enet_lro_set_port(struct apm_enet_dev_base *priv, u16 *port_list)
{
	memcpy(priv->lro.port, port_list, sizeof(u16) * LRO_MAX_PORT);
	return 0;
}

int apm_enet_lro_set_timeout(struct apm_enet_dev_base *priv, u32 timeoutms)
{
	int rc;
	
	/* Send timeout to SlimPRO */
	if (priv->lro.ready) {
		rc = ipp_send_user_msg(IPP_CONFIG_SET_HDLR, SLIMPRO_LRO, 
				SLIMPRO_LRO_TIMEOUT, CTRL_BYTE_UNSED, 
				timeoutms);
		if (rc < 0)
			return rc;
	}
	priv->lro.timeout_ms = timeoutms;
	return 0;
}

int apm_enet_lro_set_maxbytecnt(struct apm_enet_dev_base *priv, u32 maxbytecnt)
{	
	int rc;
	
	/* Send byte count to SlimPRO */
	if (priv->lro.ready) {
		rc = ipp_send_user_msg(IPP_CONFIG_SET_HDLR, SLIMPRO_LRO, 
				SLIMPRO_LRO_BYTECNT, CTRL_BYTE_UNSED, 
				maxbytecnt);
		if (rc < 0)
			return rc;
	}
	priv->lro.max_byte_cnt = maxbytecnt;
	return 0;
}

static int apm_net_lro_ready_cb(struct apm_enet_dev_base *priv)
{
	priv->lro.ready = 1;
	apm_enet_lro_set_timeout(priv, priv->lro.timeout_ms);
	apm_enet_lro_set_maxbytecnt(priv, priv->lro.max_byte_cnt);
	return 0;
}

static int apm_enet_lro_slimpro_event(struct notifier_block *this,
                             	unsigned long event, void *ptr)
{
	struct apm_enet_lro_ctx	*ctx;
	struct apm_enet_dev_base *priv;
	
	ENET_DEBUG_LRO("LRO SlimPRO notify ready\n");
	ctx = container_of(this, struct apm_enet_lro_ctx, nb);
	priv = container_of(ctx, struct apm_enet_dev_base, lro);
	apm_net_lro_ready_cb(priv);
        return NOTIFY_DONE;
}

int apm_enet_lro_init(struct apm_enet_dev_base *priv)
{
	struct apm_enet_lro_ctx	*lro_ctx = &priv->lro;
	int i;
	
	/* Assume LRO not available */
	lro_ctx->lro_available = 0;
	
	/* Check if SlimPRO is available */
	if (is_apm86xxx_lite()) 
		return -ENODEV;
	
	/* Check if SlimPRO has LRO feature */
	/* FIXME */
	
	lro_ctx->ready = 0;	/* ready when SlimPRO firmware loaded */
	lro_ctx->lro_available = 1;
	lro_ctx->enable = 0;
	memset(lro_ctx->port, 0, sizeof(u16) * LRO_MAX_PORT);
	lro_ctx->timeout_ms = 1;		/* Default */
	lro_ctx->max_byte_cnt = 32*1024;	/* Default */
	for (i = 0; i < MAX_CORES; i++)
		lro_ctx->ppc_lro_wqid[i] = QM_CFG_INVALID;
	lro_ctx->slimpro_dma_wqid = QM_CFG_INVALID;
	lro_ctx->slimpro_dma_cqid = QM_CFG_INVALID;
	lro_ctx->slimpro_wqid = QM_CFG_INVALID;
	lro_ctx->slimpro_hdr_fqid = QM_CFG_INVALID;
	lro_ctx->slimpro_data_fqid = QM_CFG_INVALID;
	
	/* Register for SlimPRO readiness */
	lro_ctx->nb.notifier_call = apm_enet_lro_slimpro_event;
	register_ipp_fwload_notifier(&lro_ctx->nb);
	
	printk(KERN_INFO "APM86xxx LRO initialized\n");
	return 0;
}

static int apm_enet_lro_cfg_buf(int queue_id, int size, int hdr_rsvd)
{
	struct apm_qm_msg_desc fp_msg_desc;
	struct apm_qm_msg16 msg;
	struct sk_buff *tmp_skb;
	phys_addr_t phy_addr;

	fp_msg_desc.msg = &msg;
	fp_msg_desc.qid = queue_id; 
	fp_msg_desc.mb_id = queue_id;

	tmp_skb = dev_alloc_skb(size + hdr_rsvd + 
			sizeof(struct sk_buff *) + LRO_SLIMPRO_ALIGN);
	if (unlikely(!tmp_skb)) {
		printk(KERN_ERR "Failed to allocate new skb size %d\n",
			size);
		return -ENOMEM;
	}
	skb_reserve(tmp_skb, hdr_rsvd + LRO_SLIMPRO_ALIGN);

	/* put back link in data buffer to skb */
	*(u32 *) (tmp_skb->data + size) = (u32) tmp_skb;

	phy_addr = virt_to_phys(tmp_skb->data);
	
	/* Program individual WORD to avoid use of memzero */
	((u32 *) &msg)[0] = apm_enet_pkt_iscoherent() << 23 | 
				(APM_PKT_POOL_SIZE << 8) | 
				(u32) (phy_addr >> 32);
	((u32 *) &msg)[1] = (u32) phy_addr;
	((u32 *) &msg)[2] = APM_QM_ETH_RTYPE << 24 | queue_id;
	((u32 *) &msg)[3] = (u32) tmp_skb;

#if defined(CONFIG_NOT_COHERENT_CACHE)
	invalidate_dcache_range((u32) tmp_skb->data,
				(u32) tmp_skb->data + size);
#endif 
	/* Fill with the new buffer address */
	if (unlikely((apm_qm_fp_dealloc_buf(&fp_msg_desc)) != 0)) {
		kfree_skb(tmp_skb);
		printk(KERN_ERR "Can not replenish FP buffer\n");
		return -1;
	}
	return 0;
}

int apm_enet_lro_rx_frame(struct apm_qm_msg_desc *rx_msg_desc)
{
	struct apm_qm_msg32 *msg = (struct apm_qm_msg32 *) rx_msg_desc->msg;
	struct apm_qm_msg16 *msg16 = &msg->msg16;
	struct apm_lro_chain *chptr;
	int i;
	int num_bufs;
	struct iphdr *iph;
	struct tcphdr *tcp;
	struct sk_buff *hdr_skb;
	struct sk_buff *data_skb;
	struct sk_buff *last_skb;
	struct apm_enet_dev_base *priv_dev;
	u64 paddr;

	priv_dev = apm_enet_qid2ctx(rx_msg_desc->qid);

	/* Get address of hdr buffer */
	paddr = (u64) msg16->DataAddrMSB << 32 | msg16->DataAddrLSB;
	chptr = phys_to_virt(paddr);
	ENET_DEBUG_LRO("LRO header 0x%02X.%08X desc 0x%p",
			msg16->DataAddrMSB, msg16->DataAddrLSB, chptr);

	/* Get link list of data buffer to hdr skb */
	hdr_skb = (struct sk_buff *) *(u32 *) ((u8 *) chptr + APM_PKT_BUF_SIZE);
	ENET_DEBUG_LRO("LRO skb 0x%p", hdr_skb);
	
	/* Advance to the header data pointer */
	skb_reserve(hdr_skb, sizeof(u32) + 2 * sizeof(u16));
	
	/* Set size of hdr data */
	skb_put(hdr_skb, chptr->hdr_len);
	iph = (struct iphdr *) (hdr_skb->data + 14);
	tcp = (struct tcphdr *) ((char *) iph + ((iph->ihl & 0x0f) * 4));

	last_skb = NULL;
	num_bufs = chptr->tot_buffs;
	if (chptr->tot_buffs > priv_dev->lro.max_lro_rx_buffs)
		priv_dev->lro.max_lro_rx_buffs = chptr->tot_buffs;
	skb_shinfo(hdr_skb)->gso_size = 0;
	ENET_DEBUG_LRO("lro rx hdr len %d num buffs %d iplen %d "
		"seq %u-%u len %d segs %d rmdr %d",
		chptr->hdr_len, chptr->tot_buffs, 
		*(u16 *) &chptr->hdr[16],
		*(u32 *) &chptr->hdr[38],
		*(u32 *) &chptr->hdr[38] + chptr->tot_len - 1,
		chptr->tot_len, chptr->tot_len / 1460, chptr->tot_len % 1460);
	ENET_DEBUG_LRO("    src/dst port %d %d tcp hdr len/flg 0x%04x\n",
		*(u16 *) &chptr->hdr[34], *(u16 *) &chptr->hdr[36], 
		*(u16 *) &chptr->hdr[46]);
	
	/* Add data skbs to frag_list of hdr skb */
	for (i = 0; i < num_bufs; i++) {
		u32 *ptr;
		paddr = (u64) chptr->buff[i].buff_addr_msb << 32 |
				chptr->buff[i].buff_addr_lsb;
	  	/* Get data skb pointer */
		ptr = (u32 *) phys_to_virt(paddr);
		ENET_DEBUG_LRO("LRO data %d len %d 0x%02X.%08X 0x%p", i,
			chptr->buff[i].buff_len,
	  		chptr->buff[i].buff_addr_msb, 
	  		chptr->buff[i].buff_addr_lsb, ptr);
		data_skb = (struct sk_buff *) *(u32 *)((u8 *) ptr + LRO_BUF_SIZE);
		ENET_DEBUG_LRO("LRO data skb 0x%p", data_skb);
		skb_put(data_skb, chptr->buff[i].buff_len);
		if (chptr->buff[i].buff_len > skb_shinfo(hdr_skb)->gso_size)
			skb_shinfo(hdr_skb)->gso_size = chptr->buff[i].buff_len;
		hdr_skb->len += chptr->buff[i].buff_len;
		hdr_skb->data_len += chptr->buff[i].buff_len;
		hdr_skb->truesize += data_skb->truesize;
		if (last_skb)
			last_skb->next = data_skb;
		else
			skb_shinfo(hdr_skb)->frag_list = data_skb;
		data_skb->next = NULL;
		last_skb = data_skb;
	  
	  	/* Alloc one new data skb for each one rxed in 
	  	   ENET_RX_PPC_FP_LRO_DDR_Q */
		apm_enet_lro_cfg_buf(chptr->buff[i].buff_fpq, LRO_BUF_SIZE,
				priv_dev->hdr_rsvd);
	}

	/* alloc one new hdr skb in IPP_RX_PPC_FQ_LRO_Q */
	apm_enet_lro_cfg_buf(priv_dev->lro.slimpro_hdr_fqid, 
			APM_PKT_BUF_SIZE);

	/* specify tcp checksum is unnecessary */
	hdr_skb->ip_summed = CHECKSUM_UNNECESSARY;
	hdr_skb->dev = priv_dev->ndev;
	hdr_skb->protocol = eth_type_trans(hdr_skb, priv_dev->ndev);

	/* Update ip header checksum */
	iph->check = 0;
	iph->check = ip_fast_csum((u8 *)iph, iph->ihl);

	/* Send to upper layers */
	netif_receive_skb(hdr_skb);

	return 0;
}

static int apm_enet_lro_init_fp(int queue_id, int buff_size, int no_of_buffs)
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
	msg.C = apm_enet_pkt_iscoherent();
	msg.BufDataLen = APM_PKT_POOL_SIZE;	/* Set buffer size */ 
	msg.FPQNum = queue_id;
	msg.RType = APM_QM_ETH_RTYPE;
	
	fp_msg_desc.qid = queue_id; 
	fp_msg_desc.mb_id = queue_id;
	fp_msg_desc.msg = &msg;

	/* Program all the free buffer pools */
	for (i = 0; i < no_of_buffs; i++) {
		tmp_skb = dev_alloc_skb(buff_size + hdr_rsvd 
				+ sizeof(struct sk_buff *) + LRO_SLIMPRO_ALIGN);
		if (unlikely(!tmp_skb)) {
			printk(KERN_ERR 
				"Failed to allocate new skb size %d", 
				buff_size);
			return -ENOMEM;
		}
		skb_reserve(tmp_skb, hdr_rsvd + LRO_SLIMPRO_ALIGN);

		msg.UserInfo = (u32) tmp_skb;

		/* put back link in data buffer to skb */
		*(u32 *) (tmp_skb->data + buff_size) = (u32) tmp_skb;
		
		/* Convert to physical address and program free buffer pool */
		phy_addr = virt_to_phys(tmp_skb->data);
		msg.DataAddrMSB = (u32) (phy_addr >> 32);
		msg.DataAddrLSB = (u32) phy_addr;

#if defined CONFIG_NOT_COHERENT_CACHE 
		invalidate_dcache_range((u32) tmp_skb->data,
			(u32) tmp_skb->data + buff_size);
#endif 
		/* Fill with the new buffer address */
		if (unlikely((apm_qm_fp_dealloc_buf(&fp_msg_desc)) != 0)) {
			printk(KERN_ERR "Can not allocate QM FP buffer\n");
			return -1;
		}
	}
	return rc;
}

static int apm_enet_lro_hw_q_init(struct apm_enet_dev_base *priv)
{
	struct qm_cfg_qconfig qconfig;
	u16 core = apm_processor_id();
	int rc = 0;
		
	for ( ; core < MAX_CORES; core++) {
		/* Allocate queue for PPC Gather LRO packet */
		memset(&qconfig, QM_CFG_INVALID, sizeof(struct qm_cfg_qconfig));
		qconfig.ip    = QM_CFG_IP_QM;
		qconfig.dev   = IP_IPP;
		qconfig.ppc   = core;
		qconfig.dir   = QM_CFG_DIR_INGRESS;
		qconfig.qsize = QM_CFG_QSIZE_16KB;
		qconfig.thr   = 1;
		qconfig.qtype = QM_CFG_QTYPE_PQ;
		rc = apm_enet_add_qconfig((unsigned long) priv, &qconfig);
		if (rc != 0) {
			printk(KERN_ERR LROID 
				"Failed to configure SlimPRO offload RX queue\n");
			goto done;
		}
		priv->lro.ppc_lro_wqid[core] = qconfig.qid;
		if (!apm_enet_is_smp()) 
			break;				
	}
	
	/* Allocate free pool for LRO DMA */
	/* FIXME - Get free pool queue from PktDMA */
	
	/* Allocate work queue for SlimPRO */
	priv->lro.slimpro_wqid = slimpro_queue_request();
	if (priv->lro.slimpro_wqid == QM_CFG_INVALID) {
		rc = -ENODEV;
		printk(KERN_ERR LROID 
			"Failed to retrieve SlimPRO offload work queue\n");
		goto done;
	}
		
	/* Allocate free pool for LRO header */
	memset(&qconfig, 0, sizeof(qconfig));
	qconfig.qid   = QM_CFG_INVALID;
	qconfig.ip    = QM_CFG_IP_QM;
	qconfig.dev   = IP_IPP;
	qconfig.ppc   = 0;
	qconfig.dir   = QM_CFG_DIR_INGRESS;
	qconfig.qsize = QM_CFG_QSIZE_16KB;
	qconfig.thr   = 1;
	qconfig.qtype = QM_CFG_QTYPE_FP;
	if ((rc = qm_cfg_add_qconfig(&qconfig))) {
		printk(KERN_ERR LROID 
			"Failed to configure SlimPRO offload header FP queue\n");
		goto done;
	}
	priv->lro.slimpro_hdr_fqid = qconfig.qid;
	
	/* Fill pool with buffer */
	rc = apm_enet_lro_init_fp(priv->lro.slimpro_hdr_fqid, 
			APM_PKT_BUF_SIZE, 8 * APM_NO_PKT_BUF, priv->hdr_rsvd);
	if (rc < 0) {
		printk(KERN_ERR LROID 
			"Failed to fill SlimPRO offload header FP queue\n");
		goto done;			
	}
	
	/* Allocate free pool for LRO data */
	memset(&qconfig, 0, sizeof(qconfig));
	qconfig.qid   = QM_CFG_INVALID;
	qconfig.ip    = QM_CFG_IP_QM;
	qconfig.dev   = IP_IPP;
	qconfig.ppc   = 0;
	qconfig.dir   = QM_CFG_DIR_INGRESS;
	qconfig.qsize = QM_CFG_QSIZE_16KB;
	qconfig.thr   = 1;
	qconfig.qtype = QM_CFG_QTYPE_FP;
	if ((rc = qm_cfg_add_qconfig(&qconfig))) {
		printk(KERN_ERR LROID 
			"Failed to configure SlimPRO offload data FP queue\n");
		goto done;
	}
	priv->lro.slimpro_data_fqid = qconfig.qid;
	/* Fill pool with buffer */
	rc = apm_enet_lro_init_fp(priv->lro.slimpro_data_fqid, 
			LRO_BUF_SIZE, 8 * APM_NO_PKT_BUF, priv->hdr_rsvd);
	if (rc < 0) {
		printk(KERN_ERR LROID 
			"Failed to fill SlimPRO offload data FP queue\n");
		goto done;			
	}
		
	return 0;
	
done:	
	return rc;	
}

static int apm_enet_lro_hw_q_deinit(struct apm_enet_dev_base *priv)
{
	/* Re-program classifier to not intercept LRO packet */
	/* FIXME */
	
	/* Remove all active connect from SlimPRO */
	/* FIXME */
	
	return 0;
}

int apm_enet_lro_irq(struct apm_qm_msg_desc *rx_msg_desc)
{
	struct apm_enet_dev_base *priv;

	priv = apm_enet_qid2ctx(rx_msg_desc->qid);

#ifdef CONFIG_NAPI
	ENET_DEBUGRXTX("LRO Rx Frame interface %d in NAPI", priv->port_id);
	
	/* Schedule polling */
	if (napi_schedule_prep(&priv->lro_napi)) {
		/* Disable interrupts for this port */
		ENET_DEBUGRXTX("LRO RX disable interrupt MBID %d\n",
			   rx_msg_desc->mb_id);
		apm_qm_disable_mb_irq(rx_msg_desc->mb_id);
	
		priv->lro.poll_msg_desc.mb_id = rx_msg_desc->mb_id;
		priv->lro.poll_msg_desc.qid = rx_msg_desc->qid;

		/* Tell system we have work to be done */
		__napi_schedule(&priv->lro_napi);
	}
	return 1;
#else
	return apm_enet_lro_rx_frame(rx_msg_desc);
#endif
}

int apm_enet_lro_poll(struct napi_struct *napi, int budget)
{
	struct apm_enet_dev_base *priv_dev = 
			container_of(napi, struct apm_enet_dev_base, lro_napi);
	struct apm_enet_lro_ctx *lro_ctx = &priv_dev->lro;		
	struct apm_qm_msg_desc poll_msg_desc;
	struct apm_qm_msg64 msg;
	int cnt = 0;

	memcpy(&poll_msg_desc, &lro_ctx->poll_msg_desc, sizeof(poll_msg_desc));
	poll_msg_desc.msg = &msg;

	while (budget--) {
		if (apm_qm_pull_msg(&poll_msg_desc) == -1) {
			/* No more messages, enable int and return */
			napi_complete(napi);
			apm_qm_enable_mb_irq(poll_msg_desc.mb_id);
			return cnt;
		} else {
			apm_enet_lro_rx_frame(&poll_msg_desc);
		}
		cnt++;
	};
	return cnt;
}

#endif
