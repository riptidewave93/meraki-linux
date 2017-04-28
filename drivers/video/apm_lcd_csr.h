/*
 * APM 86xxx LCD Header File
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
#ifndef _APM_LCD_CSR_H__
#define _APM_LCD_CSR_H__

/*	Global Base Address	*/
#define LCD_CSR_BASE_ADDR			0xddf440000ULL

/*    Address LCD_CSR  Registers */
#define START_ADDR0_ADDR                                             0x00001000
#define START_ADDR0_DEFAULT                                          0x00000000
#define END_ADDR0_ADDR                                               0x00001004
#define END_ADDR0_DEFAULT                                            0x00000000
#define START_ADDR1_ADDR                                             0x00001008
#define START_ADDR1_DEFAULT                                          0x00000000
#define END_ADDR1_ADDR                                               0x0000100c
#define END_ADDR1_DEFAULT                                            0x00000000
#define START_ADDR2_ADDR                                             0x00001010
#define START_ADDR2_DEFAULT                                          0x00000000
#define END_ADDR2_ADDR                                               0x00001014
#define END_ADDR2_DEFAULT                                            0x00000000
#define START_ADDR3_ADDR                                             0x00001018
#define START_ADDR3_DEFAULT                                          0x00000000
#define END_ADDR3_ADDR                                               0x0000101c
#define END_ADDR3_DEFAULT                                            0x00000000
#define START_ADDR4_ADDR                                             0x00001020
#define START_ADDR4_DEFAULT                                          0x00000000
#define END_ADDR4_ADDR                                               0x00001024
#define END_ADDR4_DEFAULT                                            0x00000000
#define START_ADDR5_ADDR                                             0x00001028
#define START_ADDR5_DEFAULT                                          0x00000000
#define END_ADDR5_ADDR                                               0x0000102c
#define END_ADDR5_DEFAULT                                            0x00000000
#define START_ADDR6_ADDR                                             0x00001030
#define START_ADDR6_DEFAULT                                          0x00000000
#define END_ADDR6_ADDR                                               0x00001034
#define END_ADDR6_DEFAULT                                            0x00000000
#define START_ADDR7_ADDR                                             0x00001038
#define START_ADDR7_DEFAULT                                          0x00000000
#define END_ADDR7_ADDR                                               0x0000103c
#define END_ADDR7_DEFAULT                                            0x00000000
#define START_ADDR8_ADDR                                             0x00001040
#define START_ADDR8_DEFAULT                                          0x00000000
#define END_ADDR8_ADDR                                               0x00001044
#define END_ADDR8_DEFAULT                                            0x00000000
#define START_ADDR9_ADDR                                             0x00001048
#define START_ADDR9_DEFAULT                                          0x00000000
#define END_ADDR9_ADDR                                               0x0000104c
#define END_ADDR9_DEFAULT                                            0x00000000
#define START_ADDR10_ADDR                                            0x00001050
#define START_ADDR10_DEFAULT                                         0x00000000
#define END_ADDR10_ADDR                                              0x00001054
#define END_ADDR10_DEFAULT                                           0x00000000
#define START_ADDR11_ADDR                                            0x00001058
#define START_ADDR11_DEFAULT                                         0x00000000
#define END_ADDR11_ADDR                                              0x0000105c
#define END_ADDR11_DEFAULT                                           0x00000000
#define START_ADDR12_ADDR                                            0x00001060
#define START_ADDR12_DEFAULT                                         0x00000000
#define END_ADDR12_ADDR                                              0x00001064
#define END_ADDR12_DEFAULT                                           0x00000000
#define START_ADDR13_ADDR                                            0x00001068
#define START_ADDR13_DEFAULT                                         0x00000000
#define END_ADDR13_ADDR                                              0x0000106c
#define END_ADDR13_DEFAULT                                           0x00000000
#define START_ADDR14_ADDR                                            0x00001070
#define START_ADDR14_DEFAULT                                         0x00000000
#define END_ADDR14_ADDR                                              0x00001074
#define END_ADDR14_DEFAULT                                           0x00000000
#define START_ADDR15_ADDR                                            0x00001078
#define START_ADDR15_DEFAULT                                         0x00000000
#define END_ADDR15_ADDR                                              0x0000107c
#define END_ADDR15_DEFAULT                                           0x00000000
#define START_ADDR16_ADDR                                            0x00001080
#define START_ADDR16_DEFAULT                                         0x00000000
#define END_ADDR16_ADDR                                              0x00001084
#define END_ADDR16_DEFAULT                                           0x00000000
#define CORE_ID_REG_ADDR                                             0x00001090
#define CORE_ID_REG_DEFAULT                                          0x00000065
#define DEBUG_TEST_REG_ADDR                                          0x00001094
#define DEBUG_TEST_REG_DEFAULT                                       0x00000000
#define LCDC_CR1_ADDR                                                0x00000000
#define LCDC_CR1_DEFAULT                                             0x00000000
#define LCDC_HTR_ADDR                                                0x00000008
#define LCDC_HTR_DEFAULT                                             0x00000000
#define LCDC_VTR1_ADDR                                               0x0000000c
#define LCDC_VTR1_DEFAULT                                            0x00000000
#define LCDC_VTR2_ADDR                                               0x00000010
#define LCDC_VTR2_DEFAULT                                            0x00000000
#define LCDC_PCTR_ADDR                                               0x00000014
#define LCDC_PCTR_DEFAULT                                            0x00000000
#define LCDC_ISR_ADDR                                                0x00000018
#define LCDC_ISR_DEFAULT                                             0x00000000
#define LCDC_IMR_ADDR                                                0x0000001c
#define LCDC_IMR_DEFAULT                                             0x00000000
#define LCDC_IVR_ADDR                                                0x00000020
#define LCDC_IVR_DEFAULT                                             0x00000000
#define LCDC_ISCR_ADDR                                               0x00000024
#define LCDC_ISCR_DEFAULT                                            0x00000000
#define LCDC_DBAR_ADDR                                               0x00000028
#define LCDC_DBAR_DEFAULT                                            0x00000000
#define LCDC_DCAR_ADDR                                               0x0000002c
#define LCDC_DCAR_DEFAULT                                            0x00000000
#define LCDC_DEAR_ADDR                                               0x00000030
#define LCDC_DEAR_DEFAULT                                            0x00000000
#define LCDC_PWMFR_ADDR                                              0x00000034
#define LCDC_PWMFR_DEFAULT                                           0x00000000
#define LCDC_PWMDCR_ADDR                                             0x00000038
#define LCDC_PWMDCR_DEFAULT                                          0x00000000
#define LCDC_DFBAR_ADDR                                              0x0000003c
#define LCDC_DFBAR_DEFAULT                                           0x00000000
#define LCDC_DFLAR_ADDR                                              0x00000040
#define LCDC_DFLAR_DEFAULT                                           0x00000000
#define LCDC_CIR_ADDR                                                0x000001fc
#define LCDC_CIR_DEFAULT                                             0x0090550d
#define LCDC_PALT_ADDR                                               0x00000200
#define LCDC_PALT_DEFAULT                                            0x00000000
#define LCDC_PALT_SIZE_ADDR                                          0x00000c00
#define LCDC_PALT_SIZE_DEFAULT                                       0x00000200
#define LCDC_OWER_ADDR                                               0x00000600
#define LCDC_OWER_DEFAULT                                            0x00000000
#define LCDC_OXSER0_ADDR                                             0x00000604
#define LCDC_OXSER0_DEFAULT                                          0x00000000
#define LCDC_OYSER0_ADDR                                             0x00000608
#define LCDC_OYSER0_DEFAULT                                          0x00000000
#define LCDC_OWDBAR0_ADDR                                            0x0000060c
#define LCDC_OWDBAR0_DEFAULT                                         0x00000000
#define LCDC_OWDCAR0_ADDR                                            0x00000610
#define LCDC_OWDCAR0_DEFAULT                                         0x00000000
#define LCDC_OXSER1_ADDR                                             0x00000614
#define LCDC_OXSER1_DEFAULT                                          0x00000000
#define LCDC_OYSER1_ADDR                                             0x00000618
#define LCDC_OYSER1_DEFAULT                                          0x00000000
#define LCDC_OWDBAR1_ADDR                                            0x0000061c
#define LCDC_OWDBAR1_DEFAULT                                         0x00000000
#define LCDC_OWDCAR1_ADDR                                            0x00000620
#define LCDC_OWDCAR1_DEFAULT                                         0x00000000
#define LCDC_OXSER2_ADDR                                             0x00000624
#define LCDC_OXSER2_DEFAULT                                          0x00000000
#define LCDC_OYSER2_ADDR                                             0x00000628
#define LCDC_OYSER2_DEFAULT                                          0x00000000
#define LCDC_OWDBAR2_ADDR                                            0x0000062c
#define LCDC_OWDBAR2_DEFAULT                                         0x00000000
#define LCDC_OWDCAR2_ADDR                                            0x00000630
#define LCDC_OWDCAR2_DEFAULT                                         0x00000000
#define LCDC_OXSER3_ADDR                                             0x00000634
#define LCDC_OXSER3_DEFAULT                                          0x00000000
#define LCDC_OYSER3_ADDR                                             0x00000638
#define LCDC_OYSER3_DEFAULT                                          0x00000000
#define LCDC_OWDBAR3_ADDR                                            0x0000063c
#define LCDC_OWDBAR3_DEFAULT                                         0x00000000
#define LCDC_OWDCAR3_ADDR                                            0x00000640
#define LCDC_OWDCAR3_DEFAULT                                         0x00000000
#define LCDC_OXSER4_ADDR                                             0x00000644
#define LCDC_OXSER4_DEFAULT                                          0x00000000
#define LCDC_OYSER4_ADDR                                             0x00000648
#define LCDC_OYSER4_DEFAULT                                          0x00000000
#define LCDC_OWDBAR4_ADDR                                            0x0000064c
#define LCDC_OWDBAR4_DEFAULT                                         0x00000000
#define LCDC_OWDCAR4_ADDR                                            0x00000650
#define LCDC_OWDCAR4_DEFAULT                                         0x00000000
#define LCDC_OXSER5_ADDR                                             0x00000654
#define LCDC_OXSER5_DEFAULT                                          0x00000000
#define LCDC_OYSER5_ADDR                                             0x00000658
#define LCDC_OYSER5_DEFAULT                                          0x00000000
#define LCDC_OWDBAR5_ADDR                                            0x0000065c
#define LCDC_OWDBAR5_DEFAULT                                         0x00000000
#define LCDC_OWDCAR5_ADDR                                            0x00000660
#define LCDC_OWDCAR5_DEFAULT                                         0x00000000
#define LCDC_OXSER6_ADDR                                             0x00000664
#define LCDC_OXSER6_DEFAULT                                          0x00000000
#define LCDC_OYSER6_ADDR                                             0x00000668
#define LCDC_OYSER6_DEFAULT                                          0x00000000
#define LCDC_OWDBAR6_ADDR                                            0x0000066c
#define LCDC_OWDBAR6_DEFAULT                                         0x00000000
#define LCDC_OWDCAR6_ADDR                                            0x00000670
#define LCDC_OWDCAR6_DEFAULT                                         0x00000000
#define LCDC_OXSER7_ADDR                                             0x00000674
#define LCDC_OXSER7_DEFAULT                                          0x00000000
#define LCDC_OYSER7_ADDR                                             0x00000678
#define LCDC_OYSER7_DEFAULT                                          0x00000000
#define LCDC_OWDBAR7_ADDR                                            0x0000067c
#define LCDC_OWDBAR7_DEFAULT                                         0x00000000
#define LCDC_OWDCAR7_ADDR                                            0x00000680
#define LCDC_OWDCAR7_DEFAULT                                         0x00000000
#define LCDC_OXSER8_ADDR                                             0x00000684
#define LCDC_OXSER8_DEFAULT                                          0x00000000
#define LCDC_OYSER8_ADDR                                             0x00000688
#define LCDC_OYSER8_DEFAULT                                          0x00000000
#define LCDC_OWDBAR8_ADDR                                            0x0000068c
#define LCDC_OWDBAR8_DEFAULT                                         0x00000000
#define LCDC_OWDCAR8_ADDR                                            0x00000690
#define LCDC_OWDCAR8_DEFAULT                                         0x00000000
#define LCDC_OXSER9_ADDR                                             0x00000694
#define LCDC_OXSER9_DEFAULT                                          0x00000000
#define LCDC_OYSER9_ADDR                                             0x00000698
#define LCDC_OYSER9_DEFAULT                                          0x00000000
#define LCDC_OWDBAR9_ADDR                                            0x0000069c
#define LCDC_OWDBAR9_DEFAULT                                         0x00000000
#define LCDC_OWDCAR9_ADDR                                            0x000006a0
#define LCDC_OWDCAR9_DEFAULT                                         0x00000000
#define LCDC_OXSER10_ADDR                                            0x000006a4
#define LCDC_OXSER10_DEFAULT                                         0x00000000
#define LCDC_OYSER10_ADDR                                            0x000006a8
#define LCDC_OYSER10_DEFAULT                                         0x00000000
#define LCDC_OWDBAR10_ADDR                                           0x000006ac
#define LCDC_OWDBAR10_DEFAULT                                        0x00000000
#define LCDC_OWDCAR10_ADDR                                           0x000006b0
#define LCDC_OWDCAR10_DEFAULT                                        0x00000000
#define LCDC_OXSER11_ADDR                                            0x000006b4
#define LCDC_OXSER11_DEFAULT                                         0x00000000
#define LCDC_OYSER11_ADDR                                            0x000006b8
#define LCDC_OYSER11_DEFAULT                                         0x00000000
#define LCDC_OWDBAR11_ADDR                                           0x000006bc
#define LCDC_OWDBAR11_DEFAULT                                        0x00000000
#define LCDC_OWDCAR11_ADDR                                           0x000006c0
#define LCDC_OWDCAR11_DEFAULT                                        0x00000000
#define LCDC_OXSER12_ADDR                                            0x000006c4
#define LCDC_OXSER12_DEFAULT                                         0x00000000
#define LCDC_OYSER12_ADDR                                            0x000006c8
#define LCDC_OYSER12_DEFAULT                                         0x00000000
#define LCDC_OWDBAR12_ADDR                                           0x000006cc
#define LCDC_OWDBAR12_DEFAULT                                        0x00000000
#define LCDC_OWDCAR12_ADDR                                           0x000006d0
#define LCDC_OWDCAR12_DEFAULT                                        0x00000000
#define LCDC_OXSER13_ADDR                                            0x000006d4
#define LCDC_OXSER13_DEFAULT                                         0x00000000
#define LCDC_OYSER13_ADDR                                            0x000006d8
#define LCDC_OYSER13_DEFAULT                                         0x00000000
#define LCDC_OWDBAR13_ADDR                                           0x000006dc
#define LCDC_OWDBAR13_DEFAULT                                        0x00000000
#define LCDC_OWDCAR13_ADDR                                           0x000006e0
#define LCDC_OWDCAR13_DEFAULT                                        0x00000000
#define LCDC_OXSER14_ADDR                                            0x000006e4
#define LCDC_OXSER14_DEFAULT                                         0x00000000
#define LCDC_OYSER14_ADDR                                            0x000006e8
#define LCDC_OYSER14_DEFAULT                                         0x00000000
#define LCDC_OWDBAR14_ADDR                                           0x000006ec
#define LCDC_OWDBAR14_DEFAULT                                        0x00000000
#define LCDC_OWDCAR14_ADDR                                           0x000006f0
#define LCDC_OWDCAR14_DEFAULT                                        0x00000000
#define LCDC_OXSER15_ADDR                                            0x000006f4
#define LCDC_OXSER15_DEFAULT                                         0x00000000
#define LCDC_OYSER15_ADDR                                            0x000006f8
#define LCDC_OYSER15_DEFAULT                                         0x00000000
#define LCDC_OWDBAR15_ADDR                                           0x000006fc
#define LCDC_OWDBAR15_DEFAULT                                        0x00000000
#define LCDC_OWDCAR15_ADDR                                           0x00000700
#define LCDC_OWDCAR15_DEFAULT                                        0x00000000
#define CSC_CR1_ADDR                                                 0x00002000
#define CSC_CR1_DEFAULT                                              0x00000000
#define CSC_KA1_ADDR                                                 0x00002010
#define CSC_KA1_DEFAULT                                              0x00000000
#define CSC_KB1_ADDR                                                 0x00002014
#define CSC_KB1_DEFAULT                                              0x00000000
#define CSC_KC1_ADDR                                                 0x00002018
#define CSC_KC1_DEFAULT                                              0x00000000
#define CSC_KA2_ADDR                                                 0x00002020
#define CSC_KA2_DEFAULT                                              0x00000000
#define CSC_KB2_ADDR                                                 0x00002024
#define CSC_KB2_DEFAULT                                              0x00000000
#define CSC_KC2_ADDR                                                 0x00002028
#define CSC_KC2_DEFAULT                                              0x00000000
#define CSC_KA3_ADDR                                                 0x00002030
#define CSC_KA3_DEFAULT                                              0x00000000
#define CSC_KB3_ADDR                                                 0x00002034
#define CSC_KB3_DEFAULT                                              0x00000000
#define CSC_KC3_ADDR                                                 0x00002038
#define CSC_KC3_DEFAULT                                              0x00000000
#define CSC_SX_ADDR                                                  0x0000201c
#define CSC_SX_DEFAULT                                               0x00000000
#define CSC_SY_ADDR                                                  0x0000202c
#define CSC_SY_DEFAULT                                               0x00000000
#define CSC_SZ_ADDR                                                  0x0000203c
#define CSC_SZ_DEFAULT                                               0x00000000
#define CR_CR1_ADDR                                                  0x00002100
#define CR_CR1_DEFAULT                                               0x00000000
#define CR_FIR_CE0_ADDR                                              0x00002110
#define CR_FIR_CE0_DEFAULT                                           0x00000000
#define CR_FIR_CE1_ADDR                                              0x00002114
#define CR_FIR_CE1_DEFAULT                                           0x00000000
#define CR_FIR_CE2_ADDR                                              0x00002118
#define CR_FIR_CE2_DEFAULT                                           0x00000000
#define BT656E_CR1_ADDR                                              0x00002200
#define BT656E_CR1_DEFAULT                                           0x00000000
#define BT656E_VHDO_ADDR                                             0x00002204
#define BT656E_VHDO_DEFAULT                                          0x00000000
#define BT656E_FSO_ADDR                                              0x00002208
#define BT656E_FSO_DEFAULT                                           0x00000000
#define ISR_ADDR                                                     0x0000220c
#define ISR_DEFAULT                                                  0x00000000
#define IMR_ADDR                                                     0x00002210
#define IMR_DEFAULT                                                  0x00000000
#define IVR_ADDR                                                     0x00002214
#define IVR_DEFAULT                                                  0x00000000

/*	Register start_addr0	*/ 
/*	 Fields sa	 */
#define SA0_WIDTH                                                            28
#define SA0_SHIFT                                                             4
#define SA0_MASK                                                     0xfffffff0
#define SA0_RD(src)                                   (((src) & 0xfffffff0)>>4)
#define SA0_WR(src)                              (((u32)(src)<<4) & 0xfffffff0)
#define SA0_SET(dst,src) \
                       (((dst) & ~0xfffffff0) | (((u32)(src)<<4) & 0xfffffff0))
/*	 Fields ame	 */
#define AME0_WIDTH                                                            1
#define AME0_SHIFT                                                            0
#define AME0_MASK                                                    0x00000001
#define AME0_RD(src)                                     (((src) & 0x00000001))
#define AME0_WR(src)                                (((u32)(src)) & 0x00000001)
#define AME0_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register end_addr0	*/ 
/*	 Fields sa	 */
#define SA0_F1_WIDTH                                                         28
#define SA0_F1_SHIFT                                                          4
#define SA0_F1_MASK                                                  0xfffffff0
#define SA0_F1_RD(src)                                (((src) & 0xfffffff0)>>4)
#define SA0_F1_WR(src)                           (((u32)(src)<<4) & 0xfffffff0)
#define SA0_F1_SET(dst,src) \
                       (((dst) & ~0xfffffff0) | (((u32)(src)<<4) & 0xfffffff0))
/*	 Fields msn	 */
#define MSN0_WIDTH                                                            4
#define MSN0_SHIFT                                                            0
#define MSN0_MASK                                                    0x0000000f
#define MSN0_RD(src)                                     (((src) & 0x0000000f))
#define MSN0_WR(src)                                (((u32)(src)) & 0x0000000f)
#define MSN0_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register start_addr1	*/ 
/*	 Fields sa	 */
#define SA1_WIDTH                                                            28
#define SA1_SHIFT                                                             4
#define SA1_MASK                                                     0xfffffff0
#define SA1_RD(src)                                   (((src) & 0xfffffff0)>>4)
#define SA1_WR(src)                              (((u32)(src)<<4) & 0xfffffff0)
#define SA1_SET(dst,src) \
                       (((dst) & ~0xfffffff0) | (((u32)(src)<<4) & 0xfffffff0))
/*	 Fields ame	 */
#define AME1_WIDTH                                                            1
#define AME1_SHIFT                                                            0
#define AME1_MASK                                                    0x00000001
#define AME1_RD(src)                                     (((src) & 0x00000001))
#define AME1_WR(src)                                (((u32)(src)) & 0x00000001)
#define AME1_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register end_addr1	*/ 
/*	 Fields sa	 */
#define SA1_F1_WIDTH                                                         28
#define SA1_F1_SHIFT                                                          4
#define SA1_F1_MASK                                                  0xfffffff0
#define SA1_F1_RD(src)                                (((src) & 0xfffffff0)>>4)
#define SA1_F1_WR(src)                           (((u32)(src)<<4) & 0xfffffff0)
#define SA1_F1_SET(dst,src) \
                       (((dst) & ~0xfffffff0) | (((u32)(src)<<4) & 0xfffffff0))
/*	 Fields msn	 */
#define MSN1_WIDTH                                                            4
#define MSN1_SHIFT                                                            0
#define MSN1_MASK                                                    0x0000000f
#define MSN1_RD(src)                                     (((src) & 0x0000000f))
#define MSN1_WR(src)                                (((u32)(src)) & 0x0000000f)
#define MSN1_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register start_addr2	*/ 
/*	 Fields sa	 */
#define SA2_WIDTH                                                            28
#define SA2_SHIFT                                                             4
#define SA2_MASK                                                     0xfffffff0
#define SA2_RD(src)                                   (((src) & 0xfffffff0)>>4)
#define SA2_WR(src)                              (((u32)(src)<<4) & 0xfffffff0)
#define SA2_SET(dst,src) \
                       (((dst) & ~0xfffffff0) | (((u32)(src)<<4) & 0xfffffff0))
/*	 Fields ame	 */
#define AME2_WIDTH                                                            1
#define AME2_SHIFT                                                            0
#define AME2_MASK                                                    0x00000001
#define AME2_RD(src)                                     (((src) & 0x00000001))
#define AME2_WR(src)                                (((u32)(src)) & 0x00000001)
#define AME2_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register end_addr2	*/ 
/*	 Fields sa	 */
#define SA2_F1_WIDTH                                                         28
#define SA2_F1_SHIFT                                                          4
#define SA2_F1_MASK                                                  0xfffffff0
#define SA2_F1_RD(src)                                (((src) & 0xfffffff0)>>4)
#define SA2_F1_WR(src)                           (((u32)(src)<<4) & 0xfffffff0)
#define SA2_F1_SET(dst,src) \
                       (((dst) & ~0xfffffff0) | (((u32)(src)<<4) & 0xfffffff0))
/*	 Fields msn	 */
#define MSN2_WIDTH                                                            4
#define MSN2_SHIFT                                                            0
#define MSN2_MASK                                                    0x0000000f
#define MSN2_RD(src)                                     (((src) & 0x0000000f))
#define MSN2_WR(src)                                (((u32)(src)) & 0x0000000f)
#define MSN2_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register start_addr3	*/ 
/*	 Fields sa	 */
#define SA3_WIDTH                                                            28
#define SA3_SHIFT                                                             4
#define SA3_MASK                                                     0xfffffff0
#define SA3_RD(src)                                   (((src) & 0xfffffff0)>>4)
#define SA3_WR(src)                              (((u32)(src)<<4) & 0xfffffff0)
#define SA3_SET(dst,src) \
                       (((dst) & ~0xfffffff0) | (((u32)(src)<<4) & 0xfffffff0))
/*	 Fields ame	 */
#define AME3_WIDTH                                                            1
#define AME3_SHIFT                                                            0
#define AME3_MASK                                                    0x00000001
#define AME3_RD(src)                                     (((src) & 0x00000001))
#define AME3_WR(src)                                (((u32)(src)) & 0x00000001)
#define AME3_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register end_addr3	*/ 
/*	 Fields sa	 */
#define SA3_F1_WIDTH                                                         28
#define SA3_F1_SHIFT                                                          4
#define SA3_F1_MASK                                                  0xfffffff0
#define SA3_F1_RD(src)                                (((src) & 0xfffffff0)>>4)
#define SA3_F1_WR(src)                           (((u32)(src)<<4) & 0xfffffff0)
#define SA3_F1_SET(dst,src) \
                       (((dst) & ~0xfffffff0) | (((u32)(src)<<4) & 0xfffffff0))
