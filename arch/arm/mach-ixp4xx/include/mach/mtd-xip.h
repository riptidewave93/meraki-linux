/*
 * arch/arm/mach-ixp4xx/include/mach/mtd-xip.h
 *
 * IXP4XX MTD primitives for XIP support.
 *
 * Copyright (C) 2014 Cisco, Inc.
 *
 * based on arch/arm/mach-sa1100/include/mach/mtd-xip.h
 * Copyright:	(C) 2004 MontaVista Software, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#ifndef __ARCH_IXP4XX_MTD_XIP_H__
#define __ARCH_IXP4XX_MTD_XIP_H__

#include <mach/hardware.h>

#define xip_irqpending()	(*IXP4XX_ICIP & *IXP4XX_ICMR)

/* we sample OSTS and convert desired delta to usec (1/67 ~= 1000000/66666666)*/
#define xip_currtime()		(*IXP4XX_OSTS)
#define xip_elapsed_since(x)	(unsigned)((*IXP4XX_OSTS - (x)) / 67)

#endif
