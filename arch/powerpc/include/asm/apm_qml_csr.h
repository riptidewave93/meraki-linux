/*
 * APM 86xxx QM-Lite Header File
 *
 * Copyright (c) 2011, Applied Micro Circuits Corporation
 * Author: Anup Patel <apatel@apm.com>
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
#ifndef _APM_QML_CSR_H__
#define _APM_QML_CSR_H__

#if defined(CONFIG_APM862xx)
/* For APM862xx */

/*	Global Base Address	*/
#define QML_GLBL_DIAG_CSR_BASE_ADDR				0xddd8e7000ULL

/*    Address GLBL_DIAG_CSR  Registers */
#define QML_CFG_DIAG_SEL_ADDR                                        0x00000000
#define QML_CFG_DIAG_SEL_DEFAULT                                     0x00000000
#define QML_CFG_READ_BW_LAT_ADDR_MASK_ADDR                           0x00000004
#define QML_CFG_READ_BW_LAT_ADDR_MASK_DEFAULT                        0x00000000
#define QML_CFG_READ_BW_LAT_ADDR_PAT_ADDR                            0x00000008
#define QML_CFG_READ_BW_LAT_ADDR_PAT_DEFAULT                         0xffffffff
#define QML_CFG_WRITE_BW_LAT_ADDR_MASK_ADDR                          0x0000000c
#define QML_CFG_WRITE_BW_LAT_ADDR_MASK_DEFAULT                       0x00000000
#define QML_CFG_WRITE_BW_LAT_ADDR_PAT_ADDR                           0x00000010
#define QML_CFG_WRITE_BW_LAT_ADDR_PAT_DEFAULT                        0xffffffff
#define QML_CFG_DIAG_START_STOP_ADDR                                 0x00000014
#define QML_CFG_DIAG_START_STOP_DEFAULT                              0x0000001f
#define QML_CFG_BW_MSTR_STOP_CNT_ADDR                                0x00000018
#define QML_CFG_BW_MSTR_STOP_CNT_DEFAULT                             0x00040004
#define QML_CFG_BW_SLV_STOP_CNT_ADDR                                 0x0000001c
#define QML_CFG_BW_SLV_STOP_CNT_DEFAULT                              0x00040004
#define QML_STS_READ_LATENCY_OUTPUT_ADDR                             0x00000020
#define QML_STS_READ_LATENCY_OUTPUT_DEFAULT                          0x00000000
#define QML_STS_AXI_MRD_BW_CLK_CNT_ADDR                              0x00000024
#define QML_STS_AXI_MRD_BW_CLK_CNT_DEFAULT                           0x00000000
#define QML_STS_AXI_MRD_BW_BYTE_CNT_ADDR                             0x00000028
#define QML_STS_AXI_MRD_BW_BYTE_CNT_DEFAULT                          0x00000000
#define QML_STS_AXI_MWR_BW_CLK_CNT_ADDR                              0x0000002c
#define QML_STS_AXI_MWR_BW_CLK_CNT_DEFAULT                           0x00000000
#define QML_STS_AXI_MWR_BW_BYTE_CNT_ADDR                             0x00000030
#define QML_STS_AXI_MWR_BW_BYTE_CNT_DEFAULT                          0x00000000
#define QML_STS_AXI_SRD_BW_CLK_CNT_ADDR                              0x00000034
#define QML_STS_AXI_SRD_BW_CLK_CNT_DEFAULT                           0x00000000
#define QML_STS_AXI_SRD_BW_BYTE_CNT_ADDR                             0x00000038
#define QML_STS_AXI_SRD_BW_BYTE_CNT_DEFAULT                          0x00000000
#define QML_STS_AXI_SWR_BW_CLK_CNT_ADDR                              0x0000003c
#define QML_STS_AXI_SWR_BW_CLK_CNT_DEFAULT                           0x00000000
#define QML_STS_AXI_SWR_BW_BYTE_CNT_ADDR                             0x00000040
#define QML_STS_AXI_SWR_BW_BYTE_CNT_DEFAULT                          0x00000000
#define QML_CFG_DBG_TRIG_CTRL_ADDR                                   0x00000044
#define QML_CFG_DBG_TRIG_CTRL_DEFAULT                                0x00000000
#define QML_CFG_DBG_PAT_REG_0_ADDR                                   0x00000048
#define QML_CFG_DBG_PAT_REG_0_DEFAULT                                0x00000000
#define QML_CFG_DBG_PAT_MASK_REG_0_ADDR                              0x0000004c
#define QML_CFG_DBG_PAT_MASK_REG_0_DEFAULT                           0x00000000
#define QML_CFG_DBG_PAT_REG_1_ADDR                                   0x00000050
#define QML_CFG_DBG_PAT_REG_1_DEFAULT                                0x00000000
#define QML_CFG_DBG_PAT_MASK_REG_1_ADDR                              0x00000054
#define QML_CFG_DBG_PAT_MASK_REG_1_DEFAULT                           0x00000000
#define QML_DBG_TRIG_OUT_ADDR                                        0x00000058
#define QML_DBG_TRIG_OUT_DEFAULT                                     0x00000000
#define QML_DBG_ECC_INT_OUT_ADDR                                     0x0000005c
#define QML_DBG_ECC_INT_OUT_DEFAULT                                  0x00000000
#define QML_DBG_ECC_INT_OUTMASK_ADDR                                 0x00000060
#define QML_CFG_SECC_INT_THRSH_REG_ADDR                              0x00000064
#define QML_CFG_SECC_INT_THRSH_REG_DEFAULT                           0x000000fe
#define QML_CFG_MEM_ECC_BYPASS_ADDR                                  0x00000068
#define QML_CFG_MEM_ECC_BYPASS_DEFAULT                               0x00000000
#define QML_CFG_MEM_PWRDN_DIS_ADDR                                   0x0000006c
#define QML_CFG_MEM_PWRDN_DIS_DEFAULT                                0x00000000
#define QML_CFG_MEM_RAM_SHUTDOWN_ADDR                                0x00000070
#define QML_CFG_MEM_RAM_SHUTDOWN_DEFAULT                             0xffffffff
#define QML_BLOCK_MEM_RDY_ADDR                                       0x00000074
#define QML_BLOCK_MEM_RDY_DEFAULT                                    0xffffffff
#define QML_CFG_AXI_SLV_RD_TMO_ADDR                                  0x00000078
#define QML_CFG_AXI_SLV_RD_TMO_DEFAULT                               0x83ff83ff
#define QML_CFG_AXI_SLV_WR_TMO_ADDR                                  0x0000007c
#define QML_CFG_AXI_SLV_WR_TMO_DEFAULT                               0x83ff83ff
#define QML_STS_AXI_SLV_TMO_ADDR                                     0x00000080
#define QML_STS_AXI_SLV_TMO_DEFAULT                                  0x00000000
#define QML_CFG_AXI_SS_CSR_TMO_ADDR                                  0x00000084
#define QML_CFG_AXI_SS_CSR_TMO_DEFAULT                               0x02008000
#define QML_STS_READ_LATENCY_TOT_READ_REQS_ADDR                      0x0000008c
#define QML_STS_READ_LATENCY_TOT_READ_REQS_DEFAULT                   0x00000000

/*	Register CFG_DIAG_SEL	*/ 
/*	 Fields CFG_AXI_NON_AXI_MUX_SELECT	 */
#define QML_CFG_AXI_NON_AXI_MUX_SELECT_WIDTH                                  1
#define QML_CFG_AXI_NON_AXI_MUX_SELECT_SHIFT                                 11
#define QML_CFG_AXI_NON_AXI_MUX_SELECT_MASK                          0x00000800
#define QML_CFG_AXI_NON_AXI_MUX_SELECT_RD(src)       (((src) & 0x00000800)>>11)
#define QML_CFG_AXI_NON_AXI_MUX_SELECT_WR(src)  (((u32)(src)<<11) & 0x00000800)
#define QML_CFG_AXI_NON_AXI_MUX_SELECT_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*	 Fields CFG_MUX_SELECTOR	 */
#define QML_CFG_MUX_SELECTOR_WIDTH                                           11
#define QML_CFG_MUX_SELECTOR_SHIFT                                            0
#define QML_CFG_MUX_SELECTOR_MASK                                    0x000007ff
#define QML_CFG_MUX_SELECTOR_RD(src)                     (((src) & 0x000007ff))
#define QML_CFG_MUX_SELECTOR_WR(src)                (((u32)(src)) & 0x000007ff)
#define QML_CFG_MUX_SELECTOR_SET(dst,src) \
                          (((dst) & ~0x000007ff) | (((u32)(src)) & 0x000007ff))

/*	Register CFG_READ_BW_LAT_ADDR_MASK	*/ 
/*	 Fields READ_ADDR_MASK	 */
#define QML_READ_ADDR_MASK_WIDTH                                             32
#define QML_READ_ADDR_MASK_SHIFT                                              0
#define QML_READ_ADDR_MASK_MASK                                      0xffffffff
#define QML_READ_ADDR_MASK_RD(src)                       (((src) & 0xffffffff))
#define QML_READ_ADDR_MASK_WR(src)                  (((u32)(src)) & 0xffffffff)
#define QML_READ_ADDR_MASK_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_READ_BW_LAT_ADDR_PAT	*/ 
/*	 Fields READ_ADDR_PAT	 */
#define QML_READ_ADDR_PAT_WIDTH                                              32
#define QML_READ_ADDR_PAT_SHIFT                                               0
#define QML_READ_ADDR_PAT_MASK                                       0xffffffff
#define QML_READ_ADDR_PAT_RD(src)                        (((src) & 0xffffffff))
#define QML_READ_ADDR_PAT_WR(src)                   (((u32)(src)) & 0xffffffff)
#define QML_READ_ADDR_PAT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_WRITE_BW_LAT_ADDR_MASK	*/ 
/*	 Fields WRITE_ADDR_MASK	 */
#define QML_WRITE_ADDR_MASK_WIDTH                                            32
#define QML_WRITE_ADDR_MASK_SHIFT                                             0
#define QML_WRITE_ADDR_MASK_MASK                                     0xffffffff
#define QML_WRITE_ADDR_MASK_RD(src)                      (((src) & 0xffffffff))
#define QML_WRITE_ADDR_MASK_WR(src)                 (((u32)(src)) & 0xffffffff)
#define QML_WRITE_ADDR_MASK_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_WRITE_BW_LAT_ADDR_PAT	*/ 
/*	 Fields WRITE_ADDR_PAT	 */
#define QML_WRITE_ADDR_PAT_WIDTH                                             32
#define QML_WRITE_ADDR_PAT_SHIFT                                              0
#define QML_WRITE_ADDR_PAT_MASK                                      0xffffffff
#define QML_WRITE_ADDR_PAT_RD(src)                       (((src) & 0xffffffff))
#define QML_WRITE_ADDR_PAT_WR(src)                  (((u32)(src)) & 0xffffffff)
#define QML_WRITE_ADDR_PAT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_DIAG_START_STOP	*/ 
/*	 Fields START_RD_LATENCY	 */
#define QML_START_RD_LATENCY_WIDTH                                            1
#define QML_START_RD_LATENCY_SHIFT                                            4
#define QML_START_RD_LATENCY_MASK                                    0x00000010
#define QML_START_RD_LATENCY_RD(src)                  (((src) & 0x00000010)>>4)
#define QML_START_RD_LATENCY_WR(src)             (((u32)(src)<<4) & 0x00000010)
#define QML_START_RD_LATENCY_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields START_SRD_BW	 */
#define QML_START_SRD_BW_WIDTH                                                1
#define QML_START_SRD_BW_SHIFT                                                3
#define QML_START_SRD_BW_MASK                                        0x00000008
#define QML_START_SRD_BW_RD(src)                      (((src) & 0x00000008)>>3)
#define QML_START_SRD_BW_WR(src)                 (((u32)(src)<<3) & 0x00000008)
#define QML_START_SRD_BW_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields START_MRD_BW	 */
#define QML_START_MRD_BW_WIDTH                                                1
#define QML_START_MRD_BW_SHIFT                                                2
#define QML_START_MRD_BW_MASK                                        0x00000004
#define QML_START_MRD_BW_RD(src)                      (((src) & 0x00000004)>>2)
#define QML_START_MRD_BW_WR(src)                 (((u32)(src)<<2) & 0x00000004)
#define QML_START_MRD_BW_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields START_SWR_BW	 */
#define QML_START_SWR_BW_WIDTH                                                1
#define QML_START_SWR_BW_SHIFT                                                1
#define QML_START_SWR_BW_MASK                                        0x00000002
#define QML_START_SWR_BW_RD(src)                      (((src) & 0x00000002)>>1)
#define QML_START_SWR_BW_WR(src)                 (((u32)(src)<<1) & 0x00000002)
#define QML_START_SWR_BW_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields START_MWR_BW	 */
#define QML_START_MWR_BW_WIDTH                                                1
#define QML_START_MWR_BW_SHIFT                                                0
#define QML_START_MWR_BW_MASK                                        0x00000001
#define QML_START_MWR_BW_RD(src)                         (((src) & 0x00000001))
#define QML_START_MWR_BW_WR(src)                    (((u32)(src)) & 0x00000001)
#define QML_START_MWR_BW_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register CFG_BW_MSTR_STOP_CNT	*/ 
/*	 Fields MSTR_STOP_RD_CNT	 */
#define QML_MSTR_STOP_RD_CNT_WIDTH                                           16
#define QML_MSTR_STOP_RD_CNT_SHIFT                                           16
#define QML_MSTR_STOP_RD_CNT_MASK                                    0xffff0000
#define QML_MSTR_STOP_RD_CNT_RD(src)                 (((src) & 0xffff0000)>>16)
#define QML_MSTR_STOP_RD_CNT_WR(src)            (((u32)(src)<<16) & 0xffff0000)
#define QML_MSTR_STOP_RD_CNT_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields MSTR_STOP_WR_CNT	 */
#define QML_MSTR_STOP_WR_CNT_WIDTH                                           16
#define QML_MSTR_STOP_WR_CNT_SHIFT                                            0
#define QML_MSTR_STOP_WR_CNT_MASK                                    0x0000ffff
#define QML_MSTR_STOP_WR_CNT_RD(src)                     (((src) & 0x0000ffff))
#define QML_MSTR_STOP_WR_CNT_WR(src)                (((u32)(src)) & 0x0000ffff)
#define QML_MSTR_STOP_WR_CNT_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register CFG_BW_SLV_STOP_CNT	*/ 
/*	 Fields SLV_STOP_RD_CNT	 */
#define QML_SLV_STOP_RD_CNT_WIDTH                                            16
#define QML_SLV_STOP_RD_CNT_SHIFT                                            16
#define QML_SLV_STOP_RD_CNT_MASK                                     0xffff0000
#define QML_SLV_STOP_RD_CNT_RD(src)                  (((src) & 0xffff0000)>>16)
#define QML_SLV_STOP_RD_CNT_WR(src)             (((u32)(src)<<16) & 0xffff0000)
#define QML_SLV_STOP_RD_CNT_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields SLV_STOP_WR_CNT	 */
#define QML_SLV_STOP_WR_CNT_WIDTH                                            16
#define QML_SLV_STOP_WR_CNT_SHIFT                                             0
#define QML_SLV_STOP_WR_CNT_MASK                                     0x0000ffff
#define QML_SLV_STOP_WR_CNT_RD(src)                      (((src) & 0x0000ffff))
#define QML_SLV_STOP_WR_CNT_WR(src)                 (((u32)(src)) & 0x0000ffff)
#define QML_SLV_STOP_WR_CNT_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register STS_READ_LATENCY_OUTPUT	*/ 
/*	 Fields READ_MAX	 */
#define QML_READ_MAX_WIDTH                                                   10
#define QML_READ_MAX_SHIFT                                                   22
#define QML_READ_MAX_MASK                                            0xffc00000
#define QML_READ_MAX_RD(src)                         (((src) & 0xffc00000)>>22)
#define QML_READ_MAX_SET(dst,src) \
                      (((dst) & ~0xffc00000) | (((u32)(src)<<22) & 0xffc00000))
/*	 Fields READ_TOT	 */
#define QML_READ_TOT_WIDTH                                                   22
#define QML_READ_TOT_SHIFT                                                    0
#define QML_READ_TOT_MASK                                            0x003fffff
#define QML_READ_TOT_RD(src)                             (((src) & 0x003fffff))
#define QML_READ_TOT_SET(dst,src) \
                          (((dst) & ~0x003fffff) | (((u32)(src)) & 0x003fffff))

/*	Register STS_AXI_MRD_BW_CLK_CNT	*/ 
/*	 Fields MSTR_READ_BW_CLK_CNT	 */
#define QML_MSTR_READ_BW_CLK_CNT_WIDTH                                       32
#define QML_MSTR_READ_BW_CLK_CNT_SHIFT                                        0
#define QML_MSTR_READ_BW_CLK_CNT_MASK                                0xffffffff
#define QML_MSTR_READ_BW_CLK_CNT_RD(src)                 (((src) & 0xffffffff))
#define QML_MSTR_READ_BW_CLK_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register STS_AXI_MRD_BW_BYTE_CNT	*/ 
/*	 Fields MSTR_READ_BW_BYTE_CNT	 */
#define QML_MSTR_READ_BW_BYTE_CNT_WIDTH                                      32
#define QML_MSTR_READ_BW_BYTE_CNT_SHIFT                                       0
#define QML_MSTR_READ_BW_BYTE_CNT_MASK                               0xffffffff
#define QML_MSTR_READ_BW_BYTE_CNT_RD(src)                (((src) & 0xffffffff))
#define QML_MSTR_READ_BW_BYTE_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register STS_AXI_MWR_BW_CLK_CNT	*/ 
/*	 Fields MSTR_WRITE_BW_CLK_CNT	 */
#define QML_MSTR_WRITE_BW_CLK_CNT_WIDTH                                      32
#define QML_MSTR_WRITE_BW_CLK_CNT_SHIFT                                       0
#define QML_MSTR_WRITE_BW_CLK_CNT_MASK                               0xffffffff
#define QML_MSTR_WRITE_BW_CLK_CNT_RD(src)                (((src) & 0xffffffff))
#define QML_MSTR_WRITE_BW_CLK_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register STS_AXI_MWR_BW_BYTE_CNT	*/ 
/*	 Fields MSTR_WRITE_BW_BYTE_CNT	 */
#define QML_MSTR_WRITE_BW_BYTE_CNT_WIDTH                                     32
#define QML_MSTR_WRITE_BW_BYTE_CNT_SHIFT                                      0
#define QML_MSTR_WRITE_BW_BYTE_CNT_MASK                              0xffffffff
#define QML_MSTR_WRITE_BW_BYTE_CNT_RD(src)               (((src) & 0xffffffff))
#define QML_MSTR_WRITE_BW_BYTE_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register STS_AXI_SRD_BW_CLK_CNT	*/ 
/*	 Fields SLV_READ_BW_CLK_CNT	 */
#define QML_SLV_READ_BW_CLK_CNT_WIDTH                                        32
#define QML_SLV_READ_BW_CLK_CNT_SHIFT                                         0
#define QML_SLV_READ_BW_CLK_CNT_MASK                                 0xffffffff
#define QML_SLV_READ_BW_CLK_CNT_RD(src)                  (((src) & 0xffffffff))
#define QML_SLV_READ_BW_CLK_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register STS_AXI_SRD_BW_BYTE_CNT	*/ 
/*	 Fields SLV_READ_BW_BYTE_CNT	 */
#define QML_SLV_READ_BW_BYTE_CNT_WIDTH                                       32
#define QML_SLV_READ_BW_BYTE_CNT_SHIFT                                        0
#define QML_SLV_READ_BW_BYTE_CNT_MASK                                0xffffffff
#define QML_SLV_READ_BW_BYTE_CNT_RD(src)                 (((src) & 0xffffffff))
#define QML_SLV_READ_BW_BYTE_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register STS_AXI_SWR_BW_CLK_CNT	*/ 
/*	 Fields SLV_WRITE_BW_CLK_CNT	 */
#define QML_SLV_WRITE_BW_CLK_CNT_WIDTH                                       32
#define QML_SLV_WRITE_BW_CLK_CNT_SHIFT                                        0
#define QML_SLV_WRITE_BW_CLK_CNT_MASK                                0xffffffff
#define QML_SLV_WRITE_BW_CLK_CNT_RD(src)                 (((src) & 0xffffffff))
#define QML_SLV_WRITE_BW_CLK_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register STS_AXI_SWR_BW_BYTE_CNT	*/ 
/*	 Fields SLV_WRITE_BW_BYTE_CNT	 */
#define QML_SLV_WRITE_BW_BYTE_CNT_WIDTH                                      32
#define QML_SLV_WRITE_BW_BYTE_CNT_SHIFT                                       0
#define QML_SLV_WRITE_BW_BYTE_CNT_MASK                               0xffffffff
#define QML_SLV_WRITE_BW_BYTE_CNT_RD(src)                (((src) & 0xffffffff))
#define QML_SLV_WRITE_BW_BYTE_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_DBG_TRIG_CTRL	*/ 
/*	 Fields TRIG_EN	 */
#define QML_TRIG_EN_WIDTH                                                     1
#define QML_TRIG_EN_SHIFT                                                     4
#define QML_TRIG_EN_MASK                                             0x00000010
#define QML_TRIG_EN_RD(src)                           (((src) & 0x00000010)>>4)
#define QML_TRIG_EN_WR(src)                      (((u32)(src)<<4) & 0x00000010)
#define QML_TRIG_EN_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields AND_E	 */
#define QML_AND_E_WIDTH                                                       2
#define QML_AND_E_SHIFT                                                       2
#define QML_AND_E_MASK                                               0x0000000c
#define QML_AND_E_RD(src)                             (((src) & 0x0000000c)>>2)
#define QML_AND_E_WR(src)                        (((u32)(src)<<2) & 0x0000000c)
#define QML_AND_E_SET(dst,src) \
                       (((dst) & ~0x0000000c) | (((u32)(src)<<2) & 0x0000000c))
/*	 Fields OR_E	 */
#define QML_OR_E_WIDTH                                                        2
#define QML_OR_E_SHIFT                                                        0
#define QML_OR_E_MASK                                                0x00000003
#define QML_OR_E_RD(src)                                 (((src) & 0x00000003))
#define QML_OR_E_WR(src)                            (((u32)(src)) & 0x00000003)
#define QML_OR_E_SET(dst,src) \
                          (((dst) & ~0x00000003) | (((u32)(src)) & 0x00000003))

