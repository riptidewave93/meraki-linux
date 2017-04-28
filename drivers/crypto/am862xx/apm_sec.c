/**
 * AppliedMicro AM862xx SoC Security Driver
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
 * @file   apm_sec.c
 *
 */
#include "apm_sec_csr.h"
#include "apm_sec_access.h"
#include "apm_sec_tkn.h"
#include "apm_sec_sa.h"
#include "apm_sec_priv.h"
#include <asm/apm_pktdma.h>
#include <linux/spinlock_types.h>

/* This function needs to be replace with an TRNG function call */
int __apm_sec_random(u32 *rand_val)
{
	static int g_rand_val = 1;
	if (g_rand_val == 1)
		APMSEC_DEBUG("Function apm_sec_random needs to be replaced!!!");
	*rand_val = ++g_rand_val;
	return 0;
}
int apm_sec_random(u32 *rand_val) 
	__attribute((weak, alias("__apm_sec_random")));

int __apm_sec_loadbuffer2qmsg(struct apm_sec_msg * sec_msg, 
				struct sec_tkn_ctx * tkn)
{
	APMSEC_ERR("This function musts be implemented. This function"
		"load the data pointer to the QM message structure.");
	return -1;
}
int apm_sec_loadbuffer2qmsg(struct apm_sec_msg * sec_msg, 
				struct sec_tkn_ctx * tkn) 
	__attribute((weak, alias("__apm_sec_loadbuffer2qmsg")));

u32 apm_sec_v2hwaddr(void *vaddr)
{
#if !defined(CONFIG_APM86xxx_IOCOHERENT)
	return (u32) (PADDR(vaddr) >> 4);
#else
	u32 offset;
	offset = (u32)vaddr - (u32)apm_sec_dma_addr();
	return (u32) ((apm_sec_dma_paddr() + offset) >> 4);
#endif
}

void * apm_sec_hwaddr2v(u32 hwaddr)
{
	u64 paddr = ((u64) hwaddr) << 4;
	return VADDR(paddr);
}

