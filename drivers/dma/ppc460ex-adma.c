/*
 * Copyright(c) 2006 DENX Engineering. All rights reserved.
 *
 * Author: Yuri Tikhonov <yur@emcraft.com>
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
 *  This driver supports the asynchrounous DMA copy and RAID engines available
 * on the AMCC PPC460ex Processors.
 *  Based on the Intel Xscale(R) family of I/O Processors (IOP 32x, 33x, 134x)
 * ADMA driver written by D.Williams.
 */

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
#include <asm/ppc460ex_adma.h>
#include <asm/ppc460ex_xor.h>
#include <asm/ppc4xx_ocm.h>
/* The list of channels exported by ppc460ex ADMA */
struct list_head
ppc_adma_chan_list = LIST_HEAD_INIT(ppc_adma_chan_list);

/* This flag is set when want to refetch the xor chain in the interrupt
 *	handler
 */
static u32 do_xor_refetch = 0;

/* Pointers to last submitted to DMA0, DMA1 CDBs */
static ppc460ex_desc_t *chan_last_sub[3];
static ppc460ex_desc_t *chan_first_cdb[3];

/* Pointer to last linked and submitted xor CB */
static ppc460ex_desc_t *xor_last_linked = NULL;
static ppc460ex_desc_t *xor_last_submit = NULL;

/* This array is used in data-check operations for storing a pattern */
static char ppc460ex_qword[16];

/* Since RXOR operations use the common register (MQ0_CF2H) for setting-up
 * the block size in transactions, then we do not allow to activate more than
 * only one RXOR transactions simultaneously. So use this var to store
 * the information about is RXOR currently active (PPC460EX_RXOR_RUN bit is
 * set) or not (PPC460EX_RXOR_RUN is clear).
 */
static unsigned long ppc460ex_rxor_state;

/* /proc interface is used here to enable the h/w RAID-6 capabilities
 */
static struct proc_dir_entry *ppc460ex_proot;
static struct proc_dir_entry *ppc460ex_pqroot;

/* These are used in enable & check routines
 */
static u32 ppc460ex_r6_enabled;
static u32 ppc460ex_r5_enabled;
static ppc460ex_ch_t *ppc460ex_r6_tchan;
static ppc460ex_ch_t *ppc460ex_r5_tchan;
static struct completion ppc460ex_r6_test_comp;
static struct completion ppc460ex_r5_test_comp;

static int ppc460ex_adma_dma2rxor_prep_src (ppc460ex_desc_t *desc,
		ppc460ex_rxor_cursor_t *cursor, int index,
		int src_cnt, u32 addr);
static void ppc460ex_adma_dma2rxor_set_src (ppc460ex_desc_t *desc,
		int index, dma_addr_t addr);
static void ppc460ex_adma_dma2rxor_set_mult (ppc460ex_desc_t *desc,
		int index, u8 mult);
#if 1
static inline  void pr_dma(int x, char *str)
{
	if(mfdcr(0x60)) {
		printk("<%s>  Line:%d\n",str,x);
	}
}
#else
static inline  void pr_dma(int x, char *str)
{
}
#endif
phys_addr_t fixup_bigphys_addr(phys_addr_t addr, phys_addr_t size)
{
	phys_addr_t page_4gb = 0;
	 
	return (page_4gb | addr);
}
/***********************************************************************
 * HW specific initialization
 * ********************************************************************/
static u64 ppc460ex_adma_dmamask = DMA_32BIT_MASK;

/* DMA and XOR platform devices' resources */

static struct resource ppc460ex_dma_1_resources[] = {
	{
		.flags = IORESOURCE_MEM,
	},
	{
		.start = DMA1_CS_FIFO_NEED_SERVICE_IRQ,
		.end = DMA1_CS_FIFO_NEED_SERVICE_IRQ,
		.flags = IORESOURCE_IRQ
	},
	{
		.start = DMA_ERROR_IRQ,
		.end = DMA_ERROR_IRQ,
		.flags = IORESOURCE_IRQ
	}
};


/* DMA and XOR platform devices' data */

/* DMA0,1 engines use FIFO to maintain CDBs, so we
 * should allocate the pool accordingly to size of this
 * FIFO. Thus, the pool size depends on the FIFO depth:
 * how much CDBs pointers FIFO may contaun then so much
 * CDBs we should provide in pool.
 * That is
 *   CDB size = 32B;
 *   CDBs number = (DMA0_FIFO_SIZE >> 3);
 *   Pool size = CDBs number * CDB size = 
 *      = (DMA0_FIFO_SIZE >> 3) << 5 = DMA0_FIFO_SIZE << 2.
 *
 *  As far as the XOR engine is concerned, it does not
 * use FIFOs but uses linked list. So there is no dependency
 * between pool size to allocate and the engine configuration.
 */

static struct ppc460ex_adma_platform_data ppc460ex_dma_1_data = {
	.hw_id  = PPC460EX_DMA1_ID,
	.pool_size = DMA1_FIFO_SIZE << 2,
};

/* DMA and XOR platform devices definitions */
#if 1
static struct platform_device ppc460ex_dma_1_channel = {
	.name = "PPC460EX-ADMA",
	.id = PPC460EX_DMA1_ID,
	.num_resources = ARRAY_SIZE(ppc460ex_dma_1_resources),
	.resource = ppc460ex_dma_1_resources,
	.dev = {
		.dma_mask = &ppc460ex_adma_dmamask,
		.coherent_dma_mask = DMA_64BIT_MASK,
		.platform_data = (void *) &ppc460ex_dma_1_data,
	},
};
#endif

/*
 *  Init DMA0/1 and XOR engines; allocate memory for DMAx FIFOs; set platform_device
 * memory resources addresses
 */
static void ppc460ex_configure_raid_devices(void)
{
	void *fifo_buf;
	volatile i2o_regs_t *i2o_reg;
	volatile dma_regs_t  *dma_reg1;
	/*
	 * volatile dma_regs_t *dma_reg0, *dma_reg1;
	   volatile xor_regs_t *xor_reg;
	*/
	u32 mask;

	/*
	 * Map registers and allocate fifo buffer
	 */
	if (!(i2o_reg  = ioremap(I2O_MMAP_BASE, I2O_MMAP_SIZE))) {
		printk(KERN_ERR "I2O registers mapping failed.\n");
		return;
	}
	if (!(dma_reg1 = ioremap(DMA1_MMAP_BASE, DMA_MMAP_SIZE))) {
		printk(KERN_ERR "DMA1 registers mapping failed.\n");
		goto err1;
	}

	/*  Provide memory regions for DMA's FIFOs: I2O, DMA0 and DMA1 share
	 * the base address of FIFO memory space.
	 *  Actually we need twice more physical memory than programmed in the
	 * <fsiz> register (because there are two FIFOs foreach DMA: CP and CS)
	 */
	fifo_buf = kmalloc(( DMA1_FIFO_SIZE)<<1, GFP_KERNEL);
	if (!fifo_buf) {
		printk(KERN_ERR "DMA FIFO buffer allocating failed.\n");
		goto err2;
	}

	/*
	 * Configure h/w
	 */
	/* Reset I2O/DMA */
	SDR_WRITE(SDR0_SRST0, SDR0_SRST_I2ODMA);
	SDR_WRITE(SDR0_SRST0, 0);


	/* Setup the base address of mmaped registers */
	mtdcr(DCRN_I2O0_IBAH, (u32)(I2O_MMAP_BASE >> 32));
	mtdcr(DCRN_I2O0_IBAL, (u32)(I2O_MMAP_BASE) | I2O_REG_ENABLE);

	/* SetUp FIFO memory space base address */
	out_le32(&i2o_reg->ifbah, 0);
	out_le32(&i2o_reg->ifbal, ((u32)__pa(fifo_buf)));

	/* set zero FIFO size for I2O, so the whole fifo_buf is used by DMAs.
	 * DMA0_FIFO_SIZE is defined in bytes, <fsiz> - in number of CDB pointers (8byte).
	 * DMA FIFO Length = CSlength + CPlength, where
	 *  CSlength = CPlength = (fsiz + 1) * 8.
	 */
	out_le32(&i2o_reg->ifsiz, 0);
	out_le32(&dma_reg1->fsiz, DMA_FIFO_ENABLE | ((DMA1_FIFO_SIZE>>3) - 2));
	/* Configure DMA engine */
	out_le32(&dma_reg1->cfg, DMA_CFG_DXEPR_HP | DMA_CFG_DFMPP_HP | DMA_CFG_FALGN);

	/* Clear Status */
	out_le32(&dma_reg1->dsts, ~0);

	/*
	 * Prepare WXOR/RXOR (finally it is being enabled via /proc interface of
	 * the ppc460ex ADMA driver)
	 */
	/* Set HB alias */
	mtdcr(DCRN_MQ0_BAUH, DMA_CUED_XOR_HB);

	/* Set:
	 * - LL transaction passing limit to 1;
	 * - Memory controller cycle limit to 1;
	 * - Galois Polynomial to 0x14d (default)
	 */
	mtdcr(DCRN_MQ0_CFBHL, 0x88a68000 | (1 << MQ0_CFBHL_TPLM) |
			      (1 << MQ0_CFBHL_HBCL) |
			      (PPC460EX_DEFAULT_POLY << MQ0_CFBHL_POLY));

	/* Unmask 'CS FIFO Attention' interrupts and
	 * enable generating interrupts on errors
	 */
	mask = in_le32(&i2o_reg->iopim) & ~(
		I2O_IOPIM_P0SNE | I2O_IOPIM_P1SNE |
		I2O_IOPIM_P0EM | I2O_IOPIM_P1EM);
	out_le32(&i2o_reg->iopim, mask);

	/* enable XOR engine interrupts */

	/*
	 * Unmap I2O registers
	 */
	iounmap(i2o_reg);
	printk("<%s> line %d\n", __FUNCTION__, __LINE__);

	/* Configure MQ as follows:
	 * MQ: 0x80001C80. This means
	 * - AddrAck First Request,
	 * - Read Passing Limit = 1,
	 * - Read Passing Enable,
	 * - Read Flow Through Enable,
	 * - MCIF Cycle Limit = 1.
	 */
#if 1
	mdelay(1000);
	mask = (1 << MQ_CF1_AAFR) | ((1 & MQ_CF1_RPLM_MSK) << MQ_CF1_RPLM) |
	       (1 << MQ_CF1_RPEN) | (1 << MQ_CF1_RFTE) |
	       ((1 & MQ_CF1_WRCL_MSK) << MQ_CF1_WRCL);
	mtdcr(DCRN_MQ0_CF1H, mask);
	mtdcr(DCRN_MQ0_CF1L, mask);
#endif
	printk("<%s> line %d\n", __FUNCTION__, __LINE__);

	/* Configure PLB as follows:
	 * PLB: 0xDF000000. This means
	 * - Priority level 00 fair priority,
	 * - Priority level 01 fair priority,
	 * - Priority level 11 fair priority,
	 * - High Bus Utilization enabled,
	 * - 4 Deep read pipe,
	 * - 2 Deep write pipe.
	 */
	mask = (1 << PLB_ACR_PPM0) | (1 << PLB_ACR_PPM1) | (1 << PLB_ACR_PPM3) |
	       (1 << PLB_ACR_HBU) | ((3 & PLB_ACR_RDP_MSK) << PLB_ACR_RDP) |
	       (1 << PLB_ACR_WRP);
	mtdcr(DCRN_PLB0_ACR, mask);
	mtdcr(DCRN_PLB1_ACR, mask);
	printk("<%s> line %d\n", __FUNCTION__, __LINE__);

	/*
	 * Set resource addresses
	 */

	ppc460ex_dma_1_channel.resource[0].start = (resource_size_t)(dma_reg1);
	ppc460ex_dma_1_channel.resource[0].end =
		ppc460ex_dma_1_channel.resource[0].start+DMA_MMAP_SIZE;
	printk( " ppc460ex_dma_1_channel.resource[0].start=0x%lx \n",
			ppc460ex_dma_1_channel.resource[0].start);
	printk("<%s> line %d dma_reg1=0x%lx \n", __FUNCTION__, __LINE__,dma_reg1);


	printk("<%s> line %d\n", __FUNCTION__, __LINE__);
	return;
err2:
	iounmap(dma_reg1);
err1:
	iounmap(i2o_reg);
	return;
}
#if 1
static struct platform_device *ppc460ex_devs[] __initdata = {
/*	&ppc460ex_dma_0_channel, */
	&ppc460ex_dma_1_channel,
	/*&ppc460ex_xor_channel, */
};
#endif

/******************************************************************************
 * Command (Descriptor) Blocks low-level routines
 ******************************************************************************/
/**
 * ppc460ex_desc_init_interrupt - initialize the descriptor for INTERRUPT
 * pseudo operation
 */
static inline void ppc460ex_desc_init_interrupt (ppc460ex_desc_t *desc,
							ppc460ex_ch_t *chan)
{
	xor_cb_t *p;

	switch (chan->device->id) {
	case PPC460EX_XOR_ID:
		p = desc->hw_desc;
		memset (desc->hw_desc, 0, sizeof(xor_cb_t));
		/* NOP with Command Block Complete Enable */
		p->cbc = XOR_CBCR_CBCE_BIT;
		break;
	case PPC460EX_DMA0_ID:
	case PPC460EX_DMA1_ID:
		memset (desc->hw_desc, 0, sizeof(dma_cdb_t));
		/* NOP with interrupt */
		set_bit(PPC460EX_DESC_INT, &desc->flags);
		break;
	default:
		printk(KERN_ERR "Unsupported id %d in %s\n", chan->device->id,
				__FUNCTION__);
		break;
	}
}

/**
 * ppc460ex_desc_init_null_xor - initialize the descriptor for NULL XOR
 * pseudo operation
 */
static inline void ppc460ex_desc_init_null_xor(ppc460ex_desc_t *desc)
{
	memset (desc->hw_desc, 0, sizeof(xor_cb_t));
	desc->hw_next = NULL;
	desc->src_cnt = 0;
	desc->dst_cnt = 1;
}

/**
 * ppc460ex_desc_init_pqxor_xor - initialize the descriptor for PQ_XOR
 * operation in DMA2 controller
 */
static inline void ppc460ex_desc_init_dma2rxor(ppc460ex_desc_t *desc,
		int dst_cnt, int src_cnt, unsigned long flags)
{
	xor_cb_t *hw_desc = desc->hw_desc;

	memset (desc->hw_desc, 0, sizeof(xor_cb_t));
	desc->hw_next = NULL;
	desc->src_cnt = src_cnt;
	desc->dst_cnt = dst_cnt;
	memset (desc->reverse_flags, 0, sizeof (desc->reverse_flags));
	desc->descs_per_op = 0;

	hw_desc->cbc = XOR_CBCR_TGT_BIT;
	if (flags & DMA_PREP_INTERRUPT)
		/* Enable interrupt on complete */
		hw_desc->cbc |= XOR_CBCR_CBCE_BIT;
}

/**
 * ppc460ex_desc_init_pq - initialize the descriptor for PQ_XOR operation
 */
static inline void ppc460ex_desc_init_pq(ppc460ex_desc_t *desc,
		int dst_cnt, int src_cnt, unsigned long flags,
		unsigned long op)
{
	dma_cdb_t *hw_desc;
	ppc460ex_desc_t *iter;
	u8 dopc;


	/* Common initialization of a PQ descriptors chain */

	set_bits(op, &desc->flags);
	desc->src_cnt = src_cnt;
	desc->dst_cnt = dst_cnt;

	dopc = (desc->dst_cnt == DMA_DEST_MAX_NUM) ?
		DMA_CDB_OPC_MULTICAST : DMA_CDB_OPC_MV_SG1_SG2;

	list_for_each_entry(iter, &desc->group_list, chain_node) {
		hw_desc = iter->hw_desc;
		memset (iter->hw_desc, 0, sizeof(dma_cdb_t));

		if (likely(!list_is_last(&iter->chain_node,
				&desc->group_list))) {
			/* set 'next' pointer */
			iter->hw_next = list_entry(iter->chain_node.next,
				ppc460ex_desc_t, chain_node);
			clear_bit(PPC460EX_DESC_INT, &iter->flags);
		} else {
			/* this is the last descriptor.
			 * this slot will be pasted from ADMA level
			 * each time it wants to configure parameters
			 * of the transaction (src, dst, ...)
			 */
			iter->hw_next = NULL;
			if (flags & DMA_PREP_INTERRUPT)
				set_bit(PPC460EX_DESC_INT, &iter->flags);
			else
				clear_bit(PPC460EX_DESC_INT, &iter->flags);
		}
	}

	/* Set OPS depending on WXOR/RXOR type of operation */
	if (!test_bit(PPC460EX_DESC_RXOR, &desc->flags)) {
		/* This is a WXOR only chain:
		 * - first descriptors are for zeroing destinations
		 *   if PPC460EX_ZERO_P/Q set;
		 * - descriptors remained are for GF-XOR operations.
		 */
		iter = list_first_entry(&desc->group_list,
					ppc460ex_desc_t, chain_node);

		if (test_bit(PPC460EX_ZERO_P, &desc->flags)) {
			hw_desc = iter->hw_desc;
			hw_desc->opc = DMA_CDB_OPC_MV_SG1_SG2;
			iter = list_first_entry(&iter->chain_node,
					ppc460ex_desc_t, chain_node);
		}

		if (test_bit(PPC460EX_ZERO_Q, &desc->flags)) {
			hw_desc = iter->hw_desc;
			hw_desc->opc = DMA_CDB_OPC_MV_SG1_SG2;
			iter = list_first_entry(&iter->chain_node,
					ppc460ex_desc_t, chain_node);
		}

		list_for_each_entry_from(iter, &desc->group_list, chain_node) {
			hw_desc = iter->hw_desc;
			hw_desc->opc = dopc;
		}
	} else {
		/* This is either RXOR-only or mixed RXOR/WXOR */

		/* The first 1 or 2 slots in chain are always RXOR,
		 * if need to calculate P & Q, then there are two
		 * RXOR slots; if only P or only Q, then there is one
		 */
		iter = list_first_entry(&desc->group_list,
					ppc460ex_desc_t, chain_node);
		hw_desc = iter->hw_desc;
		hw_desc->opc = DMA_CDB_OPC_MV_SG1_SG2;

		if (desc->dst_cnt == DMA_DEST_MAX_NUM) {
			iter = list_first_entry(&iter->chain_node,
						ppc460ex_desc_t, chain_node);
			hw_desc = iter->hw_desc;
			hw_desc->opc = DMA_CDB_OPC_MV_SG1_SG2;
		}

		/* The remain descs (if any) are WXORs */
		if (test_bit(PPC460EX_DESC_WXOR, &desc->flags)) {
			iter = list_first_entry(&iter->chain_node,
						ppc460ex_desc_t, chain_node);
			list_for_each_entry_from(iter, &desc->group_list,
						chain_node) {
				hw_desc = iter->hw_desc;
				hw_desc->opc = dopc;
			}
		}
	}
}
void ppc460ex_desc_init_xor(ppc460ex_desc_t *desc,
		int dst_cnt, int src_cnt, unsigned long flags,
		unsigned long op)
{
	dma_cdb_t *hw_desc;
	ppc460ex_desc_t *iter;
	u8 dopc;


	/* Common initialization of a PQ descriptors chain */

	set_bits(op, &desc->flags);
	desc->src_cnt = src_cnt;
	desc->dst_cnt = dst_cnt;

	dopc = (desc->dst_cnt == DMA_DEST_MAX_NUM) ?
		DMA_CDB_OPC_MULTICAST : DMA_CDB_OPC_MV_SG1_SG2;

	list_for_each_entry(iter, &desc->group_list, chain_node) {
		hw_desc = iter->hw_desc;
		memset (iter->hw_desc, 0, sizeof(dma_cdb_t));

		if (likely(!list_is_last(&iter->chain_node,
				&desc->group_list))) {
			/* set 'next' pointer */
			iter->hw_next = list_entry(iter->chain_node.next,
				ppc460ex_desc_t, chain_node);
			clear_bit(PPC460EX_DESC_INT, &iter->flags);
		} else {
			/* this is the last descriptor.
			 * this slot will be pasted from ADMA level
			 * each time it wants to configure parameters
			 * of the transaction (src, dst, ...)
			 */
			iter->hw_next = NULL;
			if (flags & DMA_PREP_INTERRUPT)
				set_bit(PPC460EX_DESC_INT, &iter->flags);
			else
				clear_bit(PPC460EX_DESC_INT, &iter->flags);
		}
	}

	/* Set OPS depending on WXOR/RXOR type of operation */
	if (!test_bit(PPC460EX_DESC_RXOR, &desc->flags)) {
		/* This is a WXOR only chain:
		 * - first descriptors are for zeroing destinations
		 *   if PPC460EX_ZERO_P/Q set;
		 * - descriptors remained are for GF-XOR operations.
		 */
		iter = list_first_entry(&desc->group_list,
					ppc460ex_desc_t, chain_node);

		if (test_bit(PPC460EX_ZERO_P, &desc->flags)) {
			hw_desc = iter->hw_desc;
			hw_desc->opc = DMA_CDB_OPC_MV_SG1_SG2;
			iter = list_first_entry(&iter->chain_node,
					ppc460ex_desc_t, chain_node);
		}


		list_for_each_entry_from(iter, &desc->group_list, chain_node) {
			hw_desc = iter->hw_desc;
			hw_desc->opc = dopc;
		}
	} else {
		/* This is either RXOR-only or mixed RXOR/WXOR */

		/* The first 1 or 2 slots in chain are always RXOR,
		 * if need to calculate P & Q, then there are two
		 * RXOR slots; if only P or only Q, then there is one
		 */
		iter = list_first_entry(&desc->group_list,
					ppc460ex_desc_t, chain_node);
		hw_desc = iter->hw_desc;
		hw_desc->opc = DMA_CDB_OPC_MV_SG1_SG2;

		if (desc->dst_cnt == DMA_DEST_MAX_NUM) {
			iter = list_first_entry(&iter->chain_node,
						ppc460ex_desc_t, chain_node);
			hw_desc = iter->hw_desc;
			hw_desc->opc = DMA_CDB_OPC_MV_SG1_SG2;
		}

		/* The remain descs (if any) are WXORs */
		if (test_bit(PPC460EX_DESC_WXOR, &desc->flags)) {
			iter = list_first_entry(&iter->chain_node,
						ppc460ex_desc_t, chain_node);
			list_for_each_entry_from(iter, &desc->group_list,
						chain_node) {
				hw_desc = iter->hw_desc;
				hw_desc->opc = dopc;
			}
		}
	}
}
/**
 * ppc460ex_desc_init_dma01_xor - initialize the descriptor for P_XOR operation
 */
