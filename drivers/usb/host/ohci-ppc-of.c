/*
 * OHCI HCD (Host Controller Driver) for USB.
 *
 * (C) Copyright 1999 Roman Weissgaerber <weissg@vienna.at>
 * (C) Copyright 2000-2002 David Brownell <dbrownell@users.sourceforge.net>
 * (C) Copyright 2002 Hewlett-Packard Company
 * (C) Copyright 2006 Sylvain Munaut <tnt@246tNt.com>
 *
 * Bus glue for OHCI HC on the of_platform bus
 *
 * Modified for of_platform bus from ohci-sa1111.c
 *
 * This file is licenced under the GPL.
 */

#include <linux/signal.h>
#include <linux/of_platform.h>

#include <asm/prom.h>

#ifdef CONFIG_APM86xxx 
#include <asm/apm86xxx_soc.h>
#include <asm/apm86xxx_pm.h>
#endif

static int __devinit
ohci_ppc_of_start(struct usb_hcd *hcd)
{
	struct ohci_hcd	*ohci = hcd_to_ohci(hcd);
	int		ret;

	if ((ret = ohci_init(ohci)) < 0)
		return ret;

	if ((ret = ohci_run(ohci)) < 0) {
		err("can't start %s", ohci_to_hcd(ohci)->self.bus_name);
		ohci_stop(hcd);
		return ret;
	}

	return 0;
}

static const struct hc_driver ohci_ppc_of_hc_driver = {
	.description =		hcd_name,
	.product_desc =		"OF OHCI",
	.hcd_priv_size =	sizeof(struct ohci_hcd),

	/*
	 * generic hardware linkage
	 */
	.irq =			ohci_irq,
	.flags =		HCD_USB11 | HCD_MEMORY,

	/*
	 * basic lifecycle operations
	 */
	.start =		ohci_ppc_of_start,
	.stop =			ohci_stop,
	.shutdown = 		ohci_shutdown,

	/*
	 * managing i/o requests and associated device resources
	 */
	.urb_enqueue =		ohci_urb_enqueue,
	.urb_dequeue =		ohci_urb_dequeue,
	.endpoint_disable =	ohci_endpoint_disable,

	/*
	 * scheduling support
	 */
	.get_frame_number =	ohci_get_frame,

	/*
	 * root hub support
	 */
	.hub_status_data =	ohci_hub_status_data,
	.hub_control =		ohci_hub_control,
#ifdef	CONFIG_PM
	.bus_suspend =		ohci_bus_suspend,
	.bus_resume =		ohci_bus_resume,
#endif
	.start_port_reset =	ohci_start_port_reset,
};

#if defined(CONFIG_APM86xxx_IOCOHERENT)
#include <linux/spinlock_types.h>
static spinlock_t ioc_mem_lock;
static struct list_head ioc_mem_head;
static u64 ioc_mem_phyaddr;
static int ioc_mem_size;		 
static void *ioc_mem_vaddr_base;
static void *ioc_mem_vaddr;
#define IOC_MEM_1ST_CHUNK_SIZE		(64*1024)
#define IOC_MEM_CHUNK_SIZE		128	/* Must be multiple of 32B */
#define IOC_MEM_HEADER_SIZE		32	/* Must be multiple of 32B */

static int ioc_mem_init(void)
{
	struct list_head * item;
	u8 *ptr;
	u32 i;
	
	spin_lock_init(&ioc_mem_lock);
	INIT_LIST_HEAD(&ioc_mem_head);
	ioc_mem_vaddr_base = ioremap_nocache(ioc_mem_phyaddr, ioc_mem_size);
	if (ioc_mem_vaddr_base == NULL) {
		printk(KERN_ERR "unable to map IO coherent memory\n");
		return -ENOMEM;
	}
	ioc_mem_vaddr = (u8 *) ioc_mem_vaddr_base + IOC_MEM_1ST_CHUNK_SIZE;
	
	ptr = ioc_mem_vaddr;
	for (i = 0; i < ioc_mem_size - IOC_MEM_1ST_CHUNK_SIZE; 
	     i += IOC_MEM_CHUNK_SIZE) {
		item = (struct list_head *) ptr;
		list_add(item, &ioc_mem_head);
		ptr += IOC_MEM_CHUNK_SIZE;
	}
	return 0;
}

u64 ioc2_mem_paddr(void *addr)
{
	return (unsigned long) addr - (unsigned long) ioc_mem_vaddr_base +
		ioc_mem_phyaddr;
}

void *ioc2_mem_alloc(int size)
{
	unsigned long flags;
	struct list_head * item;
	
	if (size > (IOC_MEM_CHUNK_SIZE - IOC_MEM_HEADER_SIZE)) {
		printk(KERN_ERR "unable to allocate coherent memory size %d\n",
			size);
		return NULL;
	}	
	spin_lock_irqsave(&ioc_mem_lock, flags);
	if (list_empty(&ioc_mem_head)) {
		spin_unlock_irqrestore(&ioc_mem_lock, flags);
		return NULL;
	}	
	item = ioc_mem_head.next;	
	list_del(item);			
	spin_unlock_irqrestore(&ioc_mem_lock, flags);
	return (u8 *) item + IOC_MEM_HEADER_SIZE; 
}

