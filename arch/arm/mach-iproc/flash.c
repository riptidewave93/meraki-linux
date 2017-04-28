/*
 * Copyright (C) 2013, Broadcom Corporation. All Rights Reserved.
 * 
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <linux/init.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/ioport.h>
#include <linux/spi/spi.h>
#include <linux/spi/flash.h>

#include <mach/io_map.h>
#include <mach/reg_utils.h>
#include <mach/qspi_iproc.h>
#include <mach/nand_iproc.h>
#include <mach/memory.h>

#include <asm/pgtable.h>

#ifdef CONFIG_MTD

#include <linux/mtd/mtd.h>
#include <linux/mtd/physmap.h>
#include <linux/mtd/map.h>

/* Since GSIO uses bus number 0, QSPI uses bus number 1 */
#define IPROC_QSPI_BUS_NUMBER       (1)

/* Currently NAND controller only supports 2 LUNs */
#define IPROC_NAND_MAX_LUNS         (2)

#if defined(CONFIG_IPROC_QSPI) || defined(CONFIG_IPROC_QSPI_MODULE)
static struct mtd_partition sflash_partition_map[] = {
    {
        .name = "boot",
        .offset = 0x00000000,
        .size = 640 * 1024,
    },
    {
        .name = "env",
        .offset = MTDPART_OFS_APPEND,
        .size = 384 * 1024,
    },
    {
        .name = "system",
        .offset = MTDPART_OFS_APPEND,
        .size = 15 * 1024 * 1024,
    },
    {
        .name = "rootfs",
        .offset = MTDPART_OFS_APPEND,
        .size = MTDPART_SIZ_FULL,
    },
};
#endif /* CONFIG_IPROC_QSPI || CONFIG_IPROC_QSPI_MODULE */

#if defined(CONFIG_MTD_NAND_IPROC) || defined(CONFIG_MTD_NAND_IPROC_MODULE)
static struct mtd_partition nand_partition_map[] = {
    {
        .name = "nboot",
        .offset = 0x00000000,
        .size = 2 * 1024 * 1024,
    },
    {
        .name = "nenv",
        .offset = MTDPART_OFS_APPEND,
        .size = 4 * 1024 * 1024,
    },
    {
        .name = "nsystem",
        .offset = MTDPART_OFS_APPEND,
        .size = 10 * 1024 * 1024,
    },
    {
        .name = "nrootfs",
        .offset = MTDPART_OFS_APPEND,
        .size = 48 * 1024 * 1024,
    },
    {
        .name = "ncustfs",
        .offset = MTDPART_OFS_APPEND,
        .size = MTDPART_SIZ_FULL,
    },
};
#endif /* CONFIG_MTD_NAND_IPROC || CONFIG_MTD_NAND_IPROC_MODULE */

#if defined(CONFIG_IPROC_QSPI) || defined(CONFIG_IPROC_QSPI_MODULE)
static int __init 
brcm_setup_spi_master(int cs, int bus_id)
{
	struct brcmspi_platform_data pdata;
	struct platform_device *pdev;
	const struct resource res[] = {
        {
            .start = IPROC_QSPI_IRQ_START,
            .end = IPROC_QSPI_IRQ_END,
            .flags = IORESOURCE_IRQ
        },
        {
            .start = QSPI_MSPI_SPCR0_LSB,
            .end = QSPI_MSPI_DISABLE_FLUSH_GEN + 3,
            .flags = IORESOURCE_MEM
        },
        {
            .start = QSPI_BSPI_REGS_REV_ID,
            .end = QSPI_BSPI_REGS_BSPI_PIO_DATA + 3,
            .flags = IORESOURCE_MEM
        },
        {
            .start = QSPI_RAF_START_ADDR,
            .end = QSPI_RAF_CURR_ADDR + 3,
            .flags = IORESOURCE_MEM
        },
        {
            .start = QSPI_RAF_INTERRUPT_LR_FULLNESS_REACHED,
            .end = QSPI_MSPI_INTERRUPT_MSPI_HALT_SET_TRANSACTION_DONE + 3,
            .flags = IORESOURCE_MEM
        },
        {
            .start = IPROC_IDM_QSPI_REG_BASE,
            .end = IPROC_IDM_QSPI_REG_BASE + 3,
            .flags = IORESOURCE_MEM
        },
        {
            .start = IPROC_CRU_REG_BASE,
            .end = IPROC_CRU_REG_BASE + 3,
            .flags = IORESOURCE_MEM
        },
    };
    
	memset(&pdata, 0, sizeof(pdata));
	pdata.flash_cs = cs;
	pdev = platform_device_alloc("qspi_iproc", bus_id);
	if (!pdev ||
	    platform_device_add_resources(pdev, res, sizeof(res)/sizeof(res[0])) ||
	    platform_device_add_data(pdev, &pdata, sizeof(pdata)) ||
	    platform_device_add(pdev)) {
		platform_device_put(pdev);
		return -ENODEV;
	}
	return 0;
}

