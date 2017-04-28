/*
 * arch/arm/plat-ambarella/generic/init.c
 *
 * Author: Anthony Ginger <hfjiang@ambarella.com>
 *
 * Copyright (C) 2004-2009, Ambarella, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/dma-mapping.h>
#include <linux/proc_fs.h>
#include <linux/memblock.h>
#include <linux/export.h>
#include <linux/clk.h>

#include <asm/cacheflush.h>
#include <asm/system_info.h>

#include <mach/hardware.h>
#include <mach/board.h>
#include <mach/init.h>

#include <plat/bapi.h>
#include <plat/clk.h>

/* ==========================================================================*/
u64 ambarella_dmamask = DMA_BIT_MASK(32);
EXPORT_SYMBOL(ambarella_dmamask);

/* ==========================================================================*/
static struct proc_dir_entry *ambarella_proc_dir = NULL;

int __init ambarella_create_proc_dir(void)
{
	int					retval = 0;

	ambarella_proc_dir = proc_mkdir("ambarella", NULL);
	if (!ambarella_proc_dir)
		retval = -ENOMEM;

	return retval;
}

struct proc_dir_entry *get_ambarella_proc_dir(void)
{
	return ambarella_proc_dir;
}
EXPORT_SYMBOL(get_ambarella_proc_dir);

/* ==========================================================================*/
void __init ambarella_memblock_reserve(void)
{
	struct ambarella_mem_rev_info		rev_info;
	int					i;
	u32					bstadd, bstsize;

	if (!get_ambarella_mem_rev_info(&rev_info)) {
		for (i = 0; i < rev_info.counter; i++) {
			memblock_reserve(rev_info.desc[i].physaddr,
				rev_info.desc[i].size);
		}
	}

	if (get_ambarella_bstmem_info(&bstadd, &bstsize) == AMB_BST_MAGIC) {
		pr_info("\t--:\t0x%08x[0x%08x]\tBST\n", bstadd, bstsize);
	}
}

/* ==========================================================================*/
#if defined(CONFIG_PLAT_AMBARELLA_SUPPORT_HAL)
#else
static struct clk gclk_arm = {
	.parent		= NULL,
	.name		= "gclk_arm",
	.rate		= 0,
	.frac_mode	= 0,
	.ctrl_reg	= PLL_REG_UNAVAILABLE,
	.pres_reg	= PLL_REG_UNAVAILABLE,
	.post_reg	= SCALER_ARM_ASYNC_REG,
	.frac_reg	= PLL_REG_UNAVAILABLE,
	.ctrl2_reg	= PLL_REG_UNAVAILABLE,
	.ctrl3_reg	= PLL_REG_UNAVAILABLE,
	.lock_reg	= PLL_REG_UNAVAILABLE,
	.lock_bit	= 0,
	.divider	= 0,
	.max_divider	= (1 << 3) - 1,
	.extra_scaler	= 0,
	.ops		= &ambarella_rct_scaler_ops,
};
#if defined(CONFIG_PLAT_AMBARELLA_CORTEX)
static struct clk gclk_cortex = {
	.parent		= NULL,
	.name		= "gclk_cortex",
	.rate		= 0,
	.frac_mode	= 0,
	.ctrl_reg	= PLL_CORTEX_CTRL_REG,
	.pres_reg	= PLL_REG_UNAVAILABLE,
	.post_reg	= PLL_REG_UNAVAILABLE,
	.frac_reg	= PLL_CORTEX_FRAC_REG,
	.ctrl2_reg	= PLL_CORTEX_CTRL2_REG,
	.ctrl3_reg	= PLL_CORTEX_CTRL3_REG,
	.lock_reg	= PLL_LOCK_REG,
	.lock_bit	= 2,
	.divider	= 0,
	.max_divider	= 0,
	.extra_scaler	= 0,
	.ops		= &ambarella_rct_pll_ops,
};

static struct clk gclk_axi = {
	.parent		= &gclk_cortex,
	.name		= "gclk_axi",
	.rate		= 0,
	.frac_mode	= 0,
	.ctrl_reg	= PLL_REG_UNAVAILABLE,
	.pres_reg	= PLL_REG_UNAVAILABLE,
	.post_reg	= PLL_REG_UNAVAILABLE,
	.frac_reg	= PLL_REG_UNAVAILABLE,
	.ctrl2_reg	= PLL_REG_UNAVAILABLE,
	.ctrl3_reg	= PLL_REG_UNAVAILABLE,
	.lock_reg	= PLL_REG_UNAVAILABLE,
	.lock_bit	= 0,
	.divider	= 3,
	.max_divider	= 0,
	.extra_scaler	= 0,
	.ops		= &ambarella_rct_scaler_ops,
};
#endif
#endif