int apm_sec_hwinit(void)
{
	u32 rev_data;
	u32 dev_info;
	u32 proto_alg;
	u32 val;

	apm_sec_rd32(SEC_GLB_CTRL_CSR_BLOCK, CSR_ID_ADDR, &rev_data);
	APMSEC_DEBUG("Security ID: %02d.%02d.%02d",
		REV_NO_RD(rev_data),
		BUS_ID_RD(rev_data),
		DEVICE_ID_RD(rev_data));

	/* For AXI parameter, leave default priorities */
	
	/* Enable IRQ for core, EIP96, and XTS blocks */
	apm_sec_wr32(SEC_GLB_CTRL_CSR_BLOCK, CSR_GLB_SEC_INT_STSMASK_ADDR, 
		0xFFFFFFFF & ~(QMIMASK_MASK |
				EIP96MASK_MASK | 
				XTSMASK_MASK));
		
	/* For sec QMI block, leave default values */
	
	/* Configure XTS core */
	/* FIXME */
	
	/**
	 * Configure EIP96 core 
	 */
	
	/* For EIP96 AXI read and write max burst, leave default */
	
	/* For EIP96 AXI outstanding read and write, leave default */
	
	/* For EIP96 AXI, enable error interrupts */
	apm_sec_wr32(EIP96_AXI_CSR_BLOCK, CSR_SEC_INT_STSMASK_ADDR, 
		~0xFFFFFFFF);	
		
	/* For EIP96, configure CSR_SEC_CRYPTO_CFG_0 */
	apm_sec_rd32(EIP96_CSR_BLOCK,  CSR_SEC_CRYPTO_CFG_0_ADDR, &val);
	val &= ~TKN_RD_OFFSET_SIZE0_MASK;
	val |= TKN_RD_OFFSET_SIZE0_WR(TKN_RESULT_HDR_MAX_LEN / 4);
	val &= ~TKN_RD_PREFETCH_SIZE0_MASK;
	val |= TKN_RD_PREFETCH_SIZE0_WR(0x8); /* Set token prefetch to 32B */
	apm_sec_wr32(EIP96_CSR_BLOCK, CSR_SEC_CRYPTO_CFG_0_ADDR, val);

	/* For EIP96, configure CSR_SEC_CRYPTO_CFG_1 */
	apm_sec_rd32(EIP96_CSR_BLOCK, CSR_SEC_CRYPTO_CFG_1_ADDR, &val);
	val &= ~DIS_CTX_INTERLOCK1_MASK;
	val |= DIS_CTX_INTERLOCK1_WR(0);
	apm_sec_wr32(EIP96_CSR_BLOCK, CSR_SEC_CRYPTO_CFG_1_ADDR, val);
	
	/* For EIP96, leave CSR_SEC_CRYPTO_CFG_2 to default value */
	
	/* For EIP96, leave error mask to default */

	/* For EIP96 core, read version */
	apm_sec_rd32(EIP96_CORE_CSR_BLOCK, IPE_DEV_INFO_ADDR, &dev_info);
	apm_sec_rd32(EIP96_CORE_CSR_BLOCK, IPE_PRC_ALG_EN_ADDR, &proto_alg);
	APMSEC_DEBUG("Core ver %d.%d Proto/Alg: 0x%08X",
	       MAJOR_REVISION_NUMBER_RD(dev_info),
	       MINOR_REVISION_NUMBER_RD(dev_info),
	       proto_alg);
	       
	/* For EIP96, configure context access mode */
	apm_sec_wr32(EIP96_CORE_CSR_BLOCK, IPE_CTX_CTRL_ADDR,
		CONTEXT_SIZE_WR(0x36) 
		| SEC_ADDRESS_MODE_WR(0) 
		| SEC_CONTROL_MODE_WR(1));
	
	/* For EIP96 core, configure control status register */
	apm_sec_rd32(EIP96_CORE_CSR_BLOCK, IPE_TKN_CTRL_STAT_ADDR, &val);
	val &= ~OPTIMAL_CONTEXT_UPDATES_MASK;
	val &= ~INTERRUPT_PULSE_OR_LEVEL_MASK;
	val &= ~TIME_OUT_COUNTER_ENABLE_MASK;
	val |= OPTIMAL_CONTEXT_UPDATES_WR(0);
	val |= INTERRUPT_PULSE_OR_LEVEL_WR(1 /* Level interrupt */);
	val |= TIME_OUT_COUNTER_ENABLE_WR(0);
	apm_sec_wr32(EIP96_CORE_CSR_BLOCK, IPE_TKN_CTRL_STAT_ADDR, val);

	/* For EIP96 core, setup interrupt */
	apm_sec_wr32(EIP96_CORE_CSR_BLOCK, IPE_INT_CTRL_STAT_ADDR, 
			INPUT_DMA_ERROR_ENABLE_WR(1)
			| OUTPUT_DMA_ERROR_ENABLE_WR(1)
			| PACKET_PROCESSING_ENABLE_WR(1)
			| PACKET_TIMEOUT_ENABLE_WR(1)
			| FATAL_ERROR_ENABLE_WR(1)
			| INTERRUPT_OUTPUT_PIN_ENABLE_WR(1));
			
	/* For EIP96, seed the PRNG, KEY0, KEY1, and LRSR */
	apm_sec_random(&val);
	apm_sec_wr32(EIP96_CORE_CSR_BLOCK, IPE_PRNG_SEED_L_ADDR, val);
	apm_sec_random(&val);
	apm_sec_wr32(EIP96_CORE_CSR_BLOCK, IPE_PRNG_SEED_H_ADDR, val);
	apm_sec_random(&val);
	apm_sec_wr32(EIP96_CORE_CSR_BLOCK, IPE_PRNG_KEY_0_L_ADDR, val);
	apm_sec_random(&val);
	apm_sec_wr32(EIP96_CORE_CSR_BLOCK, IPE_PRNG_KEY_0_H_ADDR, val);
	apm_sec_random(&val);
	apm_sec_wr32(EIP96_CORE_CSR_BLOCK, IPE_PRNG_KEY_1_L_ADDR, val);
	apm_sec_random(&val);
	apm_sec_wr32(EIP96_CORE_CSR_BLOCK, IPE_PRNG_KEY_1_H_ADDR, val);
	apm_sec_random(&val);
	apm_sec_wr32(EIP96_CORE_CSR_BLOCK, IPE_PRNG_LFSR_L_ADDR, val);
	apm_sec_random(&val);
	apm_sec_wr32(EIP96_CORE_CSR_BLOCK, IPE_PRNG_LFSR_H_ADDR, val);

	apm_sec_wr32(EIP96_CORE_CSR_BLOCK, IPE_PRNG_CTRL_ADDR,
		     	SEC_ENABLE_F8_WR(1)
			| AUTO_WR(1)
			| RESULT_128_WR(1));
	return 0;
}

