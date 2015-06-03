/*
 * arch/powerpc/sysdev/ppc4xx_cpm.c
 *
 * PowerPC 4xx Clock and Power Management
 *
 * (C) Copyright 2009, Applied Micro Circuits Corporation
 * Victor Gallardo (vgallardo@amcc.com)
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/proc_fs.h>
#include <linux/dma-mapping.h>
#include <asm/uaccess.h>
#include <asm/prom.h>
#include <asm/io.h>
#include <asm/dcr.h>
#include <asm/dcr-regs.h>
#include <asm/mmio-regs.h>
#include <asm/ppc4xx_cpm.h>
#include <asm/ppc4xx_ocm.h>

#define CPM_PM_MEM_DATA_SIZE	(256*sizeof(unsigned  long))
#define CPM_PM_MEM_ALIGN	16

#define CPM_ER	0x0
#define CPM_FR	0x1
#define CPM_SR	0x2

#define CPM_PCIE_SLEEP	0x00040000
#define CPM_SATA0_SLEEP	0x00001000

extern void cpm_suspend_mem(int pm_mode, unsigned long *data, void *resume);
extern const long cpm_suspend_mem_size;

extern void cpm_resume_mem(void);
extern const long cpm_resume_mem_size;

struct cpm_pm_mem {
	/* functions in OCM */
	void (*suspend)(int pm_mode, unsigned long *data, void *resume);
	void (*resume )(void);
	/* data OCM */
	unsigned long	*data;
};

struct cpm_pm_iic {
	phys_addr_t	phys;
	size_t		size;
};

struct cpm_pm_emac {
	phys_addr_t phys;
	size_t		size;
};

struct cpm {
	unsigned int index;
	unsigned int dcrbase;
	unsigned int ready;
	unsigned int save_er;
	unsigned int save_fr;
	unsigned int pm_cpu;
	unsigned int pm_off;
	unsigned int pm_doze;
	unsigned int pm_nap;
	unsigned int pm_deepsleep;
	struct cpm_pm_iic pm_iic;
	struct cpm_pm_emac pm_emac;
};

extern int	pcie_used;

//static struct proc_dir_entry * cpm_proc_entry;

static struct cpm *cpm_nodes = NULL;
static int cpm_count = 0;

static struct cpm_pm_mem cpm_pm_mem;

static u32 dcrbase_l2c = 0;

const char *cpm_mode_name(int mode)
{
        switch (mode) {
        case CPM_PM_DOZE:
                return "doze";
        case CPM_PM_NAP:
                return "nap";
        case CPM_PM_DEEPSLEEP:
                return "deepsleep";
        default:
                BUG();
        }
}

static struct cpm * get_cpm_node(unsigned int index)
{
        BUG_ON(index >= cpm_count);

	return &cpm_nodes[index];
}

static int cpm_enable(unsigned int index, unsigned int val)
{
	struct cpm *cpm = get_cpm_node(index);

	mtdcr(cpm->dcrbase + CPM_ER,
	      mfdcr(cpm->dcrbase + CPM_ER) | val);

	return 0;
}

static int cpm_force_enable(unsigned int index, unsigned int val)
{
	struct cpm *cpm = get_cpm_node(index);

	mtdcr(cpm->dcrbase + CPM_FR,
	      mfdcr(cpm->dcrbase + CPM_FR) | val);

	while ((mfdcr(cpm->dcrbase + CPM_SR) & val) != val);

	return 0;
}

static int cpm_save(unsigned int index)
{
	struct cpm *cpm = get_cpm_node(index);

	cpm->save_er = mfdcr(cpm->dcrbase + CPM_ER);
	cpm->save_fr = mfdcr(cpm->dcrbase + CPM_FR);

	return 0;
}

static int cpm_restore(unsigned int index)
{
	struct cpm *cpm = get_cpm_node(index);

	mtdcr(cpm->dcrbase + CPM_ER, cpm->save_er);
	mtdcr(cpm->dcrbase + CPM_FR, cpm->save_fr);

	while ((mfdcr(cpm->dcrbase + CPM_SR) & cpm->save_fr) != cpm->save_fr);

	return 0;
}

