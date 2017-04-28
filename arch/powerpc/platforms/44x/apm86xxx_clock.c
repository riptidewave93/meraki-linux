/*
 * APM SoC APM86xxx Clock Interface
 * 
 * Copyright (c) 2010 Applied Micro Circuits Corporation.
 * All rights reserved. Prodyut Hazarika <phazarika@apm.com>
 *
 * Implements the clk api defined in include/linux/clk.h
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
 */
 
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/string.h>
#include <linux/device.h>
#include <linux/clk.h>
#include <linux/mutex.h>
#include <linux/io.h>

#include <linux/of_platform.h>
#include <asm/clk_interface.h>
#include <asm/apm_ipp_csr.h>
#include <asm/ipp.h>
#include <asm/apm86xxx_soc.h>

#define CLK_HAS_RATE	0x1	/* has rate in Hz */
#define CLK_HAS_CTRL	0x2	/* has control reg for en/dis */
#define CLK_NEEDS_PLL	0x4	/* needs PLL to be configured */
#define CLK_IS_PLL	0x8	/* corresponds to PLL */
#define CLK_SET_RATE	0x10	/* rate can be set */

struct pll_data {
	u16	clkf_val;
	u16	clkod_val;
	u16	clkr_val;
	u16	bwadj_val;
};

struct clk {
	struct list_head node;
	const char *dev_name;
	const char *con_id;
	int flags;
	int refcnt;
	unsigned long rate;
	void (*calc) (struct clk *);
	int (*enable) (struct clk *);
	void (*disable) (struct clk *);
	int (*set_rate) (struct clk *, unsigned long rate);
	struct clk *parent;
	struct pll_data *pll;
	int reg_group;
	u32 clken_reg_off;
	u32 clken_mask;
	u32 reset_reg_off;
	u32 reset_mask;
	u32 csr_reset_reg_off;
	u32 csr_reset_mask;
};

static LIST_HEAD(clocks);
static DEFINE_MUTEX(clocks_mutex);

static struct clk *apm_clk_get(struct device *dev, const char *id)
{
	struct clk *p, *clk = ERR_PTR(-ENOENT);
	const char *dev_id = dev ? dev_name(dev) : NULL;
	int dev_match = 0;
	int id_match = 0;

	mutex_lock(&clocks_mutex);
	list_for_each_entry(p, &clocks, node) {
		if (dev_id && p->dev_name && !strcmp(dev_id, p->dev_name))
			dev_match++;
		if (id && p->con_id && !strcmp(id, p->con_id)) 
			id_match++;
		if (dev_match || id_match) {
			clk = p;
			break;
		}
	}
	mutex_unlock(&clocks_mutex);
	return clk;
}

static int _apm_clk_enable(struct clk *clk)
{
	if (clk == NULL || IS_ERR(clk))
		return -EINVAL;
	_apm_clk_enable(clk->parent);
	if (clk->enable)
		clk->enable(clk);
	return 0;
}

static int apm_clk_enable(struct clk *clk)
{
	mutex_lock(&clocks_mutex);
	_apm_clk_enable(clk);
	mutex_unlock(&clocks_mutex);
	return 0;
}

static void _apm_clk_disable(struct clk *clk)
{
	if (clk == NULL || IS_ERR(clk))
		return;
	if (clk->disable)
		clk->disable(clk);
	_apm_clk_disable(clk->parent);
}

static void apm_clk_disable(struct clk *clk)
{
	mutex_lock(&clocks_mutex);
	_apm_clk_disable(clk);
	mutex_unlock(&clocks_mutex);
}

static int apm_enable(struct clk *clk)
{
	struct apm86xxx_reset_ctrl reset_ctrl;

	BUG_ON(!clk->clken_mask);
	memset(&reset_ctrl, 0, sizeof(reset_ctrl));
	reset_ctrl.reg_group = clk->reg_group;
	reset_ctrl.clken_mask = clk->clken_mask;
	reset_ctrl.reset_mask = clk->reset_mask;
	reset_ctrl.csr_reset_mask = clk->csr_reset_mask;
	return __reset_apm86xxx_block(&reset_ctrl);
}

