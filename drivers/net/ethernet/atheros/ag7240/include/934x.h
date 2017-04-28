/*
 * vim: tabstop=8 : noexpandtab
 */
#ifndef _934x_H
#define _934x_H


/*
 * PERF CTL bits
 */
#define PERF_CTL_PCI_AHB_0		( 0)
#define PERF_CTL_PCI_AHB_1		( 1)
#define PERF_CTL_USB_0			( 2)
#define PERF_CTL_USB_1			( 3)
#define PERF_CTL_GE0_PKT_CNT		( 4)
#define PERF_CTL_GEO_AHB_1		( 5)
#define PERF_CTL_GE1_PKT_CNT		( 6)
#define PERF_CTL_GE1_AHB_1		( 7)
#define PERF_CTL_PCI_DEV_0_BUSY		( 8)
#define PERF_CTL_PCI_DEV_1_BUSY		( 9)
#define PERF_CTL_PCI_DEV_2_BUSY		(10)
#define PERF_CTL_PCI_HOST_BUSY		(11)
#define PERF_CTL_PCI_DEV_0_ARB		(12)
#define PERF_CTL_PCI_DEV_1_ARB		(13)
#define PERF_CTL_PCI_DEV_2_ARB		(14)
#define PERF_CTL_PCI_HOST_ARB		(15)
#define PERF_CTL_PCI_DEV_0_ACTIVE	(16)
#define PERF_CTL_PCI_DEV_1_ACTIVE	(17)
#define PERF_CTL_PCI_DEV_2_ACTIVE	(18)
#define PERF_CTL_HOST_ACTIVE		(19)

#define ath_perf0_ctl(_val) ath_reg_wr(ATH_PERF_CTL, (_val))
#define ath_perf1_ctl(_val) ath_reg_rmw_set(ATH_PERF_CTL, ((_val) << 8))

#define ATH_PLL_USE_REV_ID		0
#define ATH_PLL_200_200_100		1
#define ATH_PLL_300_300_150		2
#define ATH_PLL_333_333_166		3
#define ATH_PLL_266_266_133		4
#define ATH_PLL_266_266_66		5
#define ATH_PLL_400_400_200		6
#define ATH_PLL_600_400_150		7


/*
 * ATH_RESET bit defines
 */
#define ATH_RESET_SLIC			(1 << 30)
#define ATH_RESET_EXTERNAL		(1 << 28)
#define ATH_RESET_FULL_CHIP		(1 << 24)
#define ATH_RESET_GE0_MDIO		(1 << 22)
#define ATH_RESET_CPU_NMI		(1 << 21)
#define ATH_RESET_CPU_COLD_RESET_MASK	(1 << 20)
#define ATH_RESET_DMA			(1 << 19)
#define ATH_RESET_STEREO		(1 << 17)
#define ATH_RESET_DDR			(1 << 16)
#define ATH_RESET_USB_PHY_PLL_PWD_EXT	(1 << 15)
#define ATH_RESET_GE1_MAC		(1 << 13)
#define ATH_RESET_GE1_PHY		(1 << 12)
#define ATH_RESET_USB_PHY_ANALOG	(1 << 11)
#define ATH_RESET_PCIE_PHY_SHIFT	(1 << 10)
#define ATH_RESET_GE0_MAC		(1 << 9)
#define ATH_RESET_GE0_PHY		(1 << 8)
#define ATH_RESET_USBSUS_OVRIDE		(1 << 3)
#define ATH_RESET_USB_OHCI_DLL		(1 << 3)
#define ATH_RESET_USB_HOST		(1 << 5)
#define ATH_RESET_USB_PHY		(1 << 4)
#define ATH_RESET_PCI_BUS		(1 << 1)
#define ATH_RESET_PCI_CORE		(1 << 0)
#define ATH_RESET_I2S			(1 << 0)

