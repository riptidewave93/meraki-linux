/**
 *  Applied Micro Mamba DMA driver
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
 * This is the core runtime/config file for PktDMA core driver.
 *
 */
#include "apm_pktdma_access.h"

u16 apm_buf_len_result(u16 msg_len)
{
	u16 res_len = msg_len;
			
	if (msg_len & 0xFFF)
		return (msg_len & 0xFFF);

	if (msg_len == 0x7000)
		res_len =  256;
	else if (msg_len == 0x6000)
		res_len = 1024;
	else if (msg_len == 0x5000)
		res_len = 2048;
	else if (msg_len == 0x4000)
		res_len = 4096;
	else if (msg_len == 0x0000)
		res_len = 16384;

	return res_len;
}

void apm_pktdma_replenish_fp_buffers(struct fp_info *fp, int fp_num_refill)
{	/* FIXME - can we remove this function */
	if (fp && fp_num_refill) {
		PKTDMA_DRXTX("FP ID %d refill %d buffer",
				fp[0].fp_id, fp_num_refill);
		apm_pktdma_init_pool(fp[0].fp_id, FREE_POOL_DMA_BUFFER_SIZE, 
				fp_num_refill);
	}
}

static inline int apm_pktdma_is_coherent(void)
{
#if !defined(CONFIG_NOT_COHERENT_CACHE) || defined(CONFIG_APM86xxx_IOCOHERENT)
	return 1;
#endif
	return 0; /* FIXME */
}

void apm_pktdma_flush_src(void *saddr, int byte_cnt)
{
	PKTDMA_DRXTX("flushing src addr %p bytes :%d"
			, saddr, byte_cnt);
	flush_dcache_range((u32)saddr,
			    (u32)(saddr + byte_cnt));
}
EXPORT_SYMBOL(apm_pktdma_flush_src);

void apm_pktdma_flush_dest(void *daddr, int byte_cnt)
{
	PKTDMA_DRXTX("byte cnt used to flush dst = %d",
		     byte_cnt);
	flush_dcache_range((u32)daddr,
			    (u32)(daddr + byte_cnt));
}
EXPORT_SYMBOL(apm_pktdma_flush_dest);

void apm_pktdma_invalidate_dest(void *daddr, int byte_cnt)
{
	PKTDMA_DRXTX("byte cnt used to invalidate dst = %d",
		     byte_cnt);
	invalidate_dcache_range((u32)daddr,
			    (u32)(daddr + byte_cnt));
}
EXPORT_SYMBOL(apm_pktdma_invalidate_dest);

#if defined (CONFIG_APM867xx)
void apm_pktdma_err_log(int err, int flby_err)
{
	if (flby_err) {
		/* Checksum/CRC Errors*/
		switch (err) {
		case CRC_ERR:
			PKTDMA_ERR("CRC Error");
			break;
		case CHK_ERR:
			PKTDMA_ERR("Checksum Error");
			break;
		default:
			PKTDMA_ERR("No error set");
			break;
		}
		return;
	}
	switch (err) {
		case ERR_MSG_AXI:
			PKTDMA_ERR("AXI Error when reading the Src or "
					"Dst Add from the Link List");
			break;
		case ERR_BAD_MSG:
			PKTDMA_ERR("HE0 bit is not set the incoming Message");
			break;
		case ERR_READ_DATA_AXI:
			PKTDMA_ERR("AXI Error when reading data");
			break;
		case ERR_WRITE_DATA_AXI:
			PKTDMA_ERR("AXI Error when writing data");
			break;
		case ERR_FBP_TIMEOUT:
			PKTDMA_ERR("Timeout on Free Pool Buffer fetch");
			break;
		case ERR_SCT_GAT_LEN:
			PKTDMA_ERR("Gather and Scatter dont have the same Data Length");
			break;
		default:
			PKTDMA_ERR("No error set");
			break;
	}
}
#else
void apm_pktdma_err_log(int err)
{
	switch (err) {
	case ERR_MSG_AXI:
		PKTDMA_ERR("AXI Error when reading the Src or "
			"Dst Add from the Link List");
		break;
	case ERR_BAD_MSG:
		PKTDMA_ERR("HE0 bit is not set the incoming Message");
		break;
	case ERR_READ_DATA_AXI:
		PKTDMA_ERR("AXI Error when reading data");
		break;
	case ERR_WRITE_DATA_AXI:
		PKTDMA_ERR("AXI Error when writing data");
		break;
	case CRC_ERR:
		PKTDMA_ERR("CRC Error");
		break;
	case CHK_ERR:
		PKTDMA_ERR("Checksum Error");
		break;
	default:
		PKTDMA_ERR("No error set");
		break;
	}
}
#endif
int apm_pktdma_get_num_buffs(struct apm_pktdma_msg_done *comp_msg)
{
	struct apm_dma_msg1_2 *dmsg1_2 = &comp_msg->dmsg1_2;
	union list_msg_data *list_msg;
	int num_buffs;
	int i;
	
	if (!dmsg1_2->NV)
		return 1;
	
	list_msg = &comp_msg->done_list_msg_data;
	if (dmsg1_2->LL) {
		if (!list_msg->desc.src_gr_5.linksize) {
			PKTDMA_ERR("Invalid: Linksize is zero while LL bit is %d!!",
			   dmsg1_2->LL);
			return 1;
		}
		return 4 + list_msg->desc.src_gr_5.linksize;
	}
	 
	num_buffs = 1;
	i = 0;
		
	if (!list_msg->src_list[i].next_data_len) {
		PKTDMA_ERR("Invalid: Next data len is zero while NV is %d!!",
			   dmsg1_2->NV);
		return 1;
	}
	while (list_msg->src_list[i].next_data_len != 0x7800) { /* FIXME */	
		i++;
		if (++num_buffs == 5)
			break;
	}
	return num_buffs;
}

