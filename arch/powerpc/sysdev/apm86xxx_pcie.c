/**
 * APM APM86xxx PCIe Header File
 *
 * Copyright (c) 2010 Applied Micro Circuits Corporation.
 * All rights reserved. Loc Ho <lho@apm.com>, Feng Kan <fkan@apm.com>.
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
 * This module implements PCIE functionality. This setups and
 * configures PCIE controllers as either root complex or endpoint.
 *
 */

#include <linux/kernel.h>
#include <linux/pci.h>
#include <linux/init.h>
#include <linux/of.h>
#include <linux/bootmem.h>
#include <linux/delay.h>
#include <linux/clk.h>

#include <asm/io.h>
#include <asm/pci-bridge.h>
#include <asm/dcr.h>
#include <asm/dcr-regs.h>
#include <mm/mmu_decl.h>

#include <asm/ipp.h>
#include <linux/sysdev.h>
#include "apm86xxx_pcie.h"

#undef APM_PCIE_DEBUG
#undef CONFIG_PCIE_ENUM

#if 0
# define PCIE_CSR_DEBUG(fmt, ...) printk(KERN_INFO fmt, ##__VA_ARGS__); 
#else
# define PCIE_CSR_DEBUG(fmt, ...)
#endif

#if 0
# define PCIE_CFG_DEBUG(fmt, ...) printk(KERN_INFO fmt, ##__VA_ARGS__); 
#else
# define PCIE_CFG_DEBUG(fmt, ...)
#endif

#if 0
# define PCIE_CFGRAW_DEBUG(fmt, ...) printk(KERN_INFO fmt, ##__VA_ARGS__); 
#else
# define PCIE_CFGRAW_DEBUG(fmt, ...)
#endif

#ifdef APM_PCIE_DEBUG
# define PCIE_DEBUG(fmt, ...)	  printk(KERN_INFO fmt, ##__VA_ARGS__); 
#else
# define PCIE_DEBUG(x, ...)
#endif

#ifndef HIDWORD
#	define MAKE_U64(h, l)		((((u64) (h)) << 32) | (l))
#	define HIDWORD(x)		((u32) (((u64)(x)) >> 32))
#	define LODWORD(x)		((u32) ((x) & 0xFFFFFFFF))
#endif

#define RES_SIZE(res)			((res)->end - (res)->start + 1)

#define MAX_PCIE_BUS_MAPPED		0x40

enum
{
	PTYPE_ENDPOINT		    	= 0x0,
	PTYPE_LEGACY_ENDPOINT		= 0x1,
	PTYPE_ROOT_PORT		    	= 0x4,

	LNKW_X1			        = 0x1,
	LNKW_X4			        = 0x4,
	LNKW_X8			        = 0x8
};

#define PCIE_BAR_EN	0x1
static struct apm_pciex_port *apm_pciex_ports;
static unsigned int apm_pciex_port_cnt = 0;
static int dma_offset_set = 0;

static int pcie_is_buggy_csr(void)
{
	u32 data;
	
	apm86xxx_read_scu_reg(SCU_SOC_EFUSE_ADDR, &data);
	/* Extract the SW key of the EFUSE */
	switch ((EFUSE0_RD(data) >> 5) & 0xFE) {
	case 0x00:	/* APM862xx */
	case 0x01:	/* APM862xx */
	case 0x80:	/* APM862xx lite */
	case 0x40:	/* APM863xx */
	case 0x41:	/* APM863xx */
	case 0xC0:	/* APM867xx */
	case 0xC1:	/* APM867xx */
		return 1;
	default:
		return 0;
	}
 }

/* PCIE Out/In to CSR */
int pcie_csr_outle32(void * addr, u32 val)
{
	PCIE_CSR_DEBUG("PCIE CSR WR: 0x%p value: 0x%08X\n", addr, val);
	if ((((u32) addr) & 0x7fff) >= 0x1C00 &&
	    (((u32) addr) & 0x7fff) <= 0x1CFF &&
	    pcie_is_buggy_csr()) {
		out_be32(addr, val);
	} else {
		out_le32(addr, val);
	}
	asm volatile("msync");
	return 0;
}

int pcie_csr_inle32(void *addr, u32 *val)
{
	*val = in_le32(addr);
	PCIE_CSR_DEBUG("PCIE CSR RD: 0x%p value: 0x%08X\n", addr, *val);
	return 0;
}

static inline int pcie_csr_outbe32(void * addr, u32 val)
{
	PCIE_CSR_DEBUG("PCIE CSR(BE) WR: 0x%p value: 0x%08X\n", addr, val);
	out_be32(addr, val);
	return 0;
}

static inline int pcie_csr_inbe32(void * addr, u32 *val)
{
	*val = in_be32(addr);
	PCIE_CSR_DEBUG("PCIE CSR(BE) RD: 0x%p value: 0x%08X\n", addr, *val);
	return 0;
}

static inline int csr_outbe32(void * addr, u32 val)
{
	PCIE_CSR_DEBUG("SCU CSR WR: 0x%p value: 0x%08X\n", addr, val);
	out_be32(addr, val);
	return 0;
}

static inline int csr_inbe32(void * addr, u32 *val)
{
	*val = in_be32(addr);
	PCIE_CSR_DEBUG("SCU CSR RD: 0x%p value: 0x%08X\n", addr, *val);
	return 0;
}

/* PCIE Configuration Out/In */
static inline int pcie_cfg_out32(void * addr, u32 val)
{
	out_le32(addr, val);
	PCIE_CFGRAW_DEBUG("PCIE CFG WR: 0x%p value: 0x%08X (0x%08X)\n", 
			addr, val, in_le32(addr));
	return 0;
}

static inline int pcie_cfg_out16(void *addr, u16 val)
{
	u32 addr32 = (u32) addr & ~0x3;
	u32 val32  = in_le32((void *) addr32);
	
	switch ((u32) addr & 0x3) {
	case 2:
		val32 &= ~0xFFFF0000;
		val32 |= (u32) val << 16;
		break;
	case 0:
	default:
		val32 &= ~0xFFFF;
		val32 |= val;
		break;
	}
	PCIE_CFGRAW_DEBUG("PCIE CFG WR16: 0x%p value: 0x%04X (0x%08X 0x%08X)\n", 
			addr, val, addr32, val32); 
	out_le32((void *) addr32, val32);
	return 0;
}

static inline int pcie_cfg_out8(void * addr, u8 val)
{
	u32 addr32 = (u32) addr & ~0x3;
	u32 val32  = in_le32((void *) addr32);
	
	switch ((u32) addr & 0x3) {
	case 0:
		val32 &= ~0xFF;
		val32 |= val;
		break;
	case 1:
		val32 &= ~0xFF00;
		val32 |= (u32) val << 8;
		break;
	case 2:
		val32 &= ~0xFF0000;
		val32 |= (u32) val << 16;
		break;
	case 3:
	default:
		val32 &= ~0xFF000000;
		val32 |= (u32) val << 24;
		break;
	}
	PCIE_CFGRAW_DEBUG("PCIE CFG WR8: 0x%p value: 0x%04X (0x%08X 0x%08X)\n", 
			addr, val, addr32, val32); 
	out_le32((void *) addr32, val32);
	return 0;
}

static inline int pcie_cfg_in32(void * addr, u32 *val)
{
	*val = in_le32(addr);
	PCIE_CFGRAW_DEBUG("PCIE CFG RD: 0x%p value: 0x%08X\n", addr, *val);
	return 0;
}

static inline int pcie_cfg_in16(void * addr, u16 *val)
{
	u32 addr32 = (u32) addr & ~0x3;
	u32 val32  = in_le32((void *) addr32);
	
	switch ((u32) addr & 0x3) {
	case 2:
		*val = val32 >> 16;
		break;
	case 0:
	default:
		*val = val32;
		break;
	}
	PCIE_CFGRAW_DEBUG("PCIE CFG RD16: 0x%p value: 0x%04X (0x%08X 0x%08X)\n", 
			addr, *val, addr32, val32); 
	return 0;
}

static inline int pcie_cfg_in8(void * addr, u8 *val)
{
	u32 addr32 = (u32) addr & ~0x3;
	u32 val32  = in_le32((void *) addr32);
	
	switch ((u32) addr & 0x3) {
	case 3:
		*val = val32 >> 24;
		break;
	case 2:
		*val = val32 >> 16;
		break;
	case 1:
		*val = val32 >> 8;
		break;
	case 0:
	default:
		*val = val32;
		break;
	}
	PCIE_CFGRAW_DEBUG("PCIE CFG RD8: 0x%p value: 0x%02X (0x%08X 0x%08X)\n", 
			addr, *val, addr32, val32);
	return 0;
}

int pcie_host2port(struct pci_controller *host)
{
	return host->indirect_type;
}

static void __iomem *apm_pciex_get_cfg_base(struct apm_pciex_port *port,
					struct pci_bus *bus,
					unsigned int devfn)
{
	int relbus;

	/* Remove the casts when we finally remove the stupid volatile
	 * in struct pci_controller
	 */
	if (bus->number == port->hose->first_busno)
		return (void __iomem *) port->hose->cfg_addr;

	relbus = bus->number - (port->hose->first_busno + 1);
	return (void __iomem *)port->hose->cfg_data +
		((relbus  << 20) | (devfn << 12));
}

static int apm_pciex_validate_bdf(struct apm_pciex_port *port,
				   struct pci_bus *bus, unsigned int devfn)
{
	static int message = 0;

	/* Endpoint can not generate upstream(remote) config cycles */
	if (port->endpoint && bus->number != port->hose->first_busno) {
		PCIE_DEBUG("PCIE%d: invalude bdf %d %d %d\n", 
			port->index, port->endpoint, bus->number, 
			port->hose->first_busno);
		return PCIBIOS_DEVICE_NOT_FOUND;
	}
	/* Check we are within the mapped range */
	if (bus->number > port->last_busno) {
		if (!message) {
			printk(KERN_WARNING "Warning! Probing bus %u"
			       " out of range !\n", bus->number);
			message++;
		}
		return PCIBIOS_DEVICE_NOT_FOUND;
	}

	/* The root complex has only one device / function */
	if (bus->number == port->hose->first_busno && devfn != 0)
		return PCIBIOS_DEVICE_NOT_FOUND;
			
	/* The other side of the RC has only one device as well */
	if (bus->number == (port->hose->first_busno + 1) &&
	    PCI_SLOT(devfn) != 0)
		return PCIBIOS_DEVICE_NOT_FOUND;
	
	/* Check if we have a link */
	if ((bus->number != port->hose->first_busno) && !port->link) {
		PCIE_DEBUG("PCIE%d: invalid bdf %d %d link %d\n",
			port->index, bus->number, port->hose->first_busno,
			port->link);
		return PCIBIOS_DEVICE_NOT_FOUND;
	}
	
	return 0;
}

/**
 * @brief Read from PCIe configuration space
 * @param bus		PCIE bus
 * @param devfn		Device/function number to read from
 * @param offset	Offset CSR
 * @param len		# of byte (1, 2, or 4)
 * @param val		A pointer to location to store value
 * @return PCIBIOS_SUCCESSFUL is returned.
 *
 */
