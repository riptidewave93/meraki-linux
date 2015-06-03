/**
 * Applied Micro APM86xxx SoC PktDMA Driver
 *
 * Copyright (c) 2010 Applied Micro Circuits Corporation.
 * Author: Shasi Pulijala <spulijala@apm.com>
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
 * This file contains Linux platform specific PktDMA driver code for APM86xxx 
 * SoC.
 *
 */
#include <linux/kernel.h>
#include <linux/moduleparam.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/device.h>
#include <linux/proc_fs.h>
#include <linux/module.h>
#include <linux/of_platform.h>
#include <linux/clk.h>
#include <asm/uaccess.h>
#include <asm/ipp.h>
#include <asm/apm86xxx_pm.h>
#include <asm/apm86xxx_soc.h>
#include <asm/apm_qm_cfg.h>

#include "apm_pktdma_access.h"
#include "apm_dma_csr.h"
#include "apm_pktdma_core.h"

#define RES_SIZE(r)	((r)->end - (r)->start + 1)

struct iodma_pdev p_dev;	/* FIXME - pktdma */

struct apm_iodma_user_context {
	dma_async_tx_callback callback;
	void *callback_param;
};

int apm_pktdma_read_reg(u32 offset, u32 *data)
{
	*data = in_be32((void __iomem *) ((u8 *) p_dev.csr_base + offset));
	PKTDMA_DCSR("CSR RD: 0x%p Data: 0x%08X", 
		(u8 *) p_dev.csr_base + offset, *data);
	return 0;
}

int apm_pktdma_read_glb_reg(u32 offset, u32 *data)
{
	*data = in_be32((void __iomem *) ((u8 *) p_dev.csr_base_glb + offset));
	PKTDMA_DCSR("CSR RD: 0x%p Data: 0x%08X",
	       (u8 *) p_dev.csr_base_glb + offset, *data);
	return 0;
}

int apm_pktdma_read_qmi_reg(u32 offset, u32 *data)
{
	*data = in_be32((void __iomem *) ((u8 *) p_dev.csr_base_qmi + offset));
	PKTDMA_DCSR("CSR RD: 0x%p Data: 0x%08X",
		    (u8 *) p_dev.csr_base_qmi + offset, *data);
	return 0;
}

int apm_pktdma_write_qmi_reg(u32 offset, u32 data)
{
	PKTDMA_DCSR("CSR WR: 0x%p Data: 0x%08X",
		    (u8 *) p_dev.csr_base_qmi + offset, data);
	out_be32((void __iomem *) ((u8 *) p_dev.csr_base_qmi + offset), data);
	return 0;
}

int apm_pktdma_write_reg(u32 offset, u32 data)
{
	PKTDMA_DCSR("CSR WR: 0x%p Data: 0x%08X",
	       (u8 *) p_dev.csr_base + offset, data);
	out_be32((void __iomem *) ((u8 *) p_dev.csr_base + offset), data);
	return 0;
}

static int apm_iodma_handle_done(struct apm_pktdma_op_result *pktdma_result)
{
	struct apm_iodma_user_context *uc;
	
	uc = (struct apm_iodma_user_context *) pktdma_result->ctx;
	if (pktdma_result->err) {
		printk("Operation error 0x%08X\n", pktdma_result->err);
	}
	if (uc->callback)
		uc->callback(uc->callback_param);
	kfree(uc);
	return 0;
}

/**
 * Operation Management Functions
 */

struct iodma_op_state *apm_pktdma_op_get(void)
{
	struct iodma_op_state *op;
	unsigned long flags;

	spin_lock_irqsave(&p_dev.cache_lock, flags);
	if (!list_empty(&p_dev.qinfo.op_cache)) {
		struct list_head *entry = p_dev.qinfo.op_cache.next;
		list_del(entry);
		op = list_entry(entry, struct iodma_op_state, next);
		--p_dev.qinfo.op_cache_cnt;
		APM_PKTDMA_DOP_CACHE("allocate op cached 0x%p", op);
		spin_unlock_irqrestore(&p_dev.cache_lock, flags);
		return op;
	}
	spin_unlock_irqrestore(&p_dev.cache_lock, flags);

	op = kmalloc(sizeof(struct iodma_op_state) + 
			sizeof(struct apm_iodma_user_context), GFP_KERNEL);
	if (op == NULL)
		goto done;
	APM_PKTDMA_DOP_CACHE("allocate op 0x%p", op);
done:
	return op;
}

void apm_pktdma_op_free(struct iodma_op_state *op)
{	
	unsigned long flags;
	
	if (p_dev.qinfo.op_cache_cnt >= APM_PKTDMA_OP_CACHE_MAX) {
		APM_PKTDMA_DOP_CACHE("free op 0x%p", op);
		kfree(op);
		return;
	}

	spin_lock_irqsave(&p_dev.cache_lock, flags);
	++p_dev.qinfo.op_cache_cnt;
	list_add(&op->next, &p_dev.qinfo.op_cache);
	APM_PKTDMA_DOP_CACHE("free op cached 0x%p", op);
	spin_unlock_irqrestore(&p_dev.cache_lock, flags);

}

