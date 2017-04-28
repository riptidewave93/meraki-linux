/*
 * APM86xxx SoC setup code
 *
 * Copyright (c) 2010 Applied Micro Circuits Corporation.
 * Loc Ho <lho@apm.com>.
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
 */
#include <linux/module.h>
#include <linux/stddef.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/of_platform.h>
#include <linux/delay.h>
#include <linux/clk.h>
#include <asm/io.h>
#include <linux/err.h>

#include <asm/prom.h>
#include <asm/dcr.h>
#include <asm/dcr-regs.h>
#include <asm/dcr-native.h>
#include <asm/reg.h>
#include <asm/mpic.h>
#include <asm/apm86xxx_soc.h>
#include <asm/apm86xxx_pm.h>
#ifdef CONFIG_APM86xxx_SHMEM
#include <asm/apm_shm.h>
#endif
#include <asm/apm_ahbc_csr.h>
#include "apm_sdu_csr.h"
#include <asm/ipp.h>

struct l2c {
	int status;
	int size;
	int dcraddr;
	int dcrdata;
	int ecc_enabled;
};

static struct l2c l2c_info[NR_CPUS];
static u8 *sdu_base = NULL;
static u8 *ahbc_base = NULL;

#if !defined(CONFIG_APM867xx)
static u32 ahbc_usb_base[3] = {0x2018, 0x2024, 0x20ac};
static u32 ahbc_usb_hostcfg_base[3] = {0, 0x2064, 0x20b8};
static u32 ahbc_usb_clk_enable[3] = {0, 0, 0};
static u32 ahbc_usb_enable[3] = {0, 0, 0};
#endif

static inline unsigned l2c_get(int reg)
{
	mtdcr(DCRN_L2C_ADDR, reg);
	return mfdcr(DCRN_L2C_DATA);
}

static inline void l2c_set(int reg, unsigned val)
{
	mtdcr(DCRN_L2C_ADDR, reg);
	mtdcr(DCRN_L2C_DATA, val);
}

/*
 * cpu_to_l2cache (taken from ../kernel/smp.c)
 */
static struct device_node *cpu_to_l2cache(int cpu)
{
	struct device_node *np;
	struct device_node *cache;

	if (!cpu_present(cpu))
		return NULL;

	np = of_get_cpu_node(cpu, NULL);
	if (np == NULL)
		return NULL;

	cache = of_find_next_cache_node(np);

	of_node_put(np);

	return cache;
}

static void l2c_get_node(unsigned int cpu, struct l2c *l2c)
{
	struct device_node *np;
	const char *status;
	const unsigned int *dcrreg;
	const unsigned int *psize;
	const unsigned int *ecc_enable;
	int len;

#if defined(CONFIG_SMP)
	np = cpu_to_l2cache(cpu);
#else
	np = cpu_to_l2cache(0);
	cpu = (unsigned int)mfspr(SPRN_PIR);
#endif

	if (!np) {
		printk(KERN_INFO "CPU%d: could not find L2 cache node\n", cpu);
		return;
	}

	if (!of_device_is_compatible(np, "apm,l2-cache")) {
		printk(KERN_INFO "CPU%d: incompatible L2 cache node\n", cpu);
		goto out;
	}

	status = of_get_property(np, "status", &len);
	if (status && strcmp(status,"disabled") == 0) {
		printk(KERN_INFO "CPU%d: L2 cache is disabled\n", cpu);
		goto out;
	}

	psize = of_get_property(np, "cache-size", NULL);
	if (psize == NULL) {
		printk(KERN_INFO "CPU%d: Can't get L2 cache-size!\n", cpu);
		goto out;
	}

	dcrreg = of_get_property(np, "dcr-reg", &len);
	if (!dcrreg || (len != 2*sizeof(unsigned int))) {
		printk(KERN_INFO "CPU%d: L2 cache node has missing or invalid "
			   "dcr-reg property\n", cpu);
		goto out;
	}

	if (*dcrreg != DCRN_L2C_ADDR) {
		/* Do this to use mtdcr instead of mtdcrx because
		 * mtdcrx is not always available in binutils */
		printk(KERN_INFO "CPU%d: unknown L2 cache access for "
			   "dcr-reg property\n", cpu);
		goto out;
	}

	ecc_enable = of_get_property(np, "ecc-enable", &len);
	if (ecc_enable)
		l2c->ecc_enabled = *ecc_enable;
	else
		l2c->ecc_enabled = 1; /* default is enabled */

	l2c->dcraddr = *dcrreg;
	l2c->dcrdata = *dcrreg + 1;
	l2c->size    = *psize >> 10;
	l2c->status  = 1; /* enable */
out:
	if (np)
		of_node_put(np);

	return;
}

