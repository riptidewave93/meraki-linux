/**
 *
 * Copyright (c) 2008 Loc Ho <spulijala@amcc.com>
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
 * Detail Description:
 * This file AMCC TRNG offload Linux device driver
 *
 * @file trng4xx.c
 *
 *
 */
#include <linux/version.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/of_platform.h>
#include <linux/hw_random.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <crypto/trng4xx.h>

#define TRNG4XX_VER_STR	"0.1"
#define TRNG4XX_HDR	"TRNG4XX: "

/* #define TRNG4XX_DEBUG */

#if !defined(TRNG4XX_DEBUG)
#	define TRNG4XX_LOG(fmt, ...)
#else
#	define TRNG4XX_LOG(fmt, ...)		\
		do { \
			printk(KERN_INFO TRNG4XX_HDR fmt "\n", ##__VA_ARGS__); \
} while(0);
#endif

struct trng4xx_dev {
	struct resource 	res;
	u32			irq;
	volatile char __iomem   *csr;
	struct semaphore        access_prot;
	u32	datum;
};

struct hal_config {
	struct of_device 	*ofdev;
};

static struct trng4xx_dev	trng4xx_dev;

static irqreturn_t trng4xx_irq_handler(int irq, void *id);
static void trng4xx_chk_overflow(void);

int trng4xx_config_set(struct hal_config *cfg)
{
	struct device_node *rng_np = cfg->ofdev->node;
	int rc = 0;

	rc = of_address_to_resource(rng_np, 0, &trng4xx_dev.res);
	if (rc)
		return -ENODEV;

	trng4xx_dev.csr    = ioremap(trng4xx_dev.res.start,
				     trng4xx_dev.res.end -
				trng4xx_dev.res.start + 1);
	if (trng4xx_dev.csr == NULL) {
		printk(KERN_ERR "unable to ioremap 0x%02X_%08X size %d\n",
			(u32) (trng4xx_dev.res.start >> 32),
			(u32) trng4xx_dev.res.start,
			(u32) (trng4xx_dev.res.end -
					trng4xx_dev.res.start + 1));
		return -ENOMEM;
	}

	TRNG4XX_LOG("TRNG1 0x%02X_%08X size %d\n",
			(u32) (trng4xx_dev.res.start >> 32),
			(u32) trng4xx_dev.res.start,
			(u32) (trng4xx_dev.res.end -
					trng4xx_dev.res.start + 1));

	trng4xx_dev.irq = of_irq_to_resource(rng_np, 0, NULL);

        if (trng4xx_dev.irq == NO_IRQ) {
		/* Un-map CSR */
		iounmap(trng4xx_dev.csr);
		trng4xx_dev.csr = NULL;
		return -EINVAL;
	}

	return 0;
}

int trng4xx_pka_config_clear(void)
{
	iounmap(trng4xx_dev.csr);
	return 0;
}

inline int trng4xx_hw_read32(u32 reg_addr, u32 *data_val)
{
	*data_val = in_be32((unsigned __iomem *)
				(trng4xx_dev.csr + reg_addr));
	return 0;
}

inline int trng4xx_hw_write32(u32 reg_addr, u32 data_val)
{
	out_be32((unsigned __iomem *) (trng4xx_dev.csr + reg_addr),
		 data_val);
        return 0;
}

int trng4xx_hw_init(void)
{
	int rc;

	rc = request_irq(trng4xx_dev.irq, trng4xx_irq_handler,
			 0, "TRNG", NULL);
	if (rc != 0) {
		printk(KERN_ERR "failed to register interrupt IRQ %d\n",
			trng4xx_dev.irq);
		return rc;
	}
	return 0;
}

int trng4xx_hw_deinit(void)
{
	free_irq(trng4xx_dev.irq, NULL);
	return 0;
}

static irqreturn_t trng4xx_irq_handler(int irq, void *id)
{
	/* TRNG Alarm Counter overflow */
	trng4xx_chk_overflow();
	return IRQ_HANDLED;
}

/**
 * TRNG Functions
 *
 */
static void trng4xx_chk_overflow(void)
{
	/* TRNG Alarm Counter overflow */
	int     rc;
	u32    val;
	struct trng4xx_cfg {
		u32    ring1_delay_sel :3;
		u32    ring2_delay_sel :3;
		u32    reset_cnt       :6;
	} __attribute__((packed));


	rc = trng4xx_hw_read32(TRNG4XX_ALARMCNT_ADDR, &val);
	if (rc != 0)
		return;

	if (val > 128) {
		struct trng4xx_cfg *trng4xx_cfg;

		/* Alarm count is half, reset it */
		rc = trng4xx_hw_read32(TRNG4XX_CFG_ADDR, &val);
		if (rc != 0)
			return;
		trng4xx_cfg = (struct trng4xx_cfg *) &val;
		++trng4xx_cfg->ring1_delay_sel;
		trng4xx_cfg->ring2_delay_sel =
				(~trng4xx_cfg->ring1_delay_sel) & 0x07;

		rc = trng4xx_hw_write32(TRNG4XX_CFG_ADDR, val);
		if (rc != 0)
			return;
		trng4xx_hw_write32(TRNG4XX_ALARMCNT_ADDR, 0x00000000);
		if (rc != 0)
			return;
	}
}

#define MAX_TRY     3
int	trng4xx_random(u32 *rand_val)
{
	u32	val = 0;
	int	rc;
	u16     try_cnt = 0;
	
	down(&trng4xx_dev.access_prot);
	do {
		rc = trng4xx_hw_read32(TRNG4XX_STATUS_ADDR, &val);
		if (rc != 0)
			goto err;
	} while ((val & TRNG4XX_STATUS_BUSY) && ++try_cnt <= MAX_TRY);
	if (val & TRNG4XX_STATUS_BUSY) {
		rc = -EINPROGRESS;
		goto err;
	}
	rc = trng4xx_hw_read32(TRNG4XX_OUTPUT_ADDR, rand_val);

err:
	up(&trng4xx_dev.access_prot);
	return rc;
}
EXPORT_SYMBOL_GPL(trng4xx_random);

static int trng4xx_data_present(struct hwrng *rng, int wait)
{
	struct trng4xx_dev *dev = (struct trng4xx_dev *) rng->priv;
	int     i;
	u32 	val;

	down(&trng4xx_dev.access_prot);
	for (i = 0; i < 20; i++) {
		udelay(10);
		trng4xx_hw_read32(TRNG4XX_STATUS_ADDR, &val);
		if (!(val & TRNG4XX_STATUS_BUSY)) {
			trng4xx_hw_read32(TRNG4XX_OUTPUT_ADDR, &dev->datum);
			break;
		}
		if (!wait)
			break;
	}
	up(&trng4xx_dev.access_prot);
	return (val & TRNG4XX_STATUS_BUSY) ? 0 : 1;
}

static int trng4xx_data_read(struct hwrng *rng, u32 *data)
{
	struct trng4xx_dev *dev = (struct trng4xx_dev *) rng->priv;

	*data = dev->datum;
	//printk("*data = 0x%08x\n", *data);
	return 4;
}

static int trng4xx_init(struct hwrng *rng)
{
	return trng4xx_hw_init();
}

static void trng4xx_cleanup(struct hwrng *rng)
{
	trng4xx_hw_deinit();
}

static struct hwrng trng4xx_func = {
	.name		= "ppc4xx-trng",
	.init		= trng4xx_init,
	.cleanup	= trng4xx_cleanup,
	.data_present	= trng4xx_data_present,
	.data_read	= trng4xx_data_read,
	.priv		= (unsigned long) &trng4xx_dev,
};

/**
 * Setup Driver with platform registration
 *
 */
static int __devinit trng4xx_probe(struct of_device *ofdev,
				     const struct of_device_id *match)
{
	struct hal_config hw_cfg;
	int 	rc;

	hw_cfg.ofdev = ofdev;
	rc = trng4xx_config_set(&hw_cfg);
	if (rc != 0)
		return rc;

	TRNG4XX_LOG("AMCC 4xx TRNG v%s @0x%02X_%08X size %d IRQ %d\n",
		TRNG4XX_VER_STR,
		(u32) (trng4xx_dev.res.start >> 32),
		(u32) trng4xx_dev.res.start,
		(u32) (trng4xx_dev.res.end - trng4xx_dev.res.start + 1),
		trng4xx_dev.irq);

	init_MUTEX(&trng4xx_dev.access_prot);

	rc = hwrng_register(&trng4xx_func);
	if (rc) {
		printk(KERN_ERR
			"AMCC 4xx TRNG registering failed error %d\n", rc);
		goto err;
	}

	return rc;

err:
	trng4xx_pka_config_clear();
	return rc;
}

static int __devexit trng4xx_remove(struct of_device *dev)
{
	hwrng_unregister(&trng4xx_func);
	trng4xx_pka_config_clear();
	return 0;
}

static struct of_device_id trng4xx_match[] = {
	{ .compatible      = "ppc4xx-trng", },
	{ .compatible      = "amcc,ppc4xx-trng", },
	{ },
};

static struct of_platform_driver trng4xx_driver = {
	.name		= "ppc4xx-trng",
	.match_table	= trng4xx_match,
	.probe		= trng4xx_probe,
	.remove		= trng4xx_remove,
};

static int __init mod_init(void)
{
  return of_register_platform_driver(&trng4xx_driver);
}

static void __exit mod_exit(void)
{
	of_unregister_platform_driver(&trng4xx_driver);
}

module_init(mod_init);
module_exit(mod_exit);

MODULE_DESCRIPTION("AMCC 4xx True Random Number Generator");
MODULE_LICENSE("GPL");