#define RST_RESET_HOST_RESET_MSB                                     31
#define RST_RESET_HOST_RESET_LSB                                     31
#define RST_RESET_HOST_RESET_MASK                                    0x80000000
#define RST_RESET_HOST_RESET_GET(x)                                  (((x) & RST_RESET_HOST_RESET_MASK) >> RST_RESET_HOST_RESET_LSB)
#define RST_RESET_HOST_RESET_SET(x)                                  (((x) << RST_RESET_HOST_RESET_LSB) & RST_RESET_HOST_RESET_MASK)
#define RST_RESET_HOST_RESET_RESET                                   0x0 // 0
#define RST_RESET_SLIC_RESET_MSB                                     30
#define RST_RESET_SLIC_RESET_LSB                                     30
#define RST_RESET_SLIC_RESET_MASK                                    0x40000000
#define RST_RESET_SLIC_RESET_GET(x)                                  (((x) & RST_RESET_SLIC_RESET_MASK) >> RST_RESET_SLIC_RESET_LSB)
#define RST_RESET_SLIC_RESET_SET(x)                                  (((x) << RST_RESET_SLIC_RESET_LSB) & RST_RESET_SLIC_RESET_MASK)
#define RST_RESET_SLIC_RESET_RESET                                   0x0 // 0
#define RST_RESET_HDMA_RESET_MSB                                     29
#define RST_RESET_HDMA_RESET_LSB                                     29
#define RST_RESET_HDMA_RESET_MASK                                    0x20000000
#define RST_RESET_HDMA_RESET_GET(x)                                  (((x) & RST_RESET_HDMA_RESET_MASK) >> RST_RESET_HDMA_RESET_LSB)
#define RST_RESET_HDMA_RESET_SET(x)                                  (((x) << RST_RESET_HDMA_RESET_LSB) & RST_RESET_HDMA_RESET_MASK)
#define RST_RESET_HDMA_RESET_RESET                                   0x1 // 1
#define RST_RESET_EXTERNAL_RESET_MSB                                 28
#define RST_RESET_EXTERNAL_RESET_LSB                                 28
#define RST_RESET_EXTERNAL_RESET_MASK                                0x10000000
#define RST_RESET_EXTERNAL_RESET_GET(x)                              (((x) & RST_RESET_EXTERNAL_RESET_MASK) >> RST_RESET_EXTERNAL_RESET_LSB)
#define RST_RESET_EXTERNAL_RESET_SET(x)                              (((x) << RST_RESET_EXTERNAL_RESET_LSB) & RST_RESET_EXTERNAL_RESET_MASK)
#define RST_RESET_EXTERNAL_RESET_RESET                               0x0 // 0
#define RST_RESET_RTC_RESET_MSB                                      27
#define RST_RESET_RTC_RESET_LSB                                      27
#define RST_RESET_RTC_RESET_MASK                                     0x08000000
#define RST_RESET_RTC_RESET_GET(x)                                   (((x) & RST_RESET_RTC_RESET_MASK) >> RST_RESET_RTC_RESET_LSB)
#define RST_RESET_RTC_RESET_SET(x)                                   (((x) << RST_RESET_RTC_RESET_LSB) & RST_RESET_RTC_RESET_MASK)
#define RST_RESET_RTC_RESET_RESET                                    0x1 // 1
#define RST_RESET_PCIEEP_RST_INT_MSB                                 26
#define RST_RESET_PCIEEP_RST_INT_LSB                                 26
#define RST_RESET_PCIEEP_RST_INT_MASK                                0x04000000
#define RST_RESET_PCIEEP_RST_INT_GET(x)                              (((x) & RST_RESET_PCIEEP_RST_INT_MASK) >> RST_RESET_PCIEEP_RST_INT_LSB)
#define RST_RESET_PCIEEP_RST_INT_SET(x)                              (((x) << RST_RESET_PCIEEP_RST_INT_LSB) & RST_RESET_PCIEEP_RST_INT_MASK)
#define RST_RESET_PCIEEP_RST_INT_RESET                               0x0 // 0
#define RST_RESET_CHKSUM_ACC_RESET_MSB                               25
#define RST_RESET_CHKSUM_ACC_RESET_LSB                               25
#define RST_RESET_CHKSUM_ACC_RESET_MASK                              0x02000000
#define RST_RESET_CHKSUM_ACC_RESET_GET(x)                            (((x) & RST_RESET_CHKSUM_ACC_RESET_MASK) >> RST_RESET_CHKSUM_ACC_RESET_LSB)
#define RST_RESET_CHKSUM_ACC_RESET_SET(x)                            (((x) << RST_RESET_CHKSUM_ACC_RESET_LSB) & RST_RESET_CHKSUM_ACC_RESET_MASK)
#define RST_RESET_CHKSUM_ACC_RESET_RESET                             0x0 // 0
#define RST_RESET_FULL_CHIP_RESET_MSB                                24
#define RST_RESET_FULL_CHIP_RESET_LSB                                24
#define RST_RESET_FULL_CHIP_RESET_MASK                               0x01000000
#define RST_RESET_FULL_CHIP_RESET_GET(x)                             (((x) & RST_RESET_FULL_CHIP_RESET_MASK) >> RST_RESET_FULL_CHIP_RESET_LSB)
#define RST_RESET_FULL_CHIP_RESET_SET(x)                             (((x) << RST_RESET_FULL_CHIP_RESET_LSB) & RST_RESET_FULL_CHIP_RESET_MASK)
#define RST_RESET_FULL_CHIP_RESET_RESET                              0x0 // 0
#define RST_RESET_GE1_MDIO_RESET_MSB                                 23
#define RST_RESET_GE1_MDIO_RESET_LSB                                 23
#define RST_RESET_GE1_MDIO_RESET_MASK                                0x00800000
#define RST_RESET_GE1_MDIO_RESET_GET(x)                              (((x) & RST_RESET_GE1_MDIO_RESET_MASK) >> RST_RESET_GE1_MDIO_RESET_LSB)
#define RST_RESET_GE1_MDIO_RESET_SET(x)                              (((x) << RST_RESET_GE1_MDIO_RESET_LSB) & RST_RESET_GE1_MDIO_RESET_MASK)
#define RST_RESET_GE1_MDIO_RESET_RESET                               0x1 // 1
#define RST_RESET_GE0_MDIO_RESET_MSB                                 22
#define RST_RESET_GE0_MDIO_RESET_LSB                                 22
#define RST_RESET_GE0_MDIO_RESET_MASK                                0x00400000
#define RST_RESET_GE0_MDIO_RESET_GET(x)                              (((x) & RST_RESET_GE0_MDIO_RESET_MASK) >> RST_RESET_GE0_MDIO_RESET_LSB)
#define RST_RESET_GE0_MDIO_RESET_SET(x)                              (((x) << RST_RESET_GE0_MDIO_RESET_LSB) & RST_RESET_GE0_MDIO_RESET_MASK)
#define RST_RESET_GE0_MDIO_RESET_RESET                               0x1 // 1
#define RST_RESET_CPU_NMI_MSB                                        21
#define RST_RESET_CPU_NMI_LSB                                        21
#define RST_RESET_CPU_NMI_MASK                                       0x00200000
#define RST_RESET_CPU_NMI_GET(x)                                     (((x) & RST_RESET_CPU_NMI_MASK) >> RST_RESET_CPU_NMI_LSB)
#define RST_RESET_CPU_NMI_SET(x)                                     (((x) << RST_RESET_CPU_NMI_LSB) & RST_RESET_CPU_NMI_MASK)
#define RST_RESET_CPU_NMI_RESET                                      0x0 // 0
#define RST_RESET_CPU_COLD_RESET_MSB                                 20
#define RST_RESET_CPU_COLD_RESET_LSB                                 20
#define RST_RESET_CPU_COLD_RESET_MASK                                0x00100000
#define RST_RESET_CPU_COLD_RESET_GET(x)                              (((x) & RST_RESET_CPU_COLD_RESET_MASK) >> RST_RESET_CPU_COLD_RESET_LSB)
#define RST_RESET_CPU_COLD_RESET_SET(x)                              (((x) << RST_RESET_CPU_COLD_RESET_LSB) & RST_RESET_CPU_COLD_RESET_MASK)
#define RST_RESET_CPU_COLD_RESET_RESET                               0x0 // 0
#define RST_RESET_HOST_RESET_INT_MSB                                 19
#define RST_RESET_HOST_RESET_INT_LSB                                 19
#define RST_RESET_HOST_RESET_INT_MASK                                0x00080000
#define RST_RESET_HOST_RESET_INT_GET(x)                              (((x) & RST_RESET_HOST_RESET_INT_MASK) >> RST_RESET_HOST_RESET_INT_LSB)
#define RST_RESET_HOST_RESET_INT_SET(x)                              (((x) << RST_RESET_HOST_RESET_INT_LSB) & RST_RESET_HOST_RESET_INT_MASK)
#define RST_RESET_HOST_RESET_INT_RESET                               0x0 // 0
#define RST_RESET_PCIEEP_RESET_MSB                                   18
#define RST_RESET_PCIEEP_RESET_LSB                                   18
#define RST_RESET_PCIEEP_RESET_MASK                                  0x00040000
#define RST_RESET_PCIEEP_RESET_GET(x)                                (((x) & RST_RESET_PCIEEP_RESET_MASK) >> RST_RESET_PCIEEP_RESET_LSB)
#define RST_RESET_PCIEEP_RESET_SET(x)                                (((x) << RST_RESET_PCIEEP_RESET_LSB) & RST_RESET_PCIEEP_RESET_MASK)
#define RST_RESET_PCIEEP_RESET_RESET                                 0x0 // 0
#define RST_RESET_UART1_RESET_MSB                                    17
#define RST_RESET_UART1_RESET_LSB                                    17
#define RST_RESET_UART1_RESET_MASK                                   0x00020000
#define RST_RESET_UART1_RESET_GET(x)                                 (((x) & RST_RESET_UART1_RESET_MASK) >> RST_RESET_UART1_RESET_LSB)
#define RST_RESET_UART1_RESET_SET(x)                                 (((x) << RST_RESET_UART1_RESET_LSB) & RST_RESET_UART1_RESET_MASK)
#define RST_RESET_UART1_RESET_RESET                                  0x0 // 0
#define RST_RESET_DDR_RESET_MSB                                      16
#define RST_RESET_DDR_RESET_LSB                                      16
#define RST_RESET_DDR_RESET_MASK                                     0x00010000
#define RST_RESET_DDR_RESET_GET(x)                                   (((x) & RST_RESET_DDR_RESET_MASK) >> RST_RESET_DDR_RESET_LSB)
#define RST_RESET_DDR_RESET_SET(x)                                   (((x) << RST_RESET_DDR_RESET_LSB) & RST_RESET_DDR_RESET_MASK)
#define RST_RESET_DDR_RESET_RESET                                    0x0 // 0
#define RST_RESET_USB_PHY_PLL_PWD_EXT_MSB                            15
#define RST_RESET_USB_PHY_PLL_PWD_EXT_LSB                            15
#define RST_RESET_USB_PHY_PLL_PWD_EXT_MASK                           0x00008000
#define RST_RESET_USB_PHY_PLL_PWD_EXT_GET(x)                         (((x) & RST_RESET_USB_PHY_PLL_PWD_EXT_MASK) >> RST_RESET_USB_PHY_PLL_PWD_EXT_LSB)
#define RST_RESET_USB_PHY_PLL_PWD_EXT_SET(x)                         (((x) << RST_RESET_USB_PHY_PLL_PWD_EXT_LSB) & RST_RESET_USB_PHY_PLL_PWD_EXT_MASK)
#define RST_RESET_USB_PHY_PLL_PWD_EXT_RESET                          0x0 // 0
#define RST_RESET_NANDF_RESET_MSB                                    14
#define RST_RESET_NANDF_RESET_LSB                                    14
#define RST_RESET_NANDF_RESET_MASK                                   0x00004000
#define RST_RESET_NANDF_RESET_GET(x)                                 (((x) & RST_RESET_NANDF_RESET_MASK) >> RST_RESET_NANDF_RESET_LSB)
#define RST_RESET_NANDF_RESET_SET(x)                                 (((x) << RST_RESET_NANDF_RESET_LSB) & RST_RESET_NANDF_RESET_MASK)
#define RST_RESET_NANDF_RESET_RESET                                  0x1 // 1
#define RST_RESET_GE1_MAC_RESET_MSB                                  13
#define RST_RESET_GE1_MAC_RESET_LSB                                  13
#define RST_RESET_GE1_MAC_RESET_MASK                                 0x00002000
#define RST_RESET_GE1_MAC_RESET_GET(x)                               (((x) & RST_RESET_GE1_MAC_RESET_MASK) >> RST_RESET_GE1_MAC_RESET_LSB)
#define RST_RESET_GE1_MAC_RESET_SET(x)                               (((x) << RST_RESET_GE1_MAC_RESET_LSB) & RST_RESET_GE1_MAC_RESET_MASK)
#define RST_RESET_GE1_MAC_RESET_RESET                                0x1 // 1
#define RST_RESET_ETH_SWITCH_ARESET_MSB                              12
#define RST_RESET_ETH_SWITCH_ARESET_LSB                              12
#define RST_RESET_ETH_SWITCH_ARESET_MASK                             0x00001000
#define RST_RESET_ETH_SWITCH_ARESET_GET(x)                           (((x) & RST_RESET_ETH_SWITCH_ARESET_MASK) >> RST_RESET_ETH_SWITCH_ARESET_LSB)
#define RST_RESET_ETH_SWITCH_ARESET_SET(x)                           (((x) << RST_RESET_ETH_SWITCH_ARESET_LSB) & RST_RESET_ETH_SWITCH_ARESET_MASK)
#define RST_RESET_ETH_SWITCH_ARESET_RESET                            0x1 // 1
#define RST_RESET_USB_PHY_ARESET_MSB                                 11
#define RST_RESET_USB_PHY_ARESET_LSB                                 11
#define RST_RESET_USB_PHY_ARESET_MASK                                0x00000800
#define RST_RESET_USB_PHY_ARESET_GET(x)                              (((x) & RST_RESET_USB_PHY_ARESET_MASK) >> RST_RESET_USB_PHY_ARESET_LSB)
#define RST_RESET_USB_PHY_ARESET_SET(x)                              (((x) << RST_RESET_USB_PHY_ARESET_LSB) & RST_RESET_USB_PHY_ARESET_MASK)
#define RST_RESET_USB_PHY_ARESET_RESET                               0x1 // 1
#define RST_RESET_HOST_DMA_INT_MSB                                   10
#define RST_RESET_HOST_DMA_INT_LSB                                   10
#define RST_RESET_HOST_DMA_INT_MASK                                  0x00000400
#define RST_RESET_HOST_DMA_INT_GET(x)                                (((x) & RST_RESET_HOST_DMA_INT_MASK) >> RST_RESET_HOST_DMA_INT_LSB)
#define RST_RESET_HOST_DMA_INT_SET(x)                                (((x) << RST_RESET_HOST_DMA_INT_LSB) & RST_RESET_HOST_DMA_INT_MASK)
#define RST_RESET_HOST_DMA_INT_RESET                                 0x0 // 0
#define RST_RESET_GE0_MAC_RESET_MSB                                  9
#define RST_RESET_GE0_MAC_RESET_LSB                                  9
#define RST_RESET_GE0_MAC_RESET_MASK                                 0x00000200
#define RST_RESET_GE0_MAC_RESET_GET(x)                               (((x) & RST_RESET_GE0_MAC_RESET_MASK) >> RST_RESET_GE0_MAC_RESET_LSB)
#define RST_RESET_GE0_MAC_RESET_SET(x)                               (((x) << RST_RESET_GE0_MAC_RESET_LSB) & RST_RESET_GE0_MAC_RESET_MASK)
#define RST_RESET_GE0_MAC_RESET_RESET                                0x1 // 1
#define RST_RESET_ETH_SWITCH_RESET_MSB                               8
#define RST_RESET_ETH_SWITCH_RESET_LSB                               8
#define RST_RESET_ETH_SWITCH_RESET_MASK                              0x00000100
#define RST_RESET_ETH_SWITCH_RESET_GET(x)                            (((x) & RST_RESET_ETH_SWITCH_RESET_MASK) >> RST_RESET_ETH_SWITCH_RESET_LSB)
#define RST_RESET_ETH_SWITCH_RESET_SET(x)                            (((x) << RST_RESET_ETH_SWITCH_RESET_LSB) & RST_RESET_ETH_SWITCH_RESET_MASK)
#define RST_RESET_ETH_SWITCH_RESET_RESET                             0x1 // 1
#define RST_RESET_PCIE_PHY_RESET_MSB                                 7
#define RST_RESET_PCIE_PHY_RESET_LSB                                 7
#define RST_RESET_PCIE_PHY_RESET_MASK                                0x00000080
#define RST_RESET_PCIE_PHY_RESET_GET(x)                              (((x) & RST_RESET_PCIE_PHY_RESET_MASK) >> RST_RESET_PCIE_PHY_RESET_LSB)
#define RST_RESET_PCIE_PHY_RESET_SET(x)                              (((x) << RST_RESET_PCIE_PHY_RESET_LSB) & RST_RESET_PCIE_PHY_RESET_MASK)
#define RST_RESET_PCIE_PHY_RESET_RESET                               0x1 // 1
#define RST_RESET_PCIE_RESET_MSB                                     6
#define RST_RESET_PCIE_RESET_LSB                                     6
#define RST_RESET_PCIE_RESET_MASK                                    0x00000040
#define RST_RESET_PCIE_RESET_GET(x)                                  (((x) & RST_RESET_PCIE_RESET_MASK) >> RST_RESET_PCIE_RESET_LSB)
#define RST_RESET_PCIE_RESET_SET(x)                                  (((x) << RST_RESET_PCIE_RESET_LSB) & RST_RESET_PCIE_RESET_MASK)
#define RST_RESET_PCIE_RESET_RESET                                   0x1 // 1
#define RST_RESET_USB_HOST_RESET_MSB                                 5
#define RST_RESET_USB_HOST_RESET_LSB                                 5
#define RST_RESET_USB_HOST_RESET_MASK                                0x00000020
#define RST_RESET_USB_HOST_RESET_GET(x)                              (((x) & RST_RESET_USB_HOST_RESET_MASK) >> RST_RESET_USB_HOST_RESET_LSB)
#define RST_RESET_USB_HOST_RESET_SET(x)                              (((x) << RST_RESET_USB_HOST_RESET_LSB) & RST_RESET_USB_HOST_RESET_MASK)
#define RST_RESET_USB_HOST_RESET_RESET                               0x1 // 1
#define RST_RESET_USB_PHY_RESET_MSB                                  4
#define RST_RESET_USB_PHY_RESET_LSB                                  4
#define RST_RESET_USB_PHY_RESET_MASK                                 0x00000010
#define RST_RESET_USB_PHY_RESET_GET(x)                               (((x) & RST_RESET_USB_PHY_RESET_MASK) >> RST_RESET_USB_PHY_RESET_LSB)
#define RST_RESET_USB_PHY_RESET_SET(x)                               (((x) << RST_RESET_USB_PHY_RESET_LSB) & RST_RESET_USB_PHY_RESET_MASK)
#define RST_RESET_USB_PHY_RESET_RESET                                0x1 // 1
#define RST_RESET_USB_PHY_SUSPEND_OVERRIDE_MSB                       3
#define RST_RESET_USB_PHY_SUSPEND_OVERRIDE_LSB                       3
#define RST_RESET_USB_PHY_SUSPEND_OVERRIDE_MASK                      0x00000008
#define RST_RESET_USB_PHY_SUSPEND_OVERRIDE_GET(x)                    (((x) & RST_RESET_USB_PHY_SUSPEND_OVERRIDE_MASK) >> RST_RESET_USB_PHY_SUSPEND_OVERRIDE_LSB)
#define RST_RESET_USB_PHY_SUSPEND_OVERRIDE_SET(x)                    (((x) << RST_RESET_USB_PHY_SUSPEND_OVERRIDE_LSB) & RST_RESET_USB_PHY_SUSPEND_OVERRIDE_MASK)
#define RST_RESET_USB_PHY_SUSPEND_OVERRIDE_RESET                     0x0 // 0
#define RST_RESET_LUT_RESET_MSB                                      2
#define RST_RESET_LUT_RESET_LSB                                      2
#define RST_RESET_LUT_RESET_MASK                                     0x00000004
#define RST_RESET_LUT_RESET_GET(x)                                   (((x) & RST_RESET_LUT_RESET_MASK) >> RST_RESET_LUT_RESET_LSB)
#define RST_RESET_LUT_RESET_SET(x)                                   (((x) << RST_RESET_LUT_RESET_LSB) & RST_RESET_LUT_RESET_MASK)
#define RST_RESET_LUT_RESET_RESET                                    0x0 // 0
#define RST_RESET_MBOX_RESET_MSB                                     1
#define RST_RESET_MBOX_RESET_LSB                                     1
#define RST_RESET_MBOX_RESET_MASK                                    0x00000002
#define RST_RESET_MBOX_RESET_GET(x)                                  (((x) & RST_RESET_MBOX_RESET_MASK) >> RST_RESET_MBOX_RESET_LSB)
#define RST_RESET_MBOX_RESET_SET(x)                                  (((x) << RST_RESET_MBOX_RESET_LSB) & RST_RESET_MBOX_RESET_MASK)
#define RST_RESET_MBOX_RESET_RESET                                   0x0 // 0
#define RST_RESET_I2S_RESET_MSB                                      0
#define RST_RESET_I2S_RESET_LSB                                      0
#define RST_RESET_I2S_RESET_MASK                                     0x00000001
#define RST_RESET_I2S_RESET_GET(x)                                   (((x) & RST_RESET_I2S_RESET_MASK) >> RST_RESET_I2S_RESET_LSB)
#define RST_RESET_I2S_RESET_SET(x)                                   (((x) << RST_RESET_I2S_RESET_LSB) & RST_RESET_I2S_RESET_MASK)
#define RST_RESET_I2S_RESET_RESET                                    0x0 // 0
#define RST_RESET_ADDRESS                                            0x1806001c