/*	Register CFG_DBG_PAT_REG_0	*/ 
/*	 Fields PATTERN	 */
#define QML_PATTERN0_WIDTH                                                   32
#define QML_PATTERN0_SHIFT                                                    0
#define QML_PATTERN0_MASK                                            0xffffffff
#define QML_PATTERN0_RD(src)                             (((src) & 0xffffffff))
#define QML_PATTERN0_WR(src)                        (((u32)(src)) & 0xffffffff)
#define QML_PATTERN0_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_DBG_PAT_MASK_REG_0	*/ 
/*	 Fields PAT_MASK	 */
#define QML_PAT_MASK0_WIDTH                                                  32
#define QML_PAT_MASK0_SHIFT                                                   0
#define QML_PAT_MASK0_MASK                                           0xffffffff
#define QML_PAT_MASK0_RD(src)                            (((src) & 0xffffffff))
#define QML_PAT_MASK0_WR(src)                       (((u32)(src)) & 0xffffffff)
#define QML_PAT_MASK0_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_DBG_PAT_REG_1	*/ 
/*	 Fields PATTERN	 */
#define QML_PATTERN1_WIDTH                                                   32
#define QML_PATTERN1_SHIFT                                                    0
#define QML_PATTERN1_MASK                                            0xffffffff
#define QML_PATTERN1_RD(src)                             (((src) & 0xffffffff))
#define QML_PATTERN1_WR(src)                        (((u32)(src)) & 0xffffffff)
#define QML_PATTERN1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_DBG_PAT_MASK_REG_1	*/ 
/*	 Fields PAT_MASK	 */
#define QML_PAT_MASK1_WIDTH                                                  32
#define QML_PAT_MASK1_SHIFT                                                   0
#define QML_PAT_MASK1_MASK                                           0xffffffff
#define QML_PAT_MASK1_RD(src)                            (((src) & 0xffffffff))
#define QML_PAT_MASK1_WR(src)                       (((u32)(src)) & 0xffffffff)
#define QML_PAT_MASK1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register DBG_TRIG_OUT	*/ 
/*	 Fields DBG_OUT	 */
#define QML_DBG_OUT_WIDTH                                                    32
#define QML_DBG_OUT_SHIFT                                                     0
#define QML_DBG_OUT_MASK                                             0xffffffff
#define QML_DBG_OUT_RD(src)                              (((src) & 0xffffffff))
#define QML_DBG_OUT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register DBG_ECC_INT_OUT	*/ 
/*	 Fields SLV_TMO	 */
#define QML_SLV_TMO_WIDTH                                                     1
#define QML_SLV_TMO_SHIFT                                                     3
#define QML_SLV_TMO_MASK                                             0x00000008
#define QML_SLV_TMO_RD(src)                           (((src) & 0x00000008)>>3)
#define QML_SLV_TMO_WR(src)                      (((u32)(src)<<3) & 0x00000008)
#define QML_SLV_TMO_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields SECC_INT	 */
#define QML_SECC_INT_WIDTH                                                    1
#define QML_SECC_INT_SHIFT                                                    2
#define QML_SECC_INT_MASK                                            0x00000004
#define QML_SECC_INT_RD(src)                          (((src) & 0x00000004)>>2)
#define QML_SECC_INT_WR(src)                     (((u32)(src)<<2) & 0x00000004)
#define QML_SECC_INT_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields DECC_INT	 */
#define QML_DECC_INT_WIDTH                                                    1
#define QML_DECC_INT_SHIFT                                                    1
#define QML_DECC_INT_MASK                                            0x00000002
#define QML_DECC_INT_RD(src)                          (((src) & 0x00000002)>>1)
#define QML_DECC_INT_WR(src)                     (((u32)(src)<<1) & 0x00000002)
#define QML_DECC_INT_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields DBG_INT	 */
#define QML_DBG_INT_WIDTH                                                     1
#define QML_DBG_INT_SHIFT                                                     0
#define QML_DBG_INT_MASK                                             0x00000001
#define QML_DBG_INT_RD(src)                              (((src) & 0x00000001))
#define QML_DBG_INT_WR(src)                         (((u32)(src)) & 0x00000001)
#define QML_DBG_INT_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register DBG_ECC_INT_OUTMask	*/
/*    Mask Register Fields SLV_TMOMask    */
#define QML_SLV_TMOMASK_WIDTH                                                 1
#define QML_SLV_TMOMASK_SHIFT                                                 3
#define QML_SLV_TMOMASK_MASK                                         0x00000008
#define QML_SLV_TMOMASK_RD(src)                       (((src) & 0x00000008)>>3)
#define QML_SLV_TMOMASK_WR(src)                  (((u32)(src)<<3) & 0x00000008)
#define QML_SLV_TMOMASK_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*    Mask Register Fields SECC_INTMask    */
#define QML_SECC_INTMASK_WIDTH                                                1
#define QML_SECC_INTMASK_SHIFT                                                2
#define QML_SECC_INTMASK_MASK                                        0x00000004
#define QML_SECC_INTMASK_RD(src)                      (((src) & 0x00000004)>>2)
#define QML_SECC_INTMASK_WR(src)                 (((u32)(src)<<2) & 0x00000004)
#define QML_SECC_INTMASK_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*    Mask Register Fields DECC_INTMask    */
#define QML_DECC_INTMASK_WIDTH                                                1
#define QML_DECC_INTMASK_SHIFT                                                1
#define QML_DECC_INTMASK_MASK                                        0x00000002
#define QML_DECC_INTMASK_RD(src)                      (((src) & 0x00000002)>>1)
#define QML_DECC_INTMASK_WR(src)                 (((u32)(src)<<1) & 0x00000002)
#define QML_DECC_INTMASK_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*    Mask Register Fields DBG_INTMask    */
#define QML_DBG_INTMASK_WIDTH                                                 1
#define QML_DBG_INTMASK_SHIFT                                                 0
#define QML_DBG_INTMASK_MASK                                         0x00000001
#define QML_DBG_INTMASK_RD(src)                          (((src) & 0x00000001))
#define QML_DBG_INTMASK_WR(src)                     (((u32)(src)) & 0x00000001)
#define QML_DBG_INTMASK_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register CFG_SECC_INT_THRSH_REG	*/ 
/*	 Fields SECC_INT_THRSH	 */
#define QML_SECC_INT_THRSH_WIDTH                                              8
#define QML_SECC_INT_THRSH_SHIFT                                              0
#define QML_SECC_INT_THRSH_MASK                                      0x000000ff
#define QML_SECC_INT_THRSH_RD(src)                       (((src) & 0x000000ff))
#define QML_SECC_INT_THRSH_WR(src)                  (((u32)(src)) & 0x000000ff)
#define QML_SECC_INT_THRSH_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register CFG_MEM_ECC_BYPASS	*/ 
/*	 Fields CFG_ECC_BYPASS	 */
#define QML_CFG_ECC_BYPASS_WIDTH                                             16
#define QML_CFG_ECC_BYPASS_SHIFT                                              0
#define QML_CFG_ECC_BYPASS_MASK                                      0x0000ffff
#define QML_CFG_ECC_BYPASS_RD(src)                       (((src) & 0x0000ffff))
#define QML_CFG_ECC_BYPASS_WR(src)                  (((u32)(src)) & 0x0000ffff)
#define QML_CFG_ECC_BYPASS_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register CFG_MEM_PWRDN_DIS	*/ 
/*	 Fields CFG_ECC_PWRDN_DIS	 */
#define QML_CFG_ECC_PWRDN_DIS_WIDTH                                          16
#define QML_CFG_ECC_PWRDN_DIS_SHIFT                                          16
#define QML_CFG_ECC_PWRDN_DIS_MASK                                   0xffff0000
#define QML_CFG_ECC_PWRDN_DIS_RD(src)                (((src) & 0xffff0000)>>16)
#define QML_CFG_ECC_PWRDN_DIS_WR(src)           (((u32)(src)<<16) & 0xffff0000)
#define QML_CFG_ECC_PWRDN_DIS_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields CFG_PWRDN_DIS	 */
#define QML_CFG_PWRDN_DIS_WIDTH                                              16
#define QML_CFG_PWRDN_DIS_SHIFT                                               0
#define QML_CFG_PWRDN_DIS_MASK                                       0x0000ffff
#define QML_CFG_PWRDN_DIS_RD(src)                        (((src) & 0x0000ffff))
#define QML_CFG_PWRDN_DIS_WR(src)                   (((u32)(src)) & 0x0000ffff)
#define QML_CFG_PWRDN_DIS_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register CFG_MEM_RAM_SHUTDOWN	*/ 
/*	 Fields CFG_RAM_SHUTDOWN_EN	 */
#define QML_CFG_RAM_SHUTDOWN_EN_WIDTH                                        32
#define QML_CFG_RAM_SHUTDOWN_EN_SHIFT                                         0
#define QML_CFG_RAM_SHUTDOWN_EN_MASK                                 0xffffffff
#define QML_CFG_RAM_SHUTDOWN_EN_RD(src)                  (((src) & 0xffffffff))
#define QML_CFG_RAM_SHUTDOWN_EN_WR(src)             (((u32)(src)) & 0xffffffff)
#define QML_CFG_RAM_SHUTDOWN_EN_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register BLOCK_MEM_RDY	*/ 
/*	 Fields MEM_RDY	 */
#define QML_MEM_RDY_WIDTH                                                    32
#define QML_MEM_RDY_SHIFT                                                     0
#define QML_MEM_RDY_MASK                                             0xffffffff
#define QML_MEM_RDY_RD(src)                              (((src) & 0xffffffff))
#define QML_MEM_RDY_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_AXI_SLV_RD_TMO	*/ 
/*	 Fields SLV_ID_TMO_ENABLE	 */
#define QML_SLV_ID_TMO_ENABLE_WIDTH                                           1
#define QML_SLV_ID_TMO_ENABLE_SHIFT                                          31
#define QML_SLV_ID_TMO_ENABLE_MASK                                   0x80000000
#define QML_SLV_ID_TMO_ENABLE_RD(src)                (((src) & 0x80000000)>>31)
#define QML_SLV_ID_TMO_ENABLE_WR(src)           (((u32)(src)<<31) & 0x80000000)
#define QML_SLV_ID_TMO_ENABLE_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields SS_ID_TRK_ENABLE	 */
#define QML_SS_ID_TRK_ENABLE_WIDTH                                            1
#define QML_SS_ID_TRK_ENABLE_SHIFT                                           30
#define QML_SS_ID_TRK_ENABLE_MASK                                    0x40000000
#define QML_SS_ID_TRK_ENABLE_RD(src)                 (((src) & 0x40000000)>>30)
#define QML_SS_ID_TRK_ENABLE_WR(src)            (((u32)(src)<<30) & 0x40000000)
#define QML_SS_ID_TRK_ENABLE_SET(dst,src) \
                      (((dst) & ~0x40000000) | (((u32)(src)<<30) & 0x40000000))
/*	 Fields SLV_ID_TMO_VALUE	 */
#define QML_SLV_ID_TMO_VALUE_WIDTH                                           10
#define QML_SLV_ID_TMO_VALUE_SHIFT                                           16
#define QML_SLV_ID_TMO_VALUE_MASK                                    0x03ff0000
#define QML_SLV_ID_TMO_VALUE_RD(src)                 (((src) & 0x03ff0000)>>16)
#define QML_SLV_ID_TMO_VALUE_WR(src)            (((u32)(src)<<16) & 0x03ff0000)
#define QML_SLV_ID_TMO_VALUE_SET(dst,src) \
                      (((dst) & ~0x03ff0000) | (((u32)(src)<<16) & 0x03ff0000))
/*	 Fields SLV_RDY_TMO_ENABLE	 */
#define QML_SLV_RDY_TMO_ENABLE_WIDTH                                          1
#define QML_SLV_RDY_TMO_ENABLE_SHIFT                                         15
#define QML_SLV_RDY_TMO_ENABLE_MASK                                  0x00008000
#define QML_SLV_RDY_TMO_ENABLE_RD(src)               (((src) & 0x00008000)>>15)
#define QML_SLV_RDY_TMO_ENABLE_WR(src)          (((u32)(src)<<15) & 0x00008000)
#define QML_SLV_RDY_TMO_ENABLE_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))
/*	 Fields SLV_RDY_TMO_VALUE	 */
#define QML_SLV_RDY_TMO_VALUE_WIDTH                                          10
#define QML_SLV_RDY_TMO_VALUE_SHIFT                                           0
#define QML_SLV_RDY_TMO_VALUE_MASK                                   0x000003ff
#define QML_SLV_RDY_TMO_VALUE_RD(src)                    (((src) & 0x000003ff))
#define QML_SLV_RDY_TMO_VALUE_WR(src)               (((u32)(src)) & 0x000003ff)
#define QML_SLV_RDY_TMO_VALUE_SET(dst,src) \
                          (((dst) & ~0x000003ff) | (((u32)(src)) & 0x000003ff))

/*	Register CFG_AXI_SLV_WR_TMO	*/ 
/*	 Fields SLV_ID_TMO_ENABLE	 */
#define QML_SLV_ID_TMO_ENABLE_F1_WIDTH                                        1
#define QML_SLV_ID_TMO_ENABLE_F1_SHIFT                                       31
#define QML_SLV_ID_TMO_ENABLE_F1_MASK                                0x80000000
#define QML_SLV_ID_TMO_ENABLE_F1_RD(src)             (((src) & 0x80000000)>>31)
#define QML_SLV_ID_TMO_ENABLE_F1_WR(src)        (((u32)(src)<<31) & 0x80000000)
#define QML_SLV_ID_TMO_ENABLE_F1_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields SS_ID_TRK_ENABLE	 */
#define QML_SS_ID_TRK_ENABLE_F1_WIDTH                                         1
#define QML_SS_ID_TRK_ENABLE_F1_SHIFT                                        30
#define QML_SS_ID_TRK_ENABLE_F1_MASK                                 0x40000000
#define QML_SS_ID_TRK_ENABLE_F1_RD(src)              (((src) & 0x40000000)>>30)
#define QML_SS_ID_TRK_ENABLE_F1_WR(src)         (((u32)(src)<<30) & 0x40000000)
#define QML_SS_ID_TRK_ENABLE_F1_SET(dst,src) \
                      (((dst) & ~0x40000000) | (((u32)(src)<<30) & 0x40000000))
/*	 Fields SLV_ID_TMO_VALUE	 */
#define QML_SLV_ID_TMO_VALUE_F1_WIDTH                                        10
#define QML_SLV_ID_TMO_VALUE_F1_SHIFT                                        16
#define QML_SLV_ID_TMO_VALUE_F1_MASK                                 0x03ff0000
#define QML_SLV_ID_TMO_VALUE_F1_RD(src)              (((src) & 0x03ff0000)>>16)
#define QML_SLV_ID_TMO_VALUE_F1_WR(src)         (((u32)(src)<<16) & 0x03ff0000)
#define QML_SLV_ID_TMO_VALUE_F1_SET(dst,src) \
                      (((dst) & ~0x03ff0000) | (((u32)(src)<<16) & 0x03ff0000))
/*	 Fields SLV_RDY_TMO_ENABLE	 */
#define QML_SLV_RDY_TMO_ENABLE_F1_WIDTH                                       1
#define QML_SLV_RDY_TMO_ENABLE_F1_SHIFT                                      15
#define QML_SLV_RDY_TMO_ENABLE_F1_MASK                               0x00008000
#define QML_SLV_RDY_TMO_ENABLE_F1_RD(src)            (((src) & 0x00008000)>>15)
#define QML_SLV_RDY_TMO_ENABLE_F1_WR(src)       (((u32)(src)<<15) & 0x00008000)
#define QML_SLV_RDY_TMO_ENABLE_F1_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))
/*	 Fields SLV_RDY_TMO_VALUE	 */
#define QML_SLV_RDY_TMO_VALUE_F1_WIDTH                                       10
#define QML_SLV_RDY_TMO_VALUE_F1_SHIFT                                        0
#define QML_SLV_RDY_TMO_VALUE_F1_MASK                                0x000003ff
#define QML_SLV_RDY_TMO_VALUE_F1_RD(src)                 (((src) & 0x000003ff))
#define QML_SLV_RDY_TMO_VALUE_F1_WR(src)            (((u32)(src)) & 0x000003ff)
#define QML_SLV_RDY_TMO_VALUE_F1_SET(dst,src) \
                          (((dst) & ~0x000003ff) | (((u32)(src)) & 0x000003ff))

/*	Register STS_AXI_SLV_TMO	*/ 
/*	 Fields SLV_ARID_TMO	 */
#define QML_SLV_ARID_TMO_WIDTH                                                1
#define QML_SLV_ARID_TMO_SHIFT                                                3
#define QML_SLV_ARID_TMO_MASK                                        0x00000008
#define QML_SLV_ARID_TMO_RD(src)                      (((src) & 0x00000008)>>3)
#define QML_SLV_ARID_TMO_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields SLV_ARRDY_TMO	 */
#define QML_SLV_ARRDY_TMO_WIDTH                                               1
#define QML_SLV_ARRDY_TMO_SHIFT                                               2
#define QML_SLV_ARRDY_TMO_MASK                                       0x00000004
#define QML_SLV_ARRDY_TMO_RD(src)                     (((src) & 0x00000004)>>2)
#define QML_SLV_ARRDY_TMO_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields SLV_AWID_TMO	 */
#define QML_SLV_AWID_TMO_WIDTH                                                1
#define QML_SLV_AWID_TMO_SHIFT                                                1
#define QML_SLV_AWID_TMO_MASK                                        0x00000002
#define QML_SLV_AWID_TMO_RD(src)                      (((src) & 0x00000002)>>1)
#define QML_SLV_AWID_TMO_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields SLV_AWRDY_TMO	 */
#define QML_SLV_AWRDY_TMO_WIDTH                                               1
#define QML_SLV_AWRDY_TMO_SHIFT                                               0
#define QML_SLV_AWRDY_TMO_MASK                                       0x00000001
#define QML_SLV_AWRDY_TMO_RD(src)                        (((src) & 0x00000001))
#define QML_SLV_AWRDY_TMO_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register CFG_AXI_SS_CSR_TMO	*/ 
/*	 Fields CFG_CSR_TMO	 */
#define QML_CFG_CSR_TMO_WIDTH                                                16
#define QML_CFG_CSR_TMO_SHIFT                                                16
#define QML_CFG_CSR_TMO_MASK                                         0xffff0000
#define QML_CFG_CSR_TMO_RD(src)                      (((src) & 0xffff0000)>>16)
#define QML_CFG_CSR_TMO_WR(src)                 (((u32)(src)<<16) & 0xffff0000)
#define QML_CFG_CSR_TMO_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields CSR_ERR_RESP_EN	 */
#define QML_CSR_ERR_RESP_EN_WIDTH                                             1
#define QML_CSR_ERR_RESP_EN_SHIFT                                            15
#define QML_CSR_ERR_RESP_EN_MASK                                     0x00008000
#define QML_CSR_ERR_RESP_EN_RD(src)                  (((src) & 0x00008000)>>15)
#define QML_CSR_ERR_RESP_EN_WR(src)             (((u32)(src)<<15) & 0x00008000)
#define QML_CSR_ERR_RESP_EN_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))

/*	Register STS_READ_LATENCY_TOT_READ_REQS	*/ 
/*	 Fields TOT_READS	 */
#define QML_TOT_READS_WIDTH                                                  16
#define QML_TOT_READS_SHIFT                                                   0
#define QML_TOT_READS_MASK                                           0x0000ffff
#define QML_TOT_READS_RD(src)                            (((src) & 0x0000ffff))
#define QML_TOT_READS_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Global Base Address	*/
#define QML_CSR_BASE_ADDR			0xddd8d0000ULL

/*    Address QML_CSR  Registers */
#define CSR_QML_IPBRR_ADDR                                           0x00000000
#define CSR_QML_IPBRR_DEFAULT                                        0x0000006f
#define CSR_QM_CONFIG_ADDR                                           0x00000004
#define CSR_QM_CONFIG_DEFAULT                                        0x00000000
#define CSR_PBM_ADDR                                                 0x00000008
#define CSR_PBM_DEFAULT                                              0x00000000
#define CSR_PBM_BUF_WR_ADDR                                          0x0000000c
#define CSR_PBM_BUF_WR_DEFAULT                                       0x00000000
#define CSR_PBM_BUF_RD_ADDR                                          0x00000010
#define CSR_PBM_BUF_RD_DEFAULT                                       0x00000000
#define CSR_QSTATE_ADDR                                              0x00000064
#define CSR_QSTATE_DEFAULT                                           0x00000000
#define CSR_QSTATE_WR_0_ADDR                                         0x00000068
#define CSR_QSTATE_WR_0_DEFAULT                                      0x00000000
#define CSR_QSTATE_WR_1_ADDR                                         0x0000006c
#define CSR_QSTATE_WR_1_DEFAULT                                      0x00000000
#define CSR_QSTATE_WR_2_ADDR                                         0x00000070
#define CSR_QSTATE_WR_2_DEFAULT                                      0x00000000
#define CSR_QSTATE_WR_3_ADDR                                         0x00000074
#define CSR_QSTATE_WR_3_DEFAULT                                      0x00000000
#define CSR_QSTATE_RD_0_ADDR                                         0x00000078
#define CSR_QSTATE_RD_0_DEFAULT                                      0x00000000
#define CSR_QSTATE_RD_1_ADDR                                         0x0000007c
#define CSR_QSTATE_RD_1_DEFAULT                                      0x00000000
#define CSR_QSTATE_RD_2_ADDR                                         0x00000080
#define CSR_QSTATE_RD_2_DEFAULT                                      0x00000000
#define CSR_QSTATE_RD_3_ADDR                                         0x00000084
#define CSR_QSTATE_RD_3_DEFAULT                                      0x00000000
#define CSR_ENQ_BASE_A_ADDR                                          0x000000a4
#define CSR_ENQ_BASE_A_DEFAULT                                       0x00fffff4
#define CSR_ENQ_BASE_B_ADDR                                          0x000000a8
#define CSR_ENQ_BASE_B_DEFAULT                                       0x00fffff7
#define CSR_ENQ_STATUS_0_ADDR                                        0x000000ac
#define CSR_ENQ_STATUS_0_DEFAULT                                     0x00000000
#define CSR_ENQ_STATUS_1_ADDR                                        0x000000b0
#define CSR_ENQ_STATUS_1_DEFAULT                                     0x00000000
#define CSR_ENQ_STATUS_2_ADDR                                        0x000000b4
#define CSR_ENQ_STATUS_2_DEFAULT                                     0x00000000
#define CSR_ENQ_STATUS_3_ADDR                                        0x000000b8
#define CSR_ENQ_STATUS_3_DEFAULT                                     0x00000000
#define QM_INTERRUPT_ADDR                                            0x000000bc
#define QM_INTERRUPT_DEFAULT                                         0x00000000
#define QM_INTERRUPTMASK_ADDR                                        0x000000c0
#define CSR_PBM_ERRINF_ADDR                                          0x000000c4
#define CSR_PBM_ERRINF_DEFAULT                                       0x00000000
#define CSR_MSGRD_ERRINF_ADDR                                        0x000000c8
#define CSR_MSGRD_ERRINF_DEFAULT                                     0x00000000
#define CSR_QM_STATS_CFG_ADDR                                        0x00000110
#define CSR_QM_STATS_CFG_DEFAULT                                     0x00000000
#define CSR_ENQ_STATISTICS_ADDR                                      0x00000114
#define CSR_ENQ_STATISTICS_DEFAULT                                   0x00000000
#define CSR_DEQ_STATISTICS_ADDR                                      0x00000118
#define CSR_DEQ_STATISTICS_DEFAULT                                   0x00000000
#define CSR_FIFO_STATUS_ADDR                                         0x0000011c
#define CSR_FIFO_STATUS_DEFAULT                                      0x00000000
#define CSR_QML_REMAP0_ADDR                                          0x000003e0
#define CSR_QML_REMAP0_DEFAULT                                       0x00010203
#define CSR_QML_REMAP1_ADDR                                          0x000003e4
#define CSR_QML_REMAP1_DEFAULT                                       0x04050000

/*	Register csr_ipbrr	*/ 
/*	 Fields revno	 */
#define REVNO_F14_WIDTH                                                       2
#define REVNO_F14_SHIFT                                                       8
#define REVNO_F14_MASK                                               0x00000300
#define REVNO_F14_RD(src)                             (((src) & 0x00000300)>>8)
#define REVNO_F14_SET(dst,src) \
                       (((dst) & ~0x00000300) | (((u32)(src)<<8) & 0x00000300))
/*	 Fields busid	 */
#define BUSID_F12_WIDTH                                                       3
#define BUSID_F12_SHIFT                                                       5
#define BUSID_F12_MASK                                               0x000000e0
#define BUSID_F12_RD(src)                             (((src) & 0x000000e0)>>5)
#define BUSID_F12_SET(dst,src) \
                       (((dst) & ~0x000000e0) | (((u32)(src)<<5) & 0x000000e0))
/*	 Fields deviceid	 */
#define DEVICEID_F16_WIDTH                                                    5
#define DEVICEID_F16_SHIFT                                                    0
#define DEVICEID_F16_MASK                                            0x0000001f
#define DEVICEID_F16_RD(src)                             (((src) & 0x0000001f))
#define DEVICEID_F16_SET(dst,src) \
                          (((dst) & ~0x0000001f) | (((u32)(src)) & 0x0000001f))

/*	Register csr_qm_config	*/ 
/*	 Fields enable	 */
#define ENABLE_F4_WIDTH                                                       1
#define ENABLE_F4_SHIFT                                                      31
#define ENABLE_F4_MASK                                               0x80000000
#define ENABLE_F4_RD(src)                            (((src) & 0x80000000)>>31)
#define ENABLE_F4_WR(src)                       (((u32)(src)<<31) & 0x80000000)
#define ENABLE_F4_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields error_queue_enable	 */
#define ERROR_QUEUE_ENABLE_WIDTH                                              1
#define ERROR_QUEUE_ENABLE_SHIFT                                             30
#define ERROR_QUEUE_ENABLE_MASK                                      0x40000000
#define ERROR_QUEUE_ENABLE_RD(src)                   (((src) & 0x40000000)>>30)
#define ERROR_QUEUE_ENABLE_WR(src)              (((u32)(src)<<30) & 0x40000000)
#define ERROR_QUEUE_ENABLE_SET(dst,src) \
                      (((dst) & ~0x40000000) | (((u32)(src)<<30) & 0x40000000))
