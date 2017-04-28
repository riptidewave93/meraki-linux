/*
 * sata_apm.h - AppliedMicro SATA PHY definitions and inline function
 *
 * Copyright (c) 2010, Applied Micro Circuits Corporation
 * Author: Ravi Patel <rapatel@apm.com>
 *         Sourabh Daptardar <sdaptardar@apm.com> 
 *         Tanmay Inamdar <tinamdar@apm.com>
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

#ifndef _SATA_APM_H_
#define _SATA_APM_H_

#include <linux/of_platform.h>

#if defined(CONFIG_APM8018X)
#include <asm/dcr.h>
#include <asm/dcr-regs.h>

/* Phy specific registers */
#define SDR0_SATAPHY_CFG	0x80
#define SDR0_SATAPHY_CLK	0x81
#define SDR0_SATAPHY_PD		0x82
#define SDR0_SATAPHY_DRV	0x83
#define SDR0_SATAPHY_REC	0x84
#define SDR0_SATAPHY_BURST	0x85
#define SDR0_SATAPHY_COMINIT	0x86
#define SDR0_SATAPHY_COMWAKE	0x87
#define SDR0_SATAPHY_BIST	0x88
#define SDR0_SATAPHY_OBS	0x89

#define RESET_PHY		0x00000001
#define X_DIAG_RDY		0x04000000

#define PD_PORSTN_DELAY 30  /* should be atleast 20 usec as per APM 8018X UM */
#define XDIAG_DELAY 30

#ifdef ATA_DEBUG
#define ahci_stringify(x) #x
#define ahci_tostring(x) ahci_stringify(x)
#define ahci_location \
	KERN_ALERT"SDD: " __FILE__  " : %s : "  ahci_tostring(__LINE__) " :: "
#define ahci_printk(format,...) \
	printk(ahci_location format, __func__, __VA_ARGS__)

/* 
 * Dumps AHCI registers any PHY registers
 * in APM Klondike (DWC_AHSATA) SATA controller
 */
void apm_8018x_rdump(void __iomem *base);
#endif

#elif defined(CONFIG_APM86xxx)
#define PCSCSR_CTL0_ADDR	0x00004000
#define PCSCSR_CTL1_ADDR	0x00004004
#define PCSCSR_CTL2_ADDR	0x0000402c
#define PCSCSR_STATUS1_ADDR	0x00004008
#define PCSCSR_REGADDR_ADDR	0x0000400c
#define PCSCSR_REGWRDATA_ADDR	0x00004010
#define PCSCSR_REGCMD_ADDR	0x00004014
#define PCSCSR_REGCMDDONE_ADDR	0x00004018
#define PCSCSR_REGRDDATA_ADDR	0x0000401c
#define SATA_IOCOH_START0_ADDR	0x00004034
#define SATA_IOCOH_START1_ADDR	0x00004038
#define SATA_IOCOH_END0_ADDR	0x0000403c
#define SATA_IOCOH_END1_ADDR	0x00004040

#define SERDES_PCS_PRBS_TIMER_0_ADDR	0x00008004
#define SERDES_PCS_SATA_RX_LEN_1_ADDR	0x000080E7
#define SERDES_PCS_SATA_CFG_0_ADDR	0x000080EA

#define SERDES_PCS_PRBS_TIMER_0_DATA	0x0470
#define SERDES_PCS_SATA_RX_LEN_1_DATA	0x2933
#define SERDES_PCS_SATA_CFG_0_DATA	0x2FF9

/* Fields mgmt_pcs_reg_rd */
#define MGMT_PCS_REG_RD_MASK		0x00000001
#define MGMT_PCS_REG_RD_DONE_MASK	0x00000001
/* Fields mgmt_pcs_reg_wr */
#define MGMT_PCS_REG_WR_MASK		0x00000002
#define MGMT_PCS_REG_WR_DONE_MASK	0x00000002
/* Fields sds_pcs_pll_lock */
#define SDS_PCS_PLL_LOCK1_MASK		0x00000400

