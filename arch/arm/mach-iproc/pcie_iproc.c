/*
 * Copyright (C) 2013, Broadcom Corporation. All Rights Reserved.
 * 
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
/*
* Northstar PCI-Express driver
* Only supports Root-Complex (RC) mode
*
* Notes:
* PCI Domains are being used to identify the PCIe port 1:1.
*
* Only MEM access is supported, PAX does not support IO.
*
* TODO:
*	MSI interrupts,
*	DRAM > 128 MBytes (e.g. DMA zones)
*/

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/bug.h>
#include <linux/delay.h>
#include <linux/pci.h>
#include <linux/io.h>
#include <linux/ioport.h>
#include <linux/interrupt.h>

#include <mach/memory.h>
#include <mach/io_map.h>

#include <asm/mach/pci.h>
#include <asm/sizes.h>

#include <linux/version.h>

#define pci_std_swizzle pci_common_swizzle
#define SZ_32M 0x02000000
#define SZ_48M 0x03000000

/*
 * Register offset definitions
 */

#define NS_PCI_DEBUG		0
#define	SOC_PCIE_CONTROL	0x000	/* a.k.a. CLK_CONTROL reg */
#define	SOC_PCIE_PM_STATUS	0x008
#define	SOC_PCIE_PM_CONTROL	0x00c	/* in EP mode only ! */

#define	SOC_PCIE_EXT_CFG_ADDR	0x120
#define	SOC_PCIE_EXT_CFG_DATA	0x124
#define	SOC_PCIE_CFG_ADDR	0x1f8
#define	SOC_PCIE_CFG_DATA	0x1fc

#define	SOC_PCIE_SYS_RC_INTX_EN		0x330
#define	SOC_PCIE_SYS_RC_INTX_CSR	0x334
#define	SOC_PCIE_SYS_HOST_INTR_EN	0x344
#define	SOC_PCIE_SYS_HOST_INTR_CSR	0x348

#define	SOC_PCIE_HDR_OFF	0x400	/* 256 bytes per function */

/* 32-bit 4KB in-bound mapping windows for Function 0..3, n=0..7 */
#define	SOC_PCIE_SYS_IMAP0(f,n)		(0xc00+((f)<<9)((n)<<2)) 
/* 64-bit in-bound mapping windows for func 0..3 */
#define	SOC_PCIE_SYS_IMAP1(f)		(0xc80+((f)<<3))
#define	SOC_PCIE_SYS_IMAP2(f)		(0xcc0+((f)<<3))
/* 64-bit in-bound address range n=0..2 */
#define	SOC_PCIE_SYS_IARR(n)		(0xd00+((n)<<3))
/* 64-bit out-bound address filter n=0..2 */
#define	SOC_PCIE_SYS_OARR(n)		(0xd20+((n)<<3))
/* 64-bit out-bound mapping windows n=0..2 */
#define	SOC_PCIE_SYS_OMAP(n)		(0xd40+((n)<<3))

#ifdef	__nonexistent_regs_
#define	SOC_PCIE_MDIO_CONTROL	0x128
#define	SOC_PCIE_MDIO_RD_DATA	0x12c
#define	SOC_PCIE_MDIO_WR_DATA	0x130
#define	SOC_PCIE_CLK_STAT	0x1e0 
#endif

#define PCI_MAX_BUS		4
#define pcieHostPrimSecBusNum		(0x00000100 | (PCI_MAX_BUS<<16))
#define pcieSwitchPrimSecBusNum		(0x00000201 | (PCI_MAX_BUS<<16))


#ifdef	CONFIG_PCI

/*
 * Forward declarations
 */
static int soc_pci_setup(int nr, struct pci_sys_data *sys);
static struct pci_bus * soc_pci_scan_bus(int nr, struct pci_sys_data *sys);
static int soc_pcie_map_irq(struct pci_dev *dev, u8 slot, u8 pin);
static int soc_pci_read_config(struct pci_bus *bus, unsigned int devfn,
                                   int where, int size, u32 *val);
static int soc_pci_write_config(struct pci_bus *bus, unsigned int devfn,
                                    int where, int size, u32 val);

#ifndef	CONFIG_PCI_DOMAINS
#error	CONFIG_PCI_DOMAINS is required
#endif


/*
 * PCIe host controller registers
 * one entry per port
 */