#define ATH_RESET			RST_RESET_ADDRESS


#define ATH_AR9344_1_x			0x2120
#define ATH_AR9342_1_x			0x1120
#define ATH_AR9341_1_x			0x0120

#define ATH_AR9344_1_0			0x2120
#define ATH_AR9342_1_0			0x1120
#define ATH_AR9341_1_0			0x0120

#define ATH_AR9344_1_1			0x2121
#define ATH_AR9342_1_1			0x1121
#define ATH_AR9341_1_1			0x0121

#ifdef KPHACK

#undef is_ar934x
#undef is_wasp

#define is_ar934x()	(1)
#define is_wasp()	(1)

#define is_ar9344()	((ath_reg_rd(ATH_REV_ID) & ATH_REV_ID_MASK_MAJOR) == ATH_AR9344_1_x)
#define is_ar9342()	((ath_reg_rd(ATH_REV_ID) & ATH_REV_ID_MASK_MAJOR) == ATH_AR9342_1_x)
#define is_ar9341()	((ath_reg_rd(ATH_REV_ID) & ATH_REV_ID_MASK_MAJOR) == ATH_AR9341_1_x)

#define is_ar9344_10()	((ath_reg_rd(ATH_REV_ID) & ATH_REV_ID_MASK) == ATH_AR9344_1_0)
#define is_ar9342_10()	((ath_reg_rd(ATH_REV_ID) & ATH_REV_ID_MASK) == ATH_AR9342_1_0)
#define is_ar9341_10()	((ath_reg_rd(ATH_REV_ID) & ATH_REV_ID_MASK) == ATH_AR9341_1_0)