static void apm_disable(struct clk *clk)
{
	struct apm86xxx_reset_ctrl reset_ctrl;

	BUG_ON(!clk->clken_mask);
	memset(&reset_ctrl, 0, sizeof(reset_ctrl));
	reset_ctrl.reset_type = ASSERT_RESET_FLAG;
	reset_ctrl.reg_group = clk->reg_group;
	reset_ctrl.clken_mask = clk->clken_mask;
	reset_ctrl.reset_mask = clk->reset_mask;
	reset_ctrl.csr_reset_mask = clk->csr_reset_mask;
	__reset_apm86xxx_block(&reset_ctrl);
}

static unsigned long apm_clk_get_rate(struct clk *clk)
{
	return (clk->flags & CLK_HAS_RATE) ? clk->rate : 0;
}

static int apm_clk_set_rate(struct clk *clk, unsigned long rate)
{
	int ret = -EINVAL;

	mutex_lock(&clocks_mutex);
	if (clk->flags & CLK_SET_RATE) {
		ret = clk->set_rate(clk, rate);
		if (!ret)
			clk->rate = rate;
	}
	mutex_unlock(&clocks_mutex);
	return ret;
}

static void apm_refclk_calc(struct clk *clk)
{
	clk->rate = APM86xxx_REFCLK_FREQ;
}

static struct clk apm_refclk = {
	.con_id = "refclk",
	.calc = apm_refclk_calc,
};

static void apm_soc_pll_calc(struct clk *clk)
{
	u32 freq;
	
	if (__apm86xxx_get_freq(APM86xxx_SOC_PLL_FREQ, &freq) == 0) {
		clk->rate = freq; 
	}
}

static struct clk apm_soc_pll = {
	.con_id = "soc_pll",
	.flags = CLK_IS_PLL,
	.calc = apm_soc_pll_calc,
	.parent = &apm_refclk,
};

static void apm_cpu_pll_calc(struct clk *clk)
{
	u32 freq;
	
	if (__apm86xxx_get_freq(APM86xxx_CPU_PLL_FREQ, &freq) == 0) {
		clk->rate = freq;
	}
}

static struct clk apm_cpu_pll = {
	.con_id = "cpu_pll",
	.flags = CLK_IS_PLL,
	.calc = apm_cpu_pll_calc,
	.parent = &apm_refclk,
};

static struct clk apm_lpf_clk = {
	.con_id = "lpf",
	.parent = &apm_soc_pll,
};

static struct clk apm_pka_trng_clk = {
	.con_id = "pka-trng",
	.parent = &apm_lpf_clk,
	.enable = apm_enable,
	.disable = apm_disable,
	.reg_group = REG_GROUP_SRST1_CLKEN1,
	.clken_reg_off = SCU_CLKEN1_ADDR,
	.clken_mask = MPA_PKP1_F1_MASK, 
	.reset_reg_off = SCU_SRST1_ADDR,
	.reset_mask = MPA_PKP1_MASK,
};

static struct clk apm_qml_clk = {
	.con_id = "qml",
	.parent = &apm_lpf_clk,
	.enable = apm_enable,
	.disable = apm_disable,
	.reg_group = REG_GROUP_SRST_CLKEN,
	.clken_reg_off = SCU_CLKEN_ADDR,
	.clken_mask = QMLITE_F1_MASK, 
	.reset_reg_off = SCU_SRST_ADDR,
	.reset_mask = QMLITE_MASK,
	.csr_reset_reg_off = SCU_CSR_SRST_ADDR,
	.csr_reset_mask = QMLITE_F2_MASK,
};

static int enet0_enable(struct clk *clk)
{
	struct apm86xxx_reset_ctrl reset_ctrl;
	u32 val;
	int ret;
	int do_reset = 1;

	BUG_ON(!clk->clken_mask);

	ret = apm86xxx_read_scu_reg(clk->reset_reg_off, &val);
	if (!ret) {
#ifdef CONFIG_SMP
		if (cpu_enabled(1) &&
	   	(!(val & ENET1_MASK))
	     	&& (!(val & ENET0_MASK))) {
			do_reset = 0;
		}
#endif 
		memset(&reset_ctrl, 0, sizeof(reset_ctrl));
		reset_ctrl.reg_group = clk->reg_group;
		reset_ctrl.clken_mask = clk->clken_mask;
		/* Dont Reset if Port1 is out of reset */
		reset_ctrl.reset_mask =
		    	(do_reset) ? clk->reset_mask:0;
		reset_ctrl.csr_reset_mask =
		     	(do_reset) ? clk->csr_reset_mask:0;
		ret = __reset_apm86xxx_block(&reset_ctrl);
	}
	return ret;
}