#if defined(CONFIG_MACH_NS) || defined(CONFIG_MACH_NSP)
#define MAX_PCI_INTFS 3
#else
#define MAX_PCI_INTFS 2
#endif
/* this is for northstar, co-star and northstar+ */
static struct resource soc_pcie_regs[MAX_PCI_INTFS] = {
	{
	.name = "pcie0",
	.start = 0x18012000,
	.end   = 0x18012fff,
	.flags = IORESOURCE_MEM,
	},
	{
	.name = "pcie1",
	.start = 0x18013000,
	.end   = 0x18013fff,
	.flags = IORESOURCE_MEM,
	},
#if defined(CONFIG_MACH_NS) || defined(CONFIG_MACH_NSP)
	{
	.name = "pcie2",
	.start = 0x18014000,
	.end   = 0x18014fff,
	.flags = IORESOURCE_MEM,
	},
#endif
};

static struct resource soc_pcie_owin[MAX_PCI_INTFS] = {
	{
	.name = "PCIe Outbound Window, Port 0",
	.start = 0x08000000,
	.end =   0x08000000 + SZ_128M - 1,
	.flags = IORESOURCE_MEM,
	},
	{
	.name = "PCIe Outbound Window, Port 1",
	.start = 0x40000000,
	.end =   0x40000000 + SZ_128M - 1,
	.flags = IORESOURCE_MEM,
	},
#if defined(CONFIG_MACH_NS) || defined(CONFIG_MACH_NSP)
	{
	.name = "PCIe Outbound Window, Port 2",
	.start = 0x48000000,
	.end =   0x48000000 + SZ_128M - 1,
	.flags = IORESOURCE_MEM,
	},
#endif
};

/*
 * Per port control structure
 */

static struct hw_pci soc_hw_pci[MAX_PCI_INTFS] __initdata = {
	{
		.domain 	= 0,
		.swizzle 	= pci_std_swizzle,
		.nr_controllers = 1,
		.setup 		= soc_pci_setup,
		.scan 		= soc_pci_scan_bus,
		.map_irq 	= (void *) soc_pcie_map_irq,
	},
	{
		.domain 	= 1,
		.swizzle 	= pci_std_swizzle,
		.nr_controllers = 1,
		.setup 		= soc_pci_setup,
		.scan 		= soc_pci_scan_bus,
		.map_irq 	= (void *) soc_pcie_map_irq,
	},
#if defined(CONFIG_MACH_NS) || defined(CONFIG_MACH_NSP)
	{
		.domain 	= 2,
		.swizzle 	= pci_std_swizzle,
		.nr_controllers = 1,
		.setup 		= soc_pci_setup,
		.scan 		= soc_pci_scan_bus,
		.map_irq 	= (void *) soc_pcie_map_irq,
	},
#endif
};

static struct soc_pcie_port {
	struct resource * regs_res ;
	struct resource * owin_res ;
	void * __iomem reg_base;
	unsigned short irqs[6];

	bool	enable;
	bool	link;
	bool	isSwitch;
	bool	port1Active;
	bool	port2Active;
} soc_pcie_ports[MAX_PCI_INTFS] = {
	{
	.regs_res = & soc_pcie_regs[0],
	.owin_res = & soc_pcie_owin[0],
#if defined(CONFIG_MACH_HX4)
	.irqs = {214, 215, 216, 217, 218, 219},
#elif defined(CONFIG_MACH_HR2)
	.irqs = {214, 215, 216, 217, 218, 219},
#elif defined(CONFIG_MACH_KT2)
	.irqs = {214, 215, 216, 217, 218, 219},
#elif defined(CONFIG_MACH_NS)
	.irqs = {159, 160, 161, 162, 163, 164},
#elif defined(CONFIG_MACH_NSP)
	.irqs = {159, 160, 161, 162, 163, 164},
#endif
	.enable = 1,
	.isSwitch = 0,
	.port1Active = 0,
	.port2Active = 0,
	},
	{
	.regs_res = & soc_pcie_regs[1],
	.owin_res = & soc_pcie_owin[1],
#if defined(CONFIG_MACH_HX4)
	.irqs = {220, 221, 222, 222, 224, 225},
#elif defined(CONFIG_MACH_HR2)
	.irqs = {220, 221, 222, 222, 224, 225},
#elif defined(CONFIG_MACH_KT2)
	.irqs = {220, 221, 222, 222, 224, 225},
#elif defined(CONFIG_MACH_NS)
	.irqs = {165, 166, 167, 168, 169, 170},
#elif defined(CONFIG_MACH_NSP)
	.irqs = {165, 166, 167, 168, 169, 170},
#endif
	.enable = 1,
	.isSwitch = 0,
	.port1Active = 0,
	.port2Active = 0,
	},
#if defined(CONFIG_MACH_NS) || defined(CONFIG_MACH_NSP)
	{
	.regs_res = & soc_pcie_regs[2],
	.owin_res = & soc_pcie_owin[2],
	.irqs = {171, 172, 173, 174, 175, 176},
	.enable = 1,
	.isSwitch = 0,
	.port1Active = 0,
	.port2Active = 0,
	}
#endif
	};



