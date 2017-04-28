/*
 * APM SCU and iPP Messaging Driver header file
 * 
 * Copyright (c) 2010 Applied Micro Circuits Corporation.
 * All rights reserved. Prodyut Hazarika <phazarika@apm.com>
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

#ifndef __AM_IPP_H__
#define __AM_IPP_H__

#include <linux/types.h>
#include <linux/notifier.h>
#include <asm/apm_ipp_csr.h>
#include <asm/apm_ipp_interface.h>

#if defined(CONFIG_APM867xx)
/* Compatible Defines for APM862xx */
#define SOC_STRK_CLKMON_SEL3_MASK 	STRK_CLKMON_SEL3_MASK

#define MPA_GPIO_ADDR			MPA_GPIO_SEL_LO_ADDR
#define MPIC_EXT_IRQ_IN_SEL_WR		DSF_IO_DIAG_SEL_WR
#define GPIO_TYPE_WR			GPIO_OE_WR
#endif /* CONFIG_APM867xx */

#define APM_MPA_REG_OFFSET		0x800
#ifndef CONFIG_APM862xx
#define APM_MPA_PPC0_REG_OFFSET		0x2000
#endif
#define MPA_PPC_CSR_SIZE		0x1000

/* Use Core0 for iPP Messaging in SMP Mode */
#define DEFAULT_SMP_IPPMSG_CPU		0

/* Minimal CPM Registers definition */
#define CPM_REG_PLB_DFSCTRL	0x01
#define CPM_REG_CPU(id)		((id)*0x20)
#define CPM_REG_CPU_DFSCTRL(id)	(CPM_REG_CPU(id) + 0x10)
#define CPM_REG_CPU_CLKCTRL(id)	(CPM_REG_CPU(id) + 0x11)
#define CPM_REG_CPU_PWRCTRL(id)	(CPM_REG_CPU(id) + 0x12)

#define CPM_CMD_SEND		0x80000000
#define CPM_CMD_WR		0x40000000
#define CPM_CMD_RD		0x00000000
#define CPM_CMD_ADDR(reg)	((0x000000ff & (reg)) << 8)
#define CPM_CMD_WRDATA(data)	(0x000000ff & (data))
#define CPM_CMD_INPROGRESS	0x00000001
#define CPM_CMD_RDDATA		0x000000ff

#define CPM_CPU_CLKCTRL_CPUEN_MASK	0x80
#define CPM_CPU_CLKCTRL_PLBEN_MASK	0x40
#define CPM_CPU_CLKCTRL_FPUEN_MASK	0x20
#define CPM_CPU_PWRCTRL_SHUTDOWN_MASK	0x40
#define CPM_CPU_PWRCTRL_SLEEP_MASK	0x10

/* iPP Messaging Interface                               */

/* Events from the iPP Firmware */
#define IPP_FIRMWARE_LOADED		0x0001

/* Register Notifier to be invoked on iPP Fimware event */
int register_ipp_fwload_notifier(struct notifier_block *nb);

/* Unregister Notifier to be invoked on iPP Fimware event */
int unregister_ipp_fwload_notifier(struct notifier_block *nb);

/* Determine if Hardware is an APM86xxx lite version. */
int is_apm86xxx_lite(void);

/* Determine if hardware is an APM862xx variant */
int is_apm862xx(void);

/* Determine if Hardware is an APM863xx variant */
int is_apm863xx(void);

/* Determine if Hardware is an APM867xx variant */
int is_apm867xx(void);

/* Send a u32 word to iPP encoded per apm_ipp_interface.h     */
/* raw data to be sent must be encoded per iPP<->PPC protocol */ 
/* else it will be ignored by iPP                             */
/* msg_param is the additional 32 bytes sent using SCRATCH1/2 */
/* msg_param must be set to IPP_MSG_PARAM_UNUSED if not used  */
int write_ipp_raw_msg(u32 msg_data, u32 msg_param);

/* Send message to an iPP ipp_user_message_handlers             */
/* The ipp_user_message_handlers defined in apm_ipp_interface.h */
/* Any parameters can be left 0 if not used in the user message */