static void enet0_disable(struct clk *clk)
{
	struct apm86xxx_reset_ctrl reset_ctrl;

	BUG_ON(!clk->clken_mask);
	memset(&reset_ctrl, 0, sizeof(reset_ctrl));
	reset_ctrl.reset_type = ASSERT_RESET_FLAG;
	reset_ctrl.reg_group = clk->reg_group;
	reset_ctrl.clken_mask = clk->clken_mask;
	reset_ctrl.reset_mask = clk->reset_mask;
	__reset_apm86xxx_block(&reset_ctrl);
}



static struct clk apm_enet0_clk = {
	.con_id = "enet0",
	.parent = &apm_lpf_clk,
	.enable = enet0_enable,
	.disable = enet0_disable,
	.reg_group = REG_GROUP_SRST_CLKEN,
	.clken_reg_off = SCU_CLKEN_ADDR,
	.clken_mask = ENET0_F1_MASK, 
	.reset_reg_off = SCU_SRST_ADDR,
	.reset_mask = ENET0_MASK,
	.csr_reset_reg_off = SCU_CSR_SRST_ADDR,
	.csr_reset_mask = ENET0_F3_MASK,
};

static struct clk apm_enet1_clk = {
	.con_id = "enet1",
	.parent = &apm_lpf_clk,
	.enable = apm_enable,
	.disable = apm_disable,
	.reg_group = REG_GROUP_SRST_CLKEN,
	.clken_reg_off = SCU_CLKEN_ADDR,
	.clken_mask = ENET1_F1_MASK, 
	.reset_reg_off = SCU_SRST_ADDR,
	.reset_mask = ENET1_MASK,
	.csr_reset_reg_off = SCU_CSR_SRST_ADDR,
	.csr_reset_mask = ENET1_F2_MASK,
};
static void apm_hbf_clk_calc(struct clk *clk)
{
	u32 freq;
	
	if (__apm86xxx_get_freq(APM86xxx_AXI_FREQ, &freq) == 0) {
		clk->rate = freq;
	}
}

static struct clk apm_hbf_clk = {
	.con_id = "hbf",
	.calc = apm_hbf_clk_calc,
	.parent = &apm_soc_pll,
};

static struct clk apm_pcie1_clk = {
	.con_id = "pcie1",
	.parent = &apm_hbf_clk,
	.enable = apm_enable,
	.disable = apm_disable,
	.reg_group = REG_GROUP_SRST_CLKEN,
	.clken_reg_off = SCU_CLKEN_ADDR,
	.clken_mask = PCIE1_F1_MASK, 
	.reset_reg_off = SCU_SRST_ADDR,
	.reset_mask = PCIE1_MASK,
	.csr_reset_reg_off = SCU_CSR_SRST_ADDR,
	.csr_reset_mask = PCIE1_F3_MASK,
};

static struct clk apm_pcie2_clk = {
	.con_id = "pcie2",
	.parent = &apm_hbf_clk,
	.enable = apm_enable,
	.disable = apm_disable,
	.reg_group = REG_GROUP_SRST_CLKEN,
	.clken_reg_off = SCU_CLKEN_ADDR,
	.clken_mask = PCIE2_F1_MASK, 
	.reset_reg_off = SCU_SRST_ADDR,
	.reset_mask = PCIE2_MASK,
	.csr_reset_reg_off = SCU_CSR_SRST_ADDR,
	.csr_reset_mask = PCIE2_F3_MASK,
};