/*      Register PcsCsr_Ctl0    */
/*       Fields phy_pcs_rx0_sigdet       */
#define PHY_PCS_RX0_SIGDET0_WIDTH                                             1
#define PHY_PCS_RX0_SIGDET0_SHIFT                                            29
#define PHY_PCS_RX0_SIGDET0_MASK                                     0x20000000
#define PHY_PCS_RX0_SIGDET0_RD(src)                  (((src) & 0x20000000)>>29)
#define PHY_PCS_RX0_SIGDET0_WR(src)             (((u32)(src)<<29) & 0x20000000)
/*       Fields phy_tx_term_cal_in       */
#define PHY_TX_TERM_CAL_IN0_WIDTH                                             4
#define PHY_TX_TERM_CAL_IN0_SHIFT                                            25
#define PHY_TX_TERM_CAL_IN0_MASK                                     0x1e000000
#define PHY_TX_TERM_CAL_IN0_RD(src)                  (((src) & 0x1e000000)>>25)
#define PHY_TX_TERM_CAL_IN0_WR(src)             (((u32)(src)<<25) & 0x1e000000)
/*       Fields mac_pcs_x2_mode  */
#define MAC_PCS_X2_MODE0_WIDTH                                                1
#define MAC_PCS_X2_MODE0_SHIFT                                               24
#define MAC_PCS_X2_MODE0_MASK                                        0x01000000
#define MAC_PCS_X2_MODE0_RD(src)                     (((src) & 0x01000000)>>24)
#define MAC_PCS_X2_MODE0_WR(src)                (((u32)(src)<<24) & 0x01000000)
/*       Fields mac_pcs_pll_pcs_div      */
#define MAC_PCS_PLL_PCS_DIV0_WIDTH                                            6
#define MAC_PCS_PLL_PCS_DIV0_SHIFT                                           18
#define MAC_PCS_PLL_PCS_DIV0_MASK                                    0x00fc0000
#define MAC_PCS_PLL_PCS_DIV0_RD(src)                 (((src) & 0x00fc0000)>>18)
#define MAC_PCS_PLL_PCS_DIV0_WR(src)            (((u32)(src)<<18) & 0x00fc0000)
/*       Fields mac_pcs_pll_div  */
#define MAC_PCS_PLL_DIV0_WIDTH                                                6
#define MAC_PCS_PLL_DIV0_SHIFT                                               12
#define MAC_PCS_PLL_DIV0_MASK                                        0x0003f000
#define MAC_PCS_PLL_DIV0_RD(src)                     (((src) & 0x0003f000)>>12)
#define MAC_PCS_PLL_DIV0_WR(src)                (((u32)(src)<<12) & 0x0003f000)
/*       Fields mac_pcs_pll_op_range     */
#define MAC_PCS_PLL_OP_RANGE0_WIDTH                                           3
#define MAC_PCS_PLL_OP_RANGE0_SHIFT                                           9
#define MAC_PCS_PLL_OP_RANGE0_MASK                                   0x00000e00
#define MAC_PCS_PLL_OP_RANGE0_RD(src)                 (((src) & 0x00000e00)>>9)
#define MAC_PCS_PLL_OP_RANGE0_WR(src)            (((u32)(src)<<9) & 0x00000e00)
/*       Fields mac_pcs_pll_refclk_sel   */
#define MAC_PCS_PLL_REFCLK_SEL0_WIDTH                                         3
#define MAC_PCS_PLL_REFCLK_SEL0_SHIFT                                         6
#define MAC_PCS_PLL_REFCLK_SEL0_MASK                                 0x000001c0
#define MAC_PCS_PLL_REFCLK_SEL0_RD(src)               (((src) & 0x000001c0)>>6)
#define MAC_PCS_PLL_REFCLK_SEL0_WR(src)          (((u32)(src)<<6) & 0x000001c0)
/*       Fields reset    */
#define RESET0_F2_WIDTH                                                       1
#define RESET0_F2_SHIFT                                                       5
#define RESET0_F2_MASK                                               0x00000020
#define RESET0_F2_RD(src)                             (((src) & 0x00000020)>>5)
#define RESET0_F2_WR(src)                        (((u32)(src)<<5) & 0x00000020)
/*       Fields init_ctlifc_use_mgmt     */
#define INIT_CTLIFC_USE_MGMT0_WIDTH                                           1
#define INIT_CTLIFC_USE_MGMT0_SHIFT                                           4
#define INIT_CTLIFC_USE_MGMT0_MASK                                   0x00000010
#define INIT_CTLIFC_USE_MGMT0_RD(src)                 (((src) & 0x00000010)>>4)
#define INIT_CTLIFC_USE_MGMT0_WR(src)            (((u32)(src)<<4) & 0x00000010)
/*       Fields init_ctlifc_pipe_compliant       */
#define INIT_CTLIFC_PIPE_COMPLIANT0_WIDTH                                     1
#define INIT_CTLIFC_PIPE_COMPLIANT0_SHIFT                                     3
#define INIT_CTLIFC_PIPE_COMPLIANT0_MASK                             0x00000008
#define INIT_CTLIFC_PIPE_COMPLIANT0_RD(src)           (((src) & 0x00000008)>>3)
#define INIT_CTLIFC_PIPE_COMPLIANT0_WR(src)      (((u32)(src)<<3) & 0x00000008)
/*       Fields init_refclk_ac_mode      */
#define INIT_REFCLK_AC_MODE0_WIDTH                                            1
#define INIT_REFCLK_AC_MODE0_SHIFT                                            2
#define INIT_REFCLK_AC_MODE0_MASK                                    0x00000004
#define INIT_REFCLK_AC_MODE0_RD(src)                  (((src) & 0x00000004)>>2)
#define INIT_REFCLK_AC_MODE0_WR(src)             (((u32)(src)<<2) & 0x00000004)
/*       Fields init_tx_term_sel         */
#define INIT_TX_TERM_SEL0_WIDTH                                               1
#define INIT_TX_TERM_SEL0_SHIFT                                               1
#define INIT_TX_TERM_SEL0_MASK                                       0x00000002
#define INIT_TX_TERM_SEL0_RD(src)                     (((src) & 0x00000002)>>1)
#define INIT_TX_TERM_SEL0_WR(src)                (((u32)(src)<<1) & 0x00000002)
/*       Fields pcie_mode_sel    */
#define PCIE_MODE_SEL0_WIDTH                                                  1
#define PCIE_MODE_SEL0_SHIFT                                                  0
#define PCIE_MODE_SEL0_MASK                                          0x00000001
#define PCIE_MODE_SEL0_RD(src)                           (((src) & 0x00000001))
#define PCIE_MODE_SEL0_WR(src)                      (((u32)(src)) & 0x00000001)

