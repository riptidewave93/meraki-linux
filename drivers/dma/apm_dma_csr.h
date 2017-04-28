/**
 *
 * Copyright (C) 2011 AppliedMicro Confidential Information
 * All Rights Reserved.
 *
 * THIS WORK CONTAINS PROPRIETARY INFORMATION WHICH IS THE PROPERTY OF
 * AppliedMicro AND IS SUBJECT TO THE TERMS OF NON-DISCLOSURE AGREEMENT
 * BETWEEN AppliedMicro AND THE COMPANY USING THIS FILE.
 *
 * WARNING !!!
 * This is an auto-generated C header file for register definitions 
 * PLEASE DON'T MANUALLY MODIFY IN THIS FILE AS CHANGES WILL BE LOST
 */
#ifndef _APM_DMA_CSR_H__
#define _APM_DMA_CSR_H__

/*	Global Base Address	*/
#define DMA_CSR_BASE_ADDR			0xddd820000ULL

/*    Address DMA_CSR  Registers */
#define DMA_IPBRR_ADDR                                               0x00000000
#define DMA_IPBRR_DEFAULT                                            0x0000006d
#define DMA_GCR_ADDR                                                 0x00000010
#define DMA_GCR_DEFAULT                                              0x79fe0000
#define DMA_RAID6_CONT_ADDR                                          0x00000014
#define DMA_RAID6_CONT_DEFAULT                                       0x00000000
#define DMA_RFMC_ADDR                                                0x00000020
#define DMA_RFMC_DEFAULT                                             0x00001b80
#define DMA_RFMD0_ADDR                                               0x00000030
#define DMA_RFMD0_DEFAULT                                            0x00000000
#define DMA_RFMD1_ADDR                                               0x00000034
#define DMA_RFMD1_DEFAULT                                            0x00000000
#define DMA_RFMD2_ADDR                                               0x00000038
#define DMA_RFMD2_DEFAULT                                            0x00000000
#define DMA_RFMD3_ADDR                                               0x0000003c
#define DMA_RFMD3_DEFAULT                                            0x00000000
#define DMA_WFMC_ADDR                                                0x00000040
#define DMA_WFMC_DEFAULT                                             0x00000500
#define DMA_WFMD0_ADDR                                               0x00000050
#define DMA_WFMD0_DEFAULT                                            0x00000000
#define DMA_WFMD1_ADDR                                               0x00000054
#define DMA_WFMD1_DEFAULT                                            0x00000000
#define DMA_WFMD2_ADDR                                               0x00000058
#define DMA_WFMD2_DEFAULT                                            0x00000000
#define DMA_WFMD3_ADDR                                               0x0000005c
#define DMA_WFMD3_DEFAULT                                            0x00000000
#define DMA_WFMD4_ADDR                                               0x00000060
#define DMA_WFMD4_DEFAULT                                            0x00000000
#define DMA_WFMD5_ADDR                                               0x00000064
#define DMA_WFMD5_DEFAULT                                            0x00000000
#define DMA_INT_ADDR                                                 0x00000070
#define DMA_INT_DEFAULT                                              0x00000000
#define DMA_INTMASK_ADDR                                             0x00000074
#define DMA_TIMEOC_ADDR                                              0x00000080
#define DMA_TIMEOC_DEFAULT                                           0xf0000000
#define DMA_FPB_TIMEOC_ADDR                                          0x00000084
#define DMA_FPB_TIMEOC_DEFAULT                                       0xf0000000
#define DMA_FIFO_LEV0_ADDR                                           0x00000088
#define DMA_FIFO_LEV0_DEFAULT                                        0x00000000
#define DMA_FIFO_LEV1_ADDR                                           0x0000008c
#define DMA_FIFO_LEV1_DEFAULT                                        0x00000000
#define DMA_FALSE_INT_ADDR                                           0x00000090
#define DMA_FALSE_INT_DEFAULT                                        0x00000000
#define DMA_RST_CH_ADDR                                              0x00000100
#define DMA_RST_CH_DEFAULT                                           0x00000000

/*	Register dma_ipbrr	*/ 
/*	 Fields rev_no	 */
#define REV_NO_WIDTH                                                          2
#define REV_NO_SHIFT                                                          8
#define REV_NO_MASK                                                  0x00000300
#define REV_NO_RD(src)                                (((src) & 0x00000300)>>8)
#define REV_NO_SET(dst,src) \
                       (((dst) & ~0x00000300) | (((u32)(src)<<8) & 0x00000300))
/*	 Fields bus_id	 */
#define BUS_ID_WIDTH                                                          3
#define BUS_ID_SHIFT                                                          5
#define BUS_ID_MASK                                                  0x000000e0
#define BUS_ID_RD(src)                                (((src) & 0x000000e0)>>5)
#define BUS_ID_SET(dst,src) \
                       (((dst) & ~0x000000e0) | (((u32)(src)<<5) & 0x000000e0))
/*	 Fields device_id	 */
#define DEVICE_ID_WIDTH                                                       5
#define DEVICE_ID_SHIFT                                                       0
#define DEVICE_ID_MASK                                               0x0000001f
#define DEVICE_ID_RD(src)                                (((src) & 0x0000001f))
#define DEVICE_ID_SET(dst,src) \
                          (((dst) & ~0x0000001f) | (((u32)(src)) & 0x0000001f))

/*	Register dma_gcr	*/ 
/*	 Fields dma_en	 */
#define DMA_EN_WIDTH                                                          1
#define DMA_EN_SHIFT                                                         31
#define DMA_EN_MASK                                                  0x80000000
#define DMA_EN_RD(src)                               (((src) & 0x80000000)>>31)
#define DMA_EN_WR(src)                          (((u32)(src)<<31) & 0x80000000)
#define DMA_EN_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields dma_pri	 */
#define DMA_PRI_WIDTH                                                         4
#define DMA_PRI_SHIFT                                                        27
#define DMA_PRI_MASK                                                 0x78000000
#define DMA_PRI_RD(src)                              (((src) & 0x78000000)>>27)
#define DMA_PRI_WR(src)                         (((u32)(src)<<27) & 0x78000000)
#define DMA_PRI_SET(dst,src) \
                      (((dst) & ~0x78000000) | (((u32)(src)<<27) & 0x78000000))
/*	 Fields qmi_pri	 */
#define QMI_PRI_WIDTH                                                         4
#define QMI_PRI_SHIFT                                                        23
#define QMI_PRI_MASK                                                 0x07800000
#define QMI_PRI_RD(src)                              (((src) & 0x07800000)>>23)
#define QMI_PRI_WR(src)                         (((u32)(src)<<23) & 0x07800000)
#define QMI_PRI_SET(dst,src) \
                      (((dst) & ~0x07800000) | (((u32)(src)<<23) & 0x07800000))
/*	 Fields pcie1_bp_en	 */
#define PCIE1_BP_EN_WIDTH                                                     1
#define PCIE1_BP_EN_SHIFT                                                    22
#define PCIE1_BP_EN_MASK                                             0x00400000
#define PCIE1_BP_EN_RD(src)                          (((src) & 0x00400000)>>22)
#define PCIE1_BP_EN_WR(src)                     (((u32)(src)<<22) & 0x00400000)
#define PCIE1_BP_EN_SET(dst,src) \
                      (((dst) & ~0x00400000) | (((u32)(src)<<22) & 0x00400000))
/*	 Fields pcie2_bp_en	 */
#define PCIE2_BP_EN_WIDTH                                                     1
#define PCIE2_BP_EN_SHIFT                                                    21
#define PCIE2_BP_EN_MASK                                             0x00200000
#define PCIE2_BP_EN_RD(src)                          (((src) & 0x00200000)>>21)
#define PCIE2_BP_EN_WR(src)                     (((u32)(src)<<21) & 0x00200000)
#define PCIE2_BP_EN_SET(dst,src) \
                      (((dst) & ~0x00200000) | (((u32)(src)<<21) & 0x00200000))
/*	 Fields pcie3_bp_en	 */
#define PCIE3_BP_EN_WIDTH                                                     1
#define PCIE3_BP_EN_SHIFT                                                    20
#define PCIE3_BP_EN_MASK                                             0x00100000
#define PCIE3_BP_EN_RD(src)                          (((src) & 0x00100000)>>20)
#define PCIE3_BP_EN_WR(src)                     (((u32)(src)<<20) & 0x00100000)
#define PCIE3_BP_EN_SET(dst,src) \
                      (((dst) & ~0x00100000) | (((u32)(src)<<20) & 0x00100000))
/*	 Fields ahbc_bp_en	 */
#define AHBC_BP_EN_WIDTH                                                      1
#define AHBC_BP_EN_SHIFT                                                     19
#define AHBC_BP_EN_MASK                                              0x00080000
#define AHBC_BP_EN_RD(src)                           (((src) & 0x00080000)>>19)
#define AHBC_BP_EN_WR(src)                      (((u32)(src)<<19) & 0x00080000)
#define AHBC_BP_EN_SET(dst,src) \
                      (((dst) & ~0x00080000) | (((u32)(src)<<19) & 0x00080000))
/*	 Fields cont_bvalid	 */
#define CONT_BVALID_WIDTH                                                     1
#define CONT_BVALID_SHIFT                                                    18
#define CONT_BVALID_MASK                                             0x00040000
#define CONT_BVALID_RD(src)                          (((src) & 0x00040000)>>18)
#define CONT_BVALID_WR(src)                     (((u32)(src)<<18) & 0x00040000)
#define CONT_BVALID_SET(dst,src) \
                      (((dst) & ~0x00040000) | (((u32)(src)<<18) & 0x00040000))
/*	 Fields en_gs_err	 */
#define EN_GS_ERR_WIDTH                                                       1
#define EN_GS_ERR_SHIFT                                                      17
#define EN_GS_ERR_MASK                                               0x00020000
#define EN_GS_ERR_RD(src)                            (((src) & 0x00020000)>>17)
#define EN_GS_ERR_WR(src)                       (((u32)(src)<<17) & 0x00020000)
#define EN_GS_ERR_SET(dst,src) \
                      (((dst) & ~0x00020000) | (((u32)(src)<<17) & 0x00020000))
/*	 Fields en_multi_req_3	 */
#define EN_MULTI_REQ_3_WIDTH                                                  2
#define EN_MULTI_REQ_3_SHIFT                                                 10
#define EN_MULTI_REQ_3_MASK                                          0x00000c00
#define EN_MULTI_REQ_3_RD(src)                       (((src) & 0x00000c00)>>10)
#define EN_MULTI_REQ_3_WR(src)                  (((u32)(src)<<10) & 0x00000c00)
#define EN_MULTI_REQ_3_SET(dst,src) \
                      (((dst) & ~0x00000c00) | (((u32)(src)<<10) & 0x00000c00))
/*	 Fields en_multi_req_2	 */
#define EN_MULTI_REQ_2_WIDTH                                                  2
#define EN_MULTI_REQ_2_SHIFT                                                  8
#define EN_MULTI_REQ_2_MASK                                          0x00000300
#define EN_MULTI_REQ_2_RD(src)                        (((src) & 0x00000300)>>8)
#define EN_MULTI_REQ_2_WR(src)                   (((u32)(src)<<8) & 0x00000300)
#define EN_MULTI_REQ_2_SET(dst,src) \
                       (((dst) & ~0x00000300) | (((u32)(src)<<8) & 0x00000300))
/*	 Fields en_multi_req_1	 */
#define EN_MULTI_REQ_1_WIDTH                                                  2
#define EN_MULTI_REQ_1_SHIFT                                                  6
#define EN_MULTI_REQ_1_MASK                                          0x000000c0
#define EN_MULTI_REQ_1_RD(src)                        (((src) & 0x000000c0)>>6)
#define EN_MULTI_REQ_1_WR(src)                   (((u32)(src)<<6) & 0x000000c0)
#define EN_MULTI_REQ_1_SET(dst,src) \
                       (((dst) & ~0x000000c0) | (((u32)(src)<<6) & 0x000000c0))
/*	 Fields en_multi_req_0	 */
#define EN_MULTI_REQ_0_WIDTH                                                  2
#define EN_MULTI_REQ_0_SHIFT                                                  4
#define EN_MULTI_REQ_0_MASK                                          0x00000030
#define EN_MULTI_REQ_0_RD(src)                        (((src) & 0x00000030)>>4)
#define EN_MULTI_REQ_0_WR(src)                   (((u32)(src)<<4) & 0x00000030)
#define EN_MULTI_REQ_0_SET(dst,src) \
                       (((dst) & ~0x00000030) | (((u32)(src)<<4) & 0x00000030))
/*	 Fields ch_3_en	 */
#define CH_3_EN_WIDTH                                                         1
#define CH_3_EN_SHIFT                                                         3
#define CH_3_EN_MASK                                                 0x00000008
#define CH_3_EN_RD(src)                               (((src) & 0x00000008)>>3)
#define CH_3_EN_WR(src)                          (((u32)(src)<<3) & 0x00000008)
#define CH_3_EN_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields ch_2_en	 */
#define CH_2_EN_WIDTH                                                         1
#define CH_2_EN_SHIFT                                                         2
#define CH_2_EN_MASK                                                 0x00000004
#define CH_2_EN_RD(src)                               (((src) & 0x00000004)>>2)
#define CH_2_EN_WR(src)                          (((u32)(src)<<2) & 0x00000004)
#define CH_2_EN_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields ch_1_en	 */
#define CH_1_EN_WIDTH                                                         1
#define CH_1_EN_SHIFT                                                         1
#define CH_1_EN_MASK                                                 0x00000002
#define CH_1_EN_RD(src)                               (((src) & 0x00000002)>>1)
#define CH_1_EN_WR(src)                          (((u32)(src)<<1) & 0x00000002)
#define CH_1_EN_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields ch_0_en	 */
#define CH_0_EN_WIDTH                                                         1
#define CH_0_EN_SHIFT                                                         0
#define CH_0_EN_MASK                                                 0x00000001
#define CH_0_EN_RD(src)                                  (((src) & 0x00000001))
#define CH_0_EN_WR(src)                             (((u32)(src)) & 0x00000001)
#define CH_0_EN_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register dma_raid6_cont	*/ 
/*	 Fields mult_cont	 */
#define MULT_CONT_WIDTH                                                       8
#define MULT_CONT_SHIFT                                                      24
#define MULT_CONT_MASK                                               0xff000000
#define MULT_CONT_RD(src)                            (((src) & 0xff000000)>>24)
#define MULT_CONT_WR(src)                       (((u32)(src)<<24) & 0xff000000)
#define MULT_CONT_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))

/*	Register dma_rfmc	*/ 
/*	 Fields rd_dmem	 */
#define RD_DMEM_WIDTH                                                         1
#define RD_DMEM_SHIFT                                                        31
#define RD_DMEM_MASK                                                 0x80000000
#define RD_DMEM_RD(src)                              (((src) & 0x80000000)>>31)
#define RD_DMEM_WR(src)                         (((u32)(src)<<31) & 0x80000000)
#define RD_DMEM_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields rd_xmem	 */
#define RD_XMEM_WIDTH                                                         1
#define RD_XMEM_SHIFT                                                        30
#define RD_XMEM_MASK                                                 0x40000000
#define RD_XMEM_RD(src)                              (((src) & 0x40000000)>>30)
#define RD_XMEM_WR(src)                         (((u32)(src)<<30) & 0x40000000)
#define RD_XMEM_SET(dst,src) \
                      (((dst) & ~0x40000000) | (((u32)(src)<<30) & 0x40000000))
/*	 Fields mem_th	 */
#define MEM_TH_WIDTH                                                          7
#define MEM_TH_SHIFT                                                          7
#define MEM_TH_MASK                                                  0x00003f80
#define MEM_TH_RD(src)                                (((src) & 0x00003f80)>>7)
#define MEM_TH_WR(src)                           (((u32)(src)<<7) & 0x00003f80)
#define MEM_TH_SET(dst,src) \
                       (((dst) & ~0x00003f80) | (((u32)(src)<<7) & 0x00003f80))
/*	 Fields mem_add	 */
#define MEM_ADD_WIDTH                                                         7
#define MEM_ADD_SHIFT                                                         0
#define MEM_ADD_MASK                                                 0x0000007f
#define MEM_ADD_RD(src)                                  (((src) & 0x0000007f))
#define MEM_ADD_WR(src)                             (((u32)(src)) & 0x0000007f)
#define MEM_ADD_SET(dst,src) \
                          (((dst) & ~0x0000007f) | (((u32)(src)) & 0x0000007f))

/*	Register dma_rfmd0	*/ 
/*	 Fields mem_data	 */
#define MEM_DATA0_WIDTH                                                      32
#define MEM_DATA0_SHIFT                                                       0
#define MEM_DATA0_MASK                                               0xffffffff
#define MEM_DATA0_RD(src)                                (((src) & 0xffffffff))
#define MEM_DATA0_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register dma_rfmd1	*/ 
/*	 Fields mem_data	 */
#define MEM_DATA1_WIDTH                                                      32
#define MEM_DATA1_SHIFT                                                       0
#define MEM_DATA1_MASK                                               0xffffffff
#define MEM_DATA1_RD(src)                                (((src) & 0xffffffff))
#define MEM_DATA1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register dma_rfmd2	*/ 
/*	 Fields mem_data	 */
#define MEM_DATA2_WIDTH                                                      32
#define MEM_DATA2_SHIFT                                                       0
#define MEM_DATA2_MASK                                               0xffffffff
#define MEM_DATA2_RD(src)                                (((src) & 0xffffffff))
#define MEM_DATA2_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register dma_rfmd3	*/ 
/*	 Fields mem_data	 */
#define MEM_DATA3_WIDTH                                                      32
#define MEM_DATA3_SHIFT                                                       0
#define MEM_DATA3_MASK                                               0xffffffff
#define MEM_DATA3_RD(src)                                (((src) & 0xffffffff))
#define MEM_DATA3_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register dma_wfmc	*/ 
/*	 Fields rd_mem	 */
#define RD_MEM_WIDTH                                                          1
#define RD_MEM_SHIFT                                                         31
#define RD_MEM_MASK                                                  0x80000000
#define RD_MEM_RD(src)                               (((src) & 0x80000000)>>31)
#define RD_MEM_WR(src)                          (((u32)(src)<<31) & 0x80000000)
#define RD_MEM_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields rst_fifo	 */
#define RST_FIFO_WIDTH                                                        1
#define RST_FIFO_SHIFT                                                       30
#define RST_FIFO_MASK                                                0x40000000
#define RST_FIFO_RD(src)                             (((src) & 0x40000000)>>30)
#define RST_FIFO_WR(src)                        (((u32)(src)<<30) & 0x40000000)
#define RST_FIFO_SET(dst,src) \
                      (((dst) & ~0x40000000) | (((u32)(src)<<30) & 0x40000000))
