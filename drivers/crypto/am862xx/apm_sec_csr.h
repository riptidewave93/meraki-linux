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
#ifndef _APM_SEC_CSR_H__
#define _APM_SEC_CSR_H__

/*	Global Base Address	*/
#define SEC_GLBL_CTRL_CSR_BASE_ADDR			0xddd830000ULL

/*    Address SEC_GLBL_CTRL_CSR  Registers */
#define CSR_ID_ADDR                                                  0x00000000
#define CSR_ID_DEFAULT                                               0x00000073
#define CSR_AXI_RD_WRR_CFG_ADDR                                      0x00000004
#define CSR_AXI_RD_WRR_CFG_DEFAULT                                   0x88800000
#define CSR_AXI_WR_WRR_CFG_ADDR                                      0x00000008
#define CSR_AXI_WR_WRR_CFG_DEFAULT                                   0x88800000
#define CSR_GLB_SEC_INT_STS_ADDR                                     0x0000000c
#define CSR_GLB_SEC_INT_STS_DEFAULT                                  0x00000000
#define CSR_GLB_SEC_INT_STSMASK_ADDR                                 0x00000010
#define CSR_GLB_SEC_INT_TEST_ADDR                                    0x00000014
#define CSR_GLB_SEC_INT_TEST_DEFAULT                                 0x00000000

/*	Register csr_id	*/ 
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

/*	Register csr_axi_rd_wrr_cfg	*/ 
/*	 Fields qmi_rd	 */
#define QMI_RD_WIDTH                                                          4
#define QMI_RD_SHIFT                                                         28
#define QMI_RD_MASK                                                  0xf0000000
#define QMI_RD_RD(src)                               (((src) & 0xf0000000)>>28)
#define QMI_RD_WR(src)                          (((u32)(src)<<28) & 0xf0000000)
#define QMI_RD_SET(dst,src) \
                      (((dst) & ~0xf0000000) | (((u32)(src)<<28) & 0xf0000000))
/*	 Fields eip96_rd	 */
#define EIP96_RD_WIDTH                                                        4
#define EIP96_RD_SHIFT                                                       24
#define EIP96_RD_MASK                                                0x0f000000
#define EIP96_RD_RD(src)                             (((src) & 0x0f000000)>>24)
#define EIP96_RD_WR(src)                        (((u32)(src)<<24) & 0x0f000000)
#define EIP96_RD_SET(dst,src) \
                      (((dst) & ~0x0f000000) | (((u32)(src)<<24) & 0x0f000000))
/*	 Fields xts_rd	 */
#define XTS_RD_WIDTH                                                          4
#define XTS_RD_SHIFT                                                         20
#define XTS_RD_MASK                                                  0x00f00000
#define XTS_RD_RD(src)                               (((src) & 0x00f00000)>>20)
#define XTS_RD_WR(src)                          (((u32)(src)<<20) & 0x00f00000)
#define XTS_RD_SET(dst,src) \
                      (((dst) & ~0x00f00000) | (((u32)(src)<<20) & 0x00f00000))

/*	Register csr_axi_wr_wrr_cfg	*/ 
/*	 Fields qmi_wr	 */
#define QMI_WR_WIDTH                                                          4
#define QMI_WR_SHIFT                                                         28
#define QMI_WR_MASK                                                  0xf0000000
#define QMI_WR_RD(src)                               (((src) & 0xf0000000)>>28)
#define QMI_WR_WR(src)                          (((u32)(src)<<28) & 0xf0000000)
#define QMI_WR_SET(dst,src) \
                      (((dst) & ~0xf0000000) | (((u32)(src)<<28) & 0xf0000000))
/*	 Fields eip96_wr	 */
#define EIP96_WR_WIDTH                                                        4
#define EIP96_WR_SHIFT                                                       24
#define EIP96_WR_MASK                                                0x0f000000
#define EIP96_WR_RD(src)                             (((src) & 0x0f000000)>>24)
#define EIP96_WR_WR(src)                        (((u32)(src)<<24) & 0x0f000000)
#define EIP96_WR_SET(dst,src) \
                      (((dst) & ~0x0f000000) | (((u32)(src)<<24) & 0x0f000000))
/*	 Fields xts_wr	 */
#define XTS_WR_WIDTH                                                          4
#define XTS_WR_SHIFT                                                         20
#define XTS_WR_MASK                                                  0x00f00000
#define XTS_WR_RD(src)                               (((src) & 0x00f00000)>>20)
#define XTS_WR_WR(src)                          (((u32)(src)<<20) & 0x00f00000)
#define XTS_WR_SET(dst,src) \
                      (((dst) & ~0x00f00000) | (((u32)(src)<<20) & 0x00f00000))

/*	Register csr_glb_sec_int_sts	*/ 
/*	 Fields qmi	 */
#define QMI_WIDTH                                                             1
#define QMI_SHIFT                                                            31
#define QMI_MASK                                                     0x80000000
#define QMI_RD(src)                                  (((src) & 0x80000000)>>31)
#define QMI_WR(src)                             (((u32)(src)<<31) & 0x80000000)
#define QMI_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields eip96	 */
#define EIP96_WIDTH                                                           1
#define EIP96_SHIFT                                                          30
#define EIP96_MASK                                                   0x40000000
#define EIP96_RD(src)                                (((src) & 0x40000000)>>30)
#define EIP96_WR(src)                           (((u32)(src)<<30) & 0x40000000)
#define EIP96_SET(dst,src) \
                      (((dst) & ~0x40000000) | (((u32)(src)<<30) & 0x40000000))
/*	 Fields xts	 */
#define XTS_WIDTH                                                             1
#define XTS_SHIFT                                                            29
#define XTS_MASK                                                     0x20000000
#define XTS_RD(src)                                  (((src) & 0x20000000)>>29)
#define XTS_WR(src)                             (((u32)(src)<<29) & 0x20000000)
#define XTS_SET(dst,src) \
                      (((dst) & ~0x20000000) | (((u32)(src)<<29) & 0x20000000))

/*	Register csr_glb_sec_int_stsMask	*/
/*    Mask Register Fields qmiMask    */
#define QMIMASK_WIDTH                                                         1
#define QMIMASK_SHIFT                                                        31
#define QMIMASK_MASK                                                 0x80000000
#define QMIMASK_RD(src)                              (((src) & 0x80000000)>>31)
#define QMIMASK_WR(src)                         (((u32)(src)<<31) & 0x80000000)
#define QMIMASK_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*    Mask Register Fields eip96Mask    */
#define EIP96MASK_WIDTH                                                       1
#define EIP96MASK_SHIFT                                                      30
#define EIP96MASK_MASK                                               0x40000000
#define EIP96MASK_RD(src)                            (((src) & 0x40000000)>>30)
#define EIP96MASK_WR(src)                       (((u32)(src)<<30) & 0x40000000)
#define EIP96MASK_SET(dst,src) \
                      (((dst) & ~0x40000000) | (((u32)(src)<<30) & 0x40000000))
/*    Mask Register Fields xtsMask    */
#define XTSMASK_WIDTH                                                         1
#define XTSMASK_SHIFT                                                        29
#define XTSMASK_MASK                                                 0x20000000
#define XTSMASK_RD(src)                              (((src) & 0x20000000)>>29)
#define XTSMASK_WR(src)                         (((u32)(src)<<29) & 0x20000000)
#define XTSMASK_SET(dst,src) \
                      (((dst) & ~0x20000000) | (((u32)(src)<<29) & 0x20000000))

/*	Register csr_glb_sec_int_test	*/ 
/*	 Fields force_sec_int	 */
#define FORCE_SEC_INT_WIDTH                                                   1
#define FORCE_SEC_INT_SHIFT                                                  31
#define FORCE_SEC_INT_MASK                                           0x80000000
#define FORCE_SEC_INT_RD(src)                        (((src) & 0x80000000)>>31)
#define FORCE_SEC_INT_WR(src)                   (((u32)(src)<<31) & 0x80000000)
#define FORCE_SEC_INT_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))

/*	Global Base Address	*/
#define SEC_QMI_SLAVE_BASE_ADDR			0xddd830800ULL

/*    Address QMI_SLAVE  Registers */
#define SEC_CFGSSQMI0_ADDR                                           0x00000000
#define SEC_CFGSSQMI0_DEFAULT                                        0x4002c821
#define SEC_CFGSSQMI1_ADDR                                           0x00000004
#define SEC_CFGSSQMI1_DEFAULT                                        0x2de8b77c
#define SEC_CFGSSQMIFPBUFFER_ADDR                                    0x00000008
#define SEC_CFGSSQMIFPBUFFER_DEFAULT                                 0x00000000
#define SEC_CFGSSQMIWQBUFFER_ADDR                                    0x0000000c
#define SEC_CFGSSQMIWQBUFFER_DEFAULT                                 0x00000000
#define SEC_STSSSQMIFPBUFFER0_ADDR                                   0x00000010
#define SEC_STSSSQMIFPBUFFER0_DEFAULT                                0x00000000
#define SEC_STSSSQMIFPBUFFER1_ADDR                                   0x00000014
#define SEC_STSSSQMIFPBUFFER1_DEFAULT                                0x00000000
#define SEC_STSSSQMIFPBUFFER2_ADDR                                   0x00000018
#define SEC_STSSSQMIFPBUFFER2_DEFAULT                                0x00000000
#define SEC_STSSSQMIWQBUFFER0_ADDR                                   0x0000001c
#define SEC_STSSSQMIWQBUFFER0_DEFAULT                                0x00000000
#define SEC_STSSSQMIWQBUFFER1_ADDR                                   0x00000020
#define SEC_STSSSQMIWQBUFFER1_DEFAULT                                0x00000000
#define SEC_STSSSQMIWQBUFFER2_ADDR                                   0x00000024
#define SEC_STSSSQMIWQBUFFER2_DEFAULT                                0x00000000
#define SEC_STSSSQMIWQBUFFER3_ADDR                                   0x00000028
#define SEC_STSSSQMIWQBUFFER3_DEFAULT                                0x00000000
#define SEC_CFGSSQMISAB_ADDR                                         0x0000002c
#define SEC_CFGSSQMISAB_DEFAULT                                      0x00000000
#define SEC_CFGSSQMISAB0_ADDR                                        0x00000030
#define SEC_CFGSSQMISAB0_DEFAULT                                     0x00000000
#define SEC_CFGSSQMISAB1_ADDR                                        0x00000034
#define SEC_CFGSSQMISAB1_DEFAULT                                     0x00000000
#define SEC_CFGSSQMISAB2_ADDR                                        0x00000038
#define SEC_CFGSSQMISAB2_DEFAULT                                     0x00000000
#define SEC_CFGSSQMISAB3_ADDR                                        0x0000003c
#define SEC_CFGSSQMISAB3_DEFAULT                                     0x00000000
#define SEC_CFGSSQMISAB4_ADDR                                        0x00000040
#define SEC_CFGSSQMISAB4_DEFAULT                                     0x00000000
#define SEC_STSSSQMIINT0_ADDR                                        0x00000044
#define SEC_STSSSQMIINT0_DEFAULT                                     0x00000000
#define SEC_STSSSQMIINT0MASK_ADDR                                    0x00000048
#define SEC_STSSSQMIINT1_ADDR                                        0x0000004c
#define SEC_STSSSQMIINT1_DEFAULT                                     0x00000000
#define SEC_STSSSQMIINT1MASK_ADDR                                    0x00000050
#define SEC_STSSSQMIINT2_ADDR                                        0x00000054
#define SEC_STSSSQMIINT2_DEFAULT                                     0x00000000
#define SEC_STSSSQMIINT2MASK_ADDR                                    0x00000058
#define SEC_STSSSQMIINT3_ADDR                                        0x0000005c
#define SEC_STSSSQMIINT3_DEFAULT                                     0x00000000
#define SEC_STSSSQMIINT3MASK_ADDR                                    0x00000060
#define SEC_STSSSQMIINT4_ADDR                                        0x00000064
#define SEC_STSSSQMIINT4_DEFAULT                                     0x00000000
#define SEC_STSSSQMIINT4MASK_ADDR                                    0x00000068
#define SEC_CFGSSQMIDBGCTRL_ADDR                                     0x0000006c
#define SEC_CFGSSQMIDBGCTRL_DEFAULT                                  0x00000000
#define SEC_CFGSSQMIDBGDATA0_ADDR                                    0x00000070
#define SEC_CFGSSQMIDBGDATA0_DEFAULT                                 0x00000000
#define SEC_CFGSSQMIDBGDATA1_ADDR                                    0x00000074
#define SEC_CFGSSQMIDBGDATA1_DEFAULT                                 0x00000000
#define SEC_CFGSSQMIDBGDATA2_ADDR                                    0x00000078
#define SEC_CFGSSQMIDBGDATA2_DEFAULT                                 0x00000000
#define SEC_CFGSSQMIDBGDATA3_ADDR                                    0x0000007c
#define SEC_CFGSSQMIDBGDATA3_DEFAULT                                 0x00000000
#define SEC_STSSSQMIDBGDATA_ADDR                                     0x00000080
#define SEC_STSSSQMIDBGDATA_DEFAULT                                  0x00000000
#define SEC_CFGSSQMILITE_ADDR                                        0x00000084
#define SEC_CFGSSQMILITE_DEFAULT                                     0x00003400
#define SEC_CFGSSQMIFPQASSOC_ADDR                                    0x00000088
#define SEC_CFGSSQMIFPQASSOC_DEFAULT                                 0x00030000
#define SEC_CFGSSQMIWQASSOC_ADDR                                     0x0000008c
#define SEC_CFGSSQMIWQASSOC_DEFAULT                                  0x00010000
#define SEC_CFGSSQMIMEMORY_ADDR                                      0x00000090
#define SEC_CFGSSQMIMEMORY_DEFAULT                                   0x00000000
#define SEC_STSSSQMIFIFO_ADDR                                        0x00000094
#define SEC_STSSSQMIFIFO_DEFAULT                                     0x07ffffff
#define SEC_CFGSSQMIDOMEN_ADDR                                       0x00000098
#define SEC_CFGSSQMIDOMEN_DEFAULT                                    0x01ffffff
#define SEC_CFGSSQMIDOMVAL0_ADDR                                     0x0000009c
#define SEC_CFGSSQMIDOMVAL0_DEFAULT                                  0x000000fb
#define SEC_CFGSSQMIDOMVAL1_ADDR                                     0x000000a0
#define SEC_CFGSSQMIDOMVAL1_DEFAULT                                  0x000000fb
#define SEC_CFGSSQMIDOMVAL2_ADDR                                     0x000000a4
#define SEC_CFGSSQMIDOMVAL2_DEFAULT                                  0x00000004

/*	Register CfgSsQmi0	*/ 
/*	 Fields WQBavailFMWait	 */
#define SEC_WQBAVAILFMWAIT0_WIDTH                                             1
#define SEC_WQBAVAILFMWAIT0_SHIFT                                            30
#define SEC_WQBAVAILFMWAIT0_MASK                                     0x40000000
#define SEC_WQBAVAILFMWAIT0_RD(src)                  (((src) & 0x40000000)>>30)
#define SEC_WQBAVAILFMWAIT0_WR(src)             (((u32)(src)<<30) & 0x40000000)
#define SEC_WQBAVAILFMWAIT0_SET(dst,src) \
                      (((dst) & ~0x40000000) | (((u32)(src)<<30) & 0x40000000))
/*	 Fields WQ16Disable	 */
#define SEC_WQ16DISABLE0_WIDTH                                                1
#define SEC_WQ16DISABLE0_SHIFT                                               29
#define SEC_WQ16DISABLE0_MASK                                        0x20000000
#define SEC_WQ16DISABLE0_RD(src)                     (((src) & 0x20000000)>>29)
#define SEC_WQ16DISABLE0_WR(src)                (((u32)(src)<<29) & 0x20000000)
#define SEC_WQ16DISABLE0_SET(dst,src) \
                      (((dst) & ~0x20000000) | (((u32)(src)<<29) & 0x20000000))
/*	 Fields WQ16Reset	 */
#define SEC_WQ16RESET0_WIDTH                                                  1
#define SEC_WQ16RESET0_SHIFT                                                 28
#define SEC_WQ16RESET0_MASK                                          0x10000000
#define SEC_WQ16RESET0_RD(src)                       (((src) & 0x10000000)>>28)
#define SEC_WQ16RESET0_WR(src)                  (((u32)(src)<<28) & 0x10000000)
#define SEC_WQ16RESET0_SET(dst,src) \
                      (((dst) & ~0x10000000) | (((u32)(src)<<28) & 0x10000000))
/*	 Fields FP17Disable	 */
#define SEC_FP17DISABLE0_WIDTH                                                1
#define SEC_FP17DISABLE0_SHIFT                                               27
#define SEC_FP17DISABLE0_MASK                                        0x08000000
#define SEC_FP17DISABLE0_RD(src)                     (((src) & 0x08000000)>>27)
#define SEC_FP17DISABLE0_WR(src)                (((u32)(src)<<27) & 0x08000000)
#define SEC_FP17DISABLE0_SET(dst,src) \
                      (((dst) & ~0x08000000) | (((u32)(src)<<27) & 0x08000000))
/*	 Fields FP16Disable	 */
#define SEC_FP16DISABLE0_WIDTH                                                1
#define SEC_FP16DISABLE0_SHIFT                                               26
#define SEC_FP16DISABLE0_MASK                                        0x04000000
#define SEC_FP16DISABLE0_RD(src)                     (((src) & 0x04000000)>>26)
#define SEC_FP16DISABLE0_WR(src)                (((u32)(src)<<26) & 0x04000000)
#define SEC_FP16DISABLE0_SET(dst,src) \
                      (((dst) & ~0x04000000) | (((u32)(src)<<26) & 0x04000000))
/*	 Fields FP17Reset	 */
#define SEC_FP17RESET0_WIDTH                                                  1
#define SEC_FP17RESET0_SHIFT                                                 25
#define SEC_FP17RESET0_MASK                                          0x02000000
#define SEC_FP17RESET0_RD(src)                       (((src) & 0x02000000)>>25)
#define SEC_FP17RESET0_WR(src)                  (((u32)(src)<<25) & 0x02000000)
#define SEC_FP17RESET0_SET(dst,src) \
                      (((dst) & ~0x02000000) | (((u32)(src)<<25) & 0x02000000))
/*	 Fields FP16Reset	 */
#define SEC_FP16RESET0_WIDTH                                                  1
#define SEC_FP16RESET0_SHIFT                                                 24
#define SEC_FP16RESET0_MASK                                          0x01000000
#define SEC_FP16RESET0_RD(src)                       (((src) & 0x01000000)>>24)
#define SEC_FP16RESET0_WR(src)                  (((u32)(src)<<24) & 0x01000000)
#define SEC_FP16RESET0_SET(dst,src) \
                      (((dst) & ~0x01000000) | (((u32)(src)<<24) & 0x01000000))
/*	 Fields FPDecDiffThreshold	 */
#define SEC_FPDECDIFFTHRESHOLD0_WIDTH                                         3
#define SEC_FPDECDIFFTHRESHOLD0_SHIFT                                        21
#define SEC_FPDECDIFFTHRESHOLD0_MASK                                 0x00e00000
#define SEC_FPDECDIFFTHRESHOLD0_RD(src)              (((src) & 0x00e00000)>>21)
#define SEC_FPDECDIFFTHRESHOLD0_WR(src)         (((u32)(src)<<21) & 0x00e00000)
#define SEC_FPDECDIFFTHRESHOLD0_SET(dst,src) \
                      (((dst) & ~0x00e00000) | (((u32)(src)<<21) & 0x00e00000))
/*	 Fields WQDecDiffThreshold	 */
#define SEC_WQDECDIFFTHRESHOLD0_WIDTH                                         4
#define SEC_WQDECDIFFTHRESHOLD0_SHIFT                                        17
#define SEC_WQDECDIFFTHRESHOLD0_MASK                                 0x001e0000
#define SEC_WQDECDIFFTHRESHOLD0_RD(src)              (((src) & 0x001e0000)>>17)
#define SEC_WQDECDIFFTHRESHOLD0_WR(src)         (((u32)(src)<<17) & 0x001e0000)
#define SEC_WQDECDIFFTHRESHOLD0_SET(dst,src) \
                      (((dst) & ~0x001e0000) | (((u32)(src)<<17) & 0x001e0000))
/*	 Fields DeallocThreshold	 */
#define SEC_DEALLOCTHRESHOLD0_WIDTH                                           3
#define SEC_DEALLOCTHRESHOLD0_SHIFT                                          14
#define SEC_DEALLOCTHRESHOLD0_MASK                                   0x0001c000
#define SEC_DEALLOCTHRESHOLD0_RD(src)                (((src) & 0x0001c000)>>14)
#define SEC_DEALLOCTHRESHOLD0_WR(src)           (((u32)(src)<<14) & 0x0001c000)
#define SEC_DEALLOCTHRESHOLD0_SET(dst,src) \
                      (((dst) & ~0x0001c000) | (((u32)(src)<<14) & 0x0001c000))
/*	 Fields FPDecThreshold	 */
#define SEC_FPDECTHRESHOLD0_WIDTH                                             3
#define SEC_FPDECTHRESHOLD0_SHIFT                                            11
#define SEC_FPDECTHRESHOLD0_MASK                                     0x00003800
#define SEC_FPDECTHRESHOLD0_RD(src)                  (((src) & 0x00003800)>>11)
#define SEC_FPDECTHRESHOLD0_WR(src)             (((u32)(src)<<11) & 0x00003800)
#define SEC_FPDECTHRESHOLD0_SET(dst,src) \
                      (((dst) & ~0x00003800) | (((u32)(src)<<11) & 0x00003800))
/*	 Fields FPBAvlThreshold	 */
#define SEC_FPBAVLTHRESHOLD0_WIDTH                                            3
#define SEC_FPBAVLTHRESHOLD0_SHIFT                                            8
#define SEC_FPBAVLTHRESHOLD0_MASK                                    0x00000700
#define SEC_FPBAVLTHRESHOLD0_RD(src)                  (((src) & 0x00000700)>>8)
#define SEC_FPBAVLTHRESHOLD0_WR(src)             (((u32)(src)<<8) & 0x00000700)
#define SEC_FPBAVLTHRESHOLD0_SET(dst,src) \
                       (((dst) & ~0x00000700) | (((u32)(src)<<8) & 0x00000700))
/*	 Fields WQDecThreshold	 */
#define SEC_WQDECTHRESHOLD0_WIDTH                                             4
#define SEC_WQDECTHRESHOLD0_SHIFT                                             4
#define SEC_WQDECTHRESHOLD0_MASK                                     0x000000f0
#define SEC_WQDECTHRESHOLD0_RD(src)                   (((src) & 0x000000f0)>>4)
#define SEC_WQDECTHRESHOLD0_WR(src)              (((u32)(src)<<4) & 0x000000f0)
#define SEC_WQDECTHRESHOLD0_SET(dst,src) \
                       (((dst) & ~0x000000f0) | (((u32)(src)<<4) & 0x000000f0))
/*	 Fields WQBAvlThreshold	 */
#define SEC_WQBAVLTHRESHOLD0_WIDTH                                            4
#define SEC_WQBAVLTHRESHOLD0_SHIFT                                            0
#define SEC_WQBAVLTHRESHOLD0_MASK                                    0x0000000f
#define SEC_WQBAVLTHRESHOLD0_RD(src)                     (((src) & 0x0000000f))
#define SEC_WQBAVLTHRESHOLD0_WR(src)                (((u32)(src)) & 0x0000000f)
#define SEC_WQBAVLTHRESHOLD0_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register CfgSsQmi1	*/ 
/*	 Fields CmOverrideLLFields	 */
#define SEC_CMOVERRIDELLFIELDS1_WIDTH                                         1
#define SEC_CMOVERRIDELLFIELDS1_SHIFT                                        29
#define SEC_CMOVERRIDELLFIELDS1_MASK                                 0x20000000
#define SEC_CMOVERRIDELLFIELDS1_RD(src)              (((src) & 0x20000000)>>29)
#define SEC_CMOVERRIDELLFIELDS1_WR(src)         (((u32)(src)<<29) & 0x20000000)
#define SEC_CMOVERRIDELLFIELDS1_SET(dst,src) \
                      (((dst) & ~0x20000000) | (((u32)(src)<<29) & 0x20000000))
/*	 Fields CmCtrlbuffThreshold	 */
#define SEC_CMCTRLBUFFTHRESHOLD1_WIDTH                                        3
#define SEC_CMCTRLBUFFTHRESHOLD1_SHIFT                                       26
#define SEC_CMCTRLBUFFTHRESHOLD1_MASK                                0x1c000000
#define SEC_CMCTRLBUFFTHRESHOLD1_RD(src)             (((src) & 0x1c000000)>>26)
#define SEC_CMCTRLBUFFTHRESHOLD1_WR(src)        (((u32)(src)<<26) & 0x1c000000)
#define SEC_CMCTRLBUFFTHRESHOLD1_SET(dst,src) \
                      (((dst) & ~0x1c000000) | (((u32)(src)<<26) & 0x1c000000))
/*	 Fields CmDatabuffThreshold	 */
#define SEC_CMDATABUFFTHRESHOLD1_WIDTH                                        5
#define SEC_CMDATABUFFTHRESHOLD1_SHIFT                                       21
#define SEC_CMDATABUFFTHRESHOLD1_MASK                                0x03e00000
#define SEC_CMDATABUFFTHRESHOLD1_RD(src)             (((src) & 0x03e00000)>>21)
#define SEC_CMDATABUFFTHRESHOLD1_WR(src)        (((u32)(src)<<21) & 0x03e00000)
#define SEC_CMDATABUFFTHRESHOLD1_SET(dst,src) \
                      (((dst) & ~0x03e00000) | (((u32)(src)<<21) & 0x03e00000))
/*	 Fields CmMsgfThreshold	 */
#define SEC_CMMSGFTHRESHOLD1_WIDTH                                            4
#define SEC_CMMSGFTHRESHOLD1_SHIFT                                           17
#define SEC_CMMSGFTHRESHOLD1_MASK                                    0x001e0000
#define SEC_CMMSGFTHRESHOLD1_RD(src)                 (((src) & 0x001e0000)>>17)
#define SEC_CMMSGFTHRESHOLD1_WR(src)            (((u32)(src)<<17) & 0x001e0000)
#define SEC_CMMSGFTHRESHOLD1_SET(dst,src) \
                      (((dst) & ~0x001e0000) | (((u32)(src)<<17) & 0x001e0000))
/*	 Fields CmRegfThreshold	 */
#define SEC_CMREGFTHRESHOLD1_WIDTH                                            3
#define SEC_CMREGFTHRESHOLD1_SHIFT                                           14
#define SEC_CMREGFTHRESHOLD1_MASK                                    0x0001c000
#define SEC_CMREGFTHRESHOLD1_RD(src)                 (((src) & 0x0001c000)>>14)
#define SEC_CMREGFTHRESHOLD1_WR(src)            (((u32)(src)<<14) & 0x0001c000)
#define SEC_CMREGFTHRESHOLD1_SET(dst,src) \
                      (((dst) & ~0x0001c000) | (((u32)(src)<<14) & 0x0001c000))
/*	 Fields QMDeviceAddress	 */
#define SEC_QMDEVICEADDRESS1_WIDTH                                           14
#define SEC_QMDEVICEADDRESS1_SHIFT                                            0
#define SEC_QMDEVICEADDRESS1_MASK                                    0x00003fff
#define SEC_QMDEVICEADDRESS1_RD(src)                     (((src) & 0x00003fff))
#define SEC_QMDEVICEADDRESS1_WR(src)                (((u32)(src)) & 0x00003fff)
#define SEC_QMDEVICEADDRESS1_SET(dst,src) \
                          (((dst) & ~0x00003fff) | (((u32)(src)) & 0x00003fff))

/*	Register CfgSsQmiFPBuffer	*/ 
/*	 Fields Disable	 */
#define EIP_SEC_DISABLE_WIDTH                                                    16
#define EIP_SEC_DISABLE_SHIFT                                                    16
#define EIP_SEC_DISABLE_MASK                                             0xffff0000
#define EIP_SEC_DISABLE_RD(src)                          (((src) & 0xffff0000)>>16)
#define EIP_SEC_DISABLE_WR(src)                     (((u32)(src)<<16) & 0xffff0000)
#define EIP_SEC_DISABLE_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields Reset	 */
#define SEC_RESET_WIDTH                                                      16
#define SEC_RESET_SHIFT                                                       0
#define SEC_RESET_MASK                                               0x0000ffff
#define SEC_RESET_RD(src)                                (((src) & 0x0000ffff))
#define SEC_RESET_WR(src)                           (((u32)(src)) & 0x0000ffff)
#define SEC_RESET_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register CfgSsQmiWQBuffer	*/ 
/*	 Fields Disable	 */
#define SEC_DISABLE_F1_WIDTH                                                 16
#define SEC_DISABLE_F1_SHIFT                                                 16
#define SEC_DISABLE_F1_MASK                                          0xffff0000
#define SEC_DISABLE_F1_RD(src)                       (((src) & 0xffff0000)>>16)
#define SEC_DISABLE_F1_WR(src)                  (((u32)(src)<<16) & 0xffff0000)
#define SEC_DISABLE_F1_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields Reset	 */
#define SEC_RESET_F1_WIDTH                                                   16
#define SEC_RESET_F1_SHIFT                                                    0
#define SEC_RESET_F1_MASK                                            0x0000ffff
#define SEC_RESET_F1_RD(src)                             (((src) & 0x0000ffff))
#define SEC_RESET_F1_WR(src)                        (((u32)(src)) & 0x0000ffff)
#define SEC_RESET_F1_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register StsSsQmiFPBuffer0	*/ 
/*	 Fields WritePointer	 */
#define SEC_WRITEPOINTER0_WIDTH                                              32
#define SEC_WRITEPOINTER0_SHIFT                                               0
#define SEC_WRITEPOINTER0_MASK                                       0xffffffff
#define SEC_WRITEPOINTER0_RD(src)                        (((src) & 0xffffffff))
#define SEC_WRITEPOINTER0_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register StsSsQmiFPBuffer1	*/ 
/*	 Fields WritePointer	 */
#define SEC_WRITEPOINTER1_WIDTH                                               4
#define SEC_WRITEPOINTER1_SHIFT                                              24
#define SEC_WRITEPOINTER1_MASK                                       0x0f000000
#define SEC_WRITEPOINTER1_RD(src)                    (((src) & 0x0f000000)>>24)
#define SEC_WRITEPOINTER1_SET(dst,src) \
                      (((dst) & ~0x0f000000) | (((u32)(src)<<24) & 0x0f000000))
/*	 Fields Entries	 */
#define SEC_ENTRIES1_WIDTH                                                   24
#define SEC_ENTRIES1_SHIFT                                                    0
#define SEC_ENTRIES1_MASK                                            0x00ffffff
#define SEC_ENTRIES1_RD(src)                             (((src) & 0x00ffffff))
#define SEC_ENTRIES1_SET(dst,src) \
                          (((dst) & ~0x00ffffff) | (((u32)(src)) & 0x00ffffff))

/*	Register StsSsQmiFPBuffer2	*/ 
/*	 Fields Entries	 */
#define SEC_ENTRIES2_WIDTH                                                   30
#define SEC_ENTRIES2_SHIFT                                                    0
#define SEC_ENTRIES2_MASK                                            0x3fffffff
#define SEC_ENTRIES2_RD(src)                             (((src) & 0x3fffffff))
#define SEC_ENTRIES2_SET(dst,src) \
                          (((dst) & ~0x3fffffff) | (((u32)(src)) & 0x3fffffff))

/*	Register StsSsQmiWQBuffer0	*/ 
/*	 Fields WritePointer	 */
#define SEC_WRITEPOINTER0_F1_WIDTH                                           24
#define SEC_WRITEPOINTER0_F1_SHIFT                                            0
#define SEC_WRITEPOINTER0_F1_MASK                                    0x00ffffff
#define SEC_WRITEPOINTER0_F1_RD(src)                     (((src) & 0x00ffffff))
#define SEC_WRITEPOINTER0_F1_SET(dst,src) \
                          (((dst) & ~0x00ffffff) | (((u32)(src)) & 0x00ffffff))

/*	Register StsSsQmiWQBuffer1	*/ 
/*	 Fields Entries	 */
#define SEC_ENTRIES1_F1_WIDTH                                                 4
#define SEC_ENTRIES1_F1_SHIFT                                                27
#define SEC_ENTRIES1_F1_MASK                                         0x78000000
#define SEC_ENTRIES1_F1_RD(src)                      (((src) & 0x78000000)>>27)
#define SEC_ENTRIES1_F1_SET(dst,src) \
                      (((dst) & ~0x78000000) | (((u32)(src)<<27) & 0x78000000))
/*	 Fields WritePointer	 */
#define SEC_WRITEPOINTER1_F1_WIDTH                                           27
#define SEC_WRITEPOINTER1_F1_SHIFT                                            0
#define SEC_WRITEPOINTER1_F1_MASK                                    0x07ffffff
#define SEC_WRITEPOINTER1_F1_RD(src)                     (((src) & 0x07ffffff))
#define SEC_WRITEPOINTER1_F1_SET(dst,src) \
                          (((dst) & ~0x07ffffff) | (((u32)(src)) & 0x07ffffff))

/*	Register StsSsQmiWQBuffer2	*/ 
/*	 Fields Entries	 */
#define SEC_ENTRIES2_F1_WIDTH                                                32
#define SEC_ENTRIES2_F1_SHIFT                                                 0
#define SEC_ENTRIES2_F1_MASK                                         0xffffffff
#define SEC_ENTRIES2_F1_RD(src)                          (((src) & 0xffffffff))
#define SEC_ENTRIES2_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register StsSsQmiWQBuffer3	*/ 
/*	 Fields Entries	 */
#define SEC_ENTRIES3_WIDTH                                                   32
#define SEC_ENTRIES3_SHIFT                                                    0
#define SEC_ENTRIES3_MASK                                            0xffffffff
#define SEC_ENTRIES3_RD(src)                             (((src) & 0xffffffff))
#define SEC_ENTRIES3_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CfgSsQmiSab	*/ 
/*	 Fields EnableMonitoring	 */
#define SEC_ENABLEMONITORING_WIDTH                                           17
#define SEC_ENABLEMONITORING_SHIFT                                            0
#define SEC_ENABLEMONITORING_MASK                                    0x0001ffff
#define SEC_ENABLEMONITORING_RD(src)                     (((src) & 0x0001ffff))
#define SEC_ENABLEMONITORING_WR(src)                (((u32)(src)) & 0x0001ffff)
#define SEC_ENABLEMONITORING_SET(dst,src) \
                          (((dst) & ~0x0001ffff) | (((u32)(src)) & 0x0001ffff))

/*	Register CfgSsQmiSab0	*/ 
/*	 Fields QID	 */
#define SEC_QID0_WIDTH                                                       32
#define SEC_QID0_SHIFT                                                        0
#define SEC_QID0_MASK                                                0xffffffff
#define SEC_QID0_RD(src)                                 (((src) & 0xffffffff))
#define SEC_QID0_WR(src)                            (((u32)(src)) & 0xffffffff)
#define SEC_QID0_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CfgSsQmiSab1	*/ 
/*	 Fields QID	 */
#define SEC_QID1_WIDTH                                                       32
#define SEC_QID1_SHIFT                                                        0
#define SEC_QID1_MASK                                                0xffffffff
#define SEC_QID1_RD(src)                                 (((src) & 0xffffffff))
#define SEC_QID1_WR(src)                            (((u32)(src)) & 0xffffffff)
#define SEC_QID1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CfgSsQmiSab2	*/ 
/*	 Fields QID	 */
#define SEC_QID2_WIDTH                                                       32
#define SEC_QID2_SHIFT                                                        0
#define SEC_QID2_MASK                                                0xffffffff
#define SEC_QID2_RD(src)                                 (((src) & 0xffffffff))
#define SEC_QID2_WR(src)                            (((u32)(src)) & 0xffffffff)
#define SEC_QID2_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CfgSsQmiSab3	*/ 
/*	 Fields QID	 */
#define SEC_QID3_WIDTH                                                       32
#define SEC_QID3_SHIFT                                                        0
#define SEC_QID3_MASK                                                0xffffffff
#define SEC_QID3_RD(src)                                 (((src) & 0xffffffff))
#define SEC_QID3_WR(src)                            (((u32)(src)) & 0xffffffff)
#define SEC_QID3_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CfgSsQmiSab4	*/ 
/*	 Fields QID	 */
#define SEC_QID4_WIDTH                                                        8
#define SEC_QID4_SHIFT                                                        0
#define SEC_QID4_MASK                                                0x000000ff
#define SEC_QID4_RD(src)                                 (((src) & 0x000000ff))
#define SEC_QID4_WR(src)                            (((u32)(src)) & 0x000000ff)
#define SEC_QID4_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register StsSsQmiInt0	*/ 
/*	 Fields FPOverflow	 */
#define SEC_FPOVERFLOW0_WIDTH                                                18
#define SEC_FPOVERFLOW0_SHIFT                                                 0
#define SEC_FPOVERFLOW0_MASK                                         0x0003ffff
#define SEC_FPOVERFLOW0_RD(src)                          (((src) & 0x0003ffff))
#define SEC_FPOVERFLOW0_WR(src)                     (((u32)(src)) & 0x0003ffff)
#define SEC_FPOVERFLOW0_SET(dst,src) \
                          (((dst) & ~0x0003ffff) | (((u32)(src)) & 0x0003ffff))

/*	Register StsSsQmiInt0Mask	*/
/*    Mask Register Fields FPOverflowMask    */
#define SEC_FPOVERFLOWMASK_WIDTH                                             18
#define SEC_FPOVERFLOWMASK_SHIFT                                              0
#define SEC_FPOVERFLOWMASK_MASK                                      0x0003ffff
#define SEC_FPOVERFLOWMASK_RD(src)                       (((src) & 0x0003ffff))
#define SEC_FPOVERFLOWMASK_WR(src)                  (((u32)(src)) & 0x0003ffff)
#define SEC_FPOVERFLOWMASK_SET(dst,src) \
                          (((dst) & ~0x0003ffff) | (((u32)(src)) & 0x0003ffff))

/*	Register StsSsQmiInt1	*/ 
/*	 Fields WQOverflow	 */
#define SEC_WQOVERFLOW1_WIDTH                                                17
#define SEC_WQOVERFLOW1_SHIFT                                                 0
#define SEC_WQOVERFLOW1_MASK                                         0x0001ffff
#define SEC_WQOVERFLOW1_RD(src)                          (((src) & 0x0001ffff))
#define SEC_WQOVERFLOW1_WR(src)                     (((u32)(src)) & 0x0001ffff)
#define SEC_WQOVERFLOW1_SET(dst,src) \
                          (((dst) & ~0x0001ffff) | (((u32)(src)) & 0x0001ffff))

/*	Register StsSsQmiInt1Mask	*/
/*    Mask Register Fields WQOverflowMask    */
#define SEC_WQOVERFLOWMASK_WIDTH                                             17
#define SEC_WQOVERFLOWMASK_SHIFT                                              0
#define SEC_WQOVERFLOWMASK_MASK                                      0x0001ffff
#define SEC_WQOVERFLOWMASK_RD(src)                       (((src) & 0x0001ffff))
#define SEC_WQOVERFLOWMASK_WR(src)                  (((u32)(src)) & 0x0001ffff)
#define SEC_WQOVERFLOWMASK_SET(dst,src) \
                          (((dst) & ~0x0001ffff) | (((u32)(src)) & 0x0001ffff))

/*	Register StsSsQmiInt2	*/ 
/*	 Fields FPUnderRun	 */
#define SEC_FPUNDERRUN2_WIDTH                                                18
#define SEC_FPUNDERRUN2_SHIFT                                                 0
#define SEC_FPUNDERRUN2_MASK                                         0x0003ffff
#define SEC_FPUNDERRUN2_RD(src)                          (((src) & 0x0003ffff))
#define SEC_FPUNDERRUN2_WR(src)                     (((u32)(src)) & 0x0003ffff)
#define SEC_FPUNDERRUN2_SET(dst,src) \
                          (((dst) & ~0x0003ffff) | (((u32)(src)) & 0x0003ffff))

/*	Register StsSsQmiInt2Mask	*/
/*    Mask Register Fields FPUnderRunMask    */
#define SEC_FPUNDERRUNMASK_WIDTH                                             18
#define SEC_FPUNDERRUNMASK_SHIFT                                              0
#define SEC_FPUNDERRUNMASK_MASK                                      0x0003ffff
#define SEC_FPUNDERRUNMASK_RD(src)                       (((src) & 0x0003ffff))
#define SEC_FPUNDERRUNMASK_WR(src)                  (((u32)(src)) & 0x0003ffff)
#define SEC_FPUNDERRUNMASK_SET(dst,src) \
                          (((dst) & ~0x0003ffff) | (((u32)(src)) & 0x0003ffff))

/*	Register StsSsQmiInt3	*/ 
/*	 Fields WQUnderRun	 */
#define SEC_WQUNDERRUN3_WIDTH                                                17
#define SEC_WQUNDERRUN3_SHIFT                                                 0
#define SEC_WQUNDERRUN3_MASK                                         0x0001ffff
#define SEC_WQUNDERRUN3_RD(src)                          (((src) & 0x0001ffff))
#define SEC_WQUNDERRUN3_WR(src)                     (((u32)(src)) & 0x0001ffff)
#define SEC_WQUNDERRUN3_SET(dst,src) \
                          (((dst) & ~0x0001ffff) | (((u32)(src)) & 0x0001ffff))

/*	Register StsSsQmiInt3Mask	*/
/*    Mask Register Fields WQUnderRunMask    */
#define SEC_WQUNDERRUNMASK_WIDTH                                             17
#define SEC_WQUNDERRUNMASK_SHIFT                                              0
#define SEC_WQUNDERRUNMASK_MASK                                      0x0001ffff
#define SEC_WQUNDERRUNMASK_RD(src)                       (((src) & 0x0001ffff))
#define SEC_WQUNDERRUNMASK_WR(src)                  (((u32)(src)) & 0x0001ffff)
#define SEC_WQUNDERRUNMASK_SET(dst,src) \
                          (((dst) & ~0x0001ffff) | (((u32)(src)) & 0x0001ffff))

/*	Register StsSsQmiInt4	*/ 
/*	 Fields axiwcmr_slverr	 */
#define SEC_AXIWCMR_SLVERR4_WIDTH                                             1
#define SEC_AXIWCMR_SLVERR4_SHIFT                                             1
#define SEC_AXIWCMR_SLVERR4_MASK                                     0x00000002
#define SEC_AXIWCMR_SLVERR4_RD(src)                   (((src) & 0x00000002)>>1)
#define SEC_AXIWCMR_SLVERR4_WR(src)              (((u32)(src)<<1) & 0x00000002)
#define SEC_AXIWCMR_SLVERR4_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields axiwcmr_decerr	 */
#define SEC_AXIWCMR_DECERR4_WIDTH                                             1
#define SEC_AXIWCMR_DECERR4_SHIFT                                             0
#define SEC_AXIWCMR_DECERR4_MASK                                     0x00000001
#define SEC_AXIWCMR_DECERR4_RD(src)                      (((src) & 0x00000001))
#define SEC_AXIWCMR_DECERR4_WR(src)                 (((u32)(src)) & 0x00000001)
#define SEC_AXIWCMR_DECERR4_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register StsSsQmiInt4Mask	*/
/*    Mask Register Fields axiwcmr_slverrMask    */
#define SEC_AXIWCMR_SLVERRMASK_WIDTH                                          1
#define SEC_AXIWCMR_SLVERRMASK_SHIFT                                          1
#define SEC_AXIWCMR_SLVERRMASK_MASK                                  0x00000002
#define SEC_AXIWCMR_SLVERRMASK_RD(src)                (((src) & 0x00000002)>>1)
#define SEC_AXIWCMR_SLVERRMASK_WR(src)           (((u32)(src)<<1) & 0x00000002)
#define SEC_AXIWCMR_SLVERRMASK_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*    Mask Register Fields axiwcmr_decerrMask    */
#define SEC_AXIWCMR_DECERRMASK_WIDTH                                          1
#define SEC_AXIWCMR_DECERRMASK_SHIFT                                          0
#define SEC_AXIWCMR_DECERRMASK_MASK                                  0x00000001
#define SEC_AXIWCMR_DECERRMASK_RD(src)                   (((src) & 0x00000001))
#define SEC_AXIWCMR_DECERRMASK_WR(src)              (((u32)(src)) & 0x00000001)
#define SEC_AXIWCMR_DECERRMASK_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register CfgSsQmiDbgCtrl	*/ 
/*	 Fields PBID	 */
#define SEC_PBID_WIDTH                                                        6
#define SEC_PBID_SHIFT                                                       11
#define SEC_PBID_MASK                                                0x0001f800
#define SEC_PBID_RD(src)                             (((src) & 0x0001f800)>>11)
#define SEC_PBID_WR(src)                        (((u32)(src)<<11) & 0x0001f800)
#define SEC_PBID_SET(dst,src) \
                      (((dst) & ~0x0001f800) | (((u32)(src)<<11) & 0x0001f800))
/*	 Fields nAck	 */
#define SEC_NACK_WIDTH                                                        1
#define SEC_NACK_SHIFT                                                       10
#define SEC_NACK_MASK                                                0x00000400
#define SEC_NACK_RD(src)                             (((src) & 0x00000400)>>10)
#define SEC_NACK_WR(src)                        (((u32)(src)<<10) & 0x00000400)
#define SEC_NACK_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields last	 */
#define SEC_LAST_WIDTH                                                        1
#define SEC_LAST_SHIFT                                                        9
#define SEC_LAST_MASK                                                0x00000200
#define SEC_LAST_RD(src)                              (((src) & 0x00000200)>>9)
#define SEC_LAST_WR(src)                         (((u32)(src)<<9) & 0x00000200)
#define SEC_LAST_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields Push	 */
#define SEC_PUSH_WIDTH                                                        1
#define SEC_PUSH_SHIFT                                                        8
#define SEC_PUSH_MASK                                                0x00000100
#define SEC_PUSH_RD(src)                              (((src) & 0x00000100)>>8)
#define SEC_PUSH_WR(src)                         (((u32)(src)<<8) & 0x00000100)
#define SEC_PUSH_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*	 Fields Write	 */
#define SEC_WRITE_WIDTH                                                       1
#define SEC_WRITE_SHIFT                                                       7
#define SEC_WRITE_MASK                                               0x00000080
#define SEC_WRITE_RD(src)                             (((src) & 0x00000080)>>7)
#define SEC_WRITE_WR(src)                        (((u32)(src)<<7) & 0x00000080)
#define SEC_WRITE_SET(dst,src) \
                       (((dst) & ~0x00000080) | (((u32)(src)<<7) & 0x00000080))
/*	 Fields Pop	 */
#define SEC_POP_WIDTH                                                         1
#define SEC_POP_SHIFT                                                         6
#define SEC_POP_MASK                                                 0x00000040
#define SEC_POP_RD(src)                               (((src) & 0x00000040)>>6)
#define SEC_POP_WR(src)                          (((u32)(src)<<6) & 0x00000040)
#define SEC_POP_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*	 Fields Read	 */
#define SEC_READ_WIDTH                                                        1
#define SEC_READ_SHIFT                                                        5
#define SEC_READ_MASK                                                0x00000020
#define SEC_READ_RD(src)                              (((src) & 0x00000020)>>5)
#define SEC_READ_WR(src)                         (((u32)(src)<<5) & 0x00000020)
#define SEC_READ_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields BufferAddr	 */
#define SEC_BUFFERADDR_WIDTH                                                  5
#define SEC_BUFFERADDR_SHIFT                                                  0
#define SEC_BUFFERADDR_MASK                                          0x0000001f
#define SEC_BUFFERADDR_RD(src)                           (((src) & 0x0000001f))
#define SEC_BUFFERADDR_WR(src)                      (((u32)(src)) & 0x0000001f)
#define SEC_BUFFERADDR_SET(dst,src) \
                          (((dst) & ~0x0000001f) | (((u32)(src)) & 0x0000001f))

/*	Register CfgSsQmiDbgData0	*/ 
/*	 Fields Data	 */
#define SEC_DATA0_WIDTH                                                      32
#define SEC_DATA0_SHIFT                                                       0
#define SEC_DATA0_MASK                                               0xffffffff
#define SEC_DATA0_RD(src)                                (((src) & 0xffffffff))
#define SEC_DATA0_WR(src)                           (((u32)(src)) & 0xffffffff)
#define SEC_DATA0_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CfgSsQmiDbgData1	*/ 
/*	 Fields Data	 */
#define SEC_DATA1_WIDTH                                                      32
#define SEC_DATA1_SHIFT                                                       0
#define SEC_DATA1_MASK                                               0xffffffff
#define SEC_DATA1_RD(src)                                (((src) & 0xffffffff))
#define SEC_DATA1_WR(src)                           (((u32)(src)) & 0xffffffff)
#define SEC_DATA1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CfgSsQmiDbgData2	*/ 
/*	 Fields Data	 */
#define SEC_DATA2_WIDTH                                                      32
#define SEC_DATA2_SHIFT                                                       0
#define SEC_DATA2_MASK                                               0xffffffff
#define SEC_DATA2_RD(src)                                (((src) & 0xffffffff))
#define SEC_DATA2_WR(src)                           (((u32)(src)) & 0xffffffff)
#define SEC_DATA2_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CfgSsQmiDbgData3	*/ 
/*	 Fields Data	 */
#define SEC_DATA3_WIDTH                                                      32
#define SEC_DATA3_SHIFT                                                       0
#define SEC_DATA3_MASK                                               0xffffffff
#define SEC_DATA3_RD(src)                                (((src) & 0xffffffff))
#define SEC_DATA3_WR(src)                           (((u32)(src)) & 0xffffffff)
#define SEC_DATA3_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register StsSsQmiDbgData	*/ 
/*	 Fields Data	 */
#define SEC_DATA_WIDTH                                                       32
#define SEC_DATA_SHIFT                                                        0
#define SEC_DATA_MASK                                                0xffffffff
#define SEC_DATA_RD(src)                                 (((src) & 0xffffffff))
#define SEC_DATA_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CfgSsQmiLite	*/ 
/*	 Fields QMLiteDeviceAddress	 */
#define SEC_QMLITEDEVICEADDRESS_WIDTH                                        15
#define SEC_QMLITEDEVICEADDRESS_SHIFT                                         0
#define SEC_QMLITEDEVICEADDRESS_MASK                                 0x00007fff
#define SEC_QMLITEDEVICEADDRESS_RD(src)                  (((src) & 0x00007fff))
#define SEC_QMLITEDEVICEADDRESS_WR(src)             (((u32)(src)) & 0x00007fff)
#define SEC_QMLITEDEVICEADDRESS_SET(dst,src) \
                          (((dst) & ~0x00007fff) | (((u32)(src)) & 0x00007fff))

/*	Register CfgSsQmiFPQAssoc	*/ 
/*	 Fields Association	 */
#define SEC_ASSOCIATION_WIDTH                                                18
#define SEC_ASSOCIATION_SHIFT                                                 0
#define SEC_ASSOCIATION_MASK                                         0x0003ffff
#define SEC_ASSOCIATION_RD(src)                          (((src) & 0x0003ffff))
#define SEC_ASSOCIATION_WR(src)                     (((u32)(src)) & 0x0003ffff)
#define SEC_ASSOCIATION_SET(dst,src) \
                          (((dst) & ~0x0003ffff) | (((u32)(src)) & 0x0003ffff))

/*	Register CfgSsQmiWQAssoc	*/ 
/*	 Fields Association	 */
#define SEC_ASSOCIATION_F1_WIDTH                                             17
#define SEC_ASSOCIATION_F1_SHIFT                                              0
#define SEC_ASSOCIATION_F1_MASK                                      0x0001ffff
#define SEC_ASSOCIATION_F1_RD(src)                       (((src) & 0x0001ffff))
#define SEC_ASSOCIATION_F1_WR(src)                  (((u32)(src)) & 0x0001ffff)
#define SEC_ASSOCIATION_F1_SET(dst,src) \
                          (((dst) & ~0x0001ffff) | (((u32)(src)) & 0x0001ffff))

/*	Register CfgSsQmiMemory	*/ 
/*	 Fields RMA	 */
#define SEC_RMA_WIDTH                                                         2
#define SEC_RMA_SHIFT                                                         4
#define SEC_RMA_MASK                                                 0x00000030
#define SEC_RMA_RD(src)                               (((src) & 0x00000030)>>4)
#define SEC_RMA_WR(src)                          (((u32)(src)<<4) & 0x00000030)
#define SEC_RMA_SET(dst,src) \
                       (((dst) & ~0x00000030) | (((u32)(src)<<4) & 0x00000030))
/*	 Fields RMB	 */
#define SEC_RMB_WIDTH                                                         2
#define SEC_RMB_SHIFT                                                         2
#define SEC_RMB_MASK                                                 0x0000000c
#define SEC_RMB_RD(src)                               (((src) & 0x0000000c)>>2)
#define SEC_RMB_WR(src)                          (((u32)(src)<<2) & 0x0000000c)
#define SEC_RMB_SET(dst,src) \
                       (((dst) & ~0x0000000c) | (((u32)(src)<<2) & 0x0000000c))
/*	 Fields RMEA	 */
#define SEC_RMEA_WIDTH                                                        1
#define SEC_RMEA_SHIFT                                                        1
#define SEC_RMEA_MASK                                                0x00000002
#define SEC_RMEA_RD(src)                              (((src) & 0x00000002)>>1)
#define SEC_RMEA_WR(src)                         (((u32)(src)<<1) & 0x00000002)
#define SEC_RMEA_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields RMEB	 */
#define SEC_RMEB_WIDTH                                                        1
#define SEC_RMEB_SHIFT                                                        0
#define SEC_RMEB_MASK                                                0x00000001
#define SEC_RMEB_RD(src)                                 (((src) & 0x00000001))
#define SEC_RMEB_WR(src)                            (((u32)(src)) & 0x00000001)
#define SEC_RMEB_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register StsSsQmiFifo	*/ 
/*	 Fields empty	 */
#define SEC_EMPTY_WIDTH                                                      27
#define SEC_EMPTY_SHIFT                                                       0
#define SEC_EMPTY_MASK                                               0x07ffffff
#define SEC_EMPTY_RD(src)                                (((src) & 0x07ffffff))
#define SEC_EMPTY_SET(dst,src) \
                          (((dst) & ~0x07ffffff) | (((u32)(src)) & 0x07ffffff))

/*	Register CfgSsQMIDomEn	*/ 
/*	 Fields FPDomainIDEnable	 */
#define SEC_FPDOMAINIDENABLE_WIDTH                                           18
#define SEC_FPDOMAINIDENABLE_SHIFT                                            7
#define SEC_FPDOMAINIDENABLE_MASK                                    0x01ffff80
#define SEC_FPDOMAINIDENABLE_RD(src)                  (((src) & 0x01ffff80)>>7)
#define SEC_FPDOMAINIDENABLE_WR(src)             (((u32)(src)<<7) & 0x01ffff80)
#define SEC_FPDOMAINIDENABLE_SET(dst,src) \
                       (((dst) & ~0x01ffff80) | (((u32)(src)<<7) & 0x01ffff80))
/*	 Fields DecDomainIDEnable	 */
#define SEC_DECDOMAINIDENABLE_WIDTH                                           1
#define SEC_DECDOMAINIDENABLE_SHIFT                                           6
#define SEC_DECDOMAINIDENABLE_MASK                                   0x00000040
#define SEC_DECDOMAINIDENABLE_RD(src)                 (((src) & 0x00000040)>>6)
#define SEC_DECDOMAINIDENABLE_WR(src)            (((u32)(src)<<6) & 0x00000040)
#define SEC_DECDOMAINIDENABLE_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*	 Fields DeallocDomainIDEnable	 */
#define SEC_DEALLOCDOMAINIDENABLE_WIDTH                                       1
#define SEC_DEALLOCDOMAINIDENABLE_SHIFT                                       5
#define SEC_DEALLOCDOMAINIDENABLE_MASK                               0x00000020
#define SEC_DEALLOCDOMAINIDENABLE_RD(src)             (((src) & 0x00000020)>>5)
#define SEC_DEALLOCDOMAINIDENABLE_WR(src)        (((u32)(src)<<5) & 0x00000020)
#define SEC_DEALLOCDOMAINIDENABLE_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields CMDomainIDEnable	 */
#define SEC_CMDOMAINIDENABLE_WIDTH                                            5
#define SEC_CMDOMAINIDENABLE_SHIFT                                            0
#define SEC_CMDOMAINIDENABLE_MASK                                    0x0000001f
#define SEC_CMDOMAINIDENABLE_RD(src)                     (((src) & 0x0000001f))
#define SEC_CMDOMAINIDENABLE_WR(src)                (((u32)(src)) & 0x0000001f)
#define SEC_CMDOMAINIDENABLE_SET(dst,src) \
                          (((dst) & ~0x0000001f) | (((u32)(src)) & 0x0000001f))

/*	Register CfgSsQMIDomVal0	*/ 
/*	 Fields FPDomainID	 */
#define SEC_FPDOMAINID0_WIDTH                                                18
#define SEC_FPDOMAINID0_SHIFT                                                 3
#define SEC_FPDOMAINID0_MASK                                         0x001ffff8
#define SEC_FPDOMAINID0_RD(src)                       (((src) & 0x001ffff8)>>3)
#define SEC_FPDOMAINID0_WR(src)                  (((u32)(src)<<3) & 0x001ffff8)
#define SEC_FPDOMAINID0_SET(dst,src) \
                       (((dst) & ~0x001ffff8) | (((u32)(src)<<3) & 0x001ffff8))
/*	 Fields DecDomainID	 */
#define SEC_DECDOMAINID0_WIDTH                                                1
#define SEC_DECDOMAINID0_SHIFT                                                2
#define SEC_DECDOMAINID0_MASK                                        0x00000004
#define SEC_DECDOMAINID0_RD(src)                      (((src) & 0x00000004)>>2)
#define SEC_DECDOMAINID0_WR(src)                 (((u32)(src)<<2) & 0x00000004)
#define SEC_DECDOMAINID0_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields DeallocDomainID	 */
#define SEC_DEALLOCDOMAINID0_WIDTH                                            1
#define SEC_DEALLOCDOMAINID0_SHIFT                                            1
#define SEC_DEALLOCDOMAINID0_MASK                                    0x00000002
#define SEC_DEALLOCDOMAINID0_RD(src)                  (((src) & 0x00000002)>>1)
#define SEC_DEALLOCDOMAINID0_WR(src)             (((u32)(src)<<1) & 0x00000002)
#define SEC_DEALLOCDOMAINID0_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields CMDomainID	 */
#define SEC_CMDOMAINID0_WIDTH                                                 1
#define SEC_CMDOMAINID0_SHIFT                                                 0
#define SEC_CMDOMAINID0_MASK                                         0x00000001
#define SEC_CMDOMAINID0_RD(src)                          (((src) & 0x00000001))
#define SEC_CMDOMAINID0_WR(src)                     (((u32)(src)) & 0x00000001)
#define SEC_CMDOMAINID0_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register CfgSsQMIDomVal1	*/ 
/*	 Fields FPDomainID	 */
#define SEC_FPDOMAINID1_WIDTH                                                18
#define SEC_FPDOMAINID1_SHIFT                                                 3
#define SEC_FPDOMAINID1_MASK                                         0x001ffff8
#define SEC_FPDOMAINID1_RD(src)                       (((src) & 0x001ffff8)>>3)
#define SEC_FPDOMAINID1_WR(src)                  (((u32)(src)<<3) & 0x001ffff8)
#define SEC_FPDOMAINID1_SET(dst,src) \
                       (((dst) & ~0x001ffff8) | (((u32)(src)<<3) & 0x001ffff8))
/*	 Fields DecDomainID	 */
#define SEC_DECDOMAINID1_WIDTH                                                1
#define SEC_DECDOMAINID1_SHIFT                                                2
#define SEC_DECDOMAINID1_MASK                                        0x00000004
#define SEC_DECDOMAINID1_RD(src)                      (((src) & 0x00000004)>>2)
#define SEC_DECDOMAINID1_WR(src)                 (((u32)(src)<<2) & 0x00000004)
#define SEC_DECDOMAINID1_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields DeallocDomainID	 */
#define SEC_DEALLOCDOMAINID1_WIDTH                                            1
#define SEC_DEALLOCDOMAINID1_SHIFT                                            1
#define SEC_DEALLOCDOMAINID1_MASK                                    0x00000002
#define SEC_DEALLOCDOMAINID1_RD(src)                  (((src) & 0x00000002)>>1)
#define SEC_DEALLOCDOMAINID1_WR(src)             (((u32)(src)<<1) & 0x00000002)
#define SEC_DEALLOCDOMAINID1_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields CMDomainID	 */
#define SEC_CMDOMAINID1_WIDTH                                                 1
#define SEC_CMDOMAINID1_SHIFT                                                 0
#define SEC_CMDOMAINID1_MASK                                         0x00000001
#define SEC_CMDOMAINID1_RD(src)                          (((src) & 0x00000001))
#define SEC_CMDOMAINID1_WR(src)                     (((u32)(src)) & 0x00000001)
#define SEC_CMDOMAINID1_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register CfgSsQMIDomVal2	*/ 
/*	 Fields FPDomainID	 */
#define SEC_FPDOMAINID2_WIDTH                                                18
#define SEC_FPDOMAINID2_SHIFT                                                 3
#define SEC_FPDOMAINID2_MASK                                         0x001ffff8
#define SEC_FPDOMAINID2_RD(src)                       (((src) & 0x001ffff8)>>3)
#define SEC_FPDOMAINID2_WR(src)                  (((u32)(src)<<3) & 0x001ffff8)
#define SEC_FPDOMAINID2_SET(dst,src) \
                       (((dst) & ~0x001ffff8) | (((u32)(src)<<3) & 0x001ffff8))
/*	 Fields DecDomainID	 */
#define SEC_DECDOMAINID2_WIDTH                                                1
#define SEC_DECDOMAINID2_SHIFT                                                2
#define SEC_DECDOMAINID2_MASK                                        0x00000004
#define SEC_DECDOMAINID2_RD(src)                      (((src) & 0x00000004)>>2)
#define SEC_DECDOMAINID2_WR(src)                 (((u32)(src)<<2) & 0x00000004)
#define SEC_DECDOMAINID2_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields DeallocDomainID	 */
#define SEC_DEALLOCDOMAINID2_WIDTH                                            1
#define SEC_DEALLOCDOMAINID2_SHIFT                                            1
#define SEC_DEALLOCDOMAINID2_MASK                                    0x00000002
#define SEC_DEALLOCDOMAINID2_RD(src)                  (((src) & 0x00000002)>>1)
#define SEC_DEALLOCDOMAINID2_WR(src)             (((u32)(src)<<1) & 0x00000002)
#define SEC_DEALLOCDOMAINID2_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields CMDomainID	 */
#define SEC_CMDOMAINID2_WIDTH                                                 1
#define SEC_CMDOMAINID2_SHIFT                                                 0
#define SEC_CMDOMAINID2_MASK                                         0x00000001
#define SEC_CMDOMAINID2_RD(src)                          (((src) & 0x00000001))
#define SEC_CMDOMAINID2_WR(src)                     (((u32)(src)) & 0x00000001)
#define SEC_CMDOMAINID2_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Global Base Address	*/
#define SEC_EIP96_AXI_CSR_BASE_ADDR			0xddd832800ULL
#define SEC_XTS_AXI_CSR_BASE_ADDR			0xddd831000ULL

/*    Address SEC_XTS_AXI_CSR  Registers */
#define CSR_SEC_CFG_ADDR                                             0x00000000
#define CSR_SEC_CFG_DEFAULT                                          0x00000000
#define CSR_AXI_RD_MAX_BRST_CFG_ADDR                                 0x00000004
#define CSR_AXI_RD_MAX_BRST_CFG_DEFAULT                              0x00000000
#define CSR_AXI_WR_MAX_BRST_CFG_ADDR                                 0x00000008
#define CSR_AXI_WR_MAX_BRST_CFG_DEFAULT                              0x00000000
#define CSR_AXI_RD_MAX_OUTSTANDING_CFG_ADDR                          0x0000000c
#define CSR_AXI_RD_MAX_OUTSTANDING_CFG_DEFAULT                       0x11110000
#define CSR_AXI_WR_MAX_OUTSTANDING_CFG_ADDR                          0x00000010
#define CSR_AXI_WR_MAX_OUTSTANDING_CFG_DEFAULT                       0x11100000
#define CSR_SEC_INT_STS_ADDR                                         0x00000014
#define CSR_SEC_INT_STS_DEFAULT                                      0x00000000
#define CSR_SEC_INT_STSMASK_ADDR                                     0x00000018
#define CSR_AXI_SEC_FIFO_STS_ADDR                                    0x0000001c
#define CSR_AXI_SEC_FIFO_STS_DEFAULT                                 0xffc00000
#define CSR_MAX_PKT_TIMEOUT_CTRL_ADDR                                0x00000020
#define CSR_MAX_PKT_TIMEOUT_CTRL_DEFAULT                             0x7fffffff
#define CSR_MAX_FPBAVL_TIMEOUT_CTRL_ADDR                             0x00000024
#define CSR_MAX_FPBAVL_TIMEOUT_CTRL_DEFAULT                          0x7fffffff
#define CSR_MISC_CTRL_ADDR                                           0x00000028
#define CSR_MISC_CTRL_DEFAULT                                        0x80000000

/*	Register csr_sec_cfg	*/ 
/*	 Fields go	 */
#define GO_WIDTH                                                              1
#define GO_SHIFT                                                             31
#define GO_MASK                                                      0x80000000
#define GO_RD(src)                                   (((src) & 0x80000000)>>31)
#define GO_WR(src)                              (((u32)(src)<<31) & 0x80000000)
#define GO_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))

/*	Register csr_axi_rd_max_brst_cfg	*/ 
/*	 Fields tkn_rd	 */
#define TKN_RD_WIDTH                                                          2
#define TKN_RD_SHIFT                                                         30
#define TKN_RD_MASK                                                  0xc0000000
#define TKN_RD_RD(src)                               (((src) & 0xc0000000)>>30)
#define TKN_RD_WR(src)                          (((u32)(src)<<30) & 0xc0000000)
#define TKN_RD_SET(dst,src) \
                      (((dst) & ~0xc0000000) | (((u32)(src)<<30) & 0xc0000000))
/*	 Fields ctx_rd	 */
#define CTX_RD_WIDTH                                                          2
#define CTX_RD_SHIFT                                                         28
#define CTX_RD_MASK                                                  0x30000000
#define CTX_RD_RD(src)                               (((src) & 0x30000000)>>28)
#define CTX_RD_WR(src)                          (((u32)(src)<<28) & 0x30000000)
#define CTX_RD_SET(dst,src) \
                      (((dst) & ~0x30000000) | (((u32)(src)<<28) & 0x30000000))
/*	 Fields data_rd	 */
#define DATA_RD_WIDTH                                                         2
#define DATA_RD_SHIFT                                                        26
#define DATA_RD_MASK                                                 0x0c000000
#define DATA_RD_RD(src)                              (((src) & 0x0c000000)>>26)
#define DATA_RD_WR(src)                         (((u32)(src)<<26) & 0x0c000000)
#define DATA_RD_SET(dst,src) \
                      (((dst) & ~0x0c000000) | (((u32)(src)<<26) & 0x0c000000))
/*	 Fields dstll_rd	 */
#define DSTLL_RD_WIDTH                                                        2
#define DSTLL_RD_SHIFT                                                       24
#define DSTLL_RD_MASK                                                0x03000000
#define DSTLL_RD_RD(src)                             (((src) & 0x03000000)>>24)
#define DSTLL_RD_WR(src)                        (((u32)(src)<<24) & 0x03000000)
#define DSTLL_RD_SET(dst,src) \
                      (((dst) & ~0x03000000) | (((u32)(src)<<24) & 0x03000000))

/*	Register csr_axi_wr_max_brst_cfg	*/ 
/*	 Fields tkn_wr	 */
#define TKN_WR_WIDTH                                                          2
#define TKN_WR_SHIFT                                                         30
#define TKN_WR_MASK                                                  0xc0000000
#define TKN_WR_RD(src)                               (((src) & 0xc0000000)>>30)
#define TKN_WR_WR(src)                          (((u32)(src)<<30) & 0xc0000000)
#define TKN_WR_SET(dst,src) \
                      (((dst) & ~0xc0000000) | (((u32)(src)<<30) & 0xc0000000))
/*	 Fields ctx_wr	 */
#define CTX_WR_WIDTH                                                          2
#define CTX_WR_SHIFT                                                         28
#define CTX_WR_MASK                                                  0x30000000
#define CTX_WR_RD(src)                               (((src) & 0x30000000)>>28)
#define CTX_WR_WR(src)                          (((u32)(src)<<28) & 0x30000000)
#define CTX_WR_SET(dst,src) \
                      (((dst) & ~0x30000000) | (((u32)(src)<<28) & 0x30000000))
/*	 Fields data_wr	 */
#define DATA_WR_WIDTH                                                         2
#define DATA_WR_SHIFT                                                        26
#define DATA_WR_MASK                                                 0x0c000000
#define DATA_WR_RD(src)                              (((src) & 0x0c000000)>>26)
#define DATA_WR_WR(src)                         (((u32)(src)<<26) & 0x0c000000)
#define DATA_WR_SET(dst,src) \
                      (((dst) & ~0x0c000000) | (((u32)(src)<<26) & 0x0c000000))

/*	Register csr_axi_rd_max_outstanding_cfg	*/ 
/*	 Fields tkn_rd	 */
#define TKN_RD_F1_WIDTH                                                       4
#define TKN_RD_F1_SHIFT                                                      28
#define TKN_RD_F1_MASK                                               0xf0000000
#define TKN_RD_F1_RD(src)                            (((src) & 0xf0000000)>>28)
#define TKN_RD_F1_WR(src)                       (((u32)(src)<<28) & 0xf0000000)
#define TKN_RD_F1_SET(dst,src) \
                      (((dst) & ~0xf0000000) | (((u32)(src)<<28) & 0xf0000000))
/*	 Fields ctx_rd	 */
#define CTX_RD_F1_WIDTH                                                       4
#define CTX_RD_F1_SHIFT                                                      24
#define CTX_RD_F1_MASK                                               0x0f000000
#define CTX_RD_F1_RD(src)                            (((src) & 0x0f000000)>>24)
#define CTX_RD_F1_WR(src)                       (((u32)(src)<<24) & 0x0f000000)
#define CTX_RD_F1_SET(dst,src) \
                      (((dst) & ~0x0f000000) | (((u32)(src)<<24) & 0x0f000000))
/*	 Fields data_rd	 */
#define DATA_RD_F1_WIDTH                                                      4
#define DATA_RD_F1_SHIFT                                                     20
#define DATA_RD_F1_MASK                                              0x00f00000
#define DATA_RD_F1_RD(src)                           (((src) & 0x00f00000)>>20)
#define DATA_RD_F1_WR(src)                      (((u32)(src)<<20) & 0x00f00000)
#define DATA_RD_F1_SET(dst,src) \
                      (((dst) & ~0x00f00000) | (((u32)(src)<<20) & 0x00f00000))
/*	 Fields dstll_rd	 */
#define DSTLL_RD_F1_WIDTH                                                     4
#define DSTLL_RD_F1_SHIFT                                                    16
#define DSTLL_RD_F1_MASK                                             0x000f0000
#define DSTLL_RD_F1_RD(src)                          (((src) & 0x000f0000)>>16)
#define DSTLL_RD_F1_WR(src)                     (((u32)(src)<<16) & 0x000f0000)
#define DSTLL_RD_F1_SET(dst,src) \
                      (((dst) & ~0x000f0000) | (((u32)(src)<<16) & 0x000f0000))

/*	Register csr_axi_wr_max_outstanding_cfg	*/ 
/*	 Fields tkn_wr	 */
#define TKN_WR_F1_WIDTH                                                       4
#define TKN_WR_F1_SHIFT                                                      28
#define TKN_WR_F1_MASK                                               0xf0000000
#define TKN_WR_F1_RD(src)                            (((src) & 0xf0000000)>>28)
#define TKN_WR_F1_WR(src)                       (((u32)(src)<<28) & 0xf0000000)
#define TKN_WR_F1_SET(dst,src) \
                      (((dst) & ~0xf0000000) | (((u32)(src)<<28) & 0xf0000000))
/*	 Fields ctx_wr	 */
#define CTX_WR_F1_WIDTH                                                       4
#define CTX_WR_F1_SHIFT                                                      24
#define CTX_WR_F1_MASK                                               0x0f000000
#define CTX_WR_F1_RD(src)                            (((src) & 0x0f000000)>>24)
#define CTX_WR_F1_WR(src)                       (((u32)(src)<<24) & 0x0f000000)
#define CTX_WR_F1_SET(dst,src) \
                      (((dst) & ~0x0f000000) | (((u32)(src)<<24) & 0x0f000000))
/*	 Fields data_wr	 */
#define DATA_WR_F1_WIDTH                                                      4
#define DATA_WR_F1_SHIFT                                                     20
#define DATA_WR_F1_MASK                                              0x00f00000
#define DATA_WR_F1_RD(src)                           (((src) & 0x00f00000)>>20)
#define DATA_WR_F1_WR(src)                      (((u32)(src)<<20) & 0x00f00000)
#define DATA_WR_F1_SET(dst,src) \
                      (((dst) & ~0x00f00000) | (((u32)(src)<<20) & 0x00f00000))

/*	Register csr_sec_int_sts	*/ 
/*	 Fields eip96_core	 */
#define EIP96_CORE_WIDTH                                                      1
#define EIP96_CORE_SHIFT                                                     31
#define EIP96_CORE_MASK                                              0x80000000
#define EIP96_CORE_RD(src)                           (((src) & 0x80000000)>>31)
#define EIP96_CORE_WR(src)                      (((u32)(src)<<31) & 0x80000000)
#define EIP96_CORE_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields tkn_rd	 */
#define TKN_RD_F2_WIDTH                                                       1
#define TKN_RD_F2_SHIFT                                                      30
#define TKN_RD_F2_MASK                                               0x40000000
#define TKN_RD_F2_RD(src)                            (((src) & 0x40000000)>>30)
#define TKN_RD_F2_WR(src)                       (((u32)(src)<<30) & 0x40000000)
#define TKN_RD_F2_SET(dst,src) \
                      (((dst) & ~0x40000000) | (((u32)(src)<<30) & 0x40000000))
/*	 Fields ctx_rd	 */
#define CTX_RD_F2_WIDTH                                                       1
#define CTX_RD_F2_SHIFT                                                      29
#define CTX_RD_F2_MASK                                               0x20000000
#define CTX_RD_F2_RD(src)                            (((src) & 0x20000000)>>29)
#define CTX_RD_F2_WR(src)                       (((u32)(src)<<29) & 0x20000000)
#define CTX_RD_F2_SET(dst,src) \
                      (((dst) & ~0x20000000) | (((u32)(src)<<29) & 0x20000000))
/*	 Fields data_rd	 */
#define DATA_RD_F2_WIDTH                                                      1
#define DATA_RD_F2_SHIFT                                                     28
#define DATA_RD_F2_MASK                                              0x10000000
#define DATA_RD_F2_RD(src)                           (((src) & 0x10000000)>>28)
#define DATA_RD_F2_WR(src)                      (((u32)(src)<<28) & 0x10000000)
#define DATA_RD_F2_SET(dst,src) \
                      (((dst) & ~0x10000000) | (((u32)(src)<<28) & 0x10000000))
/*	 Fields dstll_rd	 */
#define DSTLL_RD_F2_WIDTH                                                     1
#define DSTLL_RD_F2_SHIFT                                                    27
#define DSTLL_RD_F2_MASK                                             0x08000000
#define DSTLL_RD_F2_RD(src)                          (((src) & 0x08000000)>>27)
#define DSTLL_RD_F2_WR(src)                     (((u32)(src)<<27) & 0x08000000)
#define DSTLL_RD_F2_SET(dst,src) \
                      (((dst) & ~0x08000000) | (((u32)(src)<<27) & 0x08000000))
/*	 Fields tkn_wr	 */
#define TKN_WR_F2_WIDTH                                                       1
#define TKN_WR_F2_SHIFT                                                      26
#define TKN_WR_F2_MASK                                               0x04000000
#define TKN_WR_F2_RD(src)                            (((src) & 0x04000000)>>26)
#define TKN_WR_F2_WR(src)                       (((u32)(src)<<26) & 0x04000000)
#define TKN_WR_F2_SET(dst,src) \
                      (((dst) & ~0x04000000) | (((u32)(src)<<26) & 0x04000000))
/*	 Fields ctx_wr	 */
#define CTX_WR_F2_WIDTH                                                       1
#define CTX_WR_F2_SHIFT                                                      25
#define CTX_WR_F2_MASK                                               0x02000000
#define CTX_WR_F2_RD(src)                            (((src) & 0x02000000)>>25)
#define CTX_WR_F2_WR(src)                       (((u32)(src)<<25) & 0x02000000)
#define CTX_WR_F2_SET(dst,src) \
                      (((dst) & ~0x02000000) | (((u32)(src)<<25) & 0x02000000))
/*	 Fields data_wr	 */
#define DATA_WR_F2_WIDTH                                                      1
#define DATA_WR_F2_SHIFT                                                     24
#define DATA_WR_F2_MASK                                              0x01000000
#define DATA_WR_F2_RD(src)                           (((src) & 0x01000000)>>24)
#define DATA_WR_F2_WR(src)                      (((u32)(src)<<24) & 0x01000000)
#define DATA_WR_F2_SET(dst,src) \
                      (((dst) & ~0x01000000) | (((u32)(src)<<24) & 0x01000000))
/*	 Fields lerr	 */
#define LERR_WIDTH                                                            1
#define LERR_SHIFT                                                           23
#define LERR_MASK                                                    0x00800000
#define LERR_RD(src)                                 (((src) & 0x00800000)>>23)
#define LERR_WR(src)                            (((u32)(src)<<23) & 0x00800000)
#define LERR_SET(dst,src) \
                      (((dst) & ~0x00800000) | (((u32)(src)<<23) & 0x00800000))

/*	Register csr_sec_int_stsMask	*/
/*    Mask Register Fields eip96_coreMask    */
#define EIP96_COREMASK_WIDTH                                                  1
#define EIP96_COREMASK_SHIFT                                                 31
#define EIP96_COREMASK_MASK                                          0x80000000
#define EIP96_COREMASK_RD(src)                       (((src) & 0x80000000)>>31)
#define EIP96_COREMASK_WR(src)                  (((u32)(src)<<31) & 0x80000000)
#define EIP96_COREMASK_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*    Mask Register Fields tkn_rdMask    */
#define TKN_RDMASK_WIDTH                                                      1
#define TKN_RDMASK_SHIFT                                                     30
#define TKN_RDMASK_MASK                                              0x40000000
#define TKN_RDMASK_RD(src)                           (((src) & 0x40000000)>>30)
#define TKN_RDMASK_WR(src)                      (((u32)(src)<<30) & 0x40000000)
#define TKN_RDMASK_SET(dst,src) \
                      (((dst) & ~0x40000000) | (((u32)(src)<<30) & 0x40000000))
/*    Mask Register Fields ctx_rdMask    */
#define CTX_RDMASK_WIDTH                                                      1
#define CTX_RDMASK_SHIFT                                                     29
#define CTX_RDMASK_MASK                                              0x20000000
#define CTX_RDMASK_RD(src)                           (((src) & 0x20000000)>>29)
#define CTX_RDMASK_WR(src)                      (((u32)(src)<<29) & 0x20000000)
#define CTX_RDMASK_SET(dst,src) \
                      (((dst) & ~0x20000000) | (((u32)(src)<<29) & 0x20000000))
/*    Mask Register Fields data_rdMask    */
#define DATA_RDMASK_WIDTH                                                     1
#define DATA_RDMASK_SHIFT                                                    28
#define DATA_RDMASK_MASK                                             0x10000000
#define DATA_RDMASK_RD(src)                          (((src) & 0x10000000)>>28)
#define DATA_RDMASK_WR(src)                     (((u32)(src)<<28) & 0x10000000)
#define DATA_RDMASK_SET(dst,src) \
                      (((dst) & ~0x10000000) | (((u32)(src)<<28) & 0x10000000))
/*    Mask Register Fields dstll_rdMask    */
#define DSTLL_RDMASK_WIDTH                                                    1
#define DSTLL_RDMASK_SHIFT                                                   27
#define DSTLL_RDMASK_MASK                                            0x08000000
#define DSTLL_RDMASK_RD(src)                         (((src) & 0x08000000)>>27)
#define DSTLL_RDMASK_WR(src)                    (((u32)(src)<<27) & 0x08000000)
#define DSTLL_RDMASK_SET(dst,src) \
                      (((dst) & ~0x08000000) | (((u32)(src)<<27) & 0x08000000))
/*    Mask Register Fields tkn_wrMask    */
#define TKN_WRMASK_WIDTH                                                      1
#define TKN_WRMASK_SHIFT                                                     26
#define TKN_WRMASK_MASK                                              0x04000000
#define TKN_WRMASK_RD(src)                           (((src) & 0x04000000)>>26)
#define TKN_WRMASK_WR(src)                      (((u32)(src)<<26) & 0x04000000)
#define TKN_WRMASK_SET(dst,src) \
                      (((dst) & ~0x04000000) | (((u32)(src)<<26) & 0x04000000))
/*    Mask Register Fields ctx_wrMask    */
#define CTX_WRMASK_WIDTH                                                      1
#define CTX_WRMASK_SHIFT                                                     25
#define CTX_WRMASK_MASK                                              0x02000000
#define CTX_WRMASK_RD(src)                           (((src) & 0x02000000)>>25)
#define CTX_WRMASK_WR(src)                      (((u32)(src)<<25) & 0x02000000)
#define CTX_WRMASK_SET(dst,src) \
                      (((dst) & ~0x02000000) | (((u32)(src)<<25) & 0x02000000))
/*    Mask Register Fields data_wrMask    */
#define DATA_WRMASK_WIDTH                                                     1
#define DATA_WRMASK_SHIFT                                                    24
#define DATA_WRMASK_MASK                                             0x01000000
#define DATA_WRMASK_RD(src)                          (((src) & 0x01000000)>>24)
#define DATA_WRMASK_WR(src)                     (((u32)(src)<<24) & 0x01000000)
#define DATA_WRMASK_SET(dst,src) \
                      (((dst) & ~0x01000000) | (((u32)(src)<<24) & 0x01000000))
/*    Mask Register Fields lerrMask    */
#define LERRMASK_WIDTH                                                        1
#define LERRMASK_SHIFT                                                       23
#define LERRMASK_MASK                                                0x00800000
#define LERRMASK_RD(src)                             (((src) & 0x00800000)>>23)
#define LERRMASK_WR(src)                        (((u32)(src)<<23) & 0x00800000)
#define LERRMASK_SET(dst,src) \
                      (((dst) & ~0x00800000) | (((u32)(src)<<23) & 0x00800000))

/*	Register csr_sec_fifo_sts	*/ 
/*	 Fields crcf_empty	 */
#define CRCF_EMPTY_WIDTH                                                      1
#define CRCF_EMPTY_SHIFT                                                     31
#define CRCF_EMPTY_MASK                                              0x80000000
#define CRCF_EMPTY_RD(src)                           (((src) & 0x80000000)>>31)
#define CRCF_EMPTY_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields cwcf_empty	 */
#define CWCF_EMPTY_WIDTH                                                      1
#define CWCF_EMPTY_SHIFT                                                     30
#define CWCF_EMPTY_MASK                                              0x40000000
#define CWCF_EMPTY_RD(src)                           (((src) & 0x40000000)>>30)
#define CWCF_EMPTY_SET(dst,src) \
                      (((dst) & ~0x40000000) | (((u32)(src)<<30) & 0x40000000))
/*	 Fields cwf_empty	 */
#define CWF_EMPTY_WIDTH                                                       1
#define CWF_EMPTY_SHIFT                                                      29
#define CWF_EMPTY_MASK                                               0x20000000
#define CWF_EMPTY_RD(src)                            (((src) & 0x20000000)>>29)
#define CWF_EMPTY_SET(dst,src) \
                      (((dst) & ~0x20000000) | (((u32)(src)<<29) & 0x20000000))
/*	 Fields trcf_empty	 */
#define TRCF_EMPTY_WIDTH                                                      1
#define TRCF_EMPTY_SHIFT                                                     28
#define TRCF_EMPTY_MASK                                              0x10000000
#define TRCF_EMPTY_RD(src)                           (((src) & 0x10000000)>>28)
#define TRCF_EMPTY_SET(dst,src) \
                      (((dst) & ~0x10000000) | (((u32)(src)<<28) & 0x10000000))
/*	 Fields twcf_empty	 */
#define TWCF_EMPTY_WIDTH                                                      1
#define TWCF_EMPTY_SHIFT                                                     27
#define TWCF_EMPTY_MASK                                              0x08000000
#define TWCF_EMPTY_RD(src)                           (((src) & 0x08000000)>>27)
#define TWCF_EMPTY_SET(dst,src) \
                      (((dst) & ~0x08000000) | (((u32)(src)<<27) & 0x08000000))
/*	 Fields twdf_empty	 */
#define TWDF_EMPTY_WIDTH                                                      1
#define TWDF_EMPTY_SHIFT                                                     26
#define TWDF_EMPTY_MASK                                              0x04000000
#define TWDF_EMPTY_RD(src)                           (((src) & 0x04000000)>>26)
#define TWDF_EMPTY_SET(dst,src) \
                      (((dst) & ~0x04000000) | (((u32)(src)<<26) & 0x04000000))
/*	 Fields dwszf_empty	 */
#define DWSZF_EMPTY_WIDTH                                                     1
#define DWSZF_EMPTY_SHIFT                                                    25
#define DWSZF_EMPTY_MASK                                             0x02000000
#define DWSZF_EMPTY_RD(src)                          (((src) & 0x02000000)>>25)
#define DWSZF_EMPTY_SET(dst,src) \
                      (((dst) & ~0x02000000) | (((u32)(src)<<25) & 0x02000000))
/*	 Fields drcf_empty	 */
#define DRCF_EMPTY_WIDTH                                                      1
#define DRCF_EMPTY_SHIFT                                                     24
#define DRCF_EMPTY_MASK                                              0x01000000
#define DRCF_EMPTY_RD(src)                           (((src) & 0x01000000)>>24)
#define DRCF_EMPTY_SET(dst,src) \
                      (((dst) & ~0x01000000) | (((u32)(src)<<24) & 0x01000000))
/*	 Fields dwcf_empty	 */
#define DWCF_EMPTY_WIDTH                                                      1
#define DWCF_EMPTY_SHIFT                                                     23
#define DWCF_EMPTY_MASK                                              0x00800000
#define DWCF_EMPTY_RD(src)                           (((src) & 0x00800000)>>23)
#define DWCF_EMPTY_SET(dst,src) \
                      (((dst) & ~0x00800000) | (((u32)(src)<<23) & 0x00800000))
/*	 Fields dwf_empty	 */
#define DWF_EMPTY_WIDTH                                                       1
#define DWF_EMPTY_SHIFT                                                      22
#define DWF_EMPTY_MASK                                               0x00400000
#define DWF_EMPTY_RD(src)                            (((src) & 0x00400000)>>22)
#define DWF_EMPTY_SET(dst,src) \
                      (((dst) & ~0x00400000) | (((u32)(src)<<22) & 0x00400000))

/*	Register csr_max_pkt_timeout_ctrl	*/ 
/*	 Fields enable	 */
#define SEC_ENABLE_WIDTH                                                      1
#define SEC_ENABLE_SHIFT                                                     31
#define SEC_ENABLE_MASK                                              0x80000000
#define SEC_ENABLE_RD(src)                           (((src) & 0x80000000)>>31)
#define SEC_ENABLE_WR(src)                      (((u32)(src)<<31) & 0x80000000)
#define SEC_ENABLE_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields cnt	 */
#define SEC_CNT_WIDTH                                                        31
#define SEC_CNT_SHIFT                                                         0
#define SEC_CNT_MASK                                                 0x7fffffff
#define SEC_CNT_RD(src)                                  (((src) & 0x7fffffff))
#define SEC_CNT_WR(src)                             (((u32)(src)) & 0x7fffffff)
#define SEC_CNT_SET(dst,src) \
                          (((dst) & ~0x7fffffff) | (((u32)(src)) & 0x7fffffff))

/*	Register csr_max_fpbavl_timeout_ctrl	*/ 
/*	 Fields enable	 */
#define SEC_ENABLE_F1_WIDTH                                                   1
#define SEC_ENABLE_F1_SHIFT                                                  31
#define SEC_ENABLE_F1_MASK                                           0x80000000
#define SEC_ENABLE_F1_RD(src)                        (((src) & 0x80000000)>>31)
#define SEC_ENABLE_F1_WR(src)                   (((u32)(src)<<31) & 0x80000000)
#define SEC_ENABLE_F1_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields cnt	 */
#define SEC_CNT_F1_WIDTH                                                     31
#define SEC_CNT_F1_SHIFT                                                      0
#define SEC_CNT_F1_MASK                                              0x7fffffff
#define SEC_CNT_F1_RD(src)                               (((src) & 0x7fffffff))
#define SEC_CNT_F1_WR(src)                          (((u32)(src)) & 0x7fffffff)
#define SEC_CNT_F1_SET(dst,src) \
                          (((dst) & ~0x7fffffff) | (((u32)(src)) & 0x7fffffff))

/*	Register csr_misc_ctrl	*/ 
/*	 Fields en_dealloc_buf_cnt	 */
#define EN_DEALLOC_BUF_CNT_WIDTH                                              1
#define EN_DEALLOC_BUF_CNT_SHIFT                                             31
#define EN_DEALLOC_BUF_CNT_MASK                                      0x80000000
#define EN_DEALLOC_BUF_CNT_RD(src)                   (((src) & 0x80000000)>>31)
#define EN_DEALLOC_BUF_CNT_WR(src)              (((u32)(src)<<31) & 0x80000000)
#define EN_DEALLOC_BUF_CNT_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))

/*	Global Base Address	*/
#define SEC_EIP96_CRYPTO_CSR_BASE_ADDR			0xddd833000ULL
#define SEC_XTS_CRYPTO_CSR_BASE_ADDR			0xddd831800ULL

/*    Address SEC_XTS_CRYPTO_CSR  Registers */
#define CSR_SEC_CRYPTO_CFG_0_ADDR                                    0x00000000
#define CSR_SEC_CRYPTO_CFG_0_DEFAULT                                 0x40000000
#define CSR_SEC_CRYPTO_CFG_1_ADDR                                    0x00000004
#define CSR_SEC_CRYPTO_CFG_1_DEFAULT                                 0xf0000000
#define CSR_SEC_CRYPTO_CFG_2_ADDR                                    0x00000008
#define CSR_SEC_CRYPTO_CFG_2_DEFAULT                                 0x00000000
#define CSR_SEC_FIFO_STS_ADDR                                        0x0000000c
#define CSR_SEC_FIFO_STS_DEFAULT                                     0xf0000000
#define CSR_SEC_EIP96_ERR_MASK_ADDR                                  0x00000010
#define CSR_SEC_EIP96_ERR_MASK_DEFAULT                               0xfffe0000

/*	Register csr_sec_crypto_cfg_0	*/ 
/*	 Fields tkn_rd_prefetch_size	 */
#define TKN_RD_PREFETCH_SIZE0_WIDTH                                           4
#define TKN_RD_PREFETCH_SIZE0_SHIFT                                          28
#define TKN_RD_PREFETCH_SIZE0_MASK                                   0xf0000000
#define TKN_RD_PREFETCH_SIZE0_RD(src)                (((src) & 0xf0000000)>>28)
#define TKN_RD_PREFETCH_SIZE0_WR(src)           (((u32)(src)<<28) & 0xf0000000)
#define TKN_RD_PREFETCH_SIZE0_SET(dst,src) \
                      (((dst) & ~0xf0000000) | (((u32)(src)<<28) & 0xf0000000))
/*	 Fields tkn_rd_offset_size	 */
#define TKN_RD_OFFSET_SIZE0_WIDTH                                             4
#define TKN_RD_OFFSET_SIZE0_SHIFT                                            24
#define TKN_RD_OFFSET_SIZE0_MASK                                     0x0f000000
#define TKN_RD_OFFSET_SIZE0_RD(src)                  (((src) & 0x0f000000)>>24)
#define TKN_RD_OFFSET_SIZE0_WR(src)             (((u32)(src)<<24) & 0x0f000000)
#define TKN_RD_OFFSET_SIZE0_SET(dst,src) \
                      (((dst) & ~0x0f000000) | (((u32)(src)<<24) & 0x0f000000))

/*	Register csr_sec_crypto_cfg_1	*/ 
/*	 Fields dis_wait_tkn_wr_done	 */
#define DIS_WAIT_TKN_WR_DONE1_WIDTH                                           1
#define DIS_WAIT_TKN_WR_DONE1_SHIFT                                          31
#define DIS_WAIT_TKN_WR_DONE1_MASK                                   0x80000000
#define DIS_WAIT_TKN_WR_DONE1_RD(src)                (((src) & 0x80000000)>>31)
#define DIS_WAIT_TKN_WR_DONE1_WR(src)           (((u32)(src)<<31) & 0x80000000)
#define DIS_WAIT_TKN_WR_DONE1_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields dis_ctx_interlock	 */
#define DIS_CTX_INTERLOCK1_WIDTH                                              1
#define DIS_CTX_INTERLOCK1_SHIFT                                             30
#define DIS_CTX_INTERLOCK1_MASK                                      0x40000000
#define DIS_CTX_INTERLOCK1_RD(src)                   (((src) & 0x40000000)>>30)
#define DIS_CTX_INTERLOCK1_WR(src)              (((u32)(src)<<30) & 0x40000000)
#define DIS_CTX_INTERLOCK1_SET(dst,src) \
                      (((dst) & ~0x40000000) | (((u32)(src)<<30) & 0x40000000))
/*	 Fields dis_zero_length_hndl	 */
#define DIS_ZERO_LENGTH_HNDL1_WIDTH                                           1
#define DIS_ZERO_LENGTH_HNDL1_SHIFT                                          29
#define DIS_ZERO_LENGTH_HNDL1_MASK                                   0x20000000
#define DIS_ZERO_LENGTH_HNDL1_RD(src)                (((src) & 0x20000000)>>29)
#define DIS_ZERO_LENGTH_HNDL1_WR(src)           (((u32)(src)<<29) & 0x20000000)
#define DIS_ZERO_LENGTH_HNDL1_SET(dst,src) \
                      (((dst) & ~0x20000000) | (((u32)(src)<<29) & 0x20000000))
/*	 Fields en_adv_dma_in_err_hndl	 */
#define EN_ADV_DMA_IN_ERR_HNDL1_WIDTH                                         1
#define EN_ADV_DMA_IN_ERR_HNDL1_SHIFT                                        28
#define EN_ADV_DMA_IN_ERR_HNDL1_MASK                                 0x10000000
#define EN_ADV_DMA_IN_ERR_HNDL1_RD(src)              (((src) & 0x10000000)>>28)
#define EN_ADV_DMA_IN_ERR_HNDL1_WR(src)         (((u32)(src)<<28) & 0x10000000)
#define EN_ADV_DMA_IN_ERR_HNDL1_SET(dst,src) \
                      (((dst) & ~0x10000000) | (((u32)(src)<<28) & 0x10000000))

/*	Register csr_sec_crypto_cfg_2	*/ 
/*	 Fields en_ib_rma	 */
#define EN_IB_RMA2_WIDTH                                                      1
#define EN_IB_RMA2_SHIFT                                                     31
#define EN_IB_RMA2_MASK                                              0x80000000
#define EN_IB_RMA2_RD(src)                           (((src) & 0x80000000)>>31)
#define EN_IB_RMA2_WR(src)                      (((u32)(src)<<31) & 0x80000000)
#define EN_IB_RMA2_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields ib_rma_ctrl	 */
#define IB_RMA_CTRL2_WIDTH                                                    2
#define IB_RMA_CTRL2_SHIFT                                                   29
#define IB_RMA_CTRL2_MASK                                            0x60000000
#define IB_RMA_CTRL2_RD(src)                         (((src) & 0x60000000)>>29)
#define IB_RMA_CTRL2_WR(src)                    (((u32)(src)<<29) & 0x60000000)
#define IB_RMA_CTRL2_SET(dst,src) \
                      (((dst) & ~0x60000000) | (((u32)(src)<<29) & 0x60000000))
/*	 Fields en_ib_rmb	 */
#define EN_IB_RMB2_WIDTH                                                      1
#define EN_IB_RMB2_SHIFT                                                     28
#define EN_IB_RMB2_MASK                                              0x10000000
#define EN_IB_RMB2_RD(src)                           (((src) & 0x10000000)>>28)
#define EN_IB_RMB2_WR(src)                      (((u32)(src)<<28) & 0x10000000)
#define EN_IB_RMB2_SET(dst,src) \
                      (((dst) & ~0x10000000) | (((u32)(src)<<28) & 0x10000000))
/*	 Fields ib_rmb_ctrl	 */
#define IB_RMB_CTRL2_WIDTH                                                    2
#define IB_RMB_CTRL2_SHIFT                                                   26
#define IB_RMB_CTRL2_MASK                                            0x0c000000
#define IB_RMB_CTRL2_RD(src)                         (((src) & 0x0c000000)>>26)
#define IB_RMB_CTRL2_WR(src)                    (((u32)(src)<<26) & 0x0c000000)
#define IB_RMB_CTRL2_SET(dst,src) \
                      (((dst) & ~0x0c000000) | (((u32)(src)<<26) & 0x0c000000))
/*	 Fields en_ob_rma	 */
#define EN_OB_RMA2_WIDTH                                                      1
#define EN_OB_RMA2_SHIFT                                                     25
#define EN_OB_RMA2_MASK                                              0x02000000
#define EN_OB_RMA2_RD(src)                           (((src) & 0x02000000)>>25)
#define EN_OB_RMA2_WR(src)                      (((u32)(src)<<25) & 0x02000000)
#define EN_OB_RMA2_SET(dst,src) \
                      (((dst) & ~0x02000000) | (((u32)(src)<<25) & 0x02000000))
/*	 Fields ob_rma_ctrl	 */
#define OB_RMA_CTRL2_WIDTH                                                    2
#define OB_RMA_CTRL2_SHIFT                                                   23
#define OB_RMA_CTRL2_MASK                                            0x01800000
#define OB_RMA_CTRL2_RD(src)                         (((src) & 0x01800000)>>23)
#define OB_RMA_CTRL2_WR(src)                    (((u32)(src)<<23) & 0x01800000)
#define OB_RMA_CTRL2_SET(dst,src) \
                      (((dst) & ~0x01800000) | (((u32)(src)<<23) & 0x01800000))
/*	 Fields en_ob_rmb	 */
#define EN_OB_RMB2_WIDTH                                                      1
#define EN_OB_RMB2_SHIFT                                                     22
#define EN_OB_RMB2_MASK                                              0x00400000
#define EN_OB_RMB2_RD(src)                           (((src) & 0x00400000)>>22)
#define EN_OB_RMB2_WR(src)                      (((u32)(src)<<22) & 0x00400000)
#define EN_OB_RMB2_SET(dst,src) \
                      (((dst) & ~0x00400000) | (((u32)(src)<<22) & 0x00400000))
/*	 Fields ob_rmb_ctrl	 */
#define OB_RMB_CTRL2_WIDTH                                                    2
#define OB_RMB_CTRL2_SHIFT                                                   20
#define OB_RMB_CTRL2_MASK                                            0x00300000
#define OB_RMB_CTRL2_RD(src)                         (((src) & 0x00300000)>>20)
#define OB_RMB_CTRL2_WR(src)                    (((u32)(src)<<20) & 0x00300000)
#define OB_RMB_CTRL2_SET(dst,src) \
                      (((dst) & ~0x00300000) | (((u32)(src)<<20) & 0x00300000))

/*	Register csr_sec_fifo_sts	*/ 
/*	 Fields crf_empty	 */
#define CRF_EMPTY_WIDTH                                                       1
#define CRF_EMPTY_SHIFT                                                      31
#define CRF_EMPTY_MASK                                               0x80000000
#define CRF_EMPTY_RD(src)                            (((src) & 0x80000000)>>31)
#define CRF_EMPTY_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields trdf_empty	 */
#define TRDF_EMPTY_WIDTH                                                      1
#define TRDF_EMPTY_SHIFT                                                     30
#define TRDF_EMPTY_MASK                                              0x40000000
#define TRDF_EMPTY_RD(src)                           (((src) & 0x40000000)>>30)
#define TRDF_EMPTY_SET(dst,src) \
                      (((dst) & ~0x40000000) | (((u32)(src)<<30) & 0x40000000))
/*	 Fields drf_empty	 */
#define DRF_EMPTY_WIDTH                                                       1
#define DRF_EMPTY_SHIFT                                                      29
#define DRF_EMPTY_MASK                                               0x20000000
#define DRF_EMPTY_RD(src)                            (((src) & 0x20000000)>>29)
#define DRF_EMPTY_SET(dst,src) \
                      (((dst) & ~0x20000000) | (((u32)(src)<<29) & 0x20000000))
/*	 Fields qmi_wq_empty	 */
#define QMI_WQ_EMPTY_WIDTH                                                    1
#define QMI_WQ_EMPTY_SHIFT                                                   28
#define QMI_WQ_EMPTY_MASK                                            0x10000000
#define QMI_WQ_EMPTY_RD(src)                         (((src) & 0x10000000)>>28)
#define QMI_WQ_EMPTY_SET(dst,src) \
                      (((dst) & ~0x10000000) | (((u32)(src)<<28) & 0x10000000))

/*	Register csr_sec_eip96_err_mask	*/ 
/*	 Fields E0_E15	 */
#define E0_E15_WIDTH                                                         16
#define E0_E15_SHIFT                                                         16
#define E0_E15_MASK                                                  0xffff0000
#define E0_E15_RD(src)                               (((src) & 0xffff0000)>>16)
#define E0_E15_WR(src)                          (((u32)(src)<<16) & 0xffff0000)
#define E0_E15_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))

/*	Global Base Address	*/
#define SEC_XTS_CORE_CSR_BASE_ADDR			0xddd832000ULL

/*    Address SEC_XTS_CORE_CSR  Registers */
#define CSR_XTS_CONTROL_ADDR                                         0x00000000
#define CSR_XTS_CONTROL_DEFAULT                                      0x00000000
#define CSR_XTS_CONFIGURATION_ADDR                                   0x00000004
#define CSR_XTS_CONFIGURATION_DEFAULT                                0x00000000
#define CSR_XTS_STATUS_ADDR                                          0x00000008
#define CSR_XTS_STATUS_DEFAULT                                       0x00600000
#define CSR_XTS_TOKEN_CONTROL_WORD_ADDR                              0x0000000c
#define CSR_XTS_TOKEN_CONTROL_WORD_DEFAULT                           0x00000000
#define CSR_XTS_TOKEN_CONTEXT_POINTER_ADDR                           0x00000010
#define CSR_XTS_TOKEN_CONTEXT_POINTER_DEFAULT                        0x00000000
#define CSR_XTS_TOKEN_DMA_LENGTH_ADDR                                0x00000014
#define CSR_XTS_TOKEN_DMA_LENGTH_DEFAULT                             0x00000000
#define CSR_XTS_TOKEN_IV_DWORD0_ADDR                                 0x00000018
#define CSR_XTS_TOKEN_IV_DWORD0_DEFAULT                              0x00000000
#define CSR_XTS_TOKEN_IV_DWORD1_ADDR                                 0x0000001c
#define CSR_XTS_TOKEN_IV_DWORD1_DEFAULT                              0x00000000
#define CSR_XTS_TOKEN_IV_DWORD2_ADDR                                 0x00000020
#define CSR_XTS_TOKEN_IV_DWORD2_DEFAULT                              0x00000000
#define CSR_XTS_TOKEN_IV_DWORD3_ADDR                                 0x00000024
#define CSR_XTS_TOKEN_IV_DWORD3_DEFAULT                              0x00000000
#define CSR_XTS_CONTEXT_KEY1_DWORD0_ADDR                             0x00000028
#define CSR_XTS_CONTEXT_KEY1_DWORD0_DEFAULT                          0x00000000
#define CSR_XTS_CONTEXT_KEY1_DWORD1_ADDR                             0x0000002c
#define CSR_XTS_CONTEXT_KEY1_DWORD1_DEFAULT                          0x00000000
#define CSR_XTS_CONTEXT_KEY1_DWORD2_ADDR                             0x00000030
#define CSR_XTS_CONTEXT_KEY1_DWORD2_DEFAULT                          0x00000000
#define CSR_XTS_CONTEXT_KEY1_DWORD3_ADDR                             0x00000034
#define CSR_XTS_CONTEXT_KEY1_DWORD3_DEFAULT                          0x00000000
#define CSR_XTS_CONTEXT_KEY1_DWORD4_ADDR                             0x00000038
#define CSR_XTS_CONTEXT_KEY1_DWORD4_DEFAULT                          0x00000000
#define CSR_XTS_CONTEXT_KEY1_DWORD5_ADDR                             0x0000003c
#define CSR_XTS_CONTEXT_KEY1_DWORD5_DEFAULT                          0x00000000
#define CSR_XTS_CONTEXT_KEY1_DWORD6_ADDR                             0x00000040
#define CSR_XTS_CONTEXT_KEY1_DWORD6_DEFAULT                          0x00000000
#define CSR_XTS_CONTEXT_KEY1_DWORD7_ADDR                             0x00000044
#define CSR_XTS_CONTEXT_KEY1_DWORD7_DEFAULT                          0x00000000
#define CSR_XTS_CONTEXT_KEY2_DWORD0_ADDR                             0x00000048
#define CSR_XTS_CONTEXT_KEY2_DWORD0_DEFAULT                          0x00000000
#define CSR_XTS_CONTEXT_KEY2_DWORD1_ADDR                             0x0000004c
#define CSR_XTS_CONTEXT_KEY2_DWORD1_DEFAULT                          0x00000000
#define CSR_XTS_CONTEXT_KEY2_DWORD2_ADDR                             0x00000050
#define CSR_XTS_CONTEXT_KEY2_DWORD2_DEFAULT                          0x00000000
#define CSR_XTS_CONTEXT_KEY2_DWORD3_ADDR                             0x00000054
#define CSR_XTS_CONTEXT_KEY2_DWORD3_DEFAULT                          0x00000000
#define CSR_XTS_CONTEXT_KEY2_DWORD4_ADDR                             0x00000058
#define CSR_XTS_CONTEXT_KEY2_DWORD4_DEFAULT                          0x00000000
#define CSR_XTS_CONTEXT_KEY2_DWORD5_ADDR                             0x0000005c
#define CSR_XTS_CONTEXT_KEY2_DWORD5_DEFAULT                          0x00000000
#define CSR_XTS_CONTEXT_KEY2_DWORD6_ADDR                             0x00000060
#define CSR_XTS_CONTEXT_KEY2_DWORD6_DEFAULT                          0x00000000
#define CSR_XTS_CONTEXT_KEY2_DWORD7_ADDR                             0x00000064
#define CSR_XTS_CONTEXT_KEY2_DWORD7_DEFAULT                          0x00000000
#define CSR_XTS_DIN_XFR_COUNT_ADDR                                   0x00000068
#define CSR_XTS_DIN_XFR_COUNT_DEFAULT                                0x00000000
#define CSR_XTS_DOUT_XFR_COUNT_ADDR                                  0x0000006c
#define CSR_XTS_DOUT_XFR_COUNT_DEFAULT                               0x00000000
#define CSR_XTS_DEBUG_TRACE_ADDR                                     0x00000070
#define CSR_XTS_DEBUG_TRACE_DEFAULT                                  0x844c4000

/*	Register CSR_XTS_CONTROL	*/ 
/*	 Fields DEBUG_TRACE_SEL_RBIT	 */
#define DEBUG_TRACE_SEL_RBIT_WIDTH                                            2
#define DEBUG_TRACE_SEL_RBIT_SHIFT                                           16
#define DEBUG_TRACE_SEL_RBIT_MASK                                    0x00030000
#define DEBUG_TRACE_SEL_RBIT_RD(src)                 (((src) & 0x00030000)>>16)
#define DEBUG_TRACE_SEL_RBIT_WR(src)            (((u32)(src)<<16) & 0x00030000)
#define DEBUG_TRACE_SEL_RBIT_SET(dst,src) \
                      (((dst) & ~0x00030000) | (((u32)(src)<<16) & 0x00030000))
/*	 Fields HALT_REQ_RBIT	 */
#define HALT_REQ_RBIT_WIDTH                                                   1
#define HALT_REQ_RBIT_SHIFT                                                  10
#define HALT_REQ_RBIT_MASK                                           0x00000400
#define HALT_REQ_RBIT_RD(src)                        (((src) & 0x00000400)>>10)
#define HALT_REQ_RBIT_WR(src)                   (((u32)(src)<<10) & 0x00000400)
#define HALT_REQ_RBIT_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields PAUSE_ON_DONE_REQ_RBIT	 */
#define PAUSE_ON_DONE_REQ_RBIT_WIDTH                                          1
#define PAUSE_ON_DONE_REQ_RBIT_SHIFT                                          9
#define PAUSE_ON_DONE_REQ_RBIT_MASK                                  0x00000200
#define PAUSE_ON_DONE_REQ_RBIT_RD(src)                (((src) & 0x00000200)>>9)
#define PAUSE_ON_DONE_REQ_RBIT_WR(src)           (((u32)(src)<<9) & 0x00000200)
#define PAUSE_ON_DONE_REQ_RBIT_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields XTS_ERROR_CLR_RBIT	 */
#define XTS_ERROR_CLR_RBIT_WIDTH                                              1
#define XTS_ERROR_CLR_RBIT_SHIFT                                              1
#define XTS_ERROR_CLR_RBIT_MASK                                      0x00000002
#define XTS_ERROR_CLR_RBIT_RD(src)                    (((src) & 0x00000002)>>1)
#define XTS_ERROR_CLR_RBIT_WR(src)               (((u32)(src)<<1) & 0x00000002)
#define XTS_ERROR_CLR_RBIT_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields XTS_CORE_RST_RBIT	 */
#define XTS_CORE_RST_RBIT_WIDTH                                               1
#define XTS_CORE_RST_RBIT_SHIFT                                               0
#define XTS_CORE_RST_RBIT_MASK                                       0x00000001
#define XTS_CORE_RST_RBIT_RD(src)                        (((src) & 0x00000001))
#define XTS_CORE_RST_RBIT_WR(src)                   (((u32)(src)) & 0x00000001)
#define XTS_CORE_RST_RBIT_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register CSR_XTS_CONFIGURATION	*/ 
/*	 Fields NO_RESIDUAL_DATA_POST_DIN_ERROR_RBIT	 */
#define NO_RESIDUAL_DATA_POST_DIN_ERROR_RBIT_WIDTH                            1
#define NO_RESIDUAL_DATA_POST_DIN_ERROR_RBIT_SHIFT                            3
#define NO_RESIDUAL_DATA_POST_DIN_ERROR_RBIT_MASK                    0x00000008
#define NO_RESIDUAL_DATA_POST_DIN_ERROR_RBIT_RD(src)  (((src) & 0x00000008)>>3)
#define NO_RESIDUAL_DATA_POST_DIN_ERROR_RBIT_WR(src) \
                                                 (((u32)(src)<<3) & 0x00000008)
#define NO_RESIDUAL_DATA_POST_DIN_ERROR_RBIT_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields HALT_ON_ERROR_RBIT	 */
#define HALT_ON_ERROR_RBIT_WIDTH                                              1
#define HALT_ON_ERROR_RBIT_SHIFT                                              2
#define HALT_ON_ERROR_RBIT_MASK                                      0x00000004
#define HALT_ON_ERROR_RBIT_RD(src)                    (((src) & 0x00000004)>>2)
#define HALT_ON_ERROR_RBIT_WR(src)               (((u32)(src)<<2) & 0x00000004)
#define HALT_ON_ERROR_RBIT_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields ERROR_INTERRUPT_MASK_RBIT	 */
#define ERROR_INTERRUPT_MASK_RBIT_WIDTH                                       1
#define ERROR_INTERRUPT_MASK_RBIT_SHIFT                                       1
#define ERROR_INTERRUPT_MASK_RBIT_MASK                               0x00000002
#define ERROR_INTERRUPT_MASK_RBIT_RD(src)             (((src) & 0x00000002)>>1)
#define ERROR_INTERRUPT_MASK_RBIT_WR(src)        (((u32)(src)<<1) & 0x00000002)
#define ERROR_INTERRUPT_MASK_RBIT_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields CTS_MODE_ENABLE_RBIT	 */
#define SEC_CTS_MODE_ENABLE_RBIT_WIDTH                                        1
#define SEC_CTS_MODE_ENABLE_RBIT_SHIFT                                        0
#define SEC_CTS_MODE_ENABLE_RBIT_MASK                                0x00000001
#define SEC_CTS_MODE_ENABLE_RBIT_RD(src)                 (((src) & 0x00000001))
#define SEC_CTS_MODE_ENABLE_RBIT_WR(src)            (((u32)(src)) & 0x00000001)
#define SEC_CTS_MODE_ENABLE_RBIT_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register CSR_XTS_STATUS	*/ 
/*	 Fields XTS_IV_J_VALUE	 */
#define XTS_IV_J_VALUE_WIDTH                                                  8
#define XTS_IV_J_VALUE_SHIFT                                                 24
#define XTS_IV_J_VALUE_MASK                                          0xff000000
#define XTS_IV_J_VALUE_RD(src)                       (((src) & 0xff000000)>>24)
#define XTS_IV_J_VALUE_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))
/*	 Fields DOUT_XFR_DONE	 */
#define DOUT_XFR_DONE_WIDTH                                                   1
#define DOUT_XFR_DONE_SHIFT                                                  22
#define DOUT_XFR_DONE_MASK                                           0x00400000
#define DOUT_XFR_DONE_RD(src)                        (((src) & 0x00400000)>>22)
#define DOUT_XFR_DONE_SET(dst,src) \
                      (((dst) & ~0x00400000) | (((u32)(src)<<22) & 0x00400000))
/*	 Fields DIN_XFR_DONE	 */
#define DIN_XFR_DONE_WIDTH                                                    1
#define DIN_XFR_DONE_SHIFT                                                   21
#define DIN_XFR_DONE_MASK                                            0x00200000
#define DIN_XFR_DONE_RD(src)                         (((src) & 0x00200000)>>21)
#define DIN_XFR_DONE_SET(dst,src) \
                      (((dst) & ~0x00200000) | (((u32)(src)<<21) & 0x00200000))
/*	 Fields CRYPTO_DATA_COUNT	 */
#define CRYPTO_DATA_COUNT_WIDTH                                               5
#define CRYPTO_DATA_COUNT_SHIFT                                              16
#define CRYPTO_DATA_COUNT_MASK                                       0x001f0000
#define CRYPTO_DATA_COUNT_RD(src)                    (((src) & 0x001f0000)>>16)
#define CRYPTO_DATA_COUNT_SET(dst,src) \
                      (((dst) & ~0x001f0000) | (((u32)(src)<<16) & 0x001f0000))
/*	 Fields HALT_ACK_RBIT	 */
#define HALT_ACK_RBIT_WIDTH                                                   1
#define HALT_ACK_RBIT_SHIFT                                                   9
#define HALT_ACK_RBIT_MASK                                           0x00000200
#define HALT_ACK_RBIT_RD(src)                         (((src) & 0x00000200)>>9)
#define HALT_ACK_RBIT_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields PAUSE_ON_DONE_ACK_RBIT	 */
#define PAUSE_ON_DONE_ACK_RBIT_WIDTH                                          1
#define PAUSE_ON_DONE_ACK_RBIT_SHIFT                                          8
#define PAUSE_ON_DONE_ACK_RBIT_MASK                                  0x00000100
#define PAUSE_ON_DONE_ACK_RBIT_RD(src)                (((src) & 0x00000100)>>8)
#define PAUSE_ON_DONE_ACK_RBIT_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*	 Fields TKN_UNDERFLOW_ERROR	 */
#define TKN_UNDERFLOW_ERROR_WIDTH                                             1
#define TKN_UNDERFLOW_ERROR_SHIFT                                             5
#define TKN_UNDERFLOW_ERROR_MASK                                     0x00000020
#define TKN_UNDERFLOW_ERROR_RD(src)                   (((src) & 0x00000020)>>5)
#define TKN_UNDERFLOW_ERROR_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields CTX_OVERFLOW_ERROR	 */
#define CTX_OVERFLOW_ERROR_WIDTH                                              1
#define CTX_OVERFLOW_ERROR_SHIFT                                              4
#define CTX_OVERFLOW_ERROR_MASK                                      0x00000010
#define CTX_OVERFLOW_ERROR_RD(src)                    (((src) & 0x00000010)>>4)
#define CTX_OVERFLOW_ERROR_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields DIN_OVERFLOW_ERROR	 */
#define DIN_OVERFLOW_ERROR_WIDTH                                              1
#define DIN_OVERFLOW_ERROR_SHIFT                                              3
#define DIN_OVERFLOW_ERROR_MASK                                      0x00000008
#define DIN_OVERFLOW_ERROR_RD(src)                    (((src) & 0x00000008)>>3)
#define DIN_OVERFLOW_ERROR_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields DOUT_OVERFLOW_ERROR	 */
#define DOUT_OVERFLOW_ERROR_WIDTH                                             1
#define DOUT_OVERFLOW_ERROR_SHIFT                                             2
#define DOUT_OVERFLOW_ERROR_MASK                                     0x00000004
#define DOUT_OVERFLOW_ERROR_RD(src)                   (((src) & 0x00000004)>>2)
#define DOUT_OVERFLOW_ERROR_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields DIN_DMA_ERROR	 */
#define DIN_DMA_ERROR_WIDTH                                                   1
#define DIN_DMA_ERROR_SHIFT                                                   1
#define DIN_DMA_ERROR_MASK                                           0x00000002
#define DIN_DMA_ERROR_RD(src)                         (((src) & 0x00000002)>>1)
#define DIN_DMA_ERROR_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields DOUT_DMA_ERROR	 */
#define DOUT_DMA_ERROR_WIDTH                                                  1
#define DOUT_DMA_ERROR_SHIFT                                                  0
#define DOUT_DMA_ERROR_MASK                                          0x00000001
#define DOUT_DMA_ERROR_RD(src)                           (((src) & 0x00000001))
#define DOUT_DMA_ERROR_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register CSR_XTS_TOKEN_CONTROL_WORD	*/ 
/*	 Fields TKN_DWORD0	 */
#define TKN_DWORD0_WIDTH                                                     32
#define TKN_DWORD0_SHIFT                                                      0
#define TKN_DWORD0_MASK                                              0xffffffff
#define TKN_DWORD0_RD(src)                               (((src) & 0xffffffff))
#define TKN_DWORD0_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CSR_XTS_TOKEN_CONTEXT_POINTER	*/ 
/*	 Fields TKN_CONTEXT_POINTER	 */
#define TKN_CONTEXT_POINTER_WIDTH                                            32
#define TKN_CONTEXT_POINTER_SHIFT                                             0
#define TKN_CONTEXT_POINTER_MASK                                     0xffffffff
#define TKN_CONTEXT_POINTER_RD(src)                      (((src) & 0xffffffff))
#define TKN_CONTEXT_POINTER_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CSR_XTS_TOKEN_DMA_LENGTH	*/ 
/*	 Fields TKN_DMA_LENGTH	 */
#define TKN_DMA_LENGTH_WIDTH                                                 32
#define TKN_DMA_LENGTH_SHIFT                                                  0
#define TKN_DMA_LENGTH_MASK                                          0xffffffff
#define TKN_DMA_LENGTH_RD(src)                           (((src) & 0xffffffff))
#define TKN_DMA_LENGTH_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CSR_XTS_TOKEN_IV_DWORD0	*/ 
/*	 Fields TKN_IV_DWORD0	 */
#define TKN_IV_DWORD00_WIDTH                                                 32
#define TKN_IV_DWORD00_SHIFT                                                  0
#define TKN_IV_DWORD00_MASK                                          0xffffffff
#define TKN_IV_DWORD00_RD(src)                           (((src) & 0xffffffff))
#define TKN_IV_DWORD00_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CSR_XTS_TOKEN_IV_DWORD1	*/ 
/*	 Fields TKN_IV_DWORD1	 */
#define TKN_IV_DWORD11_WIDTH                                                 32
#define TKN_IV_DWORD11_SHIFT                                                  0
#define TKN_IV_DWORD11_MASK                                          0xffffffff
#define TKN_IV_DWORD11_RD(src)                           (((src) & 0xffffffff))
#define TKN_IV_DWORD11_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CSR_XTS_TOKEN_IV_DWORD2	*/ 
/*	 Fields TKN_IV_DWORD2	 */
#define TKN_IV_DWORD22_WIDTH                                                 32
#define TKN_IV_DWORD22_SHIFT                                                  0
#define TKN_IV_DWORD22_MASK                                          0xffffffff
#define TKN_IV_DWORD22_RD(src)                           (((src) & 0xffffffff))
#define TKN_IV_DWORD22_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CSR_XTS_TOKEN_IV_DWORD3	*/ 
/*	 Fields TKN_IV_DWORD3	 */
#define TKN_IV_DWORD33_WIDTH                                                 32
#define TKN_IV_DWORD33_SHIFT                                                  0
#define TKN_IV_DWORD33_MASK                                          0xffffffff
#define TKN_IV_DWORD33_RD(src)                           (((src) & 0xffffffff))
#define TKN_IV_DWORD33_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CSR_XTS_CONTEXT_KEY1_DWORD0	*/ 
/*	 Fields CTX_KEY1_DWORD0	 */
#define CTX_KEY1_DWORD00_WIDTH                                               32
#define CTX_KEY1_DWORD00_SHIFT                                                0
#define CTX_KEY1_DWORD00_MASK                                        0xffffffff
#define CTX_KEY1_DWORD00_RD(src)                         (((src) & 0xffffffff))
#define CTX_KEY1_DWORD00_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CSR_XTS_CONTEXT_KEY1_DWORD1	*/ 
/*	 Fields CTX_KEY1_DWORD1	 */
#define CTX_KEY1_DWORD11_WIDTH                                               32
#define CTX_KEY1_DWORD11_SHIFT                                                0
#define CTX_KEY1_DWORD11_MASK                                        0xffffffff
#define CTX_KEY1_DWORD11_RD(src)                         (((src) & 0xffffffff))
#define CTX_KEY1_DWORD11_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CSR_XTS_CONTEXT_KEY1_DWORD2	*/ 
/*	 Fields CTX_KEY1_DWORD2	 */
#define CTX_KEY1_DWORD22_WIDTH                                               32
#define CTX_KEY1_DWORD22_SHIFT                                                0
#define CTX_KEY1_DWORD22_MASK                                        0xffffffff
#define CTX_KEY1_DWORD22_RD(src)                         (((src) & 0xffffffff))
#define CTX_KEY1_DWORD22_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CSR_XTS_CONTEXT_KEY1_DWORD3	*/ 
/*	 Fields CTX_KEY1_DWORD3	 */
#define CTX_KEY1_DWORD33_WIDTH                                               32
#define CTX_KEY1_DWORD33_SHIFT                                                0
#define CTX_KEY1_DWORD33_MASK                                        0xffffffff
#define CTX_KEY1_DWORD33_RD(src)                         (((src) & 0xffffffff))
#define CTX_KEY1_DWORD33_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CSR_XTS_CONTEXT_KEY1_DWORD4	*/ 
/*	 Fields CTX_KEY1_DWORD4	 */
#define CTX_KEY1_DWORD44_WIDTH                                               32
#define CTX_KEY1_DWORD44_SHIFT                                                0
#define CTX_KEY1_DWORD44_MASK                                        0xffffffff
#define CTX_KEY1_DWORD44_RD(src)                         (((src) & 0xffffffff))
#define CTX_KEY1_DWORD44_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CSR_XTS_CONTEXT_KEY1_DWORD5	*/ 
/*	 Fields CTX_KEY1_DWORD5	 */
#define CTX_KEY1_DWORD55_WIDTH                                               32
#define CTX_KEY1_DWORD55_SHIFT                                                0
#define CTX_KEY1_DWORD55_MASK                                        0xffffffff
#define CTX_KEY1_DWORD55_RD(src)                         (((src) & 0xffffffff))
#define CTX_KEY1_DWORD55_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CSR_XTS_CONTEXT_KEY1_DWORD6	*/ 
/*	 Fields CTX_KEY1_DWORD6	 */
#define CTX_KEY1_DWORD66_WIDTH                                               32
#define CTX_KEY1_DWORD66_SHIFT                                                0
#define CTX_KEY1_DWORD66_MASK                                        0xffffffff
#define CTX_KEY1_DWORD66_RD(src)                         (((src) & 0xffffffff))
#define CTX_KEY1_DWORD66_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CSR_XTS_CONTEXT_KEY1_DWORD7	*/ 
/*	 Fields CTX_KEY1_DWORD7	 */
#define CTX_KEY1_DWORD77_WIDTH                                               32
#define CTX_KEY1_DWORD77_SHIFT                                                0
#define CTX_KEY1_DWORD77_MASK                                        0xffffffff
#define CTX_KEY1_DWORD77_RD(src)                         (((src) & 0xffffffff))
#define CTX_KEY1_DWORD77_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CSR_XTS_CONTEXT_KEY2_DWORD0	*/ 
/*	 Fields CTX_KEY2_DWORD0	 */
#define CTX_KEY2_DWORD00_WIDTH                                               32
#define CTX_KEY2_DWORD00_SHIFT                                                0
#define CTX_KEY2_DWORD00_MASK                                        0xffffffff
#define CTX_KEY2_DWORD00_RD(src)                         (((src) & 0xffffffff))
#define CTX_KEY2_DWORD00_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CSR_XTS_CONTEXT_KEY2_DWORD1	*/ 
/*	 Fields CTX_KEY2_DWORD1	 */
#define CTX_KEY2_DWORD11_WIDTH                                               32
#define CTX_KEY2_DWORD11_SHIFT                                                0
#define CTX_KEY2_DWORD11_MASK                                        0xffffffff
#define CTX_KEY2_DWORD11_RD(src)                         (((src) & 0xffffffff))
#define CTX_KEY2_DWORD11_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CSR_XTS_CONTEXT_KEY2_DWORD2	*/ 
/*	 Fields CTX_KEY2_DWORD2	 */
#define CTX_KEY2_DWORD22_WIDTH                                               32
#define CTX_KEY2_DWORD22_SHIFT                                                0
#define CTX_KEY2_DWORD22_MASK                                        0xffffffff
#define CTX_KEY2_DWORD22_RD(src)                         (((src) & 0xffffffff))
#define CTX_KEY2_DWORD22_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CSR_XTS_CONTEXT_KEY2_DWORD3	*/ 
/*	 Fields CTX_KEY2_DWORD3	 */
#define CTX_KEY2_DWORD33_WIDTH                                               32
#define CTX_KEY2_DWORD33_SHIFT                                                0
#define CTX_KEY2_DWORD33_MASK                                        0xffffffff
#define CTX_KEY2_DWORD33_RD(src)                         (((src) & 0xffffffff))
#define CTX_KEY2_DWORD33_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CSR_XTS_CONTEXT_KEY2_DWORD4	*/ 
/*	 Fields CTX_KEY2_DWORD4	 */
#define CTX_KEY2_DWORD44_WIDTH                                               32
#define CTX_KEY2_DWORD44_SHIFT                                                0
#define CTX_KEY2_DWORD44_MASK                                        0xffffffff
#define CTX_KEY2_DWORD44_RD(src)                         (((src) & 0xffffffff))
#define CTX_KEY2_DWORD44_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CSR_XTS_CONTEXT_KEY2_DWORD5	*/ 
/*	 Fields CTX_KEY2_DWORD5	 */
#define CTX_KEY2_DWORD55_WIDTH                                               32
#define CTX_KEY2_DWORD55_SHIFT                                                0
#define CTX_KEY2_DWORD55_MASK                                        0xffffffff
#define CTX_KEY2_DWORD55_RD(src)                         (((src) & 0xffffffff))
#define CTX_KEY2_DWORD55_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CSR_XTS_CONTEXT_KEY2_DWORD6	*/ 
/*	 Fields CTX_KEY2_DWORD6	 */
#define CTX_KEY2_DWORD66_WIDTH                                               32
#define CTX_KEY2_DWORD66_SHIFT                                                0
#define CTX_KEY2_DWORD66_MASK                                        0xffffffff
#define CTX_KEY2_DWORD66_RD(src)                         (((src) & 0xffffffff))
#define CTX_KEY2_DWORD66_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CSR_XTS_CONTEXT_KEY2_DWORD7	*/ 
/*	 Fields CTX_KEY2_DWORD7	 */
#define CTX_KEY2_DWORD77_WIDTH                                               32
#define CTX_KEY2_DWORD77_SHIFT                                                0
#define CTX_KEY2_DWORD77_MASK                                        0xffffffff
#define CTX_KEY2_DWORD77_RD(src)                         (((src) & 0xffffffff))
#define CTX_KEY2_DWORD77_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CSR_XTS_DIN_XFR_COUNT	*/ 
/*	 Fields DIN_XFR_COUNT	 */
#define DIN_XFR_COUNT_WIDTH                                                  32
#define DIN_XFR_COUNT_SHIFT                                                   0
#define DIN_XFR_COUNT_MASK                                           0xffffffff
#define DIN_XFR_COUNT_RD(src)                            (((src) & 0xffffffff))
#define DIN_XFR_COUNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CSR_XTS_DOUT_XFR_COUNT	*/ 
/*	 Fields DOUT_XFR_COUNT	 */
#define DOUT_XFR_COUNT_WIDTH                                                 32
#define DOUT_XFR_COUNT_SHIFT                                                  0
#define DOUT_XFR_COUNT_MASK                                          0xffffffff
#define DOUT_XFR_COUNT_RD(src)                           (((src) & 0xffffffff))
#define DOUT_XFR_COUNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CSR_XTS_DEBUG_TRACE	*/ 
/*	 Fields DEBUG_TRACE	 */
#define DEBUG_TRACE_WIDTH                                                    32
#define DEBUG_TRACE_SHIFT                                                     0
#define DEBUG_TRACE_MASK                                             0xffffffff
#define DEBUG_TRACE_RD(src)                              (((src) & 0xffffffff))
#define DEBUG_TRACE_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*    Address SEC_EIP96_AXI_CSR  Registers */

/*    Address SEC_EIP96_CRYPTO_CSR  Registers */

/*	Global Base Address	*/
#define SEC_EIP96_CORE_CSR_BASE_ADDR			0xddd833800ULL

/*    Address SEC_EIP96_CORE_CSR  Registers */
#define IPE_TKN_CTRL_STAT_ADDR                                       0x00000000
#define IPE_TKN_CTRL_STAT_DEFAULT                                    0x00004004
#define IPE_PRC_ALG_EN_ADDR                                          0x00000004
#define IPE_PRC_ALG_EN_DEFAULT                                       0xffffffff
#define IPE_CTX_CTRL_ADDR                                            0x00000008
#define IPE_CTX_CTRL_DEFAULT                                         0x00000235
#define IPE_CTX_STAT_ADDR                                            0x0000000c
#define IPE_CTX_STAT_DEFAULT                                         0x00000000
#define IPE_INT_CTRL_STAT_ADDR                                       0x00000010
#define IPE_INT_CTRL_STAT_DEFAULT                                    0xc00f0000
#define IPE_RX_CTRL_STAT_ADDR                                        0x00000014
#define IPE_RX_CTRL_STAT_DEFAULT                                     0xf88008ff
#define IPE_TX_CTRL_STAT_ADDR                                        0x00000018
#define IPE_TX_CTRL_STAT_DEFAULT                                     0xf88008ff
#define IPE_DEV_INFO_ADDR                                            0x0000003c
#define IPE_DEV_INFO_DEFAULT                                         0xdfddf312
#define IPE_PRNG_STAT_ADDR                                           0x00000040
#define IPE_PRNG_STAT_DEFAULT                                        0x00000000
#define IPE_PRNG_CTRL_ADDR                                           0x00000044
#define IPE_PRNG_CTRL_DEFAULT                                        0x00000000
#define IPE_PRNG_SEED_L_ADDR                                         0x00000048
#define IPE_PRNG_SEED_L_DEFAULT                                      0x00000000
#define IPE_PRNG_SEED_H_ADDR                                         0x0000004c
#define IPE_PRNG_SEED_H_DEFAULT                                      0x00000000
#define IPE_PRNG_KEY_0_L_ADDR                                        0x00000050
#define IPE_PRNG_KEY_0_L_DEFAULT                                     0x00000000
#define IPE_PRNG_KEY_0_H_ADDR                                        0x00000054
#define IPE_PRNG_KEY_0_H_DEFAULT                                     0x00000000
#define IPE_PRNG_KEY_1_L_ADDR                                        0x00000058
#define IPE_PRNG_KEY_1_L_DEFAULT                                     0x00000000
#define IPE_PRNG_KEY_1_H_ADDR                                        0x0000005c
#define IPE_PRNG_KEY_1_H_DEFAULT                                     0x00000000
#define IPE_PRNG_RES0_L_ADDR                                         0x00000060
#define IPE_PRNG_RES0_L_DEFAULT                                      0x00000000
#define IPE_PRNG_RES0_H_ADDR                                         0x00000064
#define IPE_PRNG_RES0_H_DEFAULT                                      0x00000000
#define IPE_PRNG_RES1_L_ADDR                                         0x00000068
#define IPE_PRNG_RES1_L_DEFAULT                                      0x00000000
#define IPE_PRNG_RES1_H_ADDR                                         0x0000006c
#define IPE_PRNG_RES1_H_DEFAULT                                      0x00000000
#define IPE_PRNG_LFSR_L_ADDR                                         0x00000070
#define IPE_PRNG_LFSR_L_DEFAULT                                      0x00000000
#define IPE_PRNG_LFSR_H_ADDR                                         0x00000074
#define IPE_PRNG_LFSR_H_DEFAULT                                      0x00000000
#define IPE_ACT_TKN_W0_ADDR                                          0x00000080
#define IPE_ACT_TKN_W0_DEFAULT                                       0x00000000
#define IPE_ACT_TKN_W1_ADDR                                          0x00000084
#define IPE_ACT_TKN_W1_DEFAULT                                       0x00000000
#define IPE_ACT_TKN_W2_ADDR                                          0x00000088
#define IPE_ACT_TKN_W2_DEFAULT                                       0x00000000
#define IPE_ACT_TKN_W3_ADDR                                          0x0000008c
#define IPE_ACT_TKN_W3_DEFAULT                                       0x00000000
#define IPE_ACT_TKN_W4_ADDR                                          0x00000090
#define IPE_ACT_TKN_W4_DEFAULT                                       0x00000000
#define IPE_ACT_TKN_W5_ADDR                                          0x00000094
#define IPE_ACT_TKN_W5_DEFAULT                                       0x00000000
#define IPE_ACT_TKN_W6_ADDR                                          0x00000098
#define IPE_ACT_TKN_W6_DEFAULT                                       0x00000000
#define IPE_ACT_TKN_W7_ADDR                                          0x0000009c
#define IPE_ACT_TKN_W7_DEFAULT                                       0x00000000
#define IPE_ACT_TKN_W8_ADDR                                          0x000000a0
#define IPE_ACT_TKN_W8_DEFAULT                                       0x00000000
#define IPE_ACT_TKN_W9_ADDR                                          0x000000a4
#define IPE_ACT_TKN_W9_DEFAULT                                       0x00000000
#define IPE_ACT_TKN_W10_ADDR                                         0x000000a8
#define IPE_ACT_TKN_W10_DEFAULT                                      0x00000000
#define IPE_ACT_TKN_W11_ADDR                                         0x000000ac
#define IPE_ACT_TKN_W11_DEFAULT                                      0x00000000
#define IPE_ACT_TKN_W12_ADDR                                         0x000000b0
#define IPE_ACT_TKN_W12_DEFAULT                                      0x00000000
#define IPE_ACT_TKN_W13_ADDR                                         0x000000b4
#define IPE_ACT_TKN_W13_DEFAULT                                      0x00000000
#define IPE_ACT_TKN_W14_ADDR                                         0x000000b8
#define IPE_ACT_TKN_W14_DEFAULT                                      0x00000000
#define IPE_ACT_TKN_W15_ADDR                                         0x000000bc
#define IPE_ACT_TKN_W15_DEFAULT                                      0x00000000
#define IPE_NXT_TKN_W0_ACT_W16_ADDR                                  0x000000c0
#define IPE_NXT_TKN_W0_ACT_W16_DEFAULT                               0x00000000
#define IPE_NXT_TKN_W1_ACT_W17_ADDR                                  0x000000c4
#define IPE_NXT_TKN_W1_ACT_W17_DEFAULT                               0x00000000
#define IPE_NXT_TKN_W2_ACT_W18_ADDR                                  0x000000c8
#define IPE_NXT_TKN_W2_ACT_W18_DEFAULT                               0x00000000
#define IPE_NXT_TKN_W3_ACT_W19_ADDR                                  0x000000cc
#define IPE_NXT_TKN_W3_ACT_W19_DEFAULT                               0x00000000
#define IPE_NXT_TKN_W4_ACT_W20_ADDR                                  0x000000d0
#define IPE_NXT_TKN_W4_ACT_W20_DEFAULT                               0x00000000
#define IPE_NXT_TKN_W5_ACT_W21_ADDR                                  0x000000d4
#define IPE_NXT_TKN_W5_ACT_W21_DEFAULT                               0x00000000
#define IPE_NXT_TKN_W6_ACT_W22_ADDR                                  0x000000d8
#define IPE_NXT_TKN_W6_ACT_W22_DEFAULT                               0x00000000
#define IPE_NXT_TKN_W7_ACT_W23_ADDR                                  0x000000dc
#define IPE_NXT_TKN_W7_ACT_W23_DEFAULT                               0x00000000
#define IPE_NXT_TKN_W8_ACT_W24_ADDR                                  0x000000e0
#define IPE_NXT_TKN_W8_ACT_W24_DEFAULT                               0x00000000
#define IPE_NXT_TKN_W9_ACT_W25_ADDR                                  0x000000e4
#define IPE_NXT_TKN_W9_ACT_W25_DEFAULT                               0x00000000
#define IPE_NXT_TKN_W10_ACT_W26_ADDR                                 0x000000e8
#define IPE_NXT_TKN_W10_ACT_W26_DEFAULT                              0x00000000
#define IPE_NXT_TKN_W11_ACT_W27_ADDR                                 0x000000ec
#define IPE_NXT_TKN_W11_ACT_W27_DEFAULT                              0x00000000
#define IPE_NXT_TKN_W12_ACT_W28_ADDR                                 0x000000f0
#define IPE_NXT_TKN_W12_ACT_W28_DEFAULT                              0x00000000
#define IPE_NXT_TKN_W13_ACT_W29_ADDR                                 0x000000f4
#define IPE_NXT_TKN_W13_ACT_W29_DEFAULT                              0x00000000
#define IPE_NXT_TKN_W14_ACT_W30_ADDR                                 0x000000f8
#define IPE_NXT_TKN_W14_ACT_W30_DEFAULT                              0x00000000
#define IPE_NXT_TKN_W15_ACT_W31_ADDR                                 0x000000fc
#define IPE_NXT_TKN_W15_ACT_W31_DEFAULT                              0x00000000
#define IPE_RES_TKN_W0_ADDR                                          0x00000100
#define IPE_RES_TKN_W0_DEFAULT                                       0x00000000
#define IPE_RES_TKN_W1_ADDR                                          0x00000104
#define IPE_RES_TKN_W1_DEFAULT                                       0x00000000
#define IPE_RES_TKN_W2_ADDR                                          0x00000108
#define IPE_RES_TKN_W2_DEFAULT                                       0x00000000
#define IPE_RES_TKN_W3_ADDR                                          0x0000010c
#define IPE_RES_TKN_W3_DEFAULT                                       0x00000000
#define IPE_RES_TKN_W4_ADDR                                          0x00000110
#define IPE_RES_TKN_W4_DEFAULT                                       0x00000000
#define IPE_RES_TKN_W5_ADDR                                          0x00000114
#define IPE_RES_TKN_W5_DEFAULT                                       0x00000000
#define IPE_RES_TKN_W6_ADDR                                          0x00000118
#define IPE_RES_TKN_W6_DEFAULT                                       0x00000000
#define IPE_RES_TKN_W7_ADDR                                          0x0000011c
#define IPE_RES_TKN_W7_DEFAULT                                       0x00000000
#define IPE_NXT_CTX_CMD0_ADDR                                        0x00000140
#define IPE_NXT_CTX_CMD0_DEFAULT                                     0x00000000
#define IPE_NXT_CTX_CMD1_ADDR                                        0x00000144
#define IPE_NXT_CTX_CMD1_DEFAULT                                     0x00000000
#define IPE_NXT_GPR0_ADDR                                            0x00000148
#define IPE_NXT_GPR0_DEFAULT                                         0x00000000
#define IPE_NXT_GPR1_ADDR                                            0x0000014c
#define IPE_NXT_GPR1_DEFAULT                                         0x00000000
#define IPE_NXT_IV0_ADDR                                             0x00000150
#define IPE_NXT_IV0_DEFAULT                                          0x00000000
#define IPE_NXT_IV1_ADDR                                             0x00000154
#define IPE_NXT_IV1_DEFAULT                                          0x00000000
#define IPE_NXT_IV2_ADDR                                             0x00000158
#define IPE_NXT_IV2_DEFAULT                                          0x00000000
#define IPE_NXT_IV3_ADDR                                             0x0000015c
#define IPE_NXT_IV3_DEFAULT                                          0x00000000
#define IPE_NXT_KEY0_ADDR                                            0x00000160
#define IPE_NXT_KEY0_DEFAULT                                         0x00000000
#define IPE_NXT_KEY1_ADDR                                            0x00000164
#define IPE_NXT_KEY1_DEFAULT                                         0x00000000
#define IPE_NXT_KEY2_ADDR                                            0x00000168
#define IPE_NXT_KEY2_DEFAULT                                         0x00000000
#define IPE_NXT_KEY3_ADDR                                            0x0000016c
#define IPE_NXT_KEY3_DEFAULT                                         0x00000000
#define IPE_NXT_KEY4_ADDR                                            0x00000170
#define IPE_NXT_KEY4_DEFAULT                                         0x00000000
#define IPE_NXT_KEY5_ADDR                                            0x00000174
#define IPE_NXT_KEY5_DEFAULT                                         0x00000000
#define IPE_NXT_KEY6_ADDR                                            0x00000178
#define IPE_NXT_KEY6_DEFAULT                                         0x00000000
#define IPE_NXT_KEY7_ADDR                                            0x0000017c
#define IPE_NXT_KEY7_DEFAULT                                         0x00000000
#define IPE_NXT_IN_DGST0_ADDR                                        0x00000180
#define IPE_NXT_IN_DGST0_DEFAULT                                     0x00000000
#define IPE_NXT_IN_DGST1_ADDR                                        0x00000184
#define IPE_NXT_IN_DGST1_DEFAULT                                     0x00000000
#define IPE_NXT_IN_DGST2_ADDR                                        0x00000188
#define IPE_NXT_IN_DGST2_DEFAULT                                     0x00000000
#define IPE_NXT_IN_DGST3_ADDR                                        0x0000018c
#define IPE_NXT_IN_DGST3_DEFAULT                                     0x00000000
#define IPE_NXT_IN_DGST4_ADDR                                        0x00000190
#define IPE_NXT_IN_DGST4_DEFAULT                                     0x00000000
#define IPE_NXT_IN_DGST5_ADDR                                        0x00000194
#define IPE_NXT_IN_DGST5_DEFAULT                                     0x00000000
#define IPE_NXT_IN_DGST6_ADDR                                        0x00000198
#define IPE_NXT_IN_DGST6_DEFAULT                                     0x00000000
#define IPE_NXT_IN_DGST7_ADDR                                        0x0000019c
#define IPE_NXT_IN_DGST7_DEFAULT                                     0x00000000
#define IPE_NXT_OUT_DGST0_ADDR                                       0x000001a0
#define IPE_NXT_OUT_DGST0_DEFAULT                                    0x00000000
#define IPE_NXT_OUT_DGST1_ADDR                                       0x000001a4
#define IPE_NXT_OUT_DGST1_DEFAULT                                    0x00000000
#define IPE_NXT_OUT_DGST2_ADDR                                       0x000001a8
#define IPE_NXT_OUT_DGST2_DEFAULT                                    0x00000000
#define IPE_NXT_OUT_DGST3_ADDR                                       0x000001ac
#define IPE_NXT_OUT_DGST3_DEFAULT                                    0x00000000
#define IPE_NXT_OUT_DGST4_ADDR                                       0x000001b0
#define IPE_NXT_OUT_DGST4_DEFAULT                                    0x00000000
#define IPE_NXT_OUT_DGST5_ADDR                                       0x000001b4
#define IPE_NXT_OUT_DGST5_DEFAULT                                    0x00000000
#define IPE_NXT_OUT_DGST6_ADDR                                       0x000001b8
#define IPE_NXT_OUT_DGST6_DEFAULT                                    0x00000000
#define IPE_NXT_OUT_DGST7_ADDR                                       0x000001bc
#define IPE_NXT_OUT_DGST7_DEFAULT                                    0x00000000
#define IPE_NXT_DGST_CNT_ADDR                                        0x000001c0
#define IPE_NXT_DGST_CNT_DEFAULT                                     0x00000000
#define IPE_NXT_SPI_SSRC_ADDR                                        0x000001c4
#define IPE_NXT_SPI_SSRC_DEFAULT                                     0x00000000
#define IPE_NXT_SN_ADDR                                              0x000001c8
#define IPE_NXT_SN_DEFAULT                                           0x00000000
#define IPE_NXT_ESN_ADDR                                             0x000001cc
#define IPE_NXT_ESN_DEFAULT                                          0x00000000
#define IPE_NXT_SN_M0_ADDR                                           0x000001d0
#define IPE_NXT_SN_M0_DEFAULT                                        0x00000000
#define IPE_NXT_SN_M1_ADDR                                           0x000001d4
#define IPE_NXT_SN_M1_DEFAULT                                        0x00000000
#define IPE_NXT_SN_M2_ADDR                                           0x000001d8
#define IPE_NXT_SN_M2_DEFAULT                                        0x00000000
#define IPE_NXT_SN_M3_ADDR                                           0x000001dc
#define IPE_NXT_SN_M3_DEFAULT                                        0x00000000
#define IPE_NXT_CS_ADDR                                              0x000001e4
#define IPE_NXT_CS_DEFAULT                                           0x00000000
#define IPE_NXT_UP_PKT_LEN_ADDR                                      0x000001e8
#define IPE_NXT_UP_PKT_LEN_DEFAULT                                   0x00000000
#define IPE_ACT_CTX_CMD0_ADDR                                        0x00000200
#define IPE_ACT_CTX_CMD0_DEFAULT                                     0x00000000
#define IPE_ACT_CTX_CMD1_ADDR                                        0x00000204
#define IPE_ACT_CTX_CMD1_DEFAULT                                     0x00000000
#define IPE_ACT_GPR0_ADDR                                            0x00000208
#define IPE_ACT_GPR0_DEFAULT                                         0x00000000
#define IPE_ACT_GPR1_ADDR                                            0x0000020c
#define IPE_ACT_GPR1_DEFAULT                                         0x00000000
#define IPE_IV0_ADDR                                                 0x00000210
#define IPE_IV0_DEFAULT                                              0x00000000
#define IPE_IV1_ADDR                                                 0x00000214
#define IPE_IV1_DEFAULT                                              0x00000000
#define IPE_IV2_ADDR                                                 0x00000218
#define IPE_IV2_DEFAULT                                              0x00000000
#define IPE_IV3_ADDR                                                 0x0000021c
#define IPE_IV3_DEFAULT                                              0x00000000
#define IPE_KEY0_ADDR                                                0x00000220
#define IPE_KEY0_DEFAULT                                             0x00000000
#define IPE_KEY1_ADDR                                                0x00000224
#define IPE_KEY1_DEFAULT                                             0x00000000
#define IPE_KEY2_ADDR                                                0x00000228
#define IPE_KEY2_DEFAULT                                             0x00000000
#define IPE_KEY3_ADDR                                                0x0000022c
#define IPE_KEY3_DEFAULT                                             0x00000000
#define IPE_KEY4_ADDR                                                0x00000230
#define IPE_KEY4_DEFAULT                                             0x00000000
#define IPE_KEY5_ADDR                                                0x00000234
#define IPE_KEY5_DEFAULT                                             0x00000000
#define IPE_KEY6_ADDR                                                0x00000238
#define IPE_KEY6_DEFAULT                                             0x00000000
#define IPE_KEY7_ADDR                                                0x0000023c
#define IPE_KEY7_DEFAULT                                             0x00000000
#define IPE_IN_DGST0_ADDR                                            0x00000240
#define IPE_IN_DGST0_DEFAULT                                         0x00000000
#define IPE_IN_DGST1_ADDR                                            0x00000244
#define IPE_IN_DGST1_DEFAULT                                         0x00000000
#define IPE_IN_DGST2_ADDR                                            0x00000248
#define IPE_IN_DGST2_DEFAULT                                         0x00000000
#define IPE_IN_DGST3_ADDR                                            0x0000024c
#define IPE_IN_DGST3_DEFAULT                                         0x00000000
#define IPE_IN_DGST4_ADDR                                            0x00000250
#define IPE_IN_DGST4_DEFAULT                                         0x00000000
#define IPE_IN_DGST5_ADDR                                            0x00000254
#define IPE_IN_DGST5_DEFAULT                                         0x00000000
#define IPE_IN_DGST6_ADDR                                            0x00000258
#define IPE_IN_DGST6_DEFAULT                                         0x00000000
#define IPE_IN_DGST7_ADDR                                            0x0000025c
#define IPE_IN_DGST7_DEFAULT                                         0x00000000
#define IPE_OUT_DGST0_ADDR                                           0x00000260
#define IPE_OUT_DGST0_DEFAULT                                        0x00000000
#define IPE_OUT_DGST1_ADDR                                           0x00000264
#define IPE_OUT_DGST1_DEFAULT                                        0x00000000
#define IPE_OUT_DGST2_ADDR                                           0x00000268
#define IPE_OUT_DGST2_DEFAULT                                        0x00000000
#define IPE_OUT_DGST3_ADDR                                           0x0000026c
#define IPE_OUT_DGST3_DEFAULT                                        0x00000000
#define IPE_OUT_DGST4_ADDR                                           0x00000270
#define IPE_OUT_DGST4_DEFAULT                                        0x00000000
#define IPE_OUT_DGST5_ADDR                                           0x00000274
#define IPE_OUT_DGST5_DEFAULT                                        0x00000000
#define IPE_OUT_DGST6_ADDR                                           0x00000278
#define IPE_OUT_DGST6_DEFAULT                                        0x00000000
#define IPE_OUT_DGST7_ADDR                                           0x0000027c
#define IPE_OUT_DGST7_DEFAULT                                        0x00000000
#define IPE_DGST_CNT_ADDR                                            0x00000280
#define IPE_DGST_CNT_DEFAULT                                         0x00000000
#define IPE_SPI_SSRC_ADDR                                            0x00000284
#define IPE_SPI_SSRC_DEFAULT                                         0x00000000
#define IPE_SN_ADDR                                                  0x00000288
#define IPE_SN_DEFAULT                                               0x00000000
#define IPE_ESN_ADDR                                                 0x0000028c
#define IPE_ESN_DEFAULT                                              0x00000000
#define IPE_SN_M0_ADDR                                               0x00000290
#define IPE_SN_M0_DEFAULT                                            0x00000000
#define IPE_SN_M1_ADDR                                               0x00000294
#define IPE_SN_M1_DEFAULT                                            0x00000000
#define IPE_SN_M2_ADDR                                               0x00000298
#define IPE_SN_M2_DEFAULT                                            0x00000000
#define IPE_SN_M3_ADDR                                               0x0000029c
#define IPE_SN_M3_DEFAULT                                            0x00000000
#define IPE_CS_ADDR                                                  0x000002a4
#define IPE_CS_DEFAULT                                               0x00000000
#define IPE_UP_PKT_LEN_ADDR                                          0x000002a8
#define IPE_UP_PKT_LEN_DEFAULT                                       0x00000000
#define IPE_HASH_RES0_ADDR                                           0x000002c0
#define IPE_HASH_RES0_DEFAULT                                        0x00000000
#define IPE_HASH_RES1_ADDR                                           0x000002c4
#define IPE_HASH_RES1_DEFAULT                                        0x00000000
#define IPE_HASH_RES2_ADDR                                           0x000002c8
#define IPE_HASH_RES2_DEFAULT                                        0x00000000
#define IPE_HASH_RES3_ADDR                                           0x000002cc
#define IPE_HASH_RES3_DEFAULT                                        0x00000000
#define IPE_HASH_RES4_ADDR                                           0x000002d0
#define IPE_HASH_RES4_DEFAULT                                        0x00000000
#define IPE_HASH_RES5_ADDR                                           0x000002d4
#define IPE_HASH_RES5_DEFAULT                                        0x00000000
#define IPE_HASH_RES6_ADDR                                           0x000002d8
#define IPE_HASH_RES6_DEFAULT                                        0x00000000
#define IPE_HASH_RES7_ADDR                                           0x000002dc
#define IPE_HASH_RES7_DEFAULT                                        0x00000000
#define IPE_DGST_CNT_RES_ADDR                                        0x000002e0
#define IPE_DGST_CNT_RES_DEFAULT                                     0x00000000
#define IPE_SPI_RTVD_ADDR                                            0x000002e4
#define IPE_SPI_RTVD_DEFAULT                                         0x00000000
#define IPE_SN_RTVD_ADDR                                             0x000002e8
#define IPE_SN_RTVD_DEFAULT                                          0x00000000
#define IPE_ESN_CALC_ADDR                                            0x000002ec
#define IPE_ESN_CALC_DEFAULT                                         0x00000000
#define IPE_CS_RES_ADDR                                              0x000002f0
#define IPE_CS_RES_DEFAULT                                           0x00000000
#define IPE_NXT_IN_DGST8_ADDR                                        0x00000300
#define IPE_NXT_IN_DGST8_DEFAULT                                     0x00000000
#define IPE_NXT_IN_DGST9_ADDR                                        0x00000304
#define IPE_NXT_IN_DGST9_DEFAULT                                     0x00000000
#define IPE_NXT_IN_DGST10_ADDR                                       0x00000308
#define IPE_NXT_IN_DGST10_DEFAULT                                    0x00000000
#define IPE_NXT_IN_DGST11_ADDR                                       0x0000030c
#define IPE_NXT_IN_DGST11_DEFAULT                                    0x00000000
#define IPE_NXT_IN_DGST12_ADDR                                       0x00000310
#define IPE_NXT_IN_DGST12_DEFAULT                                    0x00000000
#define IPE_NXT_IN_DGST13_ADDR                                       0x00000314
#define IPE_NXT_IN_DGST13_DEFAULT                                    0x00000000
#define IPE_NXT_IN_DGST14_ADDR                                       0x00000318
#define IPE_NXT_IN_DGST14_DEFAULT                                    0x00000000
#define IPE_NXT_IN_DGST15_ADDR                                       0x0000031c
#define IPE_NXT_IN_DGST15_DEFAULT                                    0x00000000
#define IPE_NXT_OUT_DGST8_ADDR                                       0x00000320
#define IPE_NXT_OUT_DGST8_DEFAULT                                    0x00000000
#define IPE_NXT_OUT_DGST9_ADDR                                       0x00000324
#define IPE_NXT_OUT_DGST9_DEFAULT                                    0x00000000
#define IPE_NXT_OUT_DGST10_ADDR                                      0x00000328
#define IPE_NXT_OUT_DGST10_DEFAULT                                   0x00000000
#define IPE_NXT_OUT_DGST11_ADDR                                      0x0000032c
#define IPE_NXT_OUT_DGST11_DEFAULT                                   0x00000000
#define IPE_NXT_OUT_DGST12_ADDR                                      0x00000330
#define IPE_NXT_OUT_DGST12_DEFAULT                                   0x00000000
#define IPE_NXT_OUT_DGST13_ADDR                                      0x00000334
#define IPE_NXT_OUT_DGST13_DEFAULT                                   0x00000000
#define IPE_NXT_OUT_DGST14_ADDR                                      0x00000338
#define IPE_NXT_OUT_DGST14_DEFAULT                                   0x00000000
#define IPE_NXT_OUT_DGST15_ADDR                                      0x0000033c
#define IPE_NXT_OUT_DGST15_DEFAULT                                   0x00000000
#define IPE_IN_DGST8_ADDR                                            0x00000340
#define IPE_IN_DGST8_DEFAULT                                         0x00000000
#define IPE_IN_DGST9_ADDR                                            0x00000344
#define IPE_IN_DGST9_DEFAULT                                         0x00000000
#define IPE_IN_DGST10_ADDR                                           0x00000348
#define IPE_IN_DGST10_DEFAULT                                        0x00000000
#define IPE_IN_DGST11_ADDR                                           0x0000034c
#define IPE_IN_DGST11_DEFAULT                                        0x00000000
#define IPE_IN_DGST12_ADDR                                           0x00000350
#define IPE_IN_DGST12_DEFAULT                                        0x00000000
#define IPE_IN_DGST13_ADDR                                           0x00000354
#define IPE_IN_DGST13_DEFAULT                                        0x00000000
#define IPE_IN_DGST14_ADDR                                           0x00000358
#define IPE_IN_DGST14_DEFAULT                                        0x00000000
#define IPE_IN_DGST15_ADDR                                           0x0000035c
#define IPE_IN_DGST15_DEFAULT                                        0x00000000
#define IPE_OUT_DGST8_ADDR                                           0x00000360
#define IPE_OUT_DGST8_DEFAULT                                        0x00000000
#define IPE_OUT_DGST9_ADDR                                           0x00000364
#define IPE_OUT_DGST9_DEFAULT                                        0x00000000
#define IPE_OUT_DGST10_ADDR                                          0x00000368
#define IPE_OUT_DGST10_DEFAULT                                       0x00000000
#define IPE_OUT_DGST11_ADDR                                          0x0000036c
#define IPE_OUT_DGST11_DEFAULT                                       0x00000000
#define IPE_OUT_DGST12_ADDR                                          0x00000370
#define IPE_OUT_DGST12_DEFAULT                                       0x00000000
#define IPE_OUT_DGST13_ADDR                                          0x00000374
#define IPE_OUT_DGST13_DEFAULT                                       0x00000000
#define IPE_OUT_DGST14_ADDR                                          0x00000378
#define IPE_OUT_DGST14_DEFAULT                                       0x00000000
#define IPE_OUT_DGST15_ADDR                                          0x0000037c
#define IPE_OUT_DGST15_DEFAULT                                       0x00000000
#define IPE_HASH_RES8_ADDR                                           0x00000380
#define IPE_HASH_RES8_DEFAULT                                        0x00000000
#define IPE_HASH_RES9_ADDR                                           0x00000384
#define IPE_HASH_RES9_DEFAULT                                        0x00000000
#define IPE_HASH_RES10_ADDR                                          0x00000388
#define IPE_HASH_RES10_DEFAULT                                       0x00000000
#define IPE_HASH_RES11_ADDR                                          0x0000038c
#define IPE_HASH_RES11_DEFAULT                                       0x00000000
#define IPE_HASH_RES12_ADDR                                          0x00000390
#define IPE_HASH_RES12_DEFAULT                                       0x00000000
#define IPE_HASH_RES13_ADDR                                          0x00000394
#define IPE_HASH_RES13_DEFAULT                                       0x00000000
#define IPE_HASH_RES14_ADDR                                          0x00000398
#define IPE_HASH_RES14_DEFAULT                                       0x00000000
#define IPE_HASH_RES15_ADDR                                          0x0000039c
#define IPE_HASH_RES15_DEFAULT                                       0x00000000

/*	Register IPE_TKN_CTRL_STAT	*/ 
/*	 Fields Hold_processing	 */
#define HOLD_PROCESSING_WIDTH                                                 1
#define HOLD_PROCESSING_SHIFT                                                31
#define HOLD_PROCESSING_MASK                                         0x80000000
#define HOLD_PROCESSING_RD(src)                      (((src) & 0x80000000)>>31)
#define HOLD_PROCESSING_WR(src)                 (((u32)(src)<<31) & 0x80000000)
#define HOLD_PROCESSING_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields rsvd2	 */
#define RSVD2_WIDTH                                                           1
#define RSVD2_SHIFT                                                          30
#define RSVD2_MASK                                                   0x40000000
#define RSVD2_RD(src)                                (((src) & 0x40000000)>>30)
#define RSVD2_WR(src)                           (((u32)(src)<<30) & 0x40000000)
#define RSVD2_SET(dst,src) \
                      (((dst) & ~0x40000000) | (((u32)(src)<<30) & 0x40000000))
/*	 Fields Process_N_packets	 */
#define PROCESS_N_PACKETS_WIDTH                                               6
#define PROCESS_N_PACKETS_SHIFT                                              24
#define PROCESS_N_PACKETS_MASK                                       0x3f000000
#define PROCESS_N_PACKETS_RD(src)                    (((src) & 0x3f000000)>>24)
#define PROCESS_N_PACKETS_WR(src)               (((u32)(src)<<24) & 0x3f000000)
#define PROCESS_N_PACKETS_SET(dst,src) \
                      (((dst) & ~0x3f000000) | (((u32)(src)<<24) & 0x3f000000))
/*	 Fields Debug_mode	 */
#define SEC_DEBUG_MODE_WIDTH                                                  1
#define SEC_DEBUG_MODE_SHIFT                                                 23
#define SEC_DEBUG_MODE_MASK                                          0x00800000
#define SEC_DEBUG_MODE_RD(src)                       (((src) & 0x00800000)>>23)
#define SEC_DEBUG_MODE_WR(src)                  (((u32)(src)<<23) & 0x00800000)
#define SEC_DEBUG_MODE_SET(dst,src) \
                      (((dst) & ~0x00800000) | (((u32)(src)<<23) & 0x00800000))
/*	 Fields Time_out_counter_enable	 */
#define TIME_OUT_COUNTER_ENABLE_WIDTH                                         1
#define TIME_OUT_COUNTER_ENABLE_SHIFT                                        22
#define TIME_OUT_COUNTER_ENABLE_MASK                                 0x00400000
#define TIME_OUT_COUNTER_ENABLE_RD(src)              (((src) & 0x00400000)>>22)
#define TIME_OUT_COUNTER_ENABLE_WR(src)         (((u32)(src)<<22) & 0x00400000)
#define TIME_OUT_COUNTER_ENABLE_SET(dst,src) \
                      (((dst) & ~0x00400000) | (((u32)(src)<<22) & 0x00400000))
/*	 Fields rsvd1	 */
#define RSVD1_WIDTH                                                           4
#define RSVD1_SHIFT                                                          18
#define RSVD1_MASK                                                   0x003c0000
#define RSVD1_RD(src)                                (((src) & 0x003c0000)>>18)
#define RSVD1_WR(src)                           (((u32)(src)<<18) & 0x003c0000)
#define RSVD1_SET(dst,src) \
                      (((dst) & ~0x003c0000) | (((u32)(src)<<18) & 0x003c0000))
/*	 Fields Interrupt_pulse_or_level	 */
#define INTERRUPT_PULSE_OR_LEVEL_WIDTH                                        1
#define INTERRUPT_PULSE_OR_LEVEL_SHIFT                                       17
#define INTERRUPT_PULSE_OR_LEVEL_MASK                                0x00020000
#define INTERRUPT_PULSE_OR_LEVEL_RD(src)             (((src) & 0x00020000)>>17)
#define INTERRUPT_PULSE_OR_LEVEL_WR(src)        (((u32)(src)<<17) & 0x00020000)
#define INTERRUPT_PULSE_OR_LEVEL_SET(dst,src) \
                      (((dst) & ~0x00020000) | (((u32)(src)<<17) & 0x00020000))
/*	 Fields Optimal_context_updates	 */
#define OPTIMAL_CONTEXT_UPDATES_WIDTH                                         1
#define OPTIMAL_CONTEXT_UPDATES_SHIFT                                        16
#define OPTIMAL_CONTEXT_UPDATES_MASK                                 0x00010000
#define OPTIMAL_CONTEXT_UPDATES_RD(src)              (((src) & 0x00010000)>>16)
#define OPTIMAL_CONTEXT_UPDATES_WR(src)         (((u32)(src)<<16) & 0x00010000)
#define OPTIMAL_CONTEXT_UPDATES_SET(dst,src) \
                      (((dst) & ~0x00010000) | (((u32)(src)<<16) & 0x00010000))
/*	 Fields Busy	 */
#define BUSY_WIDTH                                                            1
#define BUSY_SHIFT                                                           15
#define BUSY_MASK                                                    0x00008000
#define BUSY_RD(src)                                 (((src) & 0x00008000)>>15)
#define BUSY_WR(src)                            (((u32)(src)<<15) & 0x00008000)
#define BUSY_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))
/*	 Fields Processing_held_IDLE	 */
#define PROCESSING_HELD_IDLE_WIDTH                                            1
#define PROCESSING_HELD_IDLE_SHIFT                                           14
#define PROCESSING_HELD_IDLE_MASK                                    0x00004000
#define PROCESSING_HELD_IDLE_RD(src)                 (((src) & 0x00004000)>>14)
#define PROCESSING_HELD_IDLE_WR(src)            (((u32)(src)<<14) & 0x00004000)
#define PROCESSING_HELD_IDLE_SET(dst,src) \
                      (((dst) & ~0x00004000) | (((u32)(src)<<14) & 0x00004000))
/*	 Fields Packets_to_be_processed	 */
#define PACKETS_TO_BE_PROCESSED_WIDTH                                         6
#define PACKETS_TO_BE_PROCESSED_SHIFT                                         8
#define PACKETS_TO_BE_PROCESSED_MASK                                 0x00003f00
#define PACKETS_TO_BE_PROCESSED_RD(src)               (((src) & 0x00003f00)>>8)
#define PACKETS_TO_BE_PROCESSED_WR(src)          (((u32)(src)<<8) & 0x00003f00)
#define PACKETS_TO_BE_PROCESSED_SET(dst,src) \
                       (((dst) & ~0x00003f00) | (((u32)(src)<<8) & 0x00003f00))
/*	 Fields Result_context	 */
#define RESULT_CONTEXT_WIDTH                                                  1
#define RESULT_CONTEXT_SHIFT                                                  7
#define RESULT_CONTEXT_MASK                                          0x00000080
#define RESULT_CONTEXT_RD(src)                        (((src) & 0x00000080)>>7)
#define RESULT_CONTEXT_WR(src)                   (((u32)(src)<<7) & 0x00000080)
#define RESULT_CONTEXT_SET(dst,src) \
                       (((dst) & ~0x00000080) | (((u32)(src)<<7) & 0x00000080))
/*	 Fields Context_fetch	 */
#define CONTEXT_FETCH_WIDTH                                                   1
#define CONTEXT_FETCH_SHIFT                                                   6
#define CONTEXT_FETCH_MASK                                           0x00000040
#define CONTEXT_FETCH_RD(src)                         (((src) & 0x00000040)>>6)
#define CONTEXT_FETCH_WR(src)                    (((u32)(src)<<6) & 0x00000040)
#define CONTEXT_FETCH_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*	 Fields Context_cache_avctive	 */
#define CONTEXT_CACHE_AVCTIVE_WIDTH                                           1
#define CONTEXT_CACHE_AVCTIVE_SHIFT                                           5
#define CONTEXT_CACHE_AVCTIVE_MASK                                   0x00000020
#define CONTEXT_CACHE_AVCTIVE_RD(src)                 (((src) & 0x00000020)>>5)
#define CONTEXT_CACHE_AVCTIVE_WR(src)            (((u32)(src)<<5) & 0x00000020)
#define CONTEXT_CACHE_AVCTIVE_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields Token_read_active	 */
#define TOKEN_READ_ACTIVE_WIDTH                                               1
#define TOKEN_READ_ACTIVE_SHIFT                                               4
#define TOKEN_READ_ACTIVE_MASK                                       0x00000010
#define TOKEN_READ_ACTIVE_RD(src)                     (((src) & 0x00000010)>>4)
#define TOKEN_READ_ACTIVE_WR(src)                (((u32)(src)<<4) & 0x00000010)
#define TOKEN_READ_ACTIVE_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields Result_token_available	 */
#define RESULT_TOKEN_AVAILABLE_WIDTH                                          1
#define RESULT_TOKEN_AVAILABLE_SHIFT                                          3
#define RESULT_TOKEN_AVAILABLE_MASK                                  0x00000008
#define RESULT_TOKEN_AVAILABLE_RD(src)                (((src) & 0x00000008)>>3)
#define RESULT_TOKEN_AVAILABLE_WR(src)           (((u32)(src)<<3) & 0x00000008)
#define RESULT_TOKEN_AVAILABLE_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields Token_location_available	 */
#define TOKEN_LOCATION_AVAILABLE_WIDTH                                        1
#define TOKEN_LOCATION_AVAILABLE_SHIFT                                        2
#define TOKEN_LOCATION_AVAILABLE_MASK                                0x00000004
#define TOKEN_LOCATION_AVAILABLE_RD(src)              (((src) & 0x00000004)>>2)
#define TOKEN_LOCATION_AVAILABLE_WR(src)         (((u32)(src)<<2) & 0x00000004)
#define TOKEN_LOCATION_AVAILABLE_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields Active_tokens	 */
#define ACTIVE_TOKENS_WIDTH                                                   2
#define ACTIVE_TOKENS_SHIFT                                                   0
#define ACTIVE_TOKENS_MASK                                           0x00000003
#define ACTIVE_TOKENS_RD(src)                            (((src) & 0x00000003))
#define ACTIVE_TOKENS_WR(src)                       (((u32)(src)) & 0x00000003)
#define ACTIVE_TOKENS_SET(dst,src) \
                          (((dst) & ~0x00000003) | (((u32)(src)) & 0x00000003))

/*	Register IPE_PRC_ALG_EN	*/ 
/*	 Fields GCM_HASH	 */
#define GCM_HASH_WIDTH                                                        1
#define GCM_HASH_SHIFT                                                       31
#define GCM_HASH_MASK                                                0x80000000
#define GCM_HASH_RD(src)                             (((src) & 0x80000000)>>31)
#define GCM_HASH_WR(src)                        (((u32)(src)<<31) & 0x80000000)
#define GCM_HASH_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields AES_XCBC_MAC	 */
#define AES_XCBC_MAC_WIDTH                                                    1
#define AES_XCBC_MAC_SHIFT                                                   30
#define AES_XCBC_MAC_MASK                                            0x40000000
#define AES_XCBC_MAC_RD(src)                         (((src) & 0x40000000)>>30)
#define AES_XCBC_MAC_WR(src)                    (((u32)(src)<<30) & 0x40000000)
#define AES_XCBC_MAC_SET(dst,src) \
                      (((dst) & ~0x40000000) | (((u32)(src)<<30) & 0x40000000))
/*	 Fields HMAC_SHA2	 */
#define HMAC_SHA2_WIDTH                                                       1
#define HMAC_SHA2_SHIFT                                                      29
#define HMAC_SHA2_MASK                                               0x20000000
#define HMAC_SHA2_RD(src)                            (((src) & 0x20000000)>>29)
#define HMAC_SHA2_WR(src)                       (((u32)(src)<<29) & 0x20000000)
#define HMAC_SHA2_SET(dst,src) \
                      (((dst) & ~0x20000000) | (((u32)(src)<<29) & 0x20000000))
/*	 Fields basic_SHA2	 */
#define BASIC_SHA2_WIDTH                                                      1
#define BASIC_SHA2_SHIFT                                                     28
#define BASIC_SHA2_MASK                                              0x10000000
#define BASIC_SHA2_RD(src)                           (((src) & 0x10000000)>>28)
#define BASIC_SHA2_WR(src)                      (((u32)(src)<<28) & 0x10000000)
#define BASIC_SHA2_SET(dst,src) \
                      (((dst) & ~0x10000000) | (((u32)(src)<<28) & 0x10000000))
/*	 Fields HMAC_SHA1	 */
#define HMAC_SHA1_WIDTH                                                       1
#define HMAC_SHA1_SHIFT                                                      27
#define HMAC_SHA1_MASK                                               0x08000000
#define HMAC_SHA1_RD(src)                            (((src) & 0x08000000)>>27)
#define HMAC_SHA1_WR(src)                       (((u32)(src)<<27) & 0x08000000)
#define HMAC_SHA1_SET(dst,src) \
                      (((dst) & ~0x08000000) | (((u32)(src)<<27) & 0x08000000))
/*	 Fields basic_SHA1	 */
#define BASIC_SHA1_WIDTH                                                      1
#define BASIC_SHA1_SHIFT                                                     26
#define BASIC_SHA1_MASK                                              0x04000000
#define BASIC_SHA1_RD(src)                           (((src) & 0x04000000)>>26)
#define BASIC_SHA1_WR(src)                      (((u32)(src)<<26) & 0x04000000)
#define BASIC_SHA1_SET(dst,src) \
                      (((dst) & ~0x04000000) | (((u32)(src)<<26) & 0x04000000))
/*	 Fields HMAC_MD5	 */
#define HMAC_MD5_WIDTH                                                        1
#define HMAC_MD5_SHIFT                                                       25
#define HMAC_MD5_MASK                                                0x02000000
#define HMAC_MD5_RD(src)                             (((src) & 0x02000000)>>25)
#define HMAC_MD5_WR(src)                        (((u32)(src)<<25) & 0x02000000)
#define HMAC_MD5_SET(dst,src) \
                      (((dst) & ~0x02000000) | (((u32)(src)<<25) & 0x02000000))
/*	 Fields basic_MD5	 */
#define BASIC_MD5_WIDTH                                                       1
#define BASIC_MD5_SHIFT                                                      24
#define BASIC_MD5_MASK                                               0x01000000
#define BASIC_MD5_RD(src)                            (((src) & 0x01000000)>>24)
#define BASIC_MD5_WR(src)                       (((u32)(src)<<24) & 0x01000000)
#define BASIC_MD5_SET(dst,src) \
                      (((dst) & ~0x01000000) | (((u32)(src)<<24) & 0x01000000))
/*	 Fields Kasumi_f9	 */
#define KASUMI_F9_WIDTH                                                       1
#define KASUMI_F9_SHIFT                                                      23
#define KASUMI_F9_MASK                                               0x00800000
#define KASUMI_F9_RD(src)                            (((src) & 0x00800000)>>23)
#define KASUMI_F9_WR(src)                       (((u32)(src)<<23) & 0x00800000)
#define KASUMI_F9_SET(dst,src) \
                      (((dst) & ~0x00800000) | (((u32)(src)<<23) & 0x00800000))
/*	 Fields DES3_CFB	 */
#define DES3_CFB_WIDTH                                                        1
#define DES3_CFB_SHIFT                                                       22
#define DES3_CFB_MASK                                                0x00400000
#define DES3_CFB_RD(src)                             (((src) & 0x00400000)>>22)
#define DES3_CFB_WR(src)                        (((u32)(src)<<22) & 0x00400000)
#define DES3_CFB_SET(dst,src) \
                      (((dst) & ~0x00400000) | (((u32)(src)<<22) & 0x00400000))
/*	 Fields DES3_OFB	 */
#define DES3_OFB_WIDTH                                                        1
#define DES3_OFB_SHIFT                                                       21
#define DES3_OFB_MASK                                                0x00200000
#define DES3_OFB_RD(src)                             (((src) & 0x00200000)>>21)
#define DES3_OFB_WR(src)                        (((u32)(src)<<21) & 0x00200000)
#define DES3_OFB_SET(dst,src) \
                      (((dst) & ~0x00200000) | (((u32)(src)<<21) & 0x00200000))
/*	 Fields reserved2	 */
#define RESERVED2_WIDTH                                                       1
#define RESERVED2_SHIFT                                                      20
#define RESERVED2_MASK                                               0x00100000
#define RESERVED2_RD(src)                            (((src) & 0x00100000)>>20)
#define RESERVED2_WR(src)                       (((u32)(src)<<20) & 0x00100000)
#define RESERVED2_SET(dst,src) \
                      (((dst) & ~0x00100000) | (((u32)(src)<<20) & 0x00100000))
/*	 Fields DES3_CBC	 */
#define DES3_CBC_WIDTH                                                        1
#define DES3_CBC_SHIFT                                                       19
#define DES3_CBC_MASK                                                0x00080000
#define DES3_CBC_RD(src)                             (((src) & 0x00080000)>>19)
#define DES3_CBC_WR(src)                        (((u32)(src)<<19) & 0x00080000)
#define DES3_CBC_SET(dst,src) \
                      (((dst) & ~0x00080000) | (((u32)(src)<<19) & 0x00080000))
/*	 Fields DES3_ECB	 */
#define DES3_ECB_WIDTH                                                        1
#define DES3_ECB_SHIFT                                                       18
#define DES3_ECB_MASK                                                0x00040000
#define DES3_ECB_RD(src)                             (((src) & 0x00040000)>>18)
#define DES3_ECB_WR(src)                        (((u32)(src)<<18) & 0x00040000)
#define DES3_ECB_SET(dst,src) \
                      (((dst) & ~0x00040000) | (((u32)(src)<<18) & 0x00040000))
/*	 Fields DES_CFB	 */
#define DES_CFB_WIDTH                                                         1
#define DES_CFB_SHIFT                                                        17
#define DES_CFB_MASK                                                 0x00020000
#define DES_CFB_RD(src)                              (((src) & 0x00020000)>>17)
#define DES_CFB_WR(src)                         (((u32)(src)<<17) & 0x00020000)
#define DES_CFB_SET(dst,src) \
                      (((dst) & ~0x00020000) | (((u32)(src)<<17) & 0x00020000))
/*	 Fields DES_OFB	 */
#define DES_OFB_WIDTH                                                         1
#define DES_OFB_SHIFT                                                        16
#define DES_OFB_MASK                                                 0x00010000
#define DES_OFB_RD(src)                              (((src) & 0x00010000)>>16)
#define DES_OFB_WR(src)                         (((u32)(src)<<16) & 0x00010000)
#define DES_OFB_SET(dst,src) \
                      (((dst) & ~0x00010000) | (((u32)(src)<<16) & 0x00010000))
/*	 Fields reserved1	 */
#define RESERVED1_WIDTH                                                       1
#define RESERVED1_SHIFT                                                      15
#define RESERVED1_MASK                                               0x00008000
#define RESERVED1_RD(src)                            (((src) & 0x00008000)>>15)
#define RESERVED1_WR(src)                       (((u32)(src)<<15) & 0x00008000)
#define RESERVED1_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))
/*	 Fields DES_CBC	 */
#define DES_CBC_WIDTH                                                         1
#define DES_CBC_SHIFT                                                        14
#define DES_CBC_MASK                                                 0x00004000
#define DES_CBC_RD(src)                              (((src) & 0x00004000)>>14)
#define DES_CBC_WR(src)                         (((u32)(src)<<14) & 0x00004000)
#define DES_CBC_SET(dst,src) \
                      (((dst) & ~0x00004000) | (((u32)(src)<<14) & 0x00004000))
/*	 Fields DES_ECB	 */
#define DES_ECB_WIDTH                                                         1
#define DES_ECB_SHIFT                                                        13
#define DES_ECB_MASK                                                 0x00002000
#define DES_ECB_RD(src)                              (((src) & 0x00002000)>>13)
#define DES_ECB_WR(src)                         (((u32)(src)<<13) & 0x00002000)
#define DES_ECB_SET(dst,src) \
                      (((dst) & ~0x00002000) | (((u32)(src)<<13) & 0x00002000))
/*	 Fields AES_CFB	 */
#define AES_CFB_WIDTH                                                         1
#define AES_CFB_SHIFT                                                        12
#define AES_CFB_MASK                                                 0x00001000
#define AES_CFB_RD(src)                              (((src) & 0x00001000)>>12)
#define AES_CFB_WR(src)                         (((u32)(src)<<12) & 0x00001000)
#define AES_CFB_SET(dst,src) \
                      (((dst) & ~0x00001000) | (((u32)(src)<<12) & 0x00001000))
/*	 Fields AES_OFB	 */
#define AES_OFB_WIDTH                                                         1
#define AES_OFB_SHIFT                                                        11
#define AES_OFB_MASK                                                 0x00000800
#define AES_OFB_RD(src)                              (((src) & 0x00000800)>>11)
#define AES_OFB_WR(src)                         (((u32)(src)<<11) & 0x00000800)
#define AES_OFB_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*	 Fields AES_CTR_ICM	 */
#define AES_CTR_ICM_WIDTH                                                     1
#define AES_CTR_ICM_SHIFT                                                    10
#define AES_CTR_ICM_MASK                                             0x00000400
#define AES_CTR_ICM_RD(src)                          (((src) & 0x00000400)>>10)
#define AES_CTR_ICM_WR(src)                     (((u32)(src)<<10) & 0x00000400)
#define AES_CTR_ICM_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields AES_CBC	 */
#define AES_CBC_WIDTH                                                         1
#define AES_CBC_SHIFT                                                         9
#define AES_CBC_MASK                                                 0x00000200
#define AES_CBC_RD(src)                               (((src) & 0x00000200)>>9)
#define AES_CBC_WR(src)                          (((u32)(src)<<9) & 0x00000200)
#define AES_CBC_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields AES_EBC	 */
#define AES_EBC_WIDTH                                                         1
#define AES_EBC_SHIFT                                                         8
#define AES_EBC_MASK                                                 0x00000100
#define AES_EBC_RD(src)                               (((src) & 0x00000100)>>8)
#define AES_EBC_WR(src)                          (((u32)(src)<<8) & 0x00000100)
#define AES_EBC_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*	 Fields ARC4	 */
#define ARC4_WIDTH                                                            1
#define ARC4_SHIFT                                                            7
#define ARC4_MASK                                                    0x00000080
#define ARC4_RD(src)                                  (((src) & 0x00000080)>>7)
#define ARC4_WR(src)                             (((u32)(src)<<7) & 0x00000080)
#define ARC4_SET(dst,src) \
                       (((dst) & ~0x00000080) | (((u32)(src)<<7) & 0x00000080))
/*	 Fields Kasumi_Kasumi_f8	 */
#define KASUMI_KASUMI_F8_WIDTH                                                1
#define KASUMI_KASUMI_F8_SHIFT                                                6
#define KASUMI_KASUMI_F8_MASK                                        0x00000040
#define KASUMI_KASUMI_F8_RD(src)                      (((src) & 0x00000040)>>6)
#define KASUMI_KASUMI_F8_WR(src)                 (((u32)(src)<<6) & 0x00000040)
#define KASUMI_KASUMI_F8_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*	 Fields decrypt_hash	 */
#define DECRYPT_HASH_WIDTH                                                    1
#define DECRYPT_HASH_SHIFT                                                    5
#define DECRYPT_HASH_MASK                                            0x00000020
#define DECRYPT_HASH_RD(src)                          (((src) & 0x00000020)>>5)
#define DECRYPT_HASH_WR(src)                     (((u32)(src)<<5) & 0x00000020)
#define DECRYPT_HASH_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields encrypt_hash	 */
#define ENCRYPT_HASH_WIDTH                                                    1
#define ENCRYPT_HASH_SHIFT                                                    4
#define ENCRYPT_HASH_MASK                                            0x00000010
#define ENCRYPT_HASH_RD(src)                          (((src) & 0x00000010)>>4)
#define ENCRYPT_HASH_WR(src)                     (((u32)(src)<<4) & 0x00000010)
#define ENCRYPT_HASH_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields hash_decrypt	 */
#define HASH_DECRYPT_WIDTH                                                    1
#define HASH_DECRYPT_SHIFT                                                    3
#define HASH_DECRYPT_MASK                                            0x00000008
#define HASH_DECRYPT_RD(src)                          (((src) & 0x00000008)>>3)
#define HASH_DECRYPT_WR(src)                     (((u32)(src)<<3) & 0x00000008)
#define HASH_DECRYPT_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields hash_encrypt	 */
#define HASH_ENCRYPT_WIDTH                                                    1
#define HASH_ENCRYPT_SHIFT                                                    2
#define HASH_ENCRYPT_MASK                                            0x00000004
#define HASH_ENCRYPT_RD(src)                          (((src) & 0x00000004)>>2)
#define HASH_ENCRYPT_WR(src)                     (((u32)(src)<<2) & 0x00000004)
#define HASH_ENCRYPT_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields encrypt_only	 */
#define ENCRYPT_ONLY_WIDTH                                                    1
#define ENCRYPT_ONLY_SHIFT                                                    1
#define ENCRYPT_ONLY_MASK                                            0x00000002
#define ENCRYPT_ONLY_RD(src)                          (((src) & 0x00000002)>>1)
#define ENCRYPT_ONLY_WR(src)                     (((u32)(src)<<1) & 0x00000002)
#define ENCRYPT_ONLY_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields hash_only_null	 */
#define HASH_ONLY_NULL_WIDTH                                                  1
#define HASH_ONLY_NULL_SHIFT                                                  0
#define HASH_ONLY_NULL_MASK                                          0x00000001
#define HASH_ONLY_NULL_RD(src)                           (((src) & 0x00000001))
#define HASH_ONLY_NULL_WR(src)                      (((u32)(src)) & 0x00000001)
#define HASH_ONLY_NULL_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register IPE_CTX_CTRL	*/ 
/*	 Fields rsvd	 */
#define RSVD_WIDTH                                                           22
#define RSVD_SHIFT                                                           10
#define RSVD_MASK                                                    0xfffffc00
#define RSVD_RD(src)                                 (((src) & 0xfffffc00)>>10)
#define RSVD_WR(src)                            (((u32)(src)<<10) & 0xfffffc00)
#define RSVD_SET(dst,src) \
                      (((dst) & ~0xfffffc00) | (((u32)(src)<<10) & 0xfffffc00))
/*	 Fields Control_mode	 */
#define SEC_CONTROL_MODE_WIDTH                                                1
#define SEC_CONTROL_MODE_SHIFT                                                9
#define SEC_CONTROL_MODE_MASK                                        0x00000200
#define SEC_CONTROL_MODE_RD(src)                      (((src) & 0x00000200)>>9)
#define SEC_CONTROL_MODE_WR(src)                 (((u32)(src)<<9) & 0x00000200)
#define SEC_CONTROL_MODE_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields Address_mode	 */
#define SEC_ADDRESS_MODE_WIDTH                                                1
#define SEC_ADDRESS_MODE_SHIFT                                                8
#define SEC_ADDRESS_MODE_MASK                                        0x00000100
#define SEC_ADDRESS_MODE_RD(src)                      (((src) & 0x00000100)>>8)
#define SEC_ADDRESS_MODE_WR(src)                 (((u32)(src)<<8) & 0x00000100)
#define SEC_ADDRESS_MODE_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*	 Fields Context_size	 */
#define CONTEXT_SIZE_WIDTH                                                    8
#define CONTEXT_SIZE_SHIFT                                                    0
#define CONTEXT_SIZE_MASK                                            0x000000ff
#define CONTEXT_SIZE_RD(src)                             (((src) & 0x000000ff))
#define CONTEXT_SIZE_WR(src)                        (((u32)(src)) & 0x000000ff)
#define CONTEXT_SIZE_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register IPE_CTX_STAT	*/ 
/*	 Fields Next_Packet_Current_State	 */
#define NEXT_PACKET_CURRENT_STATE_WIDTH                                       4
#define NEXT_PACKET_CURRENT_STATE_SHIFT                                      28
#define NEXT_PACKET_CURRENT_STATE_MASK                               0xf0000000
#define NEXT_PACKET_CURRENT_STATE_RD(src)            (((src) & 0xf0000000)>>28)
#define NEXT_PACKET_CURRENT_STATE_WR(src)       (((u32)(src)<<28) & 0xf0000000)
#define NEXT_PACKET_CURRENT_STATE_SET(dst,src) \
                      (((dst) & ~0xf0000000) | (((u32)(src)<<28) & 0xf0000000))
/*	 Fields Active_Packet_Processing_Current_State	 */
#define ACTIVE_PACKET_PROCESSING_CURRENT_STATE_WIDTH                          4
#define ACTIVE_PACKET_PROCESSING_CURRENT_STATE_SHIFT                         24
#define ACTIVE_PACKET_PROCESSING_CURRENT_STATE_MASK                  0x0f000000
#define ACTIVE_PACKET_PROCESSING_CURRENT_STATE_RD(src) \
                                                    (((src) & 0x0f000000)>>24)
#define ACTIVE_PACKET_PROCESSING_CURRENT_STATE_WR(src) \
                                                (((u32)(src)<<24) & 0x0f000000)
#define ACTIVE_PACKET_PROCESSING_CURRENT_STATE_SET(dst,src) \
                      (((dst) & ~0x0f000000) | (((u32)(src)<<24) & 0x0f000000))
/*	 Fields rsvd2	 */
#define RSVD2_F1_WIDTH                                                        2
#define RSVD2_F1_SHIFT                                                       22
#define RSVD2_F1_MASK                                                0x00c00000
#define RSVD2_F1_RD(src)                             (((src) & 0x00c00000)>>22)
#define RSVD2_F1_WR(src)                        (((u32)(src)<<22) & 0x00c00000)
#define RSVD2_F1_SET(dst,src) \
                      (((dst) & ~0x00c00000) | (((u32)(src)<<22) & 0x00c00000))
/*	 Fields Error_recovery	 */
#define ERROR_RECOVERY_WIDTH                                                  1
#define ERROR_RECOVERY_SHIFT                                                 21
#define ERROR_RECOVERY_MASK                                          0x00200000
#define ERROR_RECOVERY_RD(src)                       (((src) & 0x00200000)>>21)
#define ERROR_RECOVERY_WR(src)                  (((u32)(src)<<21) & 0x00200000)
#define ERROR_RECOVERY_SET(dst,src) \
                      (((dst) & ~0x00200000) | (((u32)(src)<<21) & 0x00200000))
/*	 Fields Result_Context	 */
#define RESULT_CONTEXT_F1_WIDTH                                               1
#define RESULT_CONTEXT_F1_SHIFT                                              20
#define RESULT_CONTEXT_F1_MASK                                       0x00100000
#define RESULT_CONTEXT_F1_RD(src)                    (((src) & 0x00100000)>>20)
#define RESULT_CONTEXT_F1_WR(src)               (((u32)(src)<<20) & 0x00100000)
#define RESULT_CONTEXT_F1_SET(dst,src) \
                      (((dst) & ~0x00100000) | (((u32)(src)<<20) & 0x00100000))
/*	 Fields Next_Context	 */
#define NEXT_CONTEXT_WIDTH                                                    1
#define NEXT_CONTEXT_SHIFT                                                   19
#define NEXT_CONTEXT_MASK                                            0x00080000
#define NEXT_CONTEXT_RD(src)                         (((src) & 0x00080000)>>19)
#define NEXT_CONTEXT_WR(src)                    (((u32)(src)<<19) & 0x00080000)
#define NEXT_CONTEXT_SET(dst,src) \
                      (((dst) & ~0x00080000) | (((u32)(src)<<19) & 0x00080000))
/*	 Fields Active_Context	 */
#define ACTIVE_CONTEXT_WIDTH                                                  1
#define ACTIVE_CONTEXT_SHIFT                                                 18
#define ACTIVE_CONTEXT_MASK                                          0x00040000
#define ACTIVE_CONTEXT_RD(src)                       (((src) & 0x00040000)>>18)
#define ACTIVE_CONTEXT_WR(src)                  (((u32)(src)<<18) & 0x00040000)
#define ACTIVE_CONTEXT_SET(dst,src) \
                      (((dst) & ~0x00040000) | (((u32)(src)<<18) & 0x00040000))
/*	 Fields No_Available_Tokens	 */
#define NO_AVAILABLE_TOKENS_WIDTH                                             2
#define NO_AVAILABLE_TOKENS_SHIFT                                            16
#define NO_AVAILABLE_TOKENS_MASK                                     0x00030000
#define NO_AVAILABLE_TOKENS_RD(src)                  (((src) & 0x00030000)>>16)
#define NO_AVAILABLE_TOKENS_WR(src)             (((u32)(src)<<16) & 0x00030000)
#define NO_AVAILABLE_TOKENS_SET(dst,src) \
                      (((dst) & ~0x00030000) | (((u32)(src)<<16) & 0x00030000))
/*	 Fields rsvd1	 */
#define RSVD1_F1_WIDTH                                                        1
#define RSVD1_F1_SHIFT                                                       15
#define RSVD1_F1_MASK                                                0x00008000
#define RSVD1_F1_RD(src)                             (((src) & 0x00008000)>>15)
#define RSVD1_F1_WR(src)                        (((u32)(src)<<15) & 0x00008000)
#define RSVD1_F1_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))
/*	 Fields E14	 */
#define E14_WIDTH                                                             1
#define E14_SHIFT                                                            14
#define E14_MASK                                                     0x00004000
#define E14_RD(src)                                  (((src) & 0x00004000)>>14)
#define E14_WR(src)                             (((u32)(src)<<14) & 0x00004000)
#define E14_SET(dst,src) \
                      (((dst) & ~0x00004000) | (((u32)(src)<<14) & 0x00004000))
/*	 Fields E13	 */
#define E13_WIDTH                                                             1
#define E13_SHIFT                                                            13
#define E13_MASK                                                     0x00002000
#define E13_RD(src)                                  (((src) & 0x00002000)>>13)
#define E13_WR(src)                             (((u32)(src)<<13) & 0x00002000)
#define E13_SET(dst,src) \
                      (((dst) & ~0x00002000) | (((u32)(src)<<13) & 0x00002000))
/*	 Fields E12	 */
#define E12_WIDTH                                                             1
#define E12_SHIFT                                                            12
#define E12_MASK                                                     0x00001000
#define E12_RD(src)                                  (((src) & 0x00001000)>>12)
#define E12_WR(src)                             (((u32)(src)<<12) & 0x00001000)
#define E12_SET(dst,src) \
                      (((dst) & ~0x00001000) | (((u32)(src)<<12) & 0x00001000))
/*	 Fields E11	 */
#define E11_WIDTH                                                             1
#define E11_SHIFT                                                            11
#define E11_MASK                                                     0x00000800
#define E11_RD(src)                                  (((src) & 0x00000800)>>11)
#define E11_WR(src)                             (((u32)(src)<<11) & 0x00000800)
#define E11_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*	 Fields E10	 */
#define E10_WIDTH                                                             1
#define E10_SHIFT                                                            10
#define E10_MASK                                                     0x00000400
#define E10_RD(src)                                  (((src) & 0x00000400)>>10)
#define E10_WR(src)                             (((u32)(src)<<10) & 0x00000400)
#define E10_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields E9	 */
#define E9_WIDTH                                                              1
#define E9_SHIFT                                                              9
#define E9_MASK                                                      0x00000200
#define E9_RD(src)                                    (((src) & 0x00000200)>>9)
#define E9_WR(src)                               (((u32)(src)<<9) & 0x00000200)
#define E9_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields E8	 */
#define E8_WIDTH                                                              1
#define E8_SHIFT                                                              8
#define E8_MASK                                                      0x00000100
#define E8_RD(src)                                    (((src) & 0x00000100)>>8)
#define E8_WR(src)                               (((u32)(src)<<8) & 0x00000100)
#define E8_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*	 Fields E7	 */
#define E7_WIDTH                                                              1
#define E7_SHIFT                                                              7
#define E7_MASK                                                      0x00000080
#define E7_RD(src)                                    (((src) & 0x00000080)>>7)
#define E7_WR(src)                               (((u32)(src)<<7) & 0x00000080)
#define E7_SET(dst,src) \
                       (((dst) & ~0x00000080) | (((u32)(src)<<7) & 0x00000080))
/*	 Fields E6	 */
#define E6_WIDTH                                                              1
#define E6_SHIFT                                                              6
#define E6_MASK                                                      0x00000040
#define E6_RD(src)                                    (((src) & 0x00000040)>>6)
#define E6_WR(src)                               (((u32)(src)<<6) & 0x00000040)
#define E6_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*	 Fields E5	 */
#define E5_WIDTH                                                              1
#define E5_SHIFT                                                              5
#define E5_MASK                                                      0x00000020
#define E5_RD(src)                                    (((src) & 0x00000020)>>5)
#define E5_WR(src)                               (((u32)(src)<<5) & 0x00000020)
#define E5_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields E4	 */
#define E4_WIDTH                                                              1
#define E4_SHIFT                                                              4
#define E4_MASK                                                      0x00000010
#define E4_RD(src)                                    (((src) & 0x00000010)>>4)
#define E4_WR(src)                               (((u32)(src)<<4) & 0x00000010)
#define E4_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields E3	 */
#define E3_WIDTH                                                              1
#define E3_SHIFT                                                              3
#define E3_MASK                                                      0x00000008
#define E3_RD(src)                                    (((src) & 0x00000008)>>3)
#define E3_WR(src)                               (((u32)(src)<<3) & 0x00000008)
#define E3_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields E2	 */
#define E2_WIDTH                                                              1
#define E2_SHIFT                                                              2
#define E2_MASK                                                      0x00000004
#define E2_RD(src)                                    (((src) & 0x00000004)>>2)
#define E2_WR(src)                               (((u32)(src)<<2) & 0x00000004)
#define E2_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields E1	 */
#define E1_WIDTH                                                              1
#define E1_SHIFT                                                              1
#define E1_MASK                                                      0x00000002
#define E1_RD(src)                                    (((src) & 0x00000002)>>1)
#define E1_WR(src)                               (((u32)(src)<<1) & 0x00000002)
#define E1_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields E0	 */
#define E0_WIDTH                                                              1
#define E0_SHIFT                                                              0
#define E0_MASK                                                      0x00000001
#define E0_RD(src)                                       (((src) & 0x00000001))
#define E0_WR(src)                                  (((u32)(src)) & 0x00000001)
#define E0_SET(dst,src) (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register IPE_INT_CTRL_STAT	*/ 
/*	 Fields Interrupt_output_pin_enable	 */
#define INTERRUPT_OUTPUT_PIN_ENABLE_WIDTH                                     1
#define INTERRUPT_OUTPUT_PIN_ENABLE_SHIFT                                    31
#define INTERRUPT_OUTPUT_PIN_ENABLE_MASK                             0x80000000
#define INTERRUPT_OUTPUT_PIN_ENABLE_RD(src)          (((src) & 0x80000000)>>31)
#define INTERRUPT_OUTPUT_PIN_ENABLE_WR(src)     (((u32)(src)<<31) & 0x80000000)
#define INTERRUPT_OUTPUT_PIN_ENABLE_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields Fatal_error_enable	 */
#define FATAL_ERROR_ENABLE_WIDTH                                              1
#define FATAL_ERROR_ENABLE_SHIFT                                             30
#define FATAL_ERROR_ENABLE_MASK                                      0x40000000
#define FATAL_ERROR_ENABLE_RD(src)                   (((src) & 0x40000000)>>30)
#define FATAL_ERROR_ENABLE_WR(src)              (((u32)(src)<<30) & 0x40000000)
#define FATAL_ERROR_ENABLE_SET(dst,src) \
                      (((dst) & ~0x40000000) | (((u32)(src)<<30) & 0x40000000))
/*	 Fields rsvd2	 */
#define RSVD2_F2_WIDTH                                                       10
#define RSVD2_F2_SHIFT                                                       20
#define RSVD2_F2_MASK                                                0x3ff00000
#define RSVD2_F2_RD(src)                             (((src) & 0x3ff00000)>>20)
#define RSVD2_F2_WR(src)                        (((u32)(src)<<20) & 0x3ff00000)
#define RSVD2_F2_SET(dst,src) \
                      (((dst) & ~0x3ff00000) | (((u32)(src)<<20) & 0x3ff00000))
/*	 Fields Packet_timeout_enable	 */
#define PACKET_TIMEOUT_ENABLE_WIDTH                                           1
#define PACKET_TIMEOUT_ENABLE_SHIFT                                          19
#define PACKET_TIMEOUT_ENABLE_MASK                                   0x00080000
#define PACKET_TIMEOUT_ENABLE_RD(src)                (((src) & 0x00080000)>>19)
#define PACKET_TIMEOUT_ENABLE_WR(src)           (((u32)(src)<<19) & 0x00080000)
#define PACKET_TIMEOUT_ENABLE_SET(dst,src) \
                      (((dst) & ~0x00080000) | (((u32)(src)<<19) & 0x00080000))
/*	 Fields Packet_processing_enable	 */
#define PACKET_PROCESSING_ENABLE_WIDTH                                        1
#define PACKET_PROCESSING_ENABLE_SHIFT                                       18
#define PACKET_PROCESSING_ENABLE_MASK                                0x00040000
#define PACKET_PROCESSING_ENABLE_RD(src)             (((src) & 0x00040000)>>18)
#define PACKET_PROCESSING_ENABLE_WR(src)        (((u32)(src)<<18) & 0x00040000)
#define PACKET_PROCESSING_ENABLE_SET(dst,src) \
                      (((dst) & ~0x00040000) | (((u32)(src)<<18) & 0x00040000))
/*	 Fields Output_DMA_error_enable	 */
#define OUTPUT_DMA_ERROR_ENABLE_WIDTH                                         1
#define OUTPUT_DMA_ERROR_ENABLE_SHIFT                                        17
#define OUTPUT_DMA_ERROR_ENABLE_MASK                                 0x00020000
#define OUTPUT_DMA_ERROR_ENABLE_RD(src)              (((src) & 0x00020000)>>17)
#define OUTPUT_DMA_ERROR_ENABLE_WR(src)         (((u32)(src)<<17) & 0x00020000)
#define OUTPUT_DMA_ERROR_ENABLE_SET(dst,src) \
                      (((dst) & ~0x00020000) | (((u32)(src)<<17) & 0x00020000))
/*	 Fields Input_DMA_error_enable	 */
#define INPUT_DMA_ERROR_ENABLE_WIDTH                                          1
#define INPUT_DMA_ERROR_ENABLE_SHIFT                                         16
#define INPUT_DMA_ERROR_ENABLE_MASK                                  0x00010000
#define INPUT_DMA_ERROR_ENABLE_RD(src)               (((src) & 0x00010000)>>16)
#define INPUT_DMA_ERROR_ENABLE_WR(src)          (((u32)(src)<<16) & 0x00010000)
#define INPUT_DMA_ERROR_ENABLE_SET(dst,src) \
                      (((dst) & ~0x00010000) | (((u32)(src)<<16) & 0x00010000))
/*	 Fields Interrupt_output_pin_status	 */
#define INTERRUPT_OUTPUT_PIN_STATUS_WIDTH                                     1
#define INTERRUPT_OUTPUT_PIN_STATUS_SHIFT                                    15
#define INTERRUPT_OUTPUT_PIN_STATUS_MASK                             0x00008000
#define INTERRUPT_OUTPUT_PIN_STATUS_RD(src)          (((src) & 0x00008000)>>15)
#define INTERRUPT_OUTPUT_PIN_STATUS_WR(src)     (((u32)(src)<<15) & 0x00008000)
#define INTERRUPT_OUTPUT_PIN_STATUS_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))
/*	 Fields Fatal_error_status	 */
#define FATAL_ERROR_STATUS_WIDTH                                              1
#define FATAL_ERROR_STATUS_SHIFT                                             14
#define FATAL_ERROR_STATUS_MASK                                      0x00004000
#define FATAL_ERROR_STATUS_RD(src)                   (((src) & 0x00004000)>>14)
#define FATAL_ERROR_STATUS_WR(src)              (((u32)(src)<<14) & 0x00004000)
#define FATAL_ERROR_STATUS_SET(dst,src) \
                      (((dst) & ~0x00004000) | (((u32)(src)<<14) & 0x00004000))
/*	 Fields rsvd1	 */
#define RSVD1_F2_WIDTH                                                       10
#define RSVD1_F2_SHIFT                                                        4
#define RSVD1_F2_MASK                                                0x00003ff0
#define RSVD1_F2_RD(src)                              (((src) & 0x00003ff0)>>4)
#define RSVD1_F2_WR(src)                         (((u32)(src)<<4) & 0x00003ff0)
#define RSVD1_F2_SET(dst,src) \
                       (((dst) & ~0x00003ff0) | (((u32)(src)<<4) & 0x00003ff0))
/*	 Fields Packet_timeout_status	 */
#define PACKET_TIMEOUT_STATUS_WIDTH                                           1
#define PACKET_TIMEOUT_STATUS_SHIFT                                           3
#define PACKET_TIMEOUT_STATUS_MASK                                   0x00000008
#define PACKET_TIMEOUT_STATUS_RD(src)                 (((src) & 0x00000008)>>3)
#define PACKET_TIMEOUT_STATUS_WR(src)            (((u32)(src)<<3) & 0x00000008)
#define PACKET_TIMEOUT_STATUS_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields Packet_processing_status	 */
#define PACKET_PROCESSING_STATUS_WIDTH                                        1
#define PACKET_PROCESSING_STATUS_SHIFT                                        2
#define PACKET_PROCESSING_STATUS_MASK                                0x00000004
#define PACKET_PROCESSING_STATUS_RD(src)              (((src) & 0x00000004)>>2)
#define PACKET_PROCESSING_STATUS_WR(src)         (((u32)(src)<<2) & 0x00000004)
#define PACKET_PROCESSING_STATUS_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields Output_DMA_error_status	 */
#define OUTPUT_DMA_ERROR_STATUS_WIDTH                                         1
#define OUTPUT_DMA_ERROR_STATUS_SHIFT                                         1
#define OUTPUT_DMA_ERROR_STATUS_MASK                                 0x00000002
#define OUTPUT_DMA_ERROR_STATUS_RD(src)               (((src) & 0x00000002)>>1)
#define OUTPUT_DMA_ERROR_STATUS_WR(src)          (((u32)(src)<<1) & 0x00000002)
#define OUTPUT_DMA_ERROR_STATUS_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields Input_DMA_error_status	 */
#define INPUT_DMA_ERROR_STATUS_WIDTH                                          1
#define INPUT_DMA_ERROR_STATUS_SHIFT                                          0
#define INPUT_DMA_ERROR_STATUS_MASK                                  0x00000001
#define INPUT_DMA_ERROR_STATUS_RD(src)                   (((src) & 0x00000001))
#define INPUT_DMA_ERROR_STATUS_WR(src)              (((u32)(src)) & 0x00000001)
#define INPUT_DMA_ERROR_STATUS_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register IPE_RX_CTRL_STAT	*/ 
/*	 Fields Transfer_size_mask	 */
#define TRANSFER_SIZE_MASK_WIDTH                                              8
#define TRANSFER_SIZE_MASK_SHIFT                                             24
#define TRANSFER_SIZE_MASK_MASK                                      0xff000000
#define TRANSFER_SIZE_MASK_RD(src)                   (((src) & 0xff000000)>>24)
#define TRANSFER_SIZE_MASK_WR(src)              (((u32)(src)<<24) & 0xff000000)
#define TRANSFER_SIZE_MASK_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))
/*	 Fields Max_transfer_size	 */
#define MAX_TRANSFER_SIZE_WIDTH                                               8
#define MAX_TRANSFER_SIZE_SHIFT                                              16
#define MAX_TRANSFER_SIZE_MASK                                       0x00ff0000
#define MAX_TRANSFER_SIZE_RD(src)                    (((src) & 0x00ff0000)>>16)
#define MAX_TRANSFER_SIZE_WR(src)               (((u32)(src)<<16) & 0x00ff0000)
#define MAX_TRANSFER_SIZE_SET(dst,src) \
                      (((dst) & ~0x00ff0000) | (((u32)(src)<<16) & 0x00ff0000))
/*	 Fields Min_transfer_size	 */
#define MIN_TRANSFER_SIZE_WIDTH                                               8
#define MIN_TRANSFER_SIZE_SHIFT                                               8
#define MIN_TRANSFER_SIZE_MASK                                       0x0000ff00
#define MIN_TRANSFER_SIZE_RD(src)                     (((src) & 0x0000ff00)>>8)
#define MIN_TRANSFER_SIZE_WR(src)                (((u32)(src)<<8) & 0x0000ff00)
#define MIN_TRANSFER_SIZE_SET(dst,src) \
                       (((dst) & ~0x0000ff00) | (((u32)(src)<<8) & 0x0000ff00))
/*	 Fields Available_dwords	 */
#define AVAILABLE_DWORDS_WIDTH                                                8
#define AVAILABLE_DWORDS_SHIFT                                                0
#define AVAILABLE_DWORDS_MASK                                        0x000000ff
#define AVAILABLE_DWORDS_RD(src)                         (((src) & 0x000000ff))
#define AVAILABLE_DWORDS_WR(src)                    (((u32)(src)) & 0x000000ff)
#define AVAILABLE_DWORDS_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register IPE_TX_CTRL_STAT	*/ 
/*	 Fields Transfer_size_mask	 */
#define TRANSFER_SIZE_MASK_F1_WIDTH                                           8
#define TRANSFER_SIZE_MASK_F1_SHIFT                                          24
#define TRANSFER_SIZE_MASK_F1_MASK                                   0xff000000
#define TRANSFER_SIZE_MASK_F1_RD(src)                (((src) & 0xff000000)>>24)
#define TRANSFER_SIZE_MASK_F1_WR(src)           (((u32)(src)<<24) & 0xff000000)
#define TRANSFER_SIZE_MASK_F1_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))
/*	 Fields Max_transfer_size	 */
#define MAX_TRANSFER_SIZE_F1_WIDTH                                            8
#define MAX_TRANSFER_SIZE_F1_SHIFT                                           16
#define MAX_TRANSFER_SIZE_F1_MASK                                    0x00ff0000
#define MAX_TRANSFER_SIZE_F1_RD(src)                 (((src) & 0x00ff0000)>>16)
#define MAX_TRANSFER_SIZE_F1_WR(src)            (((u32)(src)<<16) & 0x00ff0000)
#define MAX_TRANSFER_SIZE_F1_SET(dst,src) \
                      (((dst) & ~0x00ff0000) | (((u32)(src)<<16) & 0x00ff0000))
/*	 Fields Min_transfer_size	 */
#define MIN_TRANSFER_SIZE_F1_WIDTH                                            8
#define MIN_TRANSFER_SIZE_F1_SHIFT                                            8
#define MIN_TRANSFER_SIZE_F1_MASK                                    0x0000ff00
#define MIN_TRANSFER_SIZE_F1_RD(src)                  (((src) & 0x0000ff00)>>8)
#define MIN_TRANSFER_SIZE_F1_WR(src)             (((u32)(src)<<8) & 0x0000ff00)
#define MIN_TRANSFER_SIZE_F1_SET(dst,src) \
                       (((dst) & ~0x0000ff00) | (((u32)(src)<<8) & 0x0000ff00))
/*	 Fields Available_dwords	 */
#define AVAILABLE_DWORDS_F1_WIDTH                                             8
#define AVAILABLE_DWORDS_F1_SHIFT                                             0
#define AVAILABLE_DWORDS_F1_MASK                                     0x000000ff
#define AVAILABLE_DWORDS_F1_RD(src)                      (((src) & 0x000000ff))
#define AVAILABLE_DWORDS_F1_WR(src)                 (((u32)(src)) & 0x000000ff)
#define AVAILABLE_DWORDS_F1_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register IPE_DEV_INFO	*/ 
/*	 Fields Kasumi_f9	 */
#define KASUMI_F9_F1_WIDTH                                                    1
#define KASUMI_F9_F1_SHIFT                                                   31
#define KASUMI_F9_F1_MASK                                            0x80000000
#define KASUMI_F9_F1_RD(src)                         (((src) & 0x80000000)>>31)
#define KASUMI_F9_F1_WR(src)                    (((u32)(src)<<31) & 0x80000000)
#define KASUMI_F9_F1_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields GHASH	 */
#define GHASH_WIDTH                                                           1
#define GHASH_SHIFT                                                          30
#define GHASH_MASK                                                   0x40000000
#define GHASH_RD(src)                                (((src) & 0x40000000)>>30)
#define GHASH_WR(src)                           (((u32)(src)<<30) & 0x40000000)
#define GHASH_SET(dst,src) \
                      (((dst) & ~0x40000000) | (((u32)(src)<<30) & 0x40000000))
/*	 Fields CBC_MAC_key_lengths	 */
#define CBC_MAC_KEY_LENGTHS_WIDTH                                             1
#define CBC_MAC_KEY_LENGTHS_SHIFT                                            29
#define CBC_MAC_KEY_LENGTHS_MASK                                     0x20000000
#define CBC_MAC_KEY_LENGTHS_RD(src)                  (((src) & 0x20000000)>>29)
#define CBC_MAC_KEY_LENGTHS_WR(src)             (((u32)(src)<<29) & 0x20000000)
#define CBC_MAC_KEY_LENGTHS_SET(dst,src) \
                      (((dst) & ~0x20000000) | (((u32)(src)<<29) & 0x20000000))
/*	 Fields CBC_MAC_Speed	 */
#define CBC_MAC_SPEED_WIDTH                                                   1
#define CBC_MAC_SPEED_SHIFT                                                  28
#define CBC_MAC_SPEED_MASK                                           0x10000000
#define CBC_MAC_SPEED_RD(src)                        (((src) & 0x10000000)>>28)
#define CBC_MAC_SPEED_WR(src)                   (((u32)(src)<<28) & 0x10000000)
#define CBC_MAC_SPEED_SET(dst,src) \
                      (((dst) & ~0x10000000) | (((u32)(src)<<28) & 0x10000000))
/*	 Fields XCBC_MAC	 */
#define XCBC_MAC_WIDTH                                                        1
#define XCBC_MAC_SHIFT                                                       27
#define XCBC_MAC_MASK                                                0x08000000
#define XCBC_MAC_RD(src)                             (((src) & 0x08000000)>>27)
#define XCBC_MAC_WR(src)                        (((u32)(src)<<27) & 0x08000000)
#define XCBC_MAC_SET(dst,src) \
                      (((dst) & ~0x08000000) | (((u32)(src)<<27) & 0x08000000))
/*	 Fields SHA_384_512	 */
#define SHA_384_512_WIDTH                                                     1
#define SHA_384_512_SHIFT                                                    26
#define SHA_384_512_MASK                                             0x04000000
#define SHA_384_512_RD(src)                          (((src) & 0x04000000)>>26)
#define SHA_384_512_WR(src)                     (((u32)(src)<<26) & 0x04000000)
#define SHA_384_512_SET(dst,src) \
                      (((dst) & ~0x04000000) | (((u32)(src)<<26) & 0x04000000))
/*	 Fields SHA_224_256	 */
#define SHA_224_256_WIDTH                                                     1
#define SHA_224_256_SHIFT                                                    25
#define SHA_224_256_MASK                                             0x02000000
#define SHA_224_256_RD(src)                          (((src) & 0x02000000)>>25)
#define SHA_224_256_WR(src)                     (((u32)(src)<<25) & 0x02000000)
#define SHA_224_256_SET(dst,src) \
                      (((dst) & ~0x02000000) | (((u32)(src)<<25) & 0x02000000))
/*	 Fields SHA_1_Speed	 */
#define SHA_1_SPEED_WIDTH                                                     1
#define SHA_1_SPEED_SHIFT                                                    24
#define SHA_1_SPEED_MASK                                             0x01000000
#define SHA_1_SPEED_RD(src)                          (((src) & 0x01000000)>>24)
#define SHA_1_SPEED_WR(src)                     (((u32)(src)<<24) & 0x01000000)
#define SHA_1_SPEED_SET(dst,src) \
                      (((dst) & ~0x01000000) | (((u32)(src)<<24) & 0x01000000))
/*	 Fields SHA_1	 */
#define SHA_1_WIDTH                                                           1
#define SHA_1_SHIFT                                                          23
#define SHA_1_MASK                                                   0x00800000
#define SHA_1_RD(src)                                (((src) & 0x00800000)>>23)
#define SHA_1_WR(src)                           (((u32)(src)<<23) & 0x00800000)
#define SHA_1_SET(dst,src) \
                      (((dst) & ~0x00800000) | (((u32)(src)<<23) & 0x00800000))
/*	 Fields MD5	 */
#define MD5_WIDTH                                                             1
#define MD5_SHIFT                                                            22
#define MD5_MASK                                                     0x00400000
#define MD5_RD(src)                                  (((src) & 0x00400000)>>22)
#define MD5_WR(src)                             (((u32)(src)<<22) & 0x00400000)
#define MD5_SET(dst,src) \
                      (((dst) & ~0x00400000) | (((u32)(src)<<22) & 0x00400000))
/*	 Fields Kasumi_Speed	 */
#define KASUMI_SPEED_WIDTH                                                    1
#define KASUMI_SPEED_SHIFT                                                   21
#define KASUMI_SPEED_MASK                                            0x00200000
#define KASUMI_SPEED_RD(src)                         (((src) & 0x00200000)>>21)
#define KASUMI_SPEED_WR(src)                    (((u32)(src)<<21) & 0x00200000)
#define KASUMI_SPEED_SET(dst,src) \
                      (((dst) & ~0x00200000) | (((u32)(src)<<21) & 0x00200000))
/*	 Fields Kasumi	 */
#define KASUMI_WIDTH                                                          1
#define KASUMI_SHIFT                                                         20
#define KASUMI_MASK                                                  0x00100000
#define KASUMI_RD(src)                               (((src) & 0x00100000)>>20)
#define KASUMI_WR(src)                          (((u32)(src)<<20) & 0x00100000)
#define KASUMI_SET(dst,src) \
                      (((dst) & ~0x00100000) | (((u32)(src)<<20) & 0x00100000))
/*	 Fields ARC4_Speed	 */
#define ARC4_SPEED_WIDTH                                                      1
#define ARC4_SPEED_SHIFT                                                     19
#define ARC4_SPEED_MASK                                              0x00080000
#define ARC4_SPEED_RD(src)                           (((src) & 0x00080000)>>19)
#define ARC4_SPEED_WR(src)                      (((u32)(src)<<19) & 0x00080000)
#define ARC4_SPEED_SET(dst,src) \
                      (((dst) & ~0x00080000) | (((u32)(src)<<19) & 0x00080000))
/*	 Fields ARC4	 */
#define ARC4_F1_WIDTH                                                         1
#define ARC4_F1_SHIFT                                                        18
#define ARC4_F1_MASK                                                 0x00040000
#define ARC4_F1_RD(src)                              (((src) & 0x00040000)>>18)
#define ARC4_F1_WR(src)                         (((u32)(src)<<18) & 0x00040000)
#define ARC4_F1_SET(dst,src) \
                      (((dst) & ~0x00040000) | (((u32)(src)<<18) & 0x00040000))
/*	 Fields DES_Speed	 */
#define DES_SPEED_WIDTH                                                       1
#define DES_SPEED_SHIFT                                                      17
#define DES_SPEED_MASK                                               0x00020000
#define DES_SPEED_RD(src)                            (((src) & 0x00020000)>>17)
#define DES_SPEED_WR(src)                       (((u32)(src)<<17) & 0x00020000)
#define DES_SPEED_SET(dst,src) \
                      (((dst) & ~0x00020000) | (((u32)(src)<<17) & 0x00020000))
/*	 Fields DES_FB	 */
#define DES_FB_WIDTH                                                          1
#define DES_FB_SHIFT                                                         16
#define DES_FB_MASK                                                  0x00010000
#define DES_FB_RD(src)                               (((src) & 0x00010000)>>16)
#define DES_FB_WR(src)                          (((u32)(src)<<16) & 0x00010000)
#define DES_FB_SET(dst,src) \
                      (((dst) & ~0x00010000) | (((u32)(src)<<16) & 0x00010000))
/*	 Fields DES	 */
#define DES_WIDTH                                                             1
#define DES_SHIFT                                                            15
#define DES_MASK                                                     0x00008000
#define DES_RD(src)                                  (((src) & 0x00008000)>>15)
#define DES_WR(src)                             (((u32)(src)<<15) & 0x00008000)
#define DES_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))
/*	 Fields AES_Speed	 */
#define AES_SPEED_WIDTH                                                       1
#define AES_SPEED_SHIFT                                                      14
#define AES_SPEED_MASK                                               0x00004000
#define AES_SPEED_RD(src)                            (((src) & 0x00004000)>>14)
#define AES_SPEED_WR(src)                       (((u32)(src)<<14) & 0x00004000)
#define AES_SPEED_SET(dst,src) \
                      (((dst) & ~0x00004000) | (((u32)(src)<<14) & 0x00004000))
/*	 Fields AES_FB	 */
#define AES_FB_WIDTH                                                          1
#define AES_FB_SHIFT                                                         13
#define AES_FB_MASK                                                  0x00002000
#define AES_FB_RD(src)                               (((src) & 0x00002000)>>13)
#define AES_FB_WR(src)                          (((u32)(src)<<13) & 0x00002000)
#define AES_FB_SET(dst,src) \
                      (((dst) & ~0x00002000) | (((u32)(src)<<13) & 0x00002000))
/*	 Fields AES	 */
#define AES_WIDTH                                                             1
#define AES_SHIFT                                                            12
#define AES_MASK                                                     0x00001000
#define AES_RD(src)                                  (((src) & 0x00001000)>>12)
#define AES_WR(src)                             (((u32)(src)<<12) & 0x00001000)
#define AES_SET(dst,src) \
                      (((dst) & ~0x00001000) | (((u32)(src)<<12) & 0x00001000))
/*	 Fields IPE_DEV_INFO_reserved	 */
#define IPE_DEV_INFO_RESERVED_WIDTH                                           4
#define IPE_DEV_INFO_RESERVED_SHIFT                                           8
#define IPE_DEV_INFO_RESERVED_MASK                                   0x00000f00
#define IPE_DEV_INFO_RESERVED_RD(src)                 (((src) & 0x00000f00)>>8)
#define IPE_DEV_INFO_RESERVED_WR(src)            (((u32)(src)<<8) & 0x00000f00)
#define IPE_DEV_INFO_RESERVED_SET(dst,src) \
                       (((dst) & ~0x00000f00) | (((u32)(src)<<8) & 0x00000f00))
/*	 Fields Major_revision_number	 */
#define MAJOR_REVISION_NUMBER_WIDTH                                           4
#define MAJOR_REVISION_NUMBER_SHIFT                                           4
#define MAJOR_REVISION_NUMBER_MASK                                   0x000000f0
#define MAJOR_REVISION_NUMBER_RD(src)                 (((src) & 0x000000f0)>>4)
#define MAJOR_REVISION_NUMBER_WR(src)            (((u32)(src)<<4) & 0x000000f0)
#define MAJOR_REVISION_NUMBER_SET(dst,src) \
                       (((dst) & ~0x000000f0) | (((u32)(src)<<4) & 0x000000f0))
/*	 Fields Minor_revision_number	 */
#define MINOR_REVISION_NUMBER_WIDTH                                           4
#define MINOR_REVISION_NUMBER_SHIFT                                           0
#define MINOR_REVISION_NUMBER_MASK                                   0x0000000f
#define MINOR_REVISION_NUMBER_RD(src)                    (((src) & 0x0000000f))
#define MINOR_REVISION_NUMBER_WR(src)               (((u32)(src)) & 0x0000000f)
#define MINOR_REVISION_NUMBER_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register IPE_PRNG_STAT	*/ 
/*	 Fields rsvd	 */
#define RSVD_F1_WIDTH                                                        30
#define RSVD_F1_SHIFT                                                         2
#define RSVD_F1_MASK                                                 0xfffffffc
#define RSVD_F1_RD(src)                               (((src) & 0xfffffffc)>>2)
#define RSVD_F1_WR(src)                          (((u32)(src)<<2) & 0xfffffffc)
#define RSVD_F1_SET(dst,src) \
                       (((dst) & ~0xfffffffc) | (((u32)(src)<<2) & 0xfffffffc))
/*	 Fields Result_Ready	 */
#define RESULT_READY_WIDTH                                                    1
#define RESULT_READY_SHIFT                                                    1
#define RESULT_READY_MASK                                            0x00000002
#define RESULT_READY_RD(src)                          (((src) & 0x00000002)>>1)
#define RESULT_READY_WR(src)                     (((u32)(src)<<1) & 0x00000002)
#define RESULT_READY_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields Busy	 */
#define BUSY_F1_WIDTH                                                         1
#define BUSY_F1_SHIFT                                                         0
#define BUSY_F1_MASK                                                 0x00000001
#define BUSY_F1_RD(src)                                  (((src) & 0x00000001))
#define BUSY_F1_WR(src)                             (((u32)(src)) & 0x00000001)
#define BUSY_F1_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register IPE_PRNG_CTRL	*/ 
/*	 Fields rsvd	 */
#define RSVD_F2_WIDTH                                                        29
#define RSVD_F2_SHIFT                                                         3
#define RSVD_F2_MASK                                                 0xfffffff8
#define RSVD_F2_RD(src)                               (((src) & 0xfffffff8)>>3)
#define RSVD_F2_WR(src)                          (((u32)(src)<<3) & 0xfffffff8)
#define RSVD_F2_SET(dst,src) \
                       (((dst) & ~0xfffffff8) | (((u32)(src)<<3) & 0xfffffff8))
/*	 Fields Result_128	 */
#define RESULT_128_WIDTH                                                      1
#define RESULT_128_SHIFT                                                      2
#define RESULT_128_MASK                                              0x00000004
#define RESULT_128_RD(src)                            (((src) & 0x00000004)>>2)
#define RESULT_128_WR(src)                       (((u32)(src)<<2) & 0x00000004)
#define RESULT_128_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields Auto	 */
#define AUTO_WIDTH                                                            1
#define AUTO_SHIFT                                                            1
#define AUTO_MASK                                                    0x00000002
#define AUTO_RD(src)                                  (((src) & 0x00000002)>>1)
#define AUTO_WR(src)                             (((u32)(src)<<1) & 0x00000002)
#define AUTO_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields Enable	 */
#define SEC_ENABLE_F8_WIDTH                                                   1
#define SEC_ENABLE_F8_SHIFT                                                   0
#define SEC_ENABLE_F8_MASK                                           0x00000001
#define SEC_ENABLE_F8_RD(src)                            (((src) & 0x00000001))
#define SEC_ENABLE_F8_WR(src)                       (((u32)(src)) & 0x00000001)
#define SEC_ENABLE_F8_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register IPE_PRNG_SEED_L	*/ 
/*	 Fields PRNG_Seed_Low	 */
#define PRNG_SEED_LOW_WIDTH                                                  32
#define PRNG_SEED_LOW_SHIFT                                                   0
#define PRNG_SEED_LOW_MASK                                           0xffffffff
#define PRNG_SEED_LOW_RD(src)                            (((src) & 0xffffffff))
#define PRNG_SEED_LOW_WR(src)                       (((u32)(src)) & 0xffffffff)
#define PRNG_SEED_LOW_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_PRNG_SEED_H	*/ 
/*	 Fields PRNG_Seed_High	 */
#define PRNG_SEED_HIGH_WIDTH                                                 32
#define PRNG_SEED_HIGH_SHIFT                                                  0
#define PRNG_SEED_HIGH_MASK                                          0xffffffff
#define PRNG_SEED_HIGH_RD(src)                           (((src) & 0xffffffff))
#define PRNG_SEED_HIGH_WR(src)                      (((u32)(src)) & 0xffffffff)
#define PRNG_SEED_HIGH_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_PRNG_KEY_0_L	*/ 
/*	 Fields PRNG_Key0_Low	 */
#define PRNG_KEY0_LOW_WIDTH                                                  32
#define PRNG_KEY0_LOW_SHIFT                                                   0
#define PRNG_KEY0_LOW_MASK                                           0xffffffff
#define PRNG_KEY0_LOW_RD(src)                            (((src) & 0xffffffff))
#define PRNG_KEY0_LOW_WR(src)                       (((u32)(src)) & 0xffffffff)
#define PRNG_KEY0_LOW_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_PRNG_KEY_0_H	*/ 
/*	 Fields PRNG_Key0_High	 */
#define PRNG_KEY0_HIGH_WIDTH                                                 32
#define PRNG_KEY0_HIGH_SHIFT                                                  0
#define PRNG_KEY0_HIGH_MASK                                          0xffffffff
#define PRNG_KEY0_HIGH_RD(src)                           (((src) & 0xffffffff))
#define PRNG_KEY0_HIGH_WR(src)                      (((u32)(src)) & 0xffffffff)
#define PRNG_KEY0_HIGH_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_PRNG_KEY_1_L	*/ 
/*	 Fields PRNG_Key1_Low	 */
#define PRNG_KEY1_LOW_WIDTH                                                  32
#define PRNG_KEY1_LOW_SHIFT                                                   0
#define PRNG_KEY1_LOW_MASK                                           0xffffffff
#define PRNG_KEY1_LOW_RD(src)                            (((src) & 0xffffffff))
#define PRNG_KEY1_LOW_WR(src)                       (((u32)(src)) & 0xffffffff)
#define PRNG_KEY1_LOW_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_PRNG_KEY_1_H	*/ 
/*	 Fields PRNG_Key1_High	 */
#define PRNG_KEY1_HIGH_WIDTH                                                 32
#define PRNG_KEY1_HIGH_SHIFT                                                  0
#define PRNG_KEY1_HIGH_MASK                                          0xffffffff
#define PRNG_KEY1_HIGH_RD(src)                           (((src) & 0xffffffff))
#define PRNG_KEY1_HIGH_WR(src)                      (((u32)(src)) & 0xffffffff)
#define PRNG_KEY1_HIGH_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_PRNG_RES0_L	*/ 
/*	 Fields PRNG_Res0_Low	 */
#define PRNG_RES0_LOW_WIDTH                                                  32
#define PRNG_RES0_LOW_SHIFT                                                   0
#define PRNG_RES0_LOW_MASK                                           0xffffffff
#define PRNG_RES0_LOW_RD(src)                            (((src) & 0xffffffff))
#define PRNG_RES0_LOW_WR(src)                       (((u32)(src)) & 0xffffffff)
#define PRNG_RES0_LOW_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_PRNG_RES0_H	*/ 
/*	 Fields PRNG_Res0_High	 */
#define PRNG_RES0_HIGH_WIDTH                                                 32
#define PRNG_RES0_HIGH_SHIFT                                                  0
#define PRNG_RES0_HIGH_MASK                                          0xffffffff
#define PRNG_RES0_HIGH_RD(src)                           (((src) & 0xffffffff))
#define PRNG_RES0_HIGH_WR(src)                      (((u32)(src)) & 0xffffffff)
#define PRNG_RES0_HIGH_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_PRNG_RES1_L	*/ 
/*	 Fields PRNG_Res1_Low	 */
#define PRNG_RES1_LOW_WIDTH                                                  32
#define PRNG_RES1_LOW_SHIFT                                                   0
#define PRNG_RES1_LOW_MASK                                           0xffffffff
#define PRNG_RES1_LOW_RD(src)                            (((src) & 0xffffffff))
#define PRNG_RES1_LOW_WR(src)                       (((u32)(src)) & 0xffffffff)
#define PRNG_RES1_LOW_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_PRNG_RES1_H	*/ 
/*	 Fields PRNG_Res1_High	 */
#define PRNG_RES1_HIGH_WIDTH                                                 32
#define PRNG_RES1_HIGH_SHIFT                                                  0
#define PRNG_RES1_HIGH_MASK                                          0xffffffff
#define PRNG_RES1_HIGH_RD(src)                           (((src) & 0xffffffff))
#define PRNG_RES1_HIGH_WR(src)                      (((u32)(src)) & 0xffffffff)
#define PRNG_RES1_HIGH_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_PRNG_LFSR_L	*/ 
/*	 Fields PRNG_LFSR_Low	 */
#define PRNG_LFSR_LOW_WIDTH                                                  32
#define PRNG_LFSR_LOW_SHIFT                                                   0
#define PRNG_LFSR_LOW_MASK                                           0xffffffff
#define PRNG_LFSR_LOW_RD(src)                            (((src) & 0xffffffff))
#define PRNG_LFSR_LOW_WR(src)                       (((u32)(src)) & 0xffffffff)
#define PRNG_LFSR_LOW_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_PRNG_LFSR_H	*/ 
/*	 Fields PRNG_LFSR_High	 */
#define PRNG_LFSR_HIGH_WIDTH                                                 32
#define PRNG_LFSR_HIGH_SHIFT                                                  0
#define PRNG_LFSR_HIGH_MASK                                          0xffffffff
#define PRNG_LFSR_HIGH_RD(src)                           (((src) & 0xffffffff))
#define PRNG_LFSR_HIGH_WR(src)                      (((u32)(src)) & 0xffffffff)
#define PRNG_LFSR_HIGH_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_ACT_TKN_W0	*/ 
/*	 Fields IPE_ACT_TKN_W0	 */
#define IPE_ACT_TKN_W00_WIDTH                                                32
#define IPE_ACT_TKN_W00_SHIFT                                                 0
#define IPE_ACT_TKN_W00_MASK                                         0xffffffff
#define IPE_ACT_TKN_W00_RD(src)                          (((src) & 0xffffffff))
#define IPE_ACT_TKN_W00_WR(src)                     (((u32)(src)) & 0xffffffff)
#define IPE_ACT_TKN_W00_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_ACT_TKN_W1	*/ 
/*	 Fields IPE_ACT_TKN_W1	 */
#define IPE_ACT_TKN_W11_WIDTH                                                32
#define IPE_ACT_TKN_W11_SHIFT                                                 0
#define IPE_ACT_TKN_W11_MASK                                         0xffffffff
#define IPE_ACT_TKN_W11_RD(src)                          (((src) & 0xffffffff))
#define IPE_ACT_TKN_W11_WR(src)                     (((u32)(src)) & 0xffffffff)
#define IPE_ACT_TKN_W11_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_ACT_TKN_W2	*/ 
/*	 Fields IPE_ACT_TKN_W2	 */
#define IPE_ACT_TKN_W22_WIDTH                                                32
#define IPE_ACT_TKN_W22_SHIFT                                                 0
#define IPE_ACT_TKN_W22_MASK                                         0xffffffff
#define IPE_ACT_TKN_W22_RD(src)                          (((src) & 0xffffffff))
#define IPE_ACT_TKN_W22_WR(src)                     (((u32)(src)) & 0xffffffff)
#define IPE_ACT_TKN_W22_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_ACT_TKN_W3	*/ 
/*	 Fields IPE_ACT_TKN_W3	 */
#define IPE_ACT_TKN_W33_WIDTH                                                32
#define IPE_ACT_TKN_W33_SHIFT                                                 0
#define IPE_ACT_TKN_W33_MASK                                         0xffffffff
#define IPE_ACT_TKN_W33_RD(src)                          (((src) & 0xffffffff))
#define IPE_ACT_TKN_W33_WR(src)                     (((u32)(src)) & 0xffffffff)
#define IPE_ACT_TKN_W33_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_ACT_TKN_W4	*/ 
/*	 Fields IPE_ACT_TKN_W4	 */
#define IPE_ACT_TKN_W44_WIDTH                                                32
#define IPE_ACT_TKN_W44_SHIFT                                                 0
#define IPE_ACT_TKN_W44_MASK                                         0xffffffff
#define IPE_ACT_TKN_W44_RD(src)                          (((src) & 0xffffffff))
#define IPE_ACT_TKN_W44_WR(src)                     (((u32)(src)) & 0xffffffff)
#define IPE_ACT_TKN_W44_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_ACT_TKN_W5	*/ 
/*	 Fields IPE_ACT_TKN_W5	 */
#define IPE_ACT_TKN_W55_WIDTH                                                32
#define IPE_ACT_TKN_W55_SHIFT                                                 0
#define IPE_ACT_TKN_W55_MASK                                         0xffffffff
#define IPE_ACT_TKN_W55_RD(src)                          (((src) & 0xffffffff))
#define IPE_ACT_TKN_W55_WR(src)                     (((u32)(src)) & 0xffffffff)
#define IPE_ACT_TKN_W55_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_ACT_TKN_W6	*/ 
/*	 Fields IPE_ACT_TKN_W6	 */
#define IPE_ACT_TKN_W66_WIDTH                                                32
#define IPE_ACT_TKN_W66_SHIFT                                                 0
#define IPE_ACT_TKN_W66_MASK                                         0xffffffff
#define IPE_ACT_TKN_W66_RD(src)                          (((src) & 0xffffffff))
#define IPE_ACT_TKN_W66_WR(src)                     (((u32)(src)) & 0xffffffff)
#define IPE_ACT_TKN_W66_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_ACT_TKN_W7	*/ 
/*	 Fields IPE_ACT_TKN_W7	 */
#define IPE_ACT_TKN_W77_WIDTH                                                32
#define IPE_ACT_TKN_W77_SHIFT                                                 0
#define IPE_ACT_TKN_W77_MASK                                         0xffffffff
#define IPE_ACT_TKN_W77_RD(src)                          (((src) & 0xffffffff))
#define IPE_ACT_TKN_W77_WR(src)                     (((u32)(src)) & 0xffffffff)
#define IPE_ACT_TKN_W77_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_ACT_TKN_W8	*/ 
/*	 Fields IPE_ACT_TKN_W8	 */
#define IPE_ACT_TKN_W88_WIDTH                                                32
#define IPE_ACT_TKN_W88_SHIFT                                                 0
#define IPE_ACT_TKN_W88_MASK                                         0xffffffff
#define IPE_ACT_TKN_W88_RD(src)                          (((src) & 0xffffffff))
#define IPE_ACT_TKN_W88_WR(src)                     (((u32)(src)) & 0xffffffff)
#define IPE_ACT_TKN_W88_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_ACT_TKN_W9	*/ 
/*	 Fields IPE_ACT_TKN_W9	 */
#define IPE_ACT_TKN_W99_WIDTH                                                32
#define IPE_ACT_TKN_W99_SHIFT                                                 0
#define IPE_ACT_TKN_W99_MASK                                         0xffffffff
#define IPE_ACT_TKN_W99_RD(src)                          (((src) & 0xffffffff))
#define IPE_ACT_TKN_W99_WR(src)                     (((u32)(src)) & 0xffffffff)
#define IPE_ACT_TKN_W99_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_ACT_TKN_W10	*/ 
/*	 Fields IPE_ACT_TKN_W10	 */
#define IPE_ACT_TKN_W100_WIDTH                                               32
#define IPE_ACT_TKN_W100_SHIFT                                                0
#define IPE_ACT_TKN_W100_MASK                                        0xffffffff
#define IPE_ACT_TKN_W100_RD(src)                         (((src) & 0xffffffff))
#define IPE_ACT_TKN_W100_WR(src)                    (((u32)(src)) & 0xffffffff)
#define IPE_ACT_TKN_W100_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_ACT_TKN_W11	*/ 
/*	 Fields IPE_ACT_TKN_W11	 */
#define IPE_ACT_TKN_W111_WIDTH                                               32
#define IPE_ACT_TKN_W111_SHIFT                                                0
#define IPE_ACT_TKN_W111_MASK                                        0xffffffff
#define IPE_ACT_TKN_W111_RD(src)                         (((src) & 0xffffffff))
#define IPE_ACT_TKN_W111_WR(src)                    (((u32)(src)) & 0xffffffff)
#define IPE_ACT_TKN_W111_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_ACT_TKN_W12	*/ 
/*	 Fields IPE_ACT_TKN_W12	 */
#define IPE_ACT_TKN_W122_WIDTH                                               32
#define IPE_ACT_TKN_W122_SHIFT                                                0
#define IPE_ACT_TKN_W122_MASK                                        0xffffffff
#define IPE_ACT_TKN_W122_RD(src)                         (((src) & 0xffffffff))
#define IPE_ACT_TKN_W122_WR(src)                    (((u32)(src)) & 0xffffffff)
#define IPE_ACT_TKN_W122_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_ACT_TKN_W13	*/ 
/*	 Fields IPE_ACT_TKN_W13	 */
#define IPE_ACT_TKN_W133_WIDTH                                               32
#define IPE_ACT_TKN_W133_SHIFT                                                0
#define IPE_ACT_TKN_W133_MASK                                        0xffffffff
#define IPE_ACT_TKN_W133_RD(src)                         (((src) & 0xffffffff))
#define IPE_ACT_TKN_W133_WR(src)                    (((u32)(src)) & 0xffffffff)
#define IPE_ACT_TKN_W133_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_ACT_TKN_W14	*/ 
/*	 Fields IPE_ACT_TKN_W14	 */
#define IPE_ACT_TKN_W144_WIDTH                                               32
#define IPE_ACT_TKN_W144_SHIFT                                                0
#define IPE_ACT_TKN_W144_MASK                                        0xffffffff
#define IPE_ACT_TKN_W144_RD(src)                         (((src) & 0xffffffff))
#define IPE_ACT_TKN_W144_WR(src)                    (((u32)(src)) & 0xffffffff)
#define IPE_ACT_TKN_W144_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_ACT_TKN_W15	*/ 
/*	 Fields IPE_ACT_TKN_W15	 */
#define IPE_ACT_TKN_W155_WIDTH                                               32
#define IPE_ACT_TKN_W155_SHIFT                                                0
#define IPE_ACT_TKN_W155_MASK                                        0xffffffff
#define IPE_ACT_TKN_W155_RD(src)                         (((src) & 0xffffffff))
#define IPE_ACT_TKN_W155_WR(src)                    (((u32)(src)) & 0xffffffff)
#define IPE_ACT_TKN_W155_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_TKN_W0_ACT_W16	*/ 
/*	 Fields IPE_NXT_TKN_W0_ACT_W16	 */
#define IPE_NXT_TKN_W0_ACT_W166_WIDTH                                        32
#define IPE_NXT_TKN_W0_ACT_W166_SHIFT                                         0
#define IPE_NXT_TKN_W0_ACT_W166_MASK                                 0xffffffff
#define IPE_NXT_TKN_W0_ACT_W166_RD(src)                  (((src) & 0xffffffff))
#define IPE_NXT_TKN_W0_ACT_W166_WR(src)             (((u32)(src)) & 0xffffffff)
#define IPE_NXT_TKN_W0_ACT_W166_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_TKN_W1_ACT_W17	*/ 
/*	 Fields IPE_NXT_TKN_W1_ACT_W17	 */
#define IPE_NXT_TKN_W1_ACT_W177_WIDTH                                        32
#define IPE_NXT_TKN_W1_ACT_W177_SHIFT                                         0
#define IPE_NXT_TKN_W1_ACT_W177_MASK                                 0xffffffff
#define IPE_NXT_TKN_W1_ACT_W177_RD(src)                  (((src) & 0xffffffff))
#define IPE_NXT_TKN_W1_ACT_W177_WR(src)             (((u32)(src)) & 0xffffffff)
#define IPE_NXT_TKN_W1_ACT_W177_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_TKN_W2_ACT_W18	*/ 
/*	 Fields IPE_NXT_TKN_W2_ACT_W18	 */
#define IPE_NXT_TKN_W2_ACT_W188_WIDTH                                        32
#define IPE_NXT_TKN_W2_ACT_W188_SHIFT                                         0
#define IPE_NXT_TKN_W2_ACT_W188_MASK                                 0xffffffff
#define IPE_NXT_TKN_W2_ACT_W188_RD(src)                  (((src) & 0xffffffff))
#define IPE_NXT_TKN_W2_ACT_W188_WR(src)             (((u32)(src)) & 0xffffffff)
#define IPE_NXT_TKN_W2_ACT_W188_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_TKN_W3_ACT_W19	*/ 
/*	 Fields IPE_NXT_TKN_W3_ACT_W19	 */
#define IPE_NXT_TKN_W3_ACT_W199_WIDTH                                        32
#define IPE_NXT_TKN_W3_ACT_W199_SHIFT                                         0
#define IPE_NXT_TKN_W3_ACT_W199_MASK                                 0xffffffff
#define IPE_NXT_TKN_W3_ACT_W199_RD(src)                  (((src) & 0xffffffff))
#define IPE_NXT_TKN_W3_ACT_W199_WR(src)             (((u32)(src)) & 0xffffffff)
#define IPE_NXT_TKN_W3_ACT_W199_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_TKN_W4_ACT_W20	*/ 
/*	 Fields IPE_NXT_TKN_W4_ACT_W20	 */
#define IPE_NXT_TKN_W4_ACT_W200_WIDTH                                        32
#define IPE_NXT_TKN_W4_ACT_W200_SHIFT                                         0
#define IPE_NXT_TKN_W4_ACT_W200_MASK                                 0xffffffff
#define IPE_NXT_TKN_W4_ACT_W200_RD(src)                  (((src) & 0xffffffff))
#define IPE_NXT_TKN_W4_ACT_W200_WR(src)             (((u32)(src)) & 0xffffffff)
#define IPE_NXT_TKN_W4_ACT_W200_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_TKN_W5_ACT_W21	*/ 
/*	 Fields IPE_NXT_TKN_W5_ACT_W21	 */
#define IPE_NXT_TKN_W5_ACT_W211_WIDTH                                        32
#define IPE_NXT_TKN_W5_ACT_W211_SHIFT                                         0
#define IPE_NXT_TKN_W5_ACT_W211_MASK                                 0xffffffff
#define IPE_NXT_TKN_W5_ACT_W211_RD(src)                  (((src) & 0xffffffff))
#define IPE_NXT_TKN_W5_ACT_W211_WR(src)             (((u32)(src)) & 0xffffffff)
#define IPE_NXT_TKN_W5_ACT_W211_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_TKN_W6_ACT_W22	*/ 
/*	 Fields IPE_NXT_TKN_W6_ACT_W22	 */
#define IPE_NXT_TKN_W6_ACT_W222_WIDTH                                        32
#define IPE_NXT_TKN_W6_ACT_W222_SHIFT                                         0
#define IPE_NXT_TKN_W6_ACT_W222_MASK                                 0xffffffff
#define IPE_NXT_TKN_W6_ACT_W222_RD(src)                  (((src) & 0xffffffff))
#define IPE_NXT_TKN_W6_ACT_W222_WR(src)             (((u32)(src)) & 0xffffffff)
#define IPE_NXT_TKN_W6_ACT_W222_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_TKN_W7_ACT_W23	*/ 
/*	 Fields IPE_NXT_TKN_W7_ACT_W23	 */
#define IPE_NXT_TKN_W7_ACT_W233_WIDTH                                        32
#define IPE_NXT_TKN_W7_ACT_W233_SHIFT                                         0
#define IPE_NXT_TKN_W7_ACT_W233_MASK                                 0xffffffff
#define IPE_NXT_TKN_W7_ACT_W233_RD(src)                  (((src) & 0xffffffff))
#define IPE_NXT_TKN_W7_ACT_W233_WR(src)             (((u32)(src)) & 0xffffffff)
#define IPE_NXT_TKN_W7_ACT_W233_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_TKN_W8_ACT_W24	*/ 
/*	 Fields IPE_NXT_TKN_W8_ACT_W24	 */
#define IPE_NXT_TKN_W8_ACT_W244_WIDTH                                        32
#define IPE_NXT_TKN_W8_ACT_W244_SHIFT                                         0
#define IPE_NXT_TKN_W8_ACT_W244_MASK                                 0xffffffff
#define IPE_NXT_TKN_W8_ACT_W244_RD(src)                  (((src) & 0xffffffff))
#define IPE_NXT_TKN_W8_ACT_W244_WR(src)             (((u32)(src)) & 0xffffffff)
#define IPE_NXT_TKN_W8_ACT_W244_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_TKN_W9_ACT_W25	*/ 
/*	 Fields IPE_NXT_TKN_W9_ACT_W25	 */
#define IPE_NXT_TKN_W9_ACT_W255_WIDTH                                        32
#define IPE_NXT_TKN_W9_ACT_W255_SHIFT                                         0
#define IPE_NXT_TKN_W9_ACT_W255_MASK                                 0xffffffff
#define IPE_NXT_TKN_W9_ACT_W255_RD(src)                  (((src) & 0xffffffff))
#define IPE_NXT_TKN_W9_ACT_W255_WR(src)             (((u32)(src)) & 0xffffffff)
#define IPE_NXT_TKN_W9_ACT_W255_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_TKN_W10_ACT_W26	*/ 
/*	 Fields IPE_NXT_TKN_W10_ACT_W26	 */
#define IPE_NXT_TKN_W10_ACT_W266_WIDTH                                       32
#define IPE_NXT_TKN_W10_ACT_W266_SHIFT                                        0
#define IPE_NXT_TKN_W10_ACT_W266_MASK                                0xffffffff
#define IPE_NXT_TKN_W10_ACT_W266_RD(src)                 (((src) & 0xffffffff))
#define IPE_NXT_TKN_W10_ACT_W266_WR(src)            (((u32)(src)) & 0xffffffff)
#define IPE_NXT_TKN_W10_ACT_W266_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_TKN_W11_ACT_W27	*/ 
/*	 Fields IPE_NXT_TKN_W11_ACT_W27	 */
#define IPE_NXT_TKN_W11_ACT_W277_WIDTH                                       32
#define IPE_NXT_TKN_W11_ACT_W277_SHIFT                                        0
#define IPE_NXT_TKN_W11_ACT_W277_MASK                                0xffffffff
#define IPE_NXT_TKN_W11_ACT_W277_RD(src)                 (((src) & 0xffffffff))
#define IPE_NXT_TKN_W11_ACT_W277_WR(src)            (((u32)(src)) & 0xffffffff)
#define IPE_NXT_TKN_W11_ACT_W277_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_TKN_W12_ACT_W28	*/ 
/*	 Fields IPE_NXT_TKN_W12_ACT_W28	 */
#define IPE_NXT_TKN_W12_ACT_W288_WIDTH                                       32
#define IPE_NXT_TKN_W12_ACT_W288_SHIFT                                        0
#define IPE_NXT_TKN_W12_ACT_W288_MASK                                0xffffffff
#define IPE_NXT_TKN_W12_ACT_W288_RD(src)                 (((src) & 0xffffffff))
#define IPE_NXT_TKN_W12_ACT_W288_WR(src)            (((u32)(src)) & 0xffffffff)
#define IPE_NXT_TKN_W12_ACT_W288_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_TKN_W13_ACT_W29	*/ 
/*	 Fields IPE_NXT_TKN_W13_ACT_W29	 */
#define IPE_NXT_TKN_W13_ACT_W299_WIDTH                                       32
#define IPE_NXT_TKN_W13_ACT_W299_SHIFT                                        0
#define IPE_NXT_TKN_W13_ACT_W299_MASK                                0xffffffff
#define IPE_NXT_TKN_W13_ACT_W299_RD(src)                 (((src) & 0xffffffff))
#define IPE_NXT_TKN_W13_ACT_W299_WR(src)            (((u32)(src)) & 0xffffffff)
#define IPE_NXT_TKN_W13_ACT_W299_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_TKN_W14_ACT_W30	*/ 
/*	 Fields IPE_NXT_TKN_W14_ACT_W30	 */
#define IPE_NXT_TKN_W14_ACT_W300_WIDTH                                       32
#define IPE_NXT_TKN_W14_ACT_W300_SHIFT                                        0
#define IPE_NXT_TKN_W14_ACT_W300_MASK                                0xffffffff
#define IPE_NXT_TKN_W14_ACT_W300_RD(src)                 (((src) & 0xffffffff))
#define IPE_NXT_TKN_W14_ACT_W300_WR(src)            (((u32)(src)) & 0xffffffff)
#define IPE_NXT_TKN_W14_ACT_W300_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_TKN_W15_ACT_W31	*/ 
/*	 Fields IPE_NXT_TKN_W15_ACT_W31	 */
#define IPE_NXT_TKN_W15_ACT_W311_WIDTH                                       32
#define IPE_NXT_TKN_W15_ACT_W311_SHIFT                                        0
#define IPE_NXT_TKN_W15_ACT_W311_MASK                                0xffffffff
#define IPE_NXT_TKN_W15_ACT_W311_RD(src)                 (((src) & 0xffffffff))
#define IPE_NXT_TKN_W15_ACT_W311_WR(src)            (((u32)(src)) & 0xffffffff)
#define IPE_NXT_TKN_W15_ACT_W311_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_RES_TKN_W0	*/ 
/*	 Fields IPE_RES_TKN_W0	 */
#define IPE_RES_TKN_W00_WIDTH                                                32
#define IPE_RES_TKN_W00_SHIFT                                                 0
#define IPE_RES_TKN_W00_MASK                                         0xffffffff
#define IPE_RES_TKN_W00_RD(src)                          (((src) & 0xffffffff))
#define IPE_RES_TKN_W00_WR(src)                     (((u32)(src)) & 0xffffffff)
#define IPE_RES_TKN_W00_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_RES_TKN_W1	*/ 
/*	 Fields IPE_RES_TKN_W1	 */
#define IPE_RES_TKN_W11_WIDTH                                                32
#define IPE_RES_TKN_W11_SHIFT                                                 0
#define IPE_RES_TKN_W11_MASK                                         0xffffffff
#define IPE_RES_TKN_W11_RD(src)                          (((src) & 0xffffffff))
#define IPE_RES_TKN_W11_WR(src)                     (((u32)(src)) & 0xffffffff)
#define IPE_RES_TKN_W11_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_RES_TKN_W2	*/ 
/*	 Fields IPE_RES_TKN_W2	 */
#define IPE_RES_TKN_W22_WIDTH                                                32
#define IPE_RES_TKN_W22_SHIFT                                                 0
#define IPE_RES_TKN_W22_MASK                                         0xffffffff
#define IPE_RES_TKN_W22_RD(src)                          (((src) & 0xffffffff))
#define IPE_RES_TKN_W22_WR(src)                     (((u32)(src)) & 0xffffffff)
#define IPE_RES_TKN_W22_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_RES_TKN_W3	*/ 
/*	 Fields IPE_RES_TKN_W3	 */
#define IPE_RES_TKN_W33_WIDTH                                                32
#define IPE_RES_TKN_W33_SHIFT                                                 0
#define IPE_RES_TKN_W33_MASK                                         0xffffffff
#define IPE_RES_TKN_W33_RD(src)                          (((src) & 0xffffffff))
#define IPE_RES_TKN_W33_WR(src)                     (((u32)(src)) & 0xffffffff)
#define IPE_RES_TKN_W33_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_RES_TKN_W4	*/ 
/*	 Fields IPE_RES_TKN_W4	 */
#define IPE_RES_TKN_W44_WIDTH                                                32
#define IPE_RES_TKN_W44_SHIFT                                                 0
#define IPE_RES_TKN_W44_MASK                                         0xffffffff
#define IPE_RES_TKN_W44_RD(src)                          (((src) & 0xffffffff))
#define IPE_RES_TKN_W44_WR(src)                     (((u32)(src)) & 0xffffffff)
#define IPE_RES_TKN_W44_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_RES_TKN_W5	*/ 
/*	 Fields IPE_RES_TKN_W5	 */
#define IPE_RES_TKN_W55_WIDTH                                                32
#define IPE_RES_TKN_W55_SHIFT                                                 0
#define IPE_RES_TKN_W55_MASK                                         0xffffffff
#define IPE_RES_TKN_W55_RD(src)                          (((src) & 0xffffffff))
#define IPE_RES_TKN_W55_WR(src)                     (((u32)(src)) & 0xffffffff)
#define IPE_RES_TKN_W55_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_RES_TKN_W6	*/ 
/*	 Fields IPE_RES_TKN_W6	 */
#define IPE_RES_TKN_W66_WIDTH                                                32
#define IPE_RES_TKN_W66_SHIFT                                                 0
#define IPE_RES_TKN_W66_MASK                                         0xffffffff
#define IPE_RES_TKN_W66_RD(src)                          (((src) & 0xffffffff))
#define IPE_RES_TKN_W66_WR(src)                     (((u32)(src)) & 0xffffffff)
#define IPE_RES_TKN_W66_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_RES_TKN_W7	*/ 
/*	 Fields IPE_RES_TKN_W7	 */
#define IPE_RES_TKN_W77_WIDTH                                                32
#define IPE_RES_TKN_W77_SHIFT                                                 0
#define IPE_RES_TKN_W77_MASK                                         0xffffffff
#define IPE_RES_TKN_W77_RD(src)                          (((src) & 0xffffffff))
#define IPE_RES_TKN_W77_WR(src)                     (((u32)(src)) & 0xffffffff)
#define IPE_RES_TKN_W77_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_CTX_CMD0	*/ 
/*	 Fields IPE_NXT_CTX_CMD0	 */
#define IPE_NXT_CTX_CMD00_WIDTH                                              32
#define IPE_NXT_CTX_CMD00_SHIFT                                               0
#define IPE_NXT_CTX_CMD00_MASK                                       0xffffffff
#define IPE_NXT_CTX_CMD00_RD(src)                        (((src) & 0xffffffff))
#define IPE_NXT_CTX_CMD00_WR(src)                   (((u32)(src)) & 0xffffffff)
#define IPE_NXT_CTX_CMD00_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_CTX_CMD1	*/ 
/*	 Fields IPE_NXT_CTX_CMD1	 */
#define IPE_NXT_CTX_CMD11_WIDTH                                              32
#define IPE_NXT_CTX_CMD11_SHIFT                                               0
#define IPE_NXT_CTX_CMD11_MASK                                       0xffffffff
#define IPE_NXT_CTX_CMD11_RD(src)                        (((src) & 0xffffffff))
#define IPE_NXT_CTX_CMD11_WR(src)                   (((u32)(src)) & 0xffffffff)
#define IPE_NXT_CTX_CMD11_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_GPR0	*/ 
/*	 Fields IPE_NXT_GPR0	 */
#define IPE_NXT_GPR00_WIDTH                                                  32
#define IPE_NXT_GPR00_SHIFT                                                   0
#define IPE_NXT_GPR00_MASK                                           0xffffffff
#define IPE_NXT_GPR00_RD(src)                            (((src) & 0xffffffff))
#define IPE_NXT_GPR00_WR(src)                       (((u32)(src)) & 0xffffffff)
#define IPE_NXT_GPR00_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_GPR1	*/ 
/*	 Fields IPE_NXT_GPR1	 */
#define IPE_NXT_GPR11_WIDTH                                                  32
#define IPE_NXT_GPR11_SHIFT                                                   0
#define IPE_NXT_GPR11_MASK                                           0xffffffff
#define IPE_NXT_GPR11_RD(src)                            (((src) & 0xffffffff))
#define IPE_NXT_GPR11_WR(src)                       (((u32)(src)) & 0xffffffff)
#define IPE_NXT_GPR11_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_IV0	*/ 
/*	 Fields IPE_NXT_IV0	 */
#define IPE_NXT_IV00_WIDTH                                                   32
#define IPE_NXT_IV00_SHIFT                                                    0
#define IPE_NXT_IV00_MASK                                            0xffffffff
#define IPE_NXT_IV00_RD(src)                             (((src) & 0xffffffff))
#define IPE_NXT_IV00_WR(src)                        (((u32)(src)) & 0xffffffff)
#define IPE_NXT_IV00_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_IV1	*/ 
/*	 Fields IPE_NXT_IV1	 */
#define IPE_NXT_IV11_WIDTH                                                   32
#define IPE_NXT_IV11_SHIFT                                                    0
#define IPE_NXT_IV11_MASK                                            0xffffffff
#define IPE_NXT_IV11_RD(src)                             (((src) & 0xffffffff))
#define IPE_NXT_IV11_WR(src)                        (((u32)(src)) & 0xffffffff)
#define IPE_NXT_IV11_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_IV2	*/ 
/*	 Fields IPE_NXT_IV2	 */
#define IPE_NXT_IV22_WIDTH                                                   32
#define IPE_NXT_IV22_SHIFT                                                    0
#define IPE_NXT_IV22_MASK                                            0xffffffff
#define IPE_NXT_IV22_RD(src)                             (((src) & 0xffffffff))
#define IPE_NXT_IV22_WR(src)                        (((u32)(src)) & 0xffffffff)
#define IPE_NXT_IV22_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_IV3	*/ 
/*	 Fields IPE_NXT_IV3	 */
#define IPE_NXT_IV33_WIDTH                                                   32
#define IPE_NXT_IV33_SHIFT                                                    0
#define IPE_NXT_IV33_MASK                                            0xffffffff
#define IPE_NXT_IV33_RD(src)                             (((src) & 0xffffffff))
#define IPE_NXT_IV33_WR(src)                        (((u32)(src)) & 0xffffffff)
#define IPE_NXT_IV33_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_KEY0	*/ 
/*	 Fields IPE_NXT_KEY0	 */
#define IPE_NXT_KEY00_WIDTH                                                  32
#define IPE_NXT_KEY00_SHIFT                                                   0
#define IPE_NXT_KEY00_MASK                                           0xffffffff
#define IPE_NXT_KEY00_RD(src)                            (((src) & 0xffffffff))
#define IPE_NXT_KEY00_WR(src)                       (((u32)(src)) & 0xffffffff)
#define IPE_NXT_KEY00_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_KEY1	*/ 
/*	 Fields IPE_NXT_KEY1	 */
#define IPE_NXT_KEY11_WIDTH                                                  32
#define IPE_NXT_KEY11_SHIFT                                                   0
#define IPE_NXT_KEY11_MASK                                           0xffffffff
#define IPE_NXT_KEY11_RD(src)                            (((src) & 0xffffffff))
#define IPE_NXT_KEY11_WR(src)                       (((u32)(src)) & 0xffffffff)
#define IPE_NXT_KEY11_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_KEY2	*/ 
/*	 Fields IPE_NXT_KEY2	 */
#define IPE_NXT_KEY22_WIDTH                                                  32
#define IPE_NXT_KEY22_SHIFT                                                   0
#define IPE_NXT_KEY22_MASK                                           0xffffffff
#define IPE_NXT_KEY22_RD(src)                            (((src) & 0xffffffff))
#define IPE_NXT_KEY22_WR(src)                       (((u32)(src)) & 0xffffffff)
#define IPE_NXT_KEY22_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_KEY3	*/ 
/*	 Fields IPE_NXT_KEY3	 */
#define IPE_NXT_KEY33_WIDTH                                                  32
#define IPE_NXT_KEY33_SHIFT                                                   0
#define IPE_NXT_KEY33_MASK                                           0xffffffff
#define IPE_NXT_KEY33_RD(src)                            (((src) & 0xffffffff))
#define IPE_NXT_KEY33_WR(src)                       (((u32)(src)) & 0xffffffff)
#define IPE_NXT_KEY33_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_KEY4	*/ 
/*	 Fields IPE_NXT_KEY4	 */
#define IPE_NXT_KEY44_WIDTH                                                  32
#define IPE_NXT_KEY44_SHIFT                                                   0
#define IPE_NXT_KEY44_MASK                                           0xffffffff
#define IPE_NXT_KEY44_RD(src)                            (((src) & 0xffffffff))
#define IPE_NXT_KEY44_WR(src)                       (((u32)(src)) & 0xffffffff)
#define IPE_NXT_KEY44_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_KEY5	*/ 
/*	 Fields IPE_NXT_KEY5	 */
#define IPE_NXT_KEY55_WIDTH                                                  32
#define IPE_NXT_KEY55_SHIFT                                                   0
#define IPE_NXT_KEY55_MASK                                           0xffffffff
#define IPE_NXT_KEY55_RD(src)                            (((src) & 0xffffffff))
#define IPE_NXT_KEY55_WR(src)                       (((u32)(src)) & 0xffffffff)
#define IPE_NXT_KEY55_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_KEY6	*/ 
/*	 Fields IPE_NXT_KEY6	 */
#define IPE_NXT_KEY66_WIDTH                                                  32
#define IPE_NXT_KEY66_SHIFT                                                   0
#define IPE_NXT_KEY66_MASK                                           0xffffffff
#define IPE_NXT_KEY66_RD(src)                            (((src) & 0xffffffff))
#define IPE_NXT_KEY66_WR(src)                       (((u32)(src)) & 0xffffffff)
#define IPE_NXT_KEY66_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_KEY7	*/ 
/*	 Fields IPE_NXT_KEY7	 */
#define IPE_NXT_KEY77_WIDTH                                                  32
#define IPE_NXT_KEY77_SHIFT                                                   0
#define IPE_NXT_KEY77_MASK                                           0xffffffff
#define IPE_NXT_KEY77_RD(src)                            (((src) & 0xffffffff))
#define IPE_NXT_KEY77_WR(src)                       (((u32)(src)) & 0xffffffff)
#define IPE_NXT_KEY77_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_IN_DGST0	*/ 
/*	 Fields IPE_NXT_IN_DGST0	 */
#define IPE_NXT_IN_DGST00_WIDTH                                              32
#define IPE_NXT_IN_DGST00_SHIFT                                               0
#define IPE_NXT_IN_DGST00_MASK                                       0xffffffff
#define IPE_NXT_IN_DGST00_RD(src)                        (((src) & 0xffffffff))
#define IPE_NXT_IN_DGST00_WR(src)                   (((u32)(src)) & 0xffffffff)
#define IPE_NXT_IN_DGST00_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_IN_DGST1	*/ 
/*	 Fields IPE_NXT_IN_DGST1	 */
#define IPE_NXT_IN_DGST11_WIDTH                                              32
#define IPE_NXT_IN_DGST11_SHIFT                                               0
#define IPE_NXT_IN_DGST11_MASK                                       0xffffffff
#define IPE_NXT_IN_DGST11_RD(src)                        (((src) & 0xffffffff))
#define IPE_NXT_IN_DGST11_WR(src)                   (((u32)(src)) & 0xffffffff)
#define IPE_NXT_IN_DGST11_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_IN_DGST2	*/ 
/*	 Fields IPE_NXT_IN_DGST2	 */
#define IPE_NXT_IN_DGST22_WIDTH                                              32
#define IPE_NXT_IN_DGST22_SHIFT                                               0
#define IPE_NXT_IN_DGST22_MASK                                       0xffffffff
#define IPE_NXT_IN_DGST22_RD(src)                        (((src) & 0xffffffff))
#define IPE_NXT_IN_DGST22_WR(src)                   (((u32)(src)) & 0xffffffff)
#define IPE_NXT_IN_DGST22_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_IN_DGST3	*/ 
/*	 Fields IPE_NXT_IN_DGST3	 */
#define IPE_NXT_IN_DGST33_WIDTH                                              32
#define IPE_NXT_IN_DGST33_SHIFT                                               0
#define IPE_NXT_IN_DGST33_MASK                                       0xffffffff
#define IPE_NXT_IN_DGST33_RD(src)                        (((src) & 0xffffffff))
#define IPE_NXT_IN_DGST33_WR(src)                   (((u32)(src)) & 0xffffffff)
#define IPE_NXT_IN_DGST33_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_IN_DGST4	*/ 
/*	 Fields IPE_NXT_IN_DGST4	 */
#define IPE_NXT_IN_DGST44_WIDTH                                              32
#define IPE_NXT_IN_DGST44_SHIFT                                               0
#define IPE_NXT_IN_DGST44_MASK                                       0xffffffff
#define IPE_NXT_IN_DGST44_RD(src)                        (((src) & 0xffffffff))
#define IPE_NXT_IN_DGST44_WR(src)                   (((u32)(src)) & 0xffffffff)
#define IPE_NXT_IN_DGST44_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_IN_DGST5	*/ 
/*	 Fields IPE_NXT_IN_DGST5	 */
#define IPE_NXT_IN_DGST55_WIDTH                                              32
#define IPE_NXT_IN_DGST55_SHIFT                                               0
#define IPE_NXT_IN_DGST55_MASK                                       0xffffffff
#define IPE_NXT_IN_DGST55_RD(src)                        (((src) & 0xffffffff))
#define IPE_NXT_IN_DGST55_WR(src)                   (((u32)(src)) & 0xffffffff)
#define IPE_NXT_IN_DGST55_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_IN_DGST6	*/ 
/*	 Fields IPE_NXT_IN_DGST6	 */
#define IPE_NXT_IN_DGST66_WIDTH                                              32
#define IPE_NXT_IN_DGST66_SHIFT                                               0
#define IPE_NXT_IN_DGST66_MASK                                       0xffffffff
#define IPE_NXT_IN_DGST66_RD(src)                        (((src) & 0xffffffff))
#define IPE_NXT_IN_DGST66_WR(src)                   (((u32)(src)) & 0xffffffff)
#define IPE_NXT_IN_DGST66_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_IN_DGST7	*/ 
/*	 Fields IPE_NXT_IN_DGST7	 */
#define IPE_NXT_IN_DGST77_WIDTH                                              32
#define IPE_NXT_IN_DGST77_SHIFT                                               0
#define IPE_NXT_IN_DGST77_MASK                                       0xffffffff
#define IPE_NXT_IN_DGST77_RD(src)                        (((src) & 0xffffffff))
#define IPE_NXT_IN_DGST77_WR(src)                   (((u32)(src)) & 0xffffffff)
#define IPE_NXT_IN_DGST77_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_OUT_DGST0	*/ 
/*	 Fields IPE_NXT_OUT_DGST0	 */
#define IPE_NXT_OUT_DGST00_WIDTH                                             32
#define IPE_NXT_OUT_DGST00_SHIFT                                              0
#define IPE_NXT_OUT_DGST00_MASK                                      0xffffffff
#define IPE_NXT_OUT_DGST00_RD(src)                       (((src) & 0xffffffff))
#define IPE_NXT_OUT_DGST00_WR(src)                  (((u32)(src)) & 0xffffffff)
#define IPE_NXT_OUT_DGST00_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_OUT_DGST1	*/ 
/*	 Fields IPE_NXT_OUT_DGST1	 */
#define IPE_NXT_OUT_DGST11_WIDTH                                             32
#define IPE_NXT_OUT_DGST11_SHIFT                                              0
#define IPE_NXT_OUT_DGST11_MASK                                      0xffffffff
#define IPE_NXT_OUT_DGST11_RD(src)                       (((src) & 0xffffffff))
#define IPE_NXT_OUT_DGST11_WR(src)                  (((u32)(src)) & 0xffffffff)
#define IPE_NXT_OUT_DGST11_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_OUT_DGST2	*/ 
/*	 Fields IPE_NXT_OUT_DGST2	 */
#define IPE_NXT_OUT_DGST22_WIDTH                                             32
#define IPE_NXT_OUT_DGST22_SHIFT                                              0
#define IPE_NXT_OUT_DGST22_MASK                                      0xffffffff
#define IPE_NXT_OUT_DGST22_RD(src)                       (((src) & 0xffffffff))
#define IPE_NXT_OUT_DGST22_WR(src)                  (((u32)(src)) & 0xffffffff)
#define IPE_NXT_OUT_DGST22_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_OUT_DGST3	*/ 
/*	 Fields IPE_NXT_OUT_DGST3	 */
#define IPE_NXT_OUT_DGST33_WIDTH                                             32
#define IPE_NXT_OUT_DGST33_SHIFT                                              0
#define IPE_NXT_OUT_DGST33_MASK                                      0xffffffff
#define IPE_NXT_OUT_DGST33_RD(src)                       (((src) & 0xffffffff))
#define IPE_NXT_OUT_DGST33_WR(src)                  (((u32)(src)) & 0xffffffff)
#define IPE_NXT_OUT_DGST33_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_OUT_DGST4	*/ 
/*	 Fields IPE_NXT_OUT_DGST4	 */
#define IPE_NXT_OUT_DGST44_WIDTH                                             32
#define IPE_NXT_OUT_DGST44_SHIFT                                              0
#define IPE_NXT_OUT_DGST44_MASK                                      0xffffffff
#define IPE_NXT_OUT_DGST44_RD(src)                       (((src) & 0xffffffff))
#define IPE_NXT_OUT_DGST44_WR(src)                  (((u32)(src)) & 0xffffffff)
#define IPE_NXT_OUT_DGST44_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_OUT_DGST5	*/ 
/*	 Fields IPE_NXT_OUT_DGST5	 */
#define IPE_NXT_OUT_DGST55_WIDTH                                             32
#define IPE_NXT_OUT_DGST55_SHIFT                                              0
#define IPE_NXT_OUT_DGST55_MASK                                      0xffffffff
#define IPE_NXT_OUT_DGST55_RD(src)                       (((src) & 0xffffffff))
#define IPE_NXT_OUT_DGST55_WR(src)                  (((u32)(src)) & 0xffffffff)
#define IPE_NXT_OUT_DGST55_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_OUT_DGST6	*/ 
/*	 Fields IPE_NXT_OUT_DGST6	 */
#define IPE_NXT_OUT_DGST66_WIDTH                                             32
#define IPE_NXT_OUT_DGST66_SHIFT                                              0
#define IPE_NXT_OUT_DGST66_MASK                                      0xffffffff
#define IPE_NXT_OUT_DGST66_RD(src)                       (((src) & 0xffffffff))
#define IPE_NXT_OUT_DGST66_WR(src)                  (((u32)(src)) & 0xffffffff)
#define IPE_NXT_OUT_DGST66_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_OUT_DGST7	*/ 
/*	 Fields IPE_NXT_OUT_DGST7	 */
#define IPE_NXT_OUT_DGST77_WIDTH                                             32
#define IPE_NXT_OUT_DGST77_SHIFT                                              0
#define IPE_NXT_OUT_DGST77_MASK                                      0xffffffff
#define IPE_NXT_OUT_DGST77_RD(src)                       (((src) & 0xffffffff))
#define IPE_NXT_OUT_DGST77_WR(src)                  (((u32)(src)) & 0xffffffff)
#define IPE_NXT_OUT_DGST77_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_DGST_CNT	*/ 
/*	 Fields IPE_NXT_DGST_CNT	 */
#define IPE_NXT_DGST_CNT_WIDTH                                               32
#define IPE_NXT_DGST_CNT_SHIFT                                                0
#define IPE_NXT_DGST_CNT_MASK                                        0xffffffff
#define IPE_NXT_DGST_CNT_RD(src)                         (((src) & 0xffffffff))
#define IPE_NXT_DGST_CNT_WR(src)                    (((u32)(src)) & 0xffffffff)
#define IPE_NXT_DGST_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_SPI_SSRC	*/ 
/*	 Fields IPE_NXT_SPI_SSRC	 */
#define IPE_NXT_SPI_SSRC_WIDTH                                               32
#define IPE_NXT_SPI_SSRC_SHIFT                                                0
#define IPE_NXT_SPI_SSRC_MASK                                        0xffffffff
#define IPE_NXT_SPI_SSRC_RD(src)                         (((src) & 0xffffffff))
#define IPE_NXT_SPI_SSRC_WR(src)                    (((u32)(src)) & 0xffffffff)
#define IPE_NXT_SPI_SSRC_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_SN	*/ 
/*	 Fields IPE_NXT_SN	 */
#define IPE_NXT_SN_WIDTH                                                     32
#define IPE_NXT_SN_SHIFT                                                      0
#define IPE_NXT_SN_MASK                                              0xffffffff
#define IPE_NXT_SN_RD(src)                               (((src) & 0xffffffff))
#define IPE_NXT_SN_WR(src)                          (((u32)(src)) & 0xffffffff)
#define IPE_NXT_SN_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_ESN	*/ 
/*	 Fields IPE_NXT_ESN	 */
#define IPE_NXT_ESN_WIDTH                                                    32
#define IPE_NXT_ESN_SHIFT                                                     0
#define IPE_NXT_ESN_MASK                                             0xffffffff
#define IPE_NXT_ESN_RD(src)                              (((src) & 0xffffffff))
#define IPE_NXT_ESN_WR(src)                         (((u32)(src)) & 0xffffffff)
#define IPE_NXT_ESN_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_SN_M0	*/ 
/*	 Fields IPE_NXT_SN_M0	 */
#define IPE_NXT_SN_M00_WIDTH                                                 32
#define IPE_NXT_SN_M00_SHIFT                                                  0
#define IPE_NXT_SN_M00_MASK                                          0xffffffff
#define IPE_NXT_SN_M00_RD(src)                           (((src) & 0xffffffff))
#define IPE_NXT_SN_M00_WR(src)                      (((u32)(src)) & 0xffffffff)
#define IPE_NXT_SN_M00_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_SN_M1	*/ 
/*	 Fields IPE_NXT_SN_M1	 */
#define IPE_NXT_SN_M11_WIDTH                                                 32
#define IPE_NXT_SN_M11_SHIFT                                                  0
#define IPE_NXT_SN_M11_MASK                                          0xffffffff
#define IPE_NXT_SN_M11_RD(src)                           (((src) & 0xffffffff))
#define IPE_NXT_SN_M11_WR(src)                      (((u32)(src)) & 0xffffffff)
#define IPE_NXT_SN_M11_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_SN_M2	*/ 
/*	 Fields IPE_NXT_SN_M2	 */
#define IPE_NXT_SN_M22_WIDTH                                                 32
#define IPE_NXT_SN_M22_SHIFT                                                  0
#define IPE_NXT_SN_M22_MASK                                          0xffffffff
#define IPE_NXT_SN_M22_RD(src)                           (((src) & 0xffffffff))
#define IPE_NXT_SN_M22_WR(src)                      (((u32)(src)) & 0xffffffff)
#define IPE_NXT_SN_M22_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_SN_M3	*/ 
/*	 Fields IPE_NXT_SN_M3	 */
#define IPE_NXT_SN_M33_WIDTH                                                 32
#define IPE_NXT_SN_M33_SHIFT                                                  0
#define IPE_NXT_SN_M33_MASK                                          0xffffffff
#define IPE_NXT_SN_M33_RD(src)                           (((src) & 0xffffffff))
#define IPE_NXT_SN_M33_WR(src)                      (((u32)(src)) & 0xffffffff)
#define IPE_NXT_SN_M33_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_CS	*/ 
/*	 Fields IPE_NXT_CS	 */
#define IPE_NXT_CS_WIDTH                                                     32
#define IPE_NXT_CS_SHIFT                                                      0
#define IPE_NXT_CS_MASK                                              0xffffffff
#define IPE_NXT_CS_RD(src)                               (((src) & 0xffffffff))
#define IPE_NXT_CS_WR(src)                          (((u32)(src)) & 0xffffffff)
#define IPE_NXT_CS_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_UP_PKT_LEN	*/ 
/*	 Fields IPE_NXT_UP_PKT_LEN	 */
#define IPE_NXT_UP_PKT_LEN_WIDTH                                             32
#define IPE_NXT_UP_PKT_LEN_SHIFT                                              0
#define IPE_NXT_UP_PKT_LEN_MASK                                      0xffffffff
#define IPE_NXT_UP_PKT_LEN_RD(src)                       (((src) & 0xffffffff))
#define IPE_NXT_UP_PKT_LEN_WR(src)                  (((u32)(src)) & 0xffffffff)
#define IPE_NXT_UP_PKT_LEN_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_ACT_CTX_CMD0	*/ 
/*	 Fields IPE_ACT_CTX_CMD0	 */
#define IPE_ACT_CTX_CMD00_WIDTH                                              32
#define IPE_ACT_CTX_CMD00_SHIFT                                               0
#define IPE_ACT_CTX_CMD00_MASK                                       0xffffffff
#define IPE_ACT_CTX_CMD00_RD(src)                        (((src) & 0xffffffff))
#define IPE_ACT_CTX_CMD00_WR(src)                   (((u32)(src)) & 0xffffffff)
#define IPE_ACT_CTX_CMD00_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_ACT_CTX_CMD1	*/ 
/*	 Fields IPE_ACT_CTX_CMD1	 */
#define IPE_ACT_CTX_CMD11_WIDTH                                              32
#define IPE_ACT_CTX_CMD11_SHIFT                                               0
#define IPE_ACT_CTX_CMD11_MASK                                       0xffffffff
#define IPE_ACT_CTX_CMD11_RD(src)                        (((src) & 0xffffffff))
#define IPE_ACT_CTX_CMD11_WR(src)                   (((u32)(src)) & 0xffffffff)
#define IPE_ACT_CTX_CMD11_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_ACT_GPR0	*/ 
/*	 Fields IPE_ACT_GPR0	 */
#define IPE_ACT_GPR00_WIDTH                                                  32
#define IPE_ACT_GPR00_SHIFT                                                   0
#define IPE_ACT_GPR00_MASK                                           0xffffffff
#define IPE_ACT_GPR00_RD(src)                            (((src) & 0xffffffff))
#define IPE_ACT_GPR00_WR(src)                       (((u32)(src)) & 0xffffffff)
#define IPE_ACT_GPR00_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_ACT_GPR1	*/ 
/*	 Fields IPE_ACT_GPR1	 */
#define IPE_ACT_GPR11_WIDTH                                                  32
#define IPE_ACT_GPR11_SHIFT                                                   0
#define IPE_ACT_GPR11_MASK                                           0xffffffff
#define IPE_ACT_GPR11_RD(src)                            (((src) & 0xffffffff))
#define IPE_ACT_GPR11_WR(src)                       (((u32)(src)) & 0xffffffff)
#define IPE_ACT_GPR11_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_IV0	*/ 
/*	 Fields IPE_IV0	 */
#define IPE_IV00_WIDTH                                                       32
#define IPE_IV00_SHIFT                                                        0
#define IPE_IV00_MASK                                                0xffffffff
#define IPE_IV00_RD(src)                                 (((src) & 0xffffffff))
#define IPE_IV00_WR(src)                            (((u32)(src)) & 0xffffffff)
#define IPE_IV00_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_IV1	*/ 
/*	 Fields IPE_IV1	 */
#define IPE_IV11_WIDTH                                                       32
#define IPE_IV11_SHIFT                                                        0
#define IPE_IV11_MASK                                                0xffffffff
#define IPE_IV11_RD(src)                                 (((src) & 0xffffffff))
#define IPE_IV11_WR(src)                            (((u32)(src)) & 0xffffffff)
#define IPE_IV11_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_IV2	*/ 
/*	 Fields IPE_IV2	 */
#define IPE_IV22_WIDTH                                                       32
#define IPE_IV22_SHIFT                                                        0
#define IPE_IV22_MASK                                                0xffffffff
#define IPE_IV22_RD(src)                                 (((src) & 0xffffffff))
#define IPE_IV22_WR(src)                            (((u32)(src)) & 0xffffffff)
#define IPE_IV22_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_IV3	*/ 
/*	 Fields IPE_IV3	 */
#define IPE_IV33_WIDTH                                                       32
#define IPE_IV33_SHIFT                                                        0
#define IPE_IV33_MASK                                                0xffffffff
#define IPE_IV33_RD(src)                                 (((src) & 0xffffffff))
#define IPE_IV33_WR(src)                            (((u32)(src)) & 0xffffffff)
#define IPE_IV33_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_KEY0	*/ 
/*	 Fields IPE_KEY0	 */
#define IPE_KEY00_WIDTH                                                      32
#define IPE_KEY00_SHIFT                                                       0
#define IPE_KEY00_MASK                                               0xffffffff
#define IPE_KEY00_RD(src)                                (((src) & 0xffffffff))
#define IPE_KEY00_WR(src)                           (((u32)(src)) & 0xffffffff)
#define IPE_KEY00_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_KEY1	*/ 
/*	 Fields IPE_KEY1	 */
#define IPE_KEY11_WIDTH                                                      32
#define IPE_KEY11_SHIFT                                                       0
#define IPE_KEY11_MASK                                               0xffffffff
#define IPE_KEY11_RD(src)                                (((src) & 0xffffffff))
#define IPE_KEY11_WR(src)                           (((u32)(src)) & 0xffffffff)
#define IPE_KEY11_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_KEY2	*/ 
/*	 Fields IPE_KEY2	 */
#define IPE_KEY22_WIDTH                                                      32
#define IPE_KEY22_SHIFT                                                       0
#define IPE_KEY22_MASK                                               0xffffffff
#define IPE_KEY22_RD(src)                                (((src) & 0xffffffff))
#define IPE_KEY22_WR(src)                           (((u32)(src)) & 0xffffffff)
#define IPE_KEY22_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_KEY3	*/ 
/*	 Fields IPE_KEY3	 */
#define IPE_KEY33_WIDTH                                                      32
#define IPE_KEY33_SHIFT                                                       0
#define IPE_KEY33_MASK                                               0xffffffff
#define IPE_KEY33_RD(src)                                (((src) & 0xffffffff))
#define IPE_KEY33_WR(src)                           (((u32)(src)) & 0xffffffff)
#define IPE_KEY33_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_KEY4	*/ 
/*	 Fields IPE_KEY4	 */
#define IPE_KEY44_WIDTH                                                      32
#define IPE_KEY44_SHIFT                                                       0
#define IPE_KEY44_MASK                                               0xffffffff
#define IPE_KEY44_RD(src)                                (((src) & 0xffffffff))
#define IPE_KEY44_WR(src)                           (((u32)(src)) & 0xffffffff)
#define IPE_KEY44_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_KEY5	*/ 
/*	 Fields IPE_KEY5	 */
#define IPE_KEY55_WIDTH                                                      32
#define IPE_KEY55_SHIFT                                                       0
#define IPE_KEY55_MASK                                               0xffffffff
#define IPE_KEY55_RD(src)                                (((src) & 0xffffffff))
#define IPE_KEY55_WR(src)                           (((u32)(src)) & 0xffffffff)
#define IPE_KEY55_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_KEY6	*/ 
/*	 Fields IPE_KEY6	 */
#define IPE_KEY66_WIDTH                                                      32
#define IPE_KEY66_SHIFT                                                       0
#define IPE_KEY66_MASK                                               0xffffffff
#define IPE_KEY66_RD(src)                                (((src) & 0xffffffff))
#define IPE_KEY66_WR(src)                           (((u32)(src)) & 0xffffffff)
#define IPE_KEY66_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_KEY7	*/ 
/*	 Fields IPE_KEY7	 */
#define IPE_KEY77_WIDTH                                                      32
#define IPE_KEY77_SHIFT                                                       0
#define IPE_KEY77_MASK                                               0xffffffff
#define IPE_KEY77_RD(src)                                (((src) & 0xffffffff))
#define IPE_KEY77_WR(src)                           (((u32)(src)) & 0xffffffff)
#define IPE_KEY77_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_IN_DGST0	*/ 
/*	 Fields IPE_IN_DGST0	 */
#define IPE_IN_DGST00_WIDTH                                                  32
#define IPE_IN_DGST00_SHIFT                                                   0
#define IPE_IN_DGST00_MASK                                           0xffffffff
#define IPE_IN_DGST00_RD(src)                            (((src) & 0xffffffff))
#define IPE_IN_DGST00_WR(src)                       (((u32)(src)) & 0xffffffff)
#define IPE_IN_DGST00_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_IN_DGST1	*/ 
/*	 Fields IPE_IN_DGST1	 */
#define IPE_IN_DGST11_WIDTH                                                  32
#define IPE_IN_DGST11_SHIFT                                                   0
#define IPE_IN_DGST11_MASK                                           0xffffffff
#define IPE_IN_DGST11_RD(src)                            (((src) & 0xffffffff))
#define IPE_IN_DGST11_WR(src)                       (((u32)(src)) & 0xffffffff)
#define IPE_IN_DGST11_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_IN_DGST2	*/ 
/*	 Fields IPE_IN_DGST2	 */
#define IPE_IN_DGST22_WIDTH                                                  32
#define IPE_IN_DGST22_SHIFT                                                   0
#define IPE_IN_DGST22_MASK                                           0xffffffff
#define IPE_IN_DGST22_RD(src)                            (((src) & 0xffffffff))
#define IPE_IN_DGST22_WR(src)                       (((u32)(src)) & 0xffffffff)
#define IPE_IN_DGST22_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_IN_DGST3	*/ 
/*	 Fields IPE_IN_DGST3	 */
#define IPE_IN_DGST33_WIDTH                                                  32
#define IPE_IN_DGST33_SHIFT                                                   0
#define IPE_IN_DGST33_MASK                                           0xffffffff
#define IPE_IN_DGST33_RD(src)                            (((src) & 0xffffffff))
#define IPE_IN_DGST33_WR(src)                       (((u32)(src)) & 0xffffffff)
#define IPE_IN_DGST33_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_IN_DGST4	*/ 
/*	 Fields IPE_IN_DGST4	 */
#define IPE_IN_DGST44_WIDTH                                                  32
#define IPE_IN_DGST44_SHIFT                                                   0
#define IPE_IN_DGST44_MASK                                           0xffffffff
#define IPE_IN_DGST44_RD(src)                            (((src) & 0xffffffff))
#define IPE_IN_DGST44_WR(src)                       (((u32)(src)) & 0xffffffff)
#define IPE_IN_DGST44_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_IN_DGST5	*/ 
/*	 Fields IPE_IN_DGST5	 */
#define IPE_IN_DGST55_WIDTH                                                  32
#define IPE_IN_DGST55_SHIFT                                                   0
#define IPE_IN_DGST55_MASK                                           0xffffffff
#define IPE_IN_DGST55_RD(src)                            (((src) & 0xffffffff))
#define IPE_IN_DGST55_WR(src)                       (((u32)(src)) & 0xffffffff)
#define IPE_IN_DGST55_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_IN_DGST6	*/ 
/*	 Fields IPE_IN_DGST6	 */
#define IPE_IN_DGST66_WIDTH                                                  32
#define IPE_IN_DGST66_SHIFT                                                   0
#define IPE_IN_DGST66_MASK                                           0xffffffff
#define IPE_IN_DGST66_RD(src)                            (((src) & 0xffffffff))
#define IPE_IN_DGST66_WR(src)                       (((u32)(src)) & 0xffffffff)
#define IPE_IN_DGST66_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_IN_DGST7	*/ 
/*	 Fields IPE_IN_DGST7	 */
#define IPE_IN_DGST77_WIDTH                                                  32
#define IPE_IN_DGST77_SHIFT                                                   0
#define IPE_IN_DGST77_MASK                                           0xffffffff
#define IPE_IN_DGST77_RD(src)                            (((src) & 0xffffffff))
#define IPE_IN_DGST77_WR(src)                       (((u32)(src)) & 0xffffffff)
#define IPE_IN_DGST77_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_OUT_DGST0	*/ 
/*	 Fields IPE_OUT_DGST0	 */
#define IPE_OUT_DGST00_WIDTH                                                 32
#define IPE_OUT_DGST00_SHIFT                                                  0
#define IPE_OUT_DGST00_MASK                                          0xffffffff
#define IPE_OUT_DGST00_RD(src)                           (((src) & 0xffffffff))
#define IPE_OUT_DGST00_WR(src)                      (((u32)(src)) & 0xffffffff)
#define IPE_OUT_DGST00_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_OUT_DGST1	*/ 
/*	 Fields IPE_OUT_DGST1	 */
#define IPE_OUT_DGST11_WIDTH                                                 32
#define IPE_OUT_DGST11_SHIFT                                                  0
#define IPE_OUT_DGST11_MASK                                          0xffffffff
#define IPE_OUT_DGST11_RD(src)                           (((src) & 0xffffffff))
#define IPE_OUT_DGST11_WR(src)                      (((u32)(src)) & 0xffffffff)
#define IPE_OUT_DGST11_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_OUT_DGST2	*/ 
/*	 Fields IPE_OUT_DGST2	 */
#define IPE_OUT_DGST22_WIDTH                                                 32
#define IPE_OUT_DGST22_SHIFT                                                  0
#define IPE_OUT_DGST22_MASK                                          0xffffffff
#define IPE_OUT_DGST22_RD(src)                           (((src) & 0xffffffff))
#define IPE_OUT_DGST22_WR(src)                      (((u32)(src)) & 0xffffffff)
#define IPE_OUT_DGST22_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_OUT_DGST3	*/ 
/*	 Fields IPE_OUT_DGST3	 */
#define IPE_OUT_DGST33_WIDTH                                                 32
#define IPE_OUT_DGST33_SHIFT                                                  0
#define IPE_OUT_DGST33_MASK                                          0xffffffff
#define IPE_OUT_DGST33_RD(src)                           (((src) & 0xffffffff))
#define IPE_OUT_DGST33_WR(src)                      (((u32)(src)) & 0xffffffff)
#define IPE_OUT_DGST33_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_OUT_DGST4	*/ 
/*	 Fields IPE_OUT_DGST4	 */
#define IPE_OUT_DGST44_WIDTH                                                 32
#define IPE_OUT_DGST44_SHIFT                                                  0
#define IPE_OUT_DGST44_MASK                                          0xffffffff
#define IPE_OUT_DGST44_RD(src)                           (((src) & 0xffffffff))
#define IPE_OUT_DGST44_WR(src)                      (((u32)(src)) & 0xffffffff)
#define IPE_OUT_DGST44_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_OUT_DGST5	*/ 
/*	 Fields IPE_OUT_DGST5	 */
#define IPE_OUT_DGST55_WIDTH                                                 32
#define IPE_OUT_DGST55_SHIFT                                                  0
#define IPE_OUT_DGST55_MASK                                          0xffffffff
#define IPE_OUT_DGST55_RD(src)                           (((src) & 0xffffffff))
#define IPE_OUT_DGST55_WR(src)                      (((u32)(src)) & 0xffffffff)
#define IPE_OUT_DGST55_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_OUT_DGST6	*/ 
/*	 Fields IPE_OUT_DGST6	 */
#define IPE_OUT_DGST66_WIDTH                                                 32
#define IPE_OUT_DGST66_SHIFT                                                  0
#define IPE_OUT_DGST66_MASK                                          0xffffffff
#define IPE_OUT_DGST66_RD(src)                           (((src) & 0xffffffff))
#define IPE_OUT_DGST66_WR(src)                      (((u32)(src)) & 0xffffffff)
#define IPE_OUT_DGST66_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_OUT_DGST7	*/ 
/*	 Fields IPE_OUT_DGST7	 */
#define IPE_OUT_DGST77_WIDTH                                                 32
#define IPE_OUT_DGST77_SHIFT                                                  0
#define IPE_OUT_DGST77_MASK                                          0xffffffff
#define IPE_OUT_DGST77_RD(src)                           (((src) & 0xffffffff))
#define IPE_OUT_DGST77_WR(src)                      (((u32)(src)) & 0xffffffff)
#define IPE_OUT_DGST77_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_DGST_CNT	*/ 
/*	 Fields IPE_DGST_CNT	 */
#define IPE_DGST_CNT_WIDTH                                                   32
#define IPE_DGST_CNT_SHIFT                                                    0
#define IPE_DGST_CNT_MASK                                            0xffffffff
#define IPE_DGST_CNT_RD(src)                             (((src) & 0xffffffff))
#define IPE_DGST_CNT_WR(src)                        (((u32)(src)) & 0xffffffff)
#define IPE_DGST_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_SPI_SSRC	*/ 
/*	 Fields IPE_SPI_SSRC	 */
#define IPE_SPI_SSRC_WIDTH                                                   32
#define IPE_SPI_SSRC_SHIFT                                                    0
#define IPE_SPI_SSRC_MASK                                            0xffffffff
#define IPE_SPI_SSRC_RD(src)                             (((src) & 0xffffffff))
#define IPE_SPI_SSRC_WR(src)                        (((u32)(src)) & 0xffffffff)
#define IPE_SPI_SSRC_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_SN	*/ 
/*	 Fields IPE_SN	 */
#define IPE_SN_WIDTH                                                         32
#define IPE_SN_SHIFT                                                          0
#define IPE_SN_MASK                                                  0xffffffff
#define IPE_SN_RD(src)                                   (((src) & 0xffffffff))
#define IPE_SN_WR(src)                              (((u32)(src)) & 0xffffffff)
#define IPE_SN_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_ESN	*/ 
/*	 Fields IPE_ESN	 */
#define IPE_ESN_WIDTH                                                        32
#define IPE_ESN_SHIFT                                                         0
#define IPE_ESN_MASK                                                 0xffffffff
#define IPE_ESN_RD(src)                                  (((src) & 0xffffffff))
#define IPE_ESN_WR(src)                             (((u32)(src)) & 0xffffffff)
#define IPE_ESN_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_SN_M0	*/ 
/*	 Fields IPE_SN_M0	 */
#define IPE_SN_M00_WIDTH                                                     32
#define IPE_SN_M00_SHIFT                                                      0
#define IPE_SN_M00_MASK                                              0xffffffff
#define IPE_SN_M00_RD(src)                               (((src) & 0xffffffff))
#define IPE_SN_M00_WR(src)                          (((u32)(src)) & 0xffffffff)
#define IPE_SN_M00_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_SN_M1	*/ 
/*	 Fields IPE_SN_M1	 */
#define IPE_SN_M11_WIDTH                                                     32
#define IPE_SN_M11_SHIFT                                                      0
#define IPE_SN_M11_MASK                                              0xffffffff
#define IPE_SN_M11_RD(src)                               (((src) & 0xffffffff))
#define IPE_SN_M11_WR(src)                          (((u32)(src)) & 0xffffffff)
#define IPE_SN_M11_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_SN_M2	*/ 
/*	 Fields IPE_SN_M2	 */
#define IPE_SN_M22_WIDTH                                                     32
#define IPE_SN_M22_SHIFT                                                      0
#define IPE_SN_M22_MASK                                              0xffffffff
#define IPE_SN_M22_RD(src)                               (((src) & 0xffffffff))
#define IPE_SN_M22_WR(src)                          (((u32)(src)) & 0xffffffff)
#define IPE_SN_M22_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_SN_M3	*/ 
/*	 Fields IPE_SN_M3	 */
#define IPE_SN_M33_WIDTH                                                     32
#define IPE_SN_M33_SHIFT                                                      0
#define IPE_SN_M33_MASK                                              0xffffffff
#define IPE_SN_M33_RD(src)                               (((src) & 0xffffffff))
#define IPE_SN_M33_WR(src)                          (((u32)(src)) & 0xffffffff)
#define IPE_SN_M33_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_CS	*/ 
/*	 Fields IPE_CS	 */
#define IPE_CS_WIDTH                                                         32
#define IPE_CS_SHIFT                                                          0
#define IPE_CS_MASK                                                  0xffffffff
#define IPE_CS_RD(src)                                   (((src) & 0xffffffff))
#define IPE_CS_WR(src)                              (((u32)(src)) & 0xffffffff)
#define IPE_CS_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_UP_PKT_LEN	*/ 
/*	 Fields IPE_UP_PKT_LEN	 */
#define IPE_UP_PKT_LEN_WIDTH                                                 32
#define IPE_UP_PKT_LEN_SHIFT                                                  0
#define IPE_UP_PKT_LEN_MASK                                          0xffffffff
#define IPE_UP_PKT_LEN_RD(src)                           (((src) & 0xffffffff))
#define IPE_UP_PKT_LEN_WR(src)                      (((u32)(src)) & 0xffffffff)
#define IPE_UP_PKT_LEN_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_HASH_RES0	*/ 
/*	 Fields IPE_HASH_RES0	 */
#define IPE_HASH_RES00_WIDTH                                                 32
#define IPE_HASH_RES00_SHIFT                                                  0
#define IPE_HASH_RES00_MASK                                          0xffffffff
#define IPE_HASH_RES00_RD(src)                           (((src) & 0xffffffff))
#define IPE_HASH_RES00_WR(src)                      (((u32)(src)) & 0xffffffff)
#define IPE_HASH_RES00_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_HASH_RES1	*/ 
/*	 Fields IPE_HASH_RES1	 */
#define IPE_HASH_RES11_WIDTH                                                 32
#define IPE_HASH_RES11_SHIFT                                                  0
#define IPE_HASH_RES11_MASK                                          0xffffffff
#define IPE_HASH_RES11_RD(src)                           (((src) & 0xffffffff))
#define IPE_HASH_RES11_WR(src)                      (((u32)(src)) & 0xffffffff)
#define IPE_HASH_RES11_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_HASH_RES2	*/ 
/*	 Fields IPE_HASH_RES2	 */
#define IPE_HASH_RES22_WIDTH                                                 32
#define IPE_HASH_RES22_SHIFT                                                  0
#define IPE_HASH_RES22_MASK                                          0xffffffff
#define IPE_HASH_RES22_RD(src)                           (((src) & 0xffffffff))
#define IPE_HASH_RES22_WR(src)                      (((u32)(src)) & 0xffffffff)
#define IPE_HASH_RES22_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_HASH_RES3	*/ 
/*	 Fields IPE_HASH_RES3	 */
#define IPE_HASH_RES33_WIDTH                                                 32
#define IPE_HASH_RES33_SHIFT                                                  0
#define IPE_HASH_RES33_MASK                                          0xffffffff
#define IPE_HASH_RES33_RD(src)                           (((src) & 0xffffffff))
#define IPE_HASH_RES33_WR(src)                      (((u32)(src)) & 0xffffffff)
#define IPE_HASH_RES33_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_HASH_RES4	*/ 
/*	 Fields IPE_HASH_RES4	 */
#define IPE_HASH_RES44_WIDTH                                                 32
#define IPE_HASH_RES44_SHIFT                                                  0
#define IPE_HASH_RES44_MASK                                          0xffffffff
#define IPE_HASH_RES44_RD(src)                           (((src) & 0xffffffff))
#define IPE_HASH_RES44_WR(src)                      (((u32)(src)) & 0xffffffff)
#define IPE_HASH_RES44_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_HASH_RES5	*/ 
/*	 Fields IPE_HASH_RES5	 */
#define IPE_HASH_RES55_WIDTH                                                 32
#define IPE_HASH_RES55_SHIFT                                                  0
#define IPE_HASH_RES55_MASK                                          0xffffffff
#define IPE_HASH_RES55_RD(src)                           (((src) & 0xffffffff))
#define IPE_HASH_RES55_WR(src)                      (((u32)(src)) & 0xffffffff)
#define IPE_HASH_RES55_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_HASH_RES6	*/ 
/*	 Fields IPE_HASH_RES6	 */
#define IPE_HASH_RES66_WIDTH                                                 32
#define IPE_HASH_RES66_SHIFT                                                  0
#define IPE_HASH_RES66_MASK                                          0xffffffff
#define IPE_HASH_RES66_RD(src)                           (((src) & 0xffffffff))
#define IPE_HASH_RES66_WR(src)                      (((u32)(src)) & 0xffffffff)
#define IPE_HASH_RES66_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_HASH_RES7	*/ 
/*	 Fields IPE_HASH_RES7	 */
#define IPE_HASH_RES77_WIDTH                                                 32
#define IPE_HASH_RES77_SHIFT                                                  0
#define IPE_HASH_RES77_MASK                                          0xffffffff
#define IPE_HASH_RES77_RD(src)                           (((src) & 0xffffffff))
#define IPE_HASH_RES77_WR(src)                      (((u32)(src)) & 0xffffffff)
#define IPE_HASH_RES77_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_DGST_CNT_RES	*/ 
/*	 Fields IPE_DGST_CNT_RES	 */
#define IPE_DGST_CNT_RES_WIDTH                                               32
#define IPE_DGST_CNT_RES_SHIFT                                                0
#define IPE_DGST_CNT_RES_MASK                                        0xffffffff
#define IPE_DGST_CNT_RES_RD(src)                         (((src) & 0xffffffff))
#define IPE_DGST_CNT_RES_WR(src)                    (((u32)(src)) & 0xffffffff)
#define IPE_DGST_CNT_RES_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_SPI_RTVD	*/ 
/*	 Fields IPE_SPI_RTVD	 */
#define IPE_SPI_RTVD_WIDTH                                                   32
#define IPE_SPI_RTVD_SHIFT                                                    0
#define IPE_SPI_RTVD_MASK                                            0xffffffff
#define IPE_SPI_RTVD_RD(src)                             (((src) & 0xffffffff))
#define IPE_SPI_RTVD_WR(src)                        (((u32)(src)) & 0xffffffff)
#define IPE_SPI_RTVD_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_SN_RTVD	*/ 
/*	 Fields IPE_SN_RTVD	 */
#define IPE_SN_RTVD_WIDTH                                                    32
#define IPE_SN_RTVD_SHIFT                                                     0
#define IPE_SN_RTVD_MASK                                             0xffffffff
#define IPE_SN_RTVD_RD(src)                              (((src) & 0xffffffff))
#define IPE_SN_RTVD_WR(src)                         (((u32)(src)) & 0xffffffff)
#define IPE_SN_RTVD_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_ESN_CALC	*/ 
/*	 Fields IPE_ESN_CALC	 */
#define IPE_ESN_CALC_WIDTH                                                   32
#define IPE_ESN_CALC_SHIFT                                                    0
#define IPE_ESN_CALC_MASK                                            0xffffffff
#define IPE_ESN_CALC_RD(src)                             (((src) & 0xffffffff))
#define IPE_ESN_CALC_WR(src)                        (((u32)(src)) & 0xffffffff)
#define IPE_ESN_CALC_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_CS_RES	*/ 
/*	 Fields IPE_CS_RES	 */
#define IPE_CS_RES_WIDTH                                                     32
#define IPE_CS_RES_SHIFT                                                      0
#define IPE_CS_RES_MASK                                              0xffffffff
#define IPE_CS_RES_RD(src)                               (((src) & 0xffffffff))
#define IPE_CS_RES_WR(src)                          (((u32)(src)) & 0xffffffff)
#define IPE_CS_RES_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_IN_DGST8	*/ 
/*	 Fields IPE_NXT_IN_DGST8	 */
#define IPE_NXT_IN_DGST88_WIDTH                                              32
#define IPE_NXT_IN_DGST88_SHIFT                                               0
#define IPE_NXT_IN_DGST88_MASK                                       0xffffffff
#define IPE_NXT_IN_DGST88_RD(src)                        (((src) & 0xffffffff))
#define IPE_NXT_IN_DGST88_WR(src)                   (((u32)(src)) & 0xffffffff)
#define IPE_NXT_IN_DGST88_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_IN_DGST9	*/ 
/*	 Fields IPE_NXT_IN_DGST9	 */
#define IPE_NXT_IN_DGST99_WIDTH                                              32
#define IPE_NXT_IN_DGST99_SHIFT                                               0
#define IPE_NXT_IN_DGST99_MASK                                       0xffffffff
#define IPE_NXT_IN_DGST99_RD(src)                        (((src) & 0xffffffff))
#define IPE_NXT_IN_DGST99_WR(src)                   (((u32)(src)) & 0xffffffff)
#define IPE_NXT_IN_DGST99_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_IN_DGST10	*/ 
/*	 Fields IPE_NXT_IN_DGST10	 */
#define IPE_NXT_IN_DGST100_WIDTH                                             32
#define IPE_NXT_IN_DGST100_SHIFT                                              0
#define IPE_NXT_IN_DGST100_MASK                                      0xffffffff
#define IPE_NXT_IN_DGST100_RD(src)                       (((src) & 0xffffffff))
#define IPE_NXT_IN_DGST100_WR(src)                  (((u32)(src)) & 0xffffffff)
#define IPE_NXT_IN_DGST100_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_IN_DGST11	*/ 
/*	 Fields IPE_NXT_IN_DGST11	 */
#define IPE_NXT_IN_DGST111_WIDTH                                             32
#define IPE_NXT_IN_DGST111_SHIFT                                              0
#define IPE_NXT_IN_DGST111_MASK                                      0xffffffff
#define IPE_NXT_IN_DGST111_RD(src)                       (((src) & 0xffffffff))
#define IPE_NXT_IN_DGST111_WR(src)                  (((u32)(src)) & 0xffffffff)
#define IPE_NXT_IN_DGST111_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_IN_DGST12	*/ 
/*	 Fields IPE_NXT_IN_DGST12	 */
#define IPE_NXT_IN_DGST122_WIDTH                                             32
#define IPE_NXT_IN_DGST122_SHIFT                                              0
#define IPE_NXT_IN_DGST122_MASK                                      0xffffffff
#define IPE_NXT_IN_DGST122_RD(src)                       (((src) & 0xffffffff))
#define IPE_NXT_IN_DGST122_WR(src)                  (((u32)(src)) & 0xffffffff)
#define IPE_NXT_IN_DGST122_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_IN_DGST13	*/ 
/*	 Fields IPE_NXT_IN_DGST13	 */
#define IPE_NXT_IN_DGST133_WIDTH                                             32
#define IPE_NXT_IN_DGST133_SHIFT                                              0
#define IPE_NXT_IN_DGST133_MASK                                      0xffffffff
#define IPE_NXT_IN_DGST133_RD(src)                       (((src) & 0xffffffff))
#define IPE_NXT_IN_DGST133_WR(src)                  (((u32)(src)) & 0xffffffff)
#define IPE_NXT_IN_DGST133_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_IN_DGST14	*/ 
/*	 Fields IPE_NXT_IN_DGST14	 */
#define IPE_NXT_IN_DGST144_WIDTH                                             32
#define IPE_NXT_IN_DGST144_SHIFT                                              0
#define IPE_NXT_IN_DGST144_MASK                                      0xffffffff
#define IPE_NXT_IN_DGST144_RD(src)                       (((src) & 0xffffffff))
#define IPE_NXT_IN_DGST144_WR(src)                  (((u32)(src)) & 0xffffffff)
#define IPE_NXT_IN_DGST144_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_IN_DGST15	*/ 
/*	 Fields IPE_NXT_IN_DGST15	 */
#define IPE_NXT_IN_DGST155_WIDTH                                             32
#define IPE_NXT_IN_DGST155_SHIFT                                              0
#define IPE_NXT_IN_DGST155_MASK                                      0xffffffff
#define IPE_NXT_IN_DGST155_RD(src)                       (((src) & 0xffffffff))
#define IPE_NXT_IN_DGST155_WR(src)                  (((u32)(src)) & 0xffffffff)
#define IPE_NXT_IN_DGST155_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_OUT_DGST8	*/ 
/*	 Fields IPE_NXT_OUT_DGST8	 */
#define IPE_NXT_OUT_DGST88_WIDTH                                             32
#define IPE_NXT_OUT_DGST88_SHIFT                                              0
#define IPE_NXT_OUT_DGST88_MASK                                      0xffffffff
#define IPE_NXT_OUT_DGST88_RD(src)                       (((src) & 0xffffffff))
#define IPE_NXT_OUT_DGST88_WR(src)                  (((u32)(src)) & 0xffffffff)
#define IPE_NXT_OUT_DGST88_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_OUT_DGST9	*/ 
/*	 Fields IPE_NXT_OUT_DGST9	 */
#define IPE_NXT_OUT_DGST99_WIDTH                                             32
#define IPE_NXT_OUT_DGST99_SHIFT                                              0
#define IPE_NXT_OUT_DGST99_MASK                                      0xffffffff
#define IPE_NXT_OUT_DGST99_RD(src)                       (((src) & 0xffffffff))
#define IPE_NXT_OUT_DGST99_WR(src)                  (((u32)(src)) & 0xffffffff)
#define IPE_NXT_OUT_DGST99_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_OUT_DGST10	*/ 
/*	 Fields IPE_NXT_OUT_DGST10	 */
#define IPE_NXT_OUT_DGST100_WIDTH                                            32
#define IPE_NXT_OUT_DGST100_SHIFT                                             0
#define IPE_NXT_OUT_DGST100_MASK                                     0xffffffff
#define IPE_NXT_OUT_DGST100_RD(src)                      (((src) & 0xffffffff))
#define IPE_NXT_OUT_DGST100_WR(src)                 (((u32)(src)) & 0xffffffff)
#define IPE_NXT_OUT_DGST100_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_OUT_DGST11	*/ 
/*	 Fields IPE_NXT_OUT_DGST11	 */
#define IPE_NXT_OUT_DGST111_WIDTH                                            32
#define IPE_NXT_OUT_DGST111_SHIFT                                             0
#define IPE_NXT_OUT_DGST111_MASK                                     0xffffffff
#define IPE_NXT_OUT_DGST111_RD(src)                      (((src) & 0xffffffff))
#define IPE_NXT_OUT_DGST111_WR(src)                 (((u32)(src)) & 0xffffffff)
#define IPE_NXT_OUT_DGST111_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_OUT_DGST12	*/ 
/*	 Fields IPE_NXT_OUT_DGST12	 */
#define IPE_NXT_OUT_DGST122_WIDTH                                            32
#define IPE_NXT_OUT_DGST122_SHIFT                                             0
#define IPE_NXT_OUT_DGST122_MASK                                     0xffffffff
#define IPE_NXT_OUT_DGST122_RD(src)                      (((src) & 0xffffffff))
#define IPE_NXT_OUT_DGST122_WR(src)                 (((u32)(src)) & 0xffffffff)
#define IPE_NXT_OUT_DGST122_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_OUT_DGST13	*/ 
/*	 Fields IPE_NXT_OUT_DGST13	 */
#define IPE_NXT_OUT_DGST133_WIDTH                                            32
#define IPE_NXT_OUT_DGST133_SHIFT                                             0
#define IPE_NXT_OUT_DGST133_MASK                                     0xffffffff
#define IPE_NXT_OUT_DGST133_RD(src)                      (((src) & 0xffffffff))
#define IPE_NXT_OUT_DGST133_WR(src)                 (((u32)(src)) & 0xffffffff)
#define IPE_NXT_OUT_DGST133_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_OUT_DGST14	*/ 
/*	 Fields IPE_NXT_OUT_DGST14	 */
#define IPE_NXT_OUT_DGST144_WIDTH                                            32
#define IPE_NXT_OUT_DGST144_SHIFT                                             0
#define IPE_NXT_OUT_DGST144_MASK                                     0xffffffff
#define IPE_NXT_OUT_DGST144_RD(src)                      (((src) & 0xffffffff))
#define IPE_NXT_OUT_DGST144_WR(src)                 (((u32)(src)) & 0xffffffff)
#define IPE_NXT_OUT_DGST144_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_NXT_OUT_DGST15	*/ 
/*	 Fields IPE_NXT_OUT_DGST15	 */
#define IPE_NXT_OUT_DGST155_WIDTH                                            32
#define IPE_NXT_OUT_DGST155_SHIFT                                             0
#define IPE_NXT_OUT_DGST155_MASK                                     0xffffffff
#define IPE_NXT_OUT_DGST155_RD(src)                      (((src) & 0xffffffff))
#define IPE_NXT_OUT_DGST155_WR(src)                 (((u32)(src)) & 0xffffffff)
#define IPE_NXT_OUT_DGST155_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_IN_DGST8	*/ 
/*	 Fields IPE_IN_DGST8	 */
#define IPE_IN_DGST88_WIDTH                                                  32
#define IPE_IN_DGST88_SHIFT                                                   0
#define IPE_IN_DGST88_MASK                                           0xffffffff
#define IPE_IN_DGST88_RD(src)                            (((src) & 0xffffffff))
#define IPE_IN_DGST88_WR(src)                       (((u32)(src)) & 0xffffffff)
#define IPE_IN_DGST88_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_IN_DGST9	*/ 
/*	 Fields IPE_IN_DGST9	 */
#define IPE_IN_DGST99_WIDTH                                                  32
#define IPE_IN_DGST99_SHIFT                                                   0
#define IPE_IN_DGST99_MASK                                           0xffffffff
#define IPE_IN_DGST99_RD(src)                            (((src) & 0xffffffff))
#define IPE_IN_DGST99_WR(src)                       (((u32)(src)) & 0xffffffff)
#define IPE_IN_DGST99_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_IN_DGST10	*/ 
/*	 Fields IPE_IN_DGST10	 */
#define IPE_IN_DGST100_WIDTH                                                 32
#define IPE_IN_DGST100_SHIFT                                                  0
#define IPE_IN_DGST100_MASK                                          0xffffffff
#define IPE_IN_DGST100_RD(src)                           (((src) & 0xffffffff))
#define IPE_IN_DGST100_WR(src)                      (((u32)(src)) & 0xffffffff)
#define IPE_IN_DGST100_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_IN_DGST11	*/ 
/*	 Fields IPE_IN_DGST11	 */
#define IPE_IN_DGST111_WIDTH                                                 32
#define IPE_IN_DGST111_SHIFT                                                  0
#define IPE_IN_DGST111_MASK                                          0xffffffff
#define IPE_IN_DGST111_RD(src)                           (((src) & 0xffffffff))
#define IPE_IN_DGST111_WR(src)                      (((u32)(src)) & 0xffffffff)
#define IPE_IN_DGST111_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_IN_DGST12	*/ 
/*	 Fields IPE_IN_DGST12	 */
#define IPE_IN_DGST122_WIDTH                                                 32
#define IPE_IN_DGST122_SHIFT                                                  0
#define IPE_IN_DGST122_MASK                                          0xffffffff
#define IPE_IN_DGST122_RD(src)                           (((src) & 0xffffffff))
#define IPE_IN_DGST122_WR(src)                      (((u32)(src)) & 0xffffffff)
#define IPE_IN_DGST122_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_IN_DGST13	*/ 
/*	 Fields IPE_IN_DGST13	 */
#define IPE_IN_DGST133_WIDTH                                                 32
#define IPE_IN_DGST133_SHIFT                                                  0
#define IPE_IN_DGST133_MASK                                          0xffffffff
#define IPE_IN_DGST133_RD(src)                           (((src) & 0xffffffff))
#define IPE_IN_DGST133_WR(src)                      (((u32)(src)) & 0xffffffff)
#define IPE_IN_DGST133_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_IN_DGST14	*/ 
/*	 Fields IPE_IN_DGST14	 */
#define IPE_IN_DGST144_WIDTH                                                 32
#define IPE_IN_DGST144_SHIFT                                                  0
#define IPE_IN_DGST144_MASK                                          0xffffffff
#define IPE_IN_DGST144_RD(src)                           (((src) & 0xffffffff))
#define IPE_IN_DGST144_WR(src)                      (((u32)(src)) & 0xffffffff)
#define IPE_IN_DGST144_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_IN_DGST15	*/ 
/*	 Fields IPE_IN_DGST15	 */
#define IPE_IN_DGST155_WIDTH                                                 32
#define IPE_IN_DGST155_SHIFT                                                  0
#define IPE_IN_DGST155_MASK                                          0xffffffff
#define IPE_IN_DGST155_RD(src)                           (((src) & 0xffffffff))
#define IPE_IN_DGST155_WR(src)                      (((u32)(src)) & 0xffffffff)
#define IPE_IN_DGST155_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_OUT_DGST8	*/ 
/*	 Fields IPE_OUT_DGST8	 */
#define IPE_OUT_DGST88_WIDTH                                                 32
#define IPE_OUT_DGST88_SHIFT                                                  0
#define IPE_OUT_DGST88_MASK                                          0xffffffff
#define IPE_OUT_DGST88_RD(src)                           (((src) & 0xffffffff))
#define IPE_OUT_DGST88_WR(src)                      (((u32)(src)) & 0xffffffff)
#define IPE_OUT_DGST88_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_OUT_DGST9	*/ 
/*	 Fields IPE_OUT_DGST9	 */
#define IPE_OUT_DGST99_WIDTH                                                 32
#define IPE_OUT_DGST99_SHIFT                                                  0
#define IPE_OUT_DGST99_MASK                                          0xffffffff
#define IPE_OUT_DGST99_RD(src)                           (((src) & 0xffffffff))
#define IPE_OUT_DGST99_WR(src)                      (((u32)(src)) & 0xffffffff)
#define IPE_OUT_DGST99_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_OUT_DGST10	*/ 
/*	 Fields IPE_OUT_DGST10	 */
#define IPE_OUT_DGST100_WIDTH                                                32
#define IPE_OUT_DGST100_SHIFT                                                 0
#define IPE_OUT_DGST100_MASK                                         0xffffffff
#define IPE_OUT_DGST100_RD(src)                          (((src) & 0xffffffff))
#define IPE_OUT_DGST100_WR(src)                     (((u32)(src)) & 0xffffffff)
#define IPE_OUT_DGST100_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_OUT_DGST11	*/ 
/*	 Fields IPE_OUT_DGST11	 */
#define IPE_OUT_DGST111_WIDTH                                                32
#define IPE_OUT_DGST111_SHIFT                                                 0
#define IPE_OUT_DGST111_MASK                                         0xffffffff
#define IPE_OUT_DGST111_RD(src)                          (((src) & 0xffffffff))
#define IPE_OUT_DGST111_WR(src)                     (((u32)(src)) & 0xffffffff)
#define IPE_OUT_DGST111_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_OUT_DGST12	*/ 
/*	 Fields IPE_OUT_DGST12	 */
#define IPE_OUT_DGST122_WIDTH                                                32
#define IPE_OUT_DGST122_SHIFT                                                 0
#define IPE_OUT_DGST122_MASK                                         0xffffffff
#define IPE_OUT_DGST122_RD(src)                          (((src) & 0xffffffff))
#define IPE_OUT_DGST122_WR(src)                     (((u32)(src)) & 0xffffffff)
#define IPE_OUT_DGST122_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_OUT_DGST13	*/ 
/*	 Fields IPE_OUT_DGST13	 */
#define IPE_OUT_DGST133_WIDTH                                                32
#define IPE_OUT_DGST133_SHIFT                                                 0
#define IPE_OUT_DGST133_MASK                                         0xffffffff
#define IPE_OUT_DGST133_RD(src)                          (((src) & 0xffffffff))
#define IPE_OUT_DGST133_WR(src)                     (((u32)(src)) & 0xffffffff)
#define IPE_OUT_DGST133_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_OUT_DGST14	*/ 
/*	 Fields IPE_OUT_DGST14	 */
#define IPE_OUT_DGST144_WIDTH                                                32
#define IPE_OUT_DGST144_SHIFT                                                 0
#define IPE_OUT_DGST144_MASK                                         0xffffffff
#define IPE_OUT_DGST144_RD(src)                          (((src) & 0xffffffff))
#define IPE_OUT_DGST144_WR(src)                     (((u32)(src)) & 0xffffffff)
#define IPE_OUT_DGST144_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_OUT_DGST15	*/ 
/*	 Fields IPE_OUT_DGST15	 */
#define IPE_OUT_DGST155_WIDTH                                                32
#define IPE_OUT_DGST155_SHIFT                                                 0
#define IPE_OUT_DGST155_MASK                                         0xffffffff
#define IPE_OUT_DGST155_RD(src)                          (((src) & 0xffffffff))
#define IPE_OUT_DGST155_WR(src)                     (((u32)(src)) & 0xffffffff)
#define IPE_OUT_DGST155_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_HASH_RES8	*/ 
/*	 Fields IPE_HASH_RES8	 */
#define IPE_HASH_RES88_WIDTH                                                 32
#define IPE_HASH_RES88_SHIFT                                                  0
#define IPE_HASH_RES88_MASK                                          0xffffffff
#define IPE_HASH_RES88_RD(src)                           (((src) & 0xffffffff))
#define IPE_HASH_RES88_WR(src)                      (((u32)(src)) & 0xffffffff)
#define IPE_HASH_RES88_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_HASH_RES9	*/ 
/*	 Fields IPE_HASH_RES9	 */
#define IPE_HASH_RES99_WIDTH                                                 32
#define IPE_HASH_RES99_SHIFT                                                  0
#define IPE_HASH_RES99_MASK                                          0xffffffff
#define IPE_HASH_RES99_RD(src)                           (((src) & 0xffffffff))
#define IPE_HASH_RES99_WR(src)                      (((u32)(src)) & 0xffffffff)
#define IPE_HASH_RES99_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_HASH_RES10	*/ 
/*	 Fields IPE_HASH_RES10	 */
#define IPE_HASH_RES100_WIDTH                                                32
#define IPE_HASH_RES100_SHIFT                                                 0
#define IPE_HASH_RES100_MASK                                         0xffffffff
#define IPE_HASH_RES100_RD(src)                          (((src) & 0xffffffff))
#define IPE_HASH_RES100_WR(src)                     (((u32)(src)) & 0xffffffff)
#define IPE_HASH_RES100_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_HASH_RES11	*/ 
/*	 Fields IPE_HASH_RES11	 */
#define IPE_HASH_RES111_WIDTH                                                32
#define IPE_HASH_RES111_SHIFT                                                 0
#define IPE_HASH_RES111_MASK                                         0xffffffff
#define IPE_HASH_RES111_RD(src)                          (((src) & 0xffffffff))
#define IPE_HASH_RES111_WR(src)                     (((u32)(src)) & 0xffffffff)
#define IPE_HASH_RES111_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_HASH_RES12	*/ 
/*	 Fields IPE_HASH_RES12	 */
#define IPE_HASH_RES122_WIDTH                                                32
#define IPE_HASH_RES122_SHIFT                                                 0
#define IPE_HASH_RES122_MASK                                         0xffffffff
#define IPE_HASH_RES122_RD(src)                          (((src) & 0xffffffff))
#define IPE_HASH_RES122_WR(src)                     (((u32)(src)) & 0xffffffff)
#define IPE_HASH_RES122_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_HASH_RES13	*/ 
/*	 Fields IPE_HASH_RES13	 */
#define IPE_HASH_RES133_WIDTH                                                32
#define IPE_HASH_RES133_SHIFT                                                 0
#define IPE_HASH_RES133_MASK                                         0xffffffff
#define IPE_HASH_RES133_RD(src)                          (((src) & 0xffffffff))
#define IPE_HASH_RES133_WR(src)                     (((u32)(src)) & 0xffffffff)
#define IPE_HASH_RES133_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_HASH_RES14	*/ 
/*	 Fields IPE_HASH_RES14	 */
#define IPE_HASH_RES144_WIDTH                                                32
#define IPE_HASH_RES144_SHIFT                                                 0
#define IPE_HASH_RES144_MASK                                         0xffffffff
#define IPE_HASH_RES144_RD(src)                          (((src) & 0xffffffff))
#define IPE_HASH_RES144_WR(src)                     (((u32)(src)) & 0xffffffff)
#define IPE_HASH_RES144_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register IPE_HASH_RES15	*/ 
/*	 Fields IPE_HASH_RES15	 */
#define IPE_HASH_RES155_WIDTH                                                32
#define IPE_HASH_RES155_SHIFT                                                 0
#define IPE_HASH_RES155_MASK                                         0xffffffff
#define IPE_HASH_RES155_RD(src)                          (((src) & 0xffffffff))
#define IPE_HASH_RES155_WR(src)                     (((u32)(src)) & 0xffffffff)
#define IPE_HASH_RES155_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Global Base Address	*/
#define SEC_GLBL_DIAG_CSR_BASE_ADDR			0xddd837000ULL

/*    Address GLBL_DIAG_CSR  Registers */
#define SEC_CFG_DIAG_SEL_ADDR                                        0x00000000
#define SEC_CFG_DIAG_SEL_DEFAULT                                     0x00000000
#define SEC_CFG_READ_BW_LAT_ADDR_MASK_ADDR                           0x00000004
#define SEC_CFG_READ_BW_LAT_ADDR_MASK_DEFAULT                        0x00000000
#define SEC_CFG_READ_BW_LAT_ADDR_PAT_ADDR                            0x00000008
#define SEC_CFG_READ_BW_LAT_ADDR_PAT_DEFAULT                         0xffffffff
#define SEC_CFG_WRITE_BW_LAT_ADDR_MASK_ADDR                          0x0000000c
#define SEC_CFG_WRITE_BW_LAT_ADDR_MASK_DEFAULT                       0x00000000
#define SEC_CFG_WRITE_BW_LAT_ADDR_PAT_ADDR                           0x00000010
#define SEC_CFG_WRITE_BW_LAT_ADDR_PAT_DEFAULT                        0xffffffff
#define SEC_CFG_DIAG_START_STOP_ADDR                                 0x00000014
#define SEC_CFG_DIAG_START_STOP_DEFAULT                              0x0000001f
#define SEC_CFG_BW_MSTR_STOP_CNT_ADDR                                0x00000018
#define SEC_CFG_BW_MSTR_STOP_CNT_DEFAULT                             0x00040004
#define SEC_CFG_BW_SLV_STOP_CNT_ADDR                                 0x0000001c
#define SEC_CFG_BW_SLV_STOP_CNT_DEFAULT                              0x00040004
#define SEC_STS_READ_LATENCY_OUTPUT_ADDR                             0x00000020
#define SEC_STS_READ_LATENCY_OUTPUT_DEFAULT                          0x00000000
#define SEC_STS_AXI_MRD_BW_CLK_CNT_ADDR                              0x00000024
#define SEC_STS_AXI_MRD_BW_CLK_CNT_DEFAULT                           0x00000000
#define SEC_STS_AXI_MRD_BW_BYTE_CNT_ADDR                             0x00000028
#define SEC_STS_AXI_MRD_BW_BYTE_CNT_DEFAULT                          0x00000000
#define SEC_STS_AXI_MWR_BW_CLK_CNT_ADDR                              0x0000002c
#define SEC_STS_AXI_MWR_BW_CLK_CNT_DEFAULT                           0x00000000
#define SEC_STS_AXI_MWR_BW_BYTE_CNT_ADDR                             0x00000030
#define SEC_STS_AXI_MWR_BW_BYTE_CNT_DEFAULT                          0x00000000
#define SEC_STS_AXI_SRD_BW_CLK_CNT_ADDR                              0x00000034
#define SEC_STS_AXI_SRD_BW_CLK_CNT_DEFAULT                           0x00000000
#define SEC_STS_AXI_SRD_BW_BYTE_CNT_ADDR                             0x00000038
#define SEC_STS_AXI_SRD_BW_BYTE_CNT_DEFAULT                          0x00000000
#define SEC_STS_AXI_SWR_BW_CLK_CNT_ADDR                              0x0000003c
#define SEC_STS_AXI_SWR_BW_CLK_CNT_DEFAULT                           0x00000000
#define SEC_STS_AXI_SWR_BW_BYTE_CNT_ADDR                             0x00000040
#define SEC_STS_AXI_SWR_BW_BYTE_CNT_DEFAULT                          0x00000000
#define SEC_CFG_DBG_TRIG_CTRL_ADDR                                   0x00000044
#define SEC_CFG_DBG_TRIG_CTRL_DEFAULT                                0x00000000
#define SEC_CFG_DBG_PAT_REG_0_ADDR                                   0x00000048
#define SEC_CFG_DBG_PAT_REG_0_DEFAULT                                0x00000000
#define SEC_CFG_DBG_PAT_MASK_REG_0_ADDR                              0x0000004c
#define SEC_CFG_DBG_PAT_MASK_REG_0_DEFAULT                           0x00000000
#define SEC_CFG_DBG_PAT_REG_1_ADDR                                   0x00000050
#define SEC_CFG_DBG_PAT_REG_1_DEFAULT                                0x00000000
#define SEC_CFG_DBG_PAT_MASK_REG_1_ADDR                              0x00000054
#define SEC_CFG_DBG_PAT_MASK_REG_1_DEFAULT                           0x00000000
#define SEC_DBG_TRIG_OUT_ADDR                                        0x00000058
#define SEC_DBG_TRIG_OUT_DEFAULT                                     0x00000000
#define SEC_DBG_ECC_INT_OUT_ADDR                                     0x0000005c
#define SEC_DBG_ECC_INT_OUT_DEFAULT                                  0x00000000
#define SEC_DBG_ECC_INT_OUTMASK_ADDR                                 0x00000060
#define SEC_CFG_SECC_INT_THRSH_REG_ADDR                              0x00000064
#define SEC_CFG_SECC_INT_THRSH_REG_DEFAULT                           0x000000fe
#define SEC_CFG_MEM_ECC_BYPASS_ADDR                                  0x00000068
#define SEC_CFG_MEM_ECC_BYPASS_DEFAULT                               0x00000000
#define SEC_CFG_MEM_PWRDN_DIS_ADDR                                   0x0000006c
#define SEC_CFG_MEM_PWRDN_DIS_DEFAULT                                0x00000000
#define SEC_CFG_MEM_RAM_SHUTDOWN_ADDR                                0x00000070
#define SEC_CFG_MEM_RAM_SHUTDOWN_DEFAULT                             0xffffffff
#define SEC_BLOCK_MEM_RDY_ADDR                                       0x00000074
#define SEC_BLOCK_MEM_RDY_DEFAULT                                    0xffffffff
#define SEC_CFG_AXI_SLV_RD_TMO_ADDR                                  0x00000078
#define SEC_CFG_AXI_SLV_RD_TMO_DEFAULT                               0x83ff83ff
#define SEC_CFG_AXI_SLV_WR_TMO_ADDR                                  0x0000007c
#define SEC_CFG_AXI_SLV_WR_TMO_DEFAULT                               0x83ff83ff
#define SEC_STS_AXI_SLV_TMO_ADDR                                     0x00000080
#define SEC_STS_AXI_SLV_TMO_DEFAULT                                  0x00000000
#define SEC_CFG_AXI_SS_CSR_TMO_ADDR                                  0x00000084
#define SEC_CFG_AXI_SS_CSR_TMO_DEFAULT                               0x02008000
#define SEC_STS_READ_LATENCY_TOT_READ_REQS_ADDR                      0x0000008c
#define SEC_STS_READ_LATENCY_TOT_READ_REQS_DEFAULT                   0x00000000

/*	Register CFG_DIAG_SEL	*/ 
/*	 Fields CFG_AXI_NON_AXI_MUX_SELECT	 */
#define SEC_CFG_AXI_NON_AXI_MUX_SELECT_WIDTH                                  1
#define SEC_CFG_AXI_NON_AXI_MUX_SELECT_SHIFT                                 11
#define SEC_CFG_AXI_NON_AXI_MUX_SELECT_MASK                          0x00000800
#define SEC_CFG_AXI_NON_AXI_MUX_SELECT_RD(src)       (((src) & 0x00000800)>>11)
#define SEC_CFG_AXI_NON_AXI_MUX_SELECT_WR(src)  (((u32)(src)<<11) & 0x00000800)
#define SEC_CFG_AXI_NON_AXI_MUX_SELECT_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*	 Fields CFG_MUX_SELECTOR	 */
#define SEC_CFG_MUX_SELECTOR_WIDTH                                           11
#define SEC_CFG_MUX_SELECTOR_SHIFT                                            0
#define SEC_CFG_MUX_SELECTOR_MASK                                    0x000007ff
#define SEC_CFG_MUX_SELECTOR_RD(src)                     (((src) & 0x000007ff))
#define SEC_CFG_MUX_SELECTOR_WR(src)                (((u32)(src)) & 0x000007ff)
#define SEC_CFG_MUX_SELECTOR_SET(dst,src) \
                          (((dst) & ~0x000007ff) | (((u32)(src)) & 0x000007ff))

/*	Register CFG_READ_BW_LAT_ADDR_MASK	*/ 
/*	 Fields READ_ADDR_MASK	 */
#define SEC_READ_ADDR_MASK_WIDTH                                             32
#define SEC_READ_ADDR_MASK_SHIFT                                              0
#define SEC_READ_ADDR_MASK_MASK                                      0xffffffff
#define SEC_READ_ADDR_MASK_RD(src)                       (((src) & 0xffffffff))
#define SEC_READ_ADDR_MASK_WR(src)                  (((u32)(src)) & 0xffffffff)
#define SEC_READ_ADDR_MASK_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_READ_BW_LAT_ADDR_PAT	*/ 
/*	 Fields READ_ADDR_PAT	 */
#define SEC_READ_ADDR_PAT_WIDTH                                              32
#define SEC_READ_ADDR_PAT_SHIFT                                               0
#define SEC_READ_ADDR_PAT_MASK                                       0xffffffff
#define SEC_READ_ADDR_PAT_RD(src)                        (((src) & 0xffffffff))
#define SEC_READ_ADDR_PAT_WR(src)                   (((u32)(src)) & 0xffffffff)
#define SEC_READ_ADDR_PAT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_WRITE_BW_LAT_ADDR_MASK	*/ 
/*	 Fields WRITE_ADDR_MASK	 */
#define SEC_WRITE_ADDR_MASK_WIDTH                                            32
#define SEC_WRITE_ADDR_MASK_SHIFT                                             0
#define SEC_WRITE_ADDR_MASK_MASK                                     0xffffffff
#define SEC_WRITE_ADDR_MASK_RD(src)                      (((src) & 0xffffffff))
#define SEC_WRITE_ADDR_MASK_WR(src)                 (((u32)(src)) & 0xffffffff)
#define SEC_WRITE_ADDR_MASK_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_WRITE_BW_LAT_ADDR_PAT	*/ 
/*	 Fields WRITE_ADDR_PAT	 */
#define SEC_WRITE_ADDR_PAT_WIDTH                                             32
#define SEC_WRITE_ADDR_PAT_SHIFT                                              0
#define SEC_WRITE_ADDR_PAT_MASK                                      0xffffffff
#define SEC_WRITE_ADDR_PAT_RD(src)                       (((src) & 0xffffffff))
#define SEC_WRITE_ADDR_PAT_WR(src)                  (((u32)(src)) & 0xffffffff)
#define SEC_WRITE_ADDR_PAT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_DIAG_START_STOP	*/ 
/*	 Fields START_RD_LATENCY	 */
#define SEC_START_RD_LATENCY_WIDTH                                            1
#define SEC_START_RD_LATENCY_SHIFT                                            4
#define SEC_START_RD_LATENCY_MASK                                    0x00000010
#define SEC_START_RD_LATENCY_RD(src)                  (((src) & 0x00000010)>>4)
#define SEC_START_RD_LATENCY_WR(src)             (((u32)(src)<<4) & 0x00000010)
#define SEC_START_RD_LATENCY_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields START_SRD_BW	 */
#define SEC_START_SRD_BW_WIDTH                                                1
#define SEC_START_SRD_BW_SHIFT                                                3
#define SEC_START_SRD_BW_MASK                                        0x00000008
#define SEC_START_SRD_BW_RD(src)                      (((src) & 0x00000008)>>3)
#define SEC_START_SRD_BW_WR(src)                 (((u32)(src)<<3) & 0x00000008)
#define SEC_START_SRD_BW_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields START_MRD_BW	 */
#define SEC_START_MRD_BW_WIDTH                                                1
#define SEC_START_MRD_BW_SHIFT                                                2
#define SEC_START_MRD_BW_MASK                                        0x00000004
#define SEC_START_MRD_BW_RD(src)                      (((src) & 0x00000004)>>2)
#define SEC_START_MRD_BW_WR(src)                 (((u32)(src)<<2) & 0x00000004)
#define SEC_START_MRD_BW_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields START_SWR_BW	 */
#define SEC_START_SWR_BW_WIDTH                                                1
#define SEC_START_SWR_BW_SHIFT                                                1
#define SEC_START_SWR_BW_MASK                                        0x00000002
#define SEC_START_SWR_BW_RD(src)                      (((src) & 0x00000002)>>1)
#define SEC_START_SWR_BW_WR(src)                 (((u32)(src)<<1) & 0x00000002)
#define SEC_START_SWR_BW_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields START_MWR_BW	 */
#define SEC_START_MWR_BW_WIDTH                                                1
#define SEC_START_MWR_BW_SHIFT                                                0
#define SEC_START_MWR_BW_MASK                                        0x00000001
#define SEC_START_MWR_BW_RD(src)                         (((src) & 0x00000001))
#define SEC_START_MWR_BW_WR(src)                    (((u32)(src)) & 0x00000001)
#define SEC_START_MWR_BW_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register CFG_BW_MSTR_STOP_CNT	*/ 
/*	 Fields MSTR_STOP_RD_CNT	 */
#define SEC_MSTR_STOP_RD_CNT_WIDTH                                           16
#define SEC_MSTR_STOP_RD_CNT_SHIFT                                           16
#define SEC_MSTR_STOP_RD_CNT_MASK                                    0xffff0000
#define SEC_MSTR_STOP_RD_CNT_RD(src)                 (((src) & 0xffff0000)>>16)
#define SEC_MSTR_STOP_RD_CNT_WR(src)            (((u32)(src)<<16) & 0xffff0000)
#define SEC_MSTR_STOP_RD_CNT_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields MSTR_STOP_WR_CNT	 */
#define SEC_MSTR_STOP_WR_CNT_WIDTH                                           16
#define SEC_MSTR_STOP_WR_CNT_SHIFT                                            0
#define SEC_MSTR_STOP_WR_CNT_MASK                                    0x0000ffff
#define SEC_MSTR_STOP_WR_CNT_RD(src)                     (((src) & 0x0000ffff))
#define SEC_MSTR_STOP_WR_CNT_WR(src)                (((u32)(src)) & 0x0000ffff)
#define SEC_MSTR_STOP_WR_CNT_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register CFG_BW_SLV_STOP_CNT	*/ 
/*	 Fields SLV_STOP_RD_CNT	 */
#define SEC_SLV_STOP_RD_CNT_WIDTH                                            16
#define SEC_SLV_STOP_RD_CNT_SHIFT                                            16
#define SEC_SLV_STOP_RD_CNT_MASK                                     0xffff0000
#define SEC_SLV_STOP_RD_CNT_RD(src)                  (((src) & 0xffff0000)>>16)
#define SEC_SLV_STOP_RD_CNT_WR(src)             (((u32)(src)<<16) & 0xffff0000)
#define SEC_SLV_STOP_RD_CNT_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields SLV_STOP_WR_CNT	 */
#define SEC_SLV_STOP_WR_CNT_WIDTH                                            16
#define SEC_SLV_STOP_WR_CNT_SHIFT                                             0
#define SEC_SLV_STOP_WR_CNT_MASK                                     0x0000ffff
#define SEC_SLV_STOP_WR_CNT_RD(src)                      (((src) & 0x0000ffff))
#define SEC_SLV_STOP_WR_CNT_WR(src)                 (((u32)(src)) & 0x0000ffff)
#define SEC_SLV_STOP_WR_CNT_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register STS_READ_LATENCY_OUTPUT	*/ 
/*	 Fields READ_MAX	 */
#define SEC_READ_MAX_WIDTH                                                   10
#define SEC_READ_MAX_SHIFT                                                   22
#define SEC_READ_MAX_MASK                                            0xffc00000
#define SEC_READ_MAX_RD(src)                         (((src) & 0xffc00000)>>22)
#define SEC_READ_MAX_SET(dst,src) \
                      (((dst) & ~0xffc00000) | (((u32)(src)<<22) & 0xffc00000))
/*	 Fields READ_TOT	 */
#define SEC_READ_TOT_WIDTH                                                   22
#define SEC_READ_TOT_SHIFT                                                    0
#define SEC_READ_TOT_MASK                                            0x003fffff
#define SEC_READ_TOT_RD(src)                             (((src) & 0x003fffff))
#define SEC_READ_TOT_SET(dst,src) \
                          (((dst) & ~0x003fffff) | (((u32)(src)) & 0x003fffff))

/*	Register STS_AXI_MRD_BW_CLK_CNT	*/ 
/*	 Fields MSTR_READ_BW_CLK_CNT	 */
#define SEC_MSTR_READ_BW_CLK_CNT_WIDTH                                       32
#define SEC_MSTR_READ_BW_CLK_CNT_SHIFT                                        0
#define SEC_MSTR_READ_BW_CLK_CNT_MASK                                0xffffffff
#define SEC_MSTR_READ_BW_CLK_CNT_RD(src)                 (((src) & 0xffffffff))
#define SEC_MSTR_READ_BW_CLK_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register STS_AXI_MRD_BW_BYTE_CNT	*/ 
/*	 Fields MSTR_READ_BW_BYTE_CNT	 */
#define SEC_MSTR_READ_BW_BYTE_CNT_WIDTH                                      32
#define SEC_MSTR_READ_BW_BYTE_CNT_SHIFT                                       0
#define SEC_MSTR_READ_BW_BYTE_CNT_MASK                               0xffffffff
#define SEC_MSTR_READ_BW_BYTE_CNT_RD(src)                (((src) & 0xffffffff))
#define SEC_MSTR_READ_BW_BYTE_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register STS_AXI_MWR_BW_CLK_CNT	*/ 
/*	 Fields MSTR_WRITE_BW_CLK_CNT	 */
#define SEC_MSTR_WRITE_BW_CLK_CNT_WIDTH                                      32
#define SEC_MSTR_WRITE_BW_CLK_CNT_SHIFT                                       0
#define SEC_MSTR_WRITE_BW_CLK_CNT_MASK                               0xffffffff
#define SEC_MSTR_WRITE_BW_CLK_CNT_RD(src)                (((src) & 0xffffffff))
#define SEC_MSTR_WRITE_BW_CLK_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register STS_AXI_MWR_BW_BYTE_CNT	*/ 
/*	 Fields MSTR_WRITE_BW_BYTE_CNT	 */
#define SEC_MSTR_WRITE_BW_BYTE_CNT_WIDTH                                     32
#define SEC_MSTR_WRITE_BW_BYTE_CNT_SHIFT                                      0
#define SEC_MSTR_WRITE_BW_BYTE_CNT_MASK                              0xffffffff
#define SEC_MSTR_WRITE_BW_BYTE_CNT_RD(src)               (((src) & 0xffffffff))
#define SEC_MSTR_WRITE_BW_BYTE_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register STS_AXI_SRD_BW_CLK_CNT	*/ 
/*	 Fields SLV_READ_BW_CLK_CNT	 */
#define SEC_SLV_READ_BW_CLK_CNT_WIDTH                                        32
#define SEC_SLV_READ_BW_CLK_CNT_SHIFT                                         0
#define SEC_SLV_READ_BW_CLK_CNT_MASK                                 0xffffffff
#define SEC_SLV_READ_BW_CLK_CNT_RD(src)                  (((src) & 0xffffffff))
#define SEC_SLV_READ_BW_CLK_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register STS_AXI_SRD_BW_BYTE_CNT	*/ 
/*	 Fields SLV_READ_BW_BYTE_CNT	 */
#define SEC_SLV_READ_BW_BYTE_CNT_WIDTH                                       32
#define SEC_SLV_READ_BW_BYTE_CNT_SHIFT                                        0
#define SEC_SLV_READ_BW_BYTE_CNT_MASK                                0xffffffff
#define SEC_SLV_READ_BW_BYTE_CNT_RD(src)                 (((src) & 0xffffffff))
#define SEC_SLV_READ_BW_BYTE_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register STS_AXI_SWR_BW_CLK_CNT	*/ 
/*	 Fields SLV_WRITE_BW_CLK_CNT	 */
#define SEC_SLV_WRITE_BW_CLK_CNT_WIDTH                                       32
#define SEC_SLV_WRITE_BW_CLK_CNT_SHIFT                                        0
#define SEC_SLV_WRITE_BW_CLK_CNT_MASK                                0xffffffff
#define SEC_SLV_WRITE_BW_CLK_CNT_RD(src)                 (((src) & 0xffffffff))
#define SEC_SLV_WRITE_BW_CLK_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register STS_AXI_SWR_BW_BYTE_CNT	*/ 
/*	 Fields SLV_WRITE_BW_BYTE_CNT	 */
#define SEC_SLV_WRITE_BW_BYTE_CNT_WIDTH                                      32
#define SEC_SLV_WRITE_BW_BYTE_CNT_SHIFT                                       0
#define SEC_SLV_WRITE_BW_BYTE_CNT_MASK                               0xffffffff
#define SEC_SLV_WRITE_BW_BYTE_CNT_RD(src)                (((src) & 0xffffffff))
#define SEC_SLV_WRITE_BW_BYTE_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_DBG_TRIG_CTRL	*/ 
/*	 Fields TRIG_EN	 */
#define SEC_TRIG_EN_WIDTH                                                     1
#define SEC_TRIG_EN_SHIFT                                                     4
#define SEC_TRIG_EN_MASK                                             0x00000010
#define SEC_TRIG_EN_RD(src)                           (((src) & 0x00000010)>>4)
#define SEC_TRIG_EN_WR(src)                      (((u32)(src)<<4) & 0x00000010)
#define SEC_TRIG_EN_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields AND_E	 */
#define SEC_AND_E_WIDTH                                                       2
#define SEC_AND_E_SHIFT                                                       2
#define SEC_AND_E_MASK                                               0x0000000c
#define SEC_AND_E_RD(src)                             (((src) & 0x0000000c)>>2)
#define SEC_AND_E_WR(src)                        (((u32)(src)<<2) & 0x0000000c)
#define SEC_AND_E_SET(dst,src) \
                       (((dst) & ~0x0000000c) | (((u32)(src)<<2) & 0x0000000c))
/*	 Fields OR_E	 */
#define SEC_OR_E_WIDTH                                                        2
#define SEC_OR_E_SHIFT                                                        0
#define SEC_OR_E_MASK                                                0x00000003
#define SEC_OR_E_RD(src)                                 (((src) & 0x00000003))
#define SEC_OR_E_WR(src)                            (((u32)(src)) & 0x00000003)
#define SEC_OR_E_SET(dst,src) \
                          (((dst) & ~0x00000003) | (((u32)(src)) & 0x00000003))

/*	Register CFG_DBG_PAT_REG_0	*/ 
/*	 Fields PATTERN	 */
#define SEC_PATTERN0_WIDTH                                                   32
#define SEC_PATTERN0_SHIFT                                                    0
#define SEC_PATTERN0_MASK                                            0xffffffff
#define SEC_PATTERN0_RD(src)                             (((src) & 0xffffffff))
#define SEC_PATTERN0_WR(src)                        (((u32)(src)) & 0xffffffff)
#define SEC_PATTERN0_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_DBG_PAT_MASK_REG_0	*/ 
/*	 Fields PAT_MASK	 */
#define SEC_PAT_MASK0_WIDTH                                                  32
#define SEC_PAT_MASK0_SHIFT                                                   0
#define SEC_PAT_MASK0_MASK                                           0xffffffff
#define SEC_PAT_MASK0_RD(src)                            (((src) & 0xffffffff))
#define SEC_PAT_MASK0_WR(src)                       (((u32)(src)) & 0xffffffff)
#define SEC_PAT_MASK0_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_DBG_PAT_REG_1	*/ 
/*	 Fields PATTERN	 */
#define SEC_PATTERN1_WIDTH                                                   32
#define SEC_PATTERN1_SHIFT                                                    0
#define SEC_PATTERN1_MASK                                            0xffffffff
#define SEC_PATTERN1_RD(src)                             (((src) & 0xffffffff))
#define SEC_PATTERN1_WR(src)                        (((u32)(src)) & 0xffffffff)
#define SEC_PATTERN1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_DBG_PAT_MASK_REG_1	*/ 
/*	 Fields PAT_MASK	 */
#define SEC_PAT_MASK1_WIDTH                                                  32
#define SEC_PAT_MASK1_SHIFT                                                   0
#define SEC_PAT_MASK1_MASK                                           0xffffffff
#define SEC_PAT_MASK1_RD(src)                            (((src) & 0xffffffff))
#define SEC_PAT_MASK1_WR(src)                       (((u32)(src)) & 0xffffffff)
#define SEC_PAT_MASK1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register DBG_TRIG_OUT	*/ 
/*	 Fields DBG_OUT	 */
#define SEC_DBG_OUT_WIDTH                                                    32
#define SEC_DBG_OUT_SHIFT                                                     0
#define SEC_DBG_OUT_MASK                                             0xffffffff
#define SEC_DBG_OUT_RD(src)                              (((src) & 0xffffffff))
#define SEC_DBG_OUT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register DBG_ECC_INT_OUT	*/ 
/*	 Fields SLV_TMO	 */
#define SEC_SLV_TMO_WIDTH                                                     1
#define SEC_SLV_TMO_SHIFT                                                     3
#define SEC_SLV_TMO_MASK                                             0x00000008
#define SEC_SLV_TMO_RD(src)                           (((src) & 0x00000008)>>3)
#define SEC_SLV_TMO_WR(src)                      (((u32)(src)<<3) & 0x00000008)
#define SEC_SLV_TMO_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields SECC_INT	 */
#define SEC_SECC_INT_WIDTH                                                    1
#define SEC_SECC_INT_SHIFT                                                    2
#define SEC_SECC_INT_MASK                                            0x00000004
#define SEC_SECC_INT_RD(src)                          (((src) & 0x00000004)>>2)
#define SEC_SECC_INT_WR(src)                     (((u32)(src)<<2) & 0x00000004)
#define SEC_SECC_INT_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields DECC_INT	 */
#define SEC_DECC_INT_WIDTH                                                    1
#define SEC_DECC_INT_SHIFT                                                    1
#define SEC_DECC_INT_MASK                                            0x00000002
#define SEC_DECC_INT_RD(src)                          (((src) & 0x00000002)>>1)
#define SEC_DECC_INT_WR(src)                     (((u32)(src)<<1) & 0x00000002)
#define SEC_DECC_INT_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields DBG_INT	 */
#define SEC_DBG_INT_WIDTH                                                     1
#define SEC_DBG_INT_SHIFT                                                     0
#define SEC_DBG_INT_MASK                                             0x00000001
#define SEC_DBG_INT_RD(src)                              (((src) & 0x00000001))
#define SEC_DBG_INT_WR(src)                         (((u32)(src)) & 0x00000001)
#define SEC_DBG_INT_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register DBG_ECC_INT_OUTMask	*/
/*    Mask Register Fields SLV_TMOMask    */
#define SEC_SLV_TMOMASK_WIDTH                                                 1
#define SEC_SLV_TMOMASK_SHIFT                                                 3
#define SEC_SLV_TMOMASK_MASK                                         0x00000008
#define SEC_SLV_TMOMASK_RD(src)                       (((src) & 0x00000008)>>3)
#define SEC_SLV_TMOMASK_WR(src)                  (((u32)(src)<<3) & 0x00000008)
#define SEC_SLV_TMOMASK_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*    Mask Register Fields SECC_INTMask    */
#define SEC_SECC_INTMASK_WIDTH                                                1
#define SEC_SECC_INTMASK_SHIFT                                                2
#define SEC_SECC_INTMASK_MASK                                        0x00000004
#define SEC_SECC_INTMASK_RD(src)                      (((src) & 0x00000004)>>2)
#define SEC_SECC_INTMASK_WR(src)                 (((u32)(src)<<2) & 0x00000004)
#define SEC_SECC_INTMASK_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*    Mask Register Fields DECC_INTMask    */
#define SEC_DECC_INTMASK_WIDTH                                                1
#define SEC_DECC_INTMASK_SHIFT                                                1
#define SEC_DECC_INTMASK_MASK                                        0x00000002
#define SEC_DECC_INTMASK_RD(src)                      (((src) & 0x00000002)>>1)
#define SEC_DECC_INTMASK_WR(src)                 (((u32)(src)<<1) & 0x00000002)
#define SEC_DECC_INTMASK_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*    Mask Register Fields DBG_INTMask    */
#define SEC_DBG_INTMASK_WIDTH                                                 1
#define SEC_DBG_INTMASK_SHIFT                                                 0
#define SEC_DBG_INTMASK_MASK                                         0x00000001
#define SEC_DBG_INTMASK_RD(src)                          (((src) & 0x00000001))
#define SEC_DBG_INTMASK_WR(src)                     (((u32)(src)) & 0x00000001)
#define SEC_DBG_INTMASK_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register CFG_SECC_INT_THRSH_REG	*/ 
/*	 Fields SECC_INT_THRSH	 */
#define SEC_SECC_INT_THRSH_WIDTH                                              8
#define SEC_SECC_INT_THRSH_SHIFT                                              0
#define SEC_SECC_INT_THRSH_MASK                                      0x000000ff
#define SEC_SECC_INT_THRSH_RD(src)                       (((src) & 0x000000ff))
#define SEC_SECC_INT_THRSH_WR(src)                  (((u32)(src)) & 0x000000ff)
#define SEC_SECC_INT_THRSH_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register CFG_MEM_ECC_BYPASS	*/ 
/*	 Fields CFG_ECC_BYPASS	 */
#define SEC_CFG_ECC_BYPASS_WIDTH                                             16
#define SEC_CFG_ECC_BYPASS_SHIFT                                              0
#define SEC_CFG_ECC_BYPASS_MASK                                      0x0000ffff
#define SEC_CFG_ECC_BYPASS_RD(src)                       (((src) & 0x0000ffff))
#define SEC_CFG_ECC_BYPASS_WR(src)                  (((u32)(src)) & 0x0000ffff)
#define SEC_CFG_ECC_BYPASS_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register CFG_MEM_PWRDN_DIS	*/ 
/*	 Fields CFG_ECC_PWRDN_DIS	 */
#define SEC_CFG_ECC_PWRDN_DIS_WIDTH                                          16
#define SEC_CFG_ECC_PWRDN_DIS_SHIFT                                          16
#define SEC_CFG_ECC_PWRDN_DIS_MASK                                   0xffff0000
#define SEC_CFG_ECC_PWRDN_DIS_RD(src)                (((src) & 0xffff0000)>>16)
#define SEC_CFG_ECC_PWRDN_DIS_WR(src)           (((u32)(src)<<16) & 0xffff0000)
#define SEC_CFG_ECC_PWRDN_DIS_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields CFG_PWRDN_DIS	 */
#define SEC_CFG_PWRDN_DIS_WIDTH                                              16
#define SEC_CFG_PWRDN_DIS_SHIFT                                               0
#define SEC_CFG_PWRDN_DIS_MASK                                       0x0000ffff
#define SEC_CFG_PWRDN_DIS_RD(src)                        (((src) & 0x0000ffff))
#define SEC_CFG_PWRDN_DIS_WR(src)                   (((u32)(src)) & 0x0000ffff)
#define SEC_CFG_PWRDN_DIS_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register CFG_MEM_RAM_SHUTDOWN	*/ 
/*	 Fields CFG_RAM_SHUTDOWN_EN	 */
#define SEC_CFG_RAM_SHUTDOWN_EN_WIDTH                                        32
#define SEC_CFG_RAM_SHUTDOWN_EN_SHIFT                                         0
#define SEC_CFG_RAM_SHUTDOWN_EN_MASK                                 0xffffffff
#define SEC_CFG_RAM_SHUTDOWN_EN_RD(src)                  (((src) & 0xffffffff))
#define SEC_CFG_RAM_SHUTDOWN_EN_WR(src)             (((u32)(src)) & 0xffffffff)
#define SEC_CFG_RAM_SHUTDOWN_EN_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register BLOCK_MEM_RDY	*/ 
/*	 Fields MEM_RDY	 */
#define SEC_MEM_RDY_WIDTH                                                    32
#define SEC_MEM_RDY_SHIFT                                                     0
#define SEC_MEM_RDY_MASK                                             0xffffffff
#define SEC_MEM_RDY_RD(src)                              (((src) & 0xffffffff))
#define SEC_MEM_RDY_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_AXI_SLV_RD_TMO	*/ 
/*	 Fields SLV_ID_TMO_ENABLE	 */
#define SEC_SLV_ID_TMO_ENABLE_WIDTH                                           1
#define SEC_SLV_ID_TMO_ENABLE_SHIFT                                          31
#define SEC_SLV_ID_TMO_ENABLE_MASK                                   0x80000000
#define SEC_SLV_ID_TMO_ENABLE_RD(src)                (((src) & 0x80000000)>>31)
#define SEC_SLV_ID_TMO_ENABLE_WR(src)           (((u32)(src)<<31) & 0x80000000)
#define SEC_SLV_ID_TMO_ENABLE_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields SS_ID_TRK_ENABLE	 */
#define SEC_SS_ID_TRK_ENABLE_WIDTH                                            1
#define SEC_SS_ID_TRK_ENABLE_SHIFT                                           30
#define SEC_SS_ID_TRK_ENABLE_MASK                                    0x40000000
#define SEC_SS_ID_TRK_ENABLE_RD(src)                 (((src) & 0x40000000)>>30)
#define SEC_SS_ID_TRK_ENABLE_WR(src)            (((u32)(src)<<30) & 0x40000000)
#define SEC_SS_ID_TRK_ENABLE_SET(dst,src) \
                      (((dst) & ~0x40000000) | (((u32)(src)<<30) & 0x40000000))
/*	 Fields SLV_ID_TMO_VALUE	 */
#define SEC_SLV_ID_TMO_VALUE_WIDTH                                           10
#define SEC_SLV_ID_TMO_VALUE_SHIFT                                           16
#define SEC_SLV_ID_TMO_VALUE_MASK                                    0x03ff0000
#define SEC_SLV_ID_TMO_VALUE_RD(src)                 (((src) & 0x03ff0000)>>16)
#define SEC_SLV_ID_TMO_VALUE_WR(src)            (((u32)(src)<<16) & 0x03ff0000)
#define SEC_SLV_ID_TMO_VALUE_SET(dst,src) \
                      (((dst) & ~0x03ff0000) | (((u32)(src)<<16) & 0x03ff0000))
/*	 Fields SLV_RDY_TMO_ENABLE	 */
#define SEC_SLV_RDY_TMO_ENABLE_WIDTH                                          1
#define SEC_SLV_RDY_TMO_ENABLE_SHIFT                                         15
#define SEC_SLV_RDY_TMO_ENABLE_MASK                                  0x00008000
#define SEC_SLV_RDY_TMO_ENABLE_RD(src)               (((src) & 0x00008000)>>15)
#define SEC_SLV_RDY_TMO_ENABLE_WR(src)          (((u32)(src)<<15) & 0x00008000)
#define SEC_SLV_RDY_TMO_ENABLE_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))
/*	 Fields SLV_RDY_TMO_VALUE	 */
#define SEC_SLV_RDY_TMO_VALUE_WIDTH                                          10
#define SEC_SLV_RDY_TMO_VALUE_SHIFT                                           0
#define SEC_SLV_RDY_TMO_VALUE_MASK                                   0x000003ff
#define SEC_SLV_RDY_TMO_VALUE_RD(src)                    (((src) & 0x000003ff))
#define SEC_SLV_RDY_TMO_VALUE_WR(src)               (((u32)(src)) & 0x000003ff)
#define SEC_SLV_RDY_TMO_VALUE_SET(dst,src) \
                          (((dst) & ~0x000003ff) | (((u32)(src)) & 0x000003ff))

/*	Register CFG_AXI_SLV_WR_TMO	*/ 
/*	 Fields SLV_ID_TMO_ENABLE	 */
#define SEC_SLV_ID_TMO_ENABLE_F1_WIDTH                                        1
#define SEC_SLV_ID_TMO_ENABLE_F1_SHIFT                                       31
#define SEC_SLV_ID_TMO_ENABLE_F1_MASK                                0x80000000
#define SEC_SLV_ID_TMO_ENABLE_F1_RD(src)             (((src) & 0x80000000)>>31)
#define SEC_SLV_ID_TMO_ENABLE_F1_WR(src)        (((u32)(src)<<31) & 0x80000000)
#define SEC_SLV_ID_TMO_ENABLE_F1_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields SS_ID_TRK_ENABLE	 */
#define SEC_SS_ID_TRK_ENABLE_F1_WIDTH                                         1
#define SEC_SS_ID_TRK_ENABLE_F1_SHIFT                                        30
#define SEC_SS_ID_TRK_ENABLE_F1_MASK                                 0x40000000
#define SEC_SS_ID_TRK_ENABLE_F1_RD(src)              (((src) & 0x40000000)>>30)
#define SEC_SS_ID_TRK_ENABLE_F1_WR(src)         (((u32)(src)<<30) & 0x40000000)
#define SEC_SS_ID_TRK_ENABLE_F1_SET(dst,src) \
                      (((dst) & ~0x40000000) | (((u32)(src)<<30) & 0x40000000))
/*	 Fields SLV_ID_TMO_VALUE	 */
#define SEC_SLV_ID_TMO_VALUE_F1_WIDTH                                        10
#define SEC_SLV_ID_TMO_VALUE_F1_SHIFT                                        16
#define SEC_SLV_ID_TMO_VALUE_F1_MASK                                 0x03ff0000
#define SEC_SLV_ID_TMO_VALUE_F1_RD(src)              (((src) & 0x03ff0000)>>16)
#define SEC_SLV_ID_TMO_VALUE_F1_WR(src)         (((u32)(src)<<16) & 0x03ff0000)
#define SEC_SLV_ID_TMO_VALUE_F1_SET(dst,src) \
                      (((dst) & ~0x03ff0000) | (((u32)(src)<<16) & 0x03ff0000))
/*	 Fields SLV_RDY_TMO_ENABLE	 */
#define SEC_SLV_RDY_TMO_ENABLE_F1_WIDTH                                       1
#define SEC_SLV_RDY_TMO_ENABLE_F1_SHIFT                                      15
#define SEC_SLV_RDY_TMO_ENABLE_F1_MASK                               0x00008000
#define SEC_SLV_RDY_TMO_ENABLE_F1_RD(src)            (((src) & 0x00008000)>>15)
#define SEC_SLV_RDY_TMO_ENABLE_F1_WR(src)       (((u32)(src)<<15) & 0x00008000)
#define SEC_SLV_RDY_TMO_ENABLE_F1_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))
/*	 Fields SLV_RDY_TMO_VALUE	 */
#define SEC_SLV_RDY_TMO_VALUE_F1_WIDTH                                       10
#define SEC_SLV_RDY_TMO_VALUE_F1_SHIFT                                        0
#define SEC_SLV_RDY_TMO_VALUE_F1_MASK                                0x000003ff
#define SEC_SLV_RDY_TMO_VALUE_F1_RD(src)                 (((src) & 0x000003ff))
#define SEC_SLV_RDY_TMO_VALUE_F1_WR(src)            (((u32)(src)) & 0x000003ff)
#define SEC_SLV_RDY_TMO_VALUE_F1_SET(dst,src) \
                          (((dst) & ~0x000003ff) | (((u32)(src)) & 0x000003ff))

/*	Register STS_AXI_SLV_TMO	*/ 
/*	 Fields SLV_ARID_TMO	 */
#define SEC_SLV_ARID_TMO_WIDTH                                                1
#define SEC_SLV_ARID_TMO_SHIFT                                                3
#define SEC_SLV_ARID_TMO_MASK                                        0x00000008
#define SEC_SLV_ARID_TMO_RD(src)                      (((src) & 0x00000008)>>3)
#define SEC_SLV_ARID_TMO_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields SLV_ARRDY_TMO	 */
#define SEC_SLV_ARRDY_TMO_WIDTH                                               1
#define SEC_SLV_ARRDY_TMO_SHIFT                                               2
#define SEC_SLV_ARRDY_TMO_MASK                                       0x00000004
#define SEC_SLV_ARRDY_TMO_RD(src)                     (((src) & 0x00000004)>>2)
#define SEC_SLV_ARRDY_TMO_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields SLV_AWID_TMO	 */
#define SEC_SLV_AWID_TMO_WIDTH                                                1
#define SEC_SLV_AWID_TMO_SHIFT                                                1
#define SEC_SLV_AWID_TMO_MASK                                        0x00000002
#define SEC_SLV_AWID_TMO_RD(src)                      (((src) & 0x00000002)>>1)
#define SEC_SLV_AWID_TMO_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields SLV_AWRDY_TMO	 */
#define SEC_SLV_AWRDY_TMO_WIDTH                                               1
#define SEC_SLV_AWRDY_TMO_SHIFT                                               0
#define SEC_SLV_AWRDY_TMO_MASK                                       0x00000001
#define SEC_SLV_AWRDY_TMO_RD(src)                        (((src) & 0x00000001))
#define SEC_SLV_AWRDY_TMO_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register CFG_AXI_SS_CSR_TMO	*/ 
/*	 Fields CFG_CSR_TMO	 */
#define SEC_CFG_CSR_TMO_WIDTH                                                16
#define SEC_CFG_CSR_TMO_SHIFT                                                16
#define SEC_CFG_CSR_TMO_MASK                                         0xffff0000
#define SEC_CFG_CSR_TMO_RD(src)                      (((src) & 0xffff0000)>>16)
#define SEC_CFG_CSR_TMO_WR(src)                 (((u32)(src)<<16) & 0xffff0000)
#define SEC_CFG_CSR_TMO_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields CSR_ERR_RESP_EN	 */
#define SEC_CSR_ERR_RESP_EN_WIDTH                                             1
#define SEC_CSR_ERR_RESP_EN_SHIFT                                            15
#define SEC_CSR_ERR_RESP_EN_MASK                                     0x00008000
#define SEC_CSR_ERR_RESP_EN_RD(src)                  (((src) & 0x00008000)>>15)
#define SEC_CSR_ERR_RESP_EN_WR(src)             (((u32)(src)<<15) & 0x00008000)
#define SEC_CSR_ERR_RESP_EN_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))

/*	Register STS_READ_LATENCY_TOT_READ_REQS	*/ 
/*	 Fields TOT_READS	 */
#define SEC_TOT_READS_WIDTH                                                  16
#define SEC_TOT_READS_SHIFT                                                   0
#define SEC_TOT_READS_MASK                                           0x0000ffff
#define SEC_TOT_READS_RD(src)                            (((src) & 0x0000ffff))
#define SEC_TOT_READS_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Global Base Address	*/
#define SEC_AMPP_CSR_BASE_ADDR			0xddd830000ULL

/*    Address SEC_AMPP_CSR  Registers */
#define SEC_AMPP_CTL_ADDR                                            0x00008000
#define SEC_AMPP_CTL_DEFAULT                                         0x00000000
#define SEC_AMPP_DOMAIN_ID_ADDR                                      0x00008004
#define SEC_AMPP_DOMAIN_ID_DEFAULT                                   0x00000000
#define SEC_AMPP_RD_VIO_LOG_0_ADDR                                   0x00008008
#define SEC_AMPP_RD_VIO_LOG_0_DEFAULT                                0x00000000
#define SEC_AMPP_RD_VIO_LOG_1_ADDR                                   0x0000800c
#define SEC_AMPP_RD_VIO_LOG_1_DEFAULT                                0x00000000
#define SEC_AMPP_WR_VIO_LOG_0_ADDR                                   0x00008010
#define SEC_AMPP_WR_VIO_LOG_0_DEFAULT                                0x00000000
#define SEC_AMPP_WR_VIO_LOG_1_ADDR                                   0x00008014
#define SEC_AMPP_WR_VIO_LOG_1_DEFAULT                                0x00000000
#define SEC_AMPP_INT_ADDR                                            0x00008018
#define SEC_AMPP_INT_DEFAULT                                         0x00000000
#define SEC_AMPP_INTMASK_ADDR                                        0x0000801c
#define SEC_AMPP_DMT_SA_0_ADDR                                       0x00008050
#define SEC_AMPP_DMT_SA_0_DEFAULT                                    0x00000000
#define SEC_AMPP_DMT_EA_0_ADDR                                       0x00008054
#define SEC_AMPP_DMT_EA_0_DEFAULT                                    0x00000000
#define SEC_AMPP_DMT_RW_0_ADDR                                       0x00008058
#define SEC_AMPP_DMT_RW_0_DEFAULT                                    0x000000ff
#define SEC_AMPP_DMT_SA_1_ADDR                                       0x00008060
#define SEC_AMPP_DMT_SA_1_DEFAULT                                    0x00000000
#define SEC_AMPP_DMT_EA_1_ADDR                                       0x00008064
#define SEC_AMPP_DMT_EA_1_DEFAULT                                    0x00000000
#define SEC_AMPP_DMT_RW_1_ADDR                                       0x00008068
#define SEC_AMPP_DMT_RW_1_DEFAULT                                    0x000000ff

/*	Register AMPP_CTL	*/ 
/*	 Fields CFG_MST_VALID	 */
#define SEC_CFG_MST_VALID_WIDTH                                               1
#define SEC_CFG_MST_VALID_SHIFT                                              31
#define SEC_CFG_MST_VALID_MASK                                       0x80000000
#define SEC_CFG_MST_VALID_RD(src)                    (((src) & 0x80000000)>>31)
#define SEC_CFG_MST_VALID_WR(src)               (((u32)(src)<<31) & 0x80000000)
#define SEC_CFG_MST_VALID_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields PPC1_CFG_MST	 */
#define SEC_PPC1_CFG_MST_WIDTH                                                1
#define SEC_PPC1_CFG_MST_SHIFT                                                2
#define SEC_PPC1_CFG_MST_MASK                                        0x00000004
#define SEC_PPC1_CFG_MST_RD(src)                      (((src) & 0x00000004)>>2)
#define SEC_PPC1_CFG_MST_WR(src)                 (((u32)(src)<<2) & 0x00000004)
#define SEC_PPC1_CFG_MST_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields PPC0_CFG_MST	 */
#define SEC_PPC0_CFG_MST_WIDTH                                                1
#define SEC_PPC0_CFG_MST_SHIFT                                                1
#define SEC_PPC0_CFG_MST_MASK                                        0x00000002
#define SEC_PPC0_CFG_MST_RD(src)                      (((src) & 0x00000002)>>1)
#define SEC_PPC0_CFG_MST_WR(src)                 (((u32)(src)<<1) & 0x00000002)
#define SEC_PPC0_CFG_MST_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields IPP_CFG_MST	 */
#define SEC_IPP_CFG_MST_WIDTH                                                 1
#define SEC_IPP_CFG_MST_SHIFT                                                 0
#define SEC_IPP_CFG_MST_MASK                                         0x00000001
#define SEC_IPP_CFG_MST_RD(src)                          (((src) & 0x00000001))
#define SEC_IPP_CFG_MST_WR(src)                     (((u32)(src)) & 0x00000001)
#define SEC_IPP_CFG_MST_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register AMPP_DOMAIN_ID	*/ 
/*	 Fields ID_7	 */
#define SEC_ID_7_WIDTH                                                        2
#define SEC_ID_7_SHIFT                                                       14
#define SEC_ID_7_MASK                                                0x0000c000
#define SEC_ID_7_RD(src)                             (((src) & 0x0000c000)>>14)
#define SEC_ID_7_WR(src)                        (((u32)(src)<<14) & 0x0000c000)
#define SEC_ID_7_SET(dst,src) \
                      (((dst) & ~0x0000c000) | (((u32)(src)<<14) & 0x0000c000))
/*	 Fields ID_6	 */
#define SEC_ID_6_WIDTH                                                        2
#define SEC_ID_6_SHIFT                                                       12
#define SEC_ID_6_MASK                                                0x00003000
#define SEC_ID_6_RD(src)                             (((src) & 0x00003000)>>12)
#define SEC_ID_6_WR(src)                        (((u32)(src)<<12) & 0x00003000)
#define SEC_ID_6_SET(dst,src) \
                      (((dst) & ~0x00003000) | (((u32)(src)<<12) & 0x00003000))
/*	 Fields ID_5	 */
#define SEC_ID_5_WIDTH                                                        2
#define SEC_ID_5_SHIFT                                                       10
#define SEC_ID_5_MASK                                                0x00000c00
#define SEC_ID_5_RD(src)                             (((src) & 0x00000c00)>>10)
#define SEC_ID_5_WR(src)                        (((u32)(src)<<10) & 0x00000c00)
#define SEC_ID_5_SET(dst,src) \
                      (((dst) & ~0x00000c00) | (((u32)(src)<<10) & 0x00000c00))
/*	 Fields ID_4	 */
#define SEC_ID_4_WIDTH                                                        2
#define SEC_ID_4_SHIFT                                                        8
#define SEC_ID_4_MASK                                                0x00000300
#define SEC_ID_4_RD(src)                              (((src) & 0x00000300)>>8)
#define SEC_ID_4_WR(src)                         (((u32)(src)<<8) & 0x00000300)
#define SEC_ID_4_SET(dst,src) \
                       (((dst) & ~0x00000300) | (((u32)(src)<<8) & 0x00000300))
/*	 Fields ID_3	 */
#define SEC_ID_3_WIDTH                                                        2
#define SEC_ID_3_SHIFT                                                        6
#define SEC_ID_3_MASK                                                0x000000c0
#define SEC_ID_3_RD(src)                              (((src) & 0x000000c0)>>6)
#define SEC_ID_3_WR(src)                         (((u32)(src)<<6) & 0x000000c0)
#define SEC_ID_3_SET(dst,src) \
                       (((dst) & ~0x000000c0) | (((u32)(src)<<6) & 0x000000c0))
/*	 Fields ID_2	 */
#define SEC_ID_2_WIDTH                                                        2
#define SEC_ID_2_SHIFT                                                        4
#define SEC_ID_2_MASK                                                0x00000030
#define SEC_ID_2_RD(src)                              (((src) & 0x00000030)>>4)
#define SEC_ID_2_WR(src)                         (((u32)(src)<<4) & 0x00000030)
#define SEC_ID_2_SET(dst,src) \
                       (((dst) & ~0x00000030) | (((u32)(src)<<4) & 0x00000030))
/*	 Fields ID_1	 */
#define SEC_ID_1_WIDTH                                                        2
#define SEC_ID_1_SHIFT                                                        2
#define SEC_ID_1_MASK                                                0x0000000c
#define SEC_ID_1_RD(src)                              (((src) & 0x0000000c)>>2)
#define SEC_ID_1_WR(src)                         (((u32)(src)<<2) & 0x0000000c)
#define SEC_ID_1_SET(dst,src) \
                       (((dst) & ~0x0000000c) | (((u32)(src)<<2) & 0x0000000c))
/*	 Fields ID_0	 */
#define SEC_ID_0_WIDTH                                                        2
#define SEC_ID_0_SHIFT                                                        0
#define SEC_ID_0_MASK                                                0x00000003
#define SEC_ID_0_RD(src)                                 (((src) & 0x00000003))
#define SEC_ID_0_WR(src)                            (((u32)(src)) & 0x00000003)
#define SEC_ID_0_SET(dst,src) \
                          (((dst) & ~0x00000003) | (((u32)(src)) & 0x00000003))

/*	Register AMPP_RD_VIO_LOG_0	*/ 
/*	 Fields RW_ACCESS	 */
#define SEC_RW_ACCESS0_WIDTH                                                  2
#define SEC_RW_ACCESS0_SHIFT                                                 30
#define SEC_RW_ACCESS0_MASK                                          0xc0000000
#define SEC_RW_ACCESS0_RD(src)                       (((src) & 0xc0000000)>>30)
#define SEC_RW_ACCESS0_SET(dst,src) \
                      (((dst) & ~0xc0000000) | (((u32)(src)<<30) & 0xc0000000))
/*	 Fields DOMAIN_ID	 */
#define SEC_DOMAIN_ID0_WIDTH                                                  3
#define SEC_DOMAIN_ID0_SHIFT                                                 24
#define SEC_DOMAIN_ID0_MASK                                          0x07000000
#define SEC_DOMAIN_ID0_RD(src)                       (((src) & 0x07000000)>>24)
#define SEC_DOMAIN_ID0_SET(dst,src) \
                      (((dst) & ~0x07000000) | (((u32)(src)<<24) & 0x07000000))
/*	 Fields ADDRESS_36_12	 */
#define SEC_ADDRESS_36_120_WIDTH                                             24
#define SEC_ADDRESS_36_120_SHIFT                                              0
#define SEC_ADDRESS_36_120_MASK                                      0x00ffffff
#define SEC_ADDRESS_36_120_RD(src)                       (((src) & 0x00ffffff))
#define SEC_ADDRESS_36_120_SET(dst,src) \
                          (((dst) & ~0x00ffffff) | (((u32)(src)) & 0x00ffffff))

/*	Register AMPP_RD_VIO_LOG_1	*/ 
/*	 Fields TAG_9_5	 */
#define SEC_TAG_9_51_WIDTH                                                    5
#define SEC_TAG_9_51_SHIFT                                                    5
#define SEC_TAG_9_51_MASK                                            0x000003e0
#define SEC_TAG_9_51_RD(src)                          (((src) & 0x000003e0)>>5)
#define SEC_TAG_9_51_SET(dst,src) \
                       (((dst) & ~0x000003e0) | (((u32)(src)<<5) & 0x000003e0))
/*	 Fields TAG_4_0	 */
#define SEC_TAG_4_01_WIDTH                                                    5
#define SEC_TAG_4_01_SHIFT                                                    0
#define SEC_TAG_4_01_MASK                                            0x0000001f
#define SEC_TAG_4_01_RD(src)                             (((src) & 0x0000001f))
#define SEC_TAG_4_01_SET(dst,src) \
                          (((dst) & ~0x0000001f) | (((u32)(src)) & 0x0000001f))

/*	Register AMPP_WR_VIO_LOG_0	*/ 
/*	 Fields RW_ACCESS	 */
#define SEC_RW_ACCESS0_F1_WIDTH                                               2
#define SEC_RW_ACCESS0_F1_SHIFT                                              30
#define SEC_RW_ACCESS0_F1_MASK                                       0xc0000000
#define SEC_RW_ACCESS0_F1_RD(src)                    (((src) & 0xc0000000)>>30)
#define SEC_RW_ACCESS0_F1_SET(dst,src) \
                      (((dst) & ~0xc0000000) | (((u32)(src)<<30) & 0xc0000000))
/*	 Fields DOMAIN_ID	 */
#define SEC_DOMAIN_ID0_F1_WIDTH                                               3
#define SEC_DOMAIN_ID0_F1_SHIFT                                              24
#define SEC_DOMAIN_ID0_F1_MASK                                       0x07000000
#define SEC_DOMAIN_ID0_F1_RD(src)                    (((src) & 0x07000000)>>24)
#define SEC_DOMAIN_ID0_F1_SET(dst,src) \
                      (((dst) & ~0x07000000) | (((u32)(src)<<24) & 0x07000000))
/*	 Fields ADDRESS_36_12	 */
#define SEC_ADDRESS_36_120_F1_WIDTH                                          24
#define SEC_ADDRESS_36_120_F1_SHIFT                                           0
#define SEC_ADDRESS_36_120_F1_MASK                                   0x00ffffff
#define SEC_ADDRESS_36_120_F1_RD(src)                    (((src) & 0x00ffffff))
#define SEC_ADDRESS_36_120_F1_SET(dst,src) \
                          (((dst) & ~0x00ffffff) | (((u32)(src)) & 0x00ffffff))

/*	Register AMPP_WR_VIO_LOG_1	*/ 
/*	 Fields TAG_9_5	 */
#define SEC_TAG_9_51_F1_WIDTH                                                 5
#define SEC_TAG_9_51_F1_SHIFT                                                 5
#define SEC_TAG_9_51_F1_MASK                                         0x000003e0
#define SEC_TAG_9_51_F1_RD(src)                       (((src) & 0x000003e0)>>5)
#define SEC_TAG_9_51_F1_SET(dst,src) \
                       (((dst) & ~0x000003e0) | (((u32)(src)<<5) & 0x000003e0))
/*	 Fields TAG_4_0	 */
#define SEC_TAG_4_01_F1_WIDTH                                                 5
#define SEC_TAG_4_01_F1_SHIFT                                                 0
#define SEC_TAG_4_01_F1_MASK                                         0x0000001f
#define SEC_TAG_4_01_F1_RD(src)                          (((src) & 0x0000001f))
#define SEC_TAG_4_01_F1_SET(dst,src) \
                          (((dst) & ~0x0000001f) | (((u32)(src)) & 0x0000001f))

/*	Register AMPP_INT	*/ 
/*	 Fields RD_VIOL	 */
#define SEC_RD_VIOL_WIDTH                                                     1
#define SEC_RD_VIOL_SHIFT                                                     1
#define SEC_RD_VIOL_MASK                                             0x00000002
#define SEC_RD_VIOL_RD(src)                           (((src) & 0x00000002)>>1)
#define SEC_RD_VIOL_WR(src)                      (((u32)(src)<<1) & 0x00000002)
#define SEC_RD_VIOL_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields WR_VIOL	 */
#define SEC_WR_VIOL_WIDTH                                                     1
#define SEC_WR_VIOL_SHIFT                                                     0
#define SEC_WR_VIOL_MASK                                             0x00000001
#define SEC_WR_VIOL_RD(src)                              (((src) & 0x00000001))
#define SEC_WR_VIOL_WR(src)                         (((u32)(src)) & 0x00000001)
#define SEC_WR_VIOL_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register AMPP_INTMask	*/
/*    Mask Register Fields RD_VIOLMask    */
#define SEC_RD_VIOLMASK_WIDTH                                                 1
#define SEC_RD_VIOLMASK_SHIFT                                                 1
#define SEC_RD_VIOLMASK_MASK                                         0x00000002
#define SEC_RD_VIOLMASK_RD(src)                       (((src) & 0x00000002)>>1)
#define SEC_RD_VIOLMASK_WR(src)                  (((u32)(src)<<1) & 0x00000002)
#define SEC_RD_VIOLMASK_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*    Mask Register Fields WR_VIOLMask    */
#define SEC_WR_VIOLMASK_WIDTH                                                 1
#define SEC_WR_VIOLMASK_SHIFT                                                 0
#define SEC_WR_VIOLMASK_MASK                                         0x00000001
#define SEC_WR_VIOLMASK_RD(src)                          (((src) & 0x00000001))
#define SEC_WR_VIOLMASK_WR(src)                     (((u32)(src)) & 0x00000001)
#define SEC_WR_VIOLMASK_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register AMPP_DMT_SA_0	*/ 
/*	 Fields ENABLE	 */
#define SEC_ENABLE0_WIDTH                                                     1
#define SEC_ENABLE0_SHIFT                                                    31
#define SEC_ENABLE0_MASK                                             0x80000000
#define SEC_ENABLE0_RD(src)                          (((src) & 0x80000000)>>31)
#define SEC_ENABLE0_WR(src)                     (((u32)(src)<<31) & 0x80000000)
#define SEC_ENABLE0_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields START_ADDR_36_12	 */
#define SEC_START_ADDR_36_120_WIDTH                                          24
#define SEC_START_ADDR_36_120_SHIFT                                           0
#define SEC_START_ADDR_36_120_MASK                                   0x00ffffff
#define SEC_START_ADDR_36_120_RD(src)                    (((src) & 0x00ffffff))
#define SEC_START_ADDR_36_120_WR(src)               (((u32)(src)) & 0x00ffffff)
#define SEC_START_ADDR_36_120_SET(dst,src) \
                          (((dst) & ~0x00ffffff) | (((u32)(src)) & 0x00ffffff))

/*	Register AMPP_DMT_EA_0	*/ 
/*	 Fields END_ADDR_36_12	 */
#define SEC_END_ADDR_36_120_WIDTH                                            24
#define SEC_END_ADDR_36_120_SHIFT                                             0
#define SEC_END_ADDR_36_120_MASK                                     0x00ffffff
#define SEC_END_ADDR_36_120_RD(src)                      (((src) & 0x00ffffff))
#define SEC_END_ADDR_36_120_WR(src)                 (((u32)(src)) & 0x00ffffff)
#define SEC_END_ADDR_36_120_SET(dst,src) \
                          (((dst) & ~0x00ffffff) | (((u32)(src)) & 0x00ffffff))

/*	Register AMPP_DMT_RW_0	*/ 
/*	 Fields PPC1_ACCESS	 */
#define SEC_PPC1_ACCESS0_WIDTH                                                2
#define SEC_PPC1_ACCESS0_SHIFT                                                6
#define SEC_PPC1_ACCESS0_MASK                                        0x000000c0
#define SEC_PPC1_ACCESS0_RD(src)                      (((src) & 0x000000c0)>>6)
#define SEC_PPC1_ACCESS0_WR(src)                 (((u32)(src)<<6) & 0x000000c0)
#define SEC_PPC1_ACCESS0_SET(dst,src) \
                       (((dst) & ~0x000000c0) | (((u32)(src)<<6) & 0x000000c0))
/*	 Fields PPC0_ACCESS	 */
#define SEC_PPC0_ACCESS0_WIDTH                                                2
#define SEC_PPC0_ACCESS0_SHIFT                                                4
#define SEC_PPC0_ACCESS0_MASK                                        0x00000030
#define SEC_PPC0_ACCESS0_RD(src)                      (((src) & 0x00000030)>>4)
#define SEC_PPC0_ACCESS0_WR(src)                 (((u32)(src)<<4) & 0x00000030)
#define SEC_PPC0_ACCESS0_SET(dst,src) \
                       (((dst) & ~0x00000030) | (((u32)(src)<<4) & 0x00000030))
/*	 Fields EBUS_PCIE_ACCESS	 */
#define SEC_EBUS_PCIE_ACCESS0_WIDTH                                           2
#define SEC_EBUS_PCIE_ACCESS0_SHIFT                                           2
#define SEC_EBUS_PCIE_ACCESS0_MASK                                   0x0000000c
#define SEC_EBUS_PCIE_ACCESS0_RD(src)                 (((src) & 0x0000000c)>>2)
#define SEC_EBUS_PCIE_ACCESS0_WR(src)            (((u32)(src)<<2) & 0x0000000c)
#define SEC_EBUS_PCIE_ACCESS0_SET(dst,src) \
                       (((dst) & ~0x0000000c) | (((u32)(src)<<2) & 0x0000000c))
/*	 Fields IPP_ACCESS	 */
#define SEC_IPP_ACCESS0_WIDTH                                                 2
#define SEC_IPP_ACCESS0_SHIFT                                                 0
#define SEC_IPP_ACCESS0_MASK                                         0x00000003
#define SEC_IPP_ACCESS0_RD(src)                          (((src) & 0x00000003))
#define SEC_IPP_ACCESS0_WR(src)                     (((u32)(src)) & 0x00000003)
#define SEC_IPP_ACCESS0_SET(dst,src) \
                          (((dst) & ~0x00000003) | (((u32)(src)) & 0x00000003))

/*	Register AMPP_DMT_SA_1	*/ 
/*	 Fields ENABLE	 */
#define SEC_ENABLE1_WIDTH                                                     1
#define SEC_ENABLE1_SHIFT                                                    31
#define SEC_ENABLE1_MASK                                             0x80000000
#define SEC_ENABLE1_RD(src)                          (((src) & 0x80000000)>>31)
#define SEC_ENABLE1_WR(src)                     (((u32)(src)<<31) & 0x80000000)
#define SEC_ENABLE1_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields START_ADDR_36_12	 */
#define SEC_START_ADDR_36_121_WIDTH                                          24
#define SEC_START_ADDR_36_121_SHIFT                                           0
#define SEC_START_ADDR_36_121_MASK                                   0x00ffffff
#define SEC_START_ADDR_36_121_RD(src)                    (((src) & 0x00ffffff))
#define SEC_START_ADDR_36_121_WR(src)               (((u32)(src)) & 0x00ffffff)
#define SEC_START_ADDR_36_121_SET(dst,src) \
                          (((dst) & ~0x00ffffff) | (((u32)(src)) & 0x00ffffff))

/*	Register AMPP_DMT_EA_1	*/ 
/*	 Fields END_ADDR_36_12	 */
#define SEC_END_ADDR_36_121_WIDTH                                            24
#define SEC_END_ADDR_36_121_SHIFT                                             0
#define SEC_END_ADDR_36_121_MASK                                     0x00ffffff
#define SEC_END_ADDR_36_121_RD(src)                      (((src) & 0x00ffffff))
#define SEC_END_ADDR_36_121_WR(src)                 (((u32)(src)) & 0x00ffffff)
#define SEC_END_ADDR_36_121_SET(dst,src) \
                          (((dst) & ~0x00ffffff) | (((u32)(src)) & 0x00ffffff))

/*	Register AMPP_DMT_RW_1	*/ 
/*	 Fields PPC1_ACCESS	 */
#define SEC_PPC1_ACCESS1_WIDTH                                                2
#define SEC_PPC1_ACCESS1_SHIFT                                                6
#define SEC_PPC1_ACCESS1_MASK                                        0x000000c0
#define SEC_PPC1_ACCESS1_RD(src)                      (((src) & 0x000000c0)>>6)
#define SEC_PPC1_ACCESS1_WR(src)                 (((u32)(src)<<6) & 0x000000c0)
#define SEC_PPC1_ACCESS1_SET(dst,src) \
                       (((dst) & ~0x000000c0) | (((u32)(src)<<6) & 0x000000c0))
/*	 Fields PPC0_ACCESS	 */
#define SEC_PPC0_ACCESS1_WIDTH                                                2
#define SEC_PPC0_ACCESS1_SHIFT                                                4
#define SEC_PPC0_ACCESS1_MASK                                        0x00000030
#define SEC_PPC0_ACCESS1_RD(src)                      (((src) & 0x00000030)>>4)
#define SEC_PPC0_ACCESS1_WR(src)                 (((u32)(src)<<4) & 0x00000030)
#define SEC_PPC0_ACCESS1_SET(dst,src) \
                       (((dst) & ~0x00000030) | (((u32)(src)<<4) & 0x00000030))
/*	 Fields EBUS_PCIE_ACCESS	 */
#define SEC_EBUS_PCIE_ACCESS1_WIDTH                                           2
#define SEC_EBUS_PCIE_ACCESS1_SHIFT                                           2
#define SEC_EBUS_PCIE_ACCESS1_MASK                                   0x0000000c
#define SEC_EBUS_PCIE_ACCESS1_RD(src)                 (((src) & 0x0000000c)>>2)
#define SEC_EBUS_PCIE_ACCESS1_WR(src)            (((u32)(src)<<2) & 0x0000000c)
#define SEC_EBUS_PCIE_ACCESS1_SET(dst,src) \
                       (((dst) & ~0x0000000c) | (((u32)(src)<<2) & 0x0000000c))
/*	 Fields IPP_ACCESS	 */
#define SEC_IPP_ACCESS1_WIDTH                                                 2
#define SEC_IPP_ACCESS1_SHIFT                                                 0
#define SEC_IPP_ACCESS1_MASK                                         0x00000003
#define SEC_IPP_ACCESS1_RD(src)                          (((src) & 0x00000003))
#define SEC_IPP_ACCESS1_WR(src)                     (((u32)(src)) & 0x00000003)
#define SEC_IPP_ACCESS1_SET(dst,src) \
                          (((dst) & ~0x00000003) | (((u32)(src)) & 0x00000003))
	
/* Additional defines for Copatiblity with Green Mamba */
#define REV_NO_F2_RD		REV_NO_RD
#define BUS_ID_F2_RD		BUS_ID_RD
#define DEVICE_ID_F4_RD		DEVICE_ID_RD
#define QMIMASK_F2_MASK		QMIMASK_MASK
#define ADDRESS_MODE_WR		SEC_ADDRESS_MODE_WR
#define CONTROL_MODE_WR		SEC_CONTROL_MODE_WR
#define ENABLE_F8_WR		SEC_ENABLE_F8_WR
#endif