/*
 * Methods for accessing configuration registers
 */
static struct pci_ops soc_pcie_ops = {
        .read = soc_pci_read_config,
        .write = soc_pci_write_config,
};

static struct soc_pcie_port * 
	soc_pcie_sysdata2port( struct pci_sys_data * sysdata )
{
	unsigned port;

	port = sysdata->domain;
	BUG_ON( port >= ARRAY_SIZE( soc_pcie_ports ));
	return & soc_pcie_ports[ port ];
}

static struct soc_pcie_port * soc_pcie_pdev2port( struct pci_dev *pdev )
{
	return soc_pcie_sysdata2port( pdev->sysdata );
}

static struct soc_pcie_port * soc_pcie_bus2port( struct pci_bus * bus )
{
	return soc_pcie_sysdata2port( bus->sysdata );
}

static struct pci_bus __init *soc_pci_scan_bus(int nr, struct pci_sys_data *sys)
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 3, 0)
        return pci_scan_bus(sys->busnr, &soc_pcie_ops, sys);
#else
		return pci_scan_root_bus(NULL, sys->busnr, &soc_pcie_ops, sys,
					&sys->resources);
#endif
}

static int soc_pcie_map_irq(struct pci_dev *pdev, u8 slot, u8 pin)
{
        struct soc_pcie_port *port = soc_pcie_pdev2port(pdev);
//	u32 reg;
        int irq;

        irq = port->irqs[4];	/* All INTx share int src 5, last per port */

        printk("PCIe map irq: %04d:%02x:%02x.%02x slot %d, pin %d, irq: %d\n",
                pci_domain_nr(pdev->bus), 
		pdev->bus->number, 
		PCI_SLOT(pdev->devfn),
                PCI_FUNC(pdev->devfn), 
		slot, pin, irq);

        return irq;
}

static void __iomem * soc_pci_cfg_base(struct pci_bus *bus,
                                  unsigned int devfn, int where)
{
    struct soc_pcie_port *port = soc_pcie_bus2port(bus);
    int busno = bus->number;
    int slot = PCI_SLOT(devfn);
    int fn  = PCI_FUNC(devfn);
    void __iomem *base;
    int offset;
    int type;
	u32 addr_reg ;

	base = port->reg_base ;

        /* If there is no link, just show the PCI bridge. */
        if (!port->link && (busno > 0 || slot > 0))
                return NULL;
        /*
         */
	if (busno == 0) {
                if (slot >= 1)
                        return NULL;
                type = slot;
		__raw_writel( where & 0xffc, base + SOC_PCIE_EXT_CFG_ADDR );
		offset = SOC_PCIE_EXT_CFG_DATA;
	} else {
        type = 1;
		if (fn > 1)
			return NULL;
		addr_reg = 	(busno & 0xff) << 20 |
				(slot << 15) |
				(fn << 12)   |
				(where & 0xffc) |
				(type & 0x3);
 
		__raw_writel( addr_reg, base + SOC_PCIE_CFG_ADDR );
		offset =  SOC_PCIE_CFG_DATA ;
    }

    return base + offset;
}