static int pcie_read_config(struct pci_bus *bus, unsigned int devfn, 
			    int offset, int len, u32 *val)
{
	struct pci_controller *hose = (struct pci_controller *) bus->sysdata;
	struct apm_pciex_port *port = &apm_pciex_ports[hose->indirect_type];
	void __iomem *addr;
	unsigned int b, d, f;
	unsigned int l2me;
	u8 val8;
	u16 val16;

	if (apm_pciex_validate_bdf(port, bus, devfn) != 0)
		return PCIBIOS_DEVICE_NOT_FOUND;

	b = bus->number;
	d = PCI_SLOT(devfn);
	f = PCI_FUNC(devfn);	
	if (bus->number == hose->first_busno) {
		pcie_csr_outle32((u8 *) hose->cfg_addr + RTDID_ADDR, 0);
	} else if (bus->number == (hose->first_busno + 1)) {
		pcie_csr_outle32((u8 *) hose->cfg_addr + RTDID_ADDR,
			(b << 24) | (d << 19) | (f << 16));
	} else {
		pcie_csr_outle32((u8 *) hose->cfg_addr + RTDID_ADDR,
			(b << 24) | (d << 19) | (f << 16) | (1 << 15));
	}
		         
	/* 
	 * Disable PLB read data error, since enumeration 
	 * could cause machine checks
	 */
	asm("isync");
	mtdcr(DCRN_L2C_ADDR, DCRN_L2MCRER);
	l2me = mfdcr(DCRN_L2C_DATA);
	l2me &= ~L2MCSR_MSTRDE;
	mtdcr(DCRN_L2C_DATA, l2me);

	asm("isync");
	addr = apm_pciex_get_cfg_base(port, bus, devfn);
	switch(len) {
	case 1:
		pcie_cfg_in8(addr + offset, &val8);
		*val = val8;
		PCIE_CFG_DEBUG("PCIE%d CFG RD: bus %d dev %d func %d "
			"addr 0x%p val 0x%02X\n", 
			port->index, b, d, f, addr + offset, (u8) *val);	
		break;
	case 2:
		pcie_cfg_in16(addr + offset, &val16);
		*val = val16;
		PCIE_CFG_DEBUG("PCIE%d CFG RD: bus %d dev %d func %d "
			"addr 0x%p val 0x%04X\n", 
			port->index, b, d, f, addr + offset, (u16) *val);	
		break;
	default:
		pcie_cfg_in32(addr + offset, val);
		PCIE_CFG_DEBUG("PCIE%d CFG RD: bus %d dev %d func %d "
			"addr 0x%p val 0x%08X\n", 
			port->index, b, d, f, addr + offset, *val);	
		break;
	}

	/* 
	 * Reenable PLB read data error reporting 
	 */
	asm("isync");
	mtdcr(DCRN_L2C_ADDR, DCRN_L2MCSR);
	mtdcr(DCRN_L2C_DATA, mfdcr(DCRN_L2C_DATA) | L2MCSR_MSTRDE);
	mtdcr(DCRN_L2C_ADDR, DCRN_L2MCRER);
	l2me |= L2MCSR_MSTRDE;
	mtdcr(DCRN_L2C_DATA, l2me);
	return PCIBIOS_SUCCESSFUL;
}

/**
 * @brief Write to PCIe configuration space
 * @param bus		PCIE bus
 * @param devfn		Device/function number to read from
 * @param offset	Offset CSR
 * @param len		# of byte (1, 2, or 4)
 * @param val		A value to write
 * @return PCIBIOS_SUCCESSFUL is returned.
 *
 */
static int pcie_write_config(struct pci_bus *bus, unsigned int devfn,
			     int offset, int len, u32 val)
{
	struct pci_controller *hose = (struct pci_controller *) bus->sysdata;
	struct apm_pciex_port *port = &apm_pciex_ports[hose->indirect_type];
	void __iomem *addr;
	unsigned int b, d, f;
	unsigned int l2me;

	if (apm_pciex_validate_bdf(port, bus, devfn) != 0)
		return PCIBIOS_DEVICE_NOT_FOUND;

	b = bus->number;
	d = PCI_SLOT(devfn);
	f = PCI_FUNC(devfn);
	if (bus->number == hose->first_busno) {
		pcie_csr_outle32((u8 *) hose->cfg_addr + RTDID_ADDR, 0);
	} else if (bus->number == (hose->first_busno + 1)) {
		pcie_csr_outle32((u8 *) hose->cfg_addr + RTDID_ADDR,
			(b << 24) | (d << 19) | (f << 16));
	} else {
		pcie_csr_outle32((u8 *) hose->cfg_addr + RTDID_ADDR,
			(b << 24) | (d << 19) | (f << 16) | (1 << 15));
	}	         

	/* 
	 * Disable PLB read data error, since enumeration 
	 * could cause machine checks
	 */
	asm("isync");
	mtdcr(DCRN_L2C_ADDR, DCRN_L2MCRER);
	l2me = mfdcr(DCRN_L2C_DATA);
	l2me &= ~L2MCSR_MSTRDE;
	mtdcr(DCRN_L2C_DATA, l2me);

	asm("isync");
	addr = apm_pciex_get_cfg_base(port, bus, d);
	switch(len) {
	case 1:
		PCIE_CFG_DEBUG("PCIE%d CFG WR: bus %d dev %d func %d "
			"addr 0x%p val 0x%02X\n", 
			port->index, b, d, f, addr + offset, (u8) val);	
		pcie_cfg_out8(addr + offset, (u8) val);
		break;
	case 2:
		PCIE_CFG_DEBUG("PCIE%d CFG WR: bus %d dev %d func %d "
			"addr 0x%p val 0x%04X\n", 
			port->index, b, d, f, addr + offset, (u16) val);	
		pcie_cfg_out16(addr + offset, (u16) val);
		break;
	default:
		PCIE_CFG_DEBUG("PCIE%d CFG WR: bus %d dev %d func %d "
			"addr 0x%p val 0x%08X\n", 
			port->index, b, d, f, addr + offset, val);	
		pcie_cfg_out32(addr + offset, val);
		break;
	}

	/* 
	 * Reenable PLB read data error reporting 
	 */
	asm("isync");
	mtdcr(DCRN_L2C_ADDR, DCRN_L2MCSR);
	mtdcr(DCRN_L2C_DATA, mfdcr(DCRN_L2C_DATA) | L2MCSR_MSTRDE);
	mtdcr(DCRN_L2C_ADDR, DCRN_L2MCRER);
	l2me |= L2MCSR_MSTRDE;
	mtdcr(DCRN_L2C_DATA, l2me);
	return PCIBIOS_SUCCESSFUL;
}

static struct pci_ops apm_pcie_ops =
{
	.read  = pcie_read_config,
	.write = pcie_write_config
};

static void fixup_apm_pcie_bridge(struct pci_dev *dev)
{
	struct pci_controller *hose;
	int i;

	if (dev->devfn != 0 || dev->bus->self != NULL)
		return;

	hose = pci_bus_to_host(dev->bus);
	if (hose == NULL)
		return;

	if (!of_device_is_compatible(hose->dn, "apm,apm-pciex"))
		return;

	/* Hide the PCI host BARs from the kernel as their content doesn't
	 * fit well in the resource management
	 */
	for (i = 0; i < DEVICE_COUNT_RESOURCE; i++) {
		dev->resource[i].start = dev->resource[i].end = 0;
		dev->resource[i].flags = 0;
	}

	printk(KERN_INFO "PCI: Hiding APM host bridge resources %s\n",
	       pci_name(dev));
}
DECLARE_PCI_FIXUP_HEADER(PCI_ANY_ID, PCI_ANY_ID, fixup_apm_pcie_bridge);

int apm_pcie_is_host(struct pci_controller *hose)
{
	u32  	val32;
	
	pcie_csr_inle32((u8 *) hose->cfg_addr + PCIECORECTRL_ADDR, &val32);	
	switch(val32 & DEVICETYPE_F6_MASK) {
	case PTYPE_ENDPOINT:
	case PTYPE_LEGACY_ENDPOINT:
		return 0;
	case PTYPE_ROOT_PORT:
	default:
		return 1;
	}
}

#define MOSYS_OFFSET 0x4000

/* r/w to mosys csr (indirect), takes data directly, not pointer */
int mosys_wr_op(void * addr, u32 offset, u32 data)
{
	int rc = 0;
	u32 pcs_wr_done;

	out_be32((void *) (addr + MOSYS_OFFSET + X1_PCSCSR_REGADDR_ADDR), offset);
	out_be32((void *) (addr + MOSYS_OFFSET + X1_PCSCSR_REGWRDATA_ADDR), data);
	out_be32((void *) (addr + MOSYS_OFFSET + X1_PCSCSR_REGCMD_ADDR), 
			X1_MGMT_PCS_REG_WR_F2_MASK);
	pcs_wr_done  = 0;
	while (pcs_wr_done != 0x2) {
		pcs_wr_done = in_be32((void *) (addr + MOSYS_OFFSET + 
					X1_PCSCSR_REGCMDDONE_ADDR));
		pcs_wr_done &= X1_MGMT_PCS_REG_WR_DONE_F2_MASK;
	}

	return rc;
}

int mosys_rd(void * addr, u32 offset, u32 *pdata)
{
	int rc = 0;
	u32 pcs_rd_done;

	out_be32((void *) (addr + MOSYS_OFFSET + X1_PCSCSR_REGADDR_ADDR), 
			offset);
	out_be32((void *) (addr + MOSYS_OFFSET + X1_PCSCSR_REGCMD_ADDR), 
			X1_MGMT_PCS_REG_RD_F2_MASK);
	pcs_rd_done  = 0;

	while (pcs_rd_done != 0x1) {
		pcs_rd_done = in_be32((void *) 
				(addr + MOSYS_OFFSET + X1_PCSCSR_REGCMDDONE_ADDR));
		pcs_rd_done &= X1_MGMT_PCS_REG_RD_DONE_F2_MASK;
	}

	*pdata = in_be32((void *) (addr + MOSYS_OFFSET + 
				X1_PCSCSR_REGRDDATA_ADDR));
	return rc;
}

int mosys_wrx4_op(void *addr, u32 offset, u32 data)
{
	int rc = 0;
	u32 pcs_wr_done;

	out_be32((void *) (addr + MOSYS_OFFSET + X4_PCSCSR_REGADDR_ADDR), 
				offset);
	out_be32((void *) (addr + MOSYS_OFFSET + X4_PCSCSR_REGWRDATA_ADDR), 
				data);
	out_be32((void *) (addr + MOSYS_OFFSET + X4_PCSCSR_REGCMD_ADDR),
			X1_MGMT_PCS_REG_WR_F2_MASK);
	pcs_wr_done  = 0;

	while(pcs_wr_done != 0x2) {
		pcs_wr_done = in_be32((void *) (addr + MOSYS_OFFSET +
					X4_PCSCSR_REGCMDDONE_ADDR));
		pcs_wr_done &= X1_MGMT_PCS_REG_WR_DONE_F2_MASK;
	}

	return rc;
}

int mosys_rdx4(void *addr, u32 offset, u32 *pdata)
{
	int rc = 0;
	u32 pcs_rd_done;

	out_be32((void *) (addr + MOSYS_OFFSET + X4_PCSCSR_REGADDR_ADDR), 
				offset);
	out_be32((void *) (addr + MOSYS_OFFSET + X4_PCSCSR_REGCMD_ADDR),
			X1_MGMT_PCS_REG_RD_F2_MASK);
	pcs_rd_done  = 0;

	while(pcs_rd_done != 0x1) {
		pcs_rd_done = in_be32((void *) (addr + MOSYS_OFFSET +
					X4_PCSCSR_REGCMDDONE_ADDR));
		pcs_rd_done &= X1_MGMT_PCS_REG_RD_DONE_F2_MASK;
	}

	*pdata = in_be32((void *) (addr + MOSYS_OFFSET + X4_PCSCSR_REGRDDATA_ADDR));

	return rc;
}

