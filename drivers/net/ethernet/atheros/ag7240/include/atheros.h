/*
 * vim: tabstop=8 : noexpandtab
 */
#ifndef _ATHEROS_H
#define _ATHEROS_H

#include <asm/mach-ath79/ath79.h>
#include <asm/mach-ath79/ar71xx_regs.h>
#include <../ath79/common.h>

#include <asm/addrspace.h>
#include <generated/autoconf.h>
#include <gpio.h>

#ifndef __ASSEMBLY__

#define ATH_CONSOLE_BAUD	115200
#define ATH_MAX_PCI_BUS		1

#ifdef CONFIG_ATH_EMULATION
#define is_emu()	(1)
#else
#define is_emu()	(0)
#endif

#define ath_arch_init_irq() /* nothing */

extern uint32_t ath_cpu_freq, ath_ahb_freq, ath_ddr_freq,
		ath_ref_freq, ath_uart_freq;

void ath_sys_frequency(void);

typedef unsigned int ath_reg_t;

/*
 * Address map
 */
#define ATH_PCI_MEM_BASE(x)		0x10000000	/* 128M */
#define ATH_APB_BASE			0x18000000	/* 384M */
#define ATH_GE0_BASE			0x19000000	/* 16M */
#define ATH_GE1_BASE			0x1a000000	/* 16M */
#define ATH_USB_OHCI_BASE		0x1b000000
#define ATH_USB_EHCI_BASE		0x1b000000
#define ATH_SPI_BASE			0x1f000000
#define ATH_NAND_FLASH_BASE		0x1b000000u


/*
 * The IPs. Connected to CPU (hardware IP's; the first two are software)
 */
#define ATH_CPU_IRQ_GE0			ATH79_CPU_IRQ(4)
#define ATH_CPU_IRQ_GE1			ATH79_CPU_IRQ(5)

/*
 * Chip reset bit definitions 
 */

#define ATHR_RESET_EXTERNAL               (1 << 28)
#define ATHR_RESET_WMAC                   (1 << 27)
#define ATHR_RESET_FULL_CHIP              (1 << 24)
#define ATHR_RESET_CPU_NMI                (1 << 21)
#define ATHR_RESET_CPU_COLD_RESET_MASK    (1 << 20)
#define ATHR_RESET_DDR                    (1 << 16)
#define ATHR_RESET_GE1_MAC                (1 << 13)
#define ATHR_RESET_GE1_MDIO               (1 << 23)
#define ATHR_RESET_GE1_PHY                (1 << 12) /* Not valid */
#define ATHR_RESET_PCIE_PHY_SERIAL        (1 << 10)
#define ATHR_RESET_GE0_MAC                (1 << 9)
#define ATHR_RESET_GE0_MDIO               (1 << 22)
#define ATHR_RESET_GE0_PHY                (1 << 8) /* Switch reset */
#define ATHR_RESET_PCIE_PHY               (1 << 7)
#define ATHR_RESET_PCIE                   (1 << 6)
#define ATHR_RESET_USB_HOST               (1 << 5)
#define ATHR_RESET_USB_OHCI_DLL           (1 << 3)

/*
 * gpio configs
 */
#define ATH_GPIO_OE			ATH_GPIO_BASE+0x0
#define ATH_GPIO_IN			ATH_GPIO_BASE+0x4
#define ATH_GPIO_OUT			ATH_GPIO_BASE+0x8
#define ATH_GPIO_SET			ATH_GPIO_BASE+0xc
#define ATH_GPIO_CLEAR			ATH_GPIO_BASE+0x10
#define ATH_GPIO_INT_ENABLE		ATH_GPIO_BASE+0x14
#define ATH_GPIO_INT_TYPE		ATH_GPIO_BASE+0x18
#define ATH_GPIO_INT_POLARITY		ATH_GPIO_BASE+0x1c
#define ATH_GPIO_INT_PENDING		ATH_GPIO_BASE+0x20
#define ATH_GPIO_INT_MASK		ATH_GPIO_BASE+0x24
#define ATH_GPIO_IN_ETH_SWITCH_LED	ATH_GPIO_BASE+0x28
#define ATH_GPIO_OUT_FUNCTION0		ATH_GPIO_BASE+0x2c
#define ATH_GPIO_OUT_FUNCTION1		ATH_GPIO_BASE+0x30
#define ATH_GPIO_OUT_FUNCTION2		ATH_GPIO_BASE+0x34
#define ATH_GPIO_OUT_FUNCTION3		ATH_GPIO_BASE+0x38
#define ATH_GPIO_OUT_FUNCTION4		ATH_GPIO_BASE+0x3c
#define ATH_GPIO_OUT_FUNCTION5		ATH_GPIO_BASE+0x40
#define ATH_GPIO_IN_ENABLE0		ATH_GPIO_BASE+0x44
#define ATH_GPIO_IN_ENABLE1		ATH_GPIO_BASE+0x48
#define ATH_GPIO_IN_ENABLE2		ATH_GPIO_BASE+0x4c
#define ATH_GPIO_IN_ENABLE3		ATH_GPIO_BASE+0x50
#define ATH_GPIO_IN_ENABLE4		ATH_GPIO_BASE+0x54
#define ATH_GPIO_IN_ENABLE5		ATH_GPIO_BASE+0x58
#define ATH_GPIO_IN_ENABLE6		ATH_GPIO_BASE+0x5c
#define ATH_GPIO_IN_ENABLE7		ATH_GPIO_BASE+0x60
#define ATH_GPIO_IN_ENABLE8		ATH_GPIO_BASE+0x64
#define ATH_GPIO_IN_ENABLE9		ATH_GPIO_BASE+0x68
#define ATH_GPIO_FUNCTIONS		ATH_GPIO_BASE+0x6c