int ipp_send_user_msg(enum ipp_user_message_handlers id, u8 arg1, u8 arg2,
			u8 control_byte, u32 arg3);

/* Send Data Message to iPP (buffer, len) to data message hdlr        */
/* Buffer length must be multiple of word (32 bytes)                   */
/* ipp_data_message_handlers is the iPP data message handler to invoke */
/* Supported iPP data message handlers defined in apm_ipp_interface.h  */
int ipp_send_data_msg(enum ipp_data_message_handlers hdlr,
				void *buf, int len);

/* Send Addr Buffer to iPP (buffer, len) using IPP_ADDR_MSG            */
/* Buffer length must be multiple of word (32 bytes)                   */
/* First word of buffer must equal to length of whole address msg      */
/* ipp_addr_message_handlers is the iPP data message handler to invoke */
/* Supported iPP addr message handlers defined in apm_ipp_interface.h  */
int ipp_send_addr_msg(enum ipp_addr_message_handlers hdlr,
			u32 *addr_buf, int len);

/* Generic Address message to get arbitrary data from SlimPro */
/* identified by index. If success, data is returned in buf   */
int ipp_generic_rx_addrmsg(enum ipp_generic_databuf_index index,
				void *buf, int len);

/* Get SlimPro firmware features as as bit mask */
/* Each Slimpro firmware feature is identified by */
/* a bit in returned unsigned int. The bit defintiions */
/* are defined in apm_ipp_interface.h                  */
unsigned int get_ipp_features(void);

/* Get SlimPro power management statistics using firmware interface */
int get_ipp_pwrstats(struct ipp_pwrmgmt_stats *pstats);

/* Get SlimPro network statistics using firmware interface */
int get_ipp_netstats(struct ipp_net_stats *nstats);

/* Get the address and offset for buffer shared        */
/* between PowerPC and iPP to save power state         */
/* This returns -ENOBUFS if the shared buffer is not   */
/* initialized. Caller must check return value         */
int get_ipp_pwr_state_buf(ipp_pwr_state_t **addr);

/* Notify iPP to save the power state buffer  */
int save_ipp_pwr_state_buf(ipp_pwr_state_t **addr);

/* Notifiy iPP to online a CPU that is clockgated or powered off */
int ipp_online_cpu(u8 cpu);

/* Read CPM register - returns 0 if successful */
/* ret_num: CPM Register                       */
/* reg_val: is valid only if return value is 0 */
int read_cpm_register(u32 reg_num, volatile u32* reg_val);

/* Write CPM register - returns 0 if successful */
/* reg_num: CPM Register                        */
/* reg_val: Value to store in register          */
int write_cpm_register(u32 reg_num, volatile u32 reg_val);

/* Read CPM register that stores CPU divisor        */
/* core: CPU core number                            */
/* div : current CPU divisor use for specified core */
int cpm_get_cpu_div(u32 core, u32 *div);

/* Read CPM register that stores PLB2x divisor */
/* div : current PLB2x divisor                 */
int cpm_get_plb2x_div(u32 *div);

/* Write to CPM register that stores CPU divisor  */
/* core: CPU core number                          */
/* div : CPU divisor to use for specified core    */
int cpm_set_cpu_div(u32 core_mask, u32 div);

/* Write to CPM register that stores PLB2x divisor */
/* div : PLB2x divisor to use                      */
int cpm_set_plb2x_div(u32 div);