int apm_sec_hwstart(void)
{
	/* Start the EIP96 core */
	apm_sec_wr32(EIP96_AXI_CSR_BLOCK, CSR_SEC_CFG_ADDR, GO_MASK);
	return 0;
}

int apm_sec_hwstop(void)
{
	/* Stop the EIP96 core */
	apm_sec_wr32(EIP96_AXI_CSR_BLOCK, CSR_SEC_CFG_ADDR, 0x00000000);
	return 0;
}

int apm_sec_hwreset(void)
{
	u32 csr;
	struct clk *clk;
	
	/* Reset security hardware */
	clk = clk_get(NULL, "security");
	if (IS_ERR(clk))
		return -ENODEV;
	clk_enable(clk);

	apm86xxx_read_scu_reg(SCU_CSR_SRST1_ADDR, &csr);
	APMSEC_DEBUG("CSR SRST1 0x%08X ", csr);
	return 0;
}

void apm_sec_intr_hdlr(void)
{
	u32 int_sts;
	
	apm_sec_rd32(EIP96_AXI_CSR_BLOCK, CSR_SEC_INT_STS_ADDR, &int_sts);
	if (!int_sts)
		return; 
	if (int_sts & TKN_RD_F2_MASK) 
		APMSEC_ERR("token read error\n");	
	if (int_sts & CTX_RD_F2_MASK) 
		APMSEC_ERR("context read error\n");	
	if (int_sts & DATA_RD_F2_MASK) 
		APMSEC_ERR("data read error\n");	
	if (int_sts & DSTLL_RD_F2_MASK) 
		APMSEC_ERR("destination linked list read error error\n");  		
	if (int_sts & TKN_WR_F2_MASK) 
		APMSEC_ERR("token write error\n");	
	if (int_sts & CTX_WR_F2_MASK) 
		APMSEC_ERR("context write error\n");	
	if (int_sts & DATA_WR_F2_MASK) 
		APMSEC_ERR("data write error\n");
	if (int_sts & LERR_MASK)
		APMSEC_ERR("length error\n");
		  		
	apm_sec_wr32(EIP96_AXI_CSR_BLOCK, CSR_SEC_INT_STS_ADDR, int_sts);
}

/**
 * SA and Token Management Functions
 *
 */
struct sec_tkn_ctx *apm_sec_tkn_get(struct apm_sec_session_ctx *session, 
				u8 *new_tkn)
{
	struct sec_tkn_ctx *tkn;
	int	tkn_size;	
	unsigned long flags;
	
	spin_lock_irqsave(&session->lock, flags);
	if (!list_empty(&session->tkn_cache)) {
		struct list_head *entry = session->tkn_cache.next;
		list_del(entry);
		--session->tkn_cache_cnt;	
		spin_unlock_irqrestore(&session->lock, flags);
		tkn = list_entry(entry, struct sec_tkn_ctx, next);
		*new_tkn = 0;
		tkn->esp_src_len = 0;
		tkn->qmsg = NULL;
		APMSEC_SATKNLOG("allocate tkn cached 0x%p", tkn);
		return tkn;
	}
	spin_unlock_irqrestore(&session->lock, flags);

	*new_tkn = 1;
	tkn_size = session->tkn_max_len;
	tkn = APMSEC_COH_ALLOC(tkn_size);
	/* tkn = (struct sec_tkn_ctx *)apm_sec_tkn_addr(); */
	if (tkn == NULL)
		goto done;
	memset(tkn, 0, tkn_size);
	
	tkn->input_tkn_len  = session->tkn_input_len;
	tkn->result_tkn_ptr = TKN_CTX_RESULT_TKN_COMPUTE(tkn);
	tkn->context = session;
	tkn->qmsg = NULL;
	APMSEC_SATKNLOG("allocate tkn 0x%p size %d (%d)", 
			tkn, tkn_size, session->tkn_input_len);
done:
	return tkn;
}

void apm_sec_tkn_free(struct apm_sec_session_ctx *session, 
			struct sec_tkn_ctx *tkn)
{
	unsigned long flags;

