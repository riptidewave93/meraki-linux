/**
 * APM86xxx AHBC Bridge CSR
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
#ifndef _APM86xxx_AHBC_H__
#define _APM86xxx_AHBC_H__

/*****************************************************************************
 * APM867xx AHBC CSR Register Definition 
 *****************************************************************************
 */
#if defined(CONFIG_APM867xx)

/*	Global Base Address	*/
#define AHBC_TOP_REG_BASE_ADDR			0xddd862000ULL

/*    Address AHBC_TOP_REG  Registers */
#define GPIO_CFG_CTL_ADDR                                            0x00000000
#define GPIO_CFG_CTL_DEFAULT                                         0x20000000
#define AHBC_CFG_CTL_ADDR                                            0x00000004
#define AHBC_CFG_CTL_DEFAULT                                         0x00000000
#define SDR_OCHI_0_AHB2AXI_CFG_ADDR                                  0x0000000c
#define SDR_OCHI_0_AHB2AXI_CFG_DEFAULT                               0x000081ff
#define SDR_SDIO_AHB2AXI_CFG_ADDR                                    0x00000010
#define SDR_SDIO_AHB2AXI_CFG_DEFAULT                                 0x000081ff
#define SPI_CONFIG_ADDR                                              0x00000014
#define SPI_CONFIG_DEFAULT                                           0x00000003
#define USB_TUNE_ADDR                                                0x00000024
#define USB_TUNE_DEFAULT                                             0x01d80462
#define USB_RSTCTL_ADDR                                              0x00000028
#define USB_RSTCTL_DEFAULT                                           0x00000007
#define USB_CLKCTL_ADDR                                              0x0000002c
#define USB_CLKCTL_DEFAULT                                           0x0000000a
#define SDHC_TAP_DELAY_ADDR                                          0x00000030
#define SDHC_TAP_DELAY_DEFAULT                                       0x00000000
#define SDHC_CAP_REG_LOW_S1_ADDR                                     0x00000034
#define SDHC_CAP_REG_LOW_S1_DEFAULT                                  0x00000000
#define SDHC_CAP_REG_HIGH_S1_ADDR                                    0x00000038
#define SDHC_CAP_REG_HIGH_S1_DEFAULT                                 0x00000000
#define SDHC_CAP_REG_LOW_S2_ADDR                                     0x0000003c
#define SDHC_CAP_REG_LOW_S2_DEFAULT                                  0x00000000
#define SDHC_CAP_REG_HIGH_S2_ADDR                                    0x00000040
#define SDHC_CAP_REG_HIGH_S2_DEFAULT                                 0x00000000
#define SDHC_INIT_PRESET_ADDR                                        0x00000044
#define SDHC_INIT_PRESET_DEFAULT                                     0x00000000
#define SDHC_DEFAULT_PRESET_ADDR                                     0x00000048
#define SDHC_DEFAULT_PRESET_DEFAULT                                  0x00000000
#define SDHC_HIGH_SPEED_PRESET_ADDR                                  0x0000004c
#define SDHC_HIGH_SPEED_PRESET_DEFAULT                               0x00000000
#define SDHC_SDR12_PRESET_ADDR                                       0x00000050
#define SDHC_SDR12_PRESET_DEFAULT                                    0x00000000
#define SDHC_SDR25_PRESET_ADDR                                       0x00000054
#define SDHC_SDR25_PRESET_DEFAULT                                    0x00000000
#define SDHC_SDR50_PRESET_ADDR                                       0x00000058
#define SDHC_SDR50_PRESET_DEFAULT                                    0x00000000
#define SDHC_SDR104_PRESET_ADDR                                      0x0000005c
#define SDHC_SDR104_PRESET_DEFAULT                                   0x00000000
#define SDHC_DDR50_PRESET_ADDR                                       0x00000060
#define SDHC_DDR50_PRESET_DEFAULT                                    0x00000000
#define USBHOST_CFG_ADDR                                             0x00000064
#define USBHOST_CFG_DEFAULT                                          0x00000000
#define USBHOST_STATUS_ADDR                                          0x00000068
#define USBHOST_STATUS_DEFAULT                                       0x00000000
#define USBHOST_FATAL_CFG_ADDR                                       0x0000006c
#define USBHOST_FATAL_CFG_DEFAULT                                    0x00000000
#define SDR_HDLC_AHB2AXI_CFG_ADDR                                    0x00000070
#define SDR_HDLC_AHB2AXI_CFG_DEFAULT                                 0x000081ff
#define SDR_EHCI_0_AHB2AXI_CFG_ADDR                                  0x00000080
#define SDR_EHCI_0_AHB2AXI_CFG_DEFAULT                               0x000081ff
#define CFG_X2H_BRIDGE_ADDR                                          0x00000084
#define CFG_X2H_BRIDGE_DEFAULT                                       0x00000002
#define ENABLE_INTF_ADDR                                             0x00000090
#define ENABLE_INTF_DEFAULT                                          0x80000000
#define USBH_DESCRAM_ADDR                                            0x00000094
#define USBH_DESCRAM_DEFAULT                                         0x00000000
#define USBH_DATARAM_ADDR                                            0x00000098
#define USBH_DATARAM_DEFAULT                                         0x00000000
#define SDHC_DATARAM_ADDR                                            0x000000a0
#define SDHC_DATARAM_DEFAULT                                         0x00000000
#define FLASH_TXDATARAM_ADDR                                         0x000000a4
#define FLASH_TXDATARAM_DEFAULT                                      0x00000000
#define FLASH_RXDATARAM_ADDR                                         0x000000a8
#define FLASH_RXDATARAM_DEFAULT                                      0x00000000
#define FLASH_BCHDATARAM_ADDR                                        0x000000ac
#define FLASH_BCHDATARAM_DEFAULT                                     0x00000000
#define HDLC0_TXDATAFIFO_ADDR                                        0x000000b0
#define HDLC0_TXDATAFIFO_DEFAULT                                     0x00000000
#define HDLC0_RXDATAFIFO_ADDR                                        0x000000b4
#define HDLC0_RXDATAFIFO_DEFAULT                                     0x00000000
#define HDLC1_TXDATAFIFO_ADDR                                        0x000000b8
#define HDLC1_TXDATAFIFO_DEFAULT                                     0x00000000
#define HDLC1_RXDATAFIFO_ADDR                                        0x000000bc
#define HDLC1_RXDATAFIFO_DEFAULT                                     0x00000000
#define HDLC_CONFIG_ADDR                                             0x000000c0
#define HDLC_CONFIG_DEFAULT                                          0x00020000
#define SDHC_CONFIG_ADDR                                             0x000000c8
#define SDHC_CONFIG_DEFAULT                                          0x00000000
#define UART_MODE_SEL_ADDR                                           0x000000cc
#define UART_MODE_SEL_DEFAULT                                        0x00001000
#define AHB2AXI_BID_ADDR                                             0x000000d0
#define AHB2AXI_BID_DEFAULT                                          0x00000020
#define TDM_BID_ADDR                                                 0x000000d4
#define TDM_BID_DEFAULT                                              0x000000b5
#define AHBC_PARITY_ADDR                                             0x000000d8
#define AHBC_PARITY_DEFAULT                                          0x00000000
#define AHBC_INTERRUPT_ADDR                                          0x000000dc
#define AHBC_INTERRUPT_DEFAULT                                       0x00000000
#define AHBC_INTERRUPTMASK_ADDR                                      0x000000e0
#define AIM_0_AHB_ADDR                                               0x000000e4
#define AIM_0_AHB_DEFAULT                                            0x00000000
#define AIM_0_SIZE_CTL_ADDR                                          0x000000e8
#define AIM_0_SIZE_CTL_DEFAULT                                       0x00000000
#define AIM_0_AXI_LO_ADDR                                            0x000000ec
#define AIM_0_AXI_LO_DEFAULT                                         0x00000000
#define AIM_0_AXI_HI_ADDR                                            0x000000f0
#define AIM_0_AXI_HI_DEFAULT                                         0x00000000
#define AIM_1_AHB_ADDR                                               0x000000f4
#define AIM_1_AHB_DEFAULT                                            0x00000000
#define AIM_1_SIZE_CTL_ADDR                                          0x000000f8
#define AIM_1_SIZE_CTL_DEFAULT                                       0x00000000
#define AIM_1_AXI_LO_ADDR                                            0x000000fc
#define AIM_1_AXI_LO_DEFAULT                                         0x00000000
#define AIM_1_AXI_HI_ADDR                                            0x00000100
#define AIM_1_AXI_HI_DEFAULT                                         0x00000000
#define AIM_2_AHB_ADDR                                               0x00000104
#define AIM_2_AHB_DEFAULT                                            0x00000000
#define AIM_2_SIZE_CTL_ADDR                                          0x00000108
#define AIM_2_SIZE_CTL_DEFAULT                                       0x00000000
#define AIM_2_AXI_LO_ADDR                                            0x0000010c
#define AIM_2_AXI_LO_DEFAULT                                         0x00000000
#define AIM_2_AXI_HI_ADDR                                            0x00000110
#define AIM_2_AXI_HI_DEFAULT                                         0x00000000
#define AIM_3_AHB_ADDR                                               0x00000114
#define AIM_3_AHB_DEFAULT                                            0x00000000
#define AIM_3_SIZE_CTL_ADDR                                          0x00000118
#define AIM_3_SIZE_CTL_DEFAULT                                       0x00000000
#define AIM_3_AXI_LO_ADDR                                            0x0000011c
#define AIM_3_AXI_LO_DEFAULT                                         0x00000000
#define AIM_3_AXI_HI_ADDR                                            0x00000120
#define AIM_3_AXI_HI_DEFAULT                                         0x00000000
#define AIM_4_AHB_ADDR                                               0x00000124
#define AIM_4_AHB_DEFAULT                                            0x00000000
#define AIM_4_SIZE_CTL_ADDR                                          0x00000128
#define AIM_4_SIZE_CTL_DEFAULT                                       0x00000000
#define AIM_4_AXI_LO_ADDR                                            0x0000012c
#define AIM_4_AXI_LO_DEFAULT                                         0x00000000
#define AIM_4_AXI_HI_ADDR                                            0x00000130
#define AIM_4_AXI_HI_DEFAULT                                         0x00000000
#define AIM_5_AHB_ADDR                                               0x00000134
#define AIM_5_AHB_DEFAULT                                            0x00000000
#define AIM_5_SIZE_CTL_ADDR                                          0x00000138
#define AIM_5_SIZE_CTL_DEFAULT                                       0x00000000
#define AIM_5_AXI_LO_ADDR                                            0x0000013c
#define AIM_5_AXI_LO_DEFAULT                                         0x00000000
#define AIM_5_AXI_HI_ADDR                                            0x00000140
#define AIM_5_AXI_HI_DEFAULT                                         0x00000000
#define AIM_6_AHB_ADDR                                               0x00000144
#define AIM_6_AHB_DEFAULT                                            0x00000000
#define AIM_6_SIZE_CTL_ADDR                                          0x00000148
#define AIM_6_SIZE_CTL_DEFAULT                                       0x00000000
#define AIM_6_AXI_LO_ADDR                                            0x0000014c
#define AIM_6_AXI_LO_DEFAULT                                         0x00000000
#define AIM_6_AXI_HI_ADDR                                            0x00000150
#define AIM_6_AXI_HI_DEFAULT                                         0x00000000
#define AIM_7_AHB_ADDR                                               0x00000154
#define AIM_7_AHB_DEFAULT                                            0x00000000
#define AIM_7_SIZE_CTL_ADDR                                          0x00000158
#define AIM_7_SIZE_CTL_DEFAULT                                       0x00000000
#define AIM_7_AXI_LO_ADDR                                            0x0000015c
#define AIM_7_AXI_LO_DEFAULT                                         0x00000000
#define AIM_7_AXI_HI_ADDR                                            0x00000160
#define AIM_7_AXI_HI_DEFAULT                                         0x00000000
#define AIM_8_SIZE_CTL_ADDR                                          0x00000164
#define AIM_8_SIZE_CTL_DEFAULT                                       0x00000000
#define AIM_8_AXI_LO_ADDR                                            0x00000168
#define AIM_8_AXI_LO_DEFAULT                                         0x00000000
#define AIM_8_AXI_HI_ADDR                                            0x0000016c
#define AIM_8_AXI_HI_DEFAULT                                         0x00000000
#define AOM_0_AXI_LO_ADDR                                            0x00000170
#define AOM_0_AXI_LO_DEFAULT                                         0x00000000
#define AOM_0_AXI_HI_ADDR                                            0x00000174
#define AOM_0_AXI_HI_DEFAULT                                         0x00000000
#define AOM_0_SIZE_CTL_ADDR                                          0x00000178
#define AOM_0_SIZE_CTL_DEFAULT                                       0x00000000
#define AOM_0_AHB_ADDR                                               0x0000017c
#define AOM_0_AHB_DEFAULT                                            0x00000000
#define AOM_1_AXI_LO_ADDR                                            0x00000180
#define AOM_1_AXI_LO_DEFAULT                                         0x00000000
#define AOM_1_AXI_HI_ADDR                                            0x00000184
#define AOM_1_AXI_HI_DEFAULT                                         0x00000000
#define AOM_1_SIZE_CTL_ADDR                                          0x00000188
#define AOM_1_SIZE_CTL_DEFAULT                                       0x00000000
#define AOM_1_AHB_ADDR                                               0x0000018c
#define AOM_1_AHB_DEFAULT                                            0x00000000
#define AOM_2_AXI_LO_ADDR                                            0x00000190
#define AOM_2_AXI_LO_DEFAULT                                         0x00000000
#define AOM_2_AXI_HI_ADDR                                            0x00000194
#define AOM_2_AXI_HI_DEFAULT                                         0x00000000
#define AOM_2_SIZE_CTL_ADDR                                          0x00000198
#define AOM_2_SIZE_CTL_DEFAULT                                       0x00000000
#define AOM_2_AHB_ADDR                                               0x0000019c
#define AOM_2_AHB_DEFAULT                                            0x00000000
#define AOM_3_AXI_LO_ADDR                                            0x000001a0
#define AOM_3_AXI_LO_DEFAULT                                         0x00000000
#define AOM_3_AXI_HI_ADDR                                            0x000001a4
#define AOM_3_AXI_HI_DEFAULT                                         0x00000000
#define AOM_3_SIZE_CTL_ADDR                                          0x000001a8
#define AOM_3_SIZE_CTL_DEFAULT                                       0x00000000
#define AOM_3_AHB_ADDR                                               0x000001ac
#define AOM_3_AHB_DEFAULT                                            0x00000000
#define AHB_RD_ERR_ADDR_ADDR                                         0x000001b0
#define AHB_RD_ERR_ADDR_DEFAULT                                      0x00000000
#define AHB_RD_ERR_ATTRIBUTES_ADDR                                   0x000001b4
#define AHB_RD_ERR_ATTRIBUTES_DEFAULT                                0x00000000
#define AHB_WR_ERR_ADDR_ADDR                                         0x000001b8
#define AHB_WR_ERR_ADDR_DEFAULT                                      0x00000000
#define AHB_WR_ERR_DATA_ADDR                                         0x000001bc
#define AHB_WR_ERR_DATA_DEFAULT                                      0x00000000
#define AHB_WR_ERR_ATTRIBUTES_ADDR                                   0x000001c0
#define AHB_WR_ERR_ATTRIBUTES_DEFAULT                                0x00000000
#define CFG_INTR_CTL_ADDR                                            0x000001c4
#define CFG_INTR_CTL_DEFAULT                                         0x00000000
#define CFG_AUTO_RD_NFLASH_PAGE_ADDR                                 0x000001c8
#define CFG_AUTO_RD_NFLASH_PAGE_DEFAULT                              0x00000000
#define CFG_AUTO_RD_NFLASH_ADDR                                      0x000001cc
#define CFG_AUTO_RD_NFLASH_DEFAULT                                   0xe41fffff
#define CFG_AUTO_RD_NAND_DEV_ADDR                                    0x000001d0
#define CFG_AUTO_RD_NAND_DEV_DEFAULT                                 0x0003ffff
#define CFG_USB_HOST_PHY_CONNECT_ADDR                                0x000001d4
#define CFG_USB_HOST_PHY_CONNECT_DEFAULT                             0x0000f870
#define CFG_AHBC_CLK_DET_ADDR                                        0x000001d8
#define CFG_AHBC_CLK_DET_DEFAULT                                     0x00000000
#define CFG_PPC_TRC_MUX_ADDR                                         0x000001dc
#define CFG_PPC_TRC_MUX_DEFAULT                                      0x00000000
#define CFG_PIN_MUX_0_ADDR                                           0x000001e0
#define CFG_PIN_MUX_0_DEFAULT                                        0x00000000
#define CFG_PIN_MUX_1_ADDR                                           0x000001e4
#define CFG_PIN_MUX_1_DEFAULT                                        0x00000000
#define CFG_PIN_MUX_2_ADDR                                           0x000001e8
#define CFG_PIN_MUX_2_DEFAULT                                        0x00000000
#define CFG_PIN_MUX_3_ADDR                                           0x000001ec
#define CFG_PIN_MUX_3_DEFAULT                                        0x00000000
#define CFG_PIN_MUX_4_ADDR                                           0x000001f0
#define CFG_PIN_MUX_4_DEFAULT                                        0x00000000
#define CFG_PIN_MUX_5_ADDR                                           0x000001f4
#define CFG_PIN_MUX_5_DEFAULT                                        0x00000000
#define CFG_PIN_MUX_6_ADDR                                           0x000001f8
#define CFG_PIN_MUX_6_DEFAULT                                        0x00000000
#define CFG_PIN_MUX_7_ADDR                                           0x000001fc
#define CFG_PIN_MUX_7_DEFAULT                                        0x00000000

/*	Register GPIO_CFG_CTL	*/ 
/*	 Fields CFG_ENABLE_GPIO_INT	 */
#define CFG_ENABLE_GPIO_INT_WIDTH                                            26
#define CFG_ENABLE_GPIO_INT_SHIFT                                             0
#define CFG_ENABLE_GPIO_INT_MASK                                     0x03ffffff
#define CFG_ENABLE_GPIO_INT_RD(src)                      (((src) & 0x03ffffff))
#define CFG_ENABLE_GPIO_INT_WR(src)                 (((u32)(src)) & 0x03ffffff)
#define CFG_ENABLE_GPIO_INT_SET(dst,src) \
                          (((dst) & ~0x03ffffff) | (((u32)(src)) & 0x03ffffff))

/*	Register AHBC_CFG_CTL	*/ 
/*	 Fields USB_M_ENDIAN_SWAP	 */
#define USB_M_ENDIAN_SWAP_WIDTH                                               1
#define USB_M_ENDIAN_SWAP_SHIFT                                              17
#define USB_M_ENDIAN_SWAP_MASK                                       0x00020000
#define USB_M_ENDIAN_SWAP_RD(src)                    (((src) & 0x00020000)>>17)
#define USB_M_ENDIAN_SWAP_WR(src)               (((u32)(src)<<17) & 0x00020000)
#define USB_M_ENDIAN_SWAP_SET(dst,src) \
                      (((dst) & ~0x00020000) | (((u32)(src)<<17) & 0x00020000))
/*	 Fields USB_S_ENDIAN_SWAP	 */
#define USB_S_ENDIAN_SWAP_WIDTH                                               1
#define USB_S_ENDIAN_SWAP_SHIFT                                              16
#define USB_S_ENDIAN_SWAP_MASK                                       0x00010000
#define USB_S_ENDIAN_SWAP_RD(src)                    (((src) & 0x00010000)>>16)
#define USB_S_ENDIAN_SWAP_WR(src)               (((u32)(src)<<16) & 0x00010000)
#define USB_S_ENDIAN_SWAP_SET(dst,src) \
                      (((dst) & ~0x00010000) | (((u32)(src)<<16) & 0x00010000))
/*	 Fields USB_M_BIG_ENDIAN	 */
#define USB_M_BIG_ENDIAN_WIDTH                                                1
#define USB_M_BIG_ENDIAN_SHIFT                                               10
#define USB_M_BIG_ENDIAN_MASK                                        0x00000400
#define USB_M_BIG_ENDIAN_RD(src)                     (((src) & 0x00000400)>>10)
#define USB_M_BIG_ENDIAN_WR(src)                (((u32)(src)<<10) & 0x00000400)
#define USB_M_BIG_ENDIAN_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields USB_S_BIG_ENDIAN	 */
#define USB_S_BIG_ENDIAN_WIDTH                                                1
#define USB_S_BIG_ENDIAN_SHIFT                                                9
#define USB_S_BIG_ENDIAN_MASK                                        0x00000200
#define USB_S_BIG_ENDIAN_RD(src)                      (((src) & 0x00000200)>>9)
#define USB_S_BIG_ENDIAN_WR(src)                 (((u32)(src)<<9) & 0x00000200)
#define USB_S_BIG_ENDIAN_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields UFC_CSR_BIG_ENDIAN	 */
#define UFC_CSR_BIG_ENDIAN_WIDTH                                              1
#define UFC_CSR_BIG_ENDIAN_SHIFT                                              0
#define UFC_CSR_BIG_ENDIAN_MASK                                      0x00000001
#define UFC_CSR_BIG_ENDIAN_RD(src)                       (((src) & 0x00000001))
#define UFC_CSR_BIG_ENDIAN_WR(src)                  (((u32)(src)) & 0x00000001)
#define UFC_CSR_BIG_ENDIAN_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register SDR_OCHI_0_AHB2AXI_CFG	*/ 
/*	 Fields pw_axi_sideband_prio	 */
#define PW_AXI_SIDEBAND_PRIO_WIDTH                                            3
#define PW_AXI_SIDEBAND_PRIO_SHIFT                                           21
#define PW_AXI_SIDEBAND_PRIO_MASK                                    0x00e00000
#define PW_AXI_SIDEBAND_PRIO_RD(src)                 (((src) & 0x00e00000)>>21)
#define PW_AXI_SIDEBAND_PRIO_WR(src)            (((u32)(src)<<21) & 0x00e00000)
#define PW_AXI_SIDEBAND_PRIO_SET(dst,src) \
                      (((dst) & ~0x00e00000) | (((u32)(src)<<21) & 0x00e00000))
/*	 Fields pr_axi_sideband_prio	 */
#define PR_AXI_SIDEBAND_PRIO_WIDTH                                            3
#define PR_AXI_SIDEBAND_PRIO_SHIFT                                           17
#define PR_AXI_SIDEBAND_PRIO_MASK                                    0x000e0000
#define PR_AXI_SIDEBAND_PRIO_RD(src)                 (((src) & 0x000e0000)>>17)
#define PR_AXI_SIDEBAND_PRIO_WR(src)            (((u32)(src)<<17) & 0x000e0000)
#define PR_AXI_SIDEBAND_PRIO_SET(dst,src) \
                      (((dst) & ~0x000e0000) | (((u32)(src)<<17) & 0x000e0000))
/*	 Fields writes_block_reads	 */
#define WRITES_BLOCK_READS_WIDTH                                              1
#define WRITES_BLOCK_READS_SHIFT                                             15
#define WRITES_BLOCK_READS_MASK                                      0x00008000
#define WRITES_BLOCK_READS_RD(src)                   (((src) & 0x00008000)>>15)
#define WRITES_BLOCK_READS_WR(src)              (((u32)(src)<<15) & 0x00008000)
#define WRITES_BLOCK_READS_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))
/*	 Fields pw_max_outstanding	 */
#define PW_MAX_OUTSTANDING_WIDTH                                              4
#define PW_MAX_OUTSTANDING_SHIFT                                              8
#define PW_MAX_OUTSTANDING_MASK                                      0x00000f00
#define PW_MAX_OUTSTANDING_RD(src)                    (((src) & 0x00000f00)>>8)
#define PW_MAX_OUTSTANDING_WR(src)               (((u32)(src)<<8) & 0x00000f00)
#define PW_MAX_OUTSTANDING_SET(dst,src) \
                       (((dst) & ~0x00000f00) | (((u32)(src)<<8) & 0x00000f00))
/*	 Fields tx_wrr_weight	 */
#define TX_WRR_WEIGHT_WIDTH                                                   4
#define TX_WRR_WEIGHT_SHIFT                                                   4
#define TX_WRR_WEIGHT_MASK                                           0x000000f0
#define TX_WRR_WEIGHT_RD(src)                         (((src) & 0x000000f0)>>4)
#define TX_WRR_WEIGHT_WR(src)                    (((u32)(src)<<4) & 0x000000f0)
#define TX_WRR_WEIGHT_SET(dst,src) \
                       (((dst) & ~0x000000f0) | (((u32)(src)<<4) & 0x000000f0))
/*	 Fields rx_wrr_weight	 */
#define RX_WRR_WEIGHT_WIDTH                                                   4
#define RX_WRR_WEIGHT_SHIFT                                                   0
#define RX_WRR_WEIGHT_MASK                                           0x0000000f
#define RX_WRR_WEIGHT_RD(src)                            (((src) & 0x0000000f))
#define RX_WRR_WEIGHT_WR(src)                       (((u32)(src)) & 0x0000000f)
#define RX_WRR_WEIGHT_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register SDR_SDIO_AHB2AXI_CFG	*/ 
/*	 Fields pw_axi_sideband_prio	 */
#define PW_AXI_SIDEBAND_PRIO_F1_WIDTH                                         3
#define PW_AXI_SIDEBAND_PRIO_F1_SHIFT                                        21
#define PW_AXI_SIDEBAND_PRIO_F1_MASK                                 0x00e00000
#define PW_AXI_SIDEBAND_PRIO_F1_RD(src)              (((src) & 0x00e00000)>>21)
#define PW_AXI_SIDEBAND_PRIO_F1_WR(src)         (((u32)(src)<<21) & 0x00e00000)
#define PW_AXI_SIDEBAND_PRIO_F1_SET(dst,src) \
                      (((dst) & ~0x00e00000) | (((u32)(src)<<21) & 0x00e00000))
/*	 Fields pr_axi_sideband_prio	 */
#define PR_AXI_SIDEBAND_PRIO_F1_WIDTH                                         3
#define PR_AXI_SIDEBAND_PRIO_F1_SHIFT                                        17
#define PR_AXI_SIDEBAND_PRIO_F1_MASK                                 0x000e0000
#define PR_AXI_SIDEBAND_PRIO_F1_RD(src)              (((src) & 0x000e0000)>>17)
#define PR_AXI_SIDEBAND_PRIO_F1_WR(src)         (((u32)(src)<<17) & 0x000e0000)
#define PR_AXI_SIDEBAND_PRIO_F1_SET(dst,src) \
                      (((dst) & ~0x000e0000) | (((u32)(src)<<17) & 0x000e0000))
/*	 Fields writes_block_reads	 */
#define WRITES_BLOCK_READS_F1_WIDTH                                           1
#define WRITES_BLOCK_READS_F1_SHIFT                                          15
#define WRITES_BLOCK_READS_F1_MASK                                   0x00008000
#define WRITES_BLOCK_READS_F1_RD(src)                (((src) & 0x00008000)>>15)
#define WRITES_BLOCK_READS_F1_WR(src)           (((u32)(src)<<15) & 0x00008000)
#define WRITES_BLOCK_READS_F1_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))
/*	 Fields pw_max_outstanding	 */
#define PW_MAX_OUTSTANDING_F1_WIDTH                                           4
#define PW_MAX_OUTSTANDING_F1_SHIFT                                           8
#define PW_MAX_OUTSTANDING_F1_MASK                                   0x00000f00
#define PW_MAX_OUTSTANDING_F1_RD(src)                 (((src) & 0x00000f00)>>8)
#define PW_MAX_OUTSTANDING_F1_WR(src)            (((u32)(src)<<8) & 0x00000f00)
#define PW_MAX_OUTSTANDING_F1_SET(dst,src) \
                       (((dst) & ~0x00000f00) | (((u32)(src)<<8) & 0x00000f00))
/*	 Fields tx_wrr_weight	 */
#define TX_WRR_WEIGHT_F1_WIDTH                                                4
#define TX_WRR_WEIGHT_F1_SHIFT                                                4
#define TX_WRR_WEIGHT_F1_MASK                                        0x000000f0
#define TX_WRR_WEIGHT_F1_RD(src)                      (((src) & 0x000000f0)>>4)
#define TX_WRR_WEIGHT_F1_WR(src)                 (((u32)(src)<<4) & 0x000000f0)
#define TX_WRR_WEIGHT_F1_SET(dst,src) \
                       (((dst) & ~0x000000f0) | (((u32)(src)<<4) & 0x000000f0))
/*	 Fields rx_wrr_weight	 */
#define RX_WRR_WEIGHT_F1_WIDTH                                                4
#define RX_WRR_WEIGHT_F1_SHIFT                                                0
#define RX_WRR_WEIGHT_F1_MASK                                        0x0000000f
#define RX_WRR_WEIGHT_F1_RD(src)                         (((src) & 0x0000000f))
#define RX_WRR_WEIGHT_F1_WR(src)                    (((u32)(src)) & 0x0000000f)
#define RX_WRR_WEIGHT_F1_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register SPI_CONFIG	*/ 
/*	 Fields spi_1_ss_in_n	 */
#define SPI_1_SS_IN_N_WIDTH                                                   1
#define SPI_1_SS_IN_N_SHIFT                                                   1
#define SPI_1_SS_IN_N_MASK                                           0x00000002
#define SPI_1_SS_IN_N_RD(src)                         (((src) & 0x00000002)>>1)
#define SPI_1_SS_IN_N_WR(src)                    (((u32)(src)<<1) & 0x00000002)
#define SPI_1_SS_IN_N_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields spi_0_ss_in_n	 */
#define SPI_0_SS_IN_N_WIDTH                                                   1
#define SPI_0_SS_IN_N_SHIFT                                                   0
#define SPI_0_SS_IN_N_MASK                                           0x00000001
#define SPI_0_SS_IN_N_RD(src)                            (((src) & 0x00000001))
#define SPI_0_SS_IN_N_WR(src)                       (((u32)(src)) & 0x00000001)
#define SPI_0_SS_IN_N_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register USB_TUNE	*/ 
/*	 Fields USB_TXVREFTUNE	 */
#define USB_TXVREFTUNE_WIDTH                                                  4
#define USB_TXVREFTUNE_SHIFT                                                 21
#define USB_TXVREFTUNE_MASK                                          0x01e00000
#define USB_TXVREFTUNE_RD(src)                       (((src) & 0x01e00000)>>21)
#define USB_TXVREFTUNE_WR(src)                  (((u32)(src)<<21) & 0x01e00000)
#define USB_TXVREFTUNE_SET(dst,src) \
                      (((dst) & ~0x01e00000) | (((u32)(src)<<21) & 0x01e00000))
/*	 Fields USB_TXFSLSTUNE	 */
#define USB_TXFSLSTUNE_WIDTH                                                  4
#define USB_TXFSLSTUNE_SHIFT                                                 17
#define USB_TXFSLSTUNE_MASK                                          0x001e0000
#define USB_TXFSLSTUNE_RD(src)                       (((src) & 0x001e0000)>>17)
#define USB_TXFSLSTUNE_WR(src)                  (((u32)(src)<<17) & 0x001e0000)
#define USB_TXFSLSTUNE_SET(dst,src) \
                      (((dst) & ~0x001e0000) | (((u32)(src)<<17) & 0x001e0000))
/*	 Fields USB_TXPREEMPHASISTUNE	 */
#define USB_TXPREEMPHASISTUNE_WIDTH                                           2
#define USB_TXPREEMPHASISTUNE_SHIFT                                          15
#define USB_TXPREEMPHASISTUNE_MASK                                   0x00018000
#define USB_TXPREEMPHASISTUNE_RD(src)                (((src) & 0x00018000)>>15)
#define USB_TXPREEMPHASISTUNE_WR(src)           (((u32)(src)<<15) & 0x00018000)
#define USB_TXPREEMPHASISTUNE_SET(dst,src) \
                      (((dst) & ~0x00018000) | (((u32)(src)<<15) & 0x00018000))
/*	 Fields USB_TXRISETUNE	 */
#define USB_TXRISETUNE_WIDTH                                                  1
#define USB_TXRISETUNE_SHIFT                                                 14
#define USB_TXRISETUNE_MASK                                          0x00004000
#define USB_TXRISETUNE_RD(src)                       (((src) & 0x00004000)>>14)
#define USB_TXRISETUNE_WR(src)                  (((u32)(src)<<14) & 0x00004000)
#define USB_TXRISETUNE_SET(dst,src) \
                      (((dst) & ~0x00004000) | (((u32)(src)<<14) & 0x00004000))
/*	 Fields USB_TXHSXVTUNE	 */
#define USB_TXHSXVTUNE_WIDTH                                                  2
#define USB_TXHSXVTUNE_SHIFT                                                 12
#define USB_TXHSXVTUNE_MASK                                          0x00003000
#define USB_TXHSXVTUNE_RD(src)                       (((src) & 0x00003000)>>12)
#define USB_TXHSXVTUNE_WR(src)                  (((u32)(src)<<12) & 0x00003000)
#define USB_TXHSXVTUNE_SET(dst,src) \
                      (((dst) & ~0x00003000) | (((u32)(src)<<12) & 0x00003000))
/*	 Fields USB_COMPDISTUNE	 */
#define USB_COMPDISTUNE_WIDTH                                                 3
#define USB_COMPDISTUNE_SHIFT                                                 8
#define USB_COMPDISTUNE_MASK                                         0x00000700
#define USB_COMPDISTUNE_RD(src)                       (((src) & 0x00000700)>>8)
#define USB_COMPDISTUNE_WR(src)                  (((u32)(src)<<8) & 0x00000700)
#define USB_COMPDISTUNE_SET(dst,src) \
                       (((dst) & ~0x00000700) | (((u32)(src)<<8) & 0x00000700))
/*	 Fields USB_SQRXTUNE	 */
#define USB_SQRXTUNE_WIDTH                                                    3
#define USB_SQRXTUNE_SHIFT                                                    4
#define USB_SQRXTUNE_MASK                                            0x00000070
#define USB_SQRXTUNE_RD(src)                          (((src) & 0x00000070)>>4)
#define USB_SQRXTUNE_WR(src)                     (((u32)(src)<<4) & 0x00000070)
#define USB_SQRXTUNE_SET(dst,src) \
                       (((dst) & ~0x00000070) | (((u32)(src)<<4) & 0x00000070))
/*	 Fields USB_OTGTUNE	 */
#define USB_OTGTUNE_WIDTH                                                     3
#define USB_OTGTUNE_SHIFT                                                     0
#define USB_OTGTUNE_MASK                                             0x00000007
#define USB_OTGTUNE_RD(src)                              (((src) & 0x00000007))
#define USB_OTGTUNE_WR(src)                         (((u32)(src)) & 0x00000007)
#define USB_OTGTUNE_SET(dst,src) \
                          (((dst) & ~0x00000007) | (((u32)(src)) & 0x00000007))

/*	Register USB_RSTCTL	*/ 
/*	 Fields USB_OTGDISABLE	 */
#define USB_OTGDISABLE_WIDTH                                                  1
#define USB_OTGDISABLE_SHIFT                                                  2
#define USB_OTGDISABLE_MASK                                          0x00000004
#define USB_OTGDISABLE_RD(src)                        (((src) & 0x00000004)>>2)
#define USB_OTGDISABLE_WR(src)                   (((u32)(src)<<2) & 0x00000004)
#define USB_OTGDISABLE_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields USB_PORTRESET	 */
#define USB_PORTRESET_WIDTH                                                   1
#define USB_PORTRESET_SHIFT                                                   1
#define USB_PORTRESET_MASK                                           0x00000002
#define USB_PORTRESET_RD(src)                         (((src) & 0x00000002)>>1)
#define USB_PORTRESET_WR(src)                    (((u32)(src)<<1) & 0x00000002)
#define USB_PORTRESET_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields USB_POR	 */
#define USB_POR_WIDTH                                                         1
#define USB_POR_SHIFT                                                         0
#define USB_POR_MASK                                                 0x00000001
#define USB_POR_RD(src)                                  (((src) & 0x00000001))
#define USB_POR_WR(src)                             (((u32)(src)) & 0x00000001)
#define USB_POR_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register USB_CLKCTL	*/ 
/*	 Fields USB_SIM_SCALEDOWN	 */
#define USB_SIM_SCALEDOWN_WIDTH                                               2
#define USB_SIM_SCALEDOWN_SHIFT                                              30
#define USB_SIM_SCALEDOWN_MASK                                       0xc0000000
#define USB_SIM_SCALEDOWN_RD(src)                    (((src) & 0xc0000000)>>30)
#define USB_SIM_SCALEDOWN_WR(src)               (((u32)(src)<<30) & 0xc0000000)
#define USB_SIM_SCALEDOWN_SET(dst,src) \
                      (((dst) & ~0xc0000000) | (((u32)(src)<<30) & 0xc0000000))
/*	 Fields USB_DRVVBUS_POLARITY	 */
#define USB_DRVVBUS_POLARITY_WIDTH                                            1
#define USB_DRVVBUS_POLARITY_SHIFT                                            7
#define USB_DRVVBUS_POLARITY_MASK                                    0x00000080
#define USB_DRVVBUS_POLARITY_RD(src)                  (((src) & 0x00000080)>>7)
#define USB_DRVVBUS_POLARITY_WR(src)             (((u32)(src)<<7) & 0x00000080)
#define USB_DRVVBUS_POLARITY_SET(dst,src) \
                       (((dst) & ~0x00000080) | (((u32)(src)<<7) & 0x00000080))
/*	 Fields USB_REFCLKSEL	 */
#define USB_REFCLKSEL_WIDTH                                                   2
#define USB_REFCLKSEL_SHIFT                                                   2
#define USB_REFCLKSEL_MASK                                           0x0000000c
#define USB_REFCLKSEL_RD(src)                         (((src) & 0x0000000c)>>2)
#define USB_REFCLKSEL_WR(src)                    (((u32)(src)<<2) & 0x0000000c)
#define USB_REFCLKSEL_SET(dst,src) \
                       (((dst) & ~0x0000000c) | (((u32)(src)<<2) & 0x0000000c))
/*	 Fields USB_REFCLKDIV	 */
#define USB_REFCLKDIV_WIDTH                                                   2
#define USB_REFCLKDIV_SHIFT                                                   0
#define USB_REFCLKDIV_MASK                                           0x00000003
#define USB_REFCLKDIV_RD(src)                            (((src) & 0x00000003))
#define USB_REFCLKDIV_WR(src)                       (((u32)(src)) & 0x00000003)
#define USB_REFCLKDIV_SET(dst,src) \
                          (((dst) & ~0x00000003) | (((u32)(src)) & 0x00000003))

/*	Register SDHC_TAP_DELAY	*/ 
/*	 Fields CFG_OP_TAP_EN	 */
#define CFG_OP_TAP_EN_WIDTH                                                   1
#define CFG_OP_TAP_EN_SHIFT                                                  16
#define CFG_OP_TAP_EN_MASK                                           0x00010000
#define CFG_OP_TAP_EN_RD(src)                        (((src) & 0x00010000)>>16)
#define CFG_OP_TAP_EN_WR(src)                   (((u32)(src)<<16) & 0x00010000)
#define CFG_OP_TAP_EN_SET(dst,src) \
                      (((dst) & ~0x00010000) | (((u32)(src)<<16) & 0x00010000))
/*	 Fields CFG_OP_TAP_DELAY	 */
#define CFG_OP_TAP_DELAY_WIDTH                                                4
#define CFG_OP_TAP_DELAY_SHIFT                                               12
#define CFG_OP_TAP_DELAY_MASK                                        0x0000f000
#define CFG_OP_TAP_DELAY_RD(src)                     (((src) & 0x0000f000)>>12)
#define CFG_OP_TAP_DELAY_WR(src)                (((u32)(src)<<12) & 0x0000f000)
#define CFG_OP_TAP_DELAY_SET(dst,src) \
                      (((dst) & ~0x0000f000) | (((u32)(src)<<12) & 0x0000f000))
/*	 Fields CFG_OP_DLY_CTRL	 */
#define CFG_OP_DLY_CTRL_WIDTH                                                 2
#define CFG_OP_DLY_CTRL_SHIFT                                                10
#define CFG_OP_DLY_CTRL_MASK                                         0x00000c00
#define CFG_OP_DLY_CTRL_RD(src)                      (((src) & 0x00000c00)>>10)
#define CFG_OP_DLY_CTRL_WR(src)                 (((u32)(src)<<10) & 0x00000c00)
#define CFG_OP_DLY_CTRL_SET(dst,src) \
                      (((dst) & ~0x00000c00) | (((u32)(src)<<10) & 0x00000c00))
/*	 Fields CFG_IP_TAP_EN	 */
#define CFG_IP_TAP_EN_WIDTH                                                   1
#define CFG_IP_TAP_EN_SHIFT                                                   9
#define CFG_IP_TAP_EN_MASK                                           0x00000200
#define CFG_IP_TAP_EN_RD(src)                         (((src) & 0x00000200)>>9)
#define CFG_IP_TAP_EN_WR(src)                    (((u32)(src)<<9) & 0x00000200)
#define CFG_IP_TAP_EN_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields CFG_IP_TAP_DELAY	 */
#define CFG_IP_TAP_DELAY_WIDTH                                                6
#define CFG_IP_TAP_DELAY_SHIFT                                                3
#define CFG_IP_TAP_DELAY_MASK                                        0x000001f8
#define CFG_IP_TAP_DELAY_RD(src)                      (((src) & 0x000001f8)>>3)
#define CFG_IP_TAP_DELAY_WR(src)                 (((u32)(src)<<3) & 0x000001f8)
#define CFG_IP_TAP_DELAY_SET(dst,src) \
                       (((dst) & ~0x000001f8) | (((u32)(src)<<3) & 0x000001f8))
/*	 Fields CFG_IP_DLY_CTRL	 */
#define CFG_IP_DLY_CTRL_WIDTH                                                 2
#define CFG_IP_DLY_CTRL_SHIFT                                                 1
#define CFG_IP_DLY_CTRL_MASK                                         0x00000006
#define CFG_IP_DLY_CTRL_RD(src)                       (((src) & 0x00000006)>>1)
#define CFG_IP_DLY_CTRL_WR(src)                  (((u32)(src)<<1) & 0x00000006)
#define CFG_IP_DLY_CTRL_SET(dst,src) \
                       (((dst) & ~0x00000006) | (((u32)(src)<<1) & 0x00000006))
/*	 Fields CFG_CMD_CONFLICT_DIS	 */
#define CFG_CMD_CONFLICT_DIS_WIDTH                                            1
#define CFG_CMD_CONFLICT_DIS_SHIFT                                            0
#define CFG_CMD_CONFLICT_DIS_MASK                                    0x00000001
#define CFG_CMD_CONFLICT_DIS_RD(src)                     (((src) & 0x00000001))
#define CFG_CMD_CONFLICT_DIS_WR(src)                (((u32)(src)) & 0x00000001)
#define CFG_CMD_CONFLICT_DIS_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register SDHC_CAP_REG_LOW_S1	*/ 
/*	 Fields SDHC_CAP_REG_LOW_S1	 */
#define SDHC_CAP_REG_LOW_S11_WIDTH                                           32
#define SDHC_CAP_REG_LOW_S11_SHIFT                                            0
#define SDHC_CAP_REG_LOW_S11_MASK                                    0xffffffff
#define SDHC_CAP_REG_LOW_S11_RD(src)                     (((src) & 0xffffffff))
#define SDHC_CAP_REG_LOW_S11_WR(src)                (((u32)(src)) & 0xffffffff)
#define SDHC_CAP_REG_LOW_S11_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register SDHC_CAP_REG_HIGH_S1	*/ 
/*	 Fields SDHC_CAP_REG_HIGH_S1	 */
#define SDHC_CAP_REG_HIGH_S11_WIDTH                                          20
#define SDHC_CAP_REG_HIGH_S11_SHIFT                                           0
#define SDHC_CAP_REG_HIGH_S11_MASK                                   0x000fffff
#define SDHC_CAP_REG_HIGH_S11_RD(src)                    (((src) & 0x000fffff))
#define SDHC_CAP_REG_HIGH_S11_WR(src)               (((u32)(src)) & 0x000fffff)
#define SDHC_CAP_REG_HIGH_S11_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register SDHC_CAP_REG_LOW_S2	*/ 
/*	 Fields SDHC_CAP_REG_LOW_S2	 */
#define SDHC_CAP_REG_LOW_S22_WIDTH                                           32
#define SDHC_CAP_REG_LOW_S22_SHIFT                                            0
#define SDHC_CAP_REG_LOW_S22_MASK                                    0xffffffff
#define SDHC_CAP_REG_LOW_S22_RD(src)                     (((src) & 0xffffffff))
#define SDHC_CAP_REG_LOW_S22_WR(src)                (((u32)(src)) & 0xffffffff)
#define SDHC_CAP_REG_LOW_S22_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register SDHC_CAP_REG_HIGH_S2	*/ 
/*	 Fields SDHC_CAP_REG_HIGH_S2	 */
#define SDHC_CAP_REG_HIGH_S22_WIDTH                                          20
#define SDHC_CAP_REG_HIGH_S22_SHIFT                                           0
#define SDHC_CAP_REG_HIGH_S22_MASK                                   0x000fffff
#define SDHC_CAP_REG_HIGH_S22_RD(src)                    (((src) & 0x000fffff))
#define SDHC_CAP_REG_HIGH_S22_WR(src)               (((u32)(src)) & 0x000fffff)
#define SDHC_CAP_REG_HIGH_S22_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register SDHC_INIT_PRESET	*/ 
/*	 Fields INIT_PRESET_S1	 */
#define INIT_PRESET_S1_WIDTH                                                 13
#define INIT_PRESET_S1_SHIFT                                                 13
#define INIT_PRESET_S1_MASK                                          0x03ffe000
#define INIT_PRESET_S1_RD(src)                       (((src) & 0x03ffe000)>>13)
#define INIT_PRESET_S1_WR(src)                  (((u32)(src)<<13) & 0x03ffe000)
#define INIT_PRESET_S1_SET(dst,src) \
                      (((dst) & ~0x03ffe000) | (((u32)(src)<<13) & 0x03ffe000))
/*	 Fields INIT_PRESET_S2	 */
#define INIT_PRESET_S2_WIDTH                                                 13
#define INIT_PRESET_S2_SHIFT                                                  0
#define INIT_PRESET_S2_MASK                                          0x00001fff
#define INIT_PRESET_S2_RD(src)                           (((src) & 0x00001fff))
#define INIT_PRESET_S2_WR(src)                      (((u32)(src)) & 0x00001fff)
#define INIT_PRESET_S2_SET(dst,src) \
                          (((dst) & ~0x00001fff) | (((u32)(src)) & 0x00001fff))

/*	Register SDHC_DEFAULT_PRESET	*/ 
/*	 Fields DEFAULT_PRESET_S1	 */
#define DEFAULT_PRESET_S1_WIDTH                                              13
#define DEFAULT_PRESET_S1_SHIFT                                              13
#define DEFAULT_PRESET_S1_MASK                                       0x03ffe000
#define DEFAULT_PRESET_S1_RD(src)                    (((src) & 0x03ffe000)>>13)
#define DEFAULT_PRESET_S1_WR(src)               (((u32)(src)<<13) & 0x03ffe000)
#define DEFAULT_PRESET_S1_SET(dst,src) \
                      (((dst) & ~0x03ffe000) | (((u32)(src)<<13) & 0x03ffe000))
/*	 Fields DEFAULT_PRESET_S2	 */
#define DEFAULT_PRESET_S2_WIDTH                                              13
#define DEFAULT_PRESET_S2_SHIFT                                               0
#define DEFAULT_PRESET_S2_MASK                                       0x00001fff
#define DEFAULT_PRESET_S2_RD(src)                        (((src) & 0x00001fff))
#define DEFAULT_PRESET_S2_WR(src)                   (((u32)(src)) & 0x00001fff)
#define DEFAULT_PRESET_S2_SET(dst,src) \
                          (((dst) & ~0x00001fff) | (((u32)(src)) & 0x00001fff))

/*	Register SDHC_HIGH_SPEED_PRESET	*/ 
/*	 Fields HIGH_SPEED_PRESET_S1	 */
#define HIGH_SPEED_PRESET_S1_WIDTH                                           13
#define HIGH_SPEED_PRESET_S1_SHIFT                                           13
#define HIGH_SPEED_PRESET_S1_MASK                                    0x03ffe000
#define HIGH_SPEED_PRESET_S1_RD(src)                 (((src) & 0x03ffe000)>>13)
#define HIGH_SPEED_PRESET_S1_WR(src)            (((u32)(src)<<13) & 0x03ffe000)
#define HIGH_SPEED_PRESET_S1_SET(dst,src) \
                      (((dst) & ~0x03ffe000) | (((u32)(src)<<13) & 0x03ffe000))
/*	 Fields HIGH_SPEED_PRESET_S2	 */
#define HIGH_SPEED_PRESET_S2_WIDTH                                           13
#define HIGH_SPEED_PRESET_S2_SHIFT                                            0
#define HIGH_SPEED_PRESET_S2_MASK                                    0x00001fff
#define HIGH_SPEED_PRESET_S2_RD(src)                     (((src) & 0x00001fff))
#define HIGH_SPEED_PRESET_S2_WR(src)                (((u32)(src)) & 0x00001fff)
#define HIGH_SPEED_PRESET_S2_SET(dst,src) \
                          (((dst) & ~0x00001fff) | (((u32)(src)) & 0x00001fff))

/*	Register SDHC_SDR12_PRESET	*/ 
/*	 Fields SDR12_PRESET_S1	 */
#define SDR12_PRESET_S1_WIDTH                                                13
#define SDR12_PRESET_S1_SHIFT                                                13
#define SDR12_PRESET_S1_MASK                                         0x03ffe000
#define SDR12_PRESET_S1_RD(src)                      (((src) & 0x03ffe000)>>13)
#define SDR12_PRESET_S1_WR(src)                 (((u32)(src)<<13) & 0x03ffe000)
#define SDR12_PRESET_S1_SET(dst,src) \
                      (((dst) & ~0x03ffe000) | (((u32)(src)<<13) & 0x03ffe000))
/*	 Fields SDR12_PRESET_S2	 */
#define SDR12_PRESET_S2_WIDTH                                                13
#define SDR12_PRESET_S2_SHIFT                                                 0
#define SDR12_PRESET_S2_MASK                                         0x00001fff
#define SDR12_PRESET_S2_RD(src)                          (((src) & 0x00001fff))
#define SDR12_PRESET_S2_WR(src)                     (((u32)(src)) & 0x00001fff)
#define SDR12_PRESET_S2_SET(dst,src) \
                          (((dst) & ~0x00001fff) | (((u32)(src)) & 0x00001fff))

/*	Register SDHC_SDR25_PRESET	*/ 
/*	 Fields SDR25_PRESET_S1	 */
#define SDR25_PRESET_S1_WIDTH                                                13
#define SDR25_PRESET_S1_SHIFT                                                13
#define SDR25_PRESET_S1_MASK                                         0x03ffe000
#define SDR25_PRESET_S1_RD(src)                      (((src) & 0x03ffe000)>>13)
#define SDR25_PRESET_S1_WR(src)                 (((u32)(src)<<13) & 0x03ffe000)
#define SDR25_PRESET_S1_SET(dst,src) \
                      (((dst) & ~0x03ffe000) | (((u32)(src)<<13) & 0x03ffe000))
/*	 Fields SDR25_PRESET_S2	 */
#define SDR25_PRESET_S2_WIDTH                                                13
#define SDR25_PRESET_S2_SHIFT                                                 0
#define SDR25_PRESET_S2_MASK                                         0x00001fff
#define SDR25_PRESET_S2_RD(src)                          (((src) & 0x00001fff))
#define SDR25_PRESET_S2_WR(src)                     (((u32)(src)) & 0x00001fff)
#define SDR25_PRESET_S2_SET(dst,src) \
                          (((dst) & ~0x00001fff) | (((u32)(src)) & 0x00001fff))

/*	Register SDHC_SDR50_PRESET	*/ 
/*	 Fields SDR50_PRESET_S1	 */
#define SDR50_PRESET_S1_WIDTH                                                13
#define SDR50_PRESET_S1_SHIFT                                                13
#define SDR50_PRESET_S1_MASK                                         0x03ffe000
#define SDR50_PRESET_S1_RD(src)                      (((src) & 0x03ffe000)>>13)
#define SDR50_PRESET_S1_WR(src)                 (((u32)(src)<<13) & 0x03ffe000)
#define SDR50_PRESET_S1_SET(dst,src) \
                      (((dst) & ~0x03ffe000) | (((u32)(src)<<13) & 0x03ffe000))
/*	 Fields SDR50_PRESET_S2	 */
#define SDR50_PRESET_S2_WIDTH                                                13
#define SDR50_PRESET_S2_SHIFT                                                 0
#define SDR50_PRESET_S2_MASK                                         0x00001fff
#define SDR50_PRESET_S2_RD(src)                          (((src) & 0x00001fff))
#define SDR50_PRESET_S2_WR(src)                     (((u32)(src)) & 0x00001fff)
#define SDR50_PRESET_S2_SET(dst,src) \
                          (((dst) & ~0x00001fff) | (((u32)(src)) & 0x00001fff))

/*	Register SDHC_SDR104_PRESET	*/ 
/*	 Fields SDR104_PRESET_S1	 */
#define SDR104_PRESET_S1_WIDTH                                               13
#define SDR104_PRESET_S1_SHIFT                                               13
#define SDR104_PRESET_S1_MASK                                        0x03ffe000
#define SDR104_PRESET_S1_RD(src)                     (((src) & 0x03ffe000)>>13)
#define SDR104_PRESET_S1_WR(src)                (((u32)(src)<<13) & 0x03ffe000)
#define SDR104_PRESET_S1_SET(dst,src) \
                      (((dst) & ~0x03ffe000) | (((u32)(src)<<13) & 0x03ffe000))
/*	 Fields SDR104_PRESET_S2	 */
#define SDR104_PRESET_S2_WIDTH                                               13
#define SDR104_PRESET_S2_SHIFT                                                0
#define SDR104_PRESET_S2_MASK                                        0x00001fff
#define SDR104_PRESET_S2_RD(src)                         (((src) & 0x00001fff))
#define SDR104_PRESET_S2_WR(src)                    (((u32)(src)) & 0x00001fff)
#define SDR104_PRESET_S2_SET(dst,src) \
                          (((dst) & ~0x00001fff) | (((u32)(src)) & 0x00001fff))

/*	Register SDHC_DDR50_PRESET	*/ 
/*	 Fields DDR50_PRESET_S1	 */
#define DDR50_PRESET_S1_WIDTH                                                13
#define DDR50_PRESET_S1_SHIFT                                                13
#define DDR50_PRESET_S1_MASK                                         0x03ffe000
#define DDR50_PRESET_S1_RD(src)                      (((src) & 0x03ffe000)>>13)
#define DDR50_PRESET_S1_WR(src)                 (((u32)(src)<<13) & 0x03ffe000)
#define DDR50_PRESET_S1_SET(dst,src) \
                      (((dst) & ~0x03ffe000) | (((u32)(src)<<13) & 0x03ffe000))
/*	 Fields DDR50_PRESET_S2	 */
#define DDR50_PRESET_S2_WIDTH                                                13
#define DDR50_PRESET_S2_SHIFT                                                 0
#define DDR50_PRESET_S2_MASK                                         0x00001fff
#define DDR50_PRESET_S2_RD(src)                          (((src) & 0x00001fff))
#define DDR50_PRESET_S2_WR(src)                     (((u32)(src)) & 0x00001fff)
#define DDR50_PRESET_S2_SET(dst,src) \
                          (((dst) & ~0x00001fff) | (((u32)(src)) & 0x00001fff))

/*	Register USBHost_Cfg	*/ 
/*	 Fields usbh_word_if	 */
#define USBH_WORD_IF_WIDTH                                                    1
#define USBH_WORD_IF_SHIFT                                                   12
#define USBH_WORD_IF_MASK                                            0x00001000
#define USBH_WORD_IF_RD(src)                         (((src) & 0x00001000)>>12)
#define USBH_WORD_IF_WR(src)                    (((u32)(src)<<12) & 0x00001000)
#define USBH_WORD_IF_SET(dst,src) \
                      (((dst) & ~0x00001000) | (((u32)(src)<<12) & 0x00001000))
/*	 Fields usbh_ohci_susp_lgcy_i	 */
#define USBH_OHCI_SUSP_LGCY_I_WIDTH                                           1
#define USBH_OHCI_SUSP_LGCY_I_SHIFT                                          11
#define USBH_OHCI_SUSP_LGCY_I_MASK                                   0x00000800
#define USBH_OHCI_SUSP_LGCY_I_RD(src)                (((src) & 0x00000800)>>11)
#define USBH_OHCI_SUSP_LGCY_I_WR(src)           (((u32)(src)<<11) & 0x00000800)
#define USBH_OHCI_SUSP_LGCY_I_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*	 Fields usbh_app_start_clk_i	 */
#define USBH_APP_START_CLK_I_WIDTH                                            1
#define USBH_APP_START_CLK_I_SHIFT                                           10
#define USBH_APP_START_CLK_I_MASK                                    0x00000400
#define USBH_APP_START_CLK_I_RD(src)                 (((src) & 0x00000400)>>10)
#define USBH_APP_START_CLK_I_WR(src)            (((u32)(src)<<10) & 0x00000400)
#define USBH_APP_START_CLK_I_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields usbh_autoppd	 */
#define USBH_AUTOPPD_WIDTH                                                    1
#define USBH_AUTOPPD_SHIFT                                                    9
#define USBH_AUTOPPD_MASK                                            0x00000200
#define USBH_AUTOPPD_RD(src)                          (((src) & 0x00000200)>>9)
#define USBH_AUTOPPD_WR(src)                     (((u32)(src)<<9) & 0x00000200)
#define USBH_AUTOPPD_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields usbh_frame_len	 */
#define USBH_FRAME_LEN_WIDTH                                                  6
#define USBH_FRAME_LEN_SHIFT                                                  3
#define USBH_FRAME_LEN_MASK                                          0x000001f8
#define USBH_FRAME_LEN_RD(src)                        (((src) & 0x000001f8)>>3)
#define USBH_FRAME_LEN_WR(src)                   (((u32)(src)<<3) & 0x000001f8)
#define USBH_FRAME_LEN_SET(dst,src) \
                       (((dst) & ~0x000001f8) | (((u32)(src)<<3) & 0x000001f8))
/*	 Fields usbh_sim_mode	 */
#define AHBC_USBH_SIM_MODE_WIDTH                                              1
#define AHBC_USBH_SIM_MODE_SHIFT                                              2
#define AHBC_USBH_SIM_MODE_MASK                                      0x00000004
#define AHBC_USBH_SIM_MODE_RD(src)                    (((src) & 0x00000004)>>2)
#define AHBC_USBH_SIM_MODE_WR(src)               (((u32)(src)<<2) & 0x00000004)
#define AHBC_USBH_SIM_MODE_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields ss_resume_utmi_pls_dis	 */
#define SS_RESUME_UTMI_PLS_DIS_WIDTH                                          1
#define SS_RESUME_UTMI_PLS_DIS_SHIFT                                          1
#define SS_RESUME_UTMI_PLS_DIS_MASK                                  0x00000002
#define SS_RESUME_UTMI_PLS_DIS_RD(src)                (((src) & 0x00000002)>>1)
#define SS_RESUME_UTMI_PLS_DIS_WR(src)           (((u32)(src)<<1) & 0x00000002)
#define SS_RESUME_UTMI_PLS_DIS_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields ss_utmi_backward_enb	 */
#define SS_UTMI_BACKWARD_ENB_WIDTH                                            1
#define SS_UTMI_BACKWARD_ENB_SHIFT                                            0
#define SS_UTMI_BACKWARD_ENB_MASK                                    0x00000001
#define SS_UTMI_BACKWARD_ENB_RD(src)                     (((src) & 0x00000001))
#define SS_UTMI_BACKWARD_ENB_WR(src)                (((u32)(src)) & 0x00000001)
#define SS_UTMI_BACKWARD_ENB_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register USBHost_Status	*/ 
/*	 Fields ohci_rmtwkp_status	 */
#define OHCI_RMTWKP_STATUS_WIDTH                                              1
#define OHCI_RMTWKP_STATUS_SHIFT                                              0
#define OHCI_RMTWKP_STATUS_MASK                                      0x00000001
#define OHCI_RMTWKP_STATUS_RD(src)                       (((src) & 0x00000001))
#define OHCI_RMTWKP_STATUS_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register USBHost_Fatal_CFG	*/ 
/*	 Fields CFG_usbh_sys_intr	 */
#define CFG_USBH_SYS_INTR_WIDTH                                               1
#define CFG_USBH_SYS_INTR_SHIFT                                               0
#define CFG_USBH_SYS_INTR_MASK                                       0x00000001
#define CFG_USBH_SYS_INTR_RD(src)                        (((src) & 0x00000001))
#define CFG_USBH_SYS_INTR_WR(src)                   (((u32)(src)) & 0x00000001)
#define CFG_USBH_SYS_INTR_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register SDR_HDLC_AHB2AXI_CFG	*/ 
/*	 Fields pw_axi_sideband_prio	 */
#define PW_AXI_SIDEBAND_PRIO_F2_WIDTH                                         3
#define PW_AXI_SIDEBAND_PRIO_F2_SHIFT                                        21
#define PW_AXI_SIDEBAND_PRIO_F2_MASK                                 0x00e00000
#define PW_AXI_SIDEBAND_PRIO_F2_RD(src)              (((src) & 0x00e00000)>>21)
#define PW_AXI_SIDEBAND_PRIO_F2_WR(src)         (((u32)(src)<<21) & 0x00e00000)
#define PW_AXI_SIDEBAND_PRIO_F2_SET(dst,src) \
                      (((dst) & ~0x00e00000) | (((u32)(src)<<21) & 0x00e00000))
/*	 Fields pr_axi_sideband_prio	 */
#define PR_AXI_SIDEBAND_PRIO_F2_WIDTH                                         3
#define PR_AXI_SIDEBAND_PRIO_F2_SHIFT                                        17
#define PR_AXI_SIDEBAND_PRIO_F2_MASK                                 0x000e0000
#define PR_AXI_SIDEBAND_PRIO_F2_RD(src)              (((src) & 0x000e0000)>>17)
#define PR_AXI_SIDEBAND_PRIO_F2_WR(src)         (((u32)(src)<<17) & 0x000e0000)
#define PR_AXI_SIDEBAND_PRIO_F2_SET(dst,src) \
                      (((dst) & ~0x000e0000) | (((u32)(src)<<17) & 0x000e0000))
/*	 Fields writes_block_reads	 */
#define WRITES_BLOCK_READS_F2_WIDTH                                           1
#define WRITES_BLOCK_READS_F2_SHIFT                                          15
#define WRITES_BLOCK_READS_F2_MASK                                   0x00008000
#define WRITES_BLOCK_READS_F2_RD(src)                (((src) & 0x00008000)>>15)
#define WRITES_BLOCK_READS_F2_WR(src)           (((u32)(src)<<15) & 0x00008000)
#define WRITES_BLOCK_READS_F2_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))
/*	 Fields pw_max_outstanding	 */
#define PW_MAX_OUTSTANDING_F2_WIDTH                                           4
#define PW_MAX_OUTSTANDING_F2_SHIFT                                           8
#define PW_MAX_OUTSTANDING_F2_MASK                                   0x00000f00
#define PW_MAX_OUTSTANDING_F2_RD(src)                 (((src) & 0x00000f00)>>8)
#define PW_MAX_OUTSTANDING_F2_WR(src)            (((u32)(src)<<8) & 0x00000f00)
#define PW_MAX_OUTSTANDING_F2_SET(dst,src) \
                       (((dst) & ~0x00000f00) | (((u32)(src)<<8) & 0x00000f00))
/*	 Fields tx_wrr_weight	 */
#define TX_WRR_WEIGHT_F2_WIDTH                                                4
#define TX_WRR_WEIGHT_F2_SHIFT                                                4
#define TX_WRR_WEIGHT_F2_MASK                                        0x000000f0
#define TX_WRR_WEIGHT_F2_RD(src)                      (((src) & 0x000000f0)>>4)
#define TX_WRR_WEIGHT_F2_WR(src)                 (((u32)(src)<<4) & 0x000000f0)
#define TX_WRR_WEIGHT_F2_SET(dst,src) \
                       (((dst) & ~0x000000f0) | (((u32)(src)<<4) & 0x000000f0))
/*	 Fields rx_wrr_weight	 */
#define RX_WRR_WEIGHT_F2_WIDTH                                                4
#define RX_WRR_WEIGHT_F2_SHIFT                                                0
#define RX_WRR_WEIGHT_F2_MASK                                        0x0000000f
#define RX_WRR_WEIGHT_F2_RD(src)                         (((src) & 0x0000000f))
#define RX_WRR_WEIGHT_F2_WR(src)                    (((u32)(src)) & 0x0000000f)
#define RX_WRR_WEIGHT_F2_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register SDR_EHCI_0_AHB2AXI_CFG	*/ 
/*	 Fields pw_axi_sideband_prio	 */
#define PW_AXI_SIDEBAND_PRIO_F3_WIDTH                                         3
#define PW_AXI_SIDEBAND_PRIO_F3_SHIFT                                        21
#define PW_AXI_SIDEBAND_PRIO_F3_MASK                                 0x00e00000
#define PW_AXI_SIDEBAND_PRIO_F3_RD(src)              (((src) & 0x00e00000)>>21)
#define PW_AXI_SIDEBAND_PRIO_F3_WR(src)         (((u32)(src)<<21) & 0x00e00000)
#define PW_AXI_SIDEBAND_PRIO_F3_SET(dst,src) \
                      (((dst) & ~0x00e00000) | (((u32)(src)<<21) & 0x00e00000))
/*	 Fields pr_axi_sideband_prio	 */
#define PR_AXI_SIDEBAND_PRIO_F3_WIDTH                                         3
#define PR_AXI_SIDEBAND_PRIO_F3_SHIFT                                        17
#define PR_AXI_SIDEBAND_PRIO_F3_MASK                                 0x000e0000
#define PR_AXI_SIDEBAND_PRIO_F3_RD(src)              (((src) & 0x000e0000)>>17)
#define PR_AXI_SIDEBAND_PRIO_F3_WR(src)         (((u32)(src)<<17) & 0x000e0000)
#define PR_AXI_SIDEBAND_PRIO_F3_SET(dst,src) \
                      (((dst) & ~0x000e0000) | (((u32)(src)<<17) & 0x000e0000))
/*	 Fields writes_block_reads	 */
#define WRITES_BLOCK_READS_F3_WIDTH                                           1
#define WRITES_BLOCK_READS_F3_SHIFT                                          15
#define WRITES_BLOCK_READS_F3_MASK                                   0x00008000
#define WRITES_BLOCK_READS_F3_RD(src)                (((src) & 0x00008000)>>15)
#define WRITES_BLOCK_READS_F3_WR(src)           (((u32)(src)<<15) & 0x00008000)
#define WRITES_BLOCK_READS_F3_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))
/*	 Fields pw_max_outstanding	 */
#define PW_MAX_OUTSTANDING_F3_WIDTH                                           4
#define PW_MAX_OUTSTANDING_F3_SHIFT                                           8
#define PW_MAX_OUTSTANDING_F3_MASK                                   0x00000f00
#define PW_MAX_OUTSTANDING_F3_RD(src)                 (((src) & 0x00000f00)>>8)
#define PW_MAX_OUTSTANDING_F3_WR(src)            (((u32)(src)<<8) & 0x00000f00)
#define PW_MAX_OUTSTANDING_F3_SET(dst,src) \
                       (((dst) & ~0x00000f00) | (((u32)(src)<<8) & 0x00000f00))
/*	 Fields tx_wrr_weight	 */
#define TX_WRR_WEIGHT_F3_WIDTH                                                4
#define TX_WRR_WEIGHT_F3_SHIFT                                                4
#define TX_WRR_WEIGHT_F3_MASK                                        0x000000f0
#define TX_WRR_WEIGHT_F3_RD(src)                      (((src) & 0x000000f0)>>4)
#define TX_WRR_WEIGHT_F3_WR(src)                 (((u32)(src)<<4) & 0x000000f0)
#define TX_WRR_WEIGHT_F3_SET(dst,src) \
                       (((dst) & ~0x000000f0) | (((u32)(src)<<4) & 0x000000f0))
/*	 Fields rx_wrr_weight	 */
#define RX_WRR_WEIGHT_F3_WIDTH                                                4
#define RX_WRR_WEIGHT_F3_SHIFT                                                0
#define RX_WRR_WEIGHT_F3_MASK                                        0x0000000f
#define RX_WRR_WEIGHT_F3_RD(src)                         (((src) & 0x0000000f))
#define RX_WRR_WEIGHT_F3_WR(src)                    (((u32)(src)) & 0x0000000f)
#define RX_WRR_WEIGHT_F3_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register CFG_X2H_BRIDGE	*/ 
/*	 Fields CFG_X2H_RD_USE_DEFINED_ONLY	 */
#define CFG_X2H_RD_USE_DEFINED_ONLY_WIDTH                                     1
#define CFG_X2H_RD_USE_DEFINED_ONLY_SHIFT                                     1
#define CFG_X2H_RD_USE_DEFINED_ONLY_MASK                             0x00000002
#define CFG_X2H_RD_USE_DEFINED_ONLY_RD(src)           (((src) & 0x00000002)>>1)
#define CFG_X2H_RD_USE_DEFINED_ONLY_WR(src)      (((u32)(src)<<1) & 0x00000002)
#define CFG_X2H_RD_USE_DEFINED_ONLY_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields CFG_X2H_USE_DEFINED_ONLY	 */
#define CFG_X2H_USE_DEFINED_ONLY_WIDTH                                        1
#define CFG_X2H_USE_DEFINED_ONLY_SHIFT                                        0
#define CFG_X2H_USE_DEFINED_ONLY_MASK                                0x00000001
#define CFG_X2H_USE_DEFINED_ONLY_RD(src)                 (((src) & 0x00000001))
#define CFG_X2H_USE_DEFINED_ONLY_WR(src)            (((u32)(src)) & 0x00000001)
#define CFG_X2H_USE_DEFINED_ONLY_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register ENABLE_INTF	*/ 
/*	 Fields CFG_AHB_APB_CLK_RATIO	 */
#define CFG_AHB_APB_CLK_RATIO_WIDTH                                           2
#define CFG_AHB_APB_CLK_RATIO_SHIFT                                          30
#define CFG_AHB_APB_CLK_RATIO_MASK                                   0xc0000000
#define CFG_AHB_APB_CLK_RATIO_RD(src)                (((src) & 0xc0000000)>>30)
#define CFG_AHB_APB_CLK_RATIO_WR(src)           (((u32)(src)<<30) & 0xc0000000)
#define CFG_AHB_APB_CLK_RATIO_SET(dst,src) \
                      (((dst) & ~0xc0000000) | (((u32)(src)<<30) & 0xc0000000))
/*	 Fields CFG_BE_FOR_RD	 */
#define CFG_BE_FOR_RD_WIDTH                                                   6
#define CFG_BE_FOR_RD_SHIFT                                                  24
#define CFG_BE_FOR_RD_MASK                                           0x3f000000
#define CFG_BE_FOR_RD_RD(src)                        (((src) & 0x3f000000)>>24)
#define CFG_BE_FOR_RD_WR(src)                   (((u32)(src)<<24) & 0x3f000000)
#define CFG_BE_FOR_RD_SET(dst,src) \
                      (((dst) & ~0x3f000000) | (((u32)(src)<<24) & 0x3f000000))
/*	 Fields CFG_TH	 */
#define CFG_TH_WIDTH                                                         18
#define CFG_TH_SHIFT                                                          6
#define CFG_TH_MASK                                                  0x00ffffc0
#define CFG_TH_RD(src)                                (((src) & 0x00ffffc0)>>6)
#define CFG_TH_WR(src)                           (((u32)(src)<<6) & 0x00ffffc0)
#define CFG_TH_SET(dst,src) \
                       (((dst) & ~0x00ffffc0) | (((u32)(src)<<6) & 0x00ffffc0))
/*	 Fields CFG_SRAM_MUX	 */
#define CFG_SRAM_MUX_WIDTH                                                    6
#define CFG_SRAM_MUX_SHIFT                                                    0
#define CFG_SRAM_MUX_MASK                                            0x0000003f
#define CFG_SRAM_MUX_RD(src)                             (((src) & 0x0000003f))
#define CFG_SRAM_MUX_WR(src)                        (((u32)(src)) & 0x0000003f)
#define CFG_SRAM_MUX_SET(dst,src) \
                          (((dst) & ~0x0000003f) | (((u32)(src)) & 0x0000003f))

/*	Register USBH_DESCRAM	*/ 
/*	 Fields TEST1A	 */
#define TEST1A_WIDTH                                                          1
#define TEST1A_SHIFT                                                          6
#define TEST1A_MASK                                                  0x00000040
#define TEST1A_RD(src)                                (((src) & 0x00000040)>>6)
#define TEST1A_WR(src)                           (((u32)(src)<<6) & 0x00000040)
#define TEST1A_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*	 Fields RMEA	 */
#define AHBC_RMEA_WIDTH                                                       1
#define AHBC_RMEA_SHIFT                                                       5
#define AHBC_RMEA_MASK                                               0x00000020
#define AHBC_RMEA_RD(src)                             (((src) & 0x00000020)>>5)
#define AHBC_RMEA_WR(src)                        (((u32)(src)<<5) & 0x00000020)
#define AHBC_RMEA_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields RMA	 */
#define AHBC_RMA_WIDTH                                                        4
#define AHBC_RMA_SHIFT                                                        1
#define AHBC_RMA_MASK                                                0x0000001e
#define AHBC_RMA_RD(src)                              (((src) & 0x0000001e)>>1)
#define AHBC_RMA_WR(src)                         (((u32)(src)<<1) & 0x0000001e)
#define AHBC_RMA_SET(dst,src) \
                       (((dst) & ~0x0000001e) | (((u32)(src)<<1) & 0x0000001e))
/*	 Fields PWRDN_DIS	 */
#define PWRDN_DIS_WIDTH                                                       1
#define PWRDN_DIS_SHIFT                                                       0
#define PWRDN_DIS_MASK                                               0x00000001
#define PWRDN_DIS_RD(src)                                (((src) & 0x00000001))
#define PWRDN_DIS_WR(src)                           (((u32)(src)) & 0x00000001)
#define PWRDN_DIS_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register USBH_DATARAM	*/ 
/*	 Fields TEST1A	 */
#define TEST1A_F1_WIDTH                                                       1
#define TEST1A_F1_SHIFT                                                       6
#define TEST1A_F1_MASK                                               0x00000040
#define TEST1A_F1_RD(src)                             (((src) & 0x00000040)>>6)
#define TEST1A_F1_WR(src)                        (((u32)(src)<<6) & 0x00000040)
#define TEST1A_F1_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*	 Fields RMEA	 */
#define AHBC_RMEA_F1_WIDTH                                                    1
#define AHBC_RMEA_F1_SHIFT                                                    5
#define AHBC_RMEA_F1_MASK                                            0x00000020
#define AHBC_RMEA_F1_RD(src)                          (((src) & 0x00000020)>>5)
#define AHBC_RMEA_F1_WR(src)                     (((u32)(src)<<5) & 0x00000020)
#define AHBC_RMEA_F1_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields RMA	 */
#define AHBC_RMA_F1_WIDTH                                                     4
#define AHBC_RMA_F1_SHIFT                                                     1
#define AHBC_RMA_F1_MASK                                             0x0000001e
#define AHBC_RMA_F1_RD(src)                           (((src) & 0x0000001e)>>1)
#define AHBC_RMA_F1_WR(src)                      (((u32)(src)<<1) & 0x0000001e)
#define AHBC_RMA_F1_SET(dst,src) \
                       (((dst) & ~0x0000001e) | (((u32)(src)<<1) & 0x0000001e))
/*	 Fields PWRDN_DIS	 */
#define PWRDN_DIS_F1_WIDTH                                                    1
#define PWRDN_DIS_F1_SHIFT                                                    0
#define PWRDN_DIS_F1_MASK                                            0x00000001
#define PWRDN_DIS_F1_RD(src)                             (((src) & 0x00000001))
#define PWRDN_DIS_F1_WR(src)                        (((u32)(src)) & 0x00000001)
#define PWRDN_DIS_F1_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register SDHC_DATARAM	*/ 
/*	 Fields TEST1B	 */
#define TEST1B_WIDTH                                                          1
#define TEST1B_SHIFT                                                         12
#define TEST1B_MASK                                                  0x00001000
#define TEST1B_RD(src)                               (((src) & 0x00001000)>>12)
#define TEST1B_WR(src)                          (((u32)(src)<<12) & 0x00001000)
#define TEST1B_SET(dst,src) \
                      (((dst) & ~0x00001000) | (((u32)(src)<<12) & 0x00001000))
/*	 Fields TEST1A	 */
#define TEST1A_F2_WIDTH                                                       1
#define TEST1A_F2_SHIFT                                                      11
#define TEST1A_F2_MASK                                               0x00000800
#define TEST1A_F2_RD(src)                            (((src) & 0x00000800)>>11)
#define TEST1A_F2_WR(src)                       (((u32)(src)<<11) & 0x00000800)
#define TEST1A_F2_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*	 Fields RMEB	 */
#define AHBC_RMEB_WIDTH                                                       1
#define AHBC_RMEB_SHIFT                                                      10
#define AHBC_RMEB_MASK                                               0x00000400
#define AHBC_RMEB_RD(src)                            (((src) & 0x00000400)>>10)
#define AHBC_RMEB_WR(src)                       (((u32)(src)<<10) & 0x00000400)
#define AHBC_RMEB_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields RMEA	 */
#define AHBC_RMEA_F2_WIDTH                                                    1
#define AHBC_RMEA_F2_SHIFT                                                    9
#define AHBC_RMEA_F2_MASK                                            0x00000200
#define AHBC_RMEA_F2_RD(src)                          (((src) & 0x00000200)>>9)
#define AHBC_RMEA_F2_WR(src)                     (((u32)(src)<<9) & 0x00000200)
#define AHBC_RMEA_F2_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields RMB	 */
#define AHBC_RMB_WIDTH                                                        4
#define AHBC_RMB_SHIFT                                                        5
#define AHBC_RMB_MASK                                                0x000001e0
#define AHBC_RMB_RD(src)                              (((src) & 0x000001e0)>>5)
#define AHBC_RMB_WR(src)                         (((u32)(src)<<5) & 0x000001e0)
#define AHBC_RMB_SET(dst,src) \
                       (((dst) & ~0x000001e0) | (((u32)(src)<<5) & 0x000001e0))
/*	 Fields RMA	 */
#define AHBC_RMA_F2_WIDTH                                                     4
#define AHBC_RMA_F2_SHIFT                                                     1
#define AHBC_RMA_F2_MASK                                             0x0000001e
#define AHBC_RMA_F2_RD(src)                           (((src) & 0x0000001e)>>1)
#define AHBC_RMA_F2_WR(src)                      (((u32)(src)<<1) & 0x0000001e)
#define AHBC_RMA_F2_SET(dst,src) \
                       (((dst) & ~0x0000001e) | (((u32)(src)<<1) & 0x0000001e))
/*	 Fields PWRDN_DIS	 */
#define PWRDN_DIS_F2_WIDTH                                                    1
#define PWRDN_DIS_F2_SHIFT                                                    0
#define PWRDN_DIS_F2_MASK                                            0x00000001
#define PWRDN_DIS_F2_RD(src)                             (((src) & 0x00000001))
#define PWRDN_DIS_F2_WR(src)                        (((u32)(src)) & 0x00000001)
#define PWRDN_DIS_F2_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register Flash_TxDATARAM	*/ 
/*	 Fields TEST1B	 */
#define TEST1B_F1_WIDTH                                                       1
#define TEST1B_F1_SHIFT                                                      12
#define TEST1B_F1_MASK                                               0x00001000
#define TEST1B_F1_RD(src)                            (((src) & 0x00001000)>>12)
#define TEST1B_F1_WR(src)                       (((u32)(src)<<12) & 0x00001000)
#define TEST1B_F1_SET(dst,src) \
                      (((dst) & ~0x00001000) | (((u32)(src)<<12) & 0x00001000))
/*	 Fields TEST1A	 */
#define TEST1A_F3_WIDTH                                                       1
#define TEST1A_F3_SHIFT                                                      11
#define TEST1A_F3_MASK                                               0x00000800
#define TEST1A_F3_RD(src)                            (((src) & 0x00000800)>>11)
#define TEST1A_F3_WR(src)                       (((u32)(src)<<11) & 0x00000800)
#define TEST1A_F3_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*	 Fields RMEB	 */
#define AHBC_RMEB_F1_WIDTH                                                    1
#define AHBC_RMEB_F1_SHIFT                                                   10
#define AHBC_RMEB_F1_MASK                                            0x00000400
#define AHBC_RMEB_F1_RD(src)                         (((src) & 0x00000400)>>10)
#define AHBC_RMEB_F1_WR(src)                    (((u32)(src)<<10) & 0x00000400)
#define AHBC_RMEB_F1_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields RMEA	 */
#define AHBC_RMEA_F3_WIDTH                                                    1
#define AHBC_RMEA_F3_SHIFT                                                    9
#define AHBC_RMEA_F3_MASK                                            0x00000200
#define AHBC_RMEA_F3_RD(src)                          (((src) & 0x00000200)>>9)
#define AHBC_RMEA_F3_WR(src)                     (((u32)(src)<<9) & 0x00000200)
#define AHBC_RMEA_F3_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields RMB	 */
#define AHBC_RMB_F1_WIDTH                                                     4
#define AHBC_RMB_F1_SHIFT                                                     5
#define AHBC_RMB_F1_MASK                                             0x000001e0
#define AHBC_RMB_F1_RD(src)                           (((src) & 0x000001e0)>>5)
#define AHBC_RMB_F1_WR(src)                      (((u32)(src)<<5) & 0x000001e0)
#define AHBC_RMB_F1_SET(dst,src) \
                       (((dst) & ~0x000001e0) | (((u32)(src)<<5) & 0x000001e0))
/*	 Fields RMA	 */
#define AHBC_RMA_F3_WIDTH                                                     4
#define AHBC_RMA_F3_SHIFT                                                     1
#define AHBC_RMA_F3_MASK                                             0x0000001e
#define AHBC_RMA_F3_RD(src)                           (((src) & 0x0000001e)>>1)
#define AHBC_RMA_F3_WR(src)                      (((u32)(src)<<1) & 0x0000001e)
#define AHBC_RMA_F3_SET(dst,src) \
                       (((dst) & ~0x0000001e) | (((u32)(src)<<1) & 0x0000001e))
/*	 Fields PWRDN_DIS	 */
#define PWRDN_DIS_F3_WIDTH                                                    1
#define PWRDN_DIS_F3_SHIFT                                                    0
#define PWRDN_DIS_F3_MASK                                            0x00000001
#define PWRDN_DIS_F3_RD(src)                             (((src) & 0x00000001))
#define PWRDN_DIS_F3_WR(src)                        (((u32)(src)) & 0x00000001)
#define PWRDN_DIS_F3_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register Flash_RxDATARAM	*/ 
/*	 Fields TEST1B	 */
#define TEST1B_F2_WIDTH                                                       1
#define TEST1B_F2_SHIFT                                                      12
#define TEST1B_F2_MASK                                               0x00001000
#define TEST1B_F2_RD(src)                            (((src) & 0x00001000)>>12)
#define TEST1B_F2_WR(src)                       (((u32)(src)<<12) & 0x00001000)
#define TEST1B_F2_SET(dst,src) \
                      (((dst) & ~0x00001000) | (((u32)(src)<<12) & 0x00001000))
/*	 Fields TEST1A	 */
#define TEST1A_F4_WIDTH                                                       1
#define TEST1A_F4_SHIFT                                                      11
#define TEST1A_F4_MASK                                               0x00000800
#define TEST1A_F4_RD(src)                            (((src) & 0x00000800)>>11)
#define TEST1A_F4_WR(src)                       (((u32)(src)<<11) & 0x00000800)
#define TEST1A_F4_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*	 Fields RMEB	 */
#define AHBC_RMEB_F2_WIDTH                                                    1
#define AHBC_RMEB_F2_SHIFT                                                   10
#define AHBC_RMEB_F2_MASK                                            0x00000400
#define AHBC_RMEB_F2_RD(src)                         (((src) & 0x00000400)>>10)
#define AHBC_RMEB_F2_WR(src)                    (((u32)(src)<<10) & 0x00000400)
#define AHBC_RMEB_F2_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields RMEA	 */
#define AHBC_RMEA_F4_WIDTH                                                    1
#define AHBC_RMEA_F4_SHIFT                                                    9
#define AHBC_RMEA_F4_MASK                                            0x00000200
#define AHBC_RMEA_F4_RD(src)                          (((src) & 0x00000200)>>9)
#define AHBC_RMEA_F4_WR(src)                     (((u32)(src)<<9) & 0x00000200)
#define AHBC_RMEA_F4_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields RMB	 */
#define AHBC_RMB_F2_WIDTH                                                     4
#define AHBC_RMB_F2_SHIFT                                                     5
#define AHBC_RMB_F2_MASK                                             0x000001e0
#define AHBC_RMB_F2_RD(src)                           (((src) & 0x000001e0)>>5)
#define AHBC_RMB_F2_WR(src)                      (((u32)(src)<<5) & 0x000001e0)
#define AHBC_RMB_F2_SET(dst,src) \
                       (((dst) & ~0x000001e0) | (((u32)(src)<<5) & 0x000001e0))
/*	 Fields RMA	 */
#define AHBC_RMA_F4_WIDTH                                                     4
#define AHBC_RMA_F4_SHIFT                                                     1
#define AHBC_RMA_F4_MASK                                             0x0000001e
#define AHBC_RMA_F4_RD(src)                           (((src) & 0x0000001e)>>1)
#define AHBC_RMA_F4_WR(src)                      (((u32)(src)<<1) & 0x0000001e)
#define AHBC_RMA_F4_SET(dst,src) \
                       (((dst) & ~0x0000001e) | (((u32)(src)<<1) & 0x0000001e))
/*	 Fields PWRDN_DIS	 */
#define PWRDN_DIS_F4_WIDTH                                                    1
#define PWRDN_DIS_F4_SHIFT                                                    0
#define PWRDN_DIS_F4_MASK                                            0x00000001
#define PWRDN_DIS_F4_RD(src)                             (((src) & 0x00000001))
#define PWRDN_DIS_F4_WR(src)                        (((u32)(src)) & 0x00000001)
#define PWRDN_DIS_F4_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register Flash_BCHDATARAM	*/ 
/*	 Fields TEST1B	 */
#define TEST1B_F3_WIDTH                                                       1
#define TEST1B_F3_SHIFT                                                      12
#define TEST1B_F3_MASK                                               0x00001000
#define TEST1B_F3_RD(src)                            (((src) & 0x00001000)>>12)
#define TEST1B_F3_WR(src)                       (((u32)(src)<<12) & 0x00001000)
#define TEST1B_F3_SET(dst,src) \
                      (((dst) & ~0x00001000) | (((u32)(src)<<12) & 0x00001000))
/*	 Fields TEST1A	 */
#define TEST1A_F5_WIDTH                                                       1
#define TEST1A_F5_SHIFT                                                      11
#define TEST1A_F5_MASK                                               0x00000800
#define TEST1A_F5_RD(src)                            (((src) & 0x00000800)>>11)
#define TEST1A_F5_WR(src)                       (((u32)(src)<<11) & 0x00000800)
#define TEST1A_F5_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*	 Fields RMEB	 */
#define AHBC_RMEB_F3_WIDTH                                                    1
#define AHBC_RMEB_F3_SHIFT                                                   10
#define AHBC_RMEB_F3_MASK                                            0x00000400
#define AHBC_RMEB_F3_RD(src)                         (((src) & 0x00000400)>>10)
#define AHBC_RMEB_F3_WR(src)                    (((u32)(src)<<10) & 0x00000400)
#define AHBC_RMEB_F3_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields RMEA	 */
#define AHBC_RMEA_F5_WIDTH                                                    1
#define AHBC_RMEA_F5_SHIFT                                                    9
#define AHBC_RMEA_F5_MASK                                            0x00000200
#define AHBC_RMEA_F5_RD(src)                          (((src) & 0x00000200)>>9)
#define AHBC_RMEA_F5_WR(src)                     (((u32)(src)<<9) & 0x00000200)
#define AHBC_RMEA_F5_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields RMB	 */
#define AHBC_RMB_F3_WIDTH                                                     4
#define AHBC_RMB_F3_SHIFT                                                     5
#define AHBC_RMB_F3_MASK                                             0x000001e0
#define AHBC_RMB_F3_RD(src)                           (((src) & 0x000001e0)>>5)
#define AHBC_RMB_F3_WR(src)                      (((u32)(src)<<5) & 0x000001e0)
#define AHBC_RMB_F3_SET(dst,src) \
                       (((dst) & ~0x000001e0) | (((u32)(src)<<5) & 0x000001e0))
/*	 Fields RMA	 */
#define AHBC_RMA_F5_WIDTH                                                     4
#define AHBC_RMA_F5_SHIFT                                                     1
#define AHBC_RMA_F5_MASK                                             0x0000001e
#define AHBC_RMA_F5_RD(src)                           (((src) & 0x0000001e)>>1)
#define AHBC_RMA_F5_WR(src)                      (((u32)(src)<<1) & 0x0000001e)
#define AHBC_RMA_F5_SET(dst,src) \
                       (((dst) & ~0x0000001e) | (((u32)(src)<<1) & 0x0000001e))
/*	 Fields PWRDN_DIS	 */
#define PWRDN_DIS_F5_WIDTH                                                    1
#define PWRDN_DIS_F5_SHIFT                                                    0
#define PWRDN_DIS_F5_MASK                                            0x00000001
#define PWRDN_DIS_F5_RD(src)                             (((src) & 0x00000001))
#define PWRDN_DIS_F5_WR(src)                        (((u32)(src)) & 0x00000001)
#define PWRDN_DIS_F5_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register HDLC0_TxDataFifo	*/ 
/*	 Fields TEST1B	 */
#define TEST1B_F4_WIDTH                                                       1
#define TEST1B_F4_SHIFT                                                      12
#define TEST1B_F4_MASK                                               0x00001000
#define TEST1B_F4_RD(src)                            (((src) & 0x00001000)>>12)
#define TEST1B_F4_WR(src)                       (((u32)(src)<<12) & 0x00001000)
#define TEST1B_F4_SET(dst,src) \
                      (((dst) & ~0x00001000) | (((u32)(src)<<12) & 0x00001000))
/*	 Fields TEST1A	 */
#define TEST1A_F6_WIDTH                                                       1
#define TEST1A_F6_SHIFT                                                      11
#define TEST1A_F6_MASK                                               0x00000800
#define TEST1A_F6_RD(src)                            (((src) & 0x00000800)>>11)
#define TEST1A_F6_WR(src)                       (((u32)(src)<<11) & 0x00000800)
#define TEST1A_F6_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*	 Fields RMEB	 */
#define AHBC_RMEB_F4_WIDTH                                                    1
#define AHBC_RMEB_F4_SHIFT                                                   10
#define AHBC_RMEB_F4_MASK                                            0x00000400
#define AHBC_RMEB_F4_RD(src)                         (((src) & 0x00000400)>>10)
#define AHBC_RMEB_F4_WR(src)                    (((u32)(src)<<10) & 0x00000400)
#define AHBC_RMEB_F4_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields RMEA	 */
#define AHBC_RMEA_F6_WIDTH                                                    1
#define AHBC_RMEA_F6_SHIFT                                                    9
#define AHBC_RMEA_F6_MASK                                            0x00000200
#define AHBC_RMEA_F6_RD(src)                          (((src) & 0x00000200)>>9)
#define AHBC_RMEA_F6_WR(src)                     (((u32)(src)<<9) & 0x00000200)
#define AHBC_RMEA_F6_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields RMB	 */
#define AHBC_RMB_F4_WIDTH                                                     4
#define AHBC_RMB_F4_SHIFT                                                     5
#define AHBC_RMB_F4_MASK                                             0x000001e0
#define AHBC_RMB_F4_RD(src)                           (((src) & 0x000001e0)>>5)
#define AHBC_RMB_F4_WR(src)                      (((u32)(src)<<5) & 0x000001e0)
#define AHBC_RMB_F4_SET(dst,src) \
                       (((dst) & ~0x000001e0) | (((u32)(src)<<5) & 0x000001e0))
/*	 Fields RMA	 */
#define AHBC_RMA_F6_WIDTH                                                     4
#define AHBC_RMA_F6_SHIFT                                                     1
#define AHBC_RMA_F6_MASK                                             0x0000001e
#define AHBC_RMA_F6_RD(src)                           (((src) & 0x0000001e)>>1)
#define AHBC_RMA_F6_WR(src)                      (((u32)(src)<<1) & 0x0000001e)
#define AHBC_RMA_F6_SET(dst,src) \
                       (((dst) & ~0x0000001e) | (((u32)(src)<<1) & 0x0000001e))
/*	 Fields PWRDN_DIS	 */
#define PWRDN_DIS_F6_WIDTH                                                    1
#define PWRDN_DIS_F6_SHIFT                                                    0
#define PWRDN_DIS_F6_MASK                                            0x00000001
#define PWRDN_DIS_F6_RD(src)                             (((src) & 0x00000001))
#define PWRDN_DIS_F6_WR(src)                        (((u32)(src)) & 0x00000001)
#define PWRDN_DIS_F6_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register HDLC0_RxDataFifo	*/ 
/*	 Fields TEST1B	 */
#define TEST1B_F5_WIDTH                                                       1
#define TEST1B_F5_SHIFT                                                      12
#define TEST1B_F5_MASK                                               0x00001000
#define TEST1B_F5_RD(src)                            (((src) & 0x00001000)>>12)
#define TEST1B_F5_WR(src)                       (((u32)(src)<<12) & 0x00001000)
#define TEST1B_F5_SET(dst,src) \
                      (((dst) & ~0x00001000) | (((u32)(src)<<12) & 0x00001000))
/*	 Fields TEST1A	 */
#define TEST1A_F7_WIDTH                                                       1
#define TEST1A_F7_SHIFT                                                      11
#define TEST1A_F7_MASK                                               0x00000800
#define TEST1A_F7_RD(src)                            (((src) & 0x00000800)>>11)
#define TEST1A_F7_WR(src)                       (((u32)(src)<<11) & 0x00000800)
#define TEST1A_F7_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*	 Fields RMEB	 */
#define AHBC_RMEB_F5_WIDTH                                                    1
#define AHBC_RMEB_F5_SHIFT                                                   10
#define AHBC_RMEB_F5_MASK                                            0x00000400
#define AHBC_RMEB_F5_RD(src)                         (((src) & 0x00000400)>>10)
#define AHBC_RMEB_F5_WR(src)                    (((u32)(src)<<10) & 0x00000400)
#define AHBC_RMEB_F5_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields RMEA	 */
#define AHBC_RMEA_F7_WIDTH                                                    1
#define AHBC_RMEA_F7_SHIFT                                                    9
#define AHBC_RMEA_F7_MASK                                            0x00000200
#define AHBC_RMEA_F7_RD(src)                          (((src) & 0x00000200)>>9)
#define AHBC_RMEA_F7_WR(src)                     (((u32)(src)<<9) & 0x00000200)
#define AHBC_RMEA_F7_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields RMB	 */
#define AHBC_RMB_F5_WIDTH                                                     4
#define AHBC_RMB_F5_SHIFT                                                     5
#define AHBC_RMB_F5_MASK                                             0x000001e0
#define AHBC_RMB_F5_RD(src)                           (((src) & 0x000001e0)>>5)
#define AHBC_RMB_F5_WR(src)                      (((u32)(src)<<5) & 0x000001e0)
#define AHBC_RMB_F5_SET(dst,src) \
                       (((dst) & ~0x000001e0) | (((u32)(src)<<5) & 0x000001e0))
/*	 Fields RMA	 */
#define AHBC_RMA_F7_WIDTH                                                     4
#define AHBC_RMA_F7_SHIFT                                                     1
#define AHBC_RMA_F7_MASK                                             0x0000001e
#define AHBC_RMA_F7_RD(src)                           (((src) & 0x0000001e)>>1)
#define AHBC_RMA_F7_WR(src)                      (((u32)(src)<<1) & 0x0000001e)
#define AHBC_RMA_F7_SET(dst,src) \
                       (((dst) & ~0x0000001e) | (((u32)(src)<<1) & 0x0000001e))
/*	 Fields PWRDN_DIS	 */
#define PWRDN_DIS_F7_WIDTH                                                    1
#define PWRDN_DIS_F7_SHIFT                                                    0
#define PWRDN_DIS_F7_MASK                                            0x00000001
#define PWRDN_DIS_F7_RD(src)                             (((src) & 0x00000001))
#define PWRDN_DIS_F7_WR(src)                        (((u32)(src)) & 0x00000001)
#define PWRDN_DIS_F7_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register HDLC1_TxDataFifo	*/ 
/*	 Fields TEST1B	 */
#define TEST1B_F6_WIDTH                                                       1
#define TEST1B_F6_SHIFT                                                      12
#define TEST1B_F6_MASK                                               0x00001000
#define TEST1B_F6_RD(src)                            (((src) & 0x00001000)>>12)
#define TEST1B_F6_WR(src)                       (((u32)(src)<<12) & 0x00001000)
#define TEST1B_F6_SET(dst,src) \
                      (((dst) & ~0x00001000) | (((u32)(src)<<12) & 0x00001000))
/*	 Fields TEST1A	 */
#define TEST1A_F8_WIDTH                                                       1
#define TEST1A_F8_SHIFT                                                      11
#define TEST1A_F8_MASK                                               0x00000800
#define TEST1A_F8_RD(src)                            (((src) & 0x00000800)>>11)
#define TEST1A_F8_WR(src)                       (((u32)(src)<<11) & 0x00000800)
#define TEST1A_F8_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*	 Fields RMEB	 */
#define AHBC_RMEB_F6_WIDTH                                                    1
#define AHBC_RMEB_F6_SHIFT                                                   10
#define AHBC_RMEB_F6_MASK                                            0x00000400
#define AHBC_RMEB_F6_RD(src)                         (((src) & 0x00000400)>>10)
#define AHBC_RMEB_F6_WR(src)                    (((u32)(src)<<10) & 0x00000400)
#define AHBC_RMEB_F6_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields RMEA	 */
#define AHBC_RMEA_F8_WIDTH                                                    1
#define AHBC_RMEA_F8_SHIFT                                                    9
#define AHBC_RMEA_F8_MASK                                            0x00000200
#define AHBC_RMEA_F8_RD(src)                          (((src) & 0x00000200)>>9)
#define AHBC_RMEA_F8_WR(src)                     (((u32)(src)<<9) & 0x00000200)
#define AHBC_RMEA_F8_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields RMB	 */
#define AHBC_RMB_F6_WIDTH                                                     4
#define AHBC_RMB_F6_SHIFT                                                     5
#define AHBC_RMB_F6_MASK                                             0x000001e0
#define AHBC_RMB_F6_RD(src)                           (((src) & 0x000001e0)>>5)
#define AHBC_RMB_F6_WR(src)                      (((u32)(src)<<5) & 0x000001e0)
#define AHBC_RMB_F6_SET(dst,src) \
                       (((dst) & ~0x000001e0) | (((u32)(src)<<5) & 0x000001e0))
/*	 Fields RMA	 */
#define AHBC_RMA_F8_WIDTH                                                     4
#define AHBC_RMA_F8_SHIFT                                                     1
#define AHBC_RMA_F8_MASK                                             0x0000001e
#define AHBC_RMA_F8_RD(src)                           (((src) & 0x0000001e)>>1)
#define AHBC_RMA_F8_WR(src)                      (((u32)(src)<<1) & 0x0000001e)
#define AHBC_RMA_F8_SET(dst,src) \
                       (((dst) & ~0x0000001e) | (((u32)(src)<<1) & 0x0000001e))
/*	 Fields PWRDN_DIS	 */
#define PWRDN_DIS_F8_WIDTH                                                    1
#define PWRDN_DIS_F8_SHIFT                                                    0
#define PWRDN_DIS_F8_MASK                                            0x00000001
#define PWRDN_DIS_F8_RD(src)                             (((src) & 0x00000001))
#define PWRDN_DIS_F8_WR(src)                        (((u32)(src)) & 0x00000001)
#define PWRDN_DIS_F8_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register HDLC1_RxDataFifo	*/ 
/*	 Fields TEST1B	 */
#define TEST1B_F7_WIDTH                                                       1
#define TEST1B_F7_SHIFT                                                      12
#define TEST1B_F7_MASK                                               0x00001000
#define TEST1B_F7_RD(src)                            (((src) & 0x00001000)>>12)
#define TEST1B_F7_WR(src)                       (((u32)(src)<<12) & 0x00001000)
#define TEST1B_F7_SET(dst,src) \
                      (((dst) & ~0x00001000) | (((u32)(src)<<12) & 0x00001000))
/*	 Fields TEST1A	 */
#define TEST1A_F9_WIDTH                                                       1
#define TEST1A_F9_SHIFT                                                      11
#define TEST1A_F9_MASK                                               0x00000800
#define TEST1A_F9_RD(src)                            (((src) & 0x00000800)>>11)
#define TEST1A_F9_WR(src)                       (((u32)(src)<<11) & 0x00000800)
#define TEST1A_F9_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*	 Fields RMEB	 */
#define AHBC_RMEB_F7_WIDTH                                                    1
#define AHBC_RMEB_F7_SHIFT                                                   10
#define AHBC_RMEB_F7_MASK                                            0x00000400
#define AHBC_RMEB_F7_RD(src)                         (((src) & 0x00000400)>>10)
#define AHBC_RMEB_F7_WR(src)                    (((u32)(src)<<10) & 0x00000400)
#define AHBC_RMEB_F7_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields RMEA	 */
#define AHBC_RMEA_F9_WIDTH                                                    1
#define AHBC_RMEA_F9_SHIFT                                                    9
#define AHBC_RMEA_F9_MASK                                            0x00000200
#define AHBC_RMEA_F9_RD(src)                          (((src) & 0x00000200)>>9)
#define AHBC_RMEA_F9_WR(src)                     (((u32)(src)<<9) & 0x00000200)
#define AHBC_RMEA_F9_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields RMB	 */
#define AHBC_RMB_F7_WIDTH                                                     4
#define AHBC_RMB_F7_SHIFT                                                     5
#define AHBC_RMB_F7_MASK                                             0x000001e0
#define AHBC_RMB_F7_RD(src)                           (((src) & 0x000001e0)>>5)
#define AHBC_RMB_F7_WR(src)                      (((u32)(src)<<5) & 0x000001e0)
#define AHBC_RMB_F7_SET(dst,src) \
                       (((dst) & ~0x000001e0) | (((u32)(src)<<5) & 0x000001e0))
/*	 Fields RMA	 */
#define AHBC_RMA_F9_WIDTH                                                     4
#define AHBC_RMA_F9_SHIFT                                                     1
#define AHBC_RMA_F9_MASK                                             0x0000001e
#define AHBC_RMA_F9_RD(src)                           (((src) & 0x0000001e)>>1)
#define AHBC_RMA_F9_WR(src)                      (((u32)(src)<<1) & 0x0000001e)
#define AHBC_RMA_F9_SET(dst,src) \
                       (((dst) & ~0x0000001e) | (((u32)(src)<<1) & 0x0000001e))
/*	 Fields PWRDN_DIS	 */
#define PWRDN_DIS_F9_WIDTH                                                    1
#define PWRDN_DIS_F9_SHIFT                                                    0
#define PWRDN_DIS_F9_MASK                                            0x00000001
#define PWRDN_DIS_F9_RD(src)                             (((src) & 0x00000001))
#define PWRDN_DIS_F9_WR(src)                        (((u32)(src)) & 0x00000001)
#define PWRDN_DIS_F9_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register HDLC_CONFIG	*/ 
/*	 Fields CFG_AHB_HDLC_TXCLK_SELECT	 */
#define CFG_AHB_HDLC_TXCLK_SELECT_WIDTH                                       1
#define CFG_AHB_HDLC_TXCLK_SELECT_SHIFT                                      28
#define CFG_AHB_HDLC_TXCLK_SELECT_MASK                               0x10000000
#define CFG_AHB_HDLC_TXCLK_SELECT_RD(src)            (((src) & 0x10000000)>>28)
#define CFG_AHB_HDLC_TXCLK_SELECT_WR(src)       (((u32)(src)<<28) & 0x10000000)
#define CFG_AHB_HDLC_TXCLK_SELECT_SET(dst,src) \
                      (((dst) & ~0x10000000) | (((u32)(src)<<28) & 0x10000000))
/*	 Fields CFG_AHB_HDLC_TXCLK_RATIO	 */
#define CFG_AHB_HDLC_TXCLK_RATIO_WIDTH                                       10
#define CFG_AHB_HDLC_TXCLK_RATIO_SHIFT                                       16
#define CFG_AHB_HDLC_TXCLK_RATIO_MASK                                0x03ff0000
#define CFG_AHB_HDLC_TXCLK_RATIO_RD(src)             (((src) & 0x03ff0000)>>16)
#define CFG_AHB_HDLC_TXCLK_RATIO_WR(src)        (((u32)(src)<<16) & 0x03ff0000)
#define CFG_AHB_HDLC_TXCLK_RATIO_SET(dst,src) \
                      (((dst) & ~0x03ff0000) | (((u32)(src)<<16) & 0x03ff0000))
/*	 Fields CFG_ENABLE_HDLC_FSEL_LPBK_1	 */
#define CFG_ENABLE_HDLC_FSEL_LPBK_1_WIDTH                                     1
#define CFG_ENABLE_HDLC_FSEL_LPBK_1_SHIFT                                     7
#define CFG_ENABLE_HDLC_FSEL_LPBK_1_MASK                             0x00000080
#define CFG_ENABLE_HDLC_FSEL_LPBK_1_RD(src)           (((src) & 0x00000080)>>7)
#define CFG_ENABLE_HDLC_FSEL_LPBK_1_WR(src)      (((u32)(src)<<7) & 0x00000080)
#define CFG_ENABLE_HDLC_FSEL_LPBK_1_SET(dst,src) \
                       (((dst) & ~0x00000080) | (((u32)(src)<<7) & 0x00000080))
/*	 Fields CFG_ENABLE_HDLC_FSEL_LPBK_0	 */
#define CFG_ENABLE_HDLC_FSEL_LPBK_0_WIDTH                                     1
#define CFG_ENABLE_HDLC_FSEL_LPBK_0_SHIFT                                     6
#define CFG_ENABLE_HDLC_FSEL_LPBK_0_MASK                             0x00000040
#define CFG_ENABLE_HDLC_FSEL_LPBK_0_RD(src)           (((src) & 0x00000040)>>6)
#define CFG_ENABLE_HDLC_FSEL_LPBK_0_WR(src)      (((u32)(src)<<6) & 0x00000040)
#define CFG_ENABLE_HDLC_FSEL_LPBK_0_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*	 Fields CFG_ENABLE_HDLC_TXD_OD_TYPE_1	 */
#define CFG_ENABLE_HDLC_TXD_OD_TYPE_1_WIDTH                                   1
#define CFG_ENABLE_HDLC_TXD_OD_TYPE_1_SHIFT                                   5
#define CFG_ENABLE_HDLC_TXD_OD_TYPE_1_MASK                           0x00000020
#define CFG_ENABLE_HDLC_TXD_OD_TYPE_1_RD(src)         (((src) & 0x00000020)>>5)
#define CFG_ENABLE_HDLC_TXD_OD_TYPE_1_WR(src)    (((u32)(src)<<5) & 0x00000020)
#define CFG_ENABLE_HDLC_TXD_OD_TYPE_1_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields CFG_ENABLE_HDLC_TXD_OD_TYPE_0	 */
#define CFG_ENABLE_HDLC_TXD_OD_TYPE_0_WIDTH                                   1
#define CFG_ENABLE_HDLC_TXD_OD_TYPE_0_SHIFT                                   4
#define CFG_ENABLE_HDLC_TXD_OD_TYPE_0_MASK                           0x00000010
#define CFG_ENABLE_HDLC_TXD_OD_TYPE_0_RD(src)         (((src) & 0x00000010)>>4)
#define CFG_ENABLE_HDLC_TXD_OD_TYPE_0_WR(src)    (((u32)(src)<<4) & 0x00000010)
#define CFG_ENABLE_HDLC_TXD_OD_TYPE_0_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields CFG_ENABLE_HDLC_TXFSEL_OUT_1	 */
#define CFG_ENABLE_HDLC_TXFSEL_OUT_1_WIDTH                                    1
#define CFG_ENABLE_HDLC_TXFSEL_OUT_1_SHIFT                                    3
#define CFG_ENABLE_HDLC_TXFSEL_OUT_1_MASK                            0x00000008
#define CFG_ENABLE_HDLC_TXFSEL_OUT_1_RD(src)          (((src) & 0x00000008)>>3)
#define CFG_ENABLE_HDLC_TXFSEL_OUT_1_WR(src)     (((u32)(src)<<3) & 0x00000008)
#define CFG_ENABLE_HDLC_TXFSEL_OUT_1_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields CFG_ENABLE_HDLC_TXFSEL_OUT_0	 */
#define CFG_ENABLE_HDLC_TXFSEL_OUT_0_WIDTH                                    1
#define CFG_ENABLE_HDLC_TXFSEL_OUT_0_SHIFT                                    2
#define CFG_ENABLE_HDLC_TXFSEL_OUT_0_MASK                            0x00000004
#define CFG_ENABLE_HDLC_TXFSEL_OUT_0_RD(src)          (((src) & 0x00000004)>>2)
#define CFG_ENABLE_HDLC_TXFSEL_OUT_0_WR(src)     (((u32)(src)<<2) & 0x00000004)
#define CFG_ENABLE_HDLC_TXFSEL_OUT_0_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields CFG_ENABLE_HDLC_TXCLK_1	 */
#define CFG_ENABLE_HDLC_TXCLK_1_WIDTH                                         1
#define CFG_ENABLE_HDLC_TXCLK_1_SHIFT                                         1
#define CFG_ENABLE_HDLC_TXCLK_1_MASK                                 0x00000002
#define CFG_ENABLE_HDLC_TXCLK_1_RD(src)               (((src) & 0x00000002)>>1)
#define CFG_ENABLE_HDLC_TXCLK_1_WR(src)          (((u32)(src)<<1) & 0x00000002)
#define CFG_ENABLE_HDLC_TXCLK_1_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields CFG_ENABLE_HDLC_TXCLK_0	 */
#define CFG_ENABLE_HDLC_TXCLK_0_WIDTH                                         1
#define CFG_ENABLE_HDLC_TXCLK_0_SHIFT                                         0
#define CFG_ENABLE_HDLC_TXCLK_0_MASK                                 0x00000001
#define CFG_ENABLE_HDLC_TXCLK_0_RD(src)                  (((src) & 0x00000001))
#define CFG_ENABLE_HDLC_TXCLK_0_WR(src)             (((u32)(src)) & 0x00000001)
#define CFG_ENABLE_HDLC_TXCLK_0_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register SDHC_CONFIG	*/ 
/*	 Fields CFG_SDIO_RETUNE	 */
#define CFG_SDIO_RETUNE_WIDTH                                                 1
#define CFG_SDIO_RETUNE_SHIFT                                                30
#define CFG_SDIO_RETUNE_MASK                                         0x40000000
#define CFG_SDIO_RETUNE_RD(src)                      (((src) & 0x40000000)>>30)
#define CFG_SDIO_RETUNE_WR(src)                 (((u32)(src)<<30) & 0x40000000)
#define CFG_SDIO_RETUNE_SET(dst,src) \
                      (((dst) & ~0x40000000) | (((u32)(src)<<30) & 0x40000000))

/*	Register UART_MODE_SEL	*/ 
/*	 Fields CFG_AHB_SCLK_RATIO	 */
#define CFG_AHB_SCLK_RATIO_WIDTH                                              7
#define CFG_AHB_SCLK_RATIO_SHIFT                                              9
#define CFG_AHB_SCLK_RATIO_MASK                                      0x0000fe00
#define CFG_AHB_SCLK_RATIO_RD(src)                    (((src) & 0x0000fe00)>>9)
#define CFG_AHB_SCLK_RATIO_WR(src)               (((u32)(src)<<9) & 0x0000fe00)
#define CFG_AHB_SCLK_RATIO_SET(dst,src) \
                       (((dst) & ~0x0000fe00) | (((u32)(src)<<9) & 0x0000fe00))
/*	 Fields CFG_UART_INT_REFCLK	 */
#define CFG_UART_INT_REFCLK_WIDTH                                             1
#define CFG_UART_INT_REFCLK_SHIFT                                             8
#define CFG_UART_INT_REFCLK_MASK                                     0x00000100
#define CFG_UART_INT_REFCLK_RD(src)                   (((src) & 0x00000100)>>8)
#define CFG_UART_INT_REFCLK_WR(src)              (((u32)(src)<<8) & 0x00000100)
#define CFG_UART_INT_REFCLK_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*	 Fields CFG_UART_REFCLK	 */
#define CFG_UART_REFCLK_WIDTH                                                 1
#define CFG_UART_REFCLK_SHIFT                                                 5
#define CFG_UART_REFCLK_MASK                                         0x00000020
#define CFG_UART_REFCLK_RD(src)                       (((src) & 0x00000020)>>5)
#define CFG_UART_REFCLK_WR(src)                  (((u32)(src)<<5) & 0x00000020)
#define CFG_UART_REFCLK_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields CFG_CTS_PAIR_1	 */
#define CFG_CTS_PAIR_1_WIDTH                                                  1
#define CFG_CTS_PAIR_1_SHIFT                                                  4
#define CFG_CTS_PAIR_1_MASK                                          0x00000010
#define CFG_CTS_PAIR_1_RD(src)                        (((src) & 0x00000010)>>4)
#define CFG_CTS_PAIR_1_WR(src)                   (((u32)(src)<<4) & 0x00000010)
#define CFG_CTS_PAIR_1_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields CFG_CTS_PAIR_0	 */
#define CFG_CTS_PAIR_0_WIDTH                                                  1
#define CFG_CTS_PAIR_0_SHIFT                                                  3
#define CFG_CTS_PAIR_0_MASK                                          0x00000008
#define CFG_CTS_PAIR_0_RD(src)                        (((src) & 0x00000008)>>3)
#define CFG_CTS_PAIR_0_WR(src)                   (((u32)(src)<<3) & 0x00000008)
#define CFG_CTS_PAIR_0_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields CFG_ENABLE_4BIT_UART_1	 */
#define CFG_ENABLE_4BIT_UART_1_WIDTH                                          1
#define CFG_ENABLE_4BIT_UART_1_SHIFT                                          2
#define CFG_ENABLE_4BIT_UART_1_MASK                                  0x00000004
#define CFG_ENABLE_4BIT_UART_1_RD(src)                (((src) & 0x00000004)>>2)
#define CFG_ENABLE_4BIT_UART_1_WR(src)           (((u32)(src)<<2) & 0x00000004)
#define CFG_ENABLE_4BIT_UART_1_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields CFG_ENABLE_4BIT_UART_0	 */
#define CFG_ENABLE_4BIT_UART_0_WIDTH                                          1
#define CFG_ENABLE_4BIT_UART_0_SHIFT                                          1
#define CFG_ENABLE_4BIT_UART_0_MASK                                  0x00000002
#define CFG_ENABLE_4BIT_UART_0_RD(src)                (((src) & 0x00000002)>>1)
#define CFG_ENABLE_4BIT_UART_0_WR(src)           (((u32)(src)<<1) & 0x00000002)
#define CFG_ENABLE_4BIT_UART_0_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields CFG_ENABLE_8BIT_UART_0	 */
#define CFG_ENABLE_8BIT_UART_0_WIDTH                                          1
#define CFG_ENABLE_8BIT_UART_0_SHIFT                                          0
#define CFG_ENABLE_8BIT_UART_0_MASK                                  0x00000001
#define CFG_ENABLE_8BIT_UART_0_RD(src)                   (((src) & 0x00000001))
#define CFG_ENABLE_8BIT_UART_0_WR(src)              (((u32)(src)) & 0x00000001)
#define CFG_ENABLE_8BIT_UART_0_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register ahb2axi_bid	*/ 
/*	 Fields revno	 */
#define AHBC_REVNO_WIDTH                                                      2
#define AHBC_REVNO_SHIFT                                                      8
#define AHBC_REVNO_MASK                                              0x00000300
#define AHBC_REVNO_RD(src)                            (((src) & 0x00000300)>>8)
#define AHBC_REVNO_SET(dst,src) \
                       (((dst) & ~0x00000300) | (((u32)(src)<<8) & 0x00000300))
/*	 Fields busid	 */
#define AHBC_BUSID_WIDTH                                                      3
#define AHBC_BUSID_SHIFT                                                      5
#define AHBC_BUSID_MASK                                              0x000000e0
#define AHBC_BUSID_RD(src)                            (((src) & 0x000000e0)>>5)
#define AHBC_BUSID_SET(dst,src) \
                       (((dst) & ~0x000000e0) | (((u32)(src)<<5) & 0x000000e0))
/*	 Fields deviceid	 */
#define AHBC_DEVICEID_WIDTH                                                   5
#define AHBC_DEVICEID_SHIFT                                                   0
#define AHBC_DEVICEID_MASK                                           0x0000001f
#define AHBC_DEVICEID_RD(src)                            (((src) & 0x0000001f))
#define AHBC_DEVICEID_SET(dst,src) \
                          (((dst) & ~0x0000001f) | (((u32)(src)) & 0x0000001f))

/*	Register tdm_bid	*/ 
/*	 Fields revno	 */
#define AHBC_REVNO_F1_WIDTH                                                   2
#define AHBC_REVNO_F1_SHIFT                                                   8
#define AHBC_REVNO_F1_MASK                                           0x00000300
#define AHBC_REVNO_F1_RD(src)                         (((src) & 0x00000300)>>8)
#define AHBC_REVNO_F1_SET(dst,src) \
                       (((dst) & ~0x00000300) | (((u32)(src)<<8) & 0x00000300))
/*	 Fields busid	 */
#define AHBC_BUSID_F1_WIDTH                                                   3
#define AHBC_BUSID_F1_SHIFT                                                   5
#define AHBC_BUSID_F1_MASK                                           0x000000e0
#define AHBC_BUSID_F1_RD(src)                         (((src) & 0x000000e0)>>5)
#define AHBC_BUSID_F1_SET(dst,src) \
                       (((dst) & ~0x000000e0) | (((u32)(src)<<5) & 0x000000e0))
/*	 Fields deviceid	 */
#define AHBC_DEVICEID_F1_WIDTH                                                5
#define AHBC_DEVICEID_F1_SHIFT                                                0
#define AHBC_DEVICEID_F1_MASK                                        0x0000001f
#define AHBC_DEVICEID_F1_RD(src)                         (((src) & 0x0000001f))
#define AHBC_DEVICEID_F1_SET(dst,src) \
                          (((dst) & ~0x0000001f) | (((u32)(src)) & 0x0000001f))

/*	Register ahbc_parity	*/ 
/*	 Fields RAM_PARITY	 */
#define RAM_PARITY_WIDTH                                                      1
#define RAM_PARITY_SHIFT                                                      0
#define RAM_PARITY_MASK                                              0x00000001
#define RAM_PARITY_RD(src)                               (((src) & 0x00000001))
#define RAM_PARITY_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register ahbc_INT	*/ 
/*	 Fields usbh2_wr_decerr_reported	 */
#define USBH2_WR_DECERR_REPORTED_WIDTH                                        1
#define USBH2_WR_DECERR_REPORTED_SHIFT                                       27
#define USBH2_WR_DECERR_REPORTED_MASK                                0x08000000
#define USBH2_WR_DECERR_REPORTED_RD(src)             (((src) & 0x08000000)>>27)
#define USBH2_WR_DECERR_REPORTED_WR(src)        (((u32)(src)<<27) & 0x08000000)
#define USBH2_WR_DECERR_REPORTED_SET(dst,src) \
                      (((dst) & ~0x08000000) | (((u32)(src)<<27) & 0x08000000))
/*	 Fields usbh2_wr_slverr_reported	 */
#define USBH2_WR_SLVERR_REPORTED_WIDTH                                        1
#define USBH2_WR_SLVERR_REPORTED_SHIFT                                       26
#define USBH2_WR_SLVERR_REPORTED_MASK                                0x04000000
#define USBH2_WR_SLVERR_REPORTED_RD(src)             (((src) & 0x04000000)>>26)
#define USBH2_WR_SLVERR_REPORTED_WR(src)        (((u32)(src)<<26) & 0x04000000)
#define USBH2_WR_SLVERR_REPORTED_SET(dst,src) \
                      (((dst) & ~0x04000000) | (((u32)(src)<<26) & 0x04000000))
/*	 Fields usbh2_rd_decerr_reported	 */
#define USBH2_RD_DECERR_REPORTED_WIDTH                                        1
#define USBH2_RD_DECERR_REPORTED_SHIFT                                       25
#define USBH2_RD_DECERR_REPORTED_MASK                                0x02000000
#define USBH2_RD_DECERR_REPORTED_RD(src)             (((src) & 0x02000000)>>25)
#define USBH2_RD_DECERR_REPORTED_WR(src)        (((u32)(src)<<25) & 0x02000000)
#define USBH2_RD_DECERR_REPORTED_SET(dst,src) \
                      (((dst) & ~0x02000000) | (((u32)(src)<<25) & 0x02000000))
/*	 Fields usbh2_rd_slverr_reported	 */
#define USBH2_RD_SLVERR_REPORTED_WIDTH                                        1
#define USBH2_RD_SLVERR_REPORTED_SHIFT                                       24
#define USBH2_RD_SLVERR_REPORTED_MASK                                0x01000000
#define USBH2_RD_SLVERR_REPORTED_RD(src)             (((src) & 0x01000000)>>24)
#define USBH2_RD_SLVERR_REPORTED_WR(src)        (((u32)(src)<<24) & 0x01000000)
#define USBH2_RD_SLVERR_REPORTED_SET(dst,src) \
                      (((dst) & ~0x01000000) | (((u32)(src)<<24) & 0x01000000))
/*	 Fields usbh1_wr_decerr_reported	 */
#define USBH1_WR_DECERR_REPORTED_WIDTH                                        1
#define USBH1_WR_DECERR_REPORTED_SHIFT                                       23
#define USBH1_WR_DECERR_REPORTED_MASK                                0x00800000
#define USBH1_WR_DECERR_REPORTED_RD(src)             (((src) & 0x00800000)>>23)
#define USBH1_WR_DECERR_REPORTED_WR(src)        (((u32)(src)<<23) & 0x00800000)
#define USBH1_WR_DECERR_REPORTED_SET(dst,src) \
                      (((dst) & ~0x00800000) | (((u32)(src)<<23) & 0x00800000))
/*	 Fields usbh1_wr_slverr_reported	 */
#define USBH1_WR_SLVERR_REPORTED_WIDTH                                        1
#define USBH1_WR_SLVERR_REPORTED_SHIFT                                       22
#define USBH1_WR_SLVERR_REPORTED_MASK                                0x00400000
#define USBH1_WR_SLVERR_REPORTED_RD(src)             (((src) & 0x00400000)>>22)
#define USBH1_WR_SLVERR_REPORTED_WR(src)        (((u32)(src)<<22) & 0x00400000)
#define USBH1_WR_SLVERR_REPORTED_SET(dst,src) \
                      (((dst) & ~0x00400000) | (((u32)(src)<<22) & 0x00400000))
/*	 Fields usbh1_rd_decerr_reported	 */
#define USBH1_RD_DECERR_REPORTED_WIDTH                                        1
#define USBH1_RD_DECERR_REPORTED_SHIFT                                       21
#define USBH1_RD_DECERR_REPORTED_MASK                                0x00200000
#define USBH1_RD_DECERR_REPORTED_RD(src)             (((src) & 0x00200000)>>21)
#define USBH1_RD_DECERR_REPORTED_WR(src)        (((u32)(src)<<21) & 0x00200000)
#define USBH1_RD_DECERR_REPORTED_SET(dst,src) \
                      (((dst) & ~0x00200000) | (((u32)(src)<<21) & 0x00200000))
/*	 Fields usbh1_rd_slverr_reported	 */
#define USBH1_RD_SLVERR_REPORTED_WIDTH                                        1
#define USBH1_RD_SLVERR_REPORTED_SHIFT                                       20
#define USBH1_RD_SLVERR_REPORTED_MASK                                0x00100000
#define USBH1_RD_SLVERR_REPORTED_RD(src)             (((src) & 0x00100000)>>20)
#define USBH1_RD_SLVERR_REPORTED_WR(src)        (((u32)(src)<<20) & 0x00100000)
#define USBH1_RD_SLVERR_REPORTED_SET(dst,src) \
                      (((dst) & ~0x00100000) | (((u32)(src)<<20) & 0x00100000))
/*	 Fields usbh_wr_decerr_reported	 */
#define USBH_WR_DECERR_REPORTED_WIDTH                                         1
#define USBH_WR_DECERR_REPORTED_SHIFT                                        15
#define USBH_WR_DECERR_REPORTED_MASK                                 0x00008000
#define USBH_WR_DECERR_REPORTED_RD(src)              (((src) & 0x00008000)>>15)
#define USBH_WR_DECERR_REPORTED_WR(src)         (((u32)(src)<<15) & 0x00008000)
#define USBH_WR_DECERR_REPORTED_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))
/*	 Fields usbh_wr_slverr_reported	 */
#define USBH_WR_SLVERR_REPORTED_WIDTH                                         1
#define USBH_WR_SLVERR_REPORTED_SHIFT                                        14
#define USBH_WR_SLVERR_REPORTED_MASK                                 0x00004000
#define USBH_WR_SLVERR_REPORTED_RD(src)              (((src) & 0x00004000)>>14)
#define USBH_WR_SLVERR_REPORTED_WR(src)         (((u32)(src)<<14) & 0x00004000)
#define USBH_WR_SLVERR_REPORTED_SET(dst,src) \
                      (((dst) & ~0x00004000) | (((u32)(src)<<14) & 0x00004000))
/*	 Fields usbh_rd_decerr_reported	 */
#define USBH_RD_DECERR_REPORTED_WIDTH                                         1
#define USBH_RD_DECERR_REPORTED_SHIFT                                        13
#define USBH_RD_DECERR_REPORTED_MASK                                 0x00002000
#define USBH_RD_DECERR_REPORTED_RD(src)              (((src) & 0x00002000)>>13)
#define USBH_RD_DECERR_REPORTED_WR(src)         (((u32)(src)<<13) & 0x00002000)
#define USBH_RD_DECERR_REPORTED_SET(dst,src) \
                      (((dst) & ~0x00002000) | (((u32)(src)<<13) & 0x00002000))
/*	 Fields usbh_rd_slverr_reported	 */
#define USBH_RD_SLVERR_REPORTED_WIDTH                                         1
#define USBH_RD_SLVERR_REPORTED_SHIFT                                        12
#define USBH_RD_SLVERR_REPORTED_MASK                                 0x00001000
#define USBH_RD_SLVERR_REPORTED_RD(src)              (((src) & 0x00001000)>>12)
#define USBH_RD_SLVERR_REPORTED_WR(src)         (((u32)(src)<<12) & 0x00001000)
#define USBH_RD_SLVERR_REPORTED_SET(dst,src) \
                      (((dst) & ~0x00001000) | (((u32)(src)<<12) & 0x00001000))
/*	 Fields sdhc_wr_decerr_reported	 */
#define SDHC_WR_DECERR_REPORTED_WIDTH                                         1
#define SDHC_WR_DECERR_REPORTED_SHIFT                                        11
#define SDHC_WR_DECERR_REPORTED_MASK                                 0x00000800
#define SDHC_WR_DECERR_REPORTED_RD(src)              (((src) & 0x00000800)>>11)
#define SDHC_WR_DECERR_REPORTED_WR(src)         (((u32)(src)<<11) & 0x00000800)
#define SDHC_WR_DECERR_REPORTED_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*	 Fields sdhc_wr_slverr_reported	 */
#define SDHC_WR_SLVERR_REPORTED_WIDTH                                         1
#define SDHC_WR_SLVERR_REPORTED_SHIFT                                        10
#define SDHC_WR_SLVERR_REPORTED_MASK                                 0x00000400
#define SDHC_WR_SLVERR_REPORTED_RD(src)              (((src) & 0x00000400)>>10)
#define SDHC_WR_SLVERR_REPORTED_WR(src)         (((u32)(src)<<10) & 0x00000400)
#define SDHC_WR_SLVERR_REPORTED_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields sdhc_rd_decerr_reported	 */
#define SDHC_RD_DECERR_REPORTED_WIDTH                                         1
#define SDHC_RD_DECERR_REPORTED_SHIFT                                         9
#define SDHC_RD_DECERR_REPORTED_MASK                                 0x00000200
#define SDHC_RD_DECERR_REPORTED_RD(src)               (((src) & 0x00000200)>>9)
#define SDHC_RD_DECERR_REPORTED_WR(src)          (((u32)(src)<<9) & 0x00000200)
#define SDHC_RD_DECERR_REPORTED_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields sdhc_rd_slverr_reported	 */
#define SDHC_RD_SLVERR_REPORTED_WIDTH                                         1
#define SDHC_RD_SLVERR_REPORTED_SHIFT                                         8
#define SDHC_RD_SLVERR_REPORTED_MASK                                 0x00000100
#define SDHC_RD_SLVERR_REPORTED_RD(src)               (((src) & 0x00000100)>>8)
#define SDHC_RD_SLVERR_REPORTED_WR(src)          (((u32)(src)<<8) & 0x00000100)
#define SDHC_RD_SLVERR_REPORTED_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*	 Fields usb1_wr_decerr_reported	 */
#define USB1_WR_DECERR_REPORTED_WIDTH                                         1
#define USB1_WR_DECERR_REPORTED_SHIFT                                         7
#define USB1_WR_DECERR_REPORTED_MASK                                 0x00000080
#define USB1_WR_DECERR_REPORTED_RD(src)               (((src) & 0x00000080)>>7)
#define USB1_WR_DECERR_REPORTED_WR(src)          (((u32)(src)<<7) & 0x00000080)
#define USB1_WR_DECERR_REPORTED_SET(dst,src) \
                       (((dst) & ~0x00000080) | (((u32)(src)<<7) & 0x00000080))
/*	 Fields usb1_wr_slverr_reported	 */
#define USB1_WR_SLVERR_REPORTED_WIDTH                                         1
#define USB1_WR_SLVERR_REPORTED_SHIFT                                         6
#define USB1_WR_SLVERR_REPORTED_MASK                                 0x00000040
#define USB1_WR_SLVERR_REPORTED_RD(src)               (((src) & 0x00000040)>>6)
#define USB1_WR_SLVERR_REPORTED_WR(src)          (((u32)(src)<<6) & 0x00000040)
#define USB1_WR_SLVERR_REPORTED_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*	 Fields usb1_rd_decerr_reported	 */
#define USB1_RD_DECERR_REPORTED_WIDTH                                         1
#define USB1_RD_DECERR_REPORTED_SHIFT                                         5
#define USB1_RD_DECERR_REPORTED_MASK                                 0x00000020
#define USB1_RD_DECERR_REPORTED_RD(src)               (((src) & 0x00000020)>>5)
#define USB1_RD_DECERR_REPORTED_WR(src)          (((u32)(src)<<5) & 0x00000020)
#define USB1_RD_DECERR_REPORTED_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields usb1_rd_slverr_reported	 */
#define USB1_RD_SLVERR_REPORTED_WIDTH                                         1
#define USB1_RD_SLVERR_REPORTED_SHIFT                                         4
#define USB1_RD_SLVERR_REPORTED_MASK                                 0x00000010
#define USB1_RD_SLVERR_REPORTED_RD(src)               (((src) & 0x00000010)>>4)
#define USB1_RD_SLVERR_REPORTED_WR(src)          (((u32)(src)<<4) & 0x00000010)
#define USB1_RD_SLVERR_REPORTED_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))

/*	Register ahbc_interruptMask	*/
/*    Mask Register Fields usbh2_wr_decerr_reportedMask    */
#define USBH2_WR_DECERR_REPORTEDMASK_WIDTH                                    1
#define USBH2_WR_DECERR_REPORTEDMASK_SHIFT                                   27
#define USBH2_WR_DECERR_REPORTEDMASK_MASK                            0x08000000
#define USBH2_WR_DECERR_REPORTEDMASK_RD(src)         (((src) & 0x08000000)>>27)
#define USBH2_WR_DECERR_REPORTEDMASK_WR(src)    (((u32)(src)<<27) & 0x08000000)
#define USBH2_WR_DECERR_REPORTEDMASK_SET(dst,src) \
                      (((dst) & ~0x08000000) | (((u32)(src)<<27) & 0x08000000))
/*    Mask Register Fields usbh2_wr_slverr_reportedMask    */
#define USBH2_WR_SLVERR_REPORTEDMASK_WIDTH                                    1
#define USBH2_WR_SLVERR_REPORTEDMASK_SHIFT                                   26
#define USBH2_WR_SLVERR_REPORTEDMASK_MASK                            0x04000000
#define USBH2_WR_SLVERR_REPORTEDMASK_RD(src)         (((src) & 0x04000000)>>26)
#define USBH2_WR_SLVERR_REPORTEDMASK_WR(src)    (((u32)(src)<<26) & 0x04000000)
#define USBH2_WR_SLVERR_REPORTEDMASK_SET(dst,src) \
                      (((dst) & ~0x04000000) | (((u32)(src)<<26) & 0x04000000))
/*    Mask Register Fields usbh2_rd_decerr_reportedMask    */
#define USBH2_RD_DECERR_REPORTEDMASK_WIDTH                                    1
#define USBH2_RD_DECERR_REPORTEDMASK_SHIFT                                   25
#define USBH2_RD_DECERR_REPORTEDMASK_MASK                            0x02000000
#define USBH2_RD_DECERR_REPORTEDMASK_RD(src)         (((src) & 0x02000000)>>25)
#define USBH2_RD_DECERR_REPORTEDMASK_WR(src)    (((u32)(src)<<25) & 0x02000000)
#define USBH2_RD_DECERR_REPORTEDMASK_SET(dst,src) \
                      (((dst) & ~0x02000000) | (((u32)(src)<<25) & 0x02000000))
/*    Mask Register Fields usbh2_rd_slverr_reportedMask    */
#define USBH2_RD_SLVERR_REPORTEDMASK_WIDTH                                    1
#define USBH2_RD_SLVERR_REPORTEDMASK_SHIFT                                   24
#define USBH2_RD_SLVERR_REPORTEDMASK_MASK                            0x01000000
#define USBH2_RD_SLVERR_REPORTEDMASK_RD(src)         (((src) & 0x01000000)>>24)
#define USBH2_RD_SLVERR_REPORTEDMASK_WR(src)    (((u32)(src)<<24) & 0x01000000)
#define USBH2_RD_SLVERR_REPORTEDMASK_SET(dst,src) \
                      (((dst) & ~0x01000000) | (((u32)(src)<<24) & 0x01000000))
/*    Mask Register Fields usbh1_wr_decerr_reportedMask    */
#define USBH1_WR_DECERR_REPORTEDMASK_WIDTH                                    1
#define USBH1_WR_DECERR_REPORTEDMASK_SHIFT                                   23
#define USBH1_WR_DECERR_REPORTEDMASK_MASK                            0x00800000
#define USBH1_WR_DECERR_REPORTEDMASK_RD(src)         (((src) & 0x00800000)>>23)
#define USBH1_WR_DECERR_REPORTEDMASK_WR(src)    (((u32)(src)<<23) & 0x00800000)
#define USBH1_WR_DECERR_REPORTEDMASK_SET(dst,src) \
                      (((dst) & ~0x00800000) | (((u32)(src)<<23) & 0x00800000))
/*    Mask Register Fields usbh1_wr_slverr_reportedMask    */
#define USBH1_WR_SLVERR_REPORTEDMASK_WIDTH                                    1
#define USBH1_WR_SLVERR_REPORTEDMASK_SHIFT                                   22
#define USBH1_WR_SLVERR_REPORTEDMASK_MASK                            0x00400000
#define USBH1_WR_SLVERR_REPORTEDMASK_RD(src)         (((src) & 0x00400000)>>22)
#define USBH1_WR_SLVERR_REPORTEDMASK_WR(src)    (((u32)(src)<<22) & 0x00400000)
#define USBH1_WR_SLVERR_REPORTEDMASK_SET(dst,src) \
                      (((dst) & ~0x00400000) | (((u32)(src)<<22) & 0x00400000))
/*    Mask Register Fields usbh1_rd_decerr_reportedMask    */
#define USBH1_RD_DECERR_REPORTEDMASK_WIDTH                                    1
#define USBH1_RD_DECERR_REPORTEDMASK_SHIFT                                   21
#define USBH1_RD_DECERR_REPORTEDMASK_MASK                            0x00200000
#define USBH1_RD_DECERR_REPORTEDMASK_RD(src)         (((src) & 0x00200000)>>21)
#define USBH1_RD_DECERR_REPORTEDMASK_WR(src)    (((u32)(src)<<21) & 0x00200000)
#define USBH1_RD_DECERR_REPORTEDMASK_SET(dst,src) \
                      (((dst) & ~0x00200000) | (((u32)(src)<<21) & 0x00200000))
/*    Mask Register Fields usbh1_rd_slverr_reportedMask    */
#define USBH1_RD_SLVERR_REPORTEDMASK_WIDTH                                    1
#define USBH1_RD_SLVERR_REPORTEDMASK_SHIFT                                   20
#define USBH1_RD_SLVERR_REPORTEDMASK_MASK                            0x00100000
#define USBH1_RD_SLVERR_REPORTEDMASK_RD(src)         (((src) & 0x00100000)>>20)
#define USBH1_RD_SLVERR_REPORTEDMASK_WR(src)    (((u32)(src)<<20) & 0x00100000)
#define USBH1_RD_SLVERR_REPORTEDMASK_SET(dst,src) \
                      (((dst) & ~0x00100000) | (((u32)(src)<<20) & 0x00100000))
/*    Mask Register Fields usbh_wr_decerr_reportedMask    */
#define USBH_WR_DECERR_REPORTEDMASK_WIDTH                                     1
#define USBH_WR_DECERR_REPORTEDMASK_SHIFT                                    15
#define USBH_WR_DECERR_REPORTEDMASK_MASK                             0x00008000
#define USBH_WR_DECERR_REPORTEDMASK_RD(src)          (((src) & 0x00008000)>>15)
#define USBH_WR_DECERR_REPORTEDMASK_WR(src)     (((u32)(src)<<15) & 0x00008000)
#define USBH_WR_DECERR_REPORTEDMASK_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))
/*    Mask Register Fields usbh_wr_slverr_reportedMask    */
#define USBH_WR_SLVERR_REPORTEDMASK_WIDTH                                     1
#define USBH_WR_SLVERR_REPORTEDMASK_SHIFT                                    14
#define USBH_WR_SLVERR_REPORTEDMASK_MASK                             0x00004000
#define USBH_WR_SLVERR_REPORTEDMASK_RD(src)          (((src) & 0x00004000)>>14)
#define USBH_WR_SLVERR_REPORTEDMASK_WR(src)     (((u32)(src)<<14) & 0x00004000)
#define USBH_WR_SLVERR_REPORTEDMASK_SET(dst,src) \
                      (((dst) & ~0x00004000) | (((u32)(src)<<14) & 0x00004000))
/*    Mask Register Fields usbh_rd_decerr_reportedMask    */
#define USBH_RD_DECERR_REPORTEDMASK_WIDTH                                     1
#define USBH_RD_DECERR_REPORTEDMASK_SHIFT                                    13
#define USBH_RD_DECERR_REPORTEDMASK_MASK                             0x00002000
#define USBH_RD_DECERR_REPORTEDMASK_RD(src)          (((src) & 0x00002000)>>13)
#define USBH_RD_DECERR_REPORTEDMASK_WR(src)     (((u32)(src)<<13) & 0x00002000)
#define USBH_RD_DECERR_REPORTEDMASK_SET(dst,src) \
                      (((dst) & ~0x00002000) | (((u32)(src)<<13) & 0x00002000))
/*    Mask Register Fields usbh_rd_slverr_reportedMask    */
#define USBH_RD_SLVERR_REPORTEDMASK_WIDTH                                     1
#define USBH_RD_SLVERR_REPORTEDMASK_SHIFT                                    12
#define USBH_RD_SLVERR_REPORTEDMASK_MASK                             0x00001000
#define USBH_RD_SLVERR_REPORTEDMASK_RD(src)          (((src) & 0x00001000)>>12)
#define USBH_RD_SLVERR_REPORTEDMASK_WR(src)     (((u32)(src)<<12) & 0x00001000)
#define USBH_RD_SLVERR_REPORTEDMASK_SET(dst,src) \
                      (((dst) & ~0x00001000) | (((u32)(src)<<12) & 0x00001000))
/*    Mask Register Fields sdhc_wr_decerr_reportedMask    */
#define SDHC_WR_DECERR_REPORTEDMASK_WIDTH                                     1
#define SDHC_WR_DECERR_REPORTEDMASK_SHIFT                                    11
#define SDHC_WR_DECERR_REPORTEDMASK_MASK                             0x00000800
#define SDHC_WR_DECERR_REPORTEDMASK_RD(src)          (((src) & 0x00000800)>>11)
#define SDHC_WR_DECERR_REPORTEDMASK_WR(src)     (((u32)(src)<<11) & 0x00000800)
#define SDHC_WR_DECERR_REPORTEDMASK_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*    Mask Register Fields sdhc_wr_slverr_reportedMask    */
#define SDHC_WR_SLVERR_REPORTEDMASK_WIDTH                                     1
#define SDHC_WR_SLVERR_REPORTEDMASK_SHIFT                                    10
#define SDHC_WR_SLVERR_REPORTEDMASK_MASK                             0x00000400
#define SDHC_WR_SLVERR_REPORTEDMASK_RD(src)          (((src) & 0x00000400)>>10)
#define SDHC_WR_SLVERR_REPORTEDMASK_WR(src)     (((u32)(src)<<10) & 0x00000400)
#define SDHC_WR_SLVERR_REPORTEDMASK_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*    Mask Register Fields sdhc_rd_decerr_reportedMask    */
#define SDHC_RD_DECERR_REPORTEDMASK_WIDTH                                     1
#define SDHC_RD_DECERR_REPORTEDMASK_SHIFT                                     9
#define SDHC_RD_DECERR_REPORTEDMASK_MASK                             0x00000200
#define SDHC_RD_DECERR_REPORTEDMASK_RD(src)           (((src) & 0x00000200)>>9)
#define SDHC_RD_DECERR_REPORTEDMASK_WR(src)      (((u32)(src)<<9) & 0x00000200)
#define SDHC_RD_DECERR_REPORTEDMASK_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*    Mask Register Fields sdhc_rd_slverr_reportedMask    */
#define SDHC_RD_SLVERR_REPORTEDMASK_WIDTH                                     1
#define SDHC_RD_SLVERR_REPORTEDMASK_SHIFT                                     8
#define SDHC_RD_SLVERR_REPORTEDMASK_MASK                             0x00000100
#define SDHC_RD_SLVERR_REPORTEDMASK_RD(src)           (((src) & 0x00000100)>>8)
#define SDHC_RD_SLVERR_REPORTEDMASK_WR(src)      (((u32)(src)<<8) & 0x00000100)
#define SDHC_RD_SLVERR_REPORTEDMASK_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*    Mask Register Fields usb1_wr_decerr_reportedMask    */
#define USB1_WR_DECERR_REPORTEDMASK_WIDTH                                     1
#define USB1_WR_DECERR_REPORTEDMASK_SHIFT                                     7
#define USB1_WR_DECERR_REPORTEDMASK_MASK                             0x00000080
#define USB1_WR_DECERR_REPORTEDMASK_RD(src)           (((src) & 0x00000080)>>7)
#define USB1_WR_DECERR_REPORTEDMASK_WR(src)      (((u32)(src)<<7) & 0x00000080)
#define USB1_WR_DECERR_REPORTEDMASK_SET(dst,src) \
                       (((dst) & ~0x00000080) | (((u32)(src)<<7) & 0x00000080))
/*    Mask Register Fields usb1_wr_slverr_reportedMask    */
#define USB1_WR_SLVERR_REPORTEDMASK_WIDTH                                     1
#define USB1_WR_SLVERR_REPORTEDMASK_SHIFT                                     6
#define USB1_WR_SLVERR_REPORTEDMASK_MASK                             0x00000040
#define USB1_WR_SLVERR_REPORTEDMASK_RD(src)           (((src) & 0x00000040)>>6)
#define USB1_WR_SLVERR_REPORTEDMASK_WR(src)      (((u32)(src)<<6) & 0x00000040)
#define USB1_WR_SLVERR_REPORTEDMASK_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*    Mask Register Fields usb1_rd_decerr_reportedMask    */
#define USB1_RD_DECERR_REPORTEDMASK_WIDTH                                     1
#define USB1_RD_DECERR_REPORTEDMASK_SHIFT                                     5
#define USB1_RD_DECERR_REPORTEDMASK_MASK                             0x00000020
#define USB1_RD_DECERR_REPORTEDMASK_RD(src)           (((src) & 0x00000020)>>5)
#define USB1_RD_DECERR_REPORTEDMASK_WR(src)      (((u32)(src)<<5) & 0x00000020)
#define USB1_RD_DECERR_REPORTEDMASK_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*    Mask Register Fields usb1_rd_slverr_reportedMask    */
#define USB1_RD_SLVERR_REPORTEDMASK_WIDTH                                     1
#define USB1_RD_SLVERR_REPORTEDMASK_SHIFT                                     4
#define USB1_RD_SLVERR_REPORTEDMASK_MASK                             0x00000010
#define USB1_RD_SLVERR_REPORTEDMASK_RD(src)           (((src) & 0x00000010)>>4)
#define USB1_RD_SLVERR_REPORTEDMASK_WR(src)      (((u32)(src)<<4) & 0x00000010)
#define USB1_RD_SLVERR_REPORTEDMASK_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))

/*	Register AIM_0_AHB	*/ 
/*	 Fields aim_ahb_address_n	 */
#define AIM_AHB_ADDRESS_N_WIDTH                                              20
#define AIM_AHB_ADDRESS_N_SHIFT                                               0
#define AIM_AHB_ADDRESS_N_MASK                                       0x000fffff
#define AIM_AHB_ADDRESS_N_RD(src)                        (((src) & 0x000fffff))
#define AIM_AHB_ADDRESS_N_WR(src)                   (((u32)(src)) & 0x000fffff)
#define AIM_AHB_ADDRESS_N_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AIM_0_SIZE_CTL	*/ 
/*	 Fields aim_en_n	 */
#define AIM_EN_N_WIDTH                                                        1
#define AIM_EN_N_SHIFT                                                       31
#define AIM_EN_N_MASK                                                0x80000000
#define AIM_EN_N_RD(src)                             (((src) & 0x80000000)>>31)
#define AIM_EN_N_WR(src)                        (((u32)(src)<<31) & 0x80000000)
#define AIM_EN_N_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields arsb	 */
#define ARSB_WIDTH                                                            3
#define ARSB_SHIFT                                                           23
#define ARSB_MASK                                                    0x03800000
#define ARSB_RD(src)                                 (((src) & 0x03800000)>>23)
#define ARSB_WR(src)                            (((u32)(src)<<23) & 0x03800000)
#define ARSB_SET(dst,src) \
                      (((dst) & ~0x03800000) | (((u32)(src)<<23) & 0x03800000))
/*	 Fields awsb	 */
#define AWSB_WIDTH                                                            3
#define AWSB_SHIFT                                                           20
#define AWSB_MASK                                                    0x00700000
#define AWSB_RD(src)                                 (((src) & 0x00700000)>>20)
#define AWSB_WR(src)                            (((u32)(src)<<20) & 0x00700000)
#define AWSB_SET(dst,src) \
                      (((dst) & ~0x00700000) | (((u32)(src)<<20) & 0x00700000))
/*	 Fields aim_mask_n	 */
#define AIM_MASK_N_WIDTH                                                     20
#define AIM_MASK_N_SHIFT                                                      0
#define AIM_MASK_N_MASK                                              0x000fffff
#define AIM_MASK_N_RD(src)                               (((src) & 0x000fffff))
#define AIM_MASK_N_WR(src)                          (((u32)(src)) & 0x000fffff)
#define AIM_MASK_N_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AIM_0_AXI_LO	*/ 
/*	 Fields aim_axi_address_LO_n	 */
#define AIM_AXI_ADDRESS_LO_N_WIDTH                                           20
#define AIM_AXI_ADDRESS_LO_N_SHIFT                                            0
#define AIM_AXI_ADDRESS_LO_N_MASK                                    0x000fffff
#define AIM_AXI_ADDRESS_LO_N_RD(src)                     (((src) & 0x000fffff))
#define AIM_AXI_ADDRESS_LO_N_WR(src)                (((u32)(src)) & 0x000fffff)
#define AIM_AXI_ADDRESS_LO_N_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AIM_0_AXI_HI	*/ 
/*	 Fields aim_axi_address_HI_n	 */
#define AIM_AXI_ADDRESS_HI_N_WIDTH                                            8
#define AIM_AXI_ADDRESS_HI_N_SHIFT                                           24
#define AIM_AXI_ADDRESS_HI_N_MASK                                    0xff000000
#define AIM_AXI_ADDRESS_HI_N_RD(src)                 (((src) & 0xff000000)>>24)
#define AIM_AXI_ADDRESS_HI_N_WR(src)            (((u32)(src)<<24) & 0xff000000)
#define AIM_AXI_ADDRESS_HI_N_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))

/*	Register AIM_1_AHB	*/ 
/*	 Fields aim_ahb_address_n	 */
#define AIM_AHB_ADDRESS_N_F1_WIDTH                                           20
#define AIM_AHB_ADDRESS_N_F1_SHIFT                                            0
#define AIM_AHB_ADDRESS_N_F1_MASK                                    0x000fffff
#define AIM_AHB_ADDRESS_N_F1_RD(src)                     (((src) & 0x000fffff))
#define AIM_AHB_ADDRESS_N_F1_WR(src)                (((u32)(src)) & 0x000fffff)
#define AIM_AHB_ADDRESS_N_F1_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AIM_1_SIZE_CTL	*/ 
/*	 Fields aim_en_n	 */
#define AIM_EN_N_F1_WIDTH                                                     1
#define AIM_EN_N_F1_SHIFT                                                    31
#define AIM_EN_N_F1_MASK                                             0x80000000
#define AIM_EN_N_F1_RD(src)                          (((src) & 0x80000000)>>31)
#define AIM_EN_N_F1_WR(src)                     (((u32)(src)<<31) & 0x80000000)
#define AIM_EN_N_F1_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields arsb	 */
#define ARSB_F1_WIDTH                                                         3
#define ARSB_F1_SHIFT                                                        23
#define ARSB_F1_MASK                                                 0x03800000
#define ARSB_F1_RD(src)                              (((src) & 0x03800000)>>23)
#define ARSB_F1_WR(src)                         (((u32)(src)<<23) & 0x03800000)
#define ARSB_F1_SET(dst,src) \
                      (((dst) & ~0x03800000) | (((u32)(src)<<23) & 0x03800000))
/*	 Fields awsb	 */
#define AWSB_F1_WIDTH                                                         3
#define AWSB_F1_SHIFT                                                        20
#define AWSB_F1_MASK                                                 0x00700000
#define AWSB_F1_RD(src)                              (((src) & 0x00700000)>>20)
#define AWSB_F1_WR(src)                         (((u32)(src)<<20) & 0x00700000)
#define AWSB_F1_SET(dst,src) \
                      (((dst) & ~0x00700000) | (((u32)(src)<<20) & 0x00700000))
/*	 Fields aim_mask_n	 */
#define AIM_MASK_N_F1_WIDTH                                                  20
#define AIM_MASK_N_F1_SHIFT                                                   0
#define AIM_MASK_N_F1_MASK                                           0x000fffff
#define AIM_MASK_N_F1_RD(src)                            (((src) & 0x000fffff))
#define AIM_MASK_N_F1_WR(src)                       (((u32)(src)) & 0x000fffff)
#define AIM_MASK_N_F1_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AIM_1_AXI_LO	*/ 
/*	 Fields aim_axi_address_LO_n	 */
#define AIM_AXI_ADDRESS_LO_N_F1_WIDTH                                        20
#define AIM_AXI_ADDRESS_LO_N_F1_SHIFT                                         0
#define AIM_AXI_ADDRESS_LO_N_F1_MASK                                 0x000fffff
#define AIM_AXI_ADDRESS_LO_N_F1_RD(src)                  (((src) & 0x000fffff))
#define AIM_AXI_ADDRESS_LO_N_F1_WR(src)             (((u32)(src)) & 0x000fffff)
#define AIM_AXI_ADDRESS_LO_N_F1_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AIM_1_AXI_HI	*/ 
/*	 Fields aim_axi_address_HI_n	 */
#define AIM_AXI_ADDRESS_HI_N_F1_WIDTH                                         8
#define AIM_AXI_ADDRESS_HI_N_F1_SHIFT                                        24
#define AIM_AXI_ADDRESS_HI_N_F1_MASK                                 0xff000000
#define AIM_AXI_ADDRESS_HI_N_F1_RD(src)              (((src) & 0xff000000)>>24)
#define AIM_AXI_ADDRESS_HI_N_F1_WR(src)         (((u32)(src)<<24) & 0xff000000)
#define AIM_AXI_ADDRESS_HI_N_F1_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))

/*	Register AIM_2_AHB	*/ 
/*	 Fields aim_ahb_address_n	 */
#define AIM_AHB_ADDRESS_N_F2_WIDTH                                           20
#define AIM_AHB_ADDRESS_N_F2_SHIFT                                            0
#define AIM_AHB_ADDRESS_N_F2_MASK                                    0x000fffff
#define AIM_AHB_ADDRESS_N_F2_RD(src)                     (((src) & 0x000fffff))
#define AIM_AHB_ADDRESS_N_F2_WR(src)                (((u32)(src)) & 0x000fffff)
#define AIM_AHB_ADDRESS_N_F2_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AIM_2_SIZE_CTL	*/ 
/*	 Fields aim_en_n	 */
#define AIM_EN_N_F2_WIDTH                                                     1
#define AIM_EN_N_F2_SHIFT                                                    31
#define AIM_EN_N_F2_MASK                                             0x80000000
#define AIM_EN_N_F2_RD(src)                          (((src) & 0x80000000)>>31)
#define AIM_EN_N_F2_WR(src)                     (((u32)(src)<<31) & 0x80000000)
#define AIM_EN_N_F2_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields arsb	 */
#define ARSB_F2_WIDTH                                                         3
#define ARSB_F2_SHIFT                                                        23
#define ARSB_F2_MASK                                                 0x03800000
#define ARSB_F2_RD(src)                              (((src) & 0x03800000)>>23)
#define ARSB_F2_WR(src)                         (((u32)(src)<<23) & 0x03800000)
#define ARSB_F2_SET(dst,src) \
                      (((dst) & ~0x03800000) | (((u32)(src)<<23) & 0x03800000))
/*	 Fields awsb	 */
#define AWSB_F2_WIDTH                                                         3
#define AWSB_F2_SHIFT                                                        20
#define AWSB_F2_MASK                                                 0x00700000
#define AWSB_F2_RD(src)                              (((src) & 0x00700000)>>20)
#define AWSB_F2_WR(src)                         (((u32)(src)<<20) & 0x00700000)
#define AWSB_F2_SET(dst,src) \
                      (((dst) & ~0x00700000) | (((u32)(src)<<20) & 0x00700000))
/*	 Fields aim_mask_n	 */
#define AIM_MASK_N_F2_WIDTH                                                  20
#define AIM_MASK_N_F2_SHIFT                                                   0
#define AIM_MASK_N_F2_MASK                                           0x000fffff
#define AIM_MASK_N_F2_RD(src)                            (((src) & 0x000fffff))
#define AIM_MASK_N_F2_WR(src)                       (((u32)(src)) & 0x000fffff)
#define AIM_MASK_N_F2_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AIM_2_AXI_LO	*/ 
/*	 Fields aim_axi_address_LO_n	 */
#define AIM_AXI_ADDRESS_LO_N_F2_WIDTH                                        20
#define AIM_AXI_ADDRESS_LO_N_F2_SHIFT                                         0
#define AIM_AXI_ADDRESS_LO_N_F2_MASK                                 0x000fffff
#define AIM_AXI_ADDRESS_LO_N_F2_RD(src)                  (((src) & 0x000fffff))
#define AIM_AXI_ADDRESS_LO_N_F2_WR(src)             (((u32)(src)) & 0x000fffff)
#define AIM_AXI_ADDRESS_LO_N_F2_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AIM_2_AXI_HI	*/ 
/*	 Fields aim_axi_address_HI_n	 */
#define AIM_AXI_ADDRESS_HI_N_F2_WIDTH                                         8
#define AIM_AXI_ADDRESS_HI_N_F2_SHIFT                                        24
#define AIM_AXI_ADDRESS_HI_N_F2_MASK                                 0xff000000
#define AIM_AXI_ADDRESS_HI_N_F2_RD(src)              (((src) & 0xff000000)>>24)
#define AIM_AXI_ADDRESS_HI_N_F2_WR(src)         (((u32)(src)<<24) & 0xff000000)
#define AIM_AXI_ADDRESS_HI_N_F2_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))

/*	Register AIM_3_AHB	*/ 
/*	 Fields aim_ahb_address_n	 */
#define AIM_AHB_ADDRESS_N_F3_WIDTH                                           20
#define AIM_AHB_ADDRESS_N_F3_SHIFT                                            0
#define AIM_AHB_ADDRESS_N_F3_MASK                                    0x000fffff
#define AIM_AHB_ADDRESS_N_F3_RD(src)                     (((src) & 0x000fffff))
#define AIM_AHB_ADDRESS_N_F3_WR(src)                (((u32)(src)) & 0x000fffff)
#define AIM_AHB_ADDRESS_N_F3_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AIM_3_SIZE_CTL	*/ 
/*	 Fields aim_en_n	 */
#define AIM_EN_N_F3_WIDTH                                                     1
#define AIM_EN_N_F3_SHIFT                                                    31
#define AIM_EN_N_F3_MASK                                             0x80000000
#define AIM_EN_N_F3_RD(src)                          (((src) & 0x80000000)>>31)
#define AIM_EN_N_F3_WR(src)                     (((u32)(src)<<31) & 0x80000000)
#define AIM_EN_N_F3_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields arsb	 */
#define ARSB_F3_WIDTH                                                         3
#define ARSB_F3_SHIFT                                                        23
#define ARSB_F3_MASK                                                 0x03800000
#define ARSB_F3_RD(src)                              (((src) & 0x03800000)>>23)
#define ARSB_F3_WR(src)                         (((u32)(src)<<23) & 0x03800000)
#define ARSB_F3_SET(dst,src) \
                      (((dst) & ~0x03800000) | (((u32)(src)<<23) & 0x03800000))
/*	 Fields awsb	 */
#define AWSB_F3_WIDTH                                                         3
#define AWSB_F3_SHIFT                                                        20
#define AWSB_F3_MASK                                                 0x00700000
#define AWSB_F3_RD(src)                              (((src) & 0x00700000)>>20)
#define AWSB_F3_WR(src)                         (((u32)(src)<<20) & 0x00700000)
#define AWSB_F3_SET(dst,src) \
                      (((dst) & ~0x00700000) | (((u32)(src)<<20) & 0x00700000))
/*	 Fields aim_mask_n	 */
#define AIM_MASK_N_F3_WIDTH                                                  20
#define AIM_MASK_N_F3_SHIFT                                                   0
#define AIM_MASK_N_F3_MASK                                           0x000fffff
#define AIM_MASK_N_F3_RD(src)                            (((src) & 0x000fffff))
#define AIM_MASK_N_F3_WR(src)                       (((u32)(src)) & 0x000fffff)
#define AIM_MASK_N_F3_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AIM_3_AXI_LO	*/ 
/*	 Fields aim_axi_address_LO_n	 */
#define AIM_AXI_ADDRESS_LO_N_F3_WIDTH                                        20
#define AIM_AXI_ADDRESS_LO_N_F3_SHIFT                                         0
#define AIM_AXI_ADDRESS_LO_N_F3_MASK                                 0x000fffff
#define AIM_AXI_ADDRESS_LO_N_F3_RD(src)                  (((src) & 0x000fffff))
#define AIM_AXI_ADDRESS_LO_N_F3_WR(src)             (((u32)(src)) & 0x000fffff)
#define AIM_AXI_ADDRESS_LO_N_F3_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AIM_3_AXI_HI	*/ 
/*	 Fields aim_axi_address_HI_n	 */
#define AIM_AXI_ADDRESS_HI_N_F3_WIDTH                                         8
#define AIM_AXI_ADDRESS_HI_N_F3_SHIFT                                        24
#define AIM_AXI_ADDRESS_HI_N_F3_MASK                                 0xff000000
#define AIM_AXI_ADDRESS_HI_N_F3_RD(src)              (((src) & 0xff000000)>>24)
#define AIM_AXI_ADDRESS_HI_N_F3_WR(src)         (((u32)(src)<<24) & 0xff000000)
#define AIM_AXI_ADDRESS_HI_N_F3_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))

/*	Register AIM_4_AHB	*/ 
/*	 Fields aim_ahb_address_n	 */
#define AIM_AHB_ADDRESS_N_F4_WIDTH                                           20
#define AIM_AHB_ADDRESS_N_F4_SHIFT                                            0
#define AIM_AHB_ADDRESS_N_F4_MASK                                    0x000fffff
#define AIM_AHB_ADDRESS_N_F4_RD(src)                     (((src) & 0x000fffff))
#define AIM_AHB_ADDRESS_N_F4_WR(src)                (((u32)(src)) & 0x000fffff)
#define AIM_AHB_ADDRESS_N_F4_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AIM_4_SIZE_CTL	*/ 
/*	 Fields aim_en_n	 */
#define AIM_EN_N_F4_WIDTH                                                     1
#define AIM_EN_N_F4_SHIFT                                                    31
#define AIM_EN_N_F4_MASK                                             0x80000000
#define AIM_EN_N_F4_RD(src)                          (((src) & 0x80000000)>>31)
#define AIM_EN_N_F4_WR(src)                     (((u32)(src)<<31) & 0x80000000)
#define AIM_EN_N_F4_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields arsb	 */
#define ARSB_F4_WIDTH                                                         3
#define ARSB_F4_SHIFT                                                        23
#define ARSB_F4_MASK                                                 0x03800000
#define ARSB_F4_RD(src)                              (((src) & 0x03800000)>>23)
#define ARSB_F4_WR(src)                         (((u32)(src)<<23) & 0x03800000)
#define ARSB_F4_SET(dst,src) \
                      (((dst) & ~0x03800000) | (((u32)(src)<<23) & 0x03800000))
/*	 Fields awsb	 */
#define AWSB_F4_WIDTH                                                         3
#define AWSB_F4_SHIFT                                                        20
#define AWSB_F4_MASK                                                 0x00700000
#define AWSB_F4_RD(src)                              (((src) & 0x00700000)>>20)
#define AWSB_F4_WR(src)                         (((u32)(src)<<20) & 0x00700000)
#define AWSB_F4_SET(dst,src) \
                      (((dst) & ~0x00700000) | (((u32)(src)<<20) & 0x00700000))
/*	 Fields aim_mask_n	 */
#define AIM_MASK_N_F4_WIDTH                                                  20
#define AIM_MASK_N_F4_SHIFT                                                   0
#define AIM_MASK_N_F4_MASK                                           0x000fffff
#define AIM_MASK_N_F4_RD(src)                            (((src) & 0x000fffff))
#define AIM_MASK_N_F4_WR(src)                       (((u32)(src)) & 0x000fffff)
#define AIM_MASK_N_F4_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AIM_4_AXI_LO	*/ 
/*	 Fields aim_axi_address_LO_n	 */
#define AIM_AXI_ADDRESS_LO_N_F4_WIDTH                                        20
#define AIM_AXI_ADDRESS_LO_N_F4_SHIFT                                         0
#define AIM_AXI_ADDRESS_LO_N_F4_MASK                                 0x000fffff
#define AIM_AXI_ADDRESS_LO_N_F4_RD(src)                  (((src) & 0x000fffff))
#define AIM_AXI_ADDRESS_LO_N_F4_WR(src)             (((u32)(src)) & 0x000fffff)
#define AIM_AXI_ADDRESS_LO_N_F4_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AIM_4_AXI_HI	*/ 
/*	 Fields aim_axi_address_HI_n	 */
#define AIM_AXI_ADDRESS_HI_N_F4_WIDTH                                         8
#define AIM_AXI_ADDRESS_HI_N_F4_SHIFT                                        24
#define AIM_AXI_ADDRESS_HI_N_F4_MASK                                 0xff000000
#define AIM_AXI_ADDRESS_HI_N_F4_RD(src)              (((src) & 0xff000000)>>24)
#define AIM_AXI_ADDRESS_HI_N_F4_WR(src)         (((u32)(src)<<24) & 0xff000000)
#define AIM_AXI_ADDRESS_HI_N_F4_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))

/*	Register AIM_5_AHB	*/ 
/*	 Fields aim_ahb_address_n	 */
#define AIM_AHB_ADDRESS_N_F5_WIDTH                                           20
#define AIM_AHB_ADDRESS_N_F5_SHIFT                                            0
#define AIM_AHB_ADDRESS_N_F5_MASK                                    0x000fffff
#define AIM_AHB_ADDRESS_N_F5_RD(src)                     (((src) & 0x000fffff))
#define AIM_AHB_ADDRESS_N_F5_WR(src)                (((u32)(src)) & 0x000fffff)
#define AIM_AHB_ADDRESS_N_F5_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AIM_5_SIZE_CTL	*/ 
/*	 Fields aim_en_n	 */
#define AIM_EN_N_F5_WIDTH                                                     1
#define AIM_EN_N_F5_SHIFT                                                    31
#define AIM_EN_N_F5_MASK                                             0x80000000
#define AIM_EN_N_F5_RD(src)                          (((src) & 0x80000000)>>31)
#define AIM_EN_N_F5_WR(src)                     (((u32)(src)<<31) & 0x80000000)
#define AIM_EN_N_F5_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields arsb	 */
#define ARSB_F5_WIDTH                                                         3
#define ARSB_F5_SHIFT                                                        23
#define ARSB_F5_MASK                                                 0x03800000
#define ARSB_F5_RD(src)                              (((src) & 0x03800000)>>23)
#define ARSB_F5_WR(src)                         (((u32)(src)<<23) & 0x03800000)
#define ARSB_F5_SET(dst,src) \
                      (((dst) & ~0x03800000) | (((u32)(src)<<23) & 0x03800000))
/*	 Fields awsb	 */
#define AWSB_F5_WIDTH                                                         3
#define AWSB_F5_SHIFT                                                        20
#define AWSB_F5_MASK                                                 0x00700000
#define AWSB_F5_RD(src)                              (((src) & 0x00700000)>>20)
#define AWSB_F5_WR(src)                         (((u32)(src)<<20) & 0x00700000)
#define AWSB_F5_SET(dst,src) \
                      (((dst) & ~0x00700000) | (((u32)(src)<<20) & 0x00700000))
/*	 Fields aim_mask_n	 */
#define AIM_MASK_N_F5_WIDTH                                                  20
#define AIM_MASK_N_F5_SHIFT                                                   0
#define AIM_MASK_N_F5_MASK                                           0x000fffff
#define AIM_MASK_N_F5_RD(src)                            (((src) & 0x000fffff))
#define AIM_MASK_N_F5_WR(src)                       (((u32)(src)) & 0x000fffff)
#define AIM_MASK_N_F5_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AIM_5_AXI_LO	*/ 
/*	 Fields aim_axi_address_LO_n	 */
#define AIM_AXI_ADDRESS_LO_N_F5_WIDTH                                        20
#define AIM_AXI_ADDRESS_LO_N_F5_SHIFT                                         0
#define AIM_AXI_ADDRESS_LO_N_F5_MASK                                 0x000fffff
#define AIM_AXI_ADDRESS_LO_N_F5_RD(src)                  (((src) & 0x000fffff))
#define AIM_AXI_ADDRESS_LO_N_F5_WR(src)             (((u32)(src)) & 0x000fffff)
#define AIM_AXI_ADDRESS_LO_N_F5_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AIM_5_AXI_HI	*/ 
/*	 Fields aim_axi_address_HI_n	 */
#define AIM_AXI_ADDRESS_HI_N_F5_WIDTH                                         8
#define AIM_AXI_ADDRESS_HI_N_F5_SHIFT                                        24
#define AIM_AXI_ADDRESS_HI_N_F5_MASK                                 0xff000000
#define AIM_AXI_ADDRESS_HI_N_F5_RD(src)              (((src) & 0xff000000)>>24)
#define AIM_AXI_ADDRESS_HI_N_F5_WR(src)         (((u32)(src)<<24) & 0xff000000)
#define AIM_AXI_ADDRESS_HI_N_F5_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))

/*	Register AIM_6_AHB	*/ 
/*	 Fields aim_ahb_address_n	 */
#define AIM_AHB_ADDRESS_N_F6_WIDTH                                           20
#define AIM_AHB_ADDRESS_N_F6_SHIFT                                            0
#define AIM_AHB_ADDRESS_N_F6_MASK                                    0x000fffff
#define AIM_AHB_ADDRESS_N_F6_RD(src)                     (((src) & 0x000fffff))
#define AIM_AHB_ADDRESS_N_F6_WR(src)                (((u32)(src)) & 0x000fffff)
#define AIM_AHB_ADDRESS_N_F6_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AIM_6_SIZE_CTL	*/ 
/*	 Fields aim_en_n	 */
#define AIM_EN_N_F6_WIDTH                                                     1
#define AIM_EN_N_F6_SHIFT                                                    31
#define AIM_EN_N_F6_MASK                                             0x80000000
#define AIM_EN_N_F6_RD(src)                          (((src) & 0x80000000)>>31)
#define AIM_EN_N_F6_WR(src)                     (((u32)(src)<<31) & 0x80000000)
#define AIM_EN_N_F6_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields arsb	 */
#define ARSB_F6_WIDTH                                                         3
#define ARSB_F6_SHIFT                                                        23
#define ARSB_F6_MASK                                                 0x03800000
#define ARSB_F6_RD(src)                              (((src) & 0x03800000)>>23)
#define ARSB_F6_WR(src)                         (((u32)(src)<<23) & 0x03800000)
#define ARSB_F6_SET(dst,src) \
                      (((dst) & ~0x03800000) | (((u32)(src)<<23) & 0x03800000))
/*	 Fields awsb	 */
#define AWSB_F6_WIDTH                                                         3
#define AWSB_F6_SHIFT                                                        20
#define AWSB_F6_MASK                                                 0x00700000
#define AWSB_F6_RD(src)                              (((src) & 0x00700000)>>20)
#define AWSB_F6_WR(src)                         (((u32)(src)<<20) & 0x00700000)
#define AWSB_F6_SET(dst,src) \
                      (((dst) & ~0x00700000) | (((u32)(src)<<20) & 0x00700000))
/*	 Fields aim_mask_n	 */
#define AIM_MASK_N_F6_WIDTH                                                  20
#define AIM_MASK_N_F6_SHIFT                                                   0
#define AIM_MASK_N_F6_MASK                                           0x000fffff
#define AIM_MASK_N_F6_RD(src)                            (((src) & 0x000fffff))
#define AIM_MASK_N_F6_WR(src)                       (((u32)(src)) & 0x000fffff)
#define AIM_MASK_N_F6_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AIM_6_AXI_LO	*/ 
/*	 Fields aim_axi_address_LO_n	 */
#define AIM_AXI_ADDRESS_LO_N_F6_WIDTH                                        20
#define AIM_AXI_ADDRESS_LO_N_F6_SHIFT                                         0
#define AIM_AXI_ADDRESS_LO_N_F6_MASK                                 0x000fffff
#define AIM_AXI_ADDRESS_LO_N_F6_RD(src)                  (((src) & 0x000fffff))
#define AIM_AXI_ADDRESS_LO_N_F6_WR(src)             (((u32)(src)) & 0x000fffff)
#define AIM_AXI_ADDRESS_LO_N_F6_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AIM_6_AXI_HI	*/ 
/*	 Fields aim_axi_address_HI_n	 */
#define AIM_AXI_ADDRESS_HI_N_F6_WIDTH                                         8
#define AIM_AXI_ADDRESS_HI_N_F6_SHIFT                                        24
#define AIM_AXI_ADDRESS_HI_N_F6_MASK                                 0xff000000
#define AIM_AXI_ADDRESS_HI_N_F6_RD(src)              (((src) & 0xff000000)>>24)
#define AIM_AXI_ADDRESS_HI_N_F6_WR(src)         (((u32)(src)<<24) & 0xff000000)
#define AIM_AXI_ADDRESS_HI_N_F6_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))

/*	Register AIM_7_AHB	*/ 
/*	 Fields aim_ahb_address_n	 */
#define AIM_AHB_ADDRESS_N_F7_WIDTH                                           20
#define AIM_AHB_ADDRESS_N_F7_SHIFT                                            0
#define AIM_AHB_ADDRESS_N_F7_MASK                                    0x000fffff
#define AIM_AHB_ADDRESS_N_F7_RD(src)                     (((src) & 0x000fffff))
#define AIM_AHB_ADDRESS_N_F7_WR(src)                (((u32)(src)) & 0x000fffff)
#define AIM_AHB_ADDRESS_N_F7_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AIM_7_SIZE_CTL	*/ 
/*	 Fields aim_en_n	 */
#define AIM_EN_N_F7_WIDTH                                                     1
#define AIM_EN_N_F7_SHIFT                                                    31
#define AIM_EN_N_F7_MASK                                             0x80000000
#define AIM_EN_N_F7_RD(src)                          (((src) & 0x80000000)>>31)
#define AIM_EN_N_F7_WR(src)                     (((u32)(src)<<31) & 0x80000000)
#define AIM_EN_N_F7_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields arsb	 */
#define ARSB_F7_WIDTH                                                         3
#define ARSB_F7_SHIFT                                                        23
#define ARSB_F7_MASK                                                 0x03800000
#define ARSB_F7_RD(src)                              (((src) & 0x03800000)>>23)
#define ARSB_F7_WR(src)                         (((u32)(src)<<23) & 0x03800000)
#define ARSB_F7_SET(dst,src) \
                      (((dst) & ~0x03800000) | (((u32)(src)<<23) & 0x03800000))
/*	 Fields awsb	 */
#define AWSB_F7_WIDTH                                                         3
#define AWSB_F7_SHIFT                                                        20
#define AWSB_F7_MASK                                                 0x00700000
#define AWSB_F7_RD(src)                              (((src) & 0x00700000)>>20)
#define AWSB_F7_WR(src)                         (((u32)(src)<<20) & 0x00700000)
#define AWSB_F7_SET(dst,src) \
                      (((dst) & ~0x00700000) | (((u32)(src)<<20) & 0x00700000))
/*	 Fields aim_mask_n	 */
#define AIM_MASK_N_F7_WIDTH                                                  20
#define AIM_MASK_N_F7_SHIFT                                                   0
#define AIM_MASK_N_F7_MASK                                           0x000fffff
#define AIM_MASK_N_F7_RD(src)                            (((src) & 0x000fffff))
#define AIM_MASK_N_F7_WR(src)                       (((u32)(src)) & 0x000fffff)
#define AIM_MASK_N_F7_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AIM_7_AXI_LO	*/ 
/*	 Fields aim_axi_address_LO_n	 */
#define AIM_AXI_ADDRESS_LO_N_F7_WIDTH                                        20
#define AIM_AXI_ADDRESS_LO_N_F7_SHIFT                                         0
#define AIM_AXI_ADDRESS_LO_N_F7_MASK                                 0x000fffff
#define AIM_AXI_ADDRESS_LO_N_F7_RD(src)                  (((src) & 0x000fffff))
#define AIM_AXI_ADDRESS_LO_N_F7_WR(src)             (((u32)(src)) & 0x000fffff)
#define AIM_AXI_ADDRESS_LO_N_F7_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AIM_7_AXI_HI	*/ 
/*	 Fields aim_axi_address_HI_n	 */
#define AIM_AXI_ADDRESS_HI_N_F7_WIDTH                                         8
#define AIM_AXI_ADDRESS_HI_N_F7_SHIFT                                        24
#define AIM_AXI_ADDRESS_HI_N_F7_MASK                                 0xff000000
#define AIM_AXI_ADDRESS_HI_N_F7_RD(src)              (((src) & 0xff000000)>>24)
#define AIM_AXI_ADDRESS_HI_N_F7_WR(src)         (((u32)(src)<<24) & 0xff000000)
#define AIM_AXI_ADDRESS_HI_N_F7_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))

/*	Register AIM_8_SIZE_CTL	*/ 
/*	 Fields aim_mask_8	 */
#define AIM_MASK_8_WIDTH                                                     20
#define AIM_MASK_8_SHIFT                                                      0
#define AIM_MASK_8_MASK                                              0x000fffff
#define AIM_MASK_8_RD(src)                               (((src) & 0x000fffff))
#define AIM_MASK_8_WR(src)                          (((u32)(src)) & 0x000fffff)
#define AIM_MASK_8_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AIM_8_AXI_LO	*/ 
/*	 Fields arsb	 */
#define ARSB_F8_WIDTH                                                         3
#define ARSB_F8_SHIFT                                                        23
#define ARSB_F8_MASK                                                 0x03800000
#define ARSB_F8_RD(src)                              (((src) & 0x03800000)>>23)
#define ARSB_F8_WR(src)                         (((u32)(src)<<23) & 0x03800000)
#define ARSB_F8_SET(dst,src) \
                      (((dst) & ~0x03800000) | (((u32)(src)<<23) & 0x03800000))
/*	 Fields awsb	 */
#define AWSB_F8_WIDTH                                                         3
#define AWSB_F8_SHIFT                                                        20
#define AWSB_F8_MASK                                                 0x00700000
#define AWSB_F8_RD(src)                              (((src) & 0x00700000)>>20)
#define AWSB_F8_WR(src)                         (((u32)(src)<<20) & 0x00700000)
#define AWSB_F8_SET(dst,src) \
                      (((dst) & ~0x00700000) | (((u32)(src)<<20) & 0x00700000))
/*	 Fields aim_axi_address_LO_8	 */
#define AIM_AXI_ADDRESS_LO_8_WIDTH                                           20
#define AIM_AXI_ADDRESS_LO_8_SHIFT                                            0
#define AIM_AXI_ADDRESS_LO_8_MASK                                    0x000fffff
#define AIM_AXI_ADDRESS_LO_8_RD(src)                     (((src) & 0x000fffff))
#define AIM_AXI_ADDRESS_LO_8_WR(src)                (((u32)(src)) & 0x000fffff)
#define AIM_AXI_ADDRESS_LO_8_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AIM_8_AXI_HI	*/ 
/*	 Fields aim_axi_address_HI_8	 */
#define AIM_AXI_ADDRESS_HI_8_WIDTH                                            8
#define AIM_AXI_ADDRESS_HI_8_SHIFT                                           24
#define AIM_AXI_ADDRESS_HI_8_MASK                                    0xff000000
#define AIM_AXI_ADDRESS_HI_8_RD(src)                 (((src) & 0xff000000)>>24)
#define AIM_AXI_ADDRESS_HI_8_WR(src)            (((u32)(src)<<24) & 0xff000000)
#define AIM_AXI_ADDRESS_HI_8_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))

/*	Register AOM_0_AXI_LO	*/ 
/*	 Fields aom_axi_address_LO_n	 */
#define AOM_AXI_ADDRESS_LO_N_WIDTH                                           20
#define AOM_AXI_ADDRESS_LO_N_SHIFT                                            0
#define AOM_AXI_ADDRESS_LO_N_MASK                                    0x000fffff
#define AOM_AXI_ADDRESS_LO_N_RD(src)                     (((src) & 0x000fffff))
#define AOM_AXI_ADDRESS_LO_N_WR(src)                (((u32)(src)) & 0x000fffff)
#define AOM_AXI_ADDRESS_LO_N_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AOM_0_AXI_HI	*/ 
/*	 Fields aom_axi_address_HI_n	 */
#define AOM_AXI_ADDRESS_HI_N_WIDTH                                            8
#define AOM_AXI_ADDRESS_HI_N_SHIFT                                           24
#define AOM_AXI_ADDRESS_HI_N_MASK                                    0xff000000
#define AOM_AXI_ADDRESS_HI_N_RD(src)                 (((src) & 0xff000000)>>24)
#define AOM_AXI_ADDRESS_HI_N_WR(src)            (((u32)(src)<<24) & 0xff000000)
#define AOM_AXI_ADDRESS_HI_N_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))

/*	Register AOM_0_SIZE_CTL	*/ 
/*	 Fields aom_en_n	 */
#define AOM_EN_N_WIDTH                                                        1
#define AOM_EN_N_SHIFT                                                       31
#define AOM_EN_N_MASK                                                0x80000000
#define AOM_EN_N_RD(src)                             (((src) & 0x80000000)>>31)
#define AOM_EN_N_WR(src)                        (((u32)(src)<<31) & 0x80000000)
#define AOM_EN_N_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields aom_mask_n	 */
#define AOM_MASK_N_WIDTH                                                     20
#define AOM_MASK_N_SHIFT                                                      0
#define AOM_MASK_N_MASK                                              0x000fffff
#define AOM_MASK_N_RD(src)                               (((src) & 0x000fffff))
#define AOM_MASK_N_WR(src)                          (((u32)(src)) & 0x000fffff)
#define AOM_MASK_N_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AOM_0_AHB	*/ 
/*	 Fields aom_axi_address_n	 */
#define AOM_AXI_ADDRESS_N_WIDTH                                              20
#define AOM_AXI_ADDRESS_N_SHIFT                                               0
#define AOM_AXI_ADDRESS_N_MASK                                       0x000fffff
#define AOM_AXI_ADDRESS_N_RD(src)                        (((src) & 0x000fffff))
#define AOM_AXI_ADDRESS_N_WR(src)                   (((u32)(src)) & 0x000fffff)
#define AOM_AXI_ADDRESS_N_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AOM_1_AXI_LO	*/ 
/*	 Fields aom_axi_address_LO_n	 */
#define AOM_AXI_ADDRESS_LO_N_F1_WIDTH                                        20
#define AOM_AXI_ADDRESS_LO_N_F1_SHIFT                                         0
#define AOM_AXI_ADDRESS_LO_N_F1_MASK                                 0x000fffff
#define AOM_AXI_ADDRESS_LO_N_F1_RD(src)                  (((src) & 0x000fffff))
#define AOM_AXI_ADDRESS_LO_N_F1_WR(src)             (((u32)(src)) & 0x000fffff)
#define AOM_AXI_ADDRESS_LO_N_F1_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AOM_1_AXI_HI	*/ 
/*	 Fields aom_axi_address_HI_n	 */
#define AOM_AXI_ADDRESS_HI_N_F1_WIDTH                                         8
#define AOM_AXI_ADDRESS_HI_N_F1_SHIFT                                        24
#define AOM_AXI_ADDRESS_HI_N_F1_MASK                                 0xff000000
#define AOM_AXI_ADDRESS_HI_N_F1_RD(src)              (((src) & 0xff000000)>>24)
#define AOM_AXI_ADDRESS_HI_N_F1_WR(src)         (((u32)(src)<<24) & 0xff000000)
#define AOM_AXI_ADDRESS_HI_N_F1_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))

/*	Register AOM_1_SIZE_CTL	*/ 
/*	 Fields aom_en_n	 */
#define AOM_EN_N_F1_WIDTH                                                     1
#define AOM_EN_N_F1_SHIFT                                                    31
#define AOM_EN_N_F1_MASK                                             0x80000000
#define AOM_EN_N_F1_RD(src)                          (((src) & 0x80000000)>>31)
#define AOM_EN_N_F1_WR(src)                     (((u32)(src)<<31) & 0x80000000)
#define AOM_EN_N_F1_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields aom_mask_n	 */
#define AOM_MASK_N_F1_WIDTH                                                  20
#define AOM_MASK_N_F1_SHIFT                                                   0
#define AOM_MASK_N_F1_MASK                                           0x000fffff
#define AOM_MASK_N_F1_RD(src)                            (((src) & 0x000fffff))
#define AOM_MASK_N_F1_WR(src)                       (((u32)(src)) & 0x000fffff)
#define AOM_MASK_N_F1_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AOM_1_AHB	*/ 
/*	 Fields aom_axi_address_n	 */
#define AOM_AXI_ADDRESS_N_F1_WIDTH                                           20
#define AOM_AXI_ADDRESS_N_F1_SHIFT                                            0
#define AOM_AXI_ADDRESS_N_F1_MASK                                    0x000fffff
#define AOM_AXI_ADDRESS_N_F1_RD(src)                     (((src) & 0x000fffff))
#define AOM_AXI_ADDRESS_N_F1_WR(src)                (((u32)(src)) & 0x000fffff)
#define AOM_AXI_ADDRESS_N_F1_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AOM_2_AXI_LO	*/ 
/*	 Fields aom_axi_address_LO_n	 */
#define AOM_AXI_ADDRESS_LO_N_F2_WIDTH                                        20
#define AOM_AXI_ADDRESS_LO_N_F2_SHIFT                                         0
#define AOM_AXI_ADDRESS_LO_N_F2_MASK                                 0x000fffff
#define AOM_AXI_ADDRESS_LO_N_F2_RD(src)                  (((src) & 0x000fffff))
#define AOM_AXI_ADDRESS_LO_N_F2_WR(src)             (((u32)(src)) & 0x000fffff)
#define AOM_AXI_ADDRESS_LO_N_F2_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AOM_2_AXI_HI	*/ 
/*	 Fields aom_axi_address_HI_n	 */
#define AOM_AXI_ADDRESS_HI_N_F2_WIDTH                                         8
#define AOM_AXI_ADDRESS_HI_N_F2_SHIFT                                        24
#define AOM_AXI_ADDRESS_HI_N_F2_MASK                                 0xff000000
#define AOM_AXI_ADDRESS_HI_N_F2_RD(src)              (((src) & 0xff000000)>>24)
#define AOM_AXI_ADDRESS_HI_N_F2_WR(src)         (((u32)(src)<<24) & 0xff000000)
#define AOM_AXI_ADDRESS_HI_N_F2_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))

/*	Register AOM_2_SIZE_CTL	*/ 
/*	 Fields aom_en_n	 */
#define AOM_EN_N_F2_WIDTH                                                     1
#define AOM_EN_N_F2_SHIFT                                                    31
#define AOM_EN_N_F2_MASK                                             0x80000000
#define AOM_EN_N_F2_RD(src)                          (((src) & 0x80000000)>>31)
#define AOM_EN_N_F2_WR(src)                     (((u32)(src)<<31) & 0x80000000)
#define AOM_EN_N_F2_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields aom_mask_n	 */
#define AOM_MASK_N_F2_WIDTH                                                  20
#define AOM_MASK_N_F2_SHIFT                                                   0
#define AOM_MASK_N_F2_MASK                                           0x000fffff
#define AOM_MASK_N_F2_RD(src)                            (((src) & 0x000fffff))
#define AOM_MASK_N_F2_WR(src)                       (((u32)(src)) & 0x000fffff)
#define AOM_MASK_N_F2_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AOM_2_AHB	*/ 
/*	 Fields aom_axi_address_n	 */
#define AOM_AXI_ADDRESS_N_F2_WIDTH                                           20
#define AOM_AXI_ADDRESS_N_F2_SHIFT                                            0
#define AOM_AXI_ADDRESS_N_F2_MASK                                    0x000fffff
#define AOM_AXI_ADDRESS_N_F2_RD(src)                     (((src) & 0x000fffff))
#define AOM_AXI_ADDRESS_N_F2_WR(src)                (((u32)(src)) & 0x000fffff)
#define AOM_AXI_ADDRESS_N_F2_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AOM_3_AXI_LO	*/ 
/*	 Fields aom_axi_address_LO_n	 */
#define AOM_AXI_ADDRESS_LO_N_F3_WIDTH                                        20
#define AOM_AXI_ADDRESS_LO_N_F3_SHIFT                                         0
#define AOM_AXI_ADDRESS_LO_N_F3_MASK                                 0x000fffff
#define AOM_AXI_ADDRESS_LO_N_F3_RD(src)                  (((src) & 0x000fffff))
#define AOM_AXI_ADDRESS_LO_N_F3_WR(src)             (((u32)(src)) & 0x000fffff)
#define AOM_AXI_ADDRESS_LO_N_F3_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AOM_3_AXI_HI	*/ 
/*	 Fields aom_axi_address_HI_n	 */
#define AOM_AXI_ADDRESS_HI_N_F3_WIDTH                                         8
#define AOM_AXI_ADDRESS_HI_N_F3_SHIFT                                        24
#define AOM_AXI_ADDRESS_HI_N_F3_MASK                                 0xff000000
#define AOM_AXI_ADDRESS_HI_N_F3_RD(src)              (((src) & 0xff000000)>>24)
#define AOM_AXI_ADDRESS_HI_N_F3_WR(src)         (((u32)(src)<<24) & 0xff000000)
#define AOM_AXI_ADDRESS_HI_N_F3_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))

/*	Register AOM_3_SIZE_CTL	*/ 
/*	 Fields aom_en_n	 */
#define AOM_EN_N_F3_WIDTH                                                     1
#define AOM_EN_N_F3_SHIFT                                                    31
#define AOM_EN_N_F3_MASK                                             0x80000000
#define AOM_EN_N_F3_RD(src)                          (((src) & 0x80000000)>>31)
#define AOM_EN_N_F3_WR(src)                     (((u32)(src)<<31) & 0x80000000)
#define AOM_EN_N_F3_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields aom_mask_n	 */
#define AOM_MASK_N_F3_WIDTH                                                  20
#define AOM_MASK_N_F3_SHIFT                                                   0
#define AOM_MASK_N_F3_MASK                                           0x000fffff
#define AOM_MASK_N_F3_RD(src)                            (((src) & 0x000fffff))
#define AOM_MASK_N_F3_WR(src)                       (((u32)(src)) & 0x000fffff)
#define AOM_MASK_N_F3_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AOM_3_AHB	*/ 
/*	 Fields aom_axi_address_n	 */
#define AOM_AXI_ADDRESS_N_F3_WIDTH                                           20
#define AOM_AXI_ADDRESS_N_F3_SHIFT                                            0
#define AOM_AXI_ADDRESS_N_F3_MASK                                    0x000fffff
#define AOM_AXI_ADDRESS_N_F3_RD(src)                     (((src) & 0x000fffff))
#define AOM_AXI_ADDRESS_N_F3_WR(src)                (((u32)(src)) & 0x000fffff)
#define AOM_AXI_ADDRESS_N_F3_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register ahb_rd_err_addr	*/ 
/*	 Fields addr	 */
#define AHBC_ADDR_WIDTH                                                      32
#define AHBC_ADDR_SHIFT                                                       0
#define AHBC_ADDR_MASK                                               0xffffffff
#define AHBC_ADDR_RD(src)                                (((src) & 0xffffffff))
#define AHBC_ADDR_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register ahb_rd_err_attributes	*/ 
/*	 Fields ahb_htrans	 */
#define AHB_HTRANS_WIDTH                                                      2
#define AHB_HTRANS_SHIFT                                                     11
#define AHB_HTRANS_MASK                                              0x00001800
#define AHB_HTRANS_RD(src)                           (((src) & 0x00001800)>>11)
#define AHB_HTRANS_SET(dst,src) \
                      (((dst) & ~0x00001800) | (((u32)(src)<<11) & 0x00001800))
/*	 Fields ahb_hsize	 */
#define AHB_HSIZE_WIDTH                                                       3
#define AHB_HSIZE_SHIFT                                                       8
#define AHB_HSIZE_MASK                                               0x00000700
#define AHB_HSIZE_RD(src)                             (((src) & 0x00000700)>>8)
#define AHB_HSIZE_SET(dst,src) \
                       (((dst) & ~0x00000700) | (((u32)(src)<<8) & 0x00000700))
/*	 Fields ahb_hburst	 */
#define AHB_HBURST_WIDTH                                                      3
#define AHB_HBURST_SHIFT                                                      5
#define AHB_HBURST_MASK                                              0x000000e0
#define AHB_HBURST_RD(src)                            (((src) & 0x000000e0)>>5)
#define AHB_HBURST_SET(dst,src) \
                       (((dst) & ~0x000000e0) | (((u32)(src)<<5) & 0x000000e0))
/*	 Fields err_addr_decode	 */
#define ERR_ADDR_DECODE_WIDTH                                                 1
#define ERR_ADDR_DECODE_SHIFT                                                 4
#define ERR_ADDR_DECODE_MASK                                         0x00000010
#define ERR_ADDR_DECODE_RD(src)                       (((src) & 0x00000010)>>4)
#define ERR_ADDR_DECODE_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields ahb_rd_err	 */
#define AHB_RD_ERR_WIDTH                                                      1
#define AHB_RD_ERR_SHIFT                                                      0
#define AHB_RD_ERR_MASK                                              0x00000001
#define AHB_RD_ERR_RD(src)                               (((src) & 0x00000001))
#define AHB_RD_ERR_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register ahb_wr_err_addr	*/ 
/*	 Fields addr	 */
#define AHBC_ADDR_F1_WIDTH                                                   32
#define AHBC_ADDR_F1_SHIFT                                                    0
#define AHBC_ADDR_F1_MASK                                            0xffffffff
#define AHBC_ADDR_F1_RD(src)                             (((src) & 0xffffffff))
#define AHBC_ADDR_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register ahb_wr_err_data	*/ 
/*	 Fields data	 */
#define DATA_WIDTH                                                           32
#define DATA_SHIFT                                                            0
#define DATA_MASK                                                    0xffffffff
#define DATA_RD(src)                                     (((src) & 0xffffffff))
#define DATA_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register ahb_wr_err_attributes	*/ 
/*	 Fields ahb_htrans	 */
#define AHB_HTRANS_F1_WIDTH                                                   2
#define AHB_HTRANS_F1_SHIFT                                                  11
#define AHB_HTRANS_F1_MASK                                           0x00001800
#define AHB_HTRANS_F1_RD(src)                        (((src) & 0x00001800)>>11)
#define AHB_HTRANS_F1_SET(dst,src) \
                      (((dst) & ~0x00001800) | (((u32)(src)<<11) & 0x00001800))
/*	 Fields ahb_hsize	 */
#define AHB_HSIZE_F1_WIDTH                                                    3
#define AHB_HSIZE_F1_SHIFT                                                    8
#define AHB_HSIZE_F1_MASK                                            0x00000700
#define AHB_HSIZE_F1_RD(src)                          (((src) & 0x00000700)>>8)
#define AHB_HSIZE_F1_SET(dst,src) \
                       (((dst) & ~0x00000700) | (((u32)(src)<<8) & 0x00000700))
/*	 Fields ahb_hburst	 */
#define AHB_HBURST_F1_WIDTH                                                   3
#define AHB_HBURST_F1_SHIFT                                                   5
#define AHB_HBURST_F1_MASK                                           0x000000e0
#define AHB_HBURST_F1_RD(src)                         (((src) & 0x000000e0)>>5)
#define AHB_HBURST_F1_SET(dst,src) \
                       (((dst) & ~0x000000e0) | (((u32)(src)<<5) & 0x000000e0))
/*	 Fields err_addr_decode	 */
#define ERR_ADDR_DECODE_F1_WIDTH                                              1
#define ERR_ADDR_DECODE_F1_SHIFT                                              4
#define ERR_ADDR_DECODE_F1_MASK                                      0x00000010
#define ERR_ADDR_DECODE_F1_RD(src)                    (((src) & 0x00000010)>>4)
#define ERR_ADDR_DECODE_F1_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields ahb_wr_err	 */
#define AHB_WR_ERR_WIDTH                                                      1
#define AHB_WR_ERR_SHIFT                                                      0
#define AHB_WR_ERR_MASK                                              0x00000001
#define AHB_WR_ERR_RD(src)                               (((src) & 0x00000001))
#define AHB_WR_ERR_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register CFG_INTR_CTL	*/ 
/*	 Fields ahbc_i2c0_int	 */
#define AHBC_I2C0_INT_WIDTH                                                   1
#define AHBC_I2C0_INT_SHIFT                                                  27
#define AHBC_I2C0_INT_MASK                                           0x08000000
#define AHBC_I2C0_INT_RD(src)                        (((src) & 0x08000000)>>27)
#define AHBC_I2C0_INT_WR(src)                   (((u32)(src)<<27) & 0x08000000)
#define AHBC_I2C0_INT_SET(dst,src) \
                      (((dst) & ~0x08000000) | (((u32)(src)<<27) & 0x08000000))
/*	 Fields ahbc_gpiopin_int	 */
#define AHBC_GPIOPIN_INT_WIDTH                                               11
#define AHBC_GPIOPIN_INT_SHIFT                                               16
#define AHBC_GPIOPIN_INT_MASK                                        0x07ff0000
#define AHBC_GPIOPIN_INT_RD(src)                     (((src) & 0x07ff0000)>>16)
#define AHBC_GPIOPIN_INT_WR(src)                (((u32)(src)<<16) & 0x07ff0000)
#define AHBC_GPIOPIN_INT_SET(dst,src) \
                      (((dst) & ~0x07ff0000) | (((u32)(src)<<16) & 0x07ff0000))
/*	 Fields hdlc_int	 */
#define HDLC_INT_WIDTH                                                        1
#define HDLC_INT_SHIFT                                                       15
#define HDLC_INT_MASK                                                0x00008000
#define HDLC_INT_RD(src)                             (((src) & 0x00008000)>>15)
#define HDLC_INT_WR(src)                        (((u32)(src)<<15) & 0x00008000)
#define HDLC_INT_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))
/*	 Fields sdhc_int	 */
#define SDHC_INT_WIDTH                                                        1
#define SDHC_INT_SHIFT                                                       14
#define SDHC_INT_MASK                                                0x00004000
#define SDHC_INT_RD(src)                             (((src) & 0x00004000)>>14)
#define SDHC_INT_WR(src)                        (((u32)(src)<<14) & 0x00004000)
#define SDHC_INT_SET(dst,src) \
                      (((dst) & ~0x00004000) | (((u32)(src)<<14) & 0x00004000))
/*	 Fields ahbc_tdm_int	 */
#define AHBC_TDM_INT_WIDTH                                                    1
#define AHBC_TDM_INT_SHIFT                                                   13
#define AHBC_TDM_INT_MASK                                            0x00002000
#define AHBC_TDM_INT_RD(src)                         (((src) & 0x00002000)>>13)
#define AHBC_TDM_INT_WR(src)                    (((u32)(src)<<13) & 0x00002000)
#define AHBC_TDM_INT_SET(dst,src) \
                      (((dst) & ~0x00002000) | (((u32)(src)<<13) & 0x00002000))
/*	 Fields usbh1_ehci_int	 */
#define USBH1_EHCI_INT_WIDTH                                                  1
#define USBH1_EHCI_INT_SHIFT                                                 12
#define USBH1_EHCI_INT_MASK                                          0x00001000
#define USBH1_EHCI_INT_RD(src)                       (((src) & 0x00001000)>>12)
#define USBH1_EHCI_INT_WR(src)                  (((u32)(src)<<12) & 0x00001000)
#define USBH1_EHCI_INT_SET(dst,src) \
                      (((dst) & ~0x00001000) | (((u32)(src)<<12) & 0x00001000))
/*	 Fields ohci1_irq	 */
#define OHCI1_IRQ_WIDTH                                                       1
#define OHCI1_IRQ_SHIFT                                                      11
#define OHCI1_IRQ_MASK                                               0x00000800
#define OHCI1_IRQ_RD(src)                            (((src) & 0x00000800)>>11)
#define OHCI1_IRQ_WR(src)                       (((u32)(src)<<11) & 0x00000800)
#define OHCI1_IRQ_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*	 Fields usbh_ehci_int	 */
#define USBH_EHCI_INT_WIDTH                                                   1
#define USBH_EHCI_INT_SHIFT                                                  10
#define USBH_EHCI_INT_MASK                                           0x00000400
#define USBH_EHCI_INT_RD(src)                        (((src) & 0x00000400)>>10)
#define USBH_EHCI_INT_WR(src)                   (((u32)(src)<<10) & 0x00000400)
#define USBH_EHCI_INT_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields ohci_irq	 */
#define OHCI_IRQ_WIDTH                                                        1
#define OHCI_IRQ_SHIFT                                                        9
#define OHCI_IRQ_MASK                                                0x00000200
#define OHCI_IRQ_RD(src)                              (((src) & 0x00000200)>>9)
#define OHCI_IRQ_WR(src)                         (((u32)(src)<<9) & 0x00000200)
#define OHCI_IRQ_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields ahbc_Flash_Int	 */
#define AHBC_FLASH_INT_WIDTH                                                  1
#define AHBC_FLASH_INT_SHIFT                                                  8
#define AHBC_FLASH_INT_MASK                                          0x00000100
#define AHBC_FLASH_INT_RD(src)                        (((src) & 0x00000100)>>8)
#define AHBC_FLASH_INT_WR(src)                   (((u32)(src)<<8) & 0x00000100)
#define AHBC_FLASH_INT_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*	 Fields ahbc_ahb2axi_err_int	 */
#define AHBC_AHB2AXI_ERR_INT_WIDTH                                            1
#define AHBC_AHB2AXI_ERR_INT_SHIFT                                            7
#define AHBC_AHB2AXI_ERR_INT_MASK                                    0x00000080
#define AHBC_AHB2AXI_ERR_INT_RD(src)                  (((src) & 0x00000080)>>7)
#define AHBC_AHB2AXI_ERR_INT_WR(src)             (((u32)(src)<<7) & 0x00000080)
#define AHBC_AHB2AXI_ERR_INT_SET(dst,src) \
                       (((dst) & ~0x00000080) | (((u32)(src)<<7) & 0x00000080))
/*	 Fields ahbc_otg0_int	 */
#define AHBC_OTG0_INT_WIDTH                                                   1
#define AHBC_OTG0_INT_SHIFT                                                   6
#define AHBC_OTG0_INT_MASK                                           0x00000040
#define AHBC_OTG0_INT_RD(src)                         (((src) & 0x00000040)>>6)
#define AHBC_OTG0_INT_WR(src)                    (((u32)(src)<<6) & 0x00000040)
#define AHBC_OTG0_INT_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*	 Fields ahbc_spi_1_int	 */
#define AHBC_SPI_1_INT_WIDTH                                                  1
#define AHBC_SPI_1_INT_SHIFT                                                  5
#define AHBC_SPI_1_INT_MASK                                          0x00000020
#define AHBC_SPI_1_INT_RD(src)                        (((src) & 0x00000020)>>5)
#define AHBC_SPI_1_INT_WR(src)                   (((u32)(src)<<5) & 0x00000020)
#define AHBC_SPI_1_INT_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields ahbc_spi_0_int	 */
#define AHBC_SPI_0_INT_WIDTH                                                  1
#define AHBC_SPI_0_INT_SHIFT                                                  4
#define AHBC_SPI_0_INT_MASK                                          0x00000010
#define AHBC_SPI_0_INT_RD(src)                        (((src) & 0x00000010)>>4)
#define AHBC_SPI_0_INT_WR(src)                   (((u32)(src)<<4) & 0x00000010)
#define AHBC_SPI_0_INT_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields ahbc_uart_3_int	 */
#define AHBC_UART_3_INT_WIDTH                                                 1
#define AHBC_UART_3_INT_SHIFT                                                 3
#define AHBC_UART_3_INT_MASK                                         0x00000008
#define AHBC_UART_3_INT_RD(src)                       (((src) & 0x00000008)>>3)
#define AHBC_UART_3_INT_WR(src)                  (((u32)(src)<<3) & 0x00000008)
#define AHBC_UART_3_INT_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields ahbc_uart_2_int	 */
#define AHBC_UART_2_INT_WIDTH                                                 1
#define AHBC_UART_2_INT_SHIFT                                                 2
#define AHBC_UART_2_INT_MASK                                         0x00000004
#define AHBC_UART_2_INT_RD(src)                       (((src) & 0x00000004)>>2)
#define AHBC_UART_2_INT_WR(src)                  (((u32)(src)<<2) & 0x00000004)
#define AHBC_UART_2_INT_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields ahbc_uart_1_int	 */
#define AHBC_UART_1_INT_WIDTH                                                 1
#define AHBC_UART_1_INT_SHIFT                                                 1
#define AHBC_UART_1_INT_MASK                                         0x00000002
#define AHBC_UART_1_INT_RD(src)                       (((src) & 0x00000002)>>1)
#define AHBC_UART_1_INT_WR(src)                  (((u32)(src)<<1) & 0x00000002)
#define AHBC_UART_1_INT_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields ahbc_uart_0_int	 */
#define AHBC_UART_0_INT_WIDTH                                                 1
#define AHBC_UART_0_INT_SHIFT                                                 0
#define AHBC_UART_0_INT_MASK                                         0x00000001
#define AHBC_UART_0_INT_RD(src)                          (((src) & 0x00000001))
#define AHBC_UART_0_INT_WR(src)                     (((u32)(src)) & 0x00000001)
#define AHBC_UART_0_INT_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register CFG_AUTO_RD_NFLASH_PAGE	*/ 
/*	 Fields NFLASH_AUTO_READ_PAGE	 */
#define NFLASH_AUTO_READ_PAGE_WIDTH                                          20
#define NFLASH_AUTO_READ_PAGE_SHIFT                                           0
#define NFLASH_AUTO_READ_PAGE_MASK                                   0x000fffff
#define NFLASH_AUTO_READ_PAGE_RD(src)                    (((src) & 0x000fffff))
#define NFLASH_AUTO_READ_PAGE_WR(src)               (((u32)(src)) & 0x000fffff)
#define NFLASH_AUTO_READ_PAGE_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register CFG_AUTO_RD_NFLASH	*/ 
/*	 Fields BOOTSTRP_BYTE_CTRL	 */
#define BOOTSTRP_BYTE_CTRL_WIDTH                                              8
#define BOOTSTRP_BYTE_CTRL_SHIFT                                             24
#define BOOTSTRP_BYTE_CTRL_MASK                                      0xff000000
#define BOOTSTRP_BYTE_CTRL_RD(src)                   (((src) & 0xff000000)>>24)
#define BOOTSTRP_BYTE_CTRL_WR(src)              (((u32)(src)<<24) & 0xff000000)
#define BOOTSTRP_BYTE_CTRL_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))
/*	 Fields SEL_EXT_AUTO_MODE	 */
#define AHBC_SEL_EXT_AUTO_MODE_WIDTH                                          1
#define AHBC_SEL_EXT_AUTO_MODE_SHIFT                                         22
#define AHBC_SEL_EXT_AUTO_MODE_MASK                                  0x00400000
#define AHBC_SEL_EXT_AUTO_MODE_RD(src)               (((src) & 0x00400000)>>22)
#define AHBC_SEL_EXT_AUTO_MODE_WR(src)          (((u32)(src)<<22) & 0x00400000)
#define AHBC_SEL_EXT_AUTO_MODE_SET(dst,src) \
                      (((dst) & ~0x00400000) | (((u32)(src)<<22) & 0x00400000))
/*	 Fields NFLASH_ECC_DET_COR	 */
#define NFLASH_ECC_DET_COR_WIDTH                                              1
#define NFLASH_ECC_DET_COR_SHIFT                                             21
#define NFLASH_ECC_DET_COR_MASK                                      0x00200000
#define NFLASH_ECC_DET_COR_RD(src)                   (((src) & 0x00200000)>>21)
#define NFLASH_ECC_DET_COR_WR(src)              (((u32)(src)<<21) & 0x00200000)
#define NFLASH_ECC_DET_COR_SET(dst,src) \
                      (((dst) & ~0x00200000) | (((u32)(src)<<21) & 0x00200000))
/*	 Fields NFLASH_AUTOREAD_EN	 */
#define NFLASH_AUTOREAD_EN_WIDTH                                              1
#define NFLASH_AUTOREAD_EN_SHIFT                                             20
#define NFLASH_AUTOREAD_EN_MASK                                      0x00100000
#define NFLASH_AUTOREAD_EN_RD(src)                   (((src) & 0x00100000)>>20)
#define NFLASH_AUTOREAD_EN_WR(src)              (((u32)(src)<<20) & 0x00100000)
#define NFLASH_AUTOREAD_EN_SET(dst,src) \
                      (((dst) & ~0x00100000) | (((u32)(src)<<20) & 0x00100000))
/*	 Fields NFLASH_BOOT_ADDR_MSB	 */
#define NFLASH_BOOT_ADDR_MSB_WIDTH                                           20
#define NFLASH_BOOT_ADDR_MSB_SHIFT                                            0
#define NFLASH_BOOT_ADDR_MSB_MASK                                    0x000fffff
#define NFLASH_BOOT_ADDR_MSB_RD(src)                     (((src) & 0x000fffff))
#define NFLASH_BOOT_ADDR_MSB_WR(src)                (((u32)(src)) & 0x000fffff)
#define NFLASH_BOOT_ADDR_MSB_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register CFG_AUTO_RD_NAND_DEV	*/ 
/*	 Fields NFLASH_ADDR_CYCLE	 */
#define NFLASH_ADDR_CYCLE_WIDTH                                               2
#define NFLASH_ADDR_CYCLE_SHIFT                                              28
#define NFLASH_ADDR_CYCLE_MASK                                       0x30000000
#define NFLASH_ADDR_CYCLE_RD(src)                    (((src) & 0x30000000)>>28)
#define NFLASH_ADDR_CYCLE_WR(src)               (((u32)(src)<<28) & 0x30000000)
#define NFLASH_ADDR_CYCLE_SET(dst,src) \
                      (((dst) & ~0x30000000) | (((u32)(src)<<28) & 0x30000000))
/*	 Fields NFLASH_DEV_WIDTH	 */
#define NFLASH_DEV_WIDTH_WIDTH                                                1
#define NFLASH_DEV_WIDTH_SHIFT                                               27
#define NFLASH_DEV_WIDTH_MASK                                        0x08000000
#define NFLASH_DEV_WIDTH_RD(src)                     (((src) & 0x08000000)>>27)
#define NFLASH_DEV_WIDTH_WR(src)                (((u32)(src)<<27) & 0x08000000)
#define NFLASH_DEV_WIDTH_SET(dst,src) \
                      (((dst) & ~0x08000000) | (((u32)(src)<<27) & 0x08000000))
/*	 Fields NFLASH_ECC_ON_OFF	 */
#define NFLASH_ECC_ON_OFF_WIDTH                                               1
#define NFLASH_ECC_ON_OFF_SHIFT                                              26
#define NFLASH_ECC_ON_OFF_MASK                                       0x04000000
#define NFLASH_ECC_ON_OFF_RD(src)                    (((src) & 0x04000000)>>26)
#define NFLASH_ECC_ON_OFF_WR(src)               (((u32)(src)<<26) & 0x04000000)
#define NFLASH_ECC_ON_OFF_SET(dst,src) \
                      (((dst) & ~0x04000000) | (((u32)(src)<<26) & 0x04000000))
/*	 Fields NFLASH_ECC_SPARE_OFF	 */
#define NFLASH_ECC_SPARE_OFF_WIDTH                                            8
#define NFLASH_ECC_SPARE_OFF_SHIFT                                           18
#define NFLASH_ECC_SPARE_OFF_MASK                                    0x03fc0000
#define NFLASH_ECC_SPARE_OFF_RD(src)                 (((src) & 0x03fc0000)>>18)
#define NFLASH_ECC_SPARE_OFF_WR(src)            (((u32)(src)<<18) & 0x03fc0000)
#define NFLASH_ECC_SPARE_OFF_SET(dst,src) \
                      (((dst) & ~0x03fc0000) | (((u32)(src)<<18) & 0x03fc0000))
/*	 Fields NFLASH_RDYBSY_EN	 */
#define NFLASH_RDYBSY_EN_WIDTH                                                1
#define NFLASH_RDYBSY_EN_SHIFT                                               17
#define NFLASH_RDYBSY_EN_MASK                                        0x00020000
#define NFLASH_RDYBSY_EN_RD(src)                     (((src) & 0x00020000)>>17)
#define NFLASH_RDYBSY_EN_WR(src)                (((u32)(src)<<17) & 0x00020000)
#define NFLASH_RDYBSY_EN_SET(dst,src) \
                      (((dst) & ~0x00020000) | (((u32)(src)<<17) & 0x00020000))
/*	 Fields NFLASH_PAGE_SIZE_4K	 */
#define NFLASH_PAGE_SIZE_4K_WIDTH                                             1
#define NFLASH_PAGE_SIZE_4K_SHIFT                                            16
#define NFLASH_PAGE_SIZE_4K_MASK                                     0x00010000
#define NFLASH_PAGE_SIZE_4K_RD(src)                  (((src) & 0x00010000)>>16)
#define NFLASH_PAGE_SIZE_4K_WR(src)             (((u32)(src)<<16) & 0x00010000)
#define NFLASH_PAGE_SIZE_4K_SET(dst,src) \
                      (((dst) & ~0x00010000) | (((u32)(src)<<16) & 0x00010000))
/*	 Fields NFLASH_DEV_READ_CNT	 */
#define NFLASH_DEV_READ_CNT_WIDTH                                            16
#define NFLASH_DEV_READ_CNT_SHIFT                                             0
#define NFLASH_DEV_READ_CNT_MASK                                     0x0000ffff
#define NFLASH_DEV_READ_CNT_RD(src)                      (((src) & 0x0000ffff))
#define NFLASH_DEV_READ_CNT_WR(src)                 (((u32)(src)) & 0x0000ffff)
#define NFLASH_DEV_READ_CNT_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register CFG_USB_HOST_PHY_CONNECT	*/ 
/*	 Fields usb_vbusvldext	 */
#define USB_VBUSVLDEXT_WIDTH                                                  1
#define USB_VBUSVLDEXT_SHIFT                                                 16
#define USB_VBUSVLDEXT_MASK                                          0x00010000
#define USB_VBUSVLDEXT_RD(src)                       (((src) & 0x00010000)>>16)
#define USB_VBUSVLDEXT_WR(src)                  (((u32)(src)<<16) & 0x00010000)
#define USB_VBUSVLDEXT_SET(dst,src) \
                      (((dst) & ~0x00010000) | (((u32)(src)<<16) & 0x00010000))
/*	 Fields usb_vbusvldextsel	 */
#define USB_VBUSVLDEXTSEL_WIDTH                                               1
#define USB_VBUSVLDEXTSEL_SHIFT                                              15
#define USB_VBUSVLDEXTSEL_MASK                                       0x00008000
#define USB_VBUSVLDEXTSEL_RD(src)                    (((src) & 0x00008000)>>15)
#define USB_VBUSVLDEXTSEL_WR(src)               (((u32)(src)<<15) & 0x00008000)
#define USB_VBUSVLDEXTSEL_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))
/*	 Fields usb_dppulldown	 */
#define USB_DPPULLDOWN_WIDTH                                                  1
#define USB_DPPULLDOWN_SHIFT                                                 14
#define USB_DPPULLDOWN_MASK                                          0x00004000
#define USB_DPPULLDOWN_RD(src)                       (((src) & 0x00004000)>>14)
#define USB_DPPULLDOWN_WR(src)                  (((u32)(src)<<14) & 0x00004000)
#define USB_DPPULLDOWN_SET(dst,src) \
                      (((dst) & ~0x00004000) | (((u32)(src)<<14) & 0x00004000))
/*	 Fields usb_dmpulldown	 */
#define USB_DMPULLDOWN_WIDTH                                                  1
#define USB_DMPULLDOWN_SHIFT                                                 13
#define USB_DMPULLDOWN_MASK                                          0x00002000
#define USB_DMPULLDOWN_RD(src)                       (((src) & 0x00002000)>>13)
#define USB_DMPULLDOWN_WR(src)                  (((u32)(src)<<13) & 0x00002000)
#define USB_DMPULLDOWN_SET(dst,src) \
                      (((dst) & ~0x00002000) | (((u32)(src)<<13) & 0x00002000))
/*	 Fields usb_txbitstuffen	 */
#define USB_TXBITSTUFFEN_WIDTH                                                1
#define USB_TXBITSTUFFEN_SHIFT                                               12
#define USB_TXBITSTUFFEN_MASK                                        0x00001000
#define USB_TXBITSTUFFEN_RD(src)                     (((src) & 0x00001000)>>12)
#define USB_TXBITSTUFFEN_WR(src)                (((u32)(src)<<12) & 0x00001000)
#define USB_TXBITSTUFFEN_SET(dst,src) \
                      (((dst) & ~0x00001000) | (((u32)(src)<<12) & 0x00001000))
/*	 Fields usb_txbitstuffenh	 */
#define USB_TXBITSTUFFENH_WIDTH                                               1
#define USB_TXBITSTUFFENH_SHIFT                                              11
#define USB_TXBITSTUFFENH_MASK                                       0x00000800
#define USB_TXBITSTUFFENH_RD(src)                    (((src) & 0x00000800)>>11)
#define USB_TXBITSTUFFENH_WR(src)               (((u32)(src)<<11) & 0x00000800)
#define USB_TXBITSTUFFENH_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*	 Fields usb_idpullup	 */
#define USB_IDPULLUP_WIDTH                                                    1
#define USB_IDPULLUP_SHIFT                                                   10
#define USB_IDPULLUP_MASK                                            0x00000400
#define USB_IDPULLUP_RD(src)                         (((src) & 0x00000400)>>10)
#define USB_IDPULLUP_WR(src)                    (((u32)(src)<<10) & 0x00000400)
#define USB_IDPULLUP_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields usb_chrgvbus	 */
#define USB_CHRGVBUS_WIDTH                                                    1
#define USB_CHRGVBUS_SHIFT                                                    9
#define USB_CHRGVBUS_MASK                                            0x00000200
#define USB_CHRGVBUS_RD(src)                          (((src) & 0x00000200)>>9)
#define USB_CHRGVBUS_WR(src)                     (((u32)(src)<<9) & 0x00000200)
#define USB_CHRGVBUS_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields usb_dischrgvbus	 */
#define USB_DISCHRGVBUS_WIDTH                                                 1
#define USB_DISCHRGVBUS_SHIFT                                                 8
#define USB_DISCHRGVBUS_MASK                                         0x00000100
#define USB_DISCHRGVBUS_RD(src)                       (((src) & 0x00000100)>>8)
#define USB_DISCHRGVBUS_WR(src)                  (((u32)(src)<<8) & 0x00000100)
#define USB_DISCHRGVBUS_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*	 Fields usb_drvvbus	 */
#define USB_DRVVBUS_WIDTH                                                     1
#define USB_DRVVBUS_SHIFT                                                     7
#define USB_DRVVBUS_MASK                                             0x00000080
#define USB_DRVVBUS_RD(src)                           (((src) & 0x00000080)>>7)
#define USB_DRVVBUS_WR(src)                      (((u32)(src)<<7) & 0x00000080)
#define USB_DRVVBUS_SET(dst,src) \
                       (((dst) & ~0x00000080) | (((u32)(src)<<7) & 0x00000080))
/*	 Fields usb_sleepm	 */
#define USB_SLEEPM_WIDTH                                                      1
#define USB_SLEEPM_SHIFT                                                      6
#define USB_SLEEPM_MASK                                              0x00000040
#define USB_SLEEPM_RD(src)                            (((src) & 0x00000040)>>6)
#define USB_SLEEPM_WR(src)                       (((u32)(src)<<6) & 0x00000040)
#define USB_SLEEPM_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*	 Fields usb_biastune	 */
#define USB_BIASTUNE_WIDTH                                                    1
#define USB_BIASTUNE_SHIFT                                                    5
#define USB_BIASTUNE_MASK                                            0x00000020
#define USB_BIASTUNE_RD(src)                          (((src) & 0x00000020)>>5)
#define USB_BIASTUNE_WR(src)                     (((u32)(src)<<5) & 0x00000020)
#define USB_BIASTUNE_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields usb_plltune	 */
#define USB_PLLTUNE_WIDTH                                                     1
#define USB_PLLTUNE_SHIFT                                                     4
#define USB_PLLTUNE_MASK                                             0x00000010
#define USB_PLLTUNE_RD(src)                           (((src) & 0x00000010)>>4)
#define USB_PLLTUNE_WR(src)                      (((u32)(src)<<4) & 0x00000010)
#define USB_PLLTUNE_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields usb_clkcore	 */
#define USB_CLKCORE_WIDTH                                                     1
#define USB_CLKCORE_SHIFT                                                     3
#define USB_CLKCORE_MASK                                             0x00000008
#define USB_CLKCORE_RD(src)                           (((src) & 0x00000008)>>3)
#define USB_CLKCORE_WR(src)                      (((u32)(src)<<3) & 0x00000008)
#define USB_CLKCORE_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields usb_commononn	 */
#define USB_COMMONONN_WIDTH                                                   1
#define USB_COMMONONN_SHIFT                                                   2
#define USB_COMMONONN_MASK                                           0x00000004
#define USB_COMMONONN_RD(src)                         (((src) & 0x00000004)>>2)
#define USB_COMMONONN_WR(src)                    (((u32)(src)<<2) & 0x00000004)
#define USB_COMMONONN_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields usb_loopbackenb	 */
#define USB_LOOPBACKENB_WIDTH                                                 1
#define USB_LOOPBACKENB_SHIFT                                                 1
#define USB_LOOPBACKENB_MASK                                         0x00000002
#define USB_LOOPBACKENB_RD(src)                       (((src) & 0x00000002)>>1)
#define USB_LOOPBACKENB_WR(src)                  (((u32)(src)<<1) & 0x00000002)
#define USB_LOOPBACKENB_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields usb_vregtune	 */
#define USB_VREGTUNE_WIDTH                                                    1
#define USB_VREGTUNE_SHIFT                                                    0
#define USB_VREGTUNE_MASK                                            0x00000001
#define USB_VREGTUNE_RD(src)                             (((src) & 0x00000001))
#define USB_VREGTUNE_WR(src)                        (((u32)(src)) & 0x00000001)
#define USB_VREGTUNE_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register CFG_AHBC_CLK_DET	*/ 
/*	 Fields TDM_CLK_DET	 */
#define TDM_CLK_DET_WIDTH                                                     1
#define TDM_CLK_DET_SHIFT                                                     1
#define TDM_CLK_DET_MASK                                             0x00000002
#define TDM_CLK_DET_RD(src)                           (((src) & 0x00000002)>>1)
#define TDM_CLK_DET_WR(src)                      (((u32)(src)<<1) & 0x00000002)
#define TDM_CLK_DET_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields USBOTG_CLK_DET	 */
#define USBOTG_CLK_DET_WIDTH                                                  1
#define USBOTG_CLK_DET_SHIFT                                                  0
#define USBOTG_CLK_DET_MASK                                          0x00000001
#define USBOTG_CLK_DET_RD(src)                           (((src) & 0x00000001))
#define USBOTG_CLK_DET_WR(src)                      (((u32)(src)) & 0x00000001)
#define USBOTG_CLK_DET_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register CFG_PPC_TRC_MUX	*/ 
/*	 Fields SEL	 */
#define AHBC_SEL_WIDTH                                                        1
#define AHBC_SEL_SHIFT                                                        0
#define AHBC_SEL_MASK                                                0x00000001
#define AHBC_SEL_RD(src)                                 (((src) & 0x00000001))
#define AHBC_SEL_WR(src)                            (((u32)(src)) & 0x00000001)
#define AHBC_SEL_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register CFG_PIN_MUX_0	*/ 
/*	 Fields SEL	 */
#define AHBC_SEL0_WIDTH                                                      32
#define AHBC_SEL0_SHIFT                                                       0
#define AHBC_SEL0_MASK                                               0xffffffff
#define AHBC_SEL0_RD(src)                                (((src) & 0xffffffff))
#define AHBC_SEL0_WR(src)                           (((u32)(src)) & 0xffffffff)
#define AHBC_SEL0_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_PIN_MUX_1	*/ 
/*	 Fields SEL	 */
#define AHBC_SEL1_WIDTH                                                      32
#define AHBC_SEL1_SHIFT                                                       0
#define AHBC_SEL1_MASK                                               0xffffffff
#define AHBC_SEL1_RD(src)                                (((src) & 0xffffffff))
#define AHBC_SEL1_WR(src)                           (((u32)(src)) & 0xffffffff)
#define AHBC_SEL1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_PIN_MUX_2	*/ 
/*	 Fields SEL	 */
#define AHBC_SEL2_WIDTH                                                      32
#define AHBC_SEL2_SHIFT                                                       0
#define AHBC_SEL2_MASK                                               0xffffffff
#define AHBC_SEL2_RD(src)                                (((src) & 0xffffffff))
#define AHBC_SEL2_WR(src)                           (((u32)(src)) & 0xffffffff)
#define AHBC_SEL2_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_PIN_MUX_3	*/ 
/*	 Fields SEL	 */
#define AHBC_SEL3_WIDTH                                                      32
#define AHBC_SEL3_SHIFT                                                       0
#define AHBC_SEL3_MASK                                               0xffffffff
#define AHBC_SEL3_RD(src)                                (((src) & 0xffffffff))
#define AHBC_SEL3_WR(src)                           (((u32)(src)) & 0xffffffff)
#define AHBC_SEL3_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_PIN_MUX_4	*/ 
/*	 Fields SEL	 */
#define AHBC_SEL4_WIDTH                                                      32
#define AHBC_SEL4_SHIFT                                                       0
#define AHBC_SEL4_MASK                                               0xffffffff
#define AHBC_SEL4_RD(src)                                (((src) & 0xffffffff))
#define AHBC_SEL4_WR(src)                           (((u32)(src)) & 0xffffffff)
#define AHBC_SEL4_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_PIN_MUX_5	*/ 
/*	 Fields SEL	 */
#define AHBC_SEL5_WIDTH                                                      32
#define AHBC_SEL5_SHIFT                                                       0
#define AHBC_SEL5_MASK                                               0xffffffff
#define AHBC_SEL5_RD(src)                                (((src) & 0xffffffff))
#define AHBC_SEL5_WR(src)                           (((u32)(src)) & 0xffffffff)
#define AHBC_SEL5_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_PIN_MUX_6	*/ 
/*	 Fields SEL	 */
#define AHBC_SEL6_WIDTH                                                      32
#define AHBC_SEL6_SHIFT                                                       0
#define AHBC_SEL6_MASK                                               0xffffffff
#define AHBC_SEL6_RD(src)                                (((src) & 0xffffffff))
#define AHBC_SEL6_WR(src)                           (((u32)(src)) & 0xffffffff)
#define AHBC_SEL6_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_PIN_MUX_7	*/ 
/*	 Fields SEL	 */
#define AHBC_SEL7_WIDTH                                                      32
#define AHBC_SEL7_SHIFT                                                       0
#define AHBC_SEL7_MASK                                               0xffffffff
#define AHBC_SEL7_RD(src)                                (((src) & 0xffffffff))
#define AHBC_SEL7_WR(src)                           (((u32)(src)) & 0xffffffff)
#define AHBC_SEL7_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

#endif /* defined(CONFIG_APM867xx) */

/*****************************************************************************
 * APM862xx AHBC CSR Register Definition 
 *****************************************************************************
 */
#if defined(CONFIG_APM862xx)
 
/*	Global Base Address	*/
#define AHBC_TOP_REG_BASE_ADDR			0xddd862000ULL

/*    Address AHBC_TOP_REG  Registers */
#define GPIO_CFG_CTL_ADDR                                            0x00000000
#define GPIO_CFG_CTL_DEFAULT                                         0x20000000
#define AHBC_CFG_CTL_ADDR                                            0x00000004
#define AHBC_CFG_CTL_DEFAULT                                         0x00000000
#define SDR_USB_OTG_AHB2AXI_CFG_ADDR                                 0x00000008
#define SDR_USB_OTG_AHB2AXI_CFG_DEFAULT                              0x000081ff
#define SDR_OCHI_0_AHB2AXI_CFG_ADDR                                  0x0000000c
#define SDR_OCHI_0_AHB2AXI_CFG_DEFAULT                               0x000081ff
#define SDR_SDIO_AHB2AXI_CFG_ADDR                                    0x00000010
#define SDR_SDIO_AHB2AXI_CFG_DEFAULT                                 0x000081ff
#define SPI_CONFIG_ADDR                                              0x00000014
#define SPI_CONFIG_DEFAULT                                           0x00000003
#define USB0_TUNE_ADDR                                               0x00000018
#define USB0_TUNE_DEFAULT                                            0x01d80462
#define USB0_RSTCTL_ADDR                                             0x0000001c
#define USB0_RSTCTL_DEFAULT                                          0x00000007
#define USB0_CLKCTL_ADDR                                             0x00000020
#define USB0_CLKCTL_DEFAULT                                          0x00000006
#define USB1_TUNE_ADDR                                               0x00000024
#define USB1_TUNE_DEFAULT                                            0x01d80462
#define USB1_RSTCTL_ADDR                                             0x00000028
#define USB1_RSTCTL_DEFAULT                                          0x00000007
#define USB1_CLKCTL_ADDR                                             0x0000002c
#define USB1_CLKCTL_DEFAULT                                          0x0000000a
#define SDHC_TAP_DELAY_ADDR                                          0x00000030
#define SDHC_TAP_DELAY_DEFAULT                                       0x00000000
#define SDHC_CAP_REG_LOW_S1_ADDR                                     0x00000034
#define SDHC_CAP_REG_LOW_S1_DEFAULT                                  0x00000000
#define SDHC_CAP_REG_HIGH_S1_ADDR                                    0x00000038
#define SDHC_CAP_REG_HIGH_S1_DEFAULT                                 0x00000000
#define SDHC_CAP_REG_LOW_S2_ADDR                                     0x0000003c
#define SDHC_CAP_REG_LOW_S2_DEFAULT                                  0x00000000
#define SDHC_CAP_REG_HIGH_S2_ADDR                                    0x00000040
#define SDHC_CAP_REG_HIGH_S2_DEFAULT                                 0x00000000
#define SDHC_INIT_PRESET_ADDR                                        0x00000044
#define SDHC_INIT_PRESET_DEFAULT                                     0x00000000
#define SDHC_DEFAULT_PRESET_ADDR                                     0x00000048
#define SDHC_DEFAULT_PRESET_DEFAULT                                  0x00000000
#define SDHC_HIGH_SPEED_PRESET_ADDR                                  0x0000004c
#define SDHC_HIGH_SPEED_PRESET_DEFAULT                               0x00000000
#define SDHC_SDR12_PRESET_ADDR                                       0x00000050
#define SDHC_SDR12_PRESET_DEFAULT                                    0x00000000
#define SDHC_SDR25_PRESET_ADDR                                       0x00000054
#define SDHC_SDR25_PRESET_DEFAULT                                    0x00000000
#define SDHC_SDR50_PRESET_ADDR                                       0x00000058
#define SDHC_SDR50_PRESET_DEFAULT                                    0x00000000
#define SDHC_SDR104_PRESET_ADDR                                      0x0000005c
#define SDHC_SDR104_PRESET_DEFAULT                                   0x00000000
#define SDHC_DDR50_PRESET_ADDR                                       0x00000060
#define SDHC_DDR50_PRESET_DEFAULT                                    0x00000000
#define USBHOST_CFG_ADDR                                             0x00000064
#define USBHOST_CFG_DEFAULT                                          0x00000000
#define USBHOST_STATUS_ADDR                                          0x00000068
#define USBHOST_STATUS_DEFAULT                                       0x00000000
#define USBHOST_FATAL_CFG_ADDR                                       0x0000006c
#define USBHOST_FATAL_CFG_DEFAULT                                    0x00000000
#define SDR_EHCI_0_AHB2AXI_CFG_ADDR                                  0x00000080
#define SDR_EHCI_0_AHB2AXI_CFG_DEFAULT                               0x000081ff
#define SDR_EHCI_1_AHB2AXI_CFG_ADDR                                  0x00000084
#define SDR_EHCI_1_AHB2AXI_CFG_DEFAULT                               0x000081ff
#define SDR_OHCI_1_AHB2AXI_CFG_ADDR                                  0x00000088
#define SDR_OHCI_1_AHB2AXI_CFG_DEFAULT                               0x000081ff
#define SDR_TDM_AHB2AXI_CFG_ADDR                                     0x0000008c
#define SDR_TDM_AHB2AXI_CFG_DEFAULT                                  0x000081ff
#define ENABLE_INTF_ADDR                                             0x00000090
#define ENABLE_INTF_DEFAULT                                          0x00800000
#define USBH_DESCRAM_ADDR                                            0x00000094
#define USBH_DESCRAM_DEFAULT                                         0x00000000
#define USBH_DATARAM_ADDR                                            0x00000098
#define USBH_DATARAM_DEFAULT                                         0x00000000
#define OTG_DATARAM_ADDR                                             0x0000009c
#define OTG_DATARAM_DEFAULT                                          0x00000000
#define SDHC_DATARAM_ADDR                                            0x000000a0
#define SDHC_DATARAM_DEFAULT                                         0x00000000
#define FLASH_TXDATARAM_ADDR                                         0x000000a4
#define FLASH_TXDATARAM_DEFAULT                                      0x00000000
#define FLASH_RXDATARAM_ADDR                                         0x000000a8
#define FLASH_RXDATARAM_DEFAULT                                      0x00000000
#define USB2_TUNE_ADDR                                               0x000000ac
#define USB2_TUNE_DEFAULT                                            0x01d80462
#define USB2_RSTCTL_ADDR                                             0x000000b0
#define USB2_RSTCTL_DEFAULT                                          0x00000007
#define USB2_CLKCTL_ADDR                                             0x000000b4
#define USB2_CLKCTL_DEFAULT                                          0x0000000a
#define USBHOST1_CFG_ADDR                                            0x000000b8
#define USBHOST1_CFG_DEFAULT                                         0x00000000
#define USBHOST1_STATUS_ADDR                                         0x000000bc
#define USBHOST1_STATUS_DEFAULT                                      0x00000000
#define USBHOST1_FATAL_CFG_ADDR                                      0x000000c0
#define USBHOST1_FATAL_CFG_DEFAULT                                   0x00000000
#define USBH1_DESCRAM_ADDR                                           0x000000c4
#define USBH1_DESCRAM_DEFAULT                                        0x00000000
#define USBH1_DATARAM_ADDR                                           0x000000c8
#define USBH1_DATARAM_DEFAULT                                        0x00000000
#define UART_MODE_SEL_ADDR                                           0x000000cc
#define UART_MODE_SEL_DEFAULT                                        0x00000000
#define AHB2AXI_BID_ADDR                                             0x000000d0
#define AHB2AXI_BID_DEFAULT                                          0x00000020
#define TDM_BID_ADDR                                                 0x000000d4
#define TDM_BID_DEFAULT                                              0x000000b5
#define AHBC_PARITY_ADDR                                             0x000000d8
#define AHBC_PARITY_DEFAULT                                          0x00000000
#define AHBC_INTERRUPT_ADDR                                          0x000000dc
#define AHBC_INTERRUPT_DEFAULT                                       0x00000000
#define AHBC_INTERRUPTMASK_ADDR                                      0x000000e0
#define AIM_0_AHB_ADDR                                               0x000000e4
#define AIM_0_AHB_DEFAULT                                            0x00000000
#define AIM_0_SIZE_CTL_ADDR                                          0x000000e8
#define AIM_0_SIZE_CTL_DEFAULT                                       0x00000000
#define AIM_0_AXI_LO_ADDR                                            0x000000ec
#define AIM_0_AXI_LO_DEFAULT                                         0x00000000
#define AIM_0_AXI_HI_ADDR                                            0x000000f0
#define AIM_0_AXI_HI_DEFAULT                                         0x00000000
#define AIM_1_AHB_ADDR                                               0x000000f4
#define AIM_1_AHB_DEFAULT                                            0x00000000
#define AIM_1_SIZE_CTL_ADDR                                          0x000000f8
#define AIM_1_SIZE_CTL_DEFAULT                                       0x00000000
#define AIM_1_AXI_LO_ADDR                                            0x000000fc
#define AIM_1_AXI_LO_DEFAULT                                         0x00000000
#define AIM_1_AXI_HI_ADDR                                            0x00000100
#define AIM_1_AXI_HI_DEFAULT                                         0x00000000
#define AIM_2_AHB_ADDR                                               0x00000104
#define AIM_2_AHB_DEFAULT                                            0x00000000
#define AIM_2_SIZE_CTL_ADDR                                          0x00000108
#define AIM_2_SIZE_CTL_DEFAULT                                       0x00000000
#define AIM_2_AXI_LO_ADDR                                            0x0000010c
#define AIM_2_AXI_LO_DEFAULT                                         0x00000000
#define AIM_2_AXI_HI_ADDR                                            0x00000110
#define AIM_2_AXI_HI_DEFAULT                                         0x00000000
#define AIM_3_AHB_ADDR                                               0x00000114
#define AIM_3_AHB_DEFAULT                                            0x00000000
#define AIM_3_SIZE_CTL_ADDR                                          0x00000118
#define AIM_3_SIZE_CTL_DEFAULT                                       0x00000000
#define AIM_3_AXI_LO_ADDR                                            0x0000011c
#define AIM_3_AXI_LO_DEFAULT                                         0x00000000
#define AIM_3_AXI_HI_ADDR                                            0x00000120
#define AIM_3_AXI_HI_DEFAULT                                         0x00000000
#define AIM_4_AHB_ADDR                                               0x00000124
#define AIM_4_AHB_DEFAULT                                            0x00000000
#define AIM_4_SIZE_CTL_ADDR                                          0x00000128
#define AIM_4_SIZE_CTL_DEFAULT                                       0x00000000
#define AIM_4_AXI_LO_ADDR                                            0x0000012c
#define AIM_4_AXI_LO_DEFAULT                                         0x00000000
#define AIM_4_AXI_HI_ADDR                                            0x00000130
#define AIM_4_AXI_HI_DEFAULT                                         0x00000000
#define AIM_5_AHB_ADDR                                               0x00000134
#define AIM_5_AHB_DEFAULT                                            0x00000000
#define AIM_5_SIZE_CTL_ADDR                                          0x00000138
#define AIM_5_SIZE_CTL_DEFAULT                                       0x00000000
#define AIM_5_AXI_LO_ADDR                                            0x0000013c
#define AIM_5_AXI_LO_DEFAULT                                         0x00000000
#define AIM_5_AXI_HI_ADDR                                            0x00000140
#define AIM_5_AXI_HI_DEFAULT                                         0x00000000
#define AIM_6_AHB_ADDR                                               0x00000144
#define AIM_6_AHB_DEFAULT                                            0x00000000
#define AIM_6_SIZE_CTL_ADDR                                          0x00000148
#define AIM_6_SIZE_CTL_DEFAULT                                       0x00000000
#define AIM_6_AXI_LO_ADDR                                            0x0000014c
#define AIM_6_AXI_LO_DEFAULT                                         0x00000000
#define AIM_6_AXI_HI_ADDR                                            0x00000150
#define AIM_6_AXI_HI_DEFAULT                                         0x00000000
#define AIM_7_AHB_ADDR                                               0x00000154
#define AIM_7_AHB_DEFAULT                                            0x00000000
#define AIM_7_SIZE_CTL_ADDR                                          0x00000158
#define AIM_7_SIZE_CTL_DEFAULT                                       0x00000000
#define AIM_7_AXI_LO_ADDR                                            0x0000015c
#define AIM_7_AXI_LO_DEFAULT                                         0x00000000
#define AIM_7_AXI_HI_ADDR                                            0x00000160
#define AIM_7_AXI_HI_DEFAULT                                         0x00000000
#define AIM_8_SIZE_CTL_ADDR                                          0x00000164
#define AIM_8_SIZE_CTL_DEFAULT                                       0x00000000
#define AIM_8_AXI_LO_ADDR                                            0x00000168
#define AIM_8_AXI_LO_DEFAULT                                         0x00000000
#define AIM_8_AXI_HI_ADDR                                            0x0000016c
#define AIM_8_AXI_HI_DEFAULT                                         0x00000000
#define AOM_0_AXI_LO_ADDR                                            0x00000170
#define AOM_0_AXI_LO_DEFAULT                                         0x00000000
#define AOM_0_AXI_HI_ADDR                                            0x00000174
#define AOM_0_AXI_HI_DEFAULT                                         0x00000000
#define AOM_0_SIZE_CTL_ADDR                                          0x00000178
#define AOM_0_SIZE_CTL_DEFAULT                                       0x00000000
#define AOM_0_AHB_ADDR                                               0x0000017c
#define AOM_0_AHB_DEFAULT                                            0x00000000
#define AOM_1_AXI_LO_ADDR                                            0x00000180
#define AOM_1_AXI_LO_DEFAULT                                         0x00000000
#define AOM_1_AXI_HI_ADDR                                            0x00000184
#define AOM_1_AXI_HI_DEFAULT                                         0x00000000
#define AOM_1_SIZE_CTL_ADDR                                          0x00000188
#define AOM_1_SIZE_CTL_DEFAULT                                       0x00000000
#define AOM_1_AHB_ADDR                                               0x0000018c
#define AOM_1_AHB_DEFAULT                                            0x00000000
#define AOM_2_AXI_LO_ADDR                                            0x00000190
#define AOM_2_AXI_LO_DEFAULT                                         0x00000000
#define AOM_2_AXI_HI_ADDR                                            0x00000194
#define AOM_2_AXI_HI_DEFAULT                                         0x00000000
#define AOM_2_SIZE_CTL_ADDR                                          0x00000198
#define AOM_2_SIZE_CTL_DEFAULT                                       0x00000000
#define AOM_2_AHB_ADDR                                               0x0000019c
#define AOM_2_AHB_DEFAULT                                            0x00000000
#define AOM_3_AXI_LO_ADDR                                            0x000001a0
#define AOM_3_AXI_LO_DEFAULT                                         0x00000000
#define AOM_3_AXI_HI_ADDR                                            0x000001a4
#define AOM_3_AXI_HI_DEFAULT                                         0x00000000
#define AOM_3_SIZE_CTL_ADDR                                          0x000001a8
#define AOM_3_SIZE_CTL_DEFAULT                                       0x00000000
#define AOM_3_AHB_ADDR                                               0x000001ac
#define AOM_3_AHB_DEFAULT                                            0x00000000
#define AHB_RD_ERR_ADDR_ADDR                                         0x000001b0
#define AHB_RD_ERR_ADDR_DEFAULT                                      0x00000000
#define AHB_RD_ERR_ATTRIBUTES_ADDR                                   0x000001b4
#define AHB_RD_ERR_ATTRIBUTES_DEFAULT                                0x00000000
#define AHB_WR_ERR_ADDR_ADDR                                         0x000001b8
#define AHB_WR_ERR_ADDR_DEFAULT                                      0x00000000
#define AHB_WR_ERR_DATA_ADDR                                         0x000001bc
#define AHB_WR_ERR_DATA_DEFAULT                                      0x00000000
#define AHB_WR_ERR_ATTRIBUTES_ADDR                                   0x000001c0
#define AHB_WR_ERR_ATTRIBUTES_DEFAULT                                0x00000000
#define CFG_INTR_CTL_ADDR                                            0x000001c4
#define CFG_INTR_CTL_DEFAULT                                         0x00000000
#define CFG_AUTO_RD_NFLASH_PAGE_ADDR                                 0x000001c8
#define CFG_AUTO_RD_NFLASH_PAGE_DEFAULT                              0x00000000
#define CFG_AUTO_RD_NFLASH_ADDR                                      0x000001cc
#define CFG_AUTO_RD_NFLASH_DEFAULT                                   0x001fffff
#define CFG_USB_HOST_PHY1_CONNECT_ADDR                               0x000001d0
#define CFG_USB_HOST_PHY1_CONNECT_DEFAULT                            0x0000f870
#define CFG_USB_HOST_PHY2_CONNECT_ADDR                               0x000001d4
#define CFG_USB_HOST_PHY2_CONNECT_DEFAULT                            0x0000f870
#define CFG_AUTO_RD_NAND_DEV_ADDR                                    0x000001d8
#define CFG_AUTO_RD_NAND_DEV_DEFAULT                                 0x0003ffff
#define CFG_AHBC_CLK_DET_ADDR                                        0x000001dc
#define CFG_AHBC_CLK_DET_DEFAULT                                     0x00000000

/*	Register GPIO_CFG_CTL	*/ 
/*	 Fields CFG_ENABLE_GPIO_INT	 */
#define CFG_ENABLE_GPIO_INT_WIDTH                                            26
#define CFG_ENABLE_GPIO_INT_SHIFT                                             0
#define CFG_ENABLE_GPIO_INT_MASK                                     0x03ffffff
#define CFG_ENABLE_GPIO_INT_RD(src)                      (((src) & 0x03ffffff))
#define CFG_ENABLE_GPIO_INT_WR(src)                 (((u32)(src)) & 0x03ffffff)
#define CFG_ENABLE_GPIO_INT_SET(dst,src) \
                          (((dst) & ~0x03ffffff) | (((u32)(src)) & 0x03ffffff))

/*	Register AHBC_CFG_CTL	*/ 
/*	 Fields USB_M_ENDIAN_SWAP	 */
#define USB_M_ENDIAN_SWAP_WIDTH                                               1
#define USB_M_ENDIAN_SWAP_SHIFT                                              17
#define USB_M_ENDIAN_SWAP_MASK                                       0x00020000
#define USB_M_ENDIAN_SWAP_RD(src)                    (((src) & 0x00020000)>>17)
#define USB_M_ENDIAN_SWAP_WR(src)               (((u32)(src)<<17) & 0x00020000)
#define USB_M_ENDIAN_SWAP_SET(dst,src) \
                      (((dst) & ~0x00020000) | (((u32)(src)<<17) & 0x00020000))
/*	 Fields USB_S_ENDIAN_SWAP	 */
#define USB_S_ENDIAN_SWAP_WIDTH                                               1
#define USB_S_ENDIAN_SWAP_SHIFT                                              16
#define USB_S_ENDIAN_SWAP_MASK                                       0x00010000
#define USB_S_ENDIAN_SWAP_RD(src)                    (((src) & 0x00010000)>>16)
#define USB_S_ENDIAN_SWAP_WR(src)               (((u32)(src)<<16) & 0x00010000)
#define USB_S_ENDIAN_SWAP_SET(dst,src) \
                      (((dst) & ~0x00010000) | (((u32)(src)<<16) & 0x00010000))
/*	 Fields USB_M_BIG_ENDIAN	 */
#define USB_M_BIG_ENDIAN_WIDTH                                                1
#define USB_M_BIG_ENDIAN_SHIFT                                               10
#define USB_M_BIG_ENDIAN_MASK                                        0x00000400
#define USB_M_BIG_ENDIAN_RD(src)                     (((src) & 0x00000400)>>10)
#define USB_M_BIG_ENDIAN_WR(src)                (((u32)(src)<<10) & 0x00000400)
#define USB_M_BIG_ENDIAN_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields USB_S_BIG_ENDIAN	 */
#define USB_S_BIG_ENDIAN_WIDTH                                                1
#define USB_S_BIG_ENDIAN_SHIFT                                                9
#define USB_S_BIG_ENDIAN_MASK                                        0x00000200
#define USB_S_BIG_ENDIAN_RD(src)                      (((src) & 0x00000200)>>9)
#define USB_S_BIG_ENDIAN_WR(src)                 (((u32)(src)<<9) & 0x00000200)
#define USB_S_BIG_ENDIAN_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields FLASH_ENDIAN_SWAP	 */
#define FLASH_ENDIAN_SWAP_WIDTH                                               1
#define FLASH_ENDIAN_SWAP_SHIFT                                               0
#define FLASH_ENDIAN_SWAP_MASK                                       0x00000001
#define FLASH_ENDIAN_SWAP_RD(src)                        (((src) & 0x00000001))
#define FLASH_ENDIAN_SWAP_WR(src)                   (((u32)(src)) & 0x00000001)
#define FLASH_ENDIAN_SWAP_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register SDR_USB_OTG_AHB2AXI_CFG	*/ 
/*	 Fields pw_axi_sideband_prio	 */
#define PW_AXI_SIDEBAND_PRIO_WIDTH                                            3
#define PW_AXI_SIDEBAND_PRIO_SHIFT                                           21
#define PW_AXI_SIDEBAND_PRIO_MASK                                    0x00e00000
#define PW_AXI_SIDEBAND_PRIO_RD(src)                 (((src) & 0x00e00000)>>21)
#define PW_AXI_SIDEBAND_PRIO_WR(src)            (((u32)(src)<<21) & 0x00e00000)
#define PW_AXI_SIDEBAND_PRIO_SET(dst,src) \
                      (((dst) & ~0x00e00000) | (((u32)(src)<<21) & 0x00e00000))
/*	 Fields pr_axi_sideband_prio	 */
#define PR_AXI_SIDEBAND_PRIO_WIDTH                                            3
#define PR_AXI_SIDEBAND_PRIO_SHIFT                                           17
#define PR_AXI_SIDEBAND_PRIO_MASK                                    0x000e0000
#define PR_AXI_SIDEBAND_PRIO_RD(src)                 (((src) & 0x000e0000)>>17)
#define PR_AXI_SIDEBAND_PRIO_WR(src)            (((u32)(src)<<17) & 0x000e0000)
#define PR_AXI_SIDEBAND_PRIO_SET(dst,src) \
                      (((dst) & ~0x000e0000) | (((u32)(src)<<17) & 0x000e0000))
/*	 Fields writes_block_reads	 */
#define WRITES_BLOCK_READS_WIDTH                                              1
#define WRITES_BLOCK_READS_SHIFT                                             15
#define WRITES_BLOCK_READS_MASK                                      0x00008000
#define WRITES_BLOCK_READS_RD(src)                   (((src) & 0x00008000)>>15)
#define WRITES_BLOCK_READS_WR(src)              (((u32)(src)<<15) & 0x00008000)
#define WRITES_BLOCK_READS_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))
/*	 Fields pw_max_outstanding	 */
#define PW_MAX_OUTSTANDING_WIDTH                                              4
#define PW_MAX_OUTSTANDING_SHIFT                                              8
#define PW_MAX_OUTSTANDING_MASK                                      0x00000f00
#define PW_MAX_OUTSTANDING_RD(src)                    (((src) & 0x00000f00)>>8)
#define PW_MAX_OUTSTANDING_WR(src)               (((u32)(src)<<8) & 0x00000f00)
#define PW_MAX_OUTSTANDING_SET(dst,src) \
                       (((dst) & ~0x00000f00) | (((u32)(src)<<8) & 0x00000f00))
/*	 Fields tx_wrr_weight	 */
#define TX_WRR_WEIGHT_WIDTH                                                   4
#define TX_WRR_WEIGHT_SHIFT                                                   4
#define TX_WRR_WEIGHT_MASK                                           0x000000f0
#define TX_WRR_WEIGHT_RD(src)                         (((src) & 0x000000f0)>>4)
#define TX_WRR_WEIGHT_WR(src)                    (((u32)(src)<<4) & 0x000000f0)
#define TX_WRR_WEIGHT_SET(dst,src) \
                       (((dst) & ~0x000000f0) | (((u32)(src)<<4) & 0x000000f0))
/*	 Fields rx_wrr_weight	 */
#define RX_WRR_WEIGHT_WIDTH                                                   4
#define RX_WRR_WEIGHT_SHIFT                                                   0
#define RX_WRR_WEIGHT_MASK                                           0x0000000f
#define RX_WRR_WEIGHT_RD(src)                            (((src) & 0x0000000f))
#define RX_WRR_WEIGHT_WR(src)                       (((u32)(src)) & 0x0000000f)
#define RX_WRR_WEIGHT_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register SDR_OCHI_0_AHB2AXI_CFG	*/ 
/*	 Fields pw_axi_sideband_prio	 */
#define PW_AXI_SIDEBAND_PRIO_F1_WIDTH                                         3
#define PW_AXI_SIDEBAND_PRIO_F1_SHIFT                                        21
#define PW_AXI_SIDEBAND_PRIO_F1_MASK                                 0x00e00000
#define PW_AXI_SIDEBAND_PRIO_F1_RD(src)              (((src) & 0x00e00000)>>21)
#define PW_AXI_SIDEBAND_PRIO_F1_WR(src)         (((u32)(src)<<21) & 0x00e00000)
#define PW_AXI_SIDEBAND_PRIO_F1_SET(dst,src) \
                      (((dst) & ~0x00e00000) | (((u32)(src)<<21) & 0x00e00000))
/*	 Fields pr_axi_sideband_prio	 */
#define PR_AXI_SIDEBAND_PRIO_F1_WIDTH                                         3
#define PR_AXI_SIDEBAND_PRIO_F1_SHIFT                                        17
#define PR_AXI_SIDEBAND_PRIO_F1_MASK                                 0x000e0000
#define PR_AXI_SIDEBAND_PRIO_F1_RD(src)              (((src) & 0x000e0000)>>17)
#define PR_AXI_SIDEBAND_PRIO_F1_WR(src)         (((u32)(src)<<17) & 0x000e0000)
#define PR_AXI_SIDEBAND_PRIO_F1_SET(dst,src) \
                      (((dst) & ~0x000e0000) | (((u32)(src)<<17) & 0x000e0000))
/*	 Fields writes_block_reads	 */
#define WRITES_BLOCK_READS_F1_WIDTH                                           1
#define WRITES_BLOCK_READS_F1_SHIFT                                          15
#define WRITES_BLOCK_READS_F1_MASK                                   0x00008000
#define WRITES_BLOCK_READS_F1_RD(src)                (((src) & 0x00008000)>>15)
#define WRITES_BLOCK_READS_F1_WR(src)           (((u32)(src)<<15) & 0x00008000)
#define WRITES_BLOCK_READS_F1_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))
/*	 Fields pw_max_outstanding	 */
#define PW_MAX_OUTSTANDING_F1_WIDTH                                           4
#define PW_MAX_OUTSTANDING_F1_SHIFT                                           8
#define PW_MAX_OUTSTANDING_F1_MASK                                   0x00000f00
#define PW_MAX_OUTSTANDING_F1_RD(src)                 (((src) & 0x00000f00)>>8)
#define PW_MAX_OUTSTANDING_F1_WR(src)            (((u32)(src)<<8) & 0x00000f00)
#define PW_MAX_OUTSTANDING_F1_SET(dst,src) \
                       (((dst) & ~0x00000f00) | (((u32)(src)<<8) & 0x00000f00))
/*	 Fields tx_wrr_weight	 */
#define TX_WRR_WEIGHT_F1_WIDTH                                                4
#define TX_WRR_WEIGHT_F1_SHIFT                                                4
#define TX_WRR_WEIGHT_F1_MASK                                        0x000000f0
#define TX_WRR_WEIGHT_F1_RD(src)                      (((src) & 0x000000f0)>>4)
#define TX_WRR_WEIGHT_F1_WR(src)                 (((u32)(src)<<4) & 0x000000f0)
#define TX_WRR_WEIGHT_F1_SET(dst,src) \
                       (((dst) & ~0x000000f0) | (((u32)(src)<<4) & 0x000000f0))
/*	 Fields rx_wrr_weight	 */
#define RX_WRR_WEIGHT_F1_WIDTH                                                4
#define RX_WRR_WEIGHT_F1_SHIFT                                                0
#define RX_WRR_WEIGHT_F1_MASK                                        0x0000000f
#define RX_WRR_WEIGHT_F1_RD(src)                         (((src) & 0x0000000f))
#define RX_WRR_WEIGHT_F1_WR(src)                    (((u32)(src)) & 0x0000000f)
#define RX_WRR_WEIGHT_F1_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register SDR_SDIO_AHB2AXI_CFG	*/ 
/*	 Fields pw_axi_sideband_prio	 */
#define PW_AXI_SIDEBAND_PRIO_F2_WIDTH                                         3
#define PW_AXI_SIDEBAND_PRIO_F2_SHIFT                                        21
#define PW_AXI_SIDEBAND_PRIO_F2_MASK                                 0x00e00000
#define PW_AXI_SIDEBAND_PRIO_F2_RD(src)              (((src) & 0x00e00000)>>21)
#define PW_AXI_SIDEBAND_PRIO_F2_WR(src)         (((u32)(src)<<21) & 0x00e00000)
#define PW_AXI_SIDEBAND_PRIO_F2_SET(dst,src) \
                      (((dst) & ~0x00e00000) | (((u32)(src)<<21) & 0x00e00000))
/*	 Fields pr_axi_sideband_prio	 */
#define PR_AXI_SIDEBAND_PRIO_F2_WIDTH                                         3
#define PR_AXI_SIDEBAND_PRIO_F2_SHIFT                                        17
#define PR_AXI_SIDEBAND_PRIO_F2_MASK                                 0x000e0000
#define PR_AXI_SIDEBAND_PRIO_F2_RD(src)              (((src) & 0x000e0000)>>17)
#define PR_AXI_SIDEBAND_PRIO_F2_WR(src)         (((u32)(src)<<17) & 0x000e0000)
#define PR_AXI_SIDEBAND_PRIO_F2_SET(dst,src) \
                      (((dst) & ~0x000e0000) | (((u32)(src)<<17) & 0x000e0000))
/*	 Fields writes_block_reads	 */
#define WRITES_BLOCK_READS_F2_WIDTH                                           1
#define WRITES_BLOCK_READS_F2_SHIFT                                          15
#define WRITES_BLOCK_READS_F2_MASK                                   0x00008000
#define WRITES_BLOCK_READS_F2_RD(src)                (((src) & 0x00008000)>>15)
#define WRITES_BLOCK_READS_F2_WR(src)           (((u32)(src)<<15) & 0x00008000)
#define WRITES_BLOCK_READS_F2_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))
/*	 Fields pw_max_outstanding	 */
#define PW_MAX_OUTSTANDING_F2_WIDTH                                           4
#define PW_MAX_OUTSTANDING_F2_SHIFT                                           8
#define PW_MAX_OUTSTANDING_F2_MASK                                   0x00000f00
#define PW_MAX_OUTSTANDING_F2_RD(src)                 (((src) & 0x00000f00)>>8)
#define PW_MAX_OUTSTANDING_F2_WR(src)            (((u32)(src)<<8) & 0x00000f00)
#define PW_MAX_OUTSTANDING_F2_SET(dst,src) \
                       (((dst) & ~0x00000f00) | (((u32)(src)<<8) & 0x00000f00))
/*	 Fields tx_wrr_weight	 */
#define TX_WRR_WEIGHT_F2_WIDTH                                                4
#define TX_WRR_WEIGHT_F2_SHIFT                                                4
#define TX_WRR_WEIGHT_F2_MASK                                        0x000000f0
#define TX_WRR_WEIGHT_F2_RD(src)                      (((src) & 0x000000f0)>>4)
#define TX_WRR_WEIGHT_F2_WR(src)                 (((u32)(src)<<4) & 0x000000f0)
#define TX_WRR_WEIGHT_F2_SET(dst,src) \
                       (((dst) & ~0x000000f0) | (((u32)(src)<<4) & 0x000000f0))
/*	 Fields rx_wrr_weight	 */
#define RX_WRR_WEIGHT_F2_WIDTH                                                4
#define RX_WRR_WEIGHT_F2_SHIFT                                                0
#define RX_WRR_WEIGHT_F2_MASK                                        0x0000000f
#define RX_WRR_WEIGHT_F2_RD(src)                         (((src) & 0x0000000f))
#define RX_WRR_WEIGHT_F2_WR(src)                    (((u32)(src)) & 0x0000000f)
#define RX_WRR_WEIGHT_F2_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register SPI_CONFIG	*/ 
/*	 Fields spi_1_ss_in_n	 */
#define SPI_1_SS_IN_N_WIDTH                                                   1
#define SPI_1_SS_IN_N_SHIFT                                                   1
#define SPI_1_SS_IN_N_MASK                                           0x00000002
#define SPI_1_SS_IN_N_RD(src)                         (((src) & 0x00000002)>>1)
#define SPI_1_SS_IN_N_WR(src)                    (((u32)(src)<<1) & 0x00000002)
#define SPI_1_SS_IN_N_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields spi_0_ss_in_n	 */
#define SPI_0_SS_IN_N_WIDTH                                                   1
#define SPI_0_SS_IN_N_SHIFT                                                   0
#define SPI_0_SS_IN_N_MASK                                           0x00000001
#define SPI_0_SS_IN_N_RD(src)                            (((src) & 0x00000001))
#define SPI_0_SS_IN_N_WR(src)                       (((u32)(src)) & 0x00000001)
#define SPI_0_SS_IN_N_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register USB0_TUNE	*/ 
/*	 Fields USB0_TXVREFTUNE	 */
#define USB0_TXVREFTUNE_WIDTH                                                 4
#define USB0_TXVREFTUNE_SHIFT                                                21
#define USB0_TXVREFTUNE_MASK                                         0x01e00000
#define USB0_TXVREFTUNE_RD(src)                      (((src) & 0x01e00000)>>21)
#define USB0_TXVREFTUNE_WR(src)                 (((u32)(src)<<21) & 0x01e00000)
#define USB0_TXVREFTUNE_SET(dst,src) \
                      (((dst) & ~0x01e00000) | (((u32)(src)<<21) & 0x01e00000))
/*	 Fields USB0_TXFSLSTUNE	 */
#define USB0_TXFSLSTUNE_WIDTH                                                 4
#define USB0_TXFSLSTUNE_SHIFT                                                17
#define USB0_TXFSLSTUNE_MASK                                         0x001e0000
#define USB0_TXFSLSTUNE_RD(src)                      (((src) & 0x001e0000)>>17)
#define USB0_TXFSLSTUNE_WR(src)                 (((u32)(src)<<17) & 0x001e0000)
#define USB0_TXFSLSTUNE_SET(dst,src) \
                      (((dst) & ~0x001e0000) | (((u32)(src)<<17) & 0x001e0000))
/*	 Fields USB0_TXPREEMPHASISTUNE	 */
#define USB0_TXPREEMPHASISTUNE_WIDTH                                          2
#define USB0_TXPREEMPHASISTUNE_SHIFT                                         15
#define USB0_TXPREEMPHASISTUNE_MASK                                  0x00018000
#define USB0_TXPREEMPHASISTUNE_RD(src)               (((src) & 0x00018000)>>15)
#define USB0_TXPREEMPHASISTUNE_WR(src)          (((u32)(src)<<15) & 0x00018000)
#define USB0_TXPREEMPHASISTUNE_SET(dst,src) \
                      (((dst) & ~0x00018000) | (((u32)(src)<<15) & 0x00018000))
/*	 Fields USB0_TXRISETUNE	 */
#define USB0_TXRISETUNE_WIDTH                                                 1
#define USB0_TXRISETUNE_SHIFT                                                14
#define USB0_TXRISETUNE_MASK                                         0x00004000
#define USB0_TXRISETUNE_RD(src)                      (((src) & 0x00004000)>>14)
#define USB0_TXRISETUNE_WR(src)                 (((u32)(src)<<14) & 0x00004000)
#define USB0_TXRISETUNE_SET(dst,src) \
                      (((dst) & ~0x00004000) | (((u32)(src)<<14) & 0x00004000))
/*	 Fields USB0_TXHSXVTUNE	 */
#define USB0_TXHSXVTUNE_WIDTH                                                 2
#define USB0_TXHSXVTUNE_SHIFT                                                12
#define USB0_TXHSXVTUNE_MASK                                         0x00003000
#define USB0_TXHSXVTUNE_RD(src)                      (((src) & 0x00003000)>>12)
#define USB0_TXHSXVTUNE_WR(src)                 (((u32)(src)<<12) & 0x00003000)
#define USB0_TXHSXVTUNE_SET(dst,src) \
                      (((dst) & ~0x00003000) | (((u32)(src)<<12) & 0x00003000))
/*	 Fields USB0_COMPDISTUNE	 */
#define USB0_COMPDISTUNE_WIDTH                                                3
#define USB0_COMPDISTUNE_SHIFT                                                8
#define USB0_COMPDISTUNE_MASK                                        0x00000700
#define USB0_COMPDISTUNE_RD(src)                      (((src) & 0x00000700)>>8)
#define USB0_COMPDISTUNE_WR(src)                 (((u32)(src)<<8) & 0x00000700)
#define USB0_COMPDISTUNE_SET(dst,src) \
                       (((dst) & ~0x00000700) | (((u32)(src)<<8) & 0x00000700))
/*	 Fields USB0_SQRXTUNE	 */
#define USB0_SQRXTUNE_WIDTH                                                   3
#define USB0_SQRXTUNE_SHIFT                                                   4
#define USB0_SQRXTUNE_MASK                                           0x00000070
#define USB0_SQRXTUNE_RD(src)                         (((src) & 0x00000070)>>4)
#define USB0_SQRXTUNE_WR(src)                    (((u32)(src)<<4) & 0x00000070)
#define USB0_SQRXTUNE_SET(dst,src) \
                       (((dst) & ~0x00000070) | (((u32)(src)<<4) & 0x00000070))
/*	 Fields USB0_OTGTUNE	 */
#define USB0_OTGTUNE_WIDTH                                                    3
#define USB0_OTGTUNE_SHIFT                                                    0
#define USB0_OTGTUNE_MASK                                            0x00000007
#define USB0_OTGTUNE_RD(src)                             (((src) & 0x00000007))
#define USB0_OTGTUNE_WR(src)                        (((u32)(src)) & 0x00000007)
#define USB0_OTGTUNE_SET(dst,src) \
                          (((dst) & ~0x00000007) | (((u32)(src)) & 0x00000007))

/*	Register USB0_RSTCTL	*/ 
/*	 Fields USB0_OTGDISABLE	 */
#define USB0_OTGDISABLE_WIDTH                                                 1
#define USB0_OTGDISABLE_SHIFT                                                 2
#define USB0_OTGDISABLE_MASK                                         0x00000004
#define USB0_OTGDISABLE_RD(src)                       (((src) & 0x00000004)>>2)
#define USB0_OTGDISABLE_WR(src)                  (((u32)(src)<<2) & 0x00000004)
#define USB0_OTGDISABLE_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields USB0_PORTRESET	 */
#define USB0_PORTRESET_WIDTH                                                  1
#define USB0_PORTRESET_SHIFT                                                  1
#define USB0_PORTRESET_MASK                                          0x00000002
#define USB0_PORTRESET_RD(src)                        (((src) & 0x00000002)>>1)
#define USB0_PORTRESET_WR(src)                   (((u32)(src)<<1) & 0x00000002)
#define USB0_PORTRESET_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields USB0_POR	 */
#define USB0_POR_WIDTH                                                        1
#define USB0_POR_SHIFT                                                        0
#define USB0_POR_MASK                                                0x00000001
#define USB0_POR_RD(src)                                 (((src) & 0x00000001))
#define USB0_POR_WR(src)                            (((u32)(src)) & 0x00000001)
#define USB0_POR_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register USB0_CLKCTL	*/ 
/*	 Fields USB0_SIM_SCALEDOWN	 */
#define USB0_SIM_SCALEDOWN_WIDTH                                              2
#define USB0_SIM_SCALEDOWN_SHIFT                                             30
#define USB0_SIM_SCALEDOWN_MASK                                      0xc0000000
#define USB0_SIM_SCALEDOWN_RD(src)                   (((src) & 0xc0000000)>>30)
#define USB0_SIM_SCALEDOWN_WR(src)              (((u32)(src)<<30) & 0xc0000000)
#define USB0_SIM_SCALEDOWN_SET(dst,src) \
                      (((dst) & ~0xc0000000) | (((u32)(src)<<30) & 0xc0000000))
/*	 Fields USB0_DRVVBUS_POLARITY	 */
#define USB0_DRVVBUS_POLARITY_WIDTH                                           1
#define USB0_DRVVBUS_POLARITY_SHIFT                                           7
#define USB0_DRVVBUS_POLARITY_MASK                                   0x00000080
#define USB0_DRVVBUS_POLARITY_RD(src)                 (((src) & 0x00000080)>>7)
#define USB0_DRVVBUS_POLARITY_WR(src)            (((u32)(src)<<7) & 0x00000080)
#define USB0_DRVVBUS_POLARITY_SET(dst,src) \
                       (((dst) & ~0x00000080) | (((u32)(src)<<7) & 0x00000080))
/*	 Fields USB0_REFCLKSEL	 */
#define USB0_REFCLKSEL_WIDTH                                                  2
#define USB0_REFCLKSEL_SHIFT                                                  2
#define USB0_REFCLKSEL_MASK                                          0x0000000c
#define USB0_REFCLKSEL_RD(src)                        (((src) & 0x0000000c)>>2)
#define USB0_REFCLKSEL_WR(src)                   (((u32)(src)<<2) & 0x0000000c)
#define USB0_REFCLKSEL_SET(dst,src) \
                       (((dst) & ~0x0000000c) | (((u32)(src)<<2) & 0x0000000c))
/*	 Fields USB0_REFCLKDIV	 */
#define USB0_REFCLKDIV_WIDTH                                                  2
#define USB0_REFCLKDIV_SHIFT                                                  0
#define USB0_REFCLKDIV_MASK                                          0x00000003
#define USB0_REFCLKDIV_RD(src)                           (((src) & 0x00000003))
#define USB0_REFCLKDIV_WR(src)                      (((u32)(src)) & 0x00000003)
#define USB0_REFCLKDIV_SET(dst,src) \
                          (((dst) & ~0x00000003) | (((u32)(src)) & 0x00000003))

/*	Register USB1_TUNE	*/ 
/*	 Fields USB0_TXVREFTUNE	 */
#define USB0_TXVREFTUNE_F1_WIDTH                                              4
#define USB0_TXVREFTUNE_F1_SHIFT                                             21
#define USB0_TXVREFTUNE_F1_MASK                                      0x01e00000
#define USB0_TXVREFTUNE_F1_RD(src)                   (((src) & 0x01e00000)>>21)
#define USB0_TXVREFTUNE_F1_WR(src)              (((u32)(src)<<21) & 0x01e00000)
#define USB0_TXVREFTUNE_F1_SET(dst,src) \
                      (((dst) & ~0x01e00000) | (((u32)(src)<<21) & 0x01e00000))
/*	 Fields USB0_TXFSLSTUNE	 */
#define USB0_TXFSLSTUNE_F1_WIDTH                                              4
#define USB0_TXFSLSTUNE_F1_SHIFT                                             17
#define USB0_TXFSLSTUNE_F1_MASK                                      0x001e0000
#define USB0_TXFSLSTUNE_F1_RD(src)                   (((src) & 0x001e0000)>>17)
#define USB0_TXFSLSTUNE_F1_WR(src)              (((u32)(src)<<17) & 0x001e0000)
#define USB0_TXFSLSTUNE_F1_SET(dst,src) \
                      (((dst) & ~0x001e0000) | (((u32)(src)<<17) & 0x001e0000))
/*	 Fields USB0_TXPREEMPHASISTUNE	 */
#define USB0_TXPREEMPHASISTUNE_F1_WIDTH                                       2
#define USB0_TXPREEMPHASISTUNE_F1_SHIFT                                      15
#define USB0_TXPREEMPHASISTUNE_F1_MASK                               0x00018000
#define USB0_TXPREEMPHASISTUNE_F1_RD(src)            (((src) & 0x00018000)>>15)
#define USB0_TXPREEMPHASISTUNE_F1_WR(src)       (((u32)(src)<<15) & 0x00018000)
#define USB0_TXPREEMPHASISTUNE_F1_SET(dst,src) \
                      (((dst) & ~0x00018000) | (((u32)(src)<<15) & 0x00018000))
/*	 Fields USB0_TXRISETUNE	 */
#define USB0_TXRISETUNE_F1_WIDTH                                              1
#define USB0_TXRISETUNE_F1_SHIFT                                             14
#define USB0_TXRISETUNE_F1_MASK                                      0x00004000
#define USB0_TXRISETUNE_F1_RD(src)                   (((src) & 0x00004000)>>14)
#define USB0_TXRISETUNE_F1_WR(src)              (((u32)(src)<<14) & 0x00004000)
#define USB0_TXRISETUNE_F1_SET(dst,src) \
                      (((dst) & ~0x00004000) | (((u32)(src)<<14) & 0x00004000))
/*	 Fields USB0_TXHSXVTUNE	 */
#define USB0_TXHSXVTUNE_F1_WIDTH                                              2
#define USB0_TXHSXVTUNE_F1_SHIFT                                             12
#define USB0_TXHSXVTUNE_F1_MASK                                      0x00003000
#define USB0_TXHSXVTUNE_F1_RD(src)                   (((src) & 0x00003000)>>12)
#define USB0_TXHSXVTUNE_F1_WR(src)              (((u32)(src)<<12) & 0x00003000)
#define USB0_TXHSXVTUNE_F1_SET(dst,src) \
                      (((dst) & ~0x00003000) | (((u32)(src)<<12) & 0x00003000))
/*	 Fields USB0_COMPDISTUNE	 */
#define USB0_COMPDISTUNE_F1_WIDTH                                             3
#define USB0_COMPDISTUNE_F1_SHIFT                                             8
#define USB0_COMPDISTUNE_F1_MASK                                     0x00000700
#define USB0_COMPDISTUNE_F1_RD(src)                   (((src) & 0x00000700)>>8)
#define USB0_COMPDISTUNE_F1_WR(src)              (((u32)(src)<<8) & 0x00000700)
#define USB0_COMPDISTUNE_F1_SET(dst,src) \
                       (((dst) & ~0x00000700) | (((u32)(src)<<8) & 0x00000700))
/*	 Fields USB0_SQRXTUNE	 */
#define USB0_SQRXTUNE_F1_WIDTH                                                3
#define USB0_SQRXTUNE_F1_SHIFT                                                4
#define USB0_SQRXTUNE_F1_MASK                                        0x00000070
#define USB0_SQRXTUNE_F1_RD(src)                      (((src) & 0x00000070)>>4)
#define USB0_SQRXTUNE_F1_WR(src)                 (((u32)(src)<<4) & 0x00000070)
#define USB0_SQRXTUNE_F1_SET(dst,src) \
                       (((dst) & ~0x00000070) | (((u32)(src)<<4) & 0x00000070))
/*	 Fields USB0_OTGTUNE	 */
#define USB0_OTGTUNE_F1_WIDTH                                                 3
#define USB0_OTGTUNE_F1_SHIFT                                                 0
#define USB0_OTGTUNE_F1_MASK                                         0x00000007
#define USB0_OTGTUNE_F1_RD(src)                          (((src) & 0x00000007))
#define USB0_OTGTUNE_F1_WR(src)                     (((u32)(src)) & 0x00000007)
#define USB0_OTGTUNE_F1_SET(dst,src) \
                          (((dst) & ~0x00000007) | (((u32)(src)) & 0x00000007))

/*	Register USB1_RSTCTL	*/ 
/*	 Fields USB0_OTGDISABLE	 */
#define USB0_OTGDISABLE_F1_WIDTH                                              1
#define USB0_OTGDISABLE_F1_SHIFT                                              2
#define USB0_OTGDISABLE_F1_MASK                                      0x00000004
#define USB0_OTGDISABLE_F1_RD(src)                    (((src) & 0x00000004)>>2)
#define USB0_OTGDISABLE_F1_WR(src)               (((u32)(src)<<2) & 0x00000004)
#define USB0_OTGDISABLE_F1_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields USB0_PORTRESET	 */
#define USB0_PORTRESET_F1_WIDTH                                               1
#define USB0_PORTRESET_F1_SHIFT                                               1
#define USB0_PORTRESET_F1_MASK                                       0x00000002
#define USB0_PORTRESET_F1_RD(src)                     (((src) & 0x00000002)>>1)
#define USB0_PORTRESET_F1_WR(src)                (((u32)(src)<<1) & 0x00000002)
#define USB0_PORTRESET_F1_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields USB0_POR	 */
#define USB0_POR_F1_WIDTH                                                     1
#define USB0_POR_F1_SHIFT                                                     0
#define USB0_POR_F1_MASK                                             0x00000001
#define USB0_POR_F1_RD(src)                              (((src) & 0x00000001))
#define USB0_POR_F1_WR(src)                         (((u32)(src)) & 0x00000001)
#define USB0_POR_F1_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register USB1_CLKCTL	*/ 
/*	 Fields USB0_SIM_SCALEDOWN	 */
#define USB0_SIM_SCALEDOWN_F1_WIDTH                                           2
#define USB0_SIM_SCALEDOWN_F1_SHIFT                                          30
#define USB0_SIM_SCALEDOWN_F1_MASK                                   0xc0000000
#define USB0_SIM_SCALEDOWN_F1_RD(src)                (((src) & 0xc0000000)>>30)
#define USB0_SIM_SCALEDOWN_F1_WR(src)           (((u32)(src)<<30) & 0xc0000000)
#define USB0_SIM_SCALEDOWN_F1_SET(dst,src) \
                      (((dst) & ~0xc0000000) | (((u32)(src)<<30) & 0xc0000000))
/*	 Fields USB0_DRVVBUS_POLARITY	 */
#define USB0_DRVVBUS_POLARITY_F1_WIDTH                                        1
#define USB0_DRVVBUS_POLARITY_F1_SHIFT                                        7
#define USB0_DRVVBUS_POLARITY_F1_MASK                                0x00000080
#define USB0_DRVVBUS_POLARITY_F1_RD(src)              (((src) & 0x00000080)>>7)
#define USB0_DRVVBUS_POLARITY_F1_WR(src)         (((u32)(src)<<7) & 0x00000080)
#define USB0_DRVVBUS_POLARITY_F1_SET(dst,src) \
                       (((dst) & ~0x00000080) | (((u32)(src)<<7) & 0x00000080))
/*	 Fields USB0_REFCLKSEL	 */
#define USB0_REFCLKSEL_F1_WIDTH                                               2
#define USB0_REFCLKSEL_F1_SHIFT                                               2
#define USB0_REFCLKSEL_F1_MASK                                       0x0000000c
#define USB0_REFCLKSEL_F1_RD(src)                     (((src) & 0x0000000c)>>2)
#define USB0_REFCLKSEL_F1_WR(src)                (((u32)(src)<<2) & 0x0000000c)
#define USB0_REFCLKSEL_F1_SET(dst,src) \
                       (((dst) & ~0x0000000c) | (((u32)(src)<<2) & 0x0000000c))
/*	 Fields USB0_REFCLKDIV	 */
#define USB0_REFCLKDIV_F1_WIDTH                                               2
#define USB0_REFCLKDIV_F1_SHIFT                                               0
#define USB0_REFCLKDIV_F1_MASK                                       0x00000003
#define USB0_REFCLKDIV_F1_RD(src)                        (((src) & 0x00000003))
#define USB0_REFCLKDIV_F1_WR(src)                   (((u32)(src)) & 0x00000003)
#define USB0_REFCLKDIV_F1_SET(dst,src) \
                          (((dst) & ~0x00000003) | (((u32)(src)) & 0x00000003))

/*	Register SDHC_TAP_DELAY	*/ 
/*	 Fields CFG_OP_TAP_EN	 */
#define CFG_OP_TAP_EN_WIDTH                                                   1
#define CFG_OP_TAP_EN_SHIFT                                                  16
#define CFG_OP_TAP_EN_MASK                                           0x00010000
#define CFG_OP_TAP_EN_RD(src)                        (((src) & 0x00010000)>>16)
#define CFG_OP_TAP_EN_WR(src)                   (((u32)(src)<<16) & 0x00010000)
#define CFG_OP_TAP_EN_SET(dst,src) \
                      (((dst) & ~0x00010000) | (((u32)(src)<<16) & 0x00010000))
/*	 Fields CFG_OP_TAP_DELAY	 */
#define CFG_OP_TAP_DELAY_WIDTH                                                4
#define CFG_OP_TAP_DELAY_SHIFT                                               12
#define CFG_OP_TAP_DELAY_MASK                                        0x0000f000
#define CFG_OP_TAP_DELAY_RD(src)                     (((src) & 0x0000f000)>>12)
#define CFG_OP_TAP_DELAY_WR(src)                (((u32)(src)<<12) & 0x0000f000)
#define CFG_OP_TAP_DELAY_SET(dst,src) \
                      (((dst) & ~0x0000f000) | (((u32)(src)<<12) & 0x0000f000))
/*	 Fields CFG_OP_DLY_CTRL	 */
#define CFG_OP_DLY_CTRL_WIDTH                                                 2
#define CFG_OP_DLY_CTRL_SHIFT                                                10
#define CFG_OP_DLY_CTRL_MASK                                         0x00000c00
#define CFG_OP_DLY_CTRL_RD(src)                      (((src) & 0x00000c00)>>10)
#define CFG_OP_DLY_CTRL_WR(src)                 (((u32)(src)<<10) & 0x00000c00)
#define CFG_OP_DLY_CTRL_SET(dst,src) \
                      (((dst) & ~0x00000c00) | (((u32)(src)<<10) & 0x00000c00))
/*	 Fields CFG_IP_TAP_EN	 */
#define CFG_IP_TAP_EN_WIDTH                                                   1
#define CFG_IP_TAP_EN_SHIFT                                                   9
#define CFG_IP_TAP_EN_MASK                                           0x00000200
#define CFG_IP_TAP_EN_RD(src)                         (((src) & 0x00000200)>>9)
#define CFG_IP_TAP_EN_WR(src)                    (((u32)(src)<<9) & 0x00000200)
#define CFG_IP_TAP_EN_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields CFG_IP_TAP_DELAY	 */
#define CFG_IP_TAP_DELAY_WIDTH                                                6
#define CFG_IP_TAP_DELAY_SHIFT                                                3
#define CFG_IP_TAP_DELAY_MASK                                        0x000001f8
#define CFG_IP_TAP_DELAY_RD(src)                      (((src) & 0x000001f8)>>3)
#define CFG_IP_TAP_DELAY_WR(src)                 (((u32)(src)<<3) & 0x000001f8)
#define CFG_IP_TAP_DELAY_SET(dst,src) \
                       (((dst) & ~0x000001f8) | (((u32)(src)<<3) & 0x000001f8))
/*	 Fields CFG_IP_DLY_CTRL	 */
#define CFG_IP_DLY_CTRL_WIDTH                                                 2
#define CFG_IP_DLY_CTRL_SHIFT                                                 1
#define CFG_IP_DLY_CTRL_MASK                                         0x00000006
#define CFG_IP_DLY_CTRL_RD(src)                       (((src) & 0x00000006)>>1)
#define CFG_IP_DLY_CTRL_WR(src)                  (((u32)(src)<<1) & 0x00000006)
#define CFG_IP_DLY_CTRL_SET(dst,src) \
                       (((dst) & ~0x00000006) | (((u32)(src)<<1) & 0x00000006))
/*	 Fields CFG_CMD_CONFLICT_DIS	 */
#define CFG_CMD_CONFLICT_DIS_WIDTH                                            1
#define CFG_CMD_CONFLICT_DIS_SHIFT                                            0
#define CFG_CMD_CONFLICT_DIS_MASK                                    0x00000001
#define CFG_CMD_CONFLICT_DIS_RD(src)                     (((src) & 0x00000001))
#define CFG_CMD_CONFLICT_DIS_WR(src)                (((u32)(src)) & 0x00000001)
#define CFG_CMD_CONFLICT_DIS_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register SDHC_CAP_REG_LOW_S1	*/ 
/*	 Fields SDHC_CAP_REG_LOW_S1	 */
#define SDHC_CAP_REG_LOW_S11_WIDTH                                           32
#define SDHC_CAP_REG_LOW_S11_SHIFT                                            0
#define SDHC_CAP_REG_LOW_S11_MASK                                    0xffffffff
#define SDHC_CAP_REG_LOW_S11_RD(src)                     (((src) & 0xffffffff))
#define SDHC_CAP_REG_LOW_S11_WR(src)                (((u32)(src)) & 0xffffffff)
#define SDHC_CAP_REG_LOW_S11_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register SDHC_CAP_REG_HIGH_S1	*/ 
/*	 Fields SDHC_CAP_REG_HIGH_S1	 */
#define SDHC_CAP_REG_HIGH_S11_WIDTH                                          20
#define SDHC_CAP_REG_HIGH_S11_SHIFT                                           0
#define SDHC_CAP_REG_HIGH_S11_MASK                                   0x000fffff
#define SDHC_CAP_REG_HIGH_S11_RD(src)                    (((src) & 0x000fffff))
#define SDHC_CAP_REG_HIGH_S11_WR(src)               (((u32)(src)) & 0x000fffff)
#define SDHC_CAP_REG_HIGH_S11_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register SDHC_CAP_REG_LOW_S2	*/ 
/*	 Fields SDHC_CAP_REG_LOW_S2	 */
#define SDHC_CAP_REG_LOW_S22_WIDTH                                           32
#define SDHC_CAP_REG_LOW_S22_SHIFT                                            0
#define SDHC_CAP_REG_LOW_S22_MASK                                    0xffffffff
#define SDHC_CAP_REG_LOW_S22_RD(src)                     (((src) & 0xffffffff))
#define SDHC_CAP_REG_LOW_S22_WR(src)                (((u32)(src)) & 0xffffffff)
#define SDHC_CAP_REG_LOW_S22_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register SDHC_CAP_REG_HIGH_S2	*/ 
/*	 Fields SDHC_CAP_REG_HIGH_S2	 */
#define SDHC_CAP_REG_HIGH_S22_WIDTH                                          20
#define SDHC_CAP_REG_HIGH_S22_SHIFT                                           0
#define SDHC_CAP_REG_HIGH_S22_MASK                                   0x000fffff
#define SDHC_CAP_REG_HIGH_S22_RD(src)                    (((src) & 0x000fffff))
#define SDHC_CAP_REG_HIGH_S22_WR(src)               (((u32)(src)) & 0x000fffff)
#define SDHC_CAP_REG_HIGH_S22_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register SDHC_INIT_PRESET	*/ 
/*	 Fields INIT_PRESET_S1	 */
#define INIT_PRESET_S1_WIDTH                                                 13
#define INIT_PRESET_S1_SHIFT                                                 13
#define INIT_PRESET_S1_MASK                                          0x03ffe000
#define INIT_PRESET_S1_RD(src)                       (((src) & 0x03ffe000)>>13)
#define INIT_PRESET_S1_WR(src)                  (((u32)(src)<<13) & 0x03ffe000)
#define INIT_PRESET_S1_SET(dst,src) \
                      (((dst) & ~0x03ffe000) | (((u32)(src)<<13) & 0x03ffe000))
/*	 Fields INIT_PRESET_S2	 */
#define INIT_PRESET_S2_WIDTH                                                 13
#define INIT_PRESET_S2_SHIFT                                                  0
#define INIT_PRESET_S2_MASK                                          0x00001fff
#define INIT_PRESET_S2_RD(src)                           (((src) & 0x00001fff))
#define INIT_PRESET_S2_WR(src)                      (((u32)(src)) & 0x00001fff)
#define INIT_PRESET_S2_SET(dst,src) \
                          (((dst) & ~0x00001fff) | (((u32)(src)) & 0x00001fff))

/*	Register SDHC_DEFAULT_PRESET	*/ 
/*	 Fields DEFAULT_PRESET_S1	 */
#define DEFAULT_PRESET_S1_WIDTH                                              13
#define DEFAULT_PRESET_S1_SHIFT                                              13
#define DEFAULT_PRESET_S1_MASK                                       0x03ffe000
#define DEFAULT_PRESET_S1_RD(src)                    (((src) & 0x03ffe000)>>13)
#define DEFAULT_PRESET_S1_WR(src)               (((u32)(src)<<13) & 0x03ffe000)
#define DEFAULT_PRESET_S1_SET(dst,src) \
                      (((dst) & ~0x03ffe000) | (((u32)(src)<<13) & 0x03ffe000))
/*	 Fields DEFAULT_PRESET_S2	 */
#define DEFAULT_PRESET_S2_WIDTH                                              13
#define DEFAULT_PRESET_S2_SHIFT                                               0
#define DEFAULT_PRESET_S2_MASK                                       0x00001fff
#define DEFAULT_PRESET_S2_RD(src)                        (((src) & 0x00001fff))
#define DEFAULT_PRESET_S2_WR(src)                   (((u32)(src)) & 0x00001fff)
#define DEFAULT_PRESET_S2_SET(dst,src) \
                          (((dst) & ~0x00001fff) | (((u32)(src)) & 0x00001fff))

/*	Register SDHC_HIGH_SPEED_PRESET	*/ 
/*	 Fields HIGH_SPEED_PRESET_S1	 */
#define HIGH_SPEED_PRESET_S1_WIDTH                                           13
#define HIGH_SPEED_PRESET_S1_SHIFT                                           13
#define HIGH_SPEED_PRESET_S1_MASK                                    0x03ffe000
#define HIGH_SPEED_PRESET_S1_RD(src)                 (((src) & 0x03ffe000)>>13)
#define HIGH_SPEED_PRESET_S1_WR(src)            (((u32)(src)<<13) & 0x03ffe000)
#define HIGH_SPEED_PRESET_S1_SET(dst,src) \
                      (((dst) & ~0x03ffe000) | (((u32)(src)<<13) & 0x03ffe000))
/*	 Fields HIGH_SPEED_PRESET_S2	 */
#define HIGH_SPEED_PRESET_S2_WIDTH                                           13
#define HIGH_SPEED_PRESET_S2_SHIFT                                            0
#define HIGH_SPEED_PRESET_S2_MASK                                    0x00001fff
#define HIGH_SPEED_PRESET_S2_RD(src)                     (((src) & 0x00001fff))
#define HIGH_SPEED_PRESET_S2_WR(src)                (((u32)(src)) & 0x00001fff)
#define HIGH_SPEED_PRESET_S2_SET(dst,src) \
                          (((dst) & ~0x00001fff) | (((u32)(src)) & 0x00001fff))

/*	Register SDHC_SDR12_PRESET	*/ 
/*	 Fields SDR12_PRESET_S1	 */
#define SDR12_PRESET_S1_WIDTH                                                13
#define SDR12_PRESET_S1_SHIFT                                                13
#define SDR12_PRESET_S1_MASK                                         0x03ffe000
#define SDR12_PRESET_S1_RD(src)                      (((src) & 0x03ffe000)>>13)
#define SDR12_PRESET_S1_WR(src)                 (((u32)(src)<<13) & 0x03ffe000)
#define SDR12_PRESET_S1_SET(dst,src) \
                      (((dst) & ~0x03ffe000) | (((u32)(src)<<13) & 0x03ffe000))
/*	 Fields SDR12_PRESET_S2	 */
#define SDR12_PRESET_S2_WIDTH                                                13
#define SDR12_PRESET_S2_SHIFT                                                 0
#define SDR12_PRESET_S2_MASK                                         0x00001fff
#define SDR12_PRESET_S2_RD(src)                          (((src) & 0x00001fff))
#define SDR12_PRESET_S2_WR(src)                     (((u32)(src)) & 0x00001fff)
#define SDR12_PRESET_S2_SET(dst,src) \
                          (((dst) & ~0x00001fff) | (((u32)(src)) & 0x00001fff))

/*	Register SDHC_SDR25_PRESET	*/ 
/*	 Fields SDR25_PRESET_S1	 */
#define SDR25_PRESET_S1_WIDTH                                                13
#define SDR25_PRESET_S1_SHIFT                                                13
#define SDR25_PRESET_S1_MASK                                         0x03ffe000
#define SDR25_PRESET_S1_RD(src)                      (((src) & 0x03ffe000)>>13)
#define SDR25_PRESET_S1_WR(src)                 (((u32)(src)<<13) & 0x03ffe000)
#define SDR25_PRESET_S1_SET(dst,src) \
                      (((dst) & ~0x03ffe000) | (((u32)(src)<<13) & 0x03ffe000))
/*	 Fields SDR25_PRESET_S2	 */
#define SDR25_PRESET_S2_WIDTH                                                13
#define SDR25_PRESET_S2_SHIFT                                                 0
#define SDR25_PRESET_S2_MASK                                         0x00001fff
#define SDR25_PRESET_S2_RD(src)                          (((src) & 0x00001fff))
#define SDR25_PRESET_S2_WR(src)                     (((u32)(src)) & 0x00001fff)
#define SDR25_PRESET_S2_SET(dst,src) \
                          (((dst) & ~0x00001fff) | (((u32)(src)) & 0x00001fff))

/*	Register SDHC_SDR50_PRESET	*/ 
/*	 Fields SDR50_PRESET_S1	 */
#define SDR50_PRESET_S1_WIDTH                                                13
#define SDR50_PRESET_S1_SHIFT                                                13
#define SDR50_PRESET_S1_MASK                                         0x03ffe000
#define SDR50_PRESET_S1_RD(src)                      (((src) & 0x03ffe000)>>13)
#define SDR50_PRESET_S1_WR(src)                 (((u32)(src)<<13) & 0x03ffe000)
#define SDR50_PRESET_S1_SET(dst,src) \
                      (((dst) & ~0x03ffe000) | (((u32)(src)<<13) & 0x03ffe000))
/*	 Fields SDR50_PRESET_S2	 */
#define SDR50_PRESET_S2_WIDTH                                                13
#define SDR50_PRESET_S2_SHIFT                                                 0
#define SDR50_PRESET_S2_MASK                                         0x00001fff
#define SDR50_PRESET_S2_RD(src)                          (((src) & 0x00001fff))
#define SDR50_PRESET_S2_WR(src)                     (((u32)(src)) & 0x00001fff)
#define SDR50_PRESET_S2_SET(dst,src) \
                          (((dst) & ~0x00001fff) | (((u32)(src)) & 0x00001fff))

/*	Register SDHC_SDR104_PRESET	*/ 
/*	 Fields SDR104_PRESET_S1	 */
#define SDR104_PRESET_S1_WIDTH                                               13
#define SDR104_PRESET_S1_SHIFT                                               13
#define SDR104_PRESET_S1_MASK                                        0x03ffe000
#define SDR104_PRESET_S1_RD(src)                     (((src) & 0x03ffe000)>>13)
#define SDR104_PRESET_S1_WR(src)                (((u32)(src)<<13) & 0x03ffe000)
#define SDR104_PRESET_S1_SET(dst,src) \
                      (((dst) & ~0x03ffe000) | (((u32)(src)<<13) & 0x03ffe000))
/*	 Fields SDR104_PRESET_S2	 */
#define SDR104_PRESET_S2_WIDTH                                               13
#define SDR104_PRESET_S2_SHIFT                                                0
#define SDR104_PRESET_S2_MASK                                        0x00001fff
#define SDR104_PRESET_S2_RD(src)                         (((src) & 0x00001fff))
#define SDR104_PRESET_S2_WR(src)                    (((u32)(src)) & 0x00001fff)
#define SDR104_PRESET_S2_SET(dst,src) \
                          (((dst) & ~0x00001fff) | (((u32)(src)) & 0x00001fff))

/*	Register SDHC_DDR50_PRESET	*/ 
/*	 Fields DDR50_PRESET_S1	 */
#define DDR50_PRESET_S1_WIDTH                                                13
#define DDR50_PRESET_S1_SHIFT                                                13
#define DDR50_PRESET_S1_MASK                                         0x03ffe000
#define DDR50_PRESET_S1_RD(src)                      (((src) & 0x03ffe000)>>13)
#define DDR50_PRESET_S1_WR(src)                 (((u32)(src)<<13) & 0x03ffe000)
#define DDR50_PRESET_S1_SET(dst,src) \
                      (((dst) & ~0x03ffe000) | (((u32)(src)<<13) & 0x03ffe000))
/*	 Fields DDR50_PRESET_S2	 */
#define DDR50_PRESET_S2_WIDTH                                                13
#define DDR50_PRESET_S2_SHIFT                                                 0
#define DDR50_PRESET_S2_MASK                                         0x00001fff
#define DDR50_PRESET_S2_RD(src)                          (((src) & 0x00001fff))
#define DDR50_PRESET_S2_WR(src)                     (((u32)(src)) & 0x00001fff)
#define DDR50_PRESET_S2_SET(dst,src) \
                          (((dst) & ~0x00001fff) | (((u32)(src)) & 0x00001fff))

/*	Register USBHost_Cfg	*/ 
/*	 Fields usbh_word_if	 */
#define USBH_WORD_IF_WIDTH                                                    1
#define USBH_WORD_IF_SHIFT                                                   12
#define USBH_WORD_IF_MASK                                            0x00001000
#define USBH_WORD_IF_RD(src)                         (((src) & 0x00001000)>>12)
#define USBH_WORD_IF_WR(src)                    (((u32)(src)<<12) & 0x00001000)
#define USBH_WORD_IF_SET(dst,src) \
                      (((dst) & ~0x00001000) | (((u32)(src)<<12) & 0x00001000))
/*	 Fields usbh_ohci_susp_lgcy_i	 */
#define USBH_OHCI_SUSP_LGCY_I_WIDTH                                           1
#define USBH_OHCI_SUSP_LGCY_I_SHIFT                                          11
#define USBH_OHCI_SUSP_LGCY_I_MASK                                   0x00000800
#define USBH_OHCI_SUSP_LGCY_I_RD(src)                (((src) & 0x00000800)>>11)
#define USBH_OHCI_SUSP_LGCY_I_WR(src)           (((u32)(src)<<11) & 0x00000800)
#define USBH_OHCI_SUSP_LGCY_I_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*	 Fields usbh_app_start_clk_i	 */
#define USBH_APP_START_CLK_I_WIDTH                                            1
#define USBH_APP_START_CLK_I_SHIFT                                           10
#define USBH_APP_START_CLK_I_MASK                                    0x00000400
#define USBH_APP_START_CLK_I_RD(src)                 (((src) & 0x00000400)>>10)
#define USBH_APP_START_CLK_I_WR(src)            (((u32)(src)<<10) & 0x00000400)
#define USBH_APP_START_CLK_I_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields usbh_autoppd	 */
#define USBH_AUTOPPD_WIDTH                                                    1
#define USBH_AUTOPPD_SHIFT                                                    9
#define USBH_AUTOPPD_MASK                                            0x00000200
#define USBH_AUTOPPD_RD(src)                          (((src) & 0x00000200)>>9)
#define USBH_AUTOPPD_WR(src)                     (((u32)(src)<<9) & 0x00000200)
#define USBH_AUTOPPD_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields usbh_frame_len	 */
#define USBH_FRAME_LEN_WIDTH                                                  6
#define USBH_FRAME_LEN_SHIFT                                                  3
#define USBH_FRAME_LEN_MASK                                          0x000001f8
#define USBH_FRAME_LEN_RD(src)                        (((src) & 0x000001f8)>>3)
#define USBH_FRAME_LEN_WR(src)                   (((u32)(src)<<3) & 0x000001f8)
#define USBH_FRAME_LEN_SET(dst,src) \
                       (((dst) & ~0x000001f8) | (((u32)(src)<<3) & 0x000001f8))
/*	 Fields usbh_sim_mode	 */
#define USBH_SIM_MODE_WIDTH                                                   1
#define USBH_SIM_MODE_SHIFT                                                   2
#define USBH_SIM_MODE_MASK                                           0x00000004
#define USBH_SIM_MODE_RD(src)                         (((src) & 0x00000004)>>2)
#define USBH_SIM_MODE_WR(src)                    (((u32)(src)<<2) & 0x00000004)
#define USBH_SIM_MODE_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields ss_resume_utmi_pls_dis	 */
#define SS_RESUME_UTMI_PLS_DIS_WIDTH                                          1
#define SS_RESUME_UTMI_PLS_DIS_SHIFT                                          1
#define SS_RESUME_UTMI_PLS_DIS_MASK                                  0x00000002
#define SS_RESUME_UTMI_PLS_DIS_RD(src)                (((src) & 0x00000002)>>1)
#define SS_RESUME_UTMI_PLS_DIS_WR(src)           (((u32)(src)<<1) & 0x00000002)
#define SS_RESUME_UTMI_PLS_DIS_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields ss_utmi_backward_enb	 */
#define SS_UTMI_BACKWARD_ENB_WIDTH                                            1
#define SS_UTMI_BACKWARD_ENB_SHIFT                                            0
#define SS_UTMI_BACKWARD_ENB_MASK                                    0x00000001
#define SS_UTMI_BACKWARD_ENB_RD(src)                     (((src) & 0x00000001))
#define SS_UTMI_BACKWARD_ENB_WR(src)                (((u32)(src)) & 0x00000001)
#define SS_UTMI_BACKWARD_ENB_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register USBHost_Status	*/ 
/*	 Fields ohci_rmtwkp_status	 */
#define OHCI_RMTWKP_STATUS_WIDTH                                              1
#define OHCI_RMTWKP_STATUS_SHIFT                                              0
#define OHCI_RMTWKP_STATUS_MASK                                      0x00000001
#define OHCI_RMTWKP_STATUS_RD(src)                       (((src) & 0x00000001))
#define OHCI_RMTWKP_STATUS_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register USBHost_Fatal_CFG	*/ 
/*	 Fields CFG_usbh_sys_intr	 */
#define CFG_USBH_SYS_INTR_WIDTH                                               1
#define CFG_USBH_SYS_INTR_SHIFT                                               0
#define CFG_USBH_SYS_INTR_MASK                                       0x00000001
#define CFG_USBH_SYS_INTR_RD(src)                        (((src) & 0x00000001))
#define CFG_USBH_SYS_INTR_WR(src)                   (((u32)(src)) & 0x00000001)
#define CFG_USBH_SYS_INTR_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register SDR_EHCI_0_AHB2AXI_CFG	*/ 
/*	 Fields pw_axi_sideband_prio	 */
#define PW_AXI_SIDEBAND_PRIO_F3_WIDTH                                         3
#define PW_AXI_SIDEBAND_PRIO_F3_SHIFT                                        21
#define PW_AXI_SIDEBAND_PRIO_F3_MASK                                 0x00e00000
#define PW_AXI_SIDEBAND_PRIO_F3_RD(src)              (((src) & 0x00e00000)>>21)
#define PW_AXI_SIDEBAND_PRIO_F3_WR(src)         (((u32)(src)<<21) & 0x00e00000)
#define PW_AXI_SIDEBAND_PRIO_F3_SET(dst,src) \
                      (((dst) & ~0x00e00000) | (((u32)(src)<<21) & 0x00e00000))
/*	 Fields pr_axi_sideband_prio	 */
#define PR_AXI_SIDEBAND_PRIO_F3_WIDTH                                         3
#define PR_AXI_SIDEBAND_PRIO_F3_SHIFT                                        17
#define PR_AXI_SIDEBAND_PRIO_F3_MASK                                 0x000e0000
#define PR_AXI_SIDEBAND_PRIO_F3_RD(src)              (((src) & 0x000e0000)>>17)
#define PR_AXI_SIDEBAND_PRIO_F3_WR(src)         (((u32)(src)<<17) & 0x000e0000)
#define PR_AXI_SIDEBAND_PRIO_F3_SET(dst,src) \
                      (((dst) & ~0x000e0000) | (((u32)(src)<<17) & 0x000e0000))
/*	 Fields writes_block_reads	 */
#define WRITES_BLOCK_READS_F3_WIDTH                                           1
#define WRITES_BLOCK_READS_F3_SHIFT                                          15
#define WRITES_BLOCK_READS_F3_MASK                                   0x00008000
#define WRITES_BLOCK_READS_F3_RD(src)                (((src) & 0x00008000)>>15)
#define WRITES_BLOCK_READS_F3_WR(src)           (((u32)(src)<<15) & 0x00008000)
#define WRITES_BLOCK_READS_F3_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))
/*	 Fields pw_max_outstanding	 */
#define PW_MAX_OUTSTANDING_F3_WIDTH                                           4
#define PW_MAX_OUTSTANDING_F3_SHIFT                                           8
#define PW_MAX_OUTSTANDING_F3_MASK                                   0x00000f00
#define PW_MAX_OUTSTANDING_F3_RD(src)                 (((src) & 0x00000f00)>>8)
#define PW_MAX_OUTSTANDING_F3_WR(src)            (((u32)(src)<<8) & 0x00000f00)
#define PW_MAX_OUTSTANDING_F3_SET(dst,src) \
                       (((dst) & ~0x00000f00) | (((u32)(src)<<8) & 0x00000f00))
/*	 Fields tx_wrr_weight	 */
#define TX_WRR_WEIGHT_F3_WIDTH                                                4
#define TX_WRR_WEIGHT_F3_SHIFT                                                4
#define TX_WRR_WEIGHT_F3_MASK                                        0x000000f0
#define TX_WRR_WEIGHT_F3_RD(src)                      (((src) & 0x000000f0)>>4)
#define TX_WRR_WEIGHT_F3_WR(src)                 (((u32)(src)<<4) & 0x000000f0)
#define TX_WRR_WEIGHT_F3_SET(dst,src) \
                       (((dst) & ~0x000000f0) | (((u32)(src)<<4) & 0x000000f0))
/*	 Fields rx_wrr_weight	 */
#define RX_WRR_WEIGHT_F3_WIDTH                                                4
#define RX_WRR_WEIGHT_F3_SHIFT                                                0
#define RX_WRR_WEIGHT_F3_MASK                                        0x0000000f
#define RX_WRR_WEIGHT_F3_RD(src)                         (((src) & 0x0000000f))
#define RX_WRR_WEIGHT_F3_WR(src)                    (((u32)(src)) & 0x0000000f)
#define RX_WRR_WEIGHT_F3_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register SDR_EHCI_1_AHB2AXI_CFG	*/ 
/*	 Fields pw_axi_sideband_prio	 */
#define PW_AXI_SIDEBAND_PRIO_F4_WIDTH                                         3
#define PW_AXI_SIDEBAND_PRIO_F4_SHIFT                                        21
#define PW_AXI_SIDEBAND_PRIO_F4_MASK                                 0x00e00000
#define PW_AXI_SIDEBAND_PRIO_F4_RD(src)              (((src) & 0x00e00000)>>21)
#define PW_AXI_SIDEBAND_PRIO_F4_WR(src)         (((u32)(src)<<21) & 0x00e00000)
#define PW_AXI_SIDEBAND_PRIO_F4_SET(dst,src) \
                      (((dst) & ~0x00e00000) | (((u32)(src)<<21) & 0x00e00000))
/*	 Fields pr_axi_sideband_prio	 */
#define PR_AXI_SIDEBAND_PRIO_F4_WIDTH                                         3
#define PR_AXI_SIDEBAND_PRIO_F4_SHIFT                                        17
#define PR_AXI_SIDEBAND_PRIO_F4_MASK                                 0x000e0000
#define PR_AXI_SIDEBAND_PRIO_F4_RD(src)              (((src) & 0x000e0000)>>17)
#define PR_AXI_SIDEBAND_PRIO_F4_WR(src)         (((u32)(src)<<17) & 0x000e0000)
#define PR_AXI_SIDEBAND_PRIO_F4_SET(dst,src) \
                      (((dst) & ~0x000e0000) | (((u32)(src)<<17) & 0x000e0000))
/*	 Fields writes_block_reads	 */
#define WRITES_BLOCK_READS_F4_WIDTH                                           1
#define WRITES_BLOCK_READS_F4_SHIFT                                          15
#define WRITES_BLOCK_READS_F4_MASK                                   0x00008000
#define WRITES_BLOCK_READS_F4_RD(src)                (((src) & 0x00008000)>>15)
#define WRITES_BLOCK_READS_F4_WR(src)           (((u32)(src)<<15) & 0x00008000)
#define WRITES_BLOCK_READS_F4_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))
/*	 Fields pw_max_outstanding	 */
#define PW_MAX_OUTSTANDING_F4_WIDTH                                           4
#define PW_MAX_OUTSTANDING_F4_SHIFT                                           8
#define PW_MAX_OUTSTANDING_F4_MASK                                   0x00000f00
#define PW_MAX_OUTSTANDING_F4_RD(src)                 (((src) & 0x00000f00)>>8)
#define PW_MAX_OUTSTANDING_F4_WR(src)            (((u32)(src)<<8) & 0x00000f00)
#define PW_MAX_OUTSTANDING_F4_SET(dst,src) \
                       (((dst) & ~0x00000f00) | (((u32)(src)<<8) & 0x00000f00))
/*	 Fields tx_wrr_weight	 */
#define TX_WRR_WEIGHT_F4_WIDTH                                                4
#define TX_WRR_WEIGHT_F4_SHIFT                                                4
#define TX_WRR_WEIGHT_F4_MASK                                        0x000000f0
#define TX_WRR_WEIGHT_F4_RD(src)                      (((src) & 0x000000f0)>>4)
#define TX_WRR_WEIGHT_F4_WR(src)                 (((u32)(src)<<4) & 0x000000f0)
#define TX_WRR_WEIGHT_F4_SET(dst,src) \
                       (((dst) & ~0x000000f0) | (((u32)(src)<<4) & 0x000000f0))
/*	 Fields rx_wrr_weight	 */
#define RX_WRR_WEIGHT_F4_WIDTH                                                4
#define RX_WRR_WEIGHT_F4_SHIFT                                                0
#define RX_WRR_WEIGHT_F4_MASK                                        0x0000000f
#define RX_WRR_WEIGHT_F4_RD(src)                         (((src) & 0x0000000f))
#define RX_WRR_WEIGHT_F4_WR(src)                    (((u32)(src)) & 0x0000000f)
#define RX_WRR_WEIGHT_F4_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register SDR_OHCI_1_AHB2AXI_CFG	*/ 
/*	 Fields pw_axi_sideband_prio	 */
#define PW_AXI_SIDEBAND_PRIO_F5_WIDTH                                         3
#define PW_AXI_SIDEBAND_PRIO_F5_SHIFT                                        21
#define PW_AXI_SIDEBAND_PRIO_F5_MASK                                 0x00e00000
#define PW_AXI_SIDEBAND_PRIO_F5_RD(src)              (((src) & 0x00e00000)>>21)
#define PW_AXI_SIDEBAND_PRIO_F5_WR(src)         (((u32)(src)<<21) & 0x00e00000)
#define PW_AXI_SIDEBAND_PRIO_F5_SET(dst,src) \
                      (((dst) & ~0x00e00000) | (((u32)(src)<<21) & 0x00e00000))
/*	 Fields pr_axi_sideband_prio	 */
#define PR_AXI_SIDEBAND_PRIO_F5_WIDTH                                         3
#define PR_AXI_SIDEBAND_PRIO_F5_SHIFT                                        17
#define PR_AXI_SIDEBAND_PRIO_F5_MASK                                 0x000e0000
#define PR_AXI_SIDEBAND_PRIO_F5_RD(src)              (((src) & 0x000e0000)>>17)
#define PR_AXI_SIDEBAND_PRIO_F5_WR(src)         (((u32)(src)<<17) & 0x000e0000)
#define PR_AXI_SIDEBAND_PRIO_F5_SET(dst,src) \
                      (((dst) & ~0x000e0000) | (((u32)(src)<<17) & 0x000e0000))
/*	 Fields writes_block_reads	 */
#define WRITES_BLOCK_READS_F5_WIDTH                                           1
#define WRITES_BLOCK_READS_F5_SHIFT                                          15
#define WRITES_BLOCK_READS_F5_MASK                                   0x00008000
#define WRITES_BLOCK_READS_F5_RD(src)                (((src) & 0x00008000)>>15)
#define WRITES_BLOCK_READS_F5_WR(src)           (((u32)(src)<<15) & 0x00008000)
#define WRITES_BLOCK_READS_F5_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))
/*	 Fields pw_max_outstanding	 */
#define PW_MAX_OUTSTANDING_F5_WIDTH                                           4
#define PW_MAX_OUTSTANDING_F5_SHIFT                                           8
#define PW_MAX_OUTSTANDING_F5_MASK                                   0x00000f00
#define PW_MAX_OUTSTANDING_F5_RD(src)                 (((src) & 0x00000f00)>>8)
#define PW_MAX_OUTSTANDING_F5_WR(src)            (((u32)(src)<<8) & 0x00000f00)
#define PW_MAX_OUTSTANDING_F5_SET(dst,src) \
                       (((dst) & ~0x00000f00) | (((u32)(src)<<8) & 0x00000f00))
/*	 Fields tx_wrr_weight	 */
#define TX_WRR_WEIGHT_F5_WIDTH                                                4
#define TX_WRR_WEIGHT_F5_SHIFT                                                4
#define TX_WRR_WEIGHT_F5_MASK                                        0x000000f0
#define TX_WRR_WEIGHT_F5_RD(src)                      (((src) & 0x000000f0)>>4)
#define TX_WRR_WEIGHT_F5_WR(src)                 (((u32)(src)<<4) & 0x000000f0)
#define TX_WRR_WEIGHT_F5_SET(dst,src) \
                       (((dst) & ~0x000000f0) | (((u32)(src)<<4) & 0x000000f0))
/*	 Fields rx_wrr_weight	 */
#define RX_WRR_WEIGHT_F5_WIDTH                                                4
#define RX_WRR_WEIGHT_F5_SHIFT                                                0
#define RX_WRR_WEIGHT_F5_MASK                                        0x0000000f
#define RX_WRR_WEIGHT_F5_RD(src)                         (((src) & 0x0000000f))
#define RX_WRR_WEIGHT_F5_WR(src)                    (((u32)(src)) & 0x0000000f)
#define RX_WRR_WEIGHT_F5_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register SDR_tdm_AHB2AXI_CFG	*/ 
/*	 Fields pw_axi_sideband_prio	 */
#define PW_AXI_SIDEBAND_PRIO_F6_WIDTH                                         3
#define PW_AXI_SIDEBAND_PRIO_F6_SHIFT                                        21
#define PW_AXI_SIDEBAND_PRIO_F6_MASK                                 0x00e00000
#define PW_AXI_SIDEBAND_PRIO_F6_RD(src)              (((src) & 0x00e00000)>>21)
#define PW_AXI_SIDEBAND_PRIO_F6_WR(src)         (((u32)(src)<<21) & 0x00e00000)
#define PW_AXI_SIDEBAND_PRIO_F6_SET(dst,src) \
                      (((dst) & ~0x00e00000) | (((u32)(src)<<21) & 0x00e00000))
/*	 Fields pr_axi_sideband_prio	 */
#define PR_AXI_SIDEBAND_PRIO_F6_WIDTH                                         3
#define PR_AXI_SIDEBAND_PRIO_F6_SHIFT                                        17
#define PR_AXI_SIDEBAND_PRIO_F6_MASK                                 0x000e0000
#define PR_AXI_SIDEBAND_PRIO_F6_RD(src)              (((src) & 0x000e0000)>>17)
#define PR_AXI_SIDEBAND_PRIO_F6_WR(src)         (((u32)(src)<<17) & 0x000e0000)
#define PR_AXI_SIDEBAND_PRIO_F6_SET(dst,src) \
                      (((dst) & ~0x000e0000) | (((u32)(src)<<17) & 0x000e0000))
/*	 Fields writes_block_reads	 */
#define WRITES_BLOCK_READS_F6_WIDTH                                           1
#define WRITES_BLOCK_READS_F6_SHIFT                                          15
#define WRITES_BLOCK_READS_F6_MASK                                   0x00008000
#define WRITES_BLOCK_READS_F6_RD(src)                (((src) & 0x00008000)>>15)
#define WRITES_BLOCK_READS_F6_WR(src)           (((u32)(src)<<15) & 0x00008000)
#define WRITES_BLOCK_READS_F6_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))
/*	 Fields pw_max_outstanding	 */
#define PW_MAX_OUTSTANDING_F6_WIDTH                                           4
#define PW_MAX_OUTSTANDING_F6_SHIFT                                           8
#define PW_MAX_OUTSTANDING_F6_MASK                                   0x00000f00
#define PW_MAX_OUTSTANDING_F6_RD(src)                 (((src) & 0x00000f00)>>8)
#define PW_MAX_OUTSTANDING_F6_WR(src)            (((u32)(src)<<8) & 0x00000f00)
#define PW_MAX_OUTSTANDING_F6_SET(dst,src) \
                       (((dst) & ~0x00000f00) | (((u32)(src)<<8) & 0x00000f00))
/*	 Fields tx_wrr_weight	 */
#define TX_WRR_WEIGHT_F6_WIDTH                                                4
#define TX_WRR_WEIGHT_F6_SHIFT                                                4
#define TX_WRR_WEIGHT_F6_MASK                                        0x000000f0
#define TX_WRR_WEIGHT_F6_RD(src)                      (((src) & 0x000000f0)>>4)
#define TX_WRR_WEIGHT_F6_WR(src)                 (((u32)(src)<<4) & 0x000000f0)
#define TX_WRR_WEIGHT_F6_SET(dst,src) \
                       (((dst) & ~0x000000f0) | (((u32)(src)<<4) & 0x000000f0))
/*	 Fields rx_wrr_weight	 */
#define RX_WRR_WEIGHT_F6_WIDTH                                                4
#define RX_WRR_WEIGHT_F6_SHIFT                                                0
#define RX_WRR_WEIGHT_F6_MASK                                        0x0000000f
#define RX_WRR_WEIGHT_F6_RD(src)                         (((src) & 0x0000000f))
#define RX_WRR_WEIGHT_F6_WR(src)                    (((u32)(src)) & 0x0000000f)
#define RX_WRR_WEIGHT_F6_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register ENABLE_INTF	*/ 
/*	 Fields CFG_BE_FOR_RD	 */
#define CFG_BE_FOR_RD_WIDTH                                                   4
#define CFG_BE_FOR_RD_SHIFT                                                  24
#define CFG_BE_FOR_RD_MASK                                           0x0f000000
#define CFG_BE_FOR_RD_RD(src)                        (((src) & 0x0f000000)>>24)
#define CFG_BE_FOR_RD_WR(src)                   (((u32)(src)<<24) & 0x0f000000)
#define CFG_BE_FOR_RD_SET(dst,src) \
                      (((dst) & ~0x0f000000) | (((u32)(src)<<24) & 0x0f000000))
/*	 Fields CFG_X2H_RD_USE_DEFINED_ONLY	 */
#define CFG_X2H_RD_USE_DEFINED_ONLY_WIDTH                                     1
#define CFG_X2H_RD_USE_DEFINED_ONLY_SHIFT                                    23
#define CFG_X2H_RD_USE_DEFINED_ONLY_MASK                             0x00800000
#define CFG_X2H_RD_USE_DEFINED_ONLY_RD(src)          (((src) & 0x00800000)>>23)
#define CFG_X2H_RD_USE_DEFINED_ONLY_WR(src)     (((u32)(src)<<23) & 0x00800000)
#define CFG_X2H_RD_USE_DEFINED_ONLY_SET(dst,src) \
                      (((dst) & ~0x00800000) | (((u32)(src)<<23) & 0x00800000))
/*	 Fields CFG_X2H_USE_DEFINED_ONLY	 */
#define CFG_X2H_USE_DEFINED_ONLY_WIDTH                                        1
#define CFG_X2H_USE_DEFINED_ONLY_SHIFT                                       22
#define CFG_X2H_USE_DEFINED_ONLY_MASK                                0x00400000
#define CFG_X2H_USE_DEFINED_ONLY_RD(src)             (((src) & 0x00400000)>>22)
#define CFG_X2H_USE_DEFINED_ONLY_WR(src)        (((u32)(src)<<22) & 0x00400000)
#define CFG_X2H_USE_DEFINED_ONLY_SET(dst,src) \
                      (((dst) & ~0x00400000) | (((u32)(src)<<22) & 0x00400000))
/*	 Fields CFG_TH	 */
#define CFG_TH_WIDTH                                                         12
#define CFG_TH_SHIFT                                                         10
#define CFG_TH_MASK                                                  0x003ffc00
#define CFG_TH_RD(src)                               (((src) & 0x003ffc00)>>10)
#define CFG_TH_WR(src)                          (((u32)(src)<<10) & 0x003ffc00)
#define CFG_TH_SET(dst,src) \
                      (((dst) & ~0x003ffc00) | (((u32)(src)<<10) & 0x003ffc00))
/*	 Fields CFG_SWAP_SPI_CS_3	 */
#define CFG_SWAP_SPI_CS_3_WIDTH                                               1
#define CFG_SWAP_SPI_CS_3_SHIFT                                               9
#define CFG_SWAP_SPI_CS_3_MASK                                       0x00000200
#define CFG_SWAP_SPI_CS_3_RD(src)                     (((src) & 0x00000200)>>9)
#define CFG_SWAP_SPI_CS_3_WR(src)                (((u32)(src)<<9) & 0x00000200)
#define CFG_SWAP_SPI_CS_3_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields CFG_SWAP_SPI_CS_2	 */
#define CFG_SWAP_SPI_CS_2_WIDTH                                               1
#define CFG_SWAP_SPI_CS_2_SHIFT                                               8
#define CFG_SWAP_SPI_CS_2_MASK                                       0x00000100
#define CFG_SWAP_SPI_CS_2_RD(src)                     (((src) & 0x00000100)>>8)
#define CFG_SWAP_SPI_CS_2_WR(src)                (((u32)(src)<<8) & 0x00000100)
#define CFG_SWAP_SPI_CS_2_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*	 Fields CFG_TRACE_DISABLE	 */
#define CFG_TRACE_DISABLE_WIDTH                                               1
#define CFG_TRACE_DISABLE_SHIFT                                               7
#define CFG_TRACE_DISABLE_MASK                                       0x00000080
#define CFG_TRACE_DISABLE_RD(src)                     (((src) & 0x00000080)>>7)
#define CFG_TRACE_DISABLE_WR(src)                (((u32)(src)<<7) & 0x00000080)
#define CFG_TRACE_DISABLE_SET(dst,src) \
                       (((dst) & ~0x00000080) | (((u32)(src)<<7) & 0x00000080))
/*	 Fields CFG_SRAM_MUX	 */
#define CFG_SRAM_MUX_WIDTH                                                    4
#define CFG_SRAM_MUX_SHIFT                                                    3
#define CFG_SRAM_MUX_MASK                                            0x00000078
#define CFG_SRAM_MUX_RD(src)                          (((src) & 0x00000078)>>3)
#define CFG_SRAM_MUX_WR(src)                     (((u32)(src)<<3) & 0x00000078)
#define CFG_SRAM_MUX_SET(dst,src) \
                       (((dst) & ~0x00000078) | (((u32)(src)<<3) & 0x00000078))
/*	 Fields CFG_ENABLE_SDIO	 */
#define CFG_ENABLE_SDIO_WIDTH                                                 1
#define CFG_ENABLE_SDIO_SHIFT                                                 2
#define CFG_ENABLE_SDIO_MASK                                         0x00000004
#define CFG_ENABLE_SDIO_RD(src)                       (((src) & 0x00000004)>>2)
#define CFG_ENABLE_SDIO_WR(src)                  (((u32)(src)<<2) & 0x00000004)
#define CFG_ENABLE_SDIO_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields CFG_FLASH_DISABLE	 */
#define CFG_FLASH_DISABLE_WIDTH                                               1
#define CFG_FLASH_DISABLE_SHIFT                                               1
#define CFG_FLASH_DISABLE_MASK                                       0x00000002
#define CFG_FLASH_DISABLE_RD(src)                     (((src) & 0x00000002)>>1)
#define CFG_FLASH_DISABLE_WR(src)                (((u32)(src)<<1) & 0x00000002)
#define CFG_FLASH_DISABLE_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields ENABLE_TDM	 */
#define ENABLE_TDM_WIDTH                                                      1
#define ENABLE_TDM_SHIFT                                                      0
#define ENABLE_TDM_MASK                                              0x00000001
#define ENABLE_TDM_RD(src)                               (((src) & 0x00000001))
#define ENABLE_TDM_WR(src)                          (((u32)(src)) & 0x00000001)
#define ENABLE_TDM_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register USBH_DESCRAM	*/ 
/*	 Fields TEST1A	 */
#define TEST1A_WIDTH                                                          1
#define TEST1A_SHIFT                                                          6
#define TEST1A_MASK                                                  0x00000040
#define TEST1A_RD(src)                                (((src) & 0x00000040)>>6)
#define TEST1A_WR(src)                           (((u32)(src)<<6) & 0x00000040)
#define TEST1A_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*	 Fields RMEA	 */
#define RMEA_WIDTH                                                            1
#define RMEA_SHIFT                                                            5
#define RMEA_MASK                                                    0x00000020
#define RMEA_RD(src)                                  (((src) & 0x00000020)>>5)
#define RMEA_WR(src)                             (((u32)(src)<<5) & 0x00000020)
#define RMEA_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields RMA	 */
#define RMA_WIDTH                                                             4
#define RMA_SHIFT                                                             1
#define RMA_MASK                                                     0x0000001e
#define RMA_RD(src)                                   (((src) & 0x0000001e)>>1)
#define RMA_WR(src)                              (((u32)(src)<<1) & 0x0000001e)
#define RMA_SET(dst,src) \
                       (((dst) & ~0x0000001e) | (((u32)(src)<<1) & 0x0000001e))
/*	 Fields PWRDN_DIS	 */
#define PWRDN_DIS_WIDTH                                                       1
#define PWRDN_DIS_SHIFT                                                       0
#define PWRDN_DIS_MASK                                               0x00000001
#define PWRDN_DIS_RD(src)                                (((src) & 0x00000001))
#define PWRDN_DIS_WR(src)                           (((u32)(src)) & 0x00000001)
#define PWRDN_DIS_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register USBH_DATARAM	*/ 
/*	 Fields TEST1A	 */
#define TEST1A_F1_WIDTH                                                       1
#define TEST1A_F1_SHIFT                                                       6
#define TEST1A_F1_MASK                                               0x00000040
#define TEST1A_F1_RD(src)                             (((src) & 0x00000040)>>6)
#define TEST1A_F1_WR(src)                        (((u32)(src)<<6) & 0x00000040)
#define TEST1A_F1_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*	 Fields RMEA	 */
#define RMEA_F1_WIDTH                                                         1
#define RMEA_F1_SHIFT                                                         5
#define RMEA_F1_MASK                                                 0x00000020
#define RMEA_F1_RD(src)                               (((src) & 0x00000020)>>5)
#define RMEA_F1_WR(src)                          (((u32)(src)<<5) & 0x00000020)
#define RMEA_F1_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields RMA	 */
#define RMA_F1_WIDTH                                                          4
#define RMA_F1_SHIFT                                                          1
#define RMA_F1_MASK                                                  0x0000001e
#define RMA_F1_RD(src)                                (((src) & 0x0000001e)>>1)
#define RMA_F1_WR(src)                           (((u32)(src)<<1) & 0x0000001e)
#define RMA_F1_SET(dst,src) \
                       (((dst) & ~0x0000001e) | (((u32)(src)<<1) & 0x0000001e))
/*	 Fields PWRDN_DIS	 */
#define PWRDN_DIS_F1_WIDTH                                                    1
#define PWRDN_DIS_F1_SHIFT                                                    0
#define PWRDN_DIS_F1_MASK                                            0x00000001
#define PWRDN_DIS_F1_RD(src)                             (((src) & 0x00000001))
#define PWRDN_DIS_F1_WR(src)                        (((u32)(src)) & 0x00000001)
#define PWRDN_DIS_F1_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register OTG_DATARAM	*/ 
/*	 Fields TEST1A	 */
#define TEST1A_F2_WIDTH                                                       1
#define TEST1A_F2_SHIFT                                                       6
#define TEST1A_F2_MASK                                               0x00000040
#define TEST1A_F2_RD(src)                             (((src) & 0x00000040)>>6)
#define TEST1A_F2_WR(src)                        (((u32)(src)<<6) & 0x00000040)
#define TEST1A_F2_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*	 Fields RMEA	 */
#define RMEA_F2_WIDTH                                                         1
#define RMEA_F2_SHIFT                                                         5
#define RMEA_F2_MASK                                                 0x00000020
#define RMEA_F2_RD(src)                               (((src) & 0x00000020)>>5)
#define RMEA_F2_WR(src)                          (((u32)(src)<<5) & 0x00000020)
#define RMEA_F2_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields RMA	 */
#define RMA_F2_WIDTH                                                          4
#define RMA_F2_SHIFT                                                          1
#define RMA_F2_MASK                                                  0x0000001e
#define RMA_F2_RD(src)                                (((src) & 0x0000001e)>>1)
#define RMA_F2_WR(src)                           (((u32)(src)<<1) & 0x0000001e)
#define RMA_F2_SET(dst,src) \
                       (((dst) & ~0x0000001e) | (((u32)(src)<<1) & 0x0000001e))
/*	 Fields PWRDN_DIS	 */
#define PWRDN_DIS_F2_WIDTH                                                    1
#define PWRDN_DIS_F2_SHIFT                                                    0
#define PWRDN_DIS_F2_MASK                                            0x00000001
#define PWRDN_DIS_F2_RD(src)                             (((src) & 0x00000001))
#define PWRDN_DIS_F2_WR(src)                        (((u32)(src)) & 0x00000001)
#define PWRDN_DIS_F2_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register SDHC_DATARAM	*/ 
/*	 Fields TEST1B	 */
#define TEST1B_WIDTH                                                          1
#define TEST1B_SHIFT                                                         12
#define TEST1B_MASK                                                  0x00001000
#define TEST1B_RD(src)                               (((src) & 0x00001000)>>12)
#define TEST1B_WR(src)                          (((u32)(src)<<12) & 0x00001000)
#define TEST1B_SET(dst,src) \
                      (((dst) & ~0x00001000) | (((u32)(src)<<12) & 0x00001000))
/*	 Fields TEST1A	 */
#define TEST1A_F3_WIDTH                                                       1
#define TEST1A_F3_SHIFT                                                      11
#define TEST1A_F3_MASK                                               0x00000800
#define TEST1A_F3_RD(src)                            (((src) & 0x00000800)>>11)
#define TEST1A_F3_WR(src)                       (((u32)(src)<<11) & 0x00000800)
#define TEST1A_F3_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*	 Fields RMEB	 */
#define RMEB_WIDTH                                                            1
#define RMEB_SHIFT                                                           10
#define RMEB_MASK                                                    0x00000400
#define RMEB_RD(src)                                 (((src) & 0x00000400)>>10)
#define RMEB_WR(src)                            (((u32)(src)<<10) & 0x00000400)
#define RMEB_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields RMEA	 */
#define RMEA_F3_WIDTH                                                         1
#define RMEA_F3_SHIFT                                                         9
#define RMEA_F3_MASK                                                 0x00000200
#define RMEA_F3_RD(src)                               (((src) & 0x00000200)>>9)
#define RMEA_F3_WR(src)                          (((u32)(src)<<9) & 0x00000200)
#define RMEA_F3_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields RMB	 */
#define RMB_WIDTH                                                             4
#define RMB_SHIFT                                                             5
#define RMB_MASK                                                     0x000001e0
#define RMB_RD(src)                                   (((src) & 0x000001e0)>>5)
#define RMB_WR(src)                              (((u32)(src)<<5) & 0x000001e0)
#define RMB_SET(dst,src) \
                       (((dst) & ~0x000001e0) | (((u32)(src)<<5) & 0x000001e0))
/*	 Fields RMA	 */
#define RMA_F3_WIDTH                                                          4
#define RMA_F3_SHIFT                                                          1
#define RMA_F3_MASK                                                  0x0000001e
#define RMA_F3_RD(src)                                (((src) & 0x0000001e)>>1)
#define RMA_F3_WR(src)                           (((u32)(src)<<1) & 0x0000001e)
#define RMA_F3_SET(dst,src) \
                       (((dst) & ~0x0000001e) | (((u32)(src)<<1) & 0x0000001e))
/*	 Fields PWRDN_DIS	 */
#define PWRDN_DIS_F3_WIDTH                                                    1
#define PWRDN_DIS_F3_SHIFT                                                    0
#define PWRDN_DIS_F3_MASK                                            0x00000001
#define PWRDN_DIS_F3_RD(src)                             (((src) & 0x00000001))
#define PWRDN_DIS_F3_WR(src)                        (((u32)(src)) & 0x00000001)
#define PWRDN_DIS_F3_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register Flash_TxDATARAM	*/ 
/*	 Fields TEST1B	 */
#define TEST1B_F1_WIDTH                                                       1
#define TEST1B_F1_SHIFT                                                      12
#define TEST1B_F1_MASK                                               0x00001000
#define TEST1B_F1_RD(src)                            (((src) & 0x00001000)>>12)
#define TEST1B_F1_WR(src)                       (((u32)(src)<<12) & 0x00001000)
#define TEST1B_F1_SET(dst,src) \
                      (((dst) & ~0x00001000) | (((u32)(src)<<12) & 0x00001000))
/*	 Fields TEST1A	 */
#define TEST1A_F4_WIDTH                                                       1
#define TEST1A_F4_SHIFT                                                      11
#define TEST1A_F4_MASK                                               0x00000800
#define TEST1A_F4_RD(src)                            (((src) & 0x00000800)>>11)
#define TEST1A_F4_WR(src)                       (((u32)(src)<<11) & 0x00000800)
#define TEST1A_F4_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*	 Fields RMEB	 */
#define RMEB_F1_WIDTH                                                         1
#define RMEB_F1_SHIFT                                                        10
#define RMEB_F1_MASK                                                 0x00000400
#define RMEB_F1_RD(src)                              (((src) & 0x00000400)>>10)
#define RMEB_F1_WR(src)                         (((u32)(src)<<10) & 0x00000400)
#define RMEB_F1_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields RMEA	 */
#define RMEA_F4_WIDTH                                                         1
#define RMEA_F4_SHIFT                                                         9
#define RMEA_F4_MASK                                                 0x00000200
#define RMEA_F4_RD(src)                               (((src) & 0x00000200)>>9)
#define RMEA_F4_WR(src)                          (((u32)(src)<<9) & 0x00000200)
#define RMEA_F4_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields RMB	 */
#define RMB_F1_WIDTH                                                          4
#define RMB_F1_SHIFT                                                          5
#define RMB_F1_MASK                                                  0x000001e0
#define RMB_F1_RD(src)                                (((src) & 0x000001e0)>>5)
#define RMB_F1_WR(src)                           (((u32)(src)<<5) & 0x000001e0)
#define RMB_F1_SET(dst,src) \
                       (((dst) & ~0x000001e0) | (((u32)(src)<<5) & 0x000001e0))
/*	 Fields RMA	 */
#define RMA_F4_WIDTH                                                          4
#define RMA_F4_SHIFT                                                          1
#define RMA_F4_MASK                                                  0x0000001e
#define RMA_F4_RD(src)                                (((src) & 0x0000001e)>>1)
#define RMA_F4_WR(src)                           (((u32)(src)<<1) & 0x0000001e)
#define RMA_F4_SET(dst,src) \
                       (((dst) & ~0x0000001e) | (((u32)(src)<<1) & 0x0000001e))
/*	 Fields PWRDN_DIS	 */
#define PWRDN_DIS_F4_WIDTH                                                    1
#define PWRDN_DIS_F4_SHIFT                                                    0
#define PWRDN_DIS_F4_MASK                                            0x00000001
#define PWRDN_DIS_F4_RD(src)                             (((src) & 0x00000001))
#define PWRDN_DIS_F4_WR(src)                        (((u32)(src)) & 0x00000001)
#define PWRDN_DIS_F4_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register Flash_RxDATARAM	*/ 
/*	 Fields TEST1B	 */
#define TEST1B_F2_WIDTH                                                       1
#define TEST1B_F2_SHIFT                                                      12
#define TEST1B_F2_MASK                                               0x00001000
#define TEST1B_F2_RD(src)                            (((src) & 0x00001000)>>12)
#define TEST1B_F2_WR(src)                       (((u32)(src)<<12) & 0x00001000)
#define TEST1B_F2_SET(dst,src) \
                      (((dst) & ~0x00001000) | (((u32)(src)<<12) & 0x00001000))
/*	 Fields TEST1A	 */
#define TEST1A_F5_WIDTH                                                       1
#define TEST1A_F5_SHIFT                                                      11
#define TEST1A_F5_MASK                                               0x00000800
#define TEST1A_F5_RD(src)                            (((src) & 0x00000800)>>11)
#define TEST1A_F5_WR(src)                       (((u32)(src)<<11) & 0x00000800)
#define TEST1A_F5_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*	 Fields RMEB	 */
#define RMEB_F2_WIDTH                                                         1
#define RMEB_F2_SHIFT                                                        10
#define RMEB_F2_MASK                                                 0x00000400
#define RMEB_F2_RD(src)                              (((src) & 0x00000400)>>10)
#define RMEB_F2_WR(src)                         (((u32)(src)<<10) & 0x00000400)
#define RMEB_F2_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields RMEA	 */
#define RMEA_F5_WIDTH                                                         1
#define RMEA_F5_SHIFT                                                         9
#define RMEA_F5_MASK                                                 0x00000200
#define RMEA_F5_RD(src)                               (((src) & 0x00000200)>>9)
#define RMEA_F5_WR(src)                          (((u32)(src)<<9) & 0x00000200)
#define RMEA_F5_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields RMB	 */
#define RMB_F2_WIDTH                                                          4
#define RMB_F2_SHIFT                                                          5
#define RMB_F2_MASK                                                  0x000001e0
#define RMB_F2_RD(src)                                (((src) & 0x000001e0)>>5)
#define RMB_F2_WR(src)                           (((u32)(src)<<5) & 0x000001e0)
#define RMB_F2_SET(dst,src) \
                       (((dst) & ~0x000001e0) | (((u32)(src)<<5) & 0x000001e0))
/*	 Fields RMA	 */
#define RMA_F5_WIDTH                                                          4
#define RMA_F5_SHIFT                                                          1
#define RMA_F5_MASK                                                  0x0000001e
#define RMA_F5_RD(src)                                (((src) & 0x0000001e)>>1)
#define RMA_F5_WR(src)                           (((u32)(src)<<1) & 0x0000001e)
#define RMA_F5_SET(dst,src) \
                       (((dst) & ~0x0000001e) | (((u32)(src)<<1) & 0x0000001e))
/*	 Fields PWRDN_DIS	 */
#define PWRDN_DIS_F5_WIDTH                                                    1
#define PWRDN_DIS_F5_SHIFT                                                    0
#define PWRDN_DIS_F5_MASK                                            0x00000001
#define PWRDN_DIS_F5_RD(src)                             (((src) & 0x00000001))
#define PWRDN_DIS_F5_WR(src)                        (((u32)(src)) & 0x00000001)
#define PWRDN_DIS_F5_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register USB2_TUNE	*/ 
/*	 Fields USB0_TXVREFTUNE	 */
#define USB0_TXVREFTUNE_F2_WIDTH                                              4
#define USB0_TXVREFTUNE_F2_SHIFT                                             21
#define USB0_TXVREFTUNE_F2_MASK                                      0x01e00000
#define USB0_TXVREFTUNE_F2_RD(src)                   (((src) & 0x01e00000)>>21)
#define USB0_TXVREFTUNE_F2_WR(src)              (((u32)(src)<<21) & 0x01e00000)
#define USB0_TXVREFTUNE_F2_SET(dst,src) \
                      (((dst) & ~0x01e00000) | (((u32)(src)<<21) & 0x01e00000))
/*	 Fields USB0_TXFSLSTUNE	 */
#define USB0_TXFSLSTUNE_F2_WIDTH                                              4
#define USB0_TXFSLSTUNE_F2_SHIFT                                             17
#define USB0_TXFSLSTUNE_F2_MASK                                      0x001e0000
#define USB0_TXFSLSTUNE_F2_RD(src)                   (((src) & 0x001e0000)>>17)
#define USB0_TXFSLSTUNE_F2_WR(src)              (((u32)(src)<<17) & 0x001e0000)
#define USB0_TXFSLSTUNE_F2_SET(dst,src) \
                      (((dst) & ~0x001e0000) | (((u32)(src)<<17) & 0x001e0000))
/*	 Fields USB0_TXPREEMPHASISTUNE	 */
#define USB0_TXPREEMPHASISTUNE_F2_WIDTH                                       2
#define USB0_TXPREEMPHASISTUNE_F2_SHIFT                                      15
#define USB0_TXPREEMPHASISTUNE_F2_MASK                               0x00018000
#define USB0_TXPREEMPHASISTUNE_F2_RD(src)            (((src) & 0x00018000)>>15)
#define USB0_TXPREEMPHASISTUNE_F2_WR(src)       (((u32)(src)<<15) & 0x00018000)
#define USB0_TXPREEMPHASISTUNE_F2_SET(dst,src) \
                      (((dst) & ~0x00018000) | (((u32)(src)<<15) & 0x00018000))
/*	 Fields USB0_TXRISETUNE	 */
#define USB0_TXRISETUNE_F2_WIDTH                                              1
#define USB0_TXRISETUNE_F2_SHIFT                                             14
#define USB0_TXRISETUNE_F2_MASK                                      0x00004000
#define USB0_TXRISETUNE_F2_RD(src)                   (((src) & 0x00004000)>>14)
#define USB0_TXRISETUNE_F2_WR(src)              (((u32)(src)<<14) & 0x00004000)
#define USB0_TXRISETUNE_F2_SET(dst,src) \
                      (((dst) & ~0x00004000) | (((u32)(src)<<14) & 0x00004000))
/*	 Fields USB0_TXHSXVTUNE	 */
#define USB0_TXHSXVTUNE_F2_WIDTH                                              2
#define USB0_TXHSXVTUNE_F2_SHIFT                                             12
#define USB0_TXHSXVTUNE_F2_MASK                                      0x00003000
#define USB0_TXHSXVTUNE_F2_RD(src)                   (((src) & 0x00003000)>>12)
#define USB0_TXHSXVTUNE_F2_WR(src)              (((u32)(src)<<12) & 0x00003000)
#define USB0_TXHSXVTUNE_F2_SET(dst,src) \
                      (((dst) & ~0x00003000) | (((u32)(src)<<12) & 0x00003000))
/*	 Fields USB0_COMPDISTUNE	 */
#define USB0_COMPDISTUNE_F2_WIDTH                                             3
#define USB0_COMPDISTUNE_F2_SHIFT                                             8
#define USB0_COMPDISTUNE_F2_MASK                                     0x00000700
#define USB0_COMPDISTUNE_F2_RD(src)                   (((src) & 0x00000700)>>8)
#define USB0_COMPDISTUNE_F2_WR(src)              (((u32)(src)<<8) & 0x00000700)
#define USB0_COMPDISTUNE_F2_SET(dst,src) \
                       (((dst) & ~0x00000700) | (((u32)(src)<<8) & 0x00000700))
/*	 Fields USB0_SQRXTUNE	 */
#define USB0_SQRXTUNE_F2_WIDTH                                                3
#define USB0_SQRXTUNE_F2_SHIFT                                                4
#define USB0_SQRXTUNE_F2_MASK                                        0x00000070
#define USB0_SQRXTUNE_F2_RD(src)                      (((src) & 0x00000070)>>4)
#define USB0_SQRXTUNE_F2_WR(src)                 (((u32)(src)<<4) & 0x00000070)
#define USB0_SQRXTUNE_F2_SET(dst,src) \
                       (((dst) & ~0x00000070) | (((u32)(src)<<4) & 0x00000070))
/*	 Fields USB0_OTGTUNE	 */
#define USB0_OTGTUNE_F2_WIDTH                                                 3
#define USB0_OTGTUNE_F2_SHIFT                                                 0
#define USB0_OTGTUNE_F2_MASK                                         0x00000007
#define USB0_OTGTUNE_F2_RD(src)                          (((src) & 0x00000007))
#define USB0_OTGTUNE_F2_WR(src)                     (((u32)(src)) & 0x00000007)
#define USB0_OTGTUNE_F2_SET(dst,src) \
                          (((dst) & ~0x00000007) | (((u32)(src)) & 0x00000007))

/*	Register USB2_RSTCTL	*/ 
/*	 Fields USB0_OTGDISABLE	 */
#define USB0_OTGDISABLE_F2_WIDTH                                              1
#define USB0_OTGDISABLE_F2_SHIFT                                              2
#define USB0_OTGDISABLE_F2_MASK                                      0x00000004
#define USB0_OTGDISABLE_F2_RD(src)                    (((src) & 0x00000004)>>2)
#define USB0_OTGDISABLE_F2_WR(src)               (((u32)(src)<<2) & 0x00000004)
#define USB0_OTGDISABLE_F2_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields USB0_PORTRESET	 */
#define USB0_PORTRESET_F2_WIDTH                                               1
#define USB0_PORTRESET_F2_SHIFT                                               1
#define USB0_PORTRESET_F2_MASK                                       0x00000002
#define USB0_PORTRESET_F2_RD(src)                     (((src) & 0x00000002)>>1)
#define USB0_PORTRESET_F2_WR(src)                (((u32)(src)<<1) & 0x00000002)
#define USB0_PORTRESET_F2_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields USB0_POR	 */
#define USB0_POR_F2_WIDTH                                                     1
#define USB0_POR_F2_SHIFT                                                     0
#define USB0_POR_F2_MASK                                             0x00000001
#define USB0_POR_F2_RD(src)                              (((src) & 0x00000001))
#define USB0_POR_F2_WR(src)                         (((u32)(src)) & 0x00000001)
#define USB0_POR_F2_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register USB2_CLKCTL	*/ 
/*	 Fields USB0_SIM_SCALEDOWN	 */
#define USB0_SIM_SCALEDOWN_F2_WIDTH                                           2
#define USB0_SIM_SCALEDOWN_F2_SHIFT                                          30
#define USB0_SIM_SCALEDOWN_F2_MASK                                   0xc0000000
#define USB0_SIM_SCALEDOWN_F2_RD(src)                (((src) & 0xc0000000)>>30)
#define USB0_SIM_SCALEDOWN_F2_WR(src)           (((u32)(src)<<30) & 0xc0000000)
#define USB0_SIM_SCALEDOWN_F2_SET(dst,src) \
                      (((dst) & ~0xc0000000) | (((u32)(src)<<30) & 0xc0000000))
/*	 Fields USB0_DRVVBUS_POLARITY	 */
#define USB0_DRVVBUS_POLARITY_F2_WIDTH                                        1
#define USB0_DRVVBUS_POLARITY_F2_SHIFT                                        7
#define USB0_DRVVBUS_POLARITY_F2_MASK                                0x00000080
#define USB0_DRVVBUS_POLARITY_F2_RD(src)              (((src) & 0x00000080)>>7)
#define USB0_DRVVBUS_POLARITY_F2_WR(src)         (((u32)(src)<<7) & 0x00000080)
#define USB0_DRVVBUS_POLARITY_F2_SET(dst,src) \
                       (((dst) & ~0x00000080) | (((u32)(src)<<7) & 0x00000080))
/*	 Fields USB0_REFCLKSEL	 */
#define USB0_REFCLKSEL_F2_WIDTH                                               2
#define USB0_REFCLKSEL_F2_SHIFT                                               2
#define USB0_REFCLKSEL_F2_MASK                                       0x0000000c
#define USB0_REFCLKSEL_F2_RD(src)                     (((src) & 0x0000000c)>>2)
#define USB0_REFCLKSEL_F2_WR(src)                (((u32)(src)<<2) & 0x0000000c)
#define USB0_REFCLKSEL_F2_SET(dst,src) \
                       (((dst) & ~0x0000000c) | (((u32)(src)<<2) & 0x0000000c))
/*	 Fields USB0_REFCLKDIV	 */
#define USB0_REFCLKDIV_F2_WIDTH                                               2
#define USB0_REFCLKDIV_F2_SHIFT                                               0
#define USB0_REFCLKDIV_F2_MASK                                       0x00000003
#define USB0_REFCLKDIV_F2_RD(src)                        (((src) & 0x00000003))
#define USB0_REFCLKDIV_F2_WR(src)                   (((u32)(src)) & 0x00000003)
#define USB0_REFCLKDIV_F2_SET(dst,src) \
                          (((dst) & ~0x00000003) | (((u32)(src)) & 0x00000003))

/*	Register USBHost1_Cfg	*/ 
/*	 Fields usbh_word_if	 */
#define USBH_WORD_IF_F1_WIDTH                                                 1
#define USBH_WORD_IF_F1_SHIFT                                                12
#define USBH_WORD_IF_F1_MASK                                         0x00001000
#define USBH_WORD_IF_F1_RD(src)                      (((src) & 0x00001000)>>12)
#define USBH_WORD_IF_F1_WR(src)                 (((u32)(src)<<12) & 0x00001000)
#define USBH_WORD_IF_F1_SET(dst,src) \
                      (((dst) & ~0x00001000) | (((u32)(src)<<12) & 0x00001000))
/*	 Fields usbh_ohci_susp_lgcy_i	 */
#define USBH_OHCI_SUSP_LGCY_I_F1_WIDTH                                        1
#define USBH_OHCI_SUSP_LGCY_I_F1_SHIFT                                       11
#define USBH_OHCI_SUSP_LGCY_I_F1_MASK                                0x00000800
#define USBH_OHCI_SUSP_LGCY_I_F1_RD(src)             (((src) & 0x00000800)>>11)
#define USBH_OHCI_SUSP_LGCY_I_F1_WR(src)        (((u32)(src)<<11) & 0x00000800)
#define USBH_OHCI_SUSP_LGCY_I_F1_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*	 Fields usbh_app_start_clk_i	 */
#define USBH_APP_START_CLK_I_F1_WIDTH                                         1
#define USBH_APP_START_CLK_I_F1_SHIFT                                        10
#define USBH_APP_START_CLK_I_F1_MASK                                 0x00000400
#define USBH_APP_START_CLK_I_F1_RD(src)              (((src) & 0x00000400)>>10)
#define USBH_APP_START_CLK_I_F1_WR(src)         (((u32)(src)<<10) & 0x00000400)
#define USBH_APP_START_CLK_I_F1_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields usbh_autoppd	 */
#define USBH_AUTOPPD_F1_WIDTH                                                 1
#define USBH_AUTOPPD_F1_SHIFT                                                 9
#define USBH_AUTOPPD_F1_MASK                                         0x00000200
#define USBH_AUTOPPD_F1_RD(src)                       (((src) & 0x00000200)>>9)
#define USBH_AUTOPPD_F1_WR(src)                  (((u32)(src)<<9) & 0x00000200)
#define USBH_AUTOPPD_F1_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields usbh_frame_len	 */
#define USBH_FRAME_LEN_F1_WIDTH                                               6
#define USBH_FRAME_LEN_F1_SHIFT                                               3
#define USBH_FRAME_LEN_F1_MASK                                       0x000001f8
#define USBH_FRAME_LEN_F1_RD(src)                     (((src) & 0x000001f8)>>3)
#define USBH_FRAME_LEN_F1_WR(src)                (((u32)(src)<<3) & 0x000001f8)
#define USBH_FRAME_LEN_F1_SET(dst,src) \
                       (((dst) & ~0x000001f8) | (((u32)(src)<<3) & 0x000001f8))
/*	 Fields usbh_sim_mode	 */
#define USBH_SIM_MODE_F1_WIDTH                                                1
#define USBH_SIM_MODE_F1_SHIFT                                                2
#define USBH_SIM_MODE_F1_MASK                                        0x00000004
#define USBH_SIM_MODE_F1_RD(src)                      (((src) & 0x00000004)>>2)
#define USBH_SIM_MODE_F1_WR(src)                 (((u32)(src)<<2) & 0x00000004)
#define USBH_SIM_MODE_F1_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields ss_resume_utmi_pls_dis	 */
#define SS_RESUME_UTMI_PLS_DIS_F1_WIDTH                                       1
#define SS_RESUME_UTMI_PLS_DIS_F1_SHIFT                                       1
#define SS_RESUME_UTMI_PLS_DIS_F1_MASK                               0x00000002
#define SS_RESUME_UTMI_PLS_DIS_F1_RD(src)             (((src) & 0x00000002)>>1)
#define SS_RESUME_UTMI_PLS_DIS_F1_WR(src)        (((u32)(src)<<1) & 0x00000002)
#define SS_RESUME_UTMI_PLS_DIS_F1_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields ss_utmi_backward_enb	 */
#define SS_UTMI_BACKWARD_ENB_F1_WIDTH                                         1
#define SS_UTMI_BACKWARD_ENB_F1_SHIFT                                         0
#define SS_UTMI_BACKWARD_ENB_F1_MASK                                 0x00000001
#define SS_UTMI_BACKWARD_ENB_F1_RD(src)                  (((src) & 0x00000001))
#define SS_UTMI_BACKWARD_ENB_F1_WR(src)             (((u32)(src)) & 0x00000001)
#define SS_UTMI_BACKWARD_ENB_F1_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register USBHost1_Status	*/ 
/*	 Fields ohci_rmtwkp_status	 */
#define OHCI_RMTWKP_STATUS_F1_WIDTH                                           1
#define OHCI_RMTWKP_STATUS_F1_SHIFT                                           0
#define OHCI_RMTWKP_STATUS_F1_MASK                                   0x00000001
#define OHCI_RMTWKP_STATUS_F1_RD(src)                    (((src) & 0x00000001))
#define OHCI_RMTWKP_STATUS_F1_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register USBHost1_Fatal_CFG	*/ 
/*	 Fields CFG_usbh_sys_intr	 */
#define CFG_USBH_SYS_INTR_F1_WIDTH                                            1
#define CFG_USBH_SYS_INTR_F1_SHIFT                                            0
#define CFG_USBH_SYS_INTR_F1_MASK                                    0x00000001
#define CFG_USBH_SYS_INTR_F1_RD(src)                     (((src) & 0x00000001))
#define CFG_USBH_SYS_INTR_F1_WR(src)                (((u32)(src)) & 0x00000001)
#define CFG_USBH_SYS_INTR_F1_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register USBH1_DESCRAM	*/ 
/*	 Fields TEST1A	 */
#define TEST1A_F6_WIDTH                                                       1
#define TEST1A_F6_SHIFT                                                       6
#define TEST1A_F6_MASK                                               0x00000040
#define TEST1A_F6_RD(src)                             (((src) & 0x00000040)>>6)
#define TEST1A_F6_WR(src)                        (((u32)(src)<<6) & 0x00000040)
#define TEST1A_F6_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*	 Fields RMEA	 */
#define RMEA_F6_WIDTH                                                         1
#define RMEA_F6_SHIFT                                                         5
#define RMEA_F6_MASK                                                 0x00000020
#define RMEA_F6_RD(src)                               (((src) & 0x00000020)>>5)
#define RMEA_F6_WR(src)                          (((u32)(src)<<5) & 0x00000020)
#define RMEA_F6_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields RMA	 */
#define RMA_F6_WIDTH                                                          4
#define RMA_F6_SHIFT                                                          1
#define RMA_F6_MASK                                                  0x0000001e
#define RMA_F6_RD(src)                                (((src) & 0x0000001e)>>1)
#define RMA_F6_WR(src)                           (((u32)(src)<<1) & 0x0000001e)
#define RMA_F6_SET(dst,src) \
                       (((dst) & ~0x0000001e) | (((u32)(src)<<1) & 0x0000001e))
/*	 Fields PWRDN_DIS	 */
#define PWRDN_DIS_F6_WIDTH                                                    1
#define PWRDN_DIS_F6_SHIFT                                                    0
#define PWRDN_DIS_F6_MASK                                            0x00000001
#define PWRDN_DIS_F6_RD(src)                             (((src) & 0x00000001))
#define PWRDN_DIS_F6_WR(src)                        (((u32)(src)) & 0x00000001)
#define PWRDN_DIS_F6_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register USBH1_DATARAM	*/ 
/*	 Fields TEST1A	 */
#define TEST1A_F7_WIDTH                                                       1
#define TEST1A_F7_SHIFT                                                       6
#define TEST1A_F7_MASK                                               0x00000040
#define TEST1A_F7_RD(src)                             (((src) & 0x00000040)>>6)
#define TEST1A_F7_WR(src)                        (((u32)(src)<<6) & 0x00000040)
#define TEST1A_F7_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*	 Fields RMEA	 */
#define RMEA_F7_WIDTH                                                         1
#define RMEA_F7_SHIFT                                                         5
#define RMEA_F7_MASK                                                 0x00000020
#define RMEA_F7_RD(src)                               (((src) & 0x00000020)>>5)
#define RMEA_F7_WR(src)                          (((u32)(src)<<5) & 0x00000020)
#define RMEA_F7_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields RMA	 */
#define RMA_F7_WIDTH                                                          4
#define RMA_F7_SHIFT                                                          1
#define RMA_F7_MASK                                                  0x0000001e
#define RMA_F7_RD(src)                                (((src) & 0x0000001e)>>1)
#define RMA_F7_WR(src)                           (((u32)(src)<<1) & 0x0000001e)
#define RMA_F7_SET(dst,src) \
                       (((dst) & ~0x0000001e) | (((u32)(src)<<1) & 0x0000001e))
/*	 Fields PWRDN_DIS	 */
#define PWRDN_DIS_F7_WIDTH                                                    1
#define PWRDN_DIS_F7_SHIFT                                                    0
#define PWRDN_DIS_F7_MASK                                            0x00000001
#define PWRDN_DIS_F7_RD(src)                             (((src) & 0x00000001))
#define PWRDN_DIS_F7_WR(src)                        (((u32)(src)) & 0x00000001)
#define PWRDN_DIS_F7_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register UART_MODE_SEL	*/ 
/*	 Fields CFG_UART_REFCLK	 */
#define CFG_UART_REFCLK_WIDTH                                                 1
#define CFG_UART_REFCLK_SHIFT                                                 5
#define CFG_UART_REFCLK_MASK                                         0x00000020
#define CFG_UART_REFCLK_RD(src)                       (((src) & 0x00000020)>>5)
#define CFG_UART_REFCLK_WR(src)                  (((u32)(src)<<5) & 0x00000020)
#define CFG_UART_REFCLK_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields CFG_CTS_PAIR_1	 */
#define CFG_CTS_PAIR_1_WIDTH                                                  1
#define CFG_CTS_PAIR_1_SHIFT                                                  4
#define CFG_CTS_PAIR_1_MASK                                          0x00000010
#define CFG_CTS_PAIR_1_RD(src)                        (((src) & 0x00000010)>>4)
#define CFG_CTS_PAIR_1_WR(src)                   (((u32)(src)<<4) & 0x00000010)
#define CFG_CTS_PAIR_1_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields CFG_CTS_PAIR_0	 */
#define CFG_CTS_PAIR_0_WIDTH                                                  1
#define CFG_CTS_PAIR_0_SHIFT                                                  3
#define CFG_CTS_PAIR_0_MASK                                          0x00000008
#define CFG_CTS_PAIR_0_RD(src)                        (((src) & 0x00000008)>>3)
#define CFG_CTS_PAIR_0_WR(src)                   (((u32)(src)<<3) & 0x00000008)
#define CFG_CTS_PAIR_0_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields CFG_ENABLE_4BIT_UART_1	 */
#define CFG_ENABLE_4BIT_UART_1_WIDTH                                          1
#define CFG_ENABLE_4BIT_UART_1_SHIFT                                          2
#define CFG_ENABLE_4BIT_UART_1_MASK                                  0x00000004
#define CFG_ENABLE_4BIT_UART_1_RD(src)                (((src) & 0x00000004)>>2)
#define CFG_ENABLE_4BIT_UART_1_WR(src)           (((u32)(src)<<2) & 0x00000004)
#define CFG_ENABLE_4BIT_UART_1_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields CFG_ENABLE_4BIT_UART_0	 */
#define CFG_ENABLE_4BIT_UART_0_WIDTH                                          1
#define CFG_ENABLE_4BIT_UART_0_SHIFT                                          1
#define CFG_ENABLE_4BIT_UART_0_MASK                                  0x00000002
#define CFG_ENABLE_4BIT_UART_0_RD(src)                (((src) & 0x00000002)>>1)
#define CFG_ENABLE_4BIT_UART_0_WR(src)           (((u32)(src)<<1) & 0x00000002)
#define CFG_ENABLE_4BIT_UART_0_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields CFG_ENABLE_8BIT_UART_0	 */
#define CFG_ENABLE_8BIT_UART_0_WIDTH                                          1
#define CFG_ENABLE_8BIT_UART_0_SHIFT                                          0
#define CFG_ENABLE_8BIT_UART_0_MASK                                  0x00000001
#define CFG_ENABLE_8BIT_UART_0_RD(src)                   (((src) & 0x00000001))
#define CFG_ENABLE_8BIT_UART_0_WR(src)              (((u32)(src)) & 0x00000001)
#define CFG_ENABLE_8BIT_UART_0_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register ahb2axi_bid	*/ 
/*	 Fields revno	 */
#define REVNO_WIDTH                                                           2
#define REVNO_SHIFT                                                           8
#define REVNO_MASK                                                   0x00000300
#define REVNO_RD(src)                                 (((src) & 0x00000300)>>8)
#define REVNO_SET(dst,src) \
                       (((dst) & ~0x00000300) | (((u32)(src)<<8) & 0x00000300))
/*	 Fields busid	 */
#define BUSID_WIDTH                                                           3
#define BUSID_SHIFT                                                           5
#define BUSID_MASK                                                   0x000000e0
#define BUSID_RD(src)                                 (((src) & 0x000000e0)>>5)
#define BUSID_SET(dst,src) \
                       (((dst) & ~0x000000e0) | (((u32)(src)<<5) & 0x000000e0))
/*	 Fields deviceid	 */
#define DEVICEID_WIDTH                                                        5
#define DEVICEID_SHIFT                                                        0
#define DEVICEID_MASK                                                0x0000001f
#define DEVICEID_RD(src)                                 (((src) & 0x0000001f))
#define DEVICEID_SET(dst,src) \
                          (((dst) & ~0x0000001f) | (((u32)(src)) & 0x0000001f))

/*	Register tdm_bid	*/ 
/*	 Fields revno	 */
#define REVNO_F1_WIDTH                                                        2
#define REVNO_F1_SHIFT                                                        8
#define REVNO_F1_MASK                                                0x00000300
#define REVNO_F1_RD(src)                              (((src) & 0x00000300)>>8)
#define REVNO_F1_SET(dst,src) \
                       (((dst) & ~0x00000300) | (((u32)(src)<<8) & 0x00000300))
/*	 Fields busid	 */
#define BUSID_F1_WIDTH                                                        3
#define BUSID_F1_SHIFT                                                        5
#define BUSID_F1_MASK                                                0x000000e0
#define BUSID_F1_RD(src)                              (((src) & 0x000000e0)>>5)
#define BUSID_F1_SET(dst,src) \
                       (((dst) & ~0x000000e0) | (((u32)(src)<<5) & 0x000000e0))
/*	 Fields deviceid	 */
#define DEVICEID_F1_WIDTH                                                     5
#define DEVICEID_F1_SHIFT                                                     0
#define DEVICEID_F1_MASK                                             0x0000001f
#define DEVICEID_F1_RD(src)                              (((src) & 0x0000001f))
#define DEVICEID_F1_SET(dst,src) \
                          (((dst) & ~0x0000001f) | (((u32)(src)) & 0x0000001f))

/*	Register ahbc_parity	*/ 
/*	 Fields RAM_PARITY	 */
#define RAM_PARITY_WIDTH                                                     32
#define RAM_PARITY_SHIFT                                                      0
#define RAM_PARITY_MASK                                              0x00000001
#define RAM_PARITY_RD(src)                               (((src) & 0x00000001))
#define RAM_PARITY_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register ahbc_INT	*/ 
/*	 Fields usbh2_wr_decerr_reported	 */
#define USBH2_WR_DECERR_REPORTED_WIDTH                                        1
#define USBH2_WR_DECERR_REPORTED_SHIFT                                       27
#define USBH2_WR_DECERR_REPORTED_MASK                                0x08000000
#define USBH2_WR_DECERR_REPORTED_RD(src)             (((src) & 0x08000000)>>27)
#define USBH2_WR_DECERR_REPORTED_WR(src)        (((u32)(src)<<27) & 0x08000000)
#define USBH2_WR_DECERR_REPORTED_SET(dst,src) \
                      (((dst) & ~0x08000000) | (((u32)(src)<<27) & 0x08000000))
/*	 Fields usbh2_wr_slverr_reported	 */
#define USBH2_WR_SLVERR_REPORTED_WIDTH                                        1
#define USBH2_WR_SLVERR_REPORTED_SHIFT                                       26
#define USBH2_WR_SLVERR_REPORTED_MASK                                0x04000000
#define USBH2_WR_SLVERR_REPORTED_RD(src)             (((src) & 0x04000000)>>26)
#define USBH2_WR_SLVERR_REPORTED_WR(src)        (((u32)(src)<<26) & 0x04000000)
#define USBH2_WR_SLVERR_REPORTED_SET(dst,src) \
                      (((dst) & ~0x04000000) | (((u32)(src)<<26) & 0x04000000))
/*	 Fields usbh2_rd_decerr_reported	 */
#define USBH2_RD_DECERR_REPORTED_WIDTH                                        1
#define USBH2_RD_DECERR_REPORTED_SHIFT                                       25
#define USBH2_RD_DECERR_REPORTED_MASK                                0x02000000
#define USBH2_RD_DECERR_REPORTED_RD(src)             (((src) & 0x02000000)>>25)
#define USBH2_RD_DECERR_REPORTED_WR(src)        (((u32)(src)<<25) & 0x02000000)
#define USBH2_RD_DECERR_REPORTED_SET(dst,src) \
                      (((dst) & ~0x02000000) | (((u32)(src)<<25) & 0x02000000))
/*	 Fields usbh2_rd_slverr_reported	 */
#define USBH2_RD_SLVERR_REPORTED_WIDTH                                        1
#define USBH2_RD_SLVERR_REPORTED_SHIFT                                       24
#define USBH2_RD_SLVERR_REPORTED_MASK                                0x01000000
#define USBH2_RD_SLVERR_REPORTED_RD(src)             (((src) & 0x01000000)>>24)
#define USBH2_RD_SLVERR_REPORTED_WR(src)        (((u32)(src)<<24) & 0x01000000)
#define USBH2_RD_SLVERR_REPORTED_SET(dst,src) \
                      (((dst) & ~0x01000000) | (((u32)(src)<<24) & 0x01000000))
/*	 Fields usbh1_wr_decerr_reported	 */
#define USBH1_WR_DECERR_REPORTED_WIDTH                                        1
#define USBH1_WR_DECERR_REPORTED_SHIFT                                       23
#define USBH1_WR_DECERR_REPORTED_MASK                                0x00800000
#define USBH1_WR_DECERR_REPORTED_RD(src)             (((src) & 0x00800000)>>23)
#define USBH1_WR_DECERR_REPORTED_WR(src)        (((u32)(src)<<23) & 0x00800000)
#define USBH1_WR_DECERR_REPORTED_SET(dst,src) \
                      (((dst) & ~0x00800000) | (((u32)(src)<<23) & 0x00800000))
/*	 Fields usbh1_wr_slverr_reported	 */
#define USBH1_WR_SLVERR_REPORTED_WIDTH                                        1
#define USBH1_WR_SLVERR_REPORTED_SHIFT                                       22
#define USBH1_WR_SLVERR_REPORTED_MASK                                0x00400000
#define USBH1_WR_SLVERR_REPORTED_RD(src)             (((src) & 0x00400000)>>22)
#define USBH1_WR_SLVERR_REPORTED_WR(src)        (((u32)(src)<<22) & 0x00400000)
#define USBH1_WR_SLVERR_REPORTED_SET(dst,src) \
                      (((dst) & ~0x00400000) | (((u32)(src)<<22) & 0x00400000))
/*	 Fields usbh1_rd_decerr_reported	 */
#define USBH1_RD_DECERR_REPORTED_WIDTH                                        1
#define USBH1_RD_DECERR_REPORTED_SHIFT                                       21
#define USBH1_RD_DECERR_REPORTED_MASK                                0x00200000
#define USBH1_RD_DECERR_REPORTED_RD(src)             (((src) & 0x00200000)>>21)
#define USBH1_RD_DECERR_REPORTED_WR(src)        (((u32)(src)<<21) & 0x00200000)
#define USBH1_RD_DECERR_REPORTED_SET(dst,src) \
                      (((dst) & ~0x00200000) | (((u32)(src)<<21) & 0x00200000))
/*	 Fields usbh1_rd_slverr_reported	 */
#define USBH1_RD_SLVERR_REPORTED_WIDTH                                        1
#define USBH1_RD_SLVERR_REPORTED_SHIFT                                       20
#define USBH1_RD_SLVERR_REPORTED_MASK                                0x00100000
#define USBH1_RD_SLVERR_REPORTED_RD(src)             (((src) & 0x00100000)>>20)
#define USBH1_RD_SLVERR_REPORTED_WR(src)        (((u32)(src)<<20) & 0x00100000)
#define USBH1_RD_SLVERR_REPORTED_SET(dst,src) \
                      (((dst) & ~0x00100000) | (((u32)(src)<<20) & 0x00100000))
/*	 Fields tdm_wr_decerr_reported	 */
#define TDM_WR_DECERR_REPORTED_WIDTH                                          1
#define TDM_WR_DECERR_REPORTED_SHIFT                                         19
#define TDM_WR_DECERR_REPORTED_MASK                                  0x00080000
#define TDM_WR_DECERR_REPORTED_RD(src)               (((src) & 0x00080000)>>19)
#define TDM_WR_DECERR_REPORTED_WR(src)          (((u32)(src)<<19) & 0x00080000)
#define TDM_WR_DECERR_REPORTED_SET(dst,src) \
                      (((dst) & ~0x00080000) | (((u32)(src)<<19) & 0x00080000))
/*	 Fields tdm_wr_slverr_reported	 */
#define TDM_WR_SLVERR_REPORTED_WIDTH                                          1
#define TDM_WR_SLVERR_REPORTED_SHIFT                                         18
#define TDM_WR_SLVERR_REPORTED_MASK                                  0x00040000
#define TDM_WR_SLVERR_REPORTED_RD(src)               (((src) & 0x00040000)>>18)
#define TDM_WR_SLVERR_REPORTED_WR(src)          (((u32)(src)<<18) & 0x00040000)
#define TDM_WR_SLVERR_REPORTED_SET(dst,src) \
                      (((dst) & ~0x00040000) | (((u32)(src)<<18) & 0x00040000))
/*	 Fields tdm_rd_decerr_reported	 */
#define TDM_RD_DECERR_REPORTED_WIDTH                                          1
#define TDM_RD_DECERR_REPORTED_SHIFT                                         17
#define TDM_RD_DECERR_REPORTED_MASK                                  0x00020000
#define TDM_RD_DECERR_REPORTED_RD(src)               (((src) & 0x00020000)>>17)
#define TDM_RD_DECERR_REPORTED_WR(src)          (((u32)(src)<<17) & 0x00020000)
#define TDM_RD_DECERR_REPORTED_SET(dst,src) \
                      (((dst) & ~0x00020000) | (((u32)(src)<<17) & 0x00020000))
/*	 Fields tdm_rd_slverr_reported	 */
#define TDM_RD_SLVERR_REPORTED_WIDTH                                          1
#define TDM_RD_SLVERR_REPORTED_SHIFT                                         16
#define TDM_RD_SLVERR_REPORTED_MASK                                  0x00010000
#define TDM_RD_SLVERR_REPORTED_RD(src)               (((src) & 0x00010000)>>16)
#define TDM_RD_SLVERR_REPORTED_WR(src)          (((u32)(src)<<16) & 0x00010000)
#define TDM_RD_SLVERR_REPORTED_SET(dst,src) \
                      (((dst) & ~0x00010000) | (((u32)(src)<<16) & 0x00010000))
/*	 Fields usbh_wr_decerr_reported	 */
#define USBH_WR_DECERR_REPORTED_WIDTH                                         1
#define USBH_WR_DECERR_REPORTED_SHIFT                                        15
#define USBH_WR_DECERR_REPORTED_MASK                                 0x00008000
#define USBH_WR_DECERR_REPORTED_RD(src)              (((src) & 0x00008000)>>15)
#define USBH_WR_DECERR_REPORTED_WR(src)         (((u32)(src)<<15) & 0x00008000)
#define USBH_WR_DECERR_REPORTED_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))
/*	 Fields usbh_wr_slverr_reported	 */
#define USBH_WR_SLVERR_REPORTED_WIDTH                                         1
#define USBH_WR_SLVERR_REPORTED_SHIFT                                        14
#define USBH_WR_SLVERR_REPORTED_MASK                                 0x00004000
#define USBH_WR_SLVERR_REPORTED_RD(src)              (((src) & 0x00004000)>>14)
#define USBH_WR_SLVERR_REPORTED_WR(src)         (((u32)(src)<<14) & 0x00004000)
#define USBH_WR_SLVERR_REPORTED_SET(dst,src) \
                      (((dst) & ~0x00004000) | (((u32)(src)<<14) & 0x00004000))
/*	 Fields usbh_rd_decerr_reported	 */
#define USBH_RD_DECERR_REPORTED_WIDTH                                         1
#define USBH_RD_DECERR_REPORTED_SHIFT                                        13
#define USBH_RD_DECERR_REPORTED_MASK                                 0x00002000
#define USBH_RD_DECERR_REPORTED_RD(src)              (((src) & 0x00002000)>>13)
#define USBH_RD_DECERR_REPORTED_WR(src)         (((u32)(src)<<13) & 0x00002000)
#define USBH_RD_DECERR_REPORTED_SET(dst,src) \
                      (((dst) & ~0x00002000) | (((u32)(src)<<13) & 0x00002000))
/*	 Fields usbh_rd_slverr_reported	 */
#define USBH_RD_SLVERR_REPORTED_WIDTH                                         1
#define USBH_RD_SLVERR_REPORTED_SHIFT                                        12
#define USBH_RD_SLVERR_REPORTED_MASK                                 0x00001000
#define USBH_RD_SLVERR_REPORTED_RD(src)              (((src) & 0x00001000)>>12)
#define USBH_RD_SLVERR_REPORTED_WR(src)         (((u32)(src)<<12) & 0x00001000)
#define USBH_RD_SLVERR_REPORTED_SET(dst,src) \
                      (((dst) & ~0x00001000) | (((u32)(src)<<12) & 0x00001000))
/*	 Fields sdhc_wr_decerr_reported	 */
#define SDHC_WR_DECERR_REPORTED_WIDTH                                         1
#define SDHC_WR_DECERR_REPORTED_SHIFT                                        11
#define SDHC_WR_DECERR_REPORTED_MASK                                 0x00000800
#define SDHC_WR_DECERR_REPORTED_RD(src)              (((src) & 0x00000800)>>11)
#define SDHC_WR_DECERR_REPORTED_WR(src)         (((u32)(src)<<11) & 0x00000800)
#define SDHC_WR_DECERR_REPORTED_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*	 Fields sdhc_wr_slverr_reported	 */
#define SDHC_WR_SLVERR_REPORTED_WIDTH                                         1
#define SDHC_WR_SLVERR_REPORTED_SHIFT                                        10
#define SDHC_WR_SLVERR_REPORTED_MASK                                 0x00000400
#define SDHC_WR_SLVERR_REPORTED_RD(src)              (((src) & 0x00000400)>>10)
#define SDHC_WR_SLVERR_REPORTED_WR(src)         (((u32)(src)<<10) & 0x00000400)
#define SDHC_WR_SLVERR_REPORTED_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields sdhc_rd_decerr_reported	 */
#define SDHC_RD_DECERR_REPORTED_WIDTH                                         1
#define SDHC_RD_DECERR_REPORTED_SHIFT                                         9
#define SDHC_RD_DECERR_REPORTED_MASK                                 0x00000200
#define SDHC_RD_DECERR_REPORTED_RD(src)               (((src) & 0x00000200)>>9)
#define SDHC_RD_DECERR_REPORTED_WR(src)          (((u32)(src)<<9) & 0x00000200)
#define SDHC_RD_DECERR_REPORTED_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields sdhc_rd_slverr_reported	 */
#define SDHC_RD_SLVERR_REPORTED_WIDTH                                         1
#define SDHC_RD_SLVERR_REPORTED_SHIFT                                         8
#define SDHC_RD_SLVERR_REPORTED_MASK                                 0x00000100
#define SDHC_RD_SLVERR_REPORTED_RD(src)               (((src) & 0x00000100)>>8)
#define SDHC_RD_SLVERR_REPORTED_WR(src)          (((u32)(src)<<8) & 0x00000100)
#define SDHC_RD_SLVERR_REPORTED_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*	 Fields usb1_wr_decerr_reported	 */
#define USB1_WR_DECERR_REPORTED_WIDTH                                         1
#define USB1_WR_DECERR_REPORTED_SHIFT                                         7
#define USB1_WR_DECERR_REPORTED_MASK                                 0x00000080
#define USB1_WR_DECERR_REPORTED_RD(src)               (((src) & 0x00000080)>>7)
#define USB1_WR_DECERR_REPORTED_WR(src)          (((u32)(src)<<7) & 0x00000080)
#define USB1_WR_DECERR_REPORTED_SET(dst,src) \
                       (((dst) & ~0x00000080) | (((u32)(src)<<7) & 0x00000080))
/*	 Fields usb1_wr_slverr_reported	 */
#define USB1_WR_SLVERR_REPORTED_WIDTH                                         1
#define USB1_WR_SLVERR_REPORTED_SHIFT                                         6
#define USB1_WR_SLVERR_REPORTED_MASK                                 0x00000040
#define USB1_WR_SLVERR_REPORTED_RD(src)               (((src) & 0x00000040)>>6)
#define USB1_WR_SLVERR_REPORTED_WR(src)          (((u32)(src)<<6) & 0x00000040)
#define USB1_WR_SLVERR_REPORTED_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*	 Fields usb1_rd_decerr_reported	 */
#define USB1_RD_DECERR_REPORTED_WIDTH                                         1
#define USB1_RD_DECERR_REPORTED_SHIFT                                         5
#define USB1_RD_DECERR_REPORTED_MASK                                 0x00000020
#define USB1_RD_DECERR_REPORTED_RD(src)               (((src) & 0x00000020)>>5)
#define USB1_RD_DECERR_REPORTED_WR(src)          (((u32)(src)<<5) & 0x00000020)
#define USB1_RD_DECERR_REPORTED_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields usb1_rd_slverr_reported	 */
#define USB1_RD_SLVERR_REPORTED_WIDTH                                         1
#define USB1_RD_SLVERR_REPORTED_SHIFT                                         4
#define USB1_RD_SLVERR_REPORTED_MASK                                 0x00000010
#define USB1_RD_SLVERR_REPORTED_RD(src)               (((src) & 0x00000010)>>4)
#define USB1_RD_SLVERR_REPORTED_WR(src)          (((u32)(src)<<4) & 0x00000010)
#define USB1_RD_SLVERR_REPORTED_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields usb0_wr_decerr_reported	 */
#define USB0_WR_DECERR_REPORTED_WIDTH                                         1
#define USB0_WR_DECERR_REPORTED_SHIFT                                         3
#define USB0_WR_DECERR_REPORTED_MASK                                 0x00000008
#define USB0_WR_DECERR_REPORTED_RD(src)               (((src) & 0x00000008)>>3)
#define USB0_WR_DECERR_REPORTED_WR(src)          (((u32)(src)<<3) & 0x00000008)
#define USB0_WR_DECERR_REPORTED_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields usb0_wr_slverr_reported	 */
#define USB0_WR_SLVERR_REPORTED_WIDTH                                         1
#define USB0_WR_SLVERR_REPORTED_SHIFT                                         2
#define USB0_WR_SLVERR_REPORTED_MASK                                 0x00000004
#define USB0_WR_SLVERR_REPORTED_RD(src)               (((src) & 0x00000004)>>2)
#define USB0_WR_SLVERR_REPORTED_WR(src)          (((u32)(src)<<2) & 0x00000004)
#define USB0_WR_SLVERR_REPORTED_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields usb0_rd_decerr_reported	 */
#define USB0_RD_DECERR_REPORTED_WIDTH                                         1
#define USB0_RD_DECERR_REPORTED_SHIFT                                         1
#define USB0_RD_DECERR_REPORTED_MASK                                 0x00000002
#define USB0_RD_DECERR_REPORTED_RD(src)               (((src) & 0x00000002)>>1)
#define USB0_RD_DECERR_REPORTED_WR(src)          (((u32)(src)<<1) & 0x00000002)
#define USB0_RD_DECERR_REPORTED_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields usb0_rd_slverr_reported	 */
#define USB0_RD_SLVERR_REPORTED_WIDTH                                         1
#define USB0_RD_SLVERR_REPORTED_SHIFT                                         0
#define USB0_RD_SLVERR_REPORTED_MASK                                 0x00000001
#define USB0_RD_SLVERR_REPORTED_RD(src)                  (((src) & 0x00000001))
#define USB0_RD_SLVERR_REPORTED_WR(src)             (((u32)(src)) & 0x00000001)
#define USB0_RD_SLVERR_REPORTED_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register ahbc_interruptMask	*/
/*    Mask Register Fields usbh2_wr_decerr_reportedMask    */
#define USBH2_WR_DECERR_REPORTEDMASK_WIDTH                                    1
#define USBH2_WR_DECERR_REPORTEDMASK_SHIFT                                   27
#define USBH2_WR_DECERR_REPORTEDMASK_MASK                            0x08000000
#define USBH2_WR_DECERR_REPORTEDMASK_RD(src)         (((src) & 0x08000000)>>27)
#define USBH2_WR_DECERR_REPORTEDMASK_WR(src)    (((u32)(src)<<27) & 0x08000000)
#define USBH2_WR_DECERR_REPORTEDMASK_SET(dst,src) \
                      (((dst) & ~0x08000000) | (((u32)(src)<<27) & 0x08000000))
/*    Mask Register Fields usbh2_wr_slverr_reportedMask    */
#define USBH2_WR_SLVERR_REPORTEDMASK_WIDTH                                    1
#define USBH2_WR_SLVERR_REPORTEDMASK_SHIFT                                   26
#define USBH2_WR_SLVERR_REPORTEDMASK_MASK                            0x04000000
#define USBH2_WR_SLVERR_REPORTEDMASK_RD(src)         (((src) & 0x04000000)>>26)
#define USBH2_WR_SLVERR_REPORTEDMASK_WR(src)    (((u32)(src)<<26) & 0x04000000)
#define USBH2_WR_SLVERR_REPORTEDMASK_SET(dst,src) \
                      (((dst) & ~0x04000000) | (((u32)(src)<<26) & 0x04000000))
/*    Mask Register Fields usbh2_rd_decerr_reportedMask    */
#define USBH2_RD_DECERR_REPORTEDMASK_WIDTH                                    1
#define USBH2_RD_DECERR_REPORTEDMASK_SHIFT                                   25
#define USBH2_RD_DECERR_REPORTEDMASK_MASK                            0x02000000
#define USBH2_RD_DECERR_REPORTEDMASK_RD(src)         (((src) & 0x02000000)>>25)
#define USBH2_RD_DECERR_REPORTEDMASK_WR(src)    (((u32)(src)<<25) & 0x02000000)
#define USBH2_RD_DECERR_REPORTEDMASK_SET(dst,src) \
                      (((dst) & ~0x02000000) | (((u32)(src)<<25) & 0x02000000))
/*    Mask Register Fields usbh2_rd_slverr_reportedMask    */
#define USBH2_RD_SLVERR_REPORTEDMASK_WIDTH                                    1
#define USBH2_RD_SLVERR_REPORTEDMASK_SHIFT                                   24
#define USBH2_RD_SLVERR_REPORTEDMASK_MASK                            0x01000000
#define USBH2_RD_SLVERR_REPORTEDMASK_RD(src)         (((src) & 0x01000000)>>24)
#define USBH2_RD_SLVERR_REPORTEDMASK_WR(src)    (((u32)(src)<<24) & 0x01000000)
#define USBH2_RD_SLVERR_REPORTEDMASK_SET(dst,src) \
                      (((dst) & ~0x01000000) | (((u32)(src)<<24) & 0x01000000))
/*    Mask Register Fields usbh1_wr_decerr_reportedMask    */
#define USBH1_WR_DECERR_REPORTEDMASK_WIDTH                                    1
#define USBH1_WR_DECERR_REPORTEDMASK_SHIFT                                   23
#define USBH1_WR_DECERR_REPORTEDMASK_MASK                            0x00800000
#define USBH1_WR_DECERR_REPORTEDMASK_RD(src)         (((src) & 0x00800000)>>23)
#define USBH1_WR_DECERR_REPORTEDMASK_WR(src)    (((u32)(src)<<23) & 0x00800000)
#define USBH1_WR_DECERR_REPORTEDMASK_SET(dst,src) \
                      (((dst) & ~0x00800000) | (((u32)(src)<<23) & 0x00800000))
/*    Mask Register Fields usbh1_wr_slverr_reportedMask    */
#define USBH1_WR_SLVERR_REPORTEDMASK_WIDTH                                    1
#define USBH1_WR_SLVERR_REPORTEDMASK_SHIFT                                   22
#define USBH1_WR_SLVERR_REPORTEDMASK_MASK                            0x00400000
#define USBH1_WR_SLVERR_REPORTEDMASK_RD(src)         (((src) & 0x00400000)>>22)
#define USBH1_WR_SLVERR_REPORTEDMASK_WR(src)    (((u32)(src)<<22) & 0x00400000)
#define USBH1_WR_SLVERR_REPORTEDMASK_SET(dst,src) \
                      (((dst) & ~0x00400000) | (((u32)(src)<<22) & 0x00400000))
/*    Mask Register Fields usbh1_rd_decerr_reportedMask    */
#define USBH1_RD_DECERR_REPORTEDMASK_WIDTH                                    1
#define USBH1_RD_DECERR_REPORTEDMASK_SHIFT                                   21
#define USBH1_RD_DECERR_REPORTEDMASK_MASK                            0x00200000
#define USBH1_RD_DECERR_REPORTEDMASK_RD(src)         (((src) & 0x00200000)>>21)
#define USBH1_RD_DECERR_REPORTEDMASK_WR(src)    (((u32)(src)<<21) & 0x00200000)
#define USBH1_RD_DECERR_REPORTEDMASK_SET(dst,src) \
                      (((dst) & ~0x00200000) | (((u32)(src)<<21) & 0x00200000))
/*    Mask Register Fields usbh1_rd_slverr_reportedMask    */
#define USBH1_RD_SLVERR_REPORTEDMASK_WIDTH                                    1
#define USBH1_RD_SLVERR_REPORTEDMASK_SHIFT                                   20
#define USBH1_RD_SLVERR_REPORTEDMASK_MASK                            0x00100000
#define USBH1_RD_SLVERR_REPORTEDMASK_RD(src)         (((src) & 0x00100000)>>20)
#define USBH1_RD_SLVERR_REPORTEDMASK_WR(src)    (((u32)(src)<<20) & 0x00100000)
#define USBH1_RD_SLVERR_REPORTEDMASK_SET(dst,src) \
                      (((dst) & ~0x00100000) | (((u32)(src)<<20) & 0x00100000))
/*    Mask Register Fields tdm_wr_decerr_reportedMask    */
#define TDM_WR_DECERR_REPORTEDMASK_WIDTH                                      1
#define TDM_WR_DECERR_REPORTEDMASK_SHIFT                                     19
#define TDM_WR_DECERR_REPORTEDMASK_MASK                              0x00080000
#define TDM_WR_DECERR_REPORTEDMASK_RD(src)           (((src) & 0x00080000)>>19)
#define TDM_WR_DECERR_REPORTEDMASK_WR(src)      (((u32)(src)<<19) & 0x00080000)
#define TDM_WR_DECERR_REPORTEDMASK_SET(dst,src) \
                      (((dst) & ~0x00080000) | (((u32)(src)<<19) & 0x00080000))
/*    Mask Register Fields tdm_wr_slverr_reportedMask    */
#define TDM_WR_SLVERR_REPORTEDMASK_WIDTH                                      1
#define TDM_WR_SLVERR_REPORTEDMASK_SHIFT                                     18
#define TDM_WR_SLVERR_REPORTEDMASK_MASK                              0x00040000
#define TDM_WR_SLVERR_REPORTEDMASK_RD(src)           (((src) & 0x00040000)>>18)
#define TDM_WR_SLVERR_REPORTEDMASK_WR(src)      (((u32)(src)<<18) & 0x00040000)
#define TDM_WR_SLVERR_REPORTEDMASK_SET(dst,src) \
                      (((dst) & ~0x00040000) | (((u32)(src)<<18) & 0x00040000))
/*    Mask Register Fields tdm_rd_decerr_reportedMask    */
#define TDM_RD_DECERR_REPORTEDMASK_WIDTH                                      1
#define TDM_RD_DECERR_REPORTEDMASK_SHIFT                                     17
#define TDM_RD_DECERR_REPORTEDMASK_MASK                              0x00020000
#define TDM_RD_DECERR_REPORTEDMASK_RD(src)           (((src) & 0x00020000)>>17)
#define TDM_RD_DECERR_REPORTEDMASK_WR(src)      (((u32)(src)<<17) & 0x00020000)
#define TDM_RD_DECERR_REPORTEDMASK_SET(dst,src) \
                      (((dst) & ~0x00020000) | (((u32)(src)<<17) & 0x00020000))
/*    Mask Register Fields tdm_rd_slverr_reportedMask    */
#define TDM_RD_SLVERR_REPORTEDMASK_WIDTH                                      1
#define TDM_RD_SLVERR_REPORTEDMASK_SHIFT                                     16
#define TDM_RD_SLVERR_REPORTEDMASK_MASK                              0x00010000
#define TDM_RD_SLVERR_REPORTEDMASK_RD(src)           (((src) & 0x00010000)>>16)
#define TDM_RD_SLVERR_REPORTEDMASK_WR(src)      (((u32)(src)<<16) & 0x00010000)
#define TDM_RD_SLVERR_REPORTEDMASK_SET(dst,src) \
                      (((dst) & ~0x00010000) | (((u32)(src)<<16) & 0x00010000))
/*    Mask Register Fields usbh_wr_decerr_reportedMask    */
#define USBH_WR_DECERR_REPORTEDMASK_WIDTH                                     1
#define USBH_WR_DECERR_REPORTEDMASK_SHIFT                                    15
#define USBH_WR_DECERR_REPORTEDMASK_MASK                             0x00008000
#define USBH_WR_DECERR_REPORTEDMASK_RD(src)          (((src) & 0x00008000)>>15)
#define USBH_WR_DECERR_REPORTEDMASK_WR(src)     (((u32)(src)<<15) & 0x00008000)
#define USBH_WR_DECERR_REPORTEDMASK_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))
/*    Mask Register Fields usbh_wr_slverr_reportedMask    */
#define USBH_WR_SLVERR_REPORTEDMASK_WIDTH                                     1
#define USBH_WR_SLVERR_REPORTEDMASK_SHIFT                                    14
#define USBH_WR_SLVERR_REPORTEDMASK_MASK                             0x00004000
#define USBH_WR_SLVERR_REPORTEDMASK_RD(src)          (((src) & 0x00004000)>>14)
#define USBH_WR_SLVERR_REPORTEDMASK_WR(src)     (((u32)(src)<<14) & 0x00004000)
#define USBH_WR_SLVERR_REPORTEDMASK_SET(dst,src) \
                      (((dst) & ~0x00004000) | (((u32)(src)<<14) & 0x00004000))
/*    Mask Register Fields usbh_rd_decerr_reportedMask    */
#define USBH_RD_DECERR_REPORTEDMASK_WIDTH                                     1
#define USBH_RD_DECERR_REPORTEDMASK_SHIFT                                    13
#define USBH_RD_DECERR_REPORTEDMASK_MASK                             0x00002000
#define USBH_RD_DECERR_REPORTEDMASK_RD(src)          (((src) & 0x00002000)>>13)
#define USBH_RD_DECERR_REPORTEDMASK_WR(src)     (((u32)(src)<<13) & 0x00002000)
#define USBH_RD_DECERR_REPORTEDMASK_SET(dst,src) \
                      (((dst) & ~0x00002000) | (((u32)(src)<<13) & 0x00002000))
/*    Mask Register Fields usbh_rd_slverr_reportedMask    */
#define USBH_RD_SLVERR_REPORTEDMASK_WIDTH                                     1
#define USBH_RD_SLVERR_REPORTEDMASK_SHIFT                                    12
#define USBH_RD_SLVERR_REPORTEDMASK_MASK                             0x00001000
#define USBH_RD_SLVERR_REPORTEDMASK_RD(src)          (((src) & 0x00001000)>>12)
#define USBH_RD_SLVERR_REPORTEDMASK_WR(src)     (((u32)(src)<<12) & 0x00001000)
#define USBH_RD_SLVERR_REPORTEDMASK_SET(dst,src) \
                      (((dst) & ~0x00001000) | (((u32)(src)<<12) & 0x00001000))
/*    Mask Register Fields sdhc_wr_decerr_reportedMask    */
#define SDHC_WR_DECERR_REPORTEDMASK_WIDTH                                     1
#define SDHC_WR_DECERR_REPORTEDMASK_SHIFT                                    11
#define SDHC_WR_DECERR_REPORTEDMASK_MASK                             0x00000800
#define SDHC_WR_DECERR_REPORTEDMASK_RD(src)          (((src) & 0x00000800)>>11)
#define SDHC_WR_DECERR_REPORTEDMASK_WR(src)     (((u32)(src)<<11) & 0x00000800)
#define SDHC_WR_DECERR_REPORTEDMASK_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*    Mask Register Fields sdhc_wr_slverr_reportedMask    */
#define SDHC_WR_SLVERR_REPORTEDMASK_WIDTH                                     1
#define SDHC_WR_SLVERR_REPORTEDMASK_SHIFT                                    10
#define SDHC_WR_SLVERR_REPORTEDMASK_MASK                             0x00000400
#define SDHC_WR_SLVERR_REPORTEDMASK_RD(src)          (((src) & 0x00000400)>>10)
#define SDHC_WR_SLVERR_REPORTEDMASK_WR(src)     (((u32)(src)<<10) & 0x00000400)
#define SDHC_WR_SLVERR_REPORTEDMASK_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*    Mask Register Fields sdhc_rd_decerr_reportedMask    */
#define SDHC_RD_DECERR_REPORTEDMASK_WIDTH                                     1
#define SDHC_RD_DECERR_REPORTEDMASK_SHIFT                                     9
#define SDHC_RD_DECERR_REPORTEDMASK_MASK                             0x00000200
#define SDHC_RD_DECERR_REPORTEDMASK_RD(src)           (((src) & 0x00000200)>>9)
#define SDHC_RD_DECERR_REPORTEDMASK_WR(src)      (((u32)(src)<<9) & 0x00000200)
#define SDHC_RD_DECERR_REPORTEDMASK_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*    Mask Register Fields sdhc_rd_slverr_reportedMask    */
#define SDHC_RD_SLVERR_REPORTEDMASK_WIDTH                                     1
#define SDHC_RD_SLVERR_REPORTEDMASK_SHIFT                                     8
#define SDHC_RD_SLVERR_REPORTEDMASK_MASK                             0x00000100
#define SDHC_RD_SLVERR_REPORTEDMASK_RD(src)           (((src) & 0x00000100)>>8)
#define SDHC_RD_SLVERR_REPORTEDMASK_WR(src)      (((u32)(src)<<8) & 0x00000100)
#define SDHC_RD_SLVERR_REPORTEDMASK_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*    Mask Register Fields usb1_wr_decerr_reportedMask    */
#define USB1_WR_DECERR_REPORTEDMASK_WIDTH                                     1
#define USB1_WR_DECERR_REPORTEDMASK_SHIFT                                     7
#define USB1_WR_DECERR_REPORTEDMASK_MASK                             0x00000080
#define USB1_WR_DECERR_REPORTEDMASK_RD(src)           (((src) & 0x00000080)>>7)
#define USB1_WR_DECERR_REPORTEDMASK_WR(src)      (((u32)(src)<<7) & 0x00000080)
#define USB1_WR_DECERR_REPORTEDMASK_SET(dst,src) \
                       (((dst) & ~0x00000080) | (((u32)(src)<<7) & 0x00000080))
/*    Mask Register Fields usb1_wr_slverr_reportedMask    */
#define USB1_WR_SLVERR_REPORTEDMASK_WIDTH                                     1
#define USB1_WR_SLVERR_REPORTEDMASK_SHIFT                                     6
#define USB1_WR_SLVERR_REPORTEDMASK_MASK                             0x00000040
#define USB1_WR_SLVERR_REPORTEDMASK_RD(src)           (((src) & 0x00000040)>>6)
#define USB1_WR_SLVERR_REPORTEDMASK_WR(src)      (((u32)(src)<<6) & 0x00000040)
#define USB1_WR_SLVERR_REPORTEDMASK_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*    Mask Register Fields usb1_rd_decerr_reportedMask    */
#define USB1_RD_DECERR_REPORTEDMASK_WIDTH                                     1
#define USB1_RD_DECERR_REPORTEDMASK_SHIFT                                     5
#define USB1_RD_DECERR_REPORTEDMASK_MASK                             0x00000020
#define USB1_RD_DECERR_REPORTEDMASK_RD(src)           (((src) & 0x00000020)>>5)
#define USB1_RD_DECERR_REPORTEDMASK_WR(src)      (((u32)(src)<<5) & 0x00000020)
#define USB1_RD_DECERR_REPORTEDMASK_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*    Mask Register Fields usb1_rd_slverr_reportedMask    */
#define USB1_RD_SLVERR_REPORTEDMASK_WIDTH                                     1
#define USB1_RD_SLVERR_REPORTEDMASK_SHIFT                                     4
#define USB1_RD_SLVERR_REPORTEDMASK_MASK                             0x00000010
#define USB1_RD_SLVERR_REPORTEDMASK_RD(src)           (((src) & 0x00000010)>>4)
#define USB1_RD_SLVERR_REPORTEDMASK_WR(src)      (((u32)(src)<<4) & 0x00000010)
#define USB1_RD_SLVERR_REPORTEDMASK_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*    Mask Register Fields usb0_wr_decerr_reportedMask    */
#define USB0_WR_DECERR_REPORTEDMASK_WIDTH                                     1
#define USB0_WR_DECERR_REPORTEDMASK_SHIFT                                     3
#define USB0_WR_DECERR_REPORTEDMASK_MASK                             0x00000008
#define USB0_WR_DECERR_REPORTEDMASK_RD(src)           (((src) & 0x00000008)>>3)
#define USB0_WR_DECERR_REPORTEDMASK_WR(src)      (((u32)(src)<<3) & 0x00000008)
#define USB0_WR_DECERR_REPORTEDMASK_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*    Mask Register Fields usb0_wr_slverr_reportedMask    */
#define USB0_WR_SLVERR_REPORTEDMASK_WIDTH                                     1
#define USB0_WR_SLVERR_REPORTEDMASK_SHIFT                                     2
#define USB0_WR_SLVERR_REPORTEDMASK_MASK                             0x00000004
#define USB0_WR_SLVERR_REPORTEDMASK_RD(src)           (((src) & 0x00000004)>>2)
#define USB0_WR_SLVERR_REPORTEDMASK_WR(src)      (((u32)(src)<<2) & 0x00000004)
#define USB0_WR_SLVERR_REPORTEDMASK_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*    Mask Register Fields usb0_rd_decerr_reportedMask    */
#define USB0_RD_DECERR_REPORTEDMASK_WIDTH                                     1
#define USB0_RD_DECERR_REPORTEDMASK_SHIFT                                     1
#define USB0_RD_DECERR_REPORTEDMASK_MASK                             0x00000002
#define USB0_RD_DECERR_REPORTEDMASK_RD(src)           (((src) & 0x00000002)>>1)
#define USB0_RD_DECERR_REPORTEDMASK_WR(src)      (((u32)(src)<<1) & 0x00000002)
#define USB0_RD_DECERR_REPORTEDMASK_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*    Mask Register Fields usb0_rd_slverr_reportedMask    */
#define USB0_RD_SLVERR_REPORTEDMASK_WIDTH                                     1
#define USB0_RD_SLVERR_REPORTEDMASK_SHIFT                                     0
#define USB0_RD_SLVERR_REPORTEDMASK_MASK                             0x00000001
#define USB0_RD_SLVERR_REPORTEDMASK_RD(src)              (((src) & 0x00000001))
#define USB0_RD_SLVERR_REPORTEDMASK_WR(src)         (((u32)(src)) & 0x00000001)
#define USB0_RD_SLVERR_REPORTEDMASK_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register AIM_0_AHB	*/ 
/*	 Fields aim_ahb_address_n	 */
#define AIM_AHB_ADDRESS_N_WIDTH                                              20
#define AIM_AHB_ADDRESS_N_SHIFT                                               0
#define AIM_AHB_ADDRESS_N_MASK                                       0x000fffff
#define AIM_AHB_ADDRESS_N_RD(src)                        (((src) & 0x000fffff))
#define AIM_AHB_ADDRESS_N_WR(src)                   (((u32)(src)) & 0x000fffff)
#define AIM_AHB_ADDRESS_N_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AIM_0_SIZE_CTL	*/ 
/*	 Fields aim_en_n	 */
#define AIM_EN_N_WIDTH                                                        1
#define AIM_EN_N_SHIFT                                                       31
#define AIM_EN_N_MASK                                                0x80000000
#define AIM_EN_N_RD(src)                             (((src) & 0x80000000)>>31)
#define AIM_EN_N_WR(src)                        (((u32)(src)<<31) & 0x80000000)
#define AIM_EN_N_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields arsb	 */
#define ARSB_WIDTH                                                            3
#define ARSB_SHIFT                                                           23
#define ARSB_MASK                                                    0x03800000
#define ARSB_RD(src)                                 (((src) & 0x03800000)>>23)
#define ARSB_WR(src)                            (((u32)(src)<<23) & 0x03800000)
#define ARSB_SET(dst,src) \
                      (((dst) & ~0x03800000) | (((u32)(src)<<23) & 0x03800000))
/*	 Fields awsb	 */
#define AWSB_WIDTH                                                            3
#define AWSB_SHIFT                                                           20
#define AWSB_MASK                                                    0x00700000
#define AWSB_RD(src)                                 (((src) & 0x00700000)>>20)
#define AWSB_WR(src)                            (((u32)(src)<<20) & 0x00700000)
#define AWSB_SET(dst,src) \
                      (((dst) & ~0x00700000) | (((u32)(src)<<20) & 0x00700000))
/*	 Fields aim_mask_n	 */
#define AIM_MASK_N_WIDTH                                                     20
#define AIM_MASK_N_SHIFT                                                      0
#define AIM_MASK_N_MASK                                              0x000fffff
#define AIM_MASK_N_RD(src)                               (((src) & 0x000fffff))
#define AIM_MASK_N_WR(src)                          (((u32)(src)) & 0x000fffff)
#define AIM_MASK_N_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AIM_0_AXI_LO	*/ 
/*	 Fields aim_axi_address_LO_n	 */
#define AIM_AXI_ADDRESS_LO_N_WIDTH                                           20
#define AIM_AXI_ADDRESS_LO_N_SHIFT                                            0
#define AIM_AXI_ADDRESS_LO_N_MASK                                    0x000fffff
#define AIM_AXI_ADDRESS_LO_N_RD(src)                     (((src) & 0x000fffff))
#define AIM_AXI_ADDRESS_LO_N_WR(src)                (((u32)(src)) & 0x000fffff)
#define AIM_AXI_ADDRESS_LO_N_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AIM_0_AXI_HI	*/ 
/*	 Fields aim_axi_address_HI_n	 */
#define AIM_AXI_ADDRESS_HI_N_WIDTH                                            8
#define AIM_AXI_ADDRESS_HI_N_SHIFT                                           24
#define AIM_AXI_ADDRESS_HI_N_MASK                                    0xff000000
#define AIM_AXI_ADDRESS_HI_N_RD(src)                 (((src) & 0xff000000)>>24)
#define AIM_AXI_ADDRESS_HI_N_WR(src)            (((u32)(src)<<24) & 0xff000000)
#define AIM_AXI_ADDRESS_HI_N_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))

/*	Register AIM_1_AHB	*/ 
/*	 Fields aim_ahb_address_n	 */
#define AIM_AHB_ADDRESS_N_F1_WIDTH                                           20
#define AIM_AHB_ADDRESS_N_F1_SHIFT                                            0
#define AIM_AHB_ADDRESS_N_F1_MASK                                    0x000fffff
#define AIM_AHB_ADDRESS_N_F1_RD(src)                     (((src) & 0x000fffff))
#define AIM_AHB_ADDRESS_N_F1_WR(src)                (((u32)(src)) & 0x000fffff)
#define AIM_AHB_ADDRESS_N_F1_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AIM_1_SIZE_CTL	*/ 
/*	 Fields aim_en_n	 */
#define AIM_EN_N_F1_WIDTH                                                     1
#define AIM_EN_N_F1_SHIFT                                                    31
#define AIM_EN_N_F1_MASK                                             0x80000000
#define AIM_EN_N_F1_RD(src)                          (((src) & 0x80000000)>>31)
#define AIM_EN_N_F1_WR(src)                     (((u32)(src)<<31) & 0x80000000)
#define AIM_EN_N_F1_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields arsb	 */
#define ARSB_F1_WIDTH                                                         3
#define ARSB_F1_SHIFT                                                        23
#define ARSB_F1_MASK                                                 0x03800000
#define ARSB_F1_RD(src)                              (((src) & 0x03800000)>>23)
#define ARSB_F1_WR(src)                         (((u32)(src)<<23) & 0x03800000)
#define ARSB_F1_SET(dst,src) \
                      (((dst) & ~0x03800000) | (((u32)(src)<<23) & 0x03800000))
/*	 Fields awsb	 */
#define AWSB_F1_WIDTH                                                         3
#define AWSB_F1_SHIFT                                                        20
#define AWSB_F1_MASK                                                 0x00700000
#define AWSB_F1_RD(src)                              (((src) & 0x00700000)>>20)
#define AWSB_F1_WR(src)                         (((u32)(src)<<20) & 0x00700000)
#define AWSB_F1_SET(dst,src) \
                      (((dst) & ~0x00700000) | (((u32)(src)<<20) & 0x00700000))
/*	 Fields aim_mask_n	 */
#define AIM_MASK_N_F1_WIDTH                                                  20
#define AIM_MASK_N_F1_SHIFT                                                   0
#define AIM_MASK_N_F1_MASK                                           0x000fffff
#define AIM_MASK_N_F1_RD(src)                            (((src) & 0x000fffff))
#define AIM_MASK_N_F1_WR(src)                       (((u32)(src)) & 0x000fffff)
#define AIM_MASK_N_F1_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AIM_1_AXI_LO	*/ 
/*	 Fields aim_axi_address_LO_n	 */
#define AIM_AXI_ADDRESS_LO_N_F1_WIDTH                                        20
#define AIM_AXI_ADDRESS_LO_N_F1_SHIFT                                         0
#define AIM_AXI_ADDRESS_LO_N_F1_MASK                                 0x000fffff
#define AIM_AXI_ADDRESS_LO_N_F1_RD(src)                  (((src) & 0x000fffff))
#define AIM_AXI_ADDRESS_LO_N_F1_WR(src)             (((u32)(src)) & 0x000fffff)
#define AIM_AXI_ADDRESS_LO_N_F1_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AIM_1_AXI_HI	*/ 
/*	 Fields aim_axi_address_HI_n	 */
#define AIM_AXI_ADDRESS_HI_N_F1_WIDTH                                         8
#define AIM_AXI_ADDRESS_HI_N_F1_SHIFT                                        24
#define AIM_AXI_ADDRESS_HI_N_F1_MASK                                 0xff000000
#define AIM_AXI_ADDRESS_HI_N_F1_RD(src)              (((src) & 0xff000000)>>24)
#define AIM_AXI_ADDRESS_HI_N_F1_WR(src)         (((u32)(src)<<24) & 0xff000000)
#define AIM_AXI_ADDRESS_HI_N_F1_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))

/*	Register AIM_2_AHB	*/ 
/*	 Fields aim_ahb_address_n	 */
#define AIM_AHB_ADDRESS_N_F2_WIDTH                                           20
#define AIM_AHB_ADDRESS_N_F2_SHIFT                                            0
#define AIM_AHB_ADDRESS_N_F2_MASK                                    0x000fffff
#define AIM_AHB_ADDRESS_N_F2_RD(src)                     (((src) & 0x000fffff))
#define AIM_AHB_ADDRESS_N_F2_WR(src)                (((u32)(src)) & 0x000fffff)
#define AIM_AHB_ADDRESS_N_F2_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AIM_2_SIZE_CTL	*/ 
/*	 Fields aim_en_n	 */
#define AIM_EN_N_F2_WIDTH                                                     1
#define AIM_EN_N_F2_SHIFT                                                    31
#define AIM_EN_N_F2_MASK                                             0x80000000
#define AIM_EN_N_F2_RD(src)                          (((src) & 0x80000000)>>31)
#define AIM_EN_N_F2_WR(src)                     (((u32)(src)<<31) & 0x80000000)
#define AIM_EN_N_F2_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields arsb	 */
#define ARSB_F2_WIDTH                                                         3
#define ARSB_F2_SHIFT                                                        23
#define ARSB_F2_MASK                                                 0x03800000
#define ARSB_F2_RD(src)                              (((src) & 0x03800000)>>23)
#define ARSB_F2_WR(src)                         (((u32)(src)<<23) & 0x03800000)
#define ARSB_F2_SET(dst,src) \
                      (((dst) & ~0x03800000) | (((u32)(src)<<23) & 0x03800000))
/*	 Fields awsb	 */
#define AWSB_F2_WIDTH                                                         3
#define AWSB_F2_SHIFT                                                        20
#define AWSB_F2_MASK                                                 0x00700000
#define AWSB_F2_RD(src)                              (((src) & 0x00700000)>>20)
#define AWSB_F2_WR(src)                         (((u32)(src)<<20) & 0x00700000)
#define AWSB_F2_SET(dst,src) \
                      (((dst) & ~0x00700000) | (((u32)(src)<<20) & 0x00700000))
/*	 Fields aim_mask_n	 */
#define AIM_MASK_N_F2_WIDTH                                                  20
#define AIM_MASK_N_F2_SHIFT                                                   0
#define AIM_MASK_N_F2_MASK                                           0x000fffff
#define AIM_MASK_N_F2_RD(src)                            (((src) & 0x000fffff))
#define AIM_MASK_N_F2_WR(src)                       (((u32)(src)) & 0x000fffff)
#define AIM_MASK_N_F2_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AIM_2_AXI_LO	*/ 
/*	 Fields aim_axi_address_LO_n	 */
#define AIM_AXI_ADDRESS_LO_N_F2_WIDTH                                        20
#define AIM_AXI_ADDRESS_LO_N_F2_SHIFT                                         0
#define AIM_AXI_ADDRESS_LO_N_F2_MASK                                 0x000fffff
#define AIM_AXI_ADDRESS_LO_N_F2_RD(src)                  (((src) & 0x000fffff))
#define AIM_AXI_ADDRESS_LO_N_F2_WR(src)             (((u32)(src)) & 0x000fffff)
#define AIM_AXI_ADDRESS_LO_N_F2_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AIM_2_AXI_HI	*/ 
/*	 Fields aim_axi_address_HI_n	 */
#define AIM_AXI_ADDRESS_HI_N_F2_WIDTH                                         8
#define AIM_AXI_ADDRESS_HI_N_F2_SHIFT                                        24
#define AIM_AXI_ADDRESS_HI_N_F2_MASK                                 0xff000000
#define AIM_AXI_ADDRESS_HI_N_F2_RD(src)              (((src) & 0xff000000)>>24)
#define AIM_AXI_ADDRESS_HI_N_F2_WR(src)         (((u32)(src)<<24) & 0xff000000)
#define AIM_AXI_ADDRESS_HI_N_F2_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))

/*	Register AIM_3_AHB	*/ 
/*	 Fields aim_ahb_address_n	 */
#define AIM_AHB_ADDRESS_N_F3_WIDTH                                           20
#define AIM_AHB_ADDRESS_N_F3_SHIFT                                            0
#define AIM_AHB_ADDRESS_N_F3_MASK                                    0x000fffff
#define AIM_AHB_ADDRESS_N_F3_RD(src)                     (((src) & 0x000fffff))
#define AIM_AHB_ADDRESS_N_F3_WR(src)                (((u32)(src)) & 0x000fffff)
#define AIM_AHB_ADDRESS_N_F3_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AIM_3_SIZE_CTL	*/ 
/*	 Fields aim_en_n	 */
#define AIM_EN_N_F3_WIDTH                                                     1
#define AIM_EN_N_F3_SHIFT                                                    31
#define AIM_EN_N_F3_MASK                                             0x80000000
#define AIM_EN_N_F3_RD(src)                          (((src) & 0x80000000)>>31)
#define AIM_EN_N_F3_WR(src)                     (((u32)(src)<<31) & 0x80000000)
#define AIM_EN_N_F3_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields arsb	 */
#define ARSB_F3_WIDTH                                                         3
#define ARSB_F3_SHIFT                                                        23
#define ARSB_F3_MASK                                                 0x03800000
#define ARSB_F3_RD(src)                              (((src) & 0x03800000)>>23)
#define ARSB_F3_WR(src)                         (((u32)(src)<<23) & 0x03800000)
#define ARSB_F3_SET(dst,src) \
                      (((dst) & ~0x03800000) | (((u32)(src)<<23) & 0x03800000))
/*	 Fields awsb	 */
#define AWSB_F3_WIDTH                                                         3
#define AWSB_F3_SHIFT                                                        20
#define AWSB_F3_MASK                                                 0x00700000
#define AWSB_F3_RD(src)                              (((src) & 0x00700000)>>20)
#define AWSB_F3_WR(src)                         (((u32)(src)<<20) & 0x00700000)
#define AWSB_F3_SET(dst,src) \
                      (((dst) & ~0x00700000) | (((u32)(src)<<20) & 0x00700000))
/*	 Fields aim_mask_n	 */
#define AIM_MASK_N_F3_WIDTH                                                  20
#define AIM_MASK_N_F3_SHIFT                                                   0
#define AIM_MASK_N_F3_MASK                                           0x000fffff
#define AIM_MASK_N_F3_RD(src)                            (((src) & 0x000fffff))
#define AIM_MASK_N_F3_WR(src)                       (((u32)(src)) & 0x000fffff)
#define AIM_MASK_N_F3_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AIM_3_AXI_LO	*/ 
/*	 Fields aim_axi_address_LO_n	 */
#define AIM_AXI_ADDRESS_LO_N_F3_WIDTH                                        20
#define AIM_AXI_ADDRESS_LO_N_F3_SHIFT                                         0
#define AIM_AXI_ADDRESS_LO_N_F3_MASK                                 0x000fffff
#define AIM_AXI_ADDRESS_LO_N_F3_RD(src)                  (((src) & 0x000fffff))
#define AIM_AXI_ADDRESS_LO_N_F3_WR(src)             (((u32)(src)) & 0x000fffff)
#define AIM_AXI_ADDRESS_LO_N_F3_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AIM_3_AXI_HI	*/ 
/*	 Fields aim_axi_address_HI_n	 */
#define AIM_AXI_ADDRESS_HI_N_F3_WIDTH                                         8
#define AIM_AXI_ADDRESS_HI_N_F3_SHIFT                                        24
#define AIM_AXI_ADDRESS_HI_N_F3_MASK                                 0xff000000
#define AIM_AXI_ADDRESS_HI_N_F3_RD(src)              (((src) & 0xff000000)>>24)
#define AIM_AXI_ADDRESS_HI_N_F3_WR(src)         (((u32)(src)<<24) & 0xff000000)
#define AIM_AXI_ADDRESS_HI_N_F3_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))

/*	Register AIM_4_AHB	*/ 
/*	 Fields aim_ahb_address_n	 */
#define AIM_AHB_ADDRESS_N_F4_WIDTH                                           20
#define AIM_AHB_ADDRESS_N_F4_SHIFT                                            0
#define AIM_AHB_ADDRESS_N_F4_MASK                                    0x000fffff
#define AIM_AHB_ADDRESS_N_F4_RD(src)                     (((src) & 0x000fffff))
#define AIM_AHB_ADDRESS_N_F4_WR(src)                (((u32)(src)) & 0x000fffff)
#define AIM_AHB_ADDRESS_N_F4_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AIM_4_SIZE_CTL	*/ 
/*	 Fields aim_en_n	 */
#define AIM_EN_N_F4_WIDTH                                                     1
#define AIM_EN_N_F4_SHIFT                                                    31
#define AIM_EN_N_F4_MASK                                             0x80000000
#define AIM_EN_N_F4_RD(src)                          (((src) & 0x80000000)>>31)
#define AIM_EN_N_F4_WR(src)                     (((u32)(src)<<31) & 0x80000000)
#define AIM_EN_N_F4_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields arsb	 */
#define ARSB_F4_WIDTH                                                         3
#define ARSB_F4_SHIFT                                                        23
#define ARSB_F4_MASK                                                 0x03800000
#define ARSB_F4_RD(src)                              (((src) & 0x03800000)>>23)
#define ARSB_F4_WR(src)                         (((u32)(src)<<23) & 0x03800000)
#define ARSB_F4_SET(dst,src) \
                      (((dst) & ~0x03800000) | (((u32)(src)<<23) & 0x03800000))
/*	 Fields awsb	 */
#define AWSB_F4_WIDTH                                                         3
#define AWSB_F4_SHIFT                                                        20
#define AWSB_F4_MASK                                                 0x00700000
#define AWSB_F4_RD(src)                              (((src) & 0x00700000)>>20)
#define AWSB_F4_WR(src)                         (((u32)(src)<<20) & 0x00700000)
#define AWSB_F4_SET(dst,src) \
                      (((dst) & ~0x00700000) | (((u32)(src)<<20) & 0x00700000))
/*	 Fields aim_mask_n	 */
#define AIM_MASK_N_F4_WIDTH                                                  20
#define AIM_MASK_N_F4_SHIFT                                                   0
#define AIM_MASK_N_F4_MASK                                           0x000fffff
#define AIM_MASK_N_F4_RD(src)                            (((src) & 0x000fffff))
#define AIM_MASK_N_F4_WR(src)                       (((u32)(src)) & 0x000fffff)
#define AIM_MASK_N_F4_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AIM_4_AXI_LO	*/ 
/*	 Fields aim_axi_address_LO_n	 */
#define AIM_AXI_ADDRESS_LO_N_F4_WIDTH                                        20
#define AIM_AXI_ADDRESS_LO_N_F4_SHIFT                                         0
#define AIM_AXI_ADDRESS_LO_N_F4_MASK                                 0x000fffff
#define AIM_AXI_ADDRESS_LO_N_F4_RD(src)                  (((src) & 0x000fffff))
#define AIM_AXI_ADDRESS_LO_N_F4_WR(src)             (((u32)(src)) & 0x000fffff)
#define AIM_AXI_ADDRESS_LO_N_F4_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AIM_4_AXI_HI	*/ 
/*	 Fields aim_axi_address_HI_n	 */
#define AIM_AXI_ADDRESS_HI_N_F4_WIDTH                                         8
#define AIM_AXI_ADDRESS_HI_N_F4_SHIFT                                        24
#define AIM_AXI_ADDRESS_HI_N_F4_MASK                                 0xff000000
#define AIM_AXI_ADDRESS_HI_N_F4_RD(src)              (((src) & 0xff000000)>>24)
#define AIM_AXI_ADDRESS_HI_N_F4_WR(src)         (((u32)(src)<<24) & 0xff000000)
#define AIM_AXI_ADDRESS_HI_N_F4_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))

/*	Register AIM_5_AHB	*/ 
/*	 Fields aim_ahb_address_n	 */
#define AIM_AHB_ADDRESS_N_F5_WIDTH                                           20
#define AIM_AHB_ADDRESS_N_F5_SHIFT                                            0
#define AIM_AHB_ADDRESS_N_F5_MASK                                    0x000fffff
#define AIM_AHB_ADDRESS_N_F5_RD(src)                     (((src) & 0x000fffff))
#define AIM_AHB_ADDRESS_N_F5_WR(src)                (((u32)(src)) & 0x000fffff)
#define AIM_AHB_ADDRESS_N_F5_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AIM_5_SIZE_CTL	*/ 
/*	 Fields aim_en_n	 */
#define AIM_EN_N_F5_WIDTH                                                     1
#define AIM_EN_N_F5_SHIFT                                                    31
#define AIM_EN_N_F5_MASK                                             0x80000000
#define AIM_EN_N_F5_RD(src)                          (((src) & 0x80000000)>>31)
#define AIM_EN_N_F5_WR(src)                     (((u32)(src)<<31) & 0x80000000)
#define AIM_EN_N_F5_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields arsb	 */
#define ARSB_F5_WIDTH                                                         3
#define ARSB_F5_SHIFT                                                        23
#define ARSB_F5_MASK                                                 0x03800000
#define ARSB_F5_RD(src)                              (((src) & 0x03800000)>>23)
#define ARSB_F5_WR(src)                         (((u32)(src)<<23) & 0x03800000)
#define ARSB_F5_SET(dst,src) \
                      (((dst) & ~0x03800000) | (((u32)(src)<<23) & 0x03800000))
/*	 Fields awsb	 */
#define AWSB_F5_WIDTH                                                         3
#define AWSB_F5_SHIFT                                                        20
#define AWSB_F5_MASK                                                 0x00700000
#define AWSB_F5_RD(src)                              (((src) & 0x00700000)>>20)
#define AWSB_F5_WR(src)                         (((u32)(src)<<20) & 0x00700000)
#define AWSB_F5_SET(dst,src) \
                      (((dst) & ~0x00700000) | (((u32)(src)<<20) & 0x00700000))
/*	 Fields aim_mask_n	 */
#define AIM_MASK_N_F5_WIDTH                                                  20
#define AIM_MASK_N_F5_SHIFT                                                   0
#define AIM_MASK_N_F5_MASK                                           0x000fffff
#define AIM_MASK_N_F5_RD(src)                            (((src) & 0x000fffff))
#define AIM_MASK_N_F5_WR(src)                       (((u32)(src)) & 0x000fffff)
#define AIM_MASK_N_F5_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AIM_5_AXI_LO	*/ 
/*	 Fields aim_axi_address_LO_n	 */
#define AIM_AXI_ADDRESS_LO_N_F5_WIDTH                                        20
#define AIM_AXI_ADDRESS_LO_N_F5_SHIFT                                         0
#define AIM_AXI_ADDRESS_LO_N_F5_MASK                                 0x000fffff
#define AIM_AXI_ADDRESS_LO_N_F5_RD(src)                  (((src) & 0x000fffff))
#define AIM_AXI_ADDRESS_LO_N_F5_WR(src)             (((u32)(src)) & 0x000fffff)
#define AIM_AXI_ADDRESS_LO_N_F5_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AIM_5_AXI_HI	*/ 
/*	 Fields aim_axi_address_HI_n	 */
#define AIM_AXI_ADDRESS_HI_N_F5_WIDTH                                         8
#define AIM_AXI_ADDRESS_HI_N_F5_SHIFT                                        24
#define AIM_AXI_ADDRESS_HI_N_F5_MASK                                 0xff000000
#define AIM_AXI_ADDRESS_HI_N_F5_RD(src)              (((src) & 0xff000000)>>24)
#define AIM_AXI_ADDRESS_HI_N_F5_WR(src)         (((u32)(src)<<24) & 0xff000000)
#define AIM_AXI_ADDRESS_HI_N_F5_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))

/*	Register AIM_6_AHB	*/ 
/*	 Fields aim_ahb_address_n	 */
#define AIM_AHB_ADDRESS_N_F6_WIDTH                                           20
#define AIM_AHB_ADDRESS_N_F6_SHIFT                                            0
#define AIM_AHB_ADDRESS_N_F6_MASK                                    0x000fffff
#define AIM_AHB_ADDRESS_N_F6_RD(src)                     (((src) & 0x000fffff))
#define AIM_AHB_ADDRESS_N_F6_WR(src)                (((u32)(src)) & 0x000fffff)
#define AIM_AHB_ADDRESS_N_F6_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AIM_6_SIZE_CTL	*/ 
/*	 Fields aim_en_n	 */
#define AIM_EN_N_F6_WIDTH                                                     1
#define AIM_EN_N_F6_SHIFT                                                    31
#define AIM_EN_N_F6_MASK                                             0x80000000
#define AIM_EN_N_F6_RD(src)                          (((src) & 0x80000000)>>31)
#define AIM_EN_N_F6_WR(src)                     (((u32)(src)<<31) & 0x80000000)
#define AIM_EN_N_F6_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields arsb	 */
#define ARSB_F6_WIDTH                                                         3
#define ARSB_F6_SHIFT                                                        23
#define ARSB_F6_MASK                                                 0x03800000
#define ARSB_F6_RD(src)                              (((src) & 0x03800000)>>23)
#define ARSB_F6_WR(src)                         (((u32)(src)<<23) & 0x03800000)
#define ARSB_F6_SET(dst,src) \
                      (((dst) & ~0x03800000) | (((u32)(src)<<23) & 0x03800000))
/*	 Fields awsb	 */
#define AWSB_F6_WIDTH                                                         3
#define AWSB_F6_SHIFT                                                        20
#define AWSB_F6_MASK                                                 0x00700000
#define AWSB_F6_RD(src)                              (((src) & 0x00700000)>>20)
#define AWSB_F6_WR(src)                         (((u32)(src)<<20) & 0x00700000)
#define AWSB_F6_SET(dst,src) \
                      (((dst) & ~0x00700000) | (((u32)(src)<<20) & 0x00700000))
/*	 Fields aim_mask_n	 */
#define AIM_MASK_N_F6_WIDTH                                                  20
#define AIM_MASK_N_F6_SHIFT                                                   0
#define AIM_MASK_N_F6_MASK                                           0x000fffff
#define AIM_MASK_N_F6_RD(src)                            (((src) & 0x000fffff))
#define AIM_MASK_N_F6_WR(src)                       (((u32)(src)) & 0x000fffff)
#define AIM_MASK_N_F6_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AIM_6_AXI_LO	*/ 
/*	 Fields aim_axi_address_LO_n	 */
#define AIM_AXI_ADDRESS_LO_N_F6_WIDTH                                        20
#define AIM_AXI_ADDRESS_LO_N_F6_SHIFT                                         0
#define AIM_AXI_ADDRESS_LO_N_F6_MASK                                 0x000fffff
#define AIM_AXI_ADDRESS_LO_N_F6_RD(src)                  (((src) & 0x000fffff))
#define AIM_AXI_ADDRESS_LO_N_F6_WR(src)             (((u32)(src)) & 0x000fffff)
#define AIM_AXI_ADDRESS_LO_N_F6_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AIM_6_AXI_HI	*/ 
/*	 Fields aim_axi_address_HI_n	 */
#define AIM_AXI_ADDRESS_HI_N_F6_WIDTH                                         8
#define AIM_AXI_ADDRESS_HI_N_F6_SHIFT                                        24
#define AIM_AXI_ADDRESS_HI_N_F6_MASK                                 0xff000000
#define AIM_AXI_ADDRESS_HI_N_F6_RD(src)              (((src) & 0xff000000)>>24)
#define AIM_AXI_ADDRESS_HI_N_F6_WR(src)         (((u32)(src)<<24) & 0xff000000)
#define AIM_AXI_ADDRESS_HI_N_F6_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))

/*	Register AIM_7_AHB	*/ 
/*	 Fields aim_ahb_address_n	 */
#define AIM_AHB_ADDRESS_N_F7_WIDTH                                           20
#define AIM_AHB_ADDRESS_N_F7_SHIFT                                            0
#define AIM_AHB_ADDRESS_N_F7_MASK                                    0x000fffff
#define AIM_AHB_ADDRESS_N_F7_RD(src)                     (((src) & 0x000fffff))
#define AIM_AHB_ADDRESS_N_F7_WR(src)                (((u32)(src)) & 0x000fffff)
#define AIM_AHB_ADDRESS_N_F7_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AIM_7_SIZE_CTL	*/ 
/*	 Fields aim_en_n	 */
#define AIM_EN_N_F7_WIDTH                                                     1
#define AIM_EN_N_F7_SHIFT                                                    31
#define AIM_EN_N_F7_MASK                                             0x80000000
#define AIM_EN_N_F7_RD(src)                          (((src) & 0x80000000)>>31)
#define AIM_EN_N_F7_WR(src)                     (((u32)(src)<<31) & 0x80000000)
#define AIM_EN_N_F7_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields arsb	 */
#define ARSB_F7_WIDTH                                                         3
#define ARSB_F7_SHIFT                                                        23
#define ARSB_F7_MASK                                                 0x03800000
#define ARSB_F7_RD(src)                              (((src) & 0x03800000)>>23)
#define ARSB_F7_WR(src)                         (((u32)(src)<<23) & 0x03800000)
#define ARSB_F7_SET(dst,src) \
                      (((dst) & ~0x03800000) | (((u32)(src)<<23) & 0x03800000))
/*	 Fields awsb	 */
#define AWSB_F7_WIDTH                                                         3
#define AWSB_F7_SHIFT                                                        20
#define AWSB_F7_MASK                                                 0x00700000
#define AWSB_F7_RD(src)                              (((src) & 0x00700000)>>20)
#define AWSB_F7_WR(src)                         (((u32)(src)<<20) & 0x00700000)
#define AWSB_F7_SET(dst,src) \
                      (((dst) & ~0x00700000) | (((u32)(src)<<20) & 0x00700000))
/*	 Fields aim_mask_n	 */
#define AIM_MASK_N_F7_WIDTH                                                  20
#define AIM_MASK_N_F7_SHIFT                                                   0
#define AIM_MASK_N_F7_MASK                                           0x000fffff
#define AIM_MASK_N_F7_RD(src)                            (((src) & 0x000fffff))
#define AIM_MASK_N_F7_WR(src)                       (((u32)(src)) & 0x000fffff)
#define AIM_MASK_N_F7_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AIM_7_AXI_LO	*/ 
/*	 Fields aim_axi_address_LO_n	 */
#define AIM_AXI_ADDRESS_LO_N_F7_WIDTH                                        20
#define AIM_AXI_ADDRESS_LO_N_F7_SHIFT                                         0
#define AIM_AXI_ADDRESS_LO_N_F7_MASK                                 0x000fffff
#define AIM_AXI_ADDRESS_LO_N_F7_RD(src)                  (((src) & 0x000fffff))
#define AIM_AXI_ADDRESS_LO_N_F7_WR(src)             (((u32)(src)) & 0x000fffff)
#define AIM_AXI_ADDRESS_LO_N_F7_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AIM_7_AXI_HI	*/ 
/*	 Fields aim_axi_address_HI_n	 */
#define AIM_AXI_ADDRESS_HI_N_F7_WIDTH                                         8
#define AIM_AXI_ADDRESS_HI_N_F7_SHIFT                                        24
#define AIM_AXI_ADDRESS_HI_N_F7_MASK                                 0xff000000
#define AIM_AXI_ADDRESS_HI_N_F7_RD(src)              (((src) & 0xff000000)>>24)
#define AIM_AXI_ADDRESS_HI_N_F7_WR(src)         (((u32)(src)<<24) & 0xff000000)
#define AIM_AXI_ADDRESS_HI_N_F7_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))

/*	Register AIM_8_SIZE_CTL	*/ 
/*	 Fields aim_mask_8	 */
#define AIM_MASK_8_WIDTH                                                     20
#define AIM_MASK_8_SHIFT                                                      0
#define AIM_MASK_8_MASK                                              0x000fffff
#define AIM_MASK_8_RD(src)                               (((src) & 0x000fffff))
#define AIM_MASK_8_WR(src)                          (((u32)(src)) & 0x000fffff)
#define AIM_MASK_8_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AIM_8_AXI_LO	*/ 
/*	 Fields arsb	 */
#define ARSB_F8_WIDTH                                                         3
#define ARSB_F8_SHIFT                                                        23
#define ARSB_F8_MASK                                                 0x03800000
#define ARSB_F8_RD(src)                              (((src) & 0x03800000)>>23)
#define ARSB_F8_WR(src)                         (((u32)(src)<<23) & 0x03800000)
#define ARSB_F8_SET(dst,src) \
                      (((dst) & ~0x03800000) | (((u32)(src)<<23) & 0x03800000))
/*	 Fields awsb	 */
#define AWSB_F8_WIDTH                                                         3
#define AWSB_F8_SHIFT                                                        20
#define AWSB_F8_MASK                                                 0x00700000
#define AWSB_F8_RD(src)                              (((src) & 0x00700000)>>20)
#define AWSB_F8_WR(src)                         (((u32)(src)<<20) & 0x00700000)
#define AWSB_F8_SET(dst,src) \
                      (((dst) & ~0x00700000) | (((u32)(src)<<20) & 0x00700000))
/*	 Fields aim_axi_address_LO_8	 */
#define AIM_AXI_ADDRESS_LO_8_WIDTH                                           20
#define AIM_AXI_ADDRESS_LO_8_SHIFT                                            0
#define AIM_AXI_ADDRESS_LO_8_MASK                                    0x000fffff
#define AIM_AXI_ADDRESS_LO_8_RD(src)                     (((src) & 0x000fffff))
#define AIM_AXI_ADDRESS_LO_8_WR(src)                (((u32)(src)) & 0x000fffff)
#define AIM_AXI_ADDRESS_LO_8_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AIM_8_AXI_HI	*/ 
/*	 Fields aim_axi_address_HI_8	 */
#define AIM_AXI_ADDRESS_HI_8_WIDTH                                            8
#define AIM_AXI_ADDRESS_HI_8_SHIFT                                           24
#define AIM_AXI_ADDRESS_HI_8_MASK                                    0xff000000
#define AIM_AXI_ADDRESS_HI_8_RD(src)                 (((src) & 0xff000000)>>24)
#define AIM_AXI_ADDRESS_HI_8_WR(src)            (((u32)(src)<<24) & 0xff000000)
#define AIM_AXI_ADDRESS_HI_8_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))

/*	Register AOM_0_AXI_LO	*/ 
/*	 Fields aom_axi_address_LO_n	 */
#define AOM_AXI_ADDRESS_LO_N_WIDTH                                           20
#define AOM_AXI_ADDRESS_LO_N_SHIFT                                            0
#define AOM_AXI_ADDRESS_LO_N_MASK                                    0x000fffff
#define AOM_AXI_ADDRESS_LO_N_RD(src)                     (((src) & 0x000fffff))
#define AOM_AXI_ADDRESS_LO_N_WR(src)                (((u32)(src)) & 0x000fffff)
#define AOM_AXI_ADDRESS_LO_N_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AOM_0_AXI_HI	*/ 
/*	 Fields aom_axi_address_HI_n	 */
#define AOM_AXI_ADDRESS_HI_N_WIDTH                                            8
#define AOM_AXI_ADDRESS_HI_N_SHIFT                                           24
#define AOM_AXI_ADDRESS_HI_N_MASK                                    0xff000000
#define AOM_AXI_ADDRESS_HI_N_RD(src)                 (((src) & 0xff000000)>>24)
#define AOM_AXI_ADDRESS_HI_N_WR(src)            (((u32)(src)<<24) & 0xff000000)
#define AOM_AXI_ADDRESS_HI_N_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))

/*	Register AOM_0_SIZE_CTL	*/ 
/*	 Fields aom_en_n	 */
#define AOM_EN_N_WIDTH                                                        1
#define AOM_EN_N_SHIFT                                                       31
#define AOM_EN_N_MASK                                                0x80000000
#define AOM_EN_N_RD(src)                             (((src) & 0x80000000)>>31)
#define AOM_EN_N_WR(src)                        (((u32)(src)<<31) & 0x80000000)
#define AOM_EN_N_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields aom_mask_n	 */
#define AOM_MASK_N_WIDTH                                                     20
#define AOM_MASK_N_SHIFT                                                      0
#define AOM_MASK_N_MASK                                              0x000fffff
#define AOM_MASK_N_RD(src)                               (((src) & 0x000fffff))
#define AOM_MASK_N_WR(src)                          (((u32)(src)) & 0x000fffff)
#define AOM_MASK_N_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AOM_0_AHB	*/ 
/*	 Fields aom_axi_address_n	 */
#define AOM_AXI_ADDRESS_N_WIDTH                                              20
#define AOM_AXI_ADDRESS_N_SHIFT                                               0
#define AOM_AXI_ADDRESS_N_MASK                                       0x000fffff
#define AOM_AXI_ADDRESS_N_RD(src)                        (((src) & 0x000fffff))
#define AOM_AXI_ADDRESS_N_WR(src)                   (((u32)(src)) & 0x000fffff)
#define AOM_AXI_ADDRESS_N_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AOM_1_AXI_LO	*/ 
/*	 Fields aom_axi_address_LO_n	 */
#define AOM_AXI_ADDRESS_LO_N_F1_WIDTH                                        20
#define AOM_AXI_ADDRESS_LO_N_F1_SHIFT                                         0
#define AOM_AXI_ADDRESS_LO_N_F1_MASK                                 0x000fffff
#define AOM_AXI_ADDRESS_LO_N_F1_RD(src)                  (((src) & 0x000fffff))
#define AOM_AXI_ADDRESS_LO_N_F1_WR(src)             (((u32)(src)) & 0x000fffff)
#define AOM_AXI_ADDRESS_LO_N_F1_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AOM_1_AXI_HI	*/ 
/*	 Fields aom_axi_address_HI_n	 */
#define AOM_AXI_ADDRESS_HI_N_F1_WIDTH                                         8
#define AOM_AXI_ADDRESS_HI_N_F1_SHIFT                                        24
#define AOM_AXI_ADDRESS_HI_N_F1_MASK                                 0xff000000
#define AOM_AXI_ADDRESS_HI_N_F1_RD(src)              (((src) & 0xff000000)>>24)
#define AOM_AXI_ADDRESS_HI_N_F1_WR(src)         (((u32)(src)<<24) & 0xff000000)
#define AOM_AXI_ADDRESS_HI_N_F1_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))

/*	Register AOM_1_SIZE_CTL	*/ 
/*	 Fields aom_en_n	 */
#define AOM_EN_N_F1_WIDTH                                                     1
#define AOM_EN_N_F1_SHIFT                                                    31
#define AOM_EN_N_F1_MASK                                             0x80000000
#define AOM_EN_N_F1_RD(src)                          (((src) & 0x80000000)>>31)
#define AOM_EN_N_F1_WR(src)                     (((u32)(src)<<31) & 0x80000000)
#define AOM_EN_N_F1_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields aom_mask_n	 */
#define AOM_MASK_N_F1_WIDTH                                                  20
#define AOM_MASK_N_F1_SHIFT                                                   0
#define AOM_MASK_N_F1_MASK                                           0x000fffff
#define AOM_MASK_N_F1_RD(src)                            (((src) & 0x000fffff))
#define AOM_MASK_N_F1_WR(src)                       (((u32)(src)) & 0x000fffff)
#define AOM_MASK_N_F1_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AOM_1_AHB	*/ 
/*	 Fields aom_axi_address_n	 */
#define AOM_AXI_ADDRESS_N_F1_WIDTH                                           20
#define AOM_AXI_ADDRESS_N_F1_SHIFT                                            0
#define AOM_AXI_ADDRESS_N_F1_MASK                                    0x000fffff
#define AOM_AXI_ADDRESS_N_F1_RD(src)                     (((src) & 0x000fffff))
#define AOM_AXI_ADDRESS_N_F1_WR(src)                (((u32)(src)) & 0x000fffff)
#define AOM_AXI_ADDRESS_N_F1_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AOM_2_AXI_LO	*/ 
/*	 Fields aom_axi_address_LO_n	 */
#define AOM_AXI_ADDRESS_LO_N_F2_WIDTH                                        20
#define AOM_AXI_ADDRESS_LO_N_F2_SHIFT                                         0
#define AOM_AXI_ADDRESS_LO_N_F2_MASK                                 0x000fffff
#define AOM_AXI_ADDRESS_LO_N_F2_RD(src)                  (((src) & 0x000fffff))
#define AOM_AXI_ADDRESS_LO_N_F2_WR(src)             (((u32)(src)) & 0x000fffff)
#define AOM_AXI_ADDRESS_LO_N_F2_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AOM_2_AXI_HI	*/ 
/*	 Fields aom_axi_address_HI_n	 */
#define AOM_AXI_ADDRESS_HI_N_F2_WIDTH                                         8
#define AOM_AXI_ADDRESS_HI_N_F2_SHIFT                                        24
#define AOM_AXI_ADDRESS_HI_N_F2_MASK                                 0xff000000
#define AOM_AXI_ADDRESS_HI_N_F2_RD(src)              (((src) & 0xff000000)>>24)
#define AOM_AXI_ADDRESS_HI_N_F2_WR(src)         (((u32)(src)<<24) & 0xff000000)
#define AOM_AXI_ADDRESS_HI_N_F2_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))

/*	Register AOM_2_SIZE_CTL	*/ 
/*	 Fields aom_en_n	 */
#define AOM_EN_N_F2_WIDTH                                                     1
#define AOM_EN_N_F2_SHIFT                                                    31
#define AOM_EN_N_F2_MASK                                             0x80000000
#define AOM_EN_N_F2_RD(src)                          (((src) & 0x80000000)>>31)
#define AOM_EN_N_F2_WR(src)                     (((u32)(src)<<31) & 0x80000000)
#define AOM_EN_N_F2_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields aom_mask_n	 */
#define AOM_MASK_N_F2_WIDTH                                                  20
#define AOM_MASK_N_F2_SHIFT                                                   0
#define AOM_MASK_N_F2_MASK                                           0x000fffff
#define AOM_MASK_N_F2_RD(src)                            (((src) & 0x000fffff))
#define AOM_MASK_N_F2_WR(src)                       (((u32)(src)) & 0x000fffff)
#define AOM_MASK_N_F2_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AOM_2_AHB	*/ 
/*	 Fields aom_axi_address_n	 */
#define AOM_AXI_ADDRESS_N_F2_WIDTH                                           20
#define AOM_AXI_ADDRESS_N_F2_SHIFT                                            0
#define AOM_AXI_ADDRESS_N_F2_MASK                                    0x000fffff
#define AOM_AXI_ADDRESS_N_F2_RD(src)                     (((src) & 0x000fffff))
#define AOM_AXI_ADDRESS_N_F2_WR(src)                (((u32)(src)) & 0x000fffff)
#define AOM_AXI_ADDRESS_N_F2_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AOM_3_AXI_LO	*/ 
/*	 Fields aom_axi_address_LO_n	 */
#define AOM_AXI_ADDRESS_LO_N_F3_WIDTH                                        20
#define AOM_AXI_ADDRESS_LO_N_F3_SHIFT                                         0
#define AOM_AXI_ADDRESS_LO_N_F3_MASK                                 0x000fffff
#define AOM_AXI_ADDRESS_LO_N_F3_RD(src)                  (((src) & 0x000fffff))
#define AOM_AXI_ADDRESS_LO_N_F3_WR(src)             (((u32)(src)) & 0x000fffff)
#define AOM_AXI_ADDRESS_LO_N_F3_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AOM_3_AXI_HI	*/ 
/*	 Fields aom_axi_address_HI_n	 */
#define AOM_AXI_ADDRESS_HI_N_F3_WIDTH                                         8
#define AOM_AXI_ADDRESS_HI_N_F3_SHIFT                                        24
#define AOM_AXI_ADDRESS_HI_N_F3_MASK                                 0xff000000
#define AOM_AXI_ADDRESS_HI_N_F3_RD(src)              (((src) & 0xff000000)>>24)
#define AOM_AXI_ADDRESS_HI_N_F3_WR(src)         (((u32)(src)<<24) & 0xff000000)
#define AOM_AXI_ADDRESS_HI_N_F3_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))

/*	Register AOM_3_SIZE_CTL	*/ 
/*	 Fields aom_en_n	 */
#define AOM_EN_N_F3_WIDTH                                                     1
#define AOM_EN_N_F3_SHIFT                                                    31
#define AOM_EN_N_F3_MASK                                             0x80000000
#define AOM_EN_N_F3_RD(src)                          (((src) & 0x80000000)>>31)
#define AOM_EN_N_F3_WR(src)                     (((u32)(src)<<31) & 0x80000000)
#define AOM_EN_N_F3_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields aom_mask_n	 */
#define AOM_MASK_N_F3_WIDTH                                                  20
#define AOM_MASK_N_F3_SHIFT                                                   0
#define AOM_MASK_N_F3_MASK                                           0x000fffff
#define AOM_MASK_N_F3_RD(src)                            (((src) & 0x000fffff))
#define AOM_MASK_N_F3_WR(src)                       (((u32)(src)) & 0x000fffff)
#define AOM_MASK_N_F3_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register AOM_3_AHB	*/ 
/*	 Fields aom_axi_address_n	 */
#define AOM_AXI_ADDRESS_N_F3_WIDTH                                           20
#define AOM_AXI_ADDRESS_N_F3_SHIFT                                            0
#define AOM_AXI_ADDRESS_N_F3_MASK                                    0x000fffff
#define AOM_AXI_ADDRESS_N_F3_RD(src)                     (((src) & 0x000fffff))
#define AOM_AXI_ADDRESS_N_F3_WR(src)                (((u32)(src)) & 0x000fffff)
#define AOM_AXI_ADDRESS_N_F3_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register ahb_rd_err_addr	*/ 
/*	 Fields addr	 */
#define ADDR_WIDTH                                                           32
#define ADDR_SHIFT                                                            0
#define ADDR_MASK                                                    0xffffffff
#define ADDR_RD(src)                                     (((src) & 0xffffffff))
#define ADDR_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register ahb_rd_err_attributes	*/ 
/*	 Fields ahb_htrans	 */
#define AHB_HTRANS_WIDTH                                                      2
#define AHB_HTRANS_SHIFT                                                     11
#define AHB_HTRANS_MASK                                              0x00001800
#define AHB_HTRANS_RD(src)                           (((src) & 0x00001800)>>11)
#define AHB_HTRANS_SET(dst,src) \
                      (((dst) & ~0x00001800) | (((u32)(src)<<11) & 0x00001800))
/*	 Fields ahb_hsize	 */
#define AHB_HSIZE_WIDTH                                                       3
#define AHB_HSIZE_SHIFT                                                       8
#define AHB_HSIZE_MASK                                               0x00000700
#define AHB_HSIZE_RD(src)                             (((src) & 0x00000700)>>8)
#define AHB_HSIZE_SET(dst,src) \
                       (((dst) & ~0x00000700) | (((u32)(src)<<8) & 0x00000700))
/*	 Fields ahb_hburst	 */
#define AHB_HBURST_WIDTH                                                      3
#define AHB_HBURST_SHIFT                                                      5
#define AHB_HBURST_MASK                                              0x000000e0
#define AHB_HBURST_RD(src)                            (((src) & 0x000000e0)>>5)
#define AHB_HBURST_SET(dst,src) \
                       (((dst) & ~0x000000e0) | (((u32)(src)<<5) & 0x000000e0))
/*	 Fields err_addr_decode	 */
#define ERR_ADDR_DECODE_WIDTH                                                 1
#define ERR_ADDR_DECODE_SHIFT                                                 4
#define ERR_ADDR_DECODE_MASK                                         0x00000010
#define ERR_ADDR_DECODE_RD(src)                       (((src) & 0x00000010)>>4)
#define ERR_ADDR_DECODE_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields ahb_rd_err	 */
#define AHB_RD_ERR_WIDTH                                                      1
#define AHB_RD_ERR_SHIFT                                                      0
#define AHB_RD_ERR_MASK                                              0x00000001
#define AHB_RD_ERR_RD(src)                               (((src) & 0x00000001))
#define AHB_RD_ERR_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register ahb_wr_err_addr	*/ 
/*	 Fields addr	 */
#define ADDR_F1_WIDTH                                                        32
#define ADDR_F1_SHIFT                                                         0
#define ADDR_F1_MASK                                                 0xffffffff
#define ADDR_F1_RD(src)                                  (((src) & 0xffffffff))
#define ADDR_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register ahb_wr_err_data	*/ 
/*	 Fields data	 */
#define DATA_WIDTH                                                           32
#define DATA_SHIFT                                                            0
#define DATA_MASK                                                    0xffffffff
#define DATA_RD(src)                                     (((src) & 0xffffffff))
#define DATA_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register ahb_wr_err_attributes	*/ 
/*	 Fields ahb_htrans	 */
#define AHB_HTRANS_F1_WIDTH                                                   2
#define AHB_HTRANS_F1_SHIFT                                                  11
#define AHB_HTRANS_F1_MASK                                           0x00001800
#define AHB_HTRANS_F1_RD(src)                        (((src) & 0x00001800)>>11)
#define AHB_HTRANS_F1_SET(dst,src) \
                      (((dst) & ~0x00001800) | (((u32)(src)<<11) & 0x00001800))
/*	 Fields ahb_hsize	 */
#define AHB_HSIZE_F1_WIDTH                                                    3
#define AHB_HSIZE_F1_SHIFT                                                    8
#define AHB_HSIZE_F1_MASK                                            0x00000700
#define AHB_HSIZE_F1_RD(src)                          (((src) & 0x00000700)>>8)
#define AHB_HSIZE_F1_SET(dst,src) \
                       (((dst) & ~0x00000700) | (((u32)(src)<<8) & 0x00000700))
/*	 Fields ahb_hburst	 */
#define AHB_HBURST_F1_WIDTH                                                   3
#define AHB_HBURST_F1_SHIFT                                                   5
#define AHB_HBURST_F1_MASK                                           0x000000e0
#define AHB_HBURST_F1_RD(src)                         (((src) & 0x000000e0)>>5)
#define AHB_HBURST_F1_SET(dst,src) \
                       (((dst) & ~0x000000e0) | (((u32)(src)<<5) & 0x000000e0))
/*	 Fields err_addr_decode	 */
#define ERR_ADDR_DECODE_F1_WIDTH                                              1
#define ERR_ADDR_DECODE_F1_SHIFT                                              4
#define ERR_ADDR_DECODE_F1_MASK                                      0x00000010
#define ERR_ADDR_DECODE_F1_RD(src)                    (((src) & 0x00000010)>>4)
#define ERR_ADDR_DECODE_F1_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields ahb_wr_err	 */
#define AHB_WR_ERR_WIDTH                                                      1
#define AHB_WR_ERR_SHIFT                                                      0
#define AHB_WR_ERR_MASK                                              0x00000001
#define AHB_WR_ERR_RD(src)                               (((src) & 0x00000001))
#define AHB_WR_ERR_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register CFG_INTR_CTL	*/ 
/*	 Fields ahbc_i2c0_int	 */
#define AHBC_I2C0_INT_WIDTH                                                   1
#define AHBC_I2C0_INT_SHIFT                                                  27
#define AHBC_I2C0_INT_MASK                                           0x08000000
#define AHBC_I2C0_INT_RD(src)                        (((src) & 0x08000000)>>27)
#define AHBC_I2C0_INT_WR(src)                   (((u32)(src)<<27) & 0x08000000)
#define AHBC_I2C0_INT_SET(dst,src) \
                      (((dst) & ~0x08000000) | (((u32)(src)<<27) & 0x08000000))
/*	 Fields ahbc_gpiopin_int	 */
#define AHBC_GPIOPIN_INT_WIDTH                                               12
#define AHBC_GPIOPIN_INT_SHIFT                                               15
#define AHBC_GPIOPIN_INT_MASK                                        0x07ff8000
#define AHBC_GPIOPIN_INT_RD(src)                     (((src) & 0x07ff8000)>>15)
#define AHBC_GPIOPIN_INT_WR(src)                (((u32)(src)<<15) & 0x07ff8000)
#define AHBC_GPIOPIN_INT_SET(dst,src) \
                      (((dst) & ~0x07ff8000) | (((u32)(src)<<15) & 0x07ff8000))
/*	 Fields sdhc_int	 */
#define SDHC_INT_WIDTH                                                        1
#define SDHC_INT_SHIFT                                                       14
#define SDHC_INT_MASK                                                0x00004000
#define SDHC_INT_RD(src)                             (((src) & 0x00004000)>>14)
#define SDHC_INT_WR(src)                        (((u32)(src)<<14) & 0x00004000)
#define SDHC_INT_SET(dst,src) \
                      (((dst) & ~0x00004000) | (((u32)(src)<<14) & 0x00004000))
/*	 Fields ahbc_tdm_int	 */
#define AHBC_TDM_INT_WIDTH                                                    1
#define AHBC_TDM_INT_SHIFT                                                   13
#define AHBC_TDM_INT_MASK                                            0x00002000
#define AHBC_TDM_INT_RD(src)                         (((src) & 0x00002000)>>13)
#define AHBC_TDM_INT_WR(src)                    (((u32)(src)<<13) & 0x00002000)
#define AHBC_TDM_INT_SET(dst,src) \
                      (((dst) & ~0x00002000) | (((u32)(src)<<13) & 0x00002000))
/*	 Fields usbh1_ehci_int	 */
#define USBH1_EHCI_INT_WIDTH                                                  1
#define USBH1_EHCI_INT_SHIFT                                                 12
#define USBH1_EHCI_INT_MASK                                          0x00001000
#define USBH1_EHCI_INT_RD(src)                       (((src) & 0x00001000)>>12)
#define USBH1_EHCI_INT_WR(src)                  (((u32)(src)<<12) & 0x00001000)
#define USBH1_EHCI_INT_SET(dst,src) \
                      (((dst) & ~0x00001000) | (((u32)(src)<<12) & 0x00001000))
/*	 Fields ohci1_irq	 */
#define OHCI1_IRQ_WIDTH                                                       1
#define OHCI1_IRQ_SHIFT                                                      11
#define OHCI1_IRQ_MASK                                               0x00000800
#define OHCI1_IRQ_RD(src)                            (((src) & 0x00000800)>>11)
#define OHCI1_IRQ_WR(src)                       (((u32)(src)<<11) & 0x00000800)
#define OHCI1_IRQ_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*	 Fields usbh_ehci_int	 */
#define USBH_EHCI_INT_WIDTH                                                   1
#define USBH_EHCI_INT_SHIFT                                                  10
#define USBH_EHCI_INT_MASK                                           0x00000400
#define USBH_EHCI_INT_RD(src)                        (((src) & 0x00000400)>>10)
#define USBH_EHCI_INT_WR(src)                   (((u32)(src)<<10) & 0x00000400)
#define USBH_EHCI_INT_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields ohci_irq	 */
#define OHCI_IRQ_WIDTH                                                        1
#define OHCI_IRQ_SHIFT                                                        9
#define OHCI_IRQ_MASK                                                0x00000200
#define OHCI_IRQ_RD(src)                              (((src) & 0x00000200)>>9)
#define OHCI_IRQ_WR(src)                         (((u32)(src)<<9) & 0x00000200)
#define OHCI_IRQ_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields ahbc_Flash_Int	 */
#define AHBC_FLASH_INT_WIDTH                                                  1
#define AHBC_FLASH_INT_SHIFT                                                  8
#define AHBC_FLASH_INT_MASK                                          0x00000100
#define AHBC_FLASH_INT_RD(src)                        (((src) & 0x00000100)>>8)
#define AHBC_FLASH_INT_WR(src)                   (((u32)(src)<<8) & 0x00000100)
#define AHBC_FLASH_INT_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*	 Fields ahbc_ahb2axi_err_int	 */
#define AHBC_AHB2AXI_ERR_INT_WIDTH                                            1
#define AHBC_AHB2AXI_ERR_INT_SHIFT                                            7
#define AHBC_AHB2AXI_ERR_INT_MASK                                    0x00000080
#define AHBC_AHB2AXI_ERR_INT_RD(src)                  (((src) & 0x00000080)>>7)
#define AHBC_AHB2AXI_ERR_INT_WR(src)             (((u32)(src)<<7) & 0x00000080)
#define AHBC_AHB2AXI_ERR_INT_SET(dst,src) \
                       (((dst) & ~0x00000080) | (((u32)(src)<<7) & 0x00000080))
/*	 Fields ahbc_otg0_int	 */
#define AHBC_OTG0_INT_WIDTH                                                   1
#define AHBC_OTG0_INT_SHIFT                                                   6
#define AHBC_OTG0_INT_MASK                                           0x00000040
#define AHBC_OTG0_INT_RD(src)                         (((src) & 0x00000040)>>6)
#define AHBC_OTG0_INT_WR(src)                    (((u32)(src)<<6) & 0x00000040)
#define AHBC_OTG0_INT_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*	 Fields ahbc_spi_1_int	 */
#define AHBC_SPI_1_INT_WIDTH                                                  1
#define AHBC_SPI_1_INT_SHIFT                                                  5
#define AHBC_SPI_1_INT_MASK                                          0x00000020
#define AHBC_SPI_1_INT_RD(src)                        (((src) & 0x00000020)>>5)
#define AHBC_SPI_1_INT_WR(src)                   (((u32)(src)<<5) & 0x00000020)
#define AHBC_SPI_1_INT_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields ahbc_spi_0_int	 */
#define AHBC_SPI_0_INT_WIDTH                                                  1
#define AHBC_SPI_0_INT_SHIFT                                                  4
#define AHBC_SPI_0_INT_MASK                                          0x00000010
#define AHBC_SPI_0_INT_RD(src)                        (((src) & 0x00000010)>>4)
#define AHBC_SPI_0_INT_WR(src)                   (((u32)(src)<<4) & 0x00000010)
#define AHBC_SPI_0_INT_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields ahbc_uart_3_int	 */
#define AHBC_UART_3_INT_WIDTH                                                 1
#define AHBC_UART_3_INT_SHIFT                                                 3
#define AHBC_UART_3_INT_MASK                                         0x00000008
#define AHBC_UART_3_INT_RD(src)                       (((src) & 0x00000008)>>3)
#define AHBC_UART_3_INT_WR(src)                  (((u32)(src)<<3) & 0x00000008)
#define AHBC_UART_3_INT_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields ahbc_uart_2_int	 */
#define AHBC_UART_2_INT_WIDTH                                                 1
#define AHBC_UART_2_INT_SHIFT                                                 2
#define AHBC_UART_2_INT_MASK                                         0x00000004
#define AHBC_UART_2_INT_RD(src)                       (((src) & 0x00000004)>>2)
#define AHBC_UART_2_INT_WR(src)                  (((u32)(src)<<2) & 0x00000004)
#define AHBC_UART_2_INT_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields ahbc_uart_1_int	 */
#define AHBC_UART_1_INT_WIDTH                                                 1
#define AHBC_UART_1_INT_SHIFT                                                 1
#define AHBC_UART_1_INT_MASK                                         0x00000002
#define AHBC_UART_1_INT_RD(src)                       (((src) & 0x00000002)>>1)
#define AHBC_UART_1_INT_WR(src)                  (((u32)(src)<<1) & 0x00000002)
#define AHBC_UART_1_INT_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields ahbc_uart_0_int	 */
#define AHBC_UART_0_INT_WIDTH                                                 1
#define AHBC_UART_0_INT_SHIFT                                                 0
#define AHBC_UART_0_INT_MASK                                         0x00000001
#define AHBC_UART_0_INT_RD(src)                          (((src) & 0x00000001))
#define AHBC_UART_0_INT_WR(src)                     (((u32)(src)) & 0x00000001)
#define AHBC_UART_0_INT_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register CFG_AUTO_RD_NFLASH_PAGE	*/ 
/*	 Fields NFLASH_AUTO_READ_PAGE	 */
#define NFLASH_AUTO_READ_PAGE_WIDTH                                          20
#define NFLASH_AUTO_READ_PAGE_SHIFT                                           0
#define NFLASH_AUTO_READ_PAGE_MASK                                   0x000fffff
#define NFLASH_AUTO_READ_PAGE_RD(src)                    (((src) & 0x000fffff))
#define NFLASH_AUTO_READ_PAGE_WR(src)               (((u32)(src)) & 0x000fffff)
#define NFLASH_AUTO_READ_PAGE_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register CFG_AUTO_RD_NFLASH	*/ 
/*	 Fields SEL_EXT_AUTO_MODE	 */
#define SEL_EXT_AUTO_MODE_WIDTH                                               1
#define SEL_EXT_AUTO_MODE_SHIFT                                              22
#define SEL_EXT_AUTO_MODE_MASK                                       0x00400000
#define SEL_EXT_AUTO_MODE_RD(src)                    (((src) & 0x00400000)>>22)
#define SEL_EXT_AUTO_MODE_WR(src)               (((u32)(src)<<22) & 0x00400000)
#define SEL_EXT_AUTO_MODE_SET(dst,src) \
                      (((dst) & ~0x00400000) | (((u32)(src)<<22) & 0x00400000))
/*	 Fields NFLASH_ECC_DET_COR	 */
#define NFLASH_ECC_DET_COR_WIDTH                                              1
#define NFLASH_ECC_DET_COR_SHIFT                                             21
#define NFLASH_ECC_DET_COR_MASK                                      0x00200000
#define NFLASH_ECC_DET_COR_RD(src)                   (((src) & 0x00200000)>>21)
#define NFLASH_ECC_DET_COR_WR(src)              (((u32)(src)<<21) & 0x00200000)
#define NFLASH_ECC_DET_COR_SET(dst,src) \
                      (((dst) & ~0x00200000) | (((u32)(src)<<21) & 0x00200000))
/*	 Fields NFLASH_AUTOREAD_EN	 */
#define NFLASH_AUTOREAD_EN_WIDTH                                              1
#define NFLASH_AUTOREAD_EN_SHIFT                                             20
#define NFLASH_AUTOREAD_EN_MASK                                      0x00100000
#define NFLASH_AUTOREAD_EN_RD(src)                   (((src) & 0x00100000)>>20)
#define NFLASH_AUTOREAD_EN_WR(src)              (((u32)(src)<<20) & 0x00100000)
#define NFLASH_AUTOREAD_EN_SET(dst,src) \
                      (((dst) & ~0x00100000) | (((u32)(src)<<20) & 0x00100000))
/*	 Fields NFLASH_BOOT_ADDR_MSB	 */
#define NFLASH_BOOT_ADDR_MSB_WIDTH                                           20
#define NFLASH_BOOT_ADDR_MSB_SHIFT                                            0
#define NFLASH_BOOT_ADDR_MSB_MASK                                    0x000fffff
#define NFLASH_BOOT_ADDR_MSB_RD(src)                     (((src) & 0x000fffff))
#define NFLASH_BOOT_ADDR_MSB_WR(src)                (((u32)(src)) & 0x000fffff)
#define NFLASH_BOOT_ADDR_MSB_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register CFG_USB_HOST_PHY1_CONNECT	*/ 
/*	 Fields usb1_vbusvldext	 */
#define USB1_VBUSVLDEXT_WIDTH                                                 1
#define USB1_VBUSVLDEXT_SHIFT                                                16
#define USB1_VBUSVLDEXT_MASK                                         0x00010000
#define USB1_VBUSVLDEXT_RD(src)                      (((src) & 0x00010000)>>16)
#define USB1_VBUSVLDEXT_WR(src)                 (((u32)(src)<<16) & 0x00010000)
#define USB1_VBUSVLDEXT_SET(dst,src) \
                      (((dst) & ~0x00010000) | (((u32)(src)<<16) & 0x00010000))
/*	 Fields usb1_vbusvldextsel	 */
#define USB1_VBUSVLDEXTSEL_WIDTH                                              1
#define USB1_VBUSVLDEXTSEL_SHIFT                                             15
#define USB1_VBUSVLDEXTSEL_MASK                                      0x00008000
#define USB1_VBUSVLDEXTSEL_RD(src)                   (((src) & 0x00008000)>>15)
#define USB1_VBUSVLDEXTSEL_WR(src)              (((u32)(src)<<15) & 0x00008000)
#define USB1_VBUSVLDEXTSEL_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))
/*	 Fields usb1_dppulldown	 */
#define USB1_DPPULLDOWN_WIDTH                                                 1
#define USB1_DPPULLDOWN_SHIFT                                                14
#define USB1_DPPULLDOWN_MASK                                         0x00004000
#define USB1_DPPULLDOWN_RD(src)                      (((src) & 0x00004000)>>14)
#define USB1_DPPULLDOWN_WR(src)                 (((u32)(src)<<14) & 0x00004000)
#define USB1_DPPULLDOWN_SET(dst,src) \
                      (((dst) & ~0x00004000) | (((u32)(src)<<14) & 0x00004000))
/*	 Fields usb1_dmpulldown	 */
#define USB1_DMPULLDOWN_WIDTH                                                 1
#define USB1_DMPULLDOWN_SHIFT                                                13
#define USB1_DMPULLDOWN_MASK                                         0x00002000
#define USB1_DMPULLDOWN_RD(src)                      (((src) & 0x00002000)>>13)
#define USB1_DMPULLDOWN_WR(src)                 (((u32)(src)<<13) & 0x00002000)
#define USB1_DMPULLDOWN_SET(dst,src) \
                      (((dst) & ~0x00002000) | (((u32)(src)<<13) & 0x00002000))
/*	 Fields usb1_txbitstuffen	 */
#define USB1_TXBITSTUFFEN_WIDTH                                               1
#define USB1_TXBITSTUFFEN_SHIFT                                              12
#define USB1_TXBITSTUFFEN_MASK                                       0x00001000
#define USB1_TXBITSTUFFEN_RD(src)                    (((src) & 0x00001000)>>12)
#define USB1_TXBITSTUFFEN_WR(src)               (((u32)(src)<<12) & 0x00001000)
#define USB1_TXBITSTUFFEN_SET(dst,src) \
                      (((dst) & ~0x00001000) | (((u32)(src)<<12) & 0x00001000))
/*	 Fields usb1_txbitstuffenh	 */
#define USB1_TXBITSTUFFENH_WIDTH                                              1
#define USB1_TXBITSTUFFENH_SHIFT                                             11
#define USB1_TXBITSTUFFENH_MASK                                      0x00000800
#define USB1_TXBITSTUFFENH_RD(src)                   (((src) & 0x00000800)>>11)
#define USB1_TXBITSTUFFENH_WR(src)              (((u32)(src)<<11) & 0x00000800)
#define USB1_TXBITSTUFFENH_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*	 Fields usb1_idpullup	 */
#define USB1_IDPULLUP_WIDTH                                                   1
#define USB1_IDPULLUP_SHIFT                                                  10
#define USB1_IDPULLUP_MASK                                           0x00000400
#define USB1_IDPULLUP_RD(src)                        (((src) & 0x00000400)>>10)
#define USB1_IDPULLUP_WR(src)                   (((u32)(src)<<10) & 0x00000400)
#define USB1_IDPULLUP_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields usb1_chrgvbus	 */
#define USB1_CHRGVBUS_WIDTH                                                   1
#define USB1_CHRGVBUS_SHIFT                                                   9
#define USB1_CHRGVBUS_MASK                                           0x00000200
#define USB1_CHRGVBUS_RD(src)                         (((src) & 0x00000200)>>9)
#define USB1_CHRGVBUS_WR(src)                    (((u32)(src)<<9) & 0x00000200)
#define USB1_CHRGVBUS_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields usb1_dischrgvbus	 */
#define USB1_DISCHRGVBUS_WIDTH                                                1
#define USB1_DISCHRGVBUS_SHIFT                                                8
#define USB1_DISCHRGVBUS_MASK                                        0x00000100
#define USB1_DISCHRGVBUS_RD(src)                      (((src) & 0x00000100)>>8)
#define USB1_DISCHRGVBUS_WR(src)                 (((u32)(src)<<8) & 0x00000100)
#define USB1_DISCHRGVBUS_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*	 Fields usb1_drvvbus	 */
#define USB1_DRVVBUS_WIDTH                                                    1
#define USB1_DRVVBUS_SHIFT                                                    7
#define USB1_DRVVBUS_MASK                                            0x00000080
#define USB1_DRVVBUS_RD(src)                          (((src) & 0x00000080)>>7)
#define USB1_DRVVBUS_WR(src)                     (((u32)(src)<<7) & 0x00000080)
#define USB1_DRVVBUS_SET(dst,src) \
                       (((dst) & ~0x00000080) | (((u32)(src)<<7) & 0x00000080))
/*	 Fields usb1_sleepm	 */
#define USB1_SLEEPM_WIDTH                                                     1
#define USB1_SLEEPM_SHIFT                                                     6
#define USB1_SLEEPM_MASK                                             0x00000040
#define USB1_SLEEPM_RD(src)                           (((src) & 0x00000040)>>6)
#define USB1_SLEEPM_WR(src)                      (((u32)(src)<<6) & 0x00000040)
#define USB1_SLEEPM_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*	 Fields usb1_biastune	 */
#define USB1_BIASTUNE_WIDTH                                                   1
#define USB1_BIASTUNE_SHIFT                                                   5
#define USB1_BIASTUNE_MASK                                           0x00000020
#define USB1_BIASTUNE_RD(src)                         (((src) & 0x00000020)>>5)
#define USB1_BIASTUNE_WR(src)                    (((u32)(src)<<5) & 0x00000020)
#define USB1_BIASTUNE_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields usb1_plltune	 */
#define USB1_PLLTUNE_WIDTH                                                    1
#define USB1_PLLTUNE_SHIFT                                                    4
#define USB1_PLLTUNE_MASK                                            0x00000010
#define USB1_PLLTUNE_RD(src)                          (((src) & 0x00000010)>>4)
#define USB1_PLLTUNE_WR(src)                     (((u32)(src)<<4) & 0x00000010)
#define USB1_PLLTUNE_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields usb1_clkcore	 */
#define USB1_CLKCORE_WIDTH                                                    1
#define USB1_CLKCORE_SHIFT                                                    3
#define USB1_CLKCORE_MASK                                            0x00000008
#define USB1_CLKCORE_RD(src)                          (((src) & 0x00000008)>>3)
#define USB1_CLKCORE_WR(src)                     (((u32)(src)<<3) & 0x00000008)
#define USB1_CLKCORE_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields usb1_commononn	 */
#define USB1_COMMONONN_WIDTH                                                  1
#define USB1_COMMONONN_SHIFT                                                  2
#define USB1_COMMONONN_MASK                                          0x00000004
#define USB1_COMMONONN_RD(src)                        (((src) & 0x00000004)>>2)
#define USB1_COMMONONN_WR(src)                   (((u32)(src)<<2) & 0x00000004)
#define USB1_COMMONONN_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields usb1_loopbackenb	 */
#define USB1_LOOPBACKENB_WIDTH                                                1
#define USB1_LOOPBACKENB_SHIFT                                                1
#define USB1_LOOPBACKENB_MASK                                        0x00000002
#define USB1_LOOPBACKENB_RD(src)                      (((src) & 0x00000002)>>1)
#define USB1_LOOPBACKENB_WR(src)                 (((u32)(src)<<1) & 0x00000002)
#define USB1_LOOPBACKENB_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields usb1_vregtune	 */
#define USB1_VREGTUNE_WIDTH                                                   1
#define USB1_VREGTUNE_SHIFT                                                   0
#define USB1_VREGTUNE_MASK                                           0x00000001
#define USB1_VREGTUNE_RD(src)                            (((src) & 0x00000001))
#define USB1_VREGTUNE_WR(src)                       (((u32)(src)) & 0x00000001)
#define USB1_VREGTUNE_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register CFG_USB_HOST_PHY2_CONNECT	*/ 
/*	 Fields usb2_vbusvldext	 */
#define USB2_VBUSVLDEXT_WIDTH                                                 1
#define USB2_VBUSVLDEXT_SHIFT                                                16
#define USB2_VBUSVLDEXT_MASK                                         0x00010000
#define USB2_VBUSVLDEXT_RD(src)                      (((src) & 0x00010000)>>16)
#define USB2_VBUSVLDEXT_WR(src)                 (((u32)(src)<<16) & 0x00010000)
#define USB2_VBUSVLDEXT_SET(dst,src) \
                      (((dst) & ~0x00010000) | (((u32)(src)<<16) & 0x00010000))
/*	 Fields usb2_vbusvldextsel	 */
#define USB2_VBUSVLDEXTSEL_WIDTH                                              1
#define USB2_VBUSVLDEXTSEL_SHIFT                                             15
#define USB2_VBUSVLDEXTSEL_MASK                                      0x00008000
#define USB2_VBUSVLDEXTSEL_RD(src)                   (((src) & 0x00008000)>>15)
#define USB2_VBUSVLDEXTSEL_WR(src)              (((u32)(src)<<15) & 0x00008000)
#define USB2_VBUSVLDEXTSEL_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))
/*	 Fields usb2_dppulldown	 */
#define USB2_DPPULLDOWN_WIDTH                                                 1
#define USB2_DPPULLDOWN_SHIFT                                                14
#define USB2_DPPULLDOWN_MASK                                         0x00004000
#define USB2_DPPULLDOWN_RD(src)                      (((src) & 0x00004000)>>14)
#define USB2_DPPULLDOWN_WR(src)                 (((u32)(src)<<14) & 0x00004000)
#define USB2_DPPULLDOWN_SET(dst,src) \
                      (((dst) & ~0x00004000) | (((u32)(src)<<14) & 0x00004000))
/*	 Fields usb2_dmpulldown	 */
#define USB2_DMPULLDOWN_WIDTH                                                 1
#define USB2_DMPULLDOWN_SHIFT                                                13
#define USB2_DMPULLDOWN_MASK                                         0x00002000
#define USB2_DMPULLDOWN_RD(src)                      (((src) & 0x00002000)>>13)
#define USB2_DMPULLDOWN_WR(src)                 (((u32)(src)<<13) & 0x00002000)
#define USB2_DMPULLDOWN_SET(dst,src) \
                      (((dst) & ~0x00002000) | (((u32)(src)<<13) & 0x00002000))
/*	 Fields usb2_txbitstuffen	 */
#define USB2_TXBITSTUFFEN_WIDTH                                               1
#define USB2_TXBITSTUFFEN_SHIFT                                              12
#define USB2_TXBITSTUFFEN_MASK                                       0x00001000
#define USB2_TXBITSTUFFEN_RD(src)                    (((src) & 0x00001000)>>12)
#define USB2_TXBITSTUFFEN_WR(src)               (((u32)(src)<<12) & 0x00001000)
#define USB2_TXBITSTUFFEN_SET(dst,src) \
                      (((dst) & ~0x00001000) | (((u32)(src)<<12) & 0x00001000))
/*	 Fields usb2_txbitstuffenh	 */
#define USB2_TXBITSTUFFENH_WIDTH                                              1
#define USB2_TXBITSTUFFENH_SHIFT                                             11
#define USB2_TXBITSTUFFENH_MASK                                      0x00000800
#define USB2_TXBITSTUFFENH_RD(src)                   (((src) & 0x00000800)>>11)
#define USB2_TXBITSTUFFENH_WR(src)              (((u32)(src)<<11) & 0x00000800)
#define USB2_TXBITSTUFFENH_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*	 Fields usb2_idpullup	 */
#define USB2_IDPULLUP_WIDTH                                                   1
#define USB2_IDPULLUP_SHIFT                                                  10
#define USB2_IDPULLUP_MASK                                           0x00000400
#define USB2_IDPULLUP_RD(src)                        (((src) & 0x00000400)>>10)
#define USB2_IDPULLUP_WR(src)                   (((u32)(src)<<10) & 0x00000400)
#define USB2_IDPULLUP_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields usb2_chrgvbus	 */
#define USB2_CHRGVBUS_WIDTH                                                   1
#define USB2_CHRGVBUS_SHIFT                                                   9
#define USB2_CHRGVBUS_MASK                                           0x00000200
#define USB2_CHRGVBUS_RD(src)                         (((src) & 0x00000200)>>9)
#define USB2_CHRGVBUS_WR(src)                    (((u32)(src)<<9) & 0x00000200)
#define USB2_CHRGVBUS_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields usb2_dischrgvbus	 */
#define USB2_DISCHRGVBUS_WIDTH                                                1
#define USB2_DISCHRGVBUS_SHIFT                                                8
#define USB2_DISCHRGVBUS_MASK                                        0x00000100
#define USB2_DISCHRGVBUS_RD(src)                      (((src) & 0x00000100)>>8)
#define USB2_DISCHRGVBUS_WR(src)                 (((u32)(src)<<8) & 0x00000100)
#define USB2_DISCHRGVBUS_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*	 Fields usb2_drvvbus	 */
#define USB2_DRVVBUS_WIDTH                                                    1
#define USB2_DRVVBUS_SHIFT                                                    7
#define USB2_DRVVBUS_MASK                                            0x00000080
#define USB2_DRVVBUS_RD(src)                          (((src) & 0x00000080)>>7)
#define USB2_DRVVBUS_WR(src)                     (((u32)(src)<<7) & 0x00000080)
#define USB2_DRVVBUS_SET(dst,src) \
                       (((dst) & ~0x00000080) | (((u32)(src)<<7) & 0x00000080))
/*	 Fields usb2_sleepm	 */
#define USB2_SLEEPM_WIDTH                                                     1
#define USB2_SLEEPM_SHIFT                                                     6
#define USB2_SLEEPM_MASK                                             0x00000040
#define USB2_SLEEPM_RD(src)                           (((src) & 0x00000040)>>6)
#define USB2_SLEEPM_WR(src)                      (((u32)(src)<<6) & 0x00000040)
#define USB2_SLEEPM_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*	 Fields usb2_biastune	 */
#define USB2_BIASTUNE_WIDTH                                                   1
#define USB2_BIASTUNE_SHIFT                                                   5
#define USB2_BIASTUNE_MASK                                           0x00000020
#define USB2_BIASTUNE_RD(src)                         (((src) & 0x00000020)>>5)
#define USB2_BIASTUNE_WR(src)                    (((u32)(src)<<5) & 0x00000020)
#define USB2_BIASTUNE_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields usb2_plltune	 */
#define USB2_PLLTUNE_WIDTH                                                    1
#define USB2_PLLTUNE_SHIFT                                                    4
#define USB2_PLLTUNE_MASK                                            0x00000010
#define USB2_PLLTUNE_RD(src)                          (((src) & 0x00000010)>>4)
#define USB2_PLLTUNE_WR(src)                     (((u32)(src)<<4) & 0x00000010)
#define USB2_PLLTUNE_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields usb2_clkcore	 */
#define USB2_CLKCORE_WIDTH                                                    1
#define USB2_CLKCORE_SHIFT                                                    3
#define USB2_CLKCORE_MASK                                            0x00000008
#define USB2_CLKCORE_RD(src)                          (((src) & 0x00000008)>>3)
#define USB2_CLKCORE_WR(src)                     (((u32)(src)<<3) & 0x00000008)
#define USB2_CLKCORE_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields usb2_commononn	 */
#define USB2_COMMONONN_WIDTH                                                  1
#define USB2_COMMONONN_SHIFT                                                  2
#define USB2_COMMONONN_MASK                                          0x00000004
#define USB2_COMMONONN_RD(src)                        (((src) & 0x00000004)>>2)
#define USB2_COMMONONN_WR(src)                   (((u32)(src)<<2) & 0x00000004)
#define USB2_COMMONONN_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields usb2_loopbackenb	 */
#define USB2_LOOPBACKENB_WIDTH                                                1
#define USB2_LOOPBACKENB_SHIFT                                                1
#define USB2_LOOPBACKENB_MASK                                        0x00000002
#define USB2_LOOPBACKENB_RD(src)                      (((src) & 0x00000002)>>1)
#define USB2_LOOPBACKENB_WR(src)                 (((u32)(src)<<1) & 0x00000002)
#define USB2_LOOPBACKENB_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields usb2_vregtune	 */
#define USB2_VREGTUNE_WIDTH                                                   1
#define USB2_VREGTUNE_SHIFT                                                   0
#define USB2_VREGTUNE_MASK                                           0x00000001
#define USB2_VREGTUNE_RD(src)                            (((src) & 0x00000001))
#define USB2_VREGTUNE_WR(src)                       (((u32)(src)) & 0x00000001)
#define USB2_VREGTUNE_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register CFG_AUTO_RD_NAND_DEV	*/ 
/*	 Fields NFLASH_ADDR_CYCLE	 */
#define NFLASH_ADDR_CYCLE_WIDTH                                               2
#define NFLASH_ADDR_CYCLE_SHIFT                                              28
#define NFLASH_ADDR_CYCLE_MASK                                       0x30000000
#define NFLASH_ADDR_CYCLE_RD(src)                    (((src) & 0x30000000)>>28)
#define NFLASH_ADDR_CYCLE_WR(src)               (((u32)(src)<<28) & 0x30000000)
#define NFLASH_ADDR_CYCLE_SET(dst,src) \
                      (((dst) & ~0x30000000) | (((u32)(src)<<28) & 0x30000000))
/*	 Fields NFLASH_DEV_WIDTH	 */
#define NFLASH_DEV_WIDTH_WIDTH                                                1
#define NFLASH_DEV_WIDTH_SHIFT                                               27
#define NFLASH_DEV_WIDTH_MASK                                        0x08000000
#define NFLASH_DEV_WIDTH_RD(src)                     (((src) & 0x08000000)>>27)
#define NFLASH_DEV_WIDTH_WR(src)                (((u32)(src)<<27) & 0x08000000)
#define NFLASH_DEV_WIDTH_SET(dst,src) \
                      (((dst) & ~0x08000000) | (((u32)(src)<<27) & 0x08000000))
/*	 Fields NFLASH_ECC_ON_OFF	 */
#define NFLASH_ECC_ON_OFF_WIDTH                                               1
#define NFLASH_ECC_ON_OFF_SHIFT                                              26
#define NFLASH_ECC_ON_OFF_MASK                                       0x04000000
#define NFLASH_ECC_ON_OFF_RD(src)                    (((src) & 0x04000000)>>26)
#define NFLASH_ECC_ON_OFF_WR(src)               (((u32)(src)<<26) & 0x04000000)
#define NFLASH_ECC_ON_OFF_SET(dst,src) \
                      (((dst) & ~0x04000000) | (((u32)(src)<<26) & 0x04000000))
/*	 Fields NFLASH_ECC_SPARE_OFF	 */
#define NFLASH_ECC_SPARE_OFF_WIDTH                                            8
#define NFLASH_ECC_SPARE_OFF_SHIFT                                           18
#define NFLASH_ECC_SPARE_OFF_MASK                                    0x03fc0000
#define NFLASH_ECC_SPARE_OFF_RD(src)                 (((src) & 0x03fc0000)>>18)
#define NFLASH_ECC_SPARE_OFF_WR(src)            (((u32)(src)<<18) & 0x03fc0000)
#define NFLASH_ECC_SPARE_OFF_SET(dst,src) \
                      (((dst) & ~0x03fc0000) | (((u32)(src)<<18) & 0x03fc0000))
/*	 Fields NFLASH_RDYBSY_EN	 */
#define NFLASH_RDYBSY_EN_WIDTH                                                1
#define NFLASH_RDYBSY_EN_SHIFT                                               17
#define NFLASH_RDYBSY_EN_MASK                                        0x00020000
#define NFLASH_RDYBSY_EN_RD(src)                     (((src) & 0x00020000)>>17)
#define NFLASH_RDYBSY_EN_WR(src)                (((u32)(src)<<17) & 0x00020000)
#define NFLASH_RDYBSY_EN_SET(dst,src) \
                      (((dst) & ~0x00020000) | (((u32)(src)<<17) & 0x00020000))
/*	 Fields NFLASH_PAGE_SIZE_4K	 */
#define NFLASH_PAGE_SIZE_4K_WIDTH                                             1
#define NFLASH_PAGE_SIZE_4K_SHIFT                                            16
#define NFLASH_PAGE_SIZE_4K_MASK                                     0x00010000
#define NFLASH_PAGE_SIZE_4K_RD(src)                  (((src) & 0x00010000)>>16)
#define NFLASH_PAGE_SIZE_4K_WR(src)             (((u32)(src)<<16) & 0x00010000)
#define NFLASH_PAGE_SIZE_4K_SET(dst,src) \
                      (((dst) & ~0x00010000) | (((u32)(src)<<16) & 0x00010000))
/*	 Fields NFLASH_DEV_READ_CNT	 */
#define NFLASH_DEV_READ_CNT_WIDTH                                            16
#define NFLASH_DEV_READ_CNT_SHIFT                                             0
#define NFLASH_DEV_READ_CNT_MASK                                     0x0000ffff
#define NFLASH_DEV_READ_CNT_RD(src)                      (((src) & 0x0000ffff))
#define NFLASH_DEV_READ_CNT_WR(src)                 (((u32)(src)) & 0x0000ffff)
#define NFLASH_DEV_READ_CNT_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register CFG_AHBC_CLK_DET	*/ 
/*	 Fields TDM_CLK_DET	 */
#define TDM_CLK_DET_WIDTH                                                     1
#define TDM_CLK_DET_SHIFT                                                     1
#define TDM_CLK_DET_MASK                                             0x00000002
#define TDM_CLK_DET_RD(src)                           (((src) & 0x00000002)>>1)
#define TDM_CLK_DET_WR(src)                      (((u32)(src)<<1) & 0x00000002)
#define TDM_CLK_DET_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields USBOTG_CLK_DET	 */
#define USBOTG_CLK_DET_WIDTH                                                  1
#define USBOTG_CLK_DET_SHIFT                                                  0
#define USBOTG_CLK_DET_MASK                                          0x00000001
#define USBOTG_CLK_DET_RD(src)                           (((src) & 0x00000001))
#define USBOTG_CLK_DET_WR(src)                      (((u32)(src)) & 0x00000001)
#define USBOTG_CLK_DET_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

#endif /* defined(CONFIG_APM862xx) */

/*****************************************************************************
 * APM86xxx AHBC Diagnostic CSR Register Definition 
 *****************************************************************************
 */

/*	Global Base Address	*/
#define AHBC_GLBL_DIAG_CSR_AHBC_REG_BASE_ADDR			0xddd867000ULL

/*    Address GLBL_DIAG_CSR_AHBC_REG  Registers */
#define AHBC_CFG_DIAG_SEL_ADDR                                       0x00000000
#define AHBC_CFG_DIAG_SEL_DEFAULT                                    0x00000000
#define AHBC_CFG_READ_BW_LAT_ADDR_MASK_ADDR                          0x00000004
#define AHBC_CFG_READ_BW_LAT_ADDR_MASK_DEFAULT                       0x00000000
#define AHBC_CFG_READ_BW_LAT_ADDR_PAT_ADDR                           0x00000008
#define AHBC_CFG_READ_BW_LAT_ADDR_PAT_DEFAULT                        0xffffffff
#define AHBC_CFG_WRITE_BW_LAT_ADDR_MASK_ADDR                         0x0000000c
#define AHBC_CFG_WRITE_BW_LAT_ADDR_MASK_DEFAULT                      0x00000000
#define AHBC_CFG_WRITE_BW_LAT_ADDR_PAT_ADDR                          0x00000010
#define AHBC_CFG_WRITE_BW_LAT_ADDR_PAT_DEFAULT                       0xffffffff
#define AHBC_CFG_DIAG_START_STOP_ADDR                                0x00000014
#define AHBC_CFG_DIAG_START_STOP_DEFAULT                             0x0000001f
#define AHBC_CFG_BW_MSTR_STOP_CNT_ADDR                               0x00000018
#define AHBC_CFG_BW_MSTR_STOP_CNT_DEFAULT                            0x00040004
#define AHBC_CFG_BW_SLV_STOP_CNT_ADDR                                0x0000001c
#define AHBC_CFG_BW_SLV_STOP_CNT_DEFAULT                             0x00040004
#define AHBC_STS_READ_LATENCY_OUTPUT_ADDR                            0x00000020
#define AHBC_STS_READ_LATENCY_OUTPUT_DEFAULT                         0x00000000
#define AHBC_STS_AXI_MRD_BW_CLK_CNT_ADDR                             0x00000024
#define AHBC_STS_AXI_MRD_BW_CLK_CNT_DEFAULT                          0x00000000
#define AHBC_STS_AXI_MRD_BW_BYTE_CNT_ADDR                            0x00000028
#define AHBC_STS_AXI_MRD_BW_BYTE_CNT_DEFAULT                         0x00000000
#define AHBC_STS_AXI_MWR_BW_CLK_CNT_ADDR                             0x0000002c
#define AHBC_STS_AXI_MWR_BW_CLK_CNT_DEFAULT                          0x00000000
#define AHBC_STS_AXI_MWR_BW_BYTE_CNT_ADDR                            0x00000030
#define AHBC_STS_AXI_MWR_BW_BYTE_CNT_DEFAULT                         0x00000000
#define AHBC_STS_AXI_SRD_BW_CLK_CNT_ADDR                             0x00000034
#define AHBC_STS_AXI_SRD_BW_CLK_CNT_DEFAULT                          0x00000000
#define AHBC_STS_AXI_SRD_BW_BYTE_CNT_ADDR                            0x00000038
#define AHBC_STS_AXI_SRD_BW_BYTE_CNT_DEFAULT                         0x00000000
#define AHBC_STS_AXI_SWR_BW_CLK_CNT_ADDR                             0x0000003c
#define AHBC_STS_AXI_SWR_BW_CLK_CNT_DEFAULT                          0x00000000
#define AHBC_STS_AXI_SWR_BW_BYTE_CNT_ADDR                            0x00000040
#define AHBC_STS_AXI_SWR_BW_BYTE_CNT_DEFAULT                         0x00000000
#define AHBC_CFG_DBG_TRIG_CTRL_ADDR                                  0x00000044
#define AHBC_CFG_DBG_TRIG_CTRL_DEFAULT                               0x00000000
#define AHBC_CFG_DBG_PAT_REG_0_ADDR                                  0x00000048
#define AHBC_CFG_DBG_PAT_REG_0_DEFAULT                               0x00000000
#define AHBC_CFG_DBG_PAT_MASK_REG_0_ADDR                             0x0000004c
#define AHBC_CFG_DBG_PAT_MASK_REG_0_DEFAULT                          0x00000000
#define AHBC_CFG_DBG_PAT_REG_1_ADDR                                  0x00000050
#define AHBC_CFG_DBG_PAT_REG_1_DEFAULT                               0x00000000
#define AHBC_CFG_DBG_PAT_MASK_REG_1_ADDR                             0x00000054
#define AHBC_CFG_DBG_PAT_MASK_REG_1_DEFAULT                          0x00000000
#define AHBC_DBG_TRIG_OUT_ADDR                                       0x00000058
#define AHBC_DBG_TRIG_OUT_DEFAULT                                    0x00000000
#define AHBC_DBG_ECC_INT_OUT_ADDR                                    0x0000005c
#define AHBC_DBG_ECC_INT_OUT_DEFAULT                                 0x00000000
#define AHBC_DBG_ECC_INT_OUTMASK_ADDR                                0x00000060
#define AHBC_CFG_SECC_INT_THRSH_REG_ADDR                             0x00000064
#define AHBC_CFG_SECC_INT_THRSH_REG_DEFAULT                          0x000000fe
#define AHBC_CFG_MEM_ECC_BYPASS_ADDR                                 0x00000068
#define AHBC_CFG_MEM_ECC_BYPASS_DEFAULT                              0x00000000
#define AHBC_CFG_MEM_PWRDN_DIS_ADDR                                  0x0000006c
#define AHBC_CFG_MEM_PWRDN_DIS_DEFAULT                               0x00000000
#define AHBC_CFG_MEM_RAM_SHUTDOWN_ADDR                               0x00000070
#define AHBC_CFG_MEM_RAM_SHUTDOWN_DEFAULT                            0xffffffff
#define AHBC_BLOCK_MEM_RDY_ADDR                                      0x00000074
#define AHBC_BLOCK_MEM_RDY_DEFAULT                                   0xffffffff
#define AHBC_CFG_AXI_SLV_RD_TMO_ADDR                                 0x00000078
#define AHBC_CFG_AXI_SLV_RD_TMO_DEFAULT                              0x00000000
#define AHBC_CFG_AXI_SLV_WR_TMO_ADDR                                 0x0000007c
#define AHBC_CFG_AXI_SLV_WR_TMO_DEFAULT                              0x00000000
#define AHBC_STS_AXI_SLV_TMO_ADDR                                    0x00000080
#define AHBC_STS_AXI_SLV_TMO_DEFAULT                                 0x00000000
#define AHBC_CFG_AXI_SS_CSR_TMO_ADDR                                 0x00000084
#define AHBC_CFG_AXI_SS_CSR_TMO_DEFAULT                              0x02008000
#define AHBC_STS_READ_LATENCY_TOT_READ_REQS_ADDR                     0x0000008c
#define AHBC_STS_READ_LATENCY_TOT_READ_REQS_DEFAULT                  0x00000000

/*	Register CFG_DIAG_SEL	*/ 
/*	 Fields CFG_AXI_NON_AXI_MUX_SELECT	 */
#define AHBC_CFG_AXI_NON_AXI_MUX_SELECT_WIDTH                                 1
#define AHBC_CFG_AXI_NON_AXI_MUX_SELECT_SHIFT                                11
#define AHBC_CFG_AXI_NON_AXI_MUX_SELECT_MASK                         0x00000800
#define AHBC_CFG_AXI_NON_AXI_MUX_SELECT_RD(src)      (((src) & 0x00000800)>>11)
#define AHBC_CFG_AXI_NON_AXI_MUX_SELECT_WR(src) \
                                                (((u32)(src)<<11) & 0x00000800)
#define AHBC_CFG_AXI_NON_AXI_MUX_SELECT_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*	 Fields CFG_MUX_SELECTOR	 */
#define AHBC_CFG_MUX_SELECTOR_WIDTH                                          11
#define AHBC_CFG_MUX_SELECTOR_SHIFT                                           0
#define AHBC_CFG_MUX_SELECTOR_MASK                                   0x000007ff
#define AHBC_CFG_MUX_SELECTOR_RD(src)                    (((src) & 0x000007ff))
#define AHBC_CFG_MUX_SELECTOR_WR(src)               (((u32)(src)) & 0x000007ff)
#define AHBC_CFG_MUX_SELECTOR_SET(dst,src) \
                          (((dst) & ~0x000007ff) | (((u32)(src)) & 0x000007ff))

/*	Register CFG_READ_BW_LAT_ADDR_MASK	*/ 
/*	 Fields READ_ADDR_MASK	 */
#define AHBC_READ_ADDR_MASK_WIDTH                                            32
#define AHBC_READ_ADDR_MASK_SHIFT                                             0
#define AHBC_READ_ADDR_MASK_MASK                                     0xffffffff
#define AHBC_READ_ADDR_MASK_RD(src)                      (((src) & 0xffffffff))
#define AHBC_READ_ADDR_MASK_WR(src)                 (((u32)(src)) & 0xffffffff)
#define AHBC_READ_ADDR_MASK_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_READ_BW_LAT_ADDR_PAT	*/ 
/*	 Fields READ_ADDR_PAT	 */
#define AHBC_READ_ADDR_PAT_WIDTH                                             32
#define AHBC_READ_ADDR_PAT_SHIFT                                              0
#define AHBC_READ_ADDR_PAT_MASK                                      0xffffffff
#define AHBC_READ_ADDR_PAT_RD(src)                       (((src) & 0xffffffff))
#define AHBC_READ_ADDR_PAT_WR(src)                  (((u32)(src)) & 0xffffffff)
#define AHBC_READ_ADDR_PAT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_WRITE_BW_LAT_ADDR_MASK	*/ 
/*	 Fields WRITE_ADDR_MASK	 */
#define AHBC_WRITE_ADDR_MASK_WIDTH                                           32
#define AHBC_WRITE_ADDR_MASK_SHIFT                                            0
#define AHBC_WRITE_ADDR_MASK_MASK                                    0xffffffff
#define AHBC_WRITE_ADDR_MASK_RD(src)                     (((src) & 0xffffffff))
#define AHBC_WRITE_ADDR_MASK_WR(src)                (((u32)(src)) & 0xffffffff)
#define AHBC_WRITE_ADDR_MASK_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_WRITE_BW_LAT_ADDR_PAT	*/ 
/*	 Fields WRITE_ADDR_PAT	 */
#define AHBC_WRITE_ADDR_PAT_WIDTH                                            32
#define AHBC_WRITE_ADDR_PAT_SHIFT                                             0
#define AHBC_WRITE_ADDR_PAT_MASK                                     0xffffffff
#define AHBC_WRITE_ADDR_PAT_RD(src)                      (((src) & 0xffffffff))
#define AHBC_WRITE_ADDR_PAT_WR(src)                 (((u32)(src)) & 0xffffffff)
#define AHBC_WRITE_ADDR_PAT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_DIAG_START_STOP	*/ 
/*	 Fields START_RD_LATENCY	 */
#define AHBC_START_RD_LATENCY_WIDTH                                           1
#define AHBC_START_RD_LATENCY_SHIFT                                           4
#define AHBC_START_RD_LATENCY_MASK                                   0x00000010
#define AHBC_START_RD_LATENCY_RD(src)                 (((src) & 0x00000010)>>4)
#define AHBC_START_RD_LATENCY_WR(src)            (((u32)(src)<<4) & 0x00000010)
#define AHBC_START_RD_LATENCY_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields START_SRD_BW	 */
#define AHBC_START_SRD_BW_WIDTH                                               1
#define AHBC_START_SRD_BW_SHIFT                                               3
#define AHBC_START_SRD_BW_MASK                                       0x00000008
#define AHBC_START_SRD_BW_RD(src)                     (((src) & 0x00000008)>>3)
#define AHBC_START_SRD_BW_WR(src)                (((u32)(src)<<3) & 0x00000008)
#define AHBC_START_SRD_BW_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields START_MRD_BW	 */
#define AHBC_START_MRD_BW_WIDTH                                               1
#define AHBC_START_MRD_BW_SHIFT                                               2
#define AHBC_START_MRD_BW_MASK                                       0x00000004
#define AHBC_START_MRD_BW_RD(src)                     (((src) & 0x00000004)>>2)
#define AHBC_START_MRD_BW_WR(src)                (((u32)(src)<<2) & 0x00000004)
#define AHBC_START_MRD_BW_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields START_SWR_BW	 */
#define AHBC_START_SWR_BW_WIDTH                                               1
#define AHBC_START_SWR_BW_SHIFT                                               1
#define AHBC_START_SWR_BW_MASK                                       0x00000002
#define AHBC_START_SWR_BW_RD(src)                     (((src) & 0x00000002)>>1)
#define AHBC_START_SWR_BW_WR(src)                (((u32)(src)<<1) & 0x00000002)
#define AHBC_START_SWR_BW_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields START_MWR_BW	 */
#define AHBC_START_MWR_BW_WIDTH                                               1
#define AHBC_START_MWR_BW_SHIFT                                               0
#define AHBC_START_MWR_BW_MASK                                       0x00000001
#define AHBC_START_MWR_BW_RD(src)                        (((src) & 0x00000001))
#define AHBC_START_MWR_BW_WR(src)                   (((u32)(src)) & 0x00000001)
#define AHBC_START_MWR_BW_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register CFG_BW_MSTR_STOP_CNT	*/ 
/*	 Fields MSTR_STOP_RD_CNT	 */
#define AHBC_MSTR_STOP_RD_CNT_WIDTH                                          16
#define AHBC_MSTR_STOP_RD_CNT_SHIFT                                          16
#define AHBC_MSTR_STOP_RD_CNT_MASK                                   0xffff0000
#define AHBC_MSTR_STOP_RD_CNT_RD(src)                (((src) & 0xffff0000)>>16)
#define AHBC_MSTR_STOP_RD_CNT_WR(src)           (((u32)(src)<<16) & 0xffff0000)
#define AHBC_MSTR_STOP_RD_CNT_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields MSTR_STOP_WR_CNT	 */
#define AHBC_MSTR_STOP_WR_CNT_WIDTH                                          16
#define AHBC_MSTR_STOP_WR_CNT_SHIFT                                           0
#define AHBC_MSTR_STOP_WR_CNT_MASK                                   0x0000ffff
#define AHBC_MSTR_STOP_WR_CNT_RD(src)                    (((src) & 0x0000ffff))
#define AHBC_MSTR_STOP_WR_CNT_WR(src)               (((u32)(src)) & 0x0000ffff)
#define AHBC_MSTR_STOP_WR_CNT_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register CFG_BW_SLV_STOP_CNT	*/ 
/*	 Fields SLV_STOP_RD_CNT	 */
#define AHBC_SLV_STOP_RD_CNT_WIDTH                                           16
#define AHBC_SLV_STOP_RD_CNT_SHIFT                                           16
#define AHBC_SLV_STOP_RD_CNT_MASK                                    0xffff0000
#define AHBC_SLV_STOP_RD_CNT_RD(src)                 (((src) & 0xffff0000)>>16)
#define AHBC_SLV_STOP_RD_CNT_WR(src)            (((u32)(src)<<16) & 0xffff0000)
#define AHBC_SLV_STOP_RD_CNT_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields SLV_STOP_WR_CNT	 */
#define AHBC_SLV_STOP_WR_CNT_WIDTH                                           16
#define AHBC_SLV_STOP_WR_CNT_SHIFT                                            0
#define AHBC_SLV_STOP_WR_CNT_MASK                                    0x0000ffff
#define AHBC_SLV_STOP_WR_CNT_RD(src)                     (((src) & 0x0000ffff))
#define AHBC_SLV_STOP_WR_CNT_WR(src)                (((u32)(src)) & 0x0000ffff)
#define AHBC_SLV_STOP_WR_CNT_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register STS_READ_LATENCY_OUTPUT	*/ 
/*	 Fields READ_MAX	 */
#define AHBC_READ_MAX_WIDTH                                                  10
#define AHBC_READ_MAX_SHIFT                                                  22
#define AHBC_READ_MAX_MASK                                           0xffc00000
#define AHBC_READ_MAX_RD(src)                        (((src) & 0xffc00000)>>22)
#define AHBC_READ_MAX_SET(dst,src) \
                      (((dst) & ~0xffc00000) | (((u32)(src)<<22) & 0xffc00000))
/*	 Fields READ_TOT	 */
#define AHBC_READ_TOT_WIDTH                                                  22
#define AHBC_READ_TOT_SHIFT                                                   0
#define AHBC_READ_TOT_MASK                                           0x003fffff
#define AHBC_READ_TOT_RD(src)                            (((src) & 0x003fffff))
#define AHBC_READ_TOT_SET(dst,src) \
                          (((dst) & ~0x003fffff) | (((u32)(src)) & 0x003fffff))

/*	Register STS_AXI_MRD_BW_CLK_CNT	*/ 
/*	 Fields MSTR_READ_BW_CLK_CNT	 */
#define AHBC_MSTR_READ_BW_CLK_CNT_WIDTH                                      32
#define AHBC_MSTR_READ_BW_CLK_CNT_SHIFT                                       0
#define AHBC_MSTR_READ_BW_CLK_CNT_MASK                               0xffffffff
#define AHBC_MSTR_READ_BW_CLK_CNT_RD(src)                (((src) & 0xffffffff))
#define AHBC_MSTR_READ_BW_CLK_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register STS_AXI_MRD_BW_BYTE_CNT	*/ 
/*	 Fields MSTR_READ_BW_BYTE_CNT	 */
#define AHBC_MSTR_READ_BW_BYTE_CNT_WIDTH                                     32
#define AHBC_MSTR_READ_BW_BYTE_CNT_SHIFT                                      0
#define AHBC_MSTR_READ_BW_BYTE_CNT_MASK                              0xffffffff
#define AHBC_MSTR_READ_BW_BYTE_CNT_RD(src)               (((src) & 0xffffffff))
#define AHBC_MSTR_READ_BW_BYTE_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register STS_AXI_MWR_BW_CLK_CNT	*/ 
/*	 Fields MSTR_WRITE_BW_CLK_CNT	 */
#define AHBC_MSTR_WRITE_BW_CLK_CNT_WIDTH                                     32
#define AHBC_MSTR_WRITE_BW_CLK_CNT_SHIFT                                      0
#define AHBC_MSTR_WRITE_BW_CLK_CNT_MASK                              0xffffffff
#define AHBC_MSTR_WRITE_BW_CLK_CNT_RD(src)               (((src) & 0xffffffff))
#define AHBC_MSTR_WRITE_BW_CLK_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register STS_AXI_MWR_BW_BYTE_CNT	*/ 
/*	 Fields MSTR_WRITE_BW_BYTE_CNT	 */
#define AHBC_MSTR_WRITE_BW_BYTE_CNT_WIDTH                                    32
#define AHBC_MSTR_WRITE_BW_BYTE_CNT_SHIFT                                     0
#define AHBC_MSTR_WRITE_BW_BYTE_CNT_MASK                             0xffffffff
#define AHBC_MSTR_WRITE_BW_BYTE_CNT_RD(src)              (((src) & 0xffffffff))
#define AHBC_MSTR_WRITE_BW_BYTE_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register STS_AXI_SRD_BW_CLK_CNT	*/ 
/*	 Fields SLV_READ_BW_CLK_CNT	 */
#define AHBC_SLV_READ_BW_CLK_CNT_WIDTH                                       32
#define AHBC_SLV_READ_BW_CLK_CNT_SHIFT                                        0
#define AHBC_SLV_READ_BW_CLK_CNT_MASK                                0xffffffff
#define AHBC_SLV_READ_BW_CLK_CNT_RD(src)                 (((src) & 0xffffffff))
#define AHBC_SLV_READ_BW_CLK_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register STS_AXI_SRD_BW_BYTE_CNT	*/ 
/*	 Fields SLV_READ_BW_BYTE_CNT	 */
#define AHBC_SLV_READ_BW_BYTE_CNT_WIDTH                                      32
#define AHBC_SLV_READ_BW_BYTE_CNT_SHIFT                                       0
#define AHBC_SLV_READ_BW_BYTE_CNT_MASK                               0xffffffff
#define AHBC_SLV_READ_BW_BYTE_CNT_RD(src)                (((src) & 0xffffffff))
#define AHBC_SLV_READ_BW_BYTE_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register STS_AXI_SWR_BW_CLK_CNT	*/ 
/*	 Fields SLV_WRITE_BW_CLK_CNT	 */
#define AHBC_SLV_WRITE_BW_CLK_CNT_WIDTH                                      32
#define AHBC_SLV_WRITE_BW_CLK_CNT_SHIFT                                       0
#define AHBC_SLV_WRITE_BW_CLK_CNT_MASK                               0xffffffff
#define AHBC_SLV_WRITE_BW_CLK_CNT_RD(src)                (((src) & 0xffffffff))
#define AHBC_SLV_WRITE_BW_CLK_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register STS_AXI_SWR_BW_BYTE_CNT	*/ 
/*	 Fields SLV_WRITE_BW_BYTE_CNT	 */
#define AHBC_SLV_WRITE_BW_BYTE_CNT_WIDTH                                     32
#define AHBC_SLV_WRITE_BW_BYTE_CNT_SHIFT                                      0
#define AHBC_SLV_WRITE_BW_BYTE_CNT_MASK                              0xffffffff
#define AHBC_SLV_WRITE_BW_BYTE_CNT_RD(src)               (((src) & 0xffffffff))
#define AHBC_SLV_WRITE_BW_BYTE_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_DBG_TRIG_CTRL	*/ 
/*	 Fields TRIG_EN	 */
#define AHBC_TRIG_EN_WIDTH                                                    1
#define AHBC_TRIG_EN_SHIFT                                                    4
#define AHBC_TRIG_EN_MASK                                            0x00000010
#define AHBC_TRIG_EN_RD(src)                          (((src) & 0x00000010)>>4)
#define AHBC_TRIG_EN_WR(src)                     (((u32)(src)<<4) & 0x00000010)
#define AHBC_TRIG_EN_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields AND_E	 */
#define AHBC_AND_E_WIDTH                                                      2
#define AHBC_AND_E_SHIFT                                                      2
#define AHBC_AND_E_MASK                                              0x0000000c
#define AHBC_AND_E_RD(src)                            (((src) & 0x0000000c)>>2)
#define AHBC_AND_E_WR(src)                       (((u32)(src)<<2) & 0x0000000c)
#define AHBC_AND_E_SET(dst,src) \
                       (((dst) & ~0x0000000c) | (((u32)(src)<<2) & 0x0000000c))
/*	 Fields OR_E	 */
#define AHBC_OR_E_WIDTH                                                       2
#define AHBC_OR_E_SHIFT                                                       0
#define AHBC_OR_E_MASK                                               0x00000003
#define AHBC_OR_E_RD(src)                                (((src) & 0x00000003))
#define AHBC_OR_E_WR(src)                           (((u32)(src)) & 0x00000003)
#define AHBC_OR_E_SET(dst,src) \
                          (((dst) & ~0x00000003) | (((u32)(src)) & 0x00000003))

/*	Register CFG_DBG_PAT_REG_0	*/ 
/*	 Fields PATTERN	 */
#define AHBC_PATTERN0_WIDTH                                                  32
#define AHBC_PATTERN0_SHIFT                                                   0
#define AHBC_PATTERN0_MASK                                           0xffffffff
#define AHBC_PATTERN0_RD(src)                            (((src) & 0xffffffff))
#define AHBC_PATTERN0_WR(src)                       (((u32)(src)) & 0xffffffff)
#define AHBC_PATTERN0_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_DBG_PAT_MASK_REG_0	*/ 
/*	 Fields PAT_MASK	 */
#define AHBC_PAT_MASK0_WIDTH                                                 32
#define AHBC_PAT_MASK0_SHIFT                                                  0
#define AHBC_PAT_MASK0_MASK                                          0xffffffff
#define AHBC_PAT_MASK0_RD(src)                           (((src) & 0xffffffff))
#define AHBC_PAT_MASK0_WR(src)                      (((u32)(src)) & 0xffffffff)
#define AHBC_PAT_MASK0_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_DBG_PAT_REG_1	*/ 
/*	 Fields PATTERN	 */
#define AHBC_PATTERN1_WIDTH                                                  32
#define AHBC_PATTERN1_SHIFT                                                   0
#define AHBC_PATTERN1_MASK                                           0xffffffff
#define AHBC_PATTERN1_RD(src)                            (((src) & 0xffffffff))
#define AHBC_PATTERN1_WR(src)                       (((u32)(src)) & 0xffffffff)
#define AHBC_PATTERN1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_DBG_PAT_MASK_REG_1	*/ 
/*	 Fields PAT_MASK	 */
#define AHBC_PAT_MASK1_WIDTH                                                 32
#define AHBC_PAT_MASK1_SHIFT                                                  0
#define AHBC_PAT_MASK1_MASK                                          0xffffffff
#define AHBC_PAT_MASK1_RD(src)                           (((src) & 0xffffffff))
#define AHBC_PAT_MASK1_WR(src)                      (((u32)(src)) & 0xffffffff)
#define AHBC_PAT_MASK1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register DBG_TRIG_OUT	*/ 
/*	 Fields DBG_OUT	 */
#define AHBC_DBG_OUT_WIDTH                                                   32
#define AHBC_DBG_OUT_SHIFT                                                    0
#define AHBC_DBG_OUT_MASK                                            0xffffffff
#define AHBC_DBG_OUT_RD(src)                             (((src) & 0xffffffff))
#define AHBC_DBG_OUT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register DBG_ECC_INT_OUT	*/ 
/*	 Fields SLV_TMO	 */
#define AHBC_SLV_TMO_WIDTH                                                    1
#define AHBC_SLV_TMO_SHIFT                                                    3
#define AHBC_SLV_TMO_MASK                                            0x00000008
#define AHBC_SLV_TMO_RD(src)                          (((src) & 0x00000008)>>3)
#define AHBC_SLV_TMO_WR(src)                     (((u32)(src)<<3) & 0x00000008)
#define AHBC_SLV_TMO_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields SECC_INT	 */
#define AHBC_SECC_INT_WIDTH                                                   1
#define AHBC_SECC_INT_SHIFT                                                   2
#define AHBC_SECC_INT_MASK                                           0x00000004
#define AHBC_SECC_INT_RD(src)                         (((src) & 0x00000004)>>2)
#define AHBC_SECC_INT_WR(src)                    (((u32)(src)<<2) & 0x00000004)
#define AHBC_SECC_INT_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields DECC_INT	 */
#define AHBC_DECC_INT_WIDTH                                                   1
#define AHBC_DECC_INT_SHIFT                                                   1
#define AHBC_DECC_INT_MASK                                           0x00000002
#define AHBC_DECC_INT_RD(src)                         (((src) & 0x00000002)>>1)
#define AHBC_DECC_INT_WR(src)                    (((u32)(src)<<1) & 0x00000002)
#define AHBC_DECC_INT_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields DBG_INT	 */
#define AHBC_DBG_INT_WIDTH                                                    1
#define AHBC_DBG_INT_SHIFT                                                    0
#define AHBC_DBG_INT_MASK                                            0x00000001
#define AHBC_DBG_INT_RD(src)                             (((src) & 0x00000001))
#define AHBC_DBG_INT_WR(src)                        (((u32)(src)) & 0x00000001)
#define AHBC_DBG_INT_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register DBG_ECC_INT_OUTMask	*/
/*    Mask Register Fields SLV_TMOMask    */
#define AHBC_SLV_TMOMASK_WIDTH                                                1
#define AHBC_SLV_TMOMASK_SHIFT                                                3
#define AHBC_SLV_TMOMASK_MASK                                        0x00000008
#define AHBC_SLV_TMOMASK_RD(src)                      (((src) & 0x00000008)>>3)
#define AHBC_SLV_TMOMASK_WR(src)                 (((u32)(src)<<3) & 0x00000008)
#define AHBC_SLV_TMOMASK_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*    Mask Register Fields SECC_INTMask    */
#define AHBC_SECC_INTMASK_WIDTH                                               1
#define AHBC_SECC_INTMASK_SHIFT                                               2
#define AHBC_SECC_INTMASK_MASK                                       0x00000004
#define AHBC_SECC_INTMASK_RD(src)                     (((src) & 0x00000004)>>2)
#define AHBC_SECC_INTMASK_WR(src)                (((u32)(src)<<2) & 0x00000004)
#define AHBC_SECC_INTMASK_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*    Mask Register Fields DECC_INTMask    */
#define AHBC_DECC_INTMASK_WIDTH                                               1
#define AHBC_DECC_INTMASK_SHIFT                                               1
#define AHBC_DECC_INTMASK_MASK                                       0x00000002
#define AHBC_DECC_INTMASK_RD(src)                     (((src) & 0x00000002)>>1)
#define AHBC_DECC_INTMASK_WR(src)                (((u32)(src)<<1) & 0x00000002)
#define AHBC_DECC_INTMASK_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*    Mask Register Fields DBG_INTMask    */
#define AHBC_DBG_INTMASK_WIDTH                                                1
#define AHBC_DBG_INTMASK_SHIFT                                                0
#define AHBC_DBG_INTMASK_MASK                                        0x00000001
#define AHBC_DBG_INTMASK_RD(src)                         (((src) & 0x00000001))
#define AHBC_DBG_INTMASK_WR(src)                    (((u32)(src)) & 0x00000001)
#define AHBC_DBG_INTMASK_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register CFG_SECC_INT_THRSH_REG	*/ 
/*	 Fields SECC_INT_THRSH	 */
#define AHBC_SECC_INT_THRSH_WIDTH                                             8
#define AHBC_SECC_INT_THRSH_SHIFT                                             0
#define AHBC_SECC_INT_THRSH_MASK                                     0x000000ff
#define AHBC_SECC_INT_THRSH_RD(src)                      (((src) & 0x000000ff))
#define AHBC_SECC_INT_THRSH_WR(src)                 (((u32)(src)) & 0x000000ff)
#define AHBC_SECC_INT_THRSH_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register CFG_MEM_ECC_BYPASS	*/ 
/*	 Fields CFG_ECC_BYPASS	 */
#define AHBC_CFG_ECC_BYPASS_WIDTH                                            16
#define AHBC_CFG_ECC_BYPASS_SHIFT                                             0
#define AHBC_CFG_ECC_BYPASS_MASK                                     0x0000ffff
#define AHBC_CFG_ECC_BYPASS_RD(src)                      (((src) & 0x0000ffff))
#define AHBC_CFG_ECC_BYPASS_WR(src)                 (((u32)(src)) & 0x0000ffff)
#define AHBC_CFG_ECC_BYPASS_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register CFG_MEM_PWRDN_DIS	*/ 
/*	 Fields CFG_ECC_PWRDN_DIS	 */
#define AHBC_CFG_ECC_PWRDN_DIS_WIDTH                                         16
#define AHBC_CFG_ECC_PWRDN_DIS_SHIFT                                         16
#define AHBC_CFG_ECC_PWRDN_DIS_MASK                                  0xffff0000
#define AHBC_CFG_ECC_PWRDN_DIS_RD(src)               (((src) & 0xffff0000)>>16)
#define AHBC_CFG_ECC_PWRDN_DIS_WR(src)          (((u32)(src)<<16) & 0xffff0000)
#define AHBC_CFG_ECC_PWRDN_DIS_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields CFG_PWRDN_DIS	 */
#define AHBC_CFG_PWRDN_DIS_WIDTH                                             16
#define AHBC_CFG_PWRDN_DIS_SHIFT                                              0
#define AHBC_CFG_PWRDN_DIS_MASK                                      0x0000ffff
#define AHBC_CFG_PWRDN_DIS_RD(src)                       (((src) & 0x0000ffff))
#define AHBC_CFG_PWRDN_DIS_WR(src)                  (((u32)(src)) & 0x0000ffff)
#define AHBC_CFG_PWRDN_DIS_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register CFG_MEM_RAM_SHUTDOWN	*/ 
/*	 Fields CFG_RAM_SHUTDOWN_EN	 */
#define AHBC_CFG_RAM_SHUTDOWN_EN_WIDTH                                       32
#define AHBC_CFG_RAM_SHUTDOWN_EN_SHIFT                                        0
#define AHBC_CFG_RAM_SHUTDOWN_EN_MASK                                0xffffffff
#define AHBC_CFG_RAM_SHUTDOWN_EN_RD(src)                 (((src) & 0xffffffff))
#define AHBC_CFG_RAM_SHUTDOWN_EN_WR(src)            (((u32)(src)) & 0xffffffff)
#define AHBC_CFG_RAM_SHUTDOWN_EN_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register BLOCK_MEM_RDY	*/ 
/*	 Fields MEM_RDY	 */
#define AHBC_MEM_RDY_WIDTH                                                   32
#define AHBC_MEM_RDY_SHIFT                                                    0
#define AHBC_MEM_RDY_MASK                                            0xffffffff
#define AHBC_MEM_RDY_RD(src)                             (((src) & 0xffffffff))
#define AHBC_MEM_RDY_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_AXI_SLV_RD_TMO	*/ 
/*	 Fields SLV_ID_TMO_ENABLE	 */
#define AHBC_SLV_ID_TMO_ENABLE_WIDTH                                          1
#define AHBC_SLV_ID_TMO_ENABLE_SHIFT                                         31
#define AHBC_SLV_ID_TMO_ENABLE_MASK                                  0x80000000
#define AHBC_SLV_ID_TMO_ENABLE_RD(src)               (((src) & 0x80000000)>>31)
#define AHBC_SLV_ID_TMO_ENABLE_WR(src)          (((u32)(src)<<31) & 0x80000000)
#define AHBC_SLV_ID_TMO_ENABLE_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields SS_ID_TRK_ENABLE	 */
#define AHBC_SS_ID_TRK_ENABLE_WIDTH                                           1
#define AHBC_SS_ID_TRK_ENABLE_SHIFT                                          30
#define AHBC_SS_ID_TRK_ENABLE_MASK                                   0x40000000
#define AHBC_SS_ID_TRK_ENABLE_RD(src)                (((src) & 0x40000000)>>30)
#define AHBC_SS_ID_TRK_ENABLE_WR(src)           (((u32)(src)<<30) & 0x40000000)
#define AHBC_SS_ID_TRK_ENABLE_SET(dst,src) \
                      (((dst) & ~0x40000000) | (((u32)(src)<<30) & 0x40000000))
/*	 Fields SLV_ID_TMO_VALUE	 */
#define AHBC_SLV_ID_TMO_VALUE_WIDTH                                          10
#define AHBC_SLV_ID_TMO_VALUE_SHIFT                                          16
#define AHBC_SLV_ID_TMO_VALUE_MASK                                   0x03ff0000
#define AHBC_SLV_ID_TMO_VALUE_RD(src)                (((src) & 0x03ff0000)>>16)
#define AHBC_SLV_ID_TMO_VALUE_WR(src)           (((u32)(src)<<16) & 0x03ff0000)
#define AHBC_SLV_ID_TMO_VALUE_SET(dst,src) \
                      (((dst) & ~0x03ff0000) | (((u32)(src)<<16) & 0x03ff0000))
/*	 Fields SLV_RDY_TMO_ENABLE	 */
#define AHBC_SLV_RDY_TMO_ENABLE_WIDTH                                         1
#define AHBC_SLV_RDY_TMO_ENABLE_SHIFT                                        15
#define AHBC_SLV_RDY_TMO_ENABLE_MASK                                 0x00008000
#define AHBC_SLV_RDY_TMO_ENABLE_RD(src)              (((src) & 0x00008000)>>15)
#define AHBC_SLV_RDY_TMO_ENABLE_WR(src)         (((u32)(src)<<15) & 0x00008000)
#define AHBC_SLV_RDY_TMO_ENABLE_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))
/*	 Fields SLV_RDY_TMO_VALUE	 */
#define AHBC_SLV_RDY_TMO_VALUE_WIDTH                                         10
#define AHBC_SLV_RDY_TMO_VALUE_SHIFT                                          0
#define AHBC_SLV_RDY_TMO_VALUE_MASK                                  0x000003ff
#define AHBC_SLV_RDY_TMO_VALUE_RD(src)                   (((src) & 0x000003ff))
#define AHBC_SLV_RDY_TMO_VALUE_WR(src)              (((u32)(src)) & 0x000003ff)
#define AHBC_SLV_RDY_TMO_VALUE_SET(dst,src) \
                          (((dst) & ~0x000003ff) | (((u32)(src)) & 0x000003ff))

/*	Register CFG_AXI_SLV_WR_TMO	*/ 
/*	 Fields SLV_ID_TMO_ENABLE	 */
#define AHBC_SLV_ID_TMO_ENABLE_F1_WIDTH                                       1
#define AHBC_SLV_ID_TMO_ENABLE_F1_SHIFT                                      31
#define AHBC_SLV_ID_TMO_ENABLE_F1_MASK                               0x80000000
#define AHBC_SLV_ID_TMO_ENABLE_F1_RD(src)            (((src) & 0x80000000)>>31)
#define AHBC_SLV_ID_TMO_ENABLE_F1_WR(src)       (((u32)(src)<<31) & 0x80000000)
#define AHBC_SLV_ID_TMO_ENABLE_F1_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields SS_ID_TRK_ENABLE	 */
#define AHBC_SS_ID_TRK_ENABLE_F1_WIDTH                                        1
#define AHBC_SS_ID_TRK_ENABLE_F1_SHIFT                                       30
#define AHBC_SS_ID_TRK_ENABLE_F1_MASK                                0x40000000
#define AHBC_SS_ID_TRK_ENABLE_F1_RD(src)             (((src) & 0x40000000)>>30)
#define AHBC_SS_ID_TRK_ENABLE_F1_WR(src)        (((u32)(src)<<30) & 0x40000000)
#define AHBC_SS_ID_TRK_ENABLE_F1_SET(dst,src) \
                      (((dst) & ~0x40000000) | (((u32)(src)<<30) & 0x40000000))
/*	 Fields SLV_ID_TMO_VALUE	 */
#define AHBC_SLV_ID_TMO_VALUE_F1_WIDTH                                       10
#define AHBC_SLV_ID_TMO_VALUE_F1_SHIFT                                       16
#define AHBC_SLV_ID_TMO_VALUE_F1_MASK                                0x03ff0000
#define AHBC_SLV_ID_TMO_VALUE_F1_RD(src)             (((src) & 0x03ff0000)>>16)
#define AHBC_SLV_ID_TMO_VALUE_F1_WR(src)        (((u32)(src)<<16) & 0x03ff0000)
#define AHBC_SLV_ID_TMO_VALUE_F1_SET(dst,src) \
                      (((dst) & ~0x03ff0000) | (((u32)(src)<<16) & 0x03ff0000))
/*	 Fields SLV_RDY_TMO_ENABLE	 */
#define AHBC_SLV_RDY_TMO_ENABLE_F1_WIDTH                                      1
#define AHBC_SLV_RDY_TMO_ENABLE_F1_SHIFT                                     15
#define AHBC_SLV_RDY_TMO_ENABLE_F1_MASK                              0x00008000
#define AHBC_SLV_RDY_TMO_ENABLE_F1_RD(src)           (((src) & 0x00008000)>>15)
#define AHBC_SLV_RDY_TMO_ENABLE_F1_WR(src)      (((u32)(src)<<15) & 0x00008000)
#define AHBC_SLV_RDY_TMO_ENABLE_F1_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))
/*	 Fields SLV_RDY_TMO_VALUE	 */
#define AHBC_SLV_RDY_TMO_VALUE_F1_WIDTH                                      10
#define AHBC_SLV_RDY_TMO_VALUE_F1_SHIFT                                       0
#define AHBC_SLV_RDY_TMO_VALUE_F1_MASK                               0x000003ff
#define AHBC_SLV_RDY_TMO_VALUE_F1_RD(src)                (((src) & 0x000003ff))
#define AHBC_SLV_RDY_TMO_VALUE_F1_WR(src)           (((u32)(src)) & 0x000003ff)
#define AHBC_SLV_RDY_TMO_VALUE_F1_SET(dst,src) \
                          (((dst) & ~0x000003ff) | (((u32)(src)) & 0x000003ff))

/*	Register STS_AXI_SLV_TMO	*/ 
/*	 Fields SLV_ARID_TMO	 */
#define AHBC_SLV_ARID_TMO_WIDTH                                               1
#define AHBC_SLV_ARID_TMO_SHIFT                                               3
#define AHBC_SLV_ARID_TMO_MASK                                       0x00000008
#define AHBC_SLV_ARID_TMO_RD(src)                     (((src) & 0x00000008)>>3)
#define AHBC_SLV_ARID_TMO_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields SLV_ARRDY_TMO	 */
#define AHBC_SLV_ARRDY_TMO_WIDTH                                              1
#define AHBC_SLV_ARRDY_TMO_SHIFT                                              2
#define AHBC_SLV_ARRDY_TMO_MASK                                      0x00000004
#define AHBC_SLV_ARRDY_TMO_RD(src)                    (((src) & 0x00000004)>>2)
#define AHBC_SLV_ARRDY_TMO_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields SLV_AWID_TMO	 */
#define AHBC_SLV_AWID_TMO_WIDTH                                               1
#define AHBC_SLV_AWID_TMO_SHIFT                                               1
#define AHBC_SLV_AWID_TMO_MASK                                       0x00000002
#define AHBC_SLV_AWID_TMO_RD(src)                     (((src) & 0x00000002)>>1)
#define AHBC_SLV_AWID_TMO_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields SLV_AWRDY_TMO	 */
#define AHBC_SLV_AWRDY_TMO_WIDTH                                              1
#define AHBC_SLV_AWRDY_TMO_SHIFT                                              0
#define AHBC_SLV_AWRDY_TMO_MASK                                      0x00000001
#define AHBC_SLV_AWRDY_TMO_RD(src)                       (((src) & 0x00000001))
#define AHBC_SLV_AWRDY_TMO_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register CFG_AXI_SS_CSR_TMO	*/ 
/*	 Fields CFG_CSR_TMO	 */
#define AHBC_CFG_CSR_TMO_WIDTH                                               16
#define AHBC_CFG_CSR_TMO_SHIFT                                               16
#define AHBC_CFG_CSR_TMO_MASK                                        0xffff0000
#define AHBC_CFG_CSR_TMO_RD(src)                     (((src) & 0xffff0000)>>16)
#define AHBC_CFG_CSR_TMO_WR(src)                (((u32)(src)<<16) & 0xffff0000)
#define AHBC_CFG_CSR_TMO_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields CSR_ERR_RESP_EN	 */
#define AHBC_CSR_ERR_RESP_EN_WIDTH                                            1
#define AHBC_CSR_ERR_RESP_EN_SHIFT                                           15
#define AHBC_CSR_ERR_RESP_EN_MASK                                    0x00008000
#define AHBC_CSR_ERR_RESP_EN_RD(src)                 (((src) & 0x00008000)>>15)
#define AHBC_CSR_ERR_RESP_EN_WR(src)            (((u32)(src)<<15) & 0x00008000)
#define AHBC_CSR_ERR_RESP_EN_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))

/*	Register STS_READ_LATENCY_TOT_READ_REQS	*/ 
/*	 Fields TOT_READS	 */
#define AHBC_TOT_READS_WIDTH                                                 16
#define AHBC_TOT_READS_SHIFT                                                  0
#define AHBC_TOT_READS_MASK                                          0x0000ffff
#define AHBC_TOT_READS_RD(src)                           (((src) & 0x0000ffff))
#define AHBC_TOT_READS_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

#endif /* _APM86xxx_AHBC_H__ */
