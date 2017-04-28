/**
 * AppliedMicro APM86xxx SoC Security Driver
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
 * @file   apm_sec_access.c
 *
 * This is the Linux layer of the APM86xxx SoC security driver.
 */
#include <linux/kernel.h>
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
#include <crypto/hash.h>
#include <crypto/internal/hash.h>
#include <crypto/algapi.h>
#include <crypto/aead.h>
#include <linux/spinlock_types.h>
#include <asm/io.h>
#include <asm/ipp.h>
#include <asm/apm_qm_core.h>
#include <asm/apm_qm_cfg.h>
#include <asm/apm86xxx_soc.h>
#include <asm/apm86xxx_pm.h>

#include "apm_sec_priv.h"
#include "apm_sec_alg.h"
#include "apm_sec_sa.h"
#include "apm_sec_tkn.h"
#include "apm_sec_csr.h"

#define	APMSEC_VERSION_STR		"0.1"

#define APM_SEC_HW_QUEUE_LEN		1000
#define RES_SIZE(r)			((r)->end - (r)->start + 1)

struct apm_sec_ctx {
	struct list_head 	alg_list;
	struct crypto_queue 	queue;
	atomic_t		active;
	spinlock_t 		lock;
	
	struct list_head completed_tkn_queue;
	struct tasklet_struct tasklet;

	char *sec_base_csr;
	char *sec_glb_ctrl_csr;
	char *eip96_axi_csr;
	char *eip96_csr;
	char *eip96_core_csr;
	char *sec_glb_diag_csr;
	int irq;
	
	struct apm_sec_qm_queues qm_queue;
	
	int poweroff;
	int sharemode;

	/* DMA variable for IO coherent mode */
	u64 dma_paddr;
	int dma_size;
	void *dma_vaddr;
	struct list_head dma_head;
	spinlock_t dma_lock;
};

static struct apm_sec_ctx sec_ctx;

struct apm_sec_qm_queues * apm_sec_get_queue(void)
{
	return &sec_ctx.qm_queue;
}

void *apm_sec_dma_addr(void)
{
	return sec_ctx.dma_vaddr;
}
 u64 apm_sec_dma_paddr(void)
{
	return sec_ctx.dma_paddr;
}

/* FIXME - move to apm_sec.c */
static int apm_sec_qconfig(struct apm_sec_ctx *ctx)
{
	struct apm_qm_qstate *cur_qstate;
	struct apm_qm_qstate *tmp_qstate;
	struct apm_qm_qalloc sec_qalloc;
	int i;
	int rc;

	cur_qstate = kmalloc(sizeof(struct apm_qm_qstate), GFP_KERNEL);
	if (cur_qstate == NULL) {
		APMSEC_ERR("Couldn't allocate memory for Enet queues state");
		return -ENOMEM;
	}
	memset(&sec_qalloc, 0, sizeof(struct apm_qm_qalloc));

	/* Allocate egress work queue for SEC from PPC0 */
	if (ctx->qm_queue.no_tm) {
		sec_qalloc.qstates = cur_qstate;
		sec_qalloc.qm_ip_blk = IP_BLK_QM;
		sec_qalloc.ip_blk = IP_SEC;
#ifdef CONFIG_SMP
		sec_qalloc.ppc_id = 0;
#else
		sec_qalloc.ppc_id = mfspr(SPRN_PIR);
#endif
		sec_qalloc.q_type = P_QUEUE;
		sec_qalloc.q_count = 1;
		sec_qalloc.direction = DIR_EGRESS;
		sec_qalloc.qsize = SIZE_64KB;
		sec_qalloc.thr_set = 1;
		memset(cur_qstate, 0, sizeof(struct apm_qm_qstate));
		if ((rc = apm_qm_alloc_q(&sec_qalloc)) != 0) {
			APMSEC_ERR("Error allocating two egress work queues for SEC");
			kfree(cur_qstate);
			return rc;
		}
		APMSEC_DEBUG("TX QID %d", cur_qstate->q_id);
		for (i = 0; i < TOTAL_COS; i++) {
			ctx->qm_queue.tx_qid[i] = cur_qstate->q_id;
		}
	} else {
		/* TM configuration */
		sec_qalloc.qstates = cur_qstate;
		sec_qalloc.qm_ip_blk = IP_BLK_QM;
		sec_qalloc.ip_blk = IP_SEC;
		sec_qalloc.ppc_id = 0; /* FIXME */
		sec_qalloc.q_count = 1;
		sec_qalloc.direction = DIR_EGRESS;
		sec_qalloc.qsize = SIZE_64KB;
		sec_qalloc.thr_set = 1;
		
		/* Retrieve a VQ id */
		ctx->qm_queue.vqid = apm_qm_get_vq(IP_SEC);
		APMSEC_DEBUG("TM VQID %d Alg %d", ctx->qm_queue.vqid,
			     ctx->qm_queue.vqid_alg);
		
		/* Create children PQs */
		sec_qalloc.q_type    = QM_CFG_QTYPE_PQ;
		sec_qalloc.parent_vq = ctx->qm_queue.vqid;
		sec_qalloc.vqen      = 1;
		for (i = 0; i < TOTAL_COS; i++) {
			memset(cur_qstate, 0, sizeof(struct apm_qm_qstate));
			if ((rc = apm_qm_alloc_q(&sec_qalloc)) != 0) {
				APMSEC_ERR(KERN_ERR
						"could not allocate child PQ error %d",
				rc);
				kfree(cur_qstate);
				return rc;
			}
			ctx->qm_queue.tx_qid[i] = cur_qstate->q_id;
		}
		
		/* Create parent VQ */
		sec_qalloc.q_type    = QM_CFG_QTYPE_VQ;
		sec_qalloc.parent_vq = 0;
		sec_qalloc.vqen      = 0;
		for (i = 0; i < 8; i++) {
			if (i >= TOTAL_COS) {
				/* Unused entry must be the VQ id */
				sec_qalloc.pq_sel[i] = ctx->qm_queue.vqid;
				continue;
			}
			sec_qalloc.pq_sel[i] = ctx->qm_queue.tx_qid[i];
			sec_qalloc.q_sel_arb[i] = ctx->qm_queue.vqid_alg;
			sec_qalloc.shape_rate[i] = ctx->qm_queue.tx_qid_param[i];
			APMSEC_DEBUG("TM PQ %d Param %d",
				     ctx->qm_queue.tx_qid[i],
				     ctx->qm_queue.tx_qid_param[i]);
		}
		
		memset(cur_qstate, 0, sizeof(struct apm_qm_qstate));
		rc = apm_qm_alloc_vq(&sec_qalloc, ctx->qm_queue.vqid);
		if (rc != 0) {
			APMSEC_ERR(KERN_ERR "could not allocate VQ");
			kfree(cur_qstate);
			return rc;
		}
	}
	
	/* Allocate completion queue for SEC to PPCx */

	for (i = 0; i < MAX_CORES; i++) {
		tmp_qstate = get_comp_queue(IP_SEC, i);
		ctx->qm_queue.comp_qid[i] = tmp_qstate->q_id;
		ctx->qm_queue.comp_mbid[i] = tmp_qstate->mb_id;
		APMSEC_DEBUG("Completion QID CPU%d %d", i,
			     ctx->qm_queue.comp_qid[i]);
	}
	kfree(cur_qstate);
	return 0;
}

