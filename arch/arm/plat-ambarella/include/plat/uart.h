/*
 * arch/arm/plat-ambarella/include/plat/uart.h
 *
 * Author: Anthony Ginger <hfjiang@ambarella.com>
 *
 * Copyright (C) 2004-2010, Ambarella, Inc.
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

#ifndef __PLAT_AMBARELLA_UART_H__
#define __PLAT_AMBARELLA_UART_H__

/* ==========================================================================*/
#define DEFAULT_AMBARELLA_UART_MCR	(0)
#if (CHIP_REV == I1)
#define UART_FIFO_SIZE			(16)
#define DEFAULT_AMBARELLA_UART_IER	(UART_IE_ELSI | UART_IE_ERBFI | UART_IE_ETOI)
#define DEFAULT_AMBARELLA_UART_FCR	(UART_FC_FIFOE | UART_FC_RX_2_TO_FULL | UART_FC_TX_EMPTY)
#define DEFAULT_AMBARELLA_UART_FIRST_SEND_NUM	(0)
#elif (CHIP_REV == AMB_S2) || (CHIP_REV == A8) || (CHIP_REV == A7L)
#define UART_FIFO_SIZE			(16)
#define DEFAULT_AMBARELLA_UART_IER	(UART_IE_ELSI | UART_IE_ERBFI | UART_IE_ETOI)
#define DEFAULT_AMBARELLA_UART_FCR	(UART_FC_FIFOE | UART_FC_RX_2_TO_FULL | UART_FC_TX_EMPTY)
#define DEFAULT_AMBARELLA_UART_FIRST_SEND_NUM	(1)
#else
#define UART_FIFO_SIZE			(16)
#define DEFAULT_AMBARELLA_UART_IER	(UART_IE_ELSI | UART_IE_ERBFI)
#define DEFAULT_AMBARELLA_UART_FCR	(UART_FC_FIFOE | UART_FC_RX_2_TO_FULL | UART_FC_TX_EMPTY)
#define DEFAULT_AMBARELLA_UART_FIRST_SEND_NUM	(0)
#endif

/* ==========================================================================*/
#ifndef __ASSEMBLER__

struct ambarella_uart_port_info {
	void *port;	//struct uart_port *
	u32 mcr;
	u32 fcr;
	u32 ier;
	u32 tx_fifo_fix;
	u32 first_send_num;

	void (*stop_tx)(unsigned char __iomem *membase);
	void (*set_pll)(void);
	u32 (*get_pll)(void);
	u32 (*get_ms)(unsigned char __iomem *membase);
};

struct ambarella_uart_platform_info {
	const int total_port_num;
	int registed_port_num;
	struct ambarella_uart_port_info amba_port[UART_INSTANCES];
};

/* ==========================================================================*/
extern struct platform_device ambarella_uart;
extern struct platform_device ambarella_uart1;
extern struct platform_device ambarella_uart2;
extern struct platform_device ambarella_uart3;

extern struct ambarella_uart_platform_info ambarella_uart_ports;

#endif /* __ASSEMBLER__ */
/* ==========================================================================*/

#endif /* __PLAT_AMBARELLA_UART_H__ */

