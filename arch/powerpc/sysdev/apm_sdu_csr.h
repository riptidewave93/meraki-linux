/**
 * APM86xxx SDU CSR
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
#ifndef _APM_SDU_CSR_H__
#define _APM_SDU_CSR_H__

/*	Global Base Address	*/
#define SDU_CSR_BASE_ADDR			0xddd810000ULL

/*    Address SDU_CSR  Registers */
#define SDU_PERR_ADDR                                                0x00000000
#define SDU_PERR_DEFAULT                                             0x00000000
#define SDU_SLV0_STS_ADDR                                            0x00000004
#define SDU_SLV0_STS_DEFAULT                                         0x00000000
#define SDU_SLV0_ADDR_STS_ADDR                                       0x00000008
#define SDU_SLV0_ADDR_STS_DEFAULT                                    0x00000000
#define SDU_SLV1_STS_ADDR                                            0x0000000c
#define SDU_SLV1_STS_DEFAULT                                         0x00000000
#define SDU_SLV1_ADDR_STS_ADDR                                       0x00000010
#define SDU_SLV1_ADDR_STS_DEFAULT                                    0x00000000
#define SDU_SLV2_STS_ADDR                                            0x00000014
#define SDU_SLV2_STS_DEFAULT                                         0x00000000
#define SDU_SLV2_ADDR_STS_ADDR                                       0x00000018
#define SDU_SLV2_ADDR_STS_DEFAULT                                    0x00000000
#define SDU_BRG_ERR_ADDR                                             0x0000001c
#define SDU_BRG_ERR_DEFAULT                                          0x00000000
#define SDU_PLB_WERRL_ADDR                                           0x00000020
#define SDU_PLB_WERRL_DEFAULT                                        0x00000000
#define SDU_PLB_WERRH_ADDR                                           0x00000024
#define SDU_PLB_WERRH_DEFAULT                                        0x00000000
#define SDU_ERR_STS_ADDR                                             0x00000028
#define SDU_ERR_STS_DEFAULT                                          0x00000000
#define SDU_PBRG_CTRL_ADDR                                           0x0000002c
#define SDU_PBRG_CTRL_DEFAULT                                        0x00000001

/*	Register SDU_PERR	*/ 
/*	 Fields ENET_PCF_PERR	 */
#define ENET_PCF_PERR_WIDTH                                                   1
#define ENET_PCF_PERR_SHIFT                                                  15
#define ENET_PCF_PERR_MASK                                           0x00008000
#define ENET_PCF_PERR_RD(src)                        (((src) & 0x00008000)>>15)
#define ENET_PCF_PERR_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))
/*	 Fields EBM_USB_PERR	 */
#define EBM_USB_PERR_WIDTH                                                    1
#define EBM_USB_PERR_SHIFT                                                   14
#define EBM_USB_PERR_MASK                                            0x00004000
#define EBM_USB_PERR_RD(src)                         (((src) & 0x00004000)>>14)
#define EBM_USB_PERR_SET(dst,src) \
                      (((dst) & ~0x00004000) | (((u32)(src)<<14) & 0x00004000))
/*	 Fields USB_DSF_PERR	 */
#define USB_DSF_PERR_WIDTH                                                    1
#define USB_DSF_PERR_SHIFT                                                   13
#define USB_DSF_PERR_MASK                                            0x00002000
#define USB_DSF_PERR_RD(src)                         (((src) & 0x00002000)>>13)
#define USB_DSF_PERR_SET(dst,src) \
                      (((dst) & ~0x00002000) | (((u32)(src)<<13) & 0x00002000))
/*	 Fields QM_PERR	 */
#define QM_PERR_WIDTH                                                         1
#define QM_PERR_SHIFT                                                        12
#define QM_PERR_MASK                                                 0x00001000
#define QM_PERR_RD(src)                              (((src) & 0x00001000)>>12)
#define QM_PERR_SET(dst,src) \
                      (((dst) & ~0x00001000) | (((u32)(src)<<12) & 0x00001000))