void apm_pktdma_fpbuff_get_single(struct apm_pktdma_msg_done *comp_msg,
				 struct fp_info *fp)
{
	struct apm_dma_msg1_2 *dmsg1_2 = &comp_msg->dmsg1_2;
	
	fp->fp_id = dmsg1_2->FPQNum;
	fp->fp_dest_address = phys_to_virt(MAKE64(dmsg1_2->addr_hi,
						dmsg1_2->addr_lo));
	fp->fp_dlength = apm_buf_len_result(dmsg1_2->data_len);
}

struct sg_list_attr *apm_pktdma_fpbuff_get_mul(
				struct apm_pktdma_msg_done *comp_msg,
				struct fp_info *fp, u8 num_buffs)
{
	struct apm_dma_msg1_2 *dmsg1_2 = &comp_msg->dmsg1_2;
	struct sg_list_attr *buf_ptr = NULL;
	union list_msg_data *list_msg = &comp_msg->done_list_msg_data;
	int i, j;
	
	if (!dmsg1_2->LL) {
		for (i = 0, j = 1; i < (num_buffs - 1); i++, j++) {
			fp[j].fp_dest_address = phys_to_virt(
				MAKE64(list_msg->src_list[i].next_addr_hi,
					list_msg->src_list[i].next_addr_lo));
			fp[j].fp_id = list_msg->src_list[i].next_fpq_id;
			fp[j].fp_dlength = apm_buf_len_result(
					list_msg->src_list[i].next_data_len);
		}
	} else {
		buf_ptr = (struct sg_list_attr *)
				phys_to_virt(MAKE64(list_msg->desc.src_gr_5.ptr_to_src_hi,
					list_msg->desc.src_gr_5.ptr_to_src_lo));
		PKTDMA_DRXTX("linked list ptr 0x%08X", (u32) buf_ptr);
		for (i = 0, j = 1; i < 3; i++, j++) {
			fp[j].fp_dest_address = phys_to_virt(
				MAKE64(list_msg->desc.list_ll[i].next_addr_hi,
				       list_msg->desc.list_ll[i].next_addr_lo));
			fp[j].fp_id = list_msg->desc.list_ll[i].next_fpq_id;
			fp[j].fp_dlength = apm_buf_len_result(
				list_msg->desc.list_ll[i].next_data_len);
		}
		for ( i = 0, j = 4; i < (num_buffs - 4); i++, j++) {
			fp[j].fp_dest_address = phys_to_virt(
				MAKE64(buf_ptr[i].next_addr_hi,
					buf_ptr[i].next_addr_lo));
			fp[j].fp_id = buf_ptr[i].next_fpq_id;
			fp[j].fp_dlength = apm_buf_len_result(
				buf_ptr[i].next_data_len);
		}
		
	}
	return buf_ptr;
}

/**
 * Operation Completion Callback Handler
 *
 */
int apm_pktdma_op_cb_all(struct iodma_op_state *op_st)
{
	struct apm_dma_msg1_2 *dmsg1_2 = &op_st->dmsg.dmsg1_2;
	struct apm_pktdma_op_result *comp_resp = &op_st->result;
	struct sg_list_attr *buf_ptr = NULL;
	int ret = 0;
	int num_buffs;
	int fp_num_refill = 0;
#ifdef APM_PKTDMA_DEBUG
	int i;
#endif
	comp_resp->fp = NULL;
	comp_resp->err = dmsg1_2->LErr;
	if (dmsg1_2->LErr) {
		apm_pktdma_err_log(dmsg1_2->LErr);
#if defined (CONFIG_APM867xx)
		if (!dmsg1_2->EL_Err)
			goto cb_fin;	/* Critical error */
#else
		if (dmsg1_2->LErr < 5)
			goto cb_fin;	/* Critical error */
#endif
	}

	if (op_st->cb_compl_msg) {
		comp_resp->comp_msg = &op_st->dmsg;
	} else {
		num_buffs = apm_pktdma_get_num_buffs(&op_st->dmsg);
 		PKTDMA_DRXTX("number of buffers return = %d", num_buffs);
		comp_resp->fp = kzalloc(num_buffs * sizeof(struct fp_info),
					GFP_KERNEL);
		if (!comp_resp->fp) {
			PKTDMA_ERR("out of memory for Dest buff table!!");
			return -ENOMEM;
		}
		apm_pktdma_fpbuff_get_single(&op_st->dmsg, comp_resp->fp);
		if (dmsg1_2->NV && (num_buffs > 1)) {
			buf_ptr = apm_pktdma_fpbuff_get_mul(&op_st->dmsg,
					comp_resp->fp, num_buffs);
		}
#ifdef APM_PKTDMA_DEBUG
		for (i = 0; i < num_buffs; i++) {
			PKTDMA_DRXTX("returned data buf: 0x%p, date len: %d",
				comp_resp->fp[i].fp_dest_address,
				comp_resp->fp[i].fp_dlength);
		}
#endif
		fp_num_refill = dmsg1_2->LL ? (num_buffs + 1) : num_buffs;
		comp_resp->num_fp = num_buffs;
		if (op_st->fby_gen) {
			comp_resp->crc = op_st->dmsg.dmsg3.crc_result;
			comp_resp->checksum = op_st->dmsg.dmsg4.checksum_res;
		}			
	}

		
cb_fin:
	if (op_st->result.cb)
		op_st->result.cb(comp_resp);
		
	if (op_st->src_ptr) {
		PKTDMA_DEBUG("freeing gather_ptr = %p", op_st->src_ptr);
		kfree(op_st->src_ptr);
	}

	/* Replenish the free pool buffers */
	if (fp_num_refill) {
		if (buf_ptr)
			kfree(buf_ptr);
		apm_pktdma_replenish_fp_buffers(comp_resp->fp, fp_num_refill);
		kfree(comp_resp->fp);
	}

	apm_pktdma_op_free(op_st);
	
	return ret;
}