static inline void ppc460ex_desc_init_dma01_xor(ppc460ex_desc_t *desc,
		int dst_cnt, int src_cnt, unsigned long flags,
		unsigned long op)
{
	dma_cdb_t *hw_desc;
	ppc460ex_desc_t *iter;

	/* Common initialization of a XOR descriptors chain */

	set_bits(op, &desc->flags);
	desc->src_cnt = src_cnt;
	desc->dst_cnt = dst_cnt;

	list_for_each_entry(iter, &desc->group_list, chain_node) {
		hw_desc = iter->hw_desc;
		memset (iter->hw_desc, 0, sizeof(dma_cdb_t));

		if (likely(!list_is_last(&iter->chain_node,
				&desc->group_list))) {
			/* set 'next' pointer */
			iter->hw_next = list_entry(iter->chain_node.next,
				ppc460ex_desc_t, chain_node);
			clear_bit(PPC460EX_DESC_INT, &iter->flags);
		} else {
			/* this is the last descriptor.
			 * this slot will be pasted from ADMA level
			 * each time it wants to configure parameters
			 * of the transaction (src, dst, ...)
			 */
			iter->hw_next = NULL;
			if (flags & DMA_PREP_INTERRUPT)
				set_bit(PPC460EX_DESC_INT, &iter->flags);
			else
				clear_bit(PPC460EX_DESC_INT, &iter->flags);
		}
	}

	/* Set OPS depending on WXOR/RXOR type of operation */
	if (!test_bit(PPC460EX_DESC_RXOR, &desc->flags)) {
		/* This is a WXOR only chain:
		 * - first <dst_cnt> descriptors are for zeroing destinations
		 *	if PPC460EX_ZERO_P is set;
		 * - descriptors remained are for GF-XOR operations.
		 */
		iter = list_first_entry(&desc->group_list,
				ppc460ex_desc_t, chain_node);
			
		if (dst_cnt && test_bit(PPC460EX_ZERO_P,
					&desc->flags)) {
			/* MV_SG1_SG2 to zero P or Q if this is
			* just PQ_XOR operation and MV_SG1_SG2
			* if only Q has to be calculated
			*/
			hw_desc = iter->hw_desc;
			hw_desc->opc = DMA_CDB_OPC_MV_SG1_SG2;
			iter = list_first_entry(&iter->chain_node,
					ppc460ex_desc_t, chain_node);
		} 
		list_for_each_entry(iter, &desc->group_list, chain_node) {
			hw_desc = iter->hw_desc;
			if (desc->dst_cnt == DMA_DEST_MAX_NUM)
				hw_desc->opc = DMA_CDB_OPC_MULTICAST;
			else
				hw_desc->opc = DMA_CDB_OPC_MV_SG1_SG2;
		}
	} else {
		/* This is either RXOR-only or mixed RXOR/WXOR
		 * The first slot in chain is always RXOR,
		 * the slots remained (if there are) are WXOR
		 */
		list_for_each_entry(iter, &desc->group_list, chain_node) {
			hw_desc = iter->hw_desc;
			/* No DMA_CDB_OPC_MULTICAST option for RXOR */
			hw_desc->opc = DMA_CDB_OPC_MV_SG1_SG2;
		}
	}
}

/**
 * ppc460ex_desc_init_pqzero_sum - initialize the descriptor for PQ_VAL
 *	operation
 */
static inline void ppc460ex_desc_init_pqzero_sum(ppc460ex_desc_t *desc,
		int dst_cnt, int src_cnt)
{
	dma_cdb_t *hw_desc;
	ppc460ex_desc_t *iter;
	int i = 0;

	/* initialize each descriptor in chain */
	list_for_each_entry(iter, &desc->group_list, chain_node) {
		hw_desc = iter->hw_desc;
		memset (iter->hw_desc, 0, sizeof(dma_cdb_t));

		/* This is a PQ_VAL operation:
		 * - first <dst_cnt> descriptors are for GF-XOR operations;
		 * - <dst_cnt> descriptors remained are for checking the result.
		 */
		if (i++ < src_cnt)
			/* MV_SG1_SG2 if only Q is being verified
			 * MULTICAST if both P and Q are being verified
			 */
			hw_desc->opc = (dst_cnt == DMA_DEST_MAX_NUM) ?
				DMA_CDB_OPC_MULTICAST : DMA_CDB_OPC_MV_SG1_SG2;
		else
			/* DMA_CDB_OPC_DCHECK128 operation */
			hw_desc->opc = DMA_CDB_OPC_DCHECK128;

		if (likely(!list_is_last(&iter->chain_node,
				&desc->group_list))) {
			/* set 'next' pointer */
			iter->hw_next = list_entry(iter->chain_node.next,
				ppc460ex_desc_t, chain_node);
		} else {
			/* this is the last descriptor.
		 	 * this slot will be pasted from ADMA level
			 * each time it wants to configure parameters
			 * of the transaction (src, dst, ...)
			 */
			iter->hw_next = NULL;
			/* always enable interrupt generating since we get
			 * the status of pqzero from the handler
			 */
			set_bit(PPC460EX_DESC_INT, &iter->flags);
		}
	}
	desc->src_cnt = src_cnt;
	desc->dst_cnt = dst_cnt;
}

/**
 * ppc460ex_desc_init_memcpy - initialize the descriptor for MEMCPY operation
 */
static inline void ppc460ex_desc_init_memcpy(ppc460ex_desc_t *desc,
		unsigned long flags)
{
	dma_cdb_t *hw_desc = desc->hw_desc;

	memset (desc->hw_desc, 0, sizeof(dma_cdb_t));
	desc->hw_next = NULL;
	desc->src_cnt = 1;
	desc->dst_cnt = 1;

	if (flags & DMA_PREP_INTERRUPT)
		set_bit(PPC460EX_DESC_INT, &desc->flags);
	else
		clear_bit(PPC460EX_DESC_INT, &desc->flags);

	hw_desc->opc = DMA_CDB_OPC_MV_SG1_SG2;
}

/**
 * ppc460ex_desc_init_memset - initialize the descriptor for MEMSET operation
 */
static inline void ppc460ex_desc_init_memset(ppc460ex_desc_t *desc, int value,
		unsigned long flags)
{
	dma_cdb_t *hw_desc = desc->hw_desc;

	memset (desc->hw_desc, 0, sizeof(dma_cdb_t));
	desc->hw_next = NULL;
	desc->src_cnt = 1;
	desc->dst_cnt = 1;

	if (flags & DMA_PREP_INTERRUPT)
		set_bit(PPC460EX_DESC_INT, &desc->flags);
	else
		clear_bit(PPC460EX_DESC_INT, &desc->flags);

	hw_desc->sg1u = hw_desc->sg1l = cpu_to_le32((u32)value);
	hw_desc->sg3u = hw_desc->sg3l = cpu_to_le32((u32)value);
	hw_desc->opc = DMA_CDB_OPC_DFILL128;
}

/**
 * ppc460ex_desc_set_src_addr - set source address into the descriptor
 */
static inline void ppc460ex_desc_set_src_addr( ppc460ex_desc_t *desc,
					ppc460ex_ch_t *chan, int src_idx,
					dma_addr_t addrh, dma_addr_t addrl)
{
	dma_cdb_t *dma_hw_desc;
	phys_addr_t addr64, tmplow, tmphi;

	switch (chan->device->id) {
	case PPC460EX_DMA0_ID:
	case PPC460EX_DMA1_ID:
		if (!addrh) {
			addr64 = fixup_bigphys_addr(addrl, sizeof(phys_addr_t));
			tmphi = (addr64 >> 32);
			tmplow = (addr64 & 0xFFFFFFFF);
		} else {
			tmphi = addrh;
			tmplow = addrl;
		}
		dma_hw_desc = desc->hw_desc;
		dma_hw_desc->sg1l = cpu_to_le32((u32)tmplow);
		dma_hw_desc->sg1u = cpu_to_le32((u32)tmphi);
		break;
	}
}

/**
 * ppc460ex_desc_set_src_mult - set source address mult into the descriptor
 */
static inline void ppc460ex_desc_set_src_mult( ppc460ex_desc_t *desc,
			ppc460ex_ch_t *chan, u32 mult_index, int sg_index,
			unsigned char mult_value)
{
	dma_cdb_t *dma_hw_desc;
	xor_cb_t *xor_hw_desc;
	u32 *psgu;

	switch (chan->device->id) {
	case PPC460EX_DMA0_ID:
	case PPC460EX_DMA1_ID:
		dma_hw_desc = desc->hw_desc;

		switch(sg_index){
		/* for RXOR operations set multiplier
		 * into source cued address
		 */
		case DMA_CDB_SG_SRC:
			psgu = &dma_hw_desc->sg1u;
			break;
		/* for WXOR operations set multiplier
		 * into destination cued address(es)
		 */
		case DMA_CDB_SG_DST1:
			psgu = &dma_hw_desc->sg2u;
			break;
		case DMA_CDB_SG_DST2:
			psgu = &dma_hw_desc->sg3u;
			break;
		default:
			BUG();
		}

		*psgu |= cpu_to_le32(mult_value << mult_index);
		if(mfdcr(0x60) == 0xfee8) {
			printk("Line--%d mult_value = 0x%x mult_index=0x%x *psgu=0x%x\n",__LINE__, mult_value,mult_index,*psgu);
		}
		*psgu |= cpu_to_le32( 1 << mult_index);
		if(mfdcr(0x60) == 0xfee8) {
			printk("Line--%d mult_value = 0x%x mult_index=0x%x *psgu=0x%x\n",__LINE__, mult_value,mult_index,*psgu);
		}
		break;
	case PPC460EX_XOR_ID:
		xor_hw_desc = desc->hw_desc;
		break;
	default:
		BUG();
	}
}

/**
 * ppc460ex_desc_set_dest_addr - set destination address into the descriptor
 */
static inline void ppc460ex_desc_set_dest_addr(ppc460ex_desc_t *desc,
				ppc460ex_ch_t *chan,
				dma_addr_t addrh, dma_addr_t addrl,
				u32 dst_idx)
{
	dma_cdb_t *dma_hw_desc;
	xor_cb_t *xor_hw_desc;
	phys_addr_t addr64, tmphi, tmplow;
	u32 *psgu, *psgl;

	switch (chan->device->id) {
	case PPC460EX_DMA0_ID:
	case PPC460EX_DMA1_ID:
		if (!addrh) {
			addr64 = fixup_bigphys_addr(addrl, sizeof(phys_addr_t));
			tmphi = (addr64 >> 32);
			tmplow = (addr64 & 0xFFFFFFFF);
		} else {
			tmphi = addrh;
			tmplow = addrl;
		}
		dma_hw_desc = desc->hw_desc;

		psgu = dst_idx ? &dma_hw_desc->sg3u : &dma_hw_desc->sg2u;
		psgl = dst_idx ? &dma_hw_desc->sg3l : &dma_hw_desc->sg2l;

		*psgl = cpu_to_le32((u32)tmplow);
		*psgu |= cpu_to_le32((u32)tmphi);
		break;
	case PPC460EX_XOR_ID:
		xor_hw_desc = desc->hw_desc;
		xor_hw_desc->cbtal = addrl;
		xor_hw_desc->cbtah = 0;
		break;
	}
}

/**
 * ppc460ex_desc_set_byte_count - set number of data bytes involved
 * into the operation
 */
static inline void ppc460ex_desc_set_byte_count(ppc460ex_desc_t *desc,
					ppc460ex_ch_t *chan, u32 byte_count)
{
	dma_cdb_t *dma_hw_desc;

	switch (chan->device->id) {
	case PPC460EX_DMA0_ID:
	case PPC460EX_DMA1_ID:
		dma_hw_desc = desc->hw_desc;
		dma_hw_desc->cnt = cpu_to_le32(byte_count);
		break;
	}
}

/**
 * ppc460ex_desc_set_rxor_block_size - set RXOR block size
 */
static inline void ppc460ex_desc_set_rxor_block_size(u32 byte_count)
{
	/* assume that byte_count is aligned on the 512-boundary;
	 * thus write it directly to the register (bits 23:31 are
	 * reserved there).
	 */
	mtdcr(DCRN_MQ0_CF2H, byte_count);
}

/**
 * ppc460ex_desc_set_dcheck - set CHECK pattern
 */
static inline void ppc460ex_desc_set_dcheck(ppc460ex_desc_t *desc,
						ppc460ex_ch_t *chan, u8 *qword)
{
	dma_cdb_t *dma_hw_desc;

	switch (chan->device->id) {
	case PPC460EX_DMA0_ID:
	case PPC460EX_DMA1_ID:
		dma_hw_desc = desc->hw_desc;
		out_le32(&dma_hw_desc->sg3l, qword[0]);
		out_le32(&dma_hw_desc->sg3u, qword[4]);
		out_le32(&dma_hw_desc->sg2l, qword[8]);
		out_le32(&dma_hw_desc->sg2u, qword[12]);
		break;
	default:
		BUG();
	}
}

/**
 * ppc460ex_xor_set_link - set link address in xor CB
 */
static inline void ppc460ex_xor_set_link (ppc460ex_desc_t *prev_desc,
						ppc460ex_desc_t *next_desc)
{
	xor_cb_t *xor_hw_desc = prev_desc->hw_desc;

	if (unlikely(!next_desc || !(next_desc->phys))) {
		printk(KERN_ERR "%s: next_desc=0x%p; next_desc->phys=0x%x\n",
			__FUNCTION__, next_desc,
			next_desc ? next_desc->phys : 0);
		BUG();
	}

	xor_hw_desc->cbs = 0;
	xor_hw_desc->cblal = next_desc->phys;
	xor_hw_desc->cblah = 0;
	xor_hw_desc->cbc |= XOR_CBCR_LNK_BIT;
}

/**
 * ppc460ex_desc_set_link - set the address of descriptor following this
 * descriptor in chain
 */
static inline void ppc460ex_desc_set_link(ppc460ex_ch_t *chan,
		ppc460ex_desc_t *prev_desc, ppc460ex_desc_t *next_desc)
{
	unsigned long flags;
	ppc460ex_desc_t *tail = next_desc;

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
	case PPC460EX_DMA0_ID:
	case PPC460EX_DMA1_ID:
		break;
	case PPC460EX_XOR_ID:
		/* bind descriptor to the chain */
		while (tail->hw_next)
			tail = tail->hw_next;
		xor_last_linked = tail;

		if (prev_desc == xor_last_submit)
			/* do not link to the last submitted CB */
			break;
		ppc460ex_xor_set_link (prev_desc, next_desc);
		break;
	}

	local_irq_restore(flags);
}

/**
 * ppc460ex_desc_get_src_addr - extract the source address from the descriptor
 */
static inline u32 ppc460ex_desc_get_src_addr(ppc460ex_desc_t *desc,
					ppc460ex_ch_t *chan, int src_idx)
{
	dma_cdb_t *dma_hw_desc;
	xor_cb_t *xor_hw_desc;

	switch (chan->device->id) {
	case PPC460EX_DMA0_ID:
	case PPC460EX_DMA1_ID:
		dma_hw_desc = desc->hw_desc;
		/* May have 0, 1, 2, or 3 sources */
		switch (dma_hw_desc->opc) {
		case DMA_CDB_OPC_NO_OP:
		case DMA_CDB_OPC_DFILL128:
			return 0;
		case DMA_CDB_OPC_DCHECK128:
			if (unlikely(src_idx)) {
				printk(KERN_ERR "%s: try to get %d source for"
				    " DCHECK128\n", __FUNCTION__, src_idx);
				BUG();
			}
			return le32_to_cpu(dma_hw_desc->sg1l);
		case DMA_CDB_OPC_MULTICAST:
		case DMA_CDB_OPC_MV_SG1_SG2:
			if (unlikely(src_idx > 2)) {
				printk(KERN_ERR "%s: try to get %d source from"
				    " DMA descr\n", __FUNCTION__, src_idx);
				BUG();
			}
			if (src_idx) {
				if (le32_to_cpu(dma_hw_desc->sg1u) &
				    DMA_CUED_XOR_WIN_MSK) {
					u8 region;

					if (src_idx == 1)
						return le32_to_cpu(
						    dma_hw_desc->sg1l) +
							desc->unmap_len;

					region = (le32_to_cpu(
					    dma_hw_desc->sg1u)) >>
						DMA_CUED_REGION_OFF;

					region &= DMA_CUED_REGION_MSK;
					switch (region) {
					case DMA_RXOR123:
						return le32_to_cpu(
						    dma_hw_desc->sg1l) +
							(desc->unmap_len << 1);
					case DMA_RXOR124:
						return le32_to_cpu(
						    dma_hw_desc->sg1l) +
							(desc->unmap_len * 3);
					case DMA_RXOR125:
						return le32_to_cpu(
						    dma_hw_desc->sg1l) +
							(desc->unmap_len << 2);
					default:
						printk (KERN_ERR
						    "%s: try to"
						    " get src3 for region %02x"
						    "PPC460EX_DESC_RXOR12?\n",
						    __FUNCTION__, region);
						BUG();
					}
				} else {
					printk(KERN_ERR
						"%s: try to get %d"
						" source for non-cued descr\n",
						__FUNCTION__, src_idx);
					BUG();
				}
			}
			return le32_to_cpu(dma_hw_desc->sg1l);
		default:
			printk(KERN_ERR "%s: unknown OPC 0x%02x\n",
				__FUNCTION__, dma_hw_desc->opc);
			BUG();
		}
		return le32_to_cpu(dma_hw_desc->sg1l);
	case PPC460EX_XOR_ID:
		/* May have up to 16 sources */
		xor_hw_desc = desc->hw_desc;
		return xor_hw_desc->ops[src_idx].l;
	}
	return 0;
}

/**
 * ppc460ex_desc_get_dest_addr - extract the destination address from the
 * descriptor
 */
static inline u32 ppc460ex_desc_get_dest_addr(ppc460ex_desc_t *desc,
		ppc460ex_ch_t *chan, int idx)
{
	dma_cdb_t *dma_hw_desc;
	xor_cb_t *xor_hw_desc;

	switch (chan->device->id) {
	case PPC460EX_DMA0_ID:
	case PPC460EX_DMA1_ID:
		dma_hw_desc = desc->hw_desc;

		if (likely(!idx))
			return le32_to_cpu(dma_hw_desc->sg2l);
		return le32_to_cpu(dma_hw_desc->sg3l);
	case PPC460EX_XOR_ID:
		xor_hw_desc = desc->hw_desc;
		return xor_hw_desc->cbtal;
	}
	return 0;
}

/**
 * ppc460ex_desc_get_byte_count - extract the byte count from the descriptor
 */
static inline u32 ppc460ex_desc_get_byte_count(ppc460ex_desc_t *desc,
		ppc460ex_ch_t *chan)
{
	dma_cdb_t *dma_hw_desc;
	xor_cb_t *xor_hw_desc;

	switch (chan->device->id) {
	case PPC460EX_DMA0_ID:
	case PPC460EX_DMA1_ID:
		dma_hw_desc = desc->hw_desc;
		return le32_to_cpu(dma_hw_desc->cnt);
	case PPC460EX_XOR_ID:
		xor_hw_desc = desc->hw_desc;
		return xor_hw_desc->cbbc;
	default:
		BUG();
	}
	return 0;
}

/**
 * ppc460ex_desc_get_src_num - extract the number of source addresses from
 * the descriptor
 */
static inline u32 ppc460ex_desc_get_src_num(ppc460ex_desc_t *desc,
		ppc460ex_ch_t *chan)
{
	dma_cdb_t *dma_hw_desc;
	xor_cb_t *xor_hw_desc;

	switch (chan->device->id) {
	case PPC460EX_DMA0_ID:
	case PPC460EX_DMA1_ID:
		dma_hw_desc = desc->hw_desc;

		switch (dma_hw_desc->opc) {
		case DMA_CDB_OPC_NO_OP:
		case DMA_CDB_OPC_DFILL128:
			return 0;
		case DMA_CDB_OPC_DCHECK128:
			return 1;
		case DMA_CDB_OPC_MV_SG1_SG2:
		case DMA_CDB_OPC_MULTICAST:
			/*
			 * Only for RXOR operations we have more than
			 * one source
			 */
			if (le32_to_cpu(dma_hw_desc->sg1u) &
			    DMA_CUED_XOR_WIN_MSK) {
				/* RXOR op, there are 2 or 3 sources */
				if (((le32_to_cpu(dma_hw_desc->sg1u) >>
				    DMA_CUED_REGION_OFF) &
				      DMA_CUED_REGION_MSK) == DMA_RXOR12) {
					/* RXOR 1-2 */
					return 2;
				} else {
					/* RXOR 1-2-3/1-2-4/1-2-5 */
					return 3;
				}
			}
			return 1;
		default:
			printk(KERN_ERR "%s: unknown OPC 0x%02x\n",
				__FUNCTION__, dma_hw_desc->opc);
			BUG();
		}
	case PPC460EX_XOR_ID:
		/* up to 16 sources */
		xor_hw_desc = desc->hw_desc;
		return (xor_hw_desc->cbc & XOR_CDCR_OAC_MSK);
	default:
		BUG();
	}
	return 0;
}

/**
 * ppc460ex_desc_get_dst_num - get the number of destination addresses in
 * this descriptor
 */
static inline u32 ppc460ex_desc_get_dst_num(ppc460ex_desc_t *desc,
		ppc460ex_ch_t *chan)
{
	dma_cdb_t *dma_hw_desc;

	switch (chan->device->id) {
	case PPC460EX_DMA0_ID:
	case PPC460EX_DMA1_ID:
		/* May be 1 or 2 destinations */
		dma_hw_desc = desc->hw_desc;
		switch (dma_hw_desc->opc) {
		case DMA_CDB_OPC_NO_OP:
		case DMA_CDB_OPC_DCHECK128:
			return 0;
		case DMA_CDB_OPC_MV_SG1_SG2:
		case DMA_CDB_OPC_DFILL128:
			return 1;
		case DMA_CDB_OPC_MULTICAST:
			return 2;
		default:
			printk(KERN_ERR "%s: unknown OPC 0x%02x\n",
				__FUNCTION__, dma_hw_desc->opc);
			BUG();
		}
	case PPC460EX_XOR_ID:
		/* Always only 1 destination */
		return 1;
	default:
		BUG();
	}
	return 0;
}

/**
 * ppc460ex_desc_get_link - get the address of the descriptor that
 * follows this one
 */
static inline u32 ppc460ex_desc_get_link(ppc460ex_desc_t *desc,
		ppc460ex_ch_t *chan)
{
	if (!desc->hw_next)
		return 0;

	return desc->hw_next->phys;
}

/**
 * ppc460ex_desc_is_aligned - check alignment
 */
static inline int ppc460ex_desc_is_aligned(ppc460ex_desc_t *desc,
		int num_slots)
{
	return (desc->idx & (num_slots - 1)) ? 0 : 1;
}

/**
 * ppc460ex_chan_xor_slot_count - get the number of slots necessary for
 * XOR operation
 */
static inline int ppc460ex_chan_xor_slot_count(size_t len, int src_cnt,
		int *slots_per_op)
{
	int slot_cnt;

	/* each XOR descriptor provides up to 16 source operands */
	slot_cnt = *slots_per_op = (src_cnt + XOR_MAX_OPS - 1)/XOR_MAX_OPS;

	if (likely(len <= PPC460EX_ADMA_XOR_MAX_BYTE_COUNT))
		return slot_cnt;

	printk(KERN_ERR "%s: len %d > max %d !!\n",
		__FUNCTION__, len, PPC460EX_ADMA_XOR_MAX_BYTE_COUNT);
	BUG();
	return slot_cnt;
}

/**
 */
static inline int ppc460ex_chan_pqxor_slot_count (dma_addr_t *srcs,
		int src_cnt, size_t len)
{
	int order = 0;
	int state = 0;
	int addr_count = 0;
	int i;
	for (i=1; i<src_cnt; i++) {
		char *cur_addr = (char *)srcs[i];
		char *old_addr = (char *)srcs[i-1];
		switch (state) {
			case 0:
				if (cur_addr == old_addr + len) {
					/* direct RXOR */
					order = 1;
					state = 1;
					if (i == src_cnt-1) {
						addr_count++;
					}
				} else if (old_addr == cur_addr + len) {
					/* reverse RXOR */
					order = -1;
					state = 1;
					if (i == src_cnt-1) {
						addr_count++;
					}
				} else {
					state = 3;
				}
				break;
			case 1:
				if (i == src_cnt-2 || (order == -1
					&& cur_addr != old_addr - len)) {
					order = 0;
					state = 0;
					addr_count++;
				} else if (cur_addr == old_addr + len*order) {
					state = 2;
					if (i == src_cnt-1) {
						addr_count++;
					}
				} else if (cur_addr == old_addr + 2*len) {
					state = 2;
					if (i == src_cnt-1) {
						addr_count++;
					}
				} else if (cur_addr == old_addr + 3*len) {
					state = 2;
					if (i == src_cnt-1) {
						addr_count++;
					}
				} else {
					order = 0;
					state = 0;
					addr_count++;
				}
				break;
			case 2:
				order = 0;
				state = 0;
				addr_count++;
				break;
		}
		if (state == 3) break;
	}
	if (src_cnt <= 1 || (state != 1 && state != 2)) {
		/* FIXME. return 0 here and check for this when called. */
		BUG ();
	}

	return (addr_count + XOR_MAX_OPS - 1) / XOR_MAX_OPS;
}


