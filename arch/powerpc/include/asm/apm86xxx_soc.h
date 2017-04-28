/*
 * APM86xxx SoC helper code
 *
 * Copyright (c) 2010 Applied Micro Circuits Corporation.
 * Victor Gallardo <vgallardo@apm.com>.
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

#ifndef _ASM_POWERPC_APM86xxx_SOC_H
#define _ASM_POWERPC_APM86xxx_SOC_H

#ifdef __KERNEL__

#define DCRN_STRK_CR0	0x180
#define DCRN_STRK_CR1	0x181
#define DCRN_STRK_CR2	0x182
#define DCRN_STRK_CR3	0x183
#define DCRN_STRK_CR4	0x184
#define STRK_CR4_SNPI	0x80000000 /* Snoop Inhibit */

#ifndef __ASSEMBLY__
void ppc44x_idle_enable(void);
void ppc44x_idle_disable(void);

#ifndef CONFIG_SMP
unsigned int cpu_enabled(unsigned int cpu);
#endif

void cpu_l2c_enable(void);
void cpu_l2c_disable(void);

extern void flush_dcache(void);
extern void l2c_disable(void);
extern void l2c_suspend(void);
extern void l2c_resume(void);

/* AHBC USB Register table */
typedef struct ahbc_usb_reg {
	volatile uint32_t tune;
	volatile uint32_t rstctl;
	volatile uint32_t clkctl;
} ahbc_usb_reg_t;

/* AHBC USB Enable/Disable APIs */
void apm86xxx_ahbc_usb_enable(int port);
void apm86xxx_ahbc_usb_disable(int port);
int apm86xxx_usb_clk_enable(int port);

/* Get/Set AHBC Register */
u32 apm86xxx_ahbc_get_reg(u32 offset);
void apm86xxx_ahbc_set_reg(u32 offset, u32 val);

void apm86xxx_reset_system(char *cmd);

/* Enable/Disable PLB to operate at frequency lower than AXI */
void set_plb_lowfreq_mode(int enable);
#endif /* __ASSEMBLY__ */

#endif /* __KERNEL__ */

#endif /* _ASM_POWERPC_APM86xxx_SOC_H */