/*      Register PcsCsr_Ctl1    */
/*       Fields mac_pcs_rx0_encdet       */
#define MAC_PCS_RX0_ENCDET1_WIDTH                                             1
#define MAC_PCS_RX0_ENCDET1_SHIFT                                            24
#define MAC_PCS_RX0_ENCDET1_MASK                                     0x01000000
#define MAC_PCS_RX0_ENCDET1_RD(src)                  (((src) & 0x01000000)>>24)
#define MAC_PCS_RX0_ENCDET1_WR(src)             (((u32)(src)<<24) & 0x01000000)
/*       Fields mac_pcs_sata0_host       */
#define MAC_PCS_SATA0_HOST1_WIDTH                                             1
#define MAC_PCS_SATA0_HOST1_SHIFT                                            23
#define MAC_PCS_SATA0_HOST1_MASK                                     0x00800000
#define MAC_PCS_SATA0_HOST1_RD(src)                  (((src) & 0x00800000)>>23)
#define MAC_PCS_SATA0_HOST1_WR(src)             (((u32)(src)<<23) & 0x00800000)
/*       Fields mac_pcs_rx0_edet_lb      */
#define MAC_PCS_RX0_EDET_LB1_WIDTH                                            1
#define MAC_PCS_RX0_EDET_LB1_SHIFT                                           22
#define MAC_PCS_RX0_EDET_LB1_MASK                                    0x00400000
#define MAC_PCS_RX0_EDET_LB1_RD(src)                 (((src) & 0x00400000)>>22)
#define MAC_PCS_RX0_EDET_LB1_WR(src)            (((u32)(src)<<22) & 0x00400000)
/*       Fields mac_pcs_rx0_edet_eie     */
#define MAC_PCS_RX0_EDET_EIE1_WIDTH                                           1
#define MAC_PCS_RX0_EDET_EIE1_SHIFT                                          21
#define MAC_PCS_RX0_EDET_EIE1_MASK                                   0x00200000
#define MAC_PCS_RX0_EDET_EIE1_RD(src)                (((src) & 0x00200000)>>21)
#define MAC_PCS_RX0_EDET_EIE1_WR(src)           (((u32)(src)<<21) & 0x00200000)
/*       Fields mac_pcs_rx0_edet_ts      */
#define MAC_PCS_RX0_EDET_TS1_WIDTH                                            1
#define MAC_PCS_RX0_EDET_TS1_SHIFT                                           20
#define MAC_PCS_RX0_EDET_TS1_MASK                                    0x00100000
#define MAC_PCS_RX0_EDET_TS1_RD(src)                 (((src) & 0x00100000)>>20)
#define MAC_PCS_RX0_EDET_TS1_WR(src)            (((u32)(src)<<20) & 0x00100000)
/*       Fields mac_pcs_rx0_edet_skp     */
#define MAC_PCS_RX0_EDET_SKP1_WIDTH                                           1
#define MAC_PCS_RX0_EDET_SKP1_SHIFT                                          19
#define MAC_PCS_RX0_EDET_SKP1_MASK                                   0x00080000
#define MAC_PCS_RX0_EDET_SKP1_RD(src)                (((src) & 0x00080000)>>19)
#define MAC_PCS_RX0_EDET_SKP1_WR(src)           (((u32)(src)<<19) & 0x00080000)
/*       Fields mac_pcs_ctl0_op_range    */
#define MAC_PCS_CTL0_OP_RANGE1_WIDTH                                          3
#define MAC_PCS_CTL0_OP_RANGE1_SHIFT                                         16
#define MAC_PCS_CTL0_OP_RANGE1_MASK                                  0x00070000
#define MAC_PCS_CTL0_OP_RANGE1_RD(src)               (((src) & 0x00070000)>>16)
#define MAC_PCS_CTL0_OP_RANGE1_WR(src)          (((u32)(src)<<16) & 0x00070000)
/*       Fields mac_pcs_ctl0_smpl_rate   */
#define MAC_PCS_CTL0_SMPL_RATE1_WIDTH                                         3
#define MAC_PCS_CTL0_SMPL_RATE1_SHIFT                                        13
#define MAC_PCS_CTL0_SMPL_RATE1_MASK                                 0x0000e000
#define MAC_PCS_CTL0_SMPL_RATE1_RD(src)              (((src) & 0x0000e000)>>13)
#define MAC_PCS_CTL0_SMPL_RATE1_WR(src)         (((u32)(src)<<13) & 0x0000e000)
/*       Fields mac_pcs_ctl0_rx_lane_div         */
#define MAC_PCS_CTL0_RX_LANE_DIV1_WIDTH                                       2
#define MAC_PCS_CTL0_RX_LANE_DIV1_SHIFT                                      11
#define MAC_PCS_CTL0_RX_LANE_DIV1_MASK                               0x00001800
#define MAC_PCS_CTL0_RX_LANE_DIV1_RD(src)            (((src) & 0x00001800)>>11)
#define MAC_PCS_CTL0_RX_LANE_DIV1_WR(src)       (((u32)(src)<<11) & 0x00001800)
/*       Fields mac_pcs_ctl0_tx_lane_div         */
#define MAC_PCS_CTL0_TX_LANE_DIV1_WIDTH                                       2
#define MAC_PCS_CTL0_TX_LANE_DIV1_SHIFT                                       9
#define MAC_PCS_CTL0_TX_LANE_DIV1_MASK                               0x00000600
#define MAC_PCS_CTL0_TX_LANE_DIV1_RD(src)             (((src) & 0x00000600)>>9)
#define MAC_PCS_CTL0_TX_LANE_DIV1_WR(src)        (((u32)(src)<<9) & 0x00000600)
/*       Fields mac_pcs_ctl0_lane_pwr_off        */
#define MAC_PCS_CTL0_LANE_PWR_OFF1_WIDTH                                      1
#define MAC_PCS_CTL0_LANE_PWR_OFF1_SHIFT                                      8
#define MAC_PCS_CTL0_LANE_PWR_OFF1_MASK                              0x00000100
#define MAC_PCS_CTL0_LANE_PWR_OFF1_RD(src)            (((src) & 0x00000100)>>8)
#define MAC_PCS_CTL0_LANE_PWR_OFF1_WR(src)       (((u32)(src)<<8) & 0x00000100)
/*       Fields mac_pcs_ctl0_rx_mode     */
#define MAC_PCS_CTL0_RX_MODE1_WIDTH                                           4
#define MAC_PCS_CTL0_RX_MODE1_SHIFT                                           4
#define MAC_PCS_CTL0_RX_MODE1_MASK                                   0x000000f0
#define MAC_PCS_CTL0_RX_MODE1_RD(src)                 (((src) & 0x000000f0)>>4)
#define MAC_PCS_CTL0_RX_MODE1_WR(src)            (((u32)(src)<<4) & 0x000000f0)
/*       Fields mac_pcs_ctl0_tx_mode     */
#define MAC_PCS_CTL0_TX_MODE1_WIDTH                                           4
#define MAC_PCS_CTL0_TX_MODE1_SHIFT                                           0
#define MAC_PCS_CTL0_TX_MODE1_MASK                                   0x0000000f
#define MAC_PCS_CTL0_TX_MODE1_RD(src)                    (((src) & 0x0000000f))
#define MAC_PCS_CTL0_TX_MODE1_WR(src)               (((u32)(src)) & 0x0000000f)