/*	 Fields msn	 */
#define MSN3_WIDTH                                                            4
#define MSN3_SHIFT                                                            0
#define MSN3_MASK                                                    0x0000000f
#define MSN3_RD(src)                                     (((src) & 0x0000000f))
#define MSN3_WR(src)                                (((u32)(src)) & 0x0000000f)
#define MSN3_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register start_addr4	*/ 
/*	 Fields sa	 */
#define SA4_WIDTH                                                            28
#define SA4_SHIFT                                                             4
#define SA4_MASK                                                     0xfffffff0
#define SA4_RD(src)                                   (((src) & 0xfffffff0)>>4)
#define SA4_WR(src)                              (((u32)(src)<<4) & 0xfffffff0)
#define SA4_SET(dst,src) \
                       (((dst) & ~0xfffffff0) | (((u32)(src)<<4) & 0xfffffff0))
/*	 Fields ame	 */
#define AME4_WIDTH                                                            1
#define AME4_SHIFT                                                            0
#define AME4_MASK                                                    0x00000001
#define AME4_RD(src)                                     (((src) & 0x00000001))
#define AME4_WR(src)                                (((u32)(src)) & 0x00000001)
#define AME4_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register end_addr4	*/ 
/*	 Fields sa	 */
#define SA4_F1_WIDTH                                                         28
#define SA4_F1_SHIFT                                                          4
#define SA4_F1_MASK                                                  0xfffffff0
#define SA4_F1_RD(src)                                (((src) & 0xfffffff0)>>4)
#define SA4_F1_WR(src)                           (((u32)(src)<<4) & 0xfffffff0)
#define SA4_F1_SET(dst,src) \
                       (((dst) & ~0xfffffff0) | (((u32)(src)<<4) & 0xfffffff0))
/*	 Fields msn	 */
#define MSN4_WIDTH                                                            4
#define MSN4_SHIFT                                                            0
#define MSN4_MASK                                                    0x0000000f
#define MSN4_RD(src)                                     (((src) & 0x0000000f))
#define MSN4_WR(src)                                (((u32)(src)) & 0x0000000f)
#define MSN4_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register start_addr5	*/ 
/*	 Fields sa	 */
#define SA5_WIDTH                                                            28
#define SA5_SHIFT                                                             4
#define SA5_MASK                                                     0xfffffff0
#define SA5_RD(src)                                   (((src) & 0xfffffff0)>>4)
#define SA5_WR(src)                              (((u32)(src)<<4) & 0xfffffff0)
#define SA5_SET(dst,src) \
                       (((dst) & ~0xfffffff0) | (((u32)(src)<<4) & 0xfffffff0))
/*	 Fields ame	 */
#define AME5_WIDTH                                                            1
#define AME5_SHIFT                                                            0
#define AME5_MASK                                                    0x00000001
#define AME5_RD(src)                                     (((src) & 0x00000001))
#define AME5_WR(src)                                (((u32)(src)) & 0x00000001)
#define AME5_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register end_addr5	*/ 
/*	 Fields sa	 */
#define SA5_F1_WIDTH                                                         28
#define SA5_F1_SHIFT                                                          4
#define SA5_F1_MASK                                                  0xfffffff0
#define SA5_F1_RD(src)                                (((src) & 0xfffffff0)>>4)
#define SA5_F1_WR(src)                           (((u32)(src)<<4) & 0xfffffff0)
#define SA5_F1_SET(dst,src) \
                       (((dst) & ~0xfffffff0) | (((u32)(src)<<4) & 0xfffffff0))
/*	 Fields msn	 */
#define MSN5_WIDTH                                                            4
#define MSN5_SHIFT                                                            0
#define MSN5_MASK                                                    0x0000000f
#define MSN5_RD(src)                                     (((src) & 0x0000000f))
#define MSN5_WR(src)                                (((u32)(src)) & 0x0000000f)
#define MSN5_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register start_addr6	*/ 
/*	 Fields sa	 */
#define SA6_WIDTH                                                            28
#define SA6_SHIFT                                                             4
#define SA6_MASK                                                     0xfffffff0
#define SA6_RD(src)                                   (((src) & 0xfffffff0)>>4)
#define SA6_WR(src)                              (((u32)(src)<<4) & 0xfffffff0)
#define SA6_SET(dst,src) \
                       (((dst) & ~0xfffffff0) | (((u32)(src)<<4) & 0xfffffff0))
/*	 Fields ame	 */
#define AME6_WIDTH                                                            1
#define AME6_SHIFT                                                            0
#define AME6_MASK                                                    0x00000001
#define AME6_RD(src)                                     (((src) & 0x00000001))
#define AME6_WR(src)                                (((u32)(src)) & 0x00000001)
#define AME6_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register end_addr6	*/ 
/*	 Fields sa	 */
#define SA6_F1_WIDTH                                                         28
#define SA6_F1_SHIFT                                                          4
#define SA6_F1_MASK                                                  0xfffffff0
#define SA6_F1_RD(src)                                (((src) & 0xfffffff0)>>4)
#define SA6_F1_WR(src)                           (((u32)(src)<<4) & 0xfffffff0)
#define SA6_F1_SET(dst,src) \
                       (((dst) & ~0xfffffff0) | (((u32)(src)<<4) & 0xfffffff0))
/*	 Fields msn	 */
#define MSN6_WIDTH                                                            4
#define MSN6_SHIFT                                                            0
#define MSN6_MASK                                                    0x0000000f
#define MSN6_RD(src)                                     (((src) & 0x0000000f))
#define MSN6_WR(src)                                (((u32)(src)) & 0x0000000f)
#define MSN6_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register start_addr7	*/ 
/*	 Fields sa	 */
#define SA7_WIDTH                                                            28
#define SA7_SHIFT                                                             4
#define SA7_MASK                                                     0xfffffff0
#define SA7_RD(src)                                   (((src) & 0xfffffff0)>>4)
#define SA7_WR(src)                              (((u32)(src)<<4) & 0xfffffff0)
#define SA7_SET(dst,src) \
                       (((dst) & ~0xfffffff0) | (((u32)(src)<<4) & 0xfffffff0))
/*	 Fields ame	 */
#define AME7_WIDTH                                                            1
#define AME7_SHIFT                                                            0
#define AME7_MASK                                                    0x00000001
#define AME7_RD(src)                                     (((src) & 0x00000001))
#define AME7_WR(src)                                (((u32)(src)) & 0x00000001)
#define AME7_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register end_addr7	*/ 
/*	 Fields sa	 */
#define SA7_F1_WIDTH                                                         28
#define SA7_F1_SHIFT                                                          4
#define SA7_F1_MASK                                                  0xfffffff0
#define SA7_F1_RD(src)                                (((src) & 0xfffffff0)>>4)
#define SA7_F1_WR(src)                           (((u32)(src)<<4) & 0xfffffff0)
#define SA7_F1_SET(dst,src) \
                       (((dst) & ~0xfffffff0) | (((u32)(src)<<4) & 0xfffffff0))
/*	 Fields msn	 */
#define MSN7_WIDTH                                                            4
#define MSN7_SHIFT                                                            0
#define MSN7_MASK                                                    0x0000000f
#define MSN7_RD(src)                                     (((src) & 0x0000000f))
#define MSN7_WR(src)                                (((u32)(src)) & 0x0000000f)
#define MSN7_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register start_addr8	*/ 
/*	 Fields sa	 */
#define SA8_WIDTH                                                            28
#define SA8_SHIFT                                                             4
#define SA8_MASK                                                     0xfffffff0
#define SA8_RD(src)                                   (((src) & 0xfffffff0)>>4)
#define SA8_WR(src)                              (((u32)(src)<<4) & 0xfffffff0)
#define SA8_SET(dst,src) \
                       (((dst) & ~0xfffffff0) | (((u32)(src)<<4) & 0xfffffff0))
/*	 Fields ame	 */
#define AME8_WIDTH                                                            1
#define AME8_SHIFT                                                            0
#define AME8_MASK                                                    0x00000001
#define AME8_RD(src)                                     (((src) & 0x00000001))
#define AME8_WR(src)                                (((u32)(src)) & 0x00000001)
#define AME8_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register end_addr8	*/ 
/*	 Fields sa	 */
#define SA8_F1_WIDTH                                                         28
#define SA8_F1_SHIFT                                                          4
#define SA8_F1_MASK                                                  0xfffffff0
#define SA8_F1_RD(src)                                (((src) & 0xfffffff0)>>4)
#define SA8_F1_WR(src)                           (((u32)(src)<<4) & 0xfffffff0)
#define SA8_F1_SET(dst,src) \
                       (((dst) & ~0xfffffff0) | (((u32)(src)<<4) & 0xfffffff0))
/*	 Fields msn	 */
#define MSN8_WIDTH                                                            4
#define MSN8_SHIFT                                                            0
#define MSN8_MASK                                                    0x0000000f
#define MSN8_RD(src)                                     (((src) & 0x0000000f))
#define MSN8_WR(src)                                (((u32)(src)) & 0x0000000f)
#define MSN8_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register start_addr9	*/ 
/*	 Fields sa	 */
#define SA9_WIDTH                                                            28
#define SA9_SHIFT                                                             4
#define SA9_MASK                                                     0xfffffff0
#define SA9_RD(src)                                   (((src) & 0xfffffff0)>>4)
#define SA9_WR(src)                              (((u32)(src)<<4) & 0xfffffff0)
#define SA9_SET(dst,src) \
                       (((dst) & ~0xfffffff0) | (((u32)(src)<<4) & 0xfffffff0))
/*	 Fields ame	 */
#define AME9_WIDTH                                                            1
#define AME9_SHIFT                                                            0
#define AME9_MASK                                                    0x00000001
#define AME9_RD(src)                                     (((src) & 0x00000001))
#define AME9_WR(src)                                (((u32)(src)) & 0x00000001)
#define AME9_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register end_addr9	*/ 
/*	 Fields sa	 */
#define SA9_F1_WIDTH                                                         28
#define SA9_F1_SHIFT                                                          4
#define SA9_F1_MASK                                                  0xfffffff0
#define SA9_F1_RD(src)                                (((src) & 0xfffffff0)>>4)
#define SA9_F1_WR(src)                           (((u32)(src)<<4) & 0xfffffff0)
#define SA9_F1_SET(dst,src) \
                       (((dst) & ~0xfffffff0) | (((u32)(src)<<4) & 0xfffffff0))
/*	 Fields msn	 */
#define MSN9_WIDTH                                                            4
#define MSN9_SHIFT                                                            0
#define MSN9_MASK                                                    0x0000000f
#define MSN9_RD(src)                                     (((src) & 0x0000000f))
#define MSN9_WR(src)                                (((u32)(src)) & 0x0000000f)
#define MSN9_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register start_addr10	*/ 
/*	 Fields sa	 */
#define SA0_F2_WIDTH                                                         28
#define SA0_F2_SHIFT                                                          4
#define SA0_F2_MASK                                                  0xfffffff0
#define SA0_F2_RD(src)                                (((src) & 0xfffffff0)>>4)
#define SA0_F2_WR(src)                           (((u32)(src)<<4) & 0xfffffff0)
#define SA0_F2_SET(dst,src) \
                       (((dst) & ~0xfffffff0) | (((u32)(src)<<4) & 0xfffffff0))
/*	 Fields ame	 */
#define AME0_F1_WIDTH                                                         1
#define AME0_F1_SHIFT                                                         0
#define AME0_F1_MASK                                                 0x00000001
#define AME0_F1_RD(src)                                  (((src) & 0x00000001))
#define AME0_F1_WR(src)                             (((u32)(src)) & 0x00000001)
#define AME0_F1_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register end_addr10	*/ 
/*	 Fields sa	 */
#define SA0_F3_WIDTH                                                         28
#define SA0_F3_SHIFT                                                          4
#define SA0_F3_MASK                                                  0xfffffff0
#define SA0_F3_RD(src)                                (((src) & 0xfffffff0)>>4)
#define SA0_F3_WR(src)                           (((u32)(src)<<4) & 0xfffffff0)
#define SA0_F3_SET(dst,src) \
                       (((dst) & ~0xfffffff0) | (((u32)(src)<<4) & 0xfffffff0))
/*	 Fields msn	 */
#define MSN0_F1_WIDTH                                                         4
#define MSN0_F1_SHIFT                                                         0
#define MSN0_F1_MASK                                                 0x0000000f
#define MSN0_F1_RD(src)                                  (((src) & 0x0000000f))
#define MSN0_F1_WR(src)                             (((u32)(src)) & 0x0000000f)
#define MSN0_F1_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register start_addr11	*/ 
/*	 Fields sa	 */
#define SA1_F2_WIDTH                                                         28
#define SA1_F2_SHIFT                                                          4
#define SA1_F2_MASK                                                  0xfffffff0
#define SA1_F2_RD(src)                                (((src) & 0xfffffff0)>>4)
#define SA1_F2_WR(src)                           (((u32)(src)<<4) & 0xfffffff0)
#define SA1_F2_SET(dst,src) \
                       (((dst) & ~0xfffffff0) | (((u32)(src)<<4) & 0xfffffff0))
/*	 Fields ame	 */
#define AME1_F1_WIDTH                                                         1
#define AME1_F1_SHIFT                                                         0
#define AME1_F1_MASK                                                 0x00000001
#define AME1_F1_RD(src)                                  (((src) & 0x00000001))
#define AME1_F1_WR(src)                             (((u32)(src)) & 0x00000001)
#define AME1_F1_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register end_addr11	*/ 
/*	 Fields sa	 */
#define SA1_F3_WIDTH                                                         28
#define SA1_F3_SHIFT                                                          4
#define SA1_F3_MASK                                                  0xfffffff0
#define SA1_F3_RD(src)                                (((src) & 0xfffffff0)>>4)
#define SA1_F3_WR(src)                           (((u32)(src)<<4) & 0xfffffff0)
#define SA1_F3_SET(dst,src) \
                       (((dst) & ~0xfffffff0) | (((u32)(src)<<4) & 0xfffffff0))
/*	 Fields msn	 */
#define MSN1_F1_WIDTH                                                         4
#define MSN1_F1_SHIFT                                                         0
#define MSN1_F1_MASK                                                 0x0000000f
#define MSN1_F1_RD(src)                                  (((src) & 0x0000000f))
#define MSN1_F1_WR(src)                             (((u32)(src)) & 0x0000000f)
#define MSN1_F1_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register start_addr12	*/ 
/*	 Fields sa	 */
#define SA2_F2_WIDTH                                                         28
#define SA2_F2_SHIFT                                                          4
#define SA2_F2_MASK                                                  0xfffffff0
#define SA2_F2_RD(src)                                (((src) & 0xfffffff0)>>4)
#define SA2_F2_WR(src)                           (((u32)(src)<<4) & 0xfffffff0)
#define SA2_F2_SET(dst,src) \
                       (((dst) & ~0xfffffff0) | (((u32)(src)<<4) & 0xfffffff0))
/*	 Fields ame	 */
#define AME2_F1_WIDTH                                                         1
#define AME2_F1_SHIFT                                                         0
#define AME2_F1_MASK                                                 0x00000001
#define AME2_F1_RD(src)                                  (((src) & 0x00000001))
#define AME2_F1_WR(src)                             (((u32)(src)) & 0x00000001)
#define AME2_F1_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register end_addr12	*/ 
/*	 Fields sa	 */
#define SA2_F3_WIDTH                                                         28
#define SA2_F3_SHIFT                                                          4
#define SA2_F3_MASK                                                  0xfffffff0
#define SA2_F3_RD(src)                                (((src) & 0xfffffff0)>>4)
#define SA2_F3_WR(src)                           (((u32)(src)<<4) & 0xfffffff0)
#define SA2_F3_SET(dst,src) \
                       (((dst) & ~0xfffffff0) | (((u32)(src)<<4) & 0xfffffff0))
/*	 Fields msn	 */
#define MSN2_F1_WIDTH                                                         4
#define MSN2_F1_SHIFT                                                         0
#define MSN2_F1_MASK                                                 0x0000000f
#define MSN2_F1_RD(src)                                  (((src) & 0x0000000f))
#define MSN2_F1_WR(src)                             (((u32)(src)) & 0x0000000f)
#define MSN2_F1_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register start_addr13	*/ 
/*	 Fields sa	 */
#define SA3_F2_WIDTH                                                         28
#define SA3_F2_SHIFT                                                          4
#define SA3_F2_MASK                                                  0xfffffff0
#define SA3_F2_RD(src)                                (((src) & 0xfffffff0)>>4)
#define SA3_F2_WR(src)                           (((u32)(src)<<4) & 0xfffffff0)
#define SA3_F2_SET(dst,src) \
                       (((dst) & ~0xfffffff0) | (((u32)(src)<<4) & 0xfffffff0))
/*	 Fields ame	 */
#define AME3_F1_WIDTH                                                         1
#define AME3_F1_SHIFT                                                         0
#define AME3_F1_MASK                                                 0x00000001
#define AME3_F1_RD(src)                                  (((src) & 0x00000001))
#define AME3_F1_WR(src)                             (((u32)(src)) & 0x00000001)
#define AME3_F1_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register end_addr13	*/ 
/*	 Fields sa	 */
#define SA3_F3_WIDTH                                                         28
#define SA3_F3_SHIFT                                                          4
#define SA3_F3_MASK                                                  0xfffffff0
#define SA3_F3_RD(src)                                (((src) & 0xfffffff0)>>4)
#define SA3_F3_WR(src)                           (((u32)(src)<<4) & 0xfffffff0)
#define SA3_F3_SET(dst,src) \
                       (((dst) & ~0xfffffff0) | (((u32)(src)<<4) & 0xfffffff0))
/*	 Fields msn	 */
#define MSN3_F1_WIDTH                                                         4
#define MSN3_F1_SHIFT                                                         0
#define MSN3_F1_MASK                                                 0x0000000f
#define MSN3_F1_RD(src)                                  (((src) & 0x0000000f))
#define MSN3_F1_WR(src)                             (((u32)(src)) & 0x0000000f)
#define MSN3_F1_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register start_addr14	*/ 
/*	 Fields sa	 */
#define SA4_F2_WIDTH                                                         28
#define SA4_F2_SHIFT                                                          4
#define SA4_F2_MASK                                                  0xfffffff0
#define SA4_F2_RD(src)                                (((src) & 0xfffffff0)>>4)
#define SA4_F2_WR(src)                           (((u32)(src)<<4) & 0xfffffff0)
#define SA4_F2_SET(dst,src) \
                       (((dst) & ~0xfffffff0) | (((u32)(src)<<4) & 0xfffffff0))
/*	 Fields ame	 */
#define AME4_F1_WIDTH                                                         1
#define AME4_F1_SHIFT                                                         0
#define AME4_F1_MASK                                                 0x00000001
#define AME4_F1_RD(src)                                  (((src) & 0x00000001))
#define AME4_F1_WR(src)                             (((u32)(src)) & 0x00000001)
#define AME4_F1_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register end_addr14	*/ 
/*	 Fields sa	 */
#define SA4_F3_WIDTH                                                         28
#define SA4_F3_SHIFT                                                          4
#define SA4_F3_MASK                                                  0xfffffff0
#define SA4_F3_RD(src)                                (((src) & 0xfffffff0)>>4)
#define SA4_F3_WR(src)                           (((u32)(src)<<4) & 0xfffffff0)
#define SA4_F3_SET(dst,src) \
                       (((dst) & ~0xfffffff0) | (((u32)(src)<<4) & 0xfffffff0))
/*	 Fields msn	 */
#define MSN4_F1_WIDTH                                                         4
#define MSN4_F1_SHIFT                                                         0
#define MSN4_F1_MASK                                                 0x0000000f
#define MSN4_F1_RD(src)                                  (((src) & 0x0000000f))
#define MSN4_F1_WR(src)                             (((u32)(src)) & 0x0000000f)
#define MSN4_F1_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register start_addr15	*/ 
/*	 Fields sa	 */
#define SA5_F2_WIDTH                                                         28
#define SA5_F2_SHIFT                                                          4
#define SA5_F2_MASK                                                  0xfffffff0
#define SA5_F2_RD(src)                                (((src) & 0xfffffff0)>>4)
#define SA5_F2_WR(src)                           (((u32)(src)<<4) & 0xfffffff0)
#define SA5_F2_SET(dst,src) \
                       (((dst) & ~0xfffffff0) | (((u32)(src)<<4) & 0xfffffff0))
/*	 Fields ame	 */
#define AME5_F1_WIDTH                                                         1
#define AME5_F1_SHIFT                                                         0
#define AME5_F1_MASK                                                 0x00000001
#define AME5_F1_RD(src)                                  (((src) & 0x00000001))
#define AME5_F1_WR(src)                             (((u32)(src)) & 0x00000001)
#define AME5_F1_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register end_addr15	*/ 
/*	 Fields sa	 */
#define SA5_F3_WIDTH                                                         28
#define SA5_F3_SHIFT                                                          4
#define SA5_F3_MASK                                                  0xfffffff0
#define SA5_F3_RD(src)                                (((src) & 0xfffffff0)>>4)
#define SA5_F3_WR(src)                           (((u32)(src)<<4) & 0xfffffff0)
#define SA5_F3_SET(dst,src) \
                       (((dst) & ~0xfffffff0) | (((u32)(src)<<4) & 0xfffffff0))
/*	 Fields msn	 */
#define MSN5_F1_WIDTH                                                         4
#define MSN5_F1_SHIFT                                                         0
#define MSN5_F1_MASK                                                 0x0000000f
#define MSN5_F1_RD(src)                                  (((src) & 0x0000000f))
#define MSN5_F1_WR(src)                             (((u32)(src)) & 0x0000000f)
#define MSN5_F1_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register start_addr16	*/ 
/*	 Fields sa	 */
#define SA6_F2_WIDTH                                                         28
#define SA6_F2_SHIFT                                                          4
#define SA6_F2_MASK                                                  0xfffffff0
#define SA6_F2_RD(src)                                (((src) & 0xfffffff0)>>4)
#define SA6_F2_WR(src)                           (((u32)(src)<<4) & 0xfffffff0)
#define SA6_F2_SET(dst,src) \
                       (((dst) & ~0xfffffff0) | (((u32)(src)<<4) & 0xfffffff0))
/*	 Fields ame	 */
#define AME6_F1_WIDTH                                                         1
#define AME6_F1_SHIFT                                                         0
#define AME6_F1_MASK                                                 0x00000001
#define AME6_F1_RD(src)                                  (((src) & 0x00000001))
#define AME6_F1_WR(src)                             (((u32)(src)) & 0x00000001)
#define AME6_F1_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register end_addr16	*/ 
/*	 Fields sa	 */
#define SA6_F3_WIDTH                                                         28
#define SA6_F3_SHIFT                                                          4
#define SA6_F3_MASK                                                  0xfffffff0
#define SA6_F3_RD(src)                                (((src) & 0xfffffff0)>>4)
#define SA6_F3_WR(src)                           (((u32)(src)<<4) & 0xfffffff0)
#define SA6_F3_SET(dst,src) \
                       (((dst) & ~0xfffffff0) | (((u32)(src)<<4) & 0xfffffff0))
/*	 Fields msn	 */
#define MSN6_F1_WIDTH                                                         4
#define MSN6_F1_SHIFT                                                         0
#define MSN6_F1_MASK                                                 0x0000000f
#define MSN6_F1_RD(src)                                  (((src) & 0x0000000f))
#define MSN6_F1_WR(src)                             (((u32)(src)) & 0x0000000f)
#define MSN6_F1_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register core_id_reg	*/ 
/*	 Fields core_id	 */
#define CORE_ID_WIDTH                                                        32
#define CORE_ID_SHIFT                                                         0
#define CORE_ID_MASK                                                 0xffffffff
#define CORE_ID_RD(src)                                  (((src) & 0xffffffff))
#define CORE_ID_WR(src)                             (((u32)(src)) & 0xffffffff)
#define CORE_ID_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register debug_test_reg	*/ 
/*	 Fields CfgSetRsvBitoB1	 */
#define CFGSETRSVBITOB1_WIDTH                                                 1
#define CFGSETRSVBITOB1_SHIFT                                                11
#define CFGSETRSVBITOB1_MASK                                         0x00000800
#define CFGSETRSVBITOB1_RD(src)                      (((src) & 0x00000800)>>11)
#define CFGSETRSVBITOB1_WR(src)                 (((u32)(src)<<11) & 0x00000800)
#define CFGSETRSVBITOB1_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*	 Fields RMEB	 */
#define LCD_RMEB_WIDTH                                                       1
#define LCD_RMEB_SHIFT                                                      10
#define LCD_RMEB_MASK                                               0x00000400
#define LCD_RMEB_RD(src)                            (((src) & 0x00000400)>>10)
#define LCD_RMEB_WR(src)                       (((u32)(src)<<10) & 0x00000400)
#define LCD_RMEB_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields RMEA	 */
#define LCD_RMEA_WIDTH                                                       1
#define LCD_RMEA_SHIFT                                                       9
#define LCD_RMEA_MASK                                               0x00000200
#define LCD_RMEA_RD(src)                             (((src) & 0x00000200)>>9)
#define LCD_RMEA_WR(src)                        (((u32)(src)<<9) & 0x00000200)
#define LCD_RMEA_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields RMB	 */
#define LCD_RMB_WIDTH                                                        2
#define LCD_RMB_SHIFT                                                        7
#define LCD_RMB_MASK                                                0x00000180
#define LCD_RMB_RD(src)                              (((src) & 0x00000180)>>7)
#define LCD_RMB_WR(src)                         (((u32)(src)<<7) & 0x00000180)
#define LCD_RMB_SET(dst,src) \
                       (((dst) & ~0x00000180) | (((u32)(src)<<7) & 0x00000180))
