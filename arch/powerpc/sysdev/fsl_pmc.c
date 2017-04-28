/*
 * Suspend/resume support
 *
 * Copyright 2009  MontaVista Software, Inc.
 * Copyright 2010-2012 Freescale Semiconductor Inc.
 *
 * Author: Anton Vorontsov <avorontsov@ru.mvista.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <linux/init.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/export.h>
#include <linux/suspend.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/of_platform.h>
#include <linux/pm.h>
#include <asm/cacheflush.h>

#include <sysdev/fsl_soc.h>

struct pmc_regs {
	__be32 devdisr;
	__be32 devdisr2;
	__be32 res1;
	__be32 res2;
	__be32 powmgtcsr;
#define POWMGTCSR_SLP		0x00020000
#define POWMGTCSR_DPSLP		0x00100000
	__be32 res3[2];
	__be32 pmcdr;
};

static struct pmc_regs __iomem *pmc_regs;
static unsigned int pmc_flag;

#define PMC_SLEEP	0x1
#define PMC_DEEP_SLEEP	0x2

static int pmc_suspend_enter(suspend_state_t state)
{
	int ret = 0;

	switch (state) {
#ifdef CONFIG_PPC_85xx
	case PM_SUSPEND_MEM:
#ifdef CONFIG_SPE
#endif

		pr_debug("%s: Entering deep sleep\n", __func__);

		local_irq_disable();
		mpc85xx_enter_deep_sleep(get_immrbase(), POWMGTCSR_DPSLP);

		pr_debug("%s: Resumed from deep sleep\n", __func__);
		break;
#endif

	case PM_SUSPEND_STANDBY:
		local_irq_disable();
		flush_dcache_L1();

		setbits32(&pmc_regs->powmgtcsr, POWMGTCSR_SLP);
		/* At this point, the CPU is asleep. */

		/* Upon resume, wait for SLP bit to be clear. */
		ret = spin_event_timeout(
			(in_be32(&pmc_regs->powmgtcsr) & POWMGTCSR_SLP) == 0,
			10000, 10);
		if (!ret) {
			pr_err("%s: timeout waiting for SLP bit "
				"to be cleared\n", __func__);
			ret = -EINVAL;
		}
		break;

	default:
		ret = -EINVAL;

	}
	return ret;
}

static int pmc_suspend_valid(suspend_state_t state)
{
	if (((pmc_flag & PMC_SLEEP) && (state == PM_SUSPEND_STANDBY)) ||
	    ((pmc_flag & PMC_DEEP_SLEEP) && (state == PM_SUSPEND_MEM)))
		return 1;
	else
		return 0;
}

static const struct platform_suspend_ops pmc_suspend_ops = {
	.valid = pmc_suspend_valid,
	.enter = pmc_suspend_enter,
};

static int pmc_probe(struct platform_device *pdev)
{
	struct device_node *np = pdev->dev.of_node;

	pmc_regs = of_iomap(np, 0);
	if (!pmc_regs)
		return -ENOMEM;

	pmc_flag = PMC_SLEEP;
	if (of_device_is_compatible(np, "fsl,mpc8536-pmc"))
		pmc_flag |= PMC_DEEP_SLEEP;

	if (of_device_is_compatible(np, "fsl,p1022-pmc"))
		pmc_flag |= PMC_DEEP_SLEEP;

	suspend_set_ops(&pmc_suspend_ops);

	pr_info("Freescale PMC driver\n");
	return 0;
}

static const struct of_device_id pmc_ids[] = {
	{ .compatible = "fsl,mpc8548-pmc", },
	{ .compatible = "fsl,mpc8641d-pmc", },
	{ },
};

static struct platform_driver pmc_driver = {
	.driver = {
		.name = "fsl-pmc",
		.owner = THIS_MODULE,
		.of_match_table = pmc_ids,
	},
	.probe = pmc_probe,
};

static int __init pmc_init(void)
{
	return platform_driver_register(&pmc_driver);
}
device_initcall(pmc_init);
