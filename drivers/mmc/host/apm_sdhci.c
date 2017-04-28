/**
 * AppliedMicro Secure Digital Host Controller Interface.
 *
 * Copyright (c) 2010 Applied Micro Circuits Corporation.
 * All rights reserved. Loc Ho <lho@amcc.com>
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
 * This module implements PCIe functionality for APM96xxx. This setups and
 * configures PCIe controllers as either root complex or endpoint.
 *
 */
#include <linux/module.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/of.h>
#include <linux/of_platform.h>
#include <linux/mmc/host.h>
#include <linux/cpumask.h>
#include <linux/clk.h>
#include <asm/mpic.h>
#include "sdhci.h"
#ifdef CONFIG_APM86xxx
#include <asm/io.h>
#include <asm/ipp.h>
#include <asm/apm_ipp_csr.h>
#endif

/* #define DEBUG_APMSDIO */

#ifdef DEBUG_APMSDIO
#define DEBUG_RD(fmt, ...)	printk(KERN_INFO fmt, ##__VA_ARGS__); 
#define DEBUG_WR(fmt, ...)	printk(KERN_INFO fmt, ##__VA_ARGS__);
#else
#define DEBUG_RD(fmt, ...)	
#define DEBUG_WR(fmt, ...)	
#endif

struct sdhci_of_data {
	unsigned int quirks;
	struct sdhci_ops ops;
};

struct sdhci_of_host {
	unsigned int clock;
	u16 xfer_mode_shadow;
	u16 xfer_block_size_shadow;
	u8 slot;
	void *ahbc_ioaddr;
};

/*
 * Ops and quirks for the PPRO SDHC controller.
 */
static u32 apm_sdhci_readl(struct sdhci_host *host, int reg)
{
	u32 val = in_le32(host->ioaddr + reg);
	DEBUG_RD("APMSDHCI RDL: 0x%08X val: 0x%08X\n",
		(u32) host->ioaddr + reg, val);
	return val;
}

static u16 apm_sdhci_readw(struct sdhci_host *host, int reg)
{
	u32 val_act;
	u32 val;
	int base = reg & ~0x3;
	int shift = (reg & 0x2) * 8;

	val_act = in_le32(host->ioaddr + base);
	val = ((val_act >> shift) & 0xffff);
	DEBUG_RD("APMSDHC RDW: 0x%08X value: 0x%04X (0x%08X 0x%08X)\n",
		(u32) host->ioaddr + reg, val, 
		(u32) host->ioaddr + base, val_act);	
	return (u16) val;
}

static u8 apm_sdhci_readb(struct sdhci_host *host, int reg)
{
	u32 val;
	u32 val_act;
	int base = reg & ~0x3;
	int shift = (reg & 0x3) * 8;
	val_act = in_le32(host->ioaddr + base);
	val = ((val_act >> shift) & 0xff);
	DEBUG_RD("APMSDHC RDB: 0x%08X value: 0x%02X (0x%08X 0x%08X)\n",
		(u32) host->ioaddr + reg, val, 
		(u32) host->ioaddr + base, val_act);
	return (u8) val;
}

static void apm_sdhci_writel(struct sdhci_host *host, u32 val, int reg)
{
	out_le32(host->ioaddr + reg, val);
	DEBUG_WR("APMSDHC WRL: 0x%08X value: 0x%08X\n",
		(u32) host->ioaddr + reg, val);
}

static void apm_sdhci_writew(struct sdhci_host *host, u16 val, int reg)
{
	struct sdhci_of_host *of_host = sdhci_priv(host);
	int base = reg & ~0x3;
	int shift = (reg & 0x2) * 8;
#ifdef DEBUG_APMSDIO
	u32 val32;
	u32 val32_act;
#endif
	
	switch (reg) {
	case SDHCI_TRANSFER_MODE:
		/*
		 * Postpone this write, we must do it together with a
		 * command write that is down below.
		 */
		of_host->xfer_mode_shadow = val;
		return;
	case SDHCI_COMMAND:
		apm_sdhci_writel(host, val << 16 | of_host->xfer_mode_shadow,
			     SDHCI_TRANSFER_MODE);
		return;
	case SDHCI_BLOCK_COUNT:
		apm_sdhci_writel(host, val << 16 | 
				of_host->xfer_block_size_shadow,
			     	SDHCI_BLOCK_SIZE);
		return;
	case SDHCI_BLOCK_SIZE:
		/*
		 * Two last DMA bits are reserved, and first one is used for
		 * non-standard blksz of 4096 bytes that we don't support
		 * yet. So clear the DMA boundary bits.
		 */
		val &= ~SDHCI_MAKE_BLKSZ(0x7, 0);
		/*
		 * Postpone this write, we must do it together with a
		 * command write with SDHCI_BLOCK_COUNT.
		 */
		of_host->xfer_block_size_shadow = val;
		return;
	}
#ifdef DEBUG_APMSDIO
	val32_act = in_le32(host->ioaddr + base);
	val32 = (val32_act & ~((u32) 0xffff << shift)) | ((u32) val << shift);
	out_le32(host->ioaddr + base, val32);
	DEBUG_WR("APMSDHC WRW: 0x%08X value: 0x%04X "
		"(0x%08X 0x%08X 0x%08X 0x%08X)\n",
		(u32) host->ioaddr + reg, val32 >> shift,
		(u32) host->ioaddr + base, val32_act, val32,
		in_le32(host->ioaddr + base));
#else
	clrsetbits_le32(host->ioaddr + base, 0xffff << shift, val << shift);
#endif
}