static int cpm_pm_enable(int pm_mode)
{
	unsigned int i;
	unsigned int pm_val;

	for (i=0; i < cpm_count; i++) {
		struct cpm *cpm = get_cpm_node(i);

		switch(pm_mode) {
		case CPM_PM_DOZE:
			pm_val = cpm->pm_doze;
			break;
		case CPM_PM_NAP:
			pm_val = cpm->pm_nap;
			break;
		case CPM_PM_DEEPSLEEP:
			pm_val = cpm->pm_deepsleep;
			break;
		default:
			return -EINVAL;
		}

		cpm_save(i);
		cpm_enable(i, pm_val);
		cpm_force_enable(i, pm_val);
	}

	return 0;
}

static int cpm_pm_disable(int pm_mode)
{
	unsigned int i;

	for (i=0; i < cpm_count; i++) {
		switch(pm_mode) {
		case CPM_PM_DOZE:
		case CPM_PM_NAP:
		case CPM_PM_DEEPSLEEP:
			cpm_restore(i);
			break;
		default:
			return -EINVAL;
		}
	}

	return 0;
}

static void cpm_flush_caches(void)
{
	struct page *page;
	size_t size = PAGE_ALIGN(0x8000);
	unsigned long order = get_order(size);

	/* Flush and invalidate L1 cache */
	page = alloc_pages(GFP_NOWAIT, order);
	if (page) {
		unsigned long kaddr = (unsigned long)page_address(page);
		unsigned long endaddr = kaddr + size;
		unsigned long addr;

		for (addr = kaddr; addr < endaddr; addr += L1_CACHE_BYTES)
			asm ("dcbt 0,%0": :"r" (addr));
		asm ("sync");

		for (addr = kaddr; addr < endaddr; addr += L1_CACHE_BYTES)
			asm ("dcbf 0,%0": :"r" (addr));
		asm ("sync");
		asm ("isync");

		__free_pages(page, order);
	}

	if (dcrbase_l2c != 0)  {
		/* Invalidate L2 cache using the Hardware Clear Command */
		mtdcr(dcrbase_l2c + DCRN_L2C0_ADDR, 0);
		mtdcr(dcrbase_l2c + DCRN_L2C0_CMD, L2C_CMD_HCC);

		/* Wait for Command Complete to set */
		while (!(mfdcr(dcrbase_l2c + DCRN_L2C0_SR) & L2C_SR_CC))
			;
		//printk(KERN_INFO "%s\n", __func__);
	}
}

static int cpm_pm_mem_idle(int pm_mode)
{
	void __iomem *iicp = NULL;

	if (pm_mode < CPM_PM_NAP)
		return -EINVAL;

	if ( !cpm_pm_mem.suspend || !cpm_pm_mem.resume ||!cpm_pm_mem.data)
		return -ENOMEM;

	printk(KERN_INFO "%s\n", __func__);

	if (pm_mode == CPM_PM_DEEPSLEEP) {
		/* FIXME: Disable I2C interrupbecause it causes interrupt that wake up the system */
		struct cpm *cpm = get_cpm_node(0);
		iicp = ioremap(cpm->pm_iic.phys, cpm->pm_iic.size);
		cpm_pm_mem.data[CPM_PM_DATA_IIC_PTR] = (unsigned long)iicp;
	}

	if (!iicp) {
		printk(KERN_INFO "No iic\n");
		pm_mode = CPM_PM_NAP;
	} else {
		printk(KERN_INFO "%s: iic address = 0x%p\n",
			__func__, iicp);
		printk(KERN_INFO "%s: iic status = 0x%02x\n",
			__func__, in_8(iicp+IIC_STS));
	}
	/* call OCM code */
	cpm_pm_mem.suspend(pm_mode, cpm_pm_mem.data, cpm_pm_mem.resume);
	printk(KERN_INFO "%s: wakeup\n", __func__);

	if (iicp)
		iounmap(iicp);

	return 0;
}

