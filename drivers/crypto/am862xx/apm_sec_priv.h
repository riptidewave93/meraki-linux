/**
 * AppliedMicro APM862xx SoC Security Driver
 *
 * Copyright (c) 2009 Applied Micro Circuits Corporation.
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
 * @file   apm_sec_priv.h
 *
 * This file defines the private crypto driver structure and messaging
 * format of the security hardware.
 *
 */
#ifndef __PPRO_SEC_PRIV_H__
#define __PPRO_SEC_PRIV_H__

#include "apm_sec_access.h"
#include "apm_sec_tkn.h"
#include "apm_sec_sa.h"

#define MAX_CORES			2
#define TOTAL_COS			8
#define DEFAULT_COS			4

#define APM_SEC_CSR_SIZE		(64*1024) /* Total range */
#define APM_SEC_GLBL_CTRL_CSR_OFFSET	0x0000
#define APM_EIP96_AXI_CSR_OFFSET	0x2800
#define APM_EIP96_CSR_OFFSET		0x3000
#define APM_EIP96_CORE_CSR_OFFSET	0x3800
#define APM_SEC_GLBL_DIAG_OFFSET	0X7000

#define SEC_GLB_CTRL_CSR_BLOCK		1
#define EIP96_AXI_CSR_BLOCK		2
#define EIP96_CSR_BLOCK			3
#define EIP96_CORE_CSR_BLOCK		4

#ifndef HIDWORD
#	define HIDWORD(x)		((u32) (((u64)(x)) >> 32))
#	define LODWORD(x)		((u32) ((x) & 0xFFFFFFFF))
#endif

#define apm_processor_id()       	mfspr(SPRN_PIR)

#define APM_SEC_TKN_CACHE_MAX		256 /* FIXME - Parameter needs tuning */
#define APM_SEC_SA_CACHE_MAX		32 /* FIXME - Parameter needs tuning */
#define APM_SEC_QMSG_CACHE_MAX		256 /* FIXME - Parameter needs tuning */

/* Total number of extra linked buffer */
#define APM_SEC_LINK_ADDR_MAX		16

/* Errors reported in the Completion message */
#define ERR_MSG_AXI		1
#define ERR_BAD_MSG		2
#define ERR_READ_DATA_AXI	3
#define ERR_WRITE_DATA_AXI	4
#define CRYPTO_IP_ERR		5
#define SEC_ERR_BAD_MSG		6
#define INVALID_MSG_ERR		7

/* Change this according to the queue size and msg size
Now, Max size = queue size / Msg size(64KB/32)
Setting the threshold to 75% of maz size */
#define APM_SEC_CMPQ_THRES	1536

/* Message format for the first two DWORD */
struct apm_sec_msg1_2 {
	u16 srcdom_id	:8;
	u16 coherent_read :1;
	u16 data_len	:15;
	u8 rsvd		:4;
	u16 addr_hi	:4;
	u32 addr_lo;
	
	u16 HL		:1;
	u16 LErr	:3;
	u16 RType	:4;
	u16 PV		:1;
	u16 SB		:1;
	u16 HB		:1;
	u16 PB		:1;
	u16 LL		:1;
	u16 NV		:1;
	u16 HC		:2;
	u16 rsvd2	:8;
	u16 FPQNum	:8;
	u32 uinfo;
	
} __attribute__ ((packed));

/* Message format for the third DWORD */
struct apm_sec_msg_3 {
	u8 rsvd		:2;
	u16 H0FPSel	:4;
	u16 HR		:1;
	u16 HE		:1;
	u16 DR		:1;
	u16 SZ		:1;
	u8 rsvd1	:6;
	u16 H0Enq_Num	:8;
	u8 mm		:1;
	u8 ml		:1;
	u8 rsvd2	:2;
	u8 ll_len_lsb	:4;
	u32 tkn_addr;
}__attribute__ ((packed));

/* Message format for the 4th DWORD */
struct apm_sec_msg_4 {
	u8 rsvd		:2;
	u16 H1FPSel	:4;
	u16 HR		:1;
	u16 HE		:1;
	u16 DR		:1;
	u16 SZ		:1;
	u8 rsvd1	:6;
	u16 H1Enq_Num	:8;
	u16 ll_len_msb	:4;
	u16 dst_addr_hi	:4;
	u32 dst_addr_lo;
}__attribute__ ((packed));

struct apm_sec_msg_address {
	u16 next_fpq_id	:8;
	u8 rsvd		:1;
	u16 next_data_len:15;
	u8  rsvd2	:4;	/* AB bit will be set here */
	u8  next_addr_hi:4;
	u32 next_addr_lo;
} __attribute__ ((packed));

union apm_sec_addr_list {
	struct apm_sec_msg_address addr[4];
	struct {
		struct apm_sec_msg_address addr[3];
		struct {
			u32 total_length_link	:20; /* Not used in security */
			u16 linksize:8;		     /* # of entry in linked
							list where 0 = 256 */
			u16 link_data_ptr_hi	:4;
			u32 link_data_ptr_lo;
		} __attribute__ ((packed)) addr_link;
	} desc;
} __attribute__ ((packed));

