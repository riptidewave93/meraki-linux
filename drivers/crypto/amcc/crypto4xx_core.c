/**
 * AMCC SoC PPC4xx Crypto Driver
 *
 * Copyright (c) 2008 Applied Micro Circuits Corporation.
 * All rights reserved. James Hsiao <jhsiao@amcc.com>
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
 * This file implements AMCC crypto offload Linux device driver for use with
 * Linux CryptoAPI.
 */

#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/spinlock_types.h>
#include <linux/random.h>
#include <linux/scatterlist.h>
#include <linux/crypto.h>
#include <linux/dma-mapping.h>
#include <linux/platform_device.h>
#include <linux/init.h>
#include <linux/of_platform.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/highmem.h>
#include <asm/dcr.h>
#include <asm/dcr-regs.h>
#include <asm/cacheflush.h>
#include <crypto/internal/hash.h>
#include <crypto/algapi.h>
#include <asm/ppc4xx_ocm.h>
#include <crypto/internal/hash.h>
#include <crypto/algapi.h>
#include <crypto/aes.h>
#include <crypto/des.h>
#include <crypto/sha.h>
#include <crypto/ctr.h>
#include "crypto4xx_reg_def.h"
#include "crypto4xx_core.h"
#include "crypto4xx_sa.h"

#define PPC4XX_SEC_VERSION_STR			"0.5"
void my_dump_Data(const u_char* dptr, u_int size)
{
	int i;
	for (i = 0; i < size; i++) {
		printk("0x%02x, ", dptr[i]);
		if ((i+1) % 8 == 0)
			printk(" ");
		if ((i+1) % 16 == 0)
			printk("\n");
	}
	printk("\n");
}
static struct proc_dir_entry *proc_crypto4xx  = NULL;
struct proc_dir_entry *entry;
/**
 * PPC4xx Crypto Engine Initialization Routine
 */
static void crypto4xx_hw_init(struct crypto4xx_device *dev)
{
	union ce_ring_size ring_size;
	union ce_ring_contol ring_ctrl;
	union ce_part_ring_size part_ring_size;
	union ce_io_threshold io_threshold;
	u32 rand_num;
	union ce_pe_dma_cfg pe_dma_cfg;

	writel(PPC4XX_BYTE_ORDER, dev->ce_base + CRYPTO4XX_BYTE_ORDER_CFG);
	/* setup pe dma, include reset sg, pdr and pe, then release reset */
	pe_dma_cfg.w = 0;
	pe_dma_cfg.bf.bo_sgpd_en = 1;
	pe_dma_cfg.bf.bo_data_en = 0;
	pe_dma_cfg.bf.bo_sa_en = 1;
	pe_dma_cfg.bf.bo_pd_en = 1;
	pe_dma_cfg.bf.dynamic_sa_en = 1;
	pe_dma_cfg.bf.reset_sg = 1;
	pe_dma_cfg.bf.reset_pdr = 1;
	pe_dma_cfg.bf.reset_pe = 1;
	writel(pe_dma_cfg.w, dev->ce_base + CRYPTO4XX_PE_DMA_CFG);
	/* un reset pe,sg and pdr */
	pe_dma_cfg.bf.pe_mode = 0;
	pe_dma_cfg.bf.reset_sg = 0;
	pe_dma_cfg.bf.reset_pdr = 0;
	pe_dma_cfg.bf.reset_pe = 0;
	pe_dma_cfg.bf.bo_td_en = 0;
	writel(pe_dma_cfg.w, dev->ce_base + CRYPTO4XX_PE_DMA_CFG);
	writel(dev->pdr_pa, dev->ce_base + CRYPTO4XX_PDR_BASE);
	writel(dev->pdr_pa, dev->ce_base + CRYPTO4XX_RDR_BASE);
	get_random_bytes(&rand_num, sizeof(rand_num));
	writel(rand_num, dev->ce_base + CRYPTO4XX_PRNG_SEED_L);
	get_random_bytes(&rand_num, sizeof(rand_num));
	writel(rand_num, dev->ce_base + CRYPTO4XX_PRNG_SEED_H);
	writel(PPC4XX_PRNG_CTRL_AUTO_EN, dev->ce_base + CRYPTO4XX_PRNG_CTRL);	

	ring_size.w = 0;
	ring_size.bf.ring_offset = PPC4XX_PD_SIZE;
	ring_size.bf.ring_size   = PPC4XX_NUM_PD;
	writel(ring_size.w, dev->ce_base + CRYPTO4XX_RING_SIZE);
	ring_ctrl.w = 0;

	if (dev->core_dev->revb_ver == 1) {
#ifdef CONFIG_SEC_HW_POLL
		ring_ctrl.bf.ring_retry_divisor = CONFIG_SEC_HW_POLL_RETRY_FREQ;
		ring_ctrl.bf.ring_poll_divisor = CONFIG_SEC_HW_RING_POLL_FREQ;
#endif
	}
	writel(ring_ctrl.w, dev->ce_base + CRYPTO4XX_RING_CTRL);
	writel(PPC4XX_DC_3DES_EN, dev->ce_base + CRYPTO4XX_DEVICE_CTRL);
	writel(dev->gdr_pa, dev->ce_base + CRYPTO4XX_GATH_RING_BASE);
	writel(dev->sdr_pa, dev->ce_base + CRYPTO4XX_SCAT_RING_BASE);
	part_ring_size.w = 0;
	part_ring_size.bf.sdr_size = PPC4XX_SDR_SIZE;
	part_ring_size.bf.gdr_size = PPC4XX_GDR_SIZE;
	writel(part_ring_size.w, dev->ce_base + CRYPTO4XX_PART_RING_SIZE);
	writel(PPC4XX_SD_BUFFER_SIZE, dev->ce_base + CRYPTO4XX_PART_RING_CFG);
	io_threshold.w = 0;
	io_threshold.bf.output_threshold = PPC4XX_OUTPUT_THRESHOLD;
	io_threshold.bf.input_threshold  = PPC4XX_INPUT_THRESHOLD;
	writel(io_threshold.w, dev->ce_base + CRYPTO4XX_IO_THRESHOLD);

#ifdef CONFIG_SEC_PD_OCM
	writel((dev->pdr_ocm_addr >> 32), dev->ce_base + CRYPTO4XX_PDR_BASE_UADDR);
	writel((dev->pdr_ocm_addr >> 32), dev->ce_base + CRYPTO4XX_RDR_BASE_UADDR);
#else
	writel(0, dev->ce_base + CRYPTO4XX_PDR_BASE_UADDR);
	writel(0, dev->ce_base + CRYPTO4XX_RDR_BASE_UADDR);
#endif
	writel(0, dev->ce_base + CRYPTO4XX_PKT_SRC_UADDR);
	writel(0, dev->ce_base + CRYPTO4XX_PKT_DEST_UADDR);

	writel(0, dev->ce_base + CRYPTO4XX_SA_UADDR);

	writel(0, dev->ce_base + CRYPTO4XX_GATH_RING_BASE_UADDR);
	writel(0, dev->ce_base + CRYPTO4XX_SCAT_RING_BASE_UADDR);
	/* un reset pe,sg and pdr */
	pe_dma_cfg.bf.pe_mode = 1;
	pe_dma_cfg.bf.reset_sg = 0;
	pe_dma_cfg.bf.reset_pdr = 0;
	pe_dma_cfg.bf.reset_pe = 0;
	pe_dma_cfg.bf.bo_td_en = 0;
	writel(pe_dma_cfg.w, dev->ce_base + CRYPTO4XX_PE_DMA_CFG);
	/*clear all pending interrupt*/
	writel(PPC4XX_INTERRUPT_CLR, dev->ce_base + CRYPTO4XX_INT_CLR);
	writel(PPC4XX_INT_DESCR_CNT, dev->ce_base + CRYPTO4XX_INT_DESCR_CNT);
	
	if (dev->core_dev->revb_ver == 1) {
		writel(PPC4XX_INT_TIMEOUT_CNT_REVB << 10,
			dev->ce_base + CRYPTO4XX_INT_TIMEOUT_CNT);
		 /* For RevB, 460EX and 460ExR Rev B */
		writel(PPC4XX_PD_DONE_INT | PPC4XX_TMO_ERR_INT,
		dev->ce_base + CRYPTO4XX_INT_EN);
	} else 
	writel(PPC4XX_PD_DONE_INT, dev->ce_base + CRYPTO4XX_INT_EN);
}

void crypto4xx_dump_regs(struct crypto4xx_core_device* core_dev)
{
	u32 reg_dump;

	reg_dump = readl(core_dev->dev->ce_base + CRYPTO4XX_CTRL_STAT);
	printk("crypto4xx_dump_regs: CRYPTO4XX_PD_CTRL_STAT = 0x%08x\n", reg_dump);

	reg_dump = readl(core_dev->dev->ce_base + CRYPTO4XX_SOURCE);
	printk("crypto4xx_dump_regs: CRYPTO4XX_Source_REG = 0x%08x\n", reg_dump);
	
	reg_dump = readl(core_dev->dev->ce_base + CRYPTO4XX_DEST);
	printk("crypto4xx_dump_regs: CRYPTO4XX_Des_REG= 0x%08x\n", reg_dump);
	
	reg_dump = readl(core_dev->dev->ce_base + CRYPTO4XX_SA);
	printk("crypto4xx_dump_regs: CRYPTO4XX_SA_REG= 0x%08x\n", reg_dump);
	
	reg_dump = readl(core_dev->dev->ce_base + CRYPTO4XX_PE_DMA_CFG);
	printk("crypto4xx_dump_regs: CRYPTO4XX_PE_DMA_CFG = 0x%08x\n", reg_dump);
	
	reg_dump = readl(core_dev->dev->ce_base + CRYPTO4XX_RING_SIZE);
	printk("crypto4xx_dump_regs: CRYPTO4XX_RING_SIZE  = 0x%08x\n", reg_dump);
	
	reg_dump = readl(core_dev->dev->ce_base + CRYPTO4XX_RING_CTRL);
	printk("crypto4xx_dump_regs: CRYPTO4XX_RING_CTRL  = 0x%08x\n", reg_dump);
	
	reg_dump = readl(core_dev->dev->ce_base + CRYPTO4XX_IO_THRESHOLD);
	printk("crypto4xx_dump_regs: CRYPTO4XX_IO_THRESHOLD   = 0x%08x\n", reg_dump);
	
	reg_dump = readl(core_dev->dev->ce_base + CRYPTO4XX_PE_DMA_STAT);
	printk("crypto4xx_dump_regs: CRYPTO4XX_PE_DMA_STAT= 0x%08x\n", reg_dump);
	
	reg_dump = readl(core_dev->dev->ce_base + CRYPTO4XX_PDR_BASE);
	printk("crypto4xx_dump_regs: CRYPTO4XX_PDR_BASE   = 0x%08x\n", reg_dump);
	
	reg_dump = readl(core_dev->dev->ce_base + CRYPTO4XX_STATE_PTR);
	printk("crypto4xx_dump_regs: CRYPTO4XX_STATE_PTR  = 0x%08x\n", reg_dump);
	
	reg_dump = readl(core_dev->dev->ce_base + CRYPTO4XX_SA_CMD_0);
	printk("crypto4xx_dump_regs: CRYPTO4XX_SA_CMD_0   = 0x%08x\n", reg_dump);
	
	reg_dump = readl(core_dev->dev->ce_base + CRYPTO4XX_SA_CMD_1);
	printk("crypto4xx_dump_regs: CRYPTO4XX_SA_CMD_1   = 0x%08x\n", reg_dump);
	
	reg_dump = readl(core_dev->dev->ce_base + CRYPTO4XX_SPI);
	printk("crypto4xx_dump_regs: CRYPTO4XX_SPI   = 0x%08x\n", reg_dump);

	reg_dump = readl(core_dev->dev->ce_base + CRYPTO4XX_SEQ_NUM0);
	printk("crypto4xx_dump_regs: CRYPTO4XX_SEQ_NUM_0   = 0x%08x\n", reg_dump);

	reg_dump = readl(core_dev->dev->ce_base + CRYPTO4XX_SEQ_NUM1);
	printk("crypto4xx_dump_regs: CRYPTO4XX_SEQ_NUM_1   = 0x%08x\n", reg_dump);

	reg_dump = readl(core_dev->dev->ce_base + CRYPTO4XX_STATE_IV + 0);
	printk("crypto4xx_dump_regs: CRYPTO4XX_STATE_IV + 0   = 0x%08x\n", reg_dump);
	
	reg_dump = readl(core_dev->dev->ce_base + CRYPTO4XX_STATE_IV + 4);
	printk("crypto4xx_dump_regs: CRYPTO4XX_STATE_IV + 4   = 0x%08x\n", reg_dump);
	
	reg_dump = readl(core_dev->dev->ce_base +CRYPTO4XX_STATE_IV + 8);
	printk("crypto4xx_dump_regs: CRYPTO4XX_STATE_IV + 8   = 0x%08x\n", reg_dump);
	
	reg_dump = readl(core_dev->dev->ce_base + CRYPTO4XX_STATE_IV + 12);
	printk("crypto4xx_dump_regs: CRYPTO4XX_STATE_IV + 12  = 0x%08x\n", reg_dump);
	
	reg_dump = readl(core_dev->dev->ce_base + CRYPTO4XX_STATE_HASH_BYTE_CNT_0);
	printk("crypto4xx_dump_regs: CRYPTO4XX_STATE_HASH_BYTE_CNT_0  = 0x%08x\n", reg_dump);
	
}

int crypto4xx_alloc_sa(struct crypto4xx_ctx *ctx, u32 size)
{
#ifdef CONFIG_SEC_SA_OCM
	ctx->sa_out = ocm_alloc(&ctx->sa_out_ocm_addr, size * 4, 4,
				 OCM_NON_CACHED, "sec_sa_out");
	ctx->sa_out_dma_addr = (u32)ctx->sa_out_ocm_addr;
	printk("OCM Allocation done for SA Out %llx\n", (unsigned long long)ctx->sa_out_ocm_addr);
	if (ctx->sa_out == NULL)
		return -ENOMEM;
	ctx->sa_in = ocm_alloc(&ctx->sa_in_ocm_addr, size * 4, 4,
				OCM_NON_CACHED, "sec_sa_in");
	if (ctx->sa_in == NULL) {
		ocm_free(ctx->sa_out);
		return -ENOMEM;
	}
	ctx->sa_in_dma_addr = (u32)ctx->sa_in_ocm_addr;
	//printk("OCM Allocation done for SA In %llx\n", (unsigned long long)ctx->sa_in_ocm_addr);
	memset(ctx->sa_in, 0, size * 4);
	memset(ctx->sa_out, 0, size * 4);
	ctx->sa_len = size;
	return 0;
#endif
	ctx->sa_in = dma_alloc_coherent(ctx->dev->core_dev->device, size * 4,
					&ctx->sa_in_dma_addr, GFP_ATOMIC);
	if (ctx->sa_in == NULL)
		return -ENOMEM;

	ctx->sa_out = dma_alloc_coherent(ctx->dev->core_dev->device, size * 4,
					 &ctx->sa_out_dma_addr, GFP_ATOMIC);
	if (ctx->sa_out == NULL) {
		dma_free_coherent(ctx->dev->core_dev->device,
				  ctx->sa_len * 4,
				  ctx->sa_in, ctx->sa_in_dma_addr);
		return -ENOMEM;
	}

	memset(ctx->sa_in, 0, size * 4);
	memset(ctx->sa_out, 0, size * 4);
	ctx->sa_len = size;

	return 0;
}

void crypto4xx_free_sa(struct crypto4xx_ctx *ctx)
{
#ifdef CONFIG_SEC_SA_OCM
	ocm_free(ctx->sa_out);
	ocm_free(ctx->sa_in);
	ctx->sa_in_dma_addr = 0;
	ctx->sa_out_dma_addr = 0;
	ctx->sa_len = 0;
	return;
#endif
	if (ctx->sa_in != NULL)
		dma_free_coherent(ctx->dev->core_dev->device, ctx->sa_len * 4,
				  ctx->sa_in, ctx->sa_in_dma_addr);
	if (ctx->sa_out != NULL)
		dma_free_coherent(ctx->dev->core_dev->device, ctx->sa_len * 4,
				  ctx->sa_out, ctx->sa_out_dma_addr);

	ctx->sa_in_dma_addr = 0;
	ctx->sa_out_dma_addr = 0;
	ctx->sa_in = NULL;
	ctx->sa_out = NULL;
	ctx->sa_len = 0;
}

u32 crypto4xx_alloc_state_record(struct crypto4xx_ctx *ctx)
{
#ifdef CONFIG_SEC_SA_OCM
	ctx->state_record = ocm_alloc(&ctx->state_record_ocm_addr,
				       sizeof(struct sa_state_record), 4,
				       OCM_NON_CACHED, "sec_state_record");
	if (ctx->state_record == NULL)
		return -ENOMEM;
	ctx->state_record_dma_addr = (u32)ctx->state_record_ocm_addr;
#else
	ctx->state_record = dma_alloc_coherent(ctx->dev->core_dev->device,
				sizeof(struct sa_state_record),
				&ctx->state_record_dma_addr, GFP_ATOMIC);

	if (!ctx->state_record_dma_addr || !ctx->state_record)
		return -ENOMEM;
#endif
	memset(ctx->state_record, 0, sizeof(struct sa_state_record));

	return 0;
}

void crypto4xx_free_state_record(struct crypto4xx_ctx *ctx)
{
#ifdef CONFIG_SEC_SA_OCM
	if (ctx->state_record != NULL)
		ocm_free(ctx->state_record);
#else
	if (ctx->state_record != NULL)
		dma_free_coherent(ctx->dev->core_dev->device,
				  sizeof(struct sa_state_record),
				  ctx->state_record,
				  ctx->state_record_dma_addr);
#endif
	ctx->state_record = NULL;
	ctx->state_record_dma_addr = 0;
}

u32 crypto4xx_alloc_arc4_state_record(struct crypto4xx_ctx *ctx)
{
#ifdef CONFIG_SEC_SA_OCM
	ctx->arc4_state_record = ocm_alloc(&ctx->arc4_state_ocm_addr,
				       sizeof(struct arc4_sr), 4,
				       OCM_NON_CACHED, "sec_state_arc4_record");
	if (ctx->arc4_state_record == NULL)
		return -ENOMEM;
	ctx->arc4_state_record_dma_addr = (u32)ctx->arc4_state_ocm_addr;
#else
	ctx->arc4_state_record = dma_alloc_coherent(ctx->dev->core_dev->device,
			sizeof(struct arc4_sr),
			/* &dma_addr */ &ctx->arc4_state_record_dma_addr,
			GFP_ATOMIC);

	if (!ctx->arc4_state_record_dma_addr)
		return -ENOMEM;
#endif
	memset(ctx->arc4_state_record, 0, sizeof(struct arc4_sr));

	return 0;
}