int apm_pktdma_msg_rx(struct apm_qm_msg_desc *rx_msg_desc)
{
	struct apm_pktdma_msg_done *comp_msg = rx_msg_desc->msg;
	struct iodma_op_state *op;
	unsigned long flags;
	struct pktdma_percpu_data *percpu_data;
	int cpu;
	
	op = (struct iodma_op_state *) comp_msg->dmsg1_2.uinfo;
	
	if (comp_msg->dmsg1_2.NV)
		memcpy(&op->dmsg, comp_msg, 64);
	else
		memcpy(&op->dmsg, comp_msg, 32);

#ifdef APM_PKTDMA_DRXTX
	PKTDMA_DEBUG_DUMP("compl msg ", &op->dmsg,
			  comp_msg->dmsg1_2.NV ? 64 : 32);
#endif

	cpu = get_cpu();
	percpu_data = (struct pktdma_percpu_data *)
			per_cpu_ptr(p_dev.percpu_ptr, cpu);

	spin_lock_irqsave(&percpu_data->lock, flags);
	list_add_tail(&op->next, &percpu_data->head);
	spin_unlock_irqrestore(&percpu_data->lock, flags);

	tasklet_schedule(&percpu_data->rx_tasklet);

	return IRQ_HANDLED;
}

static void apm_pktdma_bh_tasklet_cb(unsigned long data)
{
	struct list_head *pos;
	struct list_head *tmp;	
	struct iodma_pdev *dma_dev;
	unsigned long flags;
	struct pktdma_percpu_data *percpu_data;
	int cpu;

	dma_dev = (struct iodma_pdev *) data;
	cpu = get_cpu();
	percpu_data = (struct pktdma_percpu_data *)
			per_cpu_ptr(dma_dev->percpu_ptr, cpu);

	/* Process completed pending operation */
	spin_lock_irqsave(&percpu_data->lock, flags);
	list_for_each_safe(pos, tmp, &percpu_data->head) {
		struct iodma_op_state *item;
		item = list_entry(pos, struct iodma_op_state, next);
		list_del(pos);
		spin_unlock_irqrestore(&percpu_data->lock, flags);
		apm_pktdma_op_cb(item);
		spin_lock_irqsave(&percpu_data->lock, flags);
	}
	spin_unlock_irqrestore(&percpu_data->lock, flags);
}

int apm_pktdma_get_fp(struct apm_pktdma_buf_info *fp_buf)
{
	struct apm_qm_msg_desc msg_desc;
	struct apm_qm_msg16 msg;
	int rc;

	msg_desc.msg = &msg;
	msg_desc.mb_id = p_dev.qinfo.queues[apm_processor_id()].ppc_fpmbid;
	msg_desc.qid = p_dev.qinfo.queues[apm_processor_id()].ppc_fpid;
	PKTDMA_DRXTX("Retrieve FP PPC QID %d MID %d", 
		msg_desc.qid, msg_desc.mb_id);
	rc = apm_qm_fp_alloc_buf(&msg_desc);
	if (rc) {
		PKTDMA_ERR("Failed to allocate FP buffer QID %d error %d",
			msg_desc.qid, rc);
		return rc;
	}
	if (msg.DataAddrMSB == 0 && msg.DataAddrLSB == 0) {
		PKTDMA_ERR("FP returned NULL pointer");
		return -ENOMEM;
	}
	PKTDMA_DRXTX("FP buffer PADDR 0x%02X.%08X len %d",
		msg.DataAddrMSB, msg.DataAddrLSB, msg.BufDataLen);
	
	/* Convert to virtual address in Linux */
	fp_buf->addr = phys_to_virt(MAKE64(msg.DataAddrMSB, msg.DataAddrLSB)); 
	fp_buf->fp_id = msg.FPQNum;
	fp_buf->datalen = msg.BufDataLen;

	return 0;
}
EXPORT_SYMBOL(apm_pktdma_get_fp);

int apm_pktdma_init_pool(int queue_id, int size, int no_of_buffs)
{
	struct apm_qm_msg_desc fp_msg_desc;
	struct apm_qm_msg16 msg;
	void *data_addr;
	u64 phy_addr;
	int i;

/*	memset(&msg, 0, sizeof(msg)); */
	
	fp_msg_desc.msg = &msg;
	fp_msg_desc.qid = queue_id;
	
	msg.BufDataLen = apm_buf_len_set(size); /* FIXME */
	msg.FPQNum = queue_id;
#if defined(CONFIG_NOT_COHERENT_CACHE)
	msg.C = 0;
#else
	msg.C = 1;
#endif	
	for (i = 0; i < no_of_buffs; i++) {
		/* Due to buffer used for link list mode of QM, 
		   buffer musts be 16B aligned. As Linux is 32B aligned,
		   we can just use actual buffer size */
		data_addr = kzalloc(size, GFP_KERNEL);
		if (!data_addr) {
			PKTDMA_ERR("Failed to allocate buffer size %d", size);
			return -ENOMEM;
		}
		//printk("data_addr = 0x%p\n", data_addr);
		phy_addr = virt_to_phys(data_addr);
		msg.UserInfo = (unsigned long) data_addr;
#if defined(CONFIG_NOT_COHERENT_CACHE)
		invalidate_dcache_range((u32)data_addr,
					 (u32)(data_addr + size));
#endif
		msg.DataAddrMSB = HIDWORD(phy_addr);
		msg.DataAddrLSB = LODWORD(phy_addr);
		
		/* Push buffer into HW free pool */
		if (apm_qm_fp_dealloc_buf(&fp_msg_desc) != 0) {
			PKTDMA_ERR(KERN_ERR "Can't fill FP ID %d", queue_id);
			return -EINVAL;
		}
	}
	return 0;
}

