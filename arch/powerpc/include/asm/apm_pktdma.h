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
 * This is the public header file for APM PktDMA Linux device driver.
 *
 */
#ifndef __APM_PKTDMA_H__
#define __APM_PKTDMA_H__

#include <asm/apm_qm_access.h>

/** IODMA Minor mode */
#define IODMA_COPY		0 /**< Copy operation */
#define IODMA_SCATTER		1 /**< Scatter - in pointer mode only */
#define IODMA_GATHER		2 /**< Gather - in pointer mode only */
#define IODMA_STRIDE		3
#define IODMA_GATHER_SCATTER	4

/** Default Cos index */
#define IODMA_DEFAULT_COS	4
#define IODMA_MIN_COS		0
#define IODMA_MAX_COS		7

/** Transfer results */
enum apm_pktdma_op_result_e {
	IODMA_SUCCESS,		/**< Completed successfully */
	IODMA_BAD_SOURCE,	/**< Invalid source */
	IODMA_BAD_DEST,		/**< invalid destination */
	IODMA_SOURCE_OP_ERROR,	/**< data transfer error from source */
	IODMA_DEST_OP_ERROR,	/**< data transfer error to destination */
	IODMA_CS_CRC_OK = 8,	/**< Checksum or CRC checked OK */
	IODMA_CS_CRC_FAILED = 12, /**< Checksum or CRC check failed */
	IODMA_GENERIC_OP_ERROR,   /**< Host mode transfer error */
};

/** Fly-by function to be executed by the IO DMA fly-by engine */
enum apm_pktdma_fb_op_type {
	IODMA_FLY_BY_NOP,	/**< No fly-by operation requested */
	IODMA_FLY_BY_CHKSUM,	/**< Checksum operation - see CHKSUM_XXX for
					opcodes */
	IODMA_FLY_BY_CRC,	/**< CRC operation - see CRC_XXX for opcodes */
	IODMA_FLY_BY_TCP=4,	/**< TCP offload (supported on packet to buffer
				     and memory to queue operations only)
				     - see opcode TCP_XXX */
	IODMA_FLY_BY_BYTE	/**< Byte manipulate - see opcode UNIT_XXX */
};

/** Fly-by opcodes */

/** CRC opcodes */
#define DMA_CRC_CHKSUM_GEN	0
#define DMA_CRC_CHKSUM_CHECK	1

#define DMA_SEED_SELECT_DEFAULT	0
#define DMA_SEED_SELECT_MSG	1

#define DMA_FBY_NONE		0
#define DMA_FBY_CRC16		1
#define DMA_FBY_CRC32		2
#define DMA_FBY_CHKSUM		3
#define DMA_FBY_XOR_2SRC	4
#define DMA_FBY_XOR_3SRC	5
#define DMA_FBY_XOR_4SRC	6
#define DMA_FBY_XOR_5SRC	7

struct stride_info {
	u16 dst_str_dis;
	u16 dst_str_1_sz;
	u16 dst_str_sz;
	u16 src_str_dis;
	u16 src_str_1_sz;
	u16 src_str_sz;
};

struct fp_info {
	u8  fp_id;		
	void * fp_dest_address;	
	u16 fp_dlength;
};

struct dma_chan_info {
	int max_chan;
	int chan_en[4];
};

struct apm_pktdma_op_result;

typedef int (*apm_pktdma_cb_ptr)(struct apm_pktdma_op_result *iodma_op_result);

/** DMA transfer operation callback argument structure */
struct apm_pktdma_op_result {
	u8  err;		/**< operation results -
				     see apm_pktdma_op_result_e */
	u16 checksum; 		/**< checksum value if applicable */
	u32 crc;		/**< crc value if applicable */
	void *ctx;		/**< user context from request */
	u32 tstamp;
	struct fp_info *fp;
	u16 num_fp;
	
	void *comp_msg;
	
	apm_pktdma_cb_ptr cb;	
	u8 opaque[0];		/**< opaque for upper layer */
};

struct apm_pktdma_flyby_info {
	u8 fb_type;		/**< fly by operation -
				     refer apm_pktdma_fb_op_type */
	u8 fb_opc;		/**< fly by opcode. Multiple relevant values
				     can be ORed together  */
	u16 fb_byte_count;	/**< byte count for chksum, crc or tcp chksum.
				     This does not include the byte_offset
				     and the header offset */
	u32 fb_seed_chksum;	/**< chksum seed or crc16 seed or
				     TCP pseudo header chksum */
	u8 fb_seed_select;
	u8 fb_gen_check;
};

/** IODMA memory to memory transfer params struct */
struct apm_pktdma_m2m_params {
	u8 chid;		/**< IODMA channel - 0 to IODMA_CHID_MAX */
	u8 cos;			/**< CoS index for TM */
	u8 m2m_mode;		/**< IODMA_COPY or IODMA_SCATTER or
				     IODMA_GATHER IODMA_GATHER_SCATTER*/
	u8 sg_count;
	u64 *sa;		/**< pointer to 40 bit physical source
				     address array */
	u64 *da;		/**< pointer to 40 bit physical destination
				     address array  */
	u16 *byte_count;	/**< byte count per scatter gather op */
	apm_pktdma_cb_ptr cb;   /**< user call back function to be invoked
				     on dma completion */
	void *context;		/**< user context pointer */
	struct stride_info str;
};

