/*
 * EHCI HCD (Host Controller Driver) for USB.
 *
 * Bus Glue for PPC On-Chip EHCI driver on the of_platform bus
 * Tested on AMCC PPC 440EPx
 *
 * Valentine Barshak <vbarshak@ru.mvista.com>
 *
 * Based on "ehci-ppc-soc.c" by Stefan Roese <sr@denx.de>
 * and "ohci-ppc-of.c" by Sylvain Munaut <tnt@246tNt.com>
 *
 * This file is licenced under the GPL.
 */

#include <linux/signal.h>

#include <linux/of.h>
#include <linux/of_platform.h>

#ifdef CONFIG_APM86xxx 
#include <asm/apm86xxx_soc.h>
#include <asm/apm86xxx_pm.h>
#endif

/* called during probe() after chip reset completes */
static int ehci_ppc_of_setup(struct usb_hcd *hcd)
{
	struct ehci_hcd	*ehci = hcd_to_ehci(hcd);
	int		retval;

	retval = ehci_halt(ehci);
	if (retval)
		return retval;

	retval = ehci_init(hcd);
	if (retval)
		return retval;

	ehci->sbrn = 0x20;
	return ehci_reset(ehci);
}


static const struct hc_driver ehci_ppc_of_hc_driver = {
	.description		= hcd_name,
	.product_desc		= "OF EHCI",
	.hcd_priv_size		= sizeof(struct ehci_hcd),

	/*
	 * generic hardware linkage
	 */
	.irq			= ehci_irq,
	.flags			= HCD_MEMORY | HCD_USB2,

	/*
	 * basic lifecycle operations
	 */
	.reset			= ehci_ppc_of_setup,
	.start			= ehci_run,
	.stop			= ehci_stop,
	.shutdown		= ehci_shutdown,

	/*
	 * managing i/o requests and associated device resources
	 */
	.urb_enqueue		= ehci_urb_enqueue,
	.urb_dequeue		= ehci_urb_dequeue,
	.endpoint_disable	= ehci_endpoint_disable,
	.endpoint_reset		= ehci_endpoint_reset,

	/*
	 * scheduling support
	 */
	.get_frame_number	= ehci_get_frame,

	/*
	 * root hub support
	 */
	.hub_status_data	= ehci_hub_status_data,
	.hub_control		= ehci_hub_control,
#ifdef	CONFIG_PM
	.bus_suspend		= ehci_bus_suspend,
	.bus_resume		= ehci_bus_resume,
#endif
	.relinquish_port	= ehci_relinquish_port,
	.port_handed_over	= ehci_port_handed_over,

	.clear_tt_buffer_complete	= ehci_clear_tt_buffer_complete,
};


/*
 * 440EPx Errata USBH_3
 * Fix: Enable Break Memory Transfer (BMT) in INSNREG3
 */
#define PPC440EPX_EHCI0_INSREG_BMT	(0x1 << 0)
static int __devinit
ppc44x_enable_bmt(struct device_node *dn)
{
	__iomem u32 *insreg_virt;

	insreg_virt = of_iomap(dn, 1);
	if (!insreg_virt)
		return  -EINVAL;

	out_be32(insreg_virt + 3, PPC440EPX_EHCI0_INSREG_BMT);

	iounmap(insreg_virt);
	return 0;
}

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

u64 ioc_mem_paddr(void *addr)
{
	return (unsigned long) addr - (unsigned long) ioc_mem_vaddr_base +
		ioc_mem_phyaddr;
}

void *ioc_mem_alloc(int size)
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

void ioc_mem_free(void *ptr)
{
	unsigned long flags;
	struct list_head * item;
	
	spin_lock_irqsave(&ioc_mem_lock, flags);
	item = (void *) ((unsigned long) ptr - IOC_MEM_HEADER_SIZE);  
	list_add(item, &ioc_mem_head);	
	spin_unlock_irqrestore(&ioc_mem_lock, flags);
}