static struct clk apm_pcie3_clk = {
	.con_id = "pcie3",
	.parent = &apm_hbf_clk,
	.enable = apm_enable,
	.disable = apm_disable,
	.reg_group = REG_GROUP_SRST_CLKEN,
	.clken_reg_off = SCU_CLKEN_ADDR,
	.clken_mask = PCIE3_F1_MASK, 
	.reset_reg_off = SCU_SRST_ADDR,
	.reset_mask = PCIE3_MASK,
	.csr_reset_reg_off = SCU_CSR_SRST_ADDR,
	.csr_reset_mask = PCIE3_F3_MASK,
};

static struct clk apm_sata0_clk = {
	.con_id = "sata0",
	.flags = CLK_HAS_CTRL,
	.parent = &apm_hbf_clk,
	.enable = apm_enable,
	.disable = apm_disable,
	.reg_group = REG_GROUP_SRST_CLKEN,
	.clken_reg_off = SCU_CLKEN_ADDR,
	.clken_mask = SATA0_F1_MASK, 
	.reset_reg_off = SCU_SRST_ADDR,
	.reset_mask = SATA0_MASK,
	.csr_reset_reg_off = SCU_CSR_SRST_ADDR,
	.csr_reset_mask = SATA0_F3_MASK,
};

static struct clk apm_sata1_clk = {
	.con_id = "sata1",
	.flags = CLK_HAS_CTRL,
	.parent = &apm_hbf_clk,
	.enable = apm_enable,
	.disable = apm_disable,
	.reg_group = REG_GROUP_SRST_CLKEN,
	.clken_reg_off = SCU_CLKEN_ADDR,
	.clken_mask = SATA1_F1_MASK, 
	.reset_reg_off = SCU_SRST_ADDR,
	.reset_mask = SATA1_MASK,
	.csr_reset_reg_off = SCU_CSR_SRST_ADDR,
	.csr_reset_mask = SATA1_F3_MASK,
};

static struct clk apm_security_clk = {
	.con_id = "security",
	.parent = &apm_hbf_clk,
	.enable = apm_enable,
	.disable = apm_disable,
	.reg_group = REG_GROUP_SRST1_CLKEN1,
	.clken_reg_off = SCU_CLKEN1_ADDR,
	.clken_mask = SEC1_F1_MASK | SEC_EIP961_MASK |
			SEC_SHIM1_MASK, 
	.reset_reg_off = SCU_SRST1_ADDR,
	.reset_mask = SEC1_MASK,
	.csr_reset_reg_off = SCU_CSR_SRST1_ADDR,
	.csr_reset_mask = SEC1_F2_MASK,
};
static int qm_enable(struct clk *clk)
{
	struct apm86xxx_reset_ctrl reset_ctrl;
#ifndef CONFIG_SMP	
	u32 val;
#endif	
	int ret;

	BUG_ON(!clk->clken_mask);
	
#ifdef CONFIG_SMP
	ret = apm86xxx_read_scu_reg(clk->clken_reg_off, &val);
	if (!ret) {
		if ((cpu_enabled(1)) && (val & QMTM_F1_MASK)) {
			return ret;
		}
	}
#endif		
	memset(&reset_ctrl, 0, sizeof(reset_ctrl));
	reset_ctrl.reg_group = clk->reg_group;
	reset_ctrl.clken_mask = clk->clken_mask;
	reset_ctrl.reset_mask = clk->reset_mask;
	reset_ctrl.csr_reset_mask = clk->csr_reset_mask;
	ret = __reset_apm86xxx_block(&reset_ctrl);
	return ret;
}

static void qm_disable(struct clk *clk)
{
	struct apm86xxx_reset_ctrl reset_ctrl;

	BUG_ON(!clk->clken_mask);
	memset(&reset_ctrl, 0, sizeof(reset_ctrl));
	reset_ctrl.reset_type = ASSERT_RESET_FLAG;
	reset_ctrl.reg_group = clk->reg_group;
	reset_ctrl.clken_mask = clk->clken_mask;
	reset_ctrl.reset_mask = clk->reset_mask;
	__reset_apm86xxx_block(&reset_ctrl);
}