#define MAX_LOOP_POLL_TIMEMS	500
#define MAX_LOOP_POLL_CNT	10
#define ACCESS_DELAY_TIMEMS	(MAX_LOOP_POLL_TIMEMS / MAX_LOOP_POLL_CNT)

int apm_pktdma_fp_pb_flush(int pbn)
{
	u32 fp_buffer_reg, max_pbn;
	u32 no_fp;
	u32 data = 0;
	u32 j, buf_addr;
	int count, rc;
	
	if (pbn <= 7) {
		fp_buffer_reg = DMA_STSSSQMIFPBUFFER1_ADDR;
		max_pbn = 0;
	} else if (pbn <= 17) {
		fp_buffer_reg = DMA_STSSSQMIFPBUFFER2_ADDR;
		max_pbn = 8;
	}
	else {
		PKTDMA_ERR("Error: Invalid FP PBN: %d \n", pbn);
		return -1;
	}

	rc = apm_pktdma_read_qmi_reg(fp_buffer_reg, &data);
	no_fp = ( (data >> ((pbn - max_pbn) * 3))  & 0x7);
	PKTDMA_DEBUG("pbn: %d, pbn_data: 0x%08X,"
			"max_pbn: %d, no_fp: %d \n",
		pbn, data, max_pbn, no_fp);
	
	for (j = 0; j < (no_fp * 4) ; j++) {
		count = 0;
		buf_addr = j % 4;
		data = DMA_PBID_WR(pbn + 0x20) |
				DMA_NACK_WR(1);
		if (buf_addr == 3) {
			data |= DMA_LAST_WR(1) |
					DMA_POP_WR(1)  |
					DMA_READ_WR(0);
		} else {
			data |= DMA_LAST_WR(0) |
					DMA_POP_WR(0)  |
					DMA_READ_WR(1);
		}
		data |= DMA_BUFFERADDR_WR(buf_addr) |
				DMA_PUSH_WR(0) |
				DMA_WRITE_WR(0);
		
		PKTDMA_DEBUG("J:%d and data:0x%08X \n ", j, data);

		udelay(1000);
		rc = apm_pktdma_write_qmi_reg(DMA_CFGSSQMIDBGCTRL_ADDR, data);
		while(1) {
			rc = apm_pktdma_read_qmi_reg(DMA_CFGSSQMIDBGCTRL_ADDR, &data);
			
			if (!DMA_NACK_RD(data))
				break;
			
			udelay(ACCESS_DELAY_TIMEMS);
			if(count++ > MAX_LOOP_POLL_CNT) {
				printk("DMA PBN flush failed \n");
				goto exit;
			}
		}
		/* Read the data from PBN, We dont care though */
		rc = apm_pktdma_read_qmi_reg(DMA_STSSSQMIDBGDATA_ADDR, &data);
		PKTDMA_DEBUG("PBN Data is : 0x%08X \n", data);
	}
exit:
	rc = apm_pktdma_read_qmi_reg(fp_buffer_reg, &data);
	no_fp = ((data >> (max_pbn - pbn)) & 0x7);
	PKTDMA_DEBUG("After flush pbn: %d, pbn_data: 0x%08X,"
			"max_pbn: %d, no_fp: %d \n", pbn, data, max_pbn, no_fp);
	return no_fp;
}

void apm_pktdma_get_total_chan(struct dma_chan_info *ch_info)
{
	int i;

	ch_info->max_chan =  p_dev.qinfo.tx_ch_cnt;
	for (i = 0; i < ch_info->max_chan; i++)
		ch_info->chan_en[i] = p_dev.qinfo.chan[i];
}
EXPORT_SYMBOL(apm_pktdma_get_total_chan);