void ioc2_mem_free(void *ptr)
{
	unsigned long flags;
	struct list_head * item;
	
	spin_lock_irqsave(&ioc_mem_lock, flags);
	item = (void *) ((unsigned long) ptr - IOC_MEM_HEADER_SIZE);  
	list_add(item, &ioc_mem_head);	
	spin_unlock_irqrestore(&ioc_mem_lock, flags);
}

void *ioc2_mem_fix_size(int idx, int size)
{
	if (size > IOC_MEM_1ST_CHUNK_SIZE / 2) {
		printk(KERN_ERR "unable to allocate coherent memory size %d\n",
			size);
		return NULL;
	}
	if (idx == 0) 
		return ioc_mem_vaddr_base;
	if (idx == 1)
		return (u8 *) ioc_mem_vaddr_base + IOC_MEM_1ST_CHUNK_SIZE / 2;
	return NULL;		
}
#endif

static int __devinit
ohci_hcd_ppc_of_probe(struct of_device *op, const struct of_device_id *match)
{
	struct device_node *dn = op->node;
	struct usb_hcd *hcd;
	struct ohci_hcd	*ohci;
	struct resource res;
	int irq;

	int rv;
	int is_bigendian;
	struct device_node *np;

#ifdef CONFIG_APM86xxx
	u32 *devid = 0;
#if defined(CONFIG_APM86xxx_IOCOHERENT)
	u32 *val;
	u32 val_len;
#endif

	rv = -ENODEV;
	devid = (u32 *)of_get_property (dn, "devid", NULL);
	op->dev.platform_data = (void *)devid;

	if (devid && (*devid == 1 || *devid == 2)) 
		rv = apm86xxx_usb_clk_enable(*devid);

	if (rv)
		return rv;

#if defined(CONFIG_APM86xxx_IOCOHERENT)
	val = (u32 *) of_get_property(dn, "dma-reg", &val_len);
	if (val == NULL || val_len < 3) {
		printk(KERN_ERR "No DTS dma-reg for IO coherent mode\n");
		return -ENODEV;
	}		
	ioc_mem_phyaddr = ((u64) val[0] << 32) | val[1];
	ioc_mem_size = val[2];	
	ioc_mem_init(); 
	dev_info(&op->dev, 
		"coherent memory PADDR 0x%010llX VADDR 0x%p size %d",
		ioc_mem_phyaddr, ioc_mem_vaddr, ioc_mem_size);
#endif
#endif

	if (usb_disabled())
		return -ENODEV;

	is_bigendian =
		of_device_is_compatible(dn, "ohci-bigendian") ||
		of_device_is_compatible(dn, "ohci-be");

	dev_dbg(&op->dev, "initializing PPC-OF USB Controller\n");

	rv = of_address_to_resource(dn, 0, &res);
	if (rv)
		return rv;

	hcd = usb_create_hcd(&ohci_ppc_of_hc_driver, &op->dev, "PPC-OF USB");
	if (!hcd)
		return -ENOMEM;

	hcd->rsrc_start = res.start;
	hcd->rsrc_len = res.end - res.start + 1;

	if (!request_mem_region(hcd->rsrc_start, hcd->rsrc_len, hcd_name)) {
		printk(KERN_ERR __FILE__ ": request_mem_region failed\n");
		rv = -EBUSY;
		goto err_rmr;
	}

	irq = irq_of_parse_and_map(dn, 0);
	if (irq == NO_IRQ) {
		printk(KERN_ERR __FILE__ ": irq_of_parse_and_map failed\n");
		rv = -EBUSY;
		goto err_irq;
	}

	hcd->regs = ioremap(hcd->rsrc_start, hcd->rsrc_len);
	if (!hcd->regs) {
		printk(KERN_ERR __FILE__ ": ioremap failed\n");
		rv = -ENOMEM;
		goto err_ioremap;
	}

	ohci = hcd_to_ohci(hcd);
	if (is_bigendian) {
		ohci->flags |= OHCI_QUIRK_BE_MMIO | OHCI_QUIRK_BE_DESC;
		if (of_device_is_compatible(dn, "fsl,mpc5200-ohci"))
			ohci->flags |= OHCI_QUIRK_FRAME_NO;
		if (of_device_is_compatible(dn, "mpc5200-ohci"))
			ohci->flags |= OHCI_QUIRK_FRAME_NO;
	}

	ohci_hcd_init(ohci);

	rv = usb_add_hcd(hcd, irq, IRQF_DISABLED);
	if (rv == 0)
		return 0;

	/* by now, 440epx is known to show usb_23 erratum */
	np = of_find_compatible_node(NULL, NULL, "ibm,usb-ehci-440epx");

	/* Work around - At this point ohci_run has executed, the
	* controller is running, everything, the root ports, etc., is
	* set up.  If the ehci driver is loaded, put the ohci core in
	* the suspended state.  The ehci driver will bring it out of
	* suspended state when / if a non-high speed USB device is
	* attached to the USB Host port.  If the ehci driver is not
	* loaded, do nothing. request_mem_region is used to test if
	* the ehci driver is loaded.
	*/
	if (np !=  NULL) {
		if (!of_address_to_resource(np, 0, &res)) {
			if (!request_mem_region(res.start, 0x4, hcd_name)) {
				writel_be((readl_be(&ohci->regs->control) |
					OHCI_USB_SUSPEND), &ohci->regs->control);
					(void) readl_be(&ohci->regs->control);
			} else
				release_mem_region(res.start, 0x4);
		} else
		    pr_debug(__FILE__ ": cannot get ehci offset from fdt\n");
	}

	iounmap(hcd->regs);
err_ioremap:
	irq_dispose_mapping(irq);
err_irq:
	release_mem_region(hcd->rsrc_start, hcd->rsrc_len);
err_rmr:
 	usb_put_hcd(hcd);

	return rv;
}

