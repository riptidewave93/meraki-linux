/*
 * Copyright (C) 2009 Applied Micro Circuits corporation.
 *
 * Author: Feng Kan <fkan@amcc.com>
 *        Tirumala Marri <tmarri@amcc.com>
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 2 of the
 * License.
 */
#include <linux/irq.h>
#include <linux/bootmem.h>
#include <linux/pci.h>
#include <linux/msi.h>
#include <linux/of_platform.h>
#include <linux/interrupt.h>
#include <linux/device.h>
#include <asm/prom.h>
#include <asm/hw_irq.h>
#include <asm/ppc-pci.h>
#include <asm/dcr.h>
#include <asm/dcr-regs.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/delay.h>
#include "ppc4xx_msi.h"

#define MSI_DEBUG	0

#define MSI_PROC_ROOT       "driver/msi"
#define MSI_PROC_NAME       "term"


void __iomem *msi_virt;
static struct ppc4xx_msi *ppc4xx_msi_data;

/* Can use this register address for catching the MSI message */
#if defined(CONFIG_460EX) || defined (CONFIG_APM82181)

#ifdef MSI_DEBUG

#define MSI_TERM_ADDR 0xc0ec70000ull
#else
#define MSI_TERM_ADDR 0xc10000080ull 
#endif /*MSI_DEBUG*/
#endif /*defined(CONFIG_460EX) || defined (CONFIG_APM82181) */

static int ppc4xx_msi_init_allocator(struct ppc4xx_msi *msi_data)
{
	int rc;

	rc = msi_bitmap_alloc(&msi_data->bitmap, NR_MSI_IRQS,
		msi_data->irqhost->of_node);
	if (rc)
		return rc;
	rc = msi_bitmap_reserve_dt_hwirqs(&msi_data->bitmap);
	if (rc < 0) {
		msi_bitmap_free(&msi_data->bitmap);
		return rc;
	}
	return 0;
}


static void ppc4xx_msi_cascade(unsigned int irq, struct irq_desc *desc)
{
	unsigned int cascade_irq;
	struct ppc4xx_msi *msi_data = ppc4xx_msi_data;
	int msir_index = -1;
#ifdef MSI_DEBUG
	printk("PCIE-MSI: cascade %s called irq %d %d\n", 
				__func__, irq, (int)virq_to_hw(irq));
#endif
	spin_lock(&desc->lock);
	if (desc->chip->mask_ack) {
		desc->chip->mask_ack(irq);
	} else {
		desc->chip->mask(irq);
		desc->chip->ack(irq);
	}

	if (unlikely(desc->status & IRQ_INPROGRESS))
		goto unlock;

	msir_index = (int)desc->handler_data;

	if (msir_index >= NR_MSI_IRQS)
		cascade_irq = NO_IRQ;

	desc->status |= IRQ_INPROGRESS;

	cascade_irq = irq_linear_revmap(msi_data->irqhost, msir_index);
	if (cascade_irq != NO_IRQ)
		generic_handle_irq(cascade_irq);
	else
                printk("%s: no cacade handler found\n", __FUNCTION__);
	desc->status &= ~IRQ_INPROGRESS;

	if (!(desc->status & IRQ_DISABLED) && desc->chip->unmask)
		desc->chip->unmask(irq);
unlock:
	spin_unlock(&desc->lock);
}

static void ppc4xx_compose_msi_msg(struct pci_dev *pdev, int hwirq,
	struct msi_msg *msg)
{
	struct ppc4xx_msi *msi_data = ppc4xx_msi_data;

	msg->address_lo = msi_data->msi_addr_lo;
	msg->address_hi = msi_data->msi_addr_hi;
	msg->data = hwirq | MSI_DATA_PATTERN;
}


int ppc4xx_setup_msi_irqs(struct pci_dev *dev, int nvec, int type)
{
	struct msi_desc *entry;
	int rc, hwirq;
	unsigned int virq;
	struct msi_msg msg;
	struct ppc4xx_msi *msi_data = ppc4xx_msi_data;

	printk("PCIE-MSI: vendor %x\n", dev->vendor);
	list_for_each_entry(entry, &dev->msi_list, list) {
		hwirq = msi_bitmap_alloc_hwirqs(&msi_data->bitmap, 1);
		if (hwirq < 0) {
			rc = hwirq;
			pr_debug("%s: fail allocating msi interrupt\n",
				__func__);
			goto out_free;
		}
		virq = irq_create_mapping(msi_data->irqhost, hwirq);
		printk("PCIE-MSI: hwirq requested %d, virt %d\n", hwirq, virq);
		if (virq == NO_IRQ) {
			pr_debug("%s: fail mapping irq\n", __func__);
			rc = -ENOSPC;
			goto out_free;
		}
		set_irq_msi(virq, entry);
		ppc4xx_compose_msi_msg(dev, hwirq, &msg);
		printk("PCIE-MSI: message: h %08x l %08x data %08x\n", 
			msg.address_hi, msg.address_lo, msg.data);

		write_msi_msg(virq, &msg);
	}

	return 0;
out_free:
	return rc;
}