/*	 Fields RMA	 */
#define LCD_RMA_WIDTH                                                        2
#define LCD_RMA_SHIFT                                                        5
#define LCD_RMA_MASK                                                0x00000060
#define LCD_RMA_RD(src)                              (((src) & 0x00000060)>>5)
#define LCD_RMA_WR(src)                         (((u32)(src)<<5) & 0x00000060)
#define LCD_RMA_SET(dst,src) \
                       (((dst) & ~0x00000060) | (((u32)(src)<<5) & 0x00000060))
/*	 Fields CfgNoInvert	 */
#define CFGNOINVERT_WIDTH                                                     1
#define CFGNOINVERT_SHIFT                                                     4
#define CFGNOINVERT_MASK                                             0x00000010
#define CFGNOINVERT_RD(src)                           (((src) & 0x00000010)>>4)
#define CFGNOINVERT_WR(src)                      (((u32)(src)<<4) & 0x00000010)
#define CFGNOINVERT_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields CfgSwapWord	 */
#define CFGSWAPWORD_WIDTH                                                     1
#define CFGSWAPWORD_SHIFT                                                     3
#define CFGSWAPWORD_MASK                                             0x00000008
#define CFGSWAPWORD_RD(src)                           (((src) & 0x00000008)>>3)
#define CFGSWAPWORD_WR(src)                      (((u32)(src)<<3) & 0x00000008)
#define CFGSWAPWORD_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields CfgMode	 */
#define LCD_CFGMODE_WIDTH                                                    1
#define LCD_CFGMODE_SHIFT                                                    2
#define LCD_CFGMODE_MASK                                            0x00000004
#define LCD_CFGMODE_RD(src)                          (((src) & 0x00000004)>>2)
#define LCD_CFGMODE_WR(src)                     (((u32)(src)<<2) & 0x00000004)
#define LCD_CFGMODE_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields CfgRes	 */
#define CFGRES_WIDTH                                                          1
#define CFGRES_SHIFT                                                          1
#define CFGRES_MASK                                                  0x00000002
#define CFGRES_RD(src)                                (((src) & 0x00000002)>>1)
#define CFGRES_WR(src)                           (((u32)(src)<<1) & 0x00000002)
#define CFGRES_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields sw_irq	 */
#define SW_IRQ_WIDTH                                                          1
#define SW_IRQ_SHIFT                                                          0
#define SW_IRQ_MASK                                                  0x00000001
#define SW_IRQ_RD(src)                                   (((src) & 0x00000001))
#define SW_IRQ_WR(src)                              (((u32)(src)) & 0x00000001)
#define SW_IRQ_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register lcdc_cr1	*/ 
/*	 Fields dfe	 */
#define DFE1_WIDTH                                                            1
#define DFE1_SHIFT                                                           23
#define DFE1_MASK                                                    0x00800000
#define DFE1_RD(src)                                 (((src) & 0x00800000)>>23)
#define DFE1_WR(src)                            (((u32)(src)<<23) & 0x00800000)
#define DFE1_SET(dst,src) \
                      (((dst) & ~0x00800000) | (((u32)(src)<<23) & 0x00800000))
/*	 Fields dfb	 */
#define DFB1_WIDTH                                                            1
#define DFB1_SHIFT                                                           22
#define DFB1_MASK                                                    0x00400000
#define DFB1_RD(src)                                 (((src) & 0x00400000)>>22)
#define DFB1_WR(src)                            (((u32)(src)<<22) & 0x00400000)
#define DFB1_SET(dst,src) \
                      (((dst) & ~0x00400000) | (((u32)(src)<<22) & 0x00400000))
/*	 Fields dfr	 */
#define DFR1_WIDTH                                                            1
#define DFR1_SHIFT                                                           21
#define DFR1_MASK                                                    0x00200000
#define DFR1_RD(src)                                 (((src) & 0x00200000)>>21)
#define DFR1_WR(src)                            (((u32)(src)<<21) & 0x00200000)
#define DFR1_SET(dst,src) \
                      (((dst) & ~0x00200000) | (((u32)(src)<<21) & 0x00200000))
/*	 Fields dee	 */
#define DEE1_WIDTH                                                            1
#define DEE1_SHIFT                                                           20
#define DEE1_MASK                                                    0x00100000
#define DEE1_RD(src)                                 (((src) & 0x00100000)>>20)
#define DEE1_WR(src)                            (((u32)(src)<<20) & 0x00100000)
#define DEE1_SET(dst,src) \
                      (((dst) & ~0x00100000) | (((u32)(src)<<20) & 0x00100000))
/*	 Fields fbp	 */
#define FBP1_WIDTH                                                            1
#define FBP1_SHIFT                                                           19
#define FBP1_MASK                                                    0x00080000
#define FBP1_RD(src)                                 (((src) & 0x00080000)>>19)
#define FBP1_WR(src)                            (((u32)(src)<<19) & 0x00080000)
#define FBP1_SET(dst,src) \
                      (((dst) & ~0x00080000) | (((u32)(src)<<19) & 0x00080000))
/*	 Fields lps	 */
#define LPS1_WIDTH                                                            1
#define LPS1_SHIFT                                                           18
#define LPS1_MASK                                                    0x00040000
#define LPS1_RD(src)                                 (((src) & 0x00040000)>>18)
#define LPS1_WR(src)                            (((u32)(src)<<18) & 0x00040000)
#define LPS1_SET(dst,src) \
                      (((dst) & ~0x00040000) | (((u32)(src)<<18) & 0x00040000))
/*	 Fields fdw	 */
#define FDW1_WIDTH                                                            2
#define FDW1_SHIFT                                                           16
#define FDW1_MASK                                                    0x00030000
#define FDW1_RD(src)                                 (((src) & 0x00030000)>>16)
#define FDW1_WR(src)                            (((u32)(src)<<16) & 0x00030000)
#define FDW1_SET(dst,src) \
                      (((dst) & ~0x00030000) | (((u32)(src)<<16) & 0x00030000))
/*	 Fields pss	 */
#define PSS1_WIDTH                                                            1
#define PSS1_SHIFT                                                           15
#define PSS1_MASK                                                    0x00008000
#define PSS1_RD(src)                                 (((src) & 0x00008000)>>15)
#define PSS1_WR(src)                            (((u32)(src)<<15) & 0x00008000)
#define PSS1_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))
/*	 Fields ops	 */
#define OPS1_WIDTH                                                            3
#define OPS1_SHIFT                                                           12
#define OPS1_MASK                                                    0x00007000
#define OPS1_RD(src)                                 (((src) & 0x00007000)>>12)
#define OPS1_WR(src)                            (((u32)(src)<<12) & 0x00007000)
#define OPS1_SET(dst,src) \
                      (((dst) & ~0x00007000) | (((u32)(src)<<12) & 0x00007000))
/*	 Fields vsp	 */
#define VSP1_WIDTH                                                            1
#define VSP1_SHIFT                                                           11
#define VSP1_MASK                                                    0x00000800
#define VSP1_RD(src)                                 (((src) & 0x00000800)>>11)
#define VSP1_WR(src)                            (((u32)(src)<<11) & 0x00000800)
#define VSP1_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*	 Fields hsp	 */
#define HSP1_WIDTH                                                            1
#define HSP1_SHIFT                                                           10
#define HSP1_MASK                                                    0x00000400
#define HSP1_RD(src)                                 (((src) & 0x00000400)>>10)
#define HSP1_WR(src)                            (((u32)(src)<<10) & 0x00000400)
#define HSP1_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields pcp	 */
#define PCP1_WIDTH                                                            1
#define PCP1_SHIFT                                                            9
#define PCP1_MASK                                                    0x00000200
#define PCP1_RD(src)                                  (((src) & 0x00000200)>>9)
#define PCP1_WR(src)                             (((u32)(src)<<9) & 0x00000200)
#define PCP1_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields dep	 */
#define DEP1_WIDTH                                                            1
#define DEP1_SHIFT                                                            8
#define DEP1_MASK                                                    0x00000100
#define DEP1_RD(src)                                  (((src) & 0x00000100)>>8)
#define DEP1_WR(src)                             (((u32)(src)<<8) & 0x00000100)
#define DEP1_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*	 Fields ebo	 */
#define EBO1_WIDTH                                                            1
#define EBO1_SHIFT                                                            7
#define EBO1_MASK                                                    0x00000080
#define EBO1_RD(src)                                  (((src) & 0x00000080)>>7)
#define EBO1_WR(src)                             (((u32)(src)<<7) & 0x00000080)
#define EBO1_SET(dst,src) \
                       (((dst) & ~0x00000080) | (((u32)(src)<<7) & 0x00000080))
/*	 Fields epo	 */
#define EPO1_WIDTH                                                            1
#define EPO1_SHIFT                                                            6
#define EPO1_MASK                                                    0x00000040
#define EPO1_RD(src)                                  (((src) & 0x00000040)>>6)
#define EPO1_WR(src)                             (((u32)(src)<<6) & 0x00000040)
#define EPO1_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*	 Fields rgb	 */
#define RGB1_WIDTH                                                            1
#define RGB1_SHIFT                                                            5
#define RGB1_MASK                                                    0x00000020
#define RGB1_RD(src)                                  (((src) & 0x00000020)>>5)
#define RGB1_WR(src)                             (((u32)(src)<<5) & 0x00000020)
#define RGB1_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields bpp	 */
#define BPP1_WIDTH                                                            3
#define BPP1_SHIFT                                                            2
#define BPP1_MASK                                                    0x0000001c
#define BPP1_RD(src)                                  (((src) & 0x0000001c)>>2)
#define BPP1_WR(src)                             (((u32)(src)<<2) & 0x0000001c)
#define BPP1_SET(dst,src) \
                       (((dst) & ~0x0000001c) | (((u32)(src)<<2) & 0x0000001c))
/*	 Fields lpe	 */
#define LPE1_WIDTH                                                            1
#define LPE1_SHIFT                                                            1
#define LPE1_MASK                                                    0x00000002
#define LPE1_RD(src)                                  (((src) & 0x00000002)>>1)
#define LPE1_WR(src)                             (((u32)(src)<<1) & 0x00000002)
#define LPE1_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields lce	 */
#define LCE1_WIDTH                                                            1
#define LCE1_SHIFT                                                            0
#define LCE1_MASK                                                    0x00000001
#define LCE1_RD(src)                                     (((src) & 0x00000001))
#define LCE1_WR(src)                                (((u32)(src)) & 0x00000001)
#define LCE1_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register lcdc_htr	*/ 
/*	 Fields hsw	 */
#define HSW_WIDTH                                                             8
#define HSW_SHIFT                                                            24
#define HSW_MASK                                                     0xff000000
#define HSW_RD(src)                                  (((src) & 0xff000000)>>24)
#define HSW_WR(src)                             (((u32)(src)<<24) & 0xff000000)
#define HSW_SET(dst,src) \
                      (((dst) & ~0xff000000) | (((u32)(src)<<24) & 0xff000000))
/*	 Fields hbp	 */
#define HBP_WIDTH                                                             8
#define HBP_SHIFT                                                            16
#define HBP_MASK                                                     0x00ff0000
#define HBP_RD(src)                                  (((src) & 0x00ff0000)>>16)
#define HBP_WR(src)                             (((u32)(src)<<16) & 0x00ff0000)
#define HBP_SET(dst,src) \
                      (((dst) & ~0x00ff0000) | (((u32)(src)<<16) & 0x00ff0000))
/*	 Fields ppl	 */
#define PPL_WIDTH                                                             8
#define PPL_SHIFT                                                             8
#define PPL_MASK                                                     0x0000ff00
#define PPL_RD(src)                                   (((src) & 0x0000ff00)>>8)
#define PPL_WR(src)                              (((u32)(src)<<8) & 0x0000ff00)
#define PPL_SET(dst,src) \
                       (((dst) & ~0x0000ff00) | (((u32)(src)<<8) & 0x0000ff00))
/*	 Fields hfp	 */
#define HFP_WIDTH                                                             8
#define HFP_SHIFT                                                             0
#define HFP_MASK                                                     0x000000ff
#define HFP_RD(src)                                      (((src) & 0x000000ff))
#define HFP_WR(src)                                 (((u32)(src)) & 0x000000ff)
#define HFP_SET(dst,src) (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register lcdc_vtr1	*/ 
/*	 Fields vbp	 */
#define VBP1_WIDTH                                                            8
#define VBP1_SHIFT                                                           16
#define VBP1_MASK                                                    0x00ff0000
#define VBP1_RD(src)                                 (((src) & 0x00ff0000)>>16)
#define VBP1_WR(src)                            (((u32)(src)<<16) & 0x00ff0000)
#define VBP1_SET(dst,src) \
                      (((dst) & ~0x00ff0000) | (((u32)(src)<<16) & 0x00ff0000))
/*	 Fields vfp	 */
#define VFP1_WIDTH                                                            8
#define VFP1_SHIFT                                                            8
#define VFP1_MASK                                                    0x0000ff00
#define VFP1_RD(src)                                  (((src) & 0x0000ff00)>>8)
#define VFP1_WR(src)                             (((u32)(src)<<8) & 0x0000ff00)
#define VFP1_SET(dst,src) \
                       (((dst) & ~0x0000ff00) | (((u32)(src)<<8) & 0x0000ff00))
/*	 Fields vsw	 */
#define VSW1_WIDTH                                                            8
#define VSW1_SHIFT                                                            0
#define VSW1_MASK                                                    0x000000ff
#define VSW1_RD(src)                                     (((src) & 0x000000ff))
#define VSW1_WR(src)                                (((u32)(src)) & 0x000000ff)
#define VSW1_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register lcdc_vtr2	*/ 
/*	 Fields lpp	 */
#define LPP2_WIDTH                                                           12
#define LPP2_SHIFT                                                            0
#define LPP2_MASK                                                    0x00000fff
#define LPP2_RD(src)                                     (((src) & 0x00000fff))
#define LPP2_WR(src)                                (((u32)(src)) & 0x00000fff)
#define LPP2_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lcdc_pctr	*/ 
/*	 Fields pci	 */
#define PCI_WIDTH                                                             1
#define PCI_SHIFT                                                             9
#define PCI_MASK                                                     0x00000200
#define PCI_RD(src)                                   (((src) & 0x00000200)>>9)
#define PCI_WR(src)                              (((u32)(src)<<9) & 0x00000200)
#define PCI_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields pcb	 */
#define PCB_WIDTH                                                             1
#define PCB_SHIFT                                                             8
#define PCB_MASK                                                     0x00000100
#define PCB_RD(src)                                   (((src) & 0x00000100)>>8)
#define PCB_WR(src)                              (((u32)(src)<<8) & 0x00000100)
#define PCB_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*	 Fields pcd	 */
#define PCD_WIDTH                                                             8
#define PCD_SHIFT                                                             0
#define PCD_MASK                                                     0x000000ff
#define PCD_RD(src)                                      (((src) & 0x000000ff))
#define PCD_WR(src)                                 (((u32)(src)) & 0x000000ff)
#define PCD_SET(dst,src) (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register lcdc_isr	*/ 
/*	 Fields flc	 */
#define FLC_WIDTH                                                             1
#define FLC_SHIFT                                                            14
#define FLC_MASK                                                     0x00004000
#define FLC_RD(src)                                  (((src) & 0x00004000)>>14)
#define FLC_SET(dst,src) \
                      (((dst) & ~0x00004000) | (((u32)(src)<<14) & 0x00004000))
/*	 Fields fnc	 */
#define FNC_WIDTH                                                             1
#define FNC_SHIFT                                                            13
#define FNC_MASK                                                     0x00002000
#define FNC_RD(src)                                  (((src) & 0x00002000)>>13)
#define FNC_SET(dst,src) \
                      (((dst) & ~0x00002000) | (((u32)(src)<<13) & 0x00002000))
/*	 Fields fbe	 */
#define FBE_WIDTH                                                             1
#define FBE_SHIFT                                                            12
#define FBE_MASK                                                     0x00001000
#define FBE_RD(src)                                  (((src) & 0x00001000)>>12)
#define FBE_SET(dst,src) \
                      (((dst) & ~0x00001000) | (((u32)(src)<<12) & 0x00001000))
/*	 Fields ars	 */
#define ARS_WIDTH                                                             1
#define ARS_SHIFT                                                            11
#define ARS_MASK                                                     0x00000800
#define ARS_RD(src)                                  (((src) & 0x00000800)>>11)
#define ARS_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*	 Fields ari	 */
#define ARI_WIDTH                                                             1
#define ARI_SHIFT                                                            10
#define ARI_MASK                                                     0x00000400
#define ARI_RD(src)                                  (((src) & 0x00000400)>>10)
#define ARI_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields abl	 */
#define ABL_WIDTH                                                             1
#define ABL_SHIFT                                                             9
#define ABL_MASK                                                     0x00000200
#define ABL_RD(src)                                   (((src) & 0x00000200)>>9)
#define ABL_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields ldd	 */
#define LDD_WIDTH                                                             1
#define LDD_SHIFT                                                             8
#define LDD_MASK                                                     0x00000100
#define LDD_RD(src)                                   (((src) & 0x00000100)>>8)
#define LDD_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*	 Fields bau	 */
#define BAU_WIDTH                                                             1
#define BAU_SHIFT                                                             7
#define BAU_MASK                                                     0x00000080
#define BAU_RD(src)                                   (((src) & 0x00000080)>>7)
#define BAU_SET(dst,src) \
                       (((dst) & ~0x00000080) | (((u32)(src)<<7) & 0x00000080))
/*	 Fields vct	 */
#define VCT_WIDTH                                                             1
#define VCT_SHIFT                                                             6
#define VCT_MASK                                                     0x00000040
#define VCT_RD(src)                                   (((src) & 0x00000040)>>6)
#define VCT_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*	 Fields mbe	 */
#define MBE_WIDTH                                                             1
#define MBE_SHIFT                                                             5
#define MBE_MASK                                                     0x00000020
#define MBE_RD(src)                                   (((src) & 0x00000020)>>5)
#define MBE_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields fer	 */
#define FER_WIDTH                                                             1
#define FER_SHIFT                                                             4
#define FER_MASK                                                     0x00000010
#define FER_RD(src)                                   (((src) & 0x00000010)>>4)
#define FER_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields ifo	 */
#define IFO_WIDTH                                                             1
#define IFO_SHIFT                                                             3
#define IFO_MASK                                                     0x00000008
#define IFO_RD(src)                                   (((src) & 0x00000008)>>3)
#define IFO_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields ifu	 */
#define IFU_WIDTH                                                             1
#define IFU_SHIFT                                                             2
#define IFU_MASK                                                     0x00000004
#define IFU_RD(src)                                   (((src) & 0x00000004)>>2)
#define IFU_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields ofo	 */
#define OFO_WIDTH                                                             1
#define OFO_SHIFT                                                             1
#define OFO_MASK                                                     0x00000002
#define OFO_RD(src)                                   (((src) & 0x00000002)>>1)
#define OFO_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields ofu	 */
#define OFU_WIDTH                                                             1
#define OFU_SHIFT                                                             0
#define OFU_MASK                                                     0x00000001
#define OFU_RD(src)                                      (((src) & 0x00000001))
#define OFU_SET(dst,src) (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register lcdc_imr	*/ 
/*	 Fields flcm	 */
#define FLCM_WIDTH                                                            1
#define FLCM_SHIFT                                                           14
#define FLCM_MASK                                                    0x00004000
#define FLCM_RD(src)                                 (((src) & 0x00004000)>>14)
#define FLCM_WR(src)                            (((u32)(src)<<14) & 0x00004000)
#define FLCM_SET(dst,src) \
                      (((dst) & ~0x00004000) | (((u32)(src)<<14) & 0x00004000))
/*	 Fields fncm	 */
#define FNCM_WIDTH                                                            1
#define FNCM_SHIFT                                                           13
#define FNCM_MASK                                                    0x00002000
#define FNCM_RD(src)                                 (((src) & 0x00002000)>>13)
#define FNCM_WR(src)                            (((u32)(src)<<13) & 0x00002000)
#define FNCM_SET(dst,src) \
                      (((dst) & ~0x00002000) | (((u32)(src)<<13) & 0x00002000))
/*	 Fields fbem	 */
#define FBEM_WIDTH                                                            1
#define FBEM_SHIFT                                                           12
#define FBEM_MASK                                                    0x00001000
#define FBEM_RD(src)                                 (((src) & 0x00001000)>>12)
#define FBEM_WR(src)                            (((u32)(src)<<12) & 0x00001000)
#define FBEM_SET(dst,src) \
                      (((dst) & ~0x00001000) | (((u32)(src)<<12) & 0x00001000))
/*	 Fields arsm	 */
#define ARSM_WIDTH                                                            1
#define ARSM_SHIFT                                                           11
#define ARSM_MASK                                                    0x00000800
#define ARSM_RD(src)                                 (((src) & 0x00000800)>>11)
#define ARSM_WR(src)                            (((u32)(src)<<11) & 0x00000800)
#define ARSM_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*	 Fields arim	 */
#define ARIM_WIDTH                                                            1
#define ARIM_SHIFT                                                           10
#define ARIM_MASK                                                    0x00000400
#define ARIM_RD(src)                                 (((src) & 0x00000400)>>10)
#define ARIM_WR(src)                            (((u32)(src)<<10) & 0x00000400)
#define ARIM_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields ablm	 */
#define ABLM_WIDTH                                                            1
#define ABLM_SHIFT                                                            9
#define ABLM_MASK                                                    0x00000200
#define ABLM_RD(src)                                  (((src) & 0x00000200)>>9)
#define ABLM_WR(src)                             (((u32)(src)<<9) & 0x00000200)
#define ABLM_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields lddm	 */
#define LDDM_WIDTH                                                            1
#define LDDM_SHIFT                                                            8
#define LDDM_MASK                                                    0x00000100
#define LDDM_RD(src)                                  (((src) & 0x00000100)>>8)
#define LDDM_WR(src)                             (((u32)(src)<<8) & 0x00000100)
#define LDDM_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*	 Fields baum	 */
#define BAUM_WIDTH                                                            1
#define BAUM_SHIFT                                                            7
#define BAUM_MASK                                                    0x00000080
#define BAUM_RD(src)                                  (((src) & 0x00000080)>>7)
#define BAUM_WR(src)                             (((u32)(src)<<7) & 0x00000080)
#define BAUM_SET(dst,src) \
                       (((dst) & ~0x00000080) | (((u32)(src)<<7) & 0x00000080))
/*	 Fields vctm	 */
#define VCTM_WIDTH                                                            1
#define VCTM_SHIFT                                                            6
#define VCTM_MASK                                                    0x00000040
#define VCTM_RD(src)                                  (((src) & 0x00000040)>>6)
#define VCTM_WR(src)                             (((u32)(src)<<6) & 0x00000040)
#define VCTM_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*	 Fields mbem	 */
#define MBEM_WIDTH                                                            1
#define MBEM_SHIFT                                                            5
#define MBEM_MASK                                                    0x00000020
#define MBEM_RD(src)                                  (((src) & 0x00000020)>>5)
#define MBEM_WR(src)                             (((u32)(src)<<5) & 0x00000020)
#define MBEM_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields ferm	 */
#define FERM_WIDTH                                                            1
#define FERM_SHIFT                                                            4
#define FERM_MASK                                                    0x00000010
#define FERM_RD(src)                                  (((src) & 0x00000010)>>4)
#define FERM_WR(src)                             (((u32)(src)<<4) & 0x00000010)
#define FERM_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields ifom	 */
#define IFOM_WIDTH                                                            1
#define IFOM_SHIFT                                                            3
#define IFOM_MASK                                                    0x00000008
#define IFOM_RD(src)                                  (((src) & 0x00000008)>>3)
#define IFOM_WR(src)                             (((u32)(src)<<3) & 0x00000008)
#define IFOM_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields ifum	 */
#define IFUM_WIDTH                                                            1
#define IFUM_SHIFT                                                            2
#define IFUM_MASK                                                    0x00000004
#define IFUM_RD(src)                                  (((src) & 0x00000004)>>2)
#define IFUM_WR(src)                             (((u32)(src)<<2) & 0x00000004)
#define IFUM_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields ofom	 */
#define OFOM_WIDTH                                                            1
#define OFOM_SHIFT                                                            1
#define OFOM_MASK                                                    0x00000002
#define OFOM_RD(src)                                  (((src) & 0x00000002)>>1)
#define OFOM_WR(src)                             (((u32)(src)<<1) & 0x00000002)
#define OFOM_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields ofum	 */
#define OFUM_WIDTH                                                            1
#define OFUM_SHIFT                                                            0
#define OFUM_MASK                                                    0x00000001
#define OFUM_RD(src)                                     (((src) & 0x00000001))
#define OFUM_WR(src)                                (((u32)(src)) & 0x00000001)
#define OFUM_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register lcdc_ivr	*/ 
/*	 Fields flc	 */
#define FLC_F1_WIDTH                                                          1
#define FLC_F1_SHIFT                                                         14
#define FLC_F1_MASK                                                  0x00004000
#define FLC_F1_RD(src)                               (((src) & 0x00004000)>>14)
#define FLC_F1_SET(dst,src) \
                      (((dst) & ~0x00004000) | (((u32)(src)<<14) & 0x00004000))
