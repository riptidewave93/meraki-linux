/*
 * APM86xxx SPI driver
 *
 * Copyright (c) 2010 Applied Micro Circuits Corporation.
 * All rights reserved. Anup Patel <apatel@apm.com>
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
 * @file apm86xxx_spi.h
 *
 * This file contains the SPI driver for APM86xxx SOCs of AppliedMicro Inc.
 */

#ifndef _APM86XXX_SPI_H__
#define _APM86XXX_SPI_H__

/** SPI mode bits understood by this driver: */
#define APM86xxx_COMPATIBLE_SPI_MODEBITS	(SPI_CPOL | SPI_CPHA | SPI_CS_HIGH | SPI_LSB_FIRST)

/** APM86xxx settings */
/** Controller Signature */
#define APM86xxx_SPI_SIGNATURE			0x00000A5
/** FIFO depth of SPI controller */
#define APM86xxx_MAX_FIFO_DEPTH			8
/** Maximum number of slaves */
#define APM86xxx_CONFIG_MAX_SLAVE		4
/** Number of locations for fifo data register */
#define APM86xxx_DR_NUM_LOCATIONS 		40

/** APM86xxx control register default values */
#define APM86xxx_CTRL0_CFS_VAL			0x0 /**< Default value */
#define APM86xxx_CTRL0_SRL_VAL			0x0 /**< Default value */
#define APM86xxx_CTRL0_SLV_OE_VAL		0x0 /**< Default value */
#define APM86xxx_CTRL0_TMOD_TXRX_VAL		0x0 /**< Transmit & Receive mode */
#define APM86xxx_CTRL0_TMOD_TX_VAL		0x1 /**< Transmit only mode */
#define APM86xxx_CTRL0_TMOD_RX_VAL		0x2 /**< Receive only mode */
#define APM86xxx_CTRL0_TMOD_EEPROM_VAL		0x3 /**< EEPROM read mode */
#define APM86xxx_CTRL0_SCPOL_VAL		0x0 /**< Polarity of serial clock */
#define APM86xxx_CTRL0_SCPH_VAL			0x0 /**< Phase of serial clock */
#define APM86xxx_CTRL0_FRF_VAL			0x0 /**< Motorola SPI frame format */
#define APM86xxx_CTRL0_DFS_VAL			0x7 /**< Data frame size (8 bits) */

/** APM86xxx interrupt mask register value */
#define APM86xxx_IMR_VAL			0x0

/** APM86xxx transmit FIFO size */
#define APM86xxx_TX_FIFO_SIZE			APM86xxx_MAX_FIFO_DEPTH

/** APM86xxx transmit threshold value */
#define APM86xxx_TXFTLR_TFT_VAL			0x7

/** APM86xxx receive FIFO size */
#define APM86xxx_RX_FIFO_SIZE			APM86xxx_MAX_FIFO_DEPTH

/** APM86xxx receive threshold value */
#define APM86xxx_RXFTLR_RFT_VAL			0x0

/** APM86xxx tunning delay count */
#define APM86xxx_TUNNING_DELAY			0x10000

/** APM86xxx SPI slave frequency */
#define APM86xxx_SPI_SPEED_HZ			4000000

struct apm86xxx_spi_regs {
	u32 ctrlr0;
	u32 ctrlr1;
	u32 ssienr;
	u32 mwcr;
	u32 ser;
	u32 baudr;
	u32 txftlr;
	u32 rxftlr;
	u32 txflr;
	u32 rxflr;
	u32 sr;
	u32 imr;
	u32 isr;
	u32 risr;
	u32 txoicr;
	u32 rxoicr;
	u32 rxuicr;
	u32 msticr;
	u32 icr;
	u32 dmacr;
	u32 dmatdlr;
	u32 dmardlr;
	u32 idr;
	u32 ssi_comp_version;
	u8 dr[APM86xxx_DR_NUM_LOCATIONS*4];
}__attribute__ ((packed));

struct apm86xxx_spi_controller_state {
	u8 activated;
	u8 mode;
	u8 chip_select;
	u8 bits_per_word;
	u8 xfer_mode;
	u32 speed_hz;
};

struct apm86xxx_spi_controller {
	spinlock_t lock;
	u8 busy;

	s16 bus_num;
	u8 bits_per_word;
	unsigned long sysclk;