	if (tkn->qmsg)
		apm_sec_qmsg_free(session, tkn->qmsg);
	if (tkn->dest_mem)
		APMSEC_FREE(tkn->dest_mem);
	spin_lock_irqsave(&session->lock, flags);
	if (session->tkn_cache_cnt >= APM_SEC_TKN_CACHE_MAX) {	
		spin_unlock_irqrestore(&session->lock, flags);
		APMSEC_SATKNLOG("free tkn 0x%p", tkn);	
		APMSEC_COH_FREE(tkn);
		return;
	}
	++session->tkn_cache_cnt;
	list_add(&tkn->next, &session->tkn_cache);
	spin_unlock_irqrestore(&session->lock, flags);
	APMSEC_SATKNLOG("free tkn cached 0x%p", tkn);
}
#define SA_ALLOC_SIZE  (sizeof(struct sec_sa_item) + 15 + 8)
struct sec_sa_item *apm_sec_sa_get(struct apm_sec_session_ctx *session)
{
	struct sec_sa_item *sa;
	int	sa_size;
	unsigned long flags;
	
	spin_lock_irqsave(&session->lock, flags);
	if (!list_empty(&session->sa_cache)) {
		struct list_head *entry = session->tkn_cache.next;
		list_del(entry);
		--session->sa_cache_cnt;
		spin_unlock_irqrestore(&session->lock, flags);
		sa = list_entry(entry, struct sec_sa_item, next);
		sa->sa_len = session->sa_len;
		APMSEC_SATKNLOG("allocate sa cached 0x%p 0x%p", 
 				sa, sa->sa_ptr);
		return sa;
	}
	spin_unlock_irqrestore(&session->lock, flags);
	
	sa_size = sizeof(struct sec_sa_item) + 15 + 8 + session->sa_max_len;
	sa = APMSEC_COH_ALLOC(sa_size);
	if (sa == NULL)
		goto done;	
	memset(sa, 0, sa_size);
	sa->sa_len = session->sa_len;
	sa->sa_ptr = SA_PTR_COMPUTE(sa);
	APMSEC_SATKNLOG("allocate sa 0x%p 0x%p size %d", 
			sa, sa ? sa->sa_ptr : NULL, sa_size);
done:
	return sa;
}
	
void apm_sec_sa_free(struct apm_sec_session_ctx *session, 
			struct sec_sa_item *sa)
{
	unsigned long flags;
	
	spin_lock_irqsave(&session->lock, flags);
	if (session->sa_cache_cnt >= APM_SEC_SA_CACHE_MAX) {	
		spin_unlock_irqrestore(&session->lock, flags);
		APMSEC_SATKNLOG("free sa 0x%p", sa);
		APMSEC_COH_FREE(sa);
		return;
	}
	++session->sa_cache_cnt;
	list_add(&sa->next, &session->sa_cache);
	spin_unlock_irqrestore(&session->lock, flags);
	APMSEC_SATKNLOG("free sa cached 0x%p", sa);
}

int apm_sec_create_sa_tkn_pool(struct apm_sec_session_ctx *session,
				 u32 sa_max_len, u32 sa_len, 
				 char sa_ib, u32 tkn_len)
{
	session->tkn_max_len   = TKN_CTX_SIZE(tkn_len);
	session->tkn_input_len = tkn_len;
	session->sa_len	       = sa_len;
	session->sa_max_len    = sa_max_len;
	session->sa_flush_done = 0;

	if (session->sa == NULL) {
		session->sa = apm_sec_sa_get(session);
		if (session->sa == NULL) {
			return -ENOMEM;
		}
	} else {
		session->sa->sa_len = sa_len;
	}
	if (session->sa_ib == NULL && sa_ib) {
		session->sa_ib = apm_sec_sa_get(session);
		if (session->sa_ib == NULL) {
			return -ENOMEM;
		}
	} else if (session->sa_ib) {
		session->sa_ib->sa_len = sa_len;
	}
	return 0;
}

