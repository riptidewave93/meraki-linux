/*
 * Meraki Topaz setup file
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#include <linux/stddef.h>
#include <linux/kernel.h>
#include <linux/pci.h>
#include <linux/kdev_t.h>
#include <linux/delay.h>
#include <linux/seq_file.h>
#include <linux/interrupt.h>
#include <linux/of_platform.h>
#include <linux/lmb.h>

#include <asm/system.h>
#include <asm/time.h>
#include <asm/machdep.h>
#include <asm/pci-bridge.h>
#include <mm/mmu_decl.h>
#include <asm/prom.h>
#include <asm/udbg.h>
#include <asm/mpic.h>
#include <asm/swiotlb.h>

#include <sysdev/fsl_soc.h>
#include <sysdev/fsl_pci.h>

#include <linux/platform_device.h>
#include <linux/leds.h>
#include <linux/leds-nu801.h>

static struct led_nu801_template tricolor_led_template = {
    .name = "tricolor",
    .num_leds = 1,
    .cki = 224 + 19,
    .sdi = 224 + 17,
    .lei = 224 + 20,
    .ndelay = 50,
    .init_brightness = {
        LED_OFF,
        LED_OFF,
        LED_OFF,
    }
};

static struct led_nu801_platform_data tricolor_led_data = {
    .num_controllers = 1,
    .template = &tricolor_led_template,
};

static struct platform_device tricolor_leds = {
    .name = "leds-nu801",
    .id = -1,
    .dev.platform_data = &tricolor_led_data,
};

static struct platform_device *topaz_platform_devices[] __initdata = {
    &tricolor_leds,

};

int __init topaz_init(void)
{
    printk("Meraki Topaz Init\n");
    return platform_add_devices(topaz_platform_devices, ARRAY_SIZE(topaz_platform_devices));
}

device_initcall(topaz_init);


void __init topaz_pic_init(void)
{
	struct mpic *mpic;
	struct resource r;
	struct device_node *np;

	np = of_find_node_by_type(NULL, "open-pic");
	if (np == NULL) {
		printk(KERN_ERR "Could not find open-pic node\n");
		return;
	}

	if (of_address_to_resource(np, 0, &r)) {
		printk(KERN_ERR "Failed to map mpic register space\n");
		of_node_put(np);
		return;
	}

	mpic = mpic_alloc(np, r.start,
			  MPIC_PRIMARY | MPIC_WANTS_RESET |
			  MPIC_BIG_ENDIAN | MPIC_BROKEN_FRR_NIRQS |
			  MPIC_SINGLE_DEST_CPU,
			0, 256, " OpenPIC  ");
	BUG_ON(mpic == NULL);
	of_node_put(np);

	mpic_init(mpic);
}

/*
 * Setup the architecture
 */
#ifdef CONFIG_SMP
extern void __init mpc85xx_smp_init(void);
#endif
static void __init topaz_setup_arch(void)
{
#ifdef CONFIG_PCI
	struct device_node *np;
	struct pci_controller *hose;
#endif
	dma_addr_t max = 0xffffffff;

	if (ppc_md.progress)
		ppc_md.progress("topaz_setup_arch()", 0);

#ifdef CONFIG_PCI
	for_each_node_by_type(np, "pci") {
		if (of_device_is_compatible(np, "fsl,mpc8540-pci") ||
		    of_device_is_compatible(np, "fsl,mpc8548-pcie") ||
		    of_device_is_compatible(np, "fsl,p1022-pcie")) {
			struct resource rsrc;
			of_address_to_resource(np, 0, &rsrc);
			if ((rsrc.start & 0xfffff) == 0x8000)
				fsl_add_bridge(np, 1);
			else
				fsl_add_bridge(np, 0);

			hose = pci_find_hose_for_OF_device(np);
			max = min(max, hose->dma_window_base_cur +
					hose->dma_window_size);
		}
	}

#endif

#ifdef CONFIG_SMP
	mpc85xx_smp_init();
#endif

#ifdef CONFIG_SWIOTLB
	if (lmb_end_of_DRAM() > max) {
		ppc_swiotlb_enable = 1;
		set_pci_dma_ops(&swiotlb_dma_ops);
		ppc_md.pci_dma_dev_setup = pci_dma_dev_setup_swiotlb;
	}
#endif

	printk("Meraki Topaz\n");
}

static struct of_device_id __initdata topaz_ids[] = {
	{ .type = "soc", },
	{ .compatible = "soc", },
	{ .compatible = "simple-bus", },
	{ .compatible = "gianfar", },
	{},
};

static int __init topaz_publish_devices(void)
{
	return of_platform_bus_probe(NULL, topaz_ids, NULL);
}
machine_device_initcall(topaz, topaz_publish_devices);

machine_arch_initcall(topaz, swiotlb_setup_bus_notifier);

/*
 * Called very early, device-tree isn't unflattened
 */
static int __init topaz_probe(void)
{
	unsigned long root = of_get_flat_dt_root();

	return of_flat_dt_is_compatible(root, "fsl,p1022ds");
}

define_machine(topaz) {
	.name			= "Topaz",
	.probe			= topaz_probe,
	.setup_arch		= topaz_setup_arch,
	.init_IRQ		= topaz_pic_init,
#ifdef CONFIG_PCI
	.pcibios_fixup_bus	= fsl_pcibios_fixup_bus,
#endif
	.get_irq		= mpic_get_irq,
	.restart		= fsl_rstcr_restart,
	.calibrate_decr		= generic_calibrate_decr,
	.progress		= udbg_progress,
};