/*	 Fields error_qid	 */
#define ERROR_QID_WIDTH                                                       8
#define ERROR_QID_SHIFT                                                       0
#define ERROR_QID_MASK                                               0x000000ff
#define ERROR_QID_RD(src)                                (((src) & 0x000000ff))
#define ERROR_QID_WR(src)                           (((u32)(src)) & 0x000000ff)
#define ERROR_QID_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register csr_pbm	*/ 
/*	 Fields slvid_pbn	 */
#define SLVID_PBN_WIDTH                                                      10
#define SLVID_PBN_SHIFT                                                       0
#define SLVID_PBN_MASK                                               0x000003ff
#define SLVID_PBN_RD(src)                                (((src) & 0x000003ff))
#define SLVID_PBN_WR(src)                           (((u32)(src)) & 0x000003ff)
#define SLVID_PBN_SET(dst,src) \
                          (((dst) & ~0x000003ff) | (((u32)(src)) & 0x000003ff))

/*	Register csr_pbm_buf_wr	*/ 
/*	 Fields overwrite	 */
#define OVERWRITE_F2_WIDTH                                                    1
#define OVERWRITE_F2_SHIFT                                                   31
#define OVERWRITE_F2_MASK                                            0x80000000
#define OVERWRITE_F2_RD(src)                         (((src) & 0x80000000)>>31)
#define OVERWRITE_F2_WR(src)                    (((u32)(src)<<31) & 0x80000000)
#define OVERWRITE_F2_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields in_service_clr	 */
#define IN_SERVICE_CLR_WIDTH                                                  1
#define IN_SERVICE_CLR_SHIFT                                                 18
#define IN_SERVICE_CLR_MASK                                          0x00040000
#define IN_SERVICE_CLR_RD(src)                       (((src) & 0x00040000)>>18)
#define IN_SERVICE_CLR_WR(src)                  (((u32)(src)<<18) & 0x00040000)
#define IN_SERVICE_CLR_SET(dst,src) \
                      (((dst) & ~0x00040000) | (((u32)(src)<<18) & 0x00040000))
/*	 Fields data	 */
#define DATA_F2_WIDTH                                                        18
#define DATA_F2_SHIFT                                                         0
#define DATA_F2_MASK                                                 0x0003ffff
#define DATA_F2_RD(src)                                  (((src) & 0x0003ffff))
#define DATA_F2_WR(src)                             (((u32)(src)) & 0x0003ffff)
#define DATA_F2_SET(dst,src) \
                          (((dst) & ~0x0003ffff) | (((u32)(src)) & 0x0003ffff))

/*	Register csr_pbm_buf_rd	*/ 
/*	 Fields pb_in_service	 */
#define PB_IN_SERVICE_WIDTH                                                   1
#define PB_IN_SERVICE_SHIFT                                                  18
#define PB_IN_SERVICE_MASK                                           0x00040000
#define PB_IN_SERVICE_RD(src)                        (((src) & 0x00040000)>>18)
#define PB_IN_SERVICE_SET(dst,src) \
                      (((dst) & ~0x00040000) | (((u32)(src)<<18) & 0x00040000))
/*	 Fields slot_num	 */
#define SLOT_NUM_WIDTH                                                        3
#define SLOT_NUM_SHIFT                                                       15
#define SLOT_NUM_MASK                                                0x00038000
#define SLOT_NUM_RD(src)                             (((src) & 0x00038000)>>15)
#define SLOT_NUM_SET(dst,src) \
                      (((dst) & ~0x00038000) | (((u32)(src)<<15) & 0x00038000))
/*	 Fields prefetch_buf_en	 */
#define PREFETCH_BUF_EN_WIDTH                                                 1
#define PREFETCH_BUF_EN_SHIFT                                                14
#define PREFETCH_BUF_EN_MASK                                         0x00004000
#define PREFETCH_BUF_EN_RD(src)                      (((src) & 0x00004000)>>14)
#define PREFETCH_BUF_EN_SET(dst,src) \
                      (((dst) & ~0x00004000) | (((u32)(src)<<14) & 0x00004000))
/*	 Fields is_free_pool	 */
#define IS_FREE_POOL_WIDTH                                                    1
#define IS_FREE_POOL_SHIFT                                                   13
#define IS_FREE_POOL_MASK                                            0x00002000
#define IS_FREE_POOL_RD(src)                         (((src) & 0x00002000)>>13)
#define IS_FREE_POOL_SET(dst,src) \
                      (((dst) & ~0x00002000) | (((u32)(src)<<13) & 0x00002000))
/*	 Fields tlvq	 */
#define TLVQ_WIDTH                                                            1
#define TLVQ_SHIFT                                                           12
#define TLVQ_MASK                                                    0x00001000
#define TLVQ_RD(src)                                 (((src) & 0x00001000)>>12)
#define TLVQ_SET(dst,src) \
                      (((dst) & ~0x00001000) | (((u32)(src)<<12) & 0x00001000))
/*	 Fields corresponding_qnum	 */
#define CORRESPONDING_QNUM_WIDTH                                              8
#define CORRESPONDING_QNUM_SHIFT                                              4
#define CORRESPONDING_QNUM_MASK                                      0x00000ff0
#define CORRESPONDING_QNUM_RD(src)                    (((src) & 0x00000ff0)>>4)
#define CORRESPONDING_QNUM_SET(dst,src) \
                       (((dst) & ~0x00000ff0) | (((u32)(src)<<4) & 0x00000ff0))
/*	 Fields num_msgs_in_buf	 */
#define NUM_MSGS_IN_BUF_WIDTH                                                 4
#define NUM_MSGS_IN_BUF_SHIFT                                                 0
#define NUM_MSGS_IN_BUF_MASK                                         0x0000000f
#define NUM_MSGS_IN_BUF_RD(src)                          (((src) & 0x0000000f))
#define NUM_MSGS_IN_BUF_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register csr_qstate	*/ 
/*	 Fields qnumber	 */
#define QNUMBER_WIDTH                                                         8
#define QNUMBER_SHIFT                                                         0
#define QNUMBER_MASK                                                 0x000000ff
#define QNUMBER_RD(src)                                  (((src) & 0x000000ff))
#define QNUMBER_WR(src)                             (((u32)(src)) & 0x000000ff)
#define QNUMBER_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register csr_qstate_wr_0	*/ 
/*	 Fields data	 */
#define DATA0_F8_WIDTH                                                       32
#define DATA0_F8_SHIFT                                                        0
#define DATA0_F8_MASK                                                0xffffffff
#define DATA0_F8_RD(src)                                 (((src) & 0xffffffff))
#define DATA0_F8_WR(src)                            (((u32)(src)) & 0xffffffff)
#define DATA0_F8_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register csr_qstate_wr_1	*/ 
/*	 Fields data	 */
#define DATA1_F8_WIDTH                                                       32
#define DATA1_F8_SHIFT                                                        0
#define DATA1_F8_MASK                                                0xffffffff
#define DATA1_F8_RD(src)                                 (((src) & 0xffffffff))
#define DATA1_F8_WR(src)                            (((u32)(src)) & 0xffffffff)
#define DATA1_F8_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register csr_qstate_wr_2	*/ 
/*	 Fields data	 */
#define DATA2_F8_WIDTH                                                       32
#define DATA2_F8_SHIFT                                                        0
#define DATA2_F8_MASK                                                0xffffffff
#define DATA2_F8_RD(src)                                 (((src) & 0xffffffff))
#define DATA2_F8_WR(src)                            (((u32)(src)) & 0xffffffff)
#define DATA2_F8_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register csr_qstate_wr_3	*/ 
/*	 Fields data	 */
#define DATA3_F4_WIDTH                                                       32
#define DATA3_F4_SHIFT                                                        0
#define DATA3_F4_MASK                                                0xffffffff
#define DATA3_F4_RD(src)                                 (((src) & 0xffffffff))
#define DATA3_F4_WR(src)                            (((u32)(src)) & 0xffffffff)
#define DATA3_F4_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register csr_qstate_rd_0	*/ 
/*	 Fields data	 */
#define DATA0_F9_WIDTH                                                       32
#define DATA0_F9_SHIFT                                                        0
#define DATA0_F9_MASK                                                0xffffffff
#define DATA0_F9_RD(src)                                 (((src) & 0xffffffff))
#define DATA0_F9_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register csr_qstate_rd_1	*/ 
/*	 Fields data	 */
#define DATA1_F9_WIDTH                                                       32
#define DATA1_F9_SHIFT                                                        0
#define DATA1_F9_MASK                                                0xffffffff
#define DATA1_F9_RD(src)                                 (((src) & 0xffffffff))
#define DATA1_F9_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register csr_qstate_rd_2	*/ 
/*	 Fields data	 */
#define DATA2_F9_WIDTH                                                       32
#define DATA2_F9_SHIFT                                                        0
#define DATA2_F9_MASK                                                0xffffffff
#define DATA2_F9_RD(src)                                 (((src) & 0xffffffff))
#define DATA2_F9_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register csr_qstate_rd_3	*/ 
/*	 Fields data	 */
#define DATA3_F5_WIDTH                                                       32
#define DATA3_F5_SHIFT                                                        0
#define DATA3_F5_MASK                                                0xffffffff
#define DATA3_F5_RD(src)                                 (((src) & 0xffffffff))
#define DATA3_F5_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register csr_enq_base_a	*/ 
/*	 Fields coherent	 */
#define COHERENT_WIDTH                                                        1
#define COHERENT_SHIFT                                                       31
#define COHERENT_MASK                                                0x80000000
#define COHERENT_RD(src)                             (((src) & 0x80000000)>>31)
#define COHERENT_WR(src)                        (((u32)(src)<<31) & 0x80000000)
#define COHERENT_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields addr	 */
#define ADDR_F10_WIDTH                                                       26
#define ADDR_F10_SHIFT                                                        0
#define ADDR_F10_MASK                                                0x03ffffff
#define ADDR_F10_RD(src)                                 (((src) & 0x03ffffff))
#define ADDR_F10_WR(src)                            (((u32)(src)) & 0x03ffffff)
#define ADDR_F10_SET(dst,src) \
                          (((dst) & ~0x03ffffff) | (((u32)(src)) & 0x03ffffff))

/*	Register csr_enq_base_b	*/ 
/*	 Fields coherent	 */
#define COHERENT_F1_WIDTH                                                     1
#define COHERENT_F1_SHIFT                                                    31
#define COHERENT_F1_MASK                                             0x80000000
#define COHERENT_F1_RD(src)                          (((src) & 0x80000000)>>31)
#define COHERENT_F1_WR(src)                     (((u32)(src)<<31) & 0x80000000)
#define COHERENT_F1_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields addr	 */
#define ADDR_F11_WIDTH                                                       26
#define ADDR_F11_SHIFT                                                        0
#define ADDR_F11_MASK                                                0x03ffffff
#define ADDR_F11_RD(src)                                 (((src) & 0x03ffffff))
#define ADDR_F11_WR(src)                            (((u32)(src)) & 0x03ffffff)
#define ADDR_F11_SET(dst,src) \
                          (((dst) & ~0x03ffffff) | (((u32)(src)) & 0x03ffffff))

/*	Register csr_enq_status_0	*/ 
/*	 Fields mbox_0	 */
#define MBOX_00_WIDTH                                                         8
#define MBOX_00_SHIFT                                                        24
#define MBOX_00_MASK                                                 0xff000000
#define MBOX_00_RD(src)                              (((src) & 0xff000000)>>24)
#define MBOX_00_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))
/*	 Fields mbox_1	 */
#define MBOX_10_WIDTH                                                         8
#define MBOX_10_SHIFT                                                        16
#define MBOX_10_MASK                                                 0x00ff0000
#define MBOX_10_RD(src)                              (((src) & 0x00ff0000)>>16)
#define MBOX_10_SET(dst,src) \
                      (((dst) & ~0x00ff0000) | (((u32)(src)<<16) & 0x00ff0000))
/*	 Fields mbox_2	 */
#define MBOX_20_WIDTH                                                         8
#define MBOX_20_SHIFT                                                         8
#define MBOX_20_MASK                                                 0x0000ff00
#define MBOX_20_RD(src)                               (((src) & 0x0000ff00)>>8)
#define MBOX_20_SET(dst,src) \
                       (((dst) & ~0x0000ff00) | (((u32)(src)<<8) & 0x0000ff00))
/*	 Fields mbox_3	 */
#define MBOX_30_WIDTH                                                         8
#define MBOX_30_SHIFT                                                         0
#define MBOX_30_MASK                                                 0x000000ff
#define MBOX_30_RD(src)                                  (((src) & 0x000000ff))
#define MBOX_30_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register csr_enq_status_1	*/ 
/*	 Fields mbox_4	 */
#define MBOX_41_WIDTH                                                         8
#define MBOX_41_SHIFT                                                        24
#define MBOX_41_MASK                                                 0xff000000
#define MBOX_41_RD(src)                              (((src) & 0xff000000)>>24)
#define MBOX_41_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))
/*	 Fields mbox_5	 */
#define MBOX_51_WIDTH                                                         8
#define MBOX_51_SHIFT                                                        16
#define MBOX_51_MASK                                                 0x00ff0000
#define MBOX_51_RD(src)                              (((src) & 0x00ff0000)>>16)
#define MBOX_51_SET(dst,src) \
                      (((dst) & ~0x00ff0000) | (((u32)(src)<<16) & 0x00ff0000))
/*	 Fields mbox_6	 */
#define MBOX_61_WIDTH                                                         8
#define MBOX_61_SHIFT                                                         8
#define MBOX_61_MASK                                                 0x0000ff00
#define MBOX_61_RD(src)                               (((src) & 0x0000ff00)>>8)
#define MBOX_61_SET(dst,src) \
                       (((dst) & ~0x0000ff00) | (((u32)(src)<<8) & 0x0000ff00))
/*	 Fields mbox_7	 */
#define MBOX_71_WIDTH                                                         8
#define MBOX_71_SHIFT                                                         0
#define MBOX_71_MASK                                                 0x000000ff
#define MBOX_71_RD(src)                                  (((src) & 0x000000ff))
#define MBOX_71_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register csr_enq_status_2	*/ 
/*	 Fields mbox_8	 */
#define MBOX_82_WIDTH                                                         8
#define MBOX_82_SHIFT                                                        24
#define MBOX_82_MASK                                                 0xff000000
#define MBOX_82_RD(src)                              (((src) & 0xff000000)>>24)
#define MBOX_82_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))
/*	 Fields mbox_9	 */
#define MBOX_92_WIDTH                                                         8
#define MBOX_92_SHIFT                                                        16
#define MBOX_92_MASK                                                 0x00ff0000
#define MBOX_92_RD(src)                              (((src) & 0x00ff0000)>>16)
#define MBOX_92_SET(dst,src) \
                      (((dst) & ~0x00ff0000) | (((u32)(src)<<16) & 0x00ff0000))
/*	 Fields mbox_10	 */
#define MBOX_102_WIDTH                                                        8
#define MBOX_102_SHIFT                                                        8
#define MBOX_102_MASK                                                0x0000ff00
#define MBOX_102_RD(src)                              (((src) & 0x0000ff00)>>8)
#define MBOX_102_SET(dst,src) \
                       (((dst) & ~0x0000ff00) | (((u32)(src)<<8) & 0x0000ff00))
/*	 Fields mbox_11	 */
#define MBOX_112_WIDTH                                                        8
#define MBOX_112_SHIFT                                                        0
#define MBOX_112_MASK                                                0x000000ff
#define MBOX_112_RD(src)                                 (((src) & 0x000000ff))
#define MBOX_112_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register csr_enq_status_3	*/ 
/*	 Fields mbox_12	 */
#define MBOX_123_WIDTH                                                        8
#define MBOX_123_SHIFT                                                       24
#define MBOX_123_MASK                                                0xff000000
#define MBOX_123_RD(src)                             (((src) & 0xff000000)>>24)
#define MBOX_123_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))
/*	 Fields mbox_13	 */
#define MBOX_133_WIDTH                                                        8
#define MBOX_133_SHIFT                                                       16
#define MBOX_133_MASK                                                0x00ff0000
#define MBOX_133_RD(src)                             (((src) & 0x00ff0000)>>16)
#define MBOX_133_SET(dst,src) \
                      (((dst) & ~0x00ff0000) | (((u32)(src)<<16) & 0x00ff0000))
/*	 Fields mbox_14	 */
#define MBOX_143_WIDTH                                                        8
#define MBOX_143_SHIFT                                                        8
#define MBOX_143_MASK                                                0x0000ff00
#define MBOX_143_RD(src)                              (((src) & 0x0000ff00)>>8)
#define MBOX_143_SET(dst,src) \
                       (((dst) & ~0x0000ff00) | (((u32)(src)<<8) & 0x0000ff00))
/*	 Fields mbox_15	 */
#define MBOX_153_WIDTH                                                        8
#define MBOX_153_SHIFT                                                        0
#define MBOX_153_MASK                                                0x000000ff
#define MBOX_153_RD(src)                                 (((src) & 0x000000ff))
#define MBOX_153_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register qm_INT	*/ 
/*	 Fields qpcore_acr_error	 */
#define QPCORE_ACR_ERROR_WIDTH                                                1
#define QPCORE_ACR_ERROR_SHIFT                                                2
#define QPCORE_ACR_ERROR_MASK                                        0x00000004
#define QPCORE_ACR_ERROR_RD(src)                      (((src) & 0x00000004)>>2)
#define QPCORE_ACR_ERROR_WR(src)                 (((u32)(src)<<2) & 0x00000004)
#define QPCORE_ACR_ERROR_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields deq_axi_error	 */
#define DEQ_AXI_ERROR_WIDTH                                                   1
#define DEQ_AXI_ERROR_SHIFT                                                   1
#define DEQ_AXI_ERROR_MASK                                           0x00000002
#define DEQ_AXI_ERROR_RD(src)                         (((src) & 0x00000002)>>1)
#define DEQ_AXI_ERROR_WR(src)                    (((u32)(src)<<1) & 0x00000002)
#define DEQ_AXI_ERROR_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields pbm_dec_error	 */
#define PBM_DEC_ERROR_WIDTH                                                   1
#define PBM_DEC_ERROR_SHIFT                                                   0
#define PBM_DEC_ERROR_MASK                                           0x00000001
#define PBM_DEC_ERROR_RD(src)                            (((src) & 0x00000001))
#define PBM_DEC_ERROR_WR(src)                       (((u32)(src)) & 0x00000001)
#define PBM_DEC_ERROR_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register qm_interruptMask	*/
/*    Mask Register Fields qpcore_acr_errorMask    */
#define QPCORE_ACR_ERRORMASK_WIDTH                                            1
#define QPCORE_ACR_ERRORMASK_SHIFT                                            2
#define QPCORE_ACR_ERRORMASK_MASK                                    0x00000004
#define QPCORE_ACR_ERRORMASK_RD(src)                  (((src) & 0x00000004)>>2)
#define QPCORE_ACR_ERRORMASK_WR(src)             (((u32)(src)<<2) & 0x00000004)
#define QPCORE_ACR_ERRORMASK_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*    Mask Register Fields deq_axi_errorMask    */
#define DEQ_AXI_ERRORMASK_WIDTH                                               1
#define DEQ_AXI_ERRORMASK_SHIFT                                               1
#define DEQ_AXI_ERRORMASK_MASK                                       0x00000002
#define DEQ_AXI_ERRORMASK_RD(src)                     (((src) & 0x00000002)>>1)
#define DEQ_AXI_ERRORMASK_WR(src)                (((u32)(src)<<1) & 0x00000002)
#define DEQ_AXI_ERRORMASK_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*    Mask Register Fields pbm_dec_errorMask    */
#define PBM_DEC_ERRORMASK_WIDTH                                               1
#define PBM_DEC_ERRORMASK_SHIFT                                               0
#define PBM_DEC_ERRORMASK_MASK                                       0x00000001
#define PBM_DEC_ERRORMASK_RD(src)                        (((src) & 0x00000001))
#define PBM_DEC_ERRORMASK_WR(src)                   (((u32)(src)) & 0x00000001)
#define PBM_DEC_ERRORMASK_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register csr_pbm_errinf	*/ 
/*	 Fields pbm_decrement_err	 */
#define PBM_DECREMENT_ERR_WIDTH                                               1
#define PBM_DECREMENT_ERR_SHIFT                                              16
#define PBM_DECREMENT_ERR_MASK                                       0x00010000
#define PBM_DECREMENT_ERR_RD(src)                    (((src) & 0x00010000)>>16)
#define PBM_DECREMENT_ERR_SET(dst,src) \
                      (((dst) & ~0x00010000) | (((u32)(src)<<16) & 0x00010000))
/*	 Fields slave_id	 */
#define SLAVE_ID_WIDTH                                                        4
#define SLAVE_ID_SHIFT                                                        6
#define SLAVE_ID_MASK                                                0x000003c0
#define SLAVE_ID_RD(src)                              (((src) & 0x000003c0)>>6)
#define SLAVE_ID_SET(dst,src) \
                       (((dst) & ~0x000003c0) | (((u32)(src)<<6) & 0x000003c0))
/*	 Fields PB_number	 */
#define PB_NUMBER_WIDTH                                                       6
#define PB_NUMBER_SHIFT                                                       0
#define PB_NUMBER_MASK                                               0x0000003f
#define PB_NUMBER_RD(src)                                (((src) & 0x0000003f))
#define PB_NUMBER_SET(dst,src) \
                          (((dst) & ~0x0000003f) | (((u32)(src)) & 0x0000003f))

/*	Register csr_msgrd_errinf	*/ 
/*	 Fields qpcore_was_disabled	 */
#define QPCORE_WAS_DISABLED_WIDTH                                             1
#define QPCORE_WAS_DISABLED_SHIFT                                            27
#define QPCORE_WAS_DISABLED_MASK                                     0x08000000
#define QPCORE_WAS_DISABLED_RD(src)                  (((src) & 0x08000000)>>27)
#define QPCORE_WAS_DISABLED_SET(dst,src) \
                      (((dst) & ~0x08000000) | (((u32)(src)<<27) & 0x08000000))
/*	 Fields drop_code	 */
#define DROP_CODE_WIDTH                                                       3
#define DROP_CODE_SHIFT                                                      24
#define DROP_CODE_MASK                                               0x07000000
#define DROP_CODE_RD(src)                            (((src) & 0x07000000)>>24)
#define DROP_CODE_SET(dst,src) \
                      (((dst) & ~0x07000000) | (((u32)(src)<<24) & 0x07000000))
/*	 Fields acr_qid	 */
#define ACR_QID_WIDTH                                                         8
#define ACR_QID_SHIFT                                                        16
#define ACR_QID_MASK                                                 0x00ff0000
#define ACR_QID_RD(src)                              (((src) & 0x00ff0000)>>16)
#define ACR_QID_SET(dst,src) \
                      (((dst) & ~0x00ff0000) | (((u32)(src)<<16) & 0x00ff0000))
/*	 Fields response_code	 */
#define RESPONSE_CODE_WIDTH                                                   2
#define RESPONSE_CODE_SHIFT                                                   8
#define RESPONSE_CODE_MASK                                           0x00000300
#define RESPONSE_CODE_RD(src)                         (((src) & 0x00000300)>>8)
#define RESPONSE_CODE_SET(dst,src) \
                       (((dst) & ~0x00000300) | (((u32)(src)<<8) & 0x00000300))
/*	 Fields qid	 */
#define QID_WIDTH                                                             8
#define QID_SHIFT                                                             0
#define QID_MASK                                                     0x000000ff
#define QID_RD(src)                                      (((src) & 0x000000ff))
#define QID_SET(dst,src) (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register csr_qm_stats_cfg	*/ 
/*	 Fields qid_enq_counter	 */
#define QID_ENQ_COUNTER_WIDTH                                                 8
#define QID_ENQ_COUNTER_SHIFT                                                24
#define QID_ENQ_COUNTER_MASK                                         0xff000000
#define QID_ENQ_COUNTER_RD(src)                      (((src) & 0xff000000)>>24)
#define QID_ENQ_COUNTER_WR(src)                 (((u32)(src)<<24) & 0xff000000)
#define QID_ENQ_COUNTER_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))
/*	 Fields qid_deq_counter	 */
#define QID_DEQ_COUNTER_WIDTH                                                 8
#define QID_DEQ_COUNTER_SHIFT                                                16
#define QID_DEQ_COUNTER_MASK                                         0x00ff0000
#define QID_DEQ_COUNTER_RD(src)                      (((src) & 0x00ff0000)>>16)
#define QID_DEQ_COUNTER_WR(src)                 (((u32)(src)<<16) & 0x00ff0000)
#define QID_DEQ_COUNTER_SET(dst,src) \
                      (((dst) & ~0x00ff0000) | (((u32)(src)<<16) & 0x00ff0000))