int apm_pktdma_op_cb_dest_mem(struct iodma_op_state *op_st)
{
	struct apm_dma_msg1_2 *dmsg1_2 = &op_st->dmsg.dmsg1_2;
	struct apm_pktdma_op_result *comp_resp = &op_st->result;

	if (op_st->src_ptr) {
		PKTDMA_DRXTX("freeing gather_ptr = %p", op_st->src_ptr);
		kfree(op_st->src_ptr);
	}

	if (op_st->dest_ptr) {
		PKTDMA_DRXTX("freeing scatter ptr = %p", op_st->dest_ptr);
		kfree(op_st->dest_ptr);
	}

	if ((comp_resp->err = dmsg1_2->LErr)) 
		apm_pktdma_err_log(dmsg1_2->LErr);
	
	if (op_st->result.cb)
		op_st->result.cb(comp_resp);
		
	apm_pktdma_op_free(op_st);
	
	return 0;
}

int apm_pktdma_op_cb(struct iodma_op_state *op_st)
{
	switch (op_st->major_opcode) {
	case MEM_TO_MEM:
	case PKT_TO_MEM:
		return apm_pktdma_op_cb_dest_mem(op_st);
	default:
		return apm_pktdma_op_cb_all(op_st);
	}
}

int apm_pktdma_crc_set(struct apm_pktdma_flyby_info *fb,
		       struct apm_pktdma_msg *dma_msg)
{
	struct apm_dma_msg_4 *msg4 = &(dma_msg->msg4);
	struct apm_dma_msg_3 *msg3 = &(dma_msg->msg3);
	
	msg4->FBY = fb->fb_type;
	msg4->GN = fb->fb_gen_check;
	msg4->SD = fb->fb_seed_chksum;
	msg3->var_data.to_buf_fby.crc_chk_byte_cnt = fb->fb_byte_count;
	if (msg4->SD)
		msg4->var_data.fby.crc_chk_seed = fb->fb_seed_chksum;

	return 0;
}

u16 apm_buf_len_set(u16 len)
{
	u16 data_length;

	if (len > 16384) {
		PKTDMA_ERR("Input Length more than the max limit");
		return 0;
	}

	if (len < 256) {
		data_length = len | 0x7000;
	} else if (len == 256) {
		data_length = 0x7000;
	} else if (len < 1024) {
		data_length = len | 0x6000;
	} else if (len == 1024) {
		data_length = 0x6000;
	} else if (len < 2048) {
		data_length = len | 0x5000;
	} else if (len == 2048) {
		data_length = 0x5000;
	} else if (len < 4096) {
		data_length = len | 0x4000;
	} else if (len == 4096) {
		data_length = 0x4000;
	} else if (len < 16384) {
		data_length = len;
	} else if (len == 16384) {
		data_length = 0x0;
	} else {
		data_length = 0;
	}	
	
	return data_length;
}

u16 apm_pktdma_len_set(u16 len)
{
	if (len < 32 || len > (16 * 1024)) {
		PKTDMA_ERR("Illegal Length:%d for a Single"
				"buffer transfer", len);
		BUG();
	}
	if (len < 16*1024) {
		return len;
	} else if (len == 16*1024) {
		return 0;
	}

	return 0;
}

void apm_pktdma_qmesg_load_single_dst(struct apm_pktdma_msg *dma_msg,
				      u64 da)
{
	dma_msg->msg3.var_data.dst_m2m.dest_addr_hi = (u8) (da >> 32);
	dma_msg->msg3.var_data.dst_m2m.dest_addr_lo = (u32) da;
}

static int apm_pktdma_qmesg_load_dst_mul(struct apm_pktdma_msg *dma_msg,
			 u64 *da, u16 *len,u8 sg_count, 
			 struct iodma_op_state *op, u16 *total_len)
{
	u16 byte_count = 0;
	struct sg_list_attr *dst_ptr;
	int i;
	u64 phys_dst_ptr;

	dst_ptr = kzalloc((sg_count * sizeof(struct sg_list_attr)), GFP_KERNEL);
	if (dst_ptr == NULL)
		return -ENOMEM;
	op->dest_ptr = dst_ptr;
	dst_ptr = (void *)PKTDMA_ALIGN_PTR((u32)dst_ptr);
	PKTDMA_DEBUG("dest ptr:0x%p,  dest ptr aligned:0x%p",
		     op->dest_ptr, dst_ptr);
	for (i = 0; i < sg_count; i++) {
		dst_ptr[i].next_addr_hi = (u8) (da[i] >> 32);
		dst_ptr[i].next_addr_lo = (u32) da[i];
		dst_ptr[i].next_data_len = apm_pktdma_len_set(len[i]);
		byte_count += len[i];
	}
#ifdef CONFIG_NOT_COHERENT_CACHE
	flush_dcache_range((u32)dst_ptr, (u32)(dst_ptr + (sg_count *
			sizeof(struct sg_list_attr))));
#endif
	phys_dst_ptr = __pa((u32)dst_ptr);
	dma_msg->msg3.var_data.scatter_m2m.dst_ptr = (u32)(phys_dst_ptr >> 4);
	dma_msg->msg3.var_data.scatter_m2m.linksize = sg_count;

