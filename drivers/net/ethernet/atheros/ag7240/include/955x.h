/*
 * vim: tabstop=8 : noexpandtab
 */
#ifndef _955x_H
#define _955x_H

/*
 * Address map
 */
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

#undef is_sco

#define is_sco()	(1)

#define ATH_WMAC_BASE		ATH_APB_BASE + 0x100000
#define ATH_WMAC_LEN		0x1ffff /* XXX:Check this */

// 32'h180b8024 (MDIO_PHY_ADDR)
#define ATH_MDIO_PHY_ADDR                           0x7
#define ATH_MDIO_OUPUT_FUNC                         2

#define MDIO_PHY_ADDR_VALUE_MSB                     2
#define MDIO_PHY_ADDR_VALUE_LSB                     0
#define MDIO_PHY_ADDR_VALUE_MASK                    0x00000007
#define MDIO_PHY_ADDR_VALUE_GET(x)                  (((x) & MDIO_PHY_ADDR_VALUE_MASK) >> MDIO_PHY_ADDR_VALUE_LSB)
#define MDIO_PHY_ADDR_VALUE_SET(x)                  (((x) << MDIO_PHY_ADDR_VALUE_LSB) & MDIO_PHY_ADDR_VALUE_MASK)
#define MDIO_PHY_ADDR_VALUE_RESET                   0x7 
#define MDIO_PHY_ADDR_ADDRESS                       0x180b8024

#define ATH_MDIO_MGMT_CFG_CLK_DIV_28                0x7
#define ATH_MDIO_MAC_MII_MGMT_CFG                   0x19000020


#define GPIO_OE_ADDRESS                                              0x18040000

// 32'h1804002c (GPIO_OUT_FUNCTION0)
#define GPIO_OUT_FUNCTION0_ENABLE_GPIO_3_MSB                         31
#define GPIO_OUT_FUNCTION0_ENABLE_GPIO_3_LSB                         24
#define GPIO_OUT_FUNCTION0_ENABLE_GPIO_3_MASK                        0xff000000
#define GPIO_OUT_FUNCTION0_ENABLE_GPIO_3_GET(x)                      (((x) & GPIO_OUT_FUNCTION0_ENABLE_GPIO_3_MASK) >> GPIO_OUT_FUNCTION0_ENABLE_GPIO_3_LSB)
#define GPIO_OUT_FUNCTION0_ENABLE_GPIO_3_SET(x)                      (((x) << GPIO_OUT_FUNCTION0_ENABLE_GPIO_3_LSB) & GPIO_OUT_FUNCTION0_ENABLE_GPIO_3_MASK)
#define GPIO_OUT_FUNCTION0_ENABLE_GPIO_3_RESET                       0x0 // 0
#define GPIO_OUT_FUNCTION0_ENABLE_GPIO_2_MSB                         23
#define GPIO_OUT_FUNCTION0_ENABLE_GPIO_2_LSB                         16
#define GPIO_OUT_FUNCTION0_ENABLE_GPIO_2_MASK                        0x00ff0000
#define GPIO_OUT_FUNCTION0_ENABLE_GPIO_2_GET(x)                      (((x) & GPIO_OUT_FUNCTION0_ENABLE_GPIO_2_MASK) >> GPIO_OUT_FUNCTION0_ENABLE_GPIO_2_LSB)
#define GPIO_OUT_FUNCTION0_ENABLE_GPIO_2_SET(x)                      (((x) << GPIO_OUT_FUNCTION0_ENABLE_GPIO_2_LSB) & GPIO_OUT_FUNCTION0_ENABLE_GPIO_2_MASK)
#define GPIO_OUT_FUNCTION0_ENABLE_GPIO_2_RESET                       0x0 // 0
#define GPIO_OUT_FUNCTION0_ENABLE_GPIO_1_MSB                         15
#define GPIO_OUT_FUNCTION0_ENABLE_GPIO_1_LSB                         8
#define GPIO_OUT_FUNCTION0_ENABLE_GPIO_1_MASK                        0x0000ff00
#define GPIO_OUT_FUNCTION0_ENABLE_GPIO_1_GET(x)                      (((x) & GPIO_OUT_FUNCTION0_ENABLE_GPIO_1_MASK) >> GPIO_OUT_FUNCTION0_ENABLE_GPIO_1_LSB)
#define GPIO_OUT_FUNCTION0_ENABLE_GPIO_1_SET(x)                      (((x) << GPIO_OUT_FUNCTION0_ENABLE_GPIO_1_LSB) & GPIO_OUT_FUNCTION0_ENABLE_GPIO_1_MASK)
#define GPIO_OUT_FUNCTION0_ENABLE_GPIO_1_RESET                       0x0 // 0
#define GPIO_OUT_FUNCTION0_ENABLE_GPIO_0_MSB                         7
#define GPIO_OUT_FUNCTION0_ENABLE_GPIO_0_LSB                         0
#define GPIO_OUT_FUNCTION0_ENABLE_GPIO_0_MASK                        0x000000ff
#define GPIO_OUT_FUNCTION0_ENABLE_GPIO_0_GET(x)                      (((x) & GPIO_OUT_FUNCTION0_ENABLE_GPIO_0_MASK) >> GPIO_OUT_FUNCTION0_ENABLE_GPIO_0_LSB)
#define GPIO_OUT_FUNCTION0_ENABLE_GPIO_0_SET(x)                      (((x) << GPIO_OUT_FUNCTION0_ENABLE_GPIO_0_LSB) & GPIO_OUT_FUNCTION0_ENABLE_GPIO_0_MASK)
#define GPIO_OUT_FUNCTION0_ENABLE_GPIO_0_RESET                       0x0 // 0
#define GPIO_OUT_FUNCTION0_ADDRESS                                   0x1804002c

#define GPIO_OUT_FUNCTION1_ENABLE_GPIO_4_MSB                         7
#define GPIO_OUT_FUNCTION1_ENABLE_GPIO_4_LSB                         0
#define GPIO_OUT_FUNCTION1_ENABLE_GPIO_4_MASK                        0x000000ff
#define GPIO_OUT_FUNCTION1_ENABLE_GPIO_4_GET(x)                      (((x) & GPIO_OUT_FUNCTION1_ENABLE_GPIO_4_MASK) >> GPIO_OUT_FUNCTION1_ENABLE_GPIO_4_LSB)
#define GPIO_OUT_FUNCTION1_ENABLE_GPIO_4_SET(x)                      (((x) << GPIO_OUT_FUNCTION1_ENABLE_GPIO_4_LSB) & GPIO_OUT_FUNCTION1_ENABLE_GPIO_4_MASK)
#define GPIO_OUT_FUNCTION1_ENABLE_GPIO_4_RESET                       0x0 // 0
#define GPIO_OUT_FUNCTION1_ADDRESS                                   0x18040030

#define GPIO_OUT_FUNCTION2_ENABLE_GPIO_11_MSB                        31
#define GPIO_OUT_FUNCTION2_ENABLE_GPIO_11_LSB                        24
#define GPIO_OUT_FUNCTION2_ENABLE_GPIO_11_MASK                       0xff000000
#define GPIO_OUT_FUNCTION2_ENABLE_GPIO_11_GET(x)                     (((x) & GPIO_OUT_FUNCTION2_ENABLE_GPIO_11_MASK) >> GPIO_OUT_FUNCTION2_ENABLE_GPIO_11_LSB)
#define GPIO_OUT_FUNCTION2_ENABLE_GPIO_11_SET(x)                     (((x) << GPIO_OUT_FUNCTION2_ENABLE_GPIO_11_LSB) & GPIO_OUT_FUNCTION2_ENABLE_GPIO_11_MASK)
#define GPIO_OUT_FUNCTION2_ENABLE_GPIO_11_RESET                      0x0 // 0
#define GPIO_OUT_FUNCTION2_ENABLE_GPIO_10_MSB                        23
#define GPIO_OUT_FUNCTION2_ENABLE_GPIO_10_LSB                        16
#define GPIO_OUT_FUNCTION2_ENABLE_GPIO_10_MASK                       0x00ff0000
#define GPIO_OUT_FUNCTION2_ENABLE_GPIO_10_GET(x)                     (((x) & GPIO_OUT_FUNCTION2_ENABLE_GPIO_10_MASK) >> GPIO_OUT_FUNCTION2_ENABLE_GPIO_10_LSB)
#define GPIO_OUT_FUNCTION2_ENABLE_GPIO_10_SET(x)                     (((x) << GPIO_OUT_FUNCTION2_ENABLE_GPIO_10_LSB) & GPIO_OUT_FUNCTION2_ENABLE_GPIO_10_MASK)
#define GPIO_OUT_FUNCTION2_ENABLE_GPIO_10_RESET                      0x0 // 0
#define GPIO_OUT_FUNCTION2_ENABLE_GPIO_9_MSB                         15
#define GPIO_OUT_FUNCTION2_ENABLE_GPIO_9_LSB                         8
#define GPIO_OUT_FUNCTION2_ENABLE_GPIO_9_MASK                        0x0000ff00
#define GPIO_OUT_FUNCTION2_ENABLE_GPIO_9_GET(x)                      (((x) & GPIO_OUT_FUNCTION2_ENABLE_GPIO_9_MASK) >> GPIO_OUT_FUNCTION2_ENABLE_GPIO_9_LSB)
#define GPIO_OUT_FUNCTION2_ENABLE_GPIO_9_SET(x)                      (((x) << GPIO_OUT_FUNCTION2_ENABLE_GPIO_9_LSB) & GPIO_OUT_FUNCTION2_ENABLE_GPIO_9_MASK)
#define GPIO_OUT_FUNCTION2_ENABLE_GPIO_9_RESET                       0x0 // 0
#define GPIO_OUT_FUNCTION2_ENABLE_GPIO_8_MSB                         7
#define GPIO_OUT_FUNCTION2_ENABLE_GPIO_8_LSB                         0
#define GPIO_OUT_FUNCTION2_ENABLE_GPIO_8_MASK                        0x000000ff
#define GPIO_OUT_FUNCTION2_ENABLE_GPIO_8_GET(x)                      (((x) & GPIO_OUT_FUNCTION2_ENABLE_GPIO_8_MASK) >> GPIO_OUT_FUNCTION2_ENABLE_GPIO_8_LSB)
#define GPIO_OUT_FUNCTION2_ENABLE_GPIO_8_SET(x)                      (((x) << GPIO_OUT_FUNCTION2_ENABLE_GPIO_8_LSB) & GPIO_OUT_FUNCTION2_ENABLE_GPIO_8_MASK)
#define GPIO_OUT_FUNCTION2_ENABLE_GPIO_8_RESET                       0x0 // 0
#define GPIO_OUT_FUNCTION2_ADDRESS                                   0x18040034

#define GPIO_OUT_FUNCTION3_ENABLE_GPIO_15_MSB                        31
#define GPIO_OUT_FUNCTION3_ENABLE_GPIO_15_LSB                        24
#define GPIO_OUT_FUNCTION3_ENABLE_GPIO_15_MASK                       0xff000000
#define GPIO_OUT_FUNCTION3_ENABLE_GPIO_15_GET(x)                     (((x) & GPIO_OUT_FUNCTION3_ENABLE_GPIO_15_MASK) >> GPIO_OUT_FUNCTION3_ENABLE_GPIO_15_LSB)
#define GPIO_OUT_FUNCTION3_ENABLE_GPIO_15_SET(x)                     (((x) << GPIO_OUT_FUNCTION3_ENABLE_GPIO_15_LSB) & GPIO_OUT_FUNCTION3_ENABLE_GPIO_15_MASK)
#define GPIO_OUT_FUNCTION3_ENABLE_GPIO_15_RESET                      0x0 // 0
#define GPIO_OUT_FUNCTION3_ENABLE_GPIO_14_MSB                        23
#define GPIO_OUT_FUNCTION3_ENABLE_GPIO_14_LSB                        16
#define GPIO_OUT_FUNCTION3_ENABLE_GPIO_14_MASK                       0x00ff0000
#define GPIO_OUT_FUNCTION3_ENABLE_GPIO_14_GET(x)                     (((x) & GPIO_OUT_FUNCTION3_ENABLE_GPIO_14_MASK) >> GPIO_OUT_FUNCTION3_ENABLE_GPIO_14_LSB)
#define GPIO_OUT_FUNCTION3_ENABLE_GPIO_14_SET(x)                     (((x) << GPIO_OUT_FUNCTION3_ENABLE_GPIO_14_LSB) & GPIO_OUT_FUNCTION3_ENABLE_GPIO_14_MASK)
#define GPIO_OUT_FUNCTION3_ENABLE_GPIO_14_RESET                      0x0 // 0
#define GPIO_OUT_FUNCTION3_ENABLE_GPIO_13_MSB                        15
#define GPIO_OUT_FUNCTION3_ENABLE_GPIO_13_LSB                        8
#define GPIO_OUT_FUNCTION3_ENABLE_GPIO_13_MASK                       0x0000ff00
#define GPIO_OUT_FUNCTION3_ENABLE_GPIO_13_GET(x)                     (((x) & GPIO_OUT_FUNCTION3_ENABLE_GPIO_13_MASK) >> GPIO_OUT_FUNCTION3_ENABLE_GPIO_13_LSB)
#define GPIO_OUT_FUNCTION3_ENABLE_GPIO_13_SET(x)                     (((x) << GPIO_OUT_FUNCTION3_ENABLE_GPIO_13_LSB) & GPIO_OUT_FUNCTION3_ENABLE_GPIO_13_MASK)
#define GPIO_OUT_FUNCTION3_ENABLE_GPIO_13_RESET                      0x0 // 0
#define GPIO_OUT_FUNCTION3_ENABLE_GPIO_12_MSB                        7
#define GPIO_OUT_FUNCTION3_ENABLE_GPIO_12_LSB                        0
#define GPIO_OUT_FUNCTION3_ENABLE_GPIO_12_MASK                       0x000000ff
#define GPIO_OUT_FUNCTION3_ENABLE_GPIO_12_GET(x)                     (((x) & GPIO_OUT_FUNCTION3_ENABLE_GPIO_12_MASK) >> GPIO_OUT_FUNCTION3_ENABLE_GPIO_12_LSB)
#define GPIO_OUT_FUNCTION3_ENABLE_GPIO_12_SET(x)                     (((x) << GPIO_OUT_FUNCTION3_ENABLE_GPIO_12_LSB) & GPIO_OUT_FUNCTION3_ENABLE_GPIO_12_MASK)
#define GPIO_OUT_FUNCTION3_ENABLE_GPIO_12_RESET                      0x0 // 0
#define GPIO_OUT_FUNCTION3_ADDRESS                                   0x18040038

#define GPIO_OUT_FUNCTION4_ENABLE_GPIO_19_MSB                        31
#define GPIO_OUT_FUNCTION4_ENABLE_GPIO_19_LSB                        24
#define GPIO_OUT_FUNCTION4_ENABLE_GPIO_19_MASK                       0xff000000
#define GPIO_OUT_FUNCTION4_ENABLE_GPIO_19_GET(x)                     (((x) & GPIO_OUT_FUNCTION4_ENABLE_GPIO_19_MASK) >> GPIO_OUT_FUNCTION4_ENABLE_GPIO_19_LSB)
#define GPIO_OUT_FUNCTION4_ENABLE_GPIO_19_SET(x)                     (((x) << GPIO_OUT_FUNCTION4_ENABLE_GPIO_19_LSB) & GPIO_OUT_FUNCTION4_ENABLE_GPIO_19_MASK)
#define GPIO_OUT_FUNCTION4_ENABLE_GPIO_19_RESET                      0x0 // 0
#define GPIO_OUT_FUNCTION4_ENABLE_GPIO_18_MSB                        23
#define GPIO_OUT_FUNCTION4_ENABLE_GPIO_18_LSB                        16
#define GPIO_OUT_FUNCTION4_ENABLE_GPIO_18_MASK                       0x00ff0000
#define GPIO_OUT_FUNCTION4_ENABLE_GPIO_18_GET(x)                     (((x) & GPIO_OUT_FUNCTION4_ENABLE_GPIO_18_MASK) >> GPIO_OUT_FUNCTION4_ENABLE_GPIO_18_LSB)
#define GPIO_OUT_FUNCTION4_ENABLE_GPIO_18_SET(x)                     (((x) << GPIO_OUT_FUNCTION4_ENABLE_GPIO_18_LSB) & GPIO_OUT_FUNCTION4_ENABLE_GPIO_18_MASK)
#define GPIO_OUT_FUNCTION4_ENABLE_GPIO_18_RESET                      0x0 // 0
#define GPIO_OUT_FUNCTION4_ENABLE_GPIO_17_MSB                        15
#define GPIO_OUT_FUNCTION4_ENABLE_GPIO_17_LSB                        8
#define GPIO_OUT_FUNCTION4_ENABLE_GPIO_17_MASK                       0x0000ff00
#define GPIO_OUT_FUNCTION4_ENABLE_GPIO_17_GET(x)                     (((x) & GPIO_OUT_FUNCTION4_ENABLE_GPIO_17_MASK) >> GPIO_OUT_FUNCTION4_ENABLE_GPIO_17_LSB)
#define GPIO_OUT_FUNCTION4_ENABLE_GPIO_17_SET(x)                     (((x) << GPIO_OUT_FUNCTION4_ENABLE_GPIO_17_LSB) & GPIO_OUT_FUNCTION4_ENABLE_GPIO_17_MASK)
#define GPIO_OUT_FUNCTION4_ENABLE_GPIO_17_RESET                      0x0 // 0
#define GPIO_OUT_FUNCTION4_ENABLE_GPIO_16_MSB                        7
#define GPIO_OUT_FUNCTION4_ENABLE_GPIO_16_LSB                        0
#define GPIO_OUT_FUNCTION4_ENABLE_GPIO_16_MASK                       0x000000ff
#define GPIO_OUT_FUNCTION4_ENABLE_GPIO_16_GET(x)                     (((x) & GPIO_OUT_FUNCTION4_ENABLE_GPIO_16_MASK) >> GPIO_OUT_FUNCTION4_ENABLE_GPIO_16_LSB)
#define GPIO_OUT_FUNCTION4_ENABLE_GPIO_16_SET(x)                     (((x) << GPIO_OUT_FUNCTION4_ENABLE_GPIO_16_LSB) & GPIO_OUT_FUNCTION4_ENABLE_GPIO_16_MASK)
#define GPIO_OUT_FUNCTION4_ENABLE_GPIO_16_RESET                      0x0 // 0
#define GPIO_OUT_FUNCTION4_ADDRESS                                   0x1804003c

#define GPIO_OUT_FUNCTION5_ENABLE_GPIO_23_MSB                        31
#define GPIO_OUT_FUNCTION5_ENABLE_GPIO_23_LSB                        24
#define GPIO_OUT_FUNCTION5_ENABLE_GPIO_23_MASK                       0xff000000
#define GPIO_OUT_FUNCTION5_ENABLE_GPIO_23_GET(x)                     (((x) & GPIO_OUT_FUNCTION5_ENABLE_GPIO_23_MASK) >> GPIO_OUT_FUNCTION5_ENABLE_GPIO_23_LSB)
#define GPIO_OUT_FUNCTION5_ENABLE_GPIO_23_SET(x)                     (((x) << GPIO_OUT_FUNCTION5_ENABLE_GPIO_23_LSB) & GPIO_OUT_FUNCTION5_ENABLE_GPIO_23_MASK)
#define GPIO_OUT_FUNCTION5_ENABLE_GPIO_23_RESET                      0x0 // 0
#define GPIO_OUT_FUNCTION5_ENABLE_GPIO_22_MSB                        23
#define GPIO_OUT_FUNCTION5_ENABLE_GPIO_22_LSB                        16
#define GPIO_OUT_FUNCTION5_ENABLE_GPIO_22_MASK                       0x00ff0000
#define GPIO_OUT_FUNCTION5_ENABLE_GPIO_22_GET(x)                     (((x) & GPIO_OUT_FUNCTION5_ENABLE_GPIO_22_MASK) >> GPIO_OUT_FUNCTION5_ENABLE_GPIO_22_LSB)
#define GPIO_OUT_FUNCTION5_ENABLE_GPIO_22_SET(x)                     (((x) << GPIO_OUT_FUNCTION5_ENABLE_GPIO_22_LSB) & GPIO_OUT_FUNCTION5_ENABLE_GPIO_22_MASK)
#define GPIO_OUT_FUNCTION5_ENABLE_GPIO_22_RESET                      0x0 // 0
#define GPIO_OUT_FUNCTION5_ENABLE_GPIO_21_MSB                        15
#define GPIO_OUT_FUNCTION5_ENABLE_GPIO_21_LSB                        8
#define GPIO_OUT_FUNCTION5_ENABLE_GPIO_21_MASK                       0x0000ff00
#define GPIO_OUT_FUNCTION5_ENABLE_GPIO_21_GET(x)                     (((x) & GPIO_OUT_FUNCTION5_ENABLE_GPIO_21_MASK) >> GPIO_OUT_FUNCTION5_ENABLE_GPIO_21_LSB)
#define GPIO_OUT_FUNCTION5_ENABLE_GPIO_21_SET(x)                     (((x) << GPIO_OUT_FUNCTION5_ENABLE_GPIO_21_LSB) & GPIO_OUT_FUNCTION5_ENABLE_GPIO_21_MASK)
#define GPIO_OUT_FUNCTION5_ENABLE_GPIO_21_RESET                      0x0 // 0
#define GPIO_OUT_FUNCTION5_ENABLE_GPIO_20_MSB                        7
#define GPIO_OUT_FUNCTION5_ENABLE_GPIO_20_LSB                        0
#define GPIO_OUT_FUNCTION5_ENABLE_GPIO_20_MASK                       0x000000ff
#define GPIO_OUT_FUNCTION5_ENABLE_GPIO_20_GET(x)                     (((x) & GPIO_OUT_FUNCTION5_ENABLE_GPIO_20_MASK) >> GPIO_OUT_FUNCTION5_ENABLE_GPIO_20_LSB)
#define GPIO_OUT_FUNCTION5_ENABLE_GPIO_20_SET(x)                     (((x) << GPIO_OUT_FUNCTION5_ENABLE_GPIO_20_LSB) & GPIO_OUT_FUNCTION5_ENABLE_GPIO_20_MASK)
#define GPIO_OUT_FUNCTION5_ENABLE_GPIO_20_RESET                      0x0 // 0
#define GPIO_OUT_FUNCTION5_ADDRESS                                   0x18040040

#define GPIO_IN_ENABLE3_MII_GE1_MDI_MSB                              23
#define GPIO_IN_ENABLE3_MII_GE1_MDI_LSB                              16
#define GPIO_IN_ENABLE3_MII_GE1_MDI_MASK                             0x00ff0000
#define GPIO_IN_ENABLE3_MII_GE1_MDI_GET(x)                           (((x) & GPIO_IN_ENABLE3_MII_GE1_MDI_MASK) >> GPIO_IN_ENABLE3_MII_GE1_MDI_LSB)
#define GPIO_IN_ENABLE3_MII_GE1_MDI_SET(x)                           (((x) << GPIO_IN_ENABLE3_MII_GE1_MDI_LSB) & GPIO_IN_ENABLE3_MII_GE1_MDI_MASK)
#define GPIO_IN_ENABLE3_MII_GE1_MDI_RESET                            0x80 // 128
#define GPIO_IN_ENABLE3_BOOT_EXT_MDC_MSB            31
#define GPIO_IN_ENABLE3_BOOT_EXT_MDC_LSB            24
#define GPIO_IN_ENABLE3_BOOT_EXT_MDC_MASK           0xff000000
#define GPIO_IN_ENABLE3_BOOT_EXT_MDC_GET(x)         (((x) & GPIO_IN_ENABLE3_BOOT_EXT_MDC_MASK) >> GPIO_IN_ENABLE3_BOOT_EXT_MDC_LSB)
#define GPIO_IN_ENABLE3_BOOT_EXT_MDC_SET(x)         (((x) << GPIO_IN_ENABLE3_BOOT_EXT_MDC_LSB) & GPIO_IN_ENABLE3_BOOT_EXT_MDC_MASK)
#define GPIO_IN_ENABLE3_BOOT_EXT_MDC_RESET          0x80 // 128