/*	Register csr_enq_statistics	*/ 
/*	 Fields enq_count	 */
#define ENQ_COUNT_WIDTH                                                      32
#define ENQ_COUNT_SHIFT                                                       0
#define ENQ_COUNT_MASK                                               0xffffffff
#define ENQ_COUNT_RD(src)                                (((src) & 0xffffffff))
#define ENQ_COUNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register csr_deq_statistics	*/ 
/*	 Fields deq_count	 */
#define DEQ_COUNT_WIDTH                                                      32
#define DEQ_COUNT_SHIFT                                                       0
#define DEQ_COUNT_MASK                                               0xffffffff
#define DEQ_COUNT_RD(src)                                (((src) & 0xffffffff))
#define DEQ_COUNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register csr_fifo_status	*/ 
/*	 Fields csr_qpcore_active_process	 */
#define CSR_QPCORE_ACTIVE_PROCESS_WIDTH                                       5
#define CSR_QPCORE_ACTIVE_PROCESS_SHIFT                                      22
#define CSR_QPCORE_ACTIVE_PROCESS_MASK                               0x07c00000
#define CSR_QPCORE_ACTIVE_PROCESS_RD(src)            (((src) & 0x07c00000)>>22)
#define CSR_QPCORE_ACTIVE_PROCESS_SET(dst,src) \
                      (((dst) & ~0x07c00000) | (((u32)(src)<<22) & 0x07c00000))
/*	 Fields csr_msgwr_outstanding_req	 */
#define CSR_MSGWR_OUTSTANDING_REQ_WIDTH                                       3
#define CSR_MSGWR_OUTSTANDING_REQ_SHIFT                                      19
#define CSR_MSGWR_OUTSTANDING_REQ_MASK                               0x00380000
#define CSR_MSGWR_OUTSTANDING_REQ_RD(src)            (((src) & 0x00380000)>>19)
#define CSR_MSGWR_OUTSTANDING_REQ_SET(dst,src) \
                      (((dst) & ~0x00380000) | (((u32)(src)<<19) & 0x00380000))
/*	 Fields csr_msgrd_fifo_stat	 */
#define CSR_MSGRD_FIFO_STAT_WIDTH                                             7
#define CSR_MSGRD_FIFO_STAT_SHIFT                                            12
#define CSR_MSGRD_FIFO_STAT_MASK                                     0x0007f000
#define CSR_MSGRD_FIFO_STAT_RD(src)                  (((src) & 0x0007f000)>>12)
#define CSR_MSGRD_FIFO_STAT_SET(dst,src) \
                      (((dst) & ~0x0007f000) | (((u32)(src)<<12) & 0x0007f000))
/*	 Fields csr_acr_mstr_cmd_level	 */
#define CSR_ACR_MSTR_CMD_LEVEL_WIDTH                                          4
#define CSR_ACR_MSTR_CMD_LEVEL_SHIFT                                          8
#define CSR_ACR_MSTR_CMD_LEVEL_MASK                                  0x00000f00
#define CSR_ACR_MSTR_CMD_LEVEL_RD(src)                (((src) & 0x00000f00)>>8)
#define CSR_ACR_MSTR_CMD_LEVEL_SET(dst,src) \
                       (((dst) & ~0x00000f00) | (((u32)(src)<<8) & 0x00000f00))
/*	 Fields csr_acr_allocated	 */
#define CSR_ACR_ALLOCATED_WIDTH                                               8
#define CSR_ACR_ALLOCATED_SHIFT                                               0
#define CSR_ACR_ALLOCATED_MASK                                       0x000000ff
#define CSR_ACR_ALLOCATED_RD(src)                        (((src) & 0x000000ff))
#define CSR_ACR_ALLOCATED_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register csr_qml_remap0	*/ 
/*	 Fields qid0	 */
#define QID00_WIDTH                                                           8
#define QID00_SHIFT                                                          24
#define QID00_MASK                                                   0xff000000
#define QID00_RD(src)                                (((src) & 0xff000000)>>24)
#define QID00_WR(src)                           (((u32)(src)<<24) & 0xff000000)
#define QID00_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))
/*	 Fields qid1	 */
#define QID10_WIDTH                                                           8
#define QID10_SHIFT                                                          16
#define QID10_MASK                                                   0x00ff0000
#define QID10_RD(src)                                (((src) & 0x00ff0000)>>16)
#define QID10_WR(src)                           (((u32)(src)<<16) & 0x00ff0000)
#define QID10_SET(dst,src) \
                      (((dst) & ~0x00ff0000) | (((u32)(src)<<16) & 0x00ff0000))
/*	 Fields qid2	 */
#define QID20_WIDTH                                                           8
#define QID20_SHIFT                                                           8
#define QID20_MASK                                                   0x0000ff00
#define QID20_RD(src)                                 (((src) & 0x0000ff00)>>8)
#define QID20_WR(src)                            (((u32)(src)<<8) & 0x0000ff00)
#define QID20_SET(dst,src) \
                       (((dst) & ~0x0000ff00) | (((u32)(src)<<8) & 0x0000ff00))
/*	 Fields qid3	 */
#define QID30_WIDTH                                                           8
#define QID30_SHIFT                                                           0
#define QID30_MASK                                                   0x000000ff
#define QID30_RD(src)                                    (((src) & 0x000000ff))
#define QID30_WR(src)                               (((u32)(src)) & 0x000000ff)
#define QID30_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register csr_qml_remap1	*/ 
/*	 Fields qid4	 */
#define QID41_WIDTH                                                           8
#define QID41_SHIFT                                                          24
#define QID41_MASK                                                   0xff000000
#define QID41_RD(src)                                (((src) & 0xff000000)>>24)
#define QID41_WR(src)                           (((u32)(src)<<24) & 0xff000000)
#define QID41_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))
/*	 Fields qid5	 */
#define QID51_WIDTH                                                           8
#define QID51_SHIFT                                                          16
#define QID51_MASK                                                   0x00ff0000
#define QID51_RD(src)                                (((src) & 0x00ff0000)>>16)
#define QID51_WR(src)                           (((u32)(src)<<16) & 0x00ff0000)
#define QID51_SET(dst,src) \
                      (((dst) & ~0x00ff0000) | (((u32)(src)<<16) & 0x00ff0000))
	
#else
/* For APM867xx or higher */

/*	Global Base Address	*/
#define QML_CSR_BASE_ADDR			0xddd8d0000ULL

/*    Address QML_CSR  Registers */
#define CSR_IPBRR_ADDR                                               0x00000000
#define CSR_IPBRR_DEFAULT                                            0x0000006f
#define CSR_QM_CONFIG_ADDR                                           0x00000004
#define CSR_QM_CONFIG_DEFAULT                                        0x00000000
#define CSR_PBM_ADDR                                                 0x00000008
#define CSR_PBM_DEFAULT                                              0x00000000
#define CSR_PBM_BUF_WR_ADDR                                          0x0000000c
#define CSR_PBM_BUF_WR_DEFAULT                                       0x00000000
#define CSR_PBM_BUF_RD_ADDR                                          0x00000010
#define CSR_PBM_BUF_RD_DEFAULT                                       0x00000000
#define CSR_QSTATE_ADDR                                              0x00000064
#define CSR_QSTATE_DEFAULT                                           0x00000000
#define CSR_QSTATE_WR_0_ADDR                                         0x00000068
#define CSR_QSTATE_WR_0_DEFAULT                                      0x00000000
#define CSR_QSTATE_WR_1_ADDR                                         0x0000006c
#define CSR_QSTATE_WR_1_DEFAULT                                      0x00000000
#define CSR_QSTATE_WR_2_ADDR                                         0x00000070
#define CSR_QSTATE_WR_2_DEFAULT                                      0x00000000
#define CSR_QSTATE_WR_3_ADDR                                         0x00000074
#define CSR_QSTATE_WR_3_DEFAULT                                      0x00000000
#define CSR_QSTATE_WR_4_ADDR                                         0x00000078
#define CSR_QSTATE_WR_4_DEFAULT                                      0x00000000
#define CSR_QSTATE_RD_0_ADDR                                         0x0000007c
#define CSR_QSTATE_RD_0_DEFAULT                                      0x00000000
#define CSR_QSTATE_RD_1_ADDR                                         0x00000080
#define CSR_QSTATE_RD_1_DEFAULT                                      0x00000000
#define CSR_QSTATE_RD_2_ADDR                                         0x00000084
#define CSR_QSTATE_RD_2_DEFAULT                                      0x00000000
#define CSR_QSTATE_RD_3_ADDR                                         0x00000088
#define CSR_QSTATE_RD_3_DEFAULT                                      0x00000000
#define CSR_QSTATE_RD_4_ADDR                                         0x0000008c
#define CSR_QSTATE_RD_4_DEFAULT                                      0x00000000
#define CSR_ENQ_BASE_A_ADDR                                          0x000000b4
#define CSR_ENQ_BASE_A_DEFAULT                                       0x00fffff4
#define CSR_ENQ_BASE_B_ADDR                                          0x000000b8
#define CSR_ENQ_BASE_B_DEFAULT                                       0x00fffff7
#define CSR_ENQ_STATUS_0_ADDR                                        0x000000bc
#define CSR_ENQ_STATUS_0_DEFAULT                                     0x00000000
#define CSR_ENQ_STATUS_1_ADDR                                        0x000000c0
#define CSR_ENQ_STATUS_1_DEFAULT                                     0x00000000
#define CSR_ENQ_STATUS_2_ADDR                                        0x000000c4
#define CSR_ENQ_STATUS_2_DEFAULT                                     0x00000000
#define CSR_ENQ_STATUS_3_ADDR                                        0x000000c8
#define CSR_ENQ_STATUS_3_DEFAULT                                     0x00000000
#define QM_INTERRUPT_ADDR                                            0x000000cc
#define QM_INTERRUPT_DEFAULT                                         0x00000000
#define QM_INTERRUPTMASK_ADDR                                        0x000000d0
#define CSR_PBM_ERRINF_ADDR                                          0x000000d4
#define CSR_PBM_ERRINF_DEFAULT                                       0x00000000
#define CSR_MSGRD_ERRINF_ADDR                                        0x000000d8
#define CSR_MSGRD_ERRINF_DEFAULT                                     0x00000000
#define CSR_QM_STATS_CFG_ADDR                                        0x0000011c
#define CSR_QM_STATS_CFG_DEFAULT                                     0x00000000
#define CSR_ENQ_STATISTICS_ADDR                                      0x00000120
#define CSR_ENQ_STATISTICS_DEFAULT                                   0x00000000
#define CSR_DEQ_STATISTICS_ADDR                                      0x00000124
#define CSR_DEQ_STATISTICS_DEFAULT                                   0x00000000
#define CSR_FIFO_STATUS_ADDR                                         0x00000128
#define CSR_FIFO_STATUS_DEFAULT                                      0x00000000
#define CSR_DOM_0_ERRQ_ADDR                                          0x00000180
#define CSR_DOM_0_ERRQ_DEFAULT                                       0x00000000
#define CSR_DOM_1_ERRQ_ADDR                                          0x00000184
#define CSR_DOM_1_ERRQ_DEFAULT                                       0x00000000
#define CSR_DOM_2_ERRQ_ADDR                                          0x00000188
#define CSR_DOM_2_ERRQ_DEFAULT                                       0x00000000
#define CSR_DOM_3_ERRQ_ADDR                                          0x0000018c
#define CSR_DOM_3_ERRQ_DEFAULT                                       0x00000000
#define CSR_QML_REMAP0_ADDR                                          0x000003e0
#define CSR_QML_REMAP0_DEFAULT                                       0x00010203
#define CSR_QML_REMAP1_ADDR                                          0x000003e4
#define CSR_QML_REMAP1_DEFAULT                                       0x04050000

/*	Register csr_ipbrr	*/ 
/*	 Fields revno	 */
#define QML_REVNO_WIDTH                                                       2
#define QML_REVNO_SHIFT                                                       8
#define QML_REVNO_MASK                                               0x00000300
#define QML_REVNO_RD(src)                             (((src) & 0x00000300)>>8)
#define QML_REVNO_SET(dst,src) \
                       (((dst) & ~0x00000300) | (((u32)(src)<<8) & 0x00000300))
/*	 Fields busid	 */
#define QML_BUSID_WIDTH                                                       3
#define QML_BUSID_SHIFT                                                       5
#define QML_BUSID_MASK                                               0x000000e0
#define QML_BUSID_RD(src)                             (((src) & 0x000000e0)>>5)
#define QML_BUSID_SET(dst,src) \
                       (((dst) & ~0x000000e0) | (((u32)(src)<<5) & 0x000000e0))
/*	 Fields deviceid	 */
#define QML_DEVICEID_WIDTH                                                    5
#define QML_DEVICEID_SHIFT                                                    0
#define QML_DEVICEID_MASK                                            0x0000001f
#define QML_DEVICEID_RD(src)                             (((src) & 0x0000001f))
#define QML_DEVICEID_SET(dst,src) \
                          (((dst) & ~0x0000001f) | (((u32)(src)) & 0x0000001f))

/*	Register csr_qm_config	*/ 
/*	 Fields enable	 */
#define QML_ENABLE_WIDTH                                                      1
#define QML_ENABLE_SHIFT                                                     31
#define QML_ENABLE_MASK                                              0x80000000
#define QML_ENABLE_RD(src)                           (((src) & 0x80000000)>>31)
#define QML_ENABLE_WR(src)                      (((u32)(src)<<31) & 0x80000000)
#define QML_ENABLE_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields mstr_dom	 */
#define MSTR_DOM_WIDTH                                                        2
#define MSTR_DOM_SHIFT                                                       28
#define MSTR_DOM_MASK                                                0x30000000
#define MSTR_DOM_RD(src)                             (((src) & 0x30000000)>>28)
#define MSTR_DOM_WR(src)                        (((u32)(src)<<28) & 0x30000000)
#define MSTR_DOM_SET(dst,src) \
                      (((dst) & ~0x30000000) | (((u32)(src)<<28) & 0x30000000))
/*	 Fields erq_ovr_protect_0	 */
#define ERQ_OVR_PROTECT_0_WIDTH                                               1
#define ERQ_OVR_PROTECT_0_SHIFT                                              27
#define ERQ_OVR_PROTECT_0_MASK                                       0x08000000
#define ERQ_OVR_PROTECT_0_RD(src)                    (((src) & 0x08000000)>>27)
#define ERQ_OVR_PROTECT_0_WR(src)               (((u32)(src)<<27) & 0x08000000)
#define ERQ_OVR_PROTECT_0_SET(dst,src) \
                      (((dst) & ~0x08000000) | (((u32)(src)<<27) & 0x08000000))
/*	 Fields erq_ovr_protect_1	 */
#define ERQ_OVR_PROTECT_1_WIDTH                                               1
#define ERQ_OVR_PROTECT_1_SHIFT                                              26
#define ERQ_OVR_PROTECT_1_MASK                                       0x04000000
#define ERQ_OVR_PROTECT_1_RD(src)                    (((src) & 0x04000000)>>26)
#define ERQ_OVR_PROTECT_1_WR(src)               (((u32)(src)<<26) & 0x04000000)
#define ERQ_OVR_PROTECT_1_SET(dst,src) \
                      (((dst) & ~0x04000000) | (((u32)(src)<<26) & 0x04000000))
/*	 Fields erq_ovr_protect_2	 */
#define ERQ_OVR_PROTECT_2_WIDTH                                               1
#define ERQ_OVR_PROTECT_2_SHIFT                                              25
#define ERQ_OVR_PROTECT_2_MASK                                       0x02000000
#define ERQ_OVR_PROTECT_2_RD(src)                    (((src) & 0x02000000)>>25)
#define ERQ_OVR_PROTECT_2_WR(src)               (((u32)(src)<<25) & 0x02000000)
#define ERQ_OVR_PROTECT_2_SET(dst,src) \
                      (((dst) & ~0x02000000) | (((u32)(src)<<25) & 0x02000000))
/*	 Fields erq_ovr_protect_3	 */
#define ERQ_OVR_PROTECT_3_WIDTH                                               1
#define ERQ_OVR_PROTECT_3_SHIFT                                              24
#define ERQ_OVR_PROTECT_3_MASK                                       0x01000000
#define ERQ_OVR_PROTECT_3_RD(src)                    (((src) & 0x01000000)>>24)
#define ERQ_OVR_PROTECT_3_WR(src)               (((u32)(src)<<24) & 0x01000000)
#define ERQ_OVR_PROTECT_3_SET(dst,src) \
                      (((dst) & ~0x01000000) | (((u32)(src)<<24) & 0x01000000))
/*	 Fields mbox_enq_status_toggle_mode	 */
#define QML_MBOX_ENQ_STATUS_TOGGLE_MODE_WIDTH                                 1
#define QML_MBOX_ENQ_STATUS_TOGGLE_MODE_SHIFT                                23
#define QML_MBOX_ENQ_STATUS_TOGGLE_MODE_MASK                         0x00800000
#define QML_MBOX_ENQ_STATUS_TOGGLE_MODE_RD(src)      (((src) & 0x00800000)>>23)
#define QML_MBOX_ENQ_STATUS_TOGGLE_MODE_WR(src) \
                                                (((u32)(src)<<23) & 0x00800000)
#define QML_MBOX_ENQ_STATUS_TOGGLE_MODE_SET(dst,src) \
                      (((dst) & ~0x00800000) | (((u32)(src)<<23) & 0x00800000))

/*	Register csr_pbm	*/ 
/*	 Fields slvid_pbn	 */
#define SLVID_PBN_WIDTH                                                      10
#define SLVID_PBN_SHIFT                                                       0
#define SLVID_PBN_MASK                                               0x000003ff
#define SLVID_PBN_RD(src)                                (((src) & 0x000003ff))
#define SLVID_PBN_WR(src)                           (((u32)(src)) & 0x000003ff)
#define SLVID_PBN_SET(dst,src) \
                          (((dst) & ~0x000003ff) | (((u32)(src)) & 0x000003ff))

/*	Register csr_pbm_buf_wr	*/ 
/*	 Fields overwrite	 */
#define OVERWRITE_WIDTH                                                       1
#define OVERWRITE_SHIFT                                                      31
#define OVERWRITE_MASK                                               0x80000000
#define OVERWRITE_RD(src)                            (((src) & 0x80000000)>>31)
#define OVERWRITE_WR(src)                       (((u32)(src)<<31) & 0x80000000)
#define OVERWRITE_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields in_service_clr	 */
#define IN_SERVICE_CLR_WIDTH                                                  1
#define IN_SERVICE_CLR_SHIFT                                                 18
#define IN_SERVICE_CLR_MASK                                          0x00040000
#define IN_SERVICE_CLR_RD(src)                       (((src) & 0x00040000)>>18)
#define IN_SERVICE_CLR_WR(src)                  (((u32)(src)<<18) & 0x00040000)
#define IN_SERVICE_CLR_SET(dst,src) \
                      (((dst) & ~0x00040000) | (((u32)(src)<<18) & 0x00040000))
/*	 Fields data	 */
#define DATA_WIDTH                                                           18
#define DATA_SHIFT                                                            0
#define DATA_MASK                                                    0x0003ffff
#define DATA_RD(src)                                     (((src) & 0x0003ffff))
#define DATA_WR(src)                                (((u32)(src)) & 0x0003ffff)
#define DATA_SET(dst,src) \
                          (((dst) & ~0x0003ffff) | (((u32)(src)) & 0x0003ffff))

/*	Register csr_pbm_buf_rd	*/ 
/*	 Fields pb_in_service	 */
#define PB_IN_SERVICE_WIDTH                                                   1
#define PB_IN_SERVICE_SHIFT                                                  18
#define PB_IN_SERVICE_MASK                                           0x00040000
#define PB_IN_SERVICE_RD(src)                        (((src) & 0x00040000)>>18)
#define PB_IN_SERVICE_SET(dst,src) \
                      (((dst) & ~0x00040000) | (((u32)(src)<<18) & 0x00040000))
/*	 Fields slot_num	 */
#define SLOT_NUM_WIDTH                                                        3
#define SLOT_NUM_SHIFT                                                       15
#define SLOT_NUM_MASK                                                0x00038000
#define SLOT_NUM_RD(src)                             (((src) & 0x00038000)>>15)
#define SLOT_NUM_SET(dst,src) \
                      (((dst) & ~0x00038000) | (((u32)(src)<<15) & 0x00038000))
/*	 Fields prefetch_buf_en	 */
#define PREFETCH_BUF_EN_WIDTH                                                 1
#define PREFETCH_BUF_EN_SHIFT                                                14
#define PREFETCH_BUF_EN_MASK                                         0x00004000
#define PREFETCH_BUF_EN_RD(src)                      (((src) & 0x00004000)>>14)
#define PREFETCH_BUF_EN_SET(dst,src) \
                      (((dst) & ~0x00004000) | (((u32)(src)<<14) & 0x00004000))
/*	 Fields is_free_pool	 */
#define IS_FREE_POOL_WIDTH                                                    1
#define IS_FREE_POOL_SHIFT                                                   13
#define IS_FREE_POOL_MASK                                            0x00002000
#define IS_FREE_POOL_RD(src)                         (((src) & 0x00002000)>>13)
#define IS_FREE_POOL_SET(dst,src) \
                      (((dst) & ~0x00002000) | (((u32)(src)<<13) & 0x00002000))
/*	 Fields tlvq	 */
#define TLVQ_WIDTH                                                            1
#define TLVQ_SHIFT                                                           12
#define TLVQ_MASK                                                    0x00001000
#define TLVQ_RD(src)                                 (((src) & 0x00001000)>>12)
#define TLVQ_SET(dst,src) \
                      (((dst) & ~0x00001000) | (((u32)(src)<<12) & 0x00001000))
/*	 Fields corresponding_qnum	 */
#define CORRESPONDING_QNUM_WIDTH                                              8
#define CORRESPONDING_QNUM_SHIFT                                              4
#define CORRESPONDING_QNUM_MASK                                      0x00000ff0
#define CORRESPONDING_QNUM_RD(src)                    (((src) & 0x00000ff0)>>4)
#define CORRESPONDING_QNUM_SET(dst,src) \
                       (((dst) & ~0x00000ff0) | (((u32)(src)<<4) & 0x00000ff0))
/*	 Fields num_msgs_in_buf	 */
#define NUM_MSGS_IN_BUF_WIDTH                                                 4
#define NUM_MSGS_IN_BUF_SHIFT                                                 0
#define NUM_MSGS_IN_BUF_MASK                                         0x0000000f
#define NUM_MSGS_IN_BUF_RD(src)                          (((src) & 0x0000000f))
#define NUM_MSGS_IN_BUF_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register csr_qstate	*/ 
/*	 Fields qnumber	 */
#define QNUMBER_WIDTH                                                         8
#define QNUMBER_SHIFT                                                         0
#define QNUMBER_MASK                                                 0x000000ff
#define QNUMBER_RD(src)                                  (((src) & 0x000000ff))
#define QNUMBER_WR(src)                             (((u32)(src)) & 0x000000ff)
#define QNUMBER_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register csr_qstate_wr_0	*/ 
/*	 Fields data	 */
#define DATA0_WIDTH                                                           6
#define DATA0_SHIFT                                                           0
#define DATA0_MASK                                                   0x0000003f
#define DATA0_RD(src)                                    (((src) & 0x0000003f))
#define DATA0_WR(src)                               (((u32)(src)) & 0x0000003f)
#define DATA0_SET(dst,src) \
                          (((dst) & ~0x0000003f) | (((u32)(src)) & 0x0000003f))

/*	Register csr_qstate_wr_1	*/ 
/*	 Fields data	 */
#define DATA1_WIDTH                                                          32
#define DATA1_SHIFT                                                           0
#define DATA1_MASK                                                   0xffffffff
#define DATA1_RD(src)                                    (((src) & 0xffffffff))
#define DATA1_WR(src)                               (((u32)(src)) & 0xffffffff)
#define DATA1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register csr_qstate_wr_2	*/ 
/*	 Fields data	 */
#define DATA2_WIDTH                                                          32
#define DATA2_SHIFT                                                           0
#define DATA2_MASK                                                   0xffffffff
#define DATA2_RD(src)                                    (((src) & 0xffffffff))
#define DATA2_WR(src)                               (((u32)(src)) & 0xffffffff)
#define DATA2_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register csr_qstate_wr_3	*/ 
/*	 Fields data	 */
#define DATA3_WIDTH                                                          32
#define DATA3_SHIFT                                                           0
#define DATA3_MASK                                                   0xffffffff
#define DATA3_RD(src)                                    (((src) & 0xffffffff))
#define DATA3_WR(src)                               (((u32)(src)) & 0xffffffff)
#define DATA3_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register csr_qstate_wr_4	*/ 
/*	 Fields data	 */
#define DATA4_WIDTH                                                          32
#define DATA4_SHIFT                                                           0
#define DATA4_MASK                                                   0xffffffff
#define DATA4_RD(src)                                    (((src) & 0xffffffff))
#define DATA4_WR(src)                               (((u32)(src)) & 0xffffffff)
#define DATA4_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register csr_qstate_rd_0	*/ 
/*	 Fields data	 */
#define DATA0_F1_WIDTH                                                        6
#define DATA0_F1_SHIFT                                                        0
#define DATA0_F1_MASK                                                0x0000003f
#define DATA0_F1_RD(src)                                 (((src) & 0x0000003f))
#define DATA0_F1_SET(dst,src) \
                          (((dst) & ~0x0000003f) | (((u32)(src)) & 0x0000003f))