/*      Register PcsCsr_RegAddr */
/*       Fields mgmt_pcs_mmd_addr        */
#define MGMT_PCS_MMD_ADDR_WIDTH                                               5
#define MGMT_PCS_MMD_ADDR_SHIFT                                              18
#define MGMT_PCS_MMD_ADDR_MASK                                       0x007c0000
#define MGMT_PCS_MMD_ADDR_RD(src)                    (((src) & 0x007c0000)>>18)
#define MGMT_PCS_MMD_ADDR_WR(src)               (((u32)(src)<<18) & 0x007c0000)
/*       Fields mgmt_pcs_lane_sel        */
#define MGMT_PCS_LANE_SEL_WIDTH                                               2
#define MGMT_PCS_LANE_SEL_SHIFT                                              16
#define MGMT_PCS_LANE_SEL_MASK                                       0x00030000
#define MGMT_PCS_LANE_SEL_RD(src)                    (((src) & 0x00030000)>>16)
#define MGMT_PCS_LANE_SEL_WR(src)               (((u32)(src)<<16) & 0x00030000)
/*       Fields mgmt_pcs_reg_addr        */
#define MGMT_PCS_REG_ADDR_WIDTH                                              16
#define MGMT_PCS_REG_ADDR_SHIFT                                               0
#define MGMT_PCS_REG_ADDR_MASK                                       0x0000ffff
#define MGMT_PCS_REG_ADDR_RD(src)                        (((src) & 0x0000ffff))
#define MGMT_PCS_REG_ADDR_WR(src)                   (((u32)(src)) & 0x0000ffff)