void set_plb_lowfreq_mode(int enable)
{
	BUG_ON(!sdu_base);

	if (enable) {
		setbits32((u32 *)(sdu_base + SDU_PBRG_CTRL_ADDR),
				PLB_LT_AXI_MASK);
	} else {
		clrbits32((u32 *)(sdu_base + SDU_PBRG_CTRL_ADDR),
				PLB_LT_AXI_MASK);
	}
}

void cpu_l2c_enable(void)
{
	unsigned long flags;
	unsigned int l2cr0;
#if defined(CONFIG_SMP)
	unsigned int l2cr2;
	unsigned int cpu = smp_processor_id();
	struct l2c *l2c = &l2c_info[cpu];
#else
	unsigned int cpu = (unsigned int) mfspr(SPRN_PIR);
	struct l2c *l2c = &l2c_info[0];
#endif

	if (!l2c->status)
		return; /* L2 cache disabled */

	/*
	 * Coherency Configuration Settings for L2 cache.
	 *
	 * See section 3.7.4 in PPC465-S User's Manual
	 * See section 7.2 and 8.8.8 in Streak Software User's Guide
	 *
	 * TLB Attributes for Coherency Operation
	 *   WL1 = 1 -- Write-through in L1 DCache
	 *   W   = 0 -- Copy-Back, not Write-Through, in L2
	 *   I   = 0 -- Not cache inhibited
	 *   M   = 0 -- Coherent
	 *   G   = 0 -- Not Guarded
	 *
	 *   Configured in the following files
	 *     arch/powerpc/include/asm/mmu-44x.h
	 *     arch/powerpc/kernel/head_44x.S
	 */

	printk(KERN_DEBUG "CPU%d enabling L2 cache\n", cpu);

	local_irq_save(flags);
	asm volatile ("sync" ::: "memory");

	flush_dcache();

	/* Clear L2FER0 and L2FER1 to prevent errors from being forced */
	l2c_set(DCRN_L2FER0, 0);
	l2c_set(DCRN_L2FER1, 0);

	/* Set Extended Real Address Prefix */
	l2c_set(DCRN_L2ERAPR, 0);
	l2c_set(DCRN_L2SLVERAPR, 0);

	/* Set PLB settings for L2, use defaults for now */
	l2c_set(DCRN_L2CR3, 0);

	/* For L2 cache, use L2CR1 defaults */
	l2c_set(DCRN_L2CR1, 0);

	/* Set TAA and DAA to appropriate values.
	 * This values will be specified in a table
	 * based on voltage and frequency. */
	l2cr0  = l2c_get(DCRN_L2CR0);
	l2cr0 &= ~(L2CR0_TAA_MASK | L2CR0_DAA_MASK);
	l2cr0 |= (L2CR0_TAA_1 | L2CR0_DAA_1);
	l2c_set(DCRN_L2CR0, l2cr0);

	/* check if ECC should be enabled or disabled */
	if (l2c->ecc_enabled) {
		unsigned int l2mcrer;

		l2cr0 = l2c_get(DCRN_L2CR0);
		l2cr0 &= ~(L2CR0_DECC | L2CR0_DECA); 
		l2c_set(DCRN_L2CR0, l2cr0);

		l2mcrer = L2MCSR_ITSBE | L2MCSR_ITDBE |
			  L2MCSR_IDSBE | L2MCSR_IDDBE |
			  L2MCSR_DTSBE | L2MCSR_DTDBE |
			  L2MCSR_DDSBE | L2MCSR_DDDBE;
		l2c_set(DCRN_L2MCRER, l2mcrer);
	} else {
		l2cr0 = l2c_get(DCRN_L2CR0);
		l2cr0 |= (L2CR0_DECC | L2CR0_DECA); 
		l2c_set(DCRN_L2CR0, l2cr0);
	}
	
	/* enable L2 cache */
	l2cr0  = l2c_get(DCRN_L2CR0);
	l2cr0 &= ~L2CR0_AS_MASK;
	l2cr0 |= (L2CR0_AS_256K | L2CR0_TAI);
	l2c_set(DCRN_L2CR0, l2cr0);

	do {
		l2cr0 = l2c_get(DCRN_L2CR0);
	} while ( (l2cr0 & L2CR0_TAI) != 0);

	/* L2 cache Op Broadcast Enable */
	mtspr(SPRN_CCR1, (mfspr(SPRN_CCR1) | CCR1_L2COBE));

#if 0
	/* Data Intervention Enable */
	PLB_PCICR[DIE] = 1; 
#endif

#if defined(CONFIG_SMP)
	/* Disable Snoop Inihibit */
	mtdcr(DCRN_STRK_CR4, mfdcr(DCRN_STRK_CR4) & ~STRK_CR4_SNPI);
	/* Join Coherency domain */
	l2cr2 = L2CR2_SNPME | L2CR2_L1CE | L2CR2_SNPRQPD3;
	l2c_set(DCRN_L2CR2, l2cr2);
#endif

	asm volatile ("sync; isync" ::: "memory");
	local_irq_restore(flags);

	printk(KERN_DEBUG "CPU%d: L2CR0 = 0x%08x\n",
		cpu, l2c_get(DCRN_L2CR0));
	printk(KERN_DEBUG "CPU%d: L2CR1 = 0x%08x\n",
		cpu, l2c_get(DCRN_L2CR1));
	printk(KERN_DEBUG "CPU%d: L2CR2 = 0x%08x\n",
		cpu, l2c_get(DCRN_L2CR2));
	printk(KERN_DEBUG "CPU%d: L2CR3 = 0x%08x\n",
		cpu, l2c_get(DCRN_L2CR3));

	printk(KERN_INFO "CPU%d %dk L2 cache enabled\n", cpu, l2c->size);

	return;
}