/* ==========================================================================*/
int __init ambarella_init_machine(char *board_name)
{
	int retval = 0;

#if defined(CONFIG_PLAT_AMBARELLA_SUPPORT_HAL)
	char *pname;
	int version;
#else
	struct clk *ppll_out_idsp = NULL;
#endif

	ambarella_board_generic.board_chip = AMBARELLA_BOARD_CHIP(system_rev);
	ambarella_board_generic.board_type = AMBARELLA_BOARD_TYPE(system_rev);
	ambarella_board_generic.board_rev = AMBARELLA_BOARD_REV(system_rev);

	pr_info("Ambarella %s:\n", board_name);
	pr_info("\tchip id:\t\t%d\n", ambarella_board_generic.board_chip);
	pr_info("\tboard type:\t\t%d\n", ambarella_board_generic.board_type);
	pr_info("\tboard revision:\t\t%d\n", ambarella_board_generic.board_rev);
#if defined(CONFIG_PLAT_AMBARELLA_SUPPORT_HAL)
	retval = amb_get_chip_name(HAL_BASE_VP, &pname);
	BUG_ON(retval != AMB_HAL_SUCCESS);
	retval = amb_get_version(HAL_BASE_VP, &version);
	BUG_ON(retval != AMB_HAL_SUCCESS);

	ambarella_board_generic.board_poc =
		amb_get_system_configuration(HAL_BASE_VP);
	pr_info("\tchip name:\t\t%s\n", pname);
	pr_info("\tHAL version:\t\t%d\n", version);
	pr_info("\treference clock:\t%d\n",
		amb_get_reference_clock_frequency(HAL_BASE_VP));
	pr_info("\tboot type:\t\t0x%08x\n",
		amb_get_boot_type(HAL_BASE_VP));
	pr_info("\thif type:\t\t0x%08x\n",
		amb_get_hif_type(HAL_BASE_VP));
#else
	ambarella_board_generic.board_poc = amba_rct_readl(SYS_CONFIG_REG);

	ppll_out_idsp = clk_get(NULL, "pll_out_idsp");
	if (IS_ERR(ppll_out_idsp)) {
		BUG();
	}
	gclk_arm.parent = ppll_out_idsp;
	ambarella_register_clk(&gclk_arm);
#if defined(CONFIG_PLAT_AMBARELLA_CORTEX)
	ambarella_register_clk(&gclk_cortex);
	ambarella_register_clk(&gclk_axi);
#endif
#endif
	pr_info("\tsystem configuration:\t0x%08x\n",
		ambarella_board_generic.board_poc);

#if defined(CONFIG_PLAT_AMBARELLA_CORTEX)
#if (CHIP_REV == I1 || CHIP_REV == AMB_S2)
	amba_rct_writel(SCALER_ARM_ASYNC_REG, 0xF);
#endif
#endif

	retval = ambarella_create_proc_dir();
	BUG_ON(retval != 0);

	retval = ambarella_init_pll();
	BUG_ON(retval != 0);

	retval = ambarella_init_tm();
	BUG_ON(retval != 0);

	retval = ambarella_init_gpio();
	BUG_ON(retval != 0);

	retval = ambarella_init_fio();
	BUG_ON(retval != 0);

	retval = ambarella_init_adc();
	BUG_ON(retval != 0);

#if defined(CONFIG_PWM)
	retval = ambarella_init_pwm();
	BUG_ON(retval != 0);
#endif
	retval = ambarella_init_fb();
	BUG_ON(retval != 0);

	retval = ambarella_init_pm();
	BUG_ON(retval != 0);

	retval = ambarella_init_audio();
	BUG_ON(retval != 0);

	retval = ambarella_init_sd();
	BUG_ON(retval != 0);

#if (ETH_INSTANCES >= 1)
	retval = ambarella_init_eth0(ambarella_board_generic.eth0_mac);
	BUG_ON(retval != 0);
#endif
#if (ETH_INSTANCES >= 2)
	retval = ambarella_init_eth1(ambarella_board_generic.eth1_mac);
	BUG_ON(retval != 0);
#endif

	return retval;
}

void ambarella_restart_machine(char mode, const char *cmd)
{
#if defined(CONFIG_AMBARELLA_SUPPORT_BAPI)
	struct ambarella_bapi_reboot_info_s	reboot_info;

	reboot_info.magic = DEFAULT_BAPI_REBOOT_MAGIC;
	reboot_info.mode = AMBARELLA_BAPI_CMD_REBOOT_NORMAL;
	if (cmd) {
		if(strcmp(cmd, "recovery") == 0) {
			reboot_info.mode = AMBARELLA_BAPI_CMD_REBOOT_RECOVERY;
		} else if(strcmp(cmd, "fastboot") == 0) {
			reboot_info.mode = AMBARELLA_BAPI_CMD_REBOOT_FASTBOOT;
		}
	}
	ambarella_bapi_cmd(AMBARELLA_BAPI_CMD_SET_REBOOT_INFO, &reboot_info);
#endif

	local_irq_disable();
	local_fiq_disable();
	flush_cache_all();
	amba_rct_writel(SOFT_RESET_REG, 0x2);
	amba_rct_writel(SOFT_RESET_REG, 0x3);
}