int apm_sec_wr32(u8 block_id, u32 reg_addr, u32 data_val)
{
	char * addr_reg_offset;

	switch (block_id) {
		case SEC_GLB_CTRL_CSR_BLOCK:
			addr_reg_offset = sec_ctx.sec_glb_ctrl_csr + reg_addr;
			break;
		case EIP96_AXI_CSR_BLOCK:
			addr_reg_offset = sec_ctx.eip96_axi_csr + reg_addr;
			break;
		case EIP96_CSR_BLOCK:
			addr_reg_offset = sec_ctx.eip96_csr + reg_addr;
			break;
		case EIP96_CORE_CSR_BLOCK:
			addr_reg_offset = sec_ctx.eip96_core_csr + reg_addr;
			break;
		default:
			APMSEC_ERR("Invalid block id %d in write reg: %d",
				   block_id, reg_addr);
			return -1;
	}
	APMSEC_WRLOG("CSR WR addr: 0x%08X val: 0x%08X",
		     (u32)addr_reg_offset, data_val);
	out_be32((volatile unsigned __iomem *) (addr_reg_offset), data_val);
	return 0;
}

int apm_sec_rd32(u8 block_id, u32 reg_addr, u32 *data_val)
{
	char * addr_reg_offset;

	switch (block_id) {
		case SEC_GLB_CTRL_CSR_BLOCK:
			addr_reg_offset = sec_ctx.sec_glb_ctrl_csr + reg_addr;
			break;
		case EIP96_AXI_CSR_BLOCK:
			addr_reg_offset = sec_ctx.eip96_axi_csr + reg_addr;
			break;
		case EIP96_CSR_BLOCK:
			addr_reg_offset = sec_ctx.eip96_csr + reg_addr;
			break;
		case EIP96_CORE_CSR_BLOCK:
			addr_reg_offset = sec_ctx.eip96_core_csr + reg_addr;
			break;
		default:
			APMSEC_ERR("Invalid block id %d in read reg: %d",
				   block_id, reg_addr);
			return -1;
	}
	*data_val = in_be32((volatile unsigned __iomem *) addr_reg_offset);
	APMSEC_RDLOG("CSR RD addr: 0x%08X val: 0x%08X",
		     (u32)addr_reg_offset, *data_val);
	return 0;
}

void apm_lsec_coh_init(void)
{
#if !defined(CONFIG_APM86xxx_IOCOHERENT)
	return;
#else
#define APM_SEC_DMA_ITEM_HDR_SIZE	32
#define APM_SEC_DMA_ITEM_SIZE		512
	struct list_head * item;
	u8 *ptr;
	u32 i;
	int total = 0;
	
	spin_lock_init(&sec_ctx.dma_lock);
	INIT_LIST_HEAD(&sec_ctx.dma_head);
	ptr = sec_ctx.dma_vaddr;
	for (i = 0; i < sec_ctx.dma_size; i += APM_SEC_DMA_ITEM_SIZE) {
		item = (struct list_head *) ptr;
		list_add(item, &sec_ctx.dma_head);
		       total++;
		ptr += APM_SEC_DMA_ITEM_SIZE;
	}
#endif
}

void *_APMSEC_COH_ALLOC(int size, const char* file, int lnum)
{
#if !defined(CONFIG_APM86xxx_IOCOHERENT)
	return kmalloc(size, GFP_KERNEL);
#else
	unsigned long flags;
	struct list_head * item;
	if (size > (APM_SEC_DMA_ITEM_SIZE - APM_SEC_DMA_ITEM_HDR_SIZE)) {
		printk(KERN_ERR "unable to allocate coherent memory size %d\n",
		       size);
		return NULL;
	}
	spin_lock_irqsave(&sec_ctx.dma_lock, flags);
	if (list_empty(&sec_ctx.dma_head)) {
		printk(KERN_ERR "coherent pool empty! attempt from %s:%d\n", file, lnum);
		spin_unlock_irqrestore(&sec_ctx.dma_lock, flags);
		return NULL;
	}

	item = sec_ctx.dma_head.next;
	list_del(item);
	spin_unlock_irqrestore(&sec_ctx.dma_lock, flags);
	return (u8 *) item + APM_SEC_DMA_ITEM_HDR_SIZE;
#endif
}

void APMSEC_COH_FREE(void *ptr)
{
#if !defined(CONFIG_APM86xxx_IOCOHERENT)
	kfree(ptr);
#else
	unsigned long flags;
	struct list_head * item;
	
	spin_lock_irqsave(&sec_ctx.dma_lock, flags);
	item = (void *) ((unsigned long) ptr - APM_SEC_DMA_ITEM_HDR_SIZE);
	list_add(item, &sec_ctx.dma_head);
	spin_unlock_irqrestore(&sec_ctx.dma_lock, flags);
#endif
}


void apm_lsec_sg2buf(struct scatterlist *src, int nbytes, char *saddr)
{
	int  i;
	void *spage;
	void *vaddr;
	
	for (i = 0; nbytes > 0; i++) {
		spage = kmap_atomic(sg_page(&src[i]));
		vaddr = spage + src[i].offset;
		memcpy(saddr, vaddr, src[i].length);
		kunmap_atomic(spage);
		nbytes -= src[i].length;
		saddr  += src[i].length;
	}
}

int apm_lsec_sg_scattered(unsigned int nbytes, struct scatterlist *sg)
{
	return ((int) nbytes - (int) sg[0].length) > 0;
}

int apm_sec_load_src_buffers(struct apm_sec_msg *sec_msg,
			     struct scatterlist *src, int nbytes,
			     struct sec_tkn_ctx *tkn)
{
	struct apm_sec_msg1_2 *msg1_2 = &sec_msg->msg1_2;
	union apm_sec_addr_list *addr_list = &sec_msg->addr_list;
	int  i;
	void *spage;
	u64  paddr;
	u32  len;

	if (!nbytes) {
		APMSEC_ERR("Zero Len source: No HW Support");
		return -EINVAL;
	}

	APMSEC_DEBUG("spage len %d src len %d", nbytes, src->length);

	spage = kmap_atomic(sg_page(src));
	APMSEC_DEBUG("spage[0] 0x%p len %d", spage + src->offset, src->length);
	APMSEC_DEBUG_DUMP("Spage ", spage + src->offset,
			  src->length);
#if defined(CONFIG_NOT_COHERENT_CACHE) || defined(CONFIG_APM86xxx_IOCOHERENT)
		if (tkn->esp_src_len) {
			flush_dcache_range((u32) (spage + src->offset - tkn->esp_src_len),
			    (u32) (spage + src->offset +
					    src->length + tkn->esp_src_len));
		} else {
			flush_dcache_range((u32) spage + src->offset,
					    (u32) spage + src->offset + src->length);
		}
#endif
	paddr = PADDR(spage + src->offset);
	kunmap_atomic(spage);
	msg1_2->addr_hi = (u32) (paddr >> 32);
	msg1_2->addr_lo = (u32) paddr;
	len = src->length;

