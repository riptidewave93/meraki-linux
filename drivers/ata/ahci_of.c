/*
 * ahci_of.c - AHCI SATA OF driver support
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
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/of_platform.h>
#include "ahci.h"

#define DRV_NAME	"ahci"
#define DRV_VERSION	"3.0"

#define AHCI_HFLAGS(flags)	.private_data	= (void *)(flags)

extern int of_ahci_reset_phy(struct ata_host *host);
/* __attribute__((weak)) */
/* int of_ahci_reset_phy(struct ata_host *host) */
/* { */
/* 	/\* Reset and initialize the PHY *\/ */
/* 	return 0; */
/* } */

extern int of_ahci_phy_chk(struct platform_device *pdev);
/* __attribute__((weak)) */
/* int of_ahci_phy_chk(struct platform_device *pdev) */
/* { */
/* 	/\* Check PHY available. Return total PHY count *\/ */
/* 	return 0; */
/* } */

extern int of_ahci_phy_cleanup(struct platform_device *pdev);
/* __attribute__((weak)) */
/* int of_ahci_phy_cleanup(struct platform_device *pdev) */
/* { */
/* 	/\* Clean up PHY as required *\/ */
/* 	return 0; */
/* } */

extern int of_ahci_set_port_ops(struct platform_device *pdev, struct ata_port_info *pip);
/* __attribute__((weak)) */
/* int of_ahci_set_port_ops(struct platform_device *pdev, struct ata_port_info *pip) */
/* { */
/* 	pip->port_ops = &ahci_ops; */

/* 	return 0; */
/* } */

extern struct scsi_host_template *of_ahci_get_ahci_sht(void);
/* static struct scsi_host_template ahci_platform_sht = { */
/* 	AHCI_SHT("ahci_of"), */
/* }; */

/* __attribute__((weak)) */
/* struct scsi_host_template *of_ahci_get_ahci_sht(void) */
/* { */
/* 	return &ahci_platform_sht; */
/* } */

int of_ahci_reset_controller(struct ata_host *host)
{
	int ret;

	if (of_ahci_reset_phy(host)) {
		ret = -EIO;
		goto exit_ahci_of_reset;
	}

	ret = ahci_reset_controller(host);
	if (ret) {
		dev_err(host->dev, "%s: SATA OF error %d\n", __func__, ret);
		goto exit_ahci_of_reset;
	}
	
exit_ahci_of_reset:
	return ret;
}