static struct clk apm_qm_tm_clk = {
	.con_id = "qm-tm",
	.parent = &apm_hbf_clk,
	.enable = qm_enable,
	.disable = qm_disable,
	.reg_group = REG_GROUP_SRST_CLKEN,
	.clken_reg_off = SCU_CLKEN_ADDR,
	.clken_mask = QMTM_F1_MASK,
	.reset_reg_off = SCU_SRST_ADDR,
	.reset_mask = QMTM_MASK,
	.csr_reset_reg_off = SCU_CSR_SRST_ADDR,
	.csr_reset_mask = QMTM_F3_MASK,
};

static struct clk apm_ocm_clk = {
	.con_id = "ocm",
	.flags = CLK_HAS_CTRL,
	.parent = &apm_hbf_clk,
	.reg_group = REG_GROUP_SRST_CLKEN,
	.clken_reg_off = SCU_CLKEN_ADDR,
	.clken_mask = OCM_F1_MASK, 
	.reset_reg_off = SCU_SRST_ADDR,
	.reset_mask = OCM_MASK,
	.csr_reset_reg_off = SCU_CSR_SRST_ADDR,
	.csr_reset_mask = OCM_F3_MASK,
};

static int pktdma_enable(struct clk *clk)
{
	struct apm86xxx_reset_ctrl reset_ctrl;
#ifdef CONFIG_SMP
	u32 val;
#endif
	int ret;

	BUG_ON(!clk->clken_mask);
	
#ifdef CONFIG_SMP
	ret = apm86xxx_read_scu_reg(clk->clken_reg_off, &val);
	if (!ret) {
		if ((cpu_enabled(1)) && (val & PKTDMA_F1_MASK)) {
			return ret;
		}
	}
#endif
	memset(&reset_ctrl, 0, sizeof(reset_ctrl));
	reset_ctrl.reg_group = clk->reg_group;
	reset_ctrl.clken_mask = clk->clken_mask;
	reset_ctrl.reset_mask = clk->reset_mask;
	reset_ctrl.csr_reset_mask = clk->csr_reset_mask;
	ret = __reset_apm86xxx_block(&reset_ctrl);
	return ret;
}

static void pktdma_disable(struct clk *clk)
{
	struct apm86xxx_reset_ctrl reset_ctrl;

	BUG_ON(!clk->clken_mask);
	memset(&reset_ctrl, 0, sizeof(reset_ctrl));
	reset_ctrl.reset_type = ASSERT_RESET_FLAG;
	reset_ctrl.reg_group = clk->reg_group;
	reset_ctrl.clken_mask = clk->clken_mask;
	reset_ctrl.reset_mask = clk->reset_mask;
	__reset_apm86xxx_block(&reset_ctrl);
}


static struct clk apm_pktdma_clk = {
	.con_id = "pktdma",
	.parent = &apm_hbf_clk,
	.enable = pktdma_enable,
	.disable = pktdma_disable,
	.reg_group = REG_GROUP_SRST_CLKEN,
	.clken_reg_off = SCU_CLKEN_ADDR,
	.clken_mask = PKTDMA_F1_MASK,
	.reset_reg_off = SCU_SRST_ADDR,
	.reset_mask = PKTDMA_MASK,
	.csr_reset_reg_off = SCU_CSR_SRST_ADDR,
	.csr_reset_mask = PKTDMA_F2_MASK,
};

static int cle_enable(struct clk *clk)
{
	struct apm86xxx_reset_ctrl reset_ctrl;
	u32 val;
	int ret;

	BUG_ON(!clk->clken_mask);

	ret = apm86xxx_read_scu_reg(clk->csr_reset_reg_off, &val);
	if (!ret) {
		memset(&reset_ctrl, 0, sizeof(reset_ctrl));
		reset_ctrl.reg_group = clk->reg_group;
		reset_ctrl.clken_mask = clk->clken_mask;
		reset_ctrl.reset_mask = clk->reset_mask;
		reset_ctrl.csr_reset_mask = (val & CLE1_MASK) ?
			clk->csr_reset_mask: 0;
		ret = __reset_apm86xxx_block(&reset_ctrl);
	}
	return ret;
}

