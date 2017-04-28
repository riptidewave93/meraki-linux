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
 * @file   apm_sec_access.h
 *
 * This is the Linux layer of the APM86xxx SoC security driver.
 */
#ifndef __APM_SEC_ACCESS_H__
#define __APM_SEC_ACCESS_H__

#include <linux/list.h>
#include <linux/interrupt.h>
#include <linux/err.h>
#include <linux/clk.h>
#include <crypto/algapi.h>
#include <asm/cacheflush.h>
#include <asm/io.h>
#include <asm/scatterlist.h>
#include <asm/apm_ipp_csr.h>
#include <asm/ipp.h>
#include <asm/apm_qm_core.h>

#undef APM_SEC_DEBUG
#undef APM_SEC_SATKNDEBUG
#undef APM_SEC_TXDEBUG
#undef APM_SEC_RXDEBUG
#undef APM_SEC_WRDEBUG
#undef APM_SEC_RDDEBUG
#undef APM_SEC_DEBUGEXTRA

/* Debugging Macro */
#define APMSEC_HDR	"APMSEC: "

#define APMSEC_ERR_DUMP(hdr, d, l) \
	do { \
		print_hex_dump(KERN_INFO, APMSEC_HDR hdr, \
				DUMP_PREFIX_ADDRESS, 16, 4,  d, l, 1); \
	} while(0);
	
#define APMSEC_ERR(fmt, ...) 	\
		printk(KERN_ERR APMSEC_HDR fmt "\n", ##__VA_ARGS__);
#define APMSEC_LOG(fmt, ...) 	\
		printk(KERN_INFO APMSEC_HDR fmt "\n", ##__VA_ARGS__);

#if !defined(APM_SEC_DEBUG)
# define APMSEC_DEBUG(fmt, ...)
# define APMSEC_DEBUG_DUMP(hdr, fmt, ...)
#else
# define APMSEC_DEBUG(fmt, ...)		\
	do { \
		printk(KERN_INFO APMSEC_HDR fmt "\n", ##__VA_ARGS__); \
	} while(0);
# define APMSEC_DEBUG_DUMP(hdr, d, l)	\
	do { \
		print_hex_dump(KERN_INFO, APMSEC_HDR hdr, \
			DUMP_PREFIX_ADDRESS, 16, 4,  d, l, 1); \
	} while(0);
#endif

#if !defined(APM_SEC_DEBUGEXTRA)
# define APMSEC_DEBUGX(fmt, ...)
# define APMSEC_DEBUGX_DUMP(hdr, fmt, ...)
#else
# define APMSEC_DEBUGX(fmt, ...)		\
	do { \
		printk(APMSEC_HDR fmt "\n", ##__VA_ARGS__); \
} while(0);
# define APMSEC_DEBUGX_DUMP(hdr, d, l)	\
	do { \
		print_hex_dump(KERN_INFO, APMSEC_HDR hdr, \
			DUMP_PREFIX_ADDRESS, 16, 4,  d, l, 1); \
	} while(0);
#endif

#if !defined(APM_SEC_TXDEBUG)
# define APMSEC_TXLOG(fmt, ...)
# define APMSEC_TXDUMP(hdr, d, l)
#else
# define APMSEC_TXLOG(fmt, ...)		\
	do { \
		printk(KERN_INFO APMSEC_HDR fmt "\n", ##__VA_ARGS__); \
	} while(0);
# define APMSEC_TXDUMP(hdr, d, l)	\
	do { \
		print_hex_dump(KERN_INFO, APMSEC_HDR hdr, \
			DUMP_PREFIX_ADDRESS, 16, 4,  d, l, 1); \
} while(0);
#endif	
	
#if !defined(APM_SEC_RXDEBUG)
# define APMSEC_RXLOG(fmt, ...)
# define APMSEC_RXDUMP(hdr, d, l)
#else
# define APMSEC_RXLOG(fmt, ...)		\
	do { \
		printk(KERN_INFO APMSEC_HDR fmt "\n", ##__VA_ARGS__); \
	} while(0);
# define APMSEC_RXDUMP(hdr, d, l)	\
	do { \
		print_hex_dump(KERN_INFO, APMSEC_HDR hdr, \
			DUMP_PREFIX_ADDRESS, 16, 4,  d, l, 1); \
	} while(0);
#endif		

#if !defined(APM_SEC_SATKNDEBUG)
# define APMSEC_SATKNLOG(fmt, ...)
# define APMSEC_SADUMP(s, l)
# define APMSEC_TKNDUMP(t)		
#else
# define APMSEC_SATKNLOG(fmt, ...)	\
	do { \
		printk(KERN_INFO APMSEC_HDR fmt "\n", ##__VA_ARGS__); \
	} while(0);
# define APMSEC_SADUMP(s, l) 		sec_sa_dump((s), (l))
# define APMSEC_TKNDUMP(t)		sec_tkn_dump((t))
#endif		

#if !defined(APM_SEC_RDDEBUG)
# define APMSEC_RDLOG(fmt, ...)
#else
# define APMSEC_RDLOG(fmt, ...)		\
	do { \
		printk(KERN_INFO APMSEC_HDR fmt "\n", ##__VA_ARGS__); \
	} while(0);
#endif		

#if !defined(APM_SEC_WRDEBUG)
# define APMSEC_WRLOG(fmt, ...)
#else
# define APMSEC_WRLOG(fmt, ...)		\
	do { \
		printk(KERN_INFO APMSEC_HDR fmt "\n", ##__VA_ARGS__); \
	} while(0);
#endif		

#define PADDR(x)	__pa(x)
#define VADDR(x)	__va(x)
#define APMSEC_ALLOC(x)	kzalloc(x, GFP_ATOMIC)
#define APMSEC_FREE(x)	kfree(x)

#define APMSEC_COH_ALLOC(size) _APMSEC_COH_ALLOC(size, __FILE__, __LINE__)
void *_APMSEC_COH_ALLOC(int size, const char *file, int lnum);
void APMSEC_COH_FREE(void *ptr);

int apm_sec_rd32(u8 block_id, u32 reg_addr, u32 *data_val);
int apm_sec_wr32(u8 block_id, u32 reg_addr, u32 data_val);
int apm_sec_setup_crypto(struct crypto_async_request *req);
struct apm_sec_qm_queues * apm_sec_get_queue(void);
void *apm_sec_dma_addr(void);
 u64 apm_sec_dma_paddr(void);
#endif