void crypto4xx_free_arc4_state_record(struct crypto4xx_ctx *ctx)
{

	if (ctx->arc4_state_record != NULL) {
#ifdef CONFIG_SEC_SA_OCM
	ocm_free(ctx->arc4_state_record);
	
#else
	dma_free_coherent(ctx->dev->core_dev->device,
			  sizeof(struct arc4_sr),
			  ctx->arc4_state_record,
			  ctx->arc4_state_record_dma_addr);
#endif
	}
	ctx->arc4_state_record = NULL;
	ctx->arc4_state_record_dma_addr = 0;
}

int datalen_check;
static int crypto4xx_device_read_procmem(char *buffer, char **start, off_t offset,
			      int count, int *eof, void *data)
{
	struct crypto4xx_core_device *core_dev = (struct crypto4xx_core_device *)data;
	int len = 0;
	u32 ring_ctrl_val;

	ring_ctrl_val = readl(core_dev->dev->ce_base + CRYPTO4XX_RING_CTRL);

	len += sprintf(buffer + len, "ring_ctrl_val = 0x%08x\n", ring_ctrl_val);
	len += sprintf(buffer + len,
		       "Crypto4xx Controller on AMCC PPC 460EX Canyonlands Board\n");
	len += sprintf(buffer + len,
		       "%u packets received for packetsize = %d\n", core_dev->dev->pkt_cnt,
		       datalen_check);
	len += sprintf(buffer + len,
		       "%lld interrupts received\n", core_dev->irq_cnt);
	*eof = 1;
	return len;
}
/**
 * alloc memory for the gather ring
 * no need to alloc buf for the ring
 * gdr_tail, gdr_head and gdr_count are initialized by this function
 */
static u32 crypto4xx_build_pdr(struct crypto4xx_device *dev)
{
	int i;
	struct pd_uinfo *pd_uinfo;

#ifdef CONFIG_SEC_PD_OCM
	int pd_size;
	pd_size = sizeof(struct ce_pd) * PPC4XX_NUM_PD;
	dev->pdr = ocm_alloc(&dev->pdr_ocm_addr, pd_size, 4,
				 OCM_NON_CACHED, "sec_pd");
	dev->pdr_pa = (u32)dev->pdr_ocm_addr;
	printk(KERN_INFO "Security OCM Allocation done for packet Descriptor: %llx,\n"
			"Virtual OCM Address: %p, OCM Allocation size: %d\n",
			(unsigned long long)dev->pdr_ocm_addr, dev->pdr, pd_size);
	if (dev->pdr == NULL) {
		printk("PD Allocation failed on OCM\n");
		return -ENOMEM;
	}
#else
	dev->pdr = dma_alloc_coherent(dev->core_dev->device,
				      sizeof(struct ce_pd) * PPC4XX_NUM_PD,
				      &dev->pdr_pa, GFP_ATOMIC);
	if (!dev->pdr)
		return -ENOMEM;
#endif
	dev->pdr_uinfo = kzalloc(sizeof(struct pd_uinfo) * PPC4XX_NUM_PD,
				GFP_KERNEL);
	if (!dev->pdr_uinfo) {
#ifndef CONFIG_SEC_PD_OCM
		dma_free_coherent(dev->core_dev->device,
				  sizeof(struct ce_pd) * PPC4XX_NUM_PD,
				  dev->pdr,
				  dev->pdr_pa);
		return -ENOMEM;
#endif
	}
	memset(dev->pdr, 0,  sizeof(struct ce_pd) * PPC4XX_NUM_PD);
	dev->shadow_sa_pool = dma_alloc_coherent(dev->core_dev->device,
				   256 * PPC4XX_NUM_PD,
				   &dev->shadow_sa_pool_pa,
				   GFP_ATOMIC);
	if (!dev->shadow_sa_pool)
		return -ENOMEM;

	dev->shadow_sr_pool = dma_alloc_coherent(dev->core_dev->device,
			 sizeof(struct sa_state_record) * PPC4XX_NUM_PD,
			 &dev->shadow_sr_pool_pa, GFP_ATOMIC);
	if (!dev->shadow_sr_pool)
		return -ENOMEM;
	for (i = 0; i < PPC4XX_NUM_PD; i++) {
		pd_uinfo = (struct pd_uinfo *) (dev->pdr_uinfo +
						sizeof(struct pd_uinfo) * i);

		/* alloc 256 bytes which is enough for any kind of dynamic sa */
		pd_uinfo->sa_va = dev->shadow_sa_pool + 256 * i;
		pd_uinfo->sa_pa = dev->shadow_sa_pool_pa + 256 * i;

		/* alloc state record */
		pd_uinfo->sr_va = dev->shadow_sr_pool +
		    sizeof(struct sa_state_record) * i;
		pd_uinfo->sr_pa = dev->shadow_sr_pool_pa +
		    sizeof(struct sa_state_record) * i;
	}

	return 0;
}

static void crypto4xx_destroy_pdr(struct crypto4xx_device *dev)
{
#ifndef CONFIG_SEC_PD_OCM
	if (dev->pdr != NULL)
		dma_free_coherent(dev->core_dev->device,
				  sizeof(struct ce_pd) * PPC4XX_NUM_PD,
				  dev->pdr, dev->pdr_pa);
#else
	ocm_free(dev->pdr);
#endif
	if (dev->shadow_sa_pool)
		dma_free_coherent(dev->core_dev->device, 256 * PPC4XX_NUM_PD,
				  dev->shadow_sa_pool, dev->shadow_sa_pool_pa);
	if (dev->shadow_sr_pool)
		dma_free_coherent(dev->core_dev->device,
			sizeof(struct sa_state_record) * PPC4XX_NUM_PD,
			dev->shadow_sr_pool, dev->shadow_sr_pool_pa);

	dev->pkt_cnt = 0;
	kfree(dev->pdr_uinfo);
}

static u32 crypto4xx_get_pd_from_pdr_nolock(struct crypto4xx_device *dev)
{
	u32 retval;
	u32 tmp;

	retval = dev->pdr_head;
	tmp = (dev->pdr_head + 1) % PPC4XX_NUM_PD;

	if (tmp == dev->pdr_tail)
		return ERING_WAS_FULL;

	dev->pdr_head = tmp;

	return retval;
}

static u32 crypto4xx_put_pd_to_pdr(struct crypto4xx_device *dev, u32 idx)
{
	struct pd_uinfo *pd_uinfo;
	unsigned long flags;

	pd_uinfo = (struct pd_uinfo *)(dev->pdr_uinfo +
				       sizeof(struct pd_uinfo) * idx);
	spin_lock_irqsave(&dev->core_dev->lock, flags);
	if (dev->pdr_tail != PPC4XX_LAST_PD)
		dev->pdr_tail++;
	else
		dev->pdr_tail = 0;
	pd_uinfo->state = PD_ENTRY_FREE;
	spin_unlock_irqrestore(&dev->core_dev->lock, flags);

	return 0;
}

static struct ce_pd *crypto4xx_get_pdp(struct crypto4xx_device *dev,
				       dma_addr_t *pd_dma, u32 idx)
{
	*pd_dma = dev->pdr_pa + sizeof(struct ce_pd) * idx;

	return dev->pdr + sizeof(struct ce_pd) * idx;
}

/**
 * alloc memory for the gather ring
 * no need to alloc buf for the ring
 * gdr_tail, gdr_head and gdr_count are initialized by this function
 */
static u32 crypto4xx_build_gdr(struct crypto4xx_device *dev)
{
	dev->gdr = dma_alloc_coherent(dev->core_dev->device,
				      sizeof(struct ce_gd) * PPC4XX_NUM_GD,
				      &dev->gdr_pa, GFP_ATOMIC);
	if (!dev->gdr)
		return -ENOMEM;

	memset(dev->gdr, 0, sizeof(struct ce_gd) * PPC4XX_NUM_GD);

	return 0;
}

static inline void crypto4xx_destroy_gdr(struct crypto4xx_device *dev)
{
	dma_free_coherent(dev->core_dev->device,
			  sizeof(struct ce_gd) * PPC4XX_NUM_GD,
			  dev->gdr, dev->gdr_pa);
}

/*
 * when this function is called.
 * preemption or interrupt must be disabled
 */
u32 crypto4xx_get_n_gd(struct crypto4xx_device *dev, int n)
{
	u32 retval;
	u32 tmp;
	if (n >= PPC4XX_NUM_GD)
		return ERING_WAS_FULL;

	retval = dev->gdr_head;
	tmp = (dev->gdr_head + n) % PPC4XX_NUM_GD;
	if (dev->gdr_head > dev->gdr_tail) {
		if (tmp < dev->gdr_head && tmp >= dev->gdr_tail)
			return ERING_WAS_FULL;
	} else if (dev->gdr_head < dev->gdr_tail) {
		if (tmp < dev->gdr_head || tmp >= dev->gdr_tail)
			return ERING_WAS_FULL;
	}
	dev->gdr_head = tmp;

	return retval;
}

static u32 crypto4xx_put_gd_to_gdr(struct crypto4xx_device *dev)
{
	unsigned long flags;

	spin_lock_irqsave(&dev->core_dev->lock, flags);
	if (dev->gdr_tail == dev->gdr_head) {
		spin_unlock_irqrestore(&dev->core_dev->lock, flags);
		return 0;
	}

	if (dev->gdr_tail != PPC4XX_LAST_GD)
		dev->gdr_tail++;
	else
		dev->gdr_tail = 0;

	spin_unlock_irqrestore(&dev->core_dev->lock, flags);

	return 0;
}

static inline struct ce_gd *crypto4xx_get_gdp(struct crypto4xx_device *dev,
					      dma_addr_t *gd_dma, u32 idx)
{
	*gd_dma = dev->gdr_pa + sizeof(struct ce_gd) * idx;

	return (struct ce_gd *) (dev->gdr + sizeof(struct ce_gd) * idx);
}

/**
 * alloc memory for the scatter ring
 * need to alloc buf for the ring
 * sdr_tail, sdr_head and sdr_count are initialized by this function
 */
static u32 crypto4xx_build_sdr(struct crypto4xx_device *dev)
{
	int i;
	struct ce_sd *sd_array;

	/* alloc memory for scatter descriptor ring */
	dev->sdr = dma_alloc_coherent(dev->core_dev->device,
				      sizeof(struct ce_sd) * PPC4XX_NUM_SD,
				      &dev->sdr_pa, GFP_ATOMIC);
	if (!dev->sdr)
		return -ENOMEM;

	dev->scatter_buffer_size = PPC4XX_SD_BUFFER_SIZE;
	dev->scatter_buffer_va =
		dma_alloc_coherent(dev->core_dev->device,
			dev->scatter_buffer_size * PPC4XX_NUM_SD,
			&dev->scatter_buffer_pa, GFP_ATOMIC);
	if (!dev->scatter_buffer_va) {
		dma_free_coherent(dev->core_dev->device,
				  sizeof(struct ce_sd) * PPC4XX_NUM_SD,
				  dev->sdr, dev->sdr_pa);
		return -ENOMEM;
	}

	sd_array = dev->sdr;

	for (i = 0; i < PPC4XX_NUM_SD; i++) {
		sd_array[i].ptr = dev->scatter_buffer_pa +
				  dev->scatter_buffer_size * i;
	}

	return 0;
}

static void crypto4xx_destroy_sdr(struct crypto4xx_device *dev)
{
	if (dev->sdr != NULL)
		dma_free_coherent(dev->core_dev->device,
				  sizeof(struct ce_sd) * PPC4XX_NUM_SD,
				  dev->sdr, dev->sdr_pa);

	if (dev->scatter_buffer_va != NULL)
		dma_free_coherent(dev->core_dev->device,
				  dev->scatter_buffer_size * PPC4XX_NUM_SD,
				  dev->scatter_buffer_va,
				  dev->scatter_buffer_pa);
}

/*
 * when this function is called.
 * preemption or interrupt must be disabled
 */
static u32 crypto4xx_get_n_sd(struct crypto4xx_device *dev, int n)
{
	u32 retval;
	u32 tmp;

	if (n >= PPC4XX_NUM_SD)
		return ERING_WAS_FULL;

	retval = dev->sdr_head;
	tmp = (dev->sdr_head + n) % PPC4XX_NUM_SD;
	if (dev->sdr_head > dev->gdr_tail) {
		if (tmp < dev->sdr_head && tmp >= dev->sdr_tail)
			return ERING_WAS_FULL;
	} else if (dev->sdr_head < dev->sdr_tail) {
		if (tmp < dev->sdr_head || tmp >= dev->sdr_tail)
			return ERING_WAS_FULL;
	} /* the head = tail, or empty case is already take cared */
	dev->sdr_head = tmp;

	return retval;
}

static u32 crypto4xx_put_sd_to_sdr(struct crypto4xx_device *dev)
{
	unsigned long flags;

	spin_lock_irqsave(&dev->core_dev->lock, flags);
	if (dev->sdr_tail == dev->sdr_head) {
		spin_unlock_irqrestore(&dev->core_dev->lock, flags);
		return 0;
	}
	if (dev->sdr_tail != PPC4XX_LAST_SD)
		dev->sdr_tail++;
	else
		dev->sdr_tail = 0;
	spin_unlock_irqrestore(&dev->core_dev->lock, flags);

	return 0;
}

static inline struct ce_sd *crypto4xx_get_sdp(struct crypto4xx_device *dev,
					      dma_addr_t *sd_dma, u32 idx)
{
	*sd_dma = dev->sdr_pa + sizeof(struct ce_sd) * idx;

	return  (struct ce_sd *)(dev->sdr + sizeof(struct ce_sd) * idx);
}

static u32 crypto4xx_fill_one_page(struct crypto4xx_device *dev,
				   dma_addr_t *addr, u32 *length,
				   u32 *idx, u32 *offset, u32 *nbytes)
{
	u32 len;

	if (*length > dev->scatter_buffer_size) {
		memcpy(phys_to_virt(*addr),
			dev->scatter_buffer_va +
			*idx * dev->scatter_buffer_size + *offset,
			dev->scatter_buffer_size);
		*offset = 0;
		*length -= dev->scatter_buffer_size;
		*nbytes -= dev->scatter_buffer_size;
		if (*idx == PPC4XX_LAST_SD)
			*idx = 0;
		else
			(*idx)++;
		*addr = *addr +  dev->scatter_buffer_size;
		return 1;
	} else if (*length < dev->scatter_buffer_size) {
		memcpy(phys_to_virt(*addr),
			dev->scatter_buffer_va +
			*idx * dev->scatter_buffer_size + *offset, *length);
		if ((*offset + *length) == dev->scatter_buffer_size) {
			if (*idx == PPC4XX_LAST_SD)
				*idx = 0;
			else
				(*idx)++;
			*nbytes -= *length;
			*offset = 0;
		} else {
			*nbytes -= *length;
			*offset += *length;
		}

		return 0;
	} else {
		len = (*nbytes <= dev->scatter_buffer_size) ?
				(*nbytes) : dev->scatter_buffer_size;
		memcpy(phys_to_virt(*addr),
			dev->scatter_buffer_va +
			*idx * dev->scatter_buffer_size + *offset,
			len);
		*offset = 0;
		*nbytes -= len;

		if (*idx == PPC4XX_LAST_SD)
			*idx = 0;
		else
			(*idx)++;

		return 0;
    }
}

static void crypto4xx_copy_pkt_to_dst(struct crypto4xx_device *dev,
				      struct ce_pd *pd,
				      struct pd_uinfo *pd_uinfo,
				      u32 nbytes,
				      struct scatterlist *dst)
{
	dma_addr_t addr;
	u32 this_sd;
	u32 offset;
	u32 len;
	u32 i;
	u32 sg_len;
	struct scatterlist *sg;

	this_sd = pd_uinfo->first_sd;
	offset = 0;
	i = 0;

	while (nbytes) {
		sg = &dst[i];
		sg_len = sg->length;
		addr = dma_map_page(dev->core_dev->device, sg_page(sg),
				sg->offset, sg->length, DMA_TO_DEVICE);

		if (offset == 0) {
			len = (nbytes <= sg->length) ? nbytes : sg->length;
			while (crypto4xx_fill_one_page(dev, &addr, &len,
				&this_sd, &offset, &nbytes))
				;
			if (!nbytes)
				return;
			i++;
		} else {
			len = (nbytes <= (dev->scatter_buffer_size - offset)) ?
				nbytes : (dev->scatter_buffer_size - offset);
			len = (sg->length < len) ? sg->length : len;
			while (crypto4xx_fill_one_page(dev, &addr, &len,
					       &this_sd, &offset, &nbytes))
				;
			if (!nbytes)
				return;
			sg_len -= len;
			if (sg_len) {
				addr += len;
				while (crypto4xx_fill_one_page(dev, &addr,
					&sg_len, &this_sd, &offset, &nbytes))
					;
			}
			i++;
		}
	}
}

static u32 crypto4xx_copy_digest_to_dst(struct pd_uinfo *pd_uinfo,
					struct crypto4xx_ctx *ctx)
{
	struct dynamic_sa_ctl *sa = (struct dynamic_sa_ctl *) ctx->sa_in;
	struct sa_state_record *state_record =
				(struct sa_state_record *) pd_uinfo->sr_va;

	switch (sa->sa_command_0.bf.hash_alg) {
	case SA_HASH_ALG_KASUMI_f9:
		crypto4xx_memcpy_le((void *)pd_uinfo->dest_va,
				     (u8 *)state_record->save_digest, 8);
		break;
	case SA_HASH_ALG_AES_XCBC_MAC_128:
		crypto4xx_memcpy_le((void *)pd_uinfo->dest_va,
				     (u8 *) state_record->save_digest, 16);
		break;
	case SA_HASH_ALG_MD5:
		crypto4xx_memcpy_le((void *)pd_uinfo->dest_va,
				     (u8 *) state_record->save_digest,
				     SA_HASH_ALG_MD5_DIGEST_SIZE);
		break;
	default:
		memcpy((void *)pd_uinfo->dest_va,
			state_record->save_digest,
			crypto4xx_sa_hash_tbl[1][sa->sa_command_0.bf.hash_alg]);
		break;
	}

	return 0;
}

static void crypto4xx_ret_sg_desc(struct crypto4xx_device *dev,
				  struct pd_uinfo *pd_uinfo)
{
	int i;
	if (pd_uinfo->num_gd) {
		for (i = 0; i < pd_uinfo->num_gd; i++)
			crypto4xx_put_gd_to_gdr(dev);
		pd_uinfo->first_gd = 0xffffffff;
		pd_uinfo->num_gd = 0;
	}
	if (pd_uinfo->num_sd) {
		for (i = 0; i < pd_uinfo->num_sd; i++)
			crypto4xx_put_sd_to_sdr(dev);

		pd_uinfo->first_sd = 0xffffffff;
		pd_uinfo->num_sd = 0;
	}
}

