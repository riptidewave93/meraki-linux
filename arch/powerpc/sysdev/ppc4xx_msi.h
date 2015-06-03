/*
 * Copyright (C) 2009 Applied Micro Circuits Corporation.
 *
 * Author: Tirumala Marri <tmarri@amcc.com>
 *             Feng Kan <fkan@amcc.com>
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 2 of the
 * License.
 */
#ifndef __PPC4XX_MSI_H__
#define __PPC4XX_MSI_H__

#include <asm/msi_bitmap.h>

#define PEIH_TERMADH	0x00
#define PEIH_TERMADL	0x08
#define PEIH_MSIED	0x10
#define PEIH_MSIMK	0x18
#define PEIH_MSIASS	0x20
#define PEIH_FLUSH0	0x30
#define PEIH_FLUSH1	0x38
#define PEIH_CNTRST	0x48

#define PE_IHS1_OFF	0x00
#define PE_IHS2_OFF	0x01

/* Adjust pattern for PCIE MSI*/
#define MSI_DATA_PATTERN 0x00004440
#define MSI_MASK_PATTERN 0x0000ffe0

#define PE_IHS2_BREV_EN 0x02

#define U64_TO_U32_LOW(val)     ((u32)((val) & 0x00000000ffffffffULL))
#define U64_TO_U32_HIGH(val)    ((u32)((val) >> 32))

#ifdef CONFIG_PHYS_64BIT
#define RES_TO_U32_LOW(val)    U64_TO_U32_LOW(val)
#define RES_TO_U32_HIGH(val)    U64_TO_U32_HIGH(val)
#else
#define RES_TO_U32_LOW(val)     (val)
#define RES_TO_U32_HIGH(val)    (0)
#endif


struct ppc4xx_msi {
	struct irq_host *irqhost;
	unsigned long cascade_irq;
	u32 msi_addr_lo;
	u32 msi_addr_hi;
	void __iomem *msi_regs;
	u32 feature;
	struct msi_bitmap bitmap;
	u32 sdr_base;
};

#ifdef CONFIG_405EX
#define NR_MSI_IRQS 14
#elif defined(CONFIG_460EX) || defined (CONFIG_460SX)
#define NR_MSI_IRQS 4
#elif defined (CONFIG_APM82181)
#define NR_MSI_IRQS 8
#endif

#endif /* __PPC4XX_MSI_H__ */