static int __devinit of_ahci_probe(struct platform_device *pdev)
{
	static int printed_version = 0;
	struct device *dev = &pdev->dev;
	struct scsi_host_template *ahci_sht_ptr = NULL;
	struct ata_port_info pi = {
	/* board_ahci_of */
	/* SDD: What features should be enabled ? */
		AHCI_HFLAGS	(AHCI_HFLAG_YES_NCQ),
		.flags		= AHCI_FLAG_COMMON,
		.pio_mask	= ATA_PIO4,
		.udma_mask	= ATA_UDMA6,
	};
	const struct ata_port_info *ppi[] = { &pi, NULL };
	struct ahci_host_priv *hpriv = NULL;
	struct ata_host *host;
	unsigned int force_port_map = 0;
	unsigned int mask_port_map = 0;
	int n_ports, i, rc = -1;
	void __iomem *mmio = NULL;
	int irq = NO_IRQ;
	u32 num_phy;
	u32 retval;

	WARN_ON((u32) ATA_MAX_QUEUE > (u32) AHCI_MAX_CMDS);

	if (!printed_version++)
		dev_printk(KERN_INFO, dev, "version " DRV_VERSION "\n");

	/* Set base address */
	mmio = of_iomap(pdev->dev.of_node, 0);
	if (!mmio) {
		dev_err(dev, "ioremap failed for SATA register address\n");
		rc = -ENODEV;
		goto error_out;
	}

	num_phy = of_ahci_phy_chk(pdev);
	if (num_phy < 0) {
		dev_err(dev, "SATA PHY initialization failed error\n");
		rc = -ENODEV;
		goto error_out;
	} else if (num_phy > 0) {
		force_port_map = (1 << num_phy) - 1;
	}
	
	/* Get SATA interrupt number */
	irq = irq_of_parse_and_map(pdev->dev.of_node, 0);
	if (irq == NO_IRQ) {
		dev_err(dev, "no SATA DMA irq\n");
		rc = -ENODEV;
		goto error_out;
	}

	hpriv = devm_kzalloc(dev, sizeof(*hpriv), GFP_KERNEL);
	if (!hpriv) {
		rc = -ENOMEM;
		goto error_out;
	}

	retval = of_ahci_set_port_ops(pdev, &pi);
	if (retval != 0) {
		dev_err(dev, "Failed to set SATA port ops\n");
		rc = -ENODEV;
		goto error_out;
	}

	retval = of_ahci_set_port_ops(pdev, &pi);
	if (retval != 0) {
		dev_err(dev, "Failed to set SATA port ops\n");
		rc = -ENODEV;
		goto error_out;
	}

	hpriv->flags |= (unsigned long) pi.private_data;
	hpriv->mmio = mmio;

	/* save initial config */
	ahci_save_initial_config(dev, hpriv, force_port_map, mask_port_map);

	/* prepare host */
	if (hpriv->cap & HOST_CAP_NCQ)
		pi.flags |= ATA_FLAG_NCQ;

	if (hpriv->cap & HOST_CAP_PMP)
		pi.flags |= ATA_FLAG_PMP;

	ahci_set_em_messages(hpriv, &pi);

	/* CAP.NP sometimes indicate the index of the last enabled
	 * port, at other times, that of the last possible port, so
	 * determining the maximum port number requires looking at
	 * both CAP.NP and port_map.
	 */
	n_ports = max(ahci_nr_ports(hpriv->cap), fls(hpriv->port_map));

	host = ata_host_alloc_pinfo(dev, ppi, n_ports);
	if (!host) {
		rc = -ENOMEM;
		goto error_out;
	}

	host->private_data = hpriv;

	if (!(hpriv->cap & HOST_CAP_SSS) || ahci_ignore_sss)
		host->flags |= ATA_HOST_PARALLEL_SCAN;
	else
		printk(KERN_INFO 
			"ahci: SSS flag set, parallel bus scan disabled\n");

	if (pi.flags & ATA_FLAG_EM)
		ahci_reset_em(host);

	for (i = 0; i < host->n_ports; i++) {
		struct ata_port *ap = host->ports[i];

		ata_port_desc(ap, "mmio %pR", mmio);
		ata_port_desc(ap, "port 0x%x", 0x100 + ap->port_no * 0x80);

		/* set initial link pm policy */
		/*		ap->pm_policy = NOT_AVAILABLE;*/

		/* set enclosure management message type */
		if (ap->flags & ATA_FLAG_EM)
			ap->em_message_type = hpriv->em_msg_type;

		/* disabled/not-implemented port */
		if (!(hpriv->port_map & (1 << i)))
			ap->ops = &ata_dummy_port_ops;
	}

	ahci_sht_ptr = of_ahci_get_ahci_sht();
	if (!ahci_sht_ptr)
		goto error_out;

	rc = of_ahci_reset_controller(host);
	if (rc)
		goto error_out;

	ahci_init_controller(host);
	ahci_print_info(host, "of");

	rc = ata_host_activate(host, irq, ahci_interrupt, IRQF_SHARED, 
				ahci_sht_ptr);
	if (rc)
		goto error_out;

	return 0;

error_out:
	if (mmio)
		iounmap(mmio);

	if (hpriv)
		kfree(hpriv);

	return rc;
}

static int __devexit of_ahci_remove(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct ata_host *host = dev_get_drvdata(dev);
	struct ahci_host_priv *hpriv = host->private_data;

	of_ahci_phy_cleanup(pdev);

	ata_host_detach(host);
	if (hpriv) {
		if (hpriv->mmio)
			iounmap(hpriv->mmio);
		kfree(hpriv);
	}

	return 0;
}

/***************** OF PLATFORM STUFF ******************************************/

static const struct of_device_id of_ahci_id_table[] = {
	{ .compatible = "apm,sata-460ex", },
	{ .compatible = "apm,sata-405ex", },
	{ .compatible = "apm,dwc-sata", },
	{ .compatible = "apm,sata-ahci", },
	{ .compatible = "sata-ahci", },
	{}
};
MODULE_DEVICE_TABLE(of, of_ahci_id_table);

static struct platform_driver of_ahci_driver = {
	.probe = of_ahci_probe,
	.remove = __devexit_p(of_ahci_remove),
	.driver = {
		.name = "ahci-sata-of",
		.of_match_table = of_ahci_id_table,
	},
};

static int __init of_ahci_init(void)
{
	return platform_driver_register(&of_ahci_driver);
}

static void __exit of_ahci_exit(void)
{
	platform_driver_unregister(&of_ahci_driver);
}

/******************************************************************************/

MODULE_AUTHOR("Ravi Patel <rapatel@apm.com>");
MODULE_DESCRIPTION("AHCI SATA OF driver");
MODULE_LICENSE("GPL");
MODULE_VERSION(DRV_VERSION);

module_init(of_ahci_init);
module_exit(of_ahci_exit);
