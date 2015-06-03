/**
 * APM PktDMA driver
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
 * This is the header file for APM PktDMA core driver.
 */

#ifndef __APM_IODMA_CORE_H__
#define __APM_IODMA_CORE_H__

#include <asm/apm_qm_core.h>
#include <asm/apm_pktdma.h>

#ifndef HIDWORD
#define HIDWORD(v)		((u32) ((v) >> 32))
#define LODWORD(v)		((u32) (v & 0xFFFFFFFF))
#define MAKE64(h,l)		(((u64) (h) << 32) | (l)) 
#endif

#define IODMA_MAX_OPS		(1024 * 2) /* Max inflight op allowed */
#define NUM_CPUS		2

#ifdef CONFIG_NOT_COHERENT_CACHE
#define PKTDMA_FLUSH_DCACHE
#endif

#define APM_IODMA_GLBL_DIAG_CSR_OFFSET	0X7000
#define APM_IODMA_QMI_SLAVE_OFFSET	0X7800

/** HW constructs */
#define APM_PKTDMA_OP_CACHE_MAX 64
#define PKTDMA_CHID_MAX		4	/* Max channel id starting from 0 */
#define IODMA_MAX_QUEUE		4
#define IODMA_MAX_DESTQ		4
#define DMA_QUEUE_MASK		0xf
#define DMA_OUTSTANDING_MASK	0X00000ff0

#define IODMA_HOST_PRIORITY	7
#define IODMA_AXI_ATTRIBUTES	0
#define IODMA_AXI_COHERENT	1
#define APM_PKTDMA_NUM_COS	(IODMA_MAX_COS + 1)

/** Major mode */
#define MEM_TO_MEM		0
#define PKT_TO_MEM		1
#define PKT_TO_BUFF		2
#define MEM_TO_BUFF		3

/** Transfer types */
#define DMA_DIRECT		0
#define DMA_SCATTER		1
#define DMA_GATHER		2
#define DMA_SCAT_GATH		3
#define DMA_STRIDE		4

#define COMP_FIRST		0x20
#define COMP_LAST		0x10

#define BUFFER_16K		0
#define BUFFER_4K		4
#define BUFFER_2K		5
#define BUFFER_1K		6
#define BUFFER_256		7

#define PKTDMA_ALIGN_PTR(x)		(((unsigned long) (x) + 15) & ~0xF)
#define PKTDMA_ALIGN_PTR_128(x)		(((unsigned long) (x) + 127) & ~0x7F)

#if defined (CONFIG_APM867xx)
#define ERR_FBP_TIMEOUT		5
#define ERR_SCT_GAT_LEN		7
#define ERR_CRC_CHKSUM 		0x1
#endif

#define ERR_MSG_AXI		1
#define ERR_BAD_MSG		2
#define ERR_READ_DATA_AXI	3
#define ERR_WRITE_DATA_AXI	4
#define CRC_ERR			5
#define CHK_ERR			6

#define FREE_POOL_DMA_BUFFER_SIZE	(2 * 1024)
#define FREE_POOL_DMA_BUFFER_NUM	16

#define FREE_POOL_PPC_BUFFER_SIZE	(2 * 1024)
#define FREE_POOL_PPC_BUFFER_NUM	16

#define apm_processor_id()       mfspr(SPRN_PIR)

/** IODMA msg1_ format first long word */
struct apm_dma_msg1_2 {
	u16 srcdom_id:8;
	u16 coherent_read:1;
	u16 data_len:15;
	u8 rsvd:3;
	u8 AB:1;
	u16 addr_hi:4;
	u32 addr_lo;
	
	u16 HL:1;
	u16 LErr:3;
	u16 RType:4;
	u16 PV:1;
	u16 SB:1;
	u16 HB:1;
	u16 PB:1;
	u16 LL:1;
	u16 NV:1;
	u16 HC:2;
#if defined (CONFIG_APM867xx)
	u16 EL_Err:2;
	u16 rsvd2:6;
#else
	u16 rsvd2:8;
#endif
	u16 FPQNum:8;
	u32 uinfo;
	
} __attribute__ ((packed));

/** IODMA msg3 format first long word */
struct apm_dma_msg_3 {
	u16 rsvd:2;
	u16 H0FPSel:4;
	u16 HR:1;
	u16 HE:1;
	u16 DR:1;
	u16 SZ:1;
	u16 rsvd2:6;
	u16 H0Enq_Num:8;
	union {
		struct {
			u16 cmd:4;
			u16 dest_addr_hi:4;
			u32 dest_addr_lo;
		} __attribute__ ((packed)) dst_m2m;
		
		struct {
			u8 linksize;
			u32 dst_ptr;
		} __attribute__ ((packed)) scatter_m2m;	
	
		struct {
			u16 rsv1;
			u8 rsv2;
			u16 crc_chk_byte_cnt;
		} __attribute__ ((packed)) to_buf_fby;
	} var_data;

}__attribute__ ((packed));

/** IODMA msg4 format first long word */
struct apm_dma_msg_4 {
	union {
		struct {
		u16 rsvd2;
		u8 rsvd3;
		u32 crc_chk_seed;
		} __attribute__ ((packed)) fby;

		struct {
			u16 rsvd:2;
			u16 dst_str_dis:9;
			u16 dst_str_sz:9;
			u16 dst_str_1_sz:9;
			u16 src_str_dis:9;
			u16 src_str_sz:9;
			u16 src_str_1_sz:9;
		}__attribute__ ((packed)) str_op;
	} var_data;
	u8 FBY:3;
	u8 ST:1;
	u8 GN:1;
	u8 SD:1;
	u8 BD:1;
	u8 SC:1;
}__attribute__ ((packed));