void cpu_l2c_disable(void)
{
	unsigned int l2cr0;

	flush_dcache();

	l2cr0 = l2c_get(DCRN_L2CR0);
	if ((l2cr0 & L2CR0_AS_256K) == L2CR0_AS_256K) {
		l2c_disable();
	}
}

void cpu_l2c_init(void)
{
	unsigned int cpu;
	struct l2c *l2c;

	for (cpu = 0; cpu < NR_CPUS; cpu++) {
		l2c = &l2c_info[cpu];
		l2c->status = 0; /* disabled by default */
		l2c_get_node(cpu, l2c);
	}

	cpu_l2c_enable();
}

/*
 * APM86xxx machine_check_4xx_custom routine
 *     This overrides the weak default custom_machine_check in kernel/trap.c
 *     to check for L2 and bridges errors.
 */
void machine_check_4xx_custom(void)
{
	u32 pesr; 
	u32 l2cr0;
	u32 l2mcsr;

	/* Check L2 for error */
	l2cr0 = l2c_get(DCRN_L2CR0);
	if ((l2cr0 & L2CR0_AS_MASK) == 0)
		goto l2_done; /* L2C disabled */

	if ((l2cr0 & L2CR0_DECC) != 0 )
		goto l2_done; /* L2C ECC disabled */

	l2mcsr = l2c_get(DCRN_L2MCSR);

	if (l2mcsr == 0) 
		goto l2_done; /* no L2C machine check detected */

	if (l2mcsr & L2MCSR_ITSBE)
		printk("Instruction Side Tag Array Single Bit Error\n");
	if (l2mcsr & L2MCSR_ITDBE)
		printk("Instruction Side Tag Array Double Bit Error\n");
	if (l2mcsr & L2MCSR_IDSBE)
		printk("Instruction Side Data Array Single Bit Error\n");
	if (l2mcsr & L2MCSR_IDDBE)
		printk("Instruction Side Data Array Double Bit Error\n");
	if (l2mcsr & L2MCSR_DTSBE)
		printk("Data Side Tag Array Single Bit Error\n");
	if (l2mcsr & L2MCSR_DTDBE)
		printk("Data Side Tag Array Double Bit Error\n");
	if (l2mcsr & L2MCSR_DDSBE)
		printk("Data Side Data Array Single Bit Error\n");
	if (l2mcsr & L2MCSR_DDDBE)
		printk("Data Side Data Array Double Bit Error\n");
	if (l2mcsr & L2MCSR_SNPTSBE)
		printk("Snoop Side Tag Array Single Bit Error\n");
	if (l2mcsr & L2MCSR_SNPTDBE)
		printk("Snoop Side Tag Array Double Bit Error\n");
	if (l2mcsr & L2MCSR_SNPDSBE)
		printk("Snoop Side Data Array Single Bit Error\n");
	if (l2mcsr & L2MCSR_SNPDDBE)
		printk("Snoop Side Data Array Double Bit Error\n");
	if (l2mcsr & L2MCSR_FARM)
		printk("Fixed Address Region Mismatch\n");
	if (l2mcsr & L2MCSR_MSTRQE)
		printk("PLB Master Port Request Error\n");
	if (l2mcsr & L2MCSR_MSTRDE)
		printk("PLB Master Port Read Data Error\n");
	if (l2mcsr & L2MCSR_MSTRDPE)
		printk("PLB Master Port Read Data Parity Error\n");
	if (l2mcsr & L2MCSR_SNPAPE)
		printk("PLB Snoop Port Address Parity Error\n");
	if (l2mcsr & L2MCSR_SLVWDE)
		printk("PLB Slave Port Write Data Error\n");
	if (l2mcsr & L2MCSR_SLVWDPE)
		printk("PLB Slave Port Write Data Parity Error\n");
	if (l2mcsr & L2MCSR_SLVAPE)
		printk("PLB Slave Port Address Parity Error\n");
	if (l2mcsr & L2MCSR_SLVBEPE)
		printk("PLB Slave Port Byte Enable Parity Error\n");
	if (l2mcsr & L2MCSR_SLVDSBE)
		printk("PLB Slave Port Data Array Single Bit Error\n");
	if (l2mcsr & L2MCSR_SLVDDBE)
		printk("PLB Slave Port Data Array Double Bit Error\n");
	if (l2mcsr & L2MCSR_SLVIAE)
		printk("PLB Slave Port Invalid Address Error\n");
	if (l2mcsr & L2MCSR_SLVICE)
		printk("PLB Slave Port Invalid Command Error\n");
	if (l2mcsr & L2MCSR_DCRTO)
		printk("DCR Timeout\n");

	l2c_set(DCRN_L2MCSR, l2mcsr);
l2_done:
	#define DCRIPR_PESR		0x24000000
	#define DCRN_PESR		0x00000008
	#define DCRN_PEARL		0x00000009
	#define DCRN_PEARH		0x0000000A	

	/* Check for bridge errors */
	mtspr(SPRN_DCRIPR, DCRIPR_PESR); /* set upper 22 bits of DCR address */
	isync();
	pesr = mfdcr(DCRN_PESR);
	if (pesr) {
		u32 addrlo;
		u32 addrhi;
		addrlo = mfdcr(DCRN_PEARL);
		addrhi = mfdcr(DCRN_PEARH);
		printk("PLB %s error 0x%08X at 0x%08X_%08X\n",
			 pesr & 0x10000000 ? "read" : "write", 
			 pesr, addrhi, addrlo);	
		/* Clear them */	
		mtdcr(DCRN_PESR, 0xFFFFFFFF);
	}	 
	mtspr(SPRN_DCRIPR, 0); /* clear upper 22 bits of DCR address */
	isync();
}

