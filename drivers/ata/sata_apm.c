/*
 * sata_apm.c - AppliedMicro SATA PHY driver
 *
 * Copyright (c) 2010, Applied Micro Circuits Corporation
 * Author: Ravi Patel <rapatel@apm.com>
 *         Sourabh Daptardar <sdaptardar@apm.com> 
 *         Tanmay Inamdar <tinamdar@apm.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 *
 */

#include <linux/module.h>
#include <linux/slab.h>
#include <linux/clk.h>
#include <asm/apm86xxx_pm.h>
#include <asm/ipp.h>
#include "ahci.h"
#include "sata_apm.h"

#undef PHY_DEBUG
#undef SATA_DEBUG

#ifdef PHY_DEBUG
#define PHYDEBUG(fmt, ...)		\
		printk(KERN_INFO fmt "\n", ##__VA_ARGS__);
#else
#define PHYDEBUG(fmt, ...)
#endif
#ifdef SATA_DEBUG
#define SATADEBUG(fmt, ...)		\
		printk(KERN_INFO fmt "\n", ##__VA_ARGS__);
#else
#define SATADEBUG(fmt, ...)
#endif

#define SATA_PLL_WAIT_TO	50	/* Loop count to PLL lock time out */

#define SATA_SEQ_AFTER_PLL_LOCK		1	
#define SATA_SEQ_AFTER_LINUX_COMRESET	2
#define SATA_SEQ_AFTER_FAST_COMRESET	3	
#define SATA_SEQ_AFTER_PM		4
#define SATA_SEQ_AFTER_APM_LINKUP	5
#define SATA_SEQ_AFTER_APM_SOFTRESET	6

static struct ata_port_operations ahci_ops_apm;
static struct scsi_host_template ahci_sht_apm = {
       AHCI_SHT("ahci_of"),
};
static struct ata_host *sata_apm_host = NULL;
static struct apm_sata_pdata *sata_apm_pdata = NULL;
static int apmsatadefaultspd = 0x00000022;

int of_ahci_post_reset_controller(struct ata_host *host);
	
static u32 sata_byte_swap(u32 data) 
{
	u32 swap_data, tmp_data1, tmp_data2, tmp_data3, tmp_data4;

	tmp_data1 = (data & 0xFF000000) >> 24;
	tmp_data2 = (data & 0x00FF0000) >> 8;
	tmp_data3 = (data & 0x0000FF00) << 8;
	tmp_data4 = (data & 0x000000FF) << 24;
	swap_data  = tmp_data1 + tmp_data2 + tmp_data3 + tmp_data4;

	return swap_data;
}

/* NOTE:
 * In a typical case the values are to be written in Little Endian
 * Format but due to the byte swapping issue we are using big endian
 * functions to read */
static inline u32 sata_csr_readl(void __iomem *addr)
{
	u32 val;
	val = in_be32(addr);
        PHYDEBUG("SATA CSR RD: 0x%p = 0x%08x", addr, val);
	return val;
}

/* NOTE:
 * In a typical case the values are to be written in Little Endian
 * Format but due to the byte swapping issue we are using big endian
 * functions to write */
static inline void sata_csr_writel(u32 val, void __iomem *addr)
{
	PHYDEBUG("SATA CSR WR: 0x%p = 0x%08x", addr, val);
	out_be32(addr, val);
/*	udelay(10); */
}

void apm_pcs_write(void __iomem *pcs_base, u32 pcs_reg, 
				u32 pcs_val)
{
	PHYDEBUG("SATA PCS WR: 0x%p = 0x%08x", 
		(u8 *) pcs_base + pcs_reg, pcs_val);
	out_be32((void __iomem *)((u8 *)pcs_base + pcs_reg), pcs_val);
/*	udelay(10); */
}

u32 apm_pcs_read(void __iomem *pcs_base, u32 pcs_reg)
{
	u32 val;
	val = in_be32((void __iomem *)((u8 *) pcs_base + pcs_reg));
	PHYDEBUG("SATA PCS RD: 0x%p = 0x%08x", (u8 *) pcs_base + pcs_reg, val);
	return val;	
}

static int apm_pcs_phy_read(void __iomem *pcs_base, u32 phy_addr, u32 *phy_val)
{
	int i;
	u32 temp;

	if (pcs_base == NULL)
		return -1;

	apm_pcs_write(pcs_base, PCSCSR_REGADDR_ADDR, phy_addr);
	apm_pcs_write(pcs_base, PCSCSR_REGCMD_ADDR, MGMT_PCS_REG_RD_MASK);
	asm volatile("msync");
	i = SATA_WAIT_TO;

	do {
		temp = apm_pcs_read(pcs_base, PCSCSR_REGCMDDONE_ADDR); 
		if ((temp & MGMT_PCS_REG_RD_DONE_MASK) != 0)
			break;
/*		udelay(1000); */
	} while (i--);

	if (temp & MGMT_PCS_REG_RD_DONE_MASK) {
		*phy_val = apm_pcs_read(pcs_base, PCSCSR_REGRDDATA_ADDR);
		PHYDEBUG("SATA PHY RD: 0x%p 0x%08x = 0x%08x", 
			(u8 *) pcs_base, phy_addr, *phy_val);
		return 0;
	} 
	PHYDEBUG("SATA PHY RD: 0x%p 0x%08x = 0x%08x failed", 
		(u8 *) pcs_base, phy_addr, *phy_val);
	return -1;
}

static int apm_pcs_phy_write(void __iomem *pcs_base, u32 phy_addr, u32 phy_val)
{
	int i;
	u32 temp;

	if (pcs_base == NULL)
		return -1;

	apm_pcs_write(pcs_base, PCSCSR_REGADDR_ADDR, phy_addr);
	apm_pcs_write(pcs_base, PCSCSR_REGWRDATA_ADDR, phy_val);
	apm_pcs_write(pcs_base, PCSCSR_REGCMD_ADDR, MGMT_PCS_REG_WR_MASK);
	asm volatile("msync");
	
	i = SATA_WAIT_TO;

	do {
		temp = apm_pcs_read(pcs_base, PCSCSR_REGCMDDONE_ADDR); 
		if ((temp & MGMT_PCS_REG_WR_DONE_MASK) != 0)
			break;
/*		udelay(1000); */
	} while (i--);

	if (temp & MGMT_PCS_REG_WR_DONE_MASK) {
		PHYDEBUG("SATA PHY WR: 0x%p 0x%08x = 0x%08x", 
			(u8 *) pcs_base, phy_addr, phy_val);
		return 0;
	}
	PHYDEBUG("SATA PHY WR: 0x%p 0x%08x = 0x%08x failed", 
		(u8 *) pcs_base, phy_addr, phy_val);
	return -1;
}

u32 sata_apm_csr_readl(u32 offset)
{
	struct ahci_host_priv *hpriv; 
	void __iomem *csr_mmio; 
	
	if (sata_apm_host == NULL)
		return 0xFFFFFFFF;	
	hpriv = sata_apm_host->private_data;
	if (hpriv == NULL)
		return 0xFFFFFFFF;	
	csr_mmio = hpriv->mmio;
	return sata_csr_readl(csr_mmio + offset);
}

u32 sata_apm_port_csr_readl(u32 port, u32 offset)
{
	struct ata_port *ap; 
	void __iomem *port_mmio; 
	
	if (sata_apm_host == NULL)
		return 0xFFFFFFFF;
		
	ap = sata_apm_host->ports[port];
	port_mmio = ahci_port_base(ap);
	return sata_csr_readl(port_mmio + offset);
}

void sata_apm_port_csr_writel(u32 port, u32 offset, u32 val)
{
	struct ata_port *ap; 
	void __iomem *port_mmio; 
	
	if (sata_apm_host == NULL)
		return;

	ap = sata_apm_host->ports[port];
	port_mmio = ahci_port_base(ap);
	sata_csr_writel(val, port_mmio + offset);
}

void sata_apm_pcs_readl(u32 port, u32 phy_addr)
{
	struct apm_sata_pdata *apm_pdata; 
	void __iomem *serdes_mmio; 
	
	if (sata_apm_host == NULL)
		return;	
	apm_pdata = sata_apm_host->dev->platform_data;
	if (apm_pdata == NULL)
		return;
	serdes_mmio = apm_pdata->apm_sata_pcs[port];
	apm_pcs_read(serdes_mmio, phy_addr);
}

u32 sata_apm_pcs_phy_readl(u32 port, u32 phy_addr)
{
	u32 phy_val;
	struct apm_sata_pdata *apm_pdata; 
	void *__iomem *serdes_mmio; 
	
	if (sata_apm_host == NULL)
		return 0xFFFFFFFF;
	apm_pdata = sata_apm_host->dev->platform_data;
	if (apm_pdata == NULL)
		return 0xFFFFFFFF;
	serdes_mmio = apm_pdata->apm_sata_pcs[port];
	apm_pcs_phy_read(serdes_mmio, phy_addr, &phy_val);
	return phy_val;
}

void sata_apm_pcs_phy_writel(u32 port, u32 phy_addr, u32 phy_val)
{
	struct apm_sata_pdata *apm_pdata;
	void *__iomem *serdes_mmio; 
	
	if (sata_apm_host == NULL)
		return;
	apm_pdata = sata_apm_host->dev->platform_data;
	if (apm_pdata == NULL)
		return;
	serdes_mmio = apm_pdata->apm_sata_pcs[port];
	apm_pcs_phy_write(serdes_mmio, phy_addr, phy_val);
}

static void apm_pcs_phy_enable(void __iomem *pcs_base, int enable)
{
	u32 phy_val = 0;

	/* Place phy in reset */
	apm_pcs_phy_read(pcs_base, PCSCSR_CTL0_ADDR, &phy_val);
	if (enable) 
		phy_val |= RESET0_F2_WR(1);
	else
		phy_val &= ~RESET0_F2_WR(1);
	apm_pcs_write(pcs_base, PCSCSR_CTL0_ADDR, phy_val);
}

static int apm_init_diag(void __iomem *diag_base)
{
	unsigned int val;
	int count = 1000;

	val = sata_csr_readl(diag_base + 0x70);

	if (val) {
		sata_csr_writel(0x0, diag_base + 0x70);

		while (count--) {
			val = sata_csr_readl(diag_base + 0x70);
			sata_csr_readl(diag_base + 0x74);

			if (val == 0) {
				goto exit_init_diag;
			}
		}
	}
	val = -1;
	
exit_init_diag:
	return val;
}

static int apm_sata_get_port_idx(struct ata_host *host, struct ata_port *port)
{
	u32 max_ports;
	struct ata_port *cur_port;
	int i;

	max_ports = host->n_ports;

	for (i = 0; i < max_ports; i++) {
		cur_port = host->ports[i];
		if (cur_port == port)
			return i;
	}

	return -1;
}

#ifdef CONFIG_SYSFS
static ssize_t show_regdump(struct device_driver *drv, char *buf)
{
        ssize_t size = 0;
	u32 val;
	u32 port = 0;
	int i;
	u32 reg2dump[] = {
		0x5800c, 
		0x5800d,
		0x5800e,
		0x5800f,
		0x58010,
		0x58011,
		0x5c000,
		0x5c001,
		0x5c002,
		0x5c004,
		0x5c005,
		0x5c012,
		0x5c013,
		0x5c020,
		0x5c065,
		0x58024,
		0xf8001,
		0xf8002,
		0xf8003,
		0xf80e7,
		0xf80ea,
		0	/* terminator */
	};
	
	for (port = 0; port < 2; port++) {
		if (buf)
			size += snprintf(buf + size, PAGE_SIZE - size,
				"Serdes Register Dump (Port %d):\n", port);
		else
			printk(KERN_INFO 
				"Serdes Register Dump (Port %d):\n", port);
		for (i = 0; reg2dump[i] != 0; i++) {
			val = sata_apm_pcs_phy_readl(port, reg2dump[i]);
			if (buf)
				size += snprintf(buf + size, PAGE_SIZE - size,
					"0x%x: 0x%x\n", reg2dump[i], val);
			else
				printk(KERN_INFO 
					"0x%x:  0x%x\n", reg2dump[i], val);
		}
	}
	return size;
}

static struct driver_attribute sata_attrs[] = {
	__ATTR(regdump, S_IRUGO, show_regdump, NULL),
};
static int add_sata_sysfs(struct device_driver *driver)
{
	int i;
	int err;

	for (i = 0; i < ARRAY_SIZE(sata_attrs); i++) {
		err = driver_create_file(driver, &sata_attrs[i]);
		if (err) goto fail;
	}
	return 0;

fail:
	while (--i >= 0) {
		driver_remove_file(driver, &sata_attrs[i]);
	}
	return err;
}
#endif

int of_ahci_is_apm_host(struct ata_host *host, struct apm_sata_pdata *pdata)
{
	if (host == NULL) 
		return pdata == sata_apm_pdata ? 1 : 0;
	if (pdata == NULL)
		return host == sata_apm_host ? 1 : 0;
	if (host == sata_apm_host && pdata == sata_apm_pdata)
		return 1;	
	else
		return 0;
}

int apm_ahci_prog_serdes(struct ata_link *link, int port_no, u32 speed, int arg)
{
	struct ata_port *port;
	struct ata_host *host;
	int port_index;

	if (link != NULL) {
	 	port = link->ap;
	  	host = port->host;
	  	port_index = apm_sata_get_port_idx(host, port);
	} else {
		port_index = port_no;
	}
	if (port_index == -1)
		return -1;

	switch (arg) {
	case SATA_SEQ_AFTER_PLL_LOCK:
#if defined(CONFIG_APM862xx)
		/* For APM862xx, Serdes gets reset on AHCI controller reset.
		   No need to program. */	
		return -1;
#else
		/* For non-APM862xx, program Serdes as soon as possible */
		break;
#endif				
	case SATA_SEQ_AFTER_LINUX_COMRESET:
	case SATA_SEQ_AFTER_FAST_COMRESET:
#if defined(CONFIG_APM862xx)
		/* For APM862xx, as Serdes gets reset on COMRESET. 
		   Re-program Serdes immediately after COMRESET. */
		break;
#else
		/* No needed for non-APM862xx */
		return -1;
#endif
	case SATA_SEQ_AFTER_APM_LINKUP:
	case SATA_SEQ_AFTER_APM_SOFTRESET:
		/* Link up after COMRESET. Re-program only if different. */
		if (sata_apm_pdata->serdes_gen[port_index] == speed)
			return -1;
		break;
	case SATA_SEQ_AFTER_PM:
		/* Power management resume */
		break;	/* break to program Serdes */
	default:
		return -1;		
	}	
	
	/* Use default if speed is NOT available */
	if (speed == 0)
		speed = (apmsatadefaultspd >> (port_index * 4)) & 0xF;
		
	if (speed == 1)  {
		SATADEBUG("APMSATA%d: Program PHY Serdes GenI", port_index);
		sata_apm_pdata->serdes_gen[port_index] = speed;
		
#if defined(CONFIG_APM862xx)
		/* Invert GenI clock */
		sata_apm_pcs_phy_writel(port_index, 0xf8001, 0x8034);
#endif		
		
		/* Tx swing and ibias */		
		sata_apm_pcs_phy_writel(port_index, 0x5800d, 0x2064);
		/* Swing/ibias and pre-emphasis override enable */
		sata_apm_pcs_phy_writel(port_index, 0x5800e, 0x0f83);
		/* Tx max slew rate */
		sata_apm_pcs_phy_writel(port_index, 0x5800f, 0x2001);
		/* Tx pre-emphasis */
		sata_apm_pcs_phy_writel(port_index, 0x58010, 0x6037);
		/* Rx peak and pole */
		sata_apm_pcs_phy_writel(port_index, 0x5800c, 0x65);
		/* Rx phase interpolator divider */
		sata_apm_pcs_phy_writel(port_index, 0x5c012, 0x300);		
	} else /* if (speed == 2) */ {
		SATADEBUG("APMSATA%d: Program PHY Serdes GenII", port_index);
		sata_apm_pdata->serdes_gen[port_index] = 2;
		
#if defined(CONFIG_APM862xx)	
		/* Invert GenI clock. Do it for Gen2 as well as initially,
		   we don't know it is Gen1 or Gen2 */
		sata_apm_pcs_phy_writel(port_index, 0xf8001, 0x8034);
#endif				
		/* Tx swing and ibias */		
#if defined(CONFIG_APM862xx)	
		sata_apm_pcs_phy_writel(port_index, 0x5800d, 0x205c);
#else
		sata_apm_pcs_phy_writel(port_index, 0x5800d, 0x207f);
#endif
		/* Swing/ibias and pre-emphasis override enable */
		sata_apm_pcs_phy_writel(port_index, 0x5800e, 0x0f83);		
		/* Tx max slew rate */
		sata_apm_pcs_phy_writel(port_index, 0x5800f, 0x4001);		
		/* Tx pre-emphasis */
#if defined(CONFIG_APM862xx)	
		sata_apm_pcs_phy_writel(port_index, 0x58010, 0x6050);
#else
		sata_apm_pcs_phy_writel(port_index, 0x58010, 0x6040);
#endif
		/* Rx peak and pole */
		sata_apm_pcs_phy_writel(port_index, 0x5800c, 0x65);		
		/* Rx phase interpolator divider */
		sata_apm_pcs_phy_writel(port_index, 0x5c012, 0x300);			
	}

	/* Delay 7 ms to allow the Serdes to take effect. From scope, it is
	   observed that there is 6ms before the swing kick in. Therefore,
	   we need to delay at less 6 ms. */
	mdelay(7);
	return 0;
}

void do_ahci_post_pm(struct ata_link *link)
{
	struct ata_port *port = link->ap;
	struct ata_host *host = port->host;
	u32 port_index;
	u32 sstatus;

	if (!of_ahci_is_apm_host(host, NULL)) {
		/* Non-APM862xx host controller */
		return;
	}

	port_index = apm_sata_get_port_idx(host, port);
	if (port_index == -1)
		return;
	
	sstatus = sata_apm_port_csr_readl(port_index, PORT_SCR_STAT);
	apm_ahci_prog_serdes(link, port_index, ((sstatus >> 24) >> 4) & 0xf, 
			SATA_SEQ_AFTER_PM);
}

static int apm_pcs_phy_init(void __iomem *pcs_base, int port, int skip_delay)
{
	int retry_max = 3;
	int i;
	int ret = 0;
	u32 phy_val;

	SATADEBUG("APMSATA: PHY init 0x%p", pcs_base);
#if !defined(CONFIG_NOT_COHERENT_CACHE) || defined(CONFIG_APM86xxx_IOCOHERENT)
	/* Enabling SATA read/write cache Coherency for full DDR RAM */
	phy_val = ADDR_HIGH_START_WR(0x00000000);
	apm_pcs_write(pcs_base, SATA_IOCOH_START0_ADDR, phy_val);
	phy_val = ADDR_LOW_START_WR(0x0);
	apm_pcs_write(pcs_base, SATA_IOCOH_START1_ADDR, phy_val);
	phy_val = ADDR_HIGH_END_WR(0x3FFFFFFF);
	apm_pcs_write(pcs_base, SATA_IOCOH_END0_ADDR, phy_val);
	phy_val = ADDR_LOW_END_WR(0xF) |
			READ_COHERENCY_MAPPER_WR(1) |
			READ_REQUESTOR_ID_WR(0) |
			READ_COHERENT_ACCESS_WR(1) |
			WRITE_COHERENCY_MAPPER_WR(1) |
			WRITE_REQUESTOR_ID_WR(0) |
			WRITE_COHERENT_ACCESS_WR(1);
	apm_pcs_write(pcs_base, SATA_IOCOH_END1_ADDR, phy_val);
#endif /* CONFIG_NOT_COHERENT_CACHE */

phy_retry:
	/* Place PHY in reset */
	phy_val = 0x209fb220;
	apm_pcs_write(pcs_base, PCSCSR_CTL0_ADDR, phy_val);
	
	phy_val = 0x00801e33;
	apm_pcs_write(pcs_base, PCSCSR_CTL1_ADDR, phy_val);

	/* Program the pre-emphasis and tx swing before reset PHY */ 
	apm_pcs_write(pcs_base, PCSCSR_CTL2_ADDR, 0x0E000000);

	/* 0x3E9FB022 bit26 rst=0 */
	phy_val = 0x209fb200;
	asm volatile("msync");
	if (!skip_delay)
		udelay(2000);		/* 2ms as required by Serdes spec */
	apm_pcs_write(pcs_base, PCSCSR_CTL0_ADDR, phy_val);
	asm volatile("msync");
	if (!skip_delay)
		udelay(2000);		/* 2ms as required by Serdes spec */
	apm_pcs_write(pcs_base, PCSCSR_CTL0_ADDR, phy_val | RESET0_F2_WR(1));
	asm volatile("msync");
	if (!skip_delay)
		udelay(2000);		/* 2ms as required by Serdes spec */
	apm_pcs_write(pcs_base, PCSCSR_CTL0_ADDR, phy_val);
	asm volatile("msync");
	if (!skip_delay)
		udelay(2000);		/* 2ms as required by Serdes spec */

	ret |= apm_pcs_phy_read(pcs_base, 0x5c000, &phy_val);
	phy_val &= ~0x1F;
	ret |= apm_pcs_phy_write(pcs_base, 0x5c000, phy_val|0x1B);

	ret |= apm_pcs_phy_read(pcs_base, 0x5c004, &phy_val);
	phy_val &= ~0xFF;
	ret |= apm_pcs_phy_write(pcs_base, 0x5c004, phy_val|0x1);

        /* Turn off Dithering */
	apm_pcs_phy_write(pcs_base, 0x5c065, 0x0);
	apm_pcs_phy_read(pcs_base, 0x5c065, &phy_val);
	udelay(10);
	apm_pcs_phy_write(pcs_base, 0x5c005, 0x6);
	apm_pcs_phy_read(pcs_base, 0x5c005, &phy_val);
	udelay(10);
	apm_pcs_phy_write(pcs_base, 0x5c005, 0x4);
	apm_pcs_phy_read(pcs_base, 0x5c005, &phy_val);
	udelay(10);

#if defined(CONFIG_APM862xx)
	/* Force PLL calibration */
	ret |= apm_pcs_phy_write(pcs_base, 0x5c001, 0x9c75);
	apm_pcs_phy_read(pcs_base, 0x5c001, &phy_val);
#endif /* defined(CONFIG_APM862xx) */

	/* Set align */
	ret |= apm_pcs_phy_write(pcs_base, 0xf80e7, SERDES_PCS_SATA_RX_LEN_1_DATA);
	apm_pcs_phy_read(pcs_base, 0xf80e7, &phy_val);
	ret |= apm_pcs_phy_write(pcs_base, 0xf80ea, SERDES_PCS_SATA_CFG_0_DATA);
	apm_pcs_phy_read(pcs_base, 0xf80ea, &phy_val);

#if 0
	phy_addr = MGMT_PCS_MMD_ADDR_WR(1) |
			MGMT_PCS_LANE_SEL_WR(1) |
			MGMT_PCS_REG_ADDR_WR(SERDES_PCS_PRBS_TIMER_0_ADDR);
	ret |= apm_pcs_phy_read(pcs_base, phy_addr, &phy_val);
	ret |= apm_pcs_phy_write(pcs_base, phy_addr, SERDES_PCS_PRBS_TIMER_0_DATA);
	ret |= apm_pcs_phy_read(pcs_base, phy_addr, &phy_val);
#endif

	/* Wait PHY lock */
	i = SATA_PLL_WAIT_TO;
	do {
		phy_val = apm_pcs_read(pcs_base, PCSCSR_STATUS1_ADDR);
		if ((phy_val & SDS_PCS_PLL_LOCK1_MASK) == 0)
			udelay(1000);
	} while ((i--) && ((phy_val & SDS_PCS_PLL_LOCK1_MASK) == 0));
	
	if (!(phy_val & SDS_PCS_PLL_LOCK1_MASK)) {
		if (retry_max-- > 0) { 
			/* Re-try PLL lock 3 times */
			goto phy_retry;
		}
		printk(KERN_ERR "APM SATA failed lock (pcs 0x%p)\n", pcs_base);
		return -1;
	}
	
	apm_ahci_prog_serdes(NULL, port, sata_apm_pdata->serdes_gen[port],
		 		SATA_SEQ_AFTER_PLL_LOCK);
	return 0;
}

int of_ahci_reset_phy(struct ata_host *host)
{
	struct apm_sata_pdata *apm_pdata = host->dev->platform_data;
	struct ahci_host_priv *hpriv = host->private_data;
	void __iomem *csr_mmio;
	void __iomem *diag_base;
	int i;
	int ret;

	if (apm_pdata == NULL) {
		printk(KERN_WARNING "PHY platform data is NULL\n");
		return 0;
	}

	/* Check if host is APM86xxx controller */
	if (!of_ahci_is_apm_host(NULL, apm_pdata))
		return 0; 

	sata_apm_host = host;
	csr_mmio = hpriv->mmio;
	diag_base = apm_pdata->apm_sata_diag;

	/* Set the diag registers here */
	apm_init_diag(diag_base);

	ret = 0;
	for (i = 0; i < apm_pdata->num_pcs; i++) {
		if (!(apm_pdata->phy_active & (1 << i)))
			continue;		
		ret |= apm_pcs_phy_init(apm_pdata->apm_sata_pcs[i], i, 0);
	}

#if defined(CONFIG_APM862xx) || defined(CONFIG_APM867xx)
	/* BISTCR register for port 0 - port 0 is selected by default 
	   Mask out SIG DET error. */
	sata_csr_writel(0x00170000, csr_mmio + 0xa4);
	sata_csr_readl(csr_mmio + 0xa4);
	
	/* BISTCR register for port 1 - Mask out SIG DET error */
	sata_csr_writel(0x100, csr_mmio + 0xF4);	/* Select port 1 */
	sata_csr_readl(csr_mmio + 0xf4);
	sata_csr_writel(0x00170000, csr_mmio + 0xa4);
	sata_csr_readl(csr_mmio + 0xa4);
#endif
	/* PI register */
	sata_csr_writel(0x03000000, csr_mmio + 0x0c);
	sata_csr_readl(csr_mmio + 0x0c);

	return ret;
}

int of_ahci_phy_chk(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct apm_sata_pdata *apm_pdata = NULL;
	u32 *num_pcs_entry;
	u32 *num_pcs_act_entry; 
	u32 num_pcs_act;
	struct clk *clk;
	u32 *val_ptr;			
	int i;
	int ret = 0;
	char devname[8];
	
	num_pcs_entry = (u32 *) of_get_property(pdev->dev.of_node, "phy", NULL);
	if (num_pcs_entry == NULL || *num_pcs_entry == 0) {
		dev_err(dev, "no phy entry in DTS\n");
		return 0;
	}
	
	num_pcs_act_entry = (u32 *) of_get_property(pdev->dev.of_node, "phy-act", NULL);
	if (num_pcs_act_entry != NULL && *num_pcs_act_entry == 0) {
		dev_warn(dev, "no phy active in DTS\n");
		return 0;
	}
	if (num_pcs_act_entry == NULL)
		num_pcs_act = 0xFFFFFFFF;
	else
		num_pcs_act = *num_pcs_act_entry;
		
	val_ptr = (u32 *) of_get_property(pdev->dev.of_node, "defaultspd", NULL);
	if (val_ptr != NULL) {
		apmsatadefaultspd = *val_ptr;		
	}
	SATADEBUG("APMSATA: default PHY speed 0x%08X", apmsatadefaultspd); 
			
	/* On APM863xx variant, only 1 SATA is available. To protect the
	   innocent, we will force to 1 */
	if (is_apm863xx()) {
		if (*num_pcs_entry > 1) {
			*num_pcs_entry = 1;
			dev_warn(dev, "Forcing SATA to single disk\n");
		}
	}
	
	if (num_pcs_entry && *num_pcs_entry) {
		apm_pdata = (struct apm_sata_pdata *)
				kzalloc(sizeof(struct apm_sata_pdata),
					GFP_KERNEL);
		if (!apm_pdata) {
			dev_err(dev, "kmalloc failed for apm_sata_pdata\n");
			ret = -1;
			goto return_sata_init;
		}
		
		apm_pdata->apm_sata_diag = of_iomap(pdev->dev.of_node, 1);
		if (!apm_pdata->apm_sata_diag) {
			dev_err(dev, "of_iomap failed for diag\n");
			ret = -1;
			goto return_sata_init;
		}

		apm_pdata->num_pcs = *num_pcs_entry;
		apm_pdata->phy_active = num_pcs_act;
		SATADEBUG("APMSATA: num phy %d act phy 0x%08X", 
			apm_pdata->num_pcs, apm_pdata->phy_active);

		for (i = 0; i < apm_pdata->num_pcs; i++) {
			if (!(apm_pdata->phy_active & (1 << i)))
				continue;
			apm_pdata->serdes_gen[i] = 0;		
			apm_pdata->apm_sata_pcs[i] = of_iomap(pdev->dev.of_node,
								(i + 2));
			if (!apm_pdata->apm_sata_pcs[i]) {
				dev_err(dev, "of_iomap failed for pcs%d\n", i);
				ret = -1;
				goto return_sata_init;
			}

			snprintf(devname, sizeof(devname), "sata%d", i);
			clk = clk_get(NULL, devname);
			if (IS_ERR(clk)) {
				dev_err(dev, "clk_get failed for sata%d\n", i);
				ret = -1;
				goto return_sata_init;
			}
			clk_enable(clk);
		}

		dev->platform_data = apm_pdata;
		ret = apm_pdata->num_pcs;
		/* save to identify APM86xxx controller */
		sata_apm_pdata = apm_pdata; 
	}
#ifdef CONFIG_SYSFS
	add_sata_sysfs(pdev->dev.driver);
#endif

return_sata_init:
	return ret;
}

void of_ahci_phy_cleanup(struct platform_device *pdev)
{
	int i;
	struct device *dev = &pdev->dev;
	struct apm_sata_pdata *apm_pdata = dev->platform_data;
	char devname[8];
	struct clk *clk = NULL;

	if (apm_pdata == NULL)
		return;
	for (i = 0; i < apm_pdata->num_pcs; i++) {
		if (!(apm_pdata->phy_active & (1 << i)))
			continue;		
		if (apm_pdata->apm_sata_pcs[i]) {
			apm_pcs_phy_enable(apm_pdata->apm_sata_pcs[i], 0);
			snprintf(devname, sizeof(devname), "sata%d", i);

			clk = clk_get(NULL, devname);
			if (!IS_ERR(clk))
				clk_disable(clk);
			
			iounmap((void __iomem *) (apm_pdata->apm_sata_pcs[i]));
		}
	}

	kfree(apm_pdata);
	dev->platform_data = NULL;
}

struct scsi_host_template *of_ahci_get_ahci_sht(void)
{
	//	ahci_sht_apm = ahci_sht;

	if (is_apm862xx() || is_apm863xx()) {
		/* Limit PRD to 4 due to IP errata */
		ahci_sht_apm.sg_tablesize = 4;
	}

	return &ahci_sht_apm;
}

/* This function is ported over from ahci library. */
static unsigned int ahci_dev_classify_apm(struct ata_port *port)
{
	void __iomem *port_mmio = ahci_port_base(port);
	struct ata_taskfile tf;
	u32 tmp;

	tmp = readl(port_mmio + PORT_SIG);
	tf.lbah		= (tmp >> 24)	& 0xff;
	tf.lbam		= (tmp >> 16)	& 0xff;
	tf.lbal		= (tmp >> 8)	& 0xff;
	tf.nsect	= (tmp)		& 0xff;

	return ata_dev_classify(&tf);
}

/* This function is ported over from ahci library. It does the inverse. */
u32 ahci_apm_wait_register(void __iomem *reg, u32 mask, u32 val,
		      unsigned long interval, unsigned long timeout)
{
	unsigned long deadline;
	u32 tmp;

	tmp = ioread32(reg);

	/* Calculate timeout _after_ the first read to make sure
	 * preceding writes reach the controller before starting to
	 * eat away the timeout.
	 */
	deadline = ata_deadline(jiffies, timeout);

	while ((tmp & mask) != val && time_before(jiffies, deadline)) {
		msleep(interval);
		tmp = ioread32(reg);
	}

	return tmp;
}

/* This function is ported over from ahci library. Its intends is to program
   Serdes as soon as possible. */
int apm_sata_link_resume(struct ata_link *link, const unsigned long *params,
		     unsigned long deadline)
{
	struct ata_port *port;	
	struct ata_host *host;
	int port_idx;
	u32 scontrol, serror;
	int rc;

	port = link->ap;
	host = port->host;
	port_idx = apm_sata_get_port_idx(host, port);

	if ((rc = sata_scr_read(link, SCR_CONTROL, &scontrol)))
		return rc;

	scontrol = (scontrol & 0x0f0) | 0x300;

	if ((rc = sata_scr_write(link, SCR_CONTROL, scontrol)))
		return rc;

	apm_ahci_prog_serdes(link, port_idx, 
			sata_apm_pdata->serdes_gen[port_idx], 
			SATA_SEQ_AFTER_LINUX_COMRESET);

	/* Some PHYs react badly if SStatus is pounded immediately
	 * after resuming.  Delay 200ms before debouncing.
	 */
	msleep(200);

	if ((rc = sata_link_debounce(link, params, deadline)))
		return rc;

	/* clear SError, some PHYs require this even for SRST to work */
	if (!(rc = sata_scr_read(link, SCR_ERROR, &serror)))
		rc = sata_scr_write(link, SCR_ERROR, serror);

	return rc != -EINVAL ? rc : 0;
}

/* Function from ahci library */
extern int sata_set_spd_needed(struct ata_link *link);
extern bool ata_phys_link_offline(struct ata_link *link);
extern int ata_wait_ready(struct ata_link *link, unsigned long deadline,
		   int (*check_ready)(struct ata_link *link));

/* This function is ported over from ahci library */
int apm_sata_link_hardreset(struct ata_link *link, const unsigned long *timing,
			unsigned long deadline,
			bool *online, int (*check_ready)(struct ata_link *))
{
	u32 scontrol;
	int rc;

	DPRINTK("ENTER sata_link_hardreset\n");

	if (online)
		*online = false;

	if (sata_set_spd(link)) {
		/* SATA spec says nothing about how to reconfigure
		 * spd.  To be on the safe side, turn off phy during
		 * reconfiguration.  This works for at least ICH7 AHCI
		 * and Sil3124.
		 */	 
		if ((rc = sata_scr_read(link, SCR_CONTROL, &scontrol)))
			goto out;

		scontrol = (scontrol & 0x0f0) | 0x304;

		if ((rc = sata_scr_write(link, SCR_CONTROL, scontrol)))
			goto out;

		sata_set_spd(link);
	}

	/* issue phy wake/reset */
	if ((rc = sata_scr_read(link, SCR_CONTROL, &scontrol)))
		goto out;
			
	scontrol = (scontrol & 0x0f0) | 0x301;

	if ((rc = sata_scr_write_flush(link, SCR_CONTROL, scontrol)))
		goto out;

	/* Couldn't find anything in SATA I/II specs, but AHCI-1.1
	 * 10.4.2 says at least 1 ms.
	 */
	msleep(1);
	
	/* bring link back */
	rc = apm_sata_link_resume(link, timing, deadline);
	if (rc)
		goto out;
	
	/* if link is offline nothing more to do */
	if (ata_phys_link_offline(link))
		goto out;

	/* Link is online.  From this point, -ENODEV too is an error. */
	if (online)
		*online = true;

	if (sata_pmp_supported(link->ap) && ata_is_host_link(link)) {
		/* If PMP is supported, we have to do follow-up SRST.
		 * Some PMPs don't send D2H Reg FIS after hardreset if
		 * the first port is empty.  Wait only for
		 * ATA_TMOUT_PMP_SRST_WAIT.
		 */
		if (check_ready) {
			unsigned long pmp_deadline;

			pmp_deadline = ata_deadline(jiffies,
						    ATA_TMOUT_PMP_SRST_WAIT);
			if (time_after(pmp_deadline, deadline))
				pmp_deadline = deadline;
			ata_wait_ready(link, pmp_deadline, check_ready);
		}
		rc = -EAGAIN;
		goto out;
	}

	rc = 0;
	if (check_ready)
		rc = ata_wait_ready(link, deadline, check_ready);
 out:
	if (rc && rc != -EAGAIN) {
		/* online is set iff link is online && reset succeeded */
		if (online)
			*online = false;
		ata_link_printk(link, KERN_ERR,
				"COMRESET failed (errno=%d)\n", rc);
	}
	DPRINTK("EXIT, rc=%d\n", rc);
	return rc;
}

int apm_ahci_hardreset(struct ata_link *link, unsigned int *class,
		       unsigned long deadline)
{
	struct ahci_port_priv *pp; 
	u8 *d2h_fis; 
	struct ata_taskfile tf;
	struct ata_port *port;	
	struct ata_host *host;
	int port_idx;
	void __iomem *port_mmio;
	unsigned long start_jiffies;
	u32 val;
	int fast_retry;
	int outer_retry;
	u32 intr_mask;

	if (sata_apm_pdata == NULL) {
		*class = ATA_DEV_NONE;
		return 0;
	}
	port = link->ap;
	host = port->host;
	port_idx = apm_sata_get_port_idx(host, port);
	if (!(sata_apm_pdata->phy_active & (1 << port_idx))) {
		*class = ATA_DEV_NONE;
		return 0;
	}
	
	SATADEBUG("APMSATA%d: Issue hardreset", port_idx);	
	port_mmio = ahci_port_base(link->ap);
	pp = port->private_data;
	d2h_fis = pp->rx_fis + RX_FIS_D2H_REG;
		
	start_jiffies = jiffies;
	outer_retry = 3;
 		
__outer_retry:		
	/* Clear PHY ready/connect IRQ */
	intr_mask = sata_apm_port_csr_readl(port_idx, PORT_IRQ_MASK);
	sata_apm_port_csr_writel(port_idx, PORT_IRQ_MASK,
				intr_mask & ~((1 << 25) | (1<<9)));
		
	for (fast_retry = 3; fast_retry > 0; fast_retry--) {
		/* We must stop the engine to issue COMRESET */
		ahci_stop_engine(port);
	
		/* clear D2H reception area to properly wait for D2H FIS */
		ata_tf_init(link->device, &tf);
		tf.command = 0x80;
		ata_tf_to_fis(&tf, 0, 0, d2h_fis);

#if defined(CONFIG_APM862xx)
		/* Handle SATA errata required double COMRESET: 
		   Issue first COMRESET and let sata_link_hardreset to issue 
		   second COMRESET. */
		SATADEBUG("APMSATA%d: Issue%s double COMRESET", port_idx,
				fast_retry == 3 ? "" : " fast");
		val = sata_apm_port_csr_readl(port_idx, PORT_SCR_CTL);
		sata_apm_port_csr_writel(port_idx, PORT_SCR_CTL, 
				(val & 0x0F000000) | 0x01030000);
		sata_apm_port_csr_writel(port_idx, PORT_SCR_CTL, val);
		mdelay(1);
#else
		SATADEBUG("APMSATA%d: Issue%s COMRESET", port_idx,
				fast_retry == 3 ? "" : " fast");
#endif	
		apm_sata_link_hardreset(link, sata_deb_timing_hotplug, 
					deadline, NULL, NULL);
	        
	        /* Start engine for this port */
                ahci_start_engine(port);

		/* Wait 500 ms as COMRESET takes 200ms. Please note that
		   function apm_sata_link_hardreset already waited 200ms. */
		val = ahci_apm_wait_register(port_mmio + PORT_SCR_STAT, 0xF, 
					0x3, 1, 300);
		if ((val & 0xF) == 0x3)
			break;
		SATADEBUG("APMSATA%d: timeout wait for link", port_idx);
	}
		
	/* Clear PHY IRQ if and only if PHY interrupt only */
	val = sata_apm_port_csr_readl(port_idx, PORT_IRQ_STAT);
	if ((val & ((1<<25) | (1<<9))) &&
		!(val & ~((1<<25) | (1<<9)))) {
		sata_apm_port_csr_writel(port_idx, PORT_IRQ_STAT, val);
	}

	/* Restore IRQ */
	sata_apm_port_csr_writel(port_idx, PORT_IRQ_MASK, 
				sata_byte_swap(pp->intr_mask));
		
	if (ata_link_offline(link)) {
		SATADEBUG("APMSATA%d: No link, force softreset", port_idx);
		goto do_followup_srst;
		
	}
	SATADEBUG("APMSATA%d: link up SStatus 0x%08X SControl 0x%08X %dms",
		port_idx, sata_apm_port_csr_readl(port_idx, PORT_SCR_STAT),
		sata_apm_port_csr_readl(port_idx, PORT_SCR_CTL),
		jiffies_to_msecs(jiffies - start_jiffies));
		
	/* Program PHY setting immediately after link up */
	val = sata_apm_port_csr_readl(port_idx, PORT_SCR_STAT);
	if (apm_ahci_prog_serdes(link, port_idx, ((val >> 24) >> 4) & 0xf, 
				SATA_SEQ_AFTER_APM_LINKUP) == 0) {
		/* Serdes got reprogrammed, reset COMRESET. 
		   For APM862xx, we want the correct Serdes programmed 
		   immediately after the COMRESET.
		   For APM867xx, we want the correct Serdes programmed
		   immediately before COMRESET. */
		SATADEBUG("APMSATA%d: Reprogrammed Serdes, restart COMRESET",
			port_idx);		
		goto __outer_retry;			
	}
	
	/*
	 * Wait for the first D2H from device,i.e,signature update notification
	 */ 
	start_jiffies = jiffies;
	val = ata_wait_register(port, port_mmio + PORT_SIG, 0xFF, 0xFF, 100, 500);
	if (val == 0xFFFFFFFF) {
		if (--outer_retry > 0) {
                        ata_port_printk(link->ap, KERN_WARNING,
                                "Restart hardreset (%d)\n", outer_retry);
                	goto __outer_retry;
                } 	
		/* Try a softreset */
		ata_port_printk(link->ap, KERN_WARNING, 
				"No Signature Update\n");
		*class = ATA_DEV_NONE;
		goto do_followup_srst;
	}
	
	*class = ahci_dev_classify_apm(link->ap);
	ata_port_printk(link->ap, KERN_INFO,
		"Signature Update detected @ %d msecs 0x%08X %d\n",
		jiffies_to_msecs(jiffies - start_jiffies), val, 
		*class);
	return 0;

	/* device signature acquisition is unreliable. Force a softreset. */
do_followup_srst:
	return -EAGAIN;
}

static int apm_ahci_softreset(struct ata_link *link, unsigned int *class,
			  unsigned long deadline)
{
	int pmp = sata_srst_pmp(link);
	struct ata_port *port;	
	struct ata_host *host;
	int port_idx;
	int rc;
	u32 val;
	
	port = link->ap;
	host = port->host;
	port_idx = apm_sata_get_port_idx(host, port);

	SATADEBUG("APMSATA%d: Issue softreset", port_idx);

	rc = ahci_do_softreset(link, class, pmp, deadline, ahci_check_ready);
	if (rc == 0) {
		/* Program PHY setting immediately after softreset */
		val = sata_apm_port_csr_readl(port_idx, PORT_SCR_STAT),
		apm_ahci_prog_serdes(link, port_idx, ((val >> 24) >> 4) & 0xf, 
			SATA_SEQ_AFTER_APM_SOFTRESET);	
	}
	return rc;
}

int of_ahci_set_port_ops(struct platform_device *pdev, struct ata_port_info *pip)
{
	struct device *dev = &pdev->dev;

	if (!of_ahci_is_apm_host(NULL, dev->platform_data)) {
		/* Non-APM862xx host controller */
		pip->port_ops = &ahci_ops;
		return 0;
	}
	
	ahci_ops_apm = ahci_ops;

	/* Override soft_reset function */
	ahci_ops_apm.softreset     = apm_ahci_softreset;
/*	ahci_ops_apm.pmp_softreset = apm_ahci_softreset; */
	ahci_ops_apm.hardreset     = apm_ahci_hardreset;
/*	ahci_ops_apm.pmp_hardreset = apm_ahci_pmp_hardreset; */
	
	pip->port_ops = &ahci_ops_apm;
	
	return 0;
}