void pcieSwitchInit( struct pci_bus *bus, unsigned int devfn)
{
    struct soc_pcie_port *port = soc_pcie_bus2port(bus);
    u32 dRead = 0;
    u32	bm = 0;

    soc_pci_read_config(bus, devfn, 0x100, 4, &dRead);

	printk("PCIE: Doing PLX switch Init...Test Read = %08x\n",(unsigned int)dRead);

	//Debug control register.
    soc_pci_read_config(bus, devfn, 0x1dc, 4, &dRead);

	dRead &= ~(1<<22);

    soc_pci_write_config(bus, devfn, 0x1dc, 4, dRead);

	//Set GPIO enables.
    soc_pci_read_config(bus, devfn, 0x62c, 4, &dRead);

	printk("PCIE: Doing PLX switch Init...GPIO Read = %08x\n",(unsigned int)dRead);

	dRead &= ~((1<<0)|(1<<1)|(1<<3));
	dRead |= ((1<<4)|(1<<5)|(1<<7));

    soc_pci_write_config(bus, devfn, 0x62c, 4, dRead);

	mdelay(50);
	dRead |= ((1<<0)|(1<<1));

    soc_pci_write_config(bus, devfn, 0x62c, 4, dRead);

    soc_pci_read_config(bus, devfn, 0x4, 2, &bm);
#if NS_PCI_DEBUG
	printk("bus master: %08x\n", bm);
#endif
	bm |= 0x06;
	soc_pci_write_config(bus, devfn, 0x4,2, bm);
	bm = 0;
    soc_pci_read_config(bus, devfn, 0x4, 2, &bm);
	printk("bus master after: %08x\n", bm);
	bm =0;
	//Bus 1 if the upstream port of the switch. Bus 2 has the two downstream ports, one on each device number.
	if(bus->number == 1)
	{
		soc_pci_write_config(bus, devfn, 0x18, 4, pcieSwitchPrimSecBusNum);

		//TODO: We need to scan all outgoing windows, to look for a base limit pair for this register.
		//npciConfigOutLong(instance, busNo, deviceNo, 0, 0x20,0xcff0c000);
		/* MEM_BASE, MEM_LIM require 1MB alignment */
		BUG_ON( (port->owin_res->start   >> 16) & 0xf );
		soc_pci_write_config(bus, devfn, PCI_MEMORY_BASE, 2,  
		port->owin_res->start   >> 16 );
		BUG_ON(((port->owin_res->start + SZ_32M) >> 16 ) & 0xf );
		soc_pci_write_config(bus, devfn, PCI_MEMORY_LIMIT, 2, 
		(port->owin_res->start + SZ_32M) >> 16 );

	}
	else if(bus->number == 2)
	{
		//TODO: I need to fix these hard coded addresses.
		if(devfn == 0x8)
		{
			soc_pci_write_config(bus, devfn, 0x18, 4, (0x00000000 | ((bus->number+1)<<16) | ((bus->number+1)<<8) | bus->number));
			BUG_ON( ((port->owin_res->start + SZ_48M)   >> 16) & 0xf );
			soc_pci_write_config(bus, devfn, PCI_MEMORY_BASE, 2, 
					     (port->owin_res->start + SZ_48M)
					     >> 16 );
			BUG_ON(((port->owin_res->start + SZ_48M + SZ_32M) >> 16 ) & 0xf );
			soc_pci_write_config(bus, devfn, PCI_MEMORY_LIMIT, 2, 
			(port->owin_res->start + SZ_48M + SZ_32M) >> 16 );
			soc_pci_read_config(bus, devfn, 0x7A, 2, &bm);
			if (bm & PCI_EXP_LNKSTA_DLLLA)
				port->port1Active = 1;
			printk("bm = %04x\n devfn = = %08x, bus = %08x\n", bm, devfn, bus->number); 
		}
		else if(devfn == 0x10)
		{
			soc_pci_write_config(bus, devfn, 0x18, 4, (0x00000000 | ((bus->number+2)<<16) | ((bus->number+2)<<8) | bus->number));
			BUG_ON( ((port->owin_res->start + (SZ_48M * 2 ))  >> 16) & 0xf );
			soc_pci_write_config(bus, devfn, PCI_MEMORY_BASE, 2,  
					     (port->owin_res->start
					      + (SZ_48M * 2 ))   >> 16 );
			BUG_ON(((port->owin_res->start + (SZ_48M * 2 ) + SZ_32M) >> 16 ) & 0xf );
			soc_pci_write_config(bus, devfn, PCI_MEMORY_LIMIT, 2,  
			(port->owin_res->start + (SZ_48M * 2 ) + SZ_32M) >> 16 );
			soc_pci_read_config(bus, devfn, 0x7A, 2, &bm);
			if (bm & PCI_EXP_LNKSTA_DLLLA)
				port->port2Active = 1;
			printk("bm = %04x\n devfn = = %08x, bus = %08x\n", bm, devfn, bus->number); 
		}
	}
}