int cpm_pm_idle(int pm_mode)
{
	unsigned long tcr_save = 0;
	unsigned long ccr0_save = 0;
	int emac0_mr0;
	void __iomem *emacp = NULL;
	struct cpm *cpm;

	printk(KERN_INFO "%s\n", __func__);


	cpm = get_cpm_node(0);
	emacp = ioremap(cpm->pm_emac.phys, cpm->pm_emac.size);
	if (emacp) {
		emac0_mr0 = in_be32(emacp + 0);
		/*printk(KERN_INFO "EMAC0_MR0 value before set WOL:0x%x\n", emac0_mr0);*/
 		emac0_mr0 &= CPM_PM_DISABLE_EMAC0_MR0_RXE;
 		out_be32(emacp + 0, emac0_mr0);
 		emac0_mr0 = in_be32(emacp + 0);
		while (!(emac0_mr0 & CPM_PM_EMAC0_MR0_RXI
					 && !(emac0_mr0 & CPM_PM_ENABLE_EMAC0_MR0_RXE))) {
			emac0_mr0 = in_be32(emacp + 0);
		}
 		emac0_mr0 |= CPM_PM_ENABLE_EMAC0_MR0_WKE;
 		out_be32(emacp + 0, emac0_mr0);
		emac0_mr0 = in_be32(emacp + 0);
		emac0_mr0 |= CPM_PM_ENABLE_EMAC0_MR0_RXE;
		out_be32(emacp + 0, emac0_mr0);
		/*printk(KERN_INFO "EMAC0_MR0 value after set WOL:0x%x\n", emac0_mr0);*/
	}

	ccr0_save = mfspr(SPRN_CCR0);
	tcr_save = mfspr(SPRN_TCR);
	mtspr(SPRN_TCR, tcr_save & ~TCR_DIE);
	mtspr(SPRN_CCR0, ccr0_save | CCR0_DTB | CCR0_GDCBT);

	cpm_flush_caches();
	cpm_pm_enable(pm_mode);
	

	if (cpm_pm_mem_idle(pm_mode) < 0) {
                unsigned long msr_save;

		/* set wait state MSR */
		msr_save = mfmsr();
                mtmsr(msr_save|MSR_WE|MSR_EE|MSR_CE|MSR_DE);
                isync();
                /* return to initial state */
                mtmsr(msr_save);
                isync();
	}

	cpm_pm_disable(pm_mode);
	mtspr(SPRN_CCR0, ccr0_save);
	mtspr(SPRN_TCR, tcr_save);
	
  	if (emacp) {
 		emac0_mr0 = in_be32(emacp + 0);
		emac0_mr0 &= CPM_PM_DISABLE_EMAC0_MR0_RXE;
		out_be32(emacp + 0, emac0_mr0);
		emac0_mr0 = in_be32(emacp + 0);
		while (!(emac0_mr0 & CPM_PM_EMAC0_MR0_RXI 
				&& !(emac0_mr0 & CPM_PM_ENABLE_EMAC0_MR0_RXE))) {
			emac0_mr0 = in_be32(emacp + 0);
		}
		emac0_mr0 &= CPM_PM_DISABLE_EMAC0_MR0_WKE;
 		out_be32(emacp + 0, emac0_mr0);
		emac0_mr0 = in_be32(emacp + 0);
		emac0_mr0 |= CPM_PM_ENABLE_EMAC0_MR0_RXE;
		out_be32(emacp + 0, emac0_mr0);
		/*printk(KERN_INFO "EMAC0_MR0 value after disable WOL:0x%x\n", emac0_mr0);*/
	}

	printk(KERN_INFO "%s: wakeup\n", __func__);

	return 0;
}


int cpm_pm_suspend(suspend_state_t state, int suspend_mode)
{
	int pm_mode;

	switch (state) {
	case PM_SUSPEND_STANDBY:
		/* standby only support DOZE */
		pm_mode = CPM_PM_DOZE;
		break;
	case PM_SUSPEND_MEM:
		if (suspend_mode < CPM_PM_NAP)
			suspend_mode = CPM_PM_DOZE;
		pm_mode = suspend_mode;
		break;
	default:
		printk(KERN_INFO "%s: -EINVAL\n", __func__);
		return -EINVAL;
	}

	cpm_pm_idle(pm_mode);

	return 0;
}