	if (nbytes <= 16*1024 && nbytes < len) {
		APMSEC_DEBUG("spage length %d != %d (nbytes)",
				len, nbytes);
		len = nbytes;
	}

	if (len < 16*1024) {
		APMSEC_DEBUG("spage HW[0] 0x%0llX len %d", paddr, len);
		msg1_2->data_len = len;
		nbytes -= len;
		paddr   = 0;
	} else if (len == 16*1024) {
		APMSEC_DEBUG("spage HW[0] 0x%0llX len %d", paddr, 16*1024);
		msg1_2->data_len = 0;
		nbytes -= 16*1024;
		paddr   = 0;
	} else {
		APMSEC_DEBUG("spage HW[0] 0x%0llX len %d", paddr, 16*1024);
		msg1_2->data_len = 0;
		nbytes -= 16*1024;
		paddr  += 16*1024;
		len    -= 16*1024;
	}
	if (nbytes == 0) {
		return 0;	/* single buffer */
	}

	src = sg_next(src);

	msg1_2->NV = 1; /* Linked 3 more buffers at least */
	for (i = 0; nbytes > 0 && i < 3; i++) {
		if (paddr == 0) {

			while(!src->length) {
				APMSEC_DEBUG("skipping sg with zero length\n");
				src = sg_next(src);
			}

			spage = kmap_atomic(sg_page(src));
			APMSEC_DEBUG("spage[%d] 0x%p len %d", i + 1,
				     spage + src->offset, src->length);
			APMSEC_DEBUG_DUMP("src2 ", spage + src->offset,
					  src->length);
#if defined(CONFIG_NOT_COHERENT_CACHE) || defined(CONFIG_APM86xxx_IOCOHERENT)
		if (tkn->esp_src_len) {
			flush_dcache_range((u32) (spage + src->offset - tkn->esp_src_len),
			    (u32) (spage + src->offset +
					    src->length + tkn->esp_src_len));
		} else {
			flush_dcache_range((u32) spage + src->offset,
					    (u32) spage + src->offset + src->length);
		}
#endif
        	        paddr = PADDR(spage + src->offset);
	                kunmap_atomic(spage);
			len = src->length;
		}
		addr_list->addr[i].next_addr_hi = (u32) (paddr >> 32);
		addr_list->addr[i].next_addr_lo = (u32) paddr;
		if (nbytes < len) {
			APMSEC_DEBUG("spage HW[%d] length %d != %d (nbytes)",
					i + 1, len, nbytes);
			len = nbytes;
		}

		if (len < 16*1024) {
			APMSEC_DEBUG("spage HW[%d] 0x%0llX len %d",
				     i + 1, paddr, len);
			addr_list->addr[i].next_data_len = len;
			nbytes -= len;
			paddr   = 0;
		} else if (len == 16*1024) {
			APMSEC_DEBUG("spage HW[%d] 0x%0llX len %d",
				     i + 1, paddr, 16*1024);
			addr_list->addr[i].next_data_len = 0;
			nbytes -= 16*1024;
			paddr   = 0;
		} else {
			APMSEC_DEBUG("spage HW[%d] 0x%0llX len %d",
				     i + 1, paddr, 16*1024);
			addr_list->addr[i].next_data_len = 0;
			nbytes -= 16*1024;
			paddr  += 16*1024;
			len    -= 16*1024;
		}

		src = sg_next(src);
	}
	if (nbytes == 0)  {
		/* We are done here */
		addr_list->addr[i].next_data_len = 0x7800;
		APMSEC_DEBUG_DUMP("src last 32 bytes: ", (void *) addr_list, 32);
		return 0;	/* at most 4 buffers */
	} else if (nbytes <= 16 * 1024 && sg_is_last(src)) {
		spage = kmap_atomic(sg_page(src));
		APMSEC_DEBUG("spage addr = 0x%p, offset = 0x%08X, len = %d",
			     spage, src->offset, src->length);
#if defined(CONFIG_NOT_COHERENT_CACHE) || defined(CONFIG_APM86xxx_IOCOHERENT)
		flush_dcache_range((u32) spage + src->offset,
				    (u32) spage + src->offset + src->length);
#endif
		paddr = PADDR(spage + src->offset);
		kunmap_atomic(spage);
		len = src->length;
		addr_list->addr[i].next_addr_hi = (u32) (paddr >> 32);
		addr_list->addr[i].next_addr_lo = (u32) paddr;
		if (len < 16*1024) {
			if (nbytes < len)
				len = nbytes;
			addr_list->addr[i].next_data_len = len;
			nbytes -= len;
			paddr   = 0;
		} else if (len == 16*1024) {
			addr_list->addr[i].next_data_len = 0;
			nbytes -= 16*1024;
			paddr   = 0;
		}
		APMSEC_DEBUG_DUMP("src last 32 bytes: ", (void *) addr_list, 32);
		return 0;
	}
	
	msg1_2->LL = 1; /* Linked list of buffers */

	for (i = 0; nbytes > 0 &&  i < APM_SEC_LINK_ADDR_MAX - 4; i++) {
		if (paddr == 0) {
			while(!src->length) {
				APMSEC_DEBUG("skipping sg with zero length\n");
				src = sg_next(src);
			}

			spage = kmap_atomic(sg_page(src));
			APMSEC_DEBUG("spage addr 0x%p offset 0x%08X len %d",
				     spage, src->offset, src->length);
#if defined(CONFIG_NOT_COHERENT_CACHE) || defined(CONFIG_APM86xxx_IOCOHERENT)
			flush_dcache_range((u32) spage + src->offset,
					    (u32) spage + src->offset + src->length);
#endif
        	        paddr = PADDR(spage + src->offset);
	                kunmap_atomic(spage);
			len   = src->length;
		}
		sec_msg->src_addr_link[i].next_addr_hi = (u32) (paddr >> 32);
		sec_msg->src_addr_link[i].next_addr_lo = (u32) paddr;
		if (len < 16*1024) {
			if (nbytes < len)
				len = nbytes;
			sec_msg->src_addr_link[i].next_data_len = len;
			nbytes -= len;
			paddr   = 0;
		} else if (len == 16*1024) {
			sec_msg->src_addr_link[i].next_data_len = 0;
			nbytes -= 16*1024;
			paddr   = 0;
		} else {
			sec_msg->src_addr_link[i].next_data_len = 0;
			nbytes -= 16*1024;
			paddr  += 16*1024;
			len    -= 16*1024;
		}
		src = sg_next(src);
	}
	addr_list->desc.addr_link.total_length_link = 0;
	addr_list->desc.addr_link.linksize = i;
	paddr = PADDR(sec_msg->src_addr_link);
	