/******************************************************************************
 * ADMA channel low-level routines
 ******************************************************************************/

static inline u32 ppc460ex_chan_get_current_descriptor(ppc460ex_ch_t *chan);
static inline void ppc460ex_chan_append(ppc460ex_ch_t *chan);

/**
 * ppc460ex_adma_device_clear_eot_status - interrupt ack to XOR or DMA engine
 */
static inline void ppc460ex_adma_device_clear_eot_status (ppc460ex_ch_t *chan)
{
	volatile dma_regs_t *dma_reg;
	u8 *p = chan->device->dma_desc_pool_virt;
	dma_cdb_t *cdb;
	u32 rv, hv, i;

	switch (chan->device->id) {
	case PPC460EX_DMA0_ID:
	case PPC460EX_DMA1_ID:
		/* read FIFO to ack */
		//dma_reg = (dma_regs_t *)chan->device->pdev->resource[0].start;
		dma_reg = (dma_regs_t *)chan->device->res[0].start;
		while (rv = in_le32(&dma_reg->csfpl)) {

			if ( chan->device->desc_memory == ADMA_DESC_MEM_OCM) 
				hv = in_le32(&dma_reg->csfph);/* clear the upper bits too */

			i = rv & DMA_CDB_ADDR_MSK;
			cdb = (dma_cdb_t *)&p[i -
			    (u32)chan->device->dma_desc_pool];

			/* Clear opcode to ack. This is necessary for
			 * ZeroSum operations only
			 */
			cdb->opc = 0;

			if (test_bit(PPC460EX_RXOR_RUN,
			    &ppc460ex_rxor_state)) {
				/* probably this is a completed RXOR op,
				 * get pointer to CDB using the fact that
				 * physical and virtual addresses of CDB
				 * in pools have the same offsets
				 */
				if (le32_to_cpu(cdb->sg1u) &
				    DMA_CUED_XOR_BASE) {
				/* this is a RXOR */
					clear_bit(PPC460EX_RXOR_RUN,
					    &ppc460ex_rxor_state);
				}
			}

			if (rv & DMA_CDB_STATUS_MSK) {
				/* ZeroSum check failed
				 */
				ppc460ex_desc_t *iter;
				dma_addr_t phys = rv & ~DMA_CDB_MSK;

				/*
				 * Update the status of corresponding
				 * descriptor.
				 */
				list_for_each_entry(iter, &chan->chain,
				    chain_node) {
					if (iter->phys == phys)
						break;
				}
				/*
				 * if cannot find the corresponding
				 * slot it's a bug
				 */
				BUG_ON (&iter->chain_node == &chan->chain);

				if (iter->xor_check_result)
					*iter->xor_check_result |=
					    rv & DMA_CDB_STATUS_MSK;
			}
		}

		rv = in_le32(&dma_reg->dsts);
		if (rv) {
			printk("DMA%d err status: 0x%x\n", chan->device->id,
				rv);
			/* write back to clear */
			out_le32(&dma_reg->dsts, rv);
		}
		break;
	}
}


/**
 * ppc460ex_chan_is_busy - get the channel status
 */
static inline int ppc460ex_chan_is_busy(ppc460ex_ch_t *chan)
{
	int busy = 0;
	volatile xor_regs_t *xor_reg;
	volatile dma_regs_t *dma_reg;

	switch (chan->device->id) {
	case PPC460EX_DMA0_ID:
	case PPC460EX_DMA1_ID:
		dma_reg = (dma_regs_t *)chan->device->res[0].start;
		/*  if command FIFO's head and tail pointers are equal and
		 * status tail is the same as command, then channel is free
		 */
		if (dma_reg->cpfhp != dma_reg->cpftp ||
		    dma_reg->cpftp != dma_reg->csftp)
			busy = 1;
		break;
	case PPC460EX_XOR_ID:
		/* use the gtcial status bit for the XORcore
		 */
		busy = (xor_reg->sr & XOR_SR_XCP_BIT) ? 1 : 0;
		break;
	}

	return busy;
}

/**
 * ppc460ex_chan_set_first_xor_descriptor -  initi XORcore chain
 */
static inline void ppc460ex_chan_set_first_xor_descriptor(ppc460ex_ch_t *chan,
						ppc460ex_desc_t *next_desc)
{
	volatile xor_regs_t *xor_reg;

	//xor_reg = (xor_regs_t *)chan->device->pdev->resource[0].start;

	if (xor_reg->sr & XOR_SR_XCP_BIT)
		printk(KERN_INFO "%s: Warn: XORcore is running "
			"when try to set the first CDB!\n",
			__FUNCTION__);

	xor_last_submit = xor_last_linked = next_desc;

	xor_reg->crsr = XOR_CRSR_64BA_BIT;

	xor_reg->cblalr = next_desc->phys;
	xor_reg->cblahr = 0;
	xor_reg->cbcr |= XOR_CBCR_LNK_BIT;

	chan->hw_chain_inited = 1;
}

/**
 * ppc460ex_dma_put_desc - put DMA0,1 descriptor to FIFO
 */
static void ppc460ex_dma_put_desc(ppc460ex_ch_t *chan,
		ppc460ex_desc_t *desc)
{
	u32 pcdb;
	volatile dma_regs_t *dma_reg =
		dma_reg = (dma_regs_t *)chan->device->res[0].start;

	pcdb = desc->phys;
	if (!test_bit(PPC460EX_DESC_INT, &desc->flags))
		pcdb |= DMA_CDB_NO_INT;
	if ( chan->device->desc_memory == ADMA_DESC_MEM_OCM) 
		pcdb |= DMA_CDB_64B_ADDR;       /* 64 bit */
	chan_last_sub[chan->device->id] = desc;
	out_le32 (&dma_reg->cpfpl, pcdb);

	if ( chan->device->desc_memory == ADMA_DESC_MEM_OCM) 
		out_le32 (&dma_reg->cpfph, 0x4); //upper bits
}

/**
 * ppc460ex_chan_append - update the h/w chain in the channel
 */
static inline void ppc460ex_chan_append(ppc460ex_ch_t *chan)
{
	volatile dma_regs_t *dma_reg;
	ppc460ex_desc_t *iter;
	u32 cur_desc;
	unsigned long flags;

	local_irq_save(flags);
	switch (chan->device->id) {
	case PPC460EX_DMA0_ID:
	case PPC460EX_DMA1_ID:
		//dma_reg = (dma_regs_t *)chan->device->pdev->resource[0].start;
		//dma_reg = (dma_regs_t *)chan->device->odev->dev.resource[0].start;
		dma_reg = (dma_regs_t *)chan->device->res[0].start;
		cur_desc = ppc460ex_chan_get_current_descriptor(chan);

		if (likely(cur_desc)) {
			iter = chan_last_sub[chan->device->id];
			BUG_ON(!iter);
		} else {
			/* first peer */
			iter = chan_first_cdb[chan->device->id];
			BUG_ON(!iter);
			ppc460ex_dma_put_desc(chan, iter);
			chan->hw_chain_inited = 1;
		}

		/* is there something new to append */
		if (!iter->hw_next)
			goto out;

		/* flush descriptors from the s/w queue to fifo */
		list_for_each_entry_continue(iter, &chan->chain, chain_node) {
			ppc460ex_dma_put_desc(chan, iter);
			if (!iter->hw_next)
				break;
		}
		break;
	}
out:	
	local_irq_restore(flags);
}

/**
 * ppc460ex_chan_get_current_descriptor - get the currently executed descriptor
 */
static inline u32 ppc460ex_chan_get_current_descriptor(ppc460ex_ch_t *chan)
{
	volatile dma_regs_t *dma_reg;
	volatile xor_regs_t *xor_reg;

	if (unlikely(!chan->hw_chain_inited))
		/* h/w descriptor chain is not initialized yet */
		return 0;

	switch (chan->device->id) {
	case PPC460EX_DMA0_ID:
	case PPC460EX_DMA1_ID:
		//dma_reg = (dma_regs_t *)chan->device->pdev->resource[0].start;
		dma_reg = (dma_regs_t *)chan->device->res[0].start;
		return (le32_to_cpu(dma_reg->acpl)) & (~DMA_CDB_MSK);
	case PPC460EX_XOR_ID:
		//xor_reg = (xor_regs_t *)chan->device->pdev->resource[0].start;
		return xor_reg->ccbalr;
	}
	return 0;
}

/**
 * ppc460ex_chan_run - enable the channel
 */
static inline void ppc460ex_chan_run(ppc460ex_ch_t *chan)
{
	volatile xor_regs_t *xor_reg;

	switch (chan->device->id) {
	case PPC460EX_DMA0_ID:
	case PPC460EX_DMA1_ID:
		/* DMAs are always enabled, do nothing */
		break;
	case PPC460EX_XOR_ID:
		/* drain write buffer */

		/* fetch descriptor pointed to in <link> */
		xor_reg->crsr = XOR_CRSR_64BA_BIT | XOR_CRSR_XAE_BIT;
		break;
	}
}


/******************************************************************************
 * ADMA device level
 ******************************************************************************/

static void ppc460ex_chan_start_null_xor(ppc460ex_ch_t *chan);
static int ppc460ex_adma_alloc_chan_resources(struct dma_chan *chan);
static dma_cookie_t ppc460ex_adma_tx_submit(
		struct dma_async_tx_descriptor *tx);

static void ppc460ex_adma_set_dest(
		ppc460ex_desc_t *tx,
		dma_addr_t addr, int index);
static void ppc460ex_adma_memcpy_xor_set_src(
		ppc460ex_desc_t *tx,
		dma_addr_t addr, int index);

static void ppc460ex_adma_dma01_xor_set_dest(
		ppc460ex_desc_t *tx,
		dma_addr_t addr, int index);
static void ppc460ex_adma_dma01_xor_set_src(
		ppc460ex_desc_t *tx,
		dma_addr_t addr, int index);
static void ppc460ex_adma_dma01_xor_set_src_mult(
		ppc460ex_desc_t *tx,
		unsigned char mult, int index);

static void ppc460ex_adma_pqxor_set_dest(
		ppc460ex_desc_t *tx,
		dma_addr_t *paddr, unsigned long flags);
static void ppc460ex_adma_pqxor_set_src(
		ppc460ex_desc_t *tx,
		dma_addr_t addr, int index);
static void ppc460ex_adma_pqxor_set_src_mult (
		ppc460ex_desc_t *sw_desc,
		unsigned char mult, int index,int dst_pos);

static void ppc460ex_adma_pqzero_sum_set_dest	(
		ppc460ex_desc_t *sw_desc,
		dma_addr_t paddr, dma_addr_t qaddr);
static void ppc460ex_adma_mq_zero_sum_set_dest	(
		ppc460ex_desc_t *sw_desc,
		dma_addr_t addr);
static void ppc460ex_adma_pqzero_sum_set_src(
		ppc460ex_desc_t *tx,
		dma_addr_t addr, int index);
static void ppc460ex_adma_pqzero_sum_set_src_mult(
		ppc460ex_desc_t *tx,
		unsigned char mult, int index, int dst_pos);

static void ppc460ex_adma_dma2rxor_set_dest (
		ppc460ex_desc_t *tx,
		dma_addr_t addr, int index);

void ppc460ex_adma_xor_set_dest(ppc460ex_desc_t *sw_desc,
		dma_addr_t *addrs, unsigned long flags);
void ppc460ex_adma_xor_set_src(
		ppc460ex_desc_t *sw_desc,
		dma_addr_t addr,
		int index);
void ppc460ex_adma_xor_set_src_mult (
		ppc460ex_desc_t *sw_desc,
		unsigned char mult, int index,int dst_pos);
/**
 * ppc460ex_can_rxor - check if the operands may be processed with RXOR
 */
static int ppc460ex_can_rxor (struct page **srcs, int src_cnt, size_t len)
{
	int i, order = 0, state = 0;

	if (unlikely(!(src_cnt > 1)))
		return 0;

	for (i=1; i<src_cnt; i++) {
		char *cur_addr = page_address (srcs[i]);
		char *old_addr = page_address (srcs[i-1]);
		switch (state) {
		case 0:
			if (cur_addr == old_addr + len) {
				/* direct RXOR */
				order = 1;
				state = 1;
			} else
			if (old_addr == cur_addr + len) {
				/* reverse RXOR */
				order = -1;
				state = 1;
			} else
				goto out;
			break;
		case 1:
			if ((i == src_cnt-2) ||
			    (order == -1 && cur_addr != old_addr - len)) {
				order = 0;
				state = 0;
			} else
			if ((cur_addr == old_addr + len*order) ||
			    (cur_addr == old_addr + 2*len) ||
			    (cur_addr == old_addr + 3*len)) {
				state = 2;
			} else {
				order = 0;
				state = 0;
			}
			break;
		case 2:
			order = 0;
			state = 0;
			break;
		}
	}

out:
	if (state == 1 || state == 2)
		return 1;

	return 0;
}

/**
 * ppc460ex_adma_device_estimate - estimate the efficiency of processing
 *	the operation given on this channel. It's assumed that 'chan' is
 *	capable to process 'cap' type of operation.
 * @chan: channel to use
 * @cap: type of transaction
 * @src_lst: array of source pointers
 * @src_cnt: number of source operands
 * @src_sz: size of each source operand
 */
int ppc460ex_adma_estimate (struct dma_chan *chan,
	enum dma_transaction_type cap, struct page **src_lst,
	int src_cnt, size_t src_sz)
{
	int ef = 1;

	if (cap == DMA_PQ || cap == DMA_PQ_VAL) {
		/* If RAID-6 capabilities were not activated don't try
		 * to use them
		 */
		if (unlikely(!ppc460ex_r6_enabled))
			return -1;
	}
	if(cap == DMA_XOR) {
		if (unlikely(!ppc460ex_r5_enabled)) 
			return -1;
	}
	/*  in the current implementation of ppc460ex ADMA driver it
	 * makes sense to pick out only pqxor case, because it may be
	 * processed:
	 * (1) either using Biskup method on DMA2;
	 * (2) or on DMA0/1.
	 *  Thus we give a favour to (1) if the sources are suitable;
	 * else let it be processed on one of the DMA0/1 engines.
	 */
	if (cap == DMA_PQ && chan->chan_id == PPC460EX_XOR_ID) {
		if (ppc460ex_can_rxor(src_lst, src_cnt, src_sz))
			ef = 3; /* override (dma0/1 + idle) */
		else
			ef = 0; /* can't process on DMA2 if !rxor */
	}

	/* channel idleness increases the priority */
	if (likely(ef) &&
	    !ppc460ex_chan_is_busy(to_ppc460ex_adma_chan(chan)))
		ef++;

	return ef;
}

/**
 * ppc460ex_get_group_entry - get group entry with index idx
 * @tdesc: is the last allocated slot in the group.
 */
static inline ppc460ex_desc_t *
ppc460ex_get_group_entry ( ppc460ex_desc_t *tdesc, u32 entry_idx)
{
	ppc460ex_desc_t *iter = tdesc->group_head;
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
 * ppc460ex_adma_free_slots - flags descriptor slots for reuse
 * @slot: Slot to free
 * Caller must hold &ppc460ex_chan->lock while calling this function
 */
static void ppc460ex_adma_free_slots(ppc460ex_desc_t *slot,
		ppc460ex_ch_t *chan)
{
	int stride = slot->slots_per_op;

	while (stride--) {
		/*async_tx_clear_ack(&slot->async_tx);*/ /* Don't need to clear. It is hack*/
		slot->slots_per_op = 0;
		slot = list_entry(slot->slot_node.next,
				ppc460ex_desc_t,
				slot_node);
	}
}

static void 
ppc460ex_adma_unmap(ppc460ex_ch_t *chan, ppc460ex_desc_t *desc)
{
	u32 src_cnt, dst_cnt;
	dma_addr_t addr;
	/*
	 * get the number of sources & destination
	 * included in this descriptor and unmap
	 * them all
	 */
	src_cnt = ppc460ex_desc_get_src_num(desc, chan);
	dst_cnt = ppc460ex_desc_get_dst_num(desc, chan);

	/* unmap destinations */
	if (!(desc->async_tx.flags & DMA_COMPL_SKIP_DEST_UNMAP)) {
		while (dst_cnt--) {
			addr = ppc460ex_desc_get_dest_addr(
				desc, chan, dst_cnt);
			dma_unmap_page(&chan->device->odev->dev,
					addr, desc->unmap_len,
					DMA_FROM_DEVICE);
		}
	}

	/* unmap sources */
	if (!(desc->async_tx.flags & DMA_COMPL_SKIP_SRC_UNMAP)) {
		while (src_cnt--) {
			addr = ppc460ex_desc_get_src_addr(
				desc, chan, src_cnt);
			dma_unmap_page(&chan->device->odev->dev,
					addr, desc->unmap_len,
					DMA_TO_DEVICE);
		}
	}

}
/**
 * ppc460ex_adma_run_tx_complete_actions - call functions to be called
 * upon complete
 */
static dma_cookie_t ppc460ex_adma_run_tx_complete_actions(
		ppc460ex_desc_t *desc,
		ppc460ex_ch_t *chan,
		dma_cookie_t cookie)
{
	int i;

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
			ppc460ex_desc_t *unmap = desc->group_head;
			/* assume 1 slot per op always */
			u32 slot_count = unmap->slot_cnt;

			/* Run through the group list and unmap addresses */
			for (i = 0; i < slot_count; i++) {
				BUG_ON(!unmap);
				ppc460ex_adma_unmap(chan, unmap);
				unmap = unmap->hw_next;
			}
			//desc->group_head = NULL;
		}
	}

	/* run dependent operations */
	dma_run_dependencies(&desc->async_tx);

	return cookie;
}

/**
 * ppc460ex_adma_clean_slot - clean up CDB slot (if ack is set)
 */
static int ppc460ex_adma_clean_slot(ppc460ex_desc_t *desc,
		ppc460ex_ch_t *chan)
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
	    desc->phys == ppc460ex_chan_get_current_descriptor(chan))
		return 1;

	if (chan->device->id != PPC460EX_XOR_ID) {
		/* our DMA interrupt handler clears opc field of
		 * each processed descriptor. For all types of
		 * operations except for ZeroSum we do not actually
		 * need ack from the interrupt handler. ZeroSum is a
		 * gtcial case since the result of this operation
		 * is available from the handler only, so if we see
		 * such type of descriptor (which is unprocessed yet)
		 * then leave it in chain.
		 */
		dma_cdb_t *cdb = desc->hw_desc;
		if (cdb->opc == DMA_CDB_OPC_DCHECK128)
			return 1;
	}

	dev_dbg(chan->device->common.dev, "\tfree slot %x: %d stride: %d\n",
		desc->phys, desc->idx, desc->slots_per_op);

	list_del(&desc->chain_node);
	ppc460ex_adma_free_slots(desc, chan);
	return 0;
}

/**
 * #define DEBUG 1__ppc460ex_adma_slot_cleanup - this is the common clean-up routine
 *	which runs through the channel CDBs list until reach the descriptor
 *	currently processed. When routine determines that all CDBs of group
 *	are completed then corresponding callbacks (if any) are called and slots
 *	are freed.
 */
static void __ppc460ex_adma_slot_cleanup(ppc460ex_ch_t *chan)
{
	ppc460ex_desc_t *iter, *_iter, *group_start = NULL;
	dma_cookie_t cookie = 0;
	u32 current_desc = ppc460ex_chan_get_current_descriptor(chan);
	int busy = ppc460ex_chan_is_busy(chan);
	int seen_current = 0, slot_cnt = 0, slots_per_op = 0;

	dev_dbg(chan->device->common.dev, "ppc460ex adma%d: %s\n",
		chan->device->id, __FUNCTION__);

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
		dev_dbg(chan->device->common.dev, "\tcookie: %d slot: %d "
		    "busy: %d this_desc: %#x next_desc: %#x cur: %#x ack: %d\n",
			iter->async_tx.cookie, iter->idx, busy, iter->phys,
		    ppc460ex_desc_get_link(iter, chan), current_desc,
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
			if (busy || ppc460ex_desc_get_link(iter, chan)) {
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
			ppc460ex_desc_t *grp_iter, *_grp_iter;
			int end_of_chain = 0;

			/* clean up the group */
			slot_cnt = group_start->slot_cnt;
			grp_iter = group_start;
			list_for_each_entry_safe_from(grp_iter, _grp_iter,
				&chan->chain, chain_node) {

				cookie = ppc460ex_adma_run_tx_complete_actions(
					grp_iter, chan, cookie);

				slot_cnt -= slots_per_op;
				end_of_chain = ppc460ex_adma_clean_slot(
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

		cookie = ppc460ex_adma_run_tx_complete_actions(iter, chan,
		    cookie);

		if (ppc460ex_adma_clean_slot(iter, chan))
			break;
	}

	BUG_ON(!seen_current);

	if (cookie > 0) {
		chan->completed_cookie = cookie;
		pr_debug("\tcompleted cookie %d\n", cookie);
#ifdef DEBUG_ADMA   
		static int tcnt=0;
		if(tcnt%100 == 0)
		printk("\t <%s> completed cookie %d\n",__FUNCTION__, cookie);
#endif
	}

}

/**
 * ppc460ex_adma_tasklet - clean up watch-dog initiator
 */
static void ppc460ex_adma_tasklet (unsigned long data)
{
	ppc460ex_ch_t *chan = (ppc460ex_ch_t *) data;
	__ppc460ex_adma_slot_cleanup(chan);
}

/**
 * ppc460ex_adma_slot_cleanup - clean up scheduled initiator
 */
static void ppc460ex_adma_slot_cleanup (ppc460ex_ch_t *chan)
{
	spin_lock_bh(&chan->lock);
	__ppc460ex_adma_slot_cleanup(chan);
	spin_unlock_bh(&chan->lock);
}

/**
 * ppc460ex_adma_alloc_slots - allocate free slots (if any)
 */
ppc460ex_desc_t *ppc460ex_adma_alloc_slots(
		ppc460ex_ch_t *chan, int num_slots,
		int slots_per_op)
{
	ppc460ex_desc_t *iter = NULL, *_iter, *alloc_start = NULL;
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
		iter = list_entry(&chan->all_slots, ppc460ex_desc_t,
			slot_node);
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
			ppc460ex_desc_t *alloc_tail = NULL;
			ppc460ex_desc_t *last_used = NULL;
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
				iter->xor_check_result = NULL;
				for (i = 0; i < slots_per_op; i++) {
					iter->slots_per_op = slots_per_op - i;
					last_used = iter;
					iter = list_entry(iter->slot_node.next,
						ppc460ex_desc_t,
						slot_node);
				}
				num_slots -= slots_per_op;
			}
			alloc_tail->group_head = alloc_start;
			alloc_tail->async_tx.cookie = -EBUSY;
			list_splice(&chain, &alloc_tail->group_list);
			chan->last_used = last_used;
			return alloc_tail;
		}
	}
	if (!retry++)
		goto retry;
	static empty_slot_cnt;
	if(!(empty_slot_cnt%100))
		dev_dbg(chan->device->common.dev,
				"No empty slots trying to free some\n");
	empty_slot_cnt++;
	/* try to free some slots if the allocation fails */
	tasklet_schedule(&chan->irq_tasklet);
	return NULL;
}

/**
 * ppc460ex_adma_alloc_chan_resources -  allocate pools for CDB slots
 */