/*	 Fields mem_th	 */
#define MEM_TH_F1_WIDTH                                                       7
#define MEM_TH_F1_SHIFT                                                       6
#define MEM_TH_F1_MASK                                               0x00001fc0
#define MEM_TH_F1_RD(src)                             (((src) & 0x00001fc0)>>6)
#define MEM_TH_F1_WR(src)                        (((u32)(src)<<6) & 0x00001fc0)
#define MEM_TH_F1_SET(dst,src) \
                       (((dst) & ~0x00001fc0) | (((u32)(src)<<6) & 0x00001fc0))
/*	 Fields mem_add	 */
#define MEM_ADD_F1_WIDTH                                                      6
#define MEM_ADD_F1_SHIFT                                                      0
#define MEM_ADD_F1_MASK                                              0x0000003f
#define MEM_ADD_F1_RD(src)                               (((src) & 0x0000003f))
#define MEM_ADD_F1_WR(src)                          (((u32)(src)) & 0x0000003f)
#define MEM_ADD_F1_SET(dst,src) \
                          (((dst) & ~0x0000003f) | (((u32)(src)) & 0x0000003f))

/*	Register dma_wfmd0	*/ 
/*	 Fields mem_data	 */
#define MEM_DATA0_F1_WIDTH                                                   32
#define MEM_DATA0_F1_SHIFT                                                    0
#define MEM_DATA0_F1_MASK                                            0xffffffff
#define MEM_DATA0_F1_RD(src)                             (((src) & 0xffffffff))
#define MEM_DATA0_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register dma_wfmd1	*/ 
/*	 Fields mem_data	 */
#define MEM_DATA1_F1_WIDTH                                                   32
#define MEM_DATA1_F1_SHIFT                                                    0
#define MEM_DATA1_F1_MASK                                            0xffffffff
#define MEM_DATA1_F1_RD(src)                             (((src) & 0xffffffff))
#define MEM_DATA1_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register dma_wfmd2	*/ 
/*	 Fields mem_data	 */
#define MEM_DATA2_F1_WIDTH                                                   32
#define MEM_DATA2_F1_SHIFT                                                    0
#define MEM_DATA2_F1_MASK                                            0xffffffff
#define MEM_DATA2_F1_RD(src)                             (((src) & 0xffffffff))
#define MEM_DATA2_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register dma_wfmd3	*/ 
/*	 Fields mem_data	 */
#define MEM_DATA3_F1_WIDTH                                                   32
#define MEM_DATA3_F1_SHIFT                                                    0
#define MEM_DATA3_F1_MASK                                            0xffffffff
#define MEM_DATA3_F1_RD(src)                             (((src) & 0xffffffff))
#define MEM_DATA3_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register dma_wfmd4	*/ 
/*	 Fields mem_data	 */
#define MEM_DATA4_WIDTH                                                      32
#define MEM_DATA4_SHIFT                                                       0
#define MEM_DATA4_MASK                                               0xffffffff
#define MEM_DATA4_RD(src)                                (((src) & 0xffffffff))
#define MEM_DATA4_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register dma_wfmd5	*/ 
/*	 Fields mem_data	 */
#define MEM_DATA5_WIDTH                                                      32
#define MEM_DATA5_SHIFT                                                       0
#define MEM_DATA5_MASK                                               0xffffffff
#define MEM_DATA5_RD(src)                                (((src) & 0xffffffff))
#define MEM_DATA5_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register dma_int	*/ 
/*	 Fields msg_src_int	 */
#define MSG_SRC_INT_WIDTH                                                     1
#define MSG_SRC_INT_SHIFT                                                    31
#define MSG_SRC_INT_MASK                                             0x80000000
#define MSG_SRC_INT_RD(src)                          (((src) & 0x80000000)>>31)
#define MSG_SRC_INT_WR(src)                     (((u32)(src)<<31) & 0x80000000)
#define MSG_SRC_INT_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields msg_dst_int	 */
#define MSG_DST_INT_WIDTH                                                     1
#define MSG_DST_INT_SHIFT                                                    30
#define MSG_DST_INT_MASK                                             0x40000000
#define MSG_DST_INT_RD(src)                          (((src) & 0x40000000)>>30)
#define MSG_DST_INT_WR(src)                     (((u32)(src)<<30) & 0x40000000)
#define MSG_DST_INT_SET(dst,src) \
                      (((dst) & ~0x40000000) | (((u32)(src)<<30) & 0x40000000))
/*	 Fields bad_msg	 */
#define BAD_MSG_WIDTH                                                         1
#define BAD_MSG_SHIFT                                                        29
#define BAD_MSG_MASK                                                 0x20000000
#define BAD_MSG_RD(src)                              (((src) & 0x20000000)>>29)
#define BAD_MSG_WR(src)                         (((u32)(src)<<29) & 0x20000000)
#define BAD_MSG_SET(dst,src) \
                      (((dst) & ~0x20000000) | (((u32)(src)<<29) & 0x20000000))
/*	 Fields rd_err_int	 */
#define RD_ERR_INT_WIDTH                                                      1
#define RD_ERR_INT_SHIFT                                                     28
#define RD_ERR_INT_MASK                                              0x10000000
#define RD_ERR_INT_RD(src)                           (((src) & 0x10000000)>>28)
#define RD_ERR_INT_WR(src)                      (((u32)(src)<<28) & 0x10000000)
#define RD_ERR_INT_SET(dst,src) \
                      (((dst) & ~0x10000000) | (((u32)(src)<<28) & 0x10000000))
/*	 Fields wr_err_int	 */
#define WR_ERR_INT_WIDTH                                                      1
#define WR_ERR_INT_SHIFT                                                     27
#define WR_ERR_INT_MASK                                              0x08000000
#define WR_ERR_INT_RD(src)                           (((src) & 0x08000000)>>27)
#define WR_ERR_INT_WR(src)                      (((u32)(src)<<27) & 0x08000000)
#define WR_ERR_INT_SET(dst,src) \
                      (((dst) & ~0x08000000) | (((u32)(src)<<27) & 0x08000000))
/*	 Fields rd_timeo_int	 */
#define RD_TIMEO_INT_WIDTH                                                    1
#define RD_TIMEO_INT_SHIFT                                                   26
#define RD_TIMEO_INT_MASK                                            0x04000000
#define RD_TIMEO_INT_RD(src)                         (((src) & 0x04000000)>>26)
#define RD_TIMEO_INT_WR(src)                    (((u32)(src)<<26) & 0x04000000)
#define RD_TIMEO_INT_SET(dst,src) \
                      (((dst) & ~0x04000000) | (((u32)(src)<<26) & 0x04000000))
/*	 Fields wr_timeo_int	 */
#define WR_TIMEO_INT_WIDTH                                                    1
#define WR_TIMEO_INT_SHIFT                                                   25
#define WR_TIMEO_INT_MASK                                            0x02000000
#define WR_TIMEO_INT_RD(src)                         (((src) & 0x02000000)>>25)
#define WR_TIMEO_INT_WR(src)                    (((u32)(src)<<25) & 0x02000000)
#define WR_TIMEO_INT_SET(dst,src) \
                      (((dst) & ~0x02000000) | (((u32)(src)<<25) & 0x02000000))
/*	 Fields rfifo_ovf_int	 */
#define RFIFO_OVF_INT_WIDTH                                                   1
#define RFIFO_OVF_INT_SHIFT                                                  24
#define RFIFO_OVF_INT_MASK                                           0x01000000
#define RFIFO_OVF_INT_RD(src)                        (((src) & 0x01000000)>>24)
#define RFIFO_OVF_INT_WR(src)                   (((u32)(src)<<24) & 0x01000000)
#define RFIFO_OVF_INT_SET(dst,src) \
                      (((dst) & ~0x01000000) | (((u32)(src)<<24) & 0x01000000))
/*	 Fields wfifo_ovf_int	 */
#define WFIFO_OVF_INT_WIDTH                                                   1
#define WFIFO_OVF_INT_SHIFT                                                  23
#define WFIFO_OVF_INT_MASK                                           0x00800000
#define WFIFO_OVF_INT_RD(src)                        (((src) & 0x00800000)>>23)
#define WFIFO_OVF_INT_WR(src)                   (((u32)(src)<<23) & 0x00800000)
#define WFIFO_OVF_INT_SET(dst,src) \
                      (((dst) & ~0x00800000) | (((u32)(src)<<23) & 0x00800000))
/*	 Fields fpb_timeo_int	 */
#define FPB_TIMEO_INT_WIDTH                                                   1
#define FPB_TIMEO_INT_SHIFT                                                  22
#define FPB_TIMEO_INT_MASK                                           0x00400000
#define FPB_TIMEO_INT_RD(src)                        (((src) & 0x00400000)>>22)
#define FPB_TIMEO_INT_WR(src)                   (((u32)(src)<<22) & 0x00400000)
#define FPB_TIMEO_INT_SET(dst,src) \
                      (((dst) & ~0x00400000) | (((u32)(src)<<22) & 0x00400000))
/*	 Fields gs_err_int	 */
#define GS_ERR_INT_WIDTH                                                      1
#define GS_ERR_INT_SHIFT                                                     21
#define GS_ERR_INT_MASK                                              0x00200000
#define GS_ERR_INT_RD(src)                           (((src) & 0x00200000)>>21)
#define GS_ERR_INT_WR(src)                      (((u32)(src)<<21) & 0x00200000)
#define GS_ERR_INT_SET(dst,src) \
                      (((dst) & ~0x00200000) | (((u32)(src)<<21) & 0x00200000))
/*	 Fields bad_dom_id_int	 */
#define BAD_DOM_ID_INT_WIDTH                                                  1
#define BAD_DOM_ID_INT_SHIFT                                                 20
#define BAD_DOM_ID_INT_MASK                                          0x00100000
#define BAD_DOM_ID_INT_RD(src)                       (((src) & 0x00100000)>>20)
#define BAD_DOM_ID_INT_WR(src)                  (((u32)(src)<<20) & 0x00100000)
#define BAD_DOM_ID_INT_SET(dst,src) \
                      (((dst) & ~0x00100000) | (((u32)(src)<<20) & 0x00100000))

/*	Register dma_intMask	*/
/*    Mask Register Fields msg_src_intMask    */
#define MSG_SRC_INTMASK_WIDTH                                                 1
#define MSG_SRC_INTMASK_SHIFT                                                31
#define MSG_SRC_INTMASK_MASK                                         0x80000000
#define MSG_SRC_INTMASK_RD(src)                      (((src) & 0x80000000)>>31)
#define MSG_SRC_INTMASK_WR(src)                 (((u32)(src)<<31) & 0x80000000)
#define MSG_SRC_INTMASK_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*    Mask Register Fields msg_dst_intMask    */
#define MSG_DST_INTMASK_WIDTH                                                 1
#define MSG_DST_INTMASK_SHIFT                                                30
#define MSG_DST_INTMASK_MASK                                         0x40000000
#define MSG_DST_INTMASK_RD(src)                      (((src) & 0x40000000)>>30)
#define MSG_DST_INTMASK_WR(src)                 (((u32)(src)<<30) & 0x40000000)
#define MSG_DST_INTMASK_SET(dst,src) \
                      (((dst) & ~0x40000000) | (((u32)(src)<<30) & 0x40000000))
/*    Mask Register Fields bad_msgMask    */
#define BAD_MSGMASK_WIDTH                                                     1
#define BAD_MSGMASK_SHIFT                                                    29
#define BAD_MSGMASK_MASK                                             0x20000000
#define BAD_MSGMASK_RD(src)                          (((src) & 0x20000000)>>29)
#define BAD_MSGMASK_WR(src)                     (((u32)(src)<<29) & 0x20000000)
#define BAD_MSGMASK_SET(dst,src) \
                      (((dst) & ~0x20000000) | (((u32)(src)<<29) & 0x20000000))
/*    Mask Register Fields rd_err_intMask    */
#define RD_ERR_INTMASK_WIDTH                                                  1
#define RD_ERR_INTMASK_SHIFT                                                 28
#define RD_ERR_INTMASK_MASK                                          0x10000000
#define RD_ERR_INTMASK_RD(src)                       (((src) & 0x10000000)>>28)
#define RD_ERR_INTMASK_WR(src)                  (((u32)(src)<<28) & 0x10000000)
#define RD_ERR_INTMASK_SET(dst,src) \
                      (((dst) & ~0x10000000) | (((u32)(src)<<28) & 0x10000000))
/*    Mask Register Fields wr_err_intMask    */
#define WR_ERR_INTMASK_WIDTH                                                  1
#define WR_ERR_INTMASK_SHIFT                                                 27
#define WR_ERR_INTMASK_MASK                                          0x08000000
#define WR_ERR_INTMASK_RD(src)                       (((src) & 0x08000000)>>27)
#define WR_ERR_INTMASK_WR(src)                  (((u32)(src)<<27) & 0x08000000)
#define WR_ERR_INTMASK_SET(dst,src) \
                      (((dst) & ~0x08000000) | (((u32)(src)<<27) & 0x08000000))
/*    Mask Register Fields rd_timeo_intMask    */
#define RD_TIMEO_INTMASK_WIDTH                                                1
#define RD_TIMEO_INTMASK_SHIFT                                               26
#define RD_TIMEO_INTMASK_MASK                                        0x04000000
#define RD_TIMEO_INTMASK_RD(src)                     (((src) & 0x04000000)>>26)
#define RD_TIMEO_INTMASK_WR(src)                (((u32)(src)<<26) & 0x04000000)
#define RD_TIMEO_INTMASK_SET(dst,src) \
                      (((dst) & ~0x04000000) | (((u32)(src)<<26) & 0x04000000))
/*    Mask Register Fields wr_timeo_intMask    */
#define WR_TIMEO_INTMASK_WIDTH                                                1
#define WR_TIMEO_INTMASK_SHIFT                                               25
#define WR_TIMEO_INTMASK_MASK                                        0x02000000
#define WR_TIMEO_INTMASK_RD(src)                     (((src) & 0x02000000)>>25)
#define WR_TIMEO_INTMASK_WR(src)                (((u32)(src)<<25) & 0x02000000)
#define WR_TIMEO_INTMASK_SET(dst,src) \
                      (((dst) & ~0x02000000) | (((u32)(src)<<25) & 0x02000000))
/*    Mask Register Fields rfifo_ovf_intMask    */
#define RFIFO_OVF_INTMASK_WIDTH                                               1
#define RFIFO_OVF_INTMASK_SHIFT                                              24
#define RFIFO_OVF_INTMASK_MASK                                       0x01000000
#define RFIFO_OVF_INTMASK_RD(src)                    (((src) & 0x01000000)>>24)
#define RFIFO_OVF_INTMASK_WR(src)               (((u32)(src)<<24) & 0x01000000)
#define RFIFO_OVF_INTMASK_SET(dst,src) \
                      (((dst) & ~0x01000000) | (((u32)(src)<<24) & 0x01000000))
/*    Mask Register Fields wfifo_ovf_intMask    */
#define WFIFO_OVF_INTMASK_WIDTH                                               1
#define WFIFO_OVF_INTMASK_SHIFT                                              23
#define WFIFO_OVF_INTMASK_MASK                                       0x00800000
#define WFIFO_OVF_INTMASK_RD(src)                    (((src) & 0x00800000)>>23)
#define WFIFO_OVF_INTMASK_WR(src)               (((u32)(src)<<23) & 0x00800000)
#define WFIFO_OVF_INTMASK_SET(dst,src) \
                      (((dst) & ~0x00800000) | (((u32)(src)<<23) & 0x00800000))
/*    Mask Register Fields fpb_timeo_intMask    */
#define FPB_TIMEO_INTMASK_WIDTH                                               1
#define FPB_TIMEO_INTMASK_SHIFT                                              22
#define FPB_TIMEO_INTMASK_MASK                                       0x00400000
#define FPB_TIMEO_INTMASK_RD(src)                    (((src) & 0x00400000)>>22)
#define FPB_TIMEO_INTMASK_WR(src)               (((u32)(src)<<22) & 0x00400000)
#define FPB_TIMEO_INTMASK_SET(dst,src) \
                      (((dst) & ~0x00400000) | (((u32)(src)<<22) & 0x00400000))
/*    Mask Register Fields gs_err_intMask    */
#define GS_ERR_INTMASK_WIDTH                                                  1
#define GS_ERR_INTMASK_SHIFT                                                 21
#define GS_ERR_INTMASK_MASK                                          0x00200000
#define GS_ERR_INTMASK_RD(src)                       (((src) & 0x00200000)>>21)
#define GS_ERR_INTMASK_WR(src)                  (((u32)(src)<<21) & 0x00200000)
#define GS_ERR_INTMASK_SET(dst,src) \
                      (((dst) & ~0x00200000) | (((u32)(src)<<21) & 0x00200000))
