/*
 * Copyright (C) 2010, Applied Micro Circuit Corporation
 *
 * Author: Feng Kan <fkan@apm.com>
 * Based on work from Tony Li and Jason Jin
 *
 * The hwirq alloc and free code reuse from sysdev/mpic_msi.c
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 2 of the
 * License.
 *
 */
#include <linux/irq.h>
#include <linux/bootmem.h>
#include <linux/msi.h>
#include <linux/pci.h>
#include <linux/of_platform.h>
#include <asm/prom.h>
#include <asm/hw_irq.h>
#include <asm/ppc-pci.h>
#include "apm_msi.h"

#ifdef MSI_DEBUG
#define msi_dbg(fmt, ...) printk(fmt, ##__VA_ARGS__);
#else
#define msi_dbg(fmt, ...)
#endif

struct apm_msi_feature {
	u32 apm_pic_ip;
	u32 msiir_offset;
};

static struct apm_msi *apm_msi;

static inline u32 apm_msi_read(u32 __iomem *base, unsigned int reg)
{
	return in_be32(base + (reg >> 2));
}

/*
 * We do not need this actually. The MSIR register has been read once
 * in the cascade interrupt. So, this MSI interrupt has been acked
*/
static void apm_msi_end_irq(unsigned int virq)
{
}

static struct irq_chip apm_msi_chip = {
	.mask		= mask_msi_irq,
	.unmask		= unmask_msi_irq,
	.ack		= apm_msi_end_irq,
	.typename	= "APM-MSI",
};

static int apm_msi_host_map(struct irq_host *h, unsigned int virq,
				irq_hw_number_t hw)
{
	struct irq_chip *chip = &apm_msi_chip;

	get_irq_desc(virq)->status |= IRQ_TYPE_EDGE_FALLING;

	set_irq_chip_and_handler(virq, chip, handle_edge_irq);

	return 0;
}

static struct irq_host_ops apm_msi_host_ops = {
	.map = apm_msi_host_map,
};