static void apm_sdhci_writeb(struct sdhci_host *host, u8 val, int reg)
{
	int base = reg & ~0x3;
	int shift = (reg & 0x3) * 8;
#ifdef DEBUG_APMSDIO
	u32 val32;
	u32 val32_act;
	
	val32_act = in_le32(host->ioaddr + base);
	val32 = (val32_act & ~((u32) 0xff << shift)) | ((u32) val << shift);
	out_le32(host->ioaddr + base, val32);
	DEBUG_WR("APMSDHC WRB: 0x%08X value: 0x%02X "
		"(0x%08X 0x%08X 0x%08X 0x%08X)\n",
		(u32) host->ioaddr + reg, (val32 >> shift) & 0xFF,
		(u32) host->ioaddr + base, val32_act, val32,
		in_le32(host->ioaddr + base));
#else
	clrsetbits_le32(host->ioaddr + base, ((u32) 0xff) << shift, 
			((u32) val) << shift);
#endif
}

static struct sdhci_of_data apm_sdhci_esdhc = {
	.quirks = 0,
	.ops = {
		.readl = apm_sdhci_readl,
		.readw = apm_sdhci_readw,
		.readb = apm_sdhci_readb,
		.writel = apm_sdhci_writel,
		.writew = apm_sdhci_writew,
		.writeb = apm_sdhci_writeb,
	},
};

#ifdef CONFIG_PM

static int apm_sdhci_suspend(struct of_device *ofdev, pm_message_t state)
{
	struct sdhci_host *host = dev_get_drvdata(&ofdev->dev);

	return sdhci_suspend_host(host, state);
}

static int apm_sdhci_resume(struct of_device *ofdev)
{
	struct sdhci_host *host = dev_get_drvdata(&ofdev->dev);
	u32 val32;
	struct sdhci_of_host *of_host;
	
	of_host = sdhci_priv(host);
#ifdef CONFIG_APM86xxx
	val32 = 0x01FC1970; 
	out_be32((volatile unsigned *)(of_host->ahbc_ioaddr + 0x34 + (of_host->slot*8)), val32);
	val32 = 0x000c008f;
	out_be32((volatile unsigned *)(of_host->ahbc_ioaddr + 0x38 + (of_host->slot*8)), val32);
#endif
	return sdhci_resume_host(host);
}

#else

#define apm_sdhci_suspend NULL
#define apm_sdhci_resume NULL

#endif

