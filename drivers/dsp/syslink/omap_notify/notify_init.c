/*
 * Syslink notify module implementation.
 *
 * Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/
 *
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


#include <linux/spinlock.h>
#include <linux/semaphore.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/list.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <asm/pgtable.h>
#include <linux/types.h>
#include <linux/cdev.h>
#include <linux/slab.h>


/*Linux specific headers*/
#include <plat/iommu.h>
#include <plat/iovmm.h>
#include <asm/page.h>
#include <linux/pagemap.h>
#include <linux/platform_device.h>
#include <linux/clk.h>


#include <syslink/notify_driver.h>
#include <syslink/notify.h>
#include <syslink/notify_shm_drv.h>

/** ============================================================================
 *  Macros and types
 *  ============================================================================
 */
/*!
 *  @brief  Page size constants
 */
#define PAGE_SIZE_4KB             0x1000
/*!
 *  @brief  Page size constants
 */
#define PAGE_SIZE_64KB            0x10000
/*!
 *  @brief  Page size constants
 */
#define PAGE_SIZE_1MB             0x100000
/*!
 *  @brief  Page size constants
 */
#define PAGE_SIZE_16MB            0x1000000


#define NOTIFY_MMUPAGE_ALIGN(size, psz)  (((size) + psz - 1) & ~(psz - 1))
#define MMU_CAM_PRESERVE          (1 << 3)
#define MMU_CAM_VALID             (1 << 2)
#define MMU_RAM_DEFAULT         0
#define IOPTE_SHIFT     12
#define IOPTE_SIZE      (1 << IOPTE_SHIFT)
#define IOPTE_MASK      (~(IOPTE_SIZE - 1))
#define IOPAGE_MASK     IOPTE_MASK

