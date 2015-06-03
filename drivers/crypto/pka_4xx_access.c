/*******************************************************************************
 *
 * Copyright (c) 2008 Loc Ho <lho@amcc.com>
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
 *
 * Detail Description:
 * This file defines ioctl structures for the Linux CryptoAPI interface. It
 * provides user space applications accesss into the Linux CryptoAPI
 * functionalities.
 *
 * @file pka4xx_acccess.c
 *
 * This file provides access and implementation of the PKA hardware
 * under Linux.
 *
 *******************************************************************************
 */
#include <linux/version.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/of_platform.h>
#include <linux/interrupt.h>
#include <asm/io.h>
#include <crypto/pka_4xx.h>
#include "pka_4xx_access.h"

#define PKA4XX_VER_STR	"0.1"

struct pka4xx_dev {
	struct pka4xx_ctx	ctx;
	struct resource 	csr_res;
        struct resource 	pka_ram_res;
};

struct hal_config {
	struct of_device 	*ofdev;
};

static struct pka4xx_dev	pka_dev = {
	.ctx = {
		0,
		{ { 1, PKA4XX_RAM_FREE_SIZE/4, 0 },
		  { 0, 0, 0 } },
		  0, 0
	}
};

struct pka4xx_ctx *pka_get_ctx(void)
{
	return &pka_dev.ctx;
}

int pka4xx_config_set(struct hal_config *cfg)
{
	struct device_node *pka_np = cfg->ofdev->node;
	int rc = 0;
	
	rc = of_address_to_resource(pka_np, 0, &pka_dev.csr_res);
	if (rc) {
		LPRINTF(LL_INFO, "error getting address to resource");
		return -ENODEV;
	}
	pka_dev.ctx.csr_paddr  = pka_dev.csr_res.start;
	pka_dev.ctx.csr    = ioremap(pka_dev.csr_res.start,
	pka_dev.csr_res.end - pka_dev.csr_res.start + 1);
	
	if (pka_dev.ctx.csr == NULL) {
		LPRINTF(LL_ERR,
			"unable to ioremap 0x%02X_%08X size %d",
			(u32) (pka_dev.csr_res.start >> 32),
			(u32) pka_dev.csr_res.start,
			(u32) (pka_dev.csr_res.end - pka_dev.csr_res.start + 1));
		return -ENOMEM;
	}

	pka_dev.ctx.irq = of_irq_to_resource(pka_np, 0, NULL);
	
	if (pka_dev.ctx.irq == NO_IRQ) {
		/* Un-map CSR */
		iounmap(pka_dev.ctx.csr);
		pka_dev.ctx.csr = NULL;
		LPRINTF(LL_ERR, "no irq");
		return -EINVAL;
	}
	
	return 0;
}

int pka4xx_config_clear(void)
{
	iounmap(pka_dev.ctx.csr);
	return 0;
}

int pka4xx_csr_hw_read32(u32 reg_addr, u32 *data_val)
{
	*data_val = in_le32((volatile unsigned __iomem *)
				(pka_dev.ctx.csr + reg_addr));
	return 0;
}

int pka4xx_csr_hw_write32(u32 reg_addr, u32 data_val)
{	
	out_le32((volatile unsigned __iomem *) (pka_dev.ctx.csr + reg_addr),
		  data_val);
	return 0;
}

/**
 * Setup Driver with platform registration
 */
static int __devinit pka4xx_probe(struct of_device *ofdev,
				    const struct of_device_id *match)
{
	struct hal_config hw_cfg;
	int rc;

	hw_cfg.ofdev = ofdev;
	rc = pka4xx_config_set(&hw_cfg);
	if (rc != 0)
		return rc;

	printk(KERN_INFO "AMCC 4xx PKA v%s @0x%02X_%08X size %d IRQ %d\n",
		PKA4XX_VER_STR,
		(u32) (pka_dev.csr_res.start >> 32),
		(u32) pka_dev.csr_res.start,
		(u32) (pka_dev.csr_res.end - pka_dev.csr_res.start + 1),
               pka_dev.ctx.irq);

	rc = pka4xx_hw_init();
	if (rc < 0) {
		LPRINTF(LL_ERR, "failed to initialize PKA");
		goto err;
	}
	printk(KERN_INFO "PKA Driver Successfully Initialized\n");
	return rc;

err:
	pka4xx_config_clear();
	return rc;
}

static int __devexit pka4xx_remove(struct of_device *dev)
{
	pka4xx_hw_deinit();
	pka4xx_config_clear();
	return 0;
}

static struct of_device_id pka4xx_match[] = {
	{ .compatible      = "ppc4xx-pka", },
	{ .compatible      = "amcc,ppc4xx-pka", },
	{ },
};

static struct of_platform_driver pka4xx_driver = {
	.name		= "ppc4xx-pka",
	.match_table	= pka4xx_match,
	.probe		= pka4xx_probe,
	.remove		= pka4xx_remove,
};

static int __init mod_init(void)
{
	return of_register_platform_driver(&pka4xx_driver);
}

static void __exit mod_exit(void)
{
	of_unregister_platform_driver(&pka4xx_driver);
}

module_init(mod_init);
module_exit(mod_exit);

MODULE_DESCRIPTION("AMCC 4xx Public Key Accelerator");
MODULE_LICENSE("GPL");

EXPORT_SYMBOL_GPL(pka4xx_mul);
EXPORT_SYMBOL_GPL(pka4xx_div);
EXPORT_SYMBOL_GPL(pka4xx_mod);
EXPORT_SYMBOL_GPL(pka4xx_add);
EXPORT_SYMBOL_GPL(pka4xx_sub);
EXPORT_SYMBOL_GPL(pka4xx_addsub);
EXPORT_SYMBOL_GPL(pka4xx_rshift);
EXPORT_SYMBOL_GPL(pka4xx_lshift);
EXPORT_SYMBOL_GPL(pka4xx_compare);
EXPORT_SYMBOL_GPL(pka4xx_expmod);
EXPORT_SYMBOL_GPL(pka4xx_expmod_crt);

