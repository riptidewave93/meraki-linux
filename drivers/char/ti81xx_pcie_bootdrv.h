/*
 *  This file contains register and offsets data for TI816X/TI814X PCIe
 *  Endpoints. Also provides basic ioctls to perform device boot from
 *  application. Details applicable for both TI816X and TI814X devices are
 *  prefixed with TI81XX/ti81xx while specific defails use TI816X/TI814X as
 *  applicable.
 *
 * Copyright (C) 2010, Texas Instruments, Incorporated
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation version 2.
 *
 * This program is distributed "as is" WITHOUT ANY WARRANTY of any
 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef __TI81XX_PCIE_BOOTDRV_H__
#define __TI81XX_PCIE_BOOTDRV_H__


#define TI81XX_PCIE_MODFILE		"ti81xx_pcie_ep"
#define TI81XX_DEV_COUNT		1

/*
 * These are the default PCI IDs for TI816X/TI814X device. Update these as
 * applicable or add Subsystem IDs and corresponding check in the code if you
 * have multiple TI816X/TI814X/TI813X based PCIe cards.
 */
#define TI81XX_PCI_VENDOR_ID               0x104c
#define TI816X_PCI_DEVICE_ID               0xb800
#define TI814X_PCI_DEVICE_ID               0xb801
#define TI813X_PCI_DEVICE_ID               0xb802

/*
 * NOTE: Most of the addresses/offsets listed in subsequent sections are not
 * relied upon by the driver - it is the boot application's responsibility to
 * consider them and use mmap accordingly. For now, these are maintained here
 * for completeness.
 *
 * Memory addresses on TI816X EP involved in handshaking with the EP Boot
 * monitor (and later, bootloader) code.
 *
 * Following assumptions are made for boot operation:
 * - Bootrom on EP has set up at least 3 BARs - BAR0, BAR1 and BAR2 to have
 *   access to PCIe application registers (4KB), OCMC RAM (256KB OCMC1 on TI816X
 *   and 128KB OCMC0 on TI814X) and DDR (8MB) respectively.
 * - All of the above BARs are enabled and host has allocated memory resources
 *   for the same.
 * - Application maintains sanity of inbound mappings before accessing BAR2.
 *   E.g.:
 *	- Load bootscript after moving BAR2 window to 4MB offset in DDR.
 *	- Load kernel after moving BAR2 window to 8MB offset in DDR (1MB room
 *	for U-Boot execution from 0x80700000).
 *	- Load RAMDISK after moving the 8MB window to 16MB offset from DDR base.
 * - All image offsets are assumed to be ___page___aligned___
 */
#define TI816X_EP_OCMC_BASE		0x40400000
#define TI814X_EP_OCMC_BASE		0x40300000
#define TI81XX_EP_EMIF_BASE		0x80000000
#define TI816X_EP_UBOOT_ADDR		TI816X_EP_OCMC_BASE
#define TI814X_EP_UBOOT_STG1_ADDR	TI814X_EP_OCMC_BASE
#define TI814X_EP_UBOOT_ADDR		0x80800000
#define TI81XX_EP_BOOTSCRIPT_ADDR	0x80400000		/* EMIF0/1 */
#define TI81XX_EP_KERNEL_ADDR		0x80900000		/* EMIF0/1 */
#define TI81XX_EP_RAMDISK_ADDR		0x81000000		/* EMIF0/1 */
#define TI816X_EP_BOOTFLAG_ADDR		0x4043FFFC		/* OCMC */
#define TI814X_EP_BOOTFLAG_ADDR		0x4031B7FC		/* OCMC */

/*
 * Memory window offsets and ranges:
 * - U-Boot	BAR1	offset=0	size=256KB	<--- TI816X
 * - U-Boot	BAR0	offset=0	size=112KB	<--- TI814X 1st Stage
 * - U-Boot	BAR2	offset=0	size=256KB	<--- TI814X
 * - Kernel	BAR2	offset=0	size=2MB
 * - Kernel	BAR2	offset=0	size=8MB
 * - RAMDISK	BAR2	offset=0	size=8MB	<--- TI816X
 * - RAMDISK	BAR2	offset=0	size=16MB	<--- TI814X
 * - Bootflag	BAR1	offset=3FFFC	size=4B		<--- TI816X
 * - Bootflag	BAR1	offset=1B7FC	size=4B		<--- TI814X
 *
 * ___Note___: Moving inbound offset inside the available BAR window (e.g.,
 * offset > 0) will lead to lesser space available for transfer within the
 * particular BAR window as the respective MAX_SIZE constraints will still need
 * t be obeyed. Also, ensure that the offsets are 32-bit word aligned.
 */