void ppc4xx_teardown_msi_irqs(struct pci_dev *dev)
{
	struct msi_desc *entry;
	struct ppc4xx_msi *msi_data = ppc4xx_msi_data;

	printk("PCIE-MSI: tearing down msi irqs for %p\n", dev);

	list_for_each_entry(entry, &dev->msi_list, list) {
		if (entry->irq == NO_IRQ)
			continue;
		set_irq_msi(entry->irq, NULL);
		msi_bitmap_free_hwirqs(&msi_data->bitmap,
					virq_to_hw(entry->irq), 1);
		irq_dispose_mapping(entry->irq);

	}

	return;
}

static int ppc4xx_msi_check_device(struct pci_dev *pdev, int nvec, int type)
{
	printk("PCIE-MSI:%s called. vec %x type %d\n",
		__func__, nvec, type);
       return 0;
}

/* Stub, unneeded. */
static void ppc4xx_msi_end_irq(unsigned int virq)
{
}


static struct irq_chip ppc4xx_msi_chip = {
	.mask	= mask_msi_irq,
	.unmask	= unmask_msi_irq,
	.ack	= ppc4xx_msi_end_irq,
	.name	= " UIC",
};

static int ppc4xx_msi_host_map(struct irq_host *h, unsigned int virq,
                               irq_hw_number_t hw)
{
	struct irq_chip *chip = &ppc4xx_msi_chip;

	irq_to_desc(virq)->status |= IRQ_TYPE_EDGE_RISING;

	set_irq_chip_and_handler(virq, chip, handle_edge_irq);

	return 0;
}

static struct irq_host_ops ppc4xx_msi_host_ops = {
	.map = ppc4xx_msi_host_map,
};


static int __devinit ppc4xx_msi_probe(struct of_device *dev,
                                       const struct of_device_id *match)
{
	struct ppc4xx_msi *msi;
	struct resource res, rmsi;
	int i;
	int rc;
	int virt_msir = 0;
	const u32 *sdr_base;
	dma_addr_t msi_phys = 0;

	printk("PCIE-MSI: Setting up MSI ...\n");
	msi = kzalloc(sizeof(struct ppc4xx_msi), GFP_KERNEL);
	if (!msi) {
		dev_err(&dev->dev, "No memory for MSI structure\n");
		rc = -ENOMEM;
		goto error_out;
	}

	/* TODO: parse device tree interrupts early to get dynamic IRQ count */
	msi->irqhost = irq_alloc_host(dev->node, IRQ_HOST_MAP_LINEAR,
		NR_MSI_IRQS, &ppc4xx_msi_host_ops, 0);
	if (msi->irqhost == NULL) {
		dev_err(&dev->dev, "No memory for MSI irqhost\n");
		rc = -ENOMEM;
		goto error_out;
	}

	/* Get MSI ranges */
	rc = of_address_to_resource(dev->node, 0, &rmsi);
	if (rc) {
		printk("%s rmsi resource error!\n",
			dev->node->full_name);
		goto error_out;
	}

	/* Get the MSI reg base */
	rc = of_address_to_resource(dev->node, 1, &res);
	printk("PCIE-MSI: MSI reg base %llx - %llx\n", res.start, res.end);
	if (rc) {
		dev_err(&dev->dev, "%s regbase resource error!\n",
			dev->node->full_name);
		goto error_out;
	}
	/* Get the sdr-base */
	sdr_base = (u32 *)of_get_property(dev->node, "sdr-base", NULL);
	printk("PCIE-MSI: MSI sdr base %08x\n", *sdr_base);
	if (sdr_base == NULL) {
		pr_debug("%s: %s - no sdr-base property found!\n", __func__,
			dev->node->full_name);
		goto error_out;
	}
	msi->sdr_base = *sdr_base;
	mtdcri(SDR0, msi->sdr_base + PE_IHS1_OFF, RES_TO_U32_HIGH(res.start));
	mtdcri(SDR0, msi->sdr_base + PE_IHS2_OFF, RES_TO_U32_LOW(res.start)|
						PE_IHS2_BREV_EN);

	printk("PCIE-MSI: PE_IHS h %08x l %08x\n", 
			mfdcri(SDR0, msi->sdr_base + PE_IHS1_OFF),
			mfdcri(SDR0, msi->sdr_base + PE_IHS2_OFF));
	
	msi->msi_regs = ioremap(res.start, res.end - res.start + 1);
        if (!msi->msi_regs) {
		pr_debug("%s: PIH mapping failed\n", __func__);
		goto error_out;
	}
#if defined(CONFIG_405EX) ||  defined(CONFIG_440SPe) || defined(CONFIG_460SX)
	/* PIMs setup for first 2GB */
	msi_virt = dma_alloc_coherent(&dev->dev, 64, &msi_phys, GFP_KERNEL);
	if (msi_virt == NULL) {
		pr_debug("%s: No memory for MSI trigger\n", __func__);
		rc = -ENOMEM;
		goto error_out;
	}
	msi->msi_addr_hi = 0;
	msi->msi_addr_lo = (u32)msi_phys;

#elif defined(CONFIG_460EX) || defined (CONFIG_APM82181)
	msi->msi_addr_hi = (u32)(MSI_TERM_ADDR>>32);
	msi->msi_addr_lo = (u32)(MSI_TERM_ADDR & 0xffffffff);
#ifdef MSI_DEBUG
        msi_virt =  ioremap(MSI_TERM_ADDR, 0x20);
        if(!msi_virt) {
                printk("ERROR: Ioremap failed  %llx\n", MSI_TERM_ADDR);
		goto error_out;
        }
#endif /*MSI_DEBUG*/

#endif
	/* Progam the Interrupt handler Termination addr registers */
	out_be32(msi->msi_regs + PEIH_TERMADH, msi->msi_addr_hi);
	out_be32(msi->msi_regs + PEIH_TERMADL, msi->msi_addr_lo);

	/* Program MSI Expected data and Mask bits */
	out_be32(msi->msi_regs + PEIH_MSIED, MSI_DATA_PATTERN);
	out_be32(msi->msi_regs + PEIH_MSIMK, MSI_MASK_PATTERN);

	iounmap(msi->msi_regs);

	msi->irqhost->host_data = msi;

	if (ppc4xx_msi_init_allocator(msi)) {
		pr_debug("%s: Error allocating MSI bitmap\n", __func__);
		goto error_out;
	}

	for (i = 0; i < NR_MSI_IRQS; i++) {
		virt_msir = irq_of_parse_and_map(dev->node, i);
		if (virt_msir != NO_IRQ) {
			set_irq_data(virt_msir, (void *)i);
			set_irq_chained_handler(virt_msir, ppc4xx_msi_cascade);
		} else {
			printk("No IRQ!\n");
		}
	}

	ppc4xx_msi_data = msi;

	WARN_ON(ppc_md.setup_msi_irqs);
	ppc_md.setup_msi_irqs = ppc4xx_setup_msi_irqs;
	ppc_md.teardown_msi_irqs = ppc4xx_teardown_msi_irqs;
	ppc_md.msi_check_device = ppc4xx_msi_check_device;
	return 0;
error_out:
	if (msi_virt)
		dma_free_coherent(&dev->dev, 64, msi_virt, msi_phys);
	if (msi)
			kfree(msi);
	return rc;
}

