/*
 * APM86xxx OCM memory allocation support
 *
 * Copyright (c) 2010 Applied Micro Circuits Corporation.
 * Victor Gallardo <vgallardo@apm.com>.
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
#include <linux/seq_file.h>
#include <linux/spinlock.h>
#include <linux/dma-mapping.h>
#include <linux/list.h>
#include <linux/sysdev.h>
#include <asm/uaccess.h>
#include <asm/prom.h>
#include <asm/dcr.h>
#include <asm/rheap.h>
#include <asm/mmu.h>
#include <asm/apm86xxx_ocm.h>
#include <asm/apm86xxx_pm.h>

struct ocm_block {
	struct list_head	list;
	void __iomem		*addr;
	int			size;
	const char 		*owner;
};

struct ocm_partition {
	int			index;
	int			type;
	int			alignment;
	unsigned long		offset;

	phys_addr_t		phys;
	void __iomem		*virt;
	void			*pm_mem;

	int			memtotal;
	int			memfree;

	rh_info_t		*rh;
	struct list_head	list;
};

struct ocm_info {
	int			index;
	int			ready;

	phys_addr_t		phys;
	int			memtotal;

	struct ocm_partition	*parts;
	int			pcount;
};

static struct ocm_info *ocm_nodes = NULL;
static int ocm_count = 0;
static spinlock_t ocm_lock;

static inline const char *ocm_type(struct ocm_partition *part)
{
	switch (part->type) {
	case OCM_NON_CACHED:
		return "non-cached";
	case OCM_CACHED:
		return "cached";
	case OCM_IPP:
		return "ipp";
	default:
		return "UNKNOWN";
	}
}

static struct ocm_info * ocm_get_node(unsigned int index)
{
	if (index >= ocm_count) {
		return NULL;
	}

	return &ocm_nodes[index];
}

static struct ocm_partition * ocm_get_partition(struct ocm_info *ocm, int type)
{
	int i;

	for (i = 0; i < ocm->pcount; i++) {
		if (ocm->parts[i].virt && type == ocm->parts[i].type) {
			return &ocm->parts[i];
		}
	}

	return NULL;
}

void * ocm_get_buf(phys_addr_t *phys, int *size, int type, int *offset)
{
	struct ocm_info *ocm;
	struct ocm_partition *part;
	int i;

	for (i = 0; i < ocm_count; i++) {
		ocm = ocm_get_node(i);

		part = ocm_get_partition(ocm, type);
		if (!part)
			continue;	

		switch(part->type) {
		case OCM_IPP:
			if (part->virt) {
				part->memfree = 0;

				if (phys)
					*phys = part->phys;
				if (size)
					*size = part->memtotal;
				if (offset)
					*offset= part->offset;

				return part->virt;
			}
			break;
		default:
			return NULL;
		}
	}

	return NULL;
}

void * ocm_alloc(phys_addr_t *phys, int size, int align,
		  int flags, const char *owner)
{
	void __iomem *addr = NULL;
	unsigned long offset;
	unsigned long sl_flags;
	struct ocm_info *ocm;
	struct ocm_partition *part;
	struct ocm_block *ocm_blk;
	int i;

	if(!(flags == OCM_NON_CACHED || flags == OCM_CACHED))
		return NULL;

	spin_lock_irqsave(&ocm_lock, sl_flags);

	for (i = 0; i < ocm_count; i++) {
		ocm = ocm_get_node(i);

		part = ocm_get_partition(ocm, flags);
		if (!part)
			continue;
		
		if (align < part->alignment)
			align = part->alignment;

		offset = rh_alloc_align(part->rh, size, align, NULL);

		if (IS_ERR_VALUE(offset))
			continue;

		ocm_blk = kzalloc(sizeof(struct ocm_block*), GFP_KERNEL);
		if (!ocm_blk) {
			printk(KERN_ERR "OCM: could not allocate ocm block");
			rh_free(part->rh, offset);
			break;
		}

		*phys = part->phys + offset;
		addr = part->virt + offset;
		size = ALIGN(size, align);

		ocm_blk->addr = addr;
		ocm_blk->size = size;
		ocm_blk->owner = owner;
		list_add_tail(&ocm_blk->list, &part->list);

		part->memfree -= size;

		break;
	}

	spin_unlock_irqrestore(&ocm_lock, sl_flags);
	return addr;
}
EXPORT_SYMBOL(ocm_alloc);

static int ocm_free_region(struct ocm_info *ocm, const void * addr)
{
	struct ocm_block *blk, *tmp;
	unsigned long offset;
	int i;

	for (i = 0; i < ocm->pcount; i++) {
		struct ocm_partition *part = &ocm->parts[i];

		if (part->virt)
			continue;

		if(!(part->type == OCM_NON_CACHED || part->type == OCM_CACHED))
			continue;

		list_for_each_entry_safe(blk,tmp,&part->list,list) {
			if (blk->addr == addr) {
				offset = addr - part->virt;
				part->memfree += blk->size;
				rh_free(part->rh, offset);
				list_del(&blk->list);
				kfree(blk);
				return 1;
			}
		}
	}

	return 0;
}

void ocm_free(const void * addr)
{
	unsigned long sl_flags;
	int i;

	if (!addr)
		return;

	spin_lock_irqsave(&ocm_lock, sl_flags);

	for (i=0; i < ocm_count; i++) {
		struct ocm_info *ocm = ocm_get_node(i);

		if (ocm_free_region(ocm, addr))
			break;
	}

	spin_unlock_irqrestore(&ocm_lock, sl_flags);
}
EXPORT_SYMBOL(ocm_free);

static void __init ocm_map_partition(struct ocm_info *ocm, int index)
{
	int ioflags;
	struct ocm_partition *part = &ocm->parts[index];

	if (part->type == OCM_CACHED)
		ioflags = _PAGE_EXEC;
	else
		ioflags = _PAGE_NO_CACHE | _PAGE_EXEC;

	part->virt = __ioremap(part->phys, part->memtotal, ioflags);
	if (!part->virt) {
		printk(KERN_WARNING
			"OCM%d: failed to ioremap memory partition %d\n",
			ocm->index, index);
		part->memfree = 0;
		return;
	}

	if(!(part->type == OCM_NON_CACHED || part->type == OCM_CACHED))
		goto done;

	part->pm_mem = kmalloc(part->memtotal, GFP_KERNEL);
	if (!part->pm_mem) {
		printk(KERN_WARNING
			"OCM%d: failed to alloc PM partition %d\n",
			ocm->index, index);
	}

	/* Create Remote Heaps */

	part->alignment = 4; /* default 4 byte alignment */

	part->rh = rh_create(part->alignment);
	rh_attach_region(part->rh, 0, part->memtotal);

	INIT_LIST_HEAD(&part->list);

