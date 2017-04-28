/*
 * linux/arch/arm/plat-omap/common.c
 *
 * Code common to all OMAP machines.
 * The file is created by Tony Lindgren <tony@atomide.com>
 *
 * Copyright (C) 2009 Texas Instruments
 * Added OMAP4 support - Santosh Shilimkar <santosh.shilimkar@ti.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/omapfb.h>
#include <linux/memblock.h>

#include <asm/mach/map.h>

#include <plat/common.h>
#include <plat/board.h>
#include <plat/vram.h>
#include <plat/dsp.h>


#include <plat/ti81xx_ram.h>

#define NO_LENGTH_CHECK 0xffffffff

struct omap_board_config_kernel *omap_board_config;
int omap_board_config_size;

static const void *get_config(u16 tag, size_t len, int skip, size_t *len_out)
{
	struct omap_board_config_kernel *kinfo = NULL;
	int i;

	/* Try to find the config from the board-specific structures
	 * in the kernel. */
	for (i = 0; i < omap_board_config_size; i++) {
		if (omap_board_config[i].tag == tag) {
			if (skip == 0) {
				kinfo = &omap_board_config[i];
				break;
			} else {
				skip--;
			}
		}
	}
	if (kinfo == NULL)
		return NULL;
	return kinfo->data;
}

const void *__omap_get_config(u16 tag, size_t len, int nr)
{
        return get_config(tag, len, nr, NULL);
}
EXPORT_SYMBOL(__omap_get_config);

const void *omap_get_var_config(u16 tag, size_t *len)
{
        return get_config(tag, NO_LENGTH_CHECK, 0, len);
}
EXPORT_SYMBOL(omap_get_var_config);

void  __init ti81xx_reserve(void)
{
	/* Reserve 1MB for strongly ordered mapping for dram barrier */
	extern phys_addr_t dram_sync_phys;
	u32 size = ALIGN(PAGE_SIZE, SZ_1M);

	dram_sync_phys = memblock_alloc(size, SZ_1M);

	if (!dram_sync_phys) {
		pr_err("%s: ### failed to reserve ddr\n", __func__);
	} else {
		pr_info("%s: ### Reserved DDR region @%p\n", __func__,
				(void *)dram_sync_phys);
	}

	memblock_free(dram_sync_phys, size);
	memblock_remove(dram_sync_phys, size);

	ti81xxfb_reserve_sdram_memblock();
	ti81xx_pcie_mem_reserve_sdram_memblock();
}

void __init omap_reserve(void)
{
	omapfb_reserve_sdram_memblock();
	omap_vram_reserve_sdram_memblock();
	omap_dsp_reserve_sdram_memblock();
}