int apm_init_pcie(struct apm_pciex_port *port)
{
	void *csr_base = port->hose->cfg_addr;

	int type_root_endpoint = port->endpoint ? PTYPE_ENDPOINT : 
						  PTYPE_ROOT_PORT; 
	u32 val;
	u32 val_phy_reset_on;
	struct clk *clk;
	int rc;
	#define PLL_DELAY_UNIT_US	100
	int timeout_us = PLL_DELAY_UNIT_US*100;

	if (!port->phy_reset) {
		/* On some PCIE devices, they behave oddly if the PCIE
		 * controller/phy is reset twice - boot loader and here.
		 * Skip PHY init is set in DTS 
		 */
                printk(KERN_INFO "PCIE%d: skip PHY init\n", port->index);
		goto skip_phy_init;
	}
		
	/* Reset and enable PCIEx clock */
	switch (port->index) {
	case 0:
		clk = clk_get(NULL, "pcie1");
		if (IS_ERR(clk))
			return -ENODEV;
		clk_enable(clk);
		rc = apm86xxx_disable_mem_shutdown((u32 __iomem *)(csr_base+
						APM_GLBL_DIAG_OFFSET+
						PCIE_CFG_MEM_RAM_SHUTDOWN_ADDR),
						PCIE1_F2_MASK);
		if (rc)
			return rc;
		break;
	case 1:
		clk = clk_get(NULL, "pcie2");
		if (IS_ERR(clk))
			return -ENODEV;
		clk_enable(clk);
		rc = apm86xxx_disable_mem_shutdown((u32 __iomem *)(csr_base+
						APM_GLBL_DIAG_OFFSET+
						PCIE_CFG_MEM_RAM_SHUTDOWN_ADDR),
						PCIE2_F2_MASK);
		if (rc)
			return rc;
		break;
	case 2:
		clk = clk_get(NULL, "pcie3");
		if (IS_ERR(clk))
			return -ENODEV;
		clk_enable(clk);
		rc = apm86xxx_disable_mem_shutdown((u32 __iomem *)(csr_base+
						APM_GLBL_DIAG_OFFSET+
						PCIE_CFG_MEM_RAM_SHUTDOWN_ADDR),
						PCIE3_F2_MASK);
		if (rc)
			return rc;
		break;
	}

	/* For PCIE1, it is shared with SATA. Select the MUX for PCIE1.
	   For PCIE0, it is shared with TRACE. Select the MUX for PCIE0. */
	if (port->index == 0 || port->index == 1) {
                /* SerDes definition for PCIE2, PCIE1, and SATA are identical*/
                csr_inbe32(csr_base + 0x4000 + X1_PCSCSR_CTL0_ADDR, &val);
                val = X1_PCIE_MODE_SEL0_SET(val, 1);
                csr_outbe32(csr_base + 0x4000 + X1_PCSCSR_CTL0_ADDR, val);
        	asm volatile("msync");
	        csr_inbe32(csr_base + 0x4000 + X1_PCSCSR_CTL0_ADDR, &val);
	}	

	/* Set all CSR registers from offset 0 - 1FFF to little endian */
	csr_outbe32(csr_base + ENDIAN_ADDR, 0xFFFFFFFF);
	asm volatile("msync");
	csr_inbe32(csr_base + ENDIAN_ADDR, &val);

	/* Configure PCIEx as root/endpoint complex mode */
	pcie_csr_inle32(csr_base + DEVICETYPE_ADDR, &val);
	val = DEVICETYPE_SET(val, type_root_endpoint);
	pcie_csr_outle32(csr_base + DEVICETYPE_ADDR, val);
	
	switch (port->index) {
	case 0: /* 1-lane */
	case 1:
		/* Configure PHY */
		val = X1_PHY_PCS_RX0_SIGDET0_F2_WR(1) |
			X1_PHY_TX_TERM_CAL_IN0_F2_WR(0xF) |
			X1_MAC_PCS_X2_MODE0_WR(0) |
			X1_MAC_PCS_PLL_PCS_DIV0_F2_WR(0x13) |
			X1_MAC_PCS_PLL_DIV0_WR(0x31) |
			X1_MAC_PCS_PLL_OP_RANGE0_WR(0) |
			X1_MAC_PCS_PLL_REFCLK_SEL0_F2_WR(0) |
			X1_RESET0_F4_WR(1) |
			X1_INIT_CTLIFC_USE_MGMT0_F2_WR(0) |
			X1_INIT_CTLIFC_PIPE_COMPLIANT0_F2_WR(1) |
			X1_INIT_REFCLK_AC_MODE0_F2_WR(0) |
			X1_INIT_TX_TERM_SEL0_F2_WR(1) |
			X1_PCIE_MODE_SEL0_WR(1);
		pcie_csr_outbe32(csr_base + 0x4000 + X1_PCSCSR_CTL0_ADDR, val);
		pcie_csr_outbe32(csr_base + 0x4000 + X1_PCSCSR_CTL1_ADDR,
				X1_MAC_PCS_RX0_ENCDET1_WR(0) |
				X1_MAC_PCS_SATA0_HOST1_WR(1) |
				X1_MAC_PCS_RX0_EDET_LB1_WR(0) |
				X1_MAC_PCS_RX0_EDET_EIE1_WR(0) |
				X1_MAC_PCS_RX0_EDET_TS1_WR(0) |
				X1_MAC_PCS_RX0_EDET_SKP1_WR(0) |
				X1_MAC_PCS_CTL0_OP_RANGE1_F2_WR(1) |
				X1_MAC_PCS_CTL0_SMPL_RATE1_WR(1) |
				X1_MAC_PCS_CTL0_RX_LANE_DIV1_WR(0) |
				X1_MAC_PCS_CTL0_TX_LANE_DIV1_WR(0) |
				X1_MAC_PCS_CTL0_LANE_PWR_OFF1_WR(0) |
				X1_MAC_PCS_CTL0_RX_MODE1_F2_WR(0x4) |
				X1_MAC_PCS_CTL0_TX_MODE1_F2_WR(0x4));
		asm volatile("msync");
		udelay(2000);		/* 2 ms as required by MoSys PHY */

		/* Saving X1_PCSCSR_CTL0_ADDR value before we cleared reset bit */
		val_phy_reset_on = val;         /* Remove PHY out of reset */		
		/* Remove PHY out of reset */
		val &= ~X1_RESET0_F4_MASK;
		pcie_csr_outbe32(csr_base + 0x4000 + X1_PCSCSR_CTL0_ADDR, val);
		asm volatile("msync");
		udelay(2000);		/* 2 ms as required by MoSys PHY */
		
		/* Due to bug in PHY, again put a PHY in to reset */
		pcie_csr_outbe32(csr_base + 0x4000 + X1_PCSCSR_CTL0_ADDR, 
				val_phy_reset_on);
		asm volatile("msync");		
		udelay(2000);		/* 2 ms as required by MoSys PHY */
		
		/* Due to bug in PHY, Remove PHY out of the reset again */
		pcie_csr_outbe32(csr_base + 0x4000 + X1_PCSCSR_CTL0_ADDR, val);
		asm volatile("msync");
		udelay(2000);		/* 2 ms as required by MoSys PHY */

                /* Poll PLL for lock and ready */
		pcie_csr_inbe32(csr_base + 0x4000 +
				X1_PCSCSR_STATUS1_ADDR, &val);				
                do {
                        pcie_csr_inbe32(csr_base + 0x4000 +
                                        X1_PCSCSR_STATUS1_ADDR, &val);
                        val &= X1_SDS_PCS_CLOCK_READY1_F2_MASK |
                                X1_SDS_PCS_PLL_LOCK1_F2_MASK;
                        if (val == (X1_SDS_PCS_CLOCK_READY1_F2_MASK |
                                    X1_SDS_PCS_PLL_LOCK1_F2_MASK))
                                break;
                        udelay(PLL_DELAY_UNIT_US);
                        timeout_us -= PLL_DELAY_UNIT_US;
                } while (timeout_us > 0);
                
#if defined(CONFIG_APM867xx)
		mosys_rd(csr_base, 0x5800D, &val);
                PCIE_DEBUG("PCIE%d: MOSYS RD 0x5800D val: 0x%08X\n", 
                	port->index, val);
                val = 0x206F;
                PCIE_DEBUG("PCIE%d: MOSYS WR 0x5800D val: 0x%08X\n", 
                	port->index, val);
                mosys_wr_op(csr_base, 0x5800D, val);
		mosys_rd(csr_base, 0x5800D, &val);
                PCIE_DEBUG("PCIE%d: MOSYS RD 0x5800D val: 0x%08X\n", 
                	port->index, val);
#endif
                break;
        case 2: /* 4-lane */
                /* Configure PHY - reset PHY and take out of reset */
                pcie_csr_inbe32(csr_base + 0x4000+X4_PCSCSR_COMMONCTL0_ADDR, &val);
		asm volatile("msync");
		udelay(2000);		/* 2 ms as required by MoSys PHY */
		
		val_phy_reset_on = val;
		/* Remove PHY out of reset */
		val &= ~X4_RESET0_F2_MASK;
                pcie_csr_outbe32(csr_base + 0x4000+X4_PCSCSR_COMMONCTL0_ADDR, val);
                asm volatile("msync");
		udelay(2000);		/* 2 ms as required by MoSys PHY */

		/* Due to bug in PHY, Again put a PHY in to reset */
		pcie_csr_outbe32(csr_base + 0x4000+X4_PCSCSR_COMMONCTL0_ADDR, 
					val_phy_reset_on);
		asm volatile("msync");		
		udelay(2000);		/* 2 ms as required by MoSys PHY */
		
		/* Due to bug in PHY, Remove PHY out of the reset again */
		pcie_csr_outbe32(csr_base + 0x4000+X4_PCSCSR_COMMONCTL0_ADDR, 
					val);
		asm volatile("msync");
		udelay(2000);		/* 2 ms as required by MoSys PHY */
		
		/* Due to timing requirement, the RX_CLK in the pcs must be inverted */
                mosys_rdx4(csr_base, 0x318001, &val);
                PCIE_DEBUG("PCIE%d: mosys rd at addr 0x318001: %x\n", 
                	port->index, val);
                val |= 1<<15;
                PCIE_DEBUG("PCIE%d: modified val: %x\n", port->index, val);
                mosys_wrx4_op(csr_base, 0x3f8001, val);
                mosys_rdx4(csr_base, 0x318001, &val);
		
                /* Poll PLL for lock and ready */
                do {
                        pcie_csr_inbe32(csr_base + 0x4000 + X4_PCSCSR_STATUS1_ADDR,
                                        &val);
                        val &= X1_SDS_PCS_CLOCK_READY1_F2_MASK |
                                X1_SDS_PCS_PLL_LOCK1_F2_MASK;
                        if (val == (X1_SDS_PCS_CLOCK_READY1_F2_MASK |
                                X1_SDS_PCS_PLL_LOCK1_F2_MASK))
                                break;
                        udelay(PLL_DELAY_UNIT_US);
                        timeout_us -= PLL_DELAY_UNIT_US;
                } while (timeout_us > 0);
		break;

	}
	if (timeout_us <= 0) {
		printk("Unable to lock PLL for PCIE port %d\n", port->index);
		return -1;
	}

skip_phy_init:

        /* Enable ECC generation and checking */
        pcie_csr_outle32(csr_base + PCIE_ADV_CAP_CTRL_ADDR, 0x1E0);

        /* Enable all PCIE event, errror, and its interrupt  */
        pcie_csr_outle32(csr_base + EVENTCAPTUREENABLE_ADDR, 0xFFFFFFFF);
        pcie_csr_outle32(csr_base + EVENTINTERUPTSTATUSMASK_ADDR, 0x00000000);
        pcie_csr_outle32(csr_base + ERRORCAPTUREENABLE_ADDR, 0xFFFFFFFF);
        pcie_csr_outle32(csr_base + ERRORINTERRUPTSTATUSMASK_ADDR, 0x00000000);
        pcie_csr_outle32(csr_base + INTXSTATUSMASK_ADDR, 0x00000000);
        /* Enable Correctable Error, Non-Fatal Error reporting,
           Fatal error reporting, unsupported request reporting,
           set max_Payload_size to 2. Enable Extended Tag field,
           Phantom function, AUX power PM, enable snoop Not require,
           and set MAX_read_request_size to 2. */
        pcie_csr_outle32(csr_base + PCIE_DEV_CTRL_ADDR, 0x2F0F);

#if 0
        switch (port->index) {
        case 2: /* 4-lane */
		/* it appears this is not needed */
		#define PCIE_PORT_FREQ_LAT	0x070c
                pcie_csr_inle32(csr_base + PCIE_PORT_FREQ_LAT, &val);
                val &= 0xFF0000FF;
                val |= 0xc8c800;
                pcie_csr_outle32(csr_base + PCIE_PORT_FREQ_LAT, val);
                pcie_csr_inle32(csr_base + PCIE_PORT_FREQ_LAT, &val);
		
                break;
        default:
                break;
        }
#endif

	#define PCIE_GEN2_CTRL_ADDR	0x080c
	pcie_csr_inle32(csr_base + PCIE_GEN2_CTRL_ADDR, &val);
	mb();	
	pcie_csr_inle32(csr_base + PCIE_GEN2_CTRL_ADDR, &val);
	pcie_csr_outle32(csr_base + PCIE_GEN2_CTRL_ADDR, val | 0x20000);

	return 0;
}