#define GPIO_IN_ENABLE3_BOOT_EXT_MDO_MSB            23
#define GPIO_IN_ENABLE3_BOOT_EXT_MDO_LSB            16
#define GPIO_IN_ENABLE3_BOOT_EXT_MDO_MASK           0x00ff0000
#define GPIO_IN_ENABLE3_BOOT_EXT_MDO_GET(x)         (((x) & GPIO_IN_ENABLE3_BOOT_EXT_MDO_MASK) >> GPIO_IN_ENABLE3_BOOT_EXT_MDO_LSB)
#define GPIO_IN_ENABLE3_BOOT_EXT_MDO_SET(x)         (((x) << GPIO_IN_ENABLE3_BOOT_EXT_MDO_LSB) & GPIO_IN_ENABLE3_BOOT_EXT_MDO_MASK)
#define GPIO_IN_ENABLE3_BOOT_EXT_MDO_RESET          0x80 /* 128 */
#define GPIO_IN_ENABLE3_ADDRESS                     0x18040050



#ifdef CONFIG_ATHRS_HW_NAT

/*
 * Ingress / Egress LUT Registers
 */
#define ATHR_EGRESS_LUT_REG		(ATH_APB_BASE + 0x80000)
#define ATHR_INGRESS_LUT_REG		(ATH_APB_BASE + 0x81000)

/*
 * Fragment LUT Registers
 */
#define ATHR_EGRESS_LUT_FRAG_REG	(ATH_APB_BASE + 0x82000)
#define ATHR_INGRESS_LUT_FRAG_REG	(ATH_APB_BASE + 0x83000)
#define ATHR_WAN_IP_ADDR_REG		(ATH_GE0_BASE + 0x210)

/*
 * Nat status Registers
 */
#define ETH_EG_NAT_STATUS		(ATH_GE0_BASE + 0x228)
#define ETH_IG_NAT_STATUS		(ATH_GE0_BASE + 0x230)

/*
 * Ager Registers
 */
#define ETH_EG_AGER_FIFO_REG		(ATH_APB_BASE + 0x80038)
#define ETH_IG_AGER_FIFO_REG		(ATH_APB_BASE + 0x81040)

/*
 *
 * Nat counter Registers
 */
#define ETH_EG_NAT_CNTR			(ATH_GE0_BASE + 0x22c)
#define ETH_IG_NAT_CNTR			(ATH_GE0_BASE + 0x234)

/*
 * Nat debug Registers
 */
#define ETH_EG_NAT_DBG			(ATH_GE0_BASE + 0x310)
#define ETH_IG_NAT_DBG			(ATH_GE0_BASE + 0x2f8)

/*
 * MAC Addr Registers
 */

#define ETH_LCL_MAC_ADDR_DW0		(ATH_GE0_BASE + 0x200)
#define ETH_LCL_MAC_ADDR_DW1		(ATH_GE0_BASE + 0x204)
#define ETH_DST_MAC_ADDR_DW0		(ATH_GE0_BASE + 0x208)
#define ETH_DST_MAC_ADDR_DW1		(ATH_GE0_BASE + 0x20c)

/*
 * Ager INTR Registers
 */
#define ATHR_LUTS_AGER_INTR		(ATH_APB_BASE + 0x70004)
#define ATHR_LUTS_AGER_INTR_MASK	(ATH_APB_BASE + 0x70008)

#endif /* #ifdef CONFIG_ATHRS_HW_NAT */

#ifdef CONFIG_ATHRS_HW_ACL

/********************************************************************
 * EG ACL Registers
 *********************************************************************/
#define ATHR_EG_ACL_STATUS		(ATH_GE0_BASE + 0x238)
#define ATHR_EG_ACL_CMD0_AND_ACTION	(ATH_GE0_BASE + 0x240)
#define ATHR_EG_ACL_CMD1234		(ATH_GE0_BASE + 0x244)
#define ATHR_EG_ACL_OPERAND0		(ATH_GE0_BASE + 0x248)
#define ATHR_EG_ACL_OPERAND1		(ATH_GE0_BASE + 0x24c)
#define ATHR_EG_ACL_MEM_CTRL		(ATH_GE0_BASE + 0x250)
#define ATHR_EG_ACL_RULE_TABLE_FIELDS	(ATH_GE0_BASE + 0x378)
#define ATHR_EG_ACL_RULE_TABLE0_LOWER	(ATH_GE0_BASE + 0x354)
#define ATHR_EG_ACL_RULE_TABLE0_UPPER	(ATH_GE0_BASE + 0x358)
#define ATHR_EG_ACL_RULE_TABLE1_LOWER	(ATH_GE0_BASE + 0x35c)
#define ATHR_EG_ACL_RULE_TABLE1_UPPER	(ATH_GE0_BASE + 0x360)
#define ATHR_EG_ACL_RULE_TABLE2_LOWER	(ATH_GE0_BASE + 0x364)
#define ATHR_EG_ACL_RULE_TABLE2_UPPER	(ATH_GE0_BASE + 0x368)
#define ATHR_EG_ACL_RULE_TABLE3_LOWER	(ATH_GE0_BASE + 0x36c)
#define ATHR_EG_ACL_RULE_TABLE3_UPPER	(ATH_GE0_BASE + 0x370)

/********************************************************************
 * IG ACL Registers
 ********************************************************************/
#define ATHR_IG_ACL_STATUS		(ATH_GE0_BASE + 0x23C)
#define ATHR_IG_ACL_CMD0_AND_ACTION	(ATH_GE0_BASE + 0x254)
#define ATHR_IG_ACL_CMD1234		(ATH_GE0_BASE + 0x258)
#define ATHR_IG_ACL_OPERAND0		(ATH_GE0_BASE + 0x25C)
#define ATHR_IG_ACL_OPERAND1		(ATH_GE0_BASE + 0x260)
#define ATHR_IG_ACL_MEM_CTRL		(ATH_GE0_BASE + 0x264)
#define ATHR_IG_ACL_RULE_TABLE_FIELDS	(ATH_GE0_BASE + 0x374)
#define ATHR_IG_ACL_RULE_TABLE0_LOWER	(ATH_GE0_BASE + 0x334)
#define ATHR_IG_ACL_RULE_TABLE0_UPPER	(ATH_GE0_BASE + 0x338)
#define ATHR_IG_ACL_RULE_TABLE1_LOWER	(ATH_GE0_BASE + 0x33c)
#define ATHR_IG_ACL_RULE_TABLE1_UPPER	(ATH_GE0_BASE + 0x340)
#define ATHR_IG_ACL_RULE_TABLE2_LOWER	(ATH_GE0_BASE + 0x344)
#define ATHR_IG_ACL_RULE_TABLE2_UPPER	(ATH_GE0_BASE + 0x348)
#define ATHR_IG_ACL_RULE_TABLE3_LOWER	(ATH_GE0_BASE + 0x34c)
#define ATHR_IG_ACL_RULE_TABLE3_UPPER	(ATH_GE0_BASE + 0x350)
#define ATHR_ACL_GMAC_DMA_TX_CTRL	(ATH_GE0_BASE + 0x180)
#define ATHR_ACL_GMAC_DMA_XFIFO_DEPTH	(ATH_GE0_BASE + 0x1a8)
#define ATHR_ACL_GMAC_CFG1		(ATH_GE0_BASE + 0x00)

#endif /* #ifdef CONFIG_ATHRS_HW_ACL */

/*
 * From
 * //depot/chips/wasp/1.0/rtl/otp_intf/blueprint/efuse_reg.h
 */


// 32'h18070000 (ETH_CFG)
#define ETH_CFG_ETH_SPARE_MSB                                        31
#define ETH_CFG_ETH_SPARE_LSB                                        22
#define ETH_CFG_ETH_SPARE_MASK                                       0xffc00000
#define ETH_CFG_ETH_SPARE_GET(x)                                     (((x) & ETH_CFG_ETH_SPARE_MASK) >> ETH_CFG_ETH_SPARE_LSB)
#define ETH_CFG_ETH_SPARE_SET(x)                                     (((x) << ETH_CFG_ETH_SPARE_LSB) & ETH_CFG_ETH_SPARE_MASK)
#define ETH_CFG_ETH_SPARE_RESET                                      0x0 // 0
#define ETH_CFG_ETH_TXEN_DELAY_MSB                                   21
#define ETH_CFG_ETH_TXEN_DELAY_LSB                                   20
#define ETH_CFG_ETH_TXEN_DELAY_MASK                                  0x00300000
#define ETH_CFG_ETH_TXEN_DELAY_GET(x)                                (((x) & ETH_CFG_ETH_TXEN_DELAY_MASK) >> ETH_CFG_ETH_TXEN_DELAY_LSB)
#define ETH_CFG_ETH_TXEN_DELAY_SET(x)                                (((x) << ETH_CFG_ETH_TXEN_DELAY_LSB) & ETH_CFG_ETH_TXEN_DELAY_MASK)
#define ETH_CFG_ETH_TXEN_DELAY_RESET                                 0x0 // 0
#define ETH_CFG_ETH_TXD_DELAY_MSB                                    19
#define ETH_CFG_ETH_TXD_DELAY_LSB                                    18
#define ETH_CFG_ETH_TXD_DELAY_MASK                                   0x000c0000
#define ETH_CFG_ETH_TXD_DELAY_GET(x)                                 (((x) & ETH_CFG_ETH_TXD_DELAY_MASK) >> ETH_CFG_ETH_TXD_DELAY_LSB)
#define ETH_CFG_ETH_TXD_DELAY_SET(x)                                 (((x) << ETH_CFG_ETH_TXD_DELAY_LSB) & ETH_CFG_ETH_TXD_DELAY_MASK)
#define ETH_CFG_ETH_TXD_DELAY_RESET                                  0x0 // 0
#define ETH_CFG_ETH_RXDV_DELAY_MSB                                   17
#define ETH_CFG_ETH_RXDV_DELAY_LSB                                   16
#define ETH_CFG_ETH_RXDV_DELAY_MASK                                  0x00030000
#define ETH_CFG_ETH_RXDV_DELAY_GET(x)                                (((x) & ETH_CFG_ETH_RXDV_DELAY_MASK) >> ETH_CFG_ETH_RXDV_DELAY_LSB)
#define ETH_CFG_ETH_RXDV_DELAY_SET(x)                                (((x) << ETH_CFG_ETH_RXDV_DELAY_LSB) & ETH_CFG_ETH_RXDV_DELAY_MASK)
#define ETH_CFG_ETH_RXDV_DELAY_RESET                                 0x0 // 0
#define ETH_CFG_ETH_RXD_DELAY_MSB                                    15
#define ETH_CFG_ETH_RXD_DELAY_LSB                                    14
#define ETH_CFG_ETH_RXD_DELAY_MASK                                   0x0000c000
#define ETH_CFG_ETH_RXD_DELAY_GET(x)                                 (((x) & ETH_CFG_ETH_RXD_DELAY_MASK) >> ETH_CFG_ETH_RXD_DELAY_LSB)
#define ETH_CFG_ETH_RXD_DELAY_SET(x)                                 (((x) << ETH_CFG_ETH_RXD_DELAY_LSB) & ETH_CFG_ETH_RXD_DELAY_MASK)
#define ETH_CFG_ETH_RXD_DELAY_RESET                                  0x0 // 0
#define ETH_CFG_RMII_GE0_MASTER_MSB                                  12
#define ETH_CFG_RMII_GE0_MASTER_LSB                                  12
#define ETH_CFG_RMII_GE0_MASTER_MASK                                 0x00001000
#define ETH_CFG_RMII_GE0_MASTER_GET(x)                               (((x) & ETH_CFG_RMII_GE0_MASTER_MASK) >> ETH_CFG_RMII_GE0_MASTER_LSB)
#define ETH_CFG_RMII_GE0_MASTER_SET(x)                               (((x) << ETH_CFG_RMII_GE0_MASTER_LSB) & ETH_CFG_RMII_GE0_MASTER_MASK)
#define ETH_CFG_RMII_GE0_MASTER_RESET                                0x1 // 1
#define ETH_CFG_MII_CNTL_SPEED_MSB                                   11
#define ETH_CFG_MII_CNTL_SPEED_LSB                                   11
#define ETH_CFG_MII_CNTL_SPEED_MASK                                  0x00000800
#define ETH_CFG_MII_CNTL_SPEED_GET(x)                                (((x) & ETH_CFG_MII_CNTL_SPEED_MASK) >> ETH_CFG_MII_CNTL_SPEED_LSB)
#define ETH_CFG_MII_CNTL_SPEED_SET(x)                                (((x) << ETH_CFG_MII_CNTL_SPEED_LSB) & ETH_CFG_MII_CNTL_SPEED_MASK)
#define ETH_CFG_MII_CNTL_SPEED_RESET                                 0x0 // 0
#define ETH_CFG_RMII_GE0_MSB                                         10
#define ETH_CFG_RMII_GE0_LSB                                         10
#define ETH_CFG_RMII_GE0_MASK                                        0x00000400
#define ETH_CFG_RMII_GE0_GET(x)                                      (((x) & ETH_CFG_RMII_GE0_MASK) >> ETH_CFG_RMII_GE0_LSB)
#define ETH_CFG_RMII_GE0_SET(x)                                      (((x) << ETH_CFG_RMII_GE0_LSB) & ETH_CFG_RMII_GE0_MASK)
#define ETH_CFG_RMII_GE0_RESET                                       0x0 // 0
#define ETH_CFG_GE0_SGMII_MSB                                        6
#define ETH_CFG_GE0_SGMII_LSB                                        6
#define ETH_CFG_GE0_SGMII_MASK                                       0x00000040
#define ETH_CFG_GE0_SGMII_GET(x)                                     (((x) & ETH_CFG_GE0_SGMII_MASK) >> ETH_CFG_GE0_SGMII_LSB)
#define ETH_CFG_GE0_SGMII_SET(x)                                     (((x) << ETH_CFG_GE0_SGMII_LSB) & ETH_CFG_GE0_SGMII_MASK)
#define ETH_CFG_GE0_SGMII_RESET                                      0x0 // 0
#define ETH_CFG_GE0_ERR_EN_MSB                                       5
#define ETH_CFG_GE0_ERR_EN_LSB                                       5
#define ETH_CFG_GE0_ERR_EN_MASK                                      0x00000020
#define ETH_CFG_GE0_ERR_EN_GET(x)                                    (((x) & ETH_CFG_GE0_ERR_EN_MASK) >> ETH_CFG_GE0_ERR_EN_LSB)
#define ETH_CFG_GE0_ERR_EN_SET(x)                                    (((x) << ETH_CFG_GE0_ERR_EN_LSB) & ETH_CFG_GE0_ERR_EN_MASK)
#define ETH_CFG_GE0_ERR_EN_RESET                                     0x0 // 0
#define ETH_CFG_MII_GE0_SLAVE_MSB                                    4
#define ETH_CFG_MII_GE0_SLAVE_LSB                                    4
#define ETH_CFG_MII_GE0_SLAVE_MASK                                   0x00000010
#define ETH_CFG_MII_GE0_SLAVE_GET(x)                                 (((x) & ETH_CFG_MII_GE0_SLAVE_MASK) >> ETH_CFG_MII_GE0_SLAVE_LSB)
#define ETH_CFG_MII_GE0_SLAVE_SET(x)                                 (((x) << ETH_CFG_MII_GE0_SLAVE_LSB) & ETH_CFG_MII_GE0_SLAVE_MASK)
#define ETH_CFG_MII_GE0_SLAVE_RESET                                  0x0 // 0
#define ETH_CFG_MII_GE0_MASTER_MSB                                   3
#define ETH_CFG_MII_GE0_MASTER_LSB                                   3
#define ETH_CFG_MII_GE0_MASTER_MASK                                  0x00000008
#define ETH_CFG_MII_GE0_MASTER_GET(x)                                (((x) & ETH_CFG_MII_GE0_MASTER_MASK) >> ETH_CFG_MII_GE0_MASTER_LSB)
#define ETH_CFG_MII_GE0_MASTER_SET(x)                                (((x) << ETH_CFG_MII_GE0_MASTER_LSB) & ETH_CFG_MII_GE0_MASTER_MASK)
#define ETH_CFG_MII_GE0_MASTER_RESET                                 0x0 // 0
#define ETH_CFG_GMII_GE0_MSB                                         2
#define ETH_CFG_GMII_GE0_LSB                                         2
#define ETH_CFG_GMII_GE0_MASK                                        0x00000004
#define ETH_CFG_GMII_GE0_GET(x)                                      (((x) & ETH_CFG_GMII_GE0_MASK) >> ETH_CFG_GMII_GE0_LSB)
#define ETH_CFG_GMII_GE0_SET(x)                                      (((x) << ETH_CFG_GMII_GE0_LSB) & ETH_CFG_GMII_GE0_MASK)
#define ETH_CFG_GMII_GE0_RESET                                       0x0 // 0
#define ETH_CFG_MII_GE0_MSB                                          1
#define ETH_CFG_MII_GE0_LSB                                          1
#define ETH_CFG_MII_GE0_MASK                                         0x00000002
#define ETH_CFG_MII_GE0_GET(x)                                       (((x) & ETH_CFG_MII_GE0_MASK) >> ETH_CFG_MII_GE0_LSB)
#define ETH_CFG_MII_GE0_SET(x)                                       (((x) << ETH_CFG_MII_GE0_LSB) & ETH_CFG_MII_GE0_MASK)
#define ETH_CFG_MII_GE0_RESET                                        0x0 // 0
#define ETH_CFG_RGMII_GE0_MSB                                        0
#define ETH_CFG_RGMII_GE0_LSB                                        0
#define ETH_CFG_RGMII_GE0_MASK                                       0x00000001
#define ETH_CFG_RGMII_GE0_GET(x)                                     (((x) & ETH_CFG_RGMII_GE0_MASK) >> ETH_CFG_RGMII_GE0_LSB)
#define ETH_CFG_RGMII_GE0_SET(x)                                     (((x) << ETH_CFG_RGMII_GE0_LSB) & ETH_CFG_RGMII_GE0_MASK)
#define ETH_CFG_RGMII_GE0_RESET                                      0x0 // 0
#define ETH_CFG_ADDRESS                                              0x18070000


/*
 * SGMII Register Defines
 */

/*
 * Sgmii config @0x18070034 contains configuration
 * bit to enable sgmii mode of operation, PRBS, and MDIO.
 */


// 32'h18070034 (SGMII_CONFIG)
#define SGMII_CONFIG_BERT_ENABLE_MSB                                 14
#define SGMII_CONFIG_BERT_ENABLE_LSB                                 14
#define SGMII_CONFIG_BERT_ENABLE_MASK                                0x00004000
#define SGMII_CONFIG_BERT_ENABLE_GET(x)                              (((x) & SGMII_CONFIG_BERT_ENABLE_MASK) >> SGMII_CONFIG_BERT_ENABLE_LSB)
#define SGMII_CONFIG_BERT_ENABLE_SET(x)                              (((x) << SGMII_CONFIG_BERT_ENABLE_LSB) & SGMII_CONFIG_BERT_ENABLE_MASK)
#define SGMII_CONFIG_BERT_ENABLE_RESET                               0x0 // 0
#define SGMII_CONFIG_PRBS_ENABLE_MSB                                 13
#define SGMII_CONFIG_PRBS_ENABLE_LSB                                 13
#define SGMII_CONFIG_PRBS_ENABLE_MASK                                0x00002000
#define SGMII_CONFIG_PRBS_ENABLE_GET(x)                              (((x) & SGMII_CONFIG_PRBS_ENABLE_MASK) >> SGMII_CONFIG_PRBS_ENABLE_LSB)
#define SGMII_CONFIG_PRBS_ENABLE_SET(x)                              (((x) << SGMII_CONFIG_PRBS_ENABLE_LSB) & SGMII_CONFIG_PRBS_ENABLE_MASK)
#define SGMII_CONFIG_PRBS_ENABLE_RESET                               0x0 // 0
#define SGMII_CONFIG_MDIO_COMPLETE_MSB                               12
#define SGMII_CONFIG_MDIO_COMPLETE_LSB                               12
#define SGMII_CONFIG_MDIO_COMPLETE_MASK                              0x00001000
#define SGMII_CONFIG_MDIO_COMPLETE_GET(x)                            (((x) & SGMII_CONFIG_MDIO_COMPLETE_MASK) >> SGMII_CONFIG_MDIO_COMPLETE_LSB)
#define SGMII_CONFIG_MDIO_COMPLETE_SET(x)                            (((x) << SGMII_CONFIG_MDIO_COMPLETE_LSB) & SGMII_CONFIG_MDIO_COMPLETE_MASK)
#define SGMII_CONFIG_MDIO_COMPLETE_RESET                             0x0 // 0
#define SGMII_CONFIG_MDIO_PULSE_MSB                                  11
#define SGMII_CONFIG_MDIO_PULSE_LSB                                  11
#define SGMII_CONFIG_MDIO_PULSE_MASK                                 0x00000800
#define SGMII_CONFIG_MDIO_PULSE_GET(x)                               (((x) & SGMII_CONFIG_MDIO_PULSE_MASK) >> SGMII_CONFIG_MDIO_PULSE_LSB)
#define SGMII_CONFIG_MDIO_PULSE_SET(x)                               (((x) << SGMII_CONFIG_MDIO_PULSE_LSB) & SGMII_CONFIG_MDIO_PULSE_MASK)
#define SGMII_CONFIG_MDIO_PULSE_RESET                                0x0 // 0
#define SGMII_CONFIG_MDIO_ENABLE_MSB                                 10
#define SGMII_CONFIG_MDIO_ENABLE_LSB                                 10
#define SGMII_CONFIG_MDIO_ENABLE_MASK                                0x00000400
#define SGMII_CONFIG_MDIO_ENABLE_GET(x)                              (((x) & SGMII_CONFIG_MDIO_ENABLE_MASK) >> SGMII_CONFIG_MDIO_ENABLE_LSB)
#define SGMII_CONFIG_MDIO_ENABLE_SET(x)                              (((x) << SGMII_CONFIG_MDIO_ENABLE_LSB) & SGMII_CONFIG_MDIO_ENABLE_MASK)
#define SGMII_CONFIG_MDIO_ENABLE_RESET                               0x0 // 0
#define SGMII_CONFIG_NEXT_PAGE_LOADED_MSB                            9
#define SGMII_CONFIG_NEXT_PAGE_LOADED_LSB                            9
#define SGMII_CONFIG_NEXT_PAGE_LOADED_MASK                           0x00000200
#define SGMII_CONFIG_NEXT_PAGE_LOADED_GET(x)                         (((x) & SGMII_CONFIG_NEXT_PAGE_LOADED_MASK) >> SGMII_CONFIG_NEXT_PAGE_LOADED_LSB)
#define SGMII_CONFIG_NEXT_PAGE_LOADED_SET(x)                         (((x) << SGMII_CONFIG_NEXT_PAGE_LOADED_LSB) & SGMII_CONFIG_NEXT_PAGE_LOADED_MASK)
#define SGMII_CONFIG_NEXT_PAGE_LOADED_RESET                          0x0 // 0
#define SGMII_CONFIG_REMOTE_PHY_LOOPBACK_MSB                         8
#define SGMII_CONFIG_REMOTE_PHY_LOOPBACK_LSB                         8
#define SGMII_CONFIG_REMOTE_PHY_LOOPBACK_MASK                        0x00000100
#define SGMII_CONFIG_REMOTE_PHY_LOOPBACK_GET(x)                      (((x) & SGMII_CONFIG_REMOTE_PHY_LOOPBACK_MASK) >> SGMII_CONFIG_REMOTE_PHY_LOOPBACK_LSB)
#define SGMII_CONFIG_REMOTE_PHY_LOOPBACK_SET(x)                      (((x) << SGMII_CONFIG_REMOTE_PHY_LOOPBACK_LSB) & SGMII_CONFIG_REMOTE_PHY_LOOPBACK_MASK)
#define SGMII_CONFIG_REMOTE_PHY_LOOPBACK_RESET                       0x0 // 0
#define SGMII_CONFIG_SPEED_MSB                                       7
#define SGMII_CONFIG_SPEED_LSB                                       6
#define SGMII_CONFIG_SPEED_MASK                                      0x000000c0
#define SGMII_CONFIG_SPEED_GET(x)                                    (((x) & SGMII_CONFIG_SPEED_MASK) >> SGMII_CONFIG_SPEED_LSB)
#define SGMII_CONFIG_SPEED_SET(x)                                    (((x) << SGMII_CONFIG_SPEED_LSB) & SGMII_CONFIG_SPEED_MASK)
#define SGMII_CONFIG_SPEED_RESET                                     0x0 // 0
#define SGMII_CONFIG_FORCE_SPEED_MSB                                 5
#define SGMII_CONFIG_FORCE_SPEED_LSB                                 5
#define SGMII_CONFIG_FORCE_SPEED_MASK                                0x00000020
#define SGMII_CONFIG_FORCE_SPEED_GET(x)                              (((x) & SGMII_CONFIG_FORCE_SPEED_MASK) >> SGMII_CONFIG_FORCE_SPEED_LSB)
#define SGMII_CONFIG_FORCE_SPEED_SET(x)                              (((x) << SGMII_CONFIG_FORCE_SPEED_LSB) & SGMII_CONFIG_FORCE_SPEED_MASK)
#define SGMII_CONFIG_FORCE_SPEED_RESET                               0x0 // 0
#define SGMII_CONFIG_MR_REG4_CHANGED_MSB                             4
#define SGMII_CONFIG_MR_REG4_CHANGED_LSB                             4
#define SGMII_CONFIG_MR_REG4_CHANGED_MASK                            0x00000010
#define SGMII_CONFIG_MR_REG4_CHANGED_GET(x)                          (((x) & SGMII_CONFIG_MR_REG4_CHANGED_MASK) >> SGMII_CONFIG_MR_REG4_CHANGED_LSB)
#define SGMII_CONFIG_MR_REG4_CHANGED_SET(x)                          (((x) << SGMII_CONFIG_MR_REG4_CHANGED_LSB) & SGMII_CONFIG_MR_REG4_CHANGED_MASK)
#define SGMII_CONFIG_MR_REG4_CHANGED_RESET                           0x0 // 0
#define SGMII_CONFIG_ENABLE_SGMII_TX_PAUSE_MSB                       3
#define SGMII_CONFIG_ENABLE_SGMII_TX_PAUSE_LSB                       3
#define SGMII_CONFIG_ENABLE_SGMII_TX_PAUSE_MASK                      0x00000008
#define SGMII_CONFIG_ENABLE_SGMII_TX_PAUSE_GET(x)                    (((x) & SGMII_CONFIG_ENABLE_SGMII_TX_PAUSE_MASK) >> SGMII_CONFIG_ENABLE_SGMII_TX_PAUSE_LSB)
#define SGMII_CONFIG_ENABLE_SGMII_TX_PAUSE_SET(x)                    (((x) << SGMII_CONFIG_ENABLE_SGMII_TX_PAUSE_LSB) & SGMII_CONFIG_ENABLE_SGMII_TX_PAUSE_MASK)
#define SGMII_CONFIG_ENABLE_SGMII_TX_PAUSE_RESET                     0x0 // 0
#define SGMII_CONFIG_MODE_CTRL_MSB                                   2
#define SGMII_CONFIG_MODE_CTRL_LSB                                   0
#define SGMII_CONFIG_MODE_CTRL_MASK                                  0x00000007
#define SGMII_CONFIG_MODE_CTRL_GET(x)                                (((x) & SGMII_CONFIG_MODE_CTRL_MASK) >> SGMII_CONFIG_MODE_CTRL_LSB)
#define SGMII_CONFIG_MODE_CTRL_SET(x)                                (((x) << SGMII_CONFIG_MODE_CTRL_LSB) & SGMII_CONFIG_MODE_CTRL_MASK)
#define SGMII_CONFIG_MODE_CTRL_RESET                                 0x0 // 0
#define SGMII_CONFIG_ADDRESS                                         0x18070034