/*    Mask Register Fields bad_dom_id_intMask    */
#define BAD_DOM_ID_INTMASK_WIDTH                                              1
#define BAD_DOM_ID_INTMASK_SHIFT                                             20
#define BAD_DOM_ID_INTMASK_MASK                                      0x00100000
#define BAD_DOM_ID_INTMASK_RD(src)                   (((src) & 0x00100000)>>20)
#define BAD_DOM_ID_INTMASK_WR(src)              (((u32)(src)<<20) & 0x00100000)
#define BAD_DOM_ID_INTMASK_SET(dst,src) \
                      (((dst) & ~0x00100000) | (((u32)(src)<<20) & 0x00100000))

/*	Register dma_timeoc	*/ 
/*	 Fields timeo_exp	 */
#define TIMEO_EXP_WIDTH                                                      32
#define TIMEO_EXP_SHIFT                                                       0
#define TIMEO_EXP_MASK                                               0xffffffff
#define TIMEO_EXP_RD(src)                                (((src) & 0xffffffff))
#define TIMEO_EXP_WR(src)                           (((u32)(src)) & 0xffffffff)
#define TIMEO_EXP_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register dma_fpb_timeoc	*/ 
/*	 Fields timeo_exp	 */
#define TIMEO_EXP_F1_WIDTH                                                   32
#define TIMEO_EXP_F1_SHIFT                                                    0
#define TIMEO_EXP_F1_MASK                                            0xffffffff
#define TIMEO_EXP_F1_RD(src)                             (((src) & 0xffffffff))
#define TIMEO_EXP_F1_WR(src)                        (((u32)(src)) & 0xffffffff)
#define TIMEO_EXP_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register dma_fifo_lev0	*/ 
/*	 Fields fifo_0_lev	 */
#define FIFO_0_LEV0_WIDTH                                                     8
#define FIFO_0_LEV0_SHIFT                                                    24
#define FIFO_0_LEV0_MASK                                             0xff000000
#define FIFO_0_LEV0_RD(src)                          (((src) & 0xff000000)>>24)
#define FIFO_0_LEV0_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))
/*	 Fields fifo_1_lev	 */
#define FIFO_1_LEV0_WIDTH                                                     8
#define FIFO_1_LEV0_SHIFT                                                    16
#define FIFO_1_LEV0_MASK                                             0x00ff0000
#define FIFO_1_LEV0_RD(src)                          (((src) & 0x00ff0000)>>16)
#define FIFO_1_LEV0_SET(dst,src) \
                      (((dst) & ~0x00ff0000) | (((u32)(src)<<16) & 0x00ff0000))
/*	 Fields fifo_2_lev	 */
#define FIFO_2_LEV0_WIDTH                                                     8
#define FIFO_2_LEV0_SHIFT                                                     8
#define FIFO_2_LEV0_MASK                                             0x0000ff00
#define FIFO_2_LEV0_RD(src)                           (((src) & 0x0000ff00)>>8)
#define FIFO_2_LEV0_SET(dst,src) \
                       (((dst) & ~0x0000ff00) | (((u32)(src)<<8) & 0x0000ff00))
/*	 Fields fifo_3_lev	 */
#define FIFO_3_LEV0_WIDTH                                                     8
#define FIFO_3_LEV0_SHIFT                                                     0
#define FIFO_3_LEV0_MASK                                             0x000000ff
#define FIFO_3_LEV0_RD(src)                              (((src) & 0x000000ff))
#define FIFO_3_LEV0_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register dma_fifo_lev1	*/ 
/*	 Fields DMA_FIFO_LEV1_Reserved	 */
#define DMA_FIFO_LEV1_RESERVED1_WIDTH                                        25
#define DMA_FIFO_LEV1_RESERVED1_SHIFT                                         7
#define DMA_FIFO_LEV1_RESERVED1_MASK                                 0xffffff80
#define DMA_FIFO_LEV1_RESERVED1_RD(src)               (((src) & 0xffffff80)>>7)
#define DMA_FIFO_LEV1_RESERVED1_SET(dst,src) \
                       (((dst) & ~0xffffff80) | (((u32)(src)<<7) & 0xffffff80))
/*	 Fields fifo_lev	 */
#define FIFO_LEV1_WIDTH                                                       7
#define FIFO_LEV1_SHIFT                                                       0
#define FIFO_LEV1_MASK                                               0x0000007f
#define FIFO_LEV1_RD(src)                                (((src) & 0x0000007f))
#define FIFO_LEV1_SET(dst,src) \
                          (((dst) & ~0x0000007f) | (((u32)(src)) & 0x0000007f))

/*	Register dma_false_int	*/ 
/*	 Fields false_int	 */
#define FALSE_INT_WIDTH                                                       1
#define FALSE_INT_SHIFT                                                       0
#define FALSE_INT_MASK                                               0x00000001
#define FALSE_INT_RD(src)                                (((src) & 0x00000001))
#define FALSE_INT_WR(src)                           (((u32)(src)) & 0x00000001)
#define FALSE_INT_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register dma_rst_ch	*/ 
/*	 Fields rst_ch_0	 */
#define RST_CH_0_WIDTH                                                        1
#define RST_CH_0_SHIFT                                                       31
#define RST_CH_0_MASK                                                0x80000000
#define RST_CH_0_RD(src)                             (((src) & 0x80000000)>>31)
#define RST_CH_0_WR(src)                        (((u32)(src)<<31) & 0x80000000)
#define RST_CH_0_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields rst_ch_1	 */
#define RST_CH_1_WIDTH                                                        1
#define RST_CH_1_SHIFT                                                       30
#define RST_CH_1_MASK                                                0x40000000
#define RST_CH_1_RD(src)                             (((src) & 0x40000000)>>30)
#define RST_CH_1_WR(src)                        (((u32)(src)<<30) & 0x40000000)
#define RST_CH_1_SET(dst,src) \
                      (((dst) & ~0x40000000) | (((u32)(src)<<30) & 0x40000000))
/*	 Fields rst_ch_2	 */
#define RST_CH_2_WIDTH                                                        1
#define RST_CH_2_SHIFT                                                       29
#define RST_CH_2_MASK                                                0x20000000
#define RST_CH_2_RD(src)                             (((src) & 0x20000000)>>29)
#define RST_CH_2_WR(src)                        (((u32)(src)<<29) & 0x20000000)
#define RST_CH_2_SET(dst,src) \
                      (((dst) & ~0x20000000) | (((u32)(src)<<29) & 0x20000000))
/*	 Fields rst_ch_3	 */
#define RST_CH_3_WIDTH                                                        1
#define RST_CH_3_SHIFT                                                       28
#define RST_CH_3_MASK                                                0x10000000
#define RST_CH_3_RD(src)                             (((src) & 0x10000000)>>28)
#define RST_CH_3_WR(src)                        (((u32)(src)<<28) & 0x10000000)
#define RST_CH_3_SET(dst,src) \
                      (((dst) & ~0x10000000) | (((u32)(src)<<28) & 0x10000000))
/*	 Fields dis_rm_pend	 */
#define DIS_RM_PEND_WIDTH                                                     1
#define DIS_RM_PEND_SHIFT                                                    27
#define DIS_RM_PEND_MASK                                             0x08000000
#define DIS_RM_PEND_RD(src)                          (((src) & 0x08000000)>>27)
#define DIS_RM_PEND_WR(src)                     (((u32)(src)<<27) & 0x08000000)
#define DIS_RM_PEND_SET(dst,src) \
                      (((dst) & ~0x08000000) | (((u32)(src)<<27) & 0x08000000))

/*	Global Base Address	*/
#define DMA_QMI_SLAVE_BASE_ADDR			0xddd827800ULL

/*    Address QMI_SLAVE  Registers */
#define DMA_CFGSSQMI0_ADDR                                           0x00000000
#define DMA_CFGSSQMI0_DEFAULT                                        0x4002c821
#define DMA_CFGSSQMI1_ADDR                                           0x00000004
#define DMA_CFGSSQMI1_DEFAULT                                        0x2de8b77c
#define DMA_CFGSSQMIFPBUFFER_ADDR                                    0x00000008
#define DMA_CFGSSQMIFPBUFFER_DEFAULT                                 0x00000000
#define DMA_CFGSSQMIWQBUFFER_ADDR                                    0x0000000c
#define DMA_CFGSSQMIWQBUFFER_DEFAULT                                 0x00000000
#define DMA_STSSSQMIFPBUFFER0_ADDR                                   0x00000010
#define DMA_STSSSQMIFPBUFFER0_DEFAULT                                0x00000000
#define DMA_STSSSQMIFPBUFFER1_ADDR                                   0x00000014
#define DMA_STSSSQMIFPBUFFER1_DEFAULT                                0x00000000
#define DMA_STSSSQMIFPBUFFER2_ADDR                                   0x00000018
#define DMA_STSSSQMIFPBUFFER2_DEFAULT                                0x00000000
#define DMA_STSSSQMIWQBUFFER0_ADDR                                   0x0000001c
#define DMA_STSSSQMIWQBUFFER0_DEFAULT                                0x00000000
#define DMA_STSSSQMIWQBUFFER1_ADDR                                   0x00000020
#define DMA_STSSSQMIWQBUFFER1_DEFAULT                                0x00000000
#define DMA_STSSSQMIWQBUFFER2_ADDR                                   0x00000024
#define DMA_STSSSQMIWQBUFFER2_DEFAULT                                0x00000000
#define DMA_STSSSQMIWQBUFFER3_ADDR                                   0x00000028
#define DMA_STSSSQMIWQBUFFER3_DEFAULT                                0x00000000
#define DMA_CFGSSQMISAB_ADDR                                         0x0000002c
#define DMA_CFGSSQMISAB_DEFAULT                                      0x00000000
#define DMA_CFGSSQMISAB0_ADDR                                        0x00000030
#define DMA_CFGSSQMISAB0_DEFAULT                                     0x00000000
#define DMA_CFGSSQMISAB1_ADDR                                        0x00000034
#define DMA_CFGSSQMISAB1_DEFAULT                                     0x00000000
#define DMA_CFGSSQMISAB2_ADDR                                        0x00000038
#define DMA_CFGSSQMISAB2_DEFAULT                                     0x00000000
#define DMA_CFGSSQMISAB3_ADDR                                        0x0000003c
#define DMA_CFGSSQMISAB3_DEFAULT                                     0x00000000
#define DMA_CFGSSQMISAB4_ADDR                                        0x00000040
#define DMA_CFGSSQMISAB4_DEFAULT                                     0x00000000
#define DMA_STSSSQMIINT0_ADDR                                        0x00000044
#define DMA_STSSSQMIINT0_DEFAULT                                     0x00000000
#define DMA_STSSSQMIINT0MASK_ADDR                                    0x00000048
#define DMA_STSSSQMIINT1_ADDR                                        0x0000004c
#define DMA_STSSSQMIINT1_DEFAULT                                     0x00000000
#define DMA_STSSSQMIINT1MASK_ADDR                                    0x00000050
#define DMA_STSSSQMIINT2_ADDR                                        0x00000054
#define DMA_STSSSQMIINT2_DEFAULT                                     0x00000000
#define DMA_STSSSQMIINT2MASK_ADDR                                    0x00000058
#define DMA_STSSSQMIINT3_ADDR                                        0x0000005c
#define DMA_STSSSQMIINT3_DEFAULT                                     0x00000000
#define DMA_STSSSQMIINT3MASK_ADDR                                    0x00000060
#define DMA_STSSSQMIINT4_ADDR                                        0x00000064
#define DMA_STSSSQMIINT4_DEFAULT                                     0x00000000
#define DMA_STSSSQMIINT4MASK_ADDR                                    0x00000068
#define DMA_CFGSSQMIDBGCTRL_ADDR                                     0x0000006c
#define DMA_CFGSSQMIDBGCTRL_DEFAULT                                  0x00000000
#define DMA_CFGSSQMIDBGDATA0_ADDR                                    0x00000070
#define DMA_CFGSSQMIDBGDATA0_DEFAULT                                 0x00000000
#define DMA_CFGSSQMIDBGDATA1_ADDR                                    0x00000074
#define DMA_CFGSSQMIDBGDATA1_DEFAULT                                 0x00000000
#define DMA_CFGSSQMIDBGDATA2_ADDR                                    0x00000078
#define DMA_CFGSSQMIDBGDATA2_DEFAULT                                 0x00000000
#define DMA_CFGSSQMIDBGDATA3_ADDR                                    0x0000007c
#define DMA_CFGSSQMIDBGDATA3_DEFAULT                                 0x00000000
#define DMA_STSSSQMIDBGDATA_ADDR                                     0x00000080
#define DMA_STSSSQMIDBGDATA_DEFAULT                                  0x00000000
#define DMA_CFGSSQMILITE_ADDR                                        0x00000084
#define DMA_CFGSSQMILITE_DEFAULT                                     0x00003400
#define DMA_CFGSSQMIFPQASSOC_ADDR                                    0x00000088
#define DMA_CFGSSQMIFPQASSOC_DEFAULT                                 0x00030000
#define DMA_CFGSSQMIWQASSOC_ADDR                                     0x0000008c
#define DMA_CFGSSQMIWQASSOC_DEFAULT                                  0x00010000
#define DMA_CFGSSQMIMEMORY_ADDR                                      0x00000090
#define DMA_CFGSSQMIMEMORY_DEFAULT                                   0x00000000
#define DMA_STSSSQMIFIFO_ADDR                                        0x00000094
#define DMA_STSSSQMIFIFO_DEFAULT                                     0x07ffffff
#define DMA_CFGSSQMIDOMEN_ADDR                                       0x00000098
#define DMA_CFGSSQMIDOMEN_DEFAULT                                    0x01ffffff
#define DMA_CFGSSQMIDOMVAL0_ADDR                                     0x0000009c
#define DMA_CFGSSQMIDOMVAL0_DEFAULT                                  0x000000fb
#define DMA_CFGSSQMIDOMVAL1_ADDR                                     0x000000a0
#define DMA_CFGSSQMIDOMVAL1_DEFAULT                                  0x000000fb
#define DMA_CFGSSQMIDOMVAL2_ADDR                                     0x000000a4
#define DMA_CFGSSQMIDOMVAL2_DEFAULT                                  0x00000004

/*	Register CfgSsQmi0	*/ 
/*	 Fields WQBavailFMWait	 */
#define DMA_WQBAVAILFMWAIT0_WIDTH                                             1
#define DMA_WQBAVAILFMWAIT0_SHIFT                                            30
#define DMA_WQBAVAILFMWAIT0_MASK                                     0x40000000
#define DMA_WQBAVAILFMWAIT0_RD(src)                  (((src) & 0x40000000)>>30)
#define DMA_WQBAVAILFMWAIT0_WR(src)             (((u32)(src)<<30) & 0x40000000)
#define DMA_WQBAVAILFMWAIT0_SET(dst,src) \
                      (((dst) & ~0x40000000) | (((u32)(src)<<30) & 0x40000000))
/*	 Fields WQ16Disable	 */
#define DMA_WQ16DISABLE0_WIDTH                                                1
#define DMA_WQ16DISABLE0_SHIFT                                               29
#define DMA_WQ16DISABLE0_MASK                                        0x20000000
#define DMA_WQ16DISABLE0_RD(src)                     (((src) & 0x20000000)>>29)
#define DMA_WQ16DISABLE0_WR(src)                (((u32)(src)<<29) & 0x20000000)
#define DMA_WQ16DISABLE0_SET(dst,src) \
                      (((dst) & ~0x20000000) | (((u32)(src)<<29) & 0x20000000))
/*	 Fields WQ16Reset	 */
#define DMA_WQ16RESET0_WIDTH                                                  1
#define DMA_WQ16RESET0_SHIFT                                                 28
#define DMA_WQ16RESET0_MASK                                          0x10000000
#define DMA_WQ16RESET0_RD(src)                       (((src) & 0x10000000)>>28)
#define DMA_WQ16RESET0_WR(src)                  (((u32)(src)<<28) & 0x10000000)
#define DMA_WQ16RESET0_SET(dst,src) \
                      (((dst) & ~0x10000000) | (((u32)(src)<<28) & 0x10000000))
/*	 Fields FP17Disable	 */
#define DMA_FP17DISABLE0_WIDTH                                                1
#define DMA_FP17DISABLE0_SHIFT                                               27
#define DMA_FP17DISABLE0_MASK                                        0x08000000
#define DMA_FP17DISABLE0_RD(src)                     (((src) & 0x08000000)>>27)
#define DMA_FP17DISABLE0_WR(src)                (((u32)(src)<<27) & 0x08000000)
#define DMA_FP17DISABLE0_SET(dst,src) \
                      (((dst) & ~0x08000000) | (((u32)(src)<<27) & 0x08000000))
/*	 Fields FP16Disable	 */
#define DMA_FP16DISABLE0_WIDTH                                                1
#define DMA_FP16DISABLE0_SHIFT                                               26
#define DMA_FP16DISABLE0_MASK                                        0x04000000
#define DMA_FP16DISABLE0_RD(src)                     (((src) & 0x04000000)>>26)
#define DMA_FP16DISABLE0_WR(src)                (((u32)(src)<<26) & 0x04000000)
#define DMA_FP16DISABLE0_SET(dst,src) \
                      (((dst) & ~0x04000000) | (((u32)(src)<<26) & 0x04000000))
/*	 Fields FP17Reset	 */
#define DMA_FP17RESET0_WIDTH                                                  1
#define DMA_FP17RESET0_SHIFT                                                 25
#define DMA_FP17RESET0_MASK                                          0x02000000
#define DMA_FP17RESET0_RD(src)                       (((src) & 0x02000000)>>25)
#define DMA_FP17RESET0_WR(src)                  (((u32)(src)<<25) & 0x02000000)
#define DMA_FP17RESET0_SET(dst,src) \
                      (((dst) & ~0x02000000) | (((u32)(src)<<25) & 0x02000000))