/** Done Messages */
struct apm_dma_msg_done_3 {
	u8 rsvd:2;
	u16 H0FPSel:4;
	u16 HR:1;
	u16 HE:1;
	u16 DR:1;
	u16 SZ:1;
	u8 rsvd2:6;
	u16 H0Enq_Num:8;
	u8 rsvd3;
	u32 crc_result;
	
} __attribute__ ((packed));

struct apm_dma_msg_done_4 {
	u32 rsvd;
	u16 rsvd2;
	u16 checksum_res;
}__attribute__ ((packed));

/** Linked List format for DMA Messages */
struct sg_list_attr {
	u16 next_fpq_id:8;
	u8 rsvd1:1;
	u16 next_data_len:15;
	u8  rsvd:4;		/*AB bit will be set here */
	u16  next_addr_hi:4;
	u32 next_addr_lo;
}__attribute__ ((packed));

union list_msg_data {
	struct sg_list_attr src_list[4];
	struct {
		struct sg_list_attr list_ll[3];
		struct {
			u32 total_length_link:20;
			u16 linksize:8;
			u16 ptr_to_src_hi:4;
			u32 ptr_to_src_lo;
		} __attribute__ ((packed)) src_gr_5;
	} desc;
}__attribute__ ((packed));

/** IODMA message structure */
struct apm_pktdma_msg {
	struct apm_dma_msg1_2 msg1_2; /** The first two 8 bytes: 16 bytes in total */
	struct apm_dma_msg_3 msg3;
	struct apm_dma_msg_4 msg4;
	union list_msg_data list_msg_data;
};

struct apm_pktdma_msg_done {
	struct apm_dma_msg1_2 dmsg1_2;
	struct apm_dma_msg_done_3 dmsg3;
	struct apm_dma_msg_done_4 dmsg4;
	union list_msg_data done_list_msg_data;
};

/** IODMA per op state struct */
struct iodma_op_state {
	/* Pointer to the chain if greater than 5 sources
	   in case of scatter/gather */
	struct list_head next;
	u8 major_opcode;
	u8 minor_opcode;
	u8 fby_gen;
		
	struct apm_pktdma_msg_done dmsg;
	
	struct sg_list_attr *src_ptr;
	struct sg_list_attr *dest_ptr;
	
	int cb_compl_msg;
	struct apm_pktdma_op_result result;	/* Must be at end of structure*/	
};

/** PKTDMA Channel - Transmit Qid's mapping structure per channel */
struct pktdma_chan_qid {
	u8 valid;
	int tx_vqid; 	 	/* VQ id for TM configuration */
	int tx_vqid_alg;	/* VQ scheduling alg */
	unsigned char tx_qid_param[APM_PKTDMA_NUM_COS];
				/* Parameter value for TM on each PQ */		
	u16 tx_qid[APM_PKTDMA_NUM_COS];
};

/** Queues parameters for completion per CPU */
struct pktdma_queue_ids {
	u8 valid;
	u32 rx_fp_qid; 		/* PktDMA to PPCx free pool */
	u32 rx_fp_pbn;
	u32 comp_qid;           /* completion q id    */
	u32 comp_mb;            /* completion mailbox */

	u32 ppc_fpid;			
	u32 ppc_fpmbid;
};

/** PKTDMA device structure */
struct pktdma_q_info {
	int config_done;
	int tx_ch_cnt;
	struct pktdma_chan_qid tx_q[PKTDMA_CHID_MAX];
	struct pktdma_queue_ids queues[NUM_CPUS];
	
	struct list_head completed_op_queue;
	struct list_head tsk_queue;
	struct list_head op_cache;
	int total_chan;
	int chan[4];
	u32 op_cache_cnt;
	int no_xor;
	int no_tm;
};

/**
 * @brief   Polling function for the DMA Completion.
 * @param   apm_qm_msg_desc QM message structure.
 */
int apm_pktdma_msg_rx(struct apm_qm_msg_desc *rx_msg_desc);

/**
 * @brief   Configure the Free Pool buffer
 * @param   queue : Queue to be configured
 *	size: size of the buffer
 *	no_of_buffs: number of buffers in the free pool
 */
int apm_pktdma_init_pool(int queue_id, int size, int no_of_buffs);

/**
 * @brief   Buffer len encoding function
 * @param   len: actual length
 * @return  Encoded length
 */
u16 apm_buf_len_set(u16 len);
  
/**
 * @brief   Completion call back from tasklet
 * @param   struct iodma_op_state operation pointer
 * @return  0
 */
int apm_pktdma_op_cb(struct iodma_op_state *op_st);
/**
 * @brief   Operation struct deinit
 * @param   struct iodma_op_state operation pointer
 * @return  0
 */
void apm_pktdma_op_deinit(struct iodma_op_state *op);

/**
 * @brief   alloc per op struct
 * @param   void
 * @return  struct iodma_op_state operation pointer
 */
struct iodma_op_state *apm_pktdma_op_get(void);

/**
 * @brief   Operation struct free
 * @param   struct iodma_op_state operation pointer
 * @return  0
 */
void apm_pktdma_op_free(struct iodma_op_state *op);

/**
 * @brief   get pktdma channel ID
 * @param   void
 * @return  channel id 
 */
int apm_pktdma_get_chan_id(void);

int apm_pktdma_read_reg(u32 offset, u32 *data);
int apm_pktdma_write_reg(u32 offset, u32 data);

#endif /* __APM_PKTDMA_CORE_H__ */