	struct of_device *ofdev;

	struct apm86xxx_spi_regs __iomem *regs;
	unsigned int irq;

	struct workqueue_struct *xfer_workqueue;
	struct work_struct xfer_work;
	struct list_head xfer_queue;

	spinlock_t state_lock;
	struct apm86xxx_spi_controller_state state;
};

/*    Address of APM86xxx_SPI Registers */
#define CTRLR0_ADDR                                                  0x00000000
#define CTRLR0_DEFAULT                                               0x00000007
#define CTRLR1_ADDR                                                  0x00000004
#define CTRLR1_DEFAULT                                               0x00000000
#define SSIENR_ADDR                                                  0x00000008
#define SSIENR_DEFAULT                                               0x00000000
#define MWCR_ADDR                                                    0x0000000c
#define MWCR_DEFAULT                                                 0x00000000
#define SER_ADDR                                                     0x00000010
#define SER_DEFAULT                                                  0x00000000
#define BAUDR_ADDR                                                   0x00000014
#define BAUDR_DEFAULT                                                0x00000000
#define TXFTLR_ADDR                                                  0x00000018
#define TXFTLR_DEFAULT                                               0x00000000
#define RXFTLR_ADDR                                                  0x0000001c
#define RXFTLR_DEFAULT                                               0x00000000
#define TXFLR_ADDR                                                   0x00000020
#define TXFLR_DEFAULT                                                0x00000000
#define RXFLR_ADDR                                                   0x00000024
#define RXFLR_DEFAULT                                                0x00000000
#define SR_ADDR                                                      0x00000028
#define SR_DEFAULT                                                   0x00000006
#define IMR_ADDR                                                     0x0000002c
#define IMR_DEFAULT                                                  0x0000003f
#define ISR_ADDR                                                     0x00000030
#define ISR_DEFAULT                                                  0x00000000
#define RISR_ADDR                                                    0x00000034
#define RISR_DEFAULT                                                 0x00000000
#define TXOICR_ADDR                                                  0x00000038
#define TXOICR_DEFAULT                                               0x00000000
#define RXOICR_ADDR                                                  0x0000003c
#define RXOICR_DEFAULT                                               0x00000000
#define RXUICR_ADDR                                                  0x00000040
#define RXUICR_DEFAULT                                               0x00000000
#define MSTICR_ADDR                                                  0x00000044
#define MSTICR_DEFAULT                                               0x00000000
#define ICR_ADDR                                                     0x00000048
#define ICR_DEFAULT                                                  0x00000000
#define IDR_ADDR                                                     0x00000058
#define IDR_DEFAULT                                                  0x000000a5
#define VIDR_ADDR                                                    0x0000005c
#define VIDR_DEFAULT                                                 0x00000000
#define DR_LOW_ADDR                                                  0x00000060
#define DR_LOW_DEFAULT                                               0x00000000
#define DR_HIGH_ADDR                                                 0x000000fc
#define DR_HIGH_DEFAULT                                              0x00000000

/*	Register CTRLR0	*/ 
/*	 Fields CFS	 */
#define CFS0_WIDTH                                                            4
#define CFS0_SHIFT                                                           12
#define CFS0_MASK                                                    0x0000f000
#define CFS0_RD(src)                                 (((src) & 0x0000f000)>>12)
#define CFS0_WR(src)                            (((u32)(src)<<12) & 0x0000f000)
#define CFS0_SET(dst,src) \
                      (((dst) & ~0x0000f000) | (((u32)(src)<<12) & 0x0000f000))
/*	 Fields SRL	 */
#define SRL0_WIDTH                                                            1
#define SRL0_SHIFT                                                           11
#define SRL0_MASK                                                    0x00000800
#define SRL0_RD(src)                                 (((src) & 0x00000800)>>11)
#define SRL0_WR(src)                            (((u32)(src)<<11) & 0x00000800)
#define SRL0_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*	 Fields SLV_OE	 */
#define SLV_OE0_WIDTH                                                         1
#define SLV_OE0_SHIFT                                                        10
#define SLV_OE0_MASK                                                 0x00000400
#define SLV_OE0_RD(src)                              (((src) & 0x00000400)>>10)
#define SLV_OE0_WR(src)                         (((u32)(src)<<10) & 0x00000400)
#define SLV_OE0_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields TMOD	 */
#define TMOD0_WIDTH                                                           2
#define TMOD0_SHIFT                                                           8
#define TMOD0_MASK                                                   0x00000300
#define TMOD0_RD(src)                                 (((src) & 0x00000300)>>8)
#define TMOD0_WR(src)                            (((u32)(src)<<8) & 0x00000300)
#define TMOD0_SET(dst,src) \
                       (((dst) & ~0x00000300) | (((u32)(src)<<8) & 0x00000300))