#define ATHR_SWITCH_CLK_SPARE		     0x18050024


#define ath_reg_rd(_phys)	(*(volatile ath_reg_t *)KSEG1ADDR(_phys))

#define ath_reg_wr_nf(_phys, _val) \
	((*(volatile ath_reg_t *)KSEG1ADDR(_phys)) = (_val))

#define ath_reg_wr(_phys, _val) do {	\
	ath_reg_wr_nf(_phys, _val);	\
	ath_reg_rd(_phys);		\
} while(0)

#define ath_reg_rmw_set(_reg, _mask)	do {			\
	ath_reg_wr((_reg), (ath_reg_rd((_reg)) | (_mask)));	\
	ath_reg_rd((_reg));					\
} while(0)

#define ath_reg_rmw_clear(_reg, _mask) do {			\
	ath_reg_wr((_reg), (ath_reg_rd((_reg)) & ~(_mask)));	\
	ath_reg_rd((_reg));					\
} while(0)

#define athr_reg_rd(_phys)	ath_reg_rd(_phys)

#define athr_reg_wr_nf(_phys, _val) ath_reg_wr_nf(_phys, _val)

#define athr_reg_wr(_phys, _val) ath_reg_wr(_phys, _val)

#define athr_reg_rmw_set(_reg, _mask)  ath_reg_rmw_set(_reg, _mask)

#define athr_reg_rmw_clear(_reg, _mask) ath_reg_rmw_clear(_reg, _mask)

#define MY_WRITE(y, z)		((*((volatile u32*)(y))) = z)

#define UART_READ(y)		ath_reg_rd((ATH_UART_BASE+y))
#define UART_WRITE(x, z)	ath_reg_wr((ATH_UART_BASE+x), z)

#endif /* __ASSEMBLY__ */

#define REG_OFFSET		4

#define OFS_RCV_BUFFER		(0 * REG_OFFSET)
#define OFS_TRANS_HOLD		(0 * REG_OFFSET)
#define OFS_SEND_BUFFER		(0 * REG_OFFSET)
#define OFS_INTR_ENABLE		(1 * REG_OFFSET)
#define OFS_INTR_ID		(2 * REG_OFFSET)
#define OFS_DATA_FORMAT		(3 * REG_OFFSET)
#define OFS_LINE_CONTROL	(3 * REG_OFFSET)
#define OFS_MODEM_CONTROL	(4 * REG_OFFSET)
#define OFS_RS232_OUTPUT	(4 * REG_OFFSET)
#define OFS_LINE_STATUS		(5 * REG_OFFSET)
#define OFS_MODEM_STATUS	(6 * REG_OFFSET)
#define OFS_RS232_INPUT		(6 * REG_OFFSET)
#define OFS_SCRATCH_PAD		(7 * REG_OFFSET)

#define OFS_DIVISOR_LSB		(0 * REG_OFFSET)
#define OFS_DIVISOR_MSB		(1 * REG_OFFSET)

#define RST_GENERAL_BASE		0xb8060000
#define DDR_CONFIG_BASE			0xb8000000
#define DDRMON_CTL_OFFSET		0x00e8
#define DDRMON_GNT_OFFSET		0x00f8

#define RST_REVISION_ID_ADDRESS                                      0x18060090

#define ATH_REV_ID			RST_REVISION_ID_ADDRESS
#define ATH_REV_ID_MASK_MAJOR		0xfff0u
#define ATH_REV_ID_MASK_MINOR		0xfu
#define ATH_REV_ID_MASK			\
	(ATH_REV_ID_MASK_MAJOR | ATH_REV_ID_MASK_MINOR)

#if 0
#ifdef CONFIG_SOC_AR724X
#include <ar7240.h>
#endif

#ifdef CONFIG_SOC_AR934X
#include <934x.h>
#endif

#if defined(CONFIG_MACH_QCA955x)
#include <955x.h>
#endif
#endif
// temporary hacks:

#define is_s17() 0
#define is_f1e() 0
#define is_s16() 0
#define is_vir_phy() 0
#define is_f2e() 0
#define is_s26() 0
#define is_s27() 0
#define is_8021() 0

#define ATH_EEPROM_GE0_MAC_ADDR 0xbfff1000
#define ATH_EEPROM_GE1_MAC_ADDR 0xbfff1006

#define ATH_PLL_ETH_XMII_CONFIG		0x2c

#endif /* _ATHEROS_H */