// 32'h1807001c (MR_AN_CONTROL)
#define MR_AN_CONTROL_PHY_RESET_MSB                                  15
#define MR_AN_CONTROL_PHY_RESET_LSB                                  15
#define MR_AN_CONTROL_PHY_RESET_MASK                                 0x00008000
#define MR_AN_CONTROL_PHY_RESET_GET(x)                               (((x) & MR_AN_CONTROL_PHY_RESET_MASK) >> MR_AN_CONTROL_PHY_RESET_LSB)
#define MR_AN_CONTROL_PHY_RESET_SET(x)                               (((x) << MR_AN_CONTROL_PHY_RESET_LSB) & MR_AN_CONTROL_PHY_RESET_MASK)
#define MR_AN_CONTROL_PHY_RESET_RESET                                0x0 // 0
#define MR_AN_CONTROL_LOOPBACK_MSB                                   14
#define MR_AN_CONTROL_LOOPBACK_LSB                                   14
#define MR_AN_CONTROL_LOOPBACK_MASK                                  0x00004000
#define MR_AN_CONTROL_LOOPBACK_GET(x)                                (((x) & MR_AN_CONTROL_LOOPBACK_MASK) >> MR_AN_CONTROL_LOOPBACK_LSB)
#define MR_AN_CONTROL_LOOPBACK_SET(x)                                (((x) << MR_AN_CONTROL_LOOPBACK_LSB) & MR_AN_CONTROL_LOOPBACK_MASK)
#define MR_AN_CONTROL_LOOPBACK_RESET                                 0x0 // 0
#define MR_AN_CONTROL_SPEED_SEL0_MSB                                 13
#define MR_AN_CONTROL_SPEED_SEL0_LSB                                 13
#define MR_AN_CONTROL_SPEED_SEL0_MASK                                0x00002000
#define MR_AN_CONTROL_SPEED_SEL0_GET(x)                              (((x) & MR_AN_CONTROL_SPEED_SEL0_MASK) >> MR_AN_CONTROL_SPEED_SEL0_LSB)
#define MR_AN_CONTROL_SPEED_SEL0_SET(x)                              (((x) << MR_AN_CONTROL_SPEED_SEL0_LSB) & MR_AN_CONTROL_SPEED_SEL0_MASK)
#define MR_AN_CONTROL_SPEED_SEL0_RESET                               0x0 // 0
#define MR_AN_CONTROL_AN_ENABLE_MSB                                  12
#define MR_AN_CONTROL_AN_ENABLE_LSB                                  12
#define MR_AN_CONTROL_AN_ENABLE_MASK                                 0x00001000
#define MR_AN_CONTROL_AN_ENABLE_GET(x)                               (((x) & MR_AN_CONTROL_AN_ENABLE_MASK) >> MR_AN_CONTROL_AN_ENABLE_LSB)
#define MR_AN_CONTROL_AN_ENABLE_SET(x)                               (((x) << MR_AN_CONTROL_AN_ENABLE_LSB) & MR_AN_CONTROL_AN_ENABLE_MASK)
#define MR_AN_CONTROL_AN_ENABLE_RESET                                0x1 // 1
#define MR_AN_CONTROL_POWER_DOWN_MSB                                 11
#define MR_AN_CONTROL_POWER_DOWN_LSB                                 11
#define MR_AN_CONTROL_POWER_DOWN_MASK                                0x00000800
#define MR_AN_CONTROL_POWER_DOWN_GET(x)                              (((x) & MR_AN_CONTROL_POWER_DOWN_MASK) >> MR_AN_CONTROL_POWER_DOWN_LSB)
#define MR_AN_CONTROL_POWER_DOWN_SET(x)                              (((x) << MR_AN_CONTROL_POWER_DOWN_LSB) & MR_AN_CONTROL_POWER_DOWN_MASK)
#define MR_AN_CONTROL_POWER_DOWN_RESET                               0x0 // 0
#define MR_AN_CONTROL_RESTART_AN_MSB                                 9
#define MR_AN_CONTROL_RESTART_AN_LSB                                 9
#define MR_AN_CONTROL_RESTART_AN_MASK                                0x00000200
#define MR_AN_CONTROL_RESTART_AN_GET(x)                              (((x) & MR_AN_CONTROL_RESTART_AN_MASK) >> MR_AN_CONTROL_RESTART_AN_LSB)
#define MR_AN_CONTROL_RESTART_AN_SET(x)                              (((x) << MR_AN_CONTROL_RESTART_AN_LSB) & MR_AN_CONTROL_RESTART_AN_MASK)
#define MR_AN_CONTROL_RESTART_AN_RESET                               0x0 // 0
#define MR_AN_CONTROL_DUPLEX_MODE_MSB                                8
#define MR_AN_CONTROL_DUPLEX_MODE_LSB                                8
#define MR_AN_CONTROL_DUPLEX_MODE_MASK                               0x00000100
#define MR_AN_CONTROL_DUPLEX_MODE_GET(x)                             (((x) & MR_AN_CONTROL_DUPLEX_MODE_MASK) >> MR_AN_CONTROL_DUPLEX_MODE_LSB)
#define MR_AN_CONTROL_DUPLEX_MODE_SET(x)                             (((x) << MR_AN_CONTROL_DUPLEX_MODE_LSB) & MR_AN_CONTROL_DUPLEX_MODE_MASK)
#define MR_AN_CONTROL_DUPLEX_MODE_RESET                              0x1 // 1
#define MR_AN_CONTROL_SPEED_SEL1_MSB                                 6
#define MR_AN_CONTROL_SPEED_SEL1_LSB                                 6
#define MR_AN_CONTROL_SPEED_SEL1_MASK                                0x00000040
#define MR_AN_CONTROL_SPEED_SEL1_GET(x)                              (((x) & MR_AN_CONTROL_SPEED_SEL1_MASK) >> MR_AN_CONTROL_SPEED_SEL1_LSB)
#define MR_AN_CONTROL_SPEED_SEL1_SET(x)                              (((x) << MR_AN_CONTROL_SPEED_SEL1_LSB) & MR_AN_CONTROL_SPEED_SEL1_MASK)
#define MR_AN_CONTROL_SPEED_SEL1_RESET                               0x1 // 1
#define MR_AN_CONTROL_ADDRESS                                        0x1807001c





// 32'h18070014 (SGMII_RESET)
#define SGMII_RESET_HW_RX_125M_N_MSB                                 4
#define SGMII_RESET_HW_RX_125M_N_LSB                                 4
#define SGMII_RESET_HW_RX_125M_N_MASK                                0x00000010
#define SGMII_RESET_HW_RX_125M_N_GET(x)                              (((x) & SGMII_RESET_HW_RX_125M_N_MASK) >> SGMII_RESET_HW_RX_125M_N_LSB)
#define SGMII_RESET_HW_RX_125M_N_SET(x)                              (((x) << SGMII_RESET_HW_RX_125M_N_LSB) & SGMII_RESET_HW_RX_125M_N_MASK)
#define SGMII_RESET_HW_RX_125M_N_RESET                               0x0 // 0
#define SGMII_RESET_TX_125M_N_MSB                                    3
#define SGMII_RESET_TX_125M_N_LSB                                    3
#define SGMII_RESET_TX_125M_N_MASK                                   0x00000008
#define SGMII_RESET_TX_125M_N_GET(x)                                 (((x) & SGMII_RESET_TX_125M_N_MASK) >> SGMII_RESET_TX_125M_N_LSB)
#define SGMII_RESET_TX_125M_N_SET(x)                                 (((x) << SGMII_RESET_TX_125M_N_LSB) & SGMII_RESET_TX_125M_N_MASK)
#define SGMII_RESET_TX_125M_N_RESET                                  0x0 // 0
#define SGMII_RESET_RX_125M_N_MSB                                    2
#define SGMII_RESET_RX_125M_N_LSB                                    2
#define SGMII_RESET_RX_125M_N_MASK                                   0x00000004
#define SGMII_RESET_RX_125M_N_GET(x)                                 (((x) & SGMII_RESET_RX_125M_N_MASK) >> SGMII_RESET_RX_125M_N_LSB)
#define SGMII_RESET_RX_125M_N_SET(x)                                 (((x) << SGMII_RESET_RX_125M_N_LSB) & SGMII_RESET_RX_125M_N_MASK)
#define SGMII_RESET_RX_125M_N_RESET                                  0x0 // 0
#define SGMII_RESET_TX_CLK_N_MSB                                     1
#define SGMII_RESET_TX_CLK_N_LSB                                     1
#define SGMII_RESET_TX_CLK_N_MASK                                    0x00000002
#define SGMII_RESET_TX_CLK_N_GET(x)                                  (((x) & SGMII_RESET_TX_CLK_N_MASK) >> SGMII_RESET_TX_CLK_N_LSB)
#define SGMII_RESET_TX_CLK_N_SET(x)                                  (((x) << SGMII_RESET_TX_CLK_N_LSB) & SGMII_RESET_TX_CLK_N_MASK)
#define SGMII_RESET_TX_CLK_N_RESET                                   0x0 // 0
#define SGMII_RESET_RX_CLK_N_MSB                                     0
#define SGMII_RESET_RX_CLK_N_LSB                                     0
#define SGMII_RESET_RX_CLK_N_MASK                                    0x00000001
#define SGMII_RESET_RX_CLK_N_GET(x)                                  (((x) & SGMII_RESET_RX_CLK_N_MASK) >> SGMII_RESET_RX_CLK_N_LSB)
#define SGMII_RESET_RX_CLK_N_SET(x)                                  (((x) << SGMII_RESET_RX_CLK_N_LSB) & SGMII_RESET_RX_CLK_N_MASK)
#define SGMII_RESET_RX_CLK_N_RESET                                   0x0 // 0
#define SGMII_RESET_ADDRESS                                          0x18070014



// 32'h18070038 (SGMII_MAC_RX_CONFIG)
#define SGMII_MAC_RX_CONFIG_LINK_MSB                                 15
#define SGMII_MAC_RX_CONFIG_LINK_LSB                                 15
#define SGMII_MAC_RX_CONFIG_LINK_MASK                                0x00008000
#define SGMII_MAC_RX_CONFIG_LINK_GET(x)                              (((x) & SGMII_MAC_RX_CONFIG_LINK_MASK) >> SGMII_MAC_RX_CONFIG_LINK_LSB)
#define SGMII_MAC_RX_CONFIG_LINK_SET(x)                              (((x) << SGMII_MAC_RX_CONFIG_LINK_LSB) & SGMII_MAC_RX_CONFIG_LINK_MASK)
#define SGMII_MAC_RX_CONFIG_LINK_RESET                               0x0 // 0
#define SGMII_MAC_RX_CONFIG_ACK_MSB                                  14
#define SGMII_MAC_RX_CONFIG_ACK_LSB                                  14
#define SGMII_MAC_RX_CONFIG_ACK_MASK                                 0x00004000
#define SGMII_MAC_RX_CONFIG_ACK_GET(x)                               (((x) & SGMII_MAC_RX_CONFIG_ACK_MASK) >> SGMII_MAC_RX_CONFIG_ACK_LSB)
#define SGMII_MAC_RX_CONFIG_ACK_SET(x)                               (((x) << SGMII_MAC_RX_CONFIG_ACK_LSB) & SGMII_MAC_RX_CONFIG_ACK_MASK)
#define SGMII_MAC_RX_CONFIG_ACK_RESET                                0x0 // 0
#define SGMII_MAC_RX_CONFIG_DUPLEX_MODE_MSB                          12
#define SGMII_MAC_RX_CONFIG_DUPLEX_MODE_LSB                          12
#define SGMII_MAC_RX_CONFIG_DUPLEX_MODE_MASK                         0x00001000
#define SGMII_MAC_RX_CONFIG_DUPLEX_MODE_GET(x)                       (((x) & SGMII_MAC_RX_CONFIG_DUPLEX_MODE_MASK) >> SGMII_MAC_RX_CONFIG_DUPLEX_MODE_LSB)
#define SGMII_MAC_RX_CONFIG_DUPLEX_MODE_SET(x)                       (((x) << SGMII_MAC_RX_CONFIG_DUPLEX_MODE_LSB) & SGMII_MAC_RX_CONFIG_DUPLEX_MODE_MASK)
#define SGMII_MAC_RX_CONFIG_DUPLEX_MODE_RESET                        0x0 // 0
#define SGMII_MAC_RX_CONFIG_SPEED_MODE_MSB                           11
#define SGMII_MAC_RX_CONFIG_SPEED_MODE_LSB                           10
#define SGMII_MAC_RX_CONFIG_SPEED_MODE_MASK                          0x00000c00
#define SGMII_MAC_RX_CONFIG_SPEED_MODE_GET(x)                        (((x) & SGMII_MAC_RX_CONFIG_SPEED_MODE_MASK) >> SGMII_MAC_RX_CONFIG_SPEED_MODE_LSB)
#define SGMII_MAC_RX_CONFIG_SPEED_MODE_SET(x)                        (((x) << SGMII_MAC_RX_CONFIG_SPEED_MODE_LSB) & SGMII_MAC_RX_CONFIG_SPEED_MODE_MASK)
#define SGMII_MAC_RX_CONFIG_SPEED_MODE_RESET                         0x0 // 0
#define SGMII_MAC_RX_CONFIG_ASM_PAUSE_MSB                            8
#define SGMII_MAC_RX_CONFIG_ASM_PAUSE_LSB                            8
#define SGMII_MAC_RX_CONFIG_ASM_PAUSE_MASK                           0x00000100
#define SGMII_MAC_RX_CONFIG_ASM_PAUSE_GET(x)                         (((x) & SGMII_MAC_RX_CONFIG_ASM_PAUSE_MASK) >> SGMII_MAC_RX_CONFIG_ASM_PAUSE_LSB)
#define SGMII_MAC_RX_CONFIG_ASM_PAUSE_SET(x)                         (((x) << SGMII_MAC_RX_CONFIG_ASM_PAUSE_LSB) & SGMII_MAC_RX_CONFIG_ASM_PAUSE_MASK)
#define SGMII_MAC_RX_CONFIG_ASM_PAUSE_RESET                          0x0 // 0
#define SGMII_MAC_RX_CONFIG_PAUSE_MSB                                7
#define SGMII_MAC_RX_CONFIG_PAUSE_LSB                                7
#define SGMII_MAC_RX_CONFIG_PAUSE_MASK                               0x00000080
#define SGMII_MAC_RX_CONFIG_PAUSE_GET(x)                             (((x) & SGMII_MAC_RX_CONFIG_PAUSE_MASK) >> SGMII_MAC_RX_CONFIG_PAUSE_LSB)
#define SGMII_MAC_RX_CONFIG_PAUSE_SET(x)                             (((x) << SGMII_MAC_RX_CONFIG_PAUSE_LSB) & SGMII_MAC_RX_CONFIG_PAUSE_MASK)
#define SGMII_MAC_RX_CONFIG_PAUSE_RESET                              0x0 // 0
#define SGMII_MAC_RX_CONFIG_RES0_MSB                                 0
#define SGMII_MAC_RX_CONFIG_RES0_LSB                                 0
#define SGMII_MAC_RX_CONFIG_RES0_MASK                                0x00000001
#define SGMII_MAC_RX_CONFIG_RES0_GET(x)                              (((x) & SGMII_MAC_RX_CONFIG_RES0_MASK) >> SGMII_MAC_RX_CONFIG_RES0_LSB)
#define SGMII_MAC_RX_CONFIG_RES0_SET(x)                              (((x) << SGMII_MAC_RX_CONFIG_RES0_LSB) & SGMII_MAC_RX_CONFIG_RES0_MASK)
#define SGMII_MAC_RX_CONFIG_RES0_RESET                               0x1 // 1
#define SGMII_MAC_RX_CONFIG_ADDRESS                                  0x18070038


// 32'h18070058 (SGMII_DEBUG)
#define SGMII_DEBUG_ARB_STATE_MSB                                    27
#define SGMII_DEBUG_ARB_STATE_LSB                                    24
#define SGMII_DEBUG_ARB_STATE_MASK                                   0x0f000000
#define SGMII_DEBUG_ARB_STATE_GET(x)                                 (((x) & SGMII_DEBUG_ARB_STATE_MASK) >> SGMII_DEBUG_ARB_STATE_LSB)
#define SGMII_DEBUG_ARB_STATE_SET(x)                                 (((x) << SGMII_DEBUG_ARB_STATE_LSB) & SGMII_DEBUG_ARB_STATE_MASK)
#define SGMII_DEBUG_ARB_STATE_RESET                                  0x0 // 0
#define SGMII_DEBUG_RX_SYNC_STATE_MSB                                23
#define SGMII_DEBUG_RX_SYNC_STATE_LSB                                16
#define SGMII_DEBUG_RX_SYNC_STATE_MASK                               0x00ff0000
#define SGMII_DEBUG_RX_SYNC_STATE_GET(x)                             (((x) & SGMII_DEBUG_RX_SYNC_STATE_MASK) >> SGMII_DEBUG_RX_SYNC_STATE_LSB)
#define SGMII_DEBUG_RX_SYNC_STATE_SET(x)                             (((x) << SGMII_DEBUG_RX_SYNC_STATE_LSB) & SGMII_DEBUG_RX_SYNC_STATE_MASK)
#define SGMII_DEBUG_RX_SYNC_STATE_RESET                              0x0 // 0
#define SGMII_DEBUG_RX_STATE_MSB                                     15
#define SGMII_DEBUG_RX_STATE_LSB                                     8
#define SGMII_DEBUG_RX_STATE_MASK                                    0x0000ff00
#define SGMII_DEBUG_RX_STATE_GET(x)                                  (((x) & SGMII_DEBUG_RX_STATE_MASK) >> SGMII_DEBUG_RX_STATE_LSB)
#define SGMII_DEBUG_RX_STATE_SET(x)                                  (((x) << SGMII_DEBUG_RX_STATE_LSB) & SGMII_DEBUG_RX_STATE_MASK)
#define SGMII_DEBUG_RX_STATE_RESET                                   0x0 // 0
#define SGMII_DEBUG_TX_STATE_MSB                                     7
#define SGMII_DEBUG_TX_STATE_LSB                                     0
#define SGMII_DEBUG_TX_STATE_MASK                                    0x000000ff
#define SGMII_DEBUG_TX_STATE_GET(x)                                  (((x) & SGMII_DEBUG_TX_STATE_MASK) >> SGMII_DEBUG_TX_STATE_LSB)
#define SGMII_DEBUG_TX_STATE_SET(x)                                  (((x) << SGMII_DEBUG_TX_STATE_LSB) & SGMII_DEBUG_TX_STATE_MASK)
#define SGMII_DEBUG_TX_STATE_RESET                                   0x0 // 0
#define SGMII_DEBUG_ADDRESS                                          0x18070058
#define SGMII_DEBUG_OFFSET                                           0x0058




// 32'h18070060 (SGMII_INTERRUPT_MASK)
#define SGMII_INTERRUPT_MASK_MASK_MSB                                7
#define SGMII_INTERRUPT_MASK_MASK_LSB                                0
#define SGMII_INTERRUPT_MASK_MASK_MASK                               0x000000ff
#define SGMII_INTERRUPT_MASK_MASK_GET(x)                             (((x) & SGMII_INTERRUPT_MASK_MASK_MASK) >> SGMII_INTERRUPT_MASK_MASK_LSB)
#define SGMII_INTERRUPT_MASK_MASK_SET(x)                             (((x) << SGMII_INTERRUPT_MASK_MASK_LSB) & SGMII_INTERRUPT_MASK_MASK_MASK)
#define SGMII_INTERRUPT_MASK_MASK_RESET                              0x0 // 0
#define SGMII_INTERRUPT_MASK_ADDRESS                                 0x18070060




// 32'h1807005c (SGMII_INTERRUPT)
#define SGMII_INTERRUPT_INTR_MSB                                     7
#define SGMII_INTERRUPT_INTR_LSB                                     0
#define SGMII_INTERRUPT_INTR_MASK                                    0x000000ff
#define SGMII_INTERRUPT_INTR_GET(x)                                  (((x) & SGMII_INTERRUPT_INTR_MASK) >> SGMII_INTERRUPT_INTR_LSB)
#define SGMII_INTERRUPT_INTR_SET(x)                                  (((x) << SGMII_INTERRUPT_INTR_LSB) & SGMII_INTERRUPT_INTR_MASK)
#define SGMII_INTERRUPT_INTR_RESET                                   0x0 // 0
#define SGMII_INTERRUPT_ADDRESS                                      0x1807005c
#define SGMII_INTERRUPT_OFFSET                                       0x005c
// SW modifiable bits
#define SGMII_INTERRUPT_SW_MASK                                      0x000000ff
// bits defined at reset
#define SGMII_INTERRUPT_RSTMASK                                      0xffffffff
// reset value (ignore bits undefined at reset)
#define SGMII_INTERRUPT_RESET                                        0x00000000

