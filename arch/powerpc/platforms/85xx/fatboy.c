/*
 * Meraki MS420 series platform definition file
 *
 * Copyright 2009,2012 Freescale Semiconductor Inc.
 * Copyright 2013,2016 Cisco Systems, Inc.
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
#include <linux/of_gpio.h>
#include <linux/fsl/guts.h>
#include <linux/pstore_ram.h>
#include <linux/memblock.h>

#include <asm/time.h>
#include <asm/machdep.h>
#include <asm/pci-bridge.h>
#include <mm/mmu_decl.h>
#include <asm/prom.h>
#include <asm/udbg.h>
#include <asm/mpic.h>
#include <asm/fatboy_cpld.h>

#include <sysdev/fsl_soc.h>
#include <sysdev/fsl_pci.h>
#include "smp.h"

#include "mpc85xx.h"

#define FATBOY_RAMOOPS_PHYS_ADDR 0x7FFF0000
#define FATBOY_RAMOOPS_SIZE 0x10000
#define FATBOY_RAMOOPS_RECORD_SIZE (FATBOY_RAMOOPS_SIZE/2) // total size / number of CPUS

void __init fatboy_pic_init(void)
{
    struct mpic *mpic;

    mpic = mpic_alloc(NULL, 0,
                      MPIC_BIG_ENDIAN |
                      MPIC_SINGLE_DEST_CPU,
                      0, 256, " OpenPIC  ");

    BUG_ON(mpic == NULL);
    mpic_init(mpic);
}

static void fatboy_restart(char *cmd)
{
    fatboy_cpld_write(FATBOY_CPLD_RESET_CTRL_1, 0xFE);
    mdelay(10);
    /* If we're still here, try resetting just the CPU. */
    fsl_rstcr_restart(cmd);
}

static struct ramoops_platform_data ramoops_data = {
    .mem_size = FATBOY_RAMOOPS_SIZE,
    .mem_address = FATBOY_RAMOOPS_PHYS_ADDR,
    .record_size = FATBOY_RAMOOPS_RECORD_SIZE,
    .dump_oops = 1,
};

static struct platform_device ramoops_dev = {
    .name = "ramoops",
    .dev = {
        .platform_data = &ramoops_data,
    },
};


int __init fatboy_device_init(void)
{
    printk(KERN_INFO "MS420 platform device initialization\n");
    if (platform_device_register(&ramoops_dev) < 0)
        printk(KERN_WARNING "WARNING: Unable to initialize ramoops device");

    return 0;
}

device_initcall(fatboy_device_init);
/*
 * Setup the architecture
 */
static void __init fatboy_setup_arch(void)
{
    struct device_node *np;
    struct ccsr_guts __iomem* guts_regs = NULL;

    if (ppc_md.progress)
        ppc_md.progress("fatboy_setup_arch()", 0);

    // Reserve ramoops region
    if (memblock_reserve(FATBOY_RAMOOPS_PHYS_ADDR, FATBOY_RAMOOPS_SIZE) < 0)
        printk(KERN_WARNING "WARNING: Unable to reserve ramoops area.\n");

    // Default the PCIE bus configuration to performance mode.  This
    // will force the MPS (max payload size) and MRRS (max read
    // request size) to the maximum the bus can support.  We can do
    // this because all the PCIe devices that will ever be used on
    // this system are hardwired to the bus, so we don't have to worry
    // about random unknown PCIe devices showing up.
    pcie_bus_config = PCIE_BUS_PERFORMANCE;

    np = of_find_compatible_node(NULL, NULL, "fsl,p2020-guts");
    if (!np) {
        printk(KERN_WARNING "Could not find global-utilities node\n");
    } else {
        guts_regs = of_iomap(np, 0);
        of_node_put(np);

        if (!guts_regs) {
            printk(KERN_WARNING "Failed to map global-utilities register space\n");
        } else {
            // Disable all secondary functions on multiplexed pins.
            out_be32(&guts_regs->pmuxcr, 0);
        }
        iounmap(guts_regs);
    }

    swiotlb_detect_4g();
    fsl_pci_assign_primary();
    mpc85xx_smp_init();

    printk(KERN_INFO "Meraki MS420 series\n");
}

machine_arch_initcall(fatboy, mpc85xx_common_publish_devices);
/*
 * Called very early, device-tree isn't unflattened
 */
static int __init fatboy_probe(void)
{
    unsigned long root = of_get_flat_dt_root();

    if (of_flat_dt_is_compatible(root, "meraki,fatboy"))
        return 1;
    return 0;
}

define_machine(fatboy) {
    .name               = "Meraki MS420 series",
    .probe              = fatboy_probe,
    .setup_arch         = fatboy_setup_arch,
    .init_IRQ           = fatboy_pic_init,
#ifdef CONFIG_PCI
    .pcibios_fixup_bus  = fsl_pcibios_fixup_bus,
    .pcibios_fixup_phb  = fsl_pcibios_fixup_phb,
#endif
    .get_irq            = mpic_get_irq,
    .restart            = fatboy_restart,
    .calibrate_decr     = generic_calibrate_decr,
    .progress           = udbg_progress,
};