void apm86xxx_ahbc_usb_enable(int port)
{
	u32 reg_val;
	ahbc_usb_reg_t *usb_reg = NULL;
	static int once_done=0;

	if (!ahbc_base)
		return;

	/* Once AHBC USB[port] is reset & enabled, do not reset & re-enabled */
	if (ahbc_usb_enable[port])
		return;

#ifdef CONFIG_APM86xxx_SHMEM
	/* IF AHBC USB OTG is out of reset, mark its flag */
	usb_reg = (ahbc_usb_reg_t *)(ahbc_base + ahbc_usb_base[0]);
	reg_val = in_be32((volatile unsigned *)&usb_reg->rstctl);
	if ((reg_val & (USB0_OTGDISABLE_WR(1) | USB0_PORTRESET_WR(1) |
			USB0_POR_WR(1))) == 0)
		ahbc_usb_enable[0] = 1;
#endif

	/* If AHBC USB_OTG is not reset & enabled, first reset & enable it */
	if (port && ahbc_usb_enable[0] == 0) {
		ahbc_usb_enable[0] = port;
		port = 0;
	}

_ahbc_usb_enable_start:

	usb_reg = (ahbc_usb_reg_t *)(ahbc_base + ahbc_usb_base[port]);
	reg_val = USB0_PORTRESET_WR(1) |
			USB0_POR_WR(1);
	/* Put USB Port in reset */
#ifdef CONFIG_APM86xxx_SHMEM
	if (port == 0) {
		atomic_csr_clrsetbits(~reg_val, reg_val, (void *)&usb_reg->rstctl);
	} else {
		out_be32((volatile unsigned *)&usb_reg->rstctl, reg_val);
	}
#else
	out_be32((volatile unsigned *)&usb_reg->rstctl, reg_val);
#endif

	reg_val = USB0_REFCLKDIV_WR(2);
	if (port)
		reg_val |= USB0_REFCLKSEL_WR(2);

	/* Set clock */
	/* Bring USB Port out of reset */
#ifdef CONFIG_APM86xxx_SHMEM
	if (port == 0) {
		atomic_csr_clrsetbits(~reg_val, reg_val, (void *)&usb_reg->clkctl);
		atomic_csr_clrbits(0xffffffff, (void *)&usb_reg->rstctl);
	} else {
		out_be32((volatile unsigned *)&usb_reg->clkctl, reg_val);
		out_be32((volatile unsigned *)&usb_reg->rstctl, 0);
	}
#else
	out_be32((volatile unsigned *)&usb_reg->clkctl, reg_val);
	out_be32((volatile unsigned *)&usb_reg->rstctl, 0);
#endif

	if (port) {
		reg_val = USBH_WORD_IF_WR(1);
		/* configure USBHOST_CFG register */
		out_be32((volatile unsigned *)
			(ahbc_base + ahbc_usb_hostcfg_base[port]), reg_val);
	}

	/* usb_tune 0x01b9c262 */
	reg_val = USB0_TXVREFTUNE_WR(0xD) |
			USB0_TXFSLSTUNE_WR(0xC) |
			USB0_TXPREEMPHASISTUNE_WR(3) |
			USB0_TXRISETUNE_WR(1) |
			USB0_TXHSXVTUNE_WR(0) |
			USB0_COMPDISTUNE_WR(2) |
			USB0_SQRXTUNE_WR(6) |
			USB0_OTGTUNE_WR(2);
	/* configure USB_TUNE register */
#ifdef CONFIG_APM86xxx_SHMEM
	if (port == 0) {
		atomic_csr_clrsetbits(~reg_val, reg_val, (void *)&usb_reg->tune);
	} else {
		out_be32((volatile unsigned *)&usb_reg->tune, reg_val);
	}
#else
	out_be32((volatile unsigned *)&usb_reg->tune, reg_val);
#endif

	if (port == 0 && !once_done) {
		apm86xxx_read_mpa_reg(MPA_DIAG_ADDR, &reg_val);
		reg_val |= MPA_USB0_CTL_SEL_MASK;
		apm86xxx_write_mpa_reg(MPA_DIAG_ADDR, reg_val);

		/* Put USB Port in reset */
		reg_val = USB0_PORTRESET_WR(1) |
			USB0_POR_WR(1);
		apm86xxx_write_mpa_reg(MPA_USB0_RSTCTL_ADDR, reg_val);

		/* Set clock */
		/* Bring USB Port out of reset */
		reg_val = USB0_REFCLKDIV_WR(2);
		apm86xxx_write_mpa_reg(MPA_USB0_CLKCTL_ADDR, reg_val);
		udelay(100);
		apm86xxx_write_mpa_reg(MPA_USB0_RSTCTL_ADDR, 0);
		
		/* usb_tune 0x01b9c262 */
		reg_val = USB0_TXVREFTUNE_WR(0xD) |
			USB0_TXFSLSTUNE_WR(0xC) |
			USB0_TXPREEMPHASISTUNE_WR(3) |
			USB0_TXRISETUNE_WR(1) |
			USB0_TXHSXVTUNE_WR(0) |
			USB0_COMPDISTUNE_WR(2) |
			USB0_SQRXTUNE_WR(6) |
			USB0_OTGTUNE_WR(2);
		apm86xxx_write_mpa_reg(MPA_USB0_TUNE_ADDR, reg_val);
		once_done = 1;	
	}

	if (port == 0) {
		/* AHBC USB_OTG is reset & enabled,
		 * so now reset & enable actual port */
		if (ahbc_usb_enable[0]) {
			port = ahbc_usb_enable[0];
			goto _ahbc_usb_enable_start;
		}
	}

	/* Tune the SoF timing to match 125us for USB Host1 and host 2 */
	if (port == 1 || port == 2) {
		if (port == 1)
			reg_val = apm86xxx_ahbc_get_reg(USBHOST_CFG_ADDR);
		else		
			reg_val = apm86xxx_ahbc_get_reg(USBHOST1_CFG_ADDR);
		reg_val &= ~0x1F8;
		reg_val |= 37 << 3;
		if (port == 1)
			apm86xxx_ahbc_set_reg(USBHOST_CFG_ADDR, reg_val);
		else		
			apm86xxx_ahbc_set_reg(USBHOST1_CFG_ADDR, reg_val);
	}

	/* Mark AHBC USB[port] flag to avoid it from reset & re-enable */
	ahbc_usb_enable[port] = 1;
}