/*      Register Sata_IoCoh_Start0      */
/*       Fields Addr_High        */
#define ADDR_HIGH_START_WIDTH                                                32
#define ADDR_HIGH_START_SHIFT                                                 0
#define ADDR_HIGH_START_MASK                                         0xffffffff
#define ADDR_HIGH_START_RD(src)                          (((src) & 0xffffffff))
#define ADDR_HIGH_START_WR(src)                     (((u32)(src)) & 0xffffffff)

/*      Register Sata_IoCoh_Start1      */
/*       Fields Addr_Low         */
#define ADDR_LOW_START_WIDTH                                                  4
#define ADDR_LOW_START_SHIFT                                                 28
#define ADDR_LOW_START_MASK                                          0xf0000000
#define ADDR_LOW_START_RD(src)                       (((src) & 0xf0000000)>>28)
#define ADDR_LOW_START_WR(src)                  (((u32)(src)<<28) & 0xf0000000)

/*      Register Sata_IoCoh_End0        */
/*       Fields Addr_High        */
#define ADDR_HIGH_END_WIDTH                                                  32
#define ADDR_HIGH_END_SHIFT                                                   0
#define ADDR_HIGH_END_MASK                                           0xffffffff
#define ADDR_HIGH_END_RD(src)                            (((src) & 0xffffffff))
#define ADDR_HIGH_END_WR(src)                       (((u32)(src)) & 0xffffffff)

