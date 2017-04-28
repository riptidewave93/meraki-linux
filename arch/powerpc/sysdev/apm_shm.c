/**
 * APM APM86xxx Shared Memory Locking and Manager
 *
 * Copyright (c) 2010, Applied Micro Circuits Corporation
 * Author: Feng Kan <fkan@apm.com>
 * Author: Vivek Mahajan <vmahajan@apm.com>
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
#include <linux/kernel.h>
#include <linux/of_platform.h>
#include <asm/pgtable.h>
#include <asm/apm_shm.h>

#define PFX "APMSHM: "
struct shmem_lo_ctx *shmem_lo;
static u32 *shmlck;

static int apm_shmem_avail(unsigned int size)
{
	phys_addr_t mem_reqd = shmem_lo->paddr + size + sizeof(struct shmem_list);
	phys_addr_t mem_avail = shmem_lo->pbase + shmem_lo->size;

	if (!shmem_lo->pbase) {
		pr_err(PFX "Base not initialized!!\n");
		return -EINVAL;
	}

	if (mem_reqd > mem_avail)
		return MEM_NOT_AVAIL;

	if (mem_reqd < mem_avail)
		return MEM_AVAIL;

	return MEM_JUST_ENOUGH;
}

void atomic_csr_setbits(u32 masks, void *csr)
{
	spin_lock(&shmem_lo->shm->lock);
	setbits32(csr, masks);
	spin_unlock(&shmem_lo->shm->lock);
}
EXPORT_SYMBOL(atomic_csr_setbits);

void atomic_csr_clrbits(u32 masks, void *csr)
{
	spin_lock(&shmem_lo->shm->lock);
	clrbits32(csr, masks);
	spin_unlock(&shmem_lo->shm->lock);
}
EXPORT_SYMBOL(atomic_csr_clrbits);

void atomic_csr_clrsetbits(u32 clrmasks, u32 setmasks, void *csr)
{
	spin_lock(&shmem_lo->shm->lock);
	clrsetbits_be32(csr, clrmasks, setmasks);
	spin_unlock(&shmem_lo->shm->lock);
}
EXPORT_SYMBOL(atomic_csr_clrsetbits);

static int apm_shmem_name_info(char *name, struct shmem_list *list)
{
	int i;
	unsigned int addr;
	struct shmem_ctx *shmem = shmem_lo->shm;
	struct shmem_list *temp;

	if (!shmem_lo->pbase) {
		pr_err(PFX "Base not initialized!!\n");
		return -EINVAL;
	}

	if (!strlen(name)) {
		pr_err(PFX "Name string incorrect!!\n");
		return -EINVAL;
	}

	temp = (struct shmem_list *) shmem_lo->vbase;
	for (i = 0; i < shmem->entries; i++) {
		if (!strcmp(temp->name, name)) {
			if (list)
				memcpy(list, temp, sizeof(struct shmem_list));
			return 1;
		}
		addr = (unsigned int) temp->pnext;
		temp = (struct shmem_list *) (shmem_lo->vbase + 
				(addr - (unsigned int)shmem_lo->pbase));
	}
	return 0;
}

void *apm_shmem_alloc(char *name, unsigned int size)
{
	struct shmem_ctx *shmem; 
	struct shmem_list shlist;
	struct shmem_list *list;
	void *vaddr;

	if (!shmem_lo || !shmem_lo->pbase) {
		pr_err(PFX "Shared Memory unavailable\n");
		return NULL;
	}

	shmem = shmem_lo->shm;
		
	spin_lock(&shmem->lock);
	if (apm_shmem_avail(size) < MEM_JUST_ENOUGH) {
		pr_err(PFX "Out of memory!\n");
		spin_unlock(&shmem->lock);
		return NULL;
	}

	if (!apm_shmem_name_info(name, NULL)) {
		vaddr = (void *)(shmem_lo->vbase + 
				((unsigned long) (shmem_lo->paddr - shmem_lo->pbase) +
				sizeof(struct shmem_list)));
		list = (struct shmem_list *) (vaddr - 
						sizeof(struct shmem_list));
		list->phandle = (unsigned int)shmem_lo->paddr;
		shmem_lo->paddr += (size + sizeof(struct shmem_list));
		list->pnext = (int *)(unsigned int)shmem_lo->paddr;
		shmem->entries++;

		strncpy(list->name, name, strlen(name));
		list->name[NAME_SZ - 1] = '\0';
		list->paddr = (void *) (list->phandle + 
					sizeof(struct shmem_list));
		list->size = size;
		spin_unlock(&shmem->lock);
		return vaddr;
	}

	spin_unlock(&shmem->lock);
	apm_shmem_name_info(name, &shlist);
	return (void *)(shmem_lo->vbase + (shlist.paddr - shmem_lo->pbase));
}
EXPORT_SYMBOL(apm_shmem_alloc);

static int __devinit apm_shm_of_probe(struct of_device *ofdev,
				const struct of_device_id *match)
{
	unsigned long size;
	struct resource res;
	unsigned int __iomem *ptr;
	unsigned long flags;

	shmem_lo = NULL;
	shmlck = NULL;
	
	if (of_address_to_resource(ofdev->node, 0, &res)) {
		dev_err(&ofdev->dev, "Can't read shmem window\n");
		return -EFAULT;
	}

	size = (unsigned long)(res.end - res.start) + 1;
	if (size & (PAGE_SIZE - 1)) {
		dev_err(&ofdev->dev, "Size is not page aligned\n");
		return -EFAULT;
	}
	
	if (res.start == 0) {
		dev_err(&ofdev->dev, 
			"invalid shared memory physical address\n");
		return -EFAULT;
	}
	
#if !defined(CONFIG_NOT_COHERENT_CACHE)
	ptr = ioremap_flags(res.start, size, _PAGE_COHERENT | PAGE_KERNEL);
#else
	ptr = ioremap_flags(res.start, size,
		_PAGE_COHERENT | _PAGE_GUARDED);
#endif
	if (!ptr) {
		dev_err(&ofdev->dev, "%s: Can't map shared memory\n",
				ofdev->node->full_name);
		return -ENOMEM;
	}

	/* Allocate the location ctx structure */
	shmem_lo = kzalloc(sizeof(struct shmem_lo_ctx), GFP_KERNEL);

	shmem_lo->shm = (struct shmem_ctx *) ptr;
	shmem_lo->vbase = (unsigned int) ptr + 
					sizeof(struct shmem_ctx) + 4;
	shmem_lo->pbase = res.start + sizeof(struct shmem_ctx) + 4;
	shmem_lo->size = size - (sizeof(struct shmem_ctx) + 4);
	shmem_lo->paddr = shmem_lo->pbase;

	shmlck = ptr;