/*	 Fields AHBC_PERR	 */
#define AHBC_PERR_WIDTH                                                       1
#define AHBC_PERR_SHIFT                                                      11
#define AHBC_PERR_MASK                                               0x00000800
#define AHBC_PERR_RD(src)                            (((src) & 0x00000800)>>11)
#define AHBC_PERR_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*	 Fields LCDC_PERR	 */
#define LCDC_PERR_WIDTH                                                       1
#define LCDC_PERR_SHIFT                                                      10
#define LCDC_PERR_MASK                                               0x00000400
#define LCDC_PERR_RD(src)                            (((src) & 0x00000400)>>10)
#define LCDC_PERR_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields SEC_PERR	 */
#define SEC_PERR_WIDTH                                                        1
#define SEC_PERR_SHIFT                                                        9
#define SEC_PERR_MASK                                                0x00000200
#define SEC_PERR_RD(src)                              (((src) & 0x00000200)>>9)
#define SEC_PERR_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields OCM_PERR	 */
#define OCM_PERR_WIDTH                                                        1
#define OCM_PERR_SHIFT                                                        8
#define OCM_PERR_MASK                                                0x00000100
#define OCM_PERR_RD(src)                              (((src) & 0x00000100)>>8)
#define OCM_PERR_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*	 Fields CLE_PERR	 */
#define CLE_PERR_WIDTH                                                        1
#define CLE_PERR_SHIFT                                                        7
#define CLE_PERR_MASK                                                0x00000080
#define CLE_PERR_RD(src)                              (((src) & 0x00000080)>>7)
#define CLE_PERR_SET(dst,src) \
                       (((dst) & ~0x00000080) | (((u32)(src)<<7) & 0x00000080))
/*	 Fields ENET_PERR	 */
#define ENET_PERR_WIDTH                                                       1
#define ENET_PERR_SHIFT                                                       6
#define ENET_PERR_MASK                                               0x00000040
#define ENET_PERR_RD(src)                             (((src) & 0x00000040)>>6)
#define ENET_PERR_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*	 Fields MPA_PERR	 */
#define MPA_PERR_WIDTH                                                        1
#define MPA_PERR_SHIFT                                                        5
#define MPA_PERR_MASK                                                0x00000020
#define MPA_PERR_RD(src)                              (((src) & 0x00000020)>>5)
#define MPA_PERR_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields PCIe3_PERR	 */
#define PCIE3_PERR_WIDTH                                                      1
#define PCIE3_PERR_SHIFT                                                      4
#define PCIE3_PERR_MASK                                              0x00000010
#define PCIE3_PERR_RD(src)                            (((src) & 0x00000010)>>4)
#define PCIE3_PERR_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields PCIe2_PERR	 */
#define PCIE2_PERR_WIDTH                                                      1
#define PCIE2_PERR_SHIFT                                                      3
#define PCIE2_PERR_MASK                                              0x00000008
#define PCIE2_PERR_RD(src)                            (((src) & 0x00000008)>>3)
#define PCIE2_PERR_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields PCIe1_PERR	 */
#define PCIE1_PERR_WIDTH                                                      1
#define PCIE1_PERR_SHIFT                                                      2
#define PCIE1_PERR_MASK                                              0x00000004
#define PCIE1_PERR_RD(src)                            (((src) & 0x00000004)>>2)
#define PCIE1_PERR_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields SATA_PERR	 */
#define SATA_PERR_WIDTH                                                       1
#define SATA_PERR_SHIFT                                                       1
#define SATA_PERR_MASK                                               0x00000002
#define SATA_PERR_RD(src)                             (((src) & 0x00000002)>>1)
#define SATA_PERR_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields MQ_PERR	 */
#define MQ_PERR_WIDTH                                                         1
#define MQ_PERR_SHIFT                                                         0
#define MQ_PERR_MASK                                                 0x00000001
#define MQ_PERR_RD(src)                                  (((src) & 0x00000001))
#define MQ_PERR_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register SDU_SLV0_STS	*/ 
/*	 Fields WrDecErr	 */
#define WRDECERR_WIDTH                                                        1
#define WRDECERR_SHIFT                                                       16
#define WRDECERR_MASK                                                0x00010000
#define WRDECERR_RD(src)                             (((src) & 0x00010000)>>16)
#define WRDECERR_SET(dst,src) \
                      (((dst) & ~0x00010000) | (((u32)(src)<<16) & 0x00010000))
/*	 Fields Info	 */
#define INFO_WIDTH                                                           10
#define INFO_SHIFT                                                            0
#define INFO_MASK                                                    0x000003ff
#define INFO_RD(src)                                     (((src) & 0x000003ff))
#define INFO_SET(dst,src) \
                          (((dst) & ~0x000003ff) | (((u32)(src)) & 0x000003ff))