	PKTDMA_DEBUG_DUMP("linked list for Dest", dst_ptr, sg_count * 8);
	*total_len = byte_count;
	return 0;

}

void apm_pktdma_qmesg_load_single_src(struct apm_pktdma_msg *dma_msg,
				      u64 sa, u16 len, u8 fp_id)
{
	dma_msg->msg1_2.addr_hi = (u8) (sa >> 32);
	dma_msg->msg1_2.addr_lo = (u32) sa;
	if (fp_id) {
		dma_msg->msg1_2.FPQNum = fp_id;
		dma_msg->msg1_2.data_len = len;
	} else {
		dma_msg->msg1_2.data_len = apm_pktdma_len_set(len);
	}
}

static int apm_pktdma_qmesg_load_src_mul(struct apm_pktdma_msg *dma_msg,
					 u64 *sa, u16 *len, u8 *fp_id,
					 u8 sg_count, struct iodma_op_state *op)
{
	int i, j, count;
	u32 total_linklist_bytecount = 0;
	struct sg_list_attr *gather_ptr;

	if (fp_id)
		apm_pktdma_qmesg_load_single_src(dma_msg, sa[0], len[0],
					 fp_id[0]);
	else
		apm_pktdma_qmesg_load_single_src(dma_msg, sa[0], len[0], 0);

	dma_msg->msg1_2.NV = 1;
	if (sg_count <= 5 ) {
		for (i = 1, j = 0; i < sg_count; i++, j++) {
			if (fp_id) {
				dma_msg->list_msg_data.src_list[j].next_fpq_id =
						fp_id[i];
				dma_msg->list_msg_data.src_list[j].
						next_data_len = len[i];
			} else
				dma_msg->list_msg_data.src_list[j].
						next_data_len =
						apm_pktdma_len_set(len[i]);
			dma_msg->list_msg_data.src_list[j].next_addr_hi =
					(u8)(sa[i] >> 32);
			dma_msg->list_msg_data.src_list[j].next_addr_lo =
					(u32) sa[i];
			PKTDMA_DRXTX("Src address 0x%08X, src len: %d, "
					"for %d",
			(u32) dma_msg->list_msg_data.src_list[j].next_addr_lo,
			len[i], i);
		}
		if (sg_count < 5)
			/* Marking the entry as invalid */
			dma_msg->list_msg_data.src_list[j].next_data_len = 0x7800;
	} else {
		dma_msg->msg1_2.LL = 1;
		for (i = 1, j = 0; i < 4; i++, j++) {
			if (fp_id) {
				dma_msg->list_msg_data.desc.list_ll[j].next_fpq_id =
						fp_id[i];
				dma_msg->list_msg_data.desc.list_ll[j].
						next_data_len = len[i];
			} else {
				dma_msg->list_msg_data.desc.list_ll[j].
						next_data_len =
						apm_pktdma_len_set(len[i]);
			}
			dma_msg->list_msg_data.desc.list_ll[j].next_addr_hi =
					(u8) (sa[i] >> 32);
			dma_msg->list_msg_data.desc.list_ll[j].next_addr_lo =
					(u32) sa[i];
		}
		count = sg_count - 4;
		if (count > 256) {
			PKTDMA_ERR("Exceeding the link size");
			count = 0;
			return -EINVAL;
		}
		gather_ptr = kzalloc((count * sizeof(struct sg_list_attr)), GFP_KERNEL);
		if (gather_ptr == NULL) {
			PKTDMA_ERR("Buffer descriptor allocation failed");
			return -ENOMEM;
		}
		op->src_ptr = gather_ptr;
		for (j = 0, i = 4; i < sg_count; i++, j++) {
			gather_ptr[j].next_addr_hi =
					(u8) (sa[i] >> 32);
			gather_ptr[j].next_addr_lo =
					(u32) sa[i];
			if (fp_id) {
				gather_ptr[j].next_fpq_id = fp_id[i];
				gather_ptr[j].next_data_len =
						len[i];
				total_linklist_bytecount
						+= apm_buf_len_result(len[i]);
			} else {
				gather_ptr[j].next_data_len =
						apm_pktdma_len_set(len[i]);
				total_linklist_bytecount
						+= len[i];
			}
		}
#ifdef CONFIG_NOT_COHERENT_CACHE
		flush_dcache_range((u32)gather_ptr, (u32)(gather_ptr + count *
			sizeof(struct sg_list_attr)));
#endif
		dma_msg->list_msg_data.desc.src_gr_5.linksize = count;
		dma_msg->list_msg_data.desc.src_gr_5.total_length_link =
				total_linklist_bytecount;
		dma_msg->list_msg_data.desc.src_gr_5.ptr_to_src_lo =
				(u32)__pa((u32)gather_ptr);
	}
	return 0;
}

