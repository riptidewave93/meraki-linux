/*
 * Meraki MS420 series platform definition file
 *
 * Copyright 2009,2012 Freescale Semiconductor Inc.
 * Copyright 2013 Cisco Systems, Inc.
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
#include <linux/leds.h>
#include <linux/leds-nu801.h>

#include <asm/time.h>
#include <asm/machdep.h>
#include <asm/pci-bridge.h>
#include <mm/mmu_decl.h>
#include <asm/prom.h>
#include <asm/udbg.h>
#include <asm/mpic.h>
#include <asm/qe.h>
#include <asm/qe_ic.h>
#include <asm/fsl_guts.h>
#include <asm/fatboy_cpld.h>

#include <sysdev/fsl_soc.h>
#include <sysdev/fsl_pci.h>
#include "smp.h"

#include "mpc85xx.h"

static struct ccsr_guts __iomem* guts_regs = NULL;

void __init fatboy_pic_init(void)
{
    struct mpic *mpic;
    unsigned long root = of_get_flat_dt_root();

#ifdef CONFIG_QUICC_ENGINE
    struct device_node *np;
#endif

    if (of_flat_dt_is_compatible(root, "fsl,MPC85XXRDB-CAMP")) {
        mpic = mpic_alloc(NULL, 0, MPIC_NO_RESET |
                          MPIC_BIG_ENDIAN |
                          MPIC_SINGLE_DEST_CPU,
                          0, 256, " OpenPIC  ");
    } else {
        mpic = mpic_alloc(NULL, 0,
                          MPIC_BIG_ENDIAN |
                          MPIC_SINGLE_DEST_CPU,
                          0, 256, " OpenPIC  ");
    }

    BUG_ON(mpic == NULL);
    mpic_init(mpic);

#ifdef CONFIG_QUICC_ENGINE
    np = of_find_compatible_node(NULL, NULL, "fsl,qe-ic");
    if (np) {
        qe_ic_init(np, 0, qe_ic_cascade_low_mpic,
                   qe_ic_cascade_high_mpic);
        of_node_put(np);

    } else
        pr_err("%s: Could not find qe-ic node\n", __func__);
#endif

}

static int map_guts(void)
{
    struct device_node *np;

    np = of_find_compatible_node(NULL, NULL, "fsl,p2020-guts");
    if (!np) {
        printk(KERN_ERR "Could not find global-utilities node\n");
        return -EINVAL;
    }

    guts_regs = of_iomap(np, 0);
    of_node_put(np);

    if (!guts_regs) {
        printk(KERN_ERR "Failed to map global-utilities register space\n");
        return -EIO;
    }

    return 0;
};

#if defined(CONFIG_PCI)
/* Host/agent status can be read from porbmsr in the global utilities*/
static int get_pcie_host_agent(void)
{
    u32 host_agent;

    if (guts_regs == NULL)
        return 0;

    host_agent = (in_be32(((void __iomem*)guts_regs) + 4) & 0x00070000) >> 16 ;

    return host_agent;
}

/*
 * To judge if the PCI(e) controller is host/agent mode through
 * the PORBMSR register.
 * 	0: agent mode
 * 	1: host mode
 */
static int is_pcie_host(u32 host_agent, resource_size_t res)
{
    if ((res & 0xfffff) == 0xa000) {
        switch (host_agent) {
        case 0:
        case 1:
        case 4:
        case 5:
            return 0;
        case 2:
        case 3:
        case 6:
        default:
            return 1;
        }
    } else
        return (host_agent % 2) ? 1 : 0;
}
#endif

static void fatboy_restart(char *cmd)
{
    fatboy_cpld_write(FATBOY_CPLD_RESET_CTRL_1, 0xFE);
    mdelay(10);
    /* If we're still here, try resetting just the CPU. */
    fsl_rstcr_restart(cmd);
}


static struct led_nu801_template tricolor_led_template = {
    .name = "tricolor",
    .num_leds = 1,
    .ndelay = 50,
    .init_brightness = {
        LED_OFF,
        LED_OFF,
        LED_OFF,
    },
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

int __init fatboy_device_init(void)
{
    int ret = -EINVAL;
    struct platform_device* device = &tricolor_leds;
    struct device_node* np;
    unsigned count;
    int gpio;

    printk(KERN_INFO "Meraki MS420 platform device init\n");
    np = of_find_compatible_node(NULL, NULL, "numen,nu801");
    if (!np) {
        pr_err("%s: Could not find system LED node\n", __func__);
        goto done;
    }

    count = of_gpio_count(np);
    if (count < 2 || count > 3) {
        pr_err("%s: Invalid number of GPIO options for tricolor LED node\n", __func__);
        goto done;
    }

    gpio = of_get_gpio(np, 0);
    if (gpio < 0) {
        pr_err("%s: Getting CKI GPIO failed for tricolor LED node\n", __func__);
        goto done;
    }
    tricolor_led_template.cki = gpio;

    gpio = of_get_gpio(np, 1);
    if (gpio < 0) {
        pr_err("%s: Getting SDI GPIO failed for tricolor LED node\n", __func__);
        goto done;
    }
    tricolor_led_template.sdi = gpio;

    if (count == 3) {
        gpio = of_get_gpio(np, 2);
        if (gpio < 0) {
            pr_err("%s: Getting LEI GPIO failed for tricolor LED node\n", __func__);
            goto done;
        }
        tricolor_led_template.lei = gpio;
    } else
        tricolor_led_template.lei = -1;

    ret = platform_add_devices(&device, 1);

done:
    if (np)
        of_node_put(np);

    return ret;
}

device_initcall(fatboy_device_init);

/*
 * Setup the architecture
 */
static void __init fatboy_setup_arch(void)
{
#if defined(CONFIG_PCI) || defined(CONFIG_QUICC_ENGINE)
    struct device_node *np;
#endif

    if (ppc_md.progress)
        ppc_md.progress("fatboy_setup_arch()", 0);

    if (map_guts())
        printk(KERN_WARNING "Unable to map Freescale global utilities registers!\n");

    // Disable all secondary functions on multiplexed pins.
    if (guts_regs)
        out_be32(&guts_regs->pmuxcr, 0);

#ifdef CONFIG_PCI
    for_each_node_by_type(np, "pci") {
        if (of_device_is_compatible(np, "fsl,mpc8548-pcie")) {
            struct resource rsrc;
            of_address_to_resource(np, 0, &rsrc);
            if (!is_pcie_host(get_pcie_host_agent(), rsrc.start))
                continue;
            fsl_add_bridge(np, 0);
        }
    }

#endif

    mpc85xx_smp_init();

#ifdef CONFIG_QUICC_ENGINE
    np = of_find_compatible_node(NULL, NULL, "fsl,qe");
    if (!np) {
        pr_err("%s: Could not find Quicc Engine node\n", __func__);
        goto qe_fail;
    }

    qe_reset();
    of_node_put(np);

    np = of_find_node_by_name(NULL, "par_io");
    if (np) {
        struct device_node *ucc;

        par_io_init(np);
        of_node_put(np);

        for_each_node_by_name(ucc, "ucc")
            par_io_of_config(ucc);

    }

qe_fail:
#endif	/* CONFIG_QUICC_ENGINE */

    printk(KERN_INFO "Meraki MS420 series\n");
}

machine_device_initcall(fatboy, mpc85xx_common_publish_devices);
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
#endif
    .get_irq            = mpic_get_irq,
    .restart            = fatboy_restart,
    .calibrate_decr     = generic_calibrate_decr,
    .progress           = udbg_progress,
};