/*	Register SDU_SLV0_ADDR_STS	*/ 
/*	 Fields WrAddr	 */
#define WRADDR_WIDTH                                                         32
#define WRADDR_SHIFT                                                          0
#define WRADDR_MASK                                                  0xffffffff
#define WRADDR_RD(src)                                   (((src) & 0xffffffff))
#define WRADDR_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register SDU_SLV1_STS	*/ 
/*	 Fields WrDecErr	 */
#define WRDECERR_F1_WIDTH                                                     1
#define WRDECERR_F1_SHIFT                                                    16
#define WRDECERR_F1_MASK                                             0x00010000
#define WRDECERR_F1_RD(src)                          (((src) & 0x00010000)>>16)
#define WRDECERR_F1_SET(dst,src) \
                      (((dst) & ~0x00010000) | (((u32)(src)<<16) & 0x00010000))
/*	 Fields Info	 */
#define INFO_F1_WIDTH                                                        10
#define INFO_F1_SHIFT                                                         0
#define INFO_F1_MASK                                                 0x000003ff
#define INFO_F1_RD(src)                                  (((src) & 0x000003ff))
#define INFO_F1_SET(dst,src) \
                          (((dst) & ~0x000003ff) | (((u32)(src)) & 0x000003ff))

/*	Register SDU_SLV1_ADDR_STS	*/ 
/*	 Fields WrAddr	 */
#define WRADDR_F1_WIDTH                                                      32
#define WRADDR_F1_SHIFT                                                       0
#define WRADDR_F1_MASK                                               0xffffffff
#define WRADDR_F1_RD(src)                                (((src) & 0xffffffff))
#define WRADDR_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register SDU_SLV2_STS	*/ 
/*	 Fields WrDecErr	 */
#define WRDECERR_F2_WIDTH                                                     1
#define WRDECERR_F2_SHIFT                                                    16
#define WRDECERR_F2_MASK                                             0x00010000
#define WRDECERR_F2_RD(src)                          (((src) & 0x00010000)>>16)
#define WRDECERR_F2_SET(dst,src) \
                      (((dst) & ~0x00010000) | (((u32)(src)<<16) & 0x00010000))
/*	 Fields Info	 */
#define INFO_F2_WIDTH                                                        10
#define INFO_F2_SHIFT                                                         0
#define INFO_F2_MASK                                                 0x000003ff
#define INFO_F2_RD(src)                                  (((src) & 0x000003ff))
#define INFO_F2_SET(dst,src) \
                          (((dst) & ~0x000003ff) | (((u32)(src)) & 0x000003ff))

/*	Register SDU_SLV2_ADDR_STS	*/ 
/*	 Fields WrAddr	 */
#define WRADDR_F2_WIDTH                                                      32
#define WRADDR_F2_SHIFT                                                       0
#define WRADDR_F2_MASK                                               0xffffffff
#define WRADDR_F2_RD(src)                                (((src) & 0xffffffff))
#define WRADDR_F2_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register SDU_BRG_ERR	*/ 
/*	 Fields AhbBrgErr	 */
#define AHBBRGERR_WIDTH                                                       1
#define AHBBRGERR_SHIFT                                                      17
#define AHBBRGERR_MASK                                               0x00020000
#define AHBBRGERR_RD(src)                            (((src) & 0x00020000)>>17)
#define AHBBRGERR_SET(dst,src) \
                      (((dst) & ~0x00020000) | (((u32)(src)<<17) & 0x00020000))
/*	 Fields PlbBrgErr	 */
#define PLBBRGERR_WIDTH                                                       1
#define PLBBRGERR_SHIFT                                                      16
#define PLBBRGERR_MASK                                               0x00010000
#define PLBBRGERR_RD(src)                            (((src) & 0x00010000)>>16)
#define PLBBRGERR_SET(dst,src) \
                      (((dst) & ~0x00010000) | (((u32)(src)<<16) & 0x00010000))
/*	 Fields WrErrAck	 */
#define WRERRACK_WIDTH                                                        1
#define WRERRACK_SHIFT                                                       15
#define WRERRACK_MASK                                                0x00008000
#define WRERRACK_RD(src)                             (((src) & 0x00008000)>>15)
#define WRERRACK_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))
/*	 Fields WrID	 */
#define WRID_WIDTH                                                            4
#define WRID_SHIFT                                                            8
#define WRID_MASK                                                    0x00000f00
#define WRID_RD(src)                                  (((src) & 0x00000f00)>>8)
#define WRID_SET(dst,src) \
                       (((dst) & ~0x00000f00) | (((u32)(src)<<8) & 0x00000f00))