static int ppc460ex_adma_alloc_chan_resources(struct dma_chan *chan)
{
	ppc460ex_ch_t *ppc460ex_chan = to_ppc460ex_adma_chan(chan);
	ppc460ex_desc_t *slot = NULL;
	char *hw_desc;
	int i, db_sz;
	int init = ppc460ex_chan->slots_allocated ? 0 : 1;
	ppc460ex_aplat_t *plat_data;

	chan->chan_id = ppc460ex_chan->device->id;
	plat_data = ppc460ex_chan->device->odev->dev.platform_data;

	/* Allocate descriptor slots */
	i = ppc460ex_chan->slots_allocated;
	if (ppc460ex_chan->device->id != PPC460EX_XOR_ID)
		db_sz = sizeof (dma_cdb_t);
	else
		db_sz = sizeof (xor_cb_t);

	for (; i < (plat_data->pool_size/db_sz); i++) {
		slot = kzalloc(sizeof(ppc460ex_desc_t), GFP_KERNEL);
		if (!slot) {
			printk(KERN_INFO "GT ADMA Channel only initialized"
				" %d descriptor slots", i--);
			break;
		}

		hw_desc = (char *) ppc460ex_chan->device->dma_desc_pool_virt;
		slot->hw_desc = (void *) &hw_desc[i * db_sz];
		dma_async_tx_descriptor_init(&slot->async_tx, chan);
		slot->async_tx.tx_submit = ppc460ex_adma_tx_submit;
		INIT_LIST_HEAD(&slot->chain_node);
		INIT_LIST_HEAD(&slot->slot_node);
		INIT_LIST_HEAD(&slot->group_list);
		hw_desc = (char *) ppc460ex_chan->device->dma_desc_pool;
		slot->phys = (dma_addr_t) &hw_desc[i * db_sz];
		slot->idx = i;
		spin_lock_bh(&ppc460ex_chan->lock);
		ppc460ex_chan->slots_allocated++;
		list_add_tail(&slot->slot_node, &ppc460ex_chan->all_slots);
		spin_unlock_bh(&ppc460ex_chan->lock);
	}

	if (i && !ppc460ex_chan->last_used) {
		ppc460ex_chan->last_used =
			list_entry(ppc460ex_chan->all_slots.next,
				ppc460ex_desc_t,
				slot_node);
	}

	dev_dbg(ppc460ex_chan->device->common.dev,
		"ppc460ex adma%d: allocated %d descriptor slots\n",
		ppc460ex_chan->device->id, i);

	/* initialize the channel and the chain with a null operation */
	if (init) {
		switch (ppc460ex_chan->device->id)
		{
		case PPC460EX_DMA0_ID:
		case PPC460EX_DMA1_ID:
			ppc460ex_chan->hw_chain_inited = 0;
			/* Use WXOR for self-testing */
			if (!ppc460ex_r5_tchan)
				ppc460ex_r5_tchan = ppc460ex_chan;
			if (!ppc460ex_r6_tchan)
				ppc460ex_r6_tchan = ppc460ex_chan;
			break;
		case PPC460EX_XOR_ID:
			ppc460ex_chan_start_null_xor(ppc460ex_chan);
			break;
		default:
			BUG();
		}
		ppc460ex_chan->needs_unmap = 1;
	}

	return (i > 0) ? i : -ENOMEM;
}

/**
 * ppc460ex_desc_assign_cookie - assign a cookie
 */
static dma_cookie_t ppc460ex_desc_assign_cookie(ppc460ex_ch_t *chan,
		ppc460ex_desc_t *desc)
{
	dma_cookie_t cookie = chan->common.cookie;
	cookie++;
	if (cookie < 0)
		cookie = 1;
	chan->common.cookie = desc->async_tx.cookie = cookie;
	return cookie;
}

/**
 * ppc460ex_rxor_set_region_data -
 */
static void ppc460ex_rxor_set_region (ppc460ex_desc_t *desc,
	u8 xor_arg_no, u32 mask)
{
	xor_cb_t *xcb = desc->hw_desc;

	xcb->ops [xor_arg_no].h |= mask;
}

/**
 * ppc460ex_rxor_set_src -
 */
static void ppc460ex_rxor_set_src (ppc460ex_desc_t *desc,
	u8 xor_arg_no, dma_addr_t addr)
{
	xor_cb_t *xcb = desc->hw_desc;

	xcb->ops [xor_arg_no].h |= DMA_CUED_XOR_BASE;
	xcb->ops [xor_arg_no].l = addr;
}

/**
 * ppc460ex_rxor_set_mult -
 */
static void ppc460ex_rxor_set_mult (ppc460ex_desc_t *desc,
	u8 xor_arg_no, u8 idx, u8 mult)
{
	xor_cb_t *xcb = desc->hw_desc;

	xcb->ops [xor_arg_no].h |= mult << (DMA_CUED_MULT1_OFF + idx * 8);
}

/**
 * ppc460ex_wxor_set_base
 */
static void ppc460ex_wxor_set_base (ppc460ex_desc_t *desc)
{
	xor_cb_t *xcb = desc->hw_desc;

	xcb->cbtah = DMA_CUED_XOR_BASE;
	xcb->cbtah |= (1 << DMA_CUED_MULT1_OFF);
}

/**
 * ppc460ex_adma_check_threshold - append CDBs to h/w chain if threshold
 *	has been achieved
 */
static void ppc460ex_adma_check_threshold(ppc460ex_ch_t *chan)
{
	dev_dbg(chan->device->common.dev, "ppc460ex adma%d: pending: %d\n",
		chan->device->id, chan->pending);

	if (chan->pending >= PPC460EX_ADMA_THRESHOLD) {
		chan->pending = 0;
		ppc460ex_chan_append(chan);
	}
}

/**
 * ppc460ex_adma_tx_submit - submit new descriptor group to the channel
 *	(it's not necessary that descriptors will be submitted to the h/w
 *	chains too right now)
 */
static dma_cookie_t ppc460ex_adma_tx_submit(struct dma_async_tx_descriptor *tx)
{
	ppc460ex_desc_t *sw_desc = tx_to_ppc460ex_adma_slot(tx);
	ppc460ex_ch_t *chan = to_ppc460ex_adma_chan(tx->chan);
	ppc460ex_desc_t *group_start, *old_chain_tail;
	int slot_cnt;
	int slots_per_op;
	dma_cookie_t cookie;

	group_start = sw_desc->group_head;
	slot_cnt = group_start->slot_cnt;
	slots_per_op = group_start->slots_per_op;

	spin_lock_bh(&chan->lock);

	cookie = ppc460ex_desc_assign_cookie(chan, sw_desc);

	if (unlikely(list_empty(&chan->chain))) {
		/* first peer */
		list_splice_init(&sw_desc->group_list, &chan->chain);
		chan_first_cdb[chan->device->id] = group_start;
	} else {
		/* isn't first peer, bind CDBs to chain */
		old_chain_tail = list_entry(chan->chain.prev,
			ppc460ex_desc_t, chain_node);
		list_splice_init(&sw_desc->group_list,
		    &old_chain_tail->chain_node);
		/* fix up the hardware chain */
		ppc460ex_desc_set_link(chan, old_chain_tail, group_start);
	}

	/* increment the pending count by the number of operations */
	chan->pending += slot_cnt / slots_per_op;
	ppc460ex_adma_check_threshold(chan);
	spin_unlock_bh(&chan->lock);

	dev_dbg(chan->device->common.dev,
		"ppc460ex adma%d: %s cookie: %d slot: %d tx %p\n",
		chan->device->id,__FUNCTION__,
		sw_desc->async_tx.cookie, sw_desc->idx, sw_desc);
	return cookie;
}

/**
 * ppc460ex_adma_prep_dma_interrupt - prepare CDB for a pseudo DMA operation
 */
static struct dma_async_tx_descriptor *ppc460ex_adma_prep_dma_interrupt(
		struct dma_chan *chan, unsigned long flags)
{
	ppc460ex_ch_t *ppc460ex_chan = to_ppc460ex_adma_chan(chan);
	ppc460ex_desc_t *sw_desc, *group_start;
	int slot_cnt, slots_per_op;

	dev_dbg(ppc460ex_chan->device->common.dev,
		"ppc460ex adma%d: %s\n", ppc460ex_chan->device->id,
		__FUNCTION__);

	spin_lock_bh(&ppc460ex_chan->lock);
	slot_cnt = slots_per_op = 1;
	sw_desc = ppc460ex_adma_alloc_slots(ppc460ex_chan, slot_cnt,
			slots_per_op);
	if (sw_desc) {
		group_start = sw_desc->group_head;
		ppc460ex_desc_init_interrupt(group_start, ppc460ex_chan);
		group_start->unmap_len = 0;
		sw_desc->async_tx.flags = flags;
	}
	spin_unlock_bh(&ppc460ex_chan->lock);

	return sw_desc ? &sw_desc->async_tx : NULL;
}

/**
 * ppc460ex_adma_prep_dma_memcpy - prepare CDB for a MEMCPY operation
 */
static struct dma_async_tx_descriptor *ppc460ex_adma_prep_dma_memcpy(
		struct dma_chan *chan, dma_addr_t dma_dest,
		dma_addr_t dma_src, size_t len, unsigned long flags)
{
	ppc460ex_ch_t *ppc460ex_chan = to_ppc460ex_adma_chan(chan);
	ppc460ex_desc_t *sw_desc, *group_start;
	int slot_cnt, slots_per_op;
	if (unlikely(!len))
		return NULL;
	BUG_ON(unlikely(len > PPC460EX_ADMA_DMA_MAX_BYTE_COUNT));

	spin_lock_bh(&ppc460ex_chan->lock);

	dev_dbg(ppc460ex_chan->device->common.dev,
		"ppc460ex adma%d: %s len: %u int_en %d \n",
		ppc460ex_chan->device->id, __FUNCTION__, len,
		flags & DMA_PREP_INTERRUPT ? 1 : 0);

	slot_cnt = slots_per_op = 1;
	sw_desc = ppc460ex_adma_alloc_slots(ppc460ex_chan, slot_cnt,
		slots_per_op);
	if (sw_desc) {
		group_start = sw_desc->group_head;
		ppc460ex_desc_init_memcpy(group_start, flags);
		ppc460ex_adma_set_dest(group_start, dma_dest, 0);
		ppc460ex_adma_memcpy_xor_set_src(group_start, dma_src, 0);
		ppc460ex_desc_set_byte_count(group_start, ppc460ex_chan, len);
		sw_desc->unmap_len = len;
		sw_desc->async_tx.flags = flags;
			if(mfdcr(0x60) == 0xfee8) {
				printk("Byte Count = 0x%x\n",len);
				printk("src= 0x%x\n",dma_src);
				printk("Dest = 0x%x\n",dma_dest);
			}
	}
	spin_unlock_bh(&ppc460ex_chan->lock);
	return sw_desc ? &sw_desc->async_tx : NULL;
}

/**
 * ppc460ex_adma_prep_dma_memset - prepare CDB for a MEMSET operation
 */
static struct dma_async_tx_descriptor *ppc460ex_adma_prep_dma_memset(
		struct dma_chan *chan, dma_addr_t dma_dest, int value,
		size_t len, unsigned long flags)
{
	ppc460ex_ch_t *ppc460ex_chan = to_ppc460ex_adma_chan(chan);
	ppc460ex_desc_t *sw_desc, *group_start;
	int slot_cnt, slots_per_op;
	if (unlikely(!len))
		return NULL;
	BUG_ON(unlikely(len > PPC460EX_ADMA_DMA_MAX_BYTE_COUNT));

	spin_lock_bh(&ppc460ex_chan->lock);

	dev_dbg(ppc460ex_chan->device->common.dev,
		"ppc460ex adma%d: %s cal: %u len: %u int_en %d\n",
		ppc460ex_chan->device->id, __FUNCTION__, value, len,
		flags & DMA_PREP_INTERRUPT ? 1 : 0);

	slot_cnt = slots_per_op = 1;
	sw_desc = ppc460ex_adma_alloc_slots(ppc460ex_chan, slot_cnt,
		slots_per_op);
	if (sw_desc) {
		group_start = sw_desc->group_head;
		ppc460ex_desc_init_memset(group_start, value, flags);
		ppc460ex_adma_set_dest(group_start, dma_dest, 0);
		ppc460ex_desc_set_byte_count(group_start, ppc460ex_chan, len);
		sw_desc->unmap_len = len;
		sw_desc->async_tx.flags = flags;
	}
	spin_unlock_bh(&ppc460ex_chan->lock);

	return sw_desc ? &sw_desc->async_tx : NULL;
}


static inline void ppc460ex_desc_set_xor_src_cnt (ppc460ex_desc_t *desc,
		int src_cnt);
static void ppc460ex_init_rxor_cursor (ppc460ex_rxor_cursor_t *cursor);

/**
 * ppc460ex_adma_init_dma2rxor_slot -
 */
static void ppc460ex_adma_init_dma2rxor_slot (ppc460ex_desc_t *desc,
		dma_addr_t *src, int src_cnt)
{
	int i;
	/* initialize CDB */
	for (i=0; i<src_cnt; i++) {
		ppc460ex_adma_dma2rxor_prep_src(desc,
			&desc->rxor_cursor,
			i, desc->src_cnt,
			(u32)src[i]);
	}
}
#if 1
static inline ppc460ex_desc_t *ppc460ex_dma_prep_pq(
		ppc460ex_ch_t *ppc460ex_chan,
		dma_addr_t *dst, unsigned int dst_cnt,
		dma_addr_t *src, unsigned int src_cnt, unsigned char *scf,
		size_t len, unsigned long flags)
{
	int slot_cnt;
	ppc460ex_desc_t *sw_desc = NULL, *iter;
	unsigned long op = 0;
	unsigned char mult = 1;

	/*  select operations WXOR/RXOR depending on the
	 * source addresses of operators and the number
	 * of destinations (RXOR support only Q-parity calculations)
	 */
	set_bit(PPC460EX_DESC_WXOR, &op);
	if (!test_and_set_bit(PPC460EX_RXOR_RUN, &ppc460ex_rxor_state)) {
		/* no active RXOR;
		 * do RXOR if:
		 * - destination os only one,
		 * - there are more than 1 source,
		 * - len is aligned on 512-byte boundary,
		 * - source addresses fit to one of 4 possible regions.
		 */
		if (dst_cnt == 1 && src_cnt > 1 &&
		    !(len & ~MQ0_CF2H_RXOR_BS_MASK) &&
		    (src[0] + len) == src[1]) {
			/* may do RXOR R1 R2 */
			set_bit(PPC460EX_DESC_RXOR, &op);
			if (src_cnt != 2) {
				/* may try to enhance region of RXOR */
				if ((src[1] + len) == src[2]) {
					/* do RXOR R1 R2 R3 */
					set_bit(PPC460EX_DESC_RXOR123,
						&op);
				} else if ((src[1] + len * 2) == src[2]) {
					/* do RXOR R1 R2 R4 */
					set_bit(PPC460EX_DESC_RXOR124, &op);
				} else if ((src[1] + len * 3) == src[2]) {
					/* do RXOR R1 R2 R5 */
					set_bit(PPC460EX_DESC_RXOR125,
						&op);
				} else {
					/* do RXOR R1 R2 */
					set_bit(PPC460EX_DESC_RXOR12,
						&op);
				}
			} else {
				/* do RXOR R1 R2 */
				set_bit(PPC460EX_DESC_RXOR12, &op);
			}
		}

		if (!test_bit(PPC460EX_DESC_RXOR, &op)) {
			/* can not do this operation with RXOR */
			clear_bit(PPC460EX_RXOR_RUN,
				&ppc460ex_rxor_state);
		} else {
			/* can do; set block size right now */
			ppc460ex_desc_set_rxor_block_size(len);
		}
	}

	/* Number of necessary slots depends on operation type selected */
	if (!test_bit(PPC460EX_DESC_RXOR, &op)) {
		/*  This is a WXOR only chain. Need descriptors for each
		 * source to GF-XOR them with WXOR, and need descriptors
		 * for each destination to zero them with WXOR
		 */
		slot_cnt = src_cnt;

		if (flags & DMA_PREP_ZERO_P) {
			slot_cnt++;
			set_bit(PPC460EX_ZERO_P, &op);
		}
		if (flags & DMA_PREP_ZERO_Q) {
			slot_cnt++;
			set_bit(PPC460EX_ZERO_Q, &op);
		}
	} else {
		/*  Need 1/2 descriptor for RXOR operation, and
		 * need (src_cnt - (2 or 3)) for WXOR of sources
		 * remained (if any)
		*/
		slot_cnt = dst_cnt;

		if (flags & DMA_PREP_ZERO_P)
			set_bit(PPC460EX_ZERO_P, &op);
		if (flags & DMA_PREP_ZERO_Q)
			set_bit(PPC460EX_ZERO_Q, &op);

		if (test_bit(PPC460EX_DESC_RXOR12, &op))
			slot_cnt += src_cnt - 2;
		else
			slot_cnt += src_cnt - 3;

		/*  Thus we have either RXOR only chain or
		 * mixed RXOR/WXOR
		 */
		if (slot_cnt == dst_cnt) {
			/* RXOR only chain */
			clear_bit(PPC460EX_DESC_WXOR, &op);
		}
	}

	spin_lock_bh(&ppc460ex_chan->lock);
	/* for both RXOR/WXOR each descriptor occupies one slot */
	sw_desc = ppc460ex_adma_alloc_slots(ppc460ex_chan, slot_cnt, 1);
	if (sw_desc) {
		ppc460ex_desc_init_pq(sw_desc, dst_cnt, src_cnt,
				flags, op);

		/* setup dst/src/mult */
			ppc460ex_adma_pqxor_set_dest(sw_desc,
				dst, flags);
		while(src_cnt--) {
			ppc460ex_adma_pqxor_set_src(sw_desc,
				src[src_cnt], src_cnt);
			if ((flags & DMA_PREP_HAVE_Q ) && !scf) {
				 mult = scf[src_cnt];
			ppc460ex_adma_pqxor_set_src_mult(sw_desc,
				scf[src_cnt], src_cnt, dst_cnt -1 );
			}
		}

		/* Setup byte count foreach slot just allocated */
		sw_desc->async_tx.flags = flags;
		list_for_each_entry(iter, &sw_desc->group_list,
				chain_node) {
			if(mfdcr(0x60) == 0xfee8) {
				printk("Byte Count = 0x%x\n",len);
				printk("src[0]= 0x%x\n",src[0]);
				printk("src[1]= 0x%x\n",src[1]);
				printk("src[2]= 0x%x\n",src[2]);
				printk("Dest = 0x%x\n",dst);
			}
			ppc460ex_desc_set_byte_count(iter,
				ppc460ex_chan, len);
			iter->unmap_len = len;
		}
	}
	spin_unlock_bh(&ppc460ex_chan->lock);

	return sw_desc;
}

#endif
static inline ppc460ex_desc_t *ppc460ex_dma_prep_xor(
		ppc460ex_ch_t *ppc460ex_chan,
		dma_addr_t *dst, unsigned int dst_cnt,
		dma_addr_t *src, unsigned int src_cnt, unsigned char *scf,
		size_t len, unsigned long flags)
{
	int slot_cnt;
	ppc460ex_desc_t *sw_desc = NULL, *iter;
	unsigned long op = 0;

	/*  select operations WXOR/RXOR depending on the
	 * source addresses of operators and the number
	 * of destinations (RXOR support only Q-parity calculations)
	 */
	set_bit(PPC460EX_DESC_WXOR, &op);
	if (!test_and_set_bit(PPC460EX_RXOR_RUN, &ppc460ex_rxor_state)) {
		/* no active RXOR;
		 * do RXOR if:
		 * - destination os only one,
		 * - there are more than 1 source,
		 * - len is aligned on 512-byte boundary,
		 * - source addresses fit to one of 4 possible regions.
		 */
		if (dst_cnt == 2 && src_cnt > 1 &&
		    !(len & ~MQ0_CF2H_RXOR_BS_MASK) &&
		    (src[0] + len) == src[1]) {
			/* may do RXOR R1 R2 */
			set_bit(PPC460EX_DESC_RXOR, &op);
			if (src_cnt != 2) {
				/* may try to enhance region of RXOR */
				if ((src[1] + len) == src[2]) {
					/* do RXOR R1 R2 R3 */
					set_bit(PPC460EX_DESC_RXOR123,
						&op);
				} else if ((src[1] + len * 2) == src[2]) {
					/* do RXOR R1 R2 R4 */
					set_bit(PPC460EX_DESC_RXOR124, &op);
				} else if ((src[1] + len * 3) == src[2]) {
					/* do RXOR R1 R2 R5 */
					set_bit(PPC460EX_DESC_RXOR125,
						&op);
				} else {
					/* do RXOR R1 R2 */
					set_bit(PPC460EX_DESC_RXOR12,
						&op);
				}
			} else {
				/* do RXOR R1 R2 */
				set_bit(PPC460EX_DESC_RXOR12, &op);
			}
		}

		if (!test_bit(PPC460EX_DESC_RXOR, &op)) {
			/* can not do this operation with RXOR */
			clear_bit(PPC460EX_RXOR_RUN,
				&ppc460ex_rxor_state);
		} else {
			/* can do; set block size right now */
			ppc460ex_desc_set_rxor_block_size(len);
		}
	}

	/* Number of necessary slots depends on operation type selected */
	if (!test_bit(PPC460EX_DESC_RXOR, &op)) {
		/*  This is a WXOR only chain. Need descriptors for each
		 * source to GF-XOR them with WXOR, and need descriptors
		 * for each destination to zero them with WXOR
		 */
		slot_cnt = src_cnt;

		if (flags & DMA_PREP_ZERO_P) {
			slot_cnt++;
			set_bit(PPC460EX_ZERO_P, &op);
		}
	} else {
		/*  Need 1/2 descriptor for RXOR operation, and
		 * need (src_cnt - (2 or 3)) for WXOR of sources
		 * remained (if any)
		*/
		slot_cnt = dst_cnt;

		if (flags & DMA_PREP_ZERO_P)
			set_bit(PPC460EX_ZERO_P, &op);

		if (test_bit(PPC460EX_DESC_RXOR12, &op))
			slot_cnt += src_cnt - 2;
		else
			slot_cnt += src_cnt - 3;

		/*  Thus we have either RXOR only chain or
		 * mixed RXOR/WXOR
		 */
		if (slot_cnt == dst_cnt) {
			/* RXOR only chain */
			clear_bit(PPC460EX_DESC_WXOR, &op);
		}
	}

	spin_lock_bh(&ppc460ex_chan->lock);
	/* for both RXOR/WXOR each descriptor occupies one slot */
	sw_desc = ppc460ex_adma_alloc_slots(ppc460ex_chan, slot_cnt, 1);
	if (sw_desc) {
		ppc460ex_desc_init_xor(sw_desc, dst_cnt, src_cnt,
				flags, op);

		/* setup dst/src/mult */
			ppc460ex_adma_xor_set_dest(sw_desc,
				dst, flags);
		while(src_cnt--) {
			ppc460ex_adma_xor_set_src(sw_desc,
				src[src_cnt], src_cnt);
		}

		/* Setup byte count foreach slot just allocated */
		sw_desc->async_tx.flags = flags;
		list_for_each_entry(iter, &sw_desc->group_list,
				chain_node) {
			if(mfdcr(0x60) == 0xfee8) {
				printk("Byte Count = 0x%x\n",len);
				printk("src[0]= 0x%x\n",src[0]);
				printk("src[1]= 0x%x\n",src[1]);
				printk("src[2]= 0x%x\n",src[2]);
				printk("Dest = 0x%x\n",dst);
			}
			ppc460ex_desc_set_byte_count(iter,
				ppc460ex_chan, len);
			iter->unmap_len = len;
		}
	}
	spin_unlock_bh(&ppc460ex_chan->lock);

	return sw_desc;
}
static inline ppc460ex_desc_t *ppc460ex_dma01_prep_xor (
		ppc460ex_ch_t *ppc460ex_chan,
		dma_addr_t dst, dma_addr_t *src, unsigned int src_cnt, 
		size_t len, unsigned long flags)
{
	int slot_cnt;
	ppc460ex_desc_t *sw_desc = NULL, *iter;
	unsigned long op = 0;
        unsigned int dst_cnt = 1; /*Marri */
	/*  select operations WXOR/RXOR depending on the
	 * source addresses of operators and the number
	 * of destinations (RXOR support only Q-parity calculations)
	 */
	set_bit(PPC460EX_DESC_WXOR, &op);
	if (!test_and_set_bit(PPC460EX_RXOR_RUN, &ppc460ex_rxor_state)) {
		/* no active RXOR;
		 * do RXOR if:
		 * - destination os only one,
		 * - there are more than 1 source,
		 * - len is aligned on 512-byte boundary,
		 * - source addresses fit to one of 4 possible regions.
		 */
		if (dst_cnt == 3 && src_cnt > 1 && /* Marri dstcnt == 3  never comes here */
		    !(len & ~MQ0_CF2H_RXOR_BS_MASK) &&
		    (src[0] - len) == src[1]) {  /* Marri */
			/* may do RXOR R1 R2 */
			set_bit(PPC460EX_DESC_RXOR, &op);
			if (src_cnt != 2) {
				/* may try to enhance region of RXOR */
				if ((src[1] - len) == src[2]) {
					/* do RXOR R1 R2 R3 */
					set_bit(PPC460EX_DESC_RXOR123,
						&op);
				} else if ((src[1] - len * 2) == src[2]) {
					/* do RXOR R1 R2 R4 */
					set_bit(PPC460EX_DESC_RXOR124, &op);
				} else if ((src[1] - len * 3) == src[2]) {
					/* do RXOR R1 R2 R5 */
					set_bit(PPC460EX_DESC_RXOR125,
						&op);
				} else {
					/* do RXOR R1 R2 */
					set_bit(PPC460EX_DESC_RXOR12,
						&op);
				}
			} else {
				/* do RXOR R1 R2 */
				set_bit(PPC460EX_DESC_RXOR12, &op);
			}
		}

		if (!test_bit(PPC460EX_DESC_RXOR, &op)) {
			/* can not do this operation with RXOR */
			clear_bit(PPC460EX_RXOR_RUN,
				&ppc460ex_rxor_state);
		} else {
			/* can do; set block size right now */
			ppc460ex_desc_set_rxor_block_size(len);
		}
	}

	/* Number of necessary slots depends on operation type selected */
	if (!test_bit(PPC460EX_DESC_RXOR, &op)) {
		/*  This is a WXOR only chain. Need descriptors for each
		 * source to GF-XOR them with WXOR, and need descriptors
		 * for each destination to zero them with WXOR
		 */
		slot_cnt = src_cnt;

#if 1
		if (flags & DMA_PREP_ZERO_P) {
			slot_cnt += dst_cnt;
			set_bit(PPC460EX_ZERO_P, &op);
		}
#endif /* RAID-6 stuff*/
	} else {
		/*  Need 1 descriptor for RXOR operation, and
		 * need (src_cnt - (2 or 3)) for WXOR of sources
		 * remained (if any)
		 *  Thus we have 1 CDB for RXOR, let the set_dst
		 * function think that this is just a zeroing descriptor
		 * and skip it when walking through the chain.
		 * So set PPC460EX_ZERO_P.
		*/
		set_bit(PPC460EX_ZERO_P, &op);

		if (test_bit(PPC460EX_DESC_RXOR12, &op)) {
			slot_cnt = src_cnt - 1;
		} else {
			slot_cnt = src_cnt - 2;
		}

		/*  Thus we have either RXOR only chain or
		 * mixed RXOR/WXOR
		 */
		if (slot_cnt == 1) {
			/* RXOR only chain */
			clear_bit(PPC460EX_DESC_WXOR, &op);
		}
	}

	spin_lock_bh(&ppc460ex_chan->lock);
	/* for both RXOR/WXOR each descriptor occupies one slot */
	sw_desc = ppc460ex_adma_alloc_slots(ppc460ex_chan, slot_cnt, 1);
	if (sw_desc) {
		ppc460ex_desc_init_dma01_xor(sw_desc, dst_cnt, src_cnt,
				flags, op);

		/* setup dst/src/mult */
		while(dst_cnt--)
			ppc460ex_adma_dma01_xor_set_dest(sw_desc,
				dst, dst_cnt);
				//dst[dst_cnt], dst_cnt); /*marri */
		while(src_cnt--) {
			ppc460ex_adma_dma01_xor_set_src(sw_desc,
				src[src_cnt], src_cnt);
			ppc460ex_adma_dma01_xor_set_src_mult(sw_desc,
				1, src_cnt);/* Marri forcing RAID-5*/
				/*scf[src_cnt], src_cnt);*/
		}

		/* Setup byte count foreach slot just allocated */
		sw_desc->async_tx.flags = flags;
		list_for_each_entry(iter, &sw_desc->group_list,
				chain_node) {
			if(mfdcr(0x60) == 0xfee8) {
				printk("Byte Count = 0x%x\n",len);
				printk("src[0]= 0x%x\n",src[0]);
				printk("src[1]= 0x%x\n",src[1]);
				printk("src[2]= 0x%x\n",src[2]);
				printk("Dest = 0x%x\n",dst);
			}
			ppc460ex_desc_set_byte_count(iter,
				ppc460ex_chan, len);
			iter->unmap_len = len;
		}
	}
		
	spin_unlock_bh(&ppc460ex_chan->lock);

	return sw_desc;
}
#if 1
/**
 * ppc460ex_adma_prep_dma_pq- prepare CDB (group) for a GF-XOR operation
 */