/*	 Fields fnc	 */
#define FNC_F1_WIDTH                                                          1
#define FNC_F1_SHIFT                                                         13
#define FNC_F1_MASK                                                  0x00002000
#define FNC_F1_RD(src)                               (((src) & 0x00002000)>>13)
#define FNC_F1_SET(dst,src) \
                      (((dst) & ~0x00002000) | (((u32)(src)<<13) & 0x00002000))
/*	 Fields fbe	 */
#define FBE_F1_WIDTH                                                          1
#define FBE_F1_SHIFT                                                         12
#define FBE_F1_MASK                                                  0x00001000
#define FBE_F1_RD(src)                               (((src) & 0x00001000)>>12)
#define FBE_F1_SET(dst,src) \
                      (((dst) & ~0x00001000) | (((u32)(src)<<12) & 0x00001000))
/*	 Fields arsv	 */
#define ARSV_WIDTH                                                            1
#define ARSV_SHIFT                                                           11
#define ARSV_MASK                                                    0x00000800
#define ARSV_RD(src)                                 (((src) & 0x00000800)>>11)
#define ARSV_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*	 Fields ariv	 */
#define ARIV_WIDTH                                                            1
#define ARIV_SHIFT                                                           10
#define ARIV_MASK                                                    0x00000400
#define ARIV_RD(src)                                 (((src) & 0x00000400)>>10)
#define ARIV_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields ablv	 */
#define ABLV_WIDTH                                                            1
#define ABLV_SHIFT                                                            9
#define ABLV_MASK                                                    0x00000200
#define ABLV_RD(src)                                  (((src) & 0x00000200)>>9)
#define ABLV_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields lddv	 */
#define LDDV_WIDTH                                                            1
#define LDDV_SHIFT                                                            8
#define LDDV_MASK                                                    0x00000100
#define LDDV_RD(src)                                  (((src) & 0x00000100)>>8)
#define LDDV_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*	 Fields bauv	 */
#define BAUV_WIDTH                                                            1
#define BAUV_SHIFT                                                            7
#define BAUV_MASK                                                    0x00000080
#define BAUV_RD(src)                                  (((src) & 0x00000080)>>7)
#define BAUV_SET(dst,src) \
                       (((dst) & ~0x00000080) | (((u32)(src)<<7) & 0x00000080))
/*	 Fields vctv	 */
#define VCTV_WIDTH                                                            1
#define VCTV_SHIFT                                                            6
#define VCTV_MASK                                                    0x00000040
#define VCTV_RD(src)                                  (((src) & 0x00000040)>>6)
#define VCTV_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*	 Fields mbev	 */
#define MBEV_WIDTH                                                            1
#define MBEV_SHIFT                                                            5
#define MBEV_MASK                                                    0x00000020
#define MBEV_RD(src)                                  (((src) & 0x00000020)>>5)
#define MBEV_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields ferv	 */
#define FERV_WIDTH                                                            1
#define FERV_SHIFT                                                            4
#define FERV_MASK                                                    0x00000010
#define FERV_RD(src)                                  (((src) & 0x00000010)>>4)
#define FERV_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields ifov	 */
#define IFOV_WIDTH                                                            1
#define IFOV_SHIFT                                                            3
#define IFOV_MASK                                                    0x00000008
#define IFOV_RD(src)                                  (((src) & 0x00000008)>>3)
#define IFOV_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields ifuv	 */
#define IFUV_WIDTH                                                            1
#define IFUV_SHIFT                                                            2
#define IFUV_MASK                                                    0x00000004
#define IFUV_RD(src)                                  (((src) & 0x00000004)>>2)
#define IFUV_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields ofov	 */
#define OFOV_WIDTH                                                            1
#define OFOV_SHIFT                                                            1
#define OFOV_MASK                                                    0x00000002
#define OFOV_RD(src)                                  (((src) & 0x00000002)>>1)
#define OFOV_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields ofuv	 */
#define OFUV_WIDTH                                                            1
#define OFUV_SHIFT                                                            0
#define OFUV_MASK                                                    0x00000001
#define OFUV_RD(src)                                     (((src) & 0x00000001))
#define OFUV_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register lcdc_iscr	*/ 
/*	 Fields vsc	 */
#define VSC_WIDTH                                                             3
#define VSC_SHIFT                                                             0
#define VSC_MASK                                                     0x00000007
#define VSC_RD(src)                                      (((src) & 0x00000007))
#define VSC_WR(src)                                 (((u32)(src)) & 0x00000007)
#define VSC_SET(dst,src) (((dst) & ~0x00000007) | (((u32)(src)) & 0x00000007))

/*	Register lcdc_dbar	*/ 
/*	 Fields dbar	 */
#define DBAR_WIDTH                                                           32
#define DBAR_SHIFT                                                            0
#define DBAR_MASK                                                    0xffffffff
#define DBAR_RD(src)                                     (((src) & 0xffffffff))
#define DBAR_WR(src)                                (((u32)(src)) & 0xffffffff)
#define DBAR_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register lcdc_dcar	*/ 
/*	 Fields dcar	 */
#define DCAR_WIDTH                                                           32
#define DCAR_SHIFT                                                            0
#define DCAR_MASK                                                    0xffffffff
#define DCAR_RD(src)                                     (((src) & 0xffffffff))
#define DCAR_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register lcdc_dear	*/ 
/*	 Fields dear	 */
#define DEAR_WIDTH                                                           32
#define DEAR_SHIFT                                                            0
#define DEAR_MASK                                                    0xffffffff
#define DEAR_RD(src)                                     (((src) & 0xffffffff))
#define DEAR_WR(src)                                (((u32)(src)) & 0xffffffff)
#define DEAR_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register lcdc_pwmfr	*/ 
/*	 Fields pwmfci	 */
#define PWMFCI_WIDTH                                                          1
#define PWMFCI_SHIFT                                                         21
#define PWMFCI_MASK                                                  0x00200000
#define PWMFCI_RD(src)                               (((src) & 0x00200000)>>21)
#define PWMFCI_WR(src)                          (((u32)(src)<<21) & 0x00200000)
#define PWMFCI_SET(dst,src) \
                      (((dst) & ~0x00200000) | (((u32)(src)<<21) & 0x00200000))
/*	 Fields pwmfce	 */
#define PWMFCE_WIDTH                                                          1
#define PWMFCE_SHIFT                                                         20
#define PWMFCE_MASK                                                  0x00100000
#define PWMFCE_RD(src)                               (((src) & 0x00100000)>>20)
#define PWMFCE_WR(src)                          (((u32)(src)<<20) & 0x00100000)
#define PWMFCE_SET(dst,src) \
                      (((dst) & ~0x00100000) | (((u32)(src)<<20) & 0x00100000))
/*	 Fields pwmfcd	 */
#define PWMFCD_WIDTH                                                         20
#define PWMFCD_SHIFT                                                          0
#define PWMFCD_MASK                                                  0x000fffff
#define PWMFCD_RD(src)                                   (((src) & 0x000fffff))
#define PWMFCD_WR(src)                              (((u32)(src)) & 0x000fffff)
#define PWMFCD_SET(dst,src) \
                          (((dst) & ~0x000fffff) | (((u32)(src)) & 0x000fffff))

/*	Register lcdc_pwmdcr	*/ 
/*	 Fields pwmdcr	 */
#define PWMDCR_WIDTH                                                          8
#define PWMDCR_SHIFT                                                          0
#define PWMDCR_MASK                                                  0x000000ff
#define PWMDCR_RD(src)                                   (((src) & 0x000000ff))
#define PWMDCR_WR(src)                              (((u32)(src)) & 0x000000ff)
#define PWMDCR_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register lcdc_dfbar	*/ 
/*	 Fields dfbar	 */
#define DFBAR_WIDTH                                                          32
#define DFBAR_SHIFT                                                           0
#define DFBAR_MASK                                                   0xffffffff
#define DFBAR_RD(src)                                    (((src) & 0xffffffff))
#define DFBAR_WR(src)                               (((u32)(src)) & 0xffffffff)
#define DFBAR_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register lcdc_dflar	*/ 
/*	 Fields dflar	 */
#define DFLAR_WIDTH                                                          32
#define DFLAR_SHIFT                                                           0
#define DFLAR_MASK                                                   0xffffffff
#define DFLAR_RD(src)                                    (((src) & 0xffffffff))
#define DFLAR_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register lcdc_cir	*/ 
/*	 Fields cir_mn	 */
#define CIR_MN_WIDTH                                                          8
#define CIR_MN_SHIFT                                                         12
#define CIR_MN_MASK                                                  0x000ff000
#define CIR_MN_RD(src)                               (((src) & 0x000ff000)>>12)
#define CIR_MN_SET(dst,src) \
                      (((dst) & ~0x000ff000) | (((u32)(src)<<12) & 0x000ff000))
/*	 Fields cir_bi	 */
#define CIR_BI_WIDTH                                                          4
#define CIR_BI_SHIFT                                                          8
#define CIR_BI_MASK                                                  0x00000f00
#define CIR_BI_RD(src)                                (((src) & 0x00000f00)>>8)
#define CIR_BI_SET(dst,src) \
                       (((dst) & ~0x00000f00) | (((u32)(src)<<8) & 0x00000f00))
/*	 Fields cir_rev	 */
#define CIR_REV_WIDTH                                                         8
#define CIR_REV_SHIFT                                                         0
#define CIR_REV_MASK                                                 0x000000ff
#define CIR_REV_RD(src)                                  (((src) & 0x000000ff))
#define CIR_REV_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register lcdc_palt	*/ 
/*	 Fields blue_0	 */
#define BLUE_0_WIDTH                                                          5
#define BLUE_0_SHIFT                                                         27
#define BLUE_0_MASK                                                  0xf8000000
#define BLUE_0_RD(src)                               (((src) & 0xf8000000)>>27)
#define BLUE_0_SET(dst,src) \
                      (((dst) & ~0xf8000000) | (((u32)(src)<<27) & 0xf8000000))
/*	 Fields green_0	 */
#define GREEN_0_WIDTH                                                         6
#define GREEN_0_SHIFT                                                        21
#define GREEN_0_MASK                                                 0x07e00000
#define GREEN_0_RD(src)                              (((src) & 0x07e00000)>>21)
#define GREEN_0_SET(dst,src) \
                      (((dst) & ~0x07e00000) | (((u32)(src)<<21) & 0x07e00000))
/*	 Fields red_0	 */
#define RED_0_WIDTH                                                           5
#define RED_0_SHIFT                                                          16
#define RED_0_MASK                                                   0x001f0000
#define RED_0_RD(src)                                (((src) & 0x001f0000)>>16)
#define RED_0_SET(dst,src) \
                      (((dst) & ~0x001f0000) | (((u32)(src)<<16) & 0x001f0000))
/*	 Fields blue_1	 */
#define BLUE_1_WIDTH                                                          5
#define BLUE_1_SHIFT                                                         11
#define BLUE_1_MASK                                                  0x0000f800
#define BLUE_1_RD(src)                               (((src) & 0x0000f800)>>11)
#define BLUE_1_SET(dst,src) \
                      (((dst) & ~0x0000f800) | (((u32)(src)<<11) & 0x0000f800))
/*	 Fields green_1	 */
#define GREEN_1_WIDTH                                                         6
#define GREEN_1_SHIFT                                                         5
#define GREEN_1_MASK                                                 0x000007e0
#define GREEN_1_RD(src)                               (((src) & 0x000007e0)>>5)
#define GREEN_1_SET(dst,src) \
                       (((dst) & ~0x000007e0) | (((u32)(src)<<5) & 0x000007e0))
/*	 Fields red_1	 */
#define RED_1_WIDTH                                                           5
#define RED_1_SHIFT                                                           0
#define RED_1_MASK                                                   0x0000001f
#define RED_1_RD(src)                                    (((src) & 0x0000001f))
#define RED_1_SET(dst,src) \
                          (((dst) & ~0x0000001f) | (((u32)(src)) & 0x0000001f))

/*	Register lcdc_palt_size	*/ 
/*	 Fields size	 */
#define LCD_SIZE_WIDTH                                                       1
#define LCD_SIZE_SHIFT                                                       9
#define LCD_SIZE_MASK                                               0x00000200
#define LCD_SIZE_RD(src)                             (((src) & 0x00000200)>>9)
#define LCD_SIZE_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))

/*	Register lcdc_ower	*/ 
/*	 Fields owe_15	 */
#define OWE_15_WIDTH                                                          1
#define OWE_15_SHIFT                                                         15
#define OWE_15_MASK                                                  0x00008000
#define OWE_15_RD(src)                               (((src) & 0x00008000)>>15)
#define OWE_15_WR(src)                          (((u32)(src)<<15) & 0x00008000)
#define OWE_15_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))
/*	 Fields owe_14	 */
#define OWE_14_WIDTH                                                          1
#define OWE_14_SHIFT                                                         14
#define OWE_14_MASK                                                  0x00004000
#define OWE_14_RD(src)                               (((src) & 0x00004000)>>14)
#define OWE_14_WR(src)                          (((u32)(src)<<14) & 0x00004000)
#define OWE_14_SET(dst,src) \
                      (((dst) & ~0x00004000) | (((u32)(src)<<14) & 0x00004000))
/*	 Fields owe_13	 */
#define OWE_13_WIDTH                                                          1
#define OWE_13_SHIFT                                                         13
#define OWE_13_MASK                                                  0x00002000
#define OWE_13_RD(src)                               (((src) & 0x00002000)>>13)
#define OWE_13_WR(src)                          (((u32)(src)<<13) & 0x00002000)
#define OWE_13_SET(dst,src) \
                      (((dst) & ~0x00002000) | (((u32)(src)<<13) & 0x00002000))
/*	 Fields owe_12	 */
#define OWE_12_WIDTH                                                          1
#define OWE_12_SHIFT                                                         12
#define OWE_12_MASK                                                  0x00001000
#define OWE_12_RD(src)                               (((src) & 0x00001000)>>12)
#define OWE_12_WR(src)                          (((u32)(src)<<12) & 0x00001000)
#define OWE_12_SET(dst,src) \
                      (((dst) & ~0x00001000) | (((u32)(src)<<12) & 0x00001000))
/*	 Fields owe_11	 */
#define OWE_11_WIDTH                                                          1
#define OWE_11_SHIFT                                                         11
#define OWE_11_MASK                                                  0x00000800
#define OWE_11_RD(src)                               (((src) & 0x00000800)>>11)
#define OWE_11_WR(src)                          (((u32)(src)<<11) & 0x00000800)
#define OWE_11_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*	 Fields owe_10	 */
#define OWE_10_WIDTH                                                          1
#define OWE_10_SHIFT                                                         10
#define OWE_10_MASK                                                  0x00000400
#define OWE_10_RD(src)                               (((src) & 0x00000400)>>10)
#define OWE_10_WR(src)                          (((u32)(src)<<10) & 0x00000400)
#define OWE_10_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields owe_09	 */
#define OWE_09_WIDTH                                                          1
#define OWE_09_SHIFT                                                          9
#define OWE_09_MASK                                                  0x00000200
#define OWE_09_RD(src)                               (((src) & 0x00000200)>>09)
#define OWE_09_WR(src)                          (((u32)(src)<<09) & 0x00000200)
#define OWE_09_SET(dst,src) \
                      (((dst) & ~0x00000200) | (((u32)(src)<<09) & 0x00000200))
/*	 Fields owe_08	 */
#define OWE_08_WIDTH                                                          1
#define OWE_08_SHIFT                                                          8
#define OWE_08_MASK                                                  0x00000100
#define OWE_08_RD(src)                               (((src) & 0x00000100)>>08)
#define OWE_08_WR(src)                          (((u32)(src)<<08) & 0x00000100)
#define OWE_08_SET(dst,src) \
                      (((dst) & ~0x00000100) | (((u32)(src)<<08) & 0x00000100))
/*	 Fields owe_07	 */
#define OWE_07_WIDTH                                                          1
#define OWE_07_SHIFT                                                          7
#define OWE_07_MASK                                                  0x00000080
#define OWE_07_RD(src)                               (((src) & 0x00000080)>>07)
#define OWE_07_WR(src)                          (((u32)(src)<<07) & 0x00000080)
#define OWE_07_SET(dst,src) \
                      (((dst) & ~0x00000080) | (((u32)(src)<<07) & 0x00000080))
/*	 Fields owe_06	 */
#define OWE_06_WIDTH                                                          1
#define OWE_06_SHIFT                                                          6
#define OWE_06_MASK                                                  0x00000040
#define OWE_06_RD(src)                               (((src) & 0x00000040)>>06)
#define OWE_06_WR(src)                          (((u32)(src)<<06) & 0x00000040)
#define OWE_06_SET(dst,src) \
                      (((dst) & ~0x00000040) | (((u32)(src)<<06) & 0x00000040))
/*	 Fields owe_05	 */
#define OWE_05_WIDTH                                                          1
#define OWE_05_SHIFT                                                          5
#define OWE_05_MASK                                                  0x00000020
#define OWE_05_RD(src)                               (((src) & 0x00000020)>>05)
#define OWE_05_WR(src)                          (((u32)(src)<<05) & 0x00000020)
#define OWE_05_SET(dst,src) \
                      (((dst) & ~0x00000020) | (((u32)(src)<<05) & 0x00000020))
/*	 Fields owe_04	 */
#define OWE_04_WIDTH                                                          1
#define OWE_04_SHIFT                                                          4
#define OWE_04_MASK                                                  0x00000010
#define OWE_04_RD(src)                               (((src) & 0x00000010)>>04)
#define OWE_04_WR(src)                          (((u32)(src)<<04) & 0x00000010)
#define OWE_04_SET(dst,src) \
                      (((dst) & ~0x00000010) | (((u32)(src)<<04) & 0x00000010))
/*	 Fields owe_03	 */
#define OWE_03_WIDTH                                                          1
#define OWE_03_SHIFT                                                          3
#define OWE_03_MASK                                                  0x00000008
#define OWE_03_RD(src)                               (((src) & 0x00000008)>>03)
#define OWE_03_WR(src)                          (((u32)(src)<<03) & 0x00000008)
#define OWE_03_SET(dst,src) \
                      (((dst) & ~0x00000008) | (((u32)(src)<<03) & 0x00000008))
/*	 Fields owe_02	 */
#define OWE_02_WIDTH                                                          1
#define OWE_02_SHIFT                                                          2
#define OWE_02_MASK                                                  0x00000004
#define OWE_02_RD(src)                               (((src) & 0x00000004)>>02)
#define OWE_02_WR(src)                          (((u32)(src)<<02) & 0x00000004)
#define OWE_02_SET(dst,src) \
                      (((dst) & ~0x00000004) | (((u32)(src)<<02) & 0x00000004))
/*	 Fields owe_01	 */
#define OWE_01_WIDTH                                                          1
#define OWE_01_SHIFT                                                          1
#define OWE_01_MASK                                                  0x00000002
#define OWE_01_RD(src)                               (((src) & 0x00000002)>>01)
#define OWE_01_WR(src)                          (((u32)(src)<<01) & 0x00000002)
#define OWE_01_SET(dst,src) \
                      (((dst) & ~0x00000002) | (((u32)(src)<<01) & 0x00000002))
/*	 Fields owe_00	 */
#define OWE_00_WIDTH                                                          1
#define OWE_00_SHIFT                                                          0
#define OWE_00_MASK                                                  0x00000001
#define OWE_00_RD(src)                                   (((src) & 0x00000001))
#define OWE_00_WR(src)                              (((u32)(src)) & 0x00000001)
#define OWE_00_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register lcdc_oxser0	*/ 
/*	 Fields ow_x_start	 */
#define OW_X_START0_WIDTH                                                    12
#define OW_X_START0_SHIFT                                                    16
#define OW_X_START0_MASK                                             0x0fff0000
#define OW_X_START0_RD(src)                          (((src) & 0x0fff0000)>>16)
#define OW_X_START0_WR(src)                     (((u32)(src)<<16) & 0x0fff0000)
#define OW_X_START0_SET(dst,src) \
                      (((dst) & ~0x0fff0000) | (((u32)(src)<<16) & 0x0fff0000))
/*	 Fields ow_x_end	 */
#define OW_X_END0_WIDTH                                                      12
#define OW_X_END0_SHIFT                                                       0
#define OW_X_END0_MASK                                               0x00000fff
#define OW_X_END0_RD(src)                                (((src) & 0x00000fff))
#define OW_X_END0_WR(src)                           (((u32)(src)) & 0x00000fff)
#define OW_X_END0_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lcdc_oyser0	*/ 
/*	 Fields ow_y_start	 */
#define OW_Y_START0_WIDTH                                                    12
#define OW_Y_START0_SHIFT                                                    16
#define OW_Y_START0_MASK                                             0x0fff0000
#define OW_Y_START0_RD(src)                          (((src) & 0x0fff0000)>>16)
#define OW_Y_START0_WR(src)                     (((u32)(src)<<16) & 0x0fff0000)
#define OW_Y_START0_SET(dst,src) \
                      (((dst) & ~0x0fff0000) | (((u32)(src)<<16) & 0x0fff0000))
/*	 Fields ow_y_end	 */
#define OW_Y_END0_WIDTH                                                      12
#define OW_Y_END0_SHIFT                                                       0
#define OW_Y_END0_MASK                                               0x00000fff
#define OW_Y_END0_RD(src)                                (((src) & 0x00000fff))
#define OW_Y_END0_WR(src)                           (((u32)(src)) & 0x00000fff)
#define OW_Y_END0_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lcdc_owdbar0	*/ 
/*	 Fields ow_dbar	 */
#define OW_DBAR0_WIDTH                                                       32
#define OW_DBAR0_SHIFT                                                        0
#define OW_DBAR0_MASK                                                0xffffffff
#define OW_DBAR0_RD(src)                                 (((src) & 0xffffffff))
#define OW_DBAR0_WR(src)                            (((u32)(src)) & 0xffffffff)
#define OW_DBAR0_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register lcdc_owdcar0	*/ 
/*	 Fields ow_dcar	 */
#define OW_DCAR0_WIDTH                                                       32
#define OW_DCAR0_SHIFT                                                        0
#define OW_DCAR0_MASK                                                0xffffffff
#define OW_DCAR0_RD(src)                                 (((src) & 0xffffffff))
#define OW_DCAR0_WR(src)                            (((u32)(src)) & 0xffffffff)
#define OW_DCAR0_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register lcdc_oxser1	*/ 
/*	 Fields ow_x_start	 */
#define OW_X_START1_WIDTH                                                    12
#define OW_X_START1_SHIFT                                                    16
#define OW_X_START1_MASK                                             0x0fff0000
#define OW_X_START1_RD(src)                          (((src) & 0x0fff0000)>>16)
#define OW_X_START1_WR(src)                     (((u32)(src)<<16) & 0x0fff0000)
#define OW_X_START1_SET(dst,src) \
                      (((dst) & ~0x0fff0000) | (((u32)(src)<<16) & 0x0fff0000))
/*	 Fields ow_x_end	 */
#define OW_X_END1_WIDTH                                                      12
#define OW_X_END1_SHIFT                                                       0
#define OW_X_END1_MASK                                               0x00000fff
#define OW_X_END1_RD(src)                                (((src) & 0x00000fff))
#define OW_X_END1_WR(src)                           (((u32)(src)) & 0x00000fff)
#define OW_X_END1_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lcdc_oyser1	*/ 
/*	 Fields ow_y_start	 */
#define OW_Y_START1_WIDTH                                                    12
#define OW_Y_START1_SHIFT                                                    16
#define OW_Y_START1_MASK                                             0x0fff0000
#define OW_Y_START1_RD(src)                          (((src) & 0x0fff0000)>>16)
#define OW_Y_START1_WR(src)                     (((u32)(src)<<16) & 0x0fff0000)
#define OW_Y_START1_SET(dst,src) \
                      (((dst) & ~0x0fff0000) | (((u32)(src)<<16) & 0x0fff0000))
/*	 Fields ow_y_end	 */
#define OW_Y_END1_WIDTH                                                      12
#define OW_Y_END1_SHIFT                                                       0
#define OW_Y_END1_MASK                                               0x00000fff
#define OW_Y_END1_RD(src)                                (((src) & 0x00000fff))
#define OW_Y_END1_WR(src)                           (((u32)(src)) & 0x00000fff)
#define OW_Y_END1_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lcdc_owdbar1	*/ 
/*	 Fields ow_dbar	 */
#define OW_DBAR1_WIDTH                                                       32
#define OW_DBAR1_SHIFT                                                        0
#define OW_DBAR1_MASK                                                0xffffffff
#define OW_DBAR1_RD(src)                                 (((src) & 0xffffffff))
#define OW_DBAR1_WR(src)                            (((u32)(src)) & 0xffffffff)
#define OW_DBAR1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register lcdc_owdcar1	*/ 
/*	 Fields OW_DCAR	 */
#define OW_DCAR1_WIDTH                                                       32
#define OW_DCAR1_SHIFT                                                        0
#define OW_DCAR1_MASK                                                0xffffffff
#define OW_DCAR1_RD(src)                                 (((src) & 0xffffffff))
#define OW_DCAR1_WR(src)                            (((u32)(src)) & 0xffffffff)
#define OW_DCAR1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register lcdc_oxser2	*/ 
/*	 Fields ow_x_start	 */
#define OW_X_START2_WIDTH                                                    12
#define OW_X_START2_SHIFT                                                    16
#define OW_X_START2_MASK                                             0x0fff0000
#define OW_X_START2_RD(src)                          (((src) & 0x0fff0000)>>16)
#define OW_X_START2_WR(src)                     (((u32)(src)<<16) & 0x0fff0000)
#define OW_X_START2_SET(dst,src) \
                      (((dst) & ~0x0fff0000) | (((u32)(src)<<16) & 0x0fff0000))