int apm_pktdma_p2m_xfer(struct apm_pktdma_p2m_params *iodma_p2m)
{
	/* Do dealloc for m2b after getting completion message */
	struct apm_pktdma_msg dma_msg;
	struct apm_qm_msg_desc send_msg;
	struct apm_dma_msg1_2 *msg1_2 = &dma_msg.msg1_2;
	struct apm_dma_msg_3 *msg3 = &dma_msg.msg3;
	struct apm_dma_msg_4 *msg4 = &dma_msg.msg4;
	u16 total_len = 0;
	struct iodma_op_state *op;

	if (p_dev.qinfo.config_done == 0) {
		PKTDMA_DEBUG("%s %d: In call to '%s', error: ",
			    __FILE__, __LINE__, __FUNCTION__);
		PKTDMA_DEBUG("apm_iodma_config_set has not been called.");
		return -1;
	}
	memset(&dma_msg, 0, 64);	/* FIXME - please avoid */

	/* Get a new op*/
	op = apm_pktdma_op_get();
	if (!op)
		return -ENOMEM;
	op->src_ptr = NULL;
	op->dest_ptr = NULL;
	op->result.cb = iodma_p2m->cb;
	op->result.ctx = iodma_p2m->context;
	op->major_opcode = PKT_TO_MEM;
	
	/*set common params im2m_moden msg */
	msg1_2->coherent_read = apm_pktdma_is_coherent(); /* set the Coherent bit */
	msg1_2->RType = APM_QM_DMA_RTYPE;
	msg1_2->uinfo = (u32)(unsigned long) op;
	msg3->HE = 1;
	/*Set to Host QID for Completion messages */
	msg3->H0Enq_Num = p_dev.qinfo.queues[apm_processor_id()].comp_qid;	
	msg3->HR = 1;
	msg4->BD = 0;
	switch (iodma_p2m->p2m_mode) {
	case IODMA_COPY:
		op->minor_opcode = DMA_DIRECT;
		msg3->DR = 1; /* Set if H0info is Dst Pointer - S */
		apm_pktdma_qmesg_load_single_src(&dma_msg,
				*iodma_p2m->fp_sa, *iodma_p2m->byte_count,
				*iodma_p2m->fp_id);
		apm_pktdma_qmesg_load_single_dst(&dma_msg,
				*iodma_p2m->da);
		PKTDMA_DEBUG("printing sa in p2m 0x%08X",
			     (u32)*iodma_p2m->fp_sa);
		PKTDMA_DEBUG("printing da in p2m 0x%08X",
			     (u32)*iodma_p2m->da);
		break;
	case IODMA_SCATTER:
		op->minor_opcode = DMA_SCATTER;
		msg4->SC = 1;
		msg3->DR = 0; /* Set if H0info is Dst Pointer - S */
		apm_pktdma_qmesg_load_dst_mul(&dma_msg,
			iodma_p2m->da, iodma_p2m->byte_count,
				iodma_p2m->sg_count, op, &total_len);
		apm_pktdma_qmesg_load_single_src(&dma_msg,
				*iodma_p2m->fp_sa, total_len,
				*iodma_p2m->fp_id);
		break;
	case IODMA_GATHER:
		op->minor_opcode = DMA_GATHER;
		msg3->DR = 1; /* Set if H0info is Dst Pointer - S */
		apm_pktdma_qmesg_load_src_mul(&dma_msg, iodma_p2m->fp_sa,
					       iodma_p2m->byte_count,
					       iodma_p2m->fp_id,
					       iodma_p2m->sg_count, op);
		apm_pktdma_qmesg_load_single_dst(&dma_msg,
				*iodma_p2m->da);
		break;
		default:
			PKTDMA_ERR("Unsupported Mode in P2M");
		break;
	}

#if defined(APM_PKTDMA_DEBUG)
	if (iodma_p2m->cos < IODMA_MIN_COS || iodma_p2m->cos > IODMA_MAX_COS)
		PKTDMA_DEBUG("COS out of range %d", iodma_p2m->cos);
#endif
	send_msg.qid = p_dev.qinfo.tx_q[iodma_p2m->chid].tx_qid[iodma_p2m->cos];
	send_msg.msg = &dma_msg;
	if (msg1_2->NV) {
		PKTDMA_DEBUG_DUMP("P2M 64 Byte Message", &dma_msg, 64);
	} else {
		PKTDMA_DEBUG_DUMP("P2M 32 Byte Message", &dma_msg, 32);
	}

	PKTDMA_DEBUG("Buffer to Memory Comp q_id %d enq id %d data len %d",
		    msg3->H0Enq_Num, send_msg.qid, msg1_2->data_len);
	apm_qm_push_msg(&send_msg);			
	return 0;
}