#if defined(CONFIG_NOT_COHERENT_CACHE)
	*ptr = 0;
#endif
	spin_lock_irqsave(&shmem_lo->shm->lock, flags);
	if (shmem_lo->shm->signature != SHM_SIGN) {
		shmem_lo->shm->entries = 0;
		shmem_lo->shm->signature = SHM_SIGN;
	}
	spin_unlock_irqrestore(&shmem_lo->shm->lock, flags);

	dev_info(&ofdev->dev, "%s @0x%010llx initialized\n", 
		APM86xxx_SHMEM_STRING, res.start);
	return 0;
}

static int __devexit apm_shm_remove(struct of_device *dev)
{
	BUG_ON(!shmem_lo->shm);

	iounmap((void *)shmem_lo->shm);
	kfree((void *)shmem_lo);
	dev_info(&dev->dev, "%s unloaded\n", APM86xxx_SHMEM_STRING);
	return 0;
}

static struct of_device_id apm_shm_of_match[] = {
	{
		.compatible      = "apm,apm86xxx-shm",
	},
	{},
};

MODULE_DEVICE_TABLE(of, apm_shm_match);

static struct of_platform_driver apm_shm_driver = {
	.name		= "apm86xxx-shm",
	.match_table	= apm_shm_of_match,
	.probe		= apm_shm_of_probe,
	.remove		= __devexit_p(apm_shm_remove),
};

static int __init apm_shm_init(void)
{
	return of_register_platform_driver(&apm_shm_driver);
}

static void __exit apm_shm_cleanup(void)
{
	of_unregister_platform_driver(&apm_shm_driver);
}

module_init(apm_shm_init);
module_exit(apm_shm_cleanup);

MODULE_DESCRIPTION(APM86xxx_SHMEM_STRING);
MODULE_AUTHOR("Vivek Mahajan <vmahajan@apm.com>");
MODULE_LICENSE("GPL v2");