void crypto4xx_append_icv_to_end(struct crypto4xx_device *dev,
				 struct scatterlist *dst,
				 struct sa_state_record *sr,
				 u32 offset,
				 u32 len)
{
	struct scatterlist *sg;
	int i = 0;
	u32 cp_len;
	dma_addr_t addr;

	sg = &dst[i];
	while (len) {
		while (sg->length < offset) {
			offset -= sg->length;
			i++;
			sg = &sg[i];
		}
		/* at here, icv could be in this sg,
		* or icv could be in the next sg
		*/
		if (sg->length > offset) {
			/* icv should be in middle of this sg */
			addr = dma_map_page(dev->core_dev->device, sg_page(sg),
					    sg->offset,
					    sg->length, DMA_TO_DEVICE);
			cp_len = (sg->length-offset >= len) ? len :
					sg->length-offset;
			len -= cp_len;
			crypto4xx_memcpy_le((u32 *)(phys_to_virt(addr)
					+ offset),
					(u8 *)sr->save_digest, cp_len);
		} else {
			/* start from begin of next sg*/
			i++;
			sg = &sg[i];
			offset = 0;
			addr = dma_map_page(dev->core_dev->device, sg_page(sg),
					    sg->offset,
					    sg->length, DMA_FROM_DEVICE);
			cp_len = (sg->length >= len) ? len : sg->length;
			len -= cp_len;
			crypto4xx_memcpy_le((u32 *) (phys_to_virt(addr)
					+ offset),
					(u8 *) sr->save_digest, cp_len);
		}
		i++;
		sg = &sg[i];
	}
}

static u32 crypto4xx_ablkcipher_done(struct crypto4xx_device *dev,
				     struct pd_uinfo *pd_uinfo,
				     struct ce_pd *pd)
{
	struct crypto4xx_ctx *ctx;
	struct ablkcipher_request *ablk_req;
	struct scatterlist *dst;
	dma_addr_t addr;

	ablk_req = ablkcipher_request_cast(pd_uinfo->async_req);
	ctx  = crypto_tfm_ctx(ablk_req->base.tfm);

	if (pd_uinfo->using_sd) {
		crypto4xx_copy_pkt_to_dst(dev, pd, pd_uinfo, ablk_req->nbytes,
					  ablk_req->dst);
	} else {
		dst = pd_uinfo->dest_va;
		addr = dma_map_page(dev->core_dev->device, sg_page(dst),
				    dst->offset, dst->length, DMA_FROM_DEVICE);
	}
	crypto4xx_ret_sg_desc(dev, pd_uinfo);

	if (pd->pd_ctl.bf.status & 0xff) {
		printk("ablkcipher return err status = 0x%08x\n",
		       pd->pd_ctl.bf.status & 0xff);
	}
	if (ablk_req->base.complete != NULL)
		ablk_req->base.complete(&ablk_req->base, 0);

	return 0;
}

static u32 crypto4xx_ahash_done(struct crypto4xx_device *dev,
				struct pd_uinfo *pd_uinfo,
				struct ce_pd *pd)
{
	struct crypto4xx_ctx *ctx;
	struct ahash_request *ahash_req;

	ahash_req = ahash_request_cast(pd_uinfo->async_req);
	ctx  = crypto_tfm_ctx(ahash_req->base.tfm);

	crypto4xx_copy_digest_to_dst(pd_uinfo,
				     crypto_tfm_ctx(ahash_req->base.tfm));
	crypto4xx_ret_sg_desc(dev, pd_uinfo);
	/* call user provided callback function x */
	if (pd->pd_ctl.bf.status & 0xff) {
		printk("ahash return err status = 0x%08x\n",
		       pd->pd_ctl.bf.status & 0xff);
	}
	if (ahash_req->base.complete != NULL)
		ahash_req->base.complete(&ahash_req->base, 0);

	return 0;
}

static u32 crypto4xx_aead_done(struct crypto4xx_device *dev,
			struct pd_uinfo *pd_uinfo,
			struct ce_pd *pd)
{
	struct aead_request *aead_req;
	struct crypto4xx_ctx *ctx;
	struct scatterlist *dst;
	dma_addr_t addr;
	struct crypto_aead *aead;

	aead_req = container_of(pd_uinfo->async_req,
				struct aead_request, base);
	aead = crypto_aead_reqtfm(aead_req);
	ctx  = crypto_tfm_ctx(aead_req->base.tfm);

	if (pd_uinfo->using_sd) {
		crypto4xx_copy_pkt_to_dst(dev, pd, pd_uinfo,
					  pd->pd_ctl_len.bf.pkt_len,
					  aead_req->dst);
	} else {
		dst = pd_uinfo->dest_va;
		addr = dma_map_page(dev->core_dev->device, sg_page(dst),
				    dst->offset,
				    dst->length, DMA_FROM_DEVICE);
	}

	if (ctx->append_icv != 0) {
		dst = pd_uinfo->dest_va;
		crypto4xx_append_icv_to_end(dev, dst,
					    (struct sa_state_record *)
					    pd_uinfo->sr_va,
					    aead_req->cryptlen,
					    crypto_aead_authsize(aead));
	}
	crypto4xx_ret_sg_desc(dev, pd_uinfo);
	/* call user provided callback function x */

	if (pd->pd_ctl.bf.status & 0xff) {
		if (pd->pd_ctl.bf.status & 1)
			printk("authentication error\n");
		if (pd->pd_ctl.bf.status & 2)
			printk("pad fail error\n");
		if (pd->pd_ctl.bf.status & 4)
			printk("seqnum fail\n");
		if (pd->pd_ctl.bf.status & 8)
			printk("error _notify\n");
		printk("aead return err status = 0x%08x\n",
		       pd->pd_ctl.bf.status & 0xff);
		printk("pd pad_ctl = 0x%08x\n", pd->pd_ctl.bf.pd_pad_ctl);
	}
	
#if 0
	void * saddr;
	dst = pd_uinfo->dest_va;
	printk("dumping aead_done length = %d\n", dst->length);
	saddr = kmap_atomic(sg_page(dst), KM_SOFTIRQ1);
	print_hex_dump(KERN_CONT, "", DUMP_PREFIX_OFFSET,
		       16, 1,
		       (void*)saddr+dst->offset, dst->length, false);
	kunmap_atomic(saddr, KM_SOFTIRQ1);
#endif
	if (aead_req->base.complete != NULL)
		aead_req->base.complete(&aead_req->base, 0);
	return 0;
}

u32 crypto4xx_pd_done(struct crypto4xx_device *dev, u32 idx)
{
	struct ce_pd *pd;
	struct pd_uinfo *pd_uinfo;

	pd =  dev->pdr + sizeof(struct ce_pd)*idx;
	pd_uinfo = dev->pdr_uinfo + sizeof(struct pd_uinfo)*idx;

	if (crypto_tfm_alg_type(pd_uinfo->async_req->tfm) ==
			CRYPTO_ALG_TYPE_AEAD)
		return crypto4xx_aead_done(dev, pd_uinfo, pd);
	else if (crypto_tfm_alg_type(pd_uinfo->async_req->tfm) ==
			CRYPTO_ALG_TYPE_ABLKCIPHER)
		return crypto4xx_ablkcipher_done(dev, pd_uinfo, pd);
	else if (crypto_tfm_alg_type(pd_uinfo->async_req->tfm) ==
			CRYPTO_ALG_TYPE_AHASH)
		return crypto4xx_ahash_done(dev, pd_uinfo, pd);

	return 0;
}

/**
 * Note: Only use this function to copy items that is word aligned.
 */
void crypto4xx_memcpy_le(unsigned int *dst,
			 const unsigned char *buf,
			 int len)
{
	u8 *tmp;
	for (; len >= 4; buf += 4, len -= 4)
		*dst++ = cpu_to_le32(*(unsigned int *) buf);

	tmp = (u8 *)dst;
	switch (len) {
	case 3:
		*tmp++ = 0;
		*tmp++ = *(buf+2);
		*tmp++ = *(buf+1);
		*tmp++ = *buf;
		break;
	case 2:
		*tmp++ = 0;
		*tmp++ = 0;
		*tmp++ = *(buf+1);
		*tmp++ = *buf;
		break;
	case 1:
		*tmp++ = 0;
		*tmp++ = 0;
		*tmp++ = 0;
		*tmp++ = *buf;
		break;
	default:
		break;
	}
}

static void crypto4xx_stop_all(struct crypto4xx_core_device *core_dev)
{
	crypto4xx_destroy_pdr(core_dev->dev);
	crypto4xx_destroy_gdr(core_dev->dev);
	crypto4xx_destroy_sdr(core_dev->dev);
	dev_set_drvdata(core_dev->device, NULL);
	iounmap(core_dev->dev->ce_base);
	kfree(core_dev->dev);
	kfree(core_dev);
}

void crypto4xx_return_pd(struct crypto4xx_device *dev,
			 u32 pd_entry, struct ce_pd *pd,
			 struct pd_uinfo *pd_uinfo)
{
	/* irq should be already disabled */
	dev->pdr_head = pd_entry;
	pd->pd_ctl.w = 0;
	pd->pd_ctl_len.w = 0;
	pd_uinfo->state = PD_ENTRY_FREE;
}

/*
 * derive number of elements in scatterlist
 * Shamlessly copy from talitos.c
 */
static int get_sg_count(struct scatterlist *sg_list, int nbytes)
{
	struct scatterlist *sg = sg_list;
	int sg_nents = 0;

	while (nbytes) {
		sg_nents++;
		if (sg->length > nbytes)
			break;
		nbytes -= sg->length;
		sg = sg_next(sg);
	}

	return sg_nents;
}

static u32 get_next_gd(u32 current)
{
	if (current != PPC4XX_LAST_GD)
		return current + 1;
	else
		return 0;
}

static u32 get_next_sd(u32 current)
{
	if (current != PPC4XX_LAST_SD)
		return current + 1;
	else
		return 0;
}
extern int ssl_arc4;
extern int tls;
int var = 0;
u32 crypto4xx_build_pd(struct crypto_async_request *req,
		       struct crypto4xx_ctx *ctx,
		       struct scatterlist *src,
		       struct scatterlist *dst,
		       unsigned int datalen,
		       struct scatterlist *assoc,
		       u32 aad_len,
		       void *iv, u32 iv_len)
{
	struct crypto4xx_device *dev = ctx->dev;
	dma_addr_t addr, pd_dma, sd_dma, gd_dma;
	struct dynamic_sa_ctl *sa;
	struct scatterlist *sg;
	struct scatterlist *aad;
	struct ce_gd *gd;
	struct ce_pd *pd;
	u32 num_gd, num_sd;
	u32 fst_gd = 0xffffffff;
	u32 fst_sd = 0xffffffff;
	u32 pd_entry;
	unsigned long flags;
	struct pd_uinfo *pd_uinfo = NULL;
	unsigned int nbytes = datalen, idx;
	u32 gd_idx = 0;
	unsigned int aadlen = 0;

	datalen_check = datalen;
	/* figure how many gd is needed */
	if (aad_len) {
		num_gd = get_sg_count(assoc, aad_len) +
				get_sg_count(src, datalen);
	} else {
	num_gd = get_sg_count(src, datalen);
	if (num_gd == 1)
		num_gd = 0;
	}

	/* figure how many sd is needed */
	if (sg_is_last(dst) || ctx->is_hash) {
		num_sd = 0;
	} else {
		if (datalen > PPC4XX_SD_BUFFER_SIZE) {
			num_sd = datalen / PPC4XX_SD_BUFFER_SIZE;
			if (datalen % PPC4XX_SD_BUFFER_SIZE)
				num_sd++;
		} else {
			num_sd = 1;
		}
	}

	/*
	 * The follow section of code needs to be protected
	 * The gather ring and scatter ring needs to be consecutive
	 * In case of run out of any kind of descriptor, the descriptor
	 * already got must be return the original place.
	 */
	spin_lock_irqsave(&dev->core_dev->lock, flags);
	if (num_gd) {
		fst_gd = crypto4xx_get_n_gd(dev, num_gd);
		if (fst_gd == ERING_WAS_FULL) {
			spin_unlock_irqrestore(&dev->core_dev->lock, flags);
			return -EAGAIN;
		}
	}
	if (num_sd) {
		fst_sd = crypto4xx_get_n_sd(dev, num_sd);
		if (fst_sd == ERING_WAS_FULL) {
			if (num_gd)
				dev->gdr_head = fst_gd;
			spin_unlock_irqrestore(&dev->core_dev->lock, flags);
			return -EAGAIN;
		}
	}
	pd_entry = crypto4xx_get_pd_from_pdr_nolock(dev);
	if (pd_entry == ERING_WAS_FULL) {
		if (num_gd)
			dev->gdr_head = fst_gd;
		if (num_sd)
			dev->sdr_head = fst_sd;
		spin_unlock_irqrestore(&dev->core_dev->lock, flags);
		return -EAGAIN;
	}
	spin_unlock_irqrestore(&dev->core_dev->lock, flags);

	pd_uinfo = (struct pd_uinfo *)(dev->pdr_uinfo +
				       sizeof(struct pd_uinfo) * pd_entry);
	pd = crypto4xx_get_pdp(dev, &pd_dma, pd_entry);
	pd_uinfo->async_req = req;
	pd_uinfo->num_gd = num_gd;
	pd_uinfo->num_sd = num_sd;

	writel(0, ctx->dev->ce_base + CRYPTO4XX_SA_UADDR);
	if (iv_len || ctx->is_hash) {
		pd->sa = pd_uinfo->sa_pa;
		sa = (struct dynamic_sa_ctl *) pd_uinfo->sa_va;
		if (ctx->direction == DIR_INBOUND)
			memcpy(sa, ctx->sa_in, ctx->sa_len * 4);
		else
			memcpy(sa, ctx->sa_out, ctx->sa_len * 4);

		memcpy((void *) sa + ctx->offset_to_sr_ptr,
			&pd_uinfo->sr_pa, 4);

		if (iv_len) {
			if (ctx->ctr_aes) {
				/* First the nonce */
				memcpy(pd_uinfo->sr_va, ctx->state_record,
				       CTR_RFC3686_NONCE_SIZE);
				/* Copy the IV that is passed through
				 * each operation
				 */
				crypto4xx_memcpy_le(pd_uinfo->sr_va +
					CTR_RFC3686_NONCE_SIZE, iv, iv_len);
			} else
				crypto4xx_memcpy_le(pd_uinfo->sr_va,
						iv, iv_len);
		}
		if (ctx->is_gcm || ctx->ctr_aes) {
			u32 seq = 1;
			/*For GCM and CTR(AES) algs adding the counter value*/
			crypto4xx_memcpy_le(pd_uinfo->sr_va + 12,
					    (void *)&seq,  4);
		}
	} else {
		if (ctx->direction == DIR_INBOUND) {
#ifdef CONFIG_SEC_SA_OCM
			writel((ctx->sa_in_ocm_addr >> 32),
				ctx->dev->ce_base + CRYPTO4XX_SA_UADDR);
#endif
			pd->sa = ctx->sa_in_dma_addr;
			sa = (struct dynamic_sa_ctl *) ctx->sa_in;
		} else {
#ifdef CONFIG_SEC_SA_OCM
			writel((ctx->sa_out_ocm_addr >> 32),
				ctx->dev->ce_base + CRYPTO4XX_SA_UADDR);
#endif
			pd->sa = ctx->sa_out_dma_addr;
			sa = (struct dynamic_sa_ctl *) ctx->sa_out;
		}
	}

	//u32 sa_upper = readl(ctx->dev->ce_base + CRYPTO4XX_SA_UADDR);
	//printk("Dumping the Upper SA address = 0x%x\n", sa_upper);
	//printk("Dumping the lower SA address = 0x%x\n", pd->sa);

	pd->sa_len = ctx->sa_len;
	if (num_gd) {
		/* get first gd we are going to use */
		gd_idx = fst_gd;
		pd_uinfo->first_gd = fst_gd;
		pd_uinfo->num_gd = num_gd;
		gd = crypto4xx_get_gdp(dev, &gd_dma, gd_idx);
		pd->src = gd_dma;
		/* enable gather */
		sa->sa_command_0.bf.gather = 1;
		idx = 0;
		if (aad_len) {
			aadlen = aad_len;
			aad = assoc;
			/* walk the sg, and setup gather array for aad*/
			while (aadlen) {
				sg = &aad[idx];
				addr = dma_map_page(dev->core_dev->device,
						sg_page(sg), sg->offset,
						sg->length, DMA_TO_DEVICE);

				gd->ptr = addr;
				gd->ctl_len.len = sg->length;
				gd->ctl_len.done = 0;
				gd->ctl_len.ready = 1;

				if (sg->length >= aadlen)
					break;

				aadlen -= sg->length;

				gd_idx = get_next_gd(gd_idx);
				gd = crypto4xx_get_gdp(dev, &gd_dma, gd_idx);
				idx++;
			}
			/* prepare gd for src */
			gd_idx = get_next_gd(gd_idx);
			gd = crypto4xx_get_gdp(dev, &gd_dma, gd_idx);
		}
		idx = 0;
		src = &src[0];
		/* walk the sg, and setup gather array */
		while (nbytes) {
			sg = &src[idx];
			addr = dma_map_page(dev->core_dev->device, sg_page(sg),
				    sg->offset, sg->length, DMA_TO_DEVICE);
			gd->ptr = addr;
			gd->ctl_len.len = sg->length;
			gd->ctl_len.done = 0;
			gd->ctl_len.ready = 1;
			if (sg->length >= nbytes)
				break;
			nbytes -= sg->length;
			gd_idx = get_next_gd(gd_idx);
			gd = crypto4xx_get_gdp(dev, &gd_dma, gd_idx);
			idx++;
		}
	} else {
		pd->src = (u32)dma_map_page(dev->core_dev->device, sg_page(src),
				src->offset, src->length, DMA_TO_DEVICE);
		/*
		 * Disable gather in sa command
		 */
		sa->sa_command_0.bf.gather = 0;
		/*
		 * Indicate gather array is not used
		 */
		pd_uinfo->first_gd = 0xffffffff;
		pd_uinfo->num_gd = 0;
	}
	if (ctx->is_hash || sg_is_last(dst)) {
		/*
		 * we know application give us dst a whole piece of memory
		 * no need to use scatter ring.
		 * In case of is_hash, the icv is always at end of src data.
		 */
		pd_uinfo->using_sd = 0;
		pd_uinfo->first_sd = 0xffffffff;
		pd_uinfo->num_sd = 0;
		pd_uinfo->dest_va = dst;
		sa->sa_command_0.bf.scatter = 0;
		if (ctx->is_hash)
			pd->dest = virt_to_phys((void *)dst);
		else
			pd->dest = (u32)dma_map_page(dev->core_dev->device,
					sg_page(dst), dst->offset,
					dst->length, DMA_TO_DEVICE);
	} else {
		struct ce_sd *sd = NULL;
		u32 sd_idx = fst_sd;
		nbytes = datalen;
		sa->sa_command_0.bf.scatter = 1;
		pd_uinfo->using_sd = 1;
		pd_uinfo->dest_va = dst;
		pd_uinfo->first_sd = fst_sd;
		pd_uinfo->num_sd = num_sd;
		sd = crypto4xx_get_sdp(dev, &sd_dma, sd_idx);
		pd->dest = sd_dma;
		/* setup scatter descriptor */
		sd->ctl.done = 0;
		sd->ctl.rdy = 1;
		/* sd->ptr should be setup by sd_init routine*/
		idx = 0;
		if (nbytes >= PPC4XX_SD_BUFFER_SIZE)
			nbytes -= PPC4XX_SD_BUFFER_SIZE;
		else
			nbytes = 0;
		while (nbytes) {
			sd_idx = get_next_sd(sd_idx);
			sd = crypto4xx_get_sdp(dev, &sd_dma, sd_idx);
			/* setup scatter descriptor */
			sd->ctl.done = 0;
			sd->ctl.rdy = 1;
			if (nbytes >= PPC4XX_SD_BUFFER_SIZE)
				nbytes -= PPC4XX_SD_BUFFER_SIZE;
			else
				/*
				 * SD entry can hold PPC4XX_SD_BUFFER_SIZE,
				 * which is more than nbytes, so done.
				 */
				nbytes = 0;
		}
	}

	sa->sa_command_1.bf.hash_crypto_offset = (aad_len >> 2);
	pd->pd_ctl.w = ctx->pd_ctl;
	pd->pd_ctl_len.w = 0x00400000 | (ctx->bypass << 24) |
			(datalen + aad_len);
	if (ctx->next_hdr)
		pd->pd_ctl.bf.next_hdr = ctx->next_hdr;
	pd_uinfo->state = PD_ENTRY_INUSE;
	wmb();
	/* write any value to push engine to read a pd */
	if (dev->core_dev->revb_ver == 1) {
#ifndef CONFIG_SEC_HW_POLL
		writel(1, dev->ce_base + CRYPTO4XX_INT_DESCR_RD);
#endif
	} else 
	writel(1, dev->ce_base + CRYPTO4XX_INT_DESCR_RD);

		
	dev->pkt_cnt++;
	return -EINPROGRESS;
}

