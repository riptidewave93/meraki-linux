/*
 *  Atheros AR7XXX/AR9XXX SoC compressed boot debug support
 *
 *  Copyright (c) 2013, Meraki Inc.
 *  Author: Kevin Paul Herbert <kph@meraki.net>
 *
 *  This code is based on arch/mips/ath79/early_printk.c.
 */

#include <linux/types.h>
#include <linux/serial_reg.h>
#include <linux/init.h>

#include <asm/addrspace.h>

#include <asm/mach-ath79/ath79.h>
#include <asm/mach-ath79/ar71xx_regs.h>
#include <asm/mach-ath79/ar933x_uart.h>

static void (*_serial_putchar)(char c);

static inline void serial_putchar_wait(volatile u32 *reg,
				       u32 mask, u32 val)
{
	u32 t;

	do {
		t = *reg;
		if ((t & mask) == val)
			break;
	} while (1);
}

static void serial_putchar_ar71xx(char ch)
{
	volatile u32 *base = (volatile u32 *)(KSEG1ADDR(AR71XX_UART_BASE));

	serial_putchar_wait(base + UART_LSR, UART_LSR_THRE, UART_LSR_THRE);
	*(base + UART_TX) = ch;
	serial_putchar_wait(base + UART_LSR, UART_LSR_THRE, UART_LSR_THRE);
}

static void serial_putchar_ar933x(char ch)
{
	volatile u32 *base = (volatile u32 *)(KSEG1ADDR(AR933X_UART_BASE));

	serial_putchar_wait(base + (AR933X_UART_DATA_REG >> 2),
			  AR933X_UART_DATA_TX_CSR,
			  AR933X_UART_DATA_TX_CSR);
	*(base + (UART_TX >> 2)) = (AR933X_UART_DATA_TX_CSR | ch);
	serial_putchar_wait(base + (AR933X_UART_DATA_REG >> 2),
			  AR933X_UART_DATA_TX_CSR,
			  AR933X_UART_DATA_TX_CSR);
}

static void serial_putchar_dummy(char ch)
{
	/* nothing to do */
}


static void serial_putchar_init(void)
{
	u32 id;

	id = *(u32 *)CKSEG1ADDR(AR71XX_RESET_BASE +
				AR71XX_RESET_REG_REV_ID);
	id &= REV_ID_MAJOR_MASK;

	switch (id) {
	case REV_ID_MAJOR_AR71XX:
	case REV_ID_MAJOR_AR7240:
	case REV_ID_MAJOR_AR7241:
	case REV_ID_MAJOR_AR7242:
	case REV_ID_MAJOR_AR913X:
	case REV_ID_MAJOR_AR9341:
	case REV_ID_MAJOR_AR9342:
	case REV_ID_MAJOR_AR9344:
	case REV_ID_MAJOR_QCA9556:
	case REV_ID_MAJOR_QCA9558:
		_serial_putchar = serial_putchar_ar71xx;
		break;

	case REV_ID_MAJOR_AR9330:
	case REV_ID_MAJOR_AR9331:
		_serial_putchar = serial_putchar_ar933x;
		break;

	default:
		_serial_putchar = serial_putchar_dummy;
		break;
	}
}

void putc(char c)
{
	if (!_serial_putchar)
		serial_putchar_init();

	_serial_putchar(c);
}