static int apm_pktdma_tx_qconfig(void)
{
	struct apm_qm_qstate *qstates;
	struct apm_qm_qalloc qalloc;
	int num_chan;
	int i;
	int j;
	int rc;

	memset(&qalloc, 0, sizeof(struct apm_qm_qalloc));
	
	if (p_dev.qinfo.no_tm) {
		num_chan = p_dev.qinfo.tx_ch_cnt;
		qstates = kzalloc(num_chan * sizeof(struct apm_qm_qstate), 
				GFP_KERNEL);
		if (qstates == NULL) {
			PKTDMA_ERR("Unable to allocate memory for queue "
				"states");
			return -ENOMEM;
		}
		memset(qstates, 0, num_chan * sizeof(struct apm_qm_qstate));
		qalloc.qstates = qstates;
		qalloc.qm_ip_blk = IP_BLK_QM;
		qalloc.ip_blk = IP_DMA;
		qalloc.q_type = P_QUEUE;
		qalloc.q_count = num_chan;
		qalloc.direction = DIR_EGRESS;
		qalloc.qsize = SIZE_64KB;
#ifdef CONFIG_SMP
		qalloc.ppc_id = 0;
#else
		qalloc.ppc_id = apm_processor_id();
#endif	
		qalloc.thr_set = 1;
			
		if ((rc = apm_qm_alloc_q(&qalloc)) != 0) {
			PKTDMA_ERR("Unable to allocate egress work queues "
				"for PKTDMA error %d", rc);
			return -EINVAL;
		}
		
		for (i = 0; i < p_dev.qinfo.tx_ch_cnt; i++) {
			struct pktdma_chan_qid *qcfg;
			
			if (p_dev.qinfo.chan[i] >= PKTDMA_CHID_MAX)
				continue;
			qcfg = &p_dev.qinfo.tx_q[p_dev.qinfo.chan[i]];
			for (j = 0; j < APM_PKTDMA_NUM_COS; j++) {
				qcfg->tx_qid[j] = qstates[i].q_id;				
				PKTDMA_DEBUG("Channel %d COS %d QID %d", 
					p_dev.qinfo.chan[i], j, 
					qcfg->tx_qid[j]);
			}
		}
		kfree(qstates);
		return 0;
	}
	
	/* TM configuration */
	qstates = kzalloc(sizeof(struct apm_qm_qstate), GFP_KERNEL);
	if (qstates == NULL) {
		PKTDMA_ERR("Unable to allocate memory for queue states");
		return -ENOMEM;
	}
	
	for (j = 0; j < p_dev.qinfo.tx_ch_cnt; j++) {
		struct pktdma_chan_qid *qcfg;
		
		qcfg = &p_dev.qinfo.tx_q[p_dev.qinfo.chan[j]];
						
		qalloc.qstates = qstates;
		qalloc.qm_ip_blk = IP_BLK_QM;
		qalloc.ip_blk = IP_DMA;
#ifdef CONFIG_SMP
		qalloc.ppc_id = 0;
#else
		qalloc.ppc_id = apm_processor_id();
#endif	
		qalloc.q_count = 1;
		qalloc.direction = DIR_EGRESS;
		qalloc.qsize = SIZE_64KB;
		qalloc.thr_set = 1;
		
		/* Retrieve a VQ id */
		qcfg->tx_vqid = apm_qm_get_vq(IP_DMA);
		PKTDMA_DEBUG("Channel %d TM VQID %d Alg %d", 
			p_dev.qinfo.chan[j], qcfg->tx_vqid, qcfg->tx_vqid_alg);
		
		/* Create children PQs */
		qalloc.q_type    = QM_CFG_QTYPE_PQ;
		qalloc.parent_vq = qcfg->tx_vqid;
		qalloc.vqen      = 1;		
		for (i = 0; i < APM_PKTDMA_NUM_COS; i++) {
			memset(qstates, 0, sizeof(struct apm_qm_qstate));
			if ((rc = apm_qm_alloc_q(&qalloc)) != 0) {
				PKTDMA_ERR(KERN_ERR  
					"could not allocate child PQ error %d",
					rc);
				kfree(qstates);
				return rc;
			}
			qcfg->tx_qid[i] = qstates->q_id;
		}
		
		/* Create parent VQ */
		qalloc.q_type    = QM_CFG_QTYPE_VQ;
		qalloc.parent_vq = 0;
		qalloc.vqen      = 0;
		for (i = 0; i < 8; i++) {
			if (i >= APM_PKTDMA_NUM_COS) {
				/* Unused entry must be the VQ id */
				qalloc.pq_sel[i] = qcfg->tx_vqid;
				continue;
			}
			qalloc.pq_sel[i] = qcfg->tx_qid[i];
			qalloc.q_sel_arb[i] = qcfg->tx_vqid_alg;
			qalloc.shape_rate[i] = qcfg->tx_qid_param[i];
			PKTDMA_DEBUG("TM PQ %d Param %d", 
				qcfg->tx_qid[i], qcfg->tx_qid_param[i]);
		}
		
		memset(qstates, 0, sizeof(struct apm_qm_qstate));
		rc = apm_qm_alloc_vq(&qalloc, qcfg->tx_vqid);
		if (rc != 0) {
			PKTDMA_ERR("could not allocate VQ");
			kfree(qstates);
			return rc;
		}
	}
	kfree(qstates);
	return 0;
}

static inline int apm_pktdma_cpuid2ip(int cpuid)
{	
	if (cpuid == 0)
		return IP_PPC0;
	return IP_PPC1;
}		