/*	 Fields FP16Reset	 */
#define DMA_FP16RESET0_WIDTH                                                  1
#define DMA_FP16RESET0_SHIFT                                                 24
#define DMA_FP16RESET0_MASK                                          0x01000000
#define DMA_FP16RESET0_RD(src)                       (((src) & 0x01000000)>>24)
#define DMA_FP16RESET0_WR(src)                  (((u32)(src)<<24) & 0x01000000)
#define DMA_FP16RESET0_SET(dst,src) \
                      (((dst) & ~0x01000000) | (((u32)(src)<<24) & 0x01000000))
/*	 Fields FPDecDiffThreshold	 */
#define DMA_FPDECDIFFTHRESHOLD0_WIDTH                                         3
#define DMA_FPDECDIFFTHRESHOLD0_SHIFT                                        21
#define DMA_FPDECDIFFTHRESHOLD0_MASK                                 0x00e00000
#define DMA_FPDECDIFFTHRESHOLD0_RD(src)              (((src) & 0x00e00000)>>21)
#define DMA_FPDECDIFFTHRESHOLD0_WR(src)         (((u32)(src)<<21) & 0x00e00000)
#define DMA_FPDECDIFFTHRESHOLD0_SET(dst,src) \
                      (((dst) & ~0x00e00000) | (((u32)(src)<<21) & 0x00e00000))
/*	 Fields WQDecDiffThreshold	 */
#define DMA_WQDECDIFFTHRESHOLD0_WIDTH                                         4
#define DMA_WQDECDIFFTHRESHOLD0_SHIFT                                        17
#define DMA_WQDECDIFFTHRESHOLD0_MASK                                 0x001e0000
#define DMA_WQDECDIFFTHRESHOLD0_RD(src)              (((src) & 0x001e0000)>>17)
#define DMA_WQDECDIFFTHRESHOLD0_WR(src)         (((u32)(src)<<17) & 0x001e0000)
#define DMA_WQDECDIFFTHRESHOLD0_SET(dst,src) \
                      (((dst) & ~0x001e0000) | (((u32)(src)<<17) & 0x001e0000))
/*	 Fields DeallocThreshold	 */
#define DMA_DEALLOCTHRESHOLD0_WIDTH                                           3
#define DMA_DEALLOCTHRESHOLD0_SHIFT                                          14
#define DMA_DEALLOCTHRESHOLD0_MASK                                   0x0001c000
#define DMA_DEALLOCTHRESHOLD0_RD(src)                (((src) & 0x0001c000)>>14)
#define DMA_DEALLOCTHRESHOLD0_WR(src)           (((u32)(src)<<14) & 0x0001c000)
#define DMA_DEALLOCTHRESHOLD0_SET(dst,src) \
                      (((dst) & ~0x0001c000) | (((u32)(src)<<14) & 0x0001c000))
/*	 Fields FPDecThreshold	 */
#define DMA_FPDECTHRESHOLD0_WIDTH                                             3
#define DMA_FPDECTHRESHOLD0_SHIFT                                            11
#define DMA_FPDECTHRESHOLD0_MASK                                     0x00003800
#define DMA_FPDECTHRESHOLD0_RD(src)                  (((src) & 0x00003800)>>11)
#define DMA_FPDECTHRESHOLD0_WR(src)             (((u32)(src)<<11) & 0x00003800)
#define DMA_FPDECTHRESHOLD0_SET(dst,src) \
                      (((dst) & ~0x00003800) | (((u32)(src)<<11) & 0x00003800))
/*	 Fields FPBAvlThreshold	 */
#define DMA_FPBAVLTHRESHOLD0_WIDTH                                            3
#define DMA_FPBAVLTHRESHOLD0_SHIFT                                            8
#define DMA_FPBAVLTHRESHOLD0_MASK                                    0x00000700
#define DMA_FPBAVLTHRESHOLD0_RD(src)                  (((src) & 0x00000700)>>8)
#define DMA_FPBAVLTHRESHOLD0_WR(src)             (((u32)(src)<<8) & 0x00000700)
#define DMA_FPBAVLTHRESHOLD0_SET(dst,src) \
                       (((dst) & ~0x00000700) | (((u32)(src)<<8) & 0x00000700))
/*	 Fields WQDecThreshold	 */
#define DMA_WQDECTHRESHOLD0_WIDTH                                             4
#define DMA_WQDECTHRESHOLD0_SHIFT                                             4
#define DMA_WQDECTHRESHOLD0_MASK                                     0x000000f0
#define DMA_WQDECTHRESHOLD0_RD(src)                   (((src) & 0x000000f0)>>4)
#define DMA_WQDECTHRESHOLD0_WR(src)              (((u32)(src)<<4) & 0x000000f0)
#define DMA_WQDECTHRESHOLD0_SET(dst,src) \
                       (((dst) & ~0x000000f0) | (((u32)(src)<<4) & 0x000000f0))
/*	 Fields WQBAvlThreshold	 */
#define DMA_WQBAVLTHRESHOLD0_WIDTH                                            4
#define DMA_WQBAVLTHRESHOLD0_SHIFT                                            0
#define DMA_WQBAVLTHRESHOLD0_MASK                                    0x0000000f
#define DMA_WQBAVLTHRESHOLD0_RD(src)                     (((src) & 0x0000000f))
#define DMA_WQBAVLTHRESHOLD0_WR(src)                (((u32)(src)) & 0x0000000f)
#define DMA_WQBAVLTHRESHOLD0_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register CfgSsQmi1	*/ 
/*	 Fields CmOverrideLLFields	 */
#define DMA_CMOVERRIDELLFIELDS1_WIDTH                                         1
#define DMA_CMOVERRIDELLFIELDS1_SHIFT                                        29
#define DMA_CMOVERRIDELLFIELDS1_MASK                                 0x20000000
#define DMA_CMOVERRIDELLFIELDS1_RD(src)              (((src) & 0x20000000)>>29)
#define DMA_CMOVERRIDELLFIELDS1_WR(src)         (((u32)(src)<<29) & 0x20000000)
#define DMA_CMOVERRIDELLFIELDS1_SET(dst,src) \
                      (((dst) & ~0x20000000) | (((u32)(src)<<29) & 0x20000000))
/*	 Fields CmCtrlbuffThreshold	 */
#define DMA_CMCTRLBUFFTHRESHOLD1_WIDTH                                        3
#define DMA_CMCTRLBUFFTHRESHOLD1_SHIFT                                       26
#define DMA_CMCTRLBUFFTHRESHOLD1_MASK                                0x1c000000
#define DMA_CMCTRLBUFFTHRESHOLD1_RD(src)             (((src) & 0x1c000000)>>26)
#define DMA_CMCTRLBUFFTHRESHOLD1_WR(src)        (((u32)(src)<<26) & 0x1c000000)
#define DMA_CMCTRLBUFFTHRESHOLD1_SET(dst,src) \
                      (((dst) & ~0x1c000000) | (((u32)(src)<<26) & 0x1c000000))
/*	 Fields CmDatabuffThreshold	 */
#define DMA_CMDATABUFFTHRESHOLD1_WIDTH                                        5
#define DMA_CMDATABUFFTHRESHOLD1_SHIFT                                       21
#define DMA_CMDATABUFFTHRESHOLD1_MASK                                0x03e00000
#define DMA_CMDATABUFFTHRESHOLD1_RD(src)             (((src) & 0x03e00000)>>21)
#define DMA_CMDATABUFFTHRESHOLD1_WR(src)        (((u32)(src)<<21) & 0x03e00000)
#define DMA_CMDATABUFFTHRESHOLD1_SET(dst,src) \
                      (((dst) & ~0x03e00000) | (((u32)(src)<<21) & 0x03e00000))
/*	 Fields CmMsgfThreshold	 */
#define DMA_CMMSGFTHRESHOLD1_WIDTH                                            4
#define DMA_CMMSGFTHRESHOLD1_SHIFT                                           17
#define DMA_CMMSGFTHRESHOLD1_MASK                                    0x001e0000
#define DMA_CMMSGFTHRESHOLD1_RD(src)                 (((src) & 0x001e0000)>>17)
#define DMA_CMMSGFTHRESHOLD1_WR(src)            (((u32)(src)<<17) & 0x001e0000)
#define DMA_CMMSGFTHRESHOLD1_SET(dst,src) \
                      (((dst) & ~0x001e0000) | (((u32)(src)<<17) & 0x001e0000))
/*	 Fields CmRegfThreshold	 */
#define DMA_CMREGFTHRESHOLD1_WIDTH                                            3
#define DMA_CMREGFTHRESHOLD1_SHIFT                                           14
#define DMA_CMREGFTHRESHOLD1_MASK                                    0x0001c000
#define DMA_CMREGFTHRESHOLD1_RD(src)                 (((src) & 0x0001c000)>>14)
#define DMA_CMREGFTHRESHOLD1_WR(src)            (((u32)(src)<<14) & 0x0001c000)
#define DMA_CMREGFTHRESHOLD1_SET(dst,src) \
                      (((dst) & ~0x0001c000) | (((u32)(src)<<14) & 0x0001c000))
/*	 Fields QMDeviceAddress	 */
#define DMA_QMDEVICEADDRESS1_WIDTH                                           14
#define DMA_QMDEVICEADDRESS1_SHIFT                                            0
#define DMA_QMDEVICEADDRESS1_MASK                                    0x00003fff
#define DMA_QMDEVICEADDRESS1_RD(src)                     (((src) & 0x00003fff))
#define DMA_QMDEVICEADDRESS1_WR(src)                (((u32)(src)) & 0x00003fff)
#define DMA_QMDEVICEADDRESS1_SET(dst,src) \
                          (((dst) & ~0x00003fff) | (((u32)(src)) & 0x00003fff))

/*	Register CfgSsQmiFPBuffer	*/ 
/*	 Fields Disable	 */
#define DMA_DISABLE_WIDTH                                                    16
#define DMA_DISABLE_SHIFT                                                    16
#define DMA_DISABLE_MASK                                             0xffff0000
#define DMA_DISABLE_RD(src)                          (((src) & 0xffff0000)>>16)
#define DMA_DISABLE_WR(src)                     (((u32)(src)<<16) & 0xffff0000)
#define DMA_DISABLE_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields Reset	 */
#define DMA_RESET_WIDTH                                                      16
#define DMA_RESET_SHIFT                                                       0
#define DMA_RESET_MASK                                               0x0000ffff
#define DMA_RESET_RD(src)                                (((src) & 0x0000ffff))
#define DMA_RESET_WR(src)                           (((u32)(src)) & 0x0000ffff)
#define DMA_RESET_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register CfgSsQmiWQBuffer	*/ 
/*	 Fields Disable	 */
#define DMA_DISABLE_F1_WIDTH                                                 16
#define DMA_DISABLE_F1_SHIFT                                                 16
#define DMA_DISABLE_F1_MASK                                          0xffff0000
#define DMA_DISABLE_F1_RD(src)                       (((src) & 0xffff0000)>>16)
#define DMA_DISABLE_F1_WR(src)                  (((u32)(src)<<16) & 0xffff0000)
#define DMA_DISABLE_F1_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields Reset	 */
#define DMA_RESET_F1_WIDTH                                                   16
#define DMA_RESET_F1_SHIFT                                                    0
#define DMA_RESET_F1_MASK                                            0x0000ffff
#define DMA_RESET_F1_RD(src)                             (((src) & 0x0000ffff))
#define DMA_RESET_F1_WR(src)                        (((u32)(src)) & 0x0000ffff)
#define DMA_RESET_F1_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register StsSsQmiFPBuffer0	*/ 
/*	 Fields WritePointer	 */
#define DMA_WRITEPOINTER0_WIDTH                                              32
#define DMA_WRITEPOINTER0_SHIFT                                               0
#define DMA_WRITEPOINTER0_MASK                                       0xffffffff
#define DMA_WRITEPOINTER0_RD(src)                        (((src) & 0xffffffff))
#define DMA_WRITEPOINTER0_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register StsSsQmiFPBuffer1	*/ 
/*	 Fields WritePointer	 */
#define DMA_WRITEPOINTER1_WIDTH                                               4
#define DMA_WRITEPOINTER1_SHIFT                                              24
#define DMA_WRITEPOINTER1_MASK                                       0x0f000000
#define DMA_WRITEPOINTER1_RD(src)                    (((src) & 0x0f000000)>>24)
#define DMA_WRITEPOINTER1_SET(dst,src) \
                      (((dst) & ~0x0f000000) | (((u32)(src)<<24) & 0x0f000000))
/*	 Fields Entries	 */
#define DMA_ENTRIES1_WIDTH                                                   24
#define DMA_ENTRIES1_SHIFT                                                    0
#define DMA_ENTRIES1_MASK                                            0x00ffffff
#define DMA_ENTRIES1_RD(src)                             (((src) & 0x00ffffff))
#define DMA_ENTRIES1_SET(dst,src) \
                          (((dst) & ~0x00ffffff) | (((u32)(src)) & 0x00ffffff))

/*	Register StsSsQmiFPBuffer2	*/ 
/*	 Fields Entries	 */
#define DMA_ENTRIES2_WIDTH                                                   30
#define DMA_ENTRIES2_SHIFT                                                    0
#define DMA_ENTRIES2_MASK                                            0x3fffffff
#define DMA_ENTRIES2_RD(src)                             (((src) & 0x3fffffff))
#define DMA_ENTRIES2_SET(dst,src) \
                          (((dst) & ~0x3fffffff) | (((u32)(src)) & 0x3fffffff))

/*	Register StsSsQmiWQBuffer0	*/ 
/*	 Fields WritePointer	 */
#define DMA_WRITEPOINTER0_F1_WIDTH                                           24
#define DMA_WRITEPOINTER0_F1_SHIFT                                            0
#define DMA_WRITEPOINTER0_F1_MASK                                    0x00ffffff
#define DMA_WRITEPOINTER0_F1_RD(src)                     (((src) & 0x00ffffff))
#define DMA_WRITEPOINTER0_F1_SET(dst,src) \
                          (((dst) & ~0x00ffffff) | (((u32)(src)) & 0x00ffffff))

/*	Register StsSsQmiWQBuffer1	*/ 
/*	 Fields Entries	 */
#define DMA_ENTRIES1_F1_WIDTH                                                 4
#define DMA_ENTRIES1_F1_SHIFT                                                27
#define DMA_ENTRIES1_F1_MASK                                         0x78000000
#define DMA_ENTRIES1_F1_RD(src)                      (((src) & 0x78000000)>>27)
#define DMA_ENTRIES1_F1_SET(dst,src) \
                      (((dst) & ~0x78000000) | (((u32)(src)<<27) & 0x78000000))
/*	 Fields WritePointer	 */
#define DMA_WRITEPOINTER1_F1_WIDTH                                           27
#define DMA_WRITEPOINTER1_F1_SHIFT                                            0
#define DMA_WRITEPOINTER1_F1_MASK                                    0x07ffffff
#define DMA_WRITEPOINTER1_F1_RD(src)                     (((src) & 0x07ffffff))
#define DMA_WRITEPOINTER1_F1_SET(dst,src) \
                          (((dst) & ~0x07ffffff) | (((u32)(src)) & 0x07ffffff))

/*	Register StsSsQmiWQBuffer2	*/ 
/*	 Fields Entries	 */
#define DMA_ENTRIES2_F1_WIDTH                                                32
#define DMA_ENTRIES2_F1_SHIFT                                                 0
#define DMA_ENTRIES2_F1_MASK                                         0xffffffff
#define DMA_ENTRIES2_F1_RD(src)                          (((src) & 0xffffffff))
#define DMA_ENTRIES2_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register StsSsQmiWQBuffer3	*/ 
/*	 Fields Entries	 */
#define DMA_ENTRIES3_WIDTH                                                   32
#define DMA_ENTRIES3_SHIFT                                                    0
#define DMA_ENTRIES3_MASK                                            0xffffffff
#define DMA_ENTRIES3_RD(src)                             (((src) & 0xffffffff))
#define DMA_ENTRIES3_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CfgSsQmiSab	*/ 
/*	 Fields EnableMonitoring	 */
#define DMA_ENABLEMONITORING_WIDTH                                           17
#define DMA_ENABLEMONITORING_SHIFT                                            0
#define DMA_ENABLEMONITORING_MASK                                    0x0001ffff
#define DMA_ENABLEMONITORING_RD(src)                     (((src) & 0x0001ffff))
#define DMA_ENABLEMONITORING_WR(src)                (((u32)(src)) & 0x0001ffff)
#define DMA_ENABLEMONITORING_SET(dst,src) \
                          (((dst) & ~0x0001ffff) | (((u32)(src)) & 0x0001ffff))