void apm86xxx_ahbc_usb_disable(int port)
{
	u32 reg_val;
	ahbc_usb_reg_t *usb_reg = NULL;

	if (!ahbc_base)
		return;

	usb_reg = (ahbc_usb_reg_t *)(ahbc_base + ahbc_usb_base[port]);

	reg_val = USB0_PORTRESET_WR(1) |
			USB0_POR_WR(1);
	/* Put USB Port in reset */
	out_be32((volatile unsigned *)&usb_reg->rstctl, reg_val);
}

int apm86xxx_usb_clk_enable(int port)
{
	struct clk *clk;
	char devname[8];

	if (ahbc_usb_clk_enable[port])
		return 0;

	snprintf(devname, sizeof(devname), "usb%d", port);
	clk = clk_get(NULL, devname);

	if (IS_ERR(clk)) {
		printk("Error in enabling USB %d clock\n", port);
		return -1;
	}

	clk_enable(clk);
	apm86xxx_ahbc_usb_enable(port);
	udelay(1000);
	ahbc_usb_clk_enable[port] = 1;
	
	return 0;
}
EXPORT_SYMBOL(apm86xxx_usb_clk_enable);

u32 apm86xxx_ahbc_get_reg(u32 offset)
{
	if (ahbc_base) {
		u32 __iomem *ahb_reg = (u32 __iomem *)(ahbc_base + 0x2000 + offset);
		return in_be32(ahb_reg);
	}

	return 0;
}
EXPORT_SYMBOL(apm86xxx_ahbc_get_reg);