/* Complete message structure */
struct apm_sec_msg {
	struct apm_sec_msg1_2 msg1_2;
	struct apm_sec_msg_3 msg3;
	struct apm_sec_msg_4 msg4;
	union apm_sec_addr_list addr_list;
	struct apm_sec_msg_address src_addr_link[APM_SEC_LINK_ADDR_MAX];
#if defined(CONFIG_APM86xxx_IOCOHERENT)
	struct apm_sec_msg_address *coh_dst_addr_link;
#else
	struct apm_sec_msg_address dst_addr_link[APM_SEC_LINK_ADDR_MAX];
#endif
	struct list_head next;
};

#define APM_SEC_MSG_HDR_SIZE	(sizeof(struct apm_sec_msg1_2) + \
				 sizeof(struct apm_sec_msg_3) + \
				 sizeof(struct apm_sec_msg_4))
#define APM_SEC_MSG_SIZE	(APM_SEC_MSG_HDR_SIZE + 32)

/* Message format for the third DWORD */
struct apm_sec_msg_result_3 {
	u8 rsvd		:2;
	u16 H0FPSel	:4;
	u16 HR		:1;
	u16 HE		:1;
	u16 DR		:1;
	u16 SZ		:1;
	u8 rsvd1	:6;
	u16 H0Enq_Num	:8;
	u8 rsvd2	:8;
	u32 tkn_addr;
}__attribute__ ((packed));

struct apm_sec_msg_result_4 {
	u16 rsvd1	:2;
	u16 H1FP_sel	:4;
	u16 HR		:1;
	u16 HE		:1;
	u16 DR		:1;
	u16 SZ		:1;
	u8  rsvd2	:6;
	u8  H1Enq_Num;
	u8  H1_info_hi;
	u32 H1_info_lo;
}__attribute__ ((packed));

struct apm_sec_msg_result {
	struct apm_sec_msg1_2 		msg1_2;
	struct apm_sec_msg_result_3 	msg3;
	struct apm_sec_msg_result_4	msg4;			
};

struct apm_sec_qm_queues {
	int no_tm;		 	/* Lite version */
	int vqid;		 	/* VQ id for TM configuration */
	int vqid_alg;		 	/* VQ scheduling alg */
	unsigned char tx_qid_param[TOTAL_COS]; 	/* Parameter value for TM on each PQ */	
	int tx_qid[TOTAL_COS];	 	/* 8 CoS queue for enqueue */	
	int comp_qid[MAX_CORES]; 	/* completion queue for PPCx */
	int comp_mbid[MAX_CORES];
};

struct apm_sec_session_ctx {
	struct sec_sa_item   *sa;	/* Allocate outbound SA */
	struct sec_sa_item   *sa_ib;	/* Allocate inbound SA if needed */
	spinlock_t lock;

	struct list_head  tkn_cache;
	u32 tkn_cache_cnt;
	u16 tkn_max_len;
	u16 tkn_input_len;
	
	struct list_head  sa_cache;
	u32 sa_cache_cnt;	
	u16 sa_len;
	u16 sa_max_len;

	struct list_head  qmsg_cache;
	u32 qmsg_cache_cnt;	

	int sa_flush_done;
	u16 pad_block_size;	/* For ESP offload */
	u16 encap_uhl;		/* For ESP offload - ENCAP UDP header length */
};

int apm_sec_hwreset(void);
int apm_sec_hwinit(void);
int apm_sec_hwstart(void);
int apm_sec_hwstop(void);
u32 apm_sec_v2hwaddr(void *vaddr);
void *apm_sec_hwaddr2v(u32 hwaddr);
struct sec_tkn_ctx *apm_sec_rmsg2tkn(struct apm_sec_msg_result *rmsg);
int apm_sec_queue2hw(struct apm_sec_session_ctx *session,
			struct sec_tkn_ctx *tkn);
void apm_sec_intr_hdlr(void);

struct apm_sec_session_ctx *apm_sec_session_create(void);
void	apm_sec_session_init(struct apm_sec_session_ctx *session);
void	apm_sec_session_free(struct apm_sec_session_ctx *session);
int 	apm_sec_create_sa_tkn_pool(struct apm_sec_session_ctx *session,
				 u32 sa_max_len, u32 sa_len, 
				 char sa_ib, u32 tkn_len);
void 	apm_sec_free_sa_tkn_pool(struct apm_sec_session_ctx *session);
struct 	sec_tkn_ctx *apm_sec_tkn_get(struct apm_sec_session_ctx *session, 
				u8 *new_tkn);
void 	apm_sec_tkn_free(struct apm_sec_session_ctx *session, 
				struct sec_tkn_ctx *tkn);
struct 	sec_sa_item *apm_sec_sa_get(struct apm_sec_session_ctx *session);
void 	apm_sec_sa_free(struct apm_sec_session_ctx *session, 
				struct sec_sa_item *sa);
void apm_sec_qmsg_free(struct apm_sec_session_ctx *session, 
			struct apm_sec_msg *qmsg);
struct apm_sec_msg * apm_sec_qmsg_get(struct apm_sec_session_ctx *session); 

void apm_sec_qmesg_load_dst_single(struct apm_sec_msg * sec_msg, void *ptr, 
					int nbytes);
void apm_lsec_sg2buf(struct scatterlist *src, int nbytes, char *saddr);
int apm_lsec_sg_scattered(unsigned int nbytes, struct scatterlist *sg);

#endif