// 32'h18070060 (SGMII_INTERRUPT_MASK)
#define SGMII_INTERRUPT_MASK_MASK_MSB                                7
#define SGMII_INTERRUPT_MASK_MASK_LSB                                0
#define SGMII_INTERRUPT_MASK_MASK_MASK                               0x000000ff
#define SGMII_INTERRUPT_MASK_MASK_GET(x)                             (((x) & SGMII_INTERRUPT_MASK_MASK_MASK) >> SGMII_INTERRUPT_MASK_MASK_LSB)
#define SGMII_INTERRUPT_MASK_MASK_SET(x)                             (((x) << SGMII_INTERRUPT_MASK_MASK_LSB) & SGMII_INTERRUPT_MASK_MASK_MASK)
#define SGMII_INTERRUPT_MASK_MASK_RESET                              0x0 // 0
#define SGMII_INTERRUPT_MASK_ADDRESS                                 0x18070060


#define SGMII_LINK_FAIL                 (1 << 0)
#define SGMII_DUPLEX_ERR                (1 << 1)
#define SGMII_MR_AN_COMPLETE            (1 << 2)
#define SGMII_LINK_MAC_CHANGE           (1 << 3)
#define SGMII_DUPLEX_MODE_CHANGE        (1 << 4)
#define SGMII_SPEED_MODE_MAC_CHANGE     (1 << 5)
#define SGMII_RX_QUIET_CHANGE           (1 << 6)
#define SGMII_RX_MDIO_COMP_CHANGE       (1 << 7)

#define SGMII_INTR			SGMII_LINK_MAC_CHANGE


// 32'h18050048 (ETH_SGMII)
#define ETH_SGMII_TX_INVERT_MSB                                      31
#define ETH_SGMII_TX_INVERT_LSB                                      31
#define ETH_SGMII_TX_INVERT_MASK                                     0x80000000
#define ETH_SGMII_TX_INVERT_GET(x)                                   (((x) & ETH_SGMII_TX_INVERT_MASK) >> ETH_SGMII_TX_INVERT_LSB)
#define ETH_SGMII_TX_INVERT_SET(x)                                   (((x) << ETH_SGMII_TX_INVERT_LSB) & ETH_SGMII_TX_INVERT_MASK)
#define ETH_SGMII_TX_INVERT_RESET                                    0x0 // 0
#define ETH_SGMII_GIGE_QUAD_MSB                                      30
#define ETH_SGMII_GIGE_QUAD_LSB                                      30
#define ETH_SGMII_GIGE_QUAD_MASK                                     0x40000000
#define ETH_SGMII_GIGE_QUAD_GET(x)                                   (((x) & ETH_SGMII_GIGE_QUAD_MASK) >> ETH_SGMII_GIGE_QUAD_LSB)
#define ETH_SGMII_GIGE_QUAD_SET(x)                                   (((x) << ETH_SGMII_GIGE_QUAD_LSB) & ETH_SGMII_GIGE_QUAD_MASK)
#define ETH_SGMII_GIGE_QUAD_RESET                                    0x0 // 0
#define ETH_SGMII_RX_DELAY_MSB                                       29
#define ETH_SGMII_RX_DELAY_LSB                                       28
#define ETH_SGMII_RX_DELAY_MASK                                      0x30000000
#define ETH_SGMII_RX_DELAY_GET(x)                                    (((x) & ETH_SGMII_RX_DELAY_MASK) >> ETH_SGMII_RX_DELAY_LSB)
#define ETH_SGMII_RX_DELAY_SET(x)                                    (((x) << ETH_SGMII_RX_DELAY_LSB) & ETH_SGMII_RX_DELAY_MASK)
#define ETH_SGMII_RX_DELAY_RESET                                     0x0 // 0
#define ETH_SGMII_TX_DELAY_MSB                                       27
#define ETH_SGMII_TX_DELAY_LSB                                       26
#define ETH_SGMII_TX_DELAY_MASK                                      0x0c000000
#define ETH_SGMII_TX_DELAY_GET(x)                                    (((x) & ETH_SGMII_TX_DELAY_MASK) >> ETH_SGMII_TX_DELAY_LSB)
#define ETH_SGMII_TX_DELAY_SET(x)                                    (((x) << ETH_SGMII_TX_DELAY_LSB) & ETH_SGMII_TX_DELAY_MASK)
#define ETH_SGMII_TX_DELAY_RESET                                     0x0 // 0
#define ETH_SGMII_CLK_SEL_MSB                                        25
#define ETH_SGMII_CLK_SEL_LSB                                        25
#define ETH_SGMII_CLK_SEL_MASK                                       0x02000000
#define ETH_SGMII_CLK_SEL_GET(x)                                     (((x) & ETH_SGMII_CLK_SEL_MASK) >> ETH_SGMII_CLK_SEL_LSB)
#define ETH_SGMII_CLK_SEL_SET(x)                                     (((x) << ETH_SGMII_CLK_SEL_LSB) & ETH_SGMII_CLK_SEL_MASK)
#define ETH_SGMII_CLK_SEL_RESET                                      0x1 // 1
#define ETH_SGMII_GIGE_MSB                                           24
#define ETH_SGMII_GIGE_LSB                                           24
#define ETH_SGMII_GIGE_MASK                                          0x01000000
#define ETH_SGMII_GIGE_GET(x)                                        (((x) & ETH_SGMII_GIGE_MASK) >> ETH_SGMII_GIGE_LSB)
#define ETH_SGMII_GIGE_SET(x)                                        (((x) << ETH_SGMII_GIGE_LSB) & ETH_SGMII_GIGE_MASK)
#define ETH_SGMII_GIGE_RESET                                         0x1 // 1
#define ETH_SGMII_PHASE1_COUNT_MSB                                   15
#define ETH_SGMII_PHASE1_COUNT_LSB                                   8
#define ETH_SGMII_PHASE1_COUNT_MASK                                  0x0000ff00
#define ETH_SGMII_PHASE1_COUNT_GET(x)                                (((x) & ETH_SGMII_PHASE1_COUNT_MASK) >> ETH_SGMII_PHASE1_COUNT_LSB)
#define ETH_SGMII_PHASE1_COUNT_SET(x)                                (((x) << ETH_SGMII_PHASE1_COUNT_LSB) & ETH_SGMII_PHASE1_COUNT_MASK)
#define ETH_SGMII_PHASE1_COUNT_RESET                                 0x1 // 1
#define ETH_SGMII_PHASE0_COUNT_MSB                                   7
#define ETH_SGMII_PHASE0_COUNT_LSB                                   0
#define ETH_SGMII_PHASE0_COUNT_MASK                                  0x000000ff
#define ETH_SGMII_PHASE0_COUNT_GET(x)                                (((x) & ETH_SGMII_PHASE0_COUNT_MASK) >> ETH_SGMII_PHASE0_COUNT_LSB)
#define ETH_SGMII_PHASE0_COUNT_SET(x)                                (((x) << ETH_SGMII_PHASE0_COUNT_LSB) & ETH_SGMII_PHASE0_COUNT_MASK)
#define ETH_SGMII_PHASE0_COUNT_RESET                                 0x1 // 1
#define ETH_SGMII_ADDRESS                                            0x18050048
#define ETH_SGMII_OFFSET                                             0x0048

// 32'h18070018 (SGMII_SERDES)
#define SGMII_SERDES_VCO_REG_MSB                                     30
#define SGMII_SERDES_VCO_REG_LSB                                     27
#define SGMII_SERDES_VCO_REG_MASK                                    0x78000000
#define SGMII_SERDES_VCO_REG_GET(x)                                  (((x) & SGMII_SERDES_VCO_REG_MASK) >> SGMII_SERDES_VCO_REG_LSB)
#define SGMII_SERDES_VCO_REG_SET(x)                                  (((x) << SGMII_SERDES_VCO_REG_LSB) & SGMII_SERDES_VCO_REG_MASK)
#define SGMII_SERDES_VCO_REG_RESET                                   0x3 // 3
#define SGMII_SERDES_RES_CALIBRATION_MSB                             26
#define SGMII_SERDES_RES_CALIBRATION_LSB                             23
#define SGMII_SERDES_RES_CALIBRATION_MASK                            0x07800000
#define SGMII_SERDES_RES_CALIBRATION_GET(x)                          (((x) & SGMII_SERDES_RES_CALIBRATION_MASK) >> SGMII_SERDES_RES_CALIBRATION_LSB)
#define SGMII_SERDES_RES_CALIBRATION_SET(x)                          (((x) << SGMII_SERDES_RES_CALIBRATION_LSB) & SGMII_SERDES_RES_CALIBRATION_MASK)
#define SGMII_SERDES_RES_CALIBRATION_RESET                           0x0 // 0
#define SGMII_SERDES_FIBER_MODE_MSB                                  21
#define SGMII_SERDES_FIBER_MODE_LSB                                  20
#define SGMII_SERDES_FIBER_MODE_MASK                                 0x00300000
#define SGMII_SERDES_FIBER_MODE_GET(x)                               (((x) & SGMII_SERDES_FIBER_MODE_MASK) >> SGMII_SERDES_FIBER_MODE_LSB)
#define SGMII_SERDES_FIBER_MODE_SET(x)                               (((x) << SGMII_SERDES_FIBER_MODE_LSB) & SGMII_SERDES_FIBER_MODE_MASK)
#define SGMII_SERDES_FIBER_MODE_RESET                                0x0 // 0
#define SGMII_SERDES_THRESHOLD_CTRL_MSB                              19
#define SGMII_SERDES_THRESHOLD_CTRL_LSB                              18
#define SGMII_SERDES_THRESHOLD_CTRL_MASK                             0x000c0000
#define SGMII_SERDES_THRESHOLD_CTRL_GET(x)                           (((x) & SGMII_SERDES_THRESHOLD_CTRL_MASK) >> SGMII_SERDES_THRESHOLD_CTRL_LSB)
#define SGMII_SERDES_THRESHOLD_CTRL_SET(x)                           (((x) << SGMII_SERDES_THRESHOLD_CTRL_LSB) & SGMII_SERDES_THRESHOLD_CTRL_MASK)
#define SGMII_SERDES_THRESHOLD_CTRL_RESET                            0x0 // 0
#define SGMII_SERDES_FIBER_SDO_MSB                                   17
#define SGMII_SERDES_FIBER_SDO_LSB                                   17
#define SGMII_SERDES_FIBER_SDO_MASK                                  0x00020000
#define SGMII_SERDES_FIBER_SDO_GET(x)                                (((x) & SGMII_SERDES_FIBER_SDO_MASK) >> SGMII_SERDES_FIBER_SDO_LSB)
#define SGMII_SERDES_FIBER_SDO_SET(x)                                (((x) << SGMII_SERDES_FIBER_SDO_LSB) & SGMII_SERDES_FIBER_SDO_MASK)
#define SGMII_SERDES_FIBER_SDO_RESET                                 0x0 // 0
#define SGMII_SERDES_EN_SIGNAL_DETECT_MSB                            16
#define SGMII_SERDES_EN_SIGNAL_DETECT_LSB                            16
#define SGMII_SERDES_EN_SIGNAL_DETECT_MASK                           0x00010000
#define SGMII_SERDES_EN_SIGNAL_DETECT_GET(x)                         (((x) & SGMII_SERDES_EN_SIGNAL_DETECT_MASK) >> SGMII_SERDES_EN_SIGNAL_DETECT_LSB)
#define SGMII_SERDES_EN_SIGNAL_DETECT_SET(x)                         (((x) << SGMII_SERDES_EN_SIGNAL_DETECT_LSB) & SGMII_SERDES_EN_SIGNAL_DETECT_MASK)
#define SGMII_SERDES_EN_SIGNAL_DETECT_RESET                          0x1 // 1
#define SGMII_SERDES_LOCK_DETECT_STATUS_MSB                          15
#define SGMII_SERDES_LOCK_DETECT_STATUS_LSB                          15
#define SGMII_SERDES_LOCK_DETECT_STATUS_MASK                         0x00008000
#define SGMII_SERDES_LOCK_DETECT_STATUS_GET(x)                       (((x) & SGMII_SERDES_LOCK_DETECT_STATUS_MASK) >> SGMII_SERDES_LOCK_DETECT_STATUS_LSB)
#define SGMII_SERDES_LOCK_DETECT_STATUS_SET(x)                       (((x) << SGMII_SERDES_LOCK_DETECT_STATUS_LSB) & SGMII_SERDES_LOCK_DETECT_STATUS_MASK)
#define SGMII_SERDES_LOCK_DETECT_STATUS_RESET                        0x0 // 0
#define SGMII_SERDES_SPARE0_MSB                                      14
#define SGMII_SERDES_SPARE0_LSB                                      11
#define SGMII_SERDES_SPARE0_MASK                                     0x00007800
#define SGMII_SERDES_SPARE0_GET(x)                                   (((x) & SGMII_SERDES_SPARE0_MASK) >> SGMII_SERDES_SPARE0_LSB)
#define SGMII_SERDES_SPARE0_SET(x)                                   (((x) << SGMII_SERDES_SPARE0_LSB) & SGMII_SERDES_SPARE0_MASK)
#define SGMII_SERDES_SPARE0_RESET                                    0x0 // 0
#define SGMII_SERDES_VCO_SLOW_MSB                                    10
#define SGMII_SERDES_VCO_SLOW_LSB                                    10
#define SGMII_SERDES_VCO_SLOW_MASK                                   0x00000400
#define SGMII_SERDES_VCO_SLOW_GET(x)                                 (((x) & SGMII_SERDES_VCO_SLOW_MASK) >> SGMII_SERDES_VCO_SLOW_LSB)
#define SGMII_SERDES_VCO_SLOW_SET(x)                                 (((x) << SGMII_SERDES_VCO_SLOW_LSB) & SGMII_SERDES_VCO_SLOW_MASK)
#define SGMII_SERDES_VCO_SLOW_RESET                                  0x0 // 0
#define SGMII_SERDES_VCO_FAST_MSB                                    9
#define SGMII_SERDES_VCO_FAST_LSB                                    9
#define SGMII_SERDES_VCO_FAST_MASK                                   0x00000200
#define SGMII_SERDES_VCO_FAST_GET(x)                                 (((x) & SGMII_SERDES_VCO_FAST_MASK) >> SGMII_SERDES_VCO_FAST_LSB)
#define SGMII_SERDES_VCO_FAST_SET(x)                                 (((x) << SGMII_SERDES_VCO_FAST_LSB) & SGMII_SERDES_VCO_FAST_MASK)
#define SGMII_SERDES_VCO_FAST_RESET                                  0x0 // 0
#define SGMII_SERDES_PLL_BW_MSB                                      8
#define SGMII_SERDES_PLL_BW_LSB                                      8
#define SGMII_SERDES_PLL_BW_MASK                                     0x00000100
#define SGMII_SERDES_PLL_BW_GET(x)                                   (((x) & SGMII_SERDES_PLL_BW_MASK) >> SGMII_SERDES_PLL_BW_LSB)
#define SGMII_SERDES_PLL_BW_SET(x)                                   (((x) << SGMII_SERDES_PLL_BW_LSB) & SGMII_SERDES_PLL_BW_MASK)
#define SGMII_SERDES_PLL_BW_RESET                                    0x1 // 1
#define SGMII_SERDES_TX_IMPEDANCE_MSB                                7
#define SGMII_SERDES_TX_IMPEDANCE_LSB                                7
#define SGMII_SERDES_TX_IMPEDANCE_MASK                               0x00000080
#define SGMII_SERDES_TX_IMPEDANCE_GET(x)                             (((x) & SGMII_SERDES_TX_IMPEDANCE_MASK) >> SGMII_SERDES_TX_IMPEDANCE_LSB)
#define SGMII_SERDES_TX_IMPEDANCE_SET(x)                             (((x) << SGMII_SERDES_TX_IMPEDANCE_LSB) & SGMII_SERDES_TX_IMPEDANCE_MASK)
#define SGMII_SERDES_TX_IMPEDANCE_RESET                              0x0 // 0
#define SGMII_SERDES_TX_DR_CTRL_MSB                                  6
#define SGMII_SERDES_TX_DR_CTRL_LSB                                  4
#define SGMII_SERDES_TX_DR_CTRL_MASK                                 0x00000070
#define SGMII_SERDES_TX_DR_CTRL_GET(x)                               (((x) & SGMII_SERDES_TX_DR_CTRL_MASK) >> SGMII_SERDES_TX_DR_CTRL_LSB)
#define SGMII_SERDES_TX_DR_CTRL_SET(x)                               (((x) << SGMII_SERDES_TX_DR_CTRL_LSB) & SGMII_SERDES_TX_DR_CTRL_MASK)
#define SGMII_SERDES_TX_DR_CTRL_RESET                                0x1 // 1
#define SGMII_SERDES_HALF_TX_MSB                                     3
#define SGMII_SERDES_HALF_TX_LSB                                     3
#define SGMII_SERDES_HALF_TX_MASK                                    0x00000008
#define SGMII_SERDES_HALF_TX_GET(x)                                  (((x) & SGMII_SERDES_HALF_TX_MASK) >> SGMII_SERDES_HALF_TX_LSB)
#define SGMII_SERDES_HALF_TX_SET(x)                                  (((x) << SGMII_SERDES_HALF_TX_LSB) & SGMII_SERDES_HALF_TX_MASK)
#define SGMII_SERDES_HALF_TX_RESET                                   0x0 // 0
#define SGMII_SERDES_CDR_BW_MSB                                      2
#define SGMII_SERDES_CDR_BW_LSB                                      1
#define SGMII_SERDES_CDR_BW_MASK                                     0x00000006
#define SGMII_SERDES_CDR_BW_GET(x)                                   (((x) & SGMII_SERDES_CDR_BW_MASK) >> SGMII_SERDES_CDR_BW_LSB)
#define SGMII_SERDES_CDR_BW_SET(x)                                   (((x) << SGMII_SERDES_CDR_BW_LSB) & SGMII_SERDES_CDR_BW_MASK)
#define SGMII_SERDES_CDR_BW_RESET                                    0x3 // 3
#define SGMII_SERDES_RX_IMPEDANCE_MSB                                0
#define SGMII_SERDES_RX_IMPEDANCE_LSB                                0
#define SGMII_SERDES_RX_IMPEDANCE_MASK                               0x00000001
#define SGMII_SERDES_RX_IMPEDANCE_GET(x)                             (((x) & SGMII_SERDES_RX_IMPEDANCE_MASK) >> SGMII_SERDES_RX_IMPEDANCE_LSB)
#define SGMII_SERDES_RX_IMPEDANCE_SET(x)                             (((x) << SGMII_SERDES_RX_IMPEDANCE_LSB) & SGMII_SERDES_RX_IMPEDANCE_MASK)
#define SGMII_SERDES_RX_IMPEDANCE_RESET                              0x0 // 0
#define SGMII_SERDES_ADDRESS                                         0x18070018
#define SGMII_SERDES_OFFSET                                          0x0018

/*
 * G-mac registers
 */