void apm86xxx_ahbc_set_reg(u32 offset, u32 val)
{
	if (ahbc_base) {
		u32 __iomem *ahb_reg = (u32 __iomem *)(ahbc_base + 0x2000 + offset);
		out_be32(ahb_reg, val);
	}
}
EXPORT_SYMBOL(apm86xxx_ahbc_set_reg);

static void __iomem *apm86xxx_ahbc_of_iomap(struct device_node *from)
{
	const u32 *node_val;
	u32 __iomem *ahbc_reg = NULL;
	u64 csr_base;
	u32 csr_size;
	int len;
	struct device_node *np_ahbc = NULL;

	/* Get AHBC address */
	np_ahbc = of_find_compatible_node(from, NULL, "apm,apm86xxx-ahbc");
	if (!np_ahbc) {
		printk(KERN_ERR "no AHBC bridge entry in DTS\n");
		goto _return;
	}

	node_val = of_get_property(np_ahbc, "reg", &len);
	csr_base = ((u64) node_val[0]) << 32 | node_val[1];
	csr_size = node_val[2];
	ahbc_reg = ioremap(csr_base, csr_size);

	if (ahbc_reg == NULL)
		printk(KERN_ERR "unable to map AHBC register\n");

	if (np_ahbc)
		of_node_put(np_ahbc);

_return:
	return ahbc_reg;
}

static int __init apm86xxx_ahb_probe(void)
{
	struct device_node *ahb_node;
	u32 __iomem *ahb_reg = NULL;
	int rc = -ENODEV;

	ahb_node = of_find_node_by_type(NULL, "ahb");
	if (ahb_node) {
		ahbc_base = (u8 *)apm86xxx_ahbc_of_iomap(ahb_node);
		if (ahbc_base) {
			rc = 0;
			ahb_reg = (u32 __iomem *)(ahbc_base + 0x2000);
#if !defined(CONFIG_NOT_COHERENT_CACHE) || defined(CONFIG_APM86xxx_IOCOHERENT)
			/* 
			 * Set AIM windows for 4GB regardless of DDR size
			 * Need 1 windows for this operation
			 */
			printk("Setup coherent AIM windows.\n");
			setbits32(&ahb_reg[89], 0);
			setbits32(&ahb_reg[90], 0x00900000);
			setbits32(&ahb_reg[91], 0);
#endif
		} else {
			printk(KERN_ERR "Cannot map AHB registers\n");
		}
	} else {
		printk("Unable to located the AHB node\n");
	}

        return rc;
}
arch_initcall(apm86xxx_ahb_probe);