/**
 * Algorithm Registration Functions
 */
static int crypto4xx_alg_init(struct crypto_tfm *tfm)
{
	struct crypto_alg *alg = tfm->__crt_alg;
	struct crypto4xx_alg *amcc_alg = crypto_alg_to_crypto4xx_alg(alg);
	struct crypto4xx_ctx *ctx = crypto_tfm_ctx(tfm);

	ctx->dev = amcc_alg->dev;
	ctx->sa_in = NULL;
	ctx->sa_out = NULL;
	ctx->sa_in_dma_addr = 0;
	ctx->sa_out_dma_addr = 0;
	ctx->sa_len = 0;
	ctx->is_gcm = 0;
	ctx->append_icv = 0;

	switch (alg->cra_flags & CRYPTO_ALG_TYPE_MASK) {
	default:
		tfm->crt_ablkcipher.reqsize = sizeof(struct crypto4xx_ctx);
		break;
	case CRYPTO_ALG_TYPE_AHASH:
		crypto_ahash_set_reqsize(__crypto_ahash_cast(tfm),
					 sizeof(struct crypto4xx_ctx));
		break;
	case CRYPTO_ALG_TYPE_AEAD:
		tfm->crt_aead.reqsize = sizeof(struct crypto4xx_ctx);
		break;
	}

	return 0;
}

static void crypto4xx_alg_exit(struct crypto_tfm *tfm)
{
	struct crypto4xx_ctx *ctx = crypto_tfm_ctx(tfm);

	crypto4xx_free_sa(ctx);
	crypto4xx_free_state_record(ctx);
	crypto4xx_free_arc4_state_record(ctx);
}

int crypto4xx_register_alg(struct crypto4xx_device *sec_dev,
			   struct crypto4xx_alg_common *crypto_alg,
			   int array_size)
{
	struct crypto4xx_alg *alg;
	int i;
	int rc = 0;

	for (i = 0; i < array_size; i++) {
		alg = kzalloc(sizeof(struct crypto4xx_alg), GFP_KERNEL);
		if (!alg)
			return -ENOMEM;

		alg->alg = crypto_alg[i];
		alg->dev = sec_dev;

		switch (alg->alg.type) {
		case CRYPTO_ALG_TYPE_AHASH:
			rc = crypto_register_ahash(&alg->alg.u.hash);
			break;

		default:
			rc = crypto_register_alg(&alg->alg.u.cipher);
			break;
		}

		if (rc) {
			list_del(&alg->entry);
			kfree(alg);
		} else {
			list_add_tail(&alg->entry, &sec_dev->alg_list);
		}
	}

	return 0;
}

static void crypto4xx_unregister_alg(struct crypto4xx_device *sec_dev)
{
	struct crypto4xx_alg *alg, *tmp;

	list_for_each_entry_safe(alg, tmp, &sec_dev->alg_list, entry) {
		list_del(&alg->entry);
		switch (alg->alg.type) {
		case CRYPTO_ALG_TYPE_AHASH:
			crypto_unregister_ahash(&alg->alg.u.hash);
			break;

		default:
			crypto_unregister_alg(&alg->alg.u.cipher);
		}
		kfree(alg);
	}
}

static void crypto4xx_bh_tasklet_cb(unsigned long data)
{
	struct device *dev = (struct device *)data;
	struct crypto4xx_core_device *core_dev = dev_get_drvdata(dev);
	struct pd_uinfo *pd_uinfo;
	struct ce_pd *pd;
	u32 tail;

	while (core_dev->dev->pdr_head != core_dev->dev->pdr_tail) {
		tail = core_dev->dev->pdr_tail;
		pd_uinfo = core_dev->dev->pdr_uinfo +
			sizeof(struct pd_uinfo)*tail;
		pd =  core_dev->dev->pdr + sizeof(struct ce_pd) * tail;
		if ((pd_uinfo->state == PD_ENTRY_INUSE) &&
				   pd->pd_ctl.bf.pe_done &&
				   !pd->pd_ctl.bf.host_ready) {
			pd->pd_ctl.bf.pe_done = 0;
			crypto4xx_pd_done(core_dev->dev, tail);
			crypto4xx_put_pd_to_pdr(core_dev->dev, tail);
			pd_uinfo->state = PD_ENTRY_FREE;
		} else {
			/* if tail not done, break */
			break;
		}
	}
}
#define SRAM_OCM_ADDR_ERR		0x0B4
#define SRAM_OCM_STATUS0		0x0B5
#define SRAM_OCM_STATUS1		0X0B6

#define PLBA0_ESRL			0x0082
#define PLBA0_ESRH			0x0083
#define PLBA0_EARL			0x0084
#define PLBA0_EARH			0x0085

/**
 * Top Half of isr.
 */
static irqreturn_t crypto4xx_ce_interrupt_handler(int irq, void *data)
{
	struct device *dev = (struct device *)data;
	struct crypto4xx_core_device *core_dev = dev_get_drvdata(dev);
	//u32 int_status;

	if (core_dev->dev->ce_base == 0)
		return 0;

	//int_status = readl(core_dev->dev->ce_base + CRYPTO4XX_INT_UNMASK_STAT);
	//printk("Interrupt status = 0x%08x\n", int_status);

	/* For RevB, 460EX and 460ExR Rev B */
	if (core_dev->revb_ver == 1) {
		writel(PPC4XX_INTERRUPT_CLR_REVB,
			core_dev->dev->ce_base + CRYPTO4XX_INT_CLR);
	} else {
	writel(PPC4XX_INTERRUPT_CLR,
	       core_dev->dev->ce_base + CRYPTO4XX_INT_CLR);
	}

	core_dev->irq_cnt++;
	tasklet_schedule(&core_dev->tasklet);

	return IRQ_HANDLED;
}


/**
 * Supported Crypto Algorithms
 */
struct crypto4xx_alg_common crypto4xx_alg[] = {
		/* Crypto DES ECB, CBC,  modes */
#if 1
        { .type = CRYPTO_ALG_TYPE_ABLKCIPHER, .u.cipher = {
		.cra_name 		= "cbc(des)",
 		.cra_driver_name 	= "ppc4xx-cbc-des",
 		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY,
 		.cra_flags 		= CRYPTO_ALG_TYPE_ABLKCIPHER | CRYPTO_ALG_ASYNC,
 		.cra_blocksize 		= DES_BLOCK_SIZE,
 		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
 		.cra_alignmask 		= 0,
 		.cra_type 		= &crypto_ablkcipher_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
 		.cra_module 		= THIS_MODULE,
 		.cra_u 		= {
			.ablkcipher = {
				.min_keysize 	= DES_KEY_SIZE,
				.max_keysize 	= DES_KEY_SIZE,
				.ivsize 	= DES_BLOCK_SIZE,
				.setkey 	= crypto4xx_setkey_3des_cbc,
				.encrypt 	= crypto4xx_encrypt,
				.decrypt 	= crypto4xx_decrypt,
			}
		}
	}},
        { .type = CRYPTO_ALG_TYPE_ABLKCIPHER, .u.cipher = {
		.cra_name 		= "ecb(des)",
		.cra_driver_name 	= "ppc4xx-ecb-des",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY,
		.cra_flags 		= CRYPTO_ALG_TYPE_ABLKCIPHER | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= DES_BLOCK_SIZE,
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_ablkcipher_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
		.cra_module 		= THIS_MODULE,
		.cra_u 		= {
			.ablkcipher = {
				.min_keysize 	= DES_KEY_SIZE,
				.max_keysize 	= DES_KEY_SIZE,
				.setkey 	= crypto4xx_setkey_3des_ecb,
				.encrypt 	= crypto4xx_encrypt,
				.decrypt 	= crypto4xx_decrypt,
			}
		}
	}},

	/* Crypto 3DES ECB, CBC, CFB, and OFB modes */
        { .type = CRYPTO_ALG_TYPE_ABLKCIPHER, .u.cipher = {
			.cra_name 		= "cbc(des3_ede)",
		.cra_driver_name 	= "ppc4xx-cbc-3des",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY,
		.cra_flags		= CRYPTO_ALG_TYPE_ABLKCIPHER | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= DES3_EDE_BLOCK_SIZE,
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_ablkcipher_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
		.cra_module 		= THIS_MODULE,
		.cra_u 		= {
			.ablkcipher = {
				.min_keysize 	= DES3_EDE_KEY_SIZE,
				.max_keysize 	= DES3_EDE_KEY_SIZE,
				.ivsize 	= DES3_EDE_BLOCK_SIZE,
				.setkey 	= crypto4xx_setkey_3des_cbc,
				.encrypt 	= crypto4xx_encrypt,
				.decrypt 	= crypto4xx_decrypt,
			}
		}
	}},
        { .type = CRYPTO_ALG_TYPE_ABLKCIPHER, .u.cipher = {
		.cra_name 		= "ecb(des3_ede)",
 		.cra_driver_name 	= "ppc4xx-ecb-3des",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY,
		.cra_flags 		= CRYPTO_ALG_TYPE_ABLKCIPHER | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= DES3_EDE_BLOCK_SIZE,
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_ablkcipher_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
		.cra_module 		= THIS_MODULE,
		.cra_u 		= {
			.ablkcipher = {
				.min_keysize 	= DES3_EDE_KEY_SIZE,
				.max_keysize 	= DES3_EDE_KEY_SIZE,
				.setkey 	= crypto4xx_setkey_3des_ecb,
				.encrypt 	= crypto4xx_encrypt,
				.decrypt 	= crypto4xx_decrypt,
			}
		}
	}},
	/* Crypto AES modes */
	{ .type = CRYPTO_ALG_TYPE_ABLKCIPHER, .u.cipher = {
		.cra_name 	= "cbc(aes)",
		.cra_driver_name = "cbc-aes-ppc4xx",
		.cra_priority 	= CRYPTO4XX_CRYPTO_PRIORITY,
		.cra_flags 	= CRYPTO_ALG_TYPE_ABLKCIPHER | CRYPTO_ALG_ASYNC,
		.cra_blocksize 	= AES_BLOCK_SIZE,
		.cra_ctxsize 	= sizeof(struct crypto4xx_ctx),
		.cra_type 	= &crypto_ablkcipher_type,
		.cra_init	= crypto4xx_alg_init,
		.cra_exit	= crypto4xx_alg_exit,
		.cra_module 	= THIS_MODULE,
		.cra_u 		= {
			.ablkcipher = {
				.min_keysize 	= AES_MIN_KEY_SIZE,
				.max_keysize 	= AES_MAX_KEY_SIZE,
				.ivsize		= AES_IV_SIZE,
				.setkey 	= crypto4xx_setkey_aes_cbc,
				.encrypt 	= crypto4xx_encrypt,
				.decrypt 	= crypto4xx_decrypt,
			}
		}
	}},
        { .type = CRYPTO_ALG_TYPE_ABLKCIPHER, .u.cipher = {
		.cra_name 		= "ofb(aes)",
		.cra_driver_name 	= "ppc4xx-ofb-aes",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY,
		.cra_flags 		= CRYPTO_ALG_TYPE_ABLKCIPHER | CRYPTO_ALG_ASYNC,
		.cra_blocksize		= AES_BLOCK_SIZE,
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_ablkcipher_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
		.cra_module 		= THIS_MODULE,
		.cra_u 		= {
			.ablkcipher = {
				.min_keysize 	= AES_MIN_KEY_SIZE,
				.max_keysize 	= AES_MAX_KEY_SIZE,
				.ivsize 	= AES_BLOCK_SIZE,
				.setkey 	= crypto4xx_setkey_aes_ofb,
				.encrypt 	= crypto4xx_encrypt,
				.decrypt 	= crypto4xx_decrypt,
			}
		}
	}},
        { .type = CRYPTO_ALG_TYPE_ABLKCIPHER, .u.cipher = {
		.cra_name 		= "cfb(aes)",
		.cra_driver_name 	= "ppc4xx-cfb-aes",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY,
		.cra_flags 		= CRYPTO_ALG_TYPE_ABLKCIPHER | CRYPTO_ALG_ASYNC,
		.cra_blocksize		= AES_BLOCK_SIZE,
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_ablkcipher_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
		.cra_module 		= THIS_MODULE,
		.cra_u 		= {
			.ablkcipher = {
				.min_keysize 	= AES_MIN_KEY_SIZE,
				.max_keysize 	= AES_MAX_KEY_SIZE,
				.ivsize 	= AES_BLOCK_SIZE,
				.setkey 	= crypto4xx_setkey_aes_cfb,
				.encrypt 	= crypto4xx_encrypt,
				.decrypt 	= crypto4xx_decrypt,
			}
		}
	}},
	/* Crypto AES ECB, CBC, CTR, GCM, CCM, and GMAC modes */
        { .type = CRYPTO_ALG_TYPE_ABLKCIPHER, .u.cipher = {
		.cra_name 		= "ecb(aes)",
		.cra_driver_name 	= "ppc4xx-ecb-aes",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY,
		.cra_flags 		= CRYPTO_ALG_TYPE_ABLKCIPHER | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= AES_BLOCK_SIZE,
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_ablkcipher_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
		.cra_module 		= THIS_MODULE,
		.cra_u 		= {
			.ablkcipher = {
				.min_keysize 	= AES_MIN_KEY_SIZE,
				.max_keysize 	= AES_MAX_KEY_SIZE,
				.setkey 	= crypto4xx_setkey_aes_ecb,
				.encrypt 	= crypto4xx_encrypt,
				.decrypt 	= crypto4xx_decrypt,
			}
		}
	}},
        { .type = CRYPTO_ALG_TYPE_ABLKCIPHER, .u.cipher = {
		.cra_name 		= "rfc3686(ctr(aes))",
		.cra_driver_name 	= "ppc4xx-ctr-aes",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY,
		.cra_flags 		= CRYPTO_ALG_TYPE_ABLKCIPHER | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= CTR_RFC3686_BLOCK_SIZE,
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type		= &crypto_ablkcipher_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
		.cra_module 		= THIS_MODULE,
		.cra_u 		= {
			.ablkcipher = {
				.min_keysize 	= AES_MIN_KEY_SIZE,
				.max_keysize 	= AES_MAX_KEY_SIZE,
				.ivsize 	= CTR_RFC3686_BLOCK_SIZE,
				.setkey 	= crypto4xx_setkey_aes_ctr,
				.encrypt 	= crypto4xx_encrypt_ctr,
				.decrypt 	= crypto4xx_decrypt_ctr,
			}
		}
	}},

	/* AEAD Algorithms */
        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "gcm(aes)",
		.cra_driver_name 	= "ppc4xx-gcm-aes",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= AES_BLOCK_SIZE,
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
		.cra_module 		= THIS_MODULE,
		.cra_u 		= {
			.aead = {
				.maxauthsize	= 16,
				.ivsize 	= 12,
				.setkey 	= crypto4xx_setkey_aes_gcm,
				.setauthsize	= crypto4xx_setauthsize_aes,
				.encrypt 	= crypto4xx_encrypt_aes_gcm,
				.decrypt 	= crypto4xx_decrypt_aes_gcm,
				.givencrypt	= crypto4xx_givencrypt_aes_gcm,
				.givdecrypt	= crypto4xx_givdecrypt_aes_gcm,
			}
		}
	}},
        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "ccm(aes)",
		.cra_driver_name 	= "ppc4xx-ccm-aes",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= AES_BLOCK_SIZE,
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
		.cra_module 		= THIS_MODULE,
		.cra_u 		= {
			.aead = {
				.ivsize 	= AES_BLOCK_SIZE,
				.maxauthsize	= 16,
				.setkey		= crypto4xx_setkey_aes_ccm,
				.setauthsize	= crypto4xx_setauthsize_aes,
				.encrypt	= crypto4xx_encrypt_aes_ccm,
				.decrypt	= crypto4xx_decrypt_aes_ccm,
				.givencrypt	= crypto4xx_givencrypt_aes_ccm,
				.givdecrypt	= crypto4xx_givdecrypt_aes_ccm,
			}
		}
	}},

	/* Hash MD5 */
        { .type = CRYPTO_ALG_TYPE_AHASH, .u.hash = {
		.init		= crypto4xx_hash_init,
		.update		= crypto4xx_hash_update,
		.final 		= crypto4xx_hash_final,
		.digest		= crypto4xx_hash_digest,
		.halg.digestsize 	= SA_HASH_ALG_MD5_DIGEST_SIZE,
		.halg.base 	= {
			.cra_name 		= "md5",
			.cra_driver_name 	= "ppc4xx-md5",
			.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY,
			.cra_flags 		= CRYPTO_ALG_TYPE_AHASH | CRYPTO_ALG_ASYNC,
			.cra_blocksize 		= 64,
			.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
			.cra_alignmask 		= 0,
			.cra_type 		= &crypto_ahash_type,
			.cra_init 		= crypto4xx_md5_alg_init,
			.cra_exit		= crypto4xx_alg_exit,
			.cra_module 		= THIS_MODULE,
		}
	}},