/* Reset Control Structure for APM86xxx IP block                   */
/* reg_group: indicate whether to use SCU_SRST or SCU_SRST1 reg    */
/* clken_mask: Mask to enable clock in SCU_CLKEN/CLKEN1 reg: 0 skip*/
/* csr_reset_mask: Mask for SCU_CSR_SRST/SRST1 reg: 0 means NO CSR */
/* reset_mask: Mask for SCU_SRST/SRST1 reg: 0 skip                 */
/* mem_rdy_mask: Mask for SCU_MRDY reg: 0 means not MEM_RDY bit    */
/* ram_shutdown_addr: Module's RAM shutdown addr: NULL to skip     */
#define APM_MPA_REG_OFFSET		0x800
#define APM_GLBL_DIAG_OFFSET            0x7000
#define REG_GROUP_SRST_CLKEN		0
#define REG_GROUP_SRST1_CLKEN1		1	
#define TOGGLE_RESET_FLAG		0
#define ASSERT_RESET_FLAG		1	
#define MEM_RDY_TIMEOUT_COUNT		10000
struct apm86xxx_reset_ctrl {
	int	reset_type;
	int	reg_group;
	u32	clken_mask;
	u32	csr_reset_mask;
	u32	reset_mask;
	u32	mem_rdy_mask;
	u32	__iomem *ram_shutdown_addr;
};

/* PLEASE DON'T CALL __reset_apm86xxx_block directly, use clk APIs    */
/* Reset APM 86xxx block - User mask pass proper values in reset_ctrl */
/* To disable APM 86xxx block, set reset_type == ASSERT_RESET_FLAG    */
/* Example: to reset Ethernet Port1 and Port 2, the values should be */
/* reg_group = REG_GROUP_SRST_CLKEN; */
/* clken_mask = ENET1_F1_MASK | ENET0_F1_MASK; in SCU_CLKEN reg: apm_ipp_csr.h */
/* csr_reset_mask = ENET0_F3_MASK | ENET1_F2_MASK; in SCU_CSR_SRST reg         */
/* reset_mask = ENET0_MASK | ENET1_MASK; in SCU_SRST reg: apm_ipp_csr.h        */
/* mem_rdy_mask = ENET0_F2_MASK; in SCU_MRDY reg: apm_ipp_csr.h                */
/* ram_shutdown_addr = Virt addr of ENET_CFG_MEM_RAM_SHUTDOWN reg:mb_enet_csr.h*/
int __reset_apm86xxx_block(struct apm86xxx_reset_ctrl *reset_ctrl);

struct apm86xxx_pll_ctrl {
	u16	clkf_val;
	u16	clkod_val;
	u16	clkr_val;
	u16	bwadj_val;
};

/* Get iPP VIRQ number */
int get_ipp_irq(void);

/* Get iPP lite GPIO VIRQ number */
int get_ipp_gpio_irq(void);

/* Get the board/platform specific Wakeup GPIO Mask */
u32 get_gpio_wakeup_mask(void);

/* Get the board/platform specific Wakeup GPIO Level Mask */
u32 get_gpio_wakeup_level_mask(void);

/* Remove APM86xxx memory block out of shutdown               */
/* ram_shutdown_addr is the virtual address managed by driver */
/* mem_rdy_mask mask is the corresponding bit in SCU_MRDY reg */
int apm86xxx_disable_mem_shutdown(u32 __iomem *ram_shutdown_addr,
					u32 mem_rdy_mask);

/* WRITE SCU register - returns 0 if successful      */
/* offfset: SCU reg offset defined in apm_ipp_csr.h */
/* Use with caution: meant for debugging only       */
int apm86xxx_write_scu_reg(u32 offset, u32 val);

/* Read SCU register - returns 0 if successful      */
/* offfset: SCU reg offset defined in apm_ipp_csr.h */
/* val is valid only if return value is 0           */
/* Use with caution: meant for debugging only       */
int apm86xxx_read_scu_reg(u32 offset, u32 *val);

/* Helper function to set SCU register bit mask */
int apm86xxx_write_scu_reg_setmask(u32 offset, u32 mask);

/* Helper function to clear SCU register bit mask */
int apm86xxx_write_scu_reg_clrmask(u32 offset, u32 mask);

/* Enable specified CPU core clocks */
int apm86xxx_enable_ppc_clocks(unsigned int cpu);

/* Disable specified CPU core clocks */
int apm86xxx_disable_ppc_clocks(unsigned int cpu);

/* Power down specifed CPU core */
int apm86xxx_powerdown_core(unsigned int cpu);

/* Power up specified CPU core */
int apm86xxx_powerup_core(unsigned int cpu);