#define is_ar9344_11()	((ath_reg_rd(ATH_REV_ID) & ATH_REV_ID_MASK) == ATH_AR9344_1_1)
#define is_ar9342_11()	((ath_reg_rd(ATH_REV_ID) & ATH_REV_ID_MASK) == ATH_AR9342_1_1)
#define is_ar9341_11()	((ath_reg_rd(ATH_REV_ID) & ATH_REV_ID_MASK) == ATH_AR9341_1_1)

#undef is_ar934x_10
#undef is_ar934x_11
#undef is_ar934x_13

#define is_ar934x_10()	((ath_reg_rd(ATH_REV_ID) & ATH_REV_ID_MASK_MINOR) == 0)
#define is_ar934x_11()	((ath_reg_rd(ATH_REV_ID) & ATH_REV_ID_MASK_MINOR) == 1)
#define is_ar934x_13()	((ath_reg_rd(ATH_REV_ID) & ATH_REV_ID_MASK_MINOR) == 3)

#undef is_ar934x_13_or_later
#undef is_ar934x_11_or_later

#define is_ar934x_13_or_later()	((ath_reg_rd(ATH_REV_ID) & ATH_REV_ID_MASK_MINOR) >= 3)
#define is_ar934x_11_or_later()	((ath_reg_rd(ATH_REV_ID) & ATH_REV_ID_MASK_MINOR) >= 1)