#endif
#if 1
	/* Hash MD5-HMAC */
        { .type = CRYPTO_ALG_TYPE_AHASH, .u.hash = {
		.init		= crypto4xx_hash_init,
		.update		= crypto4xx_hash_update,
		.final 		= crypto4xx_hash_final,
		.digest		= crypto4xx_hash_digest,
		.setkey		= crypto4xx_md5_hmac_setkey,
		.halg.digestsize	= SA_HASH_ALG_MD5_DIGEST_SIZE,
		.halg.base = {
			.cra_name 		= "hmac(md5)",
			.cra_driver_name 	= "ppc4xx-hmac-md5",
			.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY,
			.cra_flags 		= CRYPTO_ALG_TYPE_AHASH | CRYPTO_ALG_ASYNC,
			.cra_blocksize 		= 64,
			.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
			.cra_alignmask 		= 0,
			.cra_type 		= &crypto_ahash_type,
			.cra_module 		= THIS_MODULE,
		}
	}},

	/* Hash SHA1, SHA2 and HMAC */
        { .type = CRYPTO_ALG_TYPE_AHASH, .u.hash = {
		.init   	= crypto4xx_hash_init,
		.update 	= crypto4xx_hash_update,
		.final  	= crypto4xx_hash_final,
		.digest 	= crypto4xx_hash_digest,
		.halg.digestsize	= SHA1_DIGEST_SIZE,
		.halg.base = {
			.cra_name 		= "sha1",
			.cra_driver_name 	= "ppc4xx-sha1",
			.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY,
			.cra_flags 		= CRYPTO_ALG_TYPE_AHASH | CRYPTO_ALG_ASYNC,
			.cra_blocksize 		= SHA1_BLOCK_SIZE,
			.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
			.cra_alignmask 		= 0,
			.cra_type   		= &crypto_ahash_type,
			.cra_init 		= crypto4xx_sha1_alg_init,
			.cra_exit		= crypto4xx_alg_exit,
			.cra_module 		= THIS_MODULE,
		}
	}},
#endif
        { .type = CRYPTO_ALG_TYPE_AHASH, .u.hash = {
		.init   	= crypto4xx_hash_init,
		.update 	= crypto4xx_hash_update,
		.final  	= crypto4xx_hash_final,
		.digest 	= crypto4xx_hash_digest,
		.setkey 	= crypto4xx_sha1_hmac_setkey,
		.halg.digestsize 	= SHA1_DIGEST_SIZE,
		.halg.base = {
			.cra_name 		= "hmac(sha1)",
			.cra_driver_name 	= "ppc4xx-hmac-sha1",
			.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY,
			.cra_flags 		= CRYPTO_ALG_TYPE_AHASH | CRYPTO_ALG_ASYNC,
			.cra_blocksize 		= SHA1_BLOCK_SIZE,
			.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
			.cra_alignmask 		= 0,
			.cra_type   		= &crypto_ahash_type,
			.cra_module 		= THIS_MODULE,
		}
	}},

#if 1
        { .type = CRYPTO_ALG_TYPE_AHASH, .u.hash = {
		.init		= crypto4xx_hash_init,
		.update		= crypto4xx_hash_update,
		.final 		= crypto4xx_hash_final,
		.digest		= crypto4xx_hash_digest,
		.halg.digestsize	= SHA224_DIGEST_SIZE,
		.halg.base = {
			.cra_name 		= "sha224",
			.cra_driver_name 	= "ppc4xx-sha224",
			.cra_priority		= CRYPTO4XX_CRYPTO_PRIORITY,
			.cra_flags 		= CRYPTO_ALG_TYPE_AHASH | CRYPTO_ALG_ASYNC,
			.cra_blocksize 		= SHA224_BLOCK_SIZE,
			.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
			.cra_alignmask 		= 0,
			.cra_type 		= &crypto_ahash_type,
			.cra_init 		= crypto4xx_sha2_alg_init,
			.cra_exit		= crypto4xx_alg_exit,
			.cra_module 		= THIS_MODULE,
		}
	}},

        { .type = CRYPTO_ALG_TYPE_AHASH, .u.hash = {
		.init   	= crypto4xx_hash_init,
		.update		= crypto4xx_hash_update,
		.final 		= crypto4xx_hash_final,
		.digest		= crypto4xx_hash_digest,
		.setkey		= crypto4xx_sha2_hmac_setkey,
		.halg.digestsize 	= SHA224_DIGEST_SIZE,
		.halg.base = {
			.cra_name 		= "hmac(sha224)",
			.cra_driver_name 	= "ppc4xx-hmac-sha224",
			.cra_priority		= CRYPTO4XX_CRYPTO_PRIORITY,
			.cra_flags 		= CRYPTO_ALG_TYPE_AHASH | CRYPTO_ALG_ASYNC,
			.cra_blocksize 		= SHA224_BLOCK_SIZE,
			.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
			.cra_alignmask 		= 0,
			.cra_type 		= &crypto_ahash_type,
			.cra_module 		= THIS_MODULE,
		}
	}},

        { .type = CRYPTO_ALG_TYPE_AHASH, .u.hash = {
		.init		= crypto4xx_hash_init,
		.update		= crypto4xx_hash_update,
		.final 		= crypto4xx_hash_final,
		.digest		= crypto4xx_hash_digest,
		.halg.digestsize	= SHA256_DIGEST_SIZE,
		.halg.base = {
			.cra_name 		= "sha256",
			.cra_driver_name 	= "ppc4xx-sha256",
			.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY,
			.cra_flags 		= CRYPTO_ALG_TYPE_AHASH | CRYPTO_ALG_ASYNC,
			.cra_blocksize 		= SHA256_BLOCK_SIZE,
			.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
			.cra_alignmask 		= 0,
			.cra_type 		= &crypto_ahash_type,
			.cra_init 		= crypto4xx_sha2_alg_init,
			.cra_exit		= crypto4xx_alg_exit,
			.cra_module 		= THIS_MODULE,
		}
	}},

        { .type = CRYPTO_ALG_TYPE_AHASH, .u.hash = {
		.init		= crypto4xx_hash_init,
		.update		= crypto4xx_hash_update,
		.final 		= crypto4xx_hash_final,
		.digest		= crypto4xx_hash_digest,
		.setkey		= crypto4xx_sha2_hmac_setkey,
		.halg.digestsize	= SHA256_DIGEST_SIZE,
		.halg.base = {
			.cra_name 		= "hmac(sha256)",
			.cra_driver_name 	= "ppc4xx-hmac-sha256",
			.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY,
			.cra_flags 		= CRYPTO_ALG_TYPE_AHASH | CRYPTO_ALG_ASYNC,
			.cra_blocksize 		= SHA256_BLOCK_SIZE,
			.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
			.cra_alignmask 		= 0,
			.cra_type 		= &crypto_ahash_type,
			.cra_module 		= THIS_MODULE,
		}
	}},

        { .type = CRYPTO_ALG_TYPE_AHASH, .u.hash = {
		.init		= crypto4xx_hash_init,
		.update		= crypto4xx_hash_update,
		.final 		= crypto4xx_hash_final,
		.digest		= crypto4xx_hash_digest,
		.halg.digestsize	= SHA384_DIGEST_SIZE,
		.halg.base = {
			.cra_name 		= "sha384",
			.cra_driver_name 	= "ppc4xx-sha384",
			.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY,
			.cra_flags 		= CRYPTO_ALG_TYPE_AHASH | CRYPTO_ALG_ASYNC,
			.cra_blocksize 		= SHA384_BLOCK_SIZE,
			.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
			.cra_alignmask 		= 0,
			.cra_type 		= &crypto_ahash_type,
			.cra_init 		= crypto4xx_sha2_alg_init,
			.cra_exit		= crypto4xx_alg_exit,
			.cra_module 		= THIS_MODULE,
		}
	}},

        { .type = CRYPTO_ALG_TYPE_AHASH, .u.hash = {
		.init		= crypto4xx_hash_init,
		.update 	= crypto4xx_hash_update,
		.final 		= crypto4xx_hash_final,
		.digest		= crypto4xx_hash_digest,
		.setkey		= crypto4xx_sha2_hmac_setkey,
		.halg.digestsize	= SHA384_DIGEST_SIZE,
		.halg.base = {
			.cra_name 		= "hmac(sha384)",
			.cra_driver_name 	= "ppc4xx-hmac-sha384",
			.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY,
			.cra_flags 		= CRYPTO_ALG_TYPE_AHASH | CRYPTO_ALG_ASYNC,
			.cra_blocksize 		= SHA384_BLOCK_SIZE,
			.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
			.cra_alignmask 		= 0,
			.cra_type 		= &crypto_ahash_type,
			.cra_module 		= THIS_MODULE,
		}
	}},

        { .type = CRYPTO_ALG_TYPE_AHASH, .u.hash = {
		.init		= crypto4xx_hash_init,
		.update		= crypto4xx_hash_update,
		.final 		= crypto4xx_hash_final,
		.digest		= crypto4xx_hash_digest,
		.halg.digestsize 	= SHA512_DIGEST_SIZE,
		.halg.base = {
			.cra_name 		= "sha512",
			.cra_driver_name 	= "ppc4xx-sha512",
			.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY,
			.cra_flags 		= CRYPTO_ALG_TYPE_AHASH | CRYPTO_ALG_ASYNC,
			.cra_blocksize 		= SHA512_BLOCK_SIZE,
			.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
			.cra_alignmask 		= 0,
			.cra_type 		= &crypto_ahash_type,
			.cra_init 		= crypto4xx_sha2_alg_init,
			.cra_exit		= crypto4xx_alg_exit,
			.cra_module 		= THIS_MODULE,
		}
	}},
#endif
#if 1
        { .type = CRYPTO_ALG_TYPE_AHASH, .u.hash = {
		.init		= crypto4xx_hash_init,
		.update		= crypto4xx_hash_update,
		.final		= crypto4xx_hash_final,
		.digest 	= crypto4xx_hash_digest,
		.setkey 	= crypto4xx_sha2_hmac_setkey,
		.halg.digestsize 	= SHA512_DIGEST_SIZE,
		.halg.base = {
			.cra_name 		= "hmac(sha512)",
			.cra_driver_name 	= "ppc4xx-hmac-sha512",
			.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY,
			.cra_flags 		= CRYPTO_ALG_TYPE_AHASH | CRYPTO_ALG_ASYNC,
			.cra_blocksize 		= SHA512_BLOCK_SIZE,
			.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
			.cra_alignmask 		= 0,
			.cra_type 		= &crypto_ahash_type,
			.cra_module 		= THIS_MODULE,
		}
	}},
	/* Hash XCBC, GHASH, and Kasumi F9 */
	{ .type = CRYPTO_ALG_TYPE_AHASH, .u.hash = {
		.init   	= crypto4xx_hash_init,
		.update 	= crypto4xx_hash_update,
		.final  	= crypto4xx_hash_final,
		.digest 	= crypto4xx_hash_digest,
		.setkey 	= crypto4xx_xcbc_setkey,
		.halg.digestsize 	= 16,
		.halg.base = {
			.cra_name 		= "xcbc(aes)",
			.cra_driver_name	= "ppc4xx-xcbc-aes",
			.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY,
			.cra_flags 		= CRYPTO_ALG_TYPE_AHASH | CRYPTO_ALG_ASYNC,
			.cra_blocksize 		= AES_BLOCK_SIZE,
			.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
			.cra_alignmask 		= 0,
			.cra_type 		= &crypto_ahash_type,
			.cra_module 		= THIS_MODULE,
		}
	}},

	/* Crypto Kasumi and Kasumi F8 */
        { .type = CRYPTO_ALG_TYPE_ABLKCIPHER, .u.cipher = {
		.cra_name 		= "kasumi",
		.cra_driver_name 	= "ppc4xx-kasumi",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY,
		.cra_flags 		= CRYPTO_ALG_TYPE_ABLKCIPHER | CRYPTO_ALG_ASYNC,
		.cra_blocksize		= KASUMI_BLOCK_SIZE,
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_ablkcipher_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
		.cra_module 		= THIS_MODULE,
		.cra_u 			= {
			.ablkcipher = {
				.min_keysize 	= KASUMI_KEY_SIZE,
				.max_keysize 	= KASUMI_KEY_SIZE,
				.ivsize 	= KASUMI_BLOCK_SIZE,
				.setkey 	= crypto4xx_setkey_kasumi_p,
				.encrypt	= crypto4xx_encrypt,
				.decrypt	= crypto4xx_decrypt,
			}
		}
	}},
        { .type = CRYPTO_ALG_TYPE_ABLKCIPHER, .u.cipher = {
		.cra_name 		= "f8(kasumi)",
		.cra_driver_name 	= "ppc4xx-f8-kasumi",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY,
		.cra_flags 		= CRYPTO_ALG_TYPE_ABLKCIPHER | CRYPTO_ALG_ASYNC,
		.cra_blocksize		= KASUMI_BLOCK_SIZE,
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_ablkcipher_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
		.cra_module 		= THIS_MODULE,
		.cra_u 			= {
			.ablkcipher = {
				.min_keysize	= KASUMI_KEY_SIZE,
				.max_keysize	= KASUMI_KEY_SIZE,
				.ivsize		= KASUMI_BLOCK_SIZE,
				.setkey		= crypto4xx_setkey_kasumi_f8,
				.encrypt	= crypto4xx_encrypt_kasumi_f8,
				.decrypt	= crypto4xx_decrypt_kasumi_f8,
			}
		}
	}},
        { .type = CRYPTO_ALG_TYPE_AHASH, .u.hash = {
		.init   	= crypto4xx_hash_init,
		.update 	= crypto4xx_hash_update,
		.final  	= crypto4xx_hash_final,
		.digest 	= crypto4xx_kasumi_f9_digest,
		.setkey 	= crypto4xx_kasumi_f9_setkey,
		.halg.digestsize 	= 8,
		.halg.base = {
			.cra_name 		= "f9(kasumi)",
			.cra_driver_name 	= "ppc4xx-f9-kasumi",
			.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY,
			.cra_flags 		= CRYPTO_ALG_TYPE_AHASH | CRYPTO_ALG_ASYNC,
			.cra_blocksize		= KASUMI_BLOCK_SIZE,
			.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
			.cra_alignmask		= 0,
			.cra_type 		= &crypto_ahash_type,
			.cra_module 		= THIS_MODULE,
		}
	}},
#endif
#if 1
	/* Crypto ARC4 - stateless */
        { .type = CRYPTO_ALG_TYPE_ABLKCIPHER, .u.cipher = {
		.cra_name 		= "ecb(arc4)",
		.cra_driver_name 	= "ppc4xx-arc4",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY,
		.cra_flags 		= CRYPTO_ALG_TYPE_ABLKCIPHER | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 1,
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_ablkcipher_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
		.cra_module 		= THIS_MODULE,
		.cra_u 			= {
			.ablkcipher = {
				.min_keysize 	= 1,
				.max_keysize 	= 16,
				.setkey 	= crypto4xx_setkey_arc4,
				.encrypt	= crypto4xx_arc4_encrypt,
				.decrypt	= crypto4xx_arc4_decrypt,
			}
		}
	}},
	/* Crypto ARC4 - statefull */
        { .type = CRYPTO_ALG_TYPE_ABLKCIPHER, .u.cipher = {
		.cra_name 		= "cbc(arc4)",
		.cra_driver_name 	= "ppc4xx-arc4",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY,
		.cra_flags 		= CRYPTO_ALG_TYPE_BLKCIPHER | CRYPTO_ALG_ASYNC,
		.cra_blocksize		= 1,
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type		= &crypto_ablkcipher_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
		.cra_module 		= THIS_MODULE,
		.cra_u 			= {
			.ablkcipher = {
				.min_keysize 	= 1,
				.max_keysize	= 16,
				.setkey		= crypto4xx_setkey_arc4,
				.encrypt 	= crypto4xx_arc4_encrypt,
				.decrypt	= crypto4xx_arc4_decrypt,
			}
		}
	}},
#endif
#if 1
	/* IPSec combined hash and crypto Algorithms */
        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "tunnel(esp(authenc(hmac(md5),cbc(aes))))",
		.cra_driver_name 	= "tunnel-esp-cbc-aes-md5",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY_IPSEC,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 16,	/* 128-bits block */
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
		.cra_u 			={
			.aead = {
				.ivsize 		= 16,	/* IV size is 16 bytes */
				.maxauthsize		= 16,	/* Max auth data size in bytes */
				.setkey 		= crypto4xx_setkey_tunnel_esp_cbc_aes_md5,
				.setauthsize		= NULL,
				.encrypt 		= crypto4xx_encrypt_esp_cbc,
				.decrypt 		= crypto4xx_decrypt_esp_cbc,
				.givencrypt		= crypto4xx_givencrypt_esp_cbc,
			}
		}
	}},
	/* IPSec combined hash and crypto Algorithms */
        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "tunnel(esp(authenc(hmac(sha1),cbc(aes))))",
		.cra_driver_name 	= "tunnel-esp-cbc-aes-sha1",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY_IPSEC,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 16,	/* 128-bits block */
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
		.cra_u 			= {
			.aead = {
				.ivsize 		= 16,	/* IV size is 16 bytes */
				.maxauthsize		= 20,	/* Max auth data size in bytes */
				.setkey 		= crypto4xx_setkey_tunnel_esp_cbc_aes_sha1,
				.setauthsize		= NULL,
				.encrypt 		= crypto4xx_encrypt_esp_cbc,
				.decrypt 		= crypto4xx_decrypt_esp_cbc,
				.givencrypt		= crypto4xx_givencrypt_esp_cbc,
			}
		}
	}},
#endif
#if 1
	/* IPSec combined hash and crypto Algorithms */
        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "tunnel(esp(authenc(hmac(sha224),cbc(aes))))",
		.cra_driver_name 	= "tunnel-esp-cbc-aes-sha224",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY_IPSEC,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 16,	/* 128-bits block */
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
		.cra_u 			= {
			.aead = {
				.ivsize 		= 16,	/* IV size is 16 bytes */
				.maxauthsize		= 28,	/* Max auth data size in bytes */
				.setkey 		= crypto4xx_setkey_tunnel_esp_cbc_aes_sha224,
				.setauthsize		= NULL,
				.encrypt 		= crypto4xx_encrypt_esp_cbc,
				.decrypt 		= crypto4xx_decrypt_esp_cbc,
				.givencrypt		= crypto4xx_givencrypt_esp_cbc,
			}
		}
	}},