/*	Register CfgSsQmiSab0	*/ 
/*	 Fields QID	 */
#define DMA_QID0_WIDTH                                                       32
#define DMA_QID0_SHIFT                                                        0
#define DMA_QID0_MASK                                                0xffffffff
#define DMA_QID0_RD(src)                                 (((src) & 0xffffffff))
#define DMA_QID0_WR(src)                            (((u32)(src)) & 0xffffffff)
#define DMA_QID0_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CfgSsQmiSab1	*/ 
/*	 Fields QID	 */
#define DMA_QID1_WIDTH                                                       32
#define DMA_QID1_SHIFT                                                        0
#define DMA_QID1_MASK                                                0xffffffff
#define DMA_QID1_RD(src)                                 (((src) & 0xffffffff))
#define DMA_QID1_WR(src)                            (((u32)(src)) & 0xffffffff)
#define DMA_QID1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CfgSsQmiSab2	*/ 
/*	 Fields QID	 */
#define DMA_QID2_WIDTH                                                       32
#define DMA_QID2_SHIFT                                                        0
#define DMA_QID2_MASK                                                0xffffffff
#define DMA_QID2_RD(src)                                 (((src) & 0xffffffff))
#define DMA_QID2_WR(src)                            (((u32)(src)) & 0xffffffff)
#define DMA_QID2_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CfgSsQmiSab3	*/ 
/*	 Fields QID	 */
#define DMA_QID3_WIDTH                                                       32
#define DMA_QID3_SHIFT                                                        0
#define DMA_QID3_MASK                                                0xffffffff
#define DMA_QID3_RD(src)                                 (((src) & 0xffffffff))
#define DMA_QID3_WR(src)                            (((u32)(src)) & 0xffffffff)
#define DMA_QID3_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CfgSsQmiSab4	*/ 
/*	 Fields QID	 */
#define DMA_QID4_WIDTH                                                        8
#define DMA_QID4_SHIFT                                                        0
#define DMA_QID4_MASK                                                0x000000ff
#define DMA_QID4_RD(src)                                 (((src) & 0x000000ff))
#define DMA_QID4_WR(src)                            (((u32)(src)) & 0x000000ff)
#define DMA_QID4_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register StsSsQmiInt0	*/ 
/*	 Fields FPOverflow	 */
#define DMA_FPOVERFLOW0_WIDTH                                                18
#define DMA_FPOVERFLOW0_SHIFT                                                 0
#define DMA_FPOVERFLOW0_MASK                                         0x0003ffff
#define DMA_FPOVERFLOW0_RD(src)                          (((src) & 0x0003ffff))
#define DMA_FPOVERFLOW0_WR(src)                     (((u32)(src)) & 0x0003ffff)
#define DMA_FPOVERFLOW0_SET(dst,src) \
                          (((dst) & ~0x0003ffff) | (((u32)(src)) & 0x0003ffff))

/*	Register StsSsQmiInt0Mask	*/
/*    Mask Register Fields FPOverflowMask    */
#define DMA_FPOVERFLOWMASK_WIDTH                                             18
#define DMA_FPOVERFLOWMASK_SHIFT                                              0
#define DMA_FPOVERFLOWMASK_MASK                                      0x0003ffff
#define DMA_FPOVERFLOWMASK_RD(src)                       (((src) & 0x0003ffff))
#define DMA_FPOVERFLOWMASK_WR(src)                  (((u32)(src)) & 0x0003ffff)
#define DMA_FPOVERFLOWMASK_SET(dst,src) \
                          (((dst) & ~0x0003ffff) | (((u32)(src)) & 0x0003ffff))

/*	Register StsSsQmiInt1	*/ 
/*	 Fields WQOverflow	 */
#define DMA_WQOVERFLOW1_WIDTH                                                17
#define DMA_WQOVERFLOW1_SHIFT                                                 0
#define DMA_WQOVERFLOW1_MASK                                         0x0001ffff
#define DMA_WQOVERFLOW1_RD(src)                          (((src) & 0x0001ffff))
#define DMA_WQOVERFLOW1_WR(src)                     (((u32)(src)) & 0x0001ffff)
#define DMA_WQOVERFLOW1_SET(dst,src) \
                          (((dst) & ~0x0001ffff) | (((u32)(src)) & 0x0001ffff))

/*	Register StsSsQmiInt1Mask	*/
/*    Mask Register Fields WQOverflowMask    */
#define DMA_WQOVERFLOWMASK_WIDTH                                             17
#define DMA_WQOVERFLOWMASK_SHIFT                                              0
#define DMA_WQOVERFLOWMASK_MASK                                      0x0001ffff
#define DMA_WQOVERFLOWMASK_RD(src)                       (((src) & 0x0001ffff))
#define DMA_WQOVERFLOWMASK_WR(src)                  (((u32)(src)) & 0x0001ffff)
#define DMA_WQOVERFLOWMASK_SET(dst,src) \
                          (((dst) & ~0x0001ffff) | (((u32)(src)) & 0x0001ffff))

/*	Register StsSsQmiInt2	*/ 
/*	 Fields FPUnderRun	 */
#define DMA_FPUNDERRUN2_WIDTH                                                18
#define DMA_FPUNDERRUN2_SHIFT                                                 0
#define DMA_FPUNDERRUN2_MASK                                         0x0003ffff
#define DMA_FPUNDERRUN2_RD(src)                          (((src) & 0x0003ffff))
#define DMA_FPUNDERRUN2_WR(src)                     (((u32)(src)) & 0x0003ffff)
#define DMA_FPUNDERRUN2_SET(dst,src) \
                          (((dst) & ~0x0003ffff) | (((u32)(src)) & 0x0003ffff))

/*	Register StsSsQmiInt2Mask	*/
/*    Mask Register Fields FPUnderRunMask    */
#define DMA_FPUNDERRUNMASK_WIDTH                                             18
#define DMA_FPUNDERRUNMASK_SHIFT                                              0
#define DMA_FPUNDERRUNMASK_MASK                                      0x0003ffff
#define DMA_FPUNDERRUNMASK_RD(src)                       (((src) & 0x0003ffff))
#define DMA_FPUNDERRUNMASK_WR(src)                  (((u32)(src)) & 0x0003ffff)
#define DMA_FPUNDERRUNMASK_SET(dst,src) \
                          (((dst) & ~0x0003ffff) | (((u32)(src)) & 0x0003ffff))

/*	Register StsSsQmiInt3	*/ 
/*	 Fields WQUnderRun	 */
#define DMA_WQUNDERRUN3_WIDTH                                                17
#define DMA_WQUNDERRUN3_SHIFT                                                 0
#define DMA_WQUNDERRUN3_MASK                                         0x0001ffff
#define DMA_WQUNDERRUN3_RD(src)                          (((src) & 0x0001ffff))
#define DMA_WQUNDERRUN3_WR(src)                     (((u32)(src)) & 0x0001ffff)
#define DMA_WQUNDERRUN3_SET(dst,src) \
                          (((dst) & ~0x0001ffff) | (((u32)(src)) & 0x0001ffff))

/*	Register StsSsQmiInt3Mask	*/
/*    Mask Register Fields WQUnderRunMask    */
#define DMA_WQUNDERRUNMASK_WIDTH                                             17
#define DMA_WQUNDERRUNMASK_SHIFT                                              0
#define DMA_WQUNDERRUNMASK_MASK                                      0x0001ffff
#define DMA_WQUNDERRUNMASK_RD(src)                       (((src) & 0x0001ffff))
#define DMA_WQUNDERRUNMASK_WR(src)                  (((u32)(src)) & 0x0001ffff)
#define DMA_WQUNDERRUNMASK_SET(dst,src) \
                          (((dst) & ~0x0001ffff) | (((u32)(src)) & 0x0001ffff))

/*	Register StsSsQmiInt4	*/ 
/*	 Fields axiwcmr_slverr	 */
#define DMA_AXIWCMR_SLVERR4_WIDTH                                             1
#define DMA_AXIWCMR_SLVERR4_SHIFT                                             1
#define DMA_AXIWCMR_SLVERR4_MASK                                     0x00000002
#define DMA_AXIWCMR_SLVERR4_RD(src)                   (((src) & 0x00000002)>>1)
#define DMA_AXIWCMR_SLVERR4_WR(src)              (((u32)(src)<<1) & 0x00000002)
#define DMA_AXIWCMR_SLVERR4_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields axiwcmr_decerr	 */
#define DMA_AXIWCMR_DECERR4_WIDTH                                             1
#define DMA_AXIWCMR_DECERR4_SHIFT                                             0
#define DMA_AXIWCMR_DECERR4_MASK                                     0x00000001
#define DMA_AXIWCMR_DECERR4_RD(src)                      (((src) & 0x00000001))
#define DMA_AXIWCMR_DECERR4_WR(src)                 (((u32)(src)) & 0x00000001)
#define DMA_AXIWCMR_DECERR4_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register StsSsQmiInt4Mask	*/
/*    Mask Register Fields axiwcmr_slverrMask    */
#define DMA_AXIWCMR_SLVERRMASK_WIDTH                                          1
#define DMA_AXIWCMR_SLVERRMASK_SHIFT                                          1
#define DMA_AXIWCMR_SLVERRMASK_MASK                                  0x00000002
#define DMA_AXIWCMR_SLVERRMASK_RD(src)                (((src) & 0x00000002)>>1)
#define DMA_AXIWCMR_SLVERRMASK_WR(src)           (((u32)(src)<<1) & 0x00000002)
#define DMA_AXIWCMR_SLVERRMASK_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*    Mask Register Fields axiwcmr_decerrMask    */
#define DMA_AXIWCMR_DECERRMASK_WIDTH                                          1
#define DMA_AXIWCMR_DECERRMASK_SHIFT                                          0
#define DMA_AXIWCMR_DECERRMASK_MASK                                  0x00000001
#define DMA_AXIWCMR_DECERRMASK_RD(src)                   (((src) & 0x00000001))
#define DMA_AXIWCMR_DECERRMASK_WR(src)              (((u32)(src)) & 0x00000001)
#define DMA_AXIWCMR_DECERRMASK_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register CfgSsQmiDbgCtrl	*/ 
/*	 Fields PBID	 */
#define DMA_PBID_WIDTH                                                        6
#define DMA_PBID_SHIFT                                                       11
#define DMA_PBID_MASK                                                0x0001f800
#define DMA_PBID_RD(src)                             (((src) & 0x0001f800)>>11)
#define DMA_PBID_WR(src)                        (((u32)(src)<<11) & 0x0001f800)
#define DMA_PBID_SET(dst,src) \
                      (((dst) & ~0x0001f800) | (((u32)(src)<<11) & 0x0001f800))
/*	 Fields nAck	 */
#define DMA_NACK_WIDTH                                                        1
#define DMA_NACK_SHIFT                                                       10
#define DMA_NACK_MASK                                                0x00000400
#define DMA_NACK_RD(src)                             (((src) & 0x00000400)>>10)
#define DMA_NACK_WR(src)                        (((u32)(src)<<10) & 0x00000400)
#define DMA_NACK_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields last	 */
#define DMA_LAST_WIDTH                                                        1
#define DMA_LAST_SHIFT                                                        9
#define DMA_LAST_MASK                                                0x00000200
#define DMA_LAST_RD(src)                              (((src) & 0x00000200)>>9)
#define DMA_LAST_WR(src)                         (((u32)(src)<<9) & 0x00000200)
#define DMA_LAST_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields Push	 */
#define DMA_PUSH_WIDTH                                                        1
#define DMA_PUSH_SHIFT                                                        8
#define DMA_PUSH_MASK                                                0x00000100
#define DMA_PUSH_RD(src)                              (((src) & 0x00000100)>>8)
#define DMA_PUSH_WR(src)                         (((u32)(src)<<8) & 0x00000100)
#define DMA_PUSH_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*	 Fields Write	 */
#define DMA_WRITE_WIDTH                                                       1
#define DMA_WRITE_SHIFT                                                       7
#define DMA_WRITE_MASK                                               0x00000080
#define DMA_WRITE_RD(src)                             (((src) & 0x00000080)>>7)
#define DMA_WRITE_WR(src)                        (((u32)(src)<<7) & 0x00000080)
#define DMA_WRITE_SET(dst,src) \
                       (((dst) & ~0x00000080) | (((u32)(src)<<7) & 0x00000080))
/*	 Fields Pop	 */
#define DMA_POP_WIDTH                                                         1
#define DMA_POP_SHIFT                                                         6
#define DMA_POP_MASK                                                 0x00000040
#define DMA_POP_RD(src)                               (((src) & 0x00000040)>>6)
#define DMA_POP_WR(src)                          (((u32)(src)<<6) & 0x00000040)
#define DMA_POP_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*	 Fields Read	 */
#define DMA_READ_WIDTH                                                        1
#define DMA_READ_SHIFT                                                        5
#define DMA_READ_MASK                                                0x00000020
#define DMA_READ_RD(src)                              (((src) & 0x00000020)>>5)
#define DMA_READ_WR(src)                         (((u32)(src)<<5) & 0x00000020)
#define DMA_READ_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields BufferAddr	 */
#define DMA_BUFFERADDR_WIDTH                                                  5
#define DMA_BUFFERADDR_SHIFT                                                  0
#define DMA_BUFFERADDR_MASK                                          0x0000001f
#define DMA_BUFFERADDR_RD(src)                           (((src) & 0x0000001f))
#define DMA_BUFFERADDR_WR(src)                      (((u32)(src)) & 0x0000001f)
#define DMA_BUFFERADDR_SET(dst,src) \
                          (((dst) & ~0x0000001f) | (((u32)(src)) & 0x0000001f))

/*	Register CfgSsQmiDbgData0	*/ 
/*	 Fields Data	 */
#define DMA_DATA0_WIDTH                                                      32
#define DMA_DATA0_SHIFT                                                       0
#define DMA_DATA0_MASK                                               0xffffffff
#define DMA_DATA0_RD(src)                                (((src) & 0xffffffff))
#define DMA_DATA0_WR(src)                           (((u32)(src)) & 0xffffffff)
#define DMA_DATA0_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CfgSsQmiDbgData1	*/ 
/*	 Fields Data	 */
#define DMA_DATA1_WIDTH                                                      32
#define DMA_DATA1_SHIFT                                                       0
#define DMA_DATA1_MASK                                               0xffffffff
#define DMA_DATA1_RD(src)                                (((src) & 0xffffffff))
#define DMA_DATA1_WR(src)                           (((u32)(src)) & 0xffffffff)
#define DMA_DATA1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CfgSsQmiDbgData2	*/ 
/*	 Fields Data	 */
#define DMA_DATA2_WIDTH                                                      32
#define DMA_DATA2_SHIFT                                                       0
#define DMA_DATA2_MASK                                               0xffffffff
#define DMA_DATA2_RD(src)                                (((src) & 0xffffffff))
#define DMA_DATA2_WR(src)                           (((u32)(src)) & 0xffffffff)
#define DMA_DATA2_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CfgSsQmiDbgData3	*/ 
/*	 Fields Data	 */
#define DMA_DATA3_WIDTH                                                      32
#define DMA_DATA3_SHIFT                                                       0
#define DMA_DATA3_MASK                                               0xffffffff
#define DMA_DATA3_RD(src)                                (((src) & 0xffffffff))
#define DMA_DATA3_WR(src)                           (((u32)(src)) & 0xffffffff)
#define DMA_DATA3_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register StsSsQmiDbgData	*/ 
/*	 Fields Data	 */
#define DMA_DATA_WIDTH                                                       32
#define DMA_DATA_SHIFT                                                        0
#define DMA_DATA_MASK                                                0xffffffff
#define DMA_DATA_RD(src)                                 (((src) & 0xffffffff))
#define DMA_DATA_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CfgSsQmiLite	*/ 
/*	 Fields QMLiteDeviceAddress	 */
#define DMA_QMLITEDEVICEADDRESS_WIDTH                                        15
#define DMA_QMLITEDEVICEADDRESS_SHIFT                                         0
#define DMA_QMLITEDEVICEADDRESS_MASK                                 0x00007fff
#define DMA_QMLITEDEVICEADDRESS_RD(src)                  (((src) & 0x00007fff))
#define DMA_QMLITEDEVICEADDRESS_WR(src)             (((u32)(src)) & 0x00007fff)
#define DMA_QMLITEDEVICEADDRESS_SET(dst,src) \
                          (((dst) & ~0x00007fff) | (((u32)(src)) & 0x00007fff))

/*	Register CfgSsQmiFPQAssoc	*/ 
/*	 Fields Association	 */
#define DMA_ASSOCIATION_WIDTH                                                18
#define DMA_ASSOCIATION_SHIFT                                                 0
#define DMA_ASSOCIATION_MASK                                         0x0003ffff
#define DMA_ASSOCIATION_RD(src)                          (((src) & 0x0003ffff))
#define DMA_ASSOCIATION_WR(src)                     (((u32)(src)) & 0x0003ffff)
#define DMA_ASSOCIATION_SET(dst,src) \
                          (((dst) & ~0x0003ffff) | (((u32)(src)) & 0x0003ffff))

/*	Register CfgSsQmiWQAssoc	*/ 
/*	 Fields Association	 */
#define DMA_ASSOCIATION_F1_WIDTH                                             17
#define DMA_ASSOCIATION_F1_SHIFT                                              0
#define DMA_ASSOCIATION_F1_MASK                                      0x0001ffff
#define DMA_ASSOCIATION_F1_RD(src)                       (((src) & 0x0001ffff))
#define DMA_ASSOCIATION_F1_WR(src)                  (((u32)(src)) & 0x0001ffff)
#define DMA_ASSOCIATION_F1_SET(dst,src) \
                          (((dst) & ~0x0001ffff) | (((u32)(src)) & 0x0001ffff))

/*	Register CfgSsQmiMemory	*/ 
/*	 Fields RMA	 */
#define DMA_RMA_WIDTH                                                         2
#define DMA_RMA_SHIFT                                                         4
#define DMA_RMA_MASK                                                 0x00000030
#define DMA_RMA_RD(src)                               (((src) & 0x00000030)>>4)
#define DMA_RMA_WR(src)                          (((u32)(src)<<4) & 0x00000030)
#define DMA_RMA_SET(dst,src) \
                       (((dst) & ~0x00000030) | (((u32)(src)<<4) & 0x00000030))
/*	 Fields RMB	 */
#define DMA_RMB_WIDTH                                                         2
#define DMA_RMB_SHIFT                                                         2
#define DMA_RMB_MASK                                                 0x0000000c
#define DMA_RMB_RD(src)                               (((src) & 0x0000000c)>>2)
#define DMA_RMB_WR(src)                          (((u32)(src)<<2) & 0x0000000c)
#define DMA_RMB_SET(dst,src) \
                       (((dst) & ~0x0000000c) | (((u32)(src)<<2) & 0x0000000c))
