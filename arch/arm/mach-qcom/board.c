/* Copyright (c) 2010-2014 The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/init.h>
#include <linux/dma-mapping.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_platform.h>
#include <linux/clocksource.h>
#include <linux/clk-provider.h>
#include <asm/io.h>

#include <asm/mach/arch.h>

static void meraki_wdt_restart(const char *compat)
{
	void __iomem *base;
	struct device_node *np_wdog;
	struct resource *res;
	struct platform_device *pdev;

	np_wdog = of_find_compatible_node(NULL, NULL, compat);
	if (!np_wdog) {
		pr_emerg("Couldn't find %s\n",compat);
		return;
	}

	pdev = of_find_device_by_node(np_wdog);
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	base = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR(base)) {
		 pr_emerg("Couldn't map %s\n",compat);
		 return;
	}

	#define WDT_RST	0x0
	#define WDT_EN		0x8
	#define WDT_STS 	0xc
	#define WDT_BARK_TIME	0x14
	#define WDT_BITE_TIME	0x24

	/* Make the watchdog reset us in one tick */
	writel(0, base + WDT_EN);
	writel(1, base + WDT_RST);
	writel(2*4196, base + WDT_BARK_TIME);
	writel(4196, base + WDT_BITE_TIME);
	writel(1, base + WDT_EN);
	wmb();

	/* Wait for reset */
	while (1);
}

static void cryptid_wdt_restart(enum reboot_mode mode, const char *cmd)
{
   meraki_wdt_restart("qcom,kpss-wdt-ipq8064");
}

static void insect_wdt_restart(enum reboot_mode mode, const char *cmd)
{
   meraki_wdt_restart("qcom,kpss-wdt-ipq40xx");
}

static const char * const qcom_dt_match[] __initconst = {
	"qcom,apq8064",
	"qcom,apq8074-dragonboard",
	"qcom,apq8084",
	"qcom,ipq8062",
	"qcom,ipq8064",
	"qcom,msm8660-surf",
	"qcom,msm8960-cdp",
	NULL
};

DT_MACHINE_START(QCOM_DT, "Qualcomm (Flattened Device Tree)")
	.dt_compat = qcom_dt_match,
MACHINE_END

static int __init qcom_atomic_pool_size_set(void)
{
#define ATOMIC_DMA_COHERENT_POOL_SIZE	SZ_2M

	init_dma_coherent_pool_size(ATOMIC_DMA_COHERENT_POOL_SIZE);

	return 0;
}

/*
 * This should happen before atomic_pool_init(), which is a
 * postcore_initcall.
 */
core_initcall(qcom_atomic_pool_size_set);
static const char * const qcom_qca_dt_match[] __initconst = {
	"qcom,qca961x-r3pc",
	NULL
};

static void __init global_counter_enable(void)
{
	struct device_node *node;
	void __iomem *base;

	node = of_find_compatible_node(NULL, NULL, "qcom,qca-gcnt");
	if (!node) {
		pr_err("%s:can't find node\n", __func__);
		return;
	}

	base = of_iomap(node, 0);
	of_node_put(node);
	if (!base) {
		pr_err("%s:no regs for global counter\n", __func__);
		return;
	}

	writel_relaxed(1, base);
	mb(); /* memory barrier */
	iounmap(base);

#ifdef CONFIG_COMMON_CLK
	of_clk_init(NULL);
#endif
	clocksource_probe();
}

DT_MACHINE_START(QCOM_QCA_DT, "Qualcomm (Flattened Device Tree)")
	.dt_compat = qcom_qca_dt_match,
	.init_time = global_counter_enable,
	.restart = insect_wdt_restart,
MACHINE_END

static const char * const mr42_dt_match[] __initconst = {
	"meraki,mr42",
	NULL
};

DT_MACHINE_START(YOWIE, "Meraki MR42 (Yowie)")
	.dt_compat = mr42_dt_match,
	.nr = 0x136B,
	.restart = cryptid_wdt_restart,
MACHINE_END

static const char * const mr42e_dt_match[] __initconst = {
	"meraki,mr42e",
	NULL
};

DT_MACHINE_START(CITIZENSNIPS, "Meraki MR42E (Citizen Snips)")
	.dt_compat = mr42e_dt_match,
	.nr = 0x136B,
	.restart = cryptid_wdt_restart,
MACHINE_END

static const char * const mr52_dt_match[] __initconst = {
	"meraki,mr52",
	NULL
};

DT_MACHINE_START(BIGFOOT, "Meraki MR52 (Bigfoot)")
	.dt_compat = mr52_dt_match,
	.nr = 0x136B,
	.restart = cryptid_wdt_restart,
MACHINE_END

static const char * const mr53_dt_match[] __initconst = {
	"meraki,mr53",
	NULL
};

DT_MACHINE_START(SASQUATCH, "Meraki MR53 (Sasquatch)")
	.dt_compat = mr53_dt_match,
	.nr = 0x136B,
	.restart = cryptid_wdt_restart,
MACHINE_END

static const char * const mr53e_dt_match[] __initconst = {
	"meraki,mr53e",
	NULL
};

DT_MACHINE_START(DOCTOROCTOPUS, "Meraki MR53E (Doctor Octopus)")
	.dt_compat = mr53e_dt_match,
	.nr = 0x136B,
	.restart = cryptid_wdt_restart,
MACHINE_END

static const char * const mr84_dt_match[] __initconst = {
	"meraki,mr84",
	NULL
};

DT_MACHINE_START(WOOKIE, "Meraki MR84 (Wookie)")
	.dt_compat = mr84_dt_match,
	.nr = 0x136B,
	.restart = cryptid_wdt_restart,
MACHINE_END

static const char * const mr30h_dt_match[] __initconst = {
	"meraki,mr30h",
	NULL
};

DT_MACHINE_START(NOISY_CRICKET, "Meraki MR30H")
	.dt_compat = mr30h_dt_match,
	.restart = insect_wdt_restart,
MACHINE_END

static const char * const mr33_dt_match[] __initconst = {
	"meraki,mr33",
	NULL
};

DT_MACHINE_START(STINKBUG, "Meraki MR33")
	.dt_compat = mr33_dt_match,
	.restart = insect_wdt_restart,
MACHINE_END

static const char * const mr74_dt_match[] __initconst = {
	"meraki,mr74",
	NULL
};

DT_MACHINE_START(LADYBUG, "Meraki MR74")
	.dt_compat = mr74_dt_match,
	.restart = insect_wdt_restart,
MACHINE_END

static const char * const maggot_dt_match[] __initconst = {
	"meraki,maggot",
	NULL
};

DT_MACHINE_START(MAGGOT, "Meraki Maggot")
	.dt_compat = maggot_dt_match,
	.restart = insect_wdt_restart,
MACHINE_END

static const char * const dungbeetle_dt_match[] __initconst = {
	"meraki,dungbeetle",
	NULL
};

DT_MACHINE_START(DUNGBEETLE, "Meraki Dungbeetle")
	.dt_compat = dungbeetle_dt_match,
	.restart = insect_wdt_restart,
MACHINE_END

static const char * const pinocchio_dt_match[] __initconst = {
	"meraki,pinocchio",
	NULL
};

DT_MACHINE_START(PINOCCHIO, "Meraki Pinocchio")
	.dt_compat = pinocchio_dt_match,
	.restart = insect_wdt_restart,
MACHINE_END