#endif
#if 1
	/* IPSec combined hash and crypto Algorithms */
        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "tunnel(esp(authenc(hmac(sha256),cbc(aes))))",
		.cra_driver_name 	= "tunnel-esp-cbc-aes-sha256",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY_IPSEC,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 16,	/* 128-bits block */
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
		.cra_u 			= {
			.aead = {
				.ivsize 		= 16,	/* IV size is 16 bytes */
				.maxauthsize		= 32,	/* Max auth data size in bytes */
				.setkey 		= crypto4xx_setkey_tunnel_esp_cbc_aes_sha256,
				.setauthsize		= NULL,
				.encrypt 		= crypto4xx_encrypt_esp_cbc,
				.decrypt 		= crypto4xx_decrypt_esp_cbc,
				.givencrypt		= crypto4xx_givencrypt_esp_cbc,
			}
		}
	}},

        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "tunnel(esp(authenc(hmac(sha384),cbc(aes))))",
		.cra_driver_name 	= "tunnel-esp-cbc-aes-sha384",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY_IPSEC,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 16,	/* 128-bits block */
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
		.cra_u 			= {
			.aead = {
				.ivsize 		= 16,	/* IV size is 16 bytes */
				.maxauthsize		= 48,	/* Max auth data size in bytes */
				.setkey 		= crypto4xx_setkey_tunnel_esp_cbc_aes_sha384,
				.setauthsize		= NULL,
				.encrypt 		= crypto4xx_encrypt_esp_cbc,
				.decrypt 		= crypto4xx_decrypt_esp_cbc,
				.givencrypt		= crypto4xx_givencrypt_esp_cbc,
			}
		}
	}},
        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "tunnel(esp(authenc(hmac(sha512),cbc(aes))))",
		.cra_driver_name 	= "tunnel-esp-cbc-aes-sha512",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY_IPSEC,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 16,	/* 128-bits block */
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
		.cra_u 			= {
			.aead = {
				.ivsize 		= 16,	/* IV size is 16 bytes */
				.maxauthsize		= 64,	/* Max auth data size in bytes */
				.setkey 		= crypto4xx_setkey_tunnel_esp_cbc_aes_sha512,
				.setauthsize		= NULL,
				.encrypt 		= crypto4xx_encrypt_esp_cbc,
				.decrypt 		= crypto4xx_decrypt_esp_cbc,
				.givencrypt		= crypto4xx_givencrypt_esp_cbc,
			}
		}
	}},
	/* IPSec combined hash and crypto Algorithms */
        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "tunnel(esp(authenc(hmac(md5),cbc(des))))",
		.cra_driver_name 	= "tunnel-esp-cbc-des-md5",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY_IPSEC,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 8,	/* 128-bits block */
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
		.cra_u 			= {
			.aead = {
				.ivsize 		= 8,	/* IV size is 16 bytes */
				.maxauthsize		= 16,	/* Max auth data size in bytes */
				.setkey 		= crypto4xx_setkey_tunnel_esp_cbc_des_md5,
				.setauthsize		= NULL,
				.encrypt 		= crypto4xx_encrypt_esp_cbc,
				.decrypt 		= crypto4xx_decrypt_esp_cbc,
				.givencrypt		= crypto4xx_givencrypt_esp_cbc,
			}
		}
	}},
	/* IPSec combined hash and crypto Algorithms */
        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "tunnel(esp(authenc(hmac(sha1),cbc(des))))",
		.cra_driver_name 	= "tunnel-esp-cbc-des-sha1",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY_IPSEC,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 8,	/* 128-bits block */
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
		.cra_u 			= {
			.aead = {
				.ivsize 		= 8,	/* IV size is 16 bytes */
				.maxauthsize		= 20,	/* Max auth data size in bytes */
				.setkey 		= crypto4xx_setkey_tunnel_esp_cbc_des_sha1,
				.setauthsize		= NULL,
				.encrypt 		= crypto4xx_encrypt_esp_cbc,
				.decrypt 		= crypto4xx_decrypt_esp_cbc,
				.givencrypt		= crypto4xx_givencrypt_esp_cbc,
			}
		}
	}},
	/* IPSec combined hash and crypto Algorithms */
        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "tunnel(esp(authenc(hmac(sha224),cbc(des))))",
		.cra_driver_name 	= "tunnel-esp-cbc-des-sha224",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY_IPSEC,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 8,	/* 128-bits block */
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
		.cra_u 			= {
			.aead = {
			.ivsize 		= 8,	/* IV size is 16 bytes */
			.maxauthsize		= 28,	/* Max auth data size in bytes */
			.setkey 		= crypto4xx_setkey_tunnel_esp_cbc_des_sha224,
			.setauthsize		= NULL,
			.encrypt 		= crypto4xx_encrypt_esp_cbc,
			.decrypt 		= crypto4xx_decrypt_esp_cbc,
			.givencrypt		= crypto4xx_givencrypt_esp_cbc,
			}
		}
	}},
	/* IPSec combined hash and crypto Algorithms */
        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "tunnel(esp(authenc(hmac(sha256),cbc(des))))",
		.cra_driver_name 	= "tunnel-esp-cbc-des-sha256",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY_IPSEC,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 8,	/* 128-bits block */
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
		.cra_u 			= {
			.aead = {
				.ivsize 		= 8,	/* IV size is 16 bytes */
				.maxauthsize		= 32,	/* Max auth data size in bytes */
				.setkey 		= crypto4xx_setkey_tunnel_esp_cbc_des_sha256,
				.setauthsize		= NULL,
				.encrypt 		= crypto4xx_encrypt_esp_cbc,
				.decrypt 		= crypto4xx_decrypt_esp_cbc,
				.givencrypt		= crypto4xx_givencrypt_esp_cbc,
			}
		}
	}},
	/* IPSec combined hash and crypto Algorithms */
        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "tunnel(esp(authenc(hmac(sha384),cbc(des))))",
		.cra_driver_name 	= "tunnel-esp-cbc-des-sha384",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY_IPSEC,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 8,	/* 128-bits block */
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
		.cra_u 			= {
			.aead = {
				.ivsize 		= 8,	/* IV size is 16 bytes */
				.maxauthsize		= 48,	/* Max auth data size in bytes */
				.setkey 		= crypto4xx_setkey_tunnel_esp_cbc_des_sha384,
				.setauthsize		= NULL,
				.encrypt 		= crypto4xx_encrypt_esp_cbc,
				.decrypt 		= crypto4xx_decrypt_esp_cbc,
				.givencrypt		= crypto4xx_givencrypt_esp_cbc,
			}
		}
	}},
	/* IPSec combined hash and crypto Algorithms */
        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "tunnel(esp(authenc(hmac(sha512),cbc(des))))",
		.cra_driver_name 	= "tunnel-esp-cbc-des-sha512",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY_IPSEC,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 8,	/* 128-bits block */
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
		.cra_u 			= {
			.aead = {
				.ivsize 		= 8,	/* IV size is 16 bytes */
				.maxauthsize		= 64,	/* Max auth data size in bytes */
				.setkey 		= crypto4xx_setkey_tunnel_esp_cbc_des_sha512,
				.setauthsize		= NULL,
				.encrypt 		= crypto4xx_encrypt_esp_cbc,
				.decrypt 		= crypto4xx_decrypt_esp_cbc,
				.givencrypt		= crypto4xx_givencrypt_esp_cbc,
			}
		}
	}},
        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "tunnel(esp(authenc(hmac(md5),cbc(des3_ede))))",
		.cra_driver_name 	= "tunnel-esp-cbc-3des-md5",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY_IPSEC,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 8,	/* 128-bits block */
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
		.cra_u 			= {
			.aead = {
				.ivsize 		= 8,	/* IV size is 16 bytes */
				.maxauthsize		= 16,	/* Max auth data size in bytes */
				.setkey 		= crypto4xx_setkey_tunnel_esp_cbc_3des_md5,
				.setauthsize		= NULL,
				.encrypt 		= crypto4xx_encrypt_esp_cbc,
				.decrypt 		= crypto4xx_decrypt_esp_cbc,
				.givencrypt		= crypto4xx_givencrypt_esp_cbc,
			}
		}
	}},
	/* IPSec combined hash and crypto Algorithms */
        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "tunnel(esp(authenc(hmac(md5),cbc(des3_ede))))",
		.cra_driver_name 	= "tunnel-esp-cbc-3des-md5",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY_IPSEC,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 8,	/* 128-bits block */
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
		.cra_u 			= {
			.aead = {
				.ivsize 		= 8,	/* IV size is 16 bytes */
				.maxauthsize		= 16,	/* Max auth data size in bytes */
				.setkey 		= crypto4xx_setkey_tunnel_esp_cbc_3des_md5,
				.setauthsize		= NULL,
				.encrypt 		= crypto4xx_encrypt_esp_cbc,
				.decrypt 		= crypto4xx_decrypt_esp_cbc,
				.givencrypt		= crypto4xx_givencrypt_esp_cbc,
			}
		}
	}},
	/* IPSec combined hash and crypto Algorithms */
        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "tunnel(esp(authenc(hmac(sha1),cbc(des3_ede))))",
		.cra_driver_name 	= "tunnel-esp-cbc-3des-sha1",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY_IPSEC,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 8,	/* 128-bits block */
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
		.cra_u 			= {
		.aead = {
				.ivsize 		= 8,	/* IV size is 16 bytes */
				.maxauthsize		= 20,	/* Max auth data size in bytes */
				.setkey 		= crypto4xx_setkey_tunnel_esp_cbc_3des_sha1,
				.setauthsize		= NULL,
				.encrypt 		= crypto4xx_encrypt_esp_cbc,
				.decrypt 		= crypto4xx_decrypt_esp_cbc,
				.givencrypt		= crypto4xx_givencrypt_esp_cbc,
			}
		}
	}},
	/* IPSec combined hash and crypto Algorithms */
        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "tunnel(esp(authenc(hmac(sha224),cbc(des3_ede))))",
		.cra_driver_name 	= "tunnel-esp-cbc-3des-sha224",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY_IPSEC,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 8,	/* 128-bits block */
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
		.cra_u 			= {
			.aead = {
				.ivsize 		= 8,	/* IV size is 16 bytes */
				.maxauthsize		= 28,	/* Max auth data size in bytes */
				.setkey 		= crypto4xx_setkey_tunnel_esp_cbc_3des_sha224,
				.setauthsize		= NULL,
				.encrypt 		= crypto4xx_encrypt_esp_cbc,
				.decrypt 		= crypto4xx_decrypt_esp_cbc,
				.givencrypt		= crypto4xx_givencrypt_esp_cbc,
			}
		}
	}},
	/* IPSec combined hash and crypto Algorithms */
        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "tunnel(esp(authenc(hmac(sha256),cbc(des3_ede))))",
		.cra_driver_name 	= "tunnel-esp-cbc-3des-sha256",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY_IPSEC,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 8,
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
		.cra_u 			= {
			.aead = {
				.ivsize 		= 8,	/* IV size is 16 bytes */
				.maxauthsize		= 32,	/* Max auth data size in bytes */
				.setkey 		= crypto4xx_setkey_tunnel_esp_cbc_3des_sha256,
				.setauthsize		= NULL,
				.encrypt 		= crypto4xx_encrypt_esp_cbc,
				.decrypt 		= crypto4xx_decrypt_esp_cbc,
				.givencrypt		= crypto4xx_givencrypt_esp_cbc,
			}
		}
	}},
        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "tunnel(esp(authenc(hmac(sha384),cbc(des3_ede))))",
		.cra_driver_name 	= "tunnel-esp-cbc-3des-sha384",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY_IPSEC,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 8,
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
		.cra_u 			= {
			.aead = {
				.ivsize 		= 8,	/* IV size is 16 bytes */
				.maxauthsize		= 48,	/* Max auth data size in bytes */
				.setkey 		= crypto4xx_setkey_tunnel_esp_cbc_3des_sha384,
				.setauthsize		= NULL,
				.encrypt 		= crypto4xx_encrypt_esp_cbc,
				.decrypt 		= crypto4xx_decrypt_esp_cbc,
				.givencrypt		= crypto4xx_givencrypt_esp_cbc,
			}
		}
	}},
	/* IPSec combined hash and crypto Algorithms */
        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "tunnel(esp(authenc(hmac(sha512),cbc(des3_ede))))",
		.cra_driver_name 	= "tunnel-esp-cbc-3des-sha512",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY_IPSEC,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 8,
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
		.cra_u 			= {
			.aead = {
				.ivsize 		= 8,	/* IV size is 8 bytes */
				.maxauthsize		= 64,	/* Max auth data size in bytes */
				.setkey 		= crypto4xx_setkey_tunnel_esp_cbc_3des_sha512,
				.setauthsize		= NULL,
				.encrypt 		= crypto4xx_encrypt_esp_cbc,
				.decrypt 		= crypto4xx_decrypt_esp_cbc,
				.givencrypt		= crypto4xx_givencrypt_esp_cbc,
			}
		}
	}},
	
	/** IPSec transport combined hash and crypto Algorithms */
        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "transport(esp(authenc(hmac(md5),cbc(aes))))",
		.cra_driver_name 	= "transport-esp-cbc-aes-md5",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY_IPSEC,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 16,	/* 128-bits block */
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
		.cra_u 			= {
			.aead = {
				.ivsize 		= 16,	/* IV size is 16 bytes */
				.maxauthsize		= 16,	/* Max auth data size in bytes */
				.setkey 		= crypto4xx_setkey_transport_esp_cbc_aes_md5,
				.setauthsize		= NULL,
				.encrypt 		= crypto4xx_encrypt_transport_esp_cbc,
				.decrypt 		= crypto4xx_decrypt_transport_esp_cbc,
				.givencrypt		= crypto4xx_givencrypt_transport_esp_cbc,
			}
		}
	}},


	/* IPSec combined hash and crypto Algorithms */
        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "transport(esp(authenc(hmac(sha1),cbc(aes))))",
		.cra_driver_name 	= "transport-esp-cbc-aes-sha1",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY_IPSEC,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 16,	/* 128-bits block */
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
		.cra_u 			= {
			.aead = {
				.ivsize 		= 16,	/* IV size is 16 bytes */
				.maxauthsize		= 20,	/* Max auth data size in bytes */
				.setkey 		= crypto4xx_setkey_transport_esp_cbc_aes_sha1,
				.setauthsize		= NULL,
				.encrypt 		= crypto4xx_encrypt_transport_esp_cbc,
				.decrypt 		= crypto4xx_decrypt_transport_esp_cbc,
				.givencrypt		= crypto4xx_givencrypt_transport_esp_cbc,
			}
		}
	}},

	/* IPSec combined hash and crypto Algorithms */
        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "transport(esp(authenc(hmac(sha224),cbc(aes))))",
		.cra_driver_name 	= "transport-esp-cbc-aes-sha224",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY_IPSEC,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 16,	/* 128-bits block */
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
		.cra_u 			= {
			.aead = {
				.ivsize 		= 16,	/* IV size is 16 bytes */
				.maxauthsize		= 28,	/* Max auth data size in bytes */
				.setkey 		= crypto4xx_setkey_transport_esp_cbc_aes_sha224,
				.setauthsize		= NULL,
				.encrypt 		= crypto4xx_encrypt_transport_esp_cbc,
				.decrypt 		= crypto4xx_decrypt_transport_esp_cbc,
				.givencrypt		= crypto4xx_givencrypt_transport_esp_cbc,
			}
		}
	}},

	/* IPSec combined hash and crypto Algorithms */
        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "transport(esp(authenc(hmac(sha256),cbc(aes))))",
		.cra_driver_name 	= "transport-esp-cbc-aes-sha224",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY_IPSEC,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 16,	/* 128-bits block */
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
		.cra_u 			= {
			.aead = {
				.ivsize 		= 16,	/* IV size is 16 bytes */
				.maxauthsize		= 32,	/* Max auth data size in bytes */
				.setkey 		= crypto4xx_setkey_transport_esp_cbc_aes_sha256,
				.setauthsize		= NULL,
				.encrypt 		= crypto4xx_encrypt_transport_esp_cbc,
				.decrypt 		= crypto4xx_decrypt_transport_esp_cbc,
				.givencrypt		= crypto4xx_givencrypt_transport_esp_cbc,
			}
		}
	}},

	/* IPSec combined hash and crypto Algorithms */
        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "transport(esp(authenc(hmac(sha384),cbc(aes))))",
		.cra_driver_name 	= "transport-esp-cbc-aes-sha384",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY_IPSEC,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 16,	/* 128-bits block */
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit 		= crypto4xx_alg_exit,
		.cra_u 			= {
			.aead = {
				.ivsize 		= 16,	/* IV size is 16 bytes */
				.maxauthsize		= 48,	/* Max auth data size in bytes */
				.setkey 		= crypto4xx_setkey_transport_esp_cbc_aes_sha384,
				.setauthsize		= NULL,
				.encrypt 		= crypto4xx_encrypt_transport_esp_cbc,
				.decrypt 		= crypto4xx_decrypt_transport_esp_cbc,
				.givencrypt		= crypto4xx_givencrypt_transport_esp_cbc,
			}
		}
	}},


        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "transport(esp(authenc(hmac(sha512),cbc(aes))))",
		.cra_driver_name 	= "transport-esp-cbc-aes-sha512",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY_IPSEC,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 16,	/* 128-bits block */
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit 		= crypto4xx_alg_exit,
		.cra_u 			= {
			.aead = {
				.ivsize 		= 16,	/* IV size is 16 bytes */
				.maxauthsize		= 64,	/* Max auth data size in bytes */
				.setkey 		= crypto4xx_setkey_transport_esp_cbc_aes_sha512,
				.setauthsize		= NULL,
				.encrypt 		= crypto4xx_encrypt_transport_esp_cbc,
				.decrypt 		= crypto4xx_decrypt_transport_esp_cbc,
				.givencrypt		= crypto4xx_givencrypt_transport_esp_cbc,
			}
		}
	}},

	/* IPSec transport combined hash and crypto Algorithms */
        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "transport(esp(authenc(hmac(md5),cbc(des))))",
		.cra_driver_name 	= "transport-esp-cbc-des-md5",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY_IPSEC,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 8,	/* 128-bits block */
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit 		= crypto4xx_alg_exit,
		.cra_u 			= {
			.aead = {
				.ivsize 		= 8,	/* IV size is 16 bytes */
				.maxauthsize		= 16,	/* Max auth data size in bytes */
				.setkey 		= crypto4xx_setkey_transport_esp_cbc_des_md5,
				.setauthsize		= NULL,
				.encrypt 		= crypto4xx_encrypt_transport_esp_cbc,
				.decrypt 		= crypto4xx_decrypt_transport_esp_cbc,
				.givencrypt		= crypto4xx_givencrypt_transport_esp_cbc,
			}
		}
	}},


	/* IPSec combined hash and crypto Algorithms */
        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "transport(esp(authenc(hmac(sha1),cbc(des))))",
		.cra_driver_name 	= "transport-esp-cbc-des-sha1",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY_IPSEC,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 8,
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 	= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit 		= crypto4xx_alg_exit,
		.cra_u 			= {
			.aead = {

				.ivsize 		= 8,	/* IV size is 16 bytes */
				.maxauthsize		= 20,	/* Max auth data size in bytes */
				.setkey 		= crypto4xx_setkey_transport_esp_cbc_des_sha1,
				.setauthsize		= NULL,
				.encrypt 		= crypto4xx_encrypt_transport_esp_cbc,
				.decrypt 		= crypto4xx_decrypt_transport_esp_cbc,
				.givencrypt		= crypto4xx_givencrypt_transport_esp_cbc,
			}
		}
	}},

	/* IPSec combined hash and crypto Algorithms */
        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "transport(esp(authenc(hmac(sha224),cbc(des))))",
		.cra_driver_name 	= "transport-esp-cbc-des-sha224",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY_IPSEC,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 8,	/* 128-bits block */
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit 		= crypto4xx_alg_exit,
		.cra_u 			= {
			.aead = {
				.ivsize 		= 8,	/* IV size is 16 bytes */
				.maxauthsize		= 28,	/* Max auth data size in bytes */
				.setkey 		= crypto4xx_setkey_transport_esp_cbc_des_sha224,
				.setauthsize		= NULL,
				.encrypt 		= crypto4xx_encrypt_transport_esp_cbc,
				.decrypt 		= crypto4xx_decrypt_transport_esp_cbc,
				.givencrypt		= crypto4xx_givencrypt_transport_esp_cbc,
			}
		}
	}},

	/* IPSec combined hash and crypto Algorithms */
        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "transport(esp(authenc(hmac(sha256),cbc(des))))",
		.cra_driver_name 	= "transport-esp-cbc-des-sha256",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY_IPSEC,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 8,	/* 128-bits block */
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit 		= crypto4xx_alg_exit,
		.cra_u 			= {
			.aead = {
				.ivsize 		= 8,	/* IV size is 16 bytes */
				.maxauthsize		= 32,	/* Max auth data size in bytes */
				.setkey 		= crypto4xx_setkey_transport_esp_cbc_des_sha256,
				.setauthsize		= NULL,
				.encrypt 		= crypto4xx_encrypt_transport_esp_cbc,
				.decrypt 		= crypto4xx_decrypt_transport_esp_cbc,
				.givencrypt		= crypto4xx_givencrypt_transport_esp_cbc,
			}
		}
	}},


        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "transport(esp(authenc(hmac(sha384),cbc(des))))",
		.cra_driver_name 	= "transport-esp-cbc-des-sha384",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY_IPSEC,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 8,	/* 128-bits block */
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit 		= crypto4xx_alg_exit,
		.cra_u 			= {
			.aead = {
				.ivsize 		= 8,	/* IV size is 16 bytes */
				.maxauthsize		= 48,	/* Max auth data size in bytes */
				.setkey 		= crypto4xx_setkey_transport_esp_cbc_des_sha384,
				.setauthsize		= NULL,
				.encrypt 		= crypto4xx_encrypt_transport_esp_cbc,
				.decrypt 		= crypto4xx_decrypt_transport_esp_cbc,
				.givencrypt		= crypto4xx_givencrypt_transport_esp_cbc,
			}
		}
	}},
        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "transport(esp(authenc(hmac(sha512),cbc(des))))",
		.cra_driver_name 	= "transport-esp-cbc-des-sha512",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY_IPSEC,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 8,	/* 128-bits block */
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit 		= crypto4xx_alg_exit,
		.cra_u 			= {
			.aead = {
				.ivsize 		= 8,	/* IV size is 16 bytes */
				.maxauthsize		= 64,	/* Max auth data size in bytes */
				.setkey 		= crypto4xx_setkey_transport_esp_cbc_des_sha512,
				.setauthsize		= NULL,
				.encrypt 		= crypto4xx_encrypt_transport_esp_cbc,
				.decrypt 		= crypto4xx_decrypt_transport_esp_cbc,
				.givencrypt		= crypto4xx_givencrypt_transport_esp_cbc,
			}
		}
	}},

	/* IPSec transport combined hash and crypto Algorithms */
        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "transport(esp(authenc(hmac(md5),cbc(des3_ede))))",
		.cra_driver_name 	= "transport-esp-cbc-3des-md5",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY_IPSEC,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 8,	/* 128-bits block */
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit 		= crypto4xx_alg_exit,
		.cra_u 			= {
			.aead = {
				.ivsize 		= 8,	/* IV size is 16 bytes */
				.maxauthsize		= 16,	/* Max auth data size in bytes */
				.setkey 		= crypto4xx_setkey_transport_esp_cbc_3des_md5,
				.setauthsize		= NULL,
				.encrypt 		= crypto4xx_encrypt_transport_esp_cbc,
				.decrypt 		= crypto4xx_decrypt_transport_esp_cbc,
				.givencrypt		= crypto4xx_givencrypt_transport_esp_cbc,
			}
		}
	}},


	/* IPSec combined hash and crypto Algorithms */
        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "transport(esp(authenc(hmac(sha1),cbc(des3_ede))))",
		.cra_driver_name 	= "transport-esp-cbc-3des-sha1",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY_IPSEC,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 8,	/* 128-bits block */
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit 		= crypto4xx_alg_exit,
		.cra_u 			= {
			.aead = {
				.ivsize 		= 8,	/* IV size is 16 bytes */
				.maxauthsize		= 20,	/* Max auth data size in bytes */
				.setkey 		= crypto4xx_setkey_transport_esp_cbc_3des_sha1,
				.setauthsize		= NULL,
				.encrypt 		= crypto4xx_encrypt_transport_esp_cbc,
				.decrypt 		= crypto4xx_decrypt_transport_esp_cbc,
				.givencrypt		= crypto4xx_givencrypt_transport_esp_cbc,
			}
		}
	}},

        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "transport(esp(authenc(hmac(sha224),cbc(des3_ede))))",
		.cra_driver_name 	= "transport-esp-cbc-3des-sha224",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY_IPSEC,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 8,	/* 128-bits block */
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit 		= crypto4xx_alg_exit,
		.cra_u 			= {
			.aead = {
				.ivsize 		= 8,	/* IV size is 16 bytes */
				.maxauthsize		= 28,	/* Max auth data size in bytes */
				.setkey 		= crypto4xx_setkey_transport_esp_cbc_3des_sha224,
				.setauthsize		= NULL,
				.encrypt 		= crypto4xx_encrypt_transport_esp_cbc,
				.decrypt 		= crypto4xx_decrypt_transport_esp_cbc,
				.givencrypt		= crypto4xx_givencrypt_transport_esp_cbc,
			}
		}
	}},