static int apm_pktdma_qconfig(void)
{
	struct apm_qm_qstate *qstate;
	struct apm_qm_qalloc qalloc;
	int err;
	int i;

	memset(&qalloc, 0, sizeof(struct apm_qm_qalloc));
	
	/* Allocate tx queues for PKTDMA Channels */
	err = apm_pktdma_tx_qconfig();
	if (err)
		return err;
	
	qstate = kmalloc(sizeof(struct apm_qm_qstate), GFP_KERNEL);
	if (qstate == NULL) {
		PKTDMA_ERR("Can not allocate memory for queues state");
		return -ENOMEM;
	}
	/* Allocate egress work queue for PktDMA */
	for (i = 0; i < NUM_CPUS; i++) {
		
		struct apm_qm_qstate *tmp_qstate;
		struct pktdma_queue_ids *dmaq = &p_dev.qinfo.queues[i];
		
		if (!dmaq->valid)
			continue;
		/* Allocate ingress completion queue for PktDMA */
		tmp_qstate = get_comp_queue(IP_DMA, i);
		dmaq->comp_qid = tmp_qstate->q_id;
		dmaq->comp_mb = tmp_qstate->mb_id;
#ifdef CONFIG_SMP
		apm_qm_set_mb_affinity(dmaq->comp_mb, i);
#endif
		/* Allocate free pool for PktDMA */
		memset(qstate, 0, sizeof(*qstate));
		qalloc.qm_ip_blk = IP_BLK_QM;
		qalloc.ip_blk = IP_DMA;
		qalloc.ppc_id = i;
		qalloc.q_type = FREE_POOL;
		qalloc.q_count = 1;
		qalloc.direction = DIR_EGRESS;
		qalloc.qsize = SIZE_64KB;
		qalloc.qstates = qstate;
			
		if ((err = apm_qm_alloc_q(&qalloc)) != 0) {
			PKTDMA_ERR("Unable to allocate free pool");
			return -EINVAL;
		}
	
		dmaq->rx_fp_qid = qstate->q_id;
		dmaq->rx_fp_pbn = qstate->pbn - 0x20; /* FIXME Why - 0x20 */
		
		/* Allocate free pool processor */
		memset(qstate, 0, sizeof(*qstate));
		qalloc.qm_ip_blk = IP_BLK_QM;
		qalloc.ip_blk = apm_pktdma_cpuid2ip(i);
		qalloc.ppc_id = i;
		qalloc.q_type = FREE_POOL;
		qalloc.q_count = 1;
		qalloc.direction = DIR_EGRESS;
		qalloc.qsize = SIZE_64KB;
		qalloc.qstates = qstate;
		if ((err = apm_qm_alloc_q(&qalloc)) != 0) {
			PKTDMA_ERR("Unable to allocate free pool");
			return -EINVAL;
		}
			
		dmaq->ppc_fpid = qstate->q_id;
		dmaq->ppc_fpmbid = qstate->mb_id;
		
		PKTDMA_DRXTX("CPU%d CQID %d CMID %d FP QID %d DMA FP: PBN %d, PPC FPID %d "
				"PPC MBID %d", i, dmaq->comp_qid, dmaq->comp_mb,
			dmaq->rx_fp_qid, dmaq->rx_fp_pbn, dmaq->ppc_fpid, dmaq->ppc_fpmbid);
	}
	
	return 0;
}

static int apm_pktdma_fpool_init(void)
{
	int i, rc;
	
	for (i = 0; i < NUM_CPUS; i++) {
		struct pktdma_queue_ids *queue = &p_dev.qinfo.queues[i];
		
		if (!queue->valid)
			continue;
		rc = apm_pktdma_init_pool(queue->rx_fp_qid,
					  FREE_POOL_DMA_BUFFER_SIZE,
					  FREE_POOL_DMA_BUFFER_NUM);
		if (rc) {
			PKTDMA_ERR("Failed to initialize free pool QID %d",
				queue->rx_fp_qid);
			return -EINVAL;
		}

		rc = apm_pktdma_init_pool(queue->ppc_fpid,
					  FREE_POOL_PPC_BUFFER_SIZE,
					  FREE_POOL_DMA_BUFFER_NUM);
		if (rc) {
			PKTDMA_ERR("Failed to initalize free pool QID %d",
				queue->ppc_fpid);
			return -EINVAL;
		}
	}
	
	return 0;	
}

static int apm_pktdma_is_enable(void)
{
	u32 data;
	
	apm_pktdma_read_reg(DMA_GCR_ADDR, &data);
	
	return DMA_EN_RD(data) ? 1 : 0;
}

static int apm_pktdma_enable_clk(void)
{
	struct clk *clk;
	
	/* Reset PktDMA */
	clk = clk_get(NULL, "pktdma");
	if (IS_ERR(clk)) {
		PKTDMA_ERR("failed to get pktdma clock device");
		return -ENODEV;
	}
	clk_enable(clk);

	return 0;
}

static int apm_pktdma_enable_hw(void)
{
	u32 data;

	/* Enable engine */
	apm_pktdma_read_reg(DMA_GCR_ADDR, &data);
	data |= DMA_EN_WR(1);
	data |= (DMA_QUEUE_MASK + DMA_OUTSTANDING_MASK);
	apm_pktdma_write_reg(DMA_GCR_ADDR, data);

	return 0;
}

static dma_cookie_t apm_pktdma_tx_submit(struct dma_async_tx_descriptor *tx)
{
	struct apm_pktdma_m2m_params m2m;
	struct apm_iodma_user_context *uc;
	int ret;

	uc = kzalloc(sizeof(struct apm_iodma_user_context), GFP_KERNEL);
	if (!uc)
		return -ENOMEM;
	uc->callback = tx->callback;
	uc->callback_param = tx->callback_param;
	/* Get info from tx_descriptor and set in iodma_m2m */
	memset(&m2m, 0, sizeof(struct apm_pktdma_m2m_params));
	/* remap async channels to h/w channels */
	m2m.chid = tx->chan->chan_id;
	m2m.m2m_mode = IODMA_COPY;
	m2m.sg_count = 0;
	m2m.sa = &p_dev.ch_data[tx->chan->chan_id].src_addr;
	m2m.da = &p_dev.ch_data[tx->chan->chan_id].dest_addr;
	m2m.byte_count = &p_dev.ch_data[tx->chan->chan_id].len_arr;
	m2m.cb = apm_iodma_handle_done;
	m2m.context = (void *) uc;
	m2m.cos = IODMA_DEFAULT_COS;
	
	PKTDMA_DRXTX("Issuing m2m 0x%p len %d", tx, 
		p_dev.ch_data[tx->chan->chan_id].len_arr);
	
	ret = apm_pktdma_m2m_xfer(&m2m);
	if (ret) {
		PKTDMA_ERR("m2m_xfer error %d", ret);
		kfree(uc);
		return (dma_cookie_t) NULL;
	}
	return (dma_cookie_t) m2m.context;
}