/*	Register csr_qstate_rd_1	*/ 
/*	 Fields data	 */
#define DATA1_F1_WIDTH                                                       32
#define DATA1_F1_SHIFT                                                        0
#define DATA1_F1_MASK                                                0xffffffff
#define DATA1_F1_RD(src)                                 (((src) & 0xffffffff))
#define DATA1_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register csr_qstate_rd_2	*/ 
/*	 Fields data	 */
#define DATA2_F1_WIDTH                                                       32
#define DATA2_F1_SHIFT                                                        0
#define DATA2_F1_MASK                                                0xffffffff
#define DATA2_F1_RD(src)                                 (((src) & 0xffffffff))
#define DATA2_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register csr_qstate_rd_3	*/ 
/*	 Fields data	 */
#define DATA3_F1_WIDTH                                                       32
#define DATA3_F1_SHIFT                                                        0
#define DATA3_F1_MASK                                                0xffffffff
#define DATA3_F1_RD(src)                                 (((src) & 0xffffffff))
#define DATA3_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register csr_qstate_rd_4	*/ 
/*	 Fields data	 */
#define DATA4_F1_WIDTH                                                       32
#define DATA4_F1_SHIFT                                                        0
#define DATA4_F1_MASK                                                0xffffffff
#define DATA4_F1_RD(src)                                 (((src) & 0xffffffff))
#define DATA4_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register csr_enq_base_a	*/ 
/*	 Fields coherent	 */
#define COHERENT_WIDTH                                                        1
#define COHERENT_SHIFT                                                       31
#define COHERENT_MASK                                                0x80000000
#define COHERENT_RD(src)                             (((src) & 0x80000000)>>31)
#define COHERENT_WR(src)                        (((u32)(src)<<31) & 0x80000000)
#define COHERENT_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields addr	 */
#define QML_ADDR_WIDTH                                                       26
#define QML_ADDR_SHIFT                                                        0
#define QML_ADDR_MASK                                                0x03ffffff
#define QML_ADDR_RD(src)                                 (((src) & 0x03ffffff))
#define QML_ADDR_WR(src)                            (((u32)(src)) & 0x03ffffff)
#define QML_ADDR_SET(dst,src) \
                          (((dst) & ~0x03ffffff) | (((u32)(src)) & 0x03ffffff))

/*	Register csr_enq_base_b	*/ 
/*	 Fields coherent	 */
#define COHERENT_F1_WIDTH                                                     1
#define COHERENT_F1_SHIFT                                                    31
#define COHERENT_F1_MASK                                             0x80000000
#define COHERENT_F1_RD(src)                          (((src) & 0x80000000)>>31)
#define COHERENT_F1_WR(src)                     (((u32)(src)<<31) & 0x80000000)
#define COHERENT_F1_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields addr	 */
#define QML_ADDR_F1_WIDTH                                                    26
#define QML_ADDR_F1_SHIFT                                                     0
#define QML_ADDR_F1_MASK                                             0x03ffffff
#define QML_ADDR_F1_RD(src)                              (((src) & 0x03ffffff))
#define QML_ADDR_F1_WR(src)                         (((u32)(src)) & 0x03ffffff)
#define QML_ADDR_F1_SET(dst,src) \
                          (((dst) & ~0x03ffffff) | (((u32)(src)) & 0x03ffffff))

/*	Register csr_enq_status_0	*/ 
/*	 Fields mbox_0	 */
#define MBOX_00_WIDTH                                                         8
#define MBOX_00_SHIFT                                                        24
#define MBOX_00_MASK                                                 0xff000000
#define MBOX_00_RD(src)                              (((src) & 0xff000000)>>24)
#define MBOX_00_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))
/*	 Fields mbox_1	 */
#define MBOX_10_WIDTH                                                         8
#define MBOX_10_SHIFT                                                        16
#define MBOX_10_MASK                                                 0x00ff0000
#define MBOX_10_RD(src)                              (((src) & 0x00ff0000)>>16)
#define MBOX_10_SET(dst,src) \
                      (((dst) & ~0x00ff0000) | (((u32)(src)<<16) & 0x00ff0000))
/*	 Fields mbox_2	 */
#define MBOX_20_WIDTH                                                         8
#define MBOX_20_SHIFT                                                         8
#define MBOX_20_MASK                                                 0x0000ff00
#define MBOX_20_RD(src)                               (((src) & 0x0000ff00)>>8)
#define MBOX_20_SET(dst,src) \
                       (((dst) & ~0x0000ff00) | (((u32)(src)<<8) & 0x0000ff00))
/*	 Fields mbox_3	 */
#define MBOX_30_WIDTH                                                         8
#define MBOX_30_SHIFT                                                         0
#define MBOX_30_MASK                                                 0x000000ff
#define MBOX_30_RD(src)                                  (((src) & 0x000000ff))
#define MBOX_30_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register csr_enq_status_1	*/ 
/*	 Fields mbox_4	 */
#define MBOX_41_WIDTH                                                         8
#define MBOX_41_SHIFT                                                        24
#define MBOX_41_MASK                                                 0xff000000
#define MBOX_41_RD(src)                              (((src) & 0xff000000)>>24)
#define MBOX_41_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))
/*	 Fields mbox_5	 */
#define MBOX_51_WIDTH                                                         8
#define MBOX_51_SHIFT                                                        16
#define MBOX_51_MASK                                                 0x00ff0000
#define MBOX_51_RD(src)                              (((src) & 0x00ff0000)>>16)
#define MBOX_51_SET(dst,src) \
                      (((dst) & ~0x00ff0000) | (((u32)(src)<<16) & 0x00ff0000))
/*	 Fields mbox_6	 */
#define MBOX_61_WIDTH                                                         8
#define MBOX_61_SHIFT                                                         8
#define MBOX_61_MASK                                                 0x0000ff00
#define MBOX_61_RD(src)                               (((src) & 0x0000ff00)>>8)
#define MBOX_61_SET(dst,src) \
                       (((dst) & ~0x0000ff00) | (((u32)(src)<<8) & 0x0000ff00))
/*	 Fields mbox_7	 */
#define MBOX_71_WIDTH                                                         8
#define MBOX_71_SHIFT                                                         0
#define MBOX_71_MASK                                                 0x000000ff
#define MBOX_71_RD(src)                                  (((src) & 0x000000ff))
#define MBOX_71_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register csr_enq_status_2	*/ 
/*	 Fields mbox_8	 */
#define MBOX_82_WIDTH                                                         8
#define MBOX_82_SHIFT                                                        24
#define MBOX_82_MASK                                                 0xff000000
#define MBOX_82_RD(src)                              (((src) & 0xff000000)>>24)
#define MBOX_82_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))
/*	 Fields mbox_9	 */
#define MBOX_92_WIDTH                                                         8
#define MBOX_92_SHIFT                                                        16
#define MBOX_92_MASK                                                 0x00ff0000
#define MBOX_92_RD(src)                              (((src) & 0x00ff0000)>>16)
#define MBOX_92_SET(dst,src) \
                      (((dst) & ~0x00ff0000) | (((u32)(src)<<16) & 0x00ff0000))
/*	 Fields mbox_10	 */
#define MBOX_102_WIDTH                                                        8
#define MBOX_102_SHIFT                                                        8
#define MBOX_102_MASK                                                0x0000ff00
#define MBOX_102_RD(src)                              (((src) & 0x0000ff00)>>8)
#define MBOX_102_SET(dst,src) \
                       (((dst) & ~0x0000ff00) | (((u32)(src)<<8) & 0x0000ff00))
/*	 Fields mbox_11	 */
#define MBOX_112_WIDTH                                                        8
#define MBOX_112_SHIFT                                                        0
#define MBOX_112_MASK                                                0x000000ff
#define MBOX_112_RD(src)                                 (((src) & 0x000000ff))
#define MBOX_112_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register csr_enq_status_3	*/ 
/*	 Fields mbox_12	 */
#define MBOX_123_WIDTH                                                        8
#define MBOX_123_SHIFT                                                       24
#define MBOX_123_MASK                                                0xff000000
#define MBOX_123_RD(src)                             (((src) & 0xff000000)>>24)
#define MBOX_123_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))
/*	 Fields mbox_13	 */
#define MBOX_133_WIDTH                                                        8
#define MBOX_133_SHIFT                                                       16
#define MBOX_133_MASK                                                0x00ff0000
#define MBOX_133_RD(src)                             (((src) & 0x00ff0000)>>16)
#define MBOX_133_SET(dst,src) \
                      (((dst) & ~0x00ff0000) | (((u32)(src)<<16) & 0x00ff0000))
/*	 Fields mbox_14	 */
#define MBOX_143_WIDTH                                                        8
#define MBOX_143_SHIFT                                                        8
#define MBOX_143_MASK                                                0x0000ff00
#define MBOX_143_RD(src)                              (((src) & 0x0000ff00)>>8)
#define MBOX_143_SET(dst,src) \
                       (((dst) & ~0x0000ff00) | (((u32)(src)<<8) & 0x0000ff00))
/*	 Fields mbox_15	 */
#define MBOX_153_WIDTH                                                        8
#define MBOX_153_SHIFT                                                        0
#define MBOX_153_MASK                                                0x000000ff
#define MBOX_153_RD(src)                                 (((src) & 0x000000ff))
#define MBOX_153_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register qm_INT	*/ 
/*	 Fields qpcore_acr_error	 */
#define QPCORE_ACR_ERROR_WIDTH                                                1
#define QPCORE_ACR_ERROR_SHIFT                                                2
#define QPCORE_ACR_ERROR_MASK                                        0x00000004
#define QPCORE_ACR_ERROR_RD(src)                      (((src) & 0x00000004)>>2)
#define QPCORE_ACR_ERROR_WR(src)                 (((u32)(src)<<2) & 0x00000004)
#define QPCORE_ACR_ERROR_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields deq_axi_error	 */
#define DEQ_AXI_ERROR_WIDTH                                                   1
#define DEQ_AXI_ERROR_SHIFT                                                   1
#define DEQ_AXI_ERROR_MASK                                           0x00000002
#define DEQ_AXI_ERROR_RD(src)                         (((src) & 0x00000002)>>1)
#define DEQ_AXI_ERROR_WR(src)                    (((u32)(src)<<1) & 0x00000002)
#define DEQ_AXI_ERROR_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields pbm_dec_error	 */
#define PBM_DEC_ERROR_WIDTH                                                   1
#define PBM_DEC_ERROR_SHIFT                                                   0
#define PBM_DEC_ERROR_MASK                                           0x00000001
#define PBM_DEC_ERROR_RD(src)                            (((src) & 0x00000001))
#define PBM_DEC_ERROR_WR(src)                       (((u32)(src)) & 0x00000001)
#define PBM_DEC_ERROR_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register qm_interruptMask	*/
/*    Mask Register Fields qpcore_acr_errorMask    */
#define QPCORE_ACR_ERRORMASK_WIDTH                                            1
#define QPCORE_ACR_ERRORMASK_SHIFT                                            2
#define QPCORE_ACR_ERRORMASK_MASK                                    0x00000004
#define QPCORE_ACR_ERRORMASK_RD(src)                  (((src) & 0x00000004)>>2)
#define QPCORE_ACR_ERRORMASK_WR(src)             (((u32)(src)<<2) & 0x00000004)
#define QPCORE_ACR_ERRORMASK_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*    Mask Register Fields deq_axi_errorMask    */
#define DEQ_AXI_ERRORMASK_WIDTH                                               1
#define DEQ_AXI_ERRORMASK_SHIFT                                               1
#define DEQ_AXI_ERRORMASK_MASK                                       0x00000002
#define DEQ_AXI_ERRORMASK_RD(src)                     (((src) & 0x00000002)>>1)
#define DEQ_AXI_ERRORMASK_WR(src)                (((u32)(src)<<1) & 0x00000002)
#define DEQ_AXI_ERRORMASK_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*    Mask Register Fields pbm_dec_errorMask    */
#define PBM_DEC_ERRORMASK_WIDTH                                               1
#define PBM_DEC_ERRORMASK_SHIFT                                               0
#define PBM_DEC_ERRORMASK_MASK                                       0x00000001
#define PBM_DEC_ERRORMASK_RD(src)                        (((src) & 0x00000001))
#define PBM_DEC_ERRORMASK_WR(src)                   (((u32)(src)) & 0x00000001)
#define PBM_DEC_ERRORMASK_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register csr_pbm_errinf	*/ 
/*	 Fields pbm_decrement_err	 */
#define PBM_DECREMENT_ERR_WIDTH                                               1
#define PBM_DECREMENT_ERR_SHIFT                                              16
#define PBM_DECREMENT_ERR_MASK                                       0x00010000
#define PBM_DECREMENT_ERR_RD(src)                    (((src) & 0x00010000)>>16)
#define PBM_DECREMENT_ERR_SET(dst,src) \
                      (((dst) & ~0x00010000) | (((u32)(src)<<16) & 0x00010000))
/*	 Fields slave_id	 */
#define SLAVE_ID_WIDTH                                                        4
#define SLAVE_ID_SHIFT                                                        6
#define SLAVE_ID_MASK                                                0x000003c0
#define SLAVE_ID_RD(src)                              (((src) & 0x000003c0)>>6)
#define SLAVE_ID_SET(dst,src) \
                       (((dst) & ~0x000003c0) | (((u32)(src)<<6) & 0x000003c0))
/*	 Fields PB_number	 */
#define PB_NUMBER_WIDTH                                                       6
#define PB_NUMBER_SHIFT                                                       0
#define PB_NUMBER_MASK                                               0x0000003f
#define PB_NUMBER_RD(src)                                (((src) & 0x0000003f))
#define PB_NUMBER_SET(dst,src) \
                          (((dst) & ~0x0000003f) | (((u32)(src)) & 0x0000003f))

/*	Register csr_msgrd_errinf	*/ 
/*	 Fields qpcore_was_disabled	 */
#define QPCORE_WAS_DISABLED_WIDTH                                             1
#define QPCORE_WAS_DISABLED_SHIFT                                            27
#define QPCORE_WAS_DISABLED_MASK                                     0x08000000
#define QPCORE_WAS_DISABLED_RD(src)                  (((src) & 0x08000000)>>27)
#define QPCORE_WAS_DISABLED_SET(dst,src) \
                      (((dst) & ~0x08000000) | (((u32)(src)<<27) & 0x08000000))
/*	 Fields drop_code	 */
#define DROP_CODE_WIDTH                                                       3
#define DROP_CODE_SHIFT                                                      24
#define DROP_CODE_MASK                                               0x07000000
#define DROP_CODE_RD(src)                            (((src) & 0x07000000)>>24)
#define DROP_CODE_SET(dst,src) \
                      (((dst) & ~0x07000000) | (((u32)(src)<<24) & 0x07000000))
/*	 Fields acr_qid	 */
#define ACR_QID_WIDTH                                                         8
#define ACR_QID_SHIFT                                                        16
#define ACR_QID_MASK                                                 0x00ff0000
#define ACR_QID_RD(src)                              (((src) & 0x00ff0000)>>16)
#define ACR_QID_SET(dst,src) \
                      (((dst) & ~0x00ff0000) | (((u32)(src)<<16) & 0x00ff0000))
/*	 Fields response_code	 */
#define RESPONSE_CODE_WIDTH                                                   2
#define RESPONSE_CODE_SHIFT                                                   8
#define RESPONSE_CODE_MASK                                           0x00000300
#define RESPONSE_CODE_RD(src)                         (((src) & 0x00000300)>>8)
#define RESPONSE_CODE_SET(dst,src) \
                       (((dst) & ~0x00000300) | (((u32)(src)<<8) & 0x00000300))
/*	 Fields qid	 */
#define QID_WIDTH                                                             8
#define QID_SHIFT                                                             0
#define QID_MASK                                                     0x000000ff
#define QID_RD(src)                                      (((src) & 0x000000ff))
#define QID_SET(dst,src) (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register csr_qm_stats_cfg	*/ 
/*	 Fields qid_enq_counter	 */
#define QID_ENQ_COUNTER_WIDTH                                                 8
#define QID_ENQ_COUNTER_SHIFT                                                24
#define QID_ENQ_COUNTER_MASK                                         0xff000000
#define QID_ENQ_COUNTER_RD(src)                      (((src) & 0xff000000)>>24)
#define QID_ENQ_COUNTER_WR(src)                 (((u32)(src)<<24) & 0xff000000)
#define QID_ENQ_COUNTER_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))
/*	 Fields qid_deq_counter	 */
#define QID_DEQ_COUNTER_WIDTH                                                 8
#define QID_DEQ_COUNTER_SHIFT                                                16
#define QID_DEQ_COUNTER_MASK                                         0x00ff0000
#define QID_DEQ_COUNTER_RD(src)                      (((src) & 0x00ff0000)>>16)
#define QID_DEQ_COUNTER_WR(src)                 (((u32)(src)<<16) & 0x00ff0000)
#define QID_DEQ_COUNTER_SET(dst,src) \
                      (((dst) & ~0x00ff0000) | (((u32)(src)<<16) & 0x00ff0000))

/*	Register csr_enq_statistics	*/ 
/*	 Fields enq_count	 */
#define ENQ_COUNT_WIDTH                                                      32
#define ENQ_COUNT_SHIFT                                                       0
#define ENQ_COUNT_MASK                                               0xffffffff
#define ENQ_COUNT_RD(src)                                (((src) & 0xffffffff))
#define ENQ_COUNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register csr_deq_statistics	*/ 
/*	 Fields deq_count	 */
#define DEQ_COUNT_WIDTH                                                      32
#define DEQ_COUNT_SHIFT                                                       0
#define DEQ_COUNT_MASK                                               0xffffffff
#define DEQ_COUNT_RD(src)                                (((src) & 0xffffffff))
#define DEQ_COUNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register csr_fifo_status	*/ 
/*	 Fields csr_qpcore_active_process	 */
#define CSR_QPCORE_ACTIVE_PROCESS_WIDTH                                       5
#define CSR_QPCORE_ACTIVE_PROCESS_SHIFT                                      22
#define CSR_QPCORE_ACTIVE_PROCESS_MASK                               0x07c00000
#define CSR_QPCORE_ACTIVE_PROCESS_RD(src)            (((src) & 0x07c00000)>>22)
#define CSR_QPCORE_ACTIVE_PROCESS_SET(dst,src) \
                      (((dst) & ~0x07c00000) | (((u32)(src)<<22) & 0x07c00000))
/*	 Fields csr_msgwr_outstanding_req	 */
#define CSR_MSGWR_OUTSTANDING_REQ_WIDTH                                       3
#define CSR_MSGWR_OUTSTANDING_REQ_SHIFT                                      19
#define CSR_MSGWR_OUTSTANDING_REQ_MASK                               0x00380000
#define CSR_MSGWR_OUTSTANDING_REQ_RD(src)            (((src) & 0x00380000)>>19)
#define CSR_MSGWR_OUTSTANDING_REQ_SET(dst,src) \
                      (((dst) & ~0x00380000) | (((u32)(src)<<19) & 0x00380000))
/*	 Fields csr_msgrd_fifo_stat	 */
#define CSR_MSGRD_FIFO_STAT_WIDTH                                             7
#define CSR_MSGRD_FIFO_STAT_SHIFT                                            12
#define CSR_MSGRD_FIFO_STAT_MASK                                     0x0007f000
#define CSR_MSGRD_FIFO_STAT_RD(src)                  (((src) & 0x0007f000)>>12)
#define CSR_MSGRD_FIFO_STAT_SET(dst,src) \
                      (((dst) & ~0x0007f000) | (((u32)(src)<<12) & 0x0007f000))
/*	 Fields csr_acr_mstr_cmd_level	 */
#define CSR_ACR_MSTR_CMD_LEVEL_WIDTH                                          4
#define CSR_ACR_MSTR_CMD_LEVEL_SHIFT                                          8
#define CSR_ACR_MSTR_CMD_LEVEL_MASK                                  0x00000f00
#define CSR_ACR_MSTR_CMD_LEVEL_RD(src)                (((src) & 0x00000f00)>>8)
#define CSR_ACR_MSTR_CMD_LEVEL_SET(dst,src) \
                       (((dst) & ~0x00000f00) | (((u32)(src)<<8) & 0x00000f00))
/*	 Fields csr_acr_allocated	 */
#define CSR_ACR_ALLOCATED_WIDTH                                               8
#define CSR_ACR_ALLOCATED_SHIFT                                               0
#define CSR_ACR_ALLOCATED_MASK                                       0x000000ff
#define CSR_ACR_ALLOCATED_RD(src)                        (((src) & 0x000000ff))
#define CSR_ACR_ALLOCATED_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register csr_dom_0_errq	*/ 
/*	 Fields unexpected_en	 */
#define UNEXPECTED_EN_WIDTH                                                   1
#define UNEXPECTED_EN_SHIFT                                                  31
#define UNEXPECTED_EN_MASK                                           0x80000000
#define UNEXPECTED_EN_RD(src)                        (((src) & 0x80000000)>>31)
#define UNEXPECTED_EN_WR(src)                   (((u32)(src)<<31) & 0x80000000)
#define UNEXPECTED_EN_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields unexpected_qid	 */
#define UNEXPECTED_QID_WIDTH                                                  8
#define UNEXPECTED_QID_SHIFT                                                 16
#define UNEXPECTED_QID_MASK                                          0x00ff0000
#define UNEXPECTED_QID_RD(src)                       (((src) & 0x00ff0000)>>16)
#define UNEXPECTED_QID_WR(src)                  (((u32)(src)<<16) & 0x00ff0000)
#define UNEXPECTED_QID_SET(dst,src) \
                      (((dst) & ~0x00ff0000) | (((u32)(src)<<16) & 0x00ff0000))
/*	 Fields expected_en	 */
#define EXPECTED_EN_WIDTH                                                     1
#define EXPECTED_EN_SHIFT                                                    15
#define EXPECTED_EN_MASK                                             0x00008000
#define EXPECTED_EN_RD(src)                          (((src) & 0x00008000)>>15)
#define EXPECTED_EN_WR(src)                     (((u32)(src)<<15) & 0x00008000)
#define EXPECTED_EN_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))
/*	 Fields expected_qid	 */
#define EXPECTED_QID_WIDTH                                                    8
#define EXPECTED_QID_SHIFT                                                    0
#define EXPECTED_QID_MASK                                            0x000000ff
#define EXPECTED_QID_RD(src)                             (((src) & 0x000000ff))
#define EXPECTED_QID_WR(src)                        (((u32)(src)) & 0x000000ff)
#define EXPECTED_QID_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register csr_dom_1_errq	*/ 
/*	 Fields unexpected_en	 */
#define UNEXPECTED_EN_F1_WIDTH                                                1
#define UNEXPECTED_EN_F1_SHIFT                                               31
#define UNEXPECTED_EN_F1_MASK                                        0x80000000
#define UNEXPECTED_EN_F1_RD(src)                     (((src) & 0x80000000)>>31)
#define UNEXPECTED_EN_F1_WR(src)                (((u32)(src)<<31) & 0x80000000)
#define UNEXPECTED_EN_F1_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields unexpected_qid	 */
#define UNEXPECTED_QID_F1_WIDTH                                               8
#define UNEXPECTED_QID_F1_SHIFT                                              16
#define UNEXPECTED_QID_F1_MASK                                       0x00ff0000
#define UNEXPECTED_QID_F1_RD(src)                    (((src) & 0x00ff0000)>>16)
#define UNEXPECTED_QID_F1_WR(src)               (((u32)(src)<<16) & 0x00ff0000)
#define UNEXPECTED_QID_F1_SET(dst,src) \
                      (((dst) & ~0x00ff0000) | (((u32)(src)<<16) & 0x00ff0000))