static void cle_disable(struct clk *clk)
{
	struct apm86xxx_reset_ctrl reset_ctrl;

	BUG_ON(!clk->clken_mask);
	memset(&reset_ctrl, 0, sizeof(reset_ctrl));
	reset_ctrl.reset_type = ASSERT_RESET_FLAG;
	reset_ctrl.reg_group = clk->reg_group;
	reset_ctrl.clken_mask = clk->clken_mask;
	reset_ctrl.reset_mask = clk->reset_mask;
	__reset_apm86xxx_block(&reset_ctrl);
}


static struct clk apm_cle0_clk = {
	.con_id = "cle0",
	.parent = &apm_hbf_clk,
	.enable = cle_enable,
	.disable = cle_disable,
	.reg_group = REG_GROUP_SRST1_CLKEN1,
	.clken_reg_off = SCU_CLKEN1_ADDR,
	.clken_mask = CLE0_IL1_MASK, 
	.reset_reg_off = SCU_SRST1_ADDR,
	.reset_mask = CLE01_MASK,
	.csr_reset_reg_off = SCU_CSR_SRST1_ADDR,
	.csr_reset_mask = CLE1_MASK,
};

static struct clk apm_cle1_clk = {
	.con_id = "cle1",
	.parent = &apm_hbf_clk,
	.enable = cle_enable,
	.disable = cle_disable,
	.reg_group = REG_GROUP_SRST1_CLKEN1,
	.clken_reg_off = SCU_CLKEN1_ADDR,
	.clken_mask = CLE1_IL1_MASK, 
	.reset_reg_off = SCU_SRST1_ADDR,
	.reset_mask = CLE11_MASK,
	.csr_reset_reg_off = SCU_CSR_SRST1_ADDR,
	.csr_reset_mask = CLE1_MASK,
};

static struct clk apm_lac_clk = {
	.con_id = "lac",
	.parent = &apm_hbf_clk,
	.enable = apm_enable,
	.disable = apm_disable,
	.reg_group = REG_GROUP_SRST1_CLKEN1,
	.clken_reg_off = SCU_CLKEN1_ADDR,
	.clken_mask = CLE_LA1_F1_MASK,
	.reset_reg_off = SCU_SRST1_ADDR,
	.reset_mask = CLE_LA1_MASK,
	.csr_reset_reg_off = SCU_CSR_SRST1_ADDR,
	.csr_reset_mask = CLE1_MASK,
};

static struct clk apm_lcd_clk = {
	.con_id = "lcd",
	.parent = &apm_hbf_clk,
	.enable = apm_enable,
	.disable = apm_disable,
	.reg_group = REG_GROUP_SRST_CLKEN,
	.clken_reg_off = SCU_CLKEN_ADDR,
	.clken_mask = LCD_F1_MASK, 
	.reset_reg_off = SCU_SRST_ADDR,
	.reset_mask = LCD_MASK,
	.csr_reset_reg_off = SCU_CSR_SRST_ADDR,
	.csr_reset_mask = LCD_F2_MASK,
};

static void apm_ahb_clk_calc(struct clk *clk)
{
	u32 freq;
	
	if (__apm86xxx_get_freq(APM86xxx_AHB_FREQ, &freq) == 0) {
		clk->rate = freq;
	}
}

static struct clk apm_ahb_clk = {
	.con_id = "ahb",
	.calc = apm_ahb_clk_calc,
	.parent = &apm_hbf_clk,
};

static void apm_iahb_clk_calc(struct clk *clk)
{
	u32 freq;
	
	if (__apm86xxx_get_freq(APM86xxx_IAHB_FREQ, &freq) == 0) {
		clk->rate = freq;
	}
}

static struct clk apm_iahb_clk = {
	.con_id = "iahb",
	.calc = apm_iahb_clk_calc,
	.parent = &apm_hbf_clk,
};

static struct clk apm_usb_otg_clk = {
	.con_id = "usb0",	/* USB OTG */
	.flags = CLK_HAS_CTRL,
	.parent = &apm_ahb_clk,
	.enable = apm_enable,
	.disable = apm_disable,
	.reg_group = REG_GROUP_SRST_CLKEN,
	.clken_reg_off = SCU_CLKEN_ADDR,
	.clken_mask = USB0_F1_MASK, 
	.reset_reg_off = SCU_SRST_ADDR,
	.reset_mask = USB0_MASK,
};