static void __init cpm_get_pm_emac(struct cpm *cpm, struct device_node *node)
{
	const phandle *phandle;
	struct device_node *dev_node;
	struct resource rsrc;

	phandle = of_get_property(node, "pm-emac-device", NULL);
	if (!phandle) {
		printk(KERN_INFO "CPM%d: pm-memory property not defined\n",
			cpm->index);
		return;
	}

	dev_node = of_find_node_by_phandle(*phandle);
	if (!dev_node) {
		printk(KERN_ERR "CPM%d: Can't find pm-emac-device node\n",
			cpm->index);
		return;
	}

	printk(KERN_INFO "CPM%d: pm-emac-device resource %s\n",
		cpm->index, dev_node->full_name);

	if (of_address_to_resource(dev_node, 0, &rsrc)) {
		printk(KERN_ERR "CPM%d: Can't get address to %s resource\n",
			cpm->index, dev_node->full_name);
		return;
	}

	cpm->pm_emac.phys = rsrc.start;
	cpm->pm_emac.size = rsrc.end - rsrc.start + 1;
}

static void __init cpm_get_pm_iic(struct cpm *cpm, struct device_node *node)
{
	const phandle *phandle;
	struct device_node *dev_node;
	struct resource rsrc;

	phandle = of_get_property(node, "pm-iic-device", NULL);
	if (!phandle) {
		printk(KERN_INFO "CPM%d: pm-memory property not defined\n",
			cpm->index);
		return;
	}

	dev_node = of_find_node_by_phandle(*phandle);
	if (!dev_node) {
		printk(KERN_ERR "CPM%d: Can't find pm-iic-device node\n",
			cpm->index);
		return;
	}

	printk(KERN_INFO "CPM%d: pm-iic-device resource %s\n",
		cpm->index, dev_node->full_name);

	if (of_address_to_resource(dev_node, 0, &rsrc)) {
		printk(KERN_ERR "CPM%d: Can't get address to %s resource\n",
			cpm->index, dev_node->full_name);
		return;
	}

	cpm->pm_iic.phys = rsrc.start;
	cpm->pm_iic.size = rsrc.end - rsrc.start + 1;

	//printk(KERN_INFO "CPM%d: pm-iic-device address 0x%llx\n",
		//cpm->index, cpm->pm_iic.phys);
}

static void __init cpm_init_node(struct device_node *node)
{
	struct cpm *cpm;
	const unsigned int *cell_index;
	const unsigned int *dcr_reg;
	const unsigned int *pm_cpu;
	const unsigned int *pm_off;
	const unsigned int *pm_doze;
	const unsigned int *pm_nap;
	const unsigned int *pm_deepsleep;
	int len;

	cell_index = of_get_property(node, "cell-index", &len);
	if (!cell_index)
        	BUG_ON("cpm: missing cell-index property");
	else if ((len != sizeof(unsigned int)) || *cell_index >= cpm_count)
        	BUG_ON("cpm: invalid cell-index property");
	else if (cpm_nodes[*cell_index].ready)
		BUG_ON("cpm: duplicate cell-index property");

	cpm = &cpm_nodes[*cell_index];
	cpm->index = *cell_index;

	dcr_reg = of_get_property(node, "dcr-reg", &len);
	if (!dcr_reg || (len != 2*sizeof(unsigned int)))
		BUG_ON("cpm: missing or invalid dcr-reg property");

	cpm->dcrbase = *dcr_reg;

	pm_cpu = of_get_property(node, "pm-cpu", &len);
	if (pm_cpu && (len == sizeof(unsigned int)))
		cpm->pm_cpu = *pm_cpu;

	pm_off = of_get_property(node, "pm-off", &len);
	if (pm_off && (len == sizeof(unsigned int)))
		cpm->pm_off = *pm_off;

	pm_doze = of_get_property(node, "pm-doze", &len);
	if (pm_doze && (len == sizeof(unsigned int)))
		cpm->pm_doze = *pm_doze;

	pm_nap = of_get_property(node, "pm-nap", &len);
	if (pm_nap && (len == sizeof(unsigned int)))
		cpm->pm_nap = *pm_nap;

	pm_deepsleep = of_get_property(node, "pm-deepsleep", &len);
	if (pm_deepsleep && (len == sizeof(unsigned int)))
		cpm->pm_deepsleep = *pm_deepsleep;

	cpm_get_pm_iic(cpm, node);
	cpm_get_pm_emac(cpm, node);

	printk (KERN_INFO "CPM%d: DCR at 0x%x\n", cpm->index, cpm->dcrbase);

	cpm->ready = 1;

	mtdcr(cpm->dcrbase + CPM_ER, 
		mfdcr(cpm->dcrbase + CPM_ER) | cpm->pm_off | cpm->pm_cpu);
	mtdcr(cpm->dcrbase + CPM_FR,
		mfdcr(cpm->dcrbase + CPM_FR) | cpm->pm_off);

	/* put unused IP into sleep */
	if (pcie_used == 0){
		mtdcr(cpm->dcrbase + CPM_ER,
			mfdcr(cpm->dcrbase + CPM_ER) | CPM_PCIE_SLEEP);
		mtdcr(cpm->dcrbase + CPM_FR,
			mfdcr(cpm->dcrbase + CPM_FR) | CPM_PCIE_SLEEP);
	}
	else if (pcie_used == 1){
		mtdcr(cpm->dcrbase + CPM_ER,
			mfdcr(cpm->dcrbase + CPM_ER) | CPM_SATA0_SLEEP);
		mtdcr(cpm->dcrbase + CPM_FR,
			mfdcr(cpm->dcrbase + CPM_FR) | CPM_SATA0_SLEEP);
	}
}