static struct dma_async_tx_descriptor *ppc460ex_adma_prep_dma_pq(
		struct dma_chan *chan, dma_addr_t *dst,	dma_addr_t *src, 
		unsigned int src_cnt, unsigned char *scf,
		size_t len, unsigned long flags)
{
	ppc460ex_ch_t *ppc460ex_chan = to_ppc460ex_adma_chan(chan);
	ppc460ex_desc_t *sw_desc = NULL;
	int dst_cnt = 0;

	BUG_ON(!len);
	BUG_ON(unlikely(len > PPC460EX_ADMA_XOR_MAX_BYTE_COUNT));
	BUG_ON(!src_cnt);

	if (flags & DMA_PREP_HAVE_P) {
		BUG_ON(!dst[0]);
		dst_cnt++;
	} else
		BUG_ON(flags & DMA_PREP_ZERO_P);
	if (flags & DMA_PREP_HAVE_Q) {
		BUG_ON(!dst[1]);
		dst_cnt++;
	} else
		BUG_ON(flags & DMA_PREP_ZERO_Q);
	BUG_ON(!dst_cnt);

	dev_dbg(ppc460ex_chan->device->common.dev,
		"ppc460ex adma%d: %s src_cnt: %d len: %u int_en: %d\n",
		ppc460ex_chan->device->id, __FUNCTION__, src_cnt, len,
		flags & DMA_PREP_INTERRUPT ? 1 : 0);

	switch (ppc460ex_chan->device->id) {
	case PPC460EX_DMA0_ID:
	case PPC460EX_DMA1_ID:
		sw_desc = ppc460ex_dma_prep_pq(ppc460ex_chan,
				dst, dst_cnt, src, src_cnt, scf,
				len, flags);
		break;

	}

	return sw_desc ? &sw_desc->async_tx : NULL;
}
#endif
/**
 * ppc460ex_adma_prep_dma_mq_xor - prepare CDB (group) for a GF-XOR operation
 */
static struct dma_async_tx_descriptor *ppc460ex_adma_prep_dma_mq_xor(
		struct dma_chan *chan, dma_addr_t dst, 
		dma_addr_t *src, unsigned int src_cnt, 
		size_t len, unsigned long flags)
{
	ppc460ex_ch_t *ppc460ex_chan = to_ppc460ex_adma_chan(chan);
	ppc460ex_desc_t *sw_desc = NULL;
	int dst_cnt = 1;

	BUG_ON(!len);
	BUG_ON(unlikely(len > PPC460EX_ADMA_XOR_MAX_BYTE_COUNT));
	BUG_ON(!src_cnt );

//	printk("<%s> line %d\n",__FUNCTION__,__LINE__);
	dev_dbg(ppc460ex_chan->device->common.dev,
		"ppc460ex adma%d: %s src_cnt: %d len: %u int_en: %d\n",
		ppc460ex_chan->device->id, __FUNCTION__, src_cnt, len,
		flags & DMA_PREP_INTERRUPT ? 1 : 0);

	switch (ppc460ex_chan->device->id) {
	case PPC460EX_DMA0_ID:
	case PPC460EX_DMA1_ID:
#if 0
		sw_desc = ppc460ex_dma01_prep_xor (ppc460ex_chan,
				dst, src, src_cnt, 
				len, flags);
#else
		sw_desc = ppc460ex_dma_prep_xor(ppc460ex_chan,
				&dst, dst_cnt, src, src_cnt, 0,
				len, flags);
#endif
		break;

	}

	return sw_desc ? &sw_desc->async_tx : NULL;
}

/**
 * ppc460ex_adma_prep_dma_pqzero_sum - prepare CDB group for
 * a PQ_VAL operation
 */
static struct dma_async_tx_descriptor *ppc460ex_adma_prep_dma_pqzero_sum(
		struct dma_chan *chan, dma_addr_t *src, unsigned int src_cnt,
		unsigned char *scf, size_t len,
		u32 *pqres, unsigned long flags)
{
	ppc460ex_ch_t *ppc460ex_chan = to_ppc460ex_adma_chan(chan);
	ppc460ex_desc_t *sw_desc, *iter;
	dma_addr_t pdest, qdest;
	int slot_cnt, slots_per_op, idst, dst_cnt;

	if (flags & DMA_PREP_HAVE_P)
		pdest = src[src_cnt];
	else
		pdest = 0;
	if (flags & DMA_PREP_HAVE_Q)
		 qdest = src[src_cnt+1];
	else
		qdest = 0;

	BUG_ON(src_cnt < 3 );

	/* Always use WXOR for P/Q calculations (two destinations).
	 * Need two extra slots to verify results are zero. Since src_cnt
	 * is the size of the src[] buffer (which includes destination
	 * pointers at the first and/or second positions) then the number
	 * of actual sources should be reduced by DMA_DEST_MAX_NUM (2).
	 */
	idst = dst_cnt = (pdest && qdest) ? 2 : 1;

	slot_cnt = src_cnt + dst_cnt;
	slots_per_op = 1;

	spin_lock_bh(&ppc460ex_chan->lock);
	sw_desc = ppc460ex_adma_alloc_slots(ppc460ex_chan, slot_cnt,
	    slots_per_op);
	if (sw_desc) {
		ppc460ex_desc_init_pqzero_sum(sw_desc, dst_cnt, src_cnt);

		/* Setup byte count foreach slot just allocated */
		sw_desc->async_tx.flags = flags;
		list_for_each_entry(iter, &sw_desc->group_list, chain_node) {
			ppc460ex_desc_set_byte_count(iter, ppc460ex_chan,
			    len);
			iter->unmap_len = len;
		}

		/* Setup destinations for P/Q ops */
		ppc460ex_adma_pqzero_sum_set_dest(sw_desc, pdest, qdest);

		/* Setup sources and mults for P/Q ops */
		while (src_cnt--) {
			    ppc460ex_adma_pqzero_sum_set_src (sw_desc,
					src[src_cnt], src_cnt);
			    /* Setup mults for Q-check only; in case of P -
			     * keep the default 0 (==1)
			     */
			    if (qdest)
				    ppc460ex_adma_pqzero_sum_set_src_mult (sw_desc,
					scf[src_cnt], src_cnt,dst_cnt - 1 );
		}

		/* Setup zero QWORDs into DCHECK CDBs */
		idst = dst_cnt;
		list_for_each_entry_reverse(iter, &sw_desc->group_list,
		    chain_node) {
			/*
			 * The last CDB corresponds to P-parity check
			 * (if any), the one before last CDB corresponds
			 * Q-parity check
			 */
			if (idst == DMA_DEST_MAX_NUM) {
				if (idst == dst_cnt) {
					set_bit(PPC460EX_DESC_QCHECK,
							&iter->flags);
				} else {
					 set_bit(PPC460EX_DESC_PCHECK,
							 &iter->flags);
				}
			} else {
				if (qdest) {
					set_bit(PPC460EX_DESC_QCHECK,
							&iter->flags);
				} else {
					set_bit(PPC460EX_DESC_PCHECK,
							&iter->flags);
				}
			}
			iter->xor_check_result = pqres;
			/*
			 * set it to zero, if check fail then result will
			 * be updated
			 */
			*iter->xor_check_result = 0;
			ppc460ex_desc_set_dcheck(iter, ppc460ex_chan,
			    ppc460ex_qword);
			if (!(--dst_cnt))
				break;
		}
	}
	spin_unlock_bh(&ppc460ex_chan->lock);
	return sw_desc ? &sw_desc->async_tx : NULL;
}
/**
 * ppc460ex_adma_prep_dma_mq_zero_sum - prepare CDB group for
 * a PQ_VAL operation
 */
static struct dma_async_tx_descriptor *ppc460ex_adma_prep_dma_mq_zero_sum(
		struct dma_chan *chan, dma_addr_t *src, unsigned int src_cnt,
		size_t len, u32 *presult,  unsigned long flags)
{
	ppc460ex_ch_t *ppc460ex_chan = to_ppc460ex_adma_chan(chan);
	ppc460ex_desc_t *sw_desc, *iter;
	int slot_cnt, slots_per_op, idst, dst_cnt;

	BUG_ON(src_cnt < 3 || !src[0]);

	/* Always use WXOR for P/Q calculations (two destinations).
	 * Need two extra slots to verify results are zero. Since src_cnt
	 * is the size of the src[] buffer (which includes destination
	 * pointers at the first and/or second positions) then the number
	 * of actual sources should be reduced by DMA_DEST_MAX_NUM (2).
	 */
	idst = dst_cnt = 1;
	slot_cnt = src_cnt ;
	src_cnt -= dst_cnt;
	slots_per_op = 1;
		
	spin_lock_bh(&ppc460ex_chan->lock);
	sw_desc = ppc460ex_adma_alloc_slots(ppc460ex_chan, slot_cnt,
	    slots_per_op);
	if (sw_desc) {
		ppc460ex_desc_init_pqzero_sum(sw_desc, dst_cnt, src_cnt);

		/* Setup byte count foreach slot just allocated */
		sw_desc->async_tx.flags = flags;
		list_for_each_entry(iter, &sw_desc->group_list, chain_node) {
			ppc460ex_desc_set_byte_count(iter, ppc460ex_chan,
			    len);
			iter->unmap_len = len;
		}

		/* Setup destinations for P/Q ops */
				
		ppc460ex_adma_mq_zero_sum_set_dest(sw_desc, src[0]);

		/* Setup sources and mults for P/Q ops */
		src = &src[dst_cnt];
		while (src_cnt-- ) {
			    ppc460ex_adma_pqzero_sum_set_src (sw_desc,
					src[src_cnt ], src_cnt);
		}

		/* Setup zero QWORDs into DCHECK CDBs */
		idst = dst_cnt;
		list_for_each_entry_reverse(iter, &sw_desc->group_list,
		    chain_node) {
			/*
			 * The last CDB corresponds to P-parity check
			 * (if any), the one before last CDB corresponds
			 * Q-parity check
			 */
				iter->xor_check_result = presult;
			/*
			 * set it to zero, if check fail then result will
			 * be updated
			 */
			*iter->xor_check_result = 0;
			ppc460ex_desc_set_dcheck(iter, ppc460ex_chan,
			    ppc460ex_qword);
			if (!(--dst_cnt))
				break;
		}
	}
	spin_unlock_bh(&ppc460ex_chan->lock);
	return sw_desc ? &sw_desc->async_tx : NULL;
}

static void ppc460ex_adma_pq_zero_op(ppc460ex_desc_t *iter,
		ppc460ex_ch_t *chan, dma_addr_t addr)
{
	/*  To clear destinations update the descriptor
	 * (P or Q depending on index) as follows:
	 * addr is destination (0 corresponds to SG2):
	 */
	ppc460ex_desc_set_dest_addr(iter, chan, DMA_CUED_XOR_BASE, addr, 0);

	/* ... and the addr is source: */
	ppc460ex_desc_set_src_addr(iter, chan, 0, DMA_CUED_XOR_HB, addr);

	/* addr is always SG2 then the mult is always DST1 */
	//ppc460ex_desc_set_src_mult(iter, chan, DMA_CUED_MULT1_OFF,
	//			    DMA_CDB_SG_DST1, 1);
}
/**
 * ppc460ex_adma_set_dest - set destination address into descriptor
 */
static inline void ppc460ex_adma_set_dest(ppc460ex_desc_t *sw_desc,
		dma_addr_t addr, int index)
{
	ppc460ex_ch_t *chan = to_ppc460ex_adma_chan(sw_desc->async_tx.chan);
	BUG_ON(index >= sw_desc->dst_cnt);

	switch (chan->device->id) {
	case PPC460EX_DMA1_ID:
		/* to do: support transfers lengths >
		 * PPC460EX_ADMA_DMA/XOR_MAX_BYTE_COUNT
		 */
		ppc460ex_desc_set_dest_addr(sw_desc->group_head,
		//	chan, 0x8, addr, index); // Enabling HB bus
			chan, 0, addr, index);
		break;
	case PPC460EX_XOR_ID:
		sw_desc = ppc460ex_get_group_entry(sw_desc, index);
		ppc460ex_desc_set_dest_addr(sw_desc,
			chan, 0, addr, index);
		break;
	}
}


static void ppc460ex_adma_dma2rxor_set_dest (
		ppc460ex_desc_t *sw_desc,
		dma_addr_t addr, int index)
{
	ppc460ex_ch_t *chan = to_ppc460ex_adma_chan(sw_desc->async_tx.chan);
	ppc460ex_desc_t *iter;
	int i;

	switch (chan->device->id) {
	case PPC460EX_DMA0_ID:
	case PPC460EX_DMA1_ID:
		BUG();
		break;
	case PPC460EX_XOR_ID:
		iter = ppc460ex_get_group_entry(sw_desc,
			sw_desc->descs_per_op*index);
		for (i=0;i<sw_desc->descs_per_op;i++) {
			ppc460ex_desc_set_dest_addr(iter,
				chan, 0, addr, index);
			if (i) ppc460ex_wxor_set_base (iter);
			iter = list_entry (iter->chain_node.next,
				ppc460ex_desc_t, chain_node);
		}
		break;
	}
}

/**
 * ppc460ex_adma_pq_xor_set_dest - set destination address into descriptor
 * for the PQXOR operation
 */
static void ppc460ex_adma_pqxor_set_dest(ppc460ex_desc_t *sw_desc,
		dma_addr_t *addrs, unsigned long flags)
{
	ppc460ex_desc_t *iter;
	ppc460ex_ch_t *chan = to_ppc460ex_adma_chan(sw_desc->async_tx.chan);
	dma_addr_t paddr, qaddr;
	dma_addr_t addr = 0, ppath, qpath;
	int index = 0, i;

	if (flags & DMA_PREP_HAVE_P)
		paddr = addrs[0];
	else
		paddr = 0;

	if (flags & DMA_PREP_HAVE_Q)
		qaddr = addrs[1];
	else
		qaddr = 0;

	if (!paddr || !qaddr)
		addr = paddr ? paddr : qaddr;

	switch (chan->device->id) {
	case PPC460EX_DMA0_ID:
	case PPC460EX_DMA1_ID:
		/* walk through the WXOR source list and set P/Q-destinations
		 * for each slot:
		 */
		if (!test_bit(PPC460EX_DESC_RXOR, &sw_desc->flags)) {
			/* This is WXOR-only chain; may have 1/2 zero descs */
			if (test_bit(PPC460EX_ZERO_P, &sw_desc->flags))
				index++;
			if (test_bit(PPC460EX_ZERO_Q, &sw_desc->flags))
				index++;

			iter = ppc460ex_get_group_entry(sw_desc, index);
			if (addr) {
				/* one destination */
				list_for_each_entry_from(iter,
					&sw_desc->group_list, chain_node)
					ppc460ex_desc_set_dest_addr(iter, chan,
						DMA_CUED_XOR_BASE, addr, 0);
			} else {
				/* two destinations */
				list_for_each_entry_from(iter,
					&sw_desc->group_list, chain_node) {
					ppc460ex_desc_set_dest_addr(iter, chan,
						DMA_CUED_XOR_BASE, paddr, 0);
					ppc460ex_desc_set_dest_addr(iter, chan,
						DMA_CUED_XOR_BASE, qaddr, 1);
				}
			}

			if (index) {
				/*  To clear destinations update the descriptor
				 * (1st,2nd, or both depending on flags)
				 */
				index = 0;
				if (test_bit(PPC460EX_ZERO_P,
						&sw_desc->flags)) {
					iter = ppc460ex_get_group_entry(
							sw_desc, index++);
					ppc460ex_adma_pq_zero_op(iter, chan,
							paddr);
				}

				if (test_bit(PPC460EX_ZERO_Q,
						&sw_desc->flags)) {
					iter = ppc460ex_get_group_entry(
							sw_desc, index++);
					ppc460ex_adma_pq_zero_op(iter, chan,
							qaddr);
				}

				return;
			}
		} else {
			/* This is RXOR-only or RXOR/WXOR mixed chain */

			/* If we want to include destination into calculations,
			 * then make dest addresses cued with mult=1 (XOR).
			 */
			ppath = test_bit(PPC460EX_ZERO_P, &sw_desc->flags) ?
					DMA_CUED_XOR_HB :
					DMA_CUED_XOR_BASE |
						(1 << DMA_CUED_MULT1_OFF);
			qpath = test_bit(PPC460EX_ZERO_Q, &sw_desc->flags) ?
					DMA_CUED_XOR_HB :
					DMA_CUED_XOR_BASE |
						(1 << DMA_CUED_MULT1_OFF);

			/* Setup destination(s) in RXOR slot(s) */
			iter = ppc460ex_get_group_entry (sw_desc, index++);
			ppc460ex_desc_set_dest_addr(iter, chan,
						paddr ? ppath : qpath,
						paddr ? paddr : qaddr, 0);
			if (!addr) {
				/* two destinations */
				iter = ppc460ex_get_group_entry (sw_desc,
								  index++);
				ppc460ex_desc_set_dest_addr(iter, chan,
						qpath, qaddr, 0);
			}

			if (test_bit(PPC460EX_DESC_WXOR, &sw_desc->flags)) {
				/* Setup destination(s) in remaining WXOR
				 * slots
				 */
				iter = ppc460ex_get_group_entry(sw_desc,
								 index);
	                        if (addr) {
					/* one destination */
					list_for_each_entry_from(iter,
					    &sw_desc->group_list,
					    chain_node)
						ppc460ex_desc_set_dest_addr(
							iter, chan,
							DMA_CUED_XOR_BASE,
							addr, 0);

				} else {
					/* two destinations */
					list_for_each_entry_from(iter,
					    &sw_desc->group_list,
					    chain_node) {
						ppc460ex_desc_set_dest_addr(
							iter, chan,
							DMA_CUED_XOR_BASE,
							paddr, 0);
						ppc460ex_desc_set_dest_addr(
							iter, chan,
							DMA_CUED_XOR_BASE,
							qaddr, 1);
					}
				}
			}

		}
		break;

	case PPC460EX_XOR_ID:
		/* DMA2 descriptors have only 1 destination, so there are
		 * two chains - one for each dest.
		 * If we want to include destination into calculations,
		 * then make dest addresses cued with mult=1 (XOR).
		 */
		ppath = test_bit(PPC460EX_ZERO_P, &sw_desc->flags) ?
				DMA_CUED_XOR_HB :
				DMA_CUED_XOR_BASE |
					(1 << DMA_CUED_MULT1_OFF);

		qpath = test_bit(PPC460EX_ZERO_Q, &sw_desc->flags) ?
				DMA_CUED_XOR_HB :
				DMA_CUED_XOR_BASE |
					(1 << DMA_CUED_MULT1_OFF);

		iter = ppc460ex_get_group_entry (sw_desc, 0);
		for (i=0; i<sw_desc->descs_per_op; i++) {
			ppc460ex_desc_set_dest_addr(iter, chan,
				paddr ? ppath : qpath,
				paddr ? paddr : qaddr, 0);
			iter = list_entry (iter->chain_node.next,
				ppc460ex_desc_t, chain_node);
		}

		if (!addr) {
			/* Two destinations; setup Q here */
			iter = ppc460ex_get_group_entry (sw_desc,
				sw_desc->descs_per_op);
			for (i=0; i<sw_desc->descs_per_op; i++) {
				ppc460ex_desc_set_dest_addr(iter,
					chan, qpath, qaddr, 0);
				iter = list_entry (iter->chain_node.next,
					ppc460ex_desc_t, chain_node);
			}
		}

		break;
	}
}
void ppc460ex_adma_xor_set_dest(ppc460ex_desc_t *sw_desc,
		dma_addr_t *addrs, unsigned long flags)
{
	ppc460ex_desc_t *iter;
	ppc460ex_ch_t *chan = to_ppc460ex_adma_chan(sw_desc->async_tx.chan);
	dma_addr_t paddr, qaddr;
	dma_addr_t addr = 0, ppath, qpath;
	int index = 0;

	paddr = addrs[0];



	switch (chan->device->id) {
	case PPC460EX_DMA0_ID:
	case PPC460EX_DMA1_ID:
		/* walk through the WXOR source list and set P/Q-destinations
		 * for each slot:
		 */
		if (!test_bit(PPC460EX_DESC_RXOR, &sw_desc->flags)) {
			/* This is WXOR-only chain; may have 1/2 zero descs */
			if (test_bit(PPC460EX_ZERO_P, &sw_desc->flags))
				index++;

			iter = ppc460ex_get_group_entry(sw_desc, index);
			if (paddr) {
				/* one destination */
				list_for_each_entry_from(iter,
					&sw_desc->group_list, chain_node)
					ppc460ex_desc_set_dest_addr(iter, chan,
						DMA_CUED_XOR_BASE, paddr, 0);
			} else {
				BUG();
				/* two destinations */
				list_for_each_entry_from(iter,
					&sw_desc->group_list, chain_node) {
					ppc460ex_desc_set_dest_addr(iter, chan,
						DMA_CUED_XOR_BASE, paddr, 0);
					ppc460ex_desc_set_dest_addr(iter, chan,
						DMA_CUED_XOR_BASE, qaddr, 1);
				}
			}

			if (index) {
				/*  To clear destinations update the descriptor
				 * (1st,2nd, or both depending on flags)
				 */
				index = 0;
				if (test_bit(PPC460EX_ZERO_P,
						&sw_desc->flags)) {
					iter = ppc460ex_get_group_entry(
							sw_desc, index++);
					ppc460ex_adma_pq_zero_op(iter, chan,
							paddr);
				}

				return;
			}
		} else {
			/* This is RXOR-only or RXOR/WXOR mixed chain */

			/* If we want to include destination into calculations,
			 * then make dest addresses cued with mult=1 (XOR).
			 */
			ppath = test_bit(PPC460EX_ZERO_P, &sw_desc->flags) ?
					DMA_CUED_XOR_HB :
					DMA_CUED_XOR_BASE |
						(1 << DMA_CUED_MULT1_OFF);

			/* Setup destination(s) in RXOR slot(s) */
			iter = ppc460ex_get_group_entry (sw_desc, index++);
			ppc460ex_desc_set_dest_addr(iter, chan,
						paddr ? ppath : qpath,
						paddr ? paddr : qaddr, 0);
			if (!addr) {
				/* two destinations */
				iter = ppc460ex_get_group_entry (sw_desc,
								  index++);
				ppc460ex_desc_set_dest_addr(iter, chan,
						qpath, qaddr, 0);
			}

			if (test_bit(PPC460EX_DESC_WXOR, &sw_desc->flags)) {
				/* Setup destination(s) in remaining WXOR
				 * slots
				 */
				iter = ppc460ex_get_group_entry(sw_desc,
								 index);
	                        if (addr) {
					/* one destination */
					list_for_each_entry_from(iter,
					    &sw_desc->group_list,
					    chain_node)
						ppc460ex_desc_set_dest_addr(
							iter, chan,
							DMA_CUED_XOR_BASE,
							addr, 0);

				} else {
					/* two destinations */
					list_for_each_entry_from(iter,
					    &sw_desc->group_list,
					    chain_node) {
						ppc460ex_desc_set_dest_addr(
							iter, chan,
							DMA_CUED_XOR_BASE,
							paddr, 0);
						ppc460ex_desc_set_dest_addr(
							iter, chan,
							DMA_CUED_XOR_BASE,
							qaddr, 1);
					}
				}
			}

		}
		break;

	}
}
/**
 * ppc460ex_adma_dma01_xor_set_dest - set destination address into descriptor
 * for the PQXOR operation
 */
