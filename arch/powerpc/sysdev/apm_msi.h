/*
 * Copyright (C) 2010 Applied Micro Circuit Corporation
 *
 * Author: Feng Kan <fkan@apm.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 2 of the
 * License.
 *
 */
#ifndef _PPC_SYSDEV_APM_MSI_H
#define _PPC_SYSDEV_APM_MSI_H

#include <asm/msi_bitmap.h>

#if 0
#define NR_MSI_REG		8
#define IRQS_PER_MSI_REG	32
#endif
#define NR_MSI_IRQS	(NR_MSI_REG * IRQS_PER_MSI_REG)

#define APM_IP_MASK		0x0000000F
#define APM_IP_MPIC		0x00000001
#define APM_IP_MPIC_CASCADE	0x00000002
#define APM_IP_UIC		0x00000003

struct apm_msi {
	struct irq_host *irqhost;

	unsigned long cascade_irq;

	u32 msi_addr_lo;
	u32 msi_addr_hi;
	void __iomem *msi_regs;
	u32 feature;
	u32 total_irqs;
	u32 num_irqs_per_reg;

	struct msi_bitmap bitmap;
};

#endif /* _PPC_SYSDEV_APM_MSI_H */