static irqreturn_t apm86xxx_sdu_error_hdlr(int irq, void *dev)
{
	int i;	
	u32 sdu_sts;
	u32 val;
	
	static char *sdu_err_msg[] = {
		"Memory queue parity error",
		"SATA port 0/1 partiy error",
		"PCIe port 0 parity error",
		"PCIe port 1 parity error",
		"PCIe port 2 parity error",
		"iPP parity error",
		"Ethernet parity error",
		"Classifier parity error",
		"OCM parity error",
		"Security parity error",
		"LCD controller parity error",
		"AHBC parity error",
		"QM ECC error",
		"USB DFS error",
		"USB EBM error",
		"Ethernet PCF error",
		NULL
	};
		
	static char *ahbc_err_msg[] = {
		"USB0 read slave error",
		"USB0 read decoder error",
		"USB0 write slave error",
		"USB0 write decoder error",
		"USB1 read slave error",
		"USB1 read decoder error",
		"USB1 write slave error",
		"USB1 write decoder error",
		"SDHC read slave error",
		"SDHC read decoder error",
		"SDHC write slave error",
		"SDHC write decoder error",
		"USB Host0 read slave error",
		"USB Host0 read decoder error",
		"USB Host0 write slave error",
		"USB Host0 write decoder error",
		"TDM read slave error",
		"TDM read decoder error",
		"TDM write slave error",
		"TDM write decoder error",
		"USB Host1 read slave error",
		"USB Host1 read decoder error",
		"USB Host1 write slave error",
		"USB Host1 write decoder error",
		"USB Host2 read slave error",
		"USB Host2 read decoder error",
		"USB Host2 write slave error",
		"USB Host2 write decoder error",
		NULL
	};
		
	/* Check for error */
	sdu_sts = in_be32((u32 *) (sdu_base + SDU_ERR_STS_ADDR));
	if (sdu_sts & ECC_ERR_MASK) {
		/* ECC error */				
		printk(KERN_ERR "ECC error (0x%08X)\n", sdu_sts); 
		/* Check for module with ECC/parity error */
		val = in_be32((u32 *) (sdu_base + SDU_PERR_ADDR));
		for (i = 0; sdu_err_msg[i] != NULL; i++)
			if (val & (1 << i))
				panic("%s PERR 0x%08X\n", sdu_err_msg[i], val);	
	}
	if (sdu_sts & AXI_DERR_MASK) {
		/* HBF decoder error */		
		printk(KERN_ERR "HBF decoder error (0x%08X)\n", sdu_sts);
		/* Check for decoder error */
		for (i = 0; i < 3; i++) {
			val = in_be32((u32 *) (sdu_base + 
					SDU_SLV0_STS_ADDR + i * 8));
			if (val & WRDECERR_MASK) {
				u32 err_addr;
				err_addr  = in_be32((u32 *) (sdu_base + 
						SDU_SLV0_ADDR_STS_ADDR + 
						i * 8 + 4));
				/* Decoder error for write */
				printk(KERN_ERR 
					"HBF (AXI) slave %d decoder write "
					"error (0x%08X) at "
					"0x{%08X, b0000|b00000}\n", 
					i, val, err_addr);
			}
		}	
	}
	if (sdu_sts & BRG_ERR_MASK) {
		/* HBF decoder error */		
		printk(KERN_ERR "Bridges error (0x%08X)\n", sdu_sts);
		/* Check for bridges error */
		val = in_be32((u32 *) (sdu_base + SDU_BRG_ERR_ADDR));
		if (val & AHBBRGERR_MASK) {
			/* AHBC error */
			val = in_be32((u32 *) (ahbc_base + 
						AHBC_INTERRUPT_ADDR));			
			for (i = 0; ahbc_err_msg[i] != NULL; i++)
				if (val & (1 << i))
					printk(KERN_ERR "%s (0x%08X)\n", 
						ahbc_err_msg[i], val);	
		}
		if (val & PLBBRGERR_MASK) {
			/* An unsupported request is sent from PLB to HBF 
			   (AXI) bridge */
			printk(KERN_ERR 
				"PLB to HBF (AXI) bridge error (0x%08X)\n",
				val);							
		}
		if (val & WRERRACK_MASK) {
			/* An unsupported request is sent from HBF to PLB
			   bridge */
			printk(KERN_ERR 
				"HBF (AXI) to PLB brdige error (0x%08X)\n", 
				val);
		}
	}
	if (sdu_sts & PLB5_WERR_MASK) {
		/* PLB5 write ECC error */		 
		u32 ecc_addr0;
		u32 ecc_addr1;
		ecc_addr0 = in_be32((u32 *) (sdu_base + SDU_PLB_WERRL_ADDR));
		ecc_addr1 = in_be32((u32 *) (sdu_base + SDU_PLB_WERRH_ADDR));
		/* PLB to AXI ECC error */
		printk(KERN_ERR "PLB5 to HBF (AXI) ECC write error CPU%d " 
			"len %d at 0x%02X_%08X (0x%08X)\n", 
			CPUID_RD(ecc_addr1), WR_LEN_RD(ecc_addr1),
			ADDR_HIGH_RD(ecc_addr1), ecc_addr0, sdu_sts);
	}
	
        return IRQ_HANDLED;
}