static int __devinit apm_sdhci_probe(struct of_device *ofdev,
				 const struct of_device_id *match)
{
	struct device_node *np = ofdev->node;
	struct sdhci_of_data *sdhci_of_data = match->data;
	struct sdhci_host *host;
	struct sdhci_of_host *of_host;
	const u32 *pval;
       	int ret;
       	u32 val32;

#ifdef CONFIG_APM86xxx
	/* FIXME: need better if sharing SD in AMP */
	static int enable_clk = 1;
	struct clk *clk = NULL;
	
	if (enable_clk) {	
		clk = clk_get(NULL, "sdio");
		if (IS_ERR(clk)) {
			printk(KERN_ERR "clk_get failed for sdio\n");
			return -EPERM;
		}	
		clk_enable(clk);
		
		ret = clk_set_rate(clk, 50000000);
		if (ret) {
			printk("ERROR: Couldn't set SD Clk to 50MHz: %d\n", ret);
			return -ENODEV;
		}
		enable_clk = 0;
	}
#endif

	if (!of_device_is_available(np))
		return -ENODEV;

	host = sdhci_alloc_host(&ofdev->dev, sizeof(*of_host));
	if (!host) {
		printk(KERN_ERR "unable to allocate SDHCI structure\n");
		return -ENOMEM;
	}
	
	of_host = sdhci_priv(host);
	dev_set_drvdata(&ofdev->dev, host);


	host->ioaddr = of_iomap(np, 0);
	if (!host->ioaddr) {
		printk(KERN_ERR "no IO register entry in DTS\n");
		ret = -ENOMEM;
		goto err_addr_map;
	}
	of_host->ahbc_ioaddr = of_iomap(np, 1);
	if (!of_host->ahbc_ioaddr) {
		printk(KERN_ERR "no AHBC IO register entry in DTS\n");
		ret = -ENOMEM;
		goto err_no_irq;
	}
        pval = of_get_property(np, "slot", NULL);
        if (pval == NULL) {
		printk(KERN_ERR "no slot entry in DTS\n");
		ret = -EINVAL;
        	goto err_no_irq;
	}
        of_host->slot = *pval;

#ifdef CONFIG_APM86xxx
 	val32 = 0x01FC1970; 
	out_be32((volatile unsigned *)(of_host->ahbc_ioaddr + 0x34 + (of_host->slot*8)), val32);
	val32 = 0x000c008f;
	out_be32((volatile unsigned *)(of_host->ahbc_ioaddr + 0x38 + (of_host->slot*8)), val32);
#endif

	host->irq = irq_of_parse_and_map(np, 0);
	if (!host->irq) {
		ret = -EINVAL;
		goto err_no_irq;
	}
        pval = of_get_property(np, "irq-affinity", NULL);
        if (pval != NULL) {
        	cpumask_t cpu_mask = (cpumask_t) { {
        				[BITS_TO_LONGS(NR_CPUS)-1] = *pval		
        			} };
        	
	        /* Set IRQ affinity */
	        printk(KERN_INFO "set SDIO IRQ %d affinity to 0x%02X\n",
	        	host->irq, *pval);
	        mpic_set_affinity(host->irq, &cpu_mask);
	}
		
	host->hw_name = dev_name(&ofdev->dev);
	if (sdhci_of_data) {
		host->quirks = sdhci_of_data->quirks;
		host->ops = &sdhci_of_data->ops;
	}

	/* Force SDHCI do only 1 bit data transfers */
	host->quirks |= SDHCI_QUIRK_DELAY_AFTER_POWER;

	/* Enable 4 bit only */	
	host->caps &= ~MMC_CAP_8_BIT_DATA;
	host->caps |= MMC_CAP_4_BIT_DATA;

	host->mmc->ocr = 0x40f00000;
	ret = sdhci_add_host(host);
	if (ret)
		goto err_add_host;

	return 0;

err_add_host:
	irq_dispose_mapping(host->irq);
err_no_irq:
	iounmap(host->ioaddr);
err_addr_map:
	sdhci_free_host(host);
	return ret;
}

static int __devexit apm_sdhci_remove(struct of_device *ofdev)
{
	struct sdhci_host *host = dev_get_drvdata(&ofdev->dev);
	struct sdhci_of_host *of_host = sdhci_priv(host);
	
	sdhci_remove_host(host, 0);
	sdhci_free_host(host);
	irq_dispose_mapping(host->irq);
	iounmap(host->ioaddr);
	iounmap(of_host->ahbc_ioaddr);
	return 0;
}

static const struct of_device_id apm_sdhci_match[] = {
	{ .compatible = "apm,apm-sdhc", .data = &apm_sdhci_esdhc, },
	{},
};
MODULE_DEVICE_TABLE(of, apm_sdhci_match);

static struct of_platform_driver apm_sdhci_driver = {
	.driver.name = "apm-sdhci",
	.match_table = apm_sdhci_match,
	.probe = apm_sdhci_probe,
	.remove = __devexit_p(apm_sdhci_remove),
	.suspend = apm_sdhci_suspend,
	.resume	= apm_sdhci_resume,
};

static int __init apm_sdhci_init(void)
{
	return of_register_platform_driver(&apm_sdhci_driver);
}
module_init(apm_sdhci_init);

static void __exit apm_sdhci_exit(void)
{
	of_unregister_platform_driver(&apm_sdhci_driver);
}
module_exit(apm_sdhci_exit);

MODULE_DESCRIPTION("APM Secure Digital Host Controller Interface driver");
MODULE_AUTHOR("Loc Ho <lho@amcc.com>");
MODULE_LICENSE("GPL");