done:
	printk(KERN_INFO "OCM%d.%d: %d Bytes (%s)\n",
		ocm->index, index, part->memtotal, ocm_type(part));
}

static int __init ocm_init_partitions(struct device_node *node,
					 struct ocm_info * ocm)
{
	struct device_node *pnode;
	int i;

	pnode = NULL;
	ocm->pcount = 0;
	while ((pnode = of_get_next_child(node, pnode)))
		ocm->pcount++;

        if (ocm->pcount == 0) {
		printk(KERN_WARNING
			"OCM%d: no partitions found\n", ocm->index);
		return 0;
	}

        ocm->parts = kzalloc(ocm->pcount * sizeof(struct ocm_partition),
			     GFP_KERNEL);
        if (!ocm->parts) {
		ocm->pcount = 0;
		return -ENOMEM;
	}

	pnode = NULL;
	for (i = 0; i < ocm->pcount; i++) {
		const char *type;
                const u32 *reg;
                int len;

		pnode = of_get_next_child(node, pnode);

                reg = of_get_property(pnode, "reg", &len);
                if (!reg) {
			printk(KERN_WARNING "OCM%d: partition %d invalid\n",
				ocm->index, i);
			continue;
                }

                ocm->parts[i].index = i;
                ocm->parts[i].offset = reg[0];
                ocm->parts[i].memtotal = reg[1];
                ocm->parts[i].memfree = reg[1];

		ocm->parts[i].phys = ocm->phys + ocm->parts[i].offset;

		type = of_get_property(pnode, "type", &len);
		if (type && strcmp(type, "cached") == 0)
			ocm->parts[i].type = OCM_CACHED;
		else if (type && strcmp(type, "ipp") == 0)
			ocm->parts[i].type = OCM_IPP;
		else
			ocm->parts[i].type = OCM_NON_CACHED;

		ocm_map_partition(ocm, i);
        }