	/* Flush source link lisk 
	NOTE: You must flush the source link list regardless of coherency
	for APM862xx. */
	flush_dcache_range((u32) sec_msg->src_addr_link,
			    (u32) sec_msg->src_addr_link +
					    sizeof(struct apm_sec_msg_address) * i);

	addr_list->desc.addr_link.link_data_ptr_hi = (u32) (paddr >> 32);
	addr_list->desc.addr_link.link_data_ptr_lo = (u32) paddr;

	if (nbytes == 0) {
		APMSEC_DEBUG("LL bit is being set");
		APMSEC_DEBUG_DUMP("src last 32 bytes: ",
				  (void *) addr_list, 32);
		APMSEC_DEBUG_DUMP("src linked list dump ",
				  (void *) sec_msg->src_addr_link, 8 * i);

		return 0;	/* 5 - 16 buffers */
	}
	
	APMSEC_ERR("source buffer length %d too long error %d",
		   nbytes, -EINVAL);
	return -EINVAL;
}
			
int apm_sec_load_dst_buffers(struct apm_sec_msg * sec_msg, 
			     struct scatterlist *dst, u32 nbytes,
			     struct sec_tkn_ctx *tkn)
{
	struct apm_sec_msg_3 *msg3 = &sec_msg->msg3;
	struct apm_sec_msg_4 *msg4 = &sec_msg->msg4;
	
	struct apm_sec_msg_address *addr_list;
	int  i;
	void *dpage;
	u64 paddr;
	u32 len = 0;
	int scatter_count = 0;
	int result_len = nbytes;
#if defined(CONFIG_APM86xxx_IOCOHERENT)
	u32 offset;
#endif

        APMSEC_DEBUG("dpage len %d dst len %d", nbytes, dst->length);

	if (nbytes == dst->length || (nbytes <= 16*1024 && nbytes < dst->length) ) {
		/* Single buffer */
		dpage = kmap_atomic(sg_page(dst));
		paddr = PADDR(dpage + dst->offset);
#if defined(CONFIG_NOT_COHERENT_CACHE) || defined(CONFIG_APM86xxx_IOCOHERENT)
		invalidate_dcache_range((u32) dpage + dst->offset,
					 (u32) dpage + dst->offset + nbytes);
#endif
		kunmap_atomic(dpage);	
        	msg4->dst_addr_hi = (u32) (paddr >> 32);
		msg4->dst_addr_lo = (u32) paddr;
		return 0;
	} else {
		if (sg_is_last(dst)) {
			APMSEC_ERR("Incorrect dest buffer setting");
			return -EINVAL;
		}
		paddr = 0;
		dpage = NULL;
#if defined(CONFIG_APM86xxx_IOCOHERENT)
		addr_list = APMSEC_COH_ALLOC(sizeof (struct apm_sec_msg_address) *
						APM_SEC_LINK_ADDR_MAX);
		if (!addr_list) {
			printk("dest addr list alloc failed\n");
			return -EINVAL;
		}
		sec_msg->coh_dst_addr_link = (struct apm_sec_msg_address *)addr_list;
#else
		addr_list = sec_msg->dst_addr_link;
#endif
		addr_list = (struct apm_sec_msg_address *) (void *)
						TKN_ALIGN16(addr_list);
		for (i = 0; (nbytes > 0) && i < APM_SEC_LINK_ADDR_MAX; i++) {
#if defined(CONFIG_APM862xx)
			scatter_count++;
			if (scatter_count >= 3 && dst->length < 16 /* && !sg_is_last(dst) */) {
				APMSEC_DEBUG("dpage apply HW workaround");
				
				tkn->dest_mem = kzalloc(result_len, GFP_KERNEL);
				if (!tkn->dest_mem) {
					APMSEC_ERR("Memory allocation failed for"
							"tkn->dest_mem for nbytes = %d",
					result_len);
					return -ENOMEM;
				}
				paddr = PADDR(tkn->dest_mem);
				msg4->dst_addr_hi = (u32) (paddr >> 32);
				msg4->dst_addr_lo = (u32) paddr;
				tkn->dest_nbytes = result_len;
#if defined(CONFIG_NOT_COHERENT_CACHE) || defined(CONFIG_APM86xxx_IOCOHERENT)
				invalidate_dcache_range((u32) tkn->dest_mem,
		(u32) tkn->dest_mem + result_len);
#endif
				return 0;
			}
#endif
			if (paddr == 0) {
				dpage = kmap_atomic(sg_page(dst));
				paddr = PADDR(dpage + dst->offset);
				len   = dst->length;
#if defined(CONFIG_NOT_COHERENT_CACHE) || defined(CONFIG_APM86xxx_IOCOHERENT)
				invalidate_dcache_range(
					(u32) dpage + dst->offset,
					(u32) dpage + dst->offset + len);
#endif
				kunmap_atomic(dpage);
			}
			addr_list[i].next_addr_hi = (u32) (paddr >> 32);
			addr_list[i].next_addr_lo = (u32) paddr;
			if (nbytes < len) {
				len = nbytes;
			}

			if (len < 16*1024) {
				APMSEC_DEBUG("dpage HW[%d] 0x%0llX len %d", i,
					     paddr, len);
				addr_list[i].next_data_len = len;
				nbytes -= len;
				paddr   = 0;
			} else if (len == 16*1024) {
				APMSEC_DEBUG("dpage HW[%d] 0x%0llX len %d", i,
					     paddr, 16*1024);
				addr_list[i].next_data_len = 0;
				nbytes -= 16*1024;
				paddr   = 0;
			} else {
				APMSEC_DEBUG("dpage HW[%d] 0x%0llX len %d", i,
					     paddr, 16*1024);
				addr_list[i].next_data_len = 0;
				nbytes -= 16*1024;
				paddr  += 16*1024;
				len    -= 16*1024;
			}

			dst = sg_next(dst);
		}
		msg3->ml = 1;
		msg3->ll_len_lsb = i & 0xF;
		msg4->ll_len_msb = (i >> 4) & 0xF;
#if defined(CONFIG_APM86xxx_IOCOHERENT)
		offset = (u32)addr_list - (u32)apm_sec_dma_addr();
		paddr = (apm_sec_dma_paddr() + offset);
#else
		/* Flush destination link lisk
		NOTE: You must flush the destination link list regardless
		of coherency for APM862xx. */
		flush_dcache_range((u32) sec_msg->src_addr_link,
				    (u32) sec_msg->dst_addr_link +
						    sizeof(struct apm_sec_msg_address) * i);
		paddr = PADDR(addr_list);
#endif
		msg4->dst_addr_hi = (u32) (paddr >> 32);
		msg4->dst_addr_lo = (u32) paddr;
	}

	if (nbytes == 0) {
		APMSEC_DEBUG_DUMP("dst link dump: ", (void *) addr_list, i*8);
		return 0;
	}

	APMSEC_DEBUG("destination buffer length too long error %d", nbytes);
	return -EINVAL;
}

int apm_sec_loadbuffer2qmsg(struct apm_sec_msg *sec_msg,
			    struct sec_tkn_ctx *tkn)
{
	struct crypto_async_request *req = tkn->context;
	int rc;

