/**
 * AppliedMicro AM862xx QM Driver
 *
 * Copyright (c) 2010 Applied Micro Circuits Corporation.
 * All rights reserved. Keyur Chudgar <kchudgar@apm.com>
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
 * @file apm_qm_access.h
 **
 */

#ifndef __APM_QM_ACCESS_H__
#define __APM_QM_ACCESS_H__

#define QM_ALTERNATE_ENQUEUE

#include <asm/io.h>
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
#include <asm/pgtable.h>
#if defined(CONFIG_SMP)
#include <asm/smp.h>
#endif

#include <asm/dcr.h>
#include <asm/dcr-regs.h>
#include <asm/dcr-native.h>

/* debugging */

/* Enabled debug macros put here */
#define QM_DBG_ERR_CHK
#define QM_PRINT_ENABLE

#if 0
#define DQM_DBG
#define DQM_DBG_WR
#define DQM_DBG_RD
#define QM_DBG_ERR_CHK
#define QM_DBG_STATE
#define QM_PRINT_ENABLE
#endif

#ifdef DQM_DBG
#define QM_DBG(x, ...)  printk(KERN_INFO x, ##__VA_ARGS__)
#else
#define QM_DBG(x, ...)
#endif

#ifdef QM_PRINT_ENABLE
#define QM_PRINT(x, ...)  printk(KERN_INFO x, ##__VA_ARGS__)
#else
#define QM_PRINT(x, ...)
#endif

#ifdef QM_DBG_ERR_CHK
#define QM_ERR_CHK(x, ...)  printk(KERN_INFO x, ##__VA_ARGS__)
#else
#define QM_ERR_CHK(x, ...)
#endif

#ifdef DQM_DBG_WR
#define QMWRITE_PRINT(x, ...)  printk(KERN_INFO x, ##__VA_ARGS__)
#else
#define QMWRITE_PRINT(x, ...)
#endif

#ifdef DQM_DBG_RD
#define QMREAD_PRINT(x, ...)  printk(KERN_INFO x, ##__VA_ARGS__)
#else
#define QMREAD_PRINT(x, ...)
#endif

#ifdef QM_DBG_STATE
#define QM_STATE_DBG(x, ...)  printk(KERN_INFO x, ##__VA_ARGS__)
#else
#define QM_STATE_DBG(x, ...)
#endif

#define QM_DUMP(m, l)	      print_hex_dump(KERN_INFO, "QM MSG: ", \
				DUMP_PREFIX_ADDRESS, 16, 4, m, l, 1);
		
#define MEMALLOC(x) kmalloc(x, GFP_ATOMIC)

/* Enable Reporting Errors to Error Queue */
#define APM_QM_ERROR_Q_ENABLE   0

/* #ifdef CONFIG_QUEUE_OCM */ /* FIXME, find proper address from DTS if required queue in OCM */ 
#define OCM_QUEUE_BASE_VADDR 0x90000000
#define OCM_QUEUE_BASE_PADDR 0xffff8000
#define OCM_QUEUE_HIGH_ADDR 0xe
/* #endif */

#if 0
#define FLUSH_ENQ_MSG
#endif

#if !defined (CONFIG_SMP)
#if 0
#define FLUSH_MB
#endif

#if 0
#define INVALIDATE_MB
#endif

#if 0
#define CONFIG_COHERENT_MB
#endif

#if 0
#define MEMQ_CACHABLE
#endif
#endif /* #if !defined (CONFIG_SMP) */

#if 0
#define LINUX_QM_HW_TEST
#endif

/* Mailbox SoftIRQ Context */
struct apm_qm_mb_tasklet {
	u32 mailbox;
	u32 queue;
	u32 core;
	void *private;
	struct tasklet_struct tasklet;
};

extern u32 qm_ocm_enq_mbox_addr;
extern u32 qm_ocm_dq_mbox_addr;
extern u32 qm_ocm_fp_mbox_addr;
extern u32 qm_fabric_addr;
extern u32 qml_fabric_addr;
extern u32 maxqueues;

int apm_qm_wr32(int ip, u32 offset, u32 data);
int apm_qm_rd32(int ip, u32 offset, u32 *data);

void apm_qm_disable_mb_irq(u32 mb_id);

void apm_qm_enable_mb_irq(u32 mb_id);

int apm_qm_set_mb_affinity(u32 mb_id, u32 core_id);

int apm_qm_mb_tasklet_unregister(struct apm_qm_mb_tasklet *handler);

int apm_qm_mb_tasklet_register(u32 mailbox, u32 queue, u32 core, void *private,
		void (*func)(unsigned long));

int apm_qm_shutdown_q(int q_num);
int apm_qm_shutdown(void);

#endif /* __APM_QM_ACCESS_H__ */