/*	 Fields expected_en	 */
#define EXPECTED_EN_F1_WIDTH                                                  1
#define EXPECTED_EN_F1_SHIFT                                                 15
#define EXPECTED_EN_F1_MASK                                          0x00008000
#define EXPECTED_EN_F1_RD(src)                       (((src) & 0x00008000)>>15)
#define EXPECTED_EN_F1_WR(src)                  (((u32)(src)<<15) & 0x00008000)
#define EXPECTED_EN_F1_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))
/*	 Fields expected_qid	 */
#define EXPECTED_QID_F1_WIDTH                                                 8
#define EXPECTED_QID_F1_SHIFT                                                 0
#define EXPECTED_QID_F1_MASK                                         0x000000ff
#define EXPECTED_QID_F1_RD(src)                          (((src) & 0x000000ff))
#define EXPECTED_QID_F1_WR(src)                     (((u32)(src)) & 0x000000ff)
#define EXPECTED_QID_F1_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register csr_dom_2_errq	*/ 
/*	 Fields unexpected_en	 */
#define UNEXPECTED_EN_F2_WIDTH                                                1
#define UNEXPECTED_EN_F2_SHIFT                                               31
#define UNEXPECTED_EN_F2_MASK                                        0x80000000
#define UNEXPECTED_EN_F2_RD(src)                     (((src) & 0x80000000)>>31)
#define UNEXPECTED_EN_F2_WR(src)                (((u32)(src)<<31) & 0x80000000)
#define UNEXPECTED_EN_F2_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields unexpected_qid	 */
#define UNEXPECTED_QID_F2_WIDTH                                               8
#define UNEXPECTED_QID_F2_SHIFT                                              16
#define UNEXPECTED_QID_F2_MASK                                       0x00ff0000
#define UNEXPECTED_QID_F2_RD(src)                    (((src) & 0x00ff0000)>>16)
#define UNEXPECTED_QID_F2_WR(src)               (((u32)(src)<<16) & 0x00ff0000)
#define UNEXPECTED_QID_F2_SET(dst,src) \
                      (((dst) & ~0x00ff0000) | (((u32)(src)<<16) & 0x00ff0000))
/*	 Fields expected_en	 */
#define EXPECTED_EN_F2_WIDTH                                                  1
#define EXPECTED_EN_F2_SHIFT                                                 15
#define EXPECTED_EN_F2_MASK                                          0x00008000
#define EXPECTED_EN_F2_RD(src)                       (((src) & 0x00008000)>>15)
#define EXPECTED_EN_F2_WR(src)                  (((u32)(src)<<15) & 0x00008000)
#define EXPECTED_EN_F2_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))
/*	 Fields expected_qid	 */
#define EXPECTED_QID_F2_WIDTH                                                 8
#define EXPECTED_QID_F2_SHIFT                                                 0
#define EXPECTED_QID_F2_MASK                                         0x000000ff
#define EXPECTED_QID_F2_RD(src)                          (((src) & 0x000000ff))
#define EXPECTED_QID_F2_WR(src)                     (((u32)(src)) & 0x000000ff)
#define EXPECTED_QID_F2_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register csr_dom_3_errq	*/ 
/*	 Fields unexpected_en	 */
#define UNEXPECTED_EN_F3_WIDTH                                                1
#define UNEXPECTED_EN_F3_SHIFT                                               31
#define UNEXPECTED_EN_F3_MASK                                        0x80000000
#define UNEXPECTED_EN_F3_RD(src)                     (((src) & 0x80000000)>>31)
#define UNEXPECTED_EN_F3_WR(src)                (((u32)(src)<<31) & 0x80000000)
#define UNEXPECTED_EN_F3_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields unexpected_qid	 */
#define UNEXPECTED_QID_F3_WIDTH                                               8
#define UNEXPECTED_QID_F3_SHIFT                                              16
#define UNEXPECTED_QID_F3_MASK                                       0x00ff0000
#define UNEXPECTED_QID_F3_RD(src)                    (((src) & 0x00ff0000)>>16)
#define UNEXPECTED_QID_F3_WR(src)               (((u32)(src)<<16) & 0x00ff0000)
#define UNEXPECTED_QID_F3_SET(dst,src) \
                      (((dst) & ~0x00ff0000) | (((u32)(src)<<16) & 0x00ff0000))
/*	 Fields expected_en	 */
#define EXPECTED_EN_F3_WIDTH                                                  1
#define EXPECTED_EN_F3_SHIFT                                                 15
#define EXPECTED_EN_F3_MASK                                          0x00008000
#define EXPECTED_EN_F3_RD(src)                       (((src) & 0x00008000)>>15)
#define EXPECTED_EN_F3_WR(src)                  (((u32)(src)<<15) & 0x00008000)
#define EXPECTED_EN_F3_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))
/*	 Fields expected_qid	 */
#define EXPECTED_QID_F3_WIDTH                                                 8
#define EXPECTED_QID_F3_SHIFT                                                 0
#define EXPECTED_QID_F3_MASK                                         0x000000ff
#define EXPECTED_QID_F3_RD(src)                          (((src) & 0x000000ff))
#define EXPECTED_QID_F3_WR(src)                     (((u32)(src)) & 0x000000ff)
#define EXPECTED_QID_F3_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register csr_qml_remap0	*/ 
/*	 Fields qid0	 */
#define QID00_WIDTH                                                           8
#define QID00_SHIFT                                                          24
#define QID00_MASK                                                   0xff000000
#define QID00_RD(src)                                (((src) & 0xff000000)>>24)
#define QID00_WR(src)                           (((u32)(src)<<24) & 0xff000000)
#define QID00_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))
/*	 Fields qid1	 */
#define QID10_WIDTH                                                           8
#define QID10_SHIFT                                                          16
#define QID10_MASK                                                   0x00ff0000
#define QID10_RD(src)                                (((src) & 0x00ff0000)>>16)
#define QID10_WR(src)                           (((u32)(src)<<16) & 0x00ff0000)
#define QID10_SET(dst,src) \
                      (((dst) & ~0x00ff0000) | (((u32)(src)<<16) & 0x00ff0000))
/*	 Fields qid2	 */
#define QID20_WIDTH                                                           8
#define QID20_SHIFT                                                           8
#define QID20_MASK                                                   0x0000ff00
#define QID20_RD(src)                                 (((src) & 0x0000ff00)>>8)
#define QID20_WR(src)                            (((u32)(src)<<8) & 0x0000ff00)
#define QID20_SET(dst,src) \
                       (((dst) & ~0x0000ff00) | (((u32)(src)<<8) & 0x0000ff00))
/*	 Fields qid3	 */
#define QID30_WIDTH                                                           8
#define QID30_SHIFT                                                           0
#define QID30_MASK                                                   0x000000ff
#define QID30_RD(src)                                    (((src) & 0x000000ff))
#define QID30_WR(src)                               (((u32)(src)) & 0x000000ff)
#define QID30_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register csr_qml_remap1	*/ 
/*	 Fields qid4	 */
#define QID41_WIDTH                                                           8
#define QID41_SHIFT                                                          24
#define QID41_MASK                                                   0xff000000
#define QID41_RD(src)                                (((src) & 0xff000000)>>24)
#define QID41_WR(src)                           (((u32)(src)<<24) & 0xff000000)
#define QID41_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))
/*	 Fields qid5	 */
#define QID51_WIDTH                                                           8
#define QID51_SHIFT                                                          16
#define QID51_MASK                                                   0x00ff0000
#define QID51_RD(src)                                (((src) & 0x00ff0000)>>16)
#define QID51_WR(src)                           (((u32)(src)<<16) & 0x00ff0000)
#define QID51_SET(dst,src) \
                      (((dst) & ~0x00ff0000) | (((u32)(src)<<16) & 0x00ff0000))

/*	Global Base Address	*/
#define QML_GLBL_DIAG_CSR_BASE_ADDR			0xddd8d7000ULL

/*    Address GLBL_DIAG_CSR  Registers */
#define QML_CFG_DIAG_SEL_ADDR                                        0x00000000
#define QML_CFG_DIAG_SEL_DEFAULT                                     0x00000000
#define QML_CFG_READ_BW_LAT_ADDR_MASK_ADDR                           0x00000004
#define QML_CFG_READ_BW_LAT_ADDR_MASK_DEFAULT                        0x00000000
#define QML_CFG_READ_BW_LAT_ADDR_PAT_ADDR                            0x00000008
#define QML_CFG_READ_BW_LAT_ADDR_PAT_DEFAULT                         0xffffffff
#define QML_CFG_WRITE_BW_LAT_ADDR_MASK_ADDR                          0x0000000c
#define QML_CFG_WRITE_BW_LAT_ADDR_MASK_DEFAULT                       0x00000000
#define QML_CFG_WRITE_BW_LAT_ADDR_PAT_ADDR                           0x00000010
#define QML_CFG_WRITE_BW_LAT_ADDR_PAT_DEFAULT                        0xffffffff
#define QML_CFG_DIAG_START_STOP_ADDR                                 0x00000014
#define QML_CFG_DIAG_START_STOP_DEFAULT                              0x0000001f
#define QML_CFG_BW_MSTR_STOP_CNT_ADDR                                0x00000018
#define QML_CFG_BW_MSTR_STOP_CNT_DEFAULT                             0x00040004
#define QML_CFG_BW_SLV_STOP_CNT_ADDR                                 0x0000001c
#define QML_CFG_BW_SLV_STOP_CNT_DEFAULT                              0x00040004
#define QML_STS_READ_LATENCY_OUTPUT_ADDR                             0x00000020
#define QML_STS_READ_LATENCY_OUTPUT_DEFAULT                          0x00000000
#define QML_STS_AXI_MRD_BW_CLK_CNT_ADDR                              0x00000024
#define QML_STS_AXI_MRD_BW_CLK_CNT_DEFAULT                           0x00000000
#define QML_STS_AXI_MRD_BW_BYTE_CNT_ADDR                             0x00000028
#define QML_STS_AXI_MRD_BW_BYTE_CNT_DEFAULT                          0x00000000
#define QML_STS_AXI_MWR_BW_CLK_CNT_ADDR                              0x0000002c
#define QML_STS_AXI_MWR_BW_CLK_CNT_DEFAULT                           0x00000000
#define QML_STS_AXI_MWR_BW_BYTE_CNT_ADDR                             0x00000030
#define QML_STS_AXI_MWR_BW_BYTE_CNT_DEFAULT                          0x00000000
#define QML_STS_AXI_SRD_BW_CLK_CNT_ADDR                              0x00000034
#define QML_STS_AXI_SRD_BW_CLK_CNT_DEFAULT                           0x00000000
#define QML_STS_AXI_SRD_BW_BYTE_CNT_ADDR                             0x00000038
#define QML_STS_AXI_SRD_BW_BYTE_CNT_DEFAULT                          0x00000000
#define QML_STS_AXI_SWR_BW_CLK_CNT_ADDR                              0x0000003c
#define QML_STS_AXI_SWR_BW_CLK_CNT_DEFAULT                           0x00000000
#define QML_STS_AXI_SWR_BW_BYTE_CNT_ADDR                             0x00000040
#define QML_STS_AXI_SWR_BW_BYTE_CNT_DEFAULT                          0x00000000
#define QML_CFG_DBG_TRIG_CTRL_ADDR                                   0x00000044
#define QML_CFG_DBG_TRIG_CTRL_DEFAULT                                0x00000000
#define QML_CFG_DBG_PAT_REG_0_ADDR                                   0x00000048
#define QML_CFG_DBG_PAT_REG_0_DEFAULT                                0x00000000
#define QML_CFG_DBG_PAT_MASK_REG_0_ADDR                              0x0000004c
#define QML_CFG_DBG_PAT_MASK_REG_0_DEFAULT                           0x00000000
#define QML_CFG_DBG_PAT_REG_1_ADDR                                   0x00000050
#define QML_CFG_DBG_PAT_REG_1_DEFAULT                                0x00000000
#define QML_CFG_DBG_PAT_MASK_REG_1_ADDR                              0x00000054
#define QML_CFG_DBG_PAT_MASK_REG_1_DEFAULT                           0x00000000
#define QML_DBG_TRIG_OUT_ADDR                                        0x00000058
#define QML_DBG_TRIG_OUT_DEFAULT                                     0x00000000
#define QML_DBG_ECC_INT_OUT_ADDR                                     0x0000005c
#define QML_DBG_ECC_INT_OUT_DEFAULT                                  0x00000000
#define QML_DBG_ECC_INT_OUTMASK_ADDR                                 0x00000060
#define QML_CFG_SECC_INT_THRSH_REG_ADDR                              0x00000064
#define QML_CFG_SECC_INT_THRSH_REG_DEFAULT                           0x000000fe
#define QML_CFG_MEM_ECC_BYPASS_ADDR                                  0x00000068
#define QML_CFG_MEM_ECC_BYPASS_DEFAULT                               0x00000000
#define QML_CFG_MEM_PWRDN_DIS_ADDR                                   0x0000006c
#define QML_CFG_MEM_PWRDN_DIS_DEFAULT                                0x00000000
#define QML_CFG_MEM_RAM_SHUTDOWN_ADDR                                0x00000070
#define QML_CFG_MEM_RAM_SHUTDOWN_DEFAULT                             0xffffffff
#define QML_BLOCK_MEM_RDY_ADDR                                       0x00000074
#define QML_BLOCK_MEM_RDY_DEFAULT                                    0xffffffff
#define QML_CFG_AXI_SLV_RD_TMO_ADDR                                  0x00000078
#define QML_CFG_AXI_SLV_RD_TMO_DEFAULT                               0x83ff83ff
#define QML_CFG_AXI_SLV_WR_TMO_ADDR                                  0x0000007c
#define QML_CFG_AXI_SLV_WR_TMO_DEFAULT                               0x83ff83ff
#define QML_STS_AXI_SLV_TMO_ADDR                                     0x00000080
#define QML_STS_AXI_SLV_TMO_DEFAULT                                  0x00000000
#define QML_CFG_AXI_SS_CSR_TMO_ADDR                                  0x00000084
#define QML_CFG_AXI_SS_CSR_TMO_DEFAULT                               0x02008000
#define QML_STS_READ_LATENCY_TOT_READ_REQS_ADDR                      0x0000008c
#define QML_STS_READ_LATENCY_TOT_READ_REQS_DEFAULT                   0x00000000

/*	Register CFG_DIAG_SEL	*/ 
/*	 Fields CFG_AXI_NON_AXI_MUX_SELECT	 */
#define QML_CFG_AXI_NON_AXI_MUX_SELECT_WIDTH                                  1
#define QML_CFG_AXI_NON_AXI_MUX_SELECT_SHIFT                                 11
#define QML_CFG_AXI_NON_AXI_MUX_SELECT_MASK                          0x00000800
#define QML_CFG_AXI_NON_AXI_MUX_SELECT_RD(src)       (((src) & 0x00000800)>>11)
#define QML_CFG_AXI_NON_AXI_MUX_SELECT_WR(src)  (((u32)(src)<<11) & 0x00000800)
#define QML_CFG_AXI_NON_AXI_MUX_SELECT_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*	 Fields CFG_MUX_SELECTOR	 */
#define QML_CFG_MUX_SELECTOR_WIDTH                                           11
#define QML_CFG_MUX_SELECTOR_SHIFT                                            0
#define QML_CFG_MUX_SELECTOR_MASK                                    0x000007ff
#define QML_CFG_MUX_SELECTOR_RD(src)                     (((src) & 0x000007ff))
#define QML_CFG_MUX_SELECTOR_WR(src)                (((u32)(src)) & 0x000007ff)
#define QML_CFG_MUX_SELECTOR_SET(dst,src) \
                          (((dst) & ~0x000007ff) | (((u32)(src)) & 0x000007ff))

/*	Register CFG_READ_BW_LAT_ADDR_MASK	*/ 
/*	 Fields READ_ADDR_MASK	 */
#define QML_READ_ADDR_MASK_WIDTH                                             32
#define QML_READ_ADDR_MASK_SHIFT                                              0
#define QML_READ_ADDR_MASK_MASK                                      0xffffffff
#define QML_READ_ADDR_MASK_RD(src)                       (((src) & 0xffffffff))
#define QML_READ_ADDR_MASK_WR(src)                  (((u32)(src)) & 0xffffffff)
#define QML_READ_ADDR_MASK_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_READ_BW_LAT_ADDR_PAT	*/ 
/*	 Fields READ_ADDR_PAT	 */
#define QML_READ_ADDR_PAT_WIDTH                                              32
#define QML_READ_ADDR_PAT_SHIFT                                               0
#define QML_READ_ADDR_PAT_MASK                                       0xffffffff
#define QML_READ_ADDR_PAT_RD(src)                        (((src) & 0xffffffff))
#define QML_READ_ADDR_PAT_WR(src)                   (((u32)(src)) & 0xffffffff)
#define QML_READ_ADDR_PAT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_WRITE_BW_LAT_ADDR_MASK	*/ 
/*	 Fields WRITE_ADDR_MASK	 */
#define QML_WRITE_ADDR_MASK_WIDTH                                            32
#define QML_WRITE_ADDR_MASK_SHIFT                                             0
#define QML_WRITE_ADDR_MASK_MASK                                     0xffffffff
#define QML_WRITE_ADDR_MASK_RD(src)                      (((src) & 0xffffffff))
#define QML_WRITE_ADDR_MASK_WR(src)                 (((u32)(src)) & 0xffffffff)
#define QML_WRITE_ADDR_MASK_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_WRITE_BW_LAT_ADDR_PAT	*/ 
/*	 Fields WRITE_ADDR_PAT	 */
#define QML_WRITE_ADDR_PAT_WIDTH                                             32
#define QML_WRITE_ADDR_PAT_SHIFT                                              0
#define QML_WRITE_ADDR_PAT_MASK                                      0xffffffff
#define QML_WRITE_ADDR_PAT_RD(src)                       (((src) & 0xffffffff))
#define QML_WRITE_ADDR_PAT_WR(src)                  (((u32)(src)) & 0xffffffff)
#define QML_WRITE_ADDR_PAT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_DIAG_START_STOP	*/ 
/*	 Fields START_RD_LATENCY	 */
#define QML_START_RD_LATENCY_WIDTH                                            1
#define QML_START_RD_LATENCY_SHIFT                                            4
#define QML_START_RD_LATENCY_MASK                                    0x00000010
#define QML_START_RD_LATENCY_RD(src)                  (((src) & 0x00000010)>>4)
#define QML_START_RD_LATENCY_WR(src)             (((u32)(src)<<4) & 0x00000010)
#define QML_START_RD_LATENCY_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields START_SRD_BW	 */
#define QML_START_SRD_BW_WIDTH                                                1
#define QML_START_SRD_BW_SHIFT                                                3
#define QML_START_SRD_BW_MASK                                        0x00000008
#define QML_START_SRD_BW_RD(src)                      (((src) & 0x00000008)>>3)
#define QML_START_SRD_BW_WR(src)                 (((u32)(src)<<3) & 0x00000008)
#define QML_START_SRD_BW_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields START_MRD_BW	 */
#define QML_START_MRD_BW_WIDTH                                                1
#define QML_START_MRD_BW_SHIFT                                                2
#define QML_START_MRD_BW_MASK                                        0x00000004
#define QML_START_MRD_BW_RD(src)                      (((src) & 0x00000004)>>2)
#define QML_START_MRD_BW_WR(src)                 (((u32)(src)<<2) & 0x00000004)
#define QML_START_MRD_BW_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields START_SWR_BW	 */
#define QML_START_SWR_BW_WIDTH                                                1
#define QML_START_SWR_BW_SHIFT                                                1
#define QML_START_SWR_BW_MASK                                        0x00000002
#define QML_START_SWR_BW_RD(src)                      (((src) & 0x00000002)>>1)
#define QML_START_SWR_BW_WR(src)                 (((u32)(src)<<1) & 0x00000002)
#define QML_START_SWR_BW_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields START_MWR_BW	 */
#define QML_START_MWR_BW_WIDTH                                                1
#define QML_START_MWR_BW_SHIFT                                                0
#define QML_START_MWR_BW_MASK                                        0x00000001
#define QML_START_MWR_BW_RD(src)                         (((src) & 0x00000001))
#define QML_START_MWR_BW_WR(src)                    (((u32)(src)) & 0x00000001)
#define QML_START_MWR_BW_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register CFG_BW_MSTR_STOP_CNT	*/ 
/*	 Fields MSTR_STOP_RD_CNT	 */
#define QML_MSTR_STOP_RD_CNT_WIDTH                                           16
#define QML_MSTR_STOP_RD_CNT_SHIFT                                           16
#define QML_MSTR_STOP_RD_CNT_MASK                                    0xffff0000
#define QML_MSTR_STOP_RD_CNT_RD(src)                 (((src) & 0xffff0000)>>16)
#define QML_MSTR_STOP_RD_CNT_WR(src)            (((u32)(src)<<16) & 0xffff0000)
#define QML_MSTR_STOP_RD_CNT_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields MSTR_STOP_WR_CNT	 */
#define QML_MSTR_STOP_WR_CNT_WIDTH                                           16
#define QML_MSTR_STOP_WR_CNT_SHIFT                                            0
#define QML_MSTR_STOP_WR_CNT_MASK                                    0x0000ffff
#define QML_MSTR_STOP_WR_CNT_RD(src)                     (((src) & 0x0000ffff))
#define QML_MSTR_STOP_WR_CNT_WR(src)                (((u32)(src)) & 0x0000ffff)
#define QML_MSTR_STOP_WR_CNT_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register CFG_BW_SLV_STOP_CNT	*/ 
/*	 Fields SLV_STOP_RD_CNT	 */
#define QML_SLV_STOP_RD_CNT_WIDTH                                            16
#define QML_SLV_STOP_RD_CNT_SHIFT                                            16
#define QML_SLV_STOP_RD_CNT_MASK                                     0xffff0000
#define QML_SLV_STOP_RD_CNT_RD(src)                  (((src) & 0xffff0000)>>16)
#define QML_SLV_STOP_RD_CNT_WR(src)             (((u32)(src)<<16) & 0xffff0000)
#define QML_SLV_STOP_RD_CNT_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields SLV_STOP_WR_CNT	 */
#define QML_SLV_STOP_WR_CNT_WIDTH                                            16
#define QML_SLV_STOP_WR_CNT_SHIFT                                             0
#define QML_SLV_STOP_WR_CNT_MASK                                     0x0000ffff
#define QML_SLV_STOP_WR_CNT_RD(src)                      (((src) & 0x0000ffff))
#define QML_SLV_STOP_WR_CNT_WR(src)                 (((u32)(src)) & 0x0000ffff)
#define QML_SLV_STOP_WR_CNT_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register STS_READ_LATENCY_OUTPUT	*/ 
/*	 Fields READ_MAX	 */
#define QML_READ_MAX_WIDTH                                                   10
#define QML_READ_MAX_SHIFT                                                   22
#define QML_READ_MAX_MASK                                            0xffc00000
#define QML_READ_MAX_RD(src)                         (((src) & 0xffc00000)>>22)
#define QML_READ_MAX_SET(dst,src) \
                      (((dst) & ~0xffc00000) | (((u32)(src)<<22) & 0xffc00000))
/*	 Fields READ_TOT	 */
#define QML_READ_TOT_WIDTH                                                   22
#define QML_READ_TOT_SHIFT                                                    0
#define QML_READ_TOT_MASK                                            0x003fffff
#define QML_READ_TOT_RD(src)                             (((src) & 0x003fffff))
#define QML_READ_TOT_SET(dst,src) \
                          (((dst) & ~0x003fffff) | (((u32)(src)) & 0x003fffff))