#endif
#if 1
	/* IPSec combined hash and crypto Algorithms */
        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "transport(esp(authenc(hmac(sha256),cbc(des3_ede))))",
		.cra_driver_name 	= "transport-esp-cbc-3des-sha256",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY_IPSEC,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 8,	/* 128-bits block */
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
		.cra_u 			= {
			.aead = {
				.ivsize 		= 8,	/* IV size is 16 bytes */
				.maxauthsize		= 32,	/* Max auth data size in bytes */
				.setkey 		= crypto4xx_setkey_transport_esp_cbc_3des_sha256,
				.setauthsize		= NULL,
				.encrypt 		= crypto4xx_encrypt_transport_esp_cbc,
				.decrypt 		= crypto4xx_decrypt_transport_esp_cbc,
				.givencrypt		= crypto4xx_givencrypt_transport_esp_cbc,
			}
		}
	}},

	/* IPSec combined hash and crypto Algorithms */
        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "transport(esp(authenc(hmac(sha384),cbc(des3_ede))))",
		.cra_driver_name 	= "transport-esp-cbc-3des-sha384",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY_IPSEC,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 8,	/* 128-bits block */
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
		.cra_u 			= {
			.aead = {
				.ivsize 		= 8,	/* IV size is 16 bytes */
				.maxauthsize		= 48,	/* Max auth data size in bytes */
				.setkey 		= crypto4xx_setkey_transport_esp_cbc_3des_sha384,
				.setauthsize		= NULL,
				.encrypt 		= crypto4xx_encrypt_transport_esp_cbc,
				.decrypt 		= crypto4xx_decrypt_transport_esp_cbc,
				.givencrypt		= crypto4xx_givencrypt_transport_esp_cbc,
			}
		}
	}},

        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "transport(esp(authenc(hmac(sha512),cbc(des3_ede))))",
		.cra_driver_name 	= "transport-esp-cbc-3des-sha512",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY_IPSEC,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 8,
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_u 			= {
			.aead = {
				.ivsize 		= 8,	/* IV size is 16 bytes */
				.maxauthsize		= 64,	/* Max auth data size in bytes */
				.setkey 		= crypto4xx_setkey_transport_esp_cbc_3des_sha512,
				.setauthsize		= NULL,
				.encrypt 		= crypto4xx_encrypt_transport_esp_cbc,
				.decrypt 		= crypto4xx_decrypt_transport_esp_cbc,
				.givencrypt		= crypto4xx_givencrypt_transport_esp_cbc,
			}
		}
	}},
	
	{ .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "transport(esp(rfc4106(gcm(aes))))",
		.cra_driver_name 	= "transport-esp-rfc4106-gcm-aes",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY_IPSEC,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 16,	/* 128-bits block */
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,	/* Hardware requires 16 bytes aligned */
		.cra_type 		= &crypto_aead_type,
		.cra_module 		= THIS_MODULE,
		.cra_u 			= {
				.aead = {
			.ivsize 		= 8,	/* IV size for crypto */
			.maxauthsize		= 12,	/* Max auth data size in bytes */
			.setkey 		= crypto4xx_setkey_transport_esp_rfc4106_gcm,
			.setauthsize		= crypto4xx_setauthsize_aes,
			.encrypt 		= crypto4xx_encrypt_transport_esp_cbc,
			.decrypt 		= crypto4xx_decrypt_transport_esp_cbc,
			.givencrypt		= crypto4xx_givencrypt_transport_esp_cbc,
			}
		}
	}},

	{ .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "rfc4106(gcm(aes))",
		.cra_driver_name 	= "transport-esp-rfc4104-gcm-aes",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY_IPSEC-100,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 	= 16,
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 	= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_exit 		= crypto4xx_alg_exit,
		.cra_u 		= {.aead = {
		.ivsize 		= 8,	/* IV size is 16 bytes */
		.setkey 		= crypto4xx_setkey_transport_esp_rfc4106_gcm,
		.setauthsize		= crypto4xx_setauthsize_aes,
		.encrypt 		= crypto4xx_encrypt_transport_esp_cbc,
		.decrypt 		= crypto4xx_decrypt_transport_esp_cbc,
		.givencrypt		= crypto4xx_givencrypt_transport_esp_cbc,
		}
	}}},
	
        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "macsec(gcm)",
		.cra_driver_name 	= "macsec-ppc4xx",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 64,
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
		.cra_module 		= THIS_MODULE,
		.cra_u 			= {
			.aead = {
				.ivsize 		= 16,	/* IV size is 16 bytes */
				.maxauthsize		= 128,	/* Max auth data size in bytes */
				.setkey 		= crypto4xx_setkey_macsec_gcm,
				.setauthsize		= NULL,
				.encrypt 		= crypto4xx_encrypt_macsec,
				.decrypt 		= crypto4xx_decrypt_macsec,
				.givencrypt		= NULL,
				.givdecrypt		= NULL,
			}
		}
	}},
#endif
#if 1
        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "dtls(aes-sha1)",
		.cra_driver_name 	= "dtls-ppc4xx",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 64,	/* 128-bits block */
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
		.cra_module 		= THIS_MODULE,
		.cra_u 			= {
			.aead = {
				.ivsize 		= 16,	/* IV size is 16 bytes */
				.maxauthsize		= 128,	/* Max auth data size in bytes */
				.setkey 		= crypto4xx_setkey_dtls_aes_sha1,
				.setauthsize		= NULL,
				.encrypt 		= crypto4xx_encrypt_dtls,
				.decrypt 		= crypto4xx_decrypt_dtls,
				.givencrypt		= NULL,
				.givdecrypt		= NULL,
			}
		}
	}},
#endif
#if 1
        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "dtls(des-sha1)",
		.cra_driver_name 	= "ppc4xx-dtls-des-sha1",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 64,	/* 128-bits block */
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
		.cra_module 		= THIS_MODULE,
		.cra_u 		= {
		 	.aead = {
				.ivsize 		= 8,	/* IV size is 16 bytes */
				.maxauthsize		= 128,	/* Max auth data size in bytes */
				.setkey 		= crypto4xx_setkey_dtls_des_sha1,
				.setauthsize		= NULL,
				.encrypt 		= crypto4xx_encrypt_dtls,
				.decrypt 		= crypto4xx_decrypt_dtls,
				.givencrypt		= NULL,
				.givdecrypt		= NULL,
			}
	 	}
	}},
#endif
#if 1
        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "dtls(des3-sha1)",
		.cra_driver_name 	= "ppc4xx-dtls-des3-sha1",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 64,	/* 128-bits block */
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
		.cra_module 		= THIS_MODULE,
		.cra_u 			= {
		 	.aead	= {
				.ivsize 		= 8,	/* IV size is 16 bytes */
				.maxauthsize		= 128,	/* Max auth data size in bytes */
				.setkey 		= crypto4xx_setkey_dtls_des3_sha1,
				.setauthsize		= NULL,
				.encrypt 		= crypto4xx_encrypt_dtls,
				.decrypt 		= crypto4xx_decrypt_dtls,
				.givencrypt		= NULL,
				.givdecrypt		= NULL,
			}
	 	}
	}},

        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "dtls(NULL-md5)",
		.cra_driver_name 	= "ppc4xx-dtls-null-md5",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 64,	/* 128-bits block */
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
		.cra_module 		= THIS_MODULE,
		.cra_u			= {
		 	.aead = {
				.ivsize 		= 0,	/* IV size is 16 bytes */
				.maxauthsize		= 128,	/* Max auth data size in bytes */
				.setkey 		= crypto4xx_setkey_dtls_null_md5,
				.setauthsize		= NULL,
				.encrypt 		= crypto4xx_encrypt_dtls,
				.decrypt 		= crypto4xx_decrypt_dtls,
				.givencrypt		= NULL,
				.givdecrypt		= NULL,
			 }
	 	}
	}},
        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "dtls(NULL-sha1)",
		.cra_driver_name 	= "ppc4xx-dtls-null-sha1",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 64,	/* 128-bits block */
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
		.cra_module 		= THIS_MODULE,
		.cra_u 		= {
		 	.aead = {
			.ivsize 		= 0,	/* IV size is 16 bytes */
			.maxauthsize		= 128,	/* Max auth data size in bytes */
			.setkey 		= crypto4xx_setkey_dtls_null_sha1,
			.setauthsize		= NULL,
			.encrypt 		= crypto4xx_encrypt_dtls,
			.decrypt 		= crypto4xx_decrypt_dtls,
			.givencrypt		= NULL,
			.givdecrypt		= NULL,
	 		}
	 	}
	}},

#endif
#if 1
        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "ssl(aes-sha1)",
		.cra_driver_name 	= "ppc4xx-ssl-aes-sha1",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 64,	/* 64 bits  ... 128-bits block */
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0xF,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
		.cra_module 		= THIS_MODULE,
		.cra_u 		= {
		 	.aead = {
				.ivsize 		= 16,	/* IV size is 16 bytes */
				.maxauthsize		= 128,	/* Max auth data size in bytes */
				.setkey 		= crypto4xx_setkey_ssl_aes_sha1,
				.setauthsize		= NULL,
				.encrypt 		= crypto4xx_encrypt_ssl_aes,
				.decrypt 		= crypto4xx_decrypt_ssl_aes,
				.givencrypt		= NULL,
				.givdecrypt		= NULL,
			}
		}
	}},
        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "ssl(des-sha1)",
		.cra_driver_name 	= "ppc4xx-ssl-des-sha1",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 64,	/* 128-bits block */
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
		.cra_module 		= THIS_MODULE,
		.cra_u 			= {
			.aead = {
			.ivsize 		= 16,	/* IV size is 16 bytes */
			.maxauthsize		= 128,	/* Max auth data size in bytes */
			.setkey 		= crypto4xx_setkey_ssl_des_sha1,
			.setauthsize		= NULL,
			.encrypt 		= crypto4xx_encrypt_ssl_des,
			.decrypt 		= crypto4xx_decrypt_ssl_des,
			.givencrypt		= NULL,
			.givdecrypt		= NULL,
			}
		}
	}},
        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "ssl(des3-sha1)",
		.cra_driver_name 	= "ppc4xx-ssl-des3-sha1",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 64,	/* 128-bits block */
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
		.cra_module 		= THIS_MODULE,
		.cra_u 			= {
			.aead = {
				.ivsize 		= 8,	/* IV size is 16 bytes */
				.maxauthsize		= 128,	/* Max auth data size in bytes */
				.setkey 		= crypto4xx_setkey_ssl_des3_sha1,
				.setauthsize		= NULL,
				.encrypt 		= crypto4xx_encrypt_ssl_des,
				.decrypt 		= crypto4xx_decrypt_ssl_des,
				.givencrypt		= NULL,
				.givdecrypt		= NULL,
			}
		}
	}},

        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "ssl(arc4-sha1)",
		.cra_driver_name 	= "ppc4xx-ssl-arc4-sha1",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 64,	/* 128-bits block */
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
		.cra_module 		= THIS_MODULE,
		.cra_u 			= {
			.aead = {
				.ivsize 		= 16,	/* IV size is 16 bytes */
				.maxauthsize		= 128,	/* Max auth data size in bytes */
				.setkey 		= crypto4xx_setkey_ssl_arc4_sha1,
				.setauthsize		= NULL,
				.encrypt 		= crypto4xx_encrypt_ssl_arc4,
				.decrypt 		= crypto4xx_decrypt_ssl_arc4,
				.givencrypt		= NULL,
				.givdecrypt		= NULL,
			}
		}
	}},
        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "ssl(arc4-md5)",
		.cra_driver_name 	= "ppc4xx-ssl-arc4-md5",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 64,	/* 128-bits block */
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
		.cra_module 		= THIS_MODULE,
		.cra_u 			= {
			.aead = {
				.ivsize 		= 16,	/* IV size is 16 bytes */
				.maxauthsize		= 128,	/* Max auth data size in bytes */
				.setkey 		= crypto4xx_setkey_ssl_arc4_md5,
				.setauthsize		= NULL,
				.encrypt 		= crypto4xx_encrypt_ssl_arc4,
				.decrypt 		= crypto4xx_decrypt_ssl_arc4,
				.givencrypt		= NULL,
				.givdecrypt		= NULL,
			}
		}
	}},
#endif
#if 1
        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "ssl(NULL-md5)",
		.cra_driver_name 	= "ppc4xx-ssl-null-md5",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 64,	/* 128-bits block */
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
		.cra_module 		= THIS_MODULE,
		.cra_u 			= {
			.aead = {
				.ivsize 		= 0,	/* IV size is 16 bytes */
				.maxauthsize		= 128,	/* Max auth data size in bytes */
				.setkey 		= crypto4xx_setkey_ssl_null_md5,
				.setauthsize		= NULL,
				.encrypt 		= crypto4xx_encrypt_ssl_null,
				.decrypt 		= crypto4xx_decrypt_ssl_null,
				.givencrypt		= NULL,
				.givdecrypt		= NULL,
			}
		}
	}},
        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "ssl(NULL-sha1)",
		.cra_driver_name 	= "ppc4xx-ssl-null-sha1",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 64,	/* 128-bits block */
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
		.cra_module 		= THIS_MODULE,
		.cra_u 			= {
			.aead = {
				.ivsize 		= 0,	/* IV size is 16 bytes */
				.maxauthsize		= 128,	/* Max auth data size in bytes */
				.setkey 		= crypto4xx_setkey_ssl_null_sha1,
				.setauthsize		= NULL,
				.encrypt 		= crypto4xx_encrypt_ssl_null,
				.decrypt 		= crypto4xx_decrypt_ssl_null,
				.givencrypt		= NULL,
				.givdecrypt		= NULL,
			}
		}
	}},