	switch (crypto_tfm_alg_type(req->tfm)) {
		case CRYPTO_ALG_TYPE_AHASH:
		{
			struct ahash_request *ahash_req;
			int ds;
	
		/* NOTE: We need to load source before destination as 
			UP flush the source and invalidate destination.
			This is need if source and destination is same. */
			ahash_req = ahash_request_cast(req);
			rc = apm_sec_load_src_buffers(sec_msg, ahash_req->src,
					ahash_req->nbytes, tkn);
			if (rc != 0)
				break;
			ds = crypto_ahash_digestsize(__crypto_ahash_cast(ahash_req->base.tfm));
			apm_sec_qmesg_load_dst_single(sec_msg, ahash_req->result, ds);
		}
		break;
		case CRYPTO_ALG_TYPE_ABLKCIPHER:
		{
			struct ablkcipher_request *ablk_req;
			ablk_req = ablkcipher_request_cast(req);
		/* NOTE: We need to load source before destination as 
			UP flush the source and invalidate destination.
			This is need if source and destination is same. */
			rc = apm_sec_load_src_buffers(sec_msg, ablk_req->src,
					ablk_req->nbytes, tkn);
			if (rc != 0)
				break;
			rc = apm_sec_load_dst_buffers(sec_msg, ablk_req->dst,
					ablk_req->nbytes, tkn);
		}
		break;
		case CRYPTO_ALG_TYPE_AEAD:
		{
			struct aead_request *aead_req;
			aead_req = req->data;
		/* NOTE: We need to load source before destination as 
			UP flush the source and invalidate destination.
			This is need if source and destination is same. */
			rc = apm_sec_load_src_buffers(sec_msg, aead_req->src,
					aead_req->cryptlen, tkn);
			if (rc != 0)
				break;
			rc = apm_sec_load_dst_buffers(sec_msg, aead_req->dst,
					tkn->dest_nbytes, tkn);
		}
		break;
		default:
			BUG();
			rc = -EINVAL;
			break;
	}
			
	return rc;
}

struct sec_tkn_ctx *apm_sec_async_req2tkn(struct crypto_async_request *req)
{
	struct apm_sec_req_ctx *rctx;
	
	switch (crypto_tfm_alg_type(req->tfm)) {
		case CRYPTO_ALG_TYPE_AHASH:
			rctx = ahash_request_ctx(ahash_request_cast(req));
			break;
		case CRYPTO_ALG_TYPE_ABLKCIPHER:
			rctx = ablkcipher_request_ctx(ablkcipher_request_cast(req));
			break;
		case CRYPTO_ALG_TYPE_AEAD: {
			struct aead_request *aead_req = req->data;
			rctx = aead_request_ctx(aead_req);
		}break;
		default:
			BUG();
			return NULL;
	}
	return rctx->tkn;
}

static inline int apm_sec_hw_queue_available(void)
{
	/* look at the HW queue and determine if fulled? */
	return 1; /* return ppro_qma_get_queue_state(qid) == 0; FIXME */
}

static int apm_sec_send2hwq(struct crypto_async_request *req)
{
	struct apm_sec_session_ctx *session;
	struct apm_sec_req_ctx *rctx;
	int 	rc;
	
	switch (crypto_tfm_alg_type(req->tfm)) {
		case CRYPTO_ALG_TYPE_ABLKCIPHER:
			rctx = ablkcipher_request_ctx(ablkcipher_request_cast(req));
			session = crypto_tfm_ctx(ablkcipher_request_cast(req)->base.tfm);
			break;
		case CRYPTO_ALG_TYPE_AEAD: {
			struct aead_request *aead_req = req->data;
			rctx = aead_request_ctx(aead_req);
			session = crypto_tfm_ctx(aead_req->base.tfm);
		}
			break;
		case CRYPTO_ALG_TYPE_AHASH:
			rctx = ahash_request_ctx(ahash_request_cast(req));
			session = crypto_tfm_ctx(ahash_request_cast(req)->base.tfm);
			break;
		default:
			BUG();
			break;
	}
	
	if (atomic_inc_return(&sec_ctx.active) >= APM_SEC_CMPQ_THRES) {
		rc = -EAGAIN;
		goto err_out;
	}
		
	rc = apm_sec_queue2hw(session, rctx->tkn);
	if (rc != 0) {
		APMSEC_TXLOG("failed submission error 0x%08X", rc);
		goto err_out;
	}
	rc = -EINPROGRESS;
	return rc;

err_out:
		atomic_dec(&sec_ctx.active);
	return rc;
}

static int apm_sec_handle_req(struct crypto_async_request *req)
{
	int ret = -EAGAIN;

	if (apm_sec_hw_queue_available()) {
		ret = apm_sec_send2hwq(req);
		return ret;
	}
	if (ret == -EAGAIN) {
		unsigned long flags;

		spin_lock_irqsave(&sec_ctx.lock, flags);
		ret = crypto_enqueue_request(&sec_ctx.queue, req);
		spin_unlock_irqrestore(&sec_ctx.lock, flags);
	}
	return ret;
}

static int apm_sec_process_queue(void)
{
	struct crypto_async_request *req;
	unsigned long flags;
	int err = 0;

	while (apm_sec_hw_queue_available()) {
		spin_lock_irqsave(&sec_ctx.lock, flags);
		req = crypto_dequeue_request(&sec_ctx.queue);
		spin_unlock_irqrestore(&sec_ctx.lock, flags);

		if (!req)
			break;
		err = apm_sec_handle_req(req);
		if (err)
			break;
	}
	return err;
}

int apm_sec_setup_crypto(struct crypto_async_request *req)
{
	int err;

	err = apm_sec_handle_req(req);
	if (err)
		return err;
	if (apm_sec_hw_queue_available() && sec_ctx.queue.qlen)
		err = apm_sec_process_queue();
	return err;
}

/*
 * Algorithm Registration Functions
 *
 */
int apm_sec_alg_init(struct crypto_tfm *tfm)
{
	struct crypto_alg *alg = tfm->__crt_alg;
	struct apm_sec_session_ctx *session = crypto_tfm_ctx(tfm);

	apm_sec_session_init(session);
	
	if (alg->cra_type == &crypto_ablkcipher_type)
		tfm->crt_ablkcipher.reqsize = sizeof(struct apm_sec_req_ctx);
	else if (alg->cra_type == &crypto_ahash_type)
		crypto_ahash_set_reqsize(__crypto_ahash_cast(tfm),
					 sizeof(struct apm_sec_req_ctx));
	else if (alg->cra_type == &crypto_aead_type)
		tfm->crt_aead.reqsize = sizeof(struct apm_sec_req_ctx);

	return 0;
}

void apm_sec_alg_exit(struct crypto_tfm *tfm)
{
	struct apm_sec_session_ctx *session = crypto_tfm_ctx(tfm);
	apm_sec_session_free(session);
}

