/*
 * Copyright(c) 2010 Applied Micro Circuits Corporation(AMCC). All rights reserved.
 *
 * Author: Tai Tri Nguyen <ttnguyen@appliedmicro.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59
 * Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * The full GNU General Public License is included in this distribution in the
 * file called COPYING.
 */

/*
 * This driver supports the asynchrounous DMA copy and RAID engines available
 * on the AppliedMicro APM82181 Processor.
 * Based on the Intel Xscale(R) family of I/O Processors (IOP 32x, 33x, 134x)
 * ADMA driver written by D.Williams.
 */
#define ADMA_DEBUG
#undef ADMA_DEBUG

#include <linux/init.h>
#include <linux/module.h>
#include <linux/async_tx.h>
#include <linux/delay.h>
#include <linux/dma-mapping.h>
#include <linux/spinlock.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/uaccess.h>
#include <linux/of_platform.h>
#include <linux/proc_fs.h>
#include <asm/dcr.h>
#include <asm/dcr-regs.h>
#include <asm/apm82181-adma.h>

#define PPC4XX_EDMA "apm82181-adma: "
#ifdef ADMA_DEBUG
#define DBG(string, args...) \
        printk(PPC4XX_EDMA string ,##args)
#define INFO DBG("<%s> -- line %d\n",__func__,__LINE__);
#define ADMA_HEXDUMP(b, l)                                              \
  	print_hex_dump(KERN_CONT, "", DUMP_PREFIX_OFFSET,                     \
                 16, 1, (b), (l), false);
#else
#define DBG(string, args...) \
        {if (0) printk(KERN_INFO PPC4XX_EDMA string ,##args); 0; }
#define INFO DBG("");
#define ADMA_HEXDUMP(b, l)                                              \
        {if (0) print_hex_dump(KERN_CONT, "", DUMP_PREFIX_OFFSET,       \
                 8, 1, (b), (l), false); 0;}
#endif

#define MEM_HEXDUMP(b, l)                                              \
        print_hex_dump(KERN_CONT, "", DUMP_PREFIX_OFFSET,                     \
                 16, 1, (b), (l), false);

/* The list of channels exported by apm82181 ADMA */
struct list_head
ppc_adma_chan_list = LIST_HEAD_INIT(ppc_adma_chan_list);

/* This flag is set when want to refetch the xor chain in the interrupt
 * handler
 */
static u32 do_xor_refetch = 0;

/* Pointers to last submitted to DMA0/1/2/3 and XOR CDBs */
static apm82181_desc_t *chan_last_sub[5];
static apm82181_desc_t *chan_first_cdb[5];

/* Pointer to last linked and submitted xor CB */
static apm82181_desc_t *xor_last_linked = NULL;
static apm82181_desc_t *xor_last_submit = NULL;

/* /proc interface is used here to verify the h/w RAID 5 capabilities
 */
static struct proc_dir_entry *apm82181_proot;

/* These are used in enable & check routines
 */
static u32 apm82181_xor_verified;
static u32 apm82181_memcpy_verified[4];
static apm82181_ch_t *apm82181_dma_tchan[5];
static struct completion apm82181_r5_test_comp;

static inline int apm82181_chan_is_busy(apm82181_ch_t *chan);
#if 0
static phys_addr_t fixup_bigphys_addr(phys_addr_t addr, phys_addr_t size)
{
	phys_addr_t page_4gb = 0;
	 
	return (page_4gb | addr);
}
#endif
/**
 * apm82181_adma_device_estimate - estimate the efficiency of processing
 *      the operation given on this channel. It's assumed that 'chan' is
 *      capable to process 'cap' type of operation.
 * @chan: channel to use
 * @cap: type of transaction
 * @src_lst: array of source pointers
 * @src_cnt: number of source operands
 * @src_sz: size of each source operand
 */
int apm82181_adma_estimate (struct dma_chan *chan,
        enum dma_transaction_type cap, struct page **src_lst,
        int src_cnt, size_t src_sz)
{
        int ef = 1;

        /* channel idleness increases the priority */
        if (likely(ef) &&
            !apm82181_chan_is_busy(to_apm82181_adma_chan(chan)))
                ef++;
        else { 
		if(chan->chan_id !=APM82181_XOR_ID)
                        ef = -1;
	}
        return ef;
}

/******************************************************************************
 * Command (Descriptor) Blocks low-level routines
 ******************************************************************************/
/**
 * apm82181_desc_init_interrupt - initialize the descriptor for INTERRUPT
 * pseudo operation
 */
static inline void apm82181_desc_init_interrupt (apm82181_desc_t *desc,
							apm82181_ch_t *chan)
{
	xor_cb_t *p;
		
	switch (chan->device->id) {
	case APM82181_PDMA0_ID:
	case APM82181_PDMA1_ID:
	case APM82181_PDMA2_ID:
	case APM82181_PDMA3_ID:
		BUG();
		break;
	case APM82181_XOR_ID:
		p = desc->hw_desc;
                memset (desc->hw_desc, 0, sizeof(xor_cb_t));
                /* NOP with Command Block Complete Enable */
                p->cbc = XOR_CBCR_CBCE_BIT;
                break;
	default:
		printk(KERN_ERR "Unsupported id %d in %s\n", chan->device->id,
				__FUNCTION__);
		break;
	}
}

/**
 * apm82181_desc_init_xor - initialize the descriptor for XOR operation
 */
static inline void apm82181_desc_init_xor(apm82181_desc_t *desc, int src_cnt,
                unsigned long flags)
{
        xor_cb_t *hw_desc = desc->hw_desc;

        memset (desc->hw_desc, 0, sizeof(xor_cb_t));
        desc->hw_next = NULL;
        desc->src_cnt = src_cnt;
        desc->dst_cnt = 1;

        hw_desc->cbc = XOR_CBCR_TGT_BIT | src_cnt;
        if (flags & DMA_PREP_INTERRUPT)
                /* Enable interrupt on complete */
                hw_desc->cbc |= XOR_CBCR_CBCE_BIT;
}

/**
 * apm82181_desc_init_memcpy - initialize the descriptor for MEMCPY operation
 */
static inline void apm82181_desc_init_memcpy(apm82181_desc_t *desc,
		unsigned long flags)
{
	dma_cdb_t *hw_desc = desc->hw_desc;

	memset(hw_desc, 0, sizeof(dma_cdb_t));
	desc->hw_next = NULL;
	desc->src_cnt = 1;
	desc->dst_cnt = 1;
	
	if (flags & DMA_PREP_INTERRUPT)
		set_bit(APM82181_DESC_INT, &desc->flags);
	else
		clear_bit(APM82181_DESC_INT, &desc->flags);
	/* dma configuration for running */
	hw_desc->ctrl.tm = 2; /* soft init mem-mem mode */
	hw_desc->ctrl.pw = 4; /* transfer width 128 bytes */
	hw_desc->ctrl.ben = 1;/* buffer enable */
	hw_desc->ctrl.sai = 1;/* increase source addr */
	hw_desc->ctrl.dai = 1;/* increase dest addr */
	hw_desc->ctrl.tce = 1;/* chan stops when TC is reached */
	hw_desc->ctrl.cp = 3; /* hinghest priority */
	hw_desc->ctrl.sl = 0; /* source is in PLB */
	hw_desc->ctrl.pl = 0; /* dest is in PLB */
	hw_desc->cnt.tcie = 0;/* no interrupt on init */
	hw_desc->cnt.etie = 0; /* enable error interrupt */
	hw_desc->cnt.eie = 1; /* enable error interrupt */
	hw_desc->cnt.link = 0;/* not link to next cdb */
	hw_desc->cnt.sgl = 0;
	hw_desc->ctrl.ce =1;  /* enable channel */
	hw_desc->ctrl.cie =1; /* enable int channel */
}

/**
 * apm82181_desc_init_memset - initialize the descriptor for MEMSET operation
 */
static inline void apm82181_desc_init_memset(apm82181_desc_t *desc, int value,
		unsigned long flags)
{
	//dma_cdb_t *hw_desc = desc->hw_desc;

	memset (desc->hw_desc, 0, sizeof(dma_cdb_t));
	desc->hw_next = NULL;
	desc->src_cnt = 1;
	desc->dst_cnt = 1;

	if (flags & DMA_PREP_INTERRUPT)
		set_bit(APM82181_DESC_INT, &desc->flags);
	else
		clear_bit(APM82181_DESC_INT, &desc->flags);

}



/**
 * apm82181_desc_set_src_addr - set source address into the descriptor
 */
static inline void apm82181_desc_set_src_addr( apm82181_desc_t *desc,
				apm82181_ch_t *chan, int src_idx, dma_addr_t addr)
{
	dma_cdb_t *dma_hw_desc;
	xor_cb_t *xor_hw_desc;

	switch (chan->device->id) {
	case APM82181_PDMA0_ID:
	case APM82181_PDMA1_ID:
	case APM82181_PDMA2_ID:
	case APM82181_PDMA3_ID:
		dma_hw_desc = desc->hw_desc;
		dma_hw_desc->src_hi = (u32)(addr >> 32);
		dma_hw_desc->src_lo = (u32)addr;
		break;
	case APM82181_XOR_ID:
		xor_hw_desc = desc->hw_desc;
                xor_hw_desc->ops[src_idx].h = (u32)(addr >>32);
                xor_hw_desc->ops[src_idx].l = (u32)addr;
                break;
	}
}

static void apm82181_adma_set_src(apm82181_desc_t *sw_desc,
                dma_addr_t addr, int index)
{
	apm82181_ch_t *chan = to_apm82181_adma_chan(sw_desc->async_tx.chan);

        sw_desc = sw_desc->group_head;

        if (likely(sw_desc))
                apm82181_desc_set_src_addr(sw_desc, chan, index, addr);
}

/**
 * apm82181_desc_set_dest_addr - set destination address into the descriptor
 */
static inline void apm82181_desc_set_dest_addr(apm82181_desc_t *desc,
			apm82181_ch_t *chan, dma_addr_t addr, u32 index)
{
	dma_cdb_t *dma_hw_desc;
	xor_cb_t *xor_hw_desc;

	switch (chan->device->id) {
        case APM82181_PDMA0_ID:
        case APM82181_PDMA1_ID:
        case APM82181_PDMA2_ID:
        case APM82181_PDMA3_ID:	
		dma_hw_desc = desc->hw_desc;
		dma_hw_desc->dest_hi = (u32)(addr >> 32);
		dma_hw_desc->dest_lo = (u32)addr;
		break;
	case APM82181_XOR_ID:
		xor_hw_desc = desc->hw_desc;
                xor_hw_desc->cbtah = (u32)(addr >> 32);
                xor_hw_desc->cbtal |= (u32)addr;
                break;
        }
}

static int plbdma_get_transfer_width(dma_cdb_t *dma_hw_desc)
{
	switch (dma_hw_desc->ctrl.pw){
                case 0:
                        return 1; /* unit: bytes */
                case 1:
                        return 2;
                case 2:
                        return 4;
                case 3:
                        return 8;
                case 4:
                        return 16;
        }
	return 0;
}
/**
 * apm82181_desc_set_byte_count - set number of data bytes involved
 * into the operation
 */
static inline void apm82181_desc_set_byte_count(apm82181_desc_t *desc,
					apm82181_ch_t *chan, size_t byte_count)
{
	dma_cdb_t *dma_hw_desc;
	xor_cb_t *xor_hw_desc;
	int terminal_cnt, transfer_width = 0;
	
	DBG("<%s> byte_count %08x\n", __func__,byte_count);
	switch (chan->device->id){
	case APM82181_PDMA0_ID:
	case APM82181_PDMA1_ID:
	case APM82181_PDMA2_ID:
	case APM82181_PDMA3_ID:
		dma_hw_desc = desc->hw_desc;	
		transfer_width = plbdma_get_transfer_width(dma_hw_desc);
		terminal_cnt = byte_count/transfer_width;
		dma_hw_desc->cnt.tc = terminal_cnt;
		break;
	case APM82181_XOR_ID:
		xor_hw_desc = desc->hw_desc;
                xor_hw_desc->cbbc = byte_count;
		break;
	}
}

/**
 * apm82181_xor_set_link - set link address in xor CB
 */
static inline void apm82181_xor_set_link (apm82181_desc_t *prev_desc,
                                                apm82181_desc_t *next_desc)
{
        xor_cb_t *xor_hw_desc = prev_desc->hw_desc;

        if (unlikely(!next_desc || !(next_desc->phys))) {
                printk(KERN_ERR "%s: next_desc=0x%p; next_desc->phys=0x%llx\n",
                        __func__, next_desc,
                        next_desc ? next_desc->phys : 0);
                BUG();
        }
	DBG("<%s>:next_desc->phys %llx\n", __func__,next_desc->phys);
        xor_hw_desc->cbs = 0;
        xor_hw_desc->cblal = (u32)next_desc->phys;
        xor_hw_desc->cblah = (u32)(next_desc->phys >> 32);
        xor_hw_desc->cbc |= XOR_CBCR_LNK_BIT;
}

/**
 * apm82181_desc_set_link - set the address of descriptor following this
 * descriptor in chain
 */
static inline void apm82181_desc_set_link(apm82181_ch_t *chan,
		apm82181_desc_t *prev_desc, apm82181_desc_t *next_desc)
{
	unsigned long flags;
	apm82181_desc_t *tail = next_desc;

	if (unlikely(!prev_desc || !next_desc ||
		(prev_desc->hw_next && prev_desc->hw_next != next_desc))) {
		/* If previous next is overwritten something is wrong.
		 * though we may refetch from append to initiate list
		 * processing; in this case - it's ok.
		 */
		printk(KERN_ERR "%s: prev_desc=0x%p; next_desc=0x%p; "
			"prev->hw_next=0x%p\n", __FUNCTION__, prev_desc,
			next_desc, prev_desc ? prev_desc->hw_next : 0);
		BUG();
	}

	local_irq_save(flags);

	/* do s/w chaining both for DMA and XOR descriptors */
	prev_desc->hw_next = next_desc;

	switch (chan->device->id) {
	case APM82181_PDMA0_ID:
	case APM82181_PDMA1_ID:
	case APM82181_PDMA2_ID:
	case APM82181_PDMA3_ID:
		break;
	 case APM82181_XOR_ID:
                /* bind descriptor to the chain */
                while (tail->hw_next)
                        tail = tail->hw_next;
                xor_last_linked = tail;

                if (prev_desc == xor_last_submit)
                        /* do not link to the last submitted CB */
                        break;
                apm82181_xor_set_link (prev_desc, next_desc);
                break;
	default:
		BUG();
	}

	local_irq_restore(flags);
}

/**
 * apm82181_desc_get_src_addr - extract the source address from the descriptor
 */
static inline u32 apm82181_desc_get_src_addr(apm82181_desc_t *desc,
					apm82181_ch_t *chan, int src_idx)
{
	dma_cdb_t *dma_hw_desc;
		
	dma_hw_desc = desc->hw_desc;

	switch (chan->device->id) {
	case APM82181_PDMA0_ID:
	case APM82181_PDMA1_ID:
	case APM82181_PDMA2_ID:
	case APM82181_PDMA3_ID:
		break;
	default:
		return 0;
	}
		/* May have 0, 1, 2, or 3 sources */
		return (dma_hw_desc->src_lo); 
}

/**
 * apm82181_desc_get_dest_addr - extract the destination address from the
 * descriptor
 */
static inline u32 apm82181_desc_get_dest_addr(apm82181_desc_t *desc,
		apm82181_ch_t *chan, int idx)
{
	dma_cdb_t *dma_hw_desc;
		
	dma_hw_desc = desc->hw_desc;

	switch (chan->device->id) {
	case APM82181_PDMA0_ID:
	case APM82181_PDMA1_ID:
	case APM82181_PDMA2_ID:
	case APM82181_PDMA3_ID:
		break;
	default:
		return 0;
	}
		
	/* May have 0, 1, 2, or 3 sources */
	return (dma_hw_desc->dest_lo); 
}

/**
 * apm82181_desc_get_byte_count - extract the byte count from the descriptor
 */
static inline u32 apm82181_desc_get_byte_count(apm82181_desc_t *desc,
		apm82181_ch_t *chan)
{
	dma_cdb_t *dma_hw_desc;
		
	dma_hw_desc = desc->hw_desc;

	switch (chan->device->id) {
	case APM82181_PDMA0_ID:
	case APM82181_PDMA1_ID:
	case APM82181_PDMA2_ID:
	case APM82181_PDMA3_ID:
		break;
	default:
		return 0;
	}
		/* May have 0, 1, 2, or 3 sources */
		//return (dma_hw_desc->cnt); 
}


/**
 * apm82181_desc_get_link - get the address of the descriptor that
 * follows this one
 */
static inline u32 apm82181_desc_get_link(apm82181_desc_t *desc,
		apm82181_ch_t *chan)
{
	if (!desc->hw_next)
		return 0;

	return desc->hw_next->phys;
}

/**
 * apm82181_desc_is_aligned - check alignment
 */
static inline int apm82181_desc_is_aligned(apm82181_desc_t *desc,
		int num_slots)
{
	return (desc->idx & (num_slots - 1)) ? 0 : 1;
}



/******************************************************************************
 * ADMA channel low-level routines
 ******************************************************************************/

static inline phys_addr_t apm82181_chan_get_current_descriptor(apm82181_ch_t *chan);
static inline void apm82181_chan_append(apm82181_ch_t *chan);

/*
 * apm82181_adma_device_clear_eot_status - interrupt ack to XOR or DMA engine
 */
static inline void apm82181_adma_device_clear_eot_status (apm82181_ch_t *chan)
{
	u32 val ;
	int idx = chan->device->id;
	volatile xor_regs_t *xor_reg;
	INFO;
	switch (idx) {
	case APM82181_PDMA0_ID:
	case APM82181_PDMA1_ID:
	case APM82181_PDMA2_ID:
	case APM82181_PDMA3_ID:
		val = mfdcr(DCR_DMA2P40_SR);
		if(val & DMA_SR_RI(idx)){
			printk(KERN_ERR "Err occurred, DMA%d status: 0x%x\n", idx, val);
		}
		/* TC reached int, write back to clear */
		mtdcr(DCR_DMA2P40_SR, val);
		break;
	case APM82181_XOR_ID:
		/* reset status bits to ack*/
		xor_reg = chan->device->xor_base;

                val = xor_reg->sr;
		DBG("XOR engine status: 0x%08x\n", val);
                xor_reg->sr = val;

                if (val & (XOR_IE_ICBIE_BIT|XOR_IE_ICIE_BIT|XOR_IE_RPTIE_BIT)) {
                        if (val & XOR_IE_RPTIE_BIT) {
                                /* Read PLB Timeout Error.
                                 * Try to resubmit the CB
                                 */
				INFO;
                                xor_reg->cblalr = xor_reg->ccbalr;
                                xor_reg->crsr |= XOR_CRSR_XAE_BIT;
                        } else
                                printk (KERN_ERR "XOR ERR 0x%x status\n", val);
                        break;
                }

                /*  if the XORcore is idle, but there are unprocessed CBs
                 * then refetch the s/w chain here
                 */
                if (!(xor_reg->sr & XOR_SR_XCP_BIT) && do_xor_refetch) {
                        apm82181_chan_append(chan);
                }
                break;
        }
}

/*
 * apm82181_chan_is_busy - get the channel status
 */

static inline int apm82181_chan_is_busy(apm82181_ch_t *chan)
{
	int busy = 0;
	volatile xor_regs_t *xor_reg = chan->device->xor_base;

	switch (chan->device->id) {
	case APM82181_PDMA0_ID:
	case APM82181_PDMA1_ID:
	case APM82181_PDMA2_ID:
	case APM82181_PDMA3_ID:
		if(mfdcr(DCR_DMA2P40_SR) & DMA_SR_CB(chan->device->id))
                	busy = 1;
        	else
                	busy = 0;
		break;
	case APM82181_XOR_ID:
		/* use the special status bit for the XORcore
                 */
                busy = (xor_reg->sr & XOR_SR_XCP_BIT) ? 1 : 0;
                break;
	default:
		BUG();
	}

	return busy;
}

/**
 * apm82181_dma_put_desc - put PLB DMA 0/1/2/3 descriptor to FIFO
 */
static inline void apm82181_dma_put_desc(apm82181_ch_t *chan,
		apm82181_desc_t *desc)
{
	dma_cdb_t *cdb = desc->hw_desc;
	u32 sg_cmd = 0;

	/* Enable TC interrupt */
	if(test_bit(APM82181_DESC_INT, &desc->flags))
		cdb->cnt.tcie = 1;	
	else
		cdb->cnt.tcie = 0;
	/* Not link to next cdb */
	cdb->sg_hi = 0xffffffff;
	cdb->sg_lo = 0xffffffff;

	chan_last_sub[chan->device->id] = desc;

	/* Update new cdb addr */
	mtdcr(DCR_DMA2P40_SGHx(chan->device->id), (u32)(desc->phys >> 32));
	mtdcr(DCR_DMA2P40_SGLx(chan->device->id), (u32)desc->phys);

	INFO;
	DBG("slot id: %d addr: %llx\n", desc->idx, desc->phys);
	DBG("S/G addr H: %08x addr L: %08x\n",
		mfdcr(DCR_DMA2P40_SGHx(chan->device->id)),
		mfdcr(DCR_DMA2P40_SGLx(chan->device->id)));
	ADMA_HEXDUMP(cdb, 96);
	/* Enable S/G */
	sg_cmd |= (DMA_SGC_SSG(chan->device->id) | DMA_SGC_EM_ALL);
	sg_cmd |= DMA_SGC_SGL(chan->device->id, 0); /* S/G addr in PLB */
		
	mtdcr(DCR_DMA2P40_SGC, sg_cmd);
	DBG("S/G addr H: %08x addr L: %08x\n",
		mfdcr(DCR_DMA2P40_SGHx(chan->device->id)),
		mfdcr(DCR_DMA2P40_SGLx(chan->device->id)));
	/* need to use variable for logging current CDB */
	chan->current_cdb_addr = desc->phys;

}

/**
 * apm82181_chan_append - update the h/w chain in the channel
 */
static inline void apm82181_chan_append(apm82181_ch_t *chan)
{
	apm82181_desc_t *iter;
	volatile xor_regs_t *xor_reg;
	phys_addr_t cur_desc;
	xor_cb_t *xcb;
	unsigned long flags;
	INFO;

	local_irq_save(flags);

	switch (chan->device->id) {
	case APM82181_PDMA0_ID:
	case APM82181_PDMA1_ID:
	case APM82181_PDMA2_ID:
	case APM82181_PDMA3_ID:
		cur_desc = apm82181_chan_get_current_descriptor(chan);
		DBG("current_desc %llx\n", cur_desc);
		if (likely(cur_desc)) {
			INFO;
			iter = chan_last_sub[chan->device->id];
			BUG_ON(!iter);
		} else {
			INFO;
			/* first peer */
			iter = chan_first_cdb[chan->device->id];
			BUG_ON(!iter);
			INFO;
			apm82181_dma_put_desc(chan, iter);
			chan->hw_chain_inited = 1;
		}

		/* is there something new to append */
		if (!iter->hw_next)
			break;

		/* flush descriptors from the s/w queue to fifo */
		list_for_each_entry_continue(iter, &chan->chain, chain_node) {
			apm82181_dma_put_desc(chan, iter);
			if (!iter->hw_next)
				break;
		}
		break;
	case APM82181_XOR_ID:
		/* update h/w links and refetch */
                if (!xor_last_submit->hw_next)
                        break;
		xor_reg = chan->device->xor_base;
		/* the last linked CDB has to generate an interrupt
                 * that we'd be able to append the next lists to h/w
                 * regardless of the XOR engine state at the moment of
                 * appending of these next lists
                 */
                xcb = xor_last_linked->hw_desc;
                xcb->cbc |= XOR_CBCR_CBCE_BIT;

                if (!(xor_reg->sr & XOR_SR_XCP_BIT)) {
                        /* XORcore is idle. Refetch now */
                        do_xor_refetch = 0;
                        apm82181_xor_set_link(xor_last_submit,
                                xor_last_submit->hw_next);

                        xor_last_submit = xor_last_linked;
                        xor_reg->crsr |= XOR_CRSR_RCBE_BIT | XOR_CRSR_64BA_BIT;
                } else {
                        /* XORcore is running. Refetch later in the handler */
                        do_xor_refetch = 1;
                }

		break;
	}

	local_irq_restore(flags);
}

/**
 * apm82181_chan_get_current_descriptor - get the currently executed descriptor
 */
static inline phys_addr_t apm82181_chan_get_current_descriptor(apm82181_ch_t *chan)
{
	phys_addr_t curr_cdb_addr;
	volatile xor_regs_t *xor_reg;
	int idx = chan->device->id;

	if (unlikely(!chan->hw_chain_inited))
		/* h/w descriptor chain is not initialized yet */
		return 0;
	switch(idx){
	case APM82181_PDMA0_ID:
	case APM82181_PDMA1_ID:
	case APM82181_PDMA2_ID:
	case APM82181_PDMA3_ID:
		curr_cdb_addr = chan->current_cdb_addr;	
		break;
	case APM82181_XOR_ID:
		xor_reg = chan->device->xor_base;
		curr_cdb_addr = (dma_addr_t)xor_reg->ccbahr; 
		curr_cdb_addr = (curr_cdb_addr << 32) | xor_reg->ccbalr;
		break;	
	default:
		BUG();
	}
	return curr_cdb_addr; 
}


/******************************************************************************
 * ADMA device level
 ******************************************************************************/

static int apm82181_adma_alloc_chan_resources(struct dma_chan *chan);
static dma_cookie_t apm82181_adma_tx_submit(
		struct dma_async_tx_descriptor *tx);

static void apm82181_adma_set_dest(
		apm82181_desc_t *tx,
		dma_addr_t addr, int index);

/**
 * apm82181_get_group_entry - get group entry with index idx
 * @tdesc: is the last allocated slot in the group.
 */
static inline apm82181_desc_t *
apm82181_get_group_entry ( apm82181_desc_t *tdesc, u32 entry_idx)
{
	apm82181_desc_t *iter = tdesc->group_head;
	int i = 0;

	if (entry_idx < 0 || entry_idx >= (tdesc->src_cnt + tdesc->dst_cnt)) {
		printk("%s: entry_idx %d, src_cnt %d, dst_cnt %d\n",
				__func__, entry_idx, tdesc->src_cnt, tdesc->dst_cnt);
		BUG();
	}
	list_for_each_entry(iter, &tdesc->group_list, chain_node) {
		if (i++ == entry_idx)
			break;
	}
	return iter;
}

/**
 * apm82181_adma_free_slots - flags descriptor slots for reuse
 * @slot: Slot to free
 * Caller must hold &apm82181_chan->lock while calling this function
 */
static void apm82181_adma_free_slots(apm82181_desc_t *slot,
		apm82181_ch_t *chan)
{
	int stride = slot->slots_per_op;

	while (stride--) {
		/*async_tx_clear_ack(&slot->async_tx);*/ /* Don't need to clear. It is hack*/
		slot->slots_per_op = 0;
		slot = list_entry(slot->slot_node.next,
				apm82181_desc_t,
				slot_node);
	}
}

static void 
apm82181_adma_unmap(apm82181_ch_t *chan, apm82181_desc_t *desc)
{
	u32 src_cnt, dst_cnt;
	dma_addr_t addr;
	/*
	 * get the number of sources & destination
	 * included in this descriptor and unmap
	 * them all
	 */
	src_cnt = 1; 
	dst_cnt = 1; 

	/* unmap destinations */
	if (!(desc->async_tx.flags & DMA_COMPL_SKIP_DEST_UNMAP)) {
		while (dst_cnt--) {
			addr = apm82181_desc_get_dest_addr(
				desc, chan, dst_cnt);
			dma_unmap_page(&chan->device->ofdev->dev,
					addr, desc->unmap_len,
					DMA_FROM_DEVICE);
		}
	}

	/* unmap sources */
	if (!(desc->async_tx.flags & DMA_COMPL_SKIP_SRC_UNMAP)) {
		while (src_cnt--) {
			addr = apm82181_desc_get_src_addr(
				desc, chan, src_cnt);
			dma_unmap_page(&chan->device->ofdev->dev,
					addr, desc->unmap_len,
					DMA_TO_DEVICE);
		}
	}

}
/**
 * apm82181_adma_run_tx_complete_actions - call functions to be called
 * upon complete
 */
static dma_cookie_t apm82181_adma_run_tx_complete_actions(
		apm82181_desc_t *desc,
		apm82181_ch_t *chan,
		dma_cookie_t cookie)
{
	int i;
	//enum dma_data_direction dir;
	INFO;
	BUG_ON(desc->async_tx.cookie < 0);
	if (desc->async_tx.cookie > 0) {
		cookie = desc->async_tx.cookie;
		desc->async_tx.cookie = 0;

		/* call the callback (must not sleep or submit new
		 * operations to this channel)
		 */
		if (desc->async_tx.callback)
			desc->async_tx.callback(
				desc->async_tx.callback_param);

		/* unmap dma addresses
		 * (unmap_single vs unmap_page?)
		 *
		 * actually, ppc's dma_unmap_page() functions are empty, so
		 * the following code is just for the sake of completeness
		 */
		if (chan && chan->needs_unmap && desc->group_head &&
		     desc->unmap_len) {
			apm82181_desc_t *unmap = desc->group_head;
			/* assume 1 slot per op always */
			u32 slot_count = unmap->slot_cnt;

			/* Run through the group list and unmap addresses */
			for (i = 0; i < slot_count; i++) {
				BUG_ON(!unmap);
				apm82181_adma_unmap(chan, unmap);
				unmap = unmap->hw_next;
			}
			desc->group_head = NULL;
		}
	}

	/* run dependent operations */
	dma_run_dependencies(&desc->async_tx);

	return cookie;
}

/**
 * apm82181_adma_clean_slot - clean up CDB slot (if ack is set)
 */
static int apm82181_adma_clean_slot(apm82181_desc_t *desc,
		apm82181_ch_t *chan)
{
	/* the client is allowed to attach dependent operations
	 * until 'ack' is set
	 */
	if (!async_tx_test_ack(&desc->async_tx))
		return 0;

	/* leave the last descriptor in the chain
	 * so we can append to it
	 */
	if (list_is_last(&desc->chain_node, &chan->chain) ||
	    desc->phys == apm82181_chan_get_current_descriptor(chan))
		return 1;

	dev_dbg(chan->device->common.dev, "\tfree slot %llx: %d stride: %d\n",
		desc->phys, desc->idx, desc->slots_per_op);

	list_del(&desc->chain_node);
	apm82181_adma_free_slots(desc, chan);
	return 0;
}

/**
 *      __apm82181_adma_slot_cleanup - this is the common clean-up routine
 *	which runs through the channel CDBs list until reach the descriptor
 *	currently processed. When routine determines that all CDBs of group
 *	are completed then corresponding callbacks (if any) are called and slots
 *	are freed.
 */
static void __apm82181_adma_slot_cleanup(apm82181_ch_t *chan)
{
	apm82181_desc_t *iter, *_iter, *group_start = NULL;
	dma_cookie_t cookie = 0;
	phys_addr_t current_desc = apm82181_chan_get_current_descriptor(chan);
	int busy = apm82181_chan_is_busy(chan);
	int seen_current = 0, slot_cnt = 0, slots_per_op = 0;

	DBG("apm82181 adma%d: %s\n",
		chan->device->id, __FUNCTION__);
	DBG("current_desc %llx\n", current_desc);

	if (!current_desc) {
		/*  There were no transactions yet, so
		 * nothing to clean
		 */
		return;
	}

	/* free completed slots from the chain starting with
	 * the oldest descriptor
	 */
	list_for_each_entry_safe(iter, _iter, &chan->chain,
					chain_node) {
		DBG(" cookie: %d slot: %d "
		    "busy: %d this_desc: %llx next_desc: %x cur: %llx ack: %d\n",
			iter->async_tx.cookie, iter->idx, busy, iter->phys,
		    	apm82181_desc_get_link(iter, chan), current_desc,
			async_tx_test_ack(&iter->async_tx));
		prefetch(_iter);
		prefetch(&_iter->async_tx);

		/* do not advance past the current descriptor loaded into the
		 * hardware channel,subsequent descriptors are either in process
		 * or have not been submitted
		 */
		if (seen_current)
			break;

		/* stop the search if we reach the current descriptor and the
		 * channel is busy, or if it appears that the current descriptor
		 * needs to be re-read (i.e. has been appended to)
		 */
		if (iter->phys == current_desc) {
			BUG_ON(seen_current++);
			if (busy || apm82181_desc_get_link(iter, chan)) {
				/* not all descriptors of the group have
				 * been completed; exit.
				 */
				break;
			}
		}

		/* detect the start of a group transaction */
		if (!slot_cnt && !slots_per_op) {
			slot_cnt = iter->slot_cnt;
			slots_per_op = iter->slots_per_op;
			if (slot_cnt <= slots_per_op) {
				slot_cnt = 0;
				slots_per_op = 0;
			}
		}

		if (slot_cnt) {
			if (!group_start)
				group_start = iter;
			slot_cnt -= slots_per_op;
		}

		/* all the members of a group are complete */
		if (slots_per_op != 0 && slot_cnt == 0) {
			apm82181_desc_t *grp_iter, *_grp_iter;
			int end_of_chain = 0;

			/* clean up the group */
			slot_cnt = group_start->slot_cnt;
			grp_iter = group_start;
			list_for_each_entry_safe_from(grp_iter, _grp_iter,
				&chan->chain, chain_node) {

				cookie = apm82181_adma_run_tx_complete_actions(
					grp_iter, chan, cookie);

				slot_cnt -= slots_per_op;
				end_of_chain = apm82181_adma_clean_slot(
				    grp_iter, chan);
				if (end_of_chain && slot_cnt) {
					/* Should wait for ZeroSum complete */
					if (cookie > 0)
						chan->completed_cookie = cookie;
					return;
				}

				if (slot_cnt == 0 || end_of_chain)
					break;
			}

			/* the group should be complete at this point */
			BUG_ON(slot_cnt);

			slots_per_op = 0;
			group_start = NULL;
			if (end_of_chain)
				break;
			else
				continue;
		} else if (slots_per_op) /* wait for group completion */
			continue;

		cookie = apm82181_adma_run_tx_complete_actions(iter, chan,
		    cookie);

		if (apm82181_adma_clean_slot(iter, chan))
			break;
	}

	BUG_ON(!seen_current);

	if (cookie > 0) {
		chan->completed_cookie = cookie;
		DBG("completed cookie %d\n", cookie);
	}

}

/**
 * apm82181_adma_tasklet - clean up watch-dog initiator
 */
static void apm82181_adma_tasklet (unsigned long data)
{
	apm82181_ch_t *chan = (apm82181_ch_t *) data;
	spin_lock(&chan->lock);
	INFO;
	__apm82181_adma_slot_cleanup(chan);
	spin_unlock(&chan->lock);
}

/**
 * apm82181_adma_slot_cleanup - clean up scheduled initiator
 */
static void apm82181_adma_slot_cleanup (apm82181_ch_t *chan)
{
	spin_lock_bh(&chan->lock);
	__apm82181_adma_slot_cleanup(chan);
	spin_unlock_bh(&chan->lock);
}

/**
 * apm82181_adma_alloc_slots - allocate free slots (if any)
 */
static apm82181_desc_t *apm82181_adma_alloc_slots(
		apm82181_ch_t *chan, int num_slots,
		int slots_per_op)
{
	apm82181_desc_t *iter = NULL, *_iter, *alloc_start = NULL;
	struct list_head chain = LIST_HEAD_INIT(chain);
	int slots_found, retry = 0;


	BUG_ON(!num_slots || !slots_per_op);
	/* start search from the last allocated descrtiptor
	 * if a contiguous allocation can not be found start searching
	 * from the beginning of the list
	 */
retry:
	slots_found = 0;
	if (retry == 0)
		iter = chan->last_used;
	else
		iter = list_entry(&chan->all_slots, apm82181_desc_t,
			slot_node);
	prefetch(iter);
	DBG("---iter at %p idx %d\n ",iter,iter->idx);
	list_for_each_entry_safe_continue(iter, _iter, &chan->all_slots,
	    slot_node) {
		prefetch(_iter);
		prefetch(&_iter->async_tx);
		if (iter->slots_per_op) {
			slots_found = 0;
			continue;
		}

		/* start the allocation if the slot is correctly aligned */
		if (!slots_found++)
			alloc_start = iter;
		if (slots_found == num_slots) {
			apm82181_desc_t *alloc_tail = NULL;
			apm82181_desc_t *last_used = NULL;
			iter = alloc_start;
			while (num_slots) {
				int i;
	
				/* pre-ack all but the last descriptor */
				if (num_slots != slots_per_op) {
					async_tx_ack(&iter->async_tx); 
				}
				list_add_tail(&iter->chain_node, &chain);
				alloc_tail = iter;
				iter->async_tx.cookie = 0;
				iter->hw_next = NULL;
				iter->flags = 0;
				iter->slot_cnt = num_slots;
				for (i = 0; i < slots_per_op; i++) {
					iter->slots_per_op = slots_per_op - i;
					last_used = iter;
					iter = list_entry(iter->slot_node.next,
						apm82181_desc_t,
						slot_node);
				}
				num_slots -= slots_per_op;
			}
			alloc_tail->group_head = alloc_start;
			alloc_tail->async_tx.cookie = -EBUSY;
			list_splice(&chain, &alloc_tail->group_list);
			chan->last_used = last_used;
			DBG("---slot allocated at %llx idx %d, hw_desc %p tx_ack %d\n",
                                alloc_tail->phys, alloc_tail->idx, alloc_tail->hw_desc, 
				async_tx_test_ack(&alloc_tail->async_tx));
			return alloc_tail;
		}
	}
	if (!retry++)
		goto retry;
#ifdef ADMA_DEBUG
	static int empty_slot_cnt;
	if(!(empty_slot_cnt%100))
		printk(KERN_INFO"No empty slots trying to free some\n");
	empty_slot_cnt++;
#endif
	/* try to free some slots if the allocation fails */
	tasklet_schedule(&chan->irq_tasklet);
	return NULL;
}

/**
 * apm82181_chan_xor_slot_count - get the number of slots necessary for
 * XOR operation
 */
static inline int apm82181_chan_xor_slot_count(size_t len, int src_cnt,
                int *slots_per_op)
{
        int slot_cnt;

        /* each XOR descriptor provides up to 16 source operands */
        slot_cnt = *slots_per_op = (src_cnt + XOR_MAX_OPS - 1)/XOR_MAX_OPS;

        if (likely(len <= APM82181_ADMA_XOR_MAX_BYTE_COUNT))
                return slot_cnt;

        printk(KERN_ERR "%s: len %d > max %d !!\n",
                __func__, len, APM82181_ADMA_XOR_MAX_BYTE_COUNT);
        BUG();
        return slot_cnt;
}

/**
 * apm82181_desc_init_null_xor - initialize the descriptor for NULL XOR
 * pseudo operation
 */
static inline void apm82181_desc_init_null_xor(apm82181_desc_t *desc)
{
        memset (desc->hw_desc, 0, sizeof(xor_cb_t));
        desc->hw_next = NULL;
        desc->src_cnt = 0;
        desc->dst_cnt = 1;
}
/**
 * apm82181_chan_set_first_xor_descriptor -  initi XORcore chain
 */
static inline void apm82181_chan_set_first_xor_descriptor(apm82181_ch_t *chan,
                                                apm82181_desc_t *next_desc)
{
        volatile xor_regs_t *xor_reg;

        xor_reg = chan->device->xor_base;

        if (xor_reg->sr & XOR_SR_XCP_BIT)
                printk(KERN_INFO "%s: Warn: XORcore is running "
                        "when try to set the first CDB!\n",
                        __func__);

        xor_last_submit = xor_last_linked = next_desc;

        xor_reg->crsr = XOR_CRSR_64BA_BIT;

        xor_reg->cblalr = next_desc->phys;
        xor_reg->cblahr = 0;
        xor_reg->cbcr |= XOR_CBCR_LNK_BIT;

        chan->hw_chain_inited = 1;
}
/**
 * apm82181_chan_start_null_xor - initiate the first XOR operation (DMA engines
 *      use FIFOs (as opposite to chains used in XOR) so this is a XOR
 *      specific operation)
 */
static void apm82181_chan_start_null_xor(apm82181_ch_t *chan)
{
        apm82181_desc_t *sw_desc, *group_start;
        dma_cookie_t cookie;
        int slot_cnt, slots_per_op;
        volatile xor_regs_t *xor_reg = chan->device->xor_base;

        dev_dbg(chan->device->common.dev,
                "apm82181 adma%d: %s\n", chan->device->id, __func__);
	INFO;
        spin_lock_bh(&chan->lock);
        slot_cnt = apm82181_chan_xor_slot_count(0, 2, &slots_per_op);
        sw_desc = apm82181_adma_alloc_slots(chan, slot_cnt, slots_per_op);
        if (sw_desc) {
		INFO;
                group_start = sw_desc->group_head;
                list_splice_init(&sw_desc->group_list, &chan->chain);
                async_tx_ack(&sw_desc->async_tx);
                apm82181_desc_init_null_xor(group_start);
		INFO;

                cookie = chan->common.cookie;
                cookie++;
                if (cookie <= 1)
                        cookie = 2;

                /* initialize the completed cookie to be less than
                 * the most recently used cookie
                 */
                chan->completed_cookie = cookie - 1;
                chan->common.cookie = sw_desc->async_tx.cookie = cookie;

                /* channel should not be busy */
                BUG_ON(apm82181_chan_is_busy(chan));

                /* set the descriptor address */
                apm82181_chan_set_first_xor_descriptor(chan, sw_desc);

                /* run the descriptor */
                xor_reg->crsr = XOR_CRSR_64BA_BIT | XOR_CRSR_XAE_BIT;
        } else
                printk(KERN_ERR "apm82181 adma%d"
                        " failed to allocate null descriptor\n",
                        chan->device->id);
        spin_unlock_bh(&chan->lock);
}

/**
 * apm82181_adma_alloc_chan_resources -  allocate pools for CDB slots
 */
static int apm82181_adma_alloc_chan_resources(struct dma_chan *chan)
{
	apm82181_ch_t *apm82181_chan = to_apm82181_adma_chan(chan);
	apm82181_desc_t *slot = NULL;
	char *hw_desc;
	int i, db_sz;
	int init = apm82181_chan->slots_allocated ? 0 : 1;

	chan->chan_id = apm82181_chan->device->id;

	/* Allocate descriptor slots */
	i = apm82181_chan->slots_allocated;
	if (apm82181_chan->device->id != APM82181_XOR_ID)
		db_sz = sizeof (dma_cdb_t);
	else
		db_sz = sizeof (xor_cb_t);

	for (; i < (apm82181_chan->device->pool_size/db_sz); i++) {
		slot = kzalloc(sizeof(apm82181_desc_t), GFP_KERNEL);
		if (!slot) {
			printk(KERN_INFO "APM82181/GT ADMA Channel only initialized"
				" %d descriptor slots", i--);
			break;
		}

		hw_desc = (char *) apm82181_chan->device->dma_desc_pool_virt;
		slot->hw_desc = (void *) &hw_desc[i * db_sz];
		dma_async_tx_descriptor_init(&slot->async_tx, chan);
		slot->async_tx.tx_submit = apm82181_adma_tx_submit;
		INIT_LIST_HEAD(&slot->chain_node);
		INIT_LIST_HEAD(&slot->slot_node);
		INIT_LIST_HEAD(&slot->group_list);
		slot->phys = apm82181_chan->device->dma_desc_pool + i * db_sz;
		slot->idx = i;
		spin_lock_bh(&apm82181_chan->lock);
		apm82181_chan->slots_allocated++;
		list_add_tail(&slot->slot_node, &apm82181_chan->all_slots);
		spin_unlock_bh(&apm82181_chan->lock);
	}

	if (i && !apm82181_chan->last_used) {
		apm82181_chan->last_used =
			list_entry(apm82181_chan->all_slots.next,
				apm82181_desc_t,
				slot_node);
	}

	printk("apm82181 adma%d: allocated %d descriptor slots\n",
		apm82181_chan->device->id, i);

	/* initialize the channel and the chain with a null operation */
	if (init) {
		switch (apm82181_chan->device->id)
		{
		apm82181_chan->hw_chain_inited = 0;
		case APM82181_PDMA0_ID:
			apm82181_dma_tchan[0] = apm82181_chan;
			break;
		case APM82181_PDMA1_ID:
			apm82181_dma_tchan[1] = apm82181_chan;
			break;
		case APM82181_PDMA2_ID:
			apm82181_dma_tchan[2] = apm82181_chan;
			break;
		case APM82181_PDMA3_ID:
			apm82181_dma_tchan[3] = apm82181_chan;
			break;
		case APM82181_XOR_ID:
			apm82181_dma_tchan[4] = apm82181_chan;
			apm82181_chan_start_null_xor(apm82181_chan);
                        break;
		default:
			BUG();
		}
		apm82181_chan->needs_unmap = 1;
	}

	return (i > 0) ? i : -ENOMEM;
}

/**
 * apm82181_desc_assign_cookie - assign a cookie
 */
static dma_cookie_t apm82181_desc_assign_cookie(apm82181_ch_t *chan,
		apm82181_desc_t *desc)
{
	dma_cookie_t cookie = chan->common.cookie;
	cookie++;
	if (cookie < 0)
		cookie = 1;
	chan->common.cookie = desc->async_tx.cookie = cookie;
	return cookie;
}


/**
 * apm82181_adma_check_threshold - append CDBs to h/w chain if threshold
 *	has been achieved
 */
static void apm82181_adma_check_threshold(apm82181_ch_t *chan)
{
	dev_dbg(chan->device->common.dev, "apm82181 adma%d: pending: %d\n",
		chan->device->id, chan->pending);
	INFO;
	if (chan->pending >= APM82181_ADMA_THRESHOLD) {
		chan->pending = 0;
		apm82181_chan_append(chan);
	}
}

/**
 * apm82181_adma_tx_submit - submit new descriptor group to the channel
 *	(it's not necessary that descriptors will be submitted to the h/w
 *	chains too right now)
 */
static dma_cookie_t apm82181_adma_tx_submit(struct dma_async_tx_descriptor *tx)
{
	apm82181_desc_t *sw_desc = tx_to_apm82181_adma_slot(tx);
	apm82181_ch_t *chan = to_apm82181_adma_chan(tx->chan);
	apm82181_desc_t *group_start, *old_chain_tail;
	int slot_cnt;
	int slots_per_op;
	dma_cookie_t cookie;
	group_start = sw_desc->group_head;
	slot_cnt = group_start->slot_cnt;
	slots_per_op = group_start->slots_per_op;
	INFO;
	spin_lock_bh(&chan->lock);
	cookie = apm82181_desc_assign_cookie(chan, sw_desc);

	if (unlikely(list_empty(&chan->chain))) {
		/* first peer */
		list_splice_init(&sw_desc->group_list, &chan->chain);
		chan_first_cdb[chan->device->id] = group_start;
	} else {
		/* isn't first peer, bind CDBs to chain */
		old_chain_tail = list_entry(chan->chain.prev,
			apm82181_desc_t, chain_node);
		list_splice_init(&sw_desc->group_list,
		    &old_chain_tail->chain_node);
		/* fix up the hardware chain */
		apm82181_desc_set_link(chan, old_chain_tail, group_start);
	}

	/* increment the pending count by the number of operations */
	chan->pending += slot_cnt / slots_per_op;
	apm82181_adma_check_threshold(chan);
	spin_unlock_bh(&chan->lock);

	DBG("apm82181 adma%d:cookie: %d slot: %d tx %p\n",
		chan->device->id, sw_desc->async_tx.cookie, sw_desc->idx, sw_desc);
	return cookie;
}
/**
 * apm82181_adma_prep_dma_xor - prepare CDB for a XOR operation
 */
static struct dma_async_tx_descriptor *apm82181_adma_prep_dma_xor(
                struct dma_chan *chan, dma_addr_t dma_dest,
                dma_addr_t *dma_src, unsigned int src_cnt, size_t len,
                unsigned long flags)
{
	apm82181_ch_t *apm82181_chan = to_apm82181_adma_chan(chan);
        apm82181_desc_t *sw_desc, *group_start;
        int slot_cnt, slots_per_op;

#ifdef ADMA_DEBUG
        printk("\n%s(%d):\n\tsrc: ", __func__,
                apm82181_chan->device->id);
        for (slot_cnt=0; slot_cnt < src_cnt; slot_cnt++)
                printk("0x%llx ", dma_src[slot_cnt]);
        printk("\n\tdst: 0x%llx\n", dma_dest);
#endif
        if (unlikely(!len))
                return NULL;
        BUG_ON(unlikely(len > APM82181_ADMA_XOR_MAX_BYTE_COUNT));

        dev_dbg(apm82181_chan->device->common.dev,
                "apm82181 adma%d: %s src_cnt: %d len: %u int_en: %d\n",
                apm82181_chan->device->id, __func__, src_cnt, len,
                flags & DMA_PREP_INTERRUPT ? 1 : 0);

        spin_lock_bh(&apm82181_chan->lock);
        slot_cnt = apm82181_chan_xor_slot_count(len, src_cnt, &slots_per_op);
        sw_desc = apm82181_adma_alloc_slots(apm82181_chan, slot_cnt,
                        slots_per_op);
        if (sw_desc) {
                group_start = sw_desc->group_head;
                apm82181_desc_init_xor(group_start, src_cnt, flags);
                apm82181_adma_set_dest(group_start, dma_dest, 0);
                while (src_cnt--)
                        apm82181_adma_set_src(group_start,
                                dma_src[src_cnt], src_cnt);
                apm82181_desc_set_byte_count(group_start, apm82181_chan, len);
                sw_desc->unmap_len = len;
                sw_desc->async_tx.flags = flags;
        }
        spin_unlock_bh(&apm82181_chan->lock);

        return sw_desc ? &sw_desc->async_tx : NULL;
}
/**
 * apm82181_adma_prep_dma_interrupt - prepare CDB for a pseudo DMA operation
 */
static struct dma_async_tx_descriptor *apm82181_adma_prep_dma_interrupt(
		struct dma_chan *chan, unsigned long flags)
{
	apm82181_ch_t *apm82181_chan = to_apm82181_adma_chan(chan);
	apm82181_desc_t *sw_desc, *group_start;
	int slot_cnt, slots_per_op;

	dev_dbg(apm82181_chan->device->common.dev,
		"apm82181 adma%d: %s\n", apm82181_chan->device->id,
		__FUNCTION__);
	spin_lock_bh(&apm82181_chan->lock);
	slot_cnt = slots_per_op = 1;
	sw_desc = apm82181_adma_alloc_slots(apm82181_chan, slot_cnt,
			slots_per_op);
	if (sw_desc) {
		group_start = sw_desc->group_head;
		apm82181_desc_init_interrupt(group_start, apm82181_chan);
		group_start->unmap_len = 0;
		sw_desc->async_tx.flags = flags;
	}
	spin_unlock_bh(&apm82181_chan->lock);

	return sw_desc ? &sw_desc->async_tx : NULL;
}

/**
 * apm82181_adma_prep_dma_memcpy - prepare CDB for a MEMCPY operation
 */
static struct dma_async_tx_descriptor *apm82181_adma_prep_dma_memcpy(
		struct dma_chan *chan, dma_addr_t dma_dest,
		dma_addr_t dma_src, size_t len, unsigned long flags)
{
	apm82181_ch_t *apm82181_chan = to_apm82181_adma_chan(chan);
	apm82181_desc_t *sw_desc, *group_start;
	int slot_cnt, slots_per_op;
	if (unlikely(!len))
		return NULL;
	BUG_ON(unlikely(len > APM82181_ADMA_DMA_MAX_BYTE_COUNT));

	spin_lock_bh(&apm82181_chan->lock);

	dev_dbg(apm82181_chan->device->common.dev,
		"apm82181 adma%d: %s len: %u int_en %d \n",
		apm82181_chan->device->id, __FUNCTION__, len,
		flags & DMA_PREP_INTERRUPT ? 1 : 0);

	slot_cnt = slots_per_op = 1;
	sw_desc = apm82181_adma_alloc_slots(apm82181_chan, slot_cnt,
		slots_per_op);
	if (sw_desc) {
		group_start = sw_desc->group_head;
		flags |= DMA_PREP_INTERRUPT;	
		apm82181_desc_init_memcpy(group_start, flags);
		apm82181_adma_set_dest(group_start, dma_dest, 0);
		apm82181_adma_set_src(group_start, dma_src, 0);
		apm82181_desc_set_byte_count(group_start, apm82181_chan, len);
		sw_desc->unmap_len = len;
		sw_desc->async_tx.flags = flags;
	}
	spin_unlock_bh(&apm82181_chan->lock);
	return sw_desc ? &sw_desc->async_tx : NULL;
}

/**
 * apm82181_adma_prep_dma_memset - prepare CDB for a MEMSET operation
 */
static struct dma_async_tx_descriptor *apm82181_adma_prep_dma_memset(
		struct dma_chan *chan, dma_addr_t dma_dest, int value,
		size_t len, unsigned long flags)
{
	apm82181_ch_t *apm82181_chan = to_apm82181_adma_chan(chan);
	apm82181_desc_t *sw_desc, *group_start;
	int slot_cnt, slots_per_op;
	if (unlikely(!len))
		return NULL;
	BUG_ON(unlikely(len > APM82181_ADMA_DMA_MAX_BYTE_COUNT));

	spin_lock_bh(&apm82181_chan->lock);

	dev_dbg(apm82181_chan->device->common.dev,
		"apm82181 adma%d: %s cal: %u len: %u int_en %d\n",
		apm82181_chan->device->id, __FUNCTION__, value, len,
		flags & DMA_PREP_INTERRUPT ? 1 : 0);

	slot_cnt = slots_per_op = 1;
	sw_desc = apm82181_adma_alloc_slots(apm82181_chan, slot_cnt,
		slots_per_op);
	if (sw_desc) {
		group_start = sw_desc->group_head;
		apm82181_desc_init_memset(group_start, value, flags);
		apm82181_adma_set_dest(group_start, dma_dest, 0);
		apm82181_desc_set_byte_count(group_start, apm82181_chan, len);
		sw_desc->unmap_len = len;
		sw_desc->async_tx.flags = flags;
	}
	spin_unlock_bh(&apm82181_chan->lock);

	return sw_desc ? &sw_desc->async_tx : NULL;
}


/**
 * apm82181_adma_set_dest - set destination address into descriptor
 */
static void apm82181_adma_set_dest(apm82181_desc_t *sw_desc,
		dma_addr_t addr, int index)
{
	apm82181_ch_t *chan = to_apm82181_adma_chan(sw_desc->async_tx.chan);
	BUG_ON(index >= sw_desc->dst_cnt);

	switch (chan->device->id) {
	case APM82181_PDMA0_ID:
	case APM82181_PDMA1_ID:
	case APM82181_PDMA2_ID:
	case APM82181_PDMA3_ID:
		/* to do: support transfers lengths >
		 * APM82181_ADMA_DMA/XOR_MAX_BYTE_COUNT
		 */
		apm82181_desc_set_dest_addr(sw_desc->group_head,
		//	chan, 0x8, addr, index); // Enabling HB bus
			chan, addr, index);
		break;
	case APM82181_XOR_ID:
		sw_desc = apm82181_get_group_entry(sw_desc, index);
		apm82181_desc_set_dest_addr(sw_desc, chan, 
			addr, index);
		break;
	default:
		BUG();
	}
}


/**
 * apm82181_adma_free_chan_resources - free the resources allocated
 */
static void apm82181_adma_free_chan_resources(struct dma_chan *chan)
{
	apm82181_ch_t *apm82181_chan = to_apm82181_adma_chan(chan);
	apm82181_desc_t *iter, *_iter;
	int in_use_descs = 0;

	apm82181_adma_slot_cleanup(apm82181_chan);

	spin_lock_bh(&apm82181_chan->lock);
	list_for_each_entry_safe(iter, _iter, &apm82181_chan->chain,
					chain_node) {
		in_use_descs++;
		list_del(&iter->chain_node);
	}
	list_for_each_entry_safe_reverse(iter, _iter,
			&apm82181_chan->all_slots, slot_node) {
		list_del(&iter->slot_node);
		kfree(iter);
		apm82181_chan->slots_allocated--;
	}
	apm82181_chan->last_used = NULL;

	dev_dbg(apm82181_chan->device->common.dev,
		"apm82181 adma%d %s slots_allocated %d\n",
		apm82181_chan->device->id,
		__FUNCTION__, apm82181_chan->slots_allocated);
	spin_unlock_bh(&apm82181_chan->lock);

	/* one is ok since we left it on there on purpose */
	if (in_use_descs > 1)
		printk(KERN_ERR "GT: Freeing %d in use descriptors!\n",
			in_use_descs - 1);
}

/**
 * apm82181_adma_is_complete - poll the status of an ADMA transaction
 * @chan: ADMA channel handle
 * @cookie: ADMA transaction identifier
 */
static enum dma_status apm82181_adma_is_complete(struct dma_chan *chan,
	dma_cookie_t cookie, dma_cookie_t *done, dma_cookie_t *used)
{
	apm82181_ch_t *apm82181_chan = to_apm82181_adma_chan(chan);
	dma_cookie_t last_used;
	dma_cookie_t last_complete;
	enum dma_status ret;

	last_used = chan->cookie;
	last_complete = apm82181_chan->completed_cookie;

	if (done)
		*done= last_complete;
	if (used)
		*used = last_used;

	ret = dma_async_is_complete(cookie, last_complete, last_used);
	if (ret == DMA_SUCCESS)
		return ret;

	apm82181_adma_slot_cleanup(apm82181_chan);

	last_used = chan->cookie;
	last_complete = apm82181_chan->completed_cookie;

	if (done)
		*done= last_complete;
	if (used)
		*used = last_used;

	return dma_async_is_complete(cookie, last_complete, last_used);
}

/**
 * apm82181_adma_eot_handler - end of transfer interrupt handler
 */
static irqreturn_t apm82181_adma_eot_handler(int irq, void *data)
{
	apm82181_ch_t *chan = data;

	dev_dbg(chan->device->common.dev,
		"apm82181 adma%d: %s\n", chan->device->id, __FUNCTION__);
	INFO;
	if(chan->device->id == APM82181_XOR_ID)
		tasklet_schedule(&chan->irq_tasklet);
	apm82181_adma_device_clear_eot_status(chan);

	return IRQ_HANDLED;
}

/**
 * apm82181_adma_err_handler - DMA error interrupt handler;
 *	do the same things as a eot handler
 */
#if 0
static irqreturn_t apm82181_adma_err_handler(int irq, void *data)
{
	apm82181_ch_t *chan = data;
	dev_dbg(chan->device->common.dev,
		"apm82181 adma%d: %s\n", chan->device->id, __FUNCTION__);
	tasklet_schedule(&chan->irq_tasklet);
	apm82181_adma_device_clear_eot_status(chan);

	return IRQ_HANDLED;
}
#endif
/**
 * apm82181_test_callback - called when test operation has been done
 */
static void apm82181_test_callback (void *unused)
{
	complete(&apm82181_r5_test_comp);
}

/**
 * apm82181_adma_issue_pending - flush all pending descriptors to h/w
 */
static void apm82181_adma_issue_pending(struct dma_chan *chan)
{
	apm82181_ch_t *apm82181_chan = to_apm82181_adma_chan(chan);

	DBG("apm82181 adma%d: %s %d \n", apm82181_chan->device->id,
	    __FUNCTION__, apm82181_chan->pending);
	if (apm82181_chan->pending) {
		apm82181_chan->pending = 0;
		apm82181_chan_append(apm82181_chan);
	}
}

/**
 * apm82181_adma_remove - remove the asynch device
 */
static int __devexit apm82181_pdma_remove(struct platform_device *dev)
{
	apm82181_dev_t *device = platform_get_drvdata(dev);
	struct dma_chan *chan, *_chan;
	struct ppc_dma_chan_ref *ref, *_ref;
	apm82181_ch_t *apm82181_chan;
	int i;

	dma_async_device_unregister(&device->common);

	for (i = 0; i < 3; i++) {
		u32 irq;
		irq = platform_get_irq(dev, i);
		free_irq(irq, device);
	}
	

	do {
		struct resource *res;
		res = platform_get_resource(dev, IORESOURCE_MEM, 0);
		release_mem_region(res->start, res->end - res->start);
	} while (0);

	list_for_each_entry_safe(chan, _chan, &device->common.channels,
				device_node) {
		apm82181_chan = to_apm82181_adma_chan(chan);
		list_del(&chan->device_node);
		kfree(apm82181_chan);
	}

	list_for_each_entry_safe(ref, _ref, &ppc_adma_chan_list, node) {
		list_del(&ref->node);
		kfree(ref);
	}

	kfree(device);

	return 0;
}

static inline void xor_hw_init (apm82181_dev_t *adev)
{
	volatile xor_regs_t *xor_reg = adev->xor_base;
	/* Reset XOR */
        xor_reg->crsr = XOR_CRSR_XASR_BIT;
        xor_reg->crrr = XOR_CRSR_64BA_BIT;	

	/* enable XOR engine interrupts */
        xor_reg->ier = XOR_IE_CBCIE_BIT |
        	XOR_IE_ICBIE_BIT | XOR_IE_ICIE_BIT | XOR_IE_RPTIE_BIT;
}

/*
 * Per channel probe
 */
static int __devinit apm82181_dma_per_chan_probe(struct of_device *ofdev,
		const struct of_device_id *match)
{
	int ret = 0, irq;
	const u32 *index, *dcr_regs, *pool_size;
	apm82181_plb_dma_t *pdma;
	apm82181_dev_t *adev;
	apm82181_ch_t *chan;
	struct ppc_dma_chan_ref *ref;
        struct device_node *np = ofdev->node;
	struct resource res;
	int len;
	
	INFO;	
	pdma = dev_get_drvdata(ofdev->dev.parent);
	BUG_ON(!pdma);
	if ((adev = kzalloc(sizeof(*adev), GFP_KERNEL)) == NULL) {
		 printk("ERROR:No Free memory for allocating dma channels\n");
		 ret = -ENOMEM;
		 goto out;
	}
	adev->dev = &ofdev->dev;
	index = of_get_property(np, "cell-index", NULL);
        if(!index) {
                printk(KERN_ERR "adma-channel: Device node %s has missing or invalid "
                       "cell-index property\n", np->full_name);
                goto err;
        }
	adev->id = (int)*index;
	/* The XOR engine/PLB DMA 4 channels have different resources/pool_sizes */
	if (adev->id != APM82181_XOR_ID){
		dcr_regs = of_get_property(np, "dcr-reg", &len);
        	if (!dcr_regs || (len != 2 * sizeof(u32))) {
                	printk(KERN_ERR "plb_dma channel%d: Can't get DCR register base !",
                        	adev->id);
                	goto err;
        	}
		adev->dcr_base = dcr_regs[0];

		pool_size = of_get_property(np, "pool_size", NULL);
        	if(!pool_size) {
                	printk(KERN_ERR "plb_dma channel%d: Device node %s has missing or "
                       		"invalid pool_size property\n", adev->id, np->full_name);
                	goto err;
        	}
        	adev->pool_size = *pool_size;
	} else {
		if (of_address_to_resource(np, 0, &res)) {
                	printk(KERN_ERR "adma_xor channel%d %s: could not get resource address.\n",
                        	adev->id,np->full_name);
                       	goto err;
               	}

               	DBG("XOR resource start = %llx end = %llx\n", res.start, res.end);
		adev->xor_base = ioremap(res.start, res.end - res.start + 1);
		if (!adev->xor_base){
			printk(KERN_ERR "XOR engine registers memory mapping failed.\n");
			goto err;
		}			
		adev->pool_size = PAGE_SIZE << 1;
	}

	adev->pdma = pdma;
	adev->ofdev = ofdev;
	dev_set_drvdata(&(ofdev->dev),adev);

	switch (adev->id){
        case APM82181_PDMA0_ID:
        case APM82181_PDMA1_ID:
        case APM82181_PDMA2_ID:
        case APM82181_PDMA3_ID:
                dma_cap_set(DMA_MEMCPY,adev->cap_mask);
                break;
	case APM82181_XOR_ID:
		dma_cap_set(DMA_XOR,adev->cap_mask);
		dma_cap_set(DMA_INTERRUPT,adev->cap_mask);
		break;
        default:
                BUG();
        }
	/* XOR h/w configuration */
	if(adev->id == APM82181_XOR_ID)
		xor_hw_init(adev);
	/* allocate coherent memory for hardware descriptors
         * note: writecombine gives slightly better performance, but
         * requires that we explicitly drain the write buffer
         */
	if ((adev->dma_desc_pool_virt = dma_alloc_coherent(&ofdev->dev,
		adev->pool_size, &adev->dma_desc_pool, GFP_KERNEL)) == NULL) {
		ret = -ENOMEM;
		goto err_dma_alloc;
	}
	
	adev->common.cap_mask = adev->cap_mask;
	INIT_LIST_HEAD(&adev->common.channels);
        /* set base routines */
        adev->common.device_alloc_chan_resources =
            apm82181_adma_alloc_chan_resources;
        adev->common.device_free_chan_resources =
            apm82181_adma_free_chan_resources;
        adev->common.device_is_tx_complete = apm82181_adma_is_complete;
        adev->common.device_issue_pending = apm82181_adma_issue_pending;
        adev->common.dev = &ofdev->dev;

	/* set prep routines based on capability */
        if (dma_has_cap(DMA_MEMCPY, adev->common.cap_mask)) {
                adev->common.device_prep_dma_memcpy =
                    apm82181_adma_prep_dma_memcpy;
        }
        if (dma_has_cap(DMA_MEMSET, adev->common.cap_mask)) {
                adev->common.device_prep_dma_memset =
                    apm82181_adma_prep_dma_memset;
        }

        if (dma_has_cap(DMA_INTERRUPT, adev->common.cap_mask)) {
                adev->common.device_prep_dma_interrupt =
                    apm82181_adma_prep_dma_interrupt;
        }

	if (dma_has_cap(DMA_XOR, adev->common.cap_mask)) {
                adev->common.max_xor = XOR_MAX_OPS;
		adev->common.device_prep_dma_xor = 
		    apm82181_adma_prep_dma_xor;
	}

	/* create a channel */
        if ((chan = kzalloc(sizeof(*chan), GFP_KERNEL)) == NULL) {
                ret = -ENOMEM;
                goto err_chan_alloc;
        }
        tasklet_init(&chan->irq_tasklet, apm82181_adma_tasklet,
            (unsigned long)chan);
	
	irq = irq_of_parse_and_map(np, 0);
	switch (adev->id){
        case 0:
        	if (irq >= 0) {
                	ret = request_irq(irq, apm82181_adma_eot_handler,
                        	IRQF_DISABLED, "adma-chan0", chan);
                	if (ret) {
                        	printk("Failed to request IRQ %d\n",irq);
                        	ret = -EIO;
                        	goto err_irq;
			}
                }
                break;
        case 1:
        	if (irq >= 0) {
                	ret = request_irq(irq, apm82181_adma_eot_handler,
                        	IRQF_DISABLED, "adma-chan1", chan);
                	if (ret) {
                        	printk("Failed to request IRQ %d\n",irq);
                        	ret = -EIO;
                        	goto err_irq;
			}
                }
                break;
        case 2:
        	if (irq >= 0) {
                	ret = request_irq(irq, apm82181_adma_eot_handler,
                        	IRQF_DISABLED, "adma-chan2", chan);
                	if (ret) {
                        	printk("Failed to request IRQ %d\n",irq);
                        	ret = -EIO;
                        	goto err_irq;
			}
                }
                break;
	case 3:
        	if (irq >= 0) {
                	ret = request_irq(irq, apm82181_adma_eot_handler,
                        	IRQF_DISABLED, "adma-chan3", chan);
                	if (ret) {
                        	printk("Failed to request IRQ %d\n",irq);
                        	ret = -EIO;
                        	goto err_irq;
			}
                }
		break;
	case 4:
                if (irq >= 0) {
                        ret = request_irq(irq, apm82181_adma_eot_handler,
                                IRQF_DISABLED, "adma-xor", chan);
                        if (ret) {
                                printk("Failed to request IRQ %d\n",irq);
                                ret = -EIO;
                                goto err_irq;
                        }
                }
                break;
        default:
                BUG();
        }

	spin_lock_init(&chan->lock);
        chan->device = adev;
	INIT_LIST_HEAD(&chan->chain);
        INIT_LIST_HEAD(&chan->all_slots);
        chan->common.device = &adev->common;
        list_add_tail(&chan->common.device_node, &adev->common.channels);
        adev->common.chancnt++;
	
	printk( "AMCC(R) APM82181 ADMA Engine found [%d]: "
          "( capabilities: %s%s%s%s%s%s%s)\n",
          adev->id,
          dma_has_cap(DMA_PQ, adev->common.cap_mask) ? "pq_xor " : "",
          dma_has_cap(DMA_PQ_VAL, adev->common.cap_mask) ? "pq_val " :
            "",
          dma_has_cap(DMA_XOR, adev->common.cap_mask) ? "xor " : "",
          dma_has_cap(DMA_XOR_VAL, adev->common.cap_mask) ? "xor_val " :
            "",
          dma_has_cap(DMA_MEMSET, adev->common.cap_mask)  ? "memset " : "",
          dma_has_cap(DMA_MEMCPY, adev->common.cap_mask) ? "memcpy " : "",
          dma_has_cap(DMA_INTERRUPT, adev->common.cap_mask) ? "int " : "");
	INFO;
        ret = dma_async_device_register(&adev->common);
        if (ret) {
                dev_err(&ofdev->dev, "failed to register dma async device");
                goto err_irq;
        }
	INFO;
        ref = kmalloc(sizeof(*ref), GFP_KERNEL);
        if (ref) {
                INFO;
                ref->chan = &chan->common;
                INIT_LIST_HEAD(&ref->node);
                list_add_tail(&ref->node, &ppc_adma_chan_list);
        } else
                dev_warn(&ofdev->dev, "failed to allocate channel reference!\n");

	goto out;
err_irq:
	kfree(chan);
err_chan_alloc:
	dma_free_coherent(&ofdev->dev, adev->pool_size,
                        adev->dma_desc_pool_virt, adev->dma_desc_pool);
err_dma_alloc:
	if (adev->xor_base)
		iounmap(adev->xor_base);	
err:
	kfree(adev);
out:
	return ret;
}

static struct of_device_id dma_4chan_match[] = 
{
	{
		.compatible     = "amcc,apm82181-adma",
	},
	{},
};

static struct of_device_id dma_per_chan_match[] = {
	{.compatible = "amcc,apm82181-dma-4channel",},
	{.compatible = "amcc,xor",},
	{},
};
/*
 * apm82181_adma_probe - probe the asynch device
 */
static int __devinit apm82181_pdma_probe(struct of_device *ofdev,
		const struct of_device_id *match)
{
	int ret = 0; 
	apm82181_plb_dma_t *pdma;

	if ((pdma = kzalloc(sizeof(*pdma), GFP_KERNEL)) == NULL) {
		 ret = -ENOMEM; 
		 goto out;
	}
	pdma->dev = &ofdev->dev;
	pdma->ofdev = ofdev;
	printk(PPC4XX_EDMA "Probing AMCC APM82181 ADMA engines...\n");

	dev_set_drvdata(&(ofdev->dev),pdma);
	of_platform_bus_probe(ofdev->node, dma_per_chan_match,&ofdev->dev);

out:
	return ret;
}

/*
 * apm82181_test_xor - test are RAID-5 XOR capability enabled successfully.
 *      For this we just perform one DMA XOR operation with the 3 sources
 *      to a destination 
 */
static int apm82181_test_xor (apm82181_ch_t *chan)
{
        apm82181_desc_t *sw_desc, *group_start;
        struct page *pg_src[3], *pg_dest;
        char *a;
        dma_addr_t dma_src_addr[3];
        dma_addr_t dma_dst_addr;
        int rval = -EFAULT, i;
        int len = PAGE_SIZE, src_cnt = 3;
	int slot_cnt, slots_per_op;
	INFO;
        printk("ADMA channel %d XOR testing\n",chan->device->id);
	for(i = 0; i < 3; i++){
        	pg_src[i] = alloc_page(GFP_KERNEL);
        	if (!pg_src[i])
                	return -ENOMEM;
	}
        pg_dest = alloc_page(GFP_KERNEL);
        if (!pg_dest)
                return -ENOMEM;
        /* Fill the test page with ones */
        memset(page_address(pg_src[0]), 0xDA, len);
        memset(page_address(pg_src[1]), 0xDA, len);
        memset(page_address(pg_src[2]), 0x00, len);
        memset(page_address(pg_dest), 0xA5, len);
	for(i = 0; i < 3; i++){
        	a = page_address(pg_src[i]);
        	printk("The virtual addr of src %d =%x\n",i, (unsigned int)a);
        	MEM_HEXDUMP(a,50);
	}
        a = page_address(pg_dest);
        printk("The virtual addr of dest=%x\n", (unsigned int)a);
        MEM_HEXDUMP(a,50);

	for(i = 0; i < 3; i++){
        	dma_src_addr[i] = dma_map_page(chan->device->dev, pg_src[i], 0, len,
            		DMA_BIDIRECTIONAL);
	}
        dma_dst_addr = dma_map_page(chan->device->dev, pg_dest, 0, len,
            DMA_BIDIRECTIONAL);
        printk("dma_src_addr[0]: %llx; dma_src_addr[1]: %llx;\n "
		"dma_src_addr[2]: %llx; dma_dst_addr %llx, len: %x\n", dma_src_addr[0],
		dma_src_addr[1], dma_src_addr[2], dma_dst_addr, len);

        spin_lock_bh(&chan->lock);
	slot_cnt = apm82181_chan_xor_slot_count(len, src_cnt, &slots_per_op);
        sw_desc = apm82181_adma_alloc_slots(chan, slot_cnt, slots_per_op);
        if (sw_desc) {
		group_start = sw_desc->group_head;
                apm82181_desc_init_xor(group_start, src_cnt, DMA_PREP_INTERRUPT);
                /* Setup addresses */
		while (src_cnt--)
                        apm82181_adma_set_src(group_start,
                                dma_src_addr[src_cnt], src_cnt);
                apm82181_adma_set_dest(group_start, dma_dst_addr, 0);
                apm82181_desc_set_byte_count(group_start, chan, len);
                sw_desc->unmap_len = PAGE_SIZE;
        } else {
		rval = -EFAULT;
                spin_unlock_bh(&chan->lock);
                goto exit;
        }
        spin_unlock_bh(&chan->lock);

        printk("Submit CDB...\n");
        MEM_HEXDUMP(sw_desc->hw_desc, 96);
        async_tx_ack(&sw_desc->async_tx);
        sw_desc->async_tx.callback = apm82181_test_callback;
        sw_desc->async_tx.callback_param = NULL;

        init_completion(&apm82181_r5_test_comp);
        apm82181_adma_tx_submit(&sw_desc->async_tx);
        apm82181_adma_issue_pending(&chan->common);
        //wait_for_completion(&apm82181_r5_test_comp);
	/* wait for a while so that dma transaction finishes */
	mdelay(100);
	/* Now check if the test page zeroed */
        a = page_address(pg_dest);
	/* XOR result at destination */
        MEM_HEXDUMP(a,50);
        if ((*(u32*)a) == 0x00000000 && memcmp(a, a+4, PAGE_SIZE-4)==0) {
                /* page dest XOR is corect as expected - RAID-5 enabled */
                rval = 0;
        } else {
                /* RAID-5 was not enabled */
                rval = -EINVAL;
        }

exit:
	dma_unmap_page(chan->device->dev, dma_src_addr[0], PAGE_SIZE, DMA_BIDIRECTIONAL);
        dma_unmap_page(chan->device->dev, dma_src_addr[1], PAGE_SIZE, DMA_BIDIRECTIONAL);
        dma_unmap_page(chan->device->dev, dma_src_addr[2], PAGE_SIZE, DMA_BIDIRECTIONAL);
        dma_unmap_page(chan->device->dev, dma_dst_addr, PAGE_SIZE, DMA_BIDIRECTIONAL);
        __free_page(pg_src[0]);
        __free_page(pg_src[1]);
        __free_page(pg_src[2]);
        __free_page(pg_dest);
        return rval;
}


/*
 * apm82181_test_dma - test are RAID-5 capabilities enabled successfully.
 *	For this we just perform one WXOR operation with the same source
 *	and destination addresses, the GF-multiplier is 1; so if RAID-5
	o/of_platform_driver_unregister(&apm82181_pdma_driver);
 *	capabilities are enabled then we'll get src/dst filled with zero.
 */
static int apm82181_test_dma (apm82181_ch_t *chan)
{
	apm82181_desc_t *sw_desc;
	struct page *pg_src, *pg_dest;
	char *a, *d;
	dma_addr_t dma_src_addr;
	dma_addr_t dma_dst_addr;
	int rval = -EFAULT;
	int len = PAGE_SIZE;

	printk("PLB DMA channel %d memcpy testing\n",chan->device->id);
	pg_src = alloc_page(GFP_KERNEL);
	if (!pg_src)
		return -ENOMEM;
	pg_dest = alloc_page(GFP_KERNEL);
	if (!pg_dest)
		return -ENOMEM;
	/* Fill the test page with ones */
	memset(page_address(pg_src), 0x77, len);
	memset(page_address(pg_dest), 0xa5, len);
	a = page_address(pg_src);
        printk("The virtual addr of src =%x\n", (unsigned int)a);
        MEM_HEXDUMP(a,50);
        a = page_address(pg_dest);
        printk("The virtual addr of dest=%x\n", (unsigned int)a);
        MEM_HEXDUMP(a,50);
	dma_src_addr = dma_map_page(chan->device->dev, pg_src, 0, len,
	    DMA_BIDIRECTIONAL);
	dma_dst_addr = dma_map_page(chan->device->dev, pg_dest, 0, len,
	    DMA_BIDIRECTIONAL);
	printk("dma_src_addr: %llx; dma_dst_addr %llx\n", dma_src_addr, dma_dst_addr);

	spin_lock_bh(&chan->lock);
	sw_desc = apm82181_adma_alloc_slots(chan, 1, 1);
	if (sw_desc) {
		/* 1 src, 1 dst, int_ena */
		apm82181_desc_init_memcpy(sw_desc, DMA_PREP_INTERRUPT);
		//apm82181_desc_init_memcpy(sw_desc, 0);
		/* Setup adresses */
		apm82181_adma_set_src(sw_desc, dma_src_addr, 0);
		apm82181_adma_set_dest(sw_desc, dma_dst_addr, 0);
		apm82181_desc_set_byte_count(sw_desc, chan, len);
		sw_desc->unmap_len = PAGE_SIZE;
	} else {
		rval = -EFAULT;
		spin_unlock_bh(&chan->lock);
		goto exit;
	}
	spin_unlock_bh(&chan->lock);

	printk("Submit CDB...\n");
	MEM_HEXDUMP(sw_desc->hw_desc, 96);
	async_tx_ack(&sw_desc->async_tx);
	sw_desc->async_tx.callback = apm82181_test_callback;
	sw_desc->async_tx.callback_param = NULL;

	init_completion(&apm82181_r5_test_comp);
	apm82181_adma_tx_submit(&sw_desc->async_tx);
	apm82181_adma_issue_pending(&chan->common);
	//wait_for_completion(&apm82181_r5_test_comp);
	
	a = page_address(pg_src);
        d = page_address(pg_dest);
        if (!memcmp(a, d, len)) {
                rval = 0;
        } else {
                rval = -EINVAL;
        }

        a = page_address(pg_src);
        printk("\nAfter DMA done:");
        printk("\nsrc %x value:\n", (unsigned int)a);
        MEM_HEXDUMP(a,96);
        a = page_address(pg_dest);
        printk("\ndest%x value:\n", (unsigned int)a);
        MEM_HEXDUMP(a,96);

exit:
	__free_page(pg_src);
	__free_page(pg_dest);
	return rval;
}

static struct of_platform_driver apm82181_pdma_driver = {
	.name		= "apm82181_plb_dma",
	.match_table = dma_4chan_match,

	.probe		= apm82181_pdma_probe,
	//.remove		= apm82181_pdma_remove,
};
struct of_platform_driver apm82181_dma_per_chan_driver = {
	.name = "apm82181-dma-4channel",
	.match_table = dma_per_chan_match,
	.probe = apm82181_dma_per_chan_probe,
};

static int apm82181_xor_read (char *page, char **start, off_t off,
        int count, int *eof, void *data)
{
        char *p = page;

        p += sprintf(p, "%s\n",
                apm82181_xor_verified ?
                "APM82181 ASYNC XOR capability are VERIFIED.\n" :
                "APM82181 ASYNC XOR capability are NOT VERIFIED.\n");

        return p - page;
}

static int apm82181_xor_write (struct file *file, const char __user *buffer,
        unsigned long count, void *data)
{
        if(count != 2)
                return -EFAULT;
        /* Verify does it really work now */
        if (!apm82181_test_xor(apm82181_dma_tchan[4])) {
                /* APM82181 RAID-5 XOR has been activated successfully */;
                printk("APM82181 ADMA XOR engine has been verified "
                        "successfully\n");
                apm82181_xor_verified = 1;
        } else {
                /* APM82181 RAID-5 memcpy hasn't been activated! */;
                printk("APM82181 ADMA XOR engine hasn't been "
                        "verified yet\n");
                apm82181_xor_verified = 0;
        }

        return count;
}

static int apm82181_dma_read (char *page, char **start, off_t off,
	int count, int *eof, void *data)
{
	int i;

        printk("APM82181 ASYNC MEMCPY capability\n");
        for(i = 0; i < 4; i++){
                printk("\tPLB DMA channel %d: %s ", i,
                        apm82181_memcpy_verified[i] ?
                        "VERIFIED.\n" : "NOT VERIFIED.\n");
        }
        return 0;
}

static int apm82181_dma_write (struct file *file, const char __user *buffer,
	unsigned long count, void *data)
{
	/* e.g. 0xffffffff */
	char tmp[2];
	u32 val;

	if(count != 2)
		return -EFAULT;

	if (copy_from_user(tmp, buffer, count))
		return -EFAULT;
	val = simple_strtoul(tmp, NULL, 10); /* decimal base */
	if(!(val == 0 || val == 1 || val == 2 || val == 3 )) {
                printk("Error! Wrong channel id, please choose 1 valid id [0/1/2/3]\n");
                return -EFAULT;
        }

	/* Verify does it really work now */
	if (!apm82181_test_dma(apm82181_dma_tchan[val])) {
		/* APM82181 RAID-5 memcpy has been activated successfully */;
		printk("APM82181 PLBDMA MEMCPY channel %d has been verified "
			"successfully\n", val);
		apm82181_memcpy_verified[val] = 1;
	} else {
		/* APM82181 RAID-5 memcpy hasn't been activated! */;
		printk("APM82181 PLBDMA MEMCPY channel %d hasn't been "
			"verified yet\n", val);
		apm82181_memcpy_verified[val] = 0;
	}

	return count;
}

static int __init apm82181_adma_per_chan_init (void)
{
        int rval;
	rval = of_register_platform_driver(&apm82181_dma_per_chan_driver);
	return rval;
}

static int __init apm82181_adma_init (void)
{
	int rval;
	struct proc_dir_entry *p;

	rval = of_register_platform_driver(&apm82181_pdma_driver);

	if (rval == 0) {
		/* Create /proc entries */
		apm82181_proot = proc_mkdir(APM82181_DMA_PROC_ROOT, NULL);
		if (!apm82181_proot) {
			printk(KERN_ERR "%s: failed to create %s proc "
			    "directory\n",__FUNCTION__,APM82181_DMA_PROC_ROOT);
			/* User will not be able to enable h/w RAID-6 */
			return rval;
		}

		/* ADMA MEMCPY verification entry */
		p = create_proc_entry("adma_memcopy_test", 0, apm82181_proot);
		if (p) {
			p->read_proc = apm82181_dma_read;
			p->write_proc = apm82181_dma_write;
		}
		/* ADMA XOR capability verification entry */
		p = create_proc_entry("adma_xor_test", 0, apm82181_proot);
		if (p) {
			p->read_proc = apm82181_xor_read;
			p->write_proc = apm82181_xor_write;
		}
	}
	return rval;
}

#if 0
static void __exit apm82181_adma_exit (void)
{
	of_unregister_platform_driver(&apm82181_pdma_driver);
	return;
}
module_exit(apm82181_adma_exit);
#endif

module_init(apm82181_adma_per_chan_init);
module_init(apm82181_adma_init);

MODULE_AUTHOR("Tai Tri Nguyen<ttnguyen@appliedmicro.com>");
MODULE_DESCRIPTION("APM82181 ADMA Engine Driver");
MODULE_LICENSE("GPL");