/* Returns the number of allocated descriptors */
static int apm_pktdma_alloc_resources(struct dma_chan *chan)
{
	/* Assuming this will be called only once per channel */
	struct dma_async_tx_descriptor *async_tx;
	
	PKTDMA_DEBUG("Alloc async resource for %d", chan->chan_id);
	async_tx = kzalloc(sizeof(struct dma_async_tx_descriptor), GFP_KERNEL);
	if (!async_tx) {
		PKTDMA_ERR("APM IODMA async_tx mem alloc failed for "
			"channel: %d", chan->chan_id);
		return 0;
	}
	dma_async_tx_descriptor_init(async_tx, chan);
	async_tx->tx_submit = apm_pktdma_tx_submit;
	p_dev.ch_data[chan->chan_id].async_tx = async_tx;
	return 1;
}

static void apm_pktdma_free_resources(struct dma_chan *chan)
{
	/* Assuming this will be called only once per channel */
	kfree(p_dev.ch_data[chan->chan_id].async_tx);
	p_dev.ch_data[chan->chan_id].async_tx = NULL;
}

/**
 * ppro_iodma_is_complete - poll the status of an ADMA transaction
 * @chan: ADMA channel handle
 * @cookie: ADMA transaction identifier
 */
static enum dma_status apm_pktdma_is_complete(struct dma_chan *chan,
					      dma_cookie_t cookie,
					      dma_cookie_t *done,
					      dma_cookie_t *used)
{
	return DMA_SUCCESS;
}

/**
 * apm_iodma_issue_pending - flush all pending descriptors to h/w
 */
static void apm_pktdma_issue_pending(struct dma_chan *chan)
{
	/* stub function */
}

static struct dma_async_tx_descriptor *apm_pktdma_prep_memcpy(struct dma_chan
		*chan, dma_addr_t dest, dma_addr_t src, size_t len,
		unsigned long flags)
{
	unsigned long flags_lock = 0;
	if (p_dev.ch_data[chan->chan_id].async_tx) {
		spin_lock_irqsave(&p_dev.async_lock, flags_lock);
		p_dev.ch_data[chan->chan_id].dest_addr = dest;
		p_dev.ch_data[chan->chan_id].src_addr = src;
		p_dev.ch_data[chan->chan_id].len_arr = len;
		spin_unlock_irqrestore(&p_dev.async_lock, flags_lock);
		return p_dev.ch_data[chan->chan_id].async_tx;
	}
	return NULL;
}

#ifdef CONFIG_PM
static int apm_pktdma_suspend(struct of_device * dev, pm_message_t state)
{

	if (state.event & PM_EVENT_FREEZE) {
		/* To hibernate */
	} else if (state.event & PM_EVENT_SUSPEND) {
		p_dev.poweroff = 1;
		/* To suspend */
	} else if (state.event & PM_EVENT_RESUME) {
		/* To resume */
	} else if (state.event & PM_EVENT_RECOVER) {
		/* To recover from enter suspend failure */
	}

	PKTDMA_ERR("PKTDMA Suspend...");
	return 0;
}

static int apm_pktdma_resume(struct of_device* dev)
{
	int rc = 0 /* i */;

	if (!resumed_from_deepsleep())
		return rc;

	if (p_dev.poweroff) {
		/* Reset the PKTDMA hardware */
		rc = apm_pktdma_enable_clk(); /* Re-enable clock */
		if (rc != 0) {
			PKTDMA_ERR("PKTDMA HW Reset failed after powerdown");
			goto err;
		}
		/* Initialize the PKTDMA hardware */
		rc = apm_pktdma_enable_hw();
		if (rc != 0) {
			PKTDMA_ERR("PKTDMA HW Init failed after powerdown");
			goto err;
		}

		p_dev.poweroff = 0;
		PKTDMA_ERR("PKTDMA Resumed");
	}
err:	
	return rc;
}
#else
#define apm_pktdma_suspend NULL
#define apm_pktdma_resume NULL
#endif /* CONFIG_PM */

/* Called when system is shutwon */
static int apm_pktdma_shutdown(struct of_device *ofdev)
{
	int rc = 0;
#ifndef CONFIG_SMP
	return rc;
#else
	int i;
	int j;
	int rx_fp_pbn = -1;

	if (!cpu_enabled(1))
		return rc;

	PKTDMA_DEBUG("PKTDMA Shutdown Begins...");	
	for (i = 0; i < p_dev.qinfo.tx_ch_cnt; i++) {
		if (p_dev.qinfo.chan[i] >= PKTDMA_CHID_MAX)
			continue;
		for (j = 0; j < APM_PKTDMA_NUM_COS; j++) {
			PKTDMA_DEBUG("Channel %d TX QID %d",
				p_dev.qinfo.chan[i], 
				p_dev.qinfo.tx_q[p_dev.qinfo.chan[i]].tx_qid[j]);
		}
	}
	for (i = 0; i < NUM_CPUS; i++) {
		struct pktdma_queue_ids *dmaq = &p_dev.qinfo.queues[i];
		if (!dmaq->valid)
			continue;
		PKTDMA_DEBUG("Completion QID: %d\n", dmaq->comp_qid);
		PKTDMA_DEBUG("Completion MB: %d\n", dmaq->comp_mb);
		PKTDMA_DEBUG("FP QID DMA: %d\n", dmaq->rx_fp_qid);
		PKTDMA_DEBUG("FP PBN DMA: %d\n", dmaq->rx_fp_pbn);
		PKTDMA_DEBUG("FP QID PPC: %d\n", dmaq->ppc_fpid);
		PKTDMA_DEBUG("FP MB  PPC: %d\n", dmaq->ppc_fpmbid);
		rx_fp_pbn = dmaq->rx_fp_pbn;
	}

	if (mfspr(SPRN_PIR)) {
		apm_qm_shutdown();
	}
	if (rx_fp_pbn < 0) {
		PKTDMA_DEBUG("Invalid pn number %d", rx_fp_pbn);
		return 0;
	}
	PKTDMA_DEBUG("Flush PKTDMA Prefetch Buffer %d", rx_fp_pbn);
	rc = apm_pktdma_fp_pb_flush(rx_fp_pbn);
	if (rc) {
		PKTDMA_DEBUG("FP buffers still existing in the PBN %d", 
			rx_fp_pbn);
	} else {
		PKTDMA_DEBUG("FP Buffers Flushed Successfully");
		PKTDMA_DEBUG("PKTDMA Shutdown Ends");
	}

	return rc;
#endif
}

