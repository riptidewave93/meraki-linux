/*
 * Definitions required by PCI code. Should gnerally get included from io.h
 *
 * Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/
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
#ifndef __ARCH_ARM_OMAP2_PCI_H
#define __ARCH_ARM_OMAP2_PCI_H

#include <plat/ti81xx.h>

/* Enumeration flag: Checked during PCI Enumeration of Bridges */
#define pcibios_assign_all_busses()     1

/*
 * PCI Resource allocation.
 *
 * IMPORTANT: Ensure that the vals used below match with the ones passed
 * though PCIe RC platform data (from SoC/board file).
 */
#define PCIBIOS_MIN_IO			(TI816X_PCIE_IO_BASE)
#define PCI_IO_SIZE			(SZ_2M + SZ_1M)
#define PCIBIOS_MIN_MEM			(TI816X_PCIE_MEM_BASE)

/*
 * Override the I/O accessors to catch accesses to PCIe window and route them
 * through PCIe i/o routines. Pass through others as normal (non-PCIe) accesses.
 *
 * Note that, curretly only TI81XX devices support PCIe.
 */
#if defined CONFIG_PCI && !defined __ASSEMBLER__
#define is_pcie_io(addr)	(cpu_is_ti81xx() &&			\
				((u32)(addr) & ~(PCI_IO_SIZE - 1)) ==	\
					PCIBIOS_MIN_IO)


extern int ti81xx_pci_io_read(u32 addr, int size, u32 *val);
extern int ti81xx_pci_io_write(u32 addr, int size, u32 val);

static inline void _outb(u8 val, u32 addr)
{
	if (is_pcie_io(addr))
		ti81xx_pci_io_write(addr, 1, (u32)val);
	else
		__raw_writeb(val, __typesafe_io(addr));
}

static inline void _outw(u16 val, u32 addr)
{
	if (is_pcie_io(addr))
		ti81xx_pci_io_write(addr, 2, (u32)val);
	else
		__raw_writew((__force __u16)cpu_to_le16(val),
				__typesafe_io(addr));
}

static inline void _outl(u32 val, u32 addr)
{
	if (is_pcie_io(addr))
		ti81xx_pci_io_write(addr, 4, val);
	else
		__raw_writel((__force __u32)cpu_to_le32(val),
				__typesafe_io(addr));
}

static inline u8 _inb(u32 addr)
{
	if (is_pcie_io(addr)) {
		u32 val;
		ti81xx_pci_io_read(addr, 1, &val);
		return (u8)val;
	} else {
		return __raw_readb(__typesafe_io(addr));
	}
}

static inline u16 _inw(u32 addr)
{
	if (is_pcie_io(addr)) {
		u32 val;
		ti81xx_pci_io_read(addr, 2, &val);
		return (u16)val;
	} else {
		return le16_to_cpu((__force __le16)
				__raw_readw(__typesafe_io(addr)));
	}
}

static inline u32 _inl(u32 addr)
{
	if (is_pcie_io(addr)) {
		u32 val;
		ti81xx_pci_io_read(addr, 4, &val);
		return (u32)val;
	} else {
		return le32_to_cpu((__force __le32)
				__raw_readl(__typesafe_io(addr)));
	}
}

#define outb(v,p)	_outb(v,p)
#define outw(v,p)	_outw(v,p)
#define outl(v,p)	_outl(v,p)

#define inb(p)		_inb(p)
#define inw(p)		_inw(p)
#define inl(p)		_inl(p)

#define ioread8(addr)							\
	({								\
		u32 val;						\
		if (is_pcie_io(addr)) {					\
			ti81xx_pci_io_read((u32)addr, 1, &val);		\
		} else {						\
			val = __raw_readb(addr);			\
			__iormb();					\
		}							\
		(u8)val;						\
	})

#define ioread16(addr)							\
	({								\
		u32 val;						\
		if (is_pcie_io(addr)) {					\
			ti81xx_pci_io_read((u32)addr, 2, &val);		\
		} else {						\
			val = le16_to_cpu((__force __le16)__raw_readw(addr));	\
			__iormb();					\
		}							\
		(u16)val;						\
	})

#define ioread32(addr)							\
	({								\
		u32 val;						\
		if (is_pcie_io(addr)) {					\
			ti81xx_pci_io_read((u32)addr, 4, &val);		\
		} else {						\
			val = le32_to_cpu((__force __le32)__raw_readl(addr));	\
			__iormb();					\
		}							\
		val;							\
	})

#define iowrite8(val,addr)						\
	({								\
		if (is_pcie_io(addr)) {					\
			ti81xx_pci_io_write((u32)addr, 1, (u32)val);		\
		} else {						\
			__iowmb();					\
			__raw_writeb(val, addr);			\
		}							\
	})

#define iowrite16(val,addr)						\
	({								\
		if (is_pcie_io(addr)) {					\
			ti81xx_pci_io_write((u32)addr, 2, (u32)val);		\
		} else {						\
			__iowmb();					\
			__raw_writew((__force __u16)cpu_to_le16(val), addr);	\
		}							\
	})

#define iowrite32(val,addr)						\
	({								\
		if (is_pcie_io(addr)) {					\
			ti81xx_pci_io_write((u32)addr, 4, val);		\
		} else {						\
			__iowmb();					\
			__raw_writel((__force __u32)cpu_to_le32(val), addr);	\
		}							\
	})

/* Currently retaining same as io.h */
#define ioread8_rep(p,d,c)	__raw_readsb(p,d,c)
#define ioread16_rep(p,d,c)	__raw_readsw(p,d,c)
#define ioread32_rep(p,d,c)	__raw_readsl(p,d,c)

#define iowrite8_rep(p,s,c)	__raw_writesb(p,s,c)
#define iowrite16_rep(p,s,c)	__raw_writesw(p,s,c)
#define iowrite32_rep(p,s,c)	__raw_writesl(p,s,c)

/* These mimic the corresponding funcitons in arch/arm/mm/iomap.c */
#define ioport_map(port, nr)	__typesafe_io(port);
#define ioport_unmap(addr)

#endif /* CONFIG_PCI */
#endif