/**
 * @brief  This function setups PCIE controller endpoint for operation.
 * @param  host   Host controller structure
 * @param  port   PCIE controller port
 * @return 0 if successfull, < 0 if failed
 *
 */
static int apm_pcie_setup_rootpoint(struct pci_controller *hose, int portno)
{
	struct apm_pcie_map_tbl *hb = &apm_pciex_ports[portno].pcie_map_tbl;
	struct apm_pciex_port *port = &apm_pciex_ports[portno];
	void  *csr_base = hose->cfg_addr;
	u64   val64;
	u32   val32;
	int   link_wait_us;

	hose->ops = &apm_pcie_ops;
	
	/* Set bus numbers on our root port */
	pcie_csr_inle32(csr_base + PCI_PRIMARY_BUS, &val32);
	val32 &= 0xFF000000;
	pcie_csr_outle32(csr_base + PCI_PRIMARY_BUS, val32 |
			 hose->last_busno << 16 |
			 (hose->first_busno + 1) << 8 |
			 hose->first_busno);
	pcie_csr_inle32(csr_base + PCI_PRIMARY_BUS, &val32);

	/* Set up outbound PCIE translation Memory Region #1:
	 * Translate from host memory address to PCIE address
	 */
	pcie_csr_outle32(csr_base + OMR1BARL_ADDR, hb->ob_mem_addr[0].lo);
	pcie_csr_outle32(csr_base + OMR1BARH_ADDR, hb->ob_mem_addr[0].hi);
	if (hb->ob_mem_addr[0].size)
		val64 = ~(hb->ob_mem_addr[0].size - 1) | VAL_F2_MASK;
	else
		val64 = 0;
	pcie_csr_outle32(csr_base + OMR1MSKL_ADDR, LODWORD(val64));
	pcie_csr_outle32(csr_base + OMR1MSKH_ADDR, HIDWORD(val64));
	pcie_csr_outle32(csr_base + POM1L_ADDR, hb->ob_mem_addr[0].pcie_lo);
	pcie_csr_outle32(csr_base + POM1H_ADDR, hb->ob_mem_addr[0].pcie_hi);
	PCIE_DEBUG("PCIE%d: Outbound 0 HBF 0x%02X_%08X -> PCIE 0x%02X_%08X "
		   "Mask 0x%08X_%08X\n", portno,
		hb->ob_mem_addr[0].hi, hb->ob_mem_addr[0].lo,
		hb->ob_mem_addr[0].pcie_hi, hb->ob_mem_addr[0].pcie_lo,
		HIDWORD(val64), LODWORD(val64));		 

	/* Set up outbound PCIE translation memory Region #2:
	 * Translate from host memory address (ie. 0x07_XXXX_XXXX) to PCIE
	 * address at 0x00_0000_0000 + size of 1st memory map.
	 */
	pcie_csr_outle32(csr_base + OMR2BARL_ADDR, hb->ob_mem_addr[1].lo);
	pcie_csr_outle32(csr_base + OMR2BARH_ADDR, hb->ob_mem_addr[1].hi);
	if (hb->ob_mem_addr[1].size > 0)
		val64 = ~(hb->ob_mem_addr[1].size - 1) | VAL_F3_MASK;
	else
		val64 = 0;
	pcie_csr_outle32(csr_base + OMR2MSKL_ADDR, LODWORD(val64));
	pcie_csr_outle32(csr_base + OMR2MSKH_ADDR, HIDWORD(val64));
	pcie_csr_outle32(csr_base + POM2L_ADDR, hb->ob_mem_addr[1].pcie_lo);
	pcie_csr_outle32(csr_base + POM2H_ADDR, hb->ob_mem_addr[1].pcie_hi);
	PCIE_DEBUG("PCIE%d: Outbound 1 HBF 0x%02X_%08X -> PCIE 0x%02X_%08X "
		   "Mask 0x%08X_%08X\n", portno,
		hb->ob_mem_addr[1].hi, hb->ob_mem_addr[1].lo,
		hb->ob_mem_addr[1].pcie_hi, hb->ob_mem_addr[1].pcie_lo,
		HIDWORD(val64), LODWORD(val64));		 

	/* Set up outbound PCIE translation I/O Region #3:
	 * Translate from host memory address (ie. 0x07_XXXX_XXXX) to PCIE
	 * address at 0x00_0000_0000 + size of 1st & 2nd memory map.
	 */
	pcie_csr_outle32(csr_base+OMR3BARL_ADDR, hb->ob_mem_addr[2].pcie_lo);
	pcie_csr_outle32(csr_base+OMR3BARH_ADDR, hb->ob_mem_addr[2].pcie_hi);
	if (hb->ob_mem_addr[2].size > 0)
		val64 = ~(hb->ob_mem_addr[2].size - 1) | VAL_F4_MASK | IO_MASK;
	else
		val64 = 0;
	pcie_csr_outle32(csr_base + OMR3MSKL_ADDR, LODWORD(val64));
	pcie_csr_outle32(csr_base + OMR3MSKH_ADDR, HIDWORD(val64));
	pcie_csr_outle32(csr_base + POM3L_ADDR, hb->ob_mem_addr[2].pcie_lo);
	pcie_csr_outle32(csr_base + POM3H_ADDR, hb->ob_mem_addr[2].pcie_hi);
	PCIE_DEBUG("PCIE%d: Outbound 2 HBF 0x%02X_%08X -> PCIE 0x%02X_%08X "
		   "Mask 0x%08X_%08X\n", portno,
		hb->ob_mem_addr[2].hi, hb->ob_mem_addr[2].lo,
		hb->ob_mem_addr[2].pcie_hi, hb->ob_mem_addr[2].pcie_lo,
		HIDWORD(val64), LODWORD(val64));		 

	/* Set up outbound Message Region: 2^7 - 2^15
	 * Translate from host memory address (ie. 0x07_XXXX_XXXX) to PCIE
	 * address at 0x00_0000_0000 + size of 1st & 2nd memory map + size
	 * of I/O region.
	 */
	pcie_csr_outle32(csr_base + MSGBARL_ADDR, hb->ob_msg_addr_lo);
	pcie_csr_outle32(csr_base + MSGBARH_ADDR, hb->ob_msg_addr_hi);
	if (hb->ob_msg_addr_size > 0x8000)
		val32 = 0x7FFF | VAL_F5_MASK;
	else if (hb->ob_msg_addr_size > 0)
		val32 = ((u32) ~(hb->ob_msg_addr_size - 1)) | VAL_F5_MASK;
	else
		val32 = 0;
	pcie_csr_outle32(csr_base + MSGMSK_ADDR, val32);
	pcie_csr_outle32(csr_base + POM4L_ADDR, hb->ob_msg_pcie_addr_lo);
	pcie_csr_outle32(csr_base + POM4H_ADDR, hb->ob_msg_pcie_addr_hi);
	PCIE_DEBUG("PCIE%d: Outbound Msg HBF 0x%02X_%08X -> PCIE 0x%02X_%08X "
		   "Mask 0x%08X\n", portno,
		hb->ob_msg_addr_hi, hb->ob_msg_addr_lo,
		hb->ob_msg_pcie_addr_hi, hb->ob_msg_pcie_addr_lo, val32);		 

	/* Set up outbound Configuration Region:
	 * Translate from host memory address (ie. 0x07_XXXX_XXXX) to PCIE
	 * address at 0x00_0000_0000 + size of 1st & 2nd memory map + size
	 * of I/O region + size of message region.
	 */
	pcie_csr_outle32(csr_base + CFGBARL_ADDR, hb->cfg_addr_lo);
	pcie_csr_outle32(csr_base + CFGBARH_ADDR, hb->cfg_addr_hi);
	if (hb->cfg_addr_size != 0)
		val32 = ((u32) ~(hb->cfg_addr_size - 1)) | VAL_F6_MASK;
	else
		val32 = 0;
	pcie_csr_outle32(csr_base + CFGMSK_ADDR, val32);
	pcie_csr_outle32(csr_base + POM5L_ADDR, hb->cfg_pcie_lo);
	pcie_csr_outle32(csr_base + POM5H_ADDR, hb->cfg_pcie_hi);
	PCIE_DEBUG("PCIE%d: Outbound Cfg HBF 0x%02X_%08X -> PCIE 0x%02X_%08X "
		   "Mask 0x%08X\n", portno,
		hb->cfg_addr_hi, hb->cfg_addr_lo,
		hb->cfg_pcie_hi, hb->cfg_pcie_lo, val32);	 

	/* Set up in bound PCIE Memory Region #1:
	*   Translate PCIE Address (ie. 0x00_0000_0000 - 0x0F_FFFF_FFFF) to
	*    AXI range (ie. 0x00_0000_0000 - 0x0F_FFFF_FFFF).
	*/
	pcie_csr_outle32(csr_base + PCI_BASE_ADDRESS_0,
		(hb->ib_mem_addr[0].pcie_lo & PCI_BASE_ADDRESS_MEM_MASK) 
		| PCI_BASE_ADDRESS_MEM_TYPE_64);
	pcie_csr_outle32(csr_base + PCI_BASE_ADDRESS_1, 
		hb->ib_mem_addr[0].pcie_hi);
	if (hb->ib_mem_addr[0].pcie_size >= (1024*1024))
		val64 = (hb->ib_mem_addr[0].pcie_size-1); 	/* min is 1 MB */
	else
		val64 = 0;
	pcie_csr_outle32(csr_base + BAR0_MSK_L_ADDR, LODWORD(val64));
	pcie_csr_outle32(csr_base + BAR0_MSK_H_ADDR, HIDWORD(val64));
	pcie_csr_outle32(csr_base + PIM1_1L_ADDR, hb->ib_mem_addr[0].pim1_lo);
	pcie_csr_outle32(csr_base + PIM1_1H_ADDR, hb->ib_mem_addr[0].pim1_hi);
	pcie_csr_outle32(csr_base + PIM1_2L_ADDR, hb->ib_mem_addr[0].pim2_lo);
	pcie_csr_outle32(csr_base + PIM1_2H_ADDR, hb->ib_mem_addr[0].pim2_hi);
	pcie_csr_outle32(csr_base + PIM1SL_ADDR, 
			LODWORD(hb->ib_mem_addr[0].pim_size));
	pcie_csr_outle32(csr_base + PIM1SH_ADDR, 
			HIDWORD(hb->ib_mem_addr[0].pim_size));
	PCIE_DEBUG("PCIE%d: Inbound 0 PCIE 0x%02X_%08X -> HBF 0x%02X_%08X "
		   "Mask 0x%08X_%08X\n"
		   "                                    -> 0x%02X_%08X\n",
		portno,
		hb->ib_mem_addr[0].pcie_hi, hb->ib_mem_addr[0].pcie_lo,
		hb->ib_mem_addr[0].pim1_hi, hb->ib_mem_addr[0].pim1_lo,
		HIDWORD(val64), LODWORD(val64),	 
		hb->ib_mem_addr[0].pim2_hi, hb->ib_mem_addr[0].pim2_lo);

	/* Set up in bound PCIE Memory Region #2 */
	pcie_csr_outle32(csr_base + IBAR2L_ADDR,
		(hb->ib_mem_addr[1].pcie_lo & PCI_BASE_ADDRESS_MEM_MASK) 
		| PCI_BASE_ADDRESS_MEM_TYPE_64);
	pcie_csr_outle32(csr_base + IBAR2H_ADDR, hb->ib_mem_addr[1].pcie_hi);
	if (hb->ib_mem_addr[1].pcie_size >= (1024*1024))
		val64 = ~(hb->ib_mem_addr[1].pcie_size - 1)
			| VAL_MASK; /* min is 1 MB */
	else
		val64 = 0;
	pcie_csr_outle32(csr_base + IR2MSKL_ADDR, LODWORD(val64));
	pcie_csr_outle32(csr_base + IR2MSKH_ADDR, HIDWORD(val64));
	pcie_csr_outle32(csr_base + PIM2_1L_ADDR, hb->ib_mem_addr[1].pim1_lo);
	pcie_csr_outle32(csr_base + PIM2_1H_ADDR, hb->ib_mem_addr[1].pim1_hi);					   
	pcie_csr_outle32(csr_base + PIM2_2L_ADDR, hb->ib_mem_addr[1].pim2_lo);
	pcie_csr_outle32(csr_base + PIM2_2H_ADDR, hb->ib_mem_addr[1].pim2_hi);
	pcie_csr_outle32(csr_base + PIM2SL_ADDR,
		     LODWORD(hb->ib_mem_addr[1].pim_size));
	pcie_csr_outle32(csr_base + PIM2SH_ADDR,
		     HIDWORD(hb->ib_mem_addr[1].pim_size));
	PCIE_DEBUG("PCIE%d: Inbound 1 PCIE 0x%02X_%08X -> HBF 0x%02X_%08X "
		   "Mask 0x%02X_%08X\n"
		   "                                    ->     0x%02X_%08X\n",
		portno,
		hb->ib_mem_addr[1].pcie_hi, hb->ib_mem_addr[1].pcie_lo,
		hb->ib_mem_addr[1].pim1_hi, hb->ib_mem_addr[1].pim1_lo,
		HIDWORD(val64), LODWORD(val64),	 
		hb->ib_mem_addr[1].pim2_hi, hb->ib_mem_addr[1].pim2_lo);

	/* Set up in bound PCIE I/O Region: Not used */
	pcie_csr_outle32(csr_base + IR3MSK_ADDR, 0);

	/* Expansion ROM Region #4 */
	pcie_csr_outle32(csr_base + PIM4L_ADDR, hb->ib_rom_addr_lo);
	pcie_csr_outle32(csr_base + PIM4H_ADDR, hb->ib_rom_addr_hi);
	if (hb->ib_rom_addr_size >= (2*1024))	/* min 2KB */
		val32 = ~(hb->ib_rom_addr_size - 1) |
			PCIE_BAR_EN;
	else
		val32 = 0;
	pcie_csr_outle32(csr_base + EXPROM_RC_MSK_ADDR, val32);

	/* Enable I/O, Mem, and Busmaster cycles
	   NOTE: The status register (higher 2 bytes) is not affected
		as we are writing 0's
	 */
	pcie_csr_inle32(csr_base + PCI_COMMAND, &val32);
	pcie_csr_outle32(csr_base + PCI_COMMAND, 
		val32 | PCI_COMMAND_IO | PCI_COMMAND_MEMORY | 
		PCI_COMMAND_MASTER);

	/* Set Class Code to PCI-PCI bridge and Revision Id to 1 */
	pcie_csr_outle32(csr_base + PCI_VENDOR_ID, 0x911010e8);
	pcie_csr_outle32(csr_base + PCI_CLASS_REVISION, 0x06040001);
	
	/* On APM863xx, the port 2 (x4) is actual an x1 port */ 
	if (is_apm863xx() && portno == 2) {
		pcie_csr_inle32(csr_base + PCIE_PORT_LINK_CTRL_ADDR, &val32);
		val32 &= ~0x3F0000;
		val32 |= 1 << 16;		/* Change to single x1 lane */	
		pcie_csr_outle32(csr_base + PCIE_PORT_LINK_CTRL_ADDR, val32);
		
		#define PCIE_GEN2_CTRL_ADDR	0x080c
		pcie_csr_inle32(csr_base + PCIE_GEN2_CTRL_ADDR, &val32);
		val32 &= ~0x1FF00;
		val32 |= 1 << 8;		/* Limit to single lane */
		pcie_csr_outle32(csr_base + PCIE_GEN2_CTRL_ADDR, val32);
	}	
#if 0
	/* Debugging only - set SPAM Master access error as CA instead UR */
	pcie_csr_inle32(csr_base + PCIE_ERR_MSK_ADDR, &val32);
	val32 &= ~0x300;
	pcie_csr_outle32(csr_base + PCIE_ERR_MSK_ADDR, val32);
#endif

	/* 
	 * Set the remote request size to dts value, if no dts value is
	 * present, then use the value of 2 which is 512 bytes
	 * 0 - 128B 1-256 2-512 3-1024 4-2048 5-4096
	 */
	pcie_csr_outle32(csr_base + MASTER_RESP_COMP_CTRL_REG0_ADDR, port->rmt_mrrqs);

retry_link:
	/* Set Core as Root Complex and Enable core Link establishment */
	pcie_csr_outle32(csr_base + PCIECORECTRL_ADDR, 
			APPLTSSMENABLE_MASK);

	/* Wait until PHY and Data Link are up */		
	port->link = 0;
	for (link_wait_us = APM_PCIE_LINK_WAIT_US; link_wait_us > 0; ) {
		pcie_csr_inle32(csr_base + PCIECORESTATUS_ADDR, &val32);
		val32 &= (RDHLLINKUP_MASK |
				XMLHLINKUP_MASK);
		if (val32 == (RDHLLINKUP_MASK |
				XMLHLINKUP_MASK)) {
			port->link = 1;
			break;
		}
		udelay(50000);
		link_wait_us -= 50000;

		/*
		 * If no link, force link to 2.5 and turn off ltssm
		 */
		if (link_wait_us == 0) {
			pcie_csr_inle32(csr_base + PCIE_LINK_CAP_ADDR, &val32);
			if (val32 & 0x2) {
				pcie_csr_outle32(csr_base + PCIE_LINK_CAP_ADDR,
							(val32 & 0xfffffff0) | 0x1);
				pcie_csr_outle32(csr_base + PCIECORECTRL_ADDR, 0);
				asm volatile("msync");
				goto retry_link;
			} else
				break;
			
		}
	}

	if (!port->link)
		printk(KERN_INFO
			"PCIE%d: unable to detect link.\n", portno);
	else
		printk(KERN_INFO
			"PCIE%d: successfully set as rootpoint CFG Addr 0x%p "
			"CFG Data 0x%p\n", portno, hose->cfg_addr, hose->cfg_data);
	return port->link;
}