/*	 Fields ow_x_end	 */
#define OW_X_END2_WIDTH                                                      12
#define OW_X_END2_SHIFT                                                       0
#define OW_X_END2_MASK                                               0x00000fff
#define OW_X_END2_RD(src)                                (((src) & 0x00000fff))
#define OW_X_END2_WR(src)                           (((u32)(src)) & 0x00000fff)
#define OW_X_END2_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lcdc_oyser2	*/ 
/*	 Fields ow_y_start	 */
#define OW_Y_START2_WIDTH                                                    12
#define OW_Y_START2_SHIFT                                                    16
#define OW_Y_START2_MASK                                             0x0fff0000
#define OW_Y_START2_RD(src)                          (((src) & 0x0fff0000)>>16)
#define OW_Y_START2_WR(src)                     (((u32)(src)<<16) & 0x0fff0000)
#define OW_Y_START2_SET(dst,src) \
                      (((dst) & ~0x0fff0000) | (((u32)(src)<<16) & 0x0fff0000))
/*	 Fields ow_y_end	 */
#define OW_Y_END2_WIDTH                                                      12
#define OW_Y_END2_SHIFT                                                       0
#define OW_Y_END2_MASK                                               0x00000fff
#define OW_Y_END2_RD(src)                                (((src) & 0x00000fff))
#define OW_Y_END2_WR(src)                           (((u32)(src)) & 0x00000fff)
#define OW_Y_END2_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lcdc_owdbar2	*/ 
/*	 Fields ow_dbar	 */
#define OW_DBAR2_WIDTH                                                       32
#define OW_DBAR2_SHIFT                                                        0
#define OW_DBAR2_MASK                                                0xffffffff
#define OW_DBAR2_RD(src)                                 (((src) & 0xffffffff))
#define OW_DBAR2_WR(src)                            (((u32)(src)) & 0xffffffff)
#define OW_DBAR2_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register lcdc_owdcar2	*/ 
/*	 Fields OW_DCAR	 */
#define OW_DCAR2_WIDTH                                                       32
#define OW_DCAR2_SHIFT                                                        0
#define OW_DCAR2_MASK                                                0xffffffff
#define OW_DCAR2_RD(src)                                 (((src) & 0xffffffff))
#define OW_DCAR2_WR(src)                            (((u32)(src)) & 0xffffffff)
#define OW_DCAR2_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register lcdc_oxser3	*/ 
/*	 Fields ow_x_start	 */
#define OW_X_START3_WIDTH                                                    12
#define OW_X_START3_SHIFT                                                    16
#define OW_X_START3_MASK                                             0x0fff0000
#define OW_X_START3_RD(src)                          (((src) & 0x0fff0000)>>16)
#define OW_X_START3_WR(src)                     (((u32)(src)<<16) & 0x0fff0000)
#define OW_X_START3_SET(dst,src) \
                      (((dst) & ~0x0fff0000) | (((u32)(src)<<16) & 0x0fff0000))
/*	 Fields ow_x_end	 */
#define OW_X_END3_WIDTH                                                      12
#define OW_X_END3_SHIFT                                                       0
#define OW_X_END3_MASK                                               0x00000fff
#define OW_X_END3_RD(src)                                (((src) & 0x00000fff))
#define OW_X_END3_WR(src)                           (((u32)(src)) & 0x00000fff)
#define OW_X_END3_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lcdc_oyser3	*/ 
/*	 Fields ow_y_start	 */
#define OW_Y_START3_WIDTH                                                    12
#define OW_Y_START3_SHIFT                                                    16
#define OW_Y_START3_MASK                                             0x0fff0000
#define OW_Y_START3_RD(src)                          (((src) & 0x0fff0000)>>16)
#define OW_Y_START3_WR(src)                     (((u32)(src)<<16) & 0x0fff0000)
#define OW_Y_START3_SET(dst,src) \
                      (((dst) & ~0x0fff0000) | (((u32)(src)<<16) & 0x0fff0000))
/*	 Fields ow_y_end	 */
#define OW_Y_END3_WIDTH                                                      12
#define OW_Y_END3_SHIFT                                                       0
#define OW_Y_END3_MASK                                               0x00000fff
#define OW_Y_END3_RD(src)                                (((src) & 0x00000fff))
#define OW_Y_END3_WR(src)                           (((u32)(src)) & 0x00000fff)
#define OW_Y_END3_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lcdc_owdbar3	*/ 
/*	 Fields ow_dbar	 */
#define OW_DBAR3_WIDTH                                                       32
#define OW_DBAR3_SHIFT                                                        0
#define OW_DBAR3_MASK                                                0xffffffff
#define OW_DBAR3_RD(src)                                 (((src) & 0xffffffff))
#define OW_DBAR3_WR(src)                            (((u32)(src)) & 0xffffffff)
#define OW_DBAR3_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register lcdc_owdcar3	*/ 
/*	 Fields OW_DCAR	 */
#define OW_DCAR3_WIDTH                                                       32
#define OW_DCAR3_SHIFT                                                        0
#define OW_DCAR3_MASK                                                0xffffffff
#define OW_DCAR3_RD(src)                                 (((src) & 0xffffffff))
#define OW_DCAR3_WR(src)                            (((u32)(src)) & 0xffffffff)
#define OW_DCAR3_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register lcdc_oxser4	*/ 
/*	 Fields ow_x_start	 */
#define OW_X_START4_WIDTH                                                    12
#define OW_X_START4_SHIFT                                                    16
#define OW_X_START4_MASK                                             0x0fff0000
#define OW_X_START4_RD(src)                          (((src) & 0x0fff0000)>>16)
#define OW_X_START4_WR(src)                     (((u32)(src)<<16) & 0x0fff0000)
#define OW_X_START4_SET(dst,src) \
                      (((dst) & ~0x0fff0000) | (((u32)(src)<<16) & 0x0fff0000))
/*	 Fields ow_x_end	 */
#define OW_X_END4_WIDTH                                                      12
#define OW_X_END4_SHIFT                                                       0
#define OW_X_END4_MASK                                               0x00000fff
#define OW_X_END4_RD(src)                                (((src) & 0x00000fff))
#define OW_X_END4_WR(src)                           (((u32)(src)) & 0x00000fff)
#define OW_X_END4_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lcdc_oyser4	*/ 
/*	 Fields ow_y_start	 */
#define OW_Y_START4_WIDTH                                                    12
#define OW_Y_START4_SHIFT                                                    16
#define OW_Y_START4_MASK                                             0x0fff0000
#define OW_Y_START4_RD(src)                          (((src) & 0x0fff0000)>>16)
#define OW_Y_START4_WR(src)                     (((u32)(src)<<16) & 0x0fff0000)
#define OW_Y_START4_SET(dst,src) \
                      (((dst) & ~0x0fff0000) | (((u32)(src)<<16) & 0x0fff0000))
/*	 Fields ow_y_end	 */
#define OW_Y_END4_WIDTH                                                      12
#define OW_Y_END4_SHIFT                                                       0
#define OW_Y_END4_MASK                                               0x00000fff
#define OW_Y_END4_RD(src)                                (((src) & 0x00000fff))
#define OW_Y_END4_WR(src)                           (((u32)(src)) & 0x00000fff)
#define OW_Y_END4_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lcdc_owdbar4	*/ 
/*	 Fields ow_dbar	 */
#define OW_DBAR4_WIDTH                                                       32
#define OW_DBAR4_SHIFT                                                        0
#define OW_DBAR4_MASK                                                0xffffffff
#define OW_DBAR4_RD(src)                                 (((src) & 0xffffffff))
#define OW_DBAR4_WR(src)                            (((u32)(src)) & 0xffffffff)
#define OW_DBAR4_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register lcdc_owdcar4	*/ 
/*	 Fields OW_DCAR	 */
#define OW_DCAR4_WIDTH                                                       32
#define OW_DCAR4_SHIFT                                                        0
#define OW_DCAR4_MASK                                                0xffffffff
#define OW_DCAR4_RD(src)                                 (((src) & 0xffffffff))
#define OW_DCAR4_WR(src)                            (((u32)(src)) & 0xffffffff)
#define OW_DCAR4_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register lcdc_oxser5	*/ 
/*	 Fields ow_x_start	 */
#define OW_X_START5_WIDTH                                                    12
#define OW_X_START5_SHIFT                                                    16
#define OW_X_START5_MASK                                             0x0fff0000
#define OW_X_START5_RD(src)                          (((src) & 0x0fff0000)>>16)
#define OW_X_START5_WR(src)                     (((u32)(src)<<16) & 0x0fff0000)
#define OW_X_START5_SET(dst,src) \
                      (((dst) & ~0x0fff0000) | (((u32)(src)<<16) & 0x0fff0000))
/*	 Fields ow_x_end	 */
#define OW_X_END5_WIDTH                                                      12
#define OW_X_END5_SHIFT                                                       0
#define OW_X_END5_MASK                                               0x00000fff
#define OW_X_END5_RD(src)                                (((src) & 0x00000fff))
#define OW_X_END5_WR(src)                           (((u32)(src)) & 0x00000fff)
#define OW_X_END5_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lcdc_oyser5	*/ 
/*	 Fields ow_y_start	 */
#define OW_Y_START5_WIDTH                                                    12
#define OW_Y_START5_SHIFT                                                    16
#define OW_Y_START5_MASK                                             0x0fff0000
#define OW_Y_START5_RD(src)                          (((src) & 0x0fff0000)>>16)
#define OW_Y_START5_WR(src)                     (((u32)(src)<<16) & 0x0fff0000)
#define OW_Y_START5_SET(dst,src) \
                      (((dst) & ~0x0fff0000) | (((u32)(src)<<16) & 0x0fff0000))
/*	 Fields ow_y_end	 */
#define OW_Y_END5_WIDTH                                                      12
#define OW_Y_END5_SHIFT                                                       0
#define OW_Y_END5_MASK                                               0x00000fff
#define OW_Y_END5_RD(src)                                (((src) & 0x00000fff))
#define OW_Y_END5_WR(src)                           (((u32)(src)) & 0x00000fff)
#define OW_Y_END5_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lcdc_owdbar5	*/ 
/*	 Fields ow_dbar	 */
#define OW_DBAR5_WIDTH                                                       32
#define OW_DBAR5_SHIFT                                                        0
#define OW_DBAR5_MASK                                                0xffffffff
#define OW_DBAR5_RD(src)                                 (((src) & 0xffffffff))
#define OW_DBAR5_WR(src)                            (((u32)(src)) & 0xffffffff)
#define OW_DBAR5_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register lcdc_owdcar5	*/ 
/*	 Fields OW_DCAR	 */
#define OW_DCAR5_WIDTH                                                       32
#define OW_DCAR5_SHIFT                                                        0
#define OW_DCAR5_MASK                                                0xffffffff
#define OW_DCAR5_RD(src)                                 (((src) & 0xffffffff))
#define OW_DCAR5_WR(src)                            (((u32)(src)) & 0xffffffff)
#define OW_DCAR5_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register lcdc_oxser6	*/ 
/*	 Fields ow_x_start	 */
#define OW_X_START6_WIDTH                                                    12
#define OW_X_START6_SHIFT                                                    16
#define OW_X_START6_MASK                                             0x0fff0000
#define OW_X_START6_RD(src)                          (((src) & 0x0fff0000)>>16)
#define OW_X_START6_WR(src)                     (((u32)(src)<<16) & 0x0fff0000)
#define OW_X_START6_SET(dst,src) \
                      (((dst) & ~0x0fff0000) | (((u32)(src)<<16) & 0x0fff0000))
/*	 Fields ow_x_end	 */
#define OW_X_END6_WIDTH                                                      12
#define OW_X_END6_SHIFT                                                       0
#define OW_X_END6_MASK                                               0x00000fff
#define OW_X_END6_RD(src)                                (((src) & 0x00000fff))
#define OW_X_END6_WR(src)                           (((u32)(src)) & 0x00000fff)
#define OW_X_END6_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lcdc_oyser6	*/ 
/*	 Fields ow_y_start	 */
#define OW_Y_START6_WIDTH                                                    12
#define OW_Y_START6_SHIFT                                                    16
#define OW_Y_START6_MASK                                             0x0fff0000
#define OW_Y_START6_RD(src)                          (((src) & 0x0fff0000)>>16)
#define OW_Y_START6_WR(src)                     (((u32)(src)<<16) & 0x0fff0000)
#define OW_Y_START6_SET(dst,src) \
                      (((dst) & ~0x0fff0000) | (((u32)(src)<<16) & 0x0fff0000))
/*	 Fields ow_y_end	 */
#define OW_Y_END6_WIDTH                                                      12
#define OW_Y_END6_SHIFT                                                       0
#define OW_Y_END6_MASK                                               0x00000fff
#define OW_Y_END6_RD(src)                                (((src) & 0x00000fff))
#define OW_Y_END6_WR(src)                           (((u32)(src)) & 0x00000fff)
#define OW_Y_END6_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lcdc_owdbar6	*/ 
/*	 Fields ow_dbar	 */
#define OW_DBAR6_WIDTH                                                       32
#define OW_DBAR6_SHIFT                                                        0
#define OW_DBAR6_MASK                                                0xffffffff
#define OW_DBAR6_RD(src)                                 (((src) & 0xffffffff))
#define OW_DBAR6_WR(src)                            (((u32)(src)) & 0xffffffff)
#define OW_DBAR6_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register lcdc_owdcar6	*/ 
/*	 Fields OW_DCAR	 */
#define OW_DCAR6_WIDTH                                                       32
#define OW_DCAR6_SHIFT                                                        0
#define OW_DCAR6_MASK                                                0xffffffff
#define OW_DCAR6_RD(src)                                 (((src) & 0xffffffff))
#define OW_DCAR6_WR(src)                            (((u32)(src)) & 0xffffffff)
#define OW_DCAR6_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register lcdc_oxser7	*/ 
/*	 Fields ow_x_start	 */
#define OW_X_START7_WIDTH                                                    12
#define OW_X_START7_SHIFT                                                    16
#define OW_X_START7_MASK                                             0x0fff0000
#define OW_X_START7_RD(src)                          (((src) & 0x0fff0000)>>16)
#define OW_X_START7_WR(src)                     (((u32)(src)<<16) & 0x0fff0000)
#define OW_X_START7_SET(dst,src) \
                      (((dst) & ~0x0fff0000) | (((u32)(src)<<16) & 0x0fff0000))
/*	 Fields ow_x_end	 */
#define OW_X_END7_WIDTH                                                      12
#define OW_X_END7_SHIFT                                                       0
#define OW_X_END7_MASK                                               0x00000fff
#define OW_X_END7_RD(src)                                (((src) & 0x00000fff))
#define OW_X_END7_WR(src)                           (((u32)(src)) & 0x00000fff)
#define OW_X_END7_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lcdc_oyser7	*/ 
/*	 Fields ow_y_start	 */
#define OW_Y_START7_WIDTH                                                    12
#define OW_Y_START7_SHIFT                                                    16
#define OW_Y_START7_MASK                                             0x0fff0000
#define OW_Y_START7_RD(src)                          (((src) & 0x0fff0000)>>16)
#define OW_Y_START7_WR(src)                     (((u32)(src)<<16) & 0x0fff0000)
#define OW_Y_START7_SET(dst,src) \
                      (((dst) & ~0x0fff0000) | (((u32)(src)<<16) & 0x0fff0000))
/*	 Fields ow_y_end	 */
#define OW_Y_END7_WIDTH                                                      12
#define OW_Y_END7_SHIFT                                                       0
#define OW_Y_END7_MASK                                               0x00000fff
#define OW_Y_END7_RD(src)                                (((src) & 0x00000fff))
#define OW_Y_END7_WR(src)                           (((u32)(src)) & 0x00000fff)
#define OW_Y_END7_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lcdc_owdbar7	*/ 
/*	 Fields ow_dbar	 */
#define OW_DBAR7_WIDTH                                                       32
#define OW_DBAR7_SHIFT                                                        0
#define OW_DBAR7_MASK                                                0xffffffff
#define OW_DBAR7_RD(src)                                 (((src) & 0xffffffff))
#define OW_DBAR7_WR(src)                            (((u32)(src)) & 0xffffffff)
#define OW_DBAR7_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register lcdc_owdcar7	*/ 
/*	 Fields OW_DCAR	 */
#define OW_DCAR7_WIDTH                                                       32
#define OW_DCAR7_SHIFT                                                        0
#define OW_DCAR7_MASK                                                0xffffffff
#define OW_DCAR7_RD(src)                                 (((src) & 0xffffffff))
#define OW_DCAR7_WR(src)                            (((u32)(src)) & 0xffffffff)
#define OW_DCAR7_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register lcdc_oxser8	*/ 
/*	 Fields ow_x_start	 */
#define OW_X_START8_WIDTH                                                    12
#define OW_X_START8_SHIFT                                                    16
#define OW_X_START8_MASK                                             0x0fff0000
#define OW_X_START8_RD(src)                          (((src) & 0x0fff0000)>>16)
#define OW_X_START8_WR(src)                     (((u32)(src)<<16) & 0x0fff0000)
#define OW_X_START8_SET(dst,src) \
                      (((dst) & ~0x0fff0000) | (((u32)(src)<<16) & 0x0fff0000))
/*	 Fields ow_x_end	 */
#define OW_X_END8_WIDTH                                                      12
#define OW_X_END8_SHIFT                                                       0
#define OW_X_END8_MASK                                               0x00000fff
#define OW_X_END8_RD(src)                                (((src) & 0x00000fff))
#define OW_X_END8_WR(src)                           (((u32)(src)) & 0x00000fff)
#define OW_X_END8_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lcdc_oyser8	*/ 
/*	 Fields ow_y_start	 */
#define OW_Y_START8_WIDTH                                                    12
#define OW_Y_START8_SHIFT                                                    16
#define OW_Y_START8_MASK                                             0x0fff0000
#define OW_Y_START8_RD(src)                          (((src) & 0x0fff0000)>>16)
#define OW_Y_START8_WR(src)                     (((u32)(src)<<16) & 0x0fff0000)
#define OW_Y_START8_SET(dst,src) \
                      (((dst) & ~0x0fff0000) | (((u32)(src)<<16) & 0x0fff0000))
/*	 Fields ow_y_end	 */
#define OW_Y_END8_WIDTH                                                      12
#define OW_Y_END8_SHIFT                                                       0
#define OW_Y_END8_MASK                                               0x00000fff
#define OW_Y_END8_RD(src)                                (((src) & 0x00000fff))
#define OW_Y_END8_WR(src)                           (((u32)(src)) & 0x00000fff)
#define OW_Y_END8_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lcdc_owdbar8	*/ 
/*	 Fields ow_dbar	 */
#define OW_DBAR8_WIDTH                                                       32
#define OW_DBAR8_SHIFT                                                        0
#define OW_DBAR8_MASK                                                0xffffffff
#define OW_DBAR8_RD(src)                                 (((src) & 0xffffffff))
#define OW_DBAR8_WR(src)                            (((u32)(src)) & 0xffffffff)
#define OW_DBAR8_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register lcdc_owdcar8	*/ 
/*	 Fields OW_DCAR	 */
#define OW_DCAR8_WIDTH                                                       32
#define OW_DCAR8_SHIFT                                                        0
#define OW_DCAR8_MASK                                                0xffffffff
#define OW_DCAR8_RD(src)                                 (((src) & 0xffffffff))
#define OW_DCAR8_WR(src)                            (((u32)(src)) & 0xffffffff)
#define OW_DCAR8_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register lcdc_oxser9	*/ 
/*	 Fields ow_x_start	 */
#define OW_X_START9_WIDTH                                                    12
#define OW_X_START9_SHIFT                                                    16
#define OW_X_START9_MASK                                             0x0fff0000
#define OW_X_START9_RD(src)                          (((src) & 0x0fff0000)>>16)
#define OW_X_START9_WR(src)                     (((u32)(src)<<16) & 0x0fff0000)
#define OW_X_START9_SET(dst,src) \
                      (((dst) & ~0x0fff0000) | (((u32)(src)<<16) & 0x0fff0000))
/*	 Fields ow_x_end	 */
#define OW_X_END9_WIDTH                                                      12
#define OW_X_END9_SHIFT                                                       0
#define OW_X_END9_MASK                                               0x00000fff
#define OW_X_END9_RD(src)                                (((src) & 0x00000fff))
#define OW_X_END9_WR(src)                           (((u32)(src)) & 0x00000fff)
#define OW_X_END9_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lcdc_oyser9	*/ 
/*	 Fields ow_y_start	 */
#define OW_Y_START9_WIDTH                                                    12
#define OW_Y_START9_SHIFT                                                    16
#define OW_Y_START9_MASK                                             0x0fff0000
#define OW_Y_START9_RD(src)                          (((src) & 0x0fff0000)>>16)
#define OW_Y_START9_WR(src)                     (((u32)(src)<<16) & 0x0fff0000)
#define OW_Y_START9_SET(dst,src) \
                      (((dst) & ~0x0fff0000) | (((u32)(src)<<16) & 0x0fff0000))
/*	 Fields ow_y_end	 */
#define OW_Y_END9_WIDTH                                                      12
#define OW_Y_END9_SHIFT                                                       0
#define OW_Y_END9_MASK                                               0x00000fff
#define OW_Y_END9_RD(src)                                (((src) & 0x00000fff))
#define OW_Y_END9_WR(src)                           (((u32)(src)) & 0x00000fff)
#define OW_Y_END9_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lcdc_owdbar9	*/ 
/*	 Fields ow_dbar	 */
#define OW_DBAR9_WIDTH                                                       32
#define OW_DBAR9_SHIFT                                                        0
#define OW_DBAR9_MASK                                                0xffffffff
#define OW_DBAR9_RD(src)                                 (((src) & 0xffffffff))
#define OW_DBAR9_WR(src)                            (((u32)(src)) & 0xffffffff)
#define OW_DBAR9_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register lcdc_owdcar9	*/ 
/*	 Fields OW_DCAR	 */
#define OW_DCAR9_WIDTH                                                       32
#define OW_DCAR9_SHIFT                                                        0
#define OW_DCAR9_MASK                                                0xffffffff
#define OW_DCAR9_RD(src)                                 (((src) & 0xffffffff))
#define OW_DCAR9_WR(src)                            (((u32)(src)) & 0xffffffff)
#define OW_DCAR9_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register lcdc_oxser10	*/ 
/*	 Fields ow_x_start	 */
#define OW_X_START0_F1_WIDTH                                                 12
#define OW_X_START0_F1_SHIFT                                                 16
#define OW_X_START0_F1_MASK                                          0x0fff0000
#define OW_X_START0_F1_RD(src)                       (((src) & 0x0fff0000)>>16)
#define OW_X_START0_F1_WR(src)                  (((u32)(src)<<16) & 0x0fff0000)
#define OW_X_START0_F1_SET(dst,src) \
                      (((dst) & ~0x0fff0000) | (((u32)(src)<<16) & 0x0fff0000))
/*	 Fields ow_x_end	 */
#define OW_X_END0_F1_WIDTH                                                   12
#define OW_X_END0_F1_SHIFT                                                    0
#define OW_X_END0_F1_MASK                                            0x00000fff
#define OW_X_END0_F1_RD(src)                             (((src) & 0x00000fff))
#define OW_X_END0_F1_WR(src)                        (((u32)(src)) & 0x00000fff)
#define OW_X_END0_F1_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lcdc_oyser10	*/ 
/*	 Fields ow_y_start	 */
#define OW_Y_START0_F1_WIDTH                                                 12
#define OW_Y_START0_F1_SHIFT                                                 16
#define OW_Y_START0_F1_MASK                                          0x0fff0000
#define OW_Y_START0_F1_RD(src)                       (((src) & 0x0fff0000)>>16)
#define OW_Y_START0_F1_WR(src)                  (((u32)(src)<<16) & 0x0fff0000)
#define OW_Y_START0_F1_SET(dst,src) \
                      (((dst) & ~0x0fff0000) | (((u32)(src)<<16) & 0x0fff0000))
/*	 Fields ow_y_end	 */
#define OW_Y_END0_F1_WIDTH                                                   12
#define OW_Y_END0_F1_SHIFT                                                    0
#define OW_Y_END0_F1_MASK                                            0x00000fff
#define OW_Y_END0_F1_RD(src)                             (((src) & 0x00000fff))
#define OW_Y_END0_F1_WR(src)                        (((u32)(src)) & 0x00000fff)
#define OW_Y_END0_F1_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lcdc_owdbar10	*/ 
/*	 Fields ow_dbar	 */
#define OW_DBAR0_F1_WIDTH                                                    32
#define OW_DBAR0_F1_SHIFT                                                     0
#define OW_DBAR0_F1_MASK                                             0xffffffff
#define OW_DBAR0_F1_RD(src)                              (((src) & 0xffffffff))
#define OW_DBAR0_F1_WR(src)                         (((u32)(src)) & 0xffffffff)
#define OW_DBAR0_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register lcdc_owdcar10	*/ 
/*	 Fields OW_DCAR	 */
#define OW_DCAR0_F1_WIDTH                                                    32
#define OW_DCAR0_F1_SHIFT                                                     0
#define OW_DCAR0_F1_MASK                                             0xffffffff
#define OW_DCAR0_F1_RD(src)                              (((src) & 0xffffffff))
#define OW_DCAR0_F1_WR(src)                         (((u32)(src)) & 0xffffffff)
#define OW_DCAR0_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register lcdc_oxser11	*/ 
/*	 Fields ow_x_start	 */
#define OW_X_START1_F1_WIDTH                                                 12
#define OW_X_START1_F1_SHIFT                                                 16
#define OW_X_START1_F1_MASK                                          0x0fff0000
#define OW_X_START1_F1_RD(src)                       (((src) & 0x0fff0000)>>16)
#define OW_X_START1_F1_WR(src)                  (((u32)(src)<<16) & 0x0fff0000)
#define OW_X_START1_F1_SET(dst,src) \
                      (((dst) & ~0x0fff0000) | (((u32)(src)<<16) & 0x0fff0000))