int apm_pktdma_m2b_xfer(struct apm_pktdma_m2b_params *iodma_m2b)
{
	/* Do dealloc for m2b after getting completion message */
	struct apm_pktdma_msg dma_msg;
	struct apm_qm_msg_desc send_msg;
	struct apm_dma_msg1_2 *msg1_2 = &dma_msg.msg1_2;
	struct apm_dma_msg_3 *msg3 = &dma_msg.msg3;
	struct apm_dma_msg_4 *msg4 = &dma_msg.msg4;
	struct apm_pktdma_flyby_info fb_info;
	struct iodma_op_state *op;
#if 0
	int i;
#endif

#ifdef APM_PKTDMA_DEBUG
	if (p_dev.qinfo.config_done == 0) {
		PKTDMA_DEBUG("%s %d: In call to '%s', error: ",
			    __FILE__, __LINE__, __FUNCTION__);
		PKTDMA_DEBUG("apm_iodma_config_set has not been called.");
		return -1;
	}
#endif

	/* Check if XOR hardware available */
	if (p_dev.qinfo.no_xor && 
	    iodma_m2b->fb.fb_type >= DMA_FBY_XOR_2SRC &&
	    iodma_m2b->fb.fb_type <= DMA_FBY_XOR_5SRC) {
	    	/* No such feature */
		return -ENXIO;
	}
	
	memset(&dma_msg, 0, iodma_m2b->m2b_mode == IODMA_GATHER ? 64 : 32);

	/* Get a new op */
	op = apm_pktdma_op_get();
	if (!op)
		return -ENOMEM;
	op->src_ptr = NULL;
	op->dest_ptr = NULL;
	op->result.cb = iodma_m2b->cb;
	op->result.ctx = iodma_m2b->context;
	op->result.crc = op->result.checksum = 0;
	op->major_opcode = MEM_TO_BUFF;
	memset(&op->dmsg, 0, sizeof(struct apm_pktdma_msg_done));
	op->cb_compl_msg = iodma_m2b->cb_compl_msg ? 1 : 0;
	op->fby_gen = 0;
	/*set common params im2m_moden msg */
	msg1_2->coherent_read = apm_pktdma_is_coherent(); /* set the Coherent bit */
	msg1_2->RType = APM_QM_DMA_RTYPE;
	msg1_2->uinfo = (u32)(unsigned long) op;
	msg3->HE = 1;
	msg3->H0Enq_Num = p_dev.qinfo.queues[apm_processor_id()].comp_qid;	
	/*Set to Host QID for Completion messages */
	msg3->DR = 0; /* Set if H0info is Dst Pointer - S */
	msg4->BD = 1;

	if (op->cb_compl_msg) /* FIXME */
		msg3->H0FPSel = iodma_m2b->cross_pbn;
	else
		msg3->H0FPSel = p_dev.qinfo.queues[apm_processor_id()].rx_fp_pbn;
	
	/*send direct mode msg */
	switch (iodma_m2b->m2b_mode) {
	case IODMA_COPY:
		PKTDMA_DRXTX("Executing IODMA m2b direct mode operation.");
		op->minor_opcode = DMA_DIRECT;
		apm_pktdma_qmesg_load_single_src(&dma_msg, *iodma_m2b->sa,
				*iodma_m2b->byte_count, 0);
		break;
	case IODMA_GATHER:
		op->minor_opcode = DMA_GATHER;
		apm_pktdma_qmesg_load_src_mul(&dma_msg, iodma_m2b->sa,
					       iodma_m2b->byte_count,
					       NULL, iodma_m2b->sg_count,
					       op);
		break;
	default:
		PKTDMA_ERR("Unsupported Mode in M2B");
		break;
	}

	if (iodma_m2b->fby) {
		PKTDMA_DEBUG("Setting CRC and Checksum params");
		memcpy(&fb_info, &(iodma_m2b->fb),
			sizeof(struct apm_pktdma_flyby_info));
		apm_pktdma_crc_set(&fb_info, &dma_msg);
		op->fby_gen = 1;
	}

	PKTDMA_DRXTX("The free pool id mapped = %d", msg3->H0FPSel);
	PKTDMA_DEBUG_DUMP("M2B Send Message", &dma_msg, msg1_2->NV ? 64 : 32);

#if defined(APM_PKTDMA_DEBUG)
	if (iodma_m2b->cos < IODMA_MIN_COS || iodma_m2b->cos > IODMA_MAX_COS)
		PKTDMA_DEBUG("COS out of range %d", iodma_m2b->cos);
#endif
	send_msg.qid = p_dev.qinfo.tx_q[iodma_m2b->chid].tx_qid[iodma_m2b->cos];
	send_msg.msg = &dma_msg;
	
	PKTDMA_DRXTX("Memory to Buf Comp q_id: %d, enq id: %d, data len: %d",
	       msg3->H0Enq_Num, send_msg.qid, msg1_2->data_len);
	apm_qm_push_msg(&send_msg);
	
	return 0;
}

static int apm_pktdma_stride_not_aligned(u64 sa, u64 da,
					 struct stride_info *str, u16 byte_count,
					 u64 *new_sa, u16 *new_byte_cnt)
{
	u32 *saddr, *daddr;
	u16 str_frst_copy, str_size;
	int num_copies = 0;
	int len = byte_count;
	u32 *str_sa;

	str_frst_copy = (str->src_str_1_sz <= str->dst_str_1_sz)
			 ? str->src_str_1_sz : str->dst_str_1_sz;
	str_size = (str->src_str_sz <= str->dst_str_sz) ?
			str->src_str_sz : str->dst_str_sz;
	len -= str_frst_copy;
	num_copies = len / (str_size + str->dst_str_dis);
	num_copies--;
	if (((num_copies * str->dst_str_sz) + str_frst_copy) % 16 != 0) {
		printk("Stride data not aligned\n");
		saddr = (u32 *)phys_to_virt(sa);
		daddr = (u32 *)phys_to_virt(da);

		str_sa = kzalloc((byte_count +
				(str->src_str_sz + str->src_str_dis)), GFP_KERNEL);
		if (!str_sa)
			return -ENOMEM;
#if 0
		str_da = kzalloc((byte_count +
				(str->dst_str_sz + str->dst_str_dis)), GFP_KERNEL);
		if (!str_da)
			return -ENOMEM;
#endif
		memcpy(str_sa, saddr, byte_count);
		*new_sa = __pa(str_sa);
		*new_byte_cnt = byte_count + (str->src_str_sz + str->src_str_dis);
		return 1;
	} else
		return 0;

		return 0;
}