/*	 Fields RMEA	 */
#define DMA_RMEA_WIDTH                                                        1
#define DMA_RMEA_SHIFT                                                        1
#define DMA_RMEA_MASK                                                0x00000002
#define DMA_RMEA_RD(src)                              (((src) & 0x00000002)>>1)
#define DMA_RMEA_WR(src)                         (((u32)(src)<<1) & 0x00000002)
#define DMA_RMEA_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields RMEB	 */
#define DMA_RMEB_WIDTH                                                        1
#define DMA_RMEB_SHIFT                                                        0
#define DMA_RMEB_MASK                                                0x00000001
#define DMA_RMEB_RD(src)                                 (((src) & 0x00000001))
#define DMA_RMEB_WR(src)                            (((u32)(src)) & 0x00000001)
#define DMA_RMEB_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register StsSsQmiFifo	*/ 
/*	 Fields empty	 */
#define DMA_EMPTY_WIDTH                                                      27
#define DMA_EMPTY_SHIFT                                                       0
#define DMA_EMPTY_MASK                                               0x07ffffff
#define DMA_EMPTY_RD(src)                                (((src) & 0x07ffffff))
#define DMA_EMPTY_SET(dst,src) \
                          (((dst) & ~0x07ffffff) | (((u32)(src)) & 0x07ffffff))

/*	Register CfgSsQMIDomEn	*/ 
/*	 Fields FPDomainIDEnable	 */
#define DMA_FPDOMAINIDENABLE_WIDTH                                           18
#define DMA_FPDOMAINIDENABLE_SHIFT                                            7
#define DMA_FPDOMAINIDENABLE_MASK                                    0x01ffff80
#define DMA_FPDOMAINIDENABLE_RD(src)                  (((src) & 0x01ffff80)>>7)
#define DMA_FPDOMAINIDENABLE_WR(src)             (((u32)(src)<<7) & 0x01ffff80)
#define DMA_FPDOMAINIDENABLE_SET(dst,src) \
                       (((dst) & ~0x01ffff80) | (((u32)(src)<<7) & 0x01ffff80))
/*	 Fields DecDomainIDEnable	 */
#define DMA_DECDOMAINIDENABLE_WIDTH                                           1
#define DMA_DECDOMAINIDENABLE_SHIFT                                           6
#define DMA_DECDOMAINIDENABLE_MASK                                   0x00000040
#define DMA_DECDOMAINIDENABLE_RD(src)                 (((src) & 0x00000040)>>6)
#define DMA_DECDOMAINIDENABLE_WR(src)            (((u32)(src)<<6) & 0x00000040)
#define DMA_DECDOMAINIDENABLE_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*	 Fields DeallocDomainIDEnable	 */
#define DMA_DEALLOCDOMAINIDENABLE_WIDTH                                       1
#define DMA_DEALLOCDOMAINIDENABLE_SHIFT                                       5
#define DMA_DEALLOCDOMAINIDENABLE_MASK                               0x00000020
#define DMA_DEALLOCDOMAINIDENABLE_RD(src)             (((src) & 0x00000020)>>5)
#define DMA_DEALLOCDOMAINIDENABLE_WR(src)        (((u32)(src)<<5) & 0x00000020)
#define DMA_DEALLOCDOMAINIDENABLE_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields CMDomainIDEnable	 */
#define DMA_CMDOMAINIDENABLE_WIDTH                                            5
#define DMA_CMDOMAINIDENABLE_SHIFT                                            0
#define DMA_CMDOMAINIDENABLE_MASK                                    0x0000001f
#define DMA_CMDOMAINIDENABLE_RD(src)                     (((src) & 0x0000001f))
#define DMA_CMDOMAINIDENABLE_WR(src)                (((u32)(src)) & 0x0000001f)
#define DMA_CMDOMAINIDENABLE_SET(dst,src) \
                          (((dst) & ~0x0000001f) | (((u32)(src)) & 0x0000001f))

/*	Register CfgSsQMIDomVal0	*/ 
/*	 Fields FPDomainID	 */
#define DMA_FPDOMAINID0_WIDTH                                                18
#define DMA_FPDOMAINID0_SHIFT                                                 3
#define DMA_FPDOMAINID0_MASK                                         0x001ffff8
#define DMA_FPDOMAINID0_RD(src)                       (((src) & 0x001ffff8)>>3)
#define DMA_FPDOMAINID0_WR(src)                  (((u32)(src)<<3) & 0x001ffff8)
#define DMA_FPDOMAINID0_SET(dst,src) \
                       (((dst) & ~0x001ffff8) | (((u32)(src)<<3) & 0x001ffff8))
/*	 Fields DecDomainID	 */
#define DMA_DECDOMAINID0_WIDTH                                                1
#define DMA_DECDOMAINID0_SHIFT                                                2
#define DMA_DECDOMAINID0_MASK                                        0x00000004
#define DMA_DECDOMAINID0_RD(src)                      (((src) & 0x00000004)>>2)
#define DMA_DECDOMAINID0_WR(src)                 (((u32)(src)<<2) & 0x00000004)
#define DMA_DECDOMAINID0_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields DeallocDomainID	 */
#define DMA_DEALLOCDOMAINID0_WIDTH                                            1
#define DMA_DEALLOCDOMAINID0_SHIFT                                            1
#define DMA_DEALLOCDOMAINID0_MASK                                    0x00000002
#define DMA_DEALLOCDOMAINID0_RD(src)                  (((src) & 0x00000002)>>1)
#define DMA_DEALLOCDOMAINID0_WR(src)             (((u32)(src)<<1) & 0x00000002)
#define DMA_DEALLOCDOMAINID0_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields CMDomainID	 */
#define DMA_CMDOMAINID0_WIDTH                                                 1
#define DMA_CMDOMAINID0_SHIFT                                                 0
#define DMA_CMDOMAINID0_MASK                                         0x00000001
#define DMA_CMDOMAINID0_RD(src)                          (((src) & 0x00000001))
#define DMA_CMDOMAINID0_WR(src)                     (((u32)(src)) & 0x00000001)
#define DMA_CMDOMAINID0_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register CfgSsQMIDomVal1	*/ 
/*	 Fields FPDomainID	 */
#define DMA_FPDOMAINID1_WIDTH                                                18
#define DMA_FPDOMAINID1_SHIFT                                                 3
#define DMA_FPDOMAINID1_MASK                                         0x001ffff8
#define DMA_FPDOMAINID1_RD(src)                       (((src) & 0x001ffff8)>>3)
#define DMA_FPDOMAINID1_WR(src)                  (((u32)(src)<<3) & 0x001ffff8)
#define DMA_FPDOMAINID1_SET(dst,src) \
                       (((dst) & ~0x001ffff8) | (((u32)(src)<<3) & 0x001ffff8))
/*	 Fields DecDomainID	 */
#define DMA_DECDOMAINID1_WIDTH                                                1
#define DMA_DECDOMAINID1_SHIFT                                                2
#define DMA_DECDOMAINID1_MASK                                        0x00000004
#define DMA_DECDOMAINID1_RD(src)                      (((src) & 0x00000004)>>2)
#define DMA_DECDOMAINID1_WR(src)                 (((u32)(src)<<2) & 0x00000004)
#define DMA_DECDOMAINID1_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields DeallocDomainID	 */
#define DMA_DEALLOCDOMAINID1_WIDTH                                            1
#define DMA_DEALLOCDOMAINID1_SHIFT                                            1
#define DMA_DEALLOCDOMAINID1_MASK                                    0x00000002
#define DMA_DEALLOCDOMAINID1_RD(src)                  (((src) & 0x00000002)>>1)
#define DMA_DEALLOCDOMAINID1_WR(src)             (((u32)(src)<<1) & 0x00000002)
#define DMA_DEALLOCDOMAINID1_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields CMDomainID	 */
#define DMA_CMDOMAINID1_WIDTH                                                 1
#define DMA_CMDOMAINID1_SHIFT                                                 0
#define DMA_CMDOMAINID1_MASK                                         0x00000001
#define DMA_CMDOMAINID1_RD(src)                          (((src) & 0x00000001))
#define DMA_CMDOMAINID1_WR(src)                     (((u32)(src)) & 0x00000001)
#define DMA_CMDOMAINID1_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register CfgSsQMIDomVal2	*/ 
/*	 Fields FPDomainID	 */
#define DMA_FPDOMAINID2_WIDTH                                                18
#define DMA_FPDOMAINID2_SHIFT                                                 3
#define DMA_FPDOMAINID2_MASK                                         0x001ffff8
#define DMA_FPDOMAINID2_RD(src)                       (((src) & 0x001ffff8)>>3)
#define DMA_FPDOMAINID2_WR(src)                  (((u32)(src)<<3) & 0x001ffff8)
#define DMA_FPDOMAINID2_SET(dst,src) \
                       (((dst) & ~0x001ffff8) | (((u32)(src)<<3) & 0x001ffff8))
/*	 Fields DecDomainID	 */
#define DMA_DECDOMAINID2_WIDTH                                                1
#define DMA_DECDOMAINID2_SHIFT                                                2
#define DMA_DECDOMAINID2_MASK                                        0x00000004
#define DMA_DECDOMAINID2_RD(src)                      (((src) & 0x00000004)>>2)
#define DMA_DECDOMAINID2_WR(src)                 (((u32)(src)<<2) & 0x00000004)
#define DMA_DECDOMAINID2_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields DeallocDomainID	 */
#define DMA_DEALLOCDOMAINID2_WIDTH                                            1
#define DMA_DEALLOCDOMAINID2_SHIFT                                            1
#define DMA_DEALLOCDOMAINID2_MASK                                    0x00000002
#define DMA_DEALLOCDOMAINID2_RD(src)                  (((src) & 0x00000002)>>1)
#define DMA_DEALLOCDOMAINID2_WR(src)             (((u32)(src)<<1) & 0x00000002)
#define DMA_DEALLOCDOMAINID2_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields CMDomainID	 */
#define DMA_CMDOMAINID2_WIDTH                                                 1
#define DMA_CMDOMAINID2_SHIFT                                                 0
#define DMA_CMDOMAINID2_MASK                                         0x00000001
#define DMA_CMDOMAINID2_RD(src)                          (((src) & 0x00000001))
#define DMA_CMDOMAINID2_WR(src)                     (((u32)(src)) & 0x00000001)
#define DMA_CMDOMAINID2_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Global Base Address	*/
#define DMA_GLBL_DIAG_CSR_BASE_ADDR			0xddd827000ULL

/*    Address GLBL_DIAG_CSR  Registers */
#define DMA_CFG_DIAG_SEL_ADDR                                        0x00000000
#define DMA_CFG_DIAG_SEL_DEFAULT                                     0x00000000
#define DMA_CFG_READ_BW_LAT_ADDR_MASK_ADDR                           0x00000004
#define DMA_CFG_READ_BW_LAT_ADDR_MASK_DEFAULT                        0x00000000
#define DMA_CFG_READ_BW_LAT_ADDR_PAT_ADDR                            0x00000008
#define DMA_CFG_READ_BW_LAT_ADDR_PAT_DEFAULT                         0xffffffff
#define DMA_CFG_WRITE_BW_LAT_ADDR_MASK_ADDR                          0x0000000c
#define DMA_CFG_WRITE_BW_LAT_ADDR_MASK_DEFAULT                       0x00000000
#define DMA_CFG_WRITE_BW_LAT_ADDR_PAT_ADDR                           0x00000010
#define DMA_CFG_WRITE_BW_LAT_ADDR_PAT_DEFAULT                        0xffffffff
#define DMA_CFG_DIAG_START_STOP_ADDR                                 0x00000014
#define DMA_CFG_DIAG_START_STOP_DEFAULT                              0x0000001f
#define DMA_CFG_BW_MSTR_STOP_CNT_ADDR                                0x00000018
#define DMA_CFG_BW_MSTR_STOP_CNT_DEFAULT                             0x00040004
#define DMA_CFG_BW_SLV_STOP_CNT_ADDR                                 0x0000001c
#define DMA_CFG_BW_SLV_STOP_CNT_DEFAULT                              0x00040004
#define DMA_STS_READ_LATENCY_OUTPUT_ADDR                             0x00000020
#define DMA_STS_READ_LATENCY_OUTPUT_DEFAULT                          0x00000000
#define DMA_STS_AXI_MRD_BW_CLK_CNT_ADDR                              0x00000024
#define DMA_STS_AXI_MRD_BW_CLK_CNT_DEFAULT                           0x00000000
#define DMA_STS_AXI_MRD_BW_BYTE_CNT_ADDR                             0x00000028
#define DMA_STS_AXI_MRD_BW_BYTE_CNT_DEFAULT                          0x00000000
#define DMA_STS_AXI_MWR_BW_CLK_CNT_ADDR                              0x0000002c
#define DMA_STS_AXI_MWR_BW_CLK_CNT_DEFAULT                           0x00000000
#define DMA_STS_AXI_MWR_BW_BYTE_CNT_ADDR                             0x00000030
#define DMA_STS_AXI_MWR_BW_BYTE_CNT_DEFAULT                          0x00000000
#define DMA_STS_AXI_SRD_BW_CLK_CNT_ADDR                              0x00000034
#define DMA_STS_AXI_SRD_BW_CLK_CNT_DEFAULT                           0x00000000
#define DMA_STS_AXI_SRD_BW_BYTE_CNT_ADDR                             0x00000038
#define DMA_STS_AXI_SRD_BW_BYTE_CNT_DEFAULT                          0x00000000
#define DMA_STS_AXI_SWR_BW_CLK_CNT_ADDR                              0x0000003c
#define DMA_STS_AXI_SWR_BW_CLK_CNT_DEFAULT                           0x00000000
#define DMA_STS_AXI_SWR_BW_BYTE_CNT_ADDR                             0x00000040
#define DMA_STS_AXI_SWR_BW_BYTE_CNT_DEFAULT                          0x00000000
#define DMA_CFG_DBG_TRIG_CTRL_ADDR                                   0x00000044
#define DMA_CFG_DBG_TRIG_CTRL_DEFAULT                                0x00000000
#define DMA_CFG_DBG_PAT_REG_0_ADDR                                   0x00000048
#define DMA_CFG_DBG_PAT_REG_0_DEFAULT                                0x00000000
#define DMA_CFG_DBG_PAT_MASK_REG_0_ADDR                              0x0000004c
#define DMA_CFG_DBG_PAT_MASK_REG_0_DEFAULT                           0x00000000
#define DMA_CFG_DBG_PAT_REG_1_ADDR                                   0x00000050
#define DMA_CFG_DBG_PAT_REG_1_DEFAULT                                0x00000000
#define DMA_CFG_DBG_PAT_MASK_REG_1_ADDR                              0x00000054
#define DMA_CFG_DBG_PAT_MASK_REG_1_DEFAULT                           0x00000000
#define DMA_DBG_TRIG_OUT_ADDR                                        0x00000058
#define DMA_DBG_TRIG_OUT_DEFAULT                                     0x00000000
#define DMA_DBG_ECC_INT_OUT_ADDR                                     0x0000005c
#define DMA_DBG_ECC_INT_OUT_DEFAULT                                  0x00000000
#define DMA_DBG_ECC_INT_OUTMASK_ADDR                                 0x00000060
#define DMA_CFG_SECC_INT_THRSH_REG_ADDR                              0x00000064
#define DMA_CFG_SECC_INT_THRSH_REG_DEFAULT                           0x000000fe
#define DMA_CFG_MEM_ECC_BYPASS_ADDR                                  0x00000068
#define DMA_CFG_MEM_ECC_BYPASS_DEFAULT                               0x00000000
#define DMA_CFG_MEM_PWRDN_DIS_ADDR                                   0x0000006c
#define DMA_CFG_MEM_PWRDN_DIS_DEFAULT                                0x00000000
#define DMA_CFG_MEM_RAM_SHUTDOWN_ADDR                                0x00000070
#define DMA_CFG_MEM_RAM_SHUTDOWN_DEFAULT                             0xffffffff
#define DMA_BLOCK_MEM_RDY_ADDR                                       0x00000074
#define DMA_BLOCK_MEM_RDY_DEFAULT                                    0xffffffff
#define DMA_CFG_AXI_SLV_RD_TMO_ADDR                                  0x00000078
#define DMA_CFG_AXI_SLV_RD_TMO_DEFAULT                               0x83ff83ff
#define DMA_CFG_AXI_SLV_WR_TMO_ADDR                                  0x0000007c
#define DMA_CFG_AXI_SLV_WR_TMO_DEFAULT                               0x83ff83ff
#define DMA_STS_AXI_SLV_TMO_ADDR                                     0x00000080
#define DMA_STS_AXI_SLV_TMO_DEFAULT                                  0x00000000
#define DMA_CFG_AXI_SS_CSR_TMO_ADDR                                  0x00000084
#define DMA_CFG_AXI_SS_CSR_TMO_DEFAULT                               0x02008000
#define DMA_STS_READ_LATENCY_TOT_READ_REQS_ADDR                      0x0000008c
#define DMA_STS_READ_LATENCY_TOT_READ_REQS_DEFAULT                   0x00000000