/*	Register STS_AXI_MRD_BW_CLK_CNT	*/ 
/*	 Fields MSTR_READ_BW_CLK_CNT	 */
#define QML_MSTR_READ_BW_CLK_CNT_WIDTH                                       32
#define QML_MSTR_READ_BW_CLK_CNT_SHIFT                                        0
#define QML_MSTR_READ_BW_CLK_CNT_MASK                                0xffffffff
#define QML_MSTR_READ_BW_CLK_CNT_RD(src)                 (((src) & 0xffffffff))
#define QML_MSTR_READ_BW_CLK_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register STS_AXI_MRD_BW_BYTE_CNT	*/ 
/*	 Fields MSTR_READ_BW_BYTE_CNT	 */
#define QML_MSTR_READ_BW_BYTE_CNT_WIDTH                                      32
#define QML_MSTR_READ_BW_BYTE_CNT_SHIFT                                       0
#define QML_MSTR_READ_BW_BYTE_CNT_MASK                               0xffffffff
#define QML_MSTR_READ_BW_BYTE_CNT_RD(src)                (((src) & 0xffffffff))
#define QML_MSTR_READ_BW_BYTE_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register STS_AXI_MWR_BW_CLK_CNT	*/ 
/*	 Fields MSTR_WRITE_BW_CLK_CNT	 */
#define QML_MSTR_WRITE_BW_CLK_CNT_WIDTH                                      32
#define QML_MSTR_WRITE_BW_CLK_CNT_SHIFT                                       0
#define QML_MSTR_WRITE_BW_CLK_CNT_MASK                               0xffffffff
#define QML_MSTR_WRITE_BW_CLK_CNT_RD(src)                (((src) & 0xffffffff))
#define QML_MSTR_WRITE_BW_CLK_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register STS_AXI_MWR_BW_BYTE_CNT	*/ 
/*	 Fields MSTR_WRITE_BW_BYTE_CNT	 */
#define QML_MSTR_WRITE_BW_BYTE_CNT_WIDTH                                     32
#define QML_MSTR_WRITE_BW_BYTE_CNT_SHIFT                                      0
#define QML_MSTR_WRITE_BW_BYTE_CNT_MASK                              0xffffffff
#define QML_MSTR_WRITE_BW_BYTE_CNT_RD(src)               (((src) & 0xffffffff))
#define QML_MSTR_WRITE_BW_BYTE_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register STS_AXI_SRD_BW_CLK_CNT	*/ 
/*	 Fields SLV_READ_BW_CLK_CNT	 */
#define QML_SLV_READ_BW_CLK_CNT_WIDTH                                        32
#define QML_SLV_READ_BW_CLK_CNT_SHIFT                                         0
#define QML_SLV_READ_BW_CLK_CNT_MASK                                 0xffffffff
#define QML_SLV_READ_BW_CLK_CNT_RD(src)                  (((src) & 0xffffffff))
#define QML_SLV_READ_BW_CLK_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register STS_AXI_SRD_BW_BYTE_CNT	*/ 
/*	 Fields SLV_READ_BW_BYTE_CNT	 */
#define QML_SLV_READ_BW_BYTE_CNT_WIDTH                                       32
#define QML_SLV_READ_BW_BYTE_CNT_SHIFT                                        0
#define QML_SLV_READ_BW_BYTE_CNT_MASK                                0xffffffff
#define QML_SLV_READ_BW_BYTE_CNT_RD(src)                 (((src) & 0xffffffff))
#define QML_SLV_READ_BW_BYTE_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register STS_AXI_SWR_BW_CLK_CNT	*/ 
/*	 Fields SLV_WRITE_BW_CLK_CNT	 */
#define QML_SLV_WRITE_BW_CLK_CNT_WIDTH                                       32
#define QML_SLV_WRITE_BW_CLK_CNT_SHIFT                                        0
#define QML_SLV_WRITE_BW_CLK_CNT_MASK                                0xffffffff
#define QML_SLV_WRITE_BW_CLK_CNT_RD(src)                 (((src) & 0xffffffff))
#define QML_SLV_WRITE_BW_CLK_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register STS_AXI_SWR_BW_BYTE_CNT	*/ 
/*	 Fields SLV_WRITE_BW_BYTE_CNT	 */
#define QML_SLV_WRITE_BW_BYTE_CNT_WIDTH                                      32
#define QML_SLV_WRITE_BW_BYTE_CNT_SHIFT                                       0
#define QML_SLV_WRITE_BW_BYTE_CNT_MASK                               0xffffffff
#define QML_SLV_WRITE_BW_BYTE_CNT_RD(src)                (((src) & 0xffffffff))
#define QML_SLV_WRITE_BW_BYTE_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_DBG_TRIG_CTRL	*/ 
/*	 Fields TRIG_EN	 */
#define QML_TRIG_EN_WIDTH                                                     1
#define QML_TRIG_EN_SHIFT                                                     4
#define QML_TRIG_EN_MASK                                             0x00000010
#define QML_TRIG_EN_RD(src)                           (((src) & 0x00000010)>>4)
#define QML_TRIG_EN_WR(src)                      (((u32)(src)<<4) & 0x00000010)
#define QML_TRIG_EN_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields AND_E	 */
#define QML_AND_E_WIDTH                                                       2
#define QML_AND_E_SHIFT                                                       2
#define QML_AND_E_MASK                                               0x0000000c
#define QML_AND_E_RD(src)                             (((src) & 0x0000000c)>>2)
#define QML_AND_E_WR(src)                        (((u32)(src)<<2) & 0x0000000c)
#define QML_AND_E_SET(dst,src) \
                       (((dst) & ~0x0000000c) | (((u32)(src)<<2) & 0x0000000c))
/*	 Fields OR_E	 */
#define QML_OR_E_WIDTH                                                        2
#define QML_OR_E_SHIFT                                                        0
#define QML_OR_E_MASK                                                0x00000003
#define QML_OR_E_RD(src)                                 (((src) & 0x00000003))
#define QML_OR_E_WR(src)                            (((u32)(src)) & 0x00000003)
#define QML_OR_E_SET(dst,src) \
                          (((dst) & ~0x00000003) | (((u32)(src)) & 0x00000003))

/*	Register CFG_DBG_PAT_REG_0	*/ 
/*	 Fields PATTERN	 */
#define QML_PATTERN0_WIDTH                                                   32
#define QML_PATTERN0_SHIFT                                                    0
#define QML_PATTERN0_MASK                                            0xffffffff
#define QML_PATTERN0_RD(src)                             (((src) & 0xffffffff))
#define QML_PATTERN0_WR(src)                        (((u32)(src)) & 0xffffffff)
#define QML_PATTERN0_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_DBG_PAT_MASK_REG_0	*/ 
/*	 Fields PAT_MASK	 */
#define QML_PAT_MASK0_WIDTH                                                  32
#define QML_PAT_MASK0_SHIFT                                                   0
#define QML_PAT_MASK0_MASK                                           0xffffffff
#define QML_PAT_MASK0_RD(src)                            (((src) & 0xffffffff))
#define QML_PAT_MASK0_WR(src)                       (((u32)(src)) & 0xffffffff)
#define QML_PAT_MASK0_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_DBG_PAT_REG_1	*/ 
/*	 Fields PATTERN	 */
#define QML_PATTERN1_WIDTH                                                   32
#define QML_PATTERN1_SHIFT                                                    0
#define QML_PATTERN1_MASK                                            0xffffffff
#define QML_PATTERN1_RD(src)                             (((src) & 0xffffffff))
#define QML_PATTERN1_WR(src)                        (((u32)(src)) & 0xffffffff)
#define QML_PATTERN1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_DBG_PAT_MASK_REG_1	*/ 
/*	 Fields PAT_MASK	 */
#define QML_PAT_MASK1_WIDTH                                                  32
#define QML_PAT_MASK1_SHIFT                                                   0
#define QML_PAT_MASK1_MASK                                           0xffffffff
#define QML_PAT_MASK1_RD(src)                            (((src) & 0xffffffff))
#define QML_PAT_MASK1_WR(src)                       (((u32)(src)) & 0xffffffff)
#define QML_PAT_MASK1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register DBG_TRIG_OUT	*/ 
/*	 Fields DBG_OUT	 */
#define QML_DBG_OUT_WIDTH                                                    32
#define QML_DBG_OUT_SHIFT                                                     0
#define QML_DBG_OUT_MASK                                             0xffffffff
#define QML_DBG_OUT_RD(src)                              (((src) & 0xffffffff))
#define QML_DBG_OUT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register DBG_ECC_INT_OUT	*/ 
/*	 Fields SLV_TMO	 */
#define QML_SLV_TMO_WIDTH                                                     1
#define QML_SLV_TMO_SHIFT                                                     3
#define QML_SLV_TMO_MASK                                             0x00000008
#define QML_SLV_TMO_RD(src)                           (((src) & 0x00000008)>>3)
#define QML_SLV_TMO_WR(src)                      (((u32)(src)<<3) & 0x00000008)
#define QML_SLV_TMO_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields SECC_INT	 */
#define QML_SECC_INT_WIDTH                                                    1
#define QML_SECC_INT_SHIFT                                                    2
#define QML_SECC_INT_MASK                                            0x00000004
#define QML_SECC_INT_RD(src)                          (((src) & 0x00000004)>>2)
#define QML_SECC_INT_WR(src)                     (((u32)(src)<<2) & 0x00000004)
#define QML_SECC_INT_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields DECC_INT	 */
#define QML_DECC_INT_WIDTH                                                    1
#define QML_DECC_INT_SHIFT                                                    1
#define QML_DECC_INT_MASK                                            0x00000002
#define QML_DECC_INT_RD(src)                          (((src) & 0x00000002)>>1)
#define QML_DECC_INT_WR(src)                     (((u32)(src)<<1) & 0x00000002)
#define QML_DECC_INT_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields DBG_INT	 */
#define QML_DBG_INT_WIDTH                                                     1
#define QML_DBG_INT_SHIFT                                                     0
#define QML_DBG_INT_MASK                                             0x00000001
#define QML_DBG_INT_RD(src)                              (((src) & 0x00000001))
#define QML_DBG_INT_WR(src)                         (((u32)(src)) & 0x00000001)
#define QML_DBG_INT_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register DBG_ECC_INT_OUTMask	*/
/*    Mask Register Fields SLV_TMOMask    */
#define QML_SLV_TMOMASK_WIDTH                                                 1
#define QML_SLV_TMOMASK_SHIFT                                                 3
#define QML_SLV_TMOMASK_MASK                                         0x00000008
#define QML_SLV_TMOMASK_RD(src)                       (((src) & 0x00000008)>>3)
#define QML_SLV_TMOMASK_WR(src)                  (((u32)(src)<<3) & 0x00000008)
#define QML_SLV_TMOMASK_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*    Mask Register Fields SECC_INTMask    */
#define QML_SECC_INTMASK_WIDTH                                                1
#define QML_SECC_INTMASK_SHIFT                                                2
#define QML_SECC_INTMASK_MASK                                        0x00000004
#define QML_SECC_INTMASK_RD(src)                      (((src) & 0x00000004)>>2)
#define QML_SECC_INTMASK_WR(src)                 (((u32)(src)<<2) & 0x00000004)
#define QML_SECC_INTMASK_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*    Mask Register Fields DECC_INTMask    */
#define QML_DECC_INTMASK_WIDTH                                                1
#define QML_DECC_INTMASK_SHIFT                                                1
#define QML_DECC_INTMASK_MASK                                        0x00000002
#define QML_DECC_INTMASK_RD(src)                      (((src) & 0x00000002)>>1)
#define QML_DECC_INTMASK_WR(src)                 (((u32)(src)<<1) & 0x00000002)
#define QML_DECC_INTMASK_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*    Mask Register Fields DBG_INTMask    */
#define QML_DBG_INTMASK_WIDTH                                                 1
#define QML_DBG_INTMASK_SHIFT                                                 0
#define QML_DBG_INTMASK_MASK                                         0x00000001
#define QML_DBG_INTMASK_RD(src)                          (((src) & 0x00000001))
#define QML_DBG_INTMASK_WR(src)                     (((u32)(src)) & 0x00000001)
#define QML_DBG_INTMASK_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register CFG_SECC_INT_THRSH_REG	*/ 
/*	 Fields SECC_INT_THRSH	 */
#define QML_SECC_INT_THRSH_WIDTH                                              8
#define QML_SECC_INT_THRSH_SHIFT                                              0
#define QML_SECC_INT_THRSH_MASK                                      0x000000ff
#define QML_SECC_INT_THRSH_RD(src)                       (((src) & 0x000000ff))
#define QML_SECC_INT_THRSH_WR(src)                  (((u32)(src)) & 0x000000ff)
#define QML_SECC_INT_THRSH_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register CFG_MEM_ECC_BYPASS	*/ 
/*	 Fields CFG_ECC_BYPASS	 */
#define QML_CFG_ECC_BYPASS_WIDTH                                             16
#define QML_CFG_ECC_BYPASS_SHIFT                                              0
#define QML_CFG_ECC_BYPASS_MASK                                      0x0000ffff
#define QML_CFG_ECC_BYPASS_RD(src)                       (((src) & 0x0000ffff))
#define QML_CFG_ECC_BYPASS_WR(src)                  (((u32)(src)) & 0x0000ffff)
#define QML_CFG_ECC_BYPASS_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register CFG_MEM_PWRDN_DIS	*/ 
/*	 Fields CFG_ECC_PWRDN_DIS	 */
#define QML_CFG_ECC_PWRDN_DIS_WIDTH                                          16
#define QML_CFG_ECC_PWRDN_DIS_SHIFT                                          16
#define QML_CFG_ECC_PWRDN_DIS_MASK                                   0xffff0000
#define QML_CFG_ECC_PWRDN_DIS_RD(src)                (((src) & 0xffff0000)>>16)
#define QML_CFG_ECC_PWRDN_DIS_WR(src)           (((u32)(src)<<16) & 0xffff0000)
#define QML_CFG_ECC_PWRDN_DIS_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields CFG_PWRDN_DIS	 */
#define QML_CFG_PWRDN_DIS_WIDTH                                              16
#define QML_CFG_PWRDN_DIS_SHIFT                                               0
#define QML_CFG_PWRDN_DIS_MASK                                       0x0000ffff
#define QML_CFG_PWRDN_DIS_RD(src)                        (((src) & 0x0000ffff))
#define QML_CFG_PWRDN_DIS_WR(src)                   (((u32)(src)) & 0x0000ffff)
#define QML_CFG_PWRDN_DIS_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register CFG_MEM_RAM_SHUTDOWN	*/ 
/*	 Fields CFG_RAM_SHUTDOWN_EN	 */
#define QML_CFG_RAM_SHUTDOWN_EN_WIDTH                                        32
#define QML_CFG_RAM_SHUTDOWN_EN_SHIFT                                         0
#define QML_CFG_RAM_SHUTDOWN_EN_MASK                                 0xffffffff
#define QML_CFG_RAM_SHUTDOWN_EN_RD(src)                  (((src) & 0xffffffff))
#define QML_CFG_RAM_SHUTDOWN_EN_WR(src)             (((u32)(src)) & 0xffffffff)
#define QML_CFG_RAM_SHUTDOWN_EN_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register BLOCK_MEM_RDY	*/ 
/*	 Fields MEM_RDY	 */
#define QML_MEM_RDY_WIDTH                                                    32
#define QML_MEM_RDY_SHIFT                                                     0
#define QML_MEM_RDY_MASK                                             0xffffffff
#define QML_MEM_RDY_RD(src)                              (((src) & 0xffffffff))
#define QML_MEM_RDY_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_AXI_SLV_RD_TMO	*/ 
/*	 Fields SLV_ID_TMO_ENABLE	 */
#define QML_SLV_ID_TMO_ENABLE_WIDTH                                           1
#define QML_SLV_ID_TMO_ENABLE_SHIFT                                          31
#define QML_SLV_ID_TMO_ENABLE_MASK                                   0x80000000
#define QML_SLV_ID_TMO_ENABLE_RD(src)                (((src) & 0x80000000)>>31)
#define QML_SLV_ID_TMO_ENABLE_WR(src)           (((u32)(src)<<31) & 0x80000000)
#define QML_SLV_ID_TMO_ENABLE_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields SS_ID_TRK_ENABLE	 */
#define QML_SS_ID_TRK_ENABLE_WIDTH                                            1
#define QML_SS_ID_TRK_ENABLE_SHIFT                                           30
#define QML_SS_ID_TRK_ENABLE_MASK                                    0x40000000
#define QML_SS_ID_TRK_ENABLE_RD(src)                 (((src) & 0x40000000)>>30)
#define QML_SS_ID_TRK_ENABLE_WR(src)            (((u32)(src)<<30) & 0x40000000)
#define QML_SS_ID_TRK_ENABLE_SET(dst,src) \
                      (((dst) & ~0x40000000) | (((u32)(src)<<30) & 0x40000000))
/*	 Fields SLV_ID_TMO_VALUE	 */
#define QML_SLV_ID_TMO_VALUE_WIDTH                                           10
#define QML_SLV_ID_TMO_VALUE_SHIFT                                           16
#define QML_SLV_ID_TMO_VALUE_MASK                                    0x03ff0000
#define QML_SLV_ID_TMO_VALUE_RD(src)                 (((src) & 0x03ff0000)>>16)
#define QML_SLV_ID_TMO_VALUE_WR(src)            (((u32)(src)<<16) & 0x03ff0000)
#define QML_SLV_ID_TMO_VALUE_SET(dst,src) \
                      (((dst) & ~0x03ff0000) | (((u32)(src)<<16) & 0x03ff0000))
/*	 Fields SLV_RDY_TMO_ENABLE	 */
#define QML_SLV_RDY_TMO_ENABLE_WIDTH                                          1
#define QML_SLV_RDY_TMO_ENABLE_SHIFT                                         15
#define QML_SLV_RDY_TMO_ENABLE_MASK                                  0x00008000
#define QML_SLV_RDY_TMO_ENABLE_RD(src)               (((src) & 0x00008000)>>15)
#define QML_SLV_RDY_TMO_ENABLE_WR(src)          (((u32)(src)<<15) & 0x00008000)
#define QML_SLV_RDY_TMO_ENABLE_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))
/*	 Fields SLV_RDY_TMO_VALUE	 */
#define QML_SLV_RDY_TMO_VALUE_WIDTH                                          10
#define QML_SLV_RDY_TMO_VALUE_SHIFT                                           0
#define QML_SLV_RDY_TMO_VALUE_MASK                                   0x000003ff
#define QML_SLV_RDY_TMO_VALUE_RD(src)                    (((src) & 0x000003ff))
#define QML_SLV_RDY_TMO_VALUE_WR(src)               (((u32)(src)) & 0x000003ff)
#define QML_SLV_RDY_TMO_VALUE_SET(dst,src) \
                          (((dst) & ~0x000003ff) | (((u32)(src)) & 0x000003ff))

/*	Register CFG_AXI_SLV_WR_TMO	*/ 
/*	 Fields SLV_ID_TMO_ENABLE	 */
#define QML_SLV_ID_TMO_ENABLE_F1_WIDTH                                        1
#define QML_SLV_ID_TMO_ENABLE_F1_SHIFT                                       31
#define QML_SLV_ID_TMO_ENABLE_F1_MASK                                0x80000000
#define QML_SLV_ID_TMO_ENABLE_F1_RD(src)             (((src) & 0x80000000)>>31)
#define QML_SLV_ID_TMO_ENABLE_F1_WR(src)        (((u32)(src)<<31) & 0x80000000)
#define QML_SLV_ID_TMO_ENABLE_F1_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields SS_ID_TRK_ENABLE	 */
#define QML_SS_ID_TRK_ENABLE_F1_WIDTH                                         1
#define QML_SS_ID_TRK_ENABLE_F1_SHIFT                                        30
#define QML_SS_ID_TRK_ENABLE_F1_MASK                                 0x40000000
#define QML_SS_ID_TRK_ENABLE_F1_RD(src)              (((src) & 0x40000000)>>30)
#define QML_SS_ID_TRK_ENABLE_F1_WR(src)         (((u32)(src)<<30) & 0x40000000)
#define QML_SS_ID_TRK_ENABLE_F1_SET(dst,src) \
                      (((dst) & ~0x40000000) | (((u32)(src)<<30) & 0x40000000))
/*	 Fields SLV_ID_TMO_VALUE	 */
#define QML_SLV_ID_TMO_VALUE_F1_WIDTH                                        10
#define QML_SLV_ID_TMO_VALUE_F1_SHIFT                                        16
#define QML_SLV_ID_TMO_VALUE_F1_MASK                                 0x03ff0000
#define QML_SLV_ID_TMO_VALUE_F1_RD(src)              (((src) & 0x03ff0000)>>16)
#define QML_SLV_ID_TMO_VALUE_F1_WR(src)         (((u32)(src)<<16) & 0x03ff0000)
#define QML_SLV_ID_TMO_VALUE_F1_SET(dst,src) \
                      (((dst) & ~0x03ff0000) | (((u32)(src)<<16) & 0x03ff0000))
/*	 Fields SLV_RDY_TMO_ENABLE	 */
#define QML_SLV_RDY_TMO_ENABLE_F1_WIDTH                                       1
#define QML_SLV_RDY_TMO_ENABLE_F1_SHIFT                                      15
#define QML_SLV_RDY_TMO_ENABLE_F1_MASK                               0x00008000
#define QML_SLV_RDY_TMO_ENABLE_F1_RD(src)            (((src) & 0x00008000)>>15)
#define QML_SLV_RDY_TMO_ENABLE_F1_WR(src)       (((u32)(src)<<15) & 0x00008000)
#define QML_SLV_RDY_TMO_ENABLE_F1_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))
/*	 Fields SLV_RDY_TMO_VALUE	 */
#define QML_SLV_RDY_TMO_VALUE_F1_WIDTH                                       10
#define QML_SLV_RDY_TMO_VALUE_F1_SHIFT                                        0
#define QML_SLV_RDY_TMO_VALUE_F1_MASK                                0x000003ff
#define QML_SLV_RDY_TMO_VALUE_F1_RD(src)                 (((src) & 0x000003ff))
#define QML_SLV_RDY_TMO_VALUE_F1_WR(src)            (((u32)(src)) & 0x000003ff)
#define QML_SLV_RDY_TMO_VALUE_F1_SET(dst,src) \
                          (((dst) & ~0x000003ff) | (((u32)(src)) & 0x000003ff))

/*	Register STS_AXI_SLV_TMO	*/ 
/*	 Fields SLV_ARID_TMO	 */
#define QML_SLV_ARID_TMO_WIDTH                                                1
#define QML_SLV_ARID_TMO_SHIFT                                                3
#define QML_SLV_ARID_TMO_MASK                                        0x00000008
#define QML_SLV_ARID_TMO_RD(src)                      (((src) & 0x00000008)>>3)
#define QML_SLV_ARID_TMO_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields SLV_ARRDY_TMO	 */
#define QML_SLV_ARRDY_TMO_WIDTH                                               1
#define QML_SLV_ARRDY_TMO_SHIFT                                               2
#define QML_SLV_ARRDY_TMO_MASK                                       0x00000004
#define QML_SLV_ARRDY_TMO_RD(src)                     (((src) & 0x00000004)>>2)
#define QML_SLV_ARRDY_TMO_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields SLV_AWID_TMO	 */
#define QML_SLV_AWID_TMO_WIDTH                                                1
#define QML_SLV_AWID_TMO_SHIFT                                                1
#define QML_SLV_AWID_TMO_MASK                                        0x00000002
#define QML_SLV_AWID_TMO_RD(src)                      (((src) & 0x00000002)>>1)
#define QML_SLV_AWID_TMO_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields SLV_AWRDY_TMO	 */
#define QML_SLV_AWRDY_TMO_WIDTH                                               1
#define QML_SLV_AWRDY_TMO_SHIFT                                               0
#define QML_SLV_AWRDY_TMO_MASK                                       0x00000001
#define QML_SLV_AWRDY_TMO_RD(src)                        (((src) & 0x00000001))
#define QML_SLV_AWRDY_TMO_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register CFG_AXI_SS_CSR_TMO	*/ 
/*	 Fields CFG_CSR_TMO	 */
#define QML_CFG_CSR_TMO_WIDTH                                                16
#define QML_CFG_CSR_TMO_SHIFT                                                16
#define QML_CFG_CSR_TMO_MASK                                         0xffff0000
#define QML_CFG_CSR_TMO_RD(src)                      (((src) & 0xffff0000)>>16)
#define QML_CFG_CSR_TMO_WR(src)                 (((u32)(src)<<16) & 0xffff0000)
#define QML_CFG_CSR_TMO_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields CSR_ERR_RESP_EN	 */
#define QML_CSR_ERR_RESP_EN_WIDTH                                             1
#define QML_CSR_ERR_RESP_EN_SHIFT                                            15
#define QML_CSR_ERR_RESP_EN_MASK                                     0x00008000
#define QML_CSR_ERR_RESP_EN_RD(src)                  (((src) & 0x00008000)>>15)
#define QML_CSR_ERR_RESP_EN_WR(src)             (((u32)(src)<<15) & 0x00008000)
#define QML_CSR_ERR_RESP_EN_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))

/*	Register STS_READ_LATENCY_TOT_READ_REQS	*/ 
/*	 Fields TOT_READS	 */
#define QML_TOT_READS_WIDTH                                                  16
#define QML_TOT_READS_SHIFT                                                   0
#define QML_TOT_READS_MASK                                           0x0000ffff
#define QML_TOT_READS_RD(src)                            (((src) & 0x0000ffff))
#define QML_TOT_READS_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Global Base Address	*/
#define MPIC_QML_AMPP_CSR_BASE_ADDR			0xddd8d0000ULL