/*      Register Sata_IoCoh_End1        */
/*       Fields Addr_Low         */
#define ADDR_LOW_END_WIDTH                                                    4
#define ADDR_LOW_END_SHIFT                                                   28                
#define ADDR_LOW_END_MASK                                            0xf0000000
#define ADDR_LOW_END_RD(src)                         (((src) & 0xf0000000)>>28)
#define ADDR_LOW_END_WR(src)                    (((u32)(src)<<28) & 0xf0000000)
/*       Fields Read Coherency Mapper Enable */
#define READ_COHERENCY_MAPPER_WIDTH                                           1
#define READ_COHERENCY_MAPPER_SHIFT                                           7
#define READ_COHERENCY_MAPPER_MASK                                   0x00000080
#define READ_COHERENCY_MAPPER_RD(src)                 (((src) & 0x00000080)>>7)
#define READ_COHERENCY_MAPPER_WR(src)            (((u32)(src)<<7) & 0x00000080)
/*       Fields Read requestor id       */
#define READ_REQUESTOR_ID_WIDTH                                               2
#define READ_REQUESTOR_ID_SHIFT                                               5
#define READ_REQUESTOR_ID_MASK                                       0x00000060
#define READ_REQUESTOR_ID_RD(src)                     (((src) & 0x00000060)>>5)
#define READ_REQUESTOR_ID_WR(src)                (((u32)(src)<<5) & 0x00000060)
/*       Fields Read Coherent Access Enable */
#define READ_COHERENT_ACCESS_WIDTH                                            1
#define READ_COHERENT_ACCESS_SHIFT                                            4
#define READ_COHERENT_ACCESS_MASK                                    0x00000010
#define READ_COHERENT_ACCESS_RD(src)                  (((src) & 0x00000010)>>4)
#define READ_COHERENT_ACCESS_WR(src)             (((u32)(src)<<4) & 0x00000010)
/*       Fields Write Coherency Mapper Enable */
#define WRITE_COHERENCY_MAPPER_WIDTH                                          1
#define WRITE_COHERENCY_MAPPER_SHIFT                                          3
#define WRITE_COHERENCY_MAPPER_MASK                                  0x00000008
#define WRITE_COHERENCY_MAPPER_RD(src)                (((src) & 0x00000008)>>3)
#define WRITE_COHERENCY_MAPPER_WR(src)           (((u32)(src)<<3) & 0x00000008)
/*       Fields Write requestor id       */
#define WRITE_REQUESTOR_ID_WIDTH                                              2
#define WRITE_REQUESTOR_ID_SHIFT                                              1
#define WRITE_REQUESTOR_ID_MASK                                      0x00000006
#define WRITE_REQUESTOR_ID_RD(src)                    (((src) & 0x00000006)>>1)
#define WRITE_REQUESTOR_ID_WR(src)               (((u32)(src)<<1) & 0x00000006)
/*       Fields Write Coherent Access Enable */
#define WRITE_COHERENT_ACCESS_WIDTH                                           1
#define WRITE_COHERENT_ACCESS_SHIFT                                           0
#define WRITE_COHERENT_ACCESS_MASK                                   0x00000001
#define WRITE_COHERENT_ACCESS_RD(src)                    (((src) & 0x00000001))
#define WRITE_COHERENT_ACCESS_WR(src)               (((u32)(src)) & 0x00000001)

#define APM_SATA_PCS_NUM 	2
#define SATA_WAIT_TO 		500

struct apm_sata_pdata {
	int num_pcs;
	u32 phy_active; /* Each bit represent port active */
	void __iomem *apm_sata_diag;
	void __iomem *apm_sata_pcs[APM_SATA_PCS_NUM];
	u8 serdes_gen[APM_SATA_PCS_NUM];
};

u32 apm_pcs_read(void __iomem *pcs_base, u32 pcs_reg);
void apm_pcs_write(void __iomem *pcs_base, u32 pcs_reg, u32 pcs_val);

#endif /* CONFIG_APM */

#endif /* _SATA_APM_H_ */