static struct clk apm_usb_host0_clk = {
	.con_id = "usb1",	/* USB EHCI/OHCI Host 0 */
	.flags = CLK_HAS_CTRL,
	.parent = &apm_ahb_clk,
	.enable = apm_enable,
	.disable = apm_disable,
	.reg_group = REG_GROUP_SRST_CLKEN,
	.clken_reg_off = SCU_CLKEN_ADDR,
	.clken_mask = USB1_F1_MASK, 
	.reset_reg_off = SCU_SRST_ADDR,
	.reset_mask = USB1_MASK,
};

static struct clk apm_usb_host1_clk = {
	.con_id = "usb2",	/* USB EHCI/OHCI Host 1 */
	.flags = CLK_HAS_CTRL,
	.parent = &apm_ahb_clk,
	.enable = apm_enable,
	.disable = apm_disable,
	.reg_group = REG_GROUP_SRST_CLKEN,
	.clken_reg_off = SCU_CLKEN_ADDR,
	.clken_mask = USB2_F1_MASK, 
	.reset_reg_off = SCU_SRST_ADDR,
	.reset_mask = USB2_MASK,
};

static void apm_sdio_clk_calc(struct clk *clk)
{
	u32 freq;
	
	if (__apm86xxx_get_freq(APM86xxx_SDIO_FREQ, &freq) == 0) {
		clk->rate = freq; 
	}
}
static int apm_sdio_clk_set(struct clk *clk,  unsigned long rate)
{
	return __apm86xxx_set_freq(APM86xxx_SDIO_FREQ, rate);
}

static struct clk apm_sdio_clk = {
	.con_id = "sdio",
	.flags = CLK_HAS_CTRL | CLK_SET_RATE,
	.calc = apm_sdio_clk_calc,
	.parent = &apm_ahb_clk,
	.enable = apm_enable,
	.disable = apm_disable,
	.set_rate = apm_sdio_clk_set,
	.reg_group = REG_GROUP_SRST_CLKEN,
	.clken_reg_off = SCU_CLKEN_ADDR,
	.clken_mask = SDIO_F1_MASK, 
	.reset_reg_off = SCU_SRST_ADDR,
	.reset_mask = SDIO_MASK,
};

static struct clk apm_ndfc_clk = {
	.con_id = "ndfc",
	.flags = CLK_HAS_CTRL,
	.parent = &apm_ahb_clk,
	.reg_group = REG_GROUP_SRST_CLKEN,
	.clken_reg_off = SCU_CLKEN_ADDR,
	.clken_mask = NDFC_F1_MASK, 
	.reset_reg_off = SCU_SRST_ADDR,
	.reset_mask = NDFC_MASK,
};

static void apm_apb_clk_calc(struct clk *clk)
{
	u32 freq;
	
	if (__apm86xxx_get_freq(APM86xxx_APB_FREQ, &freq) == 0) {
		clk->rate = freq;
	}
}

static struct clk apm_apb_clk = {
	.con_id = "apb",
	.calc = apm_apb_clk_calc,
	.parent = &apm_ahb_clk,
};

static void apm_spi_clk_calc(struct clk *clk)
{
	/* FIXME: divisor */
	int divisor = 100;
	clk->rate = clk->parent->rate/divisor;
}

static struct clk apm_spi_clk = {
	.con_id = "spi",
	.flags = CLK_HAS_CTRL,
	.calc = apm_spi_clk_calc,
	.parent = &apm_apb_clk,
	.enable = apm_enable,
	.disable = apm_disable,
	.reg_group = REG_GROUP_SRST_CLKEN,
	.clken_reg_off = SCU_CLKEN_ADDR,
	.clken_mask = SPI_F1_MASK, 
	.reset_reg_off = SCU_SRST_ADDR,
	.reset_mask = SPI_MASK,
};

static void apm_i2c0_clk_calc(struct clk *clk)
{
	clk->rate = clk->parent->rate;
}