/*    Address MPIC_QML_AMPP_CSR  Registers */
#define QML_AMPP_CTL_ADDR                                            0x00008000
#define QML_AMPP_CTL_DEFAULT                                         0x00000000
#define QML_AMPP_DOMAIN_ID_ADDR                                      0x00008004
#define QML_AMPP_DOMAIN_ID_DEFAULT                                   0x00000000
#define QML_AMPP_RD_VIO_LOG_0_ADDR                                   0x00008008
#define QML_AMPP_RD_VIO_LOG_0_DEFAULT                                0x00000000
#define QML_AMPP_RD_VIO_LOG_1_ADDR                                   0x0000800c
#define QML_AMPP_RD_VIO_LOG_1_DEFAULT                                0x00000000
#define QML_AMPP_WR_VIO_LOG_0_ADDR                                   0x00008010
#define QML_AMPP_WR_VIO_LOG_0_DEFAULT                                0x00000000
#define QML_AMPP_WR_VIO_LOG_1_ADDR                                   0x00008014
#define QML_AMPP_WR_VIO_LOG_1_DEFAULT                                0x00000000
#define QML_AMPP_INT_ADDR                                            0x00008018
#define QML_AMPP_INT_DEFAULT                                         0x00000000
#define QML_AMPP_INTMASK_ADDR                                        0x0000801c
#define QML_AMPP_DMT_SA_0_ADDR                                       0x00008050
#define QML_AMPP_DMT_SA_0_DEFAULT                                    0x00000000
#define QML_AMPP_DMT_EA_0_ADDR                                       0x00008054
#define QML_AMPP_DMT_EA_0_DEFAULT                                    0x00000000
#define QML_AMPP_DMT_RW_0_ADDR                                       0x00008058
#define QML_AMPP_DMT_RW_0_DEFAULT                                    0x000000ff
#define QML_AMPP_DMT_SA_1_ADDR                                       0x00008060
#define QML_AMPP_DMT_SA_1_DEFAULT                                    0x00000000
#define QML_AMPP_DMT_EA_1_ADDR                                       0x00008064
#define QML_AMPP_DMT_EA_1_DEFAULT                                    0x00000000
#define QML_AMPP_DMT_RW_1_ADDR                                       0x00008068
#define QML_AMPP_DMT_RW_1_DEFAULT                                    0x000000ff

/*	Register AMPP_CTL	*/ 
/*	 Fields CFG_MST_VALID	 */
#define QML_CFG_MST_VALID_WIDTH                                               1
#define QML_CFG_MST_VALID_SHIFT                                              31
#define QML_CFG_MST_VALID_MASK                                       0x80000000
#define QML_CFG_MST_VALID_RD(src)                    (((src) & 0x80000000)>>31)
#define QML_CFG_MST_VALID_WR(src)               (((u32)(src)<<31) & 0x80000000)
#define QML_CFG_MST_VALID_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields PPC1_CFG_MST	 */
#define QML_PPC1_CFG_MST_WIDTH                                                1
#define QML_PPC1_CFG_MST_SHIFT                                                2
#define QML_PPC1_CFG_MST_MASK                                        0x00000004
#define QML_PPC1_CFG_MST_RD(src)                      (((src) & 0x00000004)>>2)
#define QML_PPC1_CFG_MST_WR(src)                 (((u32)(src)<<2) & 0x00000004)
#define QML_PPC1_CFG_MST_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields PPC0_CFG_MST	 */
#define QML_PPC0_CFG_MST_WIDTH                                                1
#define QML_PPC0_CFG_MST_SHIFT                                                1
#define QML_PPC0_CFG_MST_MASK                                        0x00000002
#define QML_PPC0_CFG_MST_RD(src)                      (((src) & 0x00000002)>>1)
#define QML_PPC0_CFG_MST_WR(src)                 (((u32)(src)<<1) & 0x00000002)
#define QML_PPC0_CFG_MST_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields IPP_CFG_MST	 */
#define QML_IPP_CFG_MST_WIDTH                                                 1
#define QML_IPP_CFG_MST_SHIFT                                                 0
#define QML_IPP_CFG_MST_MASK                                         0x00000001
#define QML_IPP_CFG_MST_RD(src)                          (((src) & 0x00000001))
#define QML_IPP_CFG_MST_WR(src)                     (((u32)(src)) & 0x00000001)
#define QML_IPP_CFG_MST_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register AMPP_DOMAIN_ID	*/ 
/*	 Fields ID_7	 */
#define QML_ID_7_WIDTH                                                        2
#define QML_ID_7_SHIFT                                                       14
#define QML_ID_7_MASK                                                0x0000c000
#define QML_ID_7_RD(src)                             (((src) & 0x0000c000)>>14)
#define QML_ID_7_WR(src)                        (((u32)(src)<<14) & 0x0000c000)
#define QML_ID_7_SET(dst,src) \
                      (((dst) & ~0x0000c000) | (((u32)(src)<<14) & 0x0000c000))
/*	 Fields ID_6	 */
#define QML_ID_6_WIDTH                                                        2
#define QML_ID_6_SHIFT                                                       12
#define QML_ID_6_MASK                                                0x00003000
#define QML_ID_6_RD(src)                             (((src) & 0x00003000)>>12)
#define QML_ID_6_WR(src)                        (((u32)(src)<<12) & 0x00003000)
#define QML_ID_6_SET(dst,src) \
                      (((dst) & ~0x00003000) | (((u32)(src)<<12) & 0x00003000))
/*	 Fields ID_5	 */
#define QML_ID_5_WIDTH                                                        2
#define QML_ID_5_SHIFT                                                       10
#define QML_ID_5_MASK                                                0x00000c00
#define QML_ID_5_RD(src)                             (((src) & 0x00000c00)>>10)
#define QML_ID_5_WR(src)                        (((u32)(src)<<10) & 0x00000c00)
#define QML_ID_5_SET(dst,src) \
                      (((dst) & ~0x00000c00) | (((u32)(src)<<10) & 0x00000c00))
/*	 Fields ID_4	 */
#define QML_ID_4_WIDTH                                                        2
#define QML_ID_4_SHIFT                                                        8
#define QML_ID_4_MASK                                                0x00000300
#define QML_ID_4_RD(src)                              (((src) & 0x00000300)>>8)
#define QML_ID_4_WR(src)                         (((u32)(src)<<8) & 0x00000300)
#define QML_ID_4_SET(dst,src) \
                       (((dst) & ~0x00000300) | (((u32)(src)<<8) & 0x00000300))
/*	 Fields ID_3	 */
#define QML_ID_3_WIDTH                                                        2
#define QML_ID_3_SHIFT                                                        6
#define QML_ID_3_MASK                                                0x000000c0
#define QML_ID_3_RD(src)                              (((src) & 0x000000c0)>>6)
#define QML_ID_3_WR(src)                         (((u32)(src)<<6) & 0x000000c0)
#define QML_ID_3_SET(dst,src) \
                       (((dst) & ~0x000000c0) | (((u32)(src)<<6) & 0x000000c0))
/*	 Fields ID_2	 */
#define QML_ID_2_WIDTH                                                        2
#define QML_ID_2_SHIFT                                                        4
#define QML_ID_2_MASK                                                0x00000030
#define QML_ID_2_RD(src)                              (((src) & 0x00000030)>>4)
#define QML_ID_2_WR(src)                         (((u32)(src)<<4) & 0x00000030)
#define QML_ID_2_SET(dst,src) \
                       (((dst) & ~0x00000030) | (((u32)(src)<<4) & 0x00000030))
/*	 Fields ID_1	 */
#define QML_ID_1_WIDTH                                                        2
#define QML_ID_1_SHIFT                                                        2
#define QML_ID_1_MASK                                                0x0000000c
#define QML_ID_1_RD(src)                              (((src) & 0x0000000c)>>2)
#define QML_ID_1_WR(src)                         (((u32)(src)<<2) & 0x0000000c)
#define QML_ID_1_SET(dst,src) \
                       (((dst) & ~0x0000000c) | (((u32)(src)<<2) & 0x0000000c))
/*	 Fields ID_0	 */
#define QML_ID_0_WIDTH                                                        2
#define QML_ID_0_SHIFT                                                        0
#define QML_ID_0_MASK                                                0x00000003
#define QML_ID_0_RD(src)                                 (((src) & 0x00000003))
#define QML_ID_0_WR(src)                            (((u32)(src)) & 0x00000003)
#define QML_ID_0_SET(dst,src) \
                          (((dst) & ~0x00000003) | (((u32)(src)) & 0x00000003))

/*	Register AMPP_RD_VIO_LOG_0	*/ 
/*	 Fields RW_ACCESS	 */
#define QML_RW_ACCESS0_WIDTH                                                  2
#define QML_RW_ACCESS0_SHIFT                                                 30
#define QML_RW_ACCESS0_MASK                                          0xc0000000
#define QML_RW_ACCESS0_RD(src)                       (((src) & 0xc0000000)>>30)
#define QML_RW_ACCESS0_SET(dst,src) \
                      (((dst) & ~0xc0000000) | (((u32)(src)<<30) & 0xc0000000))
/*	 Fields DOMAIN_ID	 */
#define QML_DOMAIN_ID0_WIDTH                                                  3
#define QML_DOMAIN_ID0_SHIFT                                                 24
#define QML_DOMAIN_ID0_MASK                                          0x07000000
#define QML_DOMAIN_ID0_RD(src)                       (((src) & 0x07000000)>>24)
#define QML_DOMAIN_ID0_SET(dst,src) \
                      (((dst) & ~0x07000000) | (((u32)(src)<<24) & 0x07000000))
/*	 Fields ADDRESS_36_12	 */
#define QML_ADDRESS_36_120_WIDTH                                             24
#define QML_ADDRESS_36_120_SHIFT                                              0
#define QML_ADDRESS_36_120_MASK                                      0x00ffffff
#define QML_ADDRESS_36_120_RD(src)                       (((src) & 0x00ffffff))
#define QML_ADDRESS_36_120_SET(dst,src) \
                          (((dst) & ~0x00ffffff) | (((u32)(src)) & 0x00ffffff))

/*	Register AMPP_RD_VIO_LOG_1	*/ 
/*	 Fields TAG_9_5	 */
#define QML_TAG_9_51_WIDTH                                                    5
#define QML_TAG_9_51_SHIFT                                                    5
#define QML_TAG_9_51_MASK                                            0x000003e0
#define QML_TAG_9_51_RD(src)                          (((src) & 0x000003e0)>>5)
#define QML_TAG_9_51_SET(dst,src) \
                       (((dst) & ~0x000003e0) | (((u32)(src)<<5) & 0x000003e0))
/*	 Fields TAG_4_0	 */
#define QML_TAG_4_01_WIDTH                                                    5
#define QML_TAG_4_01_SHIFT                                                    0
#define QML_TAG_4_01_MASK                                            0x0000001f
#define QML_TAG_4_01_RD(src)                             (((src) & 0x0000001f))
#define QML_TAG_4_01_SET(dst,src) \
                          (((dst) & ~0x0000001f) | (((u32)(src)) & 0x0000001f))

/*	Register AMPP_WR_VIO_LOG_0	*/ 
/*	 Fields RW_ACCESS	 */
#define QML_RW_ACCESS0_F1_WIDTH                                               2
#define QML_RW_ACCESS0_F1_SHIFT                                              30
#define QML_RW_ACCESS0_F1_MASK                                       0xc0000000
#define QML_RW_ACCESS0_F1_RD(src)                    (((src) & 0xc0000000)>>30)
#define QML_RW_ACCESS0_F1_SET(dst,src) \
                      (((dst) & ~0xc0000000) | (((u32)(src)<<30) & 0xc0000000))
/*	 Fields DOMAIN_ID	 */
#define QML_DOMAIN_ID0_F1_WIDTH                                               3
#define QML_DOMAIN_ID0_F1_SHIFT                                              24
#define QML_DOMAIN_ID0_F1_MASK                                       0x07000000
#define QML_DOMAIN_ID0_F1_RD(src)                    (((src) & 0x07000000)>>24)
#define QML_DOMAIN_ID0_F1_SET(dst,src) \
                      (((dst) & ~0x07000000) | (((u32)(src)<<24) & 0x07000000))
/*	 Fields ADDRESS_36_12	 */
#define QML_ADDRESS_36_120_F1_WIDTH                                          24
#define QML_ADDRESS_36_120_F1_SHIFT                                           0
#define QML_ADDRESS_36_120_F1_MASK                                   0x00ffffff
#define QML_ADDRESS_36_120_F1_RD(src)                    (((src) & 0x00ffffff))
#define QML_ADDRESS_36_120_F1_SET(dst,src) \
                          (((dst) & ~0x00ffffff) | (((u32)(src)) & 0x00ffffff))

/*	Register AMPP_WR_VIO_LOG_1	*/ 
/*	 Fields TAG_9_5	 */
#define QML_TAG_9_51_F1_WIDTH                                                 5
#define QML_TAG_9_51_F1_SHIFT                                                 5
#define QML_TAG_9_51_F1_MASK                                         0x000003e0
#define QML_TAG_9_51_F1_RD(src)                       (((src) & 0x000003e0)>>5)
#define QML_TAG_9_51_F1_SET(dst,src) \
                       (((dst) & ~0x000003e0) | (((u32)(src)<<5) & 0x000003e0))
/*	 Fields TAG_4_0	 */
#define QML_TAG_4_01_F1_WIDTH                                                 5
#define QML_TAG_4_01_F1_SHIFT                                                 0
#define QML_TAG_4_01_F1_MASK                                         0x0000001f
#define QML_TAG_4_01_F1_RD(src)                          (((src) & 0x0000001f))
#define QML_TAG_4_01_F1_SET(dst,src) \
                          (((dst) & ~0x0000001f) | (((u32)(src)) & 0x0000001f))

/*	Register AMPP_INT	*/ 
/*	 Fields RD_VIOL	 */
#define QML_RD_VIOL_WIDTH                                                     1
#define QML_RD_VIOL_SHIFT                                                     1
#define QML_RD_VIOL_MASK                                             0x00000002
#define QML_RD_VIOL_RD(src)                           (((src) & 0x00000002)>>1)
#define QML_RD_VIOL_WR(src)                      (((u32)(src)<<1) & 0x00000002)
#define QML_RD_VIOL_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields WR_VIOL	 */
#define QML_WR_VIOL_WIDTH                                                     1
#define QML_WR_VIOL_SHIFT                                                     0
#define QML_WR_VIOL_MASK                                             0x00000001
#define QML_WR_VIOL_RD(src)                              (((src) & 0x00000001))
#define QML_WR_VIOL_WR(src)                         (((u32)(src)) & 0x00000001)
#define QML_WR_VIOL_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register AMPP_INTMask	*/
/*    Mask Register Fields RD_VIOLMask    */
#define QML_RD_VIOLMASK_WIDTH                                                 1
#define QML_RD_VIOLMASK_SHIFT                                                 1
#define QML_RD_VIOLMASK_MASK                                         0x00000002
#define QML_RD_VIOLMASK_RD(src)                       (((src) & 0x00000002)>>1)
#define QML_RD_VIOLMASK_WR(src)                  (((u32)(src)<<1) & 0x00000002)
#define QML_RD_VIOLMASK_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*    Mask Register Fields WR_VIOLMask    */
#define QML_WR_VIOLMASK_WIDTH                                                 1
#define QML_WR_VIOLMASK_SHIFT                                                 0
#define QML_WR_VIOLMASK_MASK                                         0x00000001
#define QML_WR_VIOLMASK_RD(src)                          (((src) & 0x00000001))
#define QML_WR_VIOLMASK_WR(src)                     (((u32)(src)) & 0x00000001)
#define QML_WR_VIOLMASK_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register AMPP_DMT_SA_0	*/ 
/*	 Fields ENABLE	 */
#define QML_ENABLE0_WIDTH                                                     1
#define QML_ENABLE0_SHIFT                                                    31
#define QML_ENABLE0_MASK                                             0x80000000
#define QML_ENABLE0_RD(src)                          (((src) & 0x80000000)>>31)
#define QML_ENABLE0_WR(src)                     (((u32)(src)<<31) & 0x80000000)
#define QML_ENABLE0_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields START_ADDR_36_12	 */
#define QML_START_ADDR_36_120_WIDTH                                          24
#define QML_START_ADDR_36_120_SHIFT                                           0
#define QML_START_ADDR_36_120_MASK                                   0x00ffffff
#define QML_START_ADDR_36_120_RD(src)                    (((src) & 0x00ffffff))
#define QML_START_ADDR_36_120_WR(src)               (((u32)(src)) & 0x00ffffff)
#define QML_START_ADDR_36_120_SET(dst,src) \
                          (((dst) & ~0x00ffffff) | (((u32)(src)) & 0x00ffffff))

/*	Register AMPP_DMT_EA_0	*/ 
/*	 Fields END_ADDR_36_12	 */
#define QML_END_ADDR_36_120_WIDTH                                            24
#define QML_END_ADDR_36_120_SHIFT                                             0
#define QML_END_ADDR_36_120_MASK                                     0x00ffffff
#define QML_END_ADDR_36_120_RD(src)                      (((src) & 0x00ffffff))
#define QML_END_ADDR_36_120_WR(src)                 (((u32)(src)) & 0x00ffffff)
#define QML_END_ADDR_36_120_SET(dst,src) \
                          (((dst) & ~0x00ffffff) | (((u32)(src)) & 0x00ffffff))

/*	Register AMPP_DMT_RW_0	*/ 
/*	 Fields PPC1_ACCESS	 */
#define QML_PPC1_ACCESS0_WIDTH                                                2
#define QML_PPC1_ACCESS0_SHIFT                                                6
#define QML_PPC1_ACCESS0_MASK                                        0x000000c0
#define QML_PPC1_ACCESS0_RD(src)                      (((src) & 0x000000c0)>>6)
#define QML_PPC1_ACCESS0_WR(src)                 (((u32)(src)<<6) & 0x000000c0)
#define QML_PPC1_ACCESS0_SET(dst,src) \
                       (((dst) & ~0x000000c0) | (((u32)(src)<<6) & 0x000000c0))
/*	 Fields PPC0_ACCESS	 */
#define QML_PPC0_ACCESS0_WIDTH                                                2
#define QML_PPC0_ACCESS0_SHIFT                                                4
#define QML_PPC0_ACCESS0_MASK                                        0x00000030
#define QML_PPC0_ACCESS0_RD(src)                      (((src) & 0x00000030)>>4)
#define QML_PPC0_ACCESS0_WR(src)                 (((u32)(src)<<4) & 0x00000030)
#define QML_PPC0_ACCESS0_SET(dst,src) \
                       (((dst) & ~0x00000030) | (((u32)(src)<<4) & 0x00000030))
/*	 Fields EBUS_PCIE_ACCESS	 */
#define QML_EBUS_PCIE_ACCESS0_WIDTH                                           2
#define QML_EBUS_PCIE_ACCESS0_SHIFT                                           2
#define QML_EBUS_PCIE_ACCESS0_MASK                                   0x0000000c
#define QML_EBUS_PCIE_ACCESS0_RD(src)                 (((src) & 0x0000000c)>>2)
#define QML_EBUS_PCIE_ACCESS0_WR(src)            (((u32)(src)<<2) & 0x0000000c)
#define QML_EBUS_PCIE_ACCESS0_SET(dst,src) \
                       (((dst) & ~0x0000000c) | (((u32)(src)<<2) & 0x0000000c))
/*	 Fields IPP_ACCESS	 */
#define QML_IPP_ACCESS0_WIDTH                                                 2
#define QML_IPP_ACCESS0_SHIFT                                                 0
#define QML_IPP_ACCESS0_MASK                                         0x00000003
#define QML_IPP_ACCESS0_RD(src)                          (((src) & 0x00000003))
#define QML_IPP_ACCESS0_WR(src)                     (((u32)(src)) & 0x00000003)
#define QML_IPP_ACCESS0_SET(dst,src) \
                          (((dst) & ~0x00000003) | (((u32)(src)) & 0x00000003))

/*	Register AMPP_DMT_SA_1	*/ 
/*	 Fields ENABLE	 */
#define QML_ENABLE1_WIDTH                                                     1
#define QML_ENABLE1_SHIFT                                                    31
#define QML_ENABLE1_MASK                                             0x80000000
#define QML_ENABLE1_RD(src)                          (((src) & 0x80000000)>>31)
#define QML_ENABLE1_WR(src)                     (((u32)(src)<<31) & 0x80000000)
#define QML_ENABLE1_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields START_ADDR_36_12	 */
#define QML_START_ADDR_36_121_WIDTH                                          24
#define QML_START_ADDR_36_121_SHIFT                                           0
#define QML_START_ADDR_36_121_MASK                                   0x00ffffff
#define QML_START_ADDR_36_121_RD(src)                    (((src) & 0x00ffffff))
#define QML_START_ADDR_36_121_WR(src)               (((u32)(src)) & 0x00ffffff)
#define QML_START_ADDR_36_121_SET(dst,src) \
                          (((dst) & ~0x00ffffff) | (((u32)(src)) & 0x00ffffff))

/*	Register AMPP_DMT_EA_1	*/ 
/*	 Fields END_ADDR_36_12	 */
#define QML_END_ADDR_36_121_WIDTH                                            24
#define QML_END_ADDR_36_121_SHIFT                                             0
#define QML_END_ADDR_36_121_MASK                                     0x00ffffff
#define QML_END_ADDR_36_121_RD(src)                      (((src) & 0x00ffffff))
#define QML_END_ADDR_36_121_WR(src)                 (((u32)(src)) & 0x00ffffff)
#define QML_END_ADDR_36_121_SET(dst,src) \
                          (((dst) & ~0x00ffffff) | (((u32)(src)) & 0x00ffffff))

/*	Register AMPP_DMT_RW_1	*/ 
/*	 Fields PPC1_ACCESS	 */
#define QML_PPC1_ACCESS1_WIDTH                                                2
#define QML_PPC1_ACCESS1_SHIFT                                                6
#define QML_PPC1_ACCESS1_MASK                                        0x000000c0
#define QML_PPC1_ACCESS1_RD(src)                      (((src) & 0x000000c0)>>6)
#define QML_PPC1_ACCESS1_WR(src)                 (((u32)(src)<<6) & 0x000000c0)
#define QML_PPC1_ACCESS1_SET(dst,src) \
                       (((dst) & ~0x000000c0) | (((u32)(src)<<6) & 0x000000c0))
/*	 Fields PPC0_ACCESS	 */
#define QML_PPC0_ACCESS1_WIDTH                                                2
#define QML_PPC0_ACCESS1_SHIFT                                                4
#define QML_PPC0_ACCESS1_MASK                                        0x00000030
#define QML_PPC0_ACCESS1_RD(src)                      (((src) & 0x00000030)>>4)
#define QML_PPC0_ACCESS1_WR(src)                 (((u32)(src)<<4) & 0x00000030)
#define QML_PPC0_ACCESS1_SET(dst,src) \
                       (((dst) & ~0x00000030) | (((u32)(src)<<4) & 0x00000030))
/*	 Fields EBUS_PCIE_ACCESS	 */
#define QML_EBUS_PCIE_ACCESS1_WIDTH                                           2
#define QML_EBUS_PCIE_ACCESS1_SHIFT                                           2
#define QML_EBUS_PCIE_ACCESS1_MASK                                   0x0000000c
#define QML_EBUS_PCIE_ACCESS1_RD(src)                 (((src) & 0x0000000c)>>2)
#define QML_EBUS_PCIE_ACCESS1_WR(src)            (((u32)(src)<<2) & 0x0000000c)
#define QML_EBUS_PCIE_ACCESS1_SET(dst,src) \
                       (((dst) & ~0x0000000c) | (((u32)(src)<<2) & 0x0000000c))
/*	 Fields IPP_ACCESS	 */
#define QML_IPP_ACCESS1_WIDTH                                                 2
#define QML_IPP_ACCESS1_SHIFT                                                 0
#define QML_IPP_ACCESS1_MASK                                         0x00000003
#define QML_IPP_ACCESS1_RD(src)                          (((src) & 0x00000003))
#define QML_IPP_ACCESS1_WR(src)                     (((u32)(src)) & 0x00000003)
#define QML_IPP_ACCESS1_SET(dst,src) \
                          (((dst) & ~0x00000003) | (((u32)(src)) & 0x00000003))

#endif

#endif /* _APM_QML_CSR_H__ */
