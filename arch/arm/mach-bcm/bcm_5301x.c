/*
 * Broadcom BCM470X / BCM5301X ARM platform code.
 *
 * Copyright 2013 Hauke Mehrtens <hauke@hauke-m.de>
 *
 * Licensed under the GNU/GPL. See COPYING for details.
 */
#include <linux/of_platform.h>
#include <asm/hardware/cache-l2x0.h>

#include <linux/of_address.h>
#include <asm/mach/arch.h>
#include <asm/io.h>

static void venom_restart(enum reboot_mode mode, const char *cmd)
{
	void __iomem *base;
	struct device_node *np_wdog;

	np_wdog = of_find_compatible_node(NULL, NULL, "iproc-wdt");
	if (!np_wdog) {
		pr_emerg("Couldn't find brcm,iproc-wdt\n");
		return;
	}

	base = of_iomap(np_wdog, 0);
	if (!base) {
		pr_emerg("Couldn't map brcm,iproc-wdt\n");
		return;
	}

	/* Make the watchdog reset us in one tick */
	writel(1, base + 0x80);

	/* Wait for reset */
	while (1);
}

static const char *const mr26_dt_compat[] __initconst = {
	"meraki,venom",
	NULL,
};

DT_MACHINE_START(mr26, "Meraki MR26")
	.l2c_aux_val	= 0,
	.l2c_aux_mask	= ~0,
	.dt_compat	= mr26_dt_compat,
	.restart	= venom_restart,
MACHINE_END

static const char *const espresso_dt_compat[] __initconst = {
	"meraki,espresso",
	NULL,
};

DT_MACHINE_START(espresso, "Meraki Espresso")
	.l2c_aux_val	= 0,
	.l2c_aux_mask	= ~0,
	.dt_compat	= espresso_dt_compat,
	.restart	= venom_restart,
MACHINE_END

static const char *const bcm5301x_dt_compat[] __initconst = {
	"brcm,bcm4708",
	NULL,
};

DT_MACHINE_START(BCM5301X, "BCM5301X")
	.l2c_aux_val	= 0,
	.l2c_aux_mask	= ~0,
	.dt_compat	= bcm5301x_dt_compat,
MACHINE_END