static int soc_pci_read_config(struct pci_bus *bus, unsigned int devfn,
                                   int where, int size, u32 *val)
{
    void __iomem *base;
	u32 data_reg;
//	u16 tmp16;
    struct soc_pcie_port *port = soc_pcie_bus2port(bus);

	if ((bus->number > 4))
	{
		*val = ~0UL;
		return PCIBIOS_SUCCESSFUL;
	}
	if (port->isSwitch == 1)
	{
		if (bus->number == 2)
		{
			if (!((devfn == 0x8) || (devfn == 0x10)))
			{
				*val = ~0UL;
				return PCIBIOS_SUCCESSFUL;
			}
		}
		else if ((bus->number == 3) || (bus->number == 4))
		{
			if (devfn != 0)
			{
				*val = ~0UL;
				return PCIBIOS_SUCCESSFUL;
			}
			else if ((bus->number == 3) && (port->port1Active == 0))
			{
				*val = ~0UL;
				return PCIBIOS_SUCCESSFUL;
			}
			else if ((bus->number == 4) && (port->port2Active == 0))
			{
				*val = ~0UL;
				return PCIBIOS_SUCCESSFUL;
			}
		}
	}
	base = soc_pci_cfg_base(bus, devfn, where);

    if (base == NULL )
	{
            *val = ~0UL;
            return PCIBIOS_SUCCESSFUL;
	}

#if NS_PCI_DEBUG
	printk("PCI-E: R: bus %08x, where %08x, devfn %08x\n", bus->number, where, devfn);
#endif
	data_reg = __raw_readl( base );
#if NS_PCI_DEBUG
	printk("PCI-E: R: data_reg %08x\n", data_reg);
#endif


	if ((bus->number == 1) && (port->isSwitch == 0) && (where == 0) && (((data_reg >> 16) & 0x0000FFFF) == 0x00008603))
	{
		pcieSwitchInit( bus, devfn);
		port->isSwitch = 1;
	}

	if ((bus->number == 2) && (port->isSwitch == 1) && (where == 0) &&(((data_reg >> 16) & 0x0000FFFF) == 0x00008603))
		pcieSwitchInit(bus, devfn);

	/* HEADER_TYPE=00 indicates the port in EP mode */

	data_reg = (data_reg) >> ((where & 3) * 8);
	*val = data_reg;
    return PCIBIOS_SUCCESSFUL;
}

static int soc_pci_write_config(struct pci_bus *bus, unsigned int devfn,
                                    int where, int size, u32 val)
{
    void __iomem *base;
	u32  data_reg ;
	int saveWhere;
    struct soc_pcie_port *port = soc_pcie_bus2port(bus);

	saveWhere = where;
	if ((bus->number > 4))
	{
		return PCIBIOS_SUCCESSFUL;
	}
	if ((bus->number == 2) && (port->isSwitch == 1 ))
	{
		if (!((devfn == 0x8) || (devfn == 0x10)))
		{
			return PCIBIOS_SUCCESSFUL;
		}
	}
	else if ((bus->number == 3) && (port->isSwitch == 1))
	{
		if (devfn != 0)
			return PCIBIOS_SUCCESSFUL;
	}
	else if ((bus->number == 4) && (port->isSwitch == 1))
	{
		if (devfn != 0)
		{
			return PCIBIOS_SUCCESSFUL;
		}
	}
	base = soc_pci_cfg_base(bus, devfn, where);

    if (base == NULL)
	{
            return PCIBIOS_SUCCESSFUL;
	}

	if( size < 4 ){
		where = (where & 3) * 8;

#if NS_PCI_DEBUG
		printk("PCI-E: WR: bus %08x, where %08x, devfn %08x, size: %08x\n", bus->number, saveWhere, devfn, size);
#endif
		data_reg = __raw_readl( base );
#if NS_PCI_DEBUG
		printk("PCI-E: WR: after data_reg %08x\n", data_reg);
#endif

		if (size == 1)
			data_reg &= ~(0xff << where);
		else
			data_reg &= ~(0xffff << where);
		data_reg |= (val << where);
	}
	else{
		data_reg = val;
	}

#if NS_PCI_DEBUG
	printk("PCI-E: W: bus %08x, where %08x, devfn %08x, data_reg %08x, size: %08x\n", bus->number, saveWhere, devfn, data_reg, size);
#endif
	__raw_writel( data_reg, base );
#if NS_PCI_DEBUG
	printk("PCI-E: W: after\n");
#endif

    return PCIBIOS_SUCCESSFUL;
}

static int soc_pci_setup(int nr, struct pci_sys_data *sys)
{
        struct soc_pcie_port *port = soc_pcie_sysdata2port(sys);

	BUG_ON( request_resource( &iomem_resource, port->owin_res ));

#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 3, 0)
	sys->resource[0] = port->owin_res ;
	//sys->private_data = port;
#else
	pci_add_resource(&sys->resources, port->owin_res);
#endif

        return 1;
}