int apm_pktdma_m2m_xfer(struct apm_pktdma_m2m_params *iodma_m2m)
{
	struct apm_pktdma_msg dma_msg;
	struct apm_qm_msg_desc send_msg;
	struct apm_dma_msg1_2 *msg1_2 = &dma_msg.msg1_2;
	struct apm_dma_msg_3 *msg3 = &dma_msg.msg3;
	struct apm_dma_msg_4 *msg4 = &dma_msg.msg4;
	struct iodma_op_state *op;
	u16 total_len;
	u64 new_sa = 0;
	u16 new_byte_cnt = 0;

#if defined(APM_PKTDMA_DEBUG)
	if (p_dev.qinfo.config_done == 0) {
		PKTDMA_DEBUG("PktDMA un-initialized");
		return -EINVAL;
	}
#endif
	memset(&dma_msg, 0, 64);	/* FIXME - please avoid */

	/* Get a new op */
	op = apm_pktdma_op_get();
	if (!op) {
		PKTDMA_ERR("Out of memory for PktDMA operation");
		return -ENOMEM;
	}
	op->src_ptr = NULL;
	op->dest_ptr = NULL;
	op->result.cb = iodma_m2m->cb;
	op->result.ctx = iodma_m2m->context;
	op->major_opcode = MEM_TO_MEM;
	
	/* Set common params m2m_moden msg */
	msg1_2->coherent_read = apm_pktdma_is_coherent(); /* Coherent bit? */
	msg1_2->RType = APM_QM_DMA_RTYPE;
	msg1_2->uinfo = (u32) (unsigned long) op;
	msg3->HE = 1;
	/* Set to Host QID for Completion messages */
	if (apm_processor_id())
		p_dev.smp_core1++;
	else
		p_dev.smp_core0++;
	msg3->H0Enq_Num = p_dev.qinfo.queues[apm_processor_id()].comp_qid;
	/* send direct mode msg */
	switch (iodma_m2m->m2m_mode) {
	case IODMA_COPY:
		apm_pktdma_qmesg_load_single_src(&dma_msg, *iodma_m2m->sa,
				*iodma_m2m->byte_count, 0);
		apm_pktdma_qmesg_load_single_dst(&dma_msg, *iodma_m2m->da);
		op->minor_opcode = DMA_DIRECT;
		msg3->DR = 1; /* Set if H0info is Dst Pointer - S */
		PKTDMA_DRXTX("PADDR SRC: 0x%010llx PADDR DEST: 0x%010llx",
				*iodma_m2m->sa, *iodma_m2m->da);
		break;
	case IODMA_GATHER:
		op->minor_opcode = DMA_GATHER;
		msg3->DR = 1; /* Set if H0info is Dst Pointer - S */
		apm_pktdma_qmesg_load_src_mul(&dma_msg, iodma_m2m->sa,
					       iodma_m2m->byte_count,
					       NULL, iodma_m2m->sg_count,
					       op);
		PKTDMA_DRXTX("PADDR DEST: 0x%010llx", *iodma_m2m->da);
		apm_pktdma_qmesg_load_single_dst(&dma_msg, *iodma_m2m->da);
		break;
	case IODMA_SCATTER:
		op->minor_opcode = DMA_SCATTER;
		msg3->DR = 0; /* Set if H0info is Dst Pointer - S */
		msg4->SC = 1;
		apm_pktdma_qmesg_load_dst_mul(&dma_msg, iodma_m2m->da,
				iodma_m2m->byte_count,
				iodma_m2m->sg_count, op, &total_len);
		apm_pktdma_qmesg_load_single_src(&dma_msg, *iodma_m2m->sa,
				total_len, 0);
		break;
	case IODMA_GATHER_SCATTER:
		op->minor_opcode = DMA_SCAT_GATH;
		msg3->DR = 0;
		msg4->SC = 1;
		apm_pktdma_qmesg_load_src_mul(&dma_msg, iodma_m2m->sa,
					       iodma_m2m->byte_count,
					       NULL, iodma_m2m->sg_count,
					       op);
		apm_pktdma_qmesg_load_dst_mul(&dma_msg, iodma_m2m->da,
				iodma_m2m->byte_count,
				iodma_m2m->sg_count, op, &total_len);
		break;
	case IODMA_STRIDE:
		op->minor_opcode = DMA_STRIDE;
#if defined (CONFIG_APM862xx)
		/* Striding Bug Workaround for Green Mamba */
		if (apm_pktdma_stride_not_aligned(*iodma_m2m->sa, *iodma_m2m->sa,
		    &iodma_m2m->str, *iodma_m2m->byte_count, &new_sa, &new_byte_cnt)) {
			apm_pktdma_qmesg_load_single_src(&dma_msg, new_sa,
				new_byte_cnt, 0);
		}
		else  {
			apm_pktdma_qmesg_load_single_src(&dma_msg, *iodma_m2m->sa,
					*iodma_m2m->byte_count, 0);
		}
#else
		apm_pktdma_qmesg_load_single_src(&dma_msg, *iodma_m2m->sa,
		*iodma_m2m->byte_count, 0);

#endif
		msg3->DR = 1; /* Set if H0info is Dst Pointer - S */
		msg4->ST = 1;
		apm_pktdma_qmesg_load_single_dst(&dma_msg, *iodma_m2m->da);
		msg4->var_data.str_op.dst_str_dis =
				iodma_m2m->str.dst_str_dis;
		msg4->var_data.str_op.dst_str_sz =
				iodma_m2m->str.dst_str_sz;
		msg4->var_data.str_op.dst_str_1_sz =
				iodma_m2m->str.dst_str_1_sz;
		msg4->var_data.str_op.src_str_dis =
				iodma_m2m->str.src_str_dis;
		msg4->var_data.str_op.src_str_sz =
				iodma_m2m->str.src_str_sz;
		msg4->var_data.str_op.src_str_1_sz =
				iodma_m2m->str.src_str_1_sz;
		PKTDMA_DRXTX("PADDR SRC: 0x%010llx PADDR DEST: 0x%010llx",
			*iodma_m2m->sa, *iodma_m2m->da);
		break;
	default:
		PKTDMA_ERR("Unsupported Command");
		goto err;
	}
#ifdef APM_PKTDMA_DRXTX
	PKTDMA_DEBUG_DUMP("M2M Byte Message ", &dma_msg, msg1_2->NV ? 64 : 32);
#endif

#if defined(APM_PKTDMA_DEBUG)
	if (iodma_m2m->cos < IODMA_MIN_COS || iodma_m2m->cos > IODMA_MAX_COS)
		PKTDMA_DEBUG("COS out of range %d", iodma_m2m->cos);
#endif
	send_msg.qid = p_dev.qinfo.tx_q[iodma_m2m->chid].tx_qid[iodma_m2m->cos];	
	send_msg.msg = &dma_msg;
	
	PKTDMA_DRXTX("CQID %d EQID %d len %d", 
		msg3->H0Enq_Num, send_msg.qid, msg1_2->data_len);
	apm_qm_push_msg(&send_msg);
	return 0;
	
err:
	apm_pktdma_op_free(op);
	return -EINVAL;	
}