/*	 Fields ow_x_end	 */
#define OW_X_END1_F1_WIDTH                                                   12
#define OW_X_END1_F1_SHIFT                                                    0
#define OW_X_END1_F1_MASK                                            0x00000fff
#define OW_X_END1_F1_RD(src)                             (((src) & 0x00000fff))
#define OW_X_END1_F1_WR(src)                        (((u32)(src)) & 0x00000fff)
#define OW_X_END1_F1_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lcdc_oyser11	*/ 
/*	 Fields ow_y_start	 */
#define OW_Y_START1_F1_WIDTH                                                 12
#define OW_Y_START1_F1_SHIFT                                                 16
#define OW_Y_START1_F1_MASK                                          0x0fff0000
#define OW_Y_START1_F1_RD(src)                       (((src) & 0x0fff0000)>>16)
#define OW_Y_START1_F1_WR(src)                  (((u32)(src)<<16) & 0x0fff0000)
#define OW_Y_START1_F1_SET(dst,src) \
                      (((dst) & ~0x0fff0000) | (((u32)(src)<<16) & 0x0fff0000))
/*	 Fields ow_y_end	 */
#define OW_Y_END1_F1_WIDTH                                                   12
#define OW_Y_END1_F1_SHIFT                                                    0
#define OW_Y_END1_F1_MASK                                            0x00000fff
#define OW_Y_END1_F1_RD(src)                             (((src) & 0x00000fff))
#define OW_Y_END1_F1_WR(src)                        (((u32)(src)) & 0x00000fff)
#define OW_Y_END1_F1_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lcdc_owdbar11	*/ 
/*	 Fields ow_dbar	 */
#define OW_DBAR1_F1_WIDTH                                                    32
#define OW_DBAR1_F1_SHIFT                                                     0
#define OW_DBAR1_F1_MASK                                             0xffffffff
#define OW_DBAR1_F1_RD(src)                              (((src) & 0xffffffff))
#define OW_DBAR1_F1_WR(src)                         (((u32)(src)) & 0xffffffff)
#define OW_DBAR1_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register lcdc_owdcar11	*/ 
/*	 Fields OW_DCAR	 */
#define OW_DCAR1_F1_WIDTH                                                    32
#define OW_DCAR1_F1_SHIFT                                                     0
#define OW_DCAR1_F1_MASK                                             0xffffffff
#define OW_DCAR1_F1_RD(src)                              (((src) & 0xffffffff))
#define OW_DCAR1_F1_WR(src)                         (((u32)(src)) & 0xffffffff)
#define OW_DCAR1_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register lcdc_oxser12	*/ 
/*	 Fields ow_x_start	 */
#define OW_X_START2_F1_WIDTH                                                 12
#define OW_X_START2_F1_SHIFT                                                 16
#define OW_X_START2_F1_MASK                                          0x0fff0000
#define OW_X_START2_F1_RD(src)                       (((src) & 0x0fff0000)>>16)
#define OW_X_START2_F1_WR(src)                  (((u32)(src)<<16) & 0x0fff0000)
#define OW_X_START2_F1_SET(dst,src) \
                      (((dst) & ~0x0fff0000) | (((u32)(src)<<16) & 0x0fff0000))
/*	 Fields ow_x_end	 */
#define OW_X_END2_F1_WIDTH                                                   12
#define OW_X_END2_F1_SHIFT                                                    0
#define OW_X_END2_F1_MASK                                            0x00000fff
#define OW_X_END2_F1_RD(src)                             (((src) & 0x00000fff))
#define OW_X_END2_F1_WR(src)                        (((u32)(src)) & 0x00000fff)
#define OW_X_END2_F1_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lcdc_oyser12	*/ 
/*	 Fields ow_y_start	 */
#define OW_Y_START2_F1_WIDTH                                                 12
#define OW_Y_START2_F1_SHIFT                                                 16
#define OW_Y_START2_F1_MASK                                          0x0fff0000
#define OW_Y_START2_F1_RD(src)                       (((src) & 0x0fff0000)>>16)
#define OW_Y_START2_F1_WR(src)                  (((u32)(src)<<16) & 0x0fff0000)
#define OW_Y_START2_F1_SET(dst,src) \
                      (((dst) & ~0x0fff0000) | (((u32)(src)<<16) & 0x0fff0000))
/*	 Fields ow_y_end	 */
#define OW_Y_END2_F1_WIDTH                                                   12
#define OW_Y_END2_F1_SHIFT                                                    0
#define OW_Y_END2_F1_MASK                                            0x00000fff
#define OW_Y_END2_F1_RD(src)                             (((src) & 0x00000fff))
#define OW_Y_END2_F1_WR(src)                        (((u32)(src)) & 0x00000fff)
#define OW_Y_END2_F1_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lcdc_owdbar12	*/ 
/*	 Fields ow_dbar	 */
#define OW_DBAR2_F1_WIDTH                                                    32
#define OW_DBAR2_F1_SHIFT                                                     0
#define OW_DBAR2_F1_MASK                                             0xffffffff
#define OW_DBAR2_F1_RD(src)                              (((src) & 0xffffffff))
#define OW_DBAR2_F1_WR(src)                         (((u32)(src)) & 0xffffffff)
#define OW_DBAR2_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register lcdc_owdcar12	*/ 
/*	 Fields OW_DCAR	 */
#define OW_DCAR2_F1_WIDTH                                                    32
#define OW_DCAR2_F1_SHIFT                                                     0
#define OW_DCAR2_F1_MASK                                             0xffffffff
#define OW_DCAR2_F1_RD(src)                              (((src) & 0xffffffff))
#define OW_DCAR2_F1_WR(src)                         (((u32)(src)) & 0xffffffff)
#define OW_DCAR2_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register lcdc_oxser13	*/ 
/*	 Fields ow_x_start	 */
#define OW_X_START3_F1_WIDTH                                                 12
#define OW_X_START3_F1_SHIFT                                                 16
#define OW_X_START3_F1_MASK                                          0x0fff0000
#define OW_X_START3_F1_RD(src)                       (((src) & 0x0fff0000)>>16)
#define OW_X_START3_F1_WR(src)                  (((u32)(src)<<16) & 0x0fff0000)
#define OW_X_START3_F1_SET(dst,src) \
                      (((dst) & ~0x0fff0000) | (((u32)(src)<<16) & 0x0fff0000))
/*	 Fields ow_x_end	 */
#define OW_X_END3_F1_WIDTH                                                   12
#define OW_X_END3_F1_SHIFT                                                    0
#define OW_X_END3_F1_MASK                                            0x00000fff
#define OW_X_END3_F1_RD(src)                             (((src) & 0x00000fff))
#define OW_X_END3_F1_WR(src)                        (((u32)(src)) & 0x00000fff)
#define OW_X_END3_F1_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lcdc_oyser13	*/ 
/*	 Fields ow_y_start	 */
#define OW_Y_START3_F1_WIDTH                                                 12
#define OW_Y_START3_F1_SHIFT                                                 16
#define OW_Y_START3_F1_MASK                                          0x0fff0000
#define OW_Y_START3_F1_RD(src)                       (((src) & 0x0fff0000)>>16)
#define OW_Y_START3_F1_WR(src)                  (((u32)(src)<<16) & 0x0fff0000)
#define OW_Y_START3_F1_SET(dst,src) \
                      (((dst) & ~0x0fff0000) | (((u32)(src)<<16) & 0x0fff0000))
/*	 Fields ow_y_end	 */
#define OW_Y_END3_F1_WIDTH                                                   12
#define OW_Y_END3_F1_SHIFT                                                    0
#define OW_Y_END3_F1_MASK                                            0x00000fff
#define OW_Y_END3_F1_RD(src)                             (((src) & 0x00000fff))
#define OW_Y_END3_F1_WR(src)                        (((u32)(src)) & 0x00000fff)
#define OW_Y_END3_F1_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lcdc_owdbar13	*/ 
/*	 Fields ow_dbar	 */
#define OW_DBAR3_F1_WIDTH                                                    32
#define OW_DBAR3_F1_SHIFT                                                     0
#define OW_DBAR3_F1_MASK                                             0xffffffff
#define OW_DBAR3_F1_RD(src)                              (((src) & 0xffffffff))
#define OW_DBAR3_F1_WR(src)                         (((u32)(src)) & 0xffffffff)
#define OW_DBAR3_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register lcdc_owdcar13	*/ 
/*	 Fields OW_DCAR	 */
#define OW_DCAR3_F1_WIDTH                                                    32
#define OW_DCAR3_F1_SHIFT                                                     0
#define OW_DCAR3_F1_MASK                                             0xffffffff
#define OW_DCAR3_F1_RD(src)                              (((src) & 0xffffffff))
#define OW_DCAR3_F1_WR(src)                         (((u32)(src)) & 0xffffffff)
#define OW_DCAR3_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register lcdc_oxser14	*/ 
/*	 Fields ow_x_start	 */
#define OW_X_START4_F1_WIDTH                                                 12
#define OW_X_START4_F1_SHIFT                                                 16
#define OW_X_START4_F1_MASK                                          0x0fff0000
#define OW_X_START4_F1_RD(src)                       (((src) & 0x0fff0000)>>16)
#define OW_X_START4_F1_WR(src)                  (((u32)(src)<<16) & 0x0fff0000)
#define OW_X_START4_F1_SET(dst,src) \
                      (((dst) & ~0x0fff0000) | (((u32)(src)<<16) & 0x0fff0000))
/*	 Fields ow_x_end	 */
#define OW_X_END4_F1_WIDTH                                                   12
#define OW_X_END4_F1_SHIFT                                                    0
#define OW_X_END4_F1_MASK                                            0x00000fff
#define OW_X_END4_F1_RD(src)                             (((src) & 0x00000fff))
#define OW_X_END4_F1_WR(src)                        (((u32)(src)) & 0x00000fff)
#define OW_X_END4_F1_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lcdc_oyser14	*/ 
/*	 Fields ow_y_start	 */
#define OW_Y_START4_F1_WIDTH                                                 12
#define OW_Y_START4_F1_SHIFT                                                 16
#define OW_Y_START4_F1_MASK                                          0x0fff0000
#define OW_Y_START4_F1_RD(src)                       (((src) & 0x0fff0000)>>16)
#define OW_Y_START4_F1_WR(src)                  (((u32)(src)<<16) & 0x0fff0000)
#define OW_Y_START4_F1_SET(dst,src) \
                      (((dst) & ~0x0fff0000) | (((u32)(src)<<16) & 0x0fff0000))
/*	 Fields ow_y_end	 */
#define OW_Y_END4_F1_WIDTH                                                   12
#define OW_Y_END4_F1_SHIFT                                                    0
#define OW_Y_END4_F1_MASK                                            0x00000fff
#define OW_Y_END4_F1_RD(src)                             (((src) & 0x00000fff))
#define OW_Y_END4_F1_WR(src)                        (((u32)(src)) & 0x00000fff)
#define OW_Y_END4_F1_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lcdc_owdbar14	*/ 
/*	 Fields ow_dbar	 */
#define OW_DBAR4_F1_WIDTH                                                    32
#define OW_DBAR4_F1_SHIFT                                                     0
#define OW_DBAR4_F1_MASK                                             0xffffffff
#define OW_DBAR4_F1_RD(src)                              (((src) & 0xffffffff))
#define OW_DBAR4_F1_WR(src)                         (((u32)(src)) & 0xffffffff)
#define OW_DBAR4_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register lcdc_owdcar14	*/ 
/*	 Fields OW_DCAR	 */
#define OW_DCAR4_F1_WIDTH                                                    32
#define OW_DCAR4_F1_SHIFT                                                     0
#define OW_DCAR4_F1_MASK                                             0xffffffff
#define OW_DCAR4_F1_RD(src)                              (((src) & 0xffffffff))
#define OW_DCAR4_F1_WR(src)                         (((u32)(src)) & 0xffffffff)
#define OW_DCAR4_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register lcdc_oxser15	*/ 
/*	 Fields ow_x_start	 */
#define OW_X_START5_F1_WIDTH                                                 12
#define OW_X_START5_F1_SHIFT                                                 16
#define OW_X_START5_F1_MASK                                          0x0fff0000
#define OW_X_START5_F1_RD(src)                       (((src) & 0x0fff0000)>>16)
#define OW_X_START5_F1_WR(src)                  (((u32)(src)<<16) & 0x0fff0000)
#define OW_X_START5_F1_SET(dst,src) \
                      (((dst) & ~0x0fff0000) | (((u32)(src)<<16) & 0x0fff0000))
/*	 Fields ow_x_end	 */
#define OW_X_END5_F1_WIDTH                                                   12
#define OW_X_END5_F1_SHIFT                                                    0
#define OW_X_END5_F1_MASK                                            0x00000fff
#define OW_X_END5_F1_RD(src)                             (((src) & 0x00000fff))
#define OW_X_END5_F1_WR(src)                        (((u32)(src)) & 0x00000fff)
#define OW_X_END5_F1_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lcdc_oyser15	*/ 
/*	 Fields ow_y_start	 */
#define OW_Y_START5_F1_WIDTH                                                 12
#define OW_Y_START5_F1_SHIFT                                                 16
#define OW_Y_START5_F1_MASK                                          0x0fff0000
#define OW_Y_START5_F1_RD(src)                       (((src) & 0x0fff0000)>>16)
#define OW_Y_START5_F1_WR(src)                  (((u32)(src)<<16) & 0x0fff0000)
#define OW_Y_START5_F1_SET(dst,src) \
                      (((dst) & ~0x0fff0000) | (((u32)(src)<<16) & 0x0fff0000))
/*	 Fields ow_y_end	 */
#define OW_Y_END5_F1_WIDTH                                                   12
#define OW_Y_END5_F1_SHIFT                                                    0
#define OW_Y_END5_F1_MASK                                            0x00000fff
#define OW_Y_END5_F1_RD(src)                             (((src) & 0x00000fff))
#define OW_Y_END5_F1_WR(src)                        (((u32)(src)) & 0x00000fff)
#define OW_Y_END5_F1_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lcdc_owdbar15	*/ 
/*	 Fields ow_dbar	 */
#define OW_DBAR5_F1_WIDTH                                                    32
#define OW_DBAR5_F1_SHIFT                                                     0
#define OW_DBAR5_F1_MASK                                             0xffffffff
#define OW_DBAR5_F1_RD(src)                              (((src) & 0xffffffff))
#define OW_DBAR5_F1_WR(src)                         (((u32)(src)) & 0xffffffff)
#define OW_DBAR5_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register lcdc_owdcar15	*/ 
/*	 Fields ow_dcar	 */
#define OW_DCAR5_F1_WIDTH                                                    32
#define OW_DCAR5_F1_SHIFT                                                     0
#define OW_DCAR5_F1_MASK                                             0xffffffff
#define OW_DCAR5_F1_RD(src)                              (((src) & 0xffffffff))
#define OW_DCAR5_F1_WR(src)                         (((u32)(src)) & 0xffffffff)
#define OW_DCAR5_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CSC_CR1	*/ 
/*	 Fields OTR	 */
#define OTR1_WIDTH                                                            1
#define OTR1_SHIFT                                                            2
#define OTR1_MASK                                                    0x00000004
#define OTR1_RD(src)                                  (((src) & 0x00000004)>>2)
#define OTR1_WR(src)                             (((u32)(src)<<2) & 0x00000004)
#define OTR1_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields CDE	 */
#define CDE1_WIDTH                                                            1
#define CDE1_SHIFT                                                            1
#define CDE1_MASK                                                    0x00000002
#define CDE1_RD(src)                                  (((src) & 0x00000002)>>1)
#define CDE1_WR(src)                             (((u32)(src)<<1) & 0x00000002)
#define CDE1_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields CCE	 */
#define CCE1_WIDTH                                                            1
#define CCE1_SHIFT                                                            0
#define CCE1_MASK                                                    0x00000001
#define CCE1_RD(src)                                     (((src) & 0x00000001))
#define CCE1_WR(src)                                (((u32)(src)) & 0x00000001)
#define CCE1_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register CSC_KA1	*/ 
/*	 Fields KA1	 */
#define KA11_WIDTH                                                           10
#define KA11_SHIFT                                                            0
#define KA11_MASK                                                    0x000003ff
#define KA11_RD(src)                                     (((src) & 0x000003ff))
#define KA11_WR(src)                                (((u32)(src)) & 0x000003ff)
#define KA11_SET(dst,src) \
                          (((dst) & ~0x000003ff) | (((u32)(src)) & 0x000003ff))

/*	Register CSC_KB1	*/ 
/*	 Fields KB1	 */
#define KB11_WIDTH                                                           10
#define KB11_SHIFT                                                            0
#define KB11_MASK                                                    0x000003ff
#define KB11_RD(src)                                     (((src) & 0x000003ff))
#define KB11_WR(src)                                (((u32)(src)) & 0x000003ff)
#define KB11_SET(dst,src) \
                          (((dst) & ~0x000003ff) | (((u32)(src)) & 0x000003ff))

/*	Register CSC_KC1	*/ 
/*	 Fields KC1	 */
#define KC11_WIDTH                                                           10
#define KC11_SHIFT                                                            0
#define KC11_MASK                                                    0x000003ff
#define KC11_RD(src)                                     (((src) & 0x000003ff))
#define KC11_WR(src)                                (((u32)(src)) & 0x000003ff)
#define KC11_SET(dst,src) \
                          (((dst) & ~0x000003ff) | (((u32)(src)) & 0x000003ff))

/*	Register CSC_KA2	*/ 
/*	 Fields KA2	 */
#define KA22_WIDTH                                                           10
#define KA22_SHIFT                                                            0
#define KA22_MASK                                                    0x000003ff
#define KA22_RD(src)                                     (((src) & 0x000003ff))
#define KA22_WR(src)                                (((u32)(src)) & 0x000003ff)
#define KA22_SET(dst,src) \
                          (((dst) & ~0x000003ff) | (((u32)(src)) & 0x000003ff))

/*	Register CSC_KB2	*/ 
/*	 Fields KB2	 */
#define KB22_WIDTH                                                           10
#define KB22_SHIFT                                                            0
#define KB22_MASK                                                    0x000003ff
#define KB22_RD(src)                                     (((src) & 0x000003ff))
#define KB22_WR(src)                                (((u32)(src)) & 0x000003ff)
#define KB22_SET(dst,src) \
                          (((dst) & ~0x000003ff) | (((u32)(src)) & 0x000003ff))

/*	Register CSC_KC2	*/ 
/*	 Fields KC2	 */
#define KC22_WIDTH                                                           10
#define KC22_SHIFT                                                            0
#define KC22_MASK                                                    0x000003ff
#define KC22_RD(src)                                     (((src) & 0x000003ff))
#define KC22_WR(src)                                (((u32)(src)) & 0x000003ff)
#define KC22_SET(dst,src) \
                          (((dst) & ~0x000003ff) | (((u32)(src)) & 0x000003ff))

/*	Register CSC_KA3	*/ 
/*	 Fields KA3	 */
#define KA33_WIDTH                                                           10
#define KA33_SHIFT                                                            0
#define KA33_MASK                                                    0x000003ff
#define KA33_RD(src)                                     (((src) & 0x000003ff))
#define KA33_WR(src)                                (((u32)(src)) & 0x000003ff)
#define KA33_SET(dst,src) \
                          (((dst) & ~0x000003ff) | (((u32)(src)) & 0x000003ff))

/*	Register CSC_KB3	*/ 
/*	 Fields KB3	 */
#define KB33_WIDTH                                                           10
#define KB33_SHIFT                                                            0
#define KB33_MASK                                                    0x000003ff
#define KB33_RD(src)                                     (((src) & 0x000003ff))
#define KB33_WR(src)                                (((u32)(src)) & 0x000003ff)
#define KB33_SET(dst,src) \
                          (((dst) & ~0x000003ff) | (((u32)(src)) & 0x000003ff))

/*	Register CSC_KC3	*/ 
/*	 Fields KC3	 */
#define KC33_WIDTH                                                           10
#define KC33_SHIFT                                                            0
#define KC33_MASK                                                    0x000003ff
#define KC33_RD(src)                                     (((src) & 0x000003ff))
#define KC33_WR(src)                                (((u32)(src)) & 0x000003ff)
#define KC33_SET(dst,src) \
                          (((dst) & ~0x000003ff) | (((u32)(src)) & 0x000003ff))

/*	Register CSC_Sx	*/ 
/*	 Fields Sx	 */
#define SX_WIDTH                                                             10
#define SX_SHIFT                                                              0
#define SX_MASK                                                      0x000003ff
#define SX_RD(src)                                       (((src) & 0x000003ff))
#define SX_WR(src)                                  (((u32)(src)) & 0x000003ff)
#define SX_SET(dst,src) (((dst) & ~0x000003ff) | (((u32)(src)) & 0x000003ff))

/*	Register CSC_Sy	*/ 
/*	 Fields Sy	 */
#define SY_WIDTH                                                             10
#define SY_SHIFT                                                              0
#define SY_MASK                                                      0x000003ff
#define SY_RD(src)                                       (((src) & 0x000003ff))
#define SY_WR(src)                                  (((u32)(src)) & 0x000003ff)
#define SY_SET(dst,src) (((dst) & ~0x000003ff) | (((u32)(src)) & 0x000003ff))

/*	Register CSC_Sz	*/ 
/*	 Fields Sz	 */
#define SZ_WIDTH                                                             10
#define SZ_SHIFT                                                              0
#define SZ_MASK                                                      0x000003ff
#define SZ_RD(src)                                       (((src) & 0x000003ff))
#define SZ_WR(src)                                  (((u32)(src)) & 0x000003ff)
#define SZ_SET(dst,src) (((dst) & ~0x000003ff) | (((u32)(src)) & 0x000003ff))

/*	Register CR_CR1	*/ 
/*	 Fields OTR	 */
#define OTR1_F1_WIDTH                                                         1
#define OTR1_F1_SHIFT                                                         3
#define OTR1_F1_MASK                                                 0x00000008
#define OTR1_F1_RD(src)                               (((src) & 0x00000008)>>3)
#define OTR1_F1_WR(src)                          (((u32)(src)<<3) & 0x00000008)
#define OTR1_F1_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields CRF	 */
#define CRF1_WIDTH                                                            1
#define CRF1_SHIFT                                                            2
#define CRF1_MASK                                                    0x00000004
#define CRF1_RD(src)                                  (((src) & 0x00000004)>>2)
#define CRF1_WR(src)                             (((u32)(src)<<2) & 0x00000004)
#define CRF1_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields CDE	 */
#define CDE1_F1_WIDTH                                                         1
#define CDE1_F1_SHIFT                                                         1
#define CDE1_F1_MASK                                                 0x00000002
#define CDE1_F1_RD(src)                               (((src) & 0x00000002)>>1)
#define CDE1_F1_WR(src)                          (((u32)(src)<<1) & 0x00000002)
#define CDE1_F1_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields CRE	 */
#define CRE1_WIDTH                                                            1
#define CRE1_SHIFT                                                            0
#define CRE1_MASK                                                    0x00000001
#define CRE1_RD(src)                                     (((src) & 0x00000001))
#define CRE1_WR(src)                                (((u32)(src)) & 0x00000001)
#define CRE1_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register CR_FIR_CE0	*/ 
/*	 Fields CE0	 */
#define CE00_WIDTH                                                            3
#define CE00_SHIFT                                                            0
#define CE00_MASK                                                    0x00000007
#define CE00_RD(src)                                     (((src) & 0x00000007))
#define CE00_WR(src)                                (((u32)(src)) & 0x00000007)
#define CE00_SET(dst,src) \
                          (((dst) & ~0x00000007) | (((u32)(src)) & 0x00000007))

/*	Register CR_FIR_CE1	*/ 
/*	 Fields CE1	 */
#define CE11_WIDTH                                                            3
#define CE11_SHIFT                                                            0
#define CE11_MASK                                                    0x00000007
#define CE11_RD(src)                                     (((src) & 0x00000007))
#define CE11_WR(src)                                (((u32)(src)) & 0x00000007)
#define CE11_SET(dst,src) \
                          (((dst) & ~0x00000007) | (((u32)(src)) & 0x00000007))

/*	Register CR_FIR_CE2	*/ 
/*	 Fields CE2	 */
#define CE22_WIDTH                                                            3
#define CE22_SHIFT                                                            0
#define CE22_MASK                                                    0x00000007
#define CE22_RD(src)                                     (((src) & 0x00000007))
#define CE22_WR(src)                                (((u32)(src)) & 0x00000007)
#define CE22_SET(dst,src) \
                          (((dst) & ~0x00000007) | (((u32)(src)) & 0x00000007))