/* Read MPA register - returns 0 if successful      */
/* offfset: MPA reg offset defined in apm_ipp_csr.h */
/* val is valid only if return value is 0           */
/* Use with caution: meant for debugging only       */
int apm86xxx_read_mpa_reg(u32 offset, u32 *val);

/* WRITE MPA register - returns 0 if successful      */
/* offfset: MPA reg offset defined in apm_ipp_csr.h */
/* Use with caution: meant for debugging only       */
int apm86xxx_write_mpa_reg(u32 offset, u32 val);

/* Helper function to set MPA register bit mask */
int apm86xxx_write_mpa_reg_setmask(u32 offset, u32 mask);

/* Helper function to clear MPA register bit mask */
int apm86xxx_write_mpa_reg_clrmask(u32 offset, u32 mask);

#define APM86xxx_REFCLK_FREQ		25000000
enum
{
	APM86xxx_CPU_PLL_FREQ	= 0x0,
	APM86xxx_PLB_FREQ,
	APM86xxx_TRACE_PPC_FREQ,
	APM86xxx_SOC_PLL_FREQ,
	APM86xxx_AXI_FREQ,
	APM86xxx_AHB_FREQ,
	APM86xxx_IAHB_FREQ,
	APM86xxx_APB_FREQ,
	APM86xxx_PCIE_REFCLK_FREQ,
	APM86xxx_CRYPTO_FREQ,
	APM86xxx_EBUS_FREQ,
	APM86xxx_SDIO_FREQ,
	APM86xxx_DDR_PLL_FREQ,
	APM86xxx_DDR2_FREQ,
	APM86xxx_DDR1_FREQ,
	APM86xxx_MAX
};

/* PLEASE DON'T CALL __apm86xxx_get_freq directly, use clk interface */
/* Low level API to get clock/PLL frequencies */
int __apm86xxx_get_freq(int id, u32 *freq);

/* PLEASE DON'T CALL __apm86xxx_set_freq directly, use clk interface */
int __apm86xxx_set_freq(int id, unsigned long rate);

/* Init the Linux Clock Interface */
int apm_clk_init(void);

/* API to show APM Clock tree */
ssize_t apm_show_clktree(char *buf);

/* API to set CPM to local/Global mode for CPUIDx */
/* mode: IPP_LOCAL_SLEEP_MODE (for local mode)    */
/* mode: IPP_GLOBAL_SLEEP_MODE (for global mode)  */

int set_cpu_cpm_mode(int cpuid, int mode);

/* PLEASE DON'T SET LED0      */
/* LED0 is hardware fault LED */
enum
{
	APM86xxx_LED0 = 0x1<<0,
	APM86xxx_LED1 = 0x1<<1,
	APM86xxx_LED2 = 0x1<<2,
	APM86xxx_LED3 = 0x1<<3,
	APM86xxx_LED4 = 0x1<<4
};

/* Set APM86xxx LED */
void apm86xxx_set_led(u32 led_mask);

/* Clear APM86xxx LED */
void apm86xxx_clear_led(u32 led_mask);

#ifdef CONFIG_APM862xx
/* Ethernet and LCD PLLs present only in APM862xx SoC family      */
/* To reconfigure PLL, user must shutdown PLL and pass new values */
/* If PLL already configured, enable_xxx_pll returns safely       */
/* Safe to call enable_xxx_pll multiple times                     */
/* Enable APM 86xxx Ethernet PLL - User must pass proper PLL values in eth_pll */
int enable_eth_pll(struct apm86xxx_pll_ctrl *eth_pll);
/* Enable APM 86xxx LCD PLL - User must pass proper PLL values in eth_pll */
int enable_lcd_pll(struct apm86xxx_pll_ctrl *lcd_pll);

/* Shutdown Ethernet PLL */
void disable_eth_pll(void);
/* Shutdown LCD PLL */
void disable_lcd_pll(void);
#else
#define enable_eth_pll(x)
#define enable_lcd_pll(x)
#define disable_eth_pll()
#define disable_lcd_pll(x)
#endif /* CONFIG_APM862xx */

#endif /* __AM_IPP_H__ */