static int __devinit apm_pktdma_of_probe(struct of_device *ofdev,
					const struct of_device_id *match)
{
	struct device *dev = &ofdev->dev;
	struct device_node *np = ofdev->node;
	struct dma_device *dma_dev;
	struct apm_pktdma_chan *chan;
	struct resource	res;
	const u32 *ch_info;
	const u32 *tm_info;
	const u32 *tm_param_info;
	int tm_info_len;
	int tm_param_info_len;
	int chan_id;
	int end_chan;
	int rc, i, cpu;
	u32 len;
	void *percpu_ptr;
	u32 data;

	memset(&p_dev, 0, sizeof(p_dev));
	
	dev_set_drvdata(dev, &p_dev);
	p_dev.ofdev = ofdev;
	p_dev.device = dev;

	/* Determine if XOR and TM features supported */
	p_dev.qinfo.no_xor = is_apm86xxx_lite() ? 1 : 0;
	p_dev.qinfo.no_tm = is_apm86xxx_lite() ? 1 : 0;

	rc = of_address_to_resource(np, 0, &res);
	if (rc) {
		PKTDMA_ERR("No resource in DTS"); 
		return -ENODEV;
	}
	p_dev.csr_base_p = res.start;

	/* Remap dma register region */
	p_dev.csr_base = ioremap_nocache(p_dev.csr_base_p, RES_SIZE(&res));
	p_dev.csr_base_glb = p_dev.csr_base + APM_IODMA_GLBL_DIAG_CSR_OFFSET;
	p_dev.csr_base_qmi = p_dev.csr_base + APM_IODMA_QMI_SLAVE_OFFSET;

	PKTDMA_DEBUG("CSR PAddr 0x%02X.%08X VAddr 0x%p size %lld",
		     HIDWORD(res.start), LODWORD(res.start),
		     p_dev.csr_base, RES_SIZE(&res));

	/* Retrieve channel available */
	ch_info = of_get_property(np, "ch-info", &len);
	if (!ch_info) {
		PKTDMA_ERR("No channel info in DTS");
		return -EINVAL;
	}
	p_dev.qinfo.tx_ch_cnt = len / 4;
	
	/* Retrieve TM info*/
	tm_info = of_get_property(np, "tm", &tm_info_len);
	tm_param_info = of_get_property(np, "tm-param", &tm_param_info_len);
	if (!tm_info || !tm_param_info) {
		p_dev.qinfo.no_tm = 1;	/* Assume non-TM configuration */
	} else if (tm_info_len != 4*p_dev.qinfo.tx_ch_cnt) {
		PKTDMA_ERR("TM DTS setting mismatch");
		p_dev.qinfo.no_tm = 1;	/* Assume non-TM configuration */
	} else if (tm_param_info_len != 
				APM_PKTDMA_NUM_COS*4*p_dev.qinfo.tx_ch_cnt) { 
		PKTDMA_ERR("TM parameter DTS setting mismatch");
		p_dev.qinfo.no_tm = 1;	/* Assume non-TM configuration */
	}
		
	/* Setup queue configuration from DTS */
	for (i = 0; i < p_dev.qinfo.tx_ch_cnt; i++) {
		struct pktdma_chan_qid *qcfg;
		int j;
		
	     	if (ch_info[i] >= PKTDMA_CHID_MAX) 
	     		continue;	     		
		p_dev.qinfo.chan[i] = ch_info[i];
		qcfg = &p_dev.qinfo.tx_q[ch_info[i]];
		qcfg->valid = 1;
		if (!p_dev.qinfo.no_tm) {
			/* Retrieve TM configuration */
			qcfg->tx_vqid_alg = tm_info[i];
			for (j = 0; j < APM_PKTDMA_NUM_COS; j++)
				qcfg->tx_qid_param[j] = 
					tm_param_info[i*APM_PKTDMA_NUM_COS+j];
		}
	}
	
	/* Determine number of processor */
	for (i = 0; i < NUM_CPUS; i++) {
#ifndef CONFIG_SMP
		if (i != apm_processor_id())
			continue;
#endif
		p_dev.qinfo.queues[i].valid = 1;
	}

	/* Enable IP */
	rc = apm_pktdma_enable_clk();
	if (rc) {
		PKTDMA_ERR("Clk enable failed");
		return rc;
	}
	if (!apm_pktdma_is_enable()) {
		apm_pktdma_enable_hw();
	}

	rc = apm_pktdma_read_reg(DMA_IPBRR_ADDR, &data);
	if (rc != 0)
		return rc;

	PKTDMA_ERR("PktDMA Identification: %02d.%02d.%02d",
		     REV_NO_RD(data), BUS_ID_RD(data), DEVICE_ID_RD(data));

	/* Configure Pkt DMA queue */
	if (apm_pktdma_qconfig())
		return -EINVAL;

	rc = apm_pktdma_fpool_init();
	if (rc) {
		PKTDMA_ERR(KERN_INFO "Free pool init failed:%d", rc);
		return -EINVAL; /* FIXME */
	}
			
	p_dev.qinfo.config_done = 1;
	
	INIT_LIST_HEAD(&p_dev.qinfo.op_cache);

	percpu_ptr = alloc_percpu(struct pktdma_percpu_data);
	if (percpu_ptr) {
		struct pktdma_percpu_data *percpu_data;
		p_dev.percpu_ptr = percpu_ptr;

		for_each_possible_cpu(cpu) {
			percpu_data = (struct pktdma_percpu_data *)
					per_cpu_ptr(percpu_ptr, cpu);
			spin_lock_init(&percpu_data->lock);
			INIT_LIST_HEAD(&percpu_data->head);
			tasklet_init(&percpu_data->rx_tasklet, apm_pktdma_bh_tasklet_cb,
				     (unsigned long) &p_dev);
		}
	} else {
		PKTDMA_ERR(KERN_INFO "per cpu ptr allocation failed");
	}
	/* Register QM callback function for PKTDMA */
	rc = apm_qm_msg_rx_register(APM_QM_DMA_RTYPE, apm_pktdma_msg_rx);
	if (rc) {
		PKTDMA_ERR("Unable to register for QM message error 0x%08X",
			   rc);
		return rc; /* FIXME */
	}
	
	/* Register Linx ASYNC DMA */	
	spin_lock_init(&p_dev.queue_lock);
	spin_lock_init(&p_dev.async_lock);
	spin_lock_init(&p_dev.cache_lock);

	dma_dev = &p_dev.common;
	INIT_LIST_HEAD(&dma_dev->channels);
	dma_cap_set(DMA_MEMCPY, dma_dev->cap_mask);

	/* Create async channels */
	dma_dev->chancnt = p_dev.qinfo.tx_ch_cnt;
	chan_id = p_dev.qinfo.chan[0];
	end_chan = p_dev.qinfo.chan[p_dev.qinfo.tx_ch_cnt-1];
	for ( ; chan_id <= end_chan; chan_id++) {
		if ((chan = kzalloc(sizeof(*chan), GFP_KERNEL)) == NULL) {
			dma_dev->chancnt--;
			return -ENOMEM;
		}
		list_add_tail(&chan->common.device_node, &dma_dev->channels);
		/* Map channel's device to dma device */
		chan->common.device = dma_dev;
	}
	/* Set base and prep routines */
	dma_dev->device_alloc_chan_resources = apm_pktdma_alloc_resources;
	dma_dev->device_free_chan_resources = apm_pktdma_free_resources;
	dma_dev->device_is_tx_complete = apm_pktdma_is_complete;
	dma_dev->device_issue_pending = apm_pktdma_issue_pending;
	dma_dev->device_prep_dma_memcpy = apm_pktdma_prep_memcpy;
	dma_dev->dev = &ofdev->dev;
	
	rc = dma_async_device_register(dma_dev);

	printk("PktDMA %sdriver regisetered%s",
		p_dev.qinfo.no_tm ? "" : "TM ", 
		p_dev.qinfo.no_xor ? " without XOR" : "");
	return rc;
}