#endif

#define ATH_WMAC_BASE		ATH_APB_BASE + 0x100000
#define ATH_WMAC_LEN		0x1ffff /* XXX:Check this */

/*
 * Mii block
 */
#define ATH_MII0_CTRL		0x18070000
#define ATH_MII1_CTRL		0x18070004

#ifdef CONFIG_ATHRS_HW_NAT

/*
 * Ingress / Egress LUT Registers
 */
#define ATHR_EGRESS_LUT_REG       (ATH_APB_BASE + 0x80000)
#define ATHR_INGRESS_LUT_REG      (ATH_APB_BASE + 0x81000)

/*
 * Fragment LUT Registers
 */
#define ATHR_EGRESS_LUT_FRAG_REG  (ATH_APB_BASE + 0x82000)
#define ATHR_INGRESS_LUT_FRAG_REG (ATH_APB_BASE + 0x83000)

/*
 * Global IP Address Registers for wan interface
 */
#define ATHR_WAN_IP_ADDR_REG	  (ATH_GE0_BASE + 0x210)

/*
 * Nat status Registers
 */
#define ETH_EG_NAT_STATUS          (ATH_GE0_BASE + 0x228)
#define ETH_IG_NAT_STATUS          (ATH_GE0_BASE + 0x230)

/*
 * Ager Registers
 */