// 32'h0000 (MAC_CONFIGURATION_1)
#define MAC_CONFIGURATION_1_SOFT_RESET_MSB                           31
#define MAC_CONFIGURATION_1_SOFT_RESET_LSB                           31
#define MAC_CONFIGURATION_1_SOFT_RESET_MASK                          0x80000000
#define MAC_CONFIGURATION_1_SOFT_RESET_GET(x)                        (((x) & MAC_CONFIGURATION_1_SOFT_RESET_MASK) >> MAC_CONFIGURATION_1_SOFT_RESET_LSB)
#define MAC_CONFIGURATION_1_SOFT_RESET_SET(x)                        (((x) << MAC_CONFIGURATION_1_SOFT_RESET_LSB) & MAC_CONFIGURATION_1_SOFT_RESET_MASK)
#define MAC_CONFIGURATION_1_SOFT_RESET_RESET                         0x1 // 1
#define MAC_CONFIGURATION_1_SIM_RESET_MSB                            30
#define MAC_CONFIGURATION_1_SIM_RESET_LSB                            30
#define MAC_CONFIGURATION_1_SIM_RESET_MASK                           0x40000000
#define MAC_CONFIGURATION_1_SIM_RESET_GET(x)                         (((x) & MAC_CONFIGURATION_1_SIM_RESET_MASK) >> MAC_CONFIGURATION_1_SIM_RESET_LSB)
#define MAC_CONFIGURATION_1_SIM_RESET_SET(x)                         (((x) << MAC_CONFIGURATION_1_SIM_RESET_LSB) & MAC_CONFIGURATION_1_SIM_RESET_MASK)
#define MAC_CONFIGURATION_1_SIM_RESET_RESET                          0x0 // 0
#define MAC_CONFIGURATION_1_RESET_RX_MAC_MSB                         19
#define MAC_CONFIGURATION_1_RESET_RX_MAC_LSB                         19
#define MAC_CONFIGURATION_1_RESET_RX_MAC_MASK                        0x00080000
#define MAC_CONFIGURATION_1_RESET_RX_MAC_GET(x)                      (((x) & MAC_CONFIGURATION_1_RESET_RX_MAC_MASK) >> MAC_CONFIGURATION_1_RESET_RX_MAC_LSB)
#define MAC_CONFIGURATION_1_RESET_RX_MAC_SET(x)                      (((x) << MAC_CONFIGURATION_1_RESET_RX_MAC_LSB) & MAC_CONFIGURATION_1_RESET_RX_MAC_MASK)
#define MAC_CONFIGURATION_1_RESET_RX_MAC_RESET                       0x0 // 0
#define MAC_CONFIGURATION_1_RESET_TX_MAC_MSB                         18
#define MAC_CONFIGURATION_1_RESET_TX_MAC_LSB                         18
#define MAC_CONFIGURATION_1_RESET_TX_MAC_MASK                        0x00040000
#define MAC_CONFIGURATION_1_RESET_TX_MAC_GET(x)                      (((x) & MAC_CONFIGURATION_1_RESET_TX_MAC_MASK) >> MAC_CONFIGURATION_1_RESET_TX_MAC_LSB)
#define MAC_CONFIGURATION_1_RESET_TX_MAC_SET(x)                      (((x) << MAC_CONFIGURATION_1_RESET_TX_MAC_LSB) & MAC_CONFIGURATION_1_RESET_TX_MAC_MASK)
#define MAC_CONFIGURATION_1_RESET_TX_MAC_RESET                       0x0 // 0
#define MAC_CONFIGURATION_1_RESET_RX_FUNC_MSB                        17
#define MAC_CONFIGURATION_1_RESET_RX_FUNC_LSB                        17
#define MAC_CONFIGURATION_1_RESET_RX_FUNC_MASK                       0x00020000
#define MAC_CONFIGURATION_1_RESET_RX_FUNC_GET(x)                     (((x) & MAC_CONFIGURATION_1_RESET_RX_FUNC_MASK) >> MAC_CONFIGURATION_1_RESET_RX_FUNC_LSB)
#define MAC_CONFIGURATION_1_RESET_RX_FUNC_SET(x)                     (((x) << MAC_CONFIGURATION_1_RESET_RX_FUNC_LSB) & MAC_CONFIGURATION_1_RESET_RX_FUNC_MASK)
#define MAC_CONFIGURATION_1_RESET_RX_FUNC_RESET                      0x0 // 0
#define MAC_CONFIGURATION_1_RESET_TX_FUNC_MSB                        16
#define MAC_CONFIGURATION_1_RESET_TX_FUNC_LSB                        16
#define MAC_CONFIGURATION_1_RESET_TX_FUNC_MASK                       0x00010000
#define MAC_CONFIGURATION_1_RESET_TX_FUNC_GET(x)                     (((x) & MAC_CONFIGURATION_1_RESET_TX_FUNC_MASK) >> MAC_CONFIGURATION_1_RESET_TX_FUNC_LSB)
#define MAC_CONFIGURATION_1_RESET_TX_FUNC_SET(x)                     (((x) << MAC_CONFIGURATION_1_RESET_TX_FUNC_LSB) & MAC_CONFIGURATION_1_RESET_TX_FUNC_MASK)
#define MAC_CONFIGURATION_1_RESET_TX_FUNC_RESET                      0x0 // 0
#define MAC_CONFIGURATION_1_LOOP_BACK_MSB                            8
#define MAC_CONFIGURATION_1_LOOP_BACK_LSB                            8
#define MAC_CONFIGURATION_1_LOOP_BACK_MASK                           0x00000100
#define MAC_CONFIGURATION_1_LOOP_BACK_GET(x)                         (((x) & MAC_CONFIGURATION_1_LOOP_BACK_MASK) >> MAC_CONFIGURATION_1_LOOP_BACK_LSB)
#define MAC_CONFIGURATION_1_LOOP_BACK_SET(x)                         (((x) << MAC_CONFIGURATION_1_LOOP_BACK_LSB) & MAC_CONFIGURATION_1_LOOP_BACK_MASK)
#define MAC_CONFIGURATION_1_LOOP_BACK_RESET                          0x0 // 0
#define MAC_CONFIGURATION_1_RX_FLOW_MSB                              5
#define MAC_CONFIGURATION_1_RX_FLOW_LSB                              5
#define MAC_CONFIGURATION_1_RX_FLOW_MASK                             0x00000020
#define MAC_CONFIGURATION_1_RX_FLOW_GET(x)                           (((x) & MAC_CONFIGURATION_1_RX_FLOW_MASK) >> MAC_CONFIGURATION_1_RX_FLOW_LSB)
#define MAC_CONFIGURATION_1_RX_FLOW_SET(x)                           (((x) << MAC_CONFIGURATION_1_RX_FLOW_LSB) & MAC_CONFIGURATION_1_RX_FLOW_MASK)
#define MAC_CONFIGURATION_1_RX_FLOW_RESET                            0x0 // 0
#define MAC_CONFIGURATION_1_TX_FLOW_MSB                              4
#define MAC_CONFIGURATION_1_TX_FLOW_LSB                              4
#define MAC_CONFIGURATION_1_TX_FLOW_MASK                             0x00000010
#define MAC_CONFIGURATION_1_TX_FLOW_GET(x)                           (((x) & MAC_CONFIGURATION_1_TX_FLOW_MASK) >> MAC_CONFIGURATION_1_TX_FLOW_LSB)
#define MAC_CONFIGURATION_1_TX_FLOW_SET(x)                           (((x) << MAC_CONFIGURATION_1_TX_FLOW_LSB) & MAC_CONFIGURATION_1_TX_FLOW_MASK)
#define MAC_CONFIGURATION_1_TX_FLOW_RESET                            0x0 // 0
#define MAC_CONFIGURATION_1_SYNCED_RX_EN_MSB                         3
#define MAC_CONFIGURATION_1_SYNCED_RX_EN_LSB                         3
#define MAC_CONFIGURATION_1_SYNCED_RX_EN_MASK                        0x00000008
#define MAC_CONFIGURATION_1_SYNCED_RX_EN_GET(x)                      (((x) & MAC_CONFIGURATION_1_SYNCED_RX_EN_MASK) >> MAC_CONFIGURATION_1_SYNCED_RX_EN_LSB)
#define MAC_CONFIGURATION_1_SYNCED_RX_EN_SET(x)                      (((x) << MAC_CONFIGURATION_1_SYNCED_RX_EN_LSB) & MAC_CONFIGURATION_1_SYNCED_RX_EN_MASK)
#define MAC_CONFIGURATION_1_SYNCED_RX_EN_RESET                       0x0 // 0
#define MAC_CONFIGURATION_1_RX_ENABLE_MSB                            2
#define MAC_CONFIGURATION_1_RX_ENABLE_LSB                            2
#define MAC_CONFIGURATION_1_RX_ENABLE_MASK                           0x00000004
#define MAC_CONFIGURATION_1_RX_ENABLE_GET(x)                         (((x) & MAC_CONFIGURATION_1_RX_ENABLE_MASK) >> MAC_CONFIGURATION_1_RX_ENABLE_LSB)
#define MAC_CONFIGURATION_1_RX_ENABLE_SET(x)                         (((x) << MAC_CONFIGURATION_1_RX_ENABLE_LSB) & MAC_CONFIGURATION_1_RX_ENABLE_MASK)
#define MAC_CONFIGURATION_1_RX_ENABLE_RESET                          0x0 // 0
#define MAC_CONFIGURATION_1_SYNCED_TX_EN_MSB                         1
#define MAC_CONFIGURATION_1_SYNCED_TX_EN_LSB                         1
#define MAC_CONFIGURATION_1_SYNCED_TX_EN_MASK                        0x00000002
#define MAC_CONFIGURATION_1_SYNCED_TX_EN_GET(x)                      (((x) & MAC_CONFIGURATION_1_SYNCED_TX_EN_MASK) >> MAC_CONFIGURATION_1_SYNCED_TX_EN_LSB)
#define MAC_CONFIGURATION_1_SYNCED_TX_EN_SET(x)                      (((x) << MAC_CONFIGURATION_1_SYNCED_TX_EN_LSB) & MAC_CONFIGURATION_1_SYNCED_TX_EN_MASK)
#define MAC_CONFIGURATION_1_SYNCED_TX_EN_RESET                       0x0 // 0
#define MAC_CONFIGURATION_1_TX_ENABLE_MSB                            0
#define MAC_CONFIGURATION_1_TX_ENABLE_LSB                            0
#define MAC_CONFIGURATION_1_TX_ENABLE_MASK                           0x00000001
#define MAC_CONFIGURATION_1_TX_ENABLE_GET(x)                         (((x) & MAC_CONFIGURATION_1_TX_ENABLE_MASK) >> MAC_CONFIGURATION_1_TX_ENABLE_LSB)
#define MAC_CONFIGURATION_1_TX_ENABLE_SET(x)                         (((x) << MAC_CONFIGURATION_1_TX_ENABLE_LSB) & MAC_CONFIGURATION_1_TX_ENABLE_MASK)
#define MAC_CONFIGURATION_1_TX_ENABLE_RESET                          0x0 // 0
#define MAC_CONFIGURATION_1_ADDRESS                                  0x0000
#define MAC_CONFIGURATION_1_OFFSET                                   0x0000
// SW modifiable bits
#define MAC_CONFIGURATION_1_SW_MASK                                  0xc00f013f
// bits defined at reset
#define MAC_CONFIGURATION_1_RSTMASK                                  0xffffffff
// reset value (ignore bits undefined at reset)
#define MAC_CONFIGURATION_1_RESET                                    0x80000000

// 32'h0004 (MAC_CONFIGURATION_2)
#define MAC_CONFIGURATION_2_PREAMBLE_LENGTH_MSB                      15
#define MAC_CONFIGURATION_2_PREAMBLE_LENGTH_LSB                      12
#define MAC_CONFIGURATION_2_PREAMBLE_LENGTH_MASK                     0x0000f000
#define MAC_CONFIGURATION_2_PREAMBLE_LENGTH_GET(x)                   (((x) & MAC_CONFIGURATION_2_PREAMBLE_LENGTH_MASK) >> MAC_CONFIGURATION_2_PREAMBLE_LENGTH_LSB)
#define MAC_CONFIGURATION_2_PREAMBLE_LENGTH_SET(x)                   (((x) << MAC_CONFIGURATION_2_PREAMBLE_LENGTH_LSB) & MAC_CONFIGURATION_2_PREAMBLE_LENGTH_MASK)
#define MAC_CONFIGURATION_2_PREAMBLE_LENGTH_RESET                    0x7 // 7
#define MAC_CONFIGURATION_2_IF_MODE_MSB                              9
#define MAC_CONFIGURATION_2_IF_MODE_LSB                              8
#define MAC_CONFIGURATION_2_IF_MODE_MASK                             0x00000300
#define MAC_CONFIGURATION_2_IF_MODE_GET(x)                           (((x) & MAC_CONFIGURATION_2_IF_MODE_MASK) >> MAC_CONFIGURATION_2_IF_MODE_LSB)
#define MAC_CONFIGURATION_2_IF_MODE_SET(x)                           (((x) << MAC_CONFIGURATION_2_IF_MODE_LSB) & MAC_CONFIGURATION_2_IF_MODE_MASK)
#define MAC_CONFIGURATION_2_IF_MODE_RESET                            0x0 // 0
#define MAC_CONFIGURATION_2_HUGE_FRAME_MSB                           5
#define MAC_CONFIGURATION_2_HUGE_FRAME_LSB                           5
#define MAC_CONFIGURATION_2_HUGE_FRAME_MASK                          0x00000020
#define MAC_CONFIGURATION_2_HUGE_FRAME_GET(x)                        (((x) & MAC_CONFIGURATION_2_HUGE_FRAME_MASK) >> MAC_CONFIGURATION_2_HUGE_FRAME_LSB)
#define MAC_CONFIGURATION_2_HUGE_FRAME_SET(x)                        (((x) << MAC_CONFIGURATION_2_HUGE_FRAME_LSB) & MAC_CONFIGURATION_2_HUGE_FRAME_MASK)
#define MAC_CONFIGURATION_2_HUGE_FRAME_RESET                         0x0 // 0
#define MAC_CONFIGURATION_2_LENGTH_CHECK_MSB                         4
#define MAC_CONFIGURATION_2_LENGTH_CHECK_LSB                         4
#define MAC_CONFIGURATION_2_LENGTH_CHECK_MASK                        0x00000010
#define MAC_CONFIGURATION_2_LENGTH_CHECK_GET(x)                      (((x) & MAC_CONFIGURATION_2_LENGTH_CHECK_MASK) >> MAC_CONFIGURATION_2_LENGTH_CHECK_LSB)
#define MAC_CONFIGURATION_2_LENGTH_CHECK_SET(x)                      (((x) << MAC_CONFIGURATION_2_LENGTH_CHECK_LSB) & MAC_CONFIGURATION_2_LENGTH_CHECK_MASK)
#define MAC_CONFIGURATION_2_LENGTH_CHECK_RESET                       0x0 // 0
#define MAC_CONFIGURATION_2_PAD_CRC_MSB                              2
#define MAC_CONFIGURATION_2_PAD_CRC_LSB                              2
#define MAC_CONFIGURATION_2_PAD_CRC_MASK                             0x00000004
#define MAC_CONFIGURATION_2_PAD_CRC_GET(x)                           (((x) & MAC_CONFIGURATION_2_PAD_CRC_MASK) >> MAC_CONFIGURATION_2_PAD_CRC_LSB)
#define MAC_CONFIGURATION_2_PAD_CRC_SET(x)                           (((x) << MAC_CONFIGURATION_2_PAD_CRC_LSB) & MAC_CONFIGURATION_2_PAD_CRC_MASK)
#define MAC_CONFIGURATION_2_PAD_CRC_RESET                            0x0 // 0
#define MAC_CONFIGURATION_2_CRC_ENABLE_MSB                           1
#define MAC_CONFIGURATION_2_CRC_ENABLE_LSB                           1
#define MAC_CONFIGURATION_2_CRC_ENABLE_MASK                          0x00000002
#define MAC_CONFIGURATION_2_CRC_ENABLE_GET(x)                        (((x) & MAC_CONFIGURATION_2_CRC_ENABLE_MASK) >> MAC_CONFIGURATION_2_CRC_ENABLE_LSB)
#define MAC_CONFIGURATION_2_CRC_ENABLE_SET(x)                        (((x) << MAC_CONFIGURATION_2_CRC_ENABLE_LSB) & MAC_CONFIGURATION_2_CRC_ENABLE_MASK)
#define MAC_CONFIGURATION_2_CRC_ENABLE_RESET                         0x0 // 0
#define MAC_CONFIGURATION_2_FULL_DUPLEX_MSB                          0
#define MAC_CONFIGURATION_2_FULL_DUPLEX_LSB                          0
#define MAC_CONFIGURATION_2_FULL_DUPLEX_MASK                         0x00000001
#define MAC_CONFIGURATION_2_FULL_DUPLEX_GET(x)                       (((x) & MAC_CONFIGURATION_2_FULL_DUPLEX_MASK) >> MAC_CONFIGURATION_2_FULL_DUPLEX_LSB)
#define MAC_CONFIGURATION_2_FULL_DUPLEX_SET(x)                       (((x) << MAC_CONFIGURATION_2_FULL_DUPLEX_LSB) & MAC_CONFIGURATION_2_FULL_DUPLEX_MASK)
#define MAC_CONFIGURATION_2_FULL_DUPLEX_RESET                        0x0 // 0
#define MAC_CONFIGURATION_2_ADDRESS                                  0x0004
#define MAC_CONFIGURATION_2_OFFSET                                   0x0004
// SW modifiable bits
#define MAC_CONFIGURATION_2_SW_MASK                                  0x0000f337
// bits defined at reset
#define MAC_CONFIGURATION_2_RSTMASK                                  0xffffffff
// reset value (ignore bits undefined at reset)
#define MAC_CONFIGURATION_2_RESET                                    0x00007000


// 32'h0048 (AMCXFIF_CFG_0)
#define AMCXFIF_CFG_0_FTFENRPLY_MSB                                  20
#define AMCXFIF_CFG_0_FTFENRPLY_LSB                                  20
#define AMCXFIF_CFG_0_FTFENRPLY_MASK                                 0x00100000
#define AMCXFIF_CFG_0_FTFENRPLY_GET(x)                               (((x) & AMCXFIF_CFG_0_FTFENRPLY_MASK) >> AMCXFIF_CFG_0_FTFENRPLY_LSB)
#define AMCXFIF_CFG_0_FTFENRPLY_SET(x)                               (((x) << AMCXFIF_CFG_0_FTFENRPLY_LSB) & AMCXFIF_CFG_0_FTFENRPLY_MASK)
#define AMCXFIF_CFG_0_FTFENRPLY_RESET                                0x0 // 0
#define AMCXFIF_CFG_0_STFENRPLY_MSB                                  19
#define AMCXFIF_CFG_0_STFENRPLY_LSB                                  19
#define AMCXFIF_CFG_0_STFENRPLY_MASK                                 0x00080000
#define AMCXFIF_CFG_0_STFENRPLY_GET(x)                               (((x) & AMCXFIF_CFG_0_STFENRPLY_MASK) >> AMCXFIF_CFG_0_STFENRPLY_LSB)
#define AMCXFIF_CFG_0_STFENRPLY_SET(x)                               (((x) << AMCXFIF_CFG_0_STFENRPLY_LSB) & AMCXFIF_CFG_0_STFENRPLY_MASK)
#define AMCXFIF_CFG_0_STFENRPLY_RESET                                0x0 // 0
#define AMCXFIF_CFG_0_FRFENRPLY_MSB                                  18
#define AMCXFIF_CFG_0_FRFENRPLY_LSB                                  18
#define AMCXFIF_CFG_0_FRFENRPLY_MASK                                 0x00040000
#define AMCXFIF_CFG_0_FRFENRPLY_GET(x)                               (((x) & AMCXFIF_CFG_0_FRFENRPLY_MASK) >> AMCXFIF_CFG_0_FRFENRPLY_LSB)
#define AMCXFIF_CFG_0_FRFENRPLY_SET(x)                               (((x) << AMCXFIF_CFG_0_FRFENRPLY_LSB) & AMCXFIF_CFG_0_FRFENRPLY_MASK)
#define AMCXFIF_CFG_0_FRFENRPLY_RESET                                0x0 // 0
#define AMCXFIF_CFG_0_SRFENRPLY_MSB                                  17
#define AMCXFIF_CFG_0_SRFENRPLY_LSB                                  17
#define AMCXFIF_CFG_0_SRFENRPLY_MASK                                 0x00020000
#define AMCXFIF_CFG_0_SRFENRPLY_GET(x)                               (((x) & AMCXFIF_CFG_0_SRFENRPLY_MASK) >> AMCXFIF_CFG_0_SRFENRPLY_LSB)
#define AMCXFIF_CFG_0_SRFENRPLY_SET(x)                               (((x) << AMCXFIF_CFG_0_SRFENRPLY_LSB) & AMCXFIF_CFG_0_SRFENRPLY_MASK)
#define AMCXFIF_CFG_0_SRFENRPLY_RESET                                0x0 // 0
#define AMCXFIF_CFG_0_WTMENRPLY_MSB                                  16
#define AMCXFIF_CFG_0_WTMENRPLY_LSB                                  16
#define AMCXFIF_CFG_0_WTMENRPLY_MASK                                 0x00010000
#define AMCXFIF_CFG_0_WTMENRPLY_GET(x)                               (((x) & AMCXFIF_CFG_0_WTMENRPLY_MASK) >> AMCXFIF_CFG_0_WTMENRPLY_LSB)
#define AMCXFIF_CFG_0_WTMENRPLY_SET(x)                               (((x) << AMCXFIF_CFG_0_WTMENRPLY_LSB) & AMCXFIF_CFG_0_WTMENRPLY_MASK)
#define AMCXFIF_CFG_0_WTMENRPLY_RESET                                0x0 // 0
#define AMCXFIF_CFG_0_FTFENREQ_MSB                                   12
#define AMCXFIF_CFG_0_FTFENREQ_LSB                                   12
#define AMCXFIF_CFG_0_FTFENREQ_MASK                                  0x00001000
#define AMCXFIF_CFG_0_FTFENREQ_GET(x)                                (((x) & AMCXFIF_CFG_0_FTFENREQ_MASK) >> AMCXFIF_CFG_0_FTFENREQ_LSB)
#define AMCXFIF_CFG_0_FTFENREQ_SET(x)                                (((x) << AMCXFIF_CFG_0_FTFENREQ_LSB) & AMCXFIF_CFG_0_FTFENREQ_MASK)
#define AMCXFIF_CFG_0_FTFENREQ_RESET                                 0x0 // 0
#define AMCXFIF_CFG_0_STFENREQ_MSB                                   11
#define AMCXFIF_CFG_0_STFENREQ_LSB                                   11
#define AMCXFIF_CFG_0_STFENREQ_MASK                                  0x00000800
#define AMCXFIF_CFG_0_STFENREQ_GET(x)                                (((x) & AMCXFIF_CFG_0_STFENREQ_MASK) >> AMCXFIF_CFG_0_STFENREQ_LSB)
#define AMCXFIF_CFG_0_STFENREQ_SET(x)                                (((x) << AMCXFIF_CFG_0_STFENREQ_LSB) & AMCXFIF_CFG_0_STFENREQ_MASK)
#define AMCXFIF_CFG_0_STFENREQ_RESET                                 0x0 // 0
#define AMCXFIF_CFG_0_FRFENREQ_MSB                                   10
#define AMCXFIF_CFG_0_FRFENREQ_LSB                                   10
#define AMCXFIF_CFG_0_FRFENREQ_MASK                                  0x00000400
#define AMCXFIF_CFG_0_FRFENREQ_GET(x)                                (((x) & AMCXFIF_CFG_0_FRFENREQ_MASK) >> AMCXFIF_CFG_0_FRFENREQ_LSB)
#define AMCXFIF_CFG_0_FRFENREQ_SET(x)                                (((x) << AMCXFIF_CFG_0_FRFENREQ_LSB) & AMCXFIF_CFG_0_FRFENREQ_MASK)
#define AMCXFIF_CFG_0_FRFENREQ_RESET                                 0x0 // 0
#define AMCXFIF_CFG_0_SRFENREQ_MSB                                   9
#define AMCXFIF_CFG_0_SRFENREQ_LSB                                   9
#define AMCXFIF_CFG_0_SRFENREQ_MASK                                  0x00000200
#define AMCXFIF_CFG_0_SRFENREQ_GET(x)                                (((x) & AMCXFIF_CFG_0_SRFENREQ_MASK) >> AMCXFIF_CFG_0_SRFENREQ_LSB)
#define AMCXFIF_CFG_0_SRFENREQ_SET(x)                                (((x) << AMCXFIF_CFG_0_SRFENREQ_LSB) & AMCXFIF_CFG_0_SRFENREQ_MASK)
#define AMCXFIF_CFG_0_SRFENREQ_RESET                                 0x0 // 0
#define AMCXFIF_CFG_0_WTMENREQ_MSB                                   8
#define AMCXFIF_CFG_0_WTMENREQ_LSB                                   8
#define AMCXFIF_CFG_0_WTMENREQ_MASK                                  0x00000100
#define AMCXFIF_CFG_0_WTMENREQ_GET(x)                                (((x) & AMCXFIF_CFG_0_WTMENREQ_MASK) >> AMCXFIF_CFG_0_WTMENREQ_LSB)
#define AMCXFIF_CFG_0_WTMENREQ_SET(x)                                (((x) << AMCXFIF_CFG_0_WTMENREQ_LSB) & AMCXFIF_CFG_0_WTMENREQ_MASK)
#define AMCXFIF_CFG_0_WTMENREQ_RESET                                 0x0 // 0
#define AMCXFIF_CFG_0_HSTRSTFT_MSB                                   4
#define AMCXFIF_CFG_0_HSTRSTFT_LSB                                   4
#define AMCXFIF_CFG_0_HSTRSTFT_MASK                                  0x00000010
#define AMCXFIF_CFG_0_HSTRSTFT_GET(x)                                (((x) & AMCXFIF_CFG_0_HSTRSTFT_MASK) >> AMCXFIF_CFG_0_HSTRSTFT_LSB)
#define AMCXFIF_CFG_0_HSTRSTFT_SET(x)                                (((x) << AMCXFIF_CFG_0_HSTRSTFT_LSB) & AMCXFIF_CFG_0_HSTRSTFT_MASK)
#define AMCXFIF_CFG_0_HSTRSTFT_RESET                                 0x0 // 0
#define AMCXFIF_CFG_0_HSTRSTST_MSB                                   3
#define AMCXFIF_CFG_0_HSTRSTST_LSB                                   3
#define AMCXFIF_CFG_0_HSTRSTST_MASK                                  0x00000008
#define AMCXFIF_CFG_0_HSTRSTST_GET(x)                                (((x) & AMCXFIF_CFG_0_HSTRSTST_MASK) >> AMCXFIF_CFG_0_HSTRSTST_LSB)
#define AMCXFIF_CFG_0_HSTRSTST_SET(x)                                (((x) << AMCXFIF_CFG_0_HSTRSTST_LSB) & AMCXFIF_CFG_0_HSTRSTST_MASK)
#define AMCXFIF_CFG_0_HSTRSTST_RESET                                 0x0 // 0
#define AMCXFIF_CFG_0_HSTRSTFR_MSB                                   2
#define AMCXFIF_CFG_0_HSTRSTFR_LSB                                   2
#define AMCXFIF_CFG_0_HSTRSTFR_MASK                                  0x00000004
#define AMCXFIF_CFG_0_HSTRSTFR_GET(x)                                (((x) & AMCXFIF_CFG_0_HSTRSTFR_MASK) >> AMCXFIF_CFG_0_HSTRSTFR_LSB)
#define AMCXFIF_CFG_0_HSTRSTFR_SET(x)                                (((x) << AMCXFIF_CFG_0_HSTRSTFR_LSB) & AMCXFIF_CFG_0_HSTRSTFR_MASK)
#define AMCXFIF_CFG_0_HSTRSTFR_RESET                                 0x0 // 0
#define AMCXFIF_CFG_0_HSTRSTSR_MSB                                   1
#define AMCXFIF_CFG_0_HSTRSTSR_LSB                                   1
#define AMCXFIF_CFG_0_HSTRSTSR_MASK                                  0x00000002
#define AMCXFIF_CFG_0_HSTRSTSR_GET(x)                                (((x) & AMCXFIF_CFG_0_HSTRSTSR_MASK) >> AMCXFIF_CFG_0_HSTRSTSR_LSB)
#define AMCXFIF_CFG_0_HSTRSTSR_SET(x)                                (((x) << AMCXFIF_CFG_0_HSTRSTSR_LSB) & AMCXFIF_CFG_0_HSTRSTSR_MASK)
#define AMCXFIF_CFG_0_HSTRSTSR_RESET                                 0x0 // 0
#define AMCXFIF_CFG_0_HSTRSTWT_MSB                                   0
#define AMCXFIF_CFG_0_HSTRSTWT_LSB                                   0
#define AMCXFIF_CFG_0_HSTRSTWT_MASK                                  0x00000001
#define AMCXFIF_CFG_0_HSTRSTWT_GET(x)                                (((x) & AMCXFIF_CFG_0_HSTRSTWT_MASK) >> AMCXFIF_CFG_0_HSTRSTWT_LSB)
#define AMCXFIF_CFG_0_HSTRSTWT_SET(x)                                (((x) << AMCXFIF_CFG_0_HSTRSTWT_LSB) & AMCXFIF_CFG_0_HSTRSTWT_MASK)
#define AMCXFIF_CFG_0_HSTRSTWT_RESET                                 0x0 // 0
#define AMCXFIF_CFG_0_ADDRESS                                        0x0048
#define AMCXFIF_CFG_0_OFFSET                                         0x0048