int apm_pktdma_p2b_xfer(struct apm_pktdma_p2b_params *iodma_p2b)
{
	/* Do dealloc for p2b after getting completion message */
	struct apm_pktdma_msg dma_msg;
	struct apm_qm_msg_desc send_msg;
	struct apm_dma_msg1_2 *msg1_2 = &dma_msg.msg1_2;
	struct apm_dma_msg_3 *msg3 = &dma_msg.msg3;
	struct apm_dma_msg_4 *msg4 = &dma_msg.msg4;
	struct apm_pktdma_flyby_info fb_info;
	struct iodma_op_state *op;

	if (p_dev.qinfo.config_done == 0) {
		PKTDMA_DEBUG("apm_iodma_config_set has not been called.");
		return -1;
	}

	PKTDMA_DEBUG("Executing IODMA direct mode operation.");
	memset(&send_msg, 0, sizeof(struct apm_qm_msg_desc));
	memset(&dma_msg, 0, sizeof(struct apm_pktdma_msg));

	op = apm_pktdma_op_get();
	if (!op)
		return -ENOMEM;
	op->result.cb = iodma_p2b->cb;
	op->result.ctx = iodma_p2b->context;
	op->major_opcode = PKT_TO_BUFF;
	
	/*set common params im2m_moden msg */
	msg1_2->coherent_read = apm_pktdma_is_coherent(); /* set the Coherent bit */
	msg1_2->RType = APM_QM_DMA_RTYPE;
	msg1_2->uinfo = (u32)(unsigned long) op;
	msg3->HE = 1;
	msg3->HR = 1;

	msg3->H0Enq_Num = p_dev.qinfo.queues[apm_processor_id()].comp_qid;
	/*Set to Host QID for Completion messages */
	msg4->BD = 1;
	msg3->H0FPSel = p_dev.qinfo.queues[apm_processor_id()].rx_fp_pbn;
	/*send direct mode msg */
	switch (iodma_p2b->p2b_mode) {
		case IODMA_COPY:
			op->minor_opcode = DMA_DIRECT;
			apm_pktdma_qmesg_load_single_src(&dma_msg, *iodma_p2b->fp_sa,
					*iodma_p2b->byte_count, *iodma_p2b->fp_id);
			break;
		case IODMA_GATHER:
			op->minor_opcode = DMA_GATHER;
			apm_pktdma_qmesg_load_src_mul(&dma_msg, iodma_p2b->fp_sa,
					iodma_p2b->byte_count,
					iodma_p2b->fp_id, iodma_p2b->sg_count,
					op);
			break;
		default:
			break;
	}

	if (iodma_p2b->fby) {
		PKTDMA_DEBUG("Setting CRC and Checksum params");
		memcpy(&fb_info, &(iodma_p2b->fb),
			sizeof(struct apm_pktdma_flyby_info));
		apm_pktdma_crc_set(&fb_info, &dma_msg);
		op->fby_gen = 1;
	}
	PKTDMA_DEBUG("The free pool id mapped = %d", msg3->H0FPSel);
	
	if (msg1_2->NV) {
		PKTDMA_DEBUG_DUMP("P2B 64 Byte Message", (void *) &dma_msg, 64);
	} else {
		PKTDMA_DEBUG_DUMP("P2B 32 Byte Message", (void *) &dma_msg, 32);
	}
#if defined(APM_PKTDMA_DEBUG)
	if (iodma_p2b->cos < IODMA_MIN_COS || iodma_p2b->cos > IODMA_MAX_COS)
		PKTDMA_DEBUG("COS out of range %d", iodma_p2b->cos);
#endif
	send_msg.qid = p_dev.qinfo.tx_q[iodma_p2b->chid].tx_qid[iodma_p2b->cos];
	send_msg.msg = &dma_msg;
	
	PKTDMA_DEBUG("Packet to Buf Comp q_id: %d, enq id: %d, data len: %d",
		    msg3->H0Enq_Num, send_msg.qid, msg1_2->data_len);
	apm_qm_push_msg(&send_msg);
	
	return 0;
}