#define ETH_EG_AGER_FIFO_REG       (ATH_APB_BASE + 0x80020)
#define ETH_IG_AGER_FIFO_REG       (ATH_APB_BASE + 0x81030)

/*
 * Nat counter Registers
 */
#define ETH_EG_NAT_CNTR          (ATH_GE0_BASE + 0x22c)
#define ETH_IG_NAT_CNTR          (ATH_GE0_BASE + 0x234)

/*
 * Nat debug Registers
 */
#define ETH_EG_NAT_DBG           (ATH_GE0_BASE + 0x310)
#define ETH_IG_NAT_DBG           (ATH_GE0_BASE + 0x2f8)

/*
 * MAC Addr Registers
 */

#define ETH_LCL_MAC_ADDR_DW0       (ATH_GE0_BASE + 0x200)
#define ETH_LCL_MAC_ADDR_DW1       (ATH_GE0_BASE + 0x204)
#define ETH_DST_MAC_ADDR_DW0       (ATH_GE0_BASE + 0x208)
#define ETH_DST_MAC_ADDR_DW1       (ATH_GE0_BASE + 0x20c)

/*
 * Ager INTR Registers
 */
#define ATHR_LUTS_AGER_INTR        (ATH_APB_BASE + 0x70004)
#define ATHR_LUTS_AGER_INTR_MASK   (ATH_APB_BASE + 0x70008)