// 32'h0010 (MAXIMUM_FRAME_LENGTH)
#define MAXIMUM_FRAME_LENGTH_MAXIMUM_FRAME_MSB                       15
#define MAXIMUM_FRAME_LENGTH_MAXIMUM_FRAME_LSB                       0
#define MAXIMUM_FRAME_LENGTH_MAXIMUM_FRAME_MASK                      0x0000ffff
#define MAXIMUM_FRAME_LENGTH_MAXIMUM_FRAME_GET(x)                    (((x) & MAXIMUM_FRAME_LENGTH_MAXIMUM_FRAME_MASK) >> MAXIMUM_FRAME_LENGTH_MAXIMUM_FRAME_LSB)
#define MAXIMUM_FRAME_LENGTH_MAXIMUM_FRAME_SET(x)                    (((x) << MAXIMUM_FRAME_LENGTH_MAXIMUM_FRAME_LSB) & MAXIMUM_FRAME_LENGTH_MAXIMUM_FRAME_MASK)
#define MAXIMUM_FRAME_LENGTH_MAXIMUM_FRAME_RESET                     0x600 // 1536
#define MAXIMUM_FRAME_LENGTH_ADDRESS                                 0x0010
#define MAXIMUM_FRAME_LENGTH_OFFSET                                  0x0010

// 32'h0020 (MII_MGMT_CONFIGURATION)
#define MII_MGMT_CONFIGURATION_RESET_MGMT_MSB                        31
#define MII_MGMT_CONFIGURATION_RESET_MGMT_LSB                        31
#define MII_MGMT_CONFIGURATION_RESET_MGMT_MASK                       0x80000000
#define MII_MGMT_CONFIGURATION_RESET_MGMT_GET(x)                     (((x) & MII_MGMT_CONFIGURATION_RESET_MGMT_MASK) >> MII_MGMT_CONFIGURATION_RESET_MGMT_LSB)
#define MII_MGMT_CONFIGURATION_RESET_MGMT_SET(x)                     (((x) << MII_MGMT_CONFIGURATION_RESET_MGMT_LSB) & MII_MGMT_CONFIGURATION_RESET_MGMT_MASK)
#define MII_MGMT_CONFIGURATION_RESET_MGMT_RESET                      0x0 // 0
#define MII_MGMT_CONFIGURATION_SCAN_AUTO_MSB                         5
#define MII_MGMT_CONFIGURATION_SCAN_AUTO_LSB                         5
#define MII_MGMT_CONFIGURATION_SCAN_AUTO_MASK                        0x00000020
#define MII_MGMT_CONFIGURATION_SCAN_AUTO_GET(x)                      (((x) & MII_MGMT_CONFIGURATION_SCAN_AUTO_MASK) >> MII_MGMT_CONFIGURATION_SCAN_AUTO_LSB)
#define MII_MGMT_CONFIGURATION_SCAN_AUTO_SET(x)                      (((x) << MII_MGMT_CONFIGURATION_SCAN_AUTO_LSB) & MII_MGMT_CONFIGURATION_SCAN_AUTO_MASK)
#define MII_MGMT_CONFIGURATION_SCAN_AUTO_RESET                       0x0 // 0
#define MII_MGMT_CONFIGURATION_NO_PRE_MSB                            4
#define MII_MGMT_CONFIGURATION_NO_PRE_LSB                            4
#define MII_MGMT_CONFIGURATION_NO_PRE_MASK                           0x00000010
#define MII_MGMT_CONFIGURATION_NO_PRE_GET(x)                         (((x) & MII_MGMT_CONFIGURATION_NO_PRE_MASK) >> MII_MGMT_CONFIGURATION_NO_PRE_LSB)
#define MII_MGMT_CONFIGURATION_NO_PRE_SET(x)                         (((x) << MII_MGMT_CONFIGURATION_NO_PRE_LSB) & MII_MGMT_CONFIGURATION_NO_PRE_MASK)
#define MII_MGMT_CONFIGURATION_NO_PRE_RESET                          0x0 // 0
#define MII_MGMT_CONFIGURATION_MGMT_CLOCK_SELECT_MSB                 3
#define MII_MGMT_CONFIGURATION_MGMT_CLOCK_SELECT_LSB                 0
#define MII_MGMT_CONFIGURATION_MGMT_CLOCK_SELECT_MASK                0x0000000f
#define MII_MGMT_CONFIGURATION_MGMT_CLOCK_SELECT_GET(x)              (((x) & MII_MGMT_CONFIGURATION_MGMT_CLOCK_SELECT_MASK) >> MII_MGMT_CONFIGURATION_MGMT_CLOCK_SELECT_LSB)
#define MII_MGMT_CONFIGURATION_MGMT_CLOCK_SELECT_SET(x)              (((x) << MII_MGMT_CONFIGURATION_MGMT_CLOCK_SELECT_LSB) & MII_MGMT_CONFIGURATION_MGMT_CLOCK_SELECT_MASK)
#define MII_MGMT_CONFIGURATION_MGMT_CLOCK_SELECT_RESET               0x0 // 0
#define MII_MGMT_CONFIGURATION_ADDRESS                               0x0020
#define MII_MGMT_CONFIGURATION_OFFSET                                0x0020


// 32'h0038 (INTERFACE_CONTROL)
#define INTERFACE_CONTROL_UMWOLEN_MSB                                13
#define INTERFACE_CONTROL_UMWOLEN_LSB                                13
#define INTERFACE_CONTROL_UMWOLEN_MASK                               0x00002000
#define INTERFACE_CONTROL_UMWOLEN_GET(x)                             (((x) & INTERFACE_CONTROL_UMWOLEN_MASK) >> INTERFACE_CONTROL_UMWOLEN_LSB)
#define INTERFACE_CONTROL_UMWOLEN_SET(x)                             (((x) << INTERFACE_CONTROL_UMWOLEN_LSB) & INTERFACE_CONTROL_UMWOLEN_MASK)
#define INTERFACE_CONTROL_UMWOLEN_RESET                              0x0 // 0
#define INTERFACE_CONTROL_MPWOLEN_MSB                                12
#define INTERFACE_CONTROL_MPWOLEN_LSB                                12
#define INTERFACE_CONTROL_MPWOLEN_MASK                               0x00001000
#define INTERFACE_CONTROL_MPWOLEN_GET(x)                             (((x) & INTERFACE_CONTROL_MPWOLEN_MASK) >> INTERFACE_CONTROL_MPWOLEN_LSB)
#define INTERFACE_CONTROL_MPWOLEN_SET(x)                             (((x) << INTERFACE_CONTROL_MPWOLEN_LSB) & INTERFACE_CONTROL_MPWOLEN_MASK)
#define INTERFACE_CONTROL_MPWOLEN_RESET                              0x0 // 0
#define INTERFACE_CONTROL_WOLDTCTDCLR_MSB                            11
#define INTERFACE_CONTROL_WOLDTCTDCLR_LSB                            11
#define INTERFACE_CONTROL_WOLDTCTDCLR_MASK                           0x00000800
#define INTERFACE_CONTROL_WOLDTCTDCLR_GET(x)                         (((x) & INTERFACE_CONTROL_WOLDTCTDCLR_MASK) >> INTERFACE_CONTROL_WOLDTCTDCLR_LSB)
#define INTERFACE_CONTROL_WOLDTCTDCLR_SET(x)                         (((x) << INTERFACE_CONTROL_WOLDTCTDCLR_LSB) & INTERFACE_CONTROL_WOLDTCTDCLR_MASK)
#define INTERFACE_CONTROL_WOLDTCTDCLR_RESET                          0x0 // 0
#define INTERFACE_CONTROL_ADDRESS                                    0x0038
#define INTERFACE_CONTROL_OFFSET                                     0x0038

// 32'h004c (AMCXFIF_CFG_1)
#define AMCXFIF_CFG_1_CFGFRTH_MSB                                    25
#define AMCXFIF_CFG_1_CFGFRTH_LSB                                    16
#define AMCXFIF_CFG_1_CFGFRTH_MASK                                   0x03ff0000
#define AMCXFIF_CFG_1_CFGFRTH_GET(x)                                 (((x) & AMCXFIF_CFG_1_CFGFRTH_MASK) >> AMCXFIF_CFG_1_CFGFRTH_LSB)
#define AMCXFIF_CFG_1_CFGFRTH_SET(x)                                 (((x) << AMCXFIF_CFG_1_CFGFRTH_LSB) & AMCXFIF_CFG_1_CFGFRTH_MASK)
#define AMCXFIF_CFG_1_CFGFRTH_RESET                                  0x3ff // 1023
#define AMCXFIF_CFG_1_CFGXOFFRTX_MSB                                 15
#define AMCXFIF_CFG_1_CFGXOFFRTX_LSB                                 0
#define AMCXFIF_CFG_1_CFGXOFFRTX_MASK                                0x0000ffff
#define AMCXFIF_CFG_1_CFGXOFFRTX_GET(x)                              (((x) & AMCXFIF_CFG_1_CFGXOFFRTX_MASK) >> AMCXFIF_CFG_1_CFGXOFFRTX_LSB)
#define AMCXFIF_CFG_1_CFGXOFFRTX_SET(x)                              (((x) << AMCXFIF_CFG_1_CFGXOFFRTX_LSB) & AMCXFIF_CFG_1_CFGXOFFRTX_MASK)
#define AMCXFIF_CFG_1_CFGXOFFRTX_RESET                               0xffff // 65535
#define AMCXFIF_CFG_1_ADDRESS                                        0x004c
#define AMCXFIF_CFG_1_OFFSET                                         0x004c


// 32'h0050 (AMCXFIF_CFG_2)
#define AMCXFIF_CFG_2_CFGHWM_MSB                                     26
#define AMCXFIF_CFG_2_CFGHWM_LSB                                     16
#define AMCXFIF_CFG_2_CFGHWM_MASK                                    0x07ff0000
#define AMCXFIF_CFG_2_CFGHWM_GET(x)                                  (((x) & AMCXFIF_CFG_2_CFGHWM_MASK) >> AMCXFIF_CFG_2_CFGHWM_LSB)
#define AMCXFIF_CFG_2_CFGHWM_SET(x)                                  (((x) << AMCXFIF_CFG_2_CFGHWM_LSB) & AMCXFIF_CFG_2_CFGHWM_MASK)
#define AMCXFIF_CFG_2_CFGHWM_RESET                                   0x2aa // 682
#define AMCXFIF_CFG_2_CFGLWM_MSB                                     10
#define AMCXFIF_CFG_2_CFGLWM_LSB                                     0
#define AMCXFIF_CFG_2_CFGLWM_MASK                                    0x000007ff
#define AMCXFIF_CFG_2_CFGLWM_GET(x)                                  (((x) & AMCXFIF_CFG_2_CFGLWM_MASK) >> AMCXFIF_CFG_2_CFGLWM_LSB)
#define AMCXFIF_CFG_2_CFGLWM_SET(x)                                  (((x) << AMCXFIF_CFG_2_CFGLWM_LSB) & AMCXFIF_CFG_2_CFGLWM_MASK)
#define AMCXFIF_CFG_2_CFGLWM_RESET                                   0x155 // 341
#define AMCXFIF_CFG_2_ADDRESS                                        0x0050
#define AMCXFIF_CFG_2_OFFSET                                         0x0050


// 32'h0054 (AMCXFIF_CFG_3)
#define AMCXFIF_CFG_3_CFGHWMFT_MSB                                   25
#define AMCXFIF_CFG_3_CFGHWMFT_LSB                                   16
#define AMCXFIF_CFG_3_CFGHWMFT_MASK                                  0x03ff0000
#define AMCXFIF_CFG_3_CFGHWMFT_GET(x)                                (((x) & AMCXFIF_CFG_3_CFGHWMFT_MASK) >> AMCXFIF_CFG_3_CFGHWMFT_LSB)
#define AMCXFIF_CFG_3_CFGHWMFT_SET(x)                                (((x) << AMCXFIF_CFG_3_CFGHWMFT_LSB) & AMCXFIF_CFG_3_CFGHWMFT_MASK)
#define AMCXFIF_CFG_3_CFGHWMFT_RESET                                 0x155 // 341
#define AMCXFIF_CFG_3_CFGFTTH_MSB                                    9
#define AMCXFIF_CFG_3_CFGFTTH_LSB                                    0
#define AMCXFIF_CFG_3_CFGFTTH_MASK                                   0x000003ff
#define AMCXFIF_CFG_3_CFGFTTH_GET(x)                                 (((x) & AMCXFIF_CFG_3_CFGFTTH_MASK) >> AMCXFIF_CFG_3_CFGFTTH_LSB)
#define AMCXFIF_CFG_3_CFGFTTH_SET(x)                                 (((x) << AMCXFIF_CFG_3_CFGFTTH_LSB) & AMCXFIF_CFG_3_CFGFTTH_MASK)
#define AMCXFIF_CFG_3_CFGFTTH_RESET                                  0x3ff // 1023
#define AMCXFIF_CFG_3_ADDRESS                                        0x0054
#define AMCXFIF_CFG_3_OFFSET                                         0x0054

// 32'h0058 (AMCXFIF_CFG_4)
#define AMCXFIF_CFG_4_HSTFLTRFRM_MSB                                 17
#define AMCXFIF_CFG_4_HSTFLTRFRM_LSB                                 0
#define AMCXFIF_CFG_4_HSTFLTRFRM_MASK                                0x0003ffff
#define AMCXFIF_CFG_4_HSTFLTRFRM_GET(x)                              (((x) & AMCXFIF_CFG_4_HSTFLTRFRM_MASK) >> AMCXFIF_CFG_4_HSTFLTRFRM_LSB)
#define AMCXFIF_CFG_4_HSTFLTRFRM_SET(x)                              (((x) << AMCXFIF_CFG_4_HSTFLTRFRM_LSB) & AMCXFIF_CFG_4_HSTFLTRFRM_MASK)
#define AMCXFIF_CFG_4_HSTFLTRFRM_RESET                               0x0 // 0
#define AMCXFIF_CFG_4_ADDRESS                                        0x0058
#define AMCXFIF_CFG_4_OFFSET                                         0x0058


// 32'h005c (AMCXFIF_CFG_5)
#define AMCXFIF_CFG_5_SRFULL_MSB                                     21
#define AMCXFIF_CFG_5_SRFULL_LSB                                     21
#define AMCXFIF_CFG_5_SRFULL_MASK                                    0x00200000
#define AMCXFIF_CFG_5_SRFULL_GET(x)                                  (((x) & AMCXFIF_CFG_5_SRFULL_MASK) >> AMCXFIF_CFG_5_SRFULL_LSB)
#define AMCXFIF_CFG_5_SRFULL_SET(x)                                  (((x) << AMCXFIF_CFG_5_SRFULL_LSB) & AMCXFIF_CFG_5_SRFULL_MASK)
#define AMCXFIF_CFG_5_SRFULL_RESET                                   0x0 // 0
#define AMCXFIF_CFG_5_HSTSRFULLCLR_MSB                               20
#define AMCXFIF_CFG_5_HSTSRFULLCLR_LSB                               20
#define AMCXFIF_CFG_5_HSTSRFULLCLR_MASK                              0x00100000
#define AMCXFIF_CFG_5_HSTSRFULLCLR_GET(x)                            (((x) & AMCXFIF_CFG_5_HSTSRFULLCLR_MASK) >> AMCXFIF_CFG_5_HSTSRFULLCLR_LSB)
#define AMCXFIF_CFG_5_HSTSRFULLCLR_SET(x)                            (((x) << AMCXFIF_CFG_5_HSTSRFULLCLR_LSB) & AMCXFIF_CFG_5_HSTSRFULLCLR_MASK)
#define AMCXFIF_CFG_5_HSTSRFULLCLR_RESET                             0x0 // 0
#define AMCXFIF_CFG_5_CFGBYTMODE_MSB                                 19
#define AMCXFIF_CFG_5_CFGBYTMODE_LSB                                 19
#define AMCXFIF_CFG_5_CFGBYTMODE_MASK                                0x00080000
#define AMCXFIF_CFG_5_CFGBYTMODE_GET(x)                              (((x) & AMCXFIF_CFG_5_CFGBYTMODE_MASK) >> AMCXFIF_CFG_5_CFGBYTMODE_LSB)
#define AMCXFIF_CFG_5_CFGBYTMODE_SET(x)                              (((x) << AMCXFIF_CFG_5_CFGBYTMODE_LSB) & AMCXFIF_CFG_5_CFGBYTMODE_MASK)
#define AMCXFIF_CFG_5_CFGBYTMODE_RESET                               0x0 // 0
#define AMCXFIF_CFG_5_HSTDRPLT64_MSB                                 18
#define AMCXFIF_CFG_5_HSTDRPLT64_LSB                                 18
#define AMCXFIF_CFG_5_HSTDRPLT64_MASK                                0x00040000
#define AMCXFIF_CFG_5_HSTDRPLT64_GET(x)                              (((x) & AMCXFIF_CFG_5_HSTDRPLT64_MASK) >> AMCXFIF_CFG_5_HSTDRPLT64_LSB)
#define AMCXFIF_CFG_5_HSTDRPLT64_SET(x)                              (((x) << AMCXFIF_CFG_5_HSTDRPLT64_LSB) & AMCXFIF_CFG_5_HSTDRPLT64_MASK)
#define AMCXFIF_CFG_5_HSTDRPLT64_RESET                               0x0 // 0
#define AMCXFIF_CFG_5_HSTFLTRFRMDC_MSB                               17
#define AMCXFIF_CFG_5_HSTFLTRFRMDC_LSB                               0
#define AMCXFIF_CFG_5_HSTFLTRFRMDC_MASK                              0x0003ffff
#define AMCXFIF_CFG_5_HSTFLTRFRMDC_GET(x)                            (((x) & AMCXFIF_CFG_5_HSTFLTRFRMDC_MASK) >> AMCXFIF_CFG_5_HSTFLTRFRMDC_LSB)
#define AMCXFIF_CFG_5_HSTFLTRFRMDC_SET(x)                            (((x) << AMCXFIF_CFG_5_HSTFLTRFRMDC_LSB) & AMCXFIF_CFG_5_HSTFLTRFRMDC_MASK)
#define AMCXFIF_CFG_5_HSTFLTRFRMDC_RESET                             0x3ffff // 262143
#define AMCXFIF_CFG_5_ADDRESS                                        0x005c
#define AMCXFIF_CFG_5_OFFSET                                         0x005c


// 32'h01a4 (TXFIFO_TH)
#define TXFIFO_TH_TXFIFO_MAXTH_MSB                                   25
#define TXFIFO_TH_TXFIFO_MAXTH_LSB                                   16
#define TXFIFO_TH_TXFIFO_MAXTH_MASK                                  0x03ff0000
#define TXFIFO_TH_TXFIFO_MAXTH_GET(x)                                (((x) & TXFIFO_TH_TXFIFO_MAXTH_MASK) >> TXFIFO_TH_TXFIFO_MAXTH_LSB)
#define TXFIFO_TH_TXFIFO_MAXTH_SET(x)                                (((x) << TXFIFO_TH_TXFIFO_MAXTH_LSB) & TXFIFO_TH_TXFIFO_MAXTH_MASK)
#define TXFIFO_TH_TXFIFO_MAXTH_RESET                                 0x1d8 // 472
#define TXFIFO_TH_TXFIFO_MINTH_MSB                                   9
#define TXFIFO_TH_TXFIFO_MINTH_LSB                                   0
#define TXFIFO_TH_TXFIFO_MINTH_MASK                                  0x000003ff
#define TXFIFO_TH_TXFIFO_MINTH_GET(x)                                (((x) & TXFIFO_TH_TXFIFO_MINTH_MASK) >> TXFIFO_TH_TXFIFO_MINTH_LSB)
#define TXFIFO_TH_TXFIFO_MINTH_SET(x)                                (((x) << TXFIFO_TH_TXFIFO_MINTH_LSB) & TXFIFO_TH_TXFIFO_MINTH_MASK)
#define TXFIFO_TH_TXFIFO_MINTH_RESET                                 0x160 // 352
#define TXFIFO_TH_ADDRESS                                            0x01a4
#define TXFIFO_TH_OFFSET                                             0x01a4




// 32'h01bc (DMA_RESET)
#define DMA_RESET_DMA_TX_RESET_MSB                                   31
#define DMA_RESET_DMA_TX_RESET_LSB                                   31
#define DMA_RESET_DMA_TX_RESET_MASK                                  0x80000000
#define DMA_RESET_DMA_TX_RESET_GET(x)                                (((x) & DMA_RESET_DMA_TX_RESET_MASK) >> DMA_RESET_DMA_TX_RESET_LSB)
#define DMA_RESET_DMA_TX_RESET_SET(x)                                (((x) << DMA_RESET_DMA_TX_RESET_LSB) & DMA_RESET_DMA_TX_RESET_MASK)
#define DMA_RESET_DMA_TX_RESET_RESET                                 0x0 // 0
#define DMA_RESET_DMA_RX_RESET_MSB                                   30
#define DMA_RESET_DMA_RX_RESET_LSB                                   30
#define DMA_RESET_DMA_RX_RESET_MASK                                  0x40000000
#define DMA_RESET_DMA_RX_RESET_GET(x)                                (((x) & DMA_RESET_DMA_RX_RESET_MASK) >> DMA_RESET_DMA_RX_RESET_LSB)
#define DMA_RESET_DMA_RX_RESET_SET(x)                                (((x) << DMA_RESET_DMA_RX_RESET_LSB) & DMA_RESET_DMA_RX_RESET_MASK)
#define DMA_RESET_DMA_RX_RESET_RESET                                 0x0 // 0
#define DMA_RESET_SCRATCHREG_2_MSB                                   29
#define DMA_RESET_SCRATCHREG_2_LSB                                   0
#define DMA_RESET_SCRATCHREG_2_MASK                                  0x3fffffff
#define DMA_RESET_SCRATCHREG_2_GET(x)                                (((x) & DMA_RESET_SCRATCHREG_2_MASK) >> DMA_RESET_SCRATCHREG_2_LSB)
#define DMA_RESET_SCRATCHREG_2_SET(x)                                (((x) << DMA_RESET_SCRATCHREG_2_LSB) & DMA_RESET_SCRATCHREG_2_MASK)
#define DMA_RESET_SCRATCHREG_2_RESET                                 0x0 // 0
#define DMA_RESET_ADDRESS                                            0x01bc
// SW modifiable bits
#define DMA_RESET_SW_MASK                                            0xffffffff
// bits defined at reset
#define DMA_RESET_RSTMASK                                            0xffffffff
// reset value (ignore bits undefined at reset)
#define DMA_RESET_RESET                                              0x00000000




/* Moved from 955x.h */

/*
 * Config/Mac Register definitions
 */
#define ATHR_GMAC_CFG1                 0x00
#define ATHR_GMAC_CFG2                 0x04
#define ATHR_GMAC_IFCTL                0x38
#define ATHR_GMAC_MAX_PKTLEN           0x10




/*
 * fifo control registers
 */
#define ATHR_GMAC_FIFO_CFG_0           0x48
#define ATHR_GMAC_FIFO_CFG_1           0x4c
#define ATHR_GMAC_FIFO_CFG_2           0x50
#define ATHR_GMAC_FIFO_CFG_3           0x54
#define ATHR_GMAC_FIFO_CFG_4           0x58

#define ATHR_GMAC_FIFO_CFG_5           0x5c
#define ATHR_GMAC_BYTE_PER_CLK_EN          (1 << 19)

#define ATHR_GMAC_FIFO_RAM_0           0x60
#define ATHR_GMAC_FIFO_RAM_1           0x64
#define ATHR_GMAC_FIFO_RAM_2           0x68
#define ATHR_GMAC_FIFO_RAM_3           0x6c
#define ATHR_GMAC_FIFO_RAM_4           0x70
#define ATHR_GMAC_FIFO_RAM_5           0x74
#define ATHR_GMAC_FIFO_RAM_6           0x78
#define ATHR_GMAC_FIFO_RAM_7           0x7c