/*	 Fields SCPOL	 */
#define SCPOL0_WIDTH                                                          1
#define SCPOL0_SHIFT                                                          7
#define SCPOL0_MASK                                                  0x00000080
#define SCPOL0_RD(src)                                (((src) & 0x00000080)>>7)
#define SCPOL0_WR(src)                           (((u32)(src)<<7) & 0x00000080)
#define SCPOL0_SET(dst,src) \
                       (((dst) & ~0x00000080) | (((u32)(src)<<7) & 0x00000080))
/*	 Fields SCPH	 */
#define SCPH0_WIDTH                                                           1
#define SCPH0_SHIFT                                                           6
#define SCPH0_MASK                                                   0x00000040
#define SCPH0_RD(src)                                 (((src) & 0x00000040)>>6)
#define SCPH0_WR(src)                            (((u32)(src)<<6) & 0x00000040)
#define SCPH0_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*	 Fields FRF	 */
#define FRF0_WIDTH                                                            2
#define FRF0_SHIFT                                                            4
#define FRF0_MASK                                                    0x00000030
#define FRF0_RD(src)                                  (((src) & 0x00000030)>>4)
#define FRF0_WR(src)                             (((u32)(src)<<4) & 0x00000030)
#define FRF0_SET(dst,src) \
                       (((dst) & ~0x00000030) | (((u32)(src)<<4) & 0x00000030))
/*	 Fields DFS	 */
#define DFS0_WIDTH                                                            4
#define DFS0_SHIFT                                                            0
#define DFS0_MASK                                                    0x0000000f
#define DFS0_RD(src)                                     (((src) & 0x0000000f))
#define DFS0_WR(src)                                (((u32)(src)) & 0x0000000f)
#define DFS0_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register CTRLR1	*/ 
/*	 Fields NDF	 */
#define NDF1_WIDTH                                                           16
#define NDF1_SHIFT                                                            0
#define NDF1_MASK                                                    0x0000ffff
#define NDF1_RD(src)                                     (((src) & 0x0000ffff))
#define NDF1_WR(src)                                (((u32)(src)) & 0x0000ffff)
#define NDF1_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register SSIENR	*/ 
/*	 Fields SSI_EN	 */
#define SSI_EN_WIDTH                                                          1
#define SSI_EN_SHIFT                                                          0
#define SSI_EN_MASK                                                  0x00000001
#define SSI_EN_RD(src)                                   (((src) & 0x00000001))
#define SSI_EN_WR(src)                              (((u32)(src)) & 0x00000001)
#define SSI_EN_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register MWCR	*/ 
/*	 Fields MHS	 */
#define MHS_WIDTH                                                             1
#define MHS_SHIFT                                                             2
#define MHS_MASK                                                     0x00000004
#define MHS_RD(src)                                   (((src) & 0x00000004)>>2)
#define MHS_WR(src)                              (((u32)(src)<<2) & 0x00000004)
#define MHS_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields MDD	 */
#define MDD_WIDTH                                                             1
#define MDD_SHIFT                                                             1
#define MDD_MASK                                                     0x00000002
#define MDD_RD(src)                                   (((src) & 0x00000002)>>1)
#define MDD_WR(src)                              (((u32)(src)<<1) & 0x00000002)
#define MDD_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields MWMOD	 */
#define MWMOD_WIDTH                                                           1
#define MWMOD_SHIFT                                                           0
#define MWMOD_MASK                                                   0x00000001
#define MWMOD_RD(src)                                    (((src) & 0x00000001))
#define MWMOD_WR(src)                               (((u32)(src)) & 0x00000001)
#define MWMOD_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register SER	*/ 
/*	 Fields SER	 */
#define SER_WIDTH                                                             4
#define SER_SHIFT                                                             0
#define SER_MASK                                                     0x0000000f
#define SER_RD(src)                                      (((src) & 0x0000000f))
#define SER_WR(src)                                 (((u32)(src)) & 0x0000000f)
#define SER_SET(dst,src) (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register BAUDR	*/ 
/*	 Fields SCKDV	 */
#define SCKDV_WIDTH                                                          16
#define SCKDV_SHIFT                                                           0
#define SCKDV_MASK                                                   0x0000ffff
#define SCKDV_RD(src)                                    (((src) & 0x0000ffff))
#define SCKDV_WR(src)                               (((u32)(src)) & 0x0000ffff)
#define SCKDV_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register TXFTLR	*/ 
/*	 Fields TFT	 */
#define TFT_WIDTH                                                             3
#define TFT_SHIFT                                                             0
#define TFT_MASK                                                     0x00000007
#define TFT_RD(src)                                      (((src) & 0x00000007))
#define TFT_WR(src)                                 (((u32)(src)) & 0x00000007)
#define TFT_SET(dst,src) (((dst) & ~0x00000007) | (((u32)(src)) & 0x00000007))