#endif /* #ifdef CONFIG_ATHRS_HW_NAT */

#ifdef CONFIG_ATHRS_HW_ACL

/********************************************************************
 * EG ACL Registers
 *********************************************************************/
#define ATHR_EG_ACL_STATUS              (ATH_GE0_BASE + 0x238)
#define ATHR_EG_ACL_CMD0_AND_ACTION     (ATH_GE0_BASE + 0x240)
#define ATHR_EG_ACL_CMD1234             (ATH_GE0_BASE + 0x244)
#define ATHR_EG_ACL_OPERAND0            (ATH_GE0_BASE + 0x248)
#define ATHR_EG_ACL_OPERAND1            (ATH_GE0_BASE + 0x24c)
#define ATHR_EG_ACL_MEM_CTRL            (ATH_GE0_BASE + 0x250)
#define ATHR_EG_ACL_RULE_TABLE_FIELDS   (ATH_GE0_BASE + 0x378)
#define ATHR_EG_ACL_RULE_TABLE0_LOWER   (ATH_GE0_BASE + 0x354)
#define ATHR_EG_ACL_RULE_TABLE0_UPPER   (ATH_GE0_BASE + 0x358)
#define ATHR_EG_ACL_RULE_TABLE1_LOWER   (ATH_GE0_BASE + 0x35c)
#define ATHR_EG_ACL_RULE_TABLE1_UPPER   (ATH_GE0_BASE + 0x360)
#define ATHR_EG_ACL_RULE_TABLE2_LOWER   (ATH_GE0_BASE + 0x364)
#define ATHR_EG_ACL_RULE_TABLE2_UPPER   (ATH_GE0_BASE + 0x368)
#define ATHR_EG_ACL_RULE_TABLE3_LOWER   (ATH_GE0_BASE + 0x36c)
#define ATHR_EG_ACL_RULE_TABLE3_UPPER   (ATH_GE0_BASE + 0x370)