/*
 * fields
 */
#define ATHR_GMAC_CFG1_SOFT_RST        (1 << 31)
#define ATHR_GMAC_CFG1_RX_RST          (1 << 19)
#define ATHR_GMAC_CFG1_TX_RST          (1 << 18)
#define ATHR_GMAC_CFG1_LOOPBACK        (1 << 8)
#define ATHR_GMAC_CFG1_RX_EN           (1 << 2)
#define ATHR_GMAC_CFG1_TX_EN           (1 << 0)
#define ATHR_GMAC_CFG1_RX_FCTL         (1 << 5)
#define ATHR_GMAC_CFG1_TX_FCTL         (1 << 4)


#define ATHR_GMAC_CFG2_FDX             (1 << 0)
#define ATHR_GMAC_CFG2_CRC_EN          (1 << 1)
#define ATHR_GMAC_CFG2_PAD_CRC_EN      (1 << 2)
#define ATHR_GMAC_CFG2_LEN_CHECK       (1 << 4)
#define ATHR_GMAC_CFG2_HUGE_FRAME_EN   (1 << 5)
#define ATHR_GMAC_CFG2_IF_1000         (1 << 9)
#define ATHR_GMAC_CFG2_IF_10_100       (1 << 8)

#define ATHR_GMAC_IFCTL_SPEED          (1 << 16)

/*
 * DMA (tx/rx) register defines
 */
#define ATHR_GMAC_DMA_TX_CTRL               0x180
#define ATHR_GMAC_DMA_TX_DESC               0x184
#define ATHR_GMAC_DMA_TX_STATUS             0x188
#define ATHR_GMAC_DMA_RX_CTRL               0x18c
#define ATHR_GMAC_DMA_RX_DESC               0x190
#define ATHR_GMAC_DMA_RX_STATUS             0x194
#define ATHR_GMAC_DMA_INTR_MASK             0x198
#define ATHR_GMAC_DMA_INTR                  0x19c
#define ATHR_GMAC_DMA_RXFSM		    0x1b0
#define ATHR_GMAC_DMA_TXFSM		    0x1b4
#define ATHR_GMAC_DMA_FIFO_THRESH           0x1a4
#define ATHR_GMAC_DMA_XFIFO_DEPTH	    0x1a8

/*
 * DMA status mask.
 */

#define ATHR_GMAC_DMA_DMA_STATE 	    0x3
#define ATHR_GMAC_DMA_AHB_STATE 	    0x7

/*
 * QOS register Defines
 */

#define ATHR_GMAC_DMA_TX_CTRL_Q0            0x180
#define ATHR_GMAC_DMA_TX_CTRL_Q1            0x1C0
#define ATHR_GMAC_DMA_TX_CTRL_Q2            0x1C8
#define ATHR_GMAC_DMA_TX_CTRL_Q3            0x1D0

#define ATHR_GMAC_DMA_TX_DESC_Q0            0x184
#define ATHR_GMAC_DMA_TX_DESC_Q1            0x1C4
#define ATHR_GMAC_DMA_TX_DESC_Q2            0x1CC
#define ATHR_GMAC_DMA_TX_DESC_Q3            0x1D4

#define ATHR_GMAC_DMA_TX_ARB_CFG            0x1D8
#define ATHR_GMAC_TX_QOS_MODE_FIXED         0x0
#define ATHR_GMAC_TX_QOS_MODE_WEIGHTED      0x1
#define ATHR_GMAC_TX_QOS_WGT_0(x)	    ((x & 0x3F) << 8)
#define ATHR_GMAC_TX_QOS_WGT_1(x)	    ((x & 0x3F) << 14)
#define ATHR_GMAC_TX_QOS_WGT_2(x)	    ((x & 0x3F) << 20)
#define ATHR_GMAC_TX_QOS_WGT_3(x)	    ((x & 0x3F) << 26)

/*
 * tx/rx ctrl and status bits
 */
#define ATHR_GMAC_TXE                       (1 << 0)
#define ATHR_GMAC_TX_STATUS_PKTCNT_SHIFT    16
#define ATHR_GMAC_TX_STATUS_PKT_SENT        0x1
#define ATHR_GMAC_TX_STATUS_URN             0x2
#define ATHR_GMAC_TX_STATUS_BUS_ERROR       0x8

#define ATHR_GMAC_RXE                       (1 << 0)

#define ATHR_GMAC_RX_STATUS_PKTCNT_MASK     0xff0000
#define ATHR_GMAC_RX_STATUS_PKT_RCVD        (1 << 0)
#define ATHR_GMAC_RX_STATUS_OVF             (1 << 2)
#define ATHR_GMAC_RX_STATUS_BUS_ERROR       (1 << 3)

/*
 * Int and int mask
 */
#define ATHR_GMAC_INTR_TX                    (1 << 0)
#define ATHR_GMAC_INTR_TX_URN                (1 << 1)
#define ATHR_GMAC_INTR_TX_BUS_ERROR          (1 << 3)
#define ATHR_GMAC_INTR_RX                    (1 << 4)
#define ATHR_GMAC_INTR_RX_OVF                (1 << 6)
#define ATHR_GMAC_INTR_RX_BUS_ERROR          (1 << 7)

/*
 * MII registers
 */
#define ATHR_GMAC_MII_MGMT_CFG               0x20
#define ATHR_GMAC_MGMT_CFG_CLK_DIV_20        0x06
#define ATHR_GMAC_MGMT_CFG_CLK_DIV_28        0x07
#define ATHR_GMAC_MII_MGMT_CMD               0x24
#define ATHR_GMAC_MGMT_CMD_READ              0x1
#define ATHR_GMAC_MII_MGMT_ADDRESS           0x28
#define ATHR_GMAC_ADDR_SHIFT                 8
#define ATHR_GMAC_MII_MGMT_CTRL              0x2c
#define ATHR_GMAC_MII_MGMT_STATUS            0x30
#define ATHR_GMAC_MII_MGMT_IND               0x34
#define ATHR_GMAC_MGMT_IND_BUSY              (1 << 0)
#define ATHR_GMAC_MGMT_IND_INVALID           (1 << 2)
#define ATHR_GMAC_GE_MAC_ADDR1               0x40
#define ATHR_GMAC_GE_MAC_ADDR2               0x44
#define ATHR_GMAC_MII0_CONTROL               0x18070000

/*
 * MIB Registers
 */
#define ATHR_GMAC_RX_BYTES_CNTR		     0x9c                      /*Rx Byte counter*/
#define ATHR_GMAC_RX_PKT_CNTR		     0xa0		       /*Rx Packet counter*/
#define ATHR_GMAC_RX_CRC_ERR_CNTR	     0xa4                      /*Rx rcv FCR error counter */
#define ATHR_GMAC_RX_MULT_CNTR		     0xa8                      /*Rx rcv Mcast packet counter*/
#define ATHR_GMAC_RX_RBCA_CNTR	             0xac                      /*Rx Broadcast packet counter*/
#define ATHR_GMAC_RX_RXCF_CNTR               0xb0		       /*Rx rcv control frame packet counter */
#define ATHR_GMAC_RX_RXPF_CNTR	             0xb4                      /*Rx rcv pause frame work packet counter*/
#define ATHR_GMAC_RX_RXUO_CNTR	             0xb8                      /*Rx rcv unknown opcode counter*/
#define ATHR_GMAC_RX_FRM_ERR_CNTR	     0xbc                      /*Rx rcv alignment error counter*/
#define ATHR_GMAC_RX_LEN_ERR_CNTR  	     0xc0                      /*Rx Frame length error counter*/
#define ATHR_GMAC_RX_CODE_ERR_CNTR	     0xc4                      /*Rx rcv code error counter */
#define ATHR_GMAC_RX_CRS_ERR_CNTR	     0xc8                      /*Rx rcv carrier sense error counter*/
#define ATHR_GMAC_RX_RUND_CNTR               0xcc                      /*Rx rcv undersize packet counter*/
#define ATHR_GMAC_RX_OVL_ERR_CNTR            0xd0                      /*Rx rcv oversize packet counter*/
#define ATHR_GMAC_RX_RFRG_CNTR		     0xd4		       /*Rx rcv fragments counter */
#define ATHR_GMAC_RX_RJBR_CNTR               0xd8		       /*Rx rcv jabber counter*/
#define ATHR_GMAC_RX_DROP_CNTR		     0xdc                      /*Rx rcv drop*/
#define ATHR_GMAC_TOTAL_COL_CNTR             0x10c
/*
 * Tx counter registers
 */
#define ATHR_GMAC_TX_PKT_CNTR		     0xe4
#define ATHR_GMAC_TX_BYTES_CNTR		     0xe0
#define ATHR_GMAC_TX_DROP_CNTR		     0x114
#define ATHR_GMAC_TX_MULT_CNTR		     0xe8
#define ATHR_GMAC_TX_CRC_ERR_CNTR	     0x11c
#define ATHR_GMAC_TX_BRD_CNTR		     0xEC
#define ATHR_GMAC_TX_PCTRL_CNTR              0xf0			/*Tx pause control frame counter */
#define ATHR_GMAC_TX_DFRL_CNTR               0xf4                       /*TX Deferral packet counter */
#define ATHR_GMAC_TX_TEDF_CNTR		     0x1ec                      /*Tx Excessive deferal packet counter */
#define ATHR_GMAC_TX_TSCL_CNTR		     0xfc			/*Tx Single collision pcket counter */
#define ATHR_GMAC_TX_TMCL_CNTR               0x100                      /*TX Multiple collision packet counter */
#define ATHR_GMAC_TX_TLCL_CNTR               0x104                      /*Tx Late collision packet counter */
#define ATHR_GMAC_TX_TXCL_CNTR 		     0x108			/*Tx excesive collision packet counter */
#define ATHR_GMAC_TX_TNCL_CNTR               0x10c                      /*Tx total collison counter */
#define ATHR_GMAC_TX_TPFH_CNTR               0x110			/*Tx pause frames honoured counter */
#define ATHR_GMAC_TX_TJBR_CNTR		     0x118                      /*Tx jabber frame counter */
#define ATHR_GMAC_TX_TXCF_CNTR               0x120			/*Tx Control frame counter */
#define ATHR_GMAC_TX_TOVR_CNTR		     0x124			/*Tx oversize frame counter */
#define ATHR_GMAC_TX_TUND_CNTR		     0x128 			/*TX under size frame counter */
#define ATHR_GMAC_TX_TFRG_CNTR		     0x12c			/*TX Fragments frame counter */

/*
 * Ethernet config registers
 */
#define ATHR_GMAC_ETH_CFG                    0x18070000
#define ATHR_GMAC_ETH_CFG_RGMII_GE0          (1 << 0)
#define ATHR_GMAC_ETH_CFG_MII_GE0            (1 << 1)
#define ATHR_GMAC_ETH_CFG_RMII_GE0           (1 << 10)
#define ATHR_GMAC_ETH_CFG_RMII_HISPD_GE0     (1 << 11)
#define ATHR_GMAC_ETH_CFG_RMII_MASTER_MODE   (1 << 12)
#define ATHR_GMAC_ETH_CFG_GMII_GE0           (1 << 2)
#define ATHR_GMAC_ETH_CFG_MII_GE0_MASTER     (1 << 3)
#define ATHR_GMAC_ETH_CFG_MII_GE0_SLAVE      (1 << 4)
#define ATHR_GMAC_ETH_CFG_GE0_ERR_EN         (1 << 5)
#define ATHR_GMAC_ETH_CFG_SW_ONLY_MODE       (1 << 6)
#define ATHR_GMAC_ETH_CFG_SW_PHY_SWAP        (1 << 7)
#define ATHR_GMAC_ETH_CFG_SW_PHY_ADDR_SWAP   (1 << 8)
#define ATHR_GMAC_ETH_CFG_SW_APB_ACCESS      (1 << 9)
#define ATHR_GMAC_ETH_CFG_RXD_DELAY	     (1 << 14)
#define ATHR_GMAC_ETH_CFG_RDV_DELAY          (1 << 16)


#define ATH_PCI_EP_BASE_OFF				0x18127000

#define ATH_PCIE_RC_SUPP_L0				(1 << 10)
#define ATH_PCIE_RC_SUPP_L1				(1 << 11)
#define ATH_PCIE_RC_SUPP_VAL				(ATH_PCIE_RC_SUPP_L0 | ATH_PCIE_RC_SUPP_L1)

#define ATH_PCIE_RC_EN_L0				(1 << 0)
#define ATH_PCIE_RC_EN_L1				(1 << 1)
#define ATH_PCIE_RC_EN_VAL				(ATH_PCIE_RC_EN_L0 | ATH_PCIE_RC_EN_L1)

#define ATH_PCIE_EP_EN_L0				(1 << 0)
#define ATH_PCIE_EP_EN_L1				(1 << 1)

#define ATH_PCIE_RC_ASPM_SUPPORT			0x180c007c
#define ATH_PCIE_RC_ASPM_ENABLE				0x180c0080
#define ATH_PCIE_EP_ASPM_ENABLE				0x14000080

#define ATH_PCIE_RC_ASPM_SUPPORT_2			0x1825007c
#define ATH_PCIE_RC_ASPM_ENABLE_2			0x18250080
#define ATH_PCIE_EP_ASPM_ENABLE_2			0x14000080

#if defined(CONFIG_ATH_HAS_PCI_RC2) && !defined(CONFIG_ATH_EMULATION)
	/*
	 * Support RC1 and RC2
	 */
#	define ATH_DECL_PCI_IM_ARR(x)		\
		uint32_t x[] = { PCIE_INT_MASK_ADDRESS, PCIE_INT_MASK_ADDRESS_2 }
#	define ATH_DECL_PCI_IS_ARR(x)		\
		uint32_t x[] = { PCIE_INT_STATUS_ADDRESS, PCIE_INT_STATUS_ADDRESS_2 }
#	define ATH_DECL_PCI_RST_ARR(x)		\
		uint32_t x[] = { PCIE_RESET_ADDRESS, PCIE_RESET_ADDRESS_2 }
#	define ATH_DECL_PCI_ASPM_SUPP_ARR(x)	\
		uint32_t x[] = { ATH_PCIE_RC_ASPM_SUPPORT, ATH_PCIE_RC_ASPM_SUPPORT_2 }
#	define ATH_DECL_PCI_ASPM_EN_ARR(x)	\
		uint32_t x[] = { ATH_PCIE_RC_ASPM_ENABLE, ATH_PCIE_RC_ASPM_ENABLE_2 }
#	define ATH_DECL_PCI_CFG_BASE_ARR(x)	\
		uint32_t x[] = { ATH_PCI_DEV_CFGBASE, ATH_PCI_DEV_CFGBASE_2 }
#	define ATH_DECL_PCI_CRP_ARR(x)		\
		uint32_t x[] = { PCIE_RC_REG_ADDRESS, PCIERC_RC_REG_ADDRESS }
#	define ATH_DECL_PCI_IO_RES		\
		ATH_PCI_RES_IO(0, 0x0000, 0x0000), ATH_PCI_RES_IO(1, 0x0001, 0x0001)
#	define ATH_DECL_PCI_MEM_RES		\
		ATH_PCI_RES_MEM(0), ATH_PCI_RES_MEM(1)
#	define ATH_DECL_PCI_CTRLR		\
		ATH_PCI_CTRL_DESCRIPTOR(0), ATH_PCI_CTRL_DESCRIPTOR(1)
#	define ATH_PCI_RC2_IRQ		(ATH_PCI_IRQ_BASE + 1)
#elif defined(CONFIG_ATH_HAS_PCI_RC2) && defined(CONFIG_ATH_EMULATION)
	/*
	 * Support RC2 alone for emulation testing
	 */
#	define ATH_DECL_PCI_IM_ARR(x)		\
		uint32_t x[] = { PCIE_INT_MASK_ADDRESS_2 }
#	define ATH_DECL_PCI_IS_ARR(x)		\
		uint32_t x[] = { PCIE_INT_STATUS_ADDRESS_2 }
#	define ATH_DECL_PCI_RST_ARR(x)		\
		uint32_t x[] = { PCIE_RESET_ADDRESS_2 }
#	define ATH_DECL_PCI_ASPM_SUPP_ARR(x)	\
		uint32_t x[] = { ATH_PCIE_RC_ASPM_SUPPORT_2 }
#	define ATH_DECL_PCI_ASPM_EN_ARR(x)	\
		uint32_t x[] = { ATH_PCIE_RC_ASPM_ENABLE_2 }
#	define ATH_DECL_PCI_CFG_BASE_ARR(x)	\
		uint32_t x[] = { ATH_PCI_DEV_CFGBASE_2 }
#	define ATH_DECL_PCI_CRP_ARR(x)		\
		uint32_t x[] = { PCIERC_RC_REG_ADDRESS }
#	define ATH_DECL_PCI_IO_RES		\
		ATH_PCI_RES_IO(1, 0x0001, 0x0001)
#	define ATH_DECL_PCI_MEM_RES		\
		ATH_PCI_RES_MEM(1)
#	define ATH_DECL_PCI_CTRLR		\
		ATH_PCI_CTRL_DESCRIPTOR(0)
#	define ATH_PCI_RC2_IRQ		ATH_PCI_IRQ_BASE
#elif !defined(CONFIG_ATH_HAS_PCI_RC2)
	/*
	 * Support RC1 alone (for emulation and chip)
	 */
#	define ATH_DECL_PCI_IM_ARR(x)		\
		uint32_t x[] = { PCIE_INT_MASK_ADDRESS }
#	define ATH_DECL_PCI_IS_ARR(x)		\
		uint32_t x[] = { PCIE_INT_STATUS_ADDRESS }
#	define ATH_DECL_PCI_RST_ARR(x)		\
		uint32_t x[] = { PCIE_RESET_ADDRESS }
#	define ATH_DECL_PCI_ASPM_SUPP_ARR(x)	\
		uint32_t x[] = { ATH_PCIE_RC_ASPM_SUPPORT }
#	define ATH_DECL_PCI_ASPM_EN_ARR(x)	\
		uint32_t x[] = { ATH_PCIE_RC_ASPM_ENABLE }
#	define ATH_DECL_PCI_CFG_BASE_ARR(x)	\
		uint32_t x[] = { ATH_PCI_DEV_CFGBASE }
#	define ATH_DECL_PCI_CRP_ARR(x)		\
		uint32_t x[] = { PCIE_RC_REG_ADDRESS }
#	define ATH_DECL_PCI_IO_RES		\
		ATH_PCI_RES_IO(0, 0x0000, 0x0000)
#	define ATH_DECL_PCI_MEM_RES		\
		ATH_PCI_RES_MEM(0)
#	define ATH_DECL_PCI_CTRLR		\
		ATH_PCI_CTRL_DESCRIPTOR(0)
#	undef ATH_PCI_RC2_IRQ
#endif /* CONFIG_ATH_HAS_PCI_RC2 */

#define ETH_SGMII_SERDES_EN_LOCK_DETECT_MSB                          2
#define ETH_SGMII_SERDES_EN_LOCK_DETECT_LSB                          2
#define ETH_SGMII_SERDES_EN_LOCK_DETECT_MASK                         0x00000004
#define ETH_SGMII_SERDES_EN_LOCK_DETECT_GET(x)                       (((x) & ETH_SGMII_SERDES_EN_LOCK_DETECT_MASK) >> ETH_SGMII_SERDES_EN_LOCK_DETECT_LSB)
#define ETH_SGMII_SERDES_EN_LOCK_DETECT_SET(x)                       (((x) << ETH_SGMII_SERDES_EN_LOCK_DETECT_LSB) & ETH_SGMII_SERDES_EN_LOCK_DETECT_MASK)
#define ETH_SGMII_SERDES_EN_LOCK_DETECT_RESET                        0x0 // 0
#define ETH_SGMII_SERDES_PLL_REFCLK_SEL_MSB                          1
#define ETH_SGMII_SERDES_PLL_REFCLK_SEL_LSB                          1
#define ETH_SGMII_SERDES_PLL_REFCLK_SEL_MASK                         0x00000002
#define ETH_SGMII_SERDES_PLL_REFCLK_SEL_GET(x)                       (((x) & ETH_SGMII_SERDES_PLL_REFCLK_SEL_MASK) >> ETH_SGMII_SERDES_PLL_REFCLK_SEL_LSB)
#define ETH_SGMII_SERDES_PLL_REFCLK_SEL_SET(x)                       (((x) << ETH_SGMII_SERDES_PLL_REFCLK_SEL_LSB) & ETH_SGMII_SERDES_PLL_REFCLK_SEL_MASK)
#define ETH_SGMII_SERDES_PLL_REFCLK_SEL_RESET                        0x0 // 0
#define ETH_SGMII_SERDES_EN_PLL_MSB                                  0
#define ETH_SGMII_SERDES_EN_PLL_LSB                                  0
#define ETH_SGMII_SERDES_EN_PLL_MASK                                 0x00000001
#define ETH_SGMII_SERDES_EN_PLL_GET(x)                               (((x) & ETH_SGMII_SERDES_EN_PLL_MASK) >> ETH_SGMII_SERDES_EN_PLL_LSB)
#define ETH_SGMII_SERDES_EN_PLL_SET(x)                               (((x) << ETH_SGMII_SERDES_EN_PLL_LSB) & ETH_SGMII_SERDES_EN_PLL_MASK)
#define ETH_SGMII_SERDES_EN_PLL_RESET                                0x1 // 1
#define ETH_SGMII_SERDES_ADDRESS                                     0x1805004c