/*	Register CFG_DIAG_SEL	*/ 
/*	 Fields CFG_AXI_NON_AXI_MUX_SELECT	 */
#define DMA_CFG_AXI_NON_AXI_MUX_SELECT_WIDTH                                  1
#define DMA_CFG_AXI_NON_AXI_MUX_SELECT_SHIFT                                 11
#define DMA_CFG_AXI_NON_AXI_MUX_SELECT_MASK                          0x00000800
#define DMA_CFG_AXI_NON_AXI_MUX_SELECT_RD(src)       (((src) & 0x00000800)>>11)
#define DMA_CFG_AXI_NON_AXI_MUX_SELECT_WR(src)  (((u32)(src)<<11) & 0x00000800)
#define DMA_CFG_AXI_NON_AXI_MUX_SELECT_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*	 Fields CFG_MUX_SELECTOR	 */
#define DMA_CFG_MUX_SELECTOR_WIDTH                                           11
#define DMA_CFG_MUX_SELECTOR_SHIFT                                            0
#define DMA_CFG_MUX_SELECTOR_MASK                                    0x000007ff
#define DMA_CFG_MUX_SELECTOR_RD(src)                     (((src) & 0x000007ff))
#define DMA_CFG_MUX_SELECTOR_WR(src)                (((u32)(src)) & 0x000007ff)
#define DMA_CFG_MUX_SELECTOR_SET(dst,src) \
                          (((dst) & ~0x000007ff) | (((u32)(src)) & 0x000007ff))

/*	Register CFG_READ_BW_LAT_ADDR_MASK	*/ 
/*	 Fields READ_ADDR_MASK	 */
#define DMA_READ_ADDR_MASK_WIDTH                                             32
#define DMA_READ_ADDR_MASK_SHIFT                                              0
#define DMA_READ_ADDR_MASK_MASK                                      0xffffffff
#define DMA_READ_ADDR_MASK_RD(src)                       (((src) & 0xffffffff))
#define DMA_READ_ADDR_MASK_WR(src)                  (((u32)(src)) & 0xffffffff)
#define DMA_READ_ADDR_MASK_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_READ_BW_LAT_ADDR_PAT	*/ 
/*	 Fields READ_ADDR_PAT	 */
#define DMA_READ_ADDR_PAT_WIDTH                                              32
#define DMA_READ_ADDR_PAT_SHIFT                                               0
#define DMA_READ_ADDR_PAT_MASK                                       0xffffffff
#define DMA_READ_ADDR_PAT_RD(src)                        (((src) & 0xffffffff))
#define DMA_READ_ADDR_PAT_WR(src)                   (((u32)(src)) & 0xffffffff)
#define DMA_READ_ADDR_PAT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_WRITE_BW_LAT_ADDR_MASK	*/ 
/*	 Fields WRITE_ADDR_MASK	 */
#define DMA_WRITE_ADDR_MASK_WIDTH                                            32
#define DMA_WRITE_ADDR_MASK_SHIFT                                             0
#define DMA_WRITE_ADDR_MASK_MASK                                     0xffffffff
#define DMA_WRITE_ADDR_MASK_RD(src)                      (((src) & 0xffffffff))
#define DMA_WRITE_ADDR_MASK_WR(src)                 (((u32)(src)) & 0xffffffff)
#define DMA_WRITE_ADDR_MASK_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_WRITE_BW_LAT_ADDR_PAT	*/ 
/*	 Fields WRITE_ADDR_PAT	 */
#define DMA_WRITE_ADDR_PAT_WIDTH                                             32
#define DMA_WRITE_ADDR_PAT_SHIFT                                              0
#define DMA_WRITE_ADDR_PAT_MASK                                      0xffffffff
#define DMA_WRITE_ADDR_PAT_RD(src)                       (((src) & 0xffffffff))
#define DMA_WRITE_ADDR_PAT_WR(src)                  (((u32)(src)) & 0xffffffff)
#define DMA_WRITE_ADDR_PAT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_DIAG_START_STOP	*/ 
/*	 Fields START_RD_LATENCY	 */
#define DMA_START_RD_LATENCY_WIDTH                                            1
#define DMA_START_RD_LATENCY_SHIFT                                            4
#define DMA_START_RD_LATENCY_MASK                                    0x00000010
#define DMA_START_RD_LATENCY_RD(src)                  (((src) & 0x00000010)>>4)
#define DMA_START_RD_LATENCY_WR(src)             (((u32)(src)<<4) & 0x00000010)
#define DMA_START_RD_LATENCY_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields START_SRD_BW	 */
#define DMA_START_SRD_BW_WIDTH                                                1
#define DMA_START_SRD_BW_SHIFT                                                3
#define DMA_START_SRD_BW_MASK                                        0x00000008
#define DMA_START_SRD_BW_RD(src)                      (((src) & 0x00000008)>>3)
#define DMA_START_SRD_BW_WR(src)                 (((u32)(src)<<3) & 0x00000008)
#define DMA_START_SRD_BW_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields START_MRD_BW	 */
#define DMA_START_MRD_BW_WIDTH                                                1
#define DMA_START_MRD_BW_SHIFT                                                2
#define DMA_START_MRD_BW_MASK                                        0x00000004
#define DMA_START_MRD_BW_RD(src)                      (((src) & 0x00000004)>>2)
#define DMA_START_MRD_BW_WR(src)                 (((u32)(src)<<2) & 0x00000004)
#define DMA_START_MRD_BW_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields START_SWR_BW	 */
#define DMA_START_SWR_BW_WIDTH                                                1
#define DMA_START_SWR_BW_SHIFT                                                1
#define DMA_START_SWR_BW_MASK                                        0x00000002
#define DMA_START_SWR_BW_RD(src)                      (((src) & 0x00000002)>>1)
#define DMA_START_SWR_BW_WR(src)                 (((u32)(src)<<1) & 0x00000002)
#define DMA_START_SWR_BW_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields START_MWR_BW	 */
#define DMA_START_MWR_BW_WIDTH                                                1
#define DMA_START_MWR_BW_SHIFT                                                0
#define DMA_START_MWR_BW_MASK                                        0x00000001
#define DMA_START_MWR_BW_RD(src)                         (((src) & 0x00000001))
#define DMA_START_MWR_BW_WR(src)                    (((u32)(src)) & 0x00000001)
#define DMA_START_MWR_BW_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register CFG_BW_MSTR_STOP_CNT	*/ 
/*	 Fields MSTR_STOP_RD_CNT	 */
#define DMA_MSTR_STOP_RD_CNT_WIDTH                                           16
#define DMA_MSTR_STOP_RD_CNT_SHIFT                                           16
#define DMA_MSTR_STOP_RD_CNT_MASK                                    0xffff0000
#define DMA_MSTR_STOP_RD_CNT_RD(src)                 (((src) & 0xffff0000)>>16)
#define DMA_MSTR_STOP_RD_CNT_WR(src)            (((u32)(src)<<16) & 0xffff0000)
#define DMA_MSTR_STOP_RD_CNT_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields MSTR_STOP_WR_CNT	 */
#define DMA_MSTR_STOP_WR_CNT_WIDTH                                           16
#define DMA_MSTR_STOP_WR_CNT_SHIFT                                            0
#define DMA_MSTR_STOP_WR_CNT_MASK                                    0x0000ffff
#define DMA_MSTR_STOP_WR_CNT_RD(src)                     (((src) & 0x0000ffff))
#define DMA_MSTR_STOP_WR_CNT_WR(src)                (((u32)(src)) & 0x0000ffff)
#define DMA_MSTR_STOP_WR_CNT_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register CFG_BW_SLV_STOP_CNT	*/ 
/*	 Fields SLV_STOP_RD_CNT	 */
#define DMA_SLV_STOP_RD_CNT_WIDTH                                            16
#define DMA_SLV_STOP_RD_CNT_SHIFT                                            16
#define DMA_SLV_STOP_RD_CNT_MASK                                     0xffff0000
#define DMA_SLV_STOP_RD_CNT_RD(src)                  (((src) & 0xffff0000)>>16)
#define DMA_SLV_STOP_RD_CNT_WR(src)             (((u32)(src)<<16) & 0xffff0000)
#define DMA_SLV_STOP_RD_CNT_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields SLV_STOP_WR_CNT	 */
#define DMA_SLV_STOP_WR_CNT_WIDTH                                            16
#define DMA_SLV_STOP_WR_CNT_SHIFT                                             0
#define DMA_SLV_STOP_WR_CNT_MASK                                     0x0000ffff
#define DMA_SLV_STOP_WR_CNT_RD(src)                      (((src) & 0x0000ffff))
#define DMA_SLV_STOP_WR_CNT_WR(src)                 (((u32)(src)) & 0x0000ffff)
#define DMA_SLV_STOP_WR_CNT_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register STS_READ_LATENCY_OUTPUT	*/ 
/*	 Fields READ_MAX	 */
#define DMA_READ_MAX_WIDTH                                                   10
#define DMA_READ_MAX_SHIFT                                                   22
#define DMA_READ_MAX_MASK                                            0xffc00000
#define DMA_READ_MAX_RD(src)                         (((src) & 0xffc00000)>>22)
#define DMA_READ_MAX_SET(dst,src) \
                      (((dst) & ~0xffc00000) | (((u32)(src)<<22) & 0xffc00000))
/*	 Fields READ_TOT	 */
#define DMA_READ_TOT_WIDTH                                                   22
#define DMA_READ_TOT_SHIFT                                                    0
#define DMA_READ_TOT_MASK                                            0x003fffff
#define DMA_READ_TOT_RD(src)                             (((src) & 0x003fffff))
#define DMA_READ_TOT_SET(dst,src) \
                          (((dst) & ~0x003fffff) | (((u32)(src)) & 0x003fffff))

/*	Register STS_AXI_MRD_BW_CLK_CNT	*/ 
/*	 Fields MSTR_READ_BW_CLK_CNT	 */
#define DMA_MSTR_READ_BW_CLK_CNT_WIDTH                                       32
#define DMA_MSTR_READ_BW_CLK_CNT_SHIFT                                        0
#define DMA_MSTR_READ_BW_CLK_CNT_MASK                                0xffffffff
#define DMA_MSTR_READ_BW_CLK_CNT_RD(src)                 (((src) & 0xffffffff))
#define DMA_MSTR_READ_BW_CLK_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register STS_AXI_MRD_BW_BYTE_CNT	*/ 
/*	 Fields MSTR_READ_BW_BYTE_CNT	 */
#define DMA_MSTR_READ_BW_BYTE_CNT_WIDTH                                      32
#define DMA_MSTR_READ_BW_BYTE_CNT_SHIFT                                       0
#define DMA_MSTR_READ_BW_BYTE_CNT_MASK                               0xffffffff
#define DMA_MSTR_READ_BW_BYTE_CNT_RD(src)                (((src) & 0xffffffff))
#define DMA_MSTR_READ_BW_BYTE_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register STS_AXI_MWR_BW_CLK_CNT	*/ 
/*	 Fields MSTR_WRITE_BW_CLK_CNT	 */
#define DMA_MSTR_WRITE_BW_CLK_CNT_WIDTH                                      32
#define DMA_MSTR_WRITE_BW_CLK_CNT_SHIFT                                       0
#define DMA_MSTR_WRITE_BW_CLK_CNT_MASK                               0xffffffff
#define DMA_MSTR_WRITE_BW_CLK_CNT_RD(src)                (((src) & 0xffffffff))
#define DMA_MSTR_WRITE_BW_CLK_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register STS_AXI_MWR_BW_BYTE_CNT	*/ 
/*	 Fields MSTR_WRITE_BW_BYTE_CNT	 */
#define DMA_MSTR_WRITE_BW_BYTE_CNT_WIDTH                                     32
#define DMA_MSTR_WRITE_BW_BYTE_CNT_SHIFT                                      0
#define DMA_MSTR_WRITE_BW_BYTE_CNT_MASK                              0xffffffff
#define DMA_MSTR_WRITE_BW_BYTE_CNT_RD(src)               (((src) & 0xffffffff))
#define DMA_MSTR_WRITE_BW_BYTE_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register STS_AXI_SRD_BW_CLK_CNT	*/ 
/*	 Fields SLV_READ_BW_CLK_CNT	 */
#define DMA_SLV_READ_BW_CLK_CNT_WIDTH                                        32
#define DMA_SLV_READ_BW_CLK_CNT_SHIFT                                         0
#define DMA_SLV_READ_BW_CLK_CNT_MASK                                 0xffffffff
#define DMA_SLV_READ_BW_CLK_CNT_RD(src)                  (((src) & 0xffffffff))
#define DMA_SLV_READ_BW_CLK_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register STS_AXI_SRD_BW_BYTE_CNT	*/ 
/*	 Fields SLV_READ_BW_BYTE_CNT	 */
#define DMA_SLV_READ_BW_BYTE_CNT_WIDTH                                       32
#define DMA_SLV_READ_BW_BYTE_CNT_SHIFT                                        0
#define DMA_SLV_READ_BW_BYTE_CNT_MASK                                0xffffffff
#define DMA_SLV_READ_BW_BYTE_CNT_RD(src)                 (((src) & 0xffffffff))
#define DMA_SLV_READ_BW_BYTE_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register STS_AXI_SWR_BW_CLK_CNT	*/ 
/*	 Fields SLV_WRITE_BW_CLK_CNT	 */
#define DMA_SLV_WRITE_BW_CLK_CNT_WIDTH                                       32
#define DMA_SLV_WRITE_BW_CLK_CNT_SHIFT                                        0
#define DMA_SLV_WRITE_BW_CLK_CNT_MASK                                0xffffffff
#define DMA_SLV_WRITE_BW_CLK_CNT_RD(src)                 (((src) & 0xffffffff))
#define DMA_SLV_WRITE_BW_CLK_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register STS_AXI_SWR_BW_BYTE_CNT	*/ 
/*	 Fields SLV_WRITE_BW_BYTE_CNT	 */
#define DMA_SLV_WRITE_BW_BYTE_CNT_WIDTH                                      32
#define DMA_SLV_WRITE_BW_BYTE_CNT_SHIFT                                       0
#define DMA_SLV_WRITE_BW_BYTE_CNT_MASK                               0xffffffff
#define DMA_SLV_WRITE_BW_BYTE_CNT_RD(src)                (((src) & 0xffffffff))
#define DMA_SLV_WRITE_BW_BYTE_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_DBG_TRIG_CTRL	*/ 
/*	 Fields TRIG_EN	 */
#define DMA_TRIG_EN_WIDTH                                                     1
#define DMA_TRIG_EN_SHIFT                                                     4
#define DMA_TRIG_EN_MASK                                             0x00000010
#define DMA_TRIG_EN_RD(src)                           (((src) & 0x00000010)>>4)
#define DMA_TRIG_EN_WR(src)                      (((u32)(src)<<4) & 0x00000010)
#define DMA_TRIG_EN_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields AND_E	 */
#define DMA_AND_E_WIDTH                                                       2
#define DMA_AND_E_SHIFT                                                       2
#define DMA_AND_E_MASK                                               0x0000000c
#define DMA_AND_E_RD(src)                             (((src) & 0x0000000c)>>2)
#define DMA_AND_E_WR(src)                        (((u32)(src)<<2) & 0x0000000c)
#define DMA_AND_E_SET(dst,src) \
                       (((dst) & ~0x0000000c) | (((u32)(src)<<2) & 0x0000000c))
/*	 Fields OR_E	 */
#define DMA_OR_E_WIDTH                                                        2
#define DMA_OR_E_SHIFT                                                        0
#define DMA_OR_E_MASK                                                0x00000003
#define DMA_OR_E_RD(src)                                 (((src) & 0x00000003))
#define DMA_OR_E_WR(src)                            (((u32)(src)) & 0x00000003)
#define DMA_OR_E_SET(dst,src) \
                          (((dst) & ~0x00000003) | (((u32)(src)) & 0x00000003))