static int __devexit apm_pktdma_of_remove(struct of_device *dev)
{
	struct iodma_pdev *pdev = platform_get_drvdata(dev);
	
	p_dev.qinfo.config_done = 0;
	dma_async_device_unregister(&pdev->common);
	
	return 0;
}

static struct of_device_id apm_pktdma_match[] = {
	{ .compatible	= "apm-pktdma", },
	{ .compatible	= "apm,apm86xxx-pktdma", },
	{ },
};

static struct of_platform_driver apm_pktdma_of_driver = {
	.name		= "apm-pktdma",
	.match_table	= apm_pktdma_match,
	.probe		= apm_pktdma_of_probe,
	.remove		= apm_pktdma_of_remove,
#if defined(CONFIG_PM)	
	.suspend	= apm_pktdma_suspend,
	.resume		= apm_pktdma_resume,
#endif
	.shutdown 	= apm_pktdma_shutdown,
};

static int __init apm_pktdma_init(void)
{
	of_register_platform_driver(&apm_pktdma_of_driver);
	return 0;

}

static void __exit apm_pktdma_exit(void)
{
	of_unregister_platform_driver(&apm_pktdma_of_driver);
	return;
}

EXPORT_SYMBOL(apm_pktdma_m2m_xfer);
EXPORT_SYMBOL(apm_pktdma_p2m_xfer);
EXPORT_SYMBOL(apm_pktdma_m2b_xfer);

subsys_initcall(apm_pktdma_init);
module_exit(apm_pktdma_exit);

MODULE_AUTHOR("Shasi Pulijala <spulijala@apm.com>");
MODULE_DESCRIPTION("APM86xxx PktDMA device driver");
MODULE_LICENSE("GPL");