static void ppc460ex_adma_dma01_xor_set_dest(ppc460ex_desc_t *sw_desc,
		dma_addr_t addr, int index)
{
	ppc460ex_desc_t *iter;
	ppc460ex_ch_t *chan = to_ppc460ex_adma_chan(sw_desc->async_tx.chan);

	BUG_ON(index >= sw_desc->dst_cnt);
		BUG_ON(test_bit(PPC460EX_DESC_RXOR, &sw_desc->flags) && index);

	switch (chan->device->id) {
	case PPC460EX_DMA0_ID:
	case PPC460EX_DMA1_ID:
		/* walk through the WXOR source list and set P/Q-destinations
		 * for each slot:
		 */
		if (test_bit(PPC460EX_DESC_WXOR, &sw_desc->flags)) {
			/* If this is RXOR/WXOR chain then dst_cnt == 1
			 * and first WXOR descriptor is the second in RXOR/WXOR
			 * chain
			 */
			//if (!test_bit(PPC460EX_ZERO_P, &sw_desc->flags)) {
			if (!test_bit(PPC460EX_ZERO_P, &sw_desc->flags)) {
				iter = ppc460ex_get_group_entry(sw_desc, 0);
			} else {
				iter = ppc460ex_get_group_entry(sw_desc,
						sw_desc->dst_cnt);
			}
			list_for_each_entry_from(iter, &sw_desc->group_list,
					chain_node) {
				ppc460ex_desc_set_dest_addr(iter, chan,
					DMA_CUED_XOR_BASE, addr, index);
			}
			if (!test_bit(PPC460EX_DESC_RXOR, &sw_desc->flags) &&
			     test_bit(PPC460EX_ZERO_P, &sw_desc->flags)) {
				/*  In a WXOR-only case we probably has had
				 * a reasonable data at P/Q addresses, so
				 * the first operation in chain will be
				 * zeroing P/Q dest:
				 * WXOR (Q, 1*Q) -> 0.
				 *
				 *  To do this (clear) update the descriptor
				 * (P or Q depending on index) as follows:
				 * addr is destination (0 corresponds to SG2):
				 */
				iter = ppc460ex_get_group_entry (sw_desc,
					index);
				ppc460ex_desc_set_dest_addr(iter, chan,
				    DMA_CUED_XOR_BASE, addr, 0);
				/* ... and the addr is source: */
				ppc460ex_desc_set_src_addr(iter, chan, 0,
				    DMA_CUED_XOR_HB, addr);
				/* addr is always SG2 then the mult is always
					DST1 */
				ppc460ex_desc_set_src_mult(iter, chan,
				    DMA_CUED_MULT1_OFF, DMA_CDB_SG_DST1, 1);
			}
		}

		if (test_bit(PPC460EX_DESC_RXOR, &sw_desc->flags)) {
			/*
			 * setup Q-destination for RXOR slot (
			 * it shall be a HB address)
			 */
			iter = ppc460ex_get_group_entry (sw_desc, index);
			ppc460ex_desc_set_dest_addr(iter, chan,
						DMA_CUED_XOR_HB, addr, 0);
		}
		break;
	case PPC460EX_XOR_ID:
		iter = ppc460ex_get_group_entry (sw_desc, index);
		ppc460ex_desc_set_dest_addr(iter, chan, 0, addr, 0);
		break;
	}
}

/**
 * ppc460ex_adma_pq_zero_sum_set_dest - set destination address into descriptor
 * for the PQ_VAL operation
 */
static void ppc460ex_adma_mq_zero_sum_set_dest	(
		ppc460ex_desc_t *sw_desc,
		dma_addr_t addr)
{
	ppc460ex_desc_t *iter, *end;
	ppc460ex_ch_t *chan = to_ppc460ex_adma_chan(sw_desc->async_tx.chan);


	/* walk through the WXOR source list and set P/Q-destinations
	 * for each slot
	 */
	end = ppc460ex_get_group_entry(sw_desc, sw_desc->src_cnt);
	list_for_each_entry(iter, &sw_desc->group_list, chain_node) {
		if (unlikely(iter == end))
			break;
		ppc460ex_desc_set_dest_addr(iter, chan, DMA_CUED_XOR_BASE,
			 addr, 0);
	}
	/*  The descriptors remain are DATACHECK. These have no need in
	 * destination. Actually, these destination are used there
	 * as a sources for check operation. So, set addr ass source.
	 */
	end = ppc460ex_get_group_entry(sw_desc, sw_desc->src_cnt );
	BUG_ON(!end);
	ppc460ex_desc_set_src_addr(end, chan, 0, 0, addr);
}
static void ppc460ex_adma_pqzero_sum_set_dest	(
		ppc460ex_desc_t *sw_desc,
		dma_addr_t paddr, dma_addr_t qaddr)
{
	ppc460ex_desc_t *iter, *end;
	ppc460ex_ch_t *chan = to_ppc460ex_adma_chan(sw_desc->async_tx.chan);
	 dma_addr_t addr = 0;


	/* walk through the WXOR source list and set P/Q-destinations
	 * for each slot
	 */
	end = ppc460ex_get_group_entry(sw_desc, sw_desc->src_cnt);
	if(paddr && qaddr) {
		list_for_each_entry(iter, &sw_desc->group_list, chain_node) {
			if (unlikely(iter == end))
				break;
			ppc460ex_desc_set_dest_addr(iter, chan, DMA_CUED_XOR_BASE,
				 paddr, 0);
			ppc460ex_desc_set_dest_addr(iter, chan, DMA_CUED_XOR_BASE,
				 qaddr, 1);
		}
	} else {
		/* one destination */
		addr = paddr ? paddr : qaddr;
		list_for_each_entry(iter, &sw_desc->group_list, chain_node) {
			if (unlikely(iter == end))
				break;
			 ppc460ex_desc_set_dest_addr(iter, chan,
					  DMA_CUED_XOR_BASE, addr, 0);
		}
	}
	/*  The descriptors remain are DATACHECK. These have no need in
	 * destination. Actually, these destination are used there
	 * as a sources for check operation. So, set addr ass source.
	 */
	end = ppc460ex_get_group_entry(sw_desc, sw_desc->src_cnt );
	ppc460ex_desc_set_src_addr(end, chan, 0, 0, addr ? addr : paddr);
	if (!addr) {
		end = ppc460ex_get_group_entry(sw_desc, sw_desc->src_cnt + 1);
		ppc460ex_desc_set_src_addr(end, chan, 0, 0, qaddr);
	}
}

/**
 * ppc460ex_desc_set_xor_src_cnt (ppc460ex_desc_t *desc, int src_cnt)
 */
static inline void ppc460ex_desc_set_xor_src_cnt (ppc460ex_desc_t *desc,
		int src_cnt)
{
	xor_cb_t *hw_desc = desc->hw_desc;
	hw_desc->cbc &= ~XOR_CDCR_OAC_MSK;
	hw_desc->cbc |= src_cnt;
}

/**
 * ppc460ex_adma_pqxor_set_src - set source address into descriptor
 */
static void ppc460ex_adma_pqxor_set_src(
		ppc460ex_desc_t *sw_desc,
		dma_addr_t addr,
		int index)
{
	ppc460ex_ch_t *chan = to_ppc460ex_adma_chan(sw_desc->async_tx.chan);
	dma_addr_t haddr = 0;
	ppc460ex_desc_t *iter;

	switch (chan->device->id) {
	case PPC460EX_DMA0_ID:
	case PPC460EX_DMA1_ID:
		/* DMA0,1 may do: WXOR, RXOR, RXOR+WXORs chain
		 */
		if (test_bit(PPC460EX_DESC_RXOR, &sw_desc->flags)) {
			/* RXOR-only or RXOR/WXOR operation */
			int iskip = test_bit(PPC460EX_DESC_RXOR12,
				&sw_desc->flags) ?  2 : 3;

			if (index == 0) {
				/* 1st slot (RXOR) */
				/* setup sources region (R1-2-3, R1-2-4,
					or R1-2-5)*/
				if (test_bit(PPC460EX_DESC_RXOR12,
						&sw_desc->flags))
					haddr = DMA_RXOR12 <<
						DMA_CUED_REGION_OFF;
			else if (test_bit(PPC460EX_DESC_RXOR123,
			    &sw_desc->flags))
					haddr = DMA_RXOR123 <<
						DMA_CUED_REGION_OFF;
			else if (test_bit(PPC460EX_DESC_RXOR124,
			    &sw_desc->flags))
					haddr = DMA_RXOR124 <<
						DMA_CUED_REGION_OFF;
			else if (test_bit(PPC460EX_DESC_RXOR125,
			    &sw_desc->flags))
					haddr = DMA_RXOR125 <<
						DMA_CUED_REGION_OFF;
				else
					BUG();
				haddr |= DMA_CUED_XOR_BASE;
				sw_desc = sw_desc->group_head;
			} else if (index < iskip) {
				/* 1st slot (RXOR)
				 * shall actually set source address only once
				 * instead of first <iskip>
				 */
				sw_desc = NULL;
			} else {
				/* second and next slots (WXOR);
				 * skip first slot with RXOR
				 */
				haddr = DMA_CUED_XOR_HB;
				sw_desc = ppc460ex_get_group_entry(sw_desc,
				    index - iskip + 1);
			}
		} else {
			/* WXOR-only operation;
			 * skip first slots with destinations
			 */
			haddr = DMA_CUED_XOR_HB;
			if (!test_bit(PPC460EX_ZERO_P, &sw_desc->flags))
				sw_desc = ppc460ex_get_group_entry(sw_desc,
					index);
			else
				sw_desc = ppc460ex_get_group_entry(sw_desc,
					sw_desc->dst_cnt + index);
		}

		if (likely(sw_desc))
			ppc460ex_desc_set_src_addr(sw_desc, chan, index, haddr,
					addr);
		break;
	case PPC460EX_XOR_ID:
		/* DMA2 may do Biskup
		 */
		iter = sw_desc->group_head;
		if (iter->dst_cnt == 2) {
			/* both P & Q calculations required; set Q src here */
			ppc460ex_adma_dma2rxor_set_src(iter, index, addr);
			/* this is for P. Actually sw_desc already points
			 * to the second CDB though.
			 */
			iter = ppc460ex_get_group_entry(sw_desc,
				sw_desc->descs_per_op);
		}
		ppc460ex_adma_dma2rxor_set_src(iter, index, addr);
		break;
	}
}
void ppc460ex_adma_xor_set_src(
		ppc460ex_desc_t *sw_desc,
		dma_addr_t addr,
		int index)
{
	ppc460ex_ch_t *chan = to_ppc460ex_adma_chan(sw_desc->async_tx.chan);
	dma_addr_t haddr = 0;

	switch (chan->device->id) {
	case PPC460EX_DMA0_ID:
	case PPC460EX_DMA1_ID:
		/* DMA0,1 may do: WXOR, RXOR, RXOR+WXORs chain
		 */
		if (test_bit(PPC460EX_DESC_RXOR, &sw_desc->flags)) {
			/* RXOR-only or RXOR/WXOR operation */
			int iskip = test_bit(PPC460EX_DESC_RXOR12,
				&sw_desc->flags) ?  2 : 3;

			if (index == 0) {
				/* 1st slot (RXOR) */
				/* setup sources region (R1-2-3, R1-2-4,
					or R1-2-5)*/
				if (test_bit(PPC460EX_DESC_RXOR12,
						&sw_desc->flags))
					haddr = DMA_RXOR12 <<
						DMA_CUED_REGION_OFF;
			else if (test_bit(PPC460EX_DESC_RXOR123,
			    &sw_desc->flags))
					haddr = DMA_RXOR123 <<
						DMA_CUED_REGION_OFF;
			else if (test_bit(PPC460EX_DESC_RXOR124,
			    &sw_desc->flags))
					haddr = DMA_RXOR124 <<
						DMA_CUED_REGION_OFF;
			else if (test_bit(PPC460EX_DESC_RXOR125,
			    &sw_desc->flags))
					haddr = DMA_RXOR125 <<
						DMA_CUED_REGION_OFF;
				else
					BUG();
				haddr |= DMA_CUED_XOR_BASE;
				sw_desc = sw_desc->group_head;
			} else if (index < iskip) {
				/* 1st slot (RXOR)
				 * shall actually set source address only once
				 * instead of first <iskip>
				 */
				sw_desc = NULL;
			} else {
				/* second and next slots (WXOR);
				 * skip first slot with RXOR
				 */
				haddr = DMA_CUED_XOR_HB;
				sw_desc = ppc460ex_get_group_entry(sw_desc,
				    index - iskip + 1);
			}
		} else {
			/* WXOR-only operation;
			 * skip first slots with destinations
			 */
			haddr = DMA_CUED_XOR_HB;
			if (!test_bit(PPC460EX_ZERO_P, &sw_desc->flags))
				sw_desc = ppc460ex_get_group_entry(sw_desc,
					index);
			else
				sw_desc = ppc460ex_get_group_entry(sw_desc,
					sw_desc->dst_cnt + index);
		}

		if (likely(sw_desc))
			ppc460ex_desc_set_src_addr(sw_desc, chan, index, haddr,
					addr);
		break;
	}
}
/**
 * ppc460ex_adma_dma01_xor_set_src - set source address into descriptor
 */
static void ppc460ex_adma_dma01_xor_set_src(
		ppc460ex_desc_t *sw_desc,
		dma_addr_t addr,
		int index)
{
	ppc460ex_ch_t *chan = to_ppc460ex_adma_chan(sw_desc->async_tx.chan);
	dma_addr_t haddr = 0;
	ppc460ex_desc_t *iter;

	switch (chan->device->id) {
	case PPC460EX_DMA0_ID:
	case PPC460EX_DMA1_ID:
		/* DMA0,1 may do: WXOR, RXOR, RXOR+WXORs chain
		 */
		if (test_bit(PPC460EX_DESC_RXOR, &sw_desc->flags)) {
			/* RXOR-only or RXOR/WXOR operation */
			int iskip = test_bit(PPC460EX_DESC_RXOR12,
				&sw_desc->flags) ?  2 : 3;

			if (index == 0) {
				/* 1st slot (RXOR) */
				/* setup sources region (R1-2-3, R1-2-4,
					or R1-2-5)*/
				if (test_bit(PPC460EX_DESC_RXOR12,
						&sw_desc->flags))
					haddr = DMA_RXOR12 <<
						DMA_CUED_REGION_OFF;
			else if (test_bit(PPC460EX_DESC_RXOR123,
			    &sw_desc->flags))
					haddr = DMA_RXOR123 <<
						DMA_CUED_REGION_OFF;
			else if (test_bit(PPC460EX_DESC_RXOR124,
			    &sw_desc->flags))
					haddr = DMA_RXOR124 <<
						DMA_CUED_REGION_OFF;
			else if (test_bit(PPC460EX_DESC_RXOR125,
			    &sw_desc->flags))
					haddr = DMA_RXOR125 <<
						DMA_CUED_REGION_OFF;
				else
					BUG();
				haddr |= DMA_CUED_XOR_BASE;
				sw_desc = sw_desc->group_head;
			} else if (index < iskip) {
				/* 1st slot (RXOR)
				 * shall actually set source address only once
				 * instead of first <iskip>
				 */
				sw_desc = NULL;
			} else {
				/* second and next slots (WXOR);
				 * skip first slot with RXOR
				 */
				haddr = DMA_CUED_XOR_HB;
				sw_desc = ppc460ex_get_group_entry(sw_desc,
				    index - iskip + 1);
			}
		} else {
			/* WXOR-only operation;
			 * skip first slots with destinations
			 */
			haddr = DMA_CUED_XOR_HB;
			if (!test_bit(PPC460EX_ZERO_P, &sw_desc->flags)) {
				sw_desc = ppc460ex_get_group_entry(sw_desc,
					index);
			} else {
				sw_desc = ppc460ex_get_group_entry(sw_desc,
					sw_desc->dst_cnt + index);
			}
		}

		if (likely(sw_desc)) {
			ppc460ex_desc_set_src_addr(sw_desc, chan, index, haddr,
					addr);
		}
		break;
	case PPC460EX_XOR_ID:
		/* DMA2 may do Biskup
		 */
		iter = sw_desc->group_head;
		if (iter->dst_cnt == 2) {
			/* both P & Q calculations required; set Q src here */
			ppc460ex_adma_dma2rxor_set_src(iter, index, addr);
			/* this is for P. Actually sw_desc already points
			 * to the second CDB though.
			 */
			iter = ppc460ex_get_group_entry(sw_desc,
				sw_desc->descs_per_op);
		}
		ppc460ex_adma_dma2rxor_set_src(iter, index, addr);
		break;
	}
}

/**
 * ppc460ex_adma_pqzero_sum_set_src - set source address into descriptor
 */
static void ppc460ex_adma_pqzero_sum_set_src(
		ppc460ex_desc_t *sw_desc,
		dma_addr_t addr,
		int index)
{
	ppc460ex_ch_t *chan = to_ppc460ex_adma_chan(sw_desc->async_tx.chan);
	dma_addr_t haddr = DMA_CUED_XOR_HB;

	sw_desc = ppc460ex_get_group_entry(sw_desc, index);

	if (likely(sw_desc))
		ppc460ex_desc_set_src_addr(sw_desc, chan, index, haddr, addr);
}

/**
 * ppc460ex_adma_memcpy_xor_set_src - set source address into descriptor
 */
static inline void ppc460ex_adma_memcpy_xor_set_src(
		ppc460ex_desc_t *sw_desc,
		dma_addr_t addr,
		int index)
{
	ppc460ex_ch_t *chan = to_ppc460ex_adma_chan(sw_desc->async_tx.chan);

	sw_desc = sw_desc->group_head;

	if (likely(sw_desc))
		//ppc460ex_desc_set_src_addr(sw_desc, chan, index, 0x8, addr); // Enabling HB bus
		ppc460ex_desc_set_src_addr(sw_desc, chan, index, 0, addr);
}

/**
 * ppc460ex_adma_dma2rxor_inc_addr  -
 */
static void ppc460ex_adma_dma2rxor_inc_addr (ppc460ex_desc_t *desc,
	ppc460ex_rxor_cursor_t *cursor, int index, int src_cnt)
{
	cursor->addr_count++;
	if (index == src_cnt-1) {
		ppc460ex_desc_set_xor_src_cnt (desc,
			cursor->addr_count);
		if (cursor->desc_count) {
			ppc460ex_wxor_set_base (desc);
		}
	} else if (cursor->addr_count == XOR_MAX_OPS) {
		ppc460ex_desc_set_xor_src_cnt (desc,
			cursor->addr_count);
		if (cursor->desc_count) {
			ppc460ex_wxor_set_base (desc);
		}
		cursor->addr_count = 0;
		cursor->desc_count++;
	}
}

/**
 * ppc460ex_adma_dma2rxor_prep_src - setup RXOR types in DMA2 CDB
 */