static int __init cpm_pm_mem_init(void)
{
	struct cpm_pm_mem *mem;
	phys_addr_t p;

	mem = &cpm_pm_mem;

	memset(mem,0,sizeof(struct cpm_pm_mem));

	mem->suspend = ocm_alloc(&p,cpm_suspend_mem_size, CPM_PM_MEM_ALIGN,

				OCM_NON_CACHED, "cpm_suspend_mem");
	if (!mem->suspend) {
		printk(KERN_ERR "CPM: failed to allocate suspend memory!\n");
		return -ENOMEM;
	}

	mem->resume = ocm_alloc(&p,cpm_resume_mem_size, CPM_PM_MEM_ALIGN,
				OCM_NON_CACHED, "cpm_resume_mem");
	if (!mem->resume) {
		printk(KERN_ERR "CPM: failed to allocate resume memory!\n");
		ocm_free(mem->suspend);
		mem->suspend = NULL;
		return -ENOMEM;
	}

	mem->data = ocm_alloc(&p,CPM_PM_MEM_DATA_SIZE, CPM_PM_MEM_ALIGN,
				OCM_NON_CACHED, "cpm_data_mem");
	if (!mem->data) {
		printk(KERN_ERR "CPM: failed to allocate data memory!\n");
		ocm_free(mem->suspend);
		ocm_free(mem->resume);
		mem->suspend = NULL;
		mem->resume = NULL;
		return -ENOMEM;
	}

	printk(KERN_INFO "CPM: ocm suspend address 0x%p\n",mem->suspend);
	printk(KERN_INFO "CPM: ocm resume address 0x%p\n",mem->resume);
	printk(KERN_INFO "CPM: ocm data address 0x%p\n",mem->data);

	memcpy(mem->suspend, cpm_suspend_mem, cpm_suspend_mem_size);
	memcpy(mem->resume, cpm_resume_mem, cpm_resume_mem_size);
	memset(mem->data,0,CPM_PM_MEM_DATA_SIZE);

	return 0;
}

static int __init cpm_init(void)
{
	struct device_node *np;
	int count;
	const u32 *dcrreg;

	count = 0;
	for_each_compatible_node(np, NULL, "ibm,cpm") {
		count++;
	}
// 	printk(KERN_ERR "CPM: cpm_init %d!\n", count);
	if (!count) {
		return 0;
	}

        cpm_nodes = kzalloc((count*sizeof(struct cpm)), GFP_KERNEL);
        if (!cpm_nodes) {
                printk(KERN_ERR "CPM: out of memory allocating CPM!\n");
		return -ENOMEM;
        }

	cpm_count = count;

	for_each_compatible_node(np, NULL, "ibm,cpm") {
		cpm_init_node(np);
	}
	
	cpm_pm_mem_init();

//	cpm_proc_init();

	np = of_find_compatible_node(NULL, NULL, "ibm,l2-cache");
	if (!np) {
		printk(KERN_ERR "Can't get l2-cache node\n");
		return 0;
	}

	dcrreg = of_get_property(np, "dcr-reg", &count);
	if (!dcrreg || (count != 4 * sizeof(u32))) {
		printk(KERN_ERR "%s: Can't get DCR register base !",
		       np->full_name);
		of_node_put(np);
		return -ENODEV;
	}
	dcrbase_l2c = dcrreg[2];

	return 0;
}

subsys_initcall(cpm_init);