static void apm_sec_unregister_alg(void)
{
	struct apm_sec_alg *alg, *tmp;

	list_for_each_entry_safe(alg, tmp, &sec_ctx.alg_list, entry) {
		list_del(&alg->entry);
		switch (alg->type) {
			case CRYPTO_ALG_TYPE_AHASH:
				crypto_unregister_ahash(&alg->u.hash);
				break;

			default:
				crypto_unregister_alg(&alg->u.cipher);
				break;
		}
		kfree(alg);
	}
}

static int apm_sec_register_alg(void)
{
	struct apm_sec_alg *alg;
	int i;
	int rc = 0;

	for (i = 0; apm_sec_alg_tlb[i].type != 0; i++) {
		struct crypto_alg *cipher;
		 
		alg = kzalloc(sizeof(struct apm_sec_alg), GFP_KERNEL);
		if (!alg)
			return -ENOMEM;
		alg->type = apm_sec_alg_tlb[i].type;
		switch (alg->type) {
			case CRYPTO_ALG_TYPE_AHASH:
				alg->u.hash = apm_sec_alg_tlb[i].u.hash;
				cipher = &alg->u.hash.halg.base;
				break;
			default:
				alg->u.cipher = apm_sec_alg_tlb[i].u.cipher;
				cipher = &alg->u.cipher;
				break;
		}
		INIT_LIST_HEAD(&cipher->cra_list);
		if (!cipher->cra_init)
			cipher->cra_init = apm_sec_alg_init;
		if (!cipher->cra_exit)
			cipher->cra_exit = apm_sec_alg_exit;
		if (!cipher->cra_module)
			cipher->cra_module = THIS_MODULE;
		if (!cipher->cra_module)
			cipher->cra_priority = APM_SEC_CRYPTO_PRIORITY;
		switch (alg->type) {
			case CRYPTO_ALG_TYPE_AHASH:
				rc = crypto_register_ahash(&alg->u.hash);
				break;
			default:
				rc = crypto_register_alg(&alg->u.cipher);
				break;
		}
		if (rc) {
			APMSEC_ERR("failed to register alg %s error %d",
				   cipher->cra_name, rc);
			kfree(alg);
			return rc;
		}
		list_add_tail(&alg->entry, &sec_ctx.alg_list);
	}
	return rc;
}

static int apm_cp_buf2sg(struct sec_tkn_ctx *tkn,
			 struct crypto_async_request *req)
{
	int  nbytes = tkn->dest_nbytes;
	void *dest   = tkn->dest_mem;
	int  i;
	int  len;
	void *dpage;
	char *daddr;
	struct scatterlist *dst;

	APMSEC_DEBUG_DUMP("RESULT dump", tkn->dest_mem, tkn->dest_nbytes);
	if (crypto_tfm_alg_type(req->tfm) == CRYPTO_ALG_TYPE_ABLKCIPHER) {
		struct ablkcipher_request *ablk_req;
		ablk_req = ablkcipher_request_cast(req);
		dst = ablk_req->dst;
	} else if (crypto_tfm_alg_type(req->tfm) == CRYPTO_ALG_TYPE_AEAD) {
		struct aead_request *aead_req = req->data;
		dst = aead_req->dst;
	} else {
		BUG();
	}
	
	len = dst[0].length;
	dpage = kmap_atomic(sg_page(&dst[0]));
	daddr = dpage + dst[0].offset;
	memcpy(daddr, dest, len);
	kunmap_atomic(dpage);
	nbytes -= len;
	dest    += len;
	for (i = 1; nbytes > 0; i++) {
		dpage = kmap_atomic(sg_page(&dst[i]));
		daddr = dpage + dst[i].offset;
		if (nbytes < dst[i].length)
			memcpy(daddr, dest, nbytes);
		else
			memcpy(daddr, dest, dst[i].length);
		kunmap_atomic(dpage);
		nbytes -= dst[i].length;
		dest    += dst[i].length;
	}
	return 0;
}

/* FIXME - move to apm_sec.c */
void apm_sec_err_log(int err)
{
	switch (err) {
		case ERR_MSG_AXI:
			APMSEC_ERR("AXI Error when reading the Src or "
					"Dst Add from the Link List");
			break;
		case ERR_BAD_MSG:
			APMSEC_ERR("HE0 bit is not set the incoming Message");
			break;
		case ERR_READ_DATA_AXI:
			APMSEC_ERR("AXI Error when reading data");
			break;
		case ERR_WRITE_DATA_AXI:
			APMSEC_ERR("AXI Error when writing data");
			break;
		case CRYPTO_IP_ERR:
			APMSEC_ERR("CRYPTO_IP_ERR");
			break;
		case SEC_ERR_BAD_MSG:
			APMSEC_ERR("Message Format Error");
			break;
		case INVALID_MSG_ERR:
			APMSEC_ERR("Invalid Message Error");
			break;
		default:
			APMSEC_ERR("No error set");
			break;
	}
}

#ifdef APM_SEC_DEBUGEXTRA
void apm_sec_dump_src_dst_buf(struct crypto_async_request *req,
			      struct sec_tkn_ctx *tkn)
{
	struct scatterlist *dst;
	void *dpage;
	int len, i, nbytes;

	if (crypto_tfm_alg_type(req->tfm) == CRYPTO_ALG_TYPE_ABLKCIPHER) {
		struct ablkcipher_request *ablkcipher_req;
		ablkcipher_req = container_of(req, struct ablkcipher_request, base);
		dst = ablkcipher_req->dst;
	} else if (crypto_tfm_alg_type(req->tfm) == CRYPTO_ALG_TYPE_AHASH) {
		struct ahash_request *ahash_req;
		void *res;
		ahash_req = container_of(req, struct ahash_request, base);
		res = ahash_req->result;
		APMSEC_DEBUGX_DUMP("hash result ", res, tkn->dest_nbytes);
		return;
	} else if (crypto_tfm_alg_type(req->tfm) == CRYPTO_ALG_TYPE_AEAD) {
		struct aead_request *aead_req = req->data;
		dst = aead_req->dst;
	} else {
		return;
	}
		
	nbytes = tkn->dest_nbytes;
	len = dst[0].length;
	APMSEC_DEBUGX("dpage len %d, dst len %d", len, nbytes);
	dpage = kmap_atomic(sg_page(dst));
	APMSEC_DEBUGX_DUMP("dest[0] dump ", dpage + dst[0].offset,
			  len);
	kunmap_atomic(dpage);
	nbytes -= len;
	for (i = 1; nbytes > 0; i++) {
		dpage = kmap_atomic(sg_page(&dst[i]));
		APMSEC_DEBUGX_DUMP("dest dump ", dpage + dst[i].offset,
				  dst[i].length);
		kunmap_atomic(dpage);
		nbytes -= dst[i].length;
	}
	return;
}
#endif

/**
 * Token Completion Callback Handler
 *
 */
