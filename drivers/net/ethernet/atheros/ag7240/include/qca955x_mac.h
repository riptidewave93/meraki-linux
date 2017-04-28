/*
 * Copyright (c) 2008, Atheros Communications Inc.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef _QCA_955X_H
#define _QCA_955X_H

#include "atheros.h"

/* Fifo Values */
#define AMCXFIF_CFG_1_VAL		0x10ffff
#define AMCXFIF_CFG_2_VAL	        0x015500aa
#define AMCXFIF_CFG_4_VAL		0x3ffff
#define AMCXFIF_CFG_3_VAL		0x1f00140
#define TXFIFO_TH_VAL			0x01d80160
#define TXFIFO_TH_HD_VAL		0x00880060
#define AMCXFIF_CFG_3_HD_VAL		0x00f00040
#define AMCXFIF_CFG_0_VAL		0x1f00
#define ATH_MDIO_PHY_ADDR 		0x7
#define ATH_MDC_GPIO_PIN  		11
#define ATH_MDIO_GPIO_PIN 		18

#define ATH_GPIO_OUT_FUNC_ADDR          GPIO_OUT_FUNCTION4_ADDRESS
#define ATH_GPIO_OUT_FUNC_EN_MASK       GPIO_OUT_FUNCTION4_ENABLE_GPIO_18_MASK
#define ATH_GPIO_OUT_FUNC_EN_SET        GPIO_OUT_FUNCTION4_ENABLE_GPIO_18_SET
#define ATH_MII_EXT_MDI                 1
                

#define MII_UNKNOWN			-1
#define MII_PHY_UNKNOWN			-1

#define CFG_ATHR_GMAC0_MII                   ATHR_RGMII
#define CFG_ATHR_GMAC0_MII_PHY               ATHR_AR8033_PHY

#define CFG_ATHR_GMAC1_MII		     MII_UNKNOWN
#define CFG_ATHR_GMAC1_MII_PHY		     MII_PHY_UNKNOWN

/* GPIO defines */

#define ATHR_GPIO_FUNCTIONS                  ATH_GPIO_FUNCTIONS
#define ATHR_GPIO_OUT_FUNCTION1              ATH_GPIO_OUT_FUNCTION1
#define ATHR_GPIO_OUT_FUNCTION4              ATH_GPIO_OUT_FUNCTION4
#define ATHR_GPIO_OUT_FUNCTION5              ATH_GPIO_OUT_FUNCTION5
#define ATHR_GPIO_OE			     ATH_GPIO_OE
#define ATHR_GPIO_OUT			     ATH_GPIO_OUT
#define ATHR_GPIO_INT_ENABLE                 ATH_GPIO_INT_ENABLE
#define ATHR_GPIO_INT_TYPE                   ATH_GPIO_INT_TYPE

#if defined(AP136_BOOTROM_HOST)
#define set_gmac_delay()  do{  ath_reg_wr(ETH_XMII_ADDRESS, \
                                 ETH_XMII_TX_INVERT_SET(1) \
                               | ETH_XMII_RX_DELAY_SET(2)  \
                               | ETH_XMII_TX_DELAY_SET(1)  \
                               | ETH_XMII_GIGE_SET(1) ); }while(0)
#else
#define set_gmac_delay()  do{ }while(0)
#endif

extern uint32_t ath_ahb_freq;
int qca955x_gmac_attach(void *arg);
void serdes_pll_lock_detect_st(void);
#endif //_ATHR_934x_H
