/*
 * APM86xxx suspend lite support
 *
 * Copyright (c) 2011 Applied Micro Circuits Corporation.
 * Victor Gallardo (vgallardo@apm.com)
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef __POWERPC_PLATFORMS_44X_APM86xxx_SUSPEND_LITE_H
#define __POWERPC_PLATFORMS_44X_APM86xxx_SUSPEND_LITE_H

#ifndef __ASSEMBLY__

#define LITE_DATA_SIZE  (256*sizeof(unsigned int))
#define LITE_ALIGN      32

extern void apm86xxx_lite_suspend(unsigned int *data);
extern const long apm86xxx_lite_suspend_size;

extern void apm86xxx_lite_resume(unsigned int *data);
extern const long apm86xxx_lite_resume_size;

struct apm86xxx_lite {
        /* functions in OCM */
        void (*suspend)(unsigned int *data);
        void (*resume )(unsigned int *data);
        /* data in OCM */
        unsigned int *data;
        /* MEMC Registers (used for putting DDR in self-refresh) */
	unsigned int memc;
        /* SCU Registers */
	unsigned int scu;
};

#endif /* __ASSEMBLY__ */

#define LITE_RESUME			0x00
#define LITE_LR				0x04
#define LITE_SPRN_IVPR			0x08
#define LITE_SPRN_IVOR4			0x0c
#define LITE_MEMC			0x10
#define LITE_MEMC_REG_DDRC_0		0x14
#define LITE_MEMC_REG_PHY_1		0x18
#define LITE_MEMC_DDR_IO_CONTROL	0x1c
#define LITE_MEMC_DDRC_MISC		0x20
#define LITE_SCU			0x24
#define LITE_SCU_SOCDIV1		0x28

#define MEMC_REG_DDRC_0			0x010
#define  REG_DDRC_0_SELFREFRESH		0x10000000
#define  RMODE0_MASK			0x03000000
#define  PHY_SOFT_RSTB0_MASK		0x00800000
#define  ACTIVE_RANKS0_MASK		0x000f0000

#define MEMC_REG_PHY_1			0x0b0
#define  DIF_ON1_MASK			0xc0000000
#define  DIF_OFF1_MASK			0x30000000

#define MEMC_DDRC_REG_DBG		0x3a8
#define  DDRC_REG_DBG_MODE_NORMAL	0x00000001
#define  DDRC_REG_DBG_MODE_SELFREFRESH	0x00000003

#define MEMC_DDR_IO_CONTROL		0x458
#define  RF_IO_DRVRCNTR_MASK		0x0e000000
#define  RF_IO_DRVRCNTR_ADD_MASK	0x01c00000
#define  RF_IO_SR_MASK			0x00300000
#define  RF_IO_SR_ADD_MASK		0x000c0000
#define  RF_IO_PECNTR_MASK		0x00020000
#define  RF_IO_CROWBAREN_MASK		0x00008000
#define  RF_IO_RMODE_31_0_MASK		0x00003000
#define  RF_IO_RMODE_63_32_MASK		0x00000c00
#define  RF_IO_DDR2_MODE_MASK		0x80000000
#define  RF_IO_ODT_MODE_MASK		0x70000000
#define  RF_IO_POWER_DOWN_MODE_MASK	0x00000010
#define  RF_IO_PWSAVE_MODE_MASK		0x00000008
#define  RF_IO_ODT_MODE_SHIFT		28

#define MEMC_DDRC_MISC			0x528
#define  RF_IO_PEMPHEN_MASK		0x30000000
#define  RF_IO_TEN_MASK			0x0ff80000

#define SCU_SOCPLL2			0x00000048
#define  SOCPLL2_PWRDN			0x80000000
#define  SOCPLL2_RESET			0x00010000

#define SCU_SOCDIV1			0x000000c4
#define  SOCDIV1_DDR2X_CLK_FREQ_SEL1	0x00001f00


#endif	/* __POWERPC_PLATFORMS_44X_APM86xxx_SUSPEND_LITE_H */