/*
 * Check link status, return 0 if link is up in RC mode,
 * otherwise return non-zero
 */
static int __init noinline soc_pcie_check_link(unsigned index)
{
        u32 devfn = 0;
	u16 pos, tmp16;
	u8 nlw, tmp8;
	u32 tmp32;
	struct soc_pcie_port * port = &soc_pcie_ports[index];
        struct pci_sys_data sd = {
                .domain = soc_hw_pci[index].domain,
        };
        struct pci_bus bus = {
                .number = 0,
                .ops = &soc_pcie_ops,
                .sysdata = &sd,
        };

	if( ! port->enable )
		return -EINVAL;

	/* See if the port is in EP mode, indicated by header type 00 */
        pci_bus_read_config_byte(&bus, devfn, PCI_HEADER_TYPE, &tmp8);
	if( tmp8 != PCI_HEADER_TYPE_BRIDGE ) {
		pr_info("PCIe port %d in End-Point mode - ignored\n",
			soc_hw_pci[index].domain );
		return -ENODEV;
	}

	/* 
	* Under RC mode, write to function specific register 0x43c, to change
	* the CLASS code in configuration space 
	*/
	pci_bus_read_config_dword(&bus, devfn, 0x43c, &tmp32);
	tmp32 = (tmp32 & 0xff0000ff) | (PCI_CLASS_BRIDGE_PCI << 8);
	pci_bus_write_config_dword(&bus, devfn, 0x43c, tmp32);
	/* 
	* After this modification, the CLASS code in configuration space would be
	* read as PCI_CLASS_BRIDGE_PCI(0x0604) instead of network interface(0x0200) 
	*/


	/* NS PAX only changes NLW field when card is present */
        pos = pci_bus_find_capability(&bus, devfn, PCI_CAP_ID_EXP);
        pci_bus_read_config_word(&bus, devfn, pos + PCI_EXP_LNKSTA, &tmp16);

	printk("PCIE%d: LINKSTA reg %#x val %#x\n", soc_hw_pci[index].domain,
		pos+PCI_EXP_LNKSTA, tmp16 );

	nlw = (tmp16 & PCI_EXP_LNKSTA_NLW) >> PCI_EXP_LNKSTA_NLW_SHIFT ;
	port->link = tmp16 & PCI_EXP_LNKSTA_DLLLA ;

	if( nlw != 0 ) port->link = 1;

	for( ; pos < 0x100; pos += 2 )
		{
        	pci_bus_read_config_word(&bus, devfn, pos , &tmp16);
			if( tmp16 ) printk("reg[%#x]=%#x, ", pos , tmp16 );
		}
	if (nlw == 0)
	{
		/* try gen 1 */
		pci_bus_read_config_dword(&bus, devfn, 0xdc, &tmp32);
#if NS_PCI_DEBUG
		printk("\nLink status control 2 register 0xdc: %08x\n", tmp32);
#endif
		if ((tmp32 & 0xf) == 2)
		{
			tmp32 &= 0xfffffff0;
			tmp32 |= 0x1;
			pci_bus_write_config_dword(&bus, devfn, 0xdc, tmp32);
			pci_bus_read_config_dword(&bus, devfn, 0xdc, &tmp32);
			mdelay(100);
		/* NS PAX only changes NLW field when card is present */
			pos = pci_bus_find_capability(&bus, devfn, PCI_CAP_ID_EXP);
			pci_bus_read_config_word(&bus, devfn, pos + PCI_EXP_LNKSTA, &tmp16);
			nlw = (tmp16 & PCI_EXP_LNKSTA_NLW) >> PCI_EXP_LNKSTA_NLW_SHIFT ;
			port->link = tmp16 & PCI_EXP_LNKSTA_DLLLA ;
#if NS_PCI_DEBUG
			printk("Link status control 2 register 0xdc: %08x, nlw: %08x, port->link: %08x\n", tmp32, nlw, port->link);
#endif
			if( nlw != 0 ) port->link = 1;
		}
	}
	printk("PCIE%d link=%d\n", soc_hw_pci[index].domain,  port->link );

	return( (port->link)? 0: -ENOSYS );
}

/*
 * Initializte the PCIe controller
 */