/* notify slave virtual address of dsp*/
static  int  dsp_notify_va ;
module_param_named(dsp_sva, dsp_notify_va, int,
			S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(dsp_notify_va, "Specify the slave virtual address where the "
				"notify driver for dsp will be created."
				" ignore notify driver creation  is not"
				" required at kernel boot time ");
#if defined(CONFIG_ARCH_TI81XX)
/* notify slave virtual address of videom3*/
static  int  videom3_notify_va ;
module_param_named(videom3_sva, videom3_notify_va, int,
			S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(videom3_notify_va, "Specify the slave virtual address where"
				"the notify driver for video m3 will be "
				"created. ignore notify driver creation  is not"
				"required at kernel boot time ");

/* notify slave virtual address of vpssm3*/
static  int  vpssm3_notify_va ;
module_param_named(vpssm3_sva, vpssm3_notify_va, int,
			S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(vpssm3_notify_va, "Specify the slave virtual address where the"
				"notify driver for vpssm3 will be created."
				" ignore notify driver creation  is not "
				"required at kernel boot time ");
#endif

/* IOMMU Exported function */
extern
void
iopgtable_lookup_entry(struct iommu *obj, u32 da, u32 **ppgd, u32 **ppte);



struct notify_map_table_info {
	u32    actualAddress;
	/*!< Actual address( physical address) */
	u32    mappedAddress;
	/*!< Mapped address */
	u32    size;
	/*!< Size of the region mapped */
	bool      isCached;
	/*!< Is cached? */
} ;

struct omap_notify {
	char           name[32];
	struct iommu *mmu_handle;
	struct clk   *clk_handle;
	u32    map_index;
	u16    rproc_id;
	bool   attached;
};

#if defined(CONFIG_ARCH_OMAP3430)
/* DSP */
struct omap_notify notify_dsp_info = {
	.name	= "DSP", /* same as multiproc name for dsp */
	.mmu_handle = NULL,
	.clk_handle = NULL,
	.map_index  = 0
};
struct omap_notify *omap3_notify[] = { &notify_dsp_info, NULL };
#endif

#if defined(CONFIG_ARCH_TI81XX)
/* DSP */
struct omap_notify notify_dsp_info = {
	.name	= "DSP", /* same as multiproc name for dsp */
	.mmu_handle = NULL,
	.clk_handle = NULL,
	.map_index  = 0
};

/* VideoM3 */
struct omap_notify notify_video_info = {
	.name	= "VIDEO-M3", /* same as multiproc name for dsp */
	.mmu_handle = NULL,
	.clk_handle = NULL,
	.map_index  = 0
};

/* VpssM3 */
struct omap_notify notify_vpss_info = {
	.name	= "VPSS-M3", /* same as multiproc name for dsp */
	.mmu_handle = NULL,
	.clk_handle = NULL,
	.map_index  = 0
};
struct omap_notify *ti81xx_notify[] = { &notify_dsp_info,
					&notify_video_info,
					&notify_vpss_info,
					NULL };
#endif

struct omap_notify **notifies;

/*Mapped memory information  associated with each remote proc */
static struct notify_map_table_info *notify_map_info;

#define REG(x)              (*((volatile u32 *) (x)))
#define MEM(x)              (*((volatile u32 *) (x)))
#define SYSTEM_CONTROL_ADDR     0x48002000
#define SYSTEM_CONTROL_SIZE     0xC00
#define CONTROL_REVISION        0x00000000
#define CONTROL_IVA2_BOOTADDR   0x00000400
#define CONTROL_IVA2_BOOTMOD    0x00000404

#define PRCM_IVA2_CM_ADDR       0x48004000
#define PRCM_IVA2_CM_SIZE       0x800
#define CM_FCLKEN_IVA2          0x00000000
#define CM_CLKEN_PLL_IVA2       0x00000004
#define CM_IDLEST_IVA2          0x00000020
#define CM_AUTOIDLE_PLL_IVA2    0x00000034
#define CM_CLKSEL1_PLL_IVA2     0x00000040
#define CM_CLKSEL2_PLL_IVA2     0x00000044
#define CM_CLKSTCTRL_IVA2       0x00000048

#define PRCM_IVA2_PRM_ADDR      0x48306000
#define PRCM_IVA2_PRM_SIZE      0x800
#define RM_RSTCTRL_IVA2         0x00000050
#define RM_RSTST_IVA2           0x00000058
#define PM_WKDEP_IVA2           0x000000C8
#define PM_PWSTCTRL_IVA2        0x000000E0
#define PM_PWSTST_IVA2          0x000000E4

#define MAX_WAIT_COUNT          0x50000


int
OMAP3530PWR_off(u32 cm, u32 prm)
{
	return 0;
}

struct clk *clk_handle ;
/**
 * OMAP3530PWR_on - relese IVA2 MMU .
 */
int
OMAP3530PWR_on(void)
{
	int err = 0;
	u32               i;
	u32               cm;
	u32               prm;

	cm      = (u32)ioremap(0x48004000, 0x800);
	prm     = (u32)ioremap(0x48306000, 0x800);

	/* release IVA2_RST2 reset (MMU, SYSC, local interconnect, etc) */
	REG(prm + RM_RSTCTRL_IVA2) &= ~(1 << 1);
	REG(prm + RM_RSTCTRL_IVA2);

	/* wait for IVA2_RST2 signal to be released */
	for (i = 0; i < MAX_WAIT_COUNT; i++) {
		if ((REG(prm + RM_RSTST_IVA2) & (1 << 9)))
			break;
	}
	clk_handle = clk_get(NULL, "iva2_ck");
	if (IS_ERR(clk_handle)) {
		err = PTR_ERR(clk_handle);
		printk(KERN_ERR "OMAP3530PWR_on: clk_get failed for iva2_ck\n");
		goto exit;
	}

	clk_enable(clk_handle);

	return 0;
exit:
	return err;
}

/*!
 *  @brief      This function adds an MMU entry for the specfied address and
 *              size.
 */
int
notify_add_mmu_entry(void *mmu_handle, u32 slave_virt_addr, u32 size)
{
	s32                     status = 0;
	u32  *ppgd = NULL;
	u32  *ppte = NULL;
	s32                     cur_size;
	u32                     master_phys_addr;
	struct iotlb_entry tlb_entry;

	/* Align the addresses to page size */
	size += ((u32)slave_virt_addr & (PAGE_SIZE_4KB - 1));
	master_phys_addr = (u32)slave_virt_addr;
	slave_virt_addr &= ~(PAGE_SIZE_4KB-1);
	master_phys_addr &= ~(PAGE_SIZE_4KB-1);

	/* Align the size as well */
	size = NOTIFY_MMUPAGE_ALIGN(size, PAGE_SIZE_4KB);
	cur_size = size;

	/* Check every page if exists */
	do {
		/* Lookup if the entry exists */

		iopgtable_lookup_entry(mmu_handle,
					slave_virt_addr,
					(u32 **) &ppgd,
					(u32 **) &ppte);
		if (!*ppgd || !*ppte) {
			/* Entry doesnot exists, insert this page */
			tlb_entry.pgsz  = MMU_CAM_PGSZ_4K;
			tlb_entry.prsvd  = MMU_CAM_PRESERVE;
			tlb_entry.valid  = MMU_CAM_VALID;
			tlb_entry.elsz = MMU_RAM_ELSZ_16;
			tlb_entry.endian = MMU_RAM_ENDIAN_LITTLE;
			tlb_entry.mixed = MMU_RAM_DEFAULT;
			tlb_entry.da = slave_virt_addr;
			tlb_entry.pa = master_phys_addr;
			if (iopgtable_store_entry(mmu_handle, &tlb_entry)) {
					printk(KERN_ERR "ERROR in "
					"notify_add_mmu_entry\n");
				status = -1;
			}
		} else if (*ppgd && *ppte) {
			if (master_phys_addr != (*ppte & IOPTE_MASK)) {
				/* Entry doesnot exists, insert this page */
				tlb_entry.pgsz  = MMU_CAM_PGSZ_4K;
				tlb_entry.prsvd = MMU_CAM_PRESERVE;
				tlb_entry.valid = MMU_CAM_VALID;
				tlb_entry.elsz  = MMU_RAM_ELSZ_16;
				tlb_entry.endian = MMU_RAM_ENDIAN_LITTLE;
				tlb_entry.mixed  = MMU_RAM_DEFAULT;
				tlb_entry.da = slave_virt_addr;
				tlb_entry.pa = master_phys_addr;
				if (iopgtable_store_entry(mmu_handle,
								&tlb_entry)) {
						printk(KERN_ERR "ERROR in"
						"notify_add_mmu_entry\n");
					status = -1;
				}
			}
		}
		cur_size -= PAGE_SIZE_4KB;
		slave_virt_addr += PAGE_SIZE_4KB;
		master_phys_addr += PAGE_SIZE_4KB;
	} while (cur_size);

	return status;
}


unsigned int __initdata notify_pa;

/*
 * ======== notify_init ========
 *  Initialization routine. Executed when the driver is
 *  loaded (as a kernel module), or when the system
 *  is booted (when included as part of the kernel
 *  image).
*/
static int __init notify_init(void)
{
	s32 retval = 0;
	u32 memreq;
	u16 i = 0;
	struct omap_notify **list;

	multiproc_setup(NULL);

	if (false)
		;
#if defined(CONFIG_ARCH_OMAP3430)
	else if (cpu_is_omap343x()) {
		list = omap3_notify;
		notifies = list;
		OMAP3530PWR_on();
		list[0]->mmu_handle = iommu_get("iva2");
		if (list[0]->mmu_handle == NULL) {
			printk(KERN_ERR "notify_init: iommu_get failed for"
				"iva2\n");
			goto setup_fail;
		}

	}
#endif
#if defined(CONFIG_ARCH_TI81XX)
	else if (cpu_is_ti81xx()) {
		list = ti81xx_notify;
		notifies = list;
	/* FIX Me Add mmu handles to do mappings  */
	}
#endif
	notify_setup(NULL);

	/* Create notify drivers based on configuration*/
	retval = notify_shm_drv_setup(NULL);
	if (retval) {
		printk(KERN_ERR "notify_init : notify_shm_drv_setup failed\n");
		goto setup_fail;
	}

	notify_map_info = kzalloc(((sizeof(struct notify_map_table_info)) *
					multiproc_get_num_processors()),
					GFP_KERNEL);
	if (!notify_map_info) {
		printk(KERN_ERR "notify_init : kmalloc failed for"
				"notify_map_info\n");
		goto setup_fail;
	}

	if (dsp_notify_va != 0) {
		i = multiproc_get_id("DSP");
		if (i != MULTIPROC_INVALIDID) {
			list[0]->map_index = i;
			notify_map_info[i].actualAddress = dsp_notify_va;
			memreq = notify_shared_mem_req(i, (void *)
					notify_map_info[i].actualAddress);
			notify_map_info[i].size = memreq;

			if (cpu_is_omap343x()) {
				notify_add_mmu_entry(list[0]->mmu_handle,
					notify_map_info[i].actualAddress,
					notify_map_info[i].size);
			}
		}
		else {
			printk(KERN_ERR "notify_init: dsp_notify_va was "
			       "specified but no DSP found\n");
		}
	}

#if defined(CONFIG_ARCH_TI81XX)
	if (cpu_is_ti81xx()) {
		/* check if memory is provided to create notify with other
		 * procs.
		 */
		if (videom3_notify_va != 0) {
			i = multiproc_get_id("VIDEO-M3");
			list[1]->map_index = i;
			notify_map_info[i].actualAddress = videom3_notify_va;
			memreq = notify_shared_mem_req(i,                      \
			(void *)notify_map_info[i].actualAddress);
			notify_map_info[i].size = memreq;

			/* Fix Me adde mmu mapping for this memory */
		}

		if (vpssm3_notify_va != 0) {
			i = multiproc_get_id("VPSS-M3");
			list[2]->map_index = i;
			notify_map_info[i].actualAddress = vpssm3_notify_va;
			memreq = notify_shared_mem_req(i,                      \
				(void *)notify_map_info[i].actualAddress);
			notify_map_info[i].size = memreq;

			/* Fix Me adde mmu mapping for this memory */
		}
	}
#endif

	for (i = 0; i < multiproc_get_num_processors(); i++) {
		if (i != multiproc_self()
			&& notify_map_info[i].actualAddress != 0) {
			u32 vaddr = (u32) ioremap_nocache((dma_addr_t)
					notify_map_info[i].actualAddress,
					notify_map_info[i].size);

			notify_map_info[i].mappedAddress = vaddr;
			notify_map_info[i].isCached = false;
			/* Calling notify_attach to create driver */
			retval = notify_attach(i,
				(void *)notify_map_info[i].mappedAddress);
			if (retval < 0) {
				printk(KERN_ERR "notify_init : notify_attach " \
					"failed for  remote proc id %d\n", i);
				goto notify_attach_fail;
			} else {
				list[0]->attached = true;
				list[0]->rproc_id = i;
				printk(KERN_INFO  "notify_init : notify driver"
					"created  for  remote proc id %d at "
					"physical Address 0x%x\n",
					i, notify_map_info[i].actualAddress);
			}
		}
	}

	return retval;

notify_attach_fail:
	i = 0;
	while (notifies[i] != NULL) {
		if (notifies[i]->attached == true)
			notify_detach(notifies[i]->rproc_id);
	}
	for (i = 0; i < multiproc_get_num_processors(); i++) {
		if ((i != multiproc_self()) &&
			((void *)notify_map_info[i].mappedAddress != NULL))
			iounmap((unsigned int *)
				notify_map_info[i].mappedAddress);

	}

setup_fail:
	if (!notify_map_info)
		kfree(notify_map_info);

	notify_shm_drv_destroy();

	notify_destroy();

	return retval;
}

/*
 * ======== notify_exit ========
 *  This function is invoked during unlinking of notify
 *  module from the kernel. notify resources are
 *  freed in this function.
 */
static void __exit notify_exit(void)
{
	u32   i = 0;

	for (i = 0; i < multiproc_get_num_processors(); i++) {
		if ((i != multiproc_self()) &&
			((void *)notify_map_info[i].mappedAddress != NULL))
			iounmap((u32 *)notify_map_info[i].mappedAddress);
	}
	i = 0;
	while (notifies[i] != NULL) {
		if (notifies[i]->attached == true)
			notify_detach(notifies[i]->rproc_id);

		if (notifies[i]->mmu_handle)
			iommu_put(notifies[i]->mmu_handle);
	}
}

module_init(notify_init);
module_exit(notify_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("notify : multicore notify implementation");
MODULE_AUTHOR("Texas Instruments");