int msi_proc_read (char *page, char **start, off_t off,
        int count, int *eof, void *data)
{
        printk("PCIE-MSI: Read from Term Address\n");
#ifdef MSI_DEBUG
	printk("message %08x\n", in_le32(msi_virt));

        out_le32(msi_virt, 0x0);
#endif
        return 0;
}

int msi_proc_write (struct file *file, const char __user *buffer,
        unsigned long count, void *data)
{
        char tmp[2];
        unsigned long val;

	/* Verify MSIx from user space by echo "x" > /proc/4xx_msi/test */
        if (copy_from_user(tmp, buffer, count))
                return -EFAULT;
        val = simple_strtoul(tmp, NULL, 16);

	printk("PCIE-MSI: Testing interrupt:%d\n", (int)val);

#ifdef MSI_DEBUG
	printk("Write to term address with data=0x%08x\n",(u32)(MSI_DATA_PATTERN | val));
	out_le32(msi_virt, MSI_DATA_PATTERN | val);
        mdelay(10);
#endif
        return count;


}

static const struct of_device_id ppc4xx_msi_ids[] = {
       {
               .compatible = "amcc,ppc4xx-msi",
       },
       {}
};

static struct of_platform_driver ppc4xx_msi_driver = {
       .name = "ppc4xx-msi",
       .match_table = ppc4xx_msi_ids,
       .probe = ppc4xx_msi_probe,
};

static __init int ppc4xx_msi_init(void)
{
	struct proc_dir_entry *p ,*root=NULL;
	
	root = proc_mkdir(MSI_PROC_ROOT, NULL);
        if (!root) {
                printk("%s: failed to create proc entry\n",__func__);
                goto err;
        }

        p = create_proc_entry(MSI_PROC_NAME, 0, root);
        if (p) {
              p->read_proc = msi_proc_read;
              p->write_proc = msi_proc_write;
        }
err:
	return of_register_platform_driver(&ppc4xx_msi_driver);
	
}
subsys_initcall(ppc4xx_msi_init);