/*	Register BT656E_CR1	*/ 
/*	 Fields BPS	 */
#define BPS1_WIDTH                                                            1
#define BPS1_SHIFT                                                            9
#define BPS1_MASK                                                    0x00000200
#define BPS1_RD(src)                                  (((src) & 0x00000200)>>9)
#define BPS1_WR(src)                             (((u32)(src)<<9) & 0x00000200)
#define BPS1_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields BCP	 */
#define BCP1_WIDTH                                                            1
#define BCP1_SHIFT                                                            8
#define BCP1_MASK                                                    0x00000100
#define BCP1_RD(src)                                  (((src) & 0x00000100)>>8)
#define BCP1_WR(src)                             (((u32)(src)<<8) & 0x00000100)
#define BCP1_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*	 Fields BVP	 */
#define BVP1_WIDTH                                                            1
#define BVP1_SHIFT                                                            7
#define BVP1_MASK                                                    0x00000080
#define BVP1_RD(src)                                  (((src) & 0x00000080)>>7)
#define BVP1_WR(src)                             (((u32)(src)<<7) & 0x00000080)
#define BVP1_SET(dst,src) \
                       (((dst) & ~0x00000080) | (((u32)(src)<<7) & 0x00000080))
/*	 Fields BHP	 */
#define BHP1_WIDTH                                                            1
#define BHP1_SHIFT                                                            6
#define BHP1_MASK                                                    0x00000040
#define BHP1_RD(src)                                  (((src) & 0x00000040)>>6)
#define BHP1_WR(src)                             (((u32)(src)<<6) & 0x00000040)
#define BHP1_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*	 Fields BDE	 */
#define BDE1_WIDTH                                                            1
#define BDE1_SHIFT                                                            5
#define BDE1_MASK                                                    0x00000020
#define BDE1_RD(src)                                  (((src) & 0x00000020)>>5)
#define BDE1_WR(src)                             (((u32)(src)<<5) & 0x00000020)
#define BDE1_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields BSF	 */
#define BSF1_WIDTH                                                            2
#define BSF1_SHIFT                                                            3
#define BSF1_MASK                                                    0x00000018
#define BSF1_RD(src)                                  (((src) & 0x00000018)>>3)
#define BSF1_WR(src)                             (((u32)(src)<<3) & 0x00000018)
#define BSF1_SET(dst,src) \
                       (((dst) & ~0x00000018) | (((u32)(src)<<3) & 0x00000018))
/*	 Fields BFF	 */
#define BFF1_WIDTH                                                            1
#define BFF1_SHIFT                                                            2
#define BFF1_MASK                                                    0x00000004
#define BFF1_RD(src)                                  (((src) & 0x00000004)>>2)
#define BFF1_WR(src)                             (((u32)(src)<<2) & 0x00000004)
#define BFF1_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields BVF	 */
#define BVF1_WIDTH                                                            1
#define BVF1_SHIFT                                                            1
#define BVF1_MASK                                                    0x00000002
#define BVF1_RD(src)                                  (((src) & 0x00000002)>>1)
#define BVF1_WR(src)                             (((u32)(src)<<1) & 0x00000002)
#define BVF1_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields BEE	 */
#define BEE1_WIDTH                                                            1
#define BEE1_SHIFT                                                            0
#define BEE1_MASK                                                    0x00000001
#define BEE1_RD(src)                                     (((src) & 0x00000001))
#define BEE1_WR(src)                                (((u32)(src)) & 0x00000001)
#define BEE1_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register BT656E_VHDO	*/ 
/*	 Fields HDO	 */
#define HDO_WIDTH                                                             9
#define HDO_SHIFT                                                            16
#define HDO_MASK                                                     0x01ff0000
#define HDO_RD(src)                                  (((src) & 0x01ff0000)>>16)
#define HDO_WR(src)                             (((u32)(src)<<16) & 0x01ff0000)
#define HDO_SET(dst,src) \
                      (((dst) & ~0x01ff0000) | (((u32)(src)<<16) & 0x01ff0000))
/*	 Fields VDO	 */
#define VDO_WIDTH                                                             6
#define VDO_SHIFT                                                             0
#define VDO_MASK                                                     0x0000003f
#define VDO_RD(src)                                      (((src) & 0x0000003f))
#define VDO_WR(src)                                 (((u32)(src)) & 0x0000003f)
#define VDO_SET(dst,src) (((dst) & ~0x0000003f) | (((u32)(src)) & 0x0000003f))

/*	Register BT656E_FSO	*/ 
/*	 Fields PEF	 */
#define PEF_WIDTH                                                             6
#define PEF_SHIFT                                                            16
#define PEF_MASK                                                     0x003f0000
#define PEF_RD(src)                                  (((src) & 0x003f0000)>>16)
#define PEF_WR(src)                             (((u32)(src)<<16) & 0x003f0000)
#define PEF_SET(dst,src) \
                      (((dst) & ~0x003f0000) | (((u32)(src)<<16) & 0x003f0000))
/*	 Fields FSO	 */
#define FSO_WIDTH                                                            12
#define FSO_SHIFT                                                             0
#define FSO_MASK                                                     0x00000fff
#define FSO_RD(src)                                      (((src) & 0x00000fff))
#define FSO_WR(src)                                 (((u32)(src)) & 0x00000fff)
#define FSO_SET(dst,src) (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register ISR	*/ 
/*	 Fields FER	 */
#define FER_F1_WIDTH                                                          1
#define FER_F1_SHIFT                                                          2
#define FER_F1_MASK                                                  0x00000004
#define FER_F1_RD(src)                                (((src) & 0x00000004)>>2)
#define FER_F1_WR(src)                           (((u32)(src)<<2) & 0x00000004)
#define FER_F1_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields IFO	 */
#define IFO_F1_WIDTH                                                          1
#define IFO_F1_SHIFT                                                          1
#define IFO_F1_MASK                                                  0x00000002
#define IFO_F1_RD(src)                                (((src) & 0x00000002)>>1)
#define IFO_F1_WR(src)                           (((u32)(src)<<1) & 0x00000002)
#define IFO_F1_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields IFU	 */
#define IFU_F1_WIDTH                                                          1
#define IFU_F1_SHIFT                                                          0
#define IFU_F1_MASK                                                  0x00000001
#define IFU_F1_RD(src)                                   (((src) & 0x00000001))
#define IFU_F1_WR(src)                              (((u32)(src)) & 0x00000001)
#define IFU_F1_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register IMR	*/ 
/*	 Fields FERM	 */
#define FERM_F1_WIDTH                                                         1
#define FERM_F1_SHIFT                                                         2
#define FERM_F1_MASK                                                 0x00000004
#define FERM_F1_RD(src)                               (((src) & 0x00000004)>>2)
#define FERM_F1_WR(src)                          (((u32)(src)<<2) & 0x00000004)
#define FERM_F1_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields IFOM	 */
#define IFOM_F1_WIDTH                                                         1
#define IFOM_F1_SHIFT                                                         1
#define IFOM_F1_MASK                                                 0x00000002
#define IFOM_F1_RD(src)                               (((src) & 0x00000002)>>1)
#define IFOM_F1_WR(src)                          (((u32)(src)<<1) & 0x00000002)
#define IFOM_F1_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields IFUM	 */
#define IFUM_F1_WIDTH                                                         1
#define IFUM_F1_SHIFT                                                         0
#define IFUM_F1_MASK                                                 0x00000001
#define IFUM_F1_RD(src)                                  (((src) & 0x00000001))
#define IFUM_F1_WR(src)                             (((u32)(src)) & 0x00000001)
#define IFUM_F1_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register IVR	*/ 
/*	 Fields FERV	 */
#define FERV_F1_WIDTH                                                         1
#define FERV_F1_SHIFT                                                         2
#define FERV_F1_MASK                                                 0x00000004
#define FERV_F1_RD(src)                               (((src) & 0x00000004)>>2)
#define FERV_F1_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields IFOV	 */
#define IFOV_F1_WIDTH                                                         1
#define IFOV_F1_SHIFT                                                         1
#define IFOV_F1_MASK                                                 0x00000002
#define IFOV_F1_RD(src)                               (((src) & 0x00000002)>>1)
#define IFOV_F1_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields IFUV	 */
#define IFUV_F1_WIDTH                                                         1
#define IFUV_F1_SHIFT                                                         0
#define IFUV_F1_MASK                                                 0x00000001
#define IFUV_F1_RD(src)                                  (((src) & 0x00000001))
#define IFUV_F1_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Global Base Address	*/
#define LCD_GLBL_DIAG_CSR_BASE_ADDR			0xddd817000ULL

/*    Address GLBL_DIAG_CSR  Registers */
#define LCD_CFG_DIAG_SEL_ADDR                                       0x00000000
#define LCD_CFG_DIAG_SEL_DEFAULT                                    0x00000000
#define LCD_CFG_READ_BW_LAT_ADDR_MASK_ADDR                          0x00000004
#define LCD_CFG_READ_BW_LAT_ADDR_MASK_DEFAULT                       0x00000000
#define LCD_CFG_READ_BW_LAT_ADDR_PAT_ADDR                           0x00000008
#define LCD_CFG_READ_BW_LAT_ADDR_PAT_DEFAULT                        0xffffffff
#define LCD_CFG_WRITE_BW_LAT_ADDR_MASK_ADDR                         0x0000000c
#define LCD_CFG_WRITE_BW_LAT_ADDR_MASK_DEFAULT                      0x00000000
#define LCD_CFG_WRITE_BW_LAT_ADDR_PAT_ADDR                          0x00000010
#define LCD_CFG_WRITE_BW_LAT_ADDR_PAT_DEFAULT                       0xffffffff
#define LCD_CFG_DIAG_START_STOP_ADDR                                0x00000014
#define LCD_CFG_DIAG_START_STOP_DEFAULT                             0x0000001f
#define LCD_CFG_BW_MSTR_STOP_CNT_ADDR                               0x00000018
#define LCD_CFG_BW_MSTR_STOP_CNT_DEFAULT                            0x00040004
#define LCD_CFG_BW_SLV_STOP_CNT_ADDR                                0x0000001c
#define LCD_CFG_BW_SLV_STOP_CNT_DEFAULT                             0x00040004
#define LCD_STS_READ_LATENCY_OUTPUT_ADDR                            0x00000020
#define LCD_STS_READ_LATENCY_OUTPUT_DEFAULT                         0x00000000
#define LCD_STS_AXI_MRD_BW_CLK_CNT_ADDR                             0x00000024
#define LCD_STS_AXI_MRD_BW_CLK_CNT_DEFAULT                          0x00000000
#define LCD_STS_AXI_MRD_BW_BYTE_CNT_ADDR                            0x00000028
#define LCD_STS_AXI_MRD_BW_BYTE_CNT_DEFAULT                         0x00000000
#define LCD_STS_AXI_MWR_BW_CLK_CNT_ADDR                             0x0000002c
#define LCD_STS_AXI_MWR_BW_CLK_CNT_DEFAULT                          0x00000000
#define LCD_STS_AXI_MWR_BW_BYTE_CNT_ADDR                            0x00000030
#define LCD_STS_AXI_MWR_BW_BYTE_CNT_DEFAULT                         0x00000000
#define LCD_STS_AXI_SRD_BW_CLK_CNT_ADDR                             0x00000034
#define LCD_STS_AXI_SRD_BW_CLK_CNT_DEFAULT                          0x00000000
#define LCD_STS_AXI_SRD_BW_BYTE_CNT_ADDR                            0x00000038
#define LCD_STS_AXI_SRD_BW_BYTE_CNT_DEFAULT                         0x00000000
#define LCD_STS_AXI_SWR_BW_CLK_CNT_ADDR                             0x0000003c
#define LCD_STS_AXI_SWR_BW_CLK_CNT_DEFAULT                          0x00000000
#define LCD_STS_AXI_SWR_BW_BYTE_CNT_ADDR                            0x00000040
#define LCD_STS_AXI_SWR_BW_BYTE_CNT_DEFAULT                         0x00000000
#define LCD_CFG_DBG_TRIG_CTRL_ADDR                                  0x00000044
#define LCD_CFG_DBG_TRIG_CTRL_DEFAULT                               0x00000000
#define LCD_CFG_DBG_PAT_REG_0_ADDR                                  0x00000048
#define LCD_CFG_DBG_PAT_REG_0_DEFAULT                               0x00000000
#define LCD_CFG_DBG_PAT_MASK_REG_0_ADDR                             0x0000004c
#define LCD_CFG_DBG_PAT_MASK_REG_0_DEFAULT                          0x00000000
#define LCD_CFG_DBG_PAT_REG_1_ADDR                                  0x00000050
#define LCD_CFG_DBG_PAT_REG_1_DEFAULT                               0x00000000
#define LCD_CFG_DBG_PAT_MASK_REG_1_ADDR                             0x00000054
#define LCD_CFG_DBG_PAT_MASK_REG_1_DEFAULT                          0x00000000
#define LCD_DBG_TRIG_OUT_ADDR                                       0x00000058
#define LCD_DBG_TRIG_OUT_DEFAULT                                    0x00000000
#define LCD_DBG_ECC_INT_OUT_ADDR                                    0x0000005c
#define LCD_DBG_ECC_INT_OUT_DEFAULT                                 0x00000000
#define LCD_DBG_ECC_INT_OUTMASK_ADDR                                0x00000060
#define LCD_CFG_SECC_INT_THRSH_REG_ADDR                             0x00000064
#define LCD_CFG_SECC_INT_THRSH_REG_DEFAULT                          0x000000fe
#define LCD_CFG_MEM_ECC_BYPASS_ADDR                                 0x00000068
#define LCD_CFG_MEM_ECC_BYPASS_DEFAULT                              0x00000000
#define LCD_CFG_MEM_PWRDN_DIS_ADDR                                  0x0000006c
#define LCD_CFG_MEM_PWRDN_DIS_DEFAULT                               0x00000000
#define LCD_CFG_MEM_RAM_SHUTDOWN_ADDR                               0x00000070
#define LCD_CFG_MEM_RAM_SHUTDOWN_DEFAULT                            0xffffffff
#define LCD_BLOCK_MEM_RDY_ADDR                                      0x00000074
#define LCD_BLOCK_MEM_RDY_DEFAULT                                   0xffffffff
#define LCD_CFG_AXI_SLV_RD_TMO_ADDR                                 0x00000078
#define LCD_CFG_AXI_SLV_RD_TMO_DEFAULT                              0x83ff83ff
#define LCD_CFG_AXI_SLV_WR_TMO_ADDR                                 0x0000007c
#define LCD_CFG_AXI_SLV_WR_TMO_DEFAULT                              0x83ff83ff
#define LCD_STS_AXI_SLV_TMO_ADDR                                    0x00000080
#define LCD_STS_AXI_SLV_TMO_DEFAULT                                 0x00000000
#define LCD_CFG_AXI_SS_CSR_TMO_ADDR                                 0x00000084
#define LCD_CFG_AXI_SS_CSR_TMO_DEFAULT                              0x02008000
#define LCD_STS_READ_LATENCY_TOT_READ_REQS_ADDR                     0x0000008c
#define LCD_STS_READ_LATENCY_TOT_READ_REQS_DEFAULT                  0x00000000

/*	Register CFG_DIAG_SEL	*/ 
/*	 Fields CFG_AXI_NON_AXI_MUX_SELECT	 */
#define LCD_CFG_AXI_NON_AXI_MUX_SELECT_WIDTH                                 1
#define LCD_CFG_AXI_NON_AXI_MUX_SELECT_SHIFT                                11
#define LCD_CFG_AXI_NON_AXI_MUX_SELECT_MASK                         0x00000800
#define LCD_CFG_AXI_NON_AXI_MUX_SELECT_RD(src)      (((src) & 0x00000800)>>11)
#define LCD_CFG_AXI_NON_AXI_MUX_SELECT_WR(src) \
                                                (((u32)(src)<<11) & 0x00000800)
#define LCD_CFG_AXI_NON_AXI_MUX_SELECT_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*	 Fields CFG_MUX_SELECTOR	 */
#define LCD_CFG_MUX_SELECTOR_WIDTH                                          11
#define LCD_CFG_MUX_SELECTOR_SHIFT                                           0
#define LCD_CFG_MUX_SELECTOR_MASK                                   0x000007ff
#define LCD_CFG_MUX_SELECTOR_RD(src)                    (((src) & 0x000007ff))
#define LCD_CFG_MUX_SELECTOR_WR(src)               (((u32)(src)) & 0x000007ff)
#define LCD_CFG_MUX_SELECTOR_SET(dst,src) \
                          (((dst) & ~0x000007ff) | (((u32)(src)) & 0x000007ff))

/*	Register CFG_READ_BW_LAT_ADDR_MASK	*/ 
/*	 Fields READ_ADDR_MASK	 */
#define LCD_READ_ADDR_MASK_WIDTH                                            32
#define LCD_READ_ADDR_MASK_SHIFT                                             0
#define LCD_READ_ADDR_MASK_MASK                                     0xffffffff
#define LCD_READ_ADDR_MASK_RD(src)                      (((src) & 0xffffffff))
#define LCD_READ_ADDR_MASK_WR(src)                 (((u32)(src)) & 0xffffffff)
#define LCD_READ_ADDR_MASK_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_READ_BW_LAT_ADDR_PAT	*/ 
/*	 Fields READ_ADDR_PAT	 */
#define LCD_READ_ADDR_PAT_WIDTH                                             32
#define LCD_READ_ADDR_PAT_SHIFT                                              0
#define LCD_READ_ADDR_PAT_MASK                                      0xffffffff
#define LCD_READ_ADDR_PAT_RD(src)                       (((src) & 0xffffffff))
#define LCD_READ_ADDR_PAT_WR(src)                  (((u32)(src)) & 0xffffffff)
#define LCD_READ_ADDR_PAT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_WRITE_BW_LAT_ADDR_MASK	*/ 
/*	 Fields WRITE_ADDR_MASK	 */
#define LCD_WRITE_ADDR_MASK_WIDTH                                           32
#define LCD_WRITE_ADDR_MASK_SHIFT                                            0
#define LCD_WRITE_ADDR_MASK_MASK                                    0xffffffff
#define LCD_WRITE_ADDR_MASK_RD(src)                     (((src) & 0xffffffff))
#define LCD_WRITE_ADDR_MASK_WR(src)                (((u32)(src)) & 0xffffffff)
#define LCD_WRITE_ADDR_MASK_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_WRITE_BW_LAT_ADDR_PAT	*/ 
/*	 Fields WRITE_ADDR_PAT	 */
#define LCD_WRITE_ADDR_PAT_WIDTH                                            32
#define LCD_WRITE_ADDR_PAT_SHIFT                                             0
#define LCD_WRITE_ADDR_PAT_MASK                                     0xffffffff
#define LCD_WRITE_ADDR_PAT_RD(src)                      (((src) & 0xffffffff))
#define LCD_WRITE_ADDR_PAT_WR(src)                 (((u32)(src)) & 0xffffffff)
#define LCD_WRITE_ADDR_PAT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_DIAG_START_STOP	*/ 
/*	 Fields START_RD_LATENCY	 */
#define LCD_START_RD_LATENCY_WIDTH                                           1
#define LCD_START_RD_LATENCY_SHIFT                                           4
#define LCD_START_RD_LATENCY_MASK                                   0x00000010
#define LCD_START_RD_LATENCY_RD(src)                 (((src) & 0x00000010)>>4)
#define LCD_START_RD_LATENCY_WR(src)            (((u32)(src)<<4) & 0x00000010)
#define LCD_START_RD_LATENCY_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields START_SRD_BW	 */
#define LCD_START_SRD_BW_WIDTH                                               1
#define LCD_START_SRD_BW_SHIFT                                               3
#define LCD_START_SRD_BW_MASK                                       0x00000008
#define LCD_START_SRD_BW_RD(src)                     (((src) & 0x00000008)>>3)
#define LCD_START_SRD_BW_WR(src)                (((u32)(src)<<3) & 0x00000008)
#define LCD_START_SRD_BW_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields START_MRD_BW	 */
#define LCD_START_MRD_BW_WIDTH                                               1
#define LCD_START_MRD_BW_SHIFT                                               2
#define LCD_START_MRD_BW_MASK                                       0x00000004
#define LCD_START_MRD_BW_RD(src)                     (((src) & 0x00000004)>>2)
#define LCD_START_MRD_BW_WR(src)                (((u32)(src)<<2) & 0x00000004)
#define LCD_START_MRD_BW_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields START_SWR_BW	 */
#define LCD_START_SWR_BW_WIDTH                                               1
#define LCD_START_SWR_BW_SHIFT                                               1
#define LCD_START_SWR_BW_MASK                                       0x00000002
#define LCD_START_SWR_BW_RD(src)                     (((src) & 0x00000002)>>1)
#define LCD_START_SWR_BW_WR(src)                (((u32)(src)<<1) & 0x00000002)
#define LCD_START_SWR_BW_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields START_MWR_BW	 */
#define LCD_START_MWR_BW_WIDTH                                               1
#define LCD_START_MWR_BW_SHIFT                                               0
#define LCD_START_MWR_BW_MASK                                       0x00000001
#define LCD_START_MWR_BW_RD(src)                        (((src) & 0x00000001))
#define LCD_START_MWR_BW_WR(src)                   (((u32)(src)) & 0x00000001)
#define LCD_START_MWR_BW_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register CFG_BW_MSTR_STOP_CNT	*/ 
/*	 Fields MSTR_STOP_RD_CNT	 */
#define LCD_MSTR_STOP_RD_CNT_WIDTH                                          16
#define LCD_MSTR_STOP_RD_CNT_SHIFT                                          16
#define LCD_MSTR_STOP_RD_CNT_MASK                                   0xffff0000
#define LCD_MSTR_STOP_RD_CNT_RD(src)                (((src) & 0xffff0000)>>16)
#define LCD_MSTR_STOP_RD_CNT_WR(src)           (((u32)(src)<<16) & 0xffff0000)
#define LCD_MSTR_STOP_RD_CNT_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields MSTR_STOP_WR_CNT	 */
#define LCD_MSTR_STOP_WR_CNT_WIDTH                                          16
#define LCD_MSTR_STOP_WR_CNT_SHIFT                                           0
#define LCD_MSTR_STOP_WR_CNT_MASK                                   0x0000ffff
#define LCD_MSTR_STOP_WR_CNT_RD(src)                    (((src) & 0x0000ffff))
#define LCD_MSTR_STOP_WR_CNT_WR(src)               (((u32)(src)) & 0x0000ffff)
#define LCD_MSTR_STOP_WR_CNT_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register CFG_BW_SLV_STOP_CNT	*/ 
/*	 Fields SLV_STOP_RD_CNT	 */
#define LCD_SLV_STOP_RD_CNT_WIDTH                                           16
#define LCD_SLV_STOP_RD_CNT_SHIFT                                           16
#define LCD_SLV_STOP_RD_CNT_MASK                                    0xffff0000
#define LCD_SLV_STOP_RD_CNT_RD(src)                 (((src) & 0xffff0000)>>16)
#define LCD_SLV_STOP_RD_CNT_WR(src)            (((u32)(src)<<16) & 0xffff0000)
#define LCD_SLV_STOP_RD_CNT_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields SLV_STOP_WR_CNT	 */
#define LCD_SLV_STOP_WR_CNT_WIDTH                                           16
#define LCD_SLV_STOP_WR_CNT_SHIFT                                            0
#define LCD_SLV_STOP_WR_CNT_MASK                                    0x0000ffff
#define LCD_SLV_STOP_WR_CNT_RD(src)                     (((src) & 0x0000ffff))
#define LCD_SLV_STOP_WR_CNT_WR(src)                (((u32)(src)) & 0x0000ffff)
#define LCD_SLV_STOP_WR_CNT_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register STS_READ_LATENCY_OUTPUT	*/ 
/*	 Fields READ_MAX	 */
#define LCD_READ_MAX_WIDTH                                                  10
#define LCD_READ_MAX_SHIFT                                                  22
#define LCD_READ_MAX_MASK                                           0xffc00000
#define LCD_READ_MAX_RD(src)                        (((src) & 0xffc00000)>>22)
#define LCD_READ_MAX_SET(dst,src) \
                      (((dst) & ~0xffc00000) | (((u32)(src)<<22) & 0xffc00000))
/*	 Fields READ_TOT	 */
#define LCD_READ_TOT_WIDTH                                                  22
#define LCD_READ_TOT_SHIFT                                                   0
#define LCD_READ_TOT_MASK                                           0x003fffff
#define LCD_READ_TOT_RD(src)                            (((src) & 0x003fffff))
#define LCD_READ_TOT_SET(dst,src) \
                          (((dst) & ~0x003fffff) | (((u32)(src)) & 0x003fffff))