static int ppc460ex_adma_dma2rxor_prep_src (ppc460ex_desc_t *hdesc,
		ppc460ex_rxor_cursor_t *cursor, int index,
		int src_cnt, u32 addr)
{
	int rval = 0;
	u32 sign;
	ppc460ex_desc_t *desc = hdesc;
	int i;

	for (i=0;i<cursor->desc_count;i++) {
		desc = list_entry (hdesc->chain_node.next, ppc460ex_desc_t,
			chain_node);
	}

	switch (cursor->state) {
		case 0:
			if (addr == cursor->addrl + cursor->len ) {
				/* direct RXOR */
				cursor->state = 1;
				cursor->xor_count++;
				if (index == src_cnt-1) {
					ppc460ex_rxor_set_region (desc,
						cursor->addr_count,
						DMA_RXOR12 <<
							DMA_CUED_REGION_OFF);
					ppc460ex_adma_dma2rxor_inc_addr (
						desc, cursor, index, src_cnt);
				}
			} else if (cursor->addrl == addr + cursor->len) {
				/* reverse RXOR */
				cursor->state = 1;
				cursor->xor_count++;
				set_bit (cursor->addr_count,
						&desc->reverse_flags[0]);
				if (index == src_cnt-1) {
					ppc460ex_rxor_set_region (desc,
						cursor->addr_count,
						DMA_RXOR12 <<
							DMA_CUED_REGION_OFF);
					ppc460ex_adma_dma2rxor_inc_addr (
						desc, cursor, index, src_cnt);
				}
			} else {
				printk (KERN_ERR "Cannot build "
					"DMA2 RXOR command block.\n");
				BUG ();
			}
			break;
		case 1:
			sign = test_bit (cursor->addr_count,
					desc->reverse_flags)
				? -1 : 1;
			if (index == src_cnt-2 || (sign == -1
				&& addr != cursor->addrl - 2*cursor->len)) {
				cursor->state = 0;
				cursor->xor_count = 1;
				cursor->addrl = addr;
				ppc460ex_rxor_set_region (desc,
					cursor->addr_count,
					DMA_RXOR12 << DMA_CUED_REGION_OFF);
				ppc460ex_adma_dma2rxor_inc_addr (
					desc, cursor, index, src_cnt);
			} else if (addr == cursor->addrl + 2*sign*cursor->len) {
				cursor->state = 2;
				cursor->xor_count = 0;
				ppc460ex_rxor_set_region (desc,
					cursor->addr_count,
					DMA_RXOR123 << DMA_CUED_REGION_OFF);
				if (index == src_cnt-1) {
					ppc460ex_adma_dma2rxor_inc_addr (
						desc, cursor, index, src_cnt);
				}
			} else if (addr == cursor->addrl + 3*cursor->len) {
				cursor->state = 2;
				cursor->xor_count = 0;
				ppc460ex_rxor_set_region (desc,
					cursor->addr_count,
					DMA_RXOR124 << DMA_CUED_REGION_OFF);
				if (index == src_cnt-1) {
					ppc460ex_adma_dma2rxor_inc_addr (
						desc, cursor, index, src_cnt);
				}
			} else if (addr == cursor->addrl + 4*cursor->len) {
				cursor->state = 2;
				cursor->xor_count = 0;
				ppc460ex_rxor_set_region (desc,
					cursor->addr_count,
					DMA_RXOR125 << DMA_CUED_REGION_OFF);
				if (index == src_cnt-1) {
					ppc460ex_adma_dma2rxor_inc_addr (
						desc, cursor, index, src_cnt);
				}
			} else {
				cursor->state = 0;
				cursor->xor_count = 1;
				cursor->addrl = addr;
				ppc460ex_rxor_set_region (desc,
					cursor->addr_count,
					DMA_RXOR12 << DMA_CUED_REGION_OFF);
				ppc460ex_adma_dma2rxor_inc_addr (
					desc, cursor, index, src_cnt);
			}
			break;
		case 2:
			cursor->state = 0;
			cursor->addrl = addr;
			cursor->xor_count++;
			if (index) {
				ppc460ex_adma_dma2rxor_inc_addr (
					desc, cursor, index, src_cnt);
			}
			break;
	}

	return rval;
}

/**
 * ppc460ex_adma_dma2rxor_set_src - set RXOR source address; it's assumed that
 *	ppc460ex_adma_dma2rxor_prep_src() has already done prior this call
 */
static void ppc460ex_adma_dma2rxor_set_src (ppc460ex_desc_t *desc,
		int index, dma_addr_t addr)
{
	xor_cb_t *xcb = desc->hw_desc;
	int k = 0, op = 0, lop = 0;

	/* get the RXOR operand which corresponds to index addr */
	while (op <= index) {
		lop = op;
		if (k == XOR_MAX_OPS) {
			k = 0;
			desc = list_entry (desc->chain_node.next,
				ppc460ex_desc_t, chain_node);
			xcb = desc->hw_desc;

		}
		if ((xcb->ops[k++].h & (DMA_RXOR12 << DMA_CUED_REGION_OFF)) ==
		    (DMA_RXOR12 << DMA_CUED_REGION_OFF))
			op += 2;
		else
			op += 3;
	}

	if (test_bit(/*PPC460EX_DESC_RXOR_REV*/k-1, desc->reverse_flags)) {
		/* reverse operand order; put last op in RXOR group */
		if (index == op - 1)
			ppc460ex_rxor_set_src(desc, k - 1, addr);
	} else {
		/* direct operand order; put first op in RXOR group */
		if (index == lop)
			ppc460ex_rxor_set_src(desc, k - 1, addr);
	}
}

/**
 * ppc460ex_adma_dma2rxor_set_mult - set RXOR multipliers; it's assumed that
 *	ppc460ex_adma_dma2rxor_prep_src() has already done prior this call
 */
static void ppc460ex_adma_dma2rxor_set_mult (ppc460ex_desc_t *desc,
		int index, u8 mult)
{
	xor_cb_t *xcb = desc->hw_desc;
	int k = 0, op = 0, lop = 0;

	/* get the RXOR operand which corresponds to index mult */
	while (op <= index) {
		lop = op;
		if (k == XOR_MAX_OPS) {
			k = 0;
			desc = list_entry (desc->chain_node.next,
				ppc460ex_desc_t, chain_node);
			xcb = desc->hw_desc;

		}
		if ((xcb->ops[k++].h & (DMA_RXOR12 << DMA_CUED_REGION_OFF)) ==
		    (DMA_RXOR12 << DMA_CUED_REGION_OFF))
			op += 2;
		else
			op += 3;
	}

	if (test_bit(/*PPC460EX_DESC_RXOR_REV*/k-1, desc->reverse_flags)) {
		/* reverse order */
		ppc460ex_rxor_set_mult(desc, k - 1, op - index - 1, mult);
	} else {
		/* direct order */
		ppc460ex_rxor_set_mult(desc, k - 1, index - lop, mult);
	}
}

/**
 * ppc460ex_init_rxor_cursor -
 */
static void ppc460ex_init_rxor_cursor (ppc460ex_rxor_cursor_t *cursor)
{
	memset (cursor, 0, sizeof (ppc460ex_rxor_cursor_t));
	cursor->state = 2;
}

/**
 * ppc460ex_adma_pqxor_set_src_mult - set multiplication coefficient into
 * descriptor for the PQXOR operation
 */
static void ppc460ex_adma_pqxor_set_src_mult (
		ppc460ex_desc_t *sw_desc,
		unsigned char mult, int index,int dst_pos)
{
	ppc460ex_ch_t *chan = to_ppc460ex_adma_chan(sw_desc->async_tx.chan);
	u32 mult_idx, mult_dst;
	ppc460ex_desc_t *iter=NULL, *iter1=NULL;

	switch (chan->device->id) {
	case PPC460EX_DMA0_ID:
	case PPC460EX_DMA1_ID:
		if (test_bit(PPC460EX_DESC_RXOR, &sw_desc->flags)) {
			int region = test_bit(PPC460EX_DESC_RXOR12,
					&sw_desc->flags) ? 2 : 3;

			if (index < region) {
				/* RXOR multipliers */

				iter = ppc460ex_get_group_entry(sw_desc,
						sw_desc->dst_cnt - 1);
				if (sw_desc->dst_cnt == 2)
					iter1 = ppc460ex_get_group_entry(sw_desc, 0);
				mult_idx = DMA_CUED_MULT1_OFF + (index << 3);
				mult_dst = DMA_CDB_SG_SRC;
			} else {
				/* WXOR multiplier */
			iter = ppc460ex_get_group_entry(sw_desc,
			    index - region + 1);
				mult_idx = DMA_CUED_MULT1_OFF;
				mult_dst = dst_pos ? DMA_CDB_SG_DST2 :
					DMA_CDB_SG_DST1;
			}
		} else {
			int znum = 0;

			/* WXOR-only;
			 * skip first slots with destinations (if ZERO_DST has
			 * place)
			 */
			if (test_bit(PPC460EX_ZERO_P, &sw_desc->flags))
				znum++;
			if (test_bit(PPC460EX_ZERO_Q, &sw_desc->flags))
				znum++;
			iter = ppc460ex_get_group_entry(sw_desc, index + znum);
			mult_idx = DMA_CUED_MULT1_OFF;
			mult_dst = dst_pos ? DMA_CDB_SG_DST2 : DMA_CDB_SG_DST1;
		}

		if (likely(sw_desc))
			ppc460ex_desc_set_src_mult(iter, chan,
				mult_idx, mult_dst, mult);
		break;
	case PPC460EX_XOR_ID:
		iter = sw_desc->group_head;
		if (iter->dst_cnt == 2) {
			/* both P & Q calculations required; set Q mult here */
			ppc460ex_adma_dma2rxor_set_mult(iter, index, mult);
			/* this is for P. Actually sw_desc already points
			 * to the second CDB though.
			 */
			mult = 1;
			iter = ppc460ex_get_group_entry(sw_desc,
			       sw_desc->descs_per_op);
		}
		ppc460ex_adma_dma2rxor_set_mult(iter, index, mult);
		break;
	}
}
void ppc460ex_adma_xor_set_src_mult (
		ppc460ex_desc_t *sw_desc,
		unsigned char mult, int index,int dst_pos)
{
	ppc460ex_ch_t *chan = to_ppc460ex_adma_chan(sw_desc->async_tx.chan);
	u32 mult_idx, mult_dst;
	ppc460ex_desc_t *iter=NULL, *iter1=NULL;

	switch (chan->device->id) {
	case PPC460EX_DMA0_ID:
	case PPC460EX_DMA1_ID:
		if (test_bit(PPC460EX_DESC_RXOR, &sw_desc->flags)) {
			int region = test_bit(PPC460EX_DESC_RXOR12,
					&sw_desc->flags) ? 2 : 3;

			if (index < region) {
				/* RXOR multipliers */

				iter = ppc460ex_get_group_entry(sw_desc,
						sw_desc->dst_cnt - 1);
				if (sw_desc->dst_cnt == 2)
					iter1 = ppc460ex_get_group_entry(sw_desc, 0);
				mult_idx = DMA_CUED_MULT1_OFF + (index << 3);
				mult_dst = DMA_CDB_SG_SRC;
			} else {
				/* WXOR multiplier */
			iter = ppc460ex_get_group_entry(sw_desc,
			    index - region + 1);
				mult_idx = DMA_CUED_MULT1_OFF;
				mult_dst = dst_pos ? DMA_CDB_SG_DST2 :
					DMA_CDB_SG_DST1;
			}
		} else {
			int znum = 0;

			/* WXOR-only;
			 * skip first slots with destinations (if ZERO_DST has
			 * place)
			 */
			if (test_bit(PPC460EX_ZERO_P, &sw_desc->flags))
				znum++;
			if (test_bit(PPC460EX_ZERO_Q, &sw_desc->flags))
				znum++;
			iter = ppc460ex_get_group_entry(sw_desc, index + znum);
			mult_idx = DMA_CUED_MULT1_OFF;
			mult_dst = dst_pos ? DMA_CDB_SG_DST2 : DMA_CDB_SG_DST1;
		}

		if (likely(sw_desc))
			ppc460ex_desc_set_src_mult(iter, chan,
				mult_idx, mult_dst, mult);
		break;
	}
}
/**
 * ppc460ex_adma_pqxor_set_src_mult - set multiplication coefficient into
 * descriptor for the PQXOR operation
 */
static void ppc460ex_adma_dma01_xor_set_src_mult (
		ppc460ex_desc_t *sw_desc,
		unsigned char mult, int index)
{
	ppc460ex_ch_t *chan = to_ppc460ex_adma_chan(sw_desc->async_tx.chan);
	u32 mult_idx, mult_dst;

	switch (chan->device->id) {
	case PPC460EX_DMA0_ID:
	case PPC460EX_DMA1_ID:
		if (test_bit(PPC460EX_DESC_RXOR, &sw_desc->flags)) {
			int region = test_bit(PPC460EX_DESC_RXOR12,
					&sw_desc->flags) ? 2 : 3;

			if (index < region) {
				/* RXOR multipliers */
				sw_desc = ppc460ex_get_group_entry(sw_desc, 0);
				mult_idx = DMA_CUED_MULT1_OFF + (index << 3);
				mult_dst = DMA_CDB_SG_SRC;
			} else {
				/* WXOR multiplier */
			sw_desc = ppc460ex_get_group_entry(sw_desc,
			    index - region + 1);
				mult_idx = DMA_CUED_MULT1_OFF;
				mult_dst = DMA_CDB_SG_DST1;
			}
		} else {
			/* WXOR-only;
			 * skip first slots with destinations (if ZERO_DST has
			 * place)
			 */
			if (!test_bit(PPC460EX_ZERO_P, &sw_desc->flags)) {
				sw_desc = ppc460ex_get_group_entry(sw_desc,
					index);
			} else {
				sw_desc = ppc460ex_get_group_entry(sw_desc,
					sw_desc->dst_cnt + index);
			}
			mult_idx = DMA_CUED_MULT1_OFF;
			mult_dst = DMA_CDB_SG_DST1;
		}

		if (likely(sw_desc)) {
			ppc460ex_desc_set_src_mult(sw_desc, chan,
				mult_idx, mult_dst, mult);
		}
		break;
	}
}

/**
 * ppc460ex_adma_pqzero_sum_set_src_mult - set multiplication coefficient
 * into descriptor for the PQ_VAL operation
 */
static void ppc460ex_adma_pqzero_sum_set_src_mult (
		ppc460ex_desc_t *sw_desc,
		unsigned char mult, int index, int dst_pos)
{
	ppc460ex_ch_t *chan = to_ppc460ex_adma_chan(sw_desc->async_tx.chan);
	u32 mult_idx, mult_dst;

	/* set mult for sources only */
	BUG_ON(index >= sw_desc->src_cnt);

	/* get pointed slot */
	sw_desc = ppc460ex_get_group_entry(sw_desc, index);

	mult_idx = DMA_CUED_MULT1_OFF;
	mult_dst = dst_pos ? DMA_CDB_SG_DST2 : DMA_CDB_SG_DST1;

	if (likely(sw_desc))
		ppc460ex_desc_set_src_mult(sw_desc, chan, mult_idx, mult_dst,
		    mult);
}
#if 0
/**
 * ppc460ex_adma_dependency_added - schedule clean-up
 */
static void ppc460ex_adma_dependency_added(struct dma_chan *chan)
{
	ppc460ex_ch_t *ppc460ex_chan = to_ppc460ex_adma_chan(chan);
	tasklet_schedule(&ppc460ex_chan->irq_tasklet);
}
#endif

/**
 * ppc460ex_adma_free_chan_resources - free the resources allocated
 */
static void ppc460ex_adma_free_chan_resources(struct dma_chan *chan)
{
	ppc460ex_ch_t *ppc460ex_chan = to_ppc460ex_adma_chan(chan);
	ppc460ex_desc_t *iter, *_iter;
	int in_use_descs = 0;

	ppc460ex_adma_slot_cleanup(ppc460ex_chan);

	spin_lock_bh(&ppc460ex_chan->lock);
	list_for_each_entry_safe(iter, _iter, &ppc460ex_chan->chain,
					chain_node) {
		in_use_descs++;
		list_del(&iter->chain_node);
	}
	list_for_each_entry_safe_reverse(iter, _iter,
			&ppc460ex_chan->all_slots, slot_node) {
		list_del(&iter->slot_node);
		kfree(iter);
		ppc460ex_chan->slots_allocated--;
	}
	ppc460ex_chan->last_used = NULL;

	dev_dbg(ppc460ex_chan->device->common.dev,
		"ppc460ex adma%d %s slots_allocated %d\n",
		ppc460ex_chan->device->id,
		__FUNCTION__, ppc460ex_chan->slots_allocated);
	spin_unlock_bh(&ppc460ex_chan->lock);

	/* one is ok since we left it on there on purpose */
	if (in_use_descs > 1)
		printk(KERN_ERR "GT: Freeing %d in use descriptors!\n",
			in_use_descs - 1);
}

/**
 * ppc460ex_adma_is_complete - poll the status of an ADMA transaction
 * @chan: ADMA channel handle
 * @cookie: ADMA transaction identifier
 */
static enum dma_status ppc460ex_adma_is_complete(struct dma_chan *chan,
	dma_cookie_t cookie, dma_cookie_t *done, dma_cookie_t *used)
{
	ppc460ex_ch_t *ppc460ex_chan = to_ppc460ex_adma_chan(chan);
	dma_cookie_t last_used;
	dma_cookie_t last_complete;
	enum dma_status ret;

	//printk( "--------------- %s: %i-------------------------\n",__FUNCTION__,__LINE__);
	last_used = chan->cookie;
	last_complete = ppc460ex_chan->completed_cookie;

	if (done)
		*done= last_complete;
	if (used)
		*used = last_used;

	ret = dma_async_is_complete(cookie, last_complete, last_used);
	if (ret == DMA_SUCCESS)
		return ret;

	ppc460ex_adma_slot_cleanup(ppc460ex_chan);

	last_used = chan->cookie;
	last_complete = ppc460ex_chan->completed_cookie;

	if (done)
		*done= last_complete;
	if (used)
		*used = last_used;

	return dma_async_is_complete(cookie, last_complete, last_used);
}

/**
 * ppc460ex_adma_eot_handler - end of transfer interrupt handler
 */
static irqreturn_t ppc460ex_adma_eot_handler(int irq, void *data)
{
	ppc460ex_ch_t *chan = data;

	dev_dbg(chan->device->common.dev,
		"ppc460ex adma%d: %s\n", chan->device->id, __FUNCTION__);

	tasklet_schedule(&chan->irq_tasklet);
	ppc460ex_adma_device_clear_eot_status(chan);

	return IRQ_HANDLED;
}

/**
 * ppc460ex_adma_err_handler - DMA error interrupt handler;
 *	do the same things as a eot handler
 */
static irqreturn_t ppc460ex_adma_err_handler(int irq, void *data)
{
	ppc460ex_ch_t *chan = data;
	dev_dbg(chan->device->common.dev,
		"ppc460ex adma%d: %s\n", chan->device->id, __FUNCTION__);
	tasklet_schedule(&chan->irq_tasklet);
	ppc460ex_adma_device_clear_eot_status(chan);

	return IRQ_HANDLED;
}

static void ppc460ex_test_rad6_callback (void *unused)
{
	complete(&ppc460ex_r6_test_comp);
}
/**
 * ppc460ex_test_callback - called when test operation has been done
 */
static void ppc460ex_test_callback (void *unused)
{
	complete(&ppc460ex_r5_test_comp);
}

/**
 * ppc460ex_adma_issue_pending - flush all pending descriptors to h/w
 */
static void ppc460ex_adma_issue_pending(struct dma_chan *chan)
{
	ppc460ex_ch_t *ppc460ex_chan = to_ppc460ex_adma_chan(chan);
#if 0
	dev_dbg(ppc460ex_chan->device->common.dev,
	    "ppc460ex adma%d: %s %d \n", ppc460ex_chan->device->id,
	    __FUNCTION__, ppc460ex_chan->pending);
#endif

	if (ppc460ex_chan->pending) {
	dev_dbg(ppc460ex_chan->device->common.dev,
	    "ppc460ex adma%d: %s %d \n", ppc460ex_chan->device->id,
	    __FUNCTION__, ppc460ex_chan->pending);
		ppc460ex_chan->pending = 0;
		ppc460ex_chan_append(ppc460ex_chan);
	}
}

/**
 * ppc460ex_adma_remove - remove the asynch device
 */
static int __devexit ppc460ex_adma_remove(struct platform_device *dev)
{
	ppc460ex_dev_t *device = platform_get_drvdata(dev);
	struct dma_chan *chan, *_chan;
	struct ppc_dma_chan_ref *ref, *_ref;
	ppc460ex_ch_t *ppc460ex_chan;
	int i;
	ppc460ex_aplat_t *plat_data = dev->dev.platform_data;

	dma_async_device_unregister(&device->common);

	for (i = 0; i < 3; i++) {
		u32 irq;
		irq = platform_get_irq(dev, i);
		free_irq(irq, device);
	}
	
	if ( (ppc460ex_chan) && (ppc460ex_chan->device->desc_memory == ADMA_DESC_MEM_OCM))
		ocm_free(device->dma_desc_pool_virt);
	else
		dma_free_coherent(&dev->dev, plat_data->pool_size,
			device->dma_desc_pool_virt, device->dma_desc_pool);


	do {
		struct resource *res;
		res = platform_get_resource(dev, IORESOURCE_MEM, 0);
		release_mem_region(res->start, res->end - res->start);
	} while (0);

	list_for_each_entry_safe(chan, _chan, &device->common.channels,
				device_node) {
		ppc460ex_chan = to_ppc460ex_adma_chan(chan);
		list_del(&chan->device_node);
		kfree(ppc460ex_chan);
	}

	list_for_each_entry_safe(ref, _ref, &ppc_adma_chan_list, node) {
		list_del(&ref->node);
		kfree(ref);
	}

	kfree(device);

	return 0;
}

/**
 * ppc460ex_adma_probe - probe the asynch device
 */
