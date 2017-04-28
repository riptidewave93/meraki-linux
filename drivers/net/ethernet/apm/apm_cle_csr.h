/*
 * APM 86xxx Classifier Header File
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
#ifndef _MB_CLE_CSR_H__
#define _MB_CLE_CSR_H__

/*	Global Base Address	*/
#define CLE_CSR0_BASE_ADDR			0xddd8e0000ULL

/*    Address CLE_CSR0  Registers */
#define CLE_BID_ADDR                                                 0x00000000
#define CLE_BID_DEFAULT                                              0x00000062
#define INDADDR_ADDR                                                 0x00000004
#define INDADDR_DEFAULT                                              0x00000000
#define INDCMD_ADDR                                                  0x00000008
#define INDCMD_DEFAULT                                               0x00000000
#define INDCMD_STATUS_ADDR                                           0x0000000c
#define INDCMD_STATUS_DEFAULT                                        0x00000000
#define DATA_RAM0_ADDR                                               0x00000010
#define DATA_RAM0_DEFAULT                                            0x00000000
#define DATA_RAM1_ADDR                                               0x00000014
#define DATA_RAM1_DEFAULT                                            0x00000000
#define DATA_RAM2_ADDR                                               0x00000018
#define DATA_RAM2_DEFAULT                                            0x00000000
#define DATA_RAM3_ADDR                                               0x0000001c
#define DATA_RAM3_DEFAULT                                            0x00000000
#define DATA_RAM4_ADDR                                               0x00000020
#define DATA_RAM4_DEFAULT                                            0x00000000
#define DATA_RAM5_ADDR                                               0x00000024
#define DATA_RAM5_DEFAULT                                            0x00000000
#define DATA_RAM6_ADDR                                               0x00000028
#define DATA_RAM6_DEFAULT                                            0x00000000
#define DATA_RAM7_ADDR                                               0x0000002c
#define DATA_RAM7_DEFAULT                                            0x00000000
#define DATA_RAM8_ADDR                                               0x00000030
#define DATA_RAM8_DEFAULT                                            0x00000000
#define DATA_RAM9_ADDR                                               0x00000034
#define DATA_RAM9_DEFAULT                                            0x00000000
#define DATA_RAM10_ADDR                                              0x00000038
#define DATA_RAM10_DEFAULT                                           0x00000000
#define DATA_RAM11_ADDR                                              0x0000003c
#define DATA_RAM11_DEFAULT                                           0x00000000
#define DATA_RAM12_ADDR                                              0x00000040
#define DATA_RAM12_DEFAULT                                           0x00000000
#define DATA_RAM13_ADDR                                              0x00000044
#define DATA_RAM13_DEFAULT                                           0x00000000
#define DATA_RAM14_ADDR                                              0x00000048
#define DATA_RAM14_DEFAULT                                           0x00000000
#define DATA_RAM15_ADDR                                              0x0000004c
#define DATA_RAM15_DEFAULT                                           0x00000000
#define AVL_CONFIG_ADDR                                              0x00000050
#define AVL_CONFIG_DEFAULT                                           0x03ffd7fc
#define AVL_STATUS_ADDR                                              0x00000054
#define AVL_STATUS_DEFAULT                                           0x00000000
#define SNPTR0_ADDR                                                  0x00000100
#define SNPTR0_DEFAULT                                               0x00000000
#define SPPTR0_ADDR                                                  0x00000104
#define SPPTR0_DEFAULT                                               0x00000000
#define DFCLSRESDBPTR0_ADDR                                          0x00000108
#define DFCLSRESDBPTR0_DEFAULT                                       0x00001c00
#define DFCLSRESDB0_0_ADDR                                           0x0000010c
#define DFCLSRESDB0_0_DEFAULT                                        0x00000000
#define DFCLSRESDB0_1_ADDR                                           0x00000110
#define DFCLSRESDB0_1_DEFAULT                                        0x00000000
#define DFCLSRESDB0_2_ADDR                                           0x00000114
#define DFCLSRESDB0_2_DEFAULT                                        0x00000000
#define DFCLSRESDB0_3_ADDR                                           0x00000118
#define DFCLSRESDB0_3_DEFAULT                                        0x00000000
#define DFCLSRESDB0_4_ADDR                                           0x0000011c
#define DFCLSRESDB0_4_DEFAULT                                        0x00000000
#define DFCLSRESDB0_5_ADDR                                           0x00000120
#define DFCLSRESDB0_5_DEFAULT                                        0x00000000
#define PARSER_CTL0_ADDR                                             0x00000124
#define PARSER_CTL0_DEFAULT                                          0x00000000
#define TMAXHOP0_ADDR                                                0x00000128
#define TMAXHOP0_DEFAULT                                             0x00000040
#define PORTNUM0_ADDR                                                0x0000012c
#define PORTNUM0_DEFAULT                                             0x00000000
#define DS_CTL_ADDR                                                  0x00000130
#define DS_CTL_DEFAULT                                               0x00040a0b
#define SNPTR1_ADDR                                                  0x00000140
#define SNPTR1_DEFAULT                                               0x00000000
#define SPPTR1_ADDR                                                  0x00000144
#define SPPTR1_DEFAULT                                               0x00000000
#define DFCLSRESDBPTR1_ADDR                                          0x00000148
#define DFCLSRESDBPTR1_DEFAULT                                       0x00001c00
#define DFCLSRESDB1_0_ADDR                                           0x0000014c
#define DFCLSRESDB1_0_DEFAULT                                        0x00000000
#define DFCLSRESDB1_1_ADDR                                           0x00000150
#define DFCLSRESDB1_1_DEFAULT                                        0x00000000
#define DFCLSRESDB1_2_ADDR                                           0x00000154
#define DFCLSRESDB1_2_DEFAULT                                        0x00000000
#define DFCLSRESDB1_3_ADDR                                           0x00000158
#define DFCLSRESDB1_3_DEFAULT                                        0x00000000
#define DFCLSRESDB1_4_ADDR                                           0x0000015c
#define DFCLSRESDB1_4_DEFAULT                                        0x00000000
#define DFCLSRESDB1_5_ADDR                                           0x00000160
#define DFCLSRESDB1_5_DEFAULT                                        0x00000000
#define PARSER_CTL1_ADDR                                             0x00000164
#define PARSER_CTL1_DEFAULT                                          0x00000000
#define TMAXHOP1_ADDR                                                0x00000168
#define TMAXHOP1_DEFAULT                                             0x00000040
#define PORTNUM1_ADDR                                                0x0000016c
#define PORTNUM1_DEFAULT                                             0x00000001
#define PTRAM_ERR_DIAGNOSTIC_ADDR                                    0x00000200
#define PTRAM_ERR_DIAGNOSTIC_DEFAULT                                 0x00000000
#define PKTRAM0_ERR_DIAGNOSTIC_ADDR                                  0x00000204
#define PKTRAM0_ERR_DIAGNOSTIC_DEFAULT                               0x00000000
#define PKTRAM1_ERR_DIAGNOSTIC_ADDR                                  0x00000208
#define PKTRAM1_ERR_DIAGNOSTIC_DEFAULT                               0x00000000
#define AVLRAM_ERR_DIAGNOSTIC_ADDR                                   0x0000020c
#define AVLRAM_ERR_DIAGNOSTIC_DEFAULT                                0x00000000
#define DBRAM_ERR_DIAGNOSTIC_ADDR                                    0x00000210
#define DBRAM_ERR_DIAGNOSTIC_DEFAULT                                 0x00000000
#define SNPTR_CHANGE_STATUS0_ADDR                                    0x00000214
#define SNPTR_CHANGE_STATUS0_DEFAULT                                 0x00000000
#define SNPTR_CHANGE_STATUS1_ADDR                                    0x00000218
#define SNPTR_CHANGE_STATUS1_DEFAULT                                 0x00000000
#define PARSER_STATUS0_ADDR                                          0x0000021c
#define PARSER_STATUS0_DEFAULT                                       0x00000000
#define PARSER_STATUS0MASK_ADDR                                      0x00000220
#define PARSER_STATUS1_ADDR                                          0x00000224
#define PARSER_STATUS1_DEFAULT                                       0x00000000
#define PARSER_STATUS1MASK_ADDR                                      0x00000228
#define RSIF_RAM_CTRL_ADDR                                           0x00000230
#define RSIF_RAM_CTRL_DEFAULT                                        0x00000000
#define DB_RAM_CTRL_ADDR                                             0x00000234
#define DB_RAM_CTRL_DEFAULT                                          0x00000000
#define AVL_RAM3_CTRL_ADDR                                           0x00000238
#define AVL_RAM3_CTRL_DEFAULT                                        0x00000000
#define AVL_RAM2_CTRL_ADDR                                           0x0000023c
#define AVL_RAM2_CTRL_DEFAULT                                        0x00000000
#define AVL_RAM1_CTRL_ADDR                                           0x00000240
#define AVL_RAM1_CTRL_DEFAULT                                        0x00000000
#define AVL_RAM0_CTRL_ADDR                                           0x00000244
#define AVL_RAM0_CTRL_DEFAULT                                        0x00000000
#define PT_RAM3_CTRL_ADDR                                            0x00000248
#define PT_RAM3_CTRL_DEFAULT                                         0x00000000
#define PT_RAM2_CTRL_ADDR                                            0x0000024c
#define PT_RAM2_CTRL_DEFAULT                                         0x00000000
#define PT_RAM1_CTRL_ADDR                                            0x00000250
#define PT_RAM1_CTRL_DEFAULT                                         0x00000000
#define PT_RAM0_CTRL_ADDR                                            0x00000254
#define PT_RAM0_CTRL_DEFAULT                                         0x00000000
#define AVL_SEARCH_INT_ADDR                                          0x00000258
#define AVL_SEARCH_INT_DEFAULT                                       0x00000000
#define AVL_SEARCH_INTMASK_ADDR                                      0x0000025c
#define CLE_INTERRUPT_ADDR                                           0x00000260
#define CLE_INTERRUPT_DEFAULT                                        0x00000000
#define CLE_INTERRUPTMASK_ADDR                                       0x00000264
#define LSTNVST0_ADDR                                                0x00000300
#define LSTNVST0_DEFAULT                                             0x00000000
#define LSTTRCNVST0_0_ADDR                                           0x00000304
#define LSTTRCNVST0_0_DEFAULT                                        0x00000000
#define LSTTRCNVST0_1_ADDR                                           0x00000308
#define LSTTRCNVST0_1_DEFAULT                                        0x00000000
#define LSTTRCNVST0_2_ADDR                                           0x0000030c
#define LSTTRCNVST0_2_DEFAULT                                        0x00000000
#define LSTTRCNVST0_3_ADDR                                           0x00000310
#define LSTTRCNVST0_3_DEFAULT                                        0x00000000
#define LSTTRCNVST0_4_ADDR                                           0x00000314
#define LSTTRCNVST0_4_DEFAULT                                        0x00000000
#define LSTTRCNVST0_5_ADDR                                           0x00000318
#define LSTTRCNVST0_5_DEFAULT                                        0x00000000
#define LSTTRCNVST0_6_ADDR                                           0x0000031c
#define LSTTRCNVST0_6_DEFAULT                                        0x00000000
#define LSTTRCNVST0_7_ADDR                                           0x00000320
#define LSTTRCNVST0_7_DEFAULT                                        0x00000000
#define LSTTRCNVST0_8_ADDR                                           0x00000324
#define LSTTRCNVST0_8_DEFAULT                                        0x00000000
#define LSTTRCNVST0_9_ADDR                                           0x00000328
#define LSTTRCNVST0_9_DEFAULT                                        0x00000000
#define LSTTRCNVST0_10_ADDR                                          0x0000032c
#define LSTTRCNVST0_10_DEFAULT                                       0x00000000
#define LSTTRCNVST0_11_ADDR                                          0x00000330
#define LSTTRCNVST0_11_DEFAULT                                       0x00000000
#define LSTTRCNVST0_12_ADDR                                          0x00000334
#define LSTTRCNVST0_12_DEFAULT                                       0x00000000
#define LSTTRCNVST0_13_ADDR                                          0x00000338
#define LSTTRCNVST0_13_DEFAULT                                       0x00000000
#define LSTTRCNVST0_14_ADDR                                          0x0000033c
#define LSTTRCNVST0_14_DEFAULT                                       0x00000000
#define LSTTRCNVST0_15_ADDR                                          0x00000340
#define LSTTRCNVST0_15_DEFAULT                                       0x00000000
#define FTRCNVSTSTR0_0_ADDR                                          0x00000350
#define FTRCNVSTSTR0_0_DEFAULT                                       0x00000008
#define FTRCNVST0_0_ADDR                                             0x00000354
#define FTRCNVST0_0_DEFAULT                                          0x00000000
#define FTRCNVST0_1_ADDR                                             0x00000358
#define FTRCNVST0_1_DEFAULT                                          0x00000000
#define FTRCNVST0_2_ADDR                                             0x0000035c
#define FTRCNVST0_2_DEFAULT                                          0x00000000
#define FTRCNVST0_3_ADDR                                             0x00000360
#define FTRCNVST0_3_DEFAULT                                          0x00000000
#define FTRCNVST0_4_ADDR                                             0x00000364
#define FTRCNVST0_4_DEFAULT                                          0x00000000
#define FTRCNVST0_5_ADDR                                             0x00000368
#define FTRCNVST0_5_DEFAULT                                          0x00000000
#define FTRCNVST0_6_ADDR                                             0x0000036c
#define FTRCNVST0_6_DEFAULT                                          0x00000000
#define FTRCNVST0_7_ADDR                                             0x00000370
#define FTRCNVST0_7_DEFAULT                                          0x00000000
#define FTRCNVST0_8_ADDR                                             0x00000374
#define FTRCNVST0_8_DEFAULT                                          0x00000000
#define FTRCNVST0_9_ADDR                                             0x00000378
#define FTRCNVST0_9_DEFAULT                                          0x00000000
#define FTRCNVST0_10_ADDR                                            0x0000037c
#define FTRCNVST0_10_DEFAULT                                         0x00000000
#define FTRCNVST0_11_ADDR                                            0x00000380
#define FTRCNVST0_11_DEFAULT                                         0x00000000
#define FTRCNVST0_12_ADDR                                            0x00000384
#define FTRCNVST0_12_DEFAULT                                         0x00000000
#define FTRCNVST0_13_ADDR                                            0x00000388
#define FTRCNVST0_13_DEFAULT                                         0x00000000
#define FTRCNVST0_14_ADDR                                            0x0000038c
#define FTRCNVST0_14_DEFAULT                                         0x00000000
#define FTRCNVST0_15_ADDR                                            0x00000390
#define FTRCNVST0_15_DEFAULT                                         0x00000000
#define TRCNVSTMON0_0_ADDR                                           0x00000394
#define TRCNVSTMON0_0_DEFAULT                                        0x00000000
#define TRCNVSTMONCNT0_0_ADDR                                        0x00000398
#define TRCNVSTMONCNT0_0_DEFAULT                                     0x00000000
#define TRCNVSTMON0_1_ADDR                                           0x0000039c
#define TRCNVSTMON0_1_DEFAULT                                        0x00000000
#define TRCNVSTMONCNT0_1_ADDR                                        0x000003a0
#define TRCNVSTMONCNT0_1_DEFAULT                                     0x00000000
#define TRCNVSTMON0_2_ADDR                                           0x000003a4
#define TRCNVSTMON0_2_DEFAULT                                        0x00000000
#define TRCNVSTMONCNT0_2_ADDR                                        0x000003a8
#define TRCNVSTMONCNT0_2_DEFAULT                                     0x00000000
#define TRCNVSTMON0_3_ADDR                                           0x000003ac
#define TRCNVSTMON0_3_DEFAULT                                        0x00000000
#define TRCNVSTMONCNT0_3_ADDR                                        0x000003b0
#define TRCNVSTMONCNT0_3_DEFAULT                                     0x00000000
#define TRCNVSTMON0_4_ADDR                                           0x000003b4
#define TRCNVSTMON0_4_DEFAULT                                        0x00000000
#define TRCNVSTMONCNT0_4_ADDR                                        0x000003b8
#define TRCNVSTMONCNT0_4_DEFAULT                                     0x00000000
#define TRCNVSTMON0_5_ADDR                                           0x000003bc
#define TRCNVSTMON0_5_DEFAULT                                        0x00000000
#define TRCNVSTMONCNT0_5_ADDR                                        0x000003c0
#define TRCNVSTMONCNT0_5_DEFAULT                                     0x00000000
#define TRCNVSTMON0_6_ADDR                                           0x000003c4
#define TRCNVSTMON0_6_DEFAULT                                        0x00000000
#define TRCNVSTMONCNT0_6_ADDR                                        0x000003c8
#define TRCNVSTMONCNT0_6_DEFAULT                                     0x00000000
#define TRCNVSTMON0_7_ADDR                                           0x000003cc
#define TRCNVSTMON0_7_DEFAULT                                        0x00000000
#define TRCNVSTMONCNT0_7_ADDR                                        0x000003d0
#define TRCNVSTMONCNT0_7_DEFAULT                                     0x00000000
#define TRCNVSTMON0_8_ADDR                                           0x000003d4
#define TRCNVSTMON0_8_DEFAULT                                        0x00000000
#define TRCNVSTMONCNT0_8_ADDR                                        0x000003d8
#define TRCNVSTMONCNT0_8_DEFAULT                                     0x00000000
#define TRCNVSTMON0_9_ADDR                                           0x000003dc
#define TRCNVSTMON0_9_DEFAULT                                        0x00000000
#define TRCNVSTMONCNT0_9_ADDR                                        0x000003e0
#define TRCNVSTMONCNT0_9_DEFAULT                                     0x00000000
#define TRCNVSTMON0_10_ADDR                                          0x000003e4
#define TRCNVSTMON0_10_DEFAULT                                       0x00000000
#define TRCNVSTMONCNT0_10_ADDR                                       0x000003e8
#define TRCNVSTMONCNT0_10_DEFAULT                                    0x00000000
#define TRCNVSTMON0_11_ADDR                                          0x000003ec
#define TRCNVSTMON0_11_DEFAULT                                       0x00000000
#define TRCNVSTMONCNT0_11_ADDR                                       0x000003f0
#define TRCNVSTMONCNT0_11_DEFAULT                                    0x00000000
#define TRCNVSTMON0_12_ADDR                                          0x000003f4
#define TRCNVSTMON0_12_DEFAULT                                       0x00000000
#define TRCNVSTMONCNT0_12_ADDR                                       0x000003f8
#define TRCNVSTMONCNT0_12_DEFAULT                                    0x00000000
#define TRCNVSTMON0_13_ADDR                                          0x000003fc
#define TRCNVSTMON0_13_DEFAULT                                       0x00000000
#define TRCNVSTMONCNT0_13_ADDR                                       0x00000400
#define TRCNVSTMONCNT0_13_DEFAULT                                    0x00000000
#define TRCNVSTMON0_14_ADDR                                          0x00000404
#define TRCNVSTMON0_14_DEFAULT                                       0x00000000
#define TRCNVSTMONCNT0_14_ADDR                                       0x00000408
#define TRCNVSTMONCNT0_14_DEFAULT                                    0x00000000
#define TRCNVSTMON0_15_ADDR                                          0x0000040c
#define TRCNVSTMON0_15_DEFAULT                                       0x00000000
#define TRCNVSTMONCNT0_15_ADDR                                       0x00000410
#define TRCNVSTMONCNT0_15_DEFAULT                                    0x00000000
#define LSTNVST1_ADDR                                                0x00000420
#define LSTNVST1_DEFAULT                                             0x00000000
#define LSTTRCNVST1_0_ADDR                                           0x00000424
#define LSTTRCNVST1_0_DEFAULT                                        0x00000000
#define LSTTRCNVST1_1_ADDR                                           0x00000428
#define LSTTRCNVST1_1_DEFAULT                                        0x00000000
#define LSTTRCNVST1_2_ADDR                                           0x0000042c
#define LSTTRCNVST1_2_DEFAULT                                        0x00000000
#define LSTTRCNVST1_3_ADDR                                           0x00000430
#define LSTTRCNVST1_3_DEFAULT                                        0x00000000
#define LSTTRCNVST1_4_ADDR                                           0x00000434
#define LSTTRCNVST1_4_DEFAULT                                        0x00000000
#define LSTTRCNVST1_5_ADDR                                           0x00000438
#define LSTTRCNVST1_5_DEFAULT                                        0x00000000
#define LSTTRCNVST1_6_ADDR                                           0x0000043c
#define LSTTRCNVST1_6_DEFAULT                                        0x00000000
#define LSTTRCNVST1_7_ADDR                                           0x00000440
#define LSTTRCNVST1_7_DEFAULT                                        0x00000000
#define LSTTRCNVST1_8_ADDR                                           0x00000444
#define LSTTRCNVST1_8_DEFAULT                                        0x00000000
#define LSTTRCNVST1_9_ADDR                                           0x00000448
#define LSTTRCNVST1_9_DEFAULT                                        0x00000000
#define LSTTRCNVST1_10_ADDR                                          0x0000044c
#define LSTTRCNVST1_10_DEFAULT                                       0x00000000
#define LSTTRCNVST1_11_ADDR                                          0x00000450
#define LSTTRCNVST1_11_DEFAULT                                       0x00000000
#define LSTTRCNVST1_12_ADDR                                          0x00000454
#define LSTTRCNVST1_12_DEFAULT                                       0x00000000
#define LSTTRCNVST1_13_ADDR                                          0x00000458
#define LSTTRCNVST1_13_DEFAULT                                       0x00000000
#define LSTTRCNVST1_14_ADDR                                          0x0000045c
#define LSTTRCNVST1_14_DEFAULT                                       0x00000000
#define LSTTRCNVST1_15_ADDR                                          0x00000460
#define LSTTRCNVST1_15_DEFAULT                                       0x00000000
#define FTRCNVSTSTR1_0_ADDR                                          0x00000470
#define FTRCNVSTSTR1_0_DEFAULT                                       0x00000008
#define FTRCNVST1_0_ADDR                                             0x00000474
#define FTRCNVST1_0_DEFAULT                                          0x00000000
#define FTRCNVST1_1_ADDR                                             0x00000478
#define FTRCNVST1_1_DEFAULT                                          0x00000000
#define FTRCNVST1_2_ADDR                                             0x0000047c
#define FTRCNVST1_2_DEFAULT                                          0x00000000
#define FTRCNVST1_3_ADDR                                             0x00000480
#define FTRCNVST1_3_DEFAULT                                          0x00000000
#define FTRCNVST1_4_ADDR                                             0x00000484
#define FTRCNVST1_4_DEFAULT                                          0x00000000
#define FTRCNVST1_5_ADDR                                             0x00000488
#define FTRCNVST1_5_DEFAULT                                          0x00000000
#define FTRCNVST1_6_ADDR                                             0x0000048c
#define FTRCNVST1_6_DEFAULT                                          0x00000000
#define FTRCNVST1_7_ADDR                                             0x00000490
#define FTRCNVST1_7_DEFAULT                                          0x00000000
#define FTRCNVST1_8_ADDR                                             0x00000494
#define FTRCNVST1_8_DEFAULT                                          0x00000000
#define FTRCNVST1_9_ADDR                                             0x00000498
#define FTRCNVST1_9_DEFAULT                                          0x00000000
#define FTRCNVST1_10_ADDR                                            0x0000049c
#define FTRCNVST1_10_DEFAULT                                         0x00000000
#define FTRCNVST1_11_ADDR                                            0x000004a0
#define FTRCNVST1_11_DEFAULT                                         0x00000000
#define FTRCNVST1_12_ADDR                                            0x000004a4
#define FTRCNVST1_12_DEFAULT                                         0x00000000
#define FTRCNVST1_13_ADDR                                            0x000004a8
#define FTRCNVST1_13_DEFAULT                                         0x00000000
#define FTRCNVST1_14_ADDR                                            0x000004ac
#define FTRCNVST1_14_DEFAULT                                         0x00000000
#define FTRCNVST1_15_ADDR                                            0x000004b0
#define FTRCNVST1_15_DEFAULT                                         0x00000000
#define TRCNVSTMON1_0_ADDR                                           0x000004b4
#define TRCNVSTMON1_0_DEFAULT                                        0x00000000
#define TRCNVSTMONCNT1_0_ADDR                                        0x000004b8
#define TRCNVSTMONCNT1_0_DEFAULT                                     0x00000000
#define TRCNVSTMON1_1_ADDR                                           0x000004bc
#define TRCNVSTMON1_1_DEFAULT                                        0x00000000
#define TRCNVSTMONCNT1_1_ADDR                                        0x000004c0
#define TRCNVSTMONCNT1_1_DEFAULT                                     0x00000000
#define TRCNVSTMON1_2_ADDR                                           0x000004c4
#define TRCNVSTMON1_2_DEFAULT                                        0x00000000
#define TRCNVSTMONCNT1_2_ADDR                                        0x000004c8
#define TRCNVSTMONCNT1_2_DEFAULT                                     0x00000000
#define TRCNVSTMON1_3_ADDR                                           0x000004cc
#define TRCNVSTMON1_3_DEFAULT                                        0x00000000
#define TRCNVSTMONCNT1_3_ADDR                                        0x000004d0
#define TRCNVSTMONCNT1_3_DEFAULT                                     0x00000000
#define TRCNVSTMON1_4_ADDR                                           0x000004d4
#define TRCNVSTMON1_4_DEFAULT                                        0x00000000
#define TRCNVSTMONCNT1_4_ADDR                                        0x000004d8
#define TRCNVSTMONCNT1_4_DEFAULT                                     0x00000000
#define TRCNVSTMON1_5_ADDR                                           0x000004dc
#define TRCNVSTMON1_5_DEFAULT                                        0x00000000
#define TRCNVSTMONCNT1_5_ADDR                                        0x000004e0
#define TRCNVSTMONCNT1_5_DEFAULT                                     0x00000000
#define TRCNVSTMON1_6_ADDR                                           0x000004e4
#define TRCNVSTMON1_6_DEFAULT                                        0x00000000
#define TRCNVSTMONCNT1_6_ADDR                                        0x000004e8
#define TRCNVSTMONCNT1_6_DEFAULT                                     0x00000000
#define TRCNVSTMON1_7_ADDR                                           0x000004ec
#define TRCNVSTMON1_7_DEFAULT                                        0x00000000
#define TRCNVSTMONCNT1_7_ADDR                                        0x000004f0
#define TRCNVSTMONCNT1_7_DEFAULT                                     0x00000000
#define TRCNVSTMON1_8_ADDR                                           0x000004f4
#define TRCNVSTMON1_8_DEFAULT                                        0x00000000
#define TRCNVSTMONCNT1_8_ADDR                                        0x000004f8
#define TRCNVSTMONCNT1_8_DEFAULT                                     0x00000000
#define TRCNVSTMON1_9_ADDR                                           0x000004fc
#define TRCNVSTMON1_9_DEFAULT                                        0x00000000
#define TRCNVSTMONCNT1_9_ADDR                                        0x00000500
#define TRCNVSTMONCNT1_9_DEFAULT                                     0x00000000
#define TRCNVSTMON1_10_ADDR                                          0x00000504
#define TRCNVSTMON1_10_DEFAULT                                       0x00000000
#define TRCNVSTMONCNT1_10_ADDR                                       0x00000508
#define TRCNVSTMONCNT1_10_DEFAULT                                    0x00000000
#define TRCNVSTMON1_11_ADDR                                          0x0000050c
#define TRCNVSTMON1_11_DEFAULT                                       0x00000000
#define TRCNVSTMONCNT1_11_ADDR                                       0x00000510
#define TRCNVSTMONCNT1_11_DEFAULT                                    0x00000000
#define TRCNVSTMON1_12_ADDR                                          0x00000514
#define TRCNVSTMON1_12_DEFAULT                                       0x00000000
#define TRCNVSTMONCNT1_12_ADDR                                       0x00000518
#define TRCNVSTMONCNT1_12_DEFAULT                                    0x00000000
#define TRCNVSTMON1_13_ADDR                                          0x0000051c
#define TRCNVSTMON1_13_DEFAULT                                       0x00000000
#define TRCNVSTMONCNT1_13_ADDR                                       0x00000520
#define TRCNVSTMONCNT1_13_DEFAULT                                    0x00000000
#define TRCNVSTMON1_14_ADDR                                          0x00000524
#define TRCNVSTMON1_14_DEFAULT                                       0x00000000
#define TRCNVSTMONCNT1_14_ADDR                                       0x00000528
#define TRCNVSTMONCNT1_14_DEFAULT                                    0x00000000
#define TRCNVSTMON1_15_ADDR                                          0x0000052c
#define TRCNVSTMON1_15_DEFAULT                                       0x00000000
#define TRCNVSTMONCNT1_15_ADDR                                       0x00000530
#define TRCNVSTMONCNT1_15_DEFAULT                                    0x00000000
#define SNPTR2_ADDR                                                  0x00000800
#define SNPTR2_DEFAULT                                               0x00000000
#define SPPTR2_ADDR                                                  0x00000804
#define SPPTR2_DEFAULT                                               0x00000000
#define DFCLSRESDBPTR2_ADDR                                          0x00000808
#define DFCLSRESDBPTR2_DEFAULT                                       0x00001c00
#define DFCLSRESDB2_0_ADDR                                           0x0000080c
#define DFCLSRESDB2_0_DEFAULT                                        0x00000000
#define DFCLSRESDB2_1_ADDR                                           0x00000810
#define DFCLSRESDB2_1_DEFAULT                                        0x00000000
#define DFCLSRESDB2_2_ADDR                                           0x00000814
#define DFCLSRESDB2_2_DEFAULT                                        0x00000000
#define DFCLSRESDB2_3_ADDR                                           0x00000818
#define DFCLSRESDB2_3_DEFAULT                                        0x00000000
#define DFCLSRESDB2_4_ADDR                                           0x0000081c
#define DFCLSRESDB2_4_DEFAULT                                        0x00000000
#define DFCLSRESDB2_5_ADDR                                           0x00000820
#define DFCLSRESDB2_5_DEFAULT                                        0x00000000
#define PARSER_CTL2_ADDR                                             0x00000824
#define PARSER_CTL2_DEFAULT                                          0x00000000
#define TMAXHOP2_ADDR                                                0x00000828
#define TMAXHOP2_DEFAULT                                             0x00000040
#define PORTNUM2_ADDR                                                0x0000082c
#define PORTNUM2_DEFAULT                                             0x00000002
#define PKTRAM2_ERR_DIAGNOSTIC_ADDR                                  0x00000830
#define PKTRAM2_ERR_DIAGNOSTIC_DEFAULT                               0x00000000
#define SNPTR_CHANGE_STATUS2_ADDR                                    0x00000834
#define SNPTR_CHANGE_STATUS2_DEFAULT                                 0x00000000
#define PARSER_STATUS2_ADDR                                          0x00000838
#define PARSER_STATUS2_DEFAULT                                       0x00000000
#define PARSER_STATUS2MASK_ADDR                                      0x0000083c
#define LSTNVST2_ADDR                                                0x00000860
#define LSTNVST2_DEFAULT                                             0x00000000
#define LSTTRCNVST2_0_ADDR                                           0x00000864
#define LSTTRCNVST2_0_DEFAULT                                        0x00000000
#define LSTTRCNVST2_1_ADDR                                           0x00000868
#define LSTTRCNVST2_1_DEFAULT                                        0x00000000
#define LSTTRCNVST2_2_ADDR                                           0x0000086c
#define LSTTRCNVST2_2_DEFAULT                                        0x00000000
#define LSTTRCNVST2_3_ADDR                                           0x00000870
#define LSTTRCNVST2_3_DEFAULT                                        0x00000000
#define LSTTRCNVST2_4_ADDR                                           0x00000874
#define LSTTRCNVST2_4_DEFAULT                                        0x00000000
#define LSTTRCNVST2_5_ADDR                                           0x00000878
#define LSTTRCNVST2_5_DEFAULT                                        0x00000000
#define LSTTRCNVST2_6_ADDR                                           0x0000087c
#define LSTTRCNVST2_6_DEFAULT                                        0x00000000
#define LSTTRCNVST2_7_ADDR                                           0x00000880
#define LSTTRCNVST2_7_DEFAULT                                        0x00000000
#define LSTTRCNVST2_8_ADDR                                           0x00000884
#define LSTTRCNVST2_8_DEFAULT                                        0x00000000
#define LSTTRCNVST2_9_ADDR                                           0x00000888
#define LSTTRCNVST2_9_DEFAULT                                        0x00000000
#define LSTTRCNVST2_10_ADDR                                          0x0000088c
#define LSTTRCNVST2_10_DEFAULT                                       0x00000000
#define LSTTRCNVST2_11_ADDR                                          0x00000890
#define LSTTRCNVST2_11_DEFAULT                                       0x00000000
#define LSTTRCNVST2_12_ADDR                                          0x00000894
#define LSTTRCNVST2_12_DEFAULT                                       0x00000000
#define LSTTRCNVST2_13_ADDR                                          0x00000898
#define LSTTRCNVST2_13_DEFAULT                                       0x00000000
#define LSTTRCNVST2_14_ADDR                                          0x0000089c
#define LSTTRCNVST2_14_DEFAULT                                       0x00000000
#define LSTTRCNVST2_15_ADDR                                          0x000008a0
#define LSTTRCNVST2_15_DEFAULT                                       0x00000000
#define FTRCNVSTSTR2_0_ADDR                                          0x000008b0
#define FTRCNVSTSTR2_0_DEFAULT                                       0x00000008
#define FTRCNVST2_0_ADDR                                             0x000008b4
#define FTRCNVST2_0_DEFAULT                                          0x00000000
#define FTRCNVST2_1_ADDR                                             0x000008b8
#define FTRCNVST2_1_DEFAULT                                          0x00000000
#define FTRCNVST2_2_ADDR                                             0x000008bc
#define FTRCNVST2_2_DEFAULT                                          0x00000000
#define FTRCNVST2_3_ADDR                                             0x000008c0
#define FTRCNVST2_3_DEFAULT                                          0x00000000
#define FTRCNVST2_4_ADDR                                             0x000008c4
#define FTRCNVST2_4_DEFAULT                                          0x00000000
#define FTRCNVST2_5_ADDR                                             0x000008c8
#define FTRCNVST2_5_DEFAULT                                          0x00000000
#define FTRCNVST2_6_ADDR                                             0x000008cc
#define FTRCNVST2_6_DEFAULT                                          0x00000000
#define FTRCNVST2_7_ADDR                                             0x000008d0
#define FTRCNVST2_7_DEFAULT                                          0x00000000
#define FTRCNVST2_8_ADDR                                             0x000008d4
#define FTRCNVST2_8_DEFAULT                                          0x00000000
#define FTRCNVST2_9_ADDR                                             0x000008d8
#define FTRCNVST2_9_DEFAULT                                          0x00000000
#define FTRCNVST2_10_ADDR                                            0x000008dc
#define FTRCNVST2_10_DEFAULT                                         0x00000000
#define FTRCNVST2_11_ADDR                                            0x000008e0
#define FTRCNVST2_11_DEFAULT                                         0x00000000
#define FTRCNVST2_12_ADDR                                            0x000008e4
#define FTRCNVST2_12_DEFAULT                                         0x00000000
#define FTRCNVST2_13_ADDR                                            0x000008e8
#define FTRCNVST2_13_DEFAULT                                         0x00000000
#define FTRCNVST2_14_ADDR                                            0x000008ec
#define FTRCNVST2_14_DEFAULT                                         0x00000000
#define FTRCNVST2_15_ADDR                                            0x000008f0
#define FTRCNVST2_15_DEFAULT                                         0x00000000
#define TRCNVSTMON2_0_ADDR                                           0x000008f4
#define TRCNVSTMON2_0_DEFAULT                                        0x00000000
#define TRCNVSTMONCNT2_0_ADDR                                        0x000008f8
#define TRCNVSTMONCNT2_0_DEFAULT                                     0x00000000
#define TRCNVSTMON2_1_ADDR                                           0x000008fc
#define TRCNVSTMON2_1_DEFAULT                                        0x00000000
#define TRCNVSTMONCNT2_1_ADDR                                        0x00000900
#define TRCNVSTMONCNT2_1_DEFAULT                                     0x00000000
#define TRCNVSTMON2_2_ADDR                                           0x00000904
#define TRCNVSTMON2_2_DEFAULT                                        0x00000000
#define TRCNVSTMONCNT2_2_ADDR                                        0x00000908
#define TRCNVSTMONCNT2_2_DEFAULT                                     0x00000000
#define TRCNVSTMON2_3_ADDR                                           0x0000090c
#define TRCNVSTMON2_3_DEFAULT                                        0x00000000
#define TRCNVSTMONCNT2_3_ADDR                                        0x00000910
#define TRCNVSTMONCNT2_3_DEFAULT                                     0x00000000
#define TRCNVSTMON2_4_ADDR                                           0x00000914
#define TRCNVSTMON2_4_DEFAULT                                        0x00000000
#define TRCNVSTMONCNT2_4_ADDR                                        0x00000918
#define TRCNVSTMONCNT2_4_DEFAULT                                     0x00000000
#define TRCNVSTMON2_5_ADDR                                           0x0000091c
#define TRCNVSTMON2_5_DEFAULT                                        0x00000000
#define TRCNVSTMONCNT2_5_ADDR                                        0x00000920
#define TRCNVSTMONCNT2_5_DEFAULT                                     0x00000000
#define TRCNVSTMON2_6_ADDR                                           0x00000924
#define TRCNVSTMON2_6_DEFAULT                                        0x00000000
#define TRCNVSTMONCNT2_6_ADDR                                        0x00000928
#define TRCNVSTMONCNT2_6_DEFAULT                                     0x00000000
#define TRCNVSTMON2_7_ADDR                                           0x0000092c
#define TRCNVSTMON2_7_DEFAULT                                        0x00000000
#define TRCNVSTMONCNT2_7_ADDR                                        0x00000930
#define TRCNVSTMONCNT2_7_DEFAULT                                     0x00000000
#define TRCNVSTMON2_8_ADDR                                           0x00000934
#define TRCNVSTMON2_8_DEFAULT                                        0x00000000
#define TRCNVSTMONCNT2_8_ADDR                                        0x00000938
#define TRCNVSTMONCNT2_8_DEFAULT                                     0x00000000
#define TRCNVSTMON2_9_ADDR                                           0x0000093c
#define TRCNVSTMON2_9_DEFAULT                                        0x00000000
#define TRCNVSTMONCNT2_9_ADDR                                        0x00000940
#define TRCNVSTMONCNT2_9_DEFAULT                                     0x00000000
#define TRCNVSTMON2_10_ADDR                                          0x00000944
#define TRCNVSTMON2_10_DEFAULT                                       0x00000000
#define TRCNVSTMONCNT2_10_ADDR                                       0x00000948
#define TRCNVSTMONCNT2_10_DEFAULT                                    0x00000000
#define TRCNVSTMON2_11_ADDR                                          0x0000094c
#define TRCNVSTMON2_11_DEFAULT                                       0x00000000
#define TRCNVSTMONCNT2_11_ADDR                                       0x00000950
#define TRCNVSTMONCNT2_11_DEFAULT                                    0x00000000
#define TRCNVSTMON2_12_ADDR                                          0x00000954
#define TRCNVSTMON2_12_DEFAULT                                       0x00000000
#define TRCNVSTMONCNT2_12_ADDR                                       0x00000958
#define TRCNVSTMONCNT2_12_DEFAULT                                    0x00000000
#define TRCNVSTMON2_13_ADDR                                          0x0000095c
#define TRCNVSTMON2_13_DEFAULT                                       0x00000000
#define TRCNVSTMONCNT2_13_ADDR                                       0x00000960
#define TRCNVSTMONCNT2_13_DEFAULT                                    0x00000000
#define TRCNVSTMON2_14_ADDR                                          0x00000964
#define TRCNVSTMON2_14_DEFAULT                                       0x00000000
#define TRCNVSTMONCNT2_14_ADDR                                       0x00000968
#define TRCNVSTMONCNT2_14_DEFAULT                                    0x00000000
#define TRCNVSTMON2_15_ADDR                                          0x0000096c
#define TRCNVSTMON2_15_DEFAULT                                       0x00000000
#define TRCNVSTMONCNT2_15_ADDR                                       0x00000970
#define TRCNVSTMONCNT2_15_DEFAULT                                    0x00000000
#define TSIF_CFG_CTL_ADDR                                            0x00000a00
#define TSIF_CFG_CTL_DEFAULT                                         0x000002e4
#define RSIF_CFG_CTL_ADDR                                            0x00000a04
#define RSIF_CFG_CTL_DEFAULT                                         0x8a110104
#define CLE_RSIF_POLICER_STSREG0_ADDR                                0x00000a08
#define CLE_RSIF_POLICER_STSREG0_DEFAULT                             0x00000000
#define CLE_RSIF_PRIORITY_POLICER_REG0_ADDR                          0x00000a0c
#define CLE_RSIF_PRIORITY_POLICER_REG0_DEFAULT                       0x0100ffff
#define CLE_RSIF_PRIORITY_POLICER_REG1_ADDR                          0x00000a10
#define CLE_RSIF_PRIORITY_POLICER_REG1_DEFAULT                       0x64ffffff
#define CLE_RSIF_PRIORITY_POLICER_REG2_ADDR                          0x00000a14
#define CLE_RSIF_PRIORITY_POLICER_REG2_DEFAULT                       0x00000040
#define CLE_RSIF_FLOWGROUP_POLICER_REG0_ADDR                         0x00000a18
#define CLE_RSIF_FLOWGROUP_POLICER_REG0_DEFAULT                      0x0100ffff
#define CLE_RSIF_FLOWGROUP_POLICER_REG1_ADDR                         0x00000a1c
#define CLE_RSIF_FLOWGROUP_POLICER_REG1_DEFAULT                      0x00ffffff
#define CLE_RSIF_FLOWGROUP_POLICER_REG2_ADDR                         0x00000a20
#define CLE_RSIF_FLOWGROUP_POLICER_REG2_DEFAULT                      0x00000040
#define CLE_RSIF_PERFLOW_POLICER_REG0_ADDR                           0x00000a24
#define CLE_RSIF_PERFLOW_POLICER_REG0_DEFAULT                        0x0100ffff
#define CLE_RSIF_PERFLOW_POLICER_REG1_ADDR                           0x00000a28
#define CLE_RSIF_PERFLOW_POLICER_REG1_DEFAULT                        0x00ffffff
#define CLE_RSIF_PERFLOW_POLICER_REG2_ADDR                           0x00000a2c
#define CLE_RSIF_PERFLOW_POLICER_REG2_DEFAULT                        0x00000040
#define CLE_RSIF_PRIORITY_POLICER_STSREG0_ADDR                       0x00000a30
#define CLE_RSIF_PRIORITY_POLICER_STSREG0_DEFAULT                    0x00000000
#define CLE_RSIF_FLOWGROUP_POLICER_STSREG0_ADDR                      0x00000a34
#define CLE_RSIF_FLOWGROUP_POLICER_STSREG0_DEFAULT                   0x00000000
#define CLE_RSIF_PERFLOW_POLICER_STSREG0_ADDR                        0x00000a38
#define CLE_RSIF_PERFLOW_POLICER_STSREG0_DEFAULT                     0x00000000
#define CLE_RSIF_FIFO_EMPTYSTS0_ADDR                                 0x00000a3c
#define CLE_RSIF_FIFO_EMPTYSTS0_DEFAULT                              0x0000003f
#define CLE_RSIF_INT_REG0_ADDR                                       0x00000a40
#define CLE_RSIF_INT_REG0_DEFAULT                                    0x00000000
#define CLE_RSIF_INT_REG0MASK_ADDR                                   0x00000a44
#define CLE_RSIF_FINT_REG0_ADDR                                      0x00000a48
#define CLE_RSIF_FINT_REG0_DEFAULT                                   0x00000000
#define CLE_RSIF_FINT_REG0MASK_ADDR                                  0x00000a4c
#define CLE_RSIF_STS_REG0_ADDR                                       0x00000a50
#define CLE_RSIF_STS_REG0_DEFAULT                                    0x00000000
#define CLE_TSIF_FIFO_EMPTYSTS0_ADDR                                 0x00000a54
#define CLE_TSIF_FIFO_EMPTYSTS0_DEFAULT                              0x00000007
#define CLE_TSIF_INT_REG0_ADDR                                       0x00000a58
#define CLE_TSIF_INT_REG0_DEFAULT                                    0x00000000
#define CLE_TSIF_INT_REG0MASK_ADDR                                   0x00000a5c
#define CLE_TSIF_FINT_REG0_ADDR                                      0x00000a60
#define CLE_TSIF_FINT_REG0_DEFAULT                                   0x00000000
#define CLE_TSIF_FINT_REG0MASK_ADDR                                  0x00000a64
#define CLE_TSIF_STS_REG0_ADDR                                       0x00000a68
#define CLE_TSIF_STS_REG0_DEFAULT                                    0x00000000
#define CLE_AMA_CFG_REG_ADDR                                         0x00000a80
#define CLE_AMA_CFG_REG_DEFAULT                                      0x00011110
#define SW_INT_ADDR                                                  0x00000600
#define SW_INT_DEFAULT                                               0x00000000

/*	Register cle_bid	*/ 
/*	 Fields revno	 */
#define REVNO_F4_WIDTH                                                        2
#define REVNO_F4_SHIFT                                                        8
#define REVNO_F4_MASK                                                0x00000300
#define REVNO_F4_RD(src)                              (((src) & 0x00000300)>>8)
#define REVNO_F4_SET(dst,src) \
                       (((dst) & ~0x00000300) | (((u32)(src)<<8) & 0x00000300))
/*	 Fields busid	 */
#define BUSID_F4_WIDTH                                                        3
#define BUSID_F4_SHIFT                                                        5
#define BUSID_F4_MASK                                                0x000000e0
#define BUSID_F4_RD(src)                              (((src) & 0x000000e0)>>5)
#define BUSID_F4_SET(dst,src) \
                       (((dst) & ~0x000000e0) | (((u32)(src)<<5) & 0x000000e0))
/*	 Fields deviceid	 */
#define DEVICEID_F4_WIDTH                                                     5
#define DEVICEID_F4_SHIFT                                                     0
#define DEVICEID_F4_MASK                                             0x0000001f
#define DEVICEID_F4_RD(src)                              (((src) & 0x0000001f))
#define DEVICEID_F4_SET(dst,src) \
                          (((dst) & ~0x0000001f) | (((u32)(src)) & 0x0000001f))

/*	Register indaddr	*/ 
/*	 Fields addr	 */
#define ADDR_F4_WIDTH                                                        32
#define ADDR_F4_SHIFT                                                         0
#define ADDR_F4_MASK                                                 0xffffffff
#define ADDR_F4_RD(src)                                  (((src) & 0xffffffff))
#define ADDR_F4_WR(src)                             (((u32)(src)) & 0xffffffff)
#define ADDR_F4_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register indcmd	*/ 
/*	 Fields avl_search	 */
#define AVL_SEARCH_WIDTH                                                      1
#define AVL_SEARCH_SHIFT                                                      5
#define AVL_SEARCH_MASK                                              0x00000020
#define AVL_SEARCH_RD(src)                            (((src) & 0x00000020)>>5)
#define AVL_SEARCH_WR(src)                       (((u32)(src)<<5) & 0x00000020)
#define AVL_SEARCH_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields avl_del	 */
#define AVL_DEL_WIDTH                                                         1
#define AVL_DEL_SHIFT                                                         4
#define AVL_DEL_MASK                                                 0x00000010
#define AVL_DEL_RD(src)                               (((src) & 0x00000010)>>4)
#define AVL_DEL_WR(src)                          (((u32)(src)<<4) & 0x00000010)
#define AVL_DEL_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields avl_add	 */
#define AVL_ADD_WIDTH                                                         1
#define AVL_ADD_SHIFT                                                         3
#define AVL_ADD_MASK                                                 0x00000008
#define AVL_ADD_RD(src)                               (((src) & 0x00000008)>>3)
#define AVL_ADD_WR(src)                          (((u32)(src)<<3) & 0x00000008)
#define AVL_ADD_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields lock	 */
#define LOCK_WIDTH                                                            1
#define LOCK_SHIFT                                                            2
#define LOCK_MASK                                                    0x00000004
#define LOCK_RD(src)                                  (((src) & 0x00000004)>>2)
#define LOCK_WR(src)                             (((u32)(src)<<2) & 0x00000004)
#define LOCK_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields read	 */
#define READ_F2_WIDTH                                                         1
#define READ_F2_SHIFT                                                         1
#define READ_F2_MASK                                                 0x00000002
#define READ_F2_RD(src)                               (((src) & 0x00000002)>>1)
#define READ_F2_WR(src)                          (((u32)(src)<<1) & 0x00000002)
#define READ_F2_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields write	 */
#define WRITE_F2_WIDTH                                                        1
#define WRITE_F2_SHIFT                                                        0
#define WRITE_F2_MASK                                                0x00000001
#define WRITE_F2_RD(src)                                 (((src) & 0x00000001))
#define WRITE_F2_WR(src)                            (((u32)(src)) & 0x00000001)
#define WRITE_F2_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register indcmd_status	*/ 
/*	 Fields avl_err	 */
#define AVL_ERR_WIDTH                                                         1
#define AVL_ERR_SHIFT                                                        31
#define AVL_ERR_MASK                                                 0x80000000
#define AVL_ERR_RD(src)                              (((src) & 0x80000000)>>31)
#define AVL_ERR_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields avl_del_srch_add_addr	 */
#define AVL_DEL_SRCH_ADD_ADDR_WIDTH                                          11
#define AVL_DEL_SRCH_ADD_ADDR_SHIFT                                          17
#define AVL_DEL_SRCH_ADD_ADDR_MASK                                   0x0ffe0000
#define AVL_DEL_SRCH_ADD_ADDR_RD(src)                (((src) & 0x0ffe0000)>>17)
#define AVL_DEL_SRCH_ADD_ADDR_SET(dst,src) \
                      (((dst) & ~0x0ffe0000) | (((u32)(src)<<17) & 0x0ffe0000))
/*	 Fields avl_search_hit_db_ptr	 */
#define AVL_SEARCH_HIT_DB_PTR_WIDTH                                          10
#define AVL_SEARCH_HIT_DB_PTR_SHIFT                                           7
#define AVL_SEARCH_HIT_DB_PTR_MASK                                   0x0001ff80
#define AVL_SEARCH_HIT_DB_PTR_RD(src)                 (((src) & 0x0001ff80)>>7)
#define AVL_SEARCH_HIT_DB_PTR_SET(dst,src) \
                       (((dst) & ~0x0001ff80) | (((u32)(src)<<7) & 0x0001ff80))
/*	 Fields avl_search_done_miss	 */
#define AVL_SEARCH_DONE_MISS_WIDTH                                            1
#define AVL_SEARCH_DONE_MISS_SHIFT                                            6
#define AVL_SEARCH_DONE_MISS_MASK                                    0x00000040
#define AVL_SEARCH_DONE_MISS_RD(src)                  (((src) & 0x00000040)>>6)
#define AVL_SEARCH_DONE_MISS_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*	 Fields avl_search_done_hit	 */
#define AVL_SEARCH_DONE_HIT_WIDTH                                             1
#define AVL_SEARCH_DONE_HIT_SHIFT                                             5
#define AVL_SEARCH_DONE_HIT_MASK                                     0x00000020
#define AVL_SEARCH_DONE_HIT_RD(src)                   (((src) & 0x00000020)>>5)
#define AVL_SEARCH_DONE_HIT_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields avl_del_done	 */
#define AVL_DEL_DONE_WIDTH                                                    1
#define AVL_DEL_DONE_SHIFT                                                    4
#define AVL_DEL_DONE_MASK                                            0x00000010
#define AVL_DEL_DONE_RD(src)                          (((src) & 0x00000010)>>4)
#define AVL_DEL_DONE_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields avl_add_done	 */
#define AVL_ADD_DONE_WIDTH                                                    1
#define AVL_ADD_DONE_SHIFT                                                    3
#define AVL_ADD_DONE_MASK                                            0x00000008
#define AVL_ADD_DONE_RD(src)                          (((src) & 0x00000008)>>3)
#define AVL_ADD_DONE_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields read_done	 */
#define READ_DONE_WIDTH                                                       1
#define READ_DONE_SHIFT                                                       1
#define READ_DONE_MASK                                               0x00000002
#define READ_DONE_RD(src)                             (((src) & 0x00000002)>>1)
#define READ_DONE_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields write_done	 */
#define WRITE_DONE_WIDTH                                                      1
#define WRITE_DONE_SHIFT                                                      0
#define WRITE_DONE_MASK                                              0x00000001
#define WRITE_DONE_RD(src)                               (((src) & 0x00000001))
#define WRITE_DONE_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register data_ram0	*/ 
/*	 Fields data	 */
#define DATA0_WIDTH                                                          32
#define DATA0_SHIFT                                                           0
#define DATA0_MASK                                                   0xffffffff
#define DATA0_RD(src)                                    (((src) & 0xffffffff))
#define DATA0_WR(src)                               (((u32)(src)) & 0xffffffff)
#define DATA0_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register data_ram1	*/ 
/*	 Fields data	 */
#define DATA1_WIDTH                                                          32
#define DATA1_SHIFT                                                           0
#define DATA1_MASK                                                   0xffffffff
#define DATA1_RD(src)                                    (((src) & 0xffffffff))
#define DATA1_WR(src)                               (((u32)(src)) & 0xffffffff)
#define DATA1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register data_ram2	*/ 
/*	 Fields data	 */
#define DATA2_WIDTH                                                          32
#define DATA2_SHIFT                                                           0
#define DATA2_MASK                                                   0xffffffff
#define DATA2_RD(src)                                    (((src) & 0xffffffff))
#define DATA2_WR(src)                               (((u32)(src)) & 0xffffffff)
#define DATA2_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register data_ram3	*/ 
/*	 Fields data	 */
#define DATA3_WIDTH                                                          32
#define DATA3_SHIFT                                                           0
#define DATA3_MASK                                                   0xffffffff
#define DATA3_RD(src)                                    (((src) & 0xffffffff))
#define DATA3_WR(src)                               (((u32)(src)) & 0xffffffff)
#define DATA3_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register data_ram4	*/ 
/*	 Fields data	 */
#define DATA4_WIDTH                                                          32
#define DATA4_SHIFT                                                           0
#define DATA4_MASK                                                   0xffffffff
#define DATA4_RD(src)                                    (((src) & 0xffffffff))
#define DATA4_WR(src)                               (((u32)(src)) & 0xffffffff)
#define DATA4_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register data_ram5	*/ 
/*	 Fields data	 */
#define DATA5_WIDTH                                                          32
#define DATA5_SHIFT                                                           0
#define DATA5_MASK                                                   0xffffffff
#define DATA5_RD(src)                                    (((src) & 0xffffffff))
#define DATA5_WR(src)                               (((u32)(src)) & 0xffffffff)
#define DATA5_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register data_ram6	*/ 
/*	 Fields data	 */
#define DATA6_WIDTH                                                          32
#define DATA6_SHIFT                                                           0
#define DATA6_MASK                                                   0xffffffff
#define DATA6_RD(src)                                    (((src) & 0xffffffff))
#define DATA6_WR(src)                               (((u32)(src)) & 0xffffffff)
#define DATA6_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register data_ram7	*/ 
/*	 Fields data	 */
#define DATA7_WIDTH                                                          32
#define DATA7_SHIFT                                                           0
#define DATA7_MASK                                                   0xffffffff
#define DATA7_RD(src)                                    (((src) & 0xffffffff))
#define DATA7_WR(src)                               (((u32)(src)) & 0xffffffff)
#define DATA7_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register data_ram8	*/ 
/*	 Fields data	 */
#define DATA8_WIDTH                                                          32
#define DATA8_SHIFT                                                           0
#define DATA8_MASK                                                   0xffffffff
#define DATA8_RD(src)                                    (((src) & 0xffffffff))
#define DATA8_WR(src)                               (((u32)(src)) & 0xffffffff)
#define DATA8_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register data_ram9	*/ 
/*	 Fields data	 */
#define DATA9_WIDTH                                                          32
#define DATA9_SHIFT                                                           0
#define DATA9_MASK                                                   0xffffffff
#define DATA9_RD(src)                                    (((src) & 0xffffffff))
#define DATA9_WR(src)                               (((u32)(src)) & 0xffffffff)
#define DATA9_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register data_ram10	*/ 
/*	 Fields data	 */
#define DATA0_F1_WIDTH                                                       32
#define DATA0_F1_SHIFT                                                        0
#define DATA0_F1_MASK                                                0xffffffff
#define DATA0_F1_RD(src)                                 (((src) & 0xffffffff))
#define DATA0_F1_WR(src)                            (((u32)(src)) & 0xffffffff)
#define DATA0_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register data_ram11	*/ 
/*	 Fields data	 */
#define DATA1_F1_WIDTH                                                       32
#define DATA1_F1_SHIFT                                                        0
#define DATA1_F1_MASK                                                0xffffffff
#define DATA1_F1_RD(src)                                 (((src) & 0xffffffff))
#define DATA1_F1_WR(src)                            (((u32)(src)) & 0xffffffff)
#define DATA1_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register data_ram12	*/ 
/*	 Fields data	 */
#define DATA2_F1_WIDTH                                                       32
#define DATA2_F1_SHIFT                                                        0
#define DATA2_F1_MASK                                                0xffffffff
#define DATA2_F1_RD(src)                                 (((src) & 0xffffffff))
#define DATA2_F1_WR(src)                            (((u32)(src)) & 0xffffffff)
#define DATA2_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register data_ram13	*/ 
/*	 Fields data	 */
#define DATA3_F1_WIDTH                                                       32
#define DATA3_F1_SHIFT                                                        0
#define DATA3_F1_MASK                                                0xffffffff
#define DATA3_F1_RD(src)                                 (((src) & 0xffffffff))
#define DATA3_F1_WR(src)                            (((u32)(src)) & 0xffffffff)
#define DATA3_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register data_ram14	*/ 
/*	 Fields data	 */
#define DATA4_F1_WIDTH                                                       32
#define DATA4_F1_SHIFT                                                        0
#define DATA4_F1_MASK                                                0xffffffff
#define DATA4_F1_RD(src)                                 (((src) & 0xffffffff))
#define DATA4_F1_WR(src)                            (((u32)(src)) & 0xffffffff)
#define DATA4_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register data_ram15	*/ 
/*	 Fields data	 */
#define DATA5_F1_WIDTH                                                       32
#define DATA5_F1_SHIFT                                                        0
#define DATA5_F1_MASK                                                0xffffffff
#define DATA5_F1_RD(src)                                 (((src) & 0xffffffff))
#define DATA5_F1_WR(src)                            (((u32)(src)) & 0xffffffff)
#define DATA5_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register avl_config	*/ 
/*	 Fields maxsteps_thresh	 */
#define MAXSTEPS_THRESH_WIDTH                                                12
#define MAXSTEPS_THRESH_SHIFT                                                14
#define MAXSTEPS_THRESH_MASK                                         0x03ffc000
#define MAXSTEPS_THRESH_RD(src)                      (((src) & 0x03ffc000)>>14)
#define MAXSTEPS_THRESH_WR(src)                 (((u32)(src)<<14) & 0x03ffc000)
#define MAXSTEPS_THRESH_SET(dst,src) \
                      (((dst) & ~0x03ffc000) | (((u32)(src)<<14) & 0x03ffc000))
/*	 Fields srchdb_depth	 */
#define SRCHDB_DEPTH_WIDTH                                                   12
#define SRCHDB_DEPTH_SHIFT                                                    2
#define SRCHDB_DEPTH_MASK                                            0x00003ffc
#define SRCHDB_DEPTH_RD(src)                          (((src) & 0x00003ffc)>>2)
#define SRCHDB_DEPTH_WR(src)                     (((u32)(src)<<2) & 0x00003ffc)
#define SRCHDB_DEPTH_SET(dst,src) \
                       (((dst) & ~0x00003ffc) | (((u32)(src)<<2) & 0x00003ffc))
/*	 Fields string_size	 */
#define STRING_SIZE_WIDTH                                                     2
#define STRING_SIZE_SHIFT                                                     0
#define STRING_SIZE_MASK                                             0x00000003
#define STRING_SIZE_RD(src)                              (((src) & 0x00000003))
#define STRING_SIZE_WR(src)                         (((u32)(src)) & 0x00000003)
#define STRING_SIZE_SET(dst,src) \
                          (((dst) & ~0x00000003) | (((u32)(src)) & 0x00000003))

/*	Register avl_status	*/ 
/*	 Fields root_addr	 */
#define ROOT_ADDR_WIDTH                                                      11
#define ROOT_ADDR_SHIFT                                                      12
#define ROOT_ADDR_MASK                                               0x007ff000
#define ROOT_ADDR_RD(src)                            (((src) & 0x007ff000)>>12)
#define ROOT_ADDR_SET(dst,src) \
                      (((dst) & ~0x007ff000) | (((u32)(src)<<12) & 0x007ff000))
/*	 Fields node_cnt	 */
#define NODE_CNT_WIDTH                                                       12
#define NODE_CNT_SHIFT                                                        0
#define NODE_CNT_MASK                                                0x00000fff
#define NODE_CNT_RD(src)                                 (((src) & 0x00000fff))
#define NODE_CNT_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register snptr0	*/ 
/*	 Fields wol_mode	 */
#define WOL_MODE0_WIDTH                                                       1
#define WOL_MODE0_SHIFT                                                      31
#define WOL_MODE0_MASK                                               0x80000000
#define WOL_MODE0_RD(src)                            (((src) & 0x80000000)>>31)
#define WOL_MODE0_WR(src)                       (((u32)(src)<<31) & 0x80000000)
#define WOL_MODE0_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields cle_eth_spare	 */
#define CLE_ETH_SPARE0_WIDTH                                                  4
#define CLE_ETH_SPARE0_SHIFT                                                 27
#define CLE_ETH_SPARE0_MASK                                          0x78000000
#define CLE_ETH_SPARE0_RD(src)                       (((src) & 0x78000000)>>27)
#define CLE_ETH_SPARE0_WR(src)                  (((u32)(src)<<27) & 0x78000000)
#define CLE_ETH_SPARE0_SET(dst,src) \
                      (((dst) & ~0x78000000) | (((u32)(src)<<27) & 0x78000000))
/*	 Fields snptr	 */
#define SNPTR0_WIDTH                                                         12
#define SNPTR0_SHIFT                                                          0
#define SNPTR0_MASK                                                  0x00000fff
#define SNPTR0_RD(src)                                   (((src) & 0x00000fff))
#define SNPTR0_WR(src)                              (((u32)(src)) & 0x00000fff)
#define SNPTR0_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register spptr0	*/ 
/*	 Fields spptr	 */
#define SPPTR0_WIDTH                                                          9
#define SPPTR0_SHIFT                                                          0
#define SPPTR0_MASK                                                  0x000001ff
#define SPPTR0_RD(src)                                   (((src) & 0x000001ff))
#define SPPTR0_WR(src)                              (((u32)(src)) & 0x000001ff)
#define SPPTR0_SET(dst,src) \
                          (((dst) & ~0x000001ff) | (((u32)(src)) & 0x000001ff))

/*	Register dfclsresdbptr0	*/ 
/*	 Fields dfclsresdbpriority	 */
#define DFCLSRESDBPRIORITY0_WIDTH                                             3
#define DFCLSRESDBPRIORITY0_SHIFT                                            10
#define DFCLSRESDBPRIORITY0_MASK                                     0x00001c00
#define DFCLSRESDBPRIORITY0_RD(src)                  (((src) & 0x00001c00)>>10)
#define DFCLSRESDBPRIORITY0_WR(src)             (((u32)(src)<<10) & 0x00001c00)
#define DFCLSRESDBPRIORITY0_SET(dst,src) \
                      (((dst) & ~0x00001c00) | (((u32)(src)<<10) & 0x00001c00))
/*	 Fields dfclsresdbptr	 */
#define DFCLSRESDBPTR0_WIDTH                                                 10
#define DFCLSRESDBPTR0_SHIFT                                                  0
#define DFCLSRESDBPTR0_MASK                                          0x000003ff
#define DFCLSRESDBPTR0_RD(src)                           (((src) & 0x000003ff))
#define DFCLSRESDBPTR0_WR(src)                      (((u32)(src)) & 0x000003ff)
#define DFCLSRESDBPTR0_SET(dst,src) \
                          (((dst) & ~0x000003ff) | (((u32)(src)) & 0x000003ff))

/*	Register dfclsresdb0_0	*/ 
/*	 Fields dfclsresdb	 */
#define DFCLSRESDB0_WIDTH                                                    32
#define DFCLSRESDB0_SHIFT                                                     0
#define DFCLSRESDB0_MASK                                             0xffffffff
#define DFCLSRESDB0_RD(src)                              (((src) & 0xffffffff))
#define DFCLSRESDB0_WR(src)                         (((u32)(src)) & 0xffffffff)
#define DFCLSRESDB0_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register dfclsresdb0_1	*/ 
/*	 Fields dfclsresdb	 */
#define DFCLSRESDB1_WIDTH                                                    32
#define DFCLSRESDB1_SHIFT                                                     0
#define DFCLSRESDB1_MASK                                             0xffffffff
#define DFCLSRESDB1_RD(src)                              (((src) & 0xffffffff))
#define DFCLSRESDB1_WR(src)                         (((u32)(src)) & 0xffffffff)
#define DFCLSRESDB1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register dfclsresdb0_2	*/ 
/*	 Fields dfclsresdb	 */
#define DFCLSRESDB2_WIDTH                                                    32
#define DFCLSRESDB2_SHIFT                                                     0
#define DFCLSRESDB2_MASK                                             0xffffffff
#define DFCLSRESDB2_RD(src)                              (((src) & 0xffffffff))
#define DFCLSRESDB2_WR(src)                         (((u32)(src)) & 0xffffffff)
#define DFCLSRESDB2_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register dfclsresdb0_3	*/ 
/*	 Fields dfclsresdb	 */
#define DFCLSRESDB3_WIDTH                                                    32
#define DFCLSRESDB3_SHIFT                                                     0
#define DFCLSRESDB3_MASK                                             0xffffffff
#define DFCLSRESDB3_RD(src)                              (((src) & 0xffffffff))
#define DFCLSRESDB3_WR(src)                         (((u32)(src)) & 0xffffffff)
#define DFCLSRESDB3_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register dfclsresdb0_4	*/ 
/*	 Fields dfclsresdb	 */
#define DFCLSRESDB4_WIDTH                                                    32
#define DFCLSRESDB4_SHIFT                                                     0
#define DFCLSRESDB4_MASK                                             0xffffffff
#define DFCLSRESDB4_RD(src)                              (((src) & 0xffffffff))
#define DFCLSRESDB4_WR(src)                         (((u32)(src)) & 0xffffffff)
#define DFCLSRESDB4_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register dfclsresdb0_5	*/ 
/*	 Fields dfclsresdb	 */
#define DFCLSRESDB5_WIDTH                                                    32
#define DFCLSRESDB5_SHIFT                                                     0
#define DFCLSRESDB5_MASK                                             0xffffffff
#define DFCLSRESDB5_RD(src)                              (((src) & 0xffffffff))
#define DFCLSRESDB5_WR(src)                         (((u32)(src)) & 0xffffffff)
#define DFCLSRESDB5_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register parser_ctl0	*/ 
/*	 Fields pkt_data_msk_en	 */
#define PKT_DATA_MSK_EN0_WIDTH                                                1
#define PKT_DATA_MSK_EN0_SHIFT                                               31
#define PKT_DATA_MSK_EN0_MASK                                        0x80000000
#define PKT_DATA_MSK_EN0_RD(src)                     (((src) & 0x80000000)>>31)
#define PKT_DATA_MSK_EN0_WR(src)                (((u32)(src)<<31) & 0x80000000)
#define PKT_DATA_MSK_EN0_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields parser_halt	 */
#define PARSER_HALT0_WIDTH                                                    1
#define PARSER_HALT0_SHIFT                                                    0
#define PARSER_HALT0_MASK                                            0x00000001
#define PARSER_HALT0_RD(src)                             (((src) & 0x00000001))
#define PARSER_HALT0_WR(src)                        (((u32)(src)) & 0x00000001)
#define PARSER_HALT0_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register tmaxhop0	*/ 
/*	 Fields tmaxhop	 */
#define TMAXHOP0_WIDTH                                                        8
#define TMAXHOP0_SHIFT                                                        0
#define TMAXHOP0_MASK                                                0x000000ff
#define TMAXHOP0_RD(src)                                 (((src) & 0x000000ff))
#define TMAXHOP0_WR(src)                            (((u32)(src)) & 0x000000ff)
#define TMAXHOP0_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register portnum0	*/ 
/*	 Fields portnum0	 */
#define PORTNUM00_WIDTH                                                       8
#define PORTNUM00_SHIFT                                                       0
#define PORTNUM00_MASK                                               0x000000ff
#define PORTNUM00_RD(src)                                (((src) & 0x000000ff))
#define PORTNUM00_WR(src)                           (((u32)(src)) & 0x000000ff)
#define PORTNUM00_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register ds_ctl	*/ 
/*	 Fields sleep_stat	 */
#define SLEEP_STAT_WIDTH                                                      4
#define SLEEP_STAT_SHIFT                                                     28
#define SLEEP_STAT_MASK                                              0xf0000000
#define SLEEP_STAT_RD(src)                           (((src) & 0xf0000000)>>28)
#define SLEEP_STAT_SET(dst,src) \
                      (((dst) & ~0xf0000000) | (((u32)(src)<<28) & 0xf0000000))
/*	 Fields wake_stat	 */
#define WAKE_STAT_WIDTH                                                       4
#define WAKE_STAT_SHIFT                                                      24
#define WAKE_STAT_MASK                                               0x0f000000
#define WAKE_STAT_RD(src)                            (((src) & 0x0f000000)>>24)
#define WAKE_STAT_SET(dst,src) \
                      (((dst) & ~0x0f000000) | (((u32)(src)<<24) & 0x0f000000))
/*	 Fields in_progress_stat	 */
#define IN_PROGRESS_STAT_WIDTH                                                4
#define IN_PROGRESS_STAT_SHIFT                                               20
#define IN_PROGRESS_STAT_MASK                                        0x00f00000
#define IN_PROGRESS_STAT_RD(src)                     (((src) & 0x00f00000)>>20)
#define IN_PROGRESS_STAT_SET(dst,src) \
                      (((dst) & ~0x00f00000) | (((u32)(src)<<20) & 0x00f00000))
/*	 Fields ram_ds	 */
#define RAM_DS_WIDTH                                                          1
#define RAM_DS_SHIFT                                                         19
#define RAM_DS_MASK                                                  0x00080000
#define RAM_DS_RD(src)                               (((src) & 0x00080000)>>19)
#define RAM_DS_SET(dst,src) \
                      (((dst) & ~0x00080000) | (((u32)(src)<<19) & 0x00080000))
/*	 Fields ram_rdy	 */
#define RAM_RDY_WIDTH                                                         1
#define RAM_RDY_SHIFT                                                        18
#define RAM_RDY_MASK                                                 0x00040000
#define RAM_RDY_RD(src)                              (((src) & 0x00040000)>>18)
#define RAM_RDY_SET(dst,src) \
                      (((dst) & ~0x00040000) | (((u32)(src)<<18) & 0x00040000))
/*	 Fields dis_enet_rx_empty_usage	 */
#define DIS_ENET_RX_EMPTY_USAGE_WIDTH                                         1
#define DIS_ENET_RX_EMPTY_USAGE_SHIFT                                        17
#define DIS_ENET_RX_EMPTY_USAGE_MASK                                 0x00020000
#define DIS_ENET_RX_EMPTY_USAGE_RD(src)              (((src) & 0x00020000)>>17)
#define DIS_ENET_RX_EMPTY_USAGE_WR(src)         (((u32)(src)<<17) & 0x00020000)
#define DIS_ENET_RX_EMPTY_USAGE_SET(dst,src) \
                      (((dst) & ~0x00020000) | (((u32)(src)<<17) & 0x00020000))
/*	 Fields ds_ctl_enable	 */
#define DS_CTL_ENABLE_WIDTH                                                   1
#define DS_CTL_ENABLE_SHIFT                                                  16
#define DS_CTL_ENABLE_MASK                                           0x00010000
#define DS_CTL_ENABLE_RD(src)                        (((src) & 0x00010000)>>16)
#define DS_CTL_ENABLE_WR(src)                   (((u32)(src)<<16) & 0x00010000)
#define DS_CTL_ENABLE_SET(dst,src) \
                      (((dst) & ~0x00010000) | (((u32)(src)<<16) & 0x00010000))
/*	 Fields ds_sleep_timeout_cnt	 */
#define DS_SLEEP_TIMEOUT_CNT_WIDTH                                            8
#define DS_SLEEP_TIMEOUT_CNT_SHIFT                                            8
#define DS_SLEEP_TIMEOUT_CNT_MASK                                    0x0000ff00
#define DS_SLEEP_TIMEOUT_CNT_RD(src)                  (((src) & 0x0000ff00)>>8)
#define DS_SLEEP_TIMEOUT_CNT_WR(src)             (((u32)(src)<<8) & 0x0000ff00)
#define DS_SLEEP_TIMEOUT_CNT_SET(dst,src) \
                       (((dst) & ~0x0000ff00) | (((u32)(src)<<8) & 0x0000ff00))
/*	 Fields ds_wake_timeout_cnt	 */
#define DS_WAKE_TIMEOUT_CNT_WIDTH                                             8
#define DS_WAKE_TIMEOUT_CNT_SHIFT                                             0
#define DS_WAKE_TIMEOUT_CNT_MASK                                     0x000000ff
#define DS_WAKE_TIMEOUT_CNT_RD(src)                      (((src) & 0x000000ff))
#define DS_WAKE_TIMEOUT_CNT_WR(src)                 (((u32)(src)) & 0x000000ff)
#define DS_WAKE_TIMEOUT_CNT_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register snptr1	*/ 
/*	 Fields wol_mode	 */
#define WOL_MODE1_WIDTH                                                       1
#define WOL_MODE1_SHIFT                                                      31
#define WOL_MODE1_MASK                                               0x80000000
#define WOL_MODE1_RD(src)                            (((src) & 0x80000000)>>31)
#define WOL_MODE1_WR(src)                       (((u32)(src)<<31) & 0x80000000)
#define WOL_MODE1_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields cle_eth_spare	 */
#define CLE_ETH_SPARE1_WIDTH                                                  4
#define CLE_ETH_SPARE1_SHIFT                                                 27
#define CLE_ETH_SPARE1_MASK                                          0x78000000
#define CLE_ETH_SPARE1_RD(src)                       (((src) & 0x78000000)>>27)
#define CLE_ETH_SPARE1_WR(src)                  (((u32)(src)<<27) & 0x78000000)
#define CLE_ETH_SPARE1_SET(dst,src) \
                      (((dst) & ~0x78000000) | (((u32)(src)<<27) & 0x78000000))
/*	 Fields snptr	 */
#define SNPTR1_WIDTH                                                         12
#define SNPTR1_SHIFT                                                          0
#define SNPTR1_MASK                                                  0x00000fff
#define SNPTR1_RD(src)                                   (((src) & 0x00000fff))
#define SNPTR1_WR(src)                              (((u32)(src)) & 0x00000fff)
#define SNPTR1_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register spptr1	*/ 
/*	 Fields spptr	 */
#define SPPTR1_WIDTH                                                          9
#define SPPTR1_SHIFT                                                          0
#define SPPTR1_MASK                                                  0x000001ff
#define SPPTR1_RD(src)                                   (((src) & 0x000001ff))
#define SPPTR1_WR(src)                              (((u32)(src)) & 0x000001ff)
#define SPPTR1_SET(dst,src) \
                          (((dst) & ~0x000001ff) | (((u32)(src)) & 0x000001ff))

/*	Register dfclsresdbptr1	*/ 
/*	 Fields dfclsresdbpriority	 */
#define DFCLSRESDBPRIORITY1_WIDTH                                             3
#define DFCLSRESDBPRIORITY1_SHIFT                                            10
#define DFCLSRESDBPRIORITY1_MASK                                     0x00001c00
#define DFCLSRESDBPRIORITY1_RD(src)                  (((src) & 0x00001c00)>>10)
#define DFCLSRESDBPRIORITY1_WR(src)             (((u32)(src)<<10) & 0x00001c00)
#define DFCLSRESDBPRIORITY1_SET(dst,src) \
                      (((dst) & ~0x00001c00) | (((u32)(src)<<10) & 0x00001c00))
/*	 Fields dfclsresdbptr	 */
#define DFCLSRESDBPTR1_WIDTH                                                 10
#define DFCLSRESDBPTR1_SHIFT                                                  0
#define DFCLSRESDBPTR1_MASK                                          0x000003ff
#define DFCLSRESDBPTR1_RD(src)                           (((src) & 0x000003ff))
#define DFCLSRESDBPTR1_WR(src)                      (((u32)(src)) & 0x000003ff)
#define DFCLSRESDBPTR1_SET(dst,src) \
                          (((dst) & ~0x000003ff) | (((u32)(src)) & 0x000003ff))

/*	Register dfclsresdb1_0	*/ 
/*	 Fields dfclsresdb	 */
#define DFCLSRESDB0_F1_WIDTH                                                 32
#define DFCLSRESDB0_F1_SHIFT                                                  0
#define DFCLSRESDB0_F1_MASK                                          0xffffffff
#define DFCLSRESDB0_F1_RD(src)                           (((src) & 0xffffffff))
#define DFCLSRESDB0_F1_WR(src)                      (((u32)(src)) & 0xffffffff)
#define DFCLSRESDB0_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register dfclsresdb1_1	*/ 
/*	 Fields dfclsresdb	 */
#define DFCLSRESDB1_F1_WIDTH                                                 32
#define DFCLSRESDB1_F1_SHIFT                                                  0
#define DFCLSRESDB1_F1_MASK                                          0xffffffff
#define DFCLSRESDB1_F1_RD(src)                           (((src) & 0xffffffff))
#define DFCLSRESDB1_F1_WR(src)                      (((u32)(src)) & 0xffffffff)
#define DFCLSRESDB1_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register dfclsresdb1_2	*/ 
/*	 Fields dfclsresdb	 */
#define DFCLSRESDB2_F1_WIDTH                                                 32
#define DFCLSRESDB2_F1_SHIFT                                                  0
#define DFCLSRESDB2_F1_MASK                                          0xffffffff
#define DFCLSRESDB2_F1_RD(src)                           (((src) & 0xffffffff))
#define DFCLSRESDB2_F1_WR(src)                      (((u32)(src)) & 0xffffffff)
#define DFCLSRESDB2_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register dfclsresdb1_3	*/ 
/*	 Fields dfclsresdb	 */
#define DFCLSRESDB3_F1_WIDTH                                                 32
#define DFCLSRESDB3_F1_SHIFT                                                  0
#define DFCLSRESDB3_F1_MASK                                          0xffffffff
#define DFCLSRESDB3_F1_RD(src)                           (((src) & 0xffffffff))
#define DFCLSRESDB3_F1_WR(src)                      (((u32)(src)) & 0xffffffff)
#define DFCLSRESDB3_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register dfclsresdb1_4	*/ 
/*	 Fields dfclsresdb	 */
#define DFCLSRESDB4_F1_WIDTH                                                 32
#define DFCLSRESDB4_F1_SHIFT                                                  0
#define DFCLSRESDB4_F1_MASK                                          0xffffffff
#define DFCLSRESDB4_F1_RD(src)                           (((src) & 0xffffffff))
#define DFCLSRESDB4_F1_WR(src)                      (((u32)(src)) & 0xffffffff)
#define DFCLSRESDB4_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register dfclsresdb1_5	*/ 
/*	 Fields dfclsresdb	 */
#define DFCLSRESDB5_F1_WIDTH                                                 32
#define DFCLSRESDB5_F1_SHIFT                                                  0
#define DFCLSRESDB5_F1_MASK                                          0xffffffff
#define DFCLSRESDB5_F1_RD(src)                           (((src) & 0xffffffff))
#define DFCLSRESDB5_F1_WR(src)                      (((u32)(src)) & 0xffffffff)
#define DFCLSRESDB5_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register parser_ctl1	*/ 
/*	 Fields pkt_data_msk_en	 */
#define PKT_DATA_MSK_EN1_WIDTH                                                1
#define PKT_DATA_MSK_EN1_SHIFT                                               31
#define PKT_DATA_MSK_EN1_MASK                                        0x80000000
#define PKT_DATA_MSK_EN1_RD(src)                     (((src) & 0x80000000)>>31)
#define PKT_DATA_MSK_EN1_WR(src)                (((u32)(src)<<31) & 0x80000000)
#define PKT_DATA_MSK_EN1_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields parser_halt	 */
#define PARSER_HALT1_WIDTH                                                    1
#define PARSER_HALT1_SHIFT                                                    0
#define PARSER_HALT1_MASK                                            0x00000001
#define PARSER_HALT1_RD(src)                             (((src) & 0x00000001))
#define PARSER_HALT1_WR(src)                        (((u32)(src)) & 0x00000001)
#define PARSER_HALT1_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register tmaxhop1	*/ 
/*	 Fields tmaxhop	 */
#define TMAXHOP1_WIDTH                                                        8
#define TMAXHOP1_SHIFT                                                        0
#define TMAXHOP1_MASK                                                0x000000ff
#define TMAXHOP1_RD(src)                                 (((src) & 0x000000ff))
#define TMAXHOP1_WR(src)                            (((u32)(src)) & 0x000000ff)
#define TMAXHOP1_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register portnum1	*/ 
/*	 Fields portnum1	 */
#define PORTNUM11_WIDTH                                                       8
#define PORTNUM11_SHIFT                                                       0
#define PORTNUM11_MASK                                               0x000000ff
#define PORTNUM11_RD(src)                                (((src) & 0x000000ff))
#define PORTNUM11_WR(src)                           (((u32)(src)) & 0x000000ff)
#define PORTNUM11_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register ptram_err_diagnostic	*/ 
/*	 Fields Error_Addr	 */
#define ERROR_ADDR_WIDTH                                                      7
#define ERROR_ADDR_SHIFT                                                      2
#define ERROR_ADDR_MASK                                              0x000001fc
#define ERROR_ADDR_RD(src)                            (((src) & 0x000001fc)>>2)
#define ERROR_ADDR_WR(src)                       (((u32)(src)<<2) & 0x000001fc)
#define ERROR_ADDR_SET(dst,src) \
                       (((dst) & ~0x000001fc) | (((u32)(src)<<2) & 0x000001fc))
/*	 Fields Error_Addr_Enable	 */
#define ERROR_ADDR_ENABLE_WIDTH                                               1
#define ERROR_ADDR_ENABLE_SHIFT                                               1
#define ERROR_ADDR_ENABLE_MASK                                       0x00000002
#define ERROR_ADDR_ENABLE_RD(src)                     (((src) & 0x00000002)>>1)
#define ERROR_ADDR_ENABLE_WR(src)                (((u32)(src)<<1) & 0x00000002)
#define ERROR_ADDR_ENABLE_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields Force_Read_Error_Enable	 */
#define FORCE_READ_ERROR_ENABLE_WIDTH                                         1
#define FORCE_READ_ERROR_ENABLE_SHIFT                                         0
#define FORCE_READ_ERROR_ENABLE_MASK                                 0x00000001
#define FORCE_READ_ERROR_ENABLE_RD(src)                  (((src) & 0x00000001))
#define FORCE_READ_ERROR_ENABLE_WR(src)             (((u32)(src)) & 0x00000001)
#define FORCE_READ_ERROR_ENABLE_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register pktram0_err_diagnostic	*/ 
/*	 Fields Error_Addr	 */
#define ERROR_ADDR_F1_WIDTH                                                   6
#define ERROR_ADDR_F1_SHIFT                                                   2
#define ERROR_ADDR_F1_MASK                                           0x000000fc
#define ERROR_ADDR_F1_RD(src)                         (((src) & 0x000000fc)>>2)
#define ERROR_ADDR_F1_WR(src)                    (((u32)(src)<<2) & 0x000000fc)
#define ERROR_ADDR_F1_SET(dst,src) \
                       (((dst) & ~0x000000fc) | (((u32)(src)<<2) & 0x000000fc))
/*	 Fields Error_Addr_Enable	 */
#define ERROR_ADDR_ENABLE_F1_WIDTH                                            1
#define ERROR_ADDR_ENABLE_F1_SHIFT                                            1
#define ERROR_ADDR_ENABLE_F1_MASK                                    0x00000002
#define ERROR_ADDR_ENABLE_F1_RD(src)                  (((src) & 0x00000002)>>1)
#define ERROR_ADDR_ENABLE_F1_WR(src)             (((u32)(src)<<1) & 0x00000002)
#define ERROR_ADDR_ENABLE_F1_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields Force_Read_Error_Enable	 */
#define FORCE_READ_ERROR_ENABLE_F1_WIDTH                                      1
#define FORCE_READ_ERROR_ENABLE_F1_SHIFT                                      0
#define FORCE_READ_ERROR_ENABLE_F1_MASK                              0x00000001
#define FORCE_READ_ERROR_ENABLE_F1_RD(src)               (((src) & 0x00000001))
#define FORCE_READ_ERROR_ENABLE_F1_WR(src)          (((u32)(src)) & 0x00000001)
#define FORCE_READ_ERROR_ENABLE_F1_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register pktram1_err_diagnostic	*/ 
/*	 Fields Error_Addr	 */
#define ERROR_ADDR_F2_WIDTH                                                   6
#define ERROR_ADDR_F2_SHIFT                                                   2
#define ERROR_ADDR_F2_MASK                                           0x000000fc
#define ERROR_ADDR_F2_RD(src)                         (((src) & 0x000000fc)>>2)
#define ERROR_ADDR_F2_WR(src)                    (((u32)(src)<<2) & 0x000000fc)
#define ERROR_ADDR_F2_SET(dst,src) \
                       (((dst) & ~0x000000fc) | (((u32)(src)<<2) & 0x000000fc))
/*	 Fields Error_Addr_Enable	 */
#define ERROR_ADDR_ENABLE_F2_WIDTH                                            1
#define ERROR_ADDR_ENABLE_F2_SHIFT                                            1
#define ERROR_ADDR_ENABLE_F2_MASK                                    0x00000002
#define ERROR_ADDR_ENABLE_F2_RD(src)                  (((src) & 0x00000002)>>1)
#define ERROR_ADDR_ENABLE_F2_WR(src)             (((u32)(src)<<1) & 0x00000002)
#define ERROR_ADDR_ENABLE_F2_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields Force_Read_Error_Enable	 */
#define FORCE_READ_ERROR_ENABLE_F2_WIDTH                                      1
#define FORCE_READ_ERROR_ENABLE_F2_SHIFT                                      0
#define FORCE_READ_ERROR_ENABLE_F2_MASK                              0x00000001
#define FORCE_READ_ERROR_ENABLE_F2_RD(src)               (((src) & 0x00000001))
#define FORCE_READ_ERROR_ENABLE_F2_WR(src)          (((u32)(src)) & 0x00000001)
#define FORCE_READ_ERROR_ENABLE_F2_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register avlram_err_diagnostic	*/ 
/*	 Fields Error_Addr	 */
#define ERROR_ADDR_F3_WIDTH                                                   9
#define ERROR_ADDR_F3_SHIFT                                                   2
#define ERROR_ADDR_F3_MASK                                           0x000007fc
#define ERROR_ADDR_F3_RD(src)                         (((src) & 0x000007fc)>>2)
#define ERROR_ADDR_F3_WR(src)                    (((u32)(src)<<2) & 0x000007fc)
#define ERROR_ADDR_F3_SET(dst,src) \
                       (((dst) & ~0x000007fc) | (((u32)(src)<<2) & 0x000007fc))
/*	 Fields Error_Addr_Enable	 */
#define ERROR_ADDR_ENABLE_F3_WIDTH                                            1
#define ERROR_ADDR_ENABLE_F3_SHIFT                                            1
#define ERROR_ADDR_ENABLE_F3_MASK                                    0x00000002
#define ERROR_ADDR_ENABLE_F3_RD(src)                  (((src) & 0x00000002)>>1)
#define ERROR_ADDR_ENABLE_F3_WR(src)             (((u32)(src)<<1) & 0x00000002)
#define ERROR_ADDR_ENABLE_F3_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields Force_Read_Error_Enable	 */
#define FORCE_READ_ERROR_ENABLE_F3_WIDTH                                      1
#define FORCE_READ_ERROR_ENABLE_F3_SHIFT                                      0
#define FORCE_READ_ERROR_ENABLE_F3_MASK                              0x00000001
#define FORCE_READ_ERROR_ENABLE_F3_RD(src)               (((src) & 0x00000001))
#define FORCE_READ_ERROR_ENABLE_F3_WR(src)          (((u32)(src)) & 0x00000001)
#define FORCE_READ_ERROR_ENABLE_F3_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register dbram_err_diagnostic	*/ 
/*	 Fields Error_Addr	 */
#define ERROR_ADDR_F4_WIDTH                                                  10
#define ERROR_ADDR_F4_SHIFT                                                   2
#define ERROR_ADDR_F4_MASK                                           0x00000ffc
#define ERROR_ADDR_F4_RD(src)                         (((src) & 0x00000ffc)>>2)
#define ERROR_ADDR_F4_WR(src)                    (((u32)(src)<<2) & 0x00000ffc)
#define ERROR_ADDR_F4_SET(dst,src) \
                       (((dst) & ~0x00000ffc) | (((u32)(src)<<2) & 0x00000ffc))
/*	 Fields Error_Addr_Enable	 */
#define ERROR_ADDR_ENABLE_F4_WIDTH                                            1
#define ERROR_ADDR_ENABLE_F4_SHIFT                                            1
#define ERROR_ADDR_ENABLE_F4_MASK                                    0x00000002
#define ERROR_ADDR_ENABLE_F4_RD(src)                  (((src) & 0x00000002)>>1)
#define ERROR_ADDR_ENABLE_F4_WR(src)             (((u32)(src)<<1) & 0x00000002)
#define ERROR_ADDR_ENABLE_F4_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields Force_Read_Error_Enable	 */
#define FORCE_READ_ERROR_ENABLE_F4_WIDTH                                      1
#define FORCE_READ_ERROR_ENABLE_F4_SHIFT                                      0
#define FORCE_READ_ERROR_ENABLE_F4_MASK                              0x00000001
#define FORCE_READ_ERROR_ENABLE_F4_RD(src)               (((src) & 0x00000001))
#define FORCE_READ_ERROR_ENABLE_F4_WR(src)          (((u32)(src)) & 0x00000001)
#define FORCE_READ_ERROR_ENABLE_F4_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register snptr_change_status0	*/ 
/*	 Fields snptr_change_done	 */
#define SNPTR_CHANGE_DONE0_WIDTH                                              1
#define SNPTR_CHANGE_DONE0_SHIFT                                              0
#define SNPTR_CHANGE_DONE0_MASK                                      0x00000001
#define SNPTR_CHANGE_DONE0_RD(src)                       (((src) & 0x00000001))
#define SNPTR_CHANGE_DONE0_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register snptr_change_status1	*/ 
/*	 Fields snptr_change_done	 */
#define SNPTR_CHANGE_DONE1_WIDTH                                              1
#define SNPTR_CHANGE_DONE1_SHIFT                                              0
#define SNPTR_CHANGE_DONE1_MASK                                      0x00000001
#define SNPTR_CHANGE_DONE1_RD(src)                       (((src) & 0x00000001))
#define SNPTR_CHANGE_DONE1_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register parser_status0	*/ 
/*	 Fields PktRamErr	 */
#define PKTRAMERR0_WIDTH                                                      1
#define PKTRAMERR0_SHIFT                                                      4
#define PKTRAMERR0_MASK                                              0x00000010
#define PKTRAMERR0_RD(src)                            (((src) & 0x00000010)>>4)
#define PKTRAMERR0_WR(src)                       (((u32)(src)<<4) & 0x00000010)
#define PKTRAMERR0_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields PTRamErr	 */
#define PTRAMERR0_WIDTH                                                       1
#define PTRAMERR0_SHIFT                                                       3
#define PTRAMERR0_MASK                                               0x00000008
#define PTRAMERR0_RD(src)                             (((src) & 0x00000008)>>3)
#define PTRAMERR0_WR(src)                        (((u32)(src)<<3) & 0x00000008)
#define PTRAMERR0_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields PktOffsetExEop	 */
#define PKTOFFSETEXEOP0_WIDTH                                                 1
#define PKTOFFSETEXEOP0_SHIFT                                                 2
#define PKTOFFSETEXEOP0_MASK                                         0x00000004
#define PKTOFFSETEXEOP0_RD(src)                       (((src) & 0x00000004)>>2)
#define PKTOFFSETEXEOP0_WR(src)                  (((u32)(src)<<2) & 0x00000004)
#define PKTOFFSETEXEOP0_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields maxhoperr	 */
#define MAXHOPERR0_WIDTH                                                      1
#define MAXHOPERR0_SHIFT                                                      1
#define MAXHOPERR0_MASK                                              0x00000002
#define MAXHOPERR0_RD(src)                            (((src) & 0x00000002)>>1)
#define MAXHOPERR0_WR(src)                       (((u32)(src)<<1) & 0x00000002)
#define MAXHOPERR0_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields parser_halt_status	 */
#define PARSER_HALT_STATUS0_WIDTH                                             1
#define PARSER_HALT_STATUS0_SHIFT                                             0
#define PARSER_HALT_STATUS0_MASK                                     0x00000001
#define PARSER_HALT_STATUS0_RD(src)                      (((src) & 0x00000001))
#define PARSER_HALT_STATUS0_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register parser_status0Mask	*/
/*    Mask Register Fields PktRamErrMask    */
#define PKTRAMERRMASK_WIDTH                                                   1
#define PKTRAMERRMASK_SHIFT                                                   4
#define PKTRAMERRMASK_MASK                                           0x00000010
#define PKTRAMERRMASK_RD(src)                         (((src) & 0x00000010)>>4)
#define PKTRAMERRMASK_WR(src)                    (((u32)(src)<<4) & 0x00000010)
#define PKTRAMERRMASK_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*    Mask Register Fields PTRamErrMask    */
#define PTRAMERRMASK_WIDTH                                                    1
#define PTRAMERRMASK_SHIFT                                                    3
#define PTRAMERRMASK_MASK                                            0x00000008
#define PTRAMERRMASK_RD(src)                          (((src) & 0x00000008)>>3)
#define PTRAMERRMASK_WR(src)                     (((u32)(src)<<3) & 0x00000008)
#define PTRAMERRMASK_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*    Mask Register Fields PktOffsetExEopMask    */
#define PKTOFFSETEXEOPMASK_WIDTH                                              1
#define PKTOFFSETEXEOPMASK_SHIFT                                              2
#define PKTOFFSETEXEOPMASK_MASK                                      0x00000004
#define PKTOFFSETEXEOPMASK_RD(src)                    (((src) & 0x00000004)>>2)
#define PKTOFFSETEXEOPMASK_WR(src)               (((u32)(src)<<2) & 0x00000004)
#define PKTOFFSETEXEOPMASK_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*    Mask Register Fields maxhoperrMask    */
#define MAXHOPERRMASK_WIDTH                                                   1
#define MAXHOPERRMASK_SHIFT                                                   1
#define MAXHOPERRMASK_MASK                                           0x00000002
#define MAXHOPERRMASK_RD(src)                         (((src) & 0x00000002)>>1)
#define MAXHOPERRMASK_WR(src)                    (((u32)(src)<<1) & 0x00000002)
#define MAXHOPERRMASK_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*    Mask Register Fields parser_halt_statusMask    */
#define PARSER_HALT_STATUSMASK_WIDTH                                          1
#define PARSER_HALT_STATUSMASK_SHIFT                                          0
#define PARSER_HALT_STATUSMASK_MASK                                  0x00000001
#define PARSER_HALT_STATUSMASK_RD(src)                   (((src) & 0x00000001))
#define PARSER_HALT_STATUSMASK_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register parser_status1	*/ 
/*	 Fields PktRamErr	 */
#define PKTRAMERR1_WIDTH                                                      1
#define PKTRAMERR1_SHIFT                                                      4
#define PKTRAMERR1_MASK                                              0x00000010
#define PKTRAMERR1_RD(src)                            (((src) & 0x00000010)>>4)
#define PKTRAMERR1_WR(src)                       (((u32)(src)<<4) & 0x00000010)
#define PKTRAMERR1_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields PTRamErr	 */
#define PTRAMERR1_WIDTH                                                       1
#define PTRAMERR1_SHIFT                                                       3
#define PTRAMERR1_MASK                                               0x00000008
#define PTRAMERR1_RD(src)                             (((src) & 0x00000008)>>3)
#define PTRAMERR1_WR(src)                        (((u32)(src)<<3) & 0x00000008)
#define PTRAMERR1_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields PktOffsetExEop	 */
#define PKTOFFSETEXEOP1_WIDTH                                                 1
#define PKTOFFSETEXEOP1_SHIFT                                                 2
#define PKTOFFSETEXEOP1_MASK                                         0x00000004
#define PKTOFFSETEXEOP1_RD(src)                       (((src) & 0x00000004)>>2)
#define PKTOFFSETEXEOP1_WR(src)                  (((u32)(src)<<2) & 0x00000004)
#define PKTOFFSETEXEOP1_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields maxhoperr	 */
#define MAXHOPERR1_WIDTH                                                      1
#define MAXHOPERR1_SHIFT                                                      1
#define MAXHOPERR1_MASK                                              0x00000002
#define MAXHOPERR1_RD(src)                            (((src) & 0x00000002)>>1)
#define MAXHOPERR1_WR(src)                       (((u32)(src)<<1) & 0x00000002)
#define MAXHOPERR1_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields parser_halt_status	 */
#define PARSER_HALT_STATUS1_WIDTH                                             1
#define PARSER_HALT_STATUS1_SHIFT                                             0
#define PARSER_HALT_STATUS1_MASK                                     0x00000001
#define PARSER_HALT_STATUS1_RD(src)                      (((src) & 0x00000001))
#define PARSER_HALT_STATUS1_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register parser_status1Mask	*/
/*    Mask Register Fields PktRamErrMask    */
#define PKTRAMERRMASK_F1_WIDTH                                                1
#define PKTRAMERRMASK_F1_SHIFT                                                4
#define PKTRAMERRMASK_F1_MASK                                        0x00000010
#define PKTRAMERRMASK_F1_RD(src)                      (((src) & 0x00000010)>>4)
#define PKTRAMERRMASK_F1_WR(src)                 (((u32)(src)<<4) & 0x00000010)
#define PKTRAMERRMASK_F1_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*    Mask Register Fields PTRamErrMask    */
#define PTRAMERRMASK_F1_WIDTH                                                 1
#define PTRAMERRMASK_F1_SHIFT                                                 3
#define PTRAMERRMASK_F1_MASK                                         0x00000008
#define PTRAMERRMASK_F1_RD(src)                       (((src) & 0x00000008)>>3)
#define PTRAMERRMASK_F1_WR(src)                  (((u32)(src)<<3) & 0x00000008)
#define PTRAMERRMASK_F1_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*    Mask Register Fields PktOffsetExEopMask    */
#define PKTOFFSETEXEOPMASK_F1_WIDTH                                           1
#define PKTOFFSETEXEOPMASK_F1_SHIFT                                           2
#define PKTOFFSETEXEOPMASK_F1_MASK                                   0x00000004
#define PKTOFFSETEXEOPMASK_F1_RD(src)                 (((src) & 0x00000004)>>2)
#define PKTOFFSETEXEOPMASK_F1_WR(src)            (((u32)(src)<<2) & 0x00000004)
#define PKTOFFSETEXEOPMASK_F1_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*    Mask Register Fields maxhoperrMask    */
#define MAXHOPERRMASK_F1_WIDTH                                                1
#define MAXHOPERRMASK_F1_SHIFT                                                1
#define MAXHOPERRMASK_F1_MASK                                        0x00000002
#define MAXHOPERRMASK_F1_RD(src)                      (((src) & 0x00000002)>>1)
#define MAXHOPERRMASK_F1_WR(src)                 (((u32)(src)<<1) & 0x00000002)
#define MAXHOPERRMASK_F1_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*    Mask Register Fields parser_halt_statusMask    */
#define PARSER_HALT_STATUSMASK_F1_WIDTH                                       1
#define PARSER_HALT_STATUSMASK_F1_SHIFT                                       0
#define PARSER_HALT_STATUSMASK_F1_MASK                               0x00000001
#define PARSER_HALT_STATUSMASK_F1_RD(src)                (((src) & 0x00000001))
#define PARSER_HALT_STATUSMASK_F1_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register rsif_ram_ctrl	*/ 
/*	 Fields rmeb	 */
#define RMEB_F6_WIDTH                                                         1
#define RMEB_F6_SHIFT                                                         5
#define RMEB_F6_MASK                                                 0x00000020
#define RMEB_F6_RD(src)                               (((src) & 0x00000020)>>5)
#define RMEB_F6_WR(src)                          (((u32)(src)<<5) & 0x00000020)
#define RMEB_F6_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields rmb	 */
#define RMB_F6_WIDTH                                                          2
#define RMB_F6_SHIFT                                                          3
#define RMB_F6_MASK                                                  0x00000018
#define RMB_F6_RD(src)                                (((src) & 0x00000018)>>3)
#define RMB_F6_WR(src)                           (((u32)(src)<<3) & 0x00000018)
#define RMB_F6_SET(dst,src) \
                       (((dst) & ~0x00000018) | (((u32)(src)<<3) & 0x00000018))
/*	 Fields rmea	 */
#define RMEA_F16_WIDTH                                                        1
#define RMEA_F16_SHIFT                                                        2
#define RMEA_F16_MASK                                                0x00000004
#define RMEA_F16_RD(src)                              (((src) & 0x00000004)>>2)
#define RMEA_F16_WR(src)                         (((u32)(src)<<2) & 0x00000004)
#define RMEA_F16_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields rma	 */
#define RMA_F16_WIDTH                                                         2
#define RMA_F16_SHIFT                                                         0
#define RMA_F16_MASK                                                 0x00000003
#define RMA_F16_RD(src)                                  (((src) & 0x00000003))
#define RMA_F16_WR(src)                             (((u32)(src)) & 0x00000003)
#define RMA_F16_SET(dst,src) \
                          (((dst) & ~0x00000003) | (((u32)(src)) & 0x00000003))

/*	Register db_ram_ctrl	*/ 
/*	 Fields rme	 */
#define RME_WIDTH                                                             1
#define RME_SHIFT                                                             2
#define RME_MASK                                                     0x00000004
#define RME_RD(src)                                   (((src) & 0x00000004)>>2)
#define RME_WR(src)                              (((u32)(src)<<2) & 0x00000004)
#define RME_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields rm	 */
#define RM_WIDTH                                                              2
#define RM_SHIFT                                                              0
#define RM_MASK                                                      0x00000003
#define RM_RD(src)                                       (((src) & 0x00000003))
#define RM_WR(src)                                  (((u32)(src)) & 0x00000003)
#define RM_SET(dst,src) (((dst) & ~0x00000003) | (((u32)(src)) & 0x00000003))

/*	Register avl_ram3_ctrl	*/ 
/*	 Fields rme	 */
#define RME_F1_WIDTH                                                          1
#define RME_F1_SHIFT                                                          2
#define RME_F1_MASK                                                  0x00000004
#define RME_F1_RD(src)                                (((src) & 0x00000004)>>2)
#define RME_F1_WR(src)                           (((u32)(src)<<2) & 0x00000004)
#define RME_F1_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields rm	 */
#define RM_F1_WIDTH                                                           2
#define RM_F1_SHIFT                                                           0
#define RM_F1_MASK                                                   0x00000003
#define RM_F1_RD(src)                                    (((src) & 0x00000003))
#define RM_F1_WR(src)                               (((u32)(src)) & 0x00000003)
#define RM_F1_SET(dst,src) \
                          (((dst) & ~0x00000003) | (((u32)(src)) & 0x00000003))

/*	Register avl_ram2_ctrl	*/ 
/*	 Fields rme	 */
#define RME_F2_WIDTH                                                          1
#define RME_F2_SHIFT                                                          2
#define RME_F2_MASK                                                  0x00000004
#define RME_F2_RD(src)                                (((src) & 0x00000004)>>2)
#define RME_F2_WR(src)                           (((u32)(src)<<2) & 0x00000004)
#define RME_F2_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields rm	 */
#define RM_F2_WIDTH                                                           2
#define RM_F2_SHIFT                                                           0
#define RM_F2_MASK                                                   0x00000003
#define RM_F2_RD(src)                                    (((src) & 0x00000003))
#define RM_F2_WR(src)                               (((u32)(src)) & 0x00000003)
#define RM_F2_SET(dst,src) \
                          (((dst) & ~0x00000003) | (((u32)(src)) & 0x00000003))

/*	Register avl_ram1_ctrl	*/ 
/*	 Fields rme	 */
#define RME_F3_WIDTH                                                          1
#define RME_F3_SHIFT                                                          2
#define RME_F3_MASK                                                  0x00000004
#define RME_F3_RD(src)                                (((src) & 0x00000004)>>2)
#define RME_F3_WR(src)                           (((u32)(src)<<2) & 0x00000004)
#define RME_F3_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields rm	 */
#define RM_F3_WIDTH                                                           2
#define RM_F3_SHIFT                                                           0
#define RM_F3_MASK                                                   0x00000003
#define RM_F3_RD(src)                                    (((src) & 0x00000003))
#define RM_F3_WR(src)                               (((u32)(src)) & 0x00000003)
#define RM_F3_SET(dst,src) \
                          (((dst) & ~0x00000003) | (((u32)(src)) & 0x00000003))

/*	Register avl_ram0_ctrl	*/ 
/*	 Fields rme	 */
#define RME_F4_WIDTH                                                          1
#define RME_F4_SHIFT                                                          2
#define RME_F4_MASK                                                  0x00000004
#define RME_F4_RD(src)                                (((src) & 0x00000004)>>2)
#define RME_F4_WR(src)                           (((u32)(src)<<2) & 0x00000004)
#define RME_F4_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields rm	 */
#define RM_F4_WIDTH                                                           2
#define RM_F4_SHIFT                                                           0
#define RM_F4_MASK                                                   0x00000003
#define RM_F4_RD(src)                                    (((src) & 0x00000003))
#define RM_F4_WR(src)                               (((u32)(src)) & 0x00000003)
#define RM_F4_SET(dst,src) \
                          (((dst) & ~0x00000003) | (((u32)(src)) & 0x00000003))

/*	Register pt_ram3_ctrl	*/ 
/*	 Fields rme	 */
#define RME_F5_WIDTH                                                          1
#define RME_F5_SHIFT                                                          2
#define RME_F5_MASK                                                  0x00000004
#define RME_F5_RD(src)                                (((src) & 0x00000004)>>2)
#define RME_F5_WR(src)                           (((u32)(src)<<2) & 0x00000004)
#define RME_F5_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields rm	 */
#define RM_F5_WIDTH                                                           2
#define RM_F5_SHIFT                                                           0
#define RM_F5_MASK                                                   0x00000003
#define RM_F5_RD(src)                                    (((src) & 0x00000003))
#define RM_F5_WR(src)                               (((u32)(src)) & 0x00000003)
#define RM_F5_SET(dst,src) \
                          (((dst) & ~0x00000003) | (((u32)(src)) & 0x00000003))

/*	Register pt_ram2_ctrl	*/ 
/*	 Fields rme	 */
#define RME_F6_WIDTH                                                          1
#define RME_F6_SHIFT                                                          2
#define RME_F6_MASK                                                  0x00000004
#define RME_F6_RD(src)                                (((src) & 0x00000004)>>2)
#define RME_F6_WR(src)                           (((u32)(src)<<2) & 0x00000004)
#define RME_F6_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields rm	 */
#define RM_F6_WIDTH                                                           2
#define RM_F6_SHIFT                                                           0
#define RM_F6_MASK                                                   0x00000003
#define RM_F6_RD(src)                                    (((src) & 0x00000003))
#define RM_F6_WR(src)                               (((u32)(src)) & 0x00000003)
#define RM_F6_SET(dst,src) \
                          (((dst) & ~0x00000003) | (((u32)(src)) & 0x00000003))

/*	Register pt_ram1_ctrl	*/ 
/*	 Fields rme	 */
#define RME_F7_WIDTH                                                          1
#define RME_F7_SHIFT                                                          2
#define RME_F7_MASK                                                  0x00000004
#define RME_F7_RD(src)                                (((src) & 0x00000004)>>2)
#define RME_F7_WR(src)                           (((u32)(src)<<2) & 0x00000004)
#define RME_F7_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields rm	 */
#define RM_F7_WIDTH                                                           2
#define RM_F7_SHIFT                                                           0
#define RM_F7_MASK                                                   0x00000003
#define RM_F7_RD(src)                                    (((src) & 0x00000003))
#define RM_F7_WR(src)                               (((u32)(src)) & 0x00000003)
#define RM_F7_SET(dst,src) \
                          (((dst) & ~0x00000003) | (((u32)(src)) & 0x00000003))

/*	Register pt_ram0_ctrl	*/ 
/*	 Fields rme	 */
#define RME_F8_WIDTH                                                          1
#define RME_F8_SHIFT                                                          2
#define RME_F8_MASK                                                  0x00000004
#define RME_F8_RD(src)                                (((src) & 0x00000004)>>2)
#define RME_F8_WR(src)                           (((u32)(src)<<2) & 0x00000004)
#define RME_F8_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields rm	 */
#define RM_F8_WIDTH                                                           2
#define RM_F8_SHIFT                                                           0
#define RM_F8_MASK                                                   0x00000003
#define RM_F8_RD(src)                                    (((src) & 0x00000003))
#define RM_F8_WR(src)                               (((u32)(src)) & 0x00000003)
#define RM_F8_SET(dst,src) \
                          (((dst) & ~0x00000003) | (((u32)(src)) & 0x00000003))

/*	Register avl_search_int	*/ 
/*	 Fields fatal_wrselfpointerr	 */
#define FATAL_WRSELFPOINTERR_WIDTH                                            1
#define FATAL_WRSELFPOINTERR_SHIFT                                           10
#define FATAL_WRSELFPOINTERR_MASK                                    0x00000400
#define FATAL_WRSELFPOINTERR_RD(src)                 (((src) & 0x00000400)>>10)
#define FATAL_WRSELFPOINTERR_WR(src)            (((u32)(src)<<10) & 0x00000400)
#define FATAL_WRSELFPOINTERR_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields fatal_rdselfpointerr	 */
#define FATAL_RDSELFPOINTERR_WIDTH                                            1
#define FATAL_RDSELFPOINTERR_SHIFT                                            9
#define FATAL_RDSELFPOINTERR_MASK                                    0x00000200
#define FATAL_RDSELFPOINTERR_RD(src)                  (((src) & 0x00000200)>>9)
#define FATAL_RDSELFPOINTERR_WR(src)             (((u32)(src)<<9) & 0x00000200)
#define FATAL_RDSELFPOINTERR_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields fatal_wrbalerr	 */
#define FATAL_WRBALERR_WIDTH                                                  1
#define FATAL_WRBALERR_SHIFT                                                  8
#define FATAL_WRBALERR_MASK                                          0x00000100
#define FATAL_WRBALERR_RD(src)                        (((src) & 0x00000100)>>8)
#define FATAL_WRBALERR_WR(src)                   (((u32)(src)<<8) & 0x00000100)
#define FATAL_WRBALERR_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*	 Fields fatal_rdbalerr	 */
#define FATAL_RDBALERR_WIDTH                                                  1
#define FATAL_RDBALERR_SHIFT                                                  7
#define FATAL_RDBALERR_MASK                                          0x00000080
#define FATAL_RDBALERR_RD(src)                        (((src) & 0x00000080)>>7)
#define FATAL_RDBALERR_WR(src)                   (((u32)(src)<<7) & 0x00000080)
#define FATAL_RDBALERR_SET(dst,src) \
                       (((dst) & ~0x00000080) | (((u32)(src)<<7) & 0x00000080))
/*	 Fields fatal_maxstepserr	 */
#define FATAL_MAXSTEPSERR_WIDTH                                               1
#define FATAL_MAXSTEPSERR_SHIFT                                               6
#define FATAL_MAXSTEPSERR_MASK                                       0x00000040
#define FATAL_MAXSTEPSERR_RD(src)                     (((src) & 0x00000040)>>6)
#define FATAL_MAXSTEPSERR_WR(src)                (((u32)(src)<<6) & 0x00000040)
#define FATAL_MAXSTEPSERR_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*	 Fields fatal_rderr_ram	 */
#define FATAL_RDERR_RAM_WIDTH                                                 1
#define FATAL_RDERR_RAM_SHIFT                                                 5
#define FATAL_RDERR_RAM_MASK                                         0x00000020
#define FATAL_RDERR_RAM_RD(src)                       (((src) & 0x00000020)>>5)
#define FATAL_RDERR_RAM_WR(src)                  (((u32)(src)<<5) & 0x00000020)
#define FATAL_RDERR_RAM_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields rderr_ram	 */
#define RDERR_RAM_WIDTH                                                       1
#define RDERR_RAM_SHIFT                                                       4
#define RDERR_RAM_MASK                                               0x00000010
#define RDERR_RAM_RD(src)                             (((src) & 0x00000010)>>4)
#define RDERR_RAM_WR(src)                        (((u32)(src)<<4) & 0x00000010)
#define RDERR_RAM_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields delerr_ridnotfound	 */
#define DELERR_RIDNOTFOUND_WIDTH                                              1
#define DELERR_RIDNOTFOUND_SHIFT                                              3
#define DELERR_RIDNOTFOUND_MASK                                      0x00000008
#define DELERR_RIDNOTFOUND_RD(src)                    (((src) & 0x00000008)>>3)
#define DELERR_RIDNOTFOUND_WR(src)               (((u32)(src)<<3) & 0x00000008)
#define DELERR_RIDNOTFOUND_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields delerr_empty	 */
#define DELERR_EMPTY_WIDTH                                                    1
#define DELERR_EMPTY_SHIFT                                                    2
#define DELERR_EMPTY_MASK                                            0x00000004
#define DELERR_EMPTY_RD(src)                          (((src) & 0x00000004)>>2)
#define DELERR_EMPTY_WR(src)                     (((u32)(src)<<2) & 0x00000004)
#define DELERR_EMPTY_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields adderr_ridduplicate	 */
#define ADDERR_RIDDUPLICATE_WIDTH                                             1
#define ADDERR_RIDDUPLICATE_SHIFT                                             1
#define ADDERR_RIDDUPLICATE_MASK                                     0x00000002
#define ADDERR_RIDDUPLICATE_RD(src)                   (((src) & 0x00000002)>>1)
#define ADDERR_RIDDUPLICATE_WR(src)              (((u32)(src)<<1) & 0x00000002)
#define ADDERR_RIDDUPLICATE_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields adderr_full	 */
#define ADDERR_FULL_WIDTH                                                     1
#define ADDERR_FULL_SHIFT                                                     0
#define ADDERR_FULL_MASK                                             0x00000001
#define ADDERR_FULL_RD(src)                              (((src) & 0x00000001))
#define ADDERR_FULL_WR(src)                         (((u32)(src)) & 0x00000001)
#define ADDERR_FULL_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register avl_search_intMask	*/
/*    Mask Register Fields fatal_wrselfpointerrMask    */
#define FATAL_WRSELFPOINTERRMASK_WIDTH                                        1
#define FATAL_WRSELFPOINTERRMASK_SHIFT                                       10
#define FATAL_WRSELFPOINTERRMASK_MASK                                0x00000400
#define FATAL_WRSELFPOINTERRMASK_RD(src)             (((src) & 0x00000400)>>10)
#define FATAL_WRSELFPOINTERRMASK_WR(src)        (((u32)(src)<<10) & 0x00000400)
#define FATAL_WRSELFPOINTERRMASK_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*    Mask Register Fields fatal_rdselfpointerrMask    */
#define FATAL_RDSELFPOINTERRMASK_WIDTH                                        1
#define FATAL_RDSELFPOINTERRMASK_SHIFT                                        9
#define FATAL_RDSELFPOINTERRMASK_MASK                                0x00000200
#define FATAL_RDSELFPOINTERRMASK_RD(src)              (((src) & 0x00000200)>>9)
#define FATAL_RDSELFPOINTERRMASK_WR(src)         (((u32)(src)<<9) & 0x00000200)
#define FATAL_RDSELFPOINTERRMASK_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*    Mask Register Fields fatal_wrbalerrMask    */
#define FATAL_WRBALERRMASK_WIDTH                                              1
#define FATAL_WRBALERRMASK_SHIFT                                              8
#define FATAL_WRBALERRMASK_MASK                                      0x00000100
#define FATAL_WRBALERRMASK_RD(src)                    (((src) & 0x00000100)>>8)
#define FATAL_WRBALERRMASK_WR(src)               (((u32)(src)<<8) & 0x00000100)
#define FATAL_WRBALERRMASK_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*    Mask Register Fields fatal_rdbalerrMask    */
#define FATAL_RDBALERRMASK_WIDTH                                              1
#define FATAL_RDBALERRMASK_SHIFT                                              7
#define FATAL_RDBALERRMASK_MASK                                      0x00000080
#define FATAL_RDBALERRMASK_RD(src)                    (((src) & 0x00000080)>>7)
#define FATAL_RDBALERRMASK_WR(src)               (((u32)(src)<<7) & 0x00000080)
#define FATAL_RDBALERRMASK_SET(dst,src) \
                       (((dst) & ~0x00000080) | (((u32)(src)<<7) & 0x00000080))
/*    Mask Register Fields fatal_maxstepserrMask    */
#define FATAL_MAXSTEPSERRMASK_WIDTH                                           1
#define FATAL_MAXSTEPSERRMASK_SHIFT                                           6
#define FATAL_MAXSTEPSERRMASK_MASK                                   0x00000040
#define FATAL_MAXSTEPSERRMASK_RD(src)                 (((src) & 0x00000040)>>6)
#define FATAL_MAXSTEPSERRMASK_WR(src)            (((u32)(src)<<6) & 0x00000040)
#define FATAL_MAXSTEPSERRMASK_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*    Mask Register Fields fatal_rderr_ramMask    */
#define FATAL_RDERR_RAMMASK_WIDTH                                             1
#define FATAL_RDERR_RAMMASK_SHIFT                                             5
#define FATAL_RDERR_RAMMASK_MASK                                     0x00000020
#define FATAL_RDERR_RAMMASK_RD(src)                   (((src) & 0x00000020)>>5)
#define FATAL_RDERR_RAMMASK_WR(src)              (((u32)(src)<<5) & 0x00000020)
#define FATAL_RDERR_RAMMASK_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*    Mask Register Fields rderr_ramMask    */
#define RDERR_RAMMASK_WIDTH                                                   1
#define RDERR_RAMMASK_SHIFT                                                   4
#define RDERR_RAMMASK_MASK                                           0x00000010
#define RDERR_RAMMASK_RD(src)                         (((src) & 0x00000010)>>4)
#define RDERR_RAMMASK_WR(src)                    (((u32)(src)<<4) & 0x00000010)
#define RDERR_RAMMASK_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*    Mask Register Fields delerr_ridnotfoundMask    */
#define DELERR_RIDNOTFOUNDMASK_WIDTH                                          1
#define DELERR_RIDNOTFOUNDMASK_SHIFT                                          3
#define DELERR_RIDNOTFOUNDMASK_MASK                                  0x00000008
#define DELERR_RIDNOTFOUNDMASK_RD(src)                (((src) & 0x00000008)>>3)
#define DELERR_RIDNOTFOUNDMASK_WR(src)           (((u32)(src)<<3) & 0x00000008)
#define DELERR_RIDNOTFOUNDMASK_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*    Mask Register Fields delerr_emptyMask    */
#define DELERR_EMPTYMASK_WIDTH                                                1
#define DELERR_EMPTYMASK_SHIFT                                                2
#define DELERR_EMPTYMASK_MASK                                        0x00000004
#define DELERR_EMPTYMASK_RD(src)                      (((src) & 0x00000004)>>2)
#define DELERR_EMPTYMASK_WR(src)                 (((u32)(src)<<2) & 0x00000004)
#define DELERR_EMPTYMASK_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*    Mask Register Fields adderr_ridduplicateMask    */
#define ADDERR_RIDDUPLICATEMASK_WIDTH                                         1
#define ADDERR_RIDDUPLICATEMASK_SHIFT                                         1
#define ADDERR_RIDDUPLICATEMASK_MASK                                 0x00000002
#define ADDERR_RIDDUPLICATEMASK_RD(src)               (((src) & 0x00000002)>>1)
#define ADDERR_RIDDUPLICATEMASK_WR(src)          (((u32)(src)<<1) & 0x00000002)
#define ADDERR_RIDDUPLICATEMASK_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*    Mask Register Fields adderr_fullMask    */
#define ADDERR_FULLMASK_WIDTH                                                 1
#define ADDERR_FULLMASK_SHIFT                                                 0
#define ADDERR_FULLMASK_MASK                                         0x00000001
#define ADDERR_FULLMASK_RD(src)                          (((src) & 0x00000001))
#define ADDERR_FULLMASK_WR(src)                     (((u32)(src)) & 0x00000001)
#define ADDERR_FULLMASK_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register cle_INT	*/ 
/*	 Fields cle_int	 */
#define CLE_INT_WIDTH                                                         1
#define CLE_INT_SHIFT                                                         0
#define CLE_INT_MASK                                                 0x00000001
#define CLE_INT_RD(src)                                  (((src) & 0x00000001))
#define CLE_INT_WR(src)                             (((u32)(src)) & 0x00000001)
#define CLE_INT_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register cle_interruptMask	*/
/*    Mask Register Fields cle_intMask    */
#define CLE_INTMASK_WIDTH                                                     1
#define CLE_INTMASK_SHIFT                                                     0
#define CLE_INTMASK_MASK                                             0x00000001
#define CLE_INTMASK_RD(src)                              (((src) & 0x00000001))
#define CLE_INTMASK_WR(src)                         (((u32)(src)) & 0x00000001)
#define CLE_INTMASK_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register lstnvst0	*/ 
/*	 Fields lstnvst	 */
#define LSTNVST0_WIDTH                                                       12
#define LSTNVST0_SHIFT                                                        0
#define LSTNVST0_MASK                                                0x00000fff
#define LSTNVST0_RD(src)                                 (((src) & 0x00000fff))
#define LSTNVST0_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lsttrcnvst0_0	*/ 
/*	 Fields ltrcnvst	 */
#define LTRCNVST0_WIDTH                                                      12
#define LTRCNVST0_SHIFT                                                       0
#define LTRCNVST0_MASK                                               0x00000fff
#define LTRCNVST0_RD(src)                                (((src) & 0x00000fff))
#define LTRCNVST0_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lsttrcnvst0_1	*/ 
/*	 Fields ltrcnvst	 */
#define LTRCNVST1_WIDTH                                                      12
#define LTRCNVST1_SHIFT                                                       0
#define LTRCNVST1_MASK                                               0x00000fff
#define LTRCNVST1_RD(src)                                (((src) & 0x00000fff))
#define LTRCNVST1_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lsttrcnvst0_2	*/ 
/*	 Fields ltrcnvst	 */
#define LTRCNVST2_WIDTH                                                      12
#define LTRCNVST2_SHIFT                                                       0
#define LTRCNVST2_MASK                                               0x00000fff
#define LTRCNVST2_RD(src)                                (((src) & 0x00000fff))
#define LTRCNVST2_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lsttrcnvst0_3	*/ 
/*	 Fields ltrcnvst	 */
#define LTRCNVST3_WIDTH                                                      12
#define LTRCNVST3_SHIFT                                                       0
#define LTRCNVST3_MASK                                               0x00000fff
#define LTRCNVST3_RD(src)                                (((src) & 0x00000fff))
#define LTRCNVST3_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lsttrcnvst0_4	*/ 
/*	 Fields ltrcnvst	 */
#define LTRCNVST4_WIDTH                                                      12
#define LTRCNVST4_SHIFT                                                       0
#define LTRCNVST4_MASK                                               0x00000fff
#define LTRCNVST4_RD(src)                                (((src) & 0x00000fff))
#define LTRCNVST4_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lsttrcnvst0_5	*/ 
/*	 Fields ltrcnvst	 */
#define LTRCNVST5_WIDTH                                                      12
#define LTRCNVST5_SHIFT                                                       0
#define LTRCNVST5_MASK                                               0x00000fff
#define LTRCNVST5_RD(src)                                (((src) & 0x00000fff))
#define LTRCNVST5_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lsttrcnvst0_6	*/ 
/*	 Fields ltrcnvst	 */
#define LTRCNVST6_WIDTH                                                      12
#define LTRCNVST6_SHIFT                                                       0
#define LTRCNVST6_MASK                                               0x00000fff
#define LTRCNVST6_RD(src)                                (((src) & 0x00000fff))
#define LTRCNVST6_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lsttrcnvst0_7	*/ 
/*	 Fields ltrcnvst	 */
#define LTRCNVST7_WIDTH                                                      12
#define LTRCNVST7_SHIFT                                                       0
#define LTRCNVST7_MASK                                               0x00000fff
#define LTRCNVST7_RD(src)                                (((src) & 0x00000fff))
#define LTRCNVST7_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lsttrcnvst0_8	*/ 
/*	 Fields ltrcnvst	 */
#define LTRCNVST8_WIDTH                                                      12
#define LTRCNVST8_SHIFT                                                       0
#define LTRCNVST8_MASK                                               0x00000fff
#define LTRCNVST8_RD(src)                                (((src) & 0x00000fff))
#define LTRCNVST8_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lsttrcnvst0_9	*/ 
/*	 Fields ltrcnvst	 */
#define LTRCNVST9_WIDTH                                                      12
#define LTRCNVST9_SHIFT                                                       0
#define LTRCNVST9_MASK                                               0x00000fff
#define LTRCNVST9_RD(src)                                (((src) & 0x00000fff))
#define LTRCNVST9_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lsttrcnvst0_10	*/ 
/*	 Fields ltrcnvst	 */
#define LTRCNVST0_F1_WIDTH                                                   12
#define LTRCNVST0_F1_SHIFT                                                    0
#define LTRCNVST0_F1_MASK                                            0x00000fff
#define LTRCNVST0_F1_RD(src)                             (((src) & 0x00000fff))
#define LTRCNVST0_F1_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lsttrcnvst0_11	*/ 
/*	 Fields ltrcnvst	 */
#define LTRCNVST1_F1_WIDTH                                                   12
#define LTRCNVST1_F1_SHIFT                                                    0
#define LTRCNVST1_F1_MASK                                            0x00000fff
#define LTRCNVST1_F1_RD(src)                             (((src) & 0x00000fff))
#define LTRCNVST1_F1_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lsttrcnvst0_12	*/ 
/*	 Fields ltrcnvst	 */
#define LTRCNVST2_F1_WIDTH                                                   12
#define LTRCNVST2_F1_SHIFT                                                    0
#define LTRCNVST2_F1_MASK                                            0x00000fff
#define LTRCNVST2_F1_RD(src)                             (((src) & 0x00000fff))
#define LTRCNVST2_F1_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lsttrcnvst0_13	*/ 
/*	 Fields ltrcnvst	 */
#define LTRCNVST3_F1_WIDTH                                                   12
#define LTRCNVST3_F1_SHIFT                                                    0
#define LTRCNVST3_F1_MASK                                            0x00000fff
#define LTRCNVST3_F1_RD(src)                             (((src) & 0x00000fff))
#define LTRCNVST3_F1_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lsttrcnvst0_14	*/ 
/*	 Fields ltrcnvst	 */
#define LTRCNVST4_F1_WIDTH                                                   12
#define LTRCNVST4_F1_SHIFT                                                    0
#define LTRCNVST4_F1_MASK                                            0x00000fff
#define LTRCNVST4_F1_RD(src)                             (((src) & 0x00000fff))
#define LTRCNVST4_F1_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lsttrcnvst0_15	*/ 
/*	 Fields ltrcnvst	 */
#define LTRCNVST5_F1_WIDTH                                                   12
#define LTRCNVST5_F1_SHIFT                                                    0
#define LTRCNVST5_F1_MASK                                            0x00000fff
#define LTRCNVST5_F1_RD(src)                             (((src) & 0x00000fff))
#define LTRCNVST5_F1_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register ftrcnvststr0_0	*/ 
/*	 Fields ftrcnvststrt	 */
#define FTRCNVSTSTRT0_WIDTH                                                  12
#define FTRCNVSTSTRT0_SHIFT                                                   0
#define FTRCNVSTSTRT0_MASK                                           0x00000fff
#define FTRCNVSTSTRT0_RD(src)                            (((src) & 0x00000fff))
#define FTRCNVSTSTRT0_WR(src)                       (((u32)(src)) & 0x00000fff)
#define FTRCNVSTSTRT0_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register ftrcnvst0_0	*/ 
/*	 Fields ftrcnvst	 */
#define FTRCNVST0_WIDTH                                                      12
#define FTRCNVST0_SHIFT                                                       0
#define FTRCNVST0_MASK                                               0x00000fff
#define FTRCNVST0_RD(src)                                (((src) & 0x00000fff))
#define FTRCNVST0_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register ftrcnvst0_1	*/ 
/*	 Fields ftrcnvst	 */
#define FTRCNVST1_WIDTH                                                      12
#define FTRCNVST1_SHIFT                                                       0
#define FTRCNVST1_MASK                                               0x00000fff
#define FTRCNVST1_RD(src)                                (((src) & 0x00000fff))
#define FTRCNVST1_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register ftrcnvst0_2	*/ 
/*	 Fields ftrcnvst	 */
#define FTRCNVST2_WIDTH                                                      12
#define FTRCNVST2_SHIFT                                                       0
#define FTRCNVST2_MASK                                               0x00000fff
#define FTRCNVST2_RD(src)                                (((src) & 0x00000fff))
#define FTRCNVST2_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register ftrcnvst0_3	*/ 
/*	 Fields ftrcnvst	 */
#define FTRCNVST3_WIDTH                                                      12
#define FTRCNVST3_SHIFT                                                       0
#define FTRCNVST3_MASK                                               0x00000fff
#define FTRCNVST3_RD(src)                                (((src) & 0x00000fff))
#define FTRCNVST3_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register ftrcnvst0_4	*/ 
/*	 Fields ftrcnvst	 */
#define FTRCNVST4_WIDTH                                                      12
#define FTRCNVST4_SHIFT                                                       0
#define FTRCNVST4_MASK                                               0x00000fff
#define FTRCNVST4_RD(src)                                (((src) & 0x00000fff))
#define FTRCNVST4_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register ftrcnvst0_5	*/ 
/*	 Fields ftrcnvst	 */
#define FTRCNVST5_WIDTH                                                      12
#define FTRCNVST5_SHIFT                                                       0
#define FTRCNVST5_MASK                                               0x00000fff
#define FTRCNVST5_RD(src)                                (((src) & 0x00000fff))
#define FTRCNVST5_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register ftrcnvst0_6	*/ 
/*	 Fields ftrcnvst	 */
#define FTRCNVST6_WIDTH                                                      12
#define FTRCNVST6_SHIFT                                                       0
#define FTRCNVST6_MASK                                               0x00000fff
#define FTRCNVST6_RD(src)                                (((src) & 0x00000fff))
#define FTRCNVST6_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register ftrcnvst0_7	*/ 
/*	 Fields ftrcnvst	 */
#define FTRCNVST7_WIDTH                                                      12
#define FTRCNVST7_SHIFT                                                       0
#define FTRCNVST7_MASK                                               0x00000fff
#define FTRCNVST7_RD(src)                                (((src) & 0x00000fff))
#define FTRCNVST7_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register ftrcnvst0_8	*/ 
/*	 Fields ftrcnvst	 */
#define FTRCNVST8_WIDTH                                                      12
#define FTRCNVST8_SHIFT                                                       0
#define FTRCNVST8_MASK                                               0x00000fff
#define FTRCNVST8_RD(src)                                (((src) & 0x00000fff))
#define FTRCNVST8_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register ftrcnvst0_9	*/ 
/*	 Fields ftrcnvst	 */
#define FTRCNVST9_WIDTH                                                      12
#define FTRCNVST9_SHIFT                                                       0
#define FTRCNVST9_MASK                                               0x00000fff
#define FTRCNVST9_RD(src)                                (((src) & 0x00000fff))
#define FTRCNVST9_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register ftrcnvst0_10	*/ 
/*	 Fields ftrcnvst	 */
#define FTRCNVST0_F1_WIDTH                                                   12
#define FTRCNVST0_F1_SHIFT                                                    0
#define FTRCNVST0_F1_MASK                                            0x00000fff
#define FTRCNVST0_F1_RD(src)                             (((src) & 0x00000fff))
#define FTRCNVST0_F1_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register ftrcnvst0_11	*/ 
/*	 Fields ftrcnvst	 */
#define FTRCNVST1_F1_WIDTH                                                   12
#define FTRCNVST1_F1_SHIFT                                                    0
#define FTRCNVST1_F1_MASK                                            0x00000fff
#define FTRCNVST1_F1_RD(src)                             (((src) & 0x00000fff))
#define FTRCNVST1_F1_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register ftrcnvst0_12	*/ 
/*	 Fields ftrcnvst	 */
#define FTRCNVST2_F1_WIDTH                                                   12
#define FTRCNVST2_F1_SHIFT                                                    0
#define FTRCNVST2_F1_MASK                                            0x00000fff
#define FTRCNVST2_F1_RD(src)                             (((src) & 0x00000fff))
#define FTRCNVST2_F1_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register ftrcnvst0_13	*/ 
/*	 Fields ftrcnvst	 */
#define FTRCNVST3_F1_WIDTH                                                   12
#define FTRCNVST3_F1_SHIFT                                                    0
#define FTRCNVST3_F1_MASK                                            0x00000fff
#define FTRCNVST3_F1_RD(src)                             (((src) & 0x00000fff))
#define FTRCNVST3_F1_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register ftrcnvst0_14	*/ 
/*	 Fields ftrcnvst	 */
#define FTRCNVST4_F1_WIDTH                                                   12
#define FTRCNVST4_F1_SHIFT                                                    0
#define FTRCNVST4_F1_MASK                                            0x00000fff
#define FTRCNVST4_F1_RD(src)                             (((src) & 0x00000fff))
#define FTRCNVST4_F1_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register ftrcnvst0_15	*/ 
/*	 Fields ftrcnvst	 */
#define FTRCNVST5_F1_WIDTH                                                   12
#define FTRCNVST5_F1_SHIFT                                                    0
#define FTRCNVST5_F1_MASK                                            0x00000fff
#define FTRCNVST5_F1_RD(src)                             (((src) & 0x00000fff))
#define FTRCNVST5_F1_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register trcnvstmon0_0	*/ 
/*	 Fields trcnvstmon	 */
#define TRCNVSTMON0_WIDTH                                                    12
#define TRCNVSTMON0_SHIFT                                                     0
#define TRCNVSTMON0_MASK                                             0x00000fff
#define TRCNVSTMON0_RD(src)                              (((src) & 0x00000fff))
#define TRCNVSTMON0_WR(src)                         (((u32)(src)) & 0x00000fff)
#define TRCNVSTMON0_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register trcnvstmoncnt0_0	*/ 
/*	 Fields trcnvstmoncnt	 */
#define TRCNVSTMONCNT0_WIDTH                                                 32
#define TRCNVSTMONCNT0_SHIFT                                                  0
#define TRCNVSTMONCNT0_MASK                                          0xffffffff
#define TRCNVSTMONCNT0_RD(src)                           (((src) & 0xffffffff))
#define TRCNVSTMONCNT0_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register trcnvstmon0_1	*/ 
/*	 Fields trcnvstmon	 */
#define TRCNVSTMON1_WIDTH                                                    12
#define TRCNVSTMON1_SHIFT                                                     0
#define TRCNVSTMON1_MASK                                             0x00000fff
#define TRCNVSTMON1_RD(src)                              (((src) & 0x00000fff))
#define TRCNVSTMON1_WR(src)                         (((u32)(src)) & 0x00000fff)
#define TRCNVSTMON1_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register trcnvstmoncnt0_1	*/ 
/*	 Fields trcnvstmoncnt	 */
#define TRCNVSTMONCNT1_WIDTH                                                 32
#define TRCNVSTMONCNT1_SHIFT                                                  0
#define TRCNVSTMONCNT1_MASK                                          0xffffffff
#define TRCNVSTMONCNT1_RD(src)                           (((src) & 0xffffffff))
#define TRCNVSTMONCNT1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register trcnvstmon0_2	*/ 
/*	 Fields trcnvstmon	 */
#define TRCNVSTMON2_WIDTH                                                    12
#define TRCNVSTMON2_SHIFT                                                     0
#define TRCNVSTMON2_MASK                                             0x00000fff
#define TRCNVSTMON2_RD(src)                              (((src) & 0x00000fff))
#define TRCNVSTMON2_WR(src)                         (((u32)(src)) & 0x00000fff)
#define TRCNVSTMON2_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register trcnvstmoncnt0_2	*/ 
/*	 Fields trcnvstmoncnt	 */
#define TRCNVSTMONCNT2_WIDTH                                                 32
#define TRCNVSTMONCNT2_SHIFT                                                  0
#define TRCNVSTMONCNT2_MASK                                          0xffffffff
#define TRCNVSTMONCNT2_RD(src)                           (((src) & 0xffffffff))
#define TRCNVSTMONCNT2_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register trcnvstmon0_3	*/ 
/*	 Fields trcnvstmon	 */
#define TRCNVSTMON3_WIDTH                                                    12
#define TRCNVSTMON3_SHIFT                                                     0
#define TRCNVSTMON3_MASK                                             0x00000fff
#define TRCNVSTMON3_RD(src)                              (((src) & 0x00000fff))
#define TRCNVSTMON3_WR(src)                         (((u32)(src)) & 0x00000fff)
#define TRCNVSTMON3_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register trcnvstmoncnt0_3	*/ 
/*	 Fields trcnvstmoncnt	 */
#define TRCNVSTMONCNT3_WIDTH                                                 32
#define TRCNVSTMONCNT3_SHIFT                                                  0
#define TRCNVSTMONCNT3_MASK                                          0xffffffff
#define TRCNVSTMONCNT3_RD(src)                           (((src) & 0xffffffff))
#define TRCNVSTMONCNT3_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register trcnvstmon0_4	*/ 
/*	 Fields trcnvstmon	 */
#define TRCNVSTMON4_WIDTH                                                    12
#define TRCNVSTMON4_SHIFT                                                     0
#define TRCNVSTMON4_MASK                                             0x00000fff
#define TRCNVSTMON4_RD(src)                              (((src) & 0x00000fff))
#define TRCNVSTMON4_WR(src)                         (((u32)(src)) & 0x00000fff)
#define TRCNVSTMON4_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register trcnvstmoncnt0_4	*/ 
/*	 Fields trcnvstmoncnt	 */
#define TRCNVSTMONCNT4_WIDTH                                                 32
#define TRCNVSTMONCNT4_SHIFT                                                  0
#define TRCNVSTMONCNT4_MASK                                          0xffffffff
#define TRCNVSTMONCNT4_RD(src)                           (((src) & 0xffffffff))
#define TRCNVSTMONCNT4_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register trcnvstmon0_5	*/ 
/*	 Fields trcnvstmon	 */
#define TRCNVSTMON5_WIDTH                                                    12
#define TRCNVSTMON5_SHIFT                                                     0
#define TRCNVSTMON5_MASK                                             0x00000fff
#define TRCNVSTMON5_RD(src)                              (((src) & 0x00000fff))
#define TRCNVSTMON5_WR(src)                         (((u32)(src)) & 0x00000fff)
#define TRCNVSTMON5_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register trcnvstmoncnt0_5	*/ 
/*	 Fields trcnvstmoncnt	 */
#define TRCNVSTMONCNT5_WIDTH                                                 32
#define TRCNVSTMONCNT5_SHIFT                                                  0
#define TRCNVSTMONCNT5_MASK                                          0xffffffff
#define TRCNVSTMONCNT5_RD(src)                           (((src) & 0xffffffff))
#define TRCNVSTMONCNT5_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register trcnvstmon0_6	*/ 
/*	 Fields trcnvstmon	 */
#define TRCNVSTMON6_WIDTH                                                    12
#define TRCNVSTMON6_SHIFT                                                     0
#define TRCNVSTMON6_MASK                                             0x00000fff
#define TRCNVSTMON6_RD(src)                              (((src) & 0x00000fff))
#define TRCNVSTMON6_WR(src)                         (((u32)(src)) & 0x00000fff)
#define TRCNVSTMON6_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register trcnvstmoncnt0_6	*/ 
/*	 Fields trcnvstmoncnt	 */
#define TRCNVSTMONCNT6_WIDTH                                                 32
#define TRCNVSTMONCNT6_SHIFT                                                  0
#define TRCNVSTMONCNT6_MASK                                          0xffffffff
#define TRCNVSTMONCNT6_RD(src)                           (((src) & 0xffffffff))
#define TRCNVSTMONCNT6_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register trcnvstmon0_7	*/ 
/*	 Fields trcnvstmon	 */
#define TRCNVSTMON7_WIDTH                                                    12
#define TRCNVSTMON7_SHIFT                                                     0
#define TRCNVSTMON7_MASK                                             0x00000fff
#define TRCNVSTMON7_RD(src)                              (((src) & 0x00000fff))
#define TRCNVSTMON7_WR(src)                         (((u32)(src)) & 0x00000fff)
#define TRCNVSTMON7_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register trcnvstmoncnt0_7	*/ 
/*	 Fields trcnvstmoncnt	 */
#define TRCNVSTMONCNT7_WIDTH                                                 32
#define TRCNVSTMONCNT7_SHIFT                                                  0
#define TRCNVSTMONCNT7_MASK                                          0xffffffff
#define TRCNVSTMONCNT7_RD(src)                           (((src) & 0xffffffff))
#define TRCNVSTMONCNT7_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register trcnvstmon0_8	*/ 
/*	 Fields trcnvstmon	 */
#define TRCNVSTMON8_WIDTH                                                    12
#define TRCNVSTMON8_SHIFT                                                     0
#define TRCNVSTMON8_MASK                                             0x00000fff
#define TRCNVSTMON8_RD(src)                              (((src) & 0x00000fff))
#define TRCNVSTMON8_WR(src)                         (((u32)(src)) & 0x00000fff)
#define TRCNVSTMON8_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register trcnvstmoncnt0_8	*/ 
/*	 Fields trcnvstmoncnt	 */
#define TRCNVSTMONCNT8_WIDTH                                                 32
#define TRCNVSTMONCNT8_SHIFT                                                  0
#define TRCNVSTMONCNT8_MASK                                          0xffffffff
#define TRCNVSTMONCNT8_RD(src)                           (((src) & 0xffffffff))
#define TRCNVSTMONCNT8_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register trcnvstmon0_9	*/ 
/*	 Fields trcnvstmon	 */
#define TRCNVSTMON9_WIDTH                                                    12
#define TRCNVSTMON9_SHIFT                                                     0
#define TRCNVSTMON9_MASK                                             0x00000fff
#define TRCNVSTMON9_RD(src)                              (((src) & 0x00000fff))
#define TRCNVSTMON9_WR(src)                         (((u32)(src)) & 0x00000fff)
#define TRCNVSTMON9_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register trcnvstmoncnt0_9	*/ 
/*	 Fields trcnvstmoncnt	 */
#define TRCNVSTMONCNT9_WIDTH                                                 32
#define TRCNVSTMONCNT9_SHIFT                                                  0
#define TRCNVSTMONCNT9_MASK                                          0xffffffff
#define TRCNVSTMONCNT9_RD(src)                           (((src) & 0xffffffff))
#define TRCNVSTMONCNT9_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register trcnvstmon0_10	*/ 
/*	 Fields trcnvstmon	 */
#define TRCNVSTMON0_F1_WIDTH                                                 12
#define TRCNVSTMON0_F1_SHIFT                                                  0
#define TRCNVSTMON0_F1_MASK                                          0x00000fff
#define TRCNVSTMON0_F1_RD(src)                           (((src) & 0x00000fff))
#define TRCNVSTMON0_F1_WR(src)                      (((u32)(src)) & 0x00000fff)
#define TRCNVSTMON0_F1_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register trcnvstmoncnt0_10	*/ 
/*	 Fields trcnvstmoncnt	 */
#define TRCNVSTMONCNT0_F1_WIDTH                                              32
#define TRCNVSTMONCNT0_F1_SHIFT                                               0
#define TRCNVSTMONCNT0_F1_MASK                                       0xffffffff
#define TRCNVSTMONCNT0_F1_RD(src)                        (((src) & 0xffffffff))
#define TRCNVSTMONCNT0_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register trcnvstmon0_11	*/ 
/*	 Fields trcnvstmon	 */
#define TRCNVSTMON1_F1_WIDTH                                                 12
#define TRCNVSTMON1_F1_SHIFT                                                  0
#define TRCNVSTMON1_F1_MASK                                          0x00000fff
#define TRCNVSTMON1_F1_RD(src)                           (((src) & 0x00000fff))
#define TRCNVSTMON1_F1_WR(src)                      (((u32)(src)) & 0x00000fff)
#define TRCNVSTMON1_F1_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register trcnvstmoncnt0_11	*/ 
/*	 Fields trcnvstmoncnt	 */
#define TRCNVSTMONCNT1_F1_WIDTH                                              32
#define TRCNVSTMONCNT1_F1_SHIFT                                               0
#define TRCNVSTMONCNT1_F1_MASK                                       0xffffffff
#define TRCNVSTMONCNT1_F1_RD(src)                        (((src) & 0xffffffff))
#define TRCNVSTMONCNT1_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register trcnvstmon0_12	*/ 
/*	 Fields trcnvstmon	 */
#define TRCNVSTMON2_F1_WIDTH                                                 12
#define TRCNVSTMON2_F1_SHIFT                                                  0
#define TRCNVSTMON2_F1_MASK                                          0x00000fff
#define TRCNVSTMON2_F1_RD(src)                           (((src) & 0x00000fff))
#define TRCNVSTMON2_F1_WR(src)                      (((u32)(src)) & 0x00000fff)
#define TRCNVSTMON2_F1_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register trcnvstmoncnt0_12	*/ 
/*	 Fields trcnvstmoncnt	 */
#define TRCNVSTMONCNT2_F1_WIDTH                                              32
#define TRCNVSTMONCNT2_F1_SHIFT                                               0
#define TRCNVSTMONCNT2_F1_MASK                                       0xffffffff
#define TRCNVSTMONCNT2_F1_RD(src)                        (((src) & 0xffffffff))
#define TRCNVSTMONCNT2_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register trcnvstmon0_13	*/ 
/*	 Fields trcnvstmon	 */
#define TRCNVSTMON3_F1_WIDTH                                                 12
#define TRCNVSTMON3_F1_SHIFT                                                  0
#define TRCNVSTMON3_F1_MASK                                          0x00000fff
#define TRCNVSTMON3_F1_RD(src)                           (((src) & 0x00000fff))
#define TRCNVSTMON3_F1_WR(src)                      (((u32)(src)) & 0x00000fff)
#define TRCNVSTMON3_F1_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register trcnvstmoncnt0_13	*/ 
/*	 Fields trcnvstmoncnt	 */
#define TRCNVSTMONCNT3_F1_WIDTH                                              32
#define TRCNVSTMONCNT3_F1_SHIFT                                               0
#define TRCNVSTMONCNT3_F1_MASK                                       0xffffffff
#define TRCNVSTMONCNT3_F1_RD(src)                        (((src) & 0xffffffff))
#define TRCNVSTMONCNT3_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register trcnvstmon0_14	*/ 
/*	 Fields trcnvstmon	 */
#define TRCNVSTMON4_F1_WIDTH                                                 12
#define TRCNVSTMON4_F1_SHIFT                                                  0
#define TRCNVSTMON4_F1_MASK                                          0x00000fff
#define TRCNVSTMON4_F1_RD(src)                           (((src) & 0x00000fff))
#define TRCNVSTMON4_F1_WR(src)                      (((u32)(src)) & 0x00000fff)
#define TRCNVSTMON4_F1_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register trcnvstmoncnt0_14	*/ 
/*	 Fields trcnvstmoncnt	 */
#define TRCNVSTMONCNT4_F1_WIDTH                                              32
#define TRCNVSTMONCNT4_F1_SHIFT                                               0
#define TRCNVSTMONCNT4_F1_MASK                                       0xffffffff
#define TRCNVSTMONCNT4_F1_RD(src)                        (((src) & 0xffffffff))
#define TRCNVSTMONCNT4_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register trcnvstmon0_15	*/ 
/*	 Fields trcnvstmon	 */
#define TRCNVSTMON5_F1_WIDTH                                                 12
#define TRCNVSTMON5_F1_SHIFT                                                  0
#define TRCNVSTMON5_F1_MASK                                          0x00000fff
#define TRCNVSTMON5_F1_RD(src)                           (((src) & 0x00000fff))
#define TRCNVSTMON5_F1_WR(src)                      (((u32)(src)) & 0x00000fff)
#define TRCNVSTMON5_F1_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register trcnvstmoncnt0_15	*/ 
/*	 Fields trcnvstmoncnt	 */
#define TRCNVSTMONCNT5_F1_WIDTH                                              32
#define TRCNVSTMONCNT5_F1_SHIFT                                               0
#define TRCNVSTMONCNT5_F1_MASK                                       0xffffffff
#define TRCNVSTMONCNT5_F1_RD(src)                        (((src) & 0xffffffff))
#define TRCNVSTMONCNT5_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register lstnvst1	*/ 
/*	 Fields lstnvst	 */
#define LSTNVST1_WIDTH                                                       12
#define LSTNVST1_SHIFT                                                        0
#define LSTNVST1_MASK                                                0x00000fff
#define LSTNVST1_RD(src)                                 (((src) & 0x00000fff))
#define LSTNVST1_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lsttrcnvst1_0	*/ 
/*	 Fields ltrcnvst	 */
#define LTRCNVST0_F2_WIDTH                                                   12
#define LTRCNVST0_F2_SHIFT                                                    0
#define LTRCNVST0_F2_MASK                                            0x00000fff
#define LTRCNVST0_F2_RD(src)                             (((src) & 0x00000fff))
#define LTRCNVST0_F2_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lsttrcnvst1_1	*/ 
/*	 Fields ltrcnvst	 */
#define LTRCNVST1_F2_WIDTH                                                   12
#define LTRCNVST1_F2_SHIFT                                                    0
#define LTRCNVST1_F2_MASK                                            0x00000fff
#define LTRCNVST1_F2_RD(src)                             (((src) & 0x00000fff))
#define LTRCNVST1_F2_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lsttrcnvst1_2	*/ 
/*	 Fields ltrcnvst	 */
#define LTRCNVST2_F2_WIDTH                                                   12
#define LTRCNVST2_F2_SHIFT                                                    0
#define LTRCNVST2_F2_MASK                                            0x00000fff
#define LTRCNVST2_F2_RD(src)                             (((src) & 0x00000fff))
#define LTRCNVST2_F2_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lsttrcnvst1_3	*/ 
/*	 Fields ltrcnvst	 */
#define LTRCNVST3_F2_WIDTH                                                   12
#define LTRCNVST3_F2_SHIFT                                                    0
#define LTRCNVST3_F2_MASK                                            0x00000fff
#define LTRCNVST3_F2_RD(src)                             (((src) & 0x00000fff))
#define LTRCNVST3_F2_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lsttrcnvst1_4	*/ 
/*	 Fields ltrcnvst	 */
#define LTRCNVST4_F2_WIDTH                                                   12
#define LTRCNVST4_F2_SHIFT                                                    0
#define LTRCNVST4_F2_MASK                                            0x00000fff
#define LTRCNVST4_F2_RD(src)                             (((src) & 0x00000fff))
#define LTRCNVST4_F2_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lsttrcnvst1_5	*/ 
/*	 Fields ltrcnvst	 */
#define LTRCNVST5_F2_WIDTH                                                   12
#define LTRCNVST5_F2_SHIFT                                                    0
#define LTRCNVST5_F2_MASK                                            0x00000fff
#define LTRCNVST5_F2_RD(src)                             (((src) & 0x00000fff))
#define LTRCNVST5_F2_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lsttrcnvst1_6	*/ 
/*	 Fields ltrcnvst	 */
#define LTRCNVST6_F1_WIDTH                                                   12
#define LTRCNVST6_F1_SHIFT                                                    0
#define LTRCNVST6_F1_MASK                                            0x00000fff
#define LTRCNVST6_F1_RD(src)                             (((src) & 0x00000fff))
#define LTRCNVST6_F1_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lsttrcnvst1_7	*/ 
/*	 Fields ltrcnvst	 */
#define LTRCNVST7_F1_WIDTH                                                   12
#define LTRCNVST7_F1_SHIFT                                                    0
#define LTRCNVST7_F1_MASK                                            0x00000fff
#define LTRCNVST7_F1_RD(src)                             (((src) & 0x00000fff))
#define LTRCNVST7_F1_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lsttrcnvst1_8	*/ 
/*	 Fields ltrcnvst	 */
#define LTRCNVST8_F1_WIDTH                                                   12
#define LTRCNVST8_F1_SHIFT                                                    0
#define LTRCNVST8_F1_MASK                                            0x00000fff
#define LTRCNVST8_F1_RD(src)                             (((src) & 0x00000fff))
#define LTRCNVST8_F1_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lsttrcnvst1_9	*/ 
/*	 Fields ltrcnvst	 */
#define LTRCNVST9_F1_WIDTH                                                   12
#define LTRCNVST9_F1_SHIFT                                                    0
#define LTRCNVST9_F1_MASK                                            0x00000fff
#define LTRCNVST9_F1_RD(src)                             (((src) & 0x00000fff))
#define LTRCNVST9_F1_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lsttrcnvst1_10	*/ 
/*	 Fields ltrcnvst	 */
#define LTRCNVST0_F3_WIDTH                                                   12
#define LTRCNVST0_F3_SHIFT                                                    0
#define LTRCNVST0_F3_MASK                                            0x00000fff
#define LTRCNVST0_F3_RD(src)                             (((src) & 0x00000fff))
#define LTRCNVST0_F3_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lsttrcnvst1_11	*/ 
/*	 Fields ltrcnvst	 */
#define LTRCNVST1_F3_WIDTH                                                   12
#define LTRCNVST1_F3_SHIFT                                                    0
#define LTRCNVST1_F3_MASK                                            0x00000fff
#define LTRCNVST1_F3_RD(src)                             (((src) & 0x00000fff))
#define LTRCNVST1_F3_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lsttrcnvst1_12	*/ 
/*	 Fields ltrcnvst	 */
#define LTRCNVST2_F3_WIDTH                                                   12
#define LTRCNVST2_F3_SHIFT                                                    0
#define LTRCNVST2_F3_MASK                                            0x00000fff
#define LTRCNVST2_F3_RD(src)                             (((src) & 0x00000fff))
#define LTRCNVST2_F3_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lsttrcnvst1_13	*/ 
/*	 Fields ltrcnvst	 */
#define LTRCNVST3_F3_WIDTH                                                   12
#define LTRCNVST3_F3_SHIFT                                                    0
#define LTRCNVST3_F3_MASK                                            0x00000fff
#define LTRCNVST3_F3_RD(src)                             (((src) & 0x00000fff))
#define LTRCNVST3_F3_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lsttrcnvst1_14	*/ 
/*	 Fields ltrcnvst	 */
#define LTRCNVST4_F3_WIDTH                                                   12
#define LTRCNVST4_F3_SHIFT                                                    0
#define LTRCNVST4_F3_MASK                                            0x00000fff
#define LTRCNVST4_F3_RD(src)                             (((src) & 0x00000fff))
#define LTRCNVST4_F3_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lsttrcnvst1_15	*/ 
/*	 Fields ltrcnvst	 */
#define LTRCNVST5_F3_WIDTH                                                   12
#define LTRCNVST5_F3_SHIFT                                                    0
#define LTRCNVST5_F3_MASK                                            0x00000fff
#define LTRCNVST5_F3_RD(src)                             (((src) & 0x00000fff))
#define LTRCNVST5_F3_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register ftrcnvststr1_0	*/ 
/*	 Fields ftrcnvststrt	 */
#define FTRCNVSTSTRT0_F1_WIDTH                                               12
#define FTRCNVSTSTRT0_F1_SHIFT                                                0
#define FTRCNVSTSTRT0_F1_MASK                                        0x00000fff
#define FTRCNVSTSTRT0_F1_RD(src)                         (((src) & 0x00000fff))
#define FTRCNVSTSTRT0_F1_WR(src)                    (((u32)(src)) & 0x00000fff)
#define FTRCNVSTSTRT0_F1_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register ftrcnvst1_0	*/ 
/*	 Fields ftrcnvst	 */
#define FTRCNVST0_F2_WIDTH                                                   12
#define FTRCNVST0_F2_SHIFT                                                    0
#define FTRCNVST0_F2_MASK                                            0x00000fff
#define FTRCNVST0_F2_RD(src)                             (((src) & 0x00000fff))
#define FTRCNVST0_F2_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register ftrcnvst1_1	*/ 
/*	 Fields ftrcnvst	 */
#define FTRCNVST1_F2_WIDTH                                                   12
#define FTRCNVST1_F2_SHIFT                                                    0
#define FTRCNVST1_F2_MASK                                            0x00000fff
#define FTRCNVST1_F2_RD(src)                             (((src) & 0x00000fff))
#define FTRCNVST1_F2_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register ftrcnvst1_2	*/ 
/*	 Fields ftrcnvst	 */
#define FTRCNVST2_F2_WIDTH                                                   12
#define FTRCNVST2_F2_SHIFT                                                    0
#define FTRCNVST2_F2_MASK                                            0x00000fff
#define FTRCNVST2_F2_RD(src)                             (((src) & 0x00000fff))
#define FTRCNVST2_F2_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register ftrcnvst1_3	*/ 
/*	 Fields ftrcnvst	 */
#define FTRCNVST3_F2_WIDTH                                                   12
#define FTRCNVST3_F2_SHIFT                                                    0
#define FTRCNVST3_F2_MASK                                            0x00000fff
#define FTRCNVST3_F2_RD(src)                             (((src) & 0x00000fff))
#define FTRCNVST3_F2_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register ftrcnvst1_4	*/ 
/*	 Fields ftrcnvst	 */
#define FTRCNVST4_F2_WIDTH                                                   12
#define FTRCNVST4_F2_SHIFT                                                    0
#define FTRCNVST4_F2_MASK                                            0x00000fff
#define FTRCNVST4_F2_RD(src)                             (((src) & 0x00000fff))
#define FTRCNVST4_F2_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register ftrcnvst1_5	*/ 
/*	 Fields ftrcnvst	 */
#define FTRCNVST5_F2_WIDTH                                                   12
#define FTRCNVST5_F2_SHIFT                                                    0
#define FTRCNVST5_F2_MASK                                            0x00000fff
#define FTRCNVST5_F2_RD(src)                             (((src) & 0x00000fff))
#define FTRCNVST5_F2_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register ftrcnvst1_6	*/ 
/*	 Fields ftrcnvst	 */
#define FTRCNVST6_F1_WIDTH                                                   12
#define FTRCNVST6_F1_SHIFT                                                    0
#define FTRCNVST6_F1_MASK                                            0x00000fff
#define FTRCNVST6_F1_RD(src)                             (((src) & 0x00000fff))
#define FTRCNVST6_F1_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register ftrcnvst1_7	*/ 
/*	 Fields ftrcnvst	 */
#define FTRCNVST7_F1_WIDTH                                                   12
#define FTRCNVST7_F1_SHIFT                                                    0
#define FTRCNVST7_F1_MASK                                            0x00000fff
#define FTRCNVST7_F1_RD(src)                             (((src) & 0x00000fff))
#define FTRCNVST7_F1_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register ftrcnvst1_8	*/ 
/*	 Fields ftrcnvst	 */
#define FTRCNVST8_F1_WIDTH                                                   12
#define FTRCNVST8_F1_SHIFT                                                    0
#define FTRCNVST8_F1_MASK                                            0x00000fff
#define FTRCNVST8_F1_RD(src)                             (((src) & 0x00000fff))
#define FTRCNVST8_F1_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register ftrcnvst1_9	*/ 
/*	 Fields ftrcnvst	 */
#define FTRCNVST9_F1_WIDTH                                                   12
#define FTRCNVST9_F1_SHIFT                                                    0
#define FTRCNVST9_F1_MASK                                            0x00000fff
#define FTRCNVST9_F1_RD(src)                             (((src) & 0x00000fff))
#define FTRCNVST9_F1_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register ftrcnvst1_10	*/ 
/*	 Fields ftrcnvst	 */
#define FTRCNVST0_F3_WIDTH                                                   12
#define FTRCNVST0_F3_SHIFT                                                    0
#define FTRCNVST0_F3_MASK                                            0x00000fff
#define FTRCNVST0_F3_RD(src)                             (((src) & 0x00000fff))
#define FTRCNVST0_F3_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register ftrcnvst1_11	*/ 
/*	 Fields ftrcnvst	 */
#define FTRCNVST1_F3_WIDTH                                                   12
#define FTRCNVST1_F3_SHIFT                                                    0
#define FTRCNVST1_F3_MASK                                            0x00000fff
#define FTRCNVST1_F3_RD(src)                             (((src) & 0x00000fff))
#define FTRCNVST1_F3_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register ftrcnvst1_12	*/ 
/*	 Fields ftrcnvst	 */
#define FTRCNVST2_F3_WIDTH                                                   12
#define FTRCNVST2_F3_SHIFT                                                    0
#define FTRCNVST2_F3_MASK                                            0x00000fff
#define FTRCNVST2_F3_RD(src)                             (((src) & 0x00000fff))
#define FTRCNVST2_F3_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register ftrcnvst1_13	*/ 
/*	 Fields ftrcnvst	 */
#define FTRCNVST3_F3_WIDTH                                                   12
#define FTRCNVST3_F3_SHIFT                                                    0
#define FTRCNVST3_F3_MASK                                            0x00000fff
#define FTRCNVST3_F3_RD(src)                             (((src) & 0x00000fff))
#define FTRCNVST3_F3_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register ftrcnvst1_14	*/ 
/*	 Fields ftrcnvst	 */
#define FTRCNVST4_F3_WIDTH                                                   12
#define FTRCNVST4_F3_SHIFT                                                    0
#define FTRCNVST4_F3_MASK                                            0x00000fff
#define FTRCNVST4_F3_RD(src)                             (((src) & 0x00000fff))
#define FTRCNVST4_F3_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register ftrcnvst1_15	*/ 
/*	 Fields ftrcnvst	 */
#define FTRCNVST5_F3_WIDTH                                                   12
#define FTRCNVST5_F3_SHIFT                                                    0
#define FTRCNVST5_F3_MASK                                            0x00000fff
#define FTRCNVST5_F3_RD(src)                             (((src) & 0x00000fff))
#define FTRCNVST5_F3_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register trcnvstmon1_0	*/ 
/*	 Fields trcnvstmon	 */
#define TRCNVSTMON0_F2_WIDTH                                                 12
#define TRCNVSTMON0_F2_SHIFT                                                  0
#define TRCNVSTMON0_F2_MASK                                          0x00000fff
#define TRCNVSTMON0_F2_RD(src)                           (((src) & 0x00000fff))
#define TRCNVSTMON0_F2_WR(src)                      (((u32)(src)) & 0x00000fff)
#define TRCNVSTMON0_F2_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register trcnvstmoncnt1_0	*/ 
/*	 Fields trcnvstmoncnt	 */
#define TRCNVSTMONCNT0_F2_WIDTH                                              32
#define TRCNVSTMONCNT0_F2_SHIFT                                               0
#define TRCNVSTMONCNT0_F2_MASK                                       0xffffffff
#define TRCNVSTMONCNT0_F2_RD(src)                        (((src) & 0xffffffff))
#define TRCNVSTMONCNT0_F2_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register trcnvstmon1_1	*/ 
/*	 Fields trcnvstmon	 */
#define TRCNVSTMON1_F2_WIDTH                                                 12
#define TRCNVSTMON1_F2_SHIFT                                                  0
#define TRCNVSTMON1_F2_MASK                                          0x00000fff
#define TRCNVSTMON1_F2_RD(src)                           (((src) & 0x00000fff))
#define TRCNVSTMON1_F2_WR(src)                      (((u32)(src)) & 0x00000fff)
#define TRCNVSTMON1_F2_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register trcnvstmoncnt1_1	*/ 
/*	 Fields trcnvstmoncnt	 */
#define TRCNVSTMONCNT1_F2_WIDTH                                              32
#define TRCNVSTMONCNT1_F2_SHIFT                                               0
#define TRCNVSTMONCNT1_F2_MASK                                       0xffffffff
#define TRCNVSTMONCNT1_F2_RD(src)                        (((src) & 0xffffffff))
#define TRCNVSTMONCNT1_F2_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register trcnvstmon1_2	*/ 
/*	 Fields trcnvstmon	 */
#define TRCNVSTMON2_F2_WIDTH                                                 12
#define TRCNVSTMON2_F2_SHIFT                                                  0
#define TRCNVSTMON2_F2_MASK                                          0x00000fff
#define TRCNVSTMON2_F2_RD(src)                           (((src) & 0x00000fff))
#define TRCNVSTMON2_F2_WR(src)                      (((u32)(src)) & 0x00000fff)
#define TRCNVSTMON2_F2_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register trcnvstmoncnt1_2	*/ 
/*	 Fields trcnvstmoncnt	 */
#define TRCNVSTMONCNT2_F2_WIDTH                                              32
#define TRCNVSTMONCNT2_F2_SHIFT                                               0
#define TRCNVSTMONCNT2_F2_MASK                                       0xffffffff
#define TRCNVSTMONCNT2_F2_RD(src)                        (((src) & 0xffffffff))
#define TRCNVSTMONCNT2_F2_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register trcnvstmon1_3	*/ 
/*	 Fields trcnvstmon	 */
#define TRCNVSTMON3_F2_WIDTH                                                 12
#define TRCNVSTMON3_F2_SHIFT                                                  0
#define TRCNVSTMON3_F2_MASK                                          0x00000fff
#define TRCNVSTMON3_F2_RD(src)                           (((src) & 0x00000fff))
#define TRCNVSTMON3_F2_WR(src)                      (((u32)(src)) & 0x00000fff)
#define TRCNVSTMON3_F2_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register trcnvstmoncnt1_3	*/ 
/*	 Fields trcnvstmoncnt	 */
#define TRCNVSTMONCNT3_F2_WIDTH                                              32
#define TRCNVSTMONCNT3_F2_SHIFT                                               0
#define TRCNVSTMONCNT3_F2_MASK                                       0xffffffff
#define TRCNVSTMONCNT3_F2_RD(src)                        (((src) & 0xffffffff))
#define TRCNVSTMONCNT3_F2_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register trcnvstmon1_4	*/ 
/*	 Fields trcnvstmon	 */
#define TRCNVSTMON4_F2_WIDTH                                                 12
#define TRCNVSTMON4_F2_SHIFT                                                  0
#define TRCNVSTMON4_F2_MASK                                          0x00000fff
#define TRCNVSTMON4_F2_RD(src)                           (((src) & 0x00000fff))
#define TRCNVSTMON4_F2_WR(src)                      (((u32)(src)) & 0x00000fff)
#define TRCNVSTMON4_F2_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register trcnvstmoncnt1_4	*/ 
/*	 Fields trcnvstmoncnt	 */
#define TRCNVSTMONCNT4_F2_WIDTH                                              32
#define TRCNVSTMONCNT4_F2_SHIFT                                               0
#define TRCNVSTMONCNT4_F2_MASK                                       0xffffffff
#define TRCNVSTMONCNT4_F2_RD(src)                        (((src) & 0xffffffff))
#define TRCNVSTMONCNT4_F2_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register trcnvstmon1_5	*/ 
/*	 Fields trcnvstmon	 */
#define TRCNVSTMON5_F2_WIDTH                                                 12
#define TRCNVSTMON5_F2_SHIFT                                                  0
#define TRCNVSTMON5_F2_MASK                                          0x00000fff
#define TRCNVSTMON5_F2_RD(src)                           (((src) & 0x00000fff))
#define TRCNVSTMON5_F2_WR(src)                      (((u32)(src)) & 0x00000fff)
#define TRCNVSTMON5_F2_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register trcnvstmoncnt1_5	*/ 
/*	 Fields trcnvstmoncnt	 */
#define TRCNVSTMONCNT5_F2_WIDTH                                              32
#define TRCNVSTMONCNT5_F2_SHIFT                                               0
#define TRCNVSTMONCNT5_F2_MASK                                       0xffffffff
#define TRCNVSTMONCNT5_F2_RD(src)                        (((src) & 0xffffffff))
#define TRCNVSTMONCNT5_F2_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register trcnvstmon1_6	*/ 
/*	 Fields trcnvstmon	 */
#define TRCNVSTMON6_F1_WIDTH                                                 12
#define TRCNVSTMON6_F1_SHIFT                                                  0
#define TRCNVSTMON6_F1_MASK                                          0x00000fff
#define TRCNVSTMON6_F1_RD(src)                           (((src) & 0x00000fff))
#define TRCNVSTMON6_F1_WR(src)                      (((u32)(src)) & 0x00000fff)
#define TRCNVSTMON6_F1_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register trcnvstmoncnt1_6	*/ 
/*	 Fields trcnvstmoncnt	 */
#define TRCNVSTMONCNT6_F1_WIDTH                                              32
#define TRCNVSTMONCNT6_F1_SHIFT                                               0
#define TRCNVSTMONCNT6_F1_MASK                                       0xffffffff
#define TRCNVSTMONCNT6_F1_RD(src)                        (((src) & 0xffffffff))
#define TRCNVSTMONCNT6_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register trcnvstmon1_7	*/ 
/*	 Fields trcnvstmon	 */
#define TRCNVSTMON7_F1_WIDTH                                                 12
#define TRCNVSTMON7_F1_SHIFT                                                  0
#define TRCNVSTMON7_F1_MASK                                          0x00000fff
#define TRCNVSTMON7_F1_RD(src)                           (((src) & 0x00000fff))
#define TRCNVSTMON7_F1_WR(src)                      (((u32)(src)) & 0x00000fff)
#define TRCNVSTMON7_F1_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register trcnvstmoncnt1_7	*/ 
/*	 Fields trcnvstmoncnt	 */
#define TRCNVSTMONCNT7_F1_WIDTH                                              32
#define TRCNVSTMONCNT7_F1_SHIFT                                               0
#define TRCNVSTMONCNT7_F1_MASK                                       0xffffffff
#define TRCNVSTMONCNT7_F1_RD(src)                        (((src) & 0xffffffff))
#define TRCNVSTMONCNT7_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register trcnvstmon1_8	*/ 
/*	 Fields trcnvstmon	 */
#define TRCNVSTMON8_F1_WIDTH                                                 12
#define TRCNVSTMON8_F1_SHIFT                                                  0
#define TRCNVSTMON8_F1_MASK                                          0x00000fff
#define TRCNVSTMON8_F1_RD(src)                           (((src) & 0x00000fff))
#define TRCNVSTMON8_F1_WR(src)                      (((u32)(src)) & 0x00000fff)
#define TRCNVSTMON8_F1_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register trcnvstmoncnt1_8	*/ 
/*	 Fields trcnvstmoncnt	 */
#define TRCNVSTMONCNT8_F1_WIDTH                                              32
#define TRCNVSTMONCNT8_F1_SHIFT                                               0
#define TRCNVSTMONCNT8_F1_MASK                                       0xffffffff
#define TRCNVSTMONCNT8_F1_RD(src)                        (((src) & 0xffffffff))
#define TRCNVSTMONCNT8_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register trcnvstmon1_9	*/ 
/*	 Fields trcnvstmon	 */
#define TRCNVSTMON9_F1_WIDTH                                                 12
#define TRCNVSTMON9_F1_SHIFT                                                  0
#define TRCNVSTMON9_F1_MASK                                          0x00000fff
#define TRCNVSTMON9_F1_RD(src)                           (((src) & 0x00000fff))
#define TRCNVSTMON9_F1_WR(src)                      (((u32)(src)) & 0x00000fff)
#define TRCNVSTMON9_F1_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register trcnvstmoncnt1_9	*/ 
/*	 Fields trcnvstmoncnt	 */
#define TRCNVSTMONCNT9_F1_WIDTH                                              32
#define TRCNVSTMONCNT9_F1_SHIFT                                               0
#define TRCNVSTMONCNT9_F1_MASK                                       0xffffffff
#define TRCNVSTMONCNT9_F1_RD(src)                        (((src) & 0xffffffff))
#define TRCNVSTMONCNT9_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register trcnvstmon1_10	*/ 
/*	 Fields trcnvstmon	 */
#define TRCNVSTMON0_F3_WIDTH                                                 12
#define TRCNVSTMON0_F3_SHIFT                                                  0
#define TRCNVSTMON0_F3_MASK                                          0x00000fff
#define TRCNVSTMON0_F3_RD(src)                           (((src) & 0x00000fff))
#define TRCNVSTMON0_F3_WR(src)                      (((u32)(src)) & 0x00000fff)
#define TRCNVSTMON0_F3_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register trcnvstmoncnt1_10	*/ 
/*	 Fields trcnvstmoncnt	 */
#define TRCNVSTMONCNT0_F3_WIDTH                                              32
#define TRCNVSTMONCNT0_F3_SHIFT                                               0
#define TRCNVSTMONCNT0_F3_MASK                                       0xffffffff
#define TRCNVSTMONCNT0_F3_RD(src)                        (((src) & 0xffffffff))
#define TRCNVSTMONCNT0_F3_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register trcnvstmon1_11	*/ 
/*	 Fields trcnvstmon	 */
#define TRCNVSTMON1_F3_WIDTH                                                 12
#define TRCNVSTMON1_F3_SHIFT                                                  0
#define TRCNVSTMON1_F3_MASK                                          0x00000fff
#define TRCNVSTMON1_F3_RD(src)                           (((src) & 0x00000fff))
#define TRCNVSTMON1_F3_WR(src)                      (((u32)(src)) & 0x00000fff)
#define TRCNVSTMON1_F3_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register trcnvstmoncnt1_11	*/ 
/*	 Fields trcnvstmoncnt	 */
#define TRCNVSTMONCNT1_F3_WIDTH                                              32
#define TRCNVSTMONCNT1_F3_SHIFT                                               0
#define TRCNVSTMONCNT1_F3_MASK                                       0xffffffff
#define TRCNVSTMONCNT1_F3_RD(src)                        (((src) & 0xffffffff))
#define TRCNVSTMONCNT1_F3_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register trcnvstmon1_12	*/ 
/*	 Fields trcnvstmon	 */
#define TRCNVSTMON2_F3_WIDTH                                                 12
#define TRCNVSTMON2_F3_SHIFT                                                  0
#define TRCNVSTMON2_F3_MASK                                          0x00000fff
#define TRCNVSTMON2_F3_RD(src)                           (((src) & 0x00000fff))
#define TRCNVSTMON2_F3_WR(src)                      (((u32)(src)) & 0x00000fff)
#define TRCNVSTMON2_F3_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register trcnvstmoncnt1_12	*/ 
/*	 Fields trcnvstmoncnt	 */
#define TRCNVSTMONCNT2_F3_WIDTH                                              32
#define TRCNVSTMONCNT2_F3_SHIFT                                               0
#define TRCNVSTMONCNT2_F3_MASK                                       0xffffffff
#define TRCNVSTMONCNT2_F3_RD(src)                        (((src) & 0xffffffff))
#define TRCNVSTMONCNT2_F3_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register trcnvstmon1_13	*/ 
/*	 Fields trcnvstmon	 */
#define TRCNVSTMON3_F3_WIDTH                                                 12
#define TRCNVSTMON3_F3_SHIFT                                                  0
#define TRCNVSTMON3_F3_MASK                                          0x00000fff
#define TRCNVSTMON3_F3_RD(src)                           (((src) & 0x00000fff))
#define TRCNVSTMON3_F3_WR(src)                      (((u32)(src)) & 0x00000fff)
#define TRCNVSTMON3_F3_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register trcnvstmoncnt1_13	*/ 
/*	 Fields trcnvstmoncnt	 */
#define TRCNVSTMONCNT3_F3_WIDTH                                              32
#define TRCNVSTMONCNT3_F3_SHIFT                                               0
#define TRCNVSTMONCNT3_F3_MASK                                       0xffffffff
#define TRCNVSTMONCNT3_F3_RD(src)                        (((src) & 0xffffffff))
#define TRCNVSTMONCNT3_F3_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register trcnvstmon1_14	*/ 
/*	 Fields trcnvstmon	 */
#define TRCNVSTMON4_F3_WIDTH                                                 12
#define TRCNVSTMON4_F3_SHIFT                                                  0
#define TRCNVSTMON4_F3_MASK                                          0x00000fff
#define TRCNVSTMON4_F3_RD(src)                           (((src) & 0x00000fff))
#define TRCNVSTMON4_F3_WR(src)                      (((u32)(src)) & 0x00000fff)
#define TRCNVSTMON4_F3_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register trcnvstmoncnt1_14	*/ 
/*	 Fields trcnvstmoncnt	 */
#define TRCNVSTMONCNT4_F3_WIDTH                                              32
#define TRCNVSTMONCNT4_F3_SHIFT                                               0
#define TRCNVSTMONCNT4_F3_MASK                                       0xffffffff
#define TRCNVSTMONCNT4_F3_RD(src)                        (((src) & 0xffffffff))
#define TRCNVSTMONCNT4_F3_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register trcnvstmon1_15	*/ 
/*	 Fields trcnvstmon	 */
#define TRCNVSTMON5_F3_WIDTH                                                 12
#define TRCNVSTMON5_F3_SHIFT                                                  0
#define TRCNVSTMON5_F3_MASK                                          0x00000fff
#define TRCNVSTMON5_F3_RD(src)                           (((src) & 0x00000fff))
#define TRCNVSTMON5_F3_WR(src)                      (((u32)(src)) & 0x00000fff)
#define TRCNVSTMON5_F3_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register trcnvstmoncnt1_15	*/ 
/*	 Fields trcnvstmoncnt	 */
#define TRCNVSTMONCNT5_F3_WIDTH                                              32
#define TRCNVSTMONCNT5_F3_SHIFT                                               0
#define TRCNVSTMONCNT5_F3_MASK                                       0xffffffff
#define TRCNVSTMONCNT5_F3_RD(src)                        (((src) & 0xffffffff))
#define TRCNVSTMONCNT5_F3_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register snptr2	*/ 
/*	 Fields wol_mode	 */
#define WOL_MODE2_WIDTH                                                       1
#define WOL_MODE2_SHIFT                                                      31
#define WOL_MODE2_MASK                                               0x80000000
#define WOL_MODE2_RD(src)                            (((src) & 0x80000000)>>31)
#define WOL_MODE2_WR(src)                       (((u32)(src)<<31) & 0x80000000)
#define WOL_MODE2_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields cle_eth_spare	 */
#define CLE_ETH_SPARE2_WIDTH                                                  4
#define CLE_ETH_SPARE2_SHIFT                                                 27
#define CLE_ETH_SPARE2_MASK                                          0x78000000
#define CLE_ETH_SPARE2_RD(src)                       (((src) & 0x78000000)>>27)
#define CLE_ETH_SPARE2_WR(src)                  (((u32)(src)<<27) & 0x78000000)
#define CLE_ETH_SPARE2_SET(dst,src) \
                      (((dst) & ~0x78000000) | (((u32)(src)<<27) & 0x78000000))
/*	 Fields snptr	 */
#define SNPTR2_WIDTH                                                         12
#define SNPTR2_SHIFT                                                          0
#define SNPTR2_MASK                                                  0x00000fff
#define SNPTR2_RD(src)                                   (((src) & 0x00000fff))
#define SNPTR2_WR(src)                              (((u32)(src)) & 0x00000fff)
#define SNPTR2_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register spptr2	*/ 
/*	 Fields spptr	 */
#define SPPTR2_WIDTH                                                          9
#define SPPTR2_SHIFT                                                          0
#define SPPTR2_MASK                                                  0x000001ff
#define SPPTR2_RD(src)                                   (((src) & 0x000001ff))
#define SPPTR2_WR(src)                              (((u32)(src)) & 0x000001ff)
#define SPPTR2_SET(dst,src) \
                          (((dst) & ~0x000001ff) | (((u32)(src)) & 0x000001ff))

/*	Register dfclsresdbptr2	*/ 
/*	 Fields dfclsresdbpriority	 */
#define DFCLSRESDBPRIORITY2_WIDTH                                             3
#define DFCLSRESDBPRIORITY2_SHIFT                                            10
#define DFCLSRESDBPRIORITY2_MASK                                     0x00001c00
#define DFCLSRESDBPRIORITY2_RD(src)                  (((src) & 0x00001c00)>>10)
#define DFCLSRESDBPRIORITY2_WR(src)             (((u32)(src)<<10) & 0x00001c00)
#define DFCLSRESDBPRIORITY2_SET(dst,src) \
                      (((dst) & ~0x00001c00) | (((u32)(src)<<10) & 0x00001c00))
/*	 Fields dfclsresdbptr	 */
#define DFCLSRESDBPTR2_WIDTH                                                 10
#define DFCLSRESDBPTR2_SHIFT                                                  0
#define DFCLSRESDBPTR2_MASK                                          0x000003ff
#define DFCLSRESDBPTR2_RD(src)                           (((src) & 0x000003ff))
#define DFCLSRESDBPTR2_WR(src)                      (((u32)(src)) & 0x000003ff)
#define DFCLSRESDBPTR2_SET(dst,src) \
                          (((dst) & ~0x000003ff) | (((u32)(src)) & 0x000003ff))

/*	Register dfclsresdb2_0	*/ 
/*	 Fields dfclsresdb	 */
#define DFCLSRESDB0_F2_WIDTH                                                 32
#define DFCLSRESDB0_F2_SHIFT                                                  0
#define DFCLSRESDB0_F2_MASK                                          0xffffffff
#define DFCLSRESDB0_F2_RD(src)                           (((src) & 0xffffffff))
#define DFCLSRESDB0_F2_WR(src)                      (((u32)(src)) & 0xffffffff)
#define DFCLSRESDB0_F2_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register dfclsresdb2_1	*/ 
/*	 Fields dfclsresdb	 */
#define DFCLSRESDB1_F2_WIDTH                                                 32
#define DFCLSRESDB1_F2_SHIFT                                                  0
#define DFCLSRESDB1_F2_MASK                                          0xffffffff
#define DFCLSRESDB1_F2_RD(src)                           (((src) & 0xffffffff))
#define DFCLSRESDB1_F2_WR(src)                      (((u32)(src)) & 0xffffffff)
#define DFCLSRESDB1_F2_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register dfclsresdb2_2	*/ 
/*	 Fields dfclsresdb	 */
#define DFCLSRESDB2_F2_WIDTH                                                 32
#define DFCLSRESDB2_F2_SHIFT                                                  0
#define DFCLSRESDB2_F2_MASK                                          0xffffffff
#define DFCLSRESDB2_F2_RD(src)                           (((src) & 0xffffffff))
#define DFCLSRESDB2_F2_WR(src)                      (((u32)(src)) & 0xffffffff)
#define DFCLSRESDB2_F2_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register dfclsresdb2_3	*/ 
/*	 Fields dfclsresdb	 */
#define DFCLSRESDB3_F2_WIDTH                                                 32
#define DFCLSRESDB3_F2_SHIFT                                                  0
#define DFCLSRESDB3_F2_MASK                                          0xffffffff
#define DFCLSRESDB3_F2_RD(src)                           (((src) & 0xffffffff))
#define DFCLSRESDB3_F2_WR(src)                      (((u32)(src)) & 0xffffffff)
#define DFCLSRESDB3_F2_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register dfclsresdb2_4	*/ 
/*	 Fields dfclsresdb	 */
#define DFCLSRESDB4_F2_WIDTH                                                 32
#define DFCLSRESDB4_F2_SHIFT                                                  0
#define DFCLSRESDB4_F2_MASK                                          0xffffffff
#define DFCLSRESDB4_F2_RD(src)                           (((src) & 0xffffffff))
#define DFCLSRESDB4_F2_WR(src)                      (((u32)(src)) & 0xffffffff)
#define DFCLSRESDB4_F2_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register dfclsresdb2_5	*/ 
/*	 Fields dfclsresdb	 */
#define DFCLSRESDB5_F2_WIDTH                                                 32
#define DFCLSRESDB5_F2_SHIFT                                                  0
#define DFCLSRESDB5_F2_MASK                                          0xffffffff
#define DFCLSRESDB5_F2_RD(src)                           (((src) & 0xffffffff))
#define DFCLSRESDB5_F2_WR(src)                      (((u32)(src)) & 0xffffffff)
#define DFCLSRESDB5_F2_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register parser_ctl2	*/ 
/*	 Fields pkt_data_msk_en	 */
#define PKT_DATA_MSK_EN2_WIDTH                                                1
#define PKT_DATA_MSK_EN2_SHIFT                                               31
#define PKT_DATA_MSK_EN2_MASK                                        0x80000000
#define PKT_DATA_MSK_EN2_RD(src)                     (((src) & 0x80000000)>>31)
#define PKT_DATA_MSK_EN2_WR(src)                (((u32)(src)<<31) & 0x80000000)
#define PKT_DATA_MSK_EN2_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields parser_halt	 */
#define PARSER_HALT2_WIDTH                                                    1
#define PARSER_HALT2_SHIFT                                                    0
#define PARSER_HALT2_MASK                                            0x00000001
#define PARSER_HALT2_RD(src)                             (((src) & 0x00000001))
#define PARSER_HALT2_WR(src)                        (((u32)(src)) & 0x00000001)
#define PARSER_HALT2_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register tmaxhop2	*/ 
/*	 Fields tmaxhop	 */
#define TMAXHOP2_WIDTH                                                        8
#define TMAXHOP2_SHIFT                                                        0
#define TMAXHOP2_MASK                                                0x000000ff
#define TMAXHOP2_RD(src)                                 (((src) & 0x000000ff))
#define TMAXHOP2_WR(src)                            (((u32)(src)) & 0x000000ff)
#define TMAXHOP2_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register portnum2	*/ 
/*	 Fields portnum2	 */
#define PORTNUM22_WIDTH                                                       8
#define PORTNUM22_SHIFT                                                       0
#define PORTNUM22_MASK                                               0x000000ff
#define PORTNUM22_RD(src)                                (((src) & 0x000000ff))
#define PORTNUM22_WR(src)                           (((u32)(src)) & 0x000000ff)
#define PORTNUM22_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register pktram2_err_diagnostic	*/ 
/*	 Fields Error_Addr	 */
#define ERROR_ADDR_F5_WIDTH                                                   5
#define ERROR_ADDR_F5_SHIFT                                                   2
#define ERROR_ADDR_F5_MASK                                           0x0000007c
#define ERROR_ADDR_F5_RD(src)                         (((src) & 0x0000007c)>>2)
#define ERROR_ADDR_F5_WR(src)                    (((u32)(src)<<2) & 0x0000007c)
#define ERROR_ADDR_F5_SET(dst,src) \
                       (((dst) & ~0x0000007c) | (((u32)(src)<<2) & 0x0000007c))
/*	 Fields Error_Addr_Enable	 */
#define ERROR_ADDR_ENABLE_F5_WIDTH                                            1
#define ERROR_ADDR_ENABLE_F5_SHIFT                                            1
#define ERROR_ADDR_ENABLE_F5_MASK                                    0x00000002
#define ERROR_ADDR_ENABLE_F5_RD(src)                  (((src) & 0x00000002)>>1)
#define ERROR_ADDR_ENABLE_F5_WR(src)             (((u32)(src)<<1) & 0x00000002)
#define ERROR_ADDR_ENABLE_F5_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields Force_Read_Error_Enable	 */
#define FORCE_READ_ERROR_ENABLE_F5_WIDTH                                      1
#define FORCE_READ_ERROR_ENABLE_F5_SHIFT                                      0
#define FORCE_READ_ERROR_ENABLE_F5_MASK                              0x00000001
#define FORCE_READ_ERROR_ENABLE_F5_RD(src)               (((src) & 0x00000001))
#define FORCE_READ_ERROR_ENABLE_F5_WR(src)          (((u32)(src)) & 0x00000001)
#define FORCE_READ_ERROR_ENABLE_F5_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register snptr_change_status2	*/ 
/*	 Fields snptr_change_done	 */
#define SNPTR_CHANGE_DONE2_WIDTH                                              1
#define SNPTR_CHANGE_DONE2_SHIFT                                              0
#define SNPTR_CHANGE_DONE2_MASK                                      0x00000001
#define SNPTR_CHANGE_DONE2_RD(src)                       (((src) & 0x00000001))
#define SNPTR_CHANGE_DONE2_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register parser_status2	*/ 
/*	 Fields PktRamErr	 */
#define PKTRAMERR2_WIDTH                                                      1
#define PKTRAMERR2_SHIFT                                                      4
#define PKTRAMERR2_MASK                                              0x00000010
#define PKTRAMERR2_RD(src)                            (((src) & 0x00000010)>>4)
#define PKTRAMERR2_WR(src)                       (((u32)(src)<<4) & 0x00000010)
#define PKTRAMERR2_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields PTRamErr	 */
#define PTRAMERR2_WIDTH                                                       1
#define PTRAMERR2_SHIFT                                                       3
#define PTRAMERR2_MASK                                               0x00000008
#define PTRAMERR2_RD(src)                             (((src) & 0x00000008)>>3)
#define PTRAMERR2_WR(src)                        (((u32)(src)<<3) & 0x00000008)
#define PTRAMERR2_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields PktOffsetExEop	 */
#define PKTOFFSETEXEOP2_WIDTH                                                 1
#define PKTOFFSETEXEOP2_SHIFT                                                 2
#define PKTOFFSETEXEOP2_MASK                                         0x00000004
#define PKTOFFSETEXEOP2_RD(src)                       (((src) & 0x00000004)>>2)
#define PKTOFFSETEXEOP2_WR(src)                  (((u32)(src)<<2) & 0x00000004)
#define PKTOFFSETEXEOP2_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields maxhoperr	 */
#define MAXHOPERR2_WIDTH                                                      1
#define MAXHOPERR2_SHIFT                                                      1
#define MAXHOPERR2_MASK                                              0x00000002
#define MAXHOPERR2_RD(src)                            (((src) & 0x00000002)>>1)
#define MAXHOPERR2_WR(src)                       (((u32)(src)<<1) & 0x00000002)
#define MAXHOPERR2_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields parser_halt_status	 */
#define PARSER_HALT_STATUS2_WIDTH                                             1
#define PARSER_HALT_STATUS2_SHIFT                                             0
#define PARSER_HALT_STATUS2_MASK                                     0x00000001
#define PARSER_HALT_STATUS2_RD(src)                      (((src) & 0x00000001))
#define PARSER_HALT_STATUS2_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register parser_status2Mask	*/
/*    Mask Register Fields PktRamErrMask    */
#define PKTRAMERRMASK_F2_WIDTH                                                1
#define PKTRAMERRMASK_F2_SHIFT                                                4
#define PKTRAMERRMASK_F2_MASK                                        0x00000010
#define PKTRAMERRMASK_F2_RD(src)                      (((src) & 0x00000010)>>4)
#define PKTRAMERRMASK_F2_WR(src)                 (((u32)(src)<<4) & 0x00000010)
#define PKTRAMERRMASK_F2_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*    Mask Register Fields PTRamErrMask    */
#define PTRAMERRMASK_F2_WIDTH                                                 1
#define PTRAMERRMASK_F2_SHIFT                                                 3
#define PTRAMERRMASK_F2_MASK                                         0x00000008
#define PTRAMERRMASK_F2_RD(src)                       (((src) & 0x00000008)>>3)
#define PTRAMERRMASK_F2_WR(src)                  (((u32)(src)<<3) & 0x00000008)
#define PTRAMERRMASK_F2_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*    Mask Register Fields PktOffsetExEopMask    */
#define PKTOFFSETEXEOPMASK_F2_WIDTH                                           1
#define PKTOFFSETEXEOPMASK_F2_SHIFT                                           2
#define PKTOFFSETEXEOPMASK_F2_MASK                                   0x00000004
#define PKTOFFSETEXEOPMASK_F2_RD(src)                 (((src) & 0x00000004)>>2)
#define PKTOFFSETEXEOPMASK_F2_WR(src)            (((u32)(src)<<2) & 0x00000004)
#define PKTOFFSETEXEOPMASK_F2_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*    Mask Register Fields maxhoperrMask    */
#define MAXHOPERRMASK_F2_WIDTH                                                1
#define MAXHOPERRMASK_F2_SHIFT                                                1
#define MAXHOPERRMASK_F2_MASK                                        0x00000002
#define MAXHOPERRMASK_F2_RD(src)                      (((src) & 0x00000002)>>1)
#define MAXHOPERRMASK_F2_WR(src)                 (((u32)(src)<<1) & 0x00000002)
#define MAXHOPERRMASK_F2_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*    Mask Register Fields parser_halt_statusMask    */
#define PARSER_HALT_STATUSMASK_F2_WIDTH                                       1
#define PARSER_HALT_STATUSMASK_F2_SHIFT                                       0
#define PARSER_HALT_STATUSMASK_F2_MASK                               0x00000001
#define PARSER_HALT_STATUSMASK_F2_RD(src)                (((src) & 0x00000001))
#define PARSER_HALT_STATUSMASK_F2_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register lstnvst2	*/ 
/*	 Fields lstnvst	 */
#define LSTNVST2_WIDTH                                                       12
#define LSTNVST2_SHIFT                                                        0
#define LSTNVST2_MASK                                                0x00000fff
#define LSTNVST2_RD(src)                                 (((src) & 0x00000fff))
#define LSTNVST2_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lsttrcnvst2_0	*/ 
/*	 Fields ltrcnvst	 */
#define LTRCNVST0_F4_WIDTH                                                   12
#define LTRCNVST0_F4_SHIFT                                                    0
#define LTRCNVST0_F4_MASK                                            0x00000fff
#define LTRCNVST0_F4_RD(src)                             (((src) & 0x00000fff))
#define LTRCNVST0_F4_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lsttrcnvst2_1	*/ 
/*	 Fields ltrcnvst	 */
#define LTRCNVST1_F4_WIDTH                                                   12
#define LTRCNVST1_F4_SHIFT                                                    0
#define LTRCNVST1_F4_MASK                                            0x00000fff
#define LTRCNVST1_F4_RD(src)                             (((src) & 0x00000fff))
#define LTRCNVST1_F4_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lsttrcnvst2_2	*/ 
/*	 Fields ltrcnvst	 */
#define LTRCNVST2_F4_WIDTH                                                   12
#define LTRCNVST2_F4_SHIFT                                                    0
#define LTRCNVST2_F4_MASK                                            0x00000fff
#define LTRCNVST2_F4_RD(src)                             (((src) & 0x00000fff))
#define LTRCNVST2_F4_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lsttrcnvst2_3	*/ 
/*	 Fields ltrcnvst	 */
#define LTRCNVST3_F4_WIDTH                                                   12
#define LTRCNVST3_F4_SHIFT                                                    0
#define LTRCNVST3_F4_MASK                                            0x00000fff
#define LTRCNVST3_F4_RD(src)                             (((src) & 0x00000fff))
#define LTRCNVST3_F4_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lsttrcnvst2_4	*/ 
/*	 Fields ltrcnvst	 */
#define LTRCNVST4_F4_WIDTH                                                   12
#define LTRCNVST4_F4_SHIFT                                                    0
#define LTRCNVST4_F4_MASK                                            0x00000fff
#define LTRCNVST4_F4_RD(src)                             (((src) & 0x00000fff))
#define LTRCNVST4_F4_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lsttrcnvst2_5	*/ 
/*	 Fields ltrcnvst	 */
#define LTRCNVST5_F4_WIDTH                                                   12
#define LTRCNVST5_F4_SHIFT                                                    0
#define LTRCNVST5_F4_MASK                                            0x00000fff
#define LTRCNVST5_F4_RD(src)                             (((src) & 0x00000fff))
#define LTRCNVST5_F4_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lsttrcnvst2_6	*/ 
/*	 Fields ltrcnvst	 */
#define LTRCNVST6_F2_WIDTH                                                   12
#define LTRCNVST6_F2_SHIFT                                                    0
#define LTRCNVST6_F2_MASK                                            0x00000fff
#define LTRCNVST6_F2_RD(src)                             (((src) & 0x00000fff))
#define LTRCNVST6_F2_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lsttrcnvst2_7	*/ 
/*	 Fields ltrcnvst	 */
#define LTRCNVST7_F2_WIDTH                                                   12
#define LTRCNVST7_F2_SHIFT                                                    0
#define LTRCNVST7_F2_MASK                                            0x00000fff
#define LTRCNVST7_F2_RD(src)                             (((src) & 0x00000fff))
#define LTRCNVST7_F2_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lsttrcnvst2_8	*/ 
/*	 Fields ltrcnvst	 */
#define LTRCNVST8_F2_WIDTH                                                   12
#define LTRCNVST8_F2_SHIFT                                                    0
#define LTRCNVST8_F2_MASK                                            0x00000fff
#define LTRCNVST8_F2_RD(src)                             (((src) & 0x00000fff))
#define LTRCNVST8_F2_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lsttrcnvst2_9	*/ 
/*	 Fields ltrcnvst	 */
#define LTRCNVST9_F2_WIDTH                                                   12
#define LTRCNVST9_F2_SHIFT                                                    0
#define LTRCNVST9_F2_MASK                                            0x00000fff
#define LTRCNVST9_F2_RD(src)                             (((src) & 0x00000fff))
#define LTRCNVST9_F2_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lsttrcnvst2_10	*/ 
/*	 Fields ltrcnvst	 */
#define LTRCNVST0_F5_WIDTH                                                   12
#define LTRCNVST0_F5_SHIFT                                                    0
#define LTRCNVST0_F5_MASK                                            0x00000fff
#define LTRCNVST0_F5_RD(src)                             (((src) & 0x00000fff))
#define LTRCNVST0_F5_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lsttrcnvst2_11	*/ 
/*	 Fields ltrcnvst	 */
#define LTRCNVST1_F5_WIDTH                                                   12
#define LTRCNVST1_F5_SHIFT                                                    0
#define LTRCNVST1_F5_MASK                                            0x00000fff
#define LTRCNVST1_F5_RD(src)                             (((src) & 0x00000fff))
#define LTRCNVST1_F5_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lsttrcnvst2_12	*/ 
/*	 Fields ltrcnvst	 */
#define LTRCNVST2_F5_WIDTH                                                   12
#define LTRCNVST2_F5_SHIFT                                                    0
#define LTRCNVST2_F5_MASK                                            0x00000fff
#define LTRCNVST2_F5_RD(src)                             (((src) & 0x00000fff))
#define LTRCNVST2_F5_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lsttrcnvst2_13	*/ 
/*	 Fields ltrcnvst	 */
#define LTRCNVST3_F5_WIDTH                                                   12
#define LTRCNVST3_F5_SHIFT                                                    0
#define LTRCNVST3_F5_MASK                                            0x00000fff
#define LTRCNVST3_F5_RD(src)                             (((src) & 0x00000fff))
#define LTRCNVST3_F5_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lsttrcnvst2_14	*/ 
/*	 Fields ltrcnvst	 */
#define LTRCNVST4_F5_WIDTH                                                   12
#define LTRCNVST4_F5_SHIFT                                                    0
#define LTRCNVST4_F5_MASK                                            0x00000fff
#define LTRCNVST4_F5_RD(src)                             (((src) & 0x00000fff))
#define LTRCNVST4_F5_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register lsttrcnvst2_15	*/ 
/*	 Fields ltrcnvst	 */
#define LTRCNVST5_F5_WIDTH                                                   12
#define LTRCNVST5_F5_SHIFT                                                    0
#define LTRCNVST5_F5_MASK                                            0x00000fff
#define LTRCNVST5_F5_RD(src)                             (((src) & 0x00000fff))
#define LTRCNVST5_F5_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register ftrcnvststr2_0	*/ 
/*	 Fields ftrcnvststrt	 */
#define FTRCNVSTSTRT0_F2_WIDTH                                               12
#define FTRCNVSTSTRT0_F2_SHIFT                                                0
#define FTRCNVSTSTRT0_F2_MASK                                        0x00000fff
#define FTRCNVSTSTRT0_F2_RD(src)                         (((src) & 0x00000fff))
#define FTRCNVSTSTRT0_F2_WR(src)                    (((u32)(src)) & 0x00000fff)
#define FTRCNVSTSTRT0_F2_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register ftrcnvst2_0	*/ 
/*	 Fields ftrcnvst	 */
#define FTRCNVST0_F4_WIDTH                                                   12
#define FTRCNVST0_F4_SHIFT                                                    0
#define FTRCNVST0_F4_MASK                                            0x00000fff
#define FTRCNVST0_F4_RD(src)                             (((src) & 0x00000fff))
#define FTRCNVST0_F4_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register ftrcnvst2_1	*/ 
/*	 Fields ftrcnvst	 */
#define FTRCNVST1_F4_WIDTH                                                   12
#define FTRCNVST1_F4_SHIFT                                                    0
#define FTRCNVST1_F4_MASK                                            0x00000fff
#define FTRCNVST1_F4_RD(src)                             (((src) & 0x00000fff))
#define FTRCNVST1_F4_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register ftrcnvst2_2	*/ 
/*	 Fields ftrcnvst	 */
#define FTRCNVST2_F4_WIDTH                                                   12
#define FTRCNVST2_F4_SHIFT                                                    0
#define FTRCNVST2_F4_MASK                                            0x00000fff
#define FTRCNVST2_F4_RD(src)                             (((src) & 0x00000fff))
#define FTRCNVST2_F4_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register ftrcnvst2_3	*/ 
/*	 Fields ftrcnvst	 */
#define FTRCNVST3_F4_WIDTH                                                   12
#define FTRCNVST3_F4_SHIFT                                                    0
#define FTRCNVST3_F4_MASK                                            0x00000fff
#define FTRCNVST3_F4_RD(src)                             (((src) & 0x00000fff))
#define FTRCNVST3_F4_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register ftrcnvst2_4	*/ 
/*	 Fields ftrcnvst	 */
#define FTRCNVST4_F4_WIDTH                                                   12
#define FTRCNVST4_F4_SHIFT                                                    0
#define FTRCNVST4_F4_MASK                                            0x00000fff
#define FTRCNVST4_F4_RD(src)                             (((src) & 0x00000fff))
#define FTRCNVST4_F4_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register ftrcnvst2_5	*/ 
/*	 Fields ftrcnvst	 */
#define FTRCNVST5_F4_WIDTH                                                   12
#define FTRCNVST5_F4_SHIFT                                                    0
#define FTRCNVST5_F4_MASK                                            0x00000fff
#define FTRCNVST5_F4_RD(src)                             (((src) & 0x00000fff))
#define FTRCNVST5_F4_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register ftrcnvst2_6	*/ 
/*	 Fields ftrcnvst	 */
#define FTRCNVST6_F2_WIDTH                                                   12
#define FTRCNVST6_F2_SHIFT                                                    0
#define FTRCNVST6_F2_MASK                                            0x00000fff
#define FTRCNVST6_F2_RD(src)                             (((src) & 0x00000fff))
#define FTRCNVST6_F2_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register ftrcnvst2_7	*/ 
/*	 Fields ftrcnvst	 */
#define FTRCNVST7_F2_WIDTH                                                   12
#define FTRCNVST7_F2_SHIFT                                                    0
#define FTRCNVST7_F2_MASK                                            0x00000fff
#define FTRCNVST7_F2_RD(src)                             (((src) & 0x00000fff))
#define FTRCNVST7_F2_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register ftrcnvst2_8	*/ 
/*	 Fields ftrcnvst	 */
#define FTRCNVST8_F2_WIDTH                                                   12
#define FTRCNVST8_F2_SHIFT                                                    0
#define FTRCNVST8_F2_MASK                                            0x00000fff
#define FTRCNVST8_F2_RD(src)                             (((src) & 0x00000fff))
#define FTRCNVST8_F2_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register ftrcnvst2_9	*/ 
/*	 Fields ftrcnvst	 */
#define FTRCNVST9_F2_WIDTH                                                   12
#define FTRCNVST9_F2_SHIFT                                                    0
#define FTRCNVST9_F2_MASK                                            0x00000fff
#define FTRCNVST9_F2_RD(src)                             (((src) & 0x00000fff))
#define FTRCNVST9_F2_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register ftrcnvst2_10	*/ 
/*	 Fields ftrcnvst	 */
#define FTRCNVST0_F5_WIDTH                                                   12
#define FTRCNVST0_F5_SHIFT                                                    0
#define FTRCNVST0_F5_MASK                                            0x00000fff
#define FTRCNVST0_F5_RD(src)                             (((src) & 0x00000fff))
#define FTRCNVST0_F5_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register ftrcnvst2_11	*/ 
/*	 Fields ftrcnvst	 */
#define FTRCNVST1_F5_WIDTH                                                   12
#define FTRCNVST1_F5_SHIFT                                                    0
#define FTRCNVST1_F5_MASK                                            0x00000fff
#define FTRCNVST1_F5_RD(src)                             (((src) & 0x00000fff))
#define FTRCNVST1_F5_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register ftrcnvst2_12	*/ 
/*	 Fields ftrcnvst	 */
#define FTRCNVST2_F5_WIDTH                                                   12
#define FTRCNVST2_F5_SHIFT                                                    0
#define FTRCNVST2_F5_MASK                                            0x00000fff
#define FTRCNVST2_F5_RD(src)                             (((src) & 0x00000fff))
#define FTRCNVST2_F5_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register ftrcnvst2_13	*/ 
/*	 Fields ftrcnvst	 */
#define FTRCNVST3_F5_WIDTH                                                   12
#define FTRCNVST3_F5_SHIFT                                                    0
#define FTRCNVST3_F5_MASK                                            0x00000fff
#define FTRCNVST3_F5_RD(src)                             (((src) & 0x00000fff))
#define FTRCNVST3_F5_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register ftrcnvst2_14	*/ 
/*	 Fields ftrcnvst	 */
#define FTRCNVST4_F5_WIDTH                                                   12
#define FTRCNVST4_F5_SHIFT                                                    0
#define FTRCNVST4_F5_MASK                                            0x00000fff
#define FTRCNVST4_F5_RD(src)                             (((src) & 0x00000fff))
#define FTRCNVST4_F5_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register ftrcnvst2_15	*/ 
/*	 Fields ftrcnvst	 */
#define FTRCNVST5_F5_WIDTH                                                   12
#define FTRCNVST5_F5_SHIFT                                                    0
#define FTRCNVST5_F5_MASK                                            0x00000fff
#define FTRCNVST5_F5_RD(src)                             (((src) & 0x00000fff))
#define FTRCNVST5_F5_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register trcnvstmon2_0	*/ 
/*	 Fields trcnvstmon	 */
#define TRCNVSTMON0_F4_WIDTH                                                 12
#define TRCNVSTMON0_F4_SHIFT                                                  0
#define TRCNVSTMON0_F4_MASK                                          0x00000fff
#define TRCNVSTMON0_F4_RD(src)                           (((src) & 0x00000fff))
#define TRCNVSTMON0_F4_WR(src)                      (((u32)(src)) & 0x00000fff)
#define TRCNVSTMON0_F4_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register trcnvstmoncnt2_0	*/ 
/*	 Fields trcnvstmoncnt	 */
#define TRCNVSTMONCNT0_F4_WIDTH                                              32
#define TRCNVSTMONCNT0_F4_SHIFT                                               0
#define TRCNVSTMONCNT0_F4_MASK                                       0xffffffff
#define TRCNVSTMONCNT0_F4_RD(src)                        (((src) & 0xffffffff))
#define TRCNVSTMONCNT0_F4_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register trcnvstmon2_1	*/ 
/*	 Fields trcnvstmon	 */
#define TRCNVSTMON1_F4_WIDTH                                                 12
#define TRCNVSTMON1_F4_SHIFT                                                  0
#define TRCNVSTMON1_F4_MASK                                          0x00000fff
#define TRCNVSTMON1_F4_RD(src)                           (((src) & 0x00000fff))
#define TRCNVSTMON1_F4_WR(src)                      (((u32)(src)) & 0x00000fff)
#define TRCNVSTMON1_F4_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register trcnvstmoncnt2_1	*/ 
/*	 Fields trcnvstmoncnt	 */
#define TRCNVSTMONCNT1_F4_WIDTH                                              32
#define TRCNVSTMONCNT1_F4_SHIFT                                               0
#define TRCNVSTMONCNT1_F4_MASK                                       0xffffffff
#define TRCNVSTMONCNT1_F4_RD(src)                        (((src) & 0xffffffff))
#define TRCNVSTMONCNT1_F4_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register trcnvstmon2_2	*/ 
/*	 Fields trcnvstmon	 */
#define TRCNVSTMON2_F4_WIDTH                                                 12
#define TRCNVSTMON2_F4_SHIFT                                                  0
#define TRCNVSTMON2_F4_MASK                                          0x00000fff
#define TRCNVSTMON2_F4_RD(src)                           (((src) & 0x00000fff))
#define TRCNVSTMON2_F4_WR(src)                      (((u32)(src)) & 0x00000fff)
#define TRCNVSTMON2_F4_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register trcnvstmoncnt2_2	*/ 
/*	 Fields trcnvstmoncnt	 */
#define TRCNVSTMONCNT2_F4_WIDTH                                              32
#define TRCNVSTMONCNT2_F4_SHIFT                                               0
#define TRCNVSTMONCNT2_F4_MASK                                       0xffffffff
#define TRCNVSTMONCNT2_F4_RD(src)                        (((src) & 0xffffffff))
#define TRCNVSTMONCNT2_F4_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register trcnvstmon2_3	*/ 
/*	 Fields trcnvstmon	 */
#define TRCNVSTMON3_F4_WIDTH                                                 12
#define TRCNVSTMON3_F4_SHIFT                                                  0
#define TRCNVSTMON3_F4_MASK                                          0x00000fff
#define TRCNVSTMON3_F4_RD(src)                           (((src) & 0x00000fff))
#define TRCNVSTMON3_F4_WR(src)                      (((u32)(src)) & 0x00000fff)
#define TRCNVSTMON3_F4_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register trcnvstmoncnt2_3	*/ 
/*	 Fields trcnvstmoncnt	 */
#define TRCNVSTMONCNT3_F4_WIDTH                                              32
#define TRCNVSTMONCNT3_F4_SHIFT                                               0
#define TRCNVSTMONCNT3_F4_MASK                                       0xffffffff
#define TRCNVSTMONCNT3_F4_RD(src)                        (((src) & 0xffffffff))
#define TRCNVSTMONCNT3_F4_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register trcnvstmon2_4	*/ 
/*	 Fields trcnvstmon	 */
#define TRCNVSTMON4_F4_WIDTH                                                 12
#define TRCNVSTMON4_F4_SHIFT                                                  0
#define TRCNVSTMON4_F4_MASK                                          0x00000fff
#define TRCNVSTMON4_F4_RD(src)                           (((src) & 0x00000fff))
#define TRCNVSTMON4_F4_WR(src)                      (((u32)(src)) & 0x00000fff)
#define TRCNVSTMON4_F4_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register trcnvstmoncnt2_4	*/ 
/*	 Fields trcnvstmoncnt	 */
#define TRCNVSTMONCNT4_F4_WIDTH                                              32
#define TRCNVSTMONCNT4_F4_SHIFT                                               0
#define TRCNVSTMONCNT4_F4_MASK                                       0xffffffff
#define TRCNVSTMONCNT4_F4_RD(src)                        (((src) & 0xffffffff))
#define TRCNVSTMONCNT4_F4_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register trcnvstmon2_5	*/ 
/*	 Fields trcnvstmon	 */
#define TRCNVSTMON5_F4_WIDTH                                                 12
#define TRCNVSTMON5_F4_SHIFT                                                  0
#define TRCNVSTMON5_F4_MASK                                          0x00000fff
#define TRCNVSTMON5_F4_RD(src)                           (((src) & 0x00000fff))
#define TRCNVSTMON5_F4_WR(src)                      (((u32)(src)) & 0x00000fff)
#define TRCNVSTMON5_F4_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register trcnvstmoncnt2_5	*/ 
/*	 Fields trcnvstmoncnt	 */
#define TRCNVSTMONCNT5_F4_WIDTH                                              32
#define TRCNVSTMONCNT5_F4_SHIFT                                               0
#define TRCNVSTMONCNT5_F4_MASK                                       0xffffffff
#define TRCNVSTMONCNT5_F4_RD(src)                        (((src) & 0xffffffff))
#define TRCNVSTMONCNT5_F4_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register trcnvstmon2_6	*/ 
/*	 Fields trcnvstmon	 */
#define TRCNVSTMON6_F2_WIDTH                                                 12
#define TRCNVSTMON6_F2_SHIFT                                                  0
#define TRCNVSTMON6_F2_MASK                                          0x00000fff
#define TRCNVSTMON6_F2_RD(src)                           (((src) & 0x00000fff))
#define TRCNVSTMON6_F2_WR(src)                      (((u32)(src)) & 0x00000fff)
#define TRCNVSTMON6_F2_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register trcnvstmoncnt2_6	*/ 
/*	 Fields trcnvstmoncnt	 */
#define TRCNVSTMONCNT6_F2_WIDTH                                              32
#define TRCNVSTMONCNT6_F2_SHIFT                                               0
#define TRCNVSTMONCNT6_F2_MASK                                       0xffffffff
#define TRCNVSTMONCNT6_F2_RD(src)                        (((src) & 0xffffffff))
#define TRCNVSTMONCNT6_F2_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register trcnvstmon2_7	*/ 
/*	 Fields trcnvstmon	 */
#define TRCNVSTMON7_F2_WIDTH                                                 12
#define TRCNVSTMON7_F2_SHIFT                                                  0
#define TRCNVSTMON7_F2_MASK                                          0x00000fff
#define TRCNVSTMON7_F2_RD(src)                           (((src) & 0x00000fff))
#define TRCNVSTMON7_F2_WR(src)                      (((u32)(src)) & 0x00000fff)
#define TRCNVSTMON7_F2_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register trcnvstmoncnt2_7	*/ 
/*	 Fields trcnvstmoncnt	 */
#define TRCNVSTMONCNT7_F2_WIDTH                                              32
#define TRCNVSTMONCNT7_F2_SHIFT                                               0
#define TRCNVSTMONCNT7_F2_MASK                                       0xffffffff
#define TRCNVSTMONCNT7_F2_RD(src)                        (((src) & 0xffffffff))
#define TRCNVSTMONCNT7_F2_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register trcnvstmon2_8	*/ 
/*	 Fields trcnvstmon	 */
#define TRCNVSTMON8_F2_WIDTH                                                 12
#define TRCNVSTMON8_F2_SHIFT                                                  0
#define TRCNVSTMON8_F2_MASK                                          0x00000fff
#define TRCNVSTMON8_F2_RD(src)                           (((src) & 0x00000fff))
#define TRCNVSTMON8_F2_WR(src)                      (((u32)(src)) & 0x00000fff)
#define TRCNVSTMON8_F2_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register trcnvstmoncnt2_8	*/ 
/*	 Fields trcnvstmoncnt	 */
#define TRCNVSTMONCNT8_F2_WIDTH                                              32
#define TRCNVSTMONCNT8_F2_SHIFT                                               0
#define TRCNVSTMONCNT8_F2_MASK                                       0xffffffff
#define TRCNVSTMONCNT8_F2_RD(src)                        (((src) & 0xffffffff))
#define TRCNVSTMONCNT8_F2_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register trcnvstmon2_9	*/ 
/*	 Fields trcnvstmon	 */
#define TRCNVSTMON9_F2_WIDTH                                                 12
#define TRCNVSTMON9_F2_SHIFT                                                  0
#define TRCNVSTMON9_F2_MASK                                          0x00000fff
#define TRCNVSTMON9_F2_RD(src)                           (((src) & 0x00000fff))
#define TRCNVSTMON9_F2_WR(src)                      (((u32)(src)) & 0x00000fff)
#define TRCNVSTMON9_F2_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register trcnvstmoncnt2_9	*/ 
/*	 Fields trcnvstmoncnt	 */
#define TRCNVSTMONCNT9_F2_WIDTH                                              32
#define TRCNVSTMONCNT9_F2_SHIFT                                               0
#define TRCNVSTMONCNT9_F2_MASK                                       0xffffffff
#define TRCNVSTMONCNT9_F2_RD(src)                        (((src) & 0xffffffff))
#define TRCNVSTMONCNT9_F2_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register trcnvstmon2_10	*/ 
/*	 Fields trcnvstmon	 */
#define TRCNVSTMON0_F5_WIDTH                                                 12
#define TRCNVSTMON0_F5_SHIFT                                                  0
#define TRCNVSTMON0_F5_MASK                                          0x00000fff
#define TRCNVSTMON0_F5_RD(src)                           (((src) & 0x00000fff))
#define TRCNVSTMON0_F5_WR(src)                      (((u32)(src)) & 0x00000fff)
#define TRCNVSTMON0_F5_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register trcnvstmoncnt2_10	*/ 
/*	 Fields trcnvstmoncnt	 */
#define TRCNVSTMONCNT0_F5_WIDTH                                              32
#define TRCNVSTMONCNT0_F5_SHIFT                                               0
#define TRCNVSTMONCNT0_F5_MASK                                       0xffffffff
#define TRCNVSTMONCNT0_F5_RD(src)                        (((src) & 0xffffffff))
#define TRCNVSTMONCNT0_F5_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register trcnvstmon2_11	*/ 
/*	 Fields trcnvstmon	 */
#define TRCNVSTMON1_F5_WIDTH                                                 12
#define TRCNVSTMON1_F5_SHIFT                                                  0
#define TRCNVSTMON1_F5_MASK                                          0x00000fff
#define TRCNVSTMON1_F5_RD(src)                           (((src) & 0x00000fff))
#define TRCNVSTMON1_F5_WR(src)                      (((u32)(src)) & 0x00000fff)
#define TRCNVSTMON1_F5_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register trcnvstmoncnt2_11	*/ 
/*	 Fields trcnvstmoncnt	 */
#define TRCNVSTMONCNT1_F5_WIDTH                                              32
#define TRCNVSTMONCNT1_F5_SHIFT                                               0
#define TRCNVSTMONCNT1_F5_MASK                                       0xffffffff
#define TRCNVSTMONCNT1_F5_RD(src)                        (((src) & 0xffffffff))
#define TRCNVSTMONCNT1_F5_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register trcnvstmon2_12	*/ 
/*	 Fields trcnvstmon	 */
#define TRCNVSTMON2_F5_WIDTH                                                 12
#define TRCNVSTMON2_F5_SHIFT                                                  0
#define TRCNVSTMON2_F5_MASK                                          0x00000fff
#define TRCNVSTMON2_F5_RD(src)                           (((src) & 0x00000fff))
#define TRCNVSTMON2_F5_WR(src)                      (((u32)(src)) & 0x00000fff)
#define TRCNVSTMON2_F5_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register trcnvstmoncnt2_12	*/ 
/*	 Fields trcnvstmoncnt	 */
#define TRCNVSTMONCNT2_F5_WIDTH                                              32
#define TRCNVSTMONCNT2_F5_SHIFT                                               0
#define TRCNVSTMONCNT2_F5_MASK                                       0xffffffff
#define TRCNVSTMONCNT2_F5_RD(src)                        (((src) & 0xffffffff))
#define TRCNVSTMONCNT2_F5_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register trcnvstmon2_13	*/ 
/*	 Fields trcnvstmon	 */
#define TRCNVSTMON3_F5_WIDTH                                                 12
#define TRCNVSTMON3_F5_SHIFT                                                  0
#define TRCNVSTMON3_F5_MASK                                          0x00000fff
#define TRCNVSTMON3_F5_RD(src)                           (((src) & 0x00000fff))
#define TRCNVSTMON3_F5_WR(src)                      (((u32)(src)) & 0x00000fff)
#define TRCNVSTMON3_F5_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register trcnvstmoncnt2_13	*/ 
/*	 Fields trcnvstmoncnt	 */
#define TRCNVSTMONCNT3_F5_WIDTH                                              32
#define TRCNVSTMONCNT3_F5_SHIFT                                               0
#define TRCNVSTMONCNT3_F5_MASK                                       0xffffffff
#define TRCNVSTMONCNT3_F5_RD(src)                        (((src) & 0xffffffff))
#define TRCNVSTMONCNT3_F5_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register trcnvstmon2_14	*/ 
/*	 Fields trcnvstmon	 */
#define TRCNVSTMON4_F5_WIDTH                                                 12
#define TRCNVSTMON4_F5_SHIFT                                                  0
#define TRCNVSTMON4_F5_MASK                                          0x00000fff
#define TRCNVSTMON4_F5_RD(src)                           (((src) & 0x00000fff))
#define TRCNVSTMON4_F5_WR(src)                      (((u32)(src)) & 0x00000fff)
#define TRCNVSTMON4_F5_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register trcnvstmoncnt2_14	*/ 
/*	 Fields trcnvstmoncnt	 */
#define TRCNVSTMONCNT4_F5_WIDTH                                              32
#define TRCNVSTMONCNT4_F5_SHIFT                                               0
#define TRCNVSTMONCNT4_F5_MASK                                       0xffffffff
#define TRCNVSTMONCNT4_F5_RD(src)                        (((src) & 0xffffffff))
#define TRCNVSTMONCNT4_F5_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register trcnvstmon2_15	*/ 
/*	 Fields trcnvstmon	 */
#define TRCNVSTMON5_F5_WIDTH                                                 12
#define TRCNVSTMON5_F5_SHIFT                                                  0
#define TRCNVSTMON5_F5_MASK                                          0x00000fff
#define TRCNVSTMON5_F5_RD(src)                           (((src) & 0x00000fff))
#define TRCNVSTMON5_F5_WR(src)                      (((u32)(src)) & 0x00000fff)
#define TRCNVSTMON5_F5_SET(dst,src) \
                          (((dst) & ~0x00000fff) | (((u32)(src)) & 0x00000fff))

/*	Register trcnvstmoncnt2_15	*/ 
/*	 Fields trcnvstmoncnt	 */
#define TRCNVSTMONCNT5_F5_WIDTH                                              32
#define TRCNVSTMONCNT5_F5_SHIFT                                               0
#define TRCNVSTMONCNT5_F5_MASK                                       0xffffffff
#define TRCNVSTMONCNT5_F5_RD(src)                        (((src) & 0xffffffff))
#define TRCNVSTMONCNT5_F5_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register tsif_cfg_ctl	*/ 
/*	 Fields wmsg_swap	 */
#define WMSG_SWAP_WIDTH                                                       1
#define WMSG_SWAP_SHIFT                                                      10
#define WMSG_SWAP_MASK                                               0x00000400
#define WMSG_SWAP_RD(src)                            (((src) & 0x00000400)>>10)
#define WMSG_SWAP_WR(src)                       (((u32)(src)<<10) & 0x00000400)
#define WMSG_SWAP_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields big_endian	 */
#define BIG_ENDIAN_WIDTH                                                      1
#define BIG_ENDIAN_F0_SHIFT                                                   9
#define BIG_ENDIAN_F0_MASK                                           0x00000200
#define BIG_ENDIAN_F0_RD(src)                         (((src) & 0x00000200)>>9)
#define BIG_ENDIAN_F0_WR(src)                    (((u32)(src)<<9) & 0x00000200)
#define BIG_ENDIAN_F0_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields sos_eos_perbuffer	 */
#define SOS_EOS_PERBUFFER_WIDTH                                               1
#define SOS_EOS_PERBUFFER_SHIFT                                               8
#define SOS_EOS_PERBUFFER_MASK                                       0x00000100
#define SOS_EOS_PERBUFFER_RD(src)                     (((src) & 0x00000100)>>8)
#define SOS_EOS_PERBUFFER_WR(src)                (((u32)(src)<<8) & 0x00000100)
#define SOS_EOS_PERBUFFER_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*	 Fields cmsg_wmsg	 */
#define CMSG_WMSG_WIDTH                                                       1
#define CMSG_WMSG_SHIFT                                                       7
#define CMSG_WMSG_MASK                                               0x00000080
#define CMSG_WMSG_RD(src)                             (((src) & 0x00000080)>>7)
#define CMSG_WMSG_WR(src)                        (((u32)(src)<<7) & 0x00000080)
#define CMSG_WMSG_SET(dst,src) \
                       (((dst) & ~0x00000080) | (((u32)(src)<<7) & 0x00000080))
/*	 Fields dealloc_wmsg	 */
#define DEALLOC_WMSG_WIDTH                                                    1
#define DEALLOC_WMSG_SHIFT                                                    6
#define DEALLOC_WMSG_MASK                                            0x00000040
#define DEALLOC_WMSG_RD(src)                          (((src) & 0x00000040)>>6)
#define DEALLOC_WMSG_WR(src)                     (((u32)(src)<<6) & 0x00000040)
#define DEALLOC_WMSG_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*	 Fields rdm_threshold	 */
#define RDM_THRESHOLD_WIDTH                                                   3
#define RDM_THRESHOLD_SHIFT                                                   3
#define RDM_THRESHOLD_MASK                                           0x00000038
#define RDM_THRESHOLD_RD(src)                         (((src) & 0x00000038)>>3)
#define RDM_THRESHOLD_WR(src)                    (((u32)(src)<<3) & 0x00000038)
#define RDM_THRESHOLD_SET(dst,src) \
                       (((dst) & ~0x00000038) | (((u32)(src)<<3) & 0x00000038))
/*	 Fields rrm_threshold	 */
#define RRM_THRESHOLD_WIDTH                                                   3
#define RRM_THRESHOLD_SHIFT                                                   0
#define RRM_THRESHOLD_MASK                                           0x00000007
#define RRM_THRESHOLD_RD(src)                            (((src) & 0x00000007))
#define RRM_THRESHOLD_WR(src)                       (((u32)(src)) & 0x00000007)
#define RRM_THRESHOLD_SET(dst,src) \
                          (((dst) & ~0x00000007) | (((u32)(src)) & 0x00000007))

/*	Register rsif_cfg_ctl	*/ 
/*	 Fields fpbuff_timeout_en	 */
#define FPBUFF_TIMEOUT_EN_WIDTH                                               1
#define FPBUFF_TIMEOUT_EN_SHIFT                                              31
#define FPBUFF_TIMEOUT_EN_MASK                                       0x80000000
#define FPBUFF_TIMEOUT_EN_RD(src)                    (((src) & 0x80000000)>>31)
#define FPBUFF_TIMEOUT_EN_WR(src)               (((u32)(src)<<31) & 0x80000000)
#define FPBUFF_TIMEOUT_EN_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields fpbuff_timeout	 */
#define FPBUFF_TIMEOUT_WIDTH                                                  7
#define FPBUFF_TIMEOUT_SHIFT                                                 24
#define FPBUFF_TIMEOUT_MASK                                          0x7f000000
#define FPBUFF_TIMEOUT_RD(src)                       (((src) & 0x7f000000)>>24)
#define FPBUFF_TIMEOUT_WR(src)                  (((u32)(src)<<24) & 0x7f000000)
#define FPBUFF_TIMEOUT_SET(dst,src) \
                      (((dst) & ~0x7f000000) | (((u32)(src)<<24) & 0x7f000000))
/*       Fields cfg_rsif_rtype   */
#define CFG_RSIF_RTYPE_WIDTH                                                  5
#define CFG_RSIF_RTYPE_SHIFT                                                 19
#define CFG_RSIF_RTYPE_MASK                                          0x00f80000
#define CFG_RSIF_RTYPE_RD(src)                       (((src) & 0x00f80000)>>19)
#define CFG_RSIF_RTYPE_WR(src)                  (((u32)(src)<<19) & 0x00f80000)
#define CFG_RSIF_RTYPE_SET(dst,src) \
                      (((dst) & ~0x00f80000) | (((u32)(src)<<19) & 0x00f80000))
/*       Fields fpbuff_drop_en   */
#define FPBUFF_DROP_EN_WIDTH                                                  1
#define FPBUFF_DROP_EN_SHIFT                                                 17
#define FPBUFF_DROP_EN_MASK                                          0x00020000
#define FPBUFF_DROP_EN_RD(src)        1               (((src) & 0x00020000)>>17)
#define FPBUFF_DROP_EN_WR(src)                  (((u32)(src)<<17) & 0x00020000)
#define FPBUFF_DROP_EN_SET(dst,src) \
                      (((dst) & ~0x00020000) | (((u32)(src)<<17) & 0x00020000))
/*	 Fields big_endian	 */
#define BIG_ENDIAN_F1_WIDTH                                                   1
#define BIG_ENDIAN_F1_SHIFT                                                  20
#define BIG_ENDIAN_F1_MASK                                           0x00100000
#define BIG_ENDIAN_F1_RD(src)                        (((src) & 0x00100000)>>20)
#define BIG_ENDIAN_F1_WR(src)                   (((u32)(src)<<20) & 0x00100000)
#define BIG_ENDIAN_F1_SET(dst,src) \
                      (((dst) & ~0x00100000) | (((u32)(src)<<20) & 0x00100000))
/*	 Fields buffer_threshold	 */
#define BUFFER_THRESHOLD_WIDTH                                                5
#define BUFFER_THRESHOLD_SHIFT                                               12
#define BUFFER_THRESHOLD_MASK                                        0x0001f000
#define BUFFER_THRESHOLD_RD(src)                     (((src) & 0x0001f000)>>12)
#define BUFFER_THRESHOLD_WR(src)                (((u32)(src)<<12) & 0x0001f000)
#define BUFFER_THRESHOLD_SET(dst,src) \
                      (((dst) & ~0x0001f000) | (((u32)(src)<<12) & 0x0001f000))
/*	 Fields ctrl_buffer_threshold	 */
#define CTRL_BUFFER_THRESHOLD_WIDTH                                           5
#define CTRL_BUFFER_THRESHOLD_SHIFT                                           4
#define CTRL_BUFFER_THRESHOLD_MASK                                   0x000001f0
#define CTRL_BUFFER_THRESHOLD_RD(src)                 (((src) & 0x000001f0)>>4)
#define CTRL_BUFFER_THRESHOLD_WR(src)            (((u32)(src)<<4) & 0x000001f0)
#define CTRL_BUFFER_THRESHOLD_SET(dst,src) \
                       (((dst) & ~0x000001f0) | (((u32)(src)<<4) & 0x000001f0))
/*	 Fields cle_buffer_threshold	 */
#define CLE_BUFFER_THRESHOLD_WIDTH                                            3
#define CLE_BUFFER_THRESHOLD_SHIFT                                            0
#define CLE_BUFFER_THRESHOLD_MASK                                    0x00000007
#define CLE_BUFFER_THRESHOLD_RD(src)                     (((src) & 0x00000007))
#define CLE_BUFFER_THRESHOLD_WR(src)                (((u32)(src)) & 0x00000007)
#define CLE_BUFFER_THRESHOLD_SET(dst,src) \
                          (((dst) & ~0x00000007) | (((u32)(src)) & 0x00000007))

/*	Register CLE_RSIF_Policer_StsReg0	*/ 
/*	 Fields sts_rsif_plc_total_dropcnt	 */
#define STS_RSIF_PLC_TOTAL_DROPCNT0_WIDTH                                    32
#define STS_RSIF_PLC_TOTAL_DROPCNT0_SHIFT                                     0
#define STS_RSIF_PLC_TOTAL_DROPCNT0_MASK                             0xffffffff
#define STS_RSIF_PLC_TOTAL_DROPCNT0_RD(src)              (((src) & 0xffffffff))
#define STS_RSIF_PLC_TOTAL_DROPCNT0_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CLE_RSIF_Priority_Policer_Reg0	*/ 
/*	 Fields cfg_rsif_plc0_en	 */
#define CFG_RSIF_PLC0_EN0_WIDTH                                               1
#define CFG_RSIF_PLC0_EN0_SHIFT                                              24
#define CFG_RSIF_PLC0_EN0_MASK                                       0x01000000
#define CFG_RSIF_PLC0_EN0_RD(src)                    (((src) & 0x01000000)>>24)
#define CFG_RSIF_PLC0_EN0_WR(src)               (((u32)(src)<<24) & 0x01000000)
#define CFG_RSIF_PLC0_EN0_SET(dst,src) \
                      (((dst) & ~0x01000000) | (((u32)(src)<<24) & 0x01000000))
/*	 Fields cfg_rsif_plc0_addcredits	 */
#define CFG_RSIF_PLC0_ADDCREDITS0_WIDTH                                      24
#define CFG_RSIF_PLC0_ADDCREDITS0_SHIFT                                       0
#define CFG_RSIF_PLC0_ADDCREDITS0_MASK                               0x00ffffff
#define CFG_RSIF_PLC0_ADDCREDITS0_RD(src)                (((src) & 0x00ffffff))
#define CFG_RSIF_PLC0_ADDCREDITS0_WR(src)           (((u32)(src)) & 0x00ffffff)
#define CFG_RSIF_PLC0_ADDCREDITS0_SET(dst,src) \
                          (((dst) & ~0x00ffffff) | (((u32)(src)) & 0x00ffffff))

/*	Register CLE_RSIF_Priority_Policer_Reg1	*/ 
/*	 Fields cfg_rsif_plc_credit_update_time	 */
#define CFG_RSIF_PLC_CREDIT_UPDATE_TIME1_WIDTH                                7
#define CFG_RSIF_PLC_CREDIT_UPDATE_TIME1_SHIFT                               24
#define CFG_RSIF_PLC_CREDIT_UPDATE_TIME1_MASK                        0x7f000000
#define CFG_RSIF_PLC_CREDIT_UPDATE_TIME1_RD(src)     (((src) & 0x7f000000)>>24)
#define CFG_RSIF_PLC_CREDIT_UPDATE_TIME1_WR(src) \
                                                (((u32)(src)<<24) & 0x7f000000)
#define CFG_RSIF_PLC_CREDIT_UPDATE_TIME1_SET(dst,src) \
                      (((dst) & ~0x7f000000) | (((u32)(src)<<24) & 0x7f000000))
/*	 Fields cfg_rsif_plc0_hicredits	 */
#define CFG_RSIF_PLC0_HICREDITS1_WIDTH                                       24
#define CFG_RSIF_PLC0_HICREDITS1_SHIFT                                        0
#define CFG_RSIF_PLC0_HICREDITS1_MASK                                0x00ffffff
#define CFG_RSIF_PLC0_HICREDITS1_RD(src)                 (((src) & 0x00ffffff))
#define CFG_RSIF_PLC0_HICREDITS1_WR(src)            (((u32)(src)) & 0x00ffffff)
#define CFG_RSIF_PLC0_HICREDITS1_SET(dst,src) \
                          (((dst) & ~0x00ffffff) | (((u32)(src)) & 0x00ffffff))

/*	Register CLE_RSIF_Priority_Policer_Reg2	*/ 
/*	 Fields cfg_rsif_plc0_threshold	 */
#define CFG_RSIF_PLC0_THRESHOLD2_WIDTH                                       24
#define CFG_RSIF_PLC0_THRESHOLD2_SHIFT                                        0
#define CFG_RSIF_PLC0_THRESHOLD2_MASK                                0x00ffffff
#define CFG_RSIF_PLC0_THRESHOLD2_RD(src)                 (((src) & 0x00ffffff))
#define CFG_RSIF_PLC0_THRESHOLD2_WR(src)            (((u32)(src)) & 0x00ffffff)
#define CFG_RSIF_PLC0_THRESHOLD2_SET(dst,src) \
                          (((dst) & ~0x00ffffff) | (((u32)(src)) & 0x00ffffff))

/*	Register CLE_RSIF_Flowgroup_Policer_Reg0	*/ 
/*	 Fields cfg_rsif_plc1_en	 */
#define CFG_RSIF_PLC1_EN0_WIDTH                                               1
#define CFG_RSIF_PLC1_EN0_SHIFT                                              24
#define CFG_RSIF_PLC1_EN0_MASK                                       0x01000000
#define CFG_RSIF_PLC1_EN0_RD(src)                    (((src) & 0x01000000)>>24)
#define CFG_RSIF_PLC1_EN0_WR(src)               (((u32)(src)<<24) & 0x01000000)
#define CFG_RSIF_PLC1_EN0_SET(dst,src) \
                      (((dst) & ~0x01000000) | (((u32)(src)<<24) & 0x01000000))
/*	 Fields cfg_rsif_plc1_addcredits	 */
#define CFG_RSIF_PLC1_ADDCREDITS0_WIDTH                                      24
#define CFG_RSIF_PLC1_ADDCREDITS0_SHIFT                                       0
#define CFG_RSIF_PLC1_ADDCREDITS0_MASK                               0x00ffffff
#define CFG_RSIF_PLC1_ADDCREDITS0_RD(src)                (((src) & 0x00ffffff))
#define CFG_RSIF_PLC1_ADDCREDITS0_WR(src)           (((u32)(src)) & 0x00ffffff)
#define CFG_RSIF_PLC1_ADDCREDITS0_SET(dst,src) \
                          (((dst) & ~0x00ffffff) | (((u32)(src)) & 0x00ffffff))

/*	Register CLE_RSIF_Flowgroup_Policer_Reg1	*/ 
/*	 Fields cfg_rsif_plc1_hicredits	 */
#define CFG_RSIF_PLC1_HICREDITS1_WIDTH                                       24
#define CFG_RSIF_PLC1_HICREDITS1_SHIFT                                        0
#define CFG_RSIF_PLC1_HICREDITS1_MASK                                0x00ffffff
#define CFG_RSIF_PLC1_HICREDITS1_RD(src)                 (((src) & 0x00ffffff))
#define CFG_RSIF_PLC1_HICREDITS1_WR(src)            (((u32)(src)) & 0x00ffffff)
#define CFG_RSIF_PLC1_HICREDITS1_SET(dst,src) \
                          (((dst) & ~0x00ffffff) | (((u32)(src)) & 0x00ffffff))

/*	Register CLE_RSIF_Flowgroup_Policer_Reg2	*/ 
/*	 Fields cfg_rsif_plc1_threshold	 */
#define CFG_RSIF_PLC1_THRESHOLD2_WIDTH                                       24
#define CFG_RSIF_PLC1_THRESHOLD2_SHIFT                                        0
#define CFG_RSIF_PLC1_THRESHOLD2_MASK                                0x00ffffff
#define CFG_RSIF_PLC1_THRESHOLD2_RD(src)                 (((src) & 0x00ffffff))
#define CFG_RSIF_PLC1_THRESHOLD2_WR(src)            (((u32)(src)) & 0x00ffffff)
#define CFG_RSIF_PLC1_THRESHOLD2_SET(dst,src) \
                          (((dst) & ~0x00ffffff) | (((u32)(src)) & 0x00ffffff))

/*	Register CLE_RSIF_Perflow_Policer_Reg0	*/ 
/*	 Fields cfg_rsif_plc2_en	 */
#define CFG_RSIF_PLC2_EN0_WIDTH                                               1
#define CFG_RSIF_PLC2_EN0_SHIFT                                              24
#define CFG_RSIF_PLC2_EN0_MASK                                       0x01000000
#define CFG_RSIF_PLC2_EN0_RD(src)                    (((src) & 0x01000000)>>24)
#define CFG_RSIF_PLC2_EN0_WR(src)               (((u32)(src)<<24) & 0x01000000)
#define CFG_RSIF_PLC2_EN0_SET(dst,src) \
                      (((dst) & ~0x01000000) | (((u32)(src)<<24) & 0x01000000))
/*	 Fields cfg_rsif_plc2_addcredits	 */
#define CFG_RSIF_PLC2_ADDCREDITS0_WIDTH                                      24
#define CFG_RSIF_PLC2_ADDCREDITS0_SHIFT                                       0
#define CFG_RSIF_PLC2_ADDCREDITS0_MASK                               0x00ffffff
#define CFG_RSIF_PLC2_ADDCREDITS0_RD(src)                (((src) & 0x00ffffff))
#define CFG_RSIF_PLC2_ADDCREDITS0_WR(src)           (((u32)(src)) & 0x00ffffff)
#define CFG_RSIF_PLC2_ADDCREDITS0_SET(dst,src) \
                          (((dst) & ~0x00ffffff) | (((u32)(src)) & 0x00ffffff))

/*	Register CLE_RSIF_Perflow_Policer_Reg1	*/ 
/*	 Fields cfg_rsif_plc2_hicredits	 */
#define CFG_RSIF_PLC2_HICREDITS1_WIDTH                                       24
#define CFG_RSIF_PLC2_HICREDITS1_SHIFT                                        0
#define CFG_RSIF_PLC2_HICREDITS1_MASK                                0x00ffffff
#define CFG_RSIF_PLC2_HICREDITS1_RD(src)                 (((src) & 0x00ffffff))
#define CFG_RSIF_PLC2_HICREDITS1_WR(src)            (((u32)(src)) & 0x00ffffff)
#define CFG_RSIF_PLC2_HICREDITS1_SET(dst,src) \
                          (((dst) & ~0x00ffffff) | (((u32)(src)) & 0x00ffffff))

/*	Register CLE_RSIF_Perflow_Policer_Reg2	*/ 
/*	 Fields cfg_rsif_plc2_threshold	 */
#define CFG_RSIF_PLC2_THRESHOLD2_WIDTH                                       24
#define CFG_RSIF_PLC2_THRESHOLD2_SHIFT                                        0
#define CFG_RSIF_PLC2_THRESHOLD2_MASK                                0x00ffffff
#define CFG_RSIF_PLC2_THRESHOLD2_RD(src)                 (((src) & 0x00ffffff))
#define CFG_RSIF_PLC2_THRESHOLD2_WR(src)            (((u32)(src)) & 0x00ffffff)
#define CFG_RSIF_PLC2_THRESHOLD2_SET(dst,src) \
                          (((dst) & ~0x00ffffff) | (((u32)(src)) & 0x00ffffff))

/*	Register CLE_RSIF_Priority_Policer_StsReg0	*/ 
/*	 Fields sts_rsif_plc0_dropcnt	 */
#define STS_RSIF_PLC0_DROPCNT0_WIDTH                                         32
#define STS_RSIF_PLC0_DROPCNT0_SHIFT                                          0
#define STS_RSIF_PLC0_DROPCNT0_MASK                                  0xffffffff
#define STS_RSIF_PLC0_DROPCNT0_RD(src)                   (((src) & 0xffffffff))
#define STS_RSIF_PLC0_DROPCNT0_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CLE_RSIF_Flowgroup_Policer_StsReg0	*/ 
/*	 Fields sts_rsif_plc1_dropcnt	 */
#define STS_RSIF_PLC1_DROPCNT0_WIDTH                                         32
#define STS_RSIF_PLC1_DROPCNT0_SHIFT                                          0
#define STS_RSIF_PLC1_DROPCNT0_MASK                                  0xffffffff
#define STS_RSIF_PLC1_DROPCNT0_RD(src)                   (((src) & 0xffffffff))
#define STS_RSIF_PLC1_DROPCNT0_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CLE_RSIF_Perflow_Policer_StsReg0	*/ 
/*	 Fields sts_rsif_plc2_dropcnt	 */
#define STS_RSIF_PLC2_DROPCNT0_WIDTH                                         32
#define STS_RSIF_PLC2_DROPCNT0_SHIFT                                          0
#define STS_RSIF_PLC2_DROPCNT0_MASK                                  0xffffffff
#define STS_RSIF_PLC2_DROPCNT0_RD(src)                   (((src) & 0xffffffff))
#define STS_RSIF_PLC2_DROPCNT0_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CLE_RSIF_FIFO_EmptySts0	*/ 
/*	 Fields rsif_chksum_buff_fifo_empty	 */
#define RSIF_CHKSUM_BUFF_FIFO_EMPTY0_WIDTH                                    1
#define RSIF_CHKSUM_BUFF_FIFO_EMPTY0_SHIFT                                    5
#define RSIF_CHKSUM_BUFF_FIFO_EMPTY0_MASK                            0x00000020
#define RSIF_CHKSUM_BUFF_FIFO_EMPTY0_RD(src)          (((src) & 0x00000020)>>5)
#define RSIF_CHKSUM_BUFF_FIFO_EMPTY0_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields rsif_timestamp_buff_fifo_empty	 */
#define RSIF_TIMESTAMP_BUFF_FIFO_EMPTY0_WIDTH                                 1
#define RSIF_TIMESTAMP_BUFF_FIFO_EMPTY0_SHIFT                                 4
#define RSIF_TIMESTAMP_BUFF_FIFO_EMPTY0_MASK                         0x00000010
#define RSIF_TIMESTAMP_BUFF_FIFO_EMPTY0_RD(src)       (((src) & 0x00000010)>>4)
#define RSIF_TIMESTAMP_BUFF_FIFO_EMPTY0_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields rsif_err_buff_fifo_empty	 */
#define RSIF_ERR_BUFF_FIFO_EMPTY0_WIDTH                                       1
#define RSIF_ERR_BUFF_FIFO_EMPTY0_SHIFT                                       3
#define RSIF_ERR_BUFF_FIFO_EMPTY0_MASK                               0x00000008
#define RSIF_ERR_BUFF_FIFO_EMPTY0_RD(src)             (((src) & 0x00000008)>>3)
#define RSIF_ERR_BUFF_FIFO_EMPTY0_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields rsif_clebuff_fifo_empty	 */
#define RSIF_CLEBUFF_FIFO_EMPTY0_WIDTH                                        1
#define RSIF_CLEBUFF_FIFO_EMPTY0_SHIFT                                        2
#define RSIF_CLEBUFF_FIFO_EMPTY0_MASK                                0x00000004
#define RSIF_CLEBUFF_FIFO_EMPTY0_RD(src)              (((src) & 0x00000004)>>2)
#define RSIF_CLEBUFF_FIFO_EMPTY0_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields rsif_ctrlbuff_fifo_empty	 */
#define RSIF_CTRLBUFF_FIFO_EMPTY0_WIDTH                                       1
#define RSIF_CTRLBUFF_FIFO_EMPTY0_SHIFT                                       1
#define RSIF_CTRLBUFF_FIFO_EMPTY0_MASK                               0x00000002
#define RSIF_CTRLBUFF_FIFO_EMPTY0_RD(src)             (((src) & 0x00000002)>>1)
#define RSIF_CTRLBUFF_FIFO_EMPTY0_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields rsif_buf_fifo_empty	 */
#define RSIF_BUF_FIFO_EMPTY0_WIDTH                                            1
#define RSIF_BUF_FIFO_EMPTY0_SHIFT                                            0
#define RSIF_BUF_FIFO_EMPTY0_MASK                                    0x00000001
#define RSIF_BUF_FIFO_EMPTY0_RD(src)                     (((src) & 0x00000001))
#define RSIF_BUF_FIFO_EMPTY0_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register CLE_RSIF_Int_Reg0	*/ 
/*	 Fields rsif_chksum_buff_fifo_overfl_intr	 */
#define RSIF_CHKSUM_BUFF_FIFO_OVERFL_INTR0_WIDTH                              1
#define RSIF_CHKSUM_BUFF_FIFO_OVERFL_INTR0_SHIFT                             11
#define RSIF_CHKSUM_BUFF_FIFO_OVERFL_INTR0_MASK                      0x00000800
#define RSIF_CHKSUM_BUFF_FIFO_OVERFL_INTR0_RD(src)   (((src) & 0x00000800)>>11)
#define RSIF_CHKSUM_BUFF_FIFO_OVERFL_INTR0_WR(src) \
                                                (((u32)(src)<<11) & 0x00000800)
#define RSIF_CHKSUM_BUFF_FIFO_OVERFL_INTR0_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*	 Fields rsif_chksum_buff_fifo_underfl_intr	 */
#define RSIF_CHKSUM_BUFF_FIFO_UNDERFL_INTR0_WIDTH                             1
#define RSIF_CHKSUM_BUFF_FIFO_UNDERFL_INTR0_SHIFT                            10
#define RSIF_CHKSUM_BUFF_FIFO_UNDERFL_INTR0_MASK                     0x00000400
#define RSIF_CHKSUM_BUFF_FIFO_UNDERFL_INTR0_RD(src)  (((src) & 0x00000400)>>10)
#define RSIF_CHKSUM_BUFF_FIFO_UNDERFL_INTR0_WR(src) \
                                                (((u32)(src)<<10) & 0x00000400)
#define RSIF_CHKSUM_BUFF_FIFO_UNDERFL_INTR0_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields rsif_timestamp_buff_fifo_overfl_intr	 */
#define RSIF_TIMESTAMP_BUFF_FIFO_OVERFL_INTR0_WIDTH                           1
#define RSIF_TIMESTAMP_BUFF_FIFO_OVERFL_INTR0_SHIFT                           9
#define RSIF_TIMESTAMP_BUFF_FIFO_OVERFL_INTR0_MASK                   0x00000200
#define RSIF_TIMESTAMP_BUFF_FIFO_OVERFL_INTR0_RD(src) \
                                                     (((src) & 0x00000200)>>9)
#define RSIF_TIMESTAMP_BUFF_FIFO_OVERFL_INTR0_WR(src) \
                                                 (((u32)(src)<<9) & 0x00000200)
#define RSIF_TIMESTAMP_BUFF_FIFO_OVERFL_INTR0_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields rsif_timestamp_buff_fifo_underfl_intr	 */
#define RSIF_TIMESTAMP_BUFF_FIFO_UNDERFL_INTR0_WIDTH                          1
#define RSIF_TIMESTAMP_BUFF_FIFO_UNDERFL_INTR0_SHIFT                          8
#define RSIF_TIMESTAMP_BUFF_FIFO_UNDERFL_INTR0_MASK                  0x00000100
#define RSIF_TIMESTAMP_BUFF_FIFO_UNDERFL_INTR0_RD(src) \
                                                     (((src) & 0x00000100)>>8)
#define RSIF_TIMESTAMP_BUFF_FIFO_UNDERFL_INTR0_WR(src) \
                                                 (((u32)(src)<<8) & 0x00000100)
#define RSIF_TIMESTAMP_BUFF_FIFO_UNDERFL_INTR0_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*	 Fields rsif_err_buff_fifo_overfl_intr	 */
#define RSIF_ERR_BUFF_FIFO_OVERFL_INTR0_WIDTH                                 1
#define RSIF_ERR_BUFF_FIFO_OVERFL_INTR0_SHIFT                                 7
#define RSIF_ERR_BUFF_FIFO_OVERFL_INTR0_MASK                         0x00000080
#define RSIF_ERR_BUFF_FIFO_OVERFL_INTR0_RD(src)       (((src) & 0x00000080)>>7)
#define RSIF_ERR_BUFF_FIFO_OVERFL_INTR0_WR(src)  (((u32)(src)<<7) & 0x00000080)
#define RSIF_ERR_BUFF_FIFO_OVERFL_INTR0_SET(dst,src) \
                       (((dst) & ~0x00000080) | (((u32)(src)<<7) & 0x00000080))
/*	 Fields rsif_err_buff_fifo_underfl_intr	 */
#define RSIF_ERR_BUFF_FIFO_UNDERFL_INTR0_WIDTH                                1
#define RSIF_ERR_BUFF_FIFO_UNDERFL_INTR0_SHIFT                                6
#define RSIF_ERR_BUFF_FIFO_UNDERFL_INTR0_MASK                        0x00000040
#define RSIF_ERR_BUFF_FIFO_UNDERFL_INTR0_RD(src)      (((src) & 0x00000040)>>6)
#define RSIF_ERR_BUFF_FIFO_UNDERFL_INTR0_WR(src) \
                                                 (((u32)(src)<<6) & 0x00000040)
#define RSIF_ERR_BUFF_FIFO_UNDERFL_INTR0_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*	 Fields rsif_clebuff_fifo_overfl_intr	 */
#define RSIF_CLEBUFF_FIFO_OVERFL_INTR0_WIDTH                                  1
#define RSIF_CLEBUFF_FIFO_OVERFL_INTR0_SHIFT                                  5
#define RSIF_CLEBUFF_FIFO_OVERFL_INTR0_MASK                          0x00000020
#define RSIF_CLEBUFF_FIFO_OVERFL_INTR0_RD(src)        (((src) & 0x00000020)>>5)
#define RSIF_CLEBUFF_FIFO_OVERFL_INTR0_WR(src)   (((u32)(src)<<5) & 0x00000020)
#define RSIF_CLEBUFF_FIFO_OVERFL_INTR0_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields rsif_clebuff_fifo_underfl_intr	 */
#define RSIF_CLEBUFF_FIFO_UNDERFL_INTR0_WIDTH                                 1
#define RSIF_CLEBUFF_FIFO_UNDERFL_INTR0_SHIFT                                 4
#define RSIF_CLEBUFF_FIFO_UNDERFL_INTR0_MASK                         0x00000010
#define RSIF_CLEBUFF_FIFO_UNDERFL_INTR0_RD(src)       (((src) & 0x00000010)>>4)
#define RSIF_CLEBUFF_FIFO_UNDERFL_INTR0_WR(src)  (((u32)(src)<<4) & 0x00000010)
#define RSIF_CLEBUFF_FIFO_UNDERFL_INTR0_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields rsif_ctrlbuff_fifo_overfl_intr	 */
#define RSIF_CTRLBUFF_FIFO_OVERFL_INTR0_WIDTH                                 1
#define RSIF_CTRLBUFF_FIFO_OVERFL_INTR0_SHIFT                                 3
#define RSIF_CTRLBUFF_FIFO_OVERFL_INTR0_MASK                         0x00000008
#define RSIF_CTRLBUFF_FIFO_OVERFL_INTR0_RD(src)       (((src) & 0x00000008)>>3)
#define RSIF_CTRLBUFF_FIFO_OVERFL_INTR0_WR(src)  (((u32)(src)<<3) & 0x00000008)
#define RSIF_CTRLBUFF_FIFO_OVERFL_INTR0_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields rsif_ctrlbuff_fifo_underfl_intr	 */
#define RSIF_CTRLBUFF_FIFO_UNDERFL_INTR0_WIDTH                                1
#define RSIF_CTRLBUFF_FIFO_UNDERFL_INTR0_SHIFT                                2
#define RSIF_CTRLBUFF_FIFO_UNDERFL_INTR0_MASK                        0x00000004
#define RSIF_CTRLBUFF_FIFO_UNDERFL_INTR0_RD(src)      (((src) & 0x00000004)>>2)
#define RSIF_CTRLBUFF_FIFO_UNDERFL_INTR0_WR(src) \
                                                 (((u32)(src)<<2) & 0x00000004)
#define RSIF_CTRLBUFF_FIFO_UNDERFL_INTR0_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields rsif_buf_fifo_overfl_intr	 */
#define RSIF_BUF_FIFO_OVERFL_INTR0_WIDTH                                      1
#define RSIF_BUF_FIFO_OVERFL_INTR0_SHIFT                                      1
#define RSIF_BUF_FIFO_OVERFL_INTR0_MASK                              0x00000002
#define RSIF_BUF_FIFO_OVERFL_INTR0_RD(src)            (((src) & 0x00000002)>>1)
#define RSIF_BUF_FIFO_OVERFL_INTR0_WR(src)       (((u32)(src)<<1) & 0x00000002)
#define RSIF_BUF_FIFO_OVERFL_INTR0_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields rsif_buf_fifo_underfl_intr	 */
#define RSIF_BUF_FIFO_UNDERFL_INTR0_WIDTH                                     1
#define RSIF_BUF_FIFO_UNDERFL_INTR0_SHIFT                                     0
#define RSIF_BUF_FIFO_UNDERFL_INTR0_MASK                             0x00000001
#define RSIF_BUF_FIFO_UNDERFL_INTR0_RD(src)              (((src) & 0x00000001))
#define RSIF_BUF_FIFO_UNDERFL_INTR0_WR(src)         (((u32)(src)) & 0x00000001)
#define RSIF_BUF_FIFO_UNDERFL_INTR0_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register CLE_RSIF_Int_Reg0Mask	*/
/*    Mask Register Fields rsif_chksum_buff_fifo_overfl_intrMask    */
#define RSIF_CHKSUM_BUFF_FIFO_OVERFL_INTRMASK_WIDTH                           1
#define RSIF_CHKSUM_BUFF_FIFO_OVERFL_INTRMASK_SHIFT                          11
#define RSIF_CHKSUM_BUFF_FIFO_OVERFL_INTRMASK_MASK                   0x00000800
#define RSIF_CHKSUM_BUFF_FIFO_OVERFL_INTRMASK_RD(src) \
                                                    (((src) & 0x00000800)>>11)
#define RSIF_CHKSUM_BUFF_FIFO_OVERFL_INTRMASK_WR(src) \
                                                (((u32)(src)<<11) & 0x00000800)
#define RSIF_CHKSUM_BUFF_FIFO_OVERFL_INTRMASK_SET(dst,src) \
                      (((dst) & ~0x00000800) | (((u32)(src)<<11) & 0x00000800))
/*    Mask Register Fields rsif_chksum_buff_fifo_underfl_intrMask    */
#define RSIF_CHKSUM_BUFF_FIFO_UNDERFL_INTRMASK_WIDTH                          1
#define RSIF_CHKSUM_BUFF_FIFO_UNDERFL_INTRMASK_SHIFT                         10
#define RSIF_CHKSUM_BUFF_FIFO_UNDERFL_INTRMASK_MASK                  0x00000400
#define RSIF_CHKSUM_BUFF_FIFO_UNDERFL_INTRMASK_RD(src) \
                                                    (((src) & 0x00000400)>>10)
#define RSIF_CHKSUM_BUFF_FIFO_UNDERFL_INTRMASK_WR(src) \
                                                (((u32)(src)<<10) & 0x00000400)
#define RSIF_CHKSUM_BUFF_FIFO_UNDERFL_INTRMASK_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*    Mask Register Fields rsif_timestamp_buff_fifo_overfl_intrMask    */
#define RSIF_TIMESTAMP_BUFF_FIFO_OVERFL_INTRMASK_WIDTH                        1
#define RSIF_TIMESTAMP_BUFF_FIFO_OVERFL_INTRMASK_SHIFT                        9
#define RSIF_TIMESTAMP_BUFF_FIFO_OVERFL_INTRMASK_MASK                0x00000200
#define RSIF_TIMESTAMP_BUFF_FIFO_OVERFL_INTRMASK_RD(src) \
                                                     (((src) & 0x00000200)>>9)
#define RSIF_TIMESTAMP_BUFF_FIFO_OVERFL_INTRMASK_WR(src) \
                                                 (((u32)(src)<<9) & 0x00000200)
#define RSIF_TIMESTAMP_BUFF_FIFO_OVERFL_INTRMASK_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*    Mask Register Fields rsif_timestamp_buff_fifo_underfl_intrMask    */
#define RSIF_TIMESTAMP_BUFF_FIFO_UNDERFL_INTRMASK_WIDTH                       1
#define RSIF_TIMESTAMP_BUFF_FIFO_UNDERFL_INTRMASK_SHIFT                       8
#define RSIF_TIMESTAMP_BUFF_FIFO_UNDERFL_INTRMASK_MASK               0x00000100
#define RSIF_TIMESTAMP_BUFF_FIFO_UNDERFL_INTRMASK_RD(src) \
                                                     (((src) & 0x00000100)>>8)
#define RSIF_TIMESTAMP_BUFF_FIFO_UNDERFL_INTRMASK_WR(src) \
                                                 (((u32)(src)<<8) & 0x00000100)
#define RSIF_TIMESTAMP_BUFF_FIFO_UNDERFL_INTRMASK_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*    Mask Register Fields rsif_err_buff_fifo_overfl_intrMask    */
#define RSIF_ERR_BUFF_FIFO_OVERFL_INTRMASK_WIDTH                              1
#define RSIF_ERR_BUFF_FIFO_OVERFL_INTRMASK_SHIFT                              7
#define RSIF_ERR_BUFF_FIFO_OVERFL_INTRMASK_MASK                      0x00000080
#define RSIF_ERR_BUFF_FIFO_OVERFL_INTRMASK_RD(src)    (((src) & 0x00000080)>>7)
#define RSIF_ERR_BUFF_FIFO_OVERFL_INTRMASK_WR(src) \
                                                 (((u32)(src)<<7) & 0x00000080)
#define RSIF_ERR_BUFF_FIFO_OVERFL_INTRMASK_SET(dst,src) \
                       (((dst) & ~0x00000080) | (((u32)(src)<<7) & 0x00000080))
/*    Mask Register Fields rsif_err_buff_fifo_underfl_intrMask    */
#define RSIF_ERR_BUFF_FIFO_UNDERFL_INTRMASK_WIDTH                             1
#define RSIF_ERR_BUFF_FIFO_UNDERFL_INTRMASK_SHIFT                             6
#define RSIF_ERR_BUFF_FIFO_UNDERFL_INTRMASK_MASK                     0x00000040
#define RSIF_ERR_BUFF_FIFO_UNDERFL_INTRMASK_RD(src)   (((src) & 0x00000040)>>6)
#define RSIF_ERR_BUFF_FIFO_UNDERFL_INTRMASK_WR(src) \
                                                 (((u32)(src)<<6) & 0x00000040)
#define RSIF_ERR_BUFF_FIFO_UNDERFL_INTRMASK_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*    Mask Register Fields rsif_clebuff_fifo_overfl_intrMask    */
#define RSIF_CLEBUFF_FIFO_OVERFL_INTRMASK_WIDTH                               1
#define RSIF_CLEBUFF_FIFO_OVERFL_INTRMASK_SHIFT                               5
#define RSIF_CLEBUFF_FIFO_OVERFL_INTRMASK_MASK                       0x00000020
#define RSIF_CLEBUFF_FIFO_OVERFL_INTRMASK_RD(src)     (((src) & 0x00000020)>>5)
#define RSIF_CLEBUFF_FIFO_OVERFL_INTRMASK_WR(src) \
                                                 (((u32)(src)<<5) & 0x00000020)
#define RSIF_CLEBUFF_FIFO_OVERFL_INTRMASK_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*    Mask Register Fields rsif_clebuff_fifo_underfl_intrMask    */
#define RSIF_CLEBUFF_FIFO_UNDERFL_INTRMASK_WIDTH                              1
#define RSIF_CLEBUFF_FIFO_UNDERFL_INTRMASK_SHIFT                              4
#define RSIF_CLEBUFF_FIFO_UNDERFL_INTRMASK_MASK                      0x00000010
#define RSIF_CLEBUFF_FIFO_UNDERFL_INTRMASK_RD(src)    (((src) & 0x00000010)>>4)
#define RSIF_CLEBUFF_FIFO_UNDERFL_INTRMASK_WR(src) \
                                                 (((u32)(src)<<4) & 0x00000010)
#define RSIF_CLEBUFF_FIFO_UNDERFL_INTRMASK_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*    Mask Register Fields rsif_ctrlbuff_fifo_overfl_intrMask    */
#define RSIF_CTRLBUFF_FIFO_OVERFL_INTRMASK_WIDTH                              1
#define RSIF_CTRLBUFF_FIFO_OVERFL_INTRMASK_SHIFT                              3
#define RSIF_CTRLBUFF_FIFO_OVERFL_INTRMASK_MASK                      0x00000008
#define RSIF_CTRLBUFF_FIFO_OVERFL_INTRMASK_RD(src)    (((src) & 0x00000008)>>3)
#define RSIF_CTRLBUFF_FIFO_OVERFL_INTRMASK_WR(src) \
                                                 (((u32)(src)<<3) & 0x00000008)
#define RSIF_CTRLBUFF_FIFO_OVERFL_INTRMASK_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*    Mask Register Fields rsif_ctrlbuff_fifo_underfl_intrMask    */
#define RSIF_CTRLBUFF_FIFO_UNDERFL_INTRMASK_WIDTH                             1
#define RSIF_CTRLBUFF_FIFO_UNDERFL_INTRMASK_SHIFT                             2
#define RSIF_CTRLBUFF_FIFO_UNDERFL_INTRMASK_MASK                     0x00000004
#define RSIF_CTRLBUFF_FIFO_UNDERFL_INTRMASK_RD(src)   (((src) & 0x00000004)>>2)
#define RSIF_CTRLBUFF_FIFO_UNDERFL_INTRMASK_WR(src) \
                                                 (((u32)(src)<<2) & 0x00000004)
#define RSIF_CTRLBUFF_FIFO_UNDERFL_INTRMASK_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*    Mask Register Fields rsif_buf_fifo_overfl_intrMask    */
#define RSIF_BUF_FIFO_OVERFL_INTRMASK_WIDTH                                   1
#define RSIF_BUF_FIFO_OVERFL_INTRMASK_SHIFT                                   1
#define RSIF_BUF_FIFO_OVERFL_INTRMASK_MASK                           0x00000002
#define RSIF_BUF_FIFO_OVERFL_INTRMASK_RD(src)         (((src) & 0x00000002)>>1)
#define RSIF_BUF_FIFO_OVERFL_INTRMASK_WR(src)    (((u32)(src)<<1) & 0x00000002)
#define RSIF_BUF_FIFO_OVERFL_INTRMASK_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*    Mask Register Fields rsif_buf_fifo_underfl_intrMask    */
#define RSIF_BUF_FIFO_UNDERFL_INTRMASK_WIDTH                                  1
#define RSIF_BUF_FIFO_UNDERFL_INTRMASK_SHIFT                                  0
#define RSIF_BUF_FIFO_UNDERFL_INTRMASK_MASK                          0x00000001
#define RSIF_BUF_FIFO_UNDERFL_INTRMASK_RD(src)           (((src) & 0x00000001))
#define RSIF_BUF_FIFO_UNDERFL_INTRMASK_WR(src)      (((u32)(src)) & 0x00000001)
#define RSIF_BUF_FIFO_UNDERFL_INTRMASK_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register CLE_RSIF_FInt_Reg0	*/ 
/*	 Fields rsif_ss_mirrorerr_intr	 */
#define RSIF_SS_MIRRORERR_INTR0_WIDTH                                         1
#define RSIF_SS_MIRRORERR_INTR0_SHIFT                                         4
#define RSIF_SS_MIRRORERR_INTR0_MASK                                 0x00000010
#define RSIF_SS_MIRRORERR_INTR0_RD(src)               (((src) & 0x00000010)>>4)
#define RSIF_SS_MIRRORERR_INTR0_WR(src)          (((u32)(src)<<4) & 0x00000010)
#define RSIF_SS_MIRRORERR_INTR0_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields rsif_ss_split_boundary_intr	 */
#define RSIF_SS_SPLIT_BOUNDARY_INTR0_WIDTH                                    1
#define RSIF_SS_SPLIT_BOUNDARY_INTR0_SHIFT                                    3
#define RSIF_SS_SPLIT_BOUNDARY_INTR0_MASK                            0x00000008
#define RSIF_SS_SPLIT_BOUNDARY_INTR0_RD(src)          (((src) & 0x00000008)>>3)
#define RSIF_SS_SPLIT_BOUNDARY_INTR0_WR(src)     (((u32)(src)<<3) & 0x00000008)
#define RSIF_SS_SPLIT_BOUNDARY_INTR0_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields rsif_ss_fpbuff_timeout_intr	 */
#define RSIF_SS_FPBUFF_TIMEOUT_INTR0_WIDTH                                    1
#define RSIF_SS_FPBUFF_TIMEOUT_INTR0_SHIFT                                    2
#define RSIF_SS_FPBUFF_TIMEOUT_INTR0_MASK                            0x00000004
#define RSIF_SS_FPBUFF_TIMEOUT_INTR0_RD(src)          (((src) & 0x00000004)>>2)
#define RSIF_SS_FPBUFF_TIMEOUT_INTR0_WR(src)     (((u32)(src)<<2) & 0x00000004)
#define RSIF_SS_FPBUFF_TIMEOUT_INTR0_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields rsif_ss_axi_wrerr_intr	 */
#define RSIF_SS_AXI_WRERR_INTR0_WIDTH                                         1
#define RSIF_SS_AXI_WRERR_INTR0_SHIFT                                         1
#define RSIF_SS_AXI_WRERR_INTR0_MASK                                 0x00000002
#define RSIF_SS_AXI_WRERR_INTR0_RD(src)               (((src) & 0x00000002)>>1)
#define RSIF_SS_AXI_WRERR_INTR0_WR(src)          (((u32)(src)<<1) & 0x00000002)
#define RSIF_SS_AXI_WRERR_INTR0_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields sts_rsif_plc_drop	 */
#define STS_RSIF_PLC_DROP0_WIDTH                                              1
#define STS_RSIF_PLC_DROP0_SHIFT                                              0
#define STS_RSIF_PLC_DROP0_MASK                                      0x00000001
#define STS_RSIF_PLC_DROP0_RD(src)                       (((src) & 0x00000001))
#define STS_RSIF_PLC_DROP0_WR(src)                  (((u32)(src)) & 0x00000001)
#define STS_RSIF_PLC_DROP0_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register CLE_RSIF_FInt_Reg0Mask	*/
/*    Mask Register Fields rsif_ss_mirrorerr_intrMask    */
#define RSIF_SS_MIRRORERR_INTRMASK_WIDTH                                      1
#define RSIF_SS_MIRRORERR_INTRMASK_SHIFT                                      4
#define RSIF_SS_MIRRORERR_INTRMASK_MASK                              0x00000010
#define RSIF_SS_MIRRORERR_INTRMASK_RD(src)            (((src) & 0x00000010)>>4)
#define RSIF_SS_MIRRORERR_INTRMASK_WR(src)       (((u32)(src)<<4) & 0x00000010)
#define RSIF_SS_MIRRORERR_INTRMASK_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*    Mask Register Fields rsif_ss_split_boundary_intrMask    */
#define RSIF_SS_SPLIT_BOUNDARY_INTRMASK_WIDTH                                 1
#define RSIF_SS_SPLIT_BOUNDARY_INTRMASK_SHIFT                                 3
#define RSIF_SS_SPLIT_BOUNDARY_INTRMASK_MASK                         0x00000008
#define RSIF_SS_SPLIT_BOUNDARY_INTRMASK_RD(src)       (((src) & 0x00000008)>>3)
#define RSIF_SS_SPLIT_BOUNDARY_INTRMASK_WR(src)  (((u32)(src)<<3) & 0x00000008)
#define RSIF_SS_SPLIT_BOUNDARY_INTRMASK_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*    Mask Register Fields rsif_ss_fpbuff_timeout_intrMask    */
#define RSIF_SS_FPBUFF_TIMEOUT_INTRMASK_WIDTH                                 1
#define RSIF_SS_FPBUFF_TIMEOUT_INTRMASK_SHIFT                                 2
#define RSIF_SS_FPBUFF_TIMEOUT_INTRMASK_MASK                         0x00000004
#define RSIF_SS_FPBUFF_TIMEOUT_INTRMASK_RD(src)       (((src) & 0x00000004)>>2)
#define RSIF_SS_FPBUFF_TIMEOUT_INTRMASK_WR(src)  (((u32)(src)<<2) & 0x00000004)
#define RSIF_SS_FPBUFF_TIMEOUT_INTRMASK_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*    Mask Register Fields rsif_ss_axi_wrerr_intrMask    */
#define RSIF_SS_AXI_WRERR_INTRMASK_WIDTH                                      1
#define RSIF_SS_AXI_WRERR_INTRMASK_SHIFT                                      1
#define RSIF_SS_AXI_WRERR_INTRMASK_MASK                              0x00000002
#define RSIF_SS_AXI_WRERR_INTRMASK_RD(src)            (((src) & 0x00000002)>>1)
#define RSIF_SS_AXI_WRERR_INTRMASK_WR(src)       (((u32)(src)<<1) & 0x00000002)
#define RSIF_SS_AXI_WRERR_INTRMASK_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*    Mask Register Fields sts_rsif_plc_dropMask    */
#define STS_RSIF_PLC_DROPMASK_WIDTH                                           1
#define STS_RSIF_PLC_DROPMASK_SHIFT                                           0
#define STS_RSIF_PLC_DROPMASK_MASK                                   0x00000001
#define STS_RSIF_PLC_DROPMASK_RD(src)                    (((src) & 0x00000001))
#define STS_RSIF_PLC_DROPMASK_WR(src)               (((u32)(src)) & 0x00000001)
#define STS_RSIF_PLC_DROPMASK_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register CLE_RSIF_Sts_Reg0	*/ 
/*	 Fields rsif_ss_axi_wrerr_bresp	 */
#define RSIF_SS_AXI_WRERR_BRESP0_WIDTH                                        2
#define RSIF_SS_AXI_WRERR_BRESP0_SHIFT                                        0
#define RSIF_SS_AXI_WRERR_BRESP0_MASK                                0x00000003
#define RSIF_SS_AXI_WRERR_BRESP0_RD(src)                 (((src) & 0x00000003))
#define RSIF_SS_AXI_WRERR_BRESP0_SET(dst,src) \
                          (((dst) & ~0x00000003) | (((u32)(src)) & 0x00000003))

/*	Register CLE_TSIF_FIFO_EmptySts0	*/ 
/*	 Fields tsif_rdmbuff_fifo_empty	 */
#define TSIF_RDMBUFF_FIFO_EMPTY0_WIDTH                                        1
#define TSIF_RDMBUFF_FIFO_EMPTY0_SHIFT                                        2
#define TSIF_RDMBUFF_FIFO_EMPTY0_MASK                                0x00000004
#define TSIF_RDMBUFF_FIFO_EMPTY0_RD(src)              (((src) & 0x00000004)>>2)
#define TSIF_RDMBUFF_FIFO_EMPTY0_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields tsif_rrm_fifo_empty	 */
#define TSIF_RRM_FIFO_EMPTY0_WIDTH                                            1
#define TSIF_RRM_FIFO_EMPTY0_SHIFT                                            1
#define TSIF_RRM_FIFO_EMPTY0_MASK                                    0x00000002
#define TSIF_RRM_FIFO_EMPTY0_RD(src)                  (((src) & 0x00000002)>>1)
#define TSIF_RRM_FIFO_EMPTY0_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields tsif_amabuf_fifo_empty	 */
#define TSIF_AMABUF_FIFO_EMPTY0_WIDTH                                         1
#define TSIF_AMABUF_FIFO_EMPTY0_SHIFT                                         0
#define TSIF_AMABUF_FIFO_EMPTY0_MASK                                 0x00000001
#define TSIF_AMABUF_FIFO_EMPTY0_RD(src)                  (((src) & 0x00000001))
#define TSIF_AMABUF_FIFO_EMPTY0_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register CLE_TSIF_Int_Reg0	*/ 
/*	 Fields tsif_rdmbuff_fifo_overfl_intr	 */
#define TSIF_RDMBUFF_FIFO_OVERFL_INTR0_WIDTH                                  1
#define TSIF_RDMBUFF_FIFO_OVERFL_INTR0_SHIFT                                  5
#define TSIF_RDMBUFF_FIFO_OVERFL_INTR0_MASK                          0x00000020
#define TSIF_RDMBUFF_FIFO_OVERFL_INTR0_RD(src)        (((src) & 0x00000020)>>5)
#define TSIF_RDMBUFF_FIFO_OVERFL_INTR0_WR(src)   (((u32)(src)<<5) & 0x00000020)
#define TSIF_RDMBUFF_FIFO_OVERFL_INTR0_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields tsif_rdmbuff_fifo_underfl_intr	 */
#define TSIF_RDMBUFF_FIFO_UNDERFL_INTR0_WIDTH                                 1
#define TSIF_RDMBUFF_FIFO_UNDERFL_INTR0_SHIFT                                 4
#define TSIF_RDMBUFF_FIFO_UNDERFL_INTR0_MASK                         0x00000010
#define TSIF_RDMBUFF_FIFO_UNDERFL_INTR0_RD(src)       (((src) & 0x00000010)>>4)
#define TSIF_RDMBUFF_FIFO_UNDERFL_INTR0_WR(src)  (((u32)(src)<<4) & 0x00000010)
#define TSIF_RDMBUFF_FIFO_UNDERFL_INTR0_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields tsif_rrm_fifo_overfl_intr	 */
#define TSIF_RRM_FIFO_OVERFL_INTR0_WIDTH                                      1
#define TSIF_RRM_FIFO_OVERFL_INTR0_SHIFT                                      3
#define TSIF_RRM_FIFO_OVERFL_INTR0_MASK                              0x00000008
#define TSIF_RRM_FIFO_OVERFL_INTR0_RD(src)            (((src) & 0x00000008)>>3)
#define TSIF_RRM_FIFO_OVERFL_INTR0_WR(src)       (((u32)(src)<<3) & 0x00000008)
#define TSIF_RRM_FIFO_OVERFL_INTR0_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields tsif_rrm_fifo_underfl_intr	 */
#define TSIF_RRM_FIFO_UNDERFL_INTR0_WIDTH                                     1
#define TSIF_RRM_FIFO_UNDERFL_INTR0_SHIFT                                     2
#define TSIF_RRM_FIFO_UNDERFL_INTR0_MASK                             0x00000004
#define TSIF_RRM_FIFO_UNDERFL_INTR0_RD(src)           (((src) & 0x00000004)>>2)
#define TSIF_RRM_FIFO_UNDERFL_INTR0_WR(src)      (((u32)(src)<<2) & 0x00000004)
#define TSIF_RRM_FIFO_UNDERFL_INTR0_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields tsif_amabuf_fifo_overfl_intr	 */
#define TSIF_AMABUF_FIFO_OVERFL_INTR0_WIDTH                                   1
#define TSIF_AMABUF_FIFO_OVERFL_INTR0_SHIFT                                   1
#define TSIF_AMABUF_FIFO_OVERFL_INTR0_MASK                           0x00000002
#define TSIF_AMABUF_FIFO_OVERFL_INTR0_RD(src)         (((src) & 0x00000002)>>1)
#define TSIF_AMABUF_FIFO_OVERFL_INTR0_WR(src)    (((u32)(src)<<1) & 0x00000002)
#define TSIF_AMABUF_FIFO_OVERFL_INTR0_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields tsif_amabuf_fifo_underfl_intr	 */
#define TSIF_AMABUF_FIFO_UNDERFL_INTR0_WIDTH                                  1
#define TSIF_AMABUF_FIFO_UNDERFL_INTR0_SHIFT                                  0
#define TSIF_AMABUF_FIFO_UNDERFL_INTR0_MASK                          0x00000001
#define TSIF_AMABUF_FIFO_UNDERFL_INTR0_RD(src)           (((src) & 0x00000001))
#define TSIF_AMABUF_FIFO_UNDERFL_INTR0_WR(src)      (((u32)(src)) & 0x00000001)
#define TSIF_AMABUF_FIFO_UNDERFL_INTR0_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register CLE_TSIF_Int_Reg0Mask	*/
/*    Mask Register Fields tsif_rdmbuff_fifo_overfl_intrMask    */
#define TSIF_RDMBUFF_FIFO_OVERFL_INTRMASK_WIDTH                               1
#define TSIF_RDMBUFF_FIFO_OVERFL_INTRMASK_SHIFT                               5
#define TSIF_RDMBUFF_FIFO_OVERFL_INTRMASK_MASK                       0x00000020
#define TSIF_RDMBUFF_FIFO_OVERFL_INTRMASK_RD(src)     (((src) & 0x00000020)>>5)
#define TSIF_RDMBUFF_FIFO_OVERFL_INTRMASK_WR(src) \
                                                 (((u32)(src)<<5) & 0x00000020)
#define TSIF_RDMBUFF_FIFO_OVERFL_INTRMASK_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*    Mask Register Fields tsif_rdmbuff_fifo_underfl_intrMask    */
#define TSIF_RDMBUFF_FIFO_UNDERFL_INTRMASK_WIDTH                              1
#define TSIF_RDMBUFF_FIFO_UNDERFL_INTRMASK_SHIFT                              4
#define TSIF_RDMBUFF_FIFO_UNDERFL_INTRMASK_MASK                      0x00000010
#define TSIF_RDMBUFF_FIFO_UNDERFL_INTRMASK_RD(src)    (((src) & 0x00000010)>>4)
#define TSIF_RDMBUFF_FIFO_UNDERFL_INTRMASK_WR(src) \
                                                 (((u32)(src)<<4) & 0x00000010)
#define TSIF_RDMBUFF_FIFO_UNDERFL_INTRMASK_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*    Mask Register Fields tsif_rrm_fifo_overfl_intrMask    */
#define TSIF_RRM_FIFO_OVERFL_INTRMASK_WIDTH                                   1
#define TSIF_RRM_FIFO_OVERFL_INTRMASK_SHIFT                                   3
#define TSIF_RRM_FIFO_OVERFL_INTRMASK_MASK                           0x00000008
#define TSIF_RRM_FIFO_OVERFL_INTRMASK_RD(src)         (((src) & 0x00000008)>>3)
#define TSIF_RRM_FIFO_OVERFL_INTRMASK_WR(src)    (((u32)(src)<<3) & 0x00000008)
#define TSIF_RRM_FIFO_OVERFL_INTRMASK_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*    Mask Register Fields tsif_rrm_fifo_underfl_intrMask    */
#define TSIF_RRM_FIFO_UNDERFL_INTRMASK_WIDTH                                  1
#define TSIF_RRM_FIFO_UNDERFL_INTRMASK_SHIFT                                  2
#define TSIF_RRM_FIFO_UNDERFL_INTRMASK_MASK                          0x00000004
#define TSIF_RRM_FIFO_UNDERFL_INTRMASK_RD(src)        (((src) & 0x00000004)>>2)
#define TSIF_RRM_FIFO_UNDERFL_INTRMASK_WR(src)   (((u32)(src)<<2) & 0x00000004)
#define TSIF_RRM_FIFO_UNDERFL_INTRMASK_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*    Mask Register Fields tsif_amabuf_fifo_overfl_intrMask    */
#define TSIF_AMABUF_FIFO_OVERFL_INTRMASK_WIDTH                                1
#define TSIF_AMABUF_FIFO_OVERFL_INTRMASK_SHIFT                                1
#define TSIF_AMABUF_FIFO_OVERFL_INTRMASK_MASK                        0x00000002
#define TSIF_AMABUF_FIFO_OVERFL_INTRMASK_RD(src)      (((src) & 0x00000002)>>1)
#define TSIF_AMABUF_FIFO_OVERFL_INTRMASK_WR(src) \
                                                 (((u32)(src)<<1) & 0x00000002)
#define TSIF_AMABUF_FIFO_OVERFL_INTRMASK_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*    Mask Register Fields tsif_amabuf_fifo_underfl_intrMask    */
#define TSIF_AMABUF_FIFO_UNDERFL_INTRMASK_WIDTH                               1
#define TSIF_AMABUF_FIFO_UNDERFL_INTRMASK_SHIFT                               0
#define TSIF_AMABUF_FIFO_UNDERFL_INTRMASK_MASK                       0x00000001
#define TSIF_AMABUF_FIFO_UNDERFL_INTRMASK_RD(src)        (((src) & 0x00000001))
#define TSIF_AMABUF_FIFO_UNDERFL_INTRMASK_WR(src)   (((u32)(src)) & 0x00000001)
#define TSIF_AMABUF_FIFO_UNDERFL_INTRMASK_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register CLE_TSIF_FInt_Reg0	*/ 
/*	 Fields tsif_ss_axi_llrderr_intr	 */
#define TSIF_SS_AXI_LLRDERR_INTR0_WIDTH                                       1
#define TSIF_SS_AXI_LLRDERR_INTR0_SHIFT                                       2
#define TSIF_SS_AXI_LLRDERR_INTR0_MASK                               0x00000004
#define TSIF_SS_AXI_LLRDERR_INTR0_RD(src)             (((src) & 0x00000004)>>2)
#define TSIF_SS_AXI_LLRDERR_INTR0_WR(src)        (((u32)(src)<<2) & 0x00000004)
#define TSIF_SS_AXI_LLRDERR_INTR0_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields tsif_ss_axi_rderr_intr	 */
#define TSIF_SS_AXI_RDERR_INTR0_WIDTH                                         1
#define TSIF_SS_AXI_RDERR_INTR0_SHIFT                                         1
#define TSIF_SS_AXI_RDERR_INTR0_MASK                                 0x00000002
#define TSIF_SS_AXI_RDERR_INTR0_RD(src)               (((src) & 0x00000002)>>1)
#define TSIF_SS_AXI_RDERR_INTR0_WR(src)          (((u32)(src)<<1) & 0x00000002)
#define TSIF_SS_AXI_RDERR_INTR0_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields tsif_ss_bad_msg_intr	 */
#define TSIF_SS_BAD_MSG_INTR0_WIDTH                                           1
#define TSIF_SS_BAD_MSG_INTR0_SHIFT                                           0
#define TSIF_SS_BAD_MSG_INTR0_MASK                                   0x00000001
#define TSIF_SS_BAD_MSG_INTR0_RD(src)                    (((src) & 0x00000001))
#define TSIF_SS_BAD_MSG_INTR0_WR(src)               (((u32)(src)) & 0x00000001)
#define TSIF_SS_BAD_MSG_INTR0_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register CLE_TSIF_FInt_Reg0Mask	*/
/*    Mask Register Fields tsif_ss_axi_llrderr_intrMask    */
#define TSIF_SS_AXI_LLRDERR_INTRMASK_WIDTH                                    1
#define TSIF_SS_AXI_LLRDERR_INTRMASK_SHIFT                                    2
#define TSIF_SS_AXI_LLRDERR_INTRMASK_MASK                            0x00000004
#define TSIF_SS_AXI_LLRDERR_INTRMASK_RD(src)          (((src) & 0x00000004)>>2)
#define TSIF_SS_AXI_LLRDERR_INTRMASK_WR(src)     (((u32)(src)<<2) & 0x00000004)
#define TSIF_SS_AXI_LLRDERR_INTRMASK_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*    Mask Register Fields tsif_ss_axi_rderr_intrMask    */
#define TSIF_SS_AXI_RDERR_INTRMASK_WIDTH                                      1
#define TSIF_SS_AXI_RDERR_INTRMASK_SHIFT                                      1
#define TSIF_SS_AXI_RDERR_INTRMASK_MASK                              0x00000002
#define TSIF_SS_AXI_RDERR_INTRMASK_RD(src)            (((src) & 0x00000002)>>1)
#define TSIF_SS_AXI_RDERR_INTRMASK_WR(src)       (((u32)(src)<<1) & 0x00000002)
#define TSIF_SS_AXI_RDERR_INTRMASK_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*    Mask Register Fields tsif_ss_bad_msg_intrMask    */
#define TSIF_SS_BAD_MSG_INTRMASK_WIDTH                                        1
#define TSIF_SS_BAD_MSG_INTRMASK_SHIFT                                        0
#define TSIF_SS_BAD_MSG_INTRMASK_MASK                                0x00000001
#define TSIF_SS_BAD_MSG_INTRMASK_RD(src)                 (((src) & 0x00000001))
#define TSIF_SS_BAD_MSG_INTRMASK_WR(src)            (((u32)(src)) & 0x00000001)
#define TSIF_SS_BAD_MSG_INTRMASK_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register CLE_TSIF_Sts_Reg0	*/ 
/*	 Fields tsif_ss_axi_llrderr_rresp	 */
#define TSIF_SS_AXI_LLRDERR_RRESP0_WIDTH                                      2
#define TSIF_SS_AXI_LLRDERR_RRESP0_SHIFT                                      2
#define TSIF_SS_AXI_LLRDERR_RRESP0_MASK                              0x0000000c
#define TSIF_SS_AXI_LLRDERR_RRESP0_RD(src)            (((src) & 0x0000000c)>>2)
#define TSIF_SS_AXI_LLRDERR_RRESP0_SET(dst,src) \
                       (((dst) & ~0x0000000c) | (((u32)(src)<<2) & 0x0000000c))
/*	 Fields tsif_ss_axi_rderr_rresp	 */
#define TSIF_SS_AXI_RDERR_RRESP0_WIDTH                                        2
#define TSIF_SS_AXI_RDERR_RRESP0_SHIFT                                        0
#define TSIF_SS_AXI_RDERR_RRESP0_MASK                                0x00000003
#define TSIF_SS_AXI_RDERR_RRESP0_RD(src)                 (((src) & 0x00000003))
#define TSIF_SS_AXI_RDERR_RRESP0_SET(dst,src) \
                          (((dst) & ~0x00000003) | (((u32)(src)) & 0x00000003))

/*	Register CLE_AMA_cfg_reg	*/ 
/*	 Fields csr_ama_rx_wrr_weights1	 */
#define CSR_AMA_RX_WRR_WEIGHTS1_WIDTH                                         4
#define CSR_AMA_RX_WRR_WEIGHTS1_SHIFT                                        16
#define CSR_AMA_RX_WRR_WEIGHTS1_MASK                                 0x000f0000
#define CSR_AMA_RX_WRR_WEIGHTS1_RD(src)              (((src) & 0x000f0000)>>16)
#define CSR_AMA_RX_WRR_WEIGHTS1_WR(src)         (((u32)(src)<<16) & 0x000f0000)
#define CSR_AMA_RX_WRR_WEIGHTS1_SET(dst,src) \
                      (((dst) & ~0x000f0000) | (((u32)(src)<<16) & 0x000f0000))
/*	 Fields csr_ama_rx_wrr_weights0	 */
#define CSR_AMA_RX_WRR_WEIGHTS0_WIDTH                                         4
#define CSR_AMA_RX_WRR_WEIGHTS0_SHIFT                                        12
#define CSR_AMA_RX_WRR_WEIGHTS0_MASK                                 0x0000f000
#define CSR_AMA_RX_WRR_WEIGHTS0_RD(src)              (((src) & 0x0000f000)>>12)
#define CSR_AMA_RX_WRR_WEIGHTS0_WR(src)         (((u32)(src)<<12) & 0x0000f000)
#define CSR_AMA_RX_WRR_WEIGHTS0_SET(dst,src) \
                      (((dst) & ~0x0000f000) | (((u32)(src)<<12) & 0x0000f000))
/*	 Fields csr_ama_tx_wrr_weights1	 */
#define CSR_AMA_TX_WRR_WEIGHTS1_WIDTH                                         4
#define CSR_AMA_TX_WRR_WEIGHTS1_SHIFT                                         8
#define CSR_AMA_TX_WRR_WEIGHTS1_MASK                                 0x00000f00
#define CSR_AMA_TX_WRR_WEIGHTS1_RD(src)               (((src) & 0x00000f00)>>8)
#define CSR_AMA_TX_WRR_WEIGHTS1_WR(src)          (((u32)(src)<<8) & 0x00000f00)
#define CSR_AMA_TX_WRR_WEIGHTS1_SET(dst,src) \
                       (((dst) & ~0x00000f00) | (((u32)(src)<<8) & 0x00000f00))
/*	 Fields csr_ama_tx_wrr_weights0	 */
#define CSR_AMA_TX_WRR_WEIGHTS0_WIDTH                                         4
#define CSR_AMA_TX_WRR_WEIGHTS0_SHIFT                                         4
#define CSR_AMA_TX_WRR_WEIGHTS0_MASK                                 0x000000f0
#define CSR_AMA_TX_WRR_WEIGHTS0_RD(src)               (((src) & 0x000000f0)>>4)
#define CSR_AMA_TX_WRR_WEIGHTS0_WR(src)          (((u32)(src)<<4) & 0x000000f0)
#define CSR_AMA_TX_WRR_WEIGHTS0_SET(dst,src) \
                       (((dst) & ~0x000000f0) | (((u32)(src)<<4) & 0x000000f0))
/*	 Fields rx_hi_priority	 */
#define RX_HI_PRIORITY_WIDTH                                                  2
#define RX_HI_PRIORITY_SHIFT                                                  2
#define RX_HI_PRIORITY_MASK                                          0x0000000c
#define RX_HI_PRIORITY_RD(src)                        (((src) & 0x0000000c)>>2)
#define RX_HI_PRIORITY_WR(src)                   (((u32)(src)<<2) & 0x0000000c)
#define RX_HI_PRIORITY_SET(dst,src) \
                       (((dst) & ~0x0000000c) | (((u32)(src)<<2) & 0x0000000c))
/*	 Fields tx_hi_priority	 */
#define TX_HI_PRIORITY_WIDTH                                                  2
#define TX_HI_PRIORITY_SHIFT                                                  0
#define TX_HI_PRIORITY_MASK                                          0x00000003
#define TX_HI_PRIORITY_RD(src)                           (((src) & 0x00000003))
#define TX_HI_PRIORITY_WR(src)                      (((u32)(src)) & 0x00000003)
#define TX_HI_PRIORITY_SET(dst,src) \
                          (((dst) & ~0x00000003) | (((u32)(src)) & 0x00000003))

/*	Register sw_int	*/ 
/*	 Fields sw_int	 */
#define SW_INT_WIDTH                                                          1
#define SW_INT_SHIFT                                                          0
#define SW_INT_MASK                                                  0x00000001
#define SW_INT_RD(src)                                   (((src) & 0x00000001))
#define SW_INT_WR(src)                              (((u32)(src)) & 0x00000001)
#define SW_INT_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Global Base Address	*/
#define CLE_GLBL_DIAG_CSR_BASE_ADDR				0xddd8e7000ULL

/*    Address GLBL_DIAG_CSR  Registers */
#define CLE_CFG_DIAG_SEL_ADDR                                        0x00000000
#define CLE_CFG_DIAG_SEL_DEFAULT                                     0x00000000
#define CLE_CFG_READ_BW_LAT_ADDR_MASK_ADDR                           0x00000004
#define CLE_CFG_READ_BW_LAT_ADDR_MASK_DEFAULT                        0x00000000
#define CLE_CFG_READ_BW_LAT_ADDR_PAT_ADDR                            0x00000008
#define CLE_CFG_READ_BW_LAT_ADDR_PAT_DEFAULT                         0xffffffff
#define CLE_CFG_WRITE_BW_LAT_ADDR_MASK_ADDR                          0x0000000c
#define CLE_CFG_WRITE_BW_LAT_ADDR_MASK_DEFAULT                       0x00000000
#define CLE_CFG_WRITE_BW_LAT_ADDR_PAT_ADDR                           0x00000010
#define CLE_CFG_WRITE_BW_LAT_ADDR_PAT_DEFAULT                        0xffffffff
#define CLE_CFG_DIAG_START_STOP_ADDR                                 0x00000014
#define CLE_CFG_DIAG_START_STOP_DEFAULT                              0x0000000f
#define CLE_CFG_BW_MSTR_STOP_CNT_ADDR                                0x00000018
#define CLE_CFG_BW_MSTR_STOP_CNT_DEFAULT                             0x00040004
#define CLE_CFG_BW_SLV_STOP_CNT_ADDR                                 0x0000001c
#define CLE_CFG_BW_SLV_STOP_CNT_DEFAULT                              0x00040004
#define CLE_STS_READ_LATENCY_OUTPUT_ADDR                             0x00000020
#define CLE_STS_READ_LATENCY_OUTPUT_DEFAULT                          0x00000000
#define CLE_STS_AXI_MRD_BW_CLK_CNT_ADDR                              0x00000024
#define CLE_STS_AXI_MRD_BW_CLK_CNT_DEFAULT                           0x00000000
#define CLE_STS_AXI_MRD_BW_BYTE_CNT_ADDR                             0x00000028
#define CLE_STS_AXI_MRD_BW_BYTE_CNT_DEFAULT                          0x00000000
#define CLE_STS_AXI_MWR_BW_CLK_CNT_ADDR                              0x0000002c
#define CLE_STS_AXI_MWR_BW_CLK_CNT_DEFAULT                           0x00000000
#define CLE_STS_AXI_MWR_BW_BYTE_CNT_ADDR                             0x00000030
#define CLE_STS_AXI_MWR_BW_BYTE_CNT_DEFAULT                          0x00000000
#define CLE_STS_AXI_SRD_BW_CLK_CNT_ADDR                              0x00000034
#define CLE_STS_AXI_SRD_BW_CLK_CNT_DEFAULT                           0x00000000
#define CLE_STS_AXI_SRD_BW_BYTE_CNT_ADDR                             0x00000038
#define CLE_STS_AXI_SRD_BW_BYTE_CNT_DEFAULT                          0x00000000
#define CLE_STS_AXI_SWR_BW_CLK_CNT_ADDR                              0x0000003c
#define CLE_STS_AXI_SWR_BW_CLK_CNT_DEFAULT                           0x00000000
#define CLE_STS_AXI_SWR_BW_BYTE_CNT_ADDR                             0x00000040
#define CLE_STS_AXI_SWR_BW_BYTE_CNT_DEFAULT                          0x00000000
#define CLE_CFG_DBG_TRIG_CTRL_ADDR                                   0x00000044
#define CLE_CFG_DBG_TRIG_CTRL_DEFAULT                                0x00000000
#define CLE_CFG_DBG_PAT_REG_0_ADDR                                   0x00000048
#define CLE_CFG_DBG_PAT_REG_0_DEFAULT                                0x00000000
#define CLE_CFG_DBG_PAT_MASK_REG_0_ADDR                              0x0000004c
#define CLE_CFG_DBG_PAT_MASK_REG_0_DEFAULT                           0x00000000
#define CLE_CFG_DBG_PAT_REG_1_ADDR                                   0x00000050
#define CLE_CFG_DBG_PAT_REG_1_DEFAULT                                0x00000000
#define CLE_CFG_DBG_PAT_MASK_REG_1_ADDR                              0x00000054
#define CLE_CFG_DBG_PAT_MASK_REG_1_DEFAULT                           0x00000000
#define CLE_DBG_TRIG_OUT_ADDR                                        0x00000058
#define CLE_DBG_TRIG_OUT_DEFAULT                                     0x00000000
#define CLE_DBG_ECC_INT_OUT_ADDR                                     0x0000005c
#define CLE_DBG_ECC_INT_OUT_DEFAULT                                  0x00000000
#define CLE_DBG_ECC_INT_OUTMASK_ADDR                                 0x00000060
#define CLE_CFG_SECC_INT_THRSH_REG_ADDR                              0x00000064
#define CLE_CFG_SECC_INT_THRSH_REG_DEFAULT                           0x000000fe
#define CLE_CFG_MEM_ECC_BYPASS_ADDR                                  0x00000068
#define CLE_CFG_MEM_ECC_BYPASS_DEFAULT                               0x00000000
#define CLE_CFG_MEM_PWRDN_DIS_ADDR                                   0x0000006c
#define CLE_CFG_MEM_PWRDN_DIS_DEFAULT                                0x00000000
#define CLE_CFG_MEM_RAM_SHUTDOWN_ADDR                                0x00000070
#define CLE_CFG_MEM_RAM_SHUTDOWN_DEFAULT                             0xffffffff
#define CLE_BLOCK_MEM_RDY_ADDR                                       0x00000074
#define CLE_BLOCK_MEM_RDY_DEFAULT                                    0xffffffff
#define CLE_CFG_AXI_SLV_RD_TMO_ADDR                                  0x00000078
#define CLE_CFG_AXI_SLV_RD_TMO_DEFAULT                               0x00000000
#define CLE_CFG_AXI_SLV_WR_TMO_ADDR                                  0x0000007c
#define CLE_CFG_AXI_SLV_WR_TMO_DEFAULT                               0x00000000
#define CLE_STS_AXI_SLV_TMO_ADDR                                     0x00000080
#define CLE_STS_AXI_SLV_TMO_DEFAULT                                  0x00000000
#define CLE_CFG_AXI_SS_CSR_TMO_ADDR                                  0x00000084
#define CLE_CFG_AXI_SS_CSR_TMO_DEFAULT                               0x00200000

/*	Register CFG_DIAG_SEL	*/ 
/*	 Fields CFG_MUX_SELECTOR	 */
#define CLE_CFG_MUX_SELECTOR_WIDTH                                           11
#define CLE_CFG_MUX_SELECTOR_SHIFT                                            0
#define CLE_CFG_MUX_SELECTOR_MASK                                    0x000007ff
#define CLE_CFG_MUX_SELECTOR_RD(src)                     (((src) & 0x000007ff))
#define CLE_CFG_MUX_SELECTOR_WR(src)                (((u32)(src)) & 0x000007ff)
#define CLE_CFG_MUX_SELECTOR_SET(dst,src) \
                          (((dst) & ~0x000007ff) | (((u32)(src)) & 0x000007ff))

/*	Register CFG_READ_BW_LAT_ADDR_MASK	*/ 
/*	 Fields READ_ADDR_MASK	 */
#define CLE_READ_ADDR_MASK_WIDTH                                             32
#define CLE_READ_ADDR_MASK_SHIFT                                              0
#define CLE_READ_ADDR_MASK_MASK                                      0xffffffff
#define CLE_READ_ADDR_MASK_RD(src)                       (((src) & 0xffffffff))
#define CLE_READ_ADDR_MASK_WR(src)                  (((u32)(src)) & 0xffffffff)
#define CLE_READ_ADDR_MASK_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_READ_BW_LAT_ADDR_PAT	*/ 
/*	 Fields READ_ADDR_PAT	 */
#define CLE_READ_ADDR_PAT_WIDTH                                              32
#define CLE_READ_ADDR_PAT_SHIFT                                               0
#define CLE_READ_ADDR_PAT_MASK                                       0xffffffff
#define CLE_READ_ADDR_PAT_RD(src)                        (((src) & 0xffffffff))
#define CLE_READ_ADDR_PAT_WR(src)                   (((u32)(src)) & 0xffffffff)
#define CLE_READ_ADDR_PAT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_WRITE_BW_LAT_ADDR_MASK	*/ 
/*	 Fields WRITE_ADDR_MASK	 */
#define CLE_WRITE_ADDR_MASK_WIDTH                                            32
#define CLE_WRITE_ADDR_MASK_SHIFT                                             0
#define CLE_WRITE_ADDR_MASK_MASK                                     0xffffffff
#define CLE_WRITE_ADDR_MASK_RD(src)                      (((src) & 0xffffffff))
#define CLE_WRITE_ADDR_MASK_WR(src)                 (((u32)(src)) & 0xffffffff)
#define CLE_WRITE_ADDR_MASK_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_WRITE_BW_LAT_ADDR_PAT	*/ 
/*	 Fields WRITE_ADDR_PAT	 */
#define CLE_WRITE_ADDR_PAT_WIDTH                                             32
#define CLE_WRITE_ADDR_PAT_SHIFT                                              0
#define CLE_WRITE_ADDR_PAT_MASK                                      0xffffffff
#define CLE_WRITE_ADDR_PAT_RD(src)                       (((src) & 0xffffffff))
#define CLE_WRITE_ADDR_PAT_WR(src)                  (((u32)(src)) & 0xffffffff)
#define CLE_WRITE_ADDR_PAT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_DIAG_START_STOP	*/ 
/*	 Fields START_RD_LATENCY	 */
#define CLE_START_RD_LATENCY_WIDTH                                            1
#define CLE_START_RD_LATENCY_SHIFT                                            4
#define CLE_START_RD_LATENCY_MASK                                    0x00000010
#define CLE_START_RD_LATENCY_RD(src)                  (((src) & 0x00000010)>>4)
#define CLE_START_RD_LATENCY_WR(src)             (((u32)(src)<<4) & 0x00000010)
#define CLE_START_RD_LATENCY_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields START_SRD_BW	 */
#define CLE_START_SRD_BW_WIDTH                                                1
#define CLE_START_SRD_BW_SHIFT                                                3
#define CLE_START_SRD_BW_MASK                                        0x00000008
#define CLE_START_SRD_BW_RD(src)                      (((src) & 0x00000008)>>3)
#define CLE_START_SRD_BW_WR(src)                 (((u32)(src)<<3) & 0x00000008)
#define CLE_START_SRD_BW_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields START_MRD_BW	 */
#define CLE_START_MRD_BW_WIDTH                                                1
#define CLE_START_MRD_BW_SHIFT                                                2
#define CLE_START_MRD_BW_MASK                                        0x00000004
#define CLE_START_MRD_BW_RD(src)                      (((src) & 0x00000004)>>2)
#define CLE_START_MRD_BW_WR(src)                 (((u32)(src)<<2) & 0x00000004)
#define CLE_START_MRD_BW_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields START_SWR_BW	 */
#define CLE_START_SWR_BW_WIDTH                                                1
#define CLE_START_SWR_BW_SHIFT                                                1
#define CLE_START_SWR_BW_MASK                                        0x00000002
#define CLE_START_SWR_BW_RD(src)                      (((src) & 0x00000002)>>1)
#define CLE_START_SWR_BW_WR(src)                 (((u32)(src)<<1) & 0x00000002)
#define CLE_START_SWR_BW_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields START_MWR_BW	 */
#define CLE_START_MWR_BW_WIDTH                                                1
#define CLE_START_MWR_BW_SHIFT                                                0
#define CLE_START_MWR_BW_MASK                                        0x00000001
#define CLE_START_MWR_BW_RD(src)                         (((src) & 0x00000001))
#define CLE_START_MWR_BW_WR(src)                    (((u32)(src)) & 0x00000001)
#define CLE_START_MWR_BW_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register CFG_BW_MSTR_STOP_CNT	*/ 
/*	 Fields MSTR_STOP_RD_CNT	 */
#define CLE_MSTR_STOP_RD_CNT_WIDTH                                           16
#define CLE_MSTR_STOP_RD_CNT_SHIFT                                           16
#define CLE_MSTR_STOP_RD_CNT_MASK                                    0xffff0000
#define CLE_MSTR_STOP_RD_CNT_RD(src)                 (((src) & 0xffff0000)>>16)
#define CLE_MSTR_STOP_RD_CNT_WR(src)            (((u32)(src)<<16) & 0xffff0000)
#define CLE_MSTR_STOP_RD_CNT_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields MSTR_STOP_WR_CNT	 */
#define CLE_MSTR_STOP_WR_CNT_WIDTH                                           16
#define CLE_MSTR_STOP_WR_CNT_SHIFT                                            0
#define CLE_MSTR_STOP_WR_CNT_MASK                                    0x0000ffff
#define CLE_MSTR_STOP_WR_CNT_RD(src)                     (((src) & 0x0000ffff))
#define CLE_MSTR_STOP_WR_CNT_WR(src)                (((u32)(src)) & 0x0000ffff)
#define CLE_MSTR_STOP_WR_CNT_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register CFG_BW_SLV_STOP_CNT	*/ 
/*	 Fields SLV_STOP_RD_CNT	 */
#define CLE_SLV_STOP_RD_CNT_WIDTH                                            16
#define CLE_SLV_STOP_RD_CNT_SHIFT                                            16
#define CLE_SLV_STOP_RD_CNT_MASK                                     0xffff0000
#define CLE_SLV_STOP_RD_CNT_RD(src)                  (((src) & 0xffff0000)>>16)
#define CLE_SLV_STOP_RD_CNT_WR(src)             (((u32)(src)<<16) & 0xffff0000)
#define CLE_SLV_STOP_RD_CNT_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields SLV_STOP_WR_CNT	 */
#define CLE_SLV_STOP_WR_CNT_WIDTH                                            16
#define CLE_SLV_STOP_WR_CNT_SHIFT                                             0
#define CLE_SLV_STOP_WR_CNT_MASK                                     0x0000ffff
#define CLE_SLV_STOP_WR_CNT_RD(src)                      (((src) & 0x0000ffff))
#define CLE_SLV_STOP_WR_CNT_WR(src)                 (((u32)(src)) & 0x0000ffff)
#define CLE_SLV_STOP_WR_CNT_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register STS_READ_LATENCY_OUTPUT	*/ 
/*	 Fields READ_MAX	 */
#define CLE_READ_MAX_WIDTH                                                   10
#define CLE_READ_MAX_SHIFT                                                   22
#define CLE_READ_MAX_MASK                                            0xffc00000
#define CLE_READ_MAX_RD(src)                         (((src) & 0xffc00000)>>22)
#define CLE_READ_MAX_SET(dst,src) \
                      (((dst) & ~0xffc00000) | (((u32)(src)<<22) & 0xffc00000))
/*	 Fields READ_TOT	 */
#define CLE_READ_TOT_WIDTH                                                   22
#define CLE_READ_TOT_SHIFT                                                    0
#define CLE_READ_TOT_MASK                                            0x003fffff
#define CLE_READ_TOT_RD(src)                             (((src) & 0x003fffff))
#define CLE_READ_TOT_SET(dst,src) \
                          (((dst) & ~0x003fffff) | (((u32)(src)) & 0x003fffff))

/*	Register STS_AXI_MRD_BW_CLK_CNT	*/ 
/*	 Fields MSTR_READ_BW_CLK_CNT	 */
#define CLE_MSTR_READ_BW_CLK_CNT_WIDTH                                       32
#define CLE_MSTR_READ_BW_CLK_CNT_SHIFT                                        0
#define CLE_MSTR_READ_BW_CLK_CNT_MASK                                0xffffffff
#define CLE_MSTR_READ_BW_CLK_CNT_RD(src)                 (((src) & 0xffffffff))
#define CLE_MSTR_READ_BW_CLK_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register STS_AXI_MRD_BW_BYTE_CNT	*/ 
/*	 Fields MSTR_READ_BW_BYTE_CNT	 */
#define CLE_MSTR_READ_BW_BYTE_CNT_WIDTH                                      32
#define CLE_MSTR_READ_BW_BYTE_CNT_SHIFT                                       0
#define CLE_MSTR_READ_BW_BYTE_CNT_MASK                               0xffffffff
#define CLE_MSTR_READ_BW_BYTE_CNT_RD(src)                (((src) & 0xffffffff))
#define CLE_MSTR_READ_BW_BYTE_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register STS_AXI_MWR_BW_CLK_CNT	*/ 
/*	 Fields MSTR_WRITE_BW_CLK_CNT	 */
#define CLE_MSTR_WRITE_BW_CLK_CNT_WIDTH                                      32
#define CLE_MSTR_WRITE_BW_CLK_CNT_SHIFT                                       0
#define CLE_MSTR_WRITE_BW_CLK_CNT_MASK                               0xffffffff
#define CLE_MSTR_WRITE_BW_CLK_CNT_RD(src)                (((src) & 0xffffffff))
#define CLE_MSTR_WRITE_BW_CLK_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register STS_AXI_MWR_BW_BYTE_CNT	*/ 
/*	 Fields MSTR_WRITE_BW_BYTE_CNT	 */
#define CLE_MSTR_WRITE_BW_BYTE_CNT_WIDTH                                     32
#define CLE_MSTR_WRITE_BW_BYTE_CNT_SHIFT                                      0
#define CLE_MSTR_WRITE_BW_BYTE_CNT_MASK                              0xffffffff
#define CLE_MSTR_WRITE_BW_BYTE_CNT_RD(src)               (((src) & 0xffffffff))
#define CLE_MSTR_WRITE_BW_BYTE_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register STS_AXI_SRD_BW_CLK_CNT	*/ 
/*	 Fields SLV_READ_BW_CLK_CNT	 */
#define CLE_SLV_READ_BW_CLK_CNT_WIDTH                                        32
#define CLE_SLV_READ_BW_CLK_CNT_SHIFT                                         0
#define CLE_SLV_READ_BW_CLK_CNT_MASK                                 0xffffffff
#define CLE_SLV_READ_BW_CLK_CNT_RD(src)                  (((src) & 0xffffffff))
#define CLE_SLV_READ_BW_CLK_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register STS_AXI_SRD_BW_BYTE_CNT	*/ 
/*	 Fields SLV_READ_BW_BYTE_CNT	 */
#define CLE_SLV_READ_BW_BYTE_CNT_WIDTH                                       32
#define CLE_SLV_READ_BW_BYTE_CNT_SHIFT                                        0
#define CLE_SLV_READ_BW_BYTE_CNT_MASK                                0xffffffff
#define CLE_SLV_READ_BW_BYTE_CNT_RD(src)                 (((src) & 0xffffffff))
#define CLE_SLV_READ_BW_BYTE_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register STS_AXI_SWR_BW_CLK_CNT	*/ 
/*	 Fields SLV_WRITE_BW_CLK_CNT	 */
#define CLE_SLV_WRITE_BW_CLK_CNT_WIDTH                                       32
#define CLE_SLV_WRITE_BW_CLK_CNT_SHIFT                                        0
#define CLE_SLV_WRITE_BW_CLK_CNT_MASK                                0xffffffff
#define CLE_SLV_WRITE_BW_CLK_CNT_RD(src)                 (((src) & 0xffffffff))
#define CLE_SLV_WRITE_BW_CLK_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register STS_AXI_SWR_BW_BYTE_CNT	*/ 
/*	 Fields SLV_WRITE_BW_BYTE_CNT	 */
#define CLE_SLV_WRITE_BW_BYTE_CNT_WIDTH                                      32
#define CLE_SLV_WRITE_BW_BYTE_CNT_SHIFT                                       0
#define CLE_SLV_WRITE_BW_BYTE_CNT_MASK                               0xffffffff
#define CLE_SLV_WRITE_BW_BYTE_CNT_RD(src)                (((src) & 0xffffffff))
#define CLE_SLV_WRITE_BW_BYTE_CNT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_DBG_TRIG_CTRL	*/ 
/*	 Fields TRIG_EN	 */
#define CLE_TRIG_EN_WIDTH                                                     1
#define CLE_TRIG_EN_SHIFT                                                     4
#define CLE_TRIG_EN_MASK                                             0x00000010
#define CLE_TRIG_EN_RD(src)                           (((src) & 0x00000010)>>4)
#define CLE_TRIG_EN_WR(src)                      (((u32)(src)<<4) & 0x00000010)
#define CLE_TRIG_EN_SET(dst,src) \
                       (((dst) & ~0x00000010) | (((u32)(src)<<4) & 0x00000010))
/*	 Fields AND_E	 */
#define CLE_AND_E_WIDTH                                                       2
#define CLE_AND_E_SHIFT                                                       2
#define CLE_AND_E_MASK                                               0x0000000c
#define CLE_AND_E_RD(src)                             (((src) & 0x0000000c)>>2)
#define CLE_AND_E_WR(src)                        (((u32)(src)<<2) & 0x0000000c)
#define CLE_AND_E_SET(dst,src) \
                       (((dst) & ~0x0000000c) | (((u32)(src)<<2) & 0x0000000c))
/*	 Fields OR_E	 */
#define CLE_OR_E_WIDTH                                                        2
#define CLE_OR_E_SHIFT                                                        0
#define CLE_OR_E_MASK                                                0x00000003
#define CLE_OR_E_RD(src)                                 (((src) & 0x00000003))
#define CLE_OR_E_WR(src)                            (((u32)(src)) & 0x00000003)
#define CLE_OR_E_SET(dst,src) \
                          (((dst) & ~0x00000003) | (((u32)(src)) & 0x00000003))

/*	Register CFG_DBG_PAT_REG_0	*/ 
/*	 Fields PATTERN	 */
#define CLE_PATTERN0_WIDTH                                                   32
#define CLE_PATTERN0_SHIFT                                                    0
#define CLE_PATTERN0_MASK                                            0xffffffff
#define CLE_PATTERN0_RD(src)                             (((src) & 0xffffffff))
#define CLE_PATTERN0_WR(src)                        (((u32)(src)) & 0xffffffff)
#define CLE_PATTERN0_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_DBG_PAT_MASK_REG_0	*/ 
/*	 Fields PAT_MASK	 */
#define CLE_PAT_MASK0_WIDTH                                                  32
#define CLE_PAT_MASK0_SHIFT                                                   0
#define CLE_PAT_MASK0_MASK                                           0xffffffff
#define CLE_PAT_MASK0_RD(src)                            (((src) & 0xffffffff))
#define CLE_PAT_MASK0_WR(src)                       (((u32)(src)) & 0xffffffff)
#define CLE_PAT_MASK0_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_DBG_PAT_REG_1	*/ 
/*	 Fields PATTERN	 */
#define CLE_PATTERN1_WIDTH                                                   32
#define CLE_PATTERN1_SHIFT                                                    0
#define CLE_PATTERN1_MASK                                            0xffffffff
#define CLE_PATTERN1_RD(src)                             (((src) & 0xffffffff))
#define CLE_PATTERN1_WR(src)                        (((u32)(src)) & 0xffffffff)
#define CLE_PATTERN1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_DBG_PAT_MASK_REG_1	*/ 
/*	 Fields PAT_MASK	 */
#define CLE_PAT_MASK1_WIDTH                                                  32
#define CLE_PAT_MASK1_SHIFT                                                   0
#define CLE_PAT_MASK1_MASK                                           0xffffffff
#define CLE_PAT_MASK1_RD(src)                            (((src) & 0xffffffff))
#define CLE_PAT_MASK1_WR(src)                       (((u32)(src)) & 0xffffffff)
#define CLE_PAT_MASK1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register DBG_TRIG_OUT	*/ 
/*	 Fields DBG_OUT	 */
#define CLE_DBG_OUT_WIDTH                                                    32
#define CLE_DBG_OUT_SHIFT                                                     0
#define CLE_DBG_OUT_MASK                                             0xffffffff
#define CLE_DBG_OUT_RD(src)                              (((src) & 0xffffffff))
#define CLE_DBG_OUT_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register DBG_ECC_INT_OUT	*/ 
/*	 Fields SLV_TMO	 */
#define CLE_SLV_TMO_WIDTH                                                     1
#define CLE_SLV_TMO_SHIFT                                                     3
#define CLE_SLV_TMO_MASK                                             0x00000008
#define CLE_SLV_TMO_RD(src)                           (((src) & 0x00000008)>>3)
#define CLE_SLV_TMO_WR(src)                      (((u32)(src)<<3) & 0x00000008)
#define CLE_SLV_TMO_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields SECC_INT	 */
#define CLE_SECC_INT_WIDTH                                                    1
#define CLE_SECC_INT_SHIFT                                                    2
#define CLE_SECC_INT_MASK                                            0x00000004
#define CLE_SECC_INT_RD(src)                          (((src) & 0x00000004)>>2)
#define CLE_SECC_INT_WR(src)                     (((u32)(src)<<2) & 0x00000004)
#define CLE_SECC_INT_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields DECC_INT	 */
#define CLE_DECC_INT_WIDTH                                                    1
#define CLE_DECC_INT_SHIFT                                                    1
#define CLE_DECC_INT_MASK                                            0x00000002
#define CLE_DECC_INT_RD(src)                          (((src) & 0x00000002)>>1)
#define CLE_DECC_INT_WR(src)                     (((u32)(src)<<1) & 0x00000002)
#define CLE_DECC_INT_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields DBG_INT	 */
#define CLE_DBG_INT_WIDTH                                                     1
#define CLE_DBG_INT_SHIFT                                                     0
#define CLE_DBG_INT_MASK                                             0x00000001
#define CLE_DBG_INT_RD(src)                              (((src) & 0x00000001))
#define CLE_DBG_INT_WR(src)                         (((u32)(src)) & 0x00000001)
#define CLE_DBG_INT_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register DBG_ECC_INT_OUTMask	*/
/*    Mask Register Fields SLV_TMOMask    */
#define CLE_SLV_TMOMASK_WIDTH                                                 1
#define CLE_SLV_TMOMASK_SHIFT                                                 3
#define CLE_SLV_TMOMASK_MASK                                         0x00000008
#define CLE_SLV_TMOMASK_RD(src)                       (((src) & 0x00000008)>>3)
#define CLE_SLV_TMOMASK_WR(src)                  (((u32)(src)<<3) & 0x00000008)
#define CLE_SLV_TMOMASK_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*    Mask Register Fields SECC_INTMask    */
#define CLE_SECC_INTMASK_WIDTH                                                1
#define CLE_SECC_INTMASK_SHIFT                                                2
#define CLE_SECC_INTMASK_MASK                                        0x00000004
#define CLE_SECC_INTMASK_RD(src)                      (((src) & 0x00000004)>>2)
#define CLE_SECC_INTMASK_WR(src)                 (((u32)(src)<<2) & 0x00000004)
#define CLE_SECC_INTMASK_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*    Mask Register Fields DECC_INTMask    */
#define CLE_DECC_INTMASK_WIDTH                                                1
#define CLE_DECC_INTMASK_SHIFT                                                1
#define CLE_DECC_INTMASK_MASK                                        0x00000002
#define CLE_DECC_INTMASK_RD(src)                      (((src) & 0x00000002)>>1)
#define CLE_DECC_INTMASK_WR(src)                 (((u32)(src)<<1) & 0x00000002)
#define CLE_DECC_INTMASK_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*    Mask Register Fields DBG_INTMask    */
#define CLE_DBG_INTMASK_WIDTH                                                 1
#define CLE_DBG_INTMASK_SHIFT                                                 0
#define CLE_DBG_INTMASK_MASK                                         0x00000001
#define CLE_DBG_INTMASK_RD(src)                          (((src) & 0x00000001))
#define CLE_DBG_INTMASK_WR(src)                     (((u32)(src)) & 0x00000001)
#define CLE_DBG_INTMASK_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register CFG_SECC_INT_THRSH_REG	*/ 
/*	 Fields SECC_INT_THRSH	 */
#define CLE_SECC_INT_THRSH_WIDTH                                              8
#define CLE_SECC_INT_THRSH_SHIFT                                              0
#define CLE_SECC_INT_THRSH_MASK                                      0x000000ff
#define CLE_SECC_INT_THRSH_RD(src)                       (((src) & 0x000000ff))
#define CLE_SECC_INT_THRSH_WR(src)                  (((u32)(src)) & 0x000000ff)
#define CLE_SECC_INT_THRSH_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register CFG_MEM_ECC_BYPASS	*/ 
/*	 Fields CFG_ECC_BYPASS	 */
#define CLE_CFG_ECC_BYPASS_WIDTH                                             16
#define CLE_CFG_ECC_BYPASS_SHIFT                                              0
#define CLE_CFG_ECC_BYPASS_MASK                                      0x0000ffff
#define CLE_CFG_ECC_BYPASS_RD(src)                       (((src) & 0x0000ffff))
#define CLE_CFG_ECC_BYPASS_WR(src)                  (((u32)(src)) & 0x0000ffff)
#define CLE_CFG_ECC_BYPASS_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register CFG_MEM_PWRDN_DIS	*/ 
/*	 Fields CFG_ECC_PWRDN_DIS	 */
#define CLE_CFG_ECC_PWRDN_DIS_WIDTH                                          16
#define CLE_CFG_ECC_PWRDN_DIS_SHIFT                                          16
#define CLE_CFG_ECC_PWRDN_DIS_MASK                                   0xffff0000
#define CLE_CFG_ECC_PWRDN_DIS_RD(src)                (((src) & 0xffff0000)>>16)
#define CLE_CFG_ECC_PWRDN_DIS_WR(src)           (((u32)(src)<<16) & 0xffff0000)
#define CLE_CFG_ECC_PWRDN_DIS_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields CFG_PWRDN_DIS	 */
#define CLE_CFG_PWRDN_DIS_WIDTH                                              16
#define CLE_CFG_PWRDN_DIS_SHIFT                                               0
#define CLE_CFG_PWRDN_DIS_MASK                                       0x0000ffff
#define CLE_CFG_PWRDN_DIS_RD(src)                        (((src) & 0x0000ffff))
#define CLE_CFG_PWRDN_DIS_WR(src)                   (((u32)(src)) & 0x0000ffff)
#define CLE_CFG_PWRDN_DIS_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register CFG_MEM_RAM_SHUTDOWN	*/ 
/*	 Fields CFG_RAM_SHUTDOWN_EN	 */
#define CLE_CFG_RAM_SHUTDOWN_EN_WIDTH                                        32
#define CLE_CFG_RAM_SHUTDOWN_EN_SHIFT                                         0
#define CLE_CFG_RAM_SHUTDOWN_EN_MASK                                 0xffffffff
#define CLE_CFG_RAM_SHUTDOWN_EN_RD(src)                  (((src) & 0xffffffff))
#define CLE_CFG_RAM_SHUTDOWN_EN_WR(src)             (((u32)(src)) & 0xffffffff)
#define CLE_CFG_RAM_SHUTDOWN_EN_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register BLOCK_MEM_RDY	*/ 
/*	 Fields MEM_RDY	 */
#define CLE_MEM_RDY_WIDTH                                                    32
#define CLE_MEM_RDY_SHIFT                                                     0
#define CLE_MEM_RDY_MASK                                             0xffffffff
#define CLE_MEM_RDY_RD(src)                              (((src) & 0xffffffff))
#define CLE_MEM_RDY_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CFG_AXI_SLV_RD_TMO	*/ 
/*	 Fields SLV_ID_TMO_ENABLE	 */
#define CLE_SLV_ID_TMO_ENABLE_WIDTH                                           1
#define CLE_SLV_ID_TMO_ENABLE_SHIFT                                          31
#define CLE_SLV_ID_TMO_ENABLE_MASK                                   0x80000000
#define CLE_SLV_ID_TMO_ENABLE_RD(src)                (((src) & 0x80000000)>>31)
#define CLE_SLV_ID_TMO_ENABLE_WR(src)           (((u32)(src)<<31) & 0x80000000)
#define CLE_SLV_ID_TMO_ENABLE_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields SS_ID_TRK_ENABLE	 */
#define CLE_SS_ID_TRK_ENABLE_WIDTH                                            1
#define CLE_SS_ID_TRK_ENABLE_SHIFT                                           30
#define CLE_SS_ID_TRK_ENABLE_MASK                                    0x40000000
#define CLE_SS_ID_TRK_ENABLE_RD(src)                 (((src) & 0x40000000)>>30)
#define CLE_SS_ID_TRK_ENABLE_WR(src)            (((u32)(src)<<30) & 0x40000000)
#define CLE_SS_ID_TRK_ENABLE_SET(dst,src) \
                      (((dst) & ~0x40000000) | (((u32)(src)<<30) & 0x40000000))
/*	 Fields SLV_ID_TMO_VALUE	 */
#define CLE_SLV_ID_TMO_VALUE_WIDTH                                           10
#define CLE_SLV_ID_TMO_VALUE_SHIFT                                           16
#define CLE_SLV_ID_TMO_VALUE_MASK                                    0x03ff0000
#define CLE_SLV_ID_TMO_VALUE_RD(src)                 (((src) & 0x03ff0000)>>16)
#define CLE_SLV_ID_TMO_VALUE_WR(src)            (((u32)(src)<<16) & 0x03ff0000)
#define CLE_SLV_ID_TMO_VALUE_SET(dst,src) \
                      (((dst) & ~0x03ff0000) | (((u32)(src)<<16) & 0x03ff0000))
/*	 Fields SLV_RDY_TMO_ENABLE	 */
#define CLE_SLV_RDY_TMO_ENABLE_WIDTH                                          1
#define CLE_SLV_RDY_TMO_ENABLE_SHIFT                                         15
#define CLE_SLV_RDY_TMO_ENABLE_MASK                                  0x00008000
#define CLE_SLV_RDY_TMO_ENABLE_RD(src)               (((src) & 0x00008000)>>15)
#define CLE_SLV_RDY_TMO_ENABLE_WR(src)          (((u32)(src)<<15) & 0x00008000)
#define CLE_SLV_RDY_TMO_ENABLE_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))
/*	 Fields SLV_RDY_TMO_VALUE	 */
#define CLE_SLV_RDY_TMO_VALUE_WIDTH                                          10
#define CLE_SLV_RDY_TMO_VALUE_SHIFT                                           0
#define CLE_SLV_RDY_TMO_VALUE_MASK                                   0x000003ff
#define CLE_SLV_RDY_TMO_VALUE_RD(src)                    (((src) & 0x000003ff))
#define CLE_SLV_RDY_TMO_VALUE_WR(src)               (((u32)(src)) & 0x000003ff)
#define CLE_SLV_RDY_TMO_VALUE_SET(dst,src) \
                          (((dst) & ~0x000003ff) | (((u32)(src)) & 0x000003ff))

/*	Register CFG_AXI_SLV_WR_TMO	*/ 
/*	 Fields SLV_ID_TMO_ENABLE	 */
#define CLE_SLV_ID_TMO_ENABLE_F1_WIDTH                                        1
#define CLE_SLV_ID_TMO_ENABLE_F1_SHIFT                                       31
#define CLE_SLV_ID_TMO_ENABLE_F1_MASK                                0x80000000
#define CLE_SLV_ID_TMO_ENABLE_F1_RD(src)             (((src) & 0x80000000)>>31)
#define CLE_SLV_ID_TMO_ENABLE_F1_WR(src)        (((u32)(src)<<31) & 0x80000000)
#define CLE_SLV_ID_TMO_ENABLE_F1_SET(dst,src) \
                      (((dst) & ~0x80000000) | (((u32)(src)<<31) & 0x80000000))
/*	 Fields SS_ID_TRK_ENABLE	 */
#define CLE_SS_ID_TRK_ENABLE_F1_WIDTH                                         1
#define CLE_SS_ID_TRK_ENABLE_F1_SHIFT                                        30
#define CLE_SS_ID_TRK_ENABLE_F1_MASK                                 0x40000000
#define CLE_SS_ID_TRK_ENABLE_F1_RD(src)              (((src) & 0x40000000)>>30)
#define CLE_SS_ID_TRK_ENABLE_F1_WR(src)         (((u32)(src)<<30) & 0x40000000)
#define CLE_SS_ID_TRK_ENABLE_F1_SET(dst,src) \
                      (((dst) & ~0x40000000) | (((u32)(src)<<30) & 0x40000000))
/*	 Fields SLV_ID_TMO_VALUE	 */
#define CLE_SLV_ID_TMO_VALUE_F1_WIDTH                                        10
#define CLE_SLV_ID_TMO_VALUE_F1_SHIFT                                        16
#define CLE_SLV_ID_TMO_VALUE_F1_MASK                                 0x03ff0000
#define CLE_SLV_ID_TMO_VALUE_F1_RD(src)              (((src) & 0x03ff0000)>>16)
#define CLE_SLV_ID_TMO_VALUE_F1_WR(src)         (((u32)(src)<<16) & 0x03ff0000)
#define CLE_SLV_ID_TMO_VALUE_F1_SET(dst,src) \
                      (((dst) & ~0x03ff0000) | (((u32)(src)<<16) & 0x03ff0000))
/*	 Fields SLV_RDY_TMO_ENABLE	 */
#define CLE_SLV_RDY_TMO_ENABLE_F1_WIDTH                                       1
#define CLE_SLV_RDY_TMO_ENABLE_F1_SHIFT                                      15
#define CLE_SLV_RDY_TMO_ENABLE_F1_MASK                               0x00008000
#define CLE_SLV_RDY_TMO_ENABLE_F1_RD(src)            (((src) & 0x00008000)>>15)
#define CLE_SLV_RDY_TMO_ENABLE_F1_WR(src)       (((u32)(src)<<15) & 0x00008000)
#define CLE_SLV_RDY_TMO_ENABLE_F1_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))
/*	 Fields SLV_RDY_TMO_VALUE	 */
#define CLE_SLV_RDY_TMO_VALUE_F1_WIDTH                                       10
#define CLE_SLV_RDY_TMO_VALUE_F1_SHIFT                                        0
#define CLE_SLV_RDY_TMO_VALUE_F1_MASK                                0x000003ff
#define CLE_SLV_RDY_TMO_VALUE_F1_RD(src)                 (((src) & 0x000003ff))
#define CLE_SLV_RDY_TMO_VALUE_F1_WR(src)            (((u32)(src)) & 0x000003ff)
#define CLE_SLV_RDY_TMO_VALUE_F1_SET(dst,src) \
                          (((dst) & ~0x000003ff) | (((u32)(src)) & 0x000003ff))

/*	Register STS_AXI_SLV_TMO	*/ 
/*	 Fields SLV_ARID_TMO	 */
#define CLE_SLV_ARID_TMO_WIDTH                                                1
#define CLE_SLV_ARID_TMO_SHIFT                                                3
#define CLE_SLV_ARID_TMO_MASK                                        0x00000008
#define CLE_SLV_ARID_TMO_RD(src)                      (((src) & 0x00000008)>>3)
#define CLE_SLV_ARID_TMO_SET(dst,src) \
                       (((dst) & ~0x00000008) | (((u32)(src)<<3) & 0x00000008))
/*	 Fields SLV_ARRDY_TMO	 */
#define CLE_SLV_ARRDY_TMO_WIDTH                                               1
#define CLE_SLV_ARRDY_TMO_SHIFT                                               2
#define CLE_SLV_ARRDY_TMO_MASK                                       0x00000004
#define CLE_SLV_ARRDY_TMO_RD(src)                     (((src) & 0x00000004)>>2)
#define CLE_SLV_ARRDY_TMO_SET(dst,src) \
                       (((dst) & ~0x00000004) | (((u32)(src)<<2) & 0x00000004))
/*	 Fields SLV_AWID_TMO	 */
#define CLE_SLV_AWID_TMO_WIDTH                                                1
#define CLE_SLV_AWID_TMO_SHIFT                                                1
#define CLE_SLV_AWID_TMO_MASK                                        0x00000002
#define CLE_SLV_AWID_TMO_RD(src)                      (((src) & 0x00000002)>>1)
#define CLE_SLV_AWID_TMO_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields SLV_AWRDY_TMO	 */
#define CLE_SLV_AWRDY_TMO_WIDTH                                               1
#define CLE_SLV_AWRDY_TMO_SHIFT                                               0
#define CLE_SLV_AWRDY_TMO_MASK                                       0x00000001
#define CLE_SLV_AWRDY_TMO_RD(src)                        (((src) & 0x00000001))
#define CLE_SLV_AWRDY_TMO_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register CFG_AXI_SS_CSR_TMO	*/ 
/*	 Fields CFG_CSR_TMO	 */
#define CLE_CFG_CSR_TMO_WIDTH                                                16
#define CLE_CFG_CSR_TMO_SHIFT                                                16
#define CLE_CFG_CSR_TMO_MASK                                         0xffff0000
#define CLE_CFG_CSR_TMO_RD(src)                      (((src) & 0xffff0000)>>16)
#define CLE_CFG_CSR_TMO_WR(src)                 (((u32)(src)<<16) & 0xffff0000)
#define CLE_CFG_CSR_TMO_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields CSR_ERR_RESP_EN	 */
#define CLE_CSR_ERR_RESP_EN_WIDTH                                             1
#define CLE_CSR_ERR_RESP_EN_SHIFT                                            15
#define CLE_CSR_ERR_RESP_EN_MASK                                     0x00008000
#define CLE_CSR_ERR_RESP_EN_RD(src)                  (((src) & 0x00008000)>>15)
#define CLE_CSR_ERR_RESP_EN_WR(src)             (((u32)(src)<<15) & 0x00008000)
#define CLE_CSR_ERR_RESP_EN_SET(dst,src) \
                      (((dst) & ~0x00008000) | (((u32)(src)<<15) & 0x00008000))

/*	Global Base Address	*/
#define CLE_QMI_SLAVE_BASE_ADDR				0xddd8e7800ULL

/*    Address QMI_SLAVE  Registers */
#define CLE_CFGSSQMI0_ADDR                                           0x00000000
#define CLE_CFGSSQMI0_DEFAULT                                        0x0002c821
#define CLE_CFGSSQMI1_ADDR                                           0x00000004
#define CLE_CFGSSQMI1_DEFAULT                                        0x2de8b77c
#define CLE_CFGSSQMIFPBUFFER_ADDR                                    0x00000008
#define CLE_CFGSSQMIFPBUFFER_DEFAULT                                 0x00000000
#define CLE_CFGSSQMIWQBUFFER_ADDR                                    0x0000000c
#define CLE_CFGSSQMIWQBUFFER_DEFAULT                                 0x00000000
#define CLE_STSSSQMIFPBUFFER0_ADDR                                   0x00000010
#define CLE_STSSSQMIFPBUFFER0_DEFAULT                                0x00000000
#define CLE_STSSSQMIFPBUFFER1_ADDR                                   0x00000014
#define CLE_STSSSQMIFPBUFFER1_DEFAULT                                0x00000000
#define CLE_STSSSQMIFPBUFFER2_ADDR                                   0x00000018
#define CLE_STSSSQMIFPBUFFER2_DEFAULT                                0x00000000
#define CLE_STSSSQMIWQBUFFER0_ADDR                                   0x0000001c
#define CLE_STSSSQMIWQBUFFER0_DEFAULT                                0x00000000
#define CLE_STSSSQMIWQBUFFER1_ADDR                                   0x00000020
#define CLE_STSSSQMIWQBUFFER1_DEFAULT                                0x00000000
#define CLE_STSSSQMIWQBUFFER2_ADDR                                   0x00000024
#define CLE_STSSSQMIWQBUFFER2_DEFAULT                                0x00000000
#define CLE_STSSSQMIWQBUFFER3_ADDR                                   0x00000028
#define CLE_STSSSQMIWQBUFFER3_DEFAULT                                0x00000000
#define CLE_CFGSSQMISAB_ADDR                                         0x0000002c
#define CLE_CFGSSQMISAB_DEFAULT                                      0x00000000
#define CLE_CFGSSQMISAB0_ADDR                                        0x00000030
#define CLE_CFGSSQMISAB0_DEFAULT                                     0x00000000
#define CLE_CFGSSQMISAB1_ADDR                                        0x00000034
#define CLE_CFGSSQMISAB1_DEFAULT                                     0x00000000
#define CLE_CFGSSQMISAB2_ADDR                                        0x00000038
#define CLE_CFGSSQMISAB2_DEFAULT                                     0x00000000
#define CLE_CFGSSQMISAB3_ADDR                                        0x0000003c
#define CLE_CFGSSQMISAB3_DEFAULT                                     0x00000000
#define CLE_CFGSSQMISAB4_ADDR                                        0x00000040
#define CLE_CFGSSQMISAB4_DEFAULT                                     0x00000000
#define CLE_STSSSQMIINT0_ADDR                                        0x00000044
#define CLE_STSSSQMIINT0_DEFAULT                                     0x00000000
#define CLE_STSSSQMIINT0MASK_ADDR                                    0x00000048
#define CLE_STSSSQMIINT1_ADDR                                        0x0000004c
#define CLE_STSSSQMIINT1_DEFAULT                                     0x00000000
#define CLE_STSSSQMIINT1MASK_ADDR                                    0x00000050
#define CLE_STSSSQMIINT2_ADDR                                        0x00000054
#define CLE_STSSSQMIINT2_DEFAULT                                     0x00000000
#define CLE_STSSSQMIINT2MASK_ADDR                                    0x00000058
#define CLE_STSSSQMIINT3_ADDR                                        0x0000005c
#define CLE_STSSSQMIINT3_DEFAULT                                     0x00000000
#define CLE_STSSSQMIINT3MASK_ADDR                                    0x00000060
#define CLE_STSSSQMIINT4_ADDR                                        0x00000064
#define CLE_STSSSQMIINT4_DEFAULT                                     0x00000000
#define CLE_STSSSQMIINT4MASK_ADDR                                    0x00000068
#define CLE_CFGSSQMIDBGCTRL_ADDR                                     0x0000006c
#define CLE_CFGSSQMIDBGCTRL_DEFAULT                                  0x00000000
#define CLE_CFGSSQMIDBGDATA0_ADDR                                    0x00000070
#define CLE_CFGSSQMIDBGDATA0_DEFAULT                                 0x00000000
#define CLE_CFGSSQMIDBGDATA1_ADDR                                    0x00000074
#define CLE_CFGSSQMIDBGDATA1_DEFAULT                                 0x00000000
#define CLE_CFGSSQMIDBGDATA2_ADDR                                    0x00000078
#define CLE_CFGSSQMIDBGDATA2_DEFAULT                                 0x00000000
#define CLE_CFGSSQMIDBGDATA3_ADDR                                    0x0000007c
#define CLE_CFGSSQMIDBGDATA3_DEFAULT                                 0x00000000
#define CLE_STSSSQMIDBGDATA_ADDR                                     0x00000080
#define CLE_STSSSQMIDBGDATA_DEFAULT                                  0x00000000
#define CLE_CFGSSQMILITE_ADDR                                        0x00000084
#define CLE_CFGSSQMILITE_DEFAULT                                     0x00003400
#define CLE_CFGSSQMIFPQASSOC_ADDR                                    0x00000088
#define CLE_CFGSSQMIFPQASSOC_DEFAULT                                 0x00000000
#define CLE_CFGSSQMIWQASSOC_ADDR                                     0x0000008c
#define CLE_CFGSSQMIWQASSOC_DEFAULT                                  0x00000000
#define CLE_CFGSSQMIMEMORY_ADDR                                      0x00000090
#define CLE_CFGSSQMIMEMORY_DEFAULT                                   0x00000000
#define CLE_STSSSQMIFIFO_ADDR                                        0x00000094
#define CLE_STSSSQMIFIFO_DEFAULT                                     0x07ffffff

/*	Register CfgSsQmi0	*/ 
/*	 Fields WQ16Disable	 */
#define CLE_WQ16DISABLE0_WIDTH                                                1
#define CLE_WQ16DISABLE0_SHIFT                                               29
#define CLE_WQ16DISABLE0_MASK                                        0x20000000
#define CLE_WQ16DISABLE0_RD(src)                     (((src) & 0x20000000)>>29)
#define CLE_WQ16DISABLE0_WR(src)                (((u32)(src)<<29) & 0x20000000)
#define CLE_WQ16DISABLE0_SET(dst,src) \
                      (((dst) & ~0x20000000) | (((u32)(src)<<29) & 0x20000000))
/*	 Fields WQ16Reset	 */
#define CLE_WQ16RESET0_WIDTH                                                  1
#define CLE_WQ16RESET0_SHIFT                                                 28
#define CLE_WQ16RESET0_MASK                                          0x10000000
#define CLE_WQ16RESET0_RD(src)                       (((src) & 0x10000000)>>28)
#define CLE_WQ16RESET0_WR(src)                  (((u32)(src)<<28) & 0x10000000)
#define CLE_WQ16RESET0_SET(dst,src) \
                      (((dst) & ~0x10000000) | (((u32)(src)<<28) & 0x10000000))
/*	 Fields FP17Disable	 */
#define CLE_FP17DISABLE0_WIDTH                                                1
#define CLE_FP17DISABLE0_SHIFT                                               27
#define CLE_FP17DISABLE0_MASK                                        0x08000000
#define CLE_FP17DISABLE0_RD(src)                     (((src) & 0x08000000)>>27)
#define CLE_FP17DISABLE0_WR(src)                (((u32)(src)<<27) & 0x08000000)
#define CLE_FP17DISABLE0_SET(dst,src) \
                      (((dst) & ~0x08000000) | (((u32)(src)<<27) & 0x08000000))
/*	 Fields FP16Disable	 */
#define CLE_FP16DISABLE0_WIDTH                                                1
#define CLE_FP16DISABLE0_SHIFT                                               26
#define CLE_FP16DISABLE0_MASK                                        0x04000000
#define CLE_FP16DISABLE0_RD(src)                     (((src) & 0x04000000)>>26)
#define CLE_FP16DISABLE0_WR(src)                (((u32)(src)<<26) & 0x04000000)
#define CLE_FP16DISABLE0_SET(dst,src) \
                      (((dst) & ~0x04000000) | (((u32)(src)<<26) & 0x04000000))
/*	 Fields FP17Reset	 */
#define CLE_FP17RESET0_WIDTH                                                  1
#define CLE_FP17RESET0_SHIFT                                                 25
#define CLE_FP17RESET0_MASK                                          0x02000000
#define CLE_FP17RESET0_RD(src)                       (((src) & 0x02000000)>>25)
#define CLE_FP17RESET0_WR(src)                  (((u32)(src)<<25) & 0x02000000)
#define CLE_FP17RESET0_SET(dst,src) \
                      (((dst) & ~0x02000000) | (((u32)(src)<<25) & 0x02000000))
/*	 Fields FP16Reset	 */
#define CLE_FP16RESET0_WIDTH                                                  1
#define CLE_FP16RESET0_SHIFT                                                 24
#define CLE_FP16RESET0_MASK                                          0x01000000
#define CLE_FP16RESET0_RD(src)                       (((src) & 0x01000000)>>24)
#define CLE_FP16RESET0_WR(src)                  (((u32)(src)<<24) & 0x01000000)
#define CLE_FP16RESET0_SET(dst,src) \
                      (((dst) & ~0x01000000) | (((u32)(src)<<24) & 0x01000000))
/*	 Fields FPDecDiffThreshold	 */
#define CLE_FPDECDIFFTHRESHOLD0_WIDTH                                         3
#define CLE_FPDECDIFFTHRESHOLD0_SHIFT                                        21
#define CLE_FPDECDIFFTHRESHOLD0_MASK                                 0x00e00000
#define CLE_FPDECDIFFTHRESHOLD0_RD(src)              (((src) & 0x00e00000)>>21)
#define CLE_FPDECDIFFTHRESHOLD0_WR(src)         (((u32)(src)<<21) & 0x00e00000)
#define CLE_FPDECDIFFTHRESHOLD0_SET(dst,src) \
                      (((dst) & ~0x00e00000) | (((u32)(src)<<21) & 0x00e00000))
/*	 Fields WQDecDiffThreshold	 */
#define CLE_WQDECDIFFTHRESHOLD0_WIDTH                                         4
#define CLE_WQDECDIFFTHRESHOLD0_SHIFT                                        17
#define CLE_WQDECDIFFTHRESHOLD0_MASK                                 0x001e0000
#define CLE_WQDECDIFFTHRESHOLD0_RD(src)              (((src) & 0x001e0000)>>17)
#define CLE_WQDECDIFFTHRESHOLD0_WR(src)         (((u32)(src)<<17) & 0x001e0000)
#define CLE_WQDECDIFFTHRESHOLD0_SET(dst,src) \
                      (((dst) & ~0x001e0000) | (((u32)(src)<<17) & 0x001e0000))
/*	 Fields DeallocThreshold	 */
#define CLE_DEALLOCTHRESHOLD0_WIDTH                                           3
#define CLE_DEALLOCTHRESHOLD0_SHIFT                                          14
#define CLE_DEALLOCTHRESHOLD0_MASK                                   0x0001c000
#define CLE_DEALLOCTHRESHOLD0_RD(src)                (((src) & 0x0001c000)>>14)
#define CLE_DEALLOCTHRESHOLD0_WR(src)           (((u32)(src)<<14) & 0x0001c000)
#define CLE_DEALLOCTHRESHOLD0_SET(dst,src) \
                      (((dst) & ~0x0001c000) | (((u32)(src)<<14) & 0x0001c000))
/*	 Fields FPDecThreshold	 */
#define CLE_FPDECTHRESHOLD0_WIDTH                                             3
#define CLE_FPDECTHRESHOLD0_SHIFT                                            11
#define CLE_FPDECTHRESHOLD0_MASK                                     0x00003800
#define CLE_FPDECTHRESHOLD0_RD(src)                  (((src) & 0x00003800)>>11)
#define CLE_FPDECTHRESHOLD0_WR(src)             (((u32)(src)<<11) & 0x00003800)
#define CLE_FPDECTHRESHOLD0_SET(dst,src) \
                      (((dst) & ~0x00003800) | (((u32)(src)<<11) & 0x00003800))
/*	 Fields FPBAvlThreshold	 */
#define CLE_FPBAVLTHRESHOLD0_WIDTH                                            3
#define CLE_FPBAVLTHRESHOLD0_SHIFT                                            8
#define CLE_FPBAVLTHRESHOLD0_MASK                                    0x00000700
#define CLE_FPBAVLTHRESHOLD0_RD(src)                  (((src) & 0x00000700)>>8)
#define CLE_FPBAVLTHRESHOLD0_WR(src)             (((u32)(src)<<8) & 0x00000700)
#define CLE_FPBAVLTHRESHOLD0_SET(dst,src) \
                       (((dst) & ~0x00000700) | (((u32)(src)<<8) & 0x00000700))
/*	 Fields WQDecThreshold	 */
#define CLE_WQDECTHRESHOLD0_WIDTH                                             4
#define CLE_WQDECTHRESHOLD0_SHIFT                                             4
#define CLE_WQDECTHRESHOLD0_MASK                                     0x000000f0
#define CLE_WQDECTHRESHOLD0_RD(src)                   (((src) & 0x000000f0)>>4)
#define CLE_WQDECTHRESHOLD0_WR(src)              (((u32)(src)<<4) & 0x000000f0)
#define CLE_WQDECTHRESHOLD0_SET(dst,src) \
                       (((dst) & ~0x000000f0) | (((u32)(src)<<4) & 0x000000f0))
/*	 Fields WQBAvlThreshold	 */
#define CLE_WQBAVLTHRESHOLD0_WIDTH                                            4
#define CLE_WQBAVLTHRESHOLD0_SHIFT                                            0
#define CLE_WQBAVLTHRESHOLD0_MASK                                    0x0000000f
#define CLE_WQBAVLTHRESHOLD0_RD(src)                     (((src) & 0x0000000f))
#define CLE_WQBAVLTHRESHOLD0_WR(src)                (((u32)(src)) & 0x0000000f)
#define CLE_WQBAVLTHRESHOLD0_SET(dst,src) \
                          (((dst) & ~0x0000000f) | (((u32)(src)) & 0x0000000f))

/*	Register CfgSsQmi1	*/ 
/*	 Fields CmOverrideLLFields	 */
#define CLE_CMOVERRIDELLFIELDS1_WIDTH                                         1
#define CLE_CMOVERRIDELLFIELDS1_SHIFT                                        29
#define CLE_CMOVERRIDELLFIELDS1_MASK                                 0x20000000
#define CLE_CMOVERRIDELLFIELDS1_RD(src)              (((src) & 0x20000000)>>29)
#define CLE_CMOVERRIDELLFIELDS1_WR(src)         (((u32)(src)<<29) & 0x20000000)
#define CLE_CMOVERRIDELLFIELDS1_SET(dst,src) \
                      (((dst) & ~0x20000000) | (((u32)(src)<<29) & 0x20000000))
/*	 Fields CmCtrlbuffThreshold	 */
#define CLE_CMCTRLBUFFTHRESHOLD1_WIDTH                                        3
#define CLE_CMCTRLBUFFTHRESHOLD1_SHIFT                                       26
#define CLE_CMCTRLBUFFTHRESHOLD1_MASK                                0x1c000000
#define CLE_CMCTRLBUFFTHRESHOLD1_RD(src)             (((src) & 0x1c000000)>>26)
#define CLE_CMCTRLBUFFTHRESHOLD1_WR(src)        (((u32)(src)<<26) & 0x1c000000)
#define CLE_CMCTRLBUFFTHRESHOLD1_SET(dst,src) \
                      (((dst) & ~0x1c000000) | (((u32)(src)<<26) & 0x1c000000))
/*	 Fields CmDatabuffThreshold	 */
#define CLE_CMDATABUFFTHRESHOLD1_WIDTH                                        5
#define CLE_CMDATABUFFTHRESHOLD1_SHIFT                                       21
#define CLE_CMDATABUFFTHRESHOLD1_MASK                                0x03e00000
#define CLE_CMDATABUFFTHRESHOLD1_RD(src)             (((src) & 0x03e00000)>>21)
#define CLE_CMDATABUFFTHRESHOLD1_WR(src)        (((u32)(src)<<21) & 0x03e00000)
#define CLE_CMDATABUFFTHRESHOLD1_SET(dst,src) \
                      (((dst) & ~0x03e00000) | (((u32)(src)<<21) & 0x03e00000))
/*	 Fields CmMsgfThreshold	 */
#define CLE_CMMSGFTHRESHOLD1_WIDTH                                            4
#define CLE_CMMSGFTHRESHOLD1_SHIFT                                           17
#define CLE_CMMSGFTHRESHOLD1_MASK                                    0x001e0000
#define CLE_CMMSGFTHRESHOLD1_RD(src)                 (((src) & 0x001e0000)>>17)
#define CLE_CMMSGFTHRESHOLD1_WR(src)            (((u32)(src)<<17) & 0x001e0000)
#define CLE_CMMSGFTHRESHOLD1_SET(dst,src) \
                      (((dst) & ~0x001e0000) | (((u32)(src)<<17) & 0x001e0000))
/*	 Fields CmRegfThreshold	 */
#define CLE_CMREGFTHRESHOLD1_WIDTH                                            3
#define CLE_CMREGFTHRESHOLD1_SHIFT                                           14
#define CLE_CMREGFTHRESHOLD1_MASK                                    0x0001c000
#define CLE_CMREGFTHRESHOLD1_RD(src)                 (((src) & 0x0001c000)>>14)
#define CLE_CMREGFTHRESHOLD1_WR(src)            (((u32)(src)<<14) & 0x0001c000)
#define CLE_CMREGFTHRESHOLD1_SET(dst,src) \
                      (((dst) & ~0x0001c000) | (((u32)(src)<<14) & 0x0001c000))
/*	 Fields QMDeviceAddress	 */
#define CLE_QMDEVICEADDRESS1_WIDTH                                           14
#define CLE_QMDEVICEADDRESS1_SHIFT                                            0
#define CLE_QMDEVICEADDRESS1_MASK                                    0x00003fff
#define CLE_QMDEVICEADDRESS1_RD(src)                     (((src) & 0x00003fff))
#define CLE_QMDEVICEADDRESS1_WR(src)                (((u32)(src)) & 0x00003fff)
#define CLE_QMDEVICEADDRESS1_SET(dst,src) \
                          (((dst) & ~0x00003fff) | (((u32)(src)) & 0x00003fff))

/*	Register CfgSsQmiFPBuffer	*/ 
/*	 Fields Disable	 */
#define CLE_DISABLE_WIDTH                                                    16
#define CLE_DISABLE_SHIFT                                                    16
#define CLE_DISABLE_MASK                                             0xffff0000
#define CLE_DISABLE_RD(src)                          (((src) & 0xffff0000)>>16)
#define CLE_DISABLE_WR(src)                     (((u32)(src)<<16) & 0xffff0000)
#define CLE_DISABLE_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields Reset	 */
#define CLE_RESET_WIDTH                                                      16
#define CLE_RESET_SHIFT                                                       0
#define CLE_RESET_MASK                                               0x0000ffff
#define CLE_RESET_RD(src)                                (((src) & 0x0000ffff))
#define CLE_RESET_WR(src)                           (((u32)(src)) & 0x0000ffff)
#define CLE_RESET_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register CfgSsQmiWQBuffer	*/ 
/*	 Fields Disable	 */
#define CLE_DISABLE_F1_WIDTH                                                 16
#define CLE_DISABLE_F1_SHIFT                                                 16
#define CLE_DISABLE_F1_MASK                                          0xffff0000
#define CLE_DISABLE_F1_RD(src)                       (((src) & 0xffff0000)>>16)
#define CLE_DISABLE_F1_WR(src)                  (((u32)(src)<<16) & 0xffff0000)
#define CLE_DISABLE_F1_SET(dst,src) \
                      (((dst) & ~0xffff0000) | (((u32)(src)<<16) & 0xffff0000))
/*	 Fields Reset	 */
#define CLE_RESET_F1_WIDTH                                                   16
#define CLE_RESET_F1_SHIFT                                                    0
#define CLE_RESET_F1_MASK                                            0x0000ffff
#define CLE_RESET_F1_RD(src)                             (((src) & 0x0000ffff))
#define CLE_RESET_F1_WR(src)                        (((u32)(src)) & 0x0000ffff)
#define CLE_RESET_F1_SET(dst,src) \
                          (((dst) & ~0x0000ffff) | (((u32)(src)) & 0x0000ffff))

/*	Register StsSsQmiFPBuffer0	*/ 
/*	 Fields WritePointer	 */
#define CLE_WRITEPOINTER0_WIDTH                                              32
#define CLE_WRITEPOINTER0_SHIFT                                               0
#define CLE_WRITEPOINTER0_MASK                                       0xffffffff
#define CLE_WRITEPOINTER0_RD(src)                        (((src) & 0xffffffff))
#define CLE_WRITEPOINTER0_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register StsSsQmiFPBuffer1	*/ 
/*	 Fields WritePointer	 */
#define CLE_WRITEPOINTER1_WIDTH                                               4
#define CLE_WRITEPOINTER1_SHIFT                                              24
#define CLE_WRITEPOINTER1_MASK                                       0x0f000000
#define CLE_WRITEPOINTER1_RD(src)                    (((src) & 0x0f000000)>>24)
#define CLE_WRITEPOINTER1_SET(dst,src) \
                      (((dst) & ~0x0f000000) | (((u32)(src)<<24) & 0x0f000000))
/*	 Fields Entries	 */
#define CLE_ENTRIES1_WIDTH                                                   24
#define CLE_ENTRIES1_SHIFT                                                    0
#define CLE_ENTRIES1_MASK                                            0x00ffffff
#define CLE_ENTRIES1_RD(src)                             (((src) & 0x00ffffff))
#define CLE_ENTRIES1_SET(dst,src) \
                          (((dst) & ~0x00ffffff) | (((u32)(src)) & 0x00ffffff))

/*	Register StsSsQmiFPBuffer2	*/ 
/*	 Fields Entries	 */
#define CLE_ENTRIES2_WIDTH                                                   30
#define CLE_ENTRIES2_SHIFT                                                    0
#define CLE_ENTRIES2_MASK                                            0x3fffffff
#define CLE_ENTRIES2_RD(src)                             (((src) & 0x3fffffff))
#define CLE_ENTRIES2_SET(dst,src) \
                          (((dst) & ~0x3fffffff) | (((u32)(src)) & 0x3fffffff))

/*	Register StsSsQmiWQBuffer0	*/ 
/*	 Fields WritePointer	 */
#define CLE_WRITEPOINTER0_F1_WIDTH                                           24
#define CLE_WRITEPOINTER0_F1_SHIFT                                            0
#define CLE_WRITEPOINTER0_F1_MASK                                    0x00ffffff
#define CLE_WRITEPOINTER0_F1_RD(src)                     (((src) & 0x00ffffff))
#define CLE_WRITEPOINTER0_F1_SET(dst,src) \
                          (((dst) & ~0x00ffffff) | (((u32)(src)) & 0x00ffffff))

/*	Register StsSsQmiWQBuffer1	*/ 
/*	 Fields Entries	 */
#define CLE_ENTRIES1_F1_WIDTH                                                 4
#define CLE_ENTRIES1_F1_SHIFT                                                27
#define CLE_ENTRIES1_F1_MASK                                         0x78000000
#define CLE_ENTRIES1_F1_RD(src)                      (((src) & 0x78000000)>>27)
#define CLE_ENTRIES1_F1_SET(dst,src) \
                      (((dst) & ~0x78000000) | (((u32)(src)<<27) & 0x78000000))
/*	 Fields WritePointer	 */
#define CLE_WRITEPOINTER1_F1_WIDTH                                           27
#define CLE_WRITEPOINTER1_F1_SHIFT                                            0
#define CLE_WRITEPOINTER1_F1_MASK                                    0x07ffffff
#define CLE_WRITEPOINTER1_F1_RD(src)                     (((src) & 0x07ffffff))
#define CLE_WRITEPOINTER1_F1_SET(dst,src) \
                          (((dst) & ~0x07ffffff) | (((u32)(src)) & 0x07ffffff))

/*	Register StsSsQmiWQBuffer2	*/ 
/*	 Fields Entries	 */
#define CLE_ENTRIES2_F1_WIDTH                                                32
#define CLE_ENTRIES2_F1_SHIFT                                                 0
#define CLE_ENTRIES2_F1_MASK                                         0xffffffff
#define CLE_ENTRIES2_F1_RD(src)                          (((src) & 0xffffffff))
#define CLE_ENTRIES2_F1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register StsSsQmiWQBuffer3	*/ 
/*	 Fields Entries	 */
#define CLE_ENTRIES3_WIDTH                                                   32
#define CLE_ENTRIES3_SHIFT                                                    0
#define CLE_ENTRIES3_MASK                                            0xffffffff
#define CLE_ENTRIES3_RD(src)                             (((src) & 0xffffffff))
#define CLE_ENTRIES3_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CfgSsQmiSab	*/ 
/*	 Fields EnableMonitoring	 */
#define CLE_ENABLEMONITORING_WIDTH                                           17
#define CLE_ENABLEMONITORING_SHIFT                                            0
#define CLE_ENABLEMONITORING_MASK                                    0x0001ffff
#define CLE_ENABLEMONITORING_RD(src)                     (((src) & 0x0001ffff))
#define CLE_ENABLEMONITORING_WR(src)                (((u32)(src)) & 0x0001ffff)
#define CLE_ENABLEMONITORING_SET(dst,src) \
                          (((dst) & ~0x0001ffff) | (((u32)(src)) & 0x0001ffff))

/*	Register CfgSsQmiSab0	*/ 
/*	 Fields QID	 */
#define CLE_QID0_WIDTH                                                       32
#define CLE_QID0_SHIFT                                                        0
#define CLE_QID0_MASK                                                0xffffffff
#define CLE_QID0_RD(src)                                 (((src) & 0xffffffff))
#define CLE_QID0_WR(src)                            (((u32)(src)) & 0xffffffff)
#define CLE_QID0_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CfgSsQmiSab1	*/ 
/*	 Fields QID	 */
#define CLE_QID1_WIDTH                                                       32
#define CLE_QID1_SHIFT                                                        0
#define CLE_QID1_MASK                                                0xffffffff
#define CLE_QID1_RD(src)                                 (((src) & 0xffffffff))
#define CLE_QID1_WR(src)                            (((u32)(src)) & 0xffffffff)
#define CLE_QID1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CfgSsQmiSab2	*/ 
/*	 Fields QID	 */
#define CLE_QID2_WIDTH                                                       32
#define CLE_QID2_SHIFT                                                        0
#define CLE_QID2_MASK                                                0xffffffff
#define CLE_QID2_RD(src)                                 (((src) & 0xffffffff))
#define CLE_QID2_WR(src)                            (((u32)(src)) & 0xffffffff)
#define CLE_QID2_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CfgSsQmiSab3	*/ 
/*	 Fields QID	 */
#define CLE_QID3_WIDTH                                                       32
#define CLE_QID3_SHIFT                                                        0
#define CLE_QID3_MASK                                                0xffffffff
#define CLE_QID3_RD(src)                                 (((src) & 0xffffffff))
#define CLE_QID3_WR(src)                            (((u32)(src)) & 0xffffffff)
#define CLE_QID3_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CfgSsQmiSab4	*/ 
/*	 Fields QID	 */
#define CLE_QID4_WIDTH                                                        8
#define CLE_QID4_SHIFT                                                        0
#define CLE_QID4_MASK                                                0x000000ff
#define CLE_QID4_RD(src)                                 (((src) & 0x000000ff))
#define CLE_QID4_WR(src)                            (((u32)(src)) & 0x000000ff)
#define CLE_QID4_SET(dst,src) \
                          (((dst) & ~0x000000ff) | (((u32)(src)) & 0x000000ff))

/*	Register StsSsQmiInt0	*/ 
/*	 Fields FPOverflow	 */
#define CLE_FPOVERFLOW0_WIDTH                                                18
#define CLE_FPOVERFLOW0_SHIFT                                                 0
#define CLE_FPOVERFLOW0_MASK                                         0x0003ffff
#define CLE_FPOVERFLOW0_RD(src)                          (((src) & 0x0003ffff))
#define CLE_FPOVERFLOW0_WR(src)                     (((u32)(src)) & 0x0003ffff)
#define CLE_FPOVERFLOW0_SET(dst,src) \
                          (((dst) & ~0x0003ffff) | (((u32)(src)) & 0x0003ffff))

/*	Register StsSsQmiInt0Mask	*/
/*    Mask Register Fields FPOverflowMask    */
#define CLE_FPOVERFLOWMASK_WIDTH                                             18
#define CLE_FPOVERFLOWMASK_SHIFT                                              0
#define CLE_FPOVERFLOWMASK_MASK                                      0x0003ffff
#define CLE_FPOVERFLOWMASK_RD(src)                       (((src) & 0x0003ffff))
#define CLE_FPOVERFLOWMASK_WR(src)                  (((u32)(src)) & 0x0003ffff)
#define CLE_FPOVERFLOWMASK_SET(dst,src) \
                          (((dst) & ~0x0003ffff) | (((u32)(src)) & 0x0003ffff))

/*	Register StsSsQmiInt1	*/ 
/*	 Fields WQOverflow	 */
#define CLE_WQOVERFLOW1_WIDTH                                                17
#define CLE_WQOVERFLOW1_SHIFT                                                 0
#define CLE_WQOVERFLOW1_MASK                                         0x0001ffff
#define CLE_WQOVERFLOW1_RD(src)                          (((src) & 0x0001ffff))
#define CLE_WQOVERFLOW1_WR(src)                     (((u32)(src)) & 0x0001ffff)
#define CLE_WQOVERFLOW1_SET(dst,src) \
                          (((dst) & ~0x0001ffff) | (((u32)(src)) & 0x0001ffff))

/*	Register StsSsQmiInt1Mask	*/
/*    Mask Register Fields WQOverflowMask    */
#define CLE_WQOVERFLOWMASK_WIDTH                                             17
#define CLE_WQOVERFLOWMASK_SHIFT                                              0
#define CLE_WQOVERFLOWMASK_MASK                                      0x0001ffff
#define CLE_WQOVERFLOWMASK_RD(src)                       (((src) & 0x0001ffff))
#define CLE_WQOVERFLOWMASK_WR(src)                  (((u32)(src)) & 0x0001ffff)
#define CLE_WQOVERFLOWMASK_SET(dst,src) \
                          (((dst) & ~0x0001ffff) | (((u32)(src)) & 0x0001ffff))

/*	Register StsSsQmiInt2	*/ 
/*	 Fields FPUnderRun	 */
#define CLE_FPUNDERRUN2_WIDTH                                                18
#define CLE_FPUNDERRUN2_SHIFT                                                 0
#define CLE_FPUNDERRUN2_MASK                                         0x0003ffff
#define CLE_FPUNDERRUN2_RD(src)                          (((src) & 0x0003ffff))
#define CLE_FPUNDERRUN2_WR(src)                     (((u32)(src)) & 0x0003ffff)
#define CLE_FPUNDERRUN2_SET(dst,src) \
                          (((dst) & ~0x0003ffff) | (((u32)(src)) & 0x0003ffff))

/*	Register StsSsQmiInt2Mask	*/
/*    Mask Register Fields FPUnderRunMask    */
#define CLE_FPUNDERRUNMASK_WIDTH                                             18
#define CLE_FPUNDERRUNMASK_SHIFT                                              0
#define CLE_FPUNDERRUNMASK_MASK                                      0x0003ffff
#define CLE_FPUNDERRUNMASK_RD(src)                       (((src) & 0x0003ffff))
#define CLE_FPUNDERRUNMASK_WR(src)                  (((u32)(src)) & 0x0003ffff)
#define CLE_FPUNDERRUNMASK_SET(dst,src) \
                          (((dst) & ~0x0003ffff) | (((u32)(src)) & 0x0003ffff))

/*	Register StsSsQmiInt3	*/ 
/*	 Fields WQUnderRun	 */
#define CLE_WQUNDERRUN3_WIDTH                                                17
#define CLE_WQUNDERRUN3_SHIFT                                                 0
#define CLE_WQUNDERRUN3_MASK                                         0x0001ffff
#define CLE_WQUNDERRUN3_RD(src)                          (((src) & 0x0001ffff))
#define CLE_WQUNDERRUN3_WR(src)                     (((u32)(src)) & 0x0001ffff)
#define CLE_WQUNDERRUN3_SET(dst,src) \
                          (((dst) & ~0x0001ffff) | (((u32)(src)) & 0x0001ffff))

/*	Register StsSsQmiInt3Mask	*/
/*    Mask Register Fields WQUnderRunMask    */
#define CLE_WQUNDERRUNMASK_WIDTH                                             17
#define CLE_WQUNDERRUNMASK_SHIFT                                              0
#define CLE_WQUNDERRUNMASK_MASK                                      0x0001ffff
#define CLE_WQUNDERRUNMASK_RD(src)                       (((src) & 0x0001ffff))
#define CLE_WQUNDERRUNMASK_WR(src)                  (((u32)(src)) & 0x0001ffff)
#define CLE_WQUNDERRUNMASK_SET(dst,src) \
                          (((dst) & ~0x0001ffff) | (((u32)(src)) & 0x0001ffff))

/*	Register StsSsQmiInt4	*/ 
/*	 Fields axiwcmr_slverr	 */
#define CLE_AXIWCMR_SLVERR4_WIDTH                                             1
#define CLE_AXIWCMR_SLVERR4_SHIFT                                             1
#define CLE_AXIWCMR_SLVERR4_MASK                                     0x00000002
#define CLE_AXIWCMR_SLVERR4_RD(src)                   (((src) & 0x00000002)>>1)
#define CLE_AXIWCMR_SLVERR4_WR(src)              (((u32)(src)<<1) & 0x00000002)
#define CLE_AXIWCMR_SLVERR4_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields axiwcmr_decerr	 */
#define CLE_AXIWCMR_DECERR4_WIDTH                                             1
#define CLE_AXIWCMR_DECERR4_SHIFT                                             0
#define CLE_AXIWCMR_DECERR4_MASK                                     0x00000001
#define CLE_AXIWCMR_DECERR4_RD(src)                      (((src) & 0x00000001))
#define CLE_AXIWCMR_DECERR4_WR(src)                 (((u32)(src)) & 0x00000001)
#define CLE_AXIWCMR_DECERR4_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register StsSsQmiInt4Mask	*/
/*    Mask Register Fields axiwcmr_slverrMask    */
#define CLE_AXIWCMR_SLVERRMASK_WIDTH                                          1
#define CLE_AXIWCMR_SLVERRMASK_SHIFT                                          1
#define CLE_AXIWCMR_SLVERRMASK_MASK                                  0x00000002
#define CLE_AXIWCMR_SLVERRMASK_RD(src)                (((src) & 0x00000002)>>1)
#define CLE_AXIWCMR_SLVERRMASK_WR(src)           (((u32)(src)<<1) & 0x00000002)
#define CLE_AXIWCMR_SLVERRMASK_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*    Mask Register Fields axiwcmr_decerrMask    */
#define CLE_AXIWCMR_DECERRMASK_WIDTH                                          1
#define CLE_AXIWCMR_DECERRMASK_SHIFT                                          0
#define CLE_AXIWCMR_DECERRMASK_MASK                                  0x00000001
#define CLE_AXIWCMR_DECERRMASK_RD(src)                   (((src) & 0x00000001))
#define CLE_AXIWCMR_DECERRMASK_WR(src)              (((u32)(src)) & 0x00000001)
#define CLE_AXIWCMR_DECERRMASK_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register CfgSsQmiDbgCtrl	*/ 
/*	 Fields PBID	 */
#define CLE_PBID_WIDTH                                                        6
#define CLE_PBID_SHIFT                                                       11
#define CLE_PBID_MASK                                                0x0001f800
#define CLE_PBID_RD(src)                             (((src) & 0x0001f800)>>11)
#define CLE_PBID_WR(src)                        (((u32)(src)<<11) & 0x0001f800)
#define CLE_PBID_SET(dst,src) \
                      (((dst) & ~0x0001f800) | (((u32)(src)<<11) & 0x0001f800))
/*	 Fields nAck	 */
#define CLE_NACK_WIDTH                                                        1
#define CLE_NACK_SHIFT                                                       10
#define CLE_NACK_MASK                                                0x00000400
#define CLE_NACK_RD(src)                             (((src) & 0x00000400)>>10)
#define CLE_NACK_WR(src)                        (((u32)(src)<<10) & 0x00000400)
#define CLE_NACK_SET(dst,src) \
                      (((dst) & ~0x00000400) | (((u32)(src)<<10) & 0x00000400))
/*	 Fields last	 */
#define CLE_LAST_WIDTH                                                        1
#define CLE_LAST_SHIFT                                                        9
#define CLE_LAST_MASK                                                0x00000200
#define CLE_LAST_RD(src)                              (((src) & 0x00000200)>>9)
#define CLE_LAST_WR(src)                         (((u32)(src)<<9) & 0x00000200)
#define CLE_LAST_SET(dst,src) \
                       (((dst) & ~0x00000200) | (((u32)(src)<<9) & 0x00000200))
/*	 Fields Push	 */
#define CLE_PUSH_WIDTH                                                        1
#define CLE_PUSH_SHIFT                                                        8
#define CLE_PUSH_MASK                                                0x00000100
#define CLE_PUSH_RD(src)                              (((src) & 0x00000100)>>8)
#define CLE_PUSH_WR(src)                         (((u32)(src)<<8) & 0x00000100)
#define CLE_PUSH_SET(dst,src) \
                       (((dst) & ~0x00000100) | (((u32)(src)<<8) & 0x00000100))
/*	 Fields Write	 */
#define CLE_WRITE_WIDTH                                                       1
#define CLE_WRITE_SHIFT                                                       7
#define CLE_WRITE_MASK                                               0x00000080
#define CLE_WRITE_RD(src)                             (((src) & 0x00000080)>>7)
#define CLE_WRITE_WR(src)                        (((u32)(src)<<7) & 0x00000080)
#define CLE_WRITE_SET(dst,src) \
                       (((dst) & ~0x00000080) | (((u32)(src)<<7) & 0x00000080))
/*	 Fields Pop	 */
#define CLE_POP_WIDTH                                                         1
#define CLE_POP_SHIFT                                                         6
#define CLE_POP_MASK                                                 0x00000040
#define CLE_POP_RD(src)                               (((src) & 0x00000040)>>6)
#define CLE_POP_WR(src)                          (((u32)(src)<<6) & 0x00000040)
#define CLE_POP_SET(dst,src) \
                       (((dst) & ~0x00000040) | (((u32)(src)<<6) & 0x00000040))
/*	 Fields Read	 */
#define CLE_READ_WIDTH                                                        1
#define CLE_READ_SHIFT                                                        5
#define CLE_READ_MASK                                                0x00000020
#define CLE_READ_RD(src)                              (((src) & 0x00000020)>>5)
#define CLE_READ_WR(src)                         (((u32)(src)<<5) & 0x00000020)
#define CLE_READ_SET(dst,src) \
                       (((dst) & ~0x00000020) | (((u32)(src)<<5) & 0x00000020))
/*	 Fields BufferAddr	 */
#define CLE_BUFFERADDR_WIDTH                                                  5
#define CLE_BUFFERADDR_SHIFT                                                  0
#define CLE_BUFFERADDR_MASK                                          0x0000001f
#define CLE_BUFFERADDR_RD(src)                           (((src) & 0x0000001f))
#define CLE_BUFFERADDR_WR(src)                      (((u32)(src)) & 0x0000001f)
#define CLE_BUFFERADDR_SET(dst,src) \
                          (((dst) & ~0x0000001f) | (((u32)(src)) & 0x0000001f))

/*	Register CfgSsQmiDbgData0	*/ 
/*	 Fields Data	 */
#define CLE_DATA0_WIDTH                                                      32
#define CLE_DATA0_SHIFT                                                       0
#define CLE_DATA0_MASK                                               0xffffffff
#define CLE_DATA0_RD(src)                                (((src) & 0xffffffff))
#define CLE_DATA0_WR(src)                           (((u32)(src)) & 0xffffffff)
#define CLE_DATA0_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CfgSsQmiDbgData1	*/ 
/*	 Fields Data	 */
#define CLE_DATA1_WIDTH                                                      32
#define CLE_DATA1_SHIFT                                                       0
#define CLE_DATA1_MASK                                               0xffffffff
#define CLE_DATA1_RD(src)                                (((src) & 0xffffffff))
#define CLE_DATA1_WR(src)                           (((u32)(src)) & 0xffffffff)
#define CLE_DATA1_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CfgSsQmiDbgData2	*/ 
/*	 Fields Data	 */
#define CLE_DATA2_WIDTH                                                      32
#define CLE_DATA2_SHIFT                                                       0
#define CLE_DATA2_MASK                                               0xffffffff
#define CLE_DATA2_RD(src)                                (((src) & 0xffffffff))
#define CLE_DATA2_WR(src)                           (((u32)(src)) & 0xffffffff)
#define CLE_DATA2_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CfgSsQmiDbgData3	*/ 
/*	 Fields Data	 */
#define CLE_DATA3_WIDTH                                                      32
#define CLE_DATA3_SHIFT                                                       0
#define CLE_DATA3_MASK                                               0xffffffff
#define CLE_DATA3_RD(src)                                (((src) & 0xffffffff))
#define CLE_DATA3_WR(src)                           (((u32)(src)) & 0xffffffff)
#define CLE_DATA3_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register StsSsQmiDbgData	*/ 
/*	 Fields Data	 */
#define CLE_DATA_WIDTH                                                       32
#define CLE_DATA_SHIFT                                                        0
#define CLE_DATA_MASK                                                0xffffffff
#define CLE_DATA_RD(src)                                 (((src) & 0xffffffff))
#define CLE_DATA_SET(dst,src) \
                          (((dst) & ~0xffffffff) | (((u32)(src)) & 0xffffffff))

/*	Register CfgSsQmiLite	*/ 
/*	 Fields QMLiteDeviceAddress	 */
#define CLE_QMLITEDEVICEADDRESS_WIDTH                                        15
#define CLE_QMLITEDEVICEADDRESS_SHIFT                                         0
#define CLE_QMLITEDEVICEADDRESS_MASK                                 0x00007fff
#define CLE_QMLITEDEVICEADDRESS_RD(src)                  (((src) & 0x00007fff))
#define CLE_QMLITEDEVICEADDRESS_WR(src)             (((u32)(src)) & 0x00007fff)
#define CLE_QMLITEDEVICEADDRESS_SET(dst,src) \
                          (((dst) & ~0x00007fff) | (((u32)(src)) & 0x00007fff))

/*	Register CfgSsQmiFPQAssoc	*/ 
/*	 Fields Association	 */
#define CLE_ASSOCIATION_WIDTH                                                18
#define CLE_ASSOCIATION_SHIFT                                                 0
#define CLE_ASSOCIATION_MASK                                         0x0003ffff
#define CLE_ASSOCIATION_RD(src)                          (((src) & 0x0003ffff))
#define CLE_ASSOCIATION_WR(src)                     (((u32)(src)) & 0x0003ffff)
#define CLE_ASSOCIATION_SET(dst,src) \
                          (((dst) & ~0x0003ffff) | (((u32)(src)) & 0x0003ffff))

/*	Register CfgSsQmiWQAssoc	*/ 
/*	 Fields Association	 */
#define CLE_ASSOCIATION_F1_WIDTH                                             17
#define CLE_ASSOCIATION_F1_SHIFT                                              0
#define CLE_ASSOCIATION_F1_MASK                                      0x0001ffff
#define CLE_ASSOCIATION_F1_RD(src)                       (((src) & 0x0001ffff))
#define CLE_ASSOCIATION_F1_WR(src)                  (((u32)(src)) & 0x0001ffff)
#define CLE_ASSOCIATION_F1_SET(dst,src) \
                          (((dst) & ~0x0001ffff) | (((u32)(src)) & 0x0001ffff))

/*	Register CfgSsQmiMemory	*/ 
/*	 Fields RMA	 */
#define CLE_RMA_WIDTH                                                         2
#define CLE_RMA_SHIFT                                                         4
#define CLE_RMA_MASK                                                 0x00000030
#define CLE_RMA_RD(src)                               (((src) & 0x00000030)>>4)
#define CLE_RMA_WR(src)                          (((u32)(src)<<4) & 0x00000030)
#define CLE_RMA_SET(dst,src) \
                       (((dst) & ~0x00000030) | (((u32)(src)<<4) & 0x00000030))
/*	 Fields RMB	 */
#define CLE_RMB_WIDTH                                                         2
#define CLE_RMB_SHIFT                                                         2
#define CLE_RMB_MASK                                                 0x0000000c
#define CLE_RMB_RD(src)                               (((src) & 0x0000000c)>>2)
#define CLE_RMB_WR(src)                          (((u32)(src)<<2) & 0x0000000c)
#define CLE_RMB_SET(dst,src) \
                       (((dst) & ~0x0000000c) | (((u32)(src)<<2) & 0x0000000c))
/*	 Fields RMEA	 */
#define CLE_RMEA_WIDTH                                                        1
#define CLE_RMEA_SHIFT                                                        1
#define CLE_RMEA_MASK                                                0x00000002
#define CLE_RMEA_RD(src)                              (((src) & 0x00000002)>>1)
#define CLE_RMEA_WR(src)                         (((u32)(src)<<1) & 0x00000002)
#define CLE_RMEA_SET(dst,src) \
                       (((dst) & ~0x00000002) | (((u32)(src)<<1) & 0x00000002))
/*	 Fields RMEB	 */
#define CLE_RMEB_WIDTH                                                        1
#define CLE_RMEB_SHIFT                                                        0
#define CLE_RMEB_MASK                                                0x00000001
#define CLE_RMEB_RD(src)                                 (((src) & 0x00000001))
#define CLE_RMEB_WR(src)                            (((u32)(src)) & 0x00000001)
#define CLE_RMEB_SET(dst,src) \
                          (((dst) & ~0x00000001) | (((u32)(src)) & 0x00000001))

/*	Register StsSsQmiFifo	*/ 
/*	 Fields empty	 */
#define CLE_EMPTY_WIDTH                                                      27
#define CLE_EMPTY_SHIFT                                                       0
#define CLE_EMPTY_MASK                                               0x07ffffff
#define CLE_EMPTY_RD(src)                                (((src) & 0x07ffffff))
#define CLE_EMPTY_SET(dst,src) \
                          (((dst) & ~0x07ffffff) | (((u32)(src)) & 0x07ffffff))
	


#endif  //_MB_CLE_CSR_H__