static int apm_msi_init_allocator(struct apm_msi *msi_data)
{
	int rc;

	msi_dbg("MSI: initialising allocators\n");
	rc = msi_bitmap_alloc(&msi_data->bitmap, msi_data->num_irqs_per_reg,
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

static int apm_msi_check_device(struct pci_dev *pdev, int nvec, int type)
{
	if (type == PCI_CAP_ID_MSIX)
		pr_debug("apmmsi: MSI-X untested, trying anyway.\n");

	return 0;
}

static void apm_teardown_msi_irqs(struct pci_dev *pdev)
{
	struct msi_desc *entry;
	struct apm_msi *msi_data = apm_msi;

	list_for_each_entry(entry, &pdev->msi_list, list) {
		if (entry->irq == NO_IRQ)
			continue;
		set_irq_msi(entry->irq, NULL);
		msi_bitmap_free_hwirqs(&msi_data->bitmap,
				       virq_to_hw(entry->irq), 1);
		irq_dispose_mapping(entry->irq);
	}

	return;
}

static void apm_compose_msi_msg(struct pci_dev *pdev, int hwirq,
				  struct msi_msg *msg)
{
	struct apm_msi *msi_data = apm_msi;

	msg->address_lo = msi_data->msi_addr_lo;
	msg->address_hi = msi_data->msi_addr_hi;

	/*
	 * This is tricky, normally this is written with big endian
	 * and send out as little endian, we have to do a double
	 * reversal here inorder to get it back correctly.
	 */
	msg->data = (((hwirq / 32) + 1) << 5) | ((hwirq % 32) & 0x1f);

	msi_dbg("%s: allocated msi irq %d srs: %x, \n",
		__func__, hwirq, msg->data);
}

static int apm_setup_msi_irqs_linear(struct pci_dev *pdev, int nvec, int type)
{
	int rc;
	unsigned int virq;
	struct msi_desc *entry;
	struct msi_msg msg;
	struct apm_msi *msi_data = apm_msi;

	list_for_each_entry(entry, &pdev->msi_list, list) {
		virq = irq_create_direct_mapping(msi_data->irqhost);

		msi_dbg("Created mapping for virq %d\n", virq);

		if (virq == NO_IRQ) {
			pr_debug("%s: fail mapping virq 0x%x\n",
					__func__, virq);
			rc = -ENOSPC;
			goto out_free;
		}
		set_irq_msi(virq, entry);

		apm_compose_msi_msg(pdev, virq, &msg);
		write_msi_msg(virq, &msg);
	}
	return 0;

out_free:
	return rc;
}

static int apm_setup_msi_irqs_cascade(struct pci_dev *pdev, int nvec, int type)
{
	int rc, hwirq;
	unsigned int virq;
	struct msi_desc *entry;
	struct msi_msg msg;
	struct apm_msi *msi_data = apm_msi;

	list_for_each_entry(entry, &pdev->msi_list, list) {
		hwirq = msi_bitmap_alloc_hwirqs(&msi_data->bitmap, 1);
		if (hwirq < 0) {
			rc = hwirq;
			pr_debug("%s: fail allocating msi interrupt\n",
					__func__);
			goto out_free;
		}

		virq = irq_create_mapping(msi_data->irqhost, hwirq);
		msi_dbg("Cascade: Created mapping for virq %d with hwirq %d\n", virq, hwirq);
		if (virq == NO_IRQ) {
			pr_debug("%s: fail mapping hwirq 0x%x\n",
					__func__, hwirq);
			msi_bitmap_free_hwirqs(&msi_data->bitmap, hwirq, 1);
			rc = -ENOSPC;
			goto out_free;
		}
		set_irq_msi(virq, entry);

		apm_compose_msi_msg(pdev, hwirq, &msg);
		write_msi_msg(virq, &msg);
	}
	return 0;

out_free:
	return rc;
}

static int apm_setup_msi_irqs(struct pci_dev *pdev, int nvec, int type)
{
	if (apm_msi->feature == APM_IP_MPIC_CASCADE)
		apm_setup_msi_irqs_cascade(pdev, nvec, type);
	else
		apm_setup_msi_irqs_linear(pdev, nvec, type);
	
	return 0;
}

static void apm_msi_cascade(unsigned int irq, struct irq_desc *desc)
{
	unsigned int cascade_irq;
	struct apm_msi *msi_data = apm_msi;
	int msir_index = -1;
	u32 msir_value = 0;
	u32 intr_index;

	spin_lock(&desc->lock);
	if ((msi_data->feature &  APM_IP_MASK) == APM_IP_UIC) {
		if (desc->chip->mask_ack)
			desc->chip->mask_ack(irq);
		else {
			desc->chip->mask(irq);
			desc->chip->ack(irq);
		}
	}

	if (unlikely(desc->status & IRQ_INPROGRESS))
		goto unlock;

	msir_index = (int)desc->handler_data;

	cascade_irq = NO_IRQ;

	desc->status |= IRQ_INPROGRESS;
	switch (apm_msi->feature & APM_IP_MASK) {
	case APM_IP_MPIC_CASCADE:
		msir_value = apm_msi_read(msi_data->msi_regs,
			msir_index * 0x10 + 0x100);
		break;
	case APM_IP_UIC:
		msir_value = apm_msi_read(msi_data->msi_regs, msir_index * 0x4);
		break;
	}

	while (msir_value) {
		u32 bitpos;
		intr_index = fls(msir_value) - 1;
		bitpos = 31 - intr_index;

		cascade_irq = irq_linear_revmap(msi_data->irqhost,
				msir_index * msi_data->num_irqs_per_reg +
					bitpos);
		if (cascade_irq != NO_IRQ)
			generic_handle_irq(cascade_irq);
		msir_value = msir_value & ~(1 << intr_index);
	}
	desc->status &= ~IRQ_INPROGRESS;

	switch (msi_data->feature & APM_IP_MASK) {
	case APM_IP_MPIC:
	case APM_IP_MPIC_CASCADE:
		desc->chip->eoi(irq);
		break;
	case APM_IP_UIC:
		if (!(desc->status & IRQ_DISABLED) && desc->chip->unmask)
			desc->chip->unmask(irq);
		break;
	}
unlock:
	spin_unlock(&desc->lock);
}

static int setup_cascade_msi(struct of_device *dev)
{
	int rc, i, virt_msir;


	msi_dbg("Alloc %d interrupts on new irq host\n", apm_msi->total_irqs);

	apm_msi->irqhost = irq_alloc_host(dev->node, IRQ_HOST_MAP_LINEAR,
				      apm_msi->total_irqs, &apm_msi_host_ops, 0);

	apm_msi->irqhost->host_data = apm_msi;

	if (apm_msi->irqhost == NULL) {
		dev_err(&dev->dev, "No memory for MSI irqhost\n");
		goto error_out;
	}

	msi_dbg("Allocating initial msi map\n");
	rc = apm_msi_init_allocator(apm_msi);
	if (rc) {
		dev_err(&dev->dev, "Error allocating MSI bitmap\n");
		goto error_out;
	}

	for (i = 0; i < apm_msi->total_irqs/apm_msi->num_irqs_per_reg; i++) {
		virt_msir = irq_of_parse_and_map(dev->node, i);
		msi_dbg("mapped virt irq %d\n", virt_msir);
		if (virt_msir != NO_IRQ) {
			msi_dbg("%d: set %d cascade irq data\n", virt_msir, i);
			set_irq_data(virt_msir, (void *)i);
			set_irq_chained_handler(virt_msir, apm_msi_cascade);
		}
	}
	return 0;
error_out:
	return -1;
}

static int setup_linear_msi(struct of_device *dev)
{
	int i, virt_msir;

	for (i = 0; i < apm_msi->total_irqs; i++) {
		virt_msir = irq_of_parse_and_map(dev->node, i);
		msi_dbg("mapped virt irq %d\n", virt_msir);
		if (virt_msir != NO_IRQ) {
			set_irq_data(virt_msir, (void *)i);
		}
	}
	return 0;
}

static int __devinit apm_of_msi_probe(struct of_device *dev,
				const struct of_device_id *match)
{
	struct apm_msi *msi;
	struct resource res;
	int err, count=0;
	const u32 *p;
	struct apm_msi_feature *features = match->data;

	msi_dbg("Setup APM MSI support\n");

	msi = kzalloc(sizeof(struct apm_msi), GFP_KERNEL);
	if (!msi) {
		dev_err(&dev->dev, "No memory for MSI structure\n");
		err = -ENOMEM;
		goto error_out;
	}

	p = of_get_property(dev->node, "interrupts", &count);
	if (!p) {
		dev_err(&dev->dev, "no interrupts property found on %s\n",
				dev->node->full_name);
		err = -ENODEV;
		goto error_out;
	}
	count /= sizeof(u32);
	count /= 2;

	if (features->apm_pic_ip == APM_IP_MPIC_CASCADE) {
		p = of_get_property(dev->node, "num_irqs_per_reg", NULL);
		if (!p) {
			dev_err(&dev->dev, "%s: num_irqs_per_reg property not found\n",
				dev->node->full_name);
			err = -ENODEV;
			goto error_out;
		}
		msi->num_irqs_per_reg = p[0];
		msi->total_irqs = count * p[0];
	} else {
		msi->num_irqs_per_reg = 1;
		msi->total_irqs = count;
	}
	
	msi_dbg("%s: no of MSI irqs found is %d\n", dev->node->full_name, msi->total_irqs);
	

	/* Get the MSI reg base */
	err = of_address_to_resource(dev->node, 0, &res);
	if (err) {
		dev_err(&dev->dev, "%s resource error!\n",
				dev->node->full_name);
		goto error_out;
	}

	msi->msi_regs = ioremap(res.start, res.end - res.start + 1);
	if (!msi->msi_regs) {
		dev_err(&dev->dev, "ioremap problem failed\n");
		goto error_out;
	}

	msi->feature = features->apm_pic_ip;


	msi->msi_addr_hi = res.start >> 32;
	msi->msi_addr_lo = features->msiir_offset + (res.start & 0xffffffff);

	apm_msi = msi;

	if (msi->feature == APM_IP_MPIC_CASCADE)
		setup_cascade_msi(dev);
	else
		setup_linear_msi(dev);

	WARN_ON(ppc_md.setup_msi_irqs);
	ppc_md.setup_msi_irqs = apm_setup_msi_irqs;
	ppc_md.teardown_msi_irqs = apm_teardown_msi_irqs;
	ppc_md.msi_check_device = apm_msi_check_device;
	return 0;
error_out:
	kfree(msi);
	return err;
}

static const struct apm_msi_feature mpic_msi_feature = {
	.apm_pic_ip = APM_IP_MPIC,
	.msiir_offset = 0x0000,
};

static const struct apm_msi_feature mpic_msi_cascade_feature = {
	.apm_pic_ip = APM_IP_MPIC_CASCADE,
	.msiir_offset = 0x0000,
};

static const struct apm_msi_feature uic_msi_feature = {
	.apm_pic_ip = APM_IP_UIC,
	.msiir_offset = 0x00, 
};

static const struct of_device_id apm_of_msi_ids[] = {
	{
		.compatible = "apm,mpic-msi",
		.data = (void *)&mpic_msi_feature,
	},
	{
		.compatible = "apm,mpic-msi-cascade",
		.data = (void *)&mpic_msi_cascade_feature,
	},
	{
		.compatible = "apm,uic-msi",
		.data = (void *)&uic_msi_feature,
	},
	{}
};

static struct of_platform_driver apm_of_msi_driver = {
	.name = "apm-msi",
	.match_table = apm_of_msi_ids,
	.probe = apm_of_msi_probe,
};

static __init int apm_of_msi_init(void)
{
	return of_register_platform_driver(&apm_of_msi_driver);
}

subsys_initcall(apm_of_msi_init);