	return ocm->pcount;
}


static void __init ocm_init_node(int count, struct device_node *node)
{
	struct ocm_info *ocm;

	const unsigned int *cell_index;
	int len;
	struct resource res;

	ocm = ocm_get_node(count);

	cell_index = of_get_property(node, "cell-index", &len);
	if (!cell_index) {
		printk(KERN_ERR "OCM: missing cell-index property");
		return;
	}
        ocm->index = *cell_index;

	/*
	 * Get access to Memory
	 */

	if (of_address_to_resource(node, 0, &res)) {
		printk(KERN_ERR
			"OCM%d: could not get memory resource address\n",
			ocm->index);
		return;
	}

	ocm->phys = res.start;
	ocm->memtotal = res.end - res.start + 1;

	if (!request_mem_region(ocm->phys, ocm->memtotal, "ocm_mem")) {
		printk(KERN_ERR "OCM%d: could not request memory region\n",
			ocm->index);
		return;
	}

	printk(KERN_INFO "OCM%d: %d Bytes\n", ocm->index, ocm->memtotal);

	/*
	 * Get access to Partitions
	 */

	ocm_init_partitions(node, ocm);

	ocm->ready = 1;

	return;
}

#ifdef CONFIG_APM86xxx_OCM_TEST
struct ocm_test {
	int	size;
	int	align;
	u8	cmp;
	u8	*cache;
	u8	*ncache;
};

static struct ocm_test ocm_test = {256, 4, 0xAD, NULL, NULL};

static void ocm_test_init(void)
{
	phys_addr_t p;

	ocm_test.cache = ocm_alloc(&p, ocm_test.size, ocm_test.align,
				   OCM_CACHED,"ocm_test.cache");
	if (ocm_test.cache)
		memset((void*)ocm_test.cache,ocm_test.cmp,ocm_test.size);

	ocm_test.ncache = ocm_alloc(&p, ocm_test.size, ocm_test.align,
				    OCM_NON_CACHED,"ocm_test.ncache");
	if (ocm_test.ncache)
		memset((void*)ocm_test.ncache,ocm_test.cmp,ocm_test.size);
}

static int ocm_test_memcmp(u8 *mem, u8 c, int size)
{
	int i; 

	if (!mem)
		return 0;

	for (i = 0; i < size; i++) {
		if (mem[i] != c) {
			return -1;
		}
	}

	return 0;
}

static void ocm_test_mem(void)
{
	if (ocm_test_memcmp(ocm_test.cache, ocm_test.cmp, ocm_test.size))
		printk(KERN_ERR "ocm_test: ERROR testing cache memory\n");
	else
		printk(KERN_INFO "ocm_test: cache memory ok\n");

	if (ocm_test_memcmp(ocm_test.ncache, ocm_test.cmp, ocm_test.size))
		printk(KERN_ERR "ocm_test: ERROR testing non-cache memory\n");
	else
		printk(KERN_INFO "ocm_test: non-cache memory ok\n");
}
#else
static inline void ocm_test_init(void) {}
static inline void ocm_test_mem(void) {}
#endif /* CONFIG_APM86xxx_OCM_TEST */