/*	Register CFG_DBG_PAT_REG_0	*/ 
/*	 Fields PATTERN	 */
#define DMA_PATTERN0_WIDTH                                                   32
#define DMA_PATTERN0_SHIFT                                                    0
#define DMA_PATTERN0_MASK                                            0xffffffff
#define DMA_PATTERN0_RD(src)                             (((src) & 0xffffffff))
#define DMA_PATTERN0_WR(src)                        (((u32)(src)) & 0xffffffff)
#define DMA_PATTERN0_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_DBG_PAT_MASK_REG_0	*/ 
/*	 Fields PAT_MASK	 */
#define DMA_PAT_MASK0_WIDTH                                                  32
#define DMA_PAT_MASK0_SHIFT                                                   0
#define DMA_PAT_MASK0_MASK                                           0xffffffff
#define DMA_PAT_MASK0_RD(src)                            (((src) & 0xffffffff))
#define DMA_PAT_MASK0_WR(src)                       (((u32)(src)) & 0xffffffff)
#define DMA_PAT_MASK0_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_DBG_PAT_REG_1	*/ 
/*	 Fields PATTERN	 */
#define DMA_PATTERN1_WIDTH                                                   32
#define DMA_PATTERN1_SHIFT                                                    0
#define DMA_PATTERN1_MASK                                            0xffffffff
#define DMA_PATTERN1_RD(src)                             (((src) & 0xffffffff))
#define DMA_PATTERN1_WR(src)                        (((u32)(src)) & 0xffffffff)
#define DMA_PATTERN1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_DBG_PAT_MASK_REG_1	*/ 
/*	 Fields PAT_MASK	 */
#define DMA_PAT_MASK1_WIDTH                                                  32
#define DMA_PAT_MASK1_SHIFT                                                   0
#define DMA_PAT_MASK1_MASK                                           0xffffffff
#define DMA_PAT_MASK1_RD(src)                            (((src) & 0xffffffff))
#define DMA_PAT_MASK1_WR(src)                       (((u32)(src)) & 0xffffffff)
#define DMA_PAT_MASK1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register DBG_TRIG_OUT	*/ 
/*	 Fields DBG_OUT	 */
#define DMA_DBG_OUT_WIDTH                                                    32
#define DMA_DBG_OUT_SHIFT                                                     0
#define DMA_DBG_OUT_MASK                                             0xffffffff
#define DMA_DBG_OUT_RD(src)                              (((src) & 0xffffffff))
#define DMA_DBG_OUT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register DBG_ECC_INT_OUT	*/ 
/*	 Fields SLV_TMO	 */
#define DMA_SLV_TMO_WIDTH                                                     1
#define DMA_SLV_TMO_SHIFT                                                     3
#define DMA_SLV_TMO_MASK                                             0x00000008
#define DMA_SLV_TMO_RD(src)                           (((src) & 0x00000008)>>3)
#define DMA_SLV_TMO_WR(src)                      (((u32)(src)<<3) & 0x00000008)
#define DMA_SLV_TMO_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields SECC_INT	 */
#define DMA_SECC_INT_WIDTH                                                    1
#define DMA_SECC_INT_SHIFT                                                    2
#define DMA_SECC_INT_MASK                                            0x00000004
#define DMA_SECC_INT_RD(src)                          (((src) & 0x00000004)>>2)
#define DMA_SECC_INT_WR(src)                     (((u32)(src)<<2) & 0x00000004)
#define DMA_SECC_INT_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields DECC_INT	 */
#define DMA_DECC_INT_WIDTH                                                    1
#define DMA_DECC_INT_SHIFT                                                    1
#define DMA_DECC_INT_MASK                                            0x00000002
#define DMA_DECC_INT_RD(src)                          (((src) & 0x00000002)>>1)
#define DMA_DECC_INT_WR(src)                     (((u32)(src)<<1) & 0x00000002)
#define DMA_DECC_INT_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields DBG_INT	 */
#define DMA_DBG_INT_WIDTH                                                     1
#define DMA_DBG_INT_SHIFT                                                     0
#define DMA_DBG_INT_MASK                                             0x00000001
#define DMA_DBG_INT_RD(src)                              (((src) & 0x00000001))
#define DMA_DBG_INT_WR(src)                         (((u32)(src)) & 0x00000001)
#define DMA_DBG_INT_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register DBG_ECC_INT_OUTMask	*/
/*    Mask Register Fields SLV_TMOMask    */
#define DMA_SLV_TMOMASK_WIDTH                                                 1
#define DMA_SLV_TMOMASK_SHIFT                                                 3
#define DMA_SLV_TMOMASK_MASK                                         0x00000008
#define DMA_SLV_TMOMASK_RD(src)                       (((src) & 0x00000008)>>3)
#define DMA_SLV_TMOMASK_WR(src)                  (((u32)(src)<<3) & 0x00000008)
#define DMA_SLV_TMOMASK_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*    Mask Register Fields SECC_INTMask    */
#define DMA_SECC_INTMASK_WIDTH                                                1
#define DMA_SECC_INTMASK_SHIFT                                                2
#define DMA_SECC_INTMASK_MASK                                        0x00000004
#define DMA_SECC_INTMASK_RD(src)                      (((src) & 0x00000004)>>2)
#define DMA_SECC_INTMASK_WR(src)                 (((u32)(src)<<2) & 0x00000004)
#define DMA_SECC_INTMASK_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*    Mask Register Fields DECC_INTMask    */
#define DMA_DECC_INTMASK_WIDTH                                                1
#define DMA_DECC_INTMASK_SHIFT                                                1
#define DMA_DECC_INTMASK_MASK                                        0x00000002
#define DMA_DECC_INTMASK_RD(src)                      (((src) & 0x00000002)>>1)
#define DMA_DECC_INTMASK_WR(src)                 (((u32)(src)<<1) & 0x00000002)
#define DMA_DECC_INTMASK_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*    Mask Register Fields DBG_INTMask    */
#define DMA_DBG_INTMASK_WIDTH                                                 1
#define DMA_DBG_INTMASK_SHIFT                                                 0
#define DMA_DBG_INTMASK_MASK                                         0x00000001
#define DMA_DBG_INTMASK_RD(src)                          (((src) & 0x00000001))
#define DMA_DBG_INTMASK_WR(src)                     (((u32)(src)) & 0x00000001)
#define DMA_DBG_INTMASK_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register CFG_SECC_INT_THRSH_REG	*/ 
/*	 Fields SECC_INT_THRSH	 */
#define DMA_SECC_INT_THRSH_WIDTH                                              8
#define DMA_SECC_INT_THRSH_SHIFT                                              0
#define DMA_SECC_INT_THRSH_MASK                                      0x000000ff
#define DMA_SECC_INT_THRSH_RD(src)                       (((src) & 0x000000ff))
#define DMA_SECC_INT_THRSH_WR(src)                  (((u32)(src)) & 0x000000ff)
#define DMA_SECC_INT_THRSH_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register CFG_MEM_ECC_BYPASS	*/ 
/*	 Fields CFG_ECC_BYPASS	 */
#define DMA_CFG_ECC_BYPASS_WIDTH                                             16
#define DMA_CFG_ECC_BYPASS_SHIFT                                              0
#define DMA_CFG_ECC_BYPASS_MASK                                      0x0000ffff
#define DMA_CFG_ECC_BYPASS_RD(src)                       (((src) & 0x0000ffff))
#define DMA_CFG_ECC_BYPASS_WR(src)                  (((u32)(src)) & 0x0000ffff)
#define DMA_CFG_ECC_BYPASS_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register CFG_MEM_PWRDN_DIS	*/ 
/*	 Fields CFG_ECC_PWRDN_DIS	 */
#define DMA_CFG_ECC_PWRDN_DIS_WIDTH                                          16
#define DMA_CFG_ECC_PWRDN_DIS_SHIFT                                          16
#define DMA_CFG_ECC_PWRDN_DIS_MASK                                   0xffff0000
#define DMA_CFG_ECC_PWRDN_DIS_RD(src)                (((src) & 0xffff0000)>>16)
#define DMA_CFG_ECC_PWRDN_DIS_WR(src)           (((u32)(src)<<16) & 0xffff0000)
#define DMA_CFG_ECC_PWRDN_DIS_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields CFG_PWRDN_DIS	 */
#define DMA_CFG_PWRDN_DIS_WIDTH                                              16
#define DMA_CFG_PWRDN_DIS_SHIFT                                               0
#define DMA_CFG_PWRDN_DIS_MASK                                       0x0000ffff
#define DMA_CFG_PWRDN_DIS_RD(src)                        (((src) & 0x0000ffff))
#define DMA_CFG_PWRDN_DIS_WR(src)                   (((u32)(src)) & 0x0000ffff)
#define DMA_CFG_PWRDN_DIS_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register CFG_MEM_RAM_SHUTDOWN	*/ 
/*	 Fields CFG_RAM_SHUTDOWN_EN	 */
#define DMA_CFG_RAM_SHUTDOWN_EN_WIDTH                                        32
#define DMA_CFG_RAM_SHUTDOWN_EN_SHIFT                                         0
#define DMA_CFG_RAM_SHUTDOWN_EN_MASK                                 0xffffffff
#define DMA_CFG_RAM_SHUTDOWN_EN_RD(src)                  (((src) & 0xffffffff))
#define DMA_CFG_RAM_SHUTDOWN_EN_WR(src)             (((u32)(src)) & 0xffffffff)
#define DMA_CFG_RAM_SHUTDOWN_EN_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register BLOCK_MEM_RDY	*/ 
/*	 Fields MEM_RDY	 */
#define DMA_MEM_RDY_WIDTH                                                    32
#define DMA_MEM_RDY_SHIFT                                                     0
#define DMA_MEM_RDY_MASK                                             0xffffffff
#define DMA_MEM_RDY_RD(src)                              (((src) & 0xffffffff))
#define DMA_MEM_RDY_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_AXI_SLV_RD_TMO	*/ 
/*	 Fields SLV_ID_TMO_ENABLE	 */
#define DMA_SLV_ID_TMO_ENABLE_WIDTH                                           1
#define DMA_SLV_ID_TMO_ENABLE_SHIFT                                          31
#define DMA_SLV_ID_TMO_ENABLE_MASK                                   0x80000000
#define DMA_SLV_ID_TMO_ENABLE_RD(src)                (((src) & 0x80000000)>>31)
#define DMA_SLV_ID_TMO_ENABLE_WR(src)           (((u32)(src)<<31) & 0x80000000)
#define DMA_SLV_ID_TMO_ENABLE_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields SS_ID_TRK_ENABLE	 */
#define DMA_SS_ID_TRK_ENABLE_WIDTH                                            1
#define DMA_SS_ID_TRK_ENABLE_SHIFT                                           30
#define DMA_SS_ID_TRK_ENABLE_MASK                                    0x40000000
#define DMA_SS_ID_TRK_ENABLE_RD(src)                 (((src) & 0x40000000)>>30)
#define DMA_SS_ID_TRK_ENABLE_WR(src)            (((u32)(src)<<30) & 0x40000000)
#define DMA_SS_ID_TRK_ENABLE_SET(dst,src) \
                      (((dst) & ~0x40000000) | (((u32)(src)<<30) & 0x40000000))
/*	 Fields SLV_ID_TMO_VALUE	 */
#define DMA_SLV_ID_TMO_VALUE_WIDTH                                           10
#define DMA_SLV_ID_TMO_VALUE_SHIFT                                           16
#define DMA_SLV_ID_TMO_VALUE_MASK                                    0x03ff0000
#define DMA_SLV_ID_TMO_VALUE_RD(src)                 (((src) & 0x03ff0000)>>16)
#define DMA_SLV_ID_TMO_VALUE_WR(src)            (((u32)(src)<<16) & 0x03ff0000)
#define DMA_SLV_ID_TMO_VALUE_SET(dst,src) \
                      (((dst) & ~0x03ff0000) | (((u32)(src)<<16) & 0x03ff0000))
/*	 Fields SLV_RDY_TMO_ENABLE	 */
#define DMA_SLV_RDY_TMO_ENABLE_WIDTH                                          1
#define DMA_SLV_RDY_TMO_ENABLE_SHIFT                                         15
#define DMA_SLV_RDY_TMO_ENABLE_MASK                                  0x00008000
#define DMA_SLV_RDY_TMO_ENABLE_RD(src)               (((src) & 0x00008000)>>15)
#define DMA_SLV_RDY_TMO_ENABLE_WR(src)          (((u32)(src)<<15) & 0x00008000)
#define DMA_SLV_RDY_TMO_ENABLE_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))
/*	 Fields SLV_RDY_TMO_VALUE	 */
#define DMA_SLV_RDY_TMO_VALUE_WIDTH                                          10
#define DMA_SLV_RDY_TMO_VALUE_SHIFT                                           0
#define DMA_SLV_RDY_TMO_VALUE_MASK                                   0x000003ff
#define DMA_SLV_RDY_TMO_VALUE_RD(src)                    (((src) & 0x000003ff))
#define DMA_SLV_RDY_TMO_VALUE_WR(src)               (((u32)(src)) & 0x000003ff)
#define DMA_SLV_RDY_TMO_VALUE_SET(dst,src) \
                          (((dst) & ~0x000003ff) | (((u32)(src)) & 0x000003ff))

/*	Register CFG_AXI_SLV_WR_TMO	*/ 
/*	 Fields SLV_ID_TMO_ENABLE	 */
#define DMA_SLV_ID_TMO_ENABLE_F1_WIDTH                                        1
#define DMA_SLV_ID_TMO_ENABLE_F1_SHIFT                                       31
#define DMA_SLV_ID_TMO_ENABLE_F1_MASK                                0x80000000
#define DMA_SLV_ID_TMO_ENABLE_F1_RD(src)             (((src) & 0x80000000)>>31)
#define DMA_SLV_ID_TMO_ENABLE_F1_WR(src)        (((u32)(src)<<31) & 0x80000000)
#define DMA_SLV_ID_TMO_ENABLE_F1_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields SS_ID_TRK_ENABLE	 */
#define DMA_SS_ID_TRK_ENABLE_F1_WIDTH                                         1
#define DMA_SS_ID_TRK_ENABLE_F1_SHIFT                                        30
#define DMA_SS_ID_TRK_ENABLE_F1_MASK                                 0x40000000
#define DMA_SS_ID_TRK_ENABLE_F1_RD(src)              (((src) & 0x40000000)>>30)
#define DMA_SS_ID_TRK_ENABLE_F1_WR(src)         (((u32)(src)<<30) & 0x40000000)
#define DMA_SS_ID_TRK_ENABLE_F1_SET(dst,src) \
                      (((dst) & ~0x40000000) | (((u32)(src)<<30) & 0x40000000))
/*	 Fields SLV_ID_TMO_VALUE	 */
#define DMA_SLV_ID_TMO_VALUE_F1_WIDTH                                        10
#define DMA_SLV_ID_TMO_VALUE_F1_SHIFT                                        16
#define DMA_SLV_ID_TMO_VALUE_F1_MASK                                 0x03ff0000
#define DMA_SLV_ID_TMO_VALUE_F1_RD(src)              (((src) & 0x03ff0000)>>16)
#define DMA_SLV_ID_TMO_VALUE_F1_WR(src)         (((u32)(src)<<16) & 0x03ff0000)
#define DMA_SLV_ID_TMO_VALUE_F1_SET(dst,src) \
                      (((dst) & ~0x03ff0000) | (((u32)(src)<<16) & 0x03ff0000))
/*	 Fields SLV_RDY_TMO_ENABLE	 */
#define DMA_SLV_RDY_TMO_ENABLE_F1_WIDTH                                       1
#define DMA_SLV_RDY_TMO_ENABLE_F1_SHIFT                                      15
#define DMA_SLV_RDY_TMO_ENABLE_F1_MASK                               0x00008000
#define DMA_SLV_RDY_TMO_ENABLE_F1_RD(src)            (((src) & 0x00008000)>>15)
#define DMA_SLV_RDY_TMO_ENABLE_F1_WR(src)       (((u32)(src)<<15) & 0x00008000)
#define DMA_SLV_RDY_TMO_ENABLE_F1_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))
/*	 Fields SLV_RDY_TMO_VALUE	 */
#define DMA_SLV_RDY_TMO_VALUE_F1_WIDTH                                       10
#define DMA_SLV_RDY_TMO_VALUE_F1_SHIFT                                        0
#define DMA_SLV_RDY_TMO_VALUE_F1_MASK                                0x000003ff
#define DMA_SLV_RDY_TMO_VALUE_F1_RD(src)                 (((src) & 0x000003ff))
#define DMA_SLV_RDY_TMO_VALUE_F1_WR(src)            (((u32)(src)) & 0x000003ff)
#define DMA_SLV_RDY_TMO_VALUE_F1_SET(dst,src) \
                          (((dst) & ~0x000003ff) | (((u32)(src)) & 0x000003ff))

/*	Register STS_AXI_SLV_TMO	*/ 
/*	 Fields SLV_ARID_TMO	 */
#define DMA_SLV_ARID_TMO_WIDTH                                                1
#define DMA_SLV_ARID_TMO_SHIFT                                                3
#define DMA_SLV_ARID_TMO_MASK                                        0x00000008
#define DMA_SLV_ARID_TMO_RD(src)                      (((src) & 0x00000008)>>3)
#define DMA_SLV_ARID_TMO_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields SLV_ARRDY_TMO	 */
#define DMA_SLV_ARRDY_TMO_WIDTH                                               1
#define DMA_SLV_ARRDY_TMO_SHIFT                                               2
#define DMA_SLV_ARRDY_TMO_MASK                                       0x00000004
#define DMA_SLV_ARRDY_TMO_RD(src)                     (((src) & 0x00000004)>>2)
#define DMA_SLV_ARRDY_TMO_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields SLV_AWID_TMO	 */
#define DMA_SLV_AWID_TMO_WIDTH                                                1
#define DMA_SLV_AWID_TMO_SHIFT                                                1
#define DMA_SLV_AWID_TMO_MASK                                        0x00000002
#define DMA_SLV_AWID_TMO_RD(src)                      (((src) & 0x00000002)>>1)
#define DMA_SLV_AWID_TMO_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields SLV_AWRDY_TMO	 */
#define DMA_SLV_AWRDY_TMO_WIDTH                                               1
#define DMA_SLV_AWRDY_TMO_SHIFT                                               0
#define DMA_SLV_AWRDY_TMO_MASK                                       0x00000001
#define DMA_SLV_AWRDY_TMO_RD(src)                        (((src) & 0x00000001))
#define DMA_SLV_AWRDY_TMO_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register CFG_AXI_SS_CSR_TMO	*/ 
/*	 Fields CFG_CSR_TMO	 */
#define DMA_CFG_CSR_TMO_WIDTH                                                16
#define DMA_CFG_CSR_TMO_SHIFT                                                16
#define DMA_CFG_CSR_TMO_MASK                                         0xffff0000
#define DMA_CFG_CSR_TMO_RD(src)                      (((src) & 0xffff0000)>>16)
#define DMA_CFG_CSR_TMO_WR(src)                 (((u32)(src)<<16) & 0xffff0000)
#define DMA_CFG_CSR_TMO_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields CSR_ERR_RESP_EN	 */
#define DMA_CSR_ERR_RESP_EN_WIDTH                                             1
#define DMA_CSR_ERR_RESP_EN_SHIFT                                            15
#define DMA_CSR_ERR_RESP_EN_MASK                                     0x00008000
#define DMA_CSR_ERR_RESP_EN_RD(src)                  (((src) & 0x00008000)>>15)
#define DMA_CSR_ERR_RESP_EN_WR(src)             (((u32)(src)<<15) & 0x00008000)
#define DMA_CSR_ERR_RESP_EN_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))

/*	Register STS_READ_LATENCY_TOT_READ_REQS	*/ 
/*	 Fields TOT_READS	 */
#define DMA_TOT_READS_WIDTH                                                  16
#define DMA_TOT_READS_SHIFT                                                   0
#define DMA_TOT_READS_MASK                                           0x0000ffff
#define DMA_TOT_READS_RD(src)                            (((src) & 0x0000ffff))
#define DMA_TOT_READS_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))
	


#endif