// 32'h002c (ETH_XMII)
#define ETH_XMII_TX_INVERT_MSB                                       31
#define ETH_XMII_TX_INVERT_LSB                                       31
#define ETH_XMII_TX_INVERT_MASK                                      0x80000000
#define ETH_XMII_TX_INVERT_GET(x)                                    (((x) & ETH_XMII_TX_INVERT_MASK) >> ETH_XMII_TX_INVERT_LSB)
#define ETH_XMII_TX_INVERT_SET(x)                                    (((x) << ETH_XMII_TX_INVERT_LSB) & ETH_XMII_TX_INVERT_MASK)
#define ETH_XMII_TX_INVERT_RESET                                     0
#define ETH_XMII_GIGE_QUAD_MSB                                       30
#define ETH_XMII_GIGE_QUAD_LSB                                       30
#define ETH_XMII_GIGE_QUAD_MASK                                      0x40000000
#define ETH_XMII_GIGE_QUAD_GET(x)                                    (((x) & ETH_XMII_GIGE_QUAD_MASK) >> ETH_XMII_GIGE_QUAD_LSB)
#define ETH_XMII_GIGE_QUAD_SET(x)                                    (((x) << ETH_XMII_GIGE_QUAD_LSB) & ETH_XMII_GIGE_QUAD_MASK)
#define ETH_XMII_GIGE_QUAD_RESET                                     0
#define ETH_XMII_RX_DELAY_MSB                                        29
#define ETH_XMII_RX_DELAY_LSB                                        28
#define ETH_XMII_RX_DELAY_MASK                                       0x30000000
#define ETH_XMII_RX_DELAY_GET(x)                                     (((x) & ETH_XMII_RX_DELAY_MASK) >> ETH_XMII_RX_DELAY_LSB)
#define ETH_XMII_RX_DELAY_SET(x)                                     (((x) << ETH_XMII_RX_DELAY_LSB) & ETH_XMII_RX_DELAY_MASK)
#define ETH_XMII_RX_DELAY_RESET                                      0
#define ETH_XMII_TX_DELAY_MSB                                        27
#define ETH_XMII_TX_DELAY_LSB                                        26
#define ETH_XMII_TX_DELAY_MASK                                       0x0c000000
#define ETH_XMII_TX_DELAY_GET(x)                                     (((x) & ETH_XMII_TX_DELAY_MASK) >> ETH_XMII_TX_DELAY_LSB)
#define ETH_XMII_TX_DELAY_SET(x)                                     (((x) << ETH_XMII_TX_DELAY_LSB) & ETH_XMII_TX_DELAY_MASK)
#define ETH_XMII_TX_DELAY_RESET                                      0
#define ETH_XMII_GIGE_MSB                                            25
#define ETH_XMII_GIGE_LSB                                            25
#define ETH_XMII_GIGE_MASK                                           0x02000000
#define ETH_XMII_GIGE_GET(x)                                         (((x) & ETH_XMII_GIGE_MASK) >> ETH_XMII_GIGE_LSB)
#define ETH_XMII_GIGE_SET(x)                                         (((x) << ETH_XMII_GIGE_LSB) & ETH_XMII_GIGE_MASK)
#define ETH_XMII_GIGE_RESET                                          0
#define ETH_XMII_OFFSET_PHASE_MSB                                    24
#define ETH_XMII_OFFSET_PHASE_LSB                                    24
#define ETH_XMII_OFFSET_PHASE_MASK                                   0x01000000
#define ETH_XMII_OFFSET_PHASE_GET(x)                                 (((x) & ETH_XMII_OFFSET_PHASE_MASK) >> ETH_XMII_OFFSET_PHASE_LSB)
#define ETH_XMII_OFFSET_PHASE_SET(x)                                 (((x) << ETH_XMII_OFFSET_PHASE_LSB) & ETH_XMII_OFFSET_PHASE_MASK)
#define ETH_XMII_OFFSET_PHASE_RESET                                  0
#define ETH_XMII_OFFSET_COUNT_MSB                                    23
#define ETH_XMII_OFFSET_COUNT_LSB                                    16
#define ETH_XMII_OFFSET_COUNT_MASK                                   0x00ff0000
#define ETH_XMII_OFFSET_COUNT_GET(x)                                 (((x) & ETH_XMII_OFFSET_COUNT_MASK) >> ETH_XMII_OFFSET_COUNT_LSB)
#define ETH_XMII_OFFSET_COUNT_SET(x)                                 (((x) << ETH_XMII_OFFSET_COUNT_LSB) & ETH_XMII_OFFSET_COUNT_MASK)
#define ETH_XMII_OFFSET_COUNT_RESET                                  0
#define ETH_XMII_PHASE1_COUNT_MSB                                    15
#define ETH_XMII_PHASE1_COUNT_LSB                                    8
#define ETH_XMII_PHASE1_COUNT_MASK                                   0x0000ff00
#define ETH_XMII_PHASE1_COUNT_GET(x)                                 (((x) & ETH_XMII_PHASE1_COUNT_MASK) >> ETH_XMII_PHASE1_COUNT_LSB)
#define ETH_XMII_PHASE1_COUNT_SET(x)                                 (((x) << ETH_XMII_PHASE1_COUNT_LSB) & ETH_XMII_PHASE1_COUNT_MASK)
#define ETH_XMII_PHASE1_COUNT_RESET                                  1
#define ETH_XMII_PHASE0_COUNT_MSB                                    7
#define ETH_XMII_PHASE0_COUNT_LSB                                    0
#define ETH_XMII_PHASE0_COUNT_MASK                                   0x000000ff
#define ETH_XMII_PHASE0_COUNT_GET(x)                                 (((x) & ETH_XMII_PHASE0_COUNT_MASK) >> ETH_XMII_PHASE0_COUNT_LSB)
#define ETH_XMII_PHASE0_COUNT_SET(x)                                 (((x) << ETH_XMII_PHASE0_COUNT_LSB) & ETH_XMII_PHASE0_COUNT_MASK)
#define ETH_XMII_PHASE0_COUNT_RESET                                  1
#define ETH_XMII_ADDRESS                                             0x18050028
// SW modifiable bits
#define ETH_XMII_SW_MASK                                             0xffffffff
// bits defined at reset
#define ETH_XMII_RSTMASK                                             0xffffffff
// reset value (ignore bits undefined at reset)
#define ETH_XMII_RESET                                               0x00000101



#define RST_RESET_ETH_SGMII_ARESET_MSB                               12
#define RST_RESET_ETH_SGMII_ARESET_LSB                               12
#define RST_RESET_ETH_SGMII_ARESET_MASK                              0x00001000
#define RST_RESET_ETH_SGMII_ARESET_GET(x)                            (((x) & RST_RESET_ETH_SGMII_ARESET_MASK) >> RST_RESET_ETH_SGMII_ARESET_LSB)
#define RST_RESET_ETH_SGMII_ARESET_SET(x)                            (((x) << RST_RESET_ETH_SGMII_ARESET_LSB) & RST_RESET_ETH_SGMII_ARESET_MASK)
#define RST_RESET_ETH_SGMII_ARESET_RESET                             0x1 // 1


#define RST_RESET_ETH_SGMII_RESET_MSB                                8
#define RST_RESET_ETH_SGMII_RESET_LSB                                8
#define RST_RESET_ETH_SGMII_RESET_MASK                               0x00000100
#define RST_RESET_ETH_SGMII_RESET_GET(x)                             (((x) & RST_RESET_ETH_SGMII_RESET_MASK) >> RST_RESET_ETH_SGMII_RESET_LSB)
#define RST_RESET_ETH_SGMII_RESET_SET(x)                             (((x) << RST_RESET_ETH_SGMII_RESET_LSB) & RST_RESET_ETH_SGMII_RESET_MASK)
#define RST_RESET_ETH_SGMII_RESET_RESET                              0x1 // 1

/* END */

// 32'h1804004c (GPIO_IN_ENABLE2)
#define GPIO_IN_ENABLE2_SLICEXT_MCLK_MSB                             31
#define GPIO_IN_ENABLE2_SLICEXT_MCLK_LSB                             24
#define GPIO_IN_ENABLE2_SLICEXT_MCLK_MASK                            0xff000000
#define GPIO_IN_ENABLE2_SLICEXT_MCLK_GET(x)                          (((x) & GPIO_IN_ENABLE2_SLICEXT_MCLK_MASK) >> GPIO_IN_ENABLE2_SLICEXT_MCLK_LSB)
#define GPIO_IN_ENABLE2_SLICEXT_MCLK_SET(x)                          (((x) << GPIO_IN_ENABLE2_SLICEXT_MCLK_LSB) & GPIO_IN_ENABLE2_SLICEXT_MCLK_MASK)
#define GPIO_IN_ENABLE2_SLICEXT_MCLK_RESET                           0x80 // 128
#define GPIO_IN_ENABLE2_ETH__RX_CRS_MSB                              23
#define GPIO_IN_ENABLE2_ETH__RX_CRS_LSB                              16
#define GPIO_IN_ENABLE2_ETH__RX_CRS_MASK                             0x00ff0000
#define GPIO_IN_ENABLE2_ETH__RX_CRS_GET(x)                           (((x) & GPIO_IN_ENABLE2_ETH__RX_CRS_MASK) >> GPIO_IN_ENABLE2_ETH__RX_CRS_LSB)
#define GPIO_IN_ENABLE2_ETH__RX_CRS_SET(x)                           (((x) << GPIO_IN_ENABLE2_ETH__RX_CRS_LSB) & GPIO_IN_ENABLE2_ETH__RX_CRS_MASK)
#define GPIO_IN_ENABLE2_ETH__RX_CRS_RESET                            0x80 // 128
#define GPIO_IN_ENABLE2_ETH__RX_COL_MSB                              15
#define GPIO_IN_ENABLE2_ETH__RX_COL_LSB                              8
#define GPIO_IN_ENABLE2_ETH__RX_COL_MASK                             0x0000ff00
#define GPIO_IN_ENABLE2_ETH__RX_COL_GET(x)                           (((x) & GPIO_IN_ENABLE2_ETH__RX_COL_MASK) >> GPIO_IN_ENABLE2_ETH__RX_COL_LSB)
#define GPIO_IN_ENABLE2_ETH__RX_COL_SET(x)                           (((x) << GPIO_IN_ENABLE2_ETH__RX_COL_LSB) & GPIO_IN_ENABLE2_ETH__RX_COL_MASK)
#define GPIO_IN_ENABLE2_ETH__RX_COL_RESET                            0x80 // 128
#define GPIO_IN_ENABLE2_ETH__RX_ERR_MSB                              7
#define GPIO_IN_ENABLE2_ETH__RX_ERR_LSB                              0
#define GPIO_IN_ENABLE2_ETH__RX_ERR_MASK                             0x000000ff
#define GPIO_IN_ENABLE2_ETH__RX_ERR_GET(x)                           (((x) & GPIO_IN_ENABLE2_ETH__RX_ERR_MASK) >> GPIO_IN_ENABLE2_ETH__RX_ERR_LSB)
#define GPIO_IN_ENABLE2_ETH__RX_ERR_SET(x)                           (((x) << GPIO_IN_ENABLE2_ETH__RX_ERR_LSB) & GPIO_IN_ENABLE2_ETH__RX_ERR_MASK)
#define GPIO_IN_ENABLE2_ETH__RX_ERR_RESET                            0x80 // 128
#define GPIO_IN_ENABLE2_ADDRESS                                      0x1804004c
#define GPIO_IN_ENABLE2_OFFSET                                       0x004c

#define OTP_INTF2_ADDRESS                                            0x18131008
#define OTP_LDO_CONTROL_ADDRESS                                      0x18131024

#define OTP_LDO_STATUS_POWER_ON_MSB                                  0
#define OTP_LDO_STATUS_POWER_ON_LSB                                  0
#define OTP_LDO_STATUS_POWER_ON_MASK                                 0x00000001
#define OTP_LDO_STATUS_POWER_ON_GET(x)                               (((x) & OTP_LDO_STATUS_POWER_ON_MASK) >> OTP_LDO_STATUS_POWER_ON_LSB)
#define OTP_LDO_STATUS_POWER_ON_SET(x)                               (((x) << OTP_LDO_STATUS_POWER_ON_LSB) & OTP_LDO_STATUS_POWER_ON_MASK)
#define OTP_LDO_STATUS_POWER_ON_RESET                                0x0 // 0
#define OTP_LDO_STATUS_ADDRESS                                       0x1813102c

#define OTP_MEM_0_ADDRESS                                            0x18130000

#define OTP_STATUS0_EFUSE_READ_DATA_VALID_MSB                        2
#define OTP_STATUS0_EFUSE_READ_DATA_VALID_LSB                        2
#define OTP_STATUS0_EFUSE_READ_DATA_VALID_MASK                       0x00000004
#define OTP_STATUS0_EFUSE_READ_DATA_VALID_GET(x)                     (((x) & OTP_STATUS0_EFUSE_READ_DATA_VALID_MASK) >> OTP_STATUS0_EFUSE_READ_DATA_VALID_LSB)
#define OTP_STATUS0_EFUSE_READ_DATA_VALID_SET(x)                     (((x) << OTP_STATUS0_EFUSE_READ_DATA_VALID_LSB) & OTP_STATUS0_EFUSE_READ_DATA_VALID_MASK)
#define OTP_STATUS0_EFUSE_READ_DATA_VALID_RESET                      0x0 // 0
#define OTP_STATUS0_EFUSE_ACCESS_BUSY_MSB                            1
#define OTP_STATUS0_EFUSE_ACCESS_BUSY_LSB                            1
#define OTP_STATUS0_EFUSE_ACCESS_BUSY_MASK                           0x00000002
#define OTP_STATUS0_EFUSE_ACCESS_BUSY_GET(x)                         (((x) & OTP_STATUS0_EFUSE_ACCESS_BUSY_MASK) >> OTP_STATUS0_EFUSE_ACCESS_BUSY_LSB)
#define OTP_STATUS0_EFUSE_ACCESS_BUSY_SET(x)                         (((x) << OTP_STATUS0_EFUSE_ACCESS_BUSY_LSB) & OTP_STATUS0_EFUSE_ACCESS_BUSY_MASK)
#define OTP_STATUS0_EFUSE_ACCESS_BUSY_RESET                          0x0 // 0
#define OTP_STATUS0_OTP_SM_BUSY_MSB                                  0
#define OTP_STATUS0_OTP_SM_BUSY_LSB                                  0
#define OTP_STATUS0_OTP_SM_BUSY_MASK                                 0x00000001
#define OTP_STATUS0_OTP_SM_BUSY_GET(x)                               (((x) & OTP_STATUS0_OTP_SM_BUSY_MASK) >> OTP_STATUS0_OTP_SM_BUSY_LSB)
#define OTP_STATUS0_OTP_SM_BUSY_SET(x)                               (((x) << OTP_STATUS0_OTP_SM_BUSY_LSB) & OTP_STATUS0_OTP_SM_BUSY_MASK)
#define OTP_STATUS0_OTP_SM_BUSY_RESET                                0x0 // 0
#define OTP_STATUS0_ADDRESS                                          0x18131018

#define OTP_STATUS1_ADDRESS                                          0x1813101c

#define SWITCH_CLOCK_SPARE_SPARE_MSB                                 31
#define SWITCH_CLOCK_SPARE_SPARE_LSB                                 16
#define SWITCH_CLOCK_SPARE_SPARE_MASK                                0xffff0000
#define SWITCH_CLOCK_SPARE_SPARE_GET(x)                              (((x) & SWITCH_CLOCK_SPARE_SPARE_MASK) >> SWITCH_CLOCK_SPARE_SPARE_LSB)
#define SWITCH_CLOCK_SPARE_SPARE_SET(x)                              (((x) << SWITCH_CLOCK_SPARE_SPARE_LSB) & SWITCH_CLOCK_SPARE_SPARE_MASK)
#define SWITCH_CLOCK_SPARE_SPARE_RESET                               0x0 // 0
#define SWITCH_CLOCK_SPARE_MDIO_CLK_SEL1_2_MSB                       15
#define SWITCH_CLOCK_SPARE_MDIO_CLK_SEL1_2_LSB                       15
#define SWITCH_CLOCK_SPARE_MDIO_CLK_SEL1_2_MASK                      0x00008000
#define SWITCH_CLOCK_SPARE_MDIO_CLK_SEL1_2_GET(x)                    (((x) & SWITCH_CLOCK_SPARE_MDIO_CLK_SEL1_2_MASK) >> SWITCH_CLOCK_SPARE_MDIO_CLK_SEL1_2_LSB)
#define SWITCH_CLOCK_SPARE_MDIO_CLK_SEL1_2_SET(x)                    (((x) << SWITCH_CLOCK_SPARE_MDIO_CLK_SEL1_2_LSB) & SWITCH_CLOCK_SPARE_MDIO_CLK_SEL1_2_MASK)
#define SWITCH_CLOCK_SPARE_MDIO_CLK_SEL1_2_RESET                     0x0 // 0
#define SWITCH_CLOCK_SPARE_MDIO_CLK_SEL1_1_MSB                       14
#define SWITCH_CLOCK_SPARE_MDIO_CLK_SEL1_1_LSB                       14
#define SWITCH_CLOCK_SPARE_MDIO_CLK_SEL1_1_MASK                      0x00004000
#define SWITCH_CLOCK_SPARE_MDIO_CLK_SEL1_1_GET(x)                    (((x) & SWITCH_CLOCK_SPARE_MDIO_CLK_SEL1_1_MASK) >> SWITCH_CLOCK_SPARE_MDIO_CLK_SEL1_1_LSB)
#define SWITCH_CLOCK_SPARE_MDIO_CLK_SEL1_1_SET(x)                    (((x) << SWITCH_CLOCK_SPARE_MDIO_CLK_SEL1_1_LSB) & SWITCH_CLOCK_SPARE_MDIO_CLK_SEL1_1_MASK)
#define SWITCH_CLOCK_SPARE_MDIO_CLK_SEL1_1_RESET                     0x0 // 0
#define SWITCH_CLOCK_SPARE_MDIO_CLK_SEL0_2_MSB                       13
#define SWITCH_CLOCK_SPARE_MDIO_CLK_SEL0_2_LSB                       13
#define SWITCH_CLOCK_SPARE_MDIO_CLK_SEL0_2_MASK                      0x00002000
#define SWITCH_CLOCK_SPARE_MDIO_CLK_SEL0_2_GET(x)                    (((x) & SWITCH_CLOCK_SPARE_MDIO_CLK_SEL0_2_MASK) >> SWITCH_CLOCK_SPARE_MDIO_CLK_SEL0_2_LSB)
#define SWITCH_CLOCK_SPARE_MDIO_CLK_SEL0_2_SET(x)                    (((x) << SWITCH_CLOCK_SPARE_MDIO_CLK_SEL0_2_LSB) & SWITCH_CLOCK_SPARE_MDIO_CLK_SEL0_2_MASK)
#define SWITCH_CLOCK_SPARE_MDIO_CLK_SEL0_2_RESET                     0x0 // 0
#define SWITCH_CLOCK_SPARE_NANDF_CLK_SEL_MSB                         12
#define SWITCH_CLOCK_SPARE_NANDF_CLK_SEL_LSB                         12
#define SWITCH_CLOCK_SPARE_NANDF_CLK_SEL_MASK                        0x00001000
#define SWITCH_CLOCK_SPARE_NANDF_CLK_SEL_GET(x)                      (((x) & SWITCH_CLOCK_SPARE_NANDF_CLK_SEL_MASK) >> SWITCH_CLOCK_SPARE_NANDF_CLK_SEL_LSB)
#define SWITCH_CLOCK_SPARE_NANDF_CLK_SEL_SET(x)                      (((x) << SWITCH_CLOCK_SPARE_NANDF_CLK_SEL_LSB) & SWITCH_CLOCK_SPARE_NANDF_CLK_SEL_MASK)
#define SWITCH_CLOCK_SPARE_NANDF_CLK_SEL_RESET                       0x0 // 0
#define SWITCH_CLOCK_SPARE_USB_REFCLK_FREQ_SEL_MSB                   11
#define SWITCH_CLOCK_SPARE_USB_REFCLK_FREQ_SEL_LSB                   8
#define SWITCH_CLOCK_SPARE_USB_REFCLK_FREQ_SEL_MASK                  0x00000f00
#define SWITCH_CLOCK_SPARE_USB_REFCLK_FREQ_SEL_GET(x)                (((x) & SWITCH_CLOCK_SPARE_USB_REFCLK_FREQ_SEL_MASK) >> SWITCH_CLOCK_SPARE_USB_REFCLK_FREQ_SEL_LSB)
#define SWITCH_CLOCK_SPARE_USB_REFCLK_FREQ_SEL_SET(x)                (((x) << SWITCH_CLOCK_SPARE_USB_REFCLK_FREQ_SEL_LSB) & SWITCH_CLOCK_SPARE_USB_REFCLK_FREQ_SEL_MASK)
#define SWITCH_CLOCK_SPARE_USB_REFCLK_FREQ_SEL_RESET                 0x5 // 5
#define SWITCH_CLOCK_SPARE_UART1_CLK_SEL_MSB                         7
#define SWITCH_CLOCK_SPARE_UART1_CLK_SEL_LSB                         7
#define SWITCH_CLOCK_SPARE_UART1_CLK_SEL_MASK                        0x00000080
#define SWITCH_CLOCK_SPARE_UART1_CLK_SEL_GET(x)                      (((x) & SWITCH_CLOCK_SPARE_UART1_CLK_SEL_MASK) >> SWITCH_CLOCK_SPARE_UART1_CLK_SEL_LSB)
#define SWITCH_CLOCK_SPARE_UART1_CLK_SEL_SET(x)                      (((x) << SWITCH_CLOCK_SPARE_UART1_CLK_SEL_LSB) & SWITCH_CLOCK_SPARE_UART1_CLK_SEL_MASK)
#define SWITCH_CLOCK_SPARE_UART1_CLK_SEL_RESET                       0x0 // 0
#define SWITCH_CLOCK_SPARE_MDIO_CLK_SEL0_1_MSB                       6
#define SWITCH_CLOCK_SPARE_MDIO_CLK_SEL0_1_LSB                       6
#define SWITCH_CLOCK_SPARE_MDIO_CLK_SEL0_1_MASK                      0x00000040
#define SWITCH_CLOCK_SPARE_MDIO_CLK_SEL0_1_GET(x)                    (((x) & SWITCH_CLOCK_SPARE_MDIO_CLK_SEL0_1_MASK) >> SWITCH_CLOCK_SPARE_MDIO_CLK_SEL0_1_LSB)
#define SWITCH_CLOCK_SPARE_MDIO_CLK_SEL0_1_SET(x)                    (((x) << SWITCH_CLOCK_SPARE_MDIO_CLK_SEL0_1_LSB) & SWITCH_CLOCK_SPARE_MDIO_CLK_SEL0_1_MASK)
#define SWITCH_CLOCK_SPARE_MDIO_CLK_SEL0_1_RESET                     0x0 // 0
#define SWITCH_CLOCK_SPARE_I2C_CLK_SEL_MSB                           5
#define SWITCH_CLOCK_SPARE_I2C_CLK_SEL_LSB                           5
#define SWITCH_CLOCK_SPARE_I2C_CLK_SEL_MASK                          0x00000020
#define SWITCH_CLOCK_SPARE_I2C_CLK_SEL_GET(x)                        (((x) & SWITCH_CLOCK_SPARE_I2C_CLK_SEL_MASK) >> SWITCH_CLOCK_SPARE_I2C_CLK_SEL_LSB)
#define SWITCH_CLOCK_SPARE_I2C_CLK_SEL_SET(x)                        (((x) << SWITCH_CLOCK_SPARE_I2C_CLK_SEL_LSB) & SWITCH_CLOCK_SPARE_I2C_CLK_SEL_MASK)
#define SWITCH_CLOCK_SPARE_I2C_CLK_SEL_RESET                         0x0 // 0
#define SWITCH_CLOCK_SPARE_SPARE_0_MSB                               4
#define SWITCH_CLOCK_SPARE_SPARE_0_LSB                               0
#define SWITCH_CLOCK_SPARE_SPARE_0_MASK                              0x0000001f
#define SWITCH_CLOCK_SPARE_SPARE_0_GET(x)                            (((x) & SWITCH_CLOCK_SPARE_SPARE_0_MASK) >> SWITCH_CLOCK_SPARE_SPARE_0_LSB)
#define SWITCH_CLOCK_SPARE_SPARE_0_SET(x)                            (((x) << SWITCH_CLOCK_SPARE_SPARE_0_LSB) & SWITCH_CLOCK_SPARE_SPARE_0_MASK)
#define SWITCH_CLOCK_SPARE_SPARE_0_RESET                             0x0 // 0
#define SWITCH_CLOCK_SPARE_ADDRESS                                   0x18050020

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

#define RST_RESET_GE1_MAC_RESET_MSB                                  13
#define RST_RESET_GE1_MAC_RESET_LSB                                  13
#define RST_RESET_GE1_MAC_RESET_MASK                                 0x00002000
#define RST_RESET_GE1_MAC_RESET_GET(x)                               (((x) & RST_RESET_GE1_MAC_RESET_MASK) >> RST_RESET_GE1_MAC_RESET_LSB)
#define RST_RESET_GE1_MAC_RESET_SET(x)                               (((x) << RST_RESET_GE1_MAC_RESET_LSB) & RST_RESET_GE1_MAC_RESET_MASK)
#define RST_RESET_GE1_MAC_RESET_RESET                                0x1 // 1
#define RST_RESET_ETH_SGMII_ARESET_MSB                               12
#define RST_RESET_ETH_SGMII_ARESET_LSB                               12
#define RST_RESET_ETH_SGMII_ARESET_MASK                              0x00001000
#define RST_RESET_ETH_SGMII_ARESET_GET(x)                            (((x) & RST_RESET_ETH_SGMII_ARESET_MASK) >> RST_RESET_ETH_SGMII_ARESET_LSB)
#define RST_RESET_ETH_SGMII_ARESET_SET(x)                            (((x) << RST_RESET_ETH_SGMII_ARESET_LSB) & RST_RESET_ETH_SGMII_ARESET_MASK)
#define RST_RESET_ETH_SGMII_ARESET_RESET                             0x1 // 1

#define RST_RESET_ADDRESS                                            0x1806001c

#define RST_RESET_GE0_MAC_RESET_MSB                                  9
#define RST_RESET_GE0_MAC_RESET_LSB                                  9
#define RST_RESET_GE0_MAC_RESET_MASK                                 0x00000200
#define RST_RESET_GE0_MAC_RESET_GET(x)                               (((x) & RST_RESET_GE0_MAC_RESET_MASK) >> RST_RESET_GE0_MAC_RESET_LSB)
#define RST_RESET_GE0_MAC_RESET_SET(x)                               (((x) << RST_RESET_GE0_MAC_RESET_LSB) & RST_RESET_GE0_MAC_RESET_MASK)
#define RST_RESET_GE0_MAC_RESET_RESET                                0x1 // 1

#endif /* _955x_H */