static int ocm_proc_show(struct seq_file *m, void *v)
{
	struct ocm_block *blk, *tmp;
	unsigned long sl_flags;
	unsigned int i, j;

	spin_lock_irqsave(&ocm_lock, sl_flags);

	for (i=0; i < ocm_count; i++) {
		struct ocm_info *ocm = ocm_get_node(i);

		if (!ocm->ready)
			continue;

		seq_printf(m, "OCM       : %d\n", ocm->index);
		seq_printf(m, "PhysAddr  : 0x%llx\n", ocm->phys);
		seq_printf(m, "MemTotal  : %d Bytes\n\n", ocm->memtotal);

		for (j = 0; j < ocm->pcount; j++) {
			struct ocm_partition *part = &ocm->parts[j];

			if (!part->virt)
				continue;

			seq_printf(m, "Partition : %d.%d\n", 
				   ocm->index, part->index);

			seq_printf(m, "Type      : %s\n", ocm_type(part));
			seq_printf(m, "PhysAddr  : 0x%llx\n", part->phys);
			seq_printf(m, "VirtAddr  : 0x%p\n", part->virt);
			seq_printf(m, "MemTotal  : %d Bytes\n", part->memtotal);
			seq_printf(m, "MemFree   : %d Bytes\n", part->memfree);

			if (!part->rh) {
				seq_printf(m, "\n");
				continue;
			} 

			list_for_each_entry_safe(blk,tmp,&part->list,list) {
				seq_printf(m, "MemUsed   : %d Bytes (%s)\n",
					blk->size, blk->owner);
			}

			seq_printf(m, "\n");
		}
	}

	ocm_test_mem();

	spin_unlock_irqrestore(&ocm_lock, sl_flags);

	return 0;
}

static int ocm_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, ocm_proc_show, NULL);
}

static const struct file_operations ocm_proc_fops = {
	.open = ocm_proc_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = single_release,
};

static void __init ocm_proc_init(void)
{
	proc_create("ocminfo", 0, NULL, &ocm_proc_fops);
}

#ifdef CONFIG_PM
int ocm_suspend(struct sys_device *dev, pm_message_t state)
{
	int i, j;

	if (!suspending_to_deepsleep())
		return 0;

	for (i=0; i < ocm_count; i++) {
		struct ocm_info *ocm = ocm_get_node(i);

		for (j = 0; j < ocm->pcount; j++) {
			struct ocm_partition *part = &ocm->parts[j];

			if (part->pm_mem && part->memfree != part->memtotal) {
				memcpy(part->pm_mem, part->virt,
				       part->memtotal);
			}
		}
	}
		
	return 0;
}

int ocm_resume(struct sys_device *dev)
{
	int i, j;

	if (!resumed_from_deepsleep())
		return 0;

	for (i=0; i < ocm_count; i++) {
		struct ocm_info *ocm = ocm_get_node(i);

		for (j = 0; j < ocm->pcount; j++) {
			struct ocm_partition *part = &ocm->parts[j];

			if (part->pm_mem && part->memfree != part->memtotal) {
				memcpy(part->virt, part->pm_mem,
				       part->memtotal);
			}
		}
	}
		
	return 0;
}

static struct sysdev_class ocm_pm_sysclass = {
	.name = "ocm_pm",
	.suspend = ocm_suspend,
	.resume = ocm_resume,
};

static struct sys_device ocm_pm_device = {
	.cls = &ocm_pm_sysclass
};

static int __init ocm_pm_init(void)
{
	int err;

	err = sysdev_class_register(&ocm_pm_sysclass);
	if (err == 0)
		err = sysdev_register(&ocm_pm_device);

	return err;
}
#else
static int __init ocm_pm_init(void) { return 0; }
#endif /* CONFIG_PM */

static int __init ocm_init(void)
{
	struct device_node *np;
	int count;

	spin_lock_init(&ocm_lock);

	count = 0;
	for_each_compatible_node(np, NULL, "apm,ocm") {
		count++;
	}

	if (!count)
		return 0;

	ocm_nodes = kzalloc((count*sizeof(struct ocm_info)), GFP_KERNEL);
	if (!ocm_nodes) {
		printk(KERN_ERR "OCM: failed to allocate OCM nodes!\n");
		return -ENOMEM;
	}

	ocm_count = count;
	count = 0;

	for_each_compatible_node(np, NULL, "apm,ocm") {
		ocm_init_node(count, np);
		count++;
	}
	
	ocm_proc_init();
	ocm_pm_init();
	ocm_test_init();

	return 0;
}

arch_initcall(ocm_init);