/*	Register RXFTLR	*/ 
/*	 Fields RFT	 */
#define RFT_WIDTH                                                             3
#define RFT_SHIFT                                                             0
#define RFT_MASK                                                     0x00000007
#define RFT_RD(src)                                      (((src) & 0x00000007))
#define RFT_WR(src)                                 (((u32)(src)) & 0x00000007)
#define RFT_SET(dst,src) (((dst) & ~0x00000007) | (((u32)(src)) & 0x00000007))

/*	Register TXFLR	*/ 
/*	 Fields TXTFL	 */
#define TXTFL_WIDTH                                                           4
#define TXTFL_SHIFT                                                           0
#define TXTFL_MASK                                                   0x0000000f
#define TXTFL_RD(src)                                    (((src) & 0x0000000f))
#define TXTFL_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register RXFLR	*/ 
/*	 Fields RXTFL	 */
#define RXTFL_WIDTH                                                           4
#define RXTFL_SHIFT                                                           0
#define RXTFL_MASK                                                   0x0000000f
#define RXTFL_RD(src)                                    (((src) & 0x0000000f))
#define RXTFL_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register SR	*/ 
/*	 Fields DCOL	 */
#define DCOL_WIDTH                                                            1
#define DCOL_SHIFT                                                            6
#define DCOL_MASK                                                    0x00000040
#define DCOL_RD(src)                                  (((src) & 0x00000040)>>6)
#define DCOL_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*	 Fields TXE	 */
#define TXE_WIDTH                                                             1
#define TXE_SHIFT                                                             5
#define TXE_MASK                                                     0x00000020
#define TXE_RD(src)                                   (((src) & 0x00000020)>>5)
#define TXE_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields RFF	 */
#define RFF_WIDTH                                                             1
#define RFF_SHIFT                                                             4
#define RFF_MASK                                                     0x00000010
#define RFF_RD(src)                                   (((src) & 0x00000010)>>4)
#define RFF_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields RFNE	 */
#define RFNE_WIDTH                                                            1
#define RFNE_SHIFT                                                            3
#define RFNE_MASK                                                    0x00000008
#define RFNE_RD(src)                                  (((src) & 0x00000008)>>3)
#define RFNE_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields TFE	 */
#define TFE_WIDTH                                                             1
#define TFE_SHIFT                                                             2
#define TFE_MASK                                                     0x00000004
#define TFE_RD(src)                                   (((src) & 0x00000004)>>2)
#define TFE_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields TFNF	 */
#define TFNF_WIDTH                                                            1
#define TFNF_SHIFT                                                            1
#define TFNF_MASK                                                    0x00000002
#define TFNF_RD(src)                                  (((src) & 0x00000002)>>1)
#define TFNF_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields BUSY	 */
#define BUSY_WIDTH                                                            1
#define BUSY_SHIFT                                                            0
#define BUSY_MASK                                                    0x00000001
#define BUSY_RD(src)                                     (((src) & 0x00000001))
#define BUSY_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register IMR	*/ 
/*	 Fields MSTIM	 */
#define MSTIM_WIDTH                                                           1
#define MSTIM_SHIFT                                                           5
#define MSTIM_MASK                                                   0x00000020
#define MSTIM_RD(src)                                 (((src) & 0x00000020)>>5)
#define MSTIM_WR(src)                            (((u32)(src)<<5) & 0x00000020)
#define MSTIM_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields RXFIM	 */
#define RXFIM_WIDTH                                                           1
#define RXFIM_SHIFT                                                           4
#define RXFIM_MASK                                                   0x00000010
#define RXFIM_RD(src)                                 (((src) & 0x00000010)>>4)
#define RXFIM_WR(src)                            (((u32)(src)<<4) & 0x00000010)
#define RXFIM_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields RXOIM	 */
#define RXOIM_WIDTH                                                           1
#define RXOIM_SHIFT                                                           3
#define RXOIM_MASK                                                   0x00000008
#define RXOIM_RD(src)                                 (((src) & 0x00000008)>>3)
#define RXOIM_WR(src)                            (((u32)(src)<<3) & 0x00000008)
#define RXOIM_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields RXUIM	 */
#define RXUIM_WIDTH                                                           1
#define RXUIM_SHIFT                                                           2
#define RXUIM_MASK                                                   0x00000004
#define RXUIM_RD(src)                                 (((src) & 0x00000004)>>2)
#define RXUIM_WR(src)                            (((u32)(src)<<2) & 0x00000004)
#define RXUIM_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields TXOIM	 */
#define TXOIM_WIDTH                                                           1
#define TXOIM_SHIFT                                                           1
#define TXOIM_MASK                                                   0x00000002
#define TXOIM_RD(src)                                 (((src) & 0x00000002)>>1)
#define TXOIM_WR(src)                            (((u32)(src)<<1) & 0x00000002)
#define TXOIM_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields TXEIM	 */
#define TXEIM_WIDTH                                                           1
#define TXEIM_SHIFT                                                           0
#define TXEIM_MASK                                                   0x00000001
#define TXEIM_RD(src)                                    (((src) & 0x00000001))
#define TXEIM_WR(src)                               (((u32)(src)) & 0x00000001)
#define TXEIM_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register ISR	*/ 
/*	 Fields MSTIS	 */
#define MSTIS_WIDTH                                                           1
#define MSTIS_SHIFT                                                           5
#define MSTIS_MASK                                                   0x00000020
#define MSTIS_RD(src)                                 (((src) & 0x00000020)>>5)
#define MSTIS_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields RXFIS	 */
#define RXFIS_WIDTH                                                           1
#define RXFIS_SHIFT                                                           4
#define RXFIS_MASK                                                   0x00000010
#define RXFIS_RD(src)                                 (((src) & 0x00000010)>>4)
#define RXFIS_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields RXOIS	 */
#define RXOIS_WIDTH                                                           1
#define RXOIS_SHIFT                                                           3
#define RXOIS_MASK                                                   0x00000008
#define RXOIS_RD(src)                                 (((src) & 0x00000008)>>3)
#define RXOIS_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields RXUIS	 */
#define RXUIS_WIDTH                                                           1
#define RXUIS_SHIFT                                                           2
#define RXUIS_MASK                                                   0x00000004
#define RXUIS_RD(src)                                 (((src) & 0x00000004)>>2)
#define RXUIS_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields TXOIS	 */
#define TXOIS_WIDTH                                                           1
#define TXOIS_SHIFT                                                           1
#define TXOIS_MASK                                                   0x00000002
#define TXOIS_RD(src)                                 (((src) & 0x00000002)>>1)
#define TXOIS_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields TXEIS	 */
#define TXEIS_WIDTH                                                           1
#define TXEIS_SHIFT                                                           0
#define TXEIS_MASK                                                   0x00000001
#define TXEIS_RD(src)                                    (((src) & 0x00000001))
#define TXEIS_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register RISR	*/ 
/*	 Fields MSTIR	 */
#define MSTIR_WIDTH                                                           1
#define MSTIR_SHIFT                                                           5
#define MSTIR_MASK                                                   0x00000020
#define MSTIR_RD(src)                                 (((src) & 0x00000020)>>5)
#define MSTIR_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields RXFIR	 */
#define RXFIR_WIDTH                                                           1
#define RXFIR_SHIFT                                                           4
#define RXFIR_MASK                                                   0x00000010
#define RXFIR_RD(src)                                 (((src) & 0x00000010)>>4)
#define RXFIR_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields RXOIR	 */
#define RXOIR_WIDTH                                                           1
#define RXOIR_SHIFT                                                           3
#define RXOIR_MASK                                                   0x00000008
#define RXOIR_RD(src)                                 (((src) & 0x00000008)>>3)
#define RXOIR_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields RXUIR	 */
#define RXUIR_WIDTH                                                           1
#define RXUIR_SHIFT                                                           2
#define RXUIR_MASK                                                   0x00000004
#define RXUIR_RD(src)                                 (((src) & 0x00000004)>>2)
#define RXUIR_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields TXOIR	 */
#define TXOIR_WIDTH                                                           1
#define TXOIR_SHIFT                                                           1
#define TXOIR_MASK                                                   0x00000002
#define TXOIR_RD(src)                                 (((src) & 0x00000002)>>1)
#define TXOIR_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields TXEIR	 */
#define TXEIR_WIDTH                                                           1
#define TXEIR_SHIFT                                                           0
#define TXEIR_MASK                                                   0x00000001
#define TXEIR_RD(src)                                    (((src) & 0x00000001))
#define TXEIR_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register TXOICR	*/ 
/*	 Fields TXOICR	 */
#define TXOICR_WIDTH                                                          1
#define TXOICR_SHIFT                                                          0
#define TXOICR_MASK                                                  0x00000001
#define TXOICR_RD(src)                                   (((src) & 0x00000001))
#define TXOICR_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register RXOICR	*/ 
/*	 Fields RXOICR	 */
#define RXOICR_WIDTH                                                          1
#define RXOICR_SHIFT                                                          0
#define RXOICR_MASK                                                  0x00000001
#define RXOICR_RD(src)                                   (((src) & 0x00000001))
#define RXOICR_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register RXUICR	*/ 
/*	 Fields RXUICR	 */
#define RXUICR_WIDTH                                                          1
#define RXUICR_SHIFT                                                          0
#define RXUICR_MASK                                                  0x00000001
#define RXUICR_RD(src)                                   (((src) & 0x00000001))
#define RXUICR_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register MSTICR	*/ 
/*	 Fields MSTICR	 */
#define MSTICR_WIDTH                                                          1
#define MSTICR_SHIFT                                                          0
#define MSTICR_MASK                                                  0x00000001
#define MSTICR_RD(src)                                   (((src) & 0x00000001))
#define MSTICR_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register ICR	*/ 
/*	 Fields ICR	 */
#define ICR_WIDTH                                                             1
#define ICR_SHIFT                                                             0
#define ICR_MASK                                                     0x00000001
#define ICR_RD(src)                                      (((src) & 0x00000001))
#define ICR_SET(dst,src) (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register IDR	*/ 
/*	 Fields IDCODE	 */
#define IDCODE_WIDTH                                                         32
#define IDCODE_SHIFT                                                          0
#define IDCODE_MASK                                                  0xffffffff
#define IDCODE_RD(src)                                   (((src) & 0xffffffff))
#define IDCODE_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register VIDR	*/ 
/*	 Fields SSI_COMP_VERSION	 */
#define SSI_COMP_VERSION_WIDTH                                               32
#define SSI_COMP_VERSION_SHIFT                                                0
#define SSI_COMP_VERSION_MASK                                        0xffffffff
#define SSI_COMP_VERSION_RD(src)                         (((src) & 0xffffffff))
#define SSI_COMP_VERSION_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register DR_LOW	*/ 
/*	 Fields DR_LOW	 */
#define DR_LOW_WIDTH                                                         16
#define DR_LOW_SHIFT                                                          0
#define DR_LOW_MASK                                                  0x0000ffff
#define DR_LOW_RD(src)                                   (((src) & 0x0000ffff))
#define DR_LOW_WR(src)                              (((u32)(src)) & 0x0000ffff)
#define DR_LOW_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register DR_HIGH	*/ 
/*	 Fields DR_HIGH	 */
#define DR_HIGH_WIDTH                                                        16
#define DR_HIGH_SHIFT                                                         0
#define DR_HIGH_MASK                                                 0x0000ffff
#define DR_HIGH_RD(src)                                  (((src) & 0x0000ffff))
#define DR_HIGH_WR(src)                             (((u32)(src)) & 0x0000ffff)
#define DR_HIGH_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

#endif