/*	 Fields Info	 */
#define INFO_F3_WIDTH                                                         8
#define INFO_F3_SHIFT                                                         0
#define INFO_F3_MASK                                                 0x000000ff
#define INFO_F3_RD(src)                                  (((src) & 0x000000ff))
#define INFO_F3_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register SDU_PLB_WERRL	*/ 
/*	 Fields ADDR_LOW	 */
#define ADDR_LOW_WIDTH                                                       32
#define ADDR_LOW_SHIFT                                                        0
#define ADDR_LOW_MASK                                                0xffffffff
#define ADDR_LOW_RD(src)                                 (((src) & 0xffffffff))
#define ADDR_LOW_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register SDU_PLB_WERRH	*/ 
/*	 Fields CPUID	 */
#define CPUID_WIDTH                                                           1
#define CPUID_SHIFT                                                           8
#define CPUID_MASK                                                   0x00000100
#define CPUID_RD(src)                                 (((src) & 0x00000100)>>8)
#define CPUID_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*	 Fields WR_LEN	 */
#define WR_LEN_WIDTH                                                          4
#define WR_LEN_SHIFT                                                          4
#define WR_LEN_MASK                                                  0x000000f0
#define WR_LEN_RD(src)                                (((src) & 0x000000f0)>>4)
#define WR_LEN_SET(dst,src) \
                       (((dst) & ~0x000000f0) | (((u32)(src)<<4) & 0x000000f0))
/*	 Fields ADDR_HIGH	 */
#define ADDR_HIGH_WIDTH                                                       4
#define ADDR_HIGH_SHIFT                                                       0
#define ADDR_HIGH_MASK                                               0x0000000f
#define ADDR_HIGH_RD(src)                                (((src) & 0x0000000f))
#define ADDR_HIGH_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register SDU_ERR_STS	*/ 
/*	 Fields ECC_ERR	 */
#define ECC_ERR_WIDTH                                                         1
#define ECC_ERR_SHIFT                                                         3
#define ECC_ERR_MASK                                                 0x00000008
#define ECC_ERR_RD(src)                               (((src) & 0x00000008)>>3)
#define ECC_ERR_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields AXI_DERR	 */
#define AXI_DERR_WIDTH                                                        1
#define AXI_DERR_SHIFT                                                        2
#define AXI_DERR_MASK                                                0x00000004
#define AXI_DERR_RD(src)                              (((src) & 0x00000004)>>2)
#define AXI_DERR_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields BRG_ERR	 */
#define BRG_ERR_WIDTH                                                         1
#define BRG_ERR_SHIFT                                                         1
#define BRG_ERR_MASK                                                 0x00000002
#define BRG_ERR_RD(src)                               (((src) & 0x00000002)>>1)
#define BRG_ERR_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields PLB5_WERR	 */
#define PLB5_WERR_WIDTH                                                       1
#define PLB5_WERR_SHIFT                                                       0
#define PLB5_WERR_MASK                                               0x00000001
#define PLB5_WERR_RD(src)                                (((src) & 0x00000001))
#define PLB5_WERR_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register SDU_PBRG_CTRL	*/ 
/*	 Fields PLB_LT_AXI	 */
#define PLB_LT_AXI_WIDTH                                                      1
#define PLB_LT_AXI_SHIFT                                                      1
#define PLB_LT_AXI_MASK                                              0x00000002
#define PLB_LT_AXI_RD(src)                            (((src) & 0x00000002)>>1)
#define PLB_LT_AXI_WR(src)                       (((u32)(src)<<1) & 0x00000002)
#define PLB_LT_AXI_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields RD_CUT_THRU	 */
#define RD_CUT_THRU_WIDTH                                                     1
#define RD_CUT_THRU_SHIFT                                                     0
#define RD_CUT_THRU_MASK                                             0x00000001
#define RD_CUT_THRU_RD(src)                              (((src) & 0x00000001))
#define RD_CUT_THRU_WR(src)                         (((u32)(src)) & 0x00000001)
#define RD_CUT_THRU_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

#endif