static int __init 
brcm_setup_spi_flash(int cs, int bus_num, int nr_parts,	struct mtd_partition *parts)
{
	struct spi_board_info board_info;
	struct flash_platform_data *pdata;
	struct spi_master *master;
    
	pdata = kzalloc(sizeof(*pdata), GFP_KERNEL);
	if (!pdata)
		return -ENOMEM;

	pdata->nr_parts = nr_parts;
	pdata->parts = parts;

	memset(&board_info, 0, sizeof(board_info));

	strcpy(board_info.modalias, "m25p80");
	board_info.bus_num = bus_num;
	board_info.chip_select = cs;
	board_info.max_speed_hz = CONFIG_IPROC_QSPI_MAX_HZ;
	board_info.mode = SPI_MODE_3;
	board_info.platform_data = pdata;
    
	master = spi_busnum_to_master(bus_num);
	if (master) {
		/* Master driver already loaded */
		if (spi_new_device(master, &board_info) == NULL) {
			printk(KERN_WARNING "%s: can't add SPI device\n", __func__);
			kfree(pdata);
			return -ENODEV;
		}
	} else {
		/* Master driver not yet loaded, register the board first. */
		if (spi_register_board_info(&board_info, 1) != 0) {
			printk(KERN_WARNING "%s: can't register SPI device\n", __func__);
			kfree(pdata);
			return -ENODEV;
		}
	}

	return 0;
}
#endif /* CONFIG_IPROC_QSPI || CONFIG_IPROC_QSPI_MODULE */

#if defined(CONFIG_MTD_NAND_IPROC) || defined(CONFIG_MTD_NAND_IPROC_MODULE)
static void __init 
northstar_setup_nand_flash(void)
{
	u32 straps;
    struct platform_device *pdev;
    struct brcmnand_platform_data pdata;
    int i;
    
	straps = __REG32(IPROC_DMU_BASE_VA + IPROC_DMU_STRAPS_OFFSET);
    pdata.strap_boot = ((straps >> IPROC_STRAP_BOOT_DEV_SHIFT) & 3) == 1;
    pdata.strap_type = (straps >> IPROC_STRAP_NAND_TYPE_SHIFT) & 0xf;
    pdata.strap_page_size = (straps >> IPROC_STRAP_NAND_PAGE_SHIFT) & 0x3;
    if (!pdata.strap_boot) {
        pdata.strap_type &= 0x7;
    }
    pdata.nr_parts = ARRAY_SIZE(nand_partition_map);
    pdata.parts = nand_partition_map;
    
    for(i=0; i<IPROC_NAND_MAX_LUNS; i++) {
        
        /* Use the full device if it's not the first LUN */
        if (i > 0) {
            pdata.nr_parts = 0;
            pdata.parts = NULL;
        }
        
        pdata.chip_select = i;
        pdev = platform_device_alloc("nand_iproc", i);
        if (!pdev ||
            platform_device_add_data(pdev, &pdata, sizeof(pdata)) ||
            platform_device_add(pdev)) {
            platform_device_put(pdev);
        }
    }
}
#endif /* CONFIG_MTD_NAND_IPROC || CONFIG_MTD_NAND_IPROC_MODULE */

static int __init 
northstar_mtd_setup(void)
{
#if defined(CONFIG_IPROC_QSPI) || defined(CONFIG_IPROC_QSPI_MODULE)
    /* SPI flash (currently used for primary) */
    brcm_setup_spi_master(
        0, 
        IPROC_QSPI_BUS_NUMBER
        );
    brcm_setup_spi_flash(
        0, 
        IPROC_QSPI_BUS_NUMBER,
        ARRAY_SIZE(sflash_partition_map),
        sflash_partition_map
        );
#endif /* CONFIG_IPROC_QSPI || CONFIG_IPROC_QSPI_MODULE */
        
#if defined(CONFIG_MTD_NAND_IPROC) || defined(CONFIG_MTD_NAND_IPROC_MODULE)
#ifdef CONFIG_MACH_NS
    /* Don't bring up NAND driver if it's BCM53010 */
    if ((__REG32(IPROC_IDM_REGISTER_VA + 0xd500) & 0xc) != 0x4)
#endif /* CONFIG_MACH_NS */
        northstar_setup_nand_flash();
#endif /* CONFIG_MTD_NAND_IPROC || CONFIG_MTD_NAND_IPROC_MODULE */

    return 0;
}

/*
 * late_initcall means the flash drivers are already loaded, so we control
 * the order in which the /dev/mtd* devices get created.
 */
late_initcall(northstar_mtd_setup);

#endif /* CONFIG_MTD */