static struct clk apm_i2c0_clk = {
	.con_id = "i2c0",
	.calc = apm_i2c0_clk_calc,
	.parent = &apm_apb_clk,
};

static void apm_i2c1_clk_calc(struct clk *clk)
{
	clk->rate = clk->parent->rate;
}

static struct clk apm_i2c1_clk = {
	.con_id = "i2c1",
	.calc = apm_i2c1_clk_calc,
	.parent = &apm_iahb_clk,
};

static struct clk *clks[] = {
	&apm_refclk,
	&apm_soc_pll,
	&apm_cpu_pll,
	&apm_lpf_clk,
	&apm_pka_trng_clk,
	&apm_qml_clk,
	&apm_enet0_clk,
	&apm_enet1_clk,
	&apm_hbf_clk,
	&apm_pcie1_clk,
	&apm_pcie2_clk,
	&apm_pcie3_clk,
	&apm_sata0_clk,
	&apm_sata1_clk,
	&apm_security_clk,
	&apm_qm_tm_clk,
	&apm_ocm_clk,
	&apm_pktdma_clk,
	&apm_cle0_clk,
	&apm_cle1_clk,
	&apm_lac_clk,
	&apm_lcd_clk,
	&apm_ahb_clk,
	&apm_iahb_clk,
	&apm_usb_otg_clk,
	&apm_usb_host0_clk,
	&apm_usb_host1_clk,
	&apm_sdio_clk,
	&apm_ndfc_clk,
	&apm_apb_clk,
	&apm_spi_clk,
	&apm_i2c0_clk,
	&apm_i2c1_clk,
	NULL
};

/* apm_rate_clk_init must be called with mutex held */
static void apm_rate_clk_init(struct clk *clk)
{
	if (clk->calc) {
		clk->calc(clk);
		clk->flags |= CLK_HAS_RATE;
	}
	list_add(&clk->node, &clocks);
}

/* apm_rate_clks_init must be called with mutex held */
static void apm_rate_clks_init(void)
{
	struct clk **cpp, *clk;

	cpp = clks;
	while ((clk = *cpp++))
		apm_rate_clk_init(clk);
}

static struct clk_interface apm_clk_functions = {
	.clk_get		= apm_clk_get,
	.clk_enable		= apm_clk_enable,
	.clk_disable		= apm_clk_disable,
	.clk_get_rate		= apm_clk_get_rate,
	.clk_set_rate		= apm_clk_set_rate,
};

/* External Interface Hooks - initialized by iPP driver */
int apm_clk_init(void)
{
	mutex_lock(&clocks_mutex);
	apm_rate_clks_init();
	clk_functions = apm_clk_functions;
	mutex_unlock(&clocks_mutex);

	return 0;
}

ssize_t apm_show_clktree(char *buf)
{
	struct clk *p;
        ssize_t size = 0;
	int disabled;
	u32 val;

	mutex_lock(&clocks_mutex);
	size += snprintf(buf + size, PAGE_SIZE - size, "CLOCKS:\n");
	list_for_each_entry(p, &clocks, node) {
		disabled = 0;
		if (p->flags & CLK_HAS_CTRL) {
			BUG_ON(!p->clken_reg_off);
			BUG_ON(!p->clken_mask);
			disabled = apm86xxx_read_scu_reg(p->clken_reg_off,
								&val);
			if (!disabled) {
				disabled = !(val & p->clken_mask);
			}
		}

		if (disabled) {
			size += snprintf(buf + size, PAGE_SIZE - size,
						"  %s : DISABLED \n",
						p->con_id);
		} else {
			if (p->flags & CLK_HAS_RATE) {
				size += snprintf(buf + size,
						PAGE_SIZE - size,
						"  %s : %ld %sHz ",
						p->con_id,
						(p->rate < 1000000) ? 
							p->rate/1000:
							p->rate/1000000,
							(p->rate < 1000000) ?
							"K" : "M");
				size += snprintf(buf + size,
						PAGE_SIZE - size, "\n");
			} else {
				size += snprintf(buf + size, PAGE_SIZE - size,
							"  %s : ENABLED \n",
							p->con_id);
			}
		}
	}
	mutex_unlock(&clocks_mutex);
        return size;
}