/*	Register STS_AXI_MRD_BW_CLK_CNT	*/ 
/*	 Fields MSTR_READ_BW_CLK_CNT	 */
#define LCD_MSTR_READ_BW_CLK_CNT_WIDTH                                      32
#define LCD_MSTR_READ_BW_CLK_CNT_SHIFT                                       0
#define LCD_MSTR_READ_BW_CLK_CNT_MASK                               0xffffffff
#define LCD_MSTR_READ_BW_CLK_CNT_RD(src)                (((src) & 0xffffffff))
#define LCD_MSTR_READ_BW_CLK_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register STS_AXI_MRD_BW_BYTE_CNT	*/ 
/*	 Fields MSTR_READ_BW_BYTE_CNT	 */
#define LCD_MSTR_READ_BW_BYTE_CNT_WIDTH                                     32
#define LCD_MSTR_READ_BW_BYTE_CNT_SHIFT                                      0
#define LCD_MSTR_READ_BW_BYTE_CNT_MASK                              0xffffffff
#define LCD_MSTR_READ_BW_BYTE_CNT_RD(src)               (((src) & 0xffffffff))
#define LCD_MSTR_READ_BW_BYTE_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register STS_AXI_MWR_BW_CLK_CNT	*/ 
/*	 Fields MSTR_WRITE_BW_CLK_CNT	 */
#define LCD_MSTR_WRITE_BW_CLK_CNT_WIDTH                                     32
#define LCD_MSTR_WRITE_BW_CLK_CNT_SHIFT                                      0
#define LCD_MSTR_WRITE_BW_CLK_CNT_MASK                              0xffffffff
#define LCD_MSTR_WRITE_BW_CLK_CNT_RD(src)               (((src) & 0xffffffff))
#define LCD_MSTR_WRITE_BW_CLK_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register STS_AXI_MWR_BW_BYTE_CNT	*/ 
/*	 Fields MSTR_WRITE_BW_BYTE_CNT	 */
#define LCD_MSTR_WRITE_BW_BYTE_CNT_WIDTH                                    32
#define LCD_MSTR_WRITE_BW_BYTE_CNT_SHIFT                                     0
#define LCD_MSTR_WRITE_BW_BYTE_CNT_MASK                             0xffffffff
#define LCD_MSTR_WRITE_BW_BYTE_CNT_RD(src)              (((src) & 0xffffffff))
#define LCD_MSTR_WRITE_BW_BYTE_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register STS_AXI_SRD_BW_CLK_CNT	*/ 
/*	 Fields SLV_READ_BW_CLK_CNT	 */
#define LCD_SLV_READ_BW_CLK_CNT_WIDTH                                       32
#define LCD_SLV_READ_BW_CLK_CNT_SHIFT                                        0
#define LCD_SLV_READ_BW_CLK_CNT_MASK                                0xffffffff
#define LCD_SLV_READ_BW_CLK_CNT_RD(src)                 (((src) & 0xffffffff))
#define LCD_SLV_READ_BW_CLK_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register STS_AXI_SRD_BW_BYTE_CNT	*/ 
/*	 Fields SLV_READ_BW_BYTE_CNT	 */
#define LCD_SLV_READ_BW_BYTE_CNT_WIDTH                                      32
#define LCD_SLV_READ_BW_BYTE_CNT_SHIFT                                       0
#define LCD_SLV_READ_BW_BYTE_CNT_MASK                               0xffffffff
#define LCD_SLV_READ_BW_BYTE_CNT_RD(src)                (((src) & 0xffffffff))
#define LCD_SLV_READ_BW_BYTE_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register STS_AXI_SWR_BW_CLK_CNT	*/ 
/*	 Fields SLV_WRITE_BW_CLK_CNT	 */
#define LCD_SLV_WRITE_BW_CLK_CNT_WIDTH                                      32
#define LCD_SLV_WRITE_BW_CLK_CNT_SHIFT                                       0
#define LCD_SLV_WRITE_BW_CLK_CNT_MASK                               0xffffffff
#define LCD_SLV_WRITE_BW_CLK_CNT_RD(src)                (((src) & 0xffffffff))
#define LCD_SLV_WRITE_BW_CLK_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register STS_AXI_SWR_BW_BYTE_CNT	*/ 
/*	 Fields SLV_WRITE_BW_BYTE_CNT	 */
#define LCD_SLV_WRITE_BW_BYTE_CNT_WIDTH                                     32
#define LCD_SLV_WRITE_BW_BYTE_CNT_SHIFT                                      0
#define LCD_SLV_WRITE_BW_BYTE_CNT_MASK                              0xffffffff
#define LCD_SLV_WRITE_BW_BYTE_CNT_RD(src)               (((src) & 0xffffffff))
#define LCD_SLV_WRITE_BW_BYTE_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_DBG_TRIG_CTRL	*/ 
/*	 Fields TRIG_EN	 */
#define LCD_TRIG_EN_WIDTH                                                    1
#define LCD_TRIG_EN_SHIFT                                                    4
#define LCD_TRIG_EN_MASK                                            0x00000010
#define LCD_TRIG_EN_RD(src)                          (((src) & 0x00000010)>>4)
#define LCD_TRIG_EN_WR(src)                     (((u32)(src)<<4) & 0x00000010)
#define LCD_TRIG_EN_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields AND_E	 */
#define LCD_AND_E_WIDTH                                                      2
#define LCD_AND_E_SHIFT                                                      2
#define LCD_AND_E_MASK                                              0x0000000c
#define LCD_AND_E_RD(src)                            (((src) & 0x0000000c)>>2)
#define LCD_AND_E_WR(src)                       (((u32)(src)<<2) & 0x0000000c)
#define LCD_AND_E_SET(dst,src) \
                       (((dst) & ~0x0000000c) | (((u32)(src)<<2) & 0x0000000c))
/*	 Fields OR_E	 */
#define LCD_OR_E_WIDTH                                                       2
#define LCD_OR_E_SHIFT                                                       0
#define LCD_OR_E_MASK                                               0x00000003
#define LCD_OR_E_RD(src)                                (((src) & 0x00000003))
#define LCD_OR_E_WR(src)                           (((u32)(src)) & 0x00000003)
#define LCD_OR_E_SET(dst,src) \
                          (((dst) & ~0x00000003) | (((u32)(src)) & 0x00000003))

/*	Register CFG_DBG_PAT_REG_0	*/ 
/*	 Fields PATTERN	 */
#define LCD_PATTERN0_WIDTH                                                  32
#define LCD_PATTERN0_SHIFT                                                   0
#define LCD_PATTERN0_MASK                                           0xffffffff
#define LCD_PATTERN0_RD(src)                            (((src) & 0xffffffff))
#define LCD_PATTERN0_WR(src)                       (((u32)(src)) & 0xffffffff)
#define LCD_PATTERN0_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_DBG_PAT_MASK_REG_0	*/ 
/*	 Fields PAT_MASK	 */
#define LCD_PAT_MASK0_WIDTH                                                 32
#define LCD_PAT_MASK0_SHIFT                                                  0
#define LCD_PAT_MASK0_MASK                                          0xffffffff
#define LCD_PAT_MASK0_RD(src)                           (((src) & 0xffffffff))
#define LCD_PAT_MASK0_WR(src)                      (((u32)(src)) & 0xffffffff)
#define LCD_PAT_MASK0_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_DBG_PAT_REG_1	*/ 
/*	 Fields PATTERN	 */
#define LCD_PATTERN1_WIDTH                                                  32
#define LCD_PATTERN1_SHIFT                                                   0
#define LCD_PATTERN1_MASK                                           0xffffffff
#define LCD_PATTERN1_RD(src)                            (((src) & 0xffffffff))
#define LCD_PATTERN1_WR(src)                       (((u32)(src)) & 0xffffffff)
#define LCD_PATTERN1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_DBG_PAT_MASK_REG_1	*/ 
/*	 Fields PAT_MASK	 */
#define LCD_PAT_MASK1_WIDTH                                                 32
#define LCD_PAT_MASK1_SHIFT                                                  0
#define LCD_PAT_MASK1_MASK                                          0xffffffff
#define LCD_PAT_MASK1_RD(src)                           (((src) & 0xffffffff))
#define LCD_PAT_MASK1_WR(src)                      (((u32)(src)) & 0xffffffff)
#define LCD_PAT_MASK1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register DBG_TRIG_OUT	*/ 
/*	 Fields DBG_OUT	 */
#define LCD_DBG_OUT_WIDTH                                                   32
#define LCD_DBG_OUT_SHIFT                                                    0
#define LCD_DBG_OUT_MASK                                            0xffffffff
#define LCD_DBG_OUT_RD(src)                             (((src) & 0xffffffff))
#define LCD_DBG_OUT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register DBG_ECC_INT_OUT	*/ 
/*	 Fields SLV_TMO	 */
#define LCD_SLV_TMO_WIDTH                                                    1
#define LCD_SLV_TMO_SHIFT                                                    3
#define LCD_SLV_TMO_MASK                                            0x00000008
#define LCD_SLV_TMO_RD(src)                          (((src) & 0x00000008)>>3)
#define LCD_SLV_TMO_WR(src)                     (((u32)(src)<<3) & 0x00000008)
#define LCD_SLV_TMO_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields SECC_INT	 */
#define LCD_SECC_INT_WIDTH                                                   1
#define LCD_SECC_INT_SHIFT                                                   2
#define LCD_SECC_INT_MASK                                           0x00000004
#define LCD_SECC_INT_RD(src)                         (((src) & 0x00000004)>>2)
#define LCD_SECC_INT_WR(src)                    (((u32)(src)<<2) & 0x00000004)
#define LCD_SECC_INT_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields DECC_INT	 */
#define LCD_DECC_INT_WIDTH                                                   1
#define LCD_DECC_INT_SHIFT                                                   1
#define LCD_DECC_INT_MASK                                           0x00000002
#define LCD_DECC_INT_RD(src)                         (((src) & 0x00000002)>>1)
#define LCD_DECC_INT_WR(src)                    (((u32)(src)<<1) & 0x00000002)
#define LCD_DECC_INT_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields DBG_INT	 */
#define LCD_DBG_INT_WIDTH                                                    1
#define LCD_DBG_INT_SHIFT                                                    0
#define LCD_DBG_INT_MASK                                            0x00000001
#define LCD_DBG_INT_RD(src)                             (((src) & 0x00000001))
#define LCD_DBG_INT_WR(src)                        (((u32)(src)) & 0x00000001)
#define LCD_DBG_INT_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register DBG_ECC_INT_OUTMask	*/
/*    Mask Register Fields SLV_TMOMask    */
#define LCD_SLV_TMOMASK_WIDTH                                                1
#define LCD_SLV_TMOMASK_SHIFT                                                3
#define LCD_SLV_TMOMASK_MASK                                        0x00000008
#define LCD_SLV_TMOMASK_RD(src)                      (((src) & 0x00000008)>>3)
#define LCD_SLV_TMOMASK_WR(src)                 (((u32)(src)<<3) & 0x00000008)
#define LCD_SLV_TMOMASK_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*    Mask Register Fields SECC_INTMask    */
#define LCD_SECC_INTMASK_WIDTH                                               1
#define LCD_SECC_INTMASK_SHIFT                                               2
#define LCD_SECC_INTMASK_MASK                                       0x00000004
#define LCD_SECC_INTMASK_RD(src)                     (((src) & 0x00000004)>>2)
#define LCD_SECC_INTMASK_WR(src)                (((u32)(src)<<2) & 0x00000004)
#define LCD_SECC_INTMASK_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*    Mask Register Fields DECC_INTMask    */
#define LCD_DECC_INTMASK_WIDTH                                               1
#define LCD_DECC_INTMASK_SHIFT                                               1
#define LCD_DECC_INTMASK_MASK                                       0x00000002
#define LCD_DECC_INTMASK_RD(src)                     (((src) & 0x00000002)>>1)
#define LCD_DECC_INTMASK_WR(src)                (((u32)(src)<<1) & 0x00000002)
#define LCD_DECC_INTMASK_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*    Mask Register Fields DBG_INTMask    */
#define LCD_DBG_INTMASK_WIDTH                                                1
#define LCD_DBG_INTMASK_SHIFT                                                0
#define LCD_DBG_INTMASK_MASK                                        0x00000001
#define LCD_DBG_INTMASK_RD(src)                         (((src) & 0x00000001))
#define LCD_DBG_INTMASK_WR(src)                    (((u32)(src)) & 0x00000001)
#define LCD_DBG_INTMASK_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register CFG_SECC_INT_THRSH_REG	*/ 
/*	 Fields SECC_INT_THRSH	 */
#define LCD_SECC_INT_THRSH_WIDTH                                             8
#define LCD_SECC_INT_THRSH_SHIFT                                             0
#define LCD_SECC_INT_THRSH_MASK                                     0x000000ff
#define LCD_SECC_INT_THRSH_RD(src)                      (((src) & 0x000000ff))
#define LCD_SECC_INT_THRSH_WR(src)                 (((u32)(src)) & 0x000000ff)
#define LCD_SECC_INT_THRSH_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register CFG_MEM_ECC_BYPASS	*/ 
/*	 Fields CFG_ECC_BYPASS	 */
#define LCD_CFG_ECC_BYPASS_WIDTH                                            16
#define LCD_CFG_ECC_BYPASS_SHIFT                                             0
#define LCD_CFG_ECC_BYPASS_MASK                                     0x0000ffff
#define LCD_CFG_ECC_BYPASS_RD(src)                      (((src) & 0x0000ffff))
#define LCD_CFG_ECC_BYPASS_WR(src)                 (((u32)(src)) & 0x0000ffff)
#define LCD_CFG_ECC_BYPASS_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register CFG_MEM_PWRDN_DIS	*/ 
/*	 Fields CFG_ECC_PWRDN_DIS	 */
#define LCD_CFG_ECC_PWRDN_DIS_WIDTH                                         16
#define LCD_CFG_ECC_PWRDN_DIS_SHIFT                                         16
#define LCD_CFG_ECC_PWRDN_DIS_MASK                                  0xffff0000
#define LCD_CFG_ECC_PWRDN_DIS_RD(src)               (((src) & 0xffff0000)>>16)
#define LCD_CFG_ECC_PWRDN_DIS_WR(src)          (((u32)(src)<<16) & 0xffff0000)
#define LCD_CFG_ECC_PWRDN_DIS_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields CFG_PWRDN_DIS	 */
#define LCD_CFG_PWRDN_DIS_WIDTH                                             16
#define LCD_CFG_PWRDN_DIS_SHIFT                                              0
#define LCD_CFG_PWRDN_DIS_MASK                                      0x0000ffff
#define LCD_CFG_PWRDN_DIS_RD(src)                       (((src) & 0x0000ffff))
#define LCD_CFG_PWRDN_DIS_WR(src)                  (((u32)(src)) & 0x0000ffff)
#define LCD_CFG_PWRDN_DIS_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register CFG_MEM_RAM_SHUTDOWN	*/ 
/*	 Fields CFG_RAM_SHUTDOWN_EN	 */
#define LCD_CFG_RAM_SHUTDOWN_EN_WIDTH                                       32
#define LCD_CFG_RAM_SHUTDOWN_EN_SHIFT                                        0
#define LCD_CFG_RAM_SHUTDOWN_EN_MASK                                0xffffffff
#define LCD_CFG_RAM_SHUTDOWN_EN_RD(src)                 (((src) & 0xffffffff))
#define LCD_CFG_RAM_SHUTDOWN_EN_WR(src)            (((u32)(src)) & 0xffffffff)
#define LCD_CFG_RAM_SHUTDOWN_EN_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register BLOCK_MEM_RDY	*/ 
/*	 Fields MEM_RDY	 */
#define LCD_MEM_RDY_WIDTH                                                   32
#define LCD_MEM_RDY_SHIFT                                                    0
#define LCD_MEM_RDY_MASK                                            0xffffffff
#define LCD_MEM_RDY_RD(src)                             (((src) & 0xffffffff))
#define LCD_MEM_RDY_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_AXI_SLV_RD_TMO	*/ 
/*	 Fields SLV_ID_TMO_ENABLE	 */
#define LCD_SLV_ID_TMO_ENABLE_WIDTH                                          1
#define LCD_SLV_ID_TMO_ENABLE_SHIFT                                         31
#define LCD_SLV_ID_TMO_ENABLE_MASK                                  0x80000000
#define LCD_SLV_ID_TMO_ENABLE_RD(src)               (((src) & 0x80000000)>>31)
#define LCD_SLV_ID_TMO_ENABLE_WR(src)          (((u32)(src)<<31) & 0x80000000)
#define LCD_SLV_ID_TMO_ENABLE_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields SS_ID_TRK_ENABLE	 */
#define LCD_SS_ID_TRK_ENABLE_WIDTH                                           1
#define LCD_SS_ID_TRK_ENABLE_SHIFT                                          30
#define LCD_SS_ID_TRK_ENABLE_MASK                                   0x40000000
#define LCD_SS_ID_TRK_ENABLE_RD(src)                (((src) & 0x40000000)>>30)
#define LCD_SS_ID_TRK_ENABLE_WR(src)           (((u32)(src)<<30) & 0x40000000)
#define LCD_SS_ID_TRK_ENABLE_SET(dst,src) \
                      (((dst) & ~0x40000000) | (((u32)(src)<<30) & 0x40000000))
/*	 Fields SLV_ID_TMO_VALUE	 */
#define LCD_SLV_ID_TMO_VALUE_WIDTH                                          10
#define LCD_SLV_ID_TMO_VALUE_SHIFT                                          16
#define LCD_SLV_ID_TMO_VALUE_MASK                                   0x03ff0000
#define LCD_SLV_ID_TMO_VALUE_RD(src)                (((src) & 0x03ff0000)>>16)
#define LCD_SLV_ID_TMO_VALUE_WR(src)           (((u32)(src)<<16) & 0x03ff0000)
#define LCD_SLV_ID_TMO_VALUE_SET(dst,src) \
                      (((dst) & ~0x03ff0000) | (((u32)(src)<<16) & 0x03ff0000))
/*	 Fields SLV_RDY_TMO_ENABLE	 */
#define LCD_SLV_RDY_TMO_ENABLE_WIDTH                                         1
#define LCD_SLV_RDY_TMO_ENABLE_SHIFT                                        15
#define LCD_SLV_RDY_TMO_ENABLE_MASK                                 0x00008000
#define LCD_SLV_RDY_TMO_ENABLE_RD(src)              (((src) & 0x00008000)>>15)
#define LCD_SLV_RDY_TMO_ENABLE_WR(src)         (((u32)(src)<<15) & 0x00008000)
#define LCD_SLV_RDY_TMO_ENABLE_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))
/*	 Fields SLV_RDY_TMO_VALUE	 */
#define LCD_SLV_RDY_TMO_VALUE_WIDTH                                         10
#define LCD_SLV_RDY_TMO_VALUE_SHIFT                                          0
#define LCD_SLV_RDY_TMO_VALUE_MASK                                  0x000003ff
#define LCD_SLV_RDY_TMO_VALUE_RD(src)                   (((src) & 0x000003ff))
#define LCD_SLV_RDY_TMO_VALUE_WR(src)              (((u32)(src)) & 0x000003ff)
#define LCD_SLV_RDY_TMO_VALUE_SET(dst,src) \
                          (((dst) & ~0x000003ff) | (((u32)(src)) & 0x000003ff))

/*	Register CFG_AXI_SLV_WR_TMO	*/ 
/*	 Fields SLV_ID_TMO_ENABLE	 */
#define LCD_SLV_ID_TMO_ENABLE_F1_WIDTH                                       1
#define LCD_SLV_ID_TMO_ENABLE_F1_SHIFT                                      31
#define LCD_SLV_ID_TMO_ENABLE_F1_MASK                               0x80000000
#define LCD_SLV_ID_TMO_ENABLE_F1_RD(src)            (((src) & 0x80000000)>>31)
#define LCD_SLV_ID_TMO_ENABLE_F1_WR(src)       (((u32)(src)<<31) & 0x80000000)
#define LCD_SLV_ID_TMO_ENABLE_F1_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields SS_ID_TRK_ENABLE	 */
#define LCD_SS_ID_TRK_ENABLE_F1_WIDTH                                        1
#define LCD_SS_ID_TRK_ENABLE_F1_SHIFT                                       30
#define LCD_SS_ID_TRK_ENABLE_F1_MASK                                0x40000000
#define LCD_SS_ID_TRK_ENABLE_F1_RD(src)             (((src) & 0x40000000)>>30)
#define LCD_SS_ID_TRK_ENABLE_F1_WR(src)        (((u32)(src)<<30) & 0x40000000)
#define LCD_SS_ID_TRK_ENABLE_F1_SET(dst,src) \
                      (((dst) & ~0x40000000) | (((u32)(src)<<30) & 0x40000000))
/*	 Fields SLV_ID_TMO_VALUE	 */
#define LCD_SLV_ID_TMO_VALUE_F1_WIDTH                                       10
#define LCD_SLV_ID_TMO_VALUE_F1_SHIFT                                       16
#define LCD_SLV_ID_TMO_VALUE_F1_MASK                                0x03ff0000
#define LCD_SLV_ID_TMO_VALUE_F1_RD(src)             (((src) & 0x03ff0000)>>16)
#define LCD_SLV_ID_TMO_VALUE_F1_WR(src)        (((u32)(src)<<16) & 0x03ff0000)
#define LCD_SLV_ID_TMO_VALUE_F1_SET(dst,src) \
                      (((dst) & ~0x03ff0000) | (((u32)(src)<<16) & 0x03ff0000))
/*	 Fields SLV_RDY_TMO_ENABLE	 */
#define LCD_SLV_RDY_TMO_ENABLE_F1_WIDTH                                      1
#define LCD_SLV_RDY_TMO_ENABLE_F1_SHIFT                                     15
#define LCD_SLV_RDY_TMO_ENABLE_F1_MASK                              0x00008000
#define LCD_SLV_RDY_TMO_ENABLE_F1_RD(src)           (((src) & 0x00008000)>>15)
#define LCD_SLV_RDY_TMO_ENABLE_F1_WR(src)      (((u32)(src)<<15) & 0x00008000)
#define LCD_SLV_RDY_TMO_ENABLE_F1_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))
/*	 Fields SLV_RDY_TMO_VALUE	 */
#define LCD_SLV_RDY_TMO_VALUE_F1_WIDTH                                      10
#define LCD_SLV_RDY_TMO_VALUE_F1_SHIFT                                       0
#define LCD_SLV_RDY_TMO_VALUE_F1_MASK                               0x000003ff
#define LCD_SLV_RDY_TMO_VALUE_F1_RD(src)                (((src) & 0x000003ff))
#define LCD_SLV_RDY_TMO_VALUE_F1_WR(src)           (((u32)(src)) & 0x000003ff)
#define LCD_SLV_RDY_TMO_VALUE_F1_SET(dst,src) \
                          (((dst) & ~0x000003ff) | (((u32)(src)) & 0x000003ff))

/*	Register STS_AXI_SLV_TMO	*/ 
/*	 Fields SLV_ARID_TMO	 */
#define LCD_SLV_ARID_TMO_WIDTH                                               1
#define LCD_SLV_ARID_TMO_SHIFT                                               3
#define LCD_SLV_ARID_TMO_MASK                                       0x00000008
#define LCD_SLV_ARID_TMO_RD(src)                     (((src) & 0x00000008)>>3)
#define LCD_SLV_ARID_TMO_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields SLV_ARRDY_TMO	 */
#define LCD_SLV_ARRDY_TMO_WIDTH                                              1
#define LCD_SLV_ARRDY_TMO_SHIFT                                              2
#define LCD_SLV_ARRDY_TMO_MASK                                      0x00000004
#define LCD_SLV_ARRDY_TMO_RD(src)                    (((src) & 0x00000004)>>2)
#define LCD_SLV_ARRDY_TMO_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields SLV_AWID_TMO	 */
#define LCD_SLV_AWID_TMO_WIDTH                                               1
#define LCD_SLV_AWID_TMO_SHIFT                                               1
#define LCD_SLV_AWID_TMO_MASK                                       0x00000002
#define LCD_SLV_AWID_TMO_RD(src)                     (((src) & 0x00000002)>>1)
#define LCD_SLV_AWID_TMO_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields SLV_AWRDY_TMO	 */
#define LCD_SLV_AWRDY_TMO_WIDTH                                              1
#define LCD_SLV_AWRDY_TMO_SHIFT                                              0
#define LCD_SLV_AWRDY_TMO_MASK                                      0x00000001
#define LCD_SLV_AWRDY_TMO_RD(src)                       (((src) & 0x00000001))
#define LCD_SLV_AWRDY_TMO_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register CFG_AXI_SS_CSR_TMO	*/ 
/*	 Fields CFG_CSR_TMO	 */
#define LCD_CFG_CSR_TMO_WIDTH                                               16
#define LCD_CFG_CSR_TMO_SHIFT                                               16
#define LCD_CFG_CSR_TMO_MASK                                        0xffff0000
#define LCD_CFG_CSR_TMO_RD(src)                     (((src) & 0xffff0000)>>16)
#define LCD_CFG_CSR_TMO_WR(src)                (((u32)(src)<<16) & 0xffff0000)
#define LCD_CFG_CSR_TMO_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields CSR_ERR_RESP_EN	 */
#define LCD_CSR_ERR_RESP_EN_WIDTH                                            1
#define LCD_CSR_ERR_RESP_EN_SHIFT                                           15
#define LCD_CSR_ERR_RESP_EN_MASK                                    0x00008000
#define LCD_CSR_ERR_RESP_EN_RD(src)                 (((src) & 0x00008000)>>15)
#define LCD_CSR_ERR_RESP_EN_WR(src)            (((u32)(src)<<15) & 0x00008000)
#define LCD_CSR_ERR_RESP_EN_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))

/*	Register STS_READ_LATENCY_TOT_READ_REQS	*/ 
/*	 Fields TOT_READS	 */
#define LCD_TOT_READS_WIDTH                                                 16
#define LCD_TOT_READS_SHIFT                                                  0
#define LCD_TOT_READS_MASK                                          0x0000ffff
#define LCD_TOT_READS_RD(src)                           (((src) & 0x0000ffff))
#define LCD_TOT_READS_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))
	


#endif