static void __init soc_pcie_hw_init(struct soc_pcie_port * port)
{
	/* Turn-on Root-Complex (RC) mode, from reset defailt of EP */

	/* The mode is set by straps, can be overwritten via DMU
	   register <cru_straps_control> bit 5, "1" means RC
	 */

	/* Send a downstream reset */
	__raw_writel( 0x3, port->reg_base + SOC_PCIE_CONTROL);
	udelay(250);
	__raw_writel( 0x1, port->reg_base + SOC_PCIE_CONTROL);
	mdelay(250);

	/* TBD: take care of PM, check we're on */
}

/*
 * Setup the address translation
 */
static void __init soc_pcie_map_init(struct soc_pcie_port * port)
{
/*
* Disabling the address translation won't disable PCI memory read/write function,
* it just won't do address translation before or after PCIE memory transaction.
*
* As a PCIE RC driver, the OMAP/OARR could be disabled since we set OMAP/OARR
* the same value of PCIE window range. This takes the same effect as no address 
* translation.
*
* As a PCIE RC drive, the IMAP/IARR could be disabled and let the device decide which
* address to write to. After all we have no idea about the device's intention.
*
* It seems only PCIE EP mode is required to set the OMAP/OARR/IMAP/IARR function.
*
* @ Setting of IMAP/IARR
* 1. The setting of IMAP/IARR below is reversed, so the IMAP function is disabled 
* actually(the valid bit is set to size and becomes 0). 
* 2. We should take notice that the address shall be set to size-aligned, or the address
* translation might not be what we expect. For example, if the size is set to 128M, 
* the address should be set to 128M aligned.
*/

}

/*
 * Setup PCIE Host bridge
 */
#if 0
static void __init noinline soc_pcie_bridge_init(unsigned index)
{
        u32 devfn = 0;
        u8 tmp8;
	u16 tmp16;
	struct soc_pcie_port * port = &soc_pcie_ports[index];
	/* Fake <bus> object */
        struct pci_sys_data sd = {
                .domain = soc_hw_pci[index].domain,
        };
        struct pci_bus bus = {
                .number = 0,
                .ops = &soc_pcie_ops,
                .sysdata = &sd,
        };


	/*
	* PCI_PRIMARY_BUS, PCI_SECONDARY_BUS, and PCI_SUBORDINATE_BUS would be
	* set in ARM PCI enumeration process(pci_common_init). If connected to one 
	* PCIe device only, the PCI_PRIMARY_BUS, PCI_SECONDARY_BUS, and
	* PCI_SUBORDINATE_BUS would be set to 0, 1, 1 respectively.
	*/
        pci_bus_write_config_byte(&bus, devfn, PCI_PRIMARY_BUS, 0);
        pci_bus_write_config_byte(&bus, devfn, PCI_SECONDARY_BUS, 1);
        pci_bus_write_config_byte(&bus, devfn, PCI_SUBORDINATE_BUS, 4);

        pci_bus_read_config_byte(&bus, devfn, PCI_PRIMARY_BUS, &tmp8);
        pci_bus_read_config_byte(&bus, devfn, PCI_SECONDARY_BUS, &tmp8);
        pci_bus_read_config_byte(&bus, devfn, PCI_SUBORDINATE_BUS, &tmp8);

	/* MEM_BASE, MEM_LIM require 1MB alignment */
	BUG_ON( (port->owin_res->start   >> 16) & 0xf );
	printk("%s: membase %#x memlimit %#x\n", __FUNCTION__,
		port->owin_res->start, port->owin_res->end+1);
        pci_bus_write_config_word(&bus, devfn, PCI_MEMORY_BASE, 
		port->owin_res->start   >> 16 );
	BUG_ON(((port->owin_res->end+1) >> 16 ) & 0xf );
        pci_bus_write_config_word(&bus, devfn, PCI_MEMORY_LIMIT, 
		(port->owin_res->end) >> 16 );
	/*
	* Set resource->end to MEMORY_LIMIT instead of resource->end+1, 
	* PCI bridge spec depicted that the upper 12 bits of both MEMORY_BASE and 
	* MEMORY_LIMIT would be take as 0x###00000 and 0x###FFFFF respectively.
	* ARM PCI enumeration process(pci_common_init) would set MEMORY_BASE and
	* MEMORY_LIMIT(or PREF_MEMORY_BASE and PREF_MEMORY_LIMIT) to the total
	* window size that's assigned to the devices.
	*/


	/* These registers are not supported on the NS */
        pci_bus_write_config_word(&bus, devfn, PCI_IO_BASE_UPPER16, 0);
        pci_bus_write_config_word(&bus, devfn, PCI_IO_LIMIT_UPPER16, 0);

	/* Force class to that of a Bridge */
        pci_bus_write_config_word(&bus, devfn, PCI_CLASS_DEVICE,
		PCI_CLASS_BRIDGE_PCI);

        pci_bus_read_config_word(&bus, devfn, PCI_CLASS_DEVICE, &tmp16);
        pci_bus_read_config_word(&bus, devfn, PCI_MEMORY_BASE, &tmp16);
        pci_bus_read_config_word(&bus, devfn, PCI_MEMORY_LIMIT, &tmp16);
	
}
#endif