static int __devinit ppc460ex_adma_probe(struct of_device *ofdev,
		const struct of_device_id *match)
{
	struct resource *res;
	int ret=0, irq;
	ppc460ex_dev_t *adev;
	ppc460ex_ch_t *chan;
	ppc460ex_aplat_t *plat_data;
	struct ppc_dma_chan_ref *ref;
	const char *str_prop;

	printk("Improved ADMA - 08312009\n");
	plat_data = &ppc460ex_dma_1_data;
	if ((adev = kzalloc(sizeof(*adev), GFP_KERNEL)) == NULL) {
		ret = -ENOMEM;
		goto err_adev_alloc;
	}
	adev->res[0].start = ppc460ex_dma_1_channel.resource[0].start;
	adev->id = ppc460ex_dma_1_channel.id;
	printk("adev->res[0].start=0x%x\n",adev->res[0].start);

	printk( " adev->id = 0x%x ppc460ex_dma_1_channel.resource[0].start=0x%x \n",
			adev->id,ppc460ex_dma_1_channel.resource[0].start);

	/* allocate coherent memory for hardware descriptors
	 * note: writecombine gives slightly better performance, but
	 * requires that we explicitly drain the write buffer
	 */
	str_prop = of_get_property(ofdev->node, "descriptor-memory", NULL);
	if (str_prop && (!strcmp(str_prop,"ocm") || !strcmp(str_prop,"OCM"))) { 
		printk(KERN_INFO
				" descriptor-memory = %s\n", str_prop);
		adev->dma_desc_pool_virt =  ocm_alloc(&adev->dma_desc_pool, DMA1_FIFO_SIZE << 2, 4,
						OCM_NON_CACHED, "ADMA_descriptors");
		adev->desc_memory = ADMA_DESC_MEM_OCM;
	} else {
		
		if ((adev->dma_desc_pool_virt = dma_alloc_coherent(&ofdev->dev,
		     DMA1_FIFO_SIZE << 2, &adev->dma_desc_pool, GFP_KERNEL)) == NULL) {
			ret = -ENOMEM;
			goto err_dma_alloc;
		}
		adev->desc_memory = 0;
	}
	if (adev->dma_desc_pool_virt == NULL) {
		ret = -ENOMEM;
		goto err_dma_alloc;
	}
	dev_dbg(&ofdev->dev, "%s: allocted descriptor pool virt %p phys %p\n",
		__FUNCTION__, adev->dma_desc_pool_virt,
		(void *) adev->dma_desc_pool);

	adev->id = PPC460EX_DMA1_ID;
	/* create the DMA capability MASK . This used to come from resources structure*/
	dma_cap_set(DMA_MEMCPY, adev->common.cap_mask);
	dma_cap_set(DMA_INTERRUPT, adev->common.cap_mask);
	dma_cap_set(DMA_MEMSET, adev->common.cap_mask);
	dma_cap_set(DMA_PQ, adev->common.cap_mask);
	//dma_cap_set(DMA_PQ_VAL, adev->common.cap_mask);
	dma_cap_set(DMA_XOR, adev->common.cap_mask);/* Marri RAID-5 */
	dma_cap_set(DMA_XOR_VAL, adev->common.cap_mask);
	adev->odev = ofdev;
	dev_set_drvdata(&(ofdev->dev), adev);

	INIT_LIST_HEAD(&adev->common.channels);

	/* set base routines */
	adev->common.device_alloc_chan_resources =
	    ppc460ex_adma_alloc_chan_resources;
	adev->common.device_free_chan_resources =
	    ppc460ex_adma_free_chan_resources;
	adev->common.device_is_tx_complete = ppc460ex_adma_is_complete;
	adev->common.device_issue_pending = ppc460ex_adma_issue_pending;
	adev->common.dev = &ofdev->dev;

	/* set prep routines based on capability */
#if 1
	if (dma_has_cap(DMA_MEMCPY, adev->common.cap_mask)) {
		adev->common.device_prep_dma_memcpy =
		    ppc460ex_adma_prep_dma_memcpy;
	}
#endif
	if (dma_has_cap(DMA_MEMSET, adev->common.cap_mask)) {
		adev->common.device_prep_dma_memset =
		    ppc460ex_adma_prep_dma_memset;
	}
#if 1
	if (dma_has_cap(DMA_XOR, adev->common.cap_mask)) {
		adev->common.max_xor = XOR_MAX_OPS;
		adev->common.device_prep_dma_xor = ppc460ex_adma_prep_dma_mq_xor;
	}
#endif
#if 1
	if (dma_has_cap(DMA_XOR_VAL, adev->common.cap_mask)) {
		adev->common.max_xor = XOR_MAX_OPS;
		adev->common.device_prep_dma_xor_val = ppc460ex_adma_prep_dma_mq_zero_sum;
	}
#endif
	if (dma_has_cap(DMA_PQ, adev->common.cap_mask)) {
		switch (adev->id) {
		case PPC460EX_DMA1_ID:
			adev->common.max_pq = DMA1_FIFO_SIZE /
						sizeof(dma_cdb_t);
			break;
		}
		adev->common.device_prep_dma_pq =
		    ppc460ex_adma_prep_dma_pq;

	}
	if (dma_has_cap(DMA_PQ_VAL, adev->common.cap_mask)) {
		switch (adev->id) {
		case PPC460EX_DMA1_ID:
			adev->common.max_pq = DMA1_FIFO_SIZE /
						sizeof(dma_cdb_t);
			break;
		}
		adev->common.device_prep_dma_pq_val =
		    ppc460ex_adma_prep_dma_pqzero_sum;
	}

	if (dma_has_cap(DMA_INTERRUPT, adev->common.cap_mask)) {
		adev->common.device_prep_dma_interrupt =
		    ppc460ex_adma_prep_dma_interrupt;
	}

	/* create a channel */
	if ((chan = kzalloc(sizeof(*chan), GFP_KERNEL)) == NULL) {
		ret = -ENOMEM;
		goto err_chan_alloc;
	}

	tasklet_init(&chan->irq_tasklet, ppc460ex_adma_tasklet,
	    (unsigned long)chan);
	irq = irq_of_parse_and_map(ofdev->node, 0);
	printk("<%s> irq=0x%x\n",__FUNCTION__, irq);
	if (irq >= 0) {
		ret = request_irq(irq, ppc460ex_adma_eot_handler,
			IRQF_DISABLED, "adma-compl", chan);
		if (ret) {
			printk("Failed to request IRQ %d\n",irq);
			ret = -EIO;
			goto err_irq;
		}

		irq = irq_of_parse_and_map(ofdev->node, 2);
		printk("<%s> irq=0x%x\n",__FUNCTION__, irq);
		if (irq >= 0) {
			ret = request_irq(irq, ppc460ex_adma_err_handler,
				IRQF_DISABLED, "adma-err", chan);
			if (ret) {
				printk("Failed to request IRQ %d\n",irq);
				ret = -EIO;
				goto err_irq;
			}
		}
	} else
		ret = -ENXIO;

	chan->device = adev;

	/* pass the platform data */
	chan->device->odev->dev.platform_data = &ppc460ex_dma_1_data;
	spin_lock_init(&chan->lock);
#if 0
	init_timer(&chan->cleanup_watchdog);
	chan->cleanup_watchdog.data = (unsigned long) chan;
	chan->cleanup_watchdog.function = ppc460ex_adma_tasklet;
#endif
	INIT_LIST_HEAD(&chan->chain);
	INIT_LIST_HEAD(&chan->all_slots);
	chan->common.device = &adev->common;
	list_add_tail(&chan->common.device_node, &adev->common.channels);

	dev_dbg(&ofdev->dev,  "AMCC(R) PPC460 ADMA Engine found [%d]: "
	  "( %s%s%s%s%s%s%s%s)\n",
	  adev->id,
	  dma_has_cap(DMA_PQ, adev->common.cap_mask) ? "pq_xor " : "",
	  dma_has_cap(DMA_PQ_VAL, adev->common.cap_mask) ? "pq_zero_sum " :
	    "",
	  dma_has_cap(DMA_XOR, adev->common.cap_mask) ? "xor " : "",
	  dma_has_cap(DMA_XOR_VAL, adev->common.cap_mask) ? "xor_zero_sum " :
	    "",
	  dma_has_cap(DMA_MEMSET, adev->common.cap_mask)  ? "memset " : "",
	  dma_has_cap(DMA_MEMCPY, adev->common.cap_mask) ? "memcpy " : "",
	  dma_has_cap(DMA_INTERRUPT, adev->common.cap_mask) ? "int " : "");

	dma_async_device_register(&adev->common);
	ref = kmalloc(sizeof(*ref), GFP_KERNEL);
		printk("<%s> ret=0x%x\n", __FUNCTION__,ret);
	if (ref) {
		ref->chan = &chan->common;
		INIT_LIST_HEAD(&ref->node);
		list_add_tail(&ref->node, &ppc_adma_chan_list);
	} else
		printk(KERN_WARNING "%s: failed to allocate channel reference!\n",
		       __FUNCTION__);
	goto out;

err_irq:
	free_irq(irq,&adev->id);
err_chan_alloc:
	kfree(chan);
err_dma_alloc:
	if ( chan->device->desc_memory == ADMA_DESC_MEM_OCM)
		ocm_free(adev->dma_desc_pool_virt );
	else
		dma_free_coherent(&ofdev->dev,DMA1_FIFO_SIZE << 2, 
				adev->dma_desc_pool_virt, adev->dma_desc_pool);
err_adev_alloc:
	kfree(adev);
	release_mem_region(res->start, res->end - res->start);
out:
	return ret;
}

/**
 * ppc460ex_chan_start_null_xor - initiate the first XOR operation (DMA engines
 *	use FIFOs (as opposite to chains used in XOR) so this is a XOR
 *	gtcific operation)
 */
static void ppc460ex_chan_start_null_xor(ppc460ex_ch_t *chan)
{
	ppc460ex_desc_t *sw_desc, *group_start;
	dma_cookie_t cookie;
	int slot_cnt, slots_per_op;

	dev_dbg(chan->device->common.dev,
		"ppc460ex adma%d: %s\n", chan->device->id, __FUNCTION__);

	spin_lock_bh(&chan->lock);
	slot_cnt = ppc460ex_chan_xor_slot_count(0, 2, &slots_per_op);
	sw_desc = ppc460ex_adma_alloc_slots(chan, slot_cnt, slots_per_op);
	if (sw_desc) {
		group_start = sw_desc->group_head;
		list_splice_init(&sw_desc->group_list, &chan->chain);
		async_tx_ack(&sw_desc->async_tx); 
		ppc460ex_desc_init_null_xor(group_start);

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
		BUG_ON(ppc460ex_chan_is_busy(chan));

		/* set the descriptor address */
		ppc460ex_chan_set_first_xor_descriptor(chan, sw_desc);

		/* run the descriptor */
		ppc460ex_chan_run(chan);
	} else
		printk(KERN_ERR "ppc460ex adma%d"
			" failed to allocate null descriptor\n",
			chan->device->id);
	spin_unlock_bh(&chan->lock);
}

/**
 * ppc460ex_test_raid6 - test are RAID-6 capabilities enabled successfully.
 *	For this we just perform one WXOR operation with the same source
 *	and destination addresses, the GF-multiplier is 1; so if RAID-6
	o/of_platform_driver_unregister(&ppc460ex_adma_driver);
 *	capabilities are enabled then we'll get src/dst filled with zero.
 */
static int ppc460ex_test_raid6 (ppc460ex_ch_t *chan)
{
	ppc460ex_desc_t *sw_desc, *iter;
	struct page *pg;
	char *a;
	dma_addr_t dma_addr, addrs[2];;
	unsigned long op = 0;
	int rval = 0;

	if (!ppc460ex_r6_tchan)
		return -1;
	/*FIXME*/

	set_bit(PPC460EX_DESC_WXOR, &op);

	pg = alloc_page(GFP_KERNEL);
	if (!pg)
		return -ENOMEM;

	spin_lock_bh(&chan->lock);
	sw_desc = ppc460ex_adma_alloc_slots(chan, 1, 1);
	if (sw_desc) {
		/* 1 src, 1 dsr, int_ena, WXOR */
		ppc460ex_desc_init_pq(sw_desc, 1, 1, 1, op);
		list_for_each_entry(iter, &sw_desc->group_list, chain_node) {
			ppc460ex_desc_set_byte_count(iter, chan, PAGE_SIZE);
			iter->unmap_len = PAGE_SIZE;
		}
	} else {
		rval = -EFAULT;
		spin_unlock_bh(&chan->lock);
		goto exit;
	}
	spin_unlock_bh(&chan->lock);

	/* Fill the test page with ones */
	memset(page_address(pg), 0xFF, PAGE_SIZE);
#if 0
	int i = 0;
	char *pg_addr = page_address(pg);
	//for(i=0;i < PAGE_SIZE; i+=64)
	for(i=0;i < 1000; i+=64)
		printk("addr = 0x%x data = 0x%x\n",pg_addr + i,*(pg_addr+i));
#endif
	dma_addr = dma_map_page(&chan->device->odev->dev, pg, 0, PAGE_SIZE,
	    DMA_BIDIRECTIONAL);

	/* Setup adresses */
	ppc460ex_adma_pqxor_set_src(sw_desc, dma_addr, 0);
	ppc460ex_adma_pqxor_set_src_mult(sw_desc, 1, 0,0);
	addrs[0] = dma_addr;
	addrs[1] = 0;
	ppc460ex_adma_pqxor_set_dest(sw_desc, addrs, DMA_PREP_HAVE_P);

	async_tx_ack(&sw_desc->async_tx);
	sw_desc->async_tx.callback = ppc460ex_test_rad6_callback;
	sw_desc->async_tx.callback_param = NULL;

	init_completion(&ppc460ex_r6_test_comp);

	ppc460ex_adma_tx_submit(&sw_desc->async_tx);
	ppc460ex_adma_issue_pending(&chan->common);

	wait_for_completion(&ppc460ex_r6_test_comp);

	/* Now check is the test page zeroed */
	a = page_address(pg);
#if 0
	i = 0;
	for(i=0;i < PAGE_SIZE; i+=64)
		printk("addr = 0x%x data = 0x%x\n",a + i,*(a+i));
#endif
	if ((*(u32*)a) == 0 && memcmp(a, a+4, PAGE_SIZE-4)==0) {
		/* page is zero - RAID-6 enabled */
		rval = 0;
	} else {
		/* RAID-6 was not enabled */
		rval = -EINVAL;
	}
	//printk(__LINE__,__FUNCTION__);
exit:
	__free_page(pg);
	return rval;
}
/**
 * ppc460ex_test_raid5 - test are RAID-5 capabilities enabled successfully.
 *	For this we just perform one WXOR operation with the same source
 *	and destination addresses, the GF-multiplier is 1; so if RAID-5
	o/of_platform_driver_unregister(&ppc460ex_adma_driver);
 *	capabilities are enabled then we'll get src/dst filled with zero.
 */
static int ppc460ex_test_raid5 (ppc460ex_ch_t *chan)
{
	ppc460ex_desc_t *sw_desc, *iter;
	struct page *pg;
	char *a;
	dma_addr_t dma_addr;
	unsigned long op = 0;
	int rval = 0;

	if (!ppc460ex_r5_tchan)
		return -1;
	/*FIXME*/

	set_bit(PPC460EX_DESC_WXOR, &op);

	pg = alloc_page(GFP_KERNEL);
	if (!pg)
		return -ENOMEM;

	spin_lock_bh(&chan->lock);
	sw_desc = ppc460ex_adma_alloc_slots(chan, 1, 1);
	if (sw_desc) {
		/* 1 src, 1 dsr, int_ena, WXOR */
		//ppc460ex_desc_init_pqxor(sw_desc, 1, 1, 1, op);
		ppc460ex_desc_init_dma01_xor(sw_desc, 1, 1, 1, op);
		list_for_each_entry(iter, &sw_desc->group_list, chain_node) {
			ppc460ex_desc_set_byte_count(iter, chan, PAGE_SIZE);
			iter->unmap_len = PAGE_SIZE;
		}
	} else {
		rval = -EFAULT;
		spin_unlock_bh(&chan->lock);
		goto exit;
	}
	spin_unlock_bh(&chan->lock);

	/* Fill the test page with ones */
	memset(page_address(pg), 0xFF, PAGE_SIZE);
#if 0
	int i = 0;
	char *pg_addr = page_address(pg);
	for(i=0;i < PAGE_SIZE; i+=64)
		printk("addr = 0x%x data = 0x%x\n",pg_addr + i,*(pg_addr+i));
#endif
	//dma_addr = dma_map_page(&chan->device->common, pg, 0, PAGE_SIZE,
	dma_addr = dma_map_page(&chan->device->odev->dev, pg, 0, PAGE_SIZE,
	    DMA_BIDIRECTIONAL);

	/* Setup adresses */
	ppc460ex_adma_dma01_xor_set_src(sw_desc, dma_addr, 0);
	ppc460ex_adma_dma01_xor_set_src_mult(sw_desc, 1, 0);
	ppc460ex_adma_dma01_xor_set_dest(sw_desc, dma_addr, 0);

	async_tx_ack(&sw_desc->async_tx);
	sw_desc->async_tx.callback = ppc460ex_test_callback;
	sw_desc->async_tx.callback_param = NULL;

	init_completion(&ppc460ex_r5_test_comp);

	ppc460ex_adma_tx_submit(&sw_desc->async_tx);
	ppc460ex_adma_issue_pending(&chan->common);

	wait_for_completion(&ppc460ex_r5_test_comp);

	/*Make sure cache is flushed to memory*/
	dma_addr = dma_map_page(&chan->device->odev->dev, pg, 0, PAGE_SIZE,
	    DMA_BIDIRECTIONAL);
	/* Now check is the test page zeroed */
	a = page_address(pg);
#if 0
	i = 0;
	for(i=0;i < PAGE_SIZE; i+=64)
		printk("addr = 0x%x data = 0x%x\n",a + i,*(a+i));
#endif
	if ((*(u32*)a) == 0 && memcmp(a, a+4, PAGE_SIZE-4)==0) {
		/* page is zero - RAID-5 enabled */
		rval = 0;
	} else {
		/* RAID-5 was not enabled */
		rval = -EINVAL;
	}
	pr_dma(__LINE__,__FUNCTION__);
exit:
	__free_page(pg);
	return rval;
}
#if 1
static struct of_device_id adma_match[] = 
{
	{
		.compatible     = "amcc,adma",
	},
	{},
};
static struct of_platform_driver ppc460ex_adma_driver = {
	.name		= "adma",
	.match_table = adma_match,

	.probe		= ppc460ex_adma_probe,
	.remove		= ppc460ex_adma_remove,
};
#else
static struct platform_driver ppc460ex_adma_driver= {
	.probe		= ppc460ex_adma_probe,
	.remove		= ppc460ex_adma_remove,
	.driver		= {
		.owner	= "marri",
		.name	= "PPC460EX-ADMA",
	},
};
#endif

/**
 * /proc interface
 */
static int ppc460ex_poly_read (char *page, char **start, off_t off,
	int count, int *eof, void *data)
{
	char *p = page;
	u32 reg;

#ifdef CONFIG_440SP
	/* 440SP has fixed polynomial */
	reg = 0x4d;
#else
	reg = mfdcr(DCRN_MQ0_CFBHL);
	reg >>= MQ0_CFBHL_POLY;
	reg &= 0xFF;
#endif

	p += sprintf (p, "PPC460EX RAID-5 driver uses 0x1%02x polynomial.\n",
		reg);

	return p - page;
}

static int ppc460ex_poly_write (struct file *file, const char __user *buffer,
	unsigned long count, void *data)
{
	/* e.g., 0x14D or 0x11d */
	char tmp[6];
	unsigned long val, rval;

#ifdef CONFIG_440SP
	/* 440SP use default 0x14D polynomial only */
	return -EINVAL;
#endif

	if (!count || count > 6)
		return -EINVAL;

	if (copy_from_user(tmp, buffer, count))
		return -EFAULT;

	tmp[count] = 0;
	val = simple_strtoul(tmp, NULL, 16);

	if (val & ~0x1FF)
		return -EINVAL;

	val &= 0xFF;
	rval = mfdcr(DCRN_MQ0_CFBHL);
	rval &= ~(0xFF << MQ0_CFBHL_POLY);
	rval |= val << MQ0_CFBHL_POLY;
	mtdcr(DCRN_MQ0_CFBHL, rval);

	return count;
}

static int ppc460ex_r6ena_read (char *page, char **start, off_t off,
	int count, int *eof, void *data)
{
	char *p = page;

	p += sprintf(p, "%s\n",
		ppc460ex_r6_enabled ?
		"PPC460Ex RAID-6 capabilities are ENABLED.\n" :
		"PPC460Ex RAID-6 capabilities are DISABLED.\n");

	return p - page;
}

static int ppc460ex_r6ena_write (struct file *file, const char __user *buffer,
	unsigned long count, void *data)
{
	/* e.g. 0xffffffff */
	char tmp[11];
	unsigned long val;

	if (!count || count > 11)
		return -EINVAL;

	if (!ppc460ex_r6_tchan)
		return -EFAULT;

	if (copy_from_user(tmp, buffer, count))
		return -EFAULT;

	/* Write a key */
	val = simple_strtoul(tmp, NULL, 16);
	mtdcr(DCRN_MQ0_XORBA, val);
	isync();

	/* Verify does it really work now */
	if (ppc460ex_test_raid6(ppc460ex_r6_tchan) == 0) {
		/* PPC440SP(e) RAID-6 has been activated successfully */;
		printk(KERN_INFO "PPC460Ex RAID-6 has been activated "
		    "successfully\n");
		ppc460ex_r6_enabled = 0;
		ppc460ex_r5_enabled = 0;
	} else {
		/* PPC440SP(e) RAID-6 hasn't been activated! Error key ? */;
		printk(KERN_INFO "PPC460Ex RAID-6 hasn't been activated!"
		    " Error key ?\n");
		ppc460ex_r6_enabled = 0;
	}

	return count;
}
static int ppc460ex_r5ena_read (char *page, char **start, off_t off,
	int count, int *eof, void *data)
{
	char *p = page;

	p += sprintf(p, "%s\n",
		ppc460ex_r5_enabled ?
		"PPC460Ex RAID-r5 capabilities are ENABLED.\n" :
		"PPC460Ex RAID-r5 capabilities are DISABLED.\n");

	return p - page;
}

static int ppc460ex_r5ena_write (struct file *file, const char __user *buffer,
	unsigned long count, void *data)
{
	/* e.g. 0xffffffff */
	char tmp[11];
	unsigned long val;

	if (!count /*|| count > 11*/)
		return -EINVAL;

	if (!ppc460ex_r5_tchan)
		return -EFAULT;

	if (copy_from_user(tmp, buffer, count))
		return -EFAULT;

	/* Write a key */
	val = simple_strtoul(tmp, NULL, 16);
	mtdcr(DCRN_MQ0_XORBA, val);
	isync();

	/* Verify does it really work now */
	if (ppc460ex_test_raid5(ppc460ex_r5_tchan) == 0) {
		/* PPC440SP(e) RAID-6 has been activated successfully */;
		printk(KERN_INFO "PPC460Ex RAID-5 has been activated "
		    "successfully\n");
		ppc460ex_r5_enabled = 1;
		ppc460ex_r6_enabled = 0;
	} else {
		/* PPC440SP(e) RAID-6 hasn't been activated! Error key ? */;
		printk(KERN_INFO "PPC460Ex RAID-5 hasn't been activated!"
		    " Error key ?\n");
		ppc460ex_r5_enabled = 0;
	}

	return count;
}

static int __init ppc460ex_adma_init (void)
{
	int rval;
	struct proc_dir_entry *p;

	ppc460ex_configure_raid_devices();
	rval = of_register_platform_driver(&ppc460ex_adma_driver);
	//rval = platform_driver_register(&ppc460ex_adma_driver);

	if (rval == 0) {
		/* Create /proc entries */
		ppc460ex_proot = proc_mkdir(PPC460EX_R5_PROC_ROOT, NULL);
		if (!ppc460ex_proot) {
			printk(KERN_ERR "%s: failed to create %s proc "
			    "directory\n",__FUNCTION__,PPC460EX_R5_PROC_ROOT);
			/* User will not be able to enable h/w RAID-6 */
			return rval;
		}

		/* GF polynome to use */
		p = create_proc_entry("poly", 0, ppc460ex_proot);
		if (p) {
			p->read_proc = ppc460ex_poly_read;
			p->write_proc = ppc460ex_poly_write;
		}

		/* RAID-6 h/w enable entry */
		p = create_proc_entry("enable", 0, ppc460ex_proot);
		if (p) {
			p->read_proc = ppc460ex_r5ena_read;
			p->write_proc = ppc460ex_r5ena_write;
		}
		/* Create /proc entries */
		ppc460ex_pqroot = proc_mkdir(PPC460EX_R6_PROC_ROOT, NULL);
		if (!ppc460ex_pqroot) {
			printk(KERN_ERR "%s: failed to create %s proc "
			    "directory\n",__FUNCTION__,PPC460EX_R6_PROC_ROOT);
			/* User will not be able to enable h/w RAID-6 */
			return rval;
		}

		/* GF polynome to use */
		p = create_proc_entry("poly", 0, ppc460ex_pqroot);
		if (p) {
			p->read_proc = ppc460ex_poly_read;
			p->write_proc = ppc460ex_poly_write;
		}

		/* RAID-6 h/w enable entry */
		p = create_proc_entry("enable", 0, ppc460ex_pqroot);
		if (p) {
			p->read_proc = ppc460ex_r6ena_read;
			p->write_proc = ppc460ex_r6ena_write;
		}
	}
	return rval;
}

#if 0
static void __exit ppc460ex_adma_exit (void)
{
	of_unregister_platform_driver(&ppc460ex_adma_driver);
	return;
}
module_exit(ppc460ex_adma_exit);
#endif

module_init(ppc460ex_adma_init);

MODULE_AUTHOR("Tirumala Marri<tmarri@amcc.com>");
MODULE_DESCRIPTION("PPC460EX ADMA Engine Driver");
MODULE_LICENSE("GPL");
