/*
 * Copyright (c) 2010-2011, Atheros Communications Inc.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 * * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#include <linux/autoconf.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/miscdevice.h>
#include <linux/watchdog.h>
#include <linux/slab.h>
#include <linux/ioport.h>
#include <linux/fcntl.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/system.h>
#include <asm/delay.h>
#include <asm/checksum.h>
#include <linux/notifier.h>
#include <linux/reboot.h>
#include <linux/init.h>
#include <linux/spinlock.h>
#include <linux/smp_lock.h>
#include <linux/wait.h>
#include <linux/irq.h>
#include <linux/fs.h>
#include <linux/interrupt.h>
#include <linux/dmapool.h>
#include <linux/compile.h>
#include "csum_hw.h"

//#define INTERRUPT_ENABLED 1

static const char driver_name [] = "csum";
static DECLARE_WAIT_QUEUE_HEAD(wq);

static struct dma_pool *dmapool = NULL;
extern csum_hw_ops *csum_hw;


static noinline int check_pkt_void(volatile struct descriptor *desc)
{
	if((desc->info.yy & CSUM_PKT_VOID) == 0)
		return 0;
	return 1;
}

int
do_defrag_hw(struct node_info *tx_entries, struct node_info *rx_entry, int num)
{
	int i;
	volatile struct descriptor *desc = NULL, *firstDesc = NULL;
	volatile struct descriptor *rx_desc = NULL;
        dma_addr_t rx_handle;
	dma_addr_t tx_handles[512];

	if(num > 512)
		return -EINVAL;

	if (rx_entry != NULL) {
        	rx_desc = (struct descriptor *)dma_pool_alloc(dmapool, \
						GFP_KERNEL, &rx_handle);
		rx_desc->buf = rx_entry->buf;
#ifdef INTERRUPT_ENABLED
		rx_desc->info.yy = (0x0 | CSUM_PKT_VOID | CSUM_INTR_ENABLE);
		ar7240_reg_rmw_set(CHKSUM_ACC_IMASK, CSUM_RX_INRT_MASK);
#else
		rx_desc->info.yy = (0x0 | CSUM_PKT_VOID);
#endif
		rx_desc->info.xx.pktSize = rx_entry->len;
		rx_desc->next = (struct descriptor *)rx_desc;
	}

	for(i=0; i<num; i++) {
		if(NULL == desc) {

		        desc = (struct descriptor *)dma_pool_alloc(dmapool, \
						GFP_KERNEL, &tx_handles[i]);
			firstDesc = desc;
			desc->info.yy = (0x0 | CSUM_TX_SOF_MASK);
		}
		else {
		        desc->next = (struct descriptor *)dma_pool_alloc(dmapool, \
						GFP_KERNEL, &tx_handles[i]);
			desc = desc->next;
			desc->info.yy = (0x0);
		}
		desc->buf = tx_entries[i].buf;
		if(rx_entry != NULL)
			desc->info.yy |= CSUM_OFFLOAD_TYPE_WITH_CP;
		desc->info.xx.pktSize = tx_entries[i].len;
	}
	desc->next = (struct descriptor *)firstDesc;
#ifdef INTERRUPT_ENABLED
	if(rx_entry == NULL) {
		desc->info.yy |= (CSUM_TX_EOF_MASK | CSUM_INTR_ENABLE);
		ar7240_reg_rmw_set(CHKSUM_ACC_IMASK, CSUM_TX_INRT_MASK);
	}
#else
	desc->info.yy |= CSUM_TX_EOF_MASK;
#endif
	if (rx_entry != NULL) {
    		ath_reg_wr(CHKSUM_ACC_DMARX_DESC, (unsigned int)rx_handle);
		ath_reg_wr(CHKSUM_ACC_DMARX_CONTROL, CHKSUM_ACC_DMARX_ENABLE);
	}
	ath_reg_wr(CHKSUM_ACC_DMATX_DESC0, (unsigned int)tx_handles[0]);
	ath_reg_wr(CHKSUM_ACC_DMATX_CONTROL0, CHKSUM_ACC_DMATX_ENABLE);

	if (rx_entry != NULL) {
#ifdef INTERRUPT_ENABLED
		wait_event_interruptible(wq, (check_pkt_void(rx_desc) == 0));
#else
	while ((check_pkt_void(rx_desc)) != 0);
#endif
		rx_entry->checksum = (rx_desc->info.xx.pktSize & 0xffff);
		rx_entry->actual = (rx_desc->status & 0x7ffff);
	}
#ifdef INTERRUPT_ENABLED
	else {
		wait_event_interruptible(wq, (check_pkt_void(desc) == 1));
	}
#endif
	desc = firstDesc;
	for(i=0; i<num; i++) {
#ifndef INTERRUPT_ENABLED
	while ((check_pkt_void(desc)) == 0);
#endif
		tx_entries[i].checksum = (desc->info.xx.pktSize & 0xffff);
		tx_entries[i].actual = (desc->status & 0x7ffff);
		if(tx_entries[i].actual < tx_entries[i].len)
			break;
		firstDesc = desc;
		desc = desc->next;
		dma_pool_free(dmapool, (void *)firstDesc, tx_handles[i]);
	}
	if (rx_entry != NULL) {
		dma_pool_free(dmapool, (void *)rx_desc, rx_handle);
	}
	return 0;
}

int
do_frag_hw(struct node_info *rx_entries, struct node_info *tx_entry, int num)
{
	int i;
	volatile struct descriptor *desc = NULL, *firstDesc = NULL;
	volatile struct descriptor *tx_desc = NULL;

        dma_addr_t tx_handle;
	dma_addr_t rx_handles[512];

        tx_desc = (struct descriptor *)dma_pool_alloc(dmapool, \
				GFP_KERNEL, &tx_handle);

	tx_desc->buf = tx_entry->buf;
	if (num != 0) {
		tx_desc->info.yy = (0x0 | CSUM_TX_SOF_MASK | CSUM_TX_EOF_MASK |
					  CSUM_OFFLOAD_TYPE_WITH_CP);
	} else {
#ifdef INTERRUPT_ENABLED
		tx_desc->info.yy = (0x0 | CSUM_TX_SOF_MASK | CSUM_TX_EOF_MASK |
					  CSUM_OFFLOAD_TYPE_CSUM_ONLY |
					  CSUM_INTR_ENABLE);
		ar7240_reg_rmw_set(CHKSUM_ACC_IMASK, CSUM_TX_INRT_MASK);
#else
		tx_desc->info.yy = (0x0 | CSUM_TX_SOF_MASK | CSUM_TX_EOF_MASK |
					  CSUM_OFFLOAD_TYPE_CSUM_ONLY);
#endif
	}

	tx_desc->info.xx.pktSize = tx_entry->len;
	tx_desc->next = (struct descriptor *)tx_desc;

	for(i=0; i<num; i++) {
		if(NULL == desc) {
		        desc = (struct descriptor *)dma_pool_alloc(dmapool, \
					GFP_KERNEL, &rx_handles[i]);
			firstDesc = desc;
		}
		else {
		        desc->next = (struct descriptor *)dma_pool_alloc(dmapool, GFP_KERNEL, &rx_handles[i]);
			desc = desc->next;
		}
		desc->buf = rx_entries[i].buf;
		desc->info.yy = (0x0 | CSUM_PKT_VOID);
		desc->info.xx.pktSize = rx_entries[i].len;
	}
	if (num != 0) {
		desc->next = (struct descriptor *)firstDesc;
#ifdef INTERRUPT_ENABLED
		ar7240_reg_rmw_set(CHKSUM_ACC_IMASK, CSUM_RX_INRT_MASK);
		desc->info.yy |= CSUM_INTR_ENABLE;
#endif
	    	ath_reg_wr(CHKSUM_ACC_DMARX_DESC, (unsigned int)rx_handles[0]);
		ath_reg_wr(CHKSUM_ACC_DMARX_CONTROL, CHKSUM_ACC_DMARX_ENABLE);
	}
	ath_reg_wr(CHKSUM_ACC_DMATX_DESC0, (unsigned int)tx_handle);
	ath_reg_wr(CHKSUM_ACC_DMATX_CONTROL0, CHKSUM_ACC_DMATX_ENABLE);
#ifdef INTERRUPT_ENABLED
	if(num == 0)
		wait_event_interruptible(wq, (check_pkt_void(tx_desc) == 1));
#else
	while ((check_pkt_void(tx_desc)) == 0);
#endif
	if (num != 0) {

#ifdef INTERRUPT_ENABLED
		wait_event_interruptible(wq, (check_pkt_void(desc) == 0));
#endif
		desc = firstDesc;
		for(i=0; i<num; i++) {
#ifndef INTERRUPT_ENABLED
			while ((check_pkt_void(desc)) != 0);
#endif
			rx_entries[i].checksum = (desc->info.xx.pktSize & 0xffff);
			rx_entries[i].actual = (desc->status & 0x7ffff);
#if 0
			if(rx_entries[i].actual < rx_entries[i].len) {
				retval = -1;
				break;
			}
#endif
			firstDesc = desc;
			desc = desc->next;
			dma_pool_free(dmapool, (void *)firstDesc, rx_handles[i]);
		}
	}
	tx_entry->checksum = (tx_desc->info.xx.pktSize & 0xffff);
	tx_entry->actual = (tx_desc->status & 0x7ffff);
	dma_pool_free(dmapool, (void *)tx_desc, tx_handle);
	return 0;
}

static __sum16 do_csum_hw(const void *buff, int len)
{
	unsigned int checksum;

	volatile struct descriptor *tx_desc = NULL;
	dma_addr_t handle;

	tx_desc = (struct descriptor *)dma_pool_alloc(dmapool, GFP_KERNEL, &handle);

	tx_desc->buf = (char *)buff;

#ifdef INTERRUPT_ENABLED
	ar7240_reg_rmw_set(CHKSUM_ACC_IMASK, CSUM_TX_INRT_MASK);
	tx_desc->info.yy = (0x0 | CSUM_TX_SOF_MASK | CSUM_TX_EOF_MASK | CSUM_INTR_ENABLE);
#else
	tx_desc->info.yy = (0x0 | CSUM_TX_SOF_MASK | CSUM_TX_EOF_MASK);
#endif
	tx_desc->info.xx.pktSize = len;
	tx_desc->next = (struct descriptor *)tx_desc;
	printk("tx_desc->info = %p; tx_desc->next = %p tx_desc->buf = %p\n\n", \
			&tx_desc->info, tx_desc->next, tx_desc->buf);
	ath_reg_wr(CHKSUM_ACC_DMATX_DESC0, (unsigned int)handle);
	ath_reg_wr(CHKSUM_ACC_DMATX_CONTROL0, CHKSUM_ACC_DMATX_ENABLE);

#ifdef INTERRUPT_ENABLED
	wait_event_interruptible(wq, (check_pkt_void(tx_desc) == 1));
#else
	while ((check_pkt_void(tx_desc)) == 0);
#endif
	checksum = (unsigned short)((tx_desc->info.xx.pktSize) & 0xffff);

	dma_pool_free(dmapool, (void *)tx_desc, handle);

        printk("%s: checksum = 0x%x\n", __func__, checksum);

	return checksum;
}

#ifdef INTERRUPT_ENABLED
irqreturn_t csum_hw_irq_handle(int irq,void *_csum,struct pt_regs *r)
{
	if(((ath_reg_rd(CHKSUM_ACC_DMARX_DESC_STATUS)) && CSUM_RX_DESC_INTR)) {
		ar7240_reg_rmw_set(CHKSUM_ACC_DMARX_DESC_STATUS, CSUM_RX_DESC_INTR);
	}else if(((ath_reg_rd(CHKSUM_ACC_DMATX_DESC_STATUS)) && CSUM_TX_DESC_INTR)) {
		ar7240_reg_rmw_set(CHKSUM_ACC_DMATX_DESC_STATUS, CSUM_TX_DESC_INTR);
	} else {
		return IRQ_NONE;
	}
	wake_up_interruptible(&wq);
	return IRQ_HANDLED;
}
#endif
int csum_hw_init(void)
{
#ifdef INTERRUPT_ENABLED
    /* Interrupt Request */
    if((request_irq(AR7240_MISC_IRQ_CSUM, csum_hw_irq_handle,
                    IRQF_DISABLED, driver_name, NULL)) != 0)
    {
        printk("request interrupt %lx failed \n", \
			AR7240_MISC_IRQ_CSUM);
	return -EBUSY;
    }
