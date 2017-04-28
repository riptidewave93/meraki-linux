/*
 * AppliedMicro USB-OTG Device Driver
 *
 * Copyright 2008 DENX Software Engineering, Stefan Roese <sr@denx.de>
 *
 * Extract the resources (MEM & IRQ) from the dts file and put them
 * into the platform-device struct for usage in the platform-device
 * USB-OTG driver.
 *
 */

#include <linux/platform_device.h>
#include <linux/of_platform.h>
#include <asm/io.h>

/*
 * Resource template will be filled dynamically with the values
 * extracted from the dts file
 */
static struct resource usb_otg_resources[] = {
	[0] = {
		/* 460EX USB-OTG registers */
		.flags  = IORESOURCE_MEM,
	},
	[1] = {
		/* 460EX OTG IRQ */
		.flags  = IORESOURCE_IRQ,
	},
#if 0
	[2] = {
		/* High-Power workaround IRQ */
		.flags  = IORESOURCE_IRQ,
	},
	[3] = {
		/* 460EX DMA IRQ */
		.flags  = IORESOURCE_IRQ,
	},
#endif
};

static u64 dma_mask = 0xffffffffULL;

static struct platform_device usb_otg_device = {
        .name = "dwc_otg",
        .id = 0,
        .num_resources = ARRAY_SIZE(usb_otg_resources),
        .resource = usb_otg_resources,
        .dev = {
                .dma_mask = &dma_mask,
                .coherent_dma_mask = 0xffffffffULL,
        }
};

static struct platform_device *ppro_devs[] __initdata = {
        &usb_otg_device,
};

static int __devinit ppro_usb_otg_probe(struct of_device *ofdev,
					    const struct of_device_id *match)
{
	struct device_node *np = ofdev->node;
	struct resource res;
printk("USB OTG probe!!!\n\n");
	/*
	 * Extract register address reange from device tree and put it into
	 * the platform device structure
	 */
	if (of_address_to_resource(np, 0, &res)) {
		printk(KERN_ERR "%s: Can't get USB-OTG register address\n", __func__);
		return -ENOMEM;
	}
	usb_otg_resources[0].start = res.start;
	usb_otg_resources[0].end = res.end;

	/*
	 * Extract IRQ number(s) from device tree and put them into
	 * the platform device structure
	 */
	usb_otg_resources[1].start = usb_otg_resources[1].end =
		irq_of_parse_and_map(np, 0);
/*	usb_otg_resources[2].start = usb_otg_resources[2].end =
		irq_of_parse_and_map(np, 1);
	usb_otg_resources[3].start = usb_otg_resources[3].end =
		irq_of_parse_and_map(np, 2);
*/
	return platform_add_devices(ppro_devs, ARRAY_SIZE(ppro_devs));
}

static int __devexit ppro_usb_otg_remove(struct of_device *ofdev)
{
	/* Nothing to do here */
	return 0;
}

static const struct of_device_id ppro_usb_otg_match[] = {
	{ .compatible = "amcc,ohci-be", },
	{}
};

static struct of_platform_driver ppro_usb_otg_driver = {
	.name = "ohci-be",
	.match_table = ppro_usb_otg_match,
	.probe = ppro_usb_otg_probe,
	.remove = ppro_usb_otg_remove,
};

static int __init ppro_usb_otg_init(void)
{
	u32 scu_rst_vaddr;
	u32 val;
	printk("USB OTG Init!!!\n\n");
	scu_rst_vaddr = (u32) ioremap_nocache(0x7dd880000, 1024);
	scu_rst_vaddr += 0x0000000c;
	val = in_be32((void __iomem *) scu_rst_vaddr);
	printk("Current SCU_SRST val: %x\n", val);
	val |= 0x06000000;
	out_be32((void __iomem *) scu_rst_vaddr, val);
	val = in_be32((void __iomem *) scu_rst_vaddr);
	printk("Brought USB out of reset: %x\n", val);
	return of_register_platform_driver(&ppro_usb_otg_driver);
}
device_initcall(ppro_usb_otg_init);