static int __init apm86xxx_sdu_probe(void)
{
        struct device_node *np_sdu  = NULL;
        const u32 *node_val;
        int irq = NO_IRQ;
        u64 csr_base;
	u32 csr_size;
        int len;

	sdu_base = NULL;
	
        np_sdu = of_find_compatible_node(NULL, NULL, "apm,apm86xxx-sdu");
        if (!np_sdu) {
                printk(KERN_ERR "no apm-sdu entry in DTS\n");                	
                goto err;
        }

        /* Map DCRs */
        node_val = of_get_property(np_sdu, "reg", &len);
        csr_base = ((u64) node_val[0]) << 32 | node_val[1];
        csr_size = node_val[2];
        sdu_base = ioremap(csr_base, csr_size);
        if (sdu_base == NULL) {
                printk(KERN_ERR "unable to map SDU register\n");
                goto err;
        }
        
        /* Get and map irq number from device tree */
        irq = irq_of_parse_and_map(np_sdu, 0);
        if (irq == NO_IRQ) {
                printk(KERN_ERR "no IRQ entry for apm-sdu\n");
                goto err;
        }

        /* Install error handler */
        if (request_irq(irq, apm86xxx_sdu_error_hdlr, IRQF_DISABLED, "sdu", 
        		0) < 0) {
                printk(KERN_ERR "Cannot install SDU error handler\n");
                goto err;
        }
        
        /* Get AHBC address */
        if (!ahbc_base && !(ahbc_base = (u8 *)apm86xxx_ahbc_of_iomap(NULL))) {
                printk(KERN_ERR "unable to map AHBC register\n");
                goto err;
        }              
        /* Enable all AHBC error interrupt */
        out_be32((u32 *) (ahbc_base + AHBC_INTERRUPTMASK_ADDR), 0x00000000);
        
	of_node_put(np_sdu);	
	return 0;

err:
	if (irq != NO_IRQ)
		free_irq(irq, 0);
	if (sdu_base) {
		iounmap(sdu_base);
		sdu_base = NULL;	
	}
	if (np_sdu)
		of_node_put(np_sdu);	
        return -ENODEV;        
}
arch_initcall(apm86xxx_sdu_probe);

/*
 * At present, this routine just applies a system reset.
 */

#if defined(CONFIG_SMP)
extern void mpic_assertcore(unsigned int corenr);
#endif

static inline const char *reset_type_name(u32 reset_type)
{
	switch(reset_type) {
	case DBCR0_RST_CORE:   return "Core Reset";
	case DBCR0_RST_CHIP:   return "Chip Reset";
	case DBCR0_RST_SYSTEM: return "System Reset";
	default:
		BUG();
	}
}

static inline void reset_type_cmd_verify(char * cmd, u32 * reset_type)
{
	if (!cmd)
		return;
	else if (strcmp(cmd,"core") == 0)
		*reset_type = DBCR0_RST_CORE;
	else if (strcmp(cmd,"chip") == 0)
		*reset_type = DBCR0_RST_CHIP;
	else if (strcmp(cmd,"system") == 0)
		*reset_type = DBCR0_RST_SYSTEM;
}

void apm86xxx_reset_system(char *cmd)
{
	u32 reset_type = DBCR0_RST_SYSTEM;

#ifdef CONFIG_SMP
	if (cpu_enabled(1))
		reset_type = DBCR0_RST_CORE;
#endif
	/* Before reboot, tear down MPIC to disable all IRQ and EOI them */
	mpic_teardown_this_cpu(0);

	/* check if a different reset was requested */
	reset_type_cmd_verify(cmd, &reset_type);

	/* Let's reboot */
	printk(KERN_EMERG "%s\n", reset_type_name(reset_type));
	while (1) {
		mtspr(SPRN_DBCR0, mfspr(SPRN_DBCR0) | reset_type);
	}
}

#ifdef CONFIG_SMP
/* assume all CPU are enabled by default */
static unsigned int cpu_enabled_mask = 0xFF;

unsigned int cpu_enabled(unsigned int cpu)
{
	unsigned int cpu_mask = 1 << cpu;

	return (cpu_enabled_mask & cpu_mask);
}
EXPORT_SYMBOL(cpu_enabled);

static int __init cpu_late_config(void)
{
	unsigned int cpu;
	unsigned int cpu_mask;
	struct device_node *np;
	const u32 *reg;
	const char *status;
	int len;

	for_each_node_by_type(np, "cpu") {
		reg = of_get_property(np, "reg", NULL);
		if (reg == NULL)
			continue;

		cpu = *reg;

		status = of_get_property(np, "status", &len);
		if (status && strncmp(status,"disable",7) == 0) {
			cpu_mask = 1 << cpu;
			cpu_enabled_mask &= ~cpu_mask;
		}
	}

	return 0;
}

arch_initcall(cpu_late_config);

#endif