static int ohci_hcd_ppc_of_remove(struct of_device *op)
{
	struct usb_hcd *hcd = dev_get_drvdata(&op->dev);
	dev_set_drvdata(&op->dev, NULL);

	dev_dbg(&op->dev, "stopping PPC-OF USB Controller\n");

	usb_remove_hcd(hcd);

	iounmap(hcd->regs);
	irq_dispose_mapping(hcd->irq);
	release_mem_region(hcd->rsrc_start, hcd->rsrc_len);

	usb_put_hcd(hcd);

	return 0;
}

static int ohci_hcd_ppc_of_shutdown(struct of_device *op)
{
	struct usb_hcd *hcd = dev_get_drvdata(&op->dev);

        if (hcd->driver->shutdown)
                hcd->driver->shutdown(hcd);

	return 0;
}

#ifdef CONFIG_PM
static int ohci_hcd_ppc_of_suspend(struct of_device *op, pm_message_t state)
{
	return 0;
}

static int ohci_hcd_ppc_of_resume(struct of_device *op)
{
	struct usb_hcd  *hcd = dev_get_drvdata(&op->dev);
	struct ohci_hcd *ohci = hcd_to_ohci(hcd);

#if defined(CONFIG_APM86xxx)
	u32 devid = 0;

	if (!resumed_from_deepsleep())
		return 0;

	if (op->dev.platform_data) {
		devid = *(u32 *)(op->dev.platform_data);
		apm86xxx_ahbc_usb_enable(devid);
	}
#endif

	ohci_writel(ohci, OHCI_USB_SUSPEND, &ohci->regs->control);
	ohci->hc_control = ohci_readl(ohci, &ohci->regs->control);
	/* wake on ConnectStatusChange, matching external hubs */
	ohci_writel (ohci, RH_HS_DRWE, &ohci->regs->roothub.status);
	ohci_writel (ohci, roothub_a (ohci) | RH_A_NPS, &ohci->regs->roothub.a);

	return 0;
}
#endif

static struct of_device_id ohci_hcd_ppc_of_match[] = {
#ifdef CONFIG_USB_OHCI_HCD_PPC_OF_BE
	{
		.name = "usb",
		.compatible = "ohci-bigendian",
	},
	{
		.name = "usb",
		.compatible = "ohci-be",
	},
#endif
#ifdef CONFIG_USB_OHCI_HCD_PPC_OF_LE
	{
		.name = "usb",
		.compatible = "ohci-littledian",
	},
	{
		.name = "usb",
		.compatible = "ohci-le",
	},
#endif
	{},
};
MODULE_DEVICE_TABLE(of, ohci_hcd_ppc_of_match);

#if	!defined(CONFIG_USB_OHCI_HCD_PPC_OF_BE) && \
	!defined(CONFIG_USB_OHCI_HCD_PPC_OF_LE)
#error "No endianess selected for ppc-of-ohci"
#endif


static struct of_platform_driver ohci_hcd_ppc_of_driver = {
	.name		= "ppc-of-ohci",
	.match_table	= ohci_hcd_ppc_of_match,
	.probe		= ohci_hcd_ppc_of_probe,
	.remove		= ohci_hcd_ppc_of_remove,
	.shutdown 	= ohci_hcd_ppc_of_shutdown,
#ifdef CONFIG_PM
	.suspend	= ohci_hcd_ppc_of_suspend,
	.resume		= ohci_hcd_ppc_of_resume,
#endif
	.driver		= {
		.name	= "ppc-of-ohci",
		.owner	= THIS_MODULE,
	},
};