int apm_sec_tkn_cb(struct sec_tkn_ctx *tkn)
{
	struct crypto_async_request *req;
	struct apm_sec_req_ctx *rctx;
	struct sec_tkn_result_hdr *result_tkn;
	struct apm_sec_session_ctx *session;
	int	esp_nh_padlen = 0;
	int	rc;

	APMSEC_RXLOG("Process completed tkn 0x%p active %d",
		     tkn, atomic_read(&sec_ctx.active));
	
	req = tkn->context;
	session = crypto_tfm_ctx(req->tfm);

	if ((rc = atomic_dec_return(&sec_ctx.active)) < 0) {
		APMSEC_RXLOG("invalid active %d", rc);
		BUG();
	}
	
	result_tkn = TKN_CTX_RESULT_TKN(tkn);
	APMSEC_DEBUG_DUMP("Result SW Token", result_tkn, TKN_RESULT_HDR_MAX_LEN);
	
	if (crypto_tfm_alg_type(req->tfm) == CRYPTO_ALG_TYPE_ABLKCIPHER) {
		rctx = ablkcipher_request_ctx(ablkcipher_request_cast(req));
	} else if (crypto_tfm_alg_type(req->tfm) == CRYPTO_ALG_TYPE_AHASH) {
		rctx = ahash_request_ctx(ahash_request_cast(req));
	} else {
		struct aead_request *aead = req->data;
		rctx = aead_request_ctx(aead);
	}

	if (result_tkn->EXX || result_tkn->E15) {
		if (result_tkn->EXX & (TKN_RESULT_E9 |
				  TKN_RESULT_E10 |
				  TKN_RESULT_E11 |
				  TKN_RESULT_E12 |
				  TKN_RESULT_E13)) {
				rc = -EBADMSG;
			} else {
				rc = -ENOSYS;
			}
			APMSEC_ERR("EIP96 hardware error %d", rc);			
#ifdef APM_SEC_DEBUGEXTRA
			apm_sec_dump_src_dst_buf(req, tkn);
#endif
			goto out;
	}
	if (result_tkn->H || result_tkn->L || result_tkn->N ||
		   result_tkn->C || result_tkn->B) {
		/* packet exceeded 1792 bytes, result appended at end
		* of result data
		*/
		APMSEC_ERR("Unexpected result token with appeded data");
	}

	if (tkn->dest_mem)
		   apm_cp_buf2sg(tkn, req);

	   esp_nh_padlen = result_tkn->next_hdr_field |
		   (result_tkn->pad_length << 8);
	   rc = esp_nh_padlen ? esp_nh_padlen : 0;

out:
	apm_sec_tkn_free(session, tkn);
	
	if (rctx->sa) {
		apm_sec_sa_free(session, rctx->sa);
		rctx->sa = NULL;
	}
	/* Notify packet completed */
	req->complete(req, rc);
	return 0;
}

/** Error Callback Handler */
static irqreturn_t apm_sec_intr_cb(int irq, void *id)
{
	u32 stat;

	/* Determine what causes interrupt */
	apm_sec_rd32(SEC_GLB_CTRL_CSR_BLOCK, CSR_GLB_SEC_INT_STS_ADDR, &stat);
	if (stat & EIP96_MASK)
		apm_sec_intr_hdlr(); /* EIP96 interrupted */
        
	/* Clean them */
	apm_sec_wr32(SEC_GLB_CTRL_CSR_BLOCK, CSR_GLB_SEC_INT_STS_ADDR, stat);
        
	return IRQ_HANDLED;
}

static void apm_sec_bh_tasklet_cb(unsigned long data)
{
	struct apm_qm_mb_tasklet *sec_data = (struct apm_qm_mb_tasklet *)data;
	u32 mailbox = sec_data->mailbox;
	int count = 8;

        while (count--) {
		u32 rtkn;
		struct sec_tkn_ctx *ptkn;

                while ((rtkn = apm_qm_pull_comp_msg(mailbox)) != -1) {
			ptkn = (struct sec_tkn_ctx *)rtkn;
			if (likely(ptkn)) {
				apm_sec_tkn_cb(ptkn);
			}
			else
				APMSEC_ERR("Null Token from QMsg");
		}
	}

	apm_qm_enable_mb_irq(mailbox);
}

extern unsigned int meraki_crypto_memory_allocated;