/**
 * @brief  This function setups PCIE controller as Endpoint Complex for operation.
 * @param  host   Host controller structure
 * @param  portno PCIE controller port
 * @return 0 if successfull, < 0 if failed
 *
 */
static int apm_pcie_setup_endpoint(struct pci_controller *hose, int portno)
{
	struct apm_pcie_map_tbl *hb = &apm_pciex_ports[portno].pcie_map_tbl;
	struct apm_pciex_port *port = &apm_pciex_ports[portno];
	void  *csr_base = hose->cfg_addr;
	u64   val64;
	u32   val32;
	int   link_wait_us;
	
	hose->ops = &apm_pcie_ops;

	/* Set up outbound PCIE translation Memory Region #1:
	 * Translate from host memory address (ie. 0x07_XXXX_XXXX) to PCIE
	 * address
	 *
	 * Please note that register endianess depends whether the register
	 * resides inside PCIe Core or by our glue logic. See specification
	 * for more info.
	 */
	pcie_csr_outle32(csr_base + OMR1BARL_ADDR, hb->ob_mem_addr[0].lo);
	pcie_csr_outle32(csr_base + OMR1BARH_ADDR, hb->ob_mem_addr[0].hi);
	if (hb->ob_mem_addr[0].size)
		val64 = ~(hb->ob_mem_addr[0].size - 1) | VAL_F2_MASK;
	else
		val64 = 0;
	pcie_csr_outle32(csr_base + OMR1MSKL_ADDR, LODWORD(val64));
	pcie_csr_outle32(csr_base + OMR1MSKH_ADDR, HIDWORD(val64));
	pcie_csr_outle32(csr_base + POM1L_ADDR, hb->ob_mem_addr[0].pcie_lo);
	pcie_csr_outle32(csr_base + POM1H_ADDR, hb->ob_mem_addr[0].pcie_hi);
	PCIE_DEBUG("PCIE%d: Outbound 0 HBF 0x%02X_%08X -> PCIE 0x%02X_%08X "
		   "Mask 0x%02X_%08X\n", portno,
		hb->ob_mem_addr[0].hi, hb->ob_mem_addr[0].lo,
		hb->ob_mem_addr[0].pcie_hi, hb->ob_mem_addr[0].pcie_lo,
		HIDWORD(val64), LODWORD(val64));		 

	/* Set up outbound PCIE translation memory Region #2:
	 * Translate from host memory address (ie. 0x07_XXXX_XXXX) to PCIE
	 * address
	 */
	pcie_csr_outle32(csr_base + OMR2BARL_ADDR, hb->ob_mem_addr[1].lo);
	pcie_csr_outle32(csr_base + OMR2BARH_ADDR, hb->ob_mem_addr[1].hi);
	if (hb->ob_mem_addr[1].size > 0)
		val64 = ~(hb->ob_mem_addr[1].size - 1) | VAL_F2_MASK;
	else
		val64 = 0;
	pcie_csr_outle32(csr_base + OMR2MSKL_ADDR, LODWORD(val64));
	pcie_csr_outle32(csr_base + OMR2MSKH_ADDR, HIDWORD(val64));
	pcie_csr_outle32(csr_base + POM2L_ADDR, hb->ob_mem_addr[1].pcie_lo);
	pcie_csr_outle32(csr_base + POM2H_ADDR, hb->ob_mem_addr[1].pcie_hi);
	PCIE_DEBUG("PCIE%d: Outbound 1 HBF 0x%02X_%08X -> PCIE 0x%02X_%08X "
		   "size 0x%02X_%08X\n", portno,
		hb->ob_mem_addr[1].hi, hb->ob_mem_addr[1].lo,
		hb->ob_mem_addr[1].pcie_hi, hb->ob_mem_addr[1].pcie_lo,
		HIDWORD(val64), LODWORD(val64));		 

	/* Set up outbound PCIE translation Mem/IO Region #3*/
	pcie_csr_outle32(csr_base + OMR3BARL_ADDR, hb->ob_mem_addr[2].lo);
	pcie_csr_outle32(csr_base + OMR3BARH_ADDR, hb->ob_mem_addr[2].hi);
	if (hb->ob_mem_addr[2].size > 0)
		val64 = ~(hb->ob_mem_addr[2].size - 1) | VAL_F2_MASK;
	else
		val64 = 0;
	pcie_csr_outle32(csr_base + OMR3MSKL_ADDR, LODWORD(val64));
	pcie_csr_outle32(csr_base + OMR3MSKH_ADDR, HIDWORD(val64));
	pcie_csr_outle32(csr_base + POM3L_ADDR, hb->ob_mem_addr[2].pcie_lo);
	pcie_csr_outle32(csr_base + POM3H_ADDR, hb->ob_mem_addr[2].pcie_hi);
	PCIE_DEBUG("PCIE%d: Outbound 2 HBF 0x%02X_%08X -> PCIE 0x%02X_%08X "
		   "size 0x%02X_%08X\n", portno,
		hb->ob_mem_addr[2].hi, hb->ob_mem_addr[2].lo,
		hb->ob_mem_addr[2].pcie_hi, hb->ob_mem_addr[2].pcie_lo,
		HIDWORD(val64), LODWORD(val64));		 

	/* Set up outbound Message Region: 2^7 - 2^15
	 * Translate from host memory address (ie. 0x07_XXXX_XXXX) to PCIE
	 * address
	 */
	pcie_csr_outle32(csr_base + MSGBARL_ADDR, 0x000000);
	pcie_csr_outle32(csr_base + MSGBARH_ADDR, 0x000000);
	pcie_csr_outle32(csr_base + MSGMSK_ADDR, 0x000000);
	pcie_csr_outle32(csr_base + POM4L_ADDR, 0x000000);
	pcie_csr_outle32(csr_base + POM4H_ADDR, 0x000000);

	/* No outbound configuration region for endpoint device */

	/* Set up in bound PCIE Memory Region #1:
	*   PCIE Address at 0 ==> AXI range from 0 - 0x0F_FFFF_FFFF
	*   64-bits address/No prefetchable/Memory BAR
	*/
	pcie_csr_outle32(csr_base + PCI_BASE_ADDRESS_0,
		   (hb->ib_mem_addr[0].pcie_lo & PCI_BASE_ADDRESS_MEM_MASK)
		   | PCI_BASE_ADDRESS_MEM_TYPE_64);
	pcie_csr_outle32(csr_base + PCI_BASE_ADDRESS_1,
		   hb->ib_mem_addr[0].pcie_hi);
	if (hb->ib_mem_addr[0].pcie_size > 0)
		val64 = (hb->ib_mem_addr[0].pcie_size-1);/* min is 1 MB */
	else
		val64 = 0;
	pcie_csr_outle32(csr_base + BAR0_MSK_L_ADDR, LODWORD(val64));
	pcie_csr_outle32(csr_base + BAR0_MSK_H_ADDR, HIDWORD(val64));
	pcie_csr_outle32(csr_base + PIM1_1L_ADDR, hb->ib_mem_addr[0].pim1_lo);
	pcie_csr_outle32(csr_base + PIM1_1H_ADDR, hb->ib_mem_addr[0].pim1_hi);
	pcie_csr_outle32(csr_base + PIM1_2L_ADDR, hb->ib_mem_addr[0].pim2_lo);
	pcie_csr_outle32(csr_base + PIM1_2H_ADDR, hb->ib_mem_addr[0].pim2_hi);
	pcie_csr_outle32(csr_base + PIM1SL_ADDR,
			HIDWORD(hb->ib_mem_addr[0].pim_size));
	pcie_csr_outle32(csr_base + PIM1SH_ADDR,
			LODWORD(hb->ib_mem_addr[0].pim_size));
	PCIE_DEBUG("PCIE%d: Inbound 0 PCIE 0x%02X_%08X -> HBF 0x%02X_%08X "
		   "Mask 0x%02X_%08X\n"
		   "                                    ->     0x%02X_%08X\n",
		portno,
		hb->ib_mem_addr[0].pcie_hi, hb->ib_mem_addr[0].pcie_lo,
		hb->ib_mem_addr[0].pim1_hi, hb->ib_mem_addr[0].pim1_lo,
		HIDWORD(val64), LODWORD(val64),	 
		hb->ib_mem_addr[0].pim2_hi, hb->ib_mem_addr[0].pim2_lo);

	/* Set up in bound PCIE Memory Region #2 */
	pcie_csr_outle32(csr_base + PCI_BASE_ADDRESS_4,
		   (hb->ib_mem_addr[1].pcie_lo & PCI_BASE_ADDRESS_MEM_MASK) 
		   | PCI_BASE_ADDRESS_MEM_TYPE_64);
	pcie_csr_outle32(csr_base + PCI_BASE_ADDRESS_5,
		   hb->ib_mem_addr[1].pcie_hi);
	if (hb->ib_mem_addr[1].pcie_size > 0)
		val64 = (hb->ib_mem_addr[1].pcie_size - 1)
			| PCIE_BAR_EN;	 /* min is 1 MB */
	else
		val64 = 0;
	pcie_csr_outle32(csr_base + BAR2_MSK_L_ADDR, LODWORD(val64));
	pcie_csr_outle32(csr_base + BAR2_MSK_H_ADDR, HIDWORD(val64));
	pcie_csr_outle32(csr_base + PIM2_1L_ADDR, hb->ib_mem_addr[1].pim1_lo);
	pcie_csr_outle32(csr_base + PIM2_1H_ADDR, hb->ib_mem_addr[1].pim1_hi);
	pcie_csr_outle32(csr_base + PIM2_2L_ADDR, hb->ib_mem_addr[1].pim2_lo);
	pcie_csr_outle32(csr_base + PIM2_2H_ADDR, hb->ib_mem_addr[1].pim2_hi);
	pcie_csr_outle32(csr_base + PIM2SL_ADDR,
		     LODWORD(hb->ib_mem_addr[1].pim_size));
	pcie_csr_outle32(csr_base + PIM2SH_ADDR,
		     HIDWORD(hb->ib_mem_addr[1].pim_size));
	PCIE_DEBUG("PCIE%d: Inbound 1 PCIE 0x%02X_%08X -> HBF 0x%02X_%08X "
		   "Mask 0x%02X_%08X\n"
		   "                                    ->     0x%02X_%08X\n",
		portno,
		hb->ib_mem_addr[1].pcie_hi, hb->ib_mem_addr[1].pcie_lo,
		hb->ib_mem_addr[1].pim1_hi, hb->ib_mem_addr[1].pim1_lo,
		HIDWORD(val64), LODWORD(val64),	 
		hb->ib_mem_addr[1].pim2_hi, hb->ib_mem_addr[1].pim2_lo);

	/* Set up in bound PCIE I/O Region: Not used */
	pcie_csr_outle32(csr_base + PCI_BASE_ADDRESS_2, 0x00000000);
	pcie_csr_outle32(csr_base + BAR3_MSK_ADDR, 0x00000000);
	pcie_csr_outle32(csr_base + PIM3L_ADDR, 0x00000000);
	pcie_csr_outle32(csr_base + PIM3H_ADDR, 0x00000000 & ~0xFF00);

	/* Expansion ROM Region #4: */
	pcie_csr_outle32(csr_base + PIM4L_ADDR, hb->ib_rom_addr_lo);
	pcie_csr_outle32(csr_base + PIM4H_ADDR, hb->ib_rom_addr_hi);
	if (hb->ib_rom_addr_size > 0)	/* min 2KB */
		val32 = (hb->ib_rom_addr_size - 1) | 0x7FE |
			PCIE_BAR_EN;
	else
		val32 = 0;
	pcie_csr_outle32(csr_base + EXPROM_MSK_ADDR, val32);

	/* Modify the device id of endpoint port to 9111 from 9110 */
	pcie_csr_outle32(csr_base + PCI_VENDOR_ID, 0x911110e8);
	/* For endpoint set the class id to system peripheral */
	pcie_csr_outle32(csr_base + PCI_CLASS_REVISION, 0x08000001);

	/* Enable I/O, Mem, and Busmaster cycles
	NOTE: The status register (higher 2 bytes) is not affected
		as we are writing 0's
	*/
	pcie_csr_inle32(csr_base + PCI_COMMAND, &val32);
	pcie_csr_outle32(csr_base + PCI_COMMAND, 
		val32 | PCI_COMMAND_IO | PCI_COMMAND_MEMORY | 
		PCI_COMMAND_MASTER);

	/* Set Core as Root Complex and Enable core Link establishment */
	pcie_csr_outle32(csr_base + PCIECORECTRL_ADDR,
			PTYPE_ENDPOINT | APPLTSSMENABLE_MASK);

	/* Wait until PHY and Data Link are up */		
	pcie_csr_inle32(csr_base + PCIECORESTATUS_ADDR, &val32);
	port->link = 0;
	for (link_wait_us = APM_PCIE_LINK_WAIT_US; link_wait_us > 0; ) {
		pcie_csr_inle32(csr_base + PCIECORESTATUS_ADDR, &val32);
		val32 &= RDHLLINKUP_MASK |
			 XMLHLINKUP_MASK;
		if (val32 == (RDHLLINKUP_MASK |
			      XMLHLINKUP_MASK)) {
			port->link = 1;
			break;
		}
		udelay(50000); /* FIXME - need to fine tune */
		link_wait_us -= 50000;
	}

	if (!port->link)
		printk(KERN_INFO
			"PCIE%d: unable to detect link.\n", portno);
	else
		printk(KERN_INFO
			"PCIE%d: successfully set as endpoint CFG Addr 0x%p "
			"CFG Data 0x%p\n", portno, hose->cfg_addr, 
			hose->cfg_data ? hose->cfg_data : NULL);
	return port->link;
}