void apm_sec_free_sa_tkn_pool(struct apm_sec_session_ctx *session)
{
	struct sec_tkn_ctx *pos;
	struct sec_tkn_ctx *n;
	struct sec_sa_item *sapos;
	struct sec_sa_item *san;
	struct apm_sec_msg  *qpos;
	struct apm_sec_msg  *qn;
	
	if (session->sa_ib) {
		APMSEC_SATKNLOG("free sa 0x%p", session->sa_ib);
		APMSEC_COH_FREE(session->sa_ib);
		session->sa_ib = NULL;
	}
	if (session->sa) {
		APMSEC_SATKNLOG("free sa 0x%p", session->sa);
		APMSEC_COH_FREE(session->sa);
		session->sa = NULL;
	}

	list_for_each_entry_safe(pos, n, &session->tkn_cache, next) {
		APMSEC_SATKNLOG("free tkn 0x%p\n", pos);
		APMSEC_COH_FREE(pos);
	}
	INIT_LIST_HEAD(&session->tkn_cache);

	list_for_each_entry_safe(qpos, qn, &session->qmsg_cache, next) {
		APMSEC_SATKNLOG("free queue msg 0x%p\n", qpos);
		APMSEC_FREE(qpos);
	}
	INIT_LIST_HEAD(&session->qmsg_cache);

	list_for_each_entry_safe(sapos, san, &session->sa_cache, next) {
		APMSEC_SATKNLOG("free sa 0x%p", sapos);
		APMSEC_COH_FREE(sapos);
	}
	INIT_LIST_HEAD(&session->sa_cache);	
}

struct apm_sec_msg * apm_sec_qmsg_get(struct apm_sec_session_ctx *session) 
{
	struct apm_sec_msg  *qmsg;
	unsigned long flags;
	
	spin_lock_irqsave(&session->lock, flags);
	if (!list_empty(&session->qmsg_cache)) {
		struct list_head *entry = session->qmsg_cache.next;
		list_del(entry);
		--session->qmsg_cache_cnt;	
		spin_unlock_irqrestore(&session->lock, flags);
		return list_entry(entry, struct apm_sec_msg, next);
	}
	spin_unlock_irqrestore(&session->lock, flags);

	qmsg = APMSEC_ALLOC(sizeof(struct apm_sec_msg));
	if (qmsg == NULL)
		goto done;
done:
	return qmsg;
}

void apm_sec_qmsg_free(struct apm_sec_session_ctx *session, 
			struct apm_sec_msg *qmsg)
{
	unsigned long flags;
#if defined(CONFIG_APM86xxx_IOCOHERENT)
	if (qmsg->coh_dst_addr_link)
		APMSEC_COH_FREE(qmsg->coh_dst_addr_link);
#endif
	spin_lock_irqsave(&session->lock, flags);
	if (session->qmsg_cache_cnt >= APM_SEC_QMSG_CACHE_MAX) {	
		spin_unlock_irqrestore(&session->lock, flags);
		APMSEC_FREE(qmsg);
		return;
	}
	
	++session->qmsg_cache_cnt;
	list_add(&qmsg->next, &session->qmsg_cache);
	spin_unlock_irqrestore(&session->lock, flags);
}

struct apm_sec_session_ctx *apm_sec_session_create(void)
{
	struct apm_sec_session_ctx *session;
	
	session = APMSEC_ALLOC(sizeof(struct apm_sec_session_ctx));
	if (!session) {
		APMSEC_ERR("Failed to allocate session context\n");
		return NULL;
	}
	apm_sec_session_init(session);
	return session;
}

void	apm_sec_session_init(struct apm_sec_session_ctx *session)
{
	memset(session, 0, sizeof(*session));
	INIT_LIST_HEAD(&session->tkn_cache);
	INIT_LIST_HEAD(&session->sa_cache);
	INIT_LIST_HEAD(&session->qmsg_cache);
	spin_lock_init(&session->lock);
}

void	apm_sec_session_free(struct apm_sec_session_ctx *session)
{
	apm_sec_free_sa_tkn_pool(session);
}

struct sec_tkn_ctx *apm_sec_rmsg2tkn(struct apm_sec_msg_result *rmsg)
{
	return (void *) (unsigned long) rmsg->msg1_2.uinfo;
}

int apm_sec_send_msg2hw(struct apm_sec_msg *qmsg)
{
	struct apm_qm_msg_desc send_msg;
	struct apm_sec_msg1_2 *msg1_2;
	struct apm_sec_msg_3 *msg3;
	
	msg1_2 = &qmsg->msg1_2;
	msg3 = &qmsg->msg3;

	send_msg.msg   = qmsg;
	send_msg.qid   = apm_sec_get_queue()-> tx_qid[DEFAULT_COS];
	APMSEC_DEBUG("SEC msg qid: %d comp id: %d len: %d",
		     send_msg.qid, msg3->H0Enq_Num, msg1_2->data_len);

	return apm_qm_push_msg(&send_msg);
}