#endif

        if (!dmapool) {
                dmapool = dma_pool_create("csum_hw_accel", NULL, sizeof(struct descriptor), \
					(size_t)4, (size_t)SIZE_4K);

                if (!dmapool)
                        return -1;
        }

        if (!csum_hw) {
                csum_hw = (csum_hw_ops *)kmalloc(sizeof(csum_hw_ops),GFP_KERNEL);

                if (!csum_hw)
                        return -1;
        }

        rcu_assign_pointer(csum_hw->compute_csum_hw, do_csum_hw);
        printk("%s: Init done ...\n", __func__);

	return 0;
}

void csum_hw_exit(void)
{
        rcu_assign_pointer(csum_hw->compute_csum_hw, NULL);
        kfree(csum_hw);
        csum_hw = NULL;
	dma_pool_destroy(dmapool);
#ifdef INTERRUPT_ENABLED
	free_irq(AR7240_MISC_IRQ_CSUM, NULL);
#endif
}

#if 0
module_init(csum_hw_init);
module_exit(csum_hw_exit);

MODULE_AUTHOR("Atheros Communications, Inc.");
MODULE_DESCRIPTION("Support for HW Accelerated checksum");

#ifdef MODULE_LICENSE
MODULE_LICENSE("Atheros");
#endif
#endif
