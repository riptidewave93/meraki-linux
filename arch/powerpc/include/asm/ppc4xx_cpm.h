/*
 * PowerPC 4xx Clock and Power Management
 *
 * (C) Copyright 2008, Applied Micro Circuits Corporation
 * Victor Gallardo (vgallardo@amcc.com)
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

#ifndef __ASM_POWERPC_PPC4xx_CPM_H__
#define __ASM_POWERPC_PPC4xx_CPM_H__

#define CPM_PM_DOZE		0
#define CPM_PM_NAP		1
#define CPM_PM_DEEPSLEEP	2
#define CPM_PM_MODES_MAX	3


#define CPM_PM_EMAC0_MR0_RXI			0x80000000
#define CPM_PM_ENABLE_EMAC0_MR0_RXE		0x08000000
#define CPM_PM_DISABLE_EMAC0_MR0_RXE	0xf7ffffff

#if defined(CONFIG_APM82181) || defined(CONFIG_APM82161)
#define CPM_PM_ENABLE_EMAC0_MR0_WKE		0x05000000
#define CPM_PM_DISABLE_EMAC0_MR0_WKE	0xfaffffff
#else
#define CPM_PM_ENABLE_EMAC0_MR0_WKE		0x04000000
#define CPM_PM_DISABLE_EMAC0_MR0_WKE	0xfbffffff
#endif /* defined(CONFIG_APM82181) || defined(CONFIG_APM82161) */


#define CPM_PM_DATA_IIC_PTR	255

#if defined(CONFIG_460EX) || defined(CONFIG_460GT)

#define CONFIG_CPM_PM_AD5243
#define CPM_PM_AD5243_ADDR      0x5e
#define CPM_PM_AD5243_1_290V    0x80    /* 1.290 volts */
#define CPM_PM_AD5243_1_250V    0x7A    /* 1.250 volts */
#define CPM_PM_AD5243_1_200V    0x70    /* 1.200 volts */
#define CPM_PM_AD5243_1_150V    0x67    /* 1.150 volts */
#define CPM_PM_AD5243_1_100V    0x5d    /* 1.100 volts */
#define CPM_PM_AD5243_1_050V    0x54    /* 1.050 volts */
#define CPM_PM_AD5243_1_000V    0x4a    /* 1.000 volts */
#endif

#ifndef __ASSEMBLY__

#if defined(CONFIG_PPC4xx_CPM)

#include <linux/suspend.h>

int cpm_pm_idle(int pm_mode);
int cpm_pm_suspend(suspend_state_t state, int suspend_mode);
const char *cpm_mode_name(int mode);
#else

#define cpm_pm_idle(pm_mode)	do { } while(0)
#define cpm_pm_suspend(state)	0

#endif /* CONFIG_PPC4xx_CPM */

#endif /* __ASSEMBLY__ */

#endif /* __ASM_POWERPC_PPC4xx_CPM_H__ */