static int __init apm_pcie_parse_dma_ranges(struct pci_controller *host,
					     void __iomem    *reg,
					     struct resource *res,
					     struct device_node *dn)
{
	const u32 *ranges;
	u64 size;
	int rlen;
	int pna = of_n_addr_cells(dn);
	int np = pna + 5;

	/* Default */
	res->start = 0;
	res->end   = size = 0x80000000;
	res->flags = IORESOURCE_MEM | IORESOURCE_PREFETCH;

	/* Get dma-ranges property */
	ranges = of_get_property(dn, "dma-ranges", &rlen);
	if (ranges == NULL)
		goto out;

	/* Walk it */
	while ((rlen -= np * 4) >= 0) {
		u32 pci_space = ranges[0];
		u64 pci_addr = of_read_number(ranges + 1, 2);
		u64 cpu_addr = of_translate_address(dn, ranges + 3);
		size = of_read_number(ranges + pna + 3, 2);
		ranges += np;
		if (cpu_addr == OF_BAD_ADDR || size == 0)
			continue;

		/* We only care about memory */
		if ((pci_space & 0x03000000) != 0x02000000)
			continue;

		/* We currently only support memory at 0, and pci_addr
		 * within 32 bits space
		 */
		if (cpu_addr != 0 || pci_addr > 0xffffffff) {
			printk(KERN_WARNING "%s: Ignored unsupported dma range"
			       " 0x%016llx...0x%016llx -> 0x%016llx\n",
			       dn->full_name,
			       pci_addr, pci_addr + size - 1, cpu_addr);
			continue;
		}

		/* Check if not prefetchable */
		if (!(pci_space & 0x40000000))
			res->flags &= ~IORESOURCE_PREFETCH;


		/* Use that */
		res->start = pci_addr;
#ifndef CONFIG_RESOURCES_64BIT
		/* Beware of 32 bits resources */
		if ((pci_addr + size) > 0x100000000ull)
			res->end = 0xffffffff;
		else
#endif
			res->end = res->start + size - 1;
		break;
	}

