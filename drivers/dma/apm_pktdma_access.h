/**
 *  APM86xxx PktDMA driver
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
 * This is the header file for APM PktDMA Linux device driver.
 *
 */
#ifndef __APM_PKTDMA_ACCESS_H__
#define __APM_PKTDMA_ACCESS_H__

#include <linux/types.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/async_tx.h>
#include <linux/dma-mapping.h>
#include <linux/dmaengine.h>
#include <linux/proc_fs.h>
#include <linux/of_platform.h>
#include <linux/highmem.h>
#include <linux/spinlock_types.h>
#include <asm/io.h>
#include <linux/errno.h>
#include "apm_pktdma_core.h"
#include "apm_dma_csr.h"
#include <asm/apm_pktdma.h>

#undef APM_PKTDMA_DEBUG
#undef APM_PKTDMA_DRXTX
#undef APM_PKTDMA_DUMP_TX

#define PKTDMA_HDR		"PKTDMA: "

#ifdef APM_PKTDMA_DEBUG
#define PKTDMA_DEBUG(fmt, ...)		\
		printk(KERN_INFO PKTDMA_HDR fmt "\n", ##__VA_ARGS__);
#else
#define PKTDMA_DEBUG(fmt, ...)
#endif

#ifdef APM_PKTDMA_DCSR
#define PKTDMA_DCSR(fmt, ...)		\
		printk(KERN_INFO PKTDMA_HDR fmt "\n", ##__VA_ARGS__);
#else
#define PKTDMA_DCSR(fmt, ...)
#endif

#ifdef APM_PKTDMA_DRXTX
#define PKTDMA_DRXTX(fmt, ...)		\
		printk(KERN_INFO PKTDMA_HDR fmt "\n", ##__VA_ARGS__);
#else
#define PKTDMA_DRXTX(fmt, ...)
#endif

#if 0
#define APM_PKTDMA_DOP_CACHE(fmt, ...)	\
		printk(KERN_INFO PKTDMA_HDR fmt "\n", ##__VA_ARGS__);
#else
#define APM_PKTDMA_DOP_CACHE(fmt, ...)
#endif

#define PKTDMA_ERR(fmt, ...)		\
		printk(KERN_ERR PKTDMA_HDR fmt "\n", ##__VA_ARGS__);

#if !defined(APM_PKTDMA_DUMP_TX)
#define PKTDMA_DEBUG_DUMP(hdr, fmt, ...)
#else
#define PKTDMA_DEBUG_DUMP(hdr, d, l)	\
	do { \
		print_hex_dump(KERN_INFO, PKTDMA_HDR hdr, \
			DUMP_PREFIX_ADDRESS, 16, 4,  d, l, 1); \
	} while(0);
#endif

struct apm_pktdma_chan {
	int pending;
	dma_cookie_t completed_cookie;
	struct dma_chan common;
};

struct pktdma_ch_data {
	struct dma_async_tx_descriptor *async_tx;
	phys_addr_t src_addr;
	phys_addr_t dest_addr;
	u16 len_arr;
};

struct pktdma_percpu_data {
	spinlock_t lock;
	struct list_head head;
	struct tasklet_struct rx_tasklet;
};

struct iodma_pdev {
	struct device *device;
	struct of_device *ofdev;
	
	phys_addr_t csr_base_p;
	void *csr_base;
	void *csr_base_glb;
	void *csr_base_qmi;

	void *percpu_ptr;
	struct tasklet_struct rx_tasklet;
	struct pktdma_q_info qinfo;

	int chan[PKTDMA_CHID_MAX];	
	struct pktdma_ch_data ch_data[4]; /* #define later */
	struct dma_device common;
	dma_cap_mask_t c_mask;
	u64 apm_pktdma_dmamask;
	spinlock_t async_lock;
	spinlock_t queue_lock;
	spinlock_t cache_lock;
	
	int poweroff;
	u64 smp_core0;
	u64 smp_core1;
};

extern struct iodma_pdev p_dev;

#endif /* __APM_PKTDMA_ACCESS_H__ */