void *ioc_mem_fix_size(int idx, int size)
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
ehci_hcd_ppc_of_probe(struct of_device *op, const struct of_device_id *match)
{
	struct device_node *dn = op->node;
	struct usb_hcd *hcd;
	struct ehci_hcd	*ehci = NULL;
	struct resource res;
	int irq;
	int rv;
	
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

	dev_dbg(&op->dev, "initializing PPC-OF USB Controller\n");

	rv = of_address_to_resource(dn, 0, &res);
	if (rv)
		return rv;

	hcd = usb_create_hcd(&ehci_ppc_of_hc_driver, &op->dev, "PPC-OF USB");
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

	ehci = hcd_to_ehci(hcd);
	np = of_find_compatible_node(NULL, NULL, "ibm,usb-ohci-440epx");
	if (np != NULL) {
		/* claim we really affected by usb23 erratum */
		if (!of_address_to_resource(np, 0, &res))
			ehci->ohci_hcctrl_reg = ioremap(res.start +
					OHCI_HCCTRL_OFFSET, OHCI_HCCTRL_LEN);
		else
			pr_debug(__FILE__ ": no ohci offset in fdt\n");
		if (!ehci->ohci_hcctrl_reg) {
			pr_debug(__FILE__ ": ioremap for ohci hcctrl failed\n");
		} else {
			ehci->has_amcc_usb23 = 1;
		}
	}

	if (of_get_property(dn, "big-endian", NULL)) {
		ehci->big_endian_mmio = 1;
		ehci->big_endian_desc = 1;
	}
	if (of_get_property(dn, "big-endian-regs", NULL))
		ehci->big_endian_mmio = 1;
	if (of_get_property(dn, "big-endian-desc", NULL))
		ehci->big_endian_desc = 1;

	ehci->caps = hcd->regs;
	ehci->regs = hcd->regs +
			HC_LENGTH(ehci_readl(ehci, &ehci->caps->hc_capbase));

	/* cache this readonly data; minimize chip reads */
	ehci->hcs_params = ehci_readl(ehci, &ehci->caps->hcs_params);

	if (of_device_is_compatible(dn, "ibm,usb-ehci-440epx")) {
		rv = ppc44x_enable_bmt(dn);
		ehci_dbg(ehci, "Break Memory Transfer (BMT) is %senabled!\n",
				rv ? "NOT ": "");
	}

	rv = usb_add_hcd(hcd, irq, 0);
	if (rv)
		goto err_ehci;

	return 0;

err_ehci:
	if (ehci->has_amcc_usb23)
		iounmap(ehci->ohci_hcctrl_reg);
	iounmap(hcd->regs);
err_ioremap:
	irq_dispose_mapping(irq);
err_irq:
	release_mem_region(hcd->rsrc_start, hcd->rsrc_len);
err_rmr:
	usb_put_hcd(hcd);

	return rv;
}


static int ehci_hcd_ppc_of_remove(struct of_device *op)
{
	struct usb_hcd *hcd = dev_get_drvdata(&op->dev);
	struct ehci_hcd *ehci = hcd_to_ehci(hcd);

	struct device_node *np;
	struct resource res;

	dev_set_drvdata(&op->dev, NULL);

	dev_dbg(&op->dev, "stopping PPC-OF USB Controller\n");

	usb_remove_hcd(hcd);

	iounmap(hcd->regs);
	irq_dispose_mapping(hcd->irq);
	release_mem_region(hcd->rsrc_start, hcd->rsrc_len);

	/* use request_mem_region to test if the ohci driver is loaded.  if so
	 * ensure the ohci core is operational.
	 */
	if (ehci->has_amcc_usb23) {
		np = of_find_compatible_node(NULL, NULL, "ibm,usb-ohci-440epx");
		if (np != NULL) {
			if (!of_address_to_resource(np, 0, &res))
				if (!request_mem_region(res.start,
							    0x4, hcd_name))
					set_ohci_hcfs(ehci, 1);
				else
					release_mem_region(res.start, 0x4);
			else
				pr_debug(__FILE__ ": no ohci offset in fdt\n");
			of_node_put(np);
		}

		iounmap(ehci->ohci_hcctrl_reg);
	}
	usb_put_hcd(hcd);

	return 0;
}


static int ehci_hcd_ppc_of_shutdown(struct of_device *op)
{
	struct usb_hcd *hcd = dev_get_drvdata(&op->dev);

	if (hcd->driver->shutdown)
		hcd->driver->shutdown(hcd);

	return 0;
}


#ifdef CONFIG_PM
int ehci_hcd_ppc_of_suspend(struct of_device *op, pm_message_t state)
{
	return 0;
}


int ehci_hcd_ppc_of_resume(struct of_device *op)
{
	struct usb_hcd *hcd = dev_get_drvdata(&op->dev);
	struct ehci_hcd *ehci = hcd_to_ehci(hcd);

#if defined(CONFIG_APM86xxx)
	u32 devid = 0;

	if (!resumed_from_deepsleep())
		return 0;

	if (op->dev.platform_data) {
		devid = *(u32 *)(op->dev.platform_data);
		apm86xxx_ahbc_usb_enable(devid);
	}
#endif

	/* Else reset, to cope with power loss or flush-to-storage
	 * style "resume" having let BIOS kick in during reboot.
	 */
	ehci_halt(ehci);
	ehci_reset(ehci);
	ehci_writel(ehci, ehci->command, &ehci->regs->command);
	ehci_writel(ehci, FLAG_CF, &ehci->regs->configured_flag);
	ehci_readl(ehci, &ehci->regs->command); /* unblock posted writes */
	/* here we "know" root ports should always stay powered */
	ehci_port_power(ehci, 1);

	hcd->state = HC_STATE_SUSPENDED;

	return 0;
}
#endif


static struct of_device_id ehci_hcd_ppc_of_match[] = {
	{
		.compatible = "usb-ehci",
	},
	{},
};
MODULE_DEVICE_TABLE(of, ehci_hcd_ppc_of_match);


static struct of_platform_driver ehci_hcd_ppc_of_driver = {
	.name		= "ppc-of-ehci",
	.match_table	= ehci_hcd_ppc_of_match,
	.probe		= ehci_hcd_ppc_of_probe,
	.remove		= ehci_hcd_ppc_of_remove,
	.shutdown	= ehci_hcd_ppc_of_shutdown,
#ifdef CONFIG_PM
	.suspend	= ehci_hcd_ppc_of_suspend,
	.resume		= ehci_hcd_ppc_of_resume,
#endif
	.driver		= {
		.name	= "ppc-of-ehci",
		.owner	= THIS_MODULE,
	},
};