int apm_sec_queue2hw(struct apm_sec_session_ctx *session,
			struct sec_tkn_ctx *tkn)
{
	int rc;
	struct apm_qm_msg_desc send_msg;
	struct apm_sec_msg *qmsg;
	struct apm_sec_msg1_2 *msg1_2;
	struct apm_sec_msg_3 *msg3;
	qmsg = apm_sec_qmsg_get(session);
	if (qmsg == NULL) 
		return -ENOMEM;
	
	memset(qmsg, 0, APM_SEC_MSG_SIZE);
#if defined(CONFIG_APM86xxx_IOCOHERENT)
	qmsg->coh_dst_addr_link = NULL;
#endif
	tkn->qmsg = qmsg;
	msg1_2 = &qmsg->msg1_2;
	msg3 = &qmsg->msg3;
	 
#if !defined(CONFIG_NOT_COHERENT_CACHE) || defined(CONFIG_APM86xxx_IOCOHERENT)
	/* Set the Coherent bit for data only. 
	 * NOTE: Only data are coherent. SA and Token are not coherent and 
	 *	 will require flushing the cache 
         */
	msg1_2->coherent_read = 1; 	
#else
	msg1_2->coherent_read = 0;
#endif
	msg1_2->RType = APM_QM_SEC_RTYPE;
	
	msg3->HE = 1;
	msg3->H0Enq_Num = apm_sec_get_queue()->comp_qid[apm_processor_id()];
	msg3->tkn_addr = apm_sec_v2hwaddr(TKN_CTX_RESULT_TKN(tkn));
	/* msg3->tkn_addr = (u32) ((apm_sec_tkn_paddr() + 0x30) >> 4); */
	msg1_2->uinfo = (u32) (unsigned long) (tkn);
	msg3->mm = 1;  	/* Memory Mode */
	
	rc = apm_sec_loadbuffer2qmsg(qmsg, tkn);
	if (rc != 0) 
		goto err;

	send_msg.msg   = qmsg;
	send_msg.qid   = apm_sec_get_queue()-> tx_qid[DEFAULT_COS];

	APMSEC_SADUMP((u32 *) tkn->sa->sa_ptr, tkn->sa->sa_len);
	APMSEC_TKNDUMP(tkn);
	APMSEC_DEBUG("SEC msg qid: %d comp id: %d len: %d",
		send_msg.qid, msg3->H0Enq_Num, msg1_2->data_len);	
	/* Flush SA (not including the control WORD0 and WORD1
	   NOTE: You must flush SA regardless of coherency for APM862xx. */
#if !defined(CONFIG_APM86xxx_IOCOHERENT)
	if (tkn->sa && tkn->sa->sa_ptr && tkn->sa->sa_len > 0) {
	APMSEC_DEBUG("flush sa address 0x%08X len %d",
		     (u32) tkn->sa->sa_ptr, tkn->sa->sa_len);
	flush_dcache_range((u32) tkn->sa->sa_ptr,
			    (u32) tkn->sa->sa_ptr + tkn->sa->sa_len);
	}
	APMSEC_DEBUG("flush tkn address 0x%08X len %d",
		     (u32) TKN_CTX_RESULT_TKN(tkn),
		     TKN_RESULT_HDR_MAX_LEN + tkn->input_tkn_len);
	/* Flush token 
	NOTE: You must flush token regardless of coherency for APM862xx. */
	flush_dcache_range((u32) TKN_CTX_RESULT_TKN(tkn),
			    (u32) TKN_CTX_RESULT_TKN(tkn) + TKN_RESULT_HDR_MAX_LEN +
					    tkn->input_tkn_len);
#endif
	rc = apm_sec_send_msg2hw(qmsg);
err:
	if (rc == 0) {
		APMSEC_DEBUG("operation submitted %d", rc);
		return 0;
	} 
	APMSEC_ERR("operation submitted error %d", rc);
	return rc;
}

void apm_sec_qmesg_load_dst_single(struct apm_sec_msg * sec_msg, void *ptr, 
				int nbytes)
{
	u64 paddr = PADDR(ptr);

#if defined(CONFIG_NOT_COHERENT_CACHE) || defined(CONFIG_APM86xxx_IOCOHERENT)
	invalidate_dcache_range((u32) ptr, (u32) ptr + nbytes);
#endif
        sec_msg->msg4.dst_addr_hi = (u32) (paddr >> 32);
        sec_msg->msg4.dst_addr_lo = (u32) paddr;
}