static int __devinit apm_sec_probe(struct platform_device *pdev)
{
	struct device_node *np = pdev->dev.of_node;
	struct resource	   res;
	u32 *val;
	u32 val_len;
	u32 data;
	int rc;
	u32 core_id = apm_processor_id();
	
	/* Determine if security feature supported or not */
	rc = apm86xxx_read_scu_reg(SCU_SOC_EFUSE_ADDR, &data);
	if (rc == 0 && (data & 0x00040000)) {
		printk(KERN_INFO
				"No APM 862xx SoC security accelerator detected");
		return -ENODEV;
	}

	memset(&sec_ctx, 0, sizeof(sec_ctx));

	INIT_LIST_HEAD(&sec_ctx.alg_list);
	INIT_LIST_HEAD(&sec_ctx.completed_tkn_queue);
	spin_lock_init(&sec_ctx.lock);
	atomic_set(&sec_ctx.active, 0);
	sec_ctx.irq = NO_IRQ;
	
	/* Detect lite verson */
	sec_ctx.qm_queue.no_tm = is_apm86xxx_lite();
	
	/* Load CSR base address */
	rc = of_address_to_resource(np, 0, &res);
	if (rc)
		return -ENODEV;
	/* Setup CSR address pointer */
	sec_ctx.sec_base_csr = ioremap(res.start, APM_SEC_CSR_SIZE);
	sec_ctx.sec_glb_ctrl_csr = sec_ctx.sec_base_csr +
			APM_SEC_GLBL_CTRL_CSR_OFFSET;
	sec_ctx.eip96_axi_csr = sec_ctx.sec_base_csr +
			APM_EIP96_AXI_CSR_OFFSET;
	sec_ctx.eip96_csr = sec_ctx.sec_base_csr + APM_EIP96_CSR_OFFSET;
	sec_ctx.eip96_core_csr = sec_ctx.sec_base_csr +
			APM_EIP96_CORE_CSR_OFFSET;
	sec_ctx.sec_glb_diag_csr = sec_ctx.sec_base_csr +
			APM_SEC_GLBL_DIAG_OFFSET;
	/* Initialize software queue to 1 */
	crypto_init_queue(&sec_ctx.queue, 1);

	/* Retrive non-cacheable memory for SA/Token/Link list */
#if defined(CONFIG_APM86xxx_IOCOHERENT)
	val = (u32 *) of_get_property(np, "dma-reg", &val_len);
	if (val == NULL || val_len < 3) {
		printk(KERN_ERR "security core requires DTS dma-reg for "
				"IO coherent mode\n");
		return -ENODEV;
	}
	if ((val[0] == 0 && val[1] < memory_limit) || !meraki_crypto_memory_allocated) {
		printk("Mem boot parameter (0x%x) should be smaller than the AM862xx's dma buffer (mapped at 0x%x)\n", memory_limit, val[1]);
		return -ENODEV;
	}
	sec_ctx.dma_paddr = ((u64) val[0] << 32) | val[1];
	sec_ctx.dma_size = val[2];
	sec_ctx.dma_vaddr = ioremap_nocache(sec_ctx.dma_paddr,
					    sec_ctx.dma_size);
	if (sec_ctx.dma_vaddr == NULL) {
		printk(KERN_ERR "unable to map dma-reg memory\n");
		return -ENODEV;
	}
	APMSEC_LOG("coherent memory PADDR 0x%010llX VADDR 0x%p size %d",
		   sec_ctx.dma_paddr, sec_ctx.dma_vaddr, sec_ctx.dma_size);
	apm_lsec_coh_init();
#endif

	val = (u32 *) of_get_property(np, "sharemode", &val_len);
	if (val == NULL) 
		sec_ctx.sharemode = 0;
	else
		sec_ctx.sharemode = *val ? 1 : 0;
		
	/* Load TM configuration parameter */
	if (!sec_ctx.qm_queue.no_tm) {
		struct apm_sec_qm_queues *qcfg = &sec_ctx.qm_queue;		
		val = (u32 *) of_get_property(np, "tm", &val_len);
		if (val == NULL || *val <= QM_CFG_ARB_NO || 
		    *val > QM_CFG_ARB_AVB) {
probe_no_tm:	
			qcfg->no_tm = 1;
			qcfg->vqid_alg = QM_CFG_ARB_NO;
		} else {
			qcfg->vqid_alg = *val;
			if (qcfg->vqid_alg >= QM_CFG_ARB_DRR &&
			    qcfg->vqid_alg <= QM_CFG_ARB_AVB) {
			    	int i;
				val = (u32 *) of_get_property(np, "tm-param", 
								&val_len);
				if (val == NULL) {
					APMSEC_ERR("tm-param does not "
						"existed");
					goto probe_no_tm;
				}
				if (val_len != sizeof(u32)*TOTAL_COS) {
					APMSEC_ERR("tm-param invalid %d != %d",
						val_len, 
						sizeof(u32)*TOTAL_COS);
					goto probe_no_tm;
				}
				for (i = 0; i < TOTAL_COS; i++)			
			       		qcfg->tx_qid_param[i] = val[i]; 		
			}
		}
	}
	
	if (sec_ctx.sharemode) {
		/* Lock access and check if initialized? */
		/* FIXME */
	}
	
	APMSEC_DEBUG("Initialize the security hardware");
	/* Take IP out of reset */
	rc = apm_sec_hwreset();
	if (rc != 0) {
		APMSEC_ERR("unable to release HW from reset error %d", rc);
		goto err_hw_reset;
	}
	
	/* Remove Security CSR memory shutdown */
	if ((rc = apm86xxx_disable_mem_shutdown(
		    (u32 __iomem *)(sec_ctx.sec_glb_diag_csr +
		    SEC_CFG_MEM_RAM_SHUTDOWN_ADDR),
	SEC_EIP96_MASK)) != 0) {
		APMSEC_ERR("Failed to remove Security CSR memory shutdown");
		goto err_hw;
	}
		
	/* Initialize the security hardware */
	rc = apm_sec_hwinit();
	if (rc != 0)
		goto err_hw;
	/* Start the security hardware */
	rc = apm_sec_hwstart();
	if (rc != 0)
		goto err_hw;
	
	/* QM queue configuration */
	if (apm_sec_qconfig(&sec_ctx)) {
		APMSEC_ERR("Error in queue configuration");
		goto err_hw;
	}
	
	if (sec_ctx.sharemode) {
		/* Unlock access */
		/* FIXME */
	}
	
	dev_set_drvdata(&pdev->dev, &sec_ctx);
	
	/* Setup IRQ */
	sec_ctx.irq = of_irq_to_resource(np, 0, NULL);
	if (sec_ctx.irq == NO_IRQ) {
		APMSEC_DEBUG("No IRQ in DTS");
	} else {
		/* Register for irq */
		rc = request_irq(sec_ctx.irq, apm_sec_intr_cb, 0,
				 "crypto", NULL);
		if (rc != 0)
			APMSEC_ERR("security core can not register "
					"for interrupt %d", sec_ctx.irq);
	}
#ifdef CONFIG_SMP
	int i;
	for (i = 0; i < MAX_CORES; i++) {
		apm_qm_mb_tasklet_register(
			sec_ctx.qm_queue.comp_mbid[i],
			sec_ctx.qm_queue.comp_qid[i],
			rc, &sec_ctx,
			apm_sec_bh_tasklet_cb);
	}
#else
	apm_qm_mb_tasklet_register(
		sec_ctx.qm_queue.comp_mbid[core_id],
		sec_ctx.qm_queue.comp_qid[core_id],
		core_id, &sec_ctx,
		apm_sec_bh_tasklet_cb);
#endif

	/* Register security algorithms with Linux CryptoAPI */
	rc = apm_sec_register_alg();
	if (rc)
		goto err_reg_alg;

	APMSEC_LOG("APM 862xx SoC %ssecurity accelerator driver v%s%s",
		   sec_ctx.qm_queue.no_tm ? "" : "TM ",
		   APMSEC_VERSION_STR, sec_ctx.sharemode ? " share mode" : "");
	
	return 0;
	
err_reg_alg:
		apm_sec_unregister_alg();
	if (sec_ctx.irq != NO_IRQ)
		free_irq(sec_ctx.irq, "crypto");
	if (!sec_ctx.sharemode)
		apm_sec_hwstop();
	return rc;
	
err_hw:
		if (!sec_ctx.sharemode) {
	apm_sec_hwstop();
		} else {
			/* Unlock access */
			/* FIXME */
		}
		return rc;
			
err_hw_reset:	
		if (sec_ctx.sharemode) {
	/* Unlock access */
	/* FIXME */
		}
		return rc;
}

static int __devexit apm_sec_remove(struct platform_device *pdev)
{
	struct apm_sec_ctx * ctx = dev_get_drvdata(&pdev->dev);
	
	/* Un-register with Linux CryptoAPI */
	apm_sec_unregister_alg();
	
	/* Stop hardware */
	if (!ctx->sharemode)
		apm_sec_hwstop();
	
	/* Clean up pending queue */
	/* FIXME - ctx->queue */
	/* Clean up pending completed token */
	/* FIXME - ctx->completed_tkn_queue */

	APMSEC_LOG("unloaded APM 862xx SoC security accelerator driver");

	return 0;
}

static struct of_device_id apm_sec_match[] = {
	{ .compatible      = "apm,apm86xxx-crypto", },
	{ },
};

static struct platform_driver apm_sec_driver = {
	.probe		= apm_sec_probe,
	.remove		= apm_sec_remove,
	.driver = {
		.name		= "apm-crypto",
		.of_match_table	= apm_sec_match,
	},
};

static int __init apm_sec_init(void)
{
	return platform_driver_register(&apm_sec_driver);
}

static void __exit apm_sec_exit(void)
{
	platform_driver_unregister(&apm_sec_driver);
}

module_init(apm_sec_init);
module_exit(apm_sec_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Loc Ho <lho@apm.com>");
MODULE_DESCRIPTION("Driver for APM 862xx SoC security accelerator");