#if defined(CONFIG_MACH_NS) || defined(CONFIG_MACH_NSP)

static void __init
bcm5301x_3rd_pcie_init(void)
{
	uint32_t cru_straps_ctrl;
	void __iomem *ccb_mii_base;
	void __iomem *dmu_base;
	void __iomem *ccb_mii_mng_ctrl_addr;
	void __iomem *ccb_mii_mng_cmd_data_addr;

	/* Reg map */
	dmu_base = IOMEM(IPROC_DMU_BASE_VA);

	/* Check strapping of PCIE/USB3 SEL */
	cru_straps_ctrl = readl(dmu_base + IPROC_DMU_STRAPS_OFFSET);
	/* PCIE mode is not selected */
	if (cru_straps_ctrl & 0x10)
		return;

	/* Reg map */
	ccb_mii_base = IOMEM(IPROC_CCB_MDIO_REG_VA);
	ccb_mii_mng_ctrl_addr = ccb_mii_base + 0x0;
	ccb_mii_mng_cmd_data_addr = ccb_mii_base + 0x4;

	/* MDIO setting. set MDC-> MDCDIV is 7'd8 */
	writel(0x00000088, ccb_mii_mng_ctrl_addr);
	udelay(250);
	if (!(readl(ccb_mii_mng_ctrl_addr) && 0x100)) {
		printk(KERN_ERR "%s: Error seting MII ctrl_addr \n",
		       __func__);
	}
	/* PCIE PLL block register (base 0x8000) */
	writel(0x57fe8000, ccb_mii_mng_cmd_data_addr);
	udelay(250);
	if (!(readl(ccb_mii_mng_ctrl_addr) && 0x100)) {
		printk(KERN_ERR "%s: Error seting PCIE PLL block register\n",
		       __func__);
	}
	/* Check PCIE PLL lock status */
	writel(0x67c60000, ccb_mii_mng_cmd_data_addr);
	udelay(250);
	if (!(readl(ccb_mii_mng_ctrl_addr) && 0x100)) {
		printk(KERN_ERR "%s: Error checking PCIE PLL lock status\n",
		       __func__);
	}
}

#else

static void __init
bcm5301x_3rd_pcie_init(void)
{
}
#endif

static int __init soc_pcie_init(void)
{
        unsigned i;

	bcm5301x_3rd_pcie_init();
        for (i = 0; i < ARRAY_SIZE(soc_pcie_ports); i++)
		{
		struct soc_pcie_port * port = &soc_pcie_ports[i];
		
		/* Check if this port needs to be enabled */
		if( ! port->enable )
			continue;
		/* Setup PCIe controller registers */
		BUG_ON( request_resource( &iomem_resource, port->regs_res ));
		port->reg_base =
			ioremap( port->regs_res->start, 
			resource_size(port->regs_res) );
		BUG_ON( IS_ERR_OR_NULL(port->reg_base ));

                soc_pcie_hw_init( port );
		
		/* 
		* move soc_pcie_map_init after soc_pci_check_link function, 
		* since soc_pci_check_link function would have a check on 
		* RC or EP mode. And the soc_pcie_map_init function is trying
		* to set the mapping address under RC mode.
		*/
		/* soc_pcie_map_init( port ); */

		/*
		* Skip inactive ports -
		* will need to change this for hot-plugging
		*/
                if( soc_pcie_check_link(i) != 0 )
				{
					continue;
				}

		soc_pcie_map_init( port );

		/*
		* What's set in soc_pcie_bridge_init function are overwritten in pci 
		* enumeration, and the value might be different to what's done in 
		* this function. 
		*/
                /* soc_pcie_bridge_init( i ); */

		/* Announce this port to ARM/PCI common code */
                pci_common_init(&soc_hw_pci[i]);

		/* Setup virtual-wire interrupts */
		__raw_writel(0xf, port->reg_base + SOC_PCIE_SYS_RC_INTX_EN );
        	}

        pci_assign_unassigned_resources();

        return 0;
}

device_initcall(soc_pcie_init);

#endif	/* CONFIG_PCI */