/** IODMA memory to buffer transfer params struct */
struct apm_pktdma_m2b_params {
	u8 chid;
	u8 cos;			/**< CoS index for TM */
	u8 m2b_mode;		/**< IODMA_COPY or IODMA_SCATTER
				     or IODMA_GATHER */
	u8 sg_count;		/**< sg op count */
	u64 *sa;		/**< pointer to 40 bit physical source address
				     array - only one for scatter */
	u16 *byte_count;	/**< byte count per scatter gather op */
	apm_pktdma_cb_ptr cb;   /**< user call back function to be invoked
				     on dma completion */
	void *context;		/**< user context pointer */

	u8 fby;
	struct apm_pktdma_flyby_info fb;
				/**< flyby operation control structure */
	int cb_compl_msg;
	u32 cross_pbn;
};

/** IODMA packet to memory transfer params struct */
struct apm_pktdma_p2m_params {
	u8 chid;		/**< IODMA channel - 0 to IODMA_CHID_MAX */
	u8 cos;			/**< CoS index for TM */
	u8 p2m_mode;		/**< IODMA_PKT_COPY, IODMA_PKT_MOVE,
				     IODMA_PKT_CPY_GT, IODMA_PKT_MOV_GT */
	u8 sg_count;		/**< sg op count */
	u8 *fp_id;		/**< source packet free pool id for d_address */
	u64 *fp_sa;		/**< 256 byte aligned physical source packet
				     address array - only one for scatter */
	u16 *d_length;		/**< source packet length array */
	u64 *da;		/**< pointer to 40 bit physical destination
				     address array - only for gather */
	u16 *byte_count;	/**< byte count array per gather op */
	apm_pktdma_cb_ptr cb;   /**< user call back function to be invoked
				     on dma completion */
	void *context;		/**< user context pointer */
	struct apm_pktdma_flyby_info  fb;
				/**< flyby operation control structure */
};

/** IODMA packet to buffer transfer params struct */
struct apm_pktdma_p2b_params {
	u8 chid;		/**< IODMA channel - 0 to IODMA_CHID_MAX.
				     Channel is mapped to a free pool */
	u8 cos;			/**< CoS index for TM */
	u8 p2b_mode;		/**< IODMA_PKT_COPY, IODMA_PKT_MOVE or
				     IODMA_PKT_CPY_GT, IODMA_PKT_MOV_GT,
				     IODMA_PKT_MCAST */
	u8 sg_count;		/**< sg op count */
	u8* fp_id;		/**< source packet free pool id for d_address
				     - Note: only one source pool is
				     supported for gather */
	u64 *fp_sa;
	u16 *byte_count;	/**< byte count array per gather op */
	apm_pktdma_cb_ptr cb;   /**< user call back function to be invoked
				     on dma completion */
	void *context;		/**< user context pointer */
	struct apm_pktdma_flyby_info fb;
	u8 fby;			/**< flyby operation control structure */
};

struct apm_pktdma_buf_info {
	void *addr;
	u16 datalen;
	u16 fp_id;
};

/**
 * @brief   Abstracted IODMA memory to memory transfer operation routine.
 * @param   params      apm_pktdma_m2m_params structure pointer
 */
int apm_pktdma_m2m_xfer(struct apm_pktdma_m2m_params *params);

/**
 * @brief   Abstracted IODMA memory to buffer transfer operation routine.
 * @param   params      apm_pktdma_m2b_params structure pointer
 */
int apm_pktdma_m2b_xfer(struct apm_pktdma_m2b_params *params);

/**
 * @brief   Abstracted IODMA packet to memory transfer operation routine.
 * @param   params      apm_pktdma_p2m_params structure pointer
 */
int apm_pktdma_p2m_xfer(struct apm_pktdma_p2m_params *params);

/**
 * @brief   Abstracted IODMA packet to packet transfer operation routine.
 * @param   params      apm_pktdma_p2b_params structure pointer
 */
int apm_pktdma_p2b_xfer(struct apm_pktdma_p2b_params *params);

/**
 * @brief   Rountine to get the ppc fp address
 * @param   params      apm_pktdma_buf_info structure pointer
 */
int apm_pktdma_get_fp(struct apm_pktdma_buf_info *fp_buf);
/**
 * @brief   Rountine to get the total channels available and the channel ids
 * @param   params     dma_chan_info  structure pointer
 */
void apm_pktdma_get_total_chan(struct dma_chan_info *chan_info);
/**
 * @brief   Rountine to flush the dest
 * @param   params     physical destination pointer and size
 */
void apm_pktdma_flush_dest(void *daddr, int byte_cnt);
/**
 * @brief   Rountine to invalidate the dest
 * @param   params     virtual destination pointer and size
 */
void apm_pktdma_invalidate_dest(void *da, int byte_cnt);
/**
 * @brief   Rountine to flush the src
 * @param   params     virtual destination pointer and size
 */
 void apm_pktdma_flush_src(void *saddr, int byte_cnt);
#endif /* __APM_PKTDMA_H__ */