	/* We only support one global DMA offset */
	if (dma_offset_set && pci_dram_offset != res->start) {
		printk(KERN_ERR "%s: dma-ranges(s) mismatch\n",
		       dn->full_name);
		return -ENXIO;
	}

	/* Check that we can fit all of memory as we don't support
	 * DMA bounce buffers
	 */
	if (size < total_memory) {
		printk(KERN_ERR "%s: dma-ranges too small "
		       "(size=%llx total_memory=%llx)\n",
		       dn->full_name, size, total_memory);
		return -ENXIO;
	}

	/* Check we are a power of 2 size and that base is a multiple of size*/
	if (!is_power_of_2(size) ||
	    (res->start & (size - 1)) != 0) {
		printk(KERN_ERR "%s: dma-ranges unaligned\n",
		       dn->full_name);
		return -ENXIO;
	}

	/* Check that we are fully contained within 32 bits space */
	if (res->end > 0xffffffff) {
		printk(KERN_ERR "%s: dma-ranges outside of 32 bits space\n",
		       dn->full_name);
		return -ENXIO;
	}
 out:
	dma_offset_set  = 1;
	pci_dram_offset = res->start;

	printk(KERN_INFO "APM PCIe DMA offset set to 0x%08lx\n",
	       pci_dram_offset);
	return 0;
}

#ifdef CONFIG_PCIE_ENUM
/* 
 * This function get pointer ot CAPABLIITY LIST and 
 * combination of device/function number
 */
static int __init get_pci_exp_ptr(struct pci_controller *hose, int bus, u8 *ptr, int *devfn)
{
	u8 cap_id;
	u8 next;
	int j;
	
	
	u8 devfn_trace[256];
	memset(devfn_trace, 0, 256);

	for (j = 0; j <= 0xff; j++) {
		if (early_read_config_byte(hose, bus, j, 
					PCI_CAPABILITY_LIST, ptr) < 0) {
			printk(KERN_ERR 
				"get_pci_exp_ptr:couldn't read Capabliity"
				" List Register\n");
			return -1;
		}
	
		if (*ptr != 0xff) 
			break;
	}
    
	if (*ptr == 0xff)
		return 0;
    	
    	*devfn = j;

	while (*ptr) {
		if (early_read_config_byte(hose, bus, j, 
				*ptr + PCI_CAP_LIST_ID, &cap_id) < 0) {
			printk(KERN_ERR "get_pci_exp_ptr:couldn't read"
				" Capability ID\n");
			return -1;
		}
		
		if (early_read_config_byte(hose, bus, j, 
				*ptr + PCI_CAP_LIST_NEXT, &next) < 0) {
			printk(KERN_ERR "get_pci_exp_ptr:couldn't read"
				" Capability List Next\n");
			return -1;
		}
		
		if (devfn_trace[*ptr]++) {
			printk("<chain looped>\n");
			return -1;
		}
		if (cap_id == 0xff) {
			printk("<chain broken>\n");
			return -1;
		}
		
		/*
		 * We only interested in PCI Express capability
		 * which have payload size etc
		 */
		if (cap_id == PCI_CAP_ID_EXP) {
			return 0;
		} 
		*ptr = next;
	}

	return -1;
}

void apm_config_pcie_mps(struct pci_controller *hose)
{
        int i;
	int devfn;
	u32 mps;
	u32 mps_supported;
	u8  ptr;
	int adjust_mps=1;
#if 1
	void  *csr_base = hose->cfg_addr;
	u32 ecdevcppa;
	u32 ecdevctl;
#endif

	/* 
	 * Check supported max payload size for all devices
	 * If all devices on this hose supported mps > 256, then we will set the max payload
	 * size of the root complex and each device's mps to 256bytes
	 */
	for (i = hose->first_busno + 1; i <= hose->last_busno; i++) {
		if (get_pci_exp_ptr(hose, i, &ptr, &devfn) < 0) {
			printk(KERN_ERR 
				"get_pci_exp_ptr:couldn't read Capabliity"
				" List Register on bus:%d\n", i);
			continue;
		}
	    
		if (ptr == 0xff) 
			continue;
	    
		if (early_read_config_dword(hose, i, devfn, 
				ptr + PCI_EXP_DEVCAP, &mps_supported) < 0) {
			printk(KERN_ERR 
				"apm_config_pcie_mps:couldn't read Device"
				" Capability Register\n");
			return;
		}
		
		if (early_read_config_dword(hose, i, devfn, 
				ptr + PCI_EXP_DEVCTL, &mps) < 0) {
			printk(KERN_ERR 
				"apm_config_pcie_mps:couldn't read Device"
				" Capability Register\n");
			return;
		}
		PCIE_DEBUG(
			"Max pay load supported 0x%x max pay load prog 0x%x\n",
			mps_supported, mps); 
		printk(
			"Max pay load supported 0x%x max pay load prog 0x%x\n",
			mps_supported, mps); 
		if (!(mps_supported & PCI_EXP_DEVCAP_PAYLOAD)) {
			adjust_mps = 0;
	    	} 
	    
		/* Found valid payload information */
		break;
	}

	if (adjust_mps) {
		u32 devctrl = 0;
		pcie_csr_inle32(csr_base + PCIE_DEV_CTRL_ADDR, &devctrl);
		PCIE_DEBUG("devctrl = 0x%x mps = 0x%x\n", devctrl, mps);
		devctrl = (devctrl & 0xffffff8f) | mps;
		pcie_csr_outle32(csr_base + PCIE_DEV_CTRL_ADDR, devctrl);
		PCIE_DEBUG("hose->first_busno 0x%x hose 0x%x devctrl 0x%x\n",
			hose->first_busno, (u32) hose, devctrl);
		/* Set supported MPS and MPS to 256byte for the root complex */
		if (early_read_config_dword(hose, hose->first_busno, 0,  
					PECFG_ECRTCTL, &ecdevctl)<0) {
			printk(KERN_ERR 
				"apm_config_pcie_mps:couldn't read Device"
				" Control Register\n");
			return;
		}
		printk("remote dev ctrl %x\n", ecdevctl);

#if 0
		early_write_config_dword(hose, hose->first_busno, 0, 
			PECFG_ECDEVCTL, (ecdevctl & 0xffff1f1f) | 0x00001020);	    
#endif

		if (early_read_config_dword(hose, hose->first_busno, 0,  
				PECFG_ECRTCAP, &ecdevcppa) < 0) {
			printk(KERN_ERR 
				"apm_config_pcie_mps:couldn't read Device"
				" Capability Register\n");
			return;
		}

		printk("remote dev cap %x\n", ecdevcppa);

#if 0
		early_write_config_dword(hose, hose->first_busno, 0, 
			PECFG_ECDEVCPPA, 
			(ecdevcppa & 0xfffffff8) | 0x00000001);
	    
		for (i = hose->first_busno +1; i<=hose->last_busno; i++) {
			if (get_pci_exp_ptr(hose, i, &ptr, &devfn) < 0) {
		    		printk(KERN_ERR 
		    			"apm_config_pcie_mps:couldn't read "
		    			"Capabliity List Register on bus:%d\n",
		    			i);
				return;
			}
		
			if (ptr != 0xff) {   
				if (early_read_config_dword(hose, i, devfn, 
					ptr + PCI_EXP_DEVCTL, &mps) < 0) {
					printk(KERN_ERR 
						"apm_config_pcie_mps:couldn't "
						"read Device Capability "
						"Register\n");
					return;
		    		}

		    		early_write_config_dword(hose, i, devfn, 
		    			ptr + PCI_EXP_DEVCTL, 
		    			(mps & 0xffffff1f) | 0x00000030);
			}
	 	}
#endif
	}
}
#endif
	