#define TI814X_EP_UBOOT_STG1_OFFSET	0
#define TI814X_EP_UBOOT_STG1_MAX_SIZE	0x20000

#define TI81XX_EP_UBOOT_OFFSET		0
#define TI81XX_EP_UBOOT_MAX_SIZE	0x40000

#define TI81XX_EP_BOOTSCRIPT_OFFSET	0
#define TI81XX_EP_BOOTSCRIPT_MAX_SIZE	0x00200000

#define TI81XX_EP_KERNEL_OFFSET		0
#define TI81XX_EP_KERNEL_MAX_SIZE	0x00800000

#define TI81XX_EP_RAMDISK_OFFSET	0
#define TI816X_EP_RAMDISK_MAX_SIZE	0x00800000
#define TI814X_EP_RAMDISK_MAX_SIZE	0x01000000

#define TI816X_EP_BOOTFLAG_OFFSET	0x0003FFFC

#define TI814X_EP_BOOTFLAG_OFFSET	0x0001B7FC

/*
 * Inbound translation offsets - we only touch these for BAR2 to load  kernel
 * and ramdisk. In addition, 2nd stage U-Boot in case of TI814X is loaded
 * through BAR2 window.
 */
#define TI814X_EP_UBOOT_STG1_IB_OFFSET	((TI814X_EP_UBOOT_STG1_ADDR)	\
						- (TI814X_EP_UBOOT_STG1_OFFSET))
#define TI816X_EP_UBOOT_IB_OFFSET	((TI816X_EP_UBOOT_ADDR)		\
						- (TI81XX_EP_UBOOT_OFFSET))
#define TI814X_EP_UBOOT_IB_OFFSET	((TI814X_EP_UBOOT_ADDR)		\
						- (TI81XX_EP_UBOOT_OFFSET))
#define TI81XX_EP_BOOTSCRIPT_IB_OFFSET	((TI81XX_EP_BOOTSCRIPT_ADDR)	\
						- (TI81XX_EP_BOOTSCRIPT_OFFSET))
#define TI81XX_EP_KERNEL_IB_OFFSET	((TI81XX_EP_KERNEL_ADDR)	\
						- (TI81XX_EP_KERNEL_OFFSET))
#define TI81XX_EP_RAMDISK_IB_OFFSET	((TI81XX_EP_RAMDISK_ADDR)	\
						- (TI81XX_EP_RAMDISK_OFFSET))

#ifdef __KERNEL__

#include <linux/types.h>

#define CMD_STATUS			0x004
#define CFG_SETUP			0x008
#define IB_BAR(x)			(0x300 + (0x10 * x))
#define IB_START_LO(x)			(0x304 + (0x10 * x))
#define IB_START_HI(x)			(0x308 + (0x10 * x))
#define IB_OFFSET(x)			(0x30c + (0x10 * x))
#define GPR0				0x70

/* Application command register values */
#define DBI_CS2_EN_VAL			BIT(5)
#define IB_XLAT_EN_VAL			BIT(2)

#endif  /*  __KERNEL__  */

/**
 * ti81xx_bar_info - PCI Base Address Register information
 * @num: BAR register index - 0 to 5
 * @addr: For 'SET' operations, contains ti816x internal address to translate
 * @size: Size allocated for this BAR (only usd for GET operation)
 * @mode: mode of operation of PCIE core in ti81xx i.e. 32/64 bit.
 * this BAR access to. For 'GET'' operations, contains the (host) physical
 * address assigned to this BAR.
 * in case of 64 bit BARS.
 * @num: 0, 2 or 4. ( region 0, region 2, region 4).
 */
struct ti81xx_bar_info {
	u32 num;
	unsigned long addr;
	u32 size;
	u32 mode;
};

/* IOCTLs defined for the application as well as driver */
#define TI81XX_PCI_SET_DWNLD_DONE	_IOW('P', 1, unsigned int)
#define TI81XX_PCI_SET_BAR_WINDOW	_IOW('P', 2, unsigned int)
#define TI81XX_PCI_GET_BAR_INFO		_IOWR('P', 3, unsigned int)
#define TI81XX_PCI_GET_DEVICE_ID	_IOR('P', 4, unsigned int)
#define TI81XX_PCI_GET_BOOT_INFO	_IOR('P', 5, unsigned int)

#endif