/********************************************************************
 * IG ACL Registers
 ********************************************************************/
#define ATHR_IG_ACL_STATUS              (ATH_GE0_BASE + 0x23C)
#define ATHR_IG_ACL_CMD0_AND_ACTION     (ATH_GE0_BASE + 0x254)
#define ATHR_IG_ACL_CMD1234             (ATH_GE0_BASE + 0x258)
#define ATHR_IG_ACL_OPERAND0            (ATH_GE0_BASE + 0x25C)
#define ATHR_IG_ACL_OPERAND1            (ATH_GE0_BASE + 0x260)
#define ATHR_IG_ACL_MEM_CTRL            (ATH_GE0_BASE + 0x264)
#define ATHR_IG_ACL_RULE_TABLE_FIELDS   (ATH_GE0_BASE + 0x374)
#define ATHR_IG_ACL_RULE_TABLE0_LOWER   (ATH_GE0_BASE + 0x334)
#define ATHR_IG_ACL_RULE_TABLE0_UPPER   (ATH_GE0_BASE + 0x338)
#define ATHR_IG_ACL_RULE_TABLE1_LOWER   (ATH_GE0_BASE + 0x33c)
#define ATHR_IG_ACL_RULE_TABLE1_UPPER   (ATH_GE0_BASE + 0x340)
#define ATHR_IG_ACL_RULE_TABLE2_LOWER   (ATH_GE0_BASE + 0x344)
#define ATHR_IG_ACL_RULE_TABLE2_UPPER   (ATH_GE0_BASE + 0x348)
#define ATHR_IG_ACL_RULE_TABLE3_LOWER   (ATH_GE0_BASE + 0x34c)
#define ATHR_IG_ACL_RULE_TABLE3_UPPER   (ATH_GE0_BASE + 0x350)

#endif /* #ifdef CONFIG_ATHRS_HW_ACL */


/*
 * From
 * //depot/chips/wasp/1.0/rtl/otp_intf/blueprint/efuse_reg.h
 */

#define ATH_PCIE_RC_ASPM_SUPPORT			0x180c007c
#define ATH_PCIE_RC_SUPP_L0				(1 << 10)
#define ATH_PCIE_RC_SUPP_L1				(1 << 11)
#define ATH_PCIE_RC_SUPP_VAL				(ATH_PCIE_RC_SUPP_L0 | ATH_PCIE_RC_SUPP_L1)

#define ATH_PCIE_RC_ASPM_ENABLE				0x180c0080
#define ATH_PCIE_RC_EN_L0				(1 << 0)
#define ATH_PCIE_RC_EN_L1				(1 << 1)
#define ATH_PCIE_RC_EN_VAL				(ATH_PCIE_RC_EN_L0 | ATH_PCIE_RC_EN_L1)

#define ATH_PCIE_EP_ASPM_ENABLE				0x14000080
#define ATH_PCIE_EP_EN_L0				(1 << 0)
#define ATH_PCIE_EP_EN_L1				(1 << 1)

#define ATH_DECL_PCI_IM_ARR(x)		\
	uint32_t x[] = { PCIE_INT_MASK_ADDRESS }
#define ATH_DECL_PCI_IS_ARR(x)		\
	uint32_t x[] = { PCIE_INT_STATUS_ADDRESS }
#define ATH_DECL_PCI_RST_ARR(x)		\
	uint32_t x[] = { PCIE_RESET_ADDRESS }
#define ATH_DECL_PCI_ASPM_SUPP_ARR(x)	\
	uint32_t x[] = { ATH_PCIE_RC_ASPM_SUPPORT }
#define ATH_DECL_PCI_ASPM_EN_ARR(x)	\
	uint32_t x[] = { ATH_PCIE_RC_ASPM_ENABLE }
#define ATH_DECL_PCI_CFG_BASE_ARR(x)	\
	uint32_t x[] = { ATH_PCI_DEV_CFGBASE }
#define ATH_DECL_PCI_CRP_ARR(x)		\
	uint32_t x[] = { PCIE_RC_REG_ADDRESS }
#	define ATH_DECL_PCI_IO_RES		\
		ATH_PCI_RES_IO(0, 0x0000, 0x0000)
#	define ATH_DECL_PCI_MEM_RES		\
		ATH_PCI_RES_MEM(0)
#	define ATH_DECL_PCI_CTRLR		\
		ATH_PCI_CTRL_DESCRIPTOR(0)
#	undef ATH_PCI_RC2_IRQ

#endif /* _934x_H */