static void __init apm_pcie_probe_bridge(struct device_node *np)
{
	const u32 	*pval;
        int 		portno;
	int 		primary;
	const int 	*bus_range;
	struct resource	csr_res;
	struct resource	cfg_res;
	struct apm_pciex_port 	 *port = NULL;
	struct pci_controller    *hose = NULL;
	struct apm_pcie_map_tbl *hb;
	void *csr_base = NULL;
	void *scu_base = NULL;
	void *cfg_base = NULL;	
	const u8 	*val;
	int  busses;
	int  i;
	void __iomem    *reg = NULL;
	struct resource	ib_res;
	
        /* Get the port number from the device-tree */
        pval = of_get_property(np, "port", NULL);
        if (pval == NULL) {
                printk(KERN_ERR "PCIE: Can't find port number for %s\n",
                       np->full_name);
                return;
        }
        portno = *pval;

	port = &apm_pciex_ports[portno];
	port->index = portno;
	
	/*
	 * Check if device is enabled
	 */
	if (!of_device_is_available(np)) {
		printk(KERN_INFO "PCIE%d: Port disabled via device-tree\n", 
			port->index);
		return;
	}
			
	port->node = of_node_get(np);
	
	/* Check if device_type property is set to "pci" or "pci-endpoint".
	 * Resulting from this setup this PCIe port will be configured
	 * as root-complex or as endpoint.
	 */
	val = of_get_property(np, "device_type", NULL);
	if (val && !strcmp(val, "pci-endpoint")) {
		port->endpoint = 1;
	} else if (val && !strcmp(val, "pci")) {
		port->endpoint = 0;
	} else {
		printk(KERN_ERR 
			"PCIE: missing or incorrect device_type for %s\n",
		       np->full_name);
		return;
	}

	/* Get configured as primary bridge */
	primary = of_get_property(np, "primary", NULL) ? 1 : 0;

	/* Get configured bus range if any */
	bus_range = of_get_property(np, "bus-range", NULL);

	/* Get configured config space registers address */
	if (of_address_to_resource(np, 0, &cfg_res)) {
		printk(KERN_ERR "PCIE%d: Can't get PCIE config space for %s!",
		       portno, np->full_name);
		return;
	}

        /* Get the remote max read request size from the device-tree */
        pval = of_get_property(np, "rmrrqs", NULL);
        if ((pval != NULL) && (*pval <= 5)) {
		printk("Derived remote max read request size to %d\n", *pval);
		port->rmt_mrrqs = *pval;	/* 512 default */
        } else {

		port->rmt_mrrqs = 5;	/* 4096B default */
	}
	
        /* Check if PHY reset required in device-tree */
        pval = of_get_property(np, "phy-reset", NULL);
        if (pval == NULL) {
		port->phy_reset = 1;
        } else {
		port->phy_reset = *pval ? 1 : 0;
	}
	
	/* Get configured CSR space registers address */
	if (of_address_to_resource(np, 1, &csr_res)) {
		printk(KERN_ERR "PCIE%d: Can't get PCIE CSR space for %s!",
		       portno, np->full_name);
		return;
	}
	
	/* Allocate the host controller data structure */
	hose = pcibios_alloc_controller(port->node);
	if (!hose) {
		printk(KERN_ERR "PCIE%d: Can't allocate controller memory "
			"for %s!", portno, np->full_name);
		return;
	}
	port->hose = hose;
	
	/* We stick the port number in "indirect_type" so the config space
	 * ops can retrieve the port data structure easily
	 */
	hose->indirect_type = port->index;

	/* Get bus range */
	hose->first_busno = bus_range ? bus_range[0] : 0x0;
	hose->last_busno = bus_range ? bus_range[1] : 0xff;
	port->last_busno = hose->last_busno;
	 
	/* Because of how big mapping the config space is (1M per bus), we
	 * limit how many busses we support. In the long run, we could replace
	 * that with something akin to kmap_atomic instead. We set aside 1 bus
	 * for the host itself too.
	 */
	busses = hose->last_busno - hose->first_busno; /* This is off by 1 */
	if (busses > MAX_PCIE_BUS_MAPPED) {
		busses = MAX_PCIE_BUS_MAPPED;
		hose->last_busno = hose->first_busno + busses;
	}

	hb = &port->pcie_map_tbl;
	if (!port->endpoint) {
		/* Only map the external config space in cfg_data for
		 * PCIe root-complexes. External space is 1M per bus
		 */
		hb->cfg_addr_hi = HIDWORD(cfg_res.start);
		hb->cfg_addr_lo = LODWORD(cfg_res.start);
		hb->cfg_addr_size = RES_SIZE(&cfg_res);
		hb->cfg_pcie_hi = 0;
		hb->cfg_pcie_lo = LODWORD(cfg_res.start);
		cfg_base = ioremap(cfg_res.start, RES_SIZE(&cfg_res));
		if (cfg_base == NULL) {
			printk(KERN_ERR "%s: Can't map external config space!",
			       port->node->full_name);
			goto fail;
		}
		hose->cfg_data = cfg_base;
	}
	
	/* Map CSR space */
	hb->csr_addr      = csr_res.start;
	hb->csr_addr_size = RES_SIZE(&csr_res);
	csr_base = ioremap(csr_res.start, RES_SIZE(&csr_res));
	if (csr_base == NULL) {
		printk(KERN_ERR "%s: Can't map CSR space!",
			port->node->full_name);
		goto fail;
	}
	hose->cfg_addr = csr_base;  
	
	printk(KERN_INFO "PCIE%d: CSR 0x%02X_%08X (0x%p) size %d "
		"CFG 0x%02X_%08X (0x%p) %s\n",
		 portno, HIDWORD(hb->csr_addr), LODWORD(hb->csr_addr),
		 csr_base, hb->csr_addr_size, 
		 hb->cfg_addr_hi, hb->cfg_addr_lo, cfg_base, 
		 primary ? "Primary" : "Secondary");

	/* Parse outbound mapping resources */
	pci_process_bridge_OF_ranges(hose, np, primary);

	/* Now load configuration address into table -
	 * either 3 memory region or 2 memory + 1 Mem/IO regions
	 */
	for (i = 0; i < 3; i++) {
		hb->ob_mem_addr[i].hi = HIDWORD(hose->mem_resources[i].start);
		hb->ob_mem_addr[i].lo = LODWORD(hose->mem_resources[i].start);
		hb->ob_mem_addr[i].size = RES_SIZE(&hose->mem_resources[i]);
		hb->ob_mem_addr[i].pcie_hi = 
				HIDWORD(hose->mem_resources[i].start -
					hose->pci_mem_offset);
		hb->ob_mem_addr[i].pcie_lo =
				LODWORD(hose->mem_resources[i].start -
					hose->pci_mem_offset);
	}
	if (RES_SIZE(&hose->io_resource) > 0) {
		i = 2;
		hb->ob_mem_addr[i].hi = HIDWORD(hose->io_base_phys + 
						hose->io_resource.start);
		hb->ob_mem_addr[i].lo = LODWORD(hose->io_base_phys + 
						hose->io_resource.start);
		hb->ob_mem_addr[i].size = RES_SIZE(&hose->io_resource);
		hb->ob_mem_addr[i].pcie_hi = HIDWORD(hose->io_resource.start);
		hb->ob_mem_addr[i].pcie_lo = LODWORD(hose->io_resource.start);
	}
	hb->ob_msg_addr_hi 	= 0;
	hb->ob_msg_addr_lo 	= 0;
	hb->ob_msg_addr_size	= 0;
	hb->ob_msg_pcie_addr_hi = HIDWORD(MAKE_U64(hb->ob_msg_addr_hi,
						   hb->ob_msg_addr_lo) -
					  hose->pci_mem_offset);
	hb->ob_msg_pcie_addr_lo = LODWORD(MAKE_U64(hb->ob_msg_addr_hi,
						   hb->ob_msg_addr_lo) -
					  hose->pci_mem_offset);

	/* Parse inbound mapping resources */
	if (apm_pcie_parse_dma_ranges(hose, reg, &ib_res, np) != 0)
		goto fail;
	
#ifdef CONFIG_PCI_MSI
	/* Now load configuration region #1 address into table */
	/* 
	 * We will have to disable coherency from the device side
         * So we have to force coherency in DDR, so that mpic
         * is non coherent
	 */
	i = 0;
	hb->ib_mem_addr[i].pcie_hi = HIDWORD(ib_res.start);
	hb->ib_mem_addr[i].pcie_lo = LODWORD(ib_res.start);
	hb->ib_mem_addr[i].pcie_size = RES_SIZE(&ib_res);
	hb->ib_mem_addr[i].pim1_hi = 0x30;
	hb->ib_mem_addr[i].pim1_lo = 0;
	hb->ib_mem_addr[i].pim2_hi = 0;
	hb->ib_mem_addr[i].pim2_lo = 0;
	hb->ib_mem_addr[i].pim_size = 0xffffffff80000000ULL;
	/* Now load configuration region #2 address into table */
	i++;
	hb->ib_mem_addr[i].pcie_size = 0;
	/*
	 * Lets map the MPIC for this PIM, one for one
	 */
	hb->ib_mem_addr[i].pcie_hi = HIDWORD(0xddf400000ULL);
	hb->ib_mem_addr[i].pcie_lo = LODWORD(0xddf400000ULL);
	hb->ib_mem_addr[i].pcie_size = 0x100000;
	hb->ib_mem_addr[i].pim1_hi = HIDWORD(0xddf400000ULL);
	hb->ib_mem_addr[i].pim1_lo = LODWORD(0xddf400000ULL);
	hb->ib_mem_addr[i].pim2_hi = 0;
	hb->ib_mem_addr[i].pim2_lo = 0;
	hb->ib_mem_addr[i].pim_size = 0xfffffffffff00000ULL;
#else
	/* Now load configuration region #1 address into table */
	i = 0;
	hb->ib_mem_addr[i].pcie_hi = HIDWORD(ib_res.start);
	hb->ib_mem_addr[i].pcie_lo = LODWORD(ib_res.start);
	hb->ib_mem_addr[i].pcie_size = RES_SIZE(&ib_res);
	hb->ib_mem_addr[i].pim1_hi = 0;
	hb->ib_mem_addr[i].pim1_lo = 0;
	hb->ib_mem_addr[i].pim2_hi = 0;
	hb->ib_mem_addr[i].pim2_lo = 0;
	hb->ib_mem_addr[i].pim_size = 0xffffffff80000000ULL;
	/* Now load configuration region #2 address into table */
	i++;
	hb->ib_mem_addr[i].pcie_size = 0;



#endif
	/* Now load configuration region #3 address into table */
	i++;
	hb->ib_mem_addr[i].pcie_size = 0;
	/* Now load configuration Expansion ROM address into table */
	hb->ib_rom_addr_hi   = 0;
	hb->ib_rom_addr_lo   = 0;
	hb->ib_rom_addr_size = 0;

	/* Initialize the core */
	if (apm_init_pcie(port) < 0)
		goto fail;
	
	/* Setup Root/Endpoint */
	if (!port->endpoint) {
		printk(KERN_INFO "PCIE%d: Configured as root-point\n", portno);
		if (apm_pcie_setup_rootpoint(hose, portno) == 0)
			goto fail;
#ifdef CONFIG_PCIE_ENUM
		else
			apm_config_pcie_mps(hose);
#endif
	} else {
		printk(KERN_INFO "PCIE%d: Configured as end-point\n", portno);
		if (apm_pcie_setup_endpoint(hose, portno) == 0)
			goto fail;
	}

	return;
	
fail:
	printk(KERN_INFO "PCIE%d: Configuration failed\n", portno);
	if (cfg_base)
		iounmap(cfg_base);
	if (scu_base)
		iounmap(scu_base);
	if (csr_base)
		iounmap(csr_base);
	if (hose)
		pcibios_free_controller(hose);
	/* Clear field as power management will check each port */
	port->hose = NULL;
}

#ifdef CONFIG_PM
static int apm_pcie_hwresume(struct apm_pciex_port *port)
{
	/* Check if IP powered off completely (in reset state) */
	if (!port->pwred_off_flags)
		return 0;
	
	/* Re-initilize hardware */
	if (apm_init_pcie(port) < 0) {
		/* Failed PLL lock */
		return 0;
	}
	
	/* Re-setup Root/Endpoint */
	if (!port->endpoint) {
		printk(KERN_INFO "PCIE%d: re-configured as root-point\n", 
			port->index);
		if (apm_pcie_setup_rootpoint(port->hose, port->index) == 0)
			goto fail;
#ifdef CONFIG_PCIE_ENUM
		else
			apm_config_pcie_mps(port->hose);
#endif
	} else {
		printk(KERN_INFO "PCIE%d: re-configured as end-point\n", 
			port->index);
		if (apm_pcie_setup_endpoint(port->hose, port->index) == 0)
			goto fail;
	}
fail:
	return 0;
}

int apm_pcie_suspend(struct sys_device *dev, pm_message_t state)
{
	int i;
	
	for (i = 0; i < apm_pciex_port_cnt; i++) {
		struct apm_pciex_port *port = &apm_pciex_ports[i]; 
		if (port && port->hose && port->hose->cfg_addr) {
			switch (state.event) {
			case PM_EVENT_FREEZE:
			case PM_EVENT_SUSPEND:
			case PM_EVENT_HIBERNATE:
				port->pwred_off_flags = 0x01;
				break;
			default:
				port->pwred_off_flags = 0x00;
				break;
			};
		}
	}
	return 0;
}

int apm_pcie_resume(struct sys_device *dev)
{
	int i;
	for (i = 0; i < apm_pciex_port_cnt; i++) {
		struct apm_pciex_port *port = &apm_pciex_ports[i]; 
		if (port && port->hose && port->hose->cfg_addr) {
			/* Re-program the bridge again */
			apm_pcie_hwresume(port);
		}
	} 
	return 0;
}

static struct sysdev_class apm_pcie_pm_sysclass = {
	.name = "apm_pcie_pm",
	.suspend = apm_pcie_suspend,
	.resume = apm_pcie_resume,
};

static struct sys_device apm_pcie_pm_device = {
	.cls = &apm_pcie_pm_sysclass
};

static int __init apm_pcie_pm_init(void)
{
	int err;

	err = sysdev_class_register(&apm_pcie_pm_sysclass);
	if (err == 0)
		err = sysdev_register(&apm_pcie_pm_device);

	return err;
}
#else
static int __init apm_pcie_pm_init(void) { return 0; }
#endif /* CONFIG_PM */

static int __init apm_pcie_find_bridges(void)
{
	struct device_node *np;

	for_each_compatible_node(np, NULL, "apm,apm-pciex") {
		if (of_device_is_available(np))
			apm_pciex_port_cnt++;
	}
	
	if (apm_pciex_port_cnt == 0) {
		printk(KERN_ERR "PCIE: no ports found in dts\n");
		return 0;
	} 
	printk(KERN_INFO "PCIE: %d ports found in dts\n", apm_pciex_port_cnt);
	
	apm_pciex_ports = kzalloc(APM_PCIE_HOST_TOTAL * 
				   sizeof(struct apm_pciex_port), GFP_KERNEL);
	if (apm_pciex_ports == NULL) {
		printk(KERN_ERR 
			"PCIE: unable allocate memory for apm_pciex_port\n");
		return 0;
	}
	
	for_each_compatible_node(np, NULL, "apm,apm-pciex") 
		apm_pcie_probe_bridge(np);

	apm_pcie_pm_init();
		
	return 0;
}
arch_initcall(apm_pcie_find_bridges);
