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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * @file   apm_lac_access.h
 *
 * This is the Linux layer of the APM86xxx SoC Look aside Clasifier Driver 
 */
#ifndef __APM_LAC_ACCESS_H__
#define __APM_LAC_ACCESS_H__

#include <linux/list.h>
#include <linux/interrupt.h>
#include <asm/io.h>
#include <asm/ipp.h>
#include <linux/err.h>
#include <linux/clk.h>

#if 0
#define APM_LAC_DEBUG
#define APM_LAC_TXDEBUG
#define APM_LAC_RXDEBUG
//#else
#define APM_LAC_WRDEBUG
#define APM_LAC_RDDEBUG
#endif
#define CARBON_MODEL

/* Debugging Macro */
#define APMLAC_HDR	"APMLAC: "

#define APMLAC_ERR(fmt, ...) 	\
		printk(KERN_ERR APMLAC_HDR fmt "\n", ##__VA_ARGS__);

#define APMLAC_LOG(fmt, ...) 	\
		printk(KERN_INFO APMLAC_HDR fmt "\n", ##__VA_ARGS__);

#if !defined(APM_LAC_DEBUG)
# define APMLAC_DEBUG(fmt, ...)
# define APMLAC_DEBUG_DUMP(hdr, fmt, ...)
#else
# define APMLAC_DEBUG(fmt, ...)		\
	do { \
		printk(KERN_INFO APMLAC_HDR fmt "\n", ##__VA_ARGS__); \
	} while(0);

# define APMLAC_DEBUG_DUMP(hdr, d, l)	\
	do { \
		print_hex_dump(KERN_INFO, APMLAC_HDR hdr, \
			DUMP_PREFIX_ADDRESS, 16, 4,  d, l, 1); \
	} while(0);
#endif

#if !defined(APM_LAC_TXDEBUG)
# define APMLAC_TXLOG(fmt, ...)
# define APMLAC_TXDUMP(hdr, d, l)
#else
# define APMLAC_TXLOG(fmt, ...)		\
	do { \
		printk(KERN_INFO APMLAC_HDR fmt "\n", ##__VA_ARGS__); \
	} while(0);
# define APMLAC_TXDUMP(hdr, d, l)	\
	do { \
		print_hex_dump(KERN_INFO, APMLAC_HDR hdr, \
			DUMP_PREFIX_ADDRESS, 16, 4,  d, l, 1); \
} while(0);
#endif	
	
#if !defined(APM_LAC_RXDEBUG)
# define APMLAC_RXLOG(fmt, ...)
# define APMLAC_RXDUMP(hdr, d, l)
#else
# define APMLAC_RXLOG(fmt, ...)		\
	do { \
		printk(KERN_INFO APMLAC_HDR fmt "\n", ##__VA_ARGS__); \
	} while(0);
# define APMLAC_RXDUMP(hdr, d, l)	\
	do { \
		print_hex_dump(KERN_INFO, APMLAC_HDR hdr, \
			DUMP_PREFIX_ADDRESS, 16, 4,  d, l, 1); \
	} while(0);
#endif		


#if !defined(APM_LAC_RDDEBUG)
# define APMLAC_RDLOG(fmt, ...)
#else
# define APMLAC_RDLOG(fmt, ...)		\
	do { \
		printk(KERN_INFO APMLAC_HDR fmt "\n", ##__VA_ARGS__); \
	} while(0);
#endif		

#if !defined(APM_LAC_WRDEBUG)
# define APMLAC_WRLOG(fmt, ...)
#else
# define APMLAC_WRLOG(fmt, ...)		\
	do { \
		printk(KERN_INFO APMLAC_HDR fmt "\n", ##__VA_ARGS__); \
	} while(0);
#endif		

#define PADDR(x)	__pa(x)
#define VADDR(x)	__va(x)
#define APMLAC_ALLOC(x)	kmalloc(x, GFP_KERNEL)
#define APMLAC_FREE(x)	kfree(x)
#define MAX_CORES	2
#define apm_processor_id() mfspr(SPRN_PIR)

#define APM_LAC_NO_PKT_BUF	64
#define APM_LAC_POOL_SIZE	(2 << 12) /* 2KB pool size */
#define APM_LAC_BUF_SIZE 	1518 	  /* Must be greater or equal
					     to pool size or max MTU */
#define APM_LAC_SKB_RESERVE_SIZE 0
#define APM_LAC_MAX_FRAG	259	/* 4 + 255 LL Buffer */

/* Queues related parameters for Look Aside Classifier */
struct lac_queues {
	u32 rx_qid;
	u32 rx_mbid;	/* mailbox id for receive queue */
	u32 fp_qid;
	u32 fp_mbid;
	u32 fp_pbn;
	u32 tx_qid;	/* Endque q id	      */
	u32 comp_qid;	/* completion q id    */
	u32 comp_mb;	/* completion mailbox */
};

/*
 * @struct  apm_lac_msg_ext8
 * @brief   This structure represents 8 byte portion of QM extended (64B)
 *	    message format
 */
struct apm_lac_msg_ext8 {
	u32 NxtFPQNum		:8;
	u32 Rv1			:1;
	u32 NxtBufDataLength	:15;
	u32 Rv2			:4;
	u32 NxtDataAddrMSB	:4;
	u32 NxtDataAddrLSB;
}__attribute__ ((packed));

struct lac_stats {
	u32 req_sent_cnt;
	u32 res_rcvd_cnt;
	u32 comp_msg_cnt;
	u32 fp_dealloc_cnt;
	u32 lerr_cnt;
};

int apm_lac_rd32(u32 reg_offset, u32  *value);
int apm_lac_wr32(u32 reg_offset, u32 value);

#endif
