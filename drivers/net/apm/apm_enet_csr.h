/*
 * APM 86xxx Ethernet Header File
 *
 * Copyright (c) 2011, Applied Micro Circuits Corporation
 * Author: Loc Ho <lho@apm.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 *
 */
#ifndef __APM_ENET_CSR_H__
#define __APM_ENET_CSR_H__

/*	Global Base Address	*/
#define ENET_CSR_BASE_ADDR			0xddd8a2000ULL

/*    Address ENET_CSR  Registers */
#define ENET_BID_ADDR                                                0x00000000
#define ENET_BID_DEFAULT                                             0x00000063
#define ENET_FPGA_ADDR                                               0x00000004
#define ENET_FPGA_DEFAULT                                            0x00000000
#define RSIF_CONFIG_REG_ADDR                                         0x00000010
#define RSIF_CONFIG_REG_DEFAULT                                      0x7f110104
#define RSIF_PRIORITY_POLICER_REG0_ADDR                              0x00000014
#define RSIF_PRIORITY_POLICER_REG0_DEFAULT                           0x0000ffff
#define RSIF_PRIORITY_POLICER_REG1_ADDR                              0x00000018
#define RSIF_PRIORITY_POLICER_REG1_DEFAULT                           0x64ffffff
#define RSIF_PRIORITY_POLICER_REG2_ADDR                              0x0000001c
#define RSIF_PRIORITY_POLICER_REG2_DEFAULT                           0x00000040
#define RSIF_FLOWGROUP_POLICER_REG0_ADDR                             0x00000020
#define RSIF_FLOWGROUP_POLICER_REG0_DEFAULT                          0x0000ffff
#define RSIF_FLOWGROUP_POLICER_REG1_ADDR                             0x00000024
#define RSIF_FLOWGROUP_POLICER_REG1_DEFAULT                          0x64ffffff
#define RSIF_FLOWGROUP_POLICER_REG2_ADDR                             0x00000028
#define RSIF_FLOWGROUP_POLICER_REG2_DEFAULT                          0x00000040
#define RSIF_PERFLOW_POLICER_REG0_ADDR                               0x0000002c
#define RSIF_PERFLOW_POLICER_REG0_DEFAULT                            0x0000ffff
#define RSIF_PERFLOW_POLICER_REG1_ADDR                               0x00000030
#define RSIF_PERFLOW_POLICER_REG1_DEFAULT                            0x64ffffff
#define RSIF_PERFLOW_POLICER_REG2_ADDR                               0x00000034
#define RSIF_PERFLOW_POLICER_REG2_DEFAULT                            0x00000040
#define RSIF_POLICER_STSREG0_ADDR                                    0x00000038
#define RSIF_POLICER_STSREG0_DEFAULT                                 0x00000000
#define RSIF_PRIORITY_POLICER_STSREG0_ADDR                           0x0000003c
#define RSIF_PRIORITY_POLICER_STSREG0_DEFAULT                        0x00000000
#define RSIF_FLOWGROUP_POLICER_STSREG0_ADDR                          0x00000040
#define RSIF_FLOWGROUP_POLICER_STSREG0_DEFAULT                       0x00000000
#define RSIF_PERFLOW_POLICER_STSREG0_ADDR                            0x00000044
#define RSIF_PERFLOW_POLICER_STSREG0_DEFAULT                         0x00000000
#define RSIF_RAM_DBG_REG0_ADDR                                       0x00000048
#define RSIF_RAM_DBG_REG0_DEFAULT                                    0x02100000
#define RSIF_WOL_MODE0_ADDR                                          0x0000004c
#define RSIF_WOL_MODE0_DEFAULT                                       0x00000000
#define RSIF_FIFO_EMPTYSTS0_ADDR                                     0x00000050
#define RSIF_FIFO_EMPTYSTS0_DEFAULT                                  0x007f00ff
#define RSIF_INT_REG0_ADDR                                           0x00000054
#define RSIF_INT_REG0_DEFAULT                                        0x00000000
#define RSIF_INT_REG0MASK_ADDR                                       0x00000058
#define RSIF_FINT_REG0_ADDR                                          0x0000005c
#define RSIF_FINT_REG0_DEFAULT                                       0x00000000
#define RSIF_FINT_REG0MASK_ADDR                                      0x00000060
#define RSIF_STS_REG0_ADDR                                           0x00000064
#define RSIF_STS_REG0_DEFAULT                                        0x00000000
#define RSIF_H0_0_ADDR                                               0x00000068
#define RSIF_H0_0_DEFAULT                                            0x00000000
#define RSIF_H0_1_ADDR                                               0x0000006c
#define RSIF_H0_1_DEFAULT                                            0x00000010
#define RSIF_H1_0_ADDR                                               0x00000070
#define RSIF_H1_0_DEFAULT                                            0x00000000
#define RSIF_H1_1_ADDR                                               0x00000074
#define RSIF_H1_1_DEFAULT                                            0x00000300
#define RSIF_FPBUFF_TIMEOUT_STSREG0_ADDR                             0x00000078
#define RSIF_FPBUFF_TIMEOUT_STSREG0_DEFAULT                          0x00000000
#define RSIF_STS_CNTR_REG0_ADDR                                      0x0000007c
#define RSIF_STS_CNTR_REG0_DEFAULT                                   0x00000000
#define RSIF_STS_CNTR_REG1_ADDR                                      0x00000080
#define RSIF_STS_CNTR_REG1_DEFAULT                                   0x00000000
#define TSIF_CONFIG_REG_0_ADDR                                       0x00000100
#define TSIF_CONFIG_REG_0_DEFAULT                                    0x00004244
#define TSIF_CONFIG_REG_1_ADDR                                       0x00000104
#define TSIF_CONFIG_REG_1_DEFAULT                                    0x00004244
#define TSIF_MSS_REG0_0_ADDR                                         0x00000108
#define TSIF_MSS_REG0_0_DEFAULT                                      0x01000040
#define TSIF_MSS_REG0_1_ADDR                                         0x0000010c
#define TSIF_MSS_REG0_1_DEFAULT                                      0x01000040
#define TSIF_MSS_REG1_0_ADDR                                         0x00000110
#define TSIF_MSS_REG1_0_DEFAULT                                      0x05ee0400
#define TSIF_MSS_REG1_1_ADDR                                         0x00000114
#define TSIF_MSS_REG1_1_DEFAULT                                      0x05ee0400
#define TSIF_FIFO_EMPTYSTS0_ADDR                                     0x00000118
#define TSIF_FIFO_EMPTYSTS0_DEFAULT                                  0x00070007
#define TSIF_INT_REG0_ADDR                                           0x0000012c
#define TSIF_INT_REG0_DEFAULT                                        0x00000000
#define TSIF_INT_REG0MASK_ADDR                                       0x00000130
#define TSIF_FINT_REG0_ADDR                                          0x00000134
#define TSIF_FINT_REG0_DEFAULT                                       0x00000000
#define TSIF_FINT_REG0MASK_ADDR                                      0x00000138
#define TSIF_STS_REG0_ADDR                                           0x0000013c
#define TSIF_STS_REG0_DEFAULT                                        0x00000000
#define TSIF_STS_REG1_ADDR                                           0x00000140
#define TSIF_STS_REG1_DEFAULT                                        0x00000000
#define TSIF_STS_REG2_ADDR                                           0x00000144
#define TSIF_STS_REG2_DEFAULT                                        0x00000000
#define TSIF_STS_REG3_ADDR                                           0x00000148
#define TSIF_STS_REG3_DEFAULT                                        0x00000000
#define AVB_CLASSA_CREDIT_REFILL_0_ADDR                              0x00000200
#define AVB_CLASSA_CREDIT_REFILL_0_DEFAULT                           0x00000000
#define AVB_CLASSA_CREDIT_REFILL_1_ADDR                              0x00000204
#define AVB_CLASSA_CREDIT_REFILL_1_DEFAULT                           0x00000000
#define AVB_CLASSB_CREDIT_REFILL_0_ADDR                              0x00000208
#define AVB_CLASSB_CREDIT_REFILL_0_DEFAULT                           0x00000000
#define AVB_CLASSB_CREDIT_REFILL_1_ADDR                              0x0000020c
#define AVB_CLASSB_CREDIT_REFILL_1_DEFAULT                           0x00000000
#define AVB_COMMON_CONFIG1_0_ADDR                                    0x00000210
#define AVB_COMMON_CONFIG1_0_DEFAULT                                 0x00010000
#define AVB_COMMON_CONFIG1_1_ADDR                                    0x00000214
#define AVB_COMMON_CONFIG1_1_DEFAULT                                 0x00010000
#define AVB_COMMON_CONFIG2_0_ADDR                                    0x00000218
#define AVB_COMMON_CONFIG2_0_DEFAULT                                 0x00010000
#define AVB_COMMON_CONFIG2_1_ADDR                                    0x0000021c
#define AVB_COMMON_CONFIG2_1_DEFAULT                                 0x00010000
#define AVB_PER_Q_CONFIG1_0_ADDR                                     0x00000220
#define AVB_PER_Q_CONFIG1_0_DEFAULT                                  0x10efaaaa
#define AVB_PER_Q_CONFIG1_1_ADDR                                     0x00000224
#define AVB_PER_Q_CONFIG1_1_DEFAULT                                  0x10efaaaa
#define AVB_PER_Q_HI_CREDIT_0_ADDR                                   0x00000228
#define AVB_PER_Q_HI_CREDIT_0_DEFAULT                                0x00000000
#define AVB_PER_Q_HI_CREDIT_1_ADDR                                   0x0000022c
#define AVB_PER_Q_HI_CREDIT_1_DEFAULT                                0x00000000
#define AVB_PER_Q_IDLE_SLOPE0_0_ADDR                                 0x00000230
#define AVB_PER_Q_IDLE_SLOPE0_0_DEFAULT                              0xffffffff
#define AVB_PER_Q_IDLE_SLOPE0_1_ADDR                                 0x00000234
#define AVB_PER_Q_IDLE_SLOPE0_1_DEFAULT                              0xffffffff
#define AVB_PER_Q_IDLE_SLOPE1_0_ADDR                                 0x00000238
#define AVB_PER_Q_IDLE_SLOPE1_0_DEFAULT                              0xffffffff
#define AVB_PER_Q_IDLE_SLOPE1_1_ADDR                                 0x0000023c
#define AVB_PER_Q_IDLE_SLOPE1_1_DEFAULT                              0xffffffff
#define AVB_PER_Q_IDLE_SLOPE2_0_ADDR                                 0x00000240
#define AVB_PER_Q_IDLE_SLOPE2_0_DEFAULT                              0xffffffff
#define AVB_PER_Q_IDLE_SLOPE2_1_ADDR                                 0x00000244
#define AVB_PER_Q_IDLE_SLOPE2_1_DEFAULT                              0xffffffff
#define AVB_PER_Q_BYTE_NUM_Q0_0_ADDR                                 0x00000248
#define AVB_PER_Q_BYTE_NUM_Q0_0_DEFAULT                              0x00000000
#define AVB_PER_Q_BYTE_NUM_Q0_1_ADDR                                 0x0000024c
#define AVB_PER_Q_BYTE_NUM_Q0_1_DEFAULT                              0x00000000
#define AVB_PER_Q_BYTE_NUM_Q1_0_ADDR                                 0x00000250
#define AVB_PER_Q_BYTE_NUM_Q1_0_DEFAULT                              0x00000000
#define AVB_PER_Q_BYTE_NUM_Q1_1_ADDR                                 0x00000254
#define AVB_PER_Q_BYTE_NUM_Q1_1_DEFAULT                              0x00000000
#define AVB_PER_Q_BYTE_NUM_Q2_0_ADDR                                 0x00000258
#define AVB_PER_Q_BYTE_NUM_Q2_0_DEFAULT                              0x00000000
#define AVB_PER_Q_BYTE_NUM_Q2_1_ADDR                                 0x0000025c
#define AVB_PER_Q_BYTE_NUM_Q2_1_DEFAULT                              0x00000000
#define AVB_PER_Q_BYTE_NUM_Q3_0_ADDR                                 0x00000260
#define AVB_PER_Q_BYTE_NUM_Q3_0_DEFAULT                              0x00000000
#define AVB_PER_Q_BYTE_NUM_Q3_1_ADDR                                 0x00000264
#define AVB_PER_Q_BYTE_NUM_Q3_1_DEFAULT                              0x00000000
#define AVB_PER_Q_BYTE_NUM_Q4_0_ADDR                                 0x00000268
#define AVB_PER_Q_BYTE_NUM_Q4_0_DEFAULT                              0x00000000
#define AVB_PER_Q_BYTE_NUM_Q4_1_ADDR                                 0x0000026c
#define AVB_PER_Q_BYTE_NUM_Q4_1_DEFAULT                              0x00000000
#define AVB_PER_Q_BYTE_NUM_Q5_0_ADDR                                 0x00000270
#define AVB_PER_Q_BYTE_NUM_Q5_0_DEFAULT                              0x00000000
#define AVB_PER_Q_BYTE_NUM_Q5_1_ADDR                                 0x00000274
#define AVB_PER_Q_BYTE_NUM_Q5_1_DEFAULT                              0x00000000
#define AVB_PER_Q_BYTE_NUM_Q6_0_ADDR                                 0x00000278
#define AVB_PER_Q_BYTE_NUM_Q6_0_DEFAULT                              0x00000000
#define AVB_PER_Q_BYTE_NUM_Q6_1_ADDR                                 0x0000027c
#define AVB_PER_Q_BYTE_NUM_Q6_1_DEFAULT                              0x00000000
#define AVB_PER_Q_BYTE_NUM_Q7_0_ADDR                                 0x00000280
#define AVB_PER_Q_BYTE_NUM_Q7_0_DEFAULT                              0x00000000
#define AVB_PER_Q_BYTE_NUM_Q7_1_ADDR                                 0x00000284
#define AVB_PER_Q_BYTE_NUM_Q7_1_DEFAULT                              0x00000000
#define AVB_CFG_PRSNTN_TIME_0_ADDR                                   0x00000288
#define AVB_CFG_PRSNTN_TIME_0_DEFAULT                                0x00000000
#define AVB_CFG_PRSNTN_TIME_1_ADDR                                   0x0000028c
#define AVB_CFG_PRSNTN_TIME_1_DEFAULT                                0x00000000
#define CFG_LINK_AGGR_ADDR                                           0x00000290
#define CFG_LINK_AGGR_DEFAULT                                        0x00000000
#define MH_FIFO_FULL_THRESH_0_ADDR                                   0x00000300
#define MH_FIFO_FULL_THRESH_0_DEFAULT                                0x88888888
#define MH_FIFO_FULL_THRESH_1_ADDR                                   0x00000304
#define MH_FIFO_FULL_THRESH_1_DEFAULT                                0x88888888
#define DEALLOC_FIFO_FULL_THRESH_0_ADDR                              0x00000308
#define DEALLOC_FIFO_FULL_THRESH_0_DEFAULT                           0x00000007
#define DEALLOC_FIFO_FULL_THRESH_1_ADDR                              0x0000030c
#define DEALLOC_FIFO_FULL_THRESH_1_DEFAULT                           0x00000007
#define TSO_FIFO_STATUS_ADDR                                         0x00000310
#define TSO_FIFO_STATUS_DEFAULT                                      0x00000000
#define TSO_CFG_0_ADDR                                               0x00000314
#define TSO_CFG_0_DEFAULT                                            0x00000040
#define TSO_CFG_1_ADDR                                               0x00000318
#define TSO_CFG_1_DEFAULT                                            0x00000040
#define TSO_CFG_INSERT_VLAN_0_ADDR                                   0x0000031c
#define TSO_CFG_INSERT_VLAN_0_DEFAULT                                0x00000000
#define TSO_CFG_INSERT_VLAN_1_ADDR                                   0x00000320
#define TSO_CFG_INSERT_VLAN_1_DEFAULT                                0x00000000
#define TSO_INT_REG0_ADDR                                            0x00000324
#define TSO_INT_REG0_DEFAULT                                         0x00000000
#define TSO_INT_REG0MASK_ADDR                                        0x00000328
#define TTF_FIFO_FULL_THRESH_0_ADDR                                  0x0000032c
#define TTF_FIFO_FULL_THRESH_0_DEFAULT                               0x00000005
#define TTF_FIFO_FULL_THRESH_1_ADDR                                  0x00000330
#define TTF_FIFO_FULL_THRESH_1_DEFAULT                               0x00000005
#define SPI2SAP_ADDSPI_REG_ADDR                                      0x00000400
#define SPI2SAP_ADDSPI_REG_DEFAULT                                   0x00000000
#define SPI2SAP_ADDSPIEN_REG_ADDR                                    0x00000404
#define SPI2SAP_ADDSPIEN_REG_DEFAULT                                 0x00000000
#define SPI2SAP_DELSPI_REG_ADDR                                      0x00000408
#define SPI2SAP_DELSPI_REG_DEFAULT                                   0x00000000
#define SPI2SAP_DELSPIEN_REG_ADDR                                    0x0000040c
#define SPI2SAP_DELSPIEN_REG_DEFAULT                                 0x00000000
#define SPI2SAP_SEARCHSPI_REG_ADDR                                   0x00000410
#define SPI2SAP_SEARCHSPI_REG_DEFAULT                                0x00000000
#define SPI2SAP_SEARCHSPIEN_REG_ADDR                                 0x00000414
#define SPI2SAP_SEARCHSPIEN_REG_DEFAULT                              0x00000000
#define SPI2SAP_CFG0_REG_ADDR                                        0x00000418
#define SPI2SAP_CFG0_REG_DEFAULT                                     0x000fff7f
#define SPI2SAP_DBG0_REG_ADDR                                        0x0000041c
#define SPI2SAP_DBG0_REG_DEFAULT                                     0x00000000
#define SPI2SAP_DBG1_REG_ADDR                                        0x00000420
#define SPI2SAP_DBG1_REG_DEFAULT                                     0x00000000
#define SPI2SAP_DBG2_REG_ADDR                                        0x00000424
#define SPI2SAP_DBG2_REG_DEFAULT                                     0x00000000
#define SPI2SAP_INTREG_ADDR                                          0x00000428
#define SPI2SAP_INTREG_DEFAULT                                       0x00000000
#define SPI2SAP_INTREGMASK_ADDR                                      0x0000042c
#define SPI2SAP_STSREG0_ADDR                                         0x00000430
#define SPI2SAP_STSREG0_DEFAULT                                      0x00000000
#define SPI2SAP_STSREG1_ADDR                                         0x00000434
#define SPI2SAP_STSREG1_DEFAULT                                      0x00000000
#define SPI2SAP_STSREG2_ADDR                                         0x00000438
#define SPI2SAP_STSREG2_DEFAULT                                      0x00000000
#define SPI2SAP_STSREG3_ADDR                                         0x00000440
#define SPI2SAP_STSREG3_DEFAULT                                      0x0000007f
#define SPI2SAP_FIFO_EMPTYSTS0_ADDR                                  0x00000444
#define SPI2SAP_FIFO_EMPTYSTS0_DEFAULT                               0x00000003
#define SPI2SAP_INT_REG0_ADDR                                        0x00000448
#define SPI2SAP_INT_REG0_DEFAULT                                     0x00000000
#define SPI2SAP_INT_REG0MASK_ADDR                                    0x0000044c
#define ENET_SAD_ADDRCMD_REG_ADDR                                    0x00000460
#define ENET_SAD_ADDRCMD_REG_DEFAULT                                 0x00000000
#define ENET_SAD_WDATA0_REG_ADDR                                     0x00000464
#define ENET_SAD_WDATA0_REG_DEFAULT                                  0x00000000
#define ENET_SAD_WDATA1_REG_ADDR                                     0x00000468
#define ENET_SAD_WDATA1_REG_DEFAULT                                  0x00000000
#define ENET_SAD_WDATA2_REG_ADDR                                     0x0000046c
#define ENET_SAD_WDATA2_REG_DEFAULT                                  0x00000000
#define ENET_SAD_WDATA3_REG_ADDR                                     0x00000470
#define ENET_SAD_WDATA3_REG_DEFAULT                                  0x00000000
#define ENET_SAD_RDATA0_REG_ADDR                                     0x00000474
#define ENET_SAD_RDATA0_REG_DEFAULT                                  0x00000000
#define ENET_SAD_RDATA1_REG_ADDR                                     0x00000478
#define ENET_SAD_RDATA1_REG_DEFAULT                                  0x00000000
#define ENET_SAD_RDATA2_REG_ADDR                                     0x0000047c
#define ENET_SAD_RDATA2_REG_DEFAULT                                  0x00000000
#define ENET_SAD_RDATA3_REG_ADDR                                     0x00000480
#define ENET_SAD_RDATA3_REG_DEFAULT                                  0x00000000
#define ENET_SAD_CMDDONE_REG_ADDR                                    0x00000484
#define ENET_SAD_CMDDONE_REG_DEFAULT                                 0x00000000
#define RXBUF_CONFIG_THRESH_REG_ADDR                                 0x00000500
#define RXBUF_CONFIG_THRESH_REG_DEFAULT                              0x01f40000
#define RXBUF_CONFIG_PORT_DEPTH_REG_ADDR                             0x00000504
#define RXBUF_CONFIG_PORT_DEPTH_REG_DEFAULT                          0x02000200
#define RXBUF_CONFIG_START_ADDR_REG_ADDR                             0x00000508
#define RXBUF_CONFIG_START_ADDR_REG_DEFAULT                          0x00000000
#define RXBUF_CONFIG_BUFFER_REG_ADDR                                 0x0000050c
#define RXBUF_CONFIG_BUFFER_REG_DEFAULT                              0x00000000
#define TXBUF_CONFIG_THRESH_REG_ADDR                                 0x00000510
#define TXBUF_CONFIG_THRESH_REG_DEFAULT                              0x0c000000
#define TXBUF_CONFIG_PORT_DEPTH_REG_ADDR                             0x00000514
#define TXBUF_CONFIG_PORT_DEPTH_REG_DEFAULT                          0x0c000c00
#define TXBUF_CONFIG_START_ADDR_REG_ADDR                             0x00000518
#define TXBUF_CONFIG_START_ADDR_REG_DEFAULT                          0x00000c00
#define TXBUF_CONFIG_BUFFER_REG_ADDR                                 0x0000051c
#define TXBUF_CONFIG_BUFFER_REG_DEFAULT                              0x0000000a
#define TIMER_CONFIG1_REG_ADDR                                       0x00000520
#define TIMER_CONFIG1_REG_DEFAULT                                    0x004fa3e8
#define TIMER_CONFIG2_REG_ADDR                                       0x00000524
#define TIMER_CONFIG2_REG_DEFAULT                                    0x000f0101
#define RX_TX_BUF_STS_REG0_ADDR                                      0x00000528
#define RX_TX_BUF_STS_REG0_DEFAULT                                   0x00030003
#define RX_TX_BUF_CHKSM_INT_REG0_ADDR                                0x0000052c
#define RX_TX_BUF_CHKSM_INT_REG0_DEFAULT                             0x00000000
#define RX_TX_BUF_CHKSM_INT_REG0MASK_ADDR                            0x00000530
#define RXBUF_CONFIG_PAUSE_THRESH_REG_ADDR                           0x00000534
#define RXBUF_CONFIG_PAUSE_THRESH_REG_DEFAULT                        0x01f401f4
#define MEMORY_CONFIG1_REG_ADDR                                      0x00000600
#define MEMORY_CONFIG1_REG_DEFAULT                                   0x00000000
#define MEMORY_CONFIG2_REG_ADDR                                      0x00000604
#define MEMORY_CONFIG2_REG_DEFAULT                                   0x00000000
#define PTP_CONTROL_ADDR                                             0x00000650
#define PTP_CONTROL_DEFAULT                                          0x00112092
#define PTP_RX_TMSTMP_PRT0_ADDR                                      0x00000654
#define PTP_RX_TMSTMP_PRT0_DEFAULT                                   0x00000000
#define PTP_TX_TMSTMP0_PRT0_ADDR                                     0x00000658
#define PTP_TX_TMSTMP0_PRT0_DEFAULT                                  0x00000000
#define PTP_TX_TMSTMP1_PRT0_ADDR                                     0x0000065c
#define PTP_TX_TMSTMP1_PRT0_DEFAULT                                  0x00000000
#define PTP_TX_CORRECTION_PRT0_ADDR                                  0x00000660
#define PTP_TX_CORRECTION_PRT0_DEFAULT                               0x00000000
#define PTP_RX_TMSTMP_PRT1_ADDR                                      0x00000664
#define PTP_RX_TMSTMP_PRT1_DEFAULT                                   0x00000000
#define PTP_TX_TMSTMP0_PRT1_ADDR                                     0x00000668
#define PTP_TX_TMSTMP0_PRT1_DEFAULT                                  0x00000000
#define PTP_TX_TMSTMP1_PRT1_ADDR                                     0x0000066c
#define PTP_TX_TMSTMP1_PRT1_DEFAULT                                  0x00000000
#define PTP_TX_CORRECTION_PRT1_ADDR                                  0x00000670
#define PTP_TX_CORRECTION_PRT1_DEFAULT                               0x00000000
#define DEBUG_REG1_ADDR                                              0x000006fc
#define DEBUG_REG1_DEFAULT                                           0x00000000
#define DEBUG_REG_ADDR                                               0x00000700
#define DEBUG_REG_DEFAULT                                            0x003e03e8
#define MPIC_DEBUG_REG_ADDR                                          0x00000704
#define MPIC_DEBUG_REG_DEFAULT                                       0x00000000
#define TSO_ENET_STAT_REG_PORT0_ADDR                                 0x00000708
#define TSO_ENET_STAT_REG_PORT0_DEFAULT                              0x00000000
#define TXB_ENET_STAT_REG_PORT0_ADDR                                 0x0000070c
#define TXB_ENET_STAT_REG_PORT0_DEFAULT                              0x00000000
#define MACIN_ENET_STAT_REG_PORT0_ADDR                               0x00000710
#define MACIN_ENET_STAT_REG_PORT0_DEFAULT                            0x00000000
#define MACOUT_ENET_STAT_REG_PORT0_ADDR                              0x00000714
#define MACOUT_ENET_STAT_REG_PORT0_DEFAULT                           0x00000000
#define RXBIN_ENET_STAT_REG_PORT0_ADDR                               0x00000718
#define RXBIN_ENET_STAT_REG_PORT0_DEFAULT                            0x00000000
#define RSIF_ENET_STAT_REG_PORT0_ADDR                                0x0000071c
#define RSIF_ENET_STAT_REG_PORT0_DEFAULT                             0x00000000
#define TSO_ENET_STAT_REG_PORT1_ADDR                                 0x00000720
#define TSO_ENET_STAT_REG_PORT1_DEFAULT                              0x00000000
#define TXB_ENET_STAT_REG_PORT1_ADDR                                 0x00000724
#define TXB_ENET_STAT_REG_PORT1_DEFAULT                              0x00000000
#define MACIN_ENET_STAT_REG_PORT1_ADDR                               0x00000728
#define MACIN_ENET_STAT_REG_PORT1_DEFAULT                            0x00000000
#define MACOUT_ENET_STAT_REG_PORT1_ADDR                              0x0000072c
#define MACOUT_ENET_STAT_REG_PORT1_DEFAULT                           0x00000000
#define RXBIN_ENET_STAT_REG_PORT1_ADDR                               0x00000730
#define RXBIN_ENET_STAT_REG_PORT1_DEFAULT                            0x00000000
#define RSIF_ENET_STAT_REG_PORT1_ADDR                                0x00000734
#define RSIF_ENET_STAT_REG_PORT1_DEFAULT                             0x00000000
#define ENET_SPARE_CFG_REG_ADDR                                      0x00000750
#define ENET_SPARE_CFG_REG_DEFAULT                                   0x00000040
#define ENET_SPARE_STATUS_REG_ADDR                                   0x00000754
#define ENET_SPARE_STATUS_REG_DEFAULT                                0x00000000

/*	Register enet_bid	*/ 
/*	 Fields revno	 */
#define REVNO_F8_WIDTH                                                        2
#define REVNO_F8_SHIFT                                                        8
#define REVNO_F8_MASK                                                0x00000300
#define REVNO_F8_RD(src)                              (((src) & 0x00000300)>>8)
#define REVNO_F8_SET(dst,src) \
                       (((dst) & ~0x00000300) | (((u32)(src)<<8) & 0x00000300))
/*	 Fields bulsid	 */
#define BULSID_WIDTH                                                          3
#define BULSID_SHIFT                                                          5
#define BULSID_MASK                                                  0x000000e0
#define BULSID_RD(src)                                (((src) & 0x000000e0)>>5)
#define BULSID_SET(dst,src) \
                       (((dst) & ~0x000000e0) | (((u32)(src)<<5) & 0x000000e0))
/*	 Fields deviceid	 */
#define DEVICEID_F8_WIDTH                                                     5
#define DEVICEID_F8_SHIFT                                                     0
#define DEVICEID_F8_MASK                                             0x0000001f
#define DEVICEID_F8_RD(src)                              (((src) & 0x0000001f))
#define DEVICEID_F8_SET(dst,src) \
                          (((dst) & ~0x0000001f) | (((u32)(src)) & 0x0000001f))

/*	Register enet_fpga	*/ 
/*	 Fields date	 */
#define DATE_WIDTH                                                            8
#define DATE_SHIFT                                                           24
#define DATE_MASK                                                    0xff000000
#define DATE_RD(src)                                 (((src) & 0xff000000)>>24)
#define DATE_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))
/*	 Fields month	 */
#define MONTH_WIDTH                                                           8
#define MONTH_SHIFT                                                          16
#define MONTH_MASK                                                   0x00ff0000
#define MONTH_RD(src)                                (((src) & 0x00ff0000)>>16)
#define MONTH_SET(dst,src) \
                      (((dst) & ~0x00ff0000) | (((u32)(src)<<16) & 0x00ff0000))
/*	 Fields year	 */
#define YEAR_WIDTH                                                            8
#define YEAR_SHIFT                                                            8
#define YEAR_MASK                                                    0x0000ff00
#define YEAR_RD(src)                                  (((src) & 0x0000ff00)>>8)
#define YEAR_SET(dst,src) \
                       (((dst) & ~0x0000ff00) | (((u32)(src)<<8) & 0x0000ff00))
/*	 Fields version_no	 */
#define VERSION_NO_WIDTH                                                      8
#define VERSION_NO_SHIFT                                                      0
#define VERSION_NO_MASK                                              0x000000ff
#define VERSION_NO_RD(src)                               (((src) & 0x000000ff))
#define VERSION_NO_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register RSIF_Config_Reg	*/ 
/*	 Fields cfg_rsif_fpbuff_timeout_en	 */
#define CFG_RSIF_FPBUFF_TIMEOUT_EN_WIDTH                                      1
#define CFG_RSIF_FPBUFF_TIMEOUT_EN_SHIFT                                     31
#define CFG_RSIF_FPBUFF_TIMEOUT_EN_MASK                              0x80000000
#define CFG_RSIF_FPBUFF_TIMEOUT_EN_RD(src)           (((src) & 0x80000000)>>31)
#define CFG_RSIF_FPBUFF_TIMEOUT_EN_WR(src)      (((u32)(src)<<31) & 0x80000000)
#define CFG_RSIF_FPBUFF_TIMEOUT_EN_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields cfg_rsif_fpbuff_timeout	 */
#define CFG_RSIF_FPBUFF_TIMEOUT_WIDTH                                         7
#define CFG_RSIF_FPBUFF_TIMEOUT_SHIFT                                        24
#define CFG_RSIF_FPBUFF_TIMEOUT_MASK                                 0x7f000000
#define CFG_RSIF_FPBUFF_TIMEOUT_RD(src)              (((src) & 0x7f000000)>>24)
#define CFG_RSIF_FPBUFF_TIMEOUT_WR(src)         (((u32)(src)<<24) & 0x7f000000)
#define CFG_RSIF_FPBUFF_TIMEOUT_SET(dst,src) \
                      (((dst) & ~0x7f000000) | (((u32)(src)<<24) & 0x7f000000))
/*	 Fields cfg_rsif_fpbuff_drop_en	 */
#define CFG_RSIF_FPBUFF_DROP_EN_WIDTH                                         1
#define CFG_RSIF_FPBUFF_DROP_EN_SHIFT                                        21
#define CFG_RSIF_FPBUFF_DROP_EN_MASK                                 0x00200000
#define CFG_RSIF_FPBUFF_DROP_EN_RD(src)              (((src) & 0x00200000)>>21)
#define CFG_RSIF_FPBUFF_DROP_EN_WR(src)         (((u32)(src)<<21) & 0x00200000)
#define CFG_RSIF_FPBUFF_DROP_EN_SET(dst,src) \
                      (((dst) & ~0x00200000) | (((u32)(src)<<21) & 0x00200000))
/*	 Fields cfg_rsif_big_endian	 */
#define CFG_RSIF_BIG_ENDIAN_WIDTH                                             1
#define CFG_RSIF_BIG_ENDIAN_SHIFT                                            20
#define CFG_RSIF_BIG_ENDIAN_MASK                                     0x00100000
#define CFG_RSIF_BIG_ENDIAN_RD(src)                  (((src) & 0x00100000)>>20)
#define CFG_RSIF_BIG_ENDIAN_WR(src)             (((u32)(src)<<20) & 0x00100000)
#define CFG_RSIF_BIG_ENDIAN_SET(dst,src) \
                      (((dst) & ~0x00100000) | (((u32)(src)<<20) & 0x00100000))
/*	 Fields cfg_rsif_buff_threshold	 */
#define CFG_RSIF_BUFF_THRESHOLD_WIDTH                                         5
#define CFG_RSIF_BUFF_THRESHOLD_SHIFT                                        12
#define CFG_RSIF_BUFF_THRESHOLD_MASK                                 0x0001f000
#define CFG_RSIF_BUFF_THRESHOLD_RD(src)              (((src) & 0x0001f000)>>12)
#define CFG_RSIF_BUFF_THRESHOLD_WR(src)         (((u32)(src)<<12) & 0x0001f000)
#define CFG_RSIF_BUFF_THRESHOLD_SET(dst,src) \
                      (((dst) & ~0x0001f000) | (((u32)(src)<<12) & 0x0001f000))
/*	 Fields cfg_rsif_ctrlbuff_threshold	 */
#define CFG_RSIF_CTRLBUFF_THRESHOLD_WIDTH                                     5
#define CFG_RSIF_CTRLBUFF_THRESHOLD_SHIFT                                     4
#define CFG_RSIF_CTRLBUFF_THRESHOLD_MASK                             0x000001f0
#define CFG_RSIF_CTRLBUFF_THRESHOLD_RD(src)           (((src) & 0x000001f0)>>4)
#define CFG_RSIF_CTRLBUFF_THRESHOLD_WR(src)      (((u32)(src)<<4) & 0x000001f0)
#define CFG_RSIF_CTRLBUFF_THRESHOLD_SET(dst,src) \
                       (((dst) & ~0x000001f0) | (((u32)(src)<<4) & 0x000001f0))
/*	 Fields cfg_rsif_cle_buff_threshold	 */
#define CFG_RSIF_CLE_BUFF_THRESHOLD_WIDTH                                     3
#define CFG_RSIF_CLE_BUFF_THRESHOLD_SHIFT                                     0
#define CFG_RSIF_CLE_BUFF_THRESHOLD_MASK                             0x00000007
#define CFG_RSIF_CLE_BUFF_THRESHOLD_RD(src)              (((src) & 0x00000007))
#define CFG_RSIF_CLE_BUFF_THRESHOLD_WR(src)         (((u32)(src)) & 0x00000007)
#define CFG_RSIF_CLE_BUFF_THRESHOLD_SET(dst,src) \
                          (((dst) & ~0x00000007) | (((u32)(src)) & 0x00000007))

/*	Register RSIF_Priority_Policer_Reg0	*/ 
/*	 Fields cfg_rsif_plc0_addcredits	 */
#define CFG_RSIF_PLC0_ADDCREDITS0_F2_WIDTH                                   24
#define CFG_RSIF_PLC0_ADDCREDITS0_F2_SHIFT                                    0
#define CFG_RSIF_PLC0_ADDCREDITS0_F2_MASK                            0x00ffffff
#define CFG_RSIF_PLC0_ADDCREDITS0_F2_RD(src)             (((src) & 0x00ffffff))
#define CFG_RSIF_PLC0_ADDCREDITS0_F2_WR(src)        (((u32)(src)) & 0x00ffffff)
#define CFG_RSIF_PLC0_ADDCREDITS0_F2_SET(dst,src) \
                          (((dst) & ~0x00ffffff) | (((u32)(src)) & 0x00ffffff))

/*	Register RSIF_Priority_Policer_Reg1	*/ 
/*	 Fields cfg_rsif_plc_credit_update_time	 */
#define CFG_RSIF_PLC_CREDIT_UPDATE_TIME1_F2_WIDTH                             7
#define CFG_RSIF_PLC_CREDIT_UPDATE_TIME1_F2_SHIFT                            24
#define CFG_RSIF_PLC_CREDIT_UPDATE_TIME1_F2_MASK                     0x7f000000
#define CFG_RSIF_PLC_CREDIT_UPDATE_TIME1_F2_RD(src)  (((src) & 0x7f000000)>>24)
#define CFG_RSIF_PLC_CREDIT_UPDATE_TIME1_F2_WR(src) \
                                                (((u32)(src)<<24) & 0x7f000000)
#define CFG_RSIF_PLC_CREDIT_UPDATE_TIME1_F2_SET(dst,src) \
                      (((dst) & ~0x7f000000) | (((u32)(src)<<24) & 0x7f000000))
/*	 Fields cfg_rsif_plc0_hicredits	 */
#define CFG_RSIF_PLC0_HICREDITS1_F2_WIDTH                                    24
#define CFG_RSIF_PLC0_HICREDITS1_F2_SHIFT                                     0
#define CFG_RSIF_PLC0_HICREDITS1_F2_MASK                             0x00ffffff
#define CFG_RSIF_PLC0_HICREDITS1_F2_RD(src)              (((src) & 0x00ffffff))
#define CFG_RSIF_PLC0_HICREDITS1_F2_WR(src)         (((u32)(src)) & 0x00ffffff)
#define CFG_RSIF_PLC0_HICREDITS1_F2_SET(dst,src) \
                          (((dst) & ~0x00ffffff) | (((u32)(src)) & 0x00ffffff))

/*	Register RSIF_Priority_Policer_Reg2	*/ 
/*	 Fields cfg_rsif_plc0_threshold	 */
#define CFG_RSIF_PLC0_THRESHOLD2_F2_WIDTH                                    24
#define CFG_RSIF_PLC0_THRESHOLD2_F2_SHIFT                                     0
#define CFG_RSIF_PLC0_THRESHOLD2_F2_MASK                             0x00ffffff
#define CFG_RSIF_PLC0_THRESHOLD2_F2_RD(src)              (((src) & 0x00ffffff))
#define CFG_RSIF_PLC0_THRESHOLD2_F2_WR(src)         (((u32)(src)) & 0x00ffffff)
#define CFG_RSIF_PLC0_THRESHOLD2_F2_SET(dst,src) \
                          (((dst) & ~0x00ffffff) | (((u32)(src)) & 0x00ffffff))

/*	Register RSIF_Flowgroup_Policer_Reg0	*/ 
/*	 Fields cfg_rsif_plc1_addcredits	 */
#define CFG_RSIF_PLC1_ADDCREDITS0_F2_WIDTH                                   24
#define CFG_RSIF_PLC1_ADDCREDITS0_F2_SHIFT                                    0
#define CFG_RSIF_PLC1_ADDCREDITS0_F2_MASK                            0x00ffffff
#define CFG_RSIF_PLC1_ADDCREDITS0_F2_RD(src)             (((src) & 0x00ffffff))
#define CFG_RSIF_PLC1_ADDCREDITS0_F2_WR(src)        (((u32)(src)) & 0x00ffffff)
#define CFG_RSIF_PLC1_ADDCREDITS0_F2_SET(dst,src) \
                          (((dst) & ~0x00ffffff) | (((u32)(src)) & 0x00ffffff))

/*	Register RSIF_Flowgroup_Policer_Reg1	*/ 
/*	 Fields cfg_rsif_plc1_hicredits	 */
#define CFG_RSIF_PLC1_HICREDITS1_F2_WIDTH                                    24
#define CFG_RSIF_PLC1_HICREDITS1_F2_SHIFT                                     0
#define CFG_RSIF_PLC1_HICREDITS1_F2_MASK                             0x00ffffff
#define CFG_RSIF_PLC1_HICREDITS1_F2_RD(src)              (((src) & 0x00ffffff))
#define CFG_RSIF_PLC1_HICREDITS1_F2_WR(src)         (((u32)(src)) & 0x00ffffff)
#define CFG_RSIF_PLC1_HICREDITS1_F2_SET(dst,src) \
                          (((dst) & ~0x00ffffff) | (((u32)(src)) & 0x00ffffff))

/*	Register RSIF_Flowgroup_Policer_Reg2	*/ 
/*	 Fields cfg_rsif_plc1_threshold	 */
#define CFG_RSIF_PLC1_THRESHOLD2_F2_WIDTH                                    24
#define CFG_RSIF_PLC1_THRESHOLD2_F2_SHIFT                                     0
#define CFG_RSIF_PLC1_THRESHOLD2_F2_MASK                             0x00ffffff
#define CFG_RSIF_PLC1_THRESHOLD2_F2_RD(src)              (((src) & 0x00ffffff))
#define CFG_RSIF_PLC1_THRESHOLD2_F2_WR(src)         (((u32)(src)) & 0x00ffffff)
#define CFG_RSIF_PLC1_THRESHOLD2_F2_SET(dst,src) \
                          (((dst) & ~0x00ffffff) | (((u32)(src)) & 0x00ffffff))

/*	Register RSIF_Perflow_Policer_Reg0	*/ 
/*	 Fields cfg_rsif_plc0_en	 */
#define CFG_RSIF_PLC0_EN0_F2_WIDTH                                            1
#define CFG_RSIF_PLC0_EN0_F2_SHIFT                                           26
#define CFG_RSIF_PLC0_EN0_F2_MASK                                    0x04000000
#define CFG_RSIF_PLC0_EN0_F2_RD(src)                 (((src) & 0x04000000)>>26)
#define CFG_RSIF_PLC0_EN0_F2_WR(src)            (((u32)(src)<<26) & 0x04000000)
#define CFG_RSIF_PLC0_EN0_F2_SET(dst,src) \
                      (((dst) & ~0x04000000) | (((u32)(src)<<26) & 0x04000000))
/*	 Fields cfg_rsif_plc1_en	 */
#define CFG_RSIF_PLC1_EN0_F2_WIDTH                                            1
#define CFG_RSIF_PLC1_EN0_F2_SHIFT                                           25
#define CFG_RSIF_PLC1_EN0_F2_MASK                                    0x02000000
#define CFG_RSIF_PLC1_EN0_F2_RD(src)                 (((src) & 0x02000000)>>25)
#define CFG_RSIF_PLC1_EN0_F2_WR(src)            (((u32)(src)<<25) & 0x02000000)
#define CFG_RSIF_PLC1_EN0_F2_SET(dst,src) \
                      (((dst) & ~0x02000000) | (((u32)(src)<<25) & 0x02000000))
/*	 Fields cfg_rsif_plc2_en	 */
#define CFG_RSIF_PLC2_EN0_F2_WIDTH                                            1
#define CFG_RSIF_PLC2_EN0_F2_SHIFT                                           24
#define CFG_RSIF_PLC2_EN0_F2_MASK                                    0x01000000
#define CFG_RSIF_PLC2_EN0_F2_RD(src)                 (((src) & 0x01000000)>>24)
#define CFG_RSIF_PLC2_EN0_F2_WR(src)            (((u32)(src)<<24) & 0x01000000)
#define CFG_RSIF_PLC2_EN0_F2_SET(dst,src) \
                      (((dst) & ~0x01000000) | (((u32)(src)<<24) & 0x01000000))
/*	 Fields cfg_rsif_plc2_addcredits	 */
#define CFG_RSIF_PLC2_ADDCREDITS0_F2_WIDTH                                   24
#define CFG_RSIF_PLC2_ADDCREDITS0_F2_SHIFT                                    0
#define CFG_RSIF_PLC2_ADDCREDITS0_F2_MASK                            0x00ffffff
#define CFG_RSIF_PLC2_ADDCREDITS0_F2_RD(src)             (((src) & 0x00ffffff))
#define CFG_RSIF_PLC2_ADDCREDITS0_F2_WR(src)        (((u32)(src)) & 0x00ffffff)
#define CFG_RSIF_PLC2_ADDCREDITS0_F2_SET(dst,src) \
                          (((dst) & ~0x00ffffff) | (((u32)(src)) & 0x00ffffff))

/*	Register RSIF_Perflow_Policer_Reg1	*/ 
/*	 Fields cfg_rsif_plc2_hicredits	 */
#define CFG_RSIF_PLC2_HICREDITS1_F2_WIDTH                                    24
#define CFG_RSIF_PLC2_HICREDITS1_F2_SHIFT                                     0
#define CFG_RSIF_PLC2_HICREDITS1_F2_MASK                             0x00ffffff
#define CFG_RSIF_PLC2_HICREDITS1_F2_RD(src)              (((src) & 0x00ffffff))
#define CFG_RSIF_PLC2_HICREDITS1_F2_WR(src)         (((u32)(src)) & 0x00ffffff)
#define CFG_RSIF_PLC2_HICREDITS1_F2_SET(dst,src) \
                          (((dst) & ~0x00ffffff) | (((u32)(src)) & 0x00ffffff))

/*	Register RSIF_Perflow_Policer_Reg2	*/ 
/*	 Fields cfg_rsif_plc2_threshold	 */
#define CFG_RSIF_PLC2_THRESHOLD2_F2_WIDTH                                    24
#define CFG_RSIF_PLC2_THRESHOLD2_F2_SHIFT                                     0
#define CFG_RSIF_PLC2_THRESHOLD2_F2_MASK                             0x00ffffff
#define CFG_RSIF_PLC2_THRESHOLD2_F2_RD(src)              (((src) & 0x00ffffff))
#define CFG_RSIF_PLC2_THRESHOLD2_F2_WR(src)         (((u32)(src)) & 0x00ffffff)
#define CFG_RSIF_PLC2_THRESHOLD2_F2_SET(dst,src) \
                          (((dst) & ~0x00ffffff) | (((u32)(src)) & 0x00ffffff))

/*	Register RSIF_Policer_StsReg0	*/ 
/*	 Fields sts_rsif_plc_total_dropcnt	 */
#define STS_RSIF_PLC_TOTAL_DROPCNT0_F2_WIDTH                                 32
#define STS_RSIF_PLC_TOTAL_DROPCNT0_F2_SHIFT                                  0
#define STS_RSIF_PLC_TOTAL_DROPCNT0_F2_MASK                          0xffffffff
#define STS_RSIF_PLC_TOTAL_DROPCNT0_F2_RD(src)           (((src) & 0xffffffff))
#define STS_RSIF_PLC_TOTAL_DROPCNT0_F2_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register RSIF_Priority_Policer_StsReg0	*/ 
/*	 Fields sts_rsif_plc0_dropcnt	 */
#define STS_RSIF_PLC0_DROPCNT0_F2_WIDTH                                      32
#define STS_RSIF_PLC0_DROPCNT0_F2_SHIFT                                       0
#define STS_RSIF_PLC0_DROPCNT0_F2_MASK                               0xffffffff
#define STS_RSIF_PLC0_DROPCNT0_F2_RD(src)                (((src) & 0xffffffff))
#define STS_RSIF_PLC0_DROPCNT0_F2_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register RSIF_Flowgroup_Policer_StsReg0	*/ 
/*	 Fields sts_rsif_plc1_dropcnt	 */
#define STS_RSIF_PLC1_DROPCNT0_F2_WIDTH                                      32
#define STS_RSIF_PLC1_DROPCNT0_F2_SHIFT                                       0
#define STS_RSIF_PLC1_DROPCNT0_F2_MASK                               0xffffffff
#define STS_RSIF_PLC1_DROPCNT0_F2_RD(src)                (((src) & 0xffffffff))
#define STS_RSIF_PLC1_DROPCNT0_F2_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register RSIF_Perflow_Policer_StsReg0	*/ 
/*	 Fields sts_rsif_plc2_dropcnt	 */
#define STS_RSIF_PLC2_DROPCNT0_F2_WIDTH                                      32
#define STS_RSIF_PLC2_DROPCNT0_F2_SHIFT                                       0
#define STS_RSIF_PLC2_DROPCNT0_F2_MASK                               0xffffffff
#define STS_RSIF_PLC2_DROPCNT0_F2_RD(src)                (((src) & 0xffffffff))
#define STS_RSIF_PLC2_DROPCNT0_F2_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register RSIF_RAM_Dbg_Reg0	*/ 
/*	 Fields cfg_rsif_rtype	 */
#define CFG_RSIF_RTYPE0_WIDTH                                                10
#define CFG_RSIF_RTYPE0_SHIFT                                                16
#define CFG_RSIF_RTYPE0_MASK                                         0x03ff0000
#define CFG_RSIF_RTYPE0_RD(src)                      (((src) & 0x03ff0000)>>16)
#define CFG_RSIF_RTYPE0_WR(src)                 (((u32)(src)<<16) & 0x03ff0000)
#define CFG_RSIF_RTYPE0_SET(dst,src) \
                      (((dst) & ~0x03ff0000) | (((u32)(src)<<16) & 0x03ff0000))
/*	 Fields cfg_rsif_plcram_TEST1B	 */
#define CFG_RSIF_PLCRAM_TEST1B0_WIDTH                                         1
#define CFG_RSIF_PLCRAM_TEST1B0_SHIFT                                        11
#define CFG_RSIF_PLCRAM_TEST1B0_MASK                                 0x00000800
#define CFG_RSIF_PLCRAM_TEST1B0_RD(src)              (((src) & 0x00000800)>>11)
#define CFG_RSIF_PLCRAM_TEST1B0_WR(src)         (((u32)(src)<<11) & 0x00000800)
#define CFG_RSIF_PLCRAM_TEST1B0_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*	 Fields cfg_rsif_plcram_TEST1A	 */
#define CFG_RSIF_PLCRAM_TEST1A0_WIDTH                                         1
#define CFG_RSIF_PLCRAM_TEST1A0_SHIFT                                        10
#define CFG_RSIF_PLCRAM_TEST1A0_MASK                                 0x00000400
#define CFG_RSIF_PLCRAM_TEST1A0_RD(src)              (((src) & 0x00000400)>>10)
#define CFG_RSIF_PLCRAM_TEST1A0_WR(src)         (((u32)(src)<<10) & 0x00000400)
#define CFG_RSIF_PLCRAM_TEST1A0_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields cfg_rsif_plcram_RMEB	 */
#define CFG_RSIF_PLCRAM_RMEB0_WIDTH                                           1
#define CFG_RSIF_PLCRAM_RMEB0_SHIFT                                           9
#define CFG_RSIF_PLCRAM_RMEB0_MASK                                   0x00000200
#define CFG_RSIF_PLCRAM_RMEB0_RD(src)                 (((src) & 0x00000200)>>9)
#define CFG_RSIF_PLCRAM_RMEB0_WR(src)            (((u32)(src)<<9) & 0x00000200)
#define CFG_RSIF_PLCRAM_RMEB0_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields cfg_rsif_plcram_RMEA	 */
#define CFG_RSIF_PLCRAM_RMEA0_WIDTH                                           1
#define CFG_RSIF_PLCRAM_RMEA0_SHIFT                                           8
#define CFG_RSIF_PLCRAM_RMEA0_MASK                                   0x00000100
#define CFG_RSIF_PLCRAM_RMEA0_RD(src)                 (((src) & 0x00000100)>>8)
#define CFG_RSIF_PLCRAM_RMEA0_WR(src)            (((u32)(src)<<8) & 0x00000100)
#define CFG_RSIF_PLCRAM_RMEA0_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*	 Fields cfg_rsif_plcram_RMB	 */
#define CFG_RSIF_PLCRAM_RMB0_WIDTH                                            4
#define CFG_RSIF_PLCRAM_RMB0_SHIFT                                            4
#define CFG_RSIF_PLCRAM_RMB0_MASK                                    0x000000f0
#define CFG_RSIF_PLCRAM_RMB0_RD(src)                  (((src) & 0x000000f0)>>4)
#define CFG_RSIF_PLCRAM_RMB0_WR(src)             (((u32)(src)<<4) & 0x000000f0)
#define CFG_RSIF_PLCRAM_RMB0_SET(dst,src) \
                       (((dst) & ~0x000000f0) | (((u32)(src)<<4) & 0x000000f0))
/*	 Fields cfg_rsif_plcram_RMA	 */
#define CFG_RSIF_PLCRAM_RMA0_WIDTH                                            4
#define CFG_RSIF_PLCRAM_RMA0_SHIFT                                            0
#define CFG_RSIF_PLCRAM_RMA0_MASK                                    0x0000000f
#define CFG_RSIF_PLCRAM_RMA0_RD(src)                     (((src) & 0x0000000f))
#define CFG_RSIF_PLCRAM_RMA0_WR(src)                (((u32)(src)) & 0x0000000f)
#define CFG_RSIF_PLCRAM_RMA0_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register RSIF_Wol_Mode0	*/ 
/*	 Fields cfg_fpga_wol_mode_testen	 */
#define CFG_FPGA_WOL_MODE_TESTEN0_WIDTH                                       1
#define CFG_FPGA_WOL_MODE_TESTEN0_SHIFT                                       2
#define CFG_FPGA_WOL_MODE_TESTEN0_MASK                               0x00000004
#define CFG_FPGA_WOL_MODE_TESTEN0_RD(src)             (((src) & 0x00000004)>>2)
#define CFG_FPGA_WOL_MODE_TESTEN0_WR(src)        (((u32)(src)<<2) & 0x00000004)
#define CFG_FPGA_WOL_MODE_TESTEN0_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields cfg_mpa_enet_wake_up	 */
#define CFG_MPA_ENET_WAKE_UP0_WIDTH                                           1
#define CFG_MPA_ENET_WAKE_UP0_SHIFT                                           1
#define CFG_MPA_ENET_WAKE_UP0_MASK                                   0x00000002
#define CFG_MPA_ENET_WAKE_UP0_RD(src)                 (((src) & 0x00000002)>>1)
#define CFG_MPA_ENET_WAKE_UP0_WR(src)            (((u32)(src)<<1) & 0x00000002)
#define CFG_MPA_ENET_WAKE_UP0_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields cfg_mpa_enet_release_halt	 */
#define CFG_MPA_ENET_RELEASE_HALT0_WIDTH                                      1
#define CFG_MPA_ENET_RELEASE_HALT0_SHIFT                                      0
#define CFG_MPA_ENET_RELEASE_HALT0_MASK                              0x00000001
#define CFG_MPA_ENET_RELEASE_HALT0_RD(src)               (((src) & 0x00000001))
#define CFG_MPA_ENET_RELEASE_HALT0_WR(src)          (((u32)(src)) & 0x00000001)
#define CFG_MPA_ENET_RELEASE_HALT0_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register RSIF_FIFO_EmptySts0	*/ 
/*	 Fields rsif_lcl_rxbuf_fifo_empty_rxprt1	 */
#define RSIF_LCL_RXBUF_FIFO_EMPTY_RXPRT10_WIDTH                               1
#define RSIF_LCL_RXBUF_FIFO_EMPTY_RXPRT10_SHIFT                              22
#define RSIF_LCL_RXBUF_FIFO_EMPTY_RXPRT10_MASK                       0x00400000
#define RSIF_LCL_RXBUF_FIFO_EMPTY_RXPRT10_RD(src)    (((src) & 0x00400000)>>22)
#define RSIF_LCL_RXBUF_FIFO_EMPTY_RXPRT10_SET(dst,src) \
                      (((dst) & ~0x00400000) | (((u32)(src)<<22) & 0x00400000))
/*	 Fields rsif_chksum_buff_fifo_empty_rxprt1	 */
#define RSIF_CHKSUM_BUFF_FIFO_EMPTY_RXPRT10_WIDTH                             1
#define RSIF_CHKSUM_BUFF_FIFO_EMPTY_RXPRT10_SHIFT                            21
#define RSIF_CHKSUM_BUFF_FIFO_EMPTY_RXPRT10_MASK                     0x00200000
#define RSIF_CHKSUM_BUFF_FIFO_EMPTY_RXPRT10_RD(src)  (((src) & 0x00200000)>>21)
#define RSIF_CHKSUM_BUFF_FIFO_EMPTY_RXPRT10_SET(dst,src) \
                      (((dst) & ~0x00200000) | (((u32)(src)<<21) & 0x00200000))
/*	 Fields rsif_timestamp_buff_fifo_empty_rxprt1	 */
#define RSIF_TIMESTAMP_BUFF_FIFO_EMPTY_RXPRT10_WIDTH                          1
#define RSIF_TIMESTAMP_BUFF_FIFO_EMPTY_RXPRT10_SHIFT                         20
#define RSIF_TIMESTAMP_BUFF_FIFO_EMPTY_RXPRT10_MASK                  0x00100000
#define RSIF_TIMESTAMP_BUFF_FIFO_EMPTY_RXPRT10_RD(src) \
                                                    (((src) & 0x00100000)>>20)
#define RSIF_TIMESTAMP_BUFF_FIFO_EMPTY_RXPRT10_SET(dst,src) \
                      (((dst) & ~0x00100000) | (((u32)(src)<<20) & 0x00100000))
/*	 Fields rsif_err_buff_fifo_empty_rxprt1	 */
#define RSIF_ERR_BUFF_FIFO_EMPTY_RXPRT10_WIDTH                                1
#define RSIF_ERR_BUFF_FIFO_EMPTY_RXPRT10_SHIFT                               19
#define RSIF_ERR_BUFF_FIFO_EMPTY_RXPRT10_MASK                        0x00080000
#define RSIF_ERR_BUFF_FIFO_EMPTY_RXPRT10_RD(src)     (((src) & 0x00080000)>>19)
#define RSIF_ERR_BUFF_FIFO_EMPTY_RXPRT10_SET(dst,src) \
                      (((dst) & ~0x00080000) | (((u32)(src)<<19) & 0x00080000))
/*	 Fields rsif_clebuff_fifo_empty_rxprt1	 */
#define RSIF_CLEBUFF_FIFO_EMPTY_RXPRT10_WIDTH                                 1
#define RSIF_CLEBUFF_FIFO_EMPTY_RXPRT10_SHIFT                                18
#define RSIF_CLEBUFF_FIFO_EMPTY_RXPRT10_MASK                         0x00040000
#define RSIF_CLEBUFF_FIFO_EMPTY_RXPRT10_RD(src)      (((src) & 0x00040000)>>18)
#define RSIF_CLEBUFF_FIFO_EMPTY_RXPRT10_SET(dst,src) \
                      (((dst) & ~0x00040000) | (((u32)(src)<<18) & 0x00040000))
/*	 Fields rsif_ctrlbuff_fifo_empty_rxprt1	 */
#define RSIF_CTRLBUFF_FIFO_EMPTY_RXPRT10_WIDTH                                1
#define RSIF_CTRLBUFF_FIFO_EMPTY_RXPRT10_SHIFT                               17
#define RSIF_CTRLBUFF_FIFO_EMPTY_RXPRT10_MASK                        0x00020000
#define RSIF_CTRLBUFF_FIFO_EMPTY_RXPRT10_RD(src)     (((src) & 0x00020000)>>17)
#define RSIF_CTRLBUFF_FIFO_EMPTY_RXPRT10_SET(dst,src) \
                      (((dst) & ~0x00020000) | (((u32)(src)<<17) & 0x00020000))
/*	 Fields rsif_buf_fifo_empty_rxprt1	 */
#define RSIF_BUF_FIFO_EMPTY_RXPRT10_WIDTH                                     1
#define RSIF_BUF_FIFO_EMPTY_RXPRT10_SHIFT                                    16
#define RSIF_BUF_FIFO_EMPTY_RXPRT10_MASK                             0x00010000
#define RSIF_BUF_FIFO_EMPTY_RXPRT10_RD(src)          (((src) & 0x00010000)>>16)
#define RSIF_BUF_FIFO_EMPTY_RXPRT10_SET(dst,src) \
                      (((dst) & ~0x00010000) | (((u32)(src)<<16) & 0x00010000))
/*	 Fields rsif_plc_clebuff_fifo_empty	 */
#define RSIF_PLC_CLEBUFF_FIFO_EMPTY0_F2_WIDTH                                 1
#define RSIF_PLC_CLEBUFF_FIFO_EMPTY0_F2_SHIFT                                 7
#define RSIF_PLC_CLEBUFF_FIFO_EMPTY0_F2_MASK                         0x00000080
#define RSIF_PLC_CLEBUFF_FIFO_EMPTY0_F2_RD(src)       (((src) & 0x00000080)>>7)
#define RSIF_PLC_CLEBUFF_FIFO_EMPTY0_F2_SET(dst,src) \
                       (((dst) & ~0x00000080) | (((u32)(src)<<7) & 0x00000080))
/*	 Fields rsif_lcl_rxbuf_fifo_empty_rxprt0	 */
#define RSIF_LCL_RXBUF_FIFO_EMPTY_RXPRT00_WIDTH                               1
#define RSIF_LCL_RXBUF_FIFO_EMPTY_RXPRT00_SHIFT                               6
#define RSIF_LCL_RXBUF_FIFO_EMPTY_RXPRT00_MASK                       0x00000040
#define RSIF_LCL_RXBUF_FIFO_EMPTY_RXPRT00_RD(src)     (((src) & 0x00000040)>>6)
#define RSIF_LCL_RXBUF_FIFO_EMPTY_RXPRT00_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*	 Fields rsif_chksum_buff_fifo_empty_rxprt0	 */
#define RSIF_CHKSUM_BUFF_FIFO_EMPTY_RXPRT00_WIDTH                             1
#define RSIF_CHKSUM_BUFF_FIFO_EMPTY_RXPRT00_SHIFT                             5
#define RSIF_CHKSUM_BUFF_FIFO_EMPTY_RXPRT00_MASK                     0x00000020
#define RSIF_CHKSUM_BUFF_FIFO_EMPTY_RXPRT00_RD(src)   (((src) & 0x00000020)>>5)
#define RSIF_CHKSUM_BUFF_FIFO_EMPTY_RXPRT00_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields rsif_timestamp_buff_fifo_empty_rxprt0	 */
#define RSIF_TIMESTAMP_BUFF_FIFO_EMPTY_RXPRT00_WIDTH                          1
#define RSIF_TIMESTAMP_BUFF_FIFO_EMPTY_RXPRT00_SHIFT                          4
#define RSIF_TIMESTAMP_BUFF_FIFO_EMPTY_RXPRT00_MASK                  0x00000010
#define RSIF_TIMESTAMP_BUFF_FIFO_EMPTY_RXPRT00_RD(src) \
                                                     (((src) & 0x00000010)>>4)
#define RSIF_TIMESTAMP_BUFF_FIFO_EMPTY_RXPRT00_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields rsif_err_buff_fifo_empty_rxprt0	 */
#define RSIF_ERR_BUFF_FIFO_EMPTY_RXPRT00_WIDTH                                1
#define RSIF_ERR_BUFF_FIFO_EMPTY_RXPRT00_SHIFT                                3
#define RSIF_ERR_BUFF_FIFO_EMPTY_RXPRT00_MASK                        0x00000008
#define RSIF_ERR_BUFF_FIFO_EMPTY_RXPRT00_RD(src)      (((src) & 0x00000008)>>3)
#define RSIF_ERR_BUFF_FIFO_EMPTY_RXPRT00_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields rsif_clebuff_fifo_empty_rxprt0	 */
#define RSIF_CLEBUFF_FIFO_EMPTY_RXPRT00_WIDTH                                 1
#define RSIF_CLEBUFF_FIFO_EMPTY_RXPRT00_SHIFT                                 2
#define RSIF_CLEBUFF_FIFO_EMPTY_RXPRT00_MASK                         0x00000004
#define RSIF_CLEBUFF_FIFO_EMPTY_RXPRT00_RD(src)       (((src) & 0x00000004)>>2)
#define RSIF_CLEBUFF_FIFO_EMPTY_RXPRT00_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields rsif_ctrlbuff_fifo_empty_rxprt0	 */
#define RSIF_CTRLBUFF_FIFO_EMPTY_RXPRT00_WIDTH                                1
#define RSIF_CTRLBUFF_FIFO_EMPTY_RXPRT00_SHIFT                                1
#define RSIF_CTRLBUFF_FIFO_EMPTY_RXPRT00_MASK                        0x00000002
#define RSIF_CTRLBUFF_FIFO_EMPTY_RXPRT00_RD(src)      (((src) & 0x00000002)>>1)
#define RSIF_CTRLBUFF_FIFO_EMPTY_RXPRT00_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields rsif_buf_fifo_empty_rxprt0	 */
#define RSIF_BUF_FIFO_EMPTY_RXPRT00_WIDTH                                     1
#define RSIF_BUF_FIFO_EMPTY_RXPRT00_SHIFT                                     0
#define RSIF_BUF_FIFO_EMPTY_RXPRT00_MASK                             0x00000001
#define RSIF_BUF_FIFO_EMPTY_RXPRT00_RD(src)              (((src) & 0x00000001))
#define RSIF_BUF_FIFO_EMPTY_RXPRT00_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register RSIF_Int_Reg0	*/ 
/*	 Fields rsif_lcl_rxbuf_fifo_overfl_intr_rxprt1	 */
#define RSIF_LCL_RXBUF_FIFO_OVERFL_INTR_RXPRT10_WIDTH                         1
#define RSIF_LCL_RXBUF_FIFO_OVERFL_INTR_RXPRT10_SHIFT                        29
#define RSIF_LCL_RXBUF_FIFO_OVERFL_INTR_RXPRT10_MASK                 0x20000000
#define RSIF_LCL_RXBUF_FIFO_OVERFL_INTR_RXPRT10_RD(src) \
                                                    (((src) & 0x20000000)>>29)
#define RSIF_LCL_RXBUF_FIFO_OVERFL_INTR_RXPRT10_WR(src) \
                                                (((u32)(src)<<29) & 0x20000000)
#define RSIF_LCL_RXBUF_FIFO_OVERFL_INTR_RXPRT10_SET(dst,src) \
                      (((dst) & ~0x20000000) | (((u32)(src)<<29) & 0x20000000))
/*	 Fields rsif_lcl_rxbuf_fifo_underfl_intr_rxprt1	 */
#define RSIF_LCL_RXBUF_FIFO_UNDERFL_INTR_RXPRT10_WIDTH                        1
#define RSIF_LCL_RXBUF_FIFO_UNDERFL_INTR_RXPRT10_SHIFT                       28
#define RSIF_LCL_RXBUF_FIFO_UNDERFL_INTR_RXPRT10_MASK                0x10000000
#define RSIF_LCL_RXBUF_FIFO_UNDERFL_INTR_RXPRT10_RD(src) \
                                                    (((src) & 0x10000000)>>28)
#define RSIF_LCL_RXBUF_FIFO_UNDERFL_INTR_RXPRT10_WR(src) \
                                                (((u32)(src)<<28) & 0x10000000)
#define RSIF_LCL_RXBUF_FIFO_UNDERFL_INTR_RXPRT10_SET(dst,src) \
                      (((dst) & ~0x10000000) | (((u32)(src)<<28) & 0x10000000))
/*	 Fields rsif_chksum_buff_fifo_overfl_intr_rxprt1	 */
#define RSIF_CHKSUM_BUFF_FIFO_OVERFL_INTR_RXPRT10_WIDTH                       1
#define RSIF_CHKSUM_BUFF_FIFO_OVERFL_INTR_RXPRT10_SHIFT                      27
#define RSIF_CHKSUM_BUFF_FIFO_OVERFL_INTR_RXPRT10_MASK               0x08000000
#define RSIF_CHKSUM_BUFF_FIFO_OVERFL_INTR_RXPRT10_RD(src) \
                                                    (((src) & 0x08000000)>>27)
#define RSIF_CHKSUM_BUFF_FIFO_OVERFL_INTR_RXPRT10_WR(src) \
                                                (((u32)(src)<<27) & 0x08000000)
#define RSIF_CHKSUM_BUFF_FIFO_OVERFL_INTR_RXPRT10_SET(dst,src) \
                      (((dst) & ~0x08000000) | (((u32)(src)<<27) & 0x08000000))
/*	 Fields rsif_chksum_buff_fifo_underfl_intr_rxprt1	 */
#define RSIF_CHKSUM_BUFF_FIFO_UNDERFL_INTR_RXPRT10_WIDTH                      1
#define RSIF_CHKSUM_BUFF_FIFO_UNDERFL_INTR_RXPRT10_SHIFT                     26
#define RSIF_CHKSUM_BUFF_FIFO_UNDERFL_INTR_RXPRT10_MASK              0x04000000
#define RSIF_CHKSUM_BUFF_FIFO_UNDERFL_INTR_RXPRT10_RD(src) \
                                                    (((src) & 0x04000000)>>26)
#define RSIF_CHKSUM_BUFF_FIFO_UNDERFL_INTR_RXPRT10_WR(src) \
                                                (((u32)(src)<<26) & 0x04000000)
#define RSIF_CHKSUM_BUFF_FIFO_UNDERFL_INTR_RXPRT10_SET(dst,src) \
                      (((dst) & ~0x04000000) | (((u32)(src)<<26) & 0x04000000))
/*	 Fields rsif_timestamp_buff_fifo_overfl_intr_rxprt1	 */
#define RSIF_TIMESTAMP_BUFF_FIFO_OVERFL_INTR_RXPRT10_WIDTH                    1
#define RSIF_TIMESTAMP_BUFF_FIFO_OVERFL_INTR_RXPRT10_SHIFT                   25
#define RSIF_TIMESTAMP_BUFF_FIFO_OVERFL_INTR_RXPRT10_MASK            0x02000000
#define RSIF_TIMESTAMP_BUFF_FIFO_OVERFL_INTR_RXPRT10_RD(src) \
                                                    (((src) & 0x02000000)>>25)
#define RSIF_TIMESTAMP_BUFF_FIFO_OVERFL_INTR_RXPRT10_WR(src) \
                                                (((u32)(src)<<25) & 0x02000000)
#define RSIF_TIMESTAMP_BUFF_FIFO_OVERFL_INTR_RXPRT10_SET(dst,src) \
                      (((dst) & ~0x02000000) | (((u32)(src)<<25) & 0x02000000))
/*	 Fields rsif_timestamp_buff_fifo_underfl_intr_rxprt1	 */
#define RSIF_TIMESTAMP_BUFF_FIFO_UNDERFL_INTR_RXPRT10_WIDTH                   1
#define RSIF_TIMESTAMP_BUFF_FIFO_UNDERFL_INTR_RXPRT10_SHIFT                  24
#define RSIF_TIMESTAMP_BUFF_FIFO_UNDERFL_INTR_RXPRT10_MASK           0x01000000
#define RSIF_TIMESTAMP_BUFF_FIFO_UNDERFL_INTR_RXPRT10_RD(src) \
                                                    (((src) & 0x01000000)>>24)
#define RSIF_TIMESTAMP_BUFF_FIFO_UNDERFL_INTR_RXPRT10_WR(src) \
                                                (((u32)(src)<<24) & 0x01000000)
#define RSIF_TIMESTAMP_BUFF_FIFO_UNDERFL_INTR_RXPRT10_SET(dst,src) \
                      (((dst) & ~0x01000000) | (((u32)(src)<<24) & 0x01000000))
/*	 Fields rsif_err_buff_fifo_overfl_intr_rxprt1	 */
#define RSIF_ERR_BUFF_FIFO_OVERFL_INTR_RXPRT10_WIDTH                          1
#define RSIF_ERR_BUFF_FIFO_OVERFL_INTR_RXPRT10_SHIFT                         23
#define RSIF_ERR_BUFF_FIFO_OVERFL_INTR_RXPRT10_MASK                  0x00800000
#define RSIF_ERR_BUFF_FIFO_OVERFL_INTR_RXPRT10_RD(src) \
                                                    (((src) & 0x00800000)>>23)
#define RSIF_ERR_BUFF_FIFO_OVERFL_INTR_RXPRT10_WR(src) \
                                                (((u32)(src)<<23) & 0x00800000)
#define RSIF_ERR_BUFF_FIFO_OVERFL_INTR_RXPRT10_SET(dst,src) \
                      (((dst) & ~0x00800000) | (((u32)(src)<<23) & 0x00800000))
/*	 Fields rsif_err_buff_fifo_underfl_intr_rxprt1	 */
#define RSIF_ERR_BUFF_FIFO_UNDERFL_INTR_RXPRT10_WIDTH                         1
#define RSIF_ERR_BUFF_FIFO_UNDERFL_INTR_RXPRT10_SHIFT                        22
#define RSIF_ERR_BUFF_FIFO_UNDERFL_INTR_RXPRT10_MASK                 0x00400000
#define RSIF_ERR_BUFF_FIFO_UNDERFL_INTR_RXPRT10_RD(src) \
                                                    (((src) & 0x00400000)>>22)
#define RSIF_ERR_BUFF_FIFO_UNDERFL_INTR_RXPRT10_WR(src) \
                                                (((u32)(src)<<22) & 0x00400000)
#define RSIF_ERR_BUFF_FIFO_UNDERFL_INTR_RXPRT10_SET(dst,src) \
                      (((dst) & ~0x00400000) | (((u32)(src)<<22) & 0x00400000))
/*	 Fields rsif_clebuff_fifo_overfl_intr_rxprt1	 */
#define RSIF_CLEBUFF_FIFO_OVERFL_INTR_RXPRT10_WIDTH                           1
#define RSIF_CLEBUFF_FIFO_OVERFL_INTR_RXPRT10_SHIFT                          21
#define RSIF_CLEBUFF_FIFO_OVERFL_INTR_RXPRT10_MASK                   0x00200000
#define RSIF_CLEBUFF_FIFO_OVERFL_INTR_RXPRT10_RD(src) \
                                                    (((src) & 0x00200000)>>21)
#define RSIF_CLEBUFF_FIFO_OVERFL_INTR_RXPRT10_WR(src) \
                                                (((u32)(src)<<21) & 0x00200000)
#define RSIF_CLEBUFF_FIFO_OVERFL_INTR_RXPRT10_SET(dst,src) \
                      (((dst) & ~0x00200000) | (((u32)(src)<<21) & 0x00200000))
/*	 Fields rsif_clebuff_fifo_underfl_intr_rxprt1	 */
#define RSIF_CLEBUFF_FIFO_UNDERFL_INTR_RXPRT10_WIDTH                          1
#define RSIF_CLEBUFF_FIFO_UNDERFL_INTR_RXPRT10_SHIFT                         20
#define RSIF_CLEBUFF_FIFO_UNDERFL_INTR_RXPRT10_MASK                  0x00100000
#define RSIF_CLEBUFF_FIFO_UNDERFL_INTR_RXPRT10_RD(src) \
                                                    (((src) & 0x00100000)>>20)
#define RSIF_CLEBUFF_FIFO_UNDERFL_INTR_RXPRT10_WR(src) \
                                                (((u32)(src)<<20) & 0x00100000)
#define RSIF_CLEBUFF_FIFO_UNDERFL_INTR_RXPRT10_SET(dst,src) \
                      (((dst) & ~0x00100000) | (((u32)(src)<<20) & 0x00100000))
/*	 Fields rsif_ctrlbuff_fifo_overfl_intr_rxprt1	 */
#define RSIF_CTRLBUFF_FIFO_OVERFL_INTR_RXPRT10_WIDTH                          1
#define RSIF_CTRLBUFF_FIFO_OVERFL_INTR_RXPRT10_SHIFT                         19
#define RSIF_CTRLBUFF_FIFO_OVERFL_INTR_RXPRT10_MASK                  0x00080000
#define RSIF_CTRLBUFF_FIFO_OVERFL_INTR_RXPRT10_RD(src) \
                                                    (((src) & 0x00080000)>>19)
#define RSIF_CTRLBUFF_FIFO_OVERFL_INTR_RXPRT10_WR(src) \
                                                (((u32)(src)<<19) & 0x00080000)
#define RSIF_CTRLBUFF_FIFO_OVERFL_INTR_RXPRT10_SET(dst,src) \
                      (((dst) & ~0x00080000) | (((u32)(src)<<19) & 0x00080000))
/*	 Fields rsif_ctrlbuff_fifo_underfl_intr_rxprt1	 */
#define RSIF_CTRLBUFF_FIFO_UNDERFL_INTR_RXPRT10_WIDTH                         1
#define RSIF_CTRLBUFF_FIFO_UNDERFL_INTR_RXPRT10_SHIFT                        18
#define RSIF_CTRLBUFF_FIFO_UNDERFL_INTR_RXPRT10_MASK                 0x00040000
#define RSIF_CTRLBUFF_FIFO_UNDERFL_INTR_RXPRT10_RD(src) \
                                                    (((src) & 0x00040000)>>18)
#define RSIF_CTRLBUFF_FIFO_UNDERFL_INTR_RXPRT10_WR(src) \
                                                (((u32)(src)<<18) & 0x00040000)
#define RSIF_CTRLBUFF_FIFO_UNDERFL_INTR_RXPRT10_SET(dst,src) \
                      (((dst) & ~0x00040000) | (((u32)(src)<<18) & 0x00040000))
/*	 Fields rsif_buf_fifo_overfl_intr_rxprt1	 */
#define RSIF_BUF_FIFO_OVERFL_INTR_RXPRT10_WIDTH                               1
#define RSIF_BUF_FIFO_OVERFL_INTR_RXPRT10_SHIFT                              17
#define RSIF_BUF_FIFO_OVERFL_INTR_RXPRT10_MASK                       0x00020000
#define RSIF_BUF_FIFO_OVERFL_INTR_RXPRT10_RD(src)    (((src) & 0x00020000)>>17)
#define RSIF_BUF_FIFO_OVERFL_INTR_RXPRT10_WR(src) \
                                                (((u32)(src)<<17) & 0x00020000)
#define RSIF_BUF_FIFO_OVERFL_INTR_RXPRT10_SET(dst,src) \
                      (((dst) & ~0x00020000) | (((u32)(src)<<17) & 0x00020000))
/*	 Fields rsif_buf_fifo_underfl_intr_rxprt1	 */
#define RSIF_BUF_FIFO_UNDERFL_INTR_RXPRT10_WIDTH                              1
#define RSIF_BUF_FIFO_UNDERFL_INTR_RXPRT10_SHIFT                             16
#define RSIF_BUF_FIFO_UNDERFL_INTR_RXPRT10_MASK                      0x00010000
#define RSIF_BUF_FIFO_UNDERFL_INTR_RXPRT10_RD(src)   (((src) & 0x00010000)>>16)
#define RSIF_BUF_FIFO_UNDERFL_INTR_RXPRT10_WR(src) \
                                                (((u32)(src)<<16) & 0x00010000)
#define RSIF_BUF_FIFO_UNDERFL_INTR_RXPRT10_SET(dst,src) \
                      (((dst) & ~0x00010000) | (((u32)(src)<<16) & 0x00010000))
/*	 Fields rsif_plc_clebuff_fifo_overfl_intr	 */
#define RSIF_PLC_CLEBUFF_FIFO_OVERFL_INTR0_F2_WIDTH                           1
#define RSIF_PLC_CLEBUFF_FIFO_OVERFL_INTR0_F2_SHIFT                          15
#define RSIF_PLC_CLEBUFF_FIFO_OVERFL_INTR0_F2_MASK                   0x00008000
#define RSIF_PLC_CLEBUFF_FIFO_OVERFL_INTR0_F2_RD(src) \
                                                    (((src) & 0x00008000)>>15)
#define RSIF_PLC_CLEBUFF_FIFO_OVERFL_INTR0_F2_WR(src) \
                                                (((u32)(src)<<15) & 0x00008000)
#define RSIF_PLC_CLEBUFF_FIFO_OVERFL_INTR0_F2_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))
/*	 Fields rsif_plc_clebuff_fifo_underfl_intr	 */
#define RSIF_PLC_CLEBUFF_FIFO_UNDERFL_INTR0_F2_WIDTH                          1
#define RSIF_PLC_CLEBUFF_FIFO_UNDERFL_INTR0_F2_SHIFT                         14
#define RSIF_PLC_CLEBUFF_FIFO_UNDERFL_INTR0_F2_MASK                  0x00004000
#define RSIF_PLC_CLEBUFF_FIFO_UNDERFL_INTR0_F2_RD(src) \
                                                    (((src) & 0x00004000)>>14)
#define RSIF_PLC_CLEBUFF_FIFO_UNDERFL_INTR0_F2_WR(src) \
                                                (((u32)(src)<<14) & 0x00004000)
#define RSIF_PLC_CLEBUFF_FIFO_UNDERFL_INTR0_F2_SET(dst,src) \
                      (((dst) & ~0x00004000) | (((u32)(src)<<14) & 0x00004000))
/*	 Fields rsif_lcl_rxbuf_fifo_overfl_intr_rxprt0	 */
#define RSIF_LCL_RXBUF_FIFO_OVERFL_INTR_RXPRT00_WIDTH                         1
#define RSIF_LCL_RXBUF_FIFO_OVERFL_INTR_RXPRT00_SHIFT                        13
#define RSIF_LCL_RXBUF_FIFO_OVERFL_INTR_RXPRT00_MASK                 0x00002000
#define RSIF_LCL_RXBUF_FIFO_OVERFL_INTR_RXPRT00_RD(src) \
                                                    (((src) & 0x00002000)>>13)
#define RSIF_LCL_RXBUF_FIFO_OVERFL_INTR_RXPRT00_WR(src) \
                                                (((u32)(src)<<13) & 0x00002000)
#define RSIF_LCL_RXBUF_FIFO_OVERFL_INTR_RXPRT00_SET(dst,src) \
                      (((dst) & ~0x00002000) | (((u32)(src)<<13) & 0x00002000))
/*	 Fields rsif_lcl_rxbuf_fifo_underfl_intr_rxprt0	 */
#define RSIF_LCL_RXBUF_FIFO_UNDERFL_INTR_RXPRT00_WIDTH                        1
#define RSIF_LCL_RXBUF_FIFO_UNDERFL_INTR_RXPRT00_SHIFT                       12
#define RSIF_LCL_RXBUF_FIFO_UNDERFL_INTR_RXPRT00_MASK                0x00001000
#define RSIF_LCL_RXBUF_FIFO_UNDERFL_INTR_RXPRT00_RD(src) \
                                                    (((src) & 0x00001000)>>12)
#define RSIF_LCL_RXBUF_FIFO_UNDERFL_INTR_RXPRT00_WR(src) \
                                                (((u32)(src)<<12) & 0x00001000)
#define RSIF_LCL_RXBUF_FIFO_UNDERFL_INTR_RXPRT00_SET(dst,src) \
                      (((dst) & ~0x00001000) | (((u32)(src)<<12) & 0x00001000))
/*	 Fields rsif_chksum_buff_fifo_overfl_intr_rxprt0	 */
#define RSIF_CHKSUM_BUFF_FIFO_OVERFL_INTR_RXPRT00_WIDTH                       1
#define RSIF_CHKSUM_BUFF_FIFO_OVERFL_INTR_RXPRT00_SHIFT                      11
#define RSIF_CHKSUM_BUFF_FIFO_OVERFL_INTR_RXPRT00_MASK               0x00000800
#define RSIF_CHKSUM_BUFF_FIFO_OVERFL_INTR_RXPRT00_RD(src) \
                                                    (((src) & 0x00000800)>>11)
#define RSIF_CHKSUM_BUFF_FIFO_OVERFL_INTR_RXPRT00_WR(src) \
                                                (((u32)(src)<<11) & 0x00000800)
#define RSIF_CHKSUM_BUFF_FIFO_OVERFL_INTR_RXPRT00_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*	 Fields rsif_chksum_buff_fifo_underfl_intr_rxprt0	 */
#define RSIF_CHKSUM_BUFF_FIFO_UNDERFL_INTR_RXPRT00_WIDTH                      1
#define RSIF_CHKSUM_BUFF_FIFO_UNDERFL_INTR_RXPRT00_SHIFT                     10
#define RSIF_CHKSUM_BUFF_FIFO_UNDERFL_INTR_RXPRT00_MASK              0x00000400
#define RSIF_CHKSUM_BUFF_FIFO_UNDERFL_INTR_RXPRT00_RD(src) \
                                                    (((src) & 0x00000400)>>10)
#define RSIF_CHKSUM_BUFF_FIFO_UNDERFL_INTR_RXPRT00_WR(src) \
                                                (((u32)(src)<<10) & 0x00000400)
#define RSIF_CHKSUM_BUFF_FIFO_UNDERFL_INTR_RXPRT00_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields rsif_timestamp_buff_fifo_overfl_intr_rxprt0	 */
#define RSIF_TIMESTAMP_BUFF_FIFO_OVERFL_INTR_RXPRT00_WIDTH                    1
#define RSIF_TIMESTAMP_BUFF_FIFO_OVERFL_INTR_RXPRT00_SHIFT                    9
#define RSIF_TIMESTAMP_BUFF_FIFO_OVERFL_INTR_RXPRT00_MASK            0x00000200
#define RSIF_TIMESTAMP_BUFF_FIFO_OVERFL_INTR_RXPRT00_RD(src) \
                                                     (((src) & 0x00000200)>>9)
#define RSIF_TIMESTAMP_BUFF_FIFO_OVERFL_INTR_RXPRT00_WR(src) \
                                                 (((u32)(src)<<9) & 0x00000200)
#define RSIF_TIMESTAMP_BUFF_FIFO_OVERFL_INTR_RXPRT00_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields rsif_timestamp_buff_fifo_underfl_intr_rxprt0	 */
#define RSIF_TIMESTAMP_BUFF_FIFO_UNDERFL_INTR_RXPRT00_WIDTH                   1
#define RSIF_TIMESTAMP_BUFF_FIFO_UNDERFL_INTR_RXPRT00_SHIFT                   8
#define RSIF_TIMESTAMP_BUFF_FIFO_UNDERFL_INTR_RXPRT00_MASK           0x00000100
#define RSIF_TIMESTAMP_BUFF_FIFO_UNDERFL_INTR_RXPRT00_RD(src) \
                                                     (((src) & 0x00000100)>>8)
#define RSIF_TIMESTAMP_BUFF_FIFO_UNDERFL_INTR_RXPRT00_WR(src) \
                                                 (((u32)(src)<<8) & 0x00000100)
#define RSIF_TIMESTAMP_BUFF_FIFO_UNDERFL_INTR_RXPRT00_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*	 Fields rsif_err_buff_fifo_overfl_intr_rxprt0	 */
#define RSIF_ERR_BUFF_FIFO_OVERFL_INTR_RXPRT00_WIDTH                          1
#define RSIF_ERR_BUFF_FIFO_OVERFL_INTR_RXPRT00_SHIFT                          7
#define RSIF_ERR_BUFF_FIFO_OVERFL_INTR_RXPRT00_MASK                  0x00000080
#define RSIF_ERR_BUFF_FIFO_OVERFL_INTR_RXPRT00_RD(src) \
                                                     (((src) & 0x00000080)>>7)
#define RSIF_ERR_BUFF_FIFO_OVERFL_INTR_RXPRT00_WR(src) \
                                                 (((u32)(src)<<7) & 0x00000080)
#define RSIF_ERR_BUFF_FIFO_OVERFL_INTR_RXPRT00_SET(dst,src) \
                       (((dst) & ~0x00000080) | (((u32)(src)<<7) & 0x00000080))
/*	 Fields rsif_err_buff_fifo_underfl_intr_rxprt0	 */
#define RSIF_ERR_BUFF_FIFO_UNDERFL_INTR_RXPRT00_WIDTH                         1
#define RSIF_ERR_BUFF_FIFO_UNDERFL_INTR_RXPRT00_SHIFT                         6
#define RSIF_ERR_BUFF_FIFO_UNDERFL_INTR_RXPRT00_MASK                 0x00000040
#define RSIF_ERR_BUFF_FIFO_UNDERFL_INTR_RXPRT00_RD(src) \
                                                     (((src) & 0x00000040)>>6)
#define RSIF_ERR_BUFF_FIFO_UNDERFL_INTR_RXPRT00_WR(src) \
                                                 (((u32)(src)<<6) & 0x00000040)
#define RSIF_ERR_BUFF_FIFO_UNDERFL_INTR_RXPRT00_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*	 Fields rsif_clebuff_fifo_overfl_intr_rxprt0	 */
#define RSIF_CLEBUFF_FIFO_OVERFL_INTR_RXPRT00_WIDTH                           1
#define RSIF_CLEBUFF_FIFO_OVERFL_INTR_RXPRT00_SHIFT                           5
#define RSIF_CLEBUFF_FIFO_OVERFL_INTR_RXPRT00_MASK                   0x00000020
#define RSIF_CLEBUFF_FIFO_OVERFL_INTR_RXPRT00_RD(src) \
                                                     (((src) & 0x00000020)>>5)
#define RSIF_CLEBUFF_FIFO_OVERFL_INTR_RXPRT00_WR(src) \
                                                 (((u32)(src)<<5) & 0x00000020)
#define RSIF_CLEBUFF_FIFO_OVERFL_INTR_RXPRT00_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields rsif_clebuff_fifo_underfl_intr_rxprt0	 */
#define RSIF_CLEBUFF_FIFO_UNDERFL_INTR_RXPRT00_WIDTH                          1
#define RSIF_CLEBUFF_FIFO_UNDERFL_INTR_RXPRT00_SHIFT                          4
#define RSIF_CLEBUFF_FIFO_UNDERFL_INTR_RXPRT00_MASK                  0x00000010
#define RSIF_CLEBUFF_FIFO_UNDERFL_INTR_RXPRT00_RD(src) \
                                                     (((src) & 0x00000010)>>4)
#define RSIF_CLEBUFF_FIFO_UNDERFL_INTR_RXPRT00_WR(src) \
                                                 (((u32)(src)<<4) & 0x00000010)
#define RSIF_CLEBUFF_FIFO_UNDERFL_INTR_RXPRT00_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields rsif_ctrlbuff_fifo_overfl_intr_rxprt0	 */
#define RSIF_CTRLBUFF_FIFO_OVERFL_INTR_RXPRT00_WIDTH                          1
#define RSIF_CTRLBUFF_FIFO_OVERFL_INTR_RXPRT00_SHIFT                          3
#define RSIF_CTRLBUFF_FIFO_OVERFL_INTR_RXPRT00_MASK                  0x00000008
#define RSIF_CTRLBUFF_FIFO_OVERFL_INTR_RXPRT00_RD(src) \
                                                     (((src) & 0x00000008)>>3)
#define RSIF_CTRLBUFF_FIFO_OVERFL_INTR_RXPRT00_WR(src) \
                                                 (((u32)(src)<<3) & 0x00000008)
#define RSIF_CTRLBUFF_FIFO_OVERFL_INTR_RXPRT00_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields rsif_ctrlbuff_fifo_underfl_intr_rxprt0	 */
#define RSIF_CTRLBUFF_FIFO_UNDERFL_INTR_RXPRT00_WIDTH                         1
#define RSIF_CTRLBUFF_FIFO_UNDERFL_INTR_RXPRT00_SHIFT                         2
#define RSIF_CTRLBUFF_FIFO_UNDERFL_INTR_RXPRT00_MASK                 0x00000004
#define RSIF_CTRLBUFF_FIFO_UNDERFL_INTR_RXPRT00_RD(src) \
                                                     (((src) & 0x00000004)>>2)
#define RSIF_CTRLBUFF_FIFO_UNDERFL_INTR_RXPRT00_WR(src) \
                                                 (((u32)(src)<<2) & 0x00000004)
#define RSIF_CTRLBUFF_FIFO_UNDERFL_INTR_RXPRT00_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields rsif_buf_fifo_overfl_intr_rxprt0	 */
#define RSIF_BUF_FIFO_OVERFL_INTR_RXPRT00_WIDTH                               1
#define RSIF_BUF_FIFO_OVERFL_INTR_RXPRT00_SHIFT                               1
#define RSIF_BUF_FIFO_OVERFL_INTR_RXPRT00_MASK                       0x00000002
#define RSIF_BUF_FIFO_OVERFL_INTR_RXPRT00_RD(src)     (((src) & 0x00000002)>>1)
#define RSIF_BUF_FIFO_OVERFL_INTR_RXPRT00_WR(src) \
                                                 (((u32)(src)<<1) & 0x00000002)
#define RSIF_BUF_FIFO_OVERFL_INTR_RXPRT00_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields rsif_buf_fifo_underfl_intr_rxprt0	 */
#define RSIF_BUF_FIFO_UNDERFL_INTR_RXPRT00_WIDTH                              1
#define RSIF_BUF_FIFO_UNDERFL_INTR_RXPRT00_SHIFT                              0
#define RSIF_BUF_FIFO_UNDERFL_INTR_RXPRT00_MASK                      0x00000001
#define RSIF_BUF_FIFO_UNDERFL_INTR_RXPRT00_RD(src)       (((src) & 0x00000001))
#define RSIF_BUF_FIFO_UNDERFL_INTR_RXPRT00_WR(src)  (((u32)(src)) & 0x00000001)
#define RSIF_BUF_FIFO_UNDERFL_INTR_RXPRT00_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register RSIF_Int_Reg0Mask	*/
/*    Mask Register Fields rsif_lcl_rxbuf_fifo_overfl_intr_rxprt1Mask    */
#define RSIF_LCL_RXBUF_FIFO_OVERFL_INTR_RXPRT1MASK_WIDTH                      1
#define RSIF_LCL_RXBUF_FIFO_OVERFL_INTR_RXPRT1MASK_SHIFT                     29
#define RSIF_LCL_RXBUF_FIFO_OVERFL_INTR_RXPRT1MASK_MASK              0x20000000
#define RSIF_LCL_RXBUF_FIFO_OVERFL_INTR_RXPRT1MASK_RD(src) \
                                                    (((src) & 0x20000000)>>29)
#define RSIF_LCL_RXBUF_FIFO_OVERFL_INTR_RXPRT1MASK_WR(src) \
                                                (((u32)(src)<<29) & 0x20000000)
#define RSIF_LCL_RXBUF_FIFO_OVERFL_INTR_RXPRT1MASK_SET(dst,src) \
                      (((dst) & ~0x20000000) | (((u32)(src)<<29) & 0x20000000))
/*    Mask Register Fields rsif_lcl_rxbuf_fifo_underfl_intr_rxprt1Mask    */
#define RSIF_LCL_RXBUF_FIFO_UNDERFL_INTR_RXPRT1MASK_WIDTH                     1
#define RSIF_LCL_RXBUF_FIFO_UNDERFL_INTR_RXPRT1MASK_SHIFT                    28
#define RSIF_LCL_RXBUF_FIFO_UNDERFL_INTR_RXPRT1MASK_MASK             0x10000000
#define RSIF_LCL_RXBUF_FIFO_UNDERFL_INTR_RXPRT1MASK_RD(src) \
                                                    (((src) & 0x10000000)>>28)
#define RSIF_LCL_RXBUF_FIFO_UNDERFL_INTR_RXPRT1MASK_WR(src) \
                                                (((u32)(src)<<28) & 0x10000000)
#define RSIF_LCL_RXBUF_FIFO_UNDERFL_INTR_RXPRT1MASK_SET(dst,src) \
                      (((dst) & ~0x10000000) | (((u32)(src)<<28) & 0x10000000))
/*    Mask Register Fields rsif_chksum_buff_fifo_overfl_intr_rxprt1Mask    */
#define RSIF_CHKSUM_BUFF_FIFO_OVERFL_INTR_RXPRT1MASK_WIDTH                    1
#define RSIF_CHKSUM_BUFF_FIFO_OVERFL_INTR_RXPRT1MASK_SHIFT                   27
#define RSIF_CHKSUM_BUFF_FIFO_OVERFL_INTR_RXPRT1MASK_MASK            0x08000000
#define RSIF_CHKSUM_BUFF_FIFO_OVERFL_INTR_RXPRT1MASK_RD(src) \
                                                    (((src) & 0x08000000)>>27)
#define RSIF_CHKSUM_BUFF_FIFO_OVERFL_INTR_RXPRT1MASK_WR(src) \
                                                (((u32)(src)<<27) & 0x08000000)
#define RSIF_CHKSUM_BUFF_FIFO_OVERFL_INTR_RXPRT1MASK_SET(dst,src) \
                      (((dst) & ~0x08000000) | (((u32)(src)<<27) & 0x08000000))
/*    Mask Register Fields rsif_chksum_buff_fifo_underfl_intr_rxprt1Mask    */
#define RSIF_CHKSUM_BUFF_FIFO_UNDERFL_INTR_RXPRT1MASK_WIDTH                   1
#define RSIF_CHKSUM_BUFF_FIFO_UNDERFL_INTR_RXPRT1MASK_SHIFT                  26
#define RSIF_CHKSUM_BUFF_FIFO_UNDERFL_INTR_RXPRT1MASK_MASK           0x04000000
#define RSIF_CHKSUM_BUFF_FIFO_UNDERFL_INTR_RXPRT1MASK_RD(src) \
                                                    (((src) & 0x04000000)>>26)
#define RSIF_CHKSUM_BUFF_FIFO_UNDERFL_INTR_RXPRT1MASK_WR(src) \
                                                (((u32)(src)<<26) & 0x04000000)
#define RSIF_CHKSUM_BUFF_FIFO_UNDERFL_INTR_RXPRT1MASK_SET(dst,src) \
                      (((dst) & ~0x04000000) | (((u32)(src)<<26) & 0x04000000))
/*    Mask Register Fields rsif_timestamp_buff_fifo_overfl_intr_rxprt1Mask    */
#define RSIF_TIMESTAMP_BUFF_FIFO_OVERFL_INTR_RXPRT1MASK_WIDTH                 1
#define RSIF_TIMESTAMP_BUFF_FIFO_OVERFL_INTR_RXPRT1MASK_SHIFT                25
#define RSIF_TIMESTAMP_BUFF_FIFO_OVERFL_INTR_RXPRT1MASK_MASK         0x02000000
#define RSIF_TIMESTAMP_BUFF_FIFO_OVERFL_INTR_RXPRT1MASK_RD(src) \
                                                    (((src) & 0x02000000)>>25)
#define RSIF_TIMESTAMP_BUFF_FIFO_OVERFL_INTR_RXPRT1MASK_WR(src) \
                                                (((u32)(src)<<25) & 0x02000000)
#define RSIF_TIMESTAMP_BUFF_FIFO_OVERFL_INTR_RXPRT1MASK_SET(dst,src) \
                      (((dst) & ~0x02000000) | (((u32)(src)<<25) & 0x02000000))
/*    Mask Register Fields rsif_timestamp_buff_fifo_underfl_intr_rxprt1Mask    */
#define RSIF_TIMESTAMP_BUFF_FIFO_UNDERFL_INTR_RXPRT1MASK_WIDTH                1
#define RSIF_TIMESTAMP_BUFF_FIFO_UNDERFL_INTR_RXPRT1MASK_SHIFT               24
#define RSIF_TIMESTAMP_BUFF_FIFO_UNDERFL_INTR_RXPRT1MASK_MASK        0x01000000
#define RSIF_TIMESTAMP_BUFF_FIFO_UNDERFL_INTR_RXPRT1MASK_RD(src) \
                                                    (((src) & 0x01000000)>>24)
#define RSIF_TIMESTAMP_BUFF_FIFO_UNDERFL_INTR_RXPRT1MASK_WR(src) \
                                                (((u32)(src)<<24) & 0x01000000)
#define RSIF_TIMESTAMP_BUFF_FIFO_UNDERFL_INTR_RXPRT1MASK_SET(dst,src) \
                      (((dst) & ~0x01000000) | (((u32)(src)<<24) & 0x01000000))
/*    Mask Register Fields rsif_err_buff_fifo_overfl_intr_rxprt1Mask    */
#define RSIF_ERR_BUFF_FIFO_OVERFL_INTR_RXPRT1MASK_WIDTH                       1
#define RSIF_ERR_BUFF_FIFO_OVERFL_INTR_RXPRT1MASK_SHIFT                      23
#define RSIF_ERR_BUFF_FIFO_OVERFL_INTR_RXPRT1MASK_MASK               0x00800000
#define RSIF_ERR_BUFF_FIFO_OVERFL_INTR_RXPRT1MASK_RD(src) \
                                                    (((src) & 0x00800000)>>23)
#define RSIF_ERR_BUFF_FIFO_OVERFL_INTR_RXPRT1MASK_WR(src) \
                                                (((u32)(src)<<23) & 0x00800000)
#define RSIF_ERR_BUFF_FIFO_OVERFL_INTR_RXPRT1MASK_SET(dst,src) \
                      (((dst) & ~0x00800000) | (((u32)(src)<<23) & 0x00800000))
/*    Mask Register Fields rsif_err_buff_fifo_underfl_intr_rxprt1Mask    */
#define RSIF_ERR_BUFF_FIFO_UNDERFL_INTR_RXPRT1MASK_WIDTH                      1
#define RSIF_ERR_BUFF_FIFO_UNDERFL_INTR_RXPRT1MASK_SHIFT                     22
#define RSIF_ERR_BUFF_FIFO_UNDERFL_INTR_RXPRT1MASK_MASK              0x00400000
#define RSIF_ERR_BUFF_FIFO_UNDERFL_INTR_RXPRT1MASK_RD(src) \
                                                    (((src) & 0x00400000)>>22)
#define RSIF_ERR_BUFF_FIFO_UNDERFL_INTR_RXPRT1MASK_WR(src) \
                                                (((u32)(src)<<22) & 0x00400000)
#define RSIF_ERR_BUFF_FIFO_UNDERFL_INTR_RXPRT1MASK_SET(dst,src) \
                      (((dst) & ~0x00400000) | (((u32)(src)<<22) & 0x00400000))
/*    Mask Register Fields rsif_clebuff_fifo_overfl_intr_rxprt1Mask    */
#define RSIF_CLEBUFF_FIFO_OVERFL_INTR_RXPRT1MASK_WIDTH                        1
#define RSIF_CLEBUFF_FIFO_OVERFL_INTR_RXPRT1MASK_SHIFT                       21
#define RSIF_CLEBUFF_FIFO_OVERFL_INTR_RXPRT1MASK_MASK                0x00200000
#define RSIF_CLEBUFF_FIFO_OVERFL_INTR_RXPRT1MASK_RD(src) \
                                                    (((src) & 0x00200000)>>21)
#define RSIF_CLEBUFF_FIFO_OVERFL_INTR_RXPRT1MASK_WR(src) \
                                                (((u32)(src)<<21) & 0x00200000)
#define RSIF_CLEBUFF_FIFO_OVERFL_INTR_RXPRT1MASK_SET(dst,src) \
                      (((dst) & ~0x00200000) | (((u32)(src)<<21) & 0x00200000))
/*    Mask Register Fields rsif_clebuff_fifo_underfl_intr_rxprt1Mask    */
#define RSIF_CLEBUFF_FIFO_UNDERFL_INTR_RXPRT1MASK_WIDTH                       1
#define RSIF_CLEBUFF_FIFO_UNDERFL_INTR_RXPRT1MASK_SHIFT                      20
#define RSIF_CLEBUFF_FIFO_UNDERFL_INTR_RXPRT1MASK_MASK               0x00100000
#define RSIF_CLEBUFF_FIFO_UNDERFL_INTR_RXPRT1MASK_RD(src) \
                                                    (((src) & 0x00100000)>>20)
#define RSIF_CLEBUFF_FIFO_UNDERFL_INTR_RXPRT1MASK_WR(src) \
                                                (((u32)(src)<<20) & 0x00100000)
#define RSIF_CLEBUFF_FIFO_UNDERFL_INTR_RXPRT1MASK_SET(dst,src) \
                      (((dst) & ~0x00100000) | (((u32)(src)<<20) & 0x00100000))
/*    Mask Register Fields rsif_ctrlbuff_fifo_overfl_intr_rxprt1Mask    */
#define RSIF_CTRLBUFF_FIFO_OVERFL_INTR_RXPRT1MASK_WIDTH                       1
#define RSIF_CTRLBUFF_FIFO_OVERFL_INTR_RXPRT1MASK_SHIFT                      19
#define RSIF_CTRLBUFF_FIFO_OVERFL_INTR_RXPRT1MASK_MASK               0x00080000
#define RSIF_CTRLBUFF_FIFO_OVERFL_INTR_RXPRT1MASK_RD(src) \
                                                    (((src) & 0x00080000)>>19)
#define RSIF_CTRLBUFF_FIFO_OVERFL_INTR_RXPRT1MASK_WR(src) \
                                                (((u32)(src)<<19) & 0x00080000)
#define RSIF_CTRLBUFF_FIFO_OVERFL_INTR_RXPRT1MASK_SET(dst,src) \
                      (((dst) & ~0x00080000) | (((u32)(src)<<19) & 0x00080000))
/*    Mask Register Fields rsif_ctrlbuff_fifo_underfl_intr_rxprt1Mask    */
#define RSIF_CTRLBUFF_FIFO_UNDERFL_INTR_RXPRT1MASK_WIDTH                      1
#define RSIF_CTRLBUFF_FIFO_UNDERFL_INTR_RXPRT1MASK_SHIFT                     18
#define RSIF_CTRLBUFF_FIFO_UNDERFL_INTR_RXPRT1MASK_MASK              0x00040000
#define RSIF_CTRLBUFF_FIFO_UNDERFL_INTR_RXPRT1MASK_RD(src) \
                                                    (((src) & 0x00040000)>>18)
#define RSIF_CTRLBUFF_FIFO_UNDERFL_INTR_RXPRT1MASK_WR(src) \
                                                (((u32)(src)<<18) & 0x00040000)
#define RSIF_CTRLBUFF_FIFO_UNDERFL_INTR_RXPRT1MASK_SET(dst,src) \
                      (((dst) & ~0x00040000) | (((u32)(src)<<18) & 0x00040000))
/*    Mask Register Fields rsif_buf_fifo_overfl_intr_rxprt1Mask    */
#define RSIF_BUF_FIFO_OVERFL_INTR_RXPRT1MASK_WIDTH                            1
#define RSIF_BUF_FIFO_OVERFL_INTR_RXPRT1MASK_SHIFT                           17
#define RSIF_BUF_FIFO_OVERFL_INTR_RXPRT1MASK_MASK                    0x00020000
#define RSIF_BUF_FIFO_OVERFL_INTR_RXPRT1MASK_RD(src) \
                                                    (((src) & 0x00020000)>>17)
#define RSIF_BUF_FIFO_OVERFL_INTR_RXPRT1MASK_WR(src) \
                                                (((u32)(src)<<17) & 0x00020000)
#define RSIF_BUF_FIFO_OVERFL_INTR_RXPRT1MASK_SET(dst,src) \
                      (((dst) & ~0x00020000) | (((u32)(src)<<17) & 0x00020000))
/*    Mask Register Fields rsif_buf_fifo_underfl_intr_rxprt1Mask    */
#define RSIF_BUF_FIFO_UNDERFL_INTR_RXPRT1MASK_WIDTH                           1
#define RSIF_BUF_FIFO_UNDERFL_INTR_RXPRT1MASK_SHIFT                          16
#define RSIF_BUF_FIFO_UNDERFL_INTR_RXPRT1MASK_MASK                   0x00010000
#define RSIF_BUF_FIFO_UNDERFL_INTR_RXPRT1MASK_RD(src) \
                                                    (((src) & 0x00010000)>>16)
#define RSIF_BUF_FIFO_UNDERFL_INTR_RXPRT1MASK_WR(src) \
                                                (((u32)(src)<<16) & 0x00010000)
#define RSIF_BUF_FIFO_UNDERFL_INTR_RXPRT1MASK_SET(dst,src) \
                      (((dst) & ~0x00010000) | (((u32)(src)<<16) & 0x00010000))
/*    Mask Register Fields rsif_plc_clebuff_fifo_overfl_intrMask    */
#define RSIF_PLC_CLEBUFF_FIFO_OVERFL_INTRMASK_F1_WIDTH                        1
#define RSIF_PLC_CLEBUFF_FIFO_OVERFL_INTRMASK_F1_SHIFT                       15
#define RSIF_PLC_CLEBUFF_FIFO_OVERFL_INTRMASK_F1_MASK                0x00008000
#define RSIF_PLC_CLEBUFF_FIFO_OVERFL_INTRMASK_F1_RD(src) \
                                                    (((src) & 0x00008000)>>15)
#define RSIF_PLC_CLEBUFF_FIFO_OVERFL_INTRMASK_F1_WR(src) \
                                                (((u32)(src)<<15) & 0x00008000)
#define RSIF_PLC_CLEBUFF_FIFO_OVERFL_INTRMASK_F1_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))
/*    Mask Register Fields rsif_plc_clebuff_fifo_underfl_intrMask    */
#define RSIF_PLC_CLEBUFF_FIFO_UNDERFL_INTRMASK_F1_WIDTH                       1
#define RSIF_PLC_CLEBUFF_FIFO_UNDERFL_INTRMASK_F1_SHIFT                      14
#define RSIF_PLC_CLEBUFF_FIFO_UNDERFL_INTRMASK_F1_MASK               0x00004000
#define RSIF_PLC_CLEBUFF_FIFO_UNDERFL_INTRMASK_F1_RD(src) \
                                                    (((src) & 0x00004000)>>14)
#define RSIF_PLC_CLEBUFF_FIFO_UNDERFL_INTRMASK_F1_WR(src) \
                                                (((u32)(src)<<14) & 0x00004000)
#define RSIF_PLC_CLEBUFF_FIFO_UNDERFL_INTRMASK_F1_SET(dst,src) \
                      (((dst) & ~0x00004000) | (((u32)(src)<<14) & 0x00004000))
/*    Mask Register Fields rsif_lcl_rxbuf_fifo_overfl_intr_rxprt0Mask    */
#define RSIF_LCL_RXBUF_FIFO_OVERFL_INTR_RXPRT0MASK_WIDTH                      1
#define RSIF_LCL_RXBUF_FIFO_OVERFL_INTR_RXPRT0MASK_SHIFT                     13
#define RSIF_LCL_RXBUF_FIFO_OVERFL_INTR_RXPRT0MASK_MASK              0x00002000
#define RSIF_LCL_RXBUF_FIFO_OVERFL_INTR_RXPRT0MASK_RD(src) \
                                                    (((src) & 0x00002000)>>13)
#define RSIF_LCL_RXBUF_FIFO_OVERFL_INTR_RXPRT0MASK_WR(src) \
                                                (((u32)(src)<<13) & 0x00002000)
#define RSIF_LCL_RXBUF_FIFO_OVERFL_INTR_RXPRT0MASK_SET(dst,src) \
                      (((dst) & ~0x00002000) | (((u32)(src)<<13) & 0x00002000))
/*    Mask Register Fields rsif_lcl_rxbuf_fifo_underfl_intr_rxprt0Mask    */
#define RSIF_LCL_RXBUF_FIFO_UNDERFL_INTR_RXPRT0MASK_WIDTH                     1
#define RSIF_LCL_RXBUF_FIFO_UNDERFL_INTR_RXPRT0MASK_SHIFT                    12
#define RSIF_LCL_RXBUF_FIFO_UNDERFL_INTR_RXPRT0MASK_MASK             0x00001000
#define RSIF_LCL_RXBUF_FIFO_UNDERFL_INTR_RXPRT0MASK_RD(src) \
                                                    (((src) & 0x00001000)>>12)
#define RSIF_LCL_RXBUF_FIFO_UNDERFL_INTR_RXPRT0MASK_WR(src) \
                                                (((u32)(src)<<12) & 0x00001000)
#define RSIF_LCL_RXBUF_FIFO_UNDERFL_INTR_RXPRT0MASK_SET(dst,src) \
                      (((dst) & ~0x00001000) | (((u32)(src)<<12) & 0x00001000))
/*    Mask Register Fields rsif_chksum_buff_fifo_overfl_intr_rxprt0Mask    */
#define RSIF_CHKSUM_BUFF_FIFO_OVERFL_INTR_RXPRT0MASK_WIDTH                    1
#define RSIF_CHKSUM_BUFF_FIFO_OVERFL_INTR_RXPRT0MASK_SHIFT                   11
#define RSIF_CHKSUM_BUFF_FIFO_OVERFL_INTR_RXPRT0MASK_MASK            0x00000800
#define RSIF_CHKSUM_BUFF_FIFO_OVERFL_INTR_RXPRT0MASK_RD(src) \
                                                    (((src) & 0x00000800)>>11)
#define RSIF_CHKSUM_BUFF_FIFO_OVERFL_INTR_RXPRT0MASK_WR(src) \
                                                (((u32)(src)<<11) & 0x00000800)
#define RSIF_CHKSUM_BUFF_FIFO_OVERFL_INTR_RXPRT0MASK_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*    Mask Register Fields rsif_chksum_buff_fifo_underfl_intr_rxprt0Mask    */
#define RSIF_CHKSUM_BUFF_FIFO_UNDERFL_INTR_RXPRT0MASK_WIDTH                   1
#define RSIF_CHKSUM_BUFF_FIFO_UNDERFL_INTR_RXPRT0MASK_SHIFT                  10
#define RSIF_CHKSUM_BUFF_FIFO_UNDERFL_INTR_RXPRT0MASK_MASK           0x00000400
#define RSIF_CHKSUM_BUFF_FIFO_UNDERFL_INTR_RXPRT0MASK_RD(src) \
                                                    (((src) & 0x00000400)>>10)
#define RSIF_CHKSUM_BUFF_FIFO_UNDERFL_INTR_RXPRT0MASK_WR(src) \
                                                (((u32)(src)<<10) & 0x00000400)
#define RSIF_CHKSUM_BUFF_FIFO_UNDERFL_INTR_RXPRT0MASK_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*    Mask Register Fields rsif_timestamp_buff_fifo_overfl_intr_rxprt0Mask    */
#define RSIF_TIMESTAMP_BUFF_FIFO_OVERFL_INTR_RXPRT0MASK_WIDTH                 1
#define RSIF_TIMESTAMP_BUFF_FIFO_OVERFL_INTR_RXPRT0MASK_SHIFT                 9
#define RSIF_TIMESTAMP_BUFF_FIFO_OVERFL_INTR_RXPRT0MASK_MASK         0x00000200
#define RSIF_TIMESTAMP_BUFF_FIFO_OVERFL_INTR_RXPRT0MASK_RD(src) \
                                                     (((src) & 0x00000200)>>9)
#define RSIF_TIMESTAMP_BUFF_FIFO_OVERFL_INTR_RXPRT0MASK_WR(src) \
                                                 (((u32)(src)<<9) & 0x00000200)
#define RSIF_TIMESTAMP_BUFF_FIFO_OVERFL_INTR_RXPRT0MASK_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*    Mask Register Fields rsif_timestamp_buff_fifo_underfl_intr_rxprt0Mask    */
#define RSIF_TIMESTAMP_BUFF_FIFO_UNDERFL_INTR_RXPRT0MASK_WIDTH                1
#define RSIF_TIMESTAMP_BUFF_FIFO_UNDERFL_INTR_RXPRT0MASK_SHIFT                8
#define RSIF_TIMESTAMP_BUFF_FIFO_UNDERFL_INTR_RXPRT0MASK_MASK        0x00000100
#define RSIF_TIMESTAMP_BUFF_FIFO_UNDERFL_INTR_RXPRT0MASK_RD(src) \
                                                     (((src) & 0x00000100)>>8)
#define RSIF_TIMESTAMP_BUFF_FIFO_UNDERFL_INTR_RXPRT0MASK_WR(src) \
                                                 (((u32)(src)<<8) & 0x00000100)
#define RSIF_TIMESTAMP_BUFF_FIFO_UNDERFL_INTR_RXPRT0MASK_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*    Mask Register Fields rsif_err_buff_fifo_overfl_intr_rxprt0Mask    */
#define RSIF_ERR_BUFF_FIFO_OVERFL_INTR_RXPRT0MASK_WIDTH                       1
#define RSIF_ERR_BUFF_FIFO_OVERFL_INTR_RXPRT0MASK_SHIFT                       7
#define RSIF_ERR_BUFF_FIFO_OVERFL_INTR_RXPRT0MASK_MASK               0x00000080
#define RSIF_ERR_BUFF_FIFO_OVERFL_INTR_RXPRT0MASK_RD(src) \
                                                     (((src) & 0x00000080)>>7)
#define RSIF_ERR_BUFF_FIFO_OVERFL_INTR_RXPRT0MASK_WR(src) \
                                                 (((u32)(src)<<7) & 0x00000080)
#define RSIF_ERR_BUFF_FIFO_OVERFL_INTR_RXPRT0MASK_SET(dst,src) \
                       (((dst) & ~0x00000080) | (((u32)(src)<<7) & 0x00000080))
/*    Mask Register Fields rsif_err_buff_fifo_underfl_intr_rxprt0Mask    */
#define RSIF_ERR_BUFF_FIFO_UNDERFL_INTR_RXPRT0MASK_WIDTH                      1
#define RSIF_ERR_BUFF_FIFO_UNDERFL_INTR_RXPRT0MASK_SHIFT                      6
#define RSIF_ERR_BUFF_FIFO_UNDERFL_INTR_RXPRT0MASK_MASK              0x00000040
#define RSIF_ERR_BUFF_FIFO_UNDERFL_INTR_RXPRT0MASK_RD(src) \
                                                     (((src) & 0x00000040)>>6)
#define RSIF_ERR_BUFF_FIFO_UNDERFL_INTR_RXPRT0MASK_WR(src) \
                                                 (((u32)(src)<<6) & 0x00000040)
#define RSIF_ERR_BUFF_FIFO_UNDERFL_INTR_RXPRT0MASK_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*    Mask Register Fields rsif_clebuff_fifo_overfl_intr_rxprt0Mask    */
#define RSIF_CLEBUFF_FIFO_OVERFL_INTR_RXPRT0MASK_WIDTH                        1
#define RSIF_CLEBUFF_FIFO_OVERFL_INTR_RXPRT0MASK_SHIFT                        5
#define RSIF_CLEBUFF_FIFO_OVERFL_INTR_RXPRT0MASK_MASK                0x00000020
#define RSIF_CLEBUFF_FIFO_OVERFL_INTR_RXPRT0MASK_RD(src) \
                                                     (((src) & 0x00000020)>>5)
#define RSIF_CLEBUFF_FIFO_OVERFL_INTR_RXPRT0MASK_WR(src) \
                                                 (((u32)(src)<<5) & 0x00000020)
#define RSIF_CLEBUFF_FIFO_OVERFL_INTR_RXPRT0MASK_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*    Mask Register Fields rsif_clebuff_fifo_underfl_intr_rxprt0Mask    */
#define RSIF_CLEBUFF_FIFO_UNDERFL_INTR_RXPRT0MASK_WIDTH                       1
#define RSIF_CLEBUFF_FIFO_UNDERFL_INTR_RXPRT0MASK_SHIFT                       4
#define RSIF_CLEBUFF_FIFO_UNDERFL_INTR_RXPRT0MASK_MASK               0x00000010
#define RSIF_CLEBUFF_FIFO_UNDERFL_INTR_RXPRT0MASK_RD(src) \
                                                     (((src) & 0x00000010)>>4)
#define RSIF_CLEBUFF_FIFO_UNDERFL_INTR_RXPRT0MASK_WR(src) \
                                                 (((u32)(src)<<4) & 0x00000010)
#define RSIF_CLEBUFF_FIFO_UNDERFL_INTR_RXPRT0MASK_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*    Mask Register Fields rsif_ctrlbuff_fifo_overfl_intr_rxprt0Mask    */
#define RSIF_CTRLBUFF_FIFO_OVERFL_INTR_RXPRT0MASK_WIDTH                       1
#define RSIF_CTRLBUFF_FIFO_OVERFL_INTR_RXPRT0MASK_SHIFT                       3
#define RSIF_CTRLBUFF_FIFO_OVERFL_INTR_RXPRT0MASK_MASK               0x00000008
#define RSIF_CTRLBUFF_FIFO_OVERFL_INTR_RXPRT0MASK_RD(src) \
                                                     (((src) & 0x00000008)>>3)
#define RSIF_CTRLBUFF_FIFO_OVERFL_INTR_RXPRT0MASK_WR(src) \
                                                 (((u32)(src)<<3) & 0x00000008)
#define RSIF_CTRLBUFF_FIFO_OVERFL_INTR_RXPRT0MASK_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*    Mask Register Fields rsif_ctrlbuff_fifo_underfl_intr_rxprt0Mask    */
#define RSIF_CTRLBUFF_FIFO_UNDERFL_INTR_RXPRT0MASK_WIDTH                      1
#define RSIF_CTRLBUFF_FIFO_UNDERFL_INTR_RXPRT0MASK_SHIFT                      2
#define RSIF_CTRLBUFF_FIFO_UNDERFL_INTR_RXPRT0MASK_MASK              0x00000004
#define RSIF_CTRLBUFF_FIFO_UNDERFL_INTR_RXPRT0MASK_RD(src) \
                                                     (((src) & 0x00000004)>>2)
#define RSIF_CTRLBUFF_FIFO_UNDERFL_INTR_RXPRT0MASK_WR(src) \
                                                 (((u32)(src)<<2) & 0x00000004)
#define RSIF_CTRLBUFF_FIFO_UNDERFL_INTR_RXPRT0MASK_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*    Mask Register Fields rsif_buf_fifo_overfl_intr_rxprt0Mask    */
#define RSIF_BUF_FIFO_OVERFL_INTR_RXPRT0MASK_WIDTH                            1
#define RSIF_BUF_FIFO_OVERFL_INTR_RXPRT0MASK_SHIFT                            1
#define RSIF_BUF_FIFO_OVERFL_INTR_RXPRT0MASK_MASK                    0x00000002
#define RSIF_BUF_FIFO_OVERFL_INTR_RXPRT0MASK_RD(src)  (((src) & 0x00000002)>>1)
#define RSIF_BUF_FIFO_OVERFL_INTR_RXPRT0MASK_WR(src) \
                                                 (((u32)(src)<<1) & 0x00000002)
#define RSIF_BUF_FIFO_OVERFL_INTR_RXPRT0MASK_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*    Mask Register Fields rsif_buf_fifo_underfl_intr_rxprt0Mask    */
#define RSIF_BUF_FIFO_UNDERFL_INTR_RXPRT0MASK_WIDTH                           1
#define RSIF_BUF_FIFO_UNDERFL_INTR_RXPRT0MASK_SHIFT                           0
#define RSIF_BUF_FIFO_UNDERFL_INTR_RXPRT0MASK_MASK                   0x00000001
#define RSIF_BUF_FIFO_UNDERFL_INTR_RXPRT0MASK_RD(src)    (((src) & 0x00000001))
#define RSIF_BUF_FIFO_UNDERFL_INTR_RXPRT0MASK_WR(src) \
                                                    (((u32)(src)) & 0x00000001)
#define RSIF_BUF_FIFO_UNDERFL_INTR_RXPRT0MASK_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register RSIF_FInt_Reg0	*/ 
/*	 Fields rsif_ss_mirrorerr_intr_rxprt1	 */
#define RSIF_SS_MIRRORERR_INTR_RXPRT10_WIDTH                                  1
#define RSIF_SS_MIRRORERR_INTR_RXPRT10_SHIFT                                 19
#define RSIF_SS_MIRRORERR_INTR_RXPRT10_MASK                          0x00080000
#define RSIF_SS_MIRRORERR_INTR_RXPRT10_RD(src)       (((src) & 0x00080000)>>19)
#define RSIF_SS_MIRRORERR_INTR_RXPRT10_WR(src)  (((u32)(src)<<19) & 0x00080000)
#define RSIF_SS_MIRRORERR_INTR_RXPRT10_SET(dst,src) \
                      (((dst) & ~0x00080000) | (((u32)(src)<<19) & 0x00080000))
/*	 Fields rsif_ss_split_boundary_intr_rxprt1	 */
#define RSIF_SS_SPLIT_BOUNDARY_INTR_RXPRT10_WIDTH                             1
#define RSIF_SS_SPLIT_BOUNDARY_INTR_RXPRT10_SHIFT                            18
#define RSIF_SS_SPLIT_BOUNDARY_INTR_RXPRT10_MASK                     0x00040000
#define RSIF_SS_SPLIT_BOUNDARY_INTR_RXPRT10_RD(src)  (((src) & 0x00040000)>>18)
#define RSIF_SS_SPLIT_BOUNDARY_INTR_RXPRT10_WR(src) \
                                                (((u32)(src)<<18) & 0x00040000)
#define RSIF_SS_SPLIT_BOUNDARY_INTR_RXPRT10_SET(dst,src) \
                      (((dst) & ~0x00040000) | (((u32)(src)<<18) & 0x00040000))
/*	 Fields rsif_ss_fpbuff_timeout_intr_rxprt1	 */
#define RSIF_SS_FPBUFF_TIMEOUT_INTR_RXPRT10_WIDTH                             1
#define RSIF_SS_FPBUFF_TIMEOUT_INTR_RXPRT10_SHIFT                            17
#define RSIF_SS_FPBUFF_TIMEOUT_INTR_RXPRT10_MASK                     0x00020000
#define RSIF_SS_FPBUFF_TIMEOUT_INTR_RXPRT10_RD(src)  (((src) & 0x00020000)>>17)
#define RSIF_SS_FPBUFF_TIMEOUT_INTR_RXPRT10_WR(src) \
                                                (((u32)(src)<<17) & 0x00020000)
#define RSIF_SS_FPBUFF_TIMEOUT_INTR_RXPRT10_SET(dst,src) \
                      (((dst) & ~0x00020000) | (((u32)(src)<<17) & 0x00020000))
/*	 Fields rsif_ss_axi_wrerr_intr_rxprt1	 */
#define RSIF_SS_AXI_WRERR_INTR_RXPRT10_WIDTH                                  1
#define RSIF_SS_AXI_WRERR_INTR_RXPRT10_SHIFT                                 16
#define RSIF_SS_AXI_WRERR_INTR_RXPRT10_MASK                          0x00010000
#define RSIF_SS_AXI_WRERR_INTR_RXPRT10_RD(src)       (((src) & 0x00010000)>>16)
#define RSIF_SS_AXI_WRERR_INTR_RXPRT10_WR(src)  (((u32)(src)<<16) & 0x00010000)
#define RSIF_SS_AXI_WRERR_INTR_RXPRT10_SET(dst,src) \
                      (((dst) & ~0x00010000) | (((u32)(src)<<16) & 0x00010000))
/*	 Fields rsif_ss_mirrorerr_intr_rxprt0	 */
#define RSIF_SS_MIRRORERR_INTR_RXPRT00_WIDTH                                  1
#define RSIF_SS_MIRRORERR_INTR_RXPRT00_SHIFT                                  4
#define RSIF_SS_MIRRORERR_INTR_RXPRT00_MASK                          0x00000010
#define RSIF_SS_MIRRORERR_INTR_RXPRT00_RD(src)        (((src) & 0x00000010)>>4)
#define RSIF_SS_MIRRORERR_INTR_RXPRT00_WR(src)   (((u32)(src)<<4) & 0x00000010)
#define RSIF_SS_MIRRORERR_INTR_RXPRT00_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields rsif_ss_split_boundary_intr_rxprt0	 */
#define RSIF_SS_SPLIT_BOUNDARY_INTR_RXPRT00_WIDTH                             1
#define RSIF_SS_SPLIT_BOUNDARY_INTR_RXPRT00_SHIFT                             3
#define RSIF_SS_SPLIT_BOUNDARY_INTR_RXPRT00_MASK                     0x00000008
#define RSIF_SS_SPLIT_BOUNDARY_INTR_RXPRT00_RD(src)   (((src) & 0x00000008)>>3)
#define RSIF_SS_SPLIT_BOUNDARY_INTR_RXPRT00_WR(src) \
                                                 (((u32)(src)<<3) & 0x00000008)
#define RSIF_SS_SPLIT_BOUNDARY_INTR_RXPRT00_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields rsif_ss_fpbuff_timeout_intr_rxprt0	 */
#define RSIF_SS_FPBUFF_TIMEOUT_INTR_RXPRT00_WIDTH                             1
#define RSIF_SS_FPBUFF_TIMEOUT_INTR_RXPRT00_SHIFT                             2
#define RSIF_SS_FPBUFF_TIMEOUT_INTR_RXPRT00_MASK                     0x00000004
#define RSIF_SS_FPBUFF_TIMEOUT_INTR_RXPRT00_RD(src)   (((src) & 0x00000004)>>2)
#define RSIF_SS_FPBUFF_TIMEOUT_INTR_RXPRT00_WR(src) \
                                                 (((u32)(src)<<2) & 0x00000004)
#define RSIF_SS_FPBUFF_TIMEOUT_INTR_RXPRT00_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields rsif_ss_axi_wrerr_intr_rxprt0	 */
#define RSIF_SS_AXI_WRERR_INTR_RXPRT00_WIDTH                                  1
#define RSIF_SS_AXI_WRERR_INTR_RXPRT00_SHIFT                                  1
#define RSIF_SS_AXI_WRERR_INTR_RXPRT00_MASK                          0x00000002
#define RSIF_SS_AXI_WRERR_INTR_RXPRT00_RD(src)        (((src) & 0x00000002)>>1)
#define RSIF_SS_AXI_WRERR_INTR_RXPRT00_WR(src)   (((u32)(src)<<1) & 0x00000002)
#define RSIF_SS_AXI_WRERR_INTR_RXPRT00_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields sts_rsif_plc_drop	 */
#define STS_RSIF_PLC_DROP0_F2_WIDTH                                           1
#define STS_RSIF_PLC_DROP0_F2_SHIFT                                           0
#define STS_RSIF_PLC_DROP0_F2_MASK                                   0x00000001
#define STS_RSIF_PLC_DROP0_F2_RD(src)                    (((src) & 0x00000001))
#define STS_RSIF_PLC_DROP0_F2_WR(src)               (((u32)(src)) & 0x00000001)
#define STS_RSIF_PLC_DROP0_F2_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register RSIF_FInt_Reg0Mask	*/
/*    Mask Register Fields rsif_ss_mirrorerr_intr_rxprt1Mask    */
#define RSIF_SS_MIRRORERR_INTR_RXPRT1MASK_WIDTH                               1
#define RSIF_SS_MIRRORERR_INTR_RXPRT1MASK_SHIFT                              19
#define RSIF_SS_MIRRORERR_INTR_RXPRT1MASK_MASK                       0x00080000
#define RSIF_SS_MIRRORERR_INTR_RXPRT1MASK_RD(src)    (((src) & 0x00080000)>>19)
#define RSIF_SS_MIRRORERR_INTR_RXPRT1MASK_WR(src) \
                                                (((u32)(src)<<19) & 0x00080000)
#define RSIF_SS_MIRRORERR_INTR_RXPRT1MASK_SET(dst,src) \
                      (((dst) & ~0x00080000) | (((u32)(src)<<19) & 0x00080000))
/*    Mask Register Fields rsif_ss_split_boundary_intr_rxprt1Mask    */
#define RSIF_SS_SPLIT_BOUNDARY_INTR_RXPRT1MASK_WIDTH                          1
#define RSIF_SS_SPLIT_BOUNDARY_INTR_RXPRT1MASK_SHIFT                         18
#define RSIF_SS_SPLIT_BOUNDARY_INTR_RXPRT1MASK_MASK                  0x00040000
#define RSIF_SS_SPLIT_BOUNDARY_INTR_RXPRT1MASK_RD(src) \
                                                    (((src) & 0x00040000)>>18)
#define RSIF_SS_SPLIT_BOUNDARY_INTR_RXPRT1MASK_WR(src) \
                                                (((u32)(src)<<18) & 0x00040000)
#define RSIF_SS_SPLIT_BOUNDARY_INTR_RXPRT1MASK_SET(dst,src) \
                      (((dst) & ~0x00040000) | (((u32)(src)<<18) & 0x00040000))
/*    Mask Register Fields rsif_ss_fpbuff_timeout_intr_rxprt1Mask    */
#define RSIF_SS_FPBUFF_TIMEOUT_INTR_RXPRT1MASK_WIDTH                          1
#define RSIF_SS_FPBUFF_TIMEOUT_INTR_RXPRT1MASK_SHIFT                         17
#define RSIF_SS_FPBUFF_TIMEOUT_INTR_RXPRT1MASK_MASK                  0x00020000
#define RSIF_SS_FPBUFF_TIMEOUT_INTR_RXPRT1MASK_RD(src) \
                                                    (((src) & 0x00020000)>>17)
#define RSIF_SS_FPBUFF_TIMEOUT_INTR_RXPRT1MASK_WR(src) \
                                                (((u32)(src)<<17) & 0x00020000)
#define RSIF_SS_FPBUFF_TIMEOUT_INTR_RXPRT1MASK_SET(dst,src) \
                      (((dst) & ~0x00020000) | (((u32)(src)<<17) & 0x00020000))
/*    Mask Register Fields rsif_ss_axi_wrerr_intr_rxprt1Mask    */
#define RSIF_SS_AXI_WRERR_INTR_RXPRT1MASK_WIDTH                               1
#define RSIF_SS_AXI_WRERR_INTR_RXPRT1MASK_SHIFT                              16
#define RSIF_SS_AXI_WRERR_INTR_RXPRT1MASK_MASK                       0x00010000
#define RSIF_SS_AXI_WRERR_INTR_RXPRT1MASK_RD(src)    (((src) & 0x00010000)>>16)
#define RSIF_SS_AXI_WRERR_INTR_RXPRT1MASK_WR(src) \
                                                (((u32)(src)<<16) & 0x00010000)
#define RSIF_SS_AXI_WRERR_INTR_RXPRT1MASK_SET(dst,src) \
                      (((dst) & ~0x00010000) | (((u32)(src)<<16) & 0x00010000))
/*    Mask Register Fields rsif_ss_mirrorerr_intr_rxprt0Mask    */
#define RSIF_SS_MIRRORERR_INTR_RXPRT0MASK_WIDTH                               1
#define RSIF_SS_MIRRORERR_INTR_RXPRT0MASK_SHIFT                               4
#define RSIF_SS_MIRRORERR_INTR_RXPRT0MASK_MASK                       0x00000010
#define RSIF_SS_MIRRORERR_INTR_RXPRT0MASK_RD(src)     (((src) & 0x00000010)>>4)
#define RSIF_SS_MIRRORERR_INTR_RXPRT0MASK_WR(src) \
                                                 (((u32)(src)<<4) & 0x00000010)
#define RSIF_SS_MIRRORERR_INTR_RXPRT0MASK_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*    Mask Register Fields rsif_ss_split_boundary_intr_rxprt0Mask    */
#define RSIF_SS_SPLIT_BOUNDARY_INTR_RXPRT0MASK_WIDTH                          1
#define RSIF_SS_SPLIT_BOUNDARY_INTR_RXPRT0MASK_SHIFT                          3
#define RSIF_SS_SPLIT_BOUNDARY_INTR_RXPRT0MASK_MASK                  0x00000008
#define RSIF_SS_SPLIT_BOUNDARY_INTR_RXPRT0MASK_RD(src) \
                                                     (((src) & 0x00000008)>>3)
#define RSIF_SS_SPLIT_BOUNDARY_INTR_RXPRT0MASK_WR(src) \
                                                 (((u32)(src)<<3) & 0x00000008)
#define RSIF_SS_SPLIT_BOUNDARY_INTR_RXPRT0MASK_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*    Mask Register Fields rsif_ss_fpbuff_timeout_intr_rxprt0Mask    */
#define RSIF_SS_FPBUFF_TIMEOUT_INTR_RXPRT0MASK_WIDTH                          1
#define RSIF_SS_FPBUFF_TIMEOUT_INTR_RXPRT0MASK_SHIFT                          2
#define RSIF_SS_FPBUFF_TIMEOUT_INTR_RXPRT0MASK_MASK                  0x00000004
#define RSIF_SS_FPBUFF_TIMEOUT_INTR_RXPRT0MASK_RD(src) \
                                                     (((src) & 0x00000004)>>2)
#define RSIF_SS_FPBUFF_TIMEOUT_INTR_RXPRT0MASK_WR(src) \
                                                 (((u32)(src)<<2) & 0x00000004)
#define RSIF_SS_FPBUFF_TIMEOUT_INTR_RXPRT0MASK_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*    Mask Register Fields rsif_ss_axi_wrerr_intr_rxprt0Mask    */
#define RSIF_SS_AXI_WRERR_INTR_RXPRT0MASK_WIDTH                               1
#define RSIF_SS_AXI_WRERR_INTR_RXPRT0MASK_SHIFT                               1
#define RSIF_SS_AXI_WRERR_INTR_RXPRT0MASK_MASK                       0x00000002
#define RSIF_SS_AXI_WRERR_INTR_RXPRT0MASK_RD(src)     (((src) & 0x00000002)>>1)
#define RSIF_SS_AXI_WRERR_INTR_RXPRT0MASK_WR(src) \
                                                 (((u32)(src)<<1) & 0x00000002)
#define RSIF_SS_AXI_WRERR_INTR_RXPRT0MASK_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*    Mask Register Fields sts_rsif_plc_dropMask    */
#define STS_RSIF_PLC_DROPMASK_F1_WIDTH                                        1
#define STS_RSIF_PLC_DROPMASK_F1_SHIFT                                        0
#define STS_RSIF_PLC_DROPMASK_F1_MASK                                0x00000001
#define STS_RSIF_PLC_DROPMASK_F1_RD(src)                 (((src) & 0x00000001))
#define STS_RSIF_PLC_DROPMASK_F1_WR(src)            (((u32)(src)) & 0x00000001)
#define STS_RSIF_PLC_DROPMASK_F1_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register RSIF_Sts_Reg0	*/ 
/*	 Fields rsif_ss_axi_wrerr_bresp_rxprt1	 */
#define RSIF_SS_AXI_WRERR_BRESP_RXPRT10_WIDTH                                 2
#define RSIF_SS_AXI_WRERR_BRESP_RXPRT10_SHIFT                                16
#define RSIF_SS_AXI_WRERR_BRESP_RXPRT10_MASK                         0x00030000
#define RSIF_SS_AXI_WRERR_BRESP_RXPRT10_RD(src)      (((src) & 0x00030000)>>16)
#define RSIF_SS_AXI_WRERR_BRESP_RXPRT10_SET(dst,src) \
                      (((dst) & ~0x00030000) | (((u32)(src)<<16) & 0x00030000))
/*	 Fields rsif_ss_axi_wrerr_bresp_rxprt0	 */
#define RSIF_SS_AXI_WRERR_BRESP_RXPRT00_WIDTH                                 2
#define RSIF_SS_AXI_WRERR_BRESP_RXPRT00_SHIFT                                 0
#define RSIF_SS_AXI_WRERR_BRESP_RXPRT00_MASK                         0x00000003
#define RSIF_SS_AXI_WRERR_BRESP_RXPRT00_RD(src)          (((src) & 0x00000003))
#define RSIF_SS_AXI_WRERR_BRESP_RXPRT00_SET(dst,src) \
                          (((dst) & ~0x00000003) | (((u32)(src)) & 0x00000003))

/*	Register RSIF_H0_0	*/ 
/*	 Fields info	 */
#define INFO0_WIDTH                                                          32
#define INFO0_SHIFT                                                           0
#define INFO0_MASK                                                   0xffffffff
#define INFO0_RD(src)                                    (((src) & 0xffffffff))
#define INFO0_WR(src)                               (((u32)(src)) & 0xffffffff)
#define INFO0_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register RSIF_H0_1	*/ 
/*	 Fields info	 */
#define INFO1_WIDTH                                                          10
#define INFO1_SHIFT                                                           0
#define INFO1_MASK                                                   0x000003ff
#define INFO1_RD(src)                                    (((src) & 0x000003ff))
#define INFO1_WR(src)                               (((u32)(src)) & 0x000003ff)
#define INFO1_SET(dst,src) \
                          (((dst) & ~0x000003ff) | (((u32)(src)) & 0x000003ff))

/*	Register RSIF_H1_0	*/ 
/*	 Fields info	 */
#define INFO0_F1_WIDTH                                                       32
#define INFO0_F1_SHIFT                                                        0
#define INFO0_F1_MASK                                                0xffffffff
#define INFO0_F1_RD(src)                                 (((src) & 0xffffffff))
#define INFO0_F1_WR(src)                            (((u32)(src)) & 0xffffffff)
#define INFO0_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register RSIF_H1_1	*/ 
/*	 Fields enable	 */
#define ENABLE1_WIDTH                                                         1
#define ENABLE1_SHIFT                                                        16
#define ENABLE1_MASK                                                 0x00010000
#define ENABLE1_RD(src)                              (((src) & 0x00010000)>>16)
#define ENABLE1_WR(src)                         (((u32)(src)<<16) & 0x00010000)
#define ENABLE1_SET(dst,src) \
                      (((dst) & ~0x00010000) | (((u32)(src)<<16) & 0x00010000))
/*	 Fields info	 */
#define INFO1_F1_WIDTH                                                       10
#define INFO1_F1_SHIFT                                                        0
#define INFO1_F1_MASK                                                0x000003ff
#define INFO1_F1_RD(src)                                 (((src) & 0x000003ff))
#define INFO1_F1_WR(src)                            (((u32)(src)) & 0x000003ff)
#define INFO1_F1_SET(dst,src) \
                          (((dst) & ~0x000003ff) | (((u32)(src)) & 0x000003ff))

/*	Register RSIF_FPbuff_Timeout_StsReg0	*/ 
/*	 Fields sts_rsif_fpbuff_timeout_dropcnt	 */
#define STS_RSIF_FPBUFF_TIMEOUT_DROPCNT0_F2_WIDTH                            32
#define STS_RSIF_FPBUFF_TIMEOUT_DROPCNT0_F2_SHIFT                             0
#define STS_RSIF_FPBUFF_TIMEOUT_DROPCNT0_F2_MASK                     0xffffffff
#define STS_RSIF_FPBUFF_TIMEOUT_DROPCNT0_F2_RD(src)      (((src) & 0xffffffff))
#define STS_RSIF_FPBUFF_TIMEOUT_DROPCNT0_F2_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register RSIF_Sts_Cntr_Reg0	*/ 
/*	 Fields sts_rsif_mirror_pkt_cntr_prt0	 */
#define STS_RSIF_MIRROR_PKT_CNTR_PRT00_WIDTH                                 16
#define STS_RSIF_MIRROR_PKT_CNTR_PRT00_SHIFT                                 16
#define STS_RSIF_MIRROR_PKT_CNTR_PRT00_MASK                          0xffff0000
#define STS_RSIF_MIRROR_PKT_CNTR_PRT00_RD(src)       (((src) & 0xffff0000)>>16)
#define STS_RSIF_MIRROR_PKT_CNTR_PRT00_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields sts_rsif_pkt_cntr_prt0	 */
#define STS_RSIF_PKT_CNTR_PRT00_WIDTH                                        16
#define STS_RSIF_PKT_CNTR_PRT00_SHIFT                                         0
#define STS_RSIF_PKT_CNTR_PRT00_MASK                                 0x0000ffff
#define STS_RSIF_PKT_CNTR_PRT00_RD(src)                  (((src) & 0x0000ffff))
#define STS_RSIF_PKT_CNTR_PRT00_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register RSIF_Sts_Cntr_Reg1	*/ 
/*	 Fields sts_rsif_mirror_pkt_cntr_prt1	 */
#define STS_RSIF_MIRROR_PKT_CNTR_PRT11_WIDTH                                 16
#define STS_RSIF_MIRROR_PKT_CNTR_PRT11_SHIFT                                 16
#define STS_RSIF_MIRROR_PKT_CNTR_PRT11_MASK                          0xffff0000
#define STS_RSIF_MIRROR_PKT_CNTR_PRT11_RD(src)       (((src) & 0xffff0000)>>16)
#define STS_RSIF_MIRROR_PKT_CNTR_PRT11_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields sts_rsif_pkt_cntr_prt1	 */
#define STS_RSIF_PKT_CNTR_PRT11_WIDTH                                        16
#define STS_RSIF_PKT_CNTR_PRT11_SHIFT                                         0
#define STS_RSIF_PKT_CNTR_PRT11_MASK                                 0x0000ffff
#define STS_RSIF_PKT_CNTR_PRT11_RD(src)                  (((src) & 0x0000ffff))
#define STS_RSIF_PKT_CNTR_PRT11_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register TSIF_Config_Reg_0	*/ 
/*	 Fields cfg_tsif_enet	 */
#define CFG_TSIF_ENET0_WIDTH                                                  1
#define CFG_TSIF_ENET0_SHIFT                                                 14
#define CFG_TSIF_ENET0_MASK                                          0x00004000
#define CFG_TSIF_ENET0_RD(src)                       (((src) & 0x00004000)>>14)
#define CFG_TSIF_ENET0_WR(src)                  (((u32)(src)<<14) & 0x00004000)
#define CFG_TSIF_ENET0_SET(dst,src) \
                      (((dst) & ~0x00004000) | (((u32)(src)<<14) & 0x00004000))
/*	 Fields cfg_tsif_cle	 */
#define CFG_TSIF_CLE0_WIDTH                                                   1
#define CFG_TSIF_CLE0_SHIFT                                                  13
#define CFG_TSIF_CLE0_MASK                                           0x00002000
#define CFG_TSIF_CLE0_RD(src)                        (((src) & 0x00002000)>>13)
#define CFG_TSIF_CLE0_WR(src)                   (((u32)(src)<<13) & 0x00002000)
#define CFG_TSIF_CLE0_SET(dst,src) \
                      (((dst) & ~0x00002000) | (((u32)(src)<<13) & 0x00002000))
/*	 Fields cfg_tsif_sos_eos_perbuffer	 */
#define CFG_TSIF_SOS_EOS_PERBUFFER0_WIDTH                                     1
#define CFG_TSIF_SOS_EOS_PERBUFFER0_SHIFT                                    12
#define CFG_TSIF_SOS_EOS_PERBUFFER0_MASK                             0x00001000
#define CFG_TSIF_SOS_EOS_PERBUFFER0_RD(src)          (((src) & 0x00001000)>>12)
#define CFG_TSIF_SOS_EOS_PERBUFFER0_WR(src)     (((u32)(src)<<12) & 0x00001000)
#define CFG_TSIF_SOS_EOS_PERBUFFER0_SET(dst,src) \
                      (((dst) & ~0x00001000) | (((u32)(src)<<12) & 0x00001000))
/*	 Fields cfg_tsif_dealloc_wmsg	 */
#define CFG_TSIF_DEALLOC_WMSG0_WIDTH                                          1
#define CFG_TSIF_DEALLOC_WMSG0_SHIFT                                         11
#define CFG_TSIF_DEALLOC_WMSG0_MASK                                  0x00000800
#define CFG_TSIF_DEALLOC_WMSG0_RD(src)               (((src) & 0x00000800)>>11)
#define CFG_TSIF_DEALLOC_WMSG0_WR(src)          (((u32)(src)<<11) & 0x00000800)
#define CFG_TSIF_DEALLOC_WMSG0_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*	 Fields cfg_tsif_cmsg_wmsg	 */
#define CFG_TSIF_CMSG_WMSG0_WIDTH                                             1
#define CFG_TSIF_CMSG_WMSG0_SHIFT                                            10
#define CFG_TSIF_CMSG_WMSG0_MASK                                     0x00000400
#define CFG_TSIF_CMSG_WMSG0_RD(src)                  (((src) & 0x00000400)>>10)
#define CFG_TSIF_CMSG_WMSG0_WR(src)             (((u32)(src)<<10) & 0x00000400)
#define CFG_TSIF_CMSG_WMSG0_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields cfg_tsif_big_endian	 */
#define CFG_TSIF_BIG_ENDIAN0_WIDTH                                            1
#define CFG_TSIF_BIG_ENDIAN0_SHIFT                                            9
#define CFG_TSIF_BIG_ENDIAN0_MASK                                    0x00000200
#define CFG_TSIF_BIG_ENDIAN0_RD(src)                  (((src) & 0x00000200)>>9)
#define CFG_TSIF_BIG_ENDIAN0_WR(src)             (((u32)(src)<<9) & 0x00000200)
#define CFG_TSIF_BIG_ENDIAN0_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields cfg_tsif_wmsg_swap	 */
#define CFG_TSIF_WMSG_SWAP0_WIDTH                                             1
#define CFG_TSIF_WMSG_SWAP0_SHIFT                                             8
#define CFG_TSIF_WMSG_SWAP0_MASK                                     0x00000100
#define CFG_TSIF_WMSG_SWAP0_RD(src)                   (((src) & 0x00000100)>>8)
#define CFG_TSIF_WMSG_SWAP0_WR(src)              (((u32)(src)<<8) & 0x00000100)
#define CFG_TSIF_WMSG_SWAP0_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*	 Fields cfg_tsif_rrm_threshold	 */
#define CFG_TSIF_RRM_THRESHOLD0_WIDTH                                         3
#define CFG_TSIF_RRM_THRESHOLD0_SHIFT                                         4
#define CFG_TSIF_RRM_THRESHOLD0_MASK                                 0x00000070
#define CFG_TSIF_RRM_THRESHOLD0_RD(src)               (((src) & 0x00000070)>>4)
#define CFG_TSIF_RRM_THRESHOLD0_WR(src)          (((u32)(src)<<4) & 0x00000070)
#define CFG_TSIF_RRM_THRESHOLD0_SET(dst,src) \
                       (((dst) & ~0x00000070) | (((u32)(src)<<4) & 0x00000070))
/*	 Fields cfg_tsif_rdm_threshold	 */
#define CFG_TSIF_RDM_THRESHOLD0_WIDTH                                         3
#define CFG_TSIF_RDM_THRESHOLD0_SHIFT                                         0
#define CFG_TSIF_RDM_THRESHOLD0_MASK                                 0x00000007
#define CFG_TSIF_RDM_THRESHOLD0_RD(src)                  (((src) & 0x00000007))
#define CFG_TSIF_RDM_THRESHOLD0_WR(src)             (((u32)(src)) & 0x00000007)
#define CFG_TSIF_RDM_THRESHOLD0_SET(dst,src) \
                          (((dst) & ~0x00000007) | (((u32)(src)) & 0x00000007))

/*	Register TSIF_Config_Reg_1	*/ 
/*	 Fields cfg_tsif_enet	 */
#define CFG_TSIF_ENET1_WIDTH                                                  1
#define CFG_TSIF_ENET1_SHIFT                                                 14
#define CFG_TSIF_ENET1_MASK                                          0x00004000
#define CFG_TSIF_ENET1_RD(src)                       (((src) & 0x00004000)>>14)
#define CFG_TSIF_ENET1_WR(src)                  (((u32)(src)<<14) & 0x00004000)
#define CFG_TSIF_ENET1_SET(dst,src) \
                      (((dst) & ~0x00004000) | (((u32)(src)<<14) & 0x00004000))
/*	 Fields cfg_tsif_cle	 */
#define CFG_TSIF_CLE1_WIDTH                                                   1
#define CFG_TSIF_CLE1_SHIFT                                                  13
#define CFG_TSIF_CLE1_MASK                                           0x00002000
#define CFG_TSIF_CLE1_RD(src)                        (((src) & 0x00002000)>>13)
#define CFG_TSIF_CLE1_WR(src)                   (((u32)(src)<<13) & 0x00002000)
#define CFG_TSIF_CLE1_SET(dst,src) \
                      (((dst) & ~0x00002000) | (((u32)(src)<<13) & 0x00002000))
/*	 Fields cfg_tsif_sos_eos_perbuffer	 */
#define CFG_TSIF_SOS_EOS_PERBUFFER1_WIDTH                                     1
#define CFG_TSIF_SOS_EOS_PERBUFFER1_SHIFT                                    12
#define CFG_TSIF_SOS_EOS_PERBUFFER1_MASK                             0x00001000
#define CFG_TSIF_SOS_EOS_PERBUFFER1_RD(src)          (((src) & 0x00001000)>>12)
#define CFG_TSIF_SOS_EOS_PERBUFFER1_WR(src)     (((u32)(src)<<12) & 0x00001000)
#define CFG_TSIF_SOS_EOS_PERBUFFER1_SET(dst,src) \
                      (((dst) & ~0x00001000) | (((u32)(src)<<12) & 0x00001000))
/*	 Fields cfg_tsif_dealloc_wmsg	 */
#define CFG_TSIF_DEALLOC_WMSG1_WIDTH                                          1
#define CFG_TSIF_DEALLOC_WMSG1_SHIFT                                         11
#define CFG_TSIF_DEALLOC_WMSG1_MASK                                  0x00000800
#define CFG_TSIF_DEALLOC_WMSG1_RD(src)               (((src) & 0x00000800)>>11)
#define CFG_TSIF_DEALLOC_WMSG1_WR(src)          (((u32)(src)<<11) & 0x00000800)
#define CFG_TSIF_DEALLOC_WMSG1_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*	 Fields cfg_tsif_cmsg_wmsg	 */
#define CFG_TSIF_CMSG_WMSG1_WIDTH                                             1
#define CFG_TSIF_CMSG_WMSG1_SHIFT                                            10
#define CFG_TSIF_CMSG_WMSG1_MASK                                     0x00000400
#define CFG_TSIF_CMSG_WMSG1_RD(src)                  (((src) & 0x00000400)>>10)
#define CFG_TSIF_CMSG_WMSG1_WR(src)             (((u32)(src)<<10) & 0x00000400)
#define CFG_TSIF_CMSG_WMSG1_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields cfg_tsif_big_endian	 */
#define CFG_TSIF_BIG_ENDIAN1_WIDTH                                            1
#define CFG_TSIF_BIG_ENDIAN1_SHIFT                                            9
#define CFG_TSIF_BIG_ENDIAN1_MASK                                    0x00000200
#define CFG_TSIF_BIG_ENDIAN1_RD(src)                  (((src) & 0x00000200)>>9)
#define CFG_TSIF_BIG_ENDIAN1_WR(src)             (((u32)(src)<<9) & 0x00000200)
#define CFG_TSIF_BIG_ENDIAN1_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields cfg_tsif_wmsg_swap	 */
#define CFG_TSIF_WMSG_SWAP1_WIDTH                                             1
#define CFG_TSIF_WMSG_SWAP1_SHIFT                                             8
#define CFG_TSIF_WMSG_SWAP1_MASK                                     0x00000100
#define CFG_TSIF_WMSG_SWAP1_RD(src)                   (((src) & 0x00000100)>>8)
#define CFG_TSIF_WMSG_SWAP1_WR(src)              (((u32)(src)<<8) & 0x00000100)
#define CFG_TSIF_WMSG_SWAP1_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*	 Fields cfg_tsif_rrm_threshold	 */
#define CFG_TSIF_RRM_THRESHOLD1_WIDTH                                         3
#define CFG_TSIF_RRM_THRESHOLD1_SHIFT                                         4
#define CFG_TSIF_RRM_THRESHOLD1_MASK                                 0x00000070
#define CFG_TSIF_RRM_THRESHOLD1_RD(src)               (((src) & 0x00000070)>>4)
#define CFG_TSIF_RRM_THRESHOLD1_WR(src)          (((u32)(src)<<4) & 0x00000070)
#define CFG_TSIF_RRM_THRESHOLD1_SET(dst,src) \
                       (((dst) & ~0x00000070) | (((u32)(src)<<4) & 0x00000070))
/*	 Fields cfg_tsif_rdm_threshold	 */
#define CFG_TSIF_RDM_THRESHOLD1_WIDTH                                         3
#define CFG_TSIF_RDM_THRESHOLD1_SHIFT                                         0
#define CFG_TSIF_RDM_THRESHOLD1_MASK                                 0x00000007
#define CFG_TSIF_RDM_THRESHOLD1_RD(src)                  (((src) & 0x00000007))
#define CFG_TSIF_RDM_THRESHOLD1_WR(src)             (((u32)(src)) & 0x00000007)
#define CFG_TSIF_RDM_THRESHOLD1_SET(dst,src) \
                          (((dst) & ~0x00000007) | (((u32)(src)) & 0x00000007))

/*	Register TSIF_MSS_Reg0_0	*/ 
/*	 Fields cfg_tsif_mss_sz1	 */
#define CFG_TSIF_MSS_SZ10_WIDTH                                              14
#define CFG_TSIF_MSS_SZ10_SHIFT                                              16
#define CFG_TSIF_MSS_SZ10_MASK                                       0x3fff0000
#define CFG_TSIF_MSS_SZ10_RD(src)                    (((src) & 0x3fff0000)>>16)
#define CFG_TSIF_MSS_SZ10_WR(src)               (((u32)(src)<<16) & 0x3fff0000)
#define CFG_TSIF_MSS_SZ10_SET(dst,src) \
                      (((dst) & ~0x3fff0000) | (((u32)(src)<<16) & 0x3fff0000))
/*	 Fields cfg_tsif_mss_sz0	 */
#define CFG_TSIF_MSS_SZ00_WIDTH                                              14
#define CFG_TSIF_MSS_SZ00_SHIFT                                               0
#define CFG_TSIF_MSS_SZ00_MASK                                       0x00003fff
#define CFG_TSIF_MSS_SZ00_RD(src)                        (((src) & 0x00003fff))
#define CFG_TSIF_MSS_SZ00_WR(src)                   (((u32)(src)) & 0x00003fff)
#define CFG_TSIF_MSS_SZ00_SET(dst,src) \
                          (((dst) & ~0x00003fff) | (((u32)(src)) & 0x00003fff))

/*	Register TSIF_MSS_Reg0_1	*/ 
/*	 Fields cfg_tsif_mss_sz1	 */
#define CFG_TSIF_MSS_SZ11_WIDTH                                              14
#define CFG_TSIF_MSS_SZ11_SHIFT                                              16
#define CFG_TSIF_MSS_SZ11_MASK                                       0x3fff0000
#define CFG_TSIF_MSS_SZ11_RD(src)                    (((src) & 0x3fff0000)>>16)
#define CFG_TSIF_MSS_SZ11_WR(src)               (((u32)(src)<<16) & 0x3fff0000)
#define CFG_TSIF_MSS_SZ11_SET(dst,src) \
                      (((dst) & ~0x3fff0000) | (((u32)(src)<<16) & 0x3fff0000))
/*	 Fields cfg_tsif_mss_sz0	 */
#define CFG_TSIF_MSS_SZ01_WIDTH                                              14
#define CFG_TSIF_MSS_SZ01_SHIFT                                               0
#define CFG_TSIF_MSS_SZ01_MASK                                       0x00003fff
#define CFG_TSIF_MSS_SZ01_RD(src)                        (((src) & 0x00003fff))
#define CFG_TSIF_MSS_SZ01_WR(src)                   (((u32)(src)) & 0x00003fff)
#define CFG_TSIF_MSS_SZ01_SET(dst,src) \
                          (((dst) & ~0x00003fff) | (((u32)(src)) & 0x00003fff))

/*	Register TSIF_MSS_Reg1_0	*/ 
/*	 Fields cfg_tsif_mss_sz3	 */
#define CFG_TSIF_MSS_SZ30_WIDTH                                              14
#define CFG_TSIF_MSS_SZ30_SHIFT                                              16
#define CFG_TSIF_MSS_SZ30_MASK                                       0x3fff0000
#define CFG_TSIF_MSS_SZ30_RD(src)                    (((src) & 0x3fff0000)>>16)
#define CFG_TSIF_MSS_SZ30_WR(src)               (((u32)(src)<<16) & 0x3fff0000)
#define CFG_TSIF_MSS_SZ30_SET(dst,src) \
                      (((dst) & ~0x3fff0000) | (((u32)(src)<<16) & 0x3fff0000))
/*	 Fields cfg_tsif_mss_sz2	 */
#define CFG_TSIF_MSS_SZ20_WIDTH                                              14
#define CFG_TSIF_MSS_SZ20_SHIFT                                               0
#define CFG_TSIF_MSS_SZ20_MASK                                       0x00003fff
#define CFG_TSIF_MSS_SZ20_RD(src)                        (((src) & 0x00003fff))
#define CFG_TSIF_MSS_SZ20_WR(src)                   (((u32)(src)) & 0x00003fff)
#define CFG_TSIF_MSS_SZ20_SET(dst,src) \
                          (((dst) & ~0x00003fff) | (((u32)(src)) & 0x00003fff))

/*	Register TSIF_MSS_Reg1_1	*/ 
/*	 Fields cfg_tsif_mss_sz3	 */
#define CFG_TSIF_MSS_SZ31_WIDTH                                              14
#define CFG_TSIF_MSS_SZ31_SHIFT                                              16
#define CFG_TSIF_MSS_SZ31_MASK                                       0x3fff0000
#define CFG_TSIF_MSS_SZ31_RD(src)                    (((src) & 0x3fff0000)>>16)
#define CFG_TSIF_MSS_SZ31_WR(src)               (((u32)(src)<<16) & 0x3fff0000)
#define CFG_TSIF_MSS_SZ31_SET(dst,src) \
                      (((dst) & ~0x3fff0000) | (((u32)(src)<<16) & 0x3fff0000))
/*	 Fields cfg_tsif_mss_sz2	 */
#define CFG_TSIF_MSS_SZ21_WIDTH                                              14
#define CFG_TSIF_MSS_SZ21_SHIFT                                               0
#define CFG_TSIF_MSS_SZ21_MASK                                       0x00003fff
#define CFG_TSIF_MSS_SZ21_RD(src)                        (((src) & 0x00003fff))
#define CFG_TSIF_MSS_SZ21_WR(src)                   (((u32)(src)) & 0x00003fff)
#define CFG_TSIF_MSS_SZ21_SET(dst,src) \
                          (((dst) & ~0x00003fff) | (((u32)(src)) & 0x00003fff))

/*	Register TSIF_FIFO_EmptySts0	*/ 
/*	 Fields tsif_rdmbuff_fifo_empty_prt1	 */
#define TSIF_RDMBUFF_FIFO_EMPTY_PRT10_WIDTH                                   1
#define TSIF_RDMBUFF_FIFO_EMPTY_PRT10_SHIFT                                  18
#define TSIF_RDMBUFF_FIFO_EMPTY_PRT10_MASK                           0x00040000
#define TSIF_RDMBUFF_FIFO_EMPTY_PRT10_RD(src)        (((src) & 0x00040000)>>18)
#define TSIF_RDMBUFF_FIFO_EMPTY_PRT10_SET(dst,src) \
                      (((dst) & ~0x00040000) | (((u32)(src)<<18) & 0x00040000))
/*	 Fields tsif_rrm_fifo_empty_prt1	 */
#define TSIF_RRM_FIFO_EMPTY_PRT10_WIDTH                                       1
#define TSIF_RRM_FIFO_EMPTY_PRT10_SHIFT                                      17
#define TSIF_RRM_FIFO_EMPTY_PRT10_MASK                               0x00020000
#define TSIF_RRM_FIFO_EMPTY_PRT10_RD(src)            (((src) & 0x00020000)>>17)
#define TSIF_RRM_FIFO_EMPTY_PRT10_SET(dst,src) \
                      (((dst) & ~0x00020000) | (((u32)(src)<<17) & 0x00020000))
/*	 Fields tsif_amabuf_fifo_empty_prt1	 */
#define TSIF_AMABUF_FIFO_EMPTY_PRT10_WIDTH                                    1
#define TSIF_AMABUF_FIFO_EMPTY_PRT10_SHIFT                                   16
#define TSIF_AMABUF_FIFO_EMPTY_PRT10_MASK                            0x00010000
#define TSIF_AMABUF_FIFO_EMPTY_PRT10_RD(src)         (((src) & 0x00010000)>>16)
#define TSIF_AMABUF_FIFO_EMPTY_PRT10_SET(dst,src) \
                      (((dst) & ~0x00010000) | (((u32)(src)<<16) & 0x00010000))
/*	 Fields tsif_rdmbuff_fifo_empty_prt0	 */
#define TSIF_RDMBUFF_FIFO_EMPTY_PRT00_WIDTH                                   1
#define TSIF_RDMBUFF_FIFO_EMPTY_PRT00_SHIFT                                   2
#define TSIF_RDMBUFF_FIFO_EMPTY_PRT00_MASK                           0x00000004
#define TSIF_RDMBUFF_FIFO_EMPTY_PRT00_RD(src)         (((src) & 0x00000004)>>2)
#define TSIF_RDMBUFF_FIFO_EMPTY_PRT00_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields tsif_rrm_fifo_empty_prt0	 */
#define TSIF_RRM_FIFO_EMPTY_PRT00_WIDTH                                       1
#define TSIF_RRM_FIFO_EMPTY_PRT00_SHIFT                                       1
#define TSIF_RRM_FIFO_EMPTY_PRT00_MASK                               0x00000002
#define TSIF_RRM_FIFO_EMPTY_PRT00_RD(src)             (((src) & 0x00000002)>>1)
#define TSIF_RRM_FIFO_EMPTY_PRT00_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields tsif_amabuf_fifo_empty_prt0	 */
#define TSIF_AMABUF_FIFO_EMPTY_PRT00_WIDTH                                    1
#define TSIF_AMABUF_FIFO_EMPTY_PRT00_SHIFT                                    0
#define TSIF_AMABUF_FIFO_EMPTY_PRT00_MASK                            0x00000001
#define TSIF_AMABUF_FIFO_EMPTY_PRT00_RD(src)             (((src) & 0x00000001))
#define TSIF_AMABUF_FIFO_EMPTY_PRT00_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register TSIF_Int_Reg0	*/ 
/*	 Fields tsif_rdmbuff_fifo_overfl_intr_prt1	 */
#define TSIF_RDMBUFF_FIFO_OVERFL_INTR_PRT10_WIDTH                             1
#define TSIF_RDMBUFF_FIFO_OVERFL_INTR_PRT10_SHIFT                            21
#define TSIF_RDMBUFF_FIFO_OVERFL_INTR_PRT10_MASK                     0x00200000
#define TSIF_RDMBUFF_FIFO_OVERFL_INTR_PRT10_RD(src)  (((src) & 0x00200000)>>21)
#define TSIF_RDMBUFF_FIFO_OVERFL_INTR_PRT10_WR(src) \
                                                (((u32)(src)<<21) & 0x00200000)
#define TSIF_RDMBUFF_FIFO_OVERFL_INTR_PRT10_SET(dst,src) \
                      (((dst) & ~0x00200000) | (((u32)(src)<<21) & 0x00200000))
/*	 Fields tsif_rdmbuff_fifo_underfl_intr_prt1	 */
#define TSIF_RDMBUFF_FIFO_UNDERFL_INTR_PRT10_WIDTH                            1
#define TSIF_RDMBUFF_FIFO_UNDERFL_INTR_PRT10_SHIFT                           20
#define TSIF_RDMBUFF_FIFO_UNDERFL_INTR_PRT10_MASK                    0x00100000
#define TSIF_RDMBUFF_FIFO_UNDERFL_INTR_PRT10_RD(src) \
                                                    (((src) & 0x00100000)>>20)
#define TSIF_RDMBUFF_FIFO_UNDERFL_INTR_PRT10_WR(src) \
                                                (((u32)(src)<<20) & 0x00100000)
#define TSIF_RDMBUFF_FIFO_UNDERFL_INTR_PRT10_SET(dst,src) \
                      (((dst) & ~0x00100000) | (((u32)(src)<<20) & 0x00100000))
/*	 Fields tsif_rrm_fifo_overfl_intr_prt1	 */
#define TSIF_RRM_FIFO_OVERFL_INTR_PRT10_WIDTH                                 1
#define TSIF_RRM_FIFO_OVERFL_INTR_PRT10_SHIFT                                19
#define TSIF_RRM_FIFO_OVERFL_INTR_PRT10_MASK                         0x00080000
#define TSIF_RRM_FIFO_OVERFL_INTR_PRT10_RD(src)      (((src) & 0x00080000)>>19)
#define TSIF_RRM_FIFO_OVERFL_INTR_PRT10_WR(src) \
                                                (((u32)(src)<<19) & 0x00080000)
#define TSIF_RRM_FIFO_OVERFL_INTR_PRT10_SET(dst,src) \
                      (((dst) & ~0x00080000) | (((u32)(src)<<19) & 0x00080000))
/*	 Fields tsif_rrm_fifo_underfl_intr_prt1	 */
#define TSIF_RRM_FIFO_UNDERFL_INTR_PRT10_WIDTH                                1
#define TSIF_RRM_FIFO_UNDERFL_INTR_PRT10_SHIFT                               18
#define TSIF_RRM_FIFO_UNDERFL_INTR_PRT10_MASK                        0x00040000
#define TSIF_RRM_FIFO_UNDERFL_INTR_PRT10_RD(src)     (((src) & 0x00040000)>>18)
#define TSIF_RRM_FIFO_UNDERFL_INTR_PRT10_WR(src) \
                                                (((u32)(src)<<18) & 0x00040000)
#define TSIF_RRM_FIFO_UNDERFL_INTR_PRT10_SET(dst,src) \
                      (((dst) & ~0x00040000) | (((u32)(src)<<18) & 0x00040000))
/*	 Fields tsif_amabuf_fifo_overfl_intr_prt1	 */
#define TSIF_AMABUF_FIFO_OVERFL_INTR_PRT10_WIDTH                              1
#define TSIF_AMABUF_FIFO_OVERFL_INTR_PRT10_SHIFT                             17
#define TSIF_AMABUF_FIFO_OVERFL_INTR_PRT10_MASK                      0x00020000
#define TSIF_AMABUF_FIFO_OVERFL_INTR_PRT10_RD(src)   (((src) & 0x00020000)>>17)
#define TSIF_AMABUF_FIFO_OVERFL_INTR_PRT10_WR(src) \
                                                (((u32)(src)<<17) & 0x00020000)
#define TSIF_AMABUF_FIFO_OVERFL_INTR_PRT10_SET(dst,src) \
                      (((dst) & ~0x00020000) | (((u32)(src)<<17) & 0x00020000))
/*	 Fields tsif_amabuf_fifo_underfl_intr_prt1	 */
#define TSIF_AMABUF_FIFO_UNDERFL_INTR_PRT10_WIDTH                             1
#define TSIF_AMABUF_FIFO_UNDERFL_INTR_PRT10_SHIFT                            16
#define TSIF_AMABUF_FIFO_UNDERFL_INTR_PRT10_MASK                     0x00010000
#define TSIF_AMABUF_FIFO_UNDERFL_INTR_PRT10_RD(src)  (((src) & 0x00010000)>>16)
#define TSIF_AMABUF_FIFO_UNDERFL_INTR_PRT10_WR(src) \
                                                (((u32)(src)<<16) & 0x00010000)
#define TSIF_AMABUF_FIFO_UNDERFL_INTR_PRT10_SET(dst,src) \
                      (((dst) & ~0x00010000) | (((u32)(src)<<16) & 0x00010000))
/*	 Fields tsif_rdmbuff_fifo_overfl_intr_prt0	 */
#define TSIF_RDMBUFF_FIFO_OVERFL_INTR_PRT00_WIDTH                             1
#define TSIF_RDMBUFF_FIFO_OVERFL_INTR_PRT00_SHIFT                             5
#define TSIF_RDMBUFF_FIFO_OVERFL_INTR_PRT00_MASK                     0x00000020
#define TSIF_RDMBUFF_FIFO_OVERFL_INTR_PRT00_RD(src)   (((src) & 0x00000020)>>5)
#define TSIF_RDMBUFF_FIFO_OVERFL_INTR_PRT00_WR(src) \
                                                 (((u32)(src)<<5) & 0x00000020)
#define TSIF_RDMBUFF_FIFO_OVERFL_INTR_PRT00_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields tsif_rdmbuff_fifo_underfl_intr_prt0	 */
#define TSIF_RDMBUFF_FIFO_UNDERFL_INTR_PRT00_WIDTH                            1
#define TSIF_RDMBUFF_FIFO_UNDERFL_INTR_PRT00_SHIFT                            4
#define TSIF_RDMBUFF_FIFO_UNDERFL_INTR_PRT00_MASK                    0x00000010
#define TSIF_RDMBUFF_FIFO_UNDERFL_INTR_PRT00_RD(src)  (((src) & 0x00000010)>>4)
#define TSIF_RDMBUFF_FIFO_UNDERFL_INTR_PRT00_WR(src) \
                                                 (((u32)(src)<<4) & 0x00000010)
#define TSIF_RDMBUFF_FIFO_UNDERFL_INTR_PRT00_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields tsif_rrm_fifo_overfl_intr_prt0	 */
#define TSIF_RRM_FIFO_OVERFL_INTR_PRT00_WIDTH                                 1
#define TSIF_RRM_FIFO_OVERFL_INTR_PRT00_SHIFT                                 3
#define TSIF_RRM_FIFO_OVERFL_INTR_PRT00_MASK                         0x00000008
#define TSIF_RRM_FIFO_OVERFL_INTR_PRT00_RD(src)       (((src) & 0x00000008)>>3)
#define TSIF_RRM_FIFO_OVERFL_INTR_PRT00_WR(src)  (((u32)(src)<<3) & 0x00000008)
#define TSIF_RRM_FIFO_OVERFL_INTR_PRT00_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields tsif_rrm_fifo_underfl_intr_prt0	 */
#define TSIF_RRM_FIFO_UNDERFL_INTR_PRT00_WIDTH                                1
#define TSIF_RRM_FIFO_UNDERFL_INTR_PRT00_SHIFT                                2
#define TSIF_RRM_FIFO_UNDERFL_INTR_PRT00_MASK                        0x00000004
#define TSIF_RRM_FIFO_UNDERFL_INTR_PRT00_RD(src)      (((src) & 0x00000004)>>2)
#define TSIF_RRM_FIFO_UNDERFL_INTR_PRT00_WR(src) \
                                                 (((u32)(src)<<2) & 0x00000004)
#define TSIF_RRM_FIFO_UNDERFL_INTR_PRT00_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields tsif_amabuf_fifo_overfl_intr_prt0	 */
#define TSIF_AMABUF_FIFO_OVERFL_INTR_PRT00_WIDTH                              1
#define TSIF_AMABUF_FIFO_OVERFL_INTR_PRT00_SHIFT                              1
#define TSIF_AMABUF_FIFO_OVERFL_INTR_PRT00_MASK                      0x00000002
#define TSIF_AMABUF_FIFO_OVERFL_INTR_PRT00_RD(src)    (((src) & 0x00000002)>>1)
#define TSIF_AMABUF_FIFO_OVERFL_INTR_PRT00_WR(src) \
                                                 (((u32)(src)<<1) & 0x00000002)
#define TSIF_AMABUF_FIFO_OVERFL_INTR_PRT00_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields tsif_amabuf_fifo_underfl_intr_prt0	 */
#define TSIF_AMABUF_FIFO_UNDERFL_INTR_PRT00_WIDTH                             1
#define TSIF_AMABUF_FIFO_UNDERFL_INTR_PRT00_SHIFT                             0
#define TSIF_AMABUF_FIFO_UNDERFL_INTR_PRT00_MASK                     0x00000001
#define TSIF_AMABUF_FIFO_UNDERFL_INTR_PRT00_RD(src)      (((src) & 0x00000001))
#define TSIF_AMABUF_FIFO_UNDERFL_INTR_PRT00_WR(src) \
                                                    (((u32)(src)) & 0x00000001)
#define TSIF_AMABUF_FIFO_UNDERFL_INTR_PRT00_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register TSIF_Int_Reg0Mask	*/
/*    Mask Register Fields tsif_rdmbuff_fifo_overfl_intr_prt1Mask    */
#define TSIF_RDMBUFF_FIFO_OVERFL_INTR_PRT1MASK_WIDTH                          1
#define TSIF_RDMBUFF_FIFO_OVERFL_INTR_PRT1MASK_SHIFT                         21
#define TSIF_RDMBUFF_FIFO_OVERFL_INTR_PRT1MASK_MASK                  0x00200000
#define TSIF_RDMBUFF_FIFO_OVERFL_INTR_PRT1MASK_RD(src) \
                                                    (((src) & 0x00200000)>>21)
#define TSIF_RDMBUFF_FIFO_OVERFL_INTR_PRT1MASK_WR(src) \
                                                (((u32)(src)<<21) & 0x00200000)
#define TSIF_RDMBUFF_FIFO_OVERFL_INTR_PRT1MASK_SET(dst,src) \
                      (((dst) & ~0x00200000) | (((u32)(src)<<21) & 0x00200000))
/*    Mask Register Fields tsif_rdmbuff_fifo_underfl_intr_prt1Mask    */
#define TSIF_RDMBUFF_FIFO_UNDERFL_INTR_PRT1MASK_WIDTH                         1
#define TSIF_RDMBUFF_FIFO_UNDERFL_INTR_PRT1MASK_SHIFT                        20
#define TSIF_RDMBUFF_FIFO_UNDERFL_INTR_PRT1MASK_MASK                 0x00100000
#define TSIF_RDMBUFF_FIFO_UNDERFL_INTR_PRT1MASK_RD(src) \
                                                    (((src) & 0x00100000)>>20)
#define TSIF_RDMBUFF_FIFO_UNDERFL_INTR_PRT1MASK_WR(src) \
                                                (((u32)(src)<<20) & 0x00100000)
#define TSIF_RDMBUFF_FIFO_UNDERFL_INTR_PRT1MASK_SET(dst,src) \
                      (((dst) & ~0x00100000) | (((u32)(src)<<20) & 0x00100000))
/*    Mask Register Fields tsif_rrm_fifo_overfl_intr_prt1Mask    */
#define TSIF_RRM_FIFO_OVERFL_INTR_PRT1MASK_WIDTH                              1
#define TSIF_RRM_FIFO_OVERFL_INTR_PRT1MASK_SHIFT                             19
#define TSIF_RRM_FIFO_OVERFL_INTR_PRT1MASK_MASK                      0x00080000
#define TSIF_RRM_FIFO_OVERFL_INTR_PRT1MASK_RD(src)   (((src) & 0x00080000)>>19)
#define TSIF_RRM_FIFO_OVERFL_INTR_PRT1MASK_WR(src) \
                                                (((u32)(src)<<19) & 0x00080000)
#define TSIF_RRM_FIFO_OVERFL_INTR_PRT1MASK_SET(dst,src) \
                      (((dst) & ~0x00080000) | (((u32)(src)<<19) & 0x00080000))
/*    Mask Register Fields tsif_rrm_fifo_underfl_intr_prt1Mask    */
#define TSIF_RRM_FIFO_UNDERFL_INTR_PRT1MASK_WIDTH                             1
#define TSIF_RRM_FIFO_UNDERFL_INTR_PRT1MASK_SHIFT                            18
#define TSIF_RRM_FIFO_UNDERFL_INTR_PRT1MASK_MASK                     0x00040000
#define TSIF_RRM_FIFO_UNDERFL_INTR_PRT1MASK_RD(src)  (((src) & 0x00040000)>>18)
#define TSIF_RRM_FIFO_UNDERFL_INTR_PRT1MASK_WR(src) \
                                                (((u32)(src)<<18) & 0x00040000)
#define TSIF_RRM_FIFO_UNDERFL_INTR_PRT1MASK_SET(dst,src) \
                      (((dst) & ~0x00040000) | (((u32)(src)<<18) & 0x00040000))
/*    Mask Register Fields tsif_amabuf_fifo_overfl_intr_prt1Mask    */
#define TSIF_AMABUF_FIFO_OVERFL_INTR_PRT1MASK_WIDTH                           1
#define TSIF_AMABUF_FIFO_OVERFL_INTR_PRT1MASK_SHIFT                          17
#define TSIF_AMABUF_FIFO_OVERFL_INTR_PRT1MASK_MASK                   0x00020000
#define TSIF_AMABUF_FIFO_OVERFL_INTR_PRT1MASK_RD(src) \
                                                    (((src) & 0x00020000)>>17)
#define TSIF_AMABUF_FIFO_OVERFL_INTR_PRT1MASK_WR(src) \
                                                (((u32)(src)<<17) & 0x00020000)
#define TSIF_AMABUF_FIFO_OVERFL_INTR_PRT1MASK_SET(dst,src) \
                      (((dst) & ~0x00020000) | (((u32)(src)<<17) & 0x00020000))
/*    Mask Register Fields tsif_amabuf_fifo_underfl_intr_prt1Mask    */
#define TSIF_AMABUF_FIFO_UNDERFL_INTR_PRT1MASK_WIDTH                          1
#define TSIF_AMABUF_FIFO_UNDERFL_INTR_PRT1MASK_SHIFT                         16
#define TSIF_AMABUF_FIFO_UNDERFL_INTR_PRT1MASK_MASK                  0x00010000
#define TSIF_AMABUF_FIFO_UNDERFL_INTR_PRT1MASK_RD(src) \
                                                    (((src) & 0x00010000)>>16)
#define TSIF_AMABUF_FIFO_UNDERFL_INTR_PRT1MASK_WR(src) \
                                                (((u32)(src)<<16) & 0x00010000)
#define TSIF_AMABUF_FIFO_UNDERFL_INTR_PRT1MASK_SET(dst,src) \
                      (((dst) & ~0x00010000) | (((u32)(src)<<16) & 0x00010000))
/*    Mask Register Fields tsif_rdmbuff_fifo_overfl_intr_prt0Mask    */
#define TSIF_RDMBUFF_FIFO_OVERFL_INTR_PRT0MASK_WIDTH                          1
#define TSIF_RDMBUFF_FIFO_OVERFL_INTR_PRT0MASK_SHIFT                          5
#define TSIF_RDMBUFF_FIFO_OVERFL_INTR_PRT0MASK_MASK                  0x00000020
#define TSIF_RDMBUFF_FIFO_OVERFL_INTR_PRT0MASK_RD(src) \
                                                     (((src) & 0x00000020)>>5)
#define TSIF_RDMBUFF_FIFO_OVERFL_INTR_PRT0MASK_WR(src) \
                                                 (((u32)(src)<<5) & 0x00000020)
#define TSIF_RDMBUFF_FIFO_OVERFL_INTR_PRT0MASK_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*    Mask Register Fields tsif_rdmbuff_fifo_underfl_intr_prt0Mask    */
#define TSIF_RDMBUFF_FIFO_UNDERFL_INTR_PRT0MASK_WIDTH                         1
#define TSIF_RDMBUFF_FIFO_UNDERFL_INTR_PRT0MASK_SHIFT                         4
#define TSIF_RDMBUFF_FIFO_UNDERFL_INTR_PRT0MASK_MASK                 0x00000010
#define TSIF_RDMBUFF_FIFO_UNDERFL_INTR_PRT0MASK_RD(src) \
                                                     (((src) & 0x00000010)>>4)
#define TSIF_RDMBUFF_FIFO_UNDERFL_INTR_PRT0MASK_WR(src) \
                                                 (((u32)(src)<<4) & 0x00000010)
#define TSIF_RDMBUFF_FIFO_UNDERFL_INTR_PRT0MASK_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*    Mask Register Fields tsif_rrm_fifo_overfl_intr_prt0Mask    */
#define TSIF_RRM_FIFO_OVERFL_INTR_PRT0MASK_WIDTH                              1
#define TSIF_RRM_FIFO_OVERFL_INTR_PRT0MASK_SHIFT                              3
#define TSIF_RRM_FIFO_OVERFL_INTR_PRT0MASK_MASK                      0x00000008
#define TSIF_RRM_FIFO_OVERFL_INTR_PRT0MASK_RD(src)    (((src) & 0x00000008)>>3)
#define TSIF_RRM_FIFO_OVERFL_INTR_PRT0MASK_WR(src) \
                                                 (((u32)(src)<<3) & 0x00000008)
#define TSIF_RRM_FIFO_OVERFL_INTR_PRT0MASK_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*    Mask Register Fields tsif_rrm_fifo_underfl_intr_prt0Mask    */
#define TSIF_RRM_FIFO_UNDERFL_INTR_PRT0MASK_WIDTH                             1
#define TSIF_RRM_FIFO_UNDERFL_INTR_PRT0MASK_SHIFT                             2
#define TSIF_RRM_FIFO_UNDERFL_INTR_PRT0MASK_MASK                     0x00000004
#define TSIF_RRM_FIFO_UNDERFL_INTR_PRT0MASK_RD(src)   (((src) & 0x00000004)>>2)
#define TSIF_RRM_FIFO_UNDERFL_INTR_PRT0MASK_WR(src) \
                                                 (((u32)(src)<<2) & 0x00000004)
#define TSIF_RRM_FIFO_UNDERFL_INTR_PRT0MASK_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*    Mask Register Fields tsif_amabuf_fifo_overfl_intr_prt0Mask    */
#define TSIF_AMABUF_FIFO_OVERFL_INTR_PRT0MASK_WIDTH                           1
#define TSIF_AMABUF_FIFO_OVERFL_INTR_PRT0MASK_SHIFT                           1
#define TSIF_AMABUF_FIFO_OVERFL_INTR_PRT0MASK_MASK                   0x00000002
#define TSIF_AMABUF_FIFO_OVERFL_INTR_PRT0MASK_RD(src) \
                                                     (((src) & 0x00000002)>>1)
#define TSIF_AMABUF_FIFO_OVERFL_INTR_PRT0MASK_WR(src) \
                                                 (((u32)(src)<<1) & 0x00000002)
#define TSIF_AMABUF_FIFO_OVERFL_INTR_PRT0MASK_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*    Mask Register Fields tsif_amabuf_fifo_underfl_intr_prt0Mask    */
#define TSIF_AMABUF_FIFO_UNDERFL_INTR_PRT0MASK_WIDTH                          1
#define TSIF_AMABUF_FIFO_UNDERFL_INTR_PRT0MASK_SHIFT                          0
#define TSIF_AMABUF_FIFO_UNDERFL_INTR_PRT0MASK_MASK                  0x00000001
#define TSIF_AMABUF_FIFO_UNDERFL_INTR_PRT0MASK_RD(src)   (((src) & 0x00000001))
#define TSIF_AMABUF_FIFO_UNDERFL_INTR_PRT0MASK_WR(src) \
                                                    (((u32)(src)) & 0x00000001)
#define TSIF_AMABUF_FIFO_UNDERFL_INTR_PRT0MASK_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register TSIF_FInt_Reg0	*/ 
/*	 Fields tsif_ss_axi_llrderr_intr_prt1	 */
#define TSIF_SS_AXI_LLRDERR_INTR_PRT10_WIDTH                                  1
#define TSIF_SS_AXI_LLRDERR_INTR_PRT10_SHIFT                                 18
#define TSIF_SS_AXI_LLRDERR_INTR_PRT10_MASK                          0x00040000
#define TSIF_SS_AXI_LLRDERR_INTR_PRT10_RD(src)       (((src) & 0x00040000)>>18)
#define TSIF_SS_AXI_LLRDERR_INTR_PRT10_WR(src)  (((u32)(src)<<18) & 0x00040000)
#define TSIF_SS_AXI_LLRDERR_INTR_PRT10_SET(dst,src) \
                      (((dst) & ~0x00040000) | (((u32)(src)<<18) & 0x00040000))
/*	 Fields tsif_ss_axi_rderr_intr_prt1	 */
#define TSIF_SS_AXI_RDERR_INTR_PRT10_WIDTH                                    1
#define TSIF_SS_AXI_RDERR_INTR_PRT10_SHIFT                                   17
#define TSIF_SS_AXI_RDERR_INTR_PRT10_MASK                            0x00020000
#define TSIF_SS_AXI_RDERR_INTR_PRT10_RD(src)         (((src) & 0x00020000)>>17)
#define TSIF_SS_AXI_RDERR_INTR_PRT10_WR(src)    (((u32)(src)<<17) & 0x00020000)
#define TSIF_SS_AXI_RDERR_INTR_PRT10_SET(dst,src) \
                      (((dst) & ~0x00020000) | (((u32)(src)<<17) & 0x00020000))
/*	 Fields tsif_ss_bad_msg_intr_prt1	 */
#define TSIF_SS_BAD_MSG_INTR_PRT10_WIDTH                                      1
#define TSIF_SS_BAD_MSG_INTR_PRT10_SHIFT                                     16
#define TSIF_SS_BAD_MSG_INTR_PRT10_MASK                              0x00010000
#define TSIF_SS_BAD_MSG_INTR_PRT10_RD(src)           (((src) & 0x00010000)>>16)
#define TSIF_SS_BAD_MSG_INTR_PRT10_WR(src)      (((u32)(src)<<16) & 0x00010000)
#define TSIF_SS_BAD_MSG_INTR_PRT10_SET(dst,src) \
                      (((dst) & ~0x00010000) | (((u32)(src)<<16) & 0x00010000))
/*	 Fields tsif_ss_axi_llrderr_intr_prt0	 */
#define TSIF_SS_AXI_LLRDERR_INTR_PRT00_WIDTH                                  1
#define TSIF_SS_AXI_LLRDERR_INTR_PRT00_SHIFT                                  2
#define TSIF_SS_AXI_LLRDERR_INTR_PRT00_MASK                          0x00000004
#define TSIF_SS_AXI_LLRDERR_INTR_PRT00_RD(src)        (((src) & 0x00000004)>>2)
#define TSIF_SS_AXI_LLRDERR_INTR_PRT00_WR(src)   (((u32)(src)<<2) & 0x00000004)
#define TSIF_SS_AXI_LLRDERR_INTR_PRT00_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields tsif_ss_axi_rderr_intr_prt0	 */
#define TSIF_SS_AXI_RDERR_INTR_PRT00_WIDTH                                    1
#define TSIF_SS_AXI_RDERR_INTR_PRT00_SHIFT                                    1
#define TSIF_SS_AXI_RDERR_INTR_PRT00_MASK                            0x00000002
#define TSIF_SS_AXI_RDERR_INTR_PRT00_RD(src)          (((src) & 0x00000002)>>1)
#define TSIF_SS_AXI_RDERR_INTR_PRT00_WR(src)     (((u32)(src)<<1) & 0x00000002)
#define TSIF_SS_AXI_RDERR_INTR_PRT00_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields tsif_ss_bad_msg_intr_prt0	 */
#define TSIF_SS_BAD_MSG_INTR_PRT00_WIDTH                                      1
#define TSIF_SS_BAD_MSG_INTR_PRT00_SHIFT                                      0
#define TSIF_SS_BAD_MSG_INTR_PRT00_MASK                              0x00000001
#define TSIF_SS_BAD_MSG_INTR_PRT00_RD(src)               (((src) & 0x00000001))
#define TSIF_SS_BAD_MSG_INTR_PRT00_WR(src)          (((u32)(src)) & 0x00000001)
#define TSIF_SS_BAD_MSG_INTR_PRT00_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register TSIF_FInt_Reg0Mask	*/
/*    Mask Register Fields tsif_ss_axi_llrderr_intr_prt1Mask    */
#define TSIF_SS_AXI_LLRDERR_INTR_PRT1MASK_WIDTH                               1
#define TSIF_SS_AXI_LLRDERR_INTR_PRT1MASK_SHIFT                              18
#define TSIF_SS_AXI_LLRDERR_INTR_PRT1MASK_MASK                       0x00040000
#define TSIF_SS_AXI_LLRDERR_INTR_PRT1MASK_RD(src)    (((src) & 0x00040000)>>18)
#define TSIF_SS_AXI_LLRDERR_INTR_PRT1MASK_WR(src) \
                                                (((u32)(src)<<18) & 0x00040000)
#define TSIF_SS_AXI_LLRDERR_INTR_PRT1MASK_SET(dst,src) \
                      (((dst) & ~0x00040000) | (((u32)(src)<<18) & 0x00040000))
/*    Mask Register Fields tsif_ss_axi_rderr_intr_prt1Mask    */
#define TSIF_SS_AXI_RDERR_INTR_PRT1MASK_WIDTH                                 1
#define TSIF_SS_AXI_RDERR_INTR_PRT1MASK_SHIFT                                17
#define TSIF_SS_AXI_RDERR_INTR_PRT1MASK_MASK                         0x00020000
#define TSIF_SS_AXI_RDERR_INTR_PRT1MASK_RD(src)      (((src) & 0x00020000)>>17)
#define TSIF_SS_AXI_RDERR_INTR_PRT1MASK_WR(src) \
                                                (((u32)(src)<<17) & 0x00020000)
#define TSIF_SS_AXI_RDERR_INTR_PRT1MASK_SET(dst,src) \
                      (((dst) & ~0x00020000) | (((u32)(src)<<17) & 0x00020000))
/*    Mask Register Fields tsif_ss_bad_msg_intr_prt1Mask    */
#define TSIF_SS_BAD_MSG_INTR_PRT1MASK_WIDTH                                   1
#define TSIF_SS_BAD_MSG_INTR_PRT1MASK_SHIFT                                  16
#define TSIF_SS_BAD_MSG_INTR_PRT1MASK_MASK                           0x00010000
#define TSIF_SS_BAD_MSG_INTR_PRT1MASK_RD(src)        (((src) & 0x00010000)>>16)
#define TSIF_SS_BAD_MSG_INTR_PRT1MASK_WR(src)   (((u32)(src)<<16) & 0x00010000)
#define TSIF_SS_BAD_MSG_INTR_PRT1MASK_SET(dst,src) \
                      (((dst) & ~0x00010000) | (((u32)(src)<<16) & 0x00010000))
/*    Mask Register Fields tsif_ss_axi_llrderr_intr_prt0Mask    */
#define TSIF_SS_AXI_LLRDERR_INTR_PRT0MASK_WIDTH                               1
#define TSIF_SS_AXI_LLRDERR_INTR_PRT0MASK_SHIFT                               2
#define TSIF_SS_AXI_LLRDERR_INTR_PRT0MASK_MASK                       0x00000004
#define TSIF_SS_AXI_LLRDERR_INTR_PRT0MASK_RD(src)     (((src) & 0x00000004)>>2)
#define TSIF_SS_AXI_LLRDERR_INTR_PRT0MASK_WR(src) \
                                                 (((u32)(src)<<2) & 0x00000004)
#define TSIF_SS_AXI_LLRDERR_INTR_PRT0MASK_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*    Mask Register Fields tsif_ss_axi_rderr_intr_prt0Mask    */
#define TSIF_SS_AXI_RDERR_INTR_PRT0MASK_WIDTH                                 1
#define TSIF_SS_AXI_RDERR_INTR_PRT0MASK_SHIFT                                 1
#define TSIF_SS_AXI_RDERR_INTR_PRT0MASK_MASK                         0x00000002
#define TSIF_SS_AXI_RDERR_INTR_PRT0MASK_RD(src)       (((src) & 0x00000002)>>1)
#define TSIF_SS_AXI_RDERR_INTR_PRT0MASK_WR(src)  (((u32)(src)<<1) & 0x00000002)
#define TSIF_SS_AXI_RDERR_INTR_PRT0MASK_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*    Mask Register Fields tsif_ss_bad_msg_intr_prt0Mask    */
#define TSIF_SS_BAD_MSG_INTR_PRT0MASK_WIDTH                                   1
#define TSIF_SS_BAD_MSG_INTR_PRT0MASK_SHIFT                                   0
#define TSIF_SS_BAD_MSG_INTR_PRT0MASK_MASK                           0x00000001
#define TSIF_SS_BAD_MSG_INTR_PRT0MASK_RD(src)            (((src) & 0x00000001))
#define TSIF_SS_BAD_MSG_INTR_PRT0MASK_WR(src)       (((u32)(src)) & 0x00000001)
#define TSIF_SS_BAD_MSG_INTR_PRT0MASK_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register TSIF_Sts_Reg0	*/ 
/*	 Fields tsif_ss_axi_llrderr_rresp_prt1	 */
#define TSIF_SS_AXI_LLRDERR_RRESP_PRT10_WIDTH                                 2
#define TSIF_SS_AXI_LLRDERR_RRESP_PRT10_SHIFT                                18
#define TSIF_SS_AXI_LLRDERR_RRESP_PRT10_MASK                         0x000c0000
#define TSIF_SS_AXI_LLRDERR_RRESP_PRT10_RD(src)      (((src) & 0x000c0000)>>18)
#define TSIF_SS_AXI_LLRDERR_RRESP_PRT10_SET(dst,src) \
                      (((dst) & ~0x000c0000) | (((u32)(src)<<18) & 0x000c0000))
/*	 Fields tsif_ss_axi_rderr_rresp_prt1	 */
#define TSIF_SS_AXI_RDERR_RRESP_PRT10_WIDTH                                   2
#define TSIF_SS_AXI_RDERR_RRESP_PRT10_SHIFT                                  16
#define TSIF_SS_AXI_RDERR_RRESP_PRT10_MASK                           0x00030000
#define TSIF_SS_AXI_RDERR_RRESP_PRT10_RD(src)        (((src) & 0x00030000)>>16)
#define TSIF_SS_AXI_RDERR_RRESP_PRT10_SET(dst,src) \
                      (((dst) & ~0x00030000) | (((u32)(src)<<16) & 0x00030000))
/*	 Fields tsif_ss_axi_llrderr_rresp_prt0	 */
#define TSIF_SS_AXI_LLRDERR_RRESP_PRT00_WIDTH                                 2
#define TSIF_SS_AXI_LLRDERR_RRESP_PRT00_SHIFT                                 2
#define TSIF_SS_AXI_LLRDERR_RRESP_PRT00_MASK                         0x0000000c
#define TSIF_SS_AXI_LLRDERR_RRESP_PRT00_RD(src)       (((src) & 0x0000000c)>>2)
#define TSIF_SS_AXI_LLRDERR_RRESP_PRT00_SET(dst,src) \
                       (((dst) & ~0x0000000c) | (((u32)(src)<<2) & 0x0000000c))
/*	 Fields tsif_ss_axi_rderr_rresp_prt0	 */
#define TSIF_SS_AXI_RDERR_RRESP_PRT00_WIDTH                                   2
#define TSIF_SS_AXI_RDERR_RRESP_PRT00_SHIFT                                   0
#define TSIF_SS_AXI_RDERR_RRESP_PRT00_MASK                           0x00000003
#define TSIF_SS_AXI_RDERR_RRESP_PRT00_RD(src)            (((src) & 0x00000003))
#define TSIF_SS_AXI_RDERR_RRESP_PRT00_SET(dst,src) \
                          (((dst) & ~0x00000003) | (((u32)(src)) & 0x00000003))

/*	Register TSIF_Sts_Reg1	*/ 
/*	 Fields sts_tsif_wmsg_cntr_prt1	 */
#define STS_TSIF_WMSG_CNTR_PRT11_WIDTH                                       16
#define STS_TSIF_WMSG_CNTR_PRT11_SHIFT                                       16
#define STS_TSIF_WMSG_CNTR_PRT11_MASK                                0xffff0000
#define STS_TSIF_WMSG_CNTR_PRT11_RD(src)             (((src) & 0xffff0000)>>16)
#define STS_TSIF_WMSG_CNTR_PRT11_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields sts_tsif_wmsg_cntr_prt0	 */
#define STS_TSIF_WMSG_CNTR_PRT01_WIDTH                                       16
#define STS_TSIF_WMSG_CNTR_PRT01_SHIFT                                        0
#define STS_TSIF_WMSG_CNTR_PRT01_MASK                                0x0000ffff
#define STS_TSIF_WMSG_CNTR_PRT01_RD(src)                 (((src) & 0x0000ffff))
#define STS_TSIF_WMSG_CNTR_PRT01_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register TSIF_Sts_Reg2	*/ 
/*	 Fields sts_tsif_da_wmsg_cntr_prt1	 */
#define STS_TSIF_DA_WMSG_CNTR_PRT12_WIDTH                                    16
#define STS_TSIF_DA_WMSG_CNTR_PRT12_SHIFT                                    16
#define STS_TSIF_DA_WMSG_CNTR_PRT12_MASK                             0xffff0000
#define STS_TSIF_DA_WMSG_CNTR_PRT12_RD(src)          (((src) & 0xffff0000)>>16)
#define STS_TSIF_DA_WMSG_CNTR_PRT12_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields sts_tsif_da_wmsg_cntr_prt0	 */
#define STS_TSIF_DA_WMSG_CNTR_PRT02_WIDTH                                    16
#define STS_TSIF_DA_WMSG_CNTR_PRT02_SHIFT                                     0
#define STS_TSIF_DA_WMSG_CNTR_PRT02_MASK                             0x0000ffff
#define STS_TSIF_DA_WMSG_CNTR_PRT02_RD(src)              (((src) & 0x0000ffff))
#define STS_TSIF_DA_WMSG_CNTR_PRT02_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register TSIF_Sts_Reg3	*/ 
/*	 Fields sts_tsif_bad_wmsg_cntr_prt1	 */
#define STS_TSIF_BAD_WMSG_CNTR_PRT13_WIDTH                                   16
#define STS_TSIF_BAD_WMSG_CNTR_PRT13_SHIFT                                   16
#define STS_TSIF_BAD_WMSG_CNTR_PRT13_MASK                            0xffff0000
#define STS_TSIF_BAD_WMSG_CNTR_PRT13_RD(src)         (((src) & 0xffff0000)>>16)
#define STS_TSIF_BAD_WMSG_CNTR_PRT13_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields sts_tsif_bad_wmsg_cntr_prt0	 */
#define STS_TSIF_BAD_WMSG_CNTR_PRT03_WIDTH                                   16
#define STS_TSIF_BAD_WMSG_CNTR_PRT03_SHIFT                                    0
#define STS_TSIF_BAD_WMSG_CNTR_PRT03_MASK                            0x0000ffff
#define STS_TSIF_BAD_WMSG_CNTR_PRT03_RD(src)             (((src) & 0x0000ffff))
#define STS_TSIF_BAD_WMSG_CNTR_PRT03_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register AVB_classA_credit_refill_0	*/ 
/*	 Fields cfg_avb_classA_credit_refill	 */
#define CFG_AVB_CLASSA_CREDIT_REFILL0_WIDTH                                  27
#define CFG_AVB_CLASSA_CREDIT_REFILL0_SHIFT                                   0
#define CFG_AVB_CLASSA_CREDIT_REFILL0_MASK                           0x07ffffff
#define CFG_AVB_CLASSA_CREDIT_REFILL0_RD(src)            (((src) & 0x07ffffff))
#define CFG_AVB_CLASSA_CREDIT_REFILL0_WR(src)       (((u32)(src)) & 0x07ffffff)
#define CFG_AVB_CLASSA_CREDIT_REFILL0_SET(dst,src) \
                          (((dst) & ~0x07ffffff) | (((u32)(src)) & 0x07ffffff))

/*	Register AVB_classA_credit_refill_1	*/ 
/*	 Fields cfg_avb_classA_credit_refill	 */
#define CFG_AVB_CLASSA_CREDIT_REFILL1_WIDTH                                  27
#define CFG_AVB_CLASSA_CREDIT_REFILL1_SHIFT                                   0
#define CFG_AVB_CLASSA_CREDIT_REFILL1_MASK                           0x07ffffff
#define CFG_AVB_CLASSA_CREDIT_REFILL1_RD(src)            (((src) & 0x07ffffff))
#define CFG_AVB_CLASSA_CREDIT_REFILL1_WR(src)       (((u32)(src)) & 0x07ffffff)
#define CFG_AVB_CLASSA_CREDIT_REFILL1_SET(dst,src) \
                          (((dst) & ~0x07ffffff) | (((u32)(src)) & 0x07ffffff))

/*	Register AVB_classB_credit_refill_0	*/ 
/*	 Fields cfg_avb_classB_credit_refill	 */
#define CFG_AVB_CLASSB_CREDIT_REFILL0_WIDTH                                  27
#define CFG_AVB_CLASSB_CREDIT_REFILL0_SHIFT                                   0
#define CFG_AVB_CLASSB_CREDIT_REFILL0_MASK                           0x07ffffff
#define CFG_AVB_CLASSB_CREDIT_REFILL0_RD(src)            (((src) & 0x07ffffff))
#define CFG_AVB_CLASSB_CREDIT_REFILL0_WR(src)       (((u32)(src)) & 0x07ffffff)
#define CFG_AVB_CLASSB_CREDIT_REFILL0_SET(dst,src) \
                          (((dst) & ~0x07ffffff) | (((u32)(src)) & 0x07ffffff))

/*	Register AVB_classB_credit_refill_1	*/ 
/*	 Fields cfg_avb_classB_credit_refill	 */
#define CFG_AVB_CLASSB_CREDIT_REFILL1_WIDTH                                  27
#define CFG_AVB_CLASSB_CREDIT_REFILL1_SHIFT                                   0
#define CFG_AVB_CLASSB_CREDIT_REFILL1_MASK                           0x07ffffff
#define CFG_AVB_CLASSB_CREDIT_REFILL1_RD(src)            (((src) & 0x07ffffff))
#define CFG_AVB_CLASSB_CREDIT_REFILL1_WR(src)       (((u32)(src)) & 0x07ffffff)
#define CFG_AVB_CLASSB_CREDIT_REFILL1_SET(dst,src) \
                          (((dst) & ~0x07ffffff) | (((u32)(src)) & 0x07ffffff))

/*	Register AVB_common_config1_0	*/ 
/*	 Fields mac_speed	 */
#define MAC_SPEED0_WIDTH                                                      2
#define MAC_SPEED0_SHIFT                                                     27
#define MAC_SPEED0_MASK                                              0x18000000
#define MAC_SPEED0_RD(src)                           (((src) & 0x18000000)>>27)
#define MAC_SPEED0_WR(src)                      (((u32)(src)<<27) & 0x18000000)
#define MAC_SPEED0_SET(dst,src) \
                      (((dst) & ~0x18000000) | (((u32)(src)<<27) & 0x18000000))
/*	 Fields cfg_avb_add_overhead	 */
#define CFG_AVB_ADD_OVERHEAD0_WIDTH                                           1
#define CFG_AVB_ADD_OVERHEAD0_SHIFT                                          26
#define CFG_AVB_ADD_OVERHEAD0_MASK                                   0x04000000
#define CFG_AVB_ADD_OVERHEAD0_RD(src)                (((src) & 0x04000000)>>26)
#define CFG_AVB_ADD_OVERHEAD0_WR(src)           (((u32)(src)<<26) & 0x04000000)
#define CFG_AVB_ADD_OVERHEAD0_SET(dst,src) \
                      (((dst) & ~0x04000000) | (((u32)(src)<<26) & 0x04000000))
/*	 Fields cfg_avb_overhead	 */
#define CFG_AVB_OVERHEAD0_WIDTH                                               7
#define CFG_AVB_OVERHEAD0_SHIFT                                              19
#define CFG_AVB_OVERHEAD0_MASK                                       0x03f80000
#define CFG_AVB_OVERHEAD0_RD(src)                    (((src) & 0x03f80000)>>19)
#define CFG_AVB_OVERHEAD0_WR(src)               (((u32)(src)<<19) & 0x03f80000)
#define CFG_AVB_OVERHEAD0_SET(dst,src) \
                      (((dst) & ~0x03f80000) | (((u32)(src)<<19) & 0x03f80000))
/*	 Fields cfg_avb_credit_inc	 */
#define CFG_AVB_CREDIT_INC0_WIDTH                                             1
#define CFG_AVB_CREDIT_INC0_SHIFT                                            18
#define CFG_AVB_CREDIT_INC0_MASK                                     0x00040000
#define CFG_AVB_CREDIT_INC0_RD(src)                  (((src) & 0x00040000)>>18)
#define CFG_AVB_CREDIT_INC0_WR(src)             (((u32)(src)<<18) & 0x00040000)
#define CFG_AVB_CREDIT_INC0_SET(dst,src) \
                      (((dst) & ~0x00040000) | (((u32)(src)<<18) & 0x00040000))
/*	 Fields cfg_avb_credit_refill_en	 */
#define CFG_AVB_CREDIT_REFILL_EN0_WIDTH                                       1
#define CFG_AVB_CREDIT_REFILL_EN0_SHIFT                                      17
#define CFG_AVB_CREDIT_REFILL_EN0_MASK                               0x00020000
#define CFG_AVB_CREDIT_REFILL_EN0_RD(src)            (((src) & 0x00020000)>>17)
#define CFG_AVB_CREDIT_REFILL_EN0_WR(src)       (((u32)(src)<<17) & 0x00020000)
#define CFG_AVB_CREDIT_REFILL_EN0_SET(dst,src) \
                      (((dst) & ~0x00020000) | (((u32)(src)<<17) & 0x00020000))
/*	 Fields cfg_avb_ptp_sync_drain_en	 */
#define CFG_AVB_PTP_SYNC_DRAIN_EN0_WIDTH                                      1
#define CFG_AVB_PTP_SYNC_DRAIN_EN0_SHIFT                                     16
#define CFG_AVB_PTP_SYNC_DRAIN_EN0_MASK                              0x00010000
#define CFG_AVB_PTP_SYNC_DRAIN_EN0_RD(src)           (((src) & 0x00010000)>>16)
#define CFG_AVB_PTP_SYNC_DRAIN_EN0_WR(src)      (((u32)(src)<<16) & 0x00010000)
#define CFG_AVB_PTP_SYNC_DRAIN_EN0_SET(dst,src) \
                      (((dst) & ~0x00010000) | (((u32)(src)<<16) & 0x00010000))
/*	 Fields cfg_avb_ptp_sync_en_classA_cnt	 */
#define CFG_AVB_PTP_SYNC_EN_CLASSA_CNT0_WIDTH                                 8
#define CFG_AVB_PTP_SYNC_EN_CLASSA_CNT0_SHIFT                                 8
#define CFG_AVB_PTP_SYNC_EN_CLASSA_CNT0_MASK                         0x0000ff00
#define CFG_AVB_PTP_SYNC_EN_CLASSA_CNT0_RD(src)       (((src) & 0x0000ff00)>>8)
#define CFG_AVB_PTP_SYNC_EN_CLASSA_CNT0_WR(src)  (((u32)(src)<<8) & 0x0000ff00)
#define CFG_AVB_PTP_SYNC_EN_CLASSA_CNT0_SET(dst,src) \
                       (((dst) & ~0x0000ff00) | (((u32)(src)<<8) & 0x0000ff00))
/*	 Fields cfg_avb_ptp_sync_en_classB_cnt	 */
#define CFG_AVB_PTP_SYNC_EN_CLASSB_CNT0_WIDTH                                 8
#define CFG_AVB_PTP_SYNC_EN_CLASSB_CNT0_SHIFT                                 0
#define CFG_AVB_PTP_SYNC_EN_CLASSB_CNT0_MASK                         0x000000ff
#define CFG_AVB_PTP_SYNC_EN_CLASSB_CNT0_RD(src)          (((src) & 0x000000ff))
#define CFG_AVB_PTP_SYNC_EN_CLASSB_CNT0_WR(src)     (((u32)(src)) & 0x000000ff)
#define CFG_AVB_PTP_SYNC_EN_CLASSB_CNT0_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register AVB_common_config1_1	*/ 
/*	 Fields mac_speed	 */
#define MAC_SPEED1_WIDTH                                                      2
#define MAC_SPEED1_SHIFT                                                     27
#define MAC_SPEED1_MASK                                              0x18000000
#define MAC_SPEED1_RD(src)                           (((src) & 0x18000000)>>27)
#define MAC_SPEED1_WR(src)                      (((u32)(src)<<27) & 0x18000000)
#define MAC_SPEED1_SET(dst,src) \
                      (((dst) & ~0x18000000) | (((u32)(src)<<27) & 0x18000000))
/*	 Fields cfg_avb_add_overhead	 */
#define CFG_AVB_ADD_OVERHEAD1_WIDTH                                           1
#define CFG_AVB_ADD_OVERHEAD1_SHIFT                                          26
#define CFG_AVB_ADD_OVERHEAD1_MASK                                   0x04000000
#define CFG_AVB_ADD_OVERHEAD1_RD(src)                (((src) & 0x04000000)>>26)
#define CFG_AVB_ADD_OVERHEAD1_WR(src)           (((u32)(src)<<26) & 0x04000000)
#define CFG_AVB_ADD_OVERHEAD1_SET(dst,src) \
                      (((dst) & ~0x04000000) | (((u32)(src)<<26) & 0x04000000))
/*	 Fields cfg_avb_overhead	 */
#define CFG_AVB_OVERHEAD1_WIDTH                                               7
#define CFG_AVB_OVERHEAD1_SHIFT                                              19
#define CFG_AVB_OVERHEAD1_MASK                                       0x03f80000
#define CFG_AVB_OVERHEAD1_RD(src)                    (((src) & 0x03f80000)>>19)
#define CFG_AVB_OVERHEAD1_WR(src)               (((u32)(src)<<19) & 0x03f80000)
#define CFG_AVB_OVERHEAD1_SET(dst,src) \
                      (((dst) & ~0x03f80000) | (((u32)(src)<<19) & 0x03f80000))
/*	 Fields cfg_avb_credit_inc	 */
#define CFG_AVB_CREDIT_INC1_WIDTH                                             1
#define CFG_AVB_CREDIT_INC1_SHIFT                                            18
#define CFG_AVB_CREDIT_INC1_MASK                                     0x00040000
#define CFG_AVB_CREDIT_INC1_RD(src)                  (((src) & 0x00040000)>>18)
#define CFG_AVB_CREDIT_INC1_WR(src)             (((u32)(src)<<18) & 0x00040000)
#define CFG_AVB_CREDIT_INC1_SET(dst,src) \
                      (((dst) & ~0x00040000) | (((u32)(src)<<18) & 0x00040000))
/*	 Fields cfg_avb_credit_refill_en	 */
#define CFG_AVB_CREDIT_REFILL_EN1_WIDTH                                       1
#define CFG_AVB_CREDIT_REFILL_EN1_SHIFT                                      17
#define CFG_AVB_CREDIT_REFILL_EN1_MASK                               0x00020000
#define CFG_AVB_CREDIT_REFILL_EN1_RD(src)            (((src) & 0x00020000)>>17)
#define CFG_AVB_CREDIT_REFILL_EN1_WR(src)       (((u32)(src)<<17) & 0x00020000)
#define CFG_AVB_CREDIT_REFILL_EN1_SET(dst,src) \
                      (((dst) & ~0x00020000) | (((u32)(src)<<17) & 0x00020000))
/*	 Fields cfg_avb_ptp_sync_drain_en	 */
#define CFG_AVB_PTP_SYNC_DRAIN_EN1_WIDTH                                      1
#define CFG_AVB_PTP_SYNC_DRAIN_EN1_SHIFT                                     16
#define CFG_AVB_PTP_SYNC_DRAIN_EN1_MASK                              0x00010000
#define CFG_AVB_PTP_SYNC_DRAIN_EN1_RD(src)           (((src) & 0x00010000)>>16)
#define CFG_AVB_PTP_SYNC_DRAIN_EN1_WR(src)      (((u32)(src)<<16) & 0x00010000)
#define CFG_AVB_PTP_SYNC_DRAIN_EN1_SET(dst,src) \
                      (((dst) & ~0x00010000) | (((u32)(src)<<16) & 0x00010000))
/*	 Fields cfg_avb_ptp_sync_en_classA_cnt	 */
#define CFG_AVB_PTP_SYNC_EN_CLASSA_CNT1_WIDTH                                 8
#define CFG_AVB_PTP_SYNC_EN_CLASSA_CNT1_SHIFT                                 8
#define CFG_AVB_PTP_SYNC_EN_CLASSA_CNT1_MASK                         0x0000ff00
#define CFG_AVB_PTP_SYNC_EN_CLASSA_CNT1_RD(src)       (((src) & 0x0000ff00)>>8)
#define CFG_AVB_PTP_SYNC_EN_CLASSA_CNT1_WR(src)  (((u32)(src)<<8) & 0x0000ff00)
#define CFG_AVB_PTP_SYNC_EN_CLASSA_CNT1_SET(dst,src) \
                       (((dst) & ~0x0000ff00) | (((u32)(src)<<8) & 0x0000ff00))
/*	 Fields cfg_avb_ptp_sync_en_classB_cnt	 */
#define CFG_AVB_PTP_SYNC_EN_CLASSB_CNT1_WIDTH                                 8
#define CFG_AVB_PTP_SYNC_EN_CLASSB_CNT1_SHIFT                                 0
#define CFG_AVB_PTP_SYNC_EN_CLASSB_CNT1_MASK                         0x000000ff
#define CFG_AVB_PTP_SYNC_EN_CLASSB_CNT1_RD(src)          (((src) & 0x000000ff))
#define CFG_AVB_PTP_SYNC_EN_CLASSB_CNT1_WR(src)     (((u32)(src)) & 0x000000ff)
#define CFG_AVB_PTP_SYNC_EN_CLASSB_CNT1_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register AVB_common_config2_0	*/ 
/*	 Fields cfg_avb_calc_int_HundredNsec_cnt	 */
#define CFG_AVB_CALC_INT_HUNDREDNSEC_CNT0_WIDTH                              17
#define CFG_AVB_CALC_INT_HUNDREDNSEC_CNT0_SHIFT                               0
#define CFG_AVB_CALC_INT_HUNDREDNSEC_CNT0_MASK                       0x0001ffff
#define CFG_AVB_CALC_INT_HUNDREDNSEC_CNT0_RD(src)        (((src) & 0x0001ffff))
#define CFG_AVB_CALC_INT_HUNDREDNSEC_CNT0_WR(src)   (((u32)(src)) & 0x0001ffff)
#define CFG_AVB_CALC_INT_HUNDREDNSEC_CNT0_SET(dst,src) \
                          (((dst) & ~0x0001ffff) | (((u32)(src)) & 0x0001ffff))

/*	Register AVB_common_config2_1	*/ 
/*	 Fields cfg_avb_calc_int_HundredNsec_cnt	 */
#define CFG_AVB_CALC_INT_HUNDREDNSEC_CNT1_WIDTH                              17
#define CFG_AVB_CALC_INT_HUNDREDNSEC_CNT1_SHIFT                               0
#define CFG_AVB_CALC_INT_HUNDREDNSEC_CNT1_MASK                       0x0001ffff
#define CFG_AVB_CALC_INT_HUNDREDNSEC_CNT1_RD(src)        (((src) & 0x0001ffff))
#define CFG_AVB_CALC_INT_HUNDREDNSEC_CNT1_WR(src)   (((u32)(src)) & 0x0001ffff)
#define CFG_AVB_CALC_INT_HUNDREDNSEC_CNT1_SET(dst,src) \
                          (((dst) & ~0x0001ffff) | (((u32)(src)) & 0x0001ffff))

/*	Register AVB_per_q_config1_0	*/ 
/*	 Fields cfg_avb_classA	 */
#define CFG_AVB_CLASSA0_WIDTH                                                 8
#define CFG_AVB_CLASSA0_SHIFT                                                24
#define CFG_AVB_CLASSA0_MASK                                         0xff000000
#define CFG_AVB_CLASSA0_RD(src)                      (((src) & 0xff000000)>>24)
#define CFG_AVB_CLASSA0_WR(src)                 (((u32)(src)<<24) & 0xff000000)
#define CFG_AVB_CLASSA0_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))
/*	 Fields cfg_avb_classB	 */
#define CFG_AVB_CLASSB0_WIDTH                                                 8
#define CFG_AVB_CLASSB0_SHIFT                                                16
#define CFG_AVB_CLASSB0_MASK                                         0x00ff0000
#define CFG_AVB_CLASSB0_RD(src)                      (((src) & 0x00ff0000)>>16)
#define CFG_AVB_CLASSB0_WR(src)                 (((u32)(src)<<16) & 0x00ff0000)
#define CFG_AVB_CLASSB0_SET(dst,src) \
                      (((dst) & ~0x00ff0000) | (((u32)(src)<<16) & 0x00ff0000))
/*	 Fields cfg_eth_q_arb_type	 */
#define CFG_ETH_Q_ARB_TYPE0_WIDTH                                            16
#define CFG_ETH_Q_ARB_TYPE0_SHIFT                                             0
#define CFG_ETH_Q_ARB_TYPE0_MASK                                     0x0000ffff
#define CFG_ETH_Q_ARB_TYPE0_RD(src)                      (((src) & 0x0000ffff))
#define CFG_ETH_Q_ARB_TYPE0_WR(src)                 (((u32)(src)) & 0x0000ffff)
#define CFG_ETH_Q_ARB_TYPE0_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register AVB_per_q_config1_1	*/ 
/*	 Fields cfg_avb_classA	 */
#define CFG_AVB_CLASSA1_WIDTH                                                 8
#define CFG_AVB_CLASSA1_SHIFT                                                24
#define CFG_AVB_CLASSA1_MASK                                         0xff000000
#define CFG_AVB_CLASSA1_RD(src)                      (((src) & 0xff000000)>>24)
#define CFG_AVB_CLASSA1_WR(src)                 (((u32)(src)<<24) & 0xff000000)
#define CFG_AVB_CLASSA1_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))
/*	 Fields cfg_avb_classB	 */
#define CFG_AVB_CLASSB1_WIDTH                                                 8
#define CFG_AVB_CLASSB1_SHIFT                                                16
#define CFG_AVB_CLASSB1_MASK                                         0x00ff0000
#define CFG_AVB_CLASSB1_RD(src)                      (((src) & 0x00ff0000)>>16)
#define CFG_AVB_CLASSB1_WR(src)                 (((u32)(src)<<16) & 0x00ff0000)
#define CFG_AVB_CLASSB1_SET(dst,src) \
                      (((dst) & ~0x00ff0000) | (((u32)(src)<<16) & 0x00ff0000))
/*	 Fields cfg_eth_q_arb_type	 */
#define CFG_ETH_Q_ARB_TYPE1_WIDTH                                            16
#define CFG_ETH_Q_ARB_TYPE1_SHIFT                                             0
#define CFG_ETH_Q_ARB_TYPE1_MASK                                     0x0000ffff
#define CFG_ETH_Q_ARB_TYPE1_RD(src)                      (((src) & 0x0000ffff))
#define CFG_ETH_Q_ARB_TYPE1_WR(src)                 (((u32)(src)) & 0x0000ffff)
#define CFG_ETH_Q_ARB_TYPE1_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register AVB_per_q_hi_credit_0	*/ 
/*	 Fields cfg_hi_credit	 */
#define CFG_HI_CREDIT0_WIDTH                                                 32
#define CFG_HI_CREDIT0_SHIFT                                                  0
#define CFG_HI_CREDIT0_MASK                                          0xffffffff
#define CFG_HI_CREDIT0_RD(src)                           (((src) & 0xffffffff))
#define CFG_HI_CREDIT0_WR(src)                      (((u32)(src)) & 0xffffffff)
#define CFG_HI_CREDIT0_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register AVB_per_q_hi_credit_1	*/ 
/*	 Fields cfg_hi_credit	 */
#define CFG_HI_CREDIT1_WIDTH                                                 32
#define CFG_HI_CREDIT1_SHIFT                                                  0
#define CFG_HI_CREDIT1_MASK                                          0xffffffff
#define CFG_HI_CREDIT1_RD(src)                           (((src) & 0xffffffff))
#define CFG_HI_CREDIT1_WR(src)                      (((u32)(src)) & 0xffffffff)
#define CFG_HI_CREDIT1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register AVB_per_q_idle_slope0_0	*/ 
/*	 Fields cfg_idle_slope_wrr_weights_q2	 */
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q20_WIDTH                                  9
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q20_SHIFT                                 18
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q20_MASK                          0x07fc0000
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q20_RD(src)       (((src) & 0x07fc0000)>>18)
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q20_WR(src)  (((u32)(src)<<18) & 0x07fc0000)
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q20_SET(dst,src) \
                      (((dst) & ~0x07fc0000) | (((u32)(src)<<18) & 0x07fc0000))
/*	 Fields cfg_idle_slope_wrr_weights_q1	 */
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q10_WIDTH                                  9
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q10_SHIFT                                  9
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q10_MASK                          0x0003fe00
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q10_RD(src)        (((src) & 0x0003fe00)>>9)
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q10_WR(src)   (((u32)(src)<<9) & 0x0003fe00)
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q10_SET(dst,src) \
                       (((dst) & ~0x0003fe00) | (((u32)(src)<<9) & 0x0003fe00))
/*	 Fields cfg_idle_slope_wrr_weights_q0	 */
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q00_WIDTH                                  9
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q00_SHIFT                                  0
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q00_MASK                          0x000001ff
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q00_RD(src)           (((src) & 0x000001ff))
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q00_WR(src)      (((u32)(src)) & 0x000001ff)
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q00_SET(dst,src) \
                          (((dst) & ~0x000001ff) | (((u32)(src)) & 0x000001ff))

/*	Register AVB_per_q_idle_slope0_1	*/ 
/*	 Fields cfg_idle_slope_wrr_weights_q2	 */
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q21_WIDTH                                  9
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q21_SHIFT                                 18
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q21_MASK                          0x07fc0000
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q21_RD(src)       (((src) & 0x07fc0000)>>18)
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q21_WR(src)  (((u32)(src)<<18) & 0x07fc0000)
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q21_SET(dst,src) \
                      (((dst) & ~0x07fc0000) | (((u32)(src)<<18) & 0x07fc0000))
/*	 Fields cfg_idle_slope_wrr_weights_q1	 */
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q11_WIDTH                                  9
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q11_SHIFT                                  9
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q11_MASK                          0x0003fe00
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q11_RD(src)        (((src) & 0x0003fe00)>>9)
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q11_WR(src)   (((u32)(src)<<9) & 0x0003fe00)
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q11_SET(dst,src) \
                       (((dst) & ~0x0003fe00) | (((u32)(src)<<9) & 0x0003fe00))
/*	 Fields cfg_idle_slope_wrr_weights_q0	 */
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q01_WIDTH                                  9
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q01_SHIFT                                  0
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q01_MASK                          0x000001ff
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q01_RD(src)           (((src) & 0x000001ff))
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q01_WR(src)      (((u32)(src)) & 0x000001ff)
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q01_SET(dst,src) \
                          (((dst) & ~0x000001ff) | (((u32)(src)) & 0x000001ff))

/*	Register AVB_per_q_idle_slope1_0	*/ 
/*	 Fields cfg_idle_slope_wrr_weights_q5	 */
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q50_WIDTH                                  9
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q50_SHIFT                                 18
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q50_MASK                          0x07fc0000
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q50_RD(src)       (((src) & 0x07fc0000)>>18)
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q50_WR(src)  (((u32)(src)<<18) & 0x07fc0000)
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q50_SET(dst,src) \
                      (((dst) & ~0x07fc0000) | (((u32)(src)<<18) & 0x07fc0000))
/*	 Fields cfg_idle_slope_wrr_weights_q4	 */
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q40_WIDTH                                  9
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q40_SHIFT                                  9
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q40_MASK                          0x0003fe00
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q40_RD(src)        (((src) & 0x0003fe00)>>9)
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q40_WR(src)   (((u32)(src)<<9) & 0x0003fe00)
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q40_SET(dst,src) \
                       (((dst) & ~0x0003fe00) | (((u32)(src)<<9) & 0x0003fe00))
/*	 Fields cfg_idle_slope_wrr_weights_q3	 */
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q30_WIDTH                                  9
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q30_SHIFT                                  0
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q30_MASK                          0x000001ff
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q30_RD(src)           (((src) & 0x000001ff))
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q30_WR(src)      (((u32)(src)) & 0x000001ff)
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q30_SET(dst,src) \
                          (((dst) & ~0x000001ff) | (((u32)(src)) & 0x000001ff))

/*	Register AVB_per_q_idle_slope1_1	*/ 
/*	 Fields cfg_idle_slope_wrr_weights_q5	 */
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q51_WIDTH                                  9
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q51_SHIFT                                 18
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q51_MASK                          0x07fc0000
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q51_RD(src)       (((src) & 0x07fc0000)>>18)
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q51_WR(src)  (((u32)(src)<<18) & 0x07fc0000)
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q51_SET(dst,src) \
                      (((dst) & ~0x07fc0000) | (((u32)(src)<<18) & 0x07fc0000))
/*	 Fields cfg_idle_slope_wrr_weights_q4	 */
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q41_WIDTH                                  9
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q41_SHIFT                                  9
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q41_MASK                          0x0003fe00
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q41_RD(src)        (((src) & 0x0003fe00)>>9)
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q41_WR(src)   (((u32)(src)<<9) & 0x0003fe00)
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q41_SET(dst,src) \
                       (((dst) & ~0x0003fe00) | (((u32)(src)<<9) & 0x0003fe00))
/*	 Fields cfg_idle_slope_wrr_weights_q3	 */
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q31_WIDTH                                  9
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q31_SHIFT                                  0
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q31_MASK                          0x000001ff
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q31_RD(src)           (((src) & 0x000001ff))
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q31_WR(src)      (((u32)(src)) & 0x000001ff)
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q31_SET(dst,src) \
                          (((dst) & ~0x000001ff) | (((u32)(src)) & 0x000001ff))

/*	Register AVB_per_q_idle_slope2_0	*/ 
/*	 Fields cfg_idle_slope_wrr_weights_q7	 */
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q70_WIDTH                                  9
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q70_SHIFT                                  9
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q70_MASK                          0x0003fe00
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q70_RD(src)        (((src) & 0x0003fe00)>>9)
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q70_WR(src)   (((u32)(src)<<9) & 0x0003fe00)
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q70_SET(dst,src) \
                       (((dst) & ~0x0003fe00) | (((u32)(src)<<9) & 0x0003fe00))
/*	 Fields cfg_idle_slope_wrr_weights_q6	 */
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q60_WIDTH                                  9
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q60_SHIFT                                  0
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q60_MASK                          0x000001ff
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q60_RD(src)           (((src) & 0x000001ff))
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q60_WR(src)      (((u32)(src)) & 0x000001ff)
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q60_SET(dst,src) \
                          (((dst) & ~0x000001ff) | (((u32)(src)) & 0x000001ff))

/*	Register AVB_per_q_idle_slope2_1	*/ 
/*	 Fields cfg_idle_slope_wrr_weights_q7	 */
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q71_WIDTH                                  9
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q71_SHIFT                                  9
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q71_MASK                          0x0003fe00
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q71_RD(src)        (((src) & 0x0003fe00)>>9)
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q71_WR(src)   (((u32)(src)<<9) & 0x0003fe00)
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q71_SET(dst,src) \
                       (((dst) & ~0x0003fe00) | (((u32)(src)<<9) & 0x0003fe00))
/*	 Fields cfg_idle_slope_wrr_weights_q6	 */
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q61_WIDTH                                  9
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q61_SHIFT                                  0
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q61_MASK                          0x000001ff
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q61_RD(src)           (((src) & 0x000001ff))
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q61_WR(src)      (((u32)(src)) & 0x000001ff)
#define CFG_IDLE_SLOPE_WRR_WEIGHTS_Q61_SET(dst,src) \
                          (((dst) & ~0x000001ff) | (((u32)(src)) & 0x000001ff))

/*	Register AVB_per_q_byte_num_q0_0	*/ 
/*	 Fields cfg_avb_q_byte_num	 */
#define CFG_AVB_Q_BYTE_NUM0_WIDTH                                            32
#define CFG_AVB_Q_BYTE_NUM0_SHIFT                                             0
#define CFG_AVB_Q_BYTE_NUM0_MASK                                     0xffffffff
#define CFG_AVB_Q_BYTE_NUM0_RD(src)                      (((src) & 0xffffffff))
#define CFG_AVB_Q_BYTE_NUM0_WR(src)                 (((u32)(src)) & 0xffffffff)
#define CFG_AVB_Q_BYTE_NUM0_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register AVB_per_q_byte_num_q0_1	*/ 
/*	 Fields cfg_avb_q_byte_num	 */
#define CFG_AVB_Q_BYTE_NUM1_WIDTH                                            32
#define CFG_AVB_Q_BYTE_NUM1_SHIFT                                             0
#define CFG_AVB_Q_BYTE_NUM1_MASK                                     0xffffffff
#define CFG_AVB_Q_BYTE_NUM1_RD(src)                      (((src) & 0xffffffff))
#define CFG_AVB_Q_BYTE_NUM1_WR(src)                 (((u32)(src)) & 0xffffffff)
#define CFG_AVB_Q_BYTE_NUM1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register AVB_per_q_byte_num_q1_0	*/ 
/*	 Fields cfg_avb_q_byte_num	 */
#define CFG_AVB_Q_BYTE_NUM0_F1_WIDTH                                         32
#define CFG_AVB_Q_BYTE_NUM0_F1_SHIFT                                          0
#define CFG_AVB_Q_BYTE_NUM0_F1_MASK                                  0xffffffff
#define CFG_AVB_Q_BYTE_NUM0_F1_RD(src)                   (((src) & 0xffffffff))
#define CFG_AVB_Q_BYTE_NUM0_F1_WR(src)              (((u32)(src)) & 0xffffffff)
#define CFG_AVB_Q_BYTE_NUM0_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register AVB_per_q_byte_num_q1_1	*/ 
/*	 Fields cfg_avb_q_byte_num	 */
#define CFG_AVB_Q_BYTE_NUM1_F1_WIDTH                                         32
#define CFG_AVB_Q_BYTE_NUM1_F1_SHIFT                                          0
#define CFG_AVB_Q_BYTE_NUM1_F1_MASK                                  0xffffffff
#define CFG_AVB_Q_BYTE_NUM1_F1_RD(src)                   (((src) & 0xffffffff))
#define CFG_AVB_Q_BYTE_NUM1_F1_WR(src)              (((u32)(src)) & 0xffffffff)
#define CFG_AVB_Q_BYTE_NUM1_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register AVB_per_q_byte_num_q2_0	*/ 
/*	 Fields cfg_avb_q_byte_num	 */
#define CFG_AVB_Q_BYTE_NUM0_F2_WIDTH                                         32
#define CFG_AVB_Q_BYTE_NUM0_F2_SHIFT                                          0
#define CFG_AVB_Q_BYTE_NUM0_F2_MASK                                  0xffffffff
#define CFG_AVB_Q_BYTE_NUM0_F2_RD(src)                   (((src) & 0xffffffff))
#define CFG_AVB_Q_BYTE_NUM0_F2_WR(src)              (((u32)(src)) & 0xffffffff)
#define CFG_AVB_Q_BYTE_NUM0_F2_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register AVB_per_q_byte_num_q2_1	*/ 
/*	 Fields cfg_avb_q_byte_num	 */
#define CFG_AVB_Q_BYTE_NUM1_F2_WIDTH                                         32
#define CFG_AVB_Q_BYTE_NUM1_F2_SHIFT                                          0
#define CFG_AVB_Q_BYTE_NUM1_F2_MASK                                  0xffffffff
#define CFG_AVB_Q_BYTE_NUM1_F2_RD(src)                   (((src) & 0xffffffff))
#define CFG_AVB_Q_BYTE_NUM1_F2_WR(src)              (((u32)(src)) & 0xffffffff)
#define CFG_AVB_Q_BYTE_NUM1_F2_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register AVB_per_q_byte_num_q3_0	*/ 
/*	 Fields cfg_avb_q_byte_num	 */
#define CFG_AVB_Q_BYTE_NUM0_F3_WIDTH                                         32
#define CFG_AVB_Q_BYTE_NUM0_F3_SHIFT                                          0
#define CFG_AVB_Q_BYTE_NUM0_F3_MASK                                  0xffffffff
#define CFG_AVB_Q_BYTE_NUM0_F3_RD(src)                   (((src) & 0xffffffff))
#define CFG_AVB_Q_BYTE_NUM0_F3_WR(src)              (((u32)(src)) & 0xffffffff)
#define CFG_AVB_Q_BYTE_NUM0_F3_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register AVB_per_q_byte_num_q3_1	*/ 
/*	 Fields cfg_avb_q_byte_num	 */
#define CFG_AVB_Q_BYTE_NUM1_F3_WIDTH                                         32
#define CFG_AVB_Q_BYTE_NUM1_F3_SHIFT                                          0
#define CFG_AVB_Q_BYTE_NUM1_F3_MASK                                  0xffffffff
#define CFG_AVB_Q_BYTE_NUM1_F3_RD(src)                   (((src) & 0xffffffff))
#define CFG_AVB_Q_BYTE_NUM1_F3_WR(src)              (((u32)(src)) & 0xffffffff)
#define CFG_AVB_Q_BYTE_NUM1_F3_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register AVB_per_q_byte_num_q4_0	*/ 
/*	 Fields cfg_avb_q_byte_num	 */
#define CFG_AVB_Q_BYTE_NUM0_F4_WIDTH                                         32
#define CFG_AVB_Q_BYTE_NUM0_F4_SHIFT                                          0
#define CFG_AVB_Q_BYTE_NUM0_F4_MASK                                  0xffffffff
#define CFG_AVB_Q_BYTE_NUM0_F4_RD(src)                   (((src) & 0xffffffff))
#define CFG_AVB_Q_BYTE_NUM0_F4_WR(src)              (((u32)(src)) & 0xffffffff)
#define CFG_AVB_Q_BYTE_NUM0_F4_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register AVB_per_q_byte_num_q4_1	*/ 
/*	 Fields cfg_avb_q_byte_num	 */
#define CFG_AVB_Q_BYTE_NUM1_F4_WIDTH                                         32
#define CFG_AVB_Q_BYTE_NUM1_F4_SHIFT                                          0
#define CFG_AVB_Q_BYTE_NUM1_F4_MASK                                  0xffffffff
#define CFG_AVB_Q_BYTE_NUM1_F4_RD(src)                   (((src) & 0xffffffff))
#define CFG_AVB_Q_BYTE_NUM1_F4_WR(src)              (((u32)(src)) & 0xffffffff)
#define CFG_AVB_Q_BYTE_NUM1_F4_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register AVB_per_q_byte_num_q5_0	*/ 
/*	 Fields cfg_avb_q_byte_num	 */
#define CFG_AVB_Q_BYTE_NUM0_F5_WIDTH                                         32
#define CFG_AVB_Q_BYTE_NUM0_F5_SHIFT                                          0
#define CFG_AVB_Q_BYTE_NUM0_F5_MASK                                  0xffffffff
#define CFG_AVB_Q_BYTE_NUM0_F5_RD(src)                   (((src) & 0xffffffff))
#define CFG_AVB_Q_BYTE_NUM0_F5_WR(src)              (((u32)(src)) & 0xffffffff)
#define CFG_AVB_Q_BYTE_NUM0_F5_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register AVB_per_q_byte_num_q5_1	*/ 
/*	 Fields cfg_avb_q_byte_num	 */
#define CFG_AVB_Q_BYTE_NUM1_F5_WIDTH                                         32
#define CFG_AVB_Q_BYTE_NUM1_F5_SHIFT                                          0
#define CFG_AVB_Q_BYTE_NUM1_F5_MASK                                  0xffffffff
#define CFG_AVB_Q_BYTE_NUM1_F5_RD(src)                   (((src) & 0xffffffff))
#define CFG_AVB_Q_BYTE_NUM1_F5_WR(src)              (((u32)(src)) & 0xffffffff)
#define CFG_AVB_Q_BYTE_NUM1_F5_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register AVB_per_q_byte_num_q6_0	*/ 
/*	 Fields cfg_avb_q_byte_num	 */
#define CFG_AVB_Q_BYTE_NUM0_F6_WIDTH                                         32
#define CFG_AVB_Q_BYTE_NUM0_F6_SHIFT                                          0
#define CFG_AVB_Q_BYTE_NUM0_F6_MASK                                  0xffffffff
#define CFG_AVB_Q_BYTE_NUM0_F6_RD(src)                   (((src) & 0xffffffff))
#define CFG_AVB_Q_BYTE_NUM0_F6_WR(src)              (((u32)(src)) & 0xffffffff)
#define CFG_AVB_Q_BYTE_NUM0_F6_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register AVB_per_q_byte_num_q6_1	*/ 
/*	 Fields cfg_avb_q_byte_num	 */
#define CFG_AVB_Q_BYTE_NUM1_F6_WIDTH                                         32
#define CFG_AVB_Q_BYTE_NUM1_F6_SHIFT                                          0
#define CFG_AVB_Q_BYTE_NUM1_F6_MASK                                  0xffffffff
#define CFG_AVB_Q_BYTE_NUM1_F6_RD(src)                   (((src) & 0xffffffff))
#define CFG_AVB_Q_BYTE_NUM1_F6_WR(src)              (((u32)(src)) & 0xffffffff)
#define CFG_AVB_Q_BYTE_NUM1_F6_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register AVB_per_q_byte_num_q7_0	*/ 
/*	 Fields cfg_avb_q_byte_num	 */
#define CFG_AVB_Q_BYTE_NUM0_F7_WIDTH                                         32
#define CFG_AVB_Q_BYTE_NUM0_F7_SHIFT                                          0
#define CFG_AVB_Q_BYTE_NUM0_F7_MASK                                  0xffffffff
#define CFG_AVB_Q_BYTE_NUM0_F7_RD(src)                   (((src) & 0xffffffff))
#define CFG_AVB_Q_BYTE_NUM0_F7_WR(src)              (((u32)(src)) & 0xffffffff)
#define CFG_AVB_Q_BYTE_NUM0_F7_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register AVB_per_q_byte_num_q7_1	*/ 
/*	 Fields cfg_avb_q_byte_num	 */
#define CFG_AVB_Q_BYTE_NUM1_F7_WIDTH                                         32
#define CFG_AVB_Q_BYTE_NUM1_F7_SHIFT                                          0
#define CFG_AVB_Q_BYTE_NUM1_F7_MASK                                  0xffffffff
#define CFG_AVB_Q_BYTE_NUM1_F7_RD(src)                   (((src) & 0xffffffff))
#define CFG_AVB_Q_BYTE_NUM1_F7_WR(src)              (((u32)(src)) & 0xffffffff)
#define CFG_AVB_Q_BYTE_NUM1_F7_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register AVB_cfg_prsntn_time_0	*/ 
/*	 Fields offset	 */
#define OFFSET0_WIDTH                                                        32
#define OFFSET0_SHIFT                                                         0
#define OFFSET0_MASK                                                 0xffffffff
#define OFFSET0_RD(src)                                  (((src) & 0xffffffff))
#define OFFSET0_WR(src)                             (((u32)(src)) & 0xffffffff)
#define OFFSET0_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register AVB_cfg_prsntn_time_1	*/ 
/*	 Fields offset	 */
#define OFFSET1_WIDTH                                                        32
#define OFFSET1_SHIFT                                                         0
#define OFFSET1_MASK                                                 0xffffffff
#define OFFSET1_RD(src)                                  (((src) & 0xffffffff))
#define OFFSET1_WR(src)                             (((u32)(src)) & 0xffffffff)
#define OFFSET1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register cfg_link_aggr	*/ 
/*	 Fields mpa_q_port_sel	 */
#define MPA_Q_PORT_SEL_WIDTH                                                  1
#define MPA_Q_PORT_SEL_SHIFT                                                  3
#define MPA_Q_PORT_SEL_MASK                                          0x00000008
#define MPA_Q_PORT_SEL_RD(src)                        (((src) & 0x00000008)>>3)
#define MPA_Q_PORT_SEL_WR(src)                   (((u32)(src)<<3) & 0x00000008)
#define MPA_Q_PORT_SEL_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields arb_sel	 */
#define ARB_SEL_WIDTH                                                         1
#define ARB_SEL_SHIFT                                                         2
#define ARB_SEL_MASK                                                 0x00000004
#define ARB_SEL_RD(src)                               (((src) & 0x00000004)>>2)
#define ARB_SEL_WR(src)                          (((u32)(src)<<2) & 0x00000004)
#define ARB_SEL_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields en	 */
#define EN_WIDTH                                                              1
#define EN_SHIFT                                                              1
#define EN_MASK                                                      0x00000002
#define EN_RD(src)                                    (((src) & 0x00000002)>>1)
#define EN_WR(src)                               (((u32)(src)<<1) & 0x00000002)
#define EN_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields port_sel	 */
#define PORT_SEL_WIDTH                                                        1
#define PORT_SEL_SHIFT                                                        0
#define PORT_SEL_MASK                                                0x00000001
#define PORT_SEL_RD(src)                                 (((src) & 0x00000001))
#define PORT_SEL_WR(src)                            (((u32)(src)) & 0x00000001)
#define PORT_SEL_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register MH_FIFO_full_thresh_0	*/ 
/*	 Fields cfg_prio7_ffull_thresh	 */
#define CFG_PRIO7_FFULL_THRESH0_WIDTH                                         4
#define CFG_PRIO7_FFULL_THRESH0_SHIFT                                        28
#define CFG_PRIO7_FFULL_THRESH0_MASK                                 0xf0000000
#define CFG_PRIO7_FFULL_THRESH0_RD(src)              (((src) & 0xf0000000)>>28)
#define CFG_PRIO7_FFULL_THRESH0_WR(src)         (((u32)(src)<<28) & 0xf0000000)
#define CFG_PRIO7_FFULL_THRESH0_SET(dst,src) \
                      (((dst) & ~0xf0000000) | (((u32)(src)<<28) & 0xf0000000))
/*	 Fields cfg_prio6_ffull_thresh	 */
#define CFG_PRIO6_FFULL_THRESH0_WIDTH                                         4
#define CFG_PRIO6_FFULL_THRESH0_SHIFT                                        24
#define CFG_PRIO6_FFULL_THRESH0_MASK                                 0x0f000000
#define CFG_PRIO6_FFULL_THRESH0_RD(src)              (((src) & 0x0f000000)>>24)
#define CFG_PRIO6_FFULL_THRESH0_WR(src)         (((u32)(src)<<24) & 0x0f000000)
#define CFG_PRIO6_FFULL_THRESH0_SET(dst,src) \
                      (((dst) & ~0x0f000000) | (((u32)(src)<<24) & 0x0f000000))
/*	 Fields cfg_prio5_ffull_thresh	 */
#define CFG_PRIO5_FFULL_THRESH0_WIDTH                                         4
#define CFG_PRIO5_FFULL_THRESH0_SHIFT                                        20
#define CFG_PRIO5_FFULL_THRESH0_MASK                                 0x00f00000
#define CFG_PRIO5_FFULL_THRESH0_RD(src)              (((src) & 0x00f00000)>>20)
#define CFG_PRIO5_FFULL_THRESH0_WR(src)         (((u32)(src)<<20) & 0x00f00000)
#define CFG_PRIO5_FFULL_THRESH0_SET(dst,src) \
                      (((dst) & ~0x00f00000) | (((u32)(src)<<20) & 0x00f00000))
/*	 Fields cfg_prio4_ffull_thresh	 */
#define CFG_PRIO4_FFULL_THRESH0_WIDTH                                         4
#define CFG_PRIO4_FFULL_THRESH0_SHIFT                                        16
#define CFG_PRIO4_FFULL_THRESH0_MASK                                 0x000f0000
#define CFG_PRIO4_FFULL_THRESH0_RD(src)              (((src) & 0x000f0000)>>16)
#define CFG_PRIO4_FFULL_THRESH0_WR(src)         (((u32)(src)<<16) & 0x000f0000)
#define CFG_PRIO4_FFULL_THRESH0_SET(dst,src) \
                      (((dst) & ~0x000f0000) | (((u32)(src)<<16) & 0x000f0000))
/*	 Fields cfg_prio3_ffull_thresh	 */
#define CFG_PRIO3_FFULL_THRESH0_WIDTH                                         4
#define CFG_PRIO3_FFULL_THRESH0_SHIFT                                        12
#define CFG_PRIO3_FFULL_THRESH0_MASK                                 0x0000f000
#define CFG_PRIO3_FFULL_THRESH0_RD(src)              (((src) & 0x0000f000)>>12)
#define CFG_PRIO3_FFULL_THRESH0_WR(src)         (((u32)(src)<<12) & 0x0000f000)
#define CFG_PRIO3_FFULL_THRESH0_SET(dst,src) \
                      (((dst) & ~0x0000f000) | (((u32)(src)<<12) & 0x0000f000))
/*	 Fields cfg_prio2_ffull_thresh	 */
#define CFG_PRIO2_FFULL_THRESH0_WIDTH                                         4
#define CFG_PRIO2_FFULL_THRESH0_SHIFT                                         8
#define CFG_PRIO2_FFULL_THRESH0_MASK                                 0x00000f00
#define CFG_PRIO2_FFULL_THRESH0_RD(src)               (((src) & 0x00000f00)>>8)
#define CFG_PRIO2_FFULL_THRESH0_WR(src)          (((u32)(src)<<8) & 0x00000f00)
#define CFG_PRIO2_FFULL_THRESH0_SET(dst,src) \
                       (((dst) & ~0x00000f00) | (((u32)(src)<<8) & 0x00000f00))
/*	 Fields cfg_prio1_ffull_thresh	 */
#define CFG_PRIO1_FFULL_THRESH0_WIDTH                                         4
#define CFG_PRIO1_FFULL_THRESH0_SHIFT                                         4
#define CFG_PRIO1_FFULL_THRESH0_MASK                                 0x000000f0
#define CFG_PRIO1_FFULL_THRESH0_RD(src)               (((src) & 0x000000f0)>>4)
#define CFG_PRIO1_FFULL_THRESH0_WR(src)          (((u32)(src)<<4) & 0x000000f0)
#define CFG_PRIO1_FFULL_THRESH0_SET(dst,src) \
                       (((dst) & ~0x000000f0) | (((u32)(src)<<4) & 0x000000f0))
/*	 Fields cfg_prio0_ffull_thresh	 */
#define CFG_PRIO0_FFULL_THRESH0_WIDTH                                         4
#define CFG_PRIO0_FFULL_THRESH0_SHIFT                                         0
#define CFG_PRIO0_FFULL_THRESH0_MASK                                 0x0000000f
#define CFG_PRIO0_FFULL_THRESH0_RD(src)                  (((src) & 0x0000000f))
#define CFG_PRIO0_FFULL_THRESH0_WR(src)             (((u32)(src)) & 0x0000000f)
#define CFG_PRIO0_FFULL_THRESH0_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register MH_FIFO_full_thresh_1	*/ 
/*	 Fields cfg_prio7_ffull_thresh	 */
#define CFG_PRIO7_FFULL_THRESH1_WIDTH                                         4
#define CFG_PRIO7_FFULL_THRESH1_SHIFT                                        28
#define CFG_PRIO7_FFULL_THRESH1_MASK                                 0xf0000000
#define CFG_PRIO7_FFULL_THRESH1_RD(src)              (((src) & 0xf0000000)>>28)
#define CFG_PRIO7_FFULL_THRESH1_WR(src)         (((u32)(src)<<28) & 0xf0000000)
#define CFG_PRIO7_FFULL_THRESH1_SET(dst,src) \
                      (((dst) & ~0xf0000000) | (((u32)(src)<<28) & 0xf0000000))
/*	 Fields cfg_prio6_ffull_thresh	 */
#define CFG_PRIO6_FFULL_THRESH1_WIDTH                                         4
#define CFG_PRIO6_FFULL_THRESH1_SHIFT                                        24
#define CFG_PRIO6_FFULL_THRESH1_MASK                                 0x0f000000
#define CFG_PRIO6_FFULL_THRESH1_RD(src)              (((src) & 0x0f000000)>>24)
#define CFG_PRIO6_FFULL_THRESH1_WR(src)         (((u32)(src)<<24) & 0x0f000000)
#define CFG_PRIO6_FFULL_THRESH1_SET(dst,src) \
                      (((dst) & ~0x0f000000) | (((u32)(src)<<24) & 0x0f000000))
/*	 Fields cfg_prio5_ffull_thresh	 */
#define CFG_PRIO5_FFULL_THRESH1_WIDTH                                         4
#define CFG_PRIO5_FFULL_THRESH1_SHIFT                                        20
#define CFG_PRIO5_FFULL_THRESH1_MASK                                 0x00f00000
#define CFG_PRIO5_FFULL_THRESH1_RD(src)              (((src) & 0x00f00000)>>20)
#define CFG_PRIO5_FFULL_THRESH1_WR(src)         (((u32)(src)<<20) & 0x00f00000)
#define CFG_PRIO5_FFULL_THRESH1_SET(dst,src) \
                      (((dst) & ~0x00f00000) | (((u32)(src)<<20) & 0x00f00000))
/*	 Fields cfg_prio4_ffull_thresh	 */
#define CFG_PRIO4_FFULL_THRESH1_WIDTH                                         4
#define CFG_PRIO4_FFULL_THRESH1_SHIFT                                        16
#define CFG_PRIO4_FFULL_THRESH1_MASK                                 0x000f0000
#define CFG_PRIO4_FFULL_THRESH1_RD(src)              (((src) & 0x000f0000)>>16)
#define CFG_PRIO4_FFULL_THRESH1_WR(src)         (((u32)(src)<<16) & 0x000f0000)
#define CFG_PRIO4_FFULL_THRESH1_SET(dst,src) \
                      (((dst) & ~0x000f0000) | (((u32)(src)<<16) & 0x000f0000))
/*	 Fields cfg_prio3_ffull_thresh	 */
#define CFG_PRIO3_FFULL_THRESH1_WIDTH                                         4
#define CFG_PRIO3_FFULL_THRESH1_SHIFT                                        12
#define CFG_PRIO3_FFULL_THRESH1_MASK                                 0x0000f000
#define CFG_PRIO3_FFULL_THRESH1_RD(src)              (((src) & 0x0000f000)>>12)
#define CFG_PRIO3_FFULL_THRESH1_WR(src)         (((u32)(src)<<12) & 0x0000f000)
#define CFG_PRIO3_FFULL_THRESH1_SET(dst,src) \
                      (((dst) & ~0x0000f000) | (((u32)(src)<<12) & 0x0000f000))
/*	 Fields cfg_prio2_ffull_thresh	 */
#define CFG_PRIO2_FFULL_THRESH1_WIDTH                                         4
#define CFG_PRIO2_FFULL_THRESH1_SHIFT                                         8
#define CFG_PRIO2_FFULL_THRESH1_MASK                                 0x00000f00
#define CFG_PRIO2_FFULL_THRESH1_RD(src)               (((src) & 0x00000f00)>>8)
#define CFG_PRIO2_FFULL_THRESH1_WR(src)          (((u32)(src)<<8) & 0x00000f00)
#define CFG_PRIO2_FFULL_THRESH1_SET(dst,src) \
                       (((dst) & ~0x00000f00) | (((u32)(src)<<8) & 0x00000f00))
/*	 Fields cfg_prio1_ffull_thresh	 */
#define CFG_PRIO1_FFULL_THRESH1_WIDTH                                         4
#define CFG_PRIO1_FFULL_THRESH1_SHIFT                                         4
#define CFG_PRIO1_FFULL_THRESH1_MASK                                 0x000000f0
#define CFG_PRIO1_FFULL_THRESH1_RD(src)               (((src) & 0x000000f0)>>4)
#define CFG_PRIO1_FFULL_THRESH1_WR(src)          (((u32)(src)<<4) & 0x000000f0)
#define CFG_PRIO1_FFULL_THRESH1_SET(dst,src) \
                       (((dst) & ~0x000000f0) | (((u32)(src)<<4) & 0x000000f0))
/*	 Fields cfg_prio0_ffull_thresh	 */
#define CFG_PRIO0_FFULL_THRESH1_WIDTH                                         4
#define CFG_PRIO0_FFULL_THRESH1_SHIFT                                         0
#define CFG_PRIO0_FFULL_THRESH1_MASK                                 0x0000000f
#define CFG_PRIO0_FFULL_THRESH1_RD(src)                  (((src) & 0x0000000f))
#define CFG_PRIO0_FFULL_THRESH1_WR(src)             (((u32)(src)) & 0x0000000f)
#define CFG_PRIO0_FFULL_THRESH1_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register DEALLOC_FIFO_full_thresh_0	*/ 
/*	 Fields cfg_dealloc_ffull_thresh	 */
#define CFG_DEALLOC_FFULL_THRESH0_WIDTH                                       4
#define CFG_DEALLOC_FFULL_THRESH0_SHIFT                                       0
#define CFG_DEALLOC_FFULL_THRESH0_MASK                               0x0000000f
#define CFG_DEALLOC_FFULL_THRESH0_RD(src)                (((src) & 0x0000000f))
#define CFG_DEALLOC_FFULL_THRESH0_WR(src)           (((u32)(src)) & 0x0000000f)
#define CFG_DEALLOC_FFULL_THRESH0_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register DEALLOC_FIFO_full_thresh_1	*/ 
/*	 Fields cfg_dealloc_ffull_thresh	 */
#define CFG_DEALLOC_FFULL_THRESH1_WIDTH                                       4
#define CFG_DEALLOC_FFULL_THRESH1_SHIFT                                       0
#define CFG_DEALLOC_FFULL_THRESH1_MASK                               0x0000000f
#define CFG_DEALLOC_FFULL_THRESH1_RD(src)                (((src) & 0x0000000f))
#define CFG_DEALLOC_FFULL_THRESH1_WR(src)           (((u32)(src)) & 0x0000000f)
#define CFG_DEALLOC_FFULL_THRESH1_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register TSO_FIFO_Status	*/ 
/*	 Fields prio0_fifo_rlevelcnt_prt1	 */
#define PRIO0_FIFO_RLEVELCNT_PRT1_WIDTH                                       4
#define PRIO0_FIFO_RLEVELCNT_PRT1_SHIFT                                      24
#define PRIO0_FIFO_RLEVELCNT_PRT1_MASK                               0x0f000000
#define PRIO0_FIFO_RLEVELCNT_PRT1_RD(src)            (((src) & 0x0f000000)>>24)
#define PRIO0_FIFO_RLEVELCNT_PRT1_SET(dst,src) \
                      (((dst) & ~0x0f000000) | (((u32)(src)<<24) & 0x0f000000))
/*	 Fields mh_dealloc_RLevelCnt_prt1	 */
#define MH_DEALLOC_RLEVELCNT_PRT1_WIDTH                                       4
#define MH_DEALLOC_RLEVELCNT_PRT1_SHIFT                                      20
#define MH_DEALLOC_RLEVELCNT_PRT1_MASK                               0x00f00000
#define MH_DEALLOC_RLEVELCNT_PRT1_RD(src)            (((src) & 0x00f00000)>>20)
#define MH_DEALLOC_RLEVELCNT_PRT1_SET(dst,src) \
                      (((dst) & ~0x00f00000) | (((u32)(src)<<20) & 0x00f00000))
/*	 Fields mb_ttf_rlevelcnt_prt1	 */
#define MB_TTF_RLEVELCNT_PRT1_WIDTH                                           4
#define MB_TTF_RLEVELCNT_PRT1_SHIFT                                          16
#define MB_TTF_RLEVELCNT_PRT1_MASK                                   0x000f0000
#define MB_TTF_RLEVELCNT_PRT1_RD(src)                (((src) & 0x000f0000)>>16)
#define MB_TTF_RLEVELCNT_PRT1_SET(dst,src) \
                      (((dst) & ~0x000f0000) | (((u32)(src)<<16) & 0x000f0000))
/*	 Fields prio0_fifo_rlevelcnt_prt0	 */
#define PRIO0_FIFO_RLEVELCNT_PRT0_WIDTH                                       4
#define PRIO0_FIFO_RLEVELCNT_PRT0_SHIFT                                       8
#define PRIO0_FIFO_RLEVELCNT_PRT0_MASK                               0x00000f00
#define PRIO0_FIFO_RLEVELCNT_PRT0_RD(src)             (((src) & 0x00000f00)>>8)
#define PRIO0_FIFO_RLEVELCNT_PRT0_SET(dst,src) \
                       (((dst) & ~0x00000f00) | (((u32)(src)<<8) & 0x00000f00))
/*	 Fields mh_dealloc_RLevelCnt_prt0	 */
#define MH_DEALLOC_RLEVELCNT_PRT0_WIDTH                                       4
#define MH_DEALLOC_RLEVELCNT_PRT0_SHIFT                                       4
#define MH_DEALLOC_RLEVELCNT_PRT0_MASK                               0x000000f0
#define MH_DEALLOC_RLEVELCNT_PRT0_RD(src)             (((src) & 0x000000f0)>>4)
#define MH_DEALLOC_RLEVELCNT_PRT0_SET(dst,src) \
                       (((dst) & ~0x000000f0) | (((u32)(src)<<4) & 0x000000f0))
/*	 Fields mb_ttf_rlevelcnt_prt0	 */
#define MB_TTF_RLEVELCNT_PRT0_WIDTH                                           4
#define MB_TTF_RLEVELCNT_PRT0_SHIFT                                           0
#define MB_TTF_RLEVELCNT_PRT0_MASK                                   0x0000000f
#define MB_TTF_RLEVELCNT_PRT0_RD(src)                    (((src) & 0x0000000f))
#define MB_TTF_RLEVELCNT_PRT0_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register TSO_cfg_0	*/ 
/*	 Fields tcp_flag_position	 */
#define TCP_FLAG_POSITION0_WIDTH                                              2
#define TCP_FLAG_POSITION0_SHIFT                                              6
#define TCP_FLAG_POSITION0_MASK                                      0x000000c0
#define TCP_FLAG_POSITION0_RD(src)                    (((src) & 0x000000c0)>>6)
#define TCP_FLAG_POSITION0_WR(src)               (((u32)(src)<<6) & 0x000000c0)
#define TCP_FLAG_POSITION0_SET(dst,src) \
                       (((dst) & ~0x000000c0) | (((u32)(src)<<6) & 0x000000c0))
/*	 Fields mac_prop_hdr_len	 */
#define MAC_PROP_HDR_LEN0_WIDTH                                               6
#define MAC_PROP_HDR_LEN0_SHIFT                                               0
#define MAC_PROP_HDR_LEN0_MASK                                       0x0000003f
#define MAC_PROP_HDR_LEN0_RD(src)                        (((src) & 0x0000003f))
#define MAC_PROP_HDR_LEN0_WR(src)                   (((u32)(src)) & 0x0000003f)
#define MAC_PROP_HDR_LEN0_SET(dst,src) \
                          (((dst) & ~0x0000003f) | (((u32)(src)) & 0x0000003f))

/*	Register TSO_cfg_1	*/ 
/*	 Fields tcp_flag_position	 */
#define TCP_FLAG_POSITION1_WIDTH                                              2
#define TCP_FLAG_POSITION1_SHIFT                                              6
#define TCP_FLAG_POSITION1_MASK                                      0x000000c0
#define TCP_FLAG_POSITION1_RD(src)                    (((src) & 0x000000c0)>>6)
#define TCP_FLAG_POSITION1_WR(src)               (((u32)(src)<<6) & 0x000000c0)
#define TCP_FLAG_POSITION1_SET(dst,src) \
                       (((dst) & ~0x000000c0) | (((u32)(src)<<6) & 0x000000c0))
/*	 Fields mac_prop_hdr_len	 */
#define MAC_PROP_HDR_LEN1_WIDTH                                               6
#define MAC_PROP_HDR_LEN1_SHIFT                                               0
#define MAC_PROP_HDR_LEN1_MASK                                       0x0000003f
#define MAC_PROP_HDR_LEN1_RD(src)                        (((src) & 0x0000003f))
#define MAC_PROP_HDR_LEN1_WR(src)                   (((u32)(src)) & 0x0000003f)
#define MAC_PROP_HDR_LEN1_SET(dst,src) \
                          (((dst) & ~0x0000003f) | (((u32)(src)) & 0x0000003f))

/*	Register TSO_cfg_insert_vlan_0	*/ 
/*	 Fields id_tag	 */
#define ID_TAG0_WIDTH                                                        32
#define ID_TAG0_SHIFT                                                         0
#define ID_TAG0_MASK                                                 0xffffffff
#define ID_TAG0_RD(src)                                  (((src) & 0xffffffff))
#define ID_TAG0_WR(src)                             (((u32)(src)) & 0xffffffff)
#define ID_TAG0_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register TSO_cfg_insert_vlan_1	*/ 
/*	 Fields id_tag	 */
#define ID_TAG1_WIDTH                                                        32
#define ID_TAG1_SHIFT                                                         0
#define ID_TAG1_MASK                                                 0xffffffff
#define ID_TAG1_RD(src)                                  (((src) & 0xffffffff))
#define ID_TAG1_WR(src)                             (((u32)(src)) & 0xffffffff)
#define ID_TAG1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register TSO_Int_Reg0	*/ 
/*	 Fields mb_ttf_fifo_overfl_intr_prt1	 */
#define MB_TTF_FIFO_OVERFL_INTR_PRT10_WIDTH                                   1
#define MB_TTF_FIFO_OVERFL_INTR_PRT10_SHIFT                                  21
#define MB_TTF_FIFO_OVERFL_INTR_PRT10_MASK                           0x00200000
#define MB_TTF_FIFO_OVERFL_INTR_PRT10_RD(src)        (((src) & 0x00200000)>>21)
#define MB_TTF_FIFO_OVERFL_INTR_PRT10_WR(src)   (((u32)(src)<<21) & 0x00200000)
#define MB_TTF_FIFO_OVERFL_INTR_PRT10_SET(dst,src) \
                      (((dst) & ~0x00200000) | (((u32)(src)<<21) & 0x00200000))
/*	 Fields mb_ttf_fifo_underfl_intr_prt1	 */
#define MB_TTF_FIFO_UNDERFL_INTR_PRT10_WIDTH                                  1
#define MB_TTF_FIFO_UNDERFL_INTR_PRT10_SHIFT                                 20
#define MB_TTF_FIFO_UNDERFL_INTR_PRT10_MASK                          0x00100000
#define MB_TTF_FIFO_UNDERFL_INTR_PRT10_RD(src)       (((src) & 0x00100000)>>20)
#define MB_TTF_FIFO_UNDERFL_INTR_PRT10_WR(src)  (((u32)(src)<<20) & 0x00100000)
#define MB_TTF_FIFO_UNDERFL_INTR_PRT10_SET(dst,src) \
                      (((dst) & ~0x00100000) | (((u32)(src)<<20) & 0x00100000))
/*	 Fields mh_dealloc_fifo_overfl_intr_prt1	 */
#define MH_DEALLOC_FIFO_OVERFL_INTR_PRT10_WIDTH                               1
#define MH_DEALLOC_FIFO_OVERFL_INTR_PRT10_SHIFT                              19
#define MH_DEALLOC_FIFO_OVERFL_INTR_PRT10_MASK                       0x00080000
#define MH_DEALLOC_FIFO_OVERFL_INTR_PRT10_RD(src)    (((src) & 0x00080000)>>19)
#define MH_DEALLOC_FIFO_OVERFL_INTR_PRT10_WR(src) \
                                                (((u32)(src)<<19) & 0x00080000)
#define MH_DEALLOC_FIFO_OVERFL_INTR_PRT10_SET(dst,src) \
                      (((dst) & ~0x00080000) | (((u32)(src)<<19) & 0x00080000))
/*	 Fields mh_dealloc_fifo_underfl_intr_prt1	 */
#define MH_DEALLOC_FIFO_UNDERFL_INTR_PRT10_WIDTH                              1
#define MH_DEALLOC_FIFO_UNDERFL_INTR_PRT10_SHIFT                             18
#define MH_DEALLOC_FIFO_UNDERFL_INTR_PRT10_MASK                      0x00040000
#define MH_DEALLOC_FIFO_UNDERFL_INTR_PRT10_RD(src)   (((src) & 0x00040000)>>18)
#define MH_DEALLOC_FIFO_UNDERFL_INTR_PRT10_WR(src) \
                                                (((u32)(src)<<18) & 0x00040000)
#define MH_DEALLOC_FIFO_UNDERFL_INTR_PRT10_SET(dst,src) \
                      (((dst) & ~0x00040000) | (((u32)(src)<<18) & 0x00040000))
/*	 Fields mh_fifo_overfl_intr_prt1	 */
#define MH_FIFO_OVERFL_INTR_PRT10_WIDTH                                       1
#define MH_FIFO_OVERFL_INTR_PRT10_SHIFT                                      17
#define MH_FIFO_OVERFL_INTR_PRT10_MASK                               0x00020000
#define MH_FIFO_OVERFL_INTR_PRT10_RD(src)            (((src) & 0x00020000)>>17)
#define MH_FIFO_OVERFL_INTR_PRT10_WR(src)       (((u32)(src)<<17) & 0x00020000)
#define MH_FIFO_OVERFL_INTR_PRT10_SET(dst,src) \
                      (((dst) & ~0x00020000) | (((u32)(src)<<17) & 0x00020000))
/*	 Fields mh_fifo_underfl_intr_prt1	 */
#define MH_FIFO_UNDERFL_INTR_PRT10_WIDTH                                      1
#define MH_FIFO_UNDERFL_INTR_PRT10_SHIFT                                     16
#define MH_FIFO_UNDERFL_INTR_PRT10_MASK                              0x00010000
#define MH_FIFO_UNDERFL_INTR_PRT10_RD(src)           (((src) & 0x00010000)>>16)
#define MH_FIFO_UNDERFL_INTR_PRT10_WR(src)      (((u32)(src)<<16) & 0x00010000)
#define MH_FIFO_UNDERFL_INTR_PRT10_SET(dst,src) \
                      (((dst) & ~0x00010000) | (((u32)(src)<<16) & 0x00010000))
/*	 Fields mb_ttf_fifo_overfl_intr_prt0	 */
#define MB_TTF_FIFO_OVERFL_INTR_PRT00_WIDTH                                   1
#define MB_TTF_FIFO_OVERFL_INTR_PRT00_SHIFT                                   5
#define MB_TTF_FIFO_OVERFL_INTR_PRT00_MASK                           0x00000020
#define MB_TTF_FIFO_OVERFL_INTR_PRT00_RD(src)         (((src) & 0x00000020)>>5)
#define MB_TTF_FIFO_OVERFL_INTR_PRT00_WR(src)    (((u32)(src)<<5) & 0x00000020)
#define MB_TTF_FIFO_OVERFL_INTR_PRT00_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields mb_ttf_fifo_underfl_intr_prt0	 */
#define MB_TTF_FIFO_UNDERFL_INTR_PRT00_WIDTH                                  1
#define MB_TTF_FIFO_UNDERFL_INTR_PRT00_SHIFT                                  4
#define MB_TTF_FIFO_UNDERFL_INTR_PRT00_MASK                          0x00000010
#define MB_TTF_FIFO_UNDERFL_INTR_PRT00_RD(src)        (((src) & 0x00000010)>>4)
#define MB_TTF_FIFO_UNDERFL_INTR_PRT00_WR(src)   (((u32)(src)<<4) & 0x00000010)
#define MB_TTF_FIFO_UNDERFL_INTR_PRT00_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields mh_dealloc_fifo_overfl_intr_prt0	 */
#define MH_DEALLOC_FIFO_OVERFL_INTR_PRT00_WIDTH                               1
#define MH_DEALLOC_FIFO_OVERFL_INTR_PRT00_SHIFT                               3
#define MH_DEALLOC_FIFO_OVERFL_INTR_PRT00_MASK                       0x00000008
#define MH_DEALLOC_FIFO_OVERFL_INTR_PRT00_RD(src)     (((src) & 0x00000008)>>3)
#define MH_DEALLOC_FIFO_OVERFL_INTR_PRT00_WR(src) \
                                                 (((u32)(src)<<3) & 0x00000008)
#define MH_DEALLOC_FIFO_OVERFL_INTR_PRT00_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields mh_dealloc_fifo_underfl_intr_prt0	 */
#define MH_DEALLOC_FIFO_UNDERFL_INTR_PRT00_WIDTH                              1
#define MH_DEALLOC_FIFO_UNDERFL_INTR_PRT00_SHIFT                              2
#define MH_DEALLOC_FIFO_UNDERFL_INTR_PRT00_MASK                      0x00000004
#define MH_DEALLOC_FIFO_UNDERFL_INTR_PRT00_RD(src)    (((src) & 0x00000004)>>2)
#define MH_DEALLOC_FIFO_UNDERFL_INTR_PRT00_WR(src) \
                                                 (((u32)(src)<<2) & 0x00000004)
#define MH_DEALLOC_FIFO_UNDERFL_INTR_PRT00_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields mh_fifo_overfl_intr_prt0	 */
#define MH_FIFO_OVERFL_INTR_PRT00_WIDTH                                       1
#define MH_FIFO_OVERFL_INTR_PRT00_SHIFT                                       1
#define MH_FIFO_OVERFL_INTR_PRT00_MASK                               0x00000002
#define MH_FIFO_OVERFL_INTR_PRT00_RD(src)             (((src) & 0x00000002)>>1)
#define MH_FIFO_OVERFL_INTR_PRT00_WR(src)        (((u32)(src)<<1) & 0x00000002)
#define MH_FIFO_OVERFL_INTR_PRT00_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields mh_fifo_underfl_intr_prt0	 */
#define MH_FIFO_UNDERFL_INTR_PRT00_WIDTH                                      1
#define MH_FIFO_UNDERFL_INTR_PRT00_SHIFT                                      0
#define MH_FIFO_UNDERFL_INTR_PRT00_MASK                              0x00000001
#define MH_FIFO_UNDERFL_INTR_PRT00_RD(src)               (((src) & 0x00000001))
#define MH_FIFO_UNDERFL_INTR_PRT00_WR(src)          (((u32)(src)) & 0x00000001)
#define MH_FIFO_UNDERFL_INTR_PRT00_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register TSO_Int_Reg0Mask	*/
/*    Mask Register Fields mb_ttf_fifo_overfl_intr_prt1Mask    */
#define MB_TTF_FIFO_OVERFL_INTR_PRT1MASK_WIDTH                                1
#define MB_TTF_FIFO_OVERFL_INTR_PRT1MASK_SHIFT                               21
#define MB_TTF_FIFO_OVERFL_INTR_PRT1MASK_MASK                        0x00200000
#define MB_TTF_FIFO_OVERFL_INTR_PRT1MASK_RD(src)     (((src) & 0x00200000)>>21)
#define MB_TTF_FIFO_OVERFL_INTR_PRT1MASK_WR(src) \
                                                (((u32)(src)<<21) & 0x00200000)
#define MB_TTF_FIFO_OVERFL_INTR_PRT1MASK_SET(dst,src) \
                      (((dst) & ~0x00200000) | (((u32)(src)<<21) & 0x00200000))
/*    Mask Register Fields mb_ttf_fifo_underfl_intr_prt1Mask    */
#define MB_TTF_FIFO_UNDERFL_INTR_PRT1MASK_WIDTH                               1
#define MB_TTF_FIFO_UNDERFL_INTR_PRT1MASK_SHIFT                              20
#define MB_TTF_FIFO_UNDERFL_INTR_PRT1MASK_MASK                       0x00100000
#define MB_TTF_FIFO_UNDERFL_INTR_PRT1MASK_RD(src)    (((src) & 0x00100000)>>20)
#define MB_TTF_FIFO_UNDERFL_INTR_PRT1MASK_WR(src) \
                                                (((u32)(src)<<20) & 0x00100000)
#define MB_TTF_FIFO_UNDERFL_INTR_PRT1MASK_SET(dst,src) \
                      (((dst) & ~0x00100000) | (((u32)(src)<<20) & 0x00100000))
/*    Mask Register Fields mh_dealloc_fifo_overfl_intr_prt1Mask    */
#define MH_DEALLOC_FIFO_OVERFL_INTR_PRT1MASK_WIDTH                            1
#define MH_DEALLOC_FIFO_OVERFL_INTR_PRT1MASK_SHIFT                           19
#define MH_DEALLOC_FIFO_OVERFL_INTR_PRT1MASK_MASK                    0x00080000
#define MH_DEALLOC_FIFO_OVERFL_INTR_PRT1MASK_RD(src) \
                                                    (((src) & 0x00080000)>>19)
#define MH_DEALLOC_FIFO_OVERFL_INTR_PRT1MASK_WR(src) \
                                                (((u32)(src)<<19) & 0x00080000)
#define MH_DEALLOC_FIFO_OVERFL_INTR_PRT1MASK_SET(dst,src) \
                      (((dst) & ~0x00080000) | (((u32)(src)<<19) & 0x00080000))
/*    Mask Register Fields mh_dealloc_fifo_underfl_intr_prt1Mask    */
#define MH_DEALLOC_FIFO_UNDERFL_INTR_PRT1MASK_WIDTH                           1
#define MH_DEALLOC_FIFO_UNDERFL_INTR_PRT1MASK_SHIFT                          18
#define MH_DEALLOC_FIFO_UNDERFL_INTR_PRT1MASK_MASK                   0x00040000
#define MH_DEALLOC_FIFO_UNDERFL_INTR_PRT1MASK_RD(src) \
                                                    (((src) & 0x00040000)>>18)
#define MH_DEALLOC_FIFO_UNDERFL_INTR_PRT1MASK_WR(src) \
                                                (((u32)(src)<<18) & 0x00040000)
#define MH_DEALLOC_FIFO_UNDERFL_INTR_PRT1MASK_SET(dst,src) \
                      (((dst) & ~0x00040000) | (((u32)(src)<<18) & 0x00040000))
/*    Mask Register Fields mh_fifo_overfl_intr_prt1Mask    */
#define MH_FIFO_OVERFL_INTR_PRT1MASK_WIDTH                                    1
#define MH_FIFO_OVERFL_INTR_PRT1MASK_SHIFT                                   17
#define MH_FIFO_OVERFL_INTR_PRT1MASK_MASK                            0x00020000
#define MH_FIFO_OVERFL_INTR_PRT1MASK_RD(src)         (((src) & 0x00020000)>>17)
#define MH_FIFO_OVERFL_INTR_PRT1MASK_WR(src)    (((u32)(src)<<17) & 0x00020000)
#define MH_FIFO_OVERFL_INTR_PRT1MASK_SET(dst,src) \
                      (((dst) & ~0x00020000) | (((u32)(src)<<17) & 0x00020000))
/*    Mask Register Fields mh_fifo_underfl_intr_prt1Mask    */
#define MH_FIFO_UNDERFL_INTR_PRT1MASK_WIDTH                                   1
#define MH_FIFO_UNDERFL_INTR_PRT1MASK_SHIFT                                  16
#define MH_FIFO_UNDERFL_INTR_PRT1MASK_MASK                           0x00010000
#define MH_FIFO_UNDERFL_INTR_PRT1MASK_RD(src)        (((src) & 0x00010000)>>16)
#define MH_FIFO_UNDERFL_INTR_PRT1MASK_WR(src)   (((u32)(src)<<16) & 0x00010000)
#define MH_FIFO_UNDERFL_INTR_PRT1MASK_SET(dst,src) \
                      (((dst) & ~0x00010000) | (((u32)(src)<<16) & 0x00010000))
/*    Mask Register Fields mb_ttf_fifo_overfl_intr_prt0Mask    */
#define MB_TTF_FIFO_OVERFL_INTR_PRT0MASK_WIDTH                                1
#define MB_TTF_FIFO_OVERFL_INTR_PRT0MASK_SHIFT                                5
#define MB_TTF_FIFO_OVERFL_INTR_PRT0MASK_MASK                        0x00000020
#define MB_TTF_FIFO_OVERFL_INTR_PRT0MASK_RD(src)      (((src) & 0x00000020)>>5)
#define MB_TTF_FIFO_OVERFL_INTR_PRT0MASK_WR(src) \
                                                 (((u32)(src)<<5) & 0x00000020)
#define MB_TTF_FIFO_OVERFL_INTR_PRT0MASK_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*    Mask Register Fields mb_ttf_fifo_underfl_intr_prt0Mask    */
#define MB_TTF_FIFO_UNDERFL_INTR_PRT0MASK_WIDTH                               1
#define MB_TTF_FIFO_UNDERFL_INTR_PRT0MASK_SHIFT                               4
#define MB_TTF_FIFO_UNDERFL_INTR_PRT0MASK_MASK                       0x00000010
#define MB_TTF_FIFO_UNDERFL_INTR_PRT0MASK_RD(src)     (((src) & 0x00000010)>>4)
#define MB_TTF_FIFO_UNDERFL_INTR_PRT0MASK_WR(src) \
                                                 (((u32)(src)<<4) & 0x00000010)
#define MB_TTF_FIFO_UNDERFL_INTR_PRT0MASK_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*    Mask Register Fields mh_dealloc_fifo_overfl_intr_prt0Mask    */
#define MH_DEALLOC_FIFO_OVERFL_INTR_PRT0MASK_WIDTH                            1
#define MH_DEALLOC_FIFO_OVERFL_INTR_PRT0MASK_SHIFT                            3
#define MH_DEALLOC_FIFO_OVERFL_INTR_PRT0MASK_MASK                    0x00000008
#define MH_DEALLOC_FIFO_OVERFL_INTR_PRT0MASK_RD(src)  (((src) & 0x00000008)>>3)
#define MH_DEALLOC_FIFO_OVERFL_INTR_PRT0MASK_WR(src) \
                                                 (((u32)(src)<<3) & 0x00000008)
#define MH_DEALLOC_FIFO_OVERFL_INTR_PRT0MASK_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*    Mask Register Fields mh_dealloc_fifo_underfl_intr_prt0Mask    */
#define MH_DEALLOC_FIFO_UNDERFL_INTR_PRT0MASK_WIDTH                           1
#define MH_DEALLOC_FIFO_UNDERFL_INTR_PRT0MASK_SHIFT                           2
#define MH_DEALLOC_FIFO_UNDERFL_INTR_PRT0MASK_MASK                   0x00000004
#define MH_DEALLOC_FIFO_UNDERFL_INTR_PRT0MASK_RD(src) \
                                                     (((src) & 0x00000004)>>2)
#define MH_DEALLOC_FIFO_UNDERFL_INTR_PRT0MASK_WR(src) \
                                                 (((u32)(src)<<2) & 0x00000004)
#define MH_DEALLOC_FIFO_UNDERFL_INTR_PRT0MASK_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*    Mask Register Fields mh_fifo_overfl_intr_prt0Mask    */
#define MH_FIFO_OVERFL_INTR_PRT0MASK_WIDTH                                    1
#define MH_FIFO_OVERFL_INTR_PRT0MASK_SHIFT                                    1
#define MH_FIFO_OVERFL_INTR_PRT0MASK_MASK                            0x00000002
#define MH_FIFO_OVERFL_INTR_PRT0MASK_RD(src)          (((src) & 0x00000002)>>1)
#define MH_FIFO_OVERFL_INTR_PRT0MASK_WR(src)     (((u32)(src)<<1) & 0x00000002)
#define MH_FIFO_OVERFL_INTR_PRT0MASK_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*    Mask Register Fields mh_fifo_underfl_intr_prt0Mask    */
#define MH_FIFO_UNDERFL_INTR_PRT0MASK_WIDTH                                   1
#define MH_FIFO_UNDERFL_INTR_PRT0MASK_SHIFT                                   0
#define MH_FIFO_UNDERFL_INTR_PRT0MASK_MASK                           0x00000001
#define MH_FIFO_UNDERFL_INTR_PRT0MASK_RD(src)            (((src) & 0x00000001))
#define MH_FIFO_UNDERFL_INTR_PRT0MASK_WR(src)       (((u32)(src)) & 0x00000001)
#define MH_FIFO_UNDERFL_INTR_PRT0MASK_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register TTF_FIFO_full_thresh_0	*/ 
/*	 Fields cfg_ttf_ffull_thresh	 */
#define CFG_TTF_FFULL_THRESH0_WIDTH                                           4
#define CFG_TTF_FFULL_THRESH0_SHIFT                                           0
#define CFG_TTF_FFULL_THRESH0_MASK                                   0x0000000f
#define CFG_TTF_FFULL_THRESH0_RD(src)                    (((src) & 0x0000000f))
#define CFG_TTF_FFULL_THRESH0_WR(src)               (((u32)(src)) & 0x0000000f)
#define CFG_TTF_FFULL_THRESH0_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register TTF_FIFO_full_thresh_1	*/ 
/*	 Fields cfg_ttf_ffull_thresh	 */
#define CFG_TTF_FFULL_THRESH1_WIDTH                                           4
#define CFG_TTF_FFULL_THRESH1_SHIFT                                           0
#define CFG_TTF_FFULL_THRESH1_MASK                                   0x0000000f
#define CFG_TTF_FFULL_THRESH1_RD(src)                    (((src) & 0x0000000f))
#define CFG_TTF_FFULL_THRESH1_WR(src)               (((u32)(src)) & 0x0000000f)
#define CFG_TTF_FFULL_THRESH1_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register SPI2SAP_AddSPI_Reg	*/ 
/*	 Fields cfg_spi2sap_add_rid	 */
#define CFG_SPI2SAP_ADD_RID_WIDTH                                            32
#define CFG_SPI2SAP_ADD_RID_SHIFT                                             0
#define CFG_SPI2SAP_ADD_RID_MASK                                     0xffffffff
#define CFG_SPI2SAP_ADD_RID_RD(src)                      (((src) & 0xffffffff))
#define CFG_SPI2SAP_ADD_RID_WR(src)                 (((u32)(src)) & 0xffffffff)
#define CFG_SPI2SAP_ADD_RID_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register SPI2SAP_AddSPIEn_Reg	*/ 
/*	 Fields cfg_spi2sap_add_rid_port	 */
#define CFG_SPI2SAP_ADD_RID_PORT_WIDTH                                        1
#define CFG_SPI2SAP_ADD_RID_PORT_SHIFT                                        2
#define CFG_SPI2SAP_ADD_RID_PORT_MASK                                0x00000004
#define CFG_SPI2SAP_ADD_RID_PORT_RD(src)              (((src) & 0x00000004)>>2)
#define CFG_SPI2SAP_ADD_RID_PORT_WR(src)         (((u32)(src)<<2) & 0x00000004)
#define CFG_SPI2SAP_ADD_RID_PORT_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields cfg_spi2sap_add_rid_dir	 */
#define CFG_SPI2SAP_ADD_RID_DIR_WIDTH                                         1
#define CFG_SPI2SAP_ADD_RID_DIR_SHIFT                                         1
#define CFG_SPI2SAP_ADD_RID_DIR_MASK                                 0x00000002
#define CFG_SPI2SAP_ADD_RID_DIR_RD(src)               (((src) & 0x00000002)>>1)
#define CFG_SPI2SAP_ADD_RID_DIR_WR(src)          (((u32)(src)<<1) & 0x00000002)
#define CFG_SPI2SAP_ADD_RID_DIR_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields cfg_spi2sap_add_rid_en	 */
#define CFG_SPI2SAP_ADD_RID_EN_WIDTH                                          1
#define CFG_SPI2SAP_ADD_RID_EN_SHIFT                                          0
#define CFG_SPI2SAP_ADD_RID_EN_MASK                                  0x00000001
#define CFG_SPI2SAP_ADD_RID_EN_RD(src)                   (((src) & 0x00000001))
#define CFG_SPI2SAP_ADD_RID_EN_WR(src)              (((u32)(src)) & 0x00000001)
#define CFG_SPI2SAP_ADD_RID_EN_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register SPI2SAP_DelSPI_Reg	*/ 
/*	 Fields cfg_spi2sap_del_rid	 */
#define CFG_SPI2SAP_DEL_RID_WIDTH                                            32
#define CFG_SPI2SAP_DEL_RID_SHIFT                                             0
#define CFG_SPI2SAP_DEL_RID_MASK                                     0xffffffff
#define CFG_SPI2SAP_DEL_RID_RD(src)                      (((src) & 0xffffffff))
#define CFG_SPI2SAP_DEL_RID_WR(src)                 (((u32)(src)) & 0xffffffff)
#define CFG_SPI2SAP_DEL_RID_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register SPI2SAP_DelSPIEn_Reg	*/ 
/*	 Fields cfg_spi2sap_del_rid_port	 */
#define CFG_SPI2SAP_DEL_RID_PORT_WIDTH                                        1
#define CFG_SPI2SAP_DEL_RID_PORT_SHIFT                                        2
#define CFG_SPI2SAP_DEL_RID_PORT_MASK                                0x00000004
#define CFG_SPI2SAP_DEL_RID_PORT_RD(src)              (((src) & 0x00000004)>>2)
#define CFG_SPI2SAP_DEL_RID_PORT_WR(src)         (((u32)(src)<<2) & 0x00000004)
#define CFG_SPI2SAP_DEL_RID_PORT_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields cfg_spi2sap_del_rid_dir	 */
#define CFG_SPI2SAP_DEL_RID_DIR_WIDTH                                         1
#define CFG_SPI2SAP_DEL_RID_DIR_SHIFT                                         1
#define CFG_SPI2SAP_DEL_RID_DIR_MASK                                 0x00000002
#define CFG_SPI2SAP_DEL_RID_DIR_RD(src)               (((src) & 0x00000002)>>1)
#define CFG_SPI2SAP_DEL_RID_DIR_WR(src)          (((u32)(src)<<1) & 0x00000002)
#define CFG_SPI2SAP_DEL_RID_DIR_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields cfg_spi2sap_del_rid_en	 */
#define CFG_SPI2SAP_DEL_RID_EN_WIDTH                                          1
#define CFG_SPI2SAP_DEL_RID_EN_SHIFT                                          0
#define CFG_SPI2SAP_DEL_RID_EN_MASK                                  0x00000001
#define CFG_SPI2SAP_DEL_RID_EN_RD(src)                   (((src) & 0x00000001))
#define CFG_SPI2SAP_DEL_RID_EN_WR(src)              (((u32)(src)) & 0x00000001)
#define CFG_SPI2SAP_DEL_RID_EN_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register SPI2SAP_SearchSPI_Reg	*/ 
/*	 Fields cfg_spi2sap_srch_rid	 */
#define CFG_SPI2SAP_SRCH_RID_WIDTH                                           32
#define CFG_SPI2SAP_SRCH_RID_SHIFT                                            0
#define CFG_SPI2SAP_SRCH_RID_MASK                                    0xffffffff
#define CFG_SPI2SAP_SRCH_RID_RD(src)                     (((src) & 0xffffffff))
#define CFG_SPI2SAP_SRCH_RID_WR(src)                (((u32)(src)) & 0xffffffff)
#define CFG_SPI2SAP_SRCH_RID_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register SPI2SAP_SearchSPIEn_Reg	*/ 
/*	 Fields cfg_spi2sap_srch_rid_port	 */
#define CFG_SPI2SAP_SRCH_RID_PORT_WIDTH                                       1
#define CFG_SPI2SAP_SRCH_RID_PORT_SHIFT                                       2
#define CFG_SPI2SAP_SRCH_RID_PORT_MASK                               0x00000004
#define CFG_SPI2SAP_SRCH_RID_PORT_RD(src)             (((src) & 0x00000004)>>2)
#define CFG_SPI2SAP_SRCH_RID_PORT_WR(src)        (((u32)(src)<<2) & 0x00000004)
#define CFG_SPI2SAP_SRCH_RID_PORT_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields cfg_spi2sap_srch_rid_dir	 */
#define CFG_SPI2SAP_SRCH_RID_DIR_WIDTH                                        1
#define CFG_SPI2SAP_SRCH_RID_DIR_SHIFT                                        1
#define CFG_SPI2SAP_SRCH_RID_DIR_MASK                                0x00000002
#define CFG_SPI2SAP_SRCH_RID_DIR_RD(src)              (((src) & 0x00000002)>>1)
#define CFG_SPI2SAP_SRCH_RID_DIR_WR(src)         (((u32)(src)<<1) & 0x00000002)
#define CFG_SPI2SAP_SRCH_RID_DIR_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields cfg_spi2sap_srch_rid_en	 */
#define CFG_SPI2SAP_SRCH_RID_EN_WIDTH                                         1
#define CFG_SPI2SAP_SRCH_RID_EN_SHIFT                                         0
#define CFG_SPI2SAP_SRCH_RID_EN_MASK                                 0x00000001
#define CFG_SPI2SAP_SRCH_RID_EN_RD(src)                  (((src) & 0x00000001))
#define CFG_SPI2SAP_SRCH_RID_EN_WR(src)             (((u32)(src)) & 0x00000001)
#define CFG_SPI2SAP_SRCH_RID_EN_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register SPI2SAP_Cfg0_Reg	*/ 
/*	 Fields cfg_avlmgr_maxsteps_thresh	 */
#define CFG_AVLMGR_MAXSTEPS_THRESH_WIDTH                                     12
#define CFG_AVLMGR_MAXSTEPS_THRESH_SHIFT                                      8
#define CFG_AVLMGR_MAXSTEPS_THRESH_MASK                              0x000fff00
#define CFG_AVLMGR_MAXSTEPS_THRESH_RD(src)            (((src) & 0x000fff00)>>8)
#define CFG_AVLMGR_MAXSTEPS_THRESH_WR(src)       (((u32)(src)<<8) & 0x000fff00)
#define CFG_AVLMGR_MAXSTEPS_THRESH_SET(dst,src) \
                       (((dst) & ~0x000fff00) | (((u32)(src)<<8) & 0x000fff00))
/*	 Fields cfg_avlmgr_srchdb_depth	 */
#define CFG_AVLMGR_SRCHDB_DEPTH_WIDTH                                         8
#define CFG_AVLMGR_SRCHDB_DEPTH_SHIFT                                         0
#define CFG_AVLMGR_SRCHDB_DEPTH_MASK                                 0x000000ff
#define CFG_AVLMGR_SRCHDB_DEPTH_RD(src)                  (((src) & 0x000000ff))
#define CFG_AVLMGR_SRCHDB_DEPTH_WR(src)             (((u32)(src)) & 0x000000ff)
#define CFG_AVLMGR_SRCHDB_DEPTH_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register SPI2SAP_Dbg0_Reg	*/ 
/*	 Fields cfg_avlmgr_dbg_srchdb_wr_en	 */
#define CFG_AVLMGR_DBG_SRCHDB_WR_EN_WIDTH                                     1
#define CFG_AVLMGR_DBG_SRCHDB_WR_EN_SHIFT                                     9
#define CFG_AVLMGR_DBG_SRCHDB_WR_EN_MASK                             0x00000200
#define CFG_AVLMGR_DBG_SRCHDB_WR_EN_RD(src)           (((src) & 0x00000200)>>9)
#define CFG_AVLMGR_DBG_SRCHDB_WR_EN_WR(src)      (((u32)(src)<<9) & 0x00000200)
#define CFG_AVLMGR_DBG_SRCHDB_WR_EN_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields cfg_avlmgr_dbg_en	 */
#define CFG_AVLMGR_DBG_EN_WIDTH                                               1
#define CFG_AVLMGR_DBG_EN_SHIFT                                               8
#define CFG_AVLMGR_DBG_EN_MASK                                       0x00000100
#define CFG_AVLMGR_DBG_EN_RD(src)                     (((src) & 0x00000100)>>8)
#define CFG_AVLMGR_DBG_EN_WR(src)                (((u32)(src)<<8) & 0x00000100)
#define CFG_AVLMGR_DBG_EN_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*	 Fields cfg_avlmgr_dbg_srchdb_addr	 */
#define CFG_AVLMGR_DBG_SRCHDB_ADDR_WIDTH                                      7
#define CFG_AVLMGR_DBG_SRCHDB_ADDR_SHIFT                                      0
#define CFG_AVLMGR_DBG_SRCHDB_ADDR_MASK                              0x0000007f
#define CFG_AVLMGR_DBG_SRCHDB_ADDR_RD(src)               (((src) & 0x0000007f))
#define CFG_AVLMGR_DBG_SRCHDB_ADDR_WR(src)          (((u32)(src)) & 0x0000007f)
#define CFG_AVLMGR_DBG_SRCHDB_ADDR_SET(dst,src) \
                          (((dst) & ~0x0000007f) | (((u32)(src)) & 0x0000007f))

/*	Register SPI2SAP_Dbg1_Reg	*/ 
/*	 Fields cfg_avlmgr_dbg_srchdb_wdata_l	 */
#define CFG_AVLMGR_DBG_SRCHDB_WDATA_L_WIDTH                                  19
#define CFG_AVLMGR_DBG_SRCHDB_WDATA_L_SHIFT                                   0
#define CFG_AVLMGR_DBG_SRCHDB_WDATA_L_MASK                           0x0007ffff
#define CFG_AVLMGR_DBG_SRCHDB_WDATA_L_RD(src)            (((src) & 0x0007ffff))
#define CFG_AVLMGR_DBG_SRCHDB_WDATA_L_WR(src)       (((u32)(src)) & 0x0007ffff)
#define CFG_AVLMGR_DBG_SRCHDB_WDATA_L_SET(dst,src) \
                          (((dst) & ~0x0007ffff) | (((u32)(src)) & 0x0007ffff))

/*	Register SPI2SAP_Dbg2_Reg	*/ 
/*	 Fields cfg_avlmgr_dbg_srchdb_wdata_h	 */
#define CFG_AVLMGR_DBG_SRCHDB_WDATA_H_WIDTH                                  32
#define CFG_AVLMGR_DBG_SRCHDB_WDATA_H_SHIFT                                   0
#define CFG_AVLMGR_DBG_SRCHDB_WDATA_H_MASK                           0xffffffff
#define CFG_AVLMGR_DBG_SRCHDB_WDATA_H_RD(src)            (((src) & 0xffffffff))
#define CFG_AVLMGR_DBG_SRCHDB_WDATA_H_WR(src)       (((u32)(src)) & 0xffffffff)
#define CFG_AVLMGR_DBG_SRCHDB_WDATA_H_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register SPI2SAP_IntReg	*/ 
/*	 Fields sts_avlmgr_fatal_wrselfpointerr	 */
#define STS_AVLMGR_FATAL_WRSELFPOINTERR_WIDTH                                 1
#define STS_AVLMGR_FATAL_WRSELFPOINTERR_SHIFT                                10
#define STS_AVLMGR_FATAL_WRSELFPOINTERR_MASK                         0x00000400
#define STS_AVLMGR_FATAL_WRSELFPOINTERR_RD(src)      (((src) & 0x00000400)>>10)
#define STS_AVLMGR_FATAL_WRSELFPOINTERR_WR(src) \
                                                (((u32)(src)<<10) & 0x00000400)
#define STS_AVLMGR_FATAL_WRSELFPOINTERR_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields sts_avlmgr_fatal_rdselfpointerr	 */
#define STS_AVLMGR_FATAL_RDSELFPOINTERR_WIDTH                                 1
#define STS_AVLMGR_FATAL_RDSELFPOINTERR_SHIFT                                 9
#define STS_AVLMGR_FATAL_RDSELFPOINTERR_MASK                         0x00000200
#define STS_AVLMGR_FATAL_RDSELFPOINTERR_RD(src)       (((src) & 0x00000200)>>9)
#define STS_AVLMGR_FATAL_RDSELFPOINTERR_WR(src)  (((u32)(src)<<9) & 0x00000200)
#define STS_AVLMGR_FATAL_RDSELFPOINTERR_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields sts_avlmgr_fatal_wrbalerr	 */
#define STS_AVLMGR_FATAL_WRBALERR_WIDTH                                       1
#define STS_AVLMGR_FATAL_WRBALERR_SHIFT                                       8
#define STS_AVLMGR_FATAL_WRBALERR_MASK                               0x00000100
#define STS_AVLMGR_FATAL_WRBALERR_RD(src)             (((src) & 0x00000100)>>8)
#define STS_AVLMGR_FATAL_WRBALERR_WR(src)        (((u32)(src)<<8) & 0x00000100)
#define STS_AVLMGR_FATAL_WRBALERR_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*	 Fields sts_avlmgr_fatal_rdbalerr	 */
#define STS_AVLMGR_FATAL_RDBALERR_WIDTH                                       1
#define STS_AVLMGR_FATAL_RDBALERR_SHIFT                                       7
#define STS_AVLMGR_FATAL_RDBALERR_MASK                               0x00000080
#define STS_AVLMGR_FATAL_RDBALERR_RD(src)             (((src) & 0x00000080)>>7)
#define STS_AVLMGR_FATAL_RDBALERR_WR(src)        (((u32)(src)<<7) & 0x00000080)
#define STS_AVLMGR_FATAL_RDBALERR_SET(dst,src) \
                       (((dst) & ~0x00000080) | (((u32)(src)<<7) & 0x00000080))
/*	 Fields sts_avlmgr_fatal_maxstepserr	 */
#define STS_AVLMGR_FATAL_MAXSTEPSERR_WIDTH                                    1
#define STS_AVLMGR_FATAL_MAXSTEPSERR_SHIFT                                    6
#define STS_AVLMGR_FATAL_MAXSTEPSERR_MASK                            0x00000040
#define STS_AVLMGR_FATAL_MAXSTEPSERR_RD(src)          (((src) & 0x00000040)>>6)
#define STS_AVLMGR_FATAL_MAXSTEPSERR_WR(src)     (((u32)(src)<<6) & 0x00000040)
#define STS_AVLMGR_FATAL_MAXSTEPSERR_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*	 Fields sts_avlmgr_fatal_rderr_ram	 */
#define STS_AVLMGR_FATAL_RDERR_RAM_WIDTH                                      1
#define STS_AVLMGR_FATAL_RDERR_RAM_SHIFT                                      5
#define STS_AVLMGR_FATAL_RDERR_RAM_MASK                              0x00000020
#define STS_AVLMGR_FATAL_RDERR_RAM_RD(src)            (((src) & 0x00000020)>>5)
#define STS_AVLMGR_FATAL_RDERR_RAM_WR(src)       (((u32)(src)<<5) & 0x00000020)
#define STS_AVLMGR_FATAL_RDERR_RAM_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields sts_avlmgr_rderr_ram	 */
#define STS_AVLMGR_RDERR_RAM_WIDTH                                            1
#define STS_AVLMGR_RDERR_RAM_SHIFT                                            4
#define STS_AVLMGR_RDERR_RAM_MASK                                    0x00000010
#define STS_AVLMGR_RDERR_RAM_RD(src)                  (((src) & 0x00000010)>>4)
#define STS_AVLMGR_RDERR_RAM_WR(src)             (((u32)(src)<<4) & 0x00000010)
#define STS_AVLMGR_RDERR_RAM_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields sts_avlmgr_adderr_srchdb_full	 */
#define STS_AVLMGR_ADDERR_SRCHDB_FULL_WIDTH                                   1
#define STS_AVLMGR_ADDERR_SRCHDB_FULL_SHIFT                                   3
#define STS_AVLMGR_ADDERR_SRCHDB_FULL_MASK                           0x00000008
#define STS_AVLMGR_ADDERR_SRCHDB_FULL_RD(src)         (((src) & 0x00000008)>>3)
#define STS_AVLMGR_ADDERR_SRCHDB_FULL_WR(src)    (((u32)(src)<<3) & 0x00000008)
#define STS_AVLMGR_ADDERR_SRCHDB_FULL_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields sts_avlmgr_adderr_ridduplicate	 */
#define STS_AVLMGR_ADDERR_RIDDUPLICATE_WIDTH                                  1
#define STS_AVLMGR_ADDERR_RIDDUPLICATE_SHIFT                                  2
#define STS_AVLMGR_ADDERR_RIDDUPLICATE_MASK                          0x00000004
#define STS_AVLMGR_ADDERR_RIDDUPLICATE_RD(src)        (((src) & 0x00000004)>>2)
#define STS_AVLMGR_ADDERR_RIDDUPLICATE_WR(src)   (((u32)(src)<<2) & 0x00000004)
#define STS_AVLMGR_ADDERR_RIDDUPLICATE_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields sts_avlmgr_delerr_srchdb_empty	 */
#define STS_AVLMGR_DELERR_SRCHDB_EMPTY_WIDTH                                  1
#define STS_AVLMGR_DELERR_SRCHDB_EMPTY_SHIFT                                  1
#define STS_AVLMGR_DELERR_SRCHDB_EMPTY_MASK                          0x00000002
#define STS_AVLMGR_DELERR_SRCHDB_EMPTY_RD(src)        (((src) & 0x00000002)>>1)
#define STS_AVLMGR_DELERR_SRCHDB_EMPTY_WR(src)   (((u32)(src)<<1) & 0x00000002)
#define STS_AVLMGR_DELERR_SRCHDB_EMPTY_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields sts_avlmgr_delerr_ridnotfound	 */
#define STS_AVLMGR_DELERR_RIDNOTFOUND_WIDTH                                   1
#define STS_AVLMGR_DELERR_RIDNOTFOUND_SHIFT                                   0
#define STS_AVLMGR_DELERR_RIDNOTFOUND_MASK                           0x00000001
#define STS_AVLMGR_DELERR_RIDNOTFOUND_RD(src)            (((src) & 0x00000001))
#define STS_AVLMGR_DELERR_RIDNOTFOUND_WR(src)       (((u32)(src)) & 0x00000001)
#define STS_AVLMGR_DELERR_RIDNOTFOUND_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register SPI2SAP_IntRegMask	*/
/*    Mask Register Fields sts_avlmgr_fatal_wrselfpointerrMask    */
#define STS_AVLMGR_FATAL_WRSELFPOINTERRMASK_WIDTH                             1
#define STS_AVLMGR_FATAL_WRSELFPOINTERRMASK_SHIFT                            10
#define STS_AVLMGR_FATAL_WRSELFPOINTERRMASK_MASK                     0x00000400
#define STS_AVLMGR_FATAL_WRSELFPOINTERRMASK_RD(src)  (((src) & 0x00000400)>>10)
#define STS_AVLMGR_FATAL_WRSELFPOINTERRMASK_WR(src) \
                                                (((u32)(src)<<10) & 0x00000400)
#define STS_AVLMGR_FATAL_WRSELFPOINTERRMASK_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*    Mask Register Fields sts_avlmgr_fatal_rdselfpointerrMask    */
#define STS_AVLMGR_FATAL_RDSELFPOINTERRMASK_WIDTH                             1
#define STS_AVLMGR_FATAL_RDSELFPOINTERRMASK_SHIFT                             9
#define STS_AVLMGR_FATAL_RDSELFPOINTERRMASK_MASK                     0x00000200
#define STS_AVLMGR_FATAL_RDSELFPOINTERRMASK_RD(src)   (((src) & 0x00000200)>>9)
#define STS_AVLMGR_FATAL_RDSELFPOINTERRMASK_WR(src) \
                                                 (((u32)(src)<<9) & 0x00000200)
#define STS_AVLMGR_FATAL_RDSELFPOINTERRMASK_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*    Mask Register Fields sts_avlmgr_fatal_wrbalerrMask    */
#define STS_AVLMGR_FATAL_WRBALERRMASK_WIDTH                                   1
#define STS_AVLMGR_FATAL_WRBALERRMASK_SHIFT                                   8
#define STS_AVLMGR_FATAL_WRBALERRMASK_MASK                           0x00000100
#define STS_AVLMGR_FATAL_WRBALERRMASK_RD(src)         (((src) & 0x00000100)>>8)
#define STS_AVLMGR_FATAL_WRBALERRMASK_WR(src)    (((u32)(src)<<8) & 0x00000100)
#define STS_AVLMGR_FATAL_WRBALERRMASK_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*    Mask Register Fields sts_avlmgr_fatal_rdbalerrMask    */
#define STS_AVLMGR_FATAL_RDBALERRMASK_WIDTH                                   1
#define STS_AVLMGR_FATAL_RDBALERRMASK_SHIFT                                   7
#define STS_AVLMGR_FATAL_RDBALERRMASK_MASK                           0x00000080
#define STS_AVLMGR_FATAL_RDBALERRMASK_RD(src)         (((src) & 0x00000080)>>7)
#define STS_AVLMGR_FATAL_RDBALERRMASK_WR(src)    (((u32)(src)<<7) & 0x00000080)
#define STS_AVLMGR_FATAL_RDBALERRMASK_SET(dst,src) \
                       (((dst) & ~0x00000080) | (((u32)(src)<<7) & 0x00000080))
/*    Mask Register Fields sts_avlmgr_fatal_maxstepserrMask    */
#define STS_AVLMGR_FATAL_MAXSTEPSERRMASK_WIDTH                                1
#define STS_AVLMGR_FATAL_MAXSTEPSERRMASK_SHIFT                                6
#define STS_AVLMGR_FATAL_MAXSTEPSERRMASK_MASK                        0x00000040
#define STS_AVLMGR_FATAL_MAXSTEPSERRMASK_RD(src)      (((src) & 0x00000040)>>6)
#define STS_AVLMGR_FATAL_MAXSTEPSERRMASK_WR(src) \
                                                 (((u32)(src)<<6) & 0x00000040)
#define STS_AVLMGR_FATAL_MAXSTEPSERRMASK_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*    Mask Register Fields sts_avlmgr_fatal_rderr_ramMask    */
#define STS_AVLMGR_FATAL_RDERR_RAMMASK_WIDTH                                  1
#define STS_AVLMGR_FATAL_RDERR_RAMMASK_SHIFT                                  5
#define STS_AVLMGR_FATAL_RDERR_RAMMASK_MASK                          0x00000020
#define STS_AVLMGR_FATAL_RDERR_RAMMASK_RD(src)        (((src) & 0x00000020)>>5)
#define STS_AVLMGR_FATAL_RDERR_RAMMASK_WR(src)   (((u32)(src)<<5) & 0x00000020)
#define STS_AVLMGR_FATAL_RDERR_RAMMASK_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*    Mask Register Fields sts_avlmgr_rderr_ramMask    */
#define STS_AVLMGR_RDERR_RAMMASK_WIDTH                                        1
#define STS_AVLMGR_RDERR_RAMMASK_SHIFT                                        4
#define STS_AVLMGR_RDERR_RAMMASK_MASK                                0x00000010
#define STS_AVLMGR_RDERR_RAMMASK_RD(src)              (((src) & 0x00000010)>>4)
#define STS_AVLMGR_RDERR_RAMMASK_WR(src)         (((u32)(src)<<4) & 0x00000010)
#define STS_AVLMGR_RDERR_RAMMASK_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*    Mask Register Fields sts_avlmgr_adderr_srchdb_fullMask    */
#define STS_AVLMGR_ADDERR_SRCHDB_FULLMASK_WIDTH                               1
#define STS_AVLMGR_ADDERR_SRCHDB_FULLMASK_SHIFT                               3
#define STS_AVLMGR_ADDERR_SRCHDB_FULLMASK_MASK                       0x00000008
#define STS_AVLMGR_ADDERR_SRCHDB_FULLMASK_RD(src)     (((src) & 0x00000008)>>3)
#define STS_AVLMGR_ADDERR_SRCHDB_FULLMASK_WR(src) \
                                                 (((u32)(src)<<3) & 0x00000008)
#define STS_AVLMGR_ADDERR_SRCHDB_FULLMASK_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*    Mask Register Fields sts_avlmgr_adderr_ridduplicateMask    */
#define STS_AVLMGR_ADDERR_RIDDUPLICATEMASK_WIDTH                              1
#define STS_AVLMGR_ADDERR_RIDDUPLICATEMASK_SHIFT                              2
#define STS_AVLMGR_ADDERR_RIDDUPLICATEMASK_MASK                      0x00000004
#define STS_AVLMGR_ADDERR_RIDDUPLICATEMASK_RD(src)    (((src) & 0x00000004)>>2)
#define STS_AVLMGR_ADDERR_RIDDUPLICATEMASK_WR(src) \
                                                 (((u32)(src)<<2) & 0x00000004)
#define STS_AVLMGR_ADDERR_RIDDUPLICATEMASK_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*    Mask Register Fields sts_avlmgr_delerr_srchdb_emptyMask    */
#define STS_AVLMGR_DELERR_SRCHDB_EMPTYMASK_WIDTH                              1
#define STS_AVLMGR_DELERR_SRCHDB_EMPTYMASK_SHIFT                              1
#define STS_AVLMGR_DELERR_SRCHDB_EMPTYMASK_MASK                      0x00000002
#define STS_AVLMGR_DELERR_SRCHDB_EMPTYMASK_RD(src)    (((src) & 0x00000002)>>1)
#define STS_AVLMGR_DELERR_SRCHDB_EMPTYMASK_WR(src) \
                                                 (((u32)(src)<<1) & 0x00000002)
#define STS_AVLMGR_DELERR_SRCHDB_EMPTYMASK_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*    Mask Register Fields sts_avlmgr_delerr_ridnotfoundMask    */
#define STS_AVLMGR_DELERR_RIDNOTFOUNDMASK_WIDTH                               1
#define STS_AVLMGR_DELERR_RIDNOTFOUNDMASK_SHIFT                               0
#define STS_AVLMGR_DELERR_RIDNOTFOUNDMASK_MASK                       0x00000001
#define STS_AVLMGR_DELERR_RIDNOTFOUNDMASK_RD(src)        (((src) & 0x00000001))
#define STS_AVLMGR_DELERR_RIDNOTFOUNDMASK_WR(src)   (((u32)(src)) & 0x00000001)
#define STS_AVLMGR_DELERR_RIDNOTFOUNDMASK_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register SPI2SAP_StsReg0	*/ 
/*	 Fields sts_avlmgr_rootaddr	 */
#define STS_AVLMGR_ROOTADDR0_WIDTH                                            7
#define STS_AVLMGR_ROOTADDR0_SHIFT                                            8
#define STS_AVLMGR_ROOTADDR0_MASK                                    0x00007f00
#define STS_AVLMGR_ROOTADDR0_RD(src)                  (((src) & 0x00007f00)>>8)
#define STS_AVLMGR_ROOTADDR0_SET(dst,src) \
                       (((dst) & ~0x00007f00) | (((u32)(src)<<8) & 0x00007f00))
/*	 Fields sts_avlmgr_rid_cnt	 */
#define STS_AVLMGR_RID_CNT0_WIDTH                                             8
#define STS_AVLMGR_RID_CNT0_SHIFT                                             0
#define STS_AVLMGR_RID_CNT0_MASK                                     0x000000ff
#define STS_AVLMGR_RID_CNT0_RD(src)                      (((src) & 0x000000ff))
#define STS_AVLMGR_RID_CNT0_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register SPI2SAP_StsReg1	*/ 
/*	 Fields sts_avlmgr_dbg_srchdb_rdata_l	 */
#define STS_AVLMGR_DBG_SRCHDB_RDATA_L1_WIDTH                                 19
#define STS_AVLMGR_DBG_SRCHDB_RDATA_L1_SHIFT                                  0
#define STS_AVLMGR_DBG_SRCHDB_RDATA_L1_MASK                          0x0007ffff
#define STS_AVLMGR_DBG_SRCHDB_RDATA_L1_RD(src)           (((src) & 0x0007ffff))
#define STS_AVLMGR_DBG_SRCHDB_RDATA_L1_SET(dst,src) \
                          (((dst) & ~0x0007ffff) | (((u32)(src)) & 0x0007ffff))

/*	Register SPI2SAP_StsReg2	*/ 
/*	 Fields sts_avlmgr_dbg_srchdb_rdata_h	 */
#define STS_AVLMGR_DBG_SRCHDB_RDATA_H2_WIDTH                                 32
#define STS_AVLMGR_DBG_SRCHDB_RDATA_H2_SHIFT                                  0
#define STS_AVLMGR_DBG_SRCHDB_RDATA_H2_MASK                          0xffffffff
#define STS_AVLMGR_DBG_SRCHDB_RDATA_H2_RD(src)           (((src) & 0xffffffff))
#define STS_AVLMGR_DBG_SRCHDB_RDATA_H2_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register SPI2SAP_StsReg3	*/ 
/*	 Fields sts_avlmgr_deladdr	 */
#define STS_AVLMGR_DELADDR3_WIDTH                                             7
#define STS_AVLMGR_DELADDR3_SHIFT                                            16
#define STS_AVLMGR_DELADDR3_MASK                                     0x007f0000
#define STS_AVLMGR_DELADDR3_RD(src)                  (((src) & 0x007f0000)>>16)
#define STS_AVLMGR_DELADDR3_SET(dst,src) \
                      (((dst) & ~0x007f0000) | (((u32)(src)<<16) & 0x007f0000))
/*	 Fields sts_avlmgr_addaddr	 */
#define STS_AVLMGR_ADDADDR3_WIDTH                                             7
#define STS_AVLMGR_ADDADDR3_SHIFT                                             8
#define STS_AVLMGR_ADDADDR3_MASK                                     0x00007f00
#define STS_AVLMGR_ADDADDR3_RD(src)                   (((src) & 0x00007f00)>>8)
#define STS_AVLMGR_ADDADDR3_SET(dst,src) \
                       (((dst) & ~0x00007f00) | (((u32)(src)<<8) & 0x00007f00))
/*	 Fields sts_avlmgr_srchaddr	 */
#define STS_AVLMGR_SRCHADDR3_WIDTH                                            7
#define STS_AVLMGR_SRCHADDR3_SHIFT                                            0
#define STS_AVLMGR_SRCHADDR3_MASK                                    0x0000007f
#define STS_AVLMGR_SRCHADDR3_RD(src)                     (((src) & 0x0000007f))
#define STS_AVLMGR_SRCHADDR3_SET(dst,src) \
                          (((dst) & ~0x0000007f) | (((u32)(src)) & 0x0000007f))

/*	Register SPI2SAP_FIFO_EmptySts0	*/ 
/*	 Fields macif_fifo_empty_rxprt1	 */
#define MACIF_FIFO_EMPTY_RXPRT10_WIDTH                                        1
#define MACIF_FIFO_EMPTY_RXPRT10_SHIFT                                        1
#define MACIF_FIFO_EMPTY_RXPRT10_MASK                                0x00000002
#define MACIF_FIFO_EMPTY_RXPRT10_RD(src)              (((src) & 0x00000002)>>1)
#define MACIF_FIFO_EMPTY_RXPRT10_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields macif_fifo_empty_rxprt0	 */
#define MACIF_FIFO_EMPTY_RXPRT00_WIDTH                                        1
#define MACIF_FIFO_EMPTY_RXPRT00_SHIFT                                        0
#define MACIF_FIFO_EMPTY_RXPRT00_MASK                                0x00000001
#define MACIF_FIFO_EMPTY_RXPRT00_RD(src)                 (((src) & 0x00000001))
#define MACIF_FIFO_EMPTY_RXPRT00_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register SPI2SAP_Int_Reg0	*/ 
/*	 Fields macif_fifo_overfl_intr_rxprt1	 */
#define MACIF_FIFO_OVERFL_INTR_RXPRT10_WIDTH                                  1
#define MACIF_FIFO_OVERFL_INTR_RXPRT10_SHIFT                                 17
#define MACIF_FIFO_OVERFL_INTR_RXPRT10_MASK                          0x00020000
#define MACIF_FIFO_OVERFL_INTR_RXPRT10_RD(src)       (((src) & 0x00020000)>>17)
#define MACIF_FIFO_OVERFL_INTR_RXPRT10_WR(src)  (((u32)(src)<<17) & 0x00020000)
#define MACIF_FIFO_OVERFL_INTR_RXPRT10_SET(dst,src) \
                      (((dst) & ~0x00020000) | (((u32)(src)<<17) & 0x00020000))
/*	 Fields macif_fifo_underfl_intr_rxprt1	 */
#define MACIF_FIFO_UNDERFL_INTR_RXPRT10_WIDTH                                 1
#define MACIF_FIFO_UNDERFL_INTR_RXPRT10_SHIFT                                16
#define MACIF_FIFO_UNDERFL_INTR_RXPRT10_MASK                         0x00010000
#define MACIF_FIFO_UNDERFL_INTR_RXPRT10_RD(src)      (((src) & 0x00010000)>>16)
#define MACIF_FIFO_UNDERFL_INTR_RXPRT10_WR(src) \
                                                (((u32)(src)<<16) & 0x00010000)
#define MACIF_FIFO_UNDERFL_INTR_RXPRT10_SET(dst,src) \
                      (((dst) & ~0x00010000) | (((u32)(src)<<16) & 0x00010000))
/*	 Fields macif_fifo_overfl_intr_rxprt0	 */
#define MACIF_FIFO_OVERFL_INTR_RXPRT00_WIDTH                                  1
#define MACIF_FIFO_OVERFL_INTR_RXPRT00_SHIFT                                  1
#define MACIF_FIFO_OVERFL_INTR_RXPRT00_MASK                          0x00000002
#define MACIF_FIFO_OVERFL_INTR_RXPRT00_RD(src)        (((src) & 0x00000002)>>1)
#define MACIF_FIFO_OVERFL_INTR_RXPRT00_WR(src)   (((u32)(src)<<1) & 0x00000002)
#define MACIF_FIFO_OVERFL_INTR_RXPRT00_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields macif_fifo_underfl_intr_rxprt0	 */
#define MACIF_FIFO_UNDERFL_INTR_RXPRT00_WIDTH                                 1
#define MACIF_FIFO_UNDERFL_INTR_RXPRT00_SHIFT                                 0
#define MACIF_FIFO_UNDERFL_INTR_RXPRT00_MASK                         0x00000001
#define MACIF_FIFO_UNDERFL_INTR_RXPRT00_RD(src)          (((src) & 0x00000001))
#define MACIF_FIFO_UNDERFL_INTR_RXPRT00_WR(src)     (((u32)(src)) & 0x00000001)
#define MACIF_FIFO_UNDERFL_INTR_RXPRT00_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register SPI2SAP_Int_Reg0Mask	*/
/*    Mask Register Fields macif_fifo_overfl_intr_rxprt1Mask    */
#define MACIF_FIFO_OVERFL_INTR_RXPRT1MASK_WIDTH                               1
#define MACIF_FIFO_OVERFL_INTR_RXPRT1MASK_SHIFT                              17
#define MACIF_FIFO_OVERFL_INTR_RXPRT1MASK_MASK                       0x00020000
#define MACIF_FIFO_OVERFL_INTR_RXPRT1MASK_RD(src)    (((src) & 0x00020000)>>17)
#define MACIF_FIFO_OVERFL_INTR_RXPRT1MASK_WR(src) \
                                                (((u32)(src)<<17) & 0x00020000)
#define MACIF_FIFO_OVERFL_INTR_RXPRT1MASK_SET(dst,src) \
                      (((dst) & ~0x00020000) | (((u32)(src)<<17) & 0x00020000))
/*    Mask Register Fields macif_fifo_underfl_intr_rxprt1Mask    */
#define MACIF_FIFO_UNDERFL_INTR_RXPRT1MASK_WIDTH                              1
#define MACIF_FIFO_UNDERFL_INTR_RXPRT1MASK_SHIFT                             16
#define MACIF_FIFO_UNDERFL_INTR_RXPRT1MASK_MASK                      0x00010000
#define MACIF_FIFO_UNDERFL_INTR_RXPRT1MASK_RD(src)   (((src) & 0x00010000)>>16)
#define MACIF_FIFO_UNDERFL_INTR_RXPRT1MASK_WR(src) \
                                                (((u32)(src)<<16) & 0x00010000)
#define MACIF_FIFO_UNDERFL_INTR_RXPRT1MASK_SET(dst,src) \
                      (((dst) & ~0x00010000) | (((u32)(src)<<16) & 0x00010000))
/*    Mask Register Fields macif_fifo_overfl_intr_rxprt0Mask    */
#define MACIF_FIFO_OVERFL_INTR_RXPRT0MASK_WIDTH                               1
#define MACIF_FIFO_OVERFL_INTR_RXPRT0MASK_SHIFT                               1
#define MACIF_FIFO_OVERFL_INTR_RXPRT0MASK_MASK                       0x00000002
#define MACIF_FIFO_OVERFL_INTR_RXPRT0MASK_RD(src)     (((src) & 0x00000002)>>1)
#define MACIF_FIFO_OVERFL_INTR_RXPRT0MASK_WR(src) \
                                                 (((u32)(src)<<1) & 0x00000002)
#define MACIF_FIFO_OVERFL_INTR_RXPRT0MASK_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*    Mask Register Fields macif_fifo_underfl_intr_rxprt0Mask    */
#define MACIF_FIFO_UNDERFL_INTR_RXPRT0MASK_WIDTH                              1
#define MACIF_FIFO_UNDERFL_INTR_RXPRT0MASK_SHIFT                              0
#define MACIF_FIFO_UNDERFL_INTR_RXPRT0MASK_MASK                      0x00000001
#define MACIF_FIFO_UNDERFL_INTR_RXPRT0MASK_RD(src)       (((src) & 0x00000001))
#define MACIF_FIFO_UNDERFL_INTR_RXPRT0MASK_WR(src)  (((u32)(src)) & 0x00000001)
#define MACIF_FIFO_UNDERFL_INTR_RXPRT0MASK_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register ENET_SAD_AddrCmd_Reg	*/ 
/*	 Fields cfg_enet_csr_sad_req	 */
#define CFG_ENET_CSR_SAD_REQ_WIDTH                                            1
#define CFG_ENET_CSR_SAD_REQ_SHIFT                                           17
#define CFG_ENET_CSR_SAD_REQ_MASK                                    0x00020000
#define CFG_ENET_CSR_SAD_REQ_RD(src)                 (((src) & 0x00020000)>>17)
#define CFG_ENET_CSR_SAD_REQ_WR(src)            (((u32)(src)<<17) & 0x00020000)
#define CFG_ENET_CSR_SAD_REQ_SET(dst,src) \
                      (((dst) & ~0x00020000) | (((u32)(src)<<17) & 0x00020000))
/*	 Fields cfg_enet_csr_sad_rdwr	 */
#define CFG_ENET_CSR_SAD_RDWR_WIDTH                                           1
#define CFG_ENET_CSR_SAD_RDWR_SHIFT                                          16
#define CFG_ENET_CSR_SAD_RDWR_MASK                                   0x00010000
#define CFG_ENET_CSR_SAD_RDWR_RD(src)                (((src) & 0x00010000)>>16)
#define CFG_ENET_CSR_SAD_RDWR_WR(src)           (((u32)(src)<<16) & 0x00010000)
#define CFG_ENET_CSR_SAD_RDWR_SET(dst,src) \
                      (((dst) & ~0x00010000) | (((u32)(src)<<16) & 0x00010000))
/*	 Fields cfg_enet_csr_sad_addr	 */
#define CFG_ENET_CSR_SAD_ADDR_WIDTH                                          16
#define CFG_ENET_CSR_SAD_ADDR_SHIFT                                           0
#define CFG_ENET_CSR_SAD_ADDR_MASK                                   0x0000ffff
#define CFG_ENET_CSR_SAD_ADDR_RD(src)                    (((src) & 0x0000ffff))
#define CFG_ENET_CSR_SAD_ADDR_WR(src)               (((u32)(src)) & 0x0000ffff)
#define CFG_ENET_CSR_SAD_ADDR_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register ENET_SAD_Wdata0_Reg	*/ 
/*	 Fields cfg_enet_csr_sad_wdata0	 */
#define CFG_ENET_CSR_SAD_WDATA0_WIDTH                                        32
#define CFG_ENET_CSR_SAD_WDATA0_SHIFT                                         0
#define CFG_ENET_CSR_SAD_WDATA0_MASK                                 0xffffffff
#define CFG_ENET_CSR_SAD_WDATA0_RD(src)                  (((src) & 0xffffffff))
#define CFG_ENET_CSR_SAD_WDATA0_WR(src)             (((u32)(src)) & 0xffffffff)
#define CFG_ENET_CSR_SAD_WDATA0_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register ENET_SAD_Wdata1_Reg	*/ 
/*	 Fields cfg_enet_csr_sad_wdata1	 */
#define CFG_ENET_CSR_SAD_WDATA1_WIDTH                                        32
#define CFG_ENET_CSR_SAD_WDATA1_SHIFT                                         0
#define CFG_ENET_CSR_SAD_WDATA1_MASK                                 0xffffffff
#define CFG_ENET_CSR_SAD_WDATA1_RD(src)                  (((src) & 0xffffffff))
#define CFG_ENET_CSR_SAD_WDATA1_WR(src)             (((u32)(src)) & 0xffffffff)
#define CFG_ENET_CSR_SAD_WDATA1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register ENET_SAD_Wdata2_Reg	*/ 
/*	 Fields cfg_enet_csr_sad_wdata2	 */
#define CFG_ENET_CSR_SAD_WDATA2_WIDTH                                        32
#define CFG_ENET_CSR_SAD_WDATA2_SHIFT                                         0
#define CFG_ENET_CSR_SAD_WDATA2_MASK                                 0xffffffff
#define CFG_ENET_CSR_SAD_WDATA2_RD(src)                  (((src) & 0xffffffff))
#define CFG_ENET_CSR_SAD_WDATA2_WR(src)             (((u32)(src)) & 0xffffffff)
#define CFG_ENET_CSR_SAD_WDATA2_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register ENET_SAD_Wdata3_Reg	*/ 
/*	 Fields cfg_enet_csr_sad_wdata3	 */
#define CFG_ENET_CSR_SAD_WDATA3_WIDTH                                        32
#define CFG_ENET_CSR_SAD_WDATA3_SHIFT                                         0
#define CFG_ENET_CSR_SAD_WDATA3_MASK                                 0xffffffff
#define CFG_ENET_CSR_SAD_WDATA3_RD(src)                  (((src) & 0xffffffff))
#define CFG_ENET_CSR_SAD_WDATA3_WR(src)             (((u32)(src)) & 0xffffffff)
#define CFG_ENET_CSR_SAD_WDATA3_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register ENET_SAD_Rdata0_Reg	*/ 
/*	 Fields sts_enet_csr_sad_rdata0	 */
#define STS_ENET_CSR_SAD_RDATA0_WIDTH                                        32
#define STS_ENET_CSR_SAD_RDATA0_SHIFT                                         0
#define STS_ENET_CSR_SAD_RDATA0_MASK                                 0xffffffff
#define STS_ENET_CSR_SAD_RDATA0_RD(src)                  (((src) & 0xffffffff))
#define STS_ENET_CSR_SAD_RDATA0_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register ENET_SAD_Rdata1_Reg	*/ 
/*	 Fields sts_enet_csr_sad_rdata1	 */
#define STS_ENET_CSR_SAD_RDATA1_WIDTH                                        32
#define STS_ENET_CSR_SAD_RDATA1_SHIFT                                         0
#define STS_ENET_CSR_SAD_RDATA1_MASK                                 0xffffffff
#define STS_ENET_CSR_SAD_RDATA1_RD(src)                  (((src) & 0xffffffff))
#define STS_ENET_CSR_SAD_RDATA1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register ENET_SAD_Rdata2_Reg	*/ 
/*	 Fields sts_enet_csr_sad_rdata2	 */
#define STS_ENET_CSR_SAD_RDATA2_WIDTH                                        32
#define STS_ENET_CSR_SAD_RDATA2_SHIFT                                         0
#define STS_ENET_CSR_SAD_RDATA2_MASK                                 0xffffffff
#define STS_ENET_CSR_SAD_RDATA2_RD(src)                  (((src) & 0xffffffff))
#define STS_ENET_CSR_SAD_RDATA2_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register ENET_SAD_Rdata3_Reg	*/ 
/*	 Fields sts_enet_csr_sad_rdata3	 */
#define STS_ENET_CSR_SAD_RDATA3_WIDTH                                        32
#define STS_ENET_CSR_SAD_RDATA3_SHIFT                                         0
#define STS_ENET_CSR_SAD_RDATA3_MASK                                 0xffffffff
#define STS_ENET_CSR_SAD_RDATA3_RD(src)                  (((src) & 0xffffffff))
#define STS_ENET_CSR_SAD_RDATA3_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register ENET_SAD_CmdDone_Reg	*/ 
/*	 Fields cfg_enet_csr_sad_done	 */
#define CFG_ENET_CSR_SAD_DONE_WIDTH                                           1
#define CFG_ENET_CSR_SAD_DONE_SHIFT                                           0
#define CFG_ENET_CSR_SAD_DONE_MASK                                   0x00000001
#define CFG_ENET_CSR_SAD_DONE_RD(src)                    (((src) & 0x00000001))
#define CFG_ENET_CSR_SAD_DONE_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register RxBuf_Config_Thresh_Reg	*/ 
/*	 Fields cfg_rxbuf_full_thresh	 */
#define CFG_RXBUF_FULL_THRESH_WIDTH                                          16
#define CFG_RXBUF_FULL_THRESH_SHIFT                                          16
#define CFG_RXBUF_FULL_THRESH_MASK                                   0xffff0000
#define CFG_RXBUF_FULL_THRESH_RD(src)                (((src) & 0xffff0000)>>16)
#define CFG_RXBUF_FULL_THRESH_WR(src)           (((u32)(src)<<16) & 0xffff0000)
#define CFG_RXBUF_FULL_THRESH_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields cfg_rxbuf_empty_thresh	 */
#define CFG_RXBUF_EMPTY_THRESH_WIDTH                                         16
#define CFG_RXBUF_EMPTY_THRESH_SHIFT                                          0
#define CFG_RXBUF_EMPTY_THRESH_MASK                                  0x0000ffff
#define CFG_RXBUF_EMPTY_THRESH_RD(src)                   (((src) & 0x0000ffff))
#define CFG_RXBUF_EMPTY_THRESH_WR(src)              (((u32)(src)) & 0x0000ffff)
#define CFG_RXBUF_EMPTY_THRESH_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register RxBuf_Config_Port_Depth_Reg	*/ 
/*	 Fields cfg_rxbuf_port0_depth	 */
#define CFG_RXBUF_PORT0_DEPTH_WIDTH                                          16
#define CFG_RXBUF_PORT0_DEPTH_SHIFT                                          16
#define CFG_RXBUF_PORT0_DEPTH_MASK                                   0xffff0000
#define CFG_RXBUF_PORT0_DEPTH_RD(src)                (((src) & 0xffff0000)>>16)
#define CFG_RXBUF_PORT0_DEPTH_WR(src)           (((u32)(src)<<16) & 0xffff0000)
#define CFG_RXBUF_PORT0_DEPTH_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields cfg_rxbuf_port1_depth	 */
#define CFG_RXBUF_PORT1_DEPTH_WIDTH                                          16
#define CFG_RXBUF_PORT1_DEPTH_SHIFT                                           0
#define CFG_RXBUF_PORT1_DEPTH_MASK                                   0x0000ffff
#define CFG_RXBUF_PORT1_DEPTH_RD(src)                    (((src) & 0x0000ffff))
#define CFG_RXBUF_PORT1_DEPTH_WR(src)               (((u32)(src)) & 0x0000ffff)
#define CFG_RXBUF_PORT1_DEPTH_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register RxBuf_Config_Start_Addr_Reg	*/ 
/*	 Fields cfg_rxbuf_port0_startaddr	 */
#define CFG_RXBUF_PORT0_STARTADDR_WIDTH                                      16
#define CFG_RXBUF_PORT0_STARTADDR_SHIFT                                      16
#define CFG_RXBUF_PORT0_STARTADDR_MASK                               0xffff0000
#define CFG_RXBUF_PORT0_STARTADDR_RD(src)            (((src) & 0xffff0000)>>16)
#define CFG_RXBUF_PORT0_STARTADDR_WR(src)       (((u32)(src)<<16) & 0xffff0000)
#define CFG_RXBUF_PORT0_STARTADDR_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields cfg_rxbuf_port1_startaddr	 */
#define CFG_RXBUF_PORT1_STARTADDR_WIDTH                                      16
#define CFG_RXBUF_PORT1_STARTADDR_SHIFT                                       0
#define CFG_RXBUF_PORT1_STARTADDR_MASK                               0x0000ffff
#define CFG_RXBUF_PORT1_STARTADDR_RD(src)                (((src) & 0x0000ffff))
#define CFG_RXBUF_PORT1_STARTADDR_WR(src)           (((u32)(src)) & 0x0000ffff)
#define CFG_RXBUF_PORT1_STARTADDR_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register RxBuf_Config_Buffer_Reg	*/ 
/*	 Fields cfg_rxbuf_store_fwd_mode	 */
#define CFG_RXBUF_STORE_FWD_MODE_WIDTH                                        1
#define CFG_RXBUF_STORE_FWD_MODE_SHIFT                                       16
#define CFG_RXBUF_STORE_FWD_MODE_MASK                                0x00010000
#define CFG_RXBUF_STORE_FWD_MODE_RD(src)             (((src) & 0x00010000)>>16)
#define CFG_RXBUF_STORE_FWD_MODE_WR(src)        (((u32)(src)<<16) & 0x00010000)
#define CFG_RXBUF_STORE_FWD_MODE_SET(dst,src) \
                      (((dst) & ~0x00010000) | (((u32)(src)<<16) & 0x00010000))
/*	 Fields cfg_rxbuf_read_thresh	 */
#define CFG_RXBUF_READ_THRESH_WIDTH                                          16
#define CFG_RXBUF_READ_THRESH_SHIFT                                           0
#define CFG_RXBUF_READ_THRESH_MASK                                   0x0000ffff
#define CFG_RXBUF_READ_THRESH_RD(src)                    (((src) & 0x0000ffff))
#define CFG_RXBUF_READ_THRESH_WR(src)               (((u32)(src)) & 0x0000ffff)
#define CFG_RXBUF_READ_THRESH_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register TxBuf_Config_Thresh_Reg	*/ 
/*	 Fields cfg_txbuf_full_thresh	 */
#define CFG_TXBUF_FULL_THRESH_WIDTH                                          16
#define CFG_TXBUF_FULL_THRESH_SHIFT                                          16
#define CFG_TXBUF_FULL_THRESH_MASK                                   0xffff0000
#define CFG_TXBUF_FULL_THRESH_RD(src)                (((src) & 0xffff0000)>>16)
#define CFG_TXBUF_FULL_THRESH_WR(src)           (((u32)(src)<<16) & 0xffff0000)
#define CFG_TXBUF_FULL_THRESH_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields cfg_txbuf_empty_thresh	 */
#define CFG_TXBUF_EMPTY_THRESH_WIDTH                                         16
#define CFG_TXBUF_EMPTY_THRESH_SHIFT                                          0
#define CFG_TXBUF_EMPTY_THRESH_MASK                                  0x0000ffff
#define CFG_TXBUF_EMPTY_THRESH_RD(src)                   (((src) & 0x0000ffff))
#define CFG_TXBUF_EMPTY_THRESH_WR(src)              (((u32)(src)) & 0x0000ffff)
#define CFG_TXBUF_EMPTY_THRESH_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register TxBuf_Config_Port_Depth_Reg	*/ 
/*	 Fields cfg_txbuf_port0_depth	 */
#define CFG_TXBUF_PORT0_DEPTH_WIDTH                                          16
#define CFG_TXBUF_PORT0_DEPTH_SHIFT                                          16
#define CFG_TXBUF_PORT0_DEPTH_MASK                                   0xffff0000
#define CFG_TXBUF_PORT0_DEPTH_RD(src)                (((src) & 0xffff0000)>>16)
#define CFG_TXBUF_PORT0_DEPTH_WR(src)           (((u32)(src)<<16) & 0xffff0000)
#define CFG_TXBUF_PORT0_DEPTH_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields cfg_txbuf_port1_depth	 */
#define CFG_TXBUF_PORT1_DEPTH_WIDTH                                          16
#define CFG_TXBUF_PORT1_DEPTH_SHIFT                                           0
#define CFG_TXBUF_PORT1_DEPTH_MASK                                   0x0000ffff
#define CFG_TXBUF_PORT1_DEPTH_RD(src)                    (((src) & 0x0000ffff))
#define CFG_TXBUF_PORT1_DEPTH_WR(src)               (((u32)(src)) & 0x0000ffff)
#define CFG_TXBUF_PORT1_DEPTH_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register TxBuf_Config_Start_Addr_Reg	*/ 
/*	 Fields cfg_txbuf_port0_startaddr	 */
#define CFG_TXBUF_PORT0_STARTADDR_WIDTH                                      16
#define CFG_TXBUF_PORT0_STARTADDR_SHIFT                                      16
#define CFG_TXBUF_PORT0_STARTADDR_MASK                               0xffff0000
#define CFG_TXBUF_PORT0_STARTADDR_RD(src)            (((src) & 0xffff0000)>>16)
#define CFG_TXBUF_PORT0_STARTADDR_WR(src)       (((u32)(src)<<16) & 0xffff0000)
#define CFG_TXBUF_PORT0_STARTADDR_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields cfg_txbuf_port1_startaddr	 */
#define CFG_TXBUF_PORT1_STARTADDR_WIDTH                                      16
#define CFG_TXBUF_PORT1_STARTADDR_SHIFT                                       0
#define CFG_TXBUF_PORT1_STARTADDR_MASK                               0x0000ffff
#define CFG_TXBUF_PORT1_STARTADDR_RD(src)                (((src) & 0x0000ffff))
#define CFG_TXBUF_PORT1_STARTADDR_WR(src)           (((u32)(src)) & 0x0000ffff)
#define CFG_TXBUF_PORT1_STARTADDR_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register TxBuf_Config_Buffer_Reg	*/ 
/*	 Fields cfg_txbuf_store_fwd_mode	 */
#define CFG_TXBUF_STORE_FWD_MODE_WIDTH                                        1
#define CFG_TXBUF_STORE_FWD_MODE_SHIFT                                       16
#define CFG_TXBUF_STORE_FWD_MODE_MASK                                0x00010000
#define CFG_TXBUF_STORE_FWD_MODE_RD(src)             (((src) & 0x00010000)>>16)
#define CFG_TXBUF_STORE_FWD_MODE_WR(src)        (((u32)(src)<<16) & 0x00010000)
#define CFG_TXBUF_STORE_FWD_MODE_SET(dst,src) \
                      (((dst) & ~0x00010000) | (((u32)(src)<<16) & 0x00010000))
/*	 Fields cfg_txbuf_read_thresh	 */
#define CFG_TXBUF_READ_THRESH_WIDTH                                          16
#define CFG_TXBUF_READ_THRESH_SHIFT                                           0
#define CFG_TXBUF_READ_THRESH_MASK                                   0x0000ffff
#define CFG_TXBUF_READ_THRESH_RD(src)                    (((src) & 0x0000ffff))
#define CFG_TXBUF_READ_THRESH_WR(src)               (((u32)(src)) & 0x0000ffff)
#define CFG_TXBUF_READ_THRESH_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register Timer_Config1_Reg	*/ 
/*	 Fields CfgMicroSecInc	 */
#define CFGMICROSECINC_WIDTH                                                 10
#define CFGMICROSECINC_SHIFT                                                 20
#define CFGMICROSECINC_MASK                                          0x3ff00000
#define CFGMICROSECINC_RD(src)                       (((src) & 0x3ff00000)>>20)
#define CFGMICROSECINC_WR(src)                  (((u32)(src)<<20) & 0x3ff00000)
#define CFGMICROSECINC_SET(dst,src) \
                      (((dst) & ~0x3ff00000) | (((u32)(src)<<20) & 0x3ff00000))
/*	 Fields CfgMicroSecLimit	 */
#define CFGMICROSECLIMIT_WIDTH                                               10
#define CFGMICROSECLIMIT_SHIFT                                               10
#define CFGMICROSECLIMIT_MASK                                        0x000ffc00
#define CFGMICROSECLIMIT_RD(src)                     (((src) & 0x000ffc00)>>10)
#define CFGMICROSECLIMIT_WR(src)                (((u32)(src)<<10) & 0x000ffc00)
#define CFGMICROSECLIMIT_SET(dst,src) \
                      (((dst) & ~0x000ffc00) | (((u32)(src)<<10) & 0x000ffc00))
/*	 Fields CfgMilliSecLimit	 */
#define CFGMILLISECLIMIT_WIDTH                                               10
#define CFGMILLISECLIMIT_SHIFT                                                0
#define CFGMILLISECLIMIT_MASK                                        0x000003ff
#define CFGMILLISECLIMIT_RD(src)                         (((src) & 0x000003ff))
#define CFGMILLISECLIMIT_WR(src)                    (((u32)(src)) & 0x000003ff)
#define CFGMILLISECLIMIT_SET(dst,src) \
                          (((dst) & ~0x000003ff) | (((u32)(src)) & 0x000003ff))

/*	Register Timer_Config2_Reg	*/ 
/*	 Fields Cfg100NSecCntLimit	 */
#define CFG100NSECCNTLIMIT_WIDTH                                              8
#define CFG100NSECCNTLIMIT_SHIFT                                             16
#define CFG100NSECCNTLIMIT_MASK                                      0x00ff0000
#define CFG100NSECCNTLIMIT_RD(src)                   (((src) & 0x00ff0000)>>16)
#define CFG100NSECCNTLIMIT_WR(src)              (((u32)(src)<<16) & 0x00ff0000)
#define CFG100NSECCNTLIMIT_SET(dst,src) \
                      (((dst) & ~0x00ff0000) | (((u32)(src)<<16) & 0x00ff0000))
/*	 Fields CfgMCnt	 */
#define CFGMCNT_WIDTH                                                         8
#define CFGMCNT_SHIFT                                                         8
#define CFGMCNT_MASK                                                 0x0000ff00
#define CFGMCNT_RD(src)                               (((src) & 0x0000ff00)>>8)
#define CFGMCNT_WR(src)                          (((u32)(src)<<8) & 0x0000ff00)
#define CFGMCNT_SET(dst,src) \
                       (((dst) & ~0x0000ff00) | (((u32)(src)<<8) & 0x0000ff00))
/*	 Fields CfgNCnt	 */
#define CFGNCNT_WIDTH                                                         8
#define CFGNCNT_SHIFT                                                         0
#define CFGNCNT_MASK                                                 0x000000ff
#define CFGNCNT_RD(src)                                  (((src) & 0x000000ff))
#define CFGNCNT_WR(src)                             (((u32)(src)) & 0x000000ff)
#define CFGNCNT_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register Rx_Tx_Buf_Sts_Reg0	*/ 
/*	 Fields txbuf_fifo_empty_prt1	 */
#define TXBUF_FIFO_EMPTY_PRT10_WIDTH                                          1
#define TXBUF_FIFO_EMPTY_PRT10_SHIFT                                         17
#define TXBUF_FIFO_EMPTY_PRT10_MASK                                  0x00020000
#define TXBUF_FIFO_EMPTY_PRT10_RD(src)               (((src) & 0x00020000)>>17)
#define TXBUF_FIFO_EMPTY_PRT10_SET(dst,src) \
                      (((dst) & ~0x00020000) | (((u32)(src)<<17) & 0x00020000))
/*	 Fields rxbuf_fifo_empty_prt1	 */
#define RXBUF_FIFO_EMPTY_PRT10_WIDTH                                          1
#define RXBUF_FIFO_EMPTY_PRT10_SHIFT                                         16
#define RXBUF_FIFO_EMPTY_PRT10_MASK                                  0x00010000
#define RXBUF_FIFO_EMPTY_PRT10_RD(src)               (((src) & 0x00010000)>>16)
#define RXBUF_FIFO_EMPTY_PRT10_SET(dst,src) \
                      (((dst) & ~0x00010000) | (((u32)(src)<<16) & 0x00010000))
/*	 Fields txbuf_fifo_empty_prt0	 */
#define TXBUF_FIFO_EMPTY_PRT00_WIDTH                                          1
#define TXBUF_FIFO_EMPTY_PRT00_SHIFT                                          1
#define TXBUF_FIFO_EMPTY_PRT00_MASK                                  0x00000002
#define TXBUF_FIFO_EMPTY_PRT00_RD(src)                (((src) & 0x00000002)>>1)
#define TXBUF_FIFO_EMPTY_PRT00_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields rxbuf_fifo_empty_prt0	 */
#define RXBUF_FIFO_EMPTY_PRT00_WIDTH                                          1
#define RXBUF_FIFO_EMPTY_PRT00_SHIFT                                          0
#define RXBUF_FIFO_EMPTY_PRT00_MASK                                  0x00000001
#define RXBUF_FIFO_EMPTY_PRT00_RD(src)                   (((src) & 0x00000001))
#define RXBUF_FIFO_EMPTY_PRT00_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register Rx_Tx_buf_chksm_Int_Reg0	*/ 
/*	 Fields rxbuf_pause_intr_port1	 */
#define RXBUF_PAUSE_INTR_PORT10_WIDTH                                         1
#define RXBUF_PAUSE_INTR_PORT10_SHIFT                                        22
#define RXBUF_PAUSE_INTR_PORT10_MASK                                 0x00400000
#define RXBUF_PAUSE_INTR_PORT10_RD(src)              (((src) & 0x00400000)>>22)
#define RXBUF_PAUSE_INTR_PORT10_WR(src)         (((u32)(src)<<22) & 0x00400000)
#define RXBUF_PAUSE_INTR_PORT10_SET(dst,src) \
                      (((dst) & ~0x00400000) | (((u32)(src)<<22) & 0x00400000))
/*	 Fields rx_chksum_intr_port1	 */
#define RX_CHKSUM_INTR_PORT10_WIDTH                                           1
#define RX_CHKSUM_INTR_PORT10_SHIFT                                          21
#define RX_CHKSUM_INTR_PORT10_MASK                                   0x00200000
#define RX_CHKSUM_INTR_PORT10_RD(src)                (((src) & 0x00200000)>>21)
#define RX_CHKSUM_INTR_PORT10_WR(src)           (((u32)(src)<<21) & 0x00200000)
#define RX_CHKSUM_INTR_PORT10_SET(dst,src) \
                      (((dst) & ~0x00200000) | (((u32)(src)<<21) & 0x00200000))
/*	 Fields tx_chksum_intr_port1	 */
#define TX_CHKSUM_INTR_PORT10_WIDTH                                           1
#define TX_CHKSUM_INTR_PORT10_SHIFT                                          20
#define TX_CHKSUM_INTR_PORT10_MASK                                   0x00100000
#define TX_CHKSUM_INTR_PORT10_RD(src)                (((src) & 0x00100000)>>20)
#define TX_CHKSUM_INTR_PORT10_WR(src)           (((u32)(src)<<20) & 0x00100000)
#define TX_CHKSUM_INTR_PORT10_SET(dst,src) \
                      (((dst) & ~0x00100000) | (((u32)(src)<<20) & 0x00100000))
/*	 Fields rxbuf_fifo_overfl_intr_prt1	 */
#define RXBUF_FIFO_OVERFL_INTR_PRT10_WIDTH                                    1
#define RXBUF_FIFO_OVERFL_INTR_PRT10_SHIFT                                   19
#define RXBUF_FIFO_OVERFL_INTR_PRT10_MASK                            0x00080000
#define RXBUF_FIFO_OVERFL_INTR_PRT10_RD(src)         (((src) & 0x00080000)>>19)
#define RXBUF_FIFO_OVERFL_INTR_PRT10_WR(src)    (((u32)(src)<<19) & 0x00080000)
#define RXBUF_FIFO_OVERFL_INTR_PRT10_SET(dst,src) \
                      (((dst) & ~0x00080000) | (((u32)(src)<<19) & 0x00080000))
/*	 Fields rxbuf_fifo_underfl_intr_prt1	 */
#define RXBUF_FIFO_UNDERFL_INTR_PRT10_WIDTH                                   1
#define RXBUF_FIFO_UNDERFL_INTR_PRT10_SHIFT                                  18
#define RXBUF_FIFO_UNDERFL_INTR_PRT10_MASK                           0x00040000
#define RXBUF_FIFO_UNDERFL_INTR_PRT10_RD(src)        (((src) & 0x00040000)>>18)
#define RXBUF_FIFO_UNDERFL_INTR_PRT10_WR(src)   (((u32)(src)<<18) & 0x00040000)
#define RXBUF_FIFO_UNDERFL_INTR_PRT10_SET(dst,src) \
                      (((dst) & ~0x00040000) | (((u32)(src)<<18) & 0x00040000))
/*	 Fields txbuf_fifo_overfl_intr_prt1	 */
#define TXBUF_FIFO_OVERFL_INTR_PRT10_WIDTH                                    1
#define TXBUF_FIFO_OVERFL_INTR_PRT10_SHIFT                                   17
#define TXBUF_FIFO_OVERFL_INTR_PRT10_MASK                            0x00020000
#define TXBUF_FIFO_OVERFL_INTR_PRT10_RD(src)         (((src) & 0x00020000)>>17)
#define TXBUF_FIFO_OVERFL_INTR_PRT10_WR(src)    (((u32)(src)<<17) & 0x00020000)
#define TXBUF_FIFO_OVERFL_INTR_PRT10_SET(dst,src) \
                      (((dst) & ~0x00020000) | (((u32)(src)<<17) & 0x00020000))
/*	 Fields txbuf_fifo_underfl_intr_prt1	 */
#define TXBUF_FIFO_UNDERFL_INTR_PRT10_WIDTH                                   1
#define TXBUF_FIFO_UNDERFL_INTR_PRT10_SHIFT                                  16
#define TXBUF_FIFO_UNDERFL_INTR_PRT10_MASK                           0x00010000
#define TXBUF_FIFO_UNDERFL_INTR_PRT10_RD(src)        (((src) & 0x00010000)>>16)
#define TXBUF_FIFO_UNDERFL_INTR_PRT10_WR(src)   (((u32)(src)<<16) & 0x00010000)
#define TXBUF_FIFO_UNDERFL_INTR_PRT10_SET(dst,src) \
                      (((dst) & ~0x00010000) | (((u32)(src)<<16) & 0x00010000))
/*	 Fields rxbuf_pause_intr_port0	 */
#define RXBUF_PAUSE_INTR_PORT00_WIDTH                                         1
#define RXBUF_PAUSE_INTR_PORT00_SHIFT                                         6
#define RXBUF_PAUSE_INTR_PORT00_MASK                                 0x00000040
#define RXBUF_PAUSE_INTR_PORT00_RD(src)               (((src) & 0x00000040)>>6)
#define RXBUF_PAUSE_INTR_PORT00_WR(src)          (((u32)(src)<<6) & 0x00000040)
#define RXBUF_PAUSE_INTR_PORT00_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*	 Fields rx_chksum_intr_port0	 */
#define RX_CHKSUM_INTR_PORT00_WIDTH                                           1
#define RX_CHKSUM_INTR_PORT00_SHIFT                                           5
#define RX_CHKSUM_INTR_PORT00_MASK                                   0x00000020
#define RX_CHKSUM_INTR_PORT00_RD(src)                 (((src) & 0x00000020)>>5)
#define RX_CHKSUM_INTR_PORT00_WR(src)            (((u32)(src)<<5) & 0x00000020)
#define RX_CHKSUM_INTR_PORT00_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields tx_chksum_intr_port0	 */
#define TX_CHKSUM_INTR_PORT00_WIDTH                                           1
#define TX_CHKSUM_INTR_PORT00_SHIFT                                           4
#define TX_CHKSUM_INTR_PORT00_MASK                                   0x00000010
#define TX_CHKSUM_INTR_PORT00_RD(src)                 (((src) & 0x00000010)>>4)
#define TX_CHKSUM_INTR_PORT00_WR(src)            (((u32)(src)<<4) & 0x00000010)
#define TX_CHKSUM_INTR_PORT00_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields rxbuf_fifo_overfl_intr_prt0	 */
#define RXBUF_FIFO_OVERFL_INTR_PRT00_WIDTH                                    1
#define RXBUF_FIFO_OVERFL_INTR_PRT00_SHIFT                                    3
#define RXBUF_FIFO_OVERFL_INTR_PRT00_MASK                            0x00000008
#define RXBUF_FIFO_OVERFL_INTR_PRT00_RD(src)          (((src) & 0x00000008)>>3)
#define RXBUF_FIFO_OVERFL_INTR_PRT00_WR(src)     (((u32)(src)<<3) & 0x00000008)
#define RXBUF_FIFO_OVERFL_INTR_PRT00_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields rxbuf_fifo_underfl_intr_prt0	 */
#define RXBUF_FIFO_UNDERFL_INTR_PRT00_WIDTH                                   1
#define RXBUF_FIFO_UNDERFL_INTR_PRT00_SHIFT                                   2
#define RXBUF_FIFO_UNDERFL_INTR_PRT00_MASK                           0x00000004
#define RXBUF_FIFO_UNDERFL_INTR_PRT00_RD(src)         (((src) & 0x00000004)>>2)
#define RXBUF_FIFO_UNDERFL_INTR_PRT00_WR(src)    (((u32)(src)<<2) & 0x00000004)
#define RXBUF_FIFO_UNDERFL_INTR_PRT00_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields txbuf_fifo_overfl_intr_prt0	 */
#define TXBUF_FIFO_OVERFL_INTR_PRT00_WIDTH                                    1
#define TXBUF_FIFO_OVERFL_INTR_PRT00_SHIFT                                    1
#define TXBUF_FIFO_OVERFL_INTR_PRT00_MASK                            0x00000002
#define TXBUF_FIFO_OVERFL_INTR_PRT00_RD(src)          (((src) & 0x00000002)>>1)
#define TXBUF_FIFO_OVERFL_INTR_PRT00_WR(src)     (((u32)(src)<<1) & 0x00000002)
#define TXBUF_FIFO_OVERFL_INTR_PRT00_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields txbuf_fifo_underfl_intr_prt0	 */
#define TXBUF_FIFO_UNDERFL_INTR_PRT00_WIDTH                                   1
#define TXBUF_FIFO_UNDERFL_INTR_PRT00_SHIFT                                   0
#define TXBUF_FIFO_UNDERFL_INTR_PRT00_MASK                           0x00000001
#define TXBUF_FIFO_UNDERFL_INTR_PRT00_RD(src)            (((src) & 0x00000001))
#define TXBUF_FIFO_UNDERFL_INTR_PRT00_WR(src)       (((u32)(src)) & 0x00000001)
#define TXBUF_FIFO_UNDERFL_INTR_PRT00_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register Rx_Tx_buf_chksm_Int_Reg0Mask	*/
/*    Mask Register Fields rxbuf_pause_intr_port1Mask    */
#define RXBUF_PAUSE_INTR_PORT1MASK_WIDTH                                      1
#define RXBUF_PAUSE_INTR_PORT1MASK_SHIFT                                     22
#define RXBUF_PAUSE_INTR_PORT1MASK_MASK                              0x00400000
#define RXBUF_PAUSE_INTR_PORT1MASK_RD(src)           (((src) & 0x00400000)>>22)
#define RXBUF_PAUSE_INTR_PORT1MASK_WR(src)      (((u32)(src)<<22) & 0x00400000)
#define RXBUF_PAUSE_INTR_PORT1MASK_SET(dst,src) \
                      (((dst) & ~0x00400000) | (((u32)(src)<<22) & 0x00400000))
/*    Mask Register Fields rx_chksum_intr_port1Mask    */
#define RX_CHKSUM_INTR_PORT1MASK_WIDTH                                        1
#define RX_CHKSUM_INTR_PORT1MASK_SHIFT                                       21
#define RX_CHKSUM_INTR_PORT1MASK_MASK                                0x00200000
#define RX_CHKSUM_INTR_PORT1MASK_RD(src)             (((src) & 0x00200000)>>21)
#define RX_CHKSUM_INTR_PORT1MASK_WR(src)        (((u32)(src)<<21) & 0x00200000)
#define RX_CHKSUM_INTR_PORT1MASK_SET(dst,src) \
                      (((dst) & ~0x00200000) | (((u32)(src)<<21) & 0x00200000))
/*    Mask Register Fields tx_chksum_intr_port1Mask    */
#define TX_CHKSUM_INTR_PORT1MASK_WIDTH                                        1
#define TX_CHKSUM_INTR_PORT1MASK_SHIFT                                       20
#define TX_CHKSUM_INTR_PORT1MASK_MASK                                0x00100000
#define TX_CHKSUM_INTR_PORT1MASK_RD(src)             (((src) & 0x00100000)>>20)
#define TX_CHKSUM_INTR_PORT1MASK_WR(src)        (((u32)(src)<<20) & 0x00100000)
#define TX_CHKSUM_INTR_PORT1MASK_SET(dst,src) \
                      (((dst) & ~0x00100000) | (((u32)(src)<<20) & 0x00100000))
/*    Mask Register Fields rxbuf_fifo_overfl_intr_prt1Mask    */
#define RXBUF_FIFO_OVERFL_INTR_PRT1MASK_WIDTH                                 1
#define RXBUF_FIFO_OVERFL_INTR_PRT1MASK_SHIFT                                19
#define RXBUF_FIFO_OVERFL_INTR_PRT1MASK_MASK                         0x00080000
#define RXBUF_FIFO_OVERFL_INTR_PRT1MASK_RD(src)      (((src) & 0x00080000)>>19)
#define RXBUF_FIFO_OVERFL_INTR_PRT1MASK_WR(src) \
                                                (((u32)(src)<<19) & 0x00080000)
#define RXBUF_FIFO_OVERFL_INTR_PRT1MASK_SET(dst,src) \
                      (((dst) & ~0x00080000) | (((u32)(src)<<19) & 0x00080000))
/*    Mask Register Fields rxbuf_fifo_underfl_intr_prt1Mask    */
#define RXBUF_FIFO_UNDERFL_INTR_PRT1MASK_WIDTH                                1
#define RXBUF_FIFO_UNDERFL_INTR_PRT1MASK_SHIFT                               18
#define RXBUF_FIFO_UNDERFL_INTR_PRT1MASK_MASK                        0x00040000
#define RXBUF_FIFO_UNDERFL_INTR_PRT1MASK_RD(src)     (((src) & 0x00040000)>>18)
#define RXBUF_FIFO_UNDERFL_INTR_PRT1MASK_WR(src) \
                                                (((u32)(src)<<18) & 0x00040000)
#define RXBUF_FIFO_UNDERFL_INTR_PRT1MASK_SET(dst,src) \
                      (((dst) & ~0x00040000) | (((u32)(src)<<18) & 0x00040000))
/*    Mask Register Fields txbuf_fifo_overfl_intr_prt1Mask    */
#define TXBUF_FIFO_OVERFL_INTR_PRT1MASK_WIDTH                                 1
#define TXBUF_FIFO_OVERFL_INTR_PRT1MASK_SHIFT                                17
#define TXBUF_FIFO_OVERFL_INTR_PRT1MASK_MASK                         0x00020000
#define TXBUF_FIFO_OVERFL_INTR_PRT1MASK_RD(src)      (((src) & 0x00020000)>>17)
#define TXBUF_FIFO_OVERFL_INTR_PRT1MASK_WR(src) \
                                                (((u32)(src)<<17) & 0x00020000)
#define TXBUF_FIFO_OVERFL_INTR_PRT1MASK_SET(dst,src) \
                      (((dst) & ~0x00020000) | (((u32)(src)<<17) & 0x00020000))
/*    Mask Register Fields txbuf_fifo_underfl_intr_prt1Mask    */
#define TXBUF_FIFO_UNDERFL_INTR_PRT1MASK_WIDTH                                1
#define TXBUF_FIFO_UNDERFL_INTR_PRT1MASK_SHIFT                               16
#define TXBUF_FIFO_UNDERFL_INTR_PRT1MASK_MASK                        0x00010000
#define TXBUF_FIFO_UNDERFL_INTR_PRT1MASK_RD(src)     (((src) & 0x00010000)>>16)
#define TXBUF_FIFO_UNDERFL_INTR_PRT1MASK_WR(src) \
                                                (((u32)(src)<<16) & 0x00010000)
#define TXBUF_FIFO_UNDERFL_INTR_PRT1MASK_SET(dst,src) \
                      (((dst) & ~0x00010000) | (((u32)(src)<<16) & 0x00010000))
/*    Mask Register Fields rxbuf_pause_intr_port0Mask    */
#define RXBUF_PAUSE_INTR_PORT0MASK_WIDTH                                      1
#define RXBUF_PAUSE_INTR_PORT0MASK_SHIFT                                      6
#define RXBUF_PAUSE_INTR_PORT0MASK_MASK                              0x00000040
#define RXBUF_PAUSE_INTR_PORT0MASK_RD(src)            (((src) & 0x00000040)>>6)
#define RXBUF_PAUSE_INTR_PORT0MASK_WR(src)       (((u32)(src)<<6) & 0x00000040)
#define RXBUF_PAUSE_INTR_PORT0MASK_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*    Mask Register Fields rx_chksum_intr_port0Mask    */
#define RX_CHKSUM_INTR_PORT0MASK_WIDTH                                        1
#define RX_CHKSUM_INTR_PORT0MASK_SHIFT                                        5
#define RX_CHKSUM_INTR_PORT0MASK_MASK                                0x00000020
#define RX_CHKSUM_INTR_PORT0MASK_RD(src)              (((src) & 0x00000020)>>5)
#define RX_CHKSUM_INTR_PORT0MASK_WR(src)         (((u32)(src)<<5) & 0x00000020)
#define RX_CHKSUM_INTR_PORT0MASK_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*    Mask Register Fields tx_chksum_intr_port0Mask    */
#define TX_CHKSUM_INTR_PORT0MASK_WIDTH                                        1
#define TX_CHKSUM_INTR_PORT0MASK_SHIFT                                        4
#define TX_CHKSUM_INTR_PORT0MASK_MASK                                0x00000010
#define TX_CHKSUM_INTR_PORT0MASK_RD(src)              (((src) & 0x00000010)>>4)
#define TX_CHKSUM_INTR_PORT0MASK_WR(src)         (((u32)(src)<<4) & 0x00000010)
#define TX_CHKSUM_INTR_PORT0MASK_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*    Mask Register Fields rxbuf_fifo_overfl_intr_prt0Mask    */
#define RXBUF_FIFO_OVERFL_INTR_PRT0MASK_WIDTH                                 1
#define RXBUF_FIFO_OVERFL_INTR_PRT0MASK_SHIFT                                 3
#define RXBUF_FIFO_OVERFL_INTR_PRT0MASK_MASK                         0x00000008
#define RXBUF_FIFO_OVERFL_INTR_PRT0MASK_RD(src)       (((src) & 0x00000008)>>3)
#define RXBUF_FIFO_OVERFL_INTR_PRT0MASK_WR(src)  (((u32)(src)<<3) & 0x00000008)
#define RXBUF_FIFO_OVERFL_INTR_PRT0MASK_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*    Mask Register Fields rxbuf_fifo_underfl_intr_prt0Mask    */
#define RXBUF_FIFO_UNDERFL_INTR_PRT0MASK_WIDTH                                1
#define RXBUF_FIFO_UNDERFL_INTR_PRT0MASK_SHIFT                                2
#define RXBUF_FIFO_UNDERFL_INTR_PRT0MASK_MASK                        0x00000004
#define RXBUF_FIFO_UNDERFL_INTR_PRT0MASK_RD(src)      (((src) & 0x00000004)>>2)
#define RXBUF_FIFO_UNDERFL_INTR_PRT0MASK_WR(src) \
                                                 (((u32)(src)<<2) & 0x00000004)
#define RXBUF_FIFO_UNDERFL_INTR_PRT0MASK_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*    Mask Register Fields txbuf_fifo_overfl_intr_prt0Mask    */
#define TXBUF_FIFO_OVERFL_INTR_PRT0MASK_WIDTH                                 1
#define TXBUF_FIFO_OVERFL_INTR_PRT0MASK_SHIFT                                 1
#define TXBUF_FIFO_OVERFL_INTR_PRT0MASK_MASK                         0x00000002
#define TXBUF_FIFO_OVERFL_INTR_PRT0MASK_RD(src)       (((src) & 0x00000002)>>1)
#define TXBUF_FIFO_OVERFL_INTR_PRT0MASK_WR(src)  (((u32)(src)<<1) & 0x00000002)
#define TXBUF_FIFO_OVERFL_INTR_PRT0MASK_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*    Mask Register Fields txbuf_fifo_underfl_intr_prt0Mask    */
#define TXBUF_FIFO_UNDERFL_INTR_PRT0MASK_WIDTH                                1
#define TXBUF_FIFO_UNDERFL_INTR_PRT0MASK_SHIFT                                0
#define TXBUF_FIFO_UNDERFL_INTR_PRT0MASK_MASK                        0x00000001
#define TXBUF_FIFO_UNDERFL_INTR_PRT0MASK_RD(src)         (((src) & 0x00000001))
#define TXBUF_FIFO_UNDERFL_INTR_PRT0MASK_WR(src)    (((u32)(src)) & 0x00000001)
#define TXBUF_FIFO_UNDERFL_INTR_PRT0MASK_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register RxBuf_Config_Pause_Thresh_Reg	*/ 
/*	 Fields cfg_rxbuf1_pause_thresh	 */
#define CFG_RXBUF1_PAUSE_THRESH_WIDTH                                        16
#define CFG_RXBUF1_PAUSE_THRESH_SHIFT                                        16
#define CFG_RXBUF1_PAUSE_THRESH_MASK                                 0xffff0000
#define CFG_RXBUF1_PAUSE_THRESH_RD(src)              (((src) & 0xffff0000)>>16)
#define CFG_RXBUF1_PAUSE_THRESH_WR(src)         (((u32)(src)<<16) & 0xffff0000)
#define CFG_RXBUF1_PAUSE_THRESH_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields cfg_rxbuf0_pause_thresh	 */
#define CFG_RXBUF0_PAUSE_THRESH_WIDTH                                        16
#define CFG_RXBUF0_PAUSE_THRESH_SHIFT                                         0
#define CFG_RXBUF0_PAUSE_THRESH_MASK                                 0x0000ffff
#define CFG_RXBUF0_PAUSE_THRESH_RD(src)                  (((src) & 0x0000ffff))
#define CFG_RXBUF0_PAUSE_THRESH_WR(src)             (((u32)(src)) & 0x0000ffff)
#define CFG_RXBUF0_PAUSE_THRESH_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register Memory_Config1_Reg	*/ 
/*	 Fields cfg_txbuf_TEST1	 */
#define CFG_TXBUF_TEST1_WIDTH                                                 1
#define CFG_TXBUF_TEST1_SHIFT                                                31
#define CFG_TXBUF_TEST1_MASK                                         0x80000000
#define CFG_TXBUF_TEST1_RD(src)                      (((src) & 0x80000000)>>31)
#define CFG_TXBUF_TEST1_WR(src)                 (((u32)(src)<<31) & 0x80000000)
#define CFG_TXBUF_TEST1_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields cfg_txbuf_RME	 */
#define CFG_TXBUF_RME_WIDTH                                                   1
#define CFG_TXBUF_RME_SHIFT                                                  30
#define CFG_TXBUF_RME_MASK                                           0x40000000
#define CFG_TXBUF_RME_RD(src)                        (((src) & 0x40000000)>>30)
#define CFG_TXBUF_RME_WR(src)                   (((u32)(src)<<30) & 0x40000000)
#define CFG_TXBUF_RME_SET(dst,src) \
                      (((dst) & ~0x40000000) | (((u32)(src)<<30) & 0x40000000))
/*	 Fields cfg_txbuf_RM	 */
#define CFG_TXBUF_RM_WIDTH                                                    4
#define CFG_TXBUF_RM_SHIFT                                                   26
#define CFG_TXBUF_RM_MASK                                            0x3c000000
#define CFG_TXBUF_RM_RD(src)                         (((src) & 0x3c000000)>>26)
#define CFG_TXBUF_RM_WR(src)                    (((u32)(src)<<26) & 0x3c000000)
#define CFG_TXBUF_RM_SET(dst,src) \
                      (((dst) & ~0x3c000000) | (((u32)(src)<<26) & 0x3c000000))
/*	 Fields cfg_rxbuf_TEST1B_prt1	 */
#define CFG_RXBUF_TEST1B_PRT1_WIDTH                                           1
#define CFG_RXBUF_TEST1B_PRT1_SHIFT                                          24
#define CFG_RXBUF_TEST1B_PRT1_MASK                                   0x01000000
#define CFG_RXBUF_TEST1B_PRT1_RD(src)                (((src) & 0x01000000)>>24)
#define CFG_RXBUF_TEST1B_PRT1_WR(src)           (((u32)(src)<<24) & 0x01000000)
#define CFG_RXBUF_TEST1B_PRT1_SET(dst,src) \
                      (((dst) & ~0x01000000) | (((u32)(src)<<24) & 0x01000000))
/*	 Fields cfg_rxbuf_TEST1A_prt1	 */
#define CFG_RXBUF_TEST1A_PRT1_WIDTH                                           1
#define CFG_RXBUF_TEST1A_PRT1_SHIFT                                          23
#define CFG_RXBUF_TEST1A_PRT1_MASK                                   0x00800000
#define CFG_RXBUF_TEST1A_PRT1_RD(src)                (((src) & 0x00800000)>>23)
#define CFG_RXBUF_TEST1A_PRT1_WR(src)           (((u32)(src)<<23) & 0x00800000)
#define CFG_RXBUF_TEST1A_PRT1_SET(dst,src) \
                      (((dst) & ~0x00800000) | (((u32)(src)<<23) & 0x00800000))
/*	 Fields cfg_rxbuf_RMEB_prt1	 */
#define CFG_RXBUF_RMEB_PRT1_WIDTH                                             1
#define CFG_RXBUF_RMEB_PRT1_SHIFT                                            22
#define CFG_RXBUF_RMEB_PRT1_MASK                                     0x00400000
#define CFG_RXBUF_RMEB_PRT1_RD(src)                  (((src) & 0x00400000)>>22)
#define CFG_RXBUF_RMEB_PRT1_WR(src)             (((u32)(src)<<22) & 0x00400000)
#define CFG_RXBUF_RMEB_PRT1_SET(dst,src) \
                      (((dst) & ~0x00400000) | (((u32)(src)<<22) & 0x00400000))
/*	 Fields cfg_rxbuf_RMEA_prt1	 */
#define CFG_RXBUF_RMEA_PRT1_WIDTH                                             1
#define CFG_RXBUF_RMEA_PRT1_SHIFT                                            21
#define CFG_RXBUF_RMEA_PRT1_MASK                                     0x00200000
#define CFG_RXBUF_RMEA_PRT1_RD(src)                  (((src) & 0x00200000)>>21)
#define CFG_RXBUF_RMEA_PRT1_WR(src)             (((u32)(src)<<21) & 0x00200000)
#define CFG_RXBUF_RMEA_PRT1_SET(dst,src) \
                      (((dst) & ~0x00200000) | (((u32)(src)<<21) & 0x00200000))
/*	 Fields cfg_rxbuf_RMB_prt1	 */
#define CFG_RXBUF_RMB_PRT1_WIDTH                                              4
#define CFG_RXBUF_RMB_PRT1_SHIFT                                             17
#define CFG_RXBUF_RMB_PRT1_MASK                                      0x001e0000
#define CFG_RXBUF_RMB_PRT1_RD(src)                   (((src) & 0x001e0000)>>17)
#define CFG_RXBUF_RMB_PRT1_WR(src)              (((u32)(src)<<17) & 0x001e0000)
#define CFG_RXBUF_RMB_PRT1_SET(dst,src) \
                      (((dst) & ~0x001e0000) | (((u32)(src)<<17) & 0x001e0000))
/*	 Fields cfg_rxbuf_RMA_prt1	 */
#define CFG_RXBUF_RMA_PRT1_WIDTH                                              4
#define CFG_RXBUF_RMA_PRT1_SHIFT                                             13
#define CFG_RXBUF_RMA_PRT1_MASK                                      0x0001e000
#define CFG_RXBUF_RMA_PRT1_RD(src)                   (((src) & 0x0001e000)>>13)
#define CFG_RXBUF_RMA_PRT1_WR(src)              (((u32)(src)<<13) & 0x0001e000)
#define CFG_RXBUF_RMA_PRT1_SET(dst,src) \
                      (((dst) & ~0x0001e000) | (((u32)(src)<<13) & 0x0001e000))
/*	 Fields cfg_rxbuf_TEST1B_prt0	 */
#define CFG_RXBUF_TEST1B_PRT0_WIDTH                                           1
#define CFG_RXBUF_TEST1B_PRT0_SHIFT                                          11
#define CFG_RXBUF_TEST1B_PRT0_MASK                                   0x00000800
#define CFG_RXBUF_TEST1B_PRT0_RD(src)                (((src) & 0x00000800)>>11)
#define CFG_RXBUF_TEST1B_PRT0_WR(src)           (((u32)(src)<<11) & 0x00000800)
#define CFG_RXBUF_TEST1B_PRT0_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*	 Fields cfg_rxbuf_TEST1A_prt0	 */
#define CFG_RXBUF_TEST1A_PRT0_WIDTH                                           1
#define CFG_RXBUF_TEST1A_PRT0_SHIFT                                          10
#define CFG_RXBUF_TEST1A_PRT0_MASK                                   0x00000400
#define CFG_RXBUF_TEST1A_PRT0_RD(src)                (((src) & 0x00000400)>>10)
#define CFG_RXBUF_TEST1A_PRT0_WR(src)           (((u32)(src)<<10) & 0x00000400)
#define CFG_RXBUF_TEST1A_PRT0_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields cfg_rxbuf_RMEB_prt0	 */
#define CFG_RXBUF_RMEB_PRT0_WIDTH                                             1
#define CFG_RXBUF_RMEB_PRT0_SHIFT                                             9
#define CFG_RXBUF_RMEB_PRT0_MASK                                     0x00000200
#define CFG_RXBUF_RMEB_PRT0_RD(src)                   (((src) & 0x00000200)>>9)
#define CFG_RXBUF_RMEB_PRT0_WR(src)              (((u32)(src)<<9) & 0x00000200)
#define CFG_RXBUF_RMEB_PRT0_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields cfg_rxbuf_RMEA_prt0	 */
#define CFG_RXBUF_RMEA_PRT0_WIDTH                                             1
#define CFG_RXBUF_RMEA_PRT0_SHIFT                                             8
#define CFG_RXBUF_RMEA_PRT0_MASK                                     0x00000100
#define CFG_RXBUF_RMEA_PRT0_RD(src)                   (((src) & 0x00000100)>>8)
#define CFG_RXBUF_RMEA_PRT0_WR(src)              (((u32)(src)<<8) & 0x00000100)
#define CFG_RXBUF_RMEA_PRT0_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*	 Fields cfg_rxbuf_RMB_prt0	 */
#define CFG_RXBUF_RMB_PRT0_WIDTH                                              4
#define CFG_RXBUF_RMB_PRT0_SHIFT                                              4
#define CFG_RXBUF_RMB_PRT0_MASK                                      0x000000f0
#define CFG_RXBUF_RMB_PRT0_RD(src)                    (((src) & 0x000000f0)>>4)
#define CFG_RXBUF_RMB_PRT0_WR(src)               (((u32)(src)<<4) & 0x000000f0)
#define CFG_RXBUF_RMB_PRT0_SET(dst,src) \
                       (((dst) & ~0x000000f0) | (((u32)(src)<<4) & 0x000000f0))
/*	 Fields cfg_rxbuf_RMA_prt0	 */
#define CFG_RXBUF_RMA_PRT0_WIDTH                                              4
#define CFG_RXBUF_RMA_PRT0_SHIFT                                              0
#define CFG_RXBUF_RMA_PRT0_MASK                                      0x0000000f
#define CFG_RXBUF_RMA_PRT0_RD(src)                       (((src) & 0x0000000f))
#define CFG_RXBUF_RMA_PRT0_WR(src)                  (((u32)(src)) & 0x0000000f)
#define CFG_RXBUF_RMA_PRT0_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register Memory_Config2_Reg	*/ 
/*	 Fields cfg_sad_mem_TEST1	 */
#define CFG_SAD_MEM_TEST1_WIDTH                                               1
#define CFG_SAD_MEM_TEST1_SHIFT                                              13
#define CFG_SAD_MEM_TEST1_MASK                                       0x00002000
#define CFG_SAD_MEM_TEST1_RD(src)                    (((src) & 0x00002000)>>13)
#define CFG_SAD_MEM_TEST1_WR(src)               (((u32)(src)<<13) & 0x00002000)
#define CFG_SAD_MEM_TEST1_SET(dst,src) \
                      (((dst) & ~0x00002000) | (((u32)(src)<<13) & 0x00002000))
/*	 Fields cfg_sad_mem_RME	 */
#define CFG_SAD_MEM_RME_WIDTH                                                 1
#define CFG_SAD_MEM_RME_SHIFT                                                12
#define CFG_SAD_MEM_RME_MASK                                         0x00001000
#define CFG_SAD_MEM_RME_RD(src)                      (((src) & 0x00001000)>>12)
#define CFG_SAD_MEM_RME_WR(src)                 (((u32)(src)<<12) & 0x00001000)
#define CFG_SAD_MEM_RME_SET(dst,src) \
                      (((dst) & ~0x00001000) | (((u32)(src)<<12) & 0x00001000))
/*	 Fields cfg_sad_mem_RM	 */
#define CFG_SAD_MEM_RM_WIDTH                                                  4
#define CFG_SAD_MEM_RM_SHIFT                                                  8
#define CFG_SAD_MEM_RM_MASK                                          0x00000f00
#define CFG_SAD_MEM_RM_RD(src)                        (((src) & 0x00000f00)>>8)
#define CFG_SAD_MEM_RM_WR(src)                   (((u32)(src)<<8) & 0x00000f00)
#define CFG_SAD_MEM_RM_SET(dst,src) \
                       (((dst) & ~0x00000f00) | (((u32)(src)<<8) & 0x00000f00))
/*	 Fields cfg_avl_ram_TEST1	 */
#define CFG_AVL_RAM_TEST1_WIDTH                                               1
#define CFG_AVL_RAM_TEST1_SHIFT                                               5
#define CFG_AVL_RAM_TEST1_MASK                                       0x00000020
#define CFG_AVL_RAM_TEST1_RD(src)                     (((src) & 0x00000020)>>5)
#define CFG_AVL_RAM_TEST1_WR(src)                (((u32)(src)<<5) & 0x00000020)
#define CFG_AVL_RAM_TEST1_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields cfg_avl_ram_RME	 */
#define CFG_AVL_RAM_RME_WIDTH                                                 1
#define CFG_AVL_RAM_RME_SHIFT                                                 4
#define CFG_AVL_RAM_RME_MASK                                         0x00000010
#define CFG_AVL_RAM_RME_RD(src)                       (((src) & 0x00000010)>>4)
#define CFG_AVL_RAM_RME_WR(src)                  (((u32)(src)<<4) & 0x00000010)
#define CFG_AVL_RAM_RME_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields cfg_avl_ram_RM	 */
#define CFG_AVL_RAM_RM_WIDTH                                                  4
#define CFG_AVL_RAM_RM_SHIFT                                                  0
#define CFG_AVL_RAM_RM_MASK                                          0x0000000f
#define CFG_AVL_RAM_RM_RD(src)                           (((src) & 0x0000000f))
#define CFG_AVL_RAM_RM_WR(src)                      (((u32)(src)) & 0x0000000f)
#define CFG_AVL_RAM_RM_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register PTP_Control	*/ 
/*	 Fields ffdiv_vco_clk_sel	 */
#define FFDIV_VCO_CLK_SEL_WIDTH                                               1
#define FFDIV_VCO_CLK_SEL_SHIFT                                              20
#define FFDIV_VCO_CLK_SEL_MASK                                       0x00100000
#define FFDIV_VCO_CLK_SEL_RD(src)                    (((src) & 0x00100000)>>20)
#define FFDIV_VCO_CLK_SEL_WR(src)               (((u32)(src)<<20) & 0x00100000)
#define FFDIV_VCO_CLK_SEL_SET(dst,src) \
                      (((dst) & ~0x00100000) | (((u32)(src)<<20) & 0x00100000))
/*	 Fields tstmp_fifo_emptythresh_prt1	 */
#define TSTMP_FIFO_EMPTYTHRESH_PRT1_WIDTH                                     3
#define TSTMP_FIFO_EMPTYTHRESH_PRT1_SHIFT                                    17
#define TSTMP_FIFO_EMPTYTHRESH_PRT1_MASK                             0x000e0000
#define TSTMP_FIFO_EMPTYTHRESH_PRT1_RD(src)          (((src) & 0x000e0000)>>17)
#define TSTMP_FIFO_EMPTYTHRESH_PRT1_WR(src)     (((u32)(src)<<17) & 0x000e0000)
#define TSTMP_FIFO_EMPTYTHRESH_PRT1_SET(dst,src) \
                      (((dst) & ~0x000e0000) | (((u32)(src)<<17) & 0x000e0000))
/*	 Fields tstmp_fifo_fullthresh_prt1	 */
#define TSTMP_FIFO_FULLTHRESH_PRT1_WIDTH                                      3
#define TSTMP_FIFO_FULLTHRESH_PRT1_SHIFT                                     14
#define TSTMP_FIFO_FULLTHRESH_PRT1_MASK                              0x0001c000
#define TSTMP_FIFO_FULLTHRESH_PRT1_RD(src)           (((src) & 0x0001c000)>>14)
#define TSTMP_FIFO_FULLTHRESH_PRT1_WR(src)      (((u32)(src)<<14) & 0x0001c000)
#define TSTMP_FIFO_FULLTHRESH_PRT1_SET(dst,src) \
                      (((dst) & ~0x0001c000) | (((u32)(src)<<14) & 0x0001c000))
/*	 Fields tstmp_fifo_ramdepth_prt1	 */
#define TSTMP_FIFO_RAMDEPTH_PRT1_WIDTH                                        3
#define TSTMP_FIFO_RAMDEPTH_PRT1_SHIFT                                       11
#define TSTMP_FIFO_RAMDEPTH_PRT1_MASK                                0x00003800
#define TSTMP_FIFO_RAMDEPTH_PRT1_RD(src)             (((src) & 0x00003800)>>11)
#define TSTMP_FIFO_RAMDEPTH_PRT1_WR(src)        (((u32)(src)<<11) & 0x00003800)
#define TSTMP_FIFO_RAMDEPTH_PRT1_SET(dst,src) \
                      (((dst) & ~0x00003800) | (((u32)(src)<<11) & 0x00003800))
/*	 Fields tstmp_fifo_emptythresh_prt0	 */
#define TSTMP_FIFO_EMPTYTHRESH_PRT0_WIDTH                                     3
#define TSTMP_FIFO_EMPTYTHRESH_PRT0_SHIFT                                     8
#define TSTMP_FIFO_EMPTYTHRESH_PRT0_MASK                             0x00000700
#define TSTMP_FIFO_EMPTYTHRESH_PRT0_RD(src)           (((src) & 0x00000700)>>8)
#define TSTMP_FIFO_EMPTYTHRESH_PRT0_WR(src)      (((u32)(src)<<8) & 0x00000700)
#define TSTMP_FIFO_EMPTYTHRESH_PRT0_SET(dst,src) \
                       (((dst) & ~0x00000700) | (((u32)(src)<<8) & 0x00000700))
/*	 Fields tstmp_fifo_fullthresh_prt0	 */
#define TSTMP_FIFO_FULLTHRESH_PRT0_WIDTH                                      3
#define TSTMP_FIFO_FULLTHRESH_PRT0_SHIFT                                      5
#define TSTMP_FIFO_FULLTHRESH_PRT0_MASK                              0x000000e0
#define TSTMP_FIFO_FULLTHRESH_PRT0_RD(src)            (((src) & 0x000000e0)>>5)
#define TSTMP_FIFO_FULLTHRESH_PRT0_WR(src)       (((u32)(src)<<5) & 0x000000e0)
#define TSTMP_FIFO_FULLTHRESH_PRT0_SET(dst,src) \
                       (((dst) & ~0x000000e0) | (((u32)(src)<<5) & 0x000000e0))
/*	 Fields tstmp_fifo_ramdepth_prt0	 */
#define TSTMP_FIFO_RAMDEPTH_PRT0_WIDTH                                        3
#define TSTMP_FIFO_RAMDEPTH_PRT0_SHIFT                                        2
#define TSTMP_FIFO_RAMDEPTH_PRT0_MASK                                0x0000001c
#define TSTMP_FIFO_RAMDEPTH_PRT0_RD(src)              (((src) & 0x0000001c)>>2)
#define TSTMP_FIFO_RAMDEPTH_PRT0_WR(src)         (((u32)(src)<<2) & 0x0000001c)
#define TSTMP_FIFO_RAMDEPTH_PRT0_SET(dst,src) \
                       (((dst) & ~0x0000001c) | (((u32)(src)<<2) & 0x0000001c))
/*	 Fields ffdiv_clk_sel	 */
#define FFDIV_CLK_SEL_WIDTH                                                   1
#define FFDIV_CLK_SEL_SHIFT                                                   1
#define FFDIV_CLK_SEL_MASK                                           0x00000002
#define FFDIV_CLK_SEL_RD(src)                         (((src) & 0x00000002)>>1)
#define FFDIV_CLK_SEL_WR(src)                    (((u32)(src)<<1) & 0x00000002)
#define FFDIV_CLK_SEL_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields ptp_clk_sel	 */
#define PTP_CLK_SEL_WIDTH                                                     1
#define PTP_CLK_SEL_SHIFT                                                     0
#define PTP_CLK_SEL_MASK                                             0x00000001
#define PTP_CLK_SEL_RD(src)                              (((src) & 0x00000001))
#define PTP_CLK_SEL_WR(src)                         (((u32)(src)) & 0x00000001)
#define PTP_CLK_SEL_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register PTP_Rx_tmstmp_prt0	*/ 
/*	 Fields ptp_curr_sample_num	 */
#define PTP_CURR_SAMPLE_NUM0_WIDTH                                            2
#define PTP_CURR_SAMPLE_NUM0_SHIFT                                           30
#define PTP_CURR_SAMPLE_NUM0_MASK                                    0xc0000000
#define PTP_CURR_SAMPLE_NUM0_RD(src)                 (((src) & 0xc0000000)>>30)
#define PTP_CURR_SAMPLE_NUM0_SET(dst,src) \
                      (((dst) & ~0xc0000000) | (((u32)(src)<<30) & 0xc0000000))
/*	 Fields ptp_last_tstmp	 */
#define PTP_LAST_TSTMP0_WIDTH                                                30
#define PTP_LAST_TSTMP0_SHIFT                                                 0
#define PTP_LAST_TSTMP0_MASK                                         0x3fffffff
#define PTP_LAST_TSTMP0_RD(src)                          (((src) & 0x3fffffff))
#define PTP_LAST_TSTMP0_SET(dst,src) \
                          (((dst) & ~0x3fffffff) | (((u32)(src)) & 0x3fffffff))

/*	Register PTP_Tx_tmstmp0_prt0	*/ 
/*	 Fields ptp_ins_tstmp	 */
#define PTP_INS_TSTMP0_WIDTH                                                 32
#define PTP_INS_TSTMP0_SHIFT                                                  0
#define PTP_INS_TSTMP0_MASK                                          0xffffffff
#define PTP_INS_TSTMP0_RD(src)                           (((src) & 0xffffffff))
#define PTP_INS_TSTMP0_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register PTP_Tx_tmstmp1_prt0	*/ 
/*	 Fields ptp_ins_tstmp	 */
#define PTP_INS_TSTMP0_F1_WIDTH                                              32
#define PTP_INS_TSTMP0_F1_SHIFT                                               0
#define PTP_INS_TSTMP0_F1_MASK                                       0xffffffff
#define PTP_INS_TSTMP0_F1_RD(src)                        (((src) & 0xffffffff))
#define PTP_INS_TSTMP0_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register PTP_Tx_correction_prt0	*/ 
/*	 Fields value	 */
#define VALUE0_WIDTH                                                         32
#define VALUE0_SHIFT                                                          0
#define VALUE0_MASK                                                  0xffffffff
#define VALUE0_RD(src)                                   (((src) & 0xffffffff))
#define VALUE0_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register PTP_Rx_tmstmp_prt1	*/ 
/*	 Fields ptp_curr_sample_num	 */
#define PTP_CURR_SAMPLE_NUM1_WIDTH                                            2
#define PTP_CURR_SAMPLE_NUM1_SHIFT                                           30
#define PTP_CURR_SAMPLE_NUM1_MASK                                    0xc0000000
#define PTP_CURR_SAMPLE_NUM1_RD(src)                 (((src) & 0xc0000000)>>30)
#define PTP_CURR_SAMPLE_NUM1_SET(dst,src) \
                      (((dst) & ~0xc0000000) | (((u32)(src)<<30) & 0xc0000000))
/*	 Fields ptp_last_tstmp	 */
#define PTP_LAST_TSTMP1_WIDTH                                                30
#define PTP_LAST_TSTMP1_SHIFT                                                 0
#define PTP_LAST_TSTMP1_MASK                                         0x3fffffff
#define PTP_LAST_TSTMP1_RD(src)                          (((src) & 0x3fffffff))
#define PTP_LAST_TSTMP1_SET(dst,src) \
                          (((dst) & ~0x3fffffff) | (((u32)(src)) & 0x3fffffff))

/*	Register PTP_Tx_tmstmp0_prt1	*/ 
/*	 Fields ptp_ins_tstmp	 */
#define PTP_INS_TSTMP1_WIDTH                                                 32
#define PTP_INS_TSTMP1_SHIFT                                                  0
#define PTP_INS_TSTMP1_MASK                                          0xffffffff
#define PTP_INS_TSTMP1_RD(src)                           (((src) & 0xffffffff))
#define PTP_INS_TSTMP1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register PTP_Tx_tmstmp1_prt1	*/ 
/*	 Fields ptp_ins_tstmp	 */
#define PTP_INS_TSTMP1_F1_WIDTH                                              32
#define PTP_INS_TSTMP1_F1_SHIFT                                               0
#define PTP_INS_TSTMP1_F1_MASK                                       0xffffffff
#define PTP_INS_TSTMP1_F1_RD(src)                        (((src) & 0xffffffff))
#define PTP_INS_TSTMP1_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register PTP_Tx_correction_prt1	*/ 
/*	 Fields value	 */
#define VALUE1_WIDTH                                                         32
#define VALUE1_SHIFT                                                          0
#define VALUE1_MASK                                                  0xffffffff
#define VALUE1_RD(src)                                   (((src) & 0xffffffff))
#define VALUE1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register Debug_Reg1	*/ 
/*	 Fields cfg_rx_port1_duplicate	 */
#define CFG_RX_PORT1_DUPLICATE1_WIDTH                                         1
#define CFG_RX_PORT1_DUPLICATE1_SHIFT                                         2
#define CFG_RX_PORT1_DUPLICATE1_MASK                                 0x00000004
#define CFG_RX_PORT1_DUPLICATE1_RD(src)               (((src) & 0x00000004)>>2)
#define CFG_RX_PORT1_DUPLICATE1_WR(src)          (((u32)(src)<<2) & 0x00000004)
#define CFG_RX_PORT1_DUPLICATE1_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields CfgRx2TxClkLoopBackEn	 */
#define CFGRX2TXCLKLOOPBACKEN1_WIDTH                                          2
#define CFGRX2TXCLKLOOPBACKEN1_SHIFT                                          0
#define CFGRX2TXCLKLOOPBACKEN1_MASK                                  0x00000003
#define CFGRX2TXCLKLOOPBACKEN1_RD(src)                   (((src) & 0x00000003))
#define CFGRX2TXCLKLOOPBACKEN1_WR(src)              (((u32)(src)) & 0x00000003)
#define CFGRX2TXCLKLOOPBACKEN1_SET(dst,src) \
                          (((dst) & ~0x00000003) | (((u32)(src)) & 0x00000003))

/*	Register Debug_Reg	*/ 
/*	 Fields cfg_diag_bus_gray_ungray_sel	 */
#define CFG_DIAG_BUS_GRAY_UNGRAY_SEL_WIDTH                                    1
#define CFG_DIAG_BUS_GRAY_UNGRAY_SEL_SHIFT                                   31
#define CFG_DIAG_BUS_GRAY_UNGRAY_SEL_MASK                            0x80000000
#define CFG_DIAG_BUS_GRAY_UNGRAY_SEL_RD(src)         (((src) & 0x80000000)>>31)
#define CFG_DIAG_BUS_GRAY_UNGRAY_SEL_WR(src)    (((u32)(src)<<31) & 0x80000000)
#define CFG_DIAG_BUS_GRAY_UNGRAY_SEL_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields cfg_debug_txbuf_igr_rd_gnt_lmt	 */
#define CFG_DEBUG_TXBUF_IGR_RD_GNT_LMT_WIDTH                                  1
#define CFG_DEBUG_TXBUF_IGR_RD_GNT_LMT_SHIFT                                 30
#define CFG_DEBUG_TXBUF_IGR_RD_GNT_LMT_MASK                          0x40000000
#define CFG_DEBUG_TXBUF_IGR_RD_GNT_LMT_RD(src)       (((src) & 0x40000000)>>30)
#define CFG_DEBUG_TXBUF_IGR_RD_GNT_LMT_WR(src)  (((u32)(src)<<30) & 0x40000000)
#define CFG_DEBUG_TXBUF_IGR_RD_GNT_LMT_SET(dst,src) \
                      (((dst) & ~0x40000000) | (((u32)(src)<<30) & 0x40000000))
/*	 Fields cfg_debug_txbuf_igr_wr_gnt_lmt	 */
#define CFG_DEBUG_TXBUF_IGR_WR_GNT_LMT_WIDTH                                  1
#define CFG_DEBUG_TXBUF_IGR_WR_GNT_LMT_SHIFT                                 29
#define CFG_DEBUG_TXBUF_IGR_WR_GNT_LMT_MASK                          0x20000000
#define CFG_DEBUG_TXBUF_IGR_WR_GNT_LMT_RD(src)       (((src) & 0x20000000)>>29)
#define CFG_DEBUG_TXBUF_IGR_WR_GNT_LMT_WR(src)  (((u32)(src)<<29) & 0x20000000)
#define CFG_DEBUG_TXBUF_IGR_WR_GNT_LMT_SET(dst,src) \
                      (((dst) & ~0x20000000) | (((u32)(src)<<29) & 0x20000000))
/*	 Fields cfg_debug_txbuf_rd_grant_limit	 */
#define CFG_DEBUG_TXBUF_RD_GRANT_LIMIT_WIDTH                                 12
#define CFG_DEBUG_TXBUF_RD_GRANT_LIMIT_SHIFT                                 17
#define CFG_DEBUG_TXBUF_RD_GRANT_LIMIT_MASK                          0x1ffe0000
#define CFG_DEBUG_TXBUF_RD_GRANT_LIMIT_RD(src)       (((src) & 0x1ffe0000)>>17)
#define CFG_DEBUG_TXBUF_RD_GRANT_LIMIT_WR(src)  (((u32)(src)<<17) & 0x1ffe0000)
#define CFG_DEBUG_TXBUF_RD_GRANT_LIMIT_SET(dst,src) \
                      (((dst) & ~0x1ffe0000) | (((u32)(src)<<17) & 0x1ffe0000))
/*	 Fields cfg_debug_txbuf_wr_grant_limit	 */
#define CFG_DEBUG_TXBUF_WR_GRANT_LIMIT_WIDTH                                 12
#define CFG_DEBUG_TXBUF_WR_GRANT_LIMIT_SHIFT                                  5
#define CFG_DEBUG_TXBUF_WR_GRANT_LIMIT_MASK                          0x0001ffe0
#define CFG_DEBUG_TXBUF_WR_GRANT_LIMIT_RD(src)        (((src) & 0x0001ffe0)>>5)
#define CFG_DEBUG_TXBUF_WR_GRANT_LIMIT_WR(src)   (((u32)(src)<<5) & 0x0001ffe0)
#define CFG_DEBUG_TXBUF_WR_GRANT_LIMIT_SET(dst,src) \
                       (((dst) & ~0x0001ffe0) | (((u32)(src)<<5) & 0x0001ffe0))
/*	 Fields cfg_debug_rxtsotxbuf_loopback	 */
#define CFG_DEBUG_RXTSOTXBUF_LOOPBACK_WIDTH                                   1
#define CFG_DEBUG_RXTSOTXBUF_LOOPBACK_SHIFT                                   4
#define CFG_DEBUG_RXTSOTXBUF_LOOPBACK_MASK                           0x00000010
#define CFG_DEBUG_RXTSOTXBUF_LOOPBACK_RD(src)         (((src) & 0x00000010)>>4)
#define CFG_DEBUG_RXTSOTXBUF_LOOPBACK_WR(src)    (((u32)(src)<<4) & 0x00000010)
#define CFG_DEBUG_RXTSOTXBUF_LOOPBACK_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields cfg_bypass_timestamp_ins	 */
#define CFG_BYPASS_TIMESTAMP_INS_WIDTH                                        1
#define CFG_BYPASS_TIMESTAMP_INS_SHIFT                                        3
#define CFG_BYPASS_TIMESTAMP_INS_MASK                                0x00000008
#define CFG_BYPASS_TIMESTAMP_INS_RD(src)              (((src) & 0x00000008)>>3)
#define CFG_BYPASS_TIMESTAMP_INS_WR(src)         (((u32)(src)<<3) & 0x00000008)
#define CFG_BYPASS_TIMESTAMP_INS_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields cfg_bypass_unisec_tx	 */
#define CFG_BYPASS_UNISEC_TX_WIDTH                                            1
#define CFG_BYPASS_UNISEC_TX_SHIFT                                            2
#define CFG_BYPASS_UNISEC_TX_MASK                                    0x00000004
#define CFG_BYPASS_UNISEC_TX_RD(src)                  (((src) & 0x00000004)>>2)
#define CFG_BYPASS_UNISEC_TX_WR(src)             (((u32)(src)<<2) & 0x00000004)
#define CFG_BYPASS_UNISEC_TX_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields cfg_bypass_unisec_rx	 */
#define CFG_BYPASS_UNISEC_RX_WIDTH                                            1
#define CFG_BYPASS_UNISEC_RX_SHIFT                                            1
#define CFG_BYPASS_UNISEC_RX_MASK                                    0x00000002
#define CFG_BYPASS_UNISEC_RX_RD(src)                  (((src) & 0x00000002)>>1)
#define CFG_BYPASS_UNISEC_RX_WR(src)             (((u32)(src)<<1) & 0x00000002)
#define CFG_BYPASS_UNISEC_RX_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields cfg_debug_rxtxbuf_loopback	 */
#define CFG_DEBUG_RXTXBUF_LOOPBACK_WIDTH                                      1
#define CFG_DEBUG_RXTXBUF_LOOPBACK_SHIFT                                      0
#define CFG_DEBUG_RXTXBUF_LOOPBACK_MASK                              0x00000001
#define CFG_DEBUG_RXTXBUF_LOOPBACK_RD(src)               (((src) & 0x00000001))
#define CFG_DEBUG_RXTXBUF_LOOPBACK_WR(src)          (((u32)(src)) & 0x00000001)
#define CFG_DEBUG_RXTXBUF_LOOPBACK_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register mpic_debug_reg	*/ 
/*	 Fields sw_intr_debug	 */
#define SW_INTR_DEBUG_WIDTH                                                   5
#define SW_INTR_DEBUG_SHIFT                                                   0
#define SW_INTR_DEBUG_MASK                                           0x0000001f
#define SW_INTR_DEBUG_RD(src)                            (((src) & 0x0000001f))
#define SW_INTR_DEBUG_WR(src)                       (((u32)(src)) & 0x0000001f)
#define SW_INTR_DEBUG_SET(dst,src) \
                          (((dst) & ~0x0000001f) | (((u32)(src)) & 0x0000001f))

/*	Register tso_enet_stat_reg_port0	*/ 
/*	 Fields byte_cnt	 */
#define BYTE_CNT0_WIDTH                                                      16
#define BYTE_CNT0_SHIFT                                                      16
#define BYTE_CNT0_MASK                                               0xffff0000
#define BYTE_CNT0_RD(src)                            (((src) & 0xffff0000)>>16)
#define BYTE_CNT0_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields pkt_cnt	 */
#define PKT_CNT0_WIDTH                                                       16
#define PKT_CNT0_SHIFT                                                        0
#define PKT_CNT0_MASK                                                0x0000ffff
#define PKT_CNT0_RD(src)                                 (((src) & 0x0000ffff))
#define PKT_CNT0_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register txb_enet_stat_reg_port0	*/ 
/*	 Fields byte_cnt	 */
#define BYTE_CNT0_F1_WIDTH                                                   16
#define BYTE_CNT0_F1_SHIFT                                                   16
#define BYTE_CNT0_F1_MASK                                            0xffff0000
#define BYTE_CNT0_F1_RD(src)                         (((src) & 0xffff0000)>>16)
#define BYTE_CNT0_F1_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields pkt_cnt	 */
#define PKT_CNT0_F1_WIDTH                                                    16
#define PKT_CNT0_F1_SHIFT                                                     0
#define PKT_CNT0_F1_MASK                                             0x0000ffff
#define PKT_CNT0_F1_RD(src)                              (((src) & 0x0000ffff))
#define PKT_CNT0_F1_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register macin_enet_stat_reg_port0	*/ 
/*	 Fields byte_cnt	 */
#define BYTE_CNT0_F2_WIDTH                                                   16
#define BYTE_CNT0_F2_SHIFT                                                   16
#define BYTE_CNT0_F2_MASK                                            0xffff0000
#define BYTE_CNT0_F2_RD(src)                         (((src) & 0xffff0000)>>16)
#define BYTE_CNT0_F2_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields pkt_cnt	 */
#define PKT_CNT0_F2_WIDTH                                                    16
#define PKT_CNT0_F2_SHIFT                                                     0
#define PKT_CNT0_F2_MASK                                             0x0000ffff
#define PKT_CNT0_F2_RD(src)                              (((src) & 0x0000ffff))
#define PKT_CNT0_F2_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register macout_enet_stat_reg_port0	*/ 
/*	 Fields byte_cnt	 */
#define BYTE_CNT0_F3_WIDTH                                                   16
#define BYTE_CNT0_F3_SHIFT                                                   16
#define BYTE_CNT0_F3_MASK                                            0xffff0000
#define BYTE_CNT0_F3_RD(src)                         (((src) & 0xffff0000)>>16)
#define BYTE_CNT0_F3_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields pkt_cnt	 */
#define PKT_CNT0_F3_WIDTH                                                    16
#define PKT_CNT0_F3_SHIFT                                                     0
#define PKT_CNT0_F3_MASK                                             0x0000ffff
#define PKT_CNT0_F3_RD(src)                              (((src) & 0x0000ffff))
#define PKT_CNT0_F3_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register rxbin_enet_stat_reg_port0	*/ 
/*	 Fields byte_cnt	 */
#define BYTE_CNT0_F4_WIDTH                                                   16
#define BYTE_CNT0_F4_SHIFT                                                   16
#define BYTE_CNT0_F4_MASK                                            0xffff0000
#define BYTE_CNT0_F4_RD(src)                         (((src) & 0xffff0000)>>16)
#define BYTE_CNT0_F4_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields pkt_cnt	 */
#define PKT_CNT0_F4_WIDTH                                                    16
#define PKT_CNT0_F4_SHIFT                                                     0
#define PKT_CNT0_F4_MASK                                             0x0000ffff
#define PKT_CNT0_F4_RD(src)                              (((src) & 0x0000ffff))
#define PKT_CNT0_F4_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register rsif_enet_stat_reg_port0	*/ 
/*	 Fields byte_cnt	 */
#define BYTE_CNT0_F5_WIDTH                                                   16
#define BYTE_CNT0_F5_SHIFT                                                   16
#define BYTE_CNT0_F5_MASK                                            0xffff0000
#define BYTE_CNT0_F5_RD(src)                         (((src) & 0xffff0000)>>16)
#define BYTE_CNT0_F5_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields pkt_cnt	 */
#define PKT_CNT0_F5_WIDTH                                                    16
#define PKT_CNT0_F5_SHIFT                                                     0
#define PKT_CNT0_F5_MASK                                             0x0000ffff
#define PKT_CNT0_F5_RD(src)                              (((src) & 0x0000ffff))
#define PKT_CNT0_F5_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register tso_enet_stat_reg_port1	*/ 
/*	 Fields byte_cnt	 */
#define BYTE_CNT1_WIDTH                                                      16
#define BYTE_CNT1_SHIFT                                                      16
#define BYTE_CNT1_MASK                                               0xffff0000
#define BYTE_CNT1_RD(src)                            (((src) & 0xffff0000)>>16)
#define BYTE_CNT1_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields pkt_cnt	 */
#define PKT_CNT1_WIDTH                                                       16
#define PKT_CNT1_SHIFT                                                        0
#define PKT_CNT1_MASK                                                0x0000ffff
#define PKT_CNT1_RD(src)                                 (((src) & 0x0000ffff))
#define PKT_CNT1_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register txb_enet_stat_reg_port1	*/ 
/*	 Fields byte_cnt	 */
#define BYTE_CNT1_F1_WIDTH                                                   16
#define BYTE_CNT1_F1_SHIFT                                                   16
#define BYTE_CNT1_F1_MASK                                            0xffff0000
#define BYTE_CNT1_F1_RD(src)                         (((src) & 0xffff0000)>>16)
#define BYTE_CNT1_F1_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields pkt_cnt	 */
#define PKT_CNT1_F1_WIDTH                                                    16
#define PKT_CNT1_F1_SHIFT                                                     0
#define PKT_CNT1_F1_MASK                                             0x0000ffff
#define PKT_CNT1_F1_RD(src)                              (((src) & 0x0000ffff))
#define PKT_CNT1_F1_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register macin_enet_stat_reg_port1	*/ 
/*	 Fields byte_cnt	 */
#define BYTE_CNT1_F2_WIDTH                                                   16
#define BYTE_CNT1_F2_SHIFT                                                   16
#define BYTE_CNT1_F2_MASK                                            0xffff0000
#define BYTE_CNT1_F2_RD(src)                         (((src) & 0xffff0000)>>16)
#define BYTE_CNT1_F2_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields pkt_cnt	 */
#define PKT_CNT1_F2_WIDTH                                                    16
#define PKT_CNT1_F2_SHIFT                                                     0
#define PKT_CNT1_F2_MASK                                             0x0000ffff
#define PKT_CNT1_F2_RD(src)                              (((src) & 0x0000ffff))
#define PKT_CNT1_F2_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register macout_enet_stat_reg_port1	*/ 
/*	 Fields byte_cnt	 */
#define BYTE_CNT1_F3_WIDTH                                                   16
#define BYTE_CNT1_F3_SHIFT                                                   16
#define BYTE_CNT1_F3_MASK                                            0xffff0000
#define BYTE_CNT1_F3_RD(src)                         (((src) & 0xffff0000)>>16)
#define BYTE_CNT1_F3_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields pkt_cnt	 */
#define PKT_CNT1_F3_WIDTH                                                    16
#define PKT_CNT1_F3_SHIFT                                                     0
#define PKT_CNT1_F3_MASK                                             0x0000ffff
#define PKT_CNT1_F3_RD(src)                              (((src) & 0x0000ffff))
#define PKT_CNT1_F3_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register rxbin_enet_stat_reg_port1	*/ 
/*	 Fields byte_cnt	 */
#define BYTE_CNT1_F4_WIDTH                                                   16
#define BYTE_CNT1_F4_SHIFT                                                   16
#define BYTE_CNT1_F4_MASK                                            0xffff0000
#define BYTE_CNT1_F4_RD(src)                         (((src) & 0xffff0000)>>16)
#define BYTE_CNT1_F4_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields pkt_cnt	 */
#define PKT_CNT1_F4_WIDTH                                                    16
#define PKT_CNT1_F4_SHIFT                                                     0
#define PKT_CNT1_F4_MASK                                             0x0000ffff
#define PKT_CNT1_F4_RD(src)                              (((src) & 0x0000ffff))
#define PKT_CNT1_F4_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register rsif_enet_stat_reg_port1	*/ 
/*	 Fields byte_cnt	 */
#define BYTE_CNT1_F5_WIDTH                                                   16
#define BYTE_CNT1_F5_SHIFT                                                   16
#define BYTE_CNT1_F5_MASK                                            0xffff0000
#define BYTE_CNT1_F5_RD(src)                         (((src) & 0xffff0000)>>16)
#define BYTE_CNT1_F5_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields pkt_cnt	 */
#define PKT_CNT1_F5_WIDTH                                                    16
#define PKT_CNT1_F5_SHIFT                                                     0
#define PKT_CNT1_F5_MASK                                             0x0000ffff
#define PKT_CNT1_F5_RD(src)                              (((src) & 0x0000ffff))
#define PKT_CNT1_F5_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register enet_spare_cfg_reg	*/ 
/*	 Fields spare_cfg	 */
#define SPARE_CFG_WIDTH                                                      22
#define SPARE_CFG_SHIFT                                                      10
#define SPARE_CFG_MASK                                               0xfffffc00
#define SPARE_CFG_RD(src)                            (((src) & 0xfffffc00)>>10)
#define SPARE_CFG_WR(src)                       (((u32)(src)<<10) & 0xfffffc00)
#define SPARE_CFG_SET(dst,src) \
                      (((dst) & ~0xfffffc00) | (((u32)(src)<<10) & 0xfffffc00))
/*	 Fields enet_idle_timer	 */
#define ENET_IDLE_TIMER_WIDTH                                                10
#define ENET_IDLE_TIMER_SHIFT                                                 0
#define ENET_IDLE_TIMER_MASK                                         0x000003ff
#define ENET_IDLE_TIMER_RD(src)                          (((src) & 0x000003ff))
#define ENET_IDLE_TIMER_WR(src)                     (((u32)(src)) & 0x000003ff)
#define ENET_IDLE_TIMER_SET(dst,src) \
                          (((dst) & ~0x000003ff) | (((u32)(src)) & 0x000003ff))

/*	Register enet_spare_status_reg	*/ 
/*	 Fields spare_cfg	 */
#define SPARE_CFG_F1_WIDTH                                                   32
#define SPARE_CFG_F1_SHIFT                                                    0
#define SPARE_CFG_F1_MASK                                            0xffffffff
#define SPARE_CFG_F1_RD(src)                             (((src) & 0xffffffff))
#define SPARE_CFG_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Global Base Address	*/
#define ENET_FFDIV_CSR_BASE_ADDR			0xddd8a0800ULL

/*    Address ENET_FFDIV_CSR  Registers */
#define FFDIV_VERSION_ADDR                                           0x00000000
#define FFDIV_VERSION_DEFAULT                                        0x80000000
#define FFDIV_CONTROL_ADDR                                           0x00000004
#define FFDIV_CONTROL_DEFAULT                                        0x00000000
#define FFDIV_EVENT_ADDR                                             0x00000008
#define FFDIV_EVENT_DEFAULT                                          0x00000000
#define FFDIV_EVENTMASK_ADDR                                         0x0000000c
#define FFDIV_INT_ADDR                                               0x00000010
#define FFDIV_INT_DEFAULT                                            0x00000000
#define FFDIV_INTMASK_ADDR                                           0x00000014
#define FFDIV_REFDIV_ADDR                                            0x00000018
#define FFDIV_REFDIV_DEFAULT                                         0x00000000
#define FFDIV_FBINTDIV_NOMINAL_ADDR                                  0x0000001c
#define FFDIV_FBINTDIV_NOMINAL_DEFAULT                               0x40000000
#define FFDIV_FBFRACDIV_NOMINAL_ADDR                                 0x00000020
#define FFDIV_FBFRACDIV_NOMINAL_DEFAULT                              0x00000000
#define FFDIV_FBINTDIV_SECONDARY_ADDR                                0x00000024
#define FFDIV_FBINTDIV_SECONDARY_DEFAULT                             0x40000000
#define FFDIV_FBFRACDIV_SECONDARY_ADDR                               0x00000028
#define FFDIV_FBFRACDIV_SECONDARY_DEFAULT                            0x00000000
#define FFDIV_PITCOUNT_ADDR                                          0x0000002c
#define FFDIV_PITCOUNT_DEFAULT                                       0x00000000
#define FFDIV_PITRELOAD_LOW_ADDR                                     0x00000030
#define FFDIV_PITRELOAD_LOW_DEFAULT                                  0x00000000
#define FFDIV_PITRELOAD_HIGH_ADDR                                    0x00000034
#define FFDIV_PITRELOAD_HIGH_DEFAULT                                 0x00000000

/*	Register FFDiv_Version	*/ 
/*	 Fields REV	 */
#define REV_WIDTH                                                             4
#define REV_SHIFT                                                            28
#define REV_MASK                                                     0xf0000000
#define REV_RD(src)                                  (((src) & 0xf0000000)>>28)
#define REV_SET(dst,src) \
                      (((dst) & ~0xf0000000) | (((u32)(src)<<28) & 0xf0000000))

/*	Register FFDiv_Control	*/ 
/*	 Fields RST	 */
#define RST_WIDTH                                                             1
#define RST_SHIFT                                                            31
#define RST_MASK                                                     0x80000000
#define RST_RD(src)                                  (((src) & 0x80000000)>>31)
#define RST_WR(src)                             (((u32)(src)<<31) & 0x80000000)
#define RST_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields PITAREN	 */
#define PITAREN_WIDTH                                                         1
#define PITAREN_SHIFT                                                        27
#define PITAREN_MASK                                                 0x08000000
#define PITAREN_RD(src)                              (((src) & 0x08000000)>>27)
#define PITAREN_WR(src)                         (((u32)(src)<<27) & 0x08000000)
#define PITAREN_SET(dst,src) \
                      (((dst) & ~0x08000000) | (((u32)(src)<<27) & 0x08000000))
/*	 Fields PITSTATE	 */
#define PITSTATE_WIDTH                                                        1
#define PITSTATE_SHIFT                                                       26
#define PITSTATE_MASK                                                0x04000000
#define PITSTATE_RD(src)                             (((src) & 0x04000000)>>26)
#define PITSTATE_WR(src)                        (((u32)(src)<<26) & 0x04000000)
#define PITSTATE_SET(dst,src) \
                      (((dst) & ~0x04000000) | (((u32)(src)<<26) & 0x04000000))
/*	 Fields PITINTTYPE	 */
#define PITINTTYPE_WIDTH                                                      2
#define PITINTTYPE_SHIFT                                                     24
#define PITINTTYPE_MASK                                              0x03000000
#define PITINTTYPE_RD(src)                           (((src) & 0x03000000)>>24)
#define PITINTTYPE_WR(src)                      (((u32)(src)<<24) & 0x03000000)
#define PITINTTYPE_SET(dst,src) \
                      (((dst) & ~0x03000000) | (((u32)(src)<<24) & 0x03000000))
/*	 Fields REFDIVBP	 */
#define REFDIVBP_WIDTH                                                        1
#define REFDIVBP_SHIFT                                                       23
#define REFDIVBP_MASK                                                0x00800000
#define REFDIVBP_RD(src)                             (((src) & 0x00800000)>>23)
#define REFDIVBP_WR(src)                        (((u32)(src)<<23) & 0x00800000)
#define REFDIVBP_SET(dst,src) \
                      (((dst) & ~0x00800000) | (((u32)(src)<<23) & 0x00800000))
/*	 Fields ORDER	 */
#define ORDER_WIDTH                                                           2
#define ORDER_SHIFT                                                          21
#define ORDER_MASK                                                   0x00600000
#define ORDER_RD(src)                                (((src) & 0x00600000)>>21)
#define ORDER_WR(src)                           (((u32)(src)<<21) & 0x00600000)
#define ORDER_SET(dst,src) \
                      (((dst) & ~0x00600000) | (((u32)(src)<<21) & 0x00600000))
/*	 Fields VCOEN	 */
#define VCOEN_WIDTH                                                           1
#define VCOEN_SHIFT                                                          20
#define VCOEN_MASK                                                   0x00100000
#define VCOEN_RD(src)                                (((src) & 0x00100000)>>20)
#define VCOEN_WR(src)                           (((u32)(src)<<20) & 0x00100000)
#define VCOEN_SET(dst,src) \
                      (((dst) & ~0x00100000) | (((u32)(src)<<20) & 0x00100000))
/*	 Fields PIT_EN	 */
#define PIT_EN_WIDTH                                                          1
#define PIT_EN_SHIFT                                                         19
#define PIT_EN_MASK                                                  0x00080000
#define PIT_EN_RD(src)                               (((src) & 0x00080000)>>19)
#define PIT_EN_WR(src)                          (((u32)(src)<<19) & 0x00080000)
#define PIT_EN_SET(dst,src) \
                      (((dst) & ~0x00080000) | (((u32)(src)<<19) & 0x00080000))

/*	Register FFDiv_Event	*/ 
/*	 Fields PITPEND	 */
#define PITPEND_WIDTH                                                         1
#define PITPEND_SHIFT                                                        31
#define PITPEND_MASK                                                 0x80000000
#define PITPEND_RD(src)                              (((src) & 0x80000000)>>31)
#define PITPEND_WR(src)                         (((u32)(src)<<31) & 0x80000000)
#define PITPEND_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))

/*	Register FFDiv_EventMask	*/
/*    Mask Register Fields PITPENDMask    */
#define PITPENDMASK_WIDTH                                                     1
#define PITPENDMASK_SHIFT                                                    31
#define PITPENDMASK_MASK                                             0x80000000
#define PITPENDMASK_RD(src)                          (((src) & 0x80000000)>>31)
#define PITPENDMASK_WR(src)                     (((u32)(src)<<31) & 0x80000000)
#define PITPENDMASK_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))

/*	Register FFDiv_Int	*/ 
/*	 Fields INT	 */
#define INT_WIDTH                                                             1
#define INT_SHIFT                                                            31
#define INT_MASK                                                     0x80000000
#define INT_RD(src)                                  (((src) & 0x80000000)>>31)
#define INT_WR(src)                             (((u32)(src)<<31) & 0x80000000)
#define INT_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))

/*	Register FFDiv_IntMask	*/
/*    Mask Register Fields INTMask    */
#define INTMASK_WIDTH                                                         1
#define INTMASK_SHIFT                                                        31
#define INTMASK_MASK                                                 0x80000000
#define INTMASK_RD(src)                              (((src) & 0x80000000)>>31)
#define INTMASK_WR(src)                         (((u32)(src)<<31) & 0x80000000)
#define INTMASK_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))

/*	Register FFDiv_RefDiv	*/ 
/*	 Fields REFDIV	 */
#define REFDIV_WIDTH                                                         16
#define REFDIV_SHIFT                                                         16
#define REFDIV_MASK                                                  0xffff0000
#define REFDIV_RD(src)                               (((src) & 0xffff0000)>>16)
#define REFDIV_WR(src)                          (((u32)(src)<<16) & 0xffff0000)
#define REFDIV_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))

/*	Register FFDiv_FbIntDiv_Nominal	*/ 
/*	 Fields INTDIV	 */
#define INTDIV_WIDTH                                                         16
#define INTDIV_SHIFT                                                         16
#define INTDIV_MASK                                                  0xffff0000
#define INTDIV_RD(src)                               (((src) & 0xffff0000)>>16)
#define INTDIV_WR(src)                          (((u32)(src)<<16) & 0xffff0000)
#define INTDIV_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))

/*	Register FFDiv_FbFracDiv_Nominal	*/ 
/*	 Fields FRACDIV	 */
#define FRACDIV_WIDTH                                                        32
#define FRACDIV_SHIFT                                                         0
#define FRACDIV_MASK                                                 0xffffffff
#define FRACDIV_RD(src)                                  (((src) & 0xffffffff))
#define FRACDIV_WR(src)                             (((u32)(src)) & 0xffffffff)
#define FRACDIV_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register FFDiv_FbIntDiv_Secondary	*/ 
/*	 Fields INTDIV	 */
#define INTDIV_F1_WIDTH                                                      16
#define INTDIV_F1_SHIFT                                                      16
#define INTDIV_F1_MASK                                               0xffff0000
#define INTDIV_F1_RD(src)                            (((src) & 0xffff0000)>>16)
#define INTDIV_F1_WR(src)                       (((u32)(src)<<16) & 0xffff0000)
#define INTDIV_F1_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))

/*	Register FFDiv_FbFracDiv_Secondary	*/ 
/*	 Fields FRACDIV	 */
#define FRACDIV_F1_WIDTH                                                     32
#define FRACDIV_F1_SHIFT                                                      0
#define FRACDIV_F1_MASK                                              0xffffffff
#define FRACDIV_F1_RD(src)                               (((src) & 0xffffffff))
#define FRACDIV_F1_WR(src)                          (((u32)(src)) & 0xffffffff)
#define FRACDIV_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register FFDiv_PITCount	*/ 
/*	 Fields CNT	 */
#define CNT_WIDTH                                                            32
#define CNT_SHIFT                                                             0
#define CNT_MASK                                                     0xffffffff
#define CNT_RD(src)                                      (((src) & 0xffffffff))
#define CNT_WR(src)                                 (((u32)(src)) & 0xffffffff)
#define CNT_SET(dst,src) (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register FFDiv_PITReload_low	*/ 
/*	 Fields RLDVAL	 */
#define RLDVAL_WIDTH                                                         32
#define RLDVAL_SHIFT                                                          0
#define RLDVAL_MASK                                                  0xffffffff
#define RLDVAL_RD(src)                                   (((src) & 0xffffffff))
#define RLDVAL_WR(src)                              (((u32)(src)) & 0xffffffff)
#define RLDVAL_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register FFDiv_PITReload_high	*/ 
/*	 Fields RLDVAL	 */
#define RLDVAL_F1_WIDTH                                                      32
#define RLDVAL_F1_SHIFT                                                       0
#define RLDVAL_F1_MASK                                               0xffffffff
#define RLDVAL_F1_RD(src)                                (((src) & 0xffffffff))
#define RLDVAL_F1_WR(src)                           (((u32)(src)) & 0xffffffff)
#define RLDVAL_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Global Base Address	*/
#define ENET_MAC_CSR_BASE_ADDR			0xddd8a2800ULL

/*    Address ENET_MAC_CSR  Registers */
#define RGMII_REG_0_ADDR                                             0x00000000
#define RGMII_REG_0_DEFAULT                                          0x01000000
#define RGMII_REG_1_ADDR                                             0x00000004
#define RGMII_REG_1_DEFAULT                                          0x01000000
#define EEE_RX_LPI_DETECT_0_ADDR                                     0x00000100
#define EEE_RX_LPI_DETECT_0_DEFAULT                                  0x00000201
#define EEE_RX_LPI_DETECT_1_ADDR                                     0x00000104
#define EEE_RX_LPI_DETECT_1_DEFAULT                                  0x00000201
#define EEE_RX_LPI_DETECT_INTR_0_ADDR                                0x000001f0
#define EEE_RX_LPI_DETECT_INTR_0_DEFAULT                             0x00000000
#define EEE_RX_LPI_DETECT_INTR_0MASK_ADDR                            0x000001f4
#define EEE_RX_LPI_DETECT_INTR_1_ADDR                                0x000001f8
#define EEE_RX_LPI_DETECT_INTR_1_DEFAULT                             0x00000000
#define EEE_RX_LPI_DETECT_INTR_1MASK_ADDR                            0x000001fc
#define HDRPRS_CONFIG0_REG_0_ADDR                                    0x00000200
#define HDRPRS_CONFIG0_REG_0_DEFAULT                                 0x00000000
#define HDRPRS_CONFIG1_REG_0_ADDR                                    0x00000204
#define HDRPRS_CONFIG1_REG_0_DEFAULT                                 0x00001111
#define HDRPRS_CONFIG2_REG_0_ADDR                                    0x00000208
#define HDRPRS_CONFIG2_REG_0_DEFAULT                                 0x00000000
#define HDRPRS_CONFIG0_REG_1_ADDR                                    0x0000020c
#define HDRPRS_CONFIG0_REG_1_DEFAULT                                 0x00000000
#define HDRPRS_CONFIG1_REG_1_ADDR                                    0x00000210
#define HDRPRS_CONFIG1_REG_1_DEFAULT                                 0x00001111
#define HDRPRS_CONFIG2_REG_1_ADDR                                    0x00000214
#define HDRPRS_CONFIG2_REG_1_DEFAULT                                 0x00000000
#define HDRPRS_CONFIG3_REG_0_ADDR                                    0x00000218
#define HDRPRS_CONFIG3_REG_0_DEFAULT                                 0x00000000
#define HDRPRS_CONFIG3_REG_1_ADDR                                    0x0000021c
#define HDRPRS_CONFIG3_REG_1_DEFAULT                                 0x00000000
#define CSR_ECM_CFG_0_ADDR                                           0x00000220
#define CSR_ECM_CFG_0_DEFAULT                                        0x00000005
#define CSR_ECM_CFG_1_ADDR                                           0x00000224
#define CSR_ECM_CFG_1_DEFAULT                                        0x00000005
#define CSR_ECM_SWPF_0_ADDR                                          0x00000228
#define CSR_ECM_SWPF_0_DEFAULT                                       0x00000000
#define CSR_ECM_SWPF_1_ADDR                                          0x0000022c
#define CSR_ECM_SWPF_1_DEFAULT                                       0x00000000
#define CSR_MULTI_DPF_0_ADDR                                         0x00000230
#define CSR_MULTI_DPF_0_DEFAULT                                      0x00000000
#define CSR_MULTI_DPF_1_ADDR                                         0x00000234
#define CSR_MULTI_DPF_1_DEFAULT                                      0x00000000
#define CSR_ECM_RSH_0_ADDR                                           0x00000238
#define CSR_ECM_RSH_0_DEFAULT                                        0x00000000
#define CSR_ECM_RSH_1_ADDR                                           0x0000023c
#define CSR_ECM_RSH_1_DEFAULT                                        0x00000000
#define PFC_CONFIG0_REG_0_ADDR                                       0x00000300
#define PFC_CONFIG0_REG_0_DEFAULT                                    0x00000000
#define PFC_CONFIG1_REG_0_ADDR                                       0x00000304
#define PFC_CONFIG1_REG_0_DEFAULT                                    0x00000000
#define PFC_CONFIG2_REG_0_ADDR                                       0x00000308
#define PFC_CONFIG2_REG_0_DEFAULT                                    0x00000000
#define PFC_CONFIG3_REG_0_ADDR                                       0x0000030c
#define PFC_CONFIG3_REG_0_DEFAULT                                    0x00000000
#define PFC_CONFIG4_REG_0_ADDR                                       0x00000310
#define PFC_CONFIG4_REG_0_DEFAULT                                    0x00000000
#define PFC_CONFIG5_REG_0_ADDR                                       0x00000314
#define PFC_CONFIG5_REG_0_DEFAULT                                    0x00000000
#define PFC_CONFIG6_REG_0_ADDR                                       0x00000318
#define PFC_CONFIG6_REG_0_DEFAULT                                    0x00000000
#define PFC_CONFIG7_REG_0_ADDR                                       0x0000031c
#define PFC_CONFIG7_REG_0_DEFAULT                                    0x00000000
#define PFC_CONFIG8_REG_0_ADDR                                       0x00000320
#define PFC_CONFIG8_REG_0_DEFAULT                                    0x00000000
#define PFC_CONFIG9_REG_0_ADDR                                       0x00000324
#define PFC_CONFIG9_REG_0_DEFAULT                                    0x00000000
#define PFC_CONFIG10_REG_0_ADDR                                      0x00000328
#define PFC_CONFIG10_REG_0_DEFAULT                                   0x00000000
#define PFC_CONFIG0_REG_1_ADDR                                       0x00000350
#define PFC_CONFIG0_REG_1_DEFAULT                                    0x00000000
#define PFC_CONFIG1_REG_1_ADDR                                       0x00000354
#define PFC_CONFIG1_REG_1_DEFAULT                                    0x00000000
#define PFC_CONFIG2_REG_1_ADDR                                       0x00000358
#define PFC_CONFIG2_REG_1_DEFAULT                                    0x00000000
#define PFC_CONFIG3_REG_1_ADDR                                       0x0000035c
#define PFC_CONFIG3_REG_1_DEFAULT                                    0x00000000
#define PFC_CONFIG4_REG_1_ADDR                                       0x00000360
#define PFC_CONFIG4_REG_1_DEFAULT                                    0x00000000
#define PFC_CONFIG5_REG_1_ADDR                                       0x00000364
#define PFC_CONFIG5_REG_1_DEFAULT                                    0x00000000
#define PFC_CONFIG6_REG_1_ADDR                                       0x00000368
#define PFC_CONFIG6_REG_1_DEFAULT                                    0x00000000
#define PFC_CONFIG7_REG_1_ADDR                                       0x0000036c
#define PFC_CONFIG7_REG_1_DEFAULT                                    0x00000000
#define PFC_CONFIG8_REG_1_ADDR                                       0x00000370
#define PFC_CONFIG8_REG_1_DEFAULT                                    0x00000000
#define PFC_CONFIG9_REG_1_ADDR                                       0x00000374
#define PFC_CONFIG9_REG_1_DEFAULT                                    0x00000000
#define PFC_CONFIG10_REG_1_ADDR                                      0x00000378
#define PFC_CONFIG10_REG_1_DEFAULT                                   0x00000000
#define ICM_CONFIG0_REG_0_ADDR                                       0x00000400
#define ICM_CONFIG0_REG_0_DEFAULT                                    0x0008503f
#define ICM_CONFIG1_REG_0_ADDR                                       0x00000404
#define ICM_CONFIG1_REG_0_DEFAULT                                    0x02103200
#define ICM_CONFIG0_REG_1_ADDR                                       0x00000408
#define ICM_CONFIG0_REG_1_DEFAULT                                    0x0008503f
#define ICM_CONFIG1_REG_1_ADDR                                       0x0000040c
#define ICM_CONFIG1_REG_1_DEFAULT                                    0x02103200
#define ICM_CONFIG2_REG_0_ADDR                                       0x00000410
#define ICM_CONFIG2_REG_0_DEFAULT                                    0x0001000f
#define ICM_CONFIG2_REG_1_ADDR                                       0x00000414
#define ICM_CONFIG2_REG_1_DEFAULT                                    0x0001000f
#define ECM_CONFIG0_REG_0_ADDR                                       0x00000500
#define ECM_CONFIG0_REG_0_DEFAULT                                    0x00000032
#define ECM_CONFIG0_REG_1_ADDR                                       0x00000504
#define ECM_CONFIG0_REG_1_DEFAULT                                    0x00000032
#define ICM_ECM_DROP_COUNT_REG_0_ADDR                                0x00000508
#define ICM_ECM_DROP_COUNT_REG_0_DEFAULT                             0x00000000
#define ICM_ECM_DROP_COUNT_REG_1_ADDR                                0x0000050c
#define ICM_ECM_DROP_COUNT_REG_1_DEFAULT                             0x00000000
#define MAC_FIFO_STS_REG0_ADDR                                       0x00000510
#define MAC_FIFO_STS_REG0_DEFAULT                                    0x000f000f
#define MAC_INT_REG0_ADDR                                            0x00000514
#define MAC_INT_REG0_DEFAULT                                         0x00000000
#define MAC_INT_REG0MASK_ADDR                                        0x00000518
#define MAC_INT_REG1_ADDR                                            0x0000051c
#define MAC_INT_REG1_DEFAULT                                         0x00000000
#define MAC_INT_REG1MASK_ADDR                                        0x00000520


/*	Register RGMII_Reg_0	*/ 
/*	 Fields cfg_txclk_muxsel	 */
#define CFG_TXCLK_MUXSEL0_WIDTH                                               3
#define CFG_TXCLK_MUXSEL0_SHIFT                                              29
#define CFG_TXCLK_MUXSEL0_MASK                                       0xe0000000
#define CFG_TXCLK_MUXSEL0_RD(src)                    (((src) & 0xe0000000)>>29)
#define CFG_TXCLK_MUXSEL0_WR(src)               (((u32)(src)<<29) & 0xe0000000)
#define CFG_TXCLK_MUXSEL0_SET(dst,src) \
                      (((dst) & ~0xe0000000) | (((u32)(src)<<29) & 0xe0000000))
/*	 Fields cfg_rxclk_muxsel	 */
#define CFG_RXCLK_MUXSEL0_WIDTH                                               3
#define CFG_RXCLK_MUXSEL0_SHIFT                                              26
#define CFG_RXCLK_MUXSEL0_MASK                                       0x1c000000
#define CFG_RXCLK_MUXSEL0_RD(src)                    (((src) & 0x1c000000)>>26)
#define CFG_RXCLK_MUXSEL0_WR(src)               (((u32)(src)<<26) & 0x1c000000)
#define CFG_RXCLK_MUXSEL0_SET(dst,src) \
                      (((dst) & ~0x1c000000) | (((u32)(src)<<26) & 0x1c000000))
/*	 Fields cfg_loopback_tx2rx	 */
#define CFG_LOOPBACK_TX2RX0_WIDTH                                             1
#define CFG_LOOPBACK_TX2RX0_SHIFT                                            25
#define CFG_LOOPBACK_TX2RX0_MASK                                     0x02000000
#define CFG_LOOPBACK_TX2RX0_RD(src)                  (((src) & 0x02000000)>>25)
#define CFG_LOOPBACK_TX2RX0_WR(src)             (((u32)(src)<<25) & 0x02000000)
#define CFG_LOOPBACK_TX2RX0_SET(dst,src) \
                      (((dst) & ~0x02000000) | (((u32)(src)<<25) & 0x02000000))
/*	 Fields cfg_speed_125	 */
#define CFG_SPEED_1250_WIDTH                                                  1
#define CFG_SPEED_1250_SHIFT                                                 24
#define CFG_SPEED_1250_MASK                                          0x01000000
#define CFG_SPEED_1250_RD(src)                       (((src) & 0x01000000)>>24)
#define CFG_SPEED_1250_WR(src)                  (((u32)(src)<<24) & 0x01000000)
#define CFG_SPEED_1250_SET(dst,src) \
                      (((dst) & ~0x01000000) | (((u32)(src)<<24) & 0x01000000))
/*	 Fields cfg_flip_nibble_tx	 */
#define CFG_FLIP_NIBBLE_TX0_WIDTH                                             1
#define CFG_FLIP_NIBBLE_TX0_SHIFT                                            23
#define CFG_FLIP_NIBBLE_TX0_MASK                                     0x00800000
#define CFG_FLIP_NIBBLE_TX0_RD(src)                  (((src) & 0x00800000)>>23)
#define CFG_FLIP_NIBBLE_TX0_WR(src)             (((u32)(src)<<23) & 0x00800000)
#define CFG_FLIP_NIBBLE_TX0_SET(dst,src) \
                      (((dst) & ~0x00800000) | (((u32)(src)<<23) & 0x00800000))
/*	 Fields cfg_flip_nibble_rx	 */
#define CFG_FLIP_NIBBLE_RX0_WIDTH                                             1
#define CFG_FLIP_NIBBLE_RX0_SHIFT                                            22
#define CFG_FLIP_NIBBLE_RX0_MASK                                     0x00400000
#define CFG_FLIP_NIBBLE_RX0_RD(src)                  (((src) & 0x00400000)>>22)
#define CFG_FLIP_NIBBLE_RX0_WR(src)             (((u32)(src)<<22) & 0x00400000)
#define CFG_FLIP_NIBBLE_RX0_SET(dst,src) \
                      (((dst) & ~0x00400000) | (((u32)(src)<<22) & 0x00400000))

/*	Register RGMII_Reg_1	*/ 
/*	 Fields cfg_txclk_muxsel	 */
#define CFG_TXCLK_MUXSEL1_WIDTH                                               3
#define CFG_TXCLK_MUXSEL1_SHIFT                                              29
#define CFG_TXCLK_MUXSEL1_MASK                                       0xe0000000
#define CFG_TXCLK_MUXSEL1_RD(src)                    (((src) & 0xe0000000)>>29)
#define CFG_TXCLK_MUXSEL1_WR(src)               (((u32)(src)<<29) & 0xe0000000)
#define CFG_TXCLK_MUXSEL1_SET(dst,src) \
                      (((dst) & ~0xe0000000) | (((u32)(src)<<29) & 0xe0000000))
/*	 Fields cfg_rxclk_muxsel	 */
#define CFG_RXCLK_MUXSEL1_WIDTH                                               3
#define CFG_RXCLK_MUXSEL1_SHIFT                                              26
#define CFG_RXCLK_MUXSEL1_MASK                                       0x1c000000
#define CFG_RXCLK_MUXSEL1_RD(src)                    (((src) & 0x1c000000)>>26)
#define CFG_RXCLK_MUXSEL1_WR(src)               (((u32)(src)<<26) & 0x1c000000)
#define CFG_RXCLK_MUXSEL1_SET(dst,src) \
                      (((dst) & ~0x1c000000) | (((u32)(src)<<26) & 0x1c000000))
/*	 Fields cfg_loopback_tx2rx	 */
#define CFG_LOOPBACK_TX2RX1_WIDTH                                             1
#define CFG_LOOPBACK_TX2RX1_SHIFT                                            25
#define CFG_LOOPBACK_TX2RX1_MASK                                     0x02000000
#define CFG_LOOPBACK_TX2RX1_RD(src)                  (((src) & 0x02000000)>>25)
#define CFG_LOOPBACK_TX2RX1_WR(src)             (((u32)(src)<<25) & 0x02000000)
#define CFG_LOOPBACK_TX2RX1_SET(dst,src) \
                      (((dst) & ~0x02000000) | (((u32)(src)<<25) & 0x02000000))
/*	 Fields cfg_speed_125	 */
#define CFG_SPEED_1251_WIDTH                                                  1
#define CFG_SPEED_1251_SHIFT                                                 24
#define CFG_SPEED_1251_MASK                                          0x01000000
#define CFG_SPEED_1251_RD(src)                       (((src) & 0x01000000)>>24)
#define CFG_SPEED_1251_WR(src)                  (((u32)(src)<<24) & 0x01000000)
#define CFG_SPEED_1251_SET(dst,src) \
                      (((dst) & ~0x01000000) | (((u32)(src)<<24) & 0x01000000))
/*	 Fields cfg_flip_nibble_tx	 */
#define CFG_FLIP_NIBBLE_TX1_WIDTH                                             1
#define CFG_FLIP_NIBBLE_TX1_SHIFT                                            23
#define CFG_FLIP_NIBBLE_TX1_MASK                                     0x00800000
#define CFG_FLIP_NIBBLE_TX1_RD(src)                  (((src) & 0x00800000)>>23)
#define CFG_FLIP_NIBBLE_TX1_WR(src)             (((u32)(src)<<23) & 0x00800000)
#define CFG_FLIP_NIBBLE_TX1_SET(dst,src) \
                      (((dst) & ~0x00800000) | (((u32)(src)<<23) & 0x00800000))
/*	 Fields cfg_flip_nibble_rx	 */
#define CFG_FLIP_NIBBLE_RX1_WIDTH                                             1
#define CFG_FLIP_NIBBLE_RX1_SHIFT                                            22
#define CFG_FLIP_NIBBLE_RX1_MASK                                     0x00400000
#define CFG_FLIP_NIBBLE_RX1_RD(src)                  (((src) & 0x00400000)>>22)
#define CFG_FLIP_NIBBLE_RX1_WR(src)             (((u32)(src)<<22) & 0x00400000)
#define CFG_FLIP_NIBBLE_RX1_SET(dst,src) \
                      (((dst) & ~0x00400000) | (((u32)(src)<<22) & 0x00400000))

/*	Register EEE_Rx_lpi_detect_0	*/ 
/*	 Fields cfg_lpirx_debug	 */
#define CFG_LPIRX_DEBUG0_WIDTH                                                1
#define CFG_LPIRX_DEBUG0_SHIFT                                               10
#define CFG_LPIRX_DEBUG0_MASK                                        0x00000400
#define CFG_LPIRX_DEBUG0_RD(src)                     (((src) & 0x00000400)>>10)
#define CFG_LPIRX_DEBUG0_WR(src)                (((u32)(src)<<10) & 0x00000400)
#define CFG_LPIRX_DEBUG0_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields cfg_lpirx_detect_pattern	 */
#define CFG_LPIRX_DETECT_PATTERN0_WIDTH                                      10
#define CFG_LPIRX_DETECT_PATTERN0_SHIFT                                       0
#define CFG_LPIRX_DETECT_PATTERN0_MASK                               0x000003ff
#define CFG_LPIRX_DETECT_PATTERN0_RD(src)                (((src) & 0x000003ff))
#define CFG_LPIRX_DETECT_PATTERN0_WR(src)           (((u32)(src)) & 0x000003ff)
#define CFG_LPIRX_DETECT_PATTERN0_SET(dst,src) \
                          (((dst) & ~0x000003ff) | (((u32)(src)) & 0x000003ff))

/*	Register EEE_Rx_lpi_detect_1	*/ 
/*	 Fields cfg_lpirx_debug	 */
#define CFG_LPIRX_DEBUG1_WIDTH                                                1
#define CFG_LPIRX_DEBUG1_SHIFT                                               10
#define CFG_LPIRX_DEBUG1_MASK                                        0x00000400
#define CFG_LPIRX_DEBUG1_RD(src)                     (((src) & 0x00000400)>>10)
#define CFG_LPIRX_DEBUG1_WR(src)                (((u32)(src)<<10) & 0x00000400)
#define CFG_LPIRX_DEBUG1_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields cfg_lpirx_detect_pattern	 */
#define CFG_LPIRX_DETECT_PATTERN1_WIDTH                                      10
#define CFG_LPIRX_DETECT_PATTERN1_SHIFT                                       0
#define CFG_LPIRX_DETECT_PATTERN1_MASK                               0x000003ff
#define CFG_LPIRX_DETECT_PATTERN1_RD(src)                (((src) & 0x000003ff))
#define CFG_LPIRX_DETECT_PATTERN1_WR(src)           (((u32)(src)) & 0x000003ff)
#define CFG_LPIRX_DETECT_PATTERN1_SET(dst,src) \
                          (((dst) & ~0x000003ff) | (((u32)(src)) & 0x000003ff))

/*	Register EEE_Rx_lpi_detect_Intr_0	*/ 
/*	 Fields lpi_detect	 */
#define LPI_DETECT0_WIDTH                                                     1
#define LPI_DETECT0_SHIFT                                                     0
#define LPI_DETECT0_MASK                                             0x00000001
#define LPI_DETECT0_RD(src)                              (((src) & 0x00000001))
#define LPI_DETECT0_WR(src)                         (((u32)(src)) & 0x00000001)
#define LPI_DETECT0_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register EEE_Rx_lpi_detect_Intr_0Mask	*/
/*    Mask Register Fields lpi_detectMask    */
#define LPI_DETECTMASK_WIDTH                                                  1
#define LPI_DETECTMASK_SHIFT                                                  0
#define LPI_DETECTMASK_MASK                                          0x00000001
#define LPI_DETECTMASK_RD(src)                           (((src) & 0x00000001))
#define LPI_DETECTMASK_WR(src)                      (((u32)(src)) & 0x00000001)
#define LPI_DETECTMASK_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register EEE_Rx_lpi_detect_Intr_1	*/ 
/*	 Fields lpi_detect	 */
#define LPI_DETECT1_WIDTH                                                     1
#define LPI_DETECT1_SHIFT                                                     0
#define LPI_DETECT1_MASK                                             0x00000001
#define LPI_DETECT1_RD(src)                              (((src) & 0x00000001))
#define LPI_DETECT1_WR(src)                         (((u32)(src)) & 0x00000001)
#define LPI_DETECT1_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register EEE_Rx_lpi_detect_Intr_1Mask	*/
/*    Mask Register Fields lpi_detectMask    */
#define LPI_DETECTMASK_F1_WIDTH                                               1
#define LPI_DETECTMASK_F1_SHIFT                                               0
#define LPI_DETECTMASK_F1_MASK                                       0x00000001
#define LPI_DETECTMASK_F1_RD(src)                        (((src) & 0x00000001))
#define LPI_DETECTMASK_F1_WR(src)                   (((u32)(src)) & 0x00000001)
#define LPI_DETECTMASK_F1_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register HdrPrs_Config0_reg_0	*/ 
/*	 Fields cfg_macsec_bytes	 */
#define CFG_MACSEC_BYTES0_WIDTH                                               8
#define CFG_MACSEC_BYTES0_SHIFT                                              24
#define CFG_MACSEC_BYTES0_MASK                                       0xff000000
#define CFG_MACSEC_BYTES0_RD(src)                    (((src) & 0xff000000)>>24)
#define CFG_MACSEC_BYTES0_WR(src)               (((u32)(src)<<24) & 0xff000000)
#define CFG_MACSEC_BYTES0_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))
/*	 Fields cfg_vlan_bytes	 */
#define CFG_VLAN_BYTES0_WIDTH                                                 8
#define CFG_VLAN_BYTES0_SHIFT                                                16
#define CFG_VLAN_BYTES0_MASK                                         0x00ff0000
#define CFG_VLAN_BYTES0_RD(src)                      (((src) & 0x00ff0000)>>16)
#define CFG_VLAN_BYTES0_WR(src)                 (((u32)(src)<<16) & 0x00ff0000)
#define CFG_VLAN_BYTES0_SET(dst,src) \
                      (((dst) & ~0x00ff0000) | (((u32)(src)<<16) & 0x00ff0000))
/*	 Fields cfg_allow_nested_unkn_before_macsec	 */
#define CFG_ALLOW_NESTED_UNKN_BEFORE_MACSEC0_WIDTH                            1
#define CFG_ALLOW_NESTED_UNKN_BEFORE_MACSEC0_SHIFT                           11
#define CFG_ALLOW_NESTED_UNKN_BEFORE_MACSEC0_MASK                    0x00000800
#define CFG_ALLOW_NESTED_UNKN_BEFORE_MACSEC0_RD(src) \
                                                    (((src) & 0x00000800)>>11)
#define CFG_ALLOW_NESTED_UNKN_BEFORE_MACSEC0_WR(src) \
                                                (((u32)(src)<<11) & 0x00000800)
#define CFG_ALLOW_NESTED_UNKN_BEFORE_MACSEC0_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*	 Fields cfg_allow_unkn_before_macsec	 */
#define CFG_ALLOW_UNKN_BEFORE_MACSEC0_WIDTH                                   1
#define CFG_ALLOW_UNKN_BEFORE_MACSEC0_SHIFT                                  10
#define CFG_ALLOW_UNKN_BEFORE_MACSEC0_MASK                           0x00000400
#define CFG_ALLOW_UNKN_BEFORE_MACSEC0_RD(src)        (((src) & 0x00000400)>>10)
#define CFG_ALLOW_UNKN_BEFORE_MACSEC0_WR(src)   (((u32)(src)<<10) & 0x00000400)
#define CFG_ALLOW_UNKN_BEFORE_MACSEC0_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields cfg_nested_vlan	 */
#define CFG_NESTED_VLAN0_WIDTH                                                1
#define CFG_NESTED_VLAN0_SHIFT                                                9
#define CFG_NESTED_VLAN0_MASK                                        0x00000200
#define CFG_NESTED_VLAN0_RD(src)                      (((src) & 0x00000200)>>9)
#define CFG_NESTED_VLAN0_WR(src)                 (((u32)(src)<<9) & 0x00000200)
#define CFG_NESTED_VLAN0_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields cfg_skip_prop_bytes_vld	 */
#define CFG_SKIP_PROP_BYTES_VLD0_WIDTH                                        1
#define CFG_SKIP_PROP_BYTES_VLD0_SHIFT                                        8
#define CFG_SKIP_PROP_BYTES_VLD0_MASK                                0x00000100
#define CFG_SKIP_PROP_BYTES_VLD0_RD(src)              (((src) & 0x00000100)>>8)
#define CFG_SKIP_PROP_BYTES_VLD0_WR(src)         (((u32)(src)<<8) & 0x00000100)
#define CFG_SKIP_PROP_BYTES_VLD0_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*	 Fields cfg_skip_prop_bytes	 */
#define CFG_SKIP_PROP_BYTES0_WIDTH                                            8
#define CFG_SKIP_PROP_BYTES0_SHIFT                                            0
#define CFG_SKIP_PROP_BYTES0_MASK                                    0x000000ff
#define CFG_SKIP_PROP_BYTES0_RD(src)                     (((src) & 0x000000ff))
#define CFG_SKIP_PROP_BYTES0_WR(src)                (((u32)(src)) & 0x000000ff)
#define CFG_SKIP_PROP_BYTES0_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register HdrPrs_Config1_reg_0	*/ 
/*	 Fields cfg_skip_prog_bytes	 */
#define CFG_SKIP_PROG_BYTES0_WIDTH                                            8
#define CFG_SKIP_PROG_BYTES0_SHIFT                                           24
#define CFG_SKIP_PROG_BYTES0_MASK                                    0xff000000
#define CFG_SKIP_PROG_BYTES0_RD(src)                 (((src) & 0xff000000)>>24)
#define CFG_SKIP_PROG_BYTES0_WR(src)            (((u32)(src)<<24) & 0xff000000)
#define CFG_SKIP_PROG_BYTES0_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))
/*	 Fields cfg_prog_ether_type_bytes	 */
#define CFG_PROG_ETHER_TYPE_BYTES0_WIDTH                                      8
#define CFG_PROG_ETHER_TYPE_BYTES0_SHIFT                                     16
#define CFG_PROG_ETHER_TYPE_BYTES0_MASK                              0x00ff0000
#define CFG_PROG_ETHER_TYPE_BYTES0_RD(src)           (((src) & 0x00ff0000)>>16)
#define CFG_PROG_ETHER_TYPE_BYTES0_WR(src)      (((u32)(src)<<16) & 0x00ff0000)
#define CFG_PROG_ETHER_TYPE_BYTES0_SET(dst,src) \
                      (((dst) & ~0x00ff0000) | (((u32)(src)<<16) & 0x00ff0000))
/*	 Fields cfg_prog_ether_type	 */
#define CFG_PROG_ETHER_TYPE0_WIDTH                                           16
#define CFG_PROG_ETHER_TYPE0_SHIFT                                            0
#define CFG_PROG_ETHER_TYPE0_MASK                                    0x0000ffff
#define CFG_PROG_ETHER_TYPE0_RD(src)                     (((src) & 0x0000ffff))
#define CFG_PROG_ETHER_TYPE0_WR(src)                (((u32)(src)) & 0x0000ffff)
#define CFG_PROG_ETHER_TYPE0_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register HdrPrs_Config2_reg_0	*/ 
/*	 Fields cfg_mpls_bytes	 */
#define CFG_MPLS_BYTES0_WIDTH                                                 8
#define CFG_MPLS_BYTES0_SHIFT                                                24
#define CFG_MPLS_BYTES0_MASK                                         0xff000000
#define CFG_MPLS_BYTES0_RD(src)                      (((src) & 0xff000000)>>24)
#define CFG_MPLS_BYTES0_WR(src)                 (((u32)(src)<<24) & 0xff000000)
#define CFG_MPLS_BYTES0_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))
/*	 Fields cfg_spi	 */
#define CFG_SPI0_WIDTH                                                        8
#define CFG_SPI0_SHIFT                                                       16
#define CFG_SPI0_MASK                                                0x00ff0000
#define CFG_SPI0_RD(src)                             (((src) & 0x00ff0000)>>16)
#define CFG_SPI0_WR(src)                        (((u32)(src)<<16) & 0x00ff0000)
#define CFG_SPI0_SET(dst,src) \
                      (((dst) & ~0x00ff0000) | (((u32)(src)<<16) & 0x00ff0000))
/*	 Fields cfg_ipv4_bytes	 */
#define CFG_IPV4_BYTES0_WIDTH                                                 8
#define CFG_IPV4_BYTES0_SHIFT                                                 8
#define CFG_IPV4_BYTES0_MASK                                         0x0000ff00
#define CFG_IPV4_BYTES0_RD(src)                       (((src) & 0x0000ff00)>>8)
#define CFG_IPV4_BYTES0_WR(src)                  (((u32)(src)<<8) & 0x0000ff00)
#define CFG_IPV4_BYTES0_SET(dst,src) \
                       (((dst) & ~0x0000ff00) | (((u32)(src)<<8) & 0x0000ff00))
/*	 Fields cfg_ipv6_bytes	 */
#define CFG_IPV6_BYTES0_WIDTH                                                 8
#define CFG_IPV6_BYTES0_SHIFT                                                 0
#define CFG_IPV6_BYTES0_MASK                                         0x000000ff
#define CFG_IPV6_BYTES0_RD(src)                          (((src) & 0x000000ff))
#define CFG_IPV6_BYTES0_WR(src)                     (((u32)(src)) & 0x000000ff)
#define CFG_IPV6_BYTES0_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register HdrPrs_Config0_reg_1	*/ 
/*	 Fields cfg_macsec_bytes	 */
#define CFG_MACSEC_BYTES1_WIDTH                                               8
#define CFG_MACSEC_BYTES1_SHIFT                                              24
#define CFG_MACSEC_BYTES1_MASK                                       0xff000000
#define CFG_MACSEC_BYTES1_RD(src)                    (((src) & 0xff000000)>>24)
#define CFG_MACSEC_BYTES1_WR(src)               (((u32)(src)<<24) & 0xff000000)
#define CFG_MACSEC_BYTES1_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))
/*	 Fields cfg_vlan_bytes	 */
#define CFG_VLAN_BYTES1_WIDTH                                                 8
#define CFG_VLAN_BYTES1_SHIFT                                                16
#define CFG_VLAN_BYTES1_MASK                                         0x00ff0000
#define CFG_VLAN_BYTES1_RD(src)                      (((src) & 0x00ff0000)>>16)
#define CFG_VLAN_BYTES1_WR(src)                 (((u32)(src)<<16) & 0x00ff0000)
#define CFG_VLAN_BYTES1_SET(dst,src) \
                      (((dst) & ~0x00ff0000) | (((u32)(src)<<16) & 0x00ff0000))
/*	 Fields cfg_allow_nested_unkn_before_macsec	 */
#define CFG_ALLOW_NESTED_UNKN_BEFORE_MACSEC1_WIDTH                            1
#define CFG_ALLOW_NESTED_UNKN_BEFORE_MACSEC1_SHIFT                           11
#define CFG_ALLOW_NESTED_UNKN_BEFORE_MACSEC1_MASK                    0x00000800
#define CFG_ALLOW_NESTED_UNKN_BEFORE_MACSEC1_RD(src) \
                                                    (((src) & 0x00000800)>>11)
#define CFG_ALLOW_NESTED_UNKN_BEFORE_MACSEC1_WR(src) \
                                                (((u32)(src)<<11) & 0x00000800)
#define CFG_ALLOW_NESTED_UNKN_BEFORE_MACSEC1_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*	 Fields cfg_allow_unkn_before_macsec	 */
#define CFG_ALLOW_UNKN_BEFORE_MACSEC1_WIDTH                                   1
#define CFG_ALLOW_UNKN_BEFORE_MACSEC1_SHIFT                                  10
#define CFG_ALLOW_UNKN_BEFORE_MACSEC1_MASK                           0x00000400
#define CFG_ALLOW_UNKN_BEFORE_MACSEC1_RD(src)        (((src) & 0x00000400)>>10)
#define CFG_ALLOW_UNKN_BEFORE_MACSEC1_WR(src)   (((u32)(src)<<10) & 0x00000400)
#define CFG_ALLOW_UNKN_BEFORE_MACSEC1_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields cfg_nested_vlan	 */
#define CFG_NESTED_VLAN1_WIDTH                                                1
#define CFG_NESTED_VLAN1_SHIFT                                                9
#define CFG_NESTED_VLAN1_MASK                                        0x00000200
#define CFG_NESTED_VLAN1_RD(src)                      (((src) & 0x00000200)>>9)
#define CFG_NESTED_VLAN1_WR(src)                 (((u32)(src)<<9) & 0x00000200)
#define CFG_NESTED_VLAN1_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields cfg_skip_prop_bytes_vld	 */
#define CFG_SKIP_PROP_BYTES_VLD1_WIDTH                                        1
#define CFG_SKIP_PROP_BYTES_VLD1_SHIFT                                        8
#define CFG_SKIP_PROP_BYTES_VLD1_MASK                                0x00000100
#define CFG_SKIP_PROP_BYTES_VLD1_RD(src)              (((src) & 0x00000100)>>8)
#define CFG_SKIP_PROP_BYTES_VLD1_WR(src)         (((u32)(src)<<8) & 0x00000100)
#define CFG_SKIP_PROP_BYTES_VLD1_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*	 Fields cfg_skip_prop_bytes	 */
#define CFG_SKIP_PROP_BYTES1_WIDTH                                            8
#define CFG_SKIP_PROP_BYTES1_SHIFT                                            0
#define CFG_SKIP_PROP_BYTES1_MASK                                    0x000000ff
#define CFG_SKIP_PROP_BYTES1_RD(src)                     (((src) & 0x000000ff))
#define CFG_SKIP_PROP_BYTES1_WR(src)                (((u32)(src)) & 0x000000ff)
#define CFG_SKIP_PROP_BYTES1_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register HdrPrs_Config1_reg_1	*/ 
/*	 Fields cfg_skip_prog_bytes	 */
#define CFG_SKIP_PROG_BYTES1_WIDTH                                            8
#define CFG_SKIP_PROG_BYTES1_SHIFT                                           24
#define CFG_SKIP_PROG_BYTES1_MASK                                    0xff000000
#define CFG_SKIP_PROG_BYTES1_RD(src)                 (((src) & 0xff000000)>>24)
#define CFG_SKIP_PROG_BYTES1_WR(src)            (((u32)(src)<<24) & 0xff000000)
#define CFG_SKIP_PROG_BYTES1_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))
/*	 Fields cfg_prog_ether_type_bytes	 */
#define CFG_PROG_ETHER_TYPE_BYTES1_WIDTH                                      8
#define CFG_PROG_ETHER_TYPE_BYTES1_SHIFT                                     16
#define CFG_PROG_ETHER_TYPE_BYTES1_MASK                              0x00ff0000
#define CFG_PROG_ETHER_TYPE_BYTES1_RD(src)           (((src) & 0x00ff0000)>>16)
#define CFG_PROG_ETHER_TYPE_BYTES1_WR(src)      (((u32)(src)<<16) & 0x00ff0000)
#define CFG_PROG_ETHER_TYPE_BYTES1_SET(dst,src) \
                      (((dst) & ~0x00ff0000) | (((u32)(src)<<16) & 0x00ff0000))
/*	 Fields cfg_prog_ether_type	 */
#define CFG_PROG_ETHER_TYPE1_WIDTH                                           16
#define CFG_PROG_ETHER_TYPE1_SHIFT                                            0
#define CFG_PROG_ETHER_TYPE1_MASK                                    0x0000ffff
#define CFG_PROG_ETHER_TYPE1_RD(src)                     (((src) & 0x0000ffff))
#define CFG_PROG_ETHER_TYPE1_WR(src)                (((u32)(src)) & 0x0000ffff)
#define CFG_PROG_ETHER_TYPE1_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register HdrPrs_Config2_reg_1	*/ 
/*	 Fields cfg_mpls_bytes	 */
#define CFG_MPLS_BYTES1_WIDTH                                                 8
#define CFG_MPLS_BYTES1_SHIFT                                                24
#define CFG_MPLS_BYTES1_MASK                                         0xff000000
#define CFG_MPLS_BYTES1_RD(src)                      (((src) & 0xff000000)>>24)
#define CFG_MPLS_BYTES1_WR(src)                 (((u32)(src)<<24) & 0xff000000)
#define CFG_MPLS_BYTES1_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))
/*	 Fields cfg_spi	 */
#define CFG_SPI1_WIDTH                                                        8
#define CFG_SPI1_SHIFT                                                       16
#define CFG_SPI1_MASK                                                0x00ff0000
#define CFG_SPI1_RD(src)                             (((src) & 0x00ff0000)>>16)
#define CFG_SPI1_WR(src)                        (((u32)(src)<<16) & 0x00ff0000)
#define CFG_SPI1_SET(dst,src) \
                      (((dst) & ~0x00ff0000) | (((u32)(src)<<16) & 0x00ff0000))
/*	 Fields cfg_ipv4_bytes	 */
#define CFG_IPV4_BYTES1_WIDTH                                                 8
#define CFG_IPV4_BYTES1_SHIFT                                                 8
#define CFG_IPV4_BYTES1_MASK                                         0x0000ff00
#define CFG_IPV4_BYTES1_RD(src)                       (((src) & 0x0000ff00)>>8)
#define CFG_IPV4_BYTES1_WR(src)                  (((u32)(src)<<8) & 0x0000ff00)
#define CFG_IPV4_BYTES1_SET(dst,src) \
                       (((dst) & ~0x0000ff00) | (((u32)(src)<<8) & 0x0000ff00))
/*	 Fields cfg_ipv6_bytes	 */
#define CFG_IPV6_BYTES1_WIDTH                                                 8
#define CFG_IPV6_BYTES1_SHIFT                                                 0
#define CFG_IPV6_BYTES1_MASK                                         0x000000ff
#define CFG_IPV6_BYTES1_RD(src)                          (((src) & 0x000000ff))
#define CFG_IPV6_BYTES1_WR(src)                     (((u32)(src)) & 0x000000ff)
#define CFG_IPV6_BYTES1_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register HdrPrs_Config3_reg_0	*/ 
/*	 Fields cfg_prs_en_timeout	 */
#define CFG_PRS_EN_TIMEOUT0_WIDTH                                             1
#define CFG_PRS_EN_TIMEOUT0_SHIFT                                             8
#define CFG_PRS_EN_TIMEOUT0_MASK                                     0x00000100
#define CFG_PRS_EN_TIMEOUT0_RD(src)                   (((src) & 0x00000100)>>8)
#define CFG_PRS_EN_TIMEOUT0_WR(src)              (((u32)(src)<<8) & 0x00000100)
#define CFG_PRS_EN_TIMEOUT0_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*	 Fields cfg_prs_max_hdr_size	 */
#define CFG_PRS_MAX_HDR_SIZE0_WIDTH                                           8
#define CFG_PRS_MAX_HDR_SIZE0_SHIFT                                           0
#define CFG_PRS_MAX_HDR_SIZE0_MASK                                   0x000000ff
#define CFG_PRS_MAX_HDR_SIZE0_RD(src)                    (((src) & 0x000000ff))
#define CFG_PRS_MAX_HDR_SIZE0_WR(src)               (((u32)(src)) & 0x000000ff)
#define CFG_PRS_MAX_HDR_SIZE0_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register HdrPrs_Config3_reg_1	*/ 
/*	 Fields cfg_prs_en_timeout	 */
#define CFG_PRS_EN_TIMEOUT1_WIDTH                                             1
#define CFG_PRS_EN_TIMEOUT1_SHIFT                                             8
#define CFG_PRS_EN_TIMEOUT1_MASK                                     0x00000100
#define CFG_PRS_EN_TIMEOUT1_RD(src)                   (((src) & 0x00000100)>>8)
#define CFG_PRS_EN_TIMEOUT1_WR(src)              (((u32)(src)<<8) & 0x00000100)
#define CFG_PRS_EN_TIMEOUT1_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*	 Fields cfg_prs_max_hdr_size	 */
#define CFG_PRS_MAX_HDR_SIZE1_WIDTH                                           8
#define CFG_PRS_MAX_HDR_SIZE1_SHIFT                                           0
#define CFG_PRS_MAX_HDR_SIZE1_MASK                                   0x000000ff
#define CFG_PRS_MAX_HDR_SIZE1_RD(src)                    (((src) & 0x000000ff))
#define CFG_PRS_MAX_HDR_SIZE1_WR(src)               (((u32)(src)) & 0x000000ff)
#define CFG_PRS_MAX_HDR_SIZE1_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register csr_ecm_cfg_0	*/ 
/*	 Fields rsh_en	 */
#define RSH_EN0_WIDTH                                                         1
#define RSH_EN0_SHIFT                                                        29
#define RSH_EN0_MASK                                                 0x20000000
#define RSH_EN0_RD(src)                              (((src) & 0x20000000)>>29)
#define RSH_EN0_WR(src)                         (((u32)(src)<<29) & 0x20000000)
#define RSH_EN0_SET(dst,src) \
                      (((dst) & ~0x20000000) | (((u32)(src)<<29) & 0x20000000))
/*	 Fields multi_dpf_autoctrl	 */
#define MULTI_DPF_AUTOCTRL0_WIDTH                                             1
#define MULTI_DPF_AUTOCTRL0_SHIFT                                            28
#define MULTI_DPF_AUTOCTRL0_MASK                                     0x10000000
#define MULTI_DPF_AUTOCTRL0_RD(src)                  (((src) & 0x10000000)>>28)
#define MULTI_DPF_AUTOCTRL0_WR(src)             (((u32)(src)<<28) & 0x10000000)
#define MULTI_DPF_AUTOCTRL0_SET(dst,src) \
                      (((dst) & ~0x10000000) | (((u32)(src)<<28) & 0x10000000))
/*	 Fields swpf_req	 */
#define SWPF_REQ0_WIDTH                                                       1
#define SWPF_REQ0_SHIFT                                                      27
#define SWPF_REQ0_MASK                                               0x08000000
#define SWPF_REQ0_RD(src)                            (((src) & 0x08000000)>>27)
#define SWPF_REQ0_WR(src)                       (((u32)(src)<<27) & 0x08000000)
#define SWPF_REQ0_SET(dst,src) \
                      (((dst) & ~0x08000000) | (((u32)(src)<<27) & 0x08000000))
/*	 Fields almstempty_thrshld	 */
#define ALMSTEMPTY_THRSHLD0_WIDTH                                             9
#define ALMSTEMPTY_THRSHLD0_SHIFT                                            18
#define ALMSTEMPTY_THRSHLD0_MASK                                     0x07fc0000
#define ALMSTEMPTY_THRSHLD0_RD(src)                  (((src) & 0x07fc0000)>>18)
#define ALMSTEMPTY_THRSHLD0_WR(src)             (((u32)(src)<<18) & 0x07fc0000)
#define ALMSTEMPTY_THRSHLD0_SET(dst,src) \
                      (((dst) & ~0x07fc0000) | (((u32)(src)<<18) & 0x07fc0000))
/*	 Fields almstfull_thrshld	 */
#define ALMSTFULL_THRSHLD0_WIDTH                                              9
#define ALMSTFULL_THRSHLD0_SHIFT                                              9
#define ALMSTFULL_THRSHLD0_MASK                                      0x0003fe00
#define ALMSTFULL_THRSHLD0_RD(src)                    (((src) & 0x0003fe00)>>9)
#define ALMSTFULL_THRSHLD0_WR(src)               (((u32)(src)<<9) & 0x0003fe00)
#define ALMSTFULL_THRSHLD0_SET(dst,src) \
                       (((dst) & ~0x0003fe00) | (((u32)(src)<<9) & 0x0003fe00))
/*	 Fields datardy_thrshld	 */
#define DATARDY_THRSHLD0_WIDTH                                                9
#define DATARDY_THRSHLD0_SHIFT                                                0
#define DATARDY_THRSHLD0_MASK                                        0x000001ff
#define DATARDY_THRSHLD0_RD(src)                         (((src) & 0x000001ff))
#define DATARDY_THRSHLD0_WR(src)                    (((u32)(src)) & 0x000001ff)
#define DATARDY_THRSHLD0_SET(dst,src) \
                          (((dst) & ~0x000001ff) | (((u32)(src)) & 0x000001ff))

/*	Register csr_ecm_cfg_1	*/ 
/*	 Fields rsh_en	 */
#define RSH_EN1_WIDTH                                                         1
#define RSH_EN1_SHIFT                                                        29
#define RSH_EN1_MASK                                                 0x20000000
#define RSH_EN1_RD(src)                              (((src) & 0x20000000)>>29)
#define RSH_EN1_WR(src)                         (((u32)(src)<<29) & 0x20000000)
#define RSH_EN1_SET(dst,src) \
                      (((dst) & ~0x20000000) | (((u32)(src)<<29) & 0x20000000))
/*	 Fields multi_dpf_autoctrl	 */
#define MULTI_DPF_AUTOCTRL1_WIDTH                                             1
#define MULTI_DPF_AUTOCTRL1_SHIFT                                            28
#define MULTI_DPF_AUTOCTRL1_MASK                                     0x10000000
#define MULTI_DPF_AUTOCTRL1_RD(src)                  (((src) & 0x10000000)>>28)
#define MULTI_DPF_AUTOCTRL1_WR(src)             (((u32)(src)<<28) & 0x10000000)
#define MULTI_DPF_AUTOCTRL1_SET(dst,src) \
                      (((dst) & ~0x10000000) | (((u32)(src)<<28) & 0x10000000))
/*	 Fields swpf_req	 */
#define SWPF_REQ1_WIDTH                                                       1
#define SWPF_REQ1_SHIFT                                                      27
#define SWPF_REQ1_MASK                                               0x08000000
#define SWPF_REQ1_RD(src)                            (((src) & 0x08000000)>>27)
#define SWPF_REQ1_WR(src)                       (((u32)(src)<<27) & 0x08000000)
#define SWPF_REQ1_SET(dst,src) \
                      (((dst) & ~0x08000000) | (((u32)(src)<<27) & 0x08000000))
/*	 Fields almstempty_thrshld	 */
#define ALMSTEMPTY_THRSHLD1_WIDTH                                             9
#define ALMSTEMPTY_THRSHLD1_SHIFT                                            18
#define ALMSTEMPTY_THRSHLD1_MASK                                     0x07fc0000
#define ALMSTEMPTY_THRSHLD1_RD(src)                  (((src) & 0x07fc0000)>>18)
#define ALMSTEMPTY_THRSHLD1_WR(src)             (((u32)(src)<<18) & 0x07fc0000)
#define ALMSTEMPTY_THRSHLD1_SET(dst,src) \
                      (((dst) & ~0x07fc0000) | (((u32)(src)<<18) & 0x07fc0000))
/*	 Fields almstfull_thrshld	 */
#define ALMSTFULL_THRSHLD1_WIDTH                                              9
#define ALMSTFULL_THRSHLD1_SHIFT                                              9
#define ALMSTFULL_THRSHLD1_MASK                                      0x0003fe00
#define ALMSTFULL_THRSHLD1_RD(src)                    (((src) & 0x0003fe00)>>9)
#define ALMSTFULL_THRSHLD1_WR(src)               (((u32)(src)<<9) & 0x0003fe00)
#define ALMSTFULL_THRSHLD1_SET(dst,src) \
                       (((dst) & ~0x0003fe00) | (((u32)(src)<<9) & 0x0003fe00))
/*	 Fields datardy_thrshld	 */
#define DATARDY_THRSHLD1_WIDTH                                                9
#define DATARDY_THRSHLD1_SHIFT                                                0
#define DATARDY_THRSHLD1_MASK                                        0x000001ff
#define DATARDY_THRSHLD1_RD(src)                         (((src) & 0x000001ff))
#define DATARDY_THRSHLD1_WR(src)                    (((u32)(src)) & 0x000001ff)
#define DATARDY_THRSHLD1_SET(dst,src) \
                          (((dst) & ~0x000001ff) | (((u32)(src)) & 0x000001ff))

/*	Register csr_ecm_swpf_0	*/ 
/*	 Fields param	 */
#define PARAM0_WIDTH                                                         16
#define PARAM0_SHIFT                                                         16
#define PARAM0_MASK                                                  0xffff0000
#define PARAM0_RD(src)                               (((src) & 0xffff0000)>>16)
#define PARAM0_WR(src)                          (((u32)(src)<<16) & 0xffff0000)
#define PARAM0_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields extparam	 */
#define EXTPARAM0_WIDTH                                                      16
#define EXTPARAM0_SHIFT                                                       0
#define EXTPARAM0_MASK                                               0x0000ffff
#define EXTPARAM0_RD(src)                                (((src) & 0x0000ffff))
#define EXTPARAM0_WR(src)                           (((u32)(src)) & 0x0000ffff)
#define EXTPARAM0_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register csr_ecm_swpf_1	*/ 
/*	 Fields param	 */
#define PARAM1_WIDTH                                                         16
#define PARAM1_SHIFT                                                         16
#define PARAM1_MASK                                                  0xffff0000
#define PARAM1_RD(src)                               (((src) & 0xffff0000)>>16)
#define PARAM1_WR(src)                          (((u32)(src)<<16) & 0xffff0000)
#define PARAM1_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields extparam	 */
#define EXTPARAM1_WIDTH                                                      16
#define EXTPARAM1_SHIFT                                                       0
#define EXTPARAM1_MASK                                               0x0000ffff
#define EXTPARAM1_RD(src)                                (((src) & 0x0000ffff))
#define EXTPARAM1_WR(src)                           (((u32)(src)) & 0x0000ffff)
#define EXTPARAM1_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register csr_multi_dpf_0	*/ 
/*	 Fields param	 */
#define PARAM0_F1_WIDTH                                                      16
#define PARAM0_F1_SHIFT                                                      16
#define PARAM0_F1_MASK                                               0xffff0000
#define PARAM0_F1_RD(src)                            (((src) & 0xffff0000)>>16)
#define PARAM0_F1_WR(src)                       (((u32)(src)<<16) & 0xffff0000)
#define PARAM0_F1_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields extparam	 */
#define EXTPARAM0_F1_WIDTH                                                   16
#define EXTPARAM0_F1_SHIFT                                                    0
#define EXTPARAM0_F1_MASK                                            0x0000ffff
#define EXTPARAM0_F1_RD(src)                             (((src) & 0x0000ffff))
#define EXTPARAM0_F1_WR(src)                        (((u32)(src)) & 0x0000ffff)
#define EXTPARAM0_F1_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register csr_multi_dpf_1	*/ 
/*	 Fields param	 */
#define PARAM1_F1_WIDTH                                                      16
#define PARAM1_F1_SHIFT                                                      16
#define PARAM1_F1_MASK                                               0xffff0000
#define PARAM1_F1_RD(src)                            (((src) & 0xffff0000)>>16)
#define PARAM1_F1_WR(src)                       (((u32)(src)<<16) & 0xffff0000)
#define PARAM1_F1_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields extparam	 */
#define EXTPARAM1_F1_WIDTH                                                   16
#define EXTPARAM1_F1_SHIFT                                                    0
#define EXTPARAM1_F1_MASK                                            0x0000ffff
#define EXTPARAM1_F1_RD(src)                             (((src) & 0x0000ffff))
#define EXTPARAM1_F1_WR(src)                        (((u32)(src)) & 0x0000ffff)
#define EXTPARAM1_F1_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register csr_ecm_rsh_0	*/ 
/*	 Fields crd	 */
#define CRD0_WIDTH                                                           16
#define CRD0_SHIFT                                                           16
#define CRD0_MASK                                                    0xffff0000
#define CRD0_RD(src)                                 (((src) & 0xffff0000)>>16)
#define CRD0_WR(src)                            (((u32)(src)<<16) & 0xffff0000)
#define CRD0_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields per	 */
#define PER0_WIDTH                                                           16
#define PER0_SHIFT                                                            0
#define PER0_MASK                                                    0x0000ffff
#define PER0_RD(src)                                     (((src) & 0x0000ffff))
#define PER0_WR(src)                                (((u32)(src)) & 0x0000ffff)
#define PER0_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register csr_ecm_rsh_1	*/ 
/*	 Fields crd	 */
#define CRD1_WIDTH                                                           16
#define CRD1_SHIFT                                                           16
#define CRD1_MASK                                                    0xffff0000
#define CRD1_RD(src)                                 (((src) & 0xffff0000)>>16)
#define CRD1_WR(src)                            (((u32)(src)<<16) & 0xffff0000)
#define CRD1_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields per	 */
#define PER1_WIDTH                                                           16
#define PER1_SHIFT                                                            0
#define PER1_MASK                                                    0x0000ffff
#define PER1_RD(src)                                     (((src) & 0x0000ffff))
#define PER1_WR(src)                                (((u32)(src)) & 0x0000ffff)
#define PER1_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register PFC_Config0_reg_0	*/ 
/*	 Fields cfg_pfc_tx_timer_status_select_reg	 */
#define CFG_PFC_TX_TIMER_STATUS_SELECT_REG0_WIDTH                             3
#define CFG_PFC_TX_TIMER_STATUS_SELECT_REG0_SHIFT                             1
#define CFG_PFC_TX_TIMER_STATUS_SELECT_REG0_MASK                     0x0000000e
#define CFG_PFC_TX_TIMER_STATUS_SELECT_REG0_RD(src)   (((src) & 0x0000000e)>>1)
#define CFG_PFC_TX_TIMER_STATUS_SELECT_REG0_WR(src) \
                                                 (((u32)(src)<<1) & 0x0000000e)
#define CFG_PFC_TX_TIMER_STATUS_SELECT_REG0_SET(dst,src) \
                       (((dst) & ~0x0000000e) | (((u32)(src)<<1) & 0x0000000e))
/*	 Fields cfg_pfc_en	 */
#define CFG_PFC_EN0_WIDTH                                                     1
#define CFG_PFC_EN0_SHIFT                                                     0
#define CFG_PFC_EN0_MASK                                             0x00000001
#define CFG_PFC_EN0_RD(src)                              (((src) & 0x00000001))
#define CFG_PFC_EN0_WR(src)                         (((u32)(src)) & 0x00000001)
#define CFG_PFC_EN0_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register PFC_Config1_reg_0	*/ 
/*	 Fields cfg_pfc_ingr_q7_level_map_reg	 */
#define CFG_PFC_INGR_Q7_LEVEL_MAP_REG0_WIDTH                                  8
#define CFG_PFC_INGR_Q7_LEVEL_MAP_REG0_SHIFT                                 24
#define CFG_PFC_INGR_Q7_LEVEL_MAP_REG0_MASK                          0xff000000
#define CFG_PFC_INGR_Q7_LEVEL_MAP_REG0_RD(src)       (((src) & 0xff000000)>>24)
#define CFG_PFC_INGR_Q7_LEVEL_MAP_REG0_WR(src)  (((u32)(src)<<24) & 0xff000000)
#define CFG_PFC_INGR_Q7_LEVEL_MAP_REG0_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))
/*	 Fields cfg_pfc_ingr_q6_level_map_reg	 */
#define CFG_PFC_INGR_Q6_LEVEL_MAP_REG0_WIDTH                                  8
#define CFG_PFC_INGR_Q6_LEVEL_MAP_REG0_SHIFT                                 16
#define CFG_PFC_INGR_Q6_LEVEL_MAP_REG0_MASK                          0x00ff0000
#define CFG_PFC_INGR_Q6_LEVEL_MAP_REG0_RD(src)       (((src) & 0x00ff0000)>>16)
#define CFG_PFC_INGR_Q6_LEVEL_MAP_REG0_WR(src)  (((u32)(src)<<16) & 0x00ff0000)
#define CFG_PFC_INGR_Q6_LEVEL_MAP_REG0_SET(dst,src) \
                      (((dst) & ~0x00ff0000) | (((u32)(src)<<16) & 0x00ff0000))
/*	 Fields cfg_pfc_ingr_q5_level_map_reg	 */
#define CFG_PFC_INGR_Q5_LEVEL_MAP_REG0_WIDTH                                  8
#define CFG_PFC_INGR_Q5_LEVEL_MAP_REG0_SHIFT                                  8
#define CFG_PFC_INGR_Q5_LEVEL_MAP_REG0_MASK                          0x0000ff00
#define CFG_PFC_INGR_Q5_LEVEL_MAP_REG0_RD(src)        (((src) & 0x0000ff00)>>8)
#define CFG_PFC_INGR_Q5_LEVEL_MAP_REG0_WR(src)   (((u32)(src)<<8) & 0x0000ff00)
#define CFG_PFC_INGR_Q5_LEVEL_MAP_REG0_SET(dst,src) \
                       (((dst) & ~0x0000ff00) | (((u32)(src)<<8) & 0x0000ff00))
/*	 Fields cfg_pfc_ingr_q4_level_map_reg	 */
#define CFG_PFC_INGR_Q4_LEVEL_MAP_REG0_WIDTH                                  8
#define CFG_PFC_INGR_Q4_LEVEL_MAP_REG0_SHIFT                                  0
#define CFG_PFC_INGR_Q4_LEVEL_MAP_REG0_MASK                          0x000000ff
#define CFG_PFC_INGR_Q4_LEVEL_MAP_REG0_RD(src)           (((src) & 0x000000ff))
#define CFG_PFC_INGR_Q4_LEVEL_MAP_REG0_WR(src)      (((u32)(src)) & 0x000000ff)
#define CFG_PFC_INGR_Q4_LEVEL_MAP_REG0_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register PFC_Config2_reg_0	*/ 
/*	 Fields cfg_pfc_ingr_q3_level_map_reg	 */
#define CFG_PFC_INGR_Q3_LEVEL_MAP_REG0_WIDTH                                  8
#define CFG_PFC_INGR_Q3_LEVEL_MAP_REG0_SHIFT                                 24
#define CFG_PFC_INGR_Q3_LEVEL_MAP_REG0_MASK                          0xff000000
#define CFG_PFC_INGR_Q3_LEVEL_MAP_REG0_RD(src)       (((src) & 0xff000000)>>24)
#define CFG_PFC_INGR_Q3_LEVEL_MAP_REG0_WR(src)  (((u32)(src)<<24) & 0xff000000)
#define CFG_PFC_INGR_Q3_LEVEL_MAP_REG0_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))
/*	 Fields cfg_pfc_ingr_q2_level_map_reg	 */
#define CFG_PFC_INGR_Q2_LEVEL_MAP_REG0_WIDTH                                  8
#define CFG_PFC_INGR_Q2_LEVEL_MAP_REG0_SHIFT                                 16
#define CFG_PFC_INGR_Q2_LEVEL_MAP_REG0_MASK                          0x00ff0000
#define CFG_PFC_INGR_Q2_LEVEL_MAP_REG0_RD(src)       (((src) & 0x00ff0000)>>16)
#define CFG_PFC_INGR_Q2_LEVEL_MAP_REG0_WR(src)  (((u32)(src)<<16) & 0x00ff0000)
#define CFG_PFC_INGR_Q2_LEVEL_MAP_REG0_SET(dst,src) \
                      (((dst) & ~0x00ff0000) | (((u32)(src)<<16) & 0x00ff0000))
/*	 Fields cfg_pfc_ingr_q1_level_map_reg	 */
#define CFG_PFC_INGR_Q1_LEVEL_MAP_REG0_WIDTH                                  8
#define CFG_PFC_INGR_Q1_LEVEL_MAP_REG0_SHIFT                                  8
#define CFG_PFC_INGR_Q1_LEVEL_MAP_REG0_MASK                          0x0000ff00
#define CFG_PFC_INGR_Q1_LEVEL_MAP_REG0_RD(src)        (((src) & 0x0000ff00)>>8)
#define CFG_PFC_INGR_Q1_LEVEL_MAP_REG0_WR(src)   (((u32)(src)<<8) & 0x0000ff00)
#define CFG_PFC_INGR_Q1_LEVEL_MAP_REG0_SET(dst,src) \
                       (((dst) & ~0x0000ff00) | (((u32)(src)<<8) & 0x0000ff00))
/*	 Fields cfg_pfc_ingr_q0_level_map_reg	 */
#define CFG_PFC_INGR_Q0_LEVEL_MAP_REG0_WIDTH                                  8
#define CFG_PFC_INGR_Q0_LEVEL_MAP_REG0_SHIFT                                  0
#define CFG_PFC_INGR_Q0_LEVEL_MAP_REG0_MASK                          0x000000ff
#define CFG_PFC_INGR_Q0_LEVEL_MAP_REG0_RD(src)           (((src) & 0x000000ff))
#define CFG_PFC_INGR_Q0_LEVEL_MAP_REG0_WR(src)      (((u32)(src)) & 0x000000ff)
#define CFG_PFC_INGR_Q0_LEVEL_MAP_REG0_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register PFC_Config3_reg_0	*/ 
/*	 Fields cfg_pfc_ingr_free_pool3_level_map_reg	 */
#define CFG_PFC_INGR_FREE_POOL3_LEVEL_MAP_REG0_WIDTH                          8
#define CFG_PFC_INGR_FREE_POOL3_LEVEL_MAP_REG0_SHIFT                         24
#define CFG_PFC_INGR_FREE_POOL3_LEVEL_MAP_REG0_MASK                  0xff000000
#define CFG_PFC_INGR_FREE_POOL3_LEVEL_MAP_REG0_RD(src) \
                                                    (((src) & 0xff000000)>>24)
#define CFG_PFC_INGR_FREE_POOL3_LEVEL_MAP_REG0_WR(src) \
                                                (((u32)(src)<<24) & 0xff000000)
#define CFG_PFC_INGR_FREE_POOL3_LEVEL_MAP_REG0_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))
/*	 Fields cfg_pfc_ingr_free_pool2_level_map_reg	 */
#define CFG_PFC_INGR_FREE_POOL2_LEVEL_MAP_REG0_WIDTH                          8
#define CFG_PFC_INGR_FREE_POOL2_LEVEL_MAP_REG0_SHIFT                         16
#define CFG_PFC_INGR_FREE_POOL2_LEVEL_MAP_REG0_MASK                  0x00ff0000
#define CFG_PFC_INGR_FREE_POOL2_LEVEL_MAP_REG0_RD(src) \
                                                    (((src) & 0x00ff0000)>>16)
#define CFG_PFC_INGR_FREE_POOL2_LEVEL_MAP_REG0_WR(src) \
                                                (((u32)(src)<<16) & 0x00ff0000)
#define CFG_PFC_INGR_FREE_POOL2_LEVEL_MAP_REG0_SET(dst,src) \
                      (((dst) & ~0x00ff0000) | (((u32)(src)<<16) & 0x00ff0000))
/*	 Fields cfg_pfc_ingr_free_pool1_level_map_reg	 */
#define CFG_PFC_INGR_FREE_POOL1_LEVEL_MAP_REG0_WIDTH                          8
#define CFG_PFC_INGR_FREE_POOL1_LEVEL_MAP_REG0_SHIFT                          8
#define CFG_PFC_INGR_FREE_POOL1_LEVEL_MAP_REG0_MASK                  0x0000ff00
#define CFG_PFC_INGR_FREE_POOL1_LEVEL_MAP_REG0_RD(src) \
                                                     (((src) & 0x0000ff00)>>8)
#define CFG_PFC_INGR_FREE_POOL1_LEVEL_MAP_REG0_WR(src) \
                                                 (((u32)(src)<<8) & 0x0000ff00)
#define CFG_PFC_INGR_FREE_POOL1_LEVEL_MAP_REG0_SET(dst,src) \
                       (((dst) & ~0x0000ff00) | (((u32)(src)<<8) & 0x0000ff00))
/*	 Fields cfg_pfc_ingr_free_pool0_level_map_reg	 */
#define CFG_PFC_INGR_FREE_POOL0_LEVEL_MAP_REG0_WIDTH                          8
#define CFG_PFC_INGR_FREE_POOL0_LEVEL_MAP_REG0_SHIFT                          0
#define CFG_PFC_INGR_FREE_POOL0_LEVEL_MAP_REG0_MASK                  0x000000ff
#define CFG_PFC_INGR_FREE_POOL0_LEVEL_MAP_REG0_RD(src)   (((src) & 0x000000ff))
#define CFG_PFC_INGR_FREE_POOL0_LEVEL_MAP_REG0_WR(src) \
                                                    (((u32)(src)) & 0x000000ff)
#define CFG_PFC_INGR_FREE_POOL0_LEVEL_MAP_REG0_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register PFC_Config4_reg_0	*/ 
/*	 Fields cfg_pfc_our_addr_reg_lsb	 */
#define CFG_PFC_OUR_ADDR_REG_LSB0_WIDTH                                      32
#define CFG_PFC_OUR_ADDR_REG_LSB0_SHIFT                                       0
#define CFG_PFC_OUR_ADDR_REG_LSB0_MASK                               0xffffffff
#define CFG_PFC_OUR_ADDR_REG_LSB0_RD(src)                (((src) & 0xffffffff))
#define CFG_PFC_OUR_ADDR_REG_LSB0_WR(src)           (((u32)(src)) & 0xffffffff)
#define CFG_PFC_OUR_ADDR_REG_LSB0_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register PFC_Config5_reg_0	*/ 
/*	 Fields cfg_pfc_our_addr_reg_msb	 */
#define CFG_PFC_OUR_ADDR_REG_MSB0_WIDTH                                      16
#define CFG_PFC_OUR_ADDR_REG_MSB0_SHIFT                                      16
#define CFG_PFC_OUR_ADDR_REG_MSB0_MASK                               0xffff0000
#define CFG_PFC_OUR_ADDR_REG_MSB0_RD(src)            (((src) & 0xffff0000)>>16)
#define CFG_PFC_OUR_ADDR_REG_MSB0_WR(src)       (((u32)(src)<<16) & 0xffff0000)
#define CFG_PFC_OUR_ADDR_REG_MSB0_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields cfg_pfc_their_addr_reg_msb	 */
#define CFG_PFC_THEIR_ADDR_REG_MSB0_WIDTH                                    16
#define CFG_PFC_THEIR_ADDR_REG_MSB0_SHIFT                                     0
#define CFG_PFC_THEIR_ADDR_REG_MSB0_MASK                             0x0000ffff
#define CFG_PFC_THEIR_ADDR_REG_MSB0_RD(src)              (((src) & 0x0000ffff))
#define CFG_PFC_THEIR_ADDR_REG_MSB0_WR(src)         (((u32)(src)) & 0x0000ffff)
#define CFG_PFC_THEIR_ADDR_REG_MSB0_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register PFC_Config6_reg_0	*/ 
/*	 Fields cfg_pfc_their_addr_reg_lsb	 */
#define CFG_PFC_THEIR_ADDR_REG_LSB0_WIDTH                                    32
#define CFG_PFC_THEIR_ADDR_REG_LSB0_SHIFT                                     0
#define CFG_PFC_THEIR_ADDR_REG_LSB0_MASK                             0xffffffff
#define CFG_PFC_THEIR_ADDR_REG_LSB0_RD(src)              (((src) & 0xffffffff))
#define CFG_PFC_THEIR_ADDR_REG_LSB0_WR(src)         (((u32)(src)) & 0xffffffff)
#define CFG_PFC_THEIR_ADDR_REG_LSB0_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register PFC_Config7_reg_0	*/ 
/*	 Fields cfg_pfc_type_reg	 */
#define CFG_PFC_TYPE_REG0_WIDTH                                              16
#define CFG_PFC_TYPE_REG0_SHIFT                                              16
#define CFG_PFC_TYPE_REG0_MASK                                       0xffff0000
#define CFG_PFC_TYPE_REG0_RD(src)                    (((src) & 0xffff0000)>>16)
#define CFG_PFC_TYPE_REG0_WR(src)               (((u32)(src)<<16) & 0xffff0000)
#define CFG_PFC_TYPE_REG0_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields cfg_pfc_opcode_reg	 */
#define CFG_PFC_OPCODE_REG0_WIDTH                                            16
#define CFG_PFC_OPCODE_REG0_SHIFT                                             0
#define CFG_PFC_OPCODE_REG0_MASK                                     0x0000ffff
#define CFG_PFC_OPCODE_REG0_RD(src)                      (((src) & 0x0000ffff))
#define CFG_PFC_OPCODE_REG0_WR(src)                 (((u32)(src)) & 0x0000ffff)
#define CFG_PFC_OPCODE_REG0_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register PFC_Config8_reg_0	*/ 
/*	 Fields cfg_pfc_tx_level_sw_assert_reg	 */
#define CFG_PFC_TX_LEVEL_SW_ASSERT_REG0_WIDTH                                 8
#define CFG_PFC_TX_LEVEL_SW_ASSERT_REG0_SHIFT                                24
#define CFG_PFC_TX_LEVEL_SW_ASSERT_REG0_MASK                         0xff000000
#define CFG_PFC_TX_LEVEL_SW_ASSERT_REG0_RD(src)      (((src) & 0xff000000)>>24)
#define CFG_PFC_TX_LEVEL_SW_ASSERT_REG0_WR(src) \
                                                (((u32)(src)<<24) & 0xff000000)
#define CFG_PFC_TX_LEVEL_SW_ASSERT_REG0_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))
/*	 Fields cfg_pfc_tx_level_sw_mask_reg	 */
#define CFG_PFC_TX_LEVEL_SW_MASK_REG0_WIDTH                                   8
#define CFG_PFC_TX_LEVEL_SW_MASK_REG0_SHIFT                                  16
#define CFG_PFC_TX_LEVEL_SW_MASK_REG0_MASK                           0x00ff0000
#define CFG_PFC_TX_LEVEL_SW_MASK_REG0_RD(src)        (((src) & 0x00ff0000)>>16)
#define CFG_PFC_TX_LEVEL_SW_MASK_REG0_WR(src)   (((u32)(src)<<16) & 0x00ff0000)
#define CFG_PFC_TX_LEVEL_SW_MASK_REG0_SET(dst,src) \
                      (((dst) & ~0x00ff0000) | (((u32)(src)<<16) & 0x00ff0000))
/*	 Fields cfg_pfc_timer_tx_value_reg	 */
#define CFG_PFC_TIMER_TX_VALUE_REG0_WIDTH                                    16
#define CFG_PFC_TIMER_TX_VALUE_REG0_SHIFT                                     0
#define CFG_PFC_TIMER_TX_VALUE_REG0_MASK                             0x0000ffff
#define CFG_PFC_TIMER_TX_VALUE_REG0_RD(src)              (((src) & 0x0000ffff))
#define CFG_PFC_TIMER_TX_VALUE_REG0_WR(src)         (((u32)(src)) & 0x0000ffff)
#define CFG_PFC_TIMER_TX_VALUE_REG0_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register PFC_Config9_reg_0	*/ 
/*	 Fields cfg_pfc_tx_timer_status_reg	 */
#define CFG_PFC_TX_TIMER_STATUS_REG0_WIDTH                                   32
#define CFG_PFC_TX_TIMER_STATUS_REG0_SHIFT                                    0
#define CFG_PFC_TX_TIMER_STATUS_REG0_MASK                            0xffffffff
#define CFG_PFC_TX_TIMER_STATUS_REG0_RD(src)             (((src) & 0xffffffff))
#define CFG_PFC_TX_TIMER_STATUS_REG0_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register PFC_Config10_reg_0	*/ 
/*	 Fields cfg_pfc_tx_status_reg	 */
#define CFG_PFC_TX_STATUS_REG0_WIDTH                                         32
#define CFG_PFC_TX_STATUS_REG0_SHIFT                                          0
#define CFG_PFC_TX_STATUS_REG0_MASK                                  0xffffffff
#define CFG_PFC_TX_STATUS_REG0_RD(src)                   (((src) & 0xffffffff))
#define CFG_PFC_TX_STATUS_REG0_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register PFC_Config0_reg_1	*/ 
/*	 Fields cfg_pfc_tx_timer_status_select_reg	 */
#define CFG_PFC_TX_TIMER_STATUS_SELECT_REG1_WIDTH                             3
#define CFG_PFC_TX_TIMER_STATUS_SELECT_REG1_SHIFT                             1
#define CFG_PFC_TX_TIMER_STATUS_SELECT_REG1_MASK                     0x0000000e
#define CFG_PFC_TX_TIMER_STATUS_SELECT_REG1_RD(src)   (((src) & 0x0000000e)>>1)
#define CFG_PFC_TX_TIMER_STATUS_SELECT_REG1_WR(src) \
                                                 (((u32)(src)<<1) & 0x0000000e)
#define CFG_PFC_TX_TIMER_STATUS_SELECT_REG1_SET(dst,src) \
                       (((dst) & ~0x0000000e) | (((u32)(src)<<1) & 0x0000000e))
/*	 Fields cfg_pfc_en	 */
#define CFG_PFC_EN1_WIDTH                                                     1
#define CFG_PFC_EN1_SHIFT                                                     0
#define CFG_PFC_EN1_MASK                                             0x00000001
#define CFG_PFC_EN1_RD(src)                              (((src) & 0x00000001))
#define CFG_PFC_EN1_WR(src)                         (((u32)(src)) & 0x00000001)
#define CFG_PFC_EN1_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register PFC_Config1_reg_1	*/ 
/*	 Fields cfg_pfc_ingr_q7_level_map_reg	 */
#define CFG_PFC_INGR_Q7_LEVEL_MAP_REG1_WIDTH                                  8
#define CFG_PFC_INGR_Q7_LEVEL_MAP_REG1_SHIFT                                 24
#define CFG_PFC_INGR_Q7_LEVEL_MAP_REG1_MASK                          0xff000000
#define CFG_PFC_INGR_Q7_LEVEL_MAP_REG1_RD(src)       (((src) & 0xff000000)>>24)
#define CFG_PFC_INGR_Q7_LEVEL_MAP_REG1_WR(src)  (((u32)(src)<<24) & 0xff000000)
#define CFG_PFC_INGR_Q7_LEVEL_MAP_REG1_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))
/*	 Fields cfg_pfc_ingr_q6_level_map_reg	 */
#define CFG_PFC_INGR_Q6_LEVEL_MAP_REG1_WIDTH                                  8
#define CFG_PFC_INGR_Q6_LEVEL_MAP_REG1_SHIFT                                 16
#define CFG_PFC_INGR_Q6_LEVEL_MAP_REG1_MASK                          0x00ff0000
#define CFG_PFC_INGR_Q6_LEVEL_MAP_REG1_RD(src)       (((src) & 0x00ff0000)>>16)
#define CFG_PFC_INGR_Q6_LEVEL_MAP_REG1_WR(src)  (((u32)(src)<<16) & 0x00ff0000)
#define CFG_PFC_INGR_Q6_LEVEL_MAP_REG1_SET(dst,src) \
                      (((dst) & ~0x00ff0000) | (((u32)(src)<<16) & 0x00ff0000))
/*	 Fields cfg_pfc_ingr_q5_level_map_reg	 */
#define CFG_PFC_INGR_Q5_LEVEL_MAP_REG1_WIDTH                                  8
#define CFG_PFC_INGR_Q5_LEVEL_MAP_REG1_SHIFT                                  8
#define CFG_PFC_INGR_Q5_LEVEL_MAP_REG1_MASK                          0x0000ff00
#define CFG_PFC_INGR_Q5_LEVEL_MAP_REG1_RD(src)        (((src) & 0x0000ff00)>>8)
#define CFG_PFC_INGR_Q5_LEVEL_MAP_REG1_WR(src)   (((u32)(src)<<8) & 0x0000ff00)
#define CFG_PFC_INGR_Q5_LEVEL_MAP_REG1_SET(dst,src) \
                       (((dst) & ~0x0000ff00) | (((u32)(src)<<8) & 0x0000ff00))
/*	 Fields cfg_pfc_ingr_q4_level_map_reg	 */
#define CFG_PFC_INGR_Q4_LEVEL_MAP_REG1_WIDTH                                  8
#define CFG_PFC_INGR_Q4_LEVEL_MAP_REG1_SHIFT                                  0
#define CFG_PFC_INGR_Q4_LEVEL_MAP_REG1_MASK                          0x000000ff
#define CFG_PFC_INGR_Q4_LEVEL_MAP_REG1_RD(src)           (((src) & 0x000000ff))
#define CFG_PFC_INGR_Q4_LEVEL_MAP_REG1_WR(src)      (((u32)(src)) & 0x000000ff)
#define CFG_PFC_INGR_Q4_LEVEL_MAP_REG1_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register PFC_Config2_reg_1	*/ 
/*	 Fields cfg_pfc_ingr_q3_level_map_reg	 */
#define CFG_PFC_INGR_Q3_LEVEL_MAP_REG1_WIDTH                                  8
#define CFG_PFC_INGR_Q3_LEVEL_MAP_REG1_SHIFT                                 24
#define CFG_PFC_INGR_Q3_LEVEL_MAP_REG1_MASK                          0xff000000
#define CFG_PFC_INGR_Q3_LEVEL_MAP_REG1_RD(src)       (((src) & 0xff000000)>>24)
#define CFG_PFC_INGR_Q3_LEVEL_MAP_REG1_WR(src)  (((u32)(src)<<24) & 0xff000000)
#define CFG_PFC_INGR_Q3_LEVEL_MAP_REG1_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))
/*	 Fields cfg_pfc_ingr_q2_level_map_reg	 */
#define CFG_PFC_INGR_Q2_LEVEL_MAP_REG1_WIDTH                                  8
#define CFG_PFC_INGR_Q2_LEVEL_MAP_REG1_SHIFT                                 16
#define CFG_PFC_INGR_Q2_LEVEL_MAP_REG1_MASK                          0x00ff0000
#define CFG_PFC_INGR_Q2_LEVEL_MAP_REG1_RD(src)       (((src) & 0x00ff0000)>>16)
#define CFG_PFC_INGR_Q2_LEVEL_MAP_REG1_WR(src)  (((u32)(src)<<16) & 0x00ff0000)
#define CFG_PFC_INGR_Q2_LEVEL_MAP_REG1_SET(dst,src) \
                      (((dst) & ~0x00ff0000) | (((u32)(src)<<16) & 0x00ff0000))
/*	 Fields cfg_pfc_ingr_q1_level_map_reg	 */
#define CFG_PFC_INGR_Q1_LEVEL_MAP_REG1_WIDTH                                  8
#define CFG_PFC_INGR_Q1_LEVEL_MAP_REG1_SHIFT                                  8
#define CFG_PFC_INGR_Q1_LEVEL_MAP_REG1_MASK                          0x0000ff00
#define CFG_PFC_INGR_Q1_LEVEL_MAP_REG1_RD(src)        (((src) & 0x0000ff00)>>8)
#define CFG_PFC_INGR_Q1_LEVEL_MAP_REG1_WR(src)   (((u32)(src)<<8) & 0x0000ff00)
#define CFG_PFC_INGR_Q1_LEVEL_MAP_REG1_SET(dst,src) \
                       (((dst) & ~0x0000ff00) | (((u32)(src)<<8) & 0x0000ff00))
/*	 Fields cfg_pfc_ingr_q0_level_map_reg	 */
#define CFG_PFC_INGR_Q0_LEVEL_MAP_REG1_WIDTH                                  8
#define CFG_PFC_INGR_Q0_LEVEL_MAP_REG1_SHIFT                                  0
#define CFG_PFC_INGR_Q0_LEVEL_MAP_REG1_MASK                          0x000000ff
#define CFG_PFC_INGR_Q0_LEVEL_MAP_REG1_RD(src)           (((src) & 0x000000ff))
#define CFG_PFC_INGR_Q0_LEVEL_MAP_REG1_WR(src)      (((u32)(src)) & 0x000000ff)
#define CFG_PFC_INGR_Q0_LEVEL_MAP_REG1_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register PFC_Config3_reg_1	*/ 
/*	 Fields cfg_pfc_ingr_free_pool3_level_map_reg	 */
#define CFG_PFC_INGR_FREE_POOL3_LEVEL_MAP_REG1_WIDTH                          8
#define CFG_PFC_INGR_FREE_POOL3_LEVEL_MAP_REG1_SHIFT                         24
#define CFG_PFC_INGR_FREE_POOL3_LEVEL_MAP_REG1_MASK                  0xff000000
#define CFG_PFC_INGR_FREE_POOL3_LEVEL_MAP_REG1_RD(src) \
                                                    (((src) & 0xff000000)>>24)
#define CFG_PFC_INGR_FREE_POOL3_LEVEL_MAP_REG1_WR(src) \
                                                (((u32)(src)<<24) & 0xff000000)
#define CFG_PFC_INGR_FREE_POOL3_LEVEL_MAP_REG1_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))
/*	 Fields cfg_pfc_ingr_free_pool2_level_map_reg	 */
#define CFG_PFC_INGR_FREE_POOL2_LEVEL_MAP_REG1_WIDTH                          8
#define CFG_PFC_INGR_FREE_POOL2_LEVEL_MAP_REG1_SHIFT                         16
#define CFG_PFC_INGR_FREE_POOL2_LEVEL_MAP_REG1_MASK                  0x00ff0000
#define CFG_PFC_INGR_FREE_POOL2_LEVEL_MAP_REG1_RD(src) \
                                                    (((src) & 0x00ff0000)>>16)
#define CFG_PFC_INGR_FREE_POOL2_LEVEL_MAP_REG1_WR(src) \
                                                (((u32)(src)<<16) & 0x00ff0000)
#define CFG_PFC_INGR_FREE_POOL2_LEVEL_MAP_REG1_SET(dst,src) \
                      (((dst) & ~0x00ff0000) | (((u32)(src)<<16) & 0x00ff0000))
/*	 Fields cfg_pfc_ingr_free_pool1_level_map_reg	 */
#define CFG_PFC_INGR_FREE_POOL1_LEVEL_MAP_REG1_WIDTH                          8
#define CFG_PFC_INGR_FREE_POOL1_LEVEL_MAP_REG1_SHIFT                          8
#define CFG_PFC_INGR_FREE_POOL1_LEVEL_MAP_REG1_MASK                  0x0000ff00
#define CFG_PFC_INGR_FREE_POOL1_LEVEL_MAP_REG1_RD(src) \
                                                     (((src) & 0x0000ff00)>>8)
#define CFG_PFC_INGR_FREE_POOL1_LEVEL_MAP_REG1_WR(src) \
                                                 (((u32)(src)<<8) & 0x0000ff00)
#define CFG_PFC_INGR_FREE_POOL1_LEVEL_MAP_REG1_SET(dst,src) \
                       (((dst) & ~0x0000ff00) | (((u32)(src)<<8) & 0x0000ff00))
/*	 Fields cfg_pfc_ingr_free_pool0_level_map_reg	 */
#define CFG_PFC_INGR_FREE_POOL0_LEVEL_MAP_REG1_WIDTH                          8
#define CFG_PFC_INGR_FREE_POOL0_LEVEL_MAP_REG1_SHIFT                          0
#define CFG_PFC_INGR_FREE_POOL0_LEVEL_MAP_REG1_MASK                  0x000000ff
#define CFG_PFC_INGR_FREE_POOL0_LEVEL_MAP_REG1_RD(src)   (((src) & 0x000000ff))
#define CFG_PFC_INGR_FREE_POOL0_LEVEL_MAP_REG1_WR(src) \
                                                    (((u32)(src)) & 0x000000ff)
#define CFG_PFC_INGR_FREE_POOL0_LEVEL_MAP_REG1_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register PFC_Config4_reg_1	*/ 
/*	 Fields cfg_pfc_our_addr_reg_lsb	 */
#define CFG_PFC_OUR_ADDR_REG_LSB1_WIDTH                                      32
#define CFG_PFC_OUR_ADDR_REG_LSB1_SHIFT                                       0
#define CFG_PFC_OUR_ADDR_REG_LSB1_MASK                               0xffffffff
#define CFG_PFC_OUR_ADDR_REG_LSB1_RD(src)                (((src) & 0xffffffff))
#define CFG_PFC_OUR_ADDR_REG_LSB1_WR(src)           (((u32)(src)) & 0xffffffff)
#define CFG_PFC_OUR_ADDR_REG_LSB1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register PFC_Config5_reg_1	*/ 
/*	 Fields cfg_pfc_our_addr_reg_msb	 */
#define CFG_PFC_OUR_ADDR_REG_MSB1_WIDTH                                      16
#define CFG_PFC_OUR_ADDR_REG_MSB1_SHIFT                                      16
#define CFG_PFC_OUR_ADDR_REG_MSB1_MASK                               0xffff0000
#define CFG_PFC_OUR_ADDR_REG_MSB1_RD(src)            (((src) & 0xffff0000)>>16)
#define CFG_PFC_OUR_ADDR_REG_MSB1_WR(src)       (((u32)(src)<<16) & 0xffff0000)
#define CFG_PFC_OUR_ADDR_REG_MSB1_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields cfg_pfc_their_addr_reg_msb	 */
#define CFG_PFC_THEIR_ADDR_REG_MSB1_WIDTH                                    16
#define CFG_PFC_THEIR_ADDR_REG_MSB1_SHIFT                                     0
#define CFG_PFC_THEIR_ADDR_REG_MSB1_MASK                             0x0000ffff
#define CFG_PFC_THEIR_ADDR_REG_MSB1_RD(src)              (((src) & 0x0000ffff))
#define CFG_PFC_THEIR_ADDR_REG_MSB1_WR(src)         (((u32)(src)) & 0x0000ffff)
#define CFG_PFC_THEIR_ADDR_REG_MSB1_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register PFC_Config6_reg_1	*/ 
/*	 Fields cfg_pfc_their_addr_reg_lsb	 */
#define CFG_PFC_THEIR_ADDR_REG_LSB1_WIDTH                                    32
#define CFG_PFC_THEIR_ADDR_REG_LSB1_SHIFT                                     0
#define CFG_PFC_THEIR_ADDR_REG_LSB1_MASK                             0xffffffff
#define CFG_PFC_THEIR_ADDR_REG_LSB1_RD(src)              (((src) & 0xffffffff))
#define CFG_PFC_THEIR_ADDR_REG_LSB1_WR(src)         (((u32)(src)) & 0xffffffff)
#define CFG_PFC_THEIR_ADDR_REG_LSB1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register PFC_Config7_reg_1	*/ 
/*	 Fields cfg_pfc_type_reg	 */
#define CFG_PFC_TYPE_REG1_WIDTH                                              16
#define CFG_PFC_TYPE_REG1_SHIFT                                              16
#define CFG_PFC_TYPE_REG1_MASK                                       0xffff0000
#define CFG_PFC_TYPE_REG1_RD(src)                    (((src) & 0xffff0000)>>16)
#define CFG_PFC_TYPE_REG1_WR(src)               (((u32)(src)<<16) & 0xffff0000)
#define CFG_PFC_TYPE_REG1_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields cfg_pfc_opcode_reg	 */
#define CFG_PFC_OPCODE_REG1_WIDTH                                            16
#define CFG_PFC_OPCODE_REG1_SHIFT                                             0
#define CFG_PFC_OPCODE_REG1_MASK                                     0x0000ffff
#define CFG_PFC_OPCODE_REG1_RD(src)                      (((src) & 0x0000ffff))
#define CFG_PFC_OPCODE_REG1_WR(src)                 (((u32)(src)) & 0x0000ffff)
#define CFG_PFC_OPCODE_REG1_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register PFC_Config8_reg_1	*/ 
/*	 Fields cfg_pfc_tx_level_sw_assert_reg	 */
#define CFG_PFC_TX_LEVEL_SW_ASSERT_REG1_WIDTH                                 8
#define CFG_PFC_TX_LEVEL_SW_ASSERT_REG1_SHIFT                                24
#define CFG_PFC_TX_LEVEL_SW_ASSERT_REG1_MASK                         0xff000000
#define CFG_PFC_TX_LEVEL_SW_ASSERT_REG1_RD(src)      (((src) & 0xff000000)>>24)
#define CFG_PFC_TX_LEVEL_SW_ASSERT_REG1_WR(src) \
                                                (((u32)(src)<<24) & 0xff000000)
#define CFG_PFC_TX_LEVEL_SW_ASSERT_REG1_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))
/*	 Fields cfg_pfc_tx_level_sw_mask_reg	 */
#define CFG_PFC_TX_LEVEL_SW_MASK_REG1_WIDTH                                   8
#define CFG_PFC_TX_LEVEL_SW_MASK_REG1_SHIFT                                  16
#define CFG_PFC_TX_LEVEL_SW_MASK_REG1_MASK                           0x00ff0000
#define CFG_PFC_TX_LEVEL_SW_MASK_REG1_RD(src)        (((src) & 0x00ff0000)>>16)
#define CFG_PFC_TX_LEVEL_SW_MASK_REG1_WR(src)   (((u32)(src)<<16) & 0x00ff0000)
#define CFG_PFC_TX_LEVEL_SW_MASK_REG1_SET(dst,src) \
                      (((dst) & ~0x00ff0000) | (((u32)(src)<<16) & 0x00ff0000))
/*	 Fields cfg_pfc_timer_tx_value_reg	 */
#define CFG_PFC_TIMER_TX_VALUE_REG1_WIDTH                                    16
#define CFG_PFC_TIMER_TX_VALUE_REG1_SHIFT                                     0
#define CFG_PFC_TIMER_TX_VALUE_REG1_MASK                             0x0000ffff
#define CFG_PFC_TIMER_TX_VALUE_REG1_RD(src)              (((src) & 0x0000ffff))
#define CFG_PFC_TIMER_TX_VALUE_REG1_WR(src)         (((u32)(src)) & 0x0000ffff)
#define CFG_PFC_TIMER_TX_VALUE_REG1_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register PFC_Config9_reg_1	*/ 
/*	 Fields cfg_pfc_tx_timer_status_reg	 */
#define CFG_PFC_TX_TIMER_STATUS_REG1_WIDTH                                   32
#define CFG_PFC_TX_TIMER_STATUS_REG1_SHIFT                                    0
#define CFG_PFC_TX_TIMER_STATUS_REG1_MASK                            0xffffffff
#define CFG_PFC_TX_TIMER_STATUS_REG1_RD(src)             (((src) & 0xffffffff))
#define CFG_PFC_TX_TIMER_STATUS_REG1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register PFC_Config10_reg_1	*/ 
/*	 Fields cfg_pfc_tx_status_reg	 */
#define CFG_PFC_TX_STATUS_REG1_WIDTH                                         32
#define CFG_PFC_TX_STATUS_REG1_SHIFT                                          0
#define CFG_PFC_TX_STATUS_REG1_MASK                                  0xffffffff
#define CFG_PFC_TX_STATUS_REG1_RD(src)                   (((src) & 0xffffffff))
#define CFG_PFC_TX_STATUS_REG1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register ICM_Config0_reg_0	*/ 
/*	 Fields cfg_macMode	 */
#define CFG_MACMODE0_WIDTH                                                    2
#define CFG_MACMODE0_SHIFT                                                   18
#define CFG_MACMODE0_MASK                                            0x000c0000
#define CFG_MACMODE0_RD(src)                         (((src) & 0x000c0000)>>18)
#define CFG_MACMODE0_WR(src)                    (((u32)(src)<<18) & 0x000c0000)
#define CFG_MACMODE0_SET(dst,src) \
                      (((dst) & ~0x000c0000) | (((u32)(src)<<18) & 0x000c0000))
/*	 Fields cfg_ICM_almstFull_threshold	 */
#define CFG_ICM_ALMSTFULL_THRESHOLD0_WIDTH                                    9
#define CFG_ICM_ALMSTFULL_THRESHOLD0_SHIFT                                    9
#define CFG_ICM_ALMSTFULL_THRESHOLD0_MASK                            0x0003fe00
#define CFG_ICM_ALMSTFULL_THRESHOLD0_RD(src)          (((src) & 0x0003fe00)>>9)
#define CFG_ICM_ALMSTFULL_THRESHOLD0_WR(src)     (((u32)(src)<<9) & 0x0003fe00)
#define CFG_ICM_ALMSTFULL_THRESHOLD0_SET(dst,src) \
                       (((dst) & ~0x0003fe00) | (((u32)(src)<<9) & 0x0003fe00))
/*	 Fields cfg_ICM_Full_threshold	 */
#define CFG_ICM_FULL_THRESHOLD0_WIDTH                                         9
#define CFG_ICM_FULL_THRESHOLD0_SHIFT                                         0
#define CFG_ICM_FULL_THRESHOLD0_MASK                                 0x000001ff
#define CFG_ICM_FULL_THRESHOLD0_RD(src)                  (((src) & 0x000001ff))
#define CFG_ICM_FULL_THRESHOLD0_WR(src)             (((u32)(src)) & 0x000001ff)
#define CFG_ICM_FULL_THRESHOLD0_SET(dst,src) \
                          (((dst) & ~0x000001ff) | (((u32)(src)) & 0x000001ff))

/*	Register ICM_Config1_reg_0	*/ 
/*	 Fields CfgRFifoFullThr_ctrlfifo	 */
#define CFGRFIFOFULLTHR_CTRLFIFO0_WIDTH                                       6
#define CFGRFIFOFULLTHR_CTRLFIFO0_SHIFT                                      24
#define CFGRFIFOFULLTHR_CTRLFIFO0_MASK                               0x3f000000
#define CFGRFIFOFULLTHR_CTRLFIFO0_RD(src)            (((src) & 0x3f000000)>>24)
#define CFGRFIFOFULLTHR_CTRLFIFO0_WR(src)       (((u32)(src)<<24) & 0x3f000000)
#define CFGRFIFOFULLTHR_CTRLFIFO0_SET(dst,src) \
                      (((dst) & ~0x3f000000) | (((u32)(src)<<24) & 0x3f000000))
/*	 Fields CfgWFifoFullThr_ctrlfifo	 */
#define CFGWFIFOFULLTHR_CTRLFIFO0_WIDTH                                       6
#define CFGWFIFOFULLTHR_CTRLFIFO0_SHIFT                                      18
#define CFGWFIFOFULLTHR_CTRLFIFO0_MASK                               0x00fc0000
#define CFGWFIFOFULLTHR_CTRLFIFO0_RD(src)            (((src) & 0x00fc0000)>>18)
#define CFGWFIFOFULLTHR_CTRLFIFO0_WR(src)       (((u32)(src)<<18) & 0x00fc0000)
#define CFGWFIFOFULLTHR_CTRLFIFO0_SET(dst,src) \
                      (((dst) & ~0x00fc0000) | (((u32)(src)<<18) & 0x00fc0000))
/*	 Fields CfgRFifoFullThr_datafifo	 */
#define CFGRFIFOFULLTHR_DATAFIFO0_WIDTH                                       9
#define CFGRFIFOFULLTHR_DATAFIFO0_SHIFT                                       9
#define CFGRFIFOFULLTHR_DATAFIFO0_MASK                               0x0003fe00
#define CFGRFIFOFULLTHR_DATAFIFO0_RD(src)             (((src) & 0x0003fe00)>>9)
#define CFGRFIFOFULLTHR_DATAFIFO0_WR(src)        (((u32)(src)<<9) & 0x0003fe00)
#define CFGRFIFOFULLTHR_DATAFIFO0_SET(dst,src) \
                       (((dst) & ~0x0003fe00) | (((u32)(src)<<9) & 0x0003fe00))
/*	 Fields CfgWFifoFullThr_datafifo	 */
#define CFGWFIFOFULLTHR_DATAFIFO0_WIDTH                                       9
#define CFGWFIFOFULLTHR_DATAFIFO0_SHIFT                                       0
#define CFGWFIFOFULLTHR_DATAFIFO0_MASK                               0x000001ff
#define CFGWFIFOFULLTHR_DATAFIFO0_RD(src)                (((src) & 0x000001ff))
#define CFGWFIFOFULLTHR_DATAFIFO0_WR(src)           (((u32)(src)) & 0x000001ff)
#define CFGWFIFOFULLTHR_DATAFIFO0_SET(dst,src) \
                          (((dst) & ~0x000001ff) | (((u32)(src)) & 0x000001ff))

/*	Register ICM_Config0_reg_1	*/ 
/*	 Fields cfg_macMode	 */
#define CFG_MACMODE1_WIDTH                                                    2
#define CFG_MACMODE1_SHIFT                                                   18
#define CFG_MACMODE1_MASK                                            0x000c0000
#define CFG_MACMODE1_RD(src)                         (((src) & 0x000c0000)>>18)
#define CFG_MACMODE1_WR(src)                    (((u32)(src)<<18) & 0x000c0000)
#define CFG_MACMODE1_SET(dst,src) \
                      (((dst) & ~0x000c0000) | (((u32)(src)<<18) & 0x000c0000))
/*	 Fields cfg_ICM_almstFull_threshold	 */
#define CFG_ICM_ALMSTFULL_THRESHOLD1_WIDTH                                    9
#define CFG_ICM_ALMSTFULL_THRESHOLD1_SHIFT                                    9
#define CFG_ICM_ALMSTFULL_THRESHOLD1_MASK                            0x0003fe00
#define CFG_ICM_ALMSTFULL_THRESHOLD1_RD(src)          (((src) & 0x0003fe00)>>9)
#define CFG_ICM_ALMSTFULL_THRESHOLD1_WR(src)     (((u32)(src)<<9) & 0x0003fe00)
#define CFG_ICM_ALMSTFULL_THRESHOLD1_SET(dst,src) \
                       (((dst) & ~0x0003fe00) | (((u32)(src)<<9) & 0x0003fe00))
/*	 Fields cfg_ICM_Full_threshold	 */
#define CFG_ICM_FULL_THRESHOLD1_WIDTH                                         9
#define CFG_ICM_FULL_THRESHOLD1_SHIFT                                         0
#define CFG_ICM_FULL_THRESHOLD1_MASK                                 0x000001ff
#define CFG_ICM_FULL_THRESHOLD1_RD(src)                  (((src) & 0x000001ff))
#define CFG_ICM_FULL_THRESHOLD1_WR(src)             (((u32)(src)) & 0x000001ff)
#define CFG_ICM_FULL_THRESHOLD1_SET(dst,src) \
                          (((dst) & ~0x000001ff) | (((u32)(src)) & 0x000001ff))

/*	Register ICM_Config1_reg_1	*/ 
/*	 Fields CfgRFifoFullThr_ctrlfifo	 */
#define CFGRFIFOFULLTHR_CTRLFIFO1_WIDTH                                       6
#define CFGRFIFOFULLTHR_CTRLFIFO1_SHIFT                                      24
#define CFGRFIFOFULLTHR_CTRLFIFO1_MASK                               0x3f000000
#define CFGRFIFOFULLTHR_CTRLFIFO1_RD(src)            (((src) & 0x3f000000)>>24)
#define CFGRFIFOFULLTHR_CTRLFIFO1_WR(src)       (((u32)(src)<<24) & 0x3f000000)
#define CFGRFIFOFULLTHR_CTRLFIFO1_SET(dst,src) \
                      (((dst) & ~0x3f000000) | (((u32)(src)<<24) & 0x3f000000))
/*	 Fields CfgWFifoFullThr_ctrlfifo	 */
#define CFGWFIFOFULLTHR_CTRLFIFO1_WIDTH                                       6
#define CFGWFIFOFULLTHR_CTRLFIFO1_SHIFT                                      18
#define CFGWFIFOFULLTHR_CTRLFIFO1_MASK                               0x00fc0000
#define CFGWFIFOFULLTHR_CTRLFIFO1_RD(src)            (((src) & 0x00fc0000)>>18)
#define CFGWFIFOFULLTHR_CTRLFIFO1_WR(src)       (((u32)(src)<<18) & 0x00fc0000)
#define CFGWFIFOFULLTHR_CTRLFIFO1_SET(dst,src) \
                      (((dst) & ~0x00fc0000) | (((u32)(src)<<18) & 0x00fc0000))
/*	 Fields CfgRFifoFullThr_datafifo	 */
#define CFGRFIFOFULLTHR_DATAFIFO1_WIDTH                                       9
#define CFGRFIFOFULLTHR_DATAFIFO1_SHIFT                                       9
#define CFGRFIFOFULLTHR_DATAFIFO1_MASK                               0x0003fe00
#define CFGRFIFOFULLTHR_DATAFIFO1_RD(src)             (((src) & 0x0003fe00)>>9)
#define CFGRFIFOFULLTHR_DATAFIFO1_WR(src)        (((u32)(src)<<9) & 0x0003fe00)
#define CFGRFIFOFULLTHR_DATAFIFO1_SET(dst,src) \
                       (((dst) & ~0x0003fe00) | (((u32)(src)<<9) & 0x0003fe00))
/*	 Fields CfgWFifoFullThr_datafifo	 */
#define CFGWFIFOFULLTHR_DATAFIFO1_WIDTH                                       9
#define CFGWFIFOFULLTHR_DATAFIFO1_SHIFT                                       0
#define CFGWFIFOFULLTHR_DATAFIFO1_MASK                               0x000001ff
#define CFGWFIFOFULLTHR_DATAFIFO1_RD(src)                (((src) & 0x000001ff))
#define CFGWFIFOFULLTHR_DATAFIFO1_WR(src)           (((u32)(src)) & 0x000001ff)
#define CFGWFIFOFULLTHR_DATAFIFO1_SET(dst,src) \
                          (((dst) & ~0x000001ff) | (((u32)(src)) & 0x000001ff))

/*	Register ICM_Config2_reg_0	*/ 
/*	 Fields CfgWaitAsyncRdEn	 */
#define CFGWAITASYNCRDEN0_WIDTH                                               1
#define CFGWAITASYNCRDEN0_SHIFT                                              16
#define CFGWAITASYNCRDEN0_MASK                                       0x00010000
#define CFGWAITASYNCRDEN0_RD(src)                    (((src) & 0x00010000)>>16)
#define CFGWAITASYNCRDEN0_WR(src)               (((u32)(src)<<16) & 0x00010000)
#define CFGWAITASYNCRDEN0_SET(dst,src) \
                      (((dst) & ~0x00010000) | (((u32)(src)<<16) & 0x00010000))
/*	 Fields CfgWaitAsyncRd	 */
#define CFGWAITASYNCRD0_WIDTH                                                16
#define CFGWAITASYNCRD0_SHIFT                                                 0
#define CFGWAITASYNCRD0_MASK                                         0x0000ffff
#define CFGWAITASYNCRD0_RD(src)                          (((src) & 0x0000ffff))
#define CFGWAITASYNCRD0_WR(src)                     (((u32)(src)) & 0x0000ffff)
#define CFGWAITASYNCRD0_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register ICM_Config2_reg_1	*/ 
/*	 Fields CfgWaitAsyncRdEn	 */
#define CFGWAITASYNCRDEN1_WIDTH                                               1
#define CFGWAITASYNCRDEN1_SHIFT                                              16
#define CFGWAITASYNCRDEN1_MASK                                       0x00010000
#define CFGWAITASYNCRDEN1_RD(src)                    (((src) & 0x00010000)>>16)
#define CFGWAITASYNCRDEN1_WR(src)               (((u32)(src)<<16) & 0x00010000)
#define CFGWAITASYNCRDEN1_SET(dst,src) \
                      (((dst) & ~0x00010000) | (((u32)(src)<<16) & 0x00010000))
/*	 Fields CfgWaitAsyncRd	 */
#define CFGWAITASYNCRD1_WIDTH                                                16
#define CFGWAITASYNCRD1_SHIFT                                                 0
#define CFGWAITASYNCRD1_MASK                                         0x0000ffff
#define CFGWAITASYNCRD1_RD(src)                          (((src) & 0x0000ffff))
#define CFGWAITASYNCRD1_WR(src)                     (((u32)(src)) & 0x0000ffff)
#define CFGWAITASYNCRD1_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register ECM_Config0_reg_0	*/ 
/*	 Fields enet_clk_freq_sel	 */
#define ENET_CLK_FREQ_SEL0_WIDTH                                              1
#define ENET_CLK_FREQ_SEL0_SHIFT                                             20
#define ENET_CLK_FREQ_SEL0_MASK                                      0x00100000
#define ENET_CLK_FREQ_SEL0_RD(src)                   (((src) & 0x00100000)>>20)
#define ENET_CLK_FREQ_SEL0_WR(src)              (((u32)(src)<<20) & 0x00100000)
#define ENET_CLK_FREQ_SEL0_SET(dst,src) \
                      (((dst) & ~0x00100000) | (((u32)(src)<<20) & 0x00100000))
/*	 Fields CfgRFifoFullThr_ecm	 */
#define CFGRFIFOFULLTHR_ECM0_WIDTH                                           10
#define CFGRFIFOFULLTHR_ECM0_SHIFT                                           10
#define CFGRFIFOFULLTHR_ECM0_MASK                                    0x000ffc00
#define CFGRFIFOFULLTHR_ECM0_RD(src)                 (((src) & 0x000ffc00)>>10)
#define CFGRFIFOFULLTHR_ECM0_WR(src)            (((u32)(src)<<10) & 0x000ffc00)
#define CFGRFIFOFULLTHR_ECM0_SET(dst,src) \
                      (((dst) & ~0x000ffc00) | (((u32)(src)<<10) & 0x000ffc00))
/*	 Fields CfgWFifoFullThr_ecm	 */
#define CFGWFIFOFULLTHR_ECM0_WIDTH                                           10
#define CFGWFIFOFULLTHR_ECM0_SHIFT                                            0
#define CFGWFIFOFULLTHR_ECM0_MASK                                    0x000003ff
#define CFGWFIFOFULLTHR_ECM0_RD(src)                     (((src) & 0x000003ff))
#define CFGWFIFOFULLTHR_ECM0_WR(src)                (((u32)(src)) & 0x000003ff)
#define CFGWFIFOFULLTHR_ECM0_SET(dst,src) \
                          (((dst) & ~0x000003ff) | (((u32)(src)) & 0x000003ff))

/*	Register ECM_Config0_reg_1	*/ 
/*	 Fields enet_clk_freq_sel	 */
#define ENET_CLK_FREQ_SEL1_WIDTH                                              1
#define ENET_CLK_FREQ_SEL1_SHIFT                                             20
#define ENET_CLK_FREQ_SEL1_MASK                                      0x00100000
#define ENET_CLK_FREQ_SEL1_RD(src)                   (((src) & 0x00100000)>>20)
#define ENET_CLK_FREQ_SEL1_WR(src)              (((u32)(src)<<20) & 0x00100000)
#define ENET_CLK_FREQ_SEL1_SET(dst,src) \
                      (((dst) & ~0x00100000) | (((u32)(src)<<20) & 0x00100000))
/*	 Fields CfgRFifoFullThr_ecm	 */
#define CFGRFIFOFULLTHR_ECM1_WIDTH                                           10
#define CFGRFIFOFULLTHR_ECM1_SHIFT                                           10
#define CFGRFIFOFULLTHR_ECM1_MASK                                    0x000ffc00
#define CFGRFIFOFULLTHR_ECM1_RD(src)                 (((src) & 0x000ffc00)>>10)
#define CFGRFIFOFULLTHR_ECM1_WR(src)            (((u32)(src)<<10) & 0x000ffc00)
#define CFGRFIFOFULLTHR_ECM1_SET(dst,src) \
                      (((dst) & ~0x000ffc00) | (((u32)(src)<<10) & 0x000ffc00))
/*	 Fields CfgWFifoFullThr_ecm	 */
#define CFGWFIFOFULLTHR_ECM1_WIDTH                                           10
#define CFGWFIFOFULLTHR_ECM1_SHIFT                                            0
#define CFGWFIFOFULLTHR_ECM1_MASK                                    0x000003ff
#define CFGWFIFOFULLTHR_ECM1_RD(src)                     (((src) & 0x000003ff))
#define CFGWFIFOFULLTHR_ECM1_WR(src)                (((u32)(src)) & 0x000003ff)
#define CFGWFIFOFULLTHR_ECM1_SET(dst,src) \
                          (((dst) & ~0x000003ff) | (((u32)(src)) & 0x000003ff))

/*	Register ICM_ECM_Drop_Count_reg_0	*/ 
/*	 Fields ICM_Drop_Count	 */
#define ICM_DROP_COUNT0_WIDTH                                                16
#define ICM_DROP_COUNT0_SHIFT                                                16
#define ICM_DROP_COUNT0_MASK                                         0xffff0000
#define ICM_DROP_COUNT0_RD(src)                      (((src) & 0xffff0000)>>16)
#define ICM_DROP_COUNT0_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields ECM_Drop_Count	 */
#define ECM_DROP_COUNT0_WIDTH                                                16
#define ECM_DROP_COUNT0_SHIFT                                                 0
#define ECM_DROP_COUNT0_MASK                                         0x0000ffff
#define ECM_DROP_COUNT0_RD(src)                          (((src) & 0x0000ffff))
#define ECM_DROP_COUNT0_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register ICM_ECM_Drop_Count_reg_1	*/ 
/*	 Fields ICM_Drop_Count	 */
#define ICM_DROP_COUNT1_WIDTH                                                16
#define ICM_DROP_COUNT1_SHIFT                                                16
#define ICM_DROP_COUNT1_MASK                                         0xffff0000
#define ICM_DROP_COUNT1_RD(src)                      (((src) & 0xffff0000)>>16)
#define ICM_DROP_COUNT1_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields ECM_Drop_Count	 */
#define ECM_DROP_COUNT1_WIDTH                                                16
#define ECM_DROP_COUNT1_SHIFT                                                 0
#define ECM_DROP_COUNT1_MASK                                         0x0000ffff
#define ECM_DROP_COUNT1_RD(src)                          (((src) & 0x0000ffff))
#define ECM_DROP_COUNT1_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register MAC_FIFO_STS_Reg0	*/ 
/*	 Fields icm_data_fifo_empty_prt1	 */
#define ICM_DATA_FIFO_EMPTY_PRT10_WIDTH                                       1
#define ICM_DATA_FIFO_EMPTY_PRT10_SHIFT                                      19
#define ICM_DATA_FIFO_EMPTY_PRT10_MASK                               0x00080000
#define ICM_DATA_FIFO_EMPTY_PRT10_RD(src)            (((src) & 0x00080000)>>19)
#define ICM_DATA_FIFO_EMPTY_PRT10_SET(dst,src) \
                      (((dst) & ~0x00080000) | (((u32)(src)<<19) & 0x00080000))
/*	 Fields icm_ctrl_fifo_empty_prt1	 */
#define ICM_CTRL_FIFO_EMPTY_PRT10_WIDTH                                       1
#define ICM_CTRL_FIFO_EMPTY_PRT10_SHIFT                                      18
#define ICM_CTRL_FIFO_EMPTY_PRT10_MASK                               0x00040000
#define ICM_CTRL_FIFO_EMPTY_PRT10_RD(src)            (((src) & 0x00040000)>>18)
#define ICM_CTRL_FIFO_EMPTY_PRT10_SET(dst,src) \
                      (((dst) & ~0x00040000) | (((u32)(src)<<18) & 0x00040000))
/*	 Fields ecm_data_fifo_empty_prt1	 */
#define ECM_DATA_FIFO_EMPTY_PRT10_WIDTH                                       1
#define ECM_DATA_FIFO_EMPTY_PRT10_SHIFT                                      17
#define ECM_DATA_FIFO_EMPTY_PRT10_MASK                               0x00020000
#define ECM_DATA_FIFO_EMPTY_PRT10_RD(src)            (((src) & 0x00020000)>>17)
#define ECM_DATA_FIFO_EMPTY_PRT10_SET(dst,src) \
                      (((dst) & ~0x00020000) | (((u32)(src)<<17) & 0x00020000))
/*	 Fields macif_fifo_empty_rxprt1	 */
#define MACIF_FIFO_EMPTY_RXPRT10_F2_WIDTH                                     1
#define MACIF_FIFO_EMPTY_RXPRT10_F2_SHIFT                                    16
#define MACIF_FIFO_EMPTY_RXPRT10_F2_MASK                             0x00010000
#define MACIF_FIFO_EMPTY_RXPRT10_F2_RD(src)          (((src) & 0x00010000)>>16)
#define MACIF_FIFO_EMPTY_RXPRT10_F2_SET(dst,src) \
                      (((dst) & ~0x00010000) | (((u32)(src)<<16) & 0x00010000))
/*	 Fields icm_data_fifo_empty_prt0	 */
#define ICM_DATA_FIFO_EMPTY_PRT00_WIDTH                                       1
#define ICM_DATA_FIFO_EMPTY_PRT00_SHIFT                                       3
#define ICM_DATA_FIFO_EMPTY_PRT00_MASK                               0x00000008
#define ICM_DATA_FIFO_EMPTY_PRT00_RD(src)             (((src) & 0x00000008)>>3)
#define ICM_DATA_FIFO_EMPTY_PRT00_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields icm_ctrl_fifo_empty_prt0	 */
#define ICM_CTRL_FIFO_EMPTY_PRT00_WIDTH                                       1
#define ICM_CTRL_FIFO_EMPTY_PRT00_SHIFT                                       2
#define ICM_CTRL_FIFO_EMPTY_PRT00_MASK                               0x00000004
#define ICM_CTRL_FIFO_EMPTY_PRT00_RD(src)             (((src) & 0x00000004)>>2)
#define ICM_CTRL_FIFO_EMPTY_PRT00_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields ecm_data_fifo_empty_prt0	 */
#define ECM_DATA_FIFO_EMPTY_PRT00_WIDTH                                       1
#define ECM_DATA_FIFO_EMPTY_PRT00_SHIFT                                       1
#define ECM_DATA_FIFO_EMPTY_PRT00_MASK                               0x00000002
#define ECM_DATA_FIFO_EMPTY_PRT00_RD(src)             (((src) & 0x00000002)>>1)
#define ECM_DATA_FIFO_EMPTY_PRT00_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields macif_fifo_empty_rxprt0	 */
#define MACIF_FIFO_EMPTY_RXPRT00_F2_WIDTH                                     1
#define MACIF_FIFO_EMPTY_RXPRT00_F2_SHIFT                                     0
#define MACIF_FIFO_EMPTY_RXPRT00_F2_MASK                             0x00000001
#define MACIF_FIFO_EMPTY_RXPRT00_F2_RD(src)              (((src) & 0x00000001))
#define MACIF_FIFO_EMPTY_RXPRT00_F2_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register MAC_Int_Reg0	*/ 
/*	 Fields icm_data_fifo_underfl_intr_prt1	 */
#define ICM_DATA_FIFO_UNDERFL_INTR_PRT10_WIDTH                                1
#define ICM_DATA_FIFO_UNDERFL_INTR_PRT10_SHIFT                               24
#define ICM_DATA_FIFO_UNDERFL_INTR_PRT10_MASK                        0x01000000
#define ICM_DATA_FIFO_UNDERFL_INTR_PRT10_RD(src)     (((src) & 0x01000000)>>24)
#define ICM_DATA_FIFO_UNDERFL_INTR_PRT10_WR(src) \
                                                (((u32)(src)<<24) & 0x01000000)
#define ICM_DATA_FIFO_UNDERFL_INTR_PRT10_SET(dst,src) \
                      (((dst) & ~0x01000000) | (((u32)(src)<<24) & 0x01000000))
/*	 Fields icm_data_fifo_overfl_intr_prt1	 */
#define ICM_DATA_FIFO_OVERFL_INTR_PRT10_WIDTH                                 1
#define ICM_DATA_FIFO_OVERFL_INTR_PRT10_SHIFT                                23
#define ICM_DATA_FIFO_OVERFL_INTR_PRT10_MASK                         0x00800000
#define ICM_DATA_FIFO_OVERFL_INTR_PRT10_RD(src)      (((src) & 0x00800000)>>23)
#define ICM_DATA_FIFO_OVERFL_INTR_PRT10_WR(src) \
                                                (((u32)(src)<<23) & 0x00800000)
#define ICM_DATA_FIFO_OVERFL_INTR_PRT10_SET(dst,src) \
                      (((dst) & ~0x00800000) | (((u32)(src)<<23) & 0x00800000))
/*	 Fields icm_ctrl_fifo_underfl_intr_prt1	 */
#define ICM_CTRL_FIFO_UNDERFL_INTR_PRT10_WIDTH                                1
#define ICM_CTRL_FIFO_UNDERFL_INTR_PRT10_SHIFT                               22
#define ICM_CTRL_FIFO_UNDERFL_INTR_PRT10_MASK                        0x00400000
#define ICM_CTRL_FIFO_UNDERFL_INTR_PRT10_RD(src)     (((src) & 0x00400000)>>22)
#define ICM_CTRL_FIFO_UNDERFL_INTR_PRT10_WR(src) \
                                                (((u32)(src)<<22) & 0x00400000)
#define ICM_CTRL_FIFO_UNDERFL_INTR_PRT10_SET(dst,src) \
                      (((dst) & ~0x00400000) | (((u32)(src)<<22) & 0x00400000))
/*	 Fields icm_ctrl_fifo_overfl_intr_prt1	 */
#define ICM_CTRL_FIFO_OVERFL_INTR_PRT10_WIDTH                                 1
#define ICM_CTRL_FIFO_OVERFL_INTR_PRT10_SHIFT                                21
#define ICM_CTRL_FIFO_OVERFL_INTR_PRT10_MASK                         0x00200000
#define ICM_CTRL_FIFO_OVERFL_INTR_PRT10_RD(src)      (((src) & 0x00200000)>>21)
#define ICM_CTRL_FIFO_OVERFL_INTR_PRT10_WR(src) \
                                                (((u32)(src)<<21) & 0x00200000)
#define ICM_CTRL_FIFO_OVERFL_INTR_PRT10_SET(dst,src) \
                      (((dst) & ~0x00200000) | (((u32)(src)<<21) & 0x00200000))
/*	 Fields ecm_data_fifo_undern_intr_prt1	 */
#define ECM_DATA_FIFO_UNDERN_INTR_PRT10_WIDTH                                 1
#define ECM_DATA_FIFO_UNDERN_INTR_PRT10_SHIFT                                20
#define ECM_DATA_FIFO_UNDERN_INTR_PRT10_MASK                         0x00100000
#define ECM_DATA_FIFO_UNDERN_INTR_PRT10_RD(src)      (((src) & 0x00100000)>>20)
#define ECM_DATA_FIFO_UNDERN_INTR_PRT10_WR(src) \
                                                (((u32)(src)<<20) & 0x00100000)
#define ECM_DATA_FIFO_UNDERN_INTR_PRT10_SET(dst,src) \
                      (((dst) & ~0x00100000) | (((u32)(src)<<20) & 0x00100000))
/*	 Fields ecm_data_fifo_underfl_intr_prt1	 */
#define ECM_DATA_FIFO_UNDERFL_INTR_PRT10_WIDTH                                1
#define ECM_DATA_FIFO_UNDERFL_INTR_PRT10_SHIFT                               19
#define ECM_DATA_FIFO_UNDERFL_INTR_PRT10_MASK                        0x00080000
#define ECM_DATA_FIFO_UNDERFL_INTR_PRT10_RD(src)     (((src) & 0x00080000)>>19)
#define ECM_DATA_FIFO_UNDERFL_INTR_PRT10_WR(src) \
                                                (((u32)(src)<<19) & 0x00080000)
#define ECM_DATA_FIFO_UNDERFL_INTR_PRT10_SET(dst,src) \
                      (((dst) & ~0x00080000) | (((u32)(src)<<19) & 0x00080000))
/*	 Fields ecm_data_fifo_overfl_intr_prt1	 */
#define ECM_DATA_FIFO_OVERFL_INTR_PRT10_WIDTH                                 1
#define ECM_DATA_FIFO_OVERFL_INTR_PRT10_SHIFT                                18
#define ECM_DATA_FIFO_OVERFL_INTR_PRT10_MASK                         0x00040000
#define ECM_DATA_FIFO_OVERFL_INTR_PRT10_RD(src)      (((src) & 0x00040000)>>18)
#define ECM_DATA_FIFO_OVERFL_INTR_PRT10_WR(src) \
                                                (((u32)(src)<<18) & 0x00040000)
#define ECM_DATA_FIFO_OVERFL_INTR_PRT10_SET(dst,src) \
                      (((dst) & ~0x00040000) | (((u32)(src)<<18) & 0x00040000))
/*	 Fields macif_fifo_underfl_intr_rxprt1	 */
#define MACIF_FIFO_UNDERFL_INTR_RXPRT10_F2_WIDTH                              1
#define MACIF_FIFO_UNDERFL_INTR_RXPRT10_F2_SHIFT                             17
#define MACIF_FIFO_UNDERFL_INTR_RXPRT10_F2_MASK                      0x00020000
#define MACIF_FIFO_UNDERFL_INTR_RXPRT10_F2_RD(src)   (((src) & 0x00020000)>>17)
#define MACIF_FIFO_UNDERFL_INTR_RXPRT10_F2_WR(src) \
                                                (((u32)(src)<<17) & 0x00020000)
#define MACIF_FIFO_UNDERFL_INTR_RXPRT10_F2_SET(dst,src) \
                      (((dst) & ~0x00020000) | (((u32)(src)<<17) & 0x00020000))
/*	 Fields macif_fifo_overfl_intr_rxprt1	 */
#define MACIF_FIFO_OVERFL_INTR_RXPRT10_F2_WIDTH                               1
#define MACIF_FIFO_OVERFL_INTR_RXPRT10_F2_SHIFT                              16
#define MACIF_FIFO_OVERFL_INTR_RXPRT10_F2_MASK                       0x00010000
#define MACIF_FIFO_OVERFL_INTR_RXPRT10_F2_RD(src)    (((src) & 0x00010000)>>16)
#define MACIF_FIFO_OVERFL_INTR_RXPRT10_F2_WR(src) \
                                                (((u32)(src)<<16) & 0x00010000)
#define MACIF_FIFO_OVERFL_INTR_RXPRT10_F2_SET(dst,src) \
                      (((dst) & ~0x00010000) | (((u32)(src)<<16) & 0x00010000))
/*	 Fields icm_data_fifo_underfl_intr_prt0	 */
#define ICM_DATA_FIFO_UNDERFL_INTR_PRT00_WIDTH                                1
#define ICM_DATA_FIFO_UNDERFL_INTR_PRT00_SHIFT                                8
#define ICM_DATA_FIFO_UNDERFL_INTR_PRT00_MASK                        0x00000100
#define ICM_DATA_FIFO_UNDERFL_INTR_PRT00_RD(src)      (((src) & 0x00000100)>>8)
#define ICM_DATA_FIFO_UNDERFL_INTR_PRT00_WR(src) \
                                                 (((u32)(src)<<8) & 0x00000100)
#define ICM_DATA_FIFO_UNDERFL_INTR_PRT00_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*	 Fields icm_data_fifo_overfl_intr_prt0	 */
#define ICM_DATA_FIFO_OVERFL_INTR_PRT00_WIDTH                                 1
#define ICM_DATA_FIFO_OVERFL_INTR_PRT00_SHIFT                                 7
#define ICM_DATA_FIFO_OVERFL_INTR_PRT00_MASK                         0x00000080
#define ICM_DATA_FIFO_OVERFL_INTR_PRT00_RD(src)       (((src) & 0x00000080)>>7)
#define ICM_DATA_FIFO_OVERFL_INTR_PRT00_WR(src)  (((u32)(src)<<7) & 0x00000080)
#define ICM_DATA_FIFO_OVERFL_INTR_PRT00_SET(dst,src) \
                       (((dst) & ~0x00000080) | (((u32)(src)<<7) & 0x00000080))
/*	 Fields icm_ctrl_fifo_underfl_intr_prt0	 */
#define ICM_CTRL_FIFO_UNDERFL_INTR_PRT00_WIDTH                                1
#define ICM_CTRL_FIFO_UNDERFL_INTR_PRT00_SHIFT                                6
#define ICM_CTRL_FIFO_UNDERFL_INTR_PRT00_MASK                        0x00000040
#define ICM_CTRL_FIFO_UNDERFL_INTR_PRT00_RD(src)      (((src) & 0x00000040)>>6)
#define ICM_CTRL_FIFO_UNDERFL_INTR_PRT00_WR(src) \
                                                 (((u32)(src)<<6) & 0x00000040)
#define ICM_CTRL_FIFO_UNDERFL_INTR_PRT00_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*	 Fields icm_ctrl_fifo_overfl_intr_prt0	 */
#define ICM_CTRL_FIFO_OVERFL_INTR_PRT00_WIDTH                                 1
#define ICM_CTRL_FIFO_OVERFL_INTR_PRT00_SHIFT                                 5
#define ICM_CTRL_FIFO_OVERFL_INTR_PRT00_MASK                         0x00000020
#define ICM_CTRL_FIFO_OVERFL_INTR_PRT00_RD(src)       (((src) & 0x00000020)>>5)
#define ICM_CTRL_FIFO_OVERFL_INTR_PRT00_WR(src)  (((u32)(src)<<5) & 0x00000020)
#define ICM_CTRL_FIFO_OVERFL_INTR_PRT00_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields ecm_data_fifo_undern_intr_prt0	 */
#define ECM_DATA_FIFO_UNDERN_INTR_PRT00_WIDTH                                 1
#define ECM_DATA_FIFO_UNDERN_INTR_PRT00_SHIFT                                 4
#define ECM_DATA_FIFO_UNDERN_INTR_PRT00_MASK                         0x00000010
#define ECM_DATA_FIFO_UNDERN_INTR_PRT00_RD(src)       (((src) & 0x00000010)>>4)
#define ECM_DATA_FIFO_UNDERN_INTR_PRT00_WR(src)  (((u32)(src)<<4) & 0x00000010)
#define ECM_DATA_FIFO_UNDERN_INTR_PRT00_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields ecm_data_fifo_underfl_intr_prt0	 */
#define ECM_DATA_FIFO_UNDERFL_INTR_PRT00_WIDTH                                1
#define ECM_DATA_FIFO_UNDERFL_INTR_PRT00_SHIFT                                3
#define ECM_DATA_FIFO_UNDERFL_INTR_PRT00_MASK                        0x00000008
#define ECM_DATA_FIFO_UNDERFL_INTR_PRT00_RD(src)      (((src) & 0x00000008)>>3)
#define ECM_DATA_FIFO_UNDERFL_INTR_PRT00_WR(src) \
                                                 (((u32)(src)<<3) & 0x00000008)
#define ECM_DATA_FIFO_UNDERFL_INTR_PRT00_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields ecm_data_fifo_overfl_intr_prt0	 */
#define ECM_DATA_FIFO_OVERFL_INTR_PRT00_WIDTH                                 1
#define ECM_DATA_FIFO_OVERFL_INTR_PRT00_SHIFT                                 2
#define ECM_DATA_FIFO_OVERFL_INTR_PRT00_MASK                         0x00000004
#define ECM_DATA_FIFO_OVERFL_INTR_PRT00_RD(src)       (((src) & 0x00000004)>>2)
#define ECM_DATA_FIFO_OVERFL_INTR_PRT00_WR(src)  (((u32)(src)<<2) & 0x00000004)
#define ECM_DATA_FIFO_OVERFL_INTR_PRT00_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields macif_fifo_underfl_intr_rxprt0	 */
#define MACIF_FIFO_UNDERFL_INTR_RXPRT00_F2_WIDTH                              1
#define MACIF_FIFO_UNDERFL_INTR_RXPRT00_F2_SHIFT                              1
#define MACIF_FIFO_UNDERFL_INTR_RXPRT00_F2_MASK                      0x00000002
#define MACIF_FIFO_UNDERFL_INTR_RXPRT00_F2_RD(src)    (((src) & 0x00000002)>>1)
#define MACIF_FIFO_UNDERFL_INTR_RXPRT00_F2_WR(src) \
                                                 (((u32)(src)<<1) & 0x00000002)
#define MACIF_FIFO_UNDERFL_INTR_RXPRT00_F2_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields macif_fifo_overfl_intr_rxprt0	 */
#define MACIF_FIFO_OVERFL_INTR_RXPRT00_F2_WIDTH                               1
#define MACIF_FIFO_OVERFL_INTR_RXPRT00_F2_SHIFT                               0
#define MACIF_FIFO_OVERFL_INTR_RXPRT00_F2_MASK                       0x00000001
#define MACIF_FIFO_OVERFL_INTR_RXPRT00_F2_RD(src)        (((src) & 0x00000001))
#define MACIF_FIFO_OVERFL_INTR_RXPRT00_F2_WR(src)   (((u32)(src)) & 0x00000001)
#define MACIF_FIFO_OVERFL_INTR_RXPRT00_F2_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register MAC_Int_Reg0Mask	*/
/*    Mask Register Fields icm_data_fifo_underfl_intr_prt1Mask    */
#define ICM_DATA_FIFO_UNDERFL_INTR_PRT1MASK_WIDTH                             1
#define ICM_DATA_FIFO_UNDERFL_INTR_PRT1MASK_SHIFT                            24
#define ICM_DATA_FIFO_UNDERFL_INTR_PRT1MASK_MASK                     0x01000000
#define ICM_DATA_FIFO_UNDERFL_INTR_PRT1MASK_RD(src)  (((src) & 0x01000000)>>24)
#define ICM_DATA_FIFO_UNDERFL_INTR_PRT1MASK_WR(src) \
                                                (((u32)(src)<<24) & 0x01000000)
#define ICM_DATA_FIFO_UNDERFL_INTR_PRT1MASK_SET(dst,src) \
                      (((dst) & ~0x01000000) | (((u32)(src)<<24) & 0x01000000))
/*    Mask Register Fields icm_data_fifo_overfl_intr_prt1Mask    */
#define ICM_DATA_FIFO_OVERFL_INTR_PRT1MASK_WIDTH                              1
#define ICM_DATA_FIFO_OVERFL_INTR_PRT1MASK_SHIFT                             23
#define ICM_DATA_FIFO_OVERFL_INTR_PRT1MASK_MASK                      0x00800000
#define ICM_DATA_FIFO_OVERFL_INTR_PRT1MASK_RD(src)   (((src) & 0x00800000)>>23)
#define ICM_DATA_FIFO_OVERFL_INTR_PRT1MASK_WR(src) \
                                                (((u32)(src)<<23) & 0x00800000)
#define ICM_DATA_FIFO_OVERFL_INTR_PRT1MASK_SET(dst,src) \
                      (((dst) & ~0x00800000) | (((u32)(src)<<23) & 0x00800000))
/*    Mask Register Fields icm_ctrl_fifo_underfl_intr_prt1Mask    */
#define ICM_CTRL_FIFO_UNDERFL_INTR_PRT1MASK_WIDTH                             1
#define ICM_CTRL_FIFO_UNDERFL_INTR_PRT1MASK_SHIFT                            22
#define ICM_CTRL_FIFO_UNDERFL_INTR_PRT1MASK_MASK                     0x00400000
#define ICM_CTRL_FIFO_UNDERFL_INTR_PRT1MASK_RD(src)  (((src) & 0x00400000)>>22)
#define ICM_CTRL_FIFO_UNDERFL_INTR_PRT1MASK_WR(src) \
                                                (((u32)(src)<<22) & 0x00400000)
#define ICM_CTRL_FIFO_UNDERFL_INTR_PRT1MASK_SET(dst,src) \
                      (((dst) & ~0x00400000) | (((u32)(src)<<22) & 0x00400000))
/*    Mask Register Fields icm_ctrl_fifo_overfl_intr_prt1Mask    */
#define ICM_CTRL_FIFO_OVERFL_INTR_PRT1MASK_WIDTH                              1
#define ICM_CTRL_FIFO_OVERFL_INTR_PRT1MASK_SHIFT                             21
#define ICM_CTRL_FIFO_OVERFL_INTR_PRT1MASK_MASK                      0x00200000
#define ICM_CTRL_FIFO_OVERFL_INTR_PRT1MASK_RD(src)   (((src) & 0x00200000)>>21)
#define ICM_CTRL_FIFO_OVERFL_INTR_PRT1MASK_WR(src) \
                                                (((u32)(src)<<21) & 0x00200000)
#define ICM_CTRL_FIFO_OVERFL_INTR_PRT1MASK_SET(dst,src) \
                      (((dst) & ~0x00200000) | (((u32)(src)<<21) & 0x00200000))
/*    Mask Register Fields ecm_data_fifo_undern_intr_prt1Mask    */
#define ECM_DATA_FIFO_UNDERN_INTR_PRT1MASK_WIDTH                              1
#define ECM_DATA_FIFO_UNDERN_INTR_PRT1MASK_SHIFT                             20
#define ECM_DATA_FIFO_UNDERN_INTR_PRT1MASK_MASK                      0x00100000
#define ECM_DATA_FIFO_UNDERN_INTR_PRT1MASK_RD(src)   (((src) & 0x00100000)>>20)
#define ECM_DATA_FIFO_UNDERN_INTR_PRT1MASK_WR(src) \
                                                (((u32)(src)<<20) & 0x00100000)
#define ECM_DATA_FIFO_UNDERN_INTR_PRT1MASK_SET(dst,src) \
                      (((dst) & ~0x00100000) | (((u32)(src)<<20) & 0x00100000))
/*    Mask Register Fields ecm_data_fifo_underfl_intr_prt1Mask    */
#define ECM_DATA_FIFO_UNDERFL_INTR_PRT1MASK_WIDTH                             1
#define ECM_DATA_FIFO_UNDERFL_INTR_PRT1MASK_SHIFT                            19
#define ECM_DATA_FIFO_UNDERFL_INTR_PRT1MASK_MASK                     0x00080000
#define ECM_DATA_FIFO_UNDERFL_INTR_PRT1MASK_RD(src)  (((src) & 0x00080000)>>19)
#define ECM_DATA_FIFO_UNDERFL_INTR_PRT1MASK_WR(src) \
                                                (((u32)(src)<<19) & 0x00080000)
#define ECM_DATA_FIFO_UNDERFL_INTR_PRT1MASK_SET(dst,src) \
                      (((dst) & ~0x00080000) | (((u32)(src)<<19) & 0x00080000))
/*    Mask Register Fields ecm_data_fifo_overfl_intr_prt1Mask    */
#define ECM_DATA_FIFO_OVERFL_INTR_PRT1MASK_WIDTH                              1
#define ECM_DATA_FIFO_OVERFL_INTR_PRT1MASK_SHIFT                             18
#define ECM_DATA_FIFO_OVERFL_INTR_PRT1MASK_MASK                      0x00040000
#define ECM_DATA_FIFO_OVERFL_INTR_PRT1MASK_RD(src)   (((src) & 0x00040000)>>18)
#define ECM_DATA_FIFO_OVERFL_INTR_PRT1MASK_WR(src) \
                                                (((u32)(src)<<18) & 0x00040000)
#define ECM_DATA_FIFO_OVERFL_INTR_PRT1MASK_SET(dst,src) \
                      (((dst) & ~0x00040000) | (((u32)(src)<<18) & 0x00040000))
/*    Mask Register Fields macif_fifo_underfl_intr_rxprt1Mask    */
#define MACIF_FIFO_UNDERFL_INTR_RXPRT1MASK_F1_WIDTH                           1
#define MACIF_FIFO_UNDERFL_INTR_RXPRT1MASK_F1_SHIFT                          17
#define MACIF_FIFO_UNDERFL_INTR_RXPRT1MASK_F1_MASK                   0x00020000
#define MACIF_FIFO_UNDERFL_INTR_RXPRT1MASK_F1_RD(src) \
                                                    (((src) & 0x00020000)>>17)
#define MACIF_FIFO_UNDERFL_INTR_RXPRT1MASK_F1_WR(src) \
                                                (((u32)(src)<<17) & 0x00020000)
#define MACIF_FIFO_UNDERFL_INTR_RXPRT1MASK_F1_SET(dst,src) \
                      (((dst) & ~0x00020000) | (((u32)(src)<<17) & 0x00020000))
/*    Mask Register Fields macif_fifo_overfl_intr_rxprt1Mask    */
#define MACIF_FIFO_OVERFL_INTR_RXPRT1MASK_F1_WIDTH                            1
#define MACIF_FIFO_OVERFL_INTR_RXPRT1MASK_F1_SHIFT                           16
#define MACIF_FIFO_OVERFL_INTR_RXPRT1MASK_F1_MASK                    0x00010000
#define MACIF_FIFO_OVERFL_INTR_RXPRT1MASK_F1_RD(src) \
                                                    (((src) & 0x00010000)>>16)
#define MACIF_FIFO_OVERFL_INTR_RXPRT1MASK_F1_WR(src) \
                                                (((u32)(src)<<16) & 0x00010000)
#define MACIF_FIFO_OVERFL_INTR_RXPRT1MASK_F1_SET(dst,src) \
                      (((dst) & ~0x00010000) | (((u32)(src)<<16) & 0x00010000))
/*    Mask Register Fields icm_data_fifo_underfl_intr_prt0Mask    */
#define ICM_DATA_FIFO_UNDERFL_INTR_PRT0MASK_WIDTH                             1
#define ICM_DATA_FIFO_UNDERFL_INTR_PRT0MASK_SHIFT                             8
#define ICM_DATA_FIFO_UNDERFL_INTR_PRT0MASK_MASK                     0x00000100
#define ICM_DATA_FIFO_UNDERFL_INTR_PRT0MASK_RD(src)   (((src) & 0x00000100)>>8)
#define ICM_DATA_FIFO_UNDERFL_INTR_PRT0MASK_WR(src) \
                                                 (((u32)(src)<<8) & 0x00000100)
#define ICM_DATA_FIFO_UNDERFL_INTR_PRT0MASK_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*    Mask Register Fields icm_data_fifo_overfl_intr_prt0Mask    */
#define ICM_DATA_FIFO_OVERFL_INTR_PRT0MASK_WIDTH                              1
#define ICM_DATA_FIFO_OVERFL_INTR_PRT0MASK_SHIFT                              7
#define ICM_DATA_FIFO_OVERFL_INTR_PRT0MASK_MASK                      0x00000080
#define ICM_DATA_FIFO_OVERFL_INTR_PRT0MASK_RD(src)    (((src) & 0x00000080)>>7)
#define ICM_DATA_FIFO_OVERFL_INTR_PRT0MASK_WR(src) \
                                                 (((u32)(src)<<7) & 0x00000080)
#define ICM_DATA_FIFO_OVERFL_INTR_PRT0MASK_SET(dst,src) \
                       (((dst) & ~0x00000080) | (((u32)(src)<<7) & 0x00000080))
/*    Mask Register Fields icm_ctrl_fifo_underfl_intr_prt0Mask    */
#define ICM_CTRL_FIFO_UNDERFL_INTR_PRT0MASK_WIDTH                             1
#define ICM_CTRL_FIFO_UNDERFL_INTR_PRT0MASK_SHIFT                             6
#define ICM_CTRL_FIFO_UNDERFL_INTR_PRT0MASK_MASK                     0x00000040
#define ICM_CTRL_FIFO_UNDERFL_INTR_PRT0MASK_RD(src)   (((src) & 0x00000040)>>6)
#define ICM_CTRL_FIFO_UNDERFL_INTR_PRT0MASK_WR(src) \
                                                 (((u32)(src)<<6) & 0x00000040)
#define ICM_CTRL_FIFO_UNDERFL_INTR_PRT0MASK_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*    Mask Register Fields icm_ctrl_fifo_overfl_intr_prt0Mask    */
#define ICM_CTRL_FIFO_OVERFL_INTR_PRT0MASK_WIDTH                              1
#define ICM_CTRL_FIFO_OVERFL_INTR_PRT0MASK_SHIFT                              5
#define ICM_CTRL_FIFO_OVERFL_INTR_PRT0MASK_MASK                      0x00000020
#define ICM_CTRL_FIFO_OVERFL_INTR_PRT0MASK_RD(src)    (((src) & 0x00000020)>>5)
#define ICM_CTRL_FIFO_OVERFL_INTR_PRT0MASK_WR(src) \
                                                 (((u32)(src)<<5) & 0x00000020)
#define ICM_CTRL_FIFO_OVERFL_INTR_PRT0MASK_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*    Mask Register Fields ecm_data_fifo_undern_intr_prt0Mask    */
#define ECM_DATA_FIFO_UNDERN_INTR_PRT0MASK_WIDTH                              1
#define ECM_DATA_FIFO_UNDERN_INTR_PRT0MASK_SHIFT                              4
#define ECM_DATA_FIFO_UNDERN_INTR_PRT0MASK_MASK                      0x00000010
#define ECM_DATA_FIFO_UNDERN_INTR_PRT0MASK_RD(src)    (((src) & 0x00000010)>>4)
#define ECM_DATA_FIFO_UNDERN_INTR_PRT0MASK_WR(src) \
                                                 (((u32)(src)<<4) & 0x00000010)
#define ECM_DATA_FIFO_UNDERN_INTR_PRT0MASK_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*    Mask Register Fields ecm_data_fifo_underfl_intr_prt0Mask    */
#define ECM_DATA_FIFO_UNDERFL_INTR_PRT0MASK_WIDTH                             1
#define ECM_DATA_FIFO_UNDERFL_INTR_PRT0MASK_SHIFT                             3
#define ECM_DATA_FIFO_UNDERFL_INTR_PRT0MASK_MASK                     0x00000008
#define ECM_DATA_FIFO_UNDERFL_INTR_PRT0MASK_RD(src)   (((src) & 0x00000008)>>3)
#define ECM_DATA_FIFO_UNDERFL_INTR_PRT0MASK_WR(src) \
                                                 (((u32)(src)<<3) & 0x00000008)
#define ECM_DATA_FIFO_UNDERFL_INTR_PRT0MASK_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*    Mask Register Fields ecm_data_fifo_overfl_intr_prt0Mask    */
#define ECM_DATA_FIFO_OVERFL_INTR_PRT0MASK_WIDTH                              1
#define ECM_DATA_FIFO_OVERFL_INTR_PRT0MASK_SHIFT                              2
#define ECM_DATA_FIFO_OVERFL_INTR_PRT0MASK_MASK                      0x00000004
#define ECM_DATA_FIFO_OVERFL_INTR_PRT0MASK_RD(src)    (((src) & 0x00000004)>>2)
#define ECM_DATA_FIFO_OVERFL_INTR_PRT0MASK_WR(src) \
                                                 (((u32)(src)<<2) & 0x00000004)
#define ECM_DATA_FIFO_OVERFL_INTR_PRT0MASK_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*    Mask Register Fields macif_fifo_underfl_intr_rxprt0Mask    */
#define MACIF_FIFO_UNDERFL_INTR_RXPRT0MASK_F1_WIDTH                           1
#define MACIF_FIFO_UNDERFL_INTR_RXPRT0MASK_F1_SHIFT                           1
#define MACIF_FIFO_UNDERFL_INTR_RXPRT0MASK_F1_MASK                   0x00000002
#define MACIF_FIFO_UNDERFL_INTR_RXPRT0MASK_F1_RD(src) \
                                                     (((src) & 0x00000002)>>1)
#define MACIF_FIFO_UNDERFL_INTR_RXPRT0MASK_F1_WR(src) \
                                                 (((u32)(src)<<1) & 0x00000002)
#define MACIF_FIFO_UNDERFL_INTR_RXPRT0MASK_F1_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*    Mask Register Fields macif_fifo_overfl_intr_rxprt0Mask    */
#define MACIF_FIFO_OVERFL_INTR_RXPRT0MASK_F1_WIDTH                            1
#define MACIF_FIFO_OVERFL_INTR_RXPRT0MASK_F1_SHIFT                            0
#define MACIF_FIFO_OVERFL_INTR_RXPRT0MASK_F1_MASK                    0x00000001
#define MACIF_FIFO_OVERFL_INTR_RXPRT0MASK_F1_RD(src)     (((src) & 0x00000001))
#define MACIF_FIFO_OVERFL_INTR_RXPRT0MASK_F1_WR(src) \
                                                    (((u32)(src)) & 0x00000001)
#define MACIF_FIFO_OVERFL_INTR_RXPRT0MASK_F1_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register MAC_Int_Reg1	*/ 
/*	 Fields linkdown_port1	 */
#define LINKDOWN_PORT11_WIDTH                                                 1
#define LINKDOWN_PORT11_SHIFT                                                 3
#define LINKDOWN_PORT11_MASK                                         0x00000008
#define LINKDOWN_PORT11_RD(src)                       (((src) & 0x00000008)>>3)
#define LINKDOWN_PORT11_WR(src)                  (((u32)(src)<<3) & 0x00000008)
#define LINKDOWN_PORT11_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields carry_port1	 */
#define CARRY_PORT11_WIDTH                                                    1
#define CARRY_PORT11_SHIFT                                                    2
#define CARRY_PORT11_MASK                                            0x00000004
#define CARRY_PORT11_RD(src)                          (((src) & 0x00000004)>>2)
#define CARRY_PORT11_WR(src)                     (((u32)(src)<<2) & 0x00000004)
#define CARRY_PORT11_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields linkdown_port0	 */
#define LINKDOWN_PORT01_WIDTH                                                 1
#define LINKDOWN_PORT01_SHIFT                                                 1
#define LINKDOWN_PORT01_MASK                                         0x00000002
#define LINKDOWN_PORT01_RD(src)                       (((src) & 0x00000002)>>1)
#define LINKDOWN_PORT01_WR(src)                  (((u32)(src)<<1) & 0x00000002)
#define LINKDOWN_PORT01_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields carry_port0	 */
#define CARRY_PORT01_WIDTH                                                    1
#define CARRY_PORT01_SHIFT                                                    0
#define CARRY_PORT01_MASK                                            0x00000001
#define CARRY_PORT01_RD(src)                             (((src) & 0x00000001))
#define CARRY_PORT01_WR(src)                        (((u32)(src)) & 0x00000001)
#define CARRY_PORT01_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register MAC_Int_Reg1Mask	*/
/*    Mask Register Fields linkdown_port1Mask    */
#define LINKDOWN_PORT1MASK_WIDTH                                              1
#define LINKDOWN_PORT1MASK_SHIFT                                              3
#define LINKDOWN_PORT1MASK_MASK                                      0x00000008
#define LINKDOWN_PORT1MASK_RD(src)                    (((src) & 0x00000008)>>3)
#define LINKDOWN_PORT1MASK_WR(src)               (((u32)(src)<<3) & 0x00000008)
#define LINKDOWN_PORT1MASK_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*    Mask Register Fields carry_port1Mask    */
#define CARRY_PORT1MASK_WIDTH                                                 1
#define CARRY_PORT1MASK_SHIFT                                                 2
#define CARRY_PORT1MASK_MASK                                         0x00000004
#define CARRY_PORT1MASK_RD(src)                       (((src) & 0x00000004)>>2)
#define CARRY_PORT1MASK_WR(src)                  (((u32)(src)<<2) & 0x00000004)
#define CARRY_PORT1MASK_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*    Mask Register Fields linkdown_port0Mask    */
#define LINKDOWN_PORT0MASK_WIDTH                                              1
#define LINKDOWN_PORT0MASK_SHIFT                                              1
#define LINKDOWN_PORT0MASK_MASK                                      0x00000002
#define LINKDOWN_PORT0MASK_RD(src)                    (((src) & 0x00000002)>>1)
#define LINKDOWN_PORT0MASK_WR(src)               (((u32)(src)<<1) & 0x00000002)
#define LINKDOWN_PORT0MASK_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*    Mask Register Fields carry_port0Mask    */
#define CARRY_PORT0MASK_WIDTH                                                 1
#define CARRY_PORT0MASK_SHIFT                                                 0
#define CARRY_PORT0MASK_MASK                                         0x00000001
#define CARRY_PORT0MASK_RD(src)                          (((src) & 0x00000001))
#define CARRY_PORT0MASK_WR(src)                     (((u32)(src)) & 0x00000001)
#define CARRY_PORT0MASK_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Global Base Address	*/
#define ENET_MACIP_IND_CSR_BASE_ADDR			0xddd8a0000ULL

/*    Address ENET_MACIP_IND_CSR  Registers */
#define MAC_IND_ADDR_0_ADDR                                          0x00000000
#define MAC_IND_ADDR_0_DEFAULT                                       0x00000000
#define MAC_IND_COMMAND_0_ADDR                                       0x00000004
#define MAC_IND_COMMAND_0_DEFAULT                                    0x00000000
#define MAC_IND_WDATA_0_ADDR                                         0x00000008
#define MAC_IND_WDATA_0_DEFAULT                                      0x00000000
#define MAC_IND_RDATA_0_ADDR                                         0x0000000c
#define MAC_IND_RDATA_0_DEFAULT                                      0x00000000
#define MAC_IND_COMMAND_DONE_0_ADDR                                  0x00000010
#define MAC_IND_COMMAND_DONE_0_DEFAULT                               0x00000000
#define STAT_IND_ADDR_0_ADDR                                         0x00000014
#define STAT_IND_ADDR_0_DEFAULT                                      0x00000000
#define STAT_IND_COMMAND_0_ADDR                                      0x00000018
#define STAT_IND_COMMAND_0_DEFAULT                                   0x00000000
#define STAT_IND_WDATA_0_ADDR                                        0x0000001c
#define STAT_IND_WDATA_0_DEFAULT                                     0x00000000
#define STAT_IND_RDATA_0_ADDR                                        0x00000020
#define STAT_IND_RDATA_0_DEFAULT                                     0x00000000
#define STAT_IND_COMMAND_DONE_0_ADDR                                 0x00000024
#define STAT_IND_COMMAND_DONE_0_DEFAULT                              0x00000000
#define DEVICEIDENTREG_ADDR                                          0x00000028
#define DEVICEIDENTREG_DEFAULT                                       0x41000000
#define MAC_IND_ADDR_1_ADDR                                          0x00000030
#define MAC_IND_ADDR_1_DEFAULT                                       0x00000000
#define MAC_IND_COMMAND_1_ADDR                                       0x00000034
#define MAC_IND_COMMAND_1_DEFAULT                                    0x00000000
#define MAC_IND_WDATA_1_ADDR                                         0x00000038
#define MAC_IND_WDATA_1_DEFAULT                                      0x00000000
#define MAC_IND_RDATA_1_ADDR                                         0x0000003c
#define MAC_IND_RDATA_1_DEFAULT                                      0x00000000
#define MAC_IND_COMMAND_DONE_1_ADDR                                  0x00000040
#define MAC_IND_COMMAND_DONE_1_DEFAULT                               0x00000000
#define STAT_IND_ADDR_1_ADDR                                         0x00000044
#define STAT_IND_ADDR_1_DEFAULT                                      0x00000000
#define STAT_IND_COMMAND_1_ADDR                                      0x00000048
#define STAT_IND_COMMAND_1_DEFAULT                                   0x00000000
#define STAT_IND_WDATA_1_ADDR                                        0x0000004c
#define STAT_IND_WDATA_1_DEFAULT                                     0x00000000
#define STAT_IND_RDATA_1_ADDR                                        0x00000050
#define STAT_IND_RDATA_1_DEFAULT                                     0x00000000
#define STAT_IND_COMMAND_DONE_1_ADDR                                 0x00000054
#define STAT_IND_COMMAND_DONE_1_DEFAULT                              0x00000000

/*	Register MAC_Ind_Addr_0	*/ 
/*	 Fields addr	 */
#define ADDR0_WIDTH                                                          32
#define ADDR0_SHIFT                                                           0
#define ADDR0_MASK                                                   0xffffffff
#define ADDR0_RD(src)                                    (((src) & 0xffffffff))
#define ADDR0_WR(src)                               (((u32)(src)) & 0xffffffff)
#define ADDR0_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register MAC_Ind_Command_0	*/ 
/*	 Fields write	 */
#define WRITE0_WIDTH                                                          1
#define WRITE0_SHIFT                                                         31
#define WRITE0_MASK                                                  0x80000000
#define WRITE0_RD(src)                               (((src) & 0x80000000)>>31)
#define WRITE0_WR(src)                          (((u32)(src)<<31) & 0x80000000)
#define WRITE0_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields read	 */
#define READ0_WIDTH                                                           1
#define READ0_SHIFT                                                          30
#define READ0_MASK                                                   0x40000000
#define READ0_RD(src)                                (((src) & 0x40000000)>>30)
#define READ0_WR(src)                           (((u32)(src)<<30) & 0x40000000)
#define READ0_SET(dst,src) \
                      (((dst) & ~0x40000000) | (((u32)(src)<<30) & 0x40000000))

/*	Register MAC_Ind_WData_0	*/ 
/*	 Fields wrData	 */
#define WRDATA0_WIDTH                                                        32
#define WRDATA0_SHIFT                                                         0
#define WRDATA0_MASK                                                 0xffffffff
#define WRDATA0_RD(src)                                  (((src) & 0xffffffff))
#define WRDATA0_WR(src)                             (((u32)(src)) & 0xffffffff)
#define WRDATA0_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register MAC_Ind_RData_0	*/ 
/*	 Fields readData	 */
#define READDATA0_WIDTH                                                      32
#define READDATA0_SHIFT                                                       0
#define READDATA0_MASK                                               0xffffffff
#define READDATA0_RD(src)                                (((src) & 0xffffffff))
#define READDATA0_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register MAC_Ind_Command_Done_0	*/ 
/*	 Fields write_done	 */
#define WRITE_DONE0_WIDTH                                                     1
#define WRITE_DONE0_SHIFT                                                    31
#define WRITE_DONE0_MASK                                             0x80000000
#define WRITE_DONE0_RD(src)                          (((src) & 0x80000000)>>31)
#define WRITE_DONE0_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields read_done	 */
#define READ_DONE0_WIDTH                                                      1
#define READ_DONE0_SHIFT                                                     30
#define READ_DONE0_MASK                                              0x40000000
#define READ_DONE0_RD(src)                           (((src) & 0x40000000)>>30)
#define READ_DONE0_SET(dst,src) \
                      (((dst) & ~0x40000000) | (((u32)(src)<<30) & 0x40000000))

/*	Register Stat_Ind_Addr_0	*/ 
/*	 Fields addr	 */
#define ADDR0_F1_WIDTH                                                       32
#define ADDR0_F1_SHIFT                                                        0
#define ADDR0_F1_MASK                                                0xffffffff
#define ADDR0_F1_RD(src)                                 (((src) & 0xffffffff))
#define ADDR0_F1_WR(src)                            (((u32)(src)) & 0xffffffff)
#define ADDR0_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register Stat_Ind_Command_0	*/ 
/*	 Fields write	 */
#define WRITE0_F1_WIDTH                                                       1
#define WRITE0_F1_SHIFT                                                      31
#define WRITE0_F1_MASK                                               0x80000000
#define WRITE0_F1_RD(src)                            (((src) & 0x80000000)>>31)
#define WRITE0_F1_WR(src)                       (((u32)(src)<<31) & 0x80000000)
#define WRITE0_F1_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields read	 */
#define READ0_F1_WIDTH                                                        1
#define READ0_F1_SHIFT                                                       30
#define READ0_F1_MASK                                                0x40000000
#define READ0_F1_RD(src)                             (((src) & 0x40000000)>>30)
#define READ0_F1_WR(src)                        (((u32)(src)<<30) & 0x40000000)
#define READ0_F1_SET(dst,src) \
                      (((dst) & ~0x40000000) | (((u32)(src)<<30) & 0x40000000))

/*	Register Stat_Ind_WData_0	*/ 
/*	 Fields wrData	 */
#define WRDATA0_F1_WIDTH                                                     32
#define WRDATA0_F1_SHIFT                                                      0
#define WRDATA0_F1_MASK                                              0xffffffff
#define WRDATA0_F1_RD(src)                               (((src) & 0xffffffff))
#define WRDATA0_F1_WR(src)                          (((u32)(src)) & 0xffffffff)
#define WRDATA0_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register Stat_Ind_RData_0	*/ 
/*	 Fields readData	 */
#define READDATA0_F1_WIDTH                                                   32
#define READDATA0_F1_SHIFT                                                    0
#define READDATA0_F1_MASK                                            0xffffffff
#define READDATA0_F1_RD(src)                             (((src) & 0xffffffff))
#define READDATA0_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register Stat_Ind_Command_Done_0	*/ 
/*	 Fields write_done	 */
#define WRITE_DONE0_F1_WIDTH                                                  1
#define WRITE_DONE0_F1_SHIFT                                                 31
#define WRITE_DONE0_F1_MASK                                          0x80000000
#define WRITE_DONE0_F1_RD(src)                       (((src) & 0x80000000)>>31)
#define WRITE_DONE0_F1_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields read_done	 */
#define READ_DONE0_F1_WIDTH                                                   1
#define READ_DONE0_F1_SHIFT                                                  30
#define READ_DONE0_F1_MASK                                           0x40000000
#define READ_DONE0_F1_RD(src)                        (((src) & 0x40000000)>>30)
#define READ_DONE0_F1_SET(dst,src) \
                      (((dst) & ~0x40000000) | (((u32)(src)<<30) & 0x40000000))

/*	Register DeviceIdentReg	*/ 
/*	 Fields DeviceId	 */
#define DEVICEID_F10_WIDTH                                                    8
#define DEVICEID_F10_SHIFT                                                   24
#define DEVICEID_F10_MASK                                            0xff000000
#define DEVICEID_F10_RD(src)                         (((src) & 0xff000000)>>24)
#define DEVICEID_F10_WR(src)                    (((u32)(src)<<24) & 0xff000000)
#define DEVICEID_F10_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))
/*	 Fields Reserved	 */
#define RESERVED_F146_WIDTH                                                   8
#define RESERVED_F146_SHIFT                                                  16
#define RESERVED_F146_MASK                                           0x00ff0000
#define RESERVED_F146_RD(src)                        (((src) & 0x00ff0000)>>16)
#define RESERVED_F146_WR(src)                   (((u32)(src)<<16) & 0x00ff0000)
#define RESERVED_F146_SET(dst,src) \
                      (((dst) & ~0x00ff0000) | (((u32)(src)<<16) & 0x00ff0000))
/*	 Fields MajorRev	 */
#define MAJORREV_WIDTH                                                        8
#define MAJORREV_SHIFT                                                        8
#define MAJORREV_MASK                                                0x0000ff00
#define MAJORREV_RD(src)                              (((src) & 0x0000ff00)>>8)
#define MAJORREV_WR(src)                         (((u32)(src)<<8) & 0x0000ff00)
#define MAJORREV_SET(dst,src) \
                       (((dst) & ~0x0000ff00) | (((u32)(src)<<8) & 0x0000ff00))
/*	 Fields MinorRev	 */
#define MINORREV_WIDTH                                                        8
#define MINORREV_SHIFT                                                        0
#define MINORREV_MASK                                                0x000000ff
#define MINORREV_RD(src)                                 (((src) & 0x000000ff))
#define MINORREV_WR(src)                            (((u32)(src)) & 0x000000ff)
#define MINORREV_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register MAC_Ind_Addr_1	*/ 
/*	 Fields addr	 */
#define ADDR1_WIDTH                                                          32
#define ADDR1_SHIFT                                                           0
#define ADDR1_MASK                                                   0xffffffff
#define ADDR1_RD(src)                                    (((src) & 0xffffffff))
#define ADDR1_WR(src)                               (((u32)(src)) & 0xffffffff)
#define ADDR1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register MAC_Ind_Command_1	*/ 
/*	 Fields write	 */
#define WRITE1_WIDTH                                                          1
#define WRITE1_SHIFT                                                         31
#define WRITE1_MASK                                                  0x80000000
#define WRITE1_RD(src)                               (((src) & 0x80000000)>>31)
#define WRITE1_WR(src)                          (((u32)(src)<<31) & 0x80000000)
#define WRITE1_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields read	 */
#define READ1_WIDTH                                                           1
#define READ1_SHIFT                                                          30
#define READ1_MASK                                                   0x40000000
#define READ1_RD(src)                                (((src) & 0x40000000)>>30)
#define READ1_WR(src)                           (((u32)(src)<<30) & 0x40000000)
#define READ1_SET(dst,src) \
                      (((dst) & ~0x40000000) | (((u32)(src)<<30) & 0x40000000))

/*	Register MAC_Ind_WData_1	*/ 
/*	 Fields wrData	 */
#define WRDATA1_WIDTH                                                        32
#define WRDATA1_SHIFT                                                         0
#define WRDATA1_MASK                                                 0xffffffff
#define WRDATA1_RD(src)                                  (((src) & 0xffffffff))
#define WRDATA1_WR(src)                             (((u32)(src)) & 0xffffffff)
#define WRDATA1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register MAC_Ind_RData_1	*/ 
/*	 Fields readData	 */
#define READDATA1_WIDTH                                                      32
#define READDATA1_SHIFT                                                       0
#define READDATA1_MASK                                               0xffffffff
#define READDATA1_RD(src)                                (((src) & 0xffffffff))
#define READDATA1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register MAC_Ind_Command_Done_1	*/ 
/*	 Fields write_done	 */
#define WRITE_DONE1_WIDTH                                                     1
#define WRITE_DONE1_SHIFT                                                    31
#define WRITE_DONE1_MASK                                             0x80000000
#define WRITE_DONE1_RD(src)                          (((src) & 0x80000000)>>31)
#define WRITE_DONE1_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields read_done	 */
#define READ_DONE1_WIDTH                                                      1
#define READ_DONE1_SHIFT                                                     30
#define READ_DONE1_MASK                                              0x40000000
#define READ_DONE1_RD(src)                           (((src) & 0x40000000)>>30)
#define READ_DONE1_SET(dst,src) \
                      (((dst) & ~0x40000000) | (((u32)(src)<<30) & 0x40000000))

/*	Register Stat_Ind_Addr_1	*/ 
/*	 Fields addr	 */
#define ADDR1_F1_WIDTH                                                       32
#define ADDR1_F1_SHIFT                                                        0
#define ADDR1_F1_MASK                                                0xffffffff
#define ADDR1_F1_RD(src)                                 (((src) & 0xffffffff))
#define ADDR1_F1_WR(src)                            (((u32)(src)) & 0xffffffff)
#define ADDR1_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register Stat_Ind_Command_1	*/ 
/*	 Fields write	 */
#define WRITE1_F1_WIDTH                                                       1
#define WRITE1_F1_SHIFT                                                      31
#define WRITE1_F1_MASK                                               0x80000000
#define WRITE1_F1_RD(src)                            (((src) & 0x80000000)>>31)
#define WRITE1_F1_WR(src)                       (((u32)(src)<<31) & 0x80000000)
#define WRITE1_F1_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields read	 */
#define READ1_F1_WIDTH                                                        1
#define READ1_F1_SHIFT                                                       30
#define READ1_F1_MASK                                                0x40000000
#define READ1_F1_RD(src)                             (((src) & 0x40000000)>>30)
#define READ1_F1_WR(src)                        (((u32)(src)<<30) & 0x40000000)
#define READ1_F1_SET(dst,src) \
                      (((dst) & ~0x40000000) | (((u32)(src)<<30) & 0x40000000))

/*	Register Stat_Ind_WData_1	*/ 
/*	 Fields wrData	 */
#define WRDATA1_F1_WIDTH                                                     32
#define WRDATA1_F1_SHIFT                                                      0
#define WRDATA1_F1_MASK                                              0xffffffff
#define WRDATA1_F1_RD(src)                               (((src) & 0xffffffff))
#define WRDATA1_F1_WR(src)                          (((u32)(src)) & 0xffffffff)
#define WRDATA1_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register Stat_Ind_RData_1	*/ 
/*	 Fields readData	 */
#define READDATA1_F1_WIDTH                                                   32
#define READDATA1_F1_SHIFT                                                    0
#define READDATA1_F1_MASK                                            0xffffffff
#define READDATA1_F1_RD(src)                             (((src) & 0xffffffff))
#define READDATA1_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register Stat_Ind_Command_Done_1	*/ 
/*	 Fields write_done	 */
#define WRITE_DONE1_F1_WIDTH                                                  1
#define WRITE_DONE1_F1_SHIFT                                                 31
#define WRITE_DONE1_F1_MASK                                          0x80000000
#define WRITE_DONE1_F1_RD(src)                       (((src) & 0x80000000)>>31)
#define WRITE_DONE1_F1_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields read_done	 */
#define READ_DONE1_F1_WIDTH                                                   1
#define READ_DONE1_F1_SHIFT                                                  30
#define READ_DONE1_F1_MASK                                           0x40000000
#define READ_DONE1_F1_RD(src)                        (((src) & 0x40000000)>>30)
#define READ_DONE1_F1_SET(dst,src) \
                      (((dst) & ~0x40000000) | (((u32)(src)<<30) & 0x40000000))

/*	Global Base Address	*/
#define ENET_PTP_CSR_BASE_ADDR			0xddd8a1000ULL

/*    Address ENET_PTP_CSR  Registers */
#define PTP_VERSION_ADDR                                             0x00000000
#define PTP_VERSION_DEFAULT                                          0x20000000
#define PTP_CONTROL_R1_ADDR                                          0x00000004
#define PTP_CONTROL_R1_DEFAULT                                       0x00000000
#define PTP_PIT_CONTROL_ADDR                                         0x00000008
#define PTP_PIT_CONTROL_DEFAULT                                      0x00000000
#define PTP_PIT_CONTROL2_ADDR                                        0x0000000c
#define PTP_PIT_CONTROL2_DEFAULT                                     0x00000000
#define PTP_SYSTEMTIME_LO_ADDR                                       0x00000010
#define PTP_SYSTEMTIME_LO_DEFAULT                                    0x00000000
#define PTP_SYSTEMTIME_MID_ADDR                                      0x00000014
#define PTP_SYSTEMTIME_MID_DEFAULT                                   0x00000000
#define PTP_SYSTEMTIME_HI_ADDR                                       0x00000018
#define PTP_SYSTEMTIME_HI_DEFAULT                                    0x00000000
#define PTP_ADDEND_ADDR                                              0x0000001c
#define PTP_ADDEND_DEFAULT                                           0x00000000
#define PTP_ACCUM_ADDR                                               0x00000020
#define PTP_ACCUM_DEFAULT                                            0x00000000
#define PTP_TRGT_LO_0_ADDR                                           0x00000024
#define PTP_TRGT_LO_0_DEFAULT                                        0x00000000
#define PTP_TRGT_MID_0_ADDR                                          0x00000028
#define PTP_TRGT_MID_0_DEFAULT                                       0x00000000
#define PTP_TRGT_HI_0_ADDR                                           0x0000002c
#define PTP_TRGT_HI_0_DEFAULT                                        0x00000000
#define PTP_TRGT_LO_1_ADDR                                           0x00000030
#define PTP_TRGT_LO_1_DEFAULT                                        0x00000000
#define PTP_TRGT_MID_1_ADDR                                          0x00000034
#define PTP_TRGT_MID_1_DEFAULT                                       0x00000000
#define PTP_TRGT_HI_1_ADDR                                           0x00000038
#define PTP_TRGT_HI_1_DEFAULT                                        0x00000000
#define PTP_PITCOUNT_0_ADDR                                          0x0000003c
#define PTP_PITCOUNT_0_DEFAULT                                       0x00000000
#define PTP_PITRELOAD_LO_0_ADDR                                      0x00000040
#define PTP_PITRELOAD_LO_0_DEFAULT                                   0x00000000
#define PTP_PITRELOAD_HI_0_ADDR                                      0x00000044
#define PTP_PITRELOAD_HI_0_DEFAULT                                   0x00000000
#define PTP_PITCOUNT_1_ADDR                                          0x00000048
#define PTP_PITCOUNT_1_DEFAULT                                       0x00000000
#define PTP_PITRELOAD_LO_1_ADDR                                      0x0000004c
#define PTP_PITRELOAD_LO_1_DEFAULT                                   0x00000000
#define PTP_PITRELOAD_HI_1_ADDR                                      0x00000050
#define PTP_PITRELOAD_HI_1_DEFAULT                                   0x00000000
#define PTP_PITCOUNT_2_ADDR                                          0x00000054
#define PTP_PITCOUNT_2_DEFAULT                                       0x00000000
#define PTP_PITRELOAD_LO_2_ADDR                                      0x00000058
#define PTP_PITRELOAD_LO_2_DEFAULT                                   0x00000000
#define PTP_PITRELOAD_HI_2_ADDR                                      0x0000005c
#define PTP_PITRELOAD_HI_2_DEFAULT                                   0x00000000
#define PTP_PITCOUNT_3_ADDR                                          0x00000060
#define PTP_PITCOUNT_3_DEFAULT                                       0x00000000
#define PTP_PITRELOAD_LO_3_ADDR                                      0x00000064
#define PTP_PITRELOAD_LO_3_DEFAULT                                   0x00000000
#define PTP_PITRELOAD_HI_3_ADDR                                      0x00000068
#define PTP_PITRELOAD_HI_3_DEFAULT                                   0x00000000
#define PTP_PITCOUNT_4_ADDR                                          0x0000006c
#define PTP_PITCOUNT_4_DEFAULT                                       0x00000000
#define PTP_PITRELOAD_LO_4_ADDR                                      0x00000070
#define PTP_PITRELOAD_LO_4_DEFAULT                                   0x00000000
#define PTP_PITRELOAD_HI_4_ADDR                                      0x00000074
#define PTP_PITRELOAD_HI_4_DEFAULT                                   0x00000000
#define PTP_PITRELOAD_PS_LO_ADDR                                     0x00000078
#define PTP_PITRELOAD_PS_LO_DEFAULT                                  0x00000000
#define PTP_PITRELOAD_PS_HI_ADDR                                     0x0000007c
#define PTP_PITRELOAD_PS_HI_DEFAULT                                  0x00000000
#define PTP_CAP_SYSTIME_LO_ADDR                                      0x00000080
#define PTP_CAP_SYSTIME_LO_DEFAULT                                   0x00000000
#define PTP_CAP_SYSTIME_MID_ADDR                                     0x00000084
#define PTP_CAP_SYSTIME_MID_DEFAULT                                  0x00000000
#define PTP_CAP_SYSTIME_HI_ADDR                                      0x00000088
#define PTP_CAP_SYSTIME_HI_DEFAULT                                   0x00000000
#define PTP_TOD_AMT_LO_ADDR                                          0x0000008c
#define PTP_TOD_AMT_LO_DEFAULT                                       0x00000000
#define PTP_CONTROL_SYSTIME_ADDR                                     0x00000090
#define PTP_CONTROL_SYSTIME_DEFAULT                                  0x00000000
#define PTP_SYSTEMTIME_STAT_LO_ADDR                                  0x00000094
#define PTP_SYSTEMTIME_STAT_LO_DEFAULT                               0x00000000
#define PTP_SYSTEMTIME_STAT_MID_ADDR                                 0x00000098
#define PTP_SYSTEMTIME_STAT_MID_DEFAULT                              0x00000000
#define PTP_SYSTEMTIME_STAT_HI_ADDR                                  0x0000009c
#define PTP_SYSTEMTIME_STAT_HI_DEFAULT                               0x00000000
#define PTP_TMSTMP_CFG_ADDR                                          0x000000a0
#define PTP_TMSTMP_CFG_DEFAULT                                       0x000c0100
#define PTP_EVENT_ADDR                                               0x000000a4
#define PTP_EVENT_DEFAULT                                            0x00000000
#define PTP_EVENTMASK_ADDR                                           0x000000a8

/*	Register PTP_Version	*/ 
/*	 Fields REV	 */
#define REV_F2_WIDTH                                                          4
#define REV_F2_SHIFT                                                         28
#define REV_F2_MASK                                                  0xf0000000
#define REV_F2_RD(src)                               (((src) & 0xf0000000)>>28)
#define REV_F2_SET(dst,src) \
                      (((dst) & ~0xf0000000) | (((u32)(src)<<28) & 0xf0000000))

/*	Register PTP_Control	*/ 
/*	 Fields RST	 */
#define RST_F2_WIDTH                                                          1
#define RST_F2_SHIFT                                                         31
#define RST_F2_MASK                                                  0x80000000
#define RST_F2_RD(src)                               (((src) & 0x80000000)>>31)
#define RST_F2_WR(src)                          (((u32)(src)<<31) & 0x80000000)
#define RST_F2_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields GRNDMSTR	 */
#define GRNDMSTR_WIDTH                                                        1
#define GRNDMSTR_SHIFT                                                       30
#define GRNDMSTR_MASK                                                0x40000000
#define GRNDMSTR_RD(src)                             (((src) & 0x40000000)>>30)
#define GRNDMSTR_WR(src)                        (((u32)(src)<<30) & 0x40000000)
#define GRNDMSTR_SET(dst,src) \
                      (((dst) & ~0x40000000) | (((u32)(src)<<30) & 0x40000000))
/*	 Fields ACCBP	 */
#define ACCBP_WIDTH                                                           1
#define ACCBP_SHIFT                                                          29
#define ACCBP_MASK                                                   0x20000000
#define ACCBP_RD(src)                                (((src) & 0x20000000)>>29)
#define ACCBP_WR(src)                           (((u32)(src)<<29) & 0x20000000)
#define ACCBP_SET(dst,src) \
                      (((dst) & ~0x20000000) | (((u32)(src)<<29) & 0x20000000))
/*	 Fields TT0_MODE	 */
#define TT0_MODE_WIDTH                                                        1
#define TT0_MODE_SHIFT                                                       28
#define TT0_MODE_MASK                                                0x10000000
#define TT0_MODE_RD(src)                             (((src) & 0x10000000)>>28)
#define TT0_MODE_WR(src)                        (((u32)(src)<<28) & 0x10000000)
#define TT0_MODE_SET(dst,src) \
                      (((dst) & ~0x10000000) | (((u32)(src)<<28) & 0x10000000))
/*	 Fields TT1_MODE	 */
#define TT1_MODE_WIDTH                                                        1
#define TT1_MODE_SHIFT                                                       27
#define TT1_MODE_MASK                                                0x08000000
#define TT1_MODE_RD(src)                             (((src) & 0x08000000)>>27)
#define TT1_MODE_WR(src)                        (((u32)(src)<<27) & 0x08000000)
#define TT1_MODE_SET(dst,src) \
                      (((dst) & ~0x08000000) | (((u32)(src)<<27) & 0x08000000))
/*	 Fields FITSEL0	 */
#define FITSEL0_WIDTH                                                         7
#define FITSEL0_SHIFT                                                        20
#define FITSEL0_MASK                                                 0x07f00000
#define FITSEL0_RD(src)                              (((src) & 0x07f00000)>>20)
#define FITSEL0_WR(src)                         (((u32)(src)<<20) & 0x07f00000)
#define FITSEL0_SET(dst,src) \
                      (((dst) & ~0x07f00000) | (((u32)(src)<<20) & 0x07f00000))
/*	 Fields FITSEL1	 */
#define FITSEL1_WIDTH                                                         7
#define FITSEL1_SHIFT                                                        13
#define FITSEL1_MASK                                                 0x000fe000
#define FITSEL1_RD(src)                              (((src) & 0x000fe000)>>13)
#define FITSEL1_WR(src)                         (((u32)(src)<<13) & 0x000fe000)
#define FITSEL1_SET(dst,src) \
                      (((dst) & ~0x000fe000) | (((u32)(src)<<13) & 0x000fe000))
/*	 Fields TSEL	 */
#define TSEL_WIDTH                                                            4
#define TSEL_SHIFT                                                            9
#define TSEL_MASK                                                    0x00001e00
#define TSEL_RD(src)                                  (((src) & 0x00001e00)>>9)
#define TSEL_WR(src)                             (((u32)(src)<<9) & 0x00001e00)
#define TSEL_SET(dst,src) \
                       (((dst) & ~0x00001e00) | (((u32)(src)<<9) & 0x00001e00))
/*	 Fields FITINTTYPE0	 */
#define FITINTTYPE0_WIDTH                                                     2
#define FITINTTYPE0_SHIFT                                                     7
#define FITINTTYPE0_MASK                                             0x00000180
#define FITINTTYPE0_RD(src)                           (((src) & 0x00000180)>>7)
#define FITINTTYPE0_WR(src)                      (((u32)(src)<<7) & 0x00000180)
#define FITINTTYPE0_SET(dst,src) \
                       (((dst) & ~0x00000180) | (((u32)(src)<<7) & 0x00000180))
/*	 Fields FITINTTYPE1	 */
#define FITINTTYPE1_WIDTH                                                     2
#define FITINTTYPE1_SHIFT                                                     5
#define FITINTTYPE1_MASK                                             0x00000060
#define FITINTTYPE1_RD(src)                           (((src) & 0x00000060)>>5)
#define FITINTTYPE1_WR(src)                      (((u32)(src)<<5) & 0x00000060)
#define FITINTTYPE1_SET(dst,src) \
                       (((dst) & ~0x00000060) | (((u32)(src)<<5) & 0x00000060))
/*	 Fields TOD_Inc_dec	 */
#define TOD_INC_DEC_WIDTH                                                     1
#define TOD_INC_DEC_SHIFT                                                     4
#define TOD_INC_DEC_MASK                                             0x00000010
#define TOD_INC_DEC_RD(src)                           (((src) & 0x00000010)>>4)
#define TOD_INC_DEC_WR(src)                      (((u32)(src)<<4) & 0x00000010)
#define TOD_INC_DEC_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields Capture_mod	 */
#define CAPTURE_MOD_WIDTH                                                     2
#define CAPTURE_MOD_SHIFT                                                     2
#define CAPTURE_MOD_MASK                                             0x0000000c
#define CAPTURE_MOD_RD(src)                           (((src) & 0x0000000c)>>2)
#define CAPTURE_MOD_WR(src)                      (((u32)(src)<<2) & 0x0000000c)
#define CAPTURE_MOD_SET(dst,src) \
                       (((dst) & ~0x0000000c) | (((u32)(src)<<2) & 0x0000000c))
/*	 Fields GenTrig_PulseW	 */
#define GENTRIG_PULSEW_WIDTH                                                  2
#define GENTRIG_PULSEW_SHIFT                                                  0
#define GENTRIG_PULSEW_MASK                                          0x00000003
#define GENTRIG_PULSEW_RD(src)                           (((src) & 0x00000003))
#define GENTRIG_PULSEW_WR(src)                      (((u32)(src)) & 0x00000003)
#define GENTRIG_PULSEW_SET(dst,src) \
                          (((dst) & ~0x00000003) | (((u32)(src)) & 0x00000003))

/*	Register PTP_PIT_Control	*/ 
/*	 Fields PITAREN0	 */
#define PITAREN0_WIDTH                                                        1
#define PITAREN0_SHIFT                                                       31
#define PITAREN0_MASK                                                0x80000000
#define PITAREN0_RD(src)                             (((src) & 0x80000000)>>31)
#define PITAREN0_WR(src)                        (((u32)(src)<<31) & 0x80000000)
#define PITAREN0_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields PITAREN1	 */
#define PITAREN1_WIDTH                                                        1
#define PITAREN1_SHIFT                                                       30
#define PITAREN1_MASK                                                0x40000000
#define PITAREN1_RD(src)                             (((src) & 0x40000000)>>30)
#define PITAREN1_WR(src)                        (((u32)(src)<<30) & 0x40000000)
#define PITAREN1_SET(dst,src) \
                      (((dst) & ~0x40000000) | (((u32)(src)<<30) & 0x40000000))
/*	 Fields PITAREN2	 */
#define PITAREN2_WIDTH                                                        1
#define PITAREN2_SHIFT                                                       29
#define PITAREN2_MASK                                                0x20000000
#define PITAREN2_RD(src)                             (((src) & 0x20000000)>>29)
#define PITAREN2_WR(src)                        (((u32)(src)<<29) & 0x20000000)
#define PITAREN2_SET(dst,src) \
                      (((dst) & ~0x20000000) | (((u32)(src)<<29) & 0x20000000))
/*	 Fields PITAREN3	 */
#define PITAREN3_WIDTH                                                        1
#define PITAREN3_SHIFT                                                       28
#define PITAREN3_MASK                                                0x10000000
#define PITAREN3_RD(src)                             (((src) & 0x10000000)>>28)
#define PITAREN3_WR(src)                        (((u32)(src)<<28) & 0x10000000)
#define PITAREN3_SET(dst,src) \
                      (((dst) & ~0x10000000) | (((u32)(src)<<28) & 0x10000000))
/*	 Fields PITAREN4	 */
#define PITAREN4_WIDTH                                                        1
#define PITAREN4_SHIFT                                                       27
#define PITAREN4_MASK                                                0x08000000
#define PITAREN4_RD(src)                             (((src) & 0x08000000)>>27)
#define PITAREN4_WR(src)                        (((u32)(src)<<27) & 0x08000000)
#define PITAREN4_SET(dst,src) \
                      (((dst) & ~0x08000000) | (((u32)(src)<<27) & 0x08000000))
/*	 Fields PITSTATE0	 */
#define PITSTATE0_WIDTH                                                       1
#define PITSTATE0_SHIFT                                                      26
#define PITSTATE0_MASK                                               0x04000000
#define PITSTATE0_RD(src)                            (((src) & 0x04000000)>>26)
#define PITSTATE0_WR(src)                       (((u32)(src)<<26) & 0x04000000)
#define PITSTATE0_SET(dst,src) \
                      (((dst) & ~0x04000000) | (((u32)(src)<<26) & 0x04000000))
/*	 Fields PITSTATE1	 */
#define PITSTATE1_WIDTH                                                       1
#define PITSTATE1_SHIFT                                                      25
#define PITSTATE1_MASK                                               0x02000000
#define PITSTATE1_RD(src)                            (((src) & 0x02000000)>>25)
#define PITSTATE1_WR(src)                       (((u32)(src)<<25) & 0x02000000)
#define PITSTATE1_SET(dst,src) \
                      (((dst) & ~0x02000000) | (((u32)(src)<<25) & 0x02000000))
/*	 Fields PITSTATE2	 */
#define PITSTATE2_WIDTH                                                       1
#define PITSTATE2_SHIFT                                                      24
#define PITSTATE2_MASK                                               0x01000000
#define PITSTATE2_RD(src)                            (((src) & 0x01000000)>>24)
#define PITSTATE2_WR(src)                       (((u32)(src)<<24) & 0x01000000)
#define PITSTATE2_SET(dst,src) \
                      (((dst) & ~0x01000000) | (((u32)(src)<<24) & 0x01000000))
/*	 Fields PITSTATE3	 */
#define PITSTATE3_WIDTH                                                       1
#define PITSTATE3_SHIFT                                                      23
#define PITSTATE3_MASK                                               0x00800000
#define PITSTATE3_RD(src)                            (((src) & 0x00800000)>>23)
#define PITSTATE3_WR(src)                       (((u32)(src)<<23) & 0x00800000)
#define PITSTATE3_SET(dst,src) \
                      (((dst) & ~0x00800000) | (((u32)(src)<<23) & 0x00800000))
/*	 Fields PITSTATE4	 */
#define PITSTATE4_WIDTH                                                       1
#define PITSTATE4_SHIFT                                                      22
#define PITSTATE4_MASK                                               0x00400000
#define PITSTATE4_RD(src)                            (((src) & 0x00400000)>>22)
#define PITSTATE4_WR(src)                       (((u32)(src)<<22) & 0x00400000)
#define PITSTATE4_SET(dst,src) \
                      (((dst) & ~0x00400000) | (((u32)(src)<<22) & 0x00400000))
/*	 Fields PITINTTYPE0	 */
#define PITINTTYPE0_WIDTH                                                     2
#define PITINTTYPE0_SHIFT                                                    20
#define PITINTTYPE0_MASK                                             0x00300000
#define PITINTTYPE0_RD(src)                          (((src) & 0x00300000)>>20)
#define PITINTTYPE0_WR(src)                     (((u32)(src)<<20) & 0x00300000)
#define PITINTTYPE0_SET(dst,src) \
                      (((dst) & ~0x00300000) | (((u32)(src)<<20) & 0x00300000))
/*	 Fields PITINTTYPE1	 */
#define PITINTTYPE1_WIDTH                                                     2
#define PITINTTYPE1_SHIFT                                                    18
#define PITINTTYPE1_MASK                                             0x000c0000
#define PITINTTYPE1_RD(src)                          (((src) & 0x000c0000)>>18)
#define PITINTTYPE1_WR(src)                     (((u32)(src)<<18) & 0x000c0000)
#define PITINTTYPE1_SET(dst,src) \
                      (((dst) & ~0x000c0000) | (((u32)(src)<<18) & 0x000c0000))
/*	 Fields PITINTTYPE2	 */
#define PITINTTYPE2_WIDTH                                                     2
#define PITINTTYPE2_SHIFT                                                    16
#define PITINTTYPE2_MASK                                             0x00030000
#define PITINTTYPE2_RD(src)                          (((src) & 0x00030000)>>16)
#define PITINTTYPE2_WR(src)                     (((u32)(src)<<16) & 0x00030000)
#define PITINTTYPE2_SET(dst,src) \
                      (((dst) & ~0x00030000) | (((u32)(src)<<16) & 0x00030000))
/*	 Fields PITINTTYPE3	 */
#define PITINTTYPE3_WIDTH                                                     2
#define PITINTTYPE3_SHIFT                                                    14
#define PITINTTYPE3_MASK                                             0x0000c000
#define PITINTTYPE3_RD(src)                          (((src) & 0x0000c000)>>14)
#define PITINTTYPE3_WR(src)                     (((u32)(src)<<14) & 0x0000c000)
#define PITINTTYPE3_SET(dst,src) \
                      (((dst) & ~0x0000c000) | (((u32)(src)<<14) & 0x0000c000))
/*	 Fields PITINTTYPE4	 */
#define PITINTTYPE4_WIDTH                                                     2
#define PITINTTYPE4_SHIFT                                                    12
#define PITINTTYPE4_MASK                                             0x00003000
#define PITINTTYPE4_RD(src)                          (((src) & 0x00003000)>>12)
#define PITINTTYPE4_WR(src)                     (((u32)(src)<<12) & 0x00003000)
#define PITINTTYPE4_SET(dst,src) \
                      (((dst) & ~0x00003000) | (((u32)(src)<<12) & 0x00003000))
/*	 Fields PIT_PS_EN0	 */
#define PIT_PS_EN0_WIDTH                                                      1
#define PIT_PS_EN0_SHIFT                                                     11
#define PIT_PS_EN0_MASK                                              0x00000800
#define PIT_PS_EN0_RD(src)                           (((src) & 0x00000800)>>11)
#define PIT_PS_EN0_WR(src)                      (((u32)(src)<<11) & 0x00000800)
#define PIT_PS_EN0_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*	 Fields PIT_PS_EN1	 */
#define PIT_PS_EN1_WIDTH                                                      1
#define PIT_PS_EN1_SHIFT                                                     10
#define PIT_PS_EN1_MASK                                              0x00000400
#define PIT_PS_EN1_RD(src)                           (((src) & 0x00000400)>>10)
#define PIT_PS_EN1_WR(src)                      (((u32)(src)<<10) & 0x00000400)
#define PIT_PS_EN1_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields PIT_PS_EN2	 */
#define PIT_PS_EN2_WIDTH                                                      1
#define PIT_PS_EN2_SHIFT                                                      9
#define PIT_PS_EN2_MASK                                              0x00000200
#define PIT_PS_EN2_RD(src)                            (((src) & 0x00000200)>>9)
#define PIT_PS_EN2_WR(src)                       (((u32)(src)<<9) & 0x00000200)
#define PIT_PS_EN2_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields PIT_PS_EN3	 */
#define PIT_PS_EN3_WIDTH                                                      1
#define PIT_PS_EN3_SHIFT                                                      8
#define PIT_PS_EN3_MASK                                              0x00000100
#define PIT_PS_EN3_RD(src)                            (((src) & 0x00000100)>>8)
#define PIT_PS_EN3_WR(src)                       (((u32)(src)<<8) & 0x00000100)
#define PIT_PS_EN3_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*	 Fields PIT_PS_EN4	 */
#define PIT_PS_EN4_WIDTH                                                      1
#define PIT_PS_EN4_SHIFT                                                      7
#define PIT_PS_EN4_MASK                                              0x00000080
#define PIT_PS_EN4_RD(src)                            (((src) & 0x00000080)>>7)
#define PIT_PS_EN4_WR(src)                       (((u32)(src)<<7) & 0x00000080)
#define PIT_PS_EN4_SET(dst,src) \
                       (((dst) & ~0x00000080) | (((u32)(src)<<7) & 0x00000080))
/*	 Fields PIT_EN0	 */
#define PIT_EN0_WIDTH                                                         1
#define PIT_EN0_SHIFT                                                         6
#define PIT_EN0_MASK                                                 0x00000040
#define PIT_EN0_RD(src)                               (((src) & 0x00000040)>>6)
#define PIT_EN0_WR(src)                          (((u32)(src)<<6) & 0x00000040)
#define PIT_EN0_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*	 Fields PIT_EN1	 */
#define PIT_EN1_WIDTH                                                         1
#define PIT_EN1_SHIFT                                                         5
#define PIT_EN1_MASK                                                 0x00000020
#define PIT_EN1_RD(src)                               (((src) & 0x00000020)>>5)
#define PIT_EN1_WR(src)                          (((u32)(src)<<5) & 0x00000020)
#define PIT_EN1_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields PIT_EN2	 */
#define PIT_EN2_WIDTH                                                         1
#define PIT_EN2_SHIFT                                                         4
#define PIT_EN2_MASK                                                 0x00000010
#define PIT_EN2_RD(src)                               (((src) & 0x00000010)>>4)
#define PIT_EN2_WR(src)                          (((u32)(src)<<4) & 0x00000010)
#define PIT_EN2_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields PIT_EN3	 */
#define PIT_EN3_WIDTH                                                         1
#define PIT_EN3_SHIFT                                                         3
#define PIT_EN3_MASK                                                 0x00000008
#define PIT_EN3_RD(src)                               (((src) & 0x00000008)>>3)
#define PIT_EN3_WR(src)                          (((u32)(src)<<3) & 0x00000008)
#define PIT_EN3_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields PIT_EN4	 */
#define PIT_EN4_WIDTH                                                         1
#define PIT_EN4_SHIFT                                                         2
#define PIT_EN4_MASK                                                 0x00000004
#define PIT_EN4_RD(src)                               (((src) & 0x00000004)>>2)
#define PIT_EN4_WR(src)                          (((u32)(src)<<2) & 0x00000004)
#define PIT_EN4_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))

/*	Register PTP_PIT_Control2	*/ 
/*	 Fields PIT_trig0	 */
#define PIT_TRIG02_WIDTH                                                      2
#define PIT_TRIG02_SHIFT                                                     30
#define PIT_TRIG02_MASK                                              0xc0000000
#define PIT_TRIG02_RD(src)                           (((src) & 0xc0000000)>>30)
#define PIT_TRIG02_WR(src)                      (((u32)(src)<<30) & 0xc0000000)
#define PIT_TRIG02_SET(dst,src) \
                      (((dst) & ~0xc0000000) | (((u32)(src)<<30) & 0xc0000000))
/*	 Fields PIT_trig1	 */
#define PIT_TRIG12_WIDTH                                                      2
#define PIT_TRIG12_SHIFT                                                     28
#define PIT_TRIG12_MASK                                              0x30000000
#define PIT_TRIG12_RD(src)                           (((src) & 0x30000000)>>28)
#define PIT_TRIG12_WR(src)                      (((u32)(src)<<28) & 0x30000000)
#define PIT_TRIG12_SET(dst,src) \
                      (((dst) & ~0x30000000) | (((u32)(src)<<28) & 0x30000000))
/*	 Fields PIT_trig2	 */
#define PIT_TRIG22_WIDTH                                                      2
#define PIT_TRIG22_SHIFT                                                     26
#define PIT_TRIG22_MASK                                              0x0c000000
#define PIT_TRIG22_RD(src)                           (((src) & 0x0c000000)>>26)
#define PIT_TRIG22_WR(src)                      (((u32)(src)<<26) & 0x0c000000)
#define PIT_TRIG22_SET(dst,src) \
                      (((dst) & ~0x0c000000) | (((u32)(src)<<26) & 0x0c000000))
/*	 Fields PIT_trig3	 */
#define PIT_TRIG32_WIDTH                                                      2
#define PIT_TRIG32_SHIFT                                                     24
#define PIT_TRIG32_MASK                                              0x03000000
#define PIT_TRIG32_RD(src)                           (((src) & 0x03000000)>>24)
#define PIT_TRIG32_WR(src)                      (((u32)(src)<<24) & 0x03000000)
#define PIT_TRIG32_SET(dst,src) \
                      (((dst) & ~0x03000000) | (((u32)(src)<<24) & 0x03000000))
/*	 Fields PIT_trig4	 */
#define PIT_TRIG42_WIDTH                                                      2
#define PIT_TRIG42_SHIFT                                                     22
#define PIT_TRIG42_MASK                                              0x00c00000
#define PIT_TRIG42_RD(src)                           (((src) & 0x00c00000)>>22)
#define PIT_TRIG42_WR(src)                      (((u32)(src)<<22) & 0x00c00000)
#define PIT_TRIG42_SET(dst,src) \
                      (((dst) & ~0x00c00000) | (((u32)(src)<<22) & 0x00c00000))

/*	Register PTP_SystemTime_Lo	*/ 
/*	 Fields SYSTIME	 */
#define SYSTIME_WIDTH                                                        32
#define SYSTIME_SHIFT                                                         0
#define SYSTIME_MASK                                                 0xffffffff
#define SYSTIME_RD(src)                                  (((src) & 0xffffffff))
#define SYSTIME_WR(src)                             (((u32)(src)) & 0xffffffff)
#define SYSTIME_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register PTP_SystemTime_Mid	*/ 
/*	 Fields SYSTIME	 */
#define SYSTIME_F1_WIDTH                                                     32
#define SYSTIME_F1_SHIFT                                                      0
#define SYSTIME_F1_MASK                                              0xffffffff
#define SYSTIME_F1_RD(src)                               (((src) & 0xffffffff))
#define SYSTIME_F1_WR(src)                          (((u32)(src)) & 0xffffffff)
#define SYSTIME_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register PTP_SystemTime_Hi	*/ 
/*	 Fields SYSTIME	 */
#define SYSTIME_F2_WIDTH                                                     32
#define SYSTIME_F2_SHIFT                                                      0
#define SYSTIME_F2_MASK                                              0xffffffff
#define SYSTIME_F2_RD(src)                               (((src) & 0xffffffff))
#define SYSTIME_F2_WR(src)                          (((u32)(src)) & 0xffffffff)
#define SYSTIME_F2_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register PTP_Addend	*/ 
/*	 Fields ADDEND	 */
#define ADDEND_WIDTH                                                         32
#define ADDEND_SHIFT                                                          0
#define ADDEND_MASK                                                  0xffffffff
#define ADDEND_RD(src)                                   (((src) & 0xffffffff))
#define ADDEND_WR(src)                              (((u32)(src)) & 0xffffffff)
#define ADDEND_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register PTP_Accum	*/ 
/*	 Fields ACCUM	 */
#define ACCUM_WIDTH                                                          32
#define ACCUM_SHIFT                                                           0
#define ACCUM_MASK                                                   0xffffffff
#define ACCUM_RD(src)                                    (((src) & 0xffffffff))
#define ACCUM_WR(src)                               (((u32)(src)) & 0xffffffff)
#define ACCUM_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register PTP_Trgt_Lo_0	*/ 
/*	 Fields TRGT	 */
#define TRGT0_WIDTH                                                          32
#define TRGT0_SHIFT                                                           0
#define TRGT0_MASK                                                   0xffffffff
#define TRGT0_RD(src)                                    (((src) & 0xffffffff))
#define TRGT0_WR(src)                               (((u32)(src)) & 0xffffffff)
#define TRGT0_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register PTP_Trgt_Mid_0	*/ 
/*	 Fields TRGT	 */
#define TRGT0_F1_WIDTH                                                       32
#define TRGT0_F1_SHIFT                                                        0
#define TRGT0_F1_MASK                                                0xffffffff
#define TRGT0_F1_RD(src)                                 (((src) & 0xffffffff))
#define TRGT0_F1_WR(src)                            (((u32)(src)) & 0xffffffff)
#define TRGT0_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register PTP_Trgt_Hi_0	*/ 
/*	 Fields TRGT	 */
#define TRGT0_F2_WIDTH                                                       32
#define TRGT0_F2_SHIFT                                                        0
#define TRGT0_F2_MASK                                                0xffffffff
#define TRGT0_F2_RD(src)                                 (((src) & 0xffffffff))
#define TRGT0_F2_WR(src)                            (((u32)(src)) & 0xffffffff)
#define TRGT0_F2_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register PTP_Trgt_Lo_1	*/ 
/*	 Fields TRGT	 */
#define TRGT1_WIDTH                                                          32
#define TRGT1_SHIFT                                                           0
#define TRGT1_MASK                                                   0xffffffff
#define TRGT1_RD(src)                                    (((src) & 0xffffffff))
#define TRGT1_WR(src)                               (((u32)(src)) & 0xffffffff)
#define TRGT1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register PTP_Trgt_Mid_1	*/ 
/*	 Fields TRGT	 */
#define TRGT1_F1_WIDTH                                                       32
#define TRGT1_F1_SHIFT                                                        0
#define TRGT1_F1_MASK                                                0xffffffff
#define TRGT1_F1_RD(src)                                 (((src) & 0xffffffff))
#define TRGT1_F1_WR(src)                            (((u32)(src)) & 0xffffffff)
#define TRGT1_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register PTP_Trgt_Hi_1	*/ 
/*	 Fields TRGT	 */
#define TRGT1_F2_WIDTH                                                       32
#define TRGT1_F2_SHIFT                                                        0
#define TRGT1_F2_MASK                                                0xffffffff
#define TRGT1_F2_RD(src)                                 (((src) & 0xffffffff))
#define TRGT1_F2_WR(src)                            (((u32)(src)) & 0xffffffff)
#define TRGT1_F2_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register PTP_PITCount_0	*/ 
/*	 Fields CNT	 */
#define CNT0_WIDTH                                                           32
#define CNT0_SHIFT                                                            0
#define CNT0_MASK                                                    0xffffffff
#define CNT0_RD(src)                                     (((src) & 0xffffffff))
#define CNT0_WR(src)                                (((u32)(src)) & 0xffffffff)
#define CNT0_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register PTP_PITReload_Lo_0	*/ 
/*	 Fields RLDVAL	 */
#define RLDVAL0_WIDTH                                                        32
#define RLDVAL0_SHIFT                                                         0
#define RLDVAL0_MASK                                                 0xffffffff
#define RLDVAL0_RD(src)                                  (((src) & 0xffffffff))
#define RLDVAL0_WR(src)                             (((u32)(src)) & 0xffffffff)
#define RLDVAL0_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register PTP_PITReload_Hi_0	*/ 
/*	 Fields RLDVAL	 */
#define RLDVAL0_F1_WIDTH                                                     32
#define RLDVAL0_F1_SHIFT                                                      0
#define RLDVAL0_F1_MASK                                              0xffffffff
#define RLDVAL0_F1_RD(src)                               (((src) & 0xffffffff))
#define RLDVAL0_F1_WR(src)                          (((u32)(src)) & 0xffffffff)
#define RLDVAL0_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register PTP_PITCount_1	*/ 
/*	 Fields CNT	 */
#define CNT1_WIDTH                                                           32
#define CNT1_SHIFT                                                            0
#define CNT1_MASK                                                    0xffffffff
#define CNT1_RD(src)                                     (((src) & 0xffffffff))
#define CNT1_WR(src)                                (((u32)(src)) & 0xffffffff)
#define CNT1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register PTP_PITReload_Lo_1	*/ 
/*	 Fields RLDVAL	 */
#define RLDVAL1_WIDTH                                                        32
#define RLDVAL1_SHIFT                                                         0
#define RLDVAL1_MASK                                                 0xffffffff
#define RLDVAL1_RD(src)                                  (((src) & 0xffffffff))
#define RLDVAL1_WR(src)                             (((u32)(src)) & 0xffffffff)
#define RLDVAL1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register PTP_PITReload_Hi_1	*/ 
/*	 Fields RLDVAL	 */
#define RLDVAL1_F1_WIDTH                                                     32
#define RLDVAL1_F1_SHIFT                                                      0
#define RLDVAL1_F1_MASK                                              0xffffffff
#define RLDVAL1_F1_RD(src)                               (((src) & 0xffffffff))
#define RLDVAL1_F1_WR(src)                          (((u32)(src)) & 0xffffffff)
#define RLDVAL1_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register PTP_PITCount_2	*/ 
/*	 Fields CNT	 */
#define CNT2_WIDTH                                                           32
#define CNT2_SHIFT                                                            0
#define CNT2_MASK                                                    0xffffffff
#define CNT2_RD(src)                                     (((src) & 0xffffffff))
#define CNT2_WR(src)                                (((u32)(src)) & 0xffffffff)
#define CNT2_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register PTP_PITReload_Lo_2	*/ 
/*	 Fields RLDVAL	 */
#define RLDVAL2_WIDTH                                                        32
#define RLDVAL2_SHIFT                                                         0
#define RLDVAL2_MASK                                                 0xffffffff
#define RLDVAL2_RD(src)                                  (((src) & 0xffffffff))
#define RLDVAL2_WR(src)                             (((u32)(src)) & 0xffffffff)
#define RLDVAL2_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register PTP_PITReload_Hi_2	*/ 
/*	 Fields RLDVAL	 */
#define RLDVAL2_F1_WIDTH                                                     32
#define RLDVAL2_F1_SHIFT                                                      0
#define RLDVAL2_F1_MASK                                              0xffffffff
#define RLDVAL2_F1_RD(src)                               (((src) & 0xffffffff))
#define RLDVAL2_F1_WR(src)                          (((u32)(src)) & 0xffffffff)
#define RLDVAL2_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register PTP_PITCount_3	*/ 
/*	 Fields CNT	 */
#define CNT3_WIDTH                                                           32
#define CNT3_SHIFT                                                            0
#define CNT3_MASK                                                    0xffffffff
#define CNT3_RD(src)                                     (((src) & 0xffffffff))
#define CNT3_WR(src)                                (((u32)(src)) & 0xffffffff)
#define CNT3_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register PTP_PITReload_Lo_3	*/ 
/*	 Fields RLDVAL	 */
#define RLDVAL3_WIDTH                                                        32
#define RLDVAL3_SHIFT                                                         0
#define RLDVAL3_MASK                                                 0xffffffff
#define RLDVAL3_RD(src)                                  (((src) & 0xffffffff))
#define RLDVAL3_WR(src)                             (((u32)(src)) & 0xffffffff)
#define RLDVAL3_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register PTP_PITReload_Hi_3	*/ 
/*	 Fields RLDVAL	 */
#define RLDVAL3_F1_WIDTH                                                     32
#define RLDVAL3_F1_SHIFT                                                      0
#define RLDVAL3_F1_MASK                                              0xffffffff
#define RLDVAL3_F1_RD(src)                               (((src) & 0xffffffff))
#define RLDVAL3_F1_WR(src)                          (((u32)(src)) & 0xffffffff)
#define RLDVAL3_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register PTP_PITCount_4	*/ 
/*	 Fields CNT	 */
#define CNT4_WIDTH                                                           32
#define CNT4_SHIFT                                                            0
#define CNT4_MASK                                                    0xffffffff
#define CNT4_RD(src)                                     (((src) & 0xffffffff))
#define CNT4_WR(src)                                (((u32)(src)) & 0xffffffff)
#define CNT4_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register PTP_PITReload_Lo_4	*/ 
/*	 Fields RLDVAL	 */
#define RLDVAL4_WIDTH                                                        32
#define RLDVAL4_SHIFT                                                         0
#define RLDVAL4_MASK                                                 0xffffffff
#define RLDVAL4_RD(src)                                  (((src) & 0xffffffff))
#define RLDVAL4_WR(src)                             (((u32)(src)) & 0xffffffff)
#define RLDVAL4_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register PTP_PITReload_Hi_4	*/ 
/*	 Fields RLDVAL	 */
#define RLDVAL4_F1_WIDTH                                                     32
#define RLDVAL4_F1_SHIFT                                                      0
#define RLDVAL4_F1_MASK                                              0xffffffff
#define RLDVAL4_F1_RD(src)                               (((src) & 0xffffffff))
#define RLDVAL4_F1_WR(src)                          (((u32)(src)) & 0xffffffff)
#define RLDVAL4_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register PTP_PITReload_ps_Lo	*/ 
/*	 Fields RLDVAL_0	 */
#define RLDVAL_0_WIDTH                                                        3
#define RLDVAL_0_SHIFT                                                       29
#define RLDVAL_0_MASK                                                0xe0000000
#define RLDVAL_0_RD(src)                             (((src) & 0xe0000000)>>29)
#define RLDVAL_0_WR(src)                        (((u32)(src)<<29) & 0xe0000000)
#define RLDVAL_0_SET(dst,src) \
                      (((dst) & ~0xe0000000) | (((u32)(src)<<29) & 0xe0000000))
/*	 Fields RLDVAL_1	 */
#define RLDVAL_1_WIDTH                                                        3
#define RLDVAL_1_SHIFT                                                       26
#define RLDVAL_1_MASK                                                0x1c000000
#define RLDVAL_1_RD(src)                             (((src) & 0x1c000000)>>26)
#define RLDVAL_1_WR(src)                        (((u32)(src)<<26) & 0x1c000000)
#define RLDVAL_1_SET(dst,src) \
                      (((dst) & ~0x1c000000) | (((u32)(src)<<26) & 0x1c000000))
/*	 Fields RLDVAL_2	 */
#define RLDVAL_2_WIDTH                                                        3
#define RLDVAL_2_SHIFT                                                       23
#define RLDVAL_2_MASK                                                0x03800000
#define RLDVAL_2_RD(src)                             (((src) & 0x03800000)>>23)
#define RLDVAL_2_WR(src)                        (((u32)(src)<<23) & 0x03800000)
#define RLDVAL_2_SET(dst,src) \
                      (((dst) & ~0x03800000) | (((u32)(src)<<23) & 0x03800000))
/*	 Fields RLDVAL_3	 */
#define RLDVAL_3_WIDTH                                                        3
#define RLDVAL_3_SHIFT                                                       20
#define RLDVAL_3_MASK                                                0x00700000
#define RLDVAL_3_RD(src)                             (((src) & 0x00700000)>>20)
#define RLDVAL_3_WR(src)                        (((u32)(src)<<20) & 0x00700000)
#define RLDVAL_3_SET(dst,src) \
                      (((dst) & ~0x00700000) | (((u32)(src)<<20) & 0x00700000))
/*	 Fields RLDVAL_4	 */
#define RLDVAL_4_WIDTH                                                        3
#define RLDVAL_4_SHIFT                                                       17
#define RLDVAL_4_MASK                                                0x000e0000
#define RLDVAL_4_RD(src)                             (((src) & 0x000e0000)>>17)
#define RLDVAL_4_WR(src)                        (((u32)(src)<<17) & 0x000e0000)
#define RLDVAL_4_SET(dst,src) \
                      (((dst) & ~0x000e0000) | (((u32)(src)<<17) & 0x000e0000))

/*	Register PTP_PITReload_ps_Hi	*/ 
/*	 Fields RLDVAL_0	 */
#define RLDVAL_0_F1_WIDTH                                                     3
#define RLDVAL_0_F1_SHIFT                                                    29
#define RLDVAL_0_F1_MASK                                             0xe0000000
#define RLDVAL_0_F1_RD(src)                          (((src) & 0xe0000000)>>29)
#define RLDVAL_0_F1_WR(src)                     (((u32)(src)<<29) & 0xe0000000)
#define RLDVAL_0_F1_SET(dst,src) \
                      (((dst) & ~0xe0000000) | (((u32)(src)<<29) & 0xe0000000))
/*	 Fields RLDVAL_1	 */
#define RLDVAL_1_F1_WIDTH                                                     3
#define RLDVAL_1_F1_SHIFT                                                    26
#define RLDVAL_1_F1_MASK                                             0x1c000000
#define RLDVAL_1_F1_RD(src)                          (((src) & 0x1c000000)>>26)
#define RLDVAL_1_F1_WR(src)                     (((u32)(src)<<26) & 0x1c000000)
#define RLDVAL_1_F1_SET(dst,src) \
                      (((dst) & ~0x1c000000) | (((u32)(src)<<26) & 0x1c000000))
/*	 Fields RLDVAL_2	 */
#define RLDVAL_2_F1_WIDTH                                                     3
#define RLDVAL_2_F1_SHIFT                                                    23
#define RLDVAL_2_F1_MASK                                             0x03800000
#define RLDVAL_2_F1_RD(src)                          (((src) & 0x03800000)>>23)
#define RLDVAL_2_F1_WR(src)                     (((u32)(src)<<23) & 0x03800000)
#define RLDVAL_2_F1_SET(dst,src) \
                      (((dst) & ~0x03800000) | (((u32)(src)<<23) & 0x03800000))
/*	 Fields RLDVAL_3	 */
#define RLDVAL_3_F1_WIDTH                                                     3
#define RLDVAL_3_F1_SHIFT                                                    20
#define RLDVAL_3_F1_MASK                                             0x00700000
#define RLDVAL_3_F1_RD(src)                          (((src) & 0x00700000)>>20)
#define RLDVAL_3_F1_WR(src)                     (((u32)(src)<<20) & 0x00700000)
#define RLDVAL_3_F1_SET(dst,src) \
                      (((dst) & ~0x00700000) | (((u32)(src)<<20) & 0x00700000))
/*	 Fields RLDVAL_4	 */
#define RLDVAL_4_F1_WIDTH                                                     3
#define RLDVAL_4_F1_SHIFT                                                    17
#define RLDVAL_4_F1_MASK                                             0x000e0000
#define RLDVAL_4_F1_RD(src)                          (((src) & 0x000e0000)>>17)
#define RLDVAL_4_F1_WR(src)                     (((u32)(src)<<17) & 0x000e0000)
#define RLDVAL_4_F1_SET(dst,src) \
                      (((dst) & ~0x000e0000) | (((u32)(src)<<17) & 0x000e0000))

/*	Register PTP_cap_systime_Lo	*/ 
/*	 Fields SYSTIME	 */
#define SYSTIME_F3_WIDTH                                                     32
#define SYSTIME_F3_SHIFT                                                      0
#define SYSTIME_F3_MASK                                              0xffffffff
#define SYSTIME_F3_RD(src)                               (((src) & 0xffffffff))
#define SYSTIME_F3_WR(src)                          (((u32)(src)) & 0xffffffff)
#define SYSTIME_F3_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register PTP_cap_systime_Mid	*/ 
/*	 Fields SYSTIME	 */
#define SYSTIME_F4_WIDTH                                                     32
#define SYSTIME_F4_SHIFT                                                      0
#define SYSTIME_F4_MASK                                              0xffffffff
#define SYSTIME_F4_RD(src)                               (((src) & 0xffffffff))
#define SYSTIME_F4_WR(src)                          (((u32)(src)) & 0xffffffff)
#define SYSTIME_F4_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register PTP_cap_systime_Hi	*/ 
/*	 Fields SYSTIME	 */
#define SYSTIME_F5_WIDTH                                                     32
#define SYSTIME_F5_SHIFT                                                      0
#define SYSTIME_F5_MASK                                              0xffffffff
#define SYSTIME_F5_RD(src)                               (((src) & 0xffffffff))
#define SYSTIME_F5_WR(src)                          (((u32)(src)) & 0xffffffff)
#define SYSTIME_F5_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register PTP_TOD_amt_Lo	*/ 
/*	 Fields SYSTIME	 */
#define SYSTIME_F6_WIDTH                                                     32
#define SYSTIME_F6_SHIFT                                                      0
#define SYSTIME_F6_MASK                                              0xffffffff
#define SYSTIME_F6_RD(src)                               (((src) & 0xffffffff))
#define SYSTIME_F6_WR(src)                          (((u32)(src)) & 0xffffffff)
#define SYSTIME_F6_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register PTP_Control_Systime	*/ 
/*	 Fields inc_amt_ns	 */
#define INC_AMT_NS_WIDTH                                                     16
#define INC_AMT_NS_SHIFT                                                      4
#define INC_AMT_NS_MASK                                              0x000ffff0
#define INC_AMT_NS_RD(src)                            (((src) & 0x000ffff0)>>4)
#define INC_AMT_NS_WR(src)                       (((u32)(src)<<4) & 0x000ffff0)
#define INC_AMT_NS_SET(dst,src) \
                       (((dst) & ~0x000ffff0) | (((u32)(src)<<4) & 0x000ffff0))
/*	 Fields inc_amt_ps_en	 */
#define INC_AMT_PS_EN_WIDTH                                                   1
#define INC_AMT_PS_EN_SHIFT                                                   3
#define INC_AMT_PS_EN_MASK                                           0x00000008
#define INC_AMT_PS_EN_RD(src)                         (((src) & 0x00000008)>>3)
#define INC_AMT_PS_EN_WR(src)                    (((u32)(src)<<3) & 0x00000008)
#define INC_AMT_PS_EN_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields inc_amt_ps	 */
#define INC_AMT_PS_WIDTH                                                      3
#define INC_AMT_PS_SHIFT                                                      0
#define INC_AMT_PS_MASK                                              0x00000007
#define INC_AMT_PS_RD(src)                               (((src) & 0x00000007))
#define INC_AMT_PS_WR(src)                          (((u32)(src)) & 0x00000007)
#define INC_AMT_PS_SET(dst,src) \
                          (((dst) & ~0x00000007) | (((u32)(src)) & 0x00000007))

/*	Register PTP_SystemTime_stat_Lo	*/ 
/*	 Fields SYSTIME	 */
#define SYSTIME_F7_WIDTH                                                     32
#define SYSTIME_F7_SHIFT                                                      0
#define SYSTIME_F7_MASK                                              0xffffffff
#define SYSTIME_F7_RD(src)                               (((src) & 0xffffffff))
#define SYSTIME_F7_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register PTP_SystemTime_stat_Mid	*/ 
/*	 Fields SYSTIME	 */
#define SYSTIME_F8_WIDTH                                                     32
#define SYSTIME_F8_SHIFT                                                      0
#define SYSTIME_F8_MASK                                              0xffffffff
#define SYSTIME_F8_RD(src)                               (((src) & 0xffffffff))
#define SYSTIME_F8_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register PTP_SystemTime_stat_Hi	*/ 
/*	 Fields SYSTIME	 */
#define SYSTIME_F9_WIDTH                                                     32
#define SYSTIME_F9_SHIFT                                                      0
#define SYSTIME_F9_MASK                                              0xffffffff
#define SYSTIME_F9_RD(src)                               (((src) & 0xffffffff))
#define SYSTIME_F9_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register PTP_tmstmp_cfg	*/ 
/*	 Fields single_pkt	 */
#define SINGLE_PKT_WIDTH                                                      1
#define SINGLE_PKT_SHIFT                                                     31
#define SINGLE_PKT_MASK                                              0x80000000
#define SINGLE_PKT_RD(src)                           (((src) & 0x80000000)>>31)
#define SINGLE_PKT_WR(src)                      (((u32)(src)<<31) & 0x80000000)
#define SINGLE_PKT_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields max_unsisec_lat_ns	 */
#define MAX_UNSISEC_LAT_NS_WIDTH                                             12
#define MAX_UNSISEC_LAT_NS_SHIFT                                             19
#define MAX_UNSISEC_LAT_NS_MASK                                      0x7ff80000
#define MAX_UNSISEC_LAT_NS_RD(src)                   (((src) & 0x7ff80000)>>19)
#define MAX_UNSISEC_LAT_NS_WR(src)              (((u32)(src)<<19) & 0x7ff80000)
#define MAX_UNSISEC_LAT_NS_SET(dst,src) \
                      (((dst) & ~0x7ff80000) | (((u32)(src)<<19) & 0x7ff80000))
/*	 Fields max_unsisec_lat_cc	 */
#define MAX_UNSISEC_LAT_CC_WIDTH                                             10
#define MAX_UNSISEC_LAT_CC_SHIFT                                              9
#define MAX_UNSISEC_LAT_CC_MASK                                      0x0007fe00
#define MAX_UNSISEC_LAT_CC_RD(src)                    (((src) & 0x0007fe00)>>9)
#define MAX_UNSISEC_LAT_CC_WR(src)               (((u32)(src)<<9) & 0x0007fe00)
#define MAX_UNSISEC_LAT_CC_SET(dst,src) \
                       (((dst) & ~0x0007fe00) | (((u32)(src)<<9) & 0x0007fe00))
/*	 Fields extra_lat	 */
#define EXTRA_LAT_WIDTH                                                       9
#define EXTRA_LAT_SHIFT                                                       0
#define EXTRA_LAT_MASK                                               0x000001ff
#define EXTRA_LAT_RD(src)                                (((src) & 0x000001ff))
#define EXTRA_LAT_WR(src)                           (((u32)(src)) & 0x000001ff)
#define EXTRA_LAT_SET(dst,src) \
                          (((dst) & ~0x000001ff) | (((u32)(src)) & 0x000001ff))

/*	Register PTP_Event	*/ 
/*	 Fields TTIPEND0	 */
#define TTIPEND0_WIDTH                                                        1
#define TTIPEND0_SHIFT                                                       31
#define TTIPEND0_MASK                                                0x80000000
#define TTIPEND0_RD(src)                             (((src) & 0x80000000)>>31)
#define TTIPEND0_WR(src)                        (((u32)(src)<<31) & 0x80000000)
#define TTIPEND0_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields TTIPEND1	 */
#define TTIPEND1_WIDTH                                                        1
#define TTIPEND1_SHIFT                                                       30
#define TTIPEND1_MASK                                                0x40000000
#define TTIPEND1_RD(src)                             (((src) & 0x40000000)>>30)
#define TTIPEND1_WR(src)                        (((u32)(src)<<30) & 0x40000000)
#define TTIPEND1_SET(dst,src) \
                      (((dst) & ~0x40000000) | (((u32)(src)<<30) & 0x40000000))
/*	 Fields FITPEND0	 */
#define FITPEND0_WIDTH                                                        1
#define FITPEND0_SHIFT                                                       29
#define FITPEND0_MASK                                                0x20000000
#define FITPEND0_RD(src)                             (((src) & 0x20000000)>>29)
#define FITPEND0_WR(src)                        (((u32)(src)<<29) & 0x20000000)
#define FITPEND0_SET(dst,src) \
                      (((dst) & ~0x20000000) | (((u32)(src)<<29) & 0x20000000))
/*	 Fields FITPEND1	 */
#define FITPEND1_WIDTH                                                        1
#define FITPEND1_SHIFT                                                       28
#define FITPEND1_MASK                                                0x10000000
#define FITPEND1_RD(src)                             (((src) & 0x10000000)>>28)
#define FITPEND1_WR(src)                        (((u32)(src)<<28) & 0x10000000)
#define FITPEND1_SET(dst,src) \
                      (((dst) & ~0x10000000) | (((u32)(src)<<28) & 0x10000000))
/*	 Fields PITPEND0	 */
#define PITPEND0_WIDTH                                                        1
#define PITPEND0_SHIFT                                                       27
#define PITPEND0_MASK                                                0x08000000
#define PITPEND0_RD(src)                             (((src) & 0x08000000)>>27)
#define PITPEND0_WR(src)                        (((u32)(src)<<27) & 0x08000000)
#define PITPEND0_SET(dst,src) \
                      (((dst) & ~0x08000000) | (((u32)(src)<<27) & 0x08000000))
/*	 Fields PITPEND1	 */
#define PITPEND1_WIDTH                                                        1
#define PITPEND1_SHIFT                                                       26
#define PITPEND1_MASK                                                0x04000000
#define PITPEND1_RD(src)                             (((src) & 0x04000000)>>26)
#define PITPEND1_WR(src)                        (((u32)(src)<<26) & 0x04000000)
#define PITPEND1_SET(dst,src) \
                      (((dst) & ~0x04000000) | (((u32)(src)<<26) & 0x04000000))
/*	 Fields PITPEND2	 */
#define PITPEND2_WIDTH                                                        1
#define PITPEND2_SHIFT                                                       25
#define PITPEND2_MASK                                                0x02000000
#define PITPEND2_RD(src)                             (((src) & 0x02000000)>>25)
#define PITPEND2_WR(src)                        (((u32)(src)<<25) & 0x02000000)
#define PITPEND2_SET(dst,src) \
                      (((dst) & ~0x02000000) | (((u32)(src)<<25) & 0x02000000))
/*	 Fields PITPEND3	 */
#define PITPEND3_WIDTH                                                        1
#define PITPEND3_SHIFT                                                       24
#define PITPEND3_MASK                                                0x01000000
#define PITPEND3_RD(src)                             (((src) & 0x01000000)>>24)
#define PITPEND3_WR(src)                        (((u32)(src)<<24) & 0x01000000)
#define PITPEND3_SET(dst,src) \
                      (((dst) & ~0x01000000) | (((u32)(src)<<24) & 0x01000000))
/*	 Fields PITPEND4	 */
#define PITPEND4_WIDTH                                                        1
#define PITPEND4_SHIFT                                                       23
#define PITPEND4_MASK                                                0x00800000
#define PITPEND4_RD(src)                             (((src) & 0x00800000)>>23)
#define PITPEND4_WR(src)                        (((u32)(src)<<23) & 0x00800000)
#define PITPEND4_SET(dst,src) \
                      (((dst) & ~0x00800000) | (((u32)(src)<<23) & 0x00800000))
/*	 Fields OVRF_EN_1	 */
#define OVRF_EN_1_WIDTH                                                       1
#define OVRF_EN_1_SHIFT                                                      22
#define OVRF_EN_1_MASK                                               0x00400000
#define OVRF_EN_1_RD(src)                            (((src) & 0x00400000)>>22)
#define OVRF_EN_1_WR(src)                       (((u32)(src)<<22) & 0x00400000)
#define OVRF_EN_1_SET(dst,src) \
                      (((dst) & ~0x00400000) | (((u32)(src)<<22) & 0x00400000))
/*	 Fields UNDF_EN_1	 */
#define UNDF_EN_1_WIDTH                                                       1
#define UNDF_EN_1_SHIFT                                                      21
#define UNDF_EN_1_MASK                                               0x00200000
#define UNDF_EN_1_RD(src)                            (((src) & 0x00200000)>>21)
#define UNDF_EN_1_WR(src)                       (((u32)(src)<<21) & 0x00200000)
#define UNDF_EN_1_SET(dst,src) \
                      (((dst) & ~0x00200000) | (((u32)(src)<<21) & 0x00200000))
/*	 Fields FULL_EN_1	 */
#define FULL_EN_1_WIDTH                                                       1
#define FULL_EN_1_SHIFT                                                      20
#define FULL_EN_1_MASK                                               0x00100000
#define FULL_EN_1_RD(src)                            (((src) & 0x00100000)>>20)
#define FULL_EN_1_WR(src)                       (((u32)(src)<<20) & 0x00100000)
#define FULL_EN_1_SET(dst,src) \
                      (((dst) & ~0x00100000) | (((u32)(src)<<20) & 0x00100000))
/*	 Fields OVRF_EN_0	 */
#define OVRF_EN_0_WIDTH                                                       1
#define OVRF_EN_0_SHIFT                                                      19
#define OVRF_EN_0_MASK                                               0x00080000
#define OVRF_EN_0_RD(src)                            (((src) & 0x00080000)>>19)
#define OVRF_EN_0_WR(src)                       (((u32)(src)<<19) & 0x00080000)
#define OVRF_EN_0_SET(dst,src) \
                      (((dst) & ~0x00080000) | (((u32)(src)<<19) & 0x00080000))
/*	 Fields UNDF_EN_0	 */
#define UNDF_EN_0_WIDTH                                                       1
#define UNDF_EN_0_SHIFT                                                      18
#define UNDF_EN_0_MASK                                               0x00040000
#define UNDF_EN_0_RD(src)                            (((src) & 0x00040000)>>18)
#define UNDF_EN_0_WR(src)                       (((u32)(src)<<18) & 0x00040000)
#define UNDF_EN_0_SET(dst,src) \
                      (((dst) & ~0x00040000) | (((u32)(src)<<18) & 0x00040000))
/*	 Fields FULL_EN_0	 */
#define FULL_EN_0_WIDTH                                                       1
#define FULL_EN_0_SHIFT                                                      17
#define FULL_EN_0_MASK                                               0x00020000
#define FULL_EN_0_RD(src)                            (((src) & 0x00020000)>>17)
#define FULL_EN_0_WR(src)                       (((u32)(src)<<17) & 0x00020000)
#define FULL_EN_0_SET(dst,src) \
                      (((dst) & ~0x00020000) | (((u32)(src)<<17) & 0x00020000))
/*	 Fields CAPPEND	 */
#define CAPPEND_WIDTH                                                         1
#define CAPPEND_SHIFT                                                        16
#define CAPPEND_MASK                                                 0x00010000
#define CAPPEND_RD(src)                              (((src) & 0x00010000)>>16)
#define CAPPEND_WR(src)                         (((u32)(src)<<16) & 0x00010000)
#define CAPPEND_SET(dst,src) \
                      (((dst) & ~0x00010000) | (((u32)(src)<<16) & 0x00010000))

/*	Register PTP_EventMask	*/
/*    Mask Register Fields TTIPEND0Mask    */
#define TTIPEND0MASK_WIDTH                                                    1
#define TTIPEND0MASK_SHIFT                                                   31
#define TTIPEND0MASK_MASK                                            0x80000000
#define TTIPEND0MASK_RD(src)                         (((src) & 0x80000000)>>31)
#define TTIPEND0MASK_WR(src)                    (((u32)(src)<<31) & 0x80000000)
#define TTIPEND0MASK_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*    Mask Register Fields TTIPEND1Mask    */
#define TTIPEND1MASK_WIDTH                                                    1
#define TTIPEND1MASK_SHIFT                                                   30
#define TTIPEND1MASK_MASK                                            0x40000000
#define TTIPEND1MASK_RD(src)                         (((src) & 0x40000000)>>30)
#define TTIPEND1MASK_WR(src)                    (((u32)(src)<<30) & 0x40000000)
#define TTIPEND1MASK_SET(dst,src) \
                      (((dst) & ~0x40000000) | (((u32)(src)<<30) & 0x40000000))
/*    Mask Register Fields FITPEND0Mask    */
#define FITPEND0MASK_WIDTH                                                    1
#define FITPEND0MASK_SHIFT                                                   29
#define FITPEND0MASK_MASK                                            0x20000000
#define FITPEND0MASK_RD(src)                         (((src) & 0x20000000)>>29)
#define FITPEND0MASK_WR(src)                    (((u32)(src)<<29) & 0x20000000)
#define FITPEND0MASK_SET(dst,src) \
                      (((dst) & ~0x20000000) | (((u32)(src)<<29) & 0x20000000))
/*    Mask Register Fields FITPEND1Mask    */
#define FITPEND1MASK_WIDTH                                                    1
#define FITPEND1MASK_SHIFT                                                   28
#define FITPEND1MASK_MASK                                            0x10000000
#define FITPEND1MASK_RD(src)                         (((src) & 0x10000000)>>28)
#define FITPEND1MASK_WR(src)                    (((u32)(src)<<28) & 0x10000000)
#define FITPEND1MASK_SET(dst,src) \
                      (((dst) & ~0x10000000) | (((u32)(src)<<28) & 0x10000000))
/*    Mask Register Fields PITPEND0Mask    */
#define PITPEND0MASK_WIDTH                                                    1
#define PITPEND0MASK_SHIFT                                                   27
#define PITPEND0MASK_MASK                                            0x08000000
#define PITPEND0MASK_RD(src)                         (((src) & 0x08000000)>>27)
#define PITPEND0MASK_WR(src)                    (((u32)(src)<<27) & 0x08000000)
#define PITPEND0MASK_SET(dst,src) \
                      (((dst) & ~0x08000000) | (((u32)(src)<<27) & 0x08000000))
/*    Mask Register Fields PITPEND1Mask    */
#define PITPEND1MASK_WIDTH                                                    1
#define PITPEND1MASK_SHIFT                                                   26
#define PITPEND1MASK_MASK                                            0x04000000
#define PITPEND1MASK_RD(src)                         (((src) & 0x04000000)>>26)
#define PITPEND1MASK_WR(src)                    (((u32)(src)<<26) & 0x04000000)
#define PITPEND1MASK_SET(dst,src) \
                      (((dst) & ~0x04000000) | (((u32)(src)<<26) & 0x04000000))
/*    Mask Register Fields PITPEND2Mask    */
#define PITPEND2MASK_WIDTH                                                    1
#define PITPEND2MASK_SHIFT                                                   25
#define PITPEND2MASK_MASK                                            0x02000000
#define PITPEND2MASK_RD(src)                         (((src) & 0x02000000)>>25)
#define PITPEND2MASK_WR(src)                    (((u32)(src)<<25) & 0x02000000)
#define PITPEND2MASK_SET(dst,src) \
                      (((dst) & ~0x02000000) | (((u32)(src)<<25) & 0x02000000))
/*    Mask Register Fields PITPEND3Mask    */
#define PITPEND3MASK_WIDTH                                                    1
#define PITPEND3MASK_SHIFT                                                   24
#define PITPEND3MASK_MASK                                            0x01000000
#define PITPEND3MASK_RD(src)                         (((src) & 0x01000000)>>24)
#define PITPEND3MASK_WR(src)                    (((u32)(src)<<24) & 0x01000000)
#define PITPEND3MASK_SET(dst,src) \
                      (((dst) & ~0x01000000) | (((u32)(src)<<24) & 0x01000000))
/*    Mask Register Fields PITPEND4Mask    */
#define PITPEND4MASK_WIDTH                                                    1
#define PITPEND4MASK_SHIFT                                                   23
#define PITPEND4MASK_MASK                                            0x00800000
#define PITPEND4MASK_RD(src)                         (((src) & 0x00800000)>>23)
#define PITPEND4MASK_WR(src)                    (((u32)(src)<<23) & 0x00800000)
#define PITPEND4MASK_SET(dst,src) \
                      (((dst) & ~0x00800000) | (((u32)(src)<<23) & 0x00800000))
/*    Mask Register Fields OVRF_EN_1Mask    */
#define OVRF_EN_1MASK_WIDTH                                                   1
#define OVRF_EN_1MASK_SHIFT                                                  22
#define OVRF_EN_1MASK_MASK                                           0x00400000
#define OVRF_EN_1MASK_RD(src)                        (((src) & 0x00400000)>>22)
#define OVRF_EN_1MASK_WR(src)                   (((u32)(src)<<22) & 0x00400000)
#define OVRF_EN_1MASK_SET(dst,src) \
                      (((dst) & ~0x00400000) | (((u32)(src)<<22) & 0x00400000))
/*    Mask Register Fields UNDF_EN_1Mask    */
#define UNDF_EN_1MASK_WIDTH                                                   1
#define UNDF_EN_1MASK_SHIFT                                                  21
#define UNDF_EN_1MASK_MASK                                           0x00200000
#define UNDF_EN_1MASK_RD(src)                        (((src) & 0x00200000)>>21)
#define UNDF_EN_1MASK_WR(src)                   (((u32)(src)<<21) & 0x00200000)
#define UNDF_EN_1MASK_SET(dst,src) \
                      (((dst) & ~0x00200000) | (((u32)(src)<<21) & 0x00200000))
/*    Mask Register Fields FULL_EN_1Mask    */
#define FULL_EN_1MASK_WIDTH                                                   1
#define FULL_EN_1MASK_SHIFT                                                  20
#define FULL_EN_1MASK_MASK                                           0x00100000
#define FULL_EN_1MASK_RD(src)                        (((src) & 0x00100000)>>20)
#define FULL_EN_1MASK_WR(src)                   (((u32)(src)<<20) & 0x00100000)
#define FULL_EN_1MASK_SET(dst,src) \
                      (((dst) & ~0x00100000) | (((u32)(src)<<20) & 0x00100000))
/*    Mask Register Fields OVRF_EN_0Mask    */
#define OVRF_EN_0MASK_WIDTH                                                   1
#define OVRF_EN_0MASK_SHIFT                                                  19
#define OVRF_EN_0MASK_MASK                                           0x00080000
#define OVRF_EN_0MASK_RD(src)                        (((src) & 0x00080000)>>19)
#define OVRF_EN_0MASK_WR(src)                   (((u32)(src)<<19) & 0x00080000)
#define OVRF_EN_0MASK_SET(dst,src) \
                      (((dst) & ~0x00080000) | (((u32)(src)<<19) & 0x00080000))
/*    Mask Register Fields UNDF_EN_0Mask    */
#define UNDF_EN_0MASK_WIDTH                                                   1
#define UNDF_EN_0MASK_SHIFT                                                  18
#define UNDF_EN_0MASK_MASK                                           0x00040000
#define UNDF_EN_0MASK_RD(src)                        (((src) & 0x00040000)>>18)
#define UNDF_EN_0MASK_WR(src)                   (((u32)(src)<<18) & 0x00040000)
#define UNDF_EN_0MASK_SET(dst,src) \
                      (((dst) & ~0x00040000) | (((u32)(src)<<18) & 0x00040000))
/*    Mask Register Fields FULL_EN_0Mask    */
#define FULL_EN_0MASK_WIDTH                                                   1
#define FULL_EN_0MASK_SHIFT                                                  17
#define FULL_EN_0MASK_MASK                                           0x00020000
#define FULL_EN_0MASK_RD(src)                        (((src) & 0x00020000)>>17)
#define FULL_EN_0MASK_WR(src)                   (((u32)(src)<<17) & 0x00020000)
#define FULL_EN_0MASK_SET(dst,src) \
                      (((dst) & ~0x00020000) | (((u32)(src)<<17) & 0x00020000))
/*    Mask Register Fields CAPPENDMask    */
#define CAPPENDMASK_WIDTH                                                     1
#define CAPPENDMASK_SHIFT                                                    16
#define CAPPENDMASK_MASK                                             0x00010000
#define CAPPENDMASK_RD(src)                          (((src) & 0x00010000)>>16)
#define CAPPENDMASK_WR(src)                     (((u32)(src)<<16) & 0x00010000)
#define CAPPENDMASK_SET(dst,src) \
                      (((dst) & ~0x00010000) | (((u32)(src)<<16) & 0x00010000))

/*	Global Base Address	*/
#define ENET_UNISEC_CSR_BASE_ADDR			0xddd8a1800ULL

/*    Address ENET_UNISEC_CSR  Registers */
#define UNISEC_VERSION_ADDR                                          0x00000000
#define UNISEC_VERSION_DEFAULT                                       0x00000020
#define UNISEC_CONTROL_PORT0_ADDR                                    0x00000004
#define UNISEC_CONTROL_PORT0_DEFAULT                                 0x00000000
#define UNISEC_CONTROL_PORT1_ADDR                                    0x00000008
#define UNISEC_CONTROL_PORT1_DEFAULT                                 0x00000000
#define UNISEC_CONTROL_PORT2_ADDR                                    0x0000000c
#define UNISEC_CONTROL_PORT2_DEFAULT                                 0x00000000
#define UNISEC_CONTROL_PORT3_ADDR                                    0x00000010
#define UNISEC_CONTROL_PORT3_DEFAULT                                 0x00000000
#define UNISEC_CONTROL_PORT4_ADDR                                    0x00000014
#define UNISEC_CONTROL_PORT4_DEFAULT                                 0x00000000
#define UNISEC_CONTROL_PORT5_ADDR                                    0x00000018
#define UNISEC_CONTROL_PORT5_DEFAULT                                 0x00000000
#define UNISEC_CONTROL_PORT6_ADDR                                    0x0000001c
#define UNISEC_CONTROL_PORT6_DEFAULT                                 0x00000000
#define UNISEC_CONTROL_PORT7_ADDR                                    0x00000020
#define UNISEC_CONTROL_PORT7_DEFAULT                                 0x00000000
#define UNISEC_CONTROL_PORT8_ADDR                                    0x00000024
#define UNISEC_CONTROL_PORT8_DEFAULT                                 0x00000000
#define UNISEC_INT_STS_EN_ADDR                                       0x00000028
#define UNISEC_INT_STS_EN_DEFAULT                                    0x00000000
#define UNISEC_INT_SIG_EN_ADDR                                       0x0000002c
#define UNISEC_INT_SIG_EN_DEFAULT                                    0x00000000
#define UNISEC_INT_STS_ADDR                                          0x00000030
#define UNISEC_INT_STS_DEFAULT                                       0x00000000
#define UNISEC_MACSEC_SA_THRD_ADDR                                   0x00000034
#define UNISEC_MACSEC_SA_THRD_DEFAULT                                0xffffffff
#define UNISEC_IPSEC_SA_THRD_LSB_ADDR                                0x00000038
#define UNISEC_IPSEC_SA_THRD_LSB_DEFAULT                             0xffffffff
#define UNISEC_IPSEC_SA_THRD_MSB_ADDR                                0x0000003c
#define UNISEC_IPSEC_SA_THRD_MSB_DEFAULT                             0xffffffff
#define UNISEC_MACSEC_REPLAY_WINDOW_SIZE_ADDR                        0x00000040
#define UNISEC_MACSEC_REPLAY_WINDOW_SIZE_DEFAULT                     0x00000000
#define UNISEC_IPSEC_REPLAY_WINDOW_SIZE_ADDR                         0x00000044
#define UNISEC_IPSEC_REPLAY_WINDOW_SIZE_DEFAULT                      0x00000000
#define UNISEC_MACSEC_EG_MTU_SIZE_ADDR                               0x00000048
#define UNISEC_MACSEC_EG_MTU_SIZE_DEFAULT                            0x00000000
#define UNISEC_MACSEC_IG_MTU_SIZE_ADDR                               0x0000004c
#define UNISEC_MACSEC_IG_MTU_SIZE_DEFAULT                            0x00000000
#define UNISEC_IPSEC_EG_MTU_SIZE_ADDR                                0x00000050
#define UNISEC_IPSEC_EG_MTU_SIZE_DEFAULT                             0x00000000
#define UNISEC_IPSEC_IG_MTU_SIZE_ADDR                                0x00000054
#define UNISEC_IPSEC_IG_MTU_SIZE_DEFAULT                             0x00000000
#define SA_MACSEC_SOFT_EXPIRE_NUM_REG_ADDR                           0x00000058
#define SA_MACSEC_SOFT_EXPIRE_NUM_REG_DEFAULT                        0x00000000
#define SA_IPSEC_SOFT_EXPIRE_NUM_REG_ADDR                            0x0000005c
#define SA_IPSEC_SOFT_EXPIRE_NUM_REG_DEFAULT                         0x00000000
#define UNISEC_CONTROL_REGISTER_ADDR                                 0x00000060
#define UNISEC_CONTROL_REGISTER_DEFAULT                              0x00000000
#define UNISEC_MIB_ADDRESS_REGISTER_ADDR                             0x00000064
#define UNISEC_MIB_ADDRESS_REGISTER_DEFAULT                          0x00000000
#define UNISEC_MIB_DATA_MSB_REGISTER_ADDR                            0x00000068
#define UNISEC_MIB_DATA_MSB_REGISTER_DEFAULT                         0x00000000
#define UNISEC_MIB_DATA_LSB_REGISTER_ADDR                            0x0000006c
#define UNISEC_MIB_DATA_LSB_REGISTER_DEFAULT                         0x00000000

/*	Register UNIsec_VERSION	*/ 
/*	 Fields UNIsec_REVISION	 */
#define UNISEC_REVISION_WIDTH                                                 8
#define UNISEC_REVISION_SHIFT                                                24
#define UNISEC_REVISION_MASK                                         0xff000000
#define UNISEC_REVISION_RD(src)                      (((src) & 0xff000000)>>24)
#define UNISEC_REVISION_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))
/*	 Fields UNIsec_VERSION	 */
#define UNISEC_VERSION_WIDTH                                                  8
#define UNISEC_VERSION_SHIFT                                                 16
#define UNISEC_VERSION_MASK                                          0x00ff0000
#define UNISEC_VERSION_RD(src)                       (((src) & 0x00ff0000)>>16)
#define UNISEC_VERSION_SET(dst,src) \
                      (((dst) & ~0x00ff0000) | (((u32)(src)<<16) & 0x00ff0000))
/*	 Fields UNIsec_ID	 */
#define UNISEC_ID_WIDTH                                                      16
#define UNISEC_ID_SHIFT                                                       0
#define UNISEC_ID_MASK                                               0x0000ffff
#define UNISEC_ID_RD(src)                                (((src) & 0x0000ffff))
#define UNISEC_ID_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register UNIsec_CONTROL_Port0	*/ 
/*	 Fields ING_FRAME_VLD	 */
#define ING_FRAME_VLD0_WIDTH                                                  1
#define ING_FRAME_VLD0_SHIFT                                                  0
#define ING_FRAME_VLD0_MASK                                          0x00000001
#define ING_FRAME_VLD0_RD(src)                           (((src) & 0x00000001))
#define ING_FRAME_VLD0_WR(src)                      (((u32)(src)) & 0x00000001)
#define ING_FRAME_VLD0_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register UNIsec_CONTROL_Port1	*/ 
/*	 Fields ING_FRAME_VLD	 */
#define ING_FRAME_VLD1_WIDTH                                                  1
#define ING_FRAME_VLD1_SHIFT                                                  0
#define ING_FRAME_VLD1_MASK                                          0x00000001
#define ING_FRAME_VLD1_RD(src)                           (((src) & 0x00000001))
#define ING_FRAME_VLD1_WR(src)                      (((u32)(src)) & 0x00000001)
#define ING_FRAME_VLD1_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register UNIsec_CONTROL_Port2	*/ 
/*	 Fields ING_FRAME_VLD	 */
#define ING_FRAME_VLD2_WIDTH                                                  1
#define ING_FRAME_VLD2_SHIFT                                                  0
#define ING_FRAME_VLD2_MASK                                          0x00000001
#define ING_FRAME_VLD2_RD(src)                           (((src) & 0x00000001))
#define ING_FRAME_VLD2_WR(src)                      (((u32)(src)) & 0x00000001)
#define ING_FRAME_VLD2_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register UNIsec_CONTROL_Port3	*/ 
/*	 Fields ING_FRAME_VLD	 */
#define ING_FRAME_VLD3_WIDTH                                                  1
#define ING_FRAME_VLD3_SHIFT                                                  0
#define ING_FRAME_VLD3_MASK                                          0x00000001
#define ING_FRAME_VLD3_RD(src)                           (((src) & 0x00000001))
#define ING_FRAME_VLD3_WR(src)                      (((u32)(src)) & 0x00000001)
#define ING_FRAME_VLD3_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register UNIsec_CONTROL_Port4	*/ 
/*	 Fields ING_FRAME_VLD	 */
#define ING_FRAME_VLD4_WIDTH                                                  1
#define ING_FRAME_VLD4_SHIFT                                                  0
#define ING_FRAME_VLD4_MASK                                          0x00000001
#define ING_FRAME_VLD4_RD(src)                           (((src) & 0x00000001))
#define ING_FRAME_VLD4_WR(src)                      (((u32)(src)) & 0x00000001)
#define ING_FRAME_VLD4_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register UNIsec_CONTROL_Port5	*/ 
/*	 Fields ING_FRAME_VLD	 */
#define ING_FRAME_VLD5_WIDTH                                                  1
#define ING_FRAME_VLD5_SHIFT                                                  0
#define ING_FRAME_VLD5_MASK                                          0x00000001
#define ING_FRAME_VLD5_RD(src)                           (((src) & 0x00000001))
#define ING_FRAME_VLD5_WR(src)                      (((u32)(src)) & 0x00000001)
#define ING_FRAME_VLD5_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register UNIsec_CONTROL_Port6	*/ 
/*	 Fields ING_FRAME_VLD	 */
#define ING_FRAME_VLD6_WIDTH                                                  1
#define ING_FRAME_VLD6_SHIFT                                                  0
#define ING_FRAME_VLD6_MASK                                          0x00000001
#define ING_FRAME_VLD6_RD(src)                           (((src) & 0x00000001))
#define ING_FRAME_VLD6_WR(src)                      (((u32)(src)) & 0x00000001)
#define ING_FRAME_VLD6_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register UNIsec_CONTROL_Port7	*/ 
/*	 Fields ING_FRAME_VLD	 */
#define ING_FRAME_VLD7_WIDTH                                                  1
#define ING_FRAME_VLD7_SHIFT                                                  0
#define ING_FRAME_VLD7_MASK                                          0x00000001
#define ING_FRAME_VLD7_RD(src)                           (((src) & 0x00000001))
#define ING_FRAME_VLD7_WR(src)                      (((u32)(src)) & 0x00000001)
#define ING_FRAME_VLD7_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register UNIsec_CONTROL_Port8	*/ 
/*	 Fields ING_FRAME_VLD	 */
#define ING_FRAME_VLD8_WIDTH                                                  1
#define ING_FRAME_VLD8_SHIFT                                                  0
#define ING_FRAME_VLD8_MASK                                          0x00000001
#define ING_FRAME_VLD8_RD(src)                           (((src) & 0x00000001))
#define ING_FRAME_VLD8_WR(src)                      (((u32)(src)) & 0x00000001)
#define ING_FRAME_VLD8_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register UNIsec_INT_STS_EN	*/ 
/*	 Fields SA_SOFT_EXPIRE_INT_STS_EN	 */
#define SA_SOFT_EXPIRE_INT_STS_EN_WIDTH                                       1
#define SA_SOFT_EXPIRE_INT_STS_EN_SHIFT                                       1
#define SA_SOFT_EXPIRE_INT_STS_EN_MASK                               0x00000002
#define SA_SOFT_EXPIRE_INT_STS_EN_RD(src)             (((src) & 0x00000002)>>1)
#define SA_SOFT_EXPIRE_INT_STS_EN_WR(src)        (((u32)(src)<<1) & 0x00000002)
#define SA_SOFT_EXPIRE_INT_STS_EN_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields GENERAL_ERROR_INT_STS_EN	 */
#define GENERAL_ERROR_INT_STS_EN_WIDTH                                        1
#define GENERAL_ERROR_INT_STS_EN_SHIFT                                        0
#define GENERAL_ERROR_INT_STS_EN_MASK                                0x00000001
#define GENERAL_ERROR_INT_STS_EN_RD(src)                 (((src) & 0x00000001))
#define GENERAL_ERROR_INT_STS_EN_WR(src)            (((u32)(src)) & 0x00000001)
#define GENERAL_ERROR_INT_STS_EN_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register UNIsec_INT_SIG_EN	*/ 
/*	 Fields SA_SOFT_EXPIRE_INT_SIG_EN	 */
#define SA_SOFT_EXPIRE_INT_SIG_EN_WIDTH                                       1
#define SA_SOFT_EXPIRE_INT_SIG_EN_SHIFT                                       1
#define SA_SOFT_EXPIRE_INT_SIG_EN_MASK                               0x00000002
#define SA_SOFT_EXPIRE_INT_SIG_EN_RD(src)             (((src) & 0x00000002)>>1)
#define SA_SOFT_EXPIRE_INT_SIG_EN_WR(src)        (((u32)(src)<<1) & 0x00000002)
#define SA_SOFT_EXPIRE_INT_SIG_EN_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields GENERAL_ERROR_INT_SIG_EN	 */
#define GENERAL_ERROR_INT_SIG_EN_WIDTH                                        1
#define GENERAL_ERROR_INT_SIG_EN_SHIFT                                        0
#define GENERAL_ERROR_INT_SIG_EN_MASK                                0x00000001
#define GENERAL_ERROR_INT_SIG_EN_RD(src)                 (((src) & 0x00000001))
#define GENERAL_ERROR_INT_SIG_EN_WR(src)            (((u32)(src)) & 0x00000001)
#define GENERAL_ERROR_INT_SIG_EN_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register UNIsec_INT_STS	*/ 
/*	 Fields SA_SOFT_EXPIRE_INT	 */
#define SA_SOFT_EXPIRE_INT_WIDTH                                              1
#define SA_SOFT_EXPIRE_INT_SHIFT                                              1
#define SA_SOFT_EXPIRE_INT_MASK                                      0x00000002
#define SA_SOFT_EXPIRE_INT_RD(src)                    (((src) & 0x00000002)>>1)
#define SA_SOFT_EXPIRE_INT_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields GENERAL_ERROR_INT	 */
#define GENERAL_ERROR_INT_WIDTH                                               1
#define GENERAL_ERROR_INT_SHIFT                                               0
#define GENERAL_ERROR_INT_MASK                                       0x00000001
#define GENERAL_ERROR_INT_RD(src)                        (((src) & 0x00000001))
#define GENERAL_ERROR_INT_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register UNIsec_MACSEC_SA_THRD	*/ 
/*	 Fields SA_EXPIRE_THRD	 */
#define SA_EXPIRE_THRD_WIDTH                                                 32
#define SA_EXPIRE_THRD_SHIFT                                                  0
#define SA_EXPIRE_THRD_MASK                                          0xffffffff
#define SA_EXPIRE_THRD_RD(src)                           (((src) & 0xffffffff))
#define SA_EXPIRE_THRD_WR(src)                      (((u32)(src)) & 0xffffffff)
#define SA_EXPIRE_THRD_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register UNIsec_IPSEC_SA_THRD_LSB	*/ 
/*	 Fields SA_EXPIRE_THRD	 */
#define SA_EXPIRE_THRD_F1_WIDTH                                              32
#define SA_EXPIRE_THRD_F1_SHIFT                                               0
#define SA_EXPIRE_THRD_F1_MASK                                       0xffffffff
#define SA_EXPIRE_THRD_F1_RD(src)                        (((src) & 0xffffffff))
#define SA_EXPIRE_THRD_F1_WR(src)                   (((u32)(src)) & 0xffffffff)
#define SA_EXPIRE_THRD_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register UNIsec_IPSEC_SA_THRD_MSB	*/ 
/*	 Fields SA_EXPIRE_THRD	 */
#define SA_EXPIRE_THRD_F2_WIDTH                                              32
#define SA_EXPIRE_THRD_F2_SHIFT                                               0
#define SA_EXPIRE_THRD_F2_MASK                                       0xffffffff
#define SA_EXPIRE_THRD_F2_RD(src)                        (((src) & 0xffffffff))
#define SA_EXPIRE_THRD_F2_WR(src)                   (((u32)(src)) & 0xffffffff)
#define SA_EXPIRE_THRD_F2_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register UNIsec_MACsec_REPLAY_WINDOW_SIZE	*/ 
/*	 Fields UNIsec_MACSEC_REPLAY_WINDOW_SIZE	 */
#define UNISEC_MACSEC_REPLAY_WINDOW_SIZE_WIDTH                               32
#define UNISEC_MACSEC_REPLAY_WINDOW_SIZE_SHIFT                                0
#define UNISEC_MACSEC_REPLAY_WINDOW_SIZE_MASK                        0xffffffff
#define UNISEC_MACSEC_REPLAY_WINDOW_SIZE_RD(src)         (((src) & 0xffffffff))
#define UNISEC_MACSEC_REPLAY_WINDOW_SIZE_WR(src)    (((u32)(src)) & 0xffffffff)
#define UNISEC_MACSEC_REPLAY_WINDOW_SIZE_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register UNIsec_IPsec_REPLAY_WINDOW_SIZE	*/ 
/*	 Fields UNIsec_IPSEC_REPLAY_WINDOW_SIZE	 */
#define UNISEC_IPSEC_REPLAY_WINDOW_SIZE_WIDTH                                32
#define UNISEC_IPSEC_REPLAY_WINDOW_SIZE_SHIFT                                 0
#define UNISEC_IPSEC_REPLAY_WINDOW_SIZE_MASK                         0xffffffff
#define UNISEC_IPSEC_REPLAY_WINDOW_SIZE_RD(src)          (((src) & 0xffffffff))
#define UNISEC_IPSEC_REPLAY_WINDOW_SIZE_WR(src)     (((u32)(src)) & 0xffffffff)
#define UNISEC_IPSEC_REPLAY_WINDOW_SIZE_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register UNIsec_MACsec_EG_MTU_SIZE	*/ 
/*	 Fields UNIsec_MACsec_EG_MTU_SIZE	 */
#define UNISEC_MACSEC_EG_MTU_SIZE_WIDTH                                      16
#define UNISEC_MACSEC_EG_MTU_SIZE_SHIFT                                       0
#define UNISEC_MACSEC_EG_MTU_SIZE_MASK                               0x0000ffff
#define UNISEC_MACSEC_EG_MTU_SIZE_RD(src)                (((src) & 0x0000ffff))
#define UNISEC_MACSEC_EG_MTU_SIZE_WR(src)           (((u32)(src)) & 0x0000ffff)
#define UNISEC_MACSEC_EG_MTU_SIZE_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register UNIsec_MACsec_IG_MTU_SIZE	*/ 
/*	 Fields UNIsec_MACsec_IG_MTU_SIZE	 */
#define UNISEC_MACSEC_IG_MTU_SIZE_WIDTH                                      16
#define UNISEC_MACSEC_IG_MTU_SIZE_SHIFT                                       0
#define UNISEC_MACSEC_IG_MTU_SIZE_MASK                               0x0000ffff
#define UNISEC_MACSEC_IG_MTU_SIZE_RD(src)                (((src) & 0x0000ffff))
#define UNISEC_MACSEC_IG_MTU_SIZE_WR(src)           (((u32)(src)) & 0x0000ffff)
#define UNISEC_MACSEC_IG_MTU_SIZE_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register UNIsec_IPsec_EG_MTU_SIZE	*/ 
/*	 Fields UNIsec_IPsec_EG_MTU_SIZE	 */
#define UNISEC_IPSEC_EG_MTU_SIZE_WIDTH                                       32
#define UNISEC_IPSEC_EG_MTU_SIZE_SHIFT                                        0
#define UNISEC_IPSEC_EG_MTU_SIZE_MASK                                0xffffffff
#define UNISEC_IPSEC_EG_MTU_SIZE_RD(src)                 (((src) & 0xffffffff))
#define UNISEC_IPSEC_EG_MTU_SIZE_WR(src)            (((u32)(src)) & 0xffffffff)
#define UNISEC_IPSEC_EG_MTU_SIZE_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register UNIsec_IPsec_IG_MTU_SIZE	*/ 
/*	 Fields UNIsec_IPsec_IG_MTU_SIZE	 */
#define UNISEC_IPSEC_IG_MTU_SIZE_WIDTH                                       32
#define UNISEC_IPSEC_IG_MTU_SIZE_SHIFT                                        0
#define UNISEC_IPSEC_IG_MTU_SIZE_MASK                                0xffffffff
#define UNISEC_IPSEC_IG_MTU_SIZE_RD(src)                 (((src) & 0xffffffff))
#define UNISEC_IPSEC_IG_MTU_SIZE_WR(src)            (((u32)(src)) & 0xffffffff)
#define UNISEC_IPSEC_IG_MTU_SIZE_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register SA_MACsec_SOFT_EXPIRE_NUM_REG	*/ 
/*	 Fields SA_MACsec_Soft_Expire_Number	 */
#define SA_MACSEC_SOFT_EXPIRE_NUMBER_WIDTH                                    8
#define SA_MACSEC_SOFT_EXPIRE_NUMBER_SHIFT                                    0
#define SA_MACSEC_SOFT_EXPIRE_NUMBER_MASK                            0x000000ff
#define SA_MACSEC_SOFT_EXPIRE_NUMBER_RD(src)             (((src) & 0x000000ff))
#define SA_MACSEC_SOFT_EXPIRE_NUMBER_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register SA_IPsec_SOFT_EXPIRE_NUM_REG	*/ 
/*	 Fields SA_IPsec_Soft_Expire_Number	 */
#define SA_IPSEC_SOFT_EXPIRE_NUMBER_WIDTH                                     8
#define SA_IPSEC_SOFT_EXPIRE_NUMBER_SHIFT                                     0
#define SA_IPSEC_SOFT_EXPIRE_NUMBER_MASK                             0x000000ff
#define SA_IPSEC_SOFT_EXPIRE_NUMBER_RD(src)              (((src) & 0x000000ff))
#define SA_IPSEC_SOFT_EXPIRE_NUMBER_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register UNIsec_Control_Register	*/ 
/*	 Fields UNIsec_10G_en	 */
#define UNISEC_10G_EN_WIDTH                                                   1
#define UNISEC_10G_EN_SHIFT                                                   1
#define UNISEC_10G_EN_MASK                                           0x00000002
#define UNISEC_10G_EN_RD(src)                         (((src) & 0x00000002)>>1)
#define UNISEC_10G_EN_WR(src)                    (((u32)(src)<<1) & 0x00000002)
#define UNISEC_10G_EN_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields UNIsec_mib_clear_count	 */
#define UNISEC_MIB_CLEAR_COUNT_WIDTH                                          1
#define UNISEC_MIB_CLEAR_COUNT_SHIFT                                          0
#define UNISEC_MIB_CLEAR_COUNT_MASK                                  0x00000001
#define UNISEC_MIB_CLEAR_COUNT_RD(src)                   (((src) & 0x00000001))
#define UNISEC_MIB_CLEAR_COUNT_WR(src)              (((u32)(src)) & 0x00000001)
#define UNISEC_MIB_CLEAR_COUNT_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register UNIsec_MIB_Address_Register	*/ 
/*	 Fields mib_rd	 */
#define MIB_RD_WIDTH                                                          1
#define MIB_RD_SHIFT                                                         16
#define MIB_RD_MASK                                                  0x00010000
#define MIB_RD_RD(src)                               (((src) & 0x00010000)>>16)
#define MIB_RD_WR(src)                          (((u32)(src)<<16) & 0x00010000)
#define MIB_RD_SET(dst,src) \
                      (((dst) & ~0x00010000) | (((u32)(src)<<16) & 0x00010000))
/*	 Fields mib_addr_reg	 */
#define MIB_ADDR_REG_WIDTH                                                   16
#define MIB_ADDR_REG_SHIFT                                                    0
#define MIB_ADDR_REG_MASK                                            0x0000ffff
#define MIB_ADDR_REG_RD(src)                             (((src) & 0x0000ffff))
#define MIB_ADDR_REG_WR(src)                        (((u32)(src)) & 0x0000ffff)
#define MIB_ADDR_REG_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register UNIsec_MIB_Data_MSB_Register	*/ 
/*	 Fields mib_rw_data_reg_msb	 */
#define MIB_RW_DATA_REG_MSB_WIDTH                                            32
#define MIB_RW_DATA_REG_MSB_SHIFT                                             0
#define MIB_RW_DATA_REG_MSB_MASK                                     0xffffffff
#define MIB_RW_DATA_REG_MSB_RD(src)                      (((src) & 0xffffffff))
#define MIB_RW_DATA_REG_MSB_WR(src)                 (((u32)(src)) & 0xffffffff)
#define MIB_RW_DATA_REG_MSB_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register UNIsec_MIB_Data_LSB_Register	*/ 
/*	 Fields mib_rw_data_reg_lsb	 */
#define MIB_RW_DATA_REG_LSB_WIDTH                                            32
#define MIB_RW_DATA_REG_LSB_SHIFT                                             0
#define MIB_RW_DATA_REG_LSB_MASK                                     0xffffffff
#define MIB_RW_DATA_REG_LSB_RD(src)                      (((src) & 0xffffffff))
#define MIB_RW_DATA_REG_LSB_WR(src)                 (((u32)(src)) & 0xffffffff)
#define MIB_RW_DATA_REG_LSB_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Global Base Address	*/
#define ENET_GLBL_DIAG_CSR_BASE_ADDR				0xddd8a7000ULL

/*    Address GLBL_DIAG_CSR  Registers */
#define ENET_CFG_DIAG_SEL_ADDR                                       0x00000000
#define ENET_CFG_DIAG_SEL_DEFAULT                                    0x00000000
#define ENET_CFG_READ_BW_LAT_ADDR_MASK_ADDR                          0x00000004
#define ENET_CFG_READ_BW_LAT_ADDR_MASK_DEFAULT                       0x00000000
#define ENET_CFG_READ_BW_LAT_ADDR_PAT_ADDR                           0x00000008
#define ENET_CFG_READ_BW_LAT_ADDR_PAT_DEFAULT                        0xffffffff
#define ENET_CFG_WRITE_BW_LAT_ADDR_MASK_ADDR                         0x0000000c
#define ENET_CFG_WRITE_BW_LAT_ADDR_MASK_DEFAULT                      0x00000000
#define ENET_CFG_WRITE_BW_LAT_ADDR_PAT_ADDR                          0x00000010
#define ENET_CFG_WRITE_BW_LAT_ADDR_PAT_DEFAULT                       0xffffffff
#define ENET_CFG_DIAG_START_STOP_ADDR                                0x00000014
#define ENET_CFG_DIAG_START_STOP_DEFAULT                             0x0000001f
#define ENET_CFG_BW_MSTR_STOP_CNT_ADDR                               0x00000018
#define ENET_CFG_BW_MSTR_STOP_CNT_DEFAULT                            0x00040004
#define ENET_CFG_BW_SLV_STOP_CNT_ADDR                                0x0000001c
#define ENET_CFG_BW_SLV_STOP_CNT_DEFAULT                             0x00040004
#define ENET_STS_READ_LATENCY_OUTPUT_ADDR                            0x00000020
#define ENET_STS_READ_LATENCY_OUTPUT_DEFAULT                         0x00000000
#define ENET_STS_AXI_MRD_BW_CLK_CNT_ADDR                             0x00000024
#define ENET_STS_AXI_MRD_BW_CLK_CNT_DEFAULT                          0x00000000
#define ENET_STS_AXI_MRD_BW_BYTE_CNT_ADDR                            0x00000028
#define ENET_STS_AXI_MRD_BW_BYTE_CNT_DEFAULT                         0x00000000
#define ENET_STS_AXI_MWR_BW_CLK_CNT_ADDR                             0x0000002c
#define ENET_STS_AXI_MWR_BW_CLK_CNT_DEFAULT                          0x00000000
#define ENET_STS_AXI_MWR_BW_BYTE_CNT_ADDR                            0x00000030
#define ENET_STS_AXI_MWR_BW_BYTE_CNT_DEFAULT                         0x00000000
#define ENET_STS_AXI_SRD_BW_CLK_CNT_ADDR                             0x00000034
#define ENET_STS_AXI_SRD_BW_CLK_CNT_DEFAULT                          0x00000000
#define ENET_STS_AXI_SRD_BW_BYTE_CNT_ADDR                            0x00000038
#define ENET_STS_AXI_SRD_BW_BYTE_CNT_DEFAULT                         0x00000000
#define ENET_STS_AXI_SWR_BW_CLK_CNT_ADDR                             0x0000003c
#define ENET_STS_AXI_SWR_BW_CLK_CNT_DEFAULT                          0x00000000
#define ENET_STS_AXI_SWR_BW_BYTE_CNT_ADDR                            0x00000040
#define ENET_STS_AXI_SWR_BW_BYTE_CNT_DEFAULT                         0x00000000
#define ENET_CFG_DBG_TRIG_CTRL_ADDR                                  0x00000044
#define ENET_CFG_DBG_TRIG_CTRL_DEFAULT                               0x00000000
#define ENET_CFG_DBG_PAT_REG_0_ADDR                                  0x00000048
#define ENET_CFG_DBG_PAT_REG_0_DEFAULT                               0x00000000
#define ENET_CFG_DBG_PAT_MASK_REG_0_ADDR                             0x0000004c
#define ENET_CFG_DBG_PAT_MASK_REG_0_DEFAULT                          0x00000000
#define ENET_CFG_DBG_PAT_REG_1_ADDR                                  0x00000050
#define ENET_CFG_DBG_PAT_REG_1_DEFAULT                               0x00000000
#define ENET_CFG_DBG_PAT_MASK_REG_1_ADDR                             0x00000054
#define ENET_CFG_DBG_PAT_MASK_REG_1_DEFAULT                          0x00000000
#define ENET_DBG_TRIG_OUT_ADDR                                       0x00000058
#define ENET_DBG_TRIG_OUT_DEFAULT                                    0x00000000
#define ENET_DBG_ECC_INT_OUT_ADDR                                    0x0000005c
#define ENET_DBG_ECC_INT_OUT_DEFAULT                                 0x00000000
#define ENET_DBG_ECC_INT_OUTMASK_ADDR                                0x00000060
#define ENET_CFG_SECC_INT_THRSH_REG_ADDR                             0x00000064
#define ENET_CFG_SECC_INT_THRSH_REG_DEFAULT                          0x000000fe
#define ENET_CFG_MEM_ECC_BYPASS_ADDR                                 0x00000068
#define ENET_CFG_MEM_ECC_BYPASS_DEFAULT                              0x00000000
#define ENET_CFG_MEM_PWRDN_DIS_ADDR                                  0x0000006c
#define ENET_CFG_MEM_PWRDN_DIS_DEFAULT                               0x00000000
#define ENET_CFG_MEM_RAM_SHUTDOWN_ADDR                               0x00000070
#define ENET_CFG_MEM_RAM_SHUTDOWN_DEFAULT                            0xffffffff
#define ENET_BLOCK_MEM_RDY_ADDR                                      0x00000074
#define ENET_BLOCK_MEM_RDY_DEFAULT                                   0xffffffff
#define ENET_CFG_AXI_SLV_RD_TMO_ADDR                                 0x00000078
#define ENET_CFG_AXI_SLV_RD_TMO_DEFAULT                              0x83ff83ff
#define ENET_CFG_AXI_SLV_WR_TMO_ADDR                                 0x0000007c
#define ENET_CFG_AXI_SLV_WR_TMO_DEFAULT                              0x83ff83ff
#define ENET_STS_AXI_SLV_TMO_ADDR                                    0x00000080
#define ENET_STS_AXI_SLV_TMO_DEFAULT                                 0x00000000
#define ENET_CFG_AXI_SS_CSR_TMO_ADDR                                 0x00000084
#define ENET_CFG_AXI_SS_CSR_TMO_DEFAULT                              0x02008000
#define ENET_STS_READ_LATENCY_TOT_READ_REQS_ADDR                     0x0000008c
#define ENET_STS_READ_LATENCY_TOT_READ_REQS_DEFAULT                  0x00000000

/*	Register CFG_DIAG_SEL	*/ 
/*	 Fields CFG_AXI_NON_AXI_MUX_SELECT	 */
#define ENET_CFG_AXI_NON_AXI_MUX_SELECT_WIDTH                                 1
#define ENET_CFG_AXI_NON_AXI_MUX_SELECT_SHIFT                                11
#define ENET_CFG_AXI_NON_AXI_MUX_SELECT_MASK                         0x00000800
#define ENET_CFG_AXI_NON_AXI_MUX_SELECT_RD(src)      (((src) & 0x00000800)>>11)
#define ENET_CFG_AXI_NON_AXI_MUX_SELECT_WR(src) \
                                                (((u32)(src)<<11) & 0x00000800)
#define ENET_CFG_AXI_NON_AXI_MUX_SELECT_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*	 Fields CFG_MUX_SELECTOR	 */
#define ENET_CFG_MUX_SELECTOR_WIDTH                                          11
#define ENET_CFG_MUX_SELECTOR_SHIFT                                           0
#define ENET_CFG_MUX_SELECTOR_MASK                                   0x000007ff
#define ENET_CFG_MUX_SELECTOR_RD(src)                    (((src) & 0x000007ff))
#define ENET_CFG_MUX_SELECTOR_WR(src)               (((u32)(src)) & 0x000007ff)
#define ENET_CFG_MUX_SELECTOR_SET(dst,src) \
                          (((dst) & ~0x000007ff) | (((u32)(src)) & 0x000007ff))

/*	Register CFG_READ_BW_LAT_ADDR_MASK	*/ 
/*	 Fields READ_ADDR_MASK	 */
#define ENET_READ_ADDR_MASK_WIDTH                                            32
#define ENET_READ_ADDR_MASK_SHIFT                                             0
#define ENET_READ_ADDR_MASK_MASK                                     0xffffffff
#define ENET_READ_ADDR_MASK_RD(src)                      (((src) & 0xffffffff))
#define ENET_READ_ADDR_MASK_WR(src)                 (((u32)(src)) & 0xffffffff)
#define ENET_READ_ADDR_MASK_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_READ_BW_LAT_ADDR_PAT	*/ 
/*	 Fields READ_ADDR_PAT	 */
#define ENET_READ_ADDR_PAT_WIDTH                                             32
#define ENET_READ_ADDR_PAT_SHIFT                                              0
#define ENET_READ_ADDR_PAT_MASK                                      0xffffffff
#define ENET_READ_ADDR_PAT_RD(src)                       (((src) & 0xffffffff))
#define ENET_READ_ADDR_PAT_WR(src)                  (((u32)(src)) & 0xffffffff)
#define ENET_READ_ADDR_PAT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_WRITE_BW_LAT_ADDR_MASK	*/ 
/*	 Fields WRITE_ADDR_MASK	 */
#define ENET_WRITE_ADDR_MASK_WIDTH                                           32
#define ENET_WRITE_ADDR_MASK_SHIFT                                            0
#define ENET_WRITE_ADDR_MASK_MASK                                    0xffffffff
#define ENET_WRITE_ADDR_MASK_RD(src)                     (((src) & 0xffffffff))
#define ENET_WRITE_ADDR_MASK_WR(src)                (((u32)(src)) & 0xffffffff)
#define ENET_WRITE_ADDR_MASK_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_WRITE_BW_LAT_ADDR_PAT	*/ 
/*	 Fields WRITE_ADDR_PAT	 */
#define ENET_WRITE_ADDR_PAT_WIDTH                                            32
#define ENET_WRITE_ADDR_PAT_SHIFT                                             0
#define ENET_WRITE_ADDR_PAT_MASK                                     0xffffffff
#define ENET_WRITE_ADDR_PAT_RD(src)                      (((src) & 0xffffffff))
#define ENET_WRITE_ADDR_PAT_WR(src)                 (((u32)(src)) & 0xffffffff)
#define ENET_WRITE_ADDR_PAT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_DIAG_START_STOP	*/ 
/*	 Fields START_RD_LATENCY	 */
#define ENET_START_RD_LATENCY_WIDTH                                           1
#define ENET_START_RD_LATENCY_SHIFT                                           4
#define ENET_START_RD_LATENCY_MASK                                   0x00000010
#define ENET_START_RD_LATENCY_RD(src)                 (((src) & 0x00000010)>>4)
#define ENET_START_RD_LATENCY_WR(src)            (((u32)(src)<<4) & 0x00000010)
#define ENET_START_RD_LATENCY_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields START_SRD_BW	 */
#define ENET_START_SRD_BW_WIDTH                                               1
#define ENET_START_SRD_BW_SHIFT                                               3
#define ENET_START_SRD_BW_MASK                                       0x00000008
#define ENET_START_SRD_BW_RD(src)                     (((src) & 0x00000008)>>3)
#define ENET_START_SRD_BW_WR(src)                (((u32)(src)<<3) & 0x00000008)
#define ENET_START_SRD_BW_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields START_MRD_BW	 */
#define ENET_START_MRD_BW_WIDTH                                               1
#define ENET_START_MRD_BW_SHIFT                                               2
#define ENET_START_MRD_BW_MASK                                       0x00000004
#define ENET_START_MRD_BW_RD(src)                     (((src) & 0x00000004)>>2)
#define ENET_START_MRD_BW_WR(src)                (((u32)(src)<<2) & 0x00000004)
#define ENET_START_MRD_BW_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields START_SWR_BW	 */
#define ENET_START_SWR_BW_WIDTH                                               1
#define ENET_START_SWR_BW_SHIFT                                               1
#define ENET_START_SWR_BW_MASK                                       0x00000002
#define ENET_START_SWR_BW_RD(src)                     (((src) & 0x00000002)>>1)
#define ENET_START_SWR_BW_WR(src)                (((u32)(src)<<1) & 0x00000002)
#define ENET_START_SWR_BW_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields START_MWR_BW	 */
#define ENET_START_MWR_BW_WIDTH                                               1
#define ENET_START_MWR_BW_SHIFT                                               0
#define ENET_START_MWR_BW_MASK                                       0x00000001
#define ENET_START_MWR_BW_RD(src)                        (((src) & 0x00000001))
#define ENET_START_MWR_BW_WR(src)                   (((u32)(src)) & 0x00000001)
#define ENET_START_MWR_BW_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register CFG_BW_MSTR_STOP_CNT	*/ 
/*	 Fields MSTR_STOP_RD_CNT	 */
#define ENET_MSTR_STOP_RD_CNT_WIDTH                                          16
#define ENET_MSTR_STOP_RD_CNT_SHIFT                                          16
#define ENET_MSTR_STOP_RD_CNT_MASK                                   0xffff0000
#define ENET_MSTR_STOP_RD_CNT_RD(src)                (((src) & 0xffff0000)>>16)
#define ENET_MSTR_STOP_RD_CNT_WR(src)           (((u32)(src)<<16) & 0xffff0000)
#define ENET_MSTR_STOP_RD_CNT_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields MSTR_STOP_WR_CNT	 */
#define ENET_MSTR_STOP_WR_CNT_WIDTH                                          16
#define ENET_MSTR_STOP_WR_CNT_SHIFT                                           0
#define ENET_MSTR_STOP_WR_CNT_MASK                                   0x0000ffff
#define ENET_MSTR_STOP_WR_CNT_RD(src)                    (((src) & 0x0000ffff))
#define ENET_MSTR_STOP_WR_CNT_WR(src)               (((u32)(src)) & 0x0000ffff)
#define ENET_MSTR_STOP_WR_CNT_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register CFG_BW_SLV_STOP_CNT	*/ 
/*	 Fields SLV_STOP_RD_CNT	 */
#define ENET_SLV_STOP_RD_CNT_WIDTH                                           16
#define ENET_SLV_STOP_RD_CNT_SHIFT                                           16
#define ENET_SLV_STOP_RD_CNT_MASK                                    0xffff0000
#define ENET_SLV_STOP_RD_CNT_RD(src)                 (((src) & 0xffff0000)>>16)
#define ENET_SLV_STOP_RD_CNT_WR(src)            (((u32)(src)<<16) & 0xffff0000)
#define ENET_SLV_STOP_RD_CNT_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields SLV_STOP_WR_CNT	 */
#define ENET_SLV_STOP_WR_CNT_WIDTH                                           16
#define ENET_SLV_STOP_WR_CNT_SHIFT                                            0
#define ENET_SLV_STOP_WR_CNT_MASK                                    0x0000ffff
#define ENET_SLV_STOP_WR_CNT_RD(src)                     (((src) & 0x0000ffff))
#define ENET_SLV_STOP_WR_CNT_WR(src)                (((u32)(src)) & 0x0000ffff)
#define ENET_SLV_STOP_WR_CNT_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register STS_READ_LATENCY_OUTPUT	*/ 
/*	 Fields READ_MAX	 */
#define ENET_READ_MAX_WIDTH                                                  10
#define ENET_READ_MAX_SHIFT                                                  22
#define ENET_READ_MAX_MASK                                           0xffc00000
#define ENET_READ_MAX_RD(src)                        (((src) & 0xffc00000)>>22)
#define ENET_READ_MAX_SET(dst,src) \
                      (((dst) & ~0xffc00000) | (((u32)(src)<<22) & 0xffc00000))
/*	 Fields READ_TOT	 */
#define ENET_READ_TOT_WIDTH                                                  22
#define ENET_READ_TOT_SHIFT                                                   0
#define ENET_READ_TOT_MASK                                           0x003fffff
#define ENET_READ_TOT_RD(src)                            (((src) & 0x003fffff))
#define ENET_READ_TOT_SET(dst,src) \
                          (((dst) & ~0x003fffff) | (((u32)(src)) & 0x003fffff))

/*	Register STS_AXI_MRD_BW_CLK_CNT	*/ 
/*	 Fields MSTR_READ_BW_CLK_CNT	 */
#define ENET_MSTR_READ_BW_CLK_CNT_WIDTH                                      32
#define ENET_MSTR_READ_BW_CLK_CNT_SHIFT                                       0
#define ENET_MSTR_READ_BW_CLK_CNT_MASK                               0xffffffff
#define ENET_MSTR_READ_BW_CLK_CNT_RD(src)                (((src) & 0xffffffff))
#define ENET_MSTR_READ_BW_CLK_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register STS_AXI_MRD_BW_BYTE_CNT	*/ 
/*	 Fields MSTR_READ_BW_BYTE_CNT	 */
#define ENET_MSTR_READ_BW_BYTE_CNT_WIDTH                                     32
#define ENET_MSTR_READ_BW_BYTE_CNT_SHIFT                                      0
#define ENET_MSTR_READ_BW_BYTE_CNT_MASK                              0xffffffff
#define ENET_MSTR_READ_BW_BYTE_CNT_RD(src)               (((src) & 0xffffffff))
#define ENET_MSTR_READ_BW_BYTE_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register STS_AXI_MWR_BW_CLK_CNT	*/ 
/*	 Fields MSTR_WRITE_BW_CLK_CNT	 */
#define ENET_MSTR_WRITE_BW_CLK_CNT_WIDTH                                     32
#define ENET_MSTR_WRITE_BW_CLK_CNT_SHIFT                                      0
#define ENET_MSTR_WRITE_BW_CLK_CNT_MASK                              0xffffffff
#define ENET_MSTR_WRITE_BW_CLK_CNT_RD(src)               (((src) & 0xffffffff))
#define ENET_MSTR_WRITE_BW_CLK_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register STS_AXI_MWR_BW_BYTE_CNT	*/ 
/*	 Fields MSTR_WRITE_BW_BYTE_CNT	 */
#define ENET_MSTR_WRITE_BW_BYTE_CNT_WIDTH                                    32
#define ENET_MSTR_WRITE_BW_BYTE_CNT_SHIFT                                     0
#define ENET_MSTR_WRITE_BW_BYTE_CNT_MASK                             0xffffffff
#define ENET_MSTR_WRITE_BW_BYTE_CNT_RD(src)              (((src) & 0xffffffff))
#define ENET_MSTR_WRITE_BW_BYTE_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register STS_AXI_SRD_BW_CLK_CNT	*/ 
/*	 Fields SLV_READ_BW_CLK_CNT	 */
#define ENET_SLV_READ_BW_CLK_CNT_WIDTH                                       32
#define ENET_SLV_READ_BW_CLK_CNT_SHIFT                                        0
#define ENET_SLV_READ_BW_CLK_CNT_MASK                                0xffffffff
#define ENET_SLV_READ_BW_CLK_CNT_RD(src)                 (((src) & 0xffffffff))
#define ENET_SLV_READ_BW_CLK_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register STS_AXI_SRD_BW_BYTE_CNT	*/ 
/*	 Fields SLV_READ_BW_BYTE_CNT	 */
#define ENET_SLV_READ_BW_BYTE_CNT_WIDTH                                      32
#define ENET_SLV_READ_BW_BYTE_CNT_SHIFT                                       0
#define ENET_SLV_READ_BW_BYTE_CNT_MASK                               0xffffffff
#define ENET_SLV_READ_BW_BYTE_CNT_RD(src)                (((src) & 0xffffffff))
#define ENET_SLV_READ_BW_BYTE_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register STS_AXI_SWR_BW_CLK_CNT	*/ 
/*	 Fields SLV_WRITE_BW_CLK_CNT	 */
#define ENET_SLV_WRITE_BW_CLK_CNT_WIDTH                                      32
#define ENET_SLV_WRITE_BW_CLK_CNT_SHIFT                                       0
#define ENET_SLV_WRITE_BW_CLK_CNT_MASK                               0xffffffff
#define ENET_SLV_WRITE_BW_CLK_CNT_RD(src)                (((src) & 0xffffffff))
#define ENET_SLV_WRITE_BW_CLK_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register STS_AXI_SWR_BW_BYTE_CNT	*/ 
/*	 Fields SLV_WRITE_BW_BYTE_CNT	 */
#define ENET_SLV_WRITE_BW_BYTE_CNT_WIDTH                                     32
#define ENET_SLV_WRITE_BW_BYTE_CNT_SHIFT                                      0
#define ENET_SLV_WRITE_BW_BYTE_CNT_MASK                              0xffffffff
#define ENET_SLV_WRITE_BW_BYTE_CNT_RD(src)               (((src) & 0xffffffff))
#define ENET_SLV_WRITE_BW_BYTE_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_DBG_TRIG_CTRL	*/ 
/*	 Fields TRIG_EN	 */
#define ENET_TRIG_EN_WIDTH                                                    1
#define ENET_TRIG_EN_SHIFT                                                    4
#define ENET_TRIG_EN_MASK                                            0x00000010
#define ENET_TRIG_EN_RD(src)                          (((src) & 0x00000010)>>4)
#define ENET_TRIG_EN_WR(src)                     (((u32)(src)<<4) & 0x00000010)
#define ENET_TRIG_EN_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields AND_E	 */
#define ENET_AND_E_WIDTH                                                      2
#define ENET_AND_E_SHIFT                                                      2
#define ENET_AND_E_MASK                                              0x0000000c
#define ENET_AND_E_RD(src)                            (((src) & 0x0000000c)>>2)
#define ENET_AND_E_WR(src)                       (((u32)(src)<<2) & 0x0000000c)
#define ENET_AND_E_SET(dst,src) \
                       (((dst) & ~0x0000000c) | (((u32)(src)<<2) & 0x0000000c))
/*	 Fields OR_E	 */
#define ENET_OR_E_WIDTH                                                       2
#define ENET_OR_E_SHIFT                                                       0
#define ENET_OR_E_MASK                                               0x00000003
#define ENET_OR_E_RD(src)                                (((src) & 0x00000003))
#define ENET_OR_E_WR(src)                           (((u32)(src)) & 0x00000003)
#define ENET_OR_E_SET(dst,src) \
                          (((dst) & ~0x00000003) | (((u32)(src)) & 0x00000003))

/*	Register CFG_DBG_PAT_REG_0	*/ 
/*	 Fields PATTERN	 */
#define ENET_PATTERN0_WIDTH                                                  32
#define ENET_PATTERN0_SHIFT                                                   0
#define ENET_PATTERN0_MASK                                           0xffffffff
#define ENET_PATTERN0_RD(src)                            (((src) & 0xffffffff))
#define ENET_PATTERN0_WR(src)                       (((u32)(src)) & 0xffffffff)
#define ENET_PATTERN0_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_DBG_PAT_MASK_REG_0	*/ 
/*	 Fields PAT_MASK	 */
#define ENET_PAT_MASK0_WIDTH                                                 32
#define ENET_PAT_MASK0_SHIFT                                                  0
#define ENET_PAT_MASK0_MASK                                          0xffffffff
#define ENET_PAT_MASK0_RD(src)                           (((src) & 0xffffffff))
#define ENET_PAT_MASK0_WR(src)                      (((u32)(src)) & 0xffffffff)
#define ENET_PAT_MASK0_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_DBG_PAT_REG_1	*/ 
/*	 Fields PATTERN	 */
#define ENET_PATTERN1_WIDTH                                                  32
#define ENET_PATTERN1_SHIFT                                                   0
#define ENET_PATTERN1_MASK                                           0xffffffff
#define ENET_PATTERN1_RD(src)                            (((src) & 0xffffffff))
#define ENET_PATTERN1_WR(src)                       (((u32)(src)) & 0xffffffff)
#define ENET_PATTERN1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_DBG_PAT_MASK_REG_1	*/ 
/*	 Fields PAT_MASK	 */
#define ENET_PAT_MASK1_WIDTH                                                 32
#define ENET_PAT_MASK1_SHIFT                                                  0
#define ENET_PAT_MASK1_MASK                                          0xffffffff
#define ENET_PAT_MASK1_RD(src)                           (((src) & 0xffffffff))
#define ENET_PAT_MASK1_WR(src)                      (((u32)(src)) & 0xffffffff)
#define ENET_PAT_MASK1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register DBG_TRIG_OUT	*/ 
/*	 Fields DBG_OUT	 */
#define ENET_DBG_OUT_WIDTH                                                   32
#define ENET_DBG_OUT_SHIFT                                                    0
#define ENET_DBG_OUT_MASK                                            0xffffffff
#define ENET_DBG_OUT_RD(src)                             (((src) & 0xffffffff))
#define ENET_DBG_OUT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register DBG_ECC_INT_OUT	*/ 
/*	 Fields SLV_TMO	 */
#define ENET_SLV_TMO_WIDTH                                                    1
#define ENET_SLV_TMO_SHIFT                                                    3
#define ENET_SLV_TMO_MASK                                            0x00000008
#define ENET_SLV_TMO_RD(src)                          (((src) & 0x00000008)>>3)
#define ENET_SLV_TMO_WR(src)                     (((u32)(src)<<3) & 0x00000008)
#define ENET_SLV_TMO_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields SECC_INT	 */
#define ENET_SECC_INT_WIDTH                                                   1
#define ENET_SECC_INT_SHIFT                                                   2
#define ENET_SECC_INT_MASK                                           0x00000004
#define ENET_SECC_INT_RD(src)                         (((src) & 0x00000004)>>2)
#define ENET_SECC_INT_WR(src)                    (((u32)(src)<<2) & 0x00000004)
#define ENET_SECC_INT_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields DECC_INT	 */
#define ENET_DECC_INT_WIDTH                                                   1
#define ENET_DECC_INT_SHIFT                                                   1
#define ENET_DECC_INT_MASK                                           0x00000002
#define ENET_DECC_INT_RD(src)                         (((src) & 0x00000002)>>1)
#define ENET_DECC_INT_WR(src)                    (((u32)(src)<<1) & 0x00000002)
#define ENET_DECC_INT_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields DBG_INT	 */
#define ENET_DBG_INT_WIDTH                                                    1
#define ENET_DBG_INT_SHIFT                                                    0
#define ENET_DBG_INT_MASK                                            0x00000001
#define ENET_DBG_INT_RD(src)                             (((src) & 0x00000001))
#define ENET_DBG_INT_WR(src)                        (((u32)(src)) & 0x00000001)
#define ENET_DBG_INT_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register DBG_ECC_INT_OUTMask	*/
/*    Mask Register Fields SLV_TMOMask    */
#define ENET_SLV_TMOMASK_WIDTH                                                1
#define ENET_SLV_TMOMASK_SHIFT                                                3
#define ENET_SLV_TMOMASK_MASK                                        0x00000008
#define ENET_SLV_TMOMASK_RD(src)                      (((src) & 0x00000008)>>3)
#define ENET_SLV_TMOMASK_WR(src)                 (((u32)(src)<<3) & 0x00000008)
#define ENET_SLV_TMOMASK_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*    Mask Register Fields SECC_INTMask    */
#define ENET_SECC_INTMASK_WIDTH                                               1
#define ENET_SECC_INTMASK_SHIFT                                               2
#define ENET_SECC_INTMASK_MASK                                       0x00000004
#define ENET_SECC_INTMASK_RD(src)                     (((src) & 0x00000004)>>2)
#define ENET_SECC_INTMASK_WR(src)                (((u32)(src)<<2) & 0x00000004)
#define ENET_SECC_INTMASK_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*    Mask Register Fields DECC_INTMask    */
#define ENET_DECC_INTMASK_WIDTH                                               1
#define ENET_DECC_INTMASK_SHIFT                                               1
#define ENET_DECC_INTMASK_MASK                                       0x00000002
#define ENET_DECC_INTMASK_RD(src)                     (((src) & 0x00000002)>>1)
#define ENET_DECC_INTMASK_WR(src)                (((u32)(src)<<1) & 0x00000002)
#define ENET_DECC_INTMASK_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*    Mask Register Fields DBG_INTMask    */
#define ENET_DBG_INTMASK_WIDTH                                                1
#define ENET_DBG_INTMASK_SHIFT                                                0
#define ENET_DBG_INTMASK_MASK                                        0x00000001
#define ENET_DBG_INTMASK_RD(src)                         (((src) & 0x00000001))
#define ENET_DBG_INTMASK_WR(src)                    (((u32)(src)) & 0x00000001)
#define ENET_DBG_INTMASK_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register CFG_SECC_INT_THRSH_REG	*/ 
/*	 Fields SECC_INT_THRSH	 */
#define ENET_SECC_INT_THRSH_WIDTH                                             8
#define ENET_SECC_INT_THRSH_SHIFT                                             0
#define ENET_SECC_INT_THRSH_MASK                                     0x000000ff
#define ENET_SECC_INT_THRSH_RD(src)                      (((src) & 0x000000ff))
#define ENET_SECC_INT_THRSH_WR(src)                 (((u32)(src)) & 0x000000ff)
#define ENET_SECC_INT_THRSH_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register CFG_MEM_ECC_BYPASS	*/ 
/*	 Fields CFG_ECC_BYPASS	 */
#define ENET_CFG_ECC_BYPASS_WIDTH                                            16
#define ENET_CFG_ECC_BYPASS_SHIFT                                             0
#define ENET_CFG_ECC_BYPASS_MASK                                     0x0000ffff
#define ENET_CFG_ECC_BYPASS_RD(src)                      (((src) & 0x0000ffff))
#define ENET_CFG_ECC_BYPASS_WR(src)                 (((u32)(src)) & 0x0000ffff)
#define ENET_CFG_ECC_BYPASS_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register CFG_MEM_PWRDN_DIS	*/ 
/*	 Fields CFG_ECC_PWRDN_DIS	 */
#define ENET_CFG_ECC_PWRDN_DIS_WIDTH                                         16
#define ENET_CFG_ECC_PWRDN_DIS_SHIFT                                         16
#define ENET_CFG_ECC_PWRDN_DIS_MASK                                  0xffff0000
#define ENET_CFG_ECC_PWRDN_DIS_RD(src)               (((src) & 0xffff0000)>>16)
#define ENET_CFG_ECC_PWRDN_DIS_WR(src)          (((u32)(src)<<16) & 0xffff0000)
#define ENET_CFG_ECC_PWRDN_DIS_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields CFG_PWRDN_DIS	 */
#define ENET_CFG_PWRDN_DIS_WIDTH                                             16
#define ENET_CFG_PWRDN_DIS_SHIFT                                              0
#define ENET_CFG_PWRDN_DIS_MASK                                      0x0000ffff
#define ENET_CFG_PWRDN_DIS_RD(src)                       (((src) & 0x0000ffff))
#define ENET_CFG_PWRDN_DIS_WR(src)                  (((u32)(src)) & 0x0000ffff)
#define ENET_CFG_PWRDN_DIS_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register CFG_MEM_RAM_SHUTDOWN	*/ 
/*	 Fields CFG_RAM_SHUTDOWN_EN	 */
#define ENET_CFG_RAM_SHUTDOWN_EN_WIDTH                                       32
#define ENET_CFG_RAM_SHUTDOWN_EN_SHIFT                                        0
#define ENET_CFG_RAM_SHUTDOWN_EN_MASK                                0xffffffff
#define ENET_CFG_RAM_SHUTDOWN_EN_RD(src)                 (((src) & 0xffffffff))
#define ENET_CFG_RAM_SHUTDOWN_EN_WR(src)            (((u32)(src)) & 0xffffffff)
#define ENET_CFG_RAM_SHUTDOWN_EN_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register BLOCK_MEM_RDY	*/ 
/*	 Fields MEM_RDY	 */
#define ENET_MEM_RDY_WIDTH                                                   32
#define ENET_MEM_RDY_SHIFT                                                    0
#define ENET_MEM_RDY_MASK                                            0xffffffff
#define ENET_MEM_RDY_RD(src)                             (((src) & 0xffffffff))
#define ENET_MEM_RDY_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_AXI_SLV_RD_TMO	*/ 
/*	 Fields SLV_ID_TMO_ENABLE	 */
#define ENET_SLV_ID_TMO_ENABLE_WIDTH                                          1
#define ENET_SLV_ID_TMO_ENABLE_SHIFT                                         31
#define ENET_SLV_ID_TMO_ENABLE_MASK                                  0x80000000
#define ENET_SLV_ID_TMO_ENABLE_RD(src)               (((src) & 0x80000000)>>31)
#define ENET_SLV_ID_TMO_ENABLE_WR(src)          (((u32)(src)<<31) & 0x80000000)
#define ENET_SLV_ID_TMO_ENABLE_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields SS_ID_TRK_ENABLE	 */
#define ENET_SS_ID_TRK_ENABLE_WIDTH                                           1
#define ENET_SS_ID_TRK_ENABLE_SHIFT                                          30
#define ENET_SS_ID_TRK_ENABLE_MASK                                   0x40000000
#define ENET_SS_ID_TRK_ENABLE_RD(src)                (((src) & 0x40000000)>>30)
#define ENET_SS_ID_TRK_ENABLE_WR(src)           (((u32)(src)<<30) & 0x40000000)
#define ENET_SS_ID_TRK_ENABLE_SET(dst,src) \
                      (((dst) & ~0x40000000) | (((u32)(src)<<30) & 0x40000000))
/*	 Fields SLV_ID_TMO_VALUE	 */
#define ENET_SLV_ID_TMO_VALUE_WIDTH                                          10
#define ENET_SLV_ID_TMO_VALUE_SHIFT                                          16
#define ENET_SLV_ID_TMO_VALUE_MASK                                   0x03ff0000
#define ENET_SLV_ID_TMO_VALUE_RD(src)                (((src) & 0x03ff0000)>>16)
#define ENET_SLV_ID_TMO_VALUE_WR(src)           (((u32)(src)<<16) & 0x03ff0000)
#define ENET_SLV_ID_TMO_VALUE_SET(dst,src) \
                      (((dst) & ~0x03ff0000) | (((u32)(src)<<16) & 0x03ff0000))
/*	 Fields SLV_RDY_TMO_ENABLE	 */
#define ENET_SLV_RDY_TMO_ENABLE_WIDTH                                         1
#define ENET_SLV_RDY_TMO_ENABLE_SHIFT                                        15
#define ENET_SLV_RDY_TMO_ENABLE_MASK                                 0x00008000
#define ENET_SLV_RDY_TMO_ENABLE_RD(src)              (((src) & 0x00008000)>>15)
#define ENET_SLV_RDY_TMO_ENABLE_WR(src)         (((u32)(src)<<15) & 0x00008000)
#define ENET_SLV_RDY_TMO_ENABLE_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))
/*	 Fields SLV_RDY_TMO_VALUE	 */
#define ENET_SLV_RDY_TMO_VALUE_WIDTH                                         10
#define ENET_SLV_RDY_TMO_VALUE_SHIFT                                          0
#define ENET_SLV_RDY_TMO_VALUE_MASK                                  0x000003ff
#define ENET_SLV_RDY_TMO_VALUE_RD(src)                   (((src) & 0x000003ff))
#define ENET_SLV_RDY_TMO_VALUE_WR(src)              (((u32)(src)) & 0x000003ff)
#define ENET_SLV_RDY_TMO_VALUE_SET(dst,src) \
                          (((dst) & ~0x000003ff) | (((u32)(src)) & 0x000003ff))

/*	Register CFG_AXI_SLV_WR_TMO	*/ 
/*	 Fields SLV_ID_TMO_ENABLE	 */
#define ENET_SLV_ID_TMO_ENABLE_F1_WIDTH                                       1
#define ENET_SLV_ID_TMO_ENABLE_F1_SHIFT                                      31
#define ENET_SLV_ID_TMO_ENABLE_F1_MASK                               0x80000000
#define ENET_SLV_ID_TMO_ENABLE_F1_RD(src)            (((src) & 0x80000000)>>31)
#define ENET_SLV_ID_TMO_ENABLE_F1_WR(src)       (((u32)(src)<<31) & 0x80000000)
#define ENET_SLV_ID_TMO_ENABLE_F1_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields SS_ID_TRK_ENABLE	 */
#define ENET_SS_ID_TRK_ENABLE_F1_WIDTH                                        1
#define ENET_SS_ID_TRK_ENABLE_F1_SHIFT                                       30
#define ENET_SS_ID_TRK_ENABLE_F1_MASK                                0x40000000
#define ENET_SS_ID_TRK_ENABLE_F1_RD(src)             (((src) & 0x40000000)>>30)
#define ENET_SS_ID_TRK_ENABLE_F1_WR(src)        (((u32)(src)<<30) & 0x40000000)
#define ENET_SS_ID_TRK_ENABLE_F1_SET(dst,src) \
                      (((dst) & ~0x40000000) | (((u32)(src)<<30) & 0x40000000))
/*	 Fields SLV_ID_TMO_VALUE	 */
#define ENET_SLV_ID_TMO_VALUE_F1_WIDTH                                       10
#define ENET_SLV_ID_TMO_VALUE_F1_SHIFT                                       16
#define ENET_SLV_ID_TMO_VALUE_F1_MASK                                0x03ff0000
#define ENET_SLV_ID_TMO_VALUE_F1_RD(src)             (((src) & 0x03ff0000)>>16)
#define ENET_SLV_ID_TMO_VALUE_F1_WR(src)        (((u32)(src)<<16) & 0x03ff0000)
#define ENET_SLV_ID_TMO_VALUE_F1_SET(dst,src) \
                      (((dst) & ~0x03ff0000) | (((u32)(src)<<16) & 0x03ff0000))
/*	 Fields SLV_RDY_TMO_ENABLE	 */
#define ENET_SLV_RDY_TMO_ENABLE_F1_WIDTH                                      1
#define ENET_SLV_RDY_TMO_ENABLE_F1_SHIFT                                     15
#define ENET_SLV_RDY_TMO_ENABLE_F1_MASK                              0x00008000
#define ENET_SLV_RDY_TMO_ENABLE_F1_RD(src)           (((src) & 0x00008000)>>15)
#define ENET_SLV_RDY_TMO_ENABLE_F1_WR(src)      (((u32)(src)<<15) & 0x00008000)
#define ENET_SLV_RDY_TMO_ENABLE_F1_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))
/*	 Fields SLV_RDY_TMO_VALUE	 */
#define ENET_SLV_RDY_TMO_VALUE_F1_WIDTH                                      10
#define ENET_SLV_RDY_TMO_VALUE_F1_SHIFT                                       0
#define ENET_SLV_RDY_TMO_VALUE_F1_MASK                               0x000003ff
#define ENET_SLV_RDY_TMO_VALUE_F1_RD(src)                (((src) & 0x000003ff))
#define ENET_SLV_RDY_TMO_VALUE_F1_WR(src)           (((u32)(src)) & 0x000003ff)
#define ENET_SLV_RDY_TMO_VALUE_F1_SET(dst,src) \
                          (((dst) & ~0x000003ff) | (((u32)(src)) & 0x000003ff))

/*	Register STS_AXI_SLV_TMO	*/ 
/*	 Fields SLV_ARID_TMO	 */
#define ENET_SLV_ARID_TMO_WIDTH                                               1
#define ENET_SLV_ARID_TMO_SHIFT                                               3
#define ENET_SLV_ARID_TMO_MASK                                       0x00000008
#define ENET_SLV_ARID_TMO_RD(src)                     (((src) & 0x00000008)>>3)
#define ENET_SLV_ARID_TMO_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields SLV_ARRDY_TMO	 */
#define ENET_SLV_ARRDY_TMO_WIDTH                                              1
#define ENET_SLV_ARRDY_TMO_SHIFT                                              2
#define ENET_SLV_ARRDY_TMO_MASK                                      0x00000004
#define ENET_SLV_ARRDY_TMO_RD(src)                    (((src) & 0x00000004)>>2)
#define ENET_SLV_ARRDY_TMO_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields SLV_AWID_TMO	 */
#define ENET_SLV_AWID_TMO_WIDTH                                               1
#define ENET_SLV_AWID_TMO_SHIFT                                               1
#define ENET_SLV_AWID_TMO_MASK                                       0x00000002
#define ENET_SLV_AWID_TMO_RD(src)                     (((src) & 0x00000002)>>1)
#define ENET_SLV_AWID_TMO_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields SLV_AWRDY_TMO	 */
#define ENET_SLV_AWRDY_TMO_WIDTH                                              1
#define ENET_SLV_AWRDY_TMO_SHIFT                                              0
#define ENET_SLV_AWRDY_TMO_MASK                                      0x00000001
#define ENET_SLV_AWRDY_TMO_RD(src)                       (((src) & 0x00000001))
#define ENET_SLV_AWRDY_TMO_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register CFG_AXI_SS_CSR_TMO	*/ 
/*	 Fields CFG_CSR_TMO	 */
#define ENET_CFG_CSR_TMO_WIDTH                                               16
#define ENET_CFG_CSR_TMO_SHIFT                                               16
#define ENET_CFG_CSR_TMO_MASK                                        0xffff0000
#define ENET_CFG_CSR_TMO_RD(src)                     (((src) & 0xffff0000)>>16)
#define ENET_CFG_CSR_TMO_WR(src)                (((u32)(src)<<16) & 0xffff0000)
#define ENET_CFG_CSR_TMO_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields CSR_ERR_RESP_EN	 */
#define ENET_CSR_ERR_RESP_EN_WIDTH                                            1
#define ENET_CSR_ERR_RESP_EN_SHIFT                                           15
#define ENET_CSR_ERR_RESP_EN_MASK                                    0x00008000
#define ENET_CSR_ERR_RESP_EN_RD(src)                 (((src) & 0x00008000)>>15)
#define ENET_CSR_ERR_RESP_EN_WR(src)            (((u32)(src)<<15) & 0x00008000)
#define ENET_CSR_ERR_RESP_EN_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))

/*	Register STS_READ_LATENCY_TOT_READ_REQS	*/ 
/*	 Fields TOT_READS	 */
#define ENET_TOT_READS_WIDTH                                                 16
#define ENET_TOT_READS_SHIFT                                                  0
#define ENET_TOT_READS_MASK                                          0x0000ffff
#define ENET_TOT_READS_RD(src)                           (((src) & 0x0000ffff))
#define ENET_TOT_READS_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Global Base Address	*/
#define ENET_QMI_SLAVE_BASE_ADDR				0xddd8a7800ULL

/*    Address QMI_SLAVE  Registers */
#define ENET_CFGSSQMI0_ADDR                                          0x00000000
#define ENET_CFGSSQMI0_DEFAULT                                       0x0002c821
#define ENET_CFGSSQMI1_ADDR                                          0x00000004
#define ENET_CFGSSQMI1_DEFAULT                                       0x2de8b77c
#define ENET_CFGSSQMIFPBUFFER_ADDR                                   0x00000008
#define ENET_CFGSSQMIFPBUFFER_DEFAULT                                0x00000000
#define ENET_CFGSSQMIWQBUFFER_ADDR                                   0x0000000c
#define ENET_CFGSSQMIWQBUFFER_DEFAULT                                0x00000000
#define ENET_STSSSQMIFPBUFFER0_ADDR                                  0x00000010
#define ENET_STSSSQMIFPBUFFER0_DEFAULT                               0x00000000
#define ENET_STSSSQMIFPBUFFER1_ADDR                                  0x00000014
#define ENET_STSSSQMIFPBUFFER1_DEFAULT                               0x00000000
#define ENET_STSSSQMIFPBUFFER2_ADDR                                  0x00000018
#define ENET_STSSSQMIFPBUFFER2_DEFAULT                               0x00000000
#define ENET_STSSSQMIWQBUFFER0_ADDR                                  0x0000001c
#define ENET_STSSSQMIWQBUFFER0_DEFAULT                               0x00000000
#define ENET_STSSSQMIWQBUFFER1_ADDR                                  0x00000020
#define ENET_STSSSQMIWQBUFFER1_DEFAULT                               0x00000000
#define ENET_STSSSQMIWQBUFFER2_ADDR                                  0x00000024
#define ENET_STSSSQMIWQBUFFER2_DEFAULT                               0x00000000
#define ENET_STSSSQMIWQBUFFER3_ADDR                                  0x00000028
#define ENET_STSSSQMIWQBUFFER3_DEFAULT                               0x00000000
#define ENET_CFGSSQMISAB_ADDR                                        0x0000002c
#define ENET_CFGSSQMISAB_DEFAULT                                     0x00000000
#define ENET_CFGSSQMISAB0_ADDR                                       0x00000030
#define ENET_CFGSSQMISAB0_DEFAULT                                    0x00000000
#define ENET_CFGSSQMISAB1_ADDR                                       0x00000034
#define ENET_CFGSSQMISAB1_DEFAULT                                    0x00000000
#define ENET_CFGSSQMISAB2_ADDR                                       0x00000038
#define ENET_CFGSSQMISAB2_DEFAULT                                    0x00000000
#define ENET_CFGSSQMISAB3_ADDR                                       0x0000003c
#define ENET_CFGSSQMISAB3_DEFAULT                                    0x00000000
#define ENET_CFGSSQMISAB4_ADDR                                       0x00000040
#define ENET_CFGSSQMISAB4_DEFAULT                                    0x00000000
#define ENET_STSSSQMIINT0_ADDR                                       0x00000044
#define ENET_STSSSQMIINT0_DEFAULT                                    0x00000000
#define ENET_STSSSQMIINT0MASK_ADDR                                   0x00000048
#define ENET_STSSSQMIINT1_ADDR                                       0x0000004c
#define ENET_STSSSQMIINT1_DEFAULT                                    0x00000000
#define ENET_STSSSQMIINT1MASK_ADDR                                   0x00000050
#define ENET_STSSSQMIINT2_ADDR                                       0x00000054
#define ENET_STSSSQMIINT2_DEFAULT                                    0x00000000
#define ENET_STSSSQMIINT2MASK_ADDR                                   0x00000058
#define ENET_STSSSQMIINT3_ADDR                                       0x0000005c
#define ENET_STSSSQMIINT3_DEFAULT                                    0x00000000
#define ENET_STSSSQMIINT3MASK_ADDR                                   0x00000060
#define ENET_STSSSQMIINT4_ADDR                                       0x00000064
#define ENET_STSSSQMIINT4_DEFAULT                                    0x00000000
#define ENET_STSSSQMIINT4MASK_ADDR                                   0x00000068
#define ENET_CFGSSQMIDBGCTRL_ADDR                                    0x0000006c
#define ENET_CFGSSQMIDBGCTRL_DEFAULT                                 0x00000000
#define ENET_CFGSSQMIDBGDATA0_ADDR                                   0x00000070
#define ENET_CFGSSQMIDBGDATA0_DEFAULT                                0x00000000
#define ENET_CFGSSQMIDBGDATA1_ADDR                                   0x00000074
#define ENET_CFGSSQMIDBGDATA1_DEFAULT                                0x00000000
#define ENET_CFGSSQMIDBGDATA2_ADDR                                   0x00000078
#define ENET_CFGSSQMIDBGDATA2_DEFAULT                                0x00000000
#define ENET_CFGSSQMIDBGDATA3_ADDR                                   0x0000007c
#define ENET_CFGSSQMIDBGDATA3_DEFAULT                                0x00000000
#define ENET_STSSSQMIDBGDATA_ADDR                                    0x00000080
#define ENET_STSSSQMIDBGDATA_DEFAULT                                 0x00000000
#define ENET_CFGSSQMILITE_ADDR                                       0x00000084
#define ENET_CFGSSQMILITE_DEFAULT                                    0x00003400
#define ENET_CFGSSQMIFPQASSOC_ADDR                                   0x00000088
#define ENET_CFGSSQMIFPQASSOC_DEFAULT                                0x00030000
#define ENET_CFGSSQMIWQASSOC_ADDR                                    0x0000008c
#define ENET_CFGSSQMIWQASSOC_DEFAULT                                 0x00010000
#define ENET_CFGSSQMIMEMORY_ADDR                                     0x00000090
#define ENET_CFGSSQMIMEMORY_DEFAULT                                  0x00000000
#define ENET_STSSSQMIFIFO_ADDR                                       0x00000094
#define ENET_STSSSQMIFIFO_DEFAULT                                    0x07ffffff

/*	Register CfgSsQmi0	*/ 
/*	 Fields WQ16Disable	 */
#define ENET_WQ16DISABLE0_WIDTH                                               1
#define ENET_WQ16DISABLE0_SHIFT                                              29
#define ENET_WQ16DISABLE0_MASK                                       0x20000000
#define ENET_WQ16DISABLE0_RD(src)                    (((src) & 0x20000000)>>29)
#define ENET_WQ16DISABLE0_WR(src)               (((u32)(src)<<29) & 0x20000000)
#define ENET_WQ16DISABLE0_SET(dst,src) \
                      (((dst) & ~0x20000000) | (((u32)(src)<<29) & 0x20000000))
/*	 Fields WQ16Reset	 */
#define ENET_WQ16RESET0_WIDTH                                                 1
#define ENET_WQ16RESET0_SHIFT                                                28
#define ENET_WQ16RESET0_MASK                                         0x10000000
#define ENET_WQ16RESET0_RD(src)                      (((src) & 0x10000000)>>28)
#define ENET_WQ16RESET0_WR(src)                 (((u32)(src)<<28) & 0x10000000)
#define ENET_WQ16RESET0_SET(dst,src) \
                      (((dst) & ~0x10000000) | (((u32)(src)<<28) & 0x10000000))
/*	 Fields FP17Disable	 */
#define ENET_FP17DISABLE0_WIDTH                                               1
#define ENET_FP17DISABLE0_SHIFT                                              27
#define ENET_FP17DISABLE0_MASK                                       0x08000000
#define ENET_FP17DISABLE0_RD(src)                    (((src) & 0x08000000)>>27)
#define ENET_FP17DISABLE0_WR(src)               (((u32)(src)<<27) & 0x08000000)
#define ENET_FP17DISABLE0_SET(dst,src) \
                      (((dst) & ~0x08000000) | (((u32)(src)<<27) & 0x08000000))
/*	 Fields FP16Disable	 */
#define ENET_FP16DISABLE0_WIDTH                                               1
#define ENET_FP16DISABLE0_SHIFT                                              26
#define ENET_FP16DISABLE0_MASK                                       0x04000000
#define ENET_FP16DISABLE0_RD(src)                    (((src) & 0x04000000)>>26)
#define ENET_FP16DISABLE0_WR(src)               (((u32)(src)<<26) & 0x04000000)
#define ENET_FP16DISABLE0_SET(dst,src) \
                      (((dst) & ~0x04000000) | (((u32)(src)<<26) & 0x04000000))
/*	 Fields FP17Reset	 */
#define ENET_FP17RESET0_WIDTH                                                 1
#define ENET_FP17RESET0_SHIFT                                                25
#define ENET_FP17RESET0_MASK                                         0x02000000
#define ENET_FP17RESET0_RD(src)                      (((src) & 0x02000000)>>25)
#define ENET_FP17RESET0_WR(src)                 (((u32)(src)<<25) & 0x02000000)
#define ENET_FP17RESET0_SET(dst,src) \
                      (((dst) & ~0x02000000) | (((u32)(src)<<25) & 0x02000000))
/*	 Fields FP16Reset	 */
#define ENET_FP16RESET0_WIDTH                                                 1
#define ENET_FP16RESET0_SHIFT                                                24
#define ENET_FP16RESET0_MASK                                         0x01000000
#define ENET_FP16RESET0_RD(src)                      (((src) & 0x01000000)>>24)
#define ENET_FP16RESET0_WR(src)                 (((u32)(src)<<24) & 0x01000000)
#define ENET_FP16RESET0_SET(dst,src) \
                      (((dst) & ~0x01000000) | (((u32)(src)<<24) & 0x01000000))
/*	 Fields FPDecDiffThreshold	 */
#define ENET_FPDECDIFFTHRESHOLD0_WIDTH                                        3
#define ENET_FPDECDIFFTHRESHOLD0_SHIFT                                       21
#define ENET_FPDECDIFFTHRESHOLD0_MASK                                0x00e00000
#define ENET_FPDECDIFFTHRESHOLD0_RD(src)             (((src) & 0x00e00000)>>21)
#define ENET_FPDECDIFFTHRESHOLD0_WR(src)        (((u32)(src)<<21) & 0x00e00000)
#define ENET_FPDECDIFFTHRESHOLD0_SET(dst,src) \
                      (((dst) & ~0x00e00000) | (((u32)(src)<<21) & 0x00e00000))
/*	 Fields WQDecDiffThreshold	 */
#define ENET_WQDECDIFFTHRESHOLD0_WIDTH                                        4
#define ENET_WQDECDIFFTHRESHOLD0_SHIFT                                       17
#define ENET_WQDECDIFFTHRESHOLD0_MASK                                0x001e0000
#define ENET_WQDECDIFFTHRESHOLD0_RD(src)             (((src) & 0x001e0000)>>17)
#define ENET_WQDECDIFFTHRESHOLD0_WR(src)        (((u32)(src)<<17) & 0x001e0000)
#define ENET_WQDECDIFFTHRESHOLD0_SET(dst,src) \
                      (((dst) & ~0x001e0000) | (((u32)(src)<<17) & 0x001e0000))
/*	 Fields DeallocThreshold	 */
#define ENET_DEALLOCTHRESHOLD0_WIDTH                                          3
#define ENET_DEALLOCTHRESHOLD0_SHIFT                                         14
#define ENET_DEALLOCTHRESHOLD0_MASK                                  0x0001c000
#define ENET_DEALLOCTHRESHOLD0_RD(src)               (((src) & 0x0001c000)>>14)
#define ENET_DEALLOCTHRESHOLD0_WR(src)          (((u32)(src)<<14) & 0x0001c000)
#define ENET_DEALLOCTHRESHOLD0_SET(dst,src) \
                      (((dst) & ~0x0001c000) | (((u32)(src)<<14) & 0x0001c000))
/*	 Fields FPDecThreshold	 */
#define ENET_FPDECTHRESHOLD0_WIDTH                                            3
#define ENET_FPDECTHRESHOLD0_SHIFT                                           11
#define ENET_FPDECTHRESHOLD0_MASK                                    0x00003800
#define ENET_FPDECTHRESHOLD0_RD(src)                 (((src) & 0x00003800)>>11)
#define ENET_FPDECTHRESHOLD0_WR(src)            (((u32)(src)<<11) & 0x00003800)
#define ENET_FPDECTHRESHOLD0_SET(dst,src) \
                      (((dst) & ~0x00003800) | (((u32)(src)<<11) & 0x00003800))
/*	 Fields FPBAvlThreshold	 */
#define ENET_FPBAVLTHRESHOLD0_WIDTH                                           3
#define ENET_FPBAVLTHRESHOLD0_SHIFT                                           8
#define ENET_FPBAVLTHRESHOLD0_MASK                                   0x00000700
#define ENET_FPBAVLTHRESHOLD0_RD(src)                 (((src) & 0x00000700)>>8)
#define ENET_FPBAVLTHRESHOLD0_WR(src)            (((u32)(src)<<8) & 0x00000700)
#define ENET_FPBAVLTHRESHOLD0_SET(dst,src) \
                       (((dst) & ~0x00000700) | (((u32)(src)<<8) & 0x00000700))
/*	 Fields WQDecThreshold	 */
#define ENET_WQDECTHRESHOLD0_WIDTH                                            4
#define ENET_WQDECTHRESHOLD0_SHIFT                                            4
#define ENET_WQDECTHRESHOLD0_MASK                                    0x000000f0
#define ENET_WQDECTHRESHOLD0_RD(src)                  (((src) & 0x000000f0)>>4)
#define ENET_WQDECTHRESHOLD0_WR(src)             (((u32)(src)<<4) & 0x000000f0)
#define ENET_WQDECTHRESHOLD0_SET(dst,src) \
                       (((dst) & ~0x000000f0) | (((u32)(src)<<4) & 0x000000f0))
/*	 Fields WQBAvlThreshold	 */
#define ENET_WQBAVLTHRESHOLD0_WIDTH                                           4
#define ENET_WQBAVLTHRESHOLD0_SHIFT                                           0
#define ENET_WQBAVLTHRESHOLD0_MASK                                   0x0000000f
#define ENET_WQBAVLTHRESHOLD0_RD(src)                    (((src) & 0x0000000f))
#define ENET_WQBAVLTHRESHOLD0_WR(src)               (((u32)(src)) & 0x0000000f)
#define ENET_WQBAVLTHRESHOLD0_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register CfgSsQmi1	*/ 
/*	 Fields CmOverrideLLFields	 */
#define ENET_CMOVERRIDELLFIELDS1_WIDTH                                        1
#define ENET_CMOVERRIDELLFIELDS1_SHIFT                                       29
#define ENET_CMOVERRIDELLFIELDS1_MASK                                0x20000000
#define ENET_CMOVERRIDELLFIELDS1_RD(src)             (((src) & 0x20000000)>>29)
#define ENET_CMOVERRIDELLFIELDS1_WR(src)        (((u32)(src)<<29) & 0x20000000)
#define ENET_CMOVERRIDELLFIELDS1_SET(dst,src) \
                      (((dst) & ~0x20000000) | (((u32)(src)<<29) & 0x20000000))
/*	 Fields CmCtrlbuffThreshold	 */
#define ENET_CMCTRLBUFFTHRESHOLD1_WIDTH                                       3
#define ENET_CMCTRLBUFFTHRESHOLD1_SHIFT                                      26
#define ENET_CMCTRLBUFFTHRESHOLD1_MASK                               0x1c000000
#define ENET_CMCTRLBUFFTHRESHOLD1_RD(src)            (((src) & 0x1c000000)>>26)
#define ENET_CMCTRLBUFFTHRESHOLD1_WR(src)       (((u32)(src)<<26) & 0x1c000000)
#define ENET_CMCTRLBUFFTHRESHOLD1_SET(dst,src) \
                      (((dst) & ~0x1c000000) | (((u32)(src)<<26) & 0x1c000000))
/*	 Fields CmDatabuffThreshold	 */
#define ENET_CMDATABUFFTHRESHOLD1_WIDTH                                       5
#define ENET_CMDATABUFFTHRESHOLD1_SHIFT                                      21
#define ENET_CMDATABUFFTHRESHOLD1_MASK                               0x03e00000
#define ENET_CMDATABUFFTHRESHOLD1_RD(src)            (((src) & 0x03e00000)>>21)
#define ENET_CMDATABUFFTHRESHOLD1_WR(src)       (((u32)(src)<<21) & 0x03e00000)
#define ENET_CMDATABUFFTHRESHOLD1_SET(dst,src) \
                      (((dst) & ~0x03e00000) | (((u32)(src)<<21) & 0x03e00000))
/*	 Fields CmMsgfThreshold	 */
#define ENET_CMMSGFTHRESHOLD1_WIDTH                                           4
#define ENET_CMMSGFTHRESHOLD1_SHIFT                                          17
#define ENET_CMMSGFTHRESHOLD1_MASK                                   0x001e0000
#define ENET_CMMSGFTHRESHOLD1_RD(src)                (((src) & 0x001e0000)>>17)
#define ENET_CMMSGFTHRESHOLD1_WR(src)           (((u32)(src)<<17) & 0x001e0000)
#define ENET_CMMSGFTHRESHOLD1_SET(dst,src) \
                      (((dst) & ~0x001e0000) | (((u32)(src)<<17) & 0x001e0000))
/*	 Fields CmRegfThreshold	 */
#define ENET_CMREGFTHRESHOLD1_WIDTH                                           3
#define ENET_CMREGFTHRESHOLD1_SHIFT                                          14
#define ENET_CMREGFTHRESHOLD1_MASK                                   0x0001c000
#define ENET_CMREGFTHRESHOLD1_RD(src)                (((src) & 0x0001c000)>>14)
#define ENET_CMREGFTHRESHOLD1_WR(src)           (((u32)(src)<<14) & 0x0001c000)
#define ENET_CMREGFTHRESHOLD1_SET(dst,src) \
                      (((dst) & ~0x0001c000) | (((u32)(src)<<14) & 0x0001c000))
/*	 Fields QMDeviceAddress	 */
#define ENET_QMDEVICEADDRESS1_WIDTH                                          14
#define ENET_QMDEVICEADDRESS1_SHIFT                                           0
#define ENET_QMDEVICEADDRESS1_MASK                                   0x00003fff
#define ENET_QMDEVICEADDRESS1_RD(src)                    (((src) & 0x00003fff))
#define ENET_QMDEVICEADDRESS1_WR(src)               (((u32)(src)) & 0x00003fff)
#define ENET_QMDEVICEADDRESS1_SET(dst,src) \
                          (((dst) & ~0x00003fff) | (((u32)(src)) & 0x00003fff))

/*	Register CfgSsQmiFPBuffer	*/ 
/*	 Fields Disable	 */
#define ENET_DISABLE_WIDTH                                                   16
#define ENET_DISABLE_SHIFT                                                   16
#define ENET_DISABLE_MASK                                            0xffff0000
#define ENET_DISABLE_RD(src)                         (((src) & 0xffff0000)>>16)
#define ENET_DISABLE_WR(src)                    (((u32)(src)<<16) & 0xffff0000)
#define ENET_DISABLE_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields Reset	 */
#define ENET_RESET_WIDTH                                                     16
#define ENET_RESET_SHIFT                                                      0
#define ENET_RESET_MASK                                              0x0000ffff
#define ENET_RESET_RD(src)                               (((src) & 0x0000ffff))
#define ENET_RESET_WR(src)                          (((u32)(src)) & 0x0000ffff)
#define ENET_RESET_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register CfgSsQmiWQBuffer	*/ 
/*	 Fields Disable	 */
#define ENET_DISABLE_F1_WIDTH                                                16
#define ENET_DISABLE_F1_SHIFT                                                16
#define ENET_DISABLE_F1_MASK                                         0xffff0000
#define ENET_DISABLE_F1_RD(src)                      (((src) & 0xffff0000)>>16)
#define ENET_DISABLE_F1_WR(src)                 (((u32)(src)<<16) & 0xffff0000)
#define ENET_DISABLE_F1_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields Reset	 */
#define ENET_RESET_F1_WIDTH                                                  16
#define ENET_RESET_F1_SHIFT                                                   0
#define ENET_RESET_F1_MASK                                           0x0000ffff
#define ENET_RESET_F1_RD(src)                            (((src) & 0x0000ffff))
#define ENET_RESET_F1_WR(src)                       (((u32)(src)) & 0x0000ffff)
#define ENET_RESET_F1_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register StsSsQmiFPBuffer0	*/ 
/*	 Fields WritePointer	 */
#define ENET_WRITEPOINTER0_WIDTH                                             32
#define ENET_WRITEPOINTER0_SHIFT                                              0
#define ENET_WRITEPOINTER0_MASK                                      0xffffffff
#define ENET_WRITEPOINTER0_RD(src)                       (((src) & 0xffffffff))
#define ENET_WRITEPOINTER0_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register StsSsQmiFPBuffer1	*/ 
/*	 Fields WritePointer	 */
#define ENET_WRITEPOINTER1_WIDTH                                              4
#define ENET_WRITEPOINTER1_SHIFT                                             24
#define ENET_WRITEPOINTER1_MASK                                      0x0f000000
#define ENET_WRITEPOINTER1_RD(src)                   (((src) & 0x0f000000)>>24)
#define ENET_WRITEPOINTER1_SET(dst,src) \
                      (((dst) & ~0x0f000000) | (((u32)(src)<<24) & 0x0f000000))
/*	 Fields Entries	 */
#define ENET_ENTRIES1_WIDTH                                                  24
#define ENET_ENTRIES1_SHIFT                                                   0
#define ENET_ENTRIES1_MASK                                           0x00ffffff
#define ENET_ENTRIES1_RD(src)                            (((src) & 0x00ffffff))
#define ENET_ENTRIES1_SET(dst,src) \
                          (((dst) & ~0x00ffffff) | (((u32)(src)) & 0x00ffffff))

/*	Register StsSsQmiFPBuffer2	*/ 
/*	 Fields Entries	 */
#define ENET_ENTRIES2_WIDTH                                                  30
#define ENET_ENTRIES2_SHIFT                                                   0
#define ENET_ENTRIES2_MASK                                           0x3fffffff
#define ENET_ENTRIES2_RD(src)                            (((src) & 0x3fffffff))
#define ENET_ENTRIES2_SET(dst,src) \
                          (((dst) & ~0x3fffffff) | (((u32)(src)) & 0x3fffffff))

/*	Register StsSsQmiWQBuffer0	*/ 
/*	 Fields WritePointer	 */
#define ENET_WRITEPOINTER0_F1_WIDTH                                          24
#define ENET_WRITEPOINTER0_F1_SHIFT                                           0
#define ENET_WRITEPOINTER0_F1_MASK                                   0x00ffffff
#define ENET_WRITEPOINTER0_F1_RD(src)                    (((src) & 0x00ffffff))
#define ENET_WRITEPOINTER0_F1_SET(dst,src) \
                          (((dst) & ~0x00ffffff) | (((u32)(src)) & 0x00ffffff))

/*	Register StsSsQmiWQBuffer1	*/ 
/*	 Fields Entries	 */
#define ENET_ENTRIES1_F1_WIDTH                                                4
#define ENET_ENTRIES1_F1_SHIFT                                               27
#define ENET_ENTRIES1_F1_MASK                                        0x78000000
#define ENET_ENTRIES1_F1_RD(src)                     (((src) & 0x78000000)>>27)
#define ENET_ENTRIES1_F1_SET(dst,src) \
                      (((dst) & ~0x78000000) | (((u32)(src)<<27) & 0x78000000))
/*	 Fields WritePointer	 */
#define ENET_WRITEPOINTER1_F1_WIDTH                                          27
#define ENET_WRITEPOINTER1_F1_SHIFT                                           0
#define ENET_WRITEPOINTER1_F1_MASK                                   0x07ffffff
#define ENET_WRITEPOINTER1_F1_RD(src)                    (((src) & 0x07ffffff))
#define ENET_WRITEPOINTER1_F1_SET(dst,src) \
                          (((dst) & ~0x07ffffff) | (((u32)(src)) & 0x07ffffff))

/*	Register StsSsQmiWQBuffer2	*/ 
/*	 Fields Entries	 */
#define ENET_ENTRIES2_F1_WIDTH                                               32
#define ENET_ENTRIES2_F1_SHIFT                                                0
#define ENET_ENTRIES2_F1_MASK                                        0xffffffff
#define ENET_ENTRIES2_F1_RD(src)                         (((src) & 0xffffffff))
#define ENET_ENTRIES2_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register StsSsQmiWQBuffer3	*/ 
/*	 Fields Entries	 */
#define ENET_ENTRIES3_WIDTH                                                  32
#define ENET_ENTRIES3_SHIFT                                                   0
#define ENET_ENTRIES3_MASK                                           0xffffffff
#define ENET_ENTRIES3_RD(src)                            (((src) & 0xffffffff))
#define ENET_ENTRIES3_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CfgSsQmiSab	*/ 
/*	 Fields EnableMonitoring	 */
#define ENET_ENABLEMONITORING_WIDTH                                          17
#define ENET_ENABLEMONITORING_SHIFT                                           0
#define ENET_ENABLEMONITORING_MASK                                   0x0001ffff
#define ENET_ENABLEMONITORING_RD(src)                    (((src) & 0x0001ffff))
#define ENET_ENABLEMONITORING_WR(src)               (((u32)(src)) & 0x0001ffff)
#define ENET_ENABLEMONITORING_SET(dst,src) \
                          (((dst) & ~0x0001ffff) | (((u32)(src)) & 0x0001ffff))

/*	Register CfgSsQmiSab0	*/ 
/*	 Fields QID	 */
#define ENET_QID0_WIDTH                                                      32
#define ENET_QID0_SHIFT                                                       0
#define ENET_QID0_MASK                                               0xffffffff
#define ENET_QID0_RD(src)                                (((src) & 0xffffffff))
#define ENET_QID0_WR(src)                           (((u32)(src)) & 0xffffffff)
#define ENET_QID0_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CfgSsQmiSab1	*/ 
/*	 Fields QID	 */
#define ENET_QID1_WIDTH                                                      32
#define ENET_QID1_SHIFT                                                       0
#define ENET_QID1_MASK                                               0xffffffff
#define ENET_QID1_RD(src)                                (((src) & 0xffffffff))
#define ENET_QID1_WR(src)                           (((u32)(src)) & 0xffffffff)
#define ENET_QID1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CfgSsQmiSab2	*/ 
/*	 Fields QID	 */
#define ENET_QID2_WIDTH                                                      32
#define ENET_QID2_SHIFT                                                       0
#define ENET_QID2_MASK                                               0xffffffff
#define ENET_QID2_RD(src)                                (((src) & 0xffffffff))
#define ENET_QID2_WR(src)                           (((u32)(src)) & 0xffffffff)
#define ENET_QID2_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CfgSsQmiSab3	*/ 
/*	 Fields QID	 */
#define ENET_QID3_WIDTH                                                      32
#define ENET_QID3_SHIFT                                                       0
#define ENET_QID3_MASK                                               0xffffffff
#define ENET_QID3_RD(src)                                (((src) & 0xffffffff))
#define ENET_QID3_WR(src)                           (((u32)(src)) & 0xffffffff)
#define ENET_QID3_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CfgSsQmiSab4	*/ 
/*	 Fields QID	 */
#define ENET_QID4_WIDTH                                                       8
#define ENET_QID4_SHIFT                                                       0
#define ENET_QID4_MASK                                               0x000000ff
#define ENET_QID4_RD(src)                                (((src) & 0x000000ff))
#define ENET_QID4_WR(src)                           (((u32)(src)) & 0x000000ff)
#define ENET_QID4_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register StsSsQmiInt0	*/ 
/*	 Fields FPOverflow	 */
#define ENET_FPOVERFLOW0_WIDTH                                               18
#define ENET_FPOVERFLOW0_SHIFT                                                0
#define ENET_FPOVERFLOW0_MASK                                        0x0003ffff
#define ENET_FPOVERFLOW0_RD(src)                         (((src) & 0x0003ffff))
#define ENET_FPOVERFLOW0_WR(src)                    (((u32)(src)) & 0x0003ffff)
#define ENET_FPOVERFLOW0_SET(dst,src) \
                          (((dst) & ~0x0003ffff) | (((u32)(src)) & 0x0003ffff))

/*	Register StsSsQmiInt0Mask	*/
/*    Mask Register Fields FPOverflowMask    */
#define ENET_FPOVERFLOWMASK_WIDTH                                            18
#define ENET_FPOVERFLOWMASK_SHIFT                                             0
#define ENET_FPOVERFLOWMASK_MASK                                     0x0003ffff
#define ENET_FPOVERFLOWMASK_RD(src)                      (((src) & 0x0003ffff))
#define ENET_FPOVERFLOWMASK_WR(src)                 (((u32)(src)) & 0x0003ffff)
#define ENET_FPOVERFLOWMASK_SET(dst,src) \
                          (((dst) & ~0x0003ffff) | (((u32)(src)) & 0x0003ffff))

/*	Register StsSsQmiInt1	*/ 
/*	 Fields WQOverflow	 */
#define ENET_WQOVERFLOW1_WIDTH                                               17
#define ENET_WQOVERFLOW1_SHIFT                                                0
#define ENET_WQOVERFLOW1_MASK                                        0x0001ffff
#define ENET_WQOVERFLOW1_RD(src)                         (((src) & 0x0001ffff))
#define ENET_WQOVERFLOW1_WR(src)                    (((u32)(src)) & 0x0001ffff)
#define ENET_WQOVERFLOW1_SET(dst,src) \
                          (((dst) & ~0x0001ffff) | (((u32)(src)) & 0x0001ffff))

/*	Register StsSsQmiInt1Mask	*/
/*    Mask Register Fields WQOverflowMask    */
#define ENET_WQOVERFLOWMASK_WIDTH                                            17
#define ENET_WQOVERFLOWMASK_SHIFT                                             0
#define ENET_WQOVERFLOWMASK_MASK                                     0x0001ffff
#define ENET_WQOVERFLOWMASK_RD(src)                      (((src) & 0x0001ffff))
#define ENET_WQOVERFLOWMASK_WR(src)                 (((u32)(src)) & 0x0001ffff)
#define ENET_WQOVERFLOWMASK_SET(dst,src) \
                          (((dst) & ~0x0001ffff) | (((u32)(src)) & 0x0001ffff))

/*	Register StsSsQmiInt2	*/ 
/*	 Fields FPUnderRun	 */
#define ENET_FPUNDERRUN2_WIDTH                                               18
#define ENET_FPUNDERRUN2_SHIFT                                                0
#define ENET_FPUNDERRUN2_MASK                                        0x0003ffff
#define ENET_FPUNDERRUN2_RD(src)                         (((src) & 0x0003ffff))
#define ENET_FPUNDERRUN2_WR(src)                    (((u32)(src)) & 0x0003ffff)
#define ENET_FPUNDERRUN2_SET(dst,src) \
                          (((dst) & ~0x0003ffff) | (((u32)(src)) & 0x0003ffff))

/*	Register StsSsQmiInt2Mask	*/
/*    Mask Register Fields FPUnderRunMask    */
#define ENET_FPUNDERRUNMASK_WIDTH                                            18
#define ENET_FPUNDERRUNMASK_SHIFT                                             0
#define ENET_FPUNDERRUNMASK_MASK                                     0x0003ffff
#define ENET_FPUNDERRUNMASK_RD(src)                      (((src) & 0x0003ffff))
#define ENET_FPUNDERRUNMASK_WR(src)                 (((u32)(src)) & 0x0003ffff)
#define ENET_FPUNDERRUNMASK_SET(dst,src) \
                          (((dst) & ~0x0003ffff) | (((u32)(src)) & 0x0003ffff))

/*	Register StsSsQmiInt3	*/ 
/*	 Fields WQUnderRun	 */
#define ENET_WQUNDERRUN3_WIDTH                                               17
#define ENET_WQUNDERRUN3_SHIFT                                                0
#define ENET_WQUNDERRUN3_MASK                                        0x0001ffff
#define ENET_WQUNDERRUN3_RD(src)                         (((src) & 0x0001ffff))
#define ENET_WQUNDERRUN3_WR(src)                    (((u32)(src)) & 0x0001ffff)
#define ENET_WQUNDERRUN3_SET(dst,src) \
                          (((dst) & ~0x0001ffff) | (((u32)(src)) & 0x0001ffff))

/*	Register StsSsQmiInt3Mask	*/
/*    Mask Register Fields WQUnderRunMask    */
#define ENET_WQUNDERRUNMASK_WIDTH                                            17
#define ENET_WQUNDERRUNMASK_SHIFT                                             0
#define ENET_WQUNDERRUNMASK_MASK                                     0x0001ffff
#define ENET_WQUNDERRUNMASK_RD(src)                      (((src) & 0x0001ffff))
#define ENET_WQUNDERRUNMASK_WR(src)                 (((u32)(src)) & 0x0001ffff)
#define ENET_WQUNDERRUNMASK_SET(dst,src) \
                          (((dst) & ~0x0001ffff) | (((u32)(src)) & 0x0001ffff))

/*	Register StsSsQmiInt4	*/ 
/*	 Fields axiwcmr_slverr	 */
#define ENET_AXIWCMR_SLVERR4_WIDTH                                            1
#define ENET_AXIWCMR_SLVERR4_SHIFT                                            1
#define ENET_AXIWCMR_SLVERR4_MASK                                    0x00000002
#define ENET_AXIWCMR_SLVERR4_RD(src)                  (((src) & 0x00000002)>>1)
#define ENET_AXIWCMR_SLVERR4_WR(src)             (((u32)(src)<<1) & 0x00000002)
#define ENET_AXIWCMR_SLVERR4_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields axiwcmr_decerr	 */
#define ENET_AXIWCMR_DECERR4_WIDTH                                            1
#define ENET_AXIWCMR_DECERR4_SHIFT                                            0
#define ENET_AXIWCMR_DECERR4_MASK                                    0x00000001
#define ENET_AXIWCMR_DECERR4_RD(src)                     (((src) & 0x00000001))
#define ENET_AXIWCMR_DECERR4_WR(src)                (((u32)(src)) & 0x00000001)
#define ENET_AXIWCMR_DECERR4_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register StsSsQmiInt4Mask	*/
/*    Mask Register Fields axiwcmr_slverrMask    */
#define ENET_AXIWCMR_SLVERRMASK_WIDTH                                         1
#define ENET_AXIWCMR_SLVERRMASK_SHIFT                                         1
#define ENET_AXIWCMR_SLVERRMASK_MASK                                 0x00000002
#define ENET_AXIWCMR_SLVERRMASK_RD(src)               (((src) & 0x00000002)>>1)
#define ENET_AXIWCMR_SLVERRMASK_WR(src)          (((u32)(src)<<1) & 0x00000002)
#define ENET_AXIWCMR_SLVERRMASK_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*    Mask Register Fields axiwcmr_decerrMask    */
#define ENET_AXIWCMR_DECERRMASK_WIDTH                                         1
#define ENET_AXIWCMR_DECERRMASK_SHIFT                                         0
#define ENET_AXIWCMR_DECERRMASK_MASK                                 0x00000001
#define ENET_AXIWCMR_DECERRMASK_RD(src)                  (((src) & 0x00000001))
#define ENET_AXIWCMR_DECERRMASK_WR(src)             (((u32)(src)) & 0x00000001)
#define ENET_AXIWCMR_DECERRMASK_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register CfgSsQmiDbgCtrl	*/ 
/*	 Fields PBID	 */
#define ENET_PBID_WIDTH                                                       6
#define ENET_PBID_SHIFT                                                      11
#define ENET_PBID_MASK                                               0x0001f800
#define ENET_PBID_RD(src)                            (((src) & 0x0001f800)>>11)
#define ENET_PBID_WR(src)                       (((u32)(src)<<11) & 0x0001f800)
#define ENET_PBID_SET(dst,src) \
                      (((dst) & ~0x0001f800) | (((u32)(src)<<11) & 0x0001f800))
/*	 Fields nAck	 */
#define ENET_NACK_WIDTH                                                       1
#define ENET_NACK_SHIFT                                                      10
#define ENET_NACK_MASK                                               0x00000400
#define ENET_NACK_RD(src)                            (((src) & 0x00000400)>>10)
#define ENET_NACK_WR(src)                       (((u32)(src)<<10) & 0x00000400)
#define ENET_NACK_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields last	 */
#define ENET_LAST_WIDTH                                                       1
#define ENET_LAST_SHIFT                                                       9
#define ENET_LAST_MASK                                               0x00000200
#define ENET_LAST_RD(src)                             (((src) & 0x00000200)>>9)
#define ENET_LAST_WR(src)                        (((u32)(src)<<9) & 0x00000200)
#define ENET_LAST_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields Push	 */
#define ENET_PUSH_WIDTH                                                       1
#define ENET_PUSH_SHIFT                                                       8
#define ENET_PUSH_MASK                                               0x00000100
#define ENET_PUSH_RD(src)                             (((src) & 0x00000100)>>8)
#define ENET_PUSH_WR(src)                        (((u32)(src)<<8) & 0x00000100)
#define ENET_PUSH_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*	 Fields Write	 */
#define ENET_WRITE_WIDTH                                                      1
#define ENET_WRITE_SHIFT                                                      7
#define ENET_WRITE_MASK                                              0x00000080
#define ENET_WRITE_RD(src)                            (((src) & 0x00000080)>>7)
#define ENET_WRITE_WR(src)                       (((u32)(src)<<7) & 0x00000080)
#define ENET_WRITE_SET(dst,src) \
                       (((dst) & ~0x00000080) | (((u32)(src)<<7) & 0x00000080))
/*	 Fields Pop	 */
#define ENET_POP_WIDTH                                                        1
#define ENET_POP_SHIFT                                                        6
#define ENET_POP_MASK                                                0x00000040
#define ENET_POP_RD(src)                              (((src) & 0x00000040)>>6)
#define ENET_POP_WR(src)                         (((u32)(src)<<6) & 0x00000040)
#define ENET_POP_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*	 Fields Read	 */
#define ENET_READ_WIDTH                                                       1
#define ENET_READ_SHIFT                                                       5
#define ENET_READ_MASK                                               0x00000020
#define ENET_READ_RD(src)                             (((src) & 0x00000020)>>5)
#define ENET_READ_WR(src)                        (((u32)(src)<<5) & 0x00000020)
#define ENET_READ_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields BufferAddr	 */
#define ENET_BUFFERADDR_WIDTH                                                 5
#define ENET_BUFFERADDR_SHIFT                                                 0
#define ENET_BUFFERADDR_MASK                                         0x0000001f
#define ENET_BUFFERADDR_RD(src)                          (((src) & 0x0000001f))
#define ENET_BUFFERADDR_WR(src)                     (((u32)(src)) & 0x0000001f)
#define ENET_BUFFERADDR_SET(dst,src) \
                          (((dst) & ~0x0000001f) | (((u32)(src)) & 0x0000001f))

/*	Register CfgSsQmiDbgData0	*/ 
/*	 Fields Data	 */
#define ENET_DATA0_WIDTH                                                     32
#define ENET_DATA0_SHIFT                                                      0
#define ENET_DATA0_MASK                                              0xffffffff
#define ENET_DATA0_RD(src)                               (((src) & 0xffffffff))
#define ENET_DATA0_WR(src)                          (((u32)(src)) & 0xffffffff)
#define ENET_DATA0_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CfgSsQmiDbgData1	*/ 
/*	 Fields Data	 */
#define ENET_DATA1_WIDTH                                                     32
#define ENET_DATA1_SHIFT                                                      0
#define ENET_DATA1_MASK                                              0xffffffff
#define ENET_DATA1_RD(src)                               (((src) & 0xffffffff))
#define ENET_DATA1_WR(src)                          (((u32)(src)) & 0xffffffff)
#define ENET_DATA1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CfgSsQmiDbgData2	*/ 
/*	 Fields Data	 */
#define ENET_DATA2_WIDTH                                                     32
#define ENET_DATA2_SHIFT                                                      0
#define ENET_DATA2_MASK                                              0xffffffff
#define ENET_DATA2_RD(src)                               (((src) & 0xffffffff))
#define ENET_DATA2_WR(src)                          (((u32)(src)) & 0xffffffff)
#define ENET_DATA2_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CfgSsQmiDbgData3	*/ 
/*	 Fields Data	 */
#define ENET_DATA3_WIDTH                                                     32
#define ENET_DATA3_SHIFT                                                      0
#define ENET_DATA3_MASK                                              0xffffffff
#define ENET_DATA3_RD(src)                               (((src) & 0xffffffff))
#define ENET_DATA3_WR(src)                          (((u32)(src)) & 0xffffffff)
#define ENET_DATA3_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register StsSsQmiDbgData	*/ 
/*	 Fields Data	 */
#define ENET_DATA_WIDTH                                                      32
#define ENET_DATA_SHIFT                                                       0
#define ENET_DATA_MASK                                               0xffffffff
#define ENET_DATA_RD(src)                                (((src) & 0xffffffff))
#define ENET_DATA_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CfgSsQmiLite	*/ 
/*	 Fields QMLiteDeviceAddress	 */
#define ENET_QMLITEDEVICEADDRESS_WIDTH                                       15
#define ENET_QMLITEDEVICEADDRESS_SHIFT                                        0
#define ENET_QMLITEDEVICEADDRESS_MASK                                0x00007fff
#define ENET_QMLITEDEVICEADDRESS_RD(src)                 (((src) & 0x00007fff))
#define ENET_QMLITEDEVICEADDRESS_WR(src)            (((u32)(src)) & 0x00007fff)
#define ENET_QMLITEDEVICEADDRESS_SET(dst,src) \
                          (((dst) & ~0x00007fff) | (((u32)(src)) & 0x00007fff))

/*	Register CfgSsQmiFPQAssoc	*/ 
/*	 Fields Association	 */
#define ENET_ASSOCIATION_WIDTH                                               18
#define ENET_ASSOCIATION_SHIFT                                                0
#define ENET_ASSOCIATION_MASK                                        0x0003ffff
#define ENET_ASSOCIATION_RD(src)                         (((src) & 0x0003ffff))
#define ENET_ASSOCIATION_WR(src)                    (((u32)(src)) & 0x0003ffff)
#define ENET_ASSOCIATION_SET(dst,src) \
                          (((dst) & ~0x0003ffff) | (((u32)(src)) & 0x0003ffff))

/*	Register CfgSsQmiWQAssoc	*/ 
/*	 Fields Association	 */
#define ENET_ASSOCIATION_F1_WIDTH                                            17
#define ENET_ASSOCIATION_F1_SHIFT                                             0
#define ENET_ASSOCIATION_F1_MASK                                     0x0001ffff
#define ENET_ASSOCIATION_F1_RD(src)                      (((src) & 0x0001ffff))
#define ENET_ASSOCIATION_F1_WR(src)                 (((u32)(src)) & 0x0001ffff)
#define ENET_ASSOCIATION_F1_SET(dst,src) \
                          (((dst) & ~0x0001ffff) | (((u32)(src)) & 0x0001ffff))

/*	Register CfgSsQmiMemory	*/ 
/*	 Fields RMA	 */
#define ENET_RMA_WIDTH                                                        2
#define ENET_RMA_SHIFT                                                        4
#define ENET_RMA_MASK                                                0x00000030
#define ENET_RMA_RD(src)                              (((src) & 0x00000030)>>4)
#define ENET_RMA_WR(src)                         (((u32)(src)<<4) & 0x00000030)
#define ENET_RMA_SET(dst,src) \
                       (((dst) & ~0x00000030) | (((u32)(src)<<4) & 0x00000030))
/*	 Fields RMB	 */
#define ENET_RMB_WIDTH                                                        2
#define ENET_RMB_SHIFT                                                        2
#define ENET_RMB_MASK                                                0x0000000c
#define ENET_RMB_RD(src)                              (((src) & 0x0000000c)>>2)
#define ENET_RMB_WR(src)                         (((u32)(src)<<2) & 0x0000000c)
#define ENET_RMB_SET(dst,src) \
                       (((dst) & ~0x0000000c) | (((u32)(src)<<2) & 0x0000000c))
/*	 Fields RMEA	 */
#define ENET_RMEA_WIDTH                                                       1
#define ENET_RMEA_SHIFT                                                       1
#define ENET_RMEA_MASK                                               0x00000002
#define ENET_RMEA_RD(src)                             (((src) & 0x00000002)>>1)
#define ENET_RMEA_WR(src)                        (((u32)(src)<<1) & 0x00000002)
#define ENET_RMEA_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields RMEB	 */
#define ENET_RMEB_WIDTH                                                       1
#define ENET_RMEB_SHIFT                                                       0
#define ENET_RMEB_MASK                                               0x00000001
#define ENET_RMEB_RD(src)                                (((src) & 0x00000001))
#define ENET_RMEB_WR(src)                           (((u32)(src)) & 0x00000001)
#define ENET_RMEB_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register StsSsQmiFifo	*/ 
/*	 Fields empty	 */
#define ENET_EMPTY_WIDTH                                                     27
#define ENET_EMPTY_SHIFT                                                      0
#define ENET_EMPTY_MASK                                              0x07ffffff
#define ENET_EMPTY_RD(src)                               (((src) & 0x07ffffff))
#define ENET_EMPTY_SET(dst,src) \
                          (((dst) & ~0x07ffffff) | (((u32)(src)) & 0x07ffffff))

/*	Global Base Address	*/
#define PE_MCXMAC_BASE_ADDR			0x000000000ULL

/*    Address PE_MCXMAC  Registers */
#define MAC_CONFIG_1_ADDR                                            0x00000000
#define MAC_CONFIG_1_DEFAULT                                         0x80000000
#define MAC_CONFIG_2_ADDR                                            0x00000004
#define MAC_CONFIG_2_DEFAULT                                         0x00007000
#define IPG_IFG_ADDR                                                 0x00000008
#define IPG_IFG_DEFAULT                                              0x01030a03
#define HALF_DUPLEX_ADDR                                             0x0000000c
#define HALF_DUPLEX_DEFAULT                                          0x0051f3b0
#define MAX_FRAME_LEN_ADDR                                           0x00000010
#define MAX_FRAME_LEN_DEFAULT                                        0x00000600
#define MII_MGMT_CONFIG_ADDR                                         0x00000020
#define MII_MGMT_CONFIG_DEFAULT                                      0x00000000
#define MII_MGMT_COMMAND_ADDR                                        0x00000024
#define MII_MGMT_COMMAND_DEFAULT                                     0x00000000
#define MII_MGMT_ADDRESS_ADDR                                        0x00000028
#define MII_MGMT_ADDRESS_DEFAULT                                     0x00000000
#define MII_MGMT_CONTROL_ADDR                                        0x0000002c
#define MII_MGMT_CONTROL_DEFAULT                                     0x00000000
#define MII_MGMT_STATUS_ADDR                                         0x00000030
#define MII_MGMT_STATUS_DEFAULT                                      0x00000000
#define MII_MGMT_INDICATORS_ADDR                                     0x00000034
#define MII_MGMT_INDICATORS_DEFAULT                                  0x00000000
#define INTERFACE_CONTROL_ADDR                                       0x00000038
#define INTERFACE_CONTROL_DEFAULT                                    0x00000000
#define INTERFACE_STATUS_ADDR                                        0x0000003c
#define INTERFACE_STATUS_DEFAULT                                     0x00000000
#define STATION_ADDR0_ADDR                                           0x00000040
#define STATION_ADDR0_DEFAULT                                        0x00000000
#define STATION_ADDR1_ADDR                                           0x00000044
#define STATION_ADDR1_DEFAULT                                        0x00000000

/*	Register mac_config_1	*/ 
/*	 Fields soft_reset	 */
#define SOFT_RESET1_WIDTH                                                     1
#define SOFT_RESET1_SHIFT                                                    31
#define SOFT_RESET1_MASK                                             0x80000000
#define SOFT_RESET1_RD(src)                          (((src) & 0x80000000)>>31)
#define SOFT_RESET1_WR(src)                     (((u32)(src)<<31) & 0x80000000)
#define SOFT_RESET1_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields sim_reset	 */
#define SIM_RESET1_WIDTH                                                      1
#define SIM_RESET1_SHIFT                                                     30
#define SIM_RESET1_MASK                                              0x40000000
#define SIM_RESET1_RD(src)                           (((src) & 0x40000000)>>30)
#define SIM_RESET1_WR(src)                      (((u32)(src)<<30) & 0x40000000)
#define SIM_RESET1_SET(dst,src) \
                      (((dst) & ~0x40000000) | (((u32)(src)<<30) & 0x40000000))
/*	 Fields reset_rx_mc	 */
#define RESET_RX_MC1_WIDTH                                                    1
#define RESET_RX_MC1_SHIFT                                                   19
#define RESET_RX_MC1_MASK                                            0x00080000
#define RESET_RX_MC1_RD(src)                         (((src) & 0x00080000)>>19)
#define RESET_RX_MC1_WR(src)                    (((u32)(src)<<19) & 0x00080000)
#define RESET_RX_MC1_SET(dst,src) \
                      (((dst) & ~0x00080000) | (((u32)(src)<<19) & 0x00080000))
/*	 Fields reset_tx_mc	 */
#define RESET_TX_MC1_WIDTH                                                    1
#define RESET_TX_MC1_SHIFT                                                   18
#define RESET_TX_MC1_MASK                                            0x00040000
#define RESET_TX_MC1_RD(src)                         (((src) & 0x00040000)>>18)
#define RESET_TX_MC1_WR(src)                    (((u32)(src)<<18) & 0x00040000)
#define RESET_TX_MC1_SET(dst,src) \
                      (((dst) & ~0x00040000) | (((u32)(src)<<18) & 0x00040000))
/*	 Fields reset_rx_fun	 */
#define RESET_RX_FUN1_WIDTH                                                   1
#define RESET_RX_FUN1_SHIFT                                                  17
#define RESET_RX_FUN1_MASK                                           0x00020000
#define RESET_RX_FUN1_RD(src)                        (((src) & 0x00020000)>>17)
#define RESET_RX_FUN1_WR(src)                   (((u32)(src)<<17) & 0x00020000)
#define RESET_RX_FUN1_SET(dst,src) \
                      (((dst) & ~0x00020000) | (((u32)(src)<<17) & 0x00020000))
/*	 Fields reset_tx_fun	 */
#define RESET_TX_FUN1_WIDTH                                                   1
#define RESET_TX_FUN1_SHIFT                                                  16
#define RESET_TX_FUN1_MASK                                           0x00010000
#define RESET_TX_FUN1_RD(src)                        (((src) & 0x00010000)>>16)
#define RESET_TX_FUN1_WR(src)                   (((u32)(src)<<16) & 0x00010000)
#define RESET_TX_FUN1_SET(dst,src) \
                      (((dst) & ~0x00010000) | (((u32)(src)<<16) & 0x00010000))
/*	 Fields loop_back	 */
#define LOOP_BACK1_WIDTH                                                      1
#define LOOP_BACK1_SHIFT                                                      8
#define LOOP_BACK1_MASK                                              0x00000100
#define LOOP_BACK1_RD(src)                            (((src) & 0x00000100)>>8)
#define LOOP_BACK1_WR(src)                       (((u32)(src)<<8) & 0x00000100)
#define LOOP_BACK1_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*	 Fields rx_flow_en	 */
#define RX_FLOW_EN1_WIDTH                                                     1
#define RX_FLOW_EN1_SHIFT                                                     5
#define RX_FLOW_EN1_MASK                                             0x00000020
#define RX_FLOW_EN1_RD(src)                           (((src) & 0x00000020)>>5)
#define RX_FLOW_EN1_WR(src)                      (((u32)(src)<<5) & 0x00000020)
#define RX_FLOW_EN1_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields tx_flow_en	 */
#define TX_FLOW_EN1_WIDTH                                                     1
#define TX_FLOW_EN1_SHIFT                                                     4
#define TX_FLOW_EN1_MASK                                             0x00000010
#define TX_FLOW_EN1_RD(src)                           (((src) & 0x00000010)>>4)
#define TX_FLOW_EN1_WR(src)                      (((u32)(src)<<4) & 0x00000010)
#define TX_FLOW_EN1_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields syncd_rx_enable	 */
#define SYNCD_RX_ENABLE1_WIDTH                                                1
#define SYNCD_RX_ENABLE1_SHIFT                                                3
#define SYNCD_RX_ENABLE1_MASK                                        0x00000008
#define SYNCD_RX_ENABLE1_RD(src)                      (((src) & 0x00000008)>>3)
#define SYNCD_RX_ENABLE1_WR(src)                 (((u32)(src)<<3) & 0x00000008)
#define SYNCD_RX_ENABLE1_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields rx_en	 */
#define RX_EN1_WIDTH                                                          1
#define RX_EN1_SHIFT                                                          2
#define RX_EN1_MASK                                                  0x00000004
#define RX_EN1_RD(src)                                (((src) & 0x00000004)>>2)
#define RX_EN1_WR(src)                           (((u32)(src)<<2) & 0x00000004)
#define RX_EN1_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields syncd_tx_en	 */
#define SYNCD_TX_EN1_WIDTH                                                    1
#define SYNCD_TX_EN1_SHIFT                                                    1
#define SYNCD_TX_EN1_MASK                                            0x00000002
#define SYNCD_TX_EN1_RD(src)                          (((src) & 0x00000002)>>1)
#define SYNCD_TX_EN1_WR(src)                     (((u32)(src)<<1) & 0x00000002)
#define SYNCD_TX_EN1_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields tx_en	 */
#define TX_EN1_WIDTH                                                          1
#define TX_EN1_SHIFT                                                          0
#define TX_EN1_MASK                                                  0x00000001
#define TX_EN1_RD(src)                                   (((src) & 0x00000001))
#define TX_EN1_WR(src)                              (((u32)(src)) & 0x00000001)
#define TX_EN1_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register mac_config_2	*/ 
/*	 Fields preamble_length	 */
#define PREAMBLE_LENGTH2_WIDTH                                                4
#define PREAMBLE_LENGTH2_SHIFT                                               12
#define PREAMBLE_LENGTH2_MASK                                        0x0000f000
#define PREAMBLE_LENGTH2_RD(src)                     (((src) & 0x0000f000)>>12)
#define PREAMBLE_LENGTH2_WR(src)                (((u32)(src)<<12) & 0x0000f000)
#define PREAMBLE_LENGTH2_SET(dst,src) \
                      (((dst) & ~0x0000f000) | (((u32)(src)<<12) & 0x0000f000))
/*	 Fields interface_mode	 */
#define INTERFACE_MODE2_WIDTH                                                 2
#define INTERFACE_MODE2_SHIFT                                                 8
#define INTERFACE_MODE2_MASK                                         0x00000300
#define INTERFACE_MODE2_RD(src)                       (((src) & 0x00000300)>>8)
#define INTERFACE_MODE2_WR(src)                  (((u32)(src)<<8) & 0x00000300)
#define INTERFACE_MODE2_SET(dst,src) \
                       (((dst) & ~0x00000300) | (((u32)(src)<<8) & 0x00000300))
/*	 Fields huge_frame_en	 */
#define HUGE_FRAME_EN2_WIDTH                                                  1
#define HUGE_FRAME_EN2_SHIFT                                                  5
#define HUGE_FRAME_EN2_MASK                                          0x00000020
#define HUGE_FRAME_EN2_RD(src)                        (((src) & 0x00000020)>>5)
#define HUGE_FRAME_EN2_WR(src)                   (((u32)(src)<<5) & 0x00000020)
#define HUGE_FRAME_EN2_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields length_check	 */
#define LENGTH_CHECK2_WIDTH                                                   1
#define LENGTH_CHECK2_SHIFT                                                   4
#define LENGTH_CHECK2_MASK                                           0x00000010
#define LENGTH_CHECK2_RD(src)                         (((src) & 0x00000010)>>4)
#define LENGTH_CHECK2_WR(src)                    (((u32)(src)<<4) & 0x00000010)
#define LENGTH_CHECK2_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields pad_crc	 */
#define PAD_CRC2_WIDTH                                                        1
#define PAD_CRC2_SHIFT                                                        2
#define PAD_CRC2_MASK                                                0x00000004
#define PAD_CRC2_RD(src)                              (((src) & 0x00000004)>>2)
#define PAD_CRC2_WR(src)                         (((u32)(src)<<2) & 0x00000004)
#define PAD_CRC2_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields crc_en	 */
#define CRC_EN2_WIDTH                                                         1
#define CRC_EN2_SHIFT                                                         1
#define CRC_EN2_MASK                                                 0x00000002
#define CRC_EN2_RD(src)                               (((src) & 0x00000002)>>1)
#define CRC_EN2_WR(src)                          (((u32)(src)<<1) & 0x00000002)
#define CRC_EN2_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields full_duplex	 */
#define FULL_DUPLEX2_WIDTH                                                    1
#define FULL_DUPLEX2_SHIFT                                                    0
#define FULL_DUPLEX2_MASK                                            0x00000001
#define FULL_DUPLEX2_RD(src)                             (((src) & 0x00000001))
#define FULL_DUPLEX2_WR(src)                        (((u32)(src)) & 0x00000001)
#define FULL_DUPLEX2_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register ipg_ifg	*/ 
/*	 Fields non_b2b_ipg_part1	 */
#define NON_B2B_IPG_PART1_WIDTH                                               7
#define NON_B2B_IPG_PART1_SHIFT                                              24
#define NON_B2B_IPG_PART1_MASK                                       0x7f000000
#define NON_B2B_IPG_PART1_RD(src)                    (((src) & 0x7f000000)>>24)
#define NON_B2B_IPG_PART1_WR(src)               (((u32)(src)<<24) & 0x7f000000)
#define NON_B2B_IPG_PART1_SET(dst,src) \
                      (((dst) & ~0x7f000000) | (((u32)(src)<<24) & 0x7f000000))
/*	 Fields non_b2b_ipg_part2	 */
#define NON_B2B_IPG_PART2_WIDTH                                               7
#define NON_B2B_IPG_PART2_SHIFT                                              16
#define NON_B2B_IPG_PART2_MASK                                       0x007f0000
#define NON_B2B_IPG_PART2_RD(src)                    (((src) & 0x007f0000)>>16)
#define NON_B2B_IPG_PART2_WR(src)               (((u32)(src)<<16) & 0x007f0000)
#define NON_B2B_IPG_PART2_SET(dst,src) \
                      (((dst) & ~0x007f0000) | (((u32)(src)<<16) & 0x007f0000))
/*	 Fields min_ifg_enforce	 */
#define MIN_IFG_ENFORCE_WIDTH                                                 8
#define MIN_IFG_ENFORCE_SHIFT                                                 8
#define MIN_IFG_ENFORCE_MASK                                         0x0000ff00
#define MIN_IFG_ENFORCE_RD(src)                       (((src) & 0x0000ff00)>>8)
#define MIN_IFG_ENFORCE_WR(src)                  (((u32)(src)<<8) & 0x0000ff00)
#define MIN_IFG_ENFORCE_SET(dst,src) \
                       (((dst) & ~0x0000ff00) | (((u32)(src)<<8) & 0x0000ff00))
/*	 Fields b2b_ipg	 */
#define B2B_IPG_WIDTH                                                         7
#define B2B_IPG_SHIFT                                                         0
#define B2B_IPG_MASK                                                 0x0000007f
#define B2B_IPG_RD(src)                                  (((src) & 0x0000007f))
#define B2B_IPG_WR(src)                             (((u32)(src)) & 0x0000007f)
#define B2B_IPG_SET(dst,src) \
                          (((dst) & ~0x0000007f) | (((u32)(src)) & 0x0000007f))

/*	Register half_duplex	*/ 
/*	 Fields alt_beb_trunc	 */
#define ALT_BEB_TRUNC_WIDTH                                                   4
#define ALT_BEB_TRUNC_SHIFT                                                  20
#define ALT_BEB_TRUNC_MASK                                           0x00f00000
#define ALT_BEB_TRUNC_RD(src)                        (((src) & 0x00f00000)>>20)
#define ALT_BEB_TRUNC_WR(src)                   (((u32)(src)<<20) & 0x00f00000)
#define ALT_BEB_TRUNC_SET(dst,src) \
                      (((dst) & ~0x00f00000) | (((u32)(src)<<20) & 0x00f00000))
/*	 Fields alt_beb	 */
#define ALT_BEB_WIDTH                                                         1
#define ALT_BEB_SHIFT                                                        19
#define ALT_BEB_MASK                                                 0x00080000
#define ALT_BEB_RD(src)                              (((src) & 0x00080000)>>19)
#define ALT_BEB_WR(src)                         (((u32)(src)<<19) & 0x00080000)
#define ALT_BEB_SET(dst,src) \
                      (((dst) & ~0x00080000) | (((u32)(src)<<19) & 0x00080000))
/*	 Fields bp_no_backoff	 */
#define BP_NO_BACKOFF_WIDTH                                                   1
#define BP_NO_BACKOFF_SHIFT                                                  18
#define BP_NO_BACKOFF_MASK                                           0x00040000
#define BP_NO_BACKOFF_RD(src)                        (((src) & 0x00040000)>>18)
#define BP_NO_BACKOFF_WR(src)                   (((u32)(src)<<18) & 0x00040000)
#define BP_NO_BACKOFF_SET(dst,src) \
                      (((dst) & ~0x00040000) | (((u32)(src)<<18) & 0x00040000))
/*	 Fields no_backoff	 */
#define NO_BACKOFF_WIDTH                                                      1
#define NO_BACKOFF_SHIFT                                                     17
#define NO_BACKOFF_MASK                                              0x00020000
#define NO_BACKOFF_RD(src)                           (((src) & 0x00020000)>>17)
#define NO_BACKOFF_WR(src)                      (((u32)(src)<<17) & 0x00020000)
#define NO_BACKOFF_SET(dst,src) \
                      (((dst) & ~0x00020000) | (((u32)(src)<<17) & 0x00020000))
/*	 Fields excessive_defer	 */
#define EXCESSIVE_DEFER_WIDTH                                                 1
#define EXCESSIVE_DEFER_SHIFT                                                16
#define EXCESSIVE_DEFER_MASK                                         0x00010000
#define EXCESSIVE_DEFER_RD(src)                      (((src) & 0x00010000)>>16)
#define EXCESSIVE_DEFER_WR(src)                 (((u32)(src)<<16) & 0x00010000)
#define EXCESSIVE_DEFER_SET(dst,src) \
                      (((dst) & ~0x00010000) | (((u32)(src)<<16) & 0x00010000))
/*	 Fields retrans_max	 */
#define RETRANS_MAX_WIDTH                                                     4
#define RETRANS_MAX_SHIFT                                                    12
#define RETRANS_MAX_MASK                                             0x0000f000
#define RETRANS_MAX_RD(src)                          (((src) & 0x0000f000)>>12)
#define RETRANS_MAX_WR(src)                     (((u32)(src)<<12) & 0x0000f000)
#define RETRANS_MAX_SET(dst,src) \
                      (((dst) & ~0x0000f000) | (((u32)(src)<<12) & 0x0000f000))
/*	 Fields col_window	 */
#define COL_WINDOW_WIDTH                                                     10
#define COL_WINDOW_SHIFT                                                      0
#define COL_WINDOW_MASK                                              0x000003ff
#define COL_WINDOW_RD(src)                               (((src) & 0x000003ff))
#define COL_WINDOW_WR(src)                          (((u32)(src)) & 0x000003ff)
#define COL_WINDOW_SET(dst,src) \
                          (((dst) & ~0x000003ff) | (((u32)(src)) & 0x000003ff))

/*	Register max_frame_len	*/ 
/*	 Fields max_frame_len	 */
#define MAX_FRAME_LEN_WIDTH                                                  16
#define MAX_FRAME_LEN_SHIFT                                                   0
#define MAX_FRAME_LEN_MASK                                           0x0000ffff
#define MAX_FRAME_LEN_RD(src)                            (((src) & 0x0000ffff))
#define MAX_FRAME_LEN_WR(src)                       (((u32)(src)) & 0x0000ffff)
#define MAX_FRAME_LEN_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register mii_mgmt_config	*/ 
/*	 Fields reset_mii_mgmt	 */
#define RESET_MII_MGMT_WIDTH                                                  1
#define RESET_MII_MGMT_SHIFT                                                 31
#define RESET_MII_MGMT_MASK                                          0x80000000
#define RESET_MII_MGMT_RD(src)                       (((src) & 0x80000000)>>31)
#define RESET_MII_MGMT_WR(src)                  (((u32)(src)<<31) & 0x80000000)
#define RESET_MII_MGMT_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields scan_auto_incr	 */
#define SCAN_AUTO_INCR_WIDTH                                                  1
#define SCAN_AUTO_INCR_SHIFT                                                  5
#define SCAN_AUTO_INCR_MASK                                          0x00000020
#define SCAN_AUTO_INCR_RD(src)                        (((src) & 0x00000020)>>5)
#define SCAN_AUTO_INCR_WR(src)                   (((u32)(src)<<5) & 0x00000020)
#define SCAN_AUTO_INCR_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields PREAMBLE_SUPPRESSION	 */
#define PREAMBLE_SUPPRESSION_WIDTH                                            1
#define PREAMBLE_SUPPRESSION_SHIFT                                            4
#define PREAMBLE_SUPPRESSION_MASK                                    0x00000010
#define PREAMBLE_SUPPRESSION_RD(src)                  (((src) & 0x00000010)>>4)
#define PREAMBLE_SUPPRESSION_WR(src)             (((u32)(src)<<4) & 0x00000010)
#define PREAMBLE_SUPPRESSION_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields mgmt_clock_sel	 */
#define MGMT_CLOCK_SEL_WIDTH                                                  3
#define MGMT_CLOCK_SEL_SHIFT                                                  0
#define MGMT_CLOCK_SEL_MASK                                          0x00000007
#define MGMT_CLOCK_SEL_RD(src)                           (((src) & 0x00000007))
#define MGMT_CLOCK_SEL_WR(src)                      (((u32)(src)) & 0x00000007)
#define MGMT_CLOCK_SEL_SET(dst,src) \
                          (((dst) & ~0x00000007) | (((u32)(src)) & 0x00000007))

/*	Register mii_mgmt_command	*/ 
/*	 Fields scan_cycle	 */
#define SCAN_CYCLE_WIDTH                                                      1
#define SCAN_CYCLE_SHIFT                                                      1
#define SCAN_CYCLE_MASK                                              0x00000002
#define SCAN_CYCLE_RD(src)                            (((src) & 0x00000002)>>1)
#define SCAN_CYCLE_WR(src)                       (((u32)(src)<<1) & 0x00000002)
#define SCAN_CYCLE_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields read_cycle	 */
#define READ_CYCLE_WIDTH                                                      1
#define READ_CYCLE_SHIFT                                                      0
#define READ_CYCLE_MASK                                              0x00000001
#define READ_CYCLE_RD(src)                               (((src) & 0x00000001))
#define READ_CYCLE_WR(src)                          (((u32)(src)) & 0x00000001)
#define READ_CYCLE_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register mii_mgmt_address	*/ 
/*	 Fields phy_addr	 */
#define PHY_ADDR_WIDTH                                                        5
#define PHY_ADDR_SHIFT                                                        8
#define PHY_ADDR_MASK                                                0x00001f00
#define PHY_ADDR_RD(src)                              (((src) & 0x00001f00)>>8)
#define PHY_ADDR_WR(src)                         (((u32)(src)<<8) & 0x00001f00)
#define PHY_ADDR_SET(dst,src) \
                       (((dst) & ~0x00001f00) | (((u32)(src)<<8) & 0x00001f00))
/*	 Fields reg_addr	 */
#define REG_ADDR_WIDTH                                                        5
#define REG_ADDR_SHIFT                                                        0
#define REG_ADDR_MASK                                                0x0000001f
#define REG_ADDR_RD(src)                                 (((src) & 0x0000001f))
#define REG_ADDR_WR(src)                            (((u32)(src)) & 0x0000001f)
#define REG_ADDR_SET(dst,src) \
                          (((dst) & ~0x0000001f) | (((u32)(src)) & 0x0000001f))

/*	Register mii_mgmt_control	*/ 
/*	 Fields phy_control	 */
#define PHY_CONTROL_WIDTH                                                    16
#define PHY_CONTROL_SHIFT                                                     0
#define PHY_CONTROL_MASK                                             0x0000ffff
#define PHY_CONTROL_RD(src)                              (((src) & 0x0000ffff))
#define PHY_CONTROL_WR(src)                         (((u32)(src)) & 0x0000ffff)
#define PHY_CONTROL_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register mii_mgmt_status	*/ 
/*	 Fields phy_status	 */
#define PHY_STATUS_WIDTH                                                     16
#define PHY_STATUS_SHIFT                                                      0
#define PHY_STATUS_MASK                                              0x0000ffff
#define PHY_STATUS_RD(src)                               (((src) & 0x0000ffff))
#define PHY_STATUS_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register mii_mgmt_indicators	*/ 
/*	 Fields not_valid	 */
#define NOT_VALID_WIDTH                                                       1
#define NOT_VALID_SHIFT                                                       2
#define NOT_VALID_MASK                                               0x00000004
#define NOT_VALID_RD(src)                             (((src) & 0x00000004)>>2)
#define NOT_VALID_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields scanning	 */
#define SCANNING_WIDTH                                                        1
#define SCANNING_SHIFT                                                        1
#define SCANNING_MASK                                                0x00000002
#define SCANNING_RD(src)                              (((src) & 0x00000002)>>1)
#define SCANNING_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields busy	 */
#define BUSY_F14_WIDTH                                                        1
#define BUSY_F14_SHIFT                                                        0
#define BUSY_F14_MASK                                                0x00000001
#define BUSY_F14_RD(src)                                 (((src) & 0x00000001))
#define BUSY_F14_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register interface_control	*/ 
/*	 Fields reset_int	 */
#define RESET_INT_WIDTH                                                       1
#define RESET_INT_SHIFT                                                      31
#define RESET_INT_MASK                                               0x80000000
#define RESET_INT_RD(src)                            (((src) & 0x80000000)>>31)
#define RESET_INT_WR(src)                       (((u32)(src)<<31) & 0x80000000)
#define RESET_INT_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields tbi_mode	 */
#define TBI_MODE_WIDTH                                                        1
#define TBI_MODE_SHIFT                                                       27
#define TBI_MODE_MASK                                                0x08000000
#define TBI_MODE_RD(src)                             (((src) & 0x08000000)>>27)
#define TBI_MODE_WR(src)                        (((u32)(src)<<27) & 0x08000000)
#define TBI_MODE_SET(dst,src) \
                      (((dst) & ~0x08000000) | (((u32)(src)<<27) & 0x08000000))
/*	 Fields ghd_mode	 */
#define GHD_MODE_WIDTH                                                        1
#define GHD_MODE_SHIFT                                                       26
#define GHD_MODE_MASK                                                0x04000000
#define GHD_MODE_RD(src)                             (((src) & 0x04000000)>>26)
#define GHD_MODE_WR(src)                        (((u32)(src)<<26) & 0x04000000)
#define GHD_MODE_SET(dst,src) \
                      (((dst) & ~0x04000000) | (((u32)(src)<<26) & 0x04000000))
/*	 Fields lhd_mode	 */
#define LHD_MODE_WIDTH                                                        1
#define LHD_MODE_SHIFT                                                       25
#define LHD_MODE_MASK                                                0x02000000
#define LHD_MODE_RD(src)                             (((src) & 0x02000000)>>25)
#define LHD_MODE_WR(src)                        (((u32)(src)<<25) & 0x02000000)
#define LHD_MODE_SET(dst,src) \
                      (((dst) & ~0x02000000) | (((u32)(src)<<25) & 0x02000000))
/*	 Fields phy_mode	 */
#define PHY_MODE_WIDTH                                                        1
#define PHY_MODE_SHIFT                                                       24
#define PHY_MODE_MASK                                                0x01000000
#define PHY_MODE_RD(src)                             (((src) & 0x01000000)>>24)
#define PHY_MODE_WR(src)                        (((u32)(src)<<24) & 0x01000000)
#define PHY_MODE_SET(dst,src) \
                      (((dst) & ~0x01000000) | (((u32)(src)<<24) & 0x01000000))
/*	 Fields reset_permii	 */
#define RESET_PERMII_WIDTH                                                    1
#define RESET_PERMII_SHIFT                                                   23
#define RESET_PERMII_MASK                                            0x00800000
#define RESET_PERMII_RD(src)                         (((src) & 0x00800000)>>23)
#define RESET_PERMII_WR(src)                    (((u32)(src)<<23) & 0x00800000)
#define RESET_PERMII_SET(dst,src) \
                      (((dst) & ~0x00800000) | (((u32)(src)<<23) & 0x00800000))
/*	 Fields speed	 */
#define SPEED_WIDTH                                                           1
#define SPEED_SHIFT                                                          16
#define SPEED_MASK                                                   0x00010000
#define SPEED_RD(src)                                (((src) & 0x00010000)>>16)
#define SPEED_WR(src)                           (((u32)(src)<<16) & 0x00010000)
#define SPEED_SET(dst,src) \
                      (((dst) & ~0x00010000) | (((u32)(src)<<16) & 0x00010000))
/*	 Fields reset_pe100x	 */
#define RESET_PE100X_WIDTH                                                    1
#define RESET_PE100X_SHIFT                                                   15
#define RESET_PE100X_MASK                                            0x00008000
#define RESET_PE100X_RD(src)                         (((src) & 0x00008000)>>15)
#define RESET_PE100X_WR(src)                    (((u32)(src)<<15) & 0x00008000)
#define RESET_PE100X_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))
/*	 Fields force_quiet	 */
#define FORCE_QUIET_WIDTH                                                     1
#define FORCE_QUIET_SHIFT                                                    10
#define FORCE_QUIET_MASK                                             0x00000400
#define FORCE_QUIET_RD(src)                          (((src) & 0x00000400)>>10)
#define FORCE_QUIET_WR(src)                     (((u32)(src)<<10) & 0x00000400)
#define FORCE_QUIET_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields no_cipher	 */
#define NO_CIPHER_WIDTH                                                       1
#define NO_CIPHER_SHIFT                                                       9
#define NO_CIPHER_MASK                                               0x00000200
#define NO_CIPHER_RD(src)                             (((src) & 0x00000200)>>9)
#define NO_CIPHER_WR(src)                        (((u32)(src)<<9) & 0x00000200)
#define NO_CIPHER_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields disable_link_fail	 */
#define DISABLE_LINK_FAIL_WIDTH                                               1
#define DISABLE_LINK_FAIL_SHIFT                                               8
#define DISABLE_LINK_FAIL_MASK                                       0x00000100
#define DISABLE_LINK_FAIL_RD(src)                     (((src) & 0x00000100)>>8)
#define DISABLE_LINK_FAIL_WR(src)                (((u32)(src)<<8) & 0x00000100)
#define DISABLE_LINK_FAIL_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*	 Fields reset_gpsi	 */
#define RESET_GPSI_WIDTH                                                      1
#define RESET_GPSI_SHIFT                                                      7
#define RESET_GPSI_MASK                                              0x00000080
#define RESET_GPSI_RD(src)                            (((src) & 0x00000080)>>7)
#define RESET_GPSI_WR(src)                       (((u32)(src)<<7) & 0x00000080)
#define RESET_GPSI_SET(dst,src) \
                       (((dst) & ~0x00000080) | (((u32)(src)<<7) & 0x00000080))
/*	 Fields en_jabber	 */
#define EN_JABBER_WIDTH                                                       1
#define EN_JABBER_SHIFT                                                       0
#define EN_JABBER_MASK                                               0x00000001
#define EN_JABBER_RD(src)                                (((src) & 0x00000001))
#define EN_JABBER_WR(src)                           (((u32)(src)) & 0x00000001)
#define EN_JABBER_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register interface_status	*/ 
/*	 Fields excess_defer	 */
#define EXCESS_DEFER_WIDTH                                                    1
#define EXCESS_DEFER_SHIFT                                                    9
#define EXCESS_DEFER_MASK                                            0x00000200
#define EXCESS_DEFER_RD(src)                          (((src) & 0x00000200)>>9)
#define EXCESS_DEFER_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields clash	 */
#define CLASH_WIDTH                                                           1
#define CLASH_SHIFT                                                           8
#define CLASH_MASK                                                   0x00000100
#define CLASH_RD(src)                                 (((src) & 0x00000100)>>8)
#define CLASH_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*	 Fields mii_jabber	 */
#define MII_JABBER_WIDTH                                                      1
#define MII_JABBER_SHIFT                                                      7
#define MII_JABBER_MASK                                              0x00000080
#define MII_JABBER_RD(src)                            (((src) & 0x00000080)>>7)
#define MII_JABBER_SET(dst,src) \
                       (((dst) & ~0x00000080) | (((u32)(src)<<7) & 0x00000080))
/*	 Fields link_ok	 */
#define LINK_OK_WIDTH                                                         1
#define LINK_OK_SHIFT                                                         6
#define LINK_OK_MASK                                                 0x00000040
#define LINK_OK_RD(src)                               (((src) & 0x00000040)>>6)
#define LINK_OK_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*	 Fields full_duplex	 */
#define FULL_DUPLEX_WIDTH                                                     1
#define FULL_DUPLEX_SHIFT                                                     5
#define FULL_DUPLEX_MASK                                             0x00000020
#define FULL_DUPLEX_RD(src)                           (((src) & 0x00000020)>>5)
#define FULL_DUPLEX_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields speed	 */
#define SPEED_F1_WIDTH                                                        1
#define SPEED_F1_SHIFT                                                        4
#define SPEED_F1_MASK                                                0x00000010
#define SPEED_F1_RD(src)                              (((src) & 0x00000010)>>4)
#define SPEED_F1_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields link_fail	 */
#define LINK_FAIL_WIDTH                                                       1
#define LINK_FAIL_SHIFT                                                       3
#define LINK_FAIL_MASK                                               0x00000008
#define LINK_FAIL_RD(src)                             (((src) & 0x00000008)>>3)
#define LINK_FAIL_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields loss_of_carrier	 */
#define LOSS_OF_CARRIER_WIDTH                                                 1
#define LOSS_OF_CARRIER_SHIFT                                                 2
#define LOSS_OF_CARRIER_MASK                                         0x00000004
#define LOSS_OF_CARRIER_RD(src)                       (((src) & 0x00000004)>>2)
#define LOSS_OF_CARRIER_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields sqe_error	 */
#define SQE_ERROR_WIDTH                                                       1
#define SQE_ERROR_SHIFT                                                       1
#define SQE_ERROR_MASK                                               0x00000002
#define SQE_ERROR_RD(src)                             (((src) & 0x00000002)>>1)
#define SQE_ERROR_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields jabber	 */
#define JABBER_WIDTH                                                          1
#define JABBER_SHIFT                                                          0
#define JABBER_MASK                                                  0x00000001
#define JABBER_RD(src)                                   (((src) & 0x00000001))
#define JABBER_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register station_addr0	*/ 
/*	 Fields station_oct0	 */
#define STATION_OCT00_WIDTH                                                   8
#define STATION_OCT00_SHIFT                                                  24
#define STATION_OCT00_MASK                                           0xff000000
#define STATION_OCT00_RD(src)                        (((src) & 0xff000000)>>24)
#define STATION_OCT00_WR(src)                   (((u32)(src)<<24) & 0xff000000)
#define STATION_OCT00_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))
/*	 Fields station_oct1	 */
#define STATION_OCT10_WIDTH                                                   8
#define STATION_OCT10_SHIFT                                                  16
#define STATION_OCT10_MASK                                           0x00ff0000
#define STATION_OCT10_RD(src)                        (((src) & 0x00ff0000)>>16)
#define STATION_OCT10_WR(src)                   (((u32)(src)<<16) & 0x00ff0000)
#define STATION_OCT10_SET(dst,src) \
                      (((dst) & ~0x00ff0000) | (((u32)(src)<<16) & 0x00ff0000))
/*	 Fields station_oct2	 */
#define STATION_OCT20_WIDTH                                                   8
#define STATION_OCT20_SHIFT                                                   8
#define STATION_OCT20_MASK                                           0x0000ff00
#define STATION_OCT20_RD(src)                         (((src) & 0x0000ff00)>>8)
#define STATION_OCT20_WR(src)                    (((u32)(src)<<8) & 0x0000ff00)
#define STATION_OCT20_SET(dst,src) \
                       (((dst) & ~0x0000ff00) | (((u32)(src)<<8) & 0x0000ff00))
/*	 Fields station_oct3	 */
#define STATION_OCT30_WIDTH                                                   8
#define STATION_OCT30_SHIFT                                                   0
#define STATION_OCT30_MASK                                           0x000000ff
#define STATION_OCT30_RD(src)                            (((src) & 0x000000ff))
#define STATION_OCT30_WR(src)                       (((u32)(src)) & 0x000000ff)
#define STATION_OCT30_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register station_addr1	*/ 
/*	 Fields station_oct	 */
#define STATION_OCT1_WIDTH                                                    8
#define STATION_OCT1_SHIFT                                                   24
#define STATION_OCT1_MASK                                            0xff000000
#define STATION_OCT1_RD(src)                         (((src) & 0xff000000)>>24)
#define STATION_OCT1_WR(src)                    (((u32)(src)<<24) & 0xff000000)
#define STATION_OCT1_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))
/*	 Fields station_oct1	 */
#define STATION_OCT11_WIDTH                                                   8
#define STATION_OCT11_SHIFT                                                  16
#define STATION_OCT11_MASK                                           0x00ff0000
#define STATION_OCT11_RD(src)                        (((src) & 0x00ff0000)>>16)
#define STATION_OCT11_WR(src)                   (((u32)(src)<<16) & 0x00ff0000)
#define STATION_OCT11_SET(dst,src) \
                      (((dst) & ~0x00ff0000) | (((u32)(src)<<16) & 0x00ff0000))

/*	Global Base Address	*/
#define PEMSTAT_BASE_ADDR			0x000000000ULL

/*    Address PEMSTAT  Registers */
#define TR64_ADDR                                                    0x00000020
#define TR64_DEFAULT                                                 0x00000000
#define TR127_ADDR                                                   0x00000021
#define TR127_DEFAULT                                                0x00000000
#define TR255_ADDR                                                   0x00000022
#define TR255_DEFAULT                                                0x00000000
#define TR511_ADDR                                                   0x00000023
#define TR511_DEFAULT                                                0x00000000
#define TR1K_ADDR                                                    0x00000024
#define TR1K_DEFAULT                                                 0x00000000
#define TRMAX_ADDR                                                   0x00000025
#define TRMAX_DEFAULT                                                0x00000000
#define TRMGV_ADDR                                                   0x00000026
#define TRMGV_DEFAULT                                                0x00000000
#define RBYT_ADDR                                                    0x00000027
#define RBYT_DEFAULT                                                 0x00000000
#define RPKT_ADDR                                                    0x00000028
#define RPKT_DEFAULT                                                 0x00000000
#define RFCS_ADDR                                                    0x00000029
#define RFCS_DEFAULT                                                 0x00000000
#define RMCA_ADDR                                                    0x0000002a
#define RMCA_DEFAULT                                                 0x00000000
#define RBCA_ADDR                                                    0x0000002b
#define RBCA_DEFAULT                                                 0x00000000
#define RXCF_ADDR                                                    0x0000002c
#define RXCF_DEFAULT                                                 0x00000000
#define RXPF_ADDR                                                    0x0000002d
#define RXPF_DEFAULT                                                 0x00000000
#define RXUO_ADDR                                                    0x0000002e
#define RXUO_DEFAULT                                                 0x00000000
#define RALN_ADDR                                                    0x0000002f
#define RALN_DEFAULT                                                 0x00000000
#define RFLR_ADDR                                                    0x00000030
#define RFLR_DEFAULT                                                 0x00000000
#define RCDE_ADDR                                                    0x00000031
#define RCDE_DEFAULT                                                 0x00000000
#define RCSE_ADDR                                                    0x00000032
#define RCSE_DEFAULT                                                 0x00000000
#define RUND_ADDR                                                    0x00000033
#define RUND_DEFAULT                                                 0x00000000
#define ROVR_ADDR                                                    0x00000034
#define ROVR_DEFAULT                                                 0x00000000
#define RFRG_ADDR                                                    0x00000035
#define RFRG_DEFAULT                                                 0x00000000
#define RJBR_ADDR                                                    0x00000036
#define RJBR_DEFAULT                                                 0x00000000
#define RDRP_ADDR                                                    0x00000037
#define RDRP_DEFAULT                                                 0x00000000
#define TBYT_ADDR                                                    0x00000038
#define TBYT_DEFAULT                                                 0x00000000
#define TPKT_ADDR                                                    0x00000039
#define TPKT_DEFAULT                                                 0x00000000
#define TMCA_ADDR                                                    0x0000003a
#define TMCA_DEFAULT                                                 0x00000000
#define TBCA_ADDR                                                    0x0000003b
#define TBCA_DEFAULT                                                 0x00000000
#define TXPF_ADDR                                                    0x0000003c
#define TXPF_DEFAULT                                                 0x00000000
#define TDFR_ADDR                                                    0x0000003d
#define TDFR_DEFAULT                                                 0x00000000
#define TEDF_ADDR                                                    0x0000003e
#define TEDF_DEFAULT                                                 0x00000000
#define TSCL_ADDR                                                    0x0000003f
#define TSCL_DEFAULT                                                 0x00000000
#define TMCL_ADDR                                                    0x00000040
#define TMCL_DEFAULT                                                 0x00000000
#define TLCL_ADDR                                                    0x00000041
#define TLCL_DEFAULT                                                 0x00000000
#define TXCL_ADDR                                                    0x00000042
#define TXCL_DEFAULT                                                 0x00000000
#define TNCL_ADDR                                                    0x00000043
#define TNCL_DEFAULT                                                 0x00000000
#define TPFH_ADDR                                                    0x00000044
#define TPFH_DEFAULT                                                 0x00000000
#define TDRP_ADDR                                                    0x00000045
#define TDRP_DEFAULT                                                 0x00000000
#define TJBR_ADDR                                                    0x00000046
#define TJBR_DEFAULT                                                 0x00000000
#define TFCS_ADDR                                                    0x00000047
#define TFCS_DEFAULT                                                 0x00000000
#define TXCF_ADDR                                                    0x00000048
#define TXCF_DEFAULT                                                 0x00000000
#define TOVR_ADDR                                                    0x00000049
#define TOVR_DEFAULT                                                 0x00000000
#define TUND_ADDR                                                    0x0000004a
#define TUND_DEFAULT                                                 0x00000000
#define TFRG_ADDR                                                    0x0000004b
#define TFRG_DEFAULT                                                 0x00000000
#define CAR1_ADDR                                                    0x0000004c
#define CAR1_DEFAULT                                                 0x00000000
#define CAR2_ADDR                                                    0x0000004d
#define CAR2_DEFAULT                                                 0x00000000
#define CAM1_ADDR                                                    0x0000004e
#define CAM1_DEFAULT                                                 0xfe01ffff
#define CAM2_ADDR                                                    0x0000004f
#define CAM2_DEFAULT                                                 0x000fffff

/*	Register TR64	*/ 
/*	 Fields tx_rx_64b_frame_cntr	 */
#define TX_RX_64B_FRAME_CNTR4_WIDTH                                          31
#define TX_RX_64B_FRAME_CNTR4_SHIFT                                           0
#define TX_RX_64B_FRAME_CNTR4_MASK                                   0x7fffffff
#define TX_RX_64B_FRAME_CNTR4_RD(src)                    (((src) & 0x7fffffff))
#define TX_RX_64B_FRAME_CNTR4_SET(dst,src) \
                          (((dst) & ~0x7fffffff) | (((u32)(src)) & 0x7fffffff))

/*	Register TR127	*/ 
/*	 Fields tx_rx_127b_frame_cntr	 */
#define TX_RX_127B_FRAME_CNTR7_WIDTH                                         31
#define TX_RX_127B_FRAME_CNTR7_SHIFT                                          0
#define TX_RX_127B_FRAME_CNTR7_MASK                                  0x7fffffff
#define TX_RX_127B_FRAME_CNTR7_RD(src)                   (((src) & 0x7fffffff))
#define TX_RX_127B_FRAME_CNTR7_SET(dst,src) \
                          (((dst) & ~0x7fffffff) | (((u32)(src)) & 0x7fffffff))

/*	Register TR255	*/ 
/*	 Fields tx_rx_255b_frame_cntr	 */
#define TX_RX_255B_FRAME_CNTR5_WIDTH                                         31
#define TX_RX_255B_FRAME_CNTR5_SHIFT                                          0
#define TX_RX_255B_FRAME_CNTR5_MASK                                  0x7fffffff
#define TX_RX_255B_FRAME_CNTR5_RD(src)                   (((src) & 0x7fffffff))
#define TX_RX_255B_FRAME_CNTR5_SET(dst,src) \
                          (((dst) & ~0x7fffffff) | (((u32)(src)) & 0x7fffffff))

/*	Register TR511	*/ 
/*	 Fields tx_rx_511b_frame_cntr	 */
#define TX_RX_511B_FRAME_CNTR1_WIDTH                                         31
#define TX_RX_511B_FRAME_CNTR1_SHIFT                                          0
#define TX_RX_511B_FRAME_CNTR1_MASK                                  0x7fffffff
#define TX_RX_511B_FRAME_CNTR1_RD(src)                   (((src) & 0x7fffffff))
#define TX_RX_511B_FRAME_CNTR1_SET(dst,src) \
                          (((dst) & ~0x7fffffff) | (((u32)(src)) & 0x7fffffff))

/*	Register TR1K	*/ 
/*	 Fields tx_rx_1Kb_frame_cntr	 */
#define TX_RX_1KB_FRAME_CNTR_WIDTH                                           31
#define TX_RX_1KB_FRAME_CNTR_SHIFT                                            0
#define TX_RX_1KB_FRAME_CNTR_MASK                                    0x7fffffff
#define TX_RX_1KB_FRAME_CNTR_RD(src)                     (((src) & 0x7fffffff))
#define TX_RX_1KB_FRAME_CNTR_SET(dst,src) \
                          (((dst) & ~0x7fffffff) | (((u32)(src)) & 0x7fffffff))

/*	Register TRMAX	*/ 
/*	 Fields tx_rx_maxb_frame_cntr	 */
#define TX_RX_MAXB_FRAME_CNTR_WIDTH                                          31
#define TX_RX_MAXB_FRAME_CNTR_SHIFT                                           0
#define TX_RX_MAXB_FRAME_CNTR_MASK                                   0x7fffffff
#define TX_RX_MAXB_FRAME_CNTR_RD(src)                    (((src) & 0x7fffffff))
#define TX_RX_MAXB_FRAME_CNTR_SET(dst,src) \
                          (((dst) & ~0x7fffffff) | (((u32)(src)) & 0x7fffffff))

/*	Register TRMGV	*/ 
/*	 Fields trmgv	 */
#define TRMGV_WIDTH                                                          31
#define TRMGV_SHIFT                                                           0
#define TRMGV_MASK                                                   0x7fffffff
#define TRMGV_RD(src)                                    (((src) & 0x7fffffff))
#define TRMGV_SET(dst,src) \
                          (((dst) & ~0x7fffffff) | (((u32)(src)) & 0x7fffffff))

/*	Register RBYT	*/ 
/*	 Fields rx_byte_cntr	 */
#define RX_BYTE_CNTR_WIDTH                                                   31
#define RX_BYTE_CNTR_SHIFT                                                    0
#define RX_BYTE_CNTR_MASK                                            0x7fffffff
#define RX_BYTE_CNTR_RD(src)                             (((src) & 0x7fffffff))
#define RX_BYTE_CNTR_SET(dst,src) \
                          (((dst) & ~0x7fffffff) | (((u32)(src)) & 0x7fffffff))

/*	Register RPKT	*/ 
/*	 Fields rx_pkt_cntr	 */
#define RX_PKT_CNTR_WIDTH                                                    31
#define RX_PKT_CNTR_SHIFT                                                     0
#define RX_PKT_CNTR_MASK                                             0x7fffffff
#define RX_PKT_CNTR_RD(src)                              (((src) & 0x7fffffff))
#define RX_PKT_CNTR_SET(dst,src) \
                          (((dst) & ~0x7fffffff) | (((u32)(src)) & 0x7fffffff))

/*	Register RFCS	*/ 
/*	 Fields rx_fcs_error_cntr	 */
#define RX_FCS_ERROR_CNTR_WIDTH                                              16
#define RX_FCS_ERROR_CNTR_SHIFT                                               0
#define RX_FCS_ERROR_CNTR_MASK                                       0x0000ffff
#define RX_FCS_ERROR_CNTR_RD(src)                        (((src) & 0x0000ffff))
#define RX_FCS_ERROR_CNTR_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register RMCA	*/ 
/*	 Fields rx_mc_pkt_cntr	 */
#define RX_MC_PKT_CNTR_WIDTH                                                 31
#define RX_MC_PKT_CNTR_SHIFT                                                  0
#define RX_MC_PKT_CNTR_MASK                                          0x7fffffff
#define RX_MC_PKT_CNTR_RD(src)                           (((src) & 0x7fffffff))
#define RX_MC_PKT_CNTR_SET(dst,src) \
                          (((dst) & ~0x7fffffff) | (((u32)(src)) & 0x7fffffff))

/*	Register RBCA	*/ 
/*	 Fields rx_bc_pkt_cntr	 */
#define RX_BC_PKT_CNTR_WIDTH                                                 31
#define RX_BC_PKT_CNTR_SHIFT                                                  0
#define RX_BC_PKT_CNTR_MASK                                          0x7fffffff
#define RX_BC_PKT_CNTR_RD(src)                           (((src) & 0x7fffffff))
#define RX_BC_PKT_CNTR_SET(dst,src) \
                          (((dst) & ~0x7fffffff) | (((u32)(src)) & 0x7fffffff))

/*	Register RXCF	*/ 
/*	 Fields rx_ctrl_pkt_cntr	 */
#define RX_CTRL_PKT_CNTR_WIDTH                                               16
#define RX_CTRL_PKT_CNTR_SHIFT                                                0
#define RX_CTRL_PKT_CNTR_MASK                                        0x0000ffff
#define RX_CTRL_PKT_CNTR_RD(src)                         (((src) & 0x0000ffff))
#define RX_CTRL_PKT_CNTR_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register RXPF	*/ 
/*	 Fields rx_pause_pkt_cntr	 */
#define RX_PAUSE_PKT_CNTR_WIDTH                                              16
#define RX_PAUSE_PKT_CNTR_SHIFT                                               0
#define RX_PAUSE_PKT_CNTR_MASK                                       0x0000ffff
#define RX_PAUSE_PKT_CNTR_RD(src)                        (((src) & 0x0000ffff))
#define RX_PAUSE_PKT_CNTR_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register RXUO	*/ 
/*	 Fields rx_unk_opcode_cntr	 */
#define RX_UNK_OPCODE_CNTR_WIDTH                                             16
#define RX_UNK_OPCODE_CNTR_SHIFT                                              0
#define RX_UNK_OPCODE_CNTR_MASK                                      0x0000ffff
#define RX_UNK_OPCODE_CNTR_RD(src)                       (((src) & 0x0000ffff))
#define RX_UNK_OPCODE_CNTR_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register RALN	*/ 
/*	 Fields rx_align_err_cntr	 */
#define RX_ALIGN_ERR_CNTR_WIDTH                                              16
#define RX_ALIGN_ERR_CNTR_SHIFT                                               0
#define RX_ALIGN_ERR_CNTR_MASK                                       0x0000ffff
#define RX_ALIGN_ERR_CNTR_RD(src)                        (((src) & 0x0000ffff))
#define RX_ALIGN_ERR_CNTR_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register RFLR	*/ 
/*	 Fields rx_len_err_cntr	 */
#define RX_LEN_ERR_CNTR_WIDTH                                                16
#define RX_LEN_ERR_CNTR_SHIFT                                                 0
#define RX_LEN_ERR_CNTR_MASK                                         0x0000ffff
#define RX_LEN_ERR_CNTR_RD(src)                          (((src) & 0x0000ffff))
#define RX_LEN_ERR_CNTR_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register RCDE	*/ 
/*	 Fields rx_code_err_cntr	 */
#define RX_CODE_ERR_CNTR_WIDTH                                               16
#define RX_CODE_ERR_CNTR_SHIFT                                                0
#define RX_CODE_ERR_CNTR_MASK                                        0x0000ffff
#define RX_CODE_ERR_CNTR_RD(src)                         (((src) & 0x0000ffff))
#define RX_CODE_ERR_CNTR_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register RCSE	*/ 
/*	 Fields rx_false_carrier_cntr	 */
#define RX_FALSE_CARRIER_CNTR_WIDTH                                          16
#define RX_FALSE_CARRIER_CNTR_SHIFT                                           0
#define RX_FALSE_CARRIER_CNTR_MASK                                   0x0000ffff
#define RX_FALSE_CARRIER_CNTR_RD(src)                    (((src) & 0x0000ffff))
#define RX_FALSE_CARRIER_CNTR_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register RUND	*/ 
/*	 Fields rx_undrsize_pkt_cntr	 */
#define RX_UNDRSIZE_PKT_CNTR_WIDTH                                           16
#define RX_UNDRSIZE_PKT_CNTR_SHIFT                                            0
#define RX_UNDRSIZE_PKT_CNTR_MASK                                    0x0000ffff
#define RX_UNDRSIZE_PKT_CNTR_RD(src)                     (((src) & 0x0000ffff))
#define RX_UNDRSIZE_PKT_CNTR_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register ROVR	*/ 
/*	 Fields rx_ovrsize_pkt_cntr	 */
#define RX_OVRSIZE_PKT_CNTR_WIDTH                                            16
#define RX_OVRSIZE_PKT_CNTR_SHIFT                                             0
#define RX_OVRSIZE_PKT_CNTR_MASK                                     0x0000ffff
#define RX_OVRSIZE_PKT_CNTR_RD(src)                      (((src) & 0x0000ffff))
#define RX_OVRSIZE_PKT_CNTR_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register RFRG	*/ 
/*	 Fields rx_frag_cntr	 */
#define RX_FRAG_CNTR_WIDTH                                                   16
#define RX_FRAG_CNTR_SHIFT                                                    0
#define RX_FRAG_CNTR_MASK                                            0x0000ffff
#define RX_FRAG_CNTR_RD(src)                             (((src) & 0x0000ffff))
#define RX_FRAG_CNTR_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register RJBR	*/ 
/*	 Fields rx_jabber_cntr	 */
#define RX_JABBER_CNTR_WIDTH                                                 16
#define RX_JABBER_CNTR_SHIFT                                                  0
#define RX_JABBER_CNTR_MASK                                          0x0000ffff
#define RX_JABBER_CNTR_RD(src)                           (((src) & 0x0000ffff))
#define RX_JABBER_CNTR_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register RDRP	*/ 
/*	 Fields rx_dropped_pkt_cntr	 */
#define RX_DROPPED_PKT_CNTR_WIDTH                                            16
#define RX_DROPPED_PKT_CNTR_SHIFT                                             0
#define RX_DROPPED_PKT_CNTR_MASK                                     0x0000ffff
#define RX_DROPPED_PKT_CNTR_RD(src)                      (((src) & 0x0000ffff))
#define RX_DROPPED_PKT_CNTR_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register TBYT	*/ 
/*	 Fields tx_byte_cntr	 */
#define TX_BYTE_CNTR_WIDTH                                                   31
#define TX_BYTE_CNTR_SHIFT                                                    0
#define TX_BYTE_CNTR_MASK                                            0x7fffffff
#define TX_BYTE_CNTR_RD(src)                             (((src) & 0x7fffffff))
#define TX_BYTE_CNTR_SET(dst,src) \
                          (((dst) & ~0x7fffffff) | (((u32)(src)) & 0x7fffffff))

/*	Register TPKT	*/ 
/*	 Fields tx_pkt_cntr	 */
#define TX_PKT_CNTR_WIDTH                                                    31
#define TX_PKT_CNTR_SHIFT                                                     0
#define TX_PKT_CNTR_MASK                                             0x7fffffff
#define TX_PKT_CNTR_RD(src)                              (((src) & 0x7fffffff))
#define TX_PKT_CNTR_SET(dst,src) \
                          (((dst) & ~0x7fffffff) | (((u32)(src)) & 0x7fffffff))

/*	Register TMCA	*/ 
/*	 Fields tx_mc_pkt_cntr	 */
#define TX_MC_PKT_CNTR_WIDTH                                                 31
#define TX_MC_PKT_CNTR_SHIFT                                                  0
#define TX_MC_PKT_CNTR_MASK                                          0x7fffffff
#define TX_MC_PKT_CNTR_RD(src)                           (((src) & 0x7fffffff))
#define TX_MC_PKT_CNTR_SET(dst,src) \
                          (((dst) & ~0x7fffffff) | (((u32)(src)) & 0x7fffffff))

/*	Register TBCA	*/ 
/*	 Fields tx_bc_pkt_cntr	 */
#define TX_BC_PKT_CNTR_WIDTH                                                 31
#define TX_BC_PKT_CNTR_SHIFT                                                  0
#define TX_BC_PKT_CNTR_MASK                                          0x7fffffff
#define TX_BC_PKT_CNTR_RD(src)                           (((src) & 0x7fffffff))
#define TX_BC_PKT_CNTR_SET(dst,src) \
                          (((dst) & ~0x7fffffff) | (((u32)(src)) & 0x7fffffff))

/*	Register TXPF	*/ 
/*	 Fields tx_pause_pkt_cntr	 */
#define TX_PAUSE_PKT_CNTR_WIDTH                                              16
#define TX_PAUSE_PKT_CNTR_SHIFT                                               0
#define TX_PAUSE_PKT_CNTR_MASK                                       0x0000ffff
#define TX_PAUSE_PKT_CNTR_RD(src)                        (((src) & 0x0000ffff))
#define TX_PAUSE_PKT_CNTR_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register TDFR	*/ 
/*	 Fields tx_defer_pkt_cntr	 */
#define TX_DEFER_PKT_CNTR_WIDTH                                              31
#define TX_DEFER_PKT_CNTR_SHIFT                                               0
#define TX_DEFER_PKT_CNTR_MASK                                       0x7fffffff
#define TX_DEFER_PKT_CNTR_RD(src)                        (((src) & 0x7fffffff))
#define TX_DEFER_PKT_CNTR_SET(dst,src) \
                          (((dst) & ~0x7fffffff) | (((u32)(src)) & 0x7fffffff))

/*	Register TEDF	*/ 
/*	 Fields tx_exc_defer_pkt_cntr	 */
#define TX_EXC_DEFER_PKT_CNTR_WIDTH                                          31
#define TX_EXC_DEFER_PKT_CNTR_SHIFT                                           0
#define TX_EXC_DEFER_PKT_CNTR_MASK                                   0x7fffffff
#define TX_EXC_DEFER_PKT_CNTR_RD(src)                    (((src) & 0x7fffffff))
#define TX_EXC_DEFER_PKT_CNTR_SET(dst,src) \
                          (((dst) & ~0x7fffffff) | (((u32)(src)) & 0x7fffffff))

/*	Register TSCL	*/ 
/*	 Fields tx_col_pkt_cntr	 */
#define TX_COL_PKT_CNTR_WIDTH                                                31
#define TX_COL_PKT_CNTR_SHIFT                                                 0
#define TX_COL_PKT_CNTR_MASK                                         0x7fffffff
#define TX_COL_PKT_CNTR_RD(src)                          (((src) & 0x7fffffff))
#define TX_COL_PKT_CNTR_SET(dst,src) \
                          (((dst) & ~0x7fffffff) | (((u32)(src)) & 0x7fffffff))

/*	Register TMCL	*/ 
/*	 Fields tx_mul_col_pkt_cntr	 */
#define TX_MUL_COL_PKT_CNTR_WIDTH                                            31
#define TX_MUL_COL_PKT_CNTR_SHIFT                                             0
#define TX_MUL_COL_PKT_CNTR_MASK                                     0x7fffffff
#define TX_MUL_COL_PKT_CNTR_RD(src)                      (((src) & 0x7fffffff))
#define TX_MUL_COL_PKT_CNTR_SET(dst,src) \
                          (((dst) & ~0x7fffffff) | (((u32)(src)) & 0x7fffffff))

/*	Register TLCL	*/ 
/*	 Fields tx_late_col_pkt_cntr	 */
#define TX_LATE_COL_PKT_CNTR_WIDTH                                           31
#define TX_LATE_COL_PKT_CNTR_SHIFT                                            0
#define TX_LATE_COL_PKT_CNTR_MASK                                    0x7fffffff
#define TX_LATE_COL_PKT_CNTR_RD(src)                     (((src) & 0x7fffffff))
#define TX_LATE_COL_PKT_CNTR_SET(dst,src) \
                          (((dst) & ~0x7fffffff) | (((u32)(src)) & 0x7fffffff))

/*	Register TXCL	*/ 
/*	 Fields tx_exc_col_pkt_cntr	 */
#define TX_EXC_COL_PKT_CNTR_WIDTH                                            31
#define TX_EXC_COL_PKT_CNTR_SHIFT                                             0
#define TX_EXC_COL_PKT_CNTR_MASK                                     0x7fffffff
#define TX_EXC_COL_PKT_CNTR_RD(src)                      (((src) & 0x7fffffff))
#define TX_EXC_COL_PKT_CNTR_SET(dst,src) \
                          (((dst) & ~0x7fffffff) | (((u32)(src)) & 0x7fffffff))

/*	Register TNCL	*/ 
/*	 Fields tx_total_col_cntr	 */
#define TX_TOTAL_COL_CNTR_WIDTH                                              31
#define TX_TOTAL_COL_CNTR_SHIFT                                               0
#define TX_TOTAL_COL_CNTR_MASK                                       0x7fffffff
#define TX_TOTAL_COL_CNTR_RD(src)                        (((src) & 0x7fffffff))
#define TX_TOTAL_COL_CNTR_SET(dst,src) \
                          (((dst) & ~0x7fffffff) | (((u32)(src)) & 0x7fffffff))

/*	Register TPFH	*/ 
/*	 Fields tx_pause_frame_cntr	 */
#define TX_PAUSE_FRAME_CNTR_WIDTH                                            16
#define TX_PAUSE_FRAME_CNTR_SHIFT                                             0
#define TX_PAUSE_FRAME_CNTR_MASK                                     0x0000ffff
#define TX_PAUSE_FRAME_CNTR_RD(src)                      (((src) & 0x0000ffff))
#define TX_PAUSE_FRAME_CNTR_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register TDRP	*/ 
/*	 Fields tx_drop_frame_cntr	 */
#define TX_DROP_FRAME_CNTR_WIDTH                                             16
#define TX_DROP_FRAME_CNTR_SHIFT                                              0
#define TX_DROP_FRAME_CNTR_MASK                                      0x0000ffff
#define TX_DROP_FRAME_CNTR_RD(src)                       (((src) & 0x0000ffff))
#define TX_DROP_FRAME_CNTR_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register TJBR	*/ 
/*	 Fields tx_jabber_frame_cntr	 */
#define TX_JABBER_FRAME_CNTR_WIDTH                                           12
#define TX_JABBER_FRAME_CNTR_SHIFT                                            0
#define TX_JABBER_FRAME_CNTR_MASK                                    0x00000fff
#define TX_JABBER_FRAME_CNTR_RD(src)                     (((src) & 0x00000fff))
#define TX_JABBER_FRAME_CNTR_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register TFCS	*/ 
/*	 Fields tx_fcs_error_cntr	 */
#define TX_FCS_ERROR_CNTR_WIDTH                                              12
#define TX_FCS_ERROR_CNTR_SHIFT                                               0
#define TX_FCS_ERROR_CNTR_MASK                                       0x00000fff
#define TX_FCS_ERROR_CNTR_RD(src)                        (((src) & 0x00000fff))
#define TX_FCS_ERROR_CNTR_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register TXCF	*/ 
/*	 Fields tx_ctrl_frame_cntr	 */
#define TX_CTRL_FRAME_CNTR_WIDTH                                             12
#define TX_CTRL_FRAME_CNTR_SHIFT                                              0
#define TX_CTRL_FRAME_CNTR_MASK                                      0x00000fff
#define TX_CTRL_FRAME_CNTR_RD(src)                       (((src) & 0x00000fff))
#define TX_CTRL_FRAME_CNTR_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register TOVR	*/ 
/*	 Fields tx_ovrsize_frame_cntr	 */
#define TX_OVRSIZE_FRAME_CNTR_WIDTH                                          12
#define TX_OVRSIZE_FRAME_CNTR_SHIFT                                           0
#define TX_OVRSIZE_FRAME_CNTR_MASK                                   0x00000fff
#define TX_OVRSIZE_FRAME_CNTR_RD(src)                    (((src) & 0x00000fff))
#define TX_OVRSIZE_FRAME_CNTR_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register TUND	*/ 
/*	 Fields tx_undsize_frame_cntr	 */
#define TX_UNDSIZE_FRAME_CNTR_WIDTH                                          12
#define TX_UNDSIZE_FRAME_CNTR_SHIFT                                           0
#define TX_UNDSIZE_FRAME_CNTR_MASK                                   0x00000fff
#define TX_UNDSIZE_FRAME_CNTR_RD(src)                    (((src) & 0x00000fff))
#define TX_UNDSIZE_FRAME_CNTR_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register TFRG	*/ 
/*	 Fields tx_frag_cntr	 */
#define TX_FRAG_CNTR_WIDTH                                                   12
#define TX_FRAG_CNTR_SHIFT                                                    0
#define TX_FRAG_CNTR_MASK                                            0x00000fff
#define TX_FRAG_CNTR_RD(src)                             (((src) & 0x00000fff))
#define TX_FRAG_CNTR_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register CAR1	*/ 
/*	 Fields C1_64	 */
#define C1_641_WIDTH                                                          1
#define C1_641_SHIFT                                                         31
#define C1_641_MASK                                                  0x80000000
#define C1_641_RD(src)                               (((src) & 0x80000000)>>31)
#define C1_641_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields C1_127	 */
#define C1_1271_WIDTH                                                         1
#define C1_1271_SHIFT                                                        30
#define C1_1271_MASK                                                 0x40000000
#define C1_1271_RD(src)                              (((src) & 0x40000000)>>30)
#define C1_1271_SET(dst,src) \
                      (((dst) & ~0x40000000) | (((u32)(src)<<30) & 0x40000000))
/*	 Fields C1_255	 */
#define C1_2551_WIDTH                                                         1
#define C1_2551_SHIFT                                                        29
#define C1_2551_MASK                                                 0x20000000
#define C1_2551_RD(src)                              (((src) & 0x20000000)>>29)
#define C1_2551_SET(dst,src) \
                      (((dst) & ~0x20000000) | (((u32)(src)<<29) & 0x20000000))
/*	 Fields C1_511	 */
#define C1_5111_WIDTH                                                         1
#define C1_5111_SHIFT                                                        28
#define C1_5111_MASK                                                 0x10000000
#define C1_5111_RD(src)                              (((src) & 0x10000000)>>28)
#define C1_5111_SET(dst,src) \
                      (((dst) & ~0x10000000) | (((u32)(src)<<28) & 0x10000000))
/*	 Fields C1_1K	 */
#define C1_1K1_WIDTH                                                          1
#define C1_1K1_SHIFT                                                         27
#define C1_1K1_MASK                                                  0x08000000
#define C1_1K1_RD(src)                               (((src) & 0x08000000)>>27)
#define C1_1K1_SET(dst,src) \
                      (((dst) & ~0x08000000) | (((u32)(src)<<27) & 0x08000000))
/*	 Fields C1_MAX	 */
#define C1_MAX1_WIDTH                                                         1
#define C1_MAX1_SHIFT                                                        26
#define C1_MAX1_MASK                                                 0x04000000
#define C1_MAX1_RD(src)                              (((src) & 0x04000000)>>26)
#define C1_MAX1_SET(dst,src) \
                      (((dst) & ~0x04000000) | (((u32)(src)<<26) & 0x04000000))
/*	 Fields C1_MGV	 */
#define C1_MGV1_WIDTH                                                         1
#define C1_MGV1_SHIFT                                                        25
#define C1_MGV1_MASK                                                 0x02000000
#define C1_MGV1_RD(src)                              (((src) & 0x02000000)>>25)
#define C1_MGV1_SET(dst,src) \
                      (((dst) & ~0x02000000) | (((u32)(src)<<25) & 0x02000000))
/*	 Fields C1_RBY	 */
#define C1_RBY1_WIDTH                                                         1
#define C1_RBY1_SHIFT                                                        16
#define C1_RBY1_MASK                                                 0x00010000
#define C1_RBY1_RD(src)                              (((src) & 0x00010000)>>16)
#define C1_RBY1_SET(dst,src) \
                      (((dst) & ~0x00010000) | (((u32)(src)<<16) & 0x00010000))
/*	 Fields C1_RPK	 */
#define C1_RPK1_WIDTH                                                         1
#define C1_RPK1_SHIFT                                                        15
#define C1_RPK1_MASK                                                 0x00008000
#define C1_RPK1_RD(src)                              (((src) & 0x00008000)>>15)
#define C1_RPK1_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))
/*	 Fields C1_RFC	 */
#define C1_RFC1_WIDTH                                                         1
#define C1_RFC1_SHIFT                                                        14
#define C1_RFC1_MASK                                                 0x00004000
#define C1_RFC1_RD(src)                              (((src) & 0x00004000)>>14)
#define C1_RFC1_SET(dst,src) \
                      (((dst) & ~0x00004000) | (((u32)(src)<<14) & 0x00004000))
/*	 Fields C1_RMC	 */
#define C1_RMC1_WIDTH                                                         1
#define C1_RMC1_SHIFT                                                        13
#define C1_RMC1_MASK                                                 0x00002000
#define C1_RMC1_RD(src)                              (((src) & 0x00002000)>>13)
#define C1_RMC1_SET(dst,src) \
                      (((dst) & ~0x00002000) | (((u32)(src)<<13) & 0x00002000))
/*	 Fields C1_RBC	 */
#define C1_RBC1_WIDTH                                                         1
#define C1_RBC1_SHIFT                                                        12
#define C1_RBC1_MASK                                                 0x00001000
#define C1_RBC1_RD(src)                              (((src) & 0x00001000)>>12)
#define C1_RBC1_SET(dst,src) \
                      (((dst) & ~0x00001000) | (((u32)(src)<<12) & 0x00001000))
/*	 Fields C1_RXC	 */
#define C1_RXC1_WIDTH                                                         1
#define C1_RXC1_SHIFT                                                        11
#define C1_RXC1_MASK                                                 0x00000800
#define C1_RXC1_RD(src)                              (((src) & 0x00000800)>>11)
#define C1_RXC1_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*	 Fields C1_RXP	 */
#define C1_RXP1_WIDTH                                                         1
#define C1_RXP1_SHIFT                                                        10
#define C1_RXP1_MASK                                                 0x00000400
#define C1_RXP1_RD(src)                              (((src) & 0x00000400)>>10)
#define C1_RXP1_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields C1_RXU	 */
#define C1_RXU1_WIDTH                                                         1
#define C1_RXU1_SHIFT                                                         9
#define C1_RXU1_MASK                                                 0x00000200
#define C1_RXU1_RD(src)                               (((src) & 0x00000200)>>9)
#define C1_RXU1_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields C1_RAL	 */
#define C1_RAL1_WIDTH                                                         1
#define C1_RAL1_SHIFT                                                         8
#define C1_RAL1_MASK                                                 0x00000100
#define C1_RAL1_RD(src)                               (((src) & 0x00000100)>>8)
#define C1_RAL1_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*	 Fields C1_RFL	 */
#define C1_RFL1_WIDTH                                                         1
#define C1_RFL1_SHIFT                                                         7
#define C1_RFL1_MASK                                                 0x00000080
#define C1_RFL1_RD(src)                               (((src) & 0x00000080)>>7)
#define C1_RFL1_SET(dst,src) \
                       (((dst) & ~0x00000080) | (((u32)(src)<<7) & 0x00000080))
/*	 Fields C1_RCD	 */
#define C1_RCD1_WIDTH                                                         1
#define C1_RCD1_SHIFT                                                         6
#define C1_RCD1_MASK                                                 0x00000040
#define C1_RCD1_RD(src)                               (((src) & 0x00000040)>>6)
#define C1_RCD1_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*	 Fields C1_RCS	 */
#define C1_RCS1_WIDTH                                                         1
#define C1_RCS1_SHIFT                                                         5
#define C1_RCS1_MASK                                                 0x00000020
#define C1_RCS1_RD(src)                               (((src) & 0x00000020)>>5)
#define C1_RCS1_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields C1_RUN	 */
#define C1_RUN1_WIDTH                                                         1
#define C1_RUN1_SHIFT                                                         4
#define C1_RUN1_MASK                                                 0x00000010
#define C1_RUN1_RD(src)                               (((src) & 0x00000010)>>4)
#define C1_RUN1_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields C1_ROV	 */
#define C1_ROV1_WIDTH                                                         1
#define C1_ROV1_SHIFT                                                         3
#define C1_ROV1_MASK                                                 0x00000008
#define C1_ROV1_RD(src)                               (((src) & 0x00000008)>>3)
#define C1_ROV1_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields C1_RFR	 */
#define C1_RFR1_WIDTH                                                         1
#define C1_RFR1_SHIFT                                                         2
#define C1_RFR1_MASK                                                 0x00000004
#define C1_RFR1_RD(src)                               (((src) & 0x00000004)>>2)
#define C1_RFR1_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields C1_RJB	 */
#define C1_RJB1_WIDTH                                                         1
#define C1_RJB1_SHIFT                                                         1
#define C1_RJB1_MASK                                                 0x00000002
#define C1_RJB1_RD(src)                               (((src) & 0x00000002)>>1)
#define C1_RJB1_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields C1_RDR	 */
#define C1_RDR1_WIDTH                                                         1
#define C1_RDR1_SHIFT                                                         0
#define C1_RDR1_MASK                                                 0x00000001
#define C1_RDR1_RD(src)                                  (((src) & 0x00000001))
#define C1_RDR1_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register CAR2	*/ 
/*	 Fields C2_TJB	 */
#define C2_TJB2_WIDTH                                                         1
#define C2_TJB2_SHIFT                                                        19
#define C2_TJB2_MASK                                                 0x00080000
#define C2_TJB2_RD(src)                              (((src) & 0x00080000)>>19)
#define C2_TJB2_SET(dst,src) \
                      (((dst) & ~0x00080000) | (((u32)(src)<<19) & 0x00080000))
/*	 Fields C2_TFC	 */
#define C2_TFC2_WIDTH                                                         1
#define C2_TFC2_SHIFT                                                        18
#define C2_TFC2_MASK                                                 0x00040000
#define C2_TFC2_RD(src)                              (((src) & 0x00040000)>>18)
#define C2_TFC2_SET(dst,src) \
                      (((dst) & ~0x00040000) | (((u32)(src)<<18) & 0x00040000))
/*	 Fields C2_TCF	 */
#define C2_TCF2_WIDTH                                                         1
#define C2_TCF2_SHIFT                                                        17
#define C2_TCF2_MASK                                                 0x00020000
#define C2_TCF2_RD(src)                              (((src) & 0x00020000)>>17)
#define C2_TCF2_SET(dst,src) \
                      (((dst) & ~0x00020000) | (((u32)(src)<<17) & 0x00020000))
/*	 Fields C2_TOV	 */
#define C2_TOV2_WIDTH                                                         1
#define C2_TOV2_SHIFT                                                        16
#define C2_TOV2_MASK                                                 0x00010000
#define C2_TOV2_RD(src)                              (((src) & 0x00010000)>>16)
#define C2_TOV2_SET(dst,src) \
                      (((dst) & ~0x00010000) | (((u32)(src)<<16) & 0x00010000))
/*	 Fields C2_TUN	 */
#define C2_TUN2_WIDTH                                                         1
#define C2_TUN2_SHIFT                                                        15
#define C2_TUN2_MASK                                                 0x00008000
#define C2_TUN2_RD(src)                              (((src) & 0x00008000)>>15)
#define C2_TUN2_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))
/*	 Fields C2_TFG	 */
#define C2_TFG2_WIDTH                                                         1
#define C2_TFG2_SHIFT                                                        14
#define C2_TFG2_MASK                                                 0x00004000
#define C2_TFG2_RD(src)                              (((src) & 0x00004000)>>14)
#define C2_TFG2_SET(dst,src) \
                      (((dst) & ~0x00004000) | (((u32)(src)<<14) & 0x00004000))
/*	 Fields C2_TBY	 */
#define C2_TBY2_WIDTH                                                         1
#define C2_TBY2_SHIFT                                                        13
#define C2_TBY2_MASK                                                 0x00002000
#define C2_TBY2_RD(src)                              (((src) & 0x00002000)>>13)
#define C2_TBY2_SET(dst,src) \
                      (((dst) & ~0x00002000) | (((u32)(src)<<13) & 0x00002000))
/*	 Fields C2_TPK	 */
#define C2_TPK2_WIDTH                                                         1
#define C2_TPK2_SHIFT                                                        12
#define C2_TPK2_MASK                                                 0x00001000
#define C2_TPK2_RD(src)                              (((src) & 0x00001000)>>12)
#define C2_TPK2_SET(dst,src) \
                      (((dst) & ~0x00001000) | (((u32)(src)<<12) & 0x00001000))
/*	 Fields C2_TMC	 */
#define C2_TMC2_WIDTH                                                         1
#define C2_TMC2_SHIFT                                                        11
#define C2_TMC2_MASK                                                 0x00000800
#define C2_TMC2_RD(src)                              (((src) & 0x00000800)>>11)
#define C2_TMC2_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*	 Fields C2_TBC	 */
#define C2_TBC2_WIDTH                                                         1
#define C2_TBC2_SHIFT                                                        10
#define C2_TBC2_MASK                                                 0x00000400
#define C2_TBC2_RD(src)                              (((src) & 0x00000400)>>10)
#define C2_TBC2_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields C2_TPF	 */
#define C2_TPF2_WIDTH                                                         1
#define C2_TPF2_SHIFT                                                         9
#define C2_TPF2_MASK                                                 0x00000200
#define C2_TPF2_RD(src)                               (((src) & 0x00000200)>>9)
#define C2_TPF2_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields C2_TDF	 */
#define C2_TDF2_WIDTH                                                         1
#define C2_TDF2_SHIFT                                                         8
#define C2_TDF2_MASK                                                 0x00000100
#define C2_TDF2_RD(src)                               (((src) & 0x00000100)>>8)
#define C2_TDF2_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*	 Fields C2_TED	 */
#define C2_TED2_WIDTH                                                         1
#define C2_TED2_SHIFT                                                         7
#define C2_TED2_MASK                                                 0x00000080
#define C2_TED2_RD(src)                               (((src) & 0x00000080)>>7)
#define C2_TED2_SET(dst,src) \
                       (((dst) & ~0x00000080) | (((u32)(src)<<7) & 0x00000080))
/*	 Fields C2_TSC	 */
#define C2_TSC2_WIDTH                                                         1
#define C2_TSC2_SHIFT                                                         6
#define C2_TSC2_MASK                                                 0x00000040
#define C2_TSC2_RD(src)                               (((src) & 0x00000040)>>6)
#define C2_TSC2_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*	 Fields C2_TMA	 */
#define C2_TMA2_WIDTH                                                         1
#define C2_TMA2_SHIFT                                                         5
#define C2_TMA2_MASK                                                 0x00000020
#define C2_TMA2_RD(src)                               (((src) & 0x00000020)>>5)
#define C2_TMA2_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields C2_TLC	 */
#define C2_TLC2_WIDTH                                                         1
#define C2_TLC2_SHIFT                                                         4
#define C2_TLC2_MASK                                                 0x00000010
#define C2_TLC2_RD(src)                               (((src) & 0x00000010)>>4)
#define C2_TLC2_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields C2_TXC	 */
#define C2_TXC2_WIDTH                                                         1
#define C2_TXC2_SHIFT                                                         3
#define C2_TXC2_MASK                                                 0x00000008
#define C2_TXC2_RD(src)                               (((src) & 0x00000008)>>3)
#define C2_TXC2_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields C2_TNC	 */
#define C2_TNC2_WIDTH                                                         1
#define C2_TNC2_SHIFT                                                         2
#define C2_TNC2_MASK                                                 0x00000004
#define C2_TNC2_RD(src)                               (((src) & 0x00000004)>>2)
#define C2_TNC2_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields C2_TPH	 */
#define C2_TPH2_WIDTH                                                         1
#define C2_TPH2_SHIFT                                                         1
#define C2_TPH2_MASK                                                 0x00000002
#define C2_TPH2_RD(src)                               (((src) & 0x00000002)>>1)
#define C2_TPH2_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields C2_TDP	 */
#define C2_TDP2_WIDTH                                                         1
#define C2_TDP2_SHIFT                                                         0
#define C2_TDP2_MASK                                                 0x00000001
#define C2_TDP2_RD(src)                                  (((src) & 0x00000001))
#define C2_TDP2_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register CAM1	*/ 
/*	 Fields M1_64	 */
#define M1_641_WIDTH                                                          1
#define M1_641_SHIFT                                                         31
#define M1_641_MASK                                                  0x80000000
#define M1_641_RD(src)                               (((src) & 0x80000000)>>31)
#define M1_641_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields M1_127	 */
#define M1_1271_WIDTH                                                         1
#define M1_1271_SHIFT                                                        30
#define M1_1271_MASK                                                 0x40000000
#define M1_1271_RD(src)                              (((src) & 0x40000000)>>30)
#define M1_1271_SET(dst,src) \
                      (((dst) & ~0x40000000) | (((u32)(src)<<30) & 0x40000000))
/*	 Fields M1_255	 */
#define M1_2551_WIDTH                                                         1
#define M1_2551_SHIFT                                                        29
#define M1_2551_MASK                                                 0x20000000
#define M1_2551_RD(src)                              (((src) & 0x20000000)>>29)
#define M1_2551_SET(dst,src) \
                      (((dst) & ~0x20000000) | (((u32)(src)<<29) & 0x20000000))
/*	 Fields M1_511	 */
#define M1_5111_WIDTH                                                         1
#define M1_5111_SHIFT                                                        28
#define M1_5111_MASK                                                 0x10000000
#define M1_5111_RD(src)                              (((src) & 0x10000000)>>28)
#define M1_5111_SET(dst,src) \
                      (((dst) & ~0x10000000) | (((u32)(src)<<28) & 0x10000000))
/*	 Fields M1_1K	 */
#define M1_1K1_WIDTH                                                          1
#define M1_1K1_SHIFT                                                         27
#define M1_1K1_MASK                                                  0x08000000
#define M1_1K1_RD(src)                               (((src) & 0x08000000)>>27)
#define M1_1K1_SET(dst,src) \
                      (((dst) & ~0x08000000) | (((u32)(src)<<27) & 0x08000000))
/*	 Fields M1_MAX	 */
#define M1_MAX1_WIDTH                                                         1
#define M1_MAX1_SHIFT                                                        26
#define M1_MAX1_MASK                                                 0x04000000
#define M1_MAX1_RD(src)                              (((src) & 0x04000000)>>26)
#define M1_MAX1_SET(dst,src) \
                      (((dst) & ~0x04000000) | (((u32)(src)<<26) & 0x04000000))
/*	 Fields M1_MGV	 */
#define M1_MGV1_WIDTH                                                         1
#define M1_MGV1_SHIFT                                                        25
#define M1_MGV1_MASK                                                 0x02000000
#define M1_MGV1_RD(src)                              (((src) & 0x02000000)>>25)
#define M1_MGV1_SET(dst,src) \
                      (((dst) & ~0x02000000) | (((u32)(src)<<25) & 0x02000000))
/*	 Fields M1_RBY	 */
#define M1_RBY1_WIDTH                                                         1
#define M1_RBY1_SHIFT                                                        16
#define M1_RBY1_MASK                                                 0x00010000
#define M1_RBY1_RD(src)                              (((src) & 0x00010000)>>16)
#define M1_RBY1_SET(dst,src) \
                      (((dst) & ~0x00010000) | (((u32)(src)<<16) & 0x00010000))
/*	 Fields M1_RPK	 */
#define M1_RPK1_WIDTH                                                         1
#define M1_RPK1_SHIFT                                                        15
#define M1_RPK1_MASK                                                 0x00008000
#define M1_RPK1_RD(src)                              (((src) & 0x00008000)>>15)
#define M1_RPK1_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))
/*	 Fields M1_RFC	 */
#define M1_RFC1_WIDTH                                                         1
#define M1_RFC1_SHIFT                                                        14
#define M1_RFC1_MASK                                                 0x00004000
#define M1_RFC1_RD(src)                              (((src) & 0x00004000)>>14)
#define M1_RFC1_SET(dst,src) \
                      (((dst) & ~0x00004000) | (((u32)(src)<<14) & 0x00004000))
/*	 Fields M1_RMC	 */
#define M1_RMC1_WIDTH                                                         1
#define M1_RMC1_SHIFT                                                        13
#define M1_RMC1_MASK                                                 0x00002000
#define M1_RMC1_RD(src)                              (((src) & 0x00002000)>>13)
#define M1_RMC1_SET(dst,src) \
                      (((dst) & ~0x00002000) | (((u32)(src)<<13) & 0x00002000))
/*	 Fields M1_RBC	 */
#define M1_RBC1_WIDTH                                                         1
#define M1_RBC1_SHIFT                                                        12
#define M1_RBC1_MASK                                                 0x00001000
#define M1_RBC1_RD(src)                              (((src) & 0x00001000)>>12)
#define M1_RBC1_SET(dst,src) \
                      (((dst) & ~0x00001000) | (((u32)(src)<<12) & 0x00001000))
/*	 Fields M1_RXC	 */
#define M1_RXC1_WIDTH                                                         1
#define M1_RXC1_SHIFT                                                        11
#define M1_RXC1_MASK                                                 0x00000800
#define M1_RXC1_RD(src)                              (((src) & 0x00000800)>>11)
#define M1_RXC1_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*	 Fields M1_RXP	 */
#define M1_RXP1_WIDTH                                                         1
#define M1_RXP1_SHIFT                                                        10
#define M1_RXP1_MASK                                                 0x00000400
#define M1_RXP1_RD(src)                              (((src) & 0x00000400)>>10)
#define M1_RXP1_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields M1_RXU	 */
#define M1_RXU1_WIDTH                                                         1
#define M1_RXU1_SHIFT                                                         9
#define M1_RXU1_MASK                                                 0x00000200
#define M1_RXU1_RD(src)                               (((src) & 0x00000200)>>9)
#define M1_RXU1_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields M1_RAL	 */
#define M1_RAL1_WIDTH                                                         1
#define M1_RAL1_SHIFT                                                         8
#define M1_RAL1_MASK                                                 0x00000100
#define M1_RAL1_RD(src)                               (((src) & 0x00000100)>>8)
#define M1_RAL1_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*	 Fields M1_RFL	 */
#define M1_RFL1_WIDTH                                                         1
#define M1_RFL1_SHIFT                                                         7
#define M1_RFL1_MASK                                                 0x00000080
#define M1_RFL1_RD(src)                               (((src) & 0x00000080)>>7)
#define M1_RFL1_SET(dst,src) \
                       (((dst) & ~0x00000080) | (((u32)(src)<<7) & 0x00000080))
/*	 Fields M1_RCD	 */
#define M1_RCD1_WIDTH                                                         1
#define M1_RCD1_SHIFT                                                         6
#define M1_RCD1_MASK                                                 0x00000040
#define M1_RCD1_RD(src)                               (((src) & 0x00000040)>>6)
#define M1_RCD1_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*	 Fields M1_RCS	 */
#define M1_RCS1_WIDTH                                                         1
#define M1_RCS1_SHIFT                                                         5
#define M1_RCS1_MASK                                                 0x00000020
#define M1_RCS1_RD(src)                               (((src) & 0x00000020)>>5)
#define M1_RCS1_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields M1_RUN	 */
#define M1_RUN1_WIDTH                                                         1
#define M1_RUN1_SHIFT                                                         4
#define M1_RUN1_MASK                                                 0x00000010
#define M1_RUN1_RD(src)                               (((src) & 0x00000010)>>4)
#define M1_RUN1_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields M1_ROV	 */
#define M1_ROV1_WIDTH                                                         1
#define M1_ROV1_SHIFT                                                         3
#define M1_ROV1_MASK                                                 0x00000008
#define M1_ROV1_RD(src)                               (((src) & 0x00000008)>>3)
#define M1_ROV1_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields M1_RFR	 */
#define M1_RFR1_WIDTH                                                         1
#define M1_RFR1_SHIFT                                                         2
#define M1_RFR1_MASK                                                 0x00000004
#define M1_RFR1_RD(src)                               (((src) & 0x00000004)>>2)
#define M1_RFR1_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields M1_RJB	 */
#define M1_RJB1_WIDTH                                                         1
#define M1_RJB1_SHIFT                                                         1
#define M1_RJB1_MASK                                                 0x00000002
#define M1_RJB1_RD(src)                               (((src) & 0x00000002)>>1)
#define M1_RJB1_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields M1_RDR	 */
#define M1_RDR1_WIDTH                                                         1
#define M1_RDR1_SHIFT                                                         0
#define M1_RDR1_MASK                                                 0x00000001
#define M1_RDR1_RD(src)                                  (((src) & 0x00000001))
#define M1_RDR1_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register CAM2	*/ 
/*	 Fields M2_TJB	 */
#define M2_TJB2_WIDTH                                                         1
#define M2_TJB2_SHIFT                                                        19
#define M2_TJB2_MASK                                                 0x00080000
#define M2_TJB2_RD(src)                              (((src) & 0x00080000)>>19)
#define M2_TJB2_SET(dst,src) \
                      (((dst) & ~0x00080000) | (((u32)(src)<<19) & 0x00080000))
/*	 Fields M2_TFC	 */
#define M2_TFC2_WIDTH                                                         1
#define M2_TFC2_SHIFT                                                        18
#define M2_TFC2_MASK                                                 0x00040000
#define M2_TFC2_RD(src)                              (((src) & 0x00040000)>>18)
#define M2_TFC2_SET(dst,src) \
                      (((dst) & ~0x00040000) | (((u32)(src)<<18) & 0x00040000))
/*	 Fields M2_TCF	 */
#define M2_TCF2_WIDTH                                                         1
#define M2_TCF2_SHIFT                                                        17
#define M2_TCF2_MASK                                                 0x00020000
#define M2_TCF2_RD(src)                              (((src) & 0x00020000)>>17)
#define M2_TCF2_SET(dst,src) \
                      (((dst) & ~0x00020000) | (((u32)(src)<<17) & 0x00020000))
/*	 Fields M2_TOV	 */
#define M2_TOV2_WIDTH                                                         1
#define M2_TOV2_SHIFT                                                        16
#define M2_TOV2_MASK                                                 0x00010000
#define M2_TOV2_RD(src)                              (((src) & 0x00010000)>>16)
#define M2_TOV2_SET(dst,src) \
                      (((dst) & ~0x00010000) | (((u32)(src)<<16) & 0x00010000))
/*	 Fields M2_TUN	 */
#define M2_TUN2_WIDTH                                                         1
#define M2_TUN2_SHIFT                                                        15
#define M2_TUN2_MASK                                                 0x00008000
#define M2_TUN2_RD(src)                              (((src) & 0x00008000)>>15)
#define M2_TUN2_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))
/*	 Fields M2_TFG	 */
#define M2_TFG2_WIDTH                                                         1
#define M2_TFG2_SHIFT                                                        14
#define M2_TFG2_MASK                                                 0x00004000
#define M2_TFG2_RD(src)                              (((src) & 0x00004000)>>14)
#define M2_TFG2_SET(dst,src) \
                      (((dst) & ~0x00004000) | (((u32)(src)<<14) & 0x00004000))
/*	 Fields M2_TBY	 */
#define M2_TBY2_WIDTH                                                         1
#define M2_TBY2_SHIFT                                                        13
#define M2_TBY2_MASK                                                 0x00002000
#define M2_TBY2_RD(src)                              (((src) & 0x00002000)>>13)
#define M2_TBY2_SET(dst,src) \
                      (((dst) & ~0x00002000) | (((u32)(src)<<13) & 0x00002000))
/*	 Fields M2_TPK	 */
#define M2_TPK2_WIDTH                                                         1
#define M2_TPK2_SHIFT                                                        12
#define M2_TPK2_MASK                                                 0x00001000
#define M2_TPK2_RD(src)                              (((src) & 0x00001000)>>12)
#define M2_TPK2_SET(dst,src) \
                      (((dst) & ~0x00001000) | (((u32)(src)<<12) & 0x00001000))
/*	 Fields M2_TMC	 */
#define M2_TMC2_WIDTH                                                         1
#define M2_TMC2_SHIFT                                                        11
#define M2_TMC2_MASK                                                 0x00000800
#define M2_TMC2_RD(src)                              (((src) & 0x00000800)>>11)
#define M2_TMC2_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*	 Fields M2_TBC	 */
#define M2_TBC2_WIDTH                                                         1
#define M2_TBC2_SHIFT                                                        10
#define M2_TBC2_MASK                                                 0x00000400
#define M2_TBC2_RD(src)                              (((src) & 0x00000400)>>10)
#define M2_TBC2_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields M2_TPF	 */
#define M2_TPF2_WIDTH                                                         1
#define M2_TPF2_SHIFT                                                         9
#define M2_TPF2_MASK                                                 0x00000200
#define M2_TPF2_RD(src)                               (((src) & 0x00000200)>>9)
#define M2_TPF2_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields M2_TDF	 */
#define M2_TDF2_WIDTH                                                         1
#define M2_TDF2_SHIFT                                                         8
#define M2_TDF2_MASK                                                 0x00000100
#define M2_TDF2_RD(src)                               (((src) & 0x00000100)>>8)
#define M2_TDF2_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*	 Fields M2_TED	 */
#define M2_TED2_WIDTH                                                         1
#define M2_TED2_SHIFT                                                         7
#define M2_TED2_MASK                                                 0x00000080
#define M2_TED2_RD(src)                               (((src) & 0x00000080)>>7)
#define M2_TED2_SET(dst,src) \
                       (((dst) & ~0x00000080) | (((u32)(src)<<7) & 0x00000080))
/*	 Fields M2_TSC	 */
#define M2_TSC2_WIDTH                                                         1
#define M2_TSC2_SHIFT                                                         6
#define M2_TSC2_MASK                                                 0x00000040
#define M2_TSC2_RD(src)                               (((src) & 0x00000040)>>6)
#define M2_TSC2_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*	 Fields M2_TMA	 */
#define M2_TMA2_WIDTH                                                         1
#define M2_TMA2_SHIFT                                                         5
#define M2_TMA2_MASK                                                 0x00000020
#define M2_TMA2_RD(src)                               (((src) & 0x00000020)>>5)
#define M2_TMA2_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields M2_TLC	 */
#define M2_TLC2_WIDTH                                                         1
#define M2_TLC2_SHIFT                                                         4
#define M2_TLC2_MASK                                                 0x00000010
#define M2_TLC2_RD(src)                               (((src) & 0x00000010)>>4)
#define M2_TLC2_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields M2_TXC	 */
#define M2_TXC2_WIDTH                                                         1
#define M2_TXC2_SHIFT                                                         3
#define M2_TXC2_MASK                                                 0x00000008
#define M2_TXC2_RD(src)                               (((src) & 0x00000008)>>3)
#define M2_TXC2_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields M2_TNC	 */
#define M2_TNC2_WIDTH                                                         1
#define M2_TNC2_SHIFT                                                         2
#define M2_TNC2_MASK                                                 0x00000004
#define M2_TNC2_RD(src)                               (((src) & 0x00000004)>>2)
#define M2_TNC2_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields M2_TPH	 */
#define M2_TPH2_WIDTH                                                         1
#define M2_TPH2_SHIFT                                                         1
#define M2_TPH2_MASK                                                 0x00000002
#define M2_TPH2_RD(src)                               (((src) & 0x00000002)>>1)
#define M2_TPH2_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields M2_TDP	 */
#define M2_TDP2_WIDTH                                                         1
#define M2_TDP2_SHIFT                                                         0
#define M2_TDP2_MASK                                                 0x00000001
#define M2_TDP2_RD(src)                                  (((src) & 0x00000001))
#define M2_TDP2_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))
	


#endif  /* __APM_ENET_CSR_H__ */