#endif
#if 1
        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "tls(aes-sha1)",
		.cra_driver_name 	= "ppc4xx-tls-aes-sha1",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 64,	/* 128-bits block */
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
		.cra_module 		= THIS_MODULE,
		.cra_u 			= {
			.aead = {
				.ivsize 		= 16,	/* IV size is 16 bytes */
				.maxauthsize		= 128,	/* Max auth data size in bytes */
				.setkey 		= crypto4xx_setkey_tls_aes_sha1,
				.setauthsize		= NULL,
				.encrypt 		= crypto4xx_encrypt_ssl_aes,
				.decrypt 		= crypto4xx_decrypt_ssl_aes,
				.givencrypt		= NULL,
				.givdecrypt		= NULL,
			}
		}
	}},
        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "tls(des-sha1)",
		.cra_driver_name 	= "ppc4xx-tls-des-sha1",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 64,	/* 128-bits block */
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
		.cra_module 		= THIS_MODULE,
		.cra_u 			= {
			.aead = {
				.ivsize 		= 16,	/* IV size is 16 bytes */
				.maxauthsize		= 128,	/* Max auth data size in bytes */
				.setkey 		= crypto4xx_setkey_tls_des_sha1,
				.setauthsize		= NULL,
				.encrypt 		= crypto4xx_encrypt_ssl_des,
				.decrypt 		= crypto4xx_decrypt_ssl_des,
				.givencrypt		= NULL,
				.givdecrypt		= NULL,
			}
		}
	}},
        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "tls(des3-sha1)",
		.cra_driver_name 	= "ppc4xx-tls-des3-sha1",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 64,	/* 128-bits block */
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
		.cra_module 		= THIS_MODULE,
		.cra_u 			= {
			.aead = {
				.ivsize 		= 8,	/* IV size is 16 bytes */
				.maxauthsize		= 128,	/* Max auth data size in bytes */
				.setkey 		= crypto4xx_setkey_tls_des3_sha1,
				.setauthsize		= NULL,
				.encrypt 		= crypto4xx_encrypt_ssl_des,
				.decrypt 		= crypto4xx_decrypt_ssl_des,
				.givencrypt		= NULL,
				.givdecrypt		= NULL,
			}
		}
	}},
        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "tls(arc4-sha1)",
		.cra_driver_name 	= "ppc4xx-tls-arc4-sha1",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 64,	/* 128-bits block */
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
		.cra_module 		= THIS_MODULE,
		.cra_u 			= {
			.aead = {
				.ivsize 		= 16,	/* IV size is 16 bytes */
				.maxauthsize		= 128,	/* Max auth data size in bytes */
				.setkey 		= crypto4xx_setkey_tls_arc4_sha1,
				.setauthsize		= NULL,
				.encrypt 		= crypto4xx_encrypt_ssl_arc4,
				.decrypt 		= crypto4xx_decrypt_ssl_arc4,
				.givencrypt		= NULL,
				.givdecrypt		= NULL,
			}
		}
	}},
        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "tls(arc4-md5)",
		.cra_driver_name 	= "ppc4xx-tls-arc4-md5",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 64,	/* 128-bits block */
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
		.cra_module 		= THIS_MODULE,
		.cra_u 			= {
			.aead = {
				.ivsize 		= 16,	/* IV size is 16 bytes */
				.maxauthsize		= 128,	/* Max auth data size in bytes */
				.setkey 		= crypto4xx_setkey_tls_arc4_md5,
				.setauthsize		= NULL,
				.encrypt 		= crypto4xx_encrypt_ssl_arc4,
				.decrypt 		= crypto4xx_decrypt_ssl_arc4,
				.givencrypt		= NULL,
				.givdecrypt		= NULL,
			}
		}
	}},
        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "tls(NULL-md5)",
		.cra_driver_name 	= "ppc4xx-tls-null-md5",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 64,	/* 128-bits block */
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
		.cra_module 		= THIS_MODULE,
		.cra_u 			= {
			.aead = {
				.ivsize 		= 0,	/* IV size is 16 bytes */
				.maxauthsize		= 128,	/* Max auth data size in bytes */
				.setkey 		= crypto4xx_setkey_tls_null_md5,
				.setauthsize		= NULL,
				.encrypt 		= crypto4xx_encrypt_ssl_null,
				.decrypt 		= crypto4xx_decrypt_ssl_null,
				.givencrypt		= NULL,
				.givdecrypt		= NULL,
			}
		}
	}},
        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "tls(NULL-sha1)",
		.cra_driver_name 	= "ppc4xx-tls-null-sha1",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 64,	/* 128-bits block */
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
		.cra_module 		= THIS_MODULE,
		.cra_u 			= {
			.aead = {
				.ivsize 		= 0,	/* IV size is 16 bytes */
				.maxauthsize		= 128,	/* Max auth data size in bytes */
				.setkey 		= crypto4xx_setkey_tls_null_sha1,
				.setauthsize		= NULL,
				.encrypt 		= crypto4xx_encrypt_ssl_null,
				.decrypt 		= crypto4xx_decrypt_ssl_null,
				.givencrypt		= NULL,
				.givdecrypt		= NULL,
			}
		}
	}},
        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "tls1_1(aes-sha1)",
		.cra_driver_name 	= "ppc4xx-tls1.1-aes-sha1",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 64,	/* 128-bits block */
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
		.cra_module 		= THIS_MODULE,
		.cra_u 			= {
			.aead = {
				.ivsize 		= 16,	/* IV size is 16 bytes */
				.maxauthsize		= 128,	/* Max auth data size in bytes */
				.setkey 		= crypto4xx_setkey_tls1_1_aes_sha1,
				.setauthsize		= NULL,
				.encrypt 		= crypto4xx_encrypt_dtls,
				.decrypt 		= crypto4xx_decrypt_dtls,
				.givencrypt		= NULL,
				.givdecrypt		= NULL,
			}
		}
	}},
        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "tls1_1(des-sha1)",
		.cra_driver_name 	= "ppc4xx-tls1.1-des-sha1",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 64,	/* 128-bits block */
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
		.cra_module 		= THIS_MODULE,
		.cra_u 			= {
			.aead = {
				.ivsize 		= 16,	/* IV size is 16 bytes */
				.maxauthsize		= 128,	/* Max auth data size in bytes */
				.setkey 		= crypto4xx_setkey_tls1_1_des_sha1,
				.setauthsize		= NULL,
				.encrypt 		= crypto4xx_encrypt_dtls,
				.decrypt 		= crypto4xx_decrypt_dtls,
				.givencrypt		= NULL,
				.givdecrypt		= NULL,
			}
		}
	}},
        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "tls1_1(des3-sha1)",
		.cra_driver_name 	= "ppc4xx-tls1.1-des3-sha1",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 64,	/* 128-bits block */
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
		.cra_module 		= THIS_MODULE,
		.cra_u 			= {
			.aead = {
				.ivsize 		= 8,	/* IV size is 16 bytes */
				.maxauthsize		= 128,	/* Max auth data size in bytes */
				.setkey 		= crypto4xx_setkey_tls1_1_des3_sha1,
				.setauthsize		= NULL,
				.encrypt 		= crypto4xx_encrypt_dtls,
				.decrypt 		= crypto4xx_decrypt_dtls,
				.givencrypt		= NULL,
				.givdecrypt		= NULL,
			}
		}
	}},
        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "tls1_1(arc4-sha1)",
		.cra_driver_name 	= "ppc4xx-tls1.1-arc4-sha1",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 64,	/* 128-bits block */
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
		.cra_module 		= THIS_MODULE,
		.cra_u 			= {
			.aead = {
				.ivsize 		= 16,	/* IV size is 16 bytes */
				.maxauthsize		= 128,	/* Max auth data size in bytes */
				.setkey 		= crypto4xx_setkey_tls1_1_arc4_sha1,
				.setauthsize		= NULL,
				.encrypt 		= crypto4xx_encrypt_ssl_arc4,
				.decrypt 		= crypto4xx_decrypt_ssl_arc4,
				.givencrypt		= NULL,
				.givdecrypt		= NULL,
			}
		}
	}},
        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "tls1_1(arc4-md5)",
		.cra_driver_name 	= "ppc4xx-tls1.1-arc4-md5",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 64,	/* 128-bits block */
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
		.cra_module 		= THIS_MODULE,
		.cra_u 			= {
			.aead = {
				.ivsize 		= 16,	/* IV size is 16 bytes */
				.maxauthsize		= 128,	/* Max auth data size in bytes */
				.setkey 		= crypto4xx_setkey_tls1_1_arc4_md5,
				.setauthsize		= NULL,
				.encrypt 		= crypto4xx_encrypt_ssl_arc4,
				.decrypt 		= crypto4xx_decrypt_ssl_arc4,
				.givencrypt		= NULL,
				.givdecrypt		= NULL,
			}
		}
	}},
        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "tls1_1(NULL-md5)",
		.cra_driver_name 	= "ppc4xx-tls1.1-null-md5",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 64,	/* 128-bits block */
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
		.cra_module 		= THIS_MODULE,
		.cra_u 			= {
			.aead = {
				.ivsize 		= 0,	/* IV size is 16 bytes */
				.maxauthsize		= 128,	/* Max auth data size in bytes */
				.setkey 		= crypto4xx_setkey_tls1_1_null_md5,
				.setauthsize		= NULL,
				.encrypt 		= crypto4xx_encrypt_dtls,
				.decrypt 		= crypto4xx_decrypt_dtls,
				.givencrypt		= NULL,
				.givdecrypt		= NULL,
			}
		}
	}},

        { .type = CRYPTO_ALG_TYPE_AEAD, .u.cipher = {
		.cra_name 		= "tls1_1(NULL-sha1)",
		.cra_driver_name 	= "ppc4xx-tls1.1-null-sha1",
		.cra_priority 		= CRYPTO4XX_CRYPTO_PRIORITY,
		.cra_flags 		= CRYPTO_ALG_TYPE_AEAD | CRYPTO_ALG_ASYNC,
		.cra_blocksize 		= 64,	/* 128-bits block */
		.cra_ctxsize 		= sizeof(struct crypto4xx_ctx),
		.cra_alignmask 		= 0,
		.cra_type 		= &crypto_aead_type,
		.cra_init		= crypto4xx_alg_init,
		.cra_exit		= crypto4xx_alg_exit,
		.cra_module 		= THIS_MODULE,
		.cra_u 			= {
			.aead = {
				.ivsize 		= 0,	/* IV size is 16 bytes */
				.maxauthsize		= 128,	/* Max auth data size in bytes */
				.setkey 		= crypto4xx_setkey_tls1_1_null_sha1,
				.setauthsize		= NULL,
				.encrypt 		= crypto4xx_encrypt_dtls,
				.decrypt 		= crypto4xx_decrypt_dtls,
				.givencrypt		= NULL,
				.givdecrypt		= NULL,
			}
		}
	}},
#endif
};
#define CRYPTO4XX_CONSOLE_DRIVER_NAME	"crypto4xx"
/**
 * Module Initialization Routine
 */
static int __init crypto4xx_probe(struct of_device *ofdev,
				  const struct of_device_id *match)
{
	int rc;
	struct resource res;
	struct device *dev = &ofdev->dev;
	struct crypto4xx_core_device *core_dev;
	unsigned int pvr;
	unsigned short min;
	int revb_ver = 1;	/* RevB of crypto core */

	rc = of_address_to_resource(ofdev->node, 0, &res);
	if (rc)
		return -ENODEV;

	if (of_find_compatible_node(NULL, NULL, "amcc,ppc460ex-crypto")) {
		mtdcri(SDR0, PPC460EX_SDR0_SRST,
		       mfdcri(SDR0, PPC460EX_SDR0_SRST) | PPC460EX_CE_RESET);
		mtdcri(SDR0, PPC460EX_SDR0_SRST,
		       mfdcri(SDR0, PPC460EX_SDR0_SRST) & ~PPC460EX_CE_RESET);
	} else if (of_find_compatible_node(NULL, NULL,
			"amcc,ppc405ex-crypto")) {
		mtdcri(SDR0, PPC405EX_SDR0_SRST,
		       mfdcri(SDR0, PPC405EX_SDR0_SRST) | PPC405EX_CE_RESET);
		mtdcri(SDR0, PPC405EX_SDR0_SRST,
		       mfdcri(SDR0, PPC405EX_SDR0_SRST) & ~PPC405EX_CE_RESET);
		revb_ver = 0;
	} else if (of_find_compatible_node(NULL, NULL,
			"amcc,ppc460sx-crypto")) {
		mtdcri(SDR0, PPC460SX_SDR0_SRST,
		       mfdcri(SDR0, PPC460SX_SDR0_SRST) | PPC460SX_CE_RESET);
		mtdcri(SDR0, PPC460SX_SDR0_SRST,
		       mfdcri(SDR0, PPC460SX_SDR0_SRST) & ~PPC460SX_CE_RESET);
	} else {
		printk(KERN_ERR "Crypto Function Not supported!\n");
		return -EINVAL;
	}

	core_dev = kzalloc(sizeof(struct crypto4xx_core_device), GFP_KERNEL);
	if (!core_dev)
		return -ENOMEM;

	dev_set_drvdata(dev, core_dev);
	core_dev->ofdev = ofdev;
	core_dev->dev = kzalloc(sizeof(struct crypto4xx_device), GFP_KERNEL);
	if (!core_dev->dev)
		goto err_alloc_dev;

	/* Older version of 460EX/GT does not support H/W based security intr coalescing */
	pvr = mfspr(SPRN_PVR);
	printk("Reading pvr value = %x\n", pvr);
	if ((pvr & 0xfffffff0) == 0x130218A0) {
		min = PVR_MIN(pvr);
		if (min < 4) {
			printk(KERN_INFO "RevA 460EX/GT ... h/w bug in security intr coal\n");
			revb_ver = 0;
		} else {
			printk(KERN_INFO "RevB h/w security interrupt coalescing supported ...\n");
		}
	}

	core_dev->revb_ver = revb_ver;
	core_dev->dev->core_dev = core_dev;
	core_dev->device = dev;
	core_dev->irq_cnt = 0ll;
	spin_lock_init(&core_dev->lock);
	INIT_LIST_HEAD(&core_dev->dev->alg_list);
	rc = crypto4xx_build_pdr(core_dev->dev);
	if (rc)
		goto err_build_pdr;

	rc = crypto4xx_build_gdr(core_dev->dev);
	if (rc)
		goto err_build_gdr;

	rc = crypto4xx_build_sdr(core_dev->dev);
	if (rc)
		goto err_build_sdr;

	proc_crypto4xx = proc_mkdir("driver/crypto4xx", NULL);
	if(proc_crypto4xx == NULL) {
		printk(KERN_ERR
				"%s: Error creating proc entry\n",__FUNCTION__);
		return -ENOMEM;
	}
 
	entry = create_proc_read_entry("crypto4xx",
				       0,
				       proc_crypto4xx,
				       crypto4xx_device_read_procmem,
				       core_dev);
	if(entry == NULL) {
		printk(KERN_CRIT
				"%s: crypto4xx: create_proc_read_entry failed!\n",
		__FUNCTION__);
		return -ENOMEM;
	}

	/* Init tasklet for bottom half processing */
	tasklet_init(&core_dev->tasklet, crypto4xx_bh_tasklet_cb,
		     (unsigned long) dev);

	/* Register for Crypto isr, Crypto Engine IRQ */
	core_dev->irq = irq_of_parse_and_map(ofdev->node, 0);
	rc = request_irq(core_dev->irq, crypto4xx_ce_interrupt_handler, 0,
			 "CRYPTO", dev);
	if (rc)
		goto err_request_irq;

	core_dev->dev->ce_base = of_iomap(ofdev->node, 0);
	if (!core_dev->dev->ce_base) {
		dev_err(dev, "failed to of_iomap\n");
		goto err_iomap;
	}

	/* need to setup pdr, rdr, gdr and sdr before this */
	crypto4xx_hw_init(core_dev->dev);

	/* Register security algorithms with Linux CryptoAPI */
	rc = crypto4xx_register_alg(core_dev->dev, crypto4xx_alg,
			       ARRAY_SIZE(crypto4xx_alg));
	if (rc)
		goto err_start_dev;

	return 0;

err_start_dev:
	iounmap(core_dev->dev->ce_base);
err_iomap:
	free_irq(core_dev->irq, dev);
	irq_dispose_mapping(core_dev->irq);
	tasklet_kill(&core_dev->tasklet);
err_request_irq:
	crypto4xx_destroy_sdr(core_dev->dev);
err_build_sdr:
	crypto4xx_destroy_gdr(core_dev->dev);
err_build_gdr:
	crypto4xx_destroy_pdr(core_dev->dev);
err_build_pdr:
	kfree(core_dev->dev);
err_alloc_dev:
	kfree(core_dev);

	return rc;
}

static int __exit crypto4xx_remove(struct of_device *ofdev)
{
	struct device *dev = &ofdev->dev;
	struct crypto4xx_core_device *core_dev = dev_get_drvdata(dev);

	free_irq(core_dev->irq, dev);
	irq_dispose_mapping(core_dev->irq);

	tasklet_kill(&core_dev->tasklet);
	/* Un-register with Linux CryptoAPI */
	crypto4xx_unregister_alg(core_dev->dev);
	/* Free all allocated memory */
	remove_proc_entry("crypto4xx", entry) ;
	entry = NULL;
	remove_proc_entry("driver/crypto4xx", proc_crypto4xx);
	proc_crypto4xx = NULL;

	crypto4xx_stop_all(core_dev);
	return 0;
}

static struct of_device_id crypto4xx_match[] = {
	{ .compatible      = "amcc,ppc4xx-crypto",},
	{ },
};

static struct of_platform_driver crypto4xx_driver = {
	.name		= "crypto4xx",
	.match_table	= crypto4xx_match,
	.probe		= crypto4xx_probe,
	.remove		= crypto4xx_remove,
};

static int __init crypto4xx_init(void)
{
	return of_register_platform_driver(&crypto4xx_driver);
}

static void __exit crypto4xx_exit(void)
{
	of_unregister_platform_driver(&crypto4xx_driver);
}

module_init(crypto4xx_init);
module_exit(crypto4xx_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("James Hsiao <jhsiao@amcc.com>");
MODULE_DESCRIPTION("Driver for AMCC PPC4xx crypto accelerator");

